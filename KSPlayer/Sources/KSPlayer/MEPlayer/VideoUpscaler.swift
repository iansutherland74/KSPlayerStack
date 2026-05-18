//
//  VideoUpscaler.swift
//  KSPlayer
//

import CoreMedia
import CoreVideo
import Foundation
import VideoToolbox

@MainActor
final class VideoUpscaler {
    private(set) var state = VideoUpscalingState.inactive
    private var loggedMessages = Set<String>()
    #if !targetEnvironment(simulator) && (os(iOS) || os(tvOS) || os(visionOS) || os(macOS))
    private var appleSuperResolutionScaler: AnyObject?
    #endif

    func reset() {
        state = .inactive
        #if !targetEnvironment(simulator) && (os(iOS) || os(tvOS) || os(visionOS) || os(macOS))
        if #available(iOS 26.0, tvOS 26.0, visionOS 26.0, macOS 26.0, *) {
            (appleSuperResolutionScaler as? AppleVideoSuperResolutionScaler)?.reset()
        }
        appleSuperResolutionScaler = nil
        #endif
    }

    func upscale(pixelBuffer: CVPixelBuffer, time: CMTime, mode: VideoUpscalingMode) -> CVPixelBuffer? {
        switch mode {
        case .none:
            reset()
            return nil
        case .appleSuperResolution:
            guard mode.requestedScaleFactor > 1 else {
                state = .unavailable(reason: "scale factor is 1x")
                return nil
            }
            #if !targetEnvironment(simulator) && (os(iOS) || os(tvOS) || os(visionOS) || os(macOS))
            if #available(iOS 26.0, tvOS 26.0, visionOS 26.0, macOS 26.0, *) {
                let scaler: AppleVideoSuperResolutionScaler
                if let existing = appleSuperResolutionScaler as? AppleVideoSuperResolutionScaler {
                    scaler = existing
                } else {
                    scaler = AppleVideoSuperResolutionScaler(log: logOnce)
                    appleSuperResolutionScaler = scaler
                }
                guard let output = scaler.upscale(pixelBuffer: pixelBuffer, time: time, requestedScaleFactor: mode.requestedScaleFactor) else {
                    state = .unavailable(reason: scaler.unavailableReason ?? "VideoToolbox low-latency super-resolution failed")
                    return nil
                }
                state = .active(
                    sourceSize: CGSize(width: CVPixelBufferGetWidth(pixelBuffer), height: CVPixelBufferGetHeight(pixelBuffer)),
                    outputSize: CGSize(width: CVPixelBufferGetWidth(output.pixelBuffer), height: CVPixelBufferGetHeight(output.pixelBuffer)),
                    scaleFactor: output.scaleFactor
                )
                return output.pixelBuffer
            }
            #endif
            let reason = "Apple super-resolution upscaling is unavailable on this platform or runtime"
            state = .unavailable(reason: reason)
            logOnce("[video] \(reason)")
            return nil
        }
    }

    private func logOnce(_ message: String) {
        if loggedMessages.insert(message).inserted {
            KSLog(message)
        }
    }
}

public extension VideoUpscalingMode {
    static var isAppleSuperResolutionRuntimeAvailable: Bool {
        #if !targetEnvironment(simulator) && (os(iOS) || os(tvOS) || os(visionOS) || os(macOS))
        if #available(iOS 26.0, tvOS 26.0, visionOS 26.0, macOS 26.0, *) {
            return VTLowLatencySuperResolutionScalerConfiguration.isSupported
        }
        #endif
        return false
    }
}

#if !targetEnvironment(simulator) && (os(iOS) || os(tvOS) || os(visionOS) || os(macOS))
@available(iOS 26.0, tvOS 26.0, visionOS 26.0, macOS 26.0, *)
private final class AppleVideoSuperResolutionScaler {
    private struct SessionKey: Equatable {
        let width: Int
        let height: Int
        let pixelFormat: OSType
        let scaleFactor: Float
    }

    private final class ProcessingResult: @unchecked Sendable {
        var error: Error?
    }

    private let log: (String) -> Void
    private var processor: VTFrameProcessor?
    private var configuration: VTLowLatencySuperResolutionScalerConfiguration?
    private var sessionKey: SessionKey?
    private(set) var unavailableReason: String?

    init(log: @escaping (String) -> Void) {
        self.log = log
    }

    func reset() {
        processor?.endSession()
        processor = nil
        configuration = nil
        sessionKey = nil
        unavailableReason = nil
    }

    func upscale(pixelBuffer: CVPixelBuffer, time: CMTime, requestedScaleFactor: Float) -> (pixelBuffer: CVPixelBuffer, scaleFactor: Float)? {
        unavailableReason = nil
        guard VTLowLatencySuperResolutionScalerConfiguration.isSupported else {
            return unavailable("VideoToolbox low-latency super-resolution is not supported on this device")
        }
        let width = CVPixelBufferGetWidth(pixelBuffer)
        let height = CVPixelBufferGetHeight(pixelBuffer)
        guard let scaleFactor = Self.supportedScaleFactor(width: width, height: height, requestedScaleFactor: requestedScaleFactor) else {
            return unavailable("VideoToolbox low-latency super-resolution does not support \(width)x\(height) at \(requestedScaleFactor)x")
        }
        let pixelFormat = CVPixelBufferGetPixelFormatType(pixelBuffer)
        let key = SessionKey(width: width, height: height, pixelFormat: pixelFormat, scaleFactor: scaleFactor)
        guard ensureSession(key: key) else {
            return nil
        }
        guard let configuration, configuration.supportedPixelFormats.contains(pixelFormat) else {
            return unavailable("VideoToolbox low-latency super-resolution does not support pixel format \(pixelFormat)")
        }
        guard let destinationBuffer = makeDestinationBuffer(configuration: configuration, key: key) else {
            return nil
        }
        guard let sourceFrame = VTFrameProcessorFrame(buffer: pixelBuffer, presentationTimeStamp: time),
              let destinationFrame = VTFrameProcessorFrame(buffer: destinationBuffer, presentationTimeStamp: time)
        else {
            return unavailable("VideoToolbox low-latency super-resolution requires IOSurface-backed pixel buffers")
        }

        let parameters = VTLowLatencySuperResolutionScalerParameters(sourceFrame: sourceFrame, destinationFrame: destinationFrame)
        let semaphore = DispatchSemaphore(value: 0)
        let result = ProcessingResult()
        processor?.process(parameters: parameters) { _, error in
            result.error = error
            semaphore.signal()
        }
        guard semaphore.wait(timeout: .now() + .milliseconds(100)) == .success else {
            let reason = "VideoToolbox low-latency super-resolution timed out"
            unavailableReason = reason
            log("[video] \(reason)")
            reset()
            unavailableReason = reason
            return nil
        }
        if let error = result.error {
            return unavailable("VideoToolbox low-latency super-resolution failed: \(error.localizedDescription)")
        }
        CVBufferPropagateAttachments(pixelBuffer, destinationBuffer)
        return (destinationBuffer, scaleFactor)
    }

    private static func supportedScaleFactor(width: Int, height: Int, requestedScaleFactor: Float) -> Float? {
        let scaleFactors = VTLowLatencySuperResolutionScalerConfiguration.supportedScaleFactors(frameWidth: width, frameHeight: height)
        return scaleFactors
            .filter { $0 > 1 && $0 <= requestedScaleFactor }
            .max()
    }

    private func ensureSession(key: SessionKey) -> Bool {
        guard sessionKey != key else {
            return true
        }
        reset()
        let configuration = VTLowLatencySuperResolutionScalerConfiguration(frameWidth: key.width, frameHeight: key.height, scaleFactor: key.scaleFactor)
        let processor = VTFrameProcessor()
        do {
            try processor.startSession(configuration: configuration)
            self.processor = processor
            self.configuration = configuration
            sessionKey = key
            return true
        } catch {
            unavailableReason = "VideoToolbox low-latency super-resolution session failed: \(error.localizedDescription)"
            log("[video] \(unavailableReason!)")
            return false
        }
    }

    private func makeDestinationBuffer(configuration: VTLowLatencySuperResolutionScalerConfiguration, key: SessionKey) -> CVPixelBuffer? {
        let width = Int((Float(key.width) * key.scaleFactor).rounded(.toNearestOrAwayFromZero))
        let height = Int((Float(key.height) * key.scaleFactor).rounded(.toNearestOrAwayFromZero))
        var attributes = configuration.destinationPixelBufferAttributes
        attributes[kCVPixelBufferWidthKey as String] = width
        attributes[kCVPixelBufferHeightKey as String] = height
        attributes[kCVPixelBufferPixelFormatTypeKey as String] = key.pixelFormat
        attributes[kCVPixelBufferMetalCompatibilityKey as String] = true
        attributes[kCVPixelBufferIOSurfacePropertiesKey as String] = [String: String]()

        var buffer: CVPixelBuffer?
        let status = CVPixelBufferCreate(kCFAllocatorDefault, width, height, key.pixelFormat, attributes as CFDictionary, &buffer)
        guard status == kCVReturnSuccess, let buffer else {
            unavailableReason = "VideoToolbox low-latency super-resolution could not allocate \(width)x\(height) output buffer: \(status)"
            log("[video] \(unavailableReason!)")
            return nil
        }
        return buffer
    }

    private func unavailable(_ reason: String) -> (pixelBuffer: CVPixelBuffer, scaleFactor: Float)? {
        unavailableReason = reason
        log("[video] \(reason)")
        return nil
    }
}
#endif

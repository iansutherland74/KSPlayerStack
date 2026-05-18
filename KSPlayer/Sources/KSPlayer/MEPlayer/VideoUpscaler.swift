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
    private var loggedMessages = Set<String>()
    #if !targetEnvironment(simulator) && (os(iOS) || os(tvOS) || os(visionOS) || os(macOS))
    private var appleSuperResolutionScaler: AnyObject?
    #endif

    func reset() {
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
        case let .appleSuperResolution(scaleFactor):
            #if !targetEnvironment(simulator) && (os(iOS) || os(tvOS) || os(visionOS) || os(macOS))
            if #available(iOS 26.0, tvOS 26.0, visionOS 26.0, macOS 26.0, *) {
                let scaler: AppleVideoSuperResolutionScaler
                if let existing = appleSuperResolutionScaler as? AppleVideoSuperResolutionScaler {
                    scaler = existing
                } else {
                    scaler = AppleVideoSuperResolutionScaler(log: logOnce)
                    appleSuperResolutionScaler = scaler
                }
                return scaler.upscale(pixelBuffer: pixelBuffer, time: time, requestedScaleFactor: scaleFactor)
            }
            #endif
            logOnce("[video] Apple super-resolution upscaling is unavailable on this platform or runtime")
            return nil
        }
    }

    private func logOnce(_ message: String) {
        if loggedMessages.insert(message).inserted {
            KSLog(message)
        }
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

    init(log: @escaping (String) -> Void) {
        self.log = log
    }

    func reset() {
        processor?.endSession()
        processor = nil
        configuration = nil
        sessionKey = nil
    }

    func upscale(pixelBuffer: CVPixelBuffer, time: CMTime, requestedScaleFactor: Float) -> CVPixelBuffer? {
        guard VTLowLatencySuperResolutionScalerConfiguration.isSupported else {
            log("[video] VideoToolbox low-latency super-resolution is not supported on this device")
            return nil
        }
        let width = CVPixelBufferGetWidth(pixelBuffer)
        let height = CVPixelBufferGetHeight(pixelBuffer)
        guard let scaleFactor = Self.supportedScaleFactor(width: width, height: height, requestedScaleFactor: requestedScaleFactor) else {
            log("[video] VideoToolbox low-latency super-resolution does not support \(width)x\(height)")
            return nil
        }
        let pixelFormat = CVPixelBufferGetPixelFormatType(pixelBuffer)
        let key = SessionKey(width: width, height: height, pixelFormat: pixelFormat, scaleFactor: scaleFactor)
        guard ensureSession(key: key) else {
            return nil
        }
        guard let configuration, configuration.supportedPixelFormats.contains(pixelFormat) else {
            log("[video] VideoToolbox low-latency super-resolution does not support pixel format \(pixelFormat)")
            return nil
        }
        guard let destinationBuffer = makeDestinationBuffer(configuration: configuration, key: key) else {
            return nil
        }
        guard let sourceFrame = VTFrameProcessorFrame(buffer: pixelBuffer, presentationTimeStamp: time),
              let destinationFrame = VTFrameProcessorFrame(buffer: destinationBuffer, presentationTimeStamp: time)
        else {
            log("[video] VideoToolbox low-latency super-resolution requires IOSurface-backed pixel buffers")
            return nil
        }

        let parameters = VTLowLatencySuperResolutionScalerParameters(sourceFrame: sourceFrame, destinationFrame: destinationFrame)
        let semaphore = DispatchSemaphore(value: 0)
        let result = ProcessingResult()
        processor?.process(parameters: parameters) { _, error in
            result.error = error
            semaphore.signal()
        }
        guard semaphore.wait(timeout: .now() + .milliseconds(100)) == .success else {
            log("[video] VideoToolbox low-latency super-resolution timed out")
            reset()
            return nil
        }
        if let error = result.error {
            log("[video] VideoToolbox low-latency super-resolution failed: \(error.localizedDescription)")
            return nil
        }
        CVBufferPropagateAttachments(pixelBuffer, destinationBuffer)
        return destinationBuffer
    }

    private static func supportedScaleFactor(width: Int, height: Int, requestedScaleFactor: Float) -> Float? {
        let scaleFactors = VTLowLatencySuperResolutionScalerConfiguration.supportedScaleFactors(frameWidth: width, frameHeight: height)
        return scaleFactors
            .filter { $0 > 1 && $0 <= requestedScaleFactor }
            .max() ?? scaleFactors.filter { $0 > 1 }.min()
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
            log("[video] VideoToolbox low-latency super-resolution session failed: \(error.localizedDescription)")
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
            log("[video] VideoToolbox low-latency super-resolution could not allocate \(width)x\(height) output buffer: \(status)")
            return nil
        }
        return buffer
    }
}
#endif

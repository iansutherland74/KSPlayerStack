//
//  VideoUpscaler.swift
//  KSPlayer
//

import CoreMedia
import CoreVideo
import Foundation
#if canImport(ObjectiveC)
import ObjectiveC
#endif
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
        (appleSuperResolutionScaler as? AppleVideoSuperResolutionScaler)?.reset()
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
            return AppleVideoSuperResolutionScaler.isRuntimeAvailable
        }
        #endif
        return false
    }
}

#if !targetEnvironment(simulator) && (os(iOS) || os(tvOS) || os(visionOS) || os(macOS))
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
    private var processor: AnyObject?
    private var configuration: AnyObject?
    private var sessionKey: SessionKey?
    private(set) var unavailableReason: String?

    static var isRuntimeAvailable: Bool {
        VideoToolboxSuperResolutionRuntime.isSupported
    }

    init(log: @escaping (String) -> Void) {
        self.log = log
    }

    func reset() {
        if let processor {
            VideoToolboxSuperResolutionRuntime.endSession(processor)
        }
        processor = nil
        configuration = nil
        sessionKey = nil
        unavailableReason = nil
    }

    func upscale(pixelBuffer: CVPixelBuffer, time: CMTime, requestedScaleFactor: Float) -> (pixelBuffer: CVPixelBuffer, scaleFactor: Float)? {
        unavailableReason = nil
        guard Self.isRuntimeAvailable else {
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
        guard let configuration, VideoToolboxSuperResolutionRuntime.supportedPixelFormats(configuration).contains(pixelFormat) else {
            return unavailable("VideoToolbox low-latency super-resolution does not support pixel format \(pixelFormat)")
        }
        guard let destinationBuffer = makeDestinationBuffer(configuration: configuration, key: key) else {
            return nil
        }
        guard let sourceFrame = VideoToolboxSuperResolutionRuntime.makeFrame(pixelBuffer: pixelBuffer, time: time),
              let destinationFrame = VideoToolboxSuperResolutionRuntime.makeFrame(pixelBuffer: destinationBuffer, time: time)
        else {
            return unavailable("VideoToolbox low-latency super-resolution requires IOSurface-backed pixel buffers")
        }

        guard let parameters = VideoToolboxSuperResolutionRuntime.makeParameters(sourceFrame: sourceFrame, destinationFrame: destinationFrame) else {
            return unavailable("VideoToolbox low-latency super-resolution parameters are unavailable on this runtime")
        }
        let semaphore = DispatchSemaphore(value: 0)
        let result = ProcessingResult()
        guard let processor else {
            return unavailable("VideoToolbox low-latency super-resolution session is unavailable")
        }
        VideoToolboxSuperResolutionRuntime.process(processor, parameters: parameters) { error in
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
        let scaleFactors = VideoToolboxSuperResolutionRuntime.supportedScaleFactors(frameWidth: width, frameHeight: height)
        return scaleFactors
            .filter { $0 > 1 && $0 <= requestedScaleFactor }
            .max()
    }

    private func ensureSession(key: SessionKey) -> Bool {
        guard sessionKey != key else {
            return true
        }
        reset()
        guard let configuration = VideoToolboxSuperResolutionRuntime.makeConfiguration(frameWidth: key.width, frameHeight: key.height, scaleFactor: key.scaleFactor),
              let processor = VideoToolboxSuperResolutionRuntime.makeProcessor()
        else {
            unavailableReason = "VideoToolbox low-latency super-resolution is unavailable on this runtime"
            log("[video] \(unavailableReason!)")
            return false
        }
        if let error = VideoToolboxSuperResolutionRuntime.startSession(processor, configuration: configuration) {
            unavailableReason = "VideoToolbox low-latency super-resolution session failed: \(error.localizedDescription)"
            log("[video] \(unavailableReason!)")
            return false
        }
        self.processor = processor
        self.configuration = configuration
        sessionKey = key
        return true
    }

    private func makeDestinationBuffer(configuration: AnyObject, key: SessionKey) -> CVPixelBuffer? {
        let width = Int((Float(key.width) * key.scaleFactor).rounded(.toNearestOrAwayFromZero))
        let height = Int((Float(key.height) * key.scaleFactor).rounded(.toNearestOrAwayFromZero))
        var attributes = VideoToolboxSuperResolutionRuntime.destinationPixelBufferAttributes(configuration)
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

private enum VideoToolboxSuperResolutionRuntime {
    private static let configurationClassName = "VTLowLatencySuperResolutionScalerConfiguration"
    private static let parametersClassName = "VTLowLatencySuperResolutionScalerParameters"
    private static let frameClassName = "VTFrameProcessorFrame"
    private static let processorClassName = "VTFrameProcessor"

    static var isSupported: Bool {
        guard let configurationClass else {
            return false
        }
        return boolClassProperty(configurationClass, selectorName: "isSupported")
    }

    static func supportedScaleFactors(frameWidth: Int, frameHeight: Int) -> [Float] {
        guard let configurationClass,
              let method = class_getClassMethod(configurationClass, NSSelectorFromString("supportedScaleFactorsForFrameWidth:frameHeight:"))
        else {
            return []
        }
        typealias Function = @convention(c) (AnyClass, Selector, Int, Int) -> NSArray?
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        return (function(configurationClass, NSSelectorFromString("supportedScaleFactorsForFrameWidth:frameHeight:"), frameWidth, frameHeight) as? [NSNumber])?.map(\.floatValue) ?? []
    }

    static func makeConfiguration(frameWidth: Int, frameHeight: Int, scaleFactor: Float) -> AnyObject? {
        guard let configurationClass,
              let instance = allocate(configurationClass),
              let method = class_getInstanceMethod(configurationClass, NSSelectorFromString("initWithFrameWidth:frameHeight:scaleFactor:"))
        else {
            return nil
        }
        typealias Function = @convention(c) (AnyObject, Selector, Int, Int, Float) -> AnyObject?
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        return function(instance, NSSelectorFromString("initWithFrameWidth:frameHeight:scaleFactor:"), frameWidth, frameHeight, scaleFactor)
    }

    static func makeProcessor() -> AnyObject? {
        guard let processorClass,
              let instance = allocate(processorClass),
              let method = class_getInstanceMethod(processorClass, NSSelectorFromString("init"))
        else {
            return nil
        }
        typealias Function = @convention(c) (AnyObject, Selector) -> AnyObject?
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        return function(instance, NSSelectorFromString("init"))
    }

    static func makeFrame(pixelBuffer: CVPixelBuffer, time: CMTime) -> AnyObject? {
        guard let frameClass,
              let instance = allocate(frameClass),
              let method = class_getInstanceMethod(frameClass, NSSelectorFromString("initWithBuffer:presentationTimeStamp:"))
        else {
            return nil
        }
        typealias Function = @convention(c) (AnyObject, Selector, CVPixelBuffer, CMTime) -> AnyObject?
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        return function(instance, NSSelectorFromString("initWithBuffer:presentationTimeStamp:"), pixelBuffer, time)
    }

    static func makeParameters(sourceFrame: AnyObject, destinationFrame: AnyObject) -> AnyObject? {
        guard let parametersClass,
              let instance = allocate(parametersClass),
              let method = class_getInstanceMethod(parametersClass, NSSelectorFromString("initWithSourceFrame:destinationFrame:"))
        else {
            return nil
        }
        typealias Function = @convention(c) (AnyObject, Selector, AnyObject, AnyObject) -> AnyObject?
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        return function(instance, NSSelectorFromString("initWithSourceFrame:destinationFrame:"), sourceFrame, destinationFrame)
    }

    static func startSession(_ processor: AnyObject, configuration: AnyObject) -> NSError? {
        guard let method = class_getInstanceMethod(processorClass, NSSelectorFromString("startSessionWithConfiguration:error:")) else {
            return NSError(domain: "KSPlayer.VideoUpscaler", code: -1, userInfo: [NSLocalizedDescriptionKey: "VideoToolbox frame processor is unavailable"])
        }
        typealias Function = @convention(c) (AnyObject, Selector, AnyObject, UnsafeMutablePointer<NSError?>?) -> Bool
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        var error: NSError?
        return function(processor, NSSelectorFromString("startSessionWithConfiguration:error:"), configuration, &error) ? nil : error
    }

    static func process(_ processor: AnyObject, parameters: AnyObject, completionHandler: @escaping (NSError?) -> Void) {
        guard let method = class_getInstanceMethod(processorClass, NSSelectorFromString("processWithParameters:completionHandler:")) else {
            completionHandler(NSError(domain: "KSPlayer.VideoUpscaler", code: -1, userInfo: [NSLocalizedDescriptionKey: "VideoToolbox frame processor is unavailable"]))
            return
        }
        typealias Completion = @convention(block) (AnyObject, NSError?) -> Void
        typealias Function = @convention(c) (AnyObject, Selector, AnyObject, @escaping Completion) -> Void
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        function(processor, NSSelectorFromString("processWithParameters:completionHandler:"), parameters) { _, error in
            completionHandler(error)
        }
    }

    static func endSession(_ processor: AnyObject) {
        guard let method = class_getInstanceMethod(processorClass, NSSelectorFromString("endSession")) else {
            return
        }
        typealias Function = @convention(c) (AnyObject, Selector) -> Void
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        function(processor, NSSelectorFromString("endSession"))
    }

    static func supportedPixelFormats(_ configuration: AnyObject) -> [OSType] {
        (objectProperty(configuration, selectorName: "frameSupportedPixelFormats") as? [NSNumber])?.map(\.uint32Value) ?? []
    }

    static func destinationPixelBufferAttributes(_ configuration: AnyObject) -> [String: Any] {
        objectProperty(configuration, selectorName: "destinationPixelBufferAttributes") as? [String: Any] ?? [:]
    }

    private static var configurationClass: AnyClass? {
        NSClassFromString(configurationClassName)
    }

    private static var parametersClass: AnyClass? {
        NSClassFromString(parametersClassName)
    }

    private static var frameClass: AnyClass? {
        NSClassFromString(frameClassName)
    }

    private static var processorClass: AnyClass? {
        NSClassFromString(processorClassName)
    }

    private static func allocate(_ objectClass: AnyClass) -> AnyObject? {
        guard let method = class_getClassMethod(objectClass, NSSelectorFromString("alloc")) else {
            return nil
        }
        typealias Function = @convention(c) (AnyClass, Selector) -> AnyObject?
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        return function(objectClass, NSSelectorFromString("alloc"))
    }

    private static func boolClassProperty(_ objectClass: AnyClass, selectorName: String) -> Bool {
        guard let method = class_getClassMethod(objectClass, NSSelectorFromString(selectorName)) else {
            return false
        }
        typealias Function = @convention(c) (AnyClass, Selector) -> Bool
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        return function(objectClass, NSSelectorFromString(selectorName))
    }

    private static func objectProperty(_ object: AnyObject, selectorName: String) -> AnyObject? {
        guard let method = class_getInstanceMethod(object_getClass(object), NSSelectorFromString(selectorName)) else {
            return nil
        }
        typealias Function = @convention(c) (AnyObject, Selector) -> AnyObject?
        let function = unsafeBitCast(method_getImplementation(method), to: Function.self)
        return function(object, NSSelectorFromString(selectorName))
    }
}
#endif

import AVFoundation
import CoreGraphics
import CoreMedia
import CoreVideo
import Foundation
import KSPlayerONNXRuntimeSupport
import simd
#if canImport(CoreImage)
import CoreImage
#endif
#if canImport(CoreML)
import CoreML
#endif

public enum Video2DTo3DMode: Equatable, Sendable {
    /// Preserve normal 2D playback.
    case disabled
    /// Generate a conservative pseudo-stereo disparity from the frame geometry.
    case pseudoStereo
    /// Prefer an app-provided depth map, falling back to pseudo-stereo when one is unavailable.
    case depthMapPreferred

    public var isEnabled: Bool {
        self != .disabled
    }
}

public enum Video2DTo3DOutputLayout: Equatable, Sendable {
    /// Render only `KSOptions.stereoscopicVideoEye` for normal 2D displays.
    case selectedEye
    /// Render left and right eyes into a side-by-side packed frame.
    case sideBySide
    /// Render left and right eyes into a top-and-bottom packed frame.
    case topAndBottom

    func drawableSize(for baseSize: CGSize) -> CGSize {
        switch self {
        case .selectedEye:
            return baseSize
        case .sideBySide:
            return CGSize(width: baseSize.width * 2, height: baseSize.height)
        case .topAndBottom:
            return CGSize(width: baseSize.width, height: baseSize.height * 2)
        }
    }
}

public struct VideoDepthMap: Equatable, Sendable {
    public let width: Int
    public let height: Int
    /// Normalized per-pixel disparity values in row-major order. Larger values are treated as closer to the viewer.
    public let normalizedDisparity: [Float]
    public let confidence: Float?
    public let metadata: [String: String]

    public init?(width: Int, height: Int, normalizedDisparity: [Float], confidence: Float? = nil, metadata: [String: String] = [:]) {
        guard width > 0, height > 0, normalizedDisparity.count == width * height else {
            return nil
        }
        self.width = width
        self.height = height
        self.normalizedDisparity = normalizedDisparity.map { value in
            guard value.isFinite else {
                return 0.5
            }
            return min(max(value, 0), 1)
        }
        if let confidence, confidence.isFinite {
            self.confidence = min(max(confidence, 0), 1)
        } else {
            self.confidence = nil
        }
        self.metadata = metadata
    }

    public static func neutral(width: Int = 1, height: Int = 1) -> VideoDepthMap {
        VideoDepthMap(width: max(width, 1), height: max(height, 1), normalizedDisparity: Array(repeating: 0.5, count: max(width, 1) * max(height, 1)))!
    }
}

public enum VideoDepthEstimationError: Error, LocalizedError, Equatable, Sendable {
    case unavailablePlatform(String)
    case unsupportedModelInput(String)
    case unsupportedModelOutput(String)
    case preprocessingFailed(String)
    case runtimeUnavailable(String)
    case inferenceFailed(String)

    public var errorDescription: String? {
        switch self {
        case let .unavailablePlatform(reason),
             let .unsupportedModelInput(reason),
             let .unsupportedModelOutput(reason),
             let .preprocessingFailed(reason),
             let .runtimeUnavailable(reason),
             let .inferenceFailed(reason):
            return reason
        }
    }
}

public struct VideoDepthEstimationRequest {
    public let pixelBuffer: CVPixelBuffer
    public let presentationTime: CMTime
    public let naturalSize: CGSize
    public let dynamicRange: DynamicRange?

    public init(pixelBuffer: CVPixelBuffer, presentationTime: CMTime, naturalSize: CGSize, dynamicRange: DynamicRange?) {
        self.pixelBuffer = pixelBuffer
        self.presentationTime = presentationTime
        self.naturalSize = naturalSize
        self.dynamicRange = dynamicRange
    }
}

@MainActor
public protocol VideoDepthEstimationProvider: AnyObject {
    var providerID: String { get }
    func makeDepthMap(request: VideoDepthEstimationRequest) throws -> VideoDepthMap?
}

public enum VideoDepthAspectPolicy: Equatable, Sendable {
    case stretch
    case scaleAspectFit
    case scaleAspectFill
}

public enum VideoDepthNormalizationMode: Equatable, Sendable {
    /// Treat values as already normalized disparity and only sanitize invalid/out-of-range samples.
    case none
    /// Normalize finite raw depth values into `0 ... 1` per frame.
    case minMax
}

public struct VideoDepthInputSize: Equatable, Sendable {
    public let width: Int
    public let height: Int

    public init(width: Int, height: Int) {
        self.width = max(width, 1)
        self.height = max(height, 1)
    }
}

public struct VideoDepthProcessingConfiguration: Equatable, Sendable {
    public static let coreMLDefault = VideoDepthProcessingConfiguration()
    public static let preNormalizedDepthMap = VideoDepthProcessingConfiguration(
        inputSize: nil,
        normalization: .none,
        maximumInferenceFPS: nil,
        temporalSmoothingFactor: 0
    )

    public let inputSize: VideoDepthInputSize?
    public let aspectPolicy: VideoDepthAspectPolicy
    public let normalization: VideoDepthNormalizationMode
    public let invertDepth: Bool
    public let maximumInferenceFPS: Double?
    public let temporalSmoothingFactor: Float
    public let preferredOutputFeatureName: String?

    public init(
        inputSize: VideoDepthInputSize? = VideoDepthInputSize(width: 518, height: 518),
        aspectPolicy: VideoDepthAspectPolicy = .scaleAspectFit,
        normalization: VideoDepthNormalizationMode = .minMax,
        invertDepth: Bool = false,
        maximumInferenceFPS: Double? = 12,
        temporalSmoothingFactor: Float = 0.6,
        preferredOutputFeatureName: String? = nil
    ) {
        self.inputSize = inputSize
        self.aspectPolicy = aspectPolicy
        self.normalization = normalization
        self.invertDepth = invertDepth
        if let maximumInferenceFPS, maximumInferenceFPS.isFinite, maximumInferenceFPS > 0 {
            self.maximumInferenceFPS = maximumInferenceFPS
        } else {
            self.maximumInferenceFPS = nil
        }
        if temporalSmoothingFactor.isFinite {
            self.temporalSmoothingFactor = min(max(temporalSmoothingFactor, 0), 0.95)
        } else {
            self.temporalSmoothingFactor = 0
        }
        self.preferredOutputFeatureName = preferredOutputFeatureName
    }
}

public struct VideoDepthPrediction: Equatable, Sendable {
    public let width: Int
    public let height: Int
    public let values: [Float]
    public let confidence: Float?
    public let metadata: [String: String]

    public init?(width: Int, height: Int, values: [Float], confidence: Float? = nil, metadata: [String: String] = [:]) {
        guard width > 0, height > 0, values.count == width * height else {
            return nil
        }
        self.width = width
        self.height = height
        self.values = values
        if let confidence, confidence.isFinite {
            self.confidence = min(max(confidence, 0), 1)
        } else {
            self.confidence = nil
        }
        self.metadata = metadata
    }
}

public enum VideoDepthPostprocessor {
    public static func normalizedValues(
        _ values: [Float],
        normalization: VideoDepthNormalizationMode = .minMax,
        invertDepth: Bool = false
    ) -> [Float] {
        let normalized: [Float]
        switch normalization {
        case .none:
            normalized = values.map { value in
                guard value.isFinite else {
                    return 0.5
                }
                return min(max(value, 0), 1)
            }
        case .minMax:
            let finiteValues = values.filter(\.isFinite)
            guard let minimum = finiteValues.min(), let maximum = finiteValues.max() else {
                normalized = Array(repeating: 0.5, count: values.count)
                break
            }
            let range = maximum - minimum
            guard range.isFinite, range > Float.ulpOfOne else {
                normalized = Array(repeating: 0.5, count: values.count)
                break
            }
            normalized = values.map { value in
                guard value.isFinite else {
                    return 0.5
                }
                return min(max((value - minimum) / range, 0), 1)
            }
        }
        guard invertDepth else {
            return normalized
        }
        return normalized.map { 1 - $0 }
    }

    public static func smoothedValues(current: [Float], previous: [Float], factor: Float) -> [Float] {
        guard current.count == previous.count, factor.isFinite, factor > 0 else {
            return current
        }
        let previousWeight = min(max(factor, 0), 0.95)
        let currentWeight = 1 - previousWeight
        return zip(current, previous).map { currentValue, previousValue in
            min(max(previousValue * previousWeight + currentValue * currentWeight, 0), 1)
        }
    }

    public static func makeDepthMap(
        prediction: VideoDepthPrediction,
        configuration: VideoDepthProcessingConfiguration,
        previousDepthMap: VideoDepthMap? = nil
    ) -> VideoDepthMap? {
        var values = normalizedValues(
            prediction.values,
            normalization: configuration.normalization,
            invertDepth: configuration.invertDepth
        )
        if let previousDepthMap, previousDepthMap.width == prediction.width, previousDepthMap.height == prediction.height {
            values = smoothedValues(
                current: values,
                previous: previousDepthMap.normalizedDisparity,
                factor: configuration.temporalSmoothingFactor
            )
        }
        return VideoDepthMap(
            width: prediction.width,
            height: prediction.height,
            normalizedDisparity: values,
            confidence: prediction.confidence,
            metadata: prediction.metadata
        )
    }
}

public enum Video2DTo3DDiagnostic: Equatable, Sendable, CustomStringConvertible {
    case unavailable(reason: String)

    public var description: String {
        switch self {
        case let .unavailable(reason):
            return reason
        }
    }
}

@MainActor
public protocol DepthAnythingONNXInferenceBackend: AnyObject {
    func makeDepthPrediction(request: VideoDepthEstimationRequest, configuration: VideoDepthProcessingConfiguration) throws -> VideoDepthPrediction?
}

public typealias DepthAnythingV2ONNXInferenceBackend = DepthAnythingONNXInferenceBackend

@MainActor
public final class DepthAnythingDepthEstimationAdapter: VideoDepthEstimationProvider {
    public enum Runtime: String, Equatable, Sendable {
        case coreML
        case onnx
        case custom
    }

    public enum ModelFamily: String, Equatable, Sendable {
        case depthAnythingV2
        case depthAnything3
        case custom

        public var displayName: String {
            switch self {
            case .depthAnythingV2:
                return "Depth Anything V2"
            case .depthAnything3:
                return "Depth Anything 3"
            case .custom:
                return "Custom Depth Anything-compatible"
            }
        }
    }

    public enum ModelVariant: String, Equatable, Sendable {
        case small
        case base
        case large
        case giant
        case large11
        case giant11
        case metricLarge
        case monoLarge
        case nestedGiantLarge
        case nestedGiantLarge11

        public var licenseNote: String {
            licenseNote(for: .depthAnythingV2)
        }

        public func licenseNote(for family: ModelFamily) -> String {
            switch self {
            case .small:
                switch family {
                case .depthAnything3:
                    return "Depth Anything 3 Small weights are published as Apache-2.0."
                case .depthAnythingV2, .custom:
                    return "Depth Anything V2 Small weights are published as Apache-2.0."
                }
            case .base:
                switch family {
                case .depthAnything3:
                    return "Depth Anything 3 Base weights are published as Apache-2.0."
                case .depthAnythingV2, .custom:
                    return "Depth Anything V2 Base weights are published as CC-BY-NC-4.0 by upstream."
                }
            case .large, .giant, .large11, .giant11, .nestedGiantLarge, .nestedGiantLarge11:
                switch family {
                case .depthAnything3:
                    return "Depth Anything 3 Large/Giant/Nested any-view weights are published as CC-BY-NC-4.0."
                case .depthAnythingV2, .custom:
                    return "Depth Anything V2 Large/Giant weights are published as CC-BY-NC-4.0 by upstream."
                }
            case .metricLarge, .monoLarge:
                switch family {
                case .depthAnything3:
                    return "Depth Anything 3 Metric-Large and Mono-Large weights are published as Apache-2.0."
                case .depthAnythingV2, .custom:
                    return "This Depth Anything-compatible variant requires app-side license review."
                }
            }
        }
    }

    public let providerID: String
    public let runtime: Runtime
    public let modelFamily: ModelFamily
    public let modelVariant: ModelVariant
    public let modelURL: URL?
    public let processingConfiguration: VideoDepthProcessingConfiguration
    private let allowsUnsupportedPlatformInference: Bool
    private let inference: (VideoDepthEstimationRequest) throws -> VideoDepthPrediction?
    private var lastInferenceTime: Double?
    private var lastDepthMap: VideoDepthMap?

    public init(
        providerID: String = "depth-anything-v2",
        runtime: Runtime = .coreML,
        modelFamily: ModelFamily = .depthAnythingV2,
        modelVariant: ModelVariant = .small,
        modelURL: URL? = nil,
        processingConfiguration: VideoDepthProcessingConfiguration = .preNormalizedDepthMap,
        inference: @escaping (VideoDepthEstimationRequest) throws -> VideoDepthMap?
    ) {
        self.providerID = providerID
        self.runtime = runtime
        self.modelFamily = modelFamily
        self.modelVariant = modelVariant
        self.modelURL = modelURL
        self.processingConfiguration = processingConfiguration
        allowsUnsupportedPlatformInference = false
        self.inference = { request in
            guard let depthMap = try inference(request) else {
                return nil
            }
            return VideoDepthPrediction(
                width: depthMap.width,
                height: depthMap.height,
                values: depthMap.normalizedDisparity,
                confidence: depthMap.confidence,
                metadata: depthMap.metadata
            )
        }
    }

    init(
        providerID: String = "depth-anything-v2",
        runtime: Runtime,
        modelFamily: ModelFamily = .depthAnythingV2,
        modelVariant: ModelVariant = .small,
        modelURL: URL? = nil,
        processingConfiguration: VideoDepthProcessingConfiguration,
        allowsUnsupportedPlatformInference: Bool,
        predictionInference: @escaping (VideoDepthEstimationRequest) throws -> VideoDepthPrediction?
    ) {
        self.providerID = providerID
        self.runtime = runtime
        self.modelFamily = modelFamily
        self.modelVariant = modelVariant
        self.modelURL = modelURL
        self.processingConfiguration = processingConfiguration
        self.allowsUnsupportedPlatformInference = allowsUnsupportedPlatformInference
        inference = predictionInference
    }

    public convenience init(
        providerID: String = "depth-anything-onnx",
        modelFamily: ModelFamily = .depthAnythingV2,
        modelVariant: ModelVariant = .small,
        modelURL: URL? = nil,
        processingConfiguration: VideoDepthProcessingConfiguration = .coreMLDefault,
        backend: any DepthAnythingONNXInferenceBackend
    ) {
        self.init(
            providerID: providerID,
            runtime: .onnx,
            modelFamily: modelFamily,
            modelVariant: modelVariant,
            modelURL: modelURL,
            processingConfiguration: processingConfiguration,
            allowsUnsupportedPlatformInference: false
        ) { request in
            try backend.makeDepthPrediction(request: request, configuration: processingConfiguration)
        }
    }

    public func makeDepthMap(request: VideoDepthEstimationRequest) throws -> VideoDepthMap? {
        guard Video2DTo3DPolicy.isSupportedPlatform || allowsUnsupportedPlatformInference else {
            throw VideoDepthEstimationError.unavailablePlatform(Video2DTo3DPolicy.unavailablePlatformReason)
        }
        if shouldReuseLastDepthMap(for: request.presentationTime), let lastDepthMap {
            return lastDepthMap
        }
        guard let prediction = try inference(request),
              let depthMap = VideoDepthPostprocessor.makeDepthMap(
                prediction: prediction,
                configuration: processingConfiguration,
                previousDepthMap: lastDepthMap
              )
        else {
            return lastDepthMap
        }
        lastInferenceTime = seconds(for: request.presentationTime)
        lastDepthMap = depthMap
        return depthMap
    }

    private func shouldReuseLastDepthMap(for time: CMTime) -> Bool {
        guard let maximumInferenceFPS = processingConfiguration.maximumInferenceFPS,
              let lastInferenceTime,
              let currentTime = seconds(for: time)
        else {
            return false
        }
        return currentTime - lastInferenceTime < 1 / maximumInferenceFPS
    }

    private func seconds(for time: CMTime) -> Double? {
        let seconds = CMTimeGetSeconds(time)
        guard seconds.isFinite else {
            return nil
        }
        return seconds
    }
}

public typealias DepthAnythingV2DepthEstimationAdapter = DepthAnythingDepthEstimationAdapter

public struct DepthAnythingONNXTensorConfiguration: Equatable, Sendable {
    public enum Layout: String, Equatable, Sendable {
        case nchw
        case nhwc
    }

    public enum ChannelOrder: String, Equatable, Sendable {
        case rgb
        case bgr
    }

    public let layout: Layout
    public let channelOrder: ChannelOrder
    public let scale: Float
    public let mean: SIMD3<Float>
    public let standardDeviation: SIMD3<Float>

    public init(
        layout: Layout = .nchw,
        channelOrder: ChannelOrder = .rgb,
        scale: Float = 1 / 255,
        mean: SIMD3<Float> = SIMD3<Float>(0.485, 0.456, 0.406),
        standardDeviation: SIMD3<Float> = SIMD3<Float>(0.229, 0.224, 0.225)
    ) {
        self.layout = layout
        self.channelOrder = channelOrder
        self.scale = scale.isFinite ? scale : 1 / 255
        self.mean = Self.sanitized(mean, fallback: SIMD3<Float>(0.485, 0.456, 0.406))
        self.standardDeviation = SIMD3<Float>(
            max(Self.sanitized(standardDeviation, fallback: SIMD3<Float>(0.229, 0.224, 0.225)).x, Float.ulpOfOne),
            max(Self.sanitized(standardDeviation, fallback: SIMD3<Float>(0.229, 0.224, 0.225)).y, Float.ulpOfOne),
            max(Self.sanitized(standardDeviation, fallback: SIMD3<Float>(0.229, 0.224, 0.225)).z, Float.ulpOfOne)
        )
    }

    private static func sanitized(_ value: SIMD3<Float>, fallback: SIMD3<Float>) -> SIMD3<Float> {
        SIMD3<Float>(
            value.x.isFinite ? value.x : fallback.x,
            value.y.isFinite ? value.y : fallback.y,
            value.z.isFinite ? value.z : fallback.z
        )
    }
}

public struct DepthAnythingONNXRuntimeConfiguration: Equatable, Sendable {
    public enum GraphOptimizationLevel: Int32, Equatable, Sendable {
        case disabled = 0
        case basic = 1
        case extended = 2
        case layout = 3
        case all = 99
    }

    public let runtimeLibraryURL: URL?
    public let preferredInputName: String?
    public let preferredOutputName: String?
    public let graphOptimizationLevel: GraphOptimizationLevel
    public let intraOpNumThreads: Int32
    public let interOpNumThreads: Int32
    public let tensorConfiguration: DepthAnythingONNXTensorConfiguration

    public init(
        runtimeLibraryURL: URL? = nil,
        preferredInputName: String? = nil,
        preferredOutputName: String? = nil,
        graphOptimizationLevel: GraphOptimizationLevel = .extended,
        intraOpNumThreads: Int32 = 0,
        interOpNumThreads: Int32 = 0,
        tensorConfiguration: DepthAnythingONNXTensorConfiguration = DepthAnythingONNXTensorConfiguration()
    ) {
        self.runtimeLibraryURL = runtimeLibraryURL
        self.preferredInputName = preferredInputName
        self.preferredOutputName = preferredOutputName
        self.graphOptimizationLevel = graphOptimizationLevel
        self.intraOpNumThreads = max(intraOpNumThreads, 0)
        self.interOpNumThreads = max(interOpNumThreads, 0)
        self.tensorConfiguration = tensorConfiguration
    }
}

public struct DepthAnythingONNXInputTensor: Equatable, Sendable {
    public let values: [Float]
    public let shape: [Int64]
    public let width: Int
    public let height: Int
}

#if canImport(CoreImage)
public enum DepthAnythingONNXPreprocessor {
    private static let ciContext = CIContext(options: [.cacheIntermediates: false])

    public static func makeInputTensor(
        from pixelBuffer: CVPixelBuffer,
        processingConfiguration: VideoDepthProcessingConfiguration = .coreMLDefault,
        tensorConfiguration: DepthAnythingONNXTensorConfiguration = DepthAnythingONNXTensorConfiguration()
    ) throws -> DepthAnythingONNXInputTensor {
        let targetSize = processingConfiguration.inputSize ?? VideoDepthInputSize(
            width: CVPixelBufferGetWidth(pixelBuffer),
            height: CVPixelBufferGetHeight(pixelBuffer)
        )
        let width = targetSize.width
        let height = targetSize.height
        let pixelCount = width * height
        var pixels = [UInt8](repeating: 0, count: pixelCount * 4)
        let targetRect = CGRect(x: 0, y: 0, width: width, height: height)
        let image = transformedImage(
            CIImage(cvPixelBuffer: pixelBuffer),
            targetRect: targetRect,
            aspectPolicy: processingConfiguration.aspectPolicy
        )
        ciContext.render(
            image,
            toBitmap: &pixels,
            rowBytes: width * 4,
            bounds: targetRect,
            format: .RGBA8,
            colorSpace: CGColorSpaceCreateDeviceRGB()
        )

        var values = [Float](repeating: 0, count: pixelCount * 3)
        for pixelIndex in 0 ..< pixelCount {
            let offset = pixelIndex * 4
            let rgb = SIMD3<Float>(
                Float(pixels[offset]) * tensorConfiguration.scale,
                Float(pixels[offset + 1]) * tensorConfiguration.scale,
                Float(pixels[offset + 2]) * tensorConfiguration.scale
            )
            let ordered: SIMD3<Float>
            switch tensorConfiguration.channelOrder {
            case .rgb:
                ordered = rgb
            case .bgr:
                ordered = SIMD3<Float>(rgb.z, rgb.y, rgb.x)
            }
            let normalized = (ordered - tensorConfiguration.mean) / tensorConfiguration.standardDeviation
            switch tensorConfiguration.layout {
            case .nchw:
                values[pixelIndex] = normalized.x
                values[pixelCount + pixelIndex] = normalized.y
                values[pixelCount * 2 + pixelIndex] = normalized.z
            case .nhwc:
                values[pixelIndex * 3] = normalized.x
                values[pixelIndex * 3 + 1] = normalized.y
                values[pixelIndex * 3 + 2] = normalized.z
            }
        }
        let shape: [Int64]
        switch tensorConfiguration.layout {
        case .nchw:
            shape = [1, 3, Int64(height), Int64(width)]
        case .nhwc:
            shape = [1, Int64(height), Int64(width), 3]
        }
        return DepthAnythingONNXInputTensor(values: values, shape: shape, width: width, height: height)
    }

    private static func transformedImage(_ image: CIImage, targetRect: CGRect, aspectPolicy: VideoDepthAspectPolicy) -> CIImage {
        guard aspectPolicy != .stretch else {
            return image.transformed(by: CGAffineTransform(
                scaleX: targetRect.width / max(image.extent.width, 1),
                y: targetRect.height / max(image.extent.height, 1)
            )).cropped(to: targetRect)
        }
        let scaleX = targetRect.width / max(image.extent.width, 1)
        let scaleY = targetRect.height / max(image.extent.height, 1)
        let scale: CGFloat
        switch aspectPolicy {
        case .stretch:
            scale = 1
        case .scaleAspectFit:
            scale = min(scaleX, scaleY)
        case .scaleAspectFill:
            scale = max(scaleX, scaleY)
        }
        let scaledWidth = image.extent.width * scale
        let scaledHeight = image.extent.height * scale
        let offsetX = (targetRect.width - scaledWidth) * 0.5
        let offsetY = (targetRect.height - scaledHeight) * 0.5
        let transform = CGAffineTransform(scaleX: scale, y: scale).concatenating(CGAffineTransform(translationX: offsetX, y: offsetY))
        let background = CIImage(color: CIColor(red: 0, green: 0, blue: 0)).cropped(to: targetRect)
        return image.transformed(by: transform).composited(over: background).cropped(to: targetRect)
    }
}
#endif

public enum DepthAnythingONNXOutputPostprocessor {
    public static func makeDepthPrediction(
        values: [Float],
        shape: [Int64],
        outputName: String,
        metadata: [String: String] = [:]
    ) -> VideoDepthPrediction? {
        guard !values.isEmpty else {
            return nil
        }
        let safeShape = shape.map { max(Int($0), 1) }
        guard !safeShape.isEmpty else {
            return VideoDepthPrediction(width: 1, height: 1, values: [values[0]], metadata: metadata.merging(["output": outputName]) { current, _ in current })
        }
        let nonUnitAxes = safeShape.indices.filter { safeShape[$0] > 1 }
        let heightAxis: Int
        let widthAxis: Int
        if nonUnitAxes.count >= 2 {
            heightAxis = nonUnitAxes[nonUnitAxes.count - 2]
            widthAxis = nonUnitAxes[nonUnitAxes.count - 1]
        } else if let axis = nonUnitAxes.last {
            heightAxis = axis
            widthAxis = axis
        } else {
            return VideoDepthPrediction(width: 1, height: 1, values: [values[0]], metadata: metadata.merging(["output": outputName]) { current, _ in current })
        }
        let width = max(safeShape[widthAxis], 1)
        let height = nonUnitAxes.count == 1 ? 1 : max(safeShape[heightAxis], 1)
        let strides = rowMajorStrides(for: safeShape)
        let predictionValues = (0 ..< height * width).map { linearIndex -> Float in
            var index = Array(repeating: 0, count: safeShape.count)
            index[heightAxis] = nonUnitAxes.count == 1 ? 0 : linearIndex / width
            index[widthAxis] = linearIndex % width
            let offset = zip(index, strides).reduce(0) { partialResult, pair in
                partialResult + pair.0 * pair.1
            }
            guard values.indices.contains(offset) else {
                return 0.5
            }
            return values[offset]
        }
        return VideoDepthPrediction(
            width: width,
            height: height,
            values: predictionValues,
            metadata: metadata.merging(["output": outputName]) { current, _ in current }
        )
    }

    private static func rowMajorStrides(for shape: [Int]) -> [Int] {
        guard !shape.isEmpty else {
            return []
        }
        var strides = Array(repeating: 1, count: shape.count)
        if shape.count > 1 {
            for index in stride(from: shape.count - 2, through: 0, by: -1) {
                strides[index] = strides[index + 1] * shape[index + 1]
            }
        }
        return strides
    }
}

private func stringFromCStringBuffer(_ buffer: [CChar]) -> String {
    buffer.withUnsafeBufferPointer { pointer in
        guard let baseAddress = pointer.baseAddress else {
            return ""
        }
        return String(cString: baseAddress)
    }
}

#if canImport(CoreImage)
@MainActor
public final class DepthAnythingONNXRuntimeBackend: DepthAnythingONNXInferenceBackend {
    private let session: DepthAnythingONNXRuntimeSession
    private let runtimeConfiguration: DepthAnythingONNXRuntimeConfiguration

    public init(modelURL: URL, runtimeConfiguration: DepthAnythingONNXRuntimeConfiguration = DepthAnythingONNXRuntimeConfiguration()) throws {
        session = try DepthAnythingONNXRuntimeSession(modelURL: modelURL, runtimeConfiguration: runtimeConfiguration)
        self.runtimeConfiguration = runtimeConfiguration
    }

    public static func runtimeAvailability(runtimeLibraryURL: URL? = nil) -> (isAvailable: Bool, reason: String?) {
        var error = [CChar](repeating: 0, count: 1024)
        let available = error.withUnsafeMutableBufferPointer { errorBuffer in
            if let runtimeLibraryURL {
                runtimeLibraryURL.path.withCString { libraryPath in
                    KSORTIsRuntimeAvailable(libraryPath, errorBuffer.baseAddress, errorBuffer.count)
                }
            } else {
                KSORTIsRuntimeAvailable(nil, errorBuffer.baseAddress, errorBuffer.count)
            }
        }
        let reason = stringFromCStringBuffer(error)
        return (available != 0, reason.isEmpty ? nil : reason)
    }

    public func makeDepthPrediction(request: VideoDepthEstimationRequest, configuration: VideoDepthProcessingConfiguration) throws -> VideoDepthPrediction? {
        let input = try DepthAnythingONNXPreprocessor.makeInputTensor(
            from: request.pixelBuffer,
            processingConfiguration: configuration,
            tensorConfiguration: runtimeConfiguration.tensorConfiguration
        )
        let output = try session.run(input: input)
        var metadata = output.metadata
        metadata["runtime"] = "onnx"
        metadata["input"] = session.inputName
        metadata["output"] = session.outputName
        if let runtimeVersion = session.runtimeVersion {
            metadata["onnxRuntimeVersion"] = runtimeVersion
        }
        return DepthAnythingONNXOutputPostprocessor.makeDepthPrediction(
            values: output.values,
            shape: output.shape,
            outputName: session.outputName,
            metadata: metadata
        )
    }
}

@MainActor
public final class ONNXDepthEstimationProvider: VideoDepthEstimationProvider {
    public let providerID: String
    private let adapter: DepthAnythingDepthEstimationAdapter

    public init(
        providerID: String = "depth-anything-onnx",
        modelFamily: DepthAnythingDepthEstimationAdapter.ModelFamily = .depthAnythingV2,
        modelVariant: DepthAnythingDepthEstimationAdapter.ModelVariant = .small,
        modelURL: URL,
        runtimeConfiguration: DepthAnythingONNXRuntimeConfiguration = DepthAnythingONNXRuntimeConfiguration(),
        processingConfiguration: VideoDepthProcessingConfiguration = .coreMLDefault
    ) throws {
        self.providerID = providerID
        let backend = try DepthAnythingONNXRuntimeBackend(modelURL: modelURL, runtimeConfiguration: runtimeConfiguration)
        adapter = DepthAnythingDepthEstimationAdapter(
            providerID: providerID,
            modelFamily: modelFamily,
            modelVariant: modelVariant,
            modelURL: modelURL,
            processingConfiguration: processingConfiguration,
            backend: backend
        )
    }

    public func makeDepthMap(request: VideoDepthEstimationRequest) throws -> VideoDepthMap? {
        try adapter.makeDepthMap(request: request)
    }
}

private struct DepthAnythingONNXRuntimeOutput {
    let values: [Float]
    let shape: [Int64]
    let metadata: [String: String]
}

private final class DepthAnythingONNXRuntimeSession {
    let runtimeVersion: String?
    let inputName: String
    let outputName: String
    private let session: OpaquePointer

    init(modelURL: URL, runtimeConfiguration: DepthAnythingONNXRuntimeConfiguration) throws {
        var error = [CChar](repeating: 0, count: 2048)
        var createdSession: OpaquePointer?
        error.withUnsafeMutableBufferPointer { errorBuffer in
            modelURL.path.withCString { modelPath in
                Self.withOptionalCString(runtimeConfiguration.runtimeLibraryURL?.path) { runtimeLibraryPath in
                    Self.withOptionalCString(runtimeConfiguration.preferredInputName) { preferredInputName in
                        Self.withOptionalCString(runtimeConfiguration.preferredOutputName) { preferredOutputName in
                            let configuration = KSORTSessionConfiguration(
                                runtime_library_path: runtimeLibraryPath,
                                preferred_input_name: preferredInputName,
                                preferred_output_name: preferredOutputName,
                                graph_optimization_level: runtimeConfiguration.graphOptimizationLevel.rawValue,
                                intra_op_num_threads: runtimeConfiguration.intraOpNumThreads,
                                inter_op_num_threads: runtimeConfiguration.interOpNumThreads
                            )
                            createdSession = KSORTCreateSession(modelPath, configuration, errorBuffer.baseAddress, errorBuffer.count)
                        }
                    }
                }
            }
        }
        guard let createdSession else {
            throw VideoDepthEstimationError.runtimeUnavailable(Self.errorMessage(from: error, fallback: "Unable to create ONNX Runtime session."))
        }
        session = createdSession
        runtimeVersion = KSORTGetRuntimeVersion(createdSession).map { String(cString: $0) }
        inputName = KSORTGetInputName(createdSession).map { String(cString: $0) } ?? runtimeConfiguration.preferredInputName ?? "input"
        outputName = KSORTGetOutputName(createdSession).map { String(cString: $0) } ?? runtimeConfiguration.preferredOutputName ?? "output"
    }

    deinit {
        KSORTReleaseSession(session)
    }

    func run(input: DepthAnythingONNXInputTensor) throws -> DepthAnythingONNXRuntimeOutput {
        var error = [CChar](repeating: 0, count: 2048)
        var outputData: UnsafeMutablePointer<Float>?
        var outputCount = 0
        var outputShape: UnsafeMutablePointer<Int64>?
        var outputShapeCount = 0
        let result = error.withUnsafeMutableBufferPointer { errorBuffer in
            input.values.withUnsafeBufferPointer { valuesBuffer in
                input.shape.withUnsafeBufferPointer { shapeBuffer in
                    KSORTRunFloat32(
                        session,
                        valuesBuffer.baseAddress,
                        valuesBuffer.count,
                        shapeBuffer.baseAddress,
                        shapeBuffer.count,
                        &outputData,
                        &outputCount,
                        &outputShape,
                        &outputShapeCount,
                        errorBuffer.baseAddress,
                        errorBuffer.count
                    )
                }
            }
        }
        guard result != 0, let outputData, let outputShape else {
            if let outputData {
                KSORTReleaseBuffer(outputData)
            }
            if let outputShape {
                KSORTReleaseBuffer(outputShape)
            }
            throw VideoDepthEstimationError.inferenceFailed(Self.errorMessage(from: error, fallback: "ONNX Runtime inference failed."))
        }
        let values = Array(UnsafeBufferPointer(start: outputData, count: outputCount))
        let shape = Array(UnsafeBufferPointer(start: outputShape, count: outputShapeCount))
        KSORTReleaseBuffer(outputData)
        KSORTReleaseBuffer(outputShape)
        return DepthAnythingONNXRuntimeOutput(values: values, shape: shape, metadata: ["outputShape": shape.map { String($0) }.joined(separator: "x")])
    }

    private static func withOptionalCString<Result>(_ string: String?, _ body: (UnsafePointer<CChar>?) throws -> Result) rethrows -> Result {
        guard let string else {
            return try body(nil)
        }
        return try string.withCString(body)
    }

    private static func errorMessage(from buffer: [CChar], fallback: String) -> String {
        let message = stringFromCStringBuffer(buffer)
        return message.isEmpty ? fallback : message
    }
}
#endif

#if canImport(CoreML) && canImport(CoreImage)
extension DepthAnythingDepthEstimationAdapter {
    public convenience init(
        providerID: String = "depth-anything-coreml",
        modelFamily: ModelFamily = .depthAnythingV2,
        modelVariant: ModelVariant = .small,
        modelURL: URL,
        modelConfiguration: MLModelConfiguration = MLModelConfiguration(),
        processingConfiguration: VideoDepthProcessingConfiguration = .coreMLDefault
    ) throws {
        guard Video2DTo3DPolicy.isSupportedPlatform else {
            throw VideoDepthEstimationError.unavailablePlatform(Video2DTo3DPolicy.unavailablePlatformReason)
        }
        let coreMLInference = try DepthAnythingV2CoreMLInference(
            modelURL: modelURL,
            modelConfiguration: modelConfiguration,
            processingConfiguration: processingConfiguration
        )
        self.init(
            providerID: providerID,
            runtime: .coreML,
            modelFamily: modelFamily,
            modelVariant: modelVariant,
            modelURL: modelURL,
            processingConfiguration: processingConfiguration,
            allowsUnsupportedPlatformInference: false
        ) { request in
            try coreMLInference.makeDepthPrediction(request: request)
        }
    }
}

@MainActor
private final class DepthAnythingV2CoreMLInference {
    private static let ciContext = CIContext(options: [.cacheIntermediates: false])

    private let model: MLModel
    private let processingConfiguration: VideoDepthProcessingConfiguration
    private let inputDescription: MLFeatureDescription
    private let inputName: String

    init(modelURL: URL, modelConfiguration: MLModelConfiguration, processingConfiguration: VideoDepthProcessingConfiguration) throws {
        model = try MLModel(contentsOf: modelURL, configuration: modelConfiguration)
        self.processingConfiguration = processingConfiguration
        guard let input = model.modelDescription.inputDescriptionsByName.first(where: { $0.value.type == .image }) else {
            throw VideoDepthEstimationError.unsupportedModelInput("Depth Anything Core ML adapter requires an image input feature.")
        }
        inputName = input.key
        inputDescription = input.value
    }

    func makeDepthPrediction(request: VideoDepthEstimationRequest) throws -> VideoDepthPrediction? {
        let inputBuffer = try resizedInputBuffer(from: request.pixelBuffer)
        let input = try MLDictionaryFeatureProvider(dictionary: [inputName: MLFeatureValue(pixelBuffer: inputBuffer)])
        let output = try model.prediction(from: input)
        return try depthPrediction(from: output)
    }

    private func resizedInputBuffer(from pixelBuffer: CVPixelBuffer) throws -> CVPixelBuffer {
        let targetSize = targetInputSize(source: pixelBuffer)
        var outputBuffer: CVPixelBuffer?
        let attributes: [CFString: Any] = [
            kCVPixelBufferCGImageCompatibilityKey: true,
            kCVPixelBufferCGBitmapContextCompatibilityKey: true,
            kCVPixelBufferIOSurfacePropertiesKey: [:],
        ]
        let status = CVPixelBufferCreate(
            kCFAllocatorDefault,
            targetSize.width,
            targetSize.height,
            kCVPixelFormatType_32BGRA,
            attributes as CFDictionary,
            &outputBuffer
        )
        guard status == kCVReturnSuccess, let outputBuffer else {
            throw VideoDepthEstimationError.preprocessingFailed("Unable to allocate Depth Anything Core ML input buffer.")
        }

        let sourceImage = CIImage(cvPixelBuffer: pixelBuffer)
        let targetRect = CGRect(x: 0, y: 0, width: targetSize.width, height: targetSize.height)
        let transformed = transformedImage(sourceImage, targetRect: targetRect)
        Self.ciContext.render(transformed, to: outputBuffer, bounds: targetRect, colorSpace: CGColorSpaceCreateDeviceRGB())
        return outputBuffer
    }

    private func targetInputSize(source pixelBuffer: CVPixelBuffer) -> VideoDepthInputSize {
        if let inputSize = processingConfiguration.inputSize {
            return inputSize
        }
        if let imageConstraint = inputDescription.imageConstraint,
           imageConstraint.pixelsWide > 0,
           imageConstraint.pixelsHigh > 0
        {
            return VideoDepthInputSize(width: imageConstraint.pixelsWide, height: imageConstraint.pixelsHigh)
        }
        return VideoDepthInputSize(width: CVPixelBufferGetWidth(pixelBuffer), height: CVPixelBufferGetHeight(pixelBuffer))
    }

    private func transformedImage(_ image: CIImage, targetRect: CGRect) -> CIImage {
        guard processingConfiguration.aspectPolicy != .stretch else {
            return image.transformed(by: CGAffineTransform(
                scaleX: targetRect.width / max(image.extent.width, 1),
                y: targetRect.height / max(image.extent.height, 1)
            )).cropped(to: targetRect)
        }
        let scaleX = targetRect.width / max(image.extent.width, 1)
        let scaleY = targetRect.height / max(image.extent.height, 1)
        let scale: CGFloat
        switch processingConfiguration.aspectPolicy {
        case .stretch:
            scale = 1
        case .scaleAspectFit:
            scale = min(scaleX, scaleY)
        case .scaleAspectFill:
            scale = max(scaleX, scaleY)
        }
        let scaledWidth = image.extent.width * scale
        let scaledHeight = image.extent.height * scale
        let offsetX = (targetRect.width - scaledWidth) * 0.5
        let offsetY = (targetRect.height - scaledHeight) * 0.5
        let transform = CGAffineTransform(scaleX: scale, y: scale).concatenating(CGAffineTransform(translationX: offsetX, y: offsetY))
        let scaled = image.transformed(by: transform)
        let background = CIImage(color: CIColor(red: 0, green: 0, blue: 0)).cropped(to: targetRect)
        return scaled.composited(over: background).cropped(to: targetRect)
    }

    private func depthPrediction(from output: MLFeatureProvider) throws -> VideoDepthPrediction? {
        let featureNames = Array(output.featureNames)
        let orderedNames: [String]
        if let preferredOutputFeatureName = processingConfiguration.preferredOutputFeatureName {
            orderedNames = [preferredOutputFeatureName] + featureNames.filter { $0 != preferredOutputFeatureName }
        } else {
            orderedNames = featureNames.sorted { lhs, rhs in
                outputPriority(lhs) < outputPriority(rhs)
            }
        }

        for name in orderedNames {
            guard let feature = output.featureValue(for: name) else {
                continue
            }
            if let multiArray = feature.multiArrayValue,
               let prediction = Self.depthPrediction(from: multiArray, outputName: name)
            {
                return prediction
            }
            if let imageBuffer = feature.imageBufferValue,
               let prediction = Self.depthPrediction(from: imageBuffer, outputName: name)
            {
                return prediction
            }
        }
        throw VideoDepthEstimationError.unsupportedModelOutput("Depth Anything Core ML adapter could not find a depth output feature.")
    }

    private func outputPriority(_ name: String) -> Int {
        let lowercased = name.lowercased()
        if lowercased.contains("depth") {
            return 0
        }
        if lowercased.contains("disparity") {
            return 1
        }
        if lowercased.contains("prediction") {
            return 2
        }
        if lowercased.contains("confidence") {
            return 9
        }
        return 3
    }

    private static func depthPrediction(from multiArray: MLMultiArray, outputName: String) -> VideoDepthPrediction? {
        let shape = multiArray.shape.map(\.intValue)
        guard !shape.isEmpty else {
            return nil
        }
        let nonUnitAxes = shape.indices.filter { shape[$0] > 1 }
        let heightAxis: Int
        let widthAxis: Int
        if nonUnitAxes.count >= 2 {
            heightAxis = nonUnitAxes[nonUnitAxes.count - 2]
            widthAxis = nonUnitAxes[nonUnitAxes.count - 1]
        } else if let axis = nonUnitAxes.last {
            heightAxis = axis
            widthAxis = axis
        } else {
            return VideoDepthPrediction(width: 1, height: 1, values: [multiArray[[]].floatValue], metadata: ["output": outputName])
        }
        let width = max(shape[widthAxis], 1)
        let height = nonUnitAxes.count == 1 ? 1 : max(shape[heightAxis], 1)
        let values = (0 ..< height * width).map { linearIndex -> Float in
            var index = Array(repeating: 0, count: shape.count)
            index[heightAxis] = nonUnitAxes.count == 1 ? 0 : linearIndex / width
            index[widthAxis] = linearIndex % width
            return multiArray[index.map { NSNumber(value: $0) }].floatValue
        }
        return VideoDepthPrediction(width: width, height: height, values: values, metadata: ["output": outputName])
    }

    private static func depthPrediction(from pixelBuffer: CVPixelBuffer, outputName: String) -> VideoDepthPrediction? {
        let width = CVPixelBufferGetWidth(pixelBuffer)
        let height = CVPixelBufferGetHeight(pixelBuffer)
        guard width > 0, height > 0 else {
            return nil
        }
        var pixels = [UInt8](repeating: 0, count: width * height * 4)
        let image = CIImage(cvPixelBuffer: pixelBuffer)
        let rect = CGRect(x: 0, y: 0, width: width, height: height)
        ciContext.render(
            image,
            toBitmap: &pixels,
            rowBytes: width * 4,
            bounds: rect,
            format: .RGBA8,
            colorSpace: CGColorSpaceCreateDeviceRGB()
        )
        let values = stride(from: 0, to: pixels.count, by: 4).map { offset -> Float in
            let red = Float(pixels[offset])
            let green = Float(pixels[offset + 1])
            let blue = Float(pixels[offset + 2])
            return (red * 0.2126 + green * 0.7152 + blue * 0.0722) / 255
        }
        return VideoDepthPrediction(width: width, height: height, values: values, metadata: ["output": outputName])
    }
}
#endif

public struct Video2DTo3DRenderConfiguration: Equatable, Sendable {
    static let disabled = Video2DTo3DRenderConfiguration(
        isEnabled: false,
        outputLayout: .selectedEye,
        selectedEye: .left,
        depthStrength: 0,
        depthDistance: 1,
        depthCurvature: 1,
        usesDepthMap: false
    )

    public let isEnabled: Bool
    public let outputLayout: Video2DTo3DOutputLayout
    public let selectedEye: StereoscopicVideoEye
    public let depthStrength: Float
    public let depthDistance: Float
    public let depthCurvature: Float
    public let usesDepthMap: Bool

    func drawableSize(for baseSize: CGSize) -> CGSize {
        guard isEnabled else {
            return baseSize
        }
        return outputLayout.drawableSize(for: baseSize)
    }

    func fragmentUniform(for eye: StereoscopicVideoEye) -> SIMD4<Float> {
        guard isEnabled else {
            return SIMD4<Float>(0, 0, 0, 0)
        }
        let eyeSign: Float = eye == .left ? -1 : 1
        return SIMD4<Float>(1, depthStrength, usesDepthMap ? 1 : 0, eyeSign)
    }

    func shapeUniform() -> SIMD4<Float> {
        guard isEnabled else {
            return SIMD4<Float>(1, 1, 0, 0)
        }
        return SIMD4<Float>(depthDistance, depthCurvature, 0, 0)
    }
}

public enum Video2DTo3DPolicy {
    public static let defaultDepthStrength: Float = 0.35
    public static let depthStrengthRange: ClosedRange<Float> = 0 ... 1
    public static let defaultDepthDistance: Float = 1
    public static let depthDistanceRange: ClosedRange<Float> = 0 ... 2
    public static let defaultDepthCurvature: Float = 1
    public static let depthCurvatureRange: ClosedRange<Float> = 0.25 ... 3
    public static let unavailablePlatformReason = "2D-to-3D conversion is available only on visionOS / Apple Vision Pro."

    public static var isSupportedPlatform: Bool {
        #if os(xrOS) || os(visionOS)
        true
        #else
        false
        #endif
    }

    public static func validatedDepthStrength(_ value: Float) -> Float {
        guard value.isFinite else {
            return defaultDepthStrength
        }
        return min(max(value, depthStrengthRange.lowerBound), depthStrengthRange.upperBound)
    }

    public static func validatedDepthDistance(_ value: Float) -> Float {
        guard value.isFinite else {
            return defaultDepthDistance
        }
        return min(max(value, depthDistanceRange.lowerBound), depthDistanceRange.upperBound)
    }

    public static func validatedDepthCurvature(_ value: Float) -> Float {
        guard value.isFinite else {
            return defaultDepthCurvature
        }
        return min(max(value, depthCurvatureRange.lowerBound), depthCurvatureRange.upperBound)
    }

    public static func unavailableReason(mode: Video2DTo3DMode) -> String? {
        guard mode.isEnabled, !isSupportedPlatform else {
            return nil
        }
        return unavailablePlatformReason
    }

    public static func requiresMetalRenderPath(mode: Video2DTo3DMode) -> Bool {
        mode.isEnabled && isSupportedPlatform
    }

    public static func renderConfiguration(
        mode: Video2DTo3DMode,
        depthStrength: Float,
        depthDistance: Float,
        depthCurvature: Float,
        outputLayout: Video2DTo3DOutputLayout,
        selectedEye: StereoscopicVideoEye,
        display: DisplayEnum,
        stereoscopicVideoLayout: StereoscopicVideoLayout,
        hasDepthMap: Bool
    ) -> Video2DTo3DRenderConfiguration {
        guard mode.isEnabled, isSupportedPlatform, display == .plane, stereoscopicVideoLayout == .mono else {
            return .disabled
        }
        return Video2DTo3DRenderConfiguration(
            isEnabled: true,
            outputLayout: outputLayout,
            selectedEye: selectedEye,
            depthStrength: validatedDepthStrength(depthStrength),
            depthDistance: validatedDepthDistance(depthDistance),
            depthCurvature: validatedDepthCurvature(depthCurvature),
            usesDepthMap: mode == .depthMapPreferred && hasDepthMap
        )
    }
}

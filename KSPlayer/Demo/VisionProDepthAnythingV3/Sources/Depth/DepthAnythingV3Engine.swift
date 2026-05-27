#if os(visionOS) && canImport(CoreImage) && canImport(CoreML)
@preconcurrency import CoreImage
@preconcurrency import CoreML
@preconcurrency import CoreVideo
import Foundation
import KSPlayer
#if canImport(Metal)
import Metal
#endif

public enum DepthAnythingV3EngineError: Error, LocalizedError, Sendable {
    case modelResourceNotFound(String)
    case noSupportedInput([String])
    case unsupportedInputFeature(name: String, type: String)
    case unsupportedMultiArrayShape(name: String, shape: [Int])
    case unsupportedMultiArrayDataType(name: String, dataType: String)
    case pixelBufferAllocationFailed(CVReturn)
    case pixelBufferLockFailed(CVReturn)
    case pixelBufferBaseAddressUnavailable
    case outputNotFound(preferred: String, available: [String])
    case outputIsNotMultiArray(String)

    public var errorDescription: String? {
        switch self {
        case let .modelResourceNotFound(resource):
            return "Depth Anything V3 model resource '\(resource).mlmodelc' was not found in the app bundle."
        case let .noSupportedInput(available):
            return "Depth Anything V3 model has no image or multi-array input. Available inputs: \(available.joined(separator: ", "))."
        case let .unsupportedInputFeature(name, type):
            return "Depth Anything V3 input '\(name)' uses unsupported feature type \(type)."
        case let .unsupportedMultiArrayShape(name, shape):
            return "Depth Anything V3 input '\(name)' must be RGB [3, H, W], [1, 3, H, W], or [1, 1, 3, H, W]; got \(shape)."
        case let .unsupportedMultiArrayDataType(name, dataType):
            return "Depth Anything V3 input '\(name)' uses unsupported multi-array data type \(dataType)."
        case let .pixelBufferAllocationFailed(status):
            return "Unable to allocate Depth Anything V3 preprocessing pixel buffer (CVReturn \(status))."
        case let .pixelBufferLockFailed(status):
            return "Unable to lock Depth Anything V3 preprocessing pixel buffer (CVReturn \(status))."
        case .pixelBufferBaseAddressUnavailable:
            return "Depth Anything V3 preprocessing pixel buffer has no CPU-readable base address."
        case let .outputNotFound(preferred, available):
            return "Depth Anything V3 output '\(preferred)' was not found and no multi-array fallback was available. Available outputs: \(available.joined(separator: ", "))."
        case let .outputIsNotMultiArray(name):
            return "Depth Anything V3 output '\(name)' is not an MLMultiArray."
        }
    }
}

public struct DA3EngineTimings: Equatable, Sendable {
    public let preprocessingDuration: TimeInterval
    public let inferenceDuration: TimeInterval
    public let outputExtractionDuration: TimeInterval
    public let totalDuration: TimeInterval

    public static let empty = DA3EngineTimings(
        preprocessingDuration: 0,
        inferenceDuration: 0,
        outputExtractionDuration: 0,
        totalDuration: 0
    )
}

public struct DA3ModelDiagnostics: Equatable, Sendable {
    public let modelName: String
    public let inputName: String
    public let outputName: String
    public let inputKind: String
    public let inputWidth: Int
    public let inputHeight: Int
    public let supportsFlexibleInputShape: Bool
    public let requestedInputWidth: Int?
    public let requestedInputHeight: Int?
    public let computeUnits: String

    public var inputSizeSummary: String {
        guard inputWidth > 0, inputHeight > 0 else {
            return "source"
        }
        return "\(inputWidth)x\(inputHeight)"
    }

    public var requestedInputSizeSummary: String? {
        guard let requestedInputWidth, let requestedInputHeight else {
            return nil
        }
        return "\(requestedInputWidth)x\(requestedInputHeight)"
    }

    public var inputShapeNote: String {
        if supportsFlexibleInputShape, let requestedInputSizeSummary {
            return "flexible input using \(requestedInputSizeSummary)"
        }
        if supportsFlexibleInputShape {
            return "flexible input"
        }
        if let requestedInputSizeSummary, requestedInputSizeSummary != inputSizeSummary {
            return "fixed \(inputSizeSummary); requested \(requestedInputSizeSummary) ignored"
        }
        return "fixed \(inputSizeSummary)"
    }

    public var computeUnitsNote: String {
        switch computeUnits {
        case "all":
            return "Core ML .all (ANE/GPU/CPU); profile in Instruments if inference is slow"
        case "cpuAndNeuralEngine":
            return "Core ML .cpuAndNeuralEngine (ANE-preferred; ops unsupported on ANE fall back to CPU)"
        case "cpuOnly":
            return "Core ML .cpuOnly (expect low FPS — avoid on Vision Pro)"
        case "cpuAndGPU":
            return "Core ML .cpuAndGPU"
        default:
            return "Core ML computeUnits \(computeUnits)"
        }
    }

    public static let empty = DA3ModelDiagnostics(
        modelName: "",
        inputName: "",
        outputName: "",
        inputKind: "",
        inputWidth: 0,
        inputHeight: 0,
        supportsFlexibleInputShape: false,
        requestedInputWidth: nil,
        requestedInputHeight: nil,
        computeUnits: ""
    )
}

public struct DA3DepthPredictionResult: Sendable {
    public let depthOutput: DA3DepthOutput
    public let frame: DA3DepthFrame?
    public let timings: DA3EngineTimings
    public let diagnostics: DA3ModelDiagnostics
}

/// App-target engine for dynamically loading a bundled compiled Depth Anything V3 `.mlmodelc`.
public actor DepthAnythingV3Engine {
    private enum InputKind {
        case image(fixedSize: VideoDepthInputSize?)
        case multiArray(shape: [Int], dataType: MLMultiArrayDataType)
    }

    private struct InputDescriptor {
        let name: String
        let kind: InputKind
    }

    private struct LoadedModel {
        let model: MLModel
        let input: InputDescriptor
        let outputName: String
    }

    private let modelURL: URL
    private let requestedConfiguration: MLModelConfiguration
    private let allowsComputeUnitFallback: Bool
    private var effectiveConfiguration: MLModelConfiguration
    private let preferredInputName: String?
    private let preferredOutputName: String
    private let requestedInputSize: VideoDepthInputSize?
    private let imageContext: CIContext
    private let colorSpace = CGColorSpaceCreateDeviceRGB()
    private var loadedModel: LoadedModel?
    private var resizedPixelBufferPool: CVPixelBufferPool?
    private var resizedPixelBufferPoolSize: VideoDepthInputSize?

    public static func defaultModelURL(in bundle: Bundle = .main) -> URL? {
        bundle.url(forResource: "da3-small", withExtension: "mlmodelc")
    }

    public static func defaultModelConfiguration() -> MLModelConfiguration {
        let configuration = MLModelConfiguration()
        configuration.computeUnits = .all
        return configuration
    }

    /// Prefer Apple Neural Engine. Public API uses `.cpuAndNeuralEngine` (strict NE-only is not exposed on all SDKs).
    public static func neuralEnginePreferredConfiguration() -> MLModelConfiguration {
        let configuration = MLModelConfiguration()
        configuration.computeUnits = .cpuAndNeuralEngine
        return configuration
    }

    /// Same as `neuralEnginePreferredConfiguration()`; demo default for Vision Pro.
    public static func fastModelConfiguration() -> MLModelConfiguration {
        neuralEnginePreferredConfiguration()
    }

    /// `DA3_COMPUTE_UNITS=strict_ane|ane|all|cpu` scheme override for device experiments.
    public static func resolvedModelConfiguration(
        allowsFallback: Bool = true
    ) -> (configuration: MLModelConfiguration, allowsFallback: Bool) {
        guard let raw = ProcessInfo.processInfo.environment["DA3_COMPUTE_UNITS"]?
            .trimmingCharacters(in: .whitespacesAndNewlines)
            .lowercased(),
            !raw.isEmpty
        else {
            return (neuralEnginePreferredConfiguration(), allowsFallback)
        }
        switch raw {
        case "strict_ane", "neural_engine_only", "neuralengineonly":
            return (neuralEnginePreferredConfiguration(), false)
        case "ane", "neural", "cpuandneuralengine":
            return (neuralEnginePreferredConfiguration(), allowsFallback)
        case "all":
            return (defaultModelConfiguration(), allowsFallback)
        case "cpu", "cpuonly":
            var configuration = MLModelConfiguration()
            configuration.computeUnits = .cpuOnly
            return (configuration, allowsFallback)
        case "gpu", "cpuandgpu":
            var configuration = MLModelConfiguration()
            configuration.computeUnits = .cpuAndGPU
            return (configuration, allowsFallback)
        default:
            return (neuralEnginePreferredConfiguration(), allowsFallback)
        }
    }

    public init(
        modelURL: URL? = nil,
        bundle: Bundle = .main,
        configuration: MLModelConfiguration? = nil,
        allowsComputeUnitFallback: Bool? = nil,
        inputName: String? = nil,
        outputName: String = "depth",
        requestedInputSize: VideoDepthInputSize? = nil
    ) throws {
        guard let resolvedModelURL = modelURL ?? Self.defaultModelURL(in: bundle) else {
            throw DepthAnythingV3EngineError.modelResourceNotFound("da3-small")
        }
        self.modelURL = resolvedModelURL
        let resolved: (configuration: MLModelConfiguration, allowsFallback: Bool)
        if let configuration {
            resolved = (configuration, allowsComputeUnitFallback ?? true)
        } else {
            resolved = Self.resolvedModelConfiguration(allowsFallback: allowsComputeUnitFallback ?? true)
        }
        self.requestedConfiguration = resolved.configuration
        self.allowsComputeUnitFallback = resolved.allowsFallback
        self.effectiveConfiguration = resolved.configuration
        self.preferredInputName = inputName
        self.preferredOutputName = outputName
        self.requestedInputSize = requestedInputSize
        #if canImport(Metal)
        if let device = MTLCreateSystemDefaultDevice() {
            self.imageContext = CIContext(
                mtlDevice: device,
                options: [.cacheIntermediates: false, .name: "DepthAnythingV3Resize"]
            )
        } else {
            self.imageContext = CIContext(options: [.cacheIntermediates: false])
        }
        #else
        self.imageContext = CIContext(options: [.cacheIntermediates: false])
        #endif
    }

    public func makeDepthFrame(from pixelBuffer: CVPixelBuffer) throws -> DA3DepthFrame {
        let prediction = try makeDepthPrediction(from: pixelBuffer)
        if let frame = prediction.frame {
            return frame
        }
        return try DA3DepthFrame(
            multiArray: prediction.depthOutput.multiArray,
            outputName: prediction.diagnostics.outputName,
            metadata: [
                "input": prediction.diagnostics.inputName,
                "model": prediction.diagnostics.modelName,
                "computeUnits": prediction.diagnostics.computeUnits,
            ]
        )
    }

    public func prepare() throws -> DA3ModelDiagnostics {
        try Task.checkCancellation()
        Depth3DDebug.log("Preparing Core ML model at \(modelURL.lastPathComponent)", phase: "coreml-prepare")
        let diagnostics = try makeDiagnostics(for: loadModelIfNeeded())
        Depth3DDebug.log("Core ML model ready: \(diagnostics.inputName) \(diagnostics.inputKind) \(diagnostics.inputSizeSummary), output \(diagnostics.outputName)", phase: "coreml-prepare")
        return diagnostics
    }

    public func makeDepthPrediction(from pixelBuffer: CVPixelBuffer) throws -> DA3DepthPredictionResult {
        try Task.checkCancellation()
        let totalStart = Date().timeIntervalSinceReferenceDate
        let loadedModel = try loadModelIfNeeded()
        let preprocessingStart = Date().timeIntervalSinceReferenceDate
        Depth3DDebug.log("Core ML input pixel buffer \(Self.pixelBufferDescription(pixelBuffer))", phase: "coreml-input", verboseOnly: true)
        let input: MLFeatureProvider
        do {
            input = try makeInputProvider(for: pixelBuffer, input: loadedModel.input)
            Depth3DDebug.log("Core ML provided input \(Self.featureProviderDescription(input, inputName: loadedModel.input.name))", phase: "coreml-input", verboseOnly: true)
        } catch {
            Depth3DDebug.fail("Core ML input creation failed: \(error.localizedDescription). Model \(Self.modelDescription(loadedModel)) source \(Self.pixelBufferDescription(pixelBuffer))", phase: "coreml-input")
            throw error
        }
        let preprocessingDuration = Date().timeIntervalSinceReferenceDate - preprocessingStart
        let inferenceStart = Date().timeIntervalSinceReferenceDate
        let output: MLFeatureProvider
        do {
            output = try loadedModel.model.prediction(from: input)
        } catch {
            Depth3DDebug.fail("Core ML prediction failed: \(error.localizedDescription). Model \(Self.modelDescription(loadedModel)) input \(Self.featureProviderDescription(input, inputName: loadedModel.input.name))", phase: "coreml-prediction")
            throw error
        }
        let inferenceDuration = Date().timeIntervalSinceReferenceDate - inferenceStart
        if inferenceDuration > 0.15 {
            Depth3DDebug.warn(
                String(
                    format: "Core ML inference %.0f ms — likely CPU/GPU fallback or oversized graph. Profile with Xcode Core ML template; re-export depth-only FP16 for ANE.",
                    inferenceDuration * 1_000
                ),
                phase: "coreml-prediction"
            )
        }
        try Task.checkCancellation()
        let extractionStart = Date().timeIntervalSinceReferenceDate
        guard let depth = output.featureValue(for: loadedModel.outputName)?.multiArrayValue else {
            Depth3DDebug.fail("Core ML output \(loadedModel.outputName) missing multiArray. Available outputs: \(Array(output.featureNames).sorted().joined(separator: ", "))", phase: "coreml-output")
            throw DepthAnythingV3EngineError.outputIsNotMultiArray(loadedModel.outputName)
        }
        let shape = depth.shape.map(\.intValue)
        guard let dimensions = DA3DepthNumericRange.dimensions(for: shape) else {
            throw DepthAnythingV3EngineError.unsupportedMultiArrayShape(
                name: loadedModel.outputName,
                shape: shape
            )
        }
        let range = DA3DepthNumericRange.minMax(
            multiArray: depth,
            shape: shape,
            width: dimensions.width,
            height: dimensions.height
        )
        let depthOutput = DA3DepthOutput(
            multiArray: depth,
            width: dimensions.width,
            height: dimensions.height,
            rawMinimum: range.minimum,
            rawMaximum: range.maximum
        )
        let extractionDuration = Date().timeIntervalSinceReferenceDate - extractionStart
        let totalDuration = Date().timeIntervalSinceReferenceDate - totalStart
        return DA3DepthPredictionResult(
            depthOutput: depthOutput,
            frame: nil,
            timings: DA3EngineTimings(
                preprocessingDuration: preprocessingDuration,
                inferenceDuration: inferenceDuration,
                outputExtractionDuration: extractionDuration,
                totalDuration: totalDuration
            ),
            diagnostics: makeDiagnostics(for: loadedModel)
        )
    }

    private func loadModelIfNeeded() throws -> LoadedModel {
        if let loadedModel {
            return loadedModel
        }

        let model: MLModel
        do {
            model = try loadModelWithComputeUnitPolicy()
        } catch {
            Depth3DDebug.fail("Unable to load Core ML model \(modelURL.lastPathComponent): \(error.localizedDescription)", phase: "coreml-load")
            throw error
        }
        Depth3DDebug.log(
            "Loaded Core ML model computeUnits=\(Self.computeUnitsDescription(effectiveConfiguration.computeUnits)) inputs \(Self.featureDescriptions(model.modelDescription.inputDescriptionsByName)); outputs \(Self.featureDescriptions(model.modelDescription.outputDescriptionsByName))",
            phase: "coreml-load"
        )
        let input = try Self.selectInput(
            from: model.modelDescription,
            preferredName: preferredInputName
        )
        let outputName = try Self.selectOutputName(
            from: model.modelDescription,
            preferredName: preferredOutputName
        )
        let loadedModel = LoadedModel(model: model, input: input, outputName: outputName)
        self.loadedModel = loadedModel
        return loadedModel
    }

    private func loadModelWithComputeUnitPolicy() throws -> MLModel {
        var candidates: [MLModelConfiguration] = [requestedConfiguration]
        if allowsComputeUnitFallback,
           requestedConfiguration.computeUnits == .cpuAndNeuralEngine
        {
            candidates.append(Self.defaultModelConfiguration())
        }
        var lastError: Error?
        for candidate in candidates {
            do {
                let model = try MLModel(contentsOf: modelURL, configuration: candidate)
                effectiveConfiguration = candidate
                if candidate.computeUnits != requestedConfiguration.computeUnits {
                    Depth3DDebug.warn(
                        "ANE-preferred load failed; loaded \(modelURL.lastPathComponent) with computeUnits .all instead. Set DA3_COMPUTE_UNITS=strict_ane to fail fast.",
                        phase: "coreml-load"
                    )
                }
                return model
            } catch {
                lastError = error
            }
        }
        throw lastError ?? DepthAnythingV3EngineError.modelResourceNotFound(modelURL.lastPathComponent)
    }

    private static func selectInput(
        from description: MLModelDescription,
        preferredName: String?
    ) throws -> InputDescriptor {
        let inputs = description.inputDescriptionsByName
        if let preferredName, let preferred = inputs[preferredName] {
            return try inputDescriptor(name: preferredName, feature: preferred)
        }

        if let image = inputs["image"] {
            return try inputDescriptor(name: "image", feature: image)
        }

        let sortedInputs = inputs.sorted { $0.key < $1.key }
        if let imageInput = sortedInputs.first(where: { $0.value.type == .image }) {
            return try inputDescriptor(name: imageInput.key, feature: imageInput.value)
        }
        if let multiArrayInput = sortedInputs.first(where: { $0.value.type == .multiArray }) {
            return try inputDescriptor(name: multiArrayInput.key, feature: multiArrayInput.value)
        }

        throw DepthAnythingV3EngineError.noSupportedInput(sortedInputs.map(\.key))
    }

    private static func inputDescriptor(name: String, feature: MLFeatureDescription) throws -> InputDescriptor {
        switch feature.type {
        case .image:
            let fixedSize: VideoDepthInputSize?
            if let constraint = feature.imageConstraint,
               constraint.pixelsWide > 0,
               constraint.pixelsHigh > 0
            {
                fixedSize = VideoDepthInputSize(width: constraint.pixelsWide, height: constraint.pixelsHigh)
            } else {
                fixedSize = nil
            }
            return InputDescriptor(name: name, kind: .image(fixedSize: fixedSize))
        case .multiArray:
            guard let constraint = feature.multiArrayConstraint else {
                throw DepthAnythingV3EngineError.unsupportedInputFeature(name: name, type: "\(feature.type)")
            }
            return InputDescriptor(
                name: name,
                kind: .multiArray(
                    shape: constraint.shape.map(\.intValue),
                    dataType: constraint.dataType
                )
            )
        default:
            throw DepthAnythingV3EngineError.unsupportedInputFeature(name: name, type: "\(feature.type)")
        }
    }

    private static func selectOutputName(
        from description: MLModelDescription,
        preferredName: String
    ) throws -> String {
        let outputs = description.outputDescriptionsByName
        if outputs[preferredName]?.type == .multiArray {
            return preferredName
        }

        if let multiArrayOutput = outputs.sorted(by: { $0.key < $1.key }).first(where: { $0.value.type == .multiArray }) {
            return multiArrayOutput.key
        }

        throw DepthAnythingV3EngineError.outputNotFound(
            preferred: preferredName,
            available: outputs.keys.sorted()
        )
    }

    private func makeInputProvider(
        for pixelBuffer: CVPixelBuffer,
        input: InputDescriptor
    ) throws -> MLFeatureProvider {
        let value: MLFeatureValue
        switch input.kind {
        case let .image(fixedSize):
            if let targetSize = fixedSize ?? requestedInputSize {
                value = MLFeatureValue(pixelBuffer: try makeResizedBGRA8PixelBuffer(
                    from: pixelBuffer,
                    width: targetSize.width,
                    height: targetSize.height
                ))
            } else {
                value = MLFeatureValue(pixelBuffer: pixelBuffer)
            }
        case let .multiArray(shape, dataType):
            value = try MLFeatureValue(multiArray: makeImageMultiArray(
                from: pixelBuffer,
                inputName: input.name,
                shape: shape,
                dataType: dataType
            ))
        }
        return try MLDictionaryFeatureProvider(dictionary: [input.name: value])
    }

    private func makeImageMultiArray(
        from pixelBuffer: CVPixelBuffer,
        inputName: String,
        shape: [Int],
        dataType: MLMultiArrayDataType
    ) throws -> MLMultiArray {
        guard shape.count >= 3 else {
            throw DepthAnythingV3EngineError.unsupportedMultiArrayShape(name: inputName, shape: shape)
        }
        let channelAxis = shape.count - 3
        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        guard shape.indices.prefix(upTo: channelAxis).allSatisfy({ shape[$0] == 1 }),
              shape[channelAxis] == 3,
              shape[heightAxis] > 0,
              shape[widthAxis] > 0
        else {
            throw DepthAnythingV3EngineError.unsupportedMultiArrayShape(name: inputName, shape: shape)
        }

        let array = try MLMultiArray(
            shape: shape.map { NSNumber(value: $0) },
            dataType: dataType
        )
        let resizedPixelBuffer = try makeResizedBGRA8PixelBuffer(
            from: pixelBuffer,
            width: shape[widthAxis],
            height: shape[heightAxis]
        )
        try fillRGBArray(array, from: resizedPixelBuffer, inputName: inputName)
        return array
    }

    private func makeResizedBGRA8PixelBuffer(
        from pixelBuffer: CVPixelBuffer,
        width: Int,
        height: Int
    ) throws -> CVPixelBuffer {
        let outputPixelBuffer = try makeReusableResizedPixelBuffer(width: width, height: height)
        let inputImage = CIImage(cvPixelBuffer: pixelBuffer)
        let inputExtent = inputImage.extent
        let normalizedImage = inputImage.transformed(
            by: CGAffineTransform(translationX: -inputExtent.origin.x, y: -inputExtent.origin.y)
        )
        let outputBounds = CGRect(x: 0, y: 0, width: width, height: height)
        let scaledImage = normalizedImage.transformed(
            by: CGAffineTransform(
                scaleX: outputBounds.width / max(inputExtent.width, 1),
                y: outputBounds.height / max(inputExtent.height, 1)
            )
        )
        imageContext.render(
            scaledImage,
            to: outputPixelBuffer,
            bounds: outputBounds,
            colorSpace: colorSpace
        )
        return outputPixelBuffer
    }

    private func makeReusableResizedPixelBuffer(width: Int, height: Int) throws -> CVPixelBuffer {
        let requestedSize = VideoDepthInputSize(width: width, height: height)
        if resizedPixelBufferPool == nil || resizedPixelBufferPoolSize != requestedSize {
            let attributes: [CFString: Any] = [
                kCVPixelBufferCGImageCompatibilityKey: true,
                kCVPixelBufferCGBitmapContextCompatibilityKey: true,
                kCVPixelBufferIOSurfacePropertiesKey: [:],
                kCVPixelBufferPixelFormatTypeKey: kCVPixelFormatType_32BGRA,
                kCVPixelBufferWidthKey: requestedSize.width,
                kCVPixelBufferHeightKey: requestedSize.height,
            ]
            var pool: CVPixelBufferPool?
            let poolStatus = CVPixelBufferPoolCreate(kCFAllocatorDefault, nil, attributes as CFDictionary, &pool)
            guard poolStatus == kCVReturnSuccess, let pool else {
                throw DepthAnythingV3EngineError.pixelBufferAllocationFailed(poolStatus)
            }
            resizedPixelBufferPool = pool
            resizedPixelBufferPoolSize = requestedSize
        }

        guard let resizedPixelBufferPool else {
            throw DepthAnythingV3EngineError.pixelBufferAllocationFailed(kCVReturnInvalidArgument)
        }
        var outputPixelBuffer: CVPixelBuffer?
        let status = CVPixelBufferPoolCreatePixelBuffer(
            kCFAllocatorDefault,
            resizedPixelBufferPool,
            &outputPixelBuffer
        )
        guard status == kCVReturnSuccess, let outputPixelBuffer else {
            throw DepthAnythingV3EngineError.pixelBufferAllocationFailed(status)
        }
        return outputPixelBuffer
    }

    private func makeDiagnostics(for loadedModel: LoadedModel) -> DA3ModelDiagnostics {
        let inputSize = inputSize(for: loadedModel.input)
        return DA3ModelDiagnostics(
            modelName: modelURL.lastPathComponent,
            inputName: loadedModel.input.name,
            outputName: loadedModel.outputName,
            inputKind: Self.inputKindDescription(loadedModel.input.kind),
            inputWidth: inputSize?.width ?? 0,
            inputHeight: inputSize?.height ?? 0,
            supportsFlexibleInputShape: false,
            requestedInputWidth: requestedInputSize?.width,
            requestedInputHeight: requestedInputSize?.height,
            computeUnits: Self.computeUnitsDescription(effectiveConfiguration.computeUnits)
        )
    }

    private func inputSize(for input: InputDescriptor) -> VideoDepthInputSize? {
        switch input.kind {
        case let .image(fixedSize):
            return fixedSize ?? requestedInputSize
        case let .multiArray(shape, _):
            guard shape.count >= 3 else {
                return nil
            }
            return VideoDepthInputSize(width: shape[shape.count - 1], height: shape[shape.count - 2])
        }
    }

    private static func inputKindDescription(_ kind: InputKind) -> String {
        switch kind {
        case .image:
            return "image"
        case let .multiArray(_, dataType):
            return "multiArray/\(dataType)"
        }
    }

    private static func computeUnitsDescription(_ computeUnits: MLComputeUnits) -> String {
        switch computeUnits {
        case .all:
            return "all"
        case .cpuOnly:
            return "cpuOnly"
        case .cpuAndGPU:
            return "cpuAndGPU"
        case .cpuAndNeuralEngine:
            return "cpuAndNeuralEngine"
        @unknown default:
            return "\(computeUnits)"
        }
    }

    private static func modelDescription(_ loadedModel: LoadedModel) -> String {
        "input=\(loadedModel.input.name) \(inputKindDescription(loadedModel.input.kind)) output=\(loadedModel.outputName)"
    }

    private static func featureDescriptions(_ descriptions: [String: MLFeatureDescription]) -> String {
        descriptions
            .sorted { $0.key < $1.key }
            .map { name, feature in
                "\(name):\(featureDescription(feature))"
            }
            .joined(separator: "; ")
    }

    private static func featureDescription(_ feature: MLFeatureDescription) -> String {
        switch feature.type {
        case .image:
            if let constraint = feature.imageConstraint {
                return "image \(constraint.pixelsWide)x\(constraint.pixelsHigh)"
            }
            return "image"
        case .multiArray:
            if let constraint = feature.multiArrayConstraint {
                return "multiArray shape=\(constraint.shape.map(\.intValue)) type=\(constraint.dataType)"
            }
            return "multiArray"
        default:
            return "\(feature.type)"
        }
    }

    private static func featureProviderDescription(_ provider: MLFeatureProvider, inputName: String) -> String {
        guard let value = provider.featureValue(for: inputName) else {
            return "\(inputName)=missing"
        }
        if let pixelBuffer = value.imageBufferValue {
            return "\(inputName)=image \(pixelBufferDescription(pixelBuffer))"
        }
        if let multiArray = value.multiArrayValue {
            return "\(inputName)=multiArray shape=\(multiArray.shape.map(\.intValue)) type=\(multiArray.dataType)"
        }
        return "\(inputName)=\(value.type)"
    }

    private static func pixelBufferDescription(_ pixelBuffer: CVPixelBuffer) -> String {
        "\(CVPixelBufferGetWidth(pixelBuffer))x\(CVPixelBufferGetHeight(pixelBuffer)) format=\(pixelFormatDescription(CVPixelBufferGetPixelFormatType(pixelBuffer))) planes=\(CVPixelBufferGetPlaneCount(pixelBuffer))"
    }

    private static func pixelFormatDescription(_ pixelFormat: OSType) -> String {
        let bytes = [
            UInt8((pixelFormat >> 24) & 0xff),
            UInt8((pixelFormat >> 16) & 0xff),
            UInt8((pixelFormat >> 8) & 0xff),
            UInt8(pixelFormat & 0xff),
        ]
        let code = String(bytes: bytes, encoding: .macOSRoman) ?? "\(pixelFormat)"
        return "\(code)(0x\(String(pixelFormat, radix: 16)))"
    }

    private func fillRGBArray(
        _ array: MLMultiArray,
        from pixelBuffer: CVPixelBuffer,
        inputName: String
    ) throws {
        let status = CVPixelBufferLockBaseAddress(pixelBuffer, .readOnly)
        guard status == kCVReturnSuccess else {
            throw DepthAnythingV3EngineError.pixelBufferLockFailed(status)
        }
        defer {
            CVPixelBufferUnlockBaseAddress(pixelBuffer, .readOnly)
        }

        guard let baseAddress = CVPixelBufferGetBaseAddress(pixelBuffer) else {
            throw DepthAnythingV3EngineError.pixelBufferBaseAddressUnavailable
        }

        let shape = array.shape.map(\.intValue)
        let strides = array.strides.map(\.intValue)
        let channelAxis = shape.count - 3
        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        let width = shape[widthAxis]
        let height = shape[heightAxis]
        let bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer)
        let source = baseAddress.assumingMemoryBound(to: UInt8.self)

        switch array.dataType {
        case .float16:
            let destination = array.dataPointer.bindMemory(to: Float16.self, capacity: array.count)
            fillRGBArray(destination, from: source, bytesPerRow: bytesPerRow, width: width, height: height, strides: strides, channelAxis: channelAxis, heightAxis: heightAxis, widthAxis: widthAxis) { Float16($0) }
        case .float32:
            let destination = array.dataPointer.bindMemory(to: Float.self, capacity: array.count)
            fillRGBArray(destination, from: source, bytesPerRow: bytesPerRow, width: width, height: height, strides: strides, channelAxis: channelAxis, heightAxis: heightAxis, widthAxis: widthAxis) { $0 }
        case .double:
            let destination = array.dataPointer.bindMemory(to: Double.self, capacity: array.count)
            fillRGBArray(destination, from: source, bytesPerRow: bytesPerRow, width: width, height: height, strides: strides, channelAxis: channelAxis, heightAxis: heightAxis, widthAxis: widthAxis) { Double($0) }
        default:
            throw DepthAnythingV3EngineError.unsupportedMultiArrayDataType(
                name: inputName,
                dataType: "\(array.dataType)"
            )
        }
    }

    private func fillRGBArray<T>(
        _ destination: UnsafeMutablePointer<T>,
        from source: UnsafePointer<UInt8>,
        bytesPerRow: Int,
        width: Int,
        height: Int,
        strides: [Int],
        channelAxis: Int,
        heightAxis: Int,
        widthAxis: Int,
        convert: (Float) -> T
    ) {
        for y in 0 ..< height {
            let row = source.advanced(by: y * bytesPerRow)
            for x in 0 ..< width {
                let pixel = row.advanced(by: x * 4)
                let red = Float(pixel[2]) / 255
                let green = Float(pixel[1]) / 255
                let blue = Float(pixel[0]) / 255
                let baseOffset = y * strides[heightAxis] + x * strides[widthAxis]
                destination[baseOffset] = convert(red)
                destination[baseOffset + strides[channelAxis]] = convert(green)
                destination[baseOffset + (2 * strides[channelAxis])] = convert(blue)
            }
        }
    }
}
#endif

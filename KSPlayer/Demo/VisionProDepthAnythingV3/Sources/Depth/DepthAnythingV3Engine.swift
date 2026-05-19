#if os(visionOS) && canImport(CoreImage) && canImport(CoreML)
@preconcurrency import CoreImage
@preconcurrency import CoreML
@preconcurrency import CoreVideo
import Foundation
import KSPlayer

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

/// App-target engine for dynamically loading a bundled compiled Depth Anything V3 `.mlmodelc`.
public actor DepthAnythingV3Engine {
    private enum InputKind {
        case image
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
    private let configuration: MLModelConfiguration
    private let preferredInputName: String?
    private let preferredOutputName: String
    private let imageContext = CIContext(options: [.cacheIntermediates: false])
    private let colorSpace = CGColorSpaceCreateDeviceRGB()
    private var loadedModel: LoadedModel?

    public static func defaultModelURL(in bundle: Bundle = .main) -> URL? {
        bundle.url(forResource: "da3-small", withExtension: "mlmodelc")
    }

    public init(
        modelURL: URL? = nil,
        bundle: Bundle = .main,
        configuration: MLModelConfiguration = MLModelConfiguration(),
        inputName: String? = nil,
        outputName: String = "depth"
    ) throws {
        guard let resolvedModelURL = modelURL ?? Self.defaultModelURL(in: bundle) else {
            throw DepthAnythingV3EngineError.modelResourceNotFound("da3-small")
        }
        self.modelURL = resolvedModelURL
        self.configuration = configuration
        self.preferredInputName = inputName
        self.preferredOutputName = outputName
    }

    public func makeDepthFrame(from pixelBuffer: CVPixelBuffer) throws -> DA3DepthFrame {
        try Task.checkCancellation()
        let loadedModel = try loadModelIfNeeded()
        let input = try makeInputProvider(for: pixelBuffer, input: loadedModel.input)
        let output = try loadedModel.model.prediction(from: input)
        try Task.checkCancellation()
        guard let depth = output.featureValue(for: loadedModel.outputName)?.multiArrayValue else {
            throw DepthAnythingV3EngineError.outputIsNotMultiArray(loadedModel.outputName)
        }
        return try DA3DepthFrame(
            multiArray: depth,
            outputName: loadedModel.outputName,
            metadata: [
                "input": loadedModel.input.name,
                "model": modelURL.lastPathComponent,
            ]
        )
    }

    private func loadModelIfNeeded() throws -> LoadedModel {
        if let loadedModel {
            return loadedModel
        }

        let model = try MLModel(contentsOf: modelURL, configuration: configuration)
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
            return InputDescriptor(name: name, kind: .image)
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
        case .image:
            value = MLFeatureValue(pixelBuffer: pixelBuffer)
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
        let attributes: [CFString: Any] = [
            kCVPixelBufferCGImageCompatibilityKey: true,
            kCVPixelBufferCGBitmapContextCompatibilityKey: true,
            kCVPixelBufferIOSurfacePropertiesKey: [:],
        ]
        var outputPixelBuffer: CVPixelBuffer?
        let status = CVPixelBufferCreate(
            kCFAllocatorDefault,
            width,
            height,
            kCVPixelFormatType_32BGRA,
            attributes as CFDictionary,
            &outputPixelBuffer
        )
        guard status == kCVReturnSuccess, let outputPixelBuffer else {
            throw DepthAnythingV3EngineError.pixelBufferAllocationFailed(status)
        }

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

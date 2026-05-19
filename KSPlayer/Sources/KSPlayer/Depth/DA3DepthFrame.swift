import Foundation

#if canImport(CoreML)
@preconcurrency import CoreML
#endif

public enum DA3DepthFrameError: Error, Equatable, LocalizedError, Sendable {
    case invalidShape([Int])
    case invalidStrideLayout(shape: [Int], strides: [Int])
    case invalidElementCount(expected: Int, actual: Int)
    case unsupportedDataType(String)

    public var errorDescription: String? {
        switch self {
        case let .invalidShape(shape):
            return "Depth Anything V3 depth output must be [1, 1, height, width] or [height, width]; got \(shape)."
        case let .invalidStrideLayout(shape, strides):
            return "Depth Anything V3 depth output has incompatible shape \(shape) and strides \(strides)."
        case let .invalidElementCount(expected, actual):
            return "Depth Anything V3 depth output expected \(expected) values but received \(actual)."
        case let .unsupportedDataType(dataType):
            return "Depth Anything V3 depth output uses unsupported MLMultiArray data type \(dataType)."
        }
    }
}

public struct DA3DepthFrame: Equatable, Sendable {
    public let width: Int
    public let height: Int
    public let values: [Float]
    public let metadata: [String: String]

    public init(width: Int, height: Int, values: [Float], metadata: [String: String] = [:]) throws {
        guard width > 0, height > 0 else {
            throw DA3DepthFrameError.invalidShape([height, width])
        }
        let expectedCount = width * height
        guard values.count == expectedCount else {
            throw DA3DepthFrameError.invalidElementCount(expected: expectedCount, actual: values.count)
        }
        self.width = width
        self.height = height
        self.values = values
        self.metadata = metadata
    }

    public subscript(x x: Int, y y: Int) -> Float {
        values[y * width + x]
    }

    public func value(x: Int, y: Int) -> Float? {
        guard x >= 0, x < width, y >= 0, y < height else {
            return nil
        }
        return self[x: x, y: y]
    }

    public func normalizedDisparityMap(
        normalization: VideoDepthNormalizationMode = .minMax,
        invertDepth: Bool = false
    ) -> VideoDepthMap? {
        VideoDepthMap(
            width: width,
            height: height,
            normalizedDisparity: VideoDepthPostprocessor.normalizedValues(
                values,
                normalization: normalization,
                invertDepth: invertDepth
            ),
            metadata: metadata
        )
    }
}

#if canImport(CoreML)
public extension DA3DepthFrame {
    init(multiArray: MLMultiArray, outputName: String = "depth", metadata: [String: String] = [:]) throws {
        let shape = multiArray.shape.map(\.intValue)
        guard shape.count >= 2 else {
            throw DA3DepthFrameError.invalidShape(shape)
        }

        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        let leadingAxes = shape.indices.dropLast(2)
        guard leadingAxes.allSatisfy({ shape[$0] == 1 }),
              shape[heightAxis] > 0,
              shape[widthAxis] > 0
        else {
            throw DA3DepthFrameError.invalidShape(shape)
        }

        let height = shape[heightAxis]
        let width = shape[widthAxis]
        let values = try Self.depthValues(from: multiArray, shape: shape, width: width, height: height)
        var frameMetadata = metadata
        frameMetadata["output"] = outputName
        frameMetadata["shape"] = shape.map(String.init).joined(separator: "x")
        try self.init(width: width, height: height, values: values, metadata: frameMetadata)
    }

    private static func depthValues(from multiArray: MLMultiArray, shape: [Int], width: Int, height: Int) throws -> [Float] {
        let strides = multiArray.strides.map(\.intValue)
        guard strides.count == shape.count else {
            throw DA3DepthFrameError.invalidStrideLayout(shape: shape, strides: strides)
        }

        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        let offsets = (0 ..< height * width).map { linearIndex in
            (linearIndex / width) * strides[heightAxis] + (linearIndex % width) * strides[widthAxis]
        }

        if offsets.allSatisfy({ $0 >= 0 && $0 < multiArray.count }) {
            return try contiguousValues(from: multiArray, offsets: offsets)
        }

        return indexedValues(from: multiArray, shape: shape, width: width, height: height)
    }

    private static func contiguousValues(from multiArray: MLMultiArray, offsets: [Int]) throws -> [Float] {
        switch multiArray.dataType {
        case .float32:
            let pointer = multiArray.dataPointer.bindMemory(to: Float.self, capacity: multiArray.count)
            return offsets.map { pointer[$0] }
        case .float16:
            let pointer = multiArray.dataPointer.bindMemory(to: Float16.self, capacity: multiArray.count)
            return offsets.map { Float(pointer[$0]) }
        case .double:
            let pointer = multiArray.dataPointer.bindMemory(to: Double.self, capacity: multiArray.count)
            return offsets.map { Float(pointer[$0]) }
        default:
            return indexedValues(
                from: multiArray,
                shape: multiArray.shape.map(\.intValue),
                width: multiArray.shape.last?.intValue ?? 0,
                height: multiArray.shape.dropLast().last?.intValue ?? 0
            )
        }
    }

    private static func indexedValues(from multiArray: MLMultiArray, shape: [Int], width: Int, height: Int) -> [Float] {
        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        return (0 ..< height * width).map { linearIndex in
            var index = Array(repeating: 0, count: shape.count)
            index[heightAxis] = linearIndex / width
            index[widthAxis] = linearIndex % width
            return multiArray[index.map { NSNumber(value: $0) }].floatValue
        }
    }
}
#endif

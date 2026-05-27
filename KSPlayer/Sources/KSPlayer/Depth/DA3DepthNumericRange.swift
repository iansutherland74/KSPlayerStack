#if canImport(CoreML)
@preconcurrency import CoreML
import Foundation

public enum DA3DepthNumericRange {
    public struct Summary: Equatable, Sendable {
        public let minimum: Float?
        public let maximum: Float?

        public init(minimum: Float?, maximum: Float?) {
            self.minimum = minimum
            self.maximum = maximum
        }
    }

    public static func dimensions(for shape: [Int]) -> (width: Int, height: Int)? {
        guard shape.count >= 2 else {
            return nil
        }
        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        guard shape.indices.dropLast(2).allSatisfy({ shape[$0] == 1 }) else {
            return nil
        }
        let height = shape[heightAxis]
        let width = shape[widthAxis]
        guard width > 0, height > 0 else {
            return nil
        }
        return (width, height)
    }

    public static func minMax(multiArray: MLMultiArray) -> Summary {
        let shape = multiArray.shape.map(\.intValue)
        guard let dimensions = dimensions(for: shape) else {
            return Summary(minimum: nil, maximum: nil)
        }
        return minMax(
            multiArray: multiArray,
            shape: shape,
            width: dimensions.width,
            height: dimensions.height
        )
    }

    public static func minMax(
        multiArray: MLMultiArray,
        shape: [Int],
        width: Int,
        height: Int
    ) -> Summary {
        let strides = multiArray.strides.map(\.intValue)
        guard strides.count == shape.count else {
            return Summary(minimum: nil, maximum: nil)
        }

        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        let offsets = (0 ..< height * width).map { linearIndex in
            (linearIndex / width) * strides[heightAxis] + (linearIndex % width) * strides[widthAxis]
        }

        guard offsets.allSatisfy({ $0 >= 0 && $0 < multiArray.count }) else {
            return indexedMinMax(multiArray: multiArray, shape: shape, width: width, height: height)
        }

        switch multiArray.dataType {
        case .float32:
            let pointer = multiArray.dataPointer.bindMemory(to: Float.self, capacity: multiArray.count)
            return minMax(values: offsets.map { pointer[$0] })
        case .float16:
            let pointer = multiArray.dataPointer.bindMemory(to: Float16.self, capacity: multiArray.count)
            return minMax(values: offsets.map { Float(pointer[$0]) })
        case .double:
            let pointer = multiArray.dataPointer.bindMemory(to: Double.self, capacity: multiArray.count)
            return minMax(values: offsets.map { Float(pointer[$0]) })
        default:
            return indexedMinMax(multiArray: multiArray, shape: shape, width: width, height: height)
        }
    }

    private static func indexedMinMax(
        multiArray: MLMultiArray,
        shape: [Int],
        width: Int,
        height: Int
    ) -> Summary {
        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        var values: [Float] = []
        values.reserveCapacity(width * height)
        for linearIndex in 0 ..< width * height {
            var index = Array(repeating: 0, count: shape.count)
            index[heightAxis] = linearIndex / width
            index[widthAxis] = linearIndex % width
            values.append(multiArray[index.map { NSNumber(value: $0) }].floatValue)
        }
        return minMax(values: values)
    }

    public static func minMax(values: [Float]) -> Summary {
        var minimum: Float?
        var maximum: Float?
        for value in values where value.isFinite {
            minimum = minimum.map { min($0, value) } ?? value
            maximum = maximum.map { max($0, value) } ?? value
        }
        return Summary(minimum: minimum, maximum: maximum)
    }
}
#endif

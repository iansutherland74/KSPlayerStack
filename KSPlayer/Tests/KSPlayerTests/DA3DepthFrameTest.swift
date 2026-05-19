@testable import KSPlayer
#if canImport(CoreML)
@preconcurrency import CoreML
#endif
#if canImport(Metal)
@preconcurrency import Metal
#endif
import XCTest

final class DA3DepthFrameTest: XCTestCase {
    func testDepthFrameIndexesRowMajorValues() throws {
        let frame = try DA3DepthFrame(width: 3, height: 2, values: [0, 1, 2, 3, 4, 5])

        XCTAssertEqual(frame[x: 2, y: 1], 5)
        XCTAssertEqual(frame.value(x: 1, y: 0), 1)
        XCTAssertNil(frame.value(x: 3, y: 0))
        XCTAssertNil(frame.value(x: 0, y: -1))
    }

    func testDepthFrameRejectsInvalidElementCount() {
        XCTAssertThrowsError(try DA3DepthFrame(width: 2, height: 2, values: [0, 1, 2])) { error in
            XCTAssertEqual(error as? DA3DepthFrameError, .invalidElementCount(expected: 4, actual: 3))
        }
    }

    func testDepthFrameCanNormalizeWhenRendererRequiresDisparity() throws {
        let frame = try DA3DepthFrame(width: 3, height: 1, values: [10, 20, 30])

        XCTAssertEqual(frame.normalizedDisparityMap()?.normalizedDisparity, [0, 0.5, 1])
    }

    #if canImport(CoreML)
    func testMultiArrayExtractionUsesDepthAnythingV3Shape() throws {
        let array = try MLMultiArray(shape: [1, 1, 2, 3], dataType: .float32)
        for index in 0 ..< 6 {
            array[[0, 0, index / 3, index % 3] as [NSNumber]] = NSNumber(value: Float(index) + 0.25)
        }

        let frame = try DA3DepthFrame(multiArray: array)

        XCTAssertEqual(frame.width, 3)
        XCTAssertEqual(frame.height, 2)
        XCTAssertEqual(frame.values, [0.25, 1.25, 2.25, 3.25, 4.25, 5.25])
        XCTAssertEqual(frame.metadata["shape"], "1x1x2x3")
        XCTAssertEqual(frame.metadata["output"], "depth")
    }

    func testMultiArrayExtractionSupportsDouble() throws {
        let array = try MLMultiArray(shape: [1, 1, 1, 2], dataType: .double)
        array[[0, 0, 0, 0] as [NSNumber]] = NSNumber(value: 1.5)
        array[[0, 0, 0, 1] as [NSNumber]] = NSNumber(value: 2.25)

        let frame = try DA3DepthFrame(multiArray: array)

        XCTAssertEqual(frame.values, [1.5, 2.25])
    }

    func testMultiArrayExtractionSupportsNonContiguousStrides() throws {
        let rawPointer = UnsafeMutableRawPointer.allocate(
            byteCount: 12 * MemoryLayout<Float>.stride,
            alignment: MemoryLayout<Float>.alignment
        )
        let typedPointer = rawPointer.bindMemory(to: Float.self, capacity: 12)
        for index in 0 ..< 12 {
            typedPointer[index] = Float(index)
        }

        let array = try MLMultiArray(
            dataPointer: rawPointer,
            shape: [1, 1, 2, 3],
            dataType: .float32,
            strides: [12, 12, 6, 2],
            deallocator: { pointer in
                pointer.deallocate()
            }
        )

        let frame = try DA3DepthFrame(multiArray: array)

        XCTAssertEqual(frame.values, [0, 2, 4, 6, 8, 10])
    }

    func testMultiArrayExtractionRejectsMultipleChannels() throws {
        let array = try MLMultiArray(shape: [1, 2, 2, 2], dataType: .float32)

        XCTAssertThrowsError(try DA3DepthFrame(multiArray: array)) { error in
            XCTAssertEqual(error as? DA3DepthFrameError, .invalidShape([1, 2, 2, 2]))
        }
    }
    #endif

    #if canImport(Metal)
    func testDepthMetalBridgeCreatesR32FloatTexture() throws {
        guard let device = MTLCreateSystemDefaultDevice() else {
            throw XCTSkip("Metal is unavailable in this test environment.")
        }
        let frame = try DA3DepthFrame(width: 2, height: 2, values: [0.1, 0.2, 0.3, 0.4])
        let bridge = DA3DepthMetalBridge(device: device)

        let texture = try bridge.makeTexture(from: frame)

        XCTAssertEqual(texture.pixelFormat, .r32Float)
        XCTAssertEqual(texture.width, 2)
        XCTAssertEqual(texture.height, 2)
        XCTAssertEqual(DA3DepthMetalBridge.bytesPerRow(forWidth: 2), 8)

        var copiedValues = [Float](repeating: 0, count: 4)
        copiedValues.withUnsafeMutableBytes { rawBuffer in
            texture.getBytes(
                rawBuffer.baseAddress!,
                bytesPerRow: DA3DepthMetalBridge.bytesPerRow(forWidth: 2),
                from: MTLRegionMake2D(0, 0, 2, 2),
                mipmapLevel: 0
            )
        }
        XCTAssertEqual(copiedValues[0], 0.1, accuracy: 0.0001)
        XCTAssertEqual(copiedValues[1], 0.2, accuracy: 0.0001)
        XCTAssertEqual(copiedValues[2], 0.3, accuracy: 0.0001)
        XCTAssertEqual(copiedValues[3], 0.4, accuracy: 0.0001)
    }
    #endif
}

@testable import KSPlayer
import CoreVideo
import Foundation
import XCTest

final class KSVideoFrameOutputTest: XCTestCase {
    func testKeepLatestDropsOlderPendingFrameWhenCallbackIsBusy() throws {
        let firstStarted = expectation(description: "first callback started")
        let delivered = expectation(description: "callbacks delivered")
        delivered.expectedFulfillmentCount = 2
        let releaseFirst = DispatchSemaphore(value: 0)
        let received = LockedArray<Int>()
        let output = KSVideoFrameOutput(configuration: .init(maximumBufferedFrameCount: 1, dropPolicy: .keepLatest)) { pixelBuffer in
            let width = CVPixelBufferGetWidth(pixelBuffer)
            received.append(width)
            if width == 1 {
                firstStarted.fulfill()
                releaseFirst.wait()
            }
            delivered.fulfill()
        }

        output.enqueue(try makePixelBuffer(width: 1))
        wait(for: [firstStarted], timeout: 1)
        output.enqueue(try makePixelBuffer(width: 2))
        output.enqueue(try makePixelBuffer(width: 3))
        releaseFirst.signal()
        wait(for: [delivered], timeout: 2)

        XCTAssertEqual(received.values, [1, 3])
        XCTAssertEqual(output.droppedFrameCount, 1)
    }

    func testDropNewestPreservesQueuedFrameWhenCallbackIsBusy() throws {
        let firstStarted = expectation(description: "first callback started")
        let delivered = expectation(description: "callbacks delivered")
        delivered.expectedFulfillmentCount = 2
        let releaseFirst = DispatchSemaphore(value: 0)
        let received = LockedArray<Int>()
        let output = KSVideoFrameOutput(configuration: .init(maximumBufferedFrameCount: 1, dropPolicy: .dropNewest)) { pixelBuffer in
            let width = CVPixelBufferGetWidth(pixelBuffer)
            received.append(width)
            if width == 1 {
                firstStarted.fulfill()
                releaseFirst.wait()
            }
            delivered.fulfill()
        }

        output.enqueue(try makePixelBuffer(width: 1))
        wait(for: [firstStarted], timeout: 1)
        output.enqueue(try makePixelBuffer(width: 2))
        output.enqueue(try makePixelBuffer(width: 3))
        releaseFirst.signal()
        wait(for: [delivered], timeout: 2)

        XCTAssertEqual(received.values, [1, 2])
        XCTAssertEqual(output.droppedFrameCount, 1)
    }

    func testCallbackRunsOffMainThreadAndEscapedPixelBufferStaysValid() throws {
        let delivered = expectation(description: "callback delivered")
        let retained = LockedPixelBuffer()
        var output: KSVideoFrameOutput? = KSVideoFrameOutput { pixelBuffer in
            XCTAssertFalse(Thread.isMainThread)
            retained.pixelBuffer = pixelBuffer
            delivered.fulfill()
        }

        output?.enqueue(try makePixelBuffer(width: 7))
        wait(for: [delivered], timeout: 1)
        output = nil

        let pixelBuffer = try XCTUnwrap(retained.pixelBuffer)
        XCTAssertEqual(CVPixelBufferGetWidth(pixelBuffer), 7)
    }

    private func makePixelBuffer(width: Int, height: Int = 2) throws -> CVPixelBuffer {
        var pixelBuffer: CVPixelBuffer?
        let status = CVPixelBufferCreate(
            kCFAllocatorDefault,
            width,
            height,
            kCVPixelFormatType_32BGRA,
            nil,
            &pixelBuffer
        )
        XCTAssertEqual(status, kCVReturnSuccess)
        return try XCTUnwrap(pixelBuffer)
    }
}

private final class LockedArray<Element>: @unchecked Sendable {
    private let lock = NSLock()
    private var storage = [Element]()

    var values: [Element] {
        lock.lock()
        let value = storage
        lock.unlock()
        return value
    }

    func append(_ value: Element) {
        lock.lock()
        storage.append(value)
        lock.unlock()
    }
}

private final class LockedPixelBuffer: @unchecked Sendable {
    private let lock = NSLock()
    private var storage: CVPixelBuffer?

    var pixelBuffer: CVPixelBuffer? {
        get {
            lock.lock()
            let value = storage
            lock.unlock()
            return value
        }
        set {
            lock.lock()
            storage = newValue
            lock.unlock()
        }
    }
}

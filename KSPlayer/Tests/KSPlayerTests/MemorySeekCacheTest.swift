@testable import KSPlayer
import AVFoundation
import XCTest

final class MemorySeekCacheTest: XCTestCase {
    func testPacketsForSeekReturnsRangeFromVideoKeyFrame() {
        let cache = MemorySeekCache<TestMemorySeekItem>(maxDuration: 10, maxByteSize: 10_000)
        cache.store(TestMemorySeekItem(trackID: 1, mediaType: .video, seconds: 10, size: 100, isKeyFrame: true))
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 10.1, size: 50))
        cache.store(TestMemorySeekItem(trackID: 1, mediaType: .video, seconds: 11, size: 90))
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 11.1, size: 50))

        let packets = cache.packets(for: 11, requiredTrackIDs: [1, 2])

        XCTAssertEqual(packets?.map(\.memorySeekCacheTrackID), [1, 2, 1, 2])
        XCTAssertEqual(packets?.first?.seconds, 10)
    }

    func testPacketsForSeekMissesWithoutVideoKeyFrame() {
        let cache = MemorySeekCache<TestMemorySeekItem>(maxDuration: 10, maxByteSize: 10_000)
        cache.store(TestMemorySeekItem(trackID: 1, mediaType: .video, seconds: 10, size: 100))
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 10.1, size: 50))

        XCTAssertNil(cache.packets(for: 10.5, requiredTrackIDs: [1, 2]))
    }

    func testByteCapTrimsOldPackets() {
        let cache = MemorySeekCache<TestMemorySeekItem>(maxDuration: 10, maxByteSize: 120)
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 1, size: 50))
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 2, size: 50))
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 3, size: 50))

        XCTAssertEqual(cache.totalByteSize, 100)
        XCTAssertNil(cache.packets(for: 1, requiredTrackIDs: [2]))
        XCTAssertEqual(cache.packets(for: 2, requiredTrackIDs: [2])?.map(\.seconds), [2, 3])
    }

    func testDurationCapTrimsOldPackets() {
        let cache = MemorySeekCache<TestMemorySeekItem>(maxDuration: 2, maxByteSize: 10_000)
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 1, size: 50))
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 2, size: 50))
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 4, size: 50))

        XCTAssertNil(cache.packets(for: 1, requiredTrackIDs: [2]))
        XCTAssertEqual(cache.packets(for: 2, requiredTrackIDs: [2])?.map(\.seconds), [2, 4])
    }

    func testInvalidateClearsCache() {
        let cache = MemorySeekCache<TestMemorySeekItem>(maxDuration: 10, maxByteSize: 10_000)
        cache.store(TestMemorySeekItem(trackID: 2, mediaType: .audio, seconds: 1, size: 50))

        cache.invalidate()

        XCTAssertTrue(cache.isEmpty)
        XCTAssertEqual(cache.totalByteSize, 0)
        XCTAssertNil(cache.packets(for: 1, requiredTrackIDs: [2]))
    }
}

private final class TestMemorySeekItem: MemorySeekCacheItem {
    var duration: Int64 = 0
    var timestamp: Int64
    var position: Int64 = 0
    var size: Int32
    let timebase = Timebase(num: 1, den: 1000)
    let memorySeekCacheTrackID: Int32
    let memorySeekCacheMediaType: AVMediaType
    let memorySeekCacheIsKeyFrame: Bool

    init(trackID: Int32, mediaType: AVMediaType, seconds: TimeInterval, size: Int32, isKeyFrame: Bool = false) {
        memorySeekCacheTrackID = trackID
        memorySeekCacheMediaType = mediaType
        memorySeekCacheIsKeyFrame = isKeyFrame
        timestamp = Int64(seconds * 1000)
        self.size = size
    }

    func makeMemorySeekCacheCopy() -> (any MemorySeekCacheItem)? {
        TestMemorySeekItem(trackID: memorySeekCacheTrackID, mediaType: memorySeekCacheMediaType, seconds: seconds, size: size, isKeyFrame: memorySeekCacheIsKeyFrame)
    }
}

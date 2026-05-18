@testable import KSPlayer
import XCTest

final class DiskPrecacheTest: XCTestCase {
    private var temporaryURLs = [URL]()

    override func tearDownWithError() throws {
        for url in temporaryURLs {
            try? FileManager.default.removeItem(at: url)
        }
        temporaryURLs.removeAll()
        try super.tearDownWithError()
    }

    func testDiskPrecacheDefaultsToDisabled() {
        let options = KSOptions()

        XCTAssertFalse(options.isDiskPrecacheEnabled)
        XCTAssertGreaterThan(options.diskPrecacheMaxFileSize, 0)
        XCTAssertGreaterThan(options.diskPrecacheMaxCacheSize, 0)
    }

    func testCacheKeyIsDeterministicAndIncludesQuery() throws {
        let url = try XCTUnwrap(URL(string: "https://example.com/movie.mp4?token=one"))
        let sameURL = try XCTUnwrap(URL(string: "https://example.com/movie.mp4?token=one"))
        let otherURL = try XCTUnwrap(URL(string: "https://example.com/movie.mp4?token=two"))

        XCTAssertEqual(KSDiskPrecache.cacheKey(for: url), KSDiskPrecache.cacheKey(for: sameURL))
        XCTAssertNotEqual(KSDiskPrecache.cacheKey(for: url), KSDiskPrecache.cacheKey(for: otherURL))
    }

    func testSupportedRemoteMediaURLExcludesLiveManifestAndCustomSchemes() throws {
        XCTAssertTrue(KSDiskPrecache.isSupportedRemoteMediaURL(try XCTUnwrap(URL(string: "https://example.com/movie.mp4"))))
        XCTAssertTrue(KSDiskPrecache.isSupportedRemoteMediaURL(try XCTUnwrap(URL(string: "http://example.com/movie"))))
        XCTAssertFalse(KSDiskPrecache.isSupportedRemoteMediaURL(try XCTUnwrap(URL(string: "https://example.com/live.m3u8"))))
        XCTAssertFalse(KSDiskPrecache.isSupportedRemoteMediaURL(try XCTUnwrap(URL(string: "rtsp://example.com/movie.mp4"))))
        XCTAssertFalse(KSDiskPrecache.isSupportedRemoteMediaURL(URL(fileURLWithPath: "/tmp/movie.mp4")))
    }

    func testPlaybackURLUsesCompletedCacheFileWhenEnabled() throws {
        let options = KSOptions()
        options.isDiskPrecacheEnabled = true
        options.diskPrecacheDirectoryURL = temporaryDirectory()
        let remoteURL = try XCTUnwrap(URL(string: "https://example.com/video.mp4"))
        let cachedURL = KSDiskPrecache.cacheFileURL(for: remoteURL, directory: KSDiskPrecache.cacheDirectoryURL(options: options))
        try FileManager.default.createDirectory(at: cachedURL.deletingLastPathComponent(), withIntermediateDirectories: true)
        try Data([1, 2, 3]).write(to: cachedURL)

        XCTAssertEqual(KSDiskPrecache.playbackURL(for: remoteURL, options: options), cachedURL)
    }

    func testPlaybackURLReturnsOriginalWhenDisabled() throws {
        let options = KSOptions()
        options.isDiskPrecacheEnabled = false
        let remoteURL = try XCTUnwrap(URL(string: "https://example.com/video.mp4"))

        XCTAssertEqual(KSDiskPrecache.playbackURL(for: remoteURL, options: options), remoteURL)
    }

    private func temporaryDirectory() -> URL {
        let url = FileManager.default.temporaryDirectory
            .appendingPathComponent("KSPlayerDiskPrecacheTests-\(UUID().uuidString)", isDirectory: true)
        temporaryURLs.append(url)
        return url
    }
}

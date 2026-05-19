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
        XCTAssertFalse(KSDiskPrecache.isSupportedRemoteMediaURL(try XCTUnwrap(URL(string: "smb2://server/share/movie.mp4"))))
        XCTAssertFalse(KSDiskPrecache.isSupportedRemoteMediaURL(try XCTUnwrap(URL(string: "dlna://device/item/movie.mp4"))))
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

    func testDownloadRequestAppliesHeadersUserAgentAndReferer() throws {
        let options = KSOptions()
        options.userAgent = "KSPlayerTest"
        options.referer = "https://example.com/page"
        options.appendHeader(["Authorization": "Bearer token"])
        let remoteURL = try XCTUnwrap(URL(string: "https://example.com/video.mp4"))

        let request = KSDiskPrecache.downloadRequest(for: remoteURL, options: options)

        XCTAssertEqual(request.value(forHTTPHeaderField: "Authorization"), "Bearer token")
        XCTAssertEqual(request.value(forHTTPHeaderField: "User-Agent"), "KSPlayerTest")
        XCTAssertEqual(request.value(forHTTPHeaderField: "Referer"), "https://example.com/page")
    }

    func testExplicitHeaderOverridesOptionDefaultHeader() throws {
        let options = KSOptions()
        options.userAgent = "DefaultAgent"
        options.referer = "https://default.example.com"
        options.appendHeader([
            "User-Agent": "ExplicitAgent",
            "Referer": "https://explicit.example.com",
        ])
        let remoteURL = try XCTUnwrap(URL(string: "https://example.com/video.mp4"))

        let request = KSDiskPrecache.downloadRequest(for: remoteURL, options: options)

        XCTAssertEqual(request.value(forHTTPHeaderField: "User-Agent"), "ExplicitAgent")
        XCTAssertEqual(request.value(forHTTPHeaderField: "Referer"), "https://explicit.example.com")
    }

    func testCacheSizeAndClearCacheIgnoreTemporaryDownloads() throws {
        let options = KSOptions()
        options.diskPrecacheDirectoryURL = temporaryDirectory()
        let directory = KSDiskPrecache.cacheDirectoryURL(options: options)
        let cachedFile = directory.appendingPathComponent("cached.mp4")
        let temporaryFile = directory.appendingPathComponent(".cached.mp4.partial.download")
        try writeFile(at: cachedFile, bytes: 4)
        try writeFile(at: temporaryFile, bytes: 8)

        XCTAssertEqual(KSDiskPrecache.cacheSize(options: options), 4)

        KSDiskPrecache.clearCache(options: options)

        XCTAssertFalse(FileManager.default.fileExists(atPath: directory.path))
        XCTAssertEqual(KSDiskPrecache.cacheSize(options: options), 0)
    }

    func testTrimCacheRemovesLeastRecentlyTouchedCachedFiles() throws {
        let directory = temporaryDirectory()
        let olderFile = directory.appendingPathComponent("older.mp4")
        let newerFile = directory.appendingPathComponent("newer.mp4")
        let temporaryFile = directory.appendingPathComponent(".partial.download")
        try writeFile(at: olderFile, bytes: 4)
        try writeFile(at: newerFile, bytes: 4)
        try writeFile(at: temporaryFile, bytes: 4)
        try FileManager.default.setAttributes([.modificationDate: Date(timeIntervalSince1970: 100)], ofItemAtPath: olderFile.path)
        try FileManager.default.setAttributes([.modificationDate: Date(timeIntervalSince1970: 200)], ofItemAtPath: newerFile.path)

        KSDiskPrecache.trimCache(directory: directory, maxSize: 4)

        XCTAssertFalse(FileManager.default.fileExists(atPath: olderFile.path))
        XCTAssertTrue(FileManager.default.fileExists(atPath: newerFile.path))
        XCTAssertFalse(FileManager.default.fileExists(atPath: temporaryFile.path))
    }

    private func temporaryDirectory() -> URL {
        let url = FileManager.default.temporaryDirectory
            .appendingPathComponent("KSPlayerDiskPrecacheTests-\(UUID().uuidString)", isDirectory: true)
        temporaryURLs.append(url)
        return url
    }

    private func writeFile(at url: URL, bytes: Int) throws {
        try FileManager.default.createDirectory(at: url.deletingLastPathComponent(), withIntermediateDirectories: true)
        try Data(repeating: 1, count: bytes).write(to: url)
    }
}

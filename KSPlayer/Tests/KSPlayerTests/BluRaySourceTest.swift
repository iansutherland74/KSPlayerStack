@testable import KSPlayer
import XCTest

final class BluRaySourceTest: XCTestCase {
    private var temporaryURLs = [URL]()

    override func tearDownWithError() throws {
        for url in temporaryURLs {
            try? FileManager.default.removeItem(at: url)
        }
        temporaryURLs.removeAll()
        try super.tearDownWithError()
    }

    func testISOResolvesToBlurayProtocolURL() throws {
        let isoURL = temporaryDirectory().appendingPathComponent("Movie.iso")
        FileManager.default.createFile(atPath: isoURL.path, contents: Data())

        let source = try XCTUnwrap(KSBluRayURLResolver.source(for: isoURL))

        XCTAssertEqual(source.kind, .isoImage)
        XCTAssertEqual(source.url, isoURL)
        XCTAssertEqual(source.ffmpegURLString, "bluray:\(isoURL.standardizedFileURL.path)")
    }

    func testMissingISOFileDoesNotResolve() throws {
        let isoURL = temporaryDirectory().appendingPathComponent("Missing.iso")

        XCTAssertNil(KSBluRayURLResolver.source(for: isoURL))
        XCTAssertFalse(KSBluRayURLResolver.isBluRayCandidate(isoURL))
    }

    func testDirectoryNamedISODoesNotResolveAsImage() throws {
        let isoURL = temporaryDirectory().appendingPathComponent("Movie.iso", isDirectory: true)
        try FileManager.default.createDirectory(at: isoURL, withIntermediateDirectories: true)

        XCTAssertNil(KSBluRayURLResolver.source(for: isoURL))
    }

    func testDiscRootWithBDMVFolderResolves() throws {
        let rootURL = temporaryDirectory()
        let bdmvURL = rootURL.appendingPathComponent("BDMV", isDirectory: true)
        try FileManager.default.createDirectory(at: bdmvURL, withIntermediateDirectories: true)
        FileManager.default.createFile(atPath: bdmvURL.appendingPathComponent("index.bdmv").path, contents: Data())

        let source = try XCTUnwrap(KSBluRayURLResolver.source(for: rootURL))

        XCTAssertEqual(source.kind, .bdmvDirectory)
        XCTAssertEqual(source.url, rootURL)
    }

    func testBDMVFolderResolvesToDiscRoot() throws {
        let rootURL = temporaryDirectory()
        let bdmvURL = rootURL.appendingPathComponent("BDMV", isDirectory: true)
        try FileManager.default.createDirectory(at: bdmvURL, withIntermediateDirectories: true)
        FileManager.default.createFile(atPath: bdmvURL.appendingPathComponent("MovieObject.bdmv").path, contents: Data())

        let source = try XCTUnwrap(KSBluRayURLResolver.source(for: bdmvURL))

        XCTAssertEqual(source.kind, .bdmvDirectory)
        XCTAssertEqual(source.url, rootURL)
    }

    func testFileInsideBDMVResolvesToDiscRoot() throws {
        let rootURL = temporaryDirectory()
        let streamURL = rootURL.appendingPathComponent("BDMV/STREAM", isDirectory: true)
        try FileManager.default.createDirectory(at: streamURL, withIntermediateDirectories: true)
        FileManager.default.createFile(atPath: rootURL.appendingPathComponent("BDMV/index.bdmv").path, contents: Data())
        let clipURL = streamURL.appendingPathComponent("00000.m2ts")
        FileManager.default.createFile(atPath: clipURL.path, contents: Data())

        let source = try XCTUnwrap(KSBluRayURLResolver.source(for: clipURL))

        XCTAssertEqual(source.kind, .bdmvDirectory)
        XCTAssertEqual(source.url, rootURL)
    }

    func testCaseInsensitiveBDMVFolderResolvesToDiscRoot() throws {
        let rootURL = temporaryDirectory()
        let bdmvURL = rootURL.appendingPathComponent("bdmv", isDirectory: true)
        try FileManager.default.createDirectory(at: bdmvURL, withIntermediateDirectories: true)
        FileManager.default.createFile(atPath: bdmvURL.appendingPathComponent("INDEX.BDMV").path, contents: Data())

        let source = try XCTUnwrap(KSBluRayURLResolver.source(for: rootURL))

        XCTAssertEqual(source.kind, .bdmvDirectory)
        XCTAssertEqual(source.url, rootURL)
    }

    func testRemoteISOIsCandidateButNotLocalSource() throws {
        let isoURL = try XCTUnwrap(URL(string: "https://example.com/Movie.iso"))

        XCTAssertNil(KSBluRayURLResolver.source(for: isoURL))
        XCTAssertTrue(KSBluRayURLResolver.isBluRayCandidate(isoURL))
    }

    func testBluRayProtocolWhitelistIsExtendedWhenConstrained() throws {
        let options = KSOptions()
        options.formatContextOptions["protocol_whitelist"] = "file,http,https,tcp"
        let isoURL = temporaryDirectory().appendingPathComponent("Movie.iso")
        FileManager.default.createFile(atPath: isoURL.path, contents: Data())
        let source = try XCTUnwrap(KSBluRayURLResolver.source(for: isoURL))

        options.prepareFormatContextOptions(for: source)

        XCTAssertEqual(options.formatContextOptions["protocol_whitelist"] as? String, "file,http,https,tcp,bluray")
    }

    private func temporaryDirectory() -> URL {
        let url = FileManager.default.temporaryDirectory
            .appendingPathComponent("KSPlayerTests-\(UUID().uuidString)", isDirectory: true)
        try? FileManager.default.createDirectory(at: url, withIntermediateDirectories: true)
        temporaryURLs.append(url)
        return url
    }
}

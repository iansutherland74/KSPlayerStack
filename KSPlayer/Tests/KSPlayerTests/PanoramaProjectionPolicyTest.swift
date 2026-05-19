@testable import KSPlayer
import AVFoundation
import XCTest

final class PanoramaProjectionPolicyTest: XCTestCase {
    func testMetadataDetectsEquirectangularProjection() {
        let metadata = ["projection": "equirectangular"]

        XCTAssertEqual(PanoramaProjectionPolicy.detectedProjection(metadata: metadata), .equirectangular)
    }

    func testMetadataDetectsLegacySphericalFlag() {
        let metadata = ["spherical_video": "true"]

        XCTAssertEqual(PanoramaProjectionPolicy.detectedProjection(metadata: metadata), .equirectangular)
    }

    func testMetadataIgnoresFlatProjection() {
        let metadata = ["projection": "none"]

        XCTAssertNil(PanoramaProjectionPolicy.detectedProjection(metadata: metadata))
    }

    func testMetadataDetectsUnsupportedCubemap() {
        let metadata = ["projection": "cubemap"]

        XCTAssertEqual(PanoramaProjectionPolicy.detectedProjection(metadata: metadata), .cubemap)
        XCTAssertFalse(VideoProjection.cubemap.isRenderableInSphere)
    }

    func testFormatDescriptionDetectsProjectionKind() throws {
        var formatDescription: CMFormatDescription?
        let extensions = ["ProjectionKind": "Equirectangular"] as CFDictionary
        let status = CMVideoFormatDescriptionCreate(
            allocator: kCFAllocatorDefault,
            codecType: kCMVideoCodecType_H264,
            width: 3840,
            height: 1920,
            extensions: extensions,
            formatDescriptionOut: &formatDescription
        )

        XCTAssertEqual(status, noErr)
        XCTAssertEqual(
            PanoramaProjectionPolicy.detectedProjection(formatDescription: try XCTUnwrap(formatDescription)),
            .equirectangular
        )
    }

    func testAutomaticModeOnlyResolvesRenderableProjection() {
        XCTAssertEqual(
            PanoramaProjectionPolicy.resolvedProjection(mode: .automatic, detectedProjection: .equirectangular),
            .equirectangular
        )
        XCTAssertNil(PanoramaProjectionPolicy.resolvedProjection(mode: .automatic, detectedProjection: .cubemap))
    }

    func testDisabledModePreservesFlatRendering() {
        XCTAssertNil(PanoramaProjectionPolicy.resolvedProjection(mode: .disabled, detectedProjection: .equirectangular))
        XCTAssertEqual(KSOptions().panoramaMode, .disabled)
    }

    func testForcedModeDoesNotRequireMetadata() {
        XCTAssertEqual(PanoramaProjectionPolicy.resolvedProjection(mode: .equirectangular, detectedProjection: nil), .equirectangular)
    }

    @MainActor
    func testPreferredPlayerTypeRoutesPanoramaModeToMEPlayer() throws {
        let originalFirstPlayerType = KSOptions.firstPlayerType
        defer {
            KSOptions.firstPlayerType = originalFirstPlayerType
        }
        KSOptions.firstPlayerType = KSAVPlayer.self

        let options = KSOptions()
        options.panoramaMode = .automatic

        let url = try XCTUnwrap(URL(string: "https://example.com/movie.mp4"))

        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: url, options: options) == KSMEPlayer.self)
    }

    @MainActor
    func testPreferredPlayerTypeKeepsSeparateAudioVideoOnAVPlayerForPanorama() throws {
        let options = KSOptions()
        options.panoramaMode = .equirectangular
        let videoURL = try XCTUnwrap(URL(string: "https://example.com/movie.mp4"))
        let audioURL = try XCTUnwrap(URL(string: "https://example.com/audio.m4a"))

        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: videoURL, audioURL: audioURL, options: options) == KSAVPlayer.self)
    }
}

@testable import KSPlayer
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

    func testMetadataDetectsUnsupportedCubemap() {
        let metadata = ["projection": "cubemap"]

        XCTAssertEqual(PanoramaProjectionPolicy.detectedProjection(metadata: metadata), .cubemap)
        XCTAssertFalse(VideoProjection.cubemap.isRenderableInSphere)
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
}

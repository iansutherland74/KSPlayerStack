@testable import KSPlayer
import XCTest

final class VideoColorAdjustmentTest: XCTestCase {
    func testVideoColorAdjustmentDefaultsToNeutral() {
        let adjustment = KSOptions().videoColorAdjustment

        XCTAssertEqual(adjustment, .neutral)
        XCTAssertTrue(adjustment.isNeutral)
        XCTAssertFalse(adjustment.shouldApply(dynamicRange: .sdr))
    }

    func testVideoColorAdjustmentClampsInputValues() {
        let adjustment = VideoColorAdjustment(saturation: -1, brightness: 2, contrast: 3)

        XCTAssertEqual(adjustment.saturation, 0)
        XCTAssertEqual(adjustment.brightness, 1)
        XCTAssertEqual(adjustment.contrast, 2)
    }

    func testVideoColorAdjustmentFallsBackToNeutralDefaultsForNonFiniteInput() {
        let adjustment = VideoColorAdjustment(saturation: .nan, brightness: .infinity, contrast: -.infinity)

        XCTAssertEqual(adjustment.saturation, VideoColorAdjustment.defaultSaturation)
        XCTAssertEqual(adjustment.brightness, VideoColorAdjustment.defaultBrightness)
        XCTAssertEqual(adjustment.contrast, VideoColorAdjustment.defaultContrast)
        XCTAssertTrue(adjustment.isNeutral)
    }

    func testVideoColorAdjustmentAppliesToSDRWhenNonNeutral() {
        let adjustment = VideoColorAdjustment(saturation: 1.2, brightness: 0.1, contrast: 1.1)

        XCTAssertTrue(adjustment.shouldApply(dynamicRange: .sdr))
        XCTAssertFalse(adjustment.isNeutral)
    }

    func testVideoColorAdjustmentPreservesHDRByDefault() {
        let adjustment = VideoColorAdjustment(saturation: 1.2)

        XCTAssertFalse(adjustment.shouldApply(dynamicRange: .hdr10))
        XCTAssertFalse(adjustment.shouldApply(dynamicRange: .hlg))
        XCTAssertFalse(adjustment.shouldApply(dynamicRange: .dolbyVision))
    }

    func testVideoColorAdjustmentCanOptIntoHDR() {
        let adjustment = VideoColorAdjustment(saturation: 1.2, hdrPolicy: .allowHDR)

        XCTAssertTrue(adjustment.shouldApply(dynamicRange: .hdr10))
        XCTAssertTrue(adjustment.shouldApply(dynamicRange: .dolbyVision))
    }

    func testNeutralColorAdjustmentDoesNotApplyEvenWhenHDROptIn() {
        let adjustment = VideoColorAdjustment(hdrPolicy: .allowHDR)

        XCTAssertTrue(adjustment.isNeutral)
        XCTAssertFalse(adjustment.shouldApply(dynamicRange: .hdr10))
    }

    func testDisplayLayerPolicyUsesMetalOnlyWhenAdjustmentShouldApply() {
        let options = KSOptions()
        XCTAssertTrue(options.isUseDisplayLayer(dynamicRange: .sdr))

        options.videoColorAdjustment = VideoColorAdjustment(contrast: 1.1)
        XCTAssertFalse(options.isUseDisplayLayer(dynamicRange: .sdr))
        XCTAssertTrue(options.isUseDisplayLayer(dynamicRange: .hdr10))

        options.videoColorAdjustment = VideoColorAdjustment(contrast: 1.1, hdrPolicy: .allowHDR)
        XCTAssertFalse(options.isUseDisplayLayer(dynamicRange: .hdr10))
    }

    @MainActor
    func testPreferredPlayerTypeRoutesColorAdjustmentToMEPlayer() throws {
        let originalFirstPlayerType = KSOptions.firstPlayerType
        defer {
            KSOptions.firstPlayerType = originalFirstPlayerType
        }
        KSOptions.firstPlayerType = KSAVPlayer.self

        let options = KSOptions()
        options.videoColorAdjustment = VideoColorAdjustment(saturation: 1.1)

        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: try XCTUnwrap(URL(string: "https://example.com/movie.mp4")), options: options) == KSMEPlayer.self)
    }

    @MainActor
    func testPreferredPlayerTypeKeepsSeparateAudioVideoOnAVPlayer() throws {
        let originalFirstPlayerType = KSOptions.firstPlayerType
        defer {
            KSOptions.firstPlayerType = originalFirstPlayerType
        }
        KSOptions.firstPlayerType = KSMEPlayer.self

        let options = KSOptions()
        options.videoColorAdjustment = VideoColorAdjustment(saturation: 1.1)
        let videoURL = try XCTUnwrap(URL(string: "https://example.com/movie.mp4"))
        let audioURL = try XCTUnwrap(URL(string: "https://example.com/audio.m4a"))

        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: videoURL, audioURL: audioURL, options: options) == KSAVPlayer.self)
    }
}

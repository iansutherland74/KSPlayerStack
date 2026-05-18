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

    func testDisplayLayerPolicyUsesMetalOnlyWhenAdjustmentShouldApply() {
        let options = KSOptions()
        XCTAssertTrue(options.isUseDisplayLayer(dynamicRange: .sdr))

        options.videoColorAdjustment = VideoColorAdjustment(contrast: 1.1)
        XCTAssertFalse(options.isUseDisplayLayer(dynamicRange: .sdr))
        XCTAssertTrue(options.isUseDisplayLayer(dynamicRange: .hdr10))
    }
}

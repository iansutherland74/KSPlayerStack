@testable import KSPlayer
import CoreGraphics
import XCTest

final class VideoUpscalingOptionTest: XCTestCase {
    func testVideoUpscalingDefaultsToDisabled() {
        XCTAssertEqual(KSOptions().videoUpscaling, .none)
        XCTAssertFalse(KSOptions().videoUpscaling.isEnabled)
    }

    func testAppleSuperResolutionOptionStoresRequestedScaleFactor() {
        let options = KSOptions()
        options.videoUpscaling = .appleSuperResolution(scaleFactor: 2)
        XCTAssertEqual(options.videoUpscaling, .appleSuperResolution(scaleFactor: 2))
        XCTAssertTrue(options.videoUpscaling.isEnabled)
        XCTAssertEqual(options.videoUpscaling.requestedScaleFactor, 2)
    }

    func testAppleSuperResolutionClampsRequestedScaleFactor() {
        XCTAssertEqual(VideoUpscalingMode.appleSuperResolution(scaleFactor: 0).requestedScaleFactor, 1)
        XCTAssertEqual(VideoUpscalingMode.appleSuperResolution(scaleFactor: 10).requestedScaleFactor, 4)
        XCTAssertEqual(VideoUpscalingMode.appleSuperResolution(scaleFactor: .nan).requestedScaleFactor, 2)
    }

    func testAppleSuperResolutionRuntimeAvailabilityCanBeQueried() {
        _ = VideoUpscalingMode.isAppleSuperResolutionRuntimeAvailable
    }

    func testUpscalingStateDefaultsToInactive() {
        XCTAssertEqual(KSOptions().videoUpscalingState, .inactive)
        XCTAssertFalse(KSOptions().videoUpscalingState.isActive)
        XCTAssertNil(KSOptions().videoUpscalingState.unavailableReason)
    }

    func testUpscalingStateConvenienceAccessors() {
        let active = VideoUpscalingState.active(sourceSize: CGSize(width: 1920, height: 1080), outputSize: CGSize(width: 3840, height: 2160), scaleFactor: 2)
        let unavailable = VideoUpscalingState.unavailable(reason: "not supported")

        XCTAssertTrue(active.isActive)
        XCTAssertNil(active.unavailableReason)
        XCTAssertFalse(unavailable.isActive)
        XCTAssertEqual(unavailable.unavailableReason, "not supported")
    }

    func testUpscalingPolicySkipsResolvedOneTimesScale() {
        let sourceSize = CGSize(width: 1920, height: 1080)
        let mode = VideoUpscalingMode.appleSuperResolution(scaleFactor: 1)

        XCTAssertFalse(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: mode, sourceSize: sourceSize, fps: 60, dynamicRange: .sdr))
        XCTAssertEqual(
            HighPerformanceVideoPlaybackPolicy.upscalingSkipReason(mode: mode, sourceSize: sourceSize, fps: 60, dynamicRange: .sdr),
            "scale factor is 1x"
        )
    }

    func testUpscalingPolicyPreservesHDRByDefault() {
        let sourceSize = CGSize(width: 1920, height: 1080)
        let mode = VideoUpscalingMode.appleSuperResolution(scaleFactor: 2)

        XCTAssertTrue(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: mode, sourceSize: sourceSize, fps: 60, dynamicRange: .sdr))
        XCTAssertFalse(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: mode, sourceSize: sourceSize, fps: 60, dynamicRange: .hdr10))
        XCTAssertFalse(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: mode, sourceSize: sourceSize, fps: 60, dynamicRange: .dolbyVision))
    }

    func testUpscalingPolicyCanOptIntoHDR() {
        let sourceSize = CGSize(width: 1920, height: 1080)
        let mode = VideoUpscalingMode.appleSuperResolution(scaleFactor: 2, hdrPolicy: .allowHDR)

        XCTAssertTrue(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: mode, sourceSize: sourceSize, fps: 60, dynamicRange: .hdr10))
    }

    func testUpscalingPolicySkipsHighWorkload() {
        let eightK = CGSize(width: 7680, height: 4320)
        let mode = VideoUpscalingMode.appleSuperResolution(scaleFactor: 2)

        XCTAssertFalse(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: mode, sourceSize: eightK, fps: 30, dynamicRange: .sdr))
        XCTAssertFalse(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: mode, sourceSize: CGSize(width: 1920, height: 1080), fps: 120, dynamicRange: .sdr))
    }
}

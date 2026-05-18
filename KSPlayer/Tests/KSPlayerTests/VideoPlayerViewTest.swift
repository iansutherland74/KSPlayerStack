@testable import KSPlayer
import XCTest

class VideoPlayerViewTest: XCTestCase {
    func testResize() {}

    func testProgressPreviewClampsCenterWithinSlider() {
        XCTAssertEqual(ProgressPreviewResolver.previewCenterX(time: -5, totalTime: 100, sliderWidth: 300, previewWidth: 120), 60)
        XCTAssertEqual(ProgressPreviewResolver.previewCenterX(time: 50, totalTime: 100, sliderWidth: 300, previewWidth: 120), 150)
        XCTAssertEqual(ProgressPreviewResolver.previewCenterX(time: 120, totalTime: 100, sliderWidth: 300, previewWidth: 120), 240)
    }

    func testProgressPreviewNearestThumbnailIndex() {
        let times: [TimeInterval] = [0, 10, 30, 60]

        XCTAssertEqual(ProgressPreviewResolver.nearestThumbnailIndex(times: times, target: 12), 1)
        XCTAssertEqual(ProgressPreviewResolver.nearestThumbnailIndex(times: times, target: 46), 3)
        XCTAssertNil(ProgressPreviewResolver.nearestThumbnailIndex(times: [], target: 12))
    }

    func testDefinitionSwitchPrewarmPolicyRequiresSeekableVOD() {
        XCTAssertTrue(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 120,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: false,
            duration: 120,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 0,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 120,
            targetTime: 30,
            isSeekable: false,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 120,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: true,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 120,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: true
        ))
    }

    func testDefinitionSwitchHandoffTimeAdvancesOnlyWhenPlaying() {
        XCTAssertEqual(DefinitionSwitchPrewarmPolicy.handoffTime(requestedTime: 10, elapsed: 2, playbackRate: 1.5, wasPlaying: true, duration: 20), 13)
        XCTAssertEqual(DefinitionSwitchPrewarmPolicy.handoffTime(requestedTime: 10, elapsed: 2, playbackRate: 1.5, wasPlaying: false, duration: 20), 10)
        XCTAssertEqual(DefinitionSwitchPrewarmPolicy.handoffTime(requestedTime: 19, elapsed: 2, playbackRate: 1.5, wasPlaying: true, duration: 20), 20)
    }
}

@testable import KSPlayer
import AVFoundation
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

    func testMediaPlaybackTimeRangeClampsToWindow() {
        let range = MediaPlaybackTimeRange(start: 100, duration: 30)

        XCTAssertEqual(range?.duration, 30)
        XCTAssertEqual(range?.clamped(90), 100)
        XCTAssertEqual(range?.clamped(115), 115)
        XCTAssertEqual(range?.clamped(140), 130)
        XCTAssertNil(MediaPlaybackTimeRange(start: 100, duration: 0))
    }

    func testAVPlayerSeekableRangeResolverUnionsFiniteRanges() {
        let ranges = [
            CMTimeRange(start: CMTime(seconds: 120, preferredTimescale: 600), duration: CMTime(seconds: 30, preferredTimescale: 600)),
            CMTimeRange(start: CMTime(seconds: 180, preferredTimescale: 600), duration: CMTime(seconds: 20, preferredTimescale: 600)),
        ]
        let range = AVPlayerSeekableRangeResolver.range(from: ranges)

        XCTAssertEqual(range?.start, 120)
        XCTAssertEqual(range?.end, 200)
        XCTAssertNil(AVPlayerSeekableRangeResolver.range(from: [
            CMTimeRange(start: .zero, duration: .positiveInfinity),
        ]))
    }

    func testFFmpegSeekabilityAllowsPlaylistDVRButNotGenericNonSeekableLive() {
        XCTAssertTrue(FFmpegSeekabilityPolicy.isPlaylistFormat("hls"))
        XCTAssertTrue(FFmpegSeekabilityPolicy.isPlaylistFormat("dash"))
        XCTAssertTrue(FFmpegSeekabilityPolicy.isPlaylistFormat("applehttp,hls"))
        XCTAssertFalse(FFmpegSeekabilityPolicy.isPlaylistFormat("mpegts"))

        XCTAssertEqual(FFmpegSeekabilityPolicy.seekableTimeRange(duration: 60, ioSeekable: false, formatName: "hls")?.duration, 60)
        XCTAssertEqual(FFmpegSeekabilityPolicy.seekableTimeRange(duration: 60, ioSeekable: false, formatName: "dash")?.duration, 60)
        XCTAssertNil(FFmpegSeekabilityPolicy.seekableTimeRange(duration: 60, ioSeekable: false, formatName: "mpegts"))
        XCTAssertNil(FFmpegSeekabilityPolicy.seekableTimeRange(duration: 0, ioSeekable: true, formatName: "hls"))
    }

    @MainActor
    func testToolbarMapsLiveDVRSliderToMediaTime() {
        let toolbar = PlayerToolBar()

        toolbar.totalTime = 0
        toolbar.seekableTimeRange = MediaPlaybackTimeRange(start: 120, duration: 45)
        toolbar.currentTime = 150

        XCTAssertEqual(toolbar.sliderDuration, 45)
        XCTAssertEqual(toolbar.timeSlider.maximumValue, 45)
        XCTAssertEqual(toolbar.timeSlider.value, 30)
        XCTAssertEqual(toolbar.mediaTime(forSliderValue: 10), 130)
        XCTAssertEqual(toolbar.mediaTime(forSliderValue: 100), 165)
        XCTAssertEqual(toolbar.displayTime(for: 150), 30)
    }

    @MainActor
    func testToolbarDoesNotTreatNonDVRLiveAsRewindable() {
        let toolbar = PlayerToolBar()

        toolbar.totalTime = 1
        toolbar.totalTime = 0
        toolbar.seekableTimeRange = nil
        toolbar.isSeekable = false

        XCTAssertFalse(toolbar.isLiveDVRStream)
        XCTAssertEqual(toolbar.sliderDuration, 0)
        XCTAssertEqual(toolbar.timeSlider.maximumValue, Float(60 * 60 * 24))
        XCTAssertFalse(toolbar.timeSlider.isUserInteractionEnabled)
        XCTAssertEqual(toolbar.mediaTime(forSliderValue: 10), 10)
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

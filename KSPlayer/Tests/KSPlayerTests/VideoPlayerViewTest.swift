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
}

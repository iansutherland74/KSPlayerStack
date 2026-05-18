@testable import KSPlayer
import XCTest

final class VideoUpscalingOptionTest: XCTestCase {
    func testVideoUpscalingDefaultsToDisabled() {
        XCTAssertEqual(KSOptions().videoUpscaling, .none)
    }

    func testAppleSuperResolutionOptionStoresRequestedScaleFactor() {
        let options = KSOptions()
        options.videoUpscaling = .appleSuperResolution(scaleFactor: 2)
        XCTAssertEqual(options.videoUpscaling, .appleSuperResolution(scaleFactor: 2))
    }
}

@testable import KSPlayer
import XCTest

final class PictureInPicturePolicyTest: XCTestCase {
    func testStartPolicyAllowsSupportedPossibleController() {
        XCTAssertNil(PictureInPictureStartPolicy.unavailableReason(
            isSystemSupported: true,
            hasController: true,
            isPossible: true
        ))
    }

    func testStartPolicyReportsUnsupportedSystem() {
        XCTAssertEqual(
            PictureInPictureStartPolicy.unavailableReason(
                isSystemSupported: false,
                hasController: true,
                isPossible: true
            ),
            "system Picture in Picture support is unavailable"
        )
    }

    func testStartPolicyReportsMissingControllerBeforePossibleState() {
        XCTAssertEqual(
            PictureInPictureStartPolicy.unavailableReason(
                isSystemSupported: true,
                hasController: false,
                isPossible: false
            ),
            "player does not expose a Picture in Picture controller"
        )
    }

    func testStartPolicyReportsTemporarilyImpossibleController() {
        XCTAssertEqual(
            PictureInPictureStartPolicy.unavailableReason(
                isSystemSupported: true,
                hasController: true,
                isPossible: false
            ),
            "Picture in Picture is not possible for the current player state"
        )
    }

    func testSubtitlePolicyKeepsNativeLegibleAutomaticPath() {
        let diagnostic = PictureInPictureSubtitlePolicyResolver.diagnostic(
            policy: .automatic,
            usesNativeLegibleSelection: true
        )
        XCTAssertEqual(diagnostic.status, .nativeLegible)
    }

    func testSubtitlePolicyReportsInlineOverlayAutomaticPath() {
        let diagnostic = PictureInPictureSubtitlePolicyResolver.diagnostic(
            policy: .automatic,
            usesNativeLegibleSelection: false
        )
        XCTAssertEqual(diagnostic.status, .inlineOverlayOnly)
    }

    func testSubtitlePolicyAllowsExplicitBurnInWhenRendererSupportsIt() {
        let diagnostic = PictureInPictureSubtitlePolicyResolver.diagnostic(
            policy: .burnIn,
            usesNativeLegibleSelection: false,
            supportsSampleBufferBurnIn: true
        )
        XCTAssertEqual(diagnostic.status, .burnedIn)
    }

    func testSubtitlePolicyReportsExplicitBurnInUnavailable() {
        let diagnostic = PictureInPictureSubtitlePolicyResolver.diagnostic(
            policy: .burnIn,
            usesNativeLegibleSelection: true,
            supportsSampleBufferBurnIn: false,
            burnInUnavailableReason: "native AVPlayer PiP cannot mutate video frames"
        )
        XCTAssertEqual(diagnostic.status, .burnInUnavailable)
        XCTAssertTrue(diagnostic.message.contains("native AVPlayer PiP cannot mutate video frames"))
    }

    func testSubtitleBurnInLayoutStacksSecondaryAbovePrimary() {
        let primary = PictureInPictureSubtitleCue.text(
            NSAttributedString(string: "Primary", attributes: [.font: UIFont.systemFont(ofSize: 20)]),
            role: .primary
        )
        let secondary = PictureInPictureSubtitleCue.text(
            NSAttributedString(string: "Secondary", attributes: [.font: UIFont.systemFont(ofSize: 20)]),
            role: .secondary
        )
        let snapshot = PictureInPictureSubtitleSnapshot(cues: [secondary, primary])
        let commands = PictureInPictureSubtitleComposer.layout(snapshot: snapshot, videoSize: CGSize(width: 640, height: 360))

        XCTAssertEqual(commands.count, 2)
        XCTAssertGreaterThan(commands[0].rect.minY, commands[1].rect.minY)
        XCTAssertLessThanOrEqual(commands[0].rect.maxY, 355)
    }
}

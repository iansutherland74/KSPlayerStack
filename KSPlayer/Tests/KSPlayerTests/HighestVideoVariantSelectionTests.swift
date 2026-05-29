@testable import KSPlayer
import AVFoundation
import CoreMedia
import XCTest

final class HighestVideoVariantSelectionTests: XCTestCase {
    func testHighestQualityVideoTrackIndexPrefersBitrateThenResolution() {
        let tracks: [MediaPlayerTrack] = [
            MockVideoTrack(trackID: 0, bitRate: 1_500_000, width: 960, height: 540),
            MockVideoTrack(trackID: 1, bitRate: 6_500_000, width: 1920, height: 1080),
            MockVideoTrack(trackID: 2, bitRate: 3_000_000, width: 1280, height: 720),
        ]

        XCTAssertEqual(KSOptions.highestQualityVideoTrackIndex(in: tracks), 1)
    }

    func testWantedVideoReturnsHighestWhenPreferenceEnabled() {
        let options = KSOptions()
        options.prefersHighestVideoVariant = true
        let tracks: [MediaPlayerTrack] = [
            MockVideoTrack(trackID: 0, bitRate: 800_000, width: 640, height: 360),
            MockVideoTrack(trackID: 1, bitRate: 4_500_000, width: 1920, height: 1080),
        ]

        XCTAssertEqual(options.wantedVideo(tracks: tracks), 1)
    }
}

private final class MockVideoTrack: MediaPlayerTrack {
    let trackID: Int32
    let name: String
    let languageCode: String?
    let mediaType: AVMediaType = .video
    var nominalFrameRate: Float
    let bitRate: Int64
    let bitDepth: Int32 = 8
    var isEnabled = false
    let isImageSubtitle = false
    let rotation: Int16 = 0
    let dovi: DOVIDecoderConfigurationRecord? = nil
    let fieldOrder: FFmpegFieldOrder = .unknown
    let formatDescription: CMFormatDescription?

    var description: String {
        "\(Int(bitRate))bps"
    }

    init(trackID: Int32, bitRate: Int64, width: Int, height: Int, fps: Float = 30) {
        self.trackID = trackID
        self.name = "v\(trackID)"
        self.languageCode = nil
        self.bitRate = bitRate
        self.nominalFrameRate = fps
        var formatDescription: CMFormatDescription?
        CMVideoFormatDescriptionCreate(
            allocator: kCFAllocatorDefault,
            codecType: kCMVideoCodecType_H264,
            width: Int32(width),
            height: Int32(height),
            extensions: nil,
            formatDescriptionOut: &formatDescription
        )
        self.formatDescription = formatDescription
    }
}

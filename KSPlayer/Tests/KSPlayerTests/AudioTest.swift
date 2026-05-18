import AVFoundation
import FFmpegKit
@testable import KSPlayer
import Libavcodec
import XCTest

class AudioTest: XCTestCase {
    func testChannelLayout() {
        for (tag, mask) in layoutMapTuple {
            assert(tag: tag, mask: mask)
        }
    }

    private func assert(tag: AudioChannelLayoutTag, mask: UInt64) {
        let channelLayout = AVAudioChannelLayout(layout: tag.channelLayout)
        XCTAssertEqual(channelLayout.channelLayout().u.mask == mask, true)
    }

    private func assert(bitmap: AudioChannelBitmap, mask: UInt64) {
        let channelLayout = AVAudioChannelLayout(layout: bitmap.channelLayout)
        XCTAssertEqual(channelLayout.channelLayout().u.mask == mask, true)
    }

    func testDolbyAtmosEAC3ProfileMetadata() {
        let codecpar = makeAudioCodecParameters(codecID: AV_CODEC_ID_EAC3, profile: AV_PROFILE_EAC3_DDP_ATMOS)
        guard let track = FFmpegAssetTrack(codecpar: codecpar) else {
            XCTFail("Expected E-AC-3 track metadata")
            return
        }

        XCTAssertEqual(track.audioCodecMetadata?.displayName, "Dolby Digital Plus Atmos")
        XCTAssertEqual(track.audioCodecMetadata?.profileName, "Dolby Digital Plus + Dolby Atmos")
        XCTAssertEqual(track.audioCodecMetadata?.isDolbyAtmos, true)
        XCTAssertEqual(track.audioDecodeSupport.isSupported, true)
        XCTAssertTrue(track.description.contains("Dolby Digital Plus Atmos"))
    }

    func testDolbyAC4ReportsUnsupportedDecode() {
        let codecpar = makeAudioCodecParameters(codecID: AV_CODEC_ID_AC4)
        guard let track = FFmpegAssetTrack(codecpar: codecpar) else {
            XCTFail("Expected AC-4 track metadata")
            return
        }

        XCTAssertEqual(track.mediaSubType.rawValue.string, "ac-4")
        XCTAssertEqual(track.audioCodecMetadata?.displayName, "Dolby AC-4")
        XCTAssertEqual(track.audioCodecMetadata?.isDolbyAC4, true)
        XCTAssertEqual(track.audioDecodeSupport.isSupported, false)
        XCTAssertTrue(track.description.contains("AC-4 demuxing is available"))
    }

    func testAtmosChannelLayoutsMapToCoreAudioTags() {
        let channelLayout = AVChannelLayout(order: AV_CHANNEL_ORDER_NATIVE, nb_channels: 8, u: AVChannelLayout.__Unnamed_union_u(mask: swift_AV_CH_LAYOUT_5POINT1POINT2), opaque: nil)
        XCTAssertEqual(channelLayout.layoutTag, kAudioChannelLayoutTag_Atmos_5_1_2)
        XCTAssertTrue(channelLayout.isDolbyAtmosBedLayout)
    }

    private func makeAudioCodecParameters(codecID: AVCodecID, profile: Int32 = AV_PROFILE_UNKNOWN, channels: Int32 = 6) -> AVCodecParameters {
        var codecpar = AVCodecParameters()
        codecpar.codec_type = AVMEDIA_TYPE_AUDIO
        codecpar.codec_id = codecID
        codecpar.profile = profile
        codecpar.format = AV_SAMPLE_FMT_FLTP.rawValue
        codecpar.sample_rate = 48000
        codecpar.bit_rate = 640_000
        av_channel_layout_default(&codecpar.ch_layout, channels)
        return codecpar
    }
}

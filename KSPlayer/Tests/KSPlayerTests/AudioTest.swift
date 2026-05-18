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

    func testDolbyAC4AtmosTitleMetadataDoesNotClaimDecodeSupport() {
        let codecpar = makeAudioCodecParameters(codecID: AV_CODEC_ID_AC4)
        let metadata = FFmpegAssetTrack.audioCodecMetadata(
            codecID: codecpar.codec_id,
            profile: codecpar.profile,
            codecName: "ac4",
            profileName: nil,
            channelLayout: codecpar.ch_layout,
            title: "English Dolby Atmos",
            decodeSupport: FFmpegAssetTrack.audioDecodeSupport(codecID: codecpar.codec_id)
        )

        XCTAssertEqual(metadata?.displayName, "Dolby AC-4 Atmos")
        XCTAssertEqual(metadata?.isDolbyAC4, true)
        XCTAssertEqual(metadata?.isDolbyAtmos, true)
        XCTAssertEqual(metadata?.decodeSupport.isSupported, false)
    }

    func testDolbyAC4IsNotSelectedAsDecodableFFmpegAudio() {
        guard let ac4Track = FFmpegAssetTrack(codecpar: makeAudioCodecParameters(codecID: AV_CODEC_ID_AC4)),
              let eac3Track = FFmpegAssetTrack(codecpar: makeAudioCodecParameters(codecID: AV_CODEC_ID_EAC3))
        else {
            XCTFail("Expected Dolby audio track metadata")
            return
        }

        let decodableTracks = FFmpegAssetTrack.decodableAudioTracks([ac4Track, eac3Track])

        XCTAssertEqual(decodableTracks.count, 1)
        XCTAssertTrue(decodableTracks[0] === eac3Track)
        XCTAssertFalse(decodableTracks.contains { $0 === ac4Track })
    }

    func testAV1CodecMapsToCoreMediaSampleEntry() {
        let mediaSubType = AV_CODEC_ID_AV1.mediaSubType

        XCTAssertEqual(mediaSubType.rawValue.string, "av01")
        XCTAssertEqual(mediaSubType.rawValue.avc, "av1C")
    }

    func testAtmosChannelLayoutsMapToCoreAudioTags() {
        let channelLayout = AVChannelLayout(order: AV_CHANNEL_ORDER_NATIVE, nb_channels: 8, u: AVChannelLayout.__Unnamed_union_u(mask: swift_AV_CH_LAYOUT_5POINT1POINT2), opaque: nil)
        XCTAssertEqual(channelLayout.layoutTag, kAudioChannelLayoutTag_Atmos_5_1_2)
        XCTAssertTrue(channelLayout.isDolbyAtmosBedLayout)
    }

    func testAudioFormatDescriptionPreservesChannelLayout() {
        guard let track = FFmpegAssetTrack(codecpar: makeAudioCodecParameters(codecID: AV_CODEC_ID_EAC3, channels: 6)),
              let formatDescription = track.formatDescription
        else {
            XCTFail("Expected audio format description")
            return
        }

        var layoutSize = 0
        let layout = CMAudioFormatDescriptionGetChannelLayout(formatDescription, sizeOut: &layoutSize)

        XCTAssertNotNil(layout)
        XCTAssertGreaterThan(layoutSize, 0)
        XCTAssertEqual(layout.flatMap { AVAudioChannelLayout(layout: $0) }?.channelCount, 6)
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

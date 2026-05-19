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
        XCTAssertEqual(channelLayout.channelLayout().u.mask, mask, "Unexpected mask for layout tag \(tag)")
    }

    private func assert(bitmap: AudioChannelBitmap, mask: UInt64) {
        let channelLayout = AVAudioChannelLayout(layout: bitmap.channelLayout)
        XCTAssertEqual(channelLayout.channelLayout().u.mask, mask, "Unexpected mask for bitmap \(bitmap)")
    }

    #if !os(macOS)
    func testAudioRouteSharingPolicyUsesPlatformDefaultWhenUnset() {
        XCTAssertEqual(AudioRouteSharingPolicyResolver.resolvedPolicy(optionPolicy: nil, defaultPolicy: nil), AudioRouteSharingPolicyResolver.platformDefaultPolicy)
        #if os(tvOS)
        XCTAssertEqual(AudioRouteSharingPolicyResolver.platformDefaultPolicy, .longFormAudio)
        #else
        XCTAssertEqual(AudioRouteSharingPolicyResolver.platformDefaultPolicy, .longFormVideo)
        #endif
    }

    func testAudioRouteSharingPolicyPrefersOptionOverProcessDefault() {
        XCTAssertEqual(AudioRouteSharingPolicyResolver.resolvedPolicy(optionPolicy: .longFormAudio, defaultPolicy: .longFormVideo), .longFormAudio)
    }

    func testAudioRouteSharingPolicyUsesProcessDefaultWhenOptionUnset() {
        XCTAssertEqual(AudioRouteSharingPolicyResolver.resolvedPolicy(optionPolicy: nil, defaultPolicy: .longFormAudio), .longFormAudio)
    }
    #endif

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
        XCTAssertEqual(track.isSelectableForFFmpegPlayback, false)
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
        XCTAssertEqual(ac4Track.isSelectableForFFmpegPlayback, false)
        XCTAssertEqual(eac3Track.isSelectableForFFmpegPlayback, true)
    }

    func testDolbyAC4CoreMediaSubtypeDisplayName() {
        let mediaSubType = AV_CODEC_ID_AC4.mediaSubType

        XCTAssertEqual(mediaSubType, .dolbyAC4)
        XCTAssertEqual(mediaSubType.rawValue.string, "ac-4")
        XCTAssertEqual(mediaSubType.audioCodecDisplayName, "Dolby AC-4")
    }

    func testDolbyCoreMediaSubtypeDisplayNames() {
        XCTAssertEqual(CMFormatDescription.MediaSubType.dolbyDigital.audioCodecDisplayName, "Dolby Digital")
        XCTAssertEqual(CMFormatDescription.MediaSubType.dolbyDigitalPlus.audioCodecDisplayName, "Dolby Digital Plus")
        XCTAssertEqual(CMFormatDescription.MediaSubType.dolbyTrueHD.audioCodecDisplayName, "Dolby TrueHD")
    }

    func testAudioRouteOutputClassifier() {
        XCTAssertEqual(AudioRouteOutputClassifier.outputKind(portTypeRawValue: "AirPlay"), .airPlay)
        XCTAssertEqual(AudioRouteOutputClassifier.outputKind(portTypeRawValue: "BluetoothA2DPOutput"), .bluetooth)
        XCTAssertEqual(AudioRouteOutputClassifier.outputKind(portTypeRawValue: "HDMIOutput"), .hdmi)
        XCTAssertEqual(AudioRouteOutputClassifier.outputKind(portTypeRawValue: "BuiltInSpeaker"), .builtIn)
        XCTAssertEqual(AudioRouteOutputClassifier.outputKind(portTypeRawValue: "Headphones"), .wired)
        XCTAssertTrue(AudioRouteOutputClassifier.isExternalRoute(.airPlay))
        XCTAssertTrue(AudioRouteOutputClassifier.isExternalRoute(.hdmi))
        XCTAssertFalse(AudioRouteOutputClassifier.isExternalRoute(.builtIn))
    }

    func testEncodedPassthroughPolicyIsNativeOnly() {
        let nativePolicy = EncodedAudioPassthroughPolicyResolver.policy(
            pipeline: .nativeAVPlayer,
            containsEncodedPassthroughCandidate: true
        )
        let pcmPolicy = EncodedAudioPassthroughPolicyResolver.policy(
            pipeline: .decodedPCM,
            containsEncodedPassthroughCandidate: true
        )

        XCTAssertEqual(nativePolicy.availability, .nativeRouteDependent)
        XCTAssertTrue(nativePolicy.requiresHardwareRouteValidation)
        XCTAssertEqual(pcmPolicy.availability, .decodedPCMOnly)
        XCTAssertFalse(pcmPolicy.requiresHardwareRouteValidation)
        XCTAssertTrue(pcmPolicy.reason.contains("PCM"))
    }

    func testEncodedPassthroughSubtypeDetection() {
        XCTAssertTrue(EncodedAudioPassthroughPolicyResolver.isEncodedPassthroughCandidate(mediaSubTypeRawValue: "ec-3"))
        XCTAssertTrue(EncodedAudioPassthroughPolicyResolver.isEncodedPassthroughCandidate(mediaSubTypeRawValue: "ac-4"))
        XCTAssertTrue(EncodedAudioPassthroughPolicyResolver.isEncodedPassthroughCandidate(mediaSubTypeRawValue: "mlpa"))
        XCTAssertFalse(EncodedAudioPassthroughPolicyResolver.isEncodedPassthroughCandidate(mediaSubTypeRawValue: "lpcm"))
    }

    func testAudioRouteDiagnosticReportsExternalOutputsWithoutHardwareValidation() {
        let policy = EncodedAudioPassthroughPolicyResolver.policy(
            pipeline: .decodedPCM,
            containsEncodedPassthroughCandidate: true
        )
        let diagnostic = AudioRouteDiagnostic(
            playbackPipeline: .decodedPCM,
            outputPorts: [
                AudioRouteOutputDiagnostic(
                    portName: "Living Room",
                    portType: "AirPlay",
                    outputKind: .airPlay,
                    channelCount: 2,
                    isSpatialAudioEnabled: nil
                ),
            ],
            maximumOutputNumberOfChannels: 2,
            preferredOutputNumberOfChannels: 2,
            outputNumberOfChannels: 2,
            outputLatency: 0.1,
            routeSharingPolicy: "longFormAudio",
            configuredSupportsMultichannelContent: true,
            spatialPreference: .automatic,
            multichannelPreference: .automatic,
            sourceChannelCount: 6,
            allowsExternalPlayback: false,
            usesExternalPlaybackWhileExternalScreenIsActive: false,
            isExternalPlaybackActive: false,
            encodedPassthroughPolicy: policy
        )

        XCTAssertTrue(diagnostic.hasExternalOutput)
        XCTAssertEqual(diagnostic.encodedPassthroughPolicy.availability, .decodedPCMOnly)
    }

    func testAV1CodecMapsToCoreMediaSampleEntry() {
        let mediaSubType = AV_CODEC_ID_AV1.mediaSubType

        XCTAssertEqual(mediaSubType.rawValue.string, "av01")
        XCTAssertEqual(mediaSubType.rawValue.avc, "av1C")
    }

    func testUnsupportedVideoCodecReportsUnsupportedDecode() {
        guard let track = FFmpegAssetTrack(codecpar: makeVideoCodecParameters(codecID: AV_CODEC_ID_NONE)) else {
            XCTFail("Expected video track metadata")
            return
        }

        XCTAssertEqual(track.videoDecodeSupport.isSupported, false)
        XCTAssertEqual(track.isSelectableForFFmpegPlayback, false)
        XCTAssertTrue(track.description.contains("no FFmpeg decoder is available"))
    }

    func testUnsupportedVideoIsNotSelectedAsDecodableFFmpegVideo() {
        guard let unsupportedTrack = FFmpegAssetTrack(codecpar: makeVideoCodecParameters(codecID: AV_CODEC_ID_NONE)),
              let h264Track = FFmpegAssetTrack(codecpar: makeVideoCodecParameters(codecID: AV_CODEC_ID_H264))
        else {
            XCTFail("Expected video track metadata")
            return
        }

        let decodableTracks = FFmpegAssetTrack.decodableVideoTracks([unsupportedTrack, h264Track])

        XCTAssertEqual(decodableTracks.count, 1)
        XCTAssertTrue(decodableTracks[0] === h264Track)
        XCTAssertFalse(decodableTracks.contains { $0 === unsupportedTrack })
        XCTAssertEqual(unsupportedTrack.isSelectableForFFmpegPlayback, false)
        XCTAssertEqual(h264Track.isSelectableForFFmpegPlayback, true)
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

    func testAtmosAudioFormatDescriptionPreservesLayoutTag() {
        var codecpar = makeAudioCodecParameters(codecID: AV_CODEC_ID_TRUEHD, channels: 8)
        codecpar.ch_layout = AVChannelLayout(order: AV_CHANNEL_ORDER_NATIVE, nb_channels: 8, u: AVChannelLayout.__Unnamed_union_u(mask: swift_AV_CH_LAYOUT_5POINT1POINT2), opaque: nil)
        guard let track = FFmpegAssetTrack(codecpar: codecpar),
              let formatDescription = track.formatDescription
        else {
            XCTFail("Expected Atmos audio format description")
            return
        }

        var layoutSize = 0
        let layout = CMAudioFormatDescriptionGetChannelLayout(formatDescription, sizeOut: &layoutSize)

        XCTAssertNotNil(layout)
        XCTAssertGreaterThan(layoutSize, 0)
        XCTAssertEqual(layout.flatMap { AVAudioChannelLayout(layout: $0) }?.layoutTag, kAudioChannelLayoutTag_Atmos_5_1_2)
    }

    func testAudioMultichannelContentSupportResolver() {
        XCTAssertFalse(AudioMultichannelContentSupportResolver.supportsMultichannelContent(spatialPreference: .automatic, multichannelPreference: .automatic, sourceChannelCount: nil, isSpatialRoute: nil))
        XCTAssertTrue(AudioMultichannelContentSupportResolver.supportsMultichannelContent(spatialPreference: .automatic, multichannelPreference: .automatic, sourceChannelCount: 6, isSpatialRoute: false))
        XCTAssertTrue(AudioMultichannelContentSupportResolver.supportsMultichannelContent(spatialPreference: .automatic, multichannelPreference: .automatic, sourceChannelCount: 2, isSpatialRoute: true))
        XCTAssertTrue(AudioMultichannelContentSupportResolver.supportsMultichannelContent(spatialPreference: .enabled, multichannelPreference: .multichannel, sourceChannelCount: nil, isSpatialRoute: false))
        XCTAssertFalse(AudioMultichannelContentSupportResolver.supportsMultichannelContent(spatialPreference: .enabled, multichannelPreference: .stereo, sourceChannelCount: 8, isSpatialRoute: true))
        XCTAssertFalse(AudioMultichannelContentSupportResolver.supportsMultichannelContent(spatialPreference: .disabled, multichannelPreference: .multichannel, sourceChannelCount: 8, isSpatialRoute: true))
    }

    func testAudioFormatPreservesAtmosLayoutWhenChannelsArePreserved() {
        var channelLayout = AVChannelLayout(order: AV_CHANNEL_ORDER_NATIVE, nb_channels: 8, u: AVChannelLayout.__Unnamed_union_u(mask: swift_AV_CH_LAYOUT_5POINT1POINT2), opaque: nil)
        let audioFormat = AudioDescriptor.audioFormat(sampleFormat: AV_SAMPLE_FMT_FLTP, sampleRate: 48_000, outChannel: &channelLayout, channelCount: 8)

        XCTAssertEqual(audioFormat.channelCount, 8)
        XCTAssertEqual(audioFormat.channelLayout?.layoutTag, kAudioChannelLayoutTag_Atmos_5_1_2)
    }

    func testAudioFormatDownmixesToStereoLayoutWhenRequested() {
        var channelLayout = AVChannelLayout(order: AV_CHANNEL_ORDER_NATIVE, nb_channels: 8, u: AVChannelLayout.__Unnamed_union_u(mask: swift_AV_CH_LAYOUT_5POINT1POINT2), opaque: nil)
        let audioFormat = AudioDescriptor.audioFormat(sampleFormat: AV_SAMPLE_FMT_FLTP, sampleRate: 48_000, outChannel: &channelLayout, channelCount: 2)

        XCTAssertEqual(channelLayout.nb_channels, 2)
        XCTAssertEqual(audioFormat.channelCount, 2)
        XCTAssertEqual(audioFormat.channelLayout?.layoutTag, kAudioChannelLayoutTag_Stereo)
    }

    func testUnsupportedNativeLayoutFallsBackToDefaultLayoutForChannelCount() {
        var channelLayout = AVChannelLayout(order: AV_CHANNEL_ORDER_UNSPEC, nb_channels: 4, u: AVChannelLayout.__Unnamed_union_u(mask: 0), opaque: nil)
        let audioFormat = AudioDescriptor.audioFormat(sampleFormat: AV_SAMPLE_FMT_FLTP, sampleRate: 48_000, outChannel: &channelLayout, channelCount: 4)

        XCTAssertEqual(channelLayout.nb_channels, 4)
        XCTAssertEqual(audioFormat.channelCount, 4)
        XCTAssertNotNil(audioFormat.channelLayout)
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

    private func makeVideoCodecParameters(codecID: AVCodecID) -> AVCodecParameters {
        var codecpar = AVCodecParameters()
        codecpar.codec_type = AVMEDIA_TYPE_VIDEO
        codecpar.codec_id = codecID
        codecpar.format = AV_PIX_FMT_YUV420P.rawValue
        codecpar.width = 1920
        codecpar.height = 1080
        return codecpar
    }
}

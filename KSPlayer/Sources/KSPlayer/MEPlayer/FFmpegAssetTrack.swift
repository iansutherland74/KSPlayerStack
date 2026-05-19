//
//  FFmpegAssetTrack.swift
//  KSPlayer
//
//  Created by kintan on 2023/2/12.
//

import AVFoundation
import FFmpegKit
import Libavcodec
import Libavformat

public enum FFmpegAudioDecodeSupport: Equatable, CustomStringConvertible {
    case supported
    case unsupported(String)

    public var isSupported: Bool {
        if case .supported = self {
            true
        } else {
            false
        }
    }

    public var description: String {
        switch self {
        case .supported:
            return "supported"
        case let .unsupported(reason):
            return reason
        }
    }
}

public struct FFmpegAudioCodecMetadata: Equatable {
    public let displayName: String
    public let profileName: String?
    public let isDolbyAtmos: Bool
    public let isDolbyAC4: Bool
    public let isDolbyTrueHD: Bool
    public let isDolbyEAC3: Bool
    public let decodeSupport: FFmpegAudioDecodeSupport
}

public enum FFmpegVideoDecodeSupport: Equatable, CustomStringConvertible {
    case supported
    case unsupported(String)

    public var isSupported: Bool {
        if case .supported = self {
            true
        } else {
            false
        }
    }

    public var description: String {
        switch self {
        case .supported:
            return "supported"
        case let .unsupported(reason):
            return reason
        }
    }
}

public class FFmpegAssetTrack: MediaPlayerTrack, SubtitleKindProviding {
    public private(set) var trackID: Int32 = 0
    public let codecName: String
    public var name: String = ""
    public private(set) var languageCode: String?
    public var nominalFrameRate: Float = 0
    public private(set) var avgFrameRate = Timebase.defaultValue
    public private(set) var realFrameRate = Timebase.defaultValue
    public private(set) var bitRate: Int64 = 0
    public let mediaType: AVFoundation.AVMediaType
    public let formatName: String?
    public let bitDepth: Int32
    private var stream: UnsafeMutablePointer<AVStream>?
    var startTime = CMTime.zero
    var codecpar: AVCodecParameters
    var timebase: Timebase = .defaultValue
    let bitsPerRawSample: Int32
    // audio
    public let audioDescriptor: AudioDescriptor?
    public private(set) var audioCodecMetadata: FFmpegAudioCodecMetadata?
    public private(set) var audioDecodeSupport: FFmpegAudioDecodeSupport = .supported
    public var isSelectableForFFmpegPlayback: Bool {
        switch mediaType {
        case .audio:
            return audioDecodeSupport.isSupported
        case .video:
            return videoDecodeSupport.isSupported
        default:
            return true
        }
    }
    // subtitle
    public let isImageSubtitle: Bool
    public var delay: TimeInterval = 0
    var subtitle: SyncPlayerItemTrack<SubtitleFrame>?
    var embeddedFontDirectoryURL: URL?
    // video
    public private(set) var videoDecodeSupport: FFmpegVideoDecodeSupport = .supported
    public private(set) var rotation: Int16 = 0
    public var dovi: DOVIDecoderConfigurationRecord?
    public private(set) var hasHDR10PlusMetadata = false
    public let fieldOrder: FFmpegFieldOrder
    public let formatDescription: CMFormatDescription?
    public private(set) var panoramaProjection: VideoProjection?
    var closedCaptionsTrack: FFmpegAssetTrack?
    let isConvertNALSize: Bool
    var seekByBytes = false
    public var subtitleKind: SubtitleKind {
        Self.subtitleKind(codecID: codecpar.codec_id, isImageSubtitle: isImageSubtitle)
    }

    public var description: String {
        var description = audioCodecMetadata?.displayName ?? codecName
        if let formatName {
            description += ", \(formatName)"
        }
        if bitsPerRawSample > 0 {
            description += "(\(bitsPerRawSample.kmFormatted) bit)"
        }
        if let audioDescriptor {
            description += ", \(audioDescriptor.sampleRate)Hz"
            description += ", \(audioDescriptor.channel.description)"
        }
        if let formatDescription {
            if mediaType == .video {
                let naturalSize = formatDescription.naturalSize
                description += ", \(Int(naturalSize.width))x\(Int(naturalSize.height))"
                description += String(format: ", %.2f fps", nominalFrameRate)
                if let dovi {
                    description += ", \(dovi.description)"
                } else if let dynamicRangeLabel {
                    description += ", \(dynamicRangeLabel)"
                }
            }
        }
        if bitRate > 0 {
            description += ", \(bitRate.kmFormatted)bps"
        }
        if let language {
            description += "(\(language))"
        }
        if case let .unsupported(reason) = audioDecodeSupport {
            description += ", unsupported (\(reason))"
        }
        if case let .unsupported(reason) = videoDecodeSupport {
            description += ", unsupported (\(reason))"
        }
        return description
    }

    convenience init?(stream: UnsafeMutablePointer<AVStream>) {
        let codecpar = stream.pointee.codecpar.pointee
        self.init(codecpar: codecpar)
        self.stream = stream
        let metadata = toDictionary(stream.pointee.metadata)
        if let value = metadata["variant_bitrate"] ?? metadata["BPS"], let bitRate = Int64(value) {
            self.bitRate = bitRate
        }
        trackID = stream.pointee.index
        var timebase = Timebase(stream.pointee.time_base)
        if timebase.num <= 0 || timebase.den <= 0 {
            timebase = Timebase(num: 1, den: 1000)
        }
        if stream.pointee.start_time != Int64.min {
            startTime = timebase.cmtime(for: stream.pointee.start_time)
        }
        self.timebase = timebase
        avgFrameRate = Timebase(stream.pointee.avg_frame_rate)
        realFrameRate = Timebase(stream.pointee.r_frame_rate)
        if mediaType == .audio {
            var frameSize = codecpar.frame_size
            if frameSize < 1 {
                frameSize = timebase.den / timebase.num
            }
            nominalFrameRate = max(Float(codecpar.sample_rate / frameSize), 48)
        } else {
            if stream.pointee.duration > 0, stream.pointee.nb_frames > 0, stream.pointee.nb_frames != stream.pointee.duration {
                nominalFrameRate = Float(stream.pointee.nb_frames) * Float(timebase.den) / Float(stream.pointee.duration) * Float(timebase.num)
            } else if avgFrameRate.den > 0, avgFrameRate.num > 0 {
                nominalFrameRate = Float(avgFrameRate.num) / Float(avgFrameRate.den)
            } else {
                nominalFrameRate = 24
            }
        }

        if let value = metadata["language"], value != "und" {
            languageCode = value
        } else {
            languageCode = nil
        }
        if let value = metadata["title"] {
            name = value
        } else {
            name = languageCode ?? codecName
        }
        if mediaType == .video, panoramaProjection == nil {
            panoramaProjection = PanoramaProjectionPolicy.detectedProjection(metadata: metadata)
        }
        updateAudioCodecMetadata(title: name)
        // AV_DISPOSITION_DEFAULT
        if mediaType == .subtitle {
            isEnabled = !isImageSubtitle || stream.pointee.disposition & AV_DISPOSITION_FORCED == AV_DISPOSITION_FORCED
            if stream.pointee.disposition & AV_DISPOSITION_HEARING_IMPAIRED == AV_DISPOSITION_HEARING_IMPAIRED {
                name += "(hearing impaired)"
            }
        }
        //        var buf = [Int8](repeating: 0, count: 256)
        //        avcodec_string(&buf, buf.count, codecpar, 0)
    }

    init?(codecpar: AVCodecParameters) {
        self.codecpar = codecpar
        bitRate = codecpar.bit_rate
        // codec_tag byte order is LSB first CMFormatDescription.MediaSubType(rawValue: codecpar.codec_tag.bigEndian)
        let codecType = codecpar.mediaSubType
        var codecName = ""
        let profileName = avcodec_profile_name(codecpar.codec_id, codecpar.profile).flatMap { String(cString: $0) }
        if let descriptor = avcodec_descriptor_get(codecpar.codec_id) {
            codecName += String(cString: descriptor.pointee.name)
            if let profileName {
                codecName += " (\(profileName))"
            }
        } else {
            codecName = ""
        }
        self.codecName = codecName
        fieldOrder = FFmpegFieldOrder(rawValue: UInt8(codecpar.field_order.rawValue)) ?? .unknown
        var formatDescriptionOut: CMFormatDescription?
        if codecpar.codec_type == AVMEDIA_TYPE_AUDIO {
            mediaType = .audio
            audioDescriptor = AudioDescriptor(codecpar: codecpar)
            isConvertNALSize = false
            bitDepth = 0
            let layout = codecpar.ch_layout
            let channelsPerFrame = UInt32(layout.nb_channels)
            let sampleFormat = AVSampleFormat(codecpar.format)
            let bytesPerSample = UInt32(av_get_bytes_per_sample(sampleFormat))
            let formatFlags = ((sampleFormat == AV_SAMPLE_FMT_FLT || sampleFormat == AV_SAMPLE_FMT_DBL) ? kAudioFormatFlagIsFloat : sampleFormat == AV_SAMPLE_FMT_U8 ? 0 : kAudioFormatFlagIsSignedInteger) | kAudioFormatFlagIsPacked
            var audioStreamBasicDescription = AudioStreamBasicDescription(mSampleRate: Float64(codecpar.sample_rate), mFormatID: codecType.rawValue, mFormatFlags: formatFlags, mBytesPerPacket: bytesPerSample * channelsPerFrame, mFramesPerPacket: 1, mBytesPerFrame: bytesPerSample * channelsPerFrame, mChannelsPerFrame: channelsPerFrame, mBitsPerChannel: bytesPerSample * 8, mReserved: 0)
            _ = layout.withCoreAudioChannelLayout { layoutSize, layout in
                CMAudioFormatDescriptionCreate(allocator: kCFAllocatorDefault, asbd: &audioStreamBasicDescription, layoutSize: layoutSize, layout: layout, magicCookieSize: 0, magicCookie: nil, extensions: nil, formatDescriptionOut: &formatDescriptionOut)
            }
            if let name = av_get_sample_fmt_name(sampleFormat) {
                formatName = String(cString: name)
            } else {
                formatName = nil
            }
        } else if codecpar.codec_type == AVMEDIA_TYPE_VIDEO {
            audioDescriptor = nil
            mediaType = .video
            videoDecodeSupport = Self.videoDecodeSupport(codecID: codecpar.codec_id)
            var doviRecord: DOVIDecoderConfigurationRecord?
            if codecpar.nb_coded_side_data > 0, let sideDatas = codecpar.coded_side_data {
                for i in 0 ..< codecpar.nb_coded_side_data {
                    let sideData = sideDatas[Int(i)]
                    if sideData.type == AV_PKT_DATA_DOVI_CONF {
                        doviRecord = sideData.data.withMemoryRebound(to: DOVIDecoderConfigurationRecord.self, capacity: 1) { $0 }.pointee
                    } else if sideData.type == AV_PKT_DATA_DYNAMIC_HDR10_PLUS {
                        hasHDR10PlusMetadata = true
                    } else if sideData.type == AV_PKT_DATA_DISPLAYMATRIX {
                        let matrix = sideData.data.withMemoryRebound(to: Int32.self, capacity: 1) { $0 }
                        let rawRotation = -av_display_rotation_get(matrix)
                        if rawRotation.isFinite {
                            let degrees = Int(rawRotation.rounded())
                            let normalized = ((degrees % 360) + 360) % 360
                            rotation = Int16(normalized)
                        } else {
                            rotation = 0
                        }                        
                    } else if sideData.type == AV_PKT_DATA_SPHERICAL {
                        panoramaProjection = Self.sphericalProjection(data: sideData.data, size: Int32(clamping: sideData.size))
                    }
                }
            }
            let sar = codecpar.sample_aspect_ratio.size
            var extradataSize = Int32(0)
            var extradata = codecpar.extradata
            let atomsData: Data?
            if let extradata {
                extradataSize = codecpar.extradata_size
                if extradataSize >= 5, extradata[4] == 0xFE {
                    extradata[4] = 0xFF
                    isConvertNALSize = true
                } else {
                    isConvertNALSize = false
                }
                atomsData = Data(bytes: extradata, count: Int(extradataSize))
            } else {
                if codecType.rawValue == kCMVideoCodecType_VP9 {
                    // ff_videotoolbox_vpcc_extradata_create
                    var ioContext: UnsafeMutablePointer<AVIOContext>?
                    guard avio_open_dyn_buf(&ioContext) == 0 else {
                        return nil
                    }
                    ff_isom_write_vpcc(nil, ioContext, nil, 0, &self.codecpar)
                    extradataSize = avio_close_dyn_buf(ioContext, &extradata)
                    guard let extradata else {
                        return nil
                    }
                    var data = Data()
                    var array: [UInt8] = [1, 0, 0, 0]
                    data.append(&array, count: 4)
                    data.append(extradata, count: Int(extradataSize))
                    atomsData = data
                } else {
                    atomsData = nil
                }
                isConvertNALSize = false
            }
            dovi = doviRecord
            let format = AVPixelFormat(rawValue: codecpar.format)
            bitDepth = format.bitDepth
            let fullRange = codecpar.color_range == AVCOL_RANGE_JPEG
            let dolbyVisionHDRFallback = doviRecord?.hdrFallbackDynamicRange
            let dic: NSMutableDictionary = [
                kCVImageBufferChromaLocationBottomFieldKey: kCVImageBufferChromaLocation_Left,
                kCVImageBufferChromaLocationTopFieldKey: kCVImageBufferChromaLocation_Left,
                kCMFormatDescriptionExtension_Depth: format.bitDepth * Int32(format.planeCount),
                kCMFormatDescriptionExtension_FullRangeVideo: fullRange,
                codecType.rawValue == kCMVideoCodecType_HEVC ? "EnableHardwareAcceleratedVideoDecoder" : "RequireHardwareAcceleratedVideoDecoder": true,
            ]
            // kCMFormatDescriptionExtension_BitsPerComponent
            if let atomsData, let atomName = codecType.rawValue.sampleDescriptionExtensionAtomName {
                dic[kCMFormatDescriptionExtension_SampleDescriptionExtensionAtoms] = [atomName: atomsData]
            }
            dic[kCVPixelBufferPixelFormatTypeKey] = format.osType(fullRange: fullRange)
            dic[kCVImageBufferPixelAspectRatioKey] = sar.aspectRatio
            dic[kCVImageBufferColorPrimariesKey] = Self.colorPrimaries(codecpar: codecpar, dolbyVisionHDRFallback: dolbyVisionHDRFallback) as String?
            dic[kCVImageBufferTransferFunctionKey] = Self.transferFunction(codecpar: codecpar, dolbyVisionHDRFallback: dolbyVisionHDRFallback) as String?
            dic[kCVImageBufferYCbCrMatrixKey] = Self.yCbCrMatrix(codecpar: codecpar, dolbyVisionHDRFallback: dolbyVisionHDRFallback) as String?
            // swiftlint:disable line_length
            _ = CMVideoFormatDescriptionCreate(allocator: kCFAllocatorDefault, codecType: codecType.rawValue, width: codecpar.width, height: codecpar.height, extensions: dic, formatDescriptionOut: &formatDescriptionOut)
            // swiftlint:enable line_length
            if let name = av_get_pix_fmt_name(format) {
                formatName = String(cString: name)
            } else {
                formatName = nil
            }
        } else if codecpar.codec_type == AVMEDIA_TYPE_SUBTITLE {
            mediaType = .subtitle
            audioDescriptor = nil
            formatName = nil
            bitDepth = 0
            isConvertNALSize = false
            _ = CMFormatDescriptionCreate(allocator: kCFAllocatorDefault, mediaType: kCMMediaType_Subtitle, mediaSubType: codecType.rawValue, extensions: nil, formatDescriptionOut: &formatDescriptionOut)
        } else {
            bitDepth = 0
            return nil
        }
        formatDescription = formatDescriptionOut
        bitsPerRawSample = codecpar.bits_per_raw_sample
        isImageSubtitle = [AV_CODEC_ID_DVD_SUBTITLE, AV_CODEC_ID_DVB_SUBTITLE, AV_CODEC_ID_DVB_TELETEXT, AV_CODEC_ID_HDMV_PGS_SUBTITLE].contains(codecpar.codec_id)
        trackID = 0
        if mediaType == .audio {
            updateAudioCodecMetadata(profileName: profileName, title: nil)
        }
    }

    private func updateAudioCodecMetadata(profileName: String? = nil, title: String?) {
        guard mediaType == .audio else {
            return
        }
        let profileName = profileName ?? audioCodecMetadata?.profileName
        audioDecodeSupport = Self.audioDecodeSupport(codecID: codecpar.codec_id)
        audioCodecMetadata = Self.audioCodecMetadata(
            codecID: codecpar.codec_id,
            profile: codecpar.profile,
            codecName: codecName,
            profileName: profileName,
            channelLayout: codecpar.ch_layout,
            title: title,
            decodeSupport: audioDecodeSupport
        )
    }

    private static func sphericalProjection(data: UnsafeMutablePointer<UInt8>?, size: Int32) -> VideoProjection? {
        guard let data, size >= Int32(MemoryLayout<Int32>.size) else {
            return .equirectangular
        }
        let rawProjection = data.withMemoryRebound(to: Int32.self, capacity: 1) { $0.pointee }
        switch rawProjection {
        case 0:
            return .equirectangular
        case 1:
            return .cubemap
        case 2:
            return .equirectangularTiled
        default:
            return .unknown("ffmpeg-spherical-\(rawProjection)")
        }
    }

    func createContext(options: KSOptions) throws -> UnsafeMutablePointer<AVCodecContext> {
        try codecpar.createContext(options: options)
    }

    func configureAsClosedCaptionsTrack(source: FFmpegAssetTrack) {
        trackID = -(source.trackID + 1)
        name = NSLocalizedString("Closed Captions", comment: "Closed caption track name")
        startTime = source.startTime
        timebase = source.timebase
    }

    func markHDR10PlusMetadataDetected() {
        hasHDR10PlusMetadata = true
    }

    public var isEnabled: Bool {
        get {
            stream?.pointee.discard == AVDISCARD_DEFAULT
        }
        set {
            var discard = newValue ? AVDISCARD_DEFAULT : AVDISCARD_ALL
            if mediaType == .subtitle, !isImageSubtitle {
                discard = AVDISCARD_DEFAULT
            }
            stream?.pointee.discard = discard
        }
    }
}

extension AVChannelLayout {
    fileprivate func withCoreAudioChannelLayout<T>(_ body: (Int, UnsafePointer<AudioChannelLayout>?) -> T) -> T {
        guard let tag = layoutTag, let layout = AVAudioChannelLayout(layoutTag: tag) else {
            return body(0, nil)
        }
        return body(MemoryLayout<AudioChannelLayout>.size, layout.layout)
    }
}

extension FFmpegAssetTrack {
    static func subtitleKind(codecID: AVCodecID, isImageSubtitle: Bool) -> SubtitleKind {
        if codecID == AV_CODEC_ID_EIA_608 {
            return .closedCaption
        }
        if isImageSubtitle {
            return .image
        }
        switch codecID {
        case AV_CODEC_ID_ASS, AV_CODEC_ID_SSA, AV_CODEC_ID_SUBRIP, AV_CODEC_ID_WEBVTT, AV_CODEC_ID_MOV_TEXT:
            return .text
        default:
            return .unknown
        }
    }

    static func decodableAudioTracks(_ tracks: [FFmpegAssetTrack]) -> [FFmpegAssetTrack] {
        tracks.filter { $0.mediaType == .audio && $0.audioDecodeSupport.isSupported }
    }

    static func decodableVideoTracks(_ tracks: [FFmpegAssetTrack]) -> [FFmpegAssetTrack] {
        tracks.filter { $0.mediaType == .video && $0.videoDecodeSupport.isSupported }
    }

    static func audioDecodeSupport(codecID: AVCodecID) -> FFmpegAudioDecodeSupport {
        if avcodec_find_decoder(codecID) != nil {
            return .supported
        }
        if codecID == AV_CODEC_ID_AC4 {
            return .unsupported("AC-4 demuxing is available, but FFmpeg does not provide an AC-4 decoder and KSPlayer has no encoded AC-4 passthrough path")
        }
        let name = avcodec_descriptor_get(codecID).flatMap { String(cString: $0.pointee.name) } ?? "audio"
        return .unsupported("no FFmpeg decoder is available for \(name)")
    }

    static func videoDecodeSupport(codecID: AVCodecID) -> FFmpegVideoDecodeSupport {
        if avcodec_find_decoder(codecID) != nil {
            return .supported
        }
        let name = avcodec_descriptor_get(codecID).flatMap { String(cString: $0.pointee.name) } ?? "video"
        return .unsupported("no FFmpeg decoder is available for \(name)")
    }

    static func audioCodecMetadata(
        codecID: AVCodecID,
        profile: Int32,
        codecName: String,
        profileName: String?,
        channelLayout: AVChannelLayout,
        title: String?,
        decodeSupport: FFmpegAudioDecodeSupport
    ) -> FFmpegAudioCodecMetadata? {
        let isDolbyEAC3 = codecID == AV_CODEC_ID_EAC3
        let isDolbyTrueHD = codecID == AV_CODEC_ID_TRUEHD || codecID == AV_CODEC_ID_MLP
        let isDolbyAC4 = codecID == AV_CODEC_ID_AC4
        let titleSignalsAtmos = title?.range(of: "atmos", options: [.caseInsensitive, .diacriticInsensitive]) != nil
        let profileSignalsAtmos = profile == AV_PROFILE_EAC3_DDP_ATMOS || profileName?.range(of: "atmos", options: [.caseInsensitive, .diacriticInsensitive]) != nil
        let isDolbyAtmos = (isDolbyEAC3 && profileSignalsAtmos) || ((isDolbyTrueHD || isDolbyAC4) && titleSignalsAtmos) || (isDolbyTrueHD && channelLayout.isDolbyAtmosBedLayout)
        guard isDolbyEAC3 || isDolbyTrueHD || isDolbyAC4 || isDolbyAtmos else {
            return nil
        }

        let displayName: String
        if isDolbyAC4 {
            displayName = isDolbyAtmos ? "Dolby AC-4 Atmos" : "Dolby AC-4"
        } else if isDolbyTrueHD {
            displayName = isDolbyAtmos ? "Dolby TrueHD Atmos" : "Dolby TrueHD"
        } else if isDolbyAtmos {
            displayName = "Dolby Digital Plus Atmos"
        } else {
            displayName = "Dolby Digital Plus"
        }
        return FFmpegAudioCodecMetadata(
            displayName: displayName,
            profileName: profileName,
            isDolbyAtmos: isDolbyAtmos,
            isDolbyAC4: isDolbyAC4,
            isDolbyTrueHD: isDolbyTrueHD,
            isDolbyEAC3: isDolbyEAC3,
            decodeSupport: decodeSupport
        )
    }
}

extension FFmpegAssetTrack {
    var pixelFormatType: OSType? {
        let format = AVPixelFormat(codecpar.format)
        return format.osType(fullRange: formatDescription?.fullRangeVideo ?? false)
    }

    private var dolbyVisionHDRFallback: DynamicRange? {
        dovi?.hdrFallbackDynamicRange
    }

    private var colorPrimaries: CFString? {
        Self.colorPrimaries(codecpar: codecpar, dolbyVisionHDRFallback: dolbyVisionHDRFallback)
    }

    private static func colorPrimaries(codecpar: AVCodecParameters, dolbyVisionHDRFallback: DynamicRange?) -> CFString? {
        if dolbyVisionHDRFallback != nil, codecpar.color_primaries == AVCOL_PRI_UNSPECIFIED {
            return kCVImageBufferColorPrimaries_ITU_R_2020
        }
        return codecpar.color_primaries.colorPrimaries
    }

    private var transferFunction: CFString? {
        Self.transferFunction(codecpar: codecpar, dolbyVisionHDRFallback: dolbyVisionHDRFallback)
    }

    private static func transferFunction(codecpar: AVCodecParameters, dolbyVisionHDRFallback: DynamicRange?) -> CFString? {
        if let dolbyVisionHDRFallback, codecpar.color_trc == AVCOL_TRC_UNSPECIFIED {
            return dolbyVisionHDRFallback.transferFunction
        }
        return codecpar.color_trc.transferFunction
    }

    private var yCbCrMatrix: CFString? {
        Self.yCbCrMatrix(codecpar: codecpar, dolbyVisionHDRFallback: dolbyVisionHDRFallback)
    }

    private static func yCbCrMatrix(codecpar: AVCodecParameters, dolbyVisionHDRFallback: DynamicRange?) -> CFString? {
        if dolbyVisionHDRFallback != nil, codecpar.color_space == AVCOL_SPC_UNSPECIFIED {
            return kCVImageBufferYCbCrMatrix_ITU_R_2020
        }
        return codecpar.color_space.ycbcrMatrix
    }

    var dynamicRangeLabel: String? {
        if hasHDR10PlusMetadata {
            return "HDR10+"
        }
        let range = dynamicRange
        return range == .sdr ? nil : range?.description
    }
}

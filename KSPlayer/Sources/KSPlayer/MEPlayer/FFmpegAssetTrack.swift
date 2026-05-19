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

enum FFmpegClosedCaptionFormat: Equatable {
    case cea608
    case cea708
    case cea608And708
    case unknown

    var containsCEA608: Bool {
        switch self {
        case .cea608, .cea608And708, .unknown:
            return true
        case .cea708:
            return false
        }
    }

    var displayName: String? {
        switch self {
        case .cea608:
            return "CEA-608"
        case .cea708:
            return "CEA-708"
        case .cea608And708:
            return "CEA-608/708"
        case .unknown:
            return nil
        }
    }
}

enum FFmpegClosedCaptionRouting {
    static func format(codecID: AVCodecID) -> FFmpegClosedCaptionFormat? {
        if codecID == AV_CODEC_ID_EIA_608 {
            return .cea608
        }
        guard let descriptor = avcodec_descriptor_get(codecID) else {
            return nil
        }
        let name = String(cString: descriptor.pointee.name).lowercased()
        if name.contains("708") {
            return .cea708
        }
        if name.contains("608") {
            return .cea608
        }
        return nil
    }

    static func format(a53CCSideData data: UnsafeMutablePointer<UInt8>?, size: Int) -> FFmpegClosedCaptionFormat {
        guard let data, size >= 3 else {
            return .unknown
        }
        var hasCEA608 = false
        var hasCEA708 = false
        var offset = 0
        while offset + 2 < size {
            let marker = data[offset]
            let isValid = marker & 0x04 != 0
            if isValid {
                switch marker & 0x03 {
                case 0, 1:
                    hasCEA608 = true
                case 2, 3:
                    hasCEA708 = true
                default:
                    break
                }
            }
            offset += 3
        }
        switch (hasCEA608, hasCEA708) {
        case (true, true):
            return .cea608And708
        case (true, false):
            return .cea608
        case (false, true):
            return .cea708
        case (false, false):
            return .unknown
        }
    }

    static func eia608Payload(a53CCSideData data: UnsafeMutablePointer<UInt8>?, size: Int) -> Data? {
        guard let data, size > 0 else {
            return nil
        }
        guard size >= 3 else {
            return Data(bytes: data, count: size)
        }
        var payload = Data()
        var sawValidCaptionTriplet = false
        var offset = 0
        while offset + 2 < size {
            let marker = data[offset]
            let isValid = marker & 0x04 != 0
            if isValid {
                sawValidCaptionTriplet = true
                let type = marker & 0x03
                if type == 0 || type == 1 {
                    payload.append(data.advanced(by: offset), count: 3)
                }
            }
            offset += 3
        }
        if !sawValidCaptionTriplet {
            return Data(bytes: data, count: size)
        }
        return payload.isEmpty ? nil : payload
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
    private var streamlessIsEnabled = false
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
    private var closedCaptionFormat: FFmpegClosedCaptionFormat = .unknown
    private var hasLoggedUnsupportedCEA708ClosedCaptions = false
    // video
    public private(set) var videoDecodeSupport: FFmpegVideoDecodeSupport = .supported
    public private(set) var rotation: Int16 = 0
    public var dovi: DOVIDecoderConfigurationRecord?
    private let dolbyVisionDiagnosticsLock = NSLock()
    private var _dolbyVisionPlaybackDiagnostic: DolbyVisionPlaybackDiagnostic?
    public var dolbyVisionPlaybackDiagnostic: DolbyVisionPlaybackDiagnostic? {
        dolbyVisionDiagnosticsLock.lock()
        let diagnostic = _dolbyVisionPlaybackDiagnostic
        dolbyVisionDiagnosticsLock.unlock()
        return diagnostic
    }

    public private(set) var hasHDR10PlusMetadata = false
    public private(set) var hdr10PlusMetadata: HDR10PlusMetadata?
    public let fieldOrder: FFmpegFieldOrder
    public let formatDescription: CMFormatDescription?
    public private(set) var stereoscopicVideoLayout: StereoscopicVideoLayout?
    public private(set) var panoramaConfiguration: PanoramaVideoConfiguration?
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
        if mediaType == .video, let metadataConfiguration = PanoramaProjectionPolicy.detectedConfiguration(metadata: metadata) {
            setPanoramaConfiguration(panoramaConfiguration?.merging(metadataConfiguration) ?? metadataConfiguration)
        }
        if mediaType == .video, stereoscopicVideoLayout == nil {
            stereoscopicVideoLayout = StereoscopicVideoPolicy.detectedLayout(metadata: metadata)
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
            var sphericalProjection: VideoProjection?
            var stereoLayout: StereoscopicVideoLayout?
            if codecpar.nb_coded_side_data > 0, let sideDatas = codecpar.coded_side_data {
                for i in 0 ..< codecpar.nb_coded_side_data {
                    let sideData = sideDatas[Int(i)]
                    if sideData.type == AV_PKT_DATA_DOVI_CONF {
                        doviRecord = sideData.data.withMemoryRebound(to: DOVIDecoderConfigurationRecord.self, capacity: 1) { $0 }.pointee
                    } else if sideData.type == AV_PKT_DATA_DYNAMIC_HDR10_PLUS {
                        hasHDR10PlusMetadata = true
                        hdr10PlusMetadata = Self.hdr10PlusMetadata(data: sideData.data, size: sideData.size, width: codecpar.width, height: codecpar.height)
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
                        sphericalProjection = Self.sphericalProjection(data: sideData.data, size: Int32(clamping: sideData.size))
                    } else if sideData.type == AV_PKT_DATA_STEREO3D {
                        stereoLayout = Self.stereoLayout(data: sideData.data, size: Int32(clamping: sideData.size))
                    }
                }
            }
            stereoscopicVideoLayout = stereoLayout
            if let sphericalProjection {
                panoramaConfiguration = PanoramaVideoConfiguration(
                    projection: sphericalProjection,
                    stereoLayout: stereoLayout ?? .mono
                )
                panoramaProjection = sphericalProjection
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
            _dolbyVisionPlaybackDiagnostic = doviRecord.map { DolbyVisionPlaybackDiagnostic(configuration: $0) }
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
            if let fieldCount = fieldOrder.coreMediaFieldCount {
                dic[kCMFormatDescriptionExtension_FieldCount] = fieldCount
            }
            if let fieldDetail = fieldOrder.coreMediaFieldDetail {
                dic[kCMFormatDescriptionExtension_FieldDetail] = fieldDetail
            }
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
        let rawProjection = UnsafeRawBufferPointer(start: data, count: Int(size)).loadUnaligned(as: Int32.self)
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

    private static func stereoLayout(data: UnsafeMutablePointer<UInt8>?, size: Int32) -> StereoscopicVideoLayout? {
        guard let data, size >= Int32(MemoryLayout<Int32>.size) else {
            return nil
        }
        let rawStereoType = UnsafeRawBufferPointer(start: data, count: Int(size)).loadUnaligned(as: Int32.self)
        switch rawStereoType {
        case 1, 5:
            return .sideBySide
        case 2:
            return .topAndBottom
        default:
            return nil
        }
    }

    private func setPanoramaConfiguration(_ configuration: PanoramaVideoConfiguration?) {
        panoramaConfiguration = configuration
        panoramaProjection = configuration?.projection
    }

    func createContext(options: KSOptions) throws -> UnsafeMutablePointer<AVCodecContext> {
        try codecpar.createContext(options: options)
    }

    func configureAsClosedCaptionsTrack(source: FFmpegAssetTrack, format: FFmpegClosedCaptionFormat = .unknown) {
        trackID = -(source.trackID + 1)
        updateClosedCaptionFormat(format)
        startTime = source.startTime
        timebase = source.timebase
        streamlessIsEnabled = false
    }

    func updateClosedCaptionFormat(_ format: FFmpegClosedCaptionFormat) {
        if format == .unknown, closedCaptionFormat != .unknown {
            return
        }
        closedCaptionFormat = format
        let baseName = NSLocalizedString("Closed Captions", comment: "Closed caption track name")
        if let displayName = format.displayName {
            name = "\(baseName) (\(displayName))"
        } else {
            name = baseName
        }
    }

    func recordUnsupportedClosedCaptionFormat(_ format: FFmpegClosedCaptionFormat) {
        guard format == .cea708, !hasLoggedUnsupportedCEA708ClosedCaptions else {
            return
        }
        hasLoggedUnsupportedCEA708ClosedCaptions = true
        KSLog("[subtitle] CEA-708 captions detected in A53 side data; FFmpeg EIA-608 decoder routing is skipped because KSPlayer does not have a CEA-708 decoder for this path")
    }

    func markHDR10PlusMetadataDetected() {
        hasHDR10PlusMetadata = true
    }

    func markHDR10PlusMetadataDetected(_ metadata: HDR10PlusMetadata?) {
        hasHDR10PlusMetadata = true
        if let metadata {
            hdr10PlusMetadata = metadata
        }
    }

    func recordDolbyVisionPacket(
        _ packet: Packet,
        compositorAvailability: DolbyVisionFELCompositorAvailability = .unavailable(
            reason: DolbyVisionPlaybackDiagnostic.missingOpenFELCompositorReason
        ),
        playbackPolicy: DolbyVisionFELPlaybackPolicy = .allowBaseLayerFallback
    ) {
        guard let dovi,
              dovi.dv_profile == 7,
              let corePacket = packet.corePacket?.pointee,
              let data = corePacket.data,
              corePacket.size > 0
        else {
            return
        }
        let split = DolbyVisionHEVCSampleInspector.split(
            data: UnsafePointer(data),
            size: Int(corePacket.size),
            nalLengthSize: isConvertNALSize ? 3 : 4
        )
        let diagnostics = split.diagnostics
        guard diagnostics.hasDolbyVisionSignals else {
            return
        }
        mergeDolbyVisionDiagnostic { current in
            current.merging(
                observedRPUNALUnitCount: diagnostics.rpuNALUnitCount,
                observedEnhancementLayerNALUnitCount: diagnostics.enhancementLayerNALUnitCount,
                observedEnhancementLayerVCLNALUnitCount: diagnostics.enhancementLayerVCLNALUnitCount,
                largestEnhancementLayerVCLPayloadSize: diagnostics.largestEnhancementLayerVCLPayloadSize,
                rawRPUData: diagnostics.rawRPUData,
                felCompositionState: DolbyVisionFELCompositionPlanner.state(
                    configuration: dovi,
                    split: split,
                    enhancementLayerKind: current.enhancementLayerKind,
                    compositorAvailability: compositorAvailability,
                    playbackPolicy: playbackPolicy
                ),
                felPlaybackPolicy: playbackPolicy
            )
        }
    }

    func recordDolbyVisionRPUBuffer(_ data: Data) {
        mergeDolbyVisionDiagnostic { current in
            current.merging(rawRPUData: data, hasFrameRPUBuffer: true)
        }
    }

    func recordDolbyVisionFrameMetadata(
        enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind?,
        playbackPolicy: DolbyVisionFELPlaybackPolicy = .allowBaseLayerFallback
    ) {
        mergeDolbyVisionDiagnostic { current in
            current.merging(
                hasFrameDOVIMetadata: true,
                enhancementLayerKind: enhancementLayerKind,
                felPlaybackPolicy: playbackPolicy
            )
        }
    }

    private func updateDolbyVisionDiagnostic(configuration: DOVIDecoderConfigurationRecord?) {
        dolbyVisionDiagnosticsLock.lock()
        _dolbyVisionPlaybackDiagnostic = configuration.map { DolbyVisionPlaybackDiagnostic(configuration: $0) }
        dolbyVisionDiagnosticsLock.unlock()
    }

    private func mergeDolbyVisionDiagnostic(_ update: (DolbyVisionPlaybackDiagnostic) -> DolbyVisionPlaybackDiagnostic) {
        dolbyVisionDiagnosticsLock.lock()
        guard let current = _dolbyVisionPlaybackDiagnostic ?? dovi.map({ DolbyVisionPlaybackDiagnostic(configuration: $0) }) else {
            dolbyVisionDiagnosticsLock.unlock()
            return
        }
        _dolbyVisionPlaybackDiagnostic = update(current)
        dolbyVisionDiagnosticsLock.unlock()
    }

    public var isEnabled: Bool {
        get {
            guard let stream else {
                return streamlessIsEnabled
            }
            return stream.pointee.discard == AVDISCARD_DEFAULT
        }
        set {
            guard let stream else {
                streamlessIsEnabled = newValue
                return
            }
            var discard = newValue ? AVDISCARD_DEFAULT : AVDISCARD_ALL
            if mediaType == .subtitle, !isImageSubtitle {
                discard = AVDISCARD_DEFAULT
            }
            stream.pointee.discard = discard
        }
    }
}

extension FFmpegAssetTrack {
    static func hdr10PlusMetadata(data: UnsafeMutablePointer<UInt8>?, size: Int, width: Int32, height: Int32) -> HDR10PlusMetadata? {
        guard let data, size >= MemoryLayout<AVDynamicHDRPlus>.stride else {
            return nil
        }
        let rawSideData = Data(bytes: data, count: size)
        return data.withMemoryRebound(to: AVDynamicHDRPlus.self, capacity: 1) {
            HDR10PlusMetadata(dynamicHDRPlus: $0.pointee, width: width, height: height, rawSideData: rawSideData)
        }
    }
}

struct DolbyVisionHEVCSampleDiagnostics: Equatable {
    var rpuNALUnitCount = 0
    var enhancementLayerNALUnitCount = 0
    var enhancementLayerVCLNALUnitCount = 0
    var largestEnhancementLayerVCLPayloadSize = 0
    var rawRPUData: Data?
    var hasBaseLayerVCLNALUnits = false
    var baseLayerSampleSize = 0
    var enhancementLayerSampleSize = 0

    var hasDolbyVisionSignals: Bool {
        rpuNALUnitCount > 0 || enhancementLayerNALUnitCount > 0 || enhancementLayerVCLNALUnitCount > 0 || rawRPUData != nil
    }

    var hasSeparatedFELInputs: Bool {
        hasBaseLayerVCLNALUnits && enhancementLayerVCLNALUnitCount > 0 && (rpuNALUnitCount > 0 || rawRPUData != nil)
    }
}

struct DolbyVisionHEVCNALUnit: Equatable {
    let type: UInt8
    let layerID: UInt8
    let data: Data

    var isVCL: Bool {
        type <= 31
    }

    var isRPU: Bool {
        type == 62
    }

    var payloadSize: Int {
        max(data.count - 2, 0)
    }

    init?(data: UnsafePointer<UInt8>, size: Int) {
        guard size >= 2 else {
            return nil
        }
        type = (data[0] >> 1) & 0x3f
        layerID = ((data[0] & 0x01) << 5) | ((data[1] >> 3) & 0x1f)
        self.data = Data(bytes: data, count: size)
    }
}

struct DolbyVisionHEVCSampleSplit: Equatable {
    enum Storage: Equatable {
        case annexB
        case lengthPrefixed(Int)
    }

    let storage: Storage
    private(set) var baseLayerSample = Data()
    private(set) var enhancementLayerSample = Data()
    private(set) var rpuNALUnits = [Data]()
    private(set) var diagnostics = DolbyVisionHEVCSampleDiagnostics()

    var hasEnhancementLayerSample: Bool {
        !enhancementLayerSample.isEmpty
    }

    var hasDecodableBaseLayerSample: Bool {
        diagnostics.hasBaseLayerVCLNALUnits && !baseLayerSample.isEmpty
    }

    var hasSeparatedFELInputs: Bool {
        diagnostics.hasSeparatedFELInputs
    }

    mutating func append(_ nalUnit: DolbyVisionHEVCNALUnit) {
        if nalUnit.isRPU {
            diagnostics.rpuNALUnitCount += 1
            diagnostics.rawRPUData = nalUnit.data
            rpuNALUnits.append(nalUnit.data)
            return
        }

        if nalUnit.layerID > 0 {
            diagnostics.enhancementLayerNALUnitCount += 1
            if nalUnit.isVCL {
                diagnostics.enhancementLayerVCLNALUnitCount += 1
                diagnostics.largestEnhancementLayerVCLPayloadSize = max(
                    diagnostics.largestEnhancementLayerVCLPayloadSize,
                    nalUnit.payloadSize
                )
            }
            Self.append(nalUnit.data, to: &enhancementLayerSample, storage: storage)
            diagnostics.enhancementLayerSampleSize = enhancementLayerSample.count
        } else {
            if nalUnit.isVCL {
                diagnostics.hasBaseLayerVCLNALUnits = true
            }
            Self.append(nalUnit.data, to: &baseLayerSample, storage: storage)
            diagnostics.baseLayerSampleSize = baseLayerSample.count
        }
    }

    private static func append(_ nalUnit: Data, to sample: inout Data, storage: Storage) {
        switch storage {
        case .annexB:
            sample.append(contentsOf: [0x00, 0x00, 0x00, 0x01])
        case let .lengthPrefixed(nalLengthSize):
            var length = UInt32(nalUnit.count).bigEndian
            withUnsafeBytes(of: &length) { bytes in
                sample.append(contentsOf: bytes.suffix(nalLengthSize))
            }
        }
        sample.append(nalUnit)
    }
}

enum DolbyVisionFELCompositionPlanner {
    static func state(
        configuration: DOVIDecoderConfigurationRecord?,
        split: DolbyVisionHEVCSampleSplit,
        enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind,
        compositorAvailability: DolbyVisionFELCompositorAvailability = .unavailable(
            reason: DolbyVisionPlaybackDiagnostic.missingOpenFELCompositorReason
        ),
        playbackPolicy: DolbyVisionFELPlaybackPolicy = .allowBaseLayerFallback
    ) -> DolbyVisionFELCompositionState {
        guard configuration?.dv_profile == 7, configuration?.el_present_flag != 0 else {
            return .notRequired
        }
        if enhancementLayerKind == .minimumEnhancementLayer {
            return .notRequired
        }
        if case let .available(backend) = compositorAvailability, split.hasSeparatedFELInputs {
            return .available(backend: backend)
        }
        if playbackPolicy == .requireFullComposition, split.hasSeparatedFELInputs {
            return .requiredUnavailable(reason: DolbyVisionPlaybackDiagnostic.requiredFELCompositorReason)
        }
        if enhancementLayerKind == .fullEnhancementLayer {
            return .unavailable(reason: DolbyVisionPlaybackDiagnostic.missingOpenFELCompositorReason)
        }
        if split.hasSeparatedFELInputs {
            return .separatedInputsAvailable
        }
        return .awaitingMetadata
    }
}

struct DolbyVisionFELFrameSample: Equatable {
    let timestamp: Int64
    let duration: Int64
    let baseLayerSample: Data
    let enhancementLayerSample: Data
    let rpuNALUnits: [Data]
}

struct DolbyVisionFELFrameAlignmentQueue: Equatable {
    private var pendingEnhancementSamples = [Int64: DolbyVisionHEVCSampleSplit]()

    mutating func enqueueEnhancement(timestamp: Int64, split: DolbyVisionHEVCSampleSplit) {
        guard split.hasEnhancementLayerSample || !split.rpuNALUnits.isEmpty else {
            return
        }
        pendingEnhancementSamples[timestamp] = split
    }

    mutating func alignBase(
        timestamp: Int64,
        duration: Int64,
        split: DolbyVisionHEVCSampleSplit
    ) -> DolbyVisionFELFrameSample? {
        guard split.hasDecodableBaseLayerSample else {
            return nil
        }
        let enhancement = pendingEnhancementSamples.removeValue(forKey: timestamp) ?? split
        guard enhancement.hasEnhancementLayerSample || !enhancement.rpuNALUnits.isEmpty else {
            return nil
        }
        return DolbyVisionFELFrameSample(
            timestamp: timestamp,
            duration: duration,
            baseLayerSample: split.baseLayerSample,
            enhancementLayerSample: enhancement.enhancementLayerSample,
            rpuNALUnits: split.rpuNALUnits + enhancement.rpuNALUnits
        )
    }
}

enum DolbyVisionHEVCSampleInspector {
    static func inspect(data: UnsafePointer<UInt8>, size: Int, nalLengthSize: Int = 4) -> DolbyVisionHEVCSampleDiagnostics {
        split(data: data, size: size, nalLengthSize: nalLengthSize).diagnostics
    }

    static func split(data: UnsafePointer<UInt8>, size: Int, nalLengthSize: Int = 4) -> DolbyVisionHEVCSampleSplit {
        guard size > 0 else {
            return DolbyVisionHEVCSampleSplit(storage: .lengthPrefixed(nalLengthSize))
        }
        if VideoToolboxSampleData.isAnnexB(data: data, size: size) {
            return splitAnnexB(data: data, size: size)
        }
        return splitLengthPrefixed(data: data, size: size, nalLengthSize: nalLengthSize)
    }

    private static func splitAnnexB(data: UnsafePointer<UInt8>, size: Int) -> DolbyVisionHEVCSampleSplit {
        var split = DolbyVisionHEVCSampleSplit(storage: .annexB)
        var current = findStartCode(in: data, size: size, from: 0)
        while let startCode = current {
            let nalStart = startCode.offset + startCode.length
            current = findStartCode(in: data, size: size, from: nalStart)
            var nalEnd = current?.offset ?? size
            while nalEnd > nalStart, data[nalEnd - 1] == 0 {
                nalEnd -= 1
            }
            if nalEnd > nalStart, let nalUnit = DolbyVisionHEVCNALUnit(data: data.advanced(by: nalStart), size: nalEnd - nalStart) {
                split.append(nalUnit)
            }
        }
        return split
    }

    private static func splitLengthPrefixed(data: UnsafePointer<UInt8>, size: Int, nalLengthSize: Int) -> DolbyVisionHEVCSampleSplit {
        let nalLengthSize = min(max(nalLengthSize, 1), 4)
        var split = DolbyVisionHEVCSampleSplit(storage: .lengthPrefixed(nalLengthSize))
        var offset = 0
        while offset + nalLengthSize <= size {
            var nalSize = 0
            for i in 0 ..< nalLengthSize {
                nalSize = (nalSize << 8) | Int(data[offset + i])
            }
            offset += nalLengthSize
            guard nalSize > 0, offset + nalSize <= size else {
                return split
            }
            if let nalUnit = DolbyVisionHEVCNALUnit(data: data.advanced(by: offset), size: nalSize) {
                split.append(nalUnit)
            }
            offset += nalSize
        }
        return split
    }

    private static func findStartCode(in data: UnsafePointer<UInt8>, size: Int, from offset: Int) -> (offset: Int, length: Int)? {
        guard size >= 3, offset < size else {
            return nil
        }
        var index = offset
        while index + 3 <= size {
            if data[index] == 0, data[index + 1] == 0 {
                if data[index + 2] == 1 {
                    return (index, 3)
                }
                if index + 4 <= size, data[index + 2] == 0, data[index + 3] == 1 {
                    return (index, 4)
                }
            }
            index += 1
        }
        return nil
    }
}

extension AVChannelLayout {
    fileprivate func withCoreAudioChannelLayout<T>(_ body: (Int, UnsafePointer<AudioChannelLayout>?) -> T) -> T {
        guard let tag = layoutTag, let layout = AVAudioChannelLayout(layoutTag: tag) else {
            return body(0, nil)
        }
        return body(Int(layout.layout.byteSize), layout.layout)
    }
}

extension FFmpegAssetTrack {
    static func subtitleKind(codecID: AVCodecID, isImageSubtitle: Bool) -> SubtitleKind {
        if FFmpegClosedCaptionRouting.format(codecID: codecID) != nil {
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
            return HDR10PlusPlaybackDiagnostic.metadataOnlyDynamicRangeDescription
        }
        let range = dynamicRange
        return range == .sdr ? nil : range?.description
    }
}

extension HDR10PlusMetadata {
    init?(dynamicHDRPlus metadata: AVDynamicHDRPlus, width: Int32, height: Int32, rawSideData: Data?) {
        let windows = Self.windows(from: metadata, width: width, height: height)
        guard !windows.isEmpty else {
            return nil
        }
        self.init(
            applicationVersion: metadata.application_version,
            targetedSystemDisplayMaximumLuminance: Self.float(metadata.targeted_system_display_maximum_luminance),
            processingWindows: windows,
            targetedSystemDisplayActualPeakLuminance: Self.luminanceGrid(
                flag: metadata.targeted_system_display_actual_peak_luminance_flag,
                rows: metadata.num_rows_targeted_system_display_actual_peak_luminance,
                columns: metadata.num_cols_targeted_system_display_actual_peak_luminance,
                grid: metadata.targeted_system_display_actual_peak_luminance
            ),
            masteringDisplayActualPeakLuminance: Self.luminanceGrid(
                flag: metadata.mastering_display_actual_peak_luminance_flag,
                rows: metadata.num_rows_mastering_display_actual_peak_luminance,
                columns: metadata.num_cols_mastering_display_actual_peak_luminance,
                grid: metadata.mastering_display_actual_peak_luminance
            ),
            rawSideData: rawSideData
        )
    }

    private static func windows(from metadata: AVDynamicHDRPlus, width: Int32, height: Int32) -> [ProcessingWindow] {
        let params = [metadata.params.0, metadata.params.1, metadata.params.2]
        let windowCount = min(max(Int(metadata.num_windows), 0), params.count)
        return params.prefix(windowCount).enumerated().map { index, params in
            window(from: params, index: index, width: width, height: height)
        }
    }

    private static func window(from params: AVHDRPlusColorTransformParams, index: Int, width: Int32, height: Int32) -> ProcessingWindow {
        let percentiles = distributionMaxRGB(from: params)
        let toneMapping: ToneMapping?
        if params.tone_mapping_flag != 0,
           let kneePointX = float(params.knee_point_x),
           let kneePointY = float(params.knee_point_y)
        {
            toneMapping = ToneMapping(
                kneePointX: kneePointX,
                kneePointY: kneePointY,
                bezierCurveAnchors: bezierCurveAnchors(from: params)
            )
        } else {
            toneMapping = nil
        }
        return ProcessingWindow(
            bounds: index == 0 ? .fullFrame : bounds(from: params),
            selector: index == 0 ? nil : selector(from: params, width: width, height: height),
            overlapProcessOption: OverlapProcessOption(rawValue: UInt8(params.overlap_process_option.rawValue)) ?? .weightedAveraging,
            maxSCL: [
                float(params.maxscl.0),
                float(params.maxscl.1),
                float(params.maxscl.2),
            ].compactMap { $0 },
            averageMaxRGB: float(params.average_maxrgb),
            distributionMaxRGB: percentiles,
            fractionBrightPixels: float(params.fraction_bright_pixels),
            toneMapping: toneMapping,
            colorSaturationWeight: params.color_saturation_mapping_flag != 0 ? float(params.color_saturation_weight) : nil
        )
    }

    private static func selector(from params: AVHDRPlusColorTransformParams, width: Int32, height: Int32) -> PixelSelector? {
        let normalizedWidth = max(Float(width - 1), 1)
        let normalizedHeight = max(Float(height - 1), 1)
        let externalMajor = Float(params.semimajor_axis_external_ellipse) / normalizedWidth
        let externalMinor = Float(params.semiminor_axis_external_ellipse) / normalizedHeight
        guard externalMajor > 0, externalMinor > 0 else {
            return nil
        }
        let internalMajor = Float(params.semimajor_axis_internal_ellipse) / normalizedWidth
        let internalMinor = externalMinor * min(max(internalMajor / max(externalMajor, 0.0001), 0.0001), 1)
        return PixelSelector(
            centerX: Float(params.center_of_ellipse_x) / normalizedWidth,
            centerY: Float(params.center_of_ellipse_y) / normalizedHeight,
            rotationRadians: Float(params.rotation_angle) * .pi / 180,
            semimajorAxisInternal: internalMajor,
            semimajorAxisExternal: externalMajor,
            semiminorAxisInternal: internalMinor,
            semiminorAxisExternal: externalMinor
        )
    }

    private static func bounds(from params: AVHDRPlusColorTransformParams) -> WindowBounds {
        WindowBounds(
            minX: float(params.window_upper_left_corner_x) ?? 0,
            minY: float(params.window_upper_left_corner_y) ?? 0,
            maxX: float(params.window_lower_right_corner_x) ?? 1,
            maxY: float(params.window_lower_right_corner_y) ?? 1
        )
    }

    private static func distributionMaxRGB(from params: AVHDRPlusColorTransformParams) -> [Percentile] {
        let values = [
            params.distribution_maxrgb.0,
            params.distribution_maxrgb.1,
            params.distribution_maxrgb.2,
            params.distribution_maxrgb.3,
            params.distribution_maxrgb.4,
            params.distribution_maxrgb.5,
            params.distribution_maxrgb.6,
            params.distribution_maxrgb.7,
            params.distribution_maxrgb.8,
            params.distribution_maxrgb.9,
            params.distribution_maxrgb.10,
            params.distribution_maxrgb.11,
            params.distribution_maxrgb.12,
            params.distribution_maxrgb.13,
            params.distribution_maxrgb.14,
        ]
        let count = min(max(Int(params.num_distribution_maxrgb_percentiles), 0), values.count)
        return values.prefix(count).compactMap { value in
            guard let percentile = float(value.percentile) else {
                return nil
            }
            return Percentile(percentage: value.percentage, percentile: percentile)
        }
    }

    private static func bezierCurveAnchors(from params: AVHDRPlusColorTransformParams) -> [Float] {
        let values = [
            params.bezier_curve_anchors.0,
            params.bezier_curve_anchors.1,
            params.bezier_curve_anchors.2,
            params.bezier_curve_anchors.3,
            params.bezier_curve_anchors.4,
            params.bezier_curve_anchors.5,
            params.bezier_curve_anchors.6,
            params.bezier_curve_anchors.7,
            params.bezier_curve_anchors.8,
            params.bezier_curve_anchors.9,
            params.bezier_curve_anchors.10,
            params.bezier_curve_anchors.11,
            params.bezier_curve_anchors.12,
            params.bezier_curve_anchors.13,
            params.bezier_curve_anchors.14,
        ]
        let count = min(max(Int(params.num_bezier_curve_anchors), 0), values.count)
        return values.prefix(count).compactMap(float)
    }

    private static func luminanceGrid<Grid>(flag: UInt8, rows: UInt8, columns: UInt8, grid: Grid) -> LuminanceGrid? {
        guard flag != 0 else {
            return nil
        }
        let rowCount = min(max(Int(rows), 0), 25)
        let columnCount = min(max(Int(columns), 0), 25)
        let requestedCount = rowCount * columnCount
        guard requestedCount > 0 else {
            return nil
        }
        let values = withUnsafeBytes(of: grid) { rawBuffer -> [Float] in
            let rationals = rawBuffer.bindMemory(to: AVRational.self)
            return rationals.prefix(min(requestedCount, rationals.count)).compactMap(float)
        }
        return LuminanceGrid(rows: rowCount, columns: columnCount, values: values)
    }

    private static func float(_ rational: AVRational) -> Float? {
        guard rational.den != 0 else {
            return nil
        }
        return Float(rational.num) / Float(rational.den)
    }
}

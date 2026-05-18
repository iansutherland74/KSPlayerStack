//
//  KSOptions.swift
//  KSPlayer-tvOS
//
//  Created by kintan on 2018/3/9.
//

import AVFoundation
#if os(tvOS) || os(xrOS)
import DisplayCriteria
#endif
import OSLog

#if canImport(UIKit)
import UIKit
#endif

public enum PictureInPictureSubtitlePolicy: Sendable, Equatable {
    /// Use PiP-compatible subtitle paths when available. AVPlayer uses native legible media selection; custom sample-buffer PiP keeps overlay subtitles in the main UI only.
    case automatic
    /// Do not expose native PiP subtitle selection. Overlay subtitles continue to render in the inline player only.
    case disabled
}

public enum KSLowLatencyLiveProfile: Equatable, Sendable {
    /// Opt-in LAN live tuning for glass-to-glass latency-sensitive streams.
    ///
    /// This profile reduces FFmpeg probing, demux buffering, player buffering, and seek/cache features.
    /// It cannot guarantee a fixed latency target; protocol behavior, encoder GOP/B-frames, network jitter,
    /// camera buffering, and device decode capacity still dominate end-to-end latency.
    case lan
}

public enum VideoProjection: Equatable, Sendable {
    case equirectangular
    case equirectangularTiled
    case cubemap
    case unknown(String)
}

public enum PanoramaMode: Equatable, Sendable {
    /// Preserve flat video rendering unless apps explicitly select `display = .vr` or `.vrBox`.
    case disabled
    /// Switch recognized 360-degree equirectangular videos to the Metal sphere renderer.
    case automatic
    /// Force equirectangular 360-degree rendering for the selected video track.
    case equirectangular
}

public enum VideoDeinterlaceMode: Equatable, Sendable {
    /// Never add a deinterlacing filter automatically.
    case disabled
    /// Deinterlace only when stream metadata clearly marks interlaced content and the workload is reasonable.
    case automatic
    /// Always add a deinterlacing filter for the selected video track.
    case force
}

public enum AudioSpatializationPreference: Equatable, Sendable {
    /// Let KSPlayer follow the current output route's Spatial Audio capability.
    case automatic
    /// Tell the system the app can play multichannel content when the platform supports it.
    case enabled
    /// Keep system spatialization disabled for decoded PCM output.
    case disabled
}

public enum MultichannelAudioPreference: Equatable, Sendable {
    /// Preserve multichannel PCM when the active platform/route can accept it.
    case automatic
    /// Prefer the source channel count, still falling back when the route cannot expose it.
    case multichannel
    /// Downmix decoded output to stereo.
    case stereo
}

public enum PanoramaProjectionPolicy {
    public static func detectedProjection(metadata: [String: String]) -> VideoProjection? {
        let normalized = metadata.reduce(into: [String: String]()) { result, entry in
            result[entry.key.lowercased()] = entry.value.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
        }

        for key in ["projection", "spatial-media-projection", "spherical_projection", "spherical-projection"] {
            if let projection = normalized[key].flatMap(projection(from:)) {
                return projection
            }
        }

        for key in ["spherical", "spherical_video", "360", "is_360"] {
            if let value = normalized[key], ["1", "true", "yes", "equirectangular"].contains(value) {
                return .equirectangular
            }
        }
        return nil
    }

    public static func resolvedProjection(mode: PanoramaMode, detectedProjection: VideoProjection?) -> VideoProjection? {
        switch mode {
        case .disabled:
            return nil
        case .automatic:
            return detectedProjection?.isRenderableInSphere == true ? detectedProjection : nil
        case .equirectangular:
            return .equirectangular
        }
    }

    private static func projection(from value: String) -> VideoProjection? {
        if value.contains("tiled") {
            return .equirectangularTiled
        }
        if value.contains("equirectangular") || value.contains("equirect") {
            return .equirectangular
        }
        if value.contains("cubemap") || value.contains("cube") {
            return .cubemap
        }
        return value.isEmpty ? nil : .unknown(value)
    }
}

public extension VideoProjection {
    var isRenderableInSphere: Bool {
        switch self {
        case .equirectangular:
            return true
        case .equirectangularTiled, .cubemap, .unknown:
            return false
        }
    }
}

open class KSOptions {
    /// 最低缓存视频时间
    @Published
    public var preferredForwardBufferDuration = KSOptions.preferredForwardBufferDuration
    /// 最大缓存视频时间
    public var maxBufferDuration = KSOptions.maxBufferDuration
    /// 是否开启秒开
    public var isSecondOpen = KSOptions.isSecondOpen
    /// 开启精确seek
    public var isAccurateSeek = KSOptions.isAccurateSeek
    /// Applies to short videos only
    public var isLoopPlay = KSOptions.isLoopPlay
    /// Prefer queue/prebuffer based looping over end-of-item seek looping when loop playback is enabled.
    public var isSeamlessLoopEnabled = KSOptions.isSeamlessLoopEnabled
    /// seek完是否自动播放
    public var isSeekedAutoPlay = KSOptions.isSeekedAutoPlay
    /*
     AVSEEK_FLAG_BACKWARD: 1
     AVSEEK_FLAG_BYTE: 2
     AVSEEK_FLAG_ANY: 4
     AVSEEK_FLAG_FRAME: 8
     */
    public var seekFlags = Int32(1)
    // ffmpeg only cache http
    // 这个开关不能用，因为ff_tempfile: Cannot open temporary file
    public var cache = false
    /// Downloads eligible remote media files into app cache storage and reuses completed files on later playback.
    public var isDiskPrecacheEnabled = KSOptions.isDiskPrecacheEnabled
    /// Maximum single media file size stored by disk precache, in bytes.
    public var diskPrecacheMaxFileSize = KSOptions.diskPrecacheMaxFileSize
    /// Maximum total disk precache size, in bytes.
    public var diskPrecacheMaxCacheSize = KSOptions.diskPrecacheMaxCacheSize
    /// Override for tests or apps that manage their own cache location. Defaults to Caches/KSPlayerDiskPrecache.
    public var diskPrecacheDirectoryURL: URL?
    /// Keeps a small in-memory packet window so MEPlayer can satisfy short-range seeks without a demuxer seek.
    public var isMemorySeekCacheEnabled = KSOptions.isMemorySeekCacheEnabled
    /// Recent playback duration retained by the memory seek cache.
    public var memorySeekCacheDuration = KSOptions.memorySeekCacheDuration
    /// Maximum compressed packet bytes retained by the memory seek cache.
    public var memorySeekCacheMaxByteSize = KSOptions.memorySeekCacheMaxByteSize
    //  record stream
    public var outputURL: URL?
    public var avOptions = [String: Any]()
    public var formatContextOptions = [String: Any]()
    public var decoderOptions = [String: Any]()
    public var probesize: Int64?
    public var maxAnalyzeDuration: Int64?
    public var lowres = UInt8(0)
    public var nobuffer = false
    public var codecLowDelay = false
    /// Opt-in live profile. Defaults to nil so normal VOD/live buffering behavior is unchanged.
    public internal(set) var lowLatencyLiveProfile: KSLowLatencyLiveProfile?
    public var startPlayTime: TimeInterval = 0
    public var startPlayRate: Float = 1.0
    public var registerRemoteControll: Bool = true // 默认支持来自系统控制中心的控制
    public var referer: String? {
        didSet {
            if let referer {
                formatContextOptions["referer"] = referer
            } else {
                formatContextOptions["referer"] = nil
            }
            updateAVHTTPHeader("Referer", value: referer, replacingPriorValue: oldValue)
        }
    }

    public var userAgent: String? = "KSPlayer" {
        didSet {
            formatContextOptions["user_agent"] = userAgent
            updateAVHTTPHeader("User-Agent", value: userAgent, replacingPriorValue: oldValue)
        }
    }

    // audio
    public var audioFilters = [String]()
    public var syncDecodeAudio = false
    public var audioSpatializationPreference = KSOptions.audioSpatializationPreference
    public var multichannelAudioPreference = KSOptions.multichannelAudioPreference
    #if !os(macOS)
    /// Overrides the AVAudioSession route sharing policy used for playback.
    /// Set this to `.longFormAudio` when an app wants audio-only AirPlay routes.
    public var audioRouteSharingPolicy = KSOptions.audioRouteSharingPolicy
    #endif
    // sutile
    public var autoSelectEmbedSubtitle = true
    public var isSeekImageSubtitle = false
    /// Controls whether KSPlayer maps Apple's system caption appearance into rendered subtitles.
    public var subtitleCaptionAppearancePolicy = KSOptions.subtitleCaptionAppearancePolicy
    /// Improves subtitle contrast for HDR video while keeping subtitles as UI overlays.
    public var subtitleHDREffectPolicy = KSOptions.subtitleHDREffectPolicy
    /// Feeds decoded FFmpeg audio frames to an offline subtitle generator and exposes it as a subtitle track.
    public var isOfflineSubtitleGenerationEnabled = KSOptions.isOfflineSubtitleGenerationEnabled
    /// Apps can provide an on-device Speech, Translation, or custom local-model generator without bundling weights in KSPlayer.
    public var offlineSubtitleGenerator: (any AudioRecognize)? = KSOptions.offlineSubtitleGenerator
    /// Apps can opt in to online subtitle search by supplying providers configured with their own credentials.
    public var onlineSubtitleProviders: [any OnlineSubtitleProvider] = KSOptions.onlineSubtitleProviders
    /// Default languages used by online subtitle providers when a search UI does not pass explicit languages.
    public var onlineSubtitleLanguages = KSOptions.onlineSubtitleLanguages
    // video
    public var display = DisplayEnum.plane
    public var panoramaMode = KSOptions.panoramaMode
    public var videoDelay = 0.0 // s
    public var deinterlaceMode = KSOptions.deinterlaceMode
    @available(*, deprecated, message: "Use deinterlaceMode instead.")
    public var autoDeInterlace: Bool {
        get {
            deinterlaceMode != .disabled
        }
        set {
            deinterlaceMode = newValue ? .automatic : .disabled
        }
    }
    public var autoRotate = true
    public var destinationDynamicRange: DynamicRange?
    public var videoAdaptable = true
    public var videoFilters = [String]()
    public var videoUpscaling = VideoUpscalingMode.none
    /// GPU-side SDR color controls for the Metal renderer. Neutral defaults preserve existing output.
    public var videoColorAdjustment = KSOptions.videoColorAdjustment
    /// Shows a floating time bubble while hovering or scrubbing the progress bar.
    public var isProgressPreviewEnabled = KSOptions.isProgressPreviewEnabled
    /// Controls whether progress previews may warm thumbnail images in the background.
    public var progressPreviewThumbnailMode = KSOptions.progressPreviewThumbnailMode
    /// Prepares the next definition/source before swapping so VOD quality changes can hand off with minimal delay.
    public var isDefinitionSwitchPrewarmingEnabled = KSOptions.isDefinitionSwitchPrewarmingEnabled
    /// Maximum time to wait for a prewarmed definition/source before falling back to the normal replace path.
    public var definitionSwitchPrewarmTimeout = KSOptions.definitionSwitchPrewarmTimeout
    /// Automatically switches among separate KSPlayerResource definitions based on buffer health.
    ///
    /// HLS/DASH/other adaptive streaming manifests are left to the native player or FFmpeg demuxer.
    public var isAdaptiveBitrateSwitchingEnabled = KSOptions.isAdaptiveBitrateSwitchingEnabled
    public var adaptiveBitrateSwitchingPolicy = KSOptions.adaptiveBitrateSwitchingPolicy
    public var syncDecodeVideo = false
    public var hardwareDecode = KSOptions.hardwareDecode
    public var asynchronousDecompression = KSOptions.asynchronousDecompression
    public var videoDisable = false
    public var canStartPictureInPictureAutomaticallyFromInline = KSOptions.canStartPictureInPictureAutomaticallyFromInline
    /// Controls subtitle behavior for Picture in Picture. Overlay subtitles are not burned into video unless a future renderer explicitly opts in.
    public var pictureInPictureSubtitlePolicy = KSOptions.pictureInPictureSubtitlePolicy
    public var automaticWindowResize = true
    @Published
    public var videoInterlacingType: VideoInterlacingType?
    private var videoClockDelayCount = 0

    public internal(set) var formatName = ""
    public internal(set) var prepareTime = 0.0
    public internal(set) var dnsStartTime = 0.0
    public internal(set) var tcpStartTime = 0.0
    public internal(set) var tcpConnectedTime = 0.0
    public internal(set) var openTime = 0.0
    public internal(set) var findTime = 0.0
    public internal(set) var readyTime = 0.0
    public internal(set) var readAudioTime = 0.0
    public internal(set) var readVideoTime = 0.0
    public internal(set) var decodeAudioTime = 0.0
    public internal(set) var decodeVideoTime = 0.0
    public init() {
        formatContextOptions["user_agent"] = userAgent
        updateAVHTTPHeader("User-Agent", value: userAgent, replacingPriorValue: nil)
        // 参数的配置可以参考protocols.texi 和 http.c
        // 这个一定要，不然有的流就会判断不准FieldOrder
        formatContextOptions["scan_all_pmts"] = 1
        // ts直播流需要加这个才能一直直播下去，不然播放一小段就会结束了。
        formatContextOptions["reconnect"] = 1
        formatContextOptions["reconnect_streamed"] = 1
        // 这个是用来开启http的链接复用（keep-alive）。vlc默认是打开的，所以这边也默认打开。
        // 开启这个，百度网盘的视频链接无法播放
        // formatContextOptions["multiple_requests"] = 1
        // 下面是用来处理秒开的参数，有需要的自己打开。默认不开，不然在播放某些特殊的ts直播流会频繁卡顿。
//        formatContextOptions["auto_convert"] = 0
//        formatContextOptions["fps_probe_size"] = 3
//        formatContextOptions["rw_timeout"] = 10_000_000
//        formatContextOptions["max_analyze_duration"] = 300 * 1000
        // 默认情况下允许所有协议，只有嵌套协议才需要指定这个协议子集，例如m3u8里面有http。
//        formatContextOptions["protocol_whitelist"] = "file,http,https,tcp,tls,crypto,async,cache,data,httpproxy"
        // 开启这个，纯ipv6地址会无法播放。并且有些视频结束了，但还会一直尝试重连。所以这个值默认不设置
//        formatContextOptions["reconnect_at_eof"] = 1
        // 开启这个，会导致tcp Failed to resolve hostname 还会一直重试
//        formatContextOptions["reconnect_on_network_error"] = 1
        // There is total different meaning for 'listen_timeout' option in rtmp
        // set 'listen_timeout' = -1 for rtmp、rtsp
//        formatContextOptions["listen_timeout"] = 3
        decoderOptions["threads"] = "auto"
        decoderOptions["refcounted_frames"] = "1"
    }

    /// Applies latency-sensitive live defaults for LAN streams.
    ///
    /// Use this only for feeds where lower latency is more important than startup robustness,
    /// seek/cache features, or tolerance for sparse stream metadata. It does not promise sub-200ms
    /// playback; the stream protocol, encoder GOP/B-frame structure, network, camera buffering,
    /// and hardware decode availability remain decisive.
    public func applyLowLatencyLiveProfile(_ profile: KSLowLatencyLiveProfile = .lan) {
        LowLatencyLivePlaybackPolicy.apply(profile: profile, to: self)
    }

    /**
     you can add http-header or other options which mentions in https://developer.apple.com/reference/avfoundation/avurlasset/initialization_options

     to add http-header init options like this
     ```
     options.appendHeader(["Referer":"https:www.xxx.com"])
     ```
     */
    public func appendHeader(_ header: [String: String]) {
        var oldValue = avOptions["AVURLAssetHTTPHeaderFieldsKey"] as? [String: String] ?? [
            String: String
        ]()
        oldValue.merge(header) { _, new in new }
        avOptions["AVURLAssetHTTPHeaderFieldsKey"] = oldValue
        var str = formatContextOptions["headers"] as? String ?? ""
        for (key, value) in header {
            str.append("\(key): \(value)\r\n")
        }
        formatContextOptions["headers"] = str
    }

    func prepareFormatContextOptions(for url: URL) {
        guard let scheme = url.ksNormalizedScheme, url.isFFmpegOnlyInputScheme else {
            return
        }
        appendProtocolWhitelistEntries(Self.protocolWhitelistEntries(for: scheme))
    }

    private static func protocolWhitelistEntries(for scheme: String) -> [String] {
        switch scheme {
        case "nfs":
            return ["nfs", "tcp", "udp"]
        case "smb":
            return ["smb", "tcp"]
        case "srt":
            return ["srt", "udp", "tcp"]
        default:
            return [scheme]
        }
    }

    private func appendProtocolWhitelistEntries(_ entries: [String]) {
        guard let whitelist = formatContextOptions["protocol_whitelist"] as? String else {
            return
        }
        var protocols = whitelist
            .split(separator: ",")
            .map { $0.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() }
            .filter { !$0.isEmpty }
        guard !protocols.isEmpty else {
            return
        }
        for entry in entries where !protocols.contains(entry) {
            protocols.append(entry)
        }
        formatContextOptions["protocol_whitelist"] = protocols.joined(separator: ",")
    }

    private func updateAVHTTPHeader(_ field: String, value: String?, replacingPriorValue oldValue: String?) {
        let key = "AVURLAssetHTTPHeaderFieldsKey"
        var headers = avOptions[key] as? [String: String] ?? [:]
        let existingKey = headers.keys.first { $0.caseInsensitiveCompare(field) == .orderedSame }
        if let value {
            if let existingKey {
                if oldValue == nil || headers[existingKey] == oldValue {
                    headers[existingKey] = value
                }
            } else {
                headers[field] = value
            }
        } else if let existingKey, oldValue == nil || headers[existingKey] == oldValue {
            headers.removeValue(forKey: existingKey)
        }
        avOptions[key] = headers.isEmpty ? nil : headers
    }

    public func setCookie(_ cookies: [HTTPCookie]) {
        avOptions[AVURLAssetHTTPCookiesKey] = cookies
        let cookieStr = cookies.map { cookie in "\(cookie.name)=\(cookie.value)" }.joined(separator: "; ")
        appendHeader(["Cookie": cookieStr])
    }

    // 缓冲算法函数
    open func playable(capacitys: [CapacityProtocol], isFirst: Bool, isSeek: Bool) -> LoadingState {
        let packetCount = capacitys.map(\.packetCount).min() ?? 0
        let frameCount = capacitys.map(\.frameCount).min() ?? 0
        let isEndOfFile = capacitys.allSatisfy(\.isEndOfFile)
        let loadedTime = capacitys.map(\.loadedTime).min() ?? 0
        let progress = preferredForwardBufferDuration == 0 ? 100 : loadedTime * 100.0 / preferredForwardBufferDuration
        let isPlayable = capacitys.allSatisfy { capacity in
            if capacity.isEndOfFile && capacity.packetCount == 0 {
                return true
            }
            guard capacity.frameCount >= 2 else {
                return false
            }
            if capacity.isEndOfFile {
                return true
            }
            if (syncDecodeVideo && capacity.mediaType == .video) || (syncDecodeAudio && capacity.mediaType == .audio) {
                return true
            }
            if isFirst || isSeek {
                // 让纯音频能更快的打开
                if capacity.mediaType == .audio || isSecondOpen {
                    if isFirst {
                        return true
                    } else {
                        return capacity.loadedTime >= self.preferredForwardBufferDuration / 2
                    }
                }
            }
            return capacity.loadedTime >= self.preferredForwardBufferDuration
        }
        return LoadingState(loadedTime: loadedTime, progress: progress, packetCount: packetCount,
                            frameCount: frameCount, isEndOfFile: isEndOfFile, isPlayable: isPlayable,
                            isFirst: isFirst, isSeek: isSeek)
    }

    open func adaptable(state: VideoAdaptationState?) -> (Int64, Int64)? {
        guard let state, let last = state.bitRateStates.last, CACurrentMediaTime() - last.time > maxBufferDuration / 2, let index = state.bitRates.firstIndex(of: last.bitRate) else {
            return nil
        }
        let isUp = state.loadedCount > Int(Double(state.fps) * maxBufferDuration / 2)
        if isUp != state.isPlayable {
            return nil
        }
        if isUp {
            if index < state.bitRates.endIndex - 1 {
                return (last.bitRate, state.bitRates[index + 1])
            }
        } else {
            if index > state.bitRates.startIndex {
                return (last.bitRate, state.bitRates[index - 1])
            }
        }
        return nil
    }

    ///  wanted video stream index, or nil for automatic selection
    /// - Parameter : video track
    /// - Returns: The index of the track
    open func wantedVideo(tracks _: [MediaPlayerTrack]) -> Int? {
        nil
    }

    /// wanted audio stream index, or nil for automatic selection
    /// - Parameter :  audio track
    /// - Returns: The index of the track
    open func wantedAudio(tracks _: [MediaPlayerTrack]) -> Int? {
        nil
    }

    open func videoFrameMaxCount(fps: Float, naturalSize: CGSize, isLive: Bool) -> UInt8 {
        HighPerformanceVideoPlaybackPolicy.frameCapacity(fps: fps, naturalSize: naturalSize, isLive: isLive)
    }

    open func audioFrameMaxCount(fps: Float, channelCount: Int) -> UInt8 {
        let count = (Int(fps) * channelCount) >> 2
        if count >= UInt8.max {
            return UInt8.max
        } else {
            return UInt8(count)
        }
    }

    /// customize dar
    /// - Parameters:
    ///   - sar: SAR(Sample Aspect Ratio)
    ///   - dar: PAR(Pixel Aspect Ratio)
    /// - Returns: DAR(Display Aspect Ratio)
    open func customizeDar(sar _: CGSize, par _: CGSize) -> CGSize? {
        nil
    }

    // 虽然只有iOS才支持PIP。但是因为AVSampleBufferDisplayLayer能够支持HDR10+。所以默认还是推荐用AVSampleBufferDisplayLayer
    open func isUseDisplayLayer() -> Bool {
        isUseDisplayLayer(dynamicRange: nil)
    }

    open func isUseDisplayLayer(dynamicRange: DynamicRange?) -> Bool {
        display == .plane && !videoColorAdjustment.shouldApply(dynamicRange: dynamicRange)
    }

    open func urlIO(log: String) {
        if log.starts(with: "Original list of addresses"), dnsStartTime == 0 {
            dnsStartTime = CACurrentMediaTime()
        } else if log.starts(with: "Starting connection attempt to"), tcpStartTime == 0 {
            tcpStartTime = CACurrentMediaTime()
        } else if log.starts(with: "Successfully connected to"), tcpConnectedTime == 0 {
            tcpConnectedTime = CACurrentMediaTime()
        }
    }

    private var idetTypeMap = [VideoInterlacingType: UInt]()
    open func filter(log: String) {
        if log.starts(with: "Repeated Field:"), deinterlaceMode != .disabled {
            for str in log.split(separator: ",") {
                let map = str.split(separator: ":")
                if map.count >= 2 {
                    if String(map[0].trimmingCharacters(in: .whitespaces)) == "Multi frame" {
                        if let type = VideoInterlacingType(rawValue: map[1].trimmingCharacters(in: .whitespacesAndNewlines)) {
                            idetTypeMap[type] = (idetTypeMap[type] ?? 0) + 1
                            let tff = idetTypeMap[.tff] ?? 0
                            let bff = idetTypeMap[.bff] ?? 0
                            let progressive = idetTypeMap[.progressive] ?? 0
                            let undetermined = idetTypeMap[.undetermined] ?? 0
                            if progressive - tff - bff > 100 {
                                videoInterlacingType = .progressive
                                deinterlaceMode = .disabled
                            } else if bff - progressive > 100 {
                                videoInterlacingType = .bff
                                deinterlaceMode = .disabled
                            } else if tff - progressive > 100 {
                                videoInterlacingType = .tff
                                deinterlaceMode = .disabled
                            } else if undetermined - progressive - tff - bff > 100 {
                                videoInterlacingType = .undetermined
                                deinterlaceMode = .disabled
                            }
                        }
                    }
                }
            }
        }
    }

    open func sei(string: String) {
        KSLog("sei \(string)")
    }

    /**
            在创建解码器之前可以对KSOptions和assetTrack做一些处理。例如判断fieldOrder为tt或bb的话，那就自动加videofilters
     */
    open func process(assetTrack: some MediaPlayerTrack) {
        if assetTrack.mediaType == .video {
            if display == .plane {
                let detectedProjection = (assetTrack as? FFmpegAssetTrack)?.panoramaProjection
                let projection = PanoramaProjectionPolicy.resolvedProjection(mode: panoramaMode, detectedProjection: detectedProjection)
                if projection?.isRenderableInSphere == true {
                    display = .vr
                }
            }
            let decision = VideoDeinterlacePolicy.decision(
                mode: deinterlaceMode,
                fieldOrder: assetTrack.fieldOrder,
                fps: assetTrack.nominalFrameRate,
                naturalSize: assetTrack.naturalSize,
                yadifMode: KSOptions.yadifMode,
                addIdet: KSOptions.deInterlaceAddIdet
            )
            videoInterlacingType = decision.detectedInterlacingType
            if decision.shouldApplyFilter {
                hardwareDecode = false
                asynchronousDecompression = false
                for filter in decision.filters where !containsEquivalentVideoFilter(filter) {
                    videoFilters.append(filter)
                }
                if decision.doublesFrameRate {
                    assetTrack.nominalFrameRate = assetTrack.nominalFrameRate * 2
                }
            } else if let reason = decision.skipReason {
                KSLog(level: .debug, "[video] deinterlace skipped: \(reason)")
            }
            if let lowLatencyLiveProfile,
               let diagnostic = LowLatencyLivePlaybackPolicy.diagnosticMessage(
                   profile: lowLatencyLiveProfile,
                   fps: assetTrack.nominalFrameRate,
                   naturalSize: assetTrack.naturalSize,
                   hardwareDecode: hardwareDecode,
                   asynchronousDecompression: asynchronousDecompression
               )
            {
                KSLog(level: .debug, diagnostic)
            }
        }
    }

    private func containsEquivalentVideoFilter(_ filter: String) -> Bool {
        if filter == "idet" {
            return videoFilters.contains("idet")
        }
        if filter.hasPrefix("yadif") || filter.hasPrefix("bwdif") {
            return videoFilters.contains { existing in
                existing.hasPrefix("yadif") || existing.hasPrefix("bwdif")
            }
        }
        return videoFilters.contains(filter)
    }

    @MainActor
    open func updateVideo(refreshRate: Float, isDovi: Bool, formatDescription: CMFormatDescription?) {
        #if os(tvOS) || os(xrOS)
        /**
         快速更改preferredDisplayCriteria，会导致isDisplayModeSwitchInProgress变成true。
         例如退出一个视频，然后在3s内重新进入的话。所以不判断isDisplayModeSwitchInProgress了
         */
        guard let displayManager = UIApplication.shared.windows.first?.avDisplayManager,
              displayManager.isDisplayCriteriaMatchingEnabled
        else {
            return
        }
        if let dynamicRange = isDovi ? .dolbyVision : formatDescription?.dynamicRange {
            displayManager.preferredDisplayCriteria = AVDisplayCriteria(refreshRate: refreshRate, videoDynamicRange: dynamicRange.rawValue)
        }
        #endif
    }

    open func videoClockSync(main: KSClock, nextVideoTime: TimeInterval, fps: Double, frameCount: Int) -> (Double, ClockProcessType) {
        let desire = main.getTime() - videoDelay
        let diff = nextVideoTime - desire
//        print("[video] video diff \(diff) nextVideoTime \(nextVideoTime) main \(main.time.seconds)")
        if diff >= 1 / fps / 2 {
            videoClockDelayCount = 0
            return (diff, .remain)
        } else {
            if diff < -4 / fps {
                videoClockDelayCount += 1
                let log = "[video] video delay=\(diff), clock=\(desire), delay count=\(videoClockDelayCount), frameCount=\(frameCount)"
                if frameCount == 1 {
                    if diff < -1, videoClockDelayCount % 10 == 0 {
                        KSLog("\(log) drop gop Packet")
                        return (diff, .dropGOPPacket)
                    } else if videoClockDelayCount % 5 == 0 {
                        KSLog("\(log) drop next frame")
                        return (diff, .dropNextFrame)
                    } else {
                        return (diff, .next)
                    }
                } else {
                    if diff < -8, videoClockDelayCount % 100 == 0 {
                        KSLog("\(log) seek video track")
                        return (diff, .seek)
                    }
                    if diff < -1, videoClockDelayCount % 10 == 0 {
                        KSLog("\(log) flush video track")
                        return (diff, .flush)
                    }
                    if videoClockDelayCount % 2 == 0 {
                        KSLog("\(log) drop next frame")
                        return (diff, .dropNextFrame)
                    } else {
                        return (diff, .next)
                    }
                }
            } else {
                videoClockDelayCount = 0
                return (diff, .next)
            }
        }
    }

    open func availableDynamicRange(_ contentRange: DynamicRange?) -> DynamicRange? {
        #if canImport(UIKit)
        let availableHDRModes = AVPlayer.availableHDRModes
        if let preferedDynamicRange = destinationDynamicRange {
            // value of 0 indicates that no HDR modes are supported.
            if availableHDRModes == AVPlayer.HDRMode(rawValue: 0) {
                return .sdr
            } else if availableHDRModes.contains(preferedDynamicRange.hdrMode) {
                return preferedDynamicRange
            } else if let contentRange,
                      availableHDRModes.contains(contentRange.hdrMode)
            {
                return contentRange
            } else if preferedDynamicRange != .sdr { // trying update to HDR mode
                return availableHDRModes.dynamicRange
            }
        }
        return contentRange
        #else
        return destinationDynamicRange ?? contentRange
        #endif
    }

    open func playerLayerDeinit() {
        #if os(tvOS) || os(xrOS)
        runOnMainThread {
            UIApplication.shared.windows.first?.avDisplayManager.preferredDisplayCriteria = nil
        }
        #endif
    }

    open func liveAdaptivePlaybackRate(loadingState _: LoadingState) -> Float? {
        nil
//        if loadingState.isFirst {
//            return nil
//        }
//        if loadingState.loadedTime > preferredForwardBufferDuration + 5 {
//            return 1.2
//        } else if loadingState.loadedTime < preferredForwardBufferDuration / 2 {
//            return 0.8
//        } else {
//            return 1
//        }
    }

    open func process(url _: URL) -> AbstractAVIOContext? {
        nil
    }
}

public enum VideoInterlacingType: String {
    case tff
    case bff
    case progressive
    case undetermined
}

struct VideoDeinterlaceDecision: Equatable {
    let filters: [String]
    let detectedInterlacingType: VideoInterlacingType?
    let skipReason: String?

    var shouldApplyFilter: Bool {
        !filters.isEmpty
    }

    var doublesFrameRate: Bool {
        filters.contains { filter in
            filter.contains("mode=1") || filter.contains("mode=3")
        }
    }
}

enum VideoDeinterlacePolicy {
    private static let avFrameFlagInterlaced = Int32(1 << 3)
    private static let avFrameFlagTopFieldFirst = Int32(1 << 4)

    static func detectedInterlacingType(fieldOrder: FFmpegFieldOrder) -> VideoInterlacingType? {
        switch fieldOrder {
        case .tt, .tb:
            return .tff
        case .bb, .bt:
            return .bff
        case .progressive:
            return .progressive
        case .unknown:
            return nil
        }
    }

    static func detectedInterlacingType(frameFlags: Int32) -> VideoInterlacingType {
        guard frameFlags & avFrameFlagInterlaced == avFrameFlagInterlaced else {
            return .progressive
        }
        return frameFlags & avFrameFlagTopFieldFirst == avFrameFlagTopFieldFirst ? .tff : .bff
    }

    static func decision(
        mode: VideoDeinterlaceMode,
        fieldOrder: FFmpegFieldOrder,
        fps: Float,
        naturalSize: CGSize,
        yadifMode: Int,
        addIdet: Bool
    ) -> VideoDeinterlaceDecision {
        let detectedType = detectedInterlacingType(fieldOrder: fieldOrder)
        switch mode {
        case .disabled:
            return VideoDeinterlaceDecision(filters: [], detectedInterlacingType: detectedType, skipReason: "disabled")
        case .automatic:
            guard detectedType == .tff || detectedType == .bff else {
                return VideoDeinterlaceDecision(filters: [], detectedInterlacingType: detectedType, skipReason: nil)
            }
            guard !HighPerformanceVideoPlaybackPolicy.isHighWorkload(fps: fps, naturalSize: naturalSize) else {
                return VideoDeinterlaceDecision(filters: [], detectedInterlacingType: detectedType, skipReason: "high workload requires force mode")
            }
            return filterDecision(detectedType: detectedType, yadifMode: yadifMode, addIdet: addIdet)
        case .force:
            return filterDecision(detectedType: detectedType, yadifMode: yadifMode, addIdet: addIdet)
        }
    }

    private static func filterDecision(detectedType: VideoInterlacingType?, yadifMode: Int, addIdet: Bool) -> VideoDeinterlaceDecision {
        var filters = [String]()
        if addIdet {
            filters.append("idet")
        }
        // Keep deinterlacing on the software filter path; yadif_videotoolbox is built but documented here as crash-prone.
        filters.append("yadif=mode=\(yadifMode):parity=-1:deint=1")
        return VideoDeinterlaceDecision(filters: filters, detectedInterlacingType: detectedType, skipReason: nil)
    }
}

public enum VideoUpscalingMode: Equatable, Sendable {
    case none
    case appleSuperResolution(scaleFactor: Float = 2)
}

public struct VideoColorAdjustment: Equatable, Sendable {
    public enum HDRPolicy: Equatable, Sendable {
        /// Keep HDR/Dolby Vision output on the existing system-managed path.
        case preserveHDR
        /// Allow the Metal shader adjustment on HDR content. This is intentionally opt-in.
        case allowHDR
    }

    public static let neutral = VideoColorAdjustment()
    public static let saturationRange: ClosedRange<Float> = 0 ... 2
    public static let brightnessRange: ClosedRange<Float> = -1 ... 1
    public static let contrastRange: ClosedRange<Float> = 0 ... 2

    public let saturation: Float
    public let brightness: Float
    public let contrast: Float
    public let hdrPolicy: HDRPolicy

    public init(saturation: Float = 1, brightness: Float = 0, contrast: Float = 1, hdrPolicy: HDRPolicy = .preserveHDR) {
        self.saturation = Self.clamp(saturation, to: Self.saturationRange)
        self.brightness = Self.clamp(brightness, to: Self.brightnessRange)
        self.contrast = Self.clamp(contrast, to: Self.contrastRange)
        self.hdrPolicy = hdrPolicy
    }

    public var isNeutral: Bool {
        saturation == 1 && brightness == 0 && contrast == 1
    }

    public func shouldApply(dynamicRange: DynamicRange?) -> Bool {
        guard !isNeutral else {
            return false
        }
        return hdrPolicy == .allowHDR || dynamicRange?.isHDR != true
    }

    private static func clamp(_ value: Float, to range: ClosedRange<Float>) -> Float {
        min(max(value, range.lowerBound), range.upperBound)
    }
}

public enum HighPerformanceVideoPlaybackPolicy {
    static let highFrameRateThreshold: Float = 90
    static let eightKPixelThreshold = 7_680 * 4_320

    static func isHighWorkload(fps: Float, naturalSize: CGSize) -> Bool {
        fps >= highFrameRateThreshold || pixelCount(naturalSize) >= eightKPixelThreshold
    }

    static func frameCapacity(fps: Float, naturalSize: CGSize, isLive: Bool) -> UInt8 {
        guard isHighWorkload(fps: fps, naturalSize: naturalSize) else {
            return isLive ? 4 : 16
        }
        if isLive {
            return 8
        }
        let highFPSCapacity = Int(ceil(max(fps, 1) / 4))
        return UInt8(min(32, max(16, highFPSCapacity)))
    }

    static func displayFrameRateRange(fps: Float) -> (minimum: Float, maximum: Float, preferred: Float) {
        let preferred = max(1, ceil(fps))
        if fps >= highFrameRateThreshold {
            return (minimum: min(60, preferred / 2), maximum: preferred, preferred: preferred)
        }
        return (minimum: preferred, maximum: preferred * 2, preferred: preferred)
    }

    static func shouldApplyUpscaling(mode: VideoUpscalingMode, sourceSize: CGSize, fps: Float) -> Bool {
        guard mode != .none else {
            return false
        }
        return !isHighWorkload(fps: fps, naturalSize: sourceSize)
    }

    private static func pixelCount(_ size: CGSize) -> Int {
        let width = max(0, Int(size.width.rounded(.up)))
        let height = max(0, Int(size.height.rounded(.up)))
        return width * height
    }
}

enum LowLatencyLivePlaybackPolicy {
    private static let fourKPixelThreshold = 3_840 * 2_160
    private static let lanPreferredForwardBufferDuration = 0.12
    private static let lanMaxBufferDuration = 0.5
    private static let lanProbeSize: Int64 = 32 * 1024
    private static let lanMaxAnalyzeDuration: Int64 = 100 * 1000
    private static let lanMaxDelay = 100 * 1000

    static func apply(profile: KSLowLatencyLiveProfile, to options: KSOptions) {
        switch profile {
        case .lan:
            options.lowLatencyLiveProfile = profile
            options.preferredForwardBufferDuration = lanPreferredForwardBufferDuration
            options.maxBufferDuration = lanMaxBufferDuration
            options.cache = false
            options.isDiskPrecacheEnabled = false
            options.isMemorySeekCacheEnabled = false
            options.isDefinitionSwitchPrewarmingEnabled = false
            options.nobuffer = true
            options.codecLowDelay = true
            options.hardwareDecode = true
            options.asynchronousDecompression = true
            options.syncDecodeVideo = false
            options.syncDecodeAudio = false
            options.probesize = options.probesize ?? lanProbeSize
            options.maxAnalyzeDuration = options.maxAnalyzeDuration ?? lanMaxAnalyzeDuration
            setDefaultFormatOption("max_delay", value: lanMaxDelay, options: options)
            appendListOption("fflags", value: "nobuffer", separator: "+", options: options)
            appendListOption("avioflags", value: "direct", separator: "+", options: options)
        }
    }

    static func diagnosticMessage(
        profile: KSLowLatencyLiveProfile,
        fps: Float,
        naturalSize: CGSize,
        hardwareDecode: Bool,
        asynchronousDecompression: Bool
    ) -> String? {
        guard profile == .lan, pixelCount(naturalSize) >= fourKPixelThreshold else {
            return nil
        }
        if !hardwareDecode {
            return "[video] low latency live 4K may miss latency targets because hardware decode is disabled"
        }
        if fps >= HighPerformanceVideoPlaybackPolicy.highFrameRateThreshold, !asynchronousDecompression {
            return "[video] low latency live 4K high-FPS may need asynchronous VideoToolbox decode"
        }
        return nil
    }

    private static func setDefaultFormatOption(_ key: String, value: Any, options: KSOptions) {
        if options.formatContextOptions[key] == nil {
            options.formatContextOptions[key] = value
        }
    }

    private static func appendListOption(_ key: String, value: String, separator: Character, options: KSOptions) {
        guard let existing = options.formatContextOptions[key] as? String, !existing.isEmpty else {
            options.formatContextOptions[key] = value
            return
        }
        let values = existing
            .split(whereSeparator: { $0 == separator || $0 == "," || $0 == " " })
            .map(String.init)
        guard !values.contains(value) else {
            return
        }
        options.formatContextOptions[key] = existing + String(separator) + value
    }

    private static func pixelCount(_ size: CGSize) -> Int {
        let width = max(0, Int(size.width.rounded(.up)))
        let height = max(0, Int(size.height.rounded(.up)))
        return width * height
    }
}

public enum SeamlessLoopPlaybackPolicy {
    public static func avPlayerActionAtItemEnd(isLoopPlay: Bool, isSeamlessLoopEnabled: Bool) -> AVPlayer.ActionAtItemEnd {
        isLoopPlay && isSeamlessLoopEnabled ? .none : .pause
    }

    public static func avPlayerSeekTolerance(isAccurateSeek: Bool, isLoopRestart: Bool) -> CMTime {
        isAccurateSeek || isLoopRestart ? .zero : .positiveInfinity
    }

    public static func shouldUseMEPlayerPacketQueue(
        isLoopPlay: Bool,
        isSeamlessLoopEnabled: Bool,
        usesAsyncPacketQueue: Bool,
        tracksAlreadyLooping: Bool
    ) -> Bool {
        isLoopPlay && isSeamlessLoopEnabled && usesAsyncPacketQueue && !tracksAlreadyLooping
    }
}

public enum ProgressPreviewThumbnailMode: Equatable, Sendable {
    /// Never generate progress-bar preview thumbnails.
    case disabled
    /// Generate thumbnails only for local file URLs.
    case localOnly
    /// Allow thumbnail generation for any playable URL.
    case always
}

public extension KSOptions {
    nonisolated(unsafe) static var firstPlayerType: MediaPlayerProtocol.Type = KSAVPlayer.self
    nonisolated(unsafe) static var secondPlayerType: MediaPlayerProtocol.Type? = KSMEPlayer.self
    /// 最低缓存视频时间
    nonisolated(unsafe) static var preferredForwardBufferDuration = 3.0
    /// 最大缓存视频时间
    nonisolated(unsafe) static var maxBufferDuration = 30.0
    /// 是否开启秒开
    nonisolated(unsafe) static var isSecondOpen = false
    /// 开启精确seek
    nonisolated(unsafe) static var isAccurateSeek = false
    /// Applies to short videos only
    nonisolated(unsafe) static var isLoopPlay = false
    nonisolated(unsafe) static var isSeamlessLoopEnabled = false
    /// 是否自动播放，默认true
    nonisolated(unsafe) static var isAutoPlay = true
    /// seek完是否自动播放
    nonisolated(unsafe) static var isSeekedAutoPlay = true
    nonisolated(unsafe) static var hardwareDecode = true
    nonisolated(unsafe) static var isDiskPrecacheEnabled = false
    nonisolated(unsafe) static var diskPrecacheMaxFileSize: Int64 = 1_073_741_824
    nonisolated(unsafe) static var diskPrecacheMaxCacheSize: Int64 = 5_368_709_120
    nonisolated(unsafe) static var isMemorySeekCacheEnabled = false
    nonisolated(unsafe) static var memorySeekCacheDuration: TimeInterval = 8
    nonisolated(unsafe) static var memorySeekCacheMaxByteSize = 16 * 1024 * 1024
    nonisolated(unsafe) static var isProgressPreviewEnabled = true
    nonisolated(unsafe) static var progressPreviewThumbnailMode = ProgressPreviewThumbnailMode.localOnly
    nonisolated(unsafe) static var isDefinitionSwitchPrewarmingEnabled = false
    nonisolated(unsafe) static var definitionSwitchPrewarmTimeout: TimeInterval = 8
    nonisolated(unsafe) static var isAdaptiveBitrateSwitchingEnabled = false
    nonisolated(unsafe) static var adaptiveBitrateSwitchingPolicy = KSAdaptiveBitrateSwitchingPolicy()
    nonisolated(unsafe) static var videoColorAdjustment = VideoColorAdjustment.neutral
    nonisolated(unsafe) static var panoramaMode = PanoramaMode.disabled
    nonisolated(unsafe) static var deinterlaceMode = VideoDeinterlaceMode.automatic
    nonisolated(unsafe) static var subtitleCaptionAppearancePolicy = SubtitleCaptionAppearancePolicy.never
    nonisolated(unsafe) static var subtitleHDREffectPolicy = SubtitleHDREffectPolicy.automatic
    nonisolated(unsafe) static var isOfflineSubtitleGenerationEnabled = false
    nonisolated(unsafe) static var offlineSubtitleGenerator: (any AudioRecognize)?
    nonisolated(unsafe) static var onlineSubtitleProviders: [any OnlineSubtitleProvider] = []
    nonisolated(unsafe) static var onlineSubtitleLanguages = ["zh-cn"]
    nonisolated(unsafe) static var audioSpatializationPreference = AudioSpatializationPreference.automatic
    nonisolated(unsafe) static var multichannelAudioPreference = MultichannelAudioPreference.automatic
    // 默认不用自研的硬解，因为有些视频的AVPacket的pts顺序是不对的，只有解码后的AVFrame里面的pts是对的。
    nonisolated(unsafe) static var asynchronousDecompression = false
    nonisolated(unsafe) static var isPipPopViewController = false
    nonisolated(unsafe) static var canStartPictureInPictureAutomaticallyFromInline = true
    nonisolated(unsafe) static var pictureInPictureSubtitlePolicy = PictureInPictureSubtitlePolicy.automatic
    nonisolated(unsafe) static var preferredFrame = true
    nonisolated(unsafe) static var useSystemHTTPProxy = true
    #if !os(macOS)
    /// Optional process-wide default route sharing policy. Nil keeps KSPlayer's platform defaults.
    nonisolated(unsafe) static var audioRouteSharingPolicy: AVAudioSession.RouteSharingPolicy?
    #endif
    /// 日志级别
    nonisolated(unsafe) static var logLevel = LogLevel.warning
    nonisolated(unsafe) static var logger: LogHandler = OSLog(lable: "KSPlayer")
    internal static func deviceCpuCount() -> Int {
        var ncpu = UInt(0)
        var len: size_t = MemoryLayout.size(ofValue: ncpu)
        sysctlbyname("hw.ncpu", &ncpu, &len, nil, 0)
        return Int(ncpu)
    }

    static func setAudioSession(options: KSOptions? = nil) {
        #if os(macOS)
//        try? AVAudioSession.sharedInstance().setRouteSharingPolicy(.longFormAudio)
        #else
        var category = AVAudioSession.sharedInstance().category
        if category != .playAndRecord {
            category = .playback
        }
        #if os(tvOS)
        let defaultPolicy = AVAudioSession.RouteSharingPolicy.longFormAudio
        #else
        let defaultPolicy = AVAudioSession.RouteSharingPolicy.longFormVideo
        #endif
        let policy = options?.audioRouteSharingPolicy ?? audioRouteSharingPolicy ?? defaultPolicy
        try? AVAudioSession.sharedInstance().setCategory(category, mode: .moviePlayback, policy: policy)
        configureMultichannelContentSupport(options: options, sourceChannelCount: nil, isSpatialRoute: nil)
        try? AVAudioSession.sharedInstance().setActive(true)
        #endif
    }

    static func outputNumberOfChannels(channelCount sourceChannelCount: AVAudioChannelCount, options: KSOptions? = nil) -> AVAudioChannelCount {
        let multichannelPreference = options?.multichannelAudioPreference ?? KSOptions.multichannelAudioPreference
        if multichannelPreference == .stereo {
            return 2
        }
        guard sourceChannelCount > 2 else {
            return 2
        }
        #if os(macOS)
        return sourceChannelCount
        #else
        let maximumOutputNumberOfChannels = AVAudioChannelCount(AVAudioSession.sharedInstance().maximumOutputNumberOfChannels)
        let preferredOutputNumberOfChannels = AVAudioChannelCount(AVAudioSession.sharedInstance().preferredOutputNumberOfChannels)
        let isSpatialAudioEnabled = isSpatialAudioEnabled(channelCount: sourceChannelCount, options: options)
        let isUseAudioRenderer = KSOptions.audioPlayerType == AudioRendererPlayer.self
        KSLog("[audio] maximumOutputNumberOfChannels: \(maximumOutputNumberOfChannels), preferredOutputNumberOfChannels: \(preferredOutputNumberOfChannels), isSpatialAudioEnabled: \(isSpatialAudioEnabled), isUseAudioRenderer: \(isUseAudioRenderer), multichannelPreference: \(multichannelPreference)")
        let maxRouteChannelsCount = AVAudioSession.sharedInstance().currentRoute.outputs.compactMap {
            $0.channels?.count
        }.max() ?? 2
        KSLog("[audio] currentRoute max channels: \(maxRouteChannelsCount)")
        var channelCount = sourceChannelCount
        let minChannels = max(AVAudioChannelCount(2), min(maximumOutputNumberOfChannels, sourceChannelCount))
        #if os(tvOS) || targetEnvironment(simulator)
        if !(isUseAudioRenderer && isSpatialAudioEnabled) {
            // Do not use maxRouteChannelsCount here: some multichannel routes initially report 2.
            channelCount = minChannels
        }
        #else
        // iOS device speakers can virtualize Spatial Audio; Bluetooth routes may not.
        if !isSpatialAudioEnabled {
            channelCount = minChannels
        }
        #endif
        KSLog("[audio] outputNumberOfChannels: \(AVAudioSession.sharedInstance().outputNumberOfChannels) output channelCount: \(channelCount)")
        return channelCount
        #endif
    }

    #if !os(macOS)
    static func isSpatialAudioEnabled(channelCount: AVAudioChannelCount, options: KSOptions? = nil) -> Bool {
        if #available(tvOS 15.0, iOS 15.0, *) {
            let isSpatialAudioEnabled = AVAudioSession.sharedInstance().currentRoute.outputs.contains { $0.isSpatialAudioEnabled }
            configureMultichannelContentSupport(options: options, sourceChannelCount: channelCount, isSpatialRoute: isSpatialAudioEnabled)
            return isSpatialAudioEnabled
        } else {
            return false
        }
    }

    private static func configureMultichannelContentSupport(options: KSOptions?, sourceChannelCount: AVAudioChannelCount?, isSpatialRoute: Bool?) {
        guard #available(tvOS 15.0, iOS 15.0, *) else {
            return
        }
        let spatialPreference = options?.audioSpatializationPreference ?? KSOptions.audioSpatializationPreference
        let multichannelPreference = options?.multichannelAudioPreference ?? KSOptions.multichannelAudioPreference
        let supportsMultichannel: Bool
        switch spatialPreference {
        case .automatic:
            supportsMultichannel = multichannelPreference != .stereo && ((isSpatialRoute ?? false) || (sourceChannelCount ?? 0) > 2)
        case .enabled:
            supportsMultichannel = multichannelPreference != .stereo
        case .disabled:
            supportsMultichannel = false
        }
        try? AVAudioSession.sharedInstance().setSupportsMultichannelContent(supportsMultichannel)
        KSLog("[audio] supportsMultichannelContent: \(supportsMultichannel), spatialPreference: \(spatialPreference), multichannelPreference: \(multichannelPreference)")
    }
    #endif
}

public enum LogLevel: Int32, CustomStringConvertible {
    case panic = 0
    case fatal = 8
    case error = 16
    case warning = 24
    case info = 32
    case verbose = 40
    case debug = 48
    case trace = 56

    public var description: String {
        switch self {
        case .panic:
            return "panic"
        case .fatal:
            return "fault"
        case .error:
            return "error"
        case .warning:
            return "warning"
        case .info:
            return "info"
        case .verbose:
            return "verbose"
        case .debug:
            return "debug"
        case .trace:
            return "trace"
        }
    }
}

public extension LogLevel {
    var logType: OSLogType {
        switch self {
        case .panic, .fatal:
            return .fault
        case .error:
            return .error
        case .warning:
            return .debug
        case .info, .verbose, .debug:
            return .info
        case .trace:
            return .default
        }
    }
}

public protocol LogHandler {
    @inlinable
    func log(level: LogLevel, message: CustomStringConvertible, file: String, function: String, line: UInt)
}

public class OSLog: LogHandler {
    public let label: String
    public init(lable: String) {
        label = lable
    }

    @inlinable
    public func log(level: LogLevel, message: CustomStringConvertible, file: String, function: String, line: UInt) {
        os_log(level.logType, "%@ %@: %@:%d %@ | %@", level.description, label, file, line, function, message.description)
    }
}

public class FileLog: LogHandler {
    public let fileHandle: FileHandle
    public let formatter = DateFormatter()
    public init(fileHandle: FileHandle) {
        self.fileHandle = fileHandle
        formatter.dateFormat = "MM-dd HH:mm:ss.SSSSSS"
    }

    @inlinable
    public func log(level: LogLevel, message: CustomStringConvertible, file: String, function: String, line: UInt) {
        let string = String(format: "%@ %@ %@:%d %@ | %@\n", formatter.string(from: Date()), level.description, file, line, function, message.description)
        if let data = string.data(using: .utf8) {
            fileHandle.write(data)
        }
    }
}

@inlinable
public func KSLog(_ error: @autoclosure () -> Error, file: String = #file, function: String = #function, line: UInt = #line) {
    KSLog(level: .error, error().localizedDescription, file: file, function: function, line: line)
}

@inlinable
public func KSLog(level: LogLevel = .warning, _ message: @autoclosure () -> CustomStringConvertible, file: String = #file, function: String = #function, line: UInt = #line) {
    if level.rawValue <= KSOptions.logLevel.rawValue {
        let fileName = (file as NSString).lastPathComponent
        KSOptions.logger.log(level: level, message: message(), file: fileName, function: function, line: line)
    }
}

@inlinable
public func KSLog(level: LogLevel = .warning, dso: UnsafeRawPointer = #dsohandle, _ message: StaticString, _ args: CVarArg...) {
    if level.rawValue <= KSOptions.logLevel.rawValue {
        os_log(level.logType, dso: dso, message, args)
    }
}

public extension Array {
    func toDictionary<Key: Hashable>(with selectKey: (Element) -> Key) -> [Key: Element] {
        var dict = [Key: Element]()
        forEach { element in
            dict[selectKey(element)] = element
        }
        return dict
    }
}

public struct KSClock {
    public private(set) var lastMediaTime = CACurrentMediaTime()
    public internal(set) var position = Int64(0)
    public internal(set) var time = CMTime.zero {
        didSet {
            lastMediaTime = CACurrentMediaTime()
        }
    }

    func getTime() -> TimeInterval {
        time.seconds + CACurrentMediaTime() - lastMediaTime
    }
}

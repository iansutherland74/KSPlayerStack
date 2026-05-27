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
    /// Use PiP-compatible subtitle paths when available.
    ///
    /// AVPlayer can show system-rendered legible media selections in Picture in Picture. KSPlayer-owned
    /// overlay subtitles stay inline unless `burnIn` is selected for the sample-buffer KSMEPlayer path.
    case automatic
    /// Burn KSPlayer-rendered primary/secondary text, external image, and libass bitmap subtitles into
    /// KSMEPlayer sample-buffer frames for system PiP when the renderer can safely do so.
    ///
    /// This is intentionally opt-in because it changes emitted video frames, currently applies only to
    /// SDR CoreGraphics-readable sample buffers, and is not available for native AVPlayer PiP.
    case burnIn
    /// Do not expose native PiP subtitle selection. Overlay subtitles continue to render in the inline player only.
    case disabled
}

public struct PictureInPictureSubtitleDiagnostic: Equatable, Sendable, CustomStringConvertible {
    public enum Status: Equatable, Sendable {
        case disabled
        case nativeLegible
        case inlineOverlayOnly
        case burnedIn
        case burnInUnavailable
    }

    public let status: Status
    public let message: String

    public var description: String { message }
}

enum PictureInPictureSubtitleRenderMode: Equatable {
    case disabled
    case nativeLegible
    case inlineOverlayOnly
    case burnedIn
    case burnInUnavailable
}

enum PictureInPictureSubtitlePolicyResolver {
    private static let inlineOverlayReason = "KSPlayer overlay subtitles are UI layers and are not composited into the AVPlayer layer or sample-buffer frames used by system Picture in Picture."

    static func renderMode(
        policy: PictureInPictureSubtitlePolicy,
        usesNativeLegibleSelection: Bool,
        supportsSampleBufferBurnIn: Bool = false
    ) -> PictureInPictureSubtitleRenderMode {
        guard policy != .disabled else {
            return .disabled
        }
        if policy == .burnIn {
            return supportsSampleBufferBurnIn ? .burnedIn : .burnInUnavailable
        }
        return usesNativeLegibleSelection ? .nativeLegible : .inlineOverlayOnly
    }

    static func diagnostic(
        policy: PictureInPictureSubtitlePolicy,
        usesNativeLegibleSelection: Bool,
        supportsSampleBufferBurnIn: Bool = false,
        burnInUnavailableReason: String? = nil
    ) -> PictureInPictureSubtitleDiagnostic {
        switch renderMode(
            policy: policy,
            usesNativeLegibleSelection: usesNativeLegibleSelection,
            supportsSampleBufferBurnIn: supportsSampleBufferBurnIn
        ) {
        case .disabled:
            return PictureInPictureSubtitleDiagnostic(
                status: .disabled,
                message: "Picture in Picture subtitle selection is disabled by policy."
            )
        case .nativeLegible:
            return PictureInPictureSubtitleDiagnostic(
                status: .nativeLegible,
                message: "Native AVFoundation legible tracks can be shown by system Picture in Picture; KSPlayer overlay subtitles remain inline-only."
            )
        case .inlineOverlayOnly:
            return PictureInPictureSubtitleDiagnostic(
                status: .inlineOverlayOnly,
                message: inlineOverlayReason
            )
        case .burnedIn:
            return PictureInPictureSubtitleDiagnostic(
                status: .burnedIn,
                message: "KSMEPlayer will burn KSPlayer-rendered subtitles into sample-buffer frames while system Picture in Picture is active."
            )
        case .burnInUnavailable:
            let reason = burnInUnavailableReason ?? "the active player path does not expose a subtitle burn-in renderer"
            return PictureInPictureSubtitleDiagnostic(
                status: .burnInUnavailable,
                message: "Picture in Picture subtitle burn-in was requested but is unavailable because \(reason)."
            )
        }
    }
}

public enum KSLowLatencyLiveProfile: Equatable, Sendable {
    /// Opt-in LAN live tuning for glass-to-glass latency-sensitive streams.
    ///
    /// This profile reduces FFmpeg probing, demux buffering, player buffering, and seek/cache features.
    /// It cannot guarantee a fixed latency target; protocol behavior, encoder GOP/B-frames, network jitter,
    /// camera buffering, and device decode capacity still dominate end-to-end latency.
    case lan
}

public struct LowLatencyLiveEncoderSettingsRecommendation: Equatable, Sendable {
    public let preferredCodecs: [String]
    public let maximumGOPDuration: TimeInterval
    public let maximumBFrameCount: Int
    public let disablesLookahead: Bool
    public let usesConstrainedBitrate: Bool
    public let notes: [String]
}

public struct LowLatencyLiveTransportSettingsRecommendation: Equatable, Sendable {
    public let protocolName: String
    public let preferredTransport: String
    public let serverBufferDuration: TimeInterval
    public let rtpReorderQueueSize: Int
    public let usesWallClockTimestamps: Bool
    public let notes: [String]
}

public struct LowLatencyLiveSourceRecommendations: Equatable, Sendable {
    public let profile: KSLowLatencyLiveProfile
    public let encoder: LowLatencyLiveEncoderSettingsRecommendation
    public let rtsp: LowLatencyLiveTransportSettingsRecommendation
    public let rtp: LowLatencyLiveTransportSettingsRecommendation
    public let validationChecklist: [String]
    public let caveats: [String]
}

public extension KSLowLatencyLiveProfile {
    var sourceRecommendations: LowLatencyLiveSourceRecommendations {
        LowLatencyLivePlaybackPolicy.sourceRecommendations(profile: self)
    }
}

public enum VideoProjection: Equatable, Sendable {
    case equirectangular
    case equirectangularTiled
    case cubemap
    case unknown(String)
}

public enum PanoramaStereoLayout: Equatable, Hashable, Sendable {
    case mono
    case sideBySide
    case topAndBottom
}

public typealias StereoscopicVideoLayout = PanoramaStereoLayout

public enum StereoscopicVideoMode: Equatable, Sendable {
    /// Preserve the packed source frame as normal 2D video.
    case disabled
    /// Detect side-by-side or top-and-bottom packing from stream metadata and crop one eye on the Metal path.
    case automatic
    /// Treat the source as left/right packed 3D video.
    case sideBySide
    /// Treat the source as top/bottom packed 3D video.
    case topAndBottom
}

public enum StereoscopicVideoEye: Equatable, Hashable, Sendable {
    case left
    case right
}

public enum PanoramaFieldOfView: Equatable, Hashable, Sendable {
    case degrees180
    case degrees360
}

public struct PanoramaVideoConfiguration: Equatable, Sendable {
    public let projection: VideoProjection
    public let stereoLayout: PanoramaStereoLayout
    public let fieldOfView: PanoramaFieldOfView

    public init(projection: VideoProjection, stereoLayout: PanoramaStereoLayout = .mono, fieldOfView: PanoramaFieldOfView = .degrees360) {
        self.projection = projection
        self.stereoLayout = stereoLayout
        self.fieldOfView = fieldOfView
    }

    var isRenderableInSphere: Bool {
        projection.isRenderableInSphere
    }

    func merging(_ metadataConfiguration: PanoramaVideoConfiguration) -> PanoramaVideoConfiguration {
        PanoramaVideoConfiguration(
            projection: projection,
            stereoLayout: stereoLayout == .mono ? metadataConfiguration.stereoLayout : stereoLayout,
            fieldOfView: metadataConfiguration.fieldOfView
        )
    }
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

public enum AudioMultichannelContentSupportResolver {
    public static func supportsMultichannelContent(
        spatialPreference: AudioSpatializationPreference,
        multichannelPreference: MultichannelAudioPreference,
        sourceChannelCount: AVAudioChannelCount?,
        isSpatialRoute: Bool?
    ) -> Bool {
        guard multichannelPreference != .stereo else {
            return false
        }
        switch spatialPreference {
        case .automatic:
            return isSpatialRoute == true || (sourceChannelCount ?? 0) > 2
        case .enabled:
            return true
        case .disabled:
            return false
        }
    }
}

public enum AudioRouteOutputKind: String, Equatable, Sendable, CustomStringConvertible {
    case builtIn
    case wired
    case bluetooth
    case airPlay
    case hdmi
    case usb
    case carAudio
    case unknown

    public var description: String { rawValue }
}

public enum AudioRouteOutputClassifier {
    public static func outputKind(portTypeRawValue: String) -> AudioRouteOutputKind {
        let normalized = portTypeRawValue.lowercased().filter { $0.isLetter || $0.isNumber }
        if normalized.contains("airplay") {
            return .airPlay
        }
        if normalized.contains("bluetooth") {
            return .bluetooth
        }
        if normalized.contains("hdmi") {
            return .hdmi
        }
        if normalized.contains("usb") {
            return .usb
        }
        if normalized.contains("car") {
            return .carAudio
        }
        if normalized.contains("headphone") || normalized.contains("lineout") || normalized.contains("line") {
            return .wired
        }
        if normalized.contains("builtin") || normalized.contains("speaker") || normalized.contains("receiver") {
            return .builtIn
        }
        return .unknown
    }

    public static func isExternalRoute(_ outputKind: AudioRouteOutputKind) -> Bool {
        switch outputKind {
        case .airPlay, .bluetooth, .hdmi, .usb, .carAudio, .wired:
            return true
        case .builtIn, .unknown:
            return false
        }
    }
}

public enum AudioPlaybackPipeline: String, Equatable, Sendable, CustomStringConvertible {
    case nativeAVPlayer
    case decodedPCM

    public var description: String {
        switch self {
        case .nativeAVPlayer:
            return "native AVPlayer"
        case .decodedPCM:
            return "decoded PCM"
        }
    }
}

public enum EncodedAudioPassthroughAvailability: String, Equatable, Sendable {
    case notApplicable
    case nativeRouteDependent
    case decodedPCMOnly
}

public struct EncodedAudioPassthroughPolicy: Equatable, Sendable {
    public let availability: EncodedAudioPassthroughAvailability
    public let requiresHardwareRouteValidation: Bool
    public let reason: String
}

public enum EncodedAudioPassthroughPolicyResolver {
    public static func isEncodedPassthroughCandidate(mediaSubTypeRawValue: String?) -> Bool {
        guard let mediaSubTypeRawValue else {
            return false
        }
        let token = mediaSubTypeRawValue.lowercased().filter { $0.isLetter || $0.isNumber }
        return ["ec3", "eac3", "ac4", "mlpa", "truehd", "thd"].contains(token)
    }

    public static func isEncodedPassthroughCandidate(metadata: FFmpegAudioCodecMetadata?) -> Bool {
        guard let metadata else {
            return false
        }
        return metadata.isDolbyAtmos || metadata.isDolbyAC4 || metadata.isDolbyTrueHD || metadata.isDolbyEAC3
    }

    public static func policy(
        pipeline: AudioPlaybackPipeline,
        containsEncodedPassthroughCandidate: Bool
    ) -> EncodedAudioPassthroughPolicy {
        guard containsEncodedPassthroughCandidate else {
            return EncodedAudioPassthroughPolicy(
                availability: .notApplicable,
                requiresHardwareRouteValidation: false,
                reason: "No encoded Dolby passthrough candidate was detected in the selected audio track."
            )
        }
        switch pipeline {
        case .nativeAVPlayer:
            return EncodedAudioPassthroughPolicy(
                availability: .nativeRouteDependent,
                requiresHardwareRouteValidation: true,
                reason: "Encoded Atmos, AC-4, and TrueHD passthrough are delegated to Apple's native AVPlayer route and depend on the current hardware output."
            )
        case .decodedPCM:
            return EncodedAudioPassthroughPolicy(
                availability: .decodedPCMOnly,
                requiresHardwareRouteValidation: false,
                reason: "KSMEPlayer decodes supported FFmpeg audio to PCM before output and has no encoded bitstream passthrough path."
            )
        }
    }
}

public struct AudioRouteOutputDiagnostic: Equatable, Sendable {
    public let portName: String
    public let portType: String
    public let outputKind: AudioRouteOutputKind
    public let channelCount: Int?
    public let isSpatialAudioEnabled: Bool?
}

public struct AudioRouteDiagnostic: Equatable, Sendable, CustomStringConvertible {
    public let playbackPipeline: AudioPlaybackPipeline
    public let outputPorts: [AudioRouteOutputDiagnostic]
    public let maximumOutputNumberOfChannels: Int?
    public let preferredOutputNumberOfChannels: Int?
    public let outputNumberOfChannels: Int?
    public let outputLatency: TimeInterval?
    public let routeSharingPolicy: String?
    public let configuredSupportsMultichannelContent: Bool
    public let spatialPreference: AudioSpatializationPreference
    public let multichannelPreference: MultichannelAudioPreference
    public let sourceChannelCount: AVAudioChannelCount?
    public let allowsExternalPlayback: Bool?
    public let usesExternalPlaybackWhileExternalScreenIsActive: Bool?
    public let isExternalPlaybackActive: Bool?
    public let encodedPassthroughPolicy: EncodedAudioPassthroughPolicy

    public var hasExternalOutput: Bool {
        outputPorts.contains { AudioRouteOutputClassifier.isExternalRoute($0.outputKind) }
    }

    public var description: String {
        let ports = outputPorts.map { output in
            let channels = output.channelCount.map { ", channels: \($0)" } ?? ""
            let spatial = output.isSpatialAudioEnabled.map { ", spatial: \($0)" } ?? ""
            return "\(output.portName) (\(output.portType), \(output.outputKind)\(channels)\(spatial))"
        }.joined(separator: "; ")
        return "pipeline: \(playbackPipeline), ports: [\(ports)], routeSharingPolicy: \(routeSharingPolicy ?? "unavailable"), " +
            "supportsMultichannelContent: \(configuredSupportsMultichannelContent), encodedPassthrough: \(encodedPassthroughPolicy.availability.rawValue)"
    }
}

#if !os(macOS)
public enum AudioRouteSharingPolicyResolver {
    public static var platformDefaultPolicy: AVAudioSession.RouteSharingPolicy {
        #if os(tvOS)
        .longFormAudio
        #else
        .longFormVideo
        #endif
    }

    public static func resolvedPolicy(optionPolicy: AVAudioSession.RouteSharingPolicy?, defaultPolicy: AVAudioSession.RouteSharingPolicy?) -> AVAudioSession.RouteSharingPolicy {
        optionPolicy ?? defaultPolicy ?? platformDefaultPolicy
    }
}
#endif

public enum PanoramaProjectionPolicy {
    public static func detectedProjection(metadata: [String: String]) -> VideoProjection? {
        detectedConfiguration(metadata: metadata)?.projection
    }

    public static func detectedConfiguration(metadata: [String: String]) -> PanoramaVideoConfiguration? {
        let normalized = metadata.reduce(into: [String: String]()) { result, entry in
            result[normalizedKey(entry.key)] = normalizedValue(entry.value)
        }

        let projection = detectedProjection(in: normalized)

        guard let projection else {
            return nil
        }

        return PanoramaVideoConfiguration(
            projection: projection,
            stereoLayout: detectedStereoLayout(in: normalized) ?? .mono,
            fieldOfView: detectedFieldOfView(in: normalized)
        )
    }

    public static func detectedProjection(formatDescription: CMFormatDescription?) -> VideoProjection? {
        detectedConfiguration(formatDescription: formatDescription)?.projection
    }

    public static func detectedConfiguration(formatDescription: CMFormatDescription?) -> PanoramaVideoConfiguration? {
        guard let formatDescription,
              let extensions = CMFormatDescriptionGetExtensions(formatDescription) as? [AnyHashable: Any]
        else {
            return nil
        }
        return detectedConfiguration(metadata: flattenedMetadata(extensions))
    }

    public static func resolvedProjection(mode: PanoramaMode, detectedProjection: VideoProjection?) -> VideoProjection? {
        let detectedConfiguration = detectedProjection.map {
            PanoramaVideoConfiguration(projection: $0)
        }
        return resolvedConfiguration(mode: mode, detectedConfiguration: detectedConfiguration)?.projection
    }

    public static func resolvedConfiguration(
        mode: PanoramaMode,
        detectedConfiguration: PanoramaVideoConfiguration?,
        stereoLayout: PanoramaStereoLayout = .mono,
        fieldOfView: PanoramaFieldOfView = .degrees360
    ) -> PanoramaVideoConfiguration? {
        switch mode {
        case .disabled:
            return nil
        case .automatic:
            return detectedConfiguration?.isRenderableInSphere == true ? detectedConfiguration : nil
        case .equirectangular:
            return PanoramaVideoConfiguration(
                projection: .equirectangular,
                stereoLayout: stereoLayout,
                fieldOfView: fieldOfView
            )
        }
    }

    public static func detectedStereoLayout(metadata: [String: String]) -> StereoscopicVideoLayout? {
        let normalized = metadata.reduce(into: [String: String]()) { result, entry in
            result[normalizedKey(entry.key)] = normalizedValue(entry.value)
        }
        return detectedStereoLayout(in: normalized)
    }

    public static func detectedStereoLayout(formatDescription: CMFormatDescription?) -> StereoscopicVideoLayout? {
        guard let formatDescription,
              let extensions = CMFormatDescriptionGetExtensions(formatDescription) as? [AnyHashable: Any]
        else {
            return nil
        }
        return detectedStereoLayout(metadata: flattenedMetadata(extensions))
    }

    private static func detectedProjection(in normalized: [String: String]) -> VideoProjection? {
        for key in ["projection", "projectiontype", "projectionformat", "projectionkind", "spatialmediaprojection", "sphericalprojection"] {
            if let projection = normalized[key].flatMap(projection(from:)) {
                return projection
            }
        }

        for key in ["spherical", "sphericalvideo", "360", "is360", "vr180", "isvr180"] {
            if let value = normalized[key], ["1", "true", "yes", "spherical", "equirectangular", "equirectangular180", "180", "360"].contains(tokenizedValue(value)) {
                return .equirectangular
            }
        }
        return nil
    }

    private static func projection(from value: String) -> VideoProjection? {
        let tokenizedValue = tokenizedValue(value)
        if ["", "0", "false", "no", "none", "flat", "rectilinear"].contains(tokenizedValue) {
            return nil
        }
        if tokenizedValue.contains("tiled") {
            return .equirectangularTiled
        }
        if tokenizedValue.contains("equirectangular") || tokenizedValue.contains("equirect") || tokenizedValue == "spherical" || tokenizedValue == "360" || tokenizedValue == "vr180" {
            return .equirectangular
        }
        if tokenizedValue.contains("cubemap") || tokenizedValue.contains("cube") {
            return .cubemap
        }
        return tokenizedValue.isEmpty ? nil : .unknown(value)
    }

    private static func detectedStereoLayout(in normalized: [String: String]) -> StereoscopicVideoLayout? {
        for key in ["stereomode", "stereolayout", "stereo3d", "st3d", "spatialstereolayout", "sphericalstereomode", "framepacking"] {
            if let layout = normalized[key].flatMap(stereoLayout(from:)) {
                return layout
            }
        }
        return nil
    }

    private static func stereoLayout(from value: String) -> StereoscopicVideoLayout? {
        let tokenizedValue = tokenizedValue(value)
        if ["", "0", "false", "no", "none", "mono", "2d", "flat"].contains(tokenizedValue) {
            return .mono
        }
        if ["sidebyside", "leftright", "sbs", "lr"].contains(tokenizedValue) {
            return .sideBySide
        }
        if ["topbottom", "overunder", "tb", "ou"].contains(tokenizedValue) {
            return .topAndBottom
        }
        return nil
    }

    private static func detectedFieldOfView(in normalized: [String: String]) -> PanoramaFieldOfView {
        for key in ["fieldofview", "fov", "horizontalfieldofview", "hfov", "panoramafieldofview", "meshfieldofview", "sphericaldegrees", "panoramadegrees", "projection", "projectiontype", "projectionformat"] {
            if let fieldOfView = normalized[key].flatMap(fieldOfView(from:)) {
                return fieldOfView
            }
        }
        return .degrees360
    }

    private static func fieldOfView(from value: String) -> PanoramaFieldOfView? {
        let tokenizedValue = tokenizedValue(value)
        if tokenizedValue.contains("180") || tokenizedValue.contains("vr180") || tokenizedValue.contains("half") || tokenizedValue.contains("hemisphere") {
            return .degrees180
        }
        if tokenizedValue.contains("360") || tokenizedValue.contains("full") {
            return .degrees360
        }
        return nil
    }

    private static func flattenedMetadata(_ dictionary: [AnyHashable: Any]) -> [String: String] {
        var metadata = [String: String]()
        flatten(dictionary: dictionary, into: &metadata)
        return metadata
    }

    private static func flatten(dictionary: [AnyHashable: Any], into metadata: inout [String: String]) {
        for (key, value) in dictionary {
            let key = String(describing: key)
            if let dictionary = value as? [AnyHashable: Any] {
                flatten(dictionary: dictionary, into: &metadata)
            } else if let dictionary = value as? NSDictionary {
                flatten(dictionary: dictionary.reduce(into: [AnyHashable: Any]()) { result, entry in
                    if let key = entry.key as? AnyHashable {
                        result[key] = entry.value
                    }
                }, into: &metadata)
            } else if let value = value as? String {
                metadata[key] = value
            } else if let value = value as? NSNumber {
                metadata[key] = value.stringValue
            }
        }
    }

    private static func normalizedKey(_ key: String) -> String {
        key.lowercased().filter { $0.isLetter || $0.isNumber }
    }

    private static func normalizedValue(_ value: String) -> String {
        value.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    }

    private static func tokenizedValue(_ value: String) -> String {
        normalizedValue(value).filter { $0.isLetter || $0.isNumber }
    }
}

public enum StereoscopicVideoPolicy {
    public static func detectedLayout(metadata: [String: String]) -> StereoscopicVideoLayout? {
        PanoramaProjectionPolicy.detectedStereoLayout(metadata: metadata).flatMap(renderableLayout(_:))
    }

    public static func detectedLayout(formatDescription: CMFormatDescription?) -> StereoscopicVideoLayout? {
        PanoramaProjectionPolicy.detectedStereoLayout(formatDescription: formatDescription).flatMap(renderableLayout(_:))
    }

    public static func resolvedLayout(mode: StereoscopicVideoMode, detectedLayout: StereoscopicVideoLayout?) -> StereoscopicVideoLayout? {
        switch mode {
        case .disabled:
            return nil
        case .automatic:
            return detectedLayout.flatMap(renderableLayout(_:))
        case .sideBySide:
            return .sideBySide
        case .topAndBottom:
            return .topAndBottom
        }
    }

    private static func renderableLayout(_ layout: StereoscopicVideoLayout) -> StereoscopicVideoLayout? {
        layout == .mono ? nil : layout
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

enum PanoramaTextureEye: Hashable {
    case mono
    case left
    case right

    init(_ eye: StereoscopicVideoEye) {
        switch eye {
        case .left:
            self = .left
        case .right:
            self = .right
        }
    }
}

extension PanoramaStereoLayout {
    var isStereo: Bool {
        self != .mono
    }

    func textureCoordinateBounds(for eye: PanoramaTextureEye) -> CGRect {
        switch (self, eye) {
        case (.sideBySide, .left):
            return CGRect(x: 0, y: 0, width: 0.5, height: 1)
        case (.sideBySide, .right):
            return CGRect(x: 0.5, y: 0, width: 0.5, height: 1)
        case (.topAndBottom, .left):
            return CGRect(x: 0, y: 0, width: 1, height: 0.5)
        case (.topAndBottom, .right):
            return CGRect(x: 0, y: 0.5, width: 1, height: 0.5)
        default:
            return CGRect(x: 0, y: 0, width: 1, height: 1)
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
    /// KSMEPlayer-only live demux recording destination. Existing files are not overwritten.
    ///
    /// Recording writes to a sibling temporary file while active and moves it into place only after
    /// `stopRecord()` writes the trailer successfully. A crash or process kill can still leave only the
    /// temporary file.
    public var outputURL: URL?
    public var streamRecordingProgressHandler: (@Sendable (StreamRecordingProgress) -> Void)?
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
    /// Optional preferred hardware I/O buffer duration for decoded PCM playback.
    public var preferredAudioIOBufferDuration: TimeInterval?
    public var audioSpatializationPreference = KSOptions.audioSpatializationPreference
    public var multichannelAudioPreference = KSOptions.multichannelAudioPreference
    /// Last observed audio route/capability snapshot. This reports public runtime state only; it is not a hardware validation result.
    @Published
    public internal(set) var audioRouteDiagnostic: AudioRouteDiagnostic?
    #if !os(macOS)
    /// Overrides the AVAudioSession route sharing policy used for playback.
    /// Set this to `.longFormAudio` when an app wants audio-only AirPlay/Wi-Fi routes.
    /// Encoded Dolby passthrough, including any system AC-4 support, is only available through Apple's native playback route.
    /// KSMEPlayer decodes supported audio to PCM before output.
    public var audioRouteSharingPolicy = KSOptions.audioRouteSharingPolicy
    #endif
    // sutile
    public var autoSelectEmbedSubtitle = true
    /// Allows seeking to refresh selected embedded bitmap subtitle tracks; external SUP/PGS URL subtitles are decoded into overlay image cues when selected.
    public var isSeekImageSubtitle = false
    /// Renders embedded ASS/SSA subtitle tracks through libass as authored bitmap overlays when the libass module is available.
    ///
    /// If disabled, or if libass is not linked for the current platform, ASS/SSA tracks use KSPlayer's text parser fallback.
    public var isAssSubtitleImageRenderingEnabled = KSOptions.isAssSubtitleImageRenderingEnabled
    /// Controls whether KSPlayer maps Apple's system caption appearance into rendered subtitles.
    public var subtitleCaptionAppearancePolicy = KSOptions.subtitleCaptionAppearancePolicy
    /// Improves subtitle contrast for HDR video while keeping subtitles as UI overlays.
    public var subtitleHDREffectPolicy = KSOptions.subtitleHDREffectPolicy
    /// Feeds decoded FFmpeg audio frames to an offline subtitle generator and exposes it as a subtitle track.
    /// This routes single-URL playback through `KSMEPlayer`; AVPlayer, separate audio URLs, and wireless-route playback do not expose decoded frames here.
    public var isOfflineSubtitleGenerationEnabled = KSOptions.isOfflineSubtitleGenerationEnabled
    /// Apps can provide an on-device Speech, Translation, or custom local-model generator without bundling credentials or model weights in KSPlayer.
    public var offlineSubtitleGenerator: (any AudioRecognize)? = KSOptions.offlineSubtitleGenerator
    /// Apps can opt in to online subtitle search by supplying providers configured with their own credentials.
    public var onlineSubtitleProviders: [any OnlineSubtitleProvider] = KSOptions.onlineSubtitleProviders
    /// Default languages used by online subtitle providers when a search UI does not pass explicit languages.
    public var onlineSubtitleLanguages = KSOptions.onlineSubtitleLanguages
    /// Enables provider-backed translation for parsed external text subtitles. Disabled unless an app opts in and supplies a provider.
    public var isExternalSubtitleTranslationEnabled = KSOptions.isExternalSubtitleTranslationEnabled
    /// App-supplied translator for external subtitles. KSPlayer does not bundle cloud credentials or model weights.
    public var externalSubtitleTranslationProvider: (any SubtitleTranslationProvider)? = KSOptions.externalSubtitleTranslationProvider
    /// Controls whether translated external subtitles replace text or show bilingual text.
    public var externalSubtitleTranslationDisplayMode = KSOptions.externalSubtitleTranslationDisplayMode
    /// Optional source language hint passed to the translation provider.
    public var externalSubtitleTranslationSourceLanguage = KSOptions.externalSubtitleTranslationSourceLanguage
    /// Optional target language hint passed to the translation provider.
    public var externalSubtitleTranslationTargetLanguage = KSOptions.externalSubtitleTranslationTargetLanguage
    // video
    public var display = DisplayEnum.plane
    /// Controls automatic or forced routing of equirectangular 360-degree video into the Metal sphere renderer.
    public var panoramaMode = KSOptions.panoramaMode
    /// Selects the texture packing used by stereoscopic panorama videos when metadata is missing or forced rendering is used.
    public var panoramaStereoLayout = KSOptions.panoramaStereoLayout
    /// Selects whether equirectangular panorama content covers a front 180-degree hemisphere or a full 360-degree sphere.
    public var panoramaFieldOfView = KSOptions.panoramaFieldOfView
    /// Controls packed flat 3D video rendering independently from panorama mode.
    public var stereoscopicVideoMode = KSOptions.stereoscopicVideoMode
    /// Selects which eye is shown when flat side-by-side/top-and-bottom 3D is rendered on a normal 2D display.
    public var stereoscopicVideoEye = KSOptions.stereoscopicVideoEye
    /// Resolved texture packing for the selected video track. Non-mono values force the Metal path for eye cropping.
    public internal(set) var stereoscopicVideoLayout = StereoscopicVideoLayout.mono
    /// Opt-in conversion of ordinary 2D video into a pseudo-stereo presentation on the Metal path.
    public var video2DTo3DMode = KSOptions.video2DTo3DMode
    /// Horizontal disparity strength for 2D-to-3D conversion. Values are clamped to `0 ... 1` at render time.
    public var video2DTo3DDepthStrength = KSOptions.video2DTo3DDepthStrength
    /// Inter-eye distance scale for generated 2D-to-3D parallax. Values are clamped to `0 ... 2` at render time.
    public var video2DTo3DDepthDistance = KSOptions.video2DTo3DDepthDistance
    /// Curvature applied to normalized depth around the neutral plane. Values are clamped to `0.25 ... 3` at render time.
    public var video2DTo3DDepthCurvature = KSOptions.video2DTo3DDepthCurvature
    /// Temporal smoothing factor for app-provided depth textures. Values are clamped to `0 ... 0.95` at render time.
    public var video2DTo3DDepthSmoothingFactor = KSOptions.video2DTo3DDepthSmoothingFactor
    /// Output policy for generated stereo: selected eye for normal displays, or packed side-by-side/top-and-bottom frames.
    public var video2DTo3DOutputLayout = KSOptions.video2DTo3DOutputLayout
    /// Latest 2D-to-3D availability diagnostic. 2D-to-3D conversion is Vision Pro-only; regular stereoscopic and panorama rendering are separate.
    @Published
    public internal(set) var video2DTo3DDiagnostic: Video2DTo3DDiagnostic?
    /// Optional app-owned depth provider, for example a private Vision Pro Depth Anything Core ML or ONNX adapter.
    public var videoDepthEstimationProvider: (any VideoDepthEstimationProvider)?
    /// Optional app-owned callback for render-path timing diagnostics.
    public var video2DTo3DRenderMetricsHandler: (@MainActor @Sendable (Video2DTo3DRenderMetrics) -> Void)?
    /// Optional decoded-frame callback for the KSMEPlayer path. Frames are delivered off the decode/render thread.
    public var videoFrameOutput: KSVideoFrameOutput?
    /// When true, `KSPlayerLayer` routes playback through `KSMEPlayer` so decoded `CVPixelBuffer`s are available.
    public var requiresDecodedVideoFrameOutput = false
    public var videoDelay = 0.0 // s
    /// When true, `videoClockSync` never drops frames (used while CompositorLayer immersive stereo is active).
    public var relaxVideoClockSyncWhileImmersiveCompositorActive = false
    /// When true, the flat `MetalPlayView` consumes frames but does not present them (immersive compositor only).
    public var suppressWindowVideoPresentationWhileImmersiveCompositorActive = false
    /// Updated by the Vision Pro demo while immersive is active; used to gate `KSVideoFrameOutput` to the audio clock.
    public var immersiveAudioPlaybackSeconds: TimeInterval = 0
    /// Delivers audio-synced video frames from `MetalPlayView` to the immersive compositor (not decode-live-edge).
    public var immersivePresentVideoFrame: (@Sendable (CVPixelBuffer, TimeInterval) -> Void)?
    /// Controls MEPlayer deinterlacing on the FFmpeg software filter path.
    /// Native AVPlayer output keeps Apple's system-managed handling.
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
    /// GPU-side VideoToolbox upscaling for the MEPlayer/Metal path. `.none` preserves the native AVPlayer path.
    public var videoUpscaling = VideoUpscalingMode.none
    /// Last observed upscaling state for the MEPlayer/Metal renderer.
    @Published
    public internal(set) var videoUpscalingState = VideoUpscalingState.inactive
    /// GPU-side SDR color controls for the Metal renderer. Neutral defaults preserve existing output.
    public var videoColorAdjustment = KSOptions.videoColorAdjustment
    /// Controls how HDR10+ dynamic metadata is treated when KSPlayer owns FFmpeg/Metal playback.
    ///
    /// The default preserves the system display path when that path is otherwise selected. Apps can choose
    /// `.metalDynamicToneMapping` to force KSPlayer's conservative Metal knee/rolloff shader when structured metadata is
    /// available, or `.staticHDR10Fallback` to force the explicit HDR10 fallback.
    public var hdr10PlusToneMappingPolicy = KSOptions.hdr10PlusToneMappingPolicy
    /// Last observed Dolby Vision playback capability for the selected video track.
    /// General builds keep the explicit HDR10 base-layer fallback unless `dolbyVisionFELPlaybackPolicy` requires composition.
    @Published
    public internal(set) var dolbyVisionPlaybackDiagnostic: DolbyVisionPlaybackDiagnostic?
    /// Controls Profile 7 FEL behavior when full composition is unavailable.
    public var dolbyVisionFELPlaybackPolicy = KSOptions.dolbyVisionFELPlaybackPolicy
    /// Optional public/open FEL compositor backend. KSPlayer does not ship one today; this hook is for future
    /// implementations that can prove correct BL+EL+RPU residual reconstruction without private APIs.
    public var dolbyVisionFELCompositorBackend: (any DolbyVisionFELCompositorBackend)?
    public var dolbyVisionFELCompositorAvailability: DolbyVisionFELCompositorAvailability {
        dolbyVisionFELCompositorBackend?.availability
            ?? .unavailable(reason: DolbyVisionPlaybackDiagnostic.missingOpenFELCompositorReason)
    }

    /// Shows a floating time bubble while hovering or scrubbing the progress bar.
    public var isProgressPreviewEnabled = KSOptions.isProgressPreviewEnabled
    /// Controls whether progress previews may warm thumbnail images in the background.
    public var progressPreviewThumbnailMode = KSOptions.progressPreviewThumbnailMode
    /// Prepares the next definition/source before swapping so VOD quality changes can hand off with minimal delay.
    public var isDefinitionSwitchPrewarmingEnabled = KSOptions.isDefinitionSwitchPrewarmingEnabled
    /// Maximum time to wait for a prewarmed definition/source before falling back to the normal replace path.
    public var definitionSwitchPrewarmTimeout = KSOptions.definitionSwitchPrewarmTimeout
    /// Automatically switches among separate KSPlayerResource definitions based on throughput and buffer health.
    ///
    /// HLS/DASH/other adaptive streaming manifests are left to the native player or FFmpeg demuxer.
    public var isAdaptiveBitrateSwitchingEnabled = KSOptions.isAdaptiveBitrateSwitchingEnabled
    public var adaptiveBitrateSwitchingPolicy = KSOptions.adaptiveBitrateSwitchingPolicy
    public var syncDecodeVideo = false
    /// Enables hardware-backed video decode when the selected player path and runtime codec support allow it.
    /// Unsupported codecs or platforms fall back to FFmpeg software decode.
    public var hardwareDecode = KSOptions.hardwareDecode
    /// Enables KSPlayer's direct asynchronous VideoToolbox decode path for eligible compressed video packets.
    public var asynchronousDecompression = KSOptions.asynchronousDecompression
    public var videoDisable = false
    /// Allows supported iOS/iPadOS players to enter system Picture in Picture automatically from inline playback.
    public var canStartPictureInPictureAutomaticallyFromInline = KSOptions.canStartPictureInPictureAutomaticallyFromInline
    /// Controls subtitle behavior for Picture in Picture. Overlay subtitle burn-in is opt-in and limited to supported KSMEPlayer sample-buffer frames.
    public var pictureInPictureSubtitlePolicy = KSOptions.pictureInPictureSubtitlePolicy
    /// Last resolved PiP subtitle capability for the active player path.
    @Published
    public internal(set) var pictureInPictureSubtitleDiagnostic: PictureInPictureSubtitleDiagnostic?
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
        LowLatencyLivePlaybackPolicy.applyProtocolOptions(profile: lowLatencyLiveProfile, scheme: scheme, to: self)
        appendProtocolWhitelistEntries(Self.protocolWhitelistEntries(for: scheme))
    }

    private static func protocolWhitelistEntries(for scheme: String) -> [String] {
        switch scheme {
        case "rtmp", "rtp":
            return [scheme, "tcp", "udp"]
        case "rtmps":
            return ["rtmps", "rtmp", "tcp", "tls", "crypto"]
        case "rtsp":
            return ["rtsp", "rtp", "tcp", "udp", "http", "https", "tls"]
        case "nfs":
            return ["nfs", "tcp", "udp"]
        case "smb":
            return ["smb", "smb2", "tcp"]
        case "smb2":
            return ["smb2", "smb", "tcp"]
        case "upnp", "dlna":
            return [scheme, "upnp", "dlna", "http", "https", "tcp", "udp", "tls"]
        case "ftp", "sftp":
            return [scheme, "tcp"]
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
        if let lowLatencyLiveProfile {
            return LowLatencyLivePlaybackPolicy.frameCapacity(profile: lowLatencyLiveProfile, fps: fps, naturalSize: naturalSize, isLive: isLive)
        }
        return HighPerformanceVideoPlaybackPolicy.frameCapacity(fps: fps, naturalSize: naturalSize, isLive: isLive)
    }

    open func audioFrameMaxCount(fps: Float, channelCount: Int) -> UInt8 {
        if let lowLatencyLiveProfile {
            return LowLatencyLivePlaybackPolicy.audioFrameCapacity(profile: lowLatencyLiveProfile, fps: fps, channelCount: channelCount)
        }
        let count = (Int(fps) * channelCount) >> 2
        if count >= UInt8.max {
            return UInt8.max
        } else {
            return UInt8(count)
        }
    }

    open func asyncPacketQueueMaxCount(mediaType: AVMediaType, frameCapacity: UInt8) -> Int? {
        guard let lowLatencyLiveProfile else {
            return nil
        }
        return LowLatencyLivePlaybackPolicy.packetQueueCapacity(profile: lowLatencyLiveProfile, mediaType: mediaType, frameCapacity: frameCapacity)
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
        display == .plane &&
            stereoscopicVideoLayout == .mono &&
            !Video2DTo3DPolicy.requiresMetalRenderPath(mode: video2DTo3DMode) &&
            !videoColorAdjustment.shouldApply(dynamicRange: dynamicRange)
    }

    open func isUseDisplayLayer(dynamicRange: DynamicRange?, hasHDR10PlusMetadata: Bool) -> Bool {
        let usesDisplayLayer = isUseDisplayLayer(dynamicRange: dynamicRange)
        guard hasHDR10PlusMetadata else {
            return usesDisplayLayer
        }
        switch hdr10PlusToneMappingPolicy {
        case .systemManagedWhenAvailable:
            return usesDisplayLayer
        case .metalDynamicToneMapping, .staticHDR10Fallback:
            return false
        }
    }

    open func hdr10PlusPlaybackDiagnostic(
        hasHDR10PlusMetadata: Bool,
        usesDisplayLayer: Bool,
        metadata: HDR10PlusMetadata? = nil
    ) -> HDR10PlusPlaybackDiagnostic? {
        guard hasHDR10PlusMetadata else {
            return nil
        }
        return HDR10PlusPlaybackDiagnostic(
            renderPath: usesDisplayLayer ? .systemDisplayLayer : .metalRenderer,
            toneMappingPolicy: hdr10PlusToneMappingPolicy,
            metadata: metadata
        )
    }

    open func hdr10PlusMetalToneMappingUniform(
        metadata: HDR10PlusMetadata?,
        dynamicRange: DynamicRange?
    ) -> HDR10PlusMetalToneMappingUniform? {
        guard hdr10PlusToneMappingPolicy == .metalDynamicToneMapping,
              dynamicRange == .hdr10,
              let metadata
        else {
            return nil
        }
        return HDR10PlusMetalToneMappingUniform(metadata: metadata)
    }

    open func video2DTo3DRenderConfiguration(hasDepthMap: Bool) -> Video2DTo3DRenderConfiguration {
        if let reason = Video2DTo3DPolicy.unavailableReason(mode: video2DTo3DMode) {
            video2DTo3DDiagnostic = .unavailable(reason: reason)
        } else if video2DTo3DDiagnostic?.description == Video2DTo3DPolicy.unavailablePlatformReason {
            video2DTo3DDiagnostic = nil
        }
        return Video2DTo3DPolicy.renderConfiguration(
            mode: video2DTo3DMode,
            depthStrength: video2DTo3DDepthStrength,
            depthDistance: video2DTo3DDepthDistance,
            depthCurvature: video2DTo3DDepthCurvature,
            depthSmoothingFactor: video2DTo3DDepthSmoothingFactor,
            outputLayout: video2DTo3DOutputLayout,
            selectedEye: stereoscopicVideoEye,
            display: display,
            stereoscopicVideoLayout: stereoscopicVideoLayout,
            hasDepthMap: hasDepthMap
        )
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
    private var managedDeinterlaceFilters = [String]()
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
            if let track = assetTrack as? FFmpegAssetTrack {
                dolbyVisionPlaybackDiagnostic = track.dolbyVisionPlaybackDiagnostic ?? track.dovi.map {
                    DolbyVisionPlaybackDiagnostic(configuration: $0, felPlaybackPolicy: dolbyVisionFELPlaybackPolicy)
                }
            } else {
                dolbyVisionPlaybackDiagnostic = assetTrack.dovi.map {
                    DolbyVisionPlaybackDiagnostic(configuration: $0, felPlaybackPolicy: dolbyVisionFELPlaybackPolicy)
                }
            }
            removeManagedDeinterlaceFilters()
            if display == .plane {
                let detectedConfiguration = (assetTrack as? FFmpegAssetTrack)?.panoramaConfiguration
                    ?? PanoramaProjectionPolicy.detectedConfiguration(formatDescription: assetTrack.formatDescription)
                let configuration = PanoramaProjectionPolicy.resolvedConfiguration(
                    mode: panoramaMode,
                    detectedConfiguration: detectedConfiguration,
                    stereoLayout: panoramaStereoLayout,
                    fieldOfView: panoramaFieldOfView
                )
                if configuration?.isRenderableInSphere == true {
                    panoramaStereoLayout = configuration?.stereoLayout ?? .mono
                    panoramaFieldOfView = configuration?.fieldOfView ?? .degrees360
                    display = panoramaStereoLayout.isStereo ? .vrBox : .vr
                }
            }
            let detectedStereoLayout = (assetTrack as? FFmpegAssetTrack)?.stereoscopicVideoLayout
                ?? StereoscopicVideoPolicy.detectedLayout(formatDescription: assetTrack.formatDescription)
            stereoscopicVideoLayout = StereoscopicVideoPolicy.resolvedLayout(
                mode: stereoscopicVideoMode,
                detectedLayout: detectedStereoLayout
            ) ?? .mono
            let decision = VideoDeinterlacePolicy.decision(
                mode: deinterlaceMode,
                fieldOrder: assetTrack.fieldOrder,
                formatDescription: assetTrack.formatDescription,
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
                    managedDeinterlaceFilters.append(filter)
                }
                if decision.doublesFrameRate {
                    assetTrack.nominalFrameRate = assetTrack.nominalFrameRate * 2
                }
            } else if let reason = decision.skipReason {
                KSLog(level: .debug, "[video] deinterlace skipped: \(reason)")
            }
            if !decision.shouldApplyFilter {
                HighPerformanceVideoPlaybackPolicy.apply(to: self, fps: assetTrack.nominalFrameRate, naturalSize: assetTrack.naturalSize)
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

    private func removeManagedDeinterlaceFilters() {
        for filter in managedDeinterlaceFilters {
            if let index = videoFilters.firstIndex(of: filter) {
                videoFilters.remove(at: index)
            }
        }
        managedDeinterlaceFilters.removeAll()
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
        updateVideo(refreshRate: refreshRate, dynamicRange: isDovi ? .dolbyVision : formatDescription?.dynamicRange)
    }

    @MainActor
    open func updateVideo(refreshRate: Float, dynamicRange: DynamicRange?) {
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
        if let dynamicRange {
            displayManager.preferredDisplayCriteria = AVDisplayCriteria(refreshRate: refreshRate, videoDynamicRange: dynamicRange.rawValue)
        }
        #endif
    }

    open func videoClockSync(main: KSClock, nextVideoTime: TimeInterval, fps: Double, frameCount: Int) -> (Double, ClockProcessType) {
        let desire = main.getTime() - videoDelay
        let diff = nextVideoTime - desire
        if relaxVideoClockSyncWhileImmersiveCompositorActive {
            videoClockDelayCount = 0
            return (diff, .next)
        }
        // Depth/ML may use KSVideoFrameOutput, but immersive video must stay on the audio clock.
        if suppressWindowVideoPresentationWhileImmersiveCompositorActive,
           immersivePresentVideoFrame != nil
        {
            // Fall through to normal A/V sync below.
        } else if videoFrameOutput != nil || requiresDecodedVideoFrameOutput {
            videoClockDelayCount = 0
            return (diff, .next)
        }
//        print("[video] video diff \(diff) nextVideoTime \(nextVideoTime) main \(main.time.seconds)")
        if diff >= 1 / fps / 2 {
            videoClockDelayCount = 0
            return (diff, .remain)
        } else {
            if diff < -4 / fps {
                videoClockDelayCount += 1
                if let lowLatencyLiveProfile {
                    let type = LowLatencyLivePlaybackPolicy.clockProcessType(
                        profile: lowLatencyLiveProfile,
                        diff: diff,
                        fps: fps,
                        frameCount: frameCount
                    )
                    if case .next = type {
                        return (diff, .next)
                    }
                    KSLog("[video] low latency live delay=\(diff), frameCount=\(frameCount), action=\(type)")
                    return (diff, type)
                }
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

    static func detectedInterlacingType(formatDescription: CMFormatDescription?) -> VideoInterlacingType? {
        detectedInterlacingType(fieldOrder: FFmpegFieldOrder.detected(formatDescription: formatDescription))
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
        formatDescription: CMFormatDescription? = nil,
        fps: Float,
        naturalSize: CGSize,
        yadifMode: Int,
        addIdet: Bool
    ) -> VideoDeinterlaceDecision {
        let detectedType = detectedInterlacingType(fieldOrder: fieldOrder)
            ?? detectedInterlacingType(formatDescription: formatDescription)
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

public enum VideoUpscalingHDRPolicy: Equatable, Sendable {
    /// Keep HDR and Dolby Vision on the existing system-managed path.
    case preserveHDR
    /// Allow upscaling for HDR frames when the runtime scaler accepts the source.
    case allowHDR
}

/// Last observed VideoToolbox upscaling state for the MEPlayer/Metal renderer.
///
/// `KSOptions.videoUpscalingState` is updated by the renderer on the main thread. The state remains `.inactive`
/// for AVPlayer/native playback because AVPlayer does not expose the per-frame output required by this scaler.
public enum VideoUpscalingState: Equatable, Sendable {
    /// Upscaling is disabled, not yet attempted, or reset after a source/lifecycle change.
    case inactive
    /// The last rendered frame was upscaled.
    case active(sourceSize: CGSize, outputSize: CGSize, scaleFactor: Float)
    /// Upscaling was requested but skipped by policy or rejected by the current runtime/source.
    case unavailable(reason: String)

    public var isActive: Bool {
        if case .active = self {
            return true
        }
        return false
    }

    public var unavailableReason: String? {
        if case let .unavailable(reason) = self {
            return reason
        }
        return nil
    }
}

public enum VideoUpscalingMode: Equatable, Sendable {
    public static let defaultScaleFactor: Float = 2
    public static let scaleFactorRange: ClosedRange<Float> = 1 ... 4

    case none
    /// Uses VideoToolbox low-latency super-resolution when available.
    ///
    /// The requested scale is clamped to `scaleFactorRange`, then rounded down to the closest runtime-supported scale for the source size.
    /// HDR/Dolby Vision is preserved by default because unsupported conversions can silently change output appearance.
    /// Enabling this mode routes playback through `KSMEPlayer`/Metal; AVPlayer and wireless-route playback cannot apply it.
    case appleSuperResolution(scaleFactor: Float = Self.defaultScaleFactor, hdrPolicy: VideoUpscalingHDRPolicy = .preserveHDR)

    public var isEnabled: Bool {
        self != .none
    }

    public var requestedScaleFactor: Float {
        switch self {
        case .none:
            return Self.defaultScaleFactor
        case let .appleSuperResolution(scaleFactor, _):
            return Self.validatedScaleFactor(scaleFactor)
        }
    }

    var hdrPolicy: VideoUpscalingHDRPolicy {
        switch self {
        case .none:
            return .preserveHDR
        case let .appleSuperResolution(_, hdrPolicy):
            return hdrPolicy
        }
    }

    public static func validatedScaleFactor(_ scaleFactor: Float) -> Float {
        guard scaleFactor.isFinite else {
            return defaultScaleFactor
        }
        return min(max(scaleFactor, scaleFactorRange.lowerBound), scaleFactorRange.upperBound)
    }
}

public struct VideoColorAdjustment: Equatable, Sendable {
    public static let defaultSaturation: Float = 1
    public static let defaultBrightness: Float = 0
    public static let defaultContrast: Float = 1

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

    public init(
        saturation: Float = Self.defaultSaturation,
        brightness: Float = Self.defaultBrightness,
        contrast: Float = Self.defaultContrast,
        hdrPolicy: HDRPolicy = .preserveHDR
    ) {
        self.saturation = Self.clamp(saturation, to: Self.saturationRange, defaultValue: Self.defaultSaturation)
        self.brightness = Self.clamp(brightness, to: Self.brightnessRange, defaultValue: Self.defaultBrightness)
        self.contrast = Self.clamp(contrast, to: Self.contrastRange, defaultValue: Self.defaultContrast)
        self.hdrPolicy = hdrPolicy
    }

    public var isNeutral: Bool {
        saturation == Self.defaultSaturation && brightness == Self.defaultBrightness && contrast == Self.defaultContrast
    }

    public func shouldApply(dynamicRange: DynamicRange?) -> Bool {
        guard !isNeutral else {
            return false
        }
        return hdrPolicy == .allowHDR || dynamicRange?.isHDR != true
    }

    private static func clamp(_ value: Float, to range: ClosedRange<Float>, defaultValue: Float) -> Float {
        guard value.isFinite else {
            return defaultValue
        }
        return min(max(value, range.lowerBound), range.upperBound)
    }
}

public enum HighPerformanceVideoPlaybackPolicy {
    public static let highFrameRateThreshold: Float = 90
    public static let eightKPixelThreshold = 7_680 * 4_320

    public static func isHighWorkload(fps: Float, naturalSize: CGSize) -> Bool {
        normalizedFPS(fps) >= highFrameRateThreshold || pixelCount(naturalSize) >= eightKPixelThreshold
    }

    public static func frameCapacity(fps: Float, naturalSize: CGSize, isLive: Bool) -> UInt8 {
        guard isHighWorkload(fps: fps, naturalSize: naturalSize) else {
            return isLive ? 4 : 16
        }
        if isLive {
            return 8
        }
        let isEightKWorkload = pixelCount(naturalSize) >= eightKPixelThreshold
        let normalizedFPS = normalizedFPS(fps)
        if isEightKWorkload, normalizedFPS < highFrameRateThreshold {
            return 12
        }
        let highFPSCapacity = Int(ceil(normalizedFPS / 4))
        return UInt8(min(isEightKWorkload ? 16 : 32, max(16, highFPSCapacity)))
    }

    public static func displayFrameRateRange(fps: Float) -> (minimum: Float, maximum: Float, preferred: Float) {
        let normalizedFPS = normalizedFPS(fps)
        let preferred = max(1, ceil(normalizedFPS))
        if normalizedFPS >= highFrameRateThreshold {
            return (minimum: min(60, preferred / 2), maximum: preferred, preferred: preferred)
        }
        return (minimum: preferred, maximum: preferred * 2, preferred: preferred)
    }

    public static func shouldApplyUpscaling(mode: VideoUpscalingMode, sourceSize: CGSize, fps: Float, dynamicRange: DynamicRange? = nil) -> Bool {
        upscalingSkipReason(mode: mode, sourceSize: sourceSize, fps: fps, dynamicRange: dynamicRange) == nil
    }

    public static func upscalingSkipReason(mode: VideoUpscalingMode, sourceSize: CGSize, fps: Float, dynamicRange: DynamicRange? = nil) -> String? {
        guard mode.isEnabled else {
            return "disabled"
        }
        guard mode.requestedScaleFactor > 1 else {
            return "scale factor is 1x"
        }
        guard isKnownSourceSize(sourceSize) else {
            return "unknown source size"
        }
        if dynamicRange?.isHDR == true, mode.hdrPolicy == .preserveHDR {
            return "HDR output is preserved"
        }
        if isHighWorkload(fps: fps, naturalSize: sourceSize) {
            return "high workload"
        }
        return nil
    }

    public static func apply(to options: KSOptions, fps: Float, naturalSize: CGSize) {
        guard isHighWorkload(fps: fps, naturalSize: naturalSize) else {
            return
        }
        options.syncDecodeVideo = false
        if options.hardwareDecode {
            options.asynchronousDecompression = true
        }
    }

    private static func pixelCount(_ size: CGSize) -> Int {
        let width = normalizedDimension(size.width)
        let height = normalizedDimension(size.height)
        if width == Int.max || height == Int.max {
            return Int.max
        }
        let (count, overflow) = width.multipliedReportingOverflow(by: height)
        return overflow ? Int.max : count
    }

    private static func normalizedDimension(_ value: CGFloat) -> Int {
        guard value.isFinite else {
            return value > 0 ? Int.max : 0
        }
        return max(0, Int(value.rounded(.up)))
    }

    private static func normalizedFPS(_ fps: Float) -> Float {
        guard fps.isFinite, fps > 0 else {
            return 24
        }
        return fps
    }

    private static func isKnownSourceSize(_ size: CGSize) -> Bool {
        size.width.isFinite && size.height.isFinite && size.width > 0 && size.height > 0
    }
}

enum LowLatencyLivePlaybackPolicy {
    private static let fourKPixelThreshold = 3_840 * 2_160
    private static let lanPreferredForwardBufferDuration = 0.12
    private static let lanMaxBufferDuration = 0.5
    private static let lanProbeSize: Int64 = 32 * 1024
    private static let lanMaxAnalyzeDuration: Int64 = 100 * 1000
    private static let lanMaxDelay = 100 * 1000
    private static let lanReadWriteTimeout = 2 * 1000 * 1000
    private static let lanPreferredAudioIOBufferDuration = 0.005
    private static let lanVideoPacketQueueFloor = 6
    private static let lanAudioPacketQueueCapacity = 4

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
            options.videoAdaptable = false
            options.preferredAudioIOBufferDuration = options.preferredAudioIOBufferDuration ?? lanPreferredAudioIOBufferDuration
            options.probesize = options.probesize ?? lanProbeSize
            options.maxAnalyzeDuration = options.maxAnalyzeDuration ?? lanMaxAnalyzeDuration
            setDefaultFormatOption("max_delay", value: lanMaxDelay, options: options)
            setDefaultFormatOption("rw_timeout", value: lanReadWriteTimeout, options: options)
            setDefaultFormatOption("timeout", value: lanReadWriteTimeout, options: options)
            appendListOption("fflags", value: "nobuffer", separator: "+", options: options)
            appendListOption("avioflags", value: "direct", separator: "+", options: options)
            appendListOption("flags", value: "low_delay", separator: "+", options: &options.decoderOptions)
        }
    }

    static func applyProtocolOptions(profile: KSLowLatencyLiveProfile?, scheme: String, to options: KSOptions) {
        guard profile == .lan else {
            return
        }
        switch scheme {
        case "rtsp":
            setDefaultFormatOption("rtsp_transport", value: "udp", options: options)
            setDefaultFormatOption("stimeout", value: lanReadWriteTimeout, options: options)
            setDefaultFormatOption("reorder_queue_size", value: 0, options: options)
        case "rtp", "udp":
            setDefaultFormatOption("fifo_size", value: 50 * 188, options: options)
            setDefaultFormatOption("overrun_nonfatal", value: 1, options: options)
        default:
            break
        }
    }

    static func frameCapacity(profile: KSLowLatencyLiveProfile, fps: Float, naturalSize: CGSize, isLive: Bool) -> UInt8 {
        switch profile {
        case .lan:
            guard isLive else {
                return HighPerformanceVideoPlaybackPolicy.frameCapacity(fps: fps, naturalSize: naturalSize, isLive: false)
            }
            return min(HighPerformanceVideoPlaybackPolicy.frameCapacity(fps: fps, naturalSize: naturalSize, isLive: true), 6)
        }
    }

    static func audioFrameCapacity(profile: KSLowLatencyLiveProfile, fps _: Float, channelCount _: Int) -> UInt8 {
        switch profile {
        case .lan:
            return 4
        }
    }

    static func packetQueueCapacity(profile: KSLowLatencyLiveProfile, mediaType: AVMediaType, frameCapacity: UInt8) -> Int {
        switch profile {
        case .lan:
            if mediaType == .video {
                return max(lanVideoPacketQueueFloor, Int(frameCapacity) * 2)
            }
            if mediaType == .audio {
                return lanAudioPacketQueueCapacity
            }
            return Int(frameCapacity)
        }
    }

    static func clockProcessType(profile: KSLowLatencyLiveProfile, diff: Double, fps: Double, frameCount: Int) -> ClockProcessType {
        switch profile {
        case .lan:
            let normalizedFPS = fps.isFinite && fps > 0 ? fps : 60
            let frameInterval = 1 / normalizedFPS
            if diff < -0.5, frameCount <= 1 {
                return .dropGOPPacket
            }
            if diff < -0.25, frameCount > 2 {
                return .flush
            }
            if diff < -2 * frameInterval {
                return .dropNextFrame
            }
            return .next
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

    static func audioLatencyEstimate(profile: KSLowLatencyLiveProfile, audioFrameCount: Int, audioFPS: Float, preferredAudioIOBufferDuration: TimeInterval?) -> TimeInterval? {
        guard profile == .lan else {
            return nil
        }
        let queueDuration: TimeInterval?
        if audioFrameCount > 0, audioFPS.isFinite, audioFPS > 0 {
            queueDuration = TimeInterval(audioFrameCount) / TimeInterval(audioFPS)
        } else {
            queueDuration = nil
        }
        let ioBufferDuration = preferredAudioIOBufferDuration.flatMap { duration in
            duration.isFinite && duration > 0 ? duration : nil
        }
        switch (queueDuration, ioBufferDuration) {
        case let (queueDuration?, ioBufferDuration?):
            return queueDuration + ioBufferDuration
        case let (queueDuration?, nil):
            return queueDuration
        case let (nil, ioBufferDuration?):
            return ioBufferDuration
        case (nil, nil):
            return nil
        }
    }

    static func sourceRecommendations(profile: KSLowLatencyLiveProfile) -> LowLatencyLiveSourceRecommendations {
        switch profile {
        case .lan:
            return LowLatencyLiveSourceRecommendations(
                profile: profile,
                encoder: LowLatencyLiveEncoderSettingsRecommendation(
                    preferredCodecs: ["H.264 hardware profile supported by the target device", "HEVC only after device decode validation"],
                    maximumGOPDuration: 0.5,
                    maximumBFrameCount: 0,
                    disablesLookahead: true,
                    usesConstrainedBitrate: true,
                    notes: [
                        "Use an all-P or IP-only low-delay GOP; avoid B-frames and encoder lookahead.",
                        "Keep camera-side buffering and scene pre-processing features disabled when latency matters.",
                        "Prefer the lowest resolution, frame rate, and bitrate that satisfy the product requirement before testing 4K."
                    ]
                ),
                rtsp: LowLatencyLiveTransportSettingsRecommendation(
                    protocolName: "RTSP",
                    preferredTransport: "UDP/RTP on trusted LAN; TCP only when packet loss is worse than TCP head-of-line blocking",
                    serverBufferDuration: 0.1,
                    rtpReorderQueueSize: 0,
                    usesWallClockTimestamps: true,
                    notes: [
                        "Disable server-side burst buffering, large jitter buffers, and long client pre-roll.",
                        "Keep RTP packetization mode compatible with the camera and server; validate packet loss on the target switch/Wi-Fi.",
                        "Expose RTP/RTCP timestamps so player metrics can be correlated with camera/server logs."
                    ]
                ),
                rtp: LowLatencyLiveTransportSettingsRecommendation(
                    protocolName: "RTP/UDP",
                    preferredTransport: "Unicast UDP on wired LAN where possible",
                    serverBufferDuration: 0.1,
                    rtpReorderQueueSize: 0,
                    usesWallClockTimestamps: true,
                    notes: [
                        "Avoid RTP reorder queues unless the measured network requires them.",
                        "Size socket/fifo buffers for short jitter absorption, not seconds of backlog.",
                        "Use RTCP sender reports or an equivalent clock source when comparing capture and render timestamps."
                    ]
                ),
                validationChecklist: [
                    "Capture camera time, server ingress/egress time, player diagnostic timestamps, and a high-speed glass-to-glass measurement in the same run.",
                    "Run wired Ethernet before Wi-Fi so encoder/server latency is separated from network jitter.",
                    "Repeat each run after a cold start, a steady-state minute, and a network-loss/jitter injection pass.",
                    "Record device model, OS version, codec, resolution, frame rate, bitrate, GOP length, B-frame count, transport, and server buffer settings."
                ],
                caveats: [
                    "These settings are recommendations for validation, not a latency guarantee.",
                    "Sub-200ms claims require measurement with the actual camera, RTSP/RTP server, network, route, and playback device."
                ]
            )
        }
    }

    private static func setDefaultFormatOption(_ key: String, value: Any, options: KSOptions) {
        if options.formatContextOptions[key] == nil {
            options.formatContextOptions[key] = value
        }
    }

    private static func setDefaultFormatOption(_ key: String, value: Any, options: inout [String: Any]) {
        if options[key] == nil {
            options[key] = value
        }
    }

    private static func appendListOption(_ key: String, value: String, separator: Character, options: KSOptions) {
        appendListOption(key, value: value, separator: separator, options: &options.formatContextOptions)
    }

    private static func appendListOption(_ key: String, value: String, separator: Character, options: inout [String: Any]) {
        guard let existing = options[key] as? String, !existing.isEmpty else {
            options[key] = value
            return
        }
        let values = existing
            .split(whereSeparator: { $0 == separator || $0 == "," || $0 == " " })
            .map(String.init)
        guard !values.contains(value) else {
            return
        }
        options[key] = existing + String(separator) + value
    }

    private static func pixelCount(_ size: CGSize) -> Int {
        let width = max(0, Int(size.width.rounded(.up)))
        let height = max(0, Int(size.height.rounded(.up)))
        return width * height
    }
}

public enum SeamlessLoopPlaybackPolicy {
    public static func shouldManuallyRestartAVPlayer(isLoopPlay: Bool, isSeamlessLoopEnabled: Bool) -> Bool {
        isLoopPlay && !isSeamlessLoopEnabled
    }

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
        tracksAlreadyLooping: Bool,
        canSeekToStart: Bool = true
    ) -> Bool {
        isLoopPlay && isSeamlessLoopEnabled && usesAsyncPacketQueue && !tracksAlreadyLooping && canSeekToStart
    }

    public static func canRestartMEPlayerLoop(isLoopPlay: Bool, duration: TimeInterval, isSeekable: Bool) -> Bool {
        isLoopPlay && duration > 0 && isSeekable
    }
}

public enum ProgressPreviewThumbnailMode: Equatable, Sendable {
    /// Never generate progress-bar preview thumbnails.
    case disabled
    /// Generate thumbnails only for finite, seekable local file URLs.
    case localOnly
    /// Allow thumbnail generation for finite, seekable VOD URLs, including remote URLs.
    ///
    /// Live and DVR streams are still skipped because thumbnail warming performs background seeks.
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
    /// Process-wide default for hardware-backed video decode when the runtime can create a decoder.
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
    nonisolated(unsafe) static var hdr10PlusToneMappingPolicy = HDR10PlusToneMappingPolicy.systemManagedWhenAvailable
    nonisolated(unsafe) static var dolbyVisionFELPlaybackPolicy = DolbyVisionFELPlaybackPolicy.allowBaseLayerFallback
    nonisolated(unsafe) static var panoramaMode = PanoramaMode.disabled
    nonisolated(unsafe) static var panoramaStereoLayout = PanoramaStereoLayout.mono
    nonisolated(unsafe) static var panoramaFieldOfView = PanoramaFieldOfView.degrees360
    nonisolated(unsafe) static var stereoscopicVideoMode = StereoscopicVideoMode.disabled
    nonisolated(unsafe) static var stereoscopicVideoEye = StereoscopicVideoEye.left
    nonisolated(unsafe) static var video2DTo3DMode = Video2DTo3DMode.disabled
    nonisolated(unsafe) static var video2DTo3DDepthStrength = Video2DTo3DPolicy.defaultDepthStrength
    nonisolated(unsafe) static var video2DTo3DDepthDistance = Video2DTo3DPolicy.defaultDepthDistance
    nonisolated(unsafe) static var video2DTo3DDepthCurvature = Video2DTo3DPolicy.defaultDepthCurvature
    nonisolated(unsafe) static var video2DTo3DDepthSmoothingFactor = Video2DTo3DPolicy.defaultDepthSmoothingFactor
    nonisolated(unsafe) static var video2DTo3DOutputLayout = Video2DTo3DOutputLayout.selectedEye
    nonisolated(unsafe) static var deinterlaceMode = VideoDeinterlaceMode.automatic
    nonisolated(unsafe) static var subtitleCaptionAppearancePolicy = SubtitleCaptionAppearancePolicy.never
    nonisolated(unsafe) static var isAssSubtitleImageRenderingEnabled = true
    nonisolated(unsafe) static var subtitleHDREffectPolicy = SubtitleHDREffectPolicy.automatic
    nonisolated(unsafe) static var isOfflineSubtitleGenerationEnabled = false
    nonisolated(unsafe) static var offlineSubtitleGenerator: (any AudioRecognize)?
    nonisolated(unsafe) static var onlineSubtitleProviders: [any OnlineSubtitleProvider] = []
    nonisolated(unsafe) static var onlineSubtitleLanguages = ["zh-cn"]
    nonisolated(unsafe) static var isExternalSubtitleTranslationEnabled = false
    nonisolated(unsafe) static var externalSubtitleTranslationProvider: (any SubtitleTranslationProvider)?
    nonisolated(unsafe) static var externalSubtitleTranslationDisplayMode = ExternalSubtitleTranslationDisplayMode.translation
    nonisolated(unsafe) static var externalSubtitleTranslationSourceLanguage: String?
    nonisolated(unsafe) static var externalSubtitleTranslationTargetLanguage: String?
    nonisolated(unsafe) static var audioSpatializationPreference = AudioSpatializationPreference.automatic
    nonisolated(unsafe) static var multichannelAudioPreference = MultichannelAudioPreference.automatic
    // 默认不用自研的硬解，因为有些视频的AVPacket的pts顺序是不对的，只有解码后的AVFrame里面的pts是对的。
    /// Process-wide default for KSPlayer's direct asynchronous VideoToolbox decode path.
    nonisolated(unsafe) static var asynchronousDecompression = false
    nonisolated(unsafe) static var isPipPopViewController = false
    nonisolated(unsafe) static var canStartPictureInPictureAutomaticallyFromInline = true
    nonisolated(unsafe) static var pictureInPictureSubtitlePolicy = PictureInPictureSubtitlePolicy.automatic
    nonisolated(unsafe) static var preferredFrame = true
    nonisolated(unsafe) static var useSystemHTTPProxy = true
    #if !os(macOS)
    /// Optional process-wide default route sharing policy. Nil keeps KSPlayer's platform defaults.
    /// Use `.longFormAudio` to expose audio-only AirPlay/Wi-Fi routes for native playback.
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

    static func setAudioSession(options: KSOptions? = nil, playbackPipeline: AudioPlaybackPipeline = .nativeAVPlayer) {
        #if os(macOS)
//        try? AVAudioSession.sharedInstance().setRouteSharingPolicy(.longFormAudio)
        updateAudioRouteDiagnostic(options: options, playbackPipeline: playbackPipeline)
        #else
        var category = AVAudioSession.sharedInstance().category
        if category != .playAndRecord {
            category = .playback
        }
        let policy = AudioRouteSharingPolicyResolver.resolvedPolicy(optionPolicy: options?.audioRouteSharingPolicy, defaultPolicy: audioRouteSharingPolicy)
        try? AVAudioSession.sharedInstance().setCategory(category, mode: .moviePlayback, policy: policy)
        if let preferredAudioIOBufferDuration = options?.preferredAudioIOBufferDuration {
            try? AVAudioSession.sharedInstance().setPreferredIOBufferDuration(preferredAudioIOBufferDuration)
        }
        let supportsMultichannel = configureMultichannelContentSupport(options: options, sourceChannelCount: nil, isSpatialRoute: nil)
        updateAudioRouteDiagnostic(
            options: options,
            playbackPipeline: playbackPipeline,
            routeSharingPolicy: routeSharingPolicyDescription(policy),
            configuredSupportsMultichannelContent: supportsMultichannel
        )
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

    @discardableResult
    private static func configureMultichannelContentSupport(options: KSOptions?, sourceChannelCount: AVAudioChannelCount?, isSpatialRoute: Bool?) -> Bool {
        guard #available(tvOS 15.0, iOS 15.0, *) else {
            return false
        }
        let spatialPreference = options?.audioSpatializationPreference ?? KSOptions.audioSpatializationPreference
        let multichannelPreference = options?.multichannelAudioPreference ?? KSOptions.multichannelAudioPreference
        let supportsMultichannel = AudioMultichannelContentSupportResolver.supportsMultichannelContent(
            spatialPreference: spatialPreference,
            multichannelPreference: multichannelPreference,
            sourceChannelCount: sourceChannelCount,
            isSpatialRoute: isSpatialRoute
        )
        try? AVAudioSession.sharedInstance().setSupportsMultichannelContent(supportsMultichannel)
        KSLog("[audio] supportsMultichannelContent: \(supportsMultichannel), spatialPreference: \(spatialPreference), multichannelPreference: \(multichannelPreference)")
        return supportsMultichannel
    }
    #endif

    @discardableResult
    static func updateAudioRouteDiagnostic(
        options: KSOptions?,
        playbackPipeline: AudioPlaybackPipeline,
        sourceChannelCount: AVAudioChannelCount? = nil,
        containsEncodedPassthroughCandidate: Bool = false,
        allowsExternalPlayback: Bool? = nil,
        usesExternalPlaybackWhileExternalScreenIsActive: Bool? = nil,
        isExternalPlaybackActive: Bool? = nil,
        routeSharingPolicy: String? = nil,
        configuredSupportsMultichannelContent: Bool? = nil
    ) -> AudioRouteDiagnostic {
        let spatialPreference = options?.audioSpatializationPreference ?? KSOptions.audioSpatializationPreference
        let multichannelPreference = options?.multichannelAudioPreference ?? KSOptions.multichannelAudioPreference
        let spatialRoute = currentRouteContainsSpatialAudioOutput()
        let supportsMultichannel = configuredSupportsMultichannelContent ?? AudioMultichannelContentSupportResolver.supportsMultichannelContent(
            spatialPreference: spatialPreference,
            multichannelPreference: multichannelPreference,
            sourceChannelCount: sourceChannelCount,
            isSpatialRoute: spatialRoute
        )
        let resolvedRouteSharingPolicy = routeSharingPolicy ?? currentRouteSharingPolicyDescription(options: options)
        let diagnostic = AudioRouteDiagnostic(
            playbackPipeline: playbackPipeline,
            outputPorts: currentRouteOutputDiagnostics(),
            maximumOutputNumberOfChannels: maximumOutputNumberOfChannels(),
            preferredOutputNumberOfChannels: preferredOutputNumberOfChannels(),
            outputNumberOfChannels: outputNumberOfChannels(),
            outputLatency: outputLatency(),
            routeSharingPolicy: resolvedRouteSharingPolicy,
            configuredSupportsMultichannelContent: supportsMultichannel,
            spatialPreference: spatialPreference,
            multichannelPreference: multichannelPreference,
            sourceChannelCount: sourceChannelCount,
            allowsExternalPlayback: allowsExternalPlayback,
            usesExternalPlaybackWhileExternalScreenIsActive: usesExternalPlaybackWhileExternalScreenIsActive,
            isExternalPlaybackActive: isExternalPlaybackActive,
            encodedPassthroughPolicy: EncodedAudioPassthroughPolicyResolver.policy(
                pipeline: playbackPipeline,
                containsEncodedPassthroughCandidate: containsEncodedPassthroughCandidate
            )
        )
        options?.audioRouteDiagnostic = diagnostic
        KSLog("[audio] route diagnostic: \(diagnostic.description), passthroughReason: \(diagnostic.encodedPassthroughPolicy.reason)")
        return diagnostic
    }

    private static func currentRouteOutputDiagnostics() -> [AudioRouteOutputDiagnostic] {
        #if os(macOS)
        return []
        #else
        return AVAudioSession.sharedInstance().currentRoute.outputs.map { output in
            let isSpatialAudioEnabled: Bool?
            if #available(tvOS 15.0, iOS 15.0, *) {
                isSpatialAudioEnabled = output.isSpatialAudioEnabled
            } else {
                isSpatialAudioEnabled = nil
            }
            return AudioRouteOutputDiagnostic(
                portName: output.portName,
                portType: output.portType.rawValue,
                outputKind: AudioRouteOutputClassifier.outputKind(portTypeRawValue: output.portType.rawValue),
                channelCount: output.channels?.count,
                isSpatialAudioEnabled: isSpatialAudioEnabled
            )
        }
        #endif
    }

    private static func currentRouteContainsSpatialAudioOutput() -> Bool? {
        #if os(macOS)
        return nil
        #else
        if #available(tvOS 15.0, iOS 15.0, *) {
            return AVAudioSession.sharedInstance().currentRoute.outputs.contains { $0.isSpatialAudioEnabled }
        } else {
            return nil
        }
        #endif
    }

    private static func maximumOutputNumberOfChannels() -> Int? {
        #if os(macOS)
        return nil
        #else
        return AVAudioSession.sharedInstance().maximumOutputNumberOfChannels
        #endif
    }

    private static func preferredOutputNumberOfChannels() -> Int? {
        #if os(macOS)
        return nil
        #else
        return AVAudioSession.sharedInstance().preferredOutputNumberOfChannels
        #endif
    }

    private static func outputNumberOfChannels() -> Int? {
        #if os(macOS)
        return nil
        #else
        return AVAudioSession.sharedInstance().outputNumberOfChannels
        #endif
    }

    private static func outputLatency() -> TimeInterval? {
        #if os(macOS)
        return nil
        #else
        return AVAudioSession.sharedInstance().outputLatency
        #endif
    }

    private static func currentRouteSharingPolicyDescription(options: KSOptions?) -> String? {
        #if os(macOS)
        return nil
        #else
        let policy = AudioRouteSharingPolicyResolver.resolvedPolicy(optionPolicy: options?.audioRouteSharingPolicy, defaultPolicy: audioRouteSharingPolicy)
        return routeSharingPolicyDescription(policy)
        #endif
    }

    #if !os(macOS)
    private static func routeSharingPolicyDescription(_ policy: AVAudioSession.RouteSharingPolicy) -> String {
        switch policy {
        case .default:
            return "default"
        case .longFormAudio:
            return "longFormAudio"
        case .longFormVideo:
            return "longFormVideo"
        case .independent:
            return "independent"
        @unknown default:
            return "unknown(\(policy.rawValue))"
        }
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

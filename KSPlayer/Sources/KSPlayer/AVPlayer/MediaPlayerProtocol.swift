//
//  MediaPlayerProtocol.swift
//  KSPlayer-tvOS
//
//  Created by kintan on 2018/3/9.
//

import AVFoundation
import Foundation
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

public protocol MediaPlayback: AnyObject {
    var duration: TimeInterval { get }
    var fileSize: Double { get }
    var naturalSize: CGSize { get }
    var chapters: [Chapter] { get }
    var currentPlaybackTime: TimeInterval { get }
    func prepareToPlay()
    func shutdown()
    func seek(time: TimeInterval, completion: @escaping ((Bool) -> Void))
}

public enum StreamRecordingPhase: Equatable, Sendable {
    case starting
    case recording
    case finalizing
    case finished
    case failed
}

public enum StreamRecordingStreamAction: Equatable, Sendable {
    case recorded
    case skipped
}

public struct StreamRecordingStreamDiagnostic: Equatable, Sendable {
    public let inputIndex: Int
    public let outputIndex: Int?
    public let mediaType: String
    public let codecName: String?
    public let action: StreamRecordingStreamAction
    public let reason: String?

    public init(inputIndex: Int, outputIndex: Int?, mediaType: String, codecName: String?, action: StreamRecordingStreamAction, reason: String? = nil) {
        self.inputIndex = inputIndex
        self.outputIndex = outputIndex
        self.mediaType = mediaType
        self.codecName = codecName
        self.action = action
        self.reason = reason
    }
}

public struct StreamRecordingProgress: Equatable, Sendable {
    public let phase: StreamRecordingPhase
    public let destinationURL: URL?
    public let temporaryURL: URL?
    public let duration: TimeInterval
    public let bytesWritten: Int64
    public let packetsWritten: Int64
    public let packetsSkipped: Int64
    public let droppedVideoFrameCount: UInt32
    public let droppedVideoPacketCount: UInt32
    public let streams: [StreamRecordingStreamDiagnostic]
    public let message: String?

    public init(
        phase: StreamRecordingPhase,
        destinationURL: URL? = nil,
        temporaryURL: URL? = nil,
        duration: TimeInterval = 0,
        bytesWritten: Int64 = 0,
        packetsWritten: Int64 = 0,
        packetsSkipped: Int64 = 0,
        droppedVideoFrameCount: UInt32 = 0,
        droppedVideoPacketCount: UInt32 = 0,
        streams: [StreamRecordingStreamDiagnostic] = [],
        message: String? = nil
    ) {
        self.phase = phase
        self.destinationURL = destinationURL
        self.temporaryURL = temporaryURL
        self.duration = duration
        self.bytesWritten = bytesWritten
        self.packetsWritten = packetsWritten
        self.packetsSkipped = packetsSkipped
        self.droppedVideoFrameCount = droppedVideoFrameCount
        self.droppedVideoPacketCount = droppedVideoPacketCount
        self.streams = streams
        self.message = message
    }
}

public class DynamicInfo: ObservableObject {
    private let metadataBlock: () -> [String: String]
    private let bytesReadBlock: () -> Int64
    private let audioBitrateBlock: () -> Int
    private let videoBitrateBlock: () -> Int
    public var metadata: [String: String] {
        metadataBlock()
    }

    public var bytesRead: Int64 {
        bytesReadBlock()
    }

    public var audioBitrate: Int {
        audioBitrateBlock()
    }

    public var videoBitrate: Int {
        videoBitrateBlock()
    }

    @Published
    public var displayFPS = 0.0
    public var audioVideoSyncDiff = 0.0
    public var droppedVideoFrameCount = UInt32(0)
    public var droppedVideoPacketCount = UInt32(0)
    /// Runtime MEPlayer metrics for low-latency live verification.
    @Published
    public var lowLatencyLiveDiagnostic: LowLatencyLiveDiagnostic?
    /// Best-effort KSMEPlayer stream-recording progress. Bytes are written to a temporary file until stop/finalize.
    @Published
    public var streamRecordingProgress: StreamRecordingProgress?
    init(metadata: @escaping () -> [String: String], bytesRead: @escaping () -> Int64, audioBitrate: @escaping () -> Int, videoBitrate: @escaping () -> Int) {
        metadataBlock = metadata
        bytesReadBlock = bytesRead
        audioBitrateBlock = audioBitrate
        videoBitrateBlock = videoBitrate
    }
}

public struct LowLatencyLiveMetricSummary: Equatable, Sendable {
    public let sampleCount: Int
    public let latest: Double?
    public let average: Double?
    public let maximum: Double?
}

public struct LowLatencyLivePipelineTimestamps: Equatable, Sendable {
    /// Monotonic seconds relative to the prepare timestamp when available.
    public let prepare: TimeInterval?
    public let open: TimeInterval?
    public let findStreams: TimeInterval?
    public let ready: TimeInterval?
    public let firstVideoRead: TimeInterval?
    public let firstAudioRead: TimeInterval?
    public let firstVideoDecode: TimeInterval?
    public let firstAudioDecode: TimeInterval?
    public let firstVideoRender: TimeInterval?
    public let firstAudioRender: TimeInterval?
}

public struct LowLatencyLiveRollingMetrics: Equatable, Sendable {
    public let bufferedDuration: LowLatencyLiveMetricSummary
    public let packetCount: LowLatencyLiveMetricSummary
    public let frameCount: LowLatencyLiveMetricSummary
    public let absoluteAudioVideoSyncDiff: LowLatencyLiveMetricSummary
    public let displayFPS: LowLatencyLiveMetricSummary
    public let videoReadToDecodeDuration: LowLatencyLiveMetricSummary
    public let videoDecodeToRenderDuration: LowLatencyLiveMetricSummary
    public let videoReadToRenderDuration: LowLatencyLiveMetricSummary
    public let audioLatencyEstimate: LowLatencyLiveMetricSummary
}

public struct LowLatencyLiveDiagnostic: Equatable, Sendable {
    public let profile: KSLowLatencyLiveProfile
    /// Local decoded/queued media duration in seconds. This is not camera-to-screen latency.
    public let bufferedDuration: TimeInterval
    public let packetCount: Int
    public let frameCount: Int
    public let droppedVideoFrameCount: UInt32
    public let droppedVideoPacketCount: UInt32
    public let renderedVideoFrameCount: UInt64
    public let audioRenderUpdateCount: UInt64
    public let audioVideoSyncDiff: TimeInterval
    public let displayFPS: Double
    /// Local audio queue/device estimate in seconds. This is not microphone-to-speaker latency.
    public let audioLatencyEstimate: TimeInterval?
    public let timestamps: LowLatencyLivePipelineTimestamps
    public let rollingMetrics: LowLatencyLiveRollingMetrics
    public let prepareToReadyDuration: TimeInterval?
    public let openToReadyDuration: TimeInterval?
    public let startupToFirstVideoFrameDuration: TimeInterval?
    public let startupToFirstAudioFrameDuration: TimeInterval?
    public let firstVideoReadToDecodeDuration: TimeInterval?
    public let firstAudioReadToDecodeDuration: TimeInterval?
    public let firstVideoDecodeToRenderDuration: TimeInterval?
    public let firstAudioDecodeToRenderDuration: TimeInterval?
    public let firstVideoReadToRenderDuration: TimeInterval?
    public let firstAudioReadToRenderDuration: TimeInterval?
}

struct LowLatencyLiveMetricWindow: Equatable, Sendable {
    private let windowSize: Int
    private var samples = [Double]()

    init(windowSize: Int = 60) {
        self.windowSize = max(1, windowSize)
    }

    mutating func record(_ sample: Double?) {
        guard let sample, sample.isFinite else {
            return
        }
        samples.append(sample)
        if samples.count > windowSize {
            samples.removeFirst(samples.count - windowSize)
        }
    }

    var summary: LowLatencyLiveMetricSummary {
        guard !samples.isEmpty else {
            return LowLatencyLiveMetricSummary(sampleCount: 0, latest: nil, average: nil, maximum: nil)
        }
        let total = samples.reduce(0, +)
        return LowLatencyLiveMetricSummary(
            sampleCount: samples.count,
            latest: samples.last,
            average: total / Double(samples.count),
            maximum: samples.max()
        )
    }
}

struct LowLatencyLiveDiagnosticAggregator: Equatable, Sendable {
    private var bufferedDuration = LowLatencyLiveMetricWindow()
    private var packetCount = LowLatencyLiveMetricWindow()
    private var frameCount = LowLatencyLiveMetricWindow()
    private var absoluteAudioVideoSyncDiff = LowLatencyLiveMetricWindow()
    private var displayFPS = LowLatencyLiveMetricWindow()
    private var videoReadToDecodeDuration = LowLatencyLiveMetricWindow()
    private var videoDecodeToRenderDuration = LowLatencyLiveMetricWindow()
    private var videoReadToRenderDuration = LowLatencyLiveMetricWindow()
    private var audioLatencyEstimate = LowLatencyLiveMetricWindow()

    mutating func record(
        bufferedDuration bufferedDurationSample: TimeInterval,
        packetCount packetCountSample: Int,
        frameCount frameCountSample: Int,
        audioVideoSyncDiff: TimeInterval,
        displayFPS displayFPSSample: Double,
        videoReadToDecodeDuration videoReadToDecodeDurationSample: TimeInterval?,
        videoDecodeToRenderDuration videoDecodeToRenderDurationSample: TimeInterval?,
        videoReadToRenderDuration videoReadToRenderDurationSample: TimeInterval?,
        audioLatencyEstimate audioLatencyEstimateSample: TimeInterval?
    ) -> LowLatencyLiveRollingMetrics {
        bufferedDuration.record(bufferedDurationSample)
        packetCount.record(Double(packetCountSample))
        frameCount.record(Double(frameCountSample))
        absoluteAudioVideoSyncDiff.record(abs(audioVideoSyncDiff))
        displayFPS.record(displayFPSSample)
        videoReadToDecodeDuration.record(videoReadToDecodeDurationSample)
        videoDecodeToRenderDuration.record(videoDecodeToRenderDurationSample)
        videoReadToRenderDuration.record(videoReadToRenderDurationSample)
        audioLatencyEstimate.record(audioLatencyEstimateSample)
        return LowLatencyLiveRollingMetrics(
            bufferedDuration: bufferedDuration.summary,
            packetCount: packetCount.summary,
            frameCount: frameCount.summary,
            absoluteAudioVideoSyncDiff: absoluteAudioVideoSyncDiff.summary,
            displayFPS: displayFPS.summary,
            videoReadToDecodeDuration: videoReadToDecodeDuration.summary,
            videoDecodeToRenderDuration: videoDecodeToRenderDuration.summary,
            videoReadToRenderDuration: videoReadToRenderDuration.summary,
            audioLatencyEstimate: audioLatencyEstimate.summary
        )
    }
}

public struct Chapter {
    public let start: TimeInterval
    public let end: TimeInterval
    public let title: String
}

public struct MediaPlaybackTimeRange: Equatable {
    public let start: TimeInterval
    public let end: TimeInterval

    public var duration: TimeInterval {
        end - start
    }

    public init?(start: TimeInterval, duration: TimeInterval) {
        guard start.isFinite, duration.isFinite, duration > 0 else {
            return nil
        }
        self.start = max(start, 0)
        end = self.start + duration
    }

    public init?(start: TimeInterval, end: TimeInterval) {
        guard start.isFinite, end.isFinite, end > start else {
            return nil
        }
        self.start = max(start, 0)
        self.end = max(end, self.start)
    }

    public func clamped(_ time: TimeInterval) -> TimeInterval {
        min(max(time, start), end)
    }
}

enum MediaSeekTimeResolver {
    static func resolvedSeekTime(_ time: TimeInterval, seekableTimeRange: MediaPlaybackTimeRange?) -> TimeInterval? {
        guard time.isFinite else {
            return nil
        }
        let nonNegativeTime = max(time, 0)
        return seekableTimeRange?.clamped(nonNegativeTime) ?? nonNegativeTime
    }
}

public protocol MediaPlayerProtocol: MediaPlayback {
    var delegate: MediaPlayerDelegate? { get set }
    var view: UIView? { get }
    var playableTime: TimeInterval { get }
    var isReadyToPlay: Bool { get }
    var playbackState: MediaPlaybackState { get }
    var loadState: MediaLoadState { get }
    var isPlaying: Bool { get }
    var seekable: Bool { get }
    var seekableTimeRange: MediaPlaybackTimeRange? { get }
    //    var numberOfBytesTransferred: Int64 { get }
    var isMuted: Bool { get set }
    var allowsExternalPlayback: Bool { get set }
    var usesExternalPlaybackWhileExternalScreenIsActive: Bool { get set }
    var isExternalPlaybackActive: Bool { get }
    var playbackRate: Float { get set }
    var playbackVolume: Float { get set }
    var contentMode: UIViewContentMode { get set }
    var subtitleDataSouce: SubtitleDataSouce? { get }
    var pictureInPictureSubtitleDiagnostic: PictureInPictureSubtitleDiagnostic { get }
    @available(macOS 12.0, iOS 15.0, tvOS 15.0, *)
    var playbackCoordinator: AVPlaybackCoordinator { get }
    @available(tvOS 14.0, *)
    var pipController: KSPictureInPictureController? { get }
    var dynamicInfo: DynamicInfo? { get }
    init(url: URL, options: KSOptions)
    init(url: URL, audioURL: URL?, options: KSOptions)
    func replace(url: URL, options: KSOptions)
    func replace(url: URL, audioURL: URL?, options: KSOptions)
    func play()
    func pause()
    func enterBackground()
    func enterForeground()
    func thumbnailImageAtCurrentTime() async -> CGImage?
    func tracks(mediaType: AVFoundation.AVMediaType) -> [MediaPlayerTrack]
    func select(track: some MediaPlayerTrack)
}

public extension MediaPlayerProtocol {
    var pictureInPictureSubtitleDiagnostic: PictureInPictureSubtitleDiagnostic {
        PictureInPictureSubtitlePolicyResolver.diagnostic(
            policy: .automatic,
            usesNativeLegibleSelection: false
        )
    }

    init(url: URL, audioURL _: URL?, options: KSOptions) {
        self.init(url: url, options: options)
    }

    func replace(url: URL, audioURL _: URL?, options: KSOptions) {
        replace(url: url, options: options)
    }

    var nominalFrameRate: Float {
        tracks(mediaType: .video).first { $0.isEnabled }?.nominalFrameRate ?? 0
    }

    var seekableTimeRange: MediaPlaybackTimeRange? {
        guard seekable else {
            return nil
        }
        return MediaPlaybackTimeRange(start: 0, duration: duration)
    }
}

@MainActor
public protocol MediaPlayerDelegate: AnyObject {
    func readyToPlay(player: some MediaPlayerProtocol)
    func changeLoadState(player: some MediaPlayerProtocol)
    // 缓冲加载进度，0-100
    func changeBuffering(player: some MediaPlayerProtocol, progress: Int)
    func playBack(player: some MediaPlayerProtocol, loopCount: Int)
    func finish(player: some MediaPlayerProtocol, error: Error?)
}

public protocol MediaPlayerTrack: AnyObject, CustomStringConvertible {
    var trackID: Int32 { get }
    var name: String { get }
    var languageCode: String? { get }
    var mediaType: AVFoundation.AVMediaType { get }
    var nominalFrameRate: Float { get set }
    var bitRate: Int64 { get }
    var bitDepth: Int32 { get }
    var isEnabled: Bool { get set }
    var isImageSubtitle: Bool { get }
    var rotation: Int16 { get }
    var dovi: DOVIDecoderConfigurationRecord? { get }
    var fieldOrder: FFmpegFieldOrder { get }
    var formatDescription: CMFormatDescription? { get }
}

// public extension MediaPlayerTrack: Identifiable {
//    var id: Int32 { trackID }
// }

public enum MediaPlaybackState: Int {
    case idle
    case playing
    case paused
    case seeking
    case finished
    case stopped
}

public enum MediaLoadState: Int {
    case idle
    case loading
    case playable
}

// swiftlint:disable identifier_name
public struct DOVIDecoderConfigurationRecord: Equatable, Sendable {
    public let dv_version_major: UInt8
    public let dv_version_minor: UInt8
    public let dv_profile: UInt8
    public let dv_level: UInt8
    public let rpu_present_flag: UInt8
    public let el_present_flag: UInt8
    public let bl_present_flag: UInt8
    public let dv_bl_signal_compatibility_id: UInt8
}

public enum DolbyVisionProfile7EnhancementLayerKind: Equatable, Sendable, CustomStringConvertible {
    case notProfile7
    case none
    case unknown
    case minimumEnhancementLayer
    case fullEnhancementLayer

    public var description: String {
        switch self {
        case .notProfile7:
            return "not Profile 7"
        case .none:
            return "no Profile 7 enhancement layer"
        case .unknown:
            return "Profile 7 enhancement layer present; MEL/FEL not yet determined"
        case .minimumEnhancementLayer:
            return "Profile 7 MEL-compatible enhancement layer"
        case .fullEnhancementLayer:
            return "Profile 7 FEL enhancement layer"
        }
    }
}

public enum DolbyVisionPlaybackCapability: Equatable, Sendable, CustomStringConvertible {
    case nativeDolbyVision
    case baseLayerHDR10Fallback
    case minimumEnhancementLayerFallback
    case fullEnhancementLayerComposition
    case fullEnhancementLayerCompositionUnavailable
    case fullEnhancementLayerCompositionRequiredUnavailable
    case enhancementLayerKindUnknown
    case sourceMetadataFallback

    public var description: String {
        switch self {
        case .nativeDolbyVision:
            return "native/system Dolby Vision when the active playback route supports it"
        case .baseLayerHDR10Fallback:
            return "HDR10 base-layer fallback"
        case .minimumEnhancementLayerFallback:
            return "MEL-compatible HDR10 base-layer fallback with RPU metadata preserved"
        case .fullEnhancementLayerComposition:
            return "FEL full composition through configured compositor backend"
        case .fullEnhancementLayerCompositionUnavailable:
            return "FEL full composition unavailable; HDR10 base-layer fallback"
        case .fullEnhancementLayerCompositionRequiredUnavailable:
            return "FEL full composition required but unavailable; playback unsupported"
        case .enhancementLayerKindUnknown:
            return "Profile 7 enhancement layer kind unknown; HDR10 base-layer fallback"
        case .sourceMetadataFallback:
            return "source/base metadata fallback"
        }
    }
}

public enum DolbyVisionFELPlaybackPolicy: Equatable, Sendable, CustomStringConvertible {
    /// Preserve the existing general-build behavior: decode/render the HDR10 base layer and report FEL composition as unavailable.
    case allowBaseLayerFallback
    /// Private/education strict mode: once Profile 7 EL/FEL composition is needed, refuse misleading BL-only playback.
    case requireFullComposition

    public var description: String {
        switch self {
        case .allowBaseLayerFallback:
            return "allow HDR10 base-layer fallback"
        case .requireFullComposition:
            return "require full FEL composition"
        }
    }
}

public enum DolbyVisionFELCompositionState: Equatable, Sendable, CustomStringConvertible {
    case notRequired
    case awaitingMetadata
    case separatedInputsAvailable
    case available(backend: String)
    case unavailable(reason: String)
    case requiredUnavailable(reason: String)

    public var isFullCompositionAvailable: Bool {
        if case .available = self {
            return true
        }
        return false
    }

    public var blocksPlayback: Bool {
        if case .requiredUnavailable = self {
            return true
        }
        return false
    }

    public var description: String {
        switch self {
        case .notRequired:
            return "FEL composition not required"
        case .awaitingMetadata:
            return "awaiting RPU/EL metadata before composition can be assessed"
        case .separatedInputsAvailable:
            return "BL/EL/RPU inputs can be separated, but full FEL residual composition is unavailable"
        case let .available(backend):
            return "FEL composition available through \(backend)"
        case let .unavailable(reason):
            return reason
        case let .requiredUnavailable(reason):
            return reason
        }
    }
}

public enum DolbyVisionFELCompositorAvailability: Equatable, Sendable, CustomStringConvertible {
    case unavailable(reason: String)
    case available(backend: String)

    public var description: String {
        switch self {
        case let .unavailable(reason):
            return reason
        case let .available(backend):
            return "available through \(backend)"
        }
    }
}

public struct DolbyVisionFELCompositorInput {
    public let baseLayerPixelBuffer: CVPixelBuffer
    public let enhancementLayerPixelBuffer: CVPixelBuffer
    public let rpuData: Data
    public let presentationTime: CMTime
    public let diagnostic: DolbyVisionPlaybackDiagnostic

    public init(
        baseLayerPixelBuffer: CVPixelBuffer,
        enhancementLayerPixelBuffer: CVPixelBuffer,
        rpuData: Data,
        presentationTime: CMTime,
        diagnostic: DolbyVisionPlaybackDiagnostic
    ) {
        self.baseLayerPixelBuffer = baseLayerPixelBuffer
        self.enhancementLayerPixelBuffer = enhancementLayerPixelBuffer
        self.rpuData = rpuData
        self.presentationTime = presentationTime
        self.diagnostic = diagnostic
    }
}

public struct DolbyVisionFELCompositorOutput {
    public let pixelBuffer: CVPixelBuffer
    public let diagnosticMessage: String?

    public init(pixelBuffer: CVPixelBuffer, diagnosticMessage: String? = nil) {
        self.pixelBuffer = pixelBuffer
        self.diagnosticMessage = diagnosticMessage
    }
}

public protocol DolbyVisionFELCompositorBackend: AnyObject {
    var availability: DolbyVisionFELCompositorAvailability { get }
    func compose(input: DolbyVisionFELCompositorInput) throws -> DolbyVisionFELCompositorOutput
}

public struct DolbyVisionPlaybackDiagnostic: Equatable, Sendable, CustomStringConvertible {
    public static let missingOpenFELCompositorReason = "FEL composition unavailable: current FFmpeg/libdovi public APIs expose DOVI/RPU metadata but no BL+EL residual reconstruction compositor"
    public static let requiredFELCompositorReason = "FEL composition required by policy, but no available compositor backend is configured"

    public let configuration: DOVIDecoderConfigurationRecord
    public let enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind
    public let felCompositionState: DolbyVisionFELCompositionState
    public let felPlaybackPolicy: DolbyVisionFELPlaybackPolicy
    public let observedRPUNALUnitCount: Int
    public let observedEnhancementLayerNALUnitCount: Int
    public let observedEnhancementLayerVCLNALUnitCount: Int
    public let largestEnhancementLayerVCLPayloadSize: Int
    /// Runtime copy of the latest observed RPU payload for preservation/debugging. This is not a stable persisted format.
    public let rawRPUData: Data?
    public let hasFrameRPUBuffer: Bool
    public let hasFrameDOVIMetadata: Bool

    public init(
        configuration: DOVIDecoderConfigurationRecord,
        enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind? = nil,
        felCompositionState: DolbyVisionFELCompositionState? = nil,
        felPlaybackPolicy: DolbyVisionFELPlaybackPolicy = .allowBaseLayerFallback,
        observedRPUNALUnitCount: Int = 0,
        observedEnhancementLayerNALUnitCount: Int = 0,
        observedEnhancementLayerVCLNALUnitCount: Int = 0,
        largestEnhancementLayerVCLPayloadSize: Int = 0,
        rawRPUData: Data? = nil,
        hasFrameRPUBuffer: Bool = false,
        hasFrameDOVIMetadata: Bool = false
    ) {
        self.configuration = configuration
        let resolvedEnhancementLayerKind = enhancementLayerKind ?? configuration.profile7EnhancementLayerKind
        self.enhancementLayerKind = resolvedEnhancementLayerKind
        self.felPlaybackPolicy = felPlaybackPolicy
        self.felCompositionState = Self.resolvedFELCompositionState(
            configuration: configuration,
            enhancementLayerKind: resolvedEnhancementLayerKind,
            requestedState: felCompositionState,
            playbackPolicy: felPlaybackPolicy
        )
        self.observedRPUNALUnitCount = observedRPUNALUnitCount
        self.observedEnhancementLayerNALUnitCount = observedEnhancementLayerNALUnitCount
        self.observedEnhancementLayerVCLNALUnitCount = observedEnhancementLayerVCLNALUnitCount
        self.largestEnhancementLayerVCLPayloadSize = largestEnhancementLayerVCLPayloadSize
        self.rawRPUData = rawRPUData
        self.hasFrameRPUBuffer = hasFrameRPUBuffer
        self.hasFrameDOVIMetadata = hasFrameDOVIMetadata
    }

    public var capability: DolbyVisionPlaybackCapability {
        if felCompositionState.isFullCompositionAvailable {
            return .fullEnhancementLayerComposition
        }
        if felCompositionState.blocksPlayback {
            return .fullEnhancementLayerCompositionRequiredUnavailable
        }
        return configuration.playbackCapability(enhancementLayerKind: enhancementLayerKind)
    }

    public var blocksPlayback: Bool {
        felCompositionState.blocksPlayback
    }

    public var hasRPUMetadata: Bool {
        configuration.rpu_present_flag != 0 || observedRPUNALUnitCount > 0 || rawRPUData != nil || hasFrameRPUBuffer || hasFrameDOVIMetadata
    }

    public var hasObservedEnhancementLayerPayload: Bool {
        observedEnhancementLayerNALUnitCount > 0 || observedEnhancementLayerVCLNALUnitCount > 0
    }

    public func merging(
        observedRPUNALUnitCount additionalRPUNALUnitCount: Int = 0,
        observedEnhancementLayerNALUnitCount additionalEnhancementLayerNALUnitCount: Int = 0,
        observedEnhancementLayerVCLNALUnitCount additionalEnhancementLayerVCLNALUnitCount: Int = 0,
        largestEnhancementLayerVCLPayloadSize: Int = 0,
        rawRPUData: Data? = nil,
        hasFrameRPUBuffer: Bool? = nil,
        hasFrameDOVIMetadata: Bool? = nil,
        enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind? = nil,
        felCompositionState: DolbyVisionFELCompositionState? = nil,
        felPlaybackPolicy: DolbyVisionFELPlaybackPolicy? = nil
    ) -> DolbyVisionPlaybackDiagnostic {
        let resolvedEnhancementLayerKind = Self.resolvedEnhancementLayerKind(
            current: self.enhancementLayerKind,
            update: enhancementLayerKind,
            configuration: configuration
        )
        let resolvedPlaybackPolicy = felPlaybackPolicy ?? self.felPlaybackPolicy
        return DolbyVisionPlaybackDiagnostic(
            configuration: configuration,
            enhancementLayerKind: resolvedEnhancementLayerKind,
            felCompositionState: Self.resolvedFELCompositionState(
                current: self.felCompositionState,
                update: felCompositionState,
                configuration: configuration,
                enhancementLayerKind: resolvedEnhancementLayerKind,
                playbackPolicy: resolvedPlaybackPolicy
            ),
            felPlaybackPolicy: resolvedPlaybackPolicy,
            observedRPUNALUnitCount: observedRPUNALUnitCount + additionalRPUNALUnitCount,
            observedEnhancementLayerNALUnitCount: observedEnhancementLayerNALUnitCount + additionalEnhancementLayerNALUnitCount,
            observedEnhancementLayerVCLNALUnitCount: observedEnhancementLayerVCLNALUnitCount + additionalEnhancementLayerVCLNALUnitCount,
            largestEnhancementLayerVCLPayloadSize: max(self.largestEnhancementLayerVCLPayloadSize, largestEnhancementLayerVCLPayloadSize),
            rawRPUData: rawRPUData ?? self.rawRPUData,
            hasFrameRPUBuffer: hasFrameRPUBuffer ?? self.hasFrameRPUBuffer,
            hasFrameDOVIMetadata: hasFrameDOVIMetadata ?? self.hasFrameDOVIMetadata
        )
    }

    public var description: String {
        var parts = [
            "Dolby Vision profile \(configuration.dv_profile)",
            capability.description,
        ]
        if configuration.dv_profile == 7 {
            parts.append(enhancementLayerKind.description)
            parts.append(felCompositionState.description)
            if observedEnhancementLayerNALUnitCount > 0 {
                parts.append("observed EL NALs \(observedEnhancementLayerNALUnitCount)")
            }
            if observedRPUNALUnitCount > 0 {
                parts.append("observed RPU NALs \(observedRPUNALUnitCount)")
            } else if hasRPUMetadata {
                parts.append("RPU metadata present")
            }
        }
        return parts.joined(separator: "; ")
    }

    private static func resolvedFELCompositionState(
        configuration: DOVIDecoderConfigurationRecord,
        enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind,
        requestedState: DolbyVisionFELCompositionState?,
        playbackPolicy: DolbyVisionFELPlaybackPolicy
    ) -> DolbyVisionFELCompositionState {
        resolvedFELCompositionState(
            current: .awaitingMetadata,
            update: requestedState,
            configuration: configuration,
            enhancementLayerKind: enhancementLayerKind,
            playbackPolicy: playbackPolicy
        )
    }

    private static func resolvedFELCompositionState(
        current: DolbyVisionFELCompositionState,
        update: DolbyVisionFELCompositionState?,
        configuration: DOVIDecoderConfigurationRecord,
        enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind,
        playbackPolicy: DolbyVisionFELPlaybackPolicy
    ) -> DolbyVisionFELCompositionState {
        guard configuration.dv_profile == 7, configuration.el_present_flag != 0 else {
            return .notRequired
        }
        if enhancementLayerKind == .minimumEnhancementLayer {
            return .notRequired
        }
        if let update {
            if case .available = update {
                return update
            }
            if case .requiredUnavailable = update {
                return update
            }
        }
        if case .available = current {
            return current
        }
        if playbackPolicy == .requireFullComposition {
            if case let .requiredUnavailable(reason) = current {
                return .requiredUnavailable(reason: reason)
            }
            if enhancementLayerKind == .fullEnhancementLayer {
                return .requiredUnavailable(reason: Self.requiredFELCompositorReason)
            }
        }
        if enhancementLayerKind == .fullEnhancementLayer {
            return .unavailable(reason: missingOpenFELCompositorReason)
        }
        if case .unavailable = current {
            return current
        }
        if let update {
            if case .unavailable = update {
                return update
            }
            if update == .separatedInputsAvailable {
                return update
            }
        }
        return current
    }

    private static func resolvedEnhancementLayerKind(
        current: DolbyVisionProfile7EnhancementLayerKind,
        update: DolbyVisionProfile7EnhancementLayerKind?,
        configuration: DOVIDecoderConfigurationRecord
    ) -> DolbyVisionProfile7EnhancementLayerKind {
        guard configuration.dv_profile == 7 else {
            return .notProfile7
        }
        guard configuration.el_present_flag != 0 else {
            return .none
        }
        guard let update else {
            return current
        }
        if current == .fullEnhancementLayer || update == .fullEnhancementLayer {
            return .fullEnhancementLayer
        }
        if update == .minimumEnhancementLayer {
            return .minimumEnhancementLayer
        }
        return current
    }
}

public enum DolbyVisionEnhancementLayerCompositionSupport: Equatable, Sendable, CustomStringConvertible {
    case notRequired
    case unsupported(reason: String)

    public var isSupported: Bool {
        if case .notRequired = self {
            return true
        }
        return false
    }

    public var description: String {
        switch self {
        case .notRequired:
            return "enhancement-layer composition not required"
        case let .unsupported(reason):
            return reason
        }
    }
}

public enum HDR10PlusToneMappingPolicy: Equatable, Sendable, CustomStringConvertible {
    /// Prefer the system-managed display path. KSPlayer does not assert that the current device applies HDR10+ tone mapping.
    case systemManagedWhenAvailable
    /// Force the Metal path and apply KSPlayer's conservative HDR10+ knee/rolloff shader when structured metadata is available.
    case metalDynamicToneMapping
    /// Avoid system HDR10+ handling and preserve parsed dynamic metadata while rendering the HDR10 base signal.
    case staticHDR10Fallback

    public var description: String {
        switch self {
        case .systemManagedWhenAvailable:
            return "system-managed when available"
        case .metalDynamicToneMapping:
            return "Metal dynamic tone mapping"
        case .staticHDR10Fallback:
            return "static HDR10 fallback"
        }
    }
}

public enum HDR10PlusRenderPath: Equatable, Sendable, CustomStringConvertible {
    case systemDisplayLayer
    case metalRenderer

    public var description: String {
        switch self {
        case .systemDisplayLayer:
            return "system display layer"
        case .metalRenderer:
            return "Metal renderer"
        }
    }
}

public struct HDR10PlusMetadata: Equatable, Sendable {
    public struct WindowBounds: Equatable, Sendable {
        public static let fullFrame = WindowBounds(minX: 0, minY: 0, maxX: 1, maxY: 1)

        public let minX: Float
        public let minY: Float
        public let maxX: Float
        public let maxY: Float

        public init(minX: Float, minY: Float, maxX: Float, maxY: Float) {
            self.minX = Self.clamp(minX)
            self.minY = Self.clamp(minY)
            self.maxX = Self.clamp(maxX)
            self.maxY = Self.clamp(maxY)
        }

        private static func clamp(_ value: Float) -> Float {
            guard value.isFinite else {
                return 0
            }
            return Swift.min(Swift.max(value, 0), 1)
        }
    }

    public struct Percentile: Equatable, Sendable {
        public let percentage: UInt8
        public let percentile: Float

        public init(percentage: UInt8, percentile: Float) {
            self.percentage = percentage
            self.percentile = percentile
        }
    }

    public struct ToneMapping: Equatable, Sendable {
        public let kneePointX: Float
        public let kneePointY: Float
        public let bezierCurveAnchors: [Float]

        public init(kneePointX: Float, kneePointY: Float, bezierCurveAnchors: [Float]) {
            self.kneePointX = kneePointX
            self.kneePointY = kneePointY
            self.bezierCurveAnchors = bezierCurveAnchors
        }
    }

    public struct PixelSelector: Equatable, Sendable {
        public let centerX: Float
        public let centerY: Float
        public let rotationRadians: Float
        public let semimajorAxisInternal: Float
        public let semimajorAxisExternal: Float
        public let semiminorAxisInternal: Float
        public let semiminorAxisExternal: Float

        public init(
            centerX: Float,
            centerY: Float,
            rotationRadians: Float,
            semimajorAxisInternal: Float,
            semimajorAxisExternal: Float,
            semiminorAxisInternal: Float,
            semiminorAxisExternal: Float
        ) {
            self.centerX = Self.clamp01(centerX)
            self.centerY = Self.clamp01(centerY)
            self.rotationRadians = rotationRadians.isFinite ? rotationRadians : 0
            self.semimajorAxisInternal = Self.clampAxis(semimajorAxisInternal)
            self.semimajorAxisExternal = Self.clampAxis(semimajorAxisExternal)
            self.semiminorAxisInternal = Self.clampAxis(semiminorAxisInternal)
            self.semiminorAxisExternal = Self.clampAxis(semiminorAxisExternal)
        }

        private static func clamp01(_ value: Float) -> Float {
            guard value.isFinite else {
                return 0
            }
            return Swift.min(Swift.max(value, 0), 1)
        }

        private static func clampAxis(_ value: Float) -> Float {
            guard value.isFinite else {
                return 0.0001
            }
            return Swift.min(Swift.max(value, 0.0001), 1)
        }
    }

    public enum OverlapProcessOption: UInt8, Equatable, Sendable {
        case weightedAveraging = 0
        case layering = 1
    }

    public struct ProcessingWindow: Equatable, Sendable {
        public let bounds: WindowBounds
        public let selector: PixelSelector?
        public let overlapProcessOption: OverlapProcessOption
        public let maxSCL: [Float]
        public let averageMaxRGB: Float?
        public let distributionMaxRGB: [Percentile]
        public let fractionBrightPixels: Float?
        public let toneMapping: ToneMapping?
        public let colorSaturationWeight: Float?

        public init(
            bounds: WindowBounds = .fullFrame,
            selector: PixelSelector? = nil,
            overlapProcessOption: OverlapProcessOption = .weightedAveraging,
            maxSCL: [Float],
            averageMaxRGB: Float?,
            distributionMaxRGB: [Percentile] = [],
            fractionBrightPixels: Float? = nil,
            toneMapping: ToneMapping? = nil,
            colorSaturationWeight: Float? = nil
        ) {
            self.bounds = bounds
            self.selector = selector
            self.overlapProcessOption = overlapProcessOption
            self.maxSCL = maxSCL
            self.averageMaxRGB = averageMaxRGB
            self.distributionMaxRGB = distributionMaxRGB
            self.fractionBrightPixels = fractionBrightPixels
            self.toneMapping = toneMapping
            self.colorSaturationWeight = colorSaturationWeight
        }
    }

    public struct LuminanceGrid: Equatable, Sendable {
        public let rows: Int
        public let columns: Int
        public let values: [Float]

        public init?(rows: Int, columns: Int, values: [Float]) {
            guard rows > 0, columns > 0, !values.isEmpty else {
                return nil
            }
            self.rows = rows
            self.columns = columns
            self.values = values
        }

        public var maximum: Float? {
            values.max()
        }

        public var average: Float? {
            guard !values.isEmpty else {
                return nil
            }
            return values.reduce(Float(0), +) / Float(values.count)
        }
    }

    public let applicationVersion: UInt8
    public let targetedSystemDisplayMaximumLuminance: Float?
    public let processingWindows: [ProcessingWindow]
    public let targetedSystemDisplayActualPeakLuminance: LuminanceGrid?
    public let masteringDisplayActualPeakLuminance: LuminanceGrid?
    /// Runtime copy of the FFmpeg side-data payload for preservation/debugging. This is not a stable persisted format.
    public let rawSideData: Data?

    public init(
        applicationVersion: UInt8,
        targetedSystemDisplayMaximumLuminance: Float?,
        processingWindows: [ProcessingWindow],
        targetedSystemDisplayActualPeakLuminance: LuminanceGrid? = nil,
        masteringDisplayActualPeakLuminance: LuminanceGrid? = nil,
        rawSideData: Data? = nil
    ) {
        self.applicationVersion = applicationVersion
        self.targetedSystemDisplayMaximumLuminance = targetedSystemDisplayMaximumLuminance
        self.processingWindows = processingWindows
        self.targetedSystemDisplayActualPeakLuminance = targetedSystemDisplayActualPeakLuminance
        self.masteringDisplayActualPeakLuminance = masteringDisplayActualPeakLuminance
        self.rawSideData = rawSideData
    }

    public var containsToneMappingCurve: Bool {
        processingWindows.contains { $0.toneMapping != nil }
    }

    public var primaryToneMappingParameters: HDR10PlusToneMappingParameters? {
        HDR10PlusToneMappingParameters(metadata: self)
    }
}

public struct HDR10PlusToneMappingParameters: Equatable, Sendable {
    public let bounds: HDR10PlusMetadata.WindowBounds
    public let selector: HDR10PlusMetadata.PixelSelector?
    public let overlapProcessOption: HDR10PlusMetadata.OverlapProcessOption
    public let targetedSystemDisplayMaximumLuminance: Float?
    public let actualPeakMaximum: Float?
    public let maxSCL: [Float]
    public let averageMaxRGB: Float?
    public let fractionBrightPixels: Float?
    public let kneePointX: Float?
    public let kneePointY: Float?
    public let bezierCurveAnchors: [Float]
    public let colorSaturationWeight: Float?

    public init?(
        bounds: HDR10PlusMetadata.WindowBounds = .fullFrame,
        selector: HDR10PlusMetadata.PixelSelector? = nil,
        overlapProcessOption: HDR10PlusMetadata.OverlapProcessOption = .weightedAveraging,
        targetedSystemDisplayMaximumLuminance: Float?,
        actualPeakMaximum: Float? = nil,
        maxSCL: [Float],
        averageMaxRGB: Float?,
        fractionBrightPixels: Float? = nil,
        kneePointX: Float? = nil,
        kneePointY: Float? = nil,
        bezierCurveAnchors: [Float] = [],
        colorSaturationWeight: Float? = nil
    ) {
        guard !maxSCL.isEmpty || averageMaxRGB != nil || kneePointX != nil || kneePointY != nil || !bezierCurveAnchors.isEmpty else {
            return nil
        }
        self.bounds = bounds
        self.selector = selector
        self.overlapProcessOption = overlapProcessOption
        self.targetedSystemDisplayMaximumLuminance = targetedSystemDisplayMaximumLuminance
        self.actualPeakMaximum = actualPeakMaximum
        self.maxSCL = maxSCL
        self.averageMaxRGB = averageMaxRGB
        self.fractionBrightPixels = fractionBrightPixels
        self.kneePointX = kneePointX
        self.kneePointY = kneePointY
        self.bezierCurveAnchors = bezierCurveAnchors
        self.colorSaturationWeight = colorSaturationWeight
    }

    public init?(metadata: HDR10PlusMetadata) {
        guard let window = metadata.processingWindows.first else {
            return nil
        }
        self.init(metadata: metadata, window: window)
    }

    public init?(metadata: HDR10PlusMetadata, window: HDR10PlusMetadata.ProcessingWindow) {
        self.init(
            bounds: window.bounds,
            selector: window.selector,
            overlapProcessOption: window.overlapProcessOption,
            targetedSystemDisplayMaximumLuminance: metadata.targetedSystemDisplayMaximumLuminance,
            actualPeakMaximum: metadata.effectiveActualPeakMaximum,
            maxSCL: window.maxSCL,
            averageMaxRGB: window.averageMaxRGB,
            fractionBrightPixels: window.fractionBrightPixels,
            kneePointX: window.toneMapping?.kneePointX,
            kneePointY: window.toneMapping?.kneePointY,
            bezierCurveAnchors: window.toneMapping?.bezierCurveAnchors ?? [],
            colorSaturationWeight: window.colorSaturationWeight
        )
    }

    public var hasDynamicCurve: Bool {
        kneePointX != nil && kneePointY != nil && !bezierCurveAnchors.isEmpty
    }
}

public extension HDR10PlusMetadata {
    var effectiveActualPeakMaximum: Float? {
        [
            targetedSystemDisplayActualPeakLuminance?.maximum,
            masteringDisplayActualPeakLuminance?.maximum,
        ]
        .compactMap { $0 }
        .min()
    }
}

public struct HDR10PlusMetalToneMappingWindow: Equatable, Sendable {
    public let control: SIMD4<Float>
    public let scene: SIMD4<Float>
    public let region: SIMD4<Float>
    public let extra: SIMD4<Float>
    public let selector: SIMD4<Float>
    public let selectorAxes: SIMD4<Float>
}

public struct HDR10PlusMetalToneMappingUniform: Equatable, Sendable {
    public static let maxWindowCount = 64

    public let global: SIMD4<Float>
    public let windows: [HDR10PlusMetalToneMappingWindow]
    public let sourceWindowCount: Int
    public var isTruncated: Bool {
        sourceWindowCount > windows.count
    }

    public init?(parameters: HDR10PlusToneMappingParameters) {
        guard let window = Self.windowUniform(parameters: parameters) else {
            return nil
        }
        self.init(global: SIMD4<Float>(1, 1, parameters.actualPeakMaximum ?? 0, 0), windows: [window])
    }

    public init?(metadata: HDR10PlusMetadata) {
        let sourceWindowCount = metadata.processingWindows.count
        let windows = metadata.processingWindows.prefix(Self.maxWindowCount).compactMap {
            HDR10PlusToneMappingParameters(metadata: metadata, window: $0).flatMap(Self.windowUniform(parameters:))
        }
        guard !windows.isEmpty else {
            return nil
        }
        self.init(
            global: SIMD4<Float>(1, Float(windows.count), metadata.effectiveActualPeakMaximum ?? 0, 0),
            windows: windows,
            sourceWindowCount: sourceWindowCount
        )
    }

    static let disabled = HDR10PlusMetalToneMappingUniform(
        global: SIMD4<Float>(0, 0, 0, 0),
        windows: [],
        sourceWindowCount: 0
    )

    private init(
        global: SIMD4<Float>,
        windows: [HDR10PlusMetalToneMappingWindow],
        sourceWindowCount: Int? = nil
    ) {
        self.global = global
        self.windows = windows
        self.sourceWindowCount = sourceWindowCount ?? windows.count
    }

    private static func windowUniform(parameters: HDR10PlusToneMappingParameters) -> HDR10PlusMetalToneMappingWindow? {
        guard parameters.hasDynamicCurve,
              let kneePointX = parameters.kneePointX,
              let kneePointY = parameters.kneePointY
        else {
            return nil
        }
        let targetMaximum = targetMaximum(
            parameters.targetedSystemDisplayMaximumLuminance,
            actualPeakMaximum: parameters.actualPeakMaximum,
            kneePointY: kneePointY
        )
        let rolloffStrength = rolloffStrength(anchors: parameters.bezierCurveAnchors)
        return HDR10PlusMetalToneMappingWindow(
            control: SIMD4<Float>(
                1,
                clamp(kneePointX, min: 0.0001, max: 1),
                clamp(kneePointY, min: 0, max: 1),
                targetMaximum
            ),
            scene: SIMD4<Float>(
                clamp(parameters.maxSCL.max() ?? 1, min: 0.0001, max: 1),
                clamp(parameters.averageMaxRGB ?? 0, min: 0, max: 1),
                clamp(parameters.fractionBrightPixels ?? 0, min: 0, max: 1),
                rolloffStrength
            ),
            region: SIMD4<Float>(
                parameters.bounds.minX,
                parameters.bounds.minY,
                parameters.bounds.maxX,
                parameters.bounds.maxY
            ),
            extra: SIMD4<Float>(
                clamp(parameters.colorSaturationWeight ?? 1, min: 0, max: 2),
                parameters.actualPeakMaximum ?? 0,
                Float(parameters.overlapProcessOption.rawValue),
                parameters.selector == nil ? 0 : 1
            ),
            selector: selector(parameters.selector),
            selectorAxes: selectorAxes(parameters.selector)
        )
    }

    private static let defaultSelector = SIMD4<Float>(0.5, 0.5, 1, 0)
    private static let defaultSelectorAxes = SIMD4<Float>(1, 1, 1, 1)

    private static func selector(_ selector: HDR10PlusMetadata.PixelSelector?) -> SIMD4<Float> {
        guard let selector else {
            return defaultSelector
        }
        return SIMD4<Float>(
            selector.centerX,
            selector.centerY,
            cos(selector.rotationRadians),
            sin(selector.rotationRadians)
        )
    }

    private static func selectorAxes(_ selector: HDR10PlusMetadata.PixelSelector?) -> SIMD4<Float> {
        guard let selector else {
            return defaultSelectorAxes
        }
        return SIMD4<Float>(
            selector.semimajorAxisInternal,
            selector.semiminorAxisInternal,
            selector.semimajorAxisExternal,
            selector.semiminorAxisExternal
        )
    }

    private static func targetMaximum(_ targetNits: Float?, actualPeakMaximum: Float?, kneePointY: Float) -> Float {
        let normalizedTarget = (targetNits ?? 1000) / 10000
        let constrainedTarget = actualPeakMaximum.map { min(normalizedTarget, $0) } ?? normalizedTarget
        return clamp(max(constrainedTarget, kneePointY), min: 0.0001, max: 1)
    }

    private static func rolloffStrength(anchors: [Float]) -> Float {
        guard !anchors.isEmpty else {
            return 1
        }
        let averageAnchor = anchors.reduce(Float(0), +) / Float(anchors.count)
        return clamp(1 + averageAnchor * 6, min: 0.5, max: 8)
    }

    private static func clamp(_ value: Float, min: Float, max: Float) -> Float {
        guard value.isFinite else {
            return min
        }
        return Swift.min(Swift.max(value, min), max)
    }
}

public struct HDR10PlusPlaybackDiagnostic: Equatable, Sendable, CustomStringConvertible {
    public static let metadataOnlyDynamicRangeDescription = "HDR10+ metadata"

    public let renderPath: HDR10PlusRenderPath
    public let toneMappingPolicy: HDR10PlusToneMappingPolicy
    public let metadata: HDR10PlusMetadata?

    public init(
        renderPath: HDR10PlusRenderPath,
        toneMappingPolicy: HDR10PlusToneMappingPolicy,
        metadata: HDR10PlusMetadata? = nil
    ) {
        self.renderPath = renderPath
        self.toneMappingPolicy = toneMappingPolicy
        self.metadata = metadata
    }

    public var appliesApplicationDynamicToneMapping: Bool {
        renderPath == .metalRenderer &&
            toneMappingPolicy == .metalDynamicToneMapping &&
            metadata.flatMap(HDR10PlusMetalToneMappingUniform.init(metadata:)) != nil
    }

    public var description: String {
        switch renderPath {
        case .systemDisplayLayer where toneMappingPolicy == .systemManagedWhenAvailable:
            return "HDR10+ metadata detected; dynamic tone mapping is preserved only if the system display path supports it"
        case .systemDisplayLayer:
            return "HDR10+ metadata detected; using HDR10 base metadata by policy"
        case .metalRenderer:
            if appliesApplicationDynamicToneMapping {
                return "HDR10+ metadata captured; Metal path applies conservative dynamic tone mapping from ST 2094-40 knee/curve metadata"
            }
            if metadata?.primaryToneMappingParameters != nil {
                return "HDR10+ metadata captured; Metal path preserves dynamic metadata and renders HDR10 fallback"
            }
            return "HDR10+ metadata detected; Metal path renders HDR10 fallback because no structured metadata is available"
        }
    }
}

extension DOVIDecoderConfigurationRecord: CustomStringConvertible {
    static func isoBMFFRecord(data: Data) -> DOVIDecoderConfigurationRecord? {
        let bytes = [UInt8](data)
        guard bytes.count >= 4 else {
            return nil
        }
        let packedProfileLevelFlags = UInt16(bytes[2]) << 8 | UInt16(bytes[3])
        let compatibilityID: UInt8
        if bytes.count >= 5 {
            compatibilityID = (bytes[4] >> 4) & 0x0f
        } else {
            compatibilityID = 0
        }
        return DOVIDecoderConfigurationRecord(
            dv_version_major: bytes[0],
            dv_version_minor: bytes[1],
            dv_profile: UInt8((packedProfileLevelFlags >> 9) & 0x7f),
            dv_level: UInt8((packedProfileLevelFlags >> 3) & 0x3f),
            rpu_present_flag: UInt8((packedProfileLevelFlags >> 2) & 0x01),
            el_present_flag: UInt8((packedProfileLevelFlags >> 1) & 0x01),
            bl_present_flag: UInt8(packedProfileLevelFlags & 0x01),
            dv_bl_signal_compatibility_id: compatibilityID
        )
    }

    public var hdrFallbackDynamicRange: DynamicRange? {
        switch dv_profile {
        case 5:
            return .dolbyVision
        case 8 where dv_bl_signal_compatibility_id == 4:
            return .hlg
        case 7:
            return .hdr10
        case 8 where dv_bl_signal_compatibility_id == 1:
            return .hdr10
        default:
            return nil
        }
    }

    public var profile7EnhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind {
        guard dv_profile == 7 else {
            return .notProfile7
        }
        guard el_present_flag != 0 else {
            return .none
        }
        return .unknown
    }

    public var playbackDiagnostic: DolbyVisionPlaybackDiagnostic {
        DolbyVisionPlaybackDiagnostic(configuration: self)
    }

    public func playbackCapability(
        enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind? = nil
    ) -> DolbyVisionPlaybackCapability {
        let enhancementLayerKind = enhancementLayerKind ?? profile7EnhancementLayerKind
        switch dv_profile {
        case 5:
            return .nativeDolbyVision
        case 7:
            switch enhancementLayerKind {
            case .none:
                return .baseLayerHDR10Fallback
            case .minimumEnhancementLayer:
                return .minimumEnhancementLayerFallback
            case .fullEnhancementLayer:
                return .fullEnhancementLayerCompositionUnavailable
            case .unknown:
                return .enhancementLayerKindUnknown
            case .notProfile7:
                return .baseLayerHDR10Fallback
            }
        case 8 where dv_bl_signal_compatibility_id == 1:
            return .baseLayerHDR10Fallback
        default:
            return hdrFallbackDynamicRange == .dolbyVision ? .nativeDolbyVision : .sourceMetadataFallback
        }
    }

    public var profileDescription: String {
        switch dv_profile {
        case 0:
            return "AVC dual-layer early mobile/legacy"
        case 1:
            return "AVC single-layer mobile/legacy"
        case 2:
            return "AVC dual-layer SDR deprecated"
        case 3:
            return "AVC single-layer SDR legacy streaming"
        case 4:
            return "AVC dual-layer SDR legacy"
        case 5:
            return "HEVC single-layer modern streaming"
        case 6:
            return "HEVC dual-layer SDR legacy hybrid"
        case 7:
            return "HEVC dual-layer UHD Blu-ray"
        case 8 where dv_bl_signal_compatibility_id == 1:
            return "HEVC single-layer HDR10-compatible streaming/broadcast"
        case 8 where dv_bl_signal_compatibility_id == 2:
            return "HEVC single-layer SDR-compatible broadcast"
        case 8 where dv_bl_signal_compatibility_id == 4:
            return "HEVC single-layer HLG-compatible broadcast"
        case 8:
            return "HEVC single-layer compatibility \(dv_bl_signal_compatibility_id)"
        case 9:
            return "AVC single-layer mobile low-bandwidth"
        case 10:
            return "HEVC dual-layer studio/mezzanine"
        case 11:
            return "HEVC single-layer studio/mezzanine"
        case 12:
            return "AVC dual-layer legacy mezzanine/studio"
        case 13:
            return "AVC single-layer legacy mezzanine/studio"
        default:
            return "unclassified"
        }
    }

    public var enhancementLayerDescription: String? {
        guard el_present_flag != 0 else {
            return nil
        }
        switch dv_profile {
        case 7:
            return "enhancement layer present; MEL/FEL requires RPU/EL packet diagnostics and FEL residuals are not composed"
        case 10, 12:
            return "enhancement layer present; full dual-layer composition is not implemented"
        default:
            return "enhancement layer present"
        }
    }

    public var fallbackDescription: String {
        switch dv_profile {
        case 7 where el_present_flag != 0:
            return "HDR10 base layer; Profile 7 enhancement layer is diagnostic-only"
        case 7:
            return "HDR10 base layer"
        default:
            break
        }
        if let hdrFallbackDynamicRange {
            return hdrFallbackDynamicRange.description
        }
        switch dv_profile {
        case 2, 3, 4, 6,
             8 where dv_bl_signal_compatibility_id == 2:
            return "source SDR/base metadata"
        case 10, 11, 12, 13:
            return "source/base metadata for studio profile"
        default:
            return "source metadata"
        }
    }

    public var enhancementLayerCompositionSupport: DolbyVisionEnhancementLayerCompositionSupport {
        guard el_present_flag != 0 else {
            return .notRequired
        }
        switch dv_profile {
        case 7:
            return .unsupported(
                reason: "Profile 7 enhancement layer detected; KSPlayer has no dual-layer EL decoder/compositor, so FEL residual detail is not composed"
            )
        case 10, 12:
            return .unsupported(
                reason: "dual-layer Dolby Vision composition is not implemented for this profile"
            )
        default:
            return .notRequired
        }
    }

    public var playbackDescription: String {
        if dv_profile == 7 {
            return "Dolby Vision profile 7 (\(playbackCapability().description))"
        }
        if let hdrFallbackDynamicRange {
            return "Dolby Vision profile \(dv_profile) (fallback \(hdrFallbackDynamicRange.description))"
        }
        return "Dolby Vision profile \(dv_profile) (\(fallbackDescription))"
    }

    public var description: String {
        var description = "Dolby Vision profile \(dv_profile) (\(profileDescription)), level \(dv_level), rpu \(rpu_present_flag), " +
            "el \(el_present_flag), bl \(bl_present_flag), compatibility \(dv_bl_signal_compatibility_id)"
        if let enhancementLayerDescription {
            description += ", \(enhancementLayerDescription)"
        }
        if case let .unsupported(reason) = enhancementLayerCompositionSupport {
            description += ", \(reason)"
        }
        description += ", fallback \(fallbackDescription)"
        return description
    }
}

public enum FFmpegFieldOrder: UInt8, Sendable {
    case unknown = 0
    case progressive
    case tt // < Top coded_first, top displayed first
    case bb // < Bottom coded first, bottom displayed first
    case tb // < Top coded first, bottom displayed first
    case bt // < Bottom coded first, top displayed first
}

public extension FFmpegFieldOrder {
    static func detected(formatDescription: CMFormatDescription?) -> FFmpegFieldOrder {
        guard let formatDescription,
              let extensions = CMFormatDescriptionGetExtensions(formatDescription) as? [AnyHashable: Any]
        else {
            return .unknown
        }

        let fieldCount = intValue(extensionValue(in: extensions, for: kCMFormatDescriptionExtension_FieldCount))
        if fieldCount == 1 {
            return .progressive
        }

        guard let fieldDetail = stringValue(extensionValue(in: extensions, for: kCMFormatDescriptionExtension_FieldDetail)) else {
            return .unknown
        }
        switch tokenized(fieldDetail) {
        case "temporaltopfirst", "spatialfirstlineearly", "topfirst", "top":
            return .tt
        case "temporalbottomfirst", "spatialfirstlinelate", "bottomfirst", "bottom":
            return .bb
        case "progressive":
            return .progressive
        default:
            return .unknown
        }
    }

    var coreMediaFieldCount: Int? {
        switch self {
        case .unknown:
            return nil
        case .progressive:
            return 1
        case .tt, .bb, .tb, .bt:
            return 2
        }
    }

    var coreMediaFieldDetail: CFString? {
        switch self {
        case .tt, .tb:
            return kCMFormatDescriptionFieldDetail_TemporalTopFirst
        case .bb, .bt:
            return kCMFormatDescriptionFieldDetail_TemporalBottomFirst
        case .unknown, .progressive:
            return nil
        }
    }

    private static func extensionValue(in extensions: [AnyHashable: Any], for key: CFString) -> Any? {
        let targetKey = key as String
        return extensions.first { String(describing: $0.key) == targetKey }?.value
    }

    private static func intValue(_ value: Any?) -> Int? {
        if let value = value as? Int {
            return value
        }
        if let value = value as? NSNumber {
            return value.intValue
        }
        if let value = value as? String {
            return Int(value)
        }
        return nil
    }

    private static func stringValue(_ value: Any?) -> String? {
        if let value = value as? String {
            return value
        }
        if let value {
            return String(describing: value)
        }
        return nil
    }

    private static func tokenized(_ value: String) -> String {
        value.lowercased().filter { $0.isLetter || $0.isNumber }
    }
}

extension FFmpegFieldOrder: CustomStringConvertible {
    public var description: String {
        switch self {
        case .unknown:
            return "unknown"
        case .progressive:
            return "progressive"
        case .tt:
            return "top first"
        case .bb:
            return "bottom first"
        case .tb:
            return "top coded first (swapped)"
        case .bt:
            return "bottom coded first (swapped)"
        }
    }
}

// swiftlint:enable identifier_name
public extension MediaPlayerTrack {
    var subtitleKind: SubtitleKind {
        (self as? any SubtitleKindProviding)?.subtitleKind ?? (isImageSubtitle ? .image : .text)
    }

    var language: String? {
        languageCode.flatMap {
            Locale.current.localizedString(forLanguageCode: $0)
        }
    }

    var codecType: FourCharCode {
        mediaSubType.rawValue
    }

    var dynamicRange: DynamicRange? {
        if let dovi {
            return dovi.hdrFallbackDynamicRange ?? formatDescription?.dynamicRange
        } else {
            return formatDescription?.dynamicRange
        }
    }

    var dynamicRangeDescription: String {
        if let dovi {
            return dovi.playbackDescription
        }
        if let track = self as? FFmpegAssetTrack, track.hasHDR10PlusMetadata {
            return HDR10PlusPlaybackDiagnostic.metadataOnlyDynamicRangeDescription
        }
        return dynamicRange?.description ?? DynamicRange.sdr.description
    }

    var colorSpace: CGColorSpace? {
        KSOptions.colorSpace(ycbcrMatrix: yCbCrMatrix as CFString?, transferFunction: transferFunction as CFString?)
    }

    var mediaSubType: CMFormatDescription.MediaSubType {
        formatDescription?.mediaSubType ?? .boxed
    }

    var audioStreamBasicDescription: AudioStreamBasicDescription? {
        formatDescription?.audioStreamBasicDescription
    }

    var naturalSize: CGSize {
        formatDescription?.naturalSize ?? .zero
    }

    var colorPrimaries: String? {
        formatDescription?.colorPrimaries
    }

    var transferFunction: String? {
        formatDescription?.transferFunction
    }

    var yCbCrMatrix: String? {
        formatDescription?.yCbCrMatrix
    }
}

public extension CMFormatDescription {
    var dynamicRange: DynamicRange {
        let contentRange: DynamicRange
        if isDolbyVisionVideo || dolbyVisionConfigurationRecord != nil {
            contentRange = .dolbyVision
        } else if transferFunction == kCVImageBufferTransferFunction_ITU_R_2100_HLG as String { /// HLG
            contentRange = .hlg
        } else if transferFunction == kCVImageBufferTransferFunction_SMPTE_ST_2084_PQ as String { /// HDR10/HDR10+
            contentRange = .hdr10
        } else {
            contentRange = .sdr
        }
        return contentRange
    }

    var dolbyVisionConfigurationRecord: DOVIDecoderConfigurationRecord? {
        guard let extensions = CMFormatDescriptionGetExtensions(self) as? [AnyHashable: Any],
              let atoms = extensionValue(in: extensions, for: kCMFormatDescriptionExtension_SampleDescriptionExtensionAtoms) as? [AnyHashable: Any]
        else {
            return nil
        }
        for atomName in ["dvcC", "dvvC", "dvwC"] {
            guard let atom = atoms.first(where: { String(describing: $0.key) == atomName })?.value else {
                continue
            }
            if let data = atom as? Data {
                return DOVIDecoderConfigurationRecord.isoBMFFRecord(data: data)
            }
            if let data = atom as? NSData {
                return DOVIDecoderConfigurationRecord.isoBMFFRecord(data: data as Data)
            }
        }
        return nil
    }

    var bitDepth: Int32 {
        codecType.bitDepth
    }

    var codecType: FourCharCode {
        mediaSubType.rawValue
    }

    private var isDolbyVisionVideo: Bool {
        ["dvhe", "dvh1", "dvav", "dva1"].contains(codecType.string)
    }

    var colorPrimaries: String? {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCVImageBufferColorPrimariesKey] as? String
        } else {
            return nil
        }
    }

    var transferFunction: String? {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCVImageBufferTransferFunctionKey] as? String
        } else {
            return nil
        }
    }

    var yCbCrMatrix: String? {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCVImageBufferYCbCrMatrixKey] as? String
        } else {
            return nil
        }
    }

    var naturalSize: CGSize {
        let aspectRatio = aspectRatio
        return CGSize(width: Int(dimensions.width), height: Int(CGFloat(dimensions.height) * aspectRatio.height / aspectRatio.width))
    }

    var aspectRatio: CGSize {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            if let ratio = dictionary[kCVImageBufferPixelAspectRatioKey] as? NSDictionary,
               let horizontal = (ratio[kCVImageBufferPixelAspectRatioHorizontalSpacingKey] as? NSNumber)?.intValue,
               let vertical = (ratio[kCVImageBufferPixelAspectRatioVerticalSpacingKey] as? NSNumber)?.intValue,
               horizontal > 0, vertical > 0
            {
                return CGSize(width: horizontal, height: vertical)
            }
        }
        return CGSize(width: 1, height: 1)
    }

    var depth: Int32 {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCMFormatDescriptionExtension_Depth] as? Int32 ?? 24
        } else {
            return 24
        }
    }

    var fullRangeVideo: Bool {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCMFormatDescriptionExtension_FullRangeVideo] as? Bool ?? false
        } else {
            return false
        }
    }

    private func extensionValue(in extensions: [AnyHashable: Any], for key: CFString) -> Any? {
        let targetKey = key as String
        return extensions.first { String(describing: $0.key) == targetKey }?.value
    }
}

extension CMFormatDescription.MediaSubType {
    static let dolbyDigital = CMFormatDescription.MediaSubType(rawValue: "ac-3".fourCharCode)
    static let dolbyDigitalPlus = CMFormatDescription.MediaSubType(rawValue: "ec-3".fourCharCode)
    static let dolbyAC4 = CMFormatDescription.MediaSubType(rawValue: "ac-4".fourCharCode)
    static let dolbyTrueHD = CMFormatDescription.MediaSubType(rawValue: "mlpa".fourCharCode)

    var audioCodecDisplayName: String? {
        switch self {
        case .dolbyDigital:
            return "Dolby Digital"
        case .dolbyDigitalPlus:
            return "Dolby Digital Plus"
        case .dolbyAC4:
            return "Dolby AC-4"
        case .dolbyTrueHD:
            return "Dolby TrueHD"
        default:
            return nil
        }
    }
}

func setHttpProxy() {
    guard KSOptions.useSystemHTTPProxy else {
        return
    }
    guard let proxySettings = CFNetworkCopySystemProxySettings()?.takeUnretainedValue() as? NSDictionary else {
        unsetenv("http_proxy")
        return
    }
    guard let proxyHost = proxySettings[kCFNetworkProxiesHTTPProxy] as? String, let proxyPort = proxySettings[kCFNetworkProxiesHTTPPort] as? Int else {
        unsetenv("http_proxy")
        return
    }
    let httpProxy = "http://\(proxyHost):\(proxyPort)"
    setenv("http_proxy", httpProxy, 0)
}

#if os(visionOS) && canImport(CoreML) && canImport(Metal)
import Combine
@preconcurrency import CoreMedia
@preconcurrency import CoreML
@preconcurrency import CoreVideo
import Foundation
import KSPlayer
@preconcurrency import Metal

/// Xcode scheme environment overrides for on-device experiments (no UI required).
public enum DA3DemoEnvironment: Sendable {
    /// Set `DA3_INFERENCE_FPS=60` to raise the DA3 acceptance cap (device may still measure lower).
    public static var inferenceFPSOverride: Double? {
        guard let raw = ProcessInfo.processInfo.environment["DA3_INFERENCE_FPS"],
              let fps = Double(raw),
              fps.isFinite,
              fps > 0
        else {
            return nil
        }
        return fps
    }

    /// Set `DA3_START_SECONDS=60` to seek once when playback becomes ready.
    public static var startSecondsOverride: TimeInterval? {
        guard let raw = ProcessInfo.processInfo.environment["DA3_START_SECONDS"],
              let seconds = Double(raw),
              seconds.isFinite,
              seconds >= 0
        else {
            return nil
        }
        return seconds
    }

    /// Set `DA3_DEPTH_WARMUP_SECONDS=3` to run muted depth inference before starting from the seek point.
    public static var depthWarmupSecondsOverride: TimeInterval? {
        guard let raw = ProcessInfo.processInfo.environment["DA3_DEPTH_WARMUP_SECONDS"],
              let seconds = Double(raw),
              seconds.isFinite,
              seconds > 0
        else {
            return nil
        }
        return seconds
    }

    /// `DA3_COMPUTE_UNITS=strict_ane|ane|all|cpu` — see `DepthAnythingV3Engine.resolvedModelConfiguration`.
    public static var computeUnitsOverride: String? {
        let raw = ProcessInfo.processInfo.environment["DA3_COMPUTE_UNITS"]?
            .trimmingCharacters(in: .whitespacesAndNewlines)
        guard let raw, !raw.isEmpty else {
            return nil
        }
        return raw
    }
}

public enum KSPlayer3DIntegrationError: Error, Equatable, LocalizedError, Sendable {
    case metalDeviceUnavailable

    public var errorDescription: String? {
        switch self {
        case .metalDeviceUnavailable:
            return "Vision Pro Depth Anything V3 integration requires a Metal device."
        }
    }
}

public enum DA3ProcessingState: Equatable, Sendable {
    case disabled
    case idle
    case processing
    case active
    case failed(String)

    public var displayName: String {
        switch self {
        case .disabled:
            return "Disabled"
        case .idle:
            return "Ready"
        case .processing:
            return "Processing"
        case .active:
            return "Active"
        case let .failed(message):
            return "Error: \(message)"
        }
    }
}

public enum DA3VisionProPerformanceMode: String, CaseIterable, Equatable, Sendable {
    case stability
    case balanced
    case performance
    case maximum

    public var displayName: String {
        switch self {
        case .stability:
            return "Stability"
        case .balanced:
            return "Balanced"
        case .performance:
            return "Performance"
        case .maximum:
            return "Maximum"
        }
    }

    var maximumInferenceFPS: Double {
        switch self {
        case .stability:
            return 6
        case .balanced:
            return 12
        case .performance:
            return 18
        case .maximum:
            return 30
        }
    }

    /// When true, start the next depth job as soon as the previous finishes (still throttled by adaptive FPS).
    var usesContinuousDepthScheduling: Bool {
        switch self {
        case .stability, .balanced:
            return false
        case .performance, .maximum:
            return true
        }
    }

    var temporalSmoothingFactor: Float {
        switch self {
        case .stability:
            return 0.86
        case .balanced:
            return 0.62
        case .performance:
            return 0.35
        case .maximum:
            return 0.12
        }
    }

    var depthStrength: Float {
        switch self {
        case .stability:
            return 0.45
        case .balanced:
            return 0.55
        case .performance:
            return 0.62
        case .maximum:
            return 0.58
        }
    }

    var depthDistance: Float {
        switch self {
        case .stability:
            return 1.1
        case .balanced:
            return 1.25
        case .performance:
            return 1.35
        case .maximum:
            return 1.3
        }
    }

    var depthCurvature: Float {
        switch self {
        case .stability:
            return 0.9
        case .balanced:
            return 0.8
        case .performance:
            return 0.7
        case .maximum:
            return 0.75
        }
    }

    var depthContrast: Float {
        switch self {
        case .stability:
            return 1.15
        case .balanced:
            return 1.25
        case .performance:
            return 1.35
        case .maximum:
            return 1.2
        }
    }
}

public enum DA3DebugRenderMode: String, CaseIterable, Equatable, Sendable {
    case normal
    case depthDisabled
    case depthOnly
    case stereoOnly

    public var displayName: String {
        switch self {
        case .normal:
            return "Normal"
        case .depthDisabled:
            return "Depth Disabled"
        case .depthOnly:
            return "Depth Only"
        case .stereoOnly:
            return "Stereo Only"
        }
    }

    var capturesDepth: Bool {
        switch self {
        case .normal, .depthOnly:
            return true
        case .depthDisabled, .stereoOnly:
            return false
        }
    }
}

public enum DA3PresentationMode: String, CaseIterable, Equatable, Sendable {
    case native2D
    case windowSelectedEye
    case packedPreview
    case immersiveStereo

    public var displayName: String {
        switch self {
        case .native2D:
            return "2D"
        case .windowSelectedEye:
            return "Window Eye"
        case .packedPreview:
            return "Packed Preview"
        case .immersiveStereo:
            return "Immersive Stereo"
        }
    }
}

public struct DA3PipelineMetrics: Equatable, Sendable {
    public let state: DA3ProcessingState
    public let processedFrameCount: Int
    public let droppedFrameCount: Int
    public let coalescedFrameCount: Int
    public let throttledFrameCount: Int
    public let staleDepthFrameCount: Int
    public let processingFPS: Double
    public let lastFrameDuration: TimeInterval?
    public let depthFrameOffset: TimeInterval?
    public let depthWidth: Int
    public let depthHeight: Int
    public let rawDepthMinimum: Float?
    public let rawDepthMaximum: Float?
    public let normalizedDepthMinimum: Float?
    public let normalizedDepthMaximum: Float?
    public let maximumInferenceFPS: Double
    public let sourceVideoFrameRate: Double
    public let actualInferenceFPS: Double
    public let temporalSmoothingFactor: Float
    public let preprocessingDuration: TimeInterval?
    public let inferenceDuration: TimeInterval?
    public let outputExtractionDuration: TimeInterval?
    public let textureUploadDuration: TimeInterval?
    public let depthMapDuration: TimeInterval?
    public let rendererUpdateDuration: TimeInterval?
    public let renderDepthTextureUploadDuration: TimeInterval?
    public let renderDepthSmoothingDuration: TimeInterval?
    public let renderDuration: TimeInterval?
    public let renderStereoPassCount: Int
    public let performanceMode: DA3VisionProPerformanceMode
    public let debugRenderMode: DA3DebugRenderMode
    public let modelDiagnostics: DA3ModelDiagnostics
    public let depthContrast: Float
    public let isDepthInverted: Bool

    public static let empty = DA3PipelineMetrics(
        state: .disabled,
        processedFrameCount: 0,
        droppedFrameCount: 0,
        coalescedFrameCount: 0,
        throttledFrameCount: 0,
        staleDepthFrameCount: 0,
        processingFPS: 0,
        lastFrameDuration: nil,
        depthFrameOffset: nil,
        depthWidth: 0,
        depthHeight: 0,
        rawDepthMinimum: nil,
        rawDepthMaximum: nil,
        normalizedDepthMinimum: nil,
        normalizedDepthMaximum: nil,
        maximumInferenceFPS: 0,
        sourceVideoFrameRate: 0,
        actualInferenceFPS: 0,
        temporalSmoothingFactor: 0,
        preprocessingDuration: nil,
        inferenceDuration: nil,
        outputExtractionDuration: nil,
        textureUploadDuration: nil,
        depthMapDuration: nil,
        rendererUpdateDuration: nil,
        renderDepthTextureUploadDuration: nil,
        renderDepthSmoothingDuration: nil,
        renderDuration: nil,
        renderStereoPassCount: 0,
        performanceMode: .balanced,
        debugRenderMode: .normal,
        modelDiagnostics: .empty,
        depthContrast: 1,
        isDepthInverted: false
    )
}

private enum DA3VisionProDemoTuning {
    static let defaultPerformanceMode = DA3VisionProPerformanceMode.maximum
    static let staleDepthFrameOffset: TimeInterval = 0.55
    /// Muted playback window so DA3 can run ahead before the user-visible start.
    static let depthWarmupMinimumSeconds: TimeInterval = 2.5
    static let depthWarmupMaximumSeconds: TimeInterval = 8
    static let depthWarmupMinimumDepthFrames = 2

    static var depthWarmupMinimumSecondsEffective: TimeInterval {
        DA3DemoEnvironment.depthWarmupSecondsOverride ?? depthWarmupMinimumSeconds
    }
    static let defaultDepthStrength: Float = 0.55
    static let defaultDepthDistance: Float = 1.25
    static let defaultDepthCurvature: Float = 0.8
    static let defaultDepthContrast: Float = 1.25
    static let depthStrengthRange: ClosedRange<Float> = 0 ... 1
    static let depthDistanceRange: ClosedRange<Float> = 0 ... 2
    static let depthCurvatureRange: ClosedRange<Float> = 0.25 ... 3
    static let depthContrastRange: ClosedRange<Float> = 0.5 ... 3
    static let temporalSmoothingFactorRange: ClosedRange<Float> = Video2DTo3DPolicy.depthSmoothingFactorRange

    static func validatedDepthStrength(_ value: Float) -> Float {
        validated(value, range: depthStrengthRange, fallback: defaultDepthStrength)
    }

    static func validatedDepthDistance(_ value: Float) -> Float {
        validated(value, range: depthDistanceRange, fallback: defaultDepthDistance)
    }

    static func validatedDepthCurvature(_ value: Float) -> Float {
        validated(value, range: depthCurvatureRange, fallback: defaultDepthCurvature)
    }

    static func validatedDepthContrast(_ value: Float) -> Float {
        validated(value, range: depthContrastRange, fallback: defaultDepthContrast)
    }

    static func validatedTemporalSmoothingFactor(_ value: Float) -> Float {
        Video2DTo3DPolicy.validatedDepthSmoothingFactor(value)
    }

    private static func validated(_ value: Float, range: ClosedRange<Float>, fallback: Float) -> Float {
        guard value.isFinite else {
            return fallback
        }
        return min(max(value, range.lowerBound), range.upperBound)
    }
}

@MainActor
public final class KSPlayer3DIntegrationViewModel: ObservableObject {
    @Published public private(set) var lastErrorMessage: String?
    @Published public private(set) var lastActionMessage = "Ready"
    @Published public private(set) var lastPlayerStateMessage = "initialized"
    @Published public private(set) var playerState = KSPlayerState.initialized
    @Published public private(set) var is2DTo3DEnabled: Bool
    @Published public private(set) var is3DPreviewRequested: Bool
    @Published public private(set) var isApplying2DTo3DMode = false
    @Published public private(set) var isLoopEnabled: Bool
    @Published public private(set) var depthStrength: Float
    @Published public private(set) var depthDistance: Float
    @Published public private(set) var depthCurvature: Float
    @Published public private(set) var outputLayout: Video2DTo3DOutputLayout
    @Published public private(set) var depthContrast: Float
    @Published public private(set) var isDepthInverted: Bool
    @Published public private(set) var temporalSmoothingFactor: Float
    @Published public private(set) var metrics = DA3PipelineMetrics.empty
    @Published public private(set) var depthWarmupStatusMessage = ""
    @Published public private(set) var lastAppliedMode = "disabled"
    @Published public private(set) var activationStatusMessage = "Idle"
    @Published public private(set) var previewModeDescription = "Native 2D playback"
    @Published public private(set) var debugPhaseMessage = "Idle"
    @Published public private(set) var lastDebugMessage = "No depth debug events"
    @Published public private(set) var controlEventMessage = "No 3D control events"
    @Published public private(set) var controlEventCount = 0
    @Published public private(set) var presentationMode: DA3PresentationMode
    @Published public private(set) var presentationModeStatus = "Native 2D playback"
    @Published public private(set) var immersiveTemporalStatusLine = ""
    @Published public private(set) var immersiveScreenDistanceMeters: Float
    @Published public private(set) var immersiveScreenOffsetRightMeters: Float
    @Published public private(set) var immersiveScreenOffsetUpMeters: Float
    #if os(visionOS)
    @Published public private(set) var immersivePlaybackDiagnostics = ImmersivePlaybackDiagnostics.idle
    #endif
    @Published public private(set) var isImmersiveStereoPresented = false
    @Published public private(set) var isOpeningImmersiveStereo = false
    @Published public private(set) var performanceMode: DA3VisionProPerformanceMode
    @Published public private(set) var debugRenderMode: DA3DebugRenderMode = .normal

    @Published public private(set) var url: URL
    public let options: KSOptions
    public let coordinator: KSVideoPlayer.Coordinator
    public let renderer: any StereoRendererProtocol
    public var depthStrengthRange: ClosedRange<Float> { DA3VisionProDemoTuning.depthStrengthRange }
    public var depthDistanceRange: ClosedRange<Float> { DA3VisionProDemoTuning.depthDistanceRange }
    public var depthCurvatureRange: ClosedRange<Float> { DA3VisionProDemoTuning.depthCurvatureRange }
    public var depthContrastRange: ClosedRange<Float> { DA3VisionProDemoTuning.depthContrastRange }
    public var temporalSmoothingFactorRange: ClosedRange<Float> { DA3VisionProDemoTuning.temporalSmoothingFactorRange }
    public var immersiveScreenDistanceRange: ClosedRange<Float> { ImmersiveScreenPlacement.distanceRangeMeters }
    public var immersiveScreenHorizontalOffsetRange: ClosedRange<Float> { ImmersiveScreenPlacement.horizontalOffsetRangeMeters }
    public var immersiveScreenVerticalOffsetRange: ClosedRange<Float> { ImmersiveScreenPlacement.verticalOffsetRangeMeters }

    private let pipeline: DA3VideoOutputPipeline
    private let videoFrameSink = DA3DecodedVideoFrameSink()
    private let depthMapProvider: DA3CachedDepthMapProvider
    private weak var installedLayer: KSPlayerLayer?
    private var activationTask: Task<Void, Never>?
    private var firstFrameWaitTask: Task<Void, Never>?
    private var hasReceivedFirst3DFrame = false
    private var depthStrengthRampStartedAt: TimeInterval?
    private var isDepthWarmupActive = false
    private var depthWarmupStartedAt: Date?
    private var wasMutedBeforeDepthWarmup = false
    private var activationSequence = 0
    private let firstFrameTimeout: TimeInterval = 8
    private var openImmersiveStereo: (() async -> Bool)?
    private var dismissImmersiveStereo: (() async -> Void)?
    private var immersiveOpenFailureReason: String?
    private var immersivePresentationDrainTask: Task<Void, Never>?
    private var immersiveTimelineSyncTask: Task<Void, Never>?
    #if os(visionOS)
    private var immersiveDiagnosticsTask: Task<Void, Never>?
    #endif
    private var isConfiguringVideoOutput = false
    private var isVideoOutputConfigured = false
    #if os(visionOS)
    private var hasLoggedFirstImmersiveAudioSyncedFrame = false
    #endif
    private var hasFinished3DPreviewActivation = false
    private var didAttemptLaunch3DPreviewActivation = false

    public convenience init(
        url: URL,
        options: KSOptions = KSOptions(),
        modelConfiguration: MLModelConfiguration? = nil,
        allowsComputeUnitFallback: Bool? = nil
    ) throws {
        guard let device = MTLCreateSystemDefaultDevice() else {
            throw KSPlayer3DIntegrationError.metalDeviceUnavailable
        }
        nonisolated(unsafe) let suppliedModelConfiguration = modelConfiguration
        let engine = try DepthAnythingV3Engine(
            configuration: suppliedModelConfiguration,
            allowsComputeUnitFallback: allowsComputeUnitFallback
        )
        self.init(url: url, options: options, engine: engine, renderer: StereoRenderer(), device: device)
    }

    public init(
        url: URL,
        options: KSOptions = KSOptions(),
        engine: DepthAnythingV3Engine,
        renderer: any StereoRendererProtocol,
        device: any MTLDevice
    ) {
        self.url = url
        self.options = options
        // DA3 always needs FFmpeg + KSVideoFrameOutput; switching decoders mid-playback causes visible jumps.
        Self.applyStableFrameOutputDecodeOptions(to: options)
        self.coordinator = KSVideoPlayer.Coordinator()
        self.renderer = renderer
        options.video2DTo3DDepthStrength = DA3VisionProDemoTuning.validatedDepthStrength(options.video2DTo3DDepthStrength)
        options.video2DTo3DDepthDistance = DA3VisionProDemoTuning.validatedDepthDistance(options.video2DTo3DDepthDistance)
        options.video2DTo3DDepthCurvature = DA3VisionProDemoTuning.validatedDepthCurvature(options.video2DTo3DDepthCurvature)
        let initialOutputLayout = options.video2DTo3DMode.isEnabled ? options.video2DTo3DOutputLayout : .selectedEye
        let initialPerformanceMode = DA3VisionProDemoTuning.defaultPerformanceMode
        let initialDepthContrast = initialPerformanceMode.depthContrast
        let initialDepthInverted = false
        self.is2DTo3DEnabled = options.video2DTo3DMode.isEnabled
        self.isLoopEnabled = options.isLoopPlay
        self.depthStrength = initialPerformanceMode.depthStrength
        self.depthDistance = initialPerformanceMode.depthDistance
        self.depthCurvature = initialPerformanceMode.depthCurvature
        self.temporalSmoothingFactor = initialPerformanceMode.temporalSmoothingFactor
        self.outputLayout = initialOutputLayout
        self.performanceMode = initialPerformanceMode
        options.video2DTo3DDepthStrength = initialPerformanceMode.depthStrength
        options.video2DTo3DDepthDistance = initialPerformanceMode.depthDistance
        options.video2DTo3DDepthCurvature = initialPerformanceMode.depthCurvature
        options.video2DTo3DDepthSmoothingFactor = initialPerformanceMode.temporalSmoothingFactor
        options.video2DTo3DOutputLayout = initialOutputLayout
        self.depthContrast = initialDepthContrast
        self.isDepthInverted = initialDepthInverted
        let initialScreenDistance = ImmersiveScreenPlacement.validatedDistance(
            ImmersiveScreenPlacement.resolvedScreenDistanceMeters()
        )
        ImmersiveScreenPlacement.setDistanceMeters(initialScreenDistance)
        self.immersiveScreenDistanceMeters = initialScreenDistance
        self.immersiveScreenOffsetRightMeters = ImmersiveScreenPlacement.currentOffsetRightMeters
        self.immersiveScreenOffsetUpMeters = ImmersiveScreenPlacement.currentOffsetUpMeters
        self.is3DPreviewRequested = options.video2DTo3DMode.isEnabled
        self.presentationMode = options.video2DTo3DMode.isEnabled
            ? Self.presentationMode(for: initialOutputLayout)
            : .native2D
        self.lastAppliedMode = Self.appliedModeName(isEnabled: options.video2DTo3DMode.isEnabled, layout: initialOutputLayout)
        let bridge = DA3DepthMetalBridge(device: device)
        let depthGPUProcessor = try? DA3DepthGPUProcessor(device: bridge.metalDevice)
        let pipeline = DA3VideoOutputPipeline(
            engine: engine,
            bridge: bridge,
            depthGPUProcessor: depthGPUProcessor,
            renderer: renderer,
            performanceMode: initialPerformanceMode,
            debugRenderMode: .normal,
            staleDepthFrameOffset: DA3VisionProDemoTuning.staleDepthFrameOffset,
            depthContrast: initialDepthContrast,
            isDepthInverted: initialDepthInverted
        )
        self.pipeline = pipeline
        pipeline.setTemporalSmoothingFactor(initialPerformanceMode.temporalSmoothingFactor)
        videoFrameSink.pipeline = pipeline
        let depthMapProvider = DA3CachedDepthMapProvider(pipeline: pipeline)
        self.depthMapProvider = depthMapProvider
        options.videoDepthEstimationProvider = is2DTo3DEnabled ? depthMapProvider : nil
        pipeline.setEnabled(is2DTo3DEnabled)
        pipeline.setErrorHandler { [weak self] error in
            self?.lastErrorMessage = error.localizedDescription
        }
        pipeline.setMetricsHandler { [weak self] metrics in
            self?.metrics = metrics
            self?.refreshImmersivePresentationStatusIfNeeded()
            self?.updateDepthWarmupProgress(with: metrics)
        }
        pipeline.setVideoFrameHandler { [weak self] mediaTime in
            self?.handleFirst3DFrame(mediaTime: mediaTime)
        }
        applyInferenceEnvironmentOverridesIfNeeded()
        pipeline.setRenderConfigurationProvider { [weak self] in
            guard let self else {
                return .disabled
            }
            if self.shouldDeliverVideoFramesForImmersive || self.isImmersiveStereoPresented || self.isOpeningImmersiveStereo {
                // Cinema shares ImmersiveVideoFeed but must stay flat until depth is window-synced.
                let immersiveCompositorActive = self.isImmersiveStereoPresented || self.isOpeningImmersiveStereo
                let hasDepth = immersiveCompositorActive && self.metrics.processedFrameCount > 0
                return self.immersiveStereoRenderConfiguration(hasDepthMap: hasDepth)
            }
            guard self.is2DTo3DEnabled || self.is3DPreviewRequested else {
                return .disabled
            }
            return self.options.video2DTo3DRenderConfiguration(hasDepthMap: true)
        }
        pipeline.setImmersivePresentationSync { [weak self] in
            self?.syncImmersivePresentationStateIfNeeded()
        }
        Depth3DDebug.setEventHandler { [weak self] event in
            self?.handleDebugEvent(event)
        }
        options.video2DTo3DRenderMetricsHandler = { [weak pipeline] renderMetrics in
            pipeline?.recordRenderMetrics(renderMetrics)
        }
        #if os(visionOS)
        wireImmersiveAudioSyncedVideoPresentation()
        #endif
    }

    #if os(visionOS)
    private var usesImmersiveAudioSyncedPresentation: Bool {
        options.suppressWindowVideoPresentationWhileImmersiveCompositorActive
            && options.immersivePresentVideoFrame != nil
    }

    private func wireImmersiveAudioSyncedVideoPresentation() {
        options.immersivePresentVideoFrame = { [weak self] pixelBuffer, presentationSeconds in
            guard let self else { return }
            // `CVPixelBuffer` is not `Sendable`. In practice the buffer is immutable for our read-only
            // usage (we immediately normalize/copy if needed), so we pass it through an unchecked
            // sendable wrapper to avoid Swift 6 concurrency warnings without introducing copies.
            let sendablePixelBuffer = UncheckedSendablePixelBuffer(pixelBuffer)
            if Thread.isMainThread {
                MainActor.assumeIsolated {
                    self.presentStereoCompositorFrame(
                        pixelBuffer: sendablePixelBuffer.value,
                        presentationSeconds: presentationSeconds
                    )
                }
            } else {
                Task { @MainActor [weak self] in
                    self?.presentStereoCompositorFrame(
                        pixelBuffer: sendablePixelBuffer.value,
                        presentationSeconds: presentationSeconds
                    )
                }
            }
        }
    }

    private struct UncheckedSendablePixelBuffer: @unchecked Sendable {
        let value: CVPixelBuffer
        init(_ value: CVPixelBuffer) { self.value = value }
    }

    private var hasLoggedFirstCinemaWindowSyncedFrame = false

    private func presentStereoCompositorFrame(
        pixelBuffer: CVPixelBuffer,
        presentationSeconds: TimeInterval
    ) {
        let cinemaActive = isCinemaWindowStereoActive
        let immersiveActive = ImmersiveStereoSession.isUserRequestedActive
        guard cinemaActive || immersiveActive else {
            return
        }
        let frameBuffer = VideoPixelBufferNV12Normalization.nv12VideoRangeCopyIfNeeded(from: pixelBuffer)
            ?? pixelBuffer
        syncImmersivePlaybackTimelineIfPossible()
        // Depth on the shared ring is for full immersive only (FFmpeg DA3 PTS ≠ window video PTS).
        let hasDepth = immersiveActive && metrics.processedFrameCount > 0
        let configuration = immersiveStereoRenderConfiguration(hasDepthMap: hasDepth)
        guard ImmersiveVideoFeed.shared.append(
            pixelBuffer: frameBuffer,
            mediaTime: presentationSeconds,
            configuration: configuration
        ) else {
            return
        }
        let isFirstFeedFrame = cinemaActive
            ? !hasLoggedFirstCinemaWindowSyncedFrame
            : !hasLoggedFirstImmersiveAudioSyncedFrame
        if isFirstFeedFrame {
            ImmersiveVideoFeed.shared.reanchorPlaybackClock(toVideoPTS: presentationSeconds)
            if let layer = installedLayer ?? coordinator.playerLayer {
                ImmersiveStereoPlaybackTimeline.update(
                    playbackSeconds: presentationSeconds,
                    isPlaying: layer.player.isPlaying,
                    playbackRate: layer.player.playbackRate
                )
                options.immersiveAudioPlaybackSeconds = presentationSeconds
                layer.options.immersiveAudioPlaybackSeconds = presentationSeconds
            }
        }
        if cinemaActive, !hasLoggedFirstCinemaWindowSyncedFrame {
            hasLoggedFirstCinemaWindowSyncedFrame = true
            let playback = ImmersiveStereoPlaybackTimeline.nowMediaSeconds()
            let offset = presentationSeconds - playback
            let ringCount = ImmersiveVideoFeed.shared.diagnosticsSnapshot().ringCount
            let message =
                "First cinema window-synced frame pts \(String(format: "%.3f", presentationSeconds)) "
                + "audio \(String(format: "%.3f", playback)) offset \(String(format: "%+.3f", offset))s "
                + "ring=\(ringCount)"
            Depth3DDebug.log(message, phase: "immersive-compositor")
            KSLog("[video] \(message)")
            return
        }
        guard immersiveActive, !hasLoggedFirstImmersiveAudioSyncedFrame else {
            return
        }
        hasLoggedFirstImmersiveAudioSyncedFrame = true
        let playback = ImmersiveStereoPlaybackTimeline.nowMediaSeconds()
        let offset = presentationSeconds - playback
        let ringCount = ImmersiveVideoFeed.shared.diagnosticsSnapshot().ringCount
        let message =
            "First immersive audio-synced frame pts \(String(format: "%.3f", presentationSeconds)) "
            + "audio \(String(format: "%.3f", playback)) offset \(String(format: "%+.3f", offset))s "
            + "ring=\(ringCount)"
        Depth3DDebug.log(message, phase: "immersive-compositor")
        KSLog("[video] \(message)")
    }
    #endif

    private func handleDebugEvent(_ event: Depth3DDebugEvent) {
        debugPhaseMessage = event.phase
        if event.level != .info || Depth3DDebug.isVerbose {
            lastDebugMessage = event.summary
        }
    }

    public func handlePlaybackTimeUpdate(layer: KSPlayerLayer, currentTime: TimeInterval) {
        guard shouldDeliverVideoFramesForImmersive || isImmersiveStereoPresented || isOpeningImmersiveStereo else {
            return
        }
        syncImmersivePlaybackTimeline(from: layer, playbackSeconds: currentTime)
    }

    public func handleStateChanged(layer: KSPlayerLayer, state: KSPlayerState) {
        playerState = state
        refreshSourceVideoFrameRate(from: layer)
        syncImmersiveSourceDisplayAspect(from: layer)
        syncImmersivePlaybackTimeline(from: layer)
        lastPlayerStateMessage = state.description
        if state == .playedToTheEnd {
            lastActionMessage = "Playback finished - press Play to restart"
        }
        if state == .readyToPlay || state == .bufferFinished {
            prewireVideoOutputIfNeeded(on: layer)
            if isDepthWarmupActive {
                startDepthWarmupPlaybackIfNeeded(on: layer)
            }
        }
        if shouldUseVideoOutput {
            let needsForceReconfigure = !isVideoOutputConfigured || state == .readyToPlay
            configureVideoOutput(on: layer, isEnabled: true, force: needsForceReconfigure)
        }
    }

    public func handleFinish(error: Error?) {
        if let error {
            lastErrorMessage = error.localizedDescription
            lastActionMessage = "Playback failed"
        } else {
            lastActionMessage = "Playback finished - press Play to restart"
        }
    }

    public func play() {
        guard let layer = installedLayer ?? coordinator.playerLayer else {
            lastErrorMessage = "Player is not ready yet. Wait for the video surface to load, then try Play again."
            lastActionMessage = "Play tapped before player ready"
            return
        }
        lastErrorMessage = nil
        if isDepthWarmupActive, depthWarmupStartedAt == nil {
            lastActionMessage = "Play queued — finishing depth warmup first"
            startDepthWarmupPlaybackIfNeeded(on: layer)
            return
        }
        lastActionMessage = playerState == .playedToTheEnd ? "Restarting from beginning" : "Play tapped"
        layer.play()
    }

    public func restart() {
        guard let layer = installedLayer ?? coordinator.playerLayer else {
            lastErrorMessage = "Player is not ready yet. Wait for the video surface to load, then try Restart again."
            lastActionMessage = "Restart tapped before player ready"
            return
        }
        lastErrorMessage = nil
        lastActionMessage = "Restarting from beginning"
        pipeline.reset()
        renderer.reset()
        layer.seek(time: 0, autoPlay: true) { [weak self] finished in
            Task { @MainActor in
                self?.lastActionMessage = finished ? "Restarted from beginning" : "Restart queued"
            }
        }
    }

    public func pause() {
        guard let layer = installedLayer ?? coordinator.playerLayer else {
            lastActionMessage = "Pause tapped before player ready"
            return
        }
        lastActionMessage = "Pause tapped"
        layer.pause()
    }

    public func setLoopEnabled(_ isEnabled: Bool) {
        guard isLoopEnabled != isEnabled else {
            return
        }
        isLoopEnabled = isEnabled
        options.isLoopPlay = isEnabled
        lastActionMessage = isEnabled ? "Loop enabled" : "Loop disabled"
        if isEnabled, playerState == .playedToTheEnd {
            restart()
        }
    }

    public func handleEnable3DPreviewButtonTap() {
        set2DTo3DEnabled(true, source: "Enable 3D Preview button")
    }

    public func set2DTo3DEnabled(_ isEnabled: Bool) {
        set2DTo3DEnabled(isEnabled, source: "2D-to-3D toggle")
    }

    public func set2DTo3DEnabled(_ isEnabled: Bool, source: String) {
        recordControlEvent("\(source) requested \(isEnabled ? "on" : "off")")
        if isEnabled {
            enable3DPreview(source: source)
        } else {
            disable3DPreview(source: source)
        }
    }

    public func enable3DPreview() {
        enable3DPreview(source: "Enable 3D Preview button")
    }

    /// Runs DA3 prepare + renderer switch when options start with 2D-to-3D enabled (demo default).
    public func ensure3DPreviewActivatedOnLaunch() {
        guard options.video2DTo3DMode.isEnabled else {
            return
        }
        guard !hasFinished3DPreviewActivation, !didAttemptLaunch3DPreviewActivation else {
            return
        }
        didAttemptLaunch3DPreviewActivation = true
        enable3DPreview(source: "app launch")
    }

    private func enable3DPreview(source: String) {
        guard !isApplying2DTo3DMode else {
            lastActionMessage = "3D preview is preparing"
            Depth3DDebug.log("3D request ignored from \(source): already applying", phase: "control-event")
            return
        }
        if hasFinished3DPreviewActivation {
            if is2DTo3DEnabled {
                lastActionMessage = "3D preview already enabled"
            } else if let lastErrorMessage {
                lastActionMessage = "3D preview unavailable: \(lastErrorMessage)"
            } else {
                lastActionMessage = "3D preview already enabled"
            }
            Depth3DDebug.log("3D request ignored from \(source): finished=\(hasFinished3DPreviewActivation) active=\(is2DTo3DEnabled)", phase: "control-event")
            return
        }
        if presentationMode == .native2D {
            setPresentationMode(.windowSelectedEye)
        }
        begin3DPreviewActivation(source: source)
    }

    private func begin3DPreviewActivation(source: String) {
        activationTask?.cancel()
        activationSequence += 1
        let sequence = activationSequence
        let pipeline = pipeline

        is3DPreviewRequested = true
        isApplying2DTo3DMode = true
        lastErrorMessage = nil
        lastActionMessage = "3D preview requested"
        activationStatusMessage = "3D requested from \(source)"
        previewModeDescription = presentationModeDescription(isActive: false)
        Depth3DDebug.log("3D request start from \(source): sequence=\(sequence) requested=\(is3DPreviewRequested) active=\(is2DTo3DEnabled)", phase: "control-event")

        if Self.isLikelyAdaptiveStream(url) {
            activationStatusMessage = "Switching HLS to 3D decode path"
            previewModeDescription = "HLS requested; preparing KSME/Metal frame output"
            Depth3DDebug.log("HLS 3D request will attempt KSME/Metal frame output for \(url.absoluteString)", phase: "activation-hls")
        }

        lastActionMessage = "Preparing Depth Anything V3 pipeline"
        activationStatusMessage = "DA3 preparing"
        previewModeDescription = "Preparing \(presentationMode.displayName)"

        activationTask = Task(priority: .userInitiated) { [weak self, pipeline] in
            do {
                let diagnostics = try await pipeline.prepareForActivation()
                try Task.checkCancellation()
                await MainActor.run {
                    self?.complete3DPreviewActivation(
                        sequence: sequence,
                        diagnostics: diagnostics
                    )
                }
            } catch is CancellationError {
                await MainActor.run {
                    self?.finishCancelledActivation(sequence: sequence)
                }
            } catch {
                await MainActor.run {
                    self?.fail3DPreviewActivation(sequence: sequence, error: error)
                }
            }
        }
    }

    private func complete3DPreviewActivation(
        sequence: Int,
        diagnostics: DA3ModelDiagnostics
    ) {
        guard activationSequence == sequence else {
            return
        }
        activationStatusMessage = "Model ready: \(diagnostics.inputSizeSummary)"
        activationStatusMessage = Self.isLikelyAdaptiveStream(url) ? "Switching HLS to 3D decode path" : "Switching renderer"
        let skipPlayerReload = presentationMode == .immersiveStereo
            || isImmersiveStereoPresented
            || isOpeningImmersiveStereo
            || shouldSkipPlayerReloadForActiveKSMEPlayback()
        applyPrepared3DPreviewMode(skipPlayerReload: skipPlayerReload)
        hasFinished3DPreviewActivation = true
        isApplying2DTo3DMode = false
        activationTask = nil
        startFirstFrameTimeout(sequence: sequence)
        lastActionMessage = skipPlayerReload ? "3D active (immersive, no player reload)" : "3D preview active"
        Depth3DDebug.log("3D request completed: sequence=\(sequence) requested=\(is3DPreviewRequested) active=\(is2DTo3DEnabled) skipReload=\(skipPlayerReload)", phase: "control-event")
    }

    private func finishCancelledActivation(sequence: Int) {
        guard activationSequence == sequence else {
            return
        }
        isApplying2DTo3DMode = false
        activationTask = nil
        activationStatusMessage = is3DPreviewRequested ? "Active" : "Cancelled"
        Depth3DDebug.log("3D request cancelled: sequence=\(sequence) requested=\(is3DPreviewRequested) active=\(is2DTo3DEnabled)", phase: "control-event")
    }

    private func fail3DPreviewActivation(sequence: Int, error: Error) {
        guard activationSequence == sequence else {
            return
        }
        fallBackToNative2D(sequence: sequence, message: error.localizedDescription)
    }

    private func fallBackToNative2D(sequence: Int, message: String) {
        activationSequence = max(activationSequence, sequence)
        isApplying2DTo3DMode = false
        activationTask = nil
        firstFrameWaitTask?.cancel()
        firstFrameWaitTask = nil
        is3DPreviewRequested = true
        is2DTo3DEnabled = false
        previewModeDescription = "3D request held after fallback"
        activationStatusMessage = "Failed/fallback: \(message)"
        lastErrorMessage = message
        lastActionMessage = "3D preview fallback to normal 2D"
        Depth3DDebug.warn("3D preview activation rolled back but request remains visible: \(message)", phase: "activation-fallback")
        applyRenderModeOptions()
        pipeline.reset()
        pipeline.setEnabled(false)
        renderer.reset()
        if let layer = installedLayer ?? coordinator.playerLayer {
            configureVideoOutput(on: layer, isEnabled: false, force: true)
        }
    }

    /// Avoid `stop` + `replace(url:)` when FFmpeg/VT is already decoding the same stream (prevents seek-to-zero and VT storms).
    private func shouldSkipPlayerReloadForActiveKSMEPlayback() -> Bool {
        guard let layer = installedLayer ?? coordinator.playerLayer,
              layer.player is KSMEPlayer
        else {
            return false
        }
        return layer.player.loadState == .playable
    }

    private func applyPrepared3DPreviewMode(skipPlayerReload: Bool = false) {
        isApplying2DTo3DMode = true
        defer {
            isApplying2DTo3DMode = false
        }
        lastErrorMessage = nil
        lastActionMessage = playerState == .playedToTheEnd ? "2D-to-3D enabled - restarting" : "2D-to-3D enabled"
        is3DPreviewRequested = true
        is2DTo3DEnabled = true
        depthStrengthRampStartedAt = Date().timeIntervalSinceReferenceDate
        hasReceivedFirst3DFrame = false
        pipeline.setInferenceFPSCap(nil)
        applyDepthProcessingOptimizations()
        options.video2DTo3DMode = .depthMapPreferred
        options.videoAdaptable = false
        options.requiresDecodedVideoFrameOutput = true
        applyStableHLSDecodePolicyIfNeeded()
        outputLayout = Self.outputLayout(for: presentationMode)
        options.video2DTo3DOutputLayout = outputLayout
        applyRenderModeOptions()
        if !skipPlayerReload {
            pipeline.reset()
            renderer.reset()
        }
        pipeline.setEnabled(shouldCaptureDepthFrames || shouldDeliverVideoFramesForImmersive)
        if skipPlayerReload {
            pipeline.syncRenderConfigurationSnapshot()
        } else {
            mark3DPreviewPresentationActive(frameMessage: "renderer switched; waiting for decoded frames")
            beginDepthWarmup()
            reloadPlayerPreservingPlaybackTime(restartIfEnded: true)
        }
        if let layer = installedLayer ?? coordinator.playerLayer {
            configureVideoOutput(on: layer, isEnabled: shouldUseVideoOutput, force: true)
            if skipPlayerReload, !layer.player.isPlaying {
                layer.play()
            }
        }
    }

    private func disable3DPreview(source: String = "Disable 3D") {
        guard is3DPreviewRequested || is2DTo3DEnabled || isApplying2DTo3DMode else {
            lastActionMessage = "3D preview already disabled"
            Depth3DDebug.log("3D disable ignored from \(source): already off", phase: "control-event")
            return
        }
        Depth3DDebug.log("3D disable from \(source): requested=\(is3DPreviewRequested) active=\(is2DTo3DEnabled) applying=\(isApplying2DTo3DMode)", phase: "control-event")
        Task {
            await dismissImmersiveStereoIfNeeded()
        }
        activationTask?.cancel()
        activationTask = nil
        firstFrameWaitTask?.cancel()
        firstFrameWaitTask = nil
        activationSequence += 1
        isApplying2DTo3DMode = true
        defer {
            isApplying2DTo3DMode = false
        }
        lastErrorMessage = nil
        lastActionMessage = "2D-to-3D disabled"
        activationStatusMessage = "Disabled"
        previewModeDescription = "Native 2D playback"
        presentationMode = .native2D
        presentationModeStatus = "Native 2D playback"
        is3DPreviewRequested = false
        is2DTo3DEnabled = false
        hasFinished3DPreviewActivation = false
        depthStrengthRampStartedAt = nil
        hasReceivedFirst3DFrame = false
        cancelDepthWarmup(restoreMute: true)
        pipeline.setInferenceFPSCap(nil)
        options.video2DTo3DMode = .disabled
        options.videoAdaptable = true
        applyRenderModeOptions()
        pipeline.reset()
        pipeline.setEnabled(false)
        renderer.reset()
        if let layer = installedLayer ?? coordinator.playerLayer {
            configureVideoOutput(on: layer, isEnabled: false, force: true)
        }
        reloadPlayerPreservingPlaybackTime(restartIfEnded: true)
    }

    private func beginDepthWarmup() {
        isDepthWarmupActive = true
        depthWarmupStartedAt = nil
        depthWarmupStatusMessage = "Depth warmup pending (buffering)"
        lastActionMessage = "Depth warmup will run before visible playback"
        refreshDepthWarmupPresentation()
        Depth3DDebug.log("Depth warmup armed", phase: "depth-warmup")
    }

    private func cancelDepthWarmup(restoreMute: Bool) {
        isDepthWarmupActive = false
        depthWarmupStartedAt = nil
        depthWarmupStatusMessage = ""
        refreshDepthWarmupPresentation()
        guard restoreMute, let layer = installedLayer ?? coordinator.playerLayer else {
            return
        }
        layer.player.isMuted = wasMutedBeforeDepthWarmup
    }

    private func startDepthWarmupPlaybackIfNeeded(on layer: KSPlayerLayer) {
        guard isDepthWarmupActive, depthWarmupStartedAt == nil else {
            return
        }
        guard shouldCaptureDepthFrames || shouldDeliverVideoFramesForImmersive else {
            return
        }
        wasMutedBeforeDepthWarmup = layer.player.isMuted
        layer.player.isMuted = true
        depthWarmupStartedAt = Date()
        depthWarmupStatusMessage = "Depth warmup running (muted)"
        lastActionMessage = "Muted depth warmup — video starts after depth is ready"
        Depth3DDebug.log("Depth warmup playback started (muted)", phase: "depth-warmup")
        if !layer.player.isPlaying {
            layer.play()
        }
    }

    private func updateDepthWarmupProgress(with metrics: DA3PipelineMetrics) {
        guard isDepthWarmupActive, let startedAt = depthWarmupStartedAt else {
            return
        }
        let elapsed = Date().timeIntervalSince(startedAt)
        let frames = metrics.processedFrameCount
        depthWarmupStatusMessage = String(
            format: "Depth warmup %.1fs / %.0fs, %d depth frame(s)",
            elapsed,
            DA3VisionProDemoTuning.depthWarmupMinimumSecondsEffective,
            frames
        )
        let minimumSeconds = DA3VisionProDemoTuning.depthWarmupMinimumSecondsEffective
        let minimumFrames = DA3VisionProDemoTuning.depthWarmupMinimumDepthFrames
        let maximumSeconds = DA3VisionProDemoTuning.depthWarmupMaximumSeconds
        let depthReady = frames >= minimumFrames && elapsed >= minimumSeconds
        let timedOut = elapsed >= maximumSeconds && frames >= 1
        if depthReady || timedOut {
            completeDepthWarmup(reason: depthReady ? "depth ready" : "warmup timeout")
        }
    }

    private func refreshDepthWarmupPresentation() {
        // Keep feeding decoded frames into the immersive ring during depth warmup so the
        // compositor can present video immediately (depth may lag; stale-depth path handles that).
        pipeline.setSuppressImmersiveVideoUpdates(false)
    }

    private func completeDepthWarmup(reason: String) {
        guard isDepthWarmupActive else {
            return
        }
        isDepthWarmupActive = false
        depthWarmupStartedAt = nil
        depthWarmupStatusMessage = "Depth warmup complete"
        refreshDepthWarmupPresentation()
        guard let layer = installedLayer ?? coordinator.playerLayer else {
            return
        }
        layer.player.isMuted = wasMutedBeforeDepthWarmup
        let startTime = DA3DemoEnvironment.startSecondsOverride
        let currentTime = layer.player.currentPlaybackTime
        let shouldSeekToStart = startTime.map { abs(currentTime - $0) > 0.35 } == true
        lastActionMessage = shouldSeekToStart
            ? "Depth warmup finished (\(reason)) — playing from \(String(format: "%.1f", startTime ?? 0))s"
            : "Depth warmup finished (\(reason)) — playing"
        Depth3DDebug.log(
            "Depth warmup complete: \(reason), seek=\(shouldSeekToStart) target=\(startTime.map { String(format: "%.3f", $0) } ?? "none") current=\(currentTime)s",
            phase: "depth-warmup"
        )
        if shouldSeekToStart, let startTime {
            layer.seek(time: startTime, autoPlay: true) { [weak self] finished in
                guard let self else {
                    return
                }
                if finished {
                    self.lastActionMessage = "Playback started after depth warmup"
                } else {
                    self.lastActionMessage = "Depth warmup done — seek failed, tap Play"
                }
            }
        } else {
            layer.play()
        }
    }

    private func recordControlEvent(_ message: String) {
        controlEventCount += 1
        controlEventMessage = "#\(controlEventCount) \(message)"
        lastActionMessage = controlEventMessage
        Depth3DDebug.log("\(controlEventMessage): requested=\(is3DPreviewRequested) active=\(is2DTo3DEnabled) applying=\(isApplying2DTo3DMode)", phase: "control-event")
    }

    private func startFirstFrameTimeout(sequence: Int) {
        firstFrameWaitTask?.cancel()
        activationStatusMessage = "waiting for first frame"
        previewModeDescription = Self.isLikelyAdaptiveStream(url)
            ? "HLS is on the 3D decode path; waiting for KSVideoFrameOutput"
            : "3D renderer switched; waiting for KSVideoFrameOutput"
        let timeout = firstFrameTimeout
        firstFrameWaitTask = Task { [weak self] in
            try? await Task.sleep(nanoseconds: UInt64(timeout * 1_000_000_000))
            await MainActor.run {
                self?.handleFirstFrameTimeout(sequence: sequence, timeout: timeout)
            }
        }
        Depth3DDebug.log("Started first-frame watchdog sequence=\(sequence) timeout=\(timeout)s", phase: "activation-first-frame")
    }

    private func handleFirst3DFrame(mediaTime: TimeInterval?) {
        guard is3DPreviewRequested, is2DTo3DEnabled else {
            return
        }
        guard !hasReceivedFirst3DFrame else {
            return
        }
        hasReceivedFirst3DFrame = true
        firstFrameWaitTask?.cancel()
        firstFrameWaitTask = nil
        lastErrorMessage = nil
        let frameMessage = mediaTime.map { String(format: "decoded frame callback active at %.2fs", $0) } ?? "decoded frame callback active"
        mark3DPreviewPresentationActive(frameMessage: frameMessage)
        lastActionMessage = "3D preview receiving decoded frames"
        Depth3DDebug.log("First decoded frame received for 3D preview mediaTime=\(mediaTime.map { String(format: "%.3f", $0) } ?? "unknown")", phase: "activation-first-frame")
    }

    private func mark3DPreviewPresentationActive(frameMessage: String) {
        activationStatusMessage = "3D enabled"
        presentationModeStatus = presentationModeDescription(isActive: true)
        previewModeDescription = "\(presentationModeStatus); \(frameMessage)"
    }

    private func handleFirstFrameTimeout(sequence: Int, timeout: TimeInterval) {
        guard activationSequence == sequence,
              is3DPreviewRequested,
              is2DTo3DEnabled,
              firstFrameWaitTask != nil,
              !isImmersiveStereoPresented,
              !isOpeningImmersiveStereo,
              !hasLoggedFirstImmersiveAudioSyncedFrame,
              !ImmersiveVideoFeed.shared.hasFrames
        else {
            return
        }
        firstFrameWaitTask = nil
        let reason = "No decoded frame callback arrived from KSVideoFrameOutput within \(Int(timeout)) seconds after switching to the 3D decode path."
        lastErrorMessage = reason
        activationStatusMessage = "Failed: no frame callback"
        previewModeDescription = "3D request is still on; try Play/Restart, or test an MP4/local file if this HLS stream cannot decode through KSME."
        lastActionMessage = "3D preview waiting failed"
        Depth3DDebug.warn("\(reason) url=\(url.absoluteString) requested=\(is3DPreviewRequested) active=\(is2DTo3DEnabled)", phase: "activation-first-frame")
    }

    public func setDepthStrength(_ value: Float) {
        let validatedValue = DA3VisionProDemoTuning.validatedDepthStrength(value)
        guard depthStrength != validatedValue else {
            return
        }
        depthStrength = validatedValue
        options.video2DTo3DDepthStrength = validatedValue
        lastActionMessage = String(format: "Depth strength %.2f", Double(validatedValue))
    }

    public func setDepthDistance(_ value: Float) {
        let validatedValue = DA3VisionProDemoTuning.validatedDepthDistance(value)
        guard depthDistance != validatedValue else {
            return
        }
        depthDistance = validatedValue
        options.video2DTo3DDepthDistance = validatedValue
        lastActionMessage = String(format: "Depth distance %.2f", Double(validatedValue))
    }

    public func setImmersiveScreenDistanceMeters(_ value: Float) {
        let validatedValue = ImmersiveScreenPlacement.validatedDistance(value)
        guard immersiveScreenDistanceMeters != validatedValue else {
            return
        }
        immersiveScreenDistanceMeters = validatedValue
        ImmersiveScreenPlacement.setDistanceMeters(validatedValue)
        lastActionMessage = String(format: "Screen distance %.1f m", Double(validatedValue))
    }

    public func setImmersiveScreenOffsetRightMeters(_ value: Float) {
        let validatedValue = ImmersiveScreenPlacement.validatedHorizontalOffset(value)
        guard immersiveScreenOffsetRightMeters != validatedValue else {
            return
        }
        immersiveScreenOffsetRightMeters = validatedValue
        ImmersiveScreenPlacement.setOffsetRightMeters(validatedValue)
        lastActionMessage = String(format: "Screen left/right %.2f m", Double(validatedValue))
    }

    public func setImmersiveScreenOffsetUpMeters(_ value: Float) {
        let validatedValue = ImmersiveScreenPlacement.validatedVerticalOffset(value)
        guard immersiveScreenOffsetUpMeters != validatedValue else {
            return
        }
        immersiveScreenOffsetUpMeters = validatedValue
        ImmersiveScreenPlacement.setOffsetUpMeters(validatedValue)
        lastActionMessage = String(format: "Screen up/down %.2f m", Double(validatedValue))
    }

    public func resetImmersiveScreenPlacement() {
        ImmersiveScreenPlacement.resetPlacement()
        immersiveScreenDistanceMeters = ImmersiveScreenPlacement.currentDistanceMeters
        immersiveScreenOffsetRightMeters = ImmersiveScreenPlacement.currentOffsetRightMeters
        immersiveScreenOffsetUpMeters = ImmersiveScreenPlacement.currentOffsetUpMeters
        lastActionMessage = "Screen position reset"
    }

    public func syncImmersiveScreenPlacementFromStore() {
        immersiveScreenDistanceMeters = ImmersiveScreenPlacement.currentDistanceMeters
        immersiveScreenOffsetRightMeters = ImmersiveScreenPlacement.currentOffsetRightMeters
        immersiveScreenOffsetUpMeters = ImmersiveScreenPlacement.currentOffsetUpMeters
    }

    public func setCinemaWindowStereoActive(_ active: Bool) {
        guard isCinemaWindowStereoActive != active else {
            return
        }
        isCinemaWindowStereoActive = active
        #if os(visionOS)
        options.deliverDecodedVideoFrameToStereoCompositorWhileWindowVisible = active
        pipeline.setCompositorVideoFromWindowPresentation(active)
        if active {
            ImmersiveVideoFeed.shared.setAcceptingFrames(true)
            ImmersiveVideoFeed.shared.clear()
            hasLoggedFirstCinemaWindowSyncedFrame = false
        } else if !shouldDeliverVideoFramesForImmersive {
            ImmersiveVideoFeed.shared.setAcceptingFrames(false)
            ImmersiveVideoFeed.shared.clear()
            hasLoggedFirstCinemaWindowSyncedFrame = false
        }
        pipeline.setDeliversImmersiveVideo(shouldDeliverVideoFramesForImmersive)
        pipeline.setEnabled(shouldCaptureDepthFrames || shouldDeliverVideoFramesForImmersive)
        if let layer = installedLayer ?? coordinator.playerLayer {
            applyImmersivePresentationFlags(on: layer)
            if let mePlayer = layer.player as? KSMEPlayer {
                mePlayer.synchronizePresentationOptions(from: layer.options)
                if active {
                    mePlayer.drainWindowVideoPresentationQueue(maxFrames: 8)
                }
            }
            configureVideoOutput(on: layer, isEnabled: shouldUseVideoOutput, force: true)
            startImmersivePresentationDrain(on: layer)
        }
        #endif
        lastActionMessage = active ? "Cinema window stereo active" : "Cinema window stereo inactive"
    }

    public func setDepthCurvature(_ value: Float) {
        let validatedValue = DA3VisionProDemoTuning.validatedDepthCurvature(value)
        guard depthCurvature != validatedValue else {
            return
        }
        depthCurvature = validatedValue
        options.video2DTo3DDepthCurvature = validatedValue
        lastActionMessage = String(format: "Depth curvature %.2f", Double(validatedValue))
    }

    public func setOutputLayout(_ layout: Video2DTo3DOutputLayout) {
        guard outputLayout != layout else {
            return
        }
        outputLayout = layout
        options.video2DTo3DOutputLayout = layout
        presentationMode = Self.presentationMode(for: layout)
        presentationModeStatus = presentationModeDescription(isActive: is2DTo3DEnabled)
        applyRenderModeOptions()
        lastActionMessage = "Output layout \(Self.outputLayoutName(layout))"
    }

    public func bindImmersiveStereo(
        open: @escaping () async -> Bool,
        dismiss: @escaping () async -> Void
    ) {
        openImmersiveStereo = open
        dismissImmersiveStereo = dismiss
    }

    public func clearImmersiveOpenFailureReason() {
        immersiveOpenFailureReason = nil
    }

    public func setImmersiveOpenFailureReason(_ reason: String) {
        immersiveOpenFailureReason = reason
    }

    /// Ensures a cold launch or window restore does not keep immersive UI state from a prior session.
    public func resetImmersivePresentationForWindowLaunch() {
        guard isImmersiveStereoPresented || isOpeningImmersiveStereo || presentationMode == .immersiveStereo else {
            return
        }
        isOpeningImmersiveStereo = false
        isImmersiveStereoPresented = false
        ImmersiveStereoSession.setUserRequestedActive(false)
        immersiveOpenFailureReason = nil
        if presentationMode == .immersiveStereo {
            presentationMode = is2DTo3DEnabled ? .windowSelectedEye : .native2D
            presentationModeStatus = presentationModeDescription(isActive: is2DTo3DEnabled)
            previewModeDescription = presentationModeStatus
        }
        ImmersiveStereoFrameStore.shared.reset()
        #if os(visionOS)
        ImmersiveVideoFeed.shared.setAcceptingFrames(false)
        ImmersiveVideoFeed.shared.clear()
        hasLoggedFirstImmersiveAudioSyncedFrame = false
        pipeline.setSuppressImmersiveVideoUpdates(false)
        #endif
        options.relaxVideoClockSyncWhileImmersiveCompositorActive = false
        options.suppressWindowVideoPresentationWhileImmersiveCompositorActive = false
        applyRenderModeOptions()
    }

    public func setPresentationMode(_ mode: DA3PresentationMode) {
        if mode == .immersiveStereo {
            if isImmersiveStereoPresented || isOpeningImmersiveStereo {
                lastActionMessage = "Immersive stereo already active"
                return
            }
            Task {
                await activateImmersiveStereoPresentation()
            }
            return
        }
        guard presentationMode != mode else {
            lastActionMessage = "\(mode.displayName) presentation already selected"
            return
        }
        if presentationMode == .immersiveStereo {
            Task {
                await dismissImmersiveStereoIfNeeded()
            }
        }
        if mode == .native2D {
            disable3DPreview(source: "Presentation Mode 2D")
            return
        }
        lastErrorMessage = nil
        presentationMode = mode
        outputLayout = Self.outputLayout(for: mode)
        options.video2DTo3DOutputLayout = outputLayout
        presentationModeStatus = presentationModeDescription(isActive: is2DTo3DEnabled)
        previewModeDescription = presentationModeStatus
        lastActionMessage = "Presentation \(mode.displayName)"
        Depth3DDebug.log("Presentation mode \(mode.displayName), output \(Self.outputLayoutName(outputLayout))", phase: "presentation-mode")
    }

    private func activateImmersiveStereoPresentation() async {
        recordControlEvent("Immersive Stereo requested")
        guard !isOpeningImmersiveStereo, !isImmersiveStereoPresented else {
            lastActionMessage = "Immersive stereo already active"
            return
        }
        guard openImmersiveStereo != nil else {
            let message = "Immersive stereo host is not configured."
            lastErrorMessage = message
            lastActionMessage = "Immersive stereo unavailable"
            presentationModeStatus = message
            previewModeDescription = message
            Depth3DDebug.warn(message, phase: "presentation-mode")
            return
        }

        if !is2DTo3DEnabled, !isApplying2DTo3DMode {
            is3DPreviewRequested = true
            begin3DPreviewActivation(source: "Immersive Stereo (pre-open)")
            let ready = await waitFor3DPreviewReady()
            guard ready else {
                lastActionMessage = "Immersive stereo waiting for 3D — try again"
                presentationModeStatus = "3D pipeline not ready for immersive"
                previewModeDescription = presentationModeStatus
                return
            }
        } else if !is2DTo3DEnabled {
            is3DPreviewRequested = true
            let ready = await waitFor3DPreviewReady()
            guard ready else {
                lastActionMessage = "Immersive stereo waiting for 3D — try again"
                presentationModeStatus = "3D pipeline not ready for immersive"
                previewModeDescription = presentationModeStatus
                return
            }
        }

        isOpeningImmersiveStereo = true
        ImmersiveStereoSession.setUserRequestedActive(true)
        if !is2DTo3DEnabled {
            beginDepthWarmup()
        }
        applyStableHLSDecodePolicyIfNeeded(reloadPlayerWhenActive: false)
        defer {
            isOpeningImmersiveStereo = false
            applyRenderModeOptions()
        }

        if let layer = installedLayer ?? coordinator.playerLayer {
            syncImmersivePlaybackTimeline(from: layer)
            applyImmersivePresentationFlags(on: layer)
            ensureVideoOutputWired(on: layer)
            #if os(visionOS)
            ImmersiveVideoFeed.shared.pruneEntriesFarFromPlayback()
            ImmersiveVideoFeed.shared.setAcceptingFrames(true)
            #endif
        }

        lastErrorMessage = nil
        presentationMode = .immersiveStereo
        outputLayout = Self.outputLayout(for: .immersiveStereo)
        options.video2DTo3DOutputLayout = outputLayout
        applyRenderModeOptions()
        presentationModeStatus = "Opening immersive per-eye stereo"
        previewModeDescription = presentationModeStatus
        immersiveOpenFailureReason = nil
        refreshDepthWarmupPresentation()
        if let layer = installedLayer ?? coordinator.playerLayer {
            ensureVideoOutputWired(on: layer)
            startDepthWarmupPlaybackIfNeeded(on: layer)
        }
        await ImmersiveStereoARTrackingBridge.requestAuthorizationIfNeeded()
        guard ImmersiveStereoARTrackingBridge.isWorldTrackingSupported else {
            let message = "World tracking is unavailable on this device or simulator. Test Immersive Stereo on Apple Vision Pro hardware."
            lastErrorMessage = message
            lastActionMessage = "Immersive stereo unavailable here"
            presentationMode = is2DTo3DEnabled ? .windowSelectedEye : .native2D
            presentationModeStatus = message
            previewModeDescription = message
            options.relaxVideoClockSyncWhileImmersiveCompositorActive = false
            options.suppressWindowVideoPresentationWhileImmersiveCompositorActive = false
            pipeline.setDeliversImmersiveVideo(false)
            #if os(visionOS)
            ImmersiveVideoFeed.shared.setAcceptingFrames(false)
            #endif
            ImmersiveStereoSession.setUserRequestedActive(false)
            stopImmersivePresentationDrain()
            Depth3DDebug.warn(message, phase: "presentation-mode")
            return
        }

        guard let openImmersiveStereo else {
            options.relaxVideoClockSyncWhileImmersiveCompositorActive = false
            options.suppressWindowVideoPresentationWhileImmersiveCompositorActive = false
            pipeline.setDeliversImmersiveVideo(false)
            ImmersiveStereoSession.setUserRequestedActive(false)
            stopImmersivePresentationDrain()
            return
        }

        // WorldTrackingProvider stays `.paused` until ImmersiveSpace is open (device only).
        let opened = await openImmersiveStereo()
        guard opened else {
            let message = immersiveOpenFailureMessage
            lastErrorMessage = message
            lastActionMessage = "Immersive stereo open failed"
            presentationMode = is2DTo3DEnabled ? .windowSelectedEye : .native2D
            presentationModeStatus = message
            previewModeDescription = message
            ImmersiveStereoSession.setUserRequestedActive(false)
            options.relaxVideoClockSyncWhileImmersiveCompositorActive = false
            options.suppressWindowVideoPresentationWhileImmersiveCompositorActive = false
            pipeline.setDeliversImmersiveVideo(false)
            stopImmersivePresentationDrain()
            Depth3DDebug.warn(message, phase: "presentation-mode")
            return
        }

        isImmersiveStereoPresented = true
        ImmersiveStereoCompositorStatus.reset()

        let arReady = await ImmersiveStereoARTrackingBridge.ensureRunning(timeout: nil)
        if !arReady {
            let arMessage = ImmersiveStereoARTrackingBridge.sessionErrorMessage
                ?? "ARKit world tracking did not start in time"
            lastErrorMessage = arMessage
            lastActionMessage = "Immersive open — head tracking not ready"
            presentationModeStatus = "Immersive open — waiting for head tracking (\(arMessage))"
            previewModeDescription = presentationModeStatus
            Depth3DDebug.warn("Immersive open without running ARKit: \(arMessage)", phase: "presentation-mode")
        } else {
            lastErrorMessage = nil
            Depth3DDebug.log("ARKit world tracking running after immersive open", phase: "presentation-mode")
        }

        await alignDA3MetalDeviceWithCompositor()
        applyRenderModeOptions()
        applyDepthProcessingOptimizations()
        if let layer = installedLayer ?? coordinator.playerLayer {
            syncImmersivePlaybackTimeline(from: layer)
            applyImmersivePresentationFlags(on: layer)
            if !layer.player.isPlaying {
                layer.play()
            }
        }
        pipeline.setDeliversImmersiveVideo(true)
        startImmersiveTimelineSync()
        startImmersiveDiagnosticsPolling()
        _ = await waitForImmersiveRingBufferVideo(timeout: 8)
        refreshImmersivePlaybackDiagnostics()
        lastActionMessage = arReady ? "Immersive stereo active" : "Immersive stereo active (waiting for head tracking)"
        presentationModeStatus = immersiveCompositorStatusDescription()
        previewModeDescription = presentationModeStatus
        Depth3DDebug.log("Immersive stereo space opened", phase: "presentation-mode")

        if is2DTo3DEnabled {
            applyRenderModeOptions()
        }
    }

    private var immersiveOpenFailureMessage: String {
        if let immersiveOpenFailureReason {
            return immersiveOpenFailureReason
        }
        return "Unable to open immersive space. Dismiss any open immersive space and try again."
    }

    private func alignDA3MetalDeviceWithCompositor() async {
        let deadline = Date().addingTimeInterval(8)
        while Date() < deadline {
            if let compositorDevice = ImmersiveStereoMetalDeviceRegistry.compositorDevice {
                pipeline.reconfigureMetalDeviceIfNeeded(compositorDevice)
                Depth3DDebug.log("DA3 depth bridge aligned with compositor GPU", phase: "immersive-compositor")
                return
            }
            try? await Task.sleep(nanoseconds: 50_000_000)
        }
        lastErrorMessage = "Compositor GPU not ready; depth textures may be on the wrong Metal device."
        Depth3DDebug.warn(
            "Compositor Metal device not registered within 8s; DA3 may use a different GPU",
            phase: "immersive-compositor"
        )
    }

    private func dismissImmersiveStereoIfNeeded() async {
        guard isImmersiveStereoPresented || isOpeningImmersiveStereo else {
            return
        }
        ImmersiveStereoSession.setUserRequestedActive(false)
        options.relaxVideoClockSyncWhileImmersiveCompositorActive = false
        options.suppressWindowVideoPresentationWhileImmersiveCompositorActive = false
        await dismissImmersiveStereo?()
        isImmersiveStereoPresented = false
        isOpeningImmersiveStereo = false
        stopImmersivePresentationDrain()
        pipeline.setDeliversImmersiveVideo(false)
        #if os(visionOS)
        ImmersiveVideoFeed.shared.setAcceptingFrames(false)
        ImmersiveVideoFeed.shared.clear()
        #endif
        ImmersiveStereoARTrackingBridge.stopIfNeeded()
        ImmersiveStereoFrameStore.shared.reset()
        ImmersiveStereoMetalDeviceRegistry.clearCompositorDevice()
        ImmersiveStereoCompositorStatus.reset()
        if is2DTo3DEnabled {
            applyRenderModeOptions()
        }
        Depth3DDebug.log("Immersive stereo space dismissed", phase: "presentation-mode")
    }

    public func setDepthContrast(_ value: Float) {
        let validatedValue = DA3VisionProDemoTuning.validatedDepthContrast(value)
        guard depthContrast != validatedValue else {
            return
        }
        depthContrast = validatedValue
        pipeline.setDepthProcessing(depthContrast: validatedValue, isDepthInverted: isDepthInverted)
        lastActionMessage = String(format: "Depth contrast %.2f", Double(validatedValue))
    }

    public func setDepthInverted(_ inverted: Bool) {
        guard isDepthInverted != inverted else {
            return
        }
        isDepthInverted = inverted
        pipeline.setDepthProcessing(depthContrast: depthContrast, isDepthInverted: inverted)
        lastActionMessage = inverted ? "Depth inversion enabled" : "Depth inversion disabled"
    }

    public func setTemporalSmoothingFactor(_ value: Float) {
        let validatedValue = DA3VisionProDemoTuning.validatedTemporalSmoothingFactor(value)
        guard temporalSmoothingFactor != validatedValue else {
            return
        }
        temporalSmoothingFactor = validatedValue
        options.video2DTo3DDepthSmoothingFactor = validatedValue
        pipeline.setTemporalSmoothingFactor(validatedValue)
        lastActionMessage = String(format: "Metal depth smoothing %.2f", Double(validatedValue))
    }

    public func setPerformanceMode(_ mode: DA3VisionProPerformanceMode) {
        guard performanceMode != mode else {
            lastActionMessage = "\(mode.displayName) performance mode already active"
            return
        }
        performanceMode = mode
        setDepthStrength(mode.depthStrength)
        setDepthDistance(mode.depthDistance)
        setDepthCurvature(mode.depthCurvature)
        setDepthContrast(mode.depthContrast)
        setTemporalSmoothingFactor(mode.temporalSmoothingFactor)
        pipeline.setPerformanceMode(mode)
        applyDepthProcessingOptimizations()
        let schedulingNote = mode.usesContinuousDepthScheduling ? "continuous depth" : "capped depth"
        lastActionMessage = "\(mode.displayName): depth cap \(String(format: "%.0f", mode.maximumInferenceFPS)) fps, \(schedulingNote), smoothing \(String(format: "%.2f", Double(mode.temporalSmoothingFactor)))"
    }

    private func applyDepthProcessingOptimizations() {
        pipeline.setDepthProcessingOptimizations(
            skipsDepthMapBuild: debugRenderMode != .depthOnly,
            usesContinuousDepthScheduling: performanceMode.usesContinuousDepthScheduling
        )
    }

    public func setDebugRenderMode(_ mode: DA3DebugRenderMode) {
        guard debugRenderMode != mode else {
            lastActionMessage = "\(mode.displayName) debug mode already active"
            return
        }
        debugRenderMode = mode
        pipeline.setDebugRenderMode(mode)
        applyRenderModeOptions()
        pipeline.setEnabled(shouldCaptureDepthFrames)
        if let layer = installedLayer ?? coordinator.playerLayer {
            configureVideoOutput(on: layer, isEnabled: shouldUseVideoOutput, force: true)
        }
        renderer.reset()
        lastActionMessage = "\(mode.displayName) debug mode"
    }

    public func applyComfortPreviewPreset() {
        setPerformanceMode(.stability)
        setPresentationMode(.packedPreview)
        if !is2DTo3DEnabled {
            set2DTo3DEnabled(true, source: "Comfort SBS Preset")
        }
        lastActionMessage = is2DTo3DEnabled ? "Comfort packed preview preset applied" : "Comfort preset requested"
    }

    public func applyStrongPreviewPreset() {
        setPerformanceMode(.performance)
        setPresentationMode(.packedPreview)
        if !is2DTo3DEnabled {
            set2DTo3DEnabled(true, source: "Strong SBS Preset")
        }
        lastActionMessage = is2DTo3DEnabled ? "Strong packed preview preset applied" : "Strong preset requested"
    }

    public func stop() {
        activationTask?.cancel()
        activationTask = nil
        firstFrameWaitTask?.cancel()
        firstFrameWaitTask = nil
        stopImmersivePresentationDrain()
        installedLayer?.videoOutput = nil
        installedLayer = nil
        is3DPreviewRequested = false
        isApplying2DTo3DMode = false
        Task {
            await dismissImmersiveStereoIfNeeded()
        }
        options.video2DTo3DRenderMetricsHandler = nil
        pipeline.reset()
        renderer.reset()
        lastActionMessage = "Player stopped"
    }

    private var shouldCaptureDepthFrames: Bool {
        is2DTo3DEnabled && debugRenderMode.capturesDepth
    }

    private var isCinemaWindowStereoActive = false

    private var shouldDeliverVideoFramesForImmersive: Bool {
        isCinemaWindowStereoActive
            || (presentationMode == .immersiveStereo && (isImmersiveStereoPresented || isOpeningImmersiveStereo))
    }

    private var shouldUseVideoOutput: Bool {
        shouldCaptureDepthFrames || shouldDeliverVideoFramesForImmersive
    }

    /// Hide and stop presenting video in the flat window while immersive compositor is active.
    public var hidesWindowVideoSurface: Bool {
        presentationMode == .immersiveStereo && (isImmersiveStereoPresented || isOpeningImmersiveStereo)
    }

    private func immersiveStereoRenderConfiguration(hasDepthMap: Bool) -> Video2DTo3DRenderConfiguration {
        let usesDepth = hasDepthMap && debugRenderMode == .normal
        let rampedStrength: Float
        if let startedAt = depthStrengthRampStartedAt {
            let elapsed = Date().timeIntervalSinceReferenceDate - startedAt
            let rampSeconds = 0.75
            let t = max(0, min(1, elapsed / rampSeconds))
            rampedStrength = depthStrength * Float(t)
        } else {
            rampedStrength = depthStrength
        }
        return Video2DTo3DPolicy.renderConfiguration(
            mode: .depthMapPreferred,
            depthStrength: rampedStrength,
            depthDistance: depthDistance,
            depthCurvature: depthCurvature,
            depthSmoothingFactor: temporalSmoothingFactor,
            outputLayout: outputLayout,
            selectedEye: options.stereoscopicVideoEye,
            display: options.display,
            stereoscopicVideoLayout: options.stereoscopicVideoLayout,
            hasDepthMap: usesDepth
        )
    }

    private func applyRenderModeOptions() {
        let immersiveActive = isImmersiveStereoPresented || isOpeningImmersiveStereo
        let immersiveCompositorRequested = immersiveActive && ImmersiveStereoSession.isUserRequestedActive
        let immersivePresentation = presentationMode == .immersiveStereo || isOpeningImmersiveStereo
        if immersiveActive || isOpeningImmersiveStereo {
            // CineUltra-style: immersive color follows the audio clock via MetalPlayView drain, not decode live-edge.
            options.relaxVideoClockSyncWhileImmersiveCompositorActive = false
            options.suppressWindowVideoPresentationWhileImmersiveCompositorActive =
                immersivePresentation
                && (immersiveCompositorRequested || isOpeningImmersiveStereo || ImmersiveStereoSession.isUserRequestedActive)
        } else {
            options.relaxVideoClockSyncWhileImmersiveCompositorActive = false
            options.suppressWindowVideoPresentationWhileImmersiveCompositorActive = false
        }
        #if os(visionOS)
        pipeline.setSuppressImmersiveVideoUpdates(usesImmersiveAudioSyncedPresentation)
        if !usesImmersiveAudioSyncedPresentation {
            hasLoggedFirstImmersiveAudioSyncedFrame = false
        }
        #endif
        if let layer = installedLayer ?? coordinator.playerLayer {
            if shouldUseVideoOutput {
                options.requiresDecodedVideoFrameOutput = true
                layer.options.requiresDecodedVideoFrameOutput = true
            }
            // Push suppress + immersivePresentVideoFrame into layer/MetalPlayView (not only relax/suppress).
            applyImmersivePresentationFlags(on: layer)
            if let mePlayer = layer.player as? KSMEPlayer {
                mePlayer.synchronizePresentationOptions(from: layer.options)
            }
        }

        guard is2DTo3DEnabled else {
            options.video2DTo3DMode = .disabled
            options.videoDepthEstimationProvider = nil
            lastAppliedMode = Self.appliedModeName(isEnabled: false, layout: outputLayout)
            return
        }

        if immersiveActive {
            // DA3 still runs via KSVideoFrameOutput; immersive compositor renders stereo.
            // Keep the flat window on native 2D to avoid competing Metal pipelines.
            options.video2DTo3DMode = .disabled
            options.videoDepthEstimationProvider = debugRenderMode == .normal ? depthMapProvider : nil
            lastAppliedMode = "immersive-compositor/\(debugRenderMode.displayName)"
            pipeline.syncRenderConfigurationSnapshot()
            return
        }

        switch debugRenderMode {
        case .normal:
            options.video2DTo3DMode = .depthMapPreferred
            options.videoDepthEstimationProvider = depthMapProvider
        case .depthDisabled:
            options.video2DTo3DMode = .disabled
            options.videoDepthEstimationProvider = nil
        case .depthOnly:
            options.video2DTo3DMode = .disabled
            options.videoDepthEstimationProvider = nil
        case .stereoOnly:
            options.video2DTo3DMode = .pseudoStereo
            options.videoDepthEstimationProvider = nil
        }
        lastAppliedMode = "\(debugRenderMode.displayName)/\(Self.outputLayoutName(outputLayout))"
        applyDepthProcessingOptimizations()
    }

    private func refreshSourceVideoFrameRate(from layer: KSPlayerLayer) {
        let fps: Float
        if let mePlayer = layer.player as? KSMEPlayer {
            fps = mePlayer.sourceVideoFrameRate
        } else {
            fps = 0
        }
        pipeline.setSourceVideoFrameRate(fps > 0 ? Double(fps) : nil)
        applyInferenceEnvironmentOverridesIfNeeded()
        if fps > 0 {
            Depth3DDebug.log(
                "Source video frame rate \(String(format: "%.3f", fps)) fps (PTS timeline)",
                phase: "video-output",
                verboseOnly: true
            )
        }
    }

    private func applyInferenceEnvironmentOverridesIfNeeded() {
        if let fps = DA3DemoEnvironment.inferenceFPSOverride {
            pipeline.setInferenceFPSCap(fps)
        } else {
            pipeline.setInferenceFPSCap(nil)
        }
    }

    private func configureVideoOutput(on layer: KSPlayerLayer, isEnabled: Bool, force: Bool = false) {
        if isConfiguringVideoOutput {
            return
        }
        isConfiguringVideoOutput = true
        defer {
            isConfiguringVideoOutput = false
        }

        if installedLayer !== layer {
            installedLayer = layer
        }

        guard isEnabled else {
            isVideoOutputConfigured = false
            pipeline.setDeliversImmersiveVideo(false)
            #if os(visionOS)
            ImmersiveVideoFeed.shared.setAcceptingFrames(false)
            #endif
            layer.videoOutput = nil
            stopImmersivePresentationDrain()
            options.requiresDecodedVideoFrameOutput = false
            options.videoAdaptable = true
            layer.options.requiresDecodedVideoFrameOutput = false
            layer.options.videoAdaptable = true
            return
        }

        if !(layer.player is KSMEPlayer) {
            Depth3DDebug.warn(
                "KSVideoFrameOutput requires KSMEPlayer; current player is \(String(describing: type(of: layer.player))). Reloading decode path.",
                phase: "video-output"
            )
            reloadPlayerPreservingPlaybackTime(restartIfEnded: false)
            if let refreshedLayer = installedLayer ?? coordinator.playerLayer {
                ensureVideoOutputWired(on: refreshedLayer)
            }
            return
        }

        refreshDepthWarmupPresentation()
        ensureVideoOutputWired(on: layer)
        Depth3DDebug.log(
            "Video output active immersive=\(shouldDeliverVideoFramesForImmersive) capture=\(shouldCaptureDepthFrames)",
            phase: "video-output"
        )
    }

    private var shouldDrainWindowPresentationQueue: Bool {
        options.suppressWindowVideoPresentationWhileImmersiveCompositorActive && shouldUseVideoOutput
    }

    private func startImmersivePresentationDrain(on layer: KSPlayerLayer) {
        stopImmersivePresentationDrain()
        immersivePresentationDrainTask = Task { @MainActor [weak self, weak layer] in
            while !Task.isCancelled {
                guard let self, let layer else {
                    return
                }
                guard self.shouldDrainWindowPresentationQueue
                    || self.shouldDeliverVideoFramesForImmersive
                    || self.isImmersiveStereoPresented
                    || self.isOpeningImmersiveStereo
                else {
                    return
                }
                (layer.player as? KSMEPlayer)?.drainWindowVideoPresentationQueue(maxFrames: 4)
                try? await Task.sleep(nanoseconds: 16_666_667)
            }
        }
    }

    private func stopImmersivePresentationDrain() {
        immersivePresentationDrainTask?.cancel()
        immersivePresentationDrainTask = nil
        stopImmersiveTimelineSync()
        #if os(visionOS)
        stopImmersiveDiagnosticsPolling()
        #endif
    }

    private func startImmersiveTimelineSync() {
        stopImmersiveTimelineSync()
        immersiveTimelineSyncTask = Task { @MainActor [weak self] in
            while !Task.isCancelled {
                guard let self else {
                    return
                }
                guard self.isImmersiveStereoPresented || self.isOpeningImmersiveStereo else {
                    return
                }
                self.syncImmersivePlaybackTimelineIfPossible()
                try? await Task.sleep(nanoseconds: 16_666_667)
            }
        }
    }

    private func stopImmersiveTimelineSync() {
        immersiveTimelineSyncTask?.cancel()
        immersiveTimelineSyncTask = nil
    }

    #if os(visionOS)
    private func startImmersiveDiagnosticsPolling() {
        stopImmersiveDiagnosticsPolling()
        immersiveDiagnosticsTask = Task { @MainActor [weak self] in
            while !Task.isCancelled {
                guard let self else {
                    return
                }
                guard self.isImmersiveStereoPresented || self.isOpeningImmersiveStereo else {
                    return
                }
                self.refreshImmersivePlaybackDiagnostics()
                self.refreshImmersivePresentationStatusIfNeeded()
                try? await Task.sleep(nanoseconds: 1_000_000_000)
            }
        }
    }

    private func stopImmersiveDiagnosticsPolling() {
        immersiveDiagnosticsTask?.cancel()
        immersiveDiagnosticsTask = nil
        immersivePlaybackDiagnostics = .idle
    }

    private func refreshImmersivePlaybackDiagnostics() {
        let audioTime = options.immersiveAudioPlaybackSeconds
        let snapshot = ImmersivePlaybackDiagnosticsCollector.snapshot(
            audioTime: audioTime,
            playbackAnchor: ImmersiveVideoFeed.shared.playbackAnchorSnapshot,
            usesAudioSyncedPresentation: usesImmersiveAudioSyncedPresentation,
            suppressesDecodePathVideo: pipeline.isSuppressingImmersiveVideoUpdates,
            videoOutputCallbackReceived: pipeline.hasReceivedVideoOutputCallback
        )
        if immersivePlaybackDiagnostics != snapshot {
            immersivePlaybackDiagnostics = snapshot
        }
        let summary = snapshot.summaryLines.joined(separator: " | ")
        Depth3DDebug.log(summary, phase: "immersive-timing", verboseOnly: true)
    }
    #endif

    private func waitFor3DPreviewReady() async -> Bool {
        while !Task.isCancelled {
            if is2DTo3DEnabled,
               pipeline.hasReceivedVideoOutputCallback || hasReceivedFirst3DFrame || metrics.processedFrameCount > 0
            {
                return true
            }
            try? await Task.sleep(nanoseconds: 50_000_000)
        }
        return false
    }

    private func waitForImmersiveRingBufferVideo(timeout: TimeInterval) async -> Bool {
        let deadline = Date().addingTimeInterval(timeout)
        while Date() < deadline {
            if Task.isCancelled {
                return false
            }
            if let layer = installedLayer ?? coordinator.playerLayer {
                (layer.player as? KSMEPlayer)?.drainWindowVideoPresentationQueue(maxFrames: 4)
            }
            if ImmersiveVideoFeed.shared.hasFrames || hasLoggedFirstImmersiveAudioSyncedFrame {
                return true
            }
            try? await Task.sleep(nanoseconds: 33_000_000)
        }
        if !ImmersiveVideoFeed.shared.hasFrames, !hasLoggedFirstImmersiveAudioSyncedFrame {
            let callbackReceived = pipeline.hasReceivedVideoOutputCallback
            let message =
                "Immersive opened without ring-buffer video within \(Int(timeout))s "
                + "(videoOutputCallback=\(callbackReceived)) — check KSMEPlayer wiring and decode drain"
            Depth3DDebug.warn(message, phase: "immersive-compositor")
            KSLog("[video] \(message)")
        }
        return ImmersiveVideoFeed.shared.hasFrames
    }

    /// Wire `KSVideoFrameOutput` once per layer — re-assigning the handler flushes pending frames.
    private func ensureVideoOutputWired(on layer: KSPlayerLayer) {
        options.requiresDecodedVideoFrameOutput = true
        layer.options.requiresDecodedVideoFrameOutput = true
        layer.options.videoAdaptable = false
        options.videoAdaptable = false
        if Self.isLikelyAdaptiveStream(url) {
            layer.options.hardwareDecode = false
            layer.options.asynchronousDecompression = false
            options.hardwareDecode = false
            options.asynchronousDecompression = false
        }
        applyImmersivePresentationFlags(on: layer)
        let firstWire = layer.videoOutput == nil
        if firstWire {
            layer.videoOutputConfiguration = KSVideoFrameOutput.Configuration(
                maximumBufferedFrameCount: 4,
                dropPolicy: .keepLatest,
                callbackQualityOfService: .userInitiated,
                callbackQueueLabel: "DepthAnythingV3.videoOutput"
            )
            layer.videoOutput = videoFrameSink.receive
            refreshSourceVideoFrameRate(from: layer)
            pipeline.setEnabled(shouldUseVideoOutput)
            if let mePlayer = layer.player as? KSMEPlayer {
                mePlayer.synchronizePresentationOptions(from: layer.options)
            }
            isVideoOutputConfigured = true
            KSLog("[video] KSVideoFrameOutput wired (first time, stable handler)")
        } else if let mePlayer = layer.player as? KSMEPlayer {
            mePlayer.synchronizePresentationOptions(from: layer.options)
        }
        pipeline.setDeliversImmersiveVideo(shouldDeliverVideoFramesForImmersive)
        if shouldDrainWindowPresentationQueue || shouldDeliverVideoFramesForImmersive {
            startImmersivePresentationDrain(on: layer)
        }
    }

    private func applyImmersivePresentationFlags(on layer: KSPlayerLayer) {
        layer.options.relaxVideoClockSyncWhileImmersiveCompositorActive =
            options.relaxVideoClockSyncWhileImmersiveCompositorActive
        layer.options.suppressWindowVideoPresentationWhileImmersiveCompositorActive =
            options.suppressWindowVideoPresentationWhileImmersiveCompositorActive
        layer.options.immersivePresentVideoFrame = options.immersivePresentVideoFrame
        layer.options.deliverDecodedVideoFrameToStereoCompositorWhileWindowVisible =
            options.deliverDecodedVideoFrameToStereoCompositorWhileWindowVisible
        layer.options.immersiveAudioPlaybackSeconds = options.immersiveAudioPlaybackSeconds
        syncImmersivePlaybackTimeline(from: layer)
    }

    private func syncImmersivePlaybackTimeline(
        from layer: KSPlayerLayer,
        playbackSeconds: TimeInterval? = nil
    ) {
        guard shouldDeliverVideoFramesForImmersive || isImmersiveStereoPresented || isOpeningImmersiveStereo else {
            return
        }
        var seconds = playbackSeconds ?? layer.player.currentPlaybackTime
        guard seconds.isFinite else {
            return
        }
        #if os(visionOS)
        if let videoPTS = ImmersiveVideoFeed.shared.latestVideoPTS,
           videoPTS - seconds > ImmersiveVideoFeed.bootstrapToleranceSeconds
        {
            // HLS decoded PTS can lead `currentPlaybackTime` by several seconds — follow video.
            seconds = videoPTS
        }
        #endif
        ImmersiveStereoPlaybackTimeline.update(
            playbackSeconds: seconds,
            isPlaying: layer.player.isPlaying,
            playbackRate: layer.player.playbackRate
        )
        options.immersiveAudioPlaybackSeconds = seconds
        layer.options.immersiveAudioPlaybackSeconds = seconds
        #if os(visionOS)
        ImmersiveVideoFeed.shared.setPlaybackAnchor(seconds)
        #endif
    }

    private func syncImmersivePlaybackTimelineIfPossible() {
        guard let layer = installedLayer ?? coordinator.playerLayer else {
            return
        }
        syncImmersiveSourceDisplayAspect(from: layer)
        syncImmersivePlaybackTimeline(from: layer)
    }

    #if os(visionOS)
    private func syncImmersiveSourceDisplayAspect(from layer: KSPlayerLayer) {
        let naturalSize = layer.player.naturalSize
        guard naturalSize.width > 0, naturalSize.height > 0 else {
            return
        }
        ImmersiveVideoFeed.shared.setSourceDisplayAspectRatio(
            Float(naturalSize.width / naturalSize.height)
        )
    }
    #endif

    private func syncImmersivePresentationStateIfNeeded() {
        syncImmersivePlaybackTimelineIfPossible()
        pipeline.syncRenderConfigurationSnapshot()
    }

    private static func outputLayoutName(_ layout: Video2DTo3DOutputLayout) -> String {
        switch layout {
        case .selectedEye:
            return "selected eye"
        case .sideBySide:
            return "side-by-side"
        case .topAndBottom:
            return "top-and-bottom"
        }
    }

    private static func appliedModeName(isEnabled: Bool, layout: Video2DTo3DOutputLayout) -> String {
        guard isEnabled else {
            return "disabled"
        }
        return "depthMapPreferred/\(outputLayoutName(layout))"
    }

    private static func presentationMode(for layout: Video2DTo3DOutputLayout) -> DA3PresentationMode {
        switch layout {
        case .selectedEye:
            return .windowSelectedEye
        case .sideBySide, .topAndBottom:
            return .packedPreview
        }
    }

    private static func outputLayout(for mode: DA3PresentationMode) -> Video2DTo3DOutputLayout {
        switch mode {
        case .native2D, .windowSelectedEye, .immersiveStereo:
            return .selectedEye
        case .packedPreview:
            return .sideBySide
        }
    }

    private func presentationModeDescription(isActive: Bool) -> String {
        switch presentationMode {
        case .native2D:
            return "Native 2D playback"
        case .windowSelectedEye:
            return isActive
                ? "2D window selected-eye preview; not headset per-eye stereo"
                : "Preparing 2D window selected-eye preview"
        case .packedPreview:
            return isActive
                ? "Packed stereo debug preview in a 2D window"
                : "Preparing packed stereo debug preview"
        case .immersiveStereo:
            return isActive
                ? "Immersive per-eye stereo via CompositorLayer"
                : "Opening immersive per-eye stereo"
        }
    }

    #if os(visionOS) && canImport(CompositorServices)
    private func refreshImmersivePresentationStatusIfNeeded() {
        guard presentationMode == .immersiveStereo, isImmersiveStereoPresented else {
            immersiveTemporalStatusLine = ""
            return
        }
        let temporal = ImmersiveStereoCompositorStatus.current().temporal
        let cadence = "presents \(temporal.compositorPresents) · advances \(temporal.selectionChanges) · fallback \(temporal.usedFallbackFrame)"
        let temporalLine = "\(temporal.summaryLine) · \(cadence)"
        if immersiveTemporalStatusLine != temporalLine {
            immersiveTemporalStatusLine = temporalLine
        }
        let status = immersiveCompositorStatusDescription()
        if presentationModeStatus != status {
            presentationModeStatus = status
            previewModeDescription = status
        }
    }

    private func immersiveCompositorStatusDescription() -> String {
        let compositor = ImmersiveStereoCompositorStatus.current()
        let base = presentationModeDescription(isActive: is2DTo3DEnabled)
        if compositor.presentedDrawables == 0, let lastIssue = compositor.lastIssue {
            return "\(base) — \(lastIssue)"
        }
        if compositor.drewVideoFrames > 0 {
            let temporal = compositor.temporal
            return "\(base) — video \(compositor.drewVideoFrames) · \(temporal.summaryLine)"
        }
        if compositor.drewPlaceholderFrames > 0 {
            return "\(base) — standby screen (\(compositor.drewPlaceholderFrames) draws, waiting for DA3)"
        }
        if compositor.presentedWithoutAnchor > 0 {
            return "\(base) — presenting without head anchor (\(compositor.presentedWithoutAnchor))"
        }
        if compositor.emptyDrawables > 0 {
            return "\(base) — compositor running but no drawables"
        }
        if compositor.queriedFrames > 0 {
            return "\(base) — compositor queried \(compositor.queriedFrames) frames"
        }
        return base
    }
    #endif

    /// Wire FFmpeg + `KSVideoFrameOutput` before DA3 is enabled so toggling 3D does not restart decode mid-GOP.
    private func prewireVideoOutputIfNeeded(on layer: KSPlayerLayer) {
        guard layer.player is KSMEPlayer else {
            return
        }
        applyStableFrameOutputDecodePolicyIfNeeded(to: layer)
        guard !isVideoOutputConfigured else {
            return
        }
        ensureVideoOutputWired(on: layer)
        Depth3DDebug.log(
            "Pre-wired KSVideoFrameOutput at playback ready (DA3 pipeline still idle until 3D enable)",
            phase: "video-output"
        )
    }

    private static func applyStableFrameOutputDecodeOptions(to options: KSOptions) {
        options.videoAdaptable = false
        options.hardwareDecode = false
        options.asynchronousDecompression = false
        options.requiresDecodedVideoFrameOutput = true
    }

    /// FFmpeg frame-output path from the first decoded frame (VideoToolbox mid-playback switches cause jumps / -12909 on HLS).
    private func applyStableFrameOutputDecodePolicyIfNeeded(
        to layer: KSPlayerLayer? = nil,
        reloadPlayerWhenActive: Bool = false
    ) {
        let toggledHardware = options.hardwareDecode
        Self.applyStableFrameOutputDecodeOptions(to: options)
        // IPTV/HLS: bias depth scheduling to stay responsive under segment/rebuffer churn.
        if Self.isLikelyAdaptiveStream(url) {
            pipeline.setIPTVStabilityBiasEnabled(true)
        } else {
            pipeline.setIPTVStabilityBiasEnabled(false)
        }
        if let layer = layer ?? installedLayer ?? coordinator.playerLayer {
            layer.options.videoAdaptable = false
            layer.options.hardwareDecode = false
            layer.options.asynchronousDecompression = false
            layer.options.requiresDecodedVideoFrameOutput = true
            if let mePlayer = layer.player as? KSMEPlayer {
                mePlayer.synchronizePresentationOptions(from: layer.options)
            }
        } else if reloadPlayerWhenActive {
            reloadPlayerPreservingPlaybackTime(restartIfEnded: false)
        }
        if toggledHardware || reloadPlayerWhenActive {
            KSLog("[video] Frame-output path: software decode enforced (VT evicted from decoderMap)")
            Depth3DDebug.log(
                "Frame-output path: software decode enforced (VT evicted from decoderMap)",
                phase: "video-output"
            )
        }
    }

    private func applyStableHLSDecodePolicyIfNeeded(reloadPlayerWhenActive: Bool = false) {
        applyStableFrameOutputDecodePolicyIfNeeded(reloadPlayerWhenActive: reloadPlayerWhenActive)
    }

    private static func isLikelyAdaptiveStream(_ url: URL) -> Bool {
        let absoluteString = url.absoluteString.lowercased()
        return ["m3u", "m3u8", "mpd", "ism", "isml"].contains(url.pathExtension.lowercased()) ||
            absoluteString.contains(".m3u8") ||
            absoluteString.contains(".mpd")
    }

    private func reloadPlayerPreservingPlaybackTime(restartIfEnded: Bool) {
        guard let layer = installedLayer ?? coordinator.playerLayer else {
            return
        }
        let currentTime = layer.player.currentPlaybackTime
        let wasPlaying = layer.player.isPlaying
        let shouldRestart = restartIfEnded && playerState == .playedToTheEnd
        let targetTime = shouldRestart ? 0 : (currentTime.isFinite ? currentTime : 0)
        layer.set(
            url: url,
            options: options,
            preservingCurrentTime: targetTime
        )
        if isDepthWarmupActive {
            depthWarmupStartedAt = nil
            depthWarmupStatusMessage = "Depth warmup pending (rebuffering)"
            return
        }
        if wasPlaying || shouldRestart {
            layer.play()
        }
    }
}

@MainActor
private final class DA3CachedDepthMapProvider: VideoDepthEstimationProvider {
    let providerID = "depth-anything-v3-demo"

    private let pipeline: DA3VideoOutputPipeline

    init(pipeline: DA3VideoOutputPipeline) {
        self.pipeline = pipeline
    }

    func makeDepthMap(request: VideoDepthEstimationRequest) throws -> VideoDepthMap? {
        pipeline.cachedDepthMap(for: request)
    }
}

/// Stable `KSVideoFrameOutput` handler — re-assigning a new closure each configure flushes the tap.
private final class DA3DecodedVideoFrameSink: @unchecked Sendable {
    weak var pipeline: DA3VideoOutputPipeline?

    @Sendable
    func receive(_ pixelBuffer: CVPixelBuffer) {
        pipeline?.enqueue(pixelBuffer)
    }
}

private final class DA3VideoOutputPipeline: @unchecked Sendable {
    private struct DepthValueSummary {
        let rawMinimum: Float?
        let rawMaximum: Float?
        let normalizedMinimum: Float?
        let normalizedMaximum: Float?
    }

    private final class RetainedPixelBuffer: @unchecked Sendable {
        let pixelBuffer: CVPixelBuffer
        let mediaTime: TimeInterval?

        init(_ pixelBuffer: CVPixelBuffer) {
            self.pixelBuffer = pixelBuffer
            self.mediaTime = Self.mediaTime(from: pixelBuffer)
        }

        static func mediaTime(from pixelBuffer: CVPixelBuffer) -> TimeInterval? {
            guard let attachment = CVBufferCopyAttachment(
                pixelBuffer,
                KSVideoFrameOutputMetadata.presentationTimeSecondsKey,
                nil
            ) as? NSNumber else {
                return nil
            }
            let seconds = attachment.doubleValue
            return seconds.isFinite ? seconds : nil
        }
    }

    private let engine: DepthAnythingV3Engine
    private var bridge: DA3DepthMetalBridge
    private var depthGPUProcessor: DA3DepthGPUProcessor?
    private weak var renderer: (any StereoRendererProtocol)?
    private let staleDepthFrameOffset: TimeInterval
    private var performanceMode: DA3VisionProPerformanceMode
    private var debugRenderMode: DA3DebugRenderMode
    private var maximumInferenceFPS: Double
    private var sourceVideoFrameRate: Double?
    private var inferenceFPSCap: Double?
    private var adaptiveInferenceFPSCap: Double?
    private var usesAdaptiveInferenceThrottling = true
    private var iptvStabilityBiasEnabled = false
    private var hasLoggedFirstVideoOutputCallback: Bool = false
    private var depthWorkerScheduled = false
    private var temporalSmoothingFactor: Float
    private var notifiedFirstVideoFrame = false
    private var depthContrast: Float
    private var isDepthInverted: Bool
    private var onVideoFrame: @MainActor @Sendable (TimeInterval?) -> Void = { _ in }
    private var onError: @MainActor @Sendable (Error) -> Void = { _ in }
    private var onMetrics: @MainActor @Sendable (DA3PipelineMetrics) -> Void = { _ in }
    private var renderConfigurationProvider: @MainActor @Sendable () -> Video2DTo3DRenderConfiguration = { .disabled }
    private var immersivePresentationSync: (@MainActor () -> Void)?
    private var processingTask: Task<Void, Never>?
    private let lock = NSLock()
    private var isEnabled = false
    private var deliversImmersiveVideo = false
    private var compositorVideoFromWindowPresentation = false
    private var suppressImmersiveVideoUpdates = false
    private var skipsDepthMapBuild = false
    private var usesContinuousDepthScheduling = false
    private var isProcessing = false
    private var pendingDepthFrame: RetainedPixelBuffer?
    private var processedFrameCount = 0
    private var droppedFrameCount = 0
    private var coalescedFrameCount = 0
    private var throttledFrameCount = 0
    private var staleDepthFrameCount = 0
    private var processingFPS = 0.0
    private var lastSuccessTime: TimeInterval?
    private var lastAcceptedTime: TimeInterval?
    private var lastAcceptedMediaTime: TimeInterval?
    private var latestDepthMediaTime: TimeInterval?
    private var latestDepthMap: VideoDepthMap?
    private var latestModelDiagnostics = DA3ModelDiagnostics.empty
    private var metrics = DA3PipelineMetrics.empty
    private var lastPreprocessingDuration: TimeInterval?
    private var lastInferenceDuration: TimeInterval?
    private var lastOutputExtractionDuration: TimeInterval?
    private var lastTextureUploadDuration: TimeInterval?
    private var lastDepthMapDuration: TimeInterval?
    private var lastRendererUpdateDuration: TimeInterval?
    private var lastRenderDepthTextureUploadDuration: TimeInterval?
    private var lastRenderDepthSmoothingDuration: TimeInterval?
    private var lastRenderDuration: TimeInterval?
    private var lastRenderStereoPassCount = 0
    private var processingJobID = 0
    private var cachedRenderConfiguration: Video2DTo3DRenderConfiguration = .disabled
    private let depthWorkQueue = DispatchQueue(label: "KSPlayer.DA3.depth-work", qos: .userInteractive)
    private var latestDepthInput: RetainedPixelBuffer?
    private var currentPhase = "idle"
    private var phaseStartedAt: TimeInterval?
    private let watchdogTimeout: TimeInterval = 2

    init(
        engine: DepthAnythingV3Engine,
        bridge: DA3DepthMetalBridge,
        depthGPUProcessor: DA3DepthGPUProcessor?,
        renderer: any StereoRendererProtocol,
        performanceMode: DA3VisionProPerformanceMode,
        debugRenderMode: DA3DebugRenderMode,
        staleDepthFrameOffset: TimeInterval,
        depthContrast: Float,
        isDepthInverted: Bool
    ) {
        self.engine = engine
        self.bridge = bridge
        self.depthGPUProcessor = depthGPUProcessor
        self.renderer = renderer
        self.performanceMode = performanceMode
        self.debugRenderMode = debugRenderMode
        self.maximumInferenceFPS = performanceMode.maximumInferenceFPS
        self.usesContinuousDepthScheduling = performanceMode.usesContinuousDepthScheduling
        self.temporalSmoothingFactor = performanceMode.temporalSmoothingFactor
        self.staleDepthFrameOffset = staleDepthFrameOffset
        self.depthContrast = DA3VisionProDemoTuning.validatedDepthContrast(depthContrast)
        self.isDepthInverted = isDepthInverted
        metrics = makeMetrics(state: .disabled)
    }

    func setErrorHandler(_ onError: @escaping @MainActor @Sendable (Error) -> Void) {
        self.onError = onError
    }

    func setMetricsHandler(_ onMetrics: @escaping @MainActor @Sendable (DA3PipelineMetrics) -> Void) {
        self.onMetrics = onMetrics
        Task { @MainActor in
            onMetrics(self.currentMetrics())
        }
    }

    func setVideoFrameHandler(_ onVideoFrame: @escaping @MainActor @Sendable (TimeInterval?) -> Void) {
        self.onVideoFrame = onVideoFrame
    }

    func setRenderConfigurationProvider(
        _ provider: @escaping @MainActor @Sendable () -> Video2DTo3DRenderConfiguration
    ) {
        renderConfigurationProvider = provider
        syncRenderConfigurationSnapshot()
    }

    func setImmersivePresentationSync(_ handler: @escaping @MainActor () -> Void) {
        immersivePresentationSync = handler
    }

    func syncRenderConfigurationSnapshot() {
        // Critical: never synchronously block the main thread from the depth worker.
        // Render configuration can be slightly stale; keeping depth work off the main thread avoids bottlenecks.
        if Thread.isMainThread {
            MainActor.assumeIsolated {
                cachedRenderConfiguration = renderConfigurationProvider()
            }
            return
        }
        Task { @MainActor [weak self] in
            guard let self else { return }
            self.cachedRenderConfiguration = self.renderConfigurationProvider()
        }
    }

    private func syncImmersivePresentationIfNeeded() {
        guard let immersivePresentationSync else {
            return
        }
        // Never block the decode/depth worker waiting on the main thread.
        if Thread.isMainThread {
            MainActor.assumeIsolated { immersivePresentationSync() }
            return
        }
        Task { @MainActor in
            immersivePresentationSync()
        }
    }

    private func currentRenderConfiguration() -> Video2DTo3DRenderConfiguration {
        lock.lock()
        defer {
            lock.unlock()
        }
        return cachedRenderConfiguration
    }

    /// Rebind depth texture allocation to the CompositorLayer GPU when it differs from init-time default.
    func reconfigureMetalDeviceIfNeeded(_ device: any MTLDevice) {
        lock.lock()
        defer {
            lock.unlock()
        }
        guard !bridge.usesSameDevice(as: device) else {
            return
        }
        bridge = DA3DepthMetalBridge(device: device)
        Depth3DDebug.log(
            "DA3 depth bridge switched to compositor Metal device",
            phase: "immersive-compositor"
        )
    }

    func setEnabled(_ isEnabled: Bool) {
        let metrics = updateEnabledState(isEnabled)
        Task { @MainActor in
            self.onMetrics(metrics)
        }
    }

    func setDeliversImmersiveVideo(_ isEnabled: Bool) {
        lock.lock()
        deliversImmersiveVideo = isEnabled
        lock.unlock()
    }

    func setCompositorVideoFromWindowPresentation(_ isEnabled: Bool) {
        lock.lock()
        compositorVideoFromWindowPresentation = isEnabled
        lock.unlock()
    }

    private var isCompositorVideoFromWindowPresentation: Bool {
        lock.lock()
        defer {
            lock.unlock()
        }
        return compositorVideoFromWindowPresentation
    }

    var hasReceivedVideoOutputCallback: Bool {
        lock.lock()
        defer {
            lock.unlock()
        }
        return hasLoggedFirstVideoOutputCallback
    }

    func setSuppressImmersiveVideoUpdates(_ suppress: Bool) {
        lock.lock()
        suppressImmersiveVideoUpdates = suppress
        lock.unlock()
    }

    var isSuppressingImmersiveVideoUpdates: Bool {
        lock.lock()
        defer { lock.unlock() }
        return suppressImmersiveVideoUpdates
    }

    func setDepthProcessingOptimizations(
        skipsDepthMapBuild: Bool,
        usesContinuousDepthScheduling: Bool
    ) {
        lock.lock()
        self.skipsDepthMapBuild = skipsDepthMapBuild
        self.usesContinuousDepthScheduling = usesContinuousDepthScheduling
        lock.unlock()
    }

    func setDepthProcessing(depthContrast: Float, isDepthInverted: Bool) {
        let metrics = updateDepthProcessing(
            depthContrast: DA3VisionProDemoTuning.validatedDepthContrast(depthContrast),
            isDepthInverted: isDepthInverted
        )
        Task { @MainActor in
            self.onMetrics(metrics)
        }
    }

    func setPerformanceMode(_ mode: DA3VisionProPerformanceMode) {
        let metrics = updatePerformanceMode(mode)
        Task { @MainActor in
            self.onMetrics(metrics)
        }
    }

    func setInferenceFPSCap(_ fps: Double?) {
        lock.lock()
        if let fps, fps > 0 {
            inferenceFPSCap = fps
            usesAdaptiveInferenceThrottling = false
        } else {
            inferenceFPSCap = nil
            usesAdaptiveInferenceThrottling = true
        }
        syncInferenceRateToSourceLocked()
        lock.unlock()
    }

    func setIPTVStabilityBiasEnabled(_ enabled: Bool) {
        lock.lock()
        iptvStabilityBiasEnabled = enabled
        lock.unlock()
    }

    func setSourceVideoFrameRate(_ fps: Double?) {
        lock.lock()
        if let fps, fps.isFinite, fps > 0 {
            sourceVideoFrameRate = fps
        } else {
            sourceVideoFrameRate = nil
        }
        syncInferenceRateToSourceLocked()
        lock.unlock()
    }

    func setTemporalSmoothingFactor(_ factor: Float) {
        let metrics = updateTemporalSmoothingFactor(factor)
        Task { @MainActor in
            self.onMetrics(metrics)
        }
    }

    func setDebugRenderMode(_ mode: DA3DebugRenderMode) {
        let metrics = updateDebugRenderMode(mode)
        Task { @MainActor in
            self.onMetrics(metrics)
        }
    }

    func recordRenderMetrics(_ renderMetrics: Video2DTo3DRenderMetrics) {
        let metrics = updateRenderMetrics(renderMetrics)
        Task { @MainActor in
            self.onMetrics(metrics)
        }
    }

    func prepareForActivation() async throws -> DA3ModelDiagnostics {
        Depth3DDebug.log("Preparing DA3 pipeline before renderer switch", phase: "activation-prepare")
        let diagnostics = try await engine.prepare()
        let metrics = updatePreparedDiagnostics(diagnostics)
        Task { @MainActor in
            self.onMetrics(metrics)
        }
        return diagnostics
    }

    func cachedDepthMap(for request: VideoDepthEstimationRequest) -> VideoDepthMap? {
        lock.lock()
        defer {
            lock.unlock()
        }
        guard isEnabled else {
            return nil
        }
        updateDepthFrameOffset(for: request.presentationTime)
        return latestDepthMap
    }

    func enqueue(_ pixelBuffer: CVPixelBuffer) {
        let frameBuffer = VideoPixelBufferNV12Normalization.nv12VideoRangeCopyIfNeeded(from: pixelBuffer)
            ?? pixelBuffer
        let width = CVPixelBufferGetWidth(frameBuffer)
        let height = CVPixelBufferGetHeight(frameBuffer)
        let pixelFormat = CVPixelBufferGetPixelFormatType(frameBuffer)
        let mediaTime = RetainedPixelBuffer.mediaTime(from: frameBuffer)
        guard width > 0, height > 0 else {
            Depth3DDebug.fail("Dropped invalid videoOutput pixel buffer \(width)x\(height) format \(Self.pixelFormatDescription(pixelFormat)) time \(mediaTime.map { String(format: "%.3f", $0) } ?? "unknown")", phase: "video-output")
            return
        }
        lock.lock()
        if !hasLoggedFirstVideoOutputCallback {
            hasLoggedFirstVideoOutputCallback = true
            lock.unlock()
            let firstCallbackMessage =
                "First KSVideoFrameOutput callback \(width)x\(height) format \(Self.pixelFormatDescription(pixelFormat)) "
                + "time \(mediaTime.map { String(format: "%.3f", $0) } ?? "unknown")"
            Depth3DDebug.log(firstCallbackMessage, phase: "video-output")
            KSLog("[video] \(firstCallbackMessage)")
        } else {
            lock.unlock()
        }
        Depth3DDebug.log("videoOutput frame \(width)x\(height) format \(Self.pixelFormatDescription(pixelFormat)) time \(mediaTime.map { String(format: "%.3f", $0) } ?? "unknown")", phase: "video-output", verboseOnly: true)

        #if os(visionOS)
        // Use the player-provided anchor (set from `KSPlayerLayer.currentPlaybackTime`) when deciding
        // whether to accept decoded frames. `nowMediaSeconds()` can be a wall-clock-like timeline
        // before the immersive playback clock is anchored, which would incorrectly reject HLS frames.
        let anchor = ImmersiveVideoFeed.shared.playbackAnchorSnapshot
        let ringEmpty = !ImmersiveVideoFeed.shared.hasFrames
        let withinAudioSync = ringEmpty
            || ImmersiveVideoFeed.shouldAcceptDecodedFrame(
                mediaTime: mediaTime,
                bufferIsEmpty: ringEmpty,
                playbackAnchor: anchor
            )
        let immersiveRequested = ImmersiveStereoSession.isUserRequestedActive
        let deliversToStereoFeed = immersiveRequested || isDeliveringImmersiveVideo
        let feedVideoFromDecodeOutput = deliversToStereoFeed && !isCompositorVideoFromWindowPresentation
        let bootstrapRing = feedVideoFromDecodeOutput && ringEmpty
        if feedVideoFromDecodeOutput,
           withinAudioSync,
           bootstrapRing || !isSuppressingImmersiveVideoUpdates
        {
            syncImmersivePresentationIfNeeded()
            let configuration = currentRenderConfiguration()
            let appended = ImmersiveVideoFeed.shared.append(
                pixelBuffer: frameBuffer,
                mediaTime: mediaTime,
                configuration: configuration
            )
            if appended, ringEmpty {
                if let mediaTime, mediaTime.isFinite {
                    ImmersiveVideoFeed.shared.reanchorPlaybackClock(toVideoPTS: mediaTime)
                }
                let playback = ImmersiveStereoPlaybackTimeline.nowMediaSeconds()
                let offset = mediaTime.map { $0 - playback } ?? 0
                let feedDiag = ImmersiveVideoFeed.shared.diagnosticsSnapshot()
                let destination = immersiveRequested ? "immersive" : "cinema"
                let ringMessage =
                    "First \(destination) video feed frame pts \(mediaTime.map { String(format: "%.3f", $0) } ?? "unknown") "
                    + "audio \(String(format: "%.3f", playback)) offset \(String(format: "%+.3f", offset))s "
                    + "ring=\(feedDiag.ringCount) reject(out=\(feedDiag.rejectedOutsideWindow) accept=\(feedDiag.rejectedNotAccepting))"
                Depth3DDebug.log(ringMessage, phase: "immersive-compositor")
                KSLog("[video] \(ringMessage)")
            } else if feedVideoFromDecodeOutput, ringEmpty, !appended {
                let feedDiag = ImmersiveVideoFeed.shared.diagnosticsSnapshot()
                let rejectMessage =
                    "Immersive feed rejected frame pts \(mediaTime.map { String(format: "%.3f", $0) } ?? "unknown") "
                    + "anchor \(String(format: "%.3f", anchor)) accept=\(feedDiag.isAcceptingFrames) "
                    + "reject(notAccepting=\(feedDiag.rejectedNotAccepting) outside=\(feedDiag.rejectedOutsideWindow))"
                Depth3DDebug.warn(rejectMessage, phase: "immersive-compositor")
                KSLog("[video] \(rejectMessage)")
            }
        }
        #else
        let withinAudioSync = true
        #endif

        lock.lock()
        guard isEnabled, withinAudioSync else {
            lock.unlock()
            return
        }
        latestDepthInput = RetainedPixelBuffer(frameBuffer)
        lock.unlock()
        scheduleDepthWorker()
    }

    private func scheduleDepthWorker() {
        lock.lock()
        if depthWorkerScheduled {
            lock.unlock()
            return
        }
        depthWorkerScheduled = true
        lock.unlock()
        depthWorkQueue.async { [weak self] in
            guard let self else {
                return
            }
            self.lock.lock()
            self.depthWorkerScheduled = false
            self.lock.unlock()
            self.processNextDepthFrameIfNeeded()
        }
    }

    private func processNextDepthFrameIfNeeded() {
        let frame: RetainedPixelBuffer
        lock.lock()
        guard isEnabled else {
            lock.unlock()
            return
        }
        guard !isProcessing, let input = latestDepthInput else {
            lock.unlock()
            return
        }
        latestDepthInput = nil
        frame = input
        lock.unlock()

        switch beginProcessing(mediaTime: frame.mediaTime, pixelBuffer: frame.pixelBuffer) {
        case .disabled:
            scheduleDepthWorker()
            return
        case let .coalesced(metrics), let .throttled(metrics):
            Task { @MainActor in
                self.onMetrics(metrics)
            }
            scheduleDepthWorker()
            return
        case let .started(metrics):
            notifyFirstVideoFrameIfNeeded(frame.mediaTime)
            Task { @MainActor in
                self.onMetrics(metrics)
            }
            startDepthProcessing(frame)
        }
    }

    private func startDepthProcessing(_ retainedFrame: RetainedPixelBuffer) {
        let startTime = Date().timeIntervalSinceReferenceDate
        let jobID = currentProcessingJobID()
        scheduleWatchdog(jobID: jobID)
        processingTask = Task { [weak self, retainedFrame] in
            guard let self else {
                return
            }
            defer {
                self.finishProcessing()
                self.clearProcessingTaskIfCurrent()
                self.startPendingDepthProcessingIfNeeded()
                self.scheduleDepthWorker()
            }

            do {
                self.setProcessingPhase("coreml-prediction", jobID: jobID)
                let prediction = try await self.engine.makeDepthPrediction(from: retainedFrame.pixelBuffer)
                let depthOutput = prediction.depthOutput
                let textureUploadStart = Date().timeIntervalSinceReferenceDate
                self.setProcessingPhase("depth-texture", jobID: jobID)
                let processing = self.currentDepthProcessing()
                let gpuParameters = DA3DepthGPUProcessingParameters(
                    rawMinimum: depthOutput.rawMinimum ?? 0,
                    rawMaximum: depthOutput.rawMaximum ?? 1,
                    contrast: processing.depthContrast,
                    invertDepth: processing.isDepthInverted,
                    temporalSmoothingFactor: self.currentTemporalSmoothingFactor()
                )
                let depthTexture: any MTLTexture
                if let depthGPUProcessor = self.depthGPUProcessor {
                    depthTexture = try self.bridge.makeDisplayDepthTexture(
                        depthOutput: depthOutput,
                        processor: depthGPUProcessor,
                        parameters: gpuParameters
                    )
                } else {
                    let fallbackFrame = try DA3DepthFrame(
                        multiArray: depthOutput.multiArray,
                        outputName: prediction.diagnostics.outputName
                    )
                    depthTexture = try self.bridge.makeTexture(from: fallbackFrame)
                }
                let textureUploadDuration = Date().timeIntervalSinceReferenceDate - textureUploadStart
                let depthMapStart = Date().timeIntervalSinceReferenceDate
                let (depthMap, depthSummary): (VideoDepthMap?, DepthValueSummary)
                if self.skipsDepthMapBuild {
                    depthMap = nil
                    depthSummary = DepthValueSummary(
                        rawMinimum: depthOutput.rawMinimum,
                        rawMaximum: depthOutput.rawMaximum,
                        normalizedMinimum: 0,
                        normalizedMaximum: 1
                    )
                } else {
                    self.setProcessingPhase("depth-map", jobID: jobID)
                    let depthFrame = try DA3DepthFrame(
                        multiArray: depthOutput.multiArray,
                        outputName: prediction.diagnostics.outputName
                    )
                    (depthMap, depthSummary) = self.makeDepthMap(from: depthFrame)
                }
                let depthMapDuration = Date().timeIntervalSinceReferenceDate - depthMapStart
                let rendererUpdateStart = Date().timeIntervalSinceReferenceDate
                self.setProcessingPhase("renderer-update", jobID: jobID)
                guard self.isCurrentProcessingJob(jobID) else {
                    return
                }
                self.syncRenderConfigurationSnapshot()
                let configuration = self.currentRenderConfiguration()
                let deliversImmersiveVideo = self.isDeliveringImmersiveVideo
                let depthFrameForWindow = try? DA3DepthFrame(
                    multiArray: depthOutput.multiArray,
                    outputName: prediction.diagnostics.outputName
                )
                await MainActor.run {
                    if deliversImmersiveVideo, configuration.usesDepthMap {
                        self.renderer?.updateDepth(
                            depthTexture: depthTexture,
                            configuration: configuration,
                            mediaTime: retainedFrame.mediaTime
                        )
                    } else if let depthFrameForWindow {
                        self.renderer?.update(
                            sourceFrame: retainedFrame.pixelBuffer,
                            depthFrame: depthFrameForWindow,
                            depthTexture: depthTexture,
                            configuration: configuration,
                            mediaTime: retainedFrame.mediaTime
                        )
                    }
                }
                let rendererUpdateDuration = Date().timeIntervalSinceReferenceDate - rendererUpdateStart
                guard self.isCurrentProcessingJob(jobID) else {
                    return
                }
                guard let metrics = self.recordSuccess(
                    depthWidth: depthOutput.width,
                    depthHeight: depthOutput.height,
                    depthMap: depthMap,
                    depthSummary: depthSummary,
                    engineTimings: prediction.timings,
                    modelDiagnostics: prediction.diagnostics,
                    textureUploadDuration: textureUploadDuration,
                    depthMapDuration: depthMapDuration,
                    rendererUpdateDuration: rendererUpdateDuration,
                    mediaTime: retainedFrame.mediaTime,
                    duration: Date().timeIntervalSinceReferenceDate - startTime
                ) else {
                    return
                }
                await MainActor.run {
                    self.onMetrics(metrics)
                }
            } catch is CancellationError {
                Depth3DDebug.log("DA3 job \(jobID) cancelled for newer frame", phase: "pipeline", verboseOnly: true)
                return
            } catch {
                Depth3DDebug.fail("DA3 processing failed in \(self.currentProcessingPhase()): \(error.localizedDescription)", phase: "pipeline-error")
                let metrics = self.recordFailure(error)
                await MainActor.run {
                    self.onError(error)
                    self.onMetrics(metrics)
                }
            }
        }
    }

    private func startPendingDepthProcessingIfNeeded() {
        let pending: RetainedPixelBuffer?
        lock.lock()
        pending = pendingDepthFrame
        pendingDepthFrame = nil
        let shouldStart = isEnabled && !isProcessing && pending != nil
        lock.unlock()
        guard shouldStart, let pending else {
            return
        }
        switch beginProcessing(mediaTime: pending.mediaTime, pixelBuffer: pending.pixelBuffer) {
        case .disabled, .coalesced, .throttled:
            return
        case let .started(metrics):
            Task { @MainActor in
                self.onMetrics(metrics)
            }
            startDepthProcessing(pending)
        }
    }

    private func notifyFirstVideoFrameIfNeeded(_ mediaTime: TimeInterval?) {
        lock.lock()
        let shouldNotify = isEnabled && !notifiedFirstVideoFrame
        if shouldNotify {
            notifiedFirstVideoFrame = true
        }
        lock.unlock()
        guard shouldNotify else {
            return
        }
        Task { @MainActor in
            self.onVideoFrame(mediaTime)
        }
    }

    func reset() {
        lock.lock()
        processingTask?.cancel()
        processingTask = nil
        isProcessing = false
        notifiedFirstVideoFrame = false
        processedFrameCount = 0
        droppedFrameCount = 0
        coalescedFrameCount = 0
        throttledFrameCount = 0
        pendingDepthFrame = nil
        latestDepthInput = nil
        adaptiveInferenceFPSCap = nil
        usesAdaptiveInferenceThrottling = true
        hasLoggedFirstVideoOutputCallback = false
        depthWorkerScheduled = false
        depthGPUProcessor?.resetTemporalSmoothing()
        staleDepthFrameCount = 0
        processingFPS = 0
        lastSuccessTime = nil
        lastAcceptedTime = nil
        lastAcceptedMediaTime = nil
        latestDepthMediaTime = nil
        latestDepthMap = nil
        latestModelDiagnostics = .empty
        lastPreprocessingDuration = nil
        lastInferenceDuration = nil
        lastOutputExtractionDuration = nil
        lastTextureUploadDuration = nil
        lastDepthMapDuration = nil
        lastRendererUpdateDuration = nil
        lastRenderDepthTextureUploadDuration = nil
        lastRenderDepthSmoothingDuration = nil
        lastRenderDuration = nil
        lastRenderStereoPassCount = 0
        currentPhase = "idle"
        phaseStartedAt = nil
        metrics = makeMetrics(state: .disabled, preserveTransientTiming: false, depthWidth: 0, depthHeight: 0)
        lock.unlock()
    }

    private enum ProcessingStart {
        case disabled
        case coalesced(DA3PipelineMetrics)
        case throttled(DA3PipelineMetrics)
        case started(DA3PipelineMetrics)
    }

    private func beginProcessing(mediaTime: TimeInterval?, pixelBuffer: CVPixelBuffer) -> ProcessingStart {
        let now = Date().timeIntervalSinceReferenceDate
        lock.lock()
        defer {
            lock.unlock()
        }
        guard isEnabled else {
            return .disabled
        }
        // IPTV/HLS often has timeline discontinuities (segment switches, seeks). Reset temporal state so
        // depth doesn’t “drag” behind video and cause rubber-banding.
        if let mediaTime, mediaTime.isFinite, let lastPTS = lastAcceptedMediaTime {
            let delta = mediaTime - lastPTS
            if delta > 1.0 || delta < -0.25 {
                depthGPUProcessor?.resetTemporalSmoothing()
                latestDepthMap = nil
                latestDepthMediaTime = nil
            }
        }
        if usesContinuousDepthScheduling {
            guard !isProcessing else {
                pendingDepthFrame = RetainedPixelBuffer(pixelBuffer)
                coalescedFrameCount += 1
                metrics = makeMetrics(state: .processing)
                return .coalesced(metrics)
            }
            if shouldThrottleDepthInferenceLocked(now: now, mediaTime: mediaTime) {
                throttledFrameCount += 1
                metrics = makeMetrics(state: latestDepthMap == nil ? .idle : .active)
                return .throttled(metrics)
            }
            lastAcceptedTime = now
            if let mediaTime, mediaTime.isFinite {
                lastAcceptedMediaTime = mediaTime
            }
            isProcessing = true
            processingJobID += 1
            currentPhase = "accepted"
            phaseStartedAt = now
            metrics = makeMetrics(state: .processing)
            return .started(metrics)
        }

        if shouldThrottleDepthInferenceLocked(now: now, mediaTime: mediaTime) {
            throttledFrameCount += 1
            metrics = makeMetrics(state: latestDepthMap == nil ? .idle : .active)
            return .throttled(metrics)
        }
        guard !isProcessing else {
            pendingDepthFrame = RetainedPixelBuffer(pixelBuffer)
            coalescedFrameCount += 1
            metrics = makeMetrics(state: .processing)
            return .coalesced(metrics)
        }
        lastAcceptedTime = now
        if let mediaTime, mediaTime.isFinite {
            lastAcceptedMediaTime = mediaTime
        }
        isProcessing = true
        processingJobID += 1
        currentPhase = "accepted"
        phaseStartedAt = now
        metrics = makeMetrics(state: .processing)
        return .started(metrics)
    }

    private func isCurrentProcessingJob(_ jobID: Int) -> Bool {
        lock.lock()
        defer {
            lock.unlock()
        }
        return isProcessing && processingJobID == jobID
    }

    private func finishProcessing() {
        lock.lock()
        isProcessing = false
        currentPhase = "idle"
        phaseStartedAt = nil
        lock.unlock()
    }

    private func clearProcessingTaskIfCurrent() {
        lock.lock()
        processingTask = nil
        lock.unlock()
    }

    private var isDeliveringImmersiveVideo: Bool {
        lock.lock()
        defer {
            lock.unlock()
        }
        return deliversImmersiveVideo
    }

    private func currentProcessingJobID() -> Int {
        lock.lock()
        defer {
            lock.unlock()
        }
        return processingJobID
    }

    private func currentProcessingPhase() -> String {
        lock.lock()
        defer {
            lock.unlock()
        }
        return currentPhase
    }

    private func setProcessingPhase(_ phase: String, jobID: Int) {
        let now = Date().timeIntervalSinceReferenceDate
        lock.lock()
        guard processingJobID == jobID, isProcessing else {
            lock.unlock()
            return
        }
        currentPhase = phase
        phaseStartedAt = now
        lock.unlock()
        Depth3DDebug.log("DA3 job \(jobID) phase \(phase)", phase: phase, verboseOnly: true)
    }

    private func scheduleWatchdog(jobID: Int) {
        let timeout = watchdogTimeout
        Task { [weak self] in
            while !Task.isCancelled {
                try? await Task.sleep(nanoseconds: UInt64(timeout * 1_000_000_000))
                guard self?.reportStuckJobIfNeeded(jobID: jobID) == true else {
                    return
                }
            }
        }
    }

    @discardableResult
    private func reportStuckJobIfNeeded(jobID: Int) -> Bool {
        let now = Date().timeIntervalSinceReferenceDate
        lock.lock()
        let isStuck = isProcessing && processingJobID == jobID
        let phase = currentPhase
        let startedAt = phaseStartedAt
        let metricsSnapshot = metrics
        lock.unlock()

        guard isStuck, let startedAt, now - startedAt >= watchdogTimeout else {
            return isStuck
        }
        Depth3DDebug.warn(
            String(
                format: "DA3 job %d stuck in %@ for %.2fs. Last timings pre=%@ infer=%@ upload=%@ render=%@",
                jobID,
                phase,
                now - startedAt,
                Self.formatDuration(metricsSnapshot.preprocessingDuration),
                Self.formatDuration(metricsSnapshot.inferenceDuration),
                Self.formatDuration(metricsSnapshot.textureUploadDuration),
                Self.formatDuration(metricsSnapshot.renderDuration)
            ),
            phase: "pipeline-watchdog"
        )
        return true
    }

    private func updateEnabledState(_ enabled: Bool) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        isEnabled = enabled
        notifiedFirstVideoFrame = false
        if !enabled {
            isProcessing = false
            latestDepthMap = nil
            latestDepthMediaTime = nil
            processingFPS = 0
            lastSuccessTime = nil
            lastAcceptedTime = nil
            lastAcceptedMediaTime = nil
            pendingDepthFrame = nil
            lastPreprocessingDuration = nil
            lastInferenceDuration = nil
            lastOutputExtractionDuration = nil
            lastTextureUploadDuration = nil
            lastDepthMapDuration = nil
            lastRendererUpdateDuration = nil
            currentPhase = "idle"
            phaseStartedAt = nil
        }
        metrics = makeMetrics(
            state: enabled ? .idle : .disabled,
            preserveTransientTiming: false,
            depthWidth: enabled ? metrics.depthWidth : 0,
            depthHeight: enabled ? metrics.depthHeight : 0
        )
        return metrics
    }

    private func updateDepthProcessing(depthContrast: Float, isDepthInverted: Bool) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        self.depthContrast = depthContrast
        self.isDepthInverted = isDepthInverted
        latestDepthMap = nil
        latestDepthMediaTime = nil
        metrics = makeMetrics(
            state: isEnabled ? .idle : .disabled,
            preserveTransientTiming: false,
            rawDepthMinimum: nil,
            rawDepthMaximum: nil,
            normalizedDepthMinimum: nil,
            normalizedDepthMaximum: nil
        )
        return metrics
    }

    private func updatePerformanceMode(_ mode: DA3VisionProPerformanceMode) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        performanceMode = mode
        syncInferenceRateToSourceLocked()
        usesContinuousDepthScheduling = mode.usesContinuousDepthScheduling
        temporalSmoothingFactor = mode.temporalSmoothingFactor
        metrics = makeMetrics(state: isEnabled ? .idle : .disabled)
        return metrics
    }

    private func syncInferenceRateToSourceLocked() {
        // Video follows source PTS at full frame rate; depth stays on the performance-mode cap.
        maximumInferenceFPS = performanceMode.maximumInferenceFPS
    }

    private func effectiveInferenceFPSLocked() -> Double {
        var fps = performanceMode.maximumInferenceFPS
        if usesAdaptiveInferenceThrottling, let adaptiveInferenceFPSCap, adaptiveInferenceFPSCap > 0 {
            fps = min(fps, adaptiveInferenceFPSCap)
        }
        if let cap = inferenceFPSCap, cap > 0 {
            fps = min(fps, cap)
        }
        if iptvStabilityBiasEnabled {
            // Keep headroom for decode/rebuffer and compositor; depth is best-effort.
            fps = min(fps, 12)
        }
        return fps
    }

    private func shouldThrottleDepthInferenceLocked(now: TimeInterval, mediaTime: TimeInterval?) -> Bool {
        let effectiveInferenceFPS = effectiveInferenceFPSLocked()
        guard effectiveInferenceFPS > 0 else {
            return false
        }
        let minInterval = 1 / effectiveInferenceFPS
        if let mediaTime, mediaTime.isFinite {
            if let lastPTS = lastAcceptedMediaTime, mediaTime + 0.001 < lastPTS {
                return true
            }
            if let lastPTS = lastAcceptedMediaTime, mediaTime - lastPTS < minInterval * 0.9 {
                return true
            }
        } else if let lastAcceptedTime, now - lastAcceptedTime < minInterval {
            return true
        }
        return false
    }

    private func updateAdaptiveInferenceCapLocked(engineTimings: DA3EngineTimings) {
        guard usesAdaptiveInferenceThrottling else {
            return
        }
        let workload = engineTimings.preprocessingDuration
            + engineTimings.inferenceDuration
            + engineTimings.outputExtractionDuration
        guard workload.isFinite, workload > 0.005 else {
            return
        }
        let safety = iptvStabilityBiasEnabled ? 1.35 : 1.15
        let sustainableFPS = min(
            performanceMode.maximumInferenceFPS,
            max(2, 1.0 / (workload * safety))
        )
        if let existing = adaptiveInferenceFPSCap {
            adaptiveInferenceFPSCap = (existing * 0.65) + (sustainableFPS * 0.35)
        } else {
            adaptiveInferenceFPSCap = sustainableFPS
        }
        if engineTimings.inferenceDuration > 0.12 {
            Depth3DDebug.warn(
                String(
                    format: "Adaptive depth cap %.1f fps (Core ML infer %.0f ms). Re-export for ANE or use DA2 Small.",
                    adaptiveInferenceFPSCap ?? sustainableFPS,
                    engineTimings.inferenceDuration * 1_000
                ),
                phase: "pipeline-adaptive"
            )
        }
    }

    private func updateTemporalSmoothingFactor(_ factor: Float) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        temporalSmoothingFactor = DA3VisionProDemoTuning.validatedTemporalSmoothingFactor(factor)
        metrics = makeMetrics(state: isEnabled ? .active : metrics.state)
        return metrics
    }

    private func updateDebugRenderMode(_ mode: DA3DebugRenderMode) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        debugRenderMode = mode
        if !mode.capturesDepth {
            latestDepthMap = nil
            latestDepthMediaTime = nil
        }
        metrics = makeMetrics(state: isEnabled ? .idle : .disabled)
        return metrics
    }

    private func updateRenderMetrics(_ renderMetrics: Video2DTo3DRenderMetrics) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        lastRenderDepthTextureUploadDuration = renderMetrics.depthTextureUploadDuration
        lastRenderDepthSmoothingDuration = renderMetrics.depthSmoothingDuration
        lastRenderDuration = renderMetrics.renderDuration
        lastRenderStereoPassCount = renderMetrics.stereoPassCount
        metrics = makeMetrics(state: isEnabled ? .active : metrics.state)
        return metrics
    }

    private func updatePreparedDiagnostics(_ diagnostics: DA3ModelDiagnostics) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        latestModelDiagnostics = diagnostics
        metrics = makeMetrics(state: isEnabled ? .idle : metrics.state)
        return metrics
    }

    private func currentMetrics() -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        return metrics
    }

    private func makeDepthMap(from depthFrame: DA3DepthFrame) -> (VideoDepthMap?, DepthValueSummary) {
        let processing = currentDepthProcessing()
        var normalizedValues = VideoDepthPostprocessor.normalizedValues(
            depthFrame.values,
            normalization: .minMax,
            invertDepth: processing.isDepthInverted
        )
        normalizedValues = Self.adjustDepthContrast(normalizedValues, contrast: processing.depthContrast)
        var metadata = depthFrame.metadata
        metadata["normalization"] = "minMax"
        metadata["depthContrast"] = String(format: "%.2f", Double(processing.depthContrast))
        metadata["invertDepth"] = processing.isDepthInverted ? "true" : "false"
        let depthMap = VideoDepthMap(
            width: depthFrame.width,
            height: depthFrame.height,
            normalizedDisparity: normalizedValues,
            metadata: metadata
        )
        return (
            depthMap,
            DepthValueSummary(
                rawMinimum: Self.finiteRange(depthFrame.values)?.minimum,
                rawMaximum: Self.finiteRange(depthFrame.values)?.maximum,
                normalizedMinimum: Self.finiteRange(normalizedValues)?.minimum,
                normalizedMaximum: Self.finiteRange(normalizedValues)?.maximum
            )
        )
    }

    private func currentDepthProcessing() -> (depthContrast: Float, isDepthInverted: Bool) {
        lock.lock()
        defer {
            lock.unlock()
        }
        return (depthContrast, isDepthInverted)
    }

    private static func pixelFormatDescription(_ pixelFormat: OSType) -> String {
        let bytes = [
            UInt8((pixelFormat >> 24) & 0xff),
            UInt8((pixelFormat >> 16) & 0xff),
            UInt8((pixelFormat >> 8) & 0xff),
            UInt8(pixelFormat & 0xff),
        ]
        let code = String(bytes: bytes, encoding: .macOSRoman) ?? "\(pixelFormat)"
        return "\(code)(0x\(String(pixelFormat, radix: 16)))"
    }

    private static func formatDuration(_ duration: TimeInterval?) -> String {
        guard let duration else {
            return "--"
        }
        return String(format: "%.1fms", duration * 1_000)
    }

    private static func adjustDepthContrast(_ values: [Float], contrast: Float) -> [Float] {
        let safeContrast = DA3VisionProDemoTuning.validatedDepthContrast(contrast)
        guard abs(safeContrast - 1) > 0.001 else {
            return values
        }
        return values.map { value in
            min(max(((value - 0.5) * safeContrast) + 0.5, 0), 1)
        }
    }

    private static func finiteRange(_ values: [Float]) -> (minimum: Float, maximum: Float)? {
        let finiteValues = values.filter(\.isFinite)
        guard let minimum = finiteValues.min(), let maximum = finiteValues.max() else {
            return nil
        }
        return (minimum, maximum)
    }

    private func currentTemporalSmoothingFactor() -> Float {
        lock.lock()
        defer {
            lock.unlock()
        }
        return temporalSmoothingFactor
    }

    private func recordSuccess(
        depthWidth: Int,
        depthHeight: Int,
        depthMap: VideoDepthMap?,
        depthSummary: DepthValueSummary,
        engineTimings: DA3EngineTimings,
        modelDiagnostics: DA3ModelDiagnostics,
        textureUploadDuration: TimeInterval,
        depthMapDuration: TimeInterval,
        rendererUpdateDuration: TimeInterval,
        mediaTime: TimeInterval?,
        duration: TimeInterval
    ) -> DA3PipelineMetrics? {
        lock.lock()
        defer {
            lock.unlock()
        }
        guard isEnabled else {
            latestDepthMap = nil
            return nil
        }
        let now = Date().timeIntervalSinceReferenceDate
        if let lastSuccessTime {
            let interval = now - lastSuccessTime
            if interval > 0 {
                let instantaneousFPS = 1 / interval
                processingFPS = processingFPS > 0 ? (processingFPS * 0.8) + (instantaneousFPS * 0.2) : instantaneousFPS
            }
        }
        lastSuccessTime = now
        processedFrameCount += 1
        latestDepthMap = depthMap.map { smoothedDepthMap($0) } ?? latestDepthMap
        latestDepthMediaTime = mediaTime
        latestModelDiagnostics = modelDiagnostics
        lastPreprocessingDuration = engineTimings.preprocessingDuration
        lastInferenceDuration = engineTimings.inferenceDuration
        lastOutputExtractionDuration = engineTimings.outputExtractionDuration
        updateAdaptiveInferenceCapLocked(engineTimings: engineTimings)
        lastTextureUploadDuration = textureUploadDuration
        lastDepthMapDuration = depthMapDuration
        lastRendererUpdateDuration = rendererUpdateDuration
        metrics = makeMetrics(
            state: .active,
            lastFrameDuration: duration,
            preserveTransientTiming: false,
            depthWidth: depthWidth,
            depthHeight: depthHeight,
            rawDepthMinimum: depthSummary.rawMinimum,
            rawDepthMaximum: depthSummary.rawMaximum,
            normalizedDepthMinimum: depthSummary.normalizedMinimum,
            normalizedDepthMaximum: depthSummary.normalizedMaximum
        )
        return metrics
    }

    private func recordFailure(_ error: Error) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        latestDepthMap = nil
        latestDepthMediaTime = nil
        metrics = makeMetrics(state: .failed(error.localizedDescription))
        return metrics
    }

    private func updateDepthFrameOffset(for presentationTime: CMTime) {
        guard let latestDepthMap,
              let latestDepthMediaTime,
              let requestTime = seconds(for: presentationTime)
        else {
            return
        }
        let offset = requestTime - latestDepthMediaTime
        if abs(offset) > staleDepthFrameOffset {
            staleDepthFrameCount += 1
        }
        metrics = makeMetrics(
            state: latestDepthMap.normalizedDisparity.isEmpty ? .idle : .active,
            depthFrameOffset: offset
        )
    }

    private func smoothedDepthMap(_ depthMap: VideoDepthMap?) -> VideoDepthMap? {
        guard let depthMap else {
            return nil
        }
        return VideoDepthMap(
            width: depthMap.width,
            height: depthMap.height,
            normalizedDisparity: depthMap.normalizedDisparity,
            confidence: depthMap.confidence,
            metadata: depthMap.metadata.merging(["smoothing": String(format: "metal:%.2f", Double(temporalSmoothingFactor))]) { current, _ in current }
        )
    }

    private func seconds(for time: CMTime) -> TimeInterval? {
        let seconds = CMTimeGetSeconds(time)
        return seconds.isFinite ? seconds : nil
    }

    private func makeMetrics(
        state: DA3ProcessingState,
        lastFrameDuration: TimeInterval? = nil,
        depthFrameOffset: TimeInterval? = nil,
        preserveTransientTiming: Bool = true,
        depthWidth: Int? = nil,
        depthHeight: Int? = nil,
        rawDepthMinimum: Float? = nil,
        rawDepthMaximum: Float? = nil,
        normalizedDepthMinimum: Float? = nil,
        normalizedDepthMaximum: Float? = nil
    ) -> DA3PipelineMetrics {
        DA3PipelineMetrics(
            state: state,
            processedFrameCount: processedFrameCount,
            droppedFrameCount: droppedFrameCount,
            coalescedFrameCount: coalescedFrameCount,
            throttledFrameCount: throttledFrameCount,
            staleDepthFrameCount: staleDepthFrameCount,
            processingFPS: processingFPS,
            lastFrameDuration: lastFrameDuration ?? (preserveTransientTiming ? metrics.lastFrameDuration : nil),
            depthFrameOffset: depthFrameOffset ?? (preserveTransientTiming ? metrics.depthFrameOffset : nil),
            depthWidth: depthWidth ?? metrics.depthWidth,
            depthHeight: depthHeight ?? metrics.depthHeight,
            rawDepthMinimum: rawDepthMinimum ?? (preserveTransientTiming ? metrics.rawDepthMinimum : nil),
            rawDepthMaximum: rawDepthMaximum ?? (preserveTransientTiming ? metrics.rawDepthMaximum : nil),
            normalizedDepthMinimum: normalizedDepthMinimum ?? (preserveTransientTiming ? metrics.normalizedDepthMinimum : nil),
            normalizedDepthMaximum: normalizedDepthMaximum ?? (preserveTransientTiming ? metrics.normalizedDepthMaximum : nil),
            maximumInferenceFPS: effectiveInferenceFPSLocked(),
            sourceVideoFrameRate: sourceVideoFrameRate ?? 0,
            actualInferenceFPS: processingFPS,
            temporalSmoothingFactor: temporalSmoothingFactor,
            preprocessingDuration: preserveTransientTiming ? lastPreprocessingDuration : nil,
            inferenceDuration: preserveTransientTiming ? lastInferenceDuration : nil,
            outputExtractionDuration: preserveTransientTiming ? lastOutputExtractionDuration : nil,
            textureUploadDuration: preserveTransientTiming ? lastTextureUploadDuration : nil,
            depthMapDuration: preserveTransientTiming ? lastDepthMapDuration : nil,
            rendererUpdateDuration: preserveTransientTiming ? lastRendererUpdateDuration : nil,
            renderDepthTextureUploadDuration: preserveTransientTiming ? lastRenderDepthTextureUploadDuration : nil,
            renderDepthSmoothingDuration: preserveTransientTiming ? lastRenderDepthSmoothingDuration : nil,
            renderDuration: preserveTransientTiming ? lastRenderDuration : nil,
            renderStereoPassCount: lastRenderStereoPassCount,
            performanceMode: performanceMode,
            debugRenderMode: debugRenderMode,
            modelDiagnostics: latestModelDiagnostics,
            depthContrast: depthContrast,
            isDepthInverted: isDepthInverted
        )
    }
}
#endif

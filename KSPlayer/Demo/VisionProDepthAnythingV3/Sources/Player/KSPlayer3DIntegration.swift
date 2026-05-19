#if os(visionOS) && canImport(CoreML) && canImport(Metal)
import Combine
@preconcurrency import CoreMedia
@preconcurrency import CoreML
@preconcurrency import CoreVideo
import Foundation
import KSPlayer
@preconcurrency import Metal

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

public struct DA3PipelineMetrics: Equatable, Sendable {
    public let state: DA3ProcessingState
    public let processedFrameCount: Int
    public let droppedFrameCount: Int
    public let throttledFrameCount: Int
    public let staleDepthFrameCount: Int
    public let processingFPS: Double
    public let lastFrameDuration: TimeInterval?
    public let depthFrameOffset: TimeInterval?
    public let depthWidth: Int
    public let depthHeight: Int
    public let maximumInferenceFPS: Double
    public let temporalSmoothingFactor: Float

    public static let empty = DA3PipelineMetrics(
        state: .disabled,
        processedFrameCount: 0,
        droppedFrameCount: 0,
        throttledFrameCount: 0,
        staleDepthFrameCount: 0,
        processingFPS: 0,
        lastFrameDuration: nil,
        depthFrameOffset: nil,
        depthWidth: 0,
        depthHeight: 0,
        maximumInferenceFPS: 0,
        temporalSmoothingFactor: 0
    )
}

private enum DA3VisionProDemoTuning {
    static let maximumInferenceFPS = 4.0
    static let temporalSmoothingFactor: Float = 0.86
    static let staleDepthFrameOffset: TimeInterval = 0.55
    static let defaultDepthStrength: Float = 0.18
    static let defaultDepthDistance: Float = 0.65
    static let defaultDepthCurvature: Float = 0.9
    static let depthStrengthRange: ClosedRange<Float> = 0 ... 0.32
    static let depthDistanceRange: ClosedRange<Float> = 0.35 ... 1.0
    static let depthCurvatureRange: ClosedRange<Float> = 0.6 ... 1.35

    static func validatedDepthStrength(_ value: Float) -> Float {
        validated(value, range: depthStrengthRange, fallback: defaultDepthStrength)
    }

    static func validatedDepthDistance(_ value: Float) -> Float {
        validated(value, range: depthDistanceRange, fallback: defaultDepthDistance)
    }

    static func validatedDepthCurvature(_ value: Float) -> Float {
        validated(value, range: depthCurvatureRange, fallback: defaultDepthCurvature)
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
    @Published public private(set) var playerState = KSPlayerState.initialized
    @Published public private(set) var is2DTo3DEnabled: Bool
    @Published public private(set) var isApplying2DTo3DMode = false
    @Published public private(set) var isLoopEnabled: Bool
    @Published public private(set) var depthStrength: Float
    @Published public private(set) var depthDistance: Float
    @Published public private(set) var depthCurvature: Float
    @Published public private(set) var metrics = DA3PipelineMetrics.empty
    @Published public private(set) var lastAppliedMode = "disabled"

    @Published public private(set) var url: URL
    public let options: KSOptions
    public let coordinator: KSVideoPlayer.Coordinator
    public let renderer: any StereoRendererProtocol
    public var depthStrengthRange: ClosedRange<Float> { DA3VisionProDemoTuning.depthStrengthRange }
    public var depthDistanceRange: ClosedRange<Float> { DA3VisionProDemoTuning.depthDistanceRange }
    public var depthCurvatureRange: ClosedRange<Float> { DA3VisionProDemoTuning.depthCurvatureRange }

    private let pipeline: DA3VideoOutputPipeline
    private let depthMapProvider: DA3CachedDepthMapProvider
    private weak var installedLayer: KSPlayerLayer?

    public convenience init(
        url: URL,
        options: KSOptions = KSOptions(),
        modelConfiguration: MLModelConfiguration = MLModelConfiguration()
    ) throws {
        guard let device = MTLCreateSystemDefaultDevice() else {
            throw KSPlayer3DIntegrationError.metalDeviceUnavailable
        }
        let engine = try DepthAnythingV3Engine(configuration: modelConfiguration)
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
        self.coordinator = KSVideoPlayer.Coordinator()
        self.renderer = renderer
        options.video2DTo3DDepthStrength = DA3VisionProDemoTuning.validatedDepthStrength(options.video2DTo3DDepthStrength)
        options.video2DTo3DDepthDistance = DA3VisionProDemoTuning.validatedDepthDistance(options.video2DTo3DDepthDistance)
        options.video2DTo3DDepthCurvature = DA3VisionProDemoTuning.validatedDepthCurvature(options.video2DTo3DDepthCurvature)
        self.is2DTo3DEnabled = options.video2DTo3DMode.isEnabled
        self.isLoopEnabled = options.isLoopPlay
        self.depthStrength = options.video2DTo3DDepthStrength
        self.depthDistance = options.video2DTo3DDepthDistance
        self.depthCurvature = options.video2DTo3DDepthCurvature
        self.lastAppliedMode = options.video2DTo3DMode.isEnabled ? "depthMapPreferred" : "disabled"
        let pipeline = DA3VideoOutputPipeline(
            engine: engine,
            bridge: DA3DepthMetalBridge(device: device),
            renderer: renderer,
            maximumInferenceFPS: DA3VisionProDemoTuning.maximumInferenceFPS,
            temporalSmoothingFactor: DA3VisionProDemoTuning.temporalSmoothingFactor,
            staleDepthFrameOffset: DA3VisionProDemoTuning.staleDepthFrameOffset
        )
        self.pipeline = pipeline
        let depthMapProvider = DA3CachedDepthMapProvider(pipeline: pipeline)
        self.depthMapProvider = depthMapProvider
        options.videoDepthEstimationProvider = is2DTo3DEnabled ? depthMapProvider : nil
        pipeline.setEnabled(is2DTo3DEnabled)
        pipeline.setErrorHandler { [weak self] error in
            self?.lastErrorMessage = error.localizedDescription
        }
        pipeline.setMetricsHandler { [weak self] metrics in
            self?.metrics = metrics
        }
    }

    public func handleStateChanged(layer: KSPlayerLayer, state: KSPlayerState) {
        playerState = state
        if state == .playedToTheEnd {
            lastActionMessage = "Playback finished - press Play to restart"
        } else {
            lastActionMessage = "Player state: \(state.description)"
        }
        configureVideoOutput(on: layer, isEnabled: is2DTo3DEnabled)
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
            lastActionMessage = "Play unavailable"
            return
        }
        lastErrorMessage = nil
        lastActionMessage = playerState == .playedToTheEnd ? "Restarting from beginning" : "Play tapped"
        layer.play()
    }

    public func restart() {
        guard let layer = installedLayer ?? coordinator.playerLayer else {
            lastErrorMessage = "Player is not ready yet. Wait for the video surface to load, then try Restart again."
            lastActionMessage = "Restart unavailable"
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
            lastActionMessage = "Pause unavailable"
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

    public func set2DTo3DEnabled(_ isEnabled: Bool) {
        guard is2DTo3DEnabled != isEnabled else {
            return
        }
        isApplying2DTo3DMode = true
        defer {
            isApplying2DTo3DMode = false
        }
        lastErrorMessage = nil
        let shouldRestart = playerState == .playedToTheEnd
        if shouldRestart {
            lastActionMessage = isEnabled ? "2D-to-3D enabled - restarting" : "2D-to-3D disabled - restarting"
        } else {
            lastActionMessage = isEnabled ? "2D-to-3D enabled" : "2D-to-3D disabled"
        }
        is2DTo3DEnabled = isEnabled
        options.video2DTo3DMode = isEnabled ? .depthMapPreferred : .disabled
        options.videoDepthEstimationProvider = isEnabled ? depthMapProvider : nil
        lastAppliedMode = isEnabled ? "depthMapPreferred" : "disabled"
        pipeline.reset()
        pipeline.setEnabled(isEnabled)
        renderer.reset()
        if let layer = installedLayer ?? coordinator.playerLayer {
            configureVideoOutput(on: layer, isEnabled: isEnabled, force: true)
        }
        reloadPlayerPreservingPlaybackTime(restartIfEnded: true)
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

    public func setDepthCurvature(_ value: Float) {
        let validatedValue = DA3VisionProDemoTuning.validatedDepthCurvature(value)
        guard depthCurvature != validatedValue else {
            return
        }
        depthCurvature = validatedValue
        options.video2DTo3DDepthCurvature = validatedValue
        lastActionMessage = String(format: "Depth curvature %.2f", Double(validatedValue))
    }

    public func stop() {
        installedLayer?.videoOutput = nil
        installedLayer = nil
        isApplying2DTo3DMode = false
        pipeline.reset()
        renderer.reset()
        lastActionMessage = "Player stopped"
    }

    private func configureVideoOutput(on layer: KSPlayerLayer, isEnabled: Bool, force: Bool = false) {
        if installedLayer !== layer {
            installedLayer?.videoOutput = nil
            installedLayer = layer
        } else if !force {
            return
        }

        guard isEnabled else {
            layer.videoOutput = nil
            return
        }

        layer.videoOutputConfiguration = KSVideoFrameOutput.Configuration(
            maximumBufferedFrameCount: 1,
            dropPolicy: .keepLatest,
            callbackQualityOfService: .userInitiated,
            callbackQueueLabel: "DepthAnythingV3.videoOutput"
        )
        layer.videoOutput = { [pipeline] pixelBuffer in
            pipeline.enqueue(pixelBuffer)
        }
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

private final class DA3VideoOutputPipeline: @unchecked Sendable {
    private final class RetainedPixelBuffer: @unchecked Sendable {
        let pixelBuffer: CVPixelBuffer
        let mediaTime: TimeInterval?

        init(_ pixelBuffer: CVPixelBuffer) {
            self.pixelBuffer = pixelBuffer
            self.mediaTime = Self.mediaTime(from: pixelBuffer)
        }

        private static func mediaTime(from pixelBuffer: CVPixelBuffer) -> TimeInterval? {
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
    private let bridge: DA3DepthMetalBridge
    private weak var renderer: (any StereoRendererProtocol)?
    private let maximumInferenceFPS: Double
    private let temporalSmoothingFactor: Float
    private let staleDepthFrameOffset: TimeInterval
    private var onError: @MainActor @Sendable (Error) -> Void = { _ in }
    private var onMetrics: @MainActor @Sendable (DA3PipelineMetrics) -> Void = { _ in }
    private let lock = NSLock()
    private var isEnabled = false
    private var isProcessing = false
    private var processedFrameCount = 0
    private var droppedFrameCount = 0
    private var throttledFrameCount = 0
    private var staleDepthFrameCount = 0
    private var processingFPS = 0.0
    private var lastSuccessTime: TimeInterval?
    private var lastAcceptedTime: TimeInterval?
    private var latestDepthMediaTime: TimeInterval?
    private var latestDepthMap: VideoDepthMap?
    private var metrics = DA3PipelineMetrics.empty

    init(
        engine: DepthAnythingV3Engine,
        bridge: DA3DepthMetalBridge,
        renderer: any StereoRendererProtocol,
        maximumInferenceFPS: Double,
        temporalSmoothingFactor: Float,
        staleDepthFrameOffset: TimeInterval
    ) {
        self.engine = engine
        self.bridge = bridge
        self.renderer = renderer
        self.maximumInferenceFPS = maximumInferenceFPS
        self.temporalSmoothingFactor = temporalSmoothingFactor
        self.staleDepthFrameOffset = staleDepthFrameOffset
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

    func setEnabled(_ isEnabled: Bool) {
        let metrics = updateEnabledState(isEnabled)
        Task { @MainActor in
            self.onMetrics(metrics)
        }
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
        switch beginProcessing() {
        case .disabled:
            return
        case let .dropped(metrics):
            Task { @MainActor in
                self.onMetrics(metrics)
            }
            return
        case let .throttled(metrics):
            Task { @MainActor in
                self.onMetrics(metrics)
            }
            return
        case let .started(metrics):
            Task { @MainActor in
                self.onMetrics(metrics)
            }
        }

        let retainedFrame = RetainedPixelBuffer(pixelBuffer)
        let startTime = Date().timeIntervalSinceReferenceDate
        Task { [weak self, retainedFrame] in
            guard let self else {
                return
            }
            defer {
                self.finishProcessing()
            }

            do {
                let depthFrame = try await self.engine.makeDepthFrame(from: retainedFrame.pixelBuffer)
                let depthTexture = try self.bridge.makeTexture(from: depthFrame)
                let depthMap = depthFrame.normalizedDisparityMap()
                guard let metrics = self.recordSuccess(
                    depthFrame: depthFrame,
                    depthMap: depthMap,
                    mediaTime: retainedFrame.mediaTime,
                    duration: Date().timeIntervalSinceReferenceDate - startTime
                ) else {
                    return
                }
                await MainActor.run {
                    self.renderer?.update(
                        sourceFrame: retainedFrame.pixelBuffer,
                        depthFrame: depthFrame,
                        depthTexture: depthTexture
                    )
                    self.onMetrics(metrics)
                }
            } catch is CancellationError {
                return
            } catch {
                let metrics = self.recordFailure(error)
                await MainActor.run {
                    self.onError(error)
                    self.onMetrics(metrics)
                }
            }
        }
    }

    func reset() {
        lock.lock()
        isProcessing = false
        processedFrameCount = 0
        droppedFrameCount = 0
        throttledFrameCount = 0
        staleDepthFrameCount = 0
        processingFPS = 0
        lastSuccessTime = nil
        lastAcceptedTime = nil
        latestDepthMediaTime = nil
        latestDepthMap = nil
        metrics = makeMetrics(state: .disabled, preserveTransientTiming: false, depthWidth: 0, depthHeight: 0)
        lock.unlock()
    }

    private enum ProcessingStart {
        case disabled
        case dropped(DA3PipelineMetrics)
        case throttled(DA3PipelineMetrics)
        case started(DA3PipelineMetrics)
    }

    private func beginProcessing() -> ProcessingStart {
        let now = Date().timeIntervalSinceReferenceDate
        lock.lock()
        defer {
            lock.unlock()
        }
        guard isEnabled else {
            return .disabled
        }
        if let lastAcceptedTime, now - lastAcceptedTime < 1 / maximumInferenceFPS {
            throttledFrameCount += 1
            metrics = makeMetrics(state: latestDepthMap == nil ? .idle : .active)
            return .throttled(metrics)
        }
        guard !isProcessing else {
            droppedFrameCount += 1
            metrics = makeMetrics(state: .processing)
            return .dropped(metrics)
        }
        lastAcceptedTime = now
        isProcessing = true
        metrics = makeMetrics(state: .processing)
        return .started(metrics)
    }

    private func finishProcessing() {
        lock.lock()
        isProcessing = false
        lock.unlock()
    }

    private func updateEnabledState(_ enabled: Bool) -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        isEnabled = enabled
        if !enabled {
            isProcessing = false
            latestDepthMap = nil
            latestDepthMediaTime = nil
            processingFPS = 0
            lastSuccessTime = nil
            lastAcceptedTime = nil
        }
        metrics = makeMetrics(
            state: enabled ? .idle : .disabled,
            preserveTransientTiming: false,
            depthWidth: enabled ? metrics.depthWidth : 0,
            depthHeight: enabled ? metrics.depthHeight : 0
        )
        return metrics
    }

    private func currentMetrics() -> DA3PipelineMetrics {
        lock.lock()
        defer {
            lock.unlock()
        }
        return metrics
    }

    private func recordSuccess(
        depthFrame: DA3DepthFrame,
        depthMap: VideoDepthMap?,
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
        latestDepthMap = smoothedDepthMap(depthMap)
        latestDepthMediaTime = mediaTime
        metrics = makeMetrics(
            state: .active,
            lastFrameDuration: duration,
            preserveTransientTiming: false,
            depthWidth: depthFrame.width,
            depthHeight: depthFrame.height
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
        guard let previous = latestDepthMap,
              previous.width == depthMap.width,
              previous.height == depthMap.height
        else {
            return depthMap
        }
        let values = VideoDepthPostprocessor.smoothedValues(
            current: depthMap.normalizedDisparity,
            previous: previous.normalizedDisparity,
            factor: temporalSmoothingFactor
        )
        return VideoDepthMap(
            width: depthMap.width,
            height: depthMap.height,
            normalizedDisparity: values,
            confidence: depthMap.confidence,
            metadata: depthMap.metadata.merging(["smoothing": String(format: "%.2f", Double(temporalSmoothingFactor))]) { current, _ in current }
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
        depthHeight: Int? = nil
    ) -> DA3PipelineMetrics {
        DA3PipelineMetrics(
            state: state,
            processedFrameCount: processedFrameCount,
            droppedFrameCount: droppedFrameCount,
            throttledFrameCount: throttledFrameCount,
            staleDepthFrameCount: staleDepthFrameCount,
            processingFPS: processingFPS,
            lastFrameDuration: lastFrameDuration ?? (preserveTransientTiming ? metrics.lastFrameDuration : nil),
            depthFrameOffset: depthFrameOffset ?? (preserveTransientTiming ? metrics.depthFrameOffset : nil),
            depthWidth: depthWidth ?? metrics.depthWidth,
            depthHeight: depthHeight ?? metrics.depthHeight,
            maximumInferenceFPS: maximumInferenceFPS,
            temporalSmoothingFactor: temporalSmoothingFactor
        )
    }
}
#endif

#if os(visionOS) && canImport(Metal)
import CompositorServices
import CoreMedia
import CoreVideo
import KSPlayer
@preconcurrency import Metal
import os

public enum ImmersiveStereoSpace {
    public static let id = "KSPlayer.DA3.ImmersiveStereo"
}

/// CompositorLayer `LayerRenderer.device` — DA3 depth textures must be allocated on this GPU.
public enum ImmersiveStereoMetalDeviceRegistry: Sendable {
    private static let registeredDevice = OSAllocatedUnfairLock<(any MTLDevice)?>(initialState: nil)

    public static var compositorDevice: (any MTLDevice)? {
        registeredDevice.withLock { $0 }
    }

    public static func registerCompositorDevice(_ device: any MTLDevice) {
        registeredDevice.withLock { $0 = device }
    }

    public static func clearCompositorDevice() {
        registeredDevice.withLock { $0 = nil }
    }
}

/// DEBUG-friendly presentation toggles (environment variables, no UI required).
public enum ImmersiveStereoDebugPresentation: Sendable {
    /// Default mixed immersion in DEBUG so passthrough reveals whether the compositor draws.
    public static var prefersMixedImmersion: Bool {
        #if DEBUG
        ProcessInfo.processInfo.environment["DA3_IMMERSIVE_FULL"] != "1"
        #else
        false
        #endif
    }

    /// Set `DA3_IMMERSIVE_MAGENTA=1` in the Xcode scheme to force solid magenta in immersive.
    public static var enablesMagentaSolid: Bool {
        #if DEBUG
        ProcessInfo.processInfo.environment["DA3_IMMERSIVE_MAGENTA"] == "1"
        #else
        false
        #endif
    }
}

/// Set true only after the user successfully opens immersive stereo from the window UI.
public enum ImmersiveStereoSession: Sendable {
    private static let userRequestedActive = OSAllocatedUnfairLock(initialState: false)
    private static let presentationEpochState = OSAllocatedUnfairLock(initialState: 0)

    public static var isUserRequestedActive: Bool {
        userRequestedActive.withLock { $0 }
    }

    /// Bumped when immersive opens so the compositor drops stale frames.
    public static var presentationEpoch: Int {
        presentationEpochState.withLock { $0 }
    }

    public static func setUserRequestedActive(_ isActive: Bool) {
        userRequestedActive.withLock { value in
            let wasActive = value
            value = isActive
            if isActive, !wasActive {
                presentationEpochState.withLock { $0 += 1 }
            }
        }
    }
}

/// Head-locked cinema screen distance (meters). Scale is compensated so apparent size stays constant.
public enum ImmersiveScreenPlacement: Sendable {
    /// Reference distance used when tuning contain-fit scale.
    public static let referenceDistanceMeters: Float = 1.4
    public static let defaultDistanceMeters: Float = 4.2
    public static let distanceRangeMeters: ClosedRange<Float> = 1.0 ... 100.0

    private static let distanceMeters = OSAllocatedUnfairLock(initialState: defaultDistanceMeters)

    public static var currentDistanceMeters: Float {
        distanceMeters.withLock { $0 }
    }

    public static func setDistanceMeters(_ value: Float) {
        distanceMeters.withLock { $0 = validatedDistance(value) }
    }

    public static func validatedDistance(_ value: Float) -> Float {
        guard value.isFinite else {
            return defaultDistanceMeters
        }
        return min(max(value, distanceRangeMeters.lowerBound), distanceRangeMeters.upperBound)
    }

    /// Env `DA3_IMMERSIVE_SCREEN_METERS` overrides the UI slider when set.
    public static func resolvedScreenDistanceMeters() -> Float {
        if let raw = ProcessInfo.processInfo.environment["DA3_IMMERSIVE_SCREEN_METERS"],
           let meters = Float(raw),
           meters.isFinite,
           meters > 0.5
        {
            return meters
        }
        return currentDistanceMeters
    }

    /// Multiply screen scale by this to preserve angular size when distance changes.
    public static func sizeCompensation(for distanceMeters: Float) -> Float {
        distanceMeters / referenceDistanceMeters
    }
}

public struct ImmersiveStereoFrame: @unchecked Sendable {
    public let pixelBuffer: CVPixelBuffer
    public let depthTexture: (any MTLTexture)?
    public let configuration: Video2DTo3DRenderConfiguration
    public let videoMediaTime: TimeInterval?
    public let depthMediaTime: TimeInterval?

    public init(
        pixelBuffer: CVPixelBuffer,
        depthTexture: (any MTLTexture)?,
        configuration: Video2DTo3DRenderConfiguration,
        videoMediaTime: TimeInterval? = nil,
        depthMediaTime: TimeInterval? = nil
    ) {
        self.pixelBuffer = pixelBuffer
        self.depthTexture = depthTexture
        self.configuration = configuration
        self.videoMediaTime = videoMediaTime
        self.depthMediaTime = depthMediaTime
    }

    public var isDepthStaleForPresentation: Bool {
        guard let videoMediaTime, let depthMediaTime else {
            return depthTexture == nil
        }
        return videoMediaTime - depthMediaTime > 0.45
    }
}

/// Time-ordered cache of decoded color + depth for display-time selection (not "latest only").
public final class ImmersiveStereoPresentationBuffer: @unchecked Sendable {
    private struct RingEntry {
        let pixelBuffer: CVPixelBuffer
        var depthTexture: (any MTLTexture)?
        var depthMediaTime: TimeInterval?
        var configuration: Video2DTo3DRenderConfiguration
        let presentationSeconds: TimeInterval
    }

    private let lock = NSLock()
    private var entries: [RingEntry] = []
    private var lastDepthTexture: (any MTLTexture)?
    private var lastDepthMediaTime: TimeInterval?
    private let maxEntryCount: Int
    private let depthAttachTolerance: TimeInterval

    public init(maxEntryCount: Int = 24, depthAttachTolerance: TimeInterval = 0.2) {
        self.maxEntryCount = max(2, maxEntryCount)
        self.depthAttachTolerance = depthAttachTolerance
    }

    public func appendVideo(
        pixelBuffer: CVPixelBuffer,
        mediaTime: TimeInterval?,
        configuration: Video2DTo3DRenderConfiguration
    ) {
        lock.lock()
        defer {
            lock.unlock()
        }
        let seconds = mediaTime ?? nextSyntheticSeconds()
        if let last = entries.last?.presentationSeconds, seconds + 0.001 < last {
            // Seek backward: drop future PTS so immersive video is not stuck on one pre-seek frame.
            if seconds < last - 0.25 {
                entries.removeAll()
                lastDepthTexture = nil
                lastDepthMediaTime = nil
            } else {
                return
            }
        }
        let carriedDepth: (any MTLTexture)?
        let carriedDepthTime: TimeInterval?
        if let lastDepthTexture, let lastDepthMediaTime, seconds - lastDepthMediaTime <= 0.45 {
            carriedDepth = lastDepthTexture
            carriedDepthTime = lastDepthMediaTime
        } else {
            carriedDepth = nil
            carriedDepthTime = nil
        }
        entries.append(
            RingEntry(
                pixelBuffer: pixelBuffer,
                depthTexture: carriedDepth,
                depthMediaTime: carriedDepthTime,
                configuration: configuration,
                presentationSeconds: seconds
            )
        )
        trimEntries()
    }

    public func attachDepth(_ depthTexture: any MTLTexture, mediaTime: TimeInterval?) {
        lock.lock()
        defer {
            lock.unlock()
        }
        lastDepthTexture = depthTexture
        let seconds = mediaTime ?? entries.last?.presentationSeconds
        guard let seconds else {
            return
        }
        lastDepthMediaTime = seconds
        if let index = entries.indices.min(by: {
            abs(entries[$0].presentationSeconds - seconds) < abs(entries[$1].presentationSeconds - seconds)
        }),
        abs(entries[index].presentationSeconds - seconds) <= depthAttachTolerance
        {
            entries[index].depthTexture = depthTexture
            entries[index].depthMediaTime = seconds
            return
        }
        if let last = entries.last {
            entries.append(
                RingEntry(
                    pixelBuffer: last.pixelBuffer,
                    depthTexture: depthTexture,
                    depthMediaTime: seconds,
                    configuration: last.configuration,
                    presentationSeconds: seconds
                )
            )
            trimEntries()
        }
    }

    public func updateConfiguration(_ configuration: Video2DTo3DRenderConfiguration) {
        lock.lock()
        if var last = entries.popLast() {
            last.configuration = configuration
            entries.append(last)
        }
        lock.unlock()
    }

    struct PresentationSelection {
        let frame: ImmersiveStereoFrame?
        let selectedMediaSeconds: TimeInterval?
        let deltaMilliseconds: Double?
        let bufferCount: Int
        let bufferMinMediaSeconds: TimeInterval?
        let bufferMaxMediaSeconds: TimeInterval?
        let nearestCandidateMediaSeconds: TimeInterval?
        let nearestDeltaMilliseconds: Double?
    }

    func selectFrame(forMediaSeconds targetSeconds: TimeInterval) -> PresentationSelection {
        lock.lock()
        defer {
            lock.unlock()
        }
        let bufferCount = entries.count
        let bufferMinMediaSeconds = entries.map(\.presentationSeconds).min()
        let bufferMaxMediaSeconds = entries.map(\.presentationSeconds).max()
        let nearest = nearestEntry(forMediaSeconds: targetSeconds)
        let nearestCandidateMediaSeconds = nearest?.presentationSeconds
        let nearestDeltaMilliseconds = nearest.map {
            ($0.presentationSeconds - targetSeconds) * 1000
        }
        guard let entry = selectEntry(forMediaSeconds: targetSeconds) else {
            return PresentationSelection(
                frame: nil,
                selectedMediaSeconds: nil,
                deltaMilliseconds: nil,
                bufferCount: bufferCount,
                bufferMinMediaSeconds: bufferMinMediaSeconds,
                bufferMaxMediaSeconds: bufferMaxMediaSeconds,
                nearestCandidateMediaSeconds: nearestCandidateMediaSeconds,
                nearestDeltaMilliseconds: nearestDeltaMilliseconds
            )
        }
        let deltaMilliseconds = (entry.presentationSeconds - targetSeconds) * 1000
        return PresentationSelection(
            frame: ImmersiveStereoFrame(
                pixelBuffer: entry.pixelBuffer,
                depthTexture: entry.depthTexture,
                configuration: entry.configuration,
                videoMediaTime: entry.presentationSeconds,
                depthMediaTime: entry.depthMediaTime
            ),
            selectedMediaSeconds: entry.presentationSeconds,
            deltaMilliseconds: deltaMilliseconds,
            bufferCount: bufferCount,
            bufferMinMediaSeconds: bufferMinMediaSeconds,
            bufferMaxMediaSeconds: bufferMaxMediaSeconds,
            nearestCandidateMediaSeconds: nearestCandidateMediaSeconds,
            nearestDeltaMilliseconds: nearestDeltaMilliseconds
        )
    }

    /// Select the frame whose media PTS best matches the compositor target (largest PTS not after target).
    public func frame(forMediaSeconds targetSeconds: TimeInterval) -> ImmersiveStereoFrame? {
        selectFrame(forMediaSeconds: targetSeconds).frame
    }

    public var isEmpty: Bool {
        lock.lock()
        let empty = entries.isEmpty
        lock.unlock()
        return empty
    }

    var hasAnyDepthTexture: Bool {
        lock.lock()
        let hasDepth = entries.contains { $0.depthTexture != nil } || lastDepthTexture != nil
        lock.unlock()
        return hasDepth
    }

    public func clear() {
        lock.lock()
        entries.removeAll()
        lastDepthTexture = nil
        lastDepthMediaTime = nil
        lock.unlock()
    }

    private func selectEntry(forMediaSeconds targetSeconds: TimeInterval) -> RingEntry? {
        guard !entries.isEmpty else {
            return nil
        }
        let frameSlack = 1.0 / 60.0
        guard let newest = entries.max(by: { $0.presentationSeconds < $1.presentationSeconds }) else {
            return nil
        }
        // Live playback: prefer the freshest decoded frame when the compositor clock is near or past the buffer.
        if targetSeconds >= newest.presentationSeconds - frameSlack {
            return newest
        }
        if targetSeconds > newest.presentationSeconds + frameSlack {
            return newest
        }
        let notPast = entries.filter { $0.presentationSeconds <= targetSeconds + frameSlack }
        if let best = notPast.max(by: { $0.presentationSeconds < $1.presentationSeconds }) {
            let lagMilliseconds = (targetSeconds - best.presentationSeconds) * 1000
            if lagMilliseconds > 66, newest.presentationSeconds > best.presentationSeconds + frameSlack {
                return newest
            }
            return best
        }
        return nearestEntry(forMediaSeconds: targetSeconds)
    }

    private func nearestEntry(forMediaSeconds targetSeconds: TimeInterval) -> RingEntry? {
        entries.min(by: {
            abs($0.presentationSeconds - targetSeconds) < abs($1.presentationSeconds - targetSeconds)
        })
    }

    private func trimEntries() {
        if entries.count > maxEntryCount {
            entries.removeFirst(entries.count - maxEntryCount)
        }
    }

    private func nextSyntheticSeconds() -> TimeInterval {
        (entries.last?.presentationSeconds ?? 0) + 1.0 / 30.0
    }
}

public final class ImmersiveStereoFrameStore: @unchecked Sendable {
    public struct TemporalSelectionDiagnostics: Equatable, Sendable {
        public let targetMediaSeconds: TimeInterval
        public let selectedMediaSeconds: TimeInterval?
        public let deltaMilliseconds: Double?
        public let bufferCount: Int
        public let bufferMinMediaSeconds: TimeInterval?
        public let bufferMaxMediaSeconds: TimeInterval?
        public let timelineAnchored: Bool
        public let hostInstantSeconds: TimeInterval
        public let nearestCandidateMediaSeconds: TimeInterval?
        public let nearestDeltaMilliseconds: Double?
    }

    public static let shared = ImmersiveStereoFrameStore()

    private let presentationBuffer = ImmersiveStereoPresentationBuffer()
    private var latestVideoSequence: UInt64 = 0
    private let diagnosticsLock = NSLock()
    private var lastSelectionDiagnostics = TemporalSelectionDiagnostics(
        targetMediaSeconds: 0,
        selectedMediaSeconds: nil,
        deltaMilliseconds: nil,
        bufferCount: 0,
        bufferMinMediaSeconds: nil,
        bufferMaxMediaSeconds: nil,
        timelineAnchored: false,
        hostInstantSeconds: 0,
        nearestCandidateMediaSeconds: nil,
        nearestDeltaMilliseconds: nil
    )

    private init() {}

    public var hasRecentDepthTexture: Bool {
        presentationBuffer.hasAnyDepthTexture
    }

    public var hasBufferedVideoFrames: Bool {
        !presentationBuffer.isEmpty
    }

    public func latestTemporalSelectionDiagnostics() -> TemporalSelectionDiagnostics {
        diagnosticsLock.lock()
        defer {
            diagnosticsLock.unlock()
        }
        return lastSelectionDiagnostics
    }

    public func updateVideo(
        pixelBuffer: CVPixelBuffer,
        mediaTime: TimeInterval?,
        configuration: Video2DTo3DRenderConfiguration
    ) {
        _ = appendVideoForImmersive(
            pixelBuffer: pixelBuffer,
            mediaTime: mediaTime,
            configuration: configuration
        )
    }

    /// Returns true when this append is the first video entry after a reset.
    @discardableResult
    public func appendVideoForImmersive(
        pixelBuffer: CVPixelBuffer,
        mediaTime: TimeInterval?,
        configuration: Video2DTo3DRenderConfiguration
    ) -> Bool {
        let isFirst = presentationBuffer.isEmpty
        if mediaTime == nil {
            latestVideoSequence += 1
        }
        presentationBuffer.appendVideo(
            pixelBuffer: pixelBuffer,
            mediaTime: mediaTime,
            configuration: configuration
        )
        return isFirst
    }

    public func updateConfiguration(_ configuration: Video2DTo3DRenderConfiguration) {
        presentationBuffer.updateConfiguration(configuration)
    }

    public func updateDepth(_ depthTexture: any MTLTexture, mediaTime: TimeInterval?) {
        presentationBuffer.attachDepth(depthTexture, mediaTime: mediaTime)
    }

    public func update(_ frame: ImmersiveStereoFrame) {
        updateVideo(
            pixelBuffer: frame.pixelBuffer,
            mediaTime: frame.videoMediaTime,
            configuration: frame.configuration
        )
        if let depthTexture = frame.depthTexture {
            updateDepth(depthTexture, mediaTime: frame.depthMediaTime)
        }
    }

    /// Display-time selection using CompositorLayer presentation instant.
    public func frame(forPresentationTime instant: LayerRenderer.Clock.Instant) -> ImmersiveStereoFrame? {
        let hostInstantSeconds = instant.immersiveHostTimeIntervalSinceEpoch
        let mediaSeconds = ImmersiveStereoPlaybackTimeline.mediaSeconds(forPresentationTime: instant)
        let selection = presentationBuffer.selectFrame(forMediaSeconds: mediaSeconds)
        let diagnostics = TemporalSelectionDiagnostics(
            targetMediaSeconds: mediaSeconds,
            selectedMediaSeconds: selection.selectedMediaSeconds,
            deltaMilliseconds: selection.deltaMilliseconds,
            bufferCount: selection.bufferCount,
            bufferMinMediaSeconds: selection.bufferMinMediaSeconds,
            bufferMaxMediaSeconds: selection.bufferMaxMediaSeconds,
            timelineAnchored: ImmersiveStereoPlaybackTimeline.isPlaybackAnchored,
            hostInstantSeconds: hostInstantSeconds,
            nearestCandidateMediaSeconds: selection.nearestCandidateMediaSeconds,
            nearestDeltaMilliseconds: selection.nearestDeltaMilliseconds
        )
        diagnosticsLock.lock()
        lastSelectionDiagnostics = diagnostics
        diagnosticsLock.unlock()
        if ImmersiveStereoTemporalDebug.isEnabled {
            ImmersiveStereoTemporalDebug.log(diagnostics)
        }
        return selection.frame
    }

    /// Legacy latest-frame accessor (avoid for compositor draws).
    public func current() -> ImmersiveStereoFrame? {
        presentationBuffer.frame(forMediaSeconds: ImmersiveStereoPlaybackTimeline.mediaSeconds(
            forPresentationTime: LayerRenderer.Clock().now
        ))
    }

    public func clearPresentationBuffer() {
        presentationBuffer.clear()
        latestVideoSequence = 0
    }

    public func reset() {
        clearPresentationBuffer()
        ImmersiveStereoPlaybackTimeline.reset()
    }
}
#endif

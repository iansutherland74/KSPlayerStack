#if os(visionOS) && canImport(CompositorServices)
import CompositorServices
import CoreVideo
import KSPlayer
@preconcurrency import Metal

/// Decoded video ring for immersive compositor — audio-mastered, not decode live-edge.
public final class ImmersiveVideoFeed: @unchecked Sendable {
    public static let shared = ImmersiveVideoFeed()

    public static let maxDecodeLeadSeconds: TimeInterval = 0.35
    public static let maxBufferLagSeconds: TimeInterval = 1.5
    public static let bootstrapToleranceSeconds: TimeInterval = 2.0
    public static let presentationFallbackSeconds: TimeInterval = 0.5
    /// Maximum depth staleness before we drop back to neutral depth (prevents “rubbery” lag on IPTV).
    ///
    /// Override with `DA3_DEPTH_MAX_AGE` (seconds), e.g. `0.25`.
    public static let maxDepthAgeSeconds: TimeInterval = {
        #if DEBUG
        if let raw = ProcessInfo.processInfo.environment["DA3_DEPTH_MAX_AGE"],
           let value = Double(raw),
           value.isFinite,
           value > 0
        {
            return value
        }
        #endif
        return 0.30
    }()

    private struct Entry {
        let pixelBuffer: CVPixelBuffer
        var depthTexture: (any MTLTexture)?
        var depthMediaTime: TimeInterval?
        var configuration: Video2DTo3DRenderConfiguration
        let presentationSeconds: TimeInterval
        let sequence: UInt64
    }

    private var lastDepthTexture: (any MTLTexture)?
    private var lastDepthMediaTime: TimeInterval?
    private let depthAttachTolerance: TimeInterval = 0.2
    private struct DepthEntry {
        let texture: any MTLTexture
        let mediaSeconds: TimeInterval
    }
    private var depthRing: [DepthEntry] = []
    private let maxDepthEntryCount: Int = 6

    private let lock = NSLock()
    private var entries: [Entry] = []
    private var nextSequence: UInt64 = 0
    private var acceptingFrames = false
    private var playbackAnchorSeconds: TimeInterval = 0
    private let maxEntryCount: Int

    public struct Diagnostics: Equatable, Sendable {
        public var isAcceptingFrames: Bool
        public var playbackAnchorSeconds: Double
        public var ringCount: Int
        public var depthRingCount: Int
        public var minPTS: Double?
        public var maxPTS: Double?
        public var lastVideoPTS: Double?
        public var lastDepthPTS: Double?
        public var appendedFrames: Int
        public var rejectedNotAccepting: Int
        public var rejectedOutsideWindow: Int
        public var rejectedOutOfOrder: Int

        public static let idle = Diagnostics(
            isAcceptingFrames: false,
            playbackAnchorSeconds: 0,
            ringCount: 0,
            depthRingCount: 0,
            minPTS: nil,
            maxPTS: nil,
            lastVideoPTS: nil,
            lastDepthPTS: nil,
            appendedFrames: 0,
            rejectedNotAccepting: 0,
            rejectedOutsideWindow: 0,
            rejectedOutOfOrder: 0
        )
    }

    private var appendedFrames: Int = 0
    private var rejectedNotAccepting: Int = 0
    private var rejectedOutsideWindow: Int = 0
    private var rejectedOutOfOrder: Int = 0
    /// Display aspect (width/height) from `MediaPlayerProtocol.naturalSize`, when known.
    private var sourceDisplayAspectRatio: Float?

    public init(maxEntryCount: Int = 32) {
        self.maxEntryCount = max(4, maxEntryCount)
    }

    public var isAcceptingFrames: Bool {
        lock.lock()
        defer { lock.unlock() }
        return acceptingFrames
    }

    public func diagnosticsSnapshot() -> Diagnostics {
        lock.lock()
        defer { lock.unlock() }
        let minPTS = entries.first?.presentationSeconds
        let maxPTS = entries.last?.presentationSeconds
        return Diagnostics(
            isAcceptingFrames: acceptingFrames,
            playbackAnchorSeconds: playbackAnchorSeconds,
            ringCount: entries.count,
            depthRingCount: depthRing.count,
            minPTS: minPTS,
            maxPTS: maxPTS,
            lastVideoPTS: maxPTS,
            lastDepthPTS: lastDepthMediaTime,
            appendedFrames: appendedFrames,
            rejectedNotAccepting: rejectedNotAccepting,
            rejectedOutsideWindow: rejectedOutsideWindow,
            rejectedOutOfOrder: rejectedOutOfOrder
        )
    }

    public var hasFrames: Bool {
        lock.lock()
        defer { lock.unlock() }
        return !entries.isEmpty
    }

    public func setAcceptingFrames(_ accepts: Bool) {
        lock.lock()
        acceptingFrames = accepts
        lock.unlock()
    }

    /// Call when immersive opens or on each timeline sync (audio master from `KSPlayerLayer`).
    public func setPlaybackAnchor(_ seconds: TimeInterval) {
        lock.lock()
        playbackAnchorSeconds = seconds.isFinite ? max(0, seconds) : 0
        lock.unlock()
    }

    /// Re-anchor the feed to decoded video PTS (HLS often uses a different timebase than `currentPlaybackTime`).
    public func reanchorPlaybackClock(toVideoPTS seconds: TimeInterval) {
        guard seconds.isFinite else {
            return
        }
        lock.lock()
        playbackAnchorSeconds = max(0, seconds)
        lock.unlock()
    }

    public var latestVideoPTS: TimeInterval? {
        lock.lock()
        defer { lock.unlock() }
        return entries.last?.presentationSeconds
    }

    public var playbackAnchorSnapshot: TimeInterval {
        lock.lock()
        defer { lock.unlock() }
        return playbackAnchorSeconds
    }

    public func clear() {
        lock.lock()
        entries.removeAll()
        nextSequence = 0
        lastDepthTexture = nil
        lastDepthMediaTime = nil
        depthRing.removeAll()
        sourceDisplayAspectRatio = nil
        appendedFrames = 0
        rejectedNotAccepting = 0
        rejectedOutsideWindow = 0
        rejectedOutOfOrder = 0
        lock.unlock()
    }

    public func setSourceDisplayAspectRatio(_ aspect: Float?) {
        lock.lock()
        if let aspect, aspect.isFinite, aspect > 0 {
            sourceDisplayAspectRatio = aspect
        } else {
            sourceDisplayAspectRatio = nil
        }
        lock.unlock()
    }

    public func resolvedSourceDisplayAspectRatio(fallback: Float) -> Float {
        lock.lock()
        defer { lock.unlock() }
        if let sourceDisplayAspectRatio, sourceDisplayAspectRatio.isFinite, sourceDisplayAspectRatio > 0 {
            return sourceDisplayAspectRatio
        }
        return fallback
    }

    public func pruneEntriesFarFromPlayback() {
        lock.lock()
        let playback = playbackClockSecondsLocked()
        let minSeconds = playback - Self.maxBufferLagSeconds
        let maxSeconds = playback + Self.maxDecodeLeadSeconds + 0.1
        entries.removeAll { $0.presentationSeconds < minSeconds || $0.presentationSeconds > maxSeconds }
        lock.unlock()
    }

    public func updateConfiguration(_ configuration: Video2DTo3DRenderConfiguration) {
        lock.lock()
        if var last = entries.popLast() {
            last.configuration = configuration
            entries.append(last)
        }
        lock.unlock()
    }

    public func attachDepth(_ depthTexture: any MTLTexture, mediaTime: TimeInterval?) {
        lock.lock()
        defer { lock.unlock() }
        lastDepthTexture = depthTexture
        let seconds = mediaTime ?? entries.last?.presentationSeconds
        guard let seconds else {
            return
        }
        lastDepthMediaTime = seconds
        depthRing.append(DepthEntry(texture: depthTexture, mediaSeconds: seconds))
        if depthRing.count > maxDepthEntryCount {
            depthRing.removeFirst(depthRing.count - maxDepthEntryCount)
        }
        let latestConfiguration = entries.last?.configuration
        if let index = entries.indices.min(by: {
            abs(entries[$0].presentationSeconds - seconds) < abs(entries[$1].presentationSeconds - seconds)
        }),
        abs(entries[index].presentationSeconds - seconds) <= depthAttachTolerance
        {
            entries[index].depthTexture = depthTexture
            entries[index].depthMediaTime = seconds
            if let latestConfiguration {
                entries[index].configuration = latestConfiguration
            }
            return
        }
        if var last = entries.last {
            last.depthTexture = depthTexture
            last.depthMediaTime = seconds
            if let latestConfiguration {
                last.configuration = latestConfiguration
            }
            entries[entries.count - 1] = last
        }
    }

    public static func shouldAcceptDecodedFrame(
        mediaTime: TimeInterval?,
        bufferIsEmpty: Bool,
        playbackAnchor: TimeInterval
    ) -> Bool {
        guard let mediaTime, mediaTime.isFinite else {
            return true
        }
        if bufferIsEmpty {
            // Bootstrap: accept the first decoded frames regardless of PTS offset (common on HLS/IPTV).
            return true
        }
        let playback: TimeInterval
        if ImmersiveStereoPlaybackTimeline.isPlaybackAnchored {
            playback = max(playbackAnchor, ImmersiveStereoPlaybackTimeline.nowMediaSeconds())
        } else {
            playback = playbackAnchor
        }
        return mediaTime <= playback + maxDecodeLeadSeconds
            && mediaTime >= playback - maxBufferLagSeconds
    }

    @discardableResult
    public func append(
        pixelBuffer: CVPixelBuffer,
        mediaTime: TimeInterval?,
        configuration: Video2DTo3DRenderConfiguration
    ) -> Bool {
        lock.lock()
        guard acceptingFrames || ImmersiveStereoSession.isUserRequestedActive else {
            rejectedNotAccepting += 1
            lock.unlock()
            return false
        }
        let anchor = playbackClockSecondsLocked()
        let seconds: TimeInterval
        if let mediaTime, mediaTime.isFinite {
            guard Self.shouldAcceptDecodedFrame(
                mediaTime: mediaTime,
                bufferIsEmpty: entries.isEmpty,
                playbackAnchor: anchor
            ) else {
                rejectedOutsideWindow += 1
                lock.unlock()
                return false
            }
            seconds = mediaTime
        } else if let last = entries.last?.presentationSeconds {
            seconds = last + 1.0 / 30.0
        } else {
            seconds = anchor
        }
        let isFirst = entries.isEmpty
        if isFirst, seconds.isFinite {
            let clock = playbackClockSecondsLocked()
            if abs(seconds - clock) > Self.bootstrapToleranceSeconds {
                playbackAnchorSeconds = max(0, seconds)
            } else if playbackAnchorSeconds <= 0.001, seconds > 0 {
                playbackAnchorSeconds = seconds
            }
        }
        if let last = entries.last?.presentationSeconds, seconds + 0.001 < last {
            if seconds < last - 0.25 {
                entries.removeAll()
            } else {
                rejectedOutOfOrder += 1
                lock.unlock()
                return false
            }
        }
        guard let retainedBuffer = VideoPixelBufferNV12Normalization.retainCopyForVideoFeed(from: pixelBuffer) else {
            lock.unlock()
            return false
        }
        let sequence = nextSequence
        nextSequence += 1
        entries.append(
            Entry(
                pixelBuffer: retainedBuffer,
                depthTexture: nil,
                depthMediaTime: nil,
                configuration: configuration,
                presentationSeconds: seconds,
                sequence: sequence
            )
        )
        appendedFrames += 1
        if entries.count > 1 {
            trimEntriesForPlaybackClockLocked()
        }
        if entries.count > maxEntryCount {
            entries.removeFirst(entries.count - maxEntryCount)
        }
        lock.unlock()
        return isFirst
    }

    /// Returns the audio-synced frame only when the ring has a newer entry than `afterSequence`.
    public func consumeFrame(
        forMediaSeconds targetSeconds: TimeInterval,
        afterSequence: UInt64
    ) -> ImmersiveStereoFrame? {
        lock.lock()
        defer { lock.unlock() }
        let candidate: Entry?
        if let entry = selectEntry(forMediaSeconds: targetSeconds) {
            candidate = entry
        } else if let nearest = entries.min(by: {
            abs($0.presentationSeconds - targetSeconds) < abs($1.presentationSeconds - targetSeconds)
        }),
        abs(nearest.presentationSeconds - targetSeconds) <= max(Self.presentationFallbackSeconds, Self.bootstrapToleranceSeconds)
        {
            candidate = nearest
        } else {
            candidate = nil
        }
        guard let candidate, candidate.sequence > afterSequence else {
            return nil
        }
        return makeStereoFrame(from: candidate)
    }

    public func frame(forMediaSeconds targetSeconds: TimeInterval) -> ImmersiveStereoFrame? {
        lock.lock()
        defer { lock.unlock() }
        if let entry = selectEntry(forMediaSeconds: targetSeconds) {
            return makeStereoFrame(from: entry)
        }
        guard let nearest = entries.min(by: {
            abs($0.presentationSeconds - targetSeconds) < abs($1.presentationSeconds - targetSeconds)
        }),
        abs(nearest.presentationSeconds - targetSeconds) <= max(Self.presentationFallbackSeconds, Self.bootstrapToleranceSeconds)
        else {
            return nil
        }
        return makeStereoFrame(from: nearest)
    }

    public func frame(forPresentationTime instant: LayerRenderer.Clock.Instant) -> ImmersiveStereoFrame? {
        let mediaSeconds = ImmersiveStereoPlaybackTimeline.mediaSeconds(forPresentationTime: instant)
        return frame(forMediaSeconds: mediaSeconds)
    }

    private func playbackClockSecondsLocked() -> TimeInterval {
        let timeline = ImmersiveStereoPlaybackTimeline.nowMediaSeconds()
        if ImmersiveStereoPlaybackTimeline.isPlaybackAnchored {
            return timeline
        }
        return max(playbackAnchorSeconds, timeline)
    }

    private func selectEntry(forMediaSeconds targetSeconds: TimeInterval) -> Entry? {
        guard !entries.isEmpty else {
            return nil
        }
        let frameSlack = 1.0 / 60.0
        let notPast = entries.filter { $0.presentationSeconds <= targetSeconds + frameSlack }
        if let best = notPast.max(by: { $0.presentationSeconds < $1.presentationSeconds }) {
            return best
        }
        return nil
    }

    private func trimEntriesForPlaybackClockLocked() {
        guard entries.count > 1 else {
            return
        }
        let playback = playbackClockSecondsLocked()
        let minSeconds = playback - Self.maxBufferLagSeconds
        let maxSeconds = playback + Self.maxDecodeLeadSeconds + 0.1
        entries.removeAll { $0.presentationSeconds < minSeconds || $0.presentationSeconds > maxSeconds }
    }

    private func makeStereoFrame(from entry: Entry) -> ImmersiveStereoFrame {
        let videoSeconds = entry.presentationSeconds
        let selectedDepth: DepthEntry? = {
            // Pick the newest depth frame not in the future relative to the video PTS.
            let candidates = depthRing.filter { $0.mediaSeconds <= videoSeconds + (1.0 / 60.0) }
            if let best = candidates.max(by: { $0.mediaSeconds < $1.mediaSeconds }) {
                return best
            }
            // Fallback: tolerate slight future depth within attach tolerance.
            if let nearest = depthRing.min(by: { abs($0.mediaSeconds - videoSeconds) < abs($1.mediaSeconds - videoSeconds) }),
               abs(nearest.mediaSeconds - videoSeconds) <= depthAttachTolerance
            {
                return nearest
            }
            return nil
        }()
        var depthTexture = entry.depthTexture ?? selectedDepth?.texture
        var depthMediaTime = entry.depthMediaTime ?? selectedDepth?.mediaSeconds
        if depthTexture == nil,
           let lastDepthTexture,
           let lastDepthMediaTime,
           abs(lastDepthMediaTime - videoSeconds) <= Self.maxDepthAgeSeconds,
           lastDepthMediaTime <= videoSeconds + depthAttachTolerance
        {
            depthTexture = lastDepthTexture
            depthMediaTime = lastDepthMediaTime
        }
        if let depthSeconds = depthMediaTime {
            let depthTooOld = videoSeconds - depthSeconds > Self.maxDepthAgeSeconds
            let depthTooFarAhead = depthSeconds > videoSeconds + depthAttachTolerance
            if depthTooOld || depthTooFarAhead {
                return ImmersiveStereoFrame(
                    pixelBuffer: entry.pixelBuffer,
                    depthTexture: nil,
                    configuration: entry.configuration,
                    videoMediaTime: entry.presentationSeconds,
                    depthMediaTime: depthMediaTime,
                    feedSequence: entry.sequence
                )
            }
        }
        return ImmersiveStereoFrame(
            pixelBuffer: entry.pixelBuffer,
            depthTexture: depthTexture,
            configuration: entry.configuration,
            videoMediaTime: entry.presentationSeconds,
            depthMediaTime: depthMediaTime,
            feedSequence: entry.sequence
        )
    }
}
#endif

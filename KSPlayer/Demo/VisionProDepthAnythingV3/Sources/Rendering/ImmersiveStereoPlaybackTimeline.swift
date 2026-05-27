#if os(visionOS) && canImport(CompositorServices)
import CompositorServices
import Foundation

/// Maps CompositorLayer presentation instants to KSPlayer media timeline seconds (audio master).
///
/// `LayerRenderer.Clock.Instant` is mach-continuous host time. KSPlayer exposes asset timeline as
/// `currentPlaybackTime` (seconds). We sample that position on the main thread (~10 Hz from
/// `KSPlayerLayer`) and extrapolate to each compositor `presentationTime` using playback rate.
public enum ImmersiveStereoPlaybackTimeline: Sendable {
    private struct State {
        var lastReportedMediaSeconds: TimeInterval = 0
        var lastReportedHostEpoch: TimeInterval?
        var isPlaying = false
        var playbackRate: Float = 1
    }

    private static let lock = NSLock()
    nonisolated(unsafe) private static var state = State()

    public static func reset() {
        lock.lock()
        state = State()
        lock.unlock()
    }

    /// Sample the player's current media position (call from main thread when playback advances).
    public static func update(
        playbackSeconds: TimeInterval,
        isPlaying: Bool,
        playbackRate: Float,
        hostEpoch: TimeInterval? = nil
    ) {
        lock.lock()
        defer {
            lock.unlock()
        }
        let host = hostEpoch ?? LayerRenderer.Clock.Instant.epochNow
        state.lastReportedMediaSeconds = playbackSeconds
        state.lastReportedHostEpoch = host
        state.isPlaying = isPlaying
        state.playbackRate = max(playbackRate, 0.01)
    }

    public static var isPlaybackAnchored: Bool {
        lock.lock()
        let anchored = state.isPlaying && state.lastReportedHostEpoch != nil
        lock.unlock()
        return anchored
    }

    /// Small lead so compositor picks a decoded frame ready for the next photon (still audio-mastered).
    public static let compositorPresentationLeadSeconds: TimeInterval = 0.04

    /// Media seconds at the compositor's predicted presentation instant (audio clock + lead).
    public static func mediaSeconds(forPresentationTime instant: LayerRenderer.Clock.Instant) -> TimeInterval {
        lock.lock()
        defer {
            lock.unlock()
        }
        guard state.isPlaying, let sampleHost = state.lastReportedHostEpoch else {
            return state.lastReportedMediaSeconds
        }
        let targetHost = instant.immersiveHostTimeIntervalSinceEpoch
        let elapsed = targetHost - sampleHost
        return state.lastReportedMediaSeconds
            + elapsed * Double(state.playbackRate)
            + compositorPresentationLeadSeconds
    }

    /// Media seconds at the compositor clock "now" (used to drop decode frames far ahead of audio).
    public static func nowMediaSeconds() -> TimeInterval {
        mediaSeconds(forPresentationTime: LayerRenderer.Clock().now)
    }
}

private extension LayerRenderer.Clock.Instant {
    static var epochNow: TimeInterval {
        LayerRenderer.Clock().now.immersiveHostTimeIntervalSinceEpoch
    }
}
#endif

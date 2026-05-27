#if os(visionOS) && canImport(CompositorServices)
import Foundation
import KSPlayer

/// Live A/V + depth timing snapshot for the in-app immersive dashboard.
public struct ImmersivePlaybackDiagnostics: Equatable, Sendable {
    public var audioTime: Double
    public var playbackAnchor: Double
    public var timelineAnchored: Bool
    public var videoRingCount: Int
    public var depthRingCount: Int
    public var videoPTSMin: Double?
    public var videoPTSMax: Double?
    public var lastDepthPTS: Double?
    public var depthAgeMs: Double?
    public var feedAppended: Int
    public var feedRejectedNotAccepting: Int
    public var feedRejectedOutsideWindow: Int
    public var compositorVideoDraws: Int
    public var compositorPlaceholderDraws: Int
    public var compositorHasStereoFrame: Bool
    public var immersiveSessionActive: Bool
    public var usesAudioSyncedPresentation: Bool
    public var suppressesDecodePathVideo: Bool
    public var videoOutputCallbackReceived: Bool

    public static let idle = ImmersivePlaybackDiagnostics(
        audioTime: 0,
        playbackAnchor: 0,
        timelineAnchored: false,
        videoRingCount: 0,
        depthRingCount: 0,
        videoPTSMin: nil,
        videoPTSMax: nil,
        lastDepthPTS: nil,
        depthAgeMs: nil,
        feedAppended: 0,
        feedRejectedNotAccepting: 0,
        feedRejectedOutsideWindow: 0,
        compositorVideoDraws: 0,
        compositorPlaceholderDraws: 0,
        compositorHasStereoFrame: false,
        immersiveSessionActive: false,
        usesAudioSyncedPresentation: false,
        suppressesDecodePathVideo: false,
        videoOutputCallbackReceived: false
    )

    public var summaryLines: [String] {
        let audio = String(format: "%.3f", audioTime)
        let anchor = String(format: "%.3f", playbackAnchor)
        let depthAge = depthAgeMs.map { String(format: "%+.0fms", $0) } ?? "—"
        let ptsRange: String
        if let videoPTSMin, let videoPTSMax {
            ptsRange = String(format: "%.2f–%.2f", videoPTSMin, videoPTSMax)
        } else {
            ptsRange = "—"
        }
        return [
            "Audio \(audio)s  anchor \(anchor)s  \(timelineAnchored ? "anchored" : "unanchored")",
            "Ring video \(videoRingCount) depth \(depthRingCount)  PTS [\(ptsRange)]  depthAge \(depthAge)",
            "Feed +\(feedAppended)  reject(accept=\(feedRejectedNotAccepting) window=\(feedRejectedOutsideWindow))",
            "Compositor video \(compositorVideoDraws)  placeholder \(compositorPlaceholderDraws)  stereo \(compositorHasStereoFrame ? "yes" : "no")",
            "Session \(immersiveSessionActive ? "active" : "off")  audioSync \(usesAudioSyncedPresentation ? "on" : "off")  decodeSuppress \(suppressesDecodePathVideo ? "on" : "off")  vout \(videoOutputCallbackReceived ? "yes" : "no")",
        ]
    }
}

public enum ImmersivePlaybackDiagnosticsCollector {
    public static func snapshot(
        audioTime: TimeInterval,
        playbackAnchor: TimeInterval,
        usesAudioSyncedPresentation: Bool,
        suppressesDecodePathVideo: Bool,
        videoOutputCallbackReceived: Bool
    ) -> ImmersivePlaybackDiagnostics {
        let feed = ImmersiveVideoFeed.shared.diagnosticsSnapshot()
        let compositor = ImmersiveStereoCompositorStatus.current()
        let depthAgeMs: Double?
        if let lastDepth = feed.lastDepthPTS, let lastVideo = feed.lastVideoPTS {
            depthAgeMs = (lastVideo - lastDepth) * 1000
        } else {
            depthAgeMs = nil
        }
        return ImmersivePlaybackDiagnostics(
            audioTime: audioTime,
            playbackAnchor: playbackAnchor,
            timelineAnchored: ImmersiveStereoPlaybackTimeline.isPlaybackAnchored,
            videoRingCount: feed.ringCount,
            depthRingCount: feed.depthRingCount,
            videoPTSMin: feed.minPTS,
            videoPTSMax: feed.maxPTS,
            lastDepthPTS: feed.lastDepthPTS,
            depthAgeMs: depthAgeMs,
            feedAppended: feed.appendedFrames,
            feedRejectedNotAccepting: feed.rejectedNotAccepting,
            feedRejectedOutsideWindow: feed.rejectedOutsideWindow,
            compositorVideoDraws: compositor.drewVideoFrames,
            compositorPlaceholderDraws: compositor.drewPlaceholderFrames,
            compositorHasStereoFrame: compositor.hasStereoFrame,
            immersiveSessionActive: ImmersiveStereoSession.isUserRequestedActive,
            usesAudioSyncedPresentation: usesAudioSyncedPresentation,
            suppressesDecodePathVideo: suppressesDecodePathVideo,
            videoOutputCallbackReceived: videoOutputCallbackReceived
        )
    }
}
#endif

#if os(visionOS) && canImport(CompositorServices)
import CompositorServices
import Foundation
import os

/// Live compositor health for the in-app status panel.
public enum ImmersiveStereoCompositorStatus: Sendable {
    public struct Snapshot: Equatable, Sendable {
        public var queriedFrames: Int
        public var submittedFrames: Int
        public var presentedDrawables: Int
        public var missedQueryNextFrame: Int
        public var missedPredictTiming: Int
        public var emptyDrawables: Int
        public var presentedWithoutAnchor: Int
        public var usedProjectionFallback: Int
        public var skippedNoDepthTexture: Int
        public var skippedNoDeviceAnchor: Int
        public var drewVideoFrames: Int
        public var drewPlaceholderFrames: Int
        public var hasStereoFrame: Bool
        public var lastIssue: String?
        public var temporal: TemporalSnapshot

        public static let idle = Snapshot(
            queriedFrames: 0,
            submittedFrames: 0,
            presentedDrawables: 0,
            missedQueryNextFrame: 0,
            missedPredictTiming: 0,
            emptyDrawables: 0,
            presentedWithoutAnchor: 0,
            usedProjectionFallback: 0,
            skippedNoDepthTexture: 0,
            skippedNoDeviceAnchor: 0,
            drewVideoFrames: 0,
            drewPlaceholderFrames: 0,
            hasStereoFrame: false,
            lastIssue: nil,
            temporal: .idle
        )
    }

    /// Display-time selection metrics for on-device judder validation.
    public struct TemporalSnapshot: Equatable, Sendable {
        public var targetMediaSeconds: Double
        public var selectedMediaSeconds: Double?
        public var deltaMilliseconds: Double?
        public var bufferCount: Int
        public var bufferMinMediaSeconds: Double?
        public var bufferMaxMediaSeconds: Double?
        public var timelineAnchored: Bool
        public var hostInstantSeconds: Double
        public var compositorPresents: Int
        public var selectionChanges: Int
        public var sameSelectionStreak: Int
        public var usedFallbackFrame: Int
        public var missedSelection: Int

        public static let idle = TemporalSnapshot(
            targetMediaSeconds: 0,
            selectedMediaSeconds: nil,
            deltaMilliseconds: nil,
            bufferCount: 0,
            bufferMinMediaSeconds: nil,
            bufferMaxMediaSeconds: nil,
            timelineAnchored: false,
            hostInstantSeconds: 0,
            compositorPresents: 0,
            selectionChanges: 0,
            sameSelectionStreak: 0,
            usedFallbackFrame: 0,
            missedSelection: 0
        )

        public var summaryLine: String {
            let target = String(format: "%.3f", targetMediaSeconds)
            let selected = selectedMediaSeconds.map { String(format: "%.3f", $0) } ?? "—"
            let delta = deltaMilliseconds.map { String(format: "%+.0fms", $0) } ?? "—"
            let range: String
            if let bufferMinMediaSeconds, let bufferMaxMediaSeconds {
                range = String(format: "%.2f–%.2f", bufferMinMediaSeconds, bufferMaxMediaSeconds)
            } else {
                range = "—"
            }
            let anchor = timelineAnchored ? "anchored" : "paused"
            return "Temporal target \(target)s sel \(selected)s Δ\(delta) buf \(bufferCount) [\(range)] \(anchor) hold \(sameSelectionStreak)x"
        }
    }

    private static let snapshot = OSAllocatedUnfairLock(initialState: Snapshot.idle)
    nonisolated(unsafe) private static var lastRecordedSelectionSeconds: Double?

    public static func current() -> Snapshot {
        snapshot.withLock { $0 }
    }

    public static func reset() {
        snapshot.withLock { $0 = .idle }
        lastRecordedSelectionSeconds = nil
    }

    static func liveEdgeDiagnostics(
        selectedMediaSeconds: TimeInterval?,
        bufferHasFrames: Bool
    ) -> ImmersiveStereoFrameStore.TemporalSelectionDiagnostics {
        let target = ImmersiveStereoPlaybackTimeline.nowMediaSeconds()
        let delta = selectedMediaSeconds.map { ($0 - target) * 1000 }
        return ImmersiveStereoFrameStore.TemporalSelectionDiagnostics(
            targetMediaSeconds: target,
            selectedMediaSeconds: selectedMediaSeconds,
            deltaMilliseconds: delta,
            bufferCount: bufferHasFrames ? 1 : 0,
            bufferMinMediaSeconds: selectedMediaSeconds,
            bufferMaxMediaSeconds: selectedMediaSeconds,
            timelineAnchored: ImmersiveStereoPlaybackTimeline.isPlaybackAnchored,
            hostInstantSeconds: LayerRenderer.Clock().now.immersiveHostTimeIntervalSinceEpoch,
            nearestCandidateMediaSeconds: selectedMediaSeconds,
            nearestDeltaMilliseconds: delta
        )
    }

    static func recordTemporalPresentation(
        diagnostics: ImmersiveStereoFrameStore.TemporalSelectionDiagnostics,
        usedFallback: Bool,
        hadSelection: Bool
    ) {
        snapshot.withLock { state in
            state.temporal.targetMediaSeconds = diagnostics.targetMediaSeconds
            state.temporal.selectedMediaSeconds = diagnostics.selectedMediaSeconds
            state.temporal.deltaMilliseconds = diagnostics.deltaMilliseconds
            state.temporal.bufferCount = diagnostics.bufferCount
            state.temporal.bufferMinMediaSeconds = diagnostics.bufferMinMediaSeconds
            state.temporal.bufferMaxMediaSeconds = diagnostics.bufferMaxMediaSeconds
            state.temporal.timelineAnchored = diagnostics.timelineAnchored
            state.temporal.hostInstantSeconds = diagnostics.hostInstantSeconds
            state.temporal.compositorPresents += 1
            if usedFallback {
                state.temporal.usedFallbackFrame += 1
            }
            if !hadSelection {
                state.temporal.missedSelection += 1
            }
            guard let selected = diagnostics.selectedMediaSeconds else {
                state.temporal.sameSelectionStreak = 0
                lastRecordedSelectionSeconds = nil
                return
            }
            if lastRecordedSelectionSeconds == selected {
                state.temporal.sameSelectionStreak += 1
            } else {
                state.temporal.selectionChanges += 1
                state.temporal.sameSelectionStreak = 1
                lastRecordedSelectionSeconds = selected
            }
        }
    }

    static func recordQueriedFrame() {
        snapshot.withLock { $0.queriedFrames += 1 }
    }

    static func recordMissedQueryNextFrame() {
        snapshot.withLock {
            $0.missedQueryNextFrame += 1
            $0.lastIssue = "queryNextFrame returned nil"
        }
    }

    static func recordMissedPredictTiming() {
        snapshot.withLock {
            $0.missedPredictTiming += 1
            $0.lastIssue = "predictTiming returned nil"
        }
    }

    static func recordSubmission(hasStereoFrame: Bool) {
        snapshot.withLock {
            $0.submittedFrames += 1
            $0.hasStereoFrame = hasStereoFrame
        }
    }

    static func recordEmptyDrawables() {
        snapshot.withLock {
            $0.emptyDrawables += 1
            $0.lastIssue = "queryDrawables returned no drawables"
        }
    }

    static func recordPresentedDrawable(
        drewVideo: Bool,
        drewPlaceholder: Bool,
        hadAnchor: Bool,
        usedProjectionFallback: Bool
    ) {
        snapshot.withLock {
            $0.presentedDrawables += 1
            if !hadAnchor {
                $0.presentedWithoutAnchor += 1
            }
            if usedProjectionFallback {
                $0.usedProjectionFallback += 1
            }
            if drewVideo {
                $0.drewVideoFrames += 1
            }
            if drewPlaceholder {
                $0.drewPlaceholderFrames += 1
            }
        }
    }

    static func recordSkippedNoDepthTexture() {
        snapshot.withLock {
            $0.skippedNoDepthTexture += 1
            $0.lastIssue = "Compositor drawable missing depth texture"
        }
    }

    static func recordSkippedNoDeviceAnchor() {
        snapshot.withLock {
            $0.skippedNoDeviceAnchor += 1
            $0.lastIssue = "ARKit device anchor unavailable; drawable not presented"
        }
    }
}
#endif

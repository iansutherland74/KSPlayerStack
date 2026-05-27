#if os(visionOS) && canImport(CompositorServices)
import Foundation
import os

enum ImmersiveStereoTemporalDebug {
    private static let logger = Logger(subsystem: "KSPlayer.DA3", category: "immersive-temporal")

    static var isEnabled: Bool {
        ProcessInfo.processInfo.environment["DA3_TEMPORAL_DEBUG"] == "1"
    }

    static func log(_ diagnostics: ImmersiveStereoFrameStore.TemporalSelectionDiagnostics) {
        let nearest = diagnostics.nearestCandidateMediaSeconds.map { String(format: "%.3f", $0) } ?? "—"
        let nearestDelta = diagnostics.nearestDeltaMilliseconds.map { String(format: "%+.0f", $0) } ?? "—"
        let target = String(format: "%.3f", diagnostics.targetMediaSeconds)
        let selected = diagnostics.selectedMediaSeconds.map { String(format: "%.3f", $0) } ?? "nil"
        let delta = diagnostics.deltaMilliseconds.map { String(format: "%+.1f", $0) } ?? "nil"
        logger.notice(
            "temporal target=\(target)s selected=\(selected)s deltaMs=\(delta) nearest=\(nearest) nearestDeltaMs=\(nearestDelta) buf=\(diagnostics.bufferCount) anchored=\(diagnostics.timelineAnchored)"
        )
    }
}
#endif

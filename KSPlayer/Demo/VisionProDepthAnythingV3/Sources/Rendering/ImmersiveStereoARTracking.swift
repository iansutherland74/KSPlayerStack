#if os(visionOS) && canImport(CompositorServices) && canImport(ARKit)
import ARKit
import CompositorServices
import Foundation
import KSPlayer
import os

/// Keeps ARKit world tracking alive for CompositorLayer presentation.
final class ImmersiveStereoARTracking: @unchecked Sendable {
    static let shared = ImmersiveStereoARTracking()

    private struct ARState {
        var worldTracking: WorldTrackingProvider
        var session: ARKitSession
    }

    private let stateLock = OSAllocatedUnfairLock(
        initialState: ARState(
            worldTracking: WorldTrackingProvider(),
            session: ARKitSession()
        )
    )
    private let isProviderRunning = OSAllocatedUnfairLock(initialState: false)
    private let hasStartedSession = OSAllocatedUnfairLock(initialState: false)
    private let lastSessionError = OSAllocatedUnfairLock<String?>(initialState: nil)
    private let lastDeviceAnchor = OSAllocatedUnfairLock<DeviceAnchor?>(initialState: nil)
    private let logger = Logger(subsystem: "KSPlayer.DA3", category: "ImmersiveAR")

    private init() {}

    var isSessionRunning: Bool {
        isProviderRunning.withLock { $0 }
    }

    var sessionErrorMessage: String? {
        lastSessionError.withLock { $0 }
    }

    func prepareAuthorizationIfNeeded() {
        Task { @MainActor in
            await requestAuthorizationIfNeeded()
        }
    }

    @MainActor
    func requestAuthorizationIfNeeded() async {
        guard WorldTrackingProvider.isSupported else {
            return
        }
        let session = stateLock.withLock { $0.session }
        _ = await session.requestAuthorization(for: WorldTrackingProvider.requiredAuthorizations)
    }

    /// World tracking stays `.paused` until an ImmersiveSpace is open — call after `openImmersiveSpace`.
    func ensureRunning(timeout: TimeInterval = 4) async -> Bool {
        guard WorldTrackingProvider.isSupported else {
            lastSessionError.withLock {
                $0 = "World tracking is not supported on this device."
            }
            return false
        }

        let started = await startSessionOnMainActor()
        guard started else {
            return false
        }

        let deadline = Date().timeIntervalSinceReferenceDate + timeout
        while Date().timeIntervalSinceReferenceDate < deadline {
            let running = await isWorldTrackingRunningOnMainActor()
            if running {
                isProviderRunning.withLock { $0 = true }
                lastSessionError.withLock { $0 = nil }
                Depth3DDebug.log("ARKit world tracking provider running", phase: "immersive-ar")
                return true
            }
            try? await Task.sleep(nanoseconds: 50_000_000)
        }

        let stateDescription = await worldTrackingStateDescription()
        let message = "ARKit world tracking did not reach running state (state: \(stateDescription))"
        lastSessionError.withLock { $0 = message }
        Depth3DDebug.warn(message, phase: "immersive-ar")
        return false
    }

    func stopIfNeeded() {
        Task { @MainActor in
            let shouldStop = hasStartedSession.withLock { $0 }
            guard shouldStop else {
                return
            }
            let session = stateLock.withLock { $0.session }
            session.stop()
            hasStartedSession.withLock { $0 = false }
            isProviderRunning.withLock { $0 = false }
            lastSessionError.withLock { $0 = nil }
            lastDeviceAnchor.withLock { $0 = nil }
            stateLock.withLock {
                // Stopped providers cannot be re-run; allocate fresh instances for the next immersive open.
                $0.worldTracking = WorldTrackingProvider()
                $0.session = ARKitSession()
            }
            Depth3DDebug.log("ARKit world tracking session stopped (providers recreated)", phase: "immersive-ar")
        }
    }

    func deviceAnchor(for timing: LayerRenderer.Frame.Timing) -> DeviceAnchor? {
        deviceAnchor(forPresentationTime: timing.presentationTime)
    }

    /// Query head pose at the drawable's presentation instant (Compositor Services contract).
    func deviceAnchor(forPresentationTime presentationTime: LayerRenderer.Clock.Instant) -> DeviceAnchor? {
        let timestamp = presentationTime.immersiveHostTimeIntervalSinceEpoch
        let anchor = queryDeviceAnchorOnMainActor(at: timestamp)
        if let anchor {
            lastDeviceAnchor.withLock { $0 = anchor }
            return anchor
        }
        return lastDeviceAnchor.withLock { $0 }
    }

    private func queryDeviceAnchorOnMainActor(at timestamp: TimeInterval) -> DeviceAnchor? {
        let worldTracking = stateLock.withLock { $0.worldTracking }
        let query: () -> DeviceAnchor? = {
            guard worldTracking.state == .running else {
                return nil
            }
            return worldTracking.queryDeviceAnchor(atTimestamp: timestamp)
        }
        if Thread.isMainThread {
            return query()
        }
        return DispatchQueue.main.sync(execute: query)
    }

    @MainActor
    private func startSessionOnMainActor() async -> Bool {
        let snapshot = stateLock.withLock { $0 }
        if snapshot.worldTracking.state == .running {
            isProviderRunning.withLock { $0 = true }
            lastSessionError.withLock { $0 = nil }
            return true
        }

        do {
            try await snapshot.session.run([snapshot.worldTracking])
            hasStartedSession.withLock { $0 = true }
            lastSessionError.withLock { $0 = nil }
            Depth3DDebug.log("ARKit session.run completed", phase: "immersive-ar")
            return true
        } catch {
            hasStartedSession.withLock { $0 = false }
            let message = "ARKit world tracking failed: \(error.localizedDescription)"
            lastSessionError.withLock { $0 = message }
            Depth3DDebug.warn(message, phase: "immersive-ar")
            logger.error("\(message, privacy: .public)")
            return false
        }
    }

    @MainActor
    private func isWorldTrackingRunningOnMainActor() -> Bool {
        stateLock.withLock { $0.worldTracking.state == .running }
    }

    @MainActor
    private func worldTrackingStateDescription() -> String {
        String(describing: stateLock.withLock { $0.worldTracking.state })
    }
}

enum ImmersiveStereoARTrackingBridge {
    static var isWorldTrackingSupported: Bool {
        WorldTrackingProvider.isSupported
    }

    static func prepareAuthorizationIfNeeded() {
        ImmersiveStereoARTracking.shared.prepareAuthorizationIfNeeded()
    }

    static func requestAuthorizationIfNeeded() async {
        await ImmersiveStereoARTracking.shared.requestAuthorizationIfNeeded()
    }

    static var isSessionRunning: Bool {
        ImmersiveStereoARTracking.shared.isSessionRunning
    }

    static var sessionErrorMessage: String? {
        ImmersiveStereoARTracking.shared.sessionErrorMessage
    }

    static func ensureRunning(timeout: TimeInterval = 4) async -> Bool {
        await ImmersiveStereoARTracking.shared.ensureRunning(timeout: timeout)
    }

    static func stopIfNeeded() {
        ImmersiveStereoARTracking.shared.stopIfNeeded()
    }

    static func deviceAnchor(for timing: LayerRenderer.Frame.Timing) -> DeviceAnchor? {
        ImmersiveStereoARTracking.shared.deviceAnchor(for: timing)
    }

    static func deviceAnchor(forPresentationTime presentationTime: LayerRenderer.Clock.Instant) -> DeviceAnchor? {
        ImmersiveStereoARTracking.shared.deviceAnchor(forPresentationTime: presentationTime)
    }
}

extension Duration {
    var immersiveHostTimeInterval: TimeInterval {
        let components = components
        return TimeInterval(components.seconds)
            + TimeInterval(components.attoseconds) / 1_000_000_000_000_000_000
    }
}

extension LayerRenderer.Clock.Instant {
    var immersiveHostTimeIntervalSinceEpoch: TimeInterval {
        LayerRenderer.Clock.Instant.epoch.duration(to: self).immersiveHostTimeInterval
    }
}
#endif

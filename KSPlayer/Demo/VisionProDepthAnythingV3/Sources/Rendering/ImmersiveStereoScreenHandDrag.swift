#if os(visionOS) && canImport(ARKit)
import ARKit
import Foundation
import KSPlayer
import os
import simd

/// Pinch + drag hand interaction to reposition the compositor stereo screen in world space.
@MainActor
enum ImmersiveStereoScreenHandDrag {
    private static let logger = Logger(subsystem: "KSPlayer.DA3", category: "ImmersiveHandDrag")
    private static let pinchThresholdMeters: Float = 0.028
    private static let screenHalfWidthMeters: Float = 1.0
    private static let screenHalfHeightMeters: Float = 0.5625

    private static var updateTask: Task<Void, Never>?
    private static var isPinching = false
    private static var dragStartHandPosition: SIMD3<Float>?
    private static var dragStartScreenPosition: SIMD3<Float>?

    static func startMonitoring(_ handTracking: HandTrackingProvider) {
        stop()
        updateTask = Task {
            for await update in handTracking.anchorUpdates {
                guard !Task.isCancelled else { return }
                handleHandAnchor(update.anchor)
            }
        }
    }

    static func stop() {
        updateTask?.cancel()
        updateTask = nil
        isPinching = false
        dragStartHandPosition = nil
        dragStartScreenPosition = nil
    }

    private static func handleHandAnchor(_ anchor: HandAnchor) {
        guard anchor.isTracked, let skeleton = anchor.handSkeleton else {
            if isPinching {
                endDrag()
            }
            return
        }

        guard let pinchWorld = pinchWorldPosition(for: anchor, skeleton: skeleton) else {
            if isPinching {
                endDrag()
            }
            return
        }

        let screenPosition = ImmersiveScreenPlacement.worldPosition
        if isPinching {
            guard let startHand = dragStartHandPosition,
                  let startScreen = dragStartScreenPosition else {
                endDrag()
                return
            }
            let delta = pinchWorld - startHand
            ImmersiveScreenPlacement.setWorldPosition(startScreen + delta)
            return
        }

        guard isNearScreen(pinchWorld, screenPosition: screenPosition) else {
            return
        }

        isPinching = true
        dragStartHandPosition = pinchWorld
        dragStartScreenPosition = screenPosition
        ImmersiveScreenPlacement.setAnchoringMode(.worldAnchored)
    }

    private static func endDrag() {
        isPinching = false
        dragStartHandPosition = nil
        dragStartScreenPosition = nil
    }

    private static func pinchWorldPosition(
        for anchor: HandAnchor,
        skeleton: HandSkeleton
    ) -> SIMD3<Float>? {
        let handTransform = anchor.originFromAnchorTransform
        let thumb = jointWorldPosition(.thumbTip, handTransform: handTransform, in: skeleton)
        let index = jointWorldPosition(.indexFingerTip, handTransform: handTransform, in: skeleton)
        guard let thumb, let index else { return nil }
        guard simd_distance(thumb, index) < pinchThresholdMeters else { return nil }
        return (thumb + index) * 0.5
    }

    private static func jointWorldPosition(
        _ jointName: HandSkeleton.JointName,
        handTransform: simd_float4x4,
        in skeleton: HandSkeleton
    ) -> SIMD3<Float>? {
        let joint = skeleton.joint(jointName)
        guard joint.isTracked else { return nil }
        let world = handTransform * joint.anchorFromJointTransform
        return SIMD3(world.columns.3.x, world.columns.3.y, world.columns.3.z)
    }

    private static func isNearScreen(_ point: SIMD3<Float>, screenPosition: SIMD3<Float>) -> Bool {
        let local = point - screenPosition
        return abs(local.x) <= screenHalfWidthMeters
            && abs(local.y) <= screenHalfHeightMeters
            && abs(local.z) <= 0.35
    }
}
#endif

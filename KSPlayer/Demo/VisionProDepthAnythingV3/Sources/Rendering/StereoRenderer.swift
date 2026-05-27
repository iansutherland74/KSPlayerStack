#if os(visionOS) && canImport(Metal)
import Combine
@preconcurrency import CoreVideo
@preconcurrency import Metal
import KSPlayer

@MainActor
public protocol StereoRendererProtocol: AnyObject {
    func update(
        sourceFrame: CVPixelBuffer,
        depthFrame: DA3DepthFrame,
        depthTexture: any MTLTexture,
        configuration: Video2DTo3DRenderConfiguration,
        mediaTime: TimeInterval?
    )
    /// Immersive compositor already has decoded video in the ring buffer; only publish depth.
    func updateDepth(
        depthTexture: any MTLTexture,
        configuration: Video2DTo3DRenderConfiguration,
        mediaTime: TimeInterval?
    )
    func reset()
}

public struct StereoRendererSnapshot: Equatable, Sendable {
    public let sourceWidth: Int
    public let sourceHeight: Int
    public let depthWidth: Int
    public let depthHeight: Int
    public let frameCount: Int

    public static let empty = StereoRendererSnapshot(
        sourceWidth: 0,
        sourceHeight: 0,
        depthWidth: 0,
        depthHeight: 0,
        frameCount: 0
    )
}

@MainActor
public final class StereoRenderer: ObservableObject, StereoRendererProtocol {
    @Published public private(set) var snapshot = StereoRendererSnapshot.empty
    public private(set) var latestDepthTexture: (any MTLTexture)?

    public init() {}

    public func update(
        sourceFrame: CVPixelBuffer,
        depthFrame: DA3DepthFrame,
        depthTexture: any MTLTexture,
        configuration: Video2DTo3DRenderConfiguration,
        mediaTime: TimeInterval? = nil
    ) {
        latestDepthTexture = depthTexture
        ImmersiveVideoFeed.shared.updateConfiguration(configuration)
        ImmersiveVideoFeed.shared.attachDepth(depthTexture, mediaTime: mediaTime)
        snapshot = StereoRendererSnapshot(
            sourceWidth: CVPixelBufferGetWidth(sourceFrame),
            sourceHeight: CVPixelBufferGetHeight(sourceFrame),
            depthWidth: depthFrame.width,
            depthHeight: depthFrame.height,
            frameCount: snapshot.frameCount + 1
        )
    }

    public func updateDepth(
        depthTexture: any MTLTexture,
        configuration: Video2DTo3DRenderConfiguration,
        mediaTime: TimeInterval? = nil
    ) {
        latestDepthTexture = depthTexture
        ImmersiveVideoFeed.shared.updateConfiguration(configuration)
        ImmersiveVideoFeed.shared.attachDepth(depthTexture, mediaTime: mediaTime)
        snapshot = StereoRendererSnapshot(
            sourceWidth: snapshot.sourceWidth,
            sourceHeight: snapshot.sourceHeight,
            depthWidth: depthTexture.width,
            depthHeight: depthTexture.height,
            frameCount: snapshot.frameCount + 1
        )
    }

    public func reset() {
        latestDepthTexture = nil
        ImmersiveVideoFeed.shared.clear()
        ImmersiveStereoFrameStore.shared.reset()
        snapshot = .empty
    }
}
#endif

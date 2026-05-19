#if os(visionOS) && canImport(Metal)
import Combine
@preconcurrency import CoreVideo
@preconcurrency import Metal
import KSPlayer

@MainActor
public protocol StereoRendererProtocol: AnyObject {
    func update(sourceFrame: CVPixelBuffer, depthFrame: DA3DepthFrame, depthTexture: any MTLTexture)
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

    public func update(sourceFrame: CVPixelBuffer, depthFrame: DA3DepthFrame, depthTexture: any MTLTexture) {
        latestDepthTexture = depthTexture
        snapshot = StereoRendererSnapshot(
            sourceWidth: CVPixelBufferGetWidth(sourceFrame),
            sourceHeight: CVPixelBufferGetHeight(sourceFrame),
            depthWidth: depthFrame.width,
            depthHeight: depthFrame.height,
            frameCount: snapshot.frameCount + 1
        )
    }

    public func reset() {
        latestDepthTexture = nil
        snapshot = .empty
    }
}
#endif

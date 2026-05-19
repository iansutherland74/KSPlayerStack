#if os(visionOS) && DEPTH_ANYTHING_V3_GENERATED && canImport(CoreML) && canImport(Metal)
import Combine
@preconcurrency import CoreML
@preconcurrency import CoreVideo
import Foundation
import KSPlayer
@preconcurrency import Metal

public enum KSPlayer3DIntegrationError: Error, Equatable, LocalizedError, Sendable {
    case metalDeviceUnavailable

    public var errorDescription: String? {
        switch self {
        case .metalDeviceUnavailable:
            return "Vision Pro Depth Anything V3 integration requires a Metal device."
        }
    }
}

@MainActor
public final class KSPlayer3DIntegrationViewModel: ObservableObject {
    @Published public private(set) var lastErrorMessage: String?

    public let url: URL
    public let options: KSOptions
    public let coordinator: KSVideoPlayer.Coordinator
    public let renderer: any StereoRendererProtocol

    private let pipeline: DA3VideoOutputPipeline
    private weak var installedLayer: KSPlayerLayer?

    public convenience init(
        url: URL,
        options: KSOptions = KSOptions(),
        modelConfiguration: MLModelConfiguration = MLModelConfiguration()
    ) throws {
        guard let device = MTLCreateSystemDefaultDevice() else {
            throw KSPlayer3DIntegrationError.metalDeviceUnavailable
        }
        let engine = try DepthAnythingV3Engine(configuration: modelConfiguration)
        self.init(url: url, options: options, engine: engine, renderer: StereoRenderer(), device: device)
    }

    public init(
        url: URL,
        options: KSOptions = KSOptions(),
        engine: DepthAnythingV3Engine,
        renderer: any StereoRendererProtocol,
        device: any MTLDevice
    ) {
        self.url = url
        self.options = options
        self.coordinator = KSVideoPlayer.Coordinator()
        self.renderer = renderer
        let pipeline = DA3VideoOutputPipeline(
            engine: engine,
            bridge: DA3DepthMetalBridge(device: device),
            renderer: renderer
        )
        self.pipeline = pipeline
        pipeline.setErrorHandler { [weak self] error in
            self?.lastErrorMessage = error.localizedDescription
        }
    }

    public func handleStateChanged(layer: KSPlayerLayer, state _: KSPlayerState) {
        installVideoOutput(on: layer)
    }

    public func stop() {
        installedLayer?.videoOutput = nil
        installedLayer = nil
        pipeline.reset()
        renderer.reset()
    }

    private func installVideoOutput(on layer: KSPlayerLayer) {
        guard installedLayer !== layer else {
            return
        }
        installedLayer?.videoOutput = nil
        installedLayer = layer
        layer.videoOutputConfiguration = KSVideoFrameOutput.Configuration(
            maximumBufferedFrameCount: 1,
            dropPolicy: .keepLatest,
            callbackQualityOfService: .userInitiated,
            callbackQueueLabel: "DepthAnythingV3.videoOutput"
        )
        layer.videoOutput = { [pipeline] pixelBuffer in
            pipeline.enqueue(pixelBuffer)
        }
    }
}

private final class DA3VideoOutputPipeline: @unchecked Sendable {
    private final class RetainedPixelBuffer: @unchecked Sendable {
        let pixelBuffer: CVPixelBuffer

        init(_ pixelBuffer: CVPixelBuffer) {
            self.pixelBuffer = pixelBuffer
        }
    }

    private let engine: DepthAnythingV3Engine
    private let bridge: DA3DepthMetalBridge
    private weak var renderer: (any StereoRendererProtocol)?
    private var onError: @MainActor @Sendable (Error) -> Void = { _ in }
    private let lock = NSLock()
    private var isProcessing = false

    init(
        engine: DepthAnythingV3Engine,
        bridge: DA3DepthMetalBridge,
        renderer: any StereoRendererProtocol
    ) {
        self.engine = engine
        self.bridge = bridge
        self.renderer = renderer
    }

    func setErrorHandler(_ onError: @escaping @MainActor @Sendable (Error) -> Void) {
        self.onError = onError
    }

    func enqueue(_ pixelBuffer: CVPixelBuffer) {
        guard beginProcessing() else {
            return
        }

        let retainedFrame = RetainedPixelBuffer(pixelBuffer)
        Task { [weak self, retainedFrame] in
            guard let self else {
                return
            }
            defer {
                self.finishProcessing()
            }

            do {
                let depthFrame = try await self.engine.makeDepthFrame(from: retainedFrame.pixelBuffer)
                let depthTexture = try self.bridge.makeTexture(from: depthFrame)
                await MainActor.run {
                    self.renderer?.update(
                        sourceFrame: retainedFrame.pixelBuffer,
                        depthFrame: depthFrame,
                        depthTexture: depthTexture
                    )
                }
            } catch is CancellationError {
                return
            } catch {
                await MainActor.run {
                    self.onError(error)
                }
            }
        }
    }

    func reset() {
        lock.lock()
        isProcessing = false
        lock.unlock()
    }

    private func beginProcessing() -> Bool {
        lock.lock()
        defer {
            lock.unlock()
        }
        guard !isProcessing else {
            return false
        }
        isProcessing = true
        return true
    }

    private func finishProcessing() {
        lock.lock()
        isProcessing = false
        lock.unlock()
    }
}
#endif

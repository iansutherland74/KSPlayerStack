#if os(visionOS) && canImport(Metal)
import AVFoundation
import CoreMedia
import CoreVideo
import KSPlayer
@preconcurrency import Metal
import os

/// Feeds per-eye DA3 frames into RealityKit stereo video for volumetric cinema windows.
@MainActor
final class ImmersiveStereoTaggedBufferPresenter {
    enum EnqueueResult: Sendable {
        case skippedNotReady
        case skippedNoFrame
        case skippedThrottled
        case enqueued
        case failed
    }

    let videoRenderer = AVSampleBufferVideoRenderer()

    private(set) var enqueuedFrameCount = 0
    private(set) var lastEnqueueResult: EnqueueResult = .skippedNotReady

    private let logger = Logger(subsystem: "KSPlayer.DA3", category: "CinemaStereoVideo")
    private let offscreenRenderer: ImmersiveStereoOffscreenRenderer
    private let textureCache: CVMetalTextureCache
    private var pixelBufferPool: CVMutablePixelBuffer.Pool?
    private var poolDimensions = (width: 0, height: 0)
    private var lastConsumedFeedSequence: UInt64 = 0
    private var lastPresentationTimeSeconds: TimeInterval = -1
    private var isWaitingForMediaData = false

    init?(
        offscreenRenderer: ImmersiveStereoOffscreenRenderer? = ImmersiveStereoOffscreenRenderer(),
        device: MTLDevice? = MTLCreateSystemDefaultDevice()
    ) {
        guard let offscreenRenderer,
              let device
        else {
            return nil
        }
        self.offscreenRenderer = offscreenRenderer

        var cache: CVMetalTextureCache?
        guard CVMetalTextureCacheCreate(kCFAllocatorDefault, nil, device, nil, &cache) == kCVReturnSuccess,
              let cache
        else {
            return nil
        }
        self.textureCache = cache
    }

    func prepareForPlayback() async {
        guard !isWaitingForMediaData else {
            return
        }
        isWaitingForMediaData = true
        await withCheckedContinuation { continuation in
            var resumed = false
            videoRenderer.requestMediaDataWhenReady(on: .main) {
                guard !resumed else {
                    return
                }
                resumed = true
                continuation.resume()
            }
        }
    }

    @discardableResult
    func enqueueLatestFrameIfPossible() -> EnqueueResult {
        guard videoRenderer.status != .failed else {
            lastEnqueueResult = .failed
            return .failed
        }
        guard videoRenderer.isReadyForMoreMediaData else {
            lastEnqueueResult = .skippedNotReady
            return .skippedNotReady
        }

        let lookupSeconds = ImmersiveStereoPlaybackTimeline.nowMediaSeconds()
        guard let stereoFrame = ImmersiveVideoFeed.shared.consumeFrame(
            forMediaSeconds: lookupSeconds,
            afterSequence: lastConsumedFeedSequence
        ) else {
            lastEnqueueResult = .skippedNoFrame
            return .skippedNoFrame
        }

        let width = CVPixelBufferGetWidth(stereoFrame.pixelBuffer)
        let height = CVPixelBufferGetHeight(stereoFrame.pixelBuffer)
        guard width > 0, height > 0 else {
            lastEnqueueResult = .failed
            return .failed
        }

        do {
            let pool = try pixelBufferPool(width: width, height: height)
            let leftBuffer = try pool.makeMutablePixelBuffer()
            let rightBuffer = try pool.makeMutablePixelBuffer()

            // Always use the NV12→BGRA Metal path (same matrices as immersive compositor). Without a
            // real depth map, uniforms disable parallax so L/R are identical flat stereo.
            try leftBuffer.withUnsafeBuffer { leftPixelBuffer in
                guard render(eye: .left, stereoFrame: stereoFrame, into: leftPixelBuffer) else {
                    throw PresenterError.renderFailed
                }
            }
            try rightBuffer.withUnsafeBuffer { rightPixelBuffer in
                guard render(eye: .right, stereoFrame: stereoFrame, into: rightPixelBuffer) else {
                    throw PresenterError.renderFailed
                }
            }

            let taggedBuffers = Self.makeTaggedBuffers(
                left: CVReadOnlyPixelBuffer(leftBuffer),
                right: CVReadOnlyPixelBuffer(rightBuffer)
            )
            let rawMediaSeconds = stereoFrame.videoMediaTime ?? lookupSeconds
            let presentationSeconds = max(rawMediaSeconds, lastPresentationTimeSeconds + (1.0 / 60.0))
            let presentationTime = CMTime(seconds: presentationSeconds, preferredTimescale: 600)

            let sampleBuffer = CMReadySampleBuffer(
                taggedBuffers: taggedBuffers,
                formatDescription: CMTaggedBufferGroupFormatDescription(taggedBuffers: taggedBuffers),
                presentationTimeStamp: presentationTime,
                duration: CMTime(value: 1, timescale: 60)
            )

            var didEnqueue = false
            try sampleBuffer.withUnsafeSampleBuffer { cmsampleBuffer in
                guard videoRenderer.isReadyForMoreMediaData else {
                    return
                }
                videoRenderer.enqueue(cmsampleBuffer)
                didEnqueue = true
            }
            guard didEnqueue else {
                lastEnqueueResult = .skippedNotReady
                return .skippedNotReady
            }

            lastPresentationTimeSeconds = presentationSeconds
            lastConsumedFeedSequence = stereoFrame.feedSequence
            enqueuedFrameCount += 1
            lastEnqueueResult = .enqueued
            if enqueuedFrameCount == 1 || enqueuedFrameCount % 120 == 0 {
                logger.info("Cinema stereo enqueued frame \(self.enqueuedFrameCount, privacy: .public)")
            }
            return .enqueued
        } catch {
            logger.error("Cinema stereo enqueue failed: \(String(describing: error), privacy: .public)")
            lastEnqueueResult = .failed
            return .failed
        }
    }

    func reset() {
        videoRenderer.flush()
        lastConsumedFeedSequence = 0
        lastPresentationTimeSeconds = -1
        enqueuedFrameCount = 0
        lastEnqueueResult = .skippedNotReady
        isWaitingForMediaData = false
    }

    private enum PresenterError: Error {
        case renderFailed
    }

    private func render(eye: StereoscopicVideoEye, stereoFrame: ImmersiveStereoFrame, into pixelBuffer: CVPixelBuffer) -> Bool {
        guard let texture = makeTexture(from: pixelBuffer, pixelFormat: .bgra8Unorm) else {
            return false
        }
        return offscreenRenderer.render(
            eye: eye,
            stereoFrame: stereoFrame,
            into: texture,
            forceFlatPresentation: true
        )
    }

    private func pixelBufferPool(width: Int, height: Int) throws -> CVMutablePixelBuffer.Pool {
        if let pixelBufferPool, poolDimensions.width == width, poolDimensions.height == height {
            return pixelBufferPool
        }

        let mergedAttributes = CVPixelBufferCreationAttributes(
            pixelFormatType: CVPixelFormatType(rawValue: kCVPixelFormatType_32BGRA),
            size: CVImageSize(width: width, height: height)
        )

        let configuration = CVMutablePixelBuffer.Pool.Configuration(
            ageOutDuration: 5,
            minimumBufferCount: 8
        )
        let pool = try CVMutablePixelBuffer.Pool(
            pixelBufferAttributes: mergedAttributes,
            configuration: configuration
        )
        pixelBufferPool = pool
        poolDimensions = (width, height)
        return pool
    }

    private func makeTexture(from pixelBuffer: CVPixelBuffer, pixelFormat: MTLPixelFormat) -> MTLTexture? {
        let width = CVPixelBufferGetWidth(pixelBuffer)
        let height = CVPixelBufferGetHeight(pixelBuffer)
        var cvTexture: CVMetalTexture?
        let status = CVMetalTextureCacheCreateTextureFromImage(
            kCFAllocatorDefault,
            textureCache,
            pixelBuffer,
            nil,
            pixelFormat,
            width,
            height,
            0,
            &cvTexture
        )
        guard status == kCVReturnSuccess,
              let cvTexture,
              let texture = CVMetalTextureGetTexture(cvTexture)
        else {
            return nil
        }
        return texture
    }

    private static func makeTaggedBuffers(
        left: CVReadOnlyPixelBuffer,
        right: CVReadOnlyPixelBuffer
    ) -> [CMTaggedDynamicBuffer] {
        let eyes: [(Int64, CMStereoViewComponents, CVReadOnlyPixelBuffer)] = [
            (0, .leftEye, left),
            (1, .rightEye, right),
        ]
        return eyes.map { layerID, eye, pixelBuffer in
            CMTaggedDynamicBuffer(
                tags: [.videoLayerID(layerID), .stereoView(eye), .mediaType(.video)],
                content: .pixelBuffer(pixelBuffer)
            )
        }
    }
}
#endif

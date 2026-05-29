#if os(visionOS) && canImport(CompositorServices) && canImport(Metal)
import ARKit
import CompositorServices
import CoreVideo
import KSPlayer
@preconcurrency import Metal
import os
import simd

enum ImmersiveStereoCompositorLauncher: Sendable {
    /// When true, presents opaque magenta instead of video (verifies CompositorLayer visibility).
    private static let debugSolidPresentation = OSAllocatedUnfairLock(initialState: false)

    static var isDebugSolidPresentationEnabled: Bool {
        debugSolidPresentation.withLock { $0 }
    }

    static func setDebugSolidPresentationEnabled(_ enabled: Bool) {
        debugSolidPresentation.withLock { $0 = enabled }
    }

    static func start(_ layerRenderer: LayerRenderer) {
        // Retain the renderer for the lifetime of the render thread.
        nonisolated(unsafe) let retainedRenderer = layerRenderer
        DispatchQueue(label: "KSPlayer.DA3.ImmersiveStereoCompositor.launch", qos: .userInteractive).async {
            ImmersiveStereoCompositor(layerRenderer: retainedRenderer).run()
        }
    }
}

private struct ImmersiveStereoPipelinePair {
    let nv12: MTLRenderPipelineState
    let bgra: MTLRenderPipelineState
    let placeholder: MTLRenderPipelineState
    let debugSolid: MTLRenderPipelineState
}

final class ImmersiveStereoCompositor: @unchecked Sendable {
    private let layerRenderer: LayerRenderer
    private let device: MTLDevice
    private let commandQueue: MTLCommandQueue
    private let clock = LayerRenderer.Clock()
    private let textureCache: CVMetalTextureCache
    private let metalLibrary: MTLLibrary
    private let worldVertexFunction: MTLFunction
    private let samplerState: MTLSamplerState
    private let neutralDepthTexture: MTLTexture
    private let yuvMatrix709VideoRangeBuffer: MTLBuffer
    private let yuvMatrix709FullRangeBuffer: MTLBuffer
    private let colorOffsetVideoRangeBuffer: MTLBuffer
    private let colorOffsetFullRangeBuffer: MTLBuffer
    /// Compositor Services uses reverse-Z (near = 1, far = 0); default `less` rejects our fullscreen draws.
    private let reverseZDepthStencilState: MTLDepthStencilState
    private var pipelineCache: [MTLPixelFormat: ImmersiveStereoPipelinePair] = [:]
    private var loggedWaitingForFrame = false
    private var loggedFirstPresent = false
    private var loggedEmptyDrawables = false
    private var loggedMissingDeviceAnchor = false
    private var loggedPresentationWarmup = false
    private var presentationWarmupFrames = 0
    private var lastPresentationEpoch = -1
    private let logger = Logger(subsystem: "KSPlayer.DA3", category: "ImmersiveCompositor")

    /// Black-clear presents before drawing video (still submits every frame to avoid frames-in-flight crash).
    private static let presentationWarmupRequired = 1

    init(layerRenderer: LayerRenderer) {
        self.layerRenderer = layerRenderer
        let device = layerRenderer.device
        self.device = device
        ImmersiveStereoMetalDeviceRegistry.registerCompositorDevice(device)
        guard let commandQueue = device.makeCommandQueue() else {
            fatalError("Unable to create Metal command queue for immersive stereo.")
        }
        self.commandQueue = commandQueue

        var cache: CVMetalTextureCache?
        let cacheStatus = CVMetalTextureCacheCreate(kCFAllocatorDefault, nil, device, nil, &cache)
        guard cacheStatus == kCVReturnSuccess, let cache else {
            fatalError("Unable to create CVMetalTextureCache for immersive stereo.")
        }
        self.textureCache = cache

        guard let library = try? device.makeDefaultLibrary(bundle: .main),
              let worldVertexFunction = library.makeFunction(name: "immersiveWorldVertex") else {
            fatalError("Unable to load immersive stereo Metal library from app bundle.")
        }
        self.metalLibrary = library
        self.worldVertexFunction = worldVertexFunction

        let samplerDescriptor = MTLSamplerDescriptor()
        samplerDescriptor.minFilter = .linear
        samplerDescriptor.magFilter = .linear
        guard let samplerState = device.makeSamplerState(descriptor: samplerDescriptor) else {
            fatalError("Unable to create sampler state for immersive stereo.")
        }
        self.samplerState = samplerState

        var neutralDepth: Float = 0.5
        let neutralDescriptor = MTLTextureDescriptor.texture2DDescriptor(
            pixelFormat: .r32Float,
            width: 1,
            height: 1,
            mipmapped: false
        )
        neutralDescriptor.usage = [.shaderRead]
        guard let neutralDepthTexture = device.makeTexture(descriptor: neutralDescriptor) else {
            fatalError("Unable to create neutral depth texture.")
        }
        neutralDepthTexture.replace(
            region: MTLRegionMake2D(0, 0, 1, 1),
            mipmapLevel: 0,
            withBytes: &neutralDepth,
            bytesPerRow: MemoryLayout<Float>.stride
        )
        self.neutralDepthTexture = neutralDepthTexture

        guard let yuvMatrix709VideoRangeBuffer = Self.makeMatrixBuffer(
            Self.yuvMatrix709VideoRange,
            device: device,
            label: "immersiveYUV709VideoRange"
        ),
            let yuvMatrix709FullRangeBuffer = Self.makeMatrixBuffer(
                Self.yuvMatrix709FullRange,
                device: device,
                label: "immersiveYUV709FullRange"
            ),
            let colorOffsetVideoRangeBuffer = Self.makeColorOffsetBuffer(
                Self.colorOffsetVideoRange,
                device: device,
                label: "immersiveOffsetVideoRange"
            ),
            let colorOffsetFullRangeBuffer = Self.makeColorOffsetBuffer(
                Self.colorOffsetFullRange,
                device: device,
                label: "immersiveOffsetFullRange"
            )
        else {
            fatalError("Unable to create immersive YUV conversion buffers.")
        }
        self.yuvMatrix709VideoRangeBuffer = yuvMatrix709VideoRangeBuffer
        self.yuvMatrix709FullRangeBuffer = yuvMatrix709FullRangeBuffer
        self.colorOffsetVideoRangeBuffer = colorOffsetVideoRangeBuffer
        self.colorOffsetFullRangeBuffer = colorOffsetFullRangeBuffer

        let depthStencilDescriptor = MTLDepthStencilDescriptor()
        depthStencilDescriptor.depthCompareFunction = .greaterEqual
        depthStencilDescriptor.isDepthWriteEnabled = true
        guard let reverseZDepthStencilState = device.makeDepthStencilState(descriptor: depthStencilDescriptor) else {
            fatalError("Unable to create reverse-Z depth stencil state for immersive stereo.")
        }
        self.reverseZDepthStencilState = reverseZDepthStencilState
    }

    func run() {
        layerRenderer.waitUntilRunning()
        Task {
            _ = await ImmersiveStereoARTrackingBridge.ensureRunning(timeout: 12)
        }
        logger.notice("Immersive compositor render loop running")
        Depth3DDebug.log("Immersive compositor render loop running", phase: "immersive-compositor")
        while layerRenderer.state != .invalidated {
            switch layerRenderer.state {
            case .paused:
                layerRenderer.waitUntilRunning()
            case .running:
                autoreleasepool {
                    if ImmersiveStereoSession.isUserRequestedActive {
                        renderFrameIfPossible()
                    }
                }
            case .invalidated:
                ImmersiveStereoMetalDeviceRegistry.clearCompositorDevice()
                return
            @unknown default:
                ImmersiveStereoMetalDeviceRegistry.clearCompositorDevice()
                return
            }
        }
        ImmersiveStereoMetalDeviceRegistry.clearCompositorDevice()
    }

    /// Every queried frame must finish submission; warmup only changes what we draw.
    private func renderFrameIfPossible() {
        guard let frame = layerRenderer.queryNextFrame() else {
            ImmersiveStereoCompositorStatus.recordMissedQueryNextFrame()
            return
        }
        ImmersiveStereoCompositorStatus.recordQueriedFrame()

        var stereoFrame: ImmersiveStereoFrame?
        if let timing = frame.predictTiming() {
            let epoch = ImmersiveStereoSession.presentationEpoch
            if epoch != lastPresentationEpoch {
                lastPresentationEpoch = epoch
            }
            frame.startUpdate()
            stereoFrame = ImmersiveVideoFeed.shared.frame(forPresentationTime: timing.presentationTime)
            ImmersiveStereoCompositorStatus.recordTemporalPresentation(
                diagnostics: ImmersiveStereoCompositorStatus.liveEdgeDiagnostics(
                    selectedMediaSeconds: stereoFrame?.videoMediaTime,
                    bufferHasFrames: ImmersiveVideoFeed.shared.hasFrames
                ),
                usedFallback: false,
                hadSelection: stereoFrame != nil
            )
            frame.endUpdate()
            clock.wait(until: timing.optimalInputTime)

            frame.startSubmission()
            defer { frame.endSubmission() }

            let drawables = frame.queryDrawables()
            if drawables.isEmpty {
                if !loggedEmptyDrawables {
                    loggedEmptyDrawables = true
                    logger.error("queryDrawables returned empty — check CompositorLayer configuration")
                }
                ImmersiveStereoCompositorStatus.recordEmptyDrawables()
                return
            }

            let deviceAnchor = ImmersiveStereoARTrackingBridge.deviceAnchor(for: timing)
            let presentMode = resolvePresentMode(
                deviceAnchor: deviceAnchor,
                hasStereoContent: stereoFrame != nil
            )
            ImmersiveStereoCompositorStatus.recordSubmission(hasStereoFrame: stereoFrame != nil)

            for drawable in drawables {
                presentDrawable(
                    drawable: drawable,
                    stereoFrame: stereoFrame,
                    deviceAnchor: deviceAnchor,
                    mode: presentMode
                )
            }
            return
        }

        ImmersiveStereoCompositorStatus.recordMissedPredictTiming()
        frame.startSubmission()
        defer { frame.endSubmission() }
        let drawables = frame.queryDrawables()
        if drawables.isEmpty {
            ImmersiveStereoCompositorStatus.recordEmptyDrawables()
            return
        }
        ImmersiveStereoCompositorStatus.recordSubmission(hasStereoFrame: false)
        for drawable in drawables {
            presentDrawable(
                drawable: drawable,
                stereoFrame: nil,
                deviceAnchor: nil,
                mode: .warmupClear
            )
        }
    }

    private enum ImmersivePresentMode {
        case warmupClear
        case debugSolid
        case stereoContent
    }

    private func resolvePresentMode(deviceAnchor: DeviceAnchor?, hasStereoContent: Bool) -> ImmersivePresentMode {
        if hasStereoContent {
            presentationWarmupFrames = Self.presentationWarmupRequired
            loggedPresentationWarmup = false
        }
        guard deviceAnchor != nil else {
            presentationWarmupFrames = 0
            if !loggedMissingDeviceAnchor {
                loggedMissingDeviceAnchor = true
                logger.error("Warmup present without device anchor (using clear)")
            }
            ImmersiveStereoCompositorStatus.recordSkippedNoDeviceAnchor()
            return .warmupClear
        }

        if ImmersiveStereoCompositorLauncher.isDebugSolidPresentationEnabled {
            presentationWarmupFrames = Self.presentationWarmupRequired
            loggedPresentationWarmup = false
            loggedMissingDeviceAnchor = false
            return .debugSolid
        }

        presentationWarmupFrames += 1
        guard presentationWarmupFrames >= Self.presentationWarmupRequired else {
            if !loggedPresentationWarmup {
                loggedPresentationWarmup = true
                logger.notice(
                    "Immersive presentation warmup (\(self.presentationWarmupFrames)/\(Self.presentationWarmupRequired)); clear submit"
                )
            }
            return .warmupClear
        }

        loggedPresentationWarmup = false
        loggedMissingDeviceAnchor = false
        return .stereoContent
    }

    /// Dedicated layout: one drawable, one encodePresent, separate render pass per eye texture index.
    private func presentDrawable(
        drawable: LayerRenderer.Drawable,
        stereoFrame: ImmersiveStereoFrame?,
        deviceAnchor: DeviceAnchor?,
        mode: ImmersivePresentMode
    ) {
        if let deviceAnchor {
            drawable.deviceAnchor = deviceAnchor
        }

        guard !drawable.colorTextures.isEmpty else {
            logger.error("Drawable has no color texture; cannot encodePresent")
            return
        }
        guard let commandBuffer = commandQueue.makeCommandBuffer() else {
            logger.error("Failed to allocate Metal command buffer for immersive drawable")
            return
        }

        let viewCount = max(drawable.views.count, drawable.colorTextures.count)
        var drewVideo = false
        var drewPlaceholder = false
        let isWarmup = mode == .warmupClear

        for viewIndex in 0 ..< viewCount {
            guard viewIndex < drawable.colorTextures.count else {
                break
            }
            let eye: StereoscopicVideoEye = viewIndex == 0 ? .left : .right
            let colorTexture = drawable.colorTextures[viewIndex]
            let modelViewProjection: simd_float4x4
            if let deviceAnchor {
                modelViewProjection = makeModelViewProjection(
                    drawable: drawable,
                    deviceAnchor: deviceAnchor,
                    viewIndex: viewIndex,
                    stereoFrame: stereoFrame
                )
            } else {
                modelViewProjection = matrix_identity_float4x4
            }

            let renderPassDescriptor = MTLRenderPassDescriptor()
            renderPassDescriptor.colorAttachments[0].texture = colorTexture
            renderPassDescriptor.colorAttachments[0].loadAction = .clear
            renderPassDescriptor.colorAttachments[0].storeAction = .store
            // Transparent background: no visible letterbox/pillarbox frame around the screen quad.
            renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColor(red: 0, green: 0, blue: 0, alpha: 0)

            if viewIndex < drawable.depthTextures.count {
                renderPassDescriptor.depthAttachment.texture = drawable.depthTextures[viewIndex]
                renderPassDescriptor.depthAttachment.loadAction = .clear
                renderPassDescriptor.depthAttachment.storeAction = .store
                renderPassDescriptor.depthAttachment.clearDepth = 0
            } else if viewIndex == 0, !isWarmup {
                ImmersiveStereoCompositorStatus.recordSkippedNoDepthTexture()
                logger.error("Drawable has no depth texture; configure CompositorLayer depthFormat")
            }

            guard let encoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) else {
                continue
            }
            encoder.setDepthStencilState(reverseZDepthStencilState)

            switch mode {
            case .warmupClear:
                if let pipelines = pipeline(for: colorTexture.pixelFormat) {
                    encoder.setFragmentTexture(neutralDepthTexture, index: 0)
                    encoder.setFragmentSamplerState(samplerState, index: 0)
                    var uniforms = Self.placeholderUniforms(eye: eye)
                    encoder.setFragmentBytes(&uniforms, length: MemoryLayout<ImmersiveStereoUniforms>.stride, index: 0)
                    drawScreenQuad(
                        encoder: encoder,
                        pipeline: pipelines.placeholder,
                        modelViewProjection: modelViewProjection
                    )
                    drewPlaceholder = true
                }
            case .debugSolid:
                if let pipelines = pipeline(for: colorTexture.pixelFormat) {
                    drawScreenQuad(
                        encoder: encoder,
                        pipeline: pipelines.debugSolid,
                        modelViewProjection: modelViewProjection
                    )
                    drewVideo = true
                }
            case .stereoContent:
                if let stereoFrame,
                   let pipelines = pipeline(for: colorTexture.pixelFormat),
                   drawVideo(
                       encoder: encoder,
                       pipelines: pipelines,
                       stereoFrame: stereoFrame,
                       eye: eye,
                       modelViewProjection: modelViewProjection
                   ) {
                    drewVideo = true
                    loggedWaitingForFrame = false
                } else if let pipelines = pipeline(for: colorTexture.pixelFormat) {
                    if stereoFrame == nil, !loggedWaitingForFrame {
                        loggedWaitingForFrame = true
                        logger.warning("Immersive compositor has no stereo frame yet (enable 3D and wait for DA3)")
                        Depth3DDebug.warn(
                            "Immersive compositor has no stereo frame yet (enable 3D and wait for DA3)",
                            phase: "immersive-compositor"
                        )
                    }
                    encoder.setFragmentTexture(neutralDepthTexture, index: 0)
                    encoder.setFragmentSamplerState(samplerState, index: 0)
                    var uniforms = Self.placeholderUniforms(eye: eye)
                    encoder.setFragmentBytes(&uniforms, length: MemoryLayout<ImmersiveStereoUniforms>.stride, index: 0)
                    drawScreenQuad(
                        encoder: encoder,
                        pipeline: pipelines.placeholder,
                        modelViewProjection: modelViewProjection
                    )
                    drewPlaceholder = true
                }
            }
            encoder.endEncoding()
        }

        drawable.encodePresent(commandBuffer: commandBuffer)
        commandBuffer.commit()

        ImmersiveStereoCompositorStatus.recordPresentedDrawable(
            drewVideo: drewVideo,
            drewPlaceholder: drewPlaceholder || isWarmup,
            hadAnchor: deviceAnchor != nil,
            usedProjectionFallback: deviceAnchor == nil
        )
        if !loggedFirstPresent {
            loggedFirstPresent = true
            logger.notice(
                "First immersive present mode=\(String(describing: mode)) video=\(drewVideo) placeholder=\(drewPlaceholder) views=\(viewCount) anchor=\(deviceAnchor != nil)"
            )
        }
    }

    private func drawScreenQuad(
        encoder: MTLRenderCommandEncoder,
        pipeline: MTLRenderPipelineState,
        modelViewProjection: simd_float4x4
    ) {
        var mvp = modelViewProjection
        encoder.setRenderPipelineState(pipeline)
        encoder.setVertexBytes(&mvp, length: MemoryLayout<simd_float4x4>.stride, index: 0)
        encoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
    }

    private func makeModelViewProjection(
        drawable: LayerRenderer.Drawable,
        deviceAnchor: DeviceAnchor,
        viewIndex: Int,
        stereoFrame: ImmersiveStereoFrame?
    ) -> simd_float4x4 {
        let deviceTransform = deviceAnchor.originFromAnchorTransform
        let resolvedViewIndex = min(max(viewIndex, 0), max(drawable.views.count - 1, 0))

        let viewMatrix: simd_float4x4
        if drawable.views.isEmpty {
            viewMatrix = simd_inverse(deviceTransform)
        } else {
            let eyeTransform = drawable.views[resolvedViewIndex].transform
            viewMatrix = simd_inverse(deviceTransform * eyeTransform)
        }

        let projectionMatrix = drawable.computeProjection(viewIndex: resolvedViewIndex)
        let drawableAspect = drawableAspectRatio(drawable: drawable, viewIndex: resolvedViewIndex)
        let modelMatrix = Self.makeScreenModelMatrix(
            headTransform: deviceTransform,
            videoAspect: videoDisplayAspectRatio(for: stereoFrame),
            drawableAspect: drawableAspect
        )
        return projectionMatrix * viewMatrix * modelMatrix
    }

    private func drawableAspectRatio(drawable: LayerRenderer.Drawable, viewIndex: Int) -> Float? {
        guard viewIndex < drawable.colorTextures.count else {
            return nil
        }
        let texture = drawable.colorTextures[viewIndex]
        let height = Float(texture.height)
        guard height > 0 else {
            return nil
        }
        return Float(texture.width) / height
    }

    private func videoDisplayAspectRatio(for stereoFrame: ImmersiveStereoFrame?) -> Float {
        let bufferAspect = stereoFrame.map { Self.displayAspectRatio(for: $0.pixelBuffer) } ?? (16.0 / 9.0)
        return ImmersiveVideoFeed.shared.resolvedSourceDisplayAspectRatio(fallback: bufferAspect)
    }

    /// Display aspect (SAR-aware) of a decoded frame buffer.
    fileprivate static func displayAspectRatio(for pixelBuffer: CVPixelBuffer) -> Float {
        let width = Float(CVPixelBufferGetWidth(pixelBuffer))
        let height = Float(CVPixelBufferGetHeight(pixelBuffer))
        guard width > 0, height > 0 else {
            return 16.0 / 9.0
        }
        let sar = pixelBuffer.aspectRatio
        let displayWidth = width * Float(sar.width / max(sar.height, 1))
        return max(displayWidth / height, 0.1)
    }

    private func drawVideo(
        encoder: MTLRenderCommandEncoder,
        pipelines: ImmersiveStereoPipelinePair,
        stereoFrame: ImmersiveStereoFrame,
        eye: StereoscopicVideoEye,
        modelViewProjection: simd_float4x4
    ) -> Bool {
        let configuration = stereoFrame.configuration
        let usesDepthMap = (configuration.usesDepthMap || stereoFrame.depthTexture != nil)
            && !stereoFrame.isDepthStaleForPresentation
        var uniforms = Self.makeUniforms(
            configuration: configuration,
            eye: eye,
            usesDepthMap: usesDepthMap
        )
        let depthTexture = usesDepthMap ? (stereoFrame.depthTexture ?? neutralDepthTexture) : neutralDepthTexture
        let pixelFormat = CVPixelBufferGetPixelFormatType(stereoFrame.pixelBuffer)
        if isPlanar420(pixelFormat) {
            return false
        }
        if isNV12(pixelFormat),
           let lumaTexture = makeTexture(from: stereoFrame.pixelBuffer, planeIndex: 0, pixelFormat: .r8Unorm),
           let chromaTexture = makeTexture(from: stereoFrame.pixelBuffer, planeIndex: 1, pixelFormat: .rg8Unorm) {
            let yuvBuffers = yuvConversionBuffers(for: stereoFrame.pixelBuffer)
            encoder.setFragmentTexture(lumaTexture, index: 0)
            encoder.setFragmentTexture(chromaTexture, index: 1)
            encoder.setFragmentTexture(depthTexture, index: 2)
            encoder.setFragmentSamplerState(samplerState, index: 0)
            encoder.setFragmentBuffer(yuvBuffers.matrix, offset: 0, index: 0)
            encoder.setFragmentBuffer(yuvBuffers.offset, offset: 0, index: 1)
            encoder.setFragmentBytes(&uniforms, length: MemoryLayout<ImmersiveStereoUniforms>.stride, index: 2)
            drawScreenQuad(encoder: encoder, pipeline: pipelines.nv12, modelViewProjection: modelViewProjection)
            return true
        }
        if let colorSource = makeTexture(
            from: stereoFrame.pixelBuffer,
            planeIndex: 0,
            pixelFormat: .bgra8Unorm
        ) {
            encoder.setFragmentTexture(colorSource, index: 0)
            encoder.setFragmentTexture(depthTexture, index: 1)
            encoder.setFragmentSamplerState(samplerState, index: 0)
            encoder.setFragmentBytes(&uniforms, length: MemoryLayout<ImmersiveStereoUniforms>.stride, index: 0)
            drawScreenQuad(encoder: encoder, pipeline: pipelines.bgra, modelViewProjection: modelViewProjection)
            return true
        }
        return false
    }

    private func pipeline(for colorPixelFormat: MTLPixelFormat) -> ImmersiveStereoPipelinePair? {
        if let cached = pipelineCache[colorPixelFormat] {
            return cached
        }
        guard let pair = try? makePipelinePair(colorPixelFormat: colorPixelFormat) else {
            return nil
        }
        pipelineCache[colorPixelFormat] = pair
        return pair
    }

    private func makePipelinePair(colorPixelFormat: MTLPixelFormat) throws -> ImmersiveStereoPipelinePair {
        let usesRGBA16 = colorPixelFormat == .rgba16Float
        let nv12FunctionName = usesRGBA16 ? "immersiveNV12RGBA16Fragment" : "immersiveNV12Fragment"
        let bgraFunctionName = usesRGBA16 ? "immersiveRGBA16Fragment" : "immersiveBGRAFragment"
        let placeholderFunctionName = usesRGBA16 ? "immersivePlaceholderRGBA16Fragment" : "immersivePlaceholderFragment"
        let debugSolidFunctionName = usesRGBA16 ? "immersiveDebugSolidRGBA16Fragment" : "immersiveDebugSolidFragment"

        guard let nv12Function = metalLibrary.makeFunction(name: nv12FunctionName),
              let bgraFunction = metalLibrary.makeFunction(name: bgraFunctionName),
              let placeholderFunction = metalLibrary.makeFunction(name: placeholderFunctionName),
              let debugSolidFunction = metalLibrary.makeFunction(name: debugSolidFunctionName) else {
            throw NSError(
                domain: "KSPlayer.DA3.ImmersiveStereo",
                code: 1,
                userInfo: [NSLocalizedDescriptionKey: "Missing immersive fragment functions for \(colorPixelFormat)"]
            )
        }

        let nv12Descriptor = MTLRenderPipelineDescriptor()
        nv12Descriptor.vertexFunction = worldVertexFunction
        nv12Descriptor.fragmentFunction = nv12Function
        nv12Descriptor.colorAttachments[0].pixelFormat = colorPixelFormat
        nv12Descriptor.depthAttachmentPixelFormat = .depth32Float

        let bgraDescriptor = MTLRenderPipelineDescriptor()
        bgraDescriptor.vertexFunction = worldVertexFunction
        bgraDescriptor.fragmentFunction = bgraFunction
        bgraDescriptor.colorAttachments[0].pixelFormat = colorPixelFormat
        bgraDescriptor.depthAttachmentPixelFormat = .depth32Float

        let placeholderDescriptor = MTLRenderPipelineDescriptor()
        placeholderDescriptor.vertexFunction = worldVertexFunction
        placeholderDescriptor.fragmentFunction = placeholderFunction
        placeholderDescriptor.colorAttachments[0].pixelFormat = colorPixelFormat
        placeholderDescriptor.depthAttachmentPixelFormat = .depth32Float

        let debugSolidDescriptor = MTLRenderPipelineDescriptor()
        debugSolidDescriptor.vertexFunction = worldVertexFunction
        debugSolidDescriptor.fragmentFunction = debugSolidFunction
        debugSolidDescriptor.colorAttachments[0].pixelFormat = colorPixelFormat
        debugSolidDescriptor.depthAttachmentPixelFormat = .depth32Float

        return ImmersiveStereoPipelinePair(
            nv12: try device.makeRenderPipelineState(descriptor: nv12Descriptor),
            bgra: try device.makeRenderPipelineState(descriptor: bgraDescriptor),
            placeholder: try device.makeRenderPipelineState(descriptor: placeholderDescriptor),
            debugSolid: try device.makeRenderPipelineState(descriptor: debugSolidDescriptor)
        )
    }

    private func isNV12(_ pixelFormat: OSType) -> Bool {
        pixelFormat == kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange ||
            pixelFormat == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
    }

    private func isPlanar420(_ pixelFormat: OSType) -> Bool {
        pixelFormat == kCVPixelFormatType_420YpCbCr8Planar ||
            pixelFormat == kCVPixelFormatType_420YpCbCr8PlanarFullRange
    }

    private func yuvConversionBuffers(for pixelBuffer: CVPixelBuffer) -> (matrix: MTLBuffer, offset: MTLBuffer) {
        let isFullRange = CVPixelBufferGetPixelFormatType(pixelBuffer)
            == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
        if isFullRange {
            return (yuvMatrix709FullRangeBuffer, colorOffsetFullRangeBuffer)
        }
        return (yuvMatrix709VideoRangeBuffer, colorOffsetVideoRangeBuffer)
    }

    /// BT.709 full-range (matches KSPlayer `MetalRender` full-range path).
    private static let yuvMatrix709FullRange = simd_float3x3(
        SIMD3<Float>(1, 1, 1),
        SIMD3<Float>(0, -0.187_324, 1.8556),
        SIMD3<Float>(1.5748, -0.468_124, 0)
    )

    /// BT.709 video-range scale factors (matches KSPlayer `MetalRender.videoRange`).
    private static let yuvMatrix709VideoRange = simd_float3x3(
        SIMD3<Float>(255.0 / 219.0, 255.0 / 219.0, 255.0 / 219.0),
        SIMD3<Float>(0, -0.187_324 * 255.0 / 224.0, 1.8556 * 255.0 / 224.0),
        SIMD3<Float>(1.5748 * 255.0 / 224.0, -0.468_124 * 255.0 / 224.0, 0)
    )

    private static let colorOffsetVideoRange = SIMD3<Float>(-16.0 / 255.0, -128.0 / 255.0, -128.0 / 255.0)
    private static let colorOffsetFullRange = SIMD3<Float>(0, -128.0 / 255.0, -128.0 / 255.0)

    private static func makeMatrixBuffer(
        _ matrix: simd_float3x3,
        device: MTLDevice,
        label: String
    ) -> MTLBuffer? {
        var copy = matrix
        let buffer = device.makeBuffer(bytes: &copy, length: MemoryLayout<simd_float3x3>.stride)
        buffer?.label = label
        return buffer
    }

    private static func makeColorOffsetBuffer(
        _ offset: SIMD3<Float>,
        device: MTLDevice,
        label: String
    ) -> MTLBuffer? {
        var copy = offset
        let buffer = device.makeBuffer(bytes: &copy, length: MemoryLayout<SIMD3<Float>>.stride)
        buffer?.label = label
        return buffer
    }

    private func makeTexture(
        from pixelBuffer: CVPixelBuffer,
        planeIndex: Int,
        pixelFormat: MTLPixelFormat
    ) -> MTLTexture? {
        let width = CVPixelBufferGetWidthOfPlane(pixelBuffer, planeIndex)
        let height = CVPixelBufferGetHeightOfPlane(pixelBuffer, planeIndex)
        guard width > 0, height > 0 else {
            return nil
        }

        var cvTexture: CVMetalTexture?
        let status = CVMetalTextureCacheCreateTextureFromImage(
            kCFAllocatorDefault,
            textureCache,
            pixelBuffer,
            nil,
            pixelFormat,
            width,
            height,
            planeIndex,
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
}

private struct ImmersiveStereoUniforms {
    var video2DTo3D: SIMD4<Float>
    var video2DTo3DShape: SIMD4<Float>
}

extension ImmersiveStereoCompositor {
    /// Head-locked cinema screen in ARKit world space.
    /// Fits the original display aspect inside the compositor eye buffer (same framing as 2D `.fit`).
    fileprivate static func makeScreenModelMatrix(
        headTransform: simd_float4x4,
        videoAspect: Float,
        drawableAspect: Float?
    ) -> simd_float4x4 {
        let distanceMeters = ImmersiveScreenPlacement.resolvedScreenDistanceMeters()
        var translation = matrix_identity_float4x4
        translation.columns.3.z = -distanceMeters

        let safeVideoAspect = max(videoAspect, 0.1)
        var scaleX: Float = 1.0
        var scaleY: Float = 1.0 / safeVideoAspect

        if let drawableAspect, drawableAspect > 0 {
            if safeVideoAspect > drawableAspect {
                scaleX = 1.0
                scaleY = 1.0 / safeVideoAspect
            } else {
                scaleX = safeVideoAspect / drawableAspect
                scaleY = 1.0 / drawableAspect
            }
        }

        let sizeCompensation = ImmersiveScreenPlacement.sizeCompensation(for: distanceMeters)
        scaleX *= sizeCompensation
        scaleY *= sizeCompensation

        var scale = matrix_identity_float4x4
        scale.columns.0.x = scaleX
        scale.columns.1.y = scaleY

        return headTransform * translation * scale
    }

    fileprivate static func makeUniforms(
        configuration: Video2DTo3DRenderConfiguration,
        eye: StereoscopicVideoEye,
        usesDepthMap: Bool? = nil
    ) -> ImmersiveStereoUniforms {
        let depthMapActive = usesDepthMap ?? configuration.usesDepthMap
        guard configuration.isEnabled else {
            return ImmersiveStereoUniforms(
                video2DTo3D: SIMD4<Float>(1, 0.45, 0, 0),
                video2DTo3DShape: SIMD4<Float>(1.1, 0.9, 0, 0)
            )
        }
        let eyeSign: Float = eye == .left ? -1 : 1
        return ImmersiveStereoUniforms(
            video2DTo3D: SIMD4<Float>(1, configuration.depthStrength, depthMapActive ? 1 : 0, eyeSign),
            video2DTo3DShape: SIMD4<Float>(
                configuration.depthDistance,
                configuration.depthCurvature,
                configuration.depthSmoothingFactor,
                0
            )
        )
    }

    fileprivate static func placeholderUniforms(eye: StereoscopicVideoEye) -> ImmersiveStereoUniforms {
        let eyeSign: Float = eye == .left ? -1 : 1
        return ImmersiveStereoUniforms(
            video2DTo3D: SIMD4<Float>(1, 0.45, 0, eyeSign),
            video2DTo3DShape: SIMD4<Float>(1.1, 0.9, 0, 0)
        )
    }
}
#endif

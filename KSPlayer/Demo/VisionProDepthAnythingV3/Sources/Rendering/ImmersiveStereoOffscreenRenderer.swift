#if os(visionOS) && canImport(Metal)
import CoreVideo
import KSPlayer
@preconcurrency import Metal
import simd

/// Renders DA3 stereo frames into a side-by-side texture for RealityKit display.
final class ImmersiveStereoOffscreenRenderer: @unchecked Sendable {
    private struct PipelinePair {
        let nv12: MTLRenderPipelineState
        let bgra: MTLRenderPipelineState
    }

    private let device: MTLDevice
    private let commandQueue: MTLCommandQueue
    private let textureCache: CVMetalTextureCache
    private let metalLibrary: MTLLibrary
    private let worldVertexFunction: MTLFunction
    private let samplerState: MTLSamplerState
    private let neutralDepthTexture: MTLTexture
    private var pipelineCache: [MTLPixelFormat: PipelinePair] = [:]

    init?(device: MTLDevice? = MTLCreateSystemDefaultDevice()) {
        guard let device,
              let commandQueue = device.makeCommandQueue(),
              let library = try? device.makeDefaultLibrary(bundle: .main),
              let worldVertexFunction = library.makeFunction(name: "immersiveWorldVertex")
        else {
            return nil
        }

        self.device = device
        self.commandQueue = commandQueue

        var cache: CVMetalTextureCache?
        guard CVMetalTextureCacheCreate(kCFAllocatorDefault, nil, device, nil, &cache) == kCVReturnSuccess,
              let cache
        else {
            return nil
        }
        self.textureCache = cache
        self.metalLibrary = library
        self.worldVertexFunction = worldVertexFunction

        let samplerDescriptor = MTLSamplerDescriptor()
        samplerDescriptor.minFilter = .linear
        samplerDescriptor.magFilter = .linear
        guard let samplerState = device.makeSamplerState(descriptor: samplerDescriptor) else {
            return nil
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
            return nil
        }
        neutralDepthTexture.replace(
            region: MTLRegionMake2D(0, 0, 1, 1),
            mipmapLevel: 0,
            withBytes: &neutralDepth,
            bytesPerRow: MemoryLayout<Float>.stride
        )
        self.neutralDepthTexture = neutralDepthTexture
    }

    func renderSideBySide(stereoFrame: ImmersiveStereoFrame, into texture: MTLTexture) -> Bool {
        guard let commandBuffer = commandQueue.makeCommandBuffer() else {
            return false
        }

        let renderPassDescriptor = MTLRenderPassDescriptor()
        renderPassDescriptor.colorAttachments[0].texture = texture
        renderPassDescriptor.colorAttachments[0].loadAction = .clear
        renderPassDescriptor.colorAttachments[0].storeAction = .store
        renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColor(red: 0, green: 0, blue: 0, alpha: 0)

        guard let encoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) else {
            return false
        }

        let halfWidth = Double(texture.width / 2)
        let height = Double(texture.height)
        let identity = matrix_identity_float4x4

        encoder.setViewport(MTLViewport(originX: 0, originY: 0, width: halfWidth, height: height, znear: 0, zfar: 1))
        let drewLeft = drawVideo(
            encoder: encoder,
            stereoFrame: stereoFrame,
            eye: .left,
            modelViewProjection: identity,
            colorPixelFormat: texture.pixelFormat
        )

        encoder.setViewport(
            MTLViewport(originX: halfWidth, originY: 0, width: halfWidth, height: height, znear: 0, zfar: 1)
        )
        let drewRight = drawVideo(
            encoder: encoder,
            stereoFrame: stereoFrame,
            eye: .right,
            modelViewProjection: identity,
            colorPixelFormat: texture.pixelFormat
        )

        encoder.endEncoding()
        commandBuffer.commit()
        commandBuffer.waitUntilCompleted()
        return drewLeft || drewRight
    }

    func render(
        eye: StereoscopicVideoEye,
        stereoFrame: ImmersiveStereoFrame,
        into texture: MTLTexture,
        forceFlatPresentation: Bool = false
    ) -> Bool {
        guard let commandBuffer = commandQueue.makeCommandBuffer() else {
            return false
        }

        let renderPassDescriptor = MTLRenderPassDescriptor()
        renderPassDescriptor.colorAttachments[0].texture = texture
        renderPassDescriptor.colorAttachments[0].loadAction = .clear
        renderPassDescriptor.colorAttachments[0].storeAction = .store
        renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColor(red: 0, green: 0, blue: 0, alpha: 1)

        guard let encoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) else {
            return false
        }

        encoder.setViewport(
            MTLViewport(
                originX: 0,
                originY: 0,
                width: Double(texture.width),
                height: Double(texture.height),
                znear: 0,
                zfar: 1
            )
        )
        let drew = drawVideo(
            encoder: encoder,
            stereoFrame: stereoFrame,
            eye: eye,
            modelViewProjection: matrix_identity_float4x4,
            colorPixelFormat: texture.pixelFormat,
            forceFlatPresentation: forceFlatPresentation
        )
        encoder.endEncoding()
        commandBuffer.commit()
        commandBuffer.waitUntilCompleted()
        return drew
    }

    private func drawVideo(
        encoder: MTLRenderCommandEncoder,
        stereoFrame: ImmersiveStereoFrame,
        eye: StereoscopicVideoEye,
        modelViewProjection: simd_float4x4,
        colorPixelFormat: MTLPixelFormat,
        forceFlatPresentation: Bool = false
    ) -> Bool {
        guard let pipelines = pipeline(for: colorPixelFormat) else {
            return false
        }

        let configuration = stereoFrame.configuration
        let usesDepthMap = forceFlatPresentation
            ? false
            : ImmersiveStereoShaderUniforms.effectiveUsesDepthMap(
                configuration: configuration,
                depthTexture: stereoFrame.depthTexture,
                isDepthStaleForPresentation: stereoFrame.isDepthStaleForPresentation
            )
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
            encoder.setFragmentTexture(lumaTexture, index: 0)
            encoder.setFragmentTexture(chromaTexture, index: 1)
            encoder.setFragmentTexture(depthTexture, index: 2)
            encoder.setFragmentSamplerState(samplerState, index: 0)
            ImmersiveStereoYUVConversion.bind(to: encoder, pixelBuffer: stereoFrame.pixelBuffer)
            encoder.setFragmentBytes(&uniforms, length: MemoryLayout<ImmersiveStereoUniforms>.stride, index: 3)
            drawScreenQuad(encoder: encoder, pipeline: pipelines.nv12, modelViewProjection: modelViewProjection)
            return true
        }

        if isNV12(pixelFormat) {
            return false
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

    private func pipeline(for colorPixelFormat: MTLPixelFormat) -> PipelinePair? {
        if let cached = pipelineCache[colorPixelFormat] {
            return cached
        }
        guard let pair = try? makePipelinePair(colorPixelFormat: colorPixelFormat) else {
            return nil
        }
        pipelineCache[colorPixelFormat] = pair
        return pair
    }

    private func makePipelinePair(colorPixelFormat: MTLPixelFormat) throws -> PipelinePair {
        let usesRGBA16 = colorPixelFormat == .rgba16Float
        let nv12FunctionName = usesRGBA16 ? "immersiveNV12RGBA16Fragment" : "immersiveNV12Fragment"
        let bgraFunctionName = usesRGBA16 ? "immersiveRGBA16Fragment" : "immersiveBGRAFragment"

        guard let nv12Function = metalLibrary.makeFunction(name: nv12FunctionName),
              let bgraFunction = metalLibrary.makeFunction(name: bgraFunctionName) else {
            throw NSError(domain: "ImmersiveStereoOffscreenRenderer", code: 1)
        }

        let nv12Descriptor = MTLRenderPipelineDescriptor()
        nv12Descriptor.vertexFunction = worldVertexFunction
        nv12Descriptor.fragmentFunction = nv12Function
        nv12Descriptor.colorAttachments[0].pixelFormat = colorPixelFormat

        let bgraDescriptor = MTLRenderPipelineDescriptor()
        bgraDescriptor.vertexFunction = worldVertexFunction
        bgraDescriptor.fragmentFunction = bgraFunction
        bgraDescriptor.colorAttachments[0].pixelFormat = colorPixelFormat

        return PipelinePair(
            nv12: try device.makeRenderPipelineState(descriptor: nv12Descriptor),
            bgra: try device.makeRenderPipelineState(descriptor: bgraDescriptor)
        )
    }

    private func isNV12(_ pixelFormat: OSType) -> Bool {
        pixelFormat == kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange
            || pixelFormat == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
    }

    private func isPlanar420(_ pixelFormat: OSType) -> Bool {
        pixelFormat == kCVPixelFormatType_420YpCbCr8Planar
            || pixelFormat == kCVPixelFormatType_420YpCbCr8PlanarFullRange
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

    private static func makeUniforms(
        configuration: Video2DTo3DRenderConfiguration,
        eye: StereoscopicVideoEye,
        usesDepthMap: Bool
    ) -> ImmersiveStereoUniforms {
        let uniforms = ImmersiveStereoShaderUniforms.make(
            configuration: configuration,
            eye: eye,
            usesDepthMap: usesDepthMap
        )
        return ImmersiveStereoUniforms(
            video2DTo3D: uniforms.video2DTo3D,
            video2DTo3DShape: uniforms.video2DTo3DShape
        )
    }

}

private struct ImmersiveStereoUniforms {
    var video2DTo3D: SIMD4<Float>
    var video2DTo3DShape: SIMD4<Float>
}
#endif

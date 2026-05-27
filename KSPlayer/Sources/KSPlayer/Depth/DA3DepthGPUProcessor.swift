#if canImport(Metal)
@preconcurrency import Metal

public struct DA3DepthGPUProcessingParameters: Sendable {
    public var rawMinimum: Float
    public var rawMaximum: Float
    public var contrast: Float
    public var invertDepth: Bool
    public var temporalSmoothingFactor: Float

    public init(
        rawMinimum: Float,
        rawMaximum: Float,
        contrast: Float,
        invertDepth: Bool,
        temporalSmoothingFactor: Float
    ) {
        self.rawMinimum = rawMinimum
        self.rawMaximum = rawMaximum
        self.contrast = contrast
        self.invertDepth = invertDepth
        self.temporalSmoothingFactor = temporalSmoothingFactor
    }
}

public enum DA3DepthGPUProcessorError: Error, Equatable, Sendable {
    case metalUnavailable
    case pipelineUnavailable
    case invalidTextureSize(width: Int, height: Int)
}

/// GPU depth post-process (normalize + optional temporal smooth) for real-time 2D→3D.
public final class DA3DepthGPUProcessor: @unchecked Sendable {
    private struct NormalizeParameters {
        var depthMinimum: Float
        var depthMaximum: Float
        var contrast: Float
        var invertDepth: Float
    }

    private let device: any MTLDevice
    private let commandQueue: any MTLCommandQueue
    private let normalizePipeline: MTLComputePipelineState?
    private let smoothPipeline: MTLComputePipelineState?
    private let textureCache: ReusableDepthTextureCache
    private var previousTexture: (any MTLTexture)?

    public init(device: any MTLDevice, library: MTLLibrary? = nil) throws {
        guard let commandQueue = device.makeCommandQueue() else {
            throw DA3DepthGPUProcessorError.metalUnavailable
        }
        self.device = device
        self.commandQueue = commandQueue
        let resolvedLibrary = library ?? device.makeDefaultLibrary()
        if let normalizeFunction = resolvedLibrary?.makeFunction(name: "normalizeDepthTexture") {
            normalizePipeline = try? device.makeComputePipelineState(function: normalizeFunction)
        } else {
            normalizePipeline = nil
        }
        if let smoothFunction = resolvedLibrary?.makeFunction(name: "smoothDepthTexture") {
            smoothPipeline = try? device.makeComputePipelineState(function: smoothFunction)
        } else {
            smoothPipeline = nil
        }
        textureCache = ReusableDepthTextureCache(
            device: device,
            usage: [.shaderRead, .shaderWrite],
            textureCount: 4
        )
    }

    public func resetTemporalSmoothing() {
        previousTexture = nil
    }

    public func makeDisplayDepthTexture(
        source: any MTLTexture,
        parameters: DA3DepthGPUProcessingParameters
    ) -> (any MTLTexture)? {
        guard source.width > 0, source.height > 0 else {
            return nil
        }
        guard let normalized = normalize(source: source, parameters: parameters) else {
            return source
        }
        guard parameters.temporalSmoothingFactor > 0.001 else {
            previousTexture = normalized
            return normalized
        }
        guard let smoothed = smooth(current: normalized, factor: parameters.temporalSmoothingFactor) else {
            previousTexture = normalized
            return normalized
        }
        previousTexture = smoothed
        return smoothed
    }

    private func normalize(
        source: any MTLTexture,
        parameters: DA3DepthGPUProcessingParameters
    ) -> (any MTLTexture)? {
        guard let normalizePipeline,
              let commandBuffer = commandQueue.makeCommandBuffer(),
              let output = textureCache.texture(
                width: source.width,
                height: source.height,
                label: "da3DepthNormalized"
              ),
              let encoder = commandBuffer.makeComputeCommandEncoder()
        else {
            return nil
        }

        var gpuParameters = NormalizeParameters(
            depthMinimum: parameters.rawMinimum,
            depthMaximum: parameters.rawMaximum,
            contrast: parameters.contrast,
            invertDepth: parameters.invertDepth ? 1 : 0
        )

        encoder.setComputePipelineState(normalizePipeline)
        encoder.setTexture(source, index: 0)
        encoder.setTexture(output, index: 1)
        encoder.setBytes(&gpuParameters, length: MemoryLayout<NormalizeParameters>.stride, index: 0)
        let threadGroupSize = MTLSize(width: 16, height: 16, depth: 1)
        let threadGroups = MTLSize(
            width: (source.width + threadGroupSize.width - 1) / threadGroupSize.width,
            height: (source.height + threadGroupSize.height - 1) / threadGroupSize.height,
            depth: 1
        )
        encoder.dispatchThreadgroups(threadGroups, threadsPerThreadgroup: threadGroupSize)
        encoder.endEncoding()
        commandBuffer.commit()
        commandBuffer.waitUntilCompleted()
        return output
    }

    private func smooth(current: any MTLTexture, factor: Float) -> (any MTLTexture)? {
        guard let smoothPipeline,
              let commandBuffer = commandQueue.makeCommandBuffer(),
              let output = textureCache.texture(
                width: current.width,
                height: current.height,
                label: "da3DepthSmoothed"
              ),
              let encoder = commandBuffer.makeComputeCommandEncoder()
        else {
            return nil
        }

        let previous = compatiblePreviousTexture(for: current) ?? current
        var previousWeight = min(max(factor, 0), 0.95)
        var maxDisparityChange: Float = 0.08
        encoder.setComputePipelineState(smoothPipeline)
        encoder.setTexture(current, index: 0)
        encoder.setTexture(previous, index: 1)
        encoder.setTexture(output, index: 2)
        encoder.setBytes(&previousWeight, length: MemoryLayout<Float>.stride, index: 0)
        encoder.setBytes(&maxDisparityChange, length: MemoryLayout<Float>.stride, index: 1)
        let threadGroupSize = MTLSize(width: 16, height: 16, depth: 1)
        let threadGroups = MTLSize(
            width: (current.width + threadGroupSize.width - 1) / threadGroupSize.width,
            height: (current.height + threadGroupSize.height - 1) / threadGroupSize.height,
            depth: 1
        )
        encoder.dispatchThreadgroups(threadGroups, threadsPerThreadgroup: threadGroupSize)
        encoder.endEncoding()
        commandBuffer.commit()
        commandBuffer.waitUntilCompleted()
        return output
    }

    private func compatiblePreviousTexture(for current: any MTLTexture) -> (any MTLTexture)? {
        guard let previousTexture,
              previousTexture.width == current.width,
              previousTexture.height == current.height,
              previousTexture.pixelFormat == current.pixelFormat
        else {
            return nil
        }
        return previousTexture
    }
}

private final class ReusableDepthTextureCache {
    private let device: any MTLDevice
    private let usage: MTLTextureUsage
    private let textureCount: Int
    private var width = 0
    private var height = 0
    private var textures: [any MTLTexture] = []
    private var nextIndex = 0

    init(device: any MTLDevice, usage: MTLTextureUsage, textureCount: Int) {
        self.device = device
        self.usage = usage
        self.textureCount = max(textureCount, 2)
    }

    func texture(width: Int, height: Int, label: String) -> (any MTLTexture)? {
        guard width > 0, height > 0 else {
            return nil
        }
        if self.width != width || self.height != height || textures.isEmpty {
            let descriptor = MTLTextureDescriptor.texture2DDescriptor(
                pixelFormat: .r32Float,
                width: width,
                height: height,
                mipmapped: false
            )
            descriptor.usage = usage
            descriptor.storageMode = .shared
            textures = (0 ..< textureCount).compactMap { index in
                guard let texture = device.makeTexture(descriptor: descriptor) else {
                    return nil
                }
                texture.label = "\(label)-\(index)"
                return texture
            }
            self.width = width
            self.height = height
            nextIndex = 0
        }
        guard !textures.isEmpty else {
            return nil
        }
        let texture = textures[nextIndex]
        texture.label = label
        nextIndex = (nextIndex + 1) % textures.count
        return texture
    }
}
#endif

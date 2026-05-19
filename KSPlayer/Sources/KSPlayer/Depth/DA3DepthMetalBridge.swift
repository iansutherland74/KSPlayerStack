#if canImport(Metal)
@preconcurrency import Metal

public enum DA3DepthMetalBridgeError: Error, Equatable, LocalizedError, Sendable {
    case invalidFrameSize(width: Int, height: Int)
    case textureAllocationFailed(width: Int, height: Int)

    public var errorDescription: String? {
        switch self {
        case let .invalidFrameSize(width, height):
            return "Depth Anything V3 depth texture requires a positive size; got \(width)x\(height)."
        case let .textureAllocationFailed(width, height):
            return "Unable to allocate Depth Anything V3 \(width)x\(height) .r32Float depth texture."
        }
    }
}

public final class DA3DepthMetalBridge: @unchecked Sendable {
    private let device: any MTLDevice

    public init(device: any MTLDevice) {
        self.device = device
    }

    public func makeTexture(
        from frame: DA3DepthFrame,
        usage: MTLTextureUsage = [.shaderRead],
        label: String = "DepthAnythingV3Depth"
    ) throws -> any MTLTexture {
        guard frame.width > 0, frame.height > 0 else {
            throw DA3DepthMetalBridgeError.invalidFrameSize(width: frame.width, height: frame.height)
        }

        let descriptor = MTLTextureDescriptor.texture2DDescriptor(
            pixelFormat: .r32Float,
            width: frame.width,
            height: frame.height,
            mipmapped: false
        )
        descriptor.usage = usage
        descriptor.storageMode = .shared

        guard let texture = device.makeTexture(descriptor: descriptor) else {
            throw DA3DepthMetalBridgeError.textureAllocationFailed(width: frame.width, height: frame.height)
        }
        texture.label = label
        try Self.replace(texture, with: frame)
        return texture
    }

    public static func replace(_ texture: any MTLTexture, with frame: DA3DepthFrame) throws {
        guard frame.width > 0, frame.height > 0 else {
            throw DA3DepthMetalBridgeError.invalidFrameSize(width: frame.width, height: frame.height)
        }
        let expectedCount = frame.width * frame.height
        guard frame.values.count == expectedCount else {
            throw DA3DepthFrameError.invalidElementCount(expected: expectedCount, actual: frame.values.count)
        }

        frame.values.withUnsafeBytes { rawBuffer in
            guard let baseAddress = rawBuffer.baseAddress else {
                return
            }
            texture.replace(
                region: MTLRegionMake2D(0, 0, frame.width, frame.height),
                mipmapLevel: 0,
                withBytes: baseAddress,
                bytesPerRow: Self.bytesPerRow(forWidth: frame.width)
            )
        }
    }

    public static func bytesPerRow(forWidth width: Int) -> Int {
        width * MemoryLayout<Float>.stride
    }
}
#endif

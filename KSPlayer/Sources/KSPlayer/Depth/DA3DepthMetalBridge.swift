#if canImport(Metal)
@preconcurrency import Metal
#if canImport(CoreML)
@preconcurrency import CoreML
#endif

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
    private let lock = NSLock()
    private let cachedTextureCount = 3
    private var cachedWidth = 0
    private var cachedHeight = 0
    private var cachedUsage: MTLTextureUsage = []
    private var cachedTextures: [any MTLTexture] = []
    private var nextTextureIndex = 0

    public init(device: any MTLDevice) {
        self.device = device
    }

    public var metalDevice: any MTLDevice {
        device
    }

    public func usesSameDevice(as other: any MTLDevice) -> Bool {
        device.registryID == other.registryID
    }

    public func makeDisplayDepthTexture(
        depthOutput: DA3DepthOutput,
        processor: DA3DepthGPUProcessor,
        parameters: DA3DepthGPUProcessingParameters,
        usage: MTLTextureUsage = [.shaderRead],
        label: String = "DepthAnythingV3Depth"
    ) throws -> any MTLTexture {
        let rawTexture = try makeRawTexture(from: depthOutput, usage: [.shaderRead, .shaderWrite], label: "\(label)-raw")
        guard let displayTexture = processor.makeDisplayDepthTexture(
            source: rawTexture,
            parameters: parameters
        ) else {
            return rawTexture
        }
        return displayTexture
    }

    public func makeRawTexture(
        from depthOutput: DA3DepthOutput,
        usage: MTLTextureUsage = [.shaderRead],
        label: String = "DepthAnythingV3DepthRaw"
    ) throws -> any MTLTexture {
        #if canImport(CoreML)
        let texture = try reusableTexture(
            width: depthOutput.width,
            height: depthOutput.height,
            usage: usage,
            label: label
        )
        try Self.replace(texture, with: depthOutput.multiArray, width: depthOutput.width, height: depthOutput.height)
        return texture
        #else
        throw DA3DepthMetalBridgeError.textureAllocationFailed(width: depthOutput.width, height: depthOutput.height)
        #endif
    }

    public func makeTexture(
        from frame: DA3DepthFrame,
        usage: MTLTextureUsage = [.shaderRead],
        label: String = "DepthAnythingV3Depth"
    ) throws -> any MTLTexture {
        Depth3DDebug.log("Request DA3 depth texture \(frame.width)x\(frame.height) values=\(frame.values.count) usage=\(usage.rawValue)", phase: "depth-texture", verboseOnly: true)
        guard frame.width > 0, frame.height > 0 else {
            Depth3DDebug.fail("Invalid DA3 depth frame size \(frame.width)x\(frame.height)", phase: "depth-texture")
            throw DA3DepthMetalBridgeError.invalidFrameSize(width: frame.width, height: frame.height)
        }

        let texture = try reusableTexture(
            width: frame.width,
            height: frame.height,
            usage: usage,
            label: label
        )
        Depth3DDebug.assertTexture(texture, label: label, expectedPixelFormat: .r32Float, phase: "depth-texture")
        try Self.replace(texture, with: frame)
        return texture
    }

    private func reusableTexture(
        width: Int,
        height: Int,
        usage: MTLTextureUsage,
        label: String
    ) throws -> any MTLTexture {
        lock.lock()
        defer {
            lock.unlock()
        }

        if cachedWidth != width || cachedHeight != height || cachedUsage != usage || cachedTextures.isEmpty {
            cachedTextures = try (0 ..< cachedTextureCount).map { index in
                try makeTexture(width: width, height: height, usage: usage, label: "\(label)-\(index)")
            }
            cachedWidth = width
            cachedHeight = height
            cachedUsage = usage
            nextTextureIndex = 0
        }

        let texture = cachedTextures[nextTextureIndex]
        texture.label = label
        nextTextureIndex = (nextTextureIndex + 1) % cachedTextures.count
        return texture
    }

    private func makeTexture(
        width: Int,
        height: Int,
        usage: MTLTextureUsage,
        label: String
    ) throws -> any MTLTexture {
        let descriptor = MTLTextureDescriptor.texture2DDescriptor(
            pixelFormat: .r32Float,
            width: width,
            height: height,
            mipmapped: false
        )
        descriptor.usage = usage
        descriptor.storageMode = .shared

        guard let texture = device.makeTexture(descriptor: descriptor) else {
            Depth3DDebug.fail("Metal returned nil for DA3 depth texture \(width)x\(height) format \(descriptor.pixelFormat) usage \(usage.rawValue)", phase: "depth-texture")
            throw DA3DepthMetalBridgeError.textureAllocationFailed(width: width, height: height)
        }
        texture.label = label
        Depth3DDebug.log("Allocated DA3 depth texture \(label) \(width)x\(height) format \(texture.pixelFormat)", phase: "depth-texture", verboseOnly: true)
        return texture
    }

    public static func replace(_ texture: any MTLTexture, with frame: DA3DepthFrame) throws {
        Depth3DDebug.assertTexture(texture, label: texture.label ?? "DA3DepthTexture", expectedPixelFormat: .r32Float, phase: "depth-texture-replace")
        guard frame.width > 0, frame.height > 0 else {
            Depth3DDebug.fail("Invalid DA3 depth replace size \(frame.width)x\(frame.height)", phase: "depth-texture-replace")
            throw DA3DepthMetalBridgeError.invalidFrameSize(width: frame.width, height: frame.height)
        }
        let expectedCount = frame.width * frame.height
        guard frame.values.count == expectedCount else {
            Depth3DDebug.fail("Invalid DA3 depth values count expected \(expectedCount) actual \(frame.values.count)", phase: "depth-texture-replace")
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

    #if canImport(CoreML)
    public static func replace(
        _ texture: any MTLTexture,
        with multiArray: MLMultiArray,
        width: Int,
        height: Int
    ) throws {
        let shape = multiArray.shape.map(\.intValue)
        guard let dimensions = DA3DepthNumericRange.dimensions(for: shape),
              dimensions.width == width,
              dimensions.height == height
        else {
            throw DA3DepthFrameError.invalidShape(shape)
        }

        let strides = multiArray.strides.map(\.intValue)
        guard strides.count == shape.count else {
            throw DA3DepthFrameError.invalidStrideLayout(shape: shape, strides: strides)
        }

        let heightAxis = shape.count - 2
        let widthAxis = shape.count - 1
        let offsets = (0 ..< width * height).map { linearIndex in
            (linearIndex / width) * strides[heightAxis] + (linearIndex % width) * strides[widthAxis]
        }
        guard offsets.allSatisfy({ $0 >= 0 && $0 < multiArray.count }) else {
            let frame = try DA3DepthFrame(multiArray: multiArray, outputName: "depth")
            try replace(texture, with: frame)
            return
        }

        var uploadBuffer = [Float](repeating: 0, count: width * height)
        switch multiArray.dataType {
        case .float32:
            let pointer = multiArray.dataPointer.bindMemory(to: Float.self, capacity: multiArray.count)
            for index in uploadBuffer.indices {
                uploadBuffer[index] = pointer[offsets[index]]
            }
        case .float16:
            let pointer = multiArray.dataPointer.bindMemory(to: Float16.self, capacity: multiArray.count)
            for index in uploadBuffer.indices {
                uploadBuffer[index] = Float(pointer[offsets[index]])
            }
        case .double:
            let pointer = multiArray.dataPointer.bindMemory(to: Double.self, capacity: multiArray.count)
            for index in uploadBuffer.indices {
                uploadBuffer[index] = Float(pointer[offsets[index]])
            }
        default:
            let frame = try DA3DepthFrame(multiArray: multiArray, outputName: "depth")
            try replace(texture, with: frame)
            return
        }

        uploadBuffer.withUnsafeBytes { rawBuffer in
            guard let baseAddress = rawBuffer.baseAddress else {
                return
            }
            texture.replace(
                region: MTLRegionMake2D(0, 0, width, height),
                mipmapLevel: 0,
                withBytes: baseAddress,
                bytesPerRow: bytesPerRow(forWidth: width)
            )
        }
    }
    #endif
}
#endif

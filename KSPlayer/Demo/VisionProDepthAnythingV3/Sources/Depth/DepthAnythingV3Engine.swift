#if os(visionOS) && DEPTH_ANYTHING_V3_GENERATED && canImport(CoreML)
@preconcurrency import CoreML
@preconcurrency import CoreVideo
import KSPlayer

/// App-target engine for a Core ML Depth Anything V3 model named `DepthAnythingV3.mlmodel`.
public actor DepthAnythingV3Engine {
    private let model: DepthAnythingV3
    private let outputName: String

    public init(
        configuration: MLModelConfiguration = MLModelConfiguration(),
        outputName: String = "depth"
    ) throws {
        model = try DepthAnythingV3(configuration: configuration)
        self.outputName = outputName
    }

    public func makeDepthFrame(from pixelBuffer: CVPixelBuffer) throws -> DA3DepthFrame {
        try Task.checkCancellation()
        let input = DepthAnythingV3Input(image: pixelBuffer)
        let output: DepthAnythingV3Output = try model.prediction(input: input)
        try Task.checkCancellation()
        return try DA3DepthFrame(multiArray: output.depth, outputName: outputName)
    }
}
#endif

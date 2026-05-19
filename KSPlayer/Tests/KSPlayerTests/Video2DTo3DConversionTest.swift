@testable import KSPlayer
import CoreMedia
import CoreVideo
import XCTest

@MainActor
final class Video2DTo3DConversionTest: XCTestCase {
    final class FakeDepthProvider: VideoDepthEstimationProvider {
        let providerID = "fake-depth"
        var requestCount = 0

        func makeDepthMap(request _: VideoDepthEstimationRequest) throws -> VideoDepthMap? {
            requestCount += 1
            return VideoDepthMap(width: 2, height: 1, normalizedDisparity: [0.25, 0.75])
        }
    }

    final class FakeONNXBackend: DepthAnythingONNXInferenceBackend {
        var requestCount = 0

        func makeDepthPrediction(request _: VideoDepthEstimationRequest, configuration _: VideoDepthProcessingConfiguration) throws -> VideoDepthPrediction? {
            requestCount += 1
            return VideoDepthPrediction(width: 2, height: 1, values: [0, 1], metadata: ["runtime": "fake-onnx"])
        }
    }

    func testDepthMapClampsValuesAndRejectsInvalidDimensions() {
        let map = VideoDepthMap(width: 3, height: 1, normalizedDisparity: [-1, .nan, 2])

        XCTAssertEqual(map?.normalizedDisparity, [0, 0.5, 1])
        XCTAssertNil(VideoDepthMap(width: 2, height: 2, normalizedDisparity: [0.5]))
    }

    func testDepthStrengthClampsToSafeRange() {
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthStrength(-1), 0)
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthStrength(2), 1)
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthStrength(.nan), Video2DTo3DPolicy.defaultDepthStrength)
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthDistance(-1), 0)
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthDistance(3), 2)
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthDistance(.nan), Video2DTo3DPolicy.defaultDepthDistance)
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthCurvature(0), 0.25)
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthCurvature(4), 3)
        XCTAssertEqual(Video2DTo3DPolicy.validatedDepthCurvature(.nan), Video2DTo3DPolicy.defaultDepthCurvature)
    }

    func testPolicyEnablesPseudoStereoForFlatPlaneVideo() {
        let configuration = Video2DTo3DPolicy.renderConfiguration(
            mode: .pseudoStereo,
            depthStrength: 0.4,
            depthDistance: 1.25,
            depthCurvature: 0.75,
            outputLayout: .sideBySide,
            selectedEye: .right,
            display: .plane,
            stereoscopicVideoLayout: .mono,
            hasDepthMap: false
        )

        XCTAssertEqual(configuration.isEnabled, Video2DTo3DPolicy.isSupportedPlatform)
        if Video2DTo3DPolicy.isSupportedPlatform {
            XCTAssertFalse(configuration.usesDepthMap)
            XCTAssertEqual(configuration.outputLayout, .sideBySide)
            XCTAssertEqual(configuration.drawableSize(for: CGSize(width: 1920, height: 1080)), CGSize(width: 3840, height: 1080))
            XCTAssertEqual(configuration.depthDistance, 1.25)
            XCTAssertEqual(configuration.depthCurvature, 0.75)
            XCTAssertEqual(configuration.shapeUniform(), SIMD4<Float>(1.25, 0.75, 0, 0))
        }
    }

    func testPolicyUsesDepthMapOnlyWhenProvided() {
        let missingDepth = Video2DTo3DPolicy.renderConfiguration(
            mode: .depthMapPreferred,
            depthStrength: 0.5,
            depthDistance: 1,
            depthCurvature: 1,
            outputLayout: .selectedEye,
            selectedEye: .left,
            display: .plane,
            stereoscopicVideoLayout: .mono,
            hasDepthMap: false
        )
        let availableDepth = Video2DTo3DPolicy.renderConfiguration(
            mode: .depthMapPreferred,
            depthStrength: 0.5,
            depthDistance: 1,
            depthCurvature: 1,
            outputLayout: .selectedEye,
            selectedEye: .left,
            display: .plane,
            stereoscopicVideoLayout: .mono,
            hasDepthMap: true
        )

        XCTAssertEqual(missingDepth.isEnabled, Video2DTo3DPolicy.isSupportedPlatform)
        XCTAssertFalse(missingDepth.usesDepthMap)
        XCTAssertEqual(availableDepth.usesDepthMap, Video2DTo3DPolicy.isSupportedPlatform)
    }

    func testPolicyDisablesForExistingStereoAndPanoramaRendering() {
        XCTAssertFalse(Video2DTo3DPolicy.renderConfiguration(
            mode: .pseudoStereo,
            depthStrength: 0.5,
            depthDistance: 1,
            depthCurvature: 1,
            outputLayout: .selectedEye,
            selectedEye: .left,
            display: .plane,
            stereoscopicVideoLayout: .sideBySide,
            hasDepthMap: false
        ).isEnabled)
        XCTAssertFalse(Video2DTo3DPolicy.renderConfiguration(
            mode: .pseudoStereo,
            depthStrength: 0.5,
            depthDistance: 1,
            depthCurvature: 1,
            outputLayout: .selectedEye,
            selectedEye: .left,
            display: .vr,
            stereoscopicVideoLayout: .mono,
            hasDepthMap: false
        ).isEnabled)
    }

    func testOptionsRoute2DTo3DOffDisplayLayer() {
        let options = KSOptions()
        XCTAssertTrue(options.isUseDisplayLayer(dynamicRange: .sdr))

        options.video2DTo3DMode = .pseudoStereo

        XCTAssertEqual(options.isUseDisplayLayer(dynamicRange: .sdr), !Video2DTo3DPolicy.isSupportedPlatform)
    }

    func testUnsupportedPlatformProduces2DTo3DDiagnostic() {
        let options = KSOptions()
        options.video2DTo3DMode = .pseudoStereo

        let configuration = options.video2DTo3DRenderConfiguration(hasDepthMap: false)

        XCTAssertEqual(configuration.isEnabled, Video2DTo3DPolicy.isSupportedPlatform)
        if Video2DTo3DPolicy.isSupportedPlatform {
            XCTAssertNil(options.video2DTo3DDiagnostic)
        } else {
            XCTAssertEqual(options.video2DTo3DDiagnostic, .unavailable(reason: Video2DTo3DPolicy.unavailablePlatformReason))
        }
    }

    func testOptionsCarryDepthDistanceAndCurvatureControls() {
        let options = KSOptions()
        options.video2DTo3DMode = .pseudoStereo
        options.video2DTo3DDepthStrength = 0.4
        options.video2DTo3DDepthDistance = 3
        options.video2DTo3DDepthCurvature = 0

        let configuration = options.video2DTo3DRenderConfiguration(hasDepthMap: false)

        XCTAssertEqual(configuration.isEnabled, Video2DTo3DPolicy.isSupportedPlatform)
        if Video2DTo3DPolicy.isSupportedPlatform {
            XCTAssertEqual(configuration.depthStrength, 0.4)
            XCTAssertEqual(configuration.depthDistance, 2)
            XCTAssertEqual(configuration.depthCurvature, 0.25)
        }
    }

    func testDepthPostprocessorNormalizesInvalidValuesAndInverts() {
        XCTAssertEqual(
            VideoDepthPostprocessor.normalizedValues([10, 20, .nan, 30], normalization: .minMax),
            [0, 0.5, 0.5, 1]
        )
        XCTAssertEqual(
            VideoDepthPostprocessor.normalizedValues([10, 20, .nan, 30], normalization: .minMax, invertDepth: true),
            [1, 0.5, 0.5, 0]
        )
        XCTAssertEqual(
            VideoDepthPostprocessor.normalizedValues([0.25, -1, .infinity, 2], normalization: .none),
            [0.25, 0, 0.5, 1]
        )
    }

    func testDepthPostprocessorSmoothsDepthMaps() throws {
        let previous = try XCTUnwrap(VideoDepthMap(width: 2, height: 1, normalizedDisparity: [0, 1]))
        let prediction = try XCTUnwrap(VideoDepthPrediction(width: 2, height: 1, values: [1, 0]))
        let configuration = VideoDepthProcessingConfiguration(
            inputSize: nil,
            normalization: .none,
            maximumInferenceFPS: nil,
            temporalSmoothingFactor: 0.25
        )

        let map = try XCTUnwrap(VideoDepthPostprocessor.makeDepthMap(
            prediction: prediction,
            configuration: configuration,
            previousDepthMap: previous
        ))

        XCTAssertEqual(map.normalizedDisparity, [0.75, 0.25])
    }

    func testDepthAnythingModelFamiliesExposeLicensing() {
        XCTAssertTrue(DepthAnythingDepthEstimationAdapter.ModelVariant.small.licenseNote.contains("Apache-2.0"))
        XCTAssertTrue(DepthAnythingDepthEstimationAdapter.ModelVariant.base.licenseNote.contains("CC-BY-NC-4.0"))
        XCTAssertTrue(DepthAnythingDepthEstimationAdapter.ModelVariant.base.licenseNote(for: .depthAnything3).contains("Apache-2.0"))
        XCTAssertTrue(DepthAnythingDepthEstimationAdapter.ModelVariant.nestedGiantLarge11.licenseNote(for: .depthAnything3).contains("CC-BY-NC-4.0"))
        XCTAssertEqual(DepthAnythingDepthEstimationAdapter.ModelFamily.depthAnything3.displayName, "Depth Anything 3")
        XCTAssertTrue(DepthAnythingV2DepthEstimationAdapter.ModelVariant.small.licenseNote.contains("Apache-2.0"))
    }

    func testDepthAnythingONNXAdapterCarriesDA3Metadata() throws {
        let backend = FakeONNXBackend()
        let adapter = DepthAnythingDepthEstimationAdapter(
            providerID: "depth-anything-3-onnx",
            modelFamily: .depthAnything3,
            modelVariant: .small,
            modelURL: URL(fileURLWithPath: "/tmp/da3-small.onnx"),
            backend: backend
        )

        XCTAssertEqual(adapter.runtime, .onnx)
        XCTAssertEqual(adapter.modelFamily, .depthAnything3)
        XCTAssertEqual(adapter.modelVariant, .small)
        XCTAssertEqual(adapter.modelURL?.lastPathComponent, "da3-small.onnx")
    }

    func testDepthAnythingONNXRuntimeConfigurationSanitizesThreadsAndTensorValues() {
        let tensorConfiguration = DepthAnythingONNXTensorConfiguration(
            scale: .nan,
            mean: SIMD3<Float>(.nan, 0.5, 0.25),
            standardDeviation: SIMD3<Float>(0, .nan, 2)
        )
        let runtimeConfiguration = DepthAnythingONNXRuntimeConfiguration(
            runtimeLibraryURL: URL(fileURLWithPath: "/missing/libonnxruntime.dylib"),
            preferredInputName: "image",
            preferredOutputName: "depth",
            intraOpNumThreads: -1,
            interOpNumThreads: -2,
            tensorConfiguration: tensorConfiguration
        )

        XCTAssertEqual(runtimeConfiguration.intraOpNumThreads, 0)
        XCTAssertEqual(runtimeConfiguration.interOpNumThreads, 0)
        XCTAssertEqual(runtimeConfiguration.preferredInputName, "image")
        XCTAssertEqual(runtimeConfiguration.tensorConfiguration.scale, 1 / 255)
        XCTAssertEqual(runtimeConfiguration.tensorConfiguration.mean.x, 0.485)
        XCTAssertGreaterThan(runtimeConfiguration.tensorConfiguration.standardDeviation.x, 0)
    }

    func testDepthAnything3CoreMLAndONNXSetupConfiguration() {
        let coreMLConfiguration = VideoDepthProcessingConfiguration(
            inputSize: VideoDepthInputSize(width: 518, height: 518),
            normalization: .minMax,
            invertDepth: true,
            maximumInferenceFPS: 12,
            temporalSmoothingFactor: 0.6,
            preferredOutputFeatureName: "depth"
        )
        let onnxConfiguration = DepthAnythingONNXRuntimeConfiguration(
            preferredInputName: "image",
            preferredOutputName: "depth",
            tensorConfiguration: DepthAnythingONNXTensorConfiguration(layout: .nchw)
        )

        XCTAssertEqual(coreMLConfiguration.inputSize, VideoDepthInputSize(width: 518, height: 518))
        XCTAssertEqual(coreMLConfiguration.maximumInferenceFPS, 12)
        XCTAssertEqual(coreMLConfiguration.temporalSmoothingFactor, 0.6)
        XCTAssertEqual(coreMLConfiguration.preferredOutputFeatureName, "depth")
        XCTAssertTrue(coreMLConfiguration.invertDepth)
        XCTAssertEqual(onnxConfiguration.preferredInputName, "image")
        XCTAssertEqual(onnxConfiguration.preferredOutputName, "depth")
        XCTAssertEqual(onnxConfiguration.tensorConfiguration.layout, .nchw)
    }

    func testDepthAnythingONNXRuntimeReportsMissingRuntime() {
        #if canImport(CoreImage)
        let availability = DepthAnythingONNXRuntimeBackend.runtimeAvailability(
            runtimeLibraryURL: URL(fileURLWithPath: "/definitely/missing/libonnxruntime.dylib")
        )

        XCTAssertFalse(availability.isAvailable)
        XCTAssertNotNil(availability.reason)
        #endif
    }

    func testONNXDepthEstimationProviderThrowsWithoutRuntime() {
        #if canImport(CoreImage)
        XCTAssertThrowsError(try ONNXDepthEstimationProvider(
            modelURL: URL(fileURLWithPath: "/tmp/missing-depth-anything.onnx"),
            runtimeConfiguration: DepthAnythingONNXRuntimeConfiguration(
                runtimeLibraryURL: URL(fileURLWithPath: "/definitely/missing/libonnxruntime.dylib")
            )
        )) { error in
            XCTAssertTrue(error is VideoDepthEstimationError)
        }
        #endif
    }

    func testDepthAnythingONNXOutputPostprocessorUsesLastSpatialAxes() throws {
        let prediction = try XCTUnwrap(DepthAnythingONNXOutputPostprocessor.makeDepthPrediction(
            values: [0, 1, 2, 3, 4, 5],
            shape: [1, 1, 2, 3],
            outputName: "depth"
        ))

        XCTAssertEqual(prediction.width, 3)
        XCTAssertEqual(prediction.height, 2)
        XCTAssertEqual(prediction.values, [0, 1, 2, 3, 4, 5])
        XCTAssertEqual(prediction.metadata["output"], "depth")
    }

    func testDepthAnythingONNXPreprocessorCreatesNCHWTensor() throws {
        #if canImport(CoreImage)
        let tensor = try DepthAnythingONNXPreprocessor.makeInputTensor(
            from: makeRequest().pixelBuffer,
            processingConfiguration: VideoDepthProcessingConfiguration(
                inputSize: VideoDepthInputSize(width: 2, height: 2),
                aspectPolicy: .stretch,
                normalization: .none,
                maximumInferenceFPS: nil,
                temporalSmoothingFactor: 0
            ),
            tensorConfiguration: DepthAnythingONNXTensorConfiguration(
                layout: .nchw,
                scale: 1,
                mean: SIMD3<Float>(repeating: 0),
                standardDeviation: SIMD3<Float>(repeating: 1)
            )
        )

        XCTAssertEqual(tensor.shape, [1, 3, 2, 2])
        XCTAssertEqual(tensor.values.count, 12)
        XCTAssertTrue(tensor.values.allSatisfy(\.isFinite))
        #endif
    }

    func testDepthAnythingAdapterInvokesAppOwnedInference() throws {
        var receivedRequest = false
        let adapter = DepthAnythingV2DepthEstimationAdapter(modelVariant: .small) { request in
            receivedRequest = true
            XCTAssertEqual(Int(CVPixelBufferGetWidth(request.pixelBuffer)), 2)
            return VideoDepthMap.neutral(width: 2, height: 2)
        }
        let request = try makeRequest()

        if Video2DTo3DPolicy.isSupportedPlatform {
            let map = try adapter.makeDepthMap(request: request)
            XCTAssertTrue(receivedRequest)
            XCTAssertEqual(map, VideoDepthMap.neutral(width: 2, height: 2))
        } else {
            XCTAssertThrowsError(try adapter.makeDepthMap(request: request)) { error in
                XCTAssertEqual(error as? VideoDepthEstimationError, .unavailablePlatform(Video2DTo3DPolicy.unavailablePlatformReason))
            }
            XCTAssertFalse(receivedRequest)
        }
        XCTAssertTrue(DepthAnythingV2DepthEstimationAdapter.ModelVariant.small.licenseNote.contains("Apache-2.0"))
        XCTAssertTrue(DepthAnythingV2DepthEstimationAdapter.ModelVariant.base.licenseNote.contains("CC-BY-NC-4.0"))
    }

    func testDepthAnythingAdapterThrottlesAndSmoothsFakeInference() throws {
        var predictions = [
            VideoDepthPrediction(width: 2, height: 1, values: [0, 0]),
            VideoDepthPrediction(width: 2, height: 1, values: [1, 1]),
        ].compactMap { $0 }
        var requestCount = 0
        let adapter = DepthAnythingV2DepthEstimationAdapter(
            runtime: .custom,
            processingConfiguration: VideoDepthProcessingConfiguration(
                inputSize: nil,
                normalization: .none,
                maximumInferenceFPS: 2,
                temporalSmoothingFactor: 0.5
            ),
            allowsUnsupportedPlatformInference: true
        ) { _ in
            requestCount += 1
            return predictions.removeFirst()
        }

        let first = try adapter.makeDepthMap(request: makeRequest(time: CMTime(seconds: 0, preferredTimescale: 600)))
        let skipped = try adapter.makeDepthMap(request: makeRequest(time: CMTime(seconds: 0.25, preferredTimescale: 600)))
        let smoothed = try adapter.makeDepthMap(request: makeRequest(time: CMTime(seconds: 0.6, preferredTimescale: 600)))

        XCTAssertEqual(requestCount, 2)
        XCTAssertEqual(first?.normalizedDisparity, [0, 0])
        XCTAssertEqual(skipped?.normalizedDisparity, [0, 0])
        XCTAssertEqual(smoothed?.normalizedDisparity, [0.5, 0.5])
    }

    func testFakeDepthProviderConformsToPublicAPI() throws {
        let provider = FakeDepthProvider()

        let map = try provider.makeDepthMap(request: makeRequest())

        XCTAssertEqual(provider.requestCount, 1)
        XCTAssertEqual(map?.normalizedDisparity, [0.25, 0.75])
    }

    func testPreferredPlayerTypeRoutes2DTo3DToMEPlayer() throws {
        let originalFirstPlayerType = KSOptions.firstPlayerType
        defer {
            KSOptions.firstPlayerType = originalFirstPlayerType
        }
        KSOptions.firstPlayerType = KSAVPlayer.self

        let options = KSOptions()
        options.video2DTo3DMode = .depthMapPreferred
        options.videoDepthEstimationProvider = FakeDepthProvider()

        let playerType = KSPlayerLayer.preferredPlayerType(
            for: try XCTUnwrap(URL(string: "https://example.com/movie.mp4")),
            options: options
        )
        if Video2DTo3DPolicy.isSupportedPlatform {
            XCTAssertTrue(playerType == KSMEPlayer.self)
            XCTAssertNil(options.video2DTo3DDiagnostic)
        } else {
            XCTAssertTrue(playerType == KSAVPlayer.self)
            XCTAssertEqual(options.video2DTo3DDiagnostic, .unavailable(reason: Video2DTo3DPolicy.unavailablePlatformReason))
        }
    }

    func testPreferredPlayerTypeKeepsSeparateAudioVideoOnAVPlayerFor2DTo3D() throws {
        let options = KSOptions()
        options.video2DTo3DMode = .pseudoStereo
        let videoURL = try XCTUnwrap(URL(string: "https://example.com/movie.mp4"))
        let audioURL = try XCTUnwrap(URL(string: "https://example.com/audio.m4a"))

        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: videoURL, audioURL: audioURL, options: options) == KSAVPlayer.self)
    }

    private func makeRequest(time: CMTime = .zero) throws -> VideoDepthEstimationRequest {
        var pixelBuffer: CVPixelBuffer?
        let status = CVPixelBufferCreate(
            kCFAllocatorDefault,
            2,
            2,
            kCVPixelFormatType_32BGRA,
            nil,
            &pixelBuffer
        )
        XCTAssertEqual(status, kCVReturnSuccess)
        return VideoDepthEstimationRequest(
            pixelBuffer: try XCTUnwrap(pixelBuffer),
            presentationTime: time,
            naturalSize: CGSize(width: 2, height: 2),
            dynamicRange: .sdr
        )
    }
}

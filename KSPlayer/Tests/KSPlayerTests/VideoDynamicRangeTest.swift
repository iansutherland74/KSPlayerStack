@testable import KSPlayer
import AVFoundation
import CoreMedia
import XCTest

final class VideoDynamicRangeTest: XCTestCase {
    func testPQTransferFunctionClassifiesAsHDR10() throws {
        let formatDescription = try makeVideoFormatDescription(
            codecType: kCMVideoCodecType_HEVC,
            transferFunction: kCVImageBufferTransferFunction_SMPTE_ST_2084_PQ
        )

        XCTAssertEqual(formatDescription.dynamicRange, .hdr10)
    }

    func testHLGTransferFunctionClassifiesAsHLG() throws {
        let formatDescription = try makeVideoFormatDescription(
            codecType: kCMVideoCodecType_HEVC,
            transferFunction: kCVImageBufferTransferFunction_ITU_R_2100_HLG
        )

        XCTAssertEqual(formatDescription.dynamicRange, .hlg)
    }

    func testDolbyVisionCodecClassifiesAsDolbyVision() throws {
        let formatDescription = try makeVideoFormatDescription(
            codecType: kCMVideoCodecType_DolbyVisionHEVC,
            transferFunction: kCVImageBufferTransferFunction_SMPTE_ST_2084_PQ
        )

        XCTAssertEqual(formatDescription.dynamicRange, .dolbyVision)
    }

    func testDolbyVisionH1CodecClassifiesAsDolbyVision() throws {
        let formatDescription = try makeVideoFormatDescription(codecType: "dvh1".fourCharCode)

        XCTAssertEqual(formatDescription.dynamicRange, .dolbyVision)
    }

    func testUnspecifiedTransferFunctionDefaultsToSDR() throws {
        let formatDescription = try makeVideoFormatDescription(codecType: kCMVideoCodecType_HEVC)

        XCTAssertEqual(formatDescription.dynamicRange, .sdr)
    }

    func testHDR10PlusDiagnosticDoesNotClaimApplicationToneMapping() {
        let diagnostic = HDR10PlusPlaybackDiagnostic(
            renderPath: .metalRenderer,
            toneMappingPolicy: .systemManagedWhenAvailable,
            metadata: makeHDR10PlusMetadata()
        )

        XCTAssertFalse(diagnostic.appliesApplicationDynamicToneMapping)
        XCTAssertEqual(
            diagnostic.description,
            "HDR10+ metadata captured; Metal path preserves dynamic metadata and renders HDR10 fallback"
        )
    }

    func testHDR10PlusMetalToneMappingPolicyEnablesApplicationToneMapping() {
        let metadata = makeHDR10PlusMetadata()
        let diagnostic = HDR10PlusPlaybackDiagnostic(
            renderPath: .metalRenderer,
            toneMappingPolicy: .metalDynamicToneMapping,
            metadata: metadata
        )

        XCTAssertTrue(diagnostic.appliesApplicationDynamicToneMapping)
        XCTAssertEqual(
            diagnostic.description,
            "HDR10+ metadata captured; Metal path applies conservative dynamic tone mapping from ST 2094-40 knee/curve metadata"
        )
    }

    func testHDR10PlusDisplayLayerDiagnosticIsSystemConditional() {
        let diagnostic = HDR10PlusPlaybackDiagnostic(
            renderPath: .systemDisplayLayer,
            toneMappingPolicy: .systemManagedWhenAvailable
        )

        XCTAssertEqual(
            diagnostic.description,
            "HDR10+ metadata detected; dynamic tone mapping is preserved only if the system display path supports it"
        )
    }

    func testHDR10PlusStaticFallbackPolicyAvoidsDisplayLayer() {
        let options = KSOptions()
        options.hdr10PlusToneMappingPolicy = .staticHDR10Fallback

        XCTAssertFalse(options.isUseDisplayLayer(dynamicRange: .hdr10, hasHDR10PlusMetadata: true))
        XCTAssertTrue(options.isUseDisplayLayer(dynamicRange: .hdr10, hasHDR10PlusMetadata: false))
        XCTAssertEqual(
            options.hdr10PlusPlaybackDiagnostic(
                hasHDR10PlusMetadata: true,
                usesDisplayLayer: false,
                metadata: makeHDR10PlusMetadata()
            )?.description,
            "HDR10+ metadata captured; Metal path preserves dynamic metadata and renders HDR10 fallback"
        )
    }

    func testHDR10PlusMetalToneMappingPolicyAvoidsDisplayLayerAndBuildsUniform() throws {
        let options = KSOptions()
        options.hdr10PlusToneMappingPolicy = .metalDynamicToneMapping
        let metadata = makeHDR10PlusMetadata()

        XCTAssertFalse(options.isUseDisplayLayer(dynamicRange: .hdr10, hasHDR10PlusMetadata: true))
        let uniform = try XCTUnwrap(options.hdr10PlusMetalToneMappingUniform(metadata: metadata, dynamicRange: .hdr10))

        XCTAssertEqual(uniform.global.x, 1)
        XCTAssertEqual(uniform.global.y, 1)
        let window = try XCTUnwrap(uniform.windows.first)
        XCTAssertEqual(window.control.x, 1)
        XCTAssertEqual(window.control.y, 0.5)
        XCTAssertEqual(window.control.z, 0.4)
        XCTAssertEqual(window.control.w, 0.4)
        XCTAssertEqual(window.scene.x, 0.8)
        XCTAssertEqual(window.scene.y, 0.25)
        XCTAssertEqual(window.scene.z, 0.02)
        XCTAssertEqual(window.scene.w, 2.9, accuracy: 0.0001)
        XCTAssertEqual(window.extra.x, 1)
    }

    func testHDR10PlusMetalWindowLayoutMatchesShaderBuffer() {
        XCTAssertEqual(MemoryLayout<SIMD4<Float>>.stride, 16)
        XCTAssertEqual(MemoryLayout<HDR10PlusMetalToneMappingWindow>.stride, 96)
        XCTAssertEqual(HDR10PlusMetalToneMappingUniform.maxWindowCount, 64)
    }

    func testHDR10PlusMetalToneMappingUniformCarriesMultipleWindowsAndSaturation() throws {
        let metadata = makeMultiWindowHDR10PlusMetadata()
        let uniform = try XCTUnwrap(HDR10PlusMetalToneMappingUniform(metadata: metadata))

        XCTAssertEqual(uniform.global.y, 2)
        XCTAssertEqual(uniform.windows[0].control.x, 1)
        XCTAssertEqual(uniform.windows[0].region, SIMD4<Float>(0, 0, 1, 1))
        XCTAssertEqual(uniform.windows[1].control.x, 1)
        XCTAssertEqual(uniform.windows[1].control.y, 0.25)
        XCTAssertEqual(uniform.windows[1].control.z, 0.2)
        XCTAssertEqual(uniform.windows[1].region, SIMD4<Float>(0.1, 0.2, 0.6, 0.8))
        XCTAssertEqual(uniform.windows[1].extra.x, 1.25)
        XCTAssertEqual(uniform.windows[1].extra.z, Float(HDR10PlusMetadata.OverlapProcessOption.weightedAveraging.rawValue))
        XCTAssertEqual(uniform.windows[1].extra.w, 1)
        XCTAssertEqual(uniform.windows[1].selector.x, 0.35)
        XCTAssertEqual(uniform.windows[1].selector.y, 0.45)
        XCTAssertEqual(uniform.windows[1].selectorAxes, SIMD4<Float>(0.1, 0.05, 0.2, 0.1))
    }

    func testHDR10PlusLayeredOverlapModeIsCarriedToUniform() throws {
        let metadata = makeMultiWindowHDR10PlusMetadata(overlapProcessOption: .layering)
        let uniform = try XCTUnwrap(HDR10PlusMetalToneMappingUniform(metadata: metadata))

        XCTAssertEqual(uniform.windows[1].extra.z, Float(HDR10PlusMetadata.OverlapProcessOption.layering.rawValue))
    }

    func testHDR10PlusWindowCapacityIsBufferBackedBeyondThree() throws {
        let baseWindow = HDR10PlusMetadata.ProcessingWindow(
            maxSCL: [0.8],
            averageMaxRGB: 0.25,
            toneMapping: HDR10PlusMetadata.ToneMapping(kneePointX: 0.5, kneePointY: 0.4, bezierCurveAnchors: [0.2])
        )
        let metadata = HDR10PlusMetadata(
            applicationVersion: 0,
            targetedSystemDisplayMaximumLuminance: 1000,
            processingWindows: [baseWindow, baseWindow, baseWindow, baseWindow]
        )
        let uniform = try XCTUnwrap(HDR10PlusMetalToneMappingUniform(metadata: metadata))

        XCTAssertEqual(uniform.global.y, 4)
        XCTAssertEqual(uniform.windows.count, 4)
        XCTAssertFalse(uniform.isTruncated)
        XCTAssertEqual(uniform.windows[3].control.x, 1)
    }

    func testHDR10PlusWindowCapacityUsesPracticalBufferCap() throws {
        let baseWindow = HDR10PlusMetadata.ProcessingWindow(
            maxSCL: [0.8],
            averageMaxRGB: 0.25,
            toneMapping: HDR10PlusMetadata.ToneMapping(kneePointX: 0.5, kneePointY: 0.4, bezierCurveAnchors: [0.2])
        )
        let metadata = HDR10PlusMetadata(
            applicationVersion: 0,
            targetedSystemDisplayMaximumLuminance: 1000,
            processingWindows: Array(repeating: baseWindow, count: HDR10PlusMetalToneMappingUniform.maxWindowCount + 4)
        )
        let uniform = try XCTUnwrap(HDR10PlusMetalToneMappingUniform(metadata: metadata))

        XCTAssertEqual(uniform.global.y, Float(HDR10PlusMetalToneMappingUniform.maxWindowCount))
        XCTAssertEqual(uniform.windows.count, HDR10PlusMetalToneMappingUniform.maxWindowCount)
        XCTAssertTrue(uniform.isTruncated)
    }

    func testHDR10PlusPeakGridConstrainsUniform() throws {
        let metadata = makeHDR10PlusMetadata(
            targetedSystemDisplayActualPeakLuminance: HDR10PlusMetadata.LuminanceGrid(
                rows: 2,
                columns: 2,
                values: [0.05, 0.06, 0.07, 0.08]
            )
        )
        let uniform = try XCTUnwrap(HDR10PlusMetalToneMappingUniform(metadata: metadata))

        XCTAssertEqual(metadata.effectiveActualPeakMaximum, 0.08)
        XCTAssertEqual(uniform.global.z, 0.08)
        XCTAssertEqual(uniform.windows[0].extra.y, 0.08)
        XCTAssertEqual(uniform.windows[0].control.w, 0.4)
    }

    func testHDR10PlusMetalUniformFallsBackWithoutToneCurve() {
        let metadata = HDR10PlusMetadata(
            applicationVersion: 0,
            targetedSystemDisplayMaximumLuminance: 1000,
            processingWindows: [
                HDR10PlusMetadata.ProcessingWindow(maxSCL: [0.8], averageMaxRGB: 0.25),
            ]
        )

        XCTAssertNil(HDR10PlusMetalToneMappingUniform(metadata: metadata))
    }

    func testHDR10PlusMetadataSelectsPrimaryToneMappingParameters() throws {
        let metadata = makeHDR10PlusMetadata()
        let parameters = try XCTUnwrap(metadata.primaryToneMappingParameters)

        XCTAssertTrue(metadata.containsToneMappingCurve)
        XCTAssertTrue(parameters.hasDynamicCurve)
        XCTAssertEqual(parameters.targetedSystemDisplayMaximumLuminance, 1000)
        XCTAssertEqual(parameters.maxSCL, [0.8, 0.7, 0.6])
        XCTAssertEqual(parameters.averageMaxRGB, 0.25)
        XCTAssertEqual(parameters.fractionBrightPixels, 0.02)
        XCTAssertEqual(parameters.kneePointX, 0.5)
        XCTAssertEqual(parameters.kneePointY, 0.4)
        XCTAssertEqual(parameters.bezierCurveAnchors, [0.2, 0.3, 0.45])
    }

    func testHDR10PlusDiagnosticWithoutStructuredMetadataStaysFallback() {
        let diagnostic = HDR10PlusPlaybackDiagnostic(
            renderPath: .metalRenderer,
            toneMappingPolicy: .systemManagedWhenAvailable
        )

        XCTAssertNil(diagnostic.metadata)
        XCTAssertEqual(
            diagnostic.description,
            "HDR10+ metadata detected; Metal path renders HDR10 fallback because no structured metadata is available"
        )
    }

    private func makeHDR10PlusMetadata(
        targetedSystemDisplayActualPeakLuminance: HDR10PlusMetadata.LuminanceGrid? = nil
    ) -> HDR10PlusMetadata {
        HDR10PlusMetadata(
            applicationVersion: 0,
            targetedSystemDisplayMaximumLuminance: 1000,
            processingWindows: [
                HDR10PlusMetadata.ProcessingWindow(
                    maxSCL: [0.8, 0.7, 0.6],
                    averageMaxRGB: 0.25,
                    distributionMaxRGB: [
                        HDR10PlusMetadata.Percentile(percentage: 50, percentile: 0.3),
                        HDR10PlusMetadata.Percentile(percentage: 99, percentile: 0.9),
                    ],
                    fractionBrightPixels: 0.02,
                    toneMapping: HDR10PlusMetadata.ToneMapping(
                        kneePointX: 0.5,
                        kneePointY: 0.4,
                        bezierCurveAnchors: [0.2, 0.3, 0.45]
                    )
                ),
            ],
            targetedSystemDisplayActualPeakLuminance: targetedSystemDisplayActualPeakLuminance,
            rawSideData: Data([0x01, 0x02])
        )
    }

    private func makeMultiWindowHDR10PlusMetadata(
        overlapProcessOption: HDR10PlusMetadata.OverlapProcessOption = .weightedAveraging
    ) -> HDR10PlusMetadata {
        HDR10PlusMetadata(
            applicationVersion: 0,
            targetedSystemDisplayMaximumLuminance: 1000,
            processingWindows: [
                HDR10PlusMetadata.ProcessingWindow(
                    maxSCL: [0.8],
                    averageMaxRGB: 0.25,
                    toneMapping: HDR10PlusMetadata.ToneMapping(
                        kneePointX: 0.5,
                        kneePointY: 0.4,
                        bezierCurveAnchors: [0.2]
                    )
                ),
                HDR10PlusMetadata.ProcessingWindow(
                    bounds: HDR10PlusMetadata.WindowBounds(minX: 0.1, minY: 0.2, maxX: 0.6, maxY: 0.8),
                    selector: HDR10PlusMetadata.PixelSelector(
                        centerX: 0.35,
                        centerY: 0.45,
                        rotationRadians: 0,
                        semimajorAxisInternal: 0.1,
                        semimajorAxisExternal: 0.2,
                        semiminorAxisInternal: 0.05,
                        semiminorAxisExternal: 0.1
                    ),
                    overlapProcessOption: overlapProcessOption,
                    maxSCL: [0.5],
                    averageMaxRGB: 0.2,
                    toneMapping: HDR10PlusMetadata.ToneMapping(
                        kneePointX: 0.25,
                        kneePointY: 0.2,
                        bezierCurveAnchors: [0.1, 0.2]
                    ),
                    colorSaturationWeight: 1.25
                ),
            ]
        )
    }

    private func makeVideoFormatDescription(
        codecType: CMVideoCodecType,
        transferFunction: CFString? = nil
    ) throws -> CMVideoFormatDescription {
        var extensions = [CFString: Any]()
        if let transferFunction {
            extensions[kCVImageBufferColorPrimariesKey] = kCVImageBufferColorPrimaries_ITU_R_2020
            extensions[kCVImageBufferTransferFunctionKey] = transferFunction
            extensions[kCVImageBufferYCbCrMatrixKey] = kCVImageBufferYCbCrMatrix_ITU_R_2020
        }

        var formatDescription: CMVideoFormatDescription?
        let status = CMVideoFormatDescriptionCreate(
            allocator: kCFAllocatorDefault,
            codecType: codecType,
            width: 3840,
            height: 2160,
            extensions: extensions as CFDictionary,
            formatDescriptionOut: &formatDescription
        )
        XCTAssertEqual(status, noErr)
        return try XCTUnwrap(formatDescription)
    }
}

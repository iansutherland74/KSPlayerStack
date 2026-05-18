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

    func testUnspecifiedTransferFunctionDefaultsToSDR() throws {
        let formatDescription = try makeVideoFormatDescription(codecType: kCMVideoCodecType_HEVC)

        XCTAssertEqual(formatDescription.dynamicRange, .sdr)
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

@testable import KSPlayer
import CoreMedia
import VideoToolbox
import XCTest

final class VideoToolboxSampleDataTest: XCTestCase {
    func testDetectsThreeAndFourByteAnnexBStartCodes() {
        let threeByteStartCode: [UInt8] = [0x00, 0x00, 0x01, 0x65, 0x88]
        let fourByteStartCode: [UInt8] = [0x00, 0x00, 0x00, 0x01, 0x40, 0x01]

        XCTAssertTrue(threeByteStartCode.withUnsafeBufferPointer { buffer in
            VideoToolboxSampleData.isAnnexB(data: buffer.baseAddress!, size: buffer.count)
        })
        XCTAssertTrue(fourByteStartCode.withUnsafeBufferPointer { buffer in
            VideoToolboxSampleData.isAnnexB(data: buffer.baseAddress!, size: buffer.count)
        })
    }

    func testConvertsAnnexBToLengthPrefixedSample() throws {
        let annexB: [UInt8] = [
            0x00, 0x00, 0x00, 0x01, 0x67, 0x42, 0x80,
            0x00, 0x00, 0x01, 0x68, 0xce,
        ]

        let converted = try annexB.withUnsafeBufferPointer { buffer in
            try VideoToolboxSampleData.convertAnnexBToLengthPrefixed(data: buffer.baseAddress!, size: buffer.count)
        }

        XCTAssertEqual(Array(converted), [
            0x00, 0x00, 0x00, 0x03, 0x67, 0x42, 0x80,
            0x00, 0x00, 0x00, 0x02, 0x68, 0xce,
        ])
    }

    func testPreservesFourByteLengthPrefixedSampleThatLooksLikeThreeByteStartCode() throws {
        let payload = [UInt8](repeating: 0x55, count: 256)
        let lengthPrefixed = [UInt8]([0x00, 0x00, 0x01, 0x00]) + payload

        let converted = try lengthPrefixed.withUnsafeBufferPointer { buffer in
            try VideoToolboxSampleData.makeLengthPrefixedSample(
                data: buffer.baseAddress!,
                size: buffer.count,
                convertsThreeByteNALSize: false,
                codecType: kCMVideoCodecType_H264
            )
        }

        XCTAssertEqual(Array(converted), lengthPrefixed)
        XCTAssertTrue(lengthPrefixed.withUnsafeBufferPointer { buffer in
            VideoToolboxSampleData.isFourByteLengthPrefixedSample(data: buffer.baseAddress!, size: buffer.count)
        })
    }

    func testConvertsThreeByteNALSizeToFourByteNALSize() throws {
        let threeByteLengthPrefixed: [UInt8] = [
            0x00, 0x00, 0x02, 0xaa, 0xbb,
            0x00, 0x00, 0x01, 0xcc,
        ]

        let converted = try threeByteLengthPrefixed.withUnsafeBufferPointer { buffer in
            try VideoToolboxSampleData.convertThreeByteNALSizeToFourByte(data: buffer.baseAddress!, size: buffer.count)
        }

        XCTAssertEqual(Array(converted), [
            0x00, 0x00, 0x00, 0x02, 0xaa, 0xbb,
            0x00, 0x00, 0x00, 0x01, 0xcc,
        ])
    }

    func testAV1SampleDataPreservesBytesThatLookLikeStartCodes() throws {
        let av1Sample: [UInt8] = [
            0x12, 0x34, 0x00, 0x00, 0x01, 0x55,
            0xaa, 0xbb, 0x00, 0x00, 0x00, 0x01,
        ]

        let converted = try av1Sample.withUnsafeBufferPointer { buffer in
            try VideoToolboxSampleData.makeLengthPrefixedSample(
                data: buffer.baseAddress!,
                size: buffer.count,
                convertsThreeByteNALSize: true,
                codecType: kCMVideoCodecType_AV1
            )
        }

        XCTAssertEqual(Array(converted), av1Sample)
    }

    func testNALLengthPrefixConversionIsScopedToNALCodecs() {
        XCTAssertTrue(VideoToolboxSampleData.usesNALLengthPrefixes(codecType: kCMVideoCodecType_H264))
        XCTAssertTrue(VideoToolboxSampleData.usesNALLengthPrefixes(codecType: kCMVideoCodecType_HEVC))
        XCTAssertFalse(VideoToolboxSampleData.usesNALLengthPrefixes(codecType: kCMVideoCodecType_AV1))
        XCTAssertFalse(VideoToolboxSampleData.usesNALLengthPrefixes(codecType: kCMVideoCodecType_VP9))
    }

    func testVideoToolboxPolicyRequiresKnownCodecAndHardwareSupport() {
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.canAttemptAsynchronousDecompression(
            codecType: kCMVideoCodecType_H264,
            isHardwareDecodeSupported: { _ in true },
            isHardwareDecodeAllowedOnCurrentPlatform: { true }
        ))
        XCTAssertFalse(VideoToolboxHardwareDecodePolicy.canAttemptAsynchronousDecompression(
            codecType: kCMVideoCodecType_H264,
            isHardwareDecodeSupported: { _ in false },
            isHardwareDecodeAllowedOnCurrentPlatform: { true }
        ))
        XCTAssertFalse(VideoToolboxHardwareDecodePolicy.canAttemptAsynchronousDecompression(
            codecType: "zzzz".fourCharCode,
            isHardwareDecodeSupported: { _ in true },
            isHardwareDecodeAllowedOnCurrentPlatform: { true }
        ))
    }

    func testVideoToolboxPolicyReportsFallbackReasons() {
        XCTAssertEqual(
            VideoToolboxHardwareDecodePolicy.availability(
                codecType: kCMVideoCodecType_AV1,
                isHardwareDecodeSupported: { _ in true },
                isHardwareDecodeAllowedOnCurrentPlatform: { true }
            ),
            .supported
        )
        XCTAssertEqual(
            VideoToolboxHardwareDecodePolicy.availability(
                codecType: kCMVideoCodecType_AV1,
                isHardwareDecodeSupported: { _ in true },
                isHardwareDecodeAllowedOnCurrentPlatform: { false }
            ),
            .unsupported("hardware decode is unavailable on this platform")
        )
        XCTAssertEqual(
            VideoToolboxHardwareDecodePolicy.availability(
                codecType: kCMVideoCodecType_AV1,
                isHardwareDecodeSupported: { _ in false },
                isHardwareDecodeAllowedOnCurrentPlatform: { true }
            ),
            .unsupported("VideoToolbox reports no hardware decoder")
        )
    }

    func testVideoToolboxPolicyIncludesAppleHardwareCodecFamilies() {
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_H264))
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_HEVC))
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_VP9))
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_AV1))
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_AppleProRes422))
    }

    func testVideoToolboxPolicyBuildsDecoderSpecifications() throws {
        let h264Spec = try XCTUnwrap(
            VideoToolboxHardwareDecodePolicy.decoderSpecification(codecType: kCMVideoCodecType_H264)
        ) as NSDictionary
        XCTAssertEqual(
            h264Spec[kVTVideoDecoderSpecification_RequireHardwareAcceleratedVideoDecoder as String] as? Bool,
            true
        )
        XCTAssertNil(h264Spec[kVTVideoDecoderSpecification_EnableHardwareAcceleratedVideoDecoder as String])

        let hevcSpec = try XCTUnwrap(
            VideoToolboxHardwareDecodePolicy.decoderSpecification(codecType: kCMVideoCodecType_HEVC)
        ) as NSDictionary
        XCTAssertEqual(
            hevcSpec[kVTVideoDecoderSpecification_EnableHardwareAcceleratedVideoDecoder as String] as? Bool,
            true
        )
        XCTAssertNil(hevcSpec[kVTVideoDecoderSpecification_RequireHardwareAcceleratedVideoDecoder as String])

        let dolbyVisionSpec = try XCTUnwrap(
            VideoToolboxHardwareDecodePolicy.decoderSpecification(codecType: kCMVideoCodecType_DolbyVisionHEVC)
        ) as NSDictionary
        XCTAssertEqual(
            dolbyVisionSpec[kVTVideoDecoderSpecification_EnableHardwareAcceleratedVideoDecoder as String] as? Bool,
            true
        )

        XCTAssertNil(VideoToolboxHardwareDecodePolicy.decoderSpecification(codecType: "zzzz".fourCharCode))
    }
}

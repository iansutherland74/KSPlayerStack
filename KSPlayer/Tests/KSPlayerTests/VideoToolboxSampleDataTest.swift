@testable import KSPlayer
import CoreMedia
import XCTest

final class VideoToolboxSampleDataTest: XCTestCase {
    func testDetectsThreeAndFourByteAnnexBStartCodes() {
        var threeByteStartCode: [UInt8] = [0x00, 0x00, 0x01, 0x65, 0x88]
        var fourByteStartCode: [UInt8] = [0x00, 0x00, 0x00, 0x01, 0x40, 0x01]

        XCTAssertTrue(threeByteStartCode.withUnsafeBufferPointer { buffer in
            VideoToolboxSampleData.isAnnexB(data: buffer.baseAddress!, size: buffer.count)
        })
        XCTAssertTrue(fourByteStartCode.withUnsafeBufferPointer { buffer in
            VideoToolboxSampleData.isAnnexB(data: buffer.baseAddress!, size: buffer.count)
        })
    }

    func testConvertsAnnexBToLengthPrefixedSample() throws {
        var annexB: [UInt8] = [
            0x00, 0x00, 0x00, 0x01, 0x67, 0x42, 0x00,
            0x00, 0x00, 0x01, 0x68, 0xce,
        ]

        let converted = try annexB.withUnsafeBufferPointer { buffer in
            try VideoToolboxSampleData.convertAnnexBToLengthPrefixed(data: buffer.baseAddress!, size: buffer.count)
        }

        XCTAssertEqual(Array(converted), [
            0x00, 0x00, 0x00, 0x03, 0x67, 0x42, 0x00,
            0x00, 0x00, 0x00, 0x02, 0x68, 0xce,
        ])
    }

    func testConvertsThreeByteNALSizeToFourByteNALSize() throws {
        var threeByteLengthPrefixed: [UInt8] = [
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

    func testVideoToolboxPolicyRequiresKnownCodecAndHardwareSupport() {
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.canAttemptAsynchronousDecompression(codecType: kCMVideoCodecType_H264) { _ in true })
        XCTAssertFalse(VideoToolboxHardwareDecodePolicy.canAttemptAsynchronousDecompression(codecType: kCMVideoCodecType_H264) { _ in false })
        XCTAssertFalse(VideoToolboxHardwareDecodePolicy.canAttemptAsynchronousDecompression(codecType: "zzzz".fourCharCode) { _ in true })
    }

    func testVideoToolboxPolicyIncludesAppleHardwareCodecFamilies() {
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_H264))
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_HEVC))
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_VP9))
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_AV1))
        XCTAssertTrue(VideoToolboxHardwareDecodePolicy.knownHardwareCodecTypes.contains(kCMVideoCodecType_AppleProRes422))
    }
}

@testable import KSPlayer
import XCTest

final class DolbyVisionMetadataTest: XCTestCase {
    func testDolbyVisionConfigurationDescriptionIncludesProfileDiagnostics() {
        let record = makeRecord(profile: 5, compatibilityID: 0)

        XCTAssertEqual(
            record.description,
            "Dolby Vision profile 5 (HEVC single-layer modern streaming), level 6, rpu 1, " +
                "el 0, bl 1, compatibility 0, fallback Dolby Vision"
        )
    }

    func testDolbyVisionHDRFallbackDynamicRanges() {
        XCTAssertNil(makeRecord(profile: 0, compatibilityID: 0).hdrFallbackDynamicRange)
        XCTAssertNil(makeRecord(profile: 1, compatibilityID: 0).hdrFallbackDynamicRange)
        XCTAssertNil(makeRecord(profile: 2, compatibilityID: 0).hdrFallbackDynamicRange)
        XCTAssertNil(makeRecord(profile: 3, compatibilityID: 0).hdrFallbackDynamicRange)
        XCTAssertNil(makeRecord(profile: 4, compatibilityID: 0).hdrFallbackDynamicRange)
        XCTAssertEqual(makeRecord(profile: 5, compatibilityID: 0).hdrFallbackDynamicRange, .dolbyVision)
        XCTAssertNil(makeRecord(profile: 6, compatibilityID: 0).hdrFallbackDynamicRange)
        XCTAssertEqual(makeRecord(profile: 7, compatibilityID: 6).hdrFallbackDynamicRange, .hdr10)
        XCTAssertEqual(makeRecord(profile: 8, compatibilityID: 1).hdrFallbackDynamicRange, .hdr10)
        XCTAssertNil(makeRecord(profile: 8, compatibilityID: 2).hdrFallbackDynamicRange)
        XCTAssertEqual(makeRecord(profile: 8, compatibilityID: 4).hdrFallbackDynamicRange, .hlg)
        XCTAssertNil(makeRecord(profile: 9, compatibilityID: 0).hdrFallbackDynamicRange)
        XCTAssertNil(makeRecord(profile: 10, compatibilityID: 1).hdrFallbackDynamicRange)
        XCTAssertNil(makeRecord(profile: 11, compatibilityID: 1).hdrFallbackDynamicRange)
        XCTAssertNil(makeRecord(profile: 12, compatibilityID: 0).hdrFallbackDynamicRange)
        XCTAssertNil(makeRecord(profile: 13, compatibilityID: 0).hdrFallbackDynamicRange)
    }

    func testLegacySDRProfilesUseSourceMetadataFallbackDiagnostics() {
        XCTAssertEqual(makeRecord(profile: 2, compatibilityID: 0).fallbackDescription, "source SDR/base metadata")
        XCTAssertEqual(makeRecord(profile: 3, compatibilityID: 0).fallbackDescription, "source SDR/base metadata")
        XCTAssertEqual(makeRecord(profile: 4, compatibilityID: 0).fallbackDescription, "source SDR/base metadata")
        XCTAssertEqual(makeRecord(profile: 6, compatibilityID: 0).fallbackDescription, "source SDR/base metadata")
        XCTAssertEqual(makeRecord(profile: 8, compatibilityID: 2).fallbackDescription, "source SDR/base metadata")
    }

    func testProfile7EnhancementLayerDiagnosticAvoidsFELCompositionPromise() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true)

        XCTAssertEqual(
            record.enhancementLayerDescription,
            "enhancement layer present; MEL/FEL is not distinguished by this configuration record"
        )
        XCTAssertEqual(record.hdrFallbackDynamicRange, .hdr10)
    }

    func testStudioProfilesUseSourceMetadataFallbackDiagnostics() {
        XCTAssertEqual(makeRecord(profile: 10, compatibilityID: 1).fallbackDescription, "source/base metadata for studio profile")
        XCTAssertEqual(makeRecord(profile: 11, compatibilityID: 1).fallbackDescription, "source/base metadata for studio profile")
        XCTAssertEqual(makeRecord(profile: 12, compatibilityID: 0).fallbackDescription, "source/base metadata for studio profile")
        XCTAssertEqual(makeRecord(profile: 13, compatibilityID: 0).fallbackDescription, "source/base metadata for studio profile")
    }

    private func makeRecord(
        profile: UInt8,
        compatibilityID: UInt8,
        enhancementLayerPresent: Bool = false
    ) -> DOVIDecoderConfigurationRecord {
        DOVIDecoderConfigurationRecord(
            dv_version_major: 1,
            dv_version_minor: 0,
            dv_profile: profile,
            dv_level: 6,
            rpu_present_flag: 1,
            el_present_flag: enhancementLayerPresent ? 1 : 0,
            bl_present_flag: 1,
            dv_bl_signal_compatibility_id: compatibilityID
        )
    }
}

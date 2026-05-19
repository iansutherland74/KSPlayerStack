@testable import KSPlayer
import CoreMedia
import CoreVideo
import XCTest

final class DolbyVisionMetadataTest: XCTestCase {
    func testParsesDolbyVisionISOBMFFConfigurationRecord() throws {
        let data = isoBMFFRecordData(profile: 8, level: 6, rpu: true, enhancementLayer: false, baseLayer: true, compatibilityID: 1)

        let record = try XCTUnwrap(DOVIDecoderConfigurationRecord.isoBMFFRecord(data: data))

        XCTAssertEqual(record.dv_version_major, 1)
        XCTAssertEqual(record.dv_version_minor, 0)
        XCTAssertEqual(record.dv_profile, 8)
        XCTAssertEqual(record.dv_level, 6)
        XCTAssertEqual(record.rpu_present_flag, 1)
        XCTAssertEqual(record.el_present_flag, 0)
        XCTAssertEqual(record.bl_present_flag, 1)
        XCTAssertEqual(record.dv_bl_signal_compatibility_id, 1)
        XCTAssertEqual(record.hdrFallbackDynamicRange, .hdr10)
    }

    func testFormatDescriptionExposesDolbyVisionConfigurationAtoms() throws {
        let data = isoBMFFRecordData(profile: 11, level: 9, rpu: true, enhancementLayer: false, baseLayer: true, compatibilityID: 0)
        let formatDescription = try makeVideoFormatDescription(atomName: "dvwC", atomData: data)

        let record = try XCTUnwrap(formatDescription.dolbyVisionConfigurationRecord)

        XCTAssertEqual(record.dv_profile, 11)
        XCTAssertEqual(record.dv_level, 9)
        XCTAssertEqual(formatDescription.dynamicRange, .dolbyVision)
    }

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

    func testDecodedDolbyVisionFramesCarryOnlyKnownFallbackDynamicRanges() {
        let profile7Frame = VideoVTBFrame(
            fps: 24,
            isDovi: true,
            dolbyVisionFallbackDynamicRange: makeRecord(profile: 7, compatibilityID: 6).hdrFallbackDynamicRange
        )
        let profile84Frame = VideoVTBFrame(
            fps: 24,
            isDovi: true,
            dolbyVisionFallbackDynamicRange: makeRecord(profile: 8, compatibilityID: 4).hdrFallbackDynamicRange
        )
        let profile11Frame = VideoVTBFrame(
            fps: 24,
            isDovi: true,
            dolbyVisionFallbackDynamicRange: makeRecord(profile: 11, compatibilityID: 1).hdrFallbackDynamicRange
        )

        XCTAssertEqual(profile7Frame.dolbyVisionFallbackDynamicRange, .hdr10)
        XCTAssertEqual(profile84Frame.dolbyVisionFallbackDynamicRange, .hlg)
        XCTAssertNil(profile11Frame.dolbyVisionFallbackDynamicRange)
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
            "enhancement layer present; MEL/FEL requires RPU/EL packet diagnostics and FEL residuals are not composed"
        )
        XCTAssertEqual(record.hdrFallbackDynamicRange, .hdr10)
        XCTAssertEqual(record.fallbackDescription, "HDR10 base layer; Profile 7 enhancement layer is diagnostic-only")
        XCTAssertEqual(
            record.enhancementLayerCompositionSupport.description,
            "Profile 7 enhancement layer detected; KSPlayer has no dual-layer EL decoder/compositor, " +
                "so FEL residual detail is not composed"
        )
        XCTAssertEqual(
            record.playbackDescription,
            "Dolby Vision profile 7 (Profile 7 enhancement layer kind unknown; HDR10 base-layer fallback)"
        )
    }

    func testProfile7BaseLayerFallbackIsExplicitWithoutEnhancementLayer() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: false)

        XCTAssertEqual(record.enhancementLayerCompositionSupport, .notRequired)
        XCTAssertEqual(record.profile7EnhancementLayerKind, .none)
        XCTAssertEqual(record.playbackCapability(), .baseLayerHDR10Fallback)
        XCTAssertEqual(record.fallbackDescription, "HDR10 base layer")
        XCTAssertEqual(record.playbackDescription, "Dolby Vision profile 7 (HDR10 base-layer fallback)")
    }

    func testProfile7MELCompatibleCapabilityFromFrameMetadataDiagnostic() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true)

        let diagnostic = record.playbackDiagnostic.merging(
            rawRPUData: Data([0x01, 0x02, 0x03]),
            hasFrameDOVIMetadata: true,
            enhancementLayerKind: .minimumEnhancementLayer
        )

        XCTAssertEqual(diagnostic.enhancementLayerKind, .minimumEnhancementLayer)
        XCTAssertEqual(diagnostic.capability, .minimumEnhancementLayerFallback)
        XCTAssertTrue(diagnostic.hasRPUMetadata)
        XCTAssertEqual(
            diagnostic.description,
            "Dolby Vision profile 7; MEL-compatible HDR10 base-layer fallback with RPU metadata preserved; " +
                "Profile 7 MEL-compatible enhancement layer; FEL composition not required; RPU metadata present"
        )
    }

    func testProfile7FELCapabilityDoesNotClaimComposition() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true)

        let diagnostic = record.playbackDiagnostic.merging(
            observedRPUNALUnitCount: 1,
            observedEnhancementLayerNALUnitCount: 2,
            observedEnhancementLayerVCLNALUnitCount: 2,
            largestEnhancementLayerVCLPayloadSize: 2048,
            enhancementLayerKind: .fullEnhancementLayer
        )

        XCTAssertEqual(diagnostic.enhancementLayerKind, .fullEnhancementLayer)
        XCTAssertEqual(diagnostic.capability, .fullEnhancementLayerCompositionUnavailable)
        XCTAssertEqual(diagnostic.largestEnhancementLayerVCLPayloadSize, 2048)
        XCTAssertEqual(
            diagnostic.description,
            "Dolby Vision profile 7; FEL full composition unavailable; HDR10 base-layer fallback; " +
                "Profile 7 FEL enhancement layer; \(DolbyVisionPlaybackDiagnostic.missingOpenFELCompositorReason); " +
                "observed EL NALs 2; observed RPU NALs 1"
        )
    }

    func testHEVCSampleSplitterSeparatesBaseEnhancementAndRPU() {
        let baseLayerNAL = hevcNAL(type: 1, layerID: 0, payload: [0xaa, 0xbb])
        let rpuNAL = hevcNAL(type: 62, layerID: 0, payload: [0x7a, 0x01])
        let enhancementLayerNAL = hevcNAL(type: 1, layerID: 1, payload: [0x11, 0x22, 0x33])
        let sample = lengthPrefixedSample([baseLayerNAL, rpuNAL, enhancementLayerNAL])

        let split = sample.withUnsafeBytes { bytes in
            DolbyVisionHEVCSampleInspector.split(
                data: bytes.bindMemory(to: UInt8.self).baseAddress!,
                size: sample.count,
                nalLengthSize: 4
            )
        }

        let diagnostics = split.diagnostics
        XCTAssertEqual(diagnostics.rpuNALUnitCount, 1)
        XCTAssertEqual(diagnostics.enhancementLayerNALUnitCount, 1)
        XCTAssertEqual(diagnostics.enhancementLayerVCLNALUnitCount, 1)
        XCTAssertEqual(diagnostics.largestEnhancementLayerVCLPayloadSize, 3)
        XCTAssertEqual(diagnostics.rawRPUData, Data(rpuNAL))
        XCTAssertTrue(diagnostics.hasSeparatedFELInputs)
        XCTAssertEqual(split.baseLayerSample, lengthPrefixedSample([baseLayerNAL]))
        XCTAssertEqual(split.enhancementLayerSample, lengthPrefixedSample([enhancementLayerNAL]))
        XCTAssertEqual(split.rpuNALUnits, [Data(rpuNAL)])
    }

    func testFELCompositionPlannerReportsSeparatedInputsButNoFullCompositor() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true)
        let baseLayerNAL = hevcNAL(type: 1, layerID: 0, payload: [0xaa])
        let rpuNAL = hevcNAL(type: 62, layerID: 0, payload: [0x7a])
        let enhancementLayerNAL = hevcNAL(type: 1, layerID: 1, payload: [0x11])
        let sample = lengthPrefixedSample([baseLayerNAL, rpuNAL, enhancementLayerNAL])

        let split = sample.withUnsafeBytes { bytes in
            DolbyVisionHEVCSampleInspector.split(
                data: bytes.bindMemory(to: UInt8.self).baseAddress!,
                size: sample.count,
                nalLengthSize: 4
            )
        }

        XCTAssertEqual(
            DolbyVisionFELCompositionPlanner.state(
                configuration: record,
                split: split,
                enhancementLayerKind: .unknown
            ),
            .separatedInputsAvailable
        )
        XCTAssertEqual(
            DolbyVisionFELCompositionPlanner.state(
                configuration: record,
                split: split,
                enhancementLayerKind: .fullEnhancementLayer
            ),
            .unavailable(reason: DolbyVisionPlaybackDiagnostic.missingOpenFELCompositorReason)
        )
    }

    func testFELCompositionPlannerCanReportFutureBackendAvailability() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true)
        let baseLayerNAL = hevcNAL(type: 1, layerID: 0, payload: [0xaa])
        let rpuNAL = hevcNAL(type: 62, layerID: 0, payload: [0x7a])
        let enhancementLayerNAL = hevcNAL(type: 1, layerID: 1, payload: [0x11])
        let sample = lengthPrefixedSample([baseLayerNAL, rpuNAL, enhancementLayerNAL])
        let split = sample.withUnsafeBytes { bytes in
            DolbyVisionHEVCSampleInspector.split(
                data: bytes.bindMemory(to: UInt8.self).baseAddress!,
                size: sample.count,
                nalLengthSize: 4
            )
        }

        let state = DolbyVisionFELCompositionPlanner.state(
            configuration: record,
            split: split,
            enhancementLayerKind: .fullEnhancementLayer,
            compositorAvailability: .available(backend: "TestOpenFEL")
        )
        let diagnostic = record.playbackDiagnostic.merging(
            enhancementLayerKind: .fullEnhancementLayer,
            felCompositionState: state
        )

        XCTAssertEqual(state, .available(backend: "TestOpenFEL"))
        XCTAssertTrue(state.isFullCompositionAvailable)
        XCTAssertEqual(diagnostic.capability, .fullEnhancementLayerComposition)
    }

    func testFELCompositionPlannerAllowsFallbackByDefaultWhenBackendIsMissing() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true)
        let split = makeSeparatedFELSplit()

        let state = DolbyVisionFELCompositionPlanner.state(
            configuration: record,
            split: split,
            enhancementLayerKind: .fullEnhancementLayer,
            playbackPolicy: .allowBaseLayerFallback
        )
        let diagnostic = record.playbackDiagnostic.merging(
            enhancementLayerKind: .fullEnhancementLayer,
            felCompositionState: state
        )

        XCTAssertEqual(state, .unavailable(reason: DolbyVisionPlaybackDiagnostic.missingOpenFELCompositorReason))
        XCTAssertFalse(diagnostic.blocksPlayback)
        XCTAssertEqual(diagnostic.capability, .fullEnhancementLayerCompositionUnavailable)
    }

    func testFELCompositionPlannerBlocksStrictPolicyWhenBackendIsMissing() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true)
        let split = makeSeparatedFELSplit()

        let state = DolbyVisionFELCompositionPlanner.state(
            configuration: record,
            split: split,
            enhancementLayerKind: .fullEnhancementLayer,
            playbackPolicy: .requireFullComposition
        )
        let diagnostic = record.playbackDiagnostic.merging(
            enhancementLayerKind: .fullEnhancementLayer,
            felCompositionState: state,
            felPlaybackPolicy: .requireFullComposition
        )

        XCTAssertEqual(state, .requiredUnavailable(reason: DolbyVisionPlaybackDiagnostic.requiredFELCompositorReason))
        XCTAssertTrue(diagnostic.blocksPlayback)
        XCTAssertEqual(diagnostic.capability, .fullEnhancementLayerCompositionRequiredUnavailable)
        XCTAssertTrue(diagnostic.description.contains("playback unsupported"))
    }

    func testFELCompositionPlannerStrictPolicyAllowsAvailableBackend() {
        let record = makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true)
        let split = makeSeparatedFELSplit()

        let state = DolbyVisionFELCompositionPlanner.state(
            configuration: record,
            split: split,
            enhancementLayerKind: .fullEnhancementLayer,
            compositorAvailability: .available(backend: "FakeFelBaker"),
            playbackPolicy: .requireFullComposition
        )
        let diagnostic = record.playbackDiagnostic.merging(
            enhancementLayerKind: .fullEnhancementLayer,
            felCompositionState: state,
            felPlaybackPolicy: .requireFullComposition
        )

        XCTAssertEqual(state, .available(backend: "FakeFelBaker"))
        XCTAssertFalse(diagnostic.blocksPlayback)
        XCTAssertEqual(diagnostic.capability, .fullEnhancementLayerComposition)
    }

    func testOptionsExposeFutureFELCompositorBackendAvailability() {
        let options = KSOptions()

        XCTAssertEqual(
            options.dolbyVisionFELCompositorAvailability,
            .unavailable(reason: DolbyVisionPlaybackDiagnostic.missingOpenFELCompositorReason)
        )

        options.dolbyVisionFELCompositorBackend = TestFELCompositorBackend()

        XCTAssertEqual(options.dolbyVisionFELCompositorAvailability, .available(backend: "TestOpenFEL"))
    }

    func testOptionsExposeStrictFELPlaybackPolicy() {
        let options = KSOptions()

        XCTAssertEqual(options.dolbyVisionFELPlaybackPolicy, .allowBaseLayerFallback)

        options.dolbyVisionFELPlaybackPolicy = .requireFullComposition

        XCTAssertEqual(options.dolbyVisionFELPlaybackPolicy, .requireFullComposition)
    }

    func testFelBakerBackendAdapterUsesFakeShim() throws {
        let shim = TestFelBakerShim()
        let backend = FelBakerDolbyVisionFELCompositorBackend(shim: shim, outputPixelFormat: kCVPixelFormatType_32BGRA)
        let input = DolbyVisionFELCompositorInput(
            baseLayerPixelBuffer: try makePixelBuffer(),
            enhancementLayerPixelBuffer: try makePixelBuffer(),
            rpuData: Data([0x01, 0x02, 0x03]),
            presentationTime: CMTime(value: 42, timescale: 24),
            diagnostic: makeRecord(profile: 7, compatibilityID: 6, enhancementLayerPresent: true).playbackDiagnostic
        )

        let output = try backend.compose(input: input)

        XCTAssertEqual(backend.availability, .available(backend: "FakeFelBaker"))
        XCTAssertEqual(CVPixelBufferGetPixelFormatType(output.pixelBuffer), kCVPixelFormatType_32BGRA)
        XCTAssertEqual(output.diagnosticMessage, "fake FEL compose")
        XCTAssertEqual(shim.lastRPUData, Data([0x01, 0x02, 0x03]))
        XCTAssertEqual(shim.lastPresentationTime, CMTime(value: 42, timescale: 24))
    }

    func testFELFrameAlignmentMatchesEnhancementByTimestamp() throws {
        let baseLayerNAL = hevcNAL(type: 1, layerID: 0, payload: [0xaa])
        let rpuNAL = hevcNAL(type: 62, layerID: 0, payload: [0x7a])
        let enhancementLayerNAL = hevcNAL(type: 1, layerID: 1, payload: [0x11])
        let baseSample = lengthPrefixedSample([baseLayerNAL])
        let enhancementSample = lengthPrefixedSample([rpuNAL, enhancementLayerNAL])
        var queue = DolbyVisionFELFrameAlignmentQueue()

        let baseSplit = baseSample.withUnsafeBytes { bytes in
            DolbyVisionHEVCSampleInspector.split(
                data: bytes.bindMemory(to: UInt8.self).baseAddress!,
                size: baseSample.count,
                nalLengthSize: 4
            )
        }
        let enhancementSplit = enhancementSample.withUnsafeBytes { bytes in
            DolbyVisionHEVCSampleInspector.split(
                data: bytes.bindMemory(to: UInt8.self).baseAddress!,
                size: enhancementSample.count,
                nalLengthSize: 4
            )
        }

        queue.enqueueEnhancement(timestamp: 42, split: enhancementSplit)
        let aligned = try XCTUnwrap(queue.alignBase(timestamp: 42, duration: 1, split: baseSplit))

        XCTAssertEqual(aligned.timestamp, 42)
        XCTAssertEqual(aligned.duration, 1)
        XCTAssertEqual(aligned.baseLayerSample, baseSample)
        XCTAssertEqual(aligned.enhancementLayerSample, lengthPrefixedSample([enhancementLayerNAL]))
        XCTAssertEqual(aligned.rpuNALUnits, [Data(rpuNAL)])
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

    private func isoBMFFRecordData(
        profile: UInt8,
        level: UInt8,
        rpu: Bool,
        enhancementLayer: Bool,
        baseLayer: Bool,
        compatibilityID: UInt8
    ) -> Data {
        let packed = UInt16(profile & 0x7f) << 9 |
            UInt16(level & 0x3f) << 3 |
            UInt16(rpu ? 1 : 0) << 2 |
            UInt16(enhancementLayer ? 1 : 0) << 1 |
            UInt16(baseLayer ? 1 : 0)
        return Data([
            1,
            0,
            UInt8((packed >> 8) & 0xff),
            UInt8(packed & 0xff),
            (compatibilityID & 0x0f) << 4,
        ])
    }

    private func hevcNAL(type: UInt8, layerID: UInt8, payload: [UInt8]) -> [UInt8] {
        [
            (type << 1) | ((layerID >> 5) & 0x01),
            ((layerID & 0x1f) << 3) | 0x01,
        ] + payload
    }

    private func lengthPrefixedSample(_ nalUnits: [[UInt8]]) -> Data {
        nalUnits.reduce(into: Data()) { data, nalUnit in
            var length = UInt32(nalUnit.count).bigEndian
            data.append(Data(bytes: &length, count: MemoryLayout<UInt32>.size))
            data.append(contentsOf: nalUnit)
        }
    }

    private func makeSeparatedFELSplit() -> DolbyVisionHEVCSampleSplit {
        let baseLayerNAL = hevcNAL(type: 1, layerID: 0, payload: [0xaa])
        let rpuNAL = hevcNAL(type: 62, layerID: 0, payload: [0x7a])
        let enhancementLayerNAL = hevcNAL(type: 1, layerID: 1, payload: [0x11])
        let sample = lengthPrefixedSample([baseLayerNAL, rpuNAL, enhancementLayerNAL])
        return sample.withUnsafeBytes { bytes in
            DolbyVisionHEVCSampleInspector.split(
                data: bytes.bindMemory(to: UInt8.self).baseAddress!,
                size: sample.count,
                nalLengthSize: 4
            )
        }
    }

    private func makePixelBuffer() throws -> CVPixelBuffer {
        let attributes: [String: Any] = [
            kCVPixelBufferMetalCompatibilityKey as String: true,
            kCVPixelBufferIOSurfacePropertiesKey as String: [String: String](),
        ]
        var pixelBuffer: CVPixelBuffer?
        let status = CVPixelBufferCreate(
            kCFAllocatorDefault,
            4,
            4,
            kCVPixelFormatType_32BGRA,
            attributes as CFDictionary,
            &pixelBuffer
        )
        XCTAssertEqual(status, kCVReturnSuccess)
        return try XCTUnwrap(pixelBuffer)
    }

    private func makeVideoFormatDescription(atomName: String, atomData: Data) throws -> CMVideoFormatDescription {
        var formatDescription: CMVideoFormatDescription?
        let extensions = [
            kCMFormatDescriptionExtension_SampleDescriptionExtensionAtoms: [
                atomName: atomData,
            ],
        ] as CFDictionary
        let status = CMVideoFormatDescriptionCreate(
            allocator: kCFAllocatorDefault,
            codecType: "dvh1".fourCharCode,
            width: 3840,
            height: 2160,
            extensions: extensions,
            formatDescriptionOut: &formatDescription
        )
        XCTAssertEqual(status, noErr)
        return try XCTUnwrap(formatDescription)
    }
}

private final class TestFELCompositorBackend: DolbyVisionFELCompositorBackend {
    let availability: DolbyVisionFELCompositorAvailability = .available(backend: "TestOpenFEL")

    func compose(input _: DolbyVisionFELCompositorInput) throws -> DolbyVisionFELCompositorOutput {
        throw NSError(domain: "TestFELCompositorBackend", code: 0)
    }
}

private final class TestFelBakerShim: FelBakerDolbyVisionFELCompositorShim {
    let backendName = "FakeFelBaker"
    let capabilities: FelBakerDolbyVisionFELCompositorCapabilities = [.fullEnhancementLayerComposition, .rgb48Output]
    private(set) var lastRPUData: Data?
    private(set) var lastPresentationTime: CMTime?

    func compose(input: DolbyVisionFELCompositorInput, outputPixelBuffer _: CVPixelBuffer) throws -> String? {
        lastRPUData = input.rpuData
        lastPresentationTime = input.presentationTime
        return "fake FEL compose"
    }
}

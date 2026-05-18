@testable import KSPlayer
import AVFoundation
import CoreGraphics
import XCTest

class VideoPlayerViewTest: XCTestCase {
    func testResize() {}

    func testProgressPreviewClampsCenterWithinSlider() {
        XCTAssertEqual(ProgressPreviewResolver.previewCenterX(time: -5, totalTime: 100, sliderWidth: 300, previewWidth: 120), 60)
        XCTAssertEqual(ProgressPreviewResolver.previewCenterX(time: 50, totalTime: 100, sliderWidth: 300, previewWidth: 120), 150)
        XCTAssertEqual(ProgressPreviewResolver.previewCenterX(time: 120, totalTime: 100, sliderWidth: 300, previewWidth: 120), 240)
    }

    func testProgressPreviewNearestThumbnailIndex() {
        let times: [TimeInterval] = [0, 10, 30, 60]

        XCTAssertEqual(ProgressPreviewResolver.nearestThumbnailIndex(times: times, target: 12), 1)
        XCTAssertEqual(ProgressPreviewResolver.nearestThumbnailIndex(times: times, target: 46), 3)
        XCTAssertNil(ProgressPreviewResolver.nearestThumbnailIndex(times: [], target: 12))
    }

    func testHighPerformanceVideoPolicyPreservesNormalFrameCapacity() {
        XCTAssertEqual(HighPerformanceVideoPlaybackPolicy.frameCapacity(fps: 24, naturalSize: CGSize(width: 3840, height: 2160), isLive: false), 16)
        XCTAssertEqual(HighPerformanceVideoPlaybackPolicy.frameCapacity(fps: 24, naturalSize: CGSize(width: 3840, height: 2160), isLive: true), 4)
    }

    func testHighPerformanceVideoPolicyIncreasesBackpressureForHighFPS() {
        XCTAssertTrue(HighPerformanceVideoPlaybackPolicy.isHighWorkload(fps: 120, naturalSize: CGSize(width: 3840, height: 2160)))
        XCTAssertEqual(HighPerformanceVideoPlaybackPolicy.frameCapacity(fps: 120, naturalSize: CGSize(width: 3840, height: 2160), isLive: false), 30)
        XCTAssertEqual(HighPerformanceVideoPlaybackPolicy.frameCapacity(fps: 120, naturalSize: CGSize(width: 3840, height: 2160), isLive: true), 8)
    }

    func testHighPerformanceVideoPolicyIdentifies8KWorkload() {
        let eightK = CGSize(width: 7680, height: 4320)

        XCTAssertTrue(HighPerformanceVideoPlaybackPolicy.isHighWorkload(fps: 30, naturalSize: eightK))
        XCTAssertFalse(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: .appleSuperResolution(scaleFactor: 2), sourceSize: eightK, fps: 30))
        XCTAssertTrue(HighPerformanceVideoPlaybackPolicy.shouldApplyUpscaling(mode: .appleSuperResolution(scaleFactor: 2), sourceSize: CGSize(width: 1920, height: 1080), fps: 60))
    }

    func testHighPerformanceVideoPolicyPrefers120FPSDisplayRange() {
        let normalRange = HighPerformanceVideoPlaybackPolicy.displayFrameRateRange(fps: 24)
        XCTAssertEqual(normalRange.minimum, 24)
        XCTAssertEqual(normalRange.maximum, 48)
        XCTAssertEqual(normalRange.preferred, 24)

        let highFPSRange = HighPerformanceVideoPlaybackPolicy.displayFrameRateRange(fps: 120)
        XCTAssertEqual(highFPSRange.minimum, 60)
        XCTAssertEqual(highFPSRange.maximum, 120)
        XCTAssertEqual(highFPSRange.preferred, 120)
    }

    func testFFmpegOnlyURLSchemesAreRecognized() throws {
        XCTAssertTrue(try XCTUnwrap(URL(string: "smb://server/share/movie.mkv")).isFFmpegOnlyInputScheme)
        XCTAssertTrue(try XCTUnwrap(URL(string: "nfs://server/export/movie.mkv")).isFFmpegOnlyInputScheme)
        XCTAssertTrue(try XCTUnwrap(URL(string: "srt://example.com:9000")).isFFmpegOnlyInputScheme)
        XCTAssertFalse(try XCTUnwrap(URL(string: "http://example.com/movie.mkv")).isFFmpegOnlyInputScheme)
        XCTAssertFalse(try XCTUnwrap(URL(string: "upnp://device/item")).isFFmpegOnlyInputScheme)
    }

    @MainActor
    func testPreferredPlayerTypeRoutesFFmpegOnlySchemesToMEPlayer() throws {
        let originalFirstPlayerType = KSOptions.firstPlayerType
        defer {
            KSOptions.firstPlayerType = originalFirstPlayerType
        }
        KSOptions.firstPlayerType = KSAVPlayer.self

        let options = KSOptions()

        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: try XCTUnwrap(URL(string: "smb://server/share/movie.mkv")), options: options) == KSMEPlayer.self)
        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: try XCTUnwrap(URL(string: "nfs://server/export/movie.mkv")), options: options) == KSMEPlayer.self)
        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: try XCTUnwrap(URL(string: "srt://example.com:9000")), options: options) == KSMEPlayer.self)
        XCTAssertTrue(KSPlayerLayer.preferredPlayerType(for: try XCTUnwrap(URL(string: "https://example.com/movie.mkv")), options: options) == KSAVPlayer.self)
    }

    func testFFmpegOnlyProtocolWhitelistIsExtendedWhenConstrained() throws {
        let options = KSOptions()
        options.formatContextOptions["protocol_whitelist"] = "file,http,https,tcp"

        options.prepareFormatContextOptions(for: try XCTUnwrap(URL(string: "smb://server/share/movie.mkv")))

        XCTAssertEqual(options.formatContextOptions["protocol_whitelist"] as? String, "file,http,https,tcp,smb")
    }

    func testHeadersPassThroughToAVAndFFmpegOptions() {
        let options = KSOptions()
        options.appendHeader(["X-Test": "value"])

        let avHeaders = options.avOptions["AVURLAssetHTTPHeaderFieldsKey"] as? [String: String]
        XCTAssertEqual(avHeaders?["User-Agent"], "KSPlayer")
        XCTAssertEqual(avHeaders?["X-Test"], "value")
        XCTAssertTrue((options.formatContextOptions["headers"] as? String)?.contains("X-Test: value\r\n") == true)
    }

    func testLowLatencyLiveProfileAppliesLANPolicy() {
        let options = KSOptions()
        options.cache = true
        options.isDiskPrecacheEnabled = true
        options.isMemorySeekCacheEnabled = true

        options.applyLowLatencyLiveProfile(.lan)

        XCTAssertEqual(options.lowLatencyLiveProfile, .lan)
        XCTAssertEqual(options.preferredForwardBufferDuration, 0.12)
        XCTAssertEqual(options.maxBufferDuration, 0.5)
        XCTAssertFalse(options.cache)
        XCTAssertFalse(options.isDiskPrecacheEnabled)
        XCTAssertFalse(options.isMemorySeekCacheEnabled)
        XCTAssertTrue(options.nobuffer)
        XCTAssertTrue(options.codecLowDelay)
        XCTAssertTrue(options.hardwareDecode)
        XCTAssertTrue(options.asynchronousDecompression)
        XCTAssertEqual(options.probesize, Int64(32 * 1024))
        XCTAssertEqual(options.maxAnalyzeDuration, Int64(100 * 1000))
        XCTAssertEqual(options.formatContextOptions["max_delay"] as? Int, 100 * 1000)
        XCTAssertEqual(options.formatContextOptions["fflags"] as? String, "nobuffer")
        XCTAssertEqual(options.formatContextOptions["avioflags"] as? String, "direct")
    }

    func testLowLatencyLiveProfilePreservesExplicitProbeAndFormatOptions() {
        let options = KSOptions()
        options.probesize = 512 * 1024
        options.maxAnalyzeDuration = 250 * 1000
        options.formatContextOptions["max_delay"] = 250 * 1000
        options.formatContextOptions["fflags"] = "genpts"

        options.applyLowLatencyLiveProfile(.lan)
        options.applyLowLatencyLiveProfile(.lan)

        XCTAssertEqual(options.probesize, Int64(512 * 1024))
        XCTAssertEqual(options.maxAnalyzeDuration, Int64(250 * 1000))
        XCTAssertEqual(options.formatContextOptions["max_delay"] as? Int, 250 * 1000)
        XCTAssertEqual(options.formatContextOptions["fflags"] as? String, "genpts+nobuffer")
    }

    func testLowLatencyLive4KDiagnosticsFocusOnDecodeConstraints() {
        let fourK = CGSize(width: 3840, height: 2160)

        XCTAssertEqual(
            LowLatencyLivePlaybackPolicy.diagnosticMessage(profile: .lan, fps: 60, naturalSize: fourK, hardwareDecode: false, asynchronousDecompression: true),
            "[video] low latency live 4K may miss latency targets because hardware decode is disabled"
        )
        XCTAssertEqual(
            LowLatencyLivePlaybackPolicy.diagnosticMessage(profile: .lan, fps: 120, naturalSize: fourK, hardwareDecode: true, asynchronousDecompression: false),
            "[video] low latency live 4K high-FPS may need asynchronous VideoToolbox decode"
        )
        XCTAssertNil(
            LowLatencyLivePlaybackPolicy.diagnosticMessage(profile: .lan, fps: 60, naturalSize: CGSize(width: 1920, height: 1080), hardwareDecode: false, asynchronousDecompression: false)
        )
    }

    func testDeinterlacePolicyDetectsFieldOrder() {
        XCTAssertEqual(VideoDeinterlacePolicy.detectedInterlacingType(fieldOrder: .tt), .tff)
        XCTAssertEqual(VideoDeinterlacePolicy.detectedInterlacingType(fieldOrder: .tb), .tff)
        XCTAssertEqual(VideoDeinterlacePolicy.detectedInterlacingType(fieldOrder: .bb), .bff)
        XCTAssertEqual(VideoDeinterlacePolicy.detectedInterlacingType(fieldOrder: .bt), .bff)
        XCTAssertEqual(VideoDeinterlacePolicy.detectedInterlacingType(fieldOrder: .progressive), .progressive)
        XCTAssertNil(VideoDeinterlacePolicy.detectedInterlacingType(fieldOrder: .unknown))
    }

    func testDeinterlacePolicyDetectsFrameFlags() {
        XCTAssertEqual(VideoDeinterlacePolicy.detectedInterlacingType(frameFlags: 0), .progressive)
        XCTAssertEqual(VideoDeinterlacePolicy.detectedInterlacingType(frameFlags: 1 << 3), .bff)
        XCTAssertEqual(VideoDeinterlacePolicy.detectedInterlacingType(frameFlags: (1 << 3) | (1 << 4)), .tff)
    }

    func testDeinterlacePolicyAppliesAutomaticOnlyForClearInterlacedNormalWorkload() {
        let progressive = VideoDeinterlacePolicy.decision(
            mode: .automatic,
            fieldOrder: .progressive,
            fps: 24,
            naturalSize: CGSize(width: 1920, height: 1080),
            yadifMode: 1,
            addIdet: false
        )
        XCTAssertFalse(progressive.shouldApplyFilter)

        let interlaced = VideoDeinterlacePolicy.decision(
            mode: .automatic,
            fieldOrder: .tt,
            fps: 29.97,
            naturalSize: CGSize(width: 1920, height: 1080),
            yadifMode: 1,
            addIdet: true
        )
        XCTAssertEqual(interlaced.detectedInterlacingType, .tff)
        XCTAssertEqual(interlaced.filters, ["idet", "yadif=mode=1:parity=-1:deint=1"])
        XCTAssertTrue(interlaced.doublesFrameRate)
    }

    func testDeinterlacePolicySkipsHighWorkloadAutomaticUnlessForced() {
        let eightK = CGSize(width: 7680, height: 4320)
        let automatic = VideoDeinterlacePolicy.decision(
            mode: .automatic,
            fieldOrder: .tt,
            fps: 30,
            naturalSize: eightK,
            yadifMode: 0,
            addIdet: false
        )
        XCTAssertFalse(automatic.shouldApplyFilter)
        XCTAssertEqual(automatic.skipReason, "high workload requires force mode")

        let forced = VideoDeinterlacePolicy.decision(
            mode: .force,
            fieldOrder: .progressive,
            fps: 120,
            naturalSize: eightK,
            yadifMode: 0,
            addIdet: false
        )
        XCTAssertEqual(forced.filters, ["yadif=mode=0:parity=-1:deint=1"])
        XCTAssertFalse(forced.doublesFrameRate)
    }

    func testMediaPlaybackTimeRangeClampsToWindow() {
        let range = MediaPlaybackTimeRange(start: 100, duration: 30)

        XCTAssertEqual(range?.duration, 30)
        XCTAssertEqual(range?.clamped(90), 100)
        XCTAssertEqual(range?.clamped(115), 115)
        XCTAssertEqual(range?.clamped(140), 130)
        XCTAssertNil(MediaPlaybackTimeRange(start: 100, duration: 0))
    }

    func testClipExportRequestRejectsInvalidTimes() {
        let destination = URL(fileURLWithPath: "/tmp/clip.mp4")

        XCTAssertThrowsError(try KSPlayerClipExportRequest(start: 10, end: 10, destination: destination))
        XCTAssertThrowsError(try KSPlayerClipExportRequest(start: .nan, end: 20, destination: destination))
        XCTAssertNoThrow(try KSPlayerClipExportRequest(start: 10, end: 20, destination: destination))
    }

    func testClipExportRequestClampsToSeekableRange() throws {
        let destination = URL(fileURLWithPath: "/tmp/clip.mp4")
        let range = MediaPlaybackTimeRange(start: 100, duration: 30)

        let request = try KSPlayerClipExportRequest.validated(start: 90, end: 140, destination: destination, availableRange: range)

        XCTAssertEqual(request.start, 100)
        XCTAssertEqual(request.end, 130)
        XCTAssertEqual(request.duration, 30)
        XCTAssertThrowsError(try KSPlayerClipExportRequest.validated(start: 10, end: 20, destination: destination, availableRange: nil)) { error in
            XCTAssertEqual(error as? KSPlayerClipExportError, .unsupportedNonSeekableSource)
        }
    }

    func testClipDefaultDestinationUsesSourceNameAndTimes() {
        let directory = URL(fileURLWithPath: "/tmp/KSPlayer Clips", isDirectory: true)
        let source = URL(fileURLWithPath: "/media/My Movie.mov")

        let destination = KSPlayerClipDestination.makeDefault(sourceURL: source, directory: directory, start: 65.432, end: 3661.2)

        XCTAssertEqual(destination.deletingLastPathComponent(), directory)
        XCTAssertEqual(destination.lastPathComponent, "My Movie-00-01-05-43-01-01-01-20.mp4")
    }

    func testVideoConversionPresetBuildsStreamCopyArguments() {
        let input = URL(fileURLWithPath: "/tmp/input file.mov")
        let output = URL(fileURLWithPath: "/tmp/output.mp4")

        XCTAssertEqual(VideoConversionPreset.remuxMP4.ffmpegArguments(inputURL: input, outputURL: output), [
            "-hide_banner", "-y", "-i", "/tmp/input file.mov", "-map", "0", "-c", "copy", "-movflags", "+faststart", "/tmp/output.mp4",
        ])
        XCTAssertFalse(VideoConversionPreset.remuxMatroska.ffmpegArguments(inputURL: input, outputURL: output).contains("+faststart"))
    }

    func testVideoExportDefaultDestinationSanitizesRemoteName() {
        let directory = URL(fileURLWithPath: "/tmp/KSPlayer Exports", isDirectory: true)
        let source = URL(string: "https://example.com/media/My%20Movie:Final?.mkv")!

        let destination = VideoExportJob.defaultDestination(sourceURL: source, directory: directory, preset: .remuxMOV)

        XCTAssertEqual(destination.deletingLastPathComponent(), directory)
        XCTAssertEqual(destination.pathExtension, "mov")
        XCTAssertFalse(destination.lastPathComponent.contains(":"))
        XCTAssertFalse(destination.lastPathComponent.contains("?"))
    }

    func testVideoExportPathPolicyRejectsUnsafeDestinations() throws {
        let temporaryDirectory = FileManager.default.temporaryDirectory.appendingPathComponent(UUID().uuidString, isDirectory: true)
        try FileManager.default.createDirectory(at: temporaryDirectory, withIntermediateDirectories: true)
        defer { try? FileManager.default.removeItem(at: temporaryDirectory) }

        let source = temporaryDirectory.appendingPathComponent("source.mov")
        let existingDestination = temporaryDirectory.appendingPathComponent("existing.mp4")
        FileManager.default.createFile(atPath: source.path, contents: Data())
        FileManager.default.createFile(atPath: existingDestination.path, contents: Data())

        XCTAssertThrowsError(try VideoExportPathPolicy.validate(VideoExportJob(sourceURL: source, destinationURL: source))) { error in
            XCTAssertEqual(error as? VideoExportError, .sourceAndDestinationMatch(source))
        }
        XCTAssertThrowsError(try VideoExportPathPolicy.validate(VideoExportJob(sourceURL: source, destinationURL: existingDestination))) { error in
            XCTAssertEqual(error as? VideoExportError, .destinationExists(existingDestination))
        }
        XCTAssertThrowsError(try VideoExportPathPolicy.validate(VideoExportJob(sourceURL: source, destinationURL: temporaryDirectory, overwriteExisting: true))) { error in
            XCTAssertEqual(error as? VideoExportError, .unsafeDestination(temporaryDirectory))
        }
        XCTAssertNoThrow(try VideoExportPathPolicy.validate(VideoExportJob(sourceURL: source, destinationURL: existingDestination, overwriteExisting: true)))
    }

    func testAVPlayerSeekableRangeResolverUnionsFiniteRanges() {
        let ranges = [
            CMTimeRange(start: CMTime(seconds: 120, preferredTimescale: 600), duration: CMTime(seconds: 30, preferredTimescale: 600)),
            CMTimeRange(start: CMTime(seconds: 180, preferredTimescale: 600), duration: CMTime(seconds: 20, preferredTimescale: 600)),
        ]
        let range = AVPlayerSeekableRangeResolver.range(from: ranges)

        XCTAssertEqual(range?.start, 120)
        XCTAssertEqual(range?.end, 200)
        XCTAssertNil(AVPlayerSeekableRangeResolver.range(from: [
            CMTimeRange(start: .zero, duration: .positiveInfinity),
        ]))
    }

    func testSeamlessLoopPolicyKeepsAVPlayerRunningAtItemEnd() {
        XCTAssertEqual(SeamlessLoopPlaybackPolicy.avPlayerActionAtItemEnd(isLoopPlay: true, isSeamlessLoopEnabled: true), .none)
        XCTAssertEqual(SeamlessLoopPlaybackPolicy.avPlayerActionAtItemEnd(isLoopPlay: true, isSeamlessLoopEnabled: false), .pause)
        XCTAssertEqual(SeamlessLoopPlaybackPolicy.avPlayerActionAtItemEnd(isLoopPlay: false, isSeamlessLoopEnabled: true), .pause)
    }

    func testSeamlessLoopPolicyUsesExactSeekForLoopRestartOnly() {
        XCTAssertEqual(SeamlessLoopPlaybackPolicy.avPlayerSeekTolerance(isAccurateSeek: false, isLoopRestart: true), .zero)
        XCTAssertEqual(SeamlessLoopPlaybackPolicy.avPlayerSeekTolerance(isAccurateSeek: true, isLoopRestart: false), .zero)
        XCTAssertEqual(SeamlessLoopPlaybackPolicy.avPlayerSeekTolerance(isAccurateSeek: false, isLoopRestart: false), .positiveInfinity)
    }

    func testSeamlessLoopPolicyRequiresLoopingAndIdlePacketQueues() {
        XCTAssertTrue(SeamlessLoopPlaybackPolicy.shouldUseMEPlayerPacketQueue(isLoopPlay: true, isSeamlessLoopEnabled: true, usesAsyncPacketQueue: true, tracksAlreadyLooping: false))
        XCTAssertFalse(SeamlessLoopPlaybackPolicy.shouldUseMEPlayerPacketQueue(isLoopPlay: false, isSeamlessLoopEnabled: true, usesAsyncPacketQueue: true, tracksAlreadyLooping: false))
        XCTAssertFalse(SeamlessLoopPlaybackPolicy.shouldUseMEPlayerPacketQueue(isLoopPlay: true, isSeamlessLoopEnabled: false, usesAsyncPacketQueue: true, tracksAlreadyLooping: false))
        XCTAssertFalse(SeamlessLoopPlaybackPolicy.shouldUseMEPlayerPacketQueue(isLoopPlay: true, isSeamlessLoopEnabled: true, usesAsyncPacketQueue: false, tracksAlreadyLooping: false))
        XCTAssertFalse(SeamlessLoopPlaybackPolicy.shouldUseMEPlayerPacketQueue(isLoopPlay: true, isSeamlessLoopEnabled: true, usesAsyncPacketQueue: true, tracksAlreadyLooping: true))
    }

    func testSeamlessLoopOptionIsOptInByDefault() {
        XCTAssertFalse(KSOptions.isSeamlessLoopEnabled)
    }

    func testFFmpegSeekabilityAllowsPlaylistDVRButNotGenericNonSeekableLive() {
        XCTAssertTrue(FFmpegSeekabilityPolicy.isPlaylistFormat("hls"))
        XCTAssertTrue(FFmpegSeekabilityPolicy.isPlaylistFormat("dash"))
        XCTAssertTrue(FFmpegSeekabilityPolicy.isPlaylistFormat("applehttp,hls"))
        XCTAssertFalse(FFmpegSeekabilityPolicy.isPlaylistFormat("mpegts"))

        XCTAssertEqual(FFmpegSeekabilityPolicy.seekableTimeRange(duration: 60, ioSeekable: false, formatName: "hls")?.duration, 60)
        XCTAssertEqual(FFmpegSeekabilityPolicy.seekableTimeRange(duration: 60, ioSeekable: false, formatName: "dash")?.duration, 60)
        XCTAssertNil(FFmpegSeekabilityPolicy.seekableTimeRange(duration: 60, ioSeekable: false, formatName: "mpegts"))
        XCTAssertNil(FFmpegSeekabilityPolicy.seekableTimeRange(duration: 0, ioSeekable: true, formatName: "hls"))
    }

    @MainActor
    func testToolbarMapsLiveDVRSliderToMediaTime() {
        let toolbar = PlayerToolBar()

        toolbar.totalTime = 0
        toolbar.seekableTimeRange = MediaPlaybackTimeRange(start: 120, duration: 45)
        toolbar.currentTime = 150

        XCTAssertEqual(toolbar.sliderDuration, 45)
        XCTAssertEqual(toolbar.timeSlider.maximumValue, 45)
        XCTAssertEqual(toolbar.timeSlider.value, 30)
        XCTAssertEqual(toolbar.mediaTime(forSliderValue: 10), 130)
        XCTAssertEqual(toolbar.mediaTime(forSliderValue: 100), 165)
        XCTAssertEqual(toolbar.displayTime(for: 150), 30)
    }

    @MainActor
    func testToolbarDoesNotTreatNonDVRLiveAsRewindable() {
        let toolbar = PlayerToolBar()

        toolbar.totalTime = 1
        toolbar.totalTime = 0
        toolbar.seekableTimeRange = nil
        toolbar.isSeekable = false

        XCTAssertFalse(toolbar.isLiveDVRStream)
        XCTAssertEqual(toolbar.sliderDuration, 0)
        XCTAssertEqual(toolbar.timeSlider.maximumValue, Float(60 * 60 * 24))
        XCTAssertFalse(toolbar.timeSlider.isUserInteractionEnabled)
        XCTAssertEqual(toolbar.mediaTime(forSliderValue: 10), 10)
    }

    func testDefinitionSwitchPrewarmPolicyRequiresSeekableVOD() {
        XCTAssertTrue(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 120,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: false,
            duration: 120,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 0,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 120,
            targetTime: 30,
            isSeekable: false,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 120,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: true,
            isPictureInPictureActive: false
        ))
        XCTAssertFalse(DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: true,
            duration: 120,
            targetTime: 30,
            isSeekable: true,
            isExternalPlaybackActive: false,
            isPictureInPictureActive: true
        ))
    }

    func testDefinitionSwitchHandoffTimeAdvancesOnlyWhenPlaying() {
        XCTAssertEqual(DefinitionSwitchPrewarmPolicy.handoffTime(requestedTime: 10, elapsed: 2, playbackRate: 1.5, wasPlaying: true, duration: 20), 13)
        XCTAssertEqual(DefinitionSwitchPrewarmPolicy.handoffTime(requestedTime: 10, elapsed: 2, playbackRate: 1.5, wasPlaying: false, duration: 20), 10)
        XCTAssertEqual(DefinitionSwitchPrewarmPolicy.handoffTime(requestedTime: 19, elapsed: 2, playbackRate: 1.5, wasPlaying: true, duration: 20), 20)
    }

    func testAdaptiveBitratePolicyDowngradesOnRebuffer() {
        let policy = KSAdaptiveBitrateSwitchingPolicy(minimumSwitchInterval: 20, downgradeRebufferCount: 1)

        XCTAssertEqual(policy.decision(
            definitionRank: 1,
            definitionCount: 3,
            bufferAhead: 8,
            rebufferCount: 1,
            stableBufferDuration: 0,
            secondsSinceLastSwitch: 21,
            isLive: false,
            isAdaptiveStreamingManifest: false
        ), .switchToLower)
    }

    func testAdaptiveBitratePolicyUpgradesAfterStableBuffer() {
        let policy = KSAdaptiveBitrateSwitchingPolicy(minimumSwitchInterval: 20, upgradeBufferThreshold: 10, upgradeObservationDuration: 30)

        XCTAssertEqual(policy.decision(
            definitionRank: 0,
            definitionCount: 3,
            bufferAhead: 12,
            rebufferCount: 0,
            stableBufferDuration: 31,
            secondsSinceLastSwitch: 21,
            isLive: false,
            isAdaptiveStreamingManifest: false
        ), .switchToHigher)
    }

    func testAdaptiveBitratePolicyUsesCooldownAndSkipsManifests() {
        let policy = KSAdaptiveBitrateSwitchingPolicy(minimumSwitchInterval: 20, upgradeBufferThreshold: 10, upgradeObservationDuration: 30)

        XCTAssertEqual(policy.decision(
            definitionRank: 0,
            definitionCount: 3,
            bufferAhead: 12,
            rebufferCount: 0,
            stableBufferDuration: 31,
            secondsSinceLastSwitch: 5,
            isLive: false,
            isAdaptiveStreamingManifest: false
        ), .stay)
        XCTAssertEqual(policy.decision(
            definitionRank: 1,
            definitionCount: 3,
            bufferAhead: 1,
            rebufferCount: 1,
            stableBufferDuration: 0,
            secondsSinceLastSwitch: 21,
            isLive: false,
            isAdaptiveStreamingManifest: true
        ), .stay)
    }

    func testResumeStateClampsFiniteVodTime() {
        let state = KSPlayerResumeState(
            url: URL(fileURLWithPath: "/tmp/movie.mp4"),
            currentTime: 125,
            duration: 120,
            wasPlaying: true,
            playbackRate: 1.25
        )

        XCTAssertEqual(state.resumableTime, 120)
        XCTAssertTrue(state.wasPlaying)
        XCTAssertEqual(state.playbackRate, 1.25)
    }

    func testResumeStateKeepsLiveTimeWithoutFiniteDuration() {
        XCTAssertEqual(KSPlayerResumeState.resumableTime(currentTime: 42, duration: 0), 42)
        XCTAssertEqual(KSPlayerResumeState.resumableTime(currentTime: 42, duration: .infinity), 42)
        XCTAssertEqual(KSPlayerResumeState.resumableTime(currentTime: .nan, duration: 120), 0)
        XCTAssertEqual(KSPlayerResumeState.resumableTime(currentTime: -10, duration: 120), 0)
    }

    func testCompactLayoutClampsToContainerMargins() {
        let layout = KSPlayerCompactLayout(size: CGSize(width: 400, height: 400), margin: 16)
        let size = layout.resolvedSize(containerSize: CGSize(width: 200, height: 120))

        XCTAssertEqual(size.width, 168)
        XCTAssertEqual(size.height, 88)
    }
}

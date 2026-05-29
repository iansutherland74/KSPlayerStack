//
//  KSMEPlayer.swift
//  KSPlayer
//
//  Created by kintan on 2018/3/9.
//

import AVFoundation
import AVKit
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

public class KSMEPlayer: NSObject, @unchecked Sendable {
    private var loopCount = 1
    private var playerItem: MEPlayerItem
    public let audioOutput: AudioOutput
    private var options: KSOptions
    private var bufferingCountDownTimer: Timer?
    public private(set) var videoOutput: (VideoOutput & UIView)? {
        didSet {
            oldValue?.invalidate()
            Task { @MainActor in
                oldValue?.removeFromSuperview()
            }
        }
    }

    public private(set) var bufferingProgress = 0 {
        willSet {
            Task { @MainActor [weak self] in
                guard let self else { return }
                delegate?.changeBuffering(player: self, progress: newValue)
            }
        }
    }

    private lazy var _pipController: Any? = {
        if #available(iOS 15.0, tvOS 15.0, macOS 12.0, *), let videoOutput {
            let contentSource = AVPictureInPictureController.ContentSource(sampleBufferDisplayLayer: videoOutput.displayLayer, playbackDelegate: self)
            let pip = KSPictureInPictureController(contentSource: contentSource)
            return pip
        } else {
            return nil
        }
    }()

    @available(tvOS 14.0, *)
    public var pipController: KSPictureInPictureController? {
        _pipController as? KSPictureInPictureController
    }

    private lazy var _playbackCoordinator: Any? = {
        if #available(macOS 12.0, iOS 15.0, tvOS 15.0, *) {
            let coordinator = AVDelegatingPlaybackCoordinator(playbackControlDelegate: self)
            coordinator.suspensionReasonsThatTriggerWaiting = [.stallRecovery]
            return coordinator
        } else {
            return nil
        }
    }()

    @available(macOS 12.0, iOS 15.0, tvOS 15.0, *)
    public var playbackCoordinator: AVPlaybackCoordinator {
        // swiftlint:disable force_cast
        _playbackCoordinator as! AVPlaybackCoordinator
        // swiftlint:enable force_cast
    }

    public private(set) var playableTime = TimeInterval(0)
    public weak var delegate: MediaPlayerDelegate?
    public private(set) var isReadyToPlay = false
    public var allowsExternalPlayback: Bool = false
    public var usesExternalPlaybackWhileExternalScreenIsActive: Bool = false
    var decodedVideoFrameOutput: KSVideoFrameOutput? {
        didSet {
            options.videoFrameOutput = decodedVideoFrameOutput
        }
    }

    public var playbackRate: Float = 1 {
        didSet {
            if playbackRate != audioOutput.playbackRate {
                audioOutput.playbackRate = playbackRate
                if audioOutput is AudioUnitPlayer {
                    var audioFilters = options.audioFilters.filter {
                        !$0.hasPrefix("atempo=")
                    }
                    if playbackRate != 1 {
                        audioFilters.append("atempo=\(playbackRate)")
                    }
                    options.audioFilters = audioFilters
                }
            }
        }
    }

    public private(set) var loadState = MediaLoadState.idle {
        didSet {
            if loadState != oldValue {
                playOrPause()
            }
        }
    }

    public private(set) var playbackState = MediaPlaybackState.idle {
        didSet {
            if playbackState != oldValue {
                playOrPause()
                if playbackState == .finished {
                    Task { @MainActor [weak self] in
                        guard let self else { return }
                        delegate?.finish(player: self, error: nil)
                    }
                }
            }
        }
    }

    public required init(url: URL, options: KSOptions) {
        KSOptions.setAudioSession(options: options, playbackPipeline: .decodedPCM)
        audioOutput = KSOptions.audioPlayerType.init()
        playerItem = MEPlayerItem(url: url, options: options)
        if options.videoDisable {
            videoOutput = nil
        } else {
            videoOutput = KSOptions.videoPlayerType.init(options: options)
        }
        self.options = options
        decodedVideoFrameOutput = options.videoFrameOutput
        super.init()
        playerItem.delegate = self
        audioOutput.renderSource = playerItem
        videoOutput?.renderSource = playerItem
        videoOutput?.displayLayerDelegate = self
        #if !os(macOS)
        NotificationCenter.default.addObserver(self, selector: #selector(audioRouteChange), name: AVAudioSession.routeChangeNotification, object: AVAudioSession.sharedInstance())
        if #available(tvOS 15.0, iOS 15.0, *) {
            NotificationCenter.default.addObserver(self, selector: #selector(spatialCapabilityChange), name: AVAudioSession.spatialPlaybackCapabilitiesChangedNotification, object: nil)
        }
        #endif
    }

    deinit {
        #if !os(macOS)
        try? AVAudioSession.sharedInstance().setPreferredOutputNumberOfChannels(2)
        #endif
        NotificationCenter.default.removeObserver(self)
        videoOutput?.invalidate()
        playerItem.shutdown()
    }
}

// MARK: - private functions

private extension KSMEPlayer {
    func playOrPause() {
        Task { @MainActor [weak self] in
            guard let self else { return }
            let isPaused = !(self.playbackState == .playing && self.loadState == .playable)
            if isPaused {
                self.audioOutput.pause()
                self.videoOutput?.pause()
            } else {
                self.audioOutput.play()
                self.videoOutput?.play()
            }
            self.delegate?.changeLoadState(player: self)
        }
    }

    @objc private func spatialCapabilityChange(notification _: Notification) {
        KSLog("[audio] spatialCapabilityChange")
        for track in tracks(mediaType: .audio) {
            (track as? FFmpegAssetTrack)?.audioDescriptor?.updateAudioFormat(options: options)
        }
        updateAudioRouteDiagnosticForCurrentAudioTrack()
    }

    #if !os(macOS)
    @objc private func audioRouteChange(notification: Notification) {
        KSLog("[audio] audioRouteChange")
        guard notification.userInfo?[AVAudioSessionRouteChangeReasonKey] as? UInt != nil else {
            return
        }
//        let routeChangeReason = AVAudioSession.RouteChangeReason(rawValue: reason)
//        guard [AVAudioSession.RouteChangeReason.newDeviceAvailable, .oldDeviceUnavailable, .routeConfigurationChange].contains(routeChangeReason) else {
//            return
//        }
        for track in tracks(mediaType: .audio) {
            (track as? FFmpegAssetTrack)?.audioDescriptor?.updateAudioFormat(options: options)
        }
        updateAudioRouteDiagnosticForCurrentAudioTrack()
        audioOutput.flush()
    }
    #endif

    func updateAudioRouteDiagnosticForCurrentAudioTrack() {
        let audioTrack = tracks(mediaType: .audio).first { $0.isEnabled } as? FFmpegAssetTrack
        KSOptions.updateAudioRouteDiagnostic(
            options: options,
            playbackPipeline: .decodedPCM,
            sourceChannelCount: audioTrack?.audioDescriptor?.sourceChannelCount,
            containsEncodedPassthroughCandidate: EncodedAudioPassthroughPolicyResolver.isEncodedPassthroughCandidate(metadata: audioTrack?.audioCodecMetadata),
            allowsExternalPlayback: allowsExternalPlayback,
            usesExternalPlaybackWhileExternalScreenIsActive: usesExternalPlaybackWhileExternalScreenIsActive,
            isExternalPlaybackActive: isExternalPlaybackActive
        )
    }
}

extension KSMEPlayer: MEPlayerDelegate {
    func sourceDidOpened() {
        isReadyToPlay = true
        options.readyTime = CACurrentMediaTime()
        let vidoeTracks = tracks(mediaType: .video)
        if vidoeTracks.isEmpty {
            videoOutput = nil
        }
        let audioDescriptor = tracks(mediaType: .audio).first { $0.isEnabled }.flatMap {
            $0 as? FFmpegAssetTrack
        }?.audioDescriptor
        Task { @MainActor [weak self] in
            guard let self else { return }
            if let audioDescriptor {
                audioDescriptor.updateAudioFormat(options: options)
                updateAudioRouteDiagnosticForCurrentAudioTrack()
                KSLog("[audio] audio type: \(audioOutput) prepare audioFormat )")
                audioOutput.prepare(audioFormat: audioDescriptor.audioFormat)
            }
            if let controlTimebase = videoOutput?.displayLayer.controlTimebase, options.startPlayTime > 1 {
                CMTimebaseSetTime(controlTimebase, time: CMTimeMake(value: Int64(options.startPlayTime), timescale: 1))
            }
            delegate?.readyToPlay(player: self)
        }
    }

    func sourceDidFailed(error: NSError?) {
        Task { @MainActor [weak self] in
            guard let self else { return }
            self.delegate?.finish(player: self, error: error)
        }
    }

    func sourceDidFinished() {
        Task { @MainActor [weak self] in
            guard let self else { return }
            if self.playerItem.canRestartLoopPlayback {
                self.loopCount += 1
                self.delegate?.playBack(player: self, loopCount: self.loopCount)
                self.audioOutput.play()
                self.videoOutput?.play()
            } else {
                self.playbackState = .finished
            }
        }
    }

    func sourceDidChange(loadingState: LoadingState) {
        if loadingState.isEndOfFile {
            playableTime = duration
        } else {
            playableTime = currentPlaybackTime + loadingState.loadedTime
        }
        if loadState == .playable {
            if !loadingState.isEndOfFile, loadingState.frameCount == 0, loadingState.packetCount == 0, options.preferredForwardBufferDuration != 0 {
                loadState = .loading
                if playbackState == .playing {
                    runOnMainThread { [weak self] in
                        // 在主线程更新进度
                        self?.bufferingProgress = 0
                    }
                }
            }
        } else {
            if loadingState.isFirst {
                if videoOutput?.pixelBuffer == nil {
                    videoOutput?.readNextFrame()
                }
            }
            var progress = 100
            if loadingState.isPlayable {
                loadState = .playable
            } else {
                if loadingState.progress.isInfinite {
                    progress = 100
                } else if loadingState.progress.isNaN {
                    progress = 0
                } else {
                    progress = min(100, Int(loadingState.progress))
                }
            }
            let bufferingProgress = progress
            if playbackState == .playing {
                runOnMainThread { [weak self] in
                    // 在主线程更新进度
                    self?.bufferingProgress = bufferingProgress
                }
            }
        }
        if duration == 0, playbackState == .playing, loadState == .playable {
            if let rate = options.liveAdaptivePlaybackRate(loadingState: loadingState) {
                playbackRate = rate
            }
        }
    }

    func sourceDidChange(oldBitRate: Int64, newBitrate: Int64) {
        KSLog("oldBitRate \(oldBitRate) change to newBitrate \(newBitrate)")
        let needsPresentationFlush = options.suppressWindowVideoPresentationWhileImmersiveCompositorActive
            || options.relaxVideoClockSyncWhileImmersiveCompositorActive
            || options.videoFrameOutput != nil
        if needsPresentationFlush {
            playerItem.flushDecodedVideoPresentationBuffers()
            decodedVideoFrameOutput?.flush()
        }
    }
}

extension KSMEPlayer: @preconcurrency MediaPlayerProtocol {
    public var chapters: [Chapter] {
        playerItem.chapters
    }

    public var offlineSubtitleGeneratorInfo: (any SubtitleInfo)? {
        guard options.isOfflineSubtitleGenerationEnabled else {
            return nil
        }
        return options.offlineSubtitleGenerator
    }

    public var subtitleDataSouce: SubtitleDataSouce? { self }
    public var pictureInPictureSubtitleDiagnostic: PictureInPictureSubtitleDiagnostic {
        PictureInPictureSubtitlePolicyResolver.diagnostic(
            policy: options.pictureInPictureSubtitlePolicy,
            usesNativeLegibleSelection: false,
            supportsSampleBufferBurnIn: true
        )
    }

    public var playbackVolume: Float {
        get {
            audioOutput.volume
        }
        set {
            audioOutput.volume = newValue
        }
    }

    public var isPlaying: Bool { playbackState == .playing }

    @MainActor
    public var naturalSize: CGSize {
        options.display == .plane ? playerItem.naturalSize : KSOptions.sceneSize
    }

    public var isExternalPlaybackActive: Bool { false }

    public var view: UIView? { videoOutput }

    /// Unblocks the non-expanding decoded-video queue when the flat window stops presenting frames.
    public func drainWindowVideoPresentationQueue(maxFrames: Int = 16) {
        guard let videoOutput else {
            return
        }
        let frameBudget = max(1, maxFrames)
        for _ in 0 ..< frameBudget {
            if let metalView = videoOutput as? MetalPlayView {
                metalView.readNextFrame(force: false)
            } else {
                videoOutput.readNextFrame()
            }
        }
    }

    /// Clears queued window frames and drains the render queue after wiring `KSVideoFrameOutput`.
    public func flushDecodedVideoPresentationForFrameOutput(maxDrainFrames: Int = 16) {
        playerItem.flushDecodedVideoPresentationBuffers()
        drainWindowVideoPresentationQueue(maxFrames: maxDrainFrames)
    }

    /// Copies presentation flags from the shared `KSPlayerLayer` / view-model options into the
    /// player-owned `KSOptions` instance used by decode tracks and `MetalPlayView`.
    public func synchronizePresentationOptions(from source: KSOptions) {
        let hadSoftwareDecodePath = Self.requiresSoftwareVideoDecodePath(options)
        options.relaxVideoClockSyncWhileImmersiveCompositorActive =
            source.relaxVideoClockSyncWhileImmersiveCompositorActive
        options.suppressWindowVideoPresentationWhileImmersiveCompositorActive =
            source.suppressWindowVideoPresentationWhileImmersiveCompositorActive
        options.immersiveAudioPlaybackSeconds = source.immersiveAudioPlaybackSeconds
        options.immersivePresentVideoFrame = source.immersivePresentVideoFrame
        options.deliverDecodedVideoFrameToStereoCompositorWhileWindowVisible =
            source.deliverDecodedVideoFrameToStereoCompositorWhileWindowVisible
        options.videoAdaptable = source.videoAdaptable
        options.requiresDecodedVideoFrameOutput = source.requiresDecodedVideoFrameOutput
        options.hardwareDecode = source.hardwareDecode
        options.asynchronousDecompression = source.asynchronousDecompression
        videoOutput?.options = options
        let needsSoftwareDecodePath = Self.requiresSoftwareVideoDecodePath(options)
        guard needsSoftwareDecodePath else {
            return
        }
        if !hadSoftwareDecodePath {
            forceSoftwareVideoDecoders()
        } else {
            playerItem.forceSoftwareVideoDecoders()
        }
    }

    private static func requiresSoftwareVideoDecodePath(_ options: KSOptions) -> Bool {
        !options.hardwareDecode
            || options.requiresDecodedVideoFrameOutput
            || options.videoFrameOutput != nil
    }

    /// Drops active VideoToolbox decoders so the next packets use FFmpeg (required after turning off `hardwareDecode` mid-playback).
    public func forceSoftwareVideoDecoders() {
        playerItem.forceSoftwareVideoDecoders()
        flushDecodedVideoPresentationForFrameOutput(maxDrainFrames: 32)
    }

    public var isDecodedVideoFrameOutputWired: Bool {
        options.videoFrameOutput != nil
    }

    public func replace(url: URL, options: KSOptions) {
        KSLog("replaceUrl \(self)")
        KSOptions.setAudioSession(options: options, playbackPipeline: .decodedPCM)
        shutdown()
        playerItem.delegate = nil
        let inheritedVideoFrameOutput = decodedVideoFrameOutput ?? self.options.videoFrameOutput
        options.videoFrameOutput = inheritedVideoFrameOutput
        decodedVideoFrameOutput = inheritedVideoFrameOutput
        playerItem = MEPlayerItem(url: url, options: options)
        if options.videoDisable {
            videoOutput = nil
        } else if videoOutput == nil {
            videoOutput = KSOptions.videoPlayerType.init(options: options)
            videoOutput?.displayLayerDelegate = self
        }
        self.options = options
        playerItem.delegate = self
        audioOutput.flush()
        audioOutput.renderSource = playerItem
        videoOutput?.renderSource = playerItem
        videoOutput?.options = options
    }

    public var currentPlaybackTime: TimeInterval {
        get {
            playerItem.currentPlaybackTime
        }
        set {
            seek(time: newValue) { _ in }
        }
    }

    public var duration: TimeInterval { playerItem.duration }

    /// Nominal decoded video frame rate (PTS timeline), not display-link rate.
    public var sourceVideoFrameRate: Float { playerItem.sourceVideoFrameRate }

    public var fileSize: Double { playerItem.fileSize }

    public var seekable: Bool { playerItem.seekable }

    public var seekableTimeRange: MediaPlaybackTimeRange? { playerItem.seekableTimeRange }

    public var dynamicInfo: DynamicInfo? {
        playerItem.dynamicInfo
    }

    public func seek(time: TimeInterval, completion: @escaping ((Bool) -> Void)) {
        guard let time = MediaSeekTimeResolver.resolvedSeekTime(time, seekableTimeRange: seekableTimeRange) else {
            completion(false)
            return
        }
        playbackState = .seeking
        runOnMainThread { [weak self] in
            self?.bufferingProgress = 0
        }
        let seekTime: TimeInterval
        if time >= duration, options.isLoopPlay {
            seekTime = 0
        } else {
            seekTime = time
        }
        playerItem.seek(time: seekTime) { [weak self] result in
            guard let self else { return }
            if result {
                self.audioOutput.flush()
                runOnMainThread { [weak self] in
                    guard let self else { return }
                    if let controlTimebase = self.videoOutput?.displayLayer.controlTimebase {
                        CMTimebaseSetTime(controlTimebase, time: CMTimeMake(value: Int64(self.currentPlaybackTime), timescale: 1))
                    }
                }
            }
            completion(result)
        }
    }

    public func prepareToPlay() {
        KSLog("prepareToPlay \(self)")
        options.prepareTime = CACurrentMediaTime()
        playerItem.prepareToPlay()
        bufferingProgress = 0
    }

    public func play() {
        KSLog("play \(self)")
        playbackState = .playing
        if #available(iOS 15.0, tvOS 15.0, macOS 12.0, *) {
            pipController?.invalidatePlaybackState()
        }
    }

    public func pause() {
        KSLog("pause \(self)")
        playbackState = .paused
        if #available(iOS 15.0, tvOS 15.0, macOS 12.0, *) {
            pipController?.invalidatePlaybackState()
        }
    }

    public func shutdown() {
        KSLog("shutdown \(self)")
        playbackState = .stopped
        loadState = .idle
        isReadyToPlay = false
        loopCount = 0
        if options.isOfflineSubtitleGenerationEnabled {
            options.offlineSubtitleGenerator?.reset()
        }
        playerItem.shutdown()
        options.prepareTime = 0
        options.dnsStartTime = 0
        options.tcpStartTime = 0
        options.tcpConnectedTime = 0
        options.openTime = 0
        options.findTime = 0
        options.readyTime = 0
        options.readAudioTime = 0
        options.readVideoTime = 0
        options.decodeAudioTime = 0
        options.decodeVideoTime = 0
        if KSOptions.isClearVideoWhereReplace {
            videoOutput?.flush()
        }
    }

    @MainActor
    public var contentMode: UIViewContentMode {
        get {
            view?.contentMode ?? .center
        }
        set {
            view?.contentMode = newValue
        }
    }

    public func thumbnailImageAtCurrentTime() async -> CGImage? {
        videoOutput?.pixelBuffer?.cgImage()
    }

    public func enterBackground() {}

    public func enterForeground() {}

    public var isMuted: Bool {
        get {
            audioOutput.isMuted
        }
        set {
            audioOutput.isMuted = newValue
        }
    }

    public func tracks(mediaType: AVFoundation.AVMediaType) -> [MediaPlayerTrack] {
        playerItem.assetTracks.compactMap { track -> MediaPlayerTrack? in
            if track.mediaType == mediaType {
                return track
            } else if mediaType == .subtitle {
                return track.closedCaptionsTrack
            }
            return nil
        }
    }

    public func select(track: some MediaPlayerTrack) {
        let isSeek = playerItem.select(track: track)
        if isSeek {
            updateAudioRouteDiagnosticForCurrentAudioTrack()
            audioOutput.flush()
        }
    }

    func setPictureInPictureActive(_ active: Bool) {
        (videoOutput as? PictureInPictureSubtitleBurnInRendering)?.isPictureInPictureActive = active
        if !active {
            setPictureInPictureSubtitleSnapshot(nil)
        }
    }

    func setPictureInPictureSubtitleSnapshot(_ snapshot: PictureInPictureSubtitleSnapshot?) {
        (videoOutput as? PictureInPictureSubtitleBurnInRendering)?.pictureInPictureSubtitleSnapshot = snapshot
    }
}

extension KSMEPlayer: DecodedVideoFrameOutputConfigurable {}

@available(tvOS 14.0, *)
extension KSMEPlayer: AVPictureInPictureSampleBufferPlaybackDelegate {
    public func pictureInPictureController(_: AVPictureInPictureController, setPlaying playing: Bool) {
        playing ? play() : pause()
    }

    public func pictureInPictureControllerTimeRangeForPlayback(_: AVPictureInPictureController) -> CMTimeRange {
        // Handle live streams.
        if duration == 0 {
            return CMTimeRange(start: .negativeInfinity, duration: .positiveInfinity)
        }
        return CMTimeRange(start: 0, end: duration)
    }

    public func pictureInPictureControllerIsPlaybackPaused(_: AVPictureInPictureController) -> Bool {
        !isPlaying
    }

    public func pictureInPictureController(_: AVPictureInPictureController, didTransitionToRenderSize _: CMVideoDimensions) {}
    public func pictureInPictureController(_: AVPictureInPictureController, skipByInterval skipInterval: CMTime) async {
        seek(time: currentPlaybackTime + skipInterval.seconds) { _ in }
    }

    public func pictureInPictureControllerShouldProhibitBackgroundAudioPlayback(_: AVPictureInPictureController) -> Bool {
        false
    }
}

@available(macOS 12.0, iOS 15.0, tvOS 15.0, *)
extension KSMEPlayer: AVPlaybackCoordinatorPlaybackControlDelegate {
    public func playbackCoordinator(_: AVDelegatingPlaybackCoordinator, didIssue playCommand: AVDelegatingPlaybackCoordinatorPlayCommand, completionHandler: @escaping @Sendable () -> Void) {
        guard playCommand.expectedCurrentItemIdentifier == (playbackCoordinator as? AVDelegatingPlaybackCoordinator)?.currentItemIdentifier else {
            completionHandler()
            return
        }
        DispatchQueue.main.async { [weak self] in
            guard let self else {
                return
            }
            if self.playbackState != .playing {
                self.play()
            }
            completionHandler()
        }
    }

    public func playbackCoordinator(_: AVDelegatingPlaybackCoordinator, didIssue pauseCommand: AVDelegatingPlaybackCoordinatorPauseCommand, completionHandler: @escaping @Sendable () -> Void) {
        guard pauseCommand.expectedCurrentItemIdentifier == (playbackCoordinator as? AVDelegatingPlaybackCoordinator)?.currentItemIdentifier else {
            completionHandler()
            return
        }
        DispatchQueue.main.async { [weak self] in
            guard let self else {
                return
            }
            if self.playbackState != .paused {
                self.pause()
            }
            completionHandler()
        }
    }

    public func playbackCoordinator(_: AVDelegatingPlaybackCoordinator, didIssue seekCommand: AVDelegatingPlaybackCoordinatorSeekCommand) async {
        guard seekCommand.expectedCurrentItemIdentifier == (playbackCoordinator as? AVDelegatingPlaybackCoordinator)?.currentItemIdentifier else {
            return
        }
        let seekTime = fmod(seekCommand.itemTime.seconds, duration)
        if abs(currentPlaybackTime - seekTime) < CGFLOAT_EPSILON {
            return
        }
        seek(time: seekTime) { _ in }
    }

    public func playbackCoordinator(_: AVDelegatingPlaybackCoordinator, didIssue bufferingCommand: AVDelegatingPlaybackCoordinatorBufferingCommand, completionHandler: @escaping @Sendable () -> Void) {
        guard bufferingCommand.expectedCurrentItemIdentifier == (playbackCoordinator as? AVDelegatingPlaybackCoordinator)?.currentItemIdentifier else {
            completionHandler()
            return
        }
        DispatchQueue.main.async { [weak self] in
            guard let self else {
                return
            }
            guard self.loadState != .playable, let countDown = bufferingCommand.completionDueDate?.timeIntervalSinceNow else {
                completionHandler()
                return
            }
            self.bufferingCountDownTimer?.invalidate()
            self.bufferingCountDownTimer = nil
            self.bufferingCountDownTimer = Timer(timeInterval: countDown, repeats: false) { _ in
                completionHandler()
            }
        }
    }
}

extension KSMEPlayer: DisplayLayerDelegate {
    public func change(displayLayer: AVSampleBufferDisplayLayer) {
        if #available(iOS 15.0, tvOS 15.0, macOS 12.0, *) {
            if pipController?.isPictureInPictureActive == true {
                pipController?.stop(restoreUserInterface: false)
                KSLog("[pip] stopped sample-buffer Picture in Picture because the display layer changed")
            }
            let contentSource = AVPictureInPictureController.ContentSource(sampleBufferDisplayLayer: displayLayer, playbackDelegate: self)
            _pipController = KSPictureInPictureController(contentSource: contentSource)
            // 更改contentSource会直接crash
//            pipController?.contentSource = contentSource
        }
    }
}

public extension KSMEPlayer {
    func startRecord(url: URL, progress: (@Sendable (StreamRecordingProgress) -> Void)? = nil) {
        playerItem.startRecord(url: url, progress: progress)
    }

    func stopRecord() {
        playerItem.stopRecord()
    }

    @available(*, deprecated, renamed: "stopRecord()")
    func stoptRecord() {
        stopRecord()
    }
}

public struct StreamRecordingSupportDiagnostic: Equatable, Sendable {
    public let isSupported: Bool
    public let message: String

    public init(isSupported: Bool, message: String) {
        self.isSupported = isSupported
        self.message = message
    }
}

enum StreamRecordingSourcePolicy {
    static func diagnostic(playerType: MediaPlayerProtocol.Type, audioURL: URL?) -> StreamRecordingSupportDiagnostic {
        if let audioURL {
            return StreamRecordingSupportDiagnostic(
                isSupported: false,
                message: "Live stream recording does not merge separate audio sources. The active audio URL is \(audioURL.ksRedactedAbsoluteString)."
            )
        }
        if playerType == KSMEPlayer.self {
            return StreamRecordingSupportDiagnostic(
                isSupported: true,
                message: "KSMEPlayer can stream-copy active demuxed audio/video and compatible embedded subtitle streams to a temporary file, then finalize on stop."
            )
        }
        return StreamRecordingSupportDiagnostic(
            isSupported: false,
            message: "Live stream recording is KSMEPlayer-only; AVPlayer/native playback does not expose packets for KSPlayer stream-copy recording."
        )
    }
}

public extension KSPlayerLayer {
    var streamRecordingSupportDiagnostic: StreamRecordingSupportDiagnostic {
        StreamRecordingSourcePolicy.diagnostic(playerType: type(of: player), audioURL: audioURL)
    }
}

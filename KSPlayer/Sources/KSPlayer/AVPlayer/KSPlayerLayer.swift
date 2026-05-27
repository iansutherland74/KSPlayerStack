//
//  KSPlayerLayerView.swift
//  Pods
//
//  Created by kintan on 16/4/28.
//
//
import AVFoundation
import AVKit
import MediaPlayer
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

/**
 Player status emun
 - setURL:      set url
 - readyToPlay:    player ready to play
 - buffering:      player buffering
 - bufferFinished: buffer finished
 - playedToTheEnd: played to the End
 - error:          error with playing
 */
public enum KSPlayerState: CustomStringConvertible, Sendable {
    case initialized
    case preparing
    case readyToPlay
    case buffering
    case bufferFinished
    case paused
    case playedToTheEnd
    case error
    public var description: String {
        switch self {
        case .initialized:
            return "initialized"
        case .preparing:
            return "preparing"
        case .readyToPlay:
            return "readyToPlay"
        case .buffering:
            return "buffering"
        case .bufferFinished:
            return "bufferFinished"
        case .paused:
            return "paused"
        case .playedToTheEnd:
            return "playedToTheEnd"
        case .error:
            return "error"
        }
    }

    public var isPlaying: Bool { self == .buffering || self == .bufferFinished }
}

@MainActor
public protocol KSPlayerLayerDelegate: AnyObject {
    func player(layer: KSPlayerLayer, state: KSPlayerState)
    func player(layer: KSPlayerLayer, currentTime: TimeInterval, totalTime: TimeInterval)
    func player(layer: KSPlayerLayer, finish error: Error?)
    func player(layer: KSPlayerLayer, bufferedCount: Int, consumeTime: TimeInterval)
}

struct DefinitionSwitchPrewarmPolicy {
    static func canPrewarm(isEnabled: Bool, duration: TimeInterval, targetTime: TimeInterval, isSeekable: Bool, isExternalPlaybackActive: Bool, isPictureInPictureActive: Bool) -> Bool {
        isEnabled
            && duration.isFinite
            && duration > 0
            && targetTime.isFinite
            && targetTime >= 0
            && targetTime < duration
            && isSeekable
            && !isExternalPlaybackActive
            && !isPictureInPictureActive
    }

    static func handoffTime(requestedTime: TimeInterval, elapsed: TimeInterval, playbackRate: Float, wasPlaying: Bool, duration: TimeInterval) -> TimeInterval {
        guard requestedTime.isFinite, duration.isFinite, duration > 0 else {
            return max(requestedTime, 0)
        }
        let advancedTime = wasPlaying ? requestedTime + max(elapsed, 0) * Double(playbackRate) : requestedTime
        return min(max(advancedTime, 0), duration)
    }
}

struct DefinitionSwitchTrackSelection {
    let mediaType: AVMediaType
    let trackID: Int32
    let name: String
    let languageCode: String?

    init?(track: (any MediaPlayerTrack)?) {
        guard let track else {
            return nil
        }
        mediaType = track.mediaType
        trackID = track.trackID
        name = track.name
        languageCode = track.languageCode
    }

    init(mediaType: AVMediaType, trackID: Int32, name: String, languageCode: String?) {
        self.mediaType = mediaType
        self.trackID = trackID
        self.name = name
        self.languageCode = languageCode
    }

    func matchingTrack(in player: any MediaPlayerProtocol) -> (any MediaPlayerTrack)? {
        let tracks = player.tracks(mediaType: mediaType)
        return tracks.first { $0.trackID == trackID } ?? tracks.first {
            $0.name == name && $0.languageCode == languageCode
        }
    }
}

private final class DefinitionSwitchPrewarmDelegate: MediaPlayerDelegate {
    nonisolated(unsafe) var readyToPlayHandler: (@MainActor (any MediaPlayerProtocol) -> Void)?
    nonisolated(unsafe) var loadStateHandler: (@MainActor (any MediaPlayerProtocol) -> Void)?
    nonisolated(unsafe) var finishHandler: (@MainActor (any MediaPlayerProtocol, Error?) -> Void)?

    func readyToPlay(player: some MediaPlayerProtocol) {
        readyToPlayHandler?(player)
    }

    func changeLoadState(player: some MediaPlayerProtocol) {
        loadStateHandler?(player)
    }

    func changeBuffering(player _: some MediaPlayerProtocol, progress _: Int) {}

    func playBack(player _: some MediaPlayerProtocol, loopCount _: Int) {}

    func finish(player: some MediaPlayerProtocol, error: Error?) {
        finishHandler?(player, error)
    }
}

private final class DefinitionSwitchPrewarmContext {
    let player: any MediaPlayerProtocol
    let delegate: DefinitionSwitchPrewarmDelegate
    let url: URL
    let audioURL: URL?
    let options: KSOptions
    let requestedTime: TimeInterval
    let startedAt = CACurrentMediaTime()
    let shouldAutoPlayAfterSwitch: Bool
    let playbackRate: Float
    let playbackVolume: Float
    let selectedTrackSelections: [DefinitionSwitchTrackSelection]
    var timeoutWorkItem: DispatchWorkItem?
    var didSeek = false

    init(player: any MediaPlayerProtocol, delegate: DefinitionSwitchPrewarmDelegate, url: URL, audioURL: URL?, options: KSOptions, requestedTime: TimeInterval,
         shouldAutoPlayAfterSwitch: Bool, playbackRate: Float, playbackVolume: Float, selectedTrackSelections: [DefinitionSwitchTrackSelection])
    {
        self.player = player
        self.delegate = delegate
        self.url = url
        self.audioURL = audioURL
        self.options = options
        self.requestedTime = requestedTime
        self.shouldAutoPlayAfterSwitch = shouldAutoPlayAfterSwitch
        self.playbackRate = playbackRate
        self.playbackVolume = playbackVolume
        self.selectedTrackSelections = selectedTrackSelections
    }

    func cancel() {
        timeoutWorkItem?.cancel()
        timeoutWorkItem = nil
        player.shutdown()
    }
}

open class KSPlayerLayer: NSObject, @unchecked Sendable {
    public weak var delegate: KSPlayerLayerDelegate?
    @Published
    public var bufferingProgress: Int = 0
    @Published
    public var loopCount: Int = 0
    @Published
    public var isPipActive = false {
        didSet {
            guard !isUpdatingPictureInPictureState else {
                return
            }
            if #available(tvOS 14.0, *) {
                guard let pipController = player.pipController else {
                    if isPipActive {
                        KSLog("[pip] start skipped: \(PictureInPictureStartPolicy.unavailableReason(isSystemSupported: AVPictureInPictureController.isPictureInPictureSupported(), hasController: false, isPossible: false) ?? "unknown reason")")
                        setPictureInPictureActiveFromController(false)
                    }
                    return
                }

                if isPipActive {
                    options.pictureInPictureSubtitleDiagnostic = player.pictureInPictureSubtitleDiagnostic
                    KSLog("[pip] subtitle policy: \(player.pictureInPictureSubtitleDiagnostic.description)")
                    (player as? KSMEPlayer)?.setPictureInPictureActive(true)
                    // 一定要async才不会pip之后就暂停播放
                    Task { @MainActor [weak self] in
                        guard let self, self.isPipActive else { return }
                        if self.player.pipController?.start(view: self) != true {
                            self.setPictureInPictureActiveFromController(false)
                        }
                    }
                } else {
                    (player as? KSMEPlayer)?.setPictureInPictureActive(false)
                    pipController.stop(restoreUserInterface: true)
                }
            }
        }
    }

    public private(set) var options: KSOptions

    /// Decoded video frame callback for app-owned ML pipelines such as Depth Anything.
    ///
    /// KSPlayer emits `CVPixelBuffer`s from the KSMEPlayer decode path on a private serial queue. The callback is not
    /// invoked on the main thread, and the queue drops frames according to `videoOutputConfiguration` when the app is
    /// slower than playback.
    public var videoOutput: KSVideoOutputHandler? {
        didSet {
            updateDecodedVideoFrameOutput()
        }
    }

    /// Buffering and drop policy for `videoOutput`. Defaults to a single queued frame and keeping the latest frame.
    public var videoOutputConfiguration = KSVideoFrameOutput.Configuration() {
        didSet {
            updateDecodedVideoFrameOutput()
        }
    }

    public var player: MediaPlayerProtocol {
        didSet {
            KSLog("player is \(player)")
            (oldValue as? DecodedVideoFrameOutputConfigurable)?.decodedVideoFrameOutput = nil
            if #available(tvOS 14.0, *), isPipActive {
                stopPictureInPictureForPlayerReplacement(oldPlayer: oldValue)
            }
            state = .initialized
            runOnMainThread { [weak self] in
                guard let self else { return }
                if let oldView = oldValue.view, let superview = oldView.superview, let view = player.view {
                    #if canImport(UIKit)
                    superview.insertSubview(view, belowSubview: oldView)
                    #else
                    superview.addSubview(view, positioned: .below, relativeTo: oldView)
                    #endif
                    view.translatesAutoresizingMaskIntoConstraints = false
                    NSLayoutConstraint.activate([
                        view.topAnchor.constraint(equalTo: superview.topAnchor),
                        view.leadingAnchor.constraint(equalTo: superview.leadingAnchor),
                        view.bottomAnchor.constraint(equalTo: superview.bottomAnchor),
                        view.trailingAnchor.constraint(equalTo: superview.trailingAnchor),
                    ])
                }
                oldValue.view?.removeFromSuperview()
            }
            player.playbackRate = oldValue.playbackRate
            player.playbackVolume = oldValue.playbackVolume
            player.delegate = self
            player.contentMode = .scaleAspectFit
            applyDecodedVideoFrameOutput(to: player)
            if isAutoPlay, !isCommittingPrewarmedPlayer {
                prepareToPlay()
            }
        }
    }

    public private(set) var url: URL {
        didSet {
            if isCommittingPrewarmedPlayer {
                return
            }
            let firstPlayerType = preferredPlayerType(for: url, audioURL: audioURL, respectsWirelessRoute: true)
            if type(of: player) == firstPlayerType {
                if url == oldValue {
                    if isAutoPlay {
                        play()
                    }
                } else {
                    stop()
                    player.replace(url: url, audioURL: audioURL, options: options)
                    if isAutoPlay {
                        prepareToPlay()
                    }
                }
            } else {
                stop()
                player = firstPlayerType.init(url: url, audioURL: audioURL, options: options)
            }
        }
    }

    public private(set) var audioURL: URL?

    /// 播发器的几种状态

    public private(set) var state = KSPlayerState.initialized {
        willSet {
            if state != newValue {
                Task { @MainActor [weak self] in
                    guard let self else { return }
                    KSLog("playerStateDidChange - \(newValue)")
                    self.delegate?.player(layer: self, state: newValue)
                }
            }
        }
    }

    private lazy var timer: Timer = .scheduledTimer(withTimeInterval: 0.1, repeats: true) { [weak self] _ in
        MainActor.assumeIsolated {
            guard let self, self.player.isReadyToPlay else {
                return
            }
            self.delegate?.player(layer: self, currentTime: self.player.currentPlaybackTime, totalTime: self.player.duration)
            if self.player.playbackState == .playing, self.player.loadState == .playable, self.state == .buffering {
                // 一个兜底保护，正常不能走到这里
                self.state = .bufferFinished
            }
            if self.player.isPlaying {
                MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPNowPlayingInfoPropertyElapsedPlaybackTime] = self.player.currentPlaybackTime
            }
        }
    }

    private var urls = [URL]()
    private var isAutoPlay: Bool
    private var isWirelessRouteActive = false
    private var bufferedCount = 0
    private var shouldSeekTo: TimeInterval = 0
    private var startTime: TimeInterval = 0
    private var prewarmContext: DefinitionSwitchPrewarmContext?
    private var isCommittingPrewarmedPlayer = false
    private var isUpdatingPictureInPictureState = false
    private var decodedVideoFrameOutput: KSVideoFrameOutput?
    public init(url: URL, audioURL: URL? = nil, isAutoPlay: Bool = KSOptions.isAutoPlay, options: KSOptions, delegate: KSPlayerLayerDelegate? = nil) {
        self.url = url
        self.audioURL = audioURL
        self.options = options
        self.delegate = delegate
        let firstPlayerType = Self.preferredPlayerType(for: url, audioURL: audioURL, options: options)
        player = firstPlayerType.init(url: url, audioURL: audioURL, options: options)
        self.isAutoPlay = isAutoPlay
        super.init()
        player.playbackRate = options.startPlayRate
        if options.registerRemoteControll {
            registerRemoteControllEvent()
        }
        player.delegate = self
        player.contentMode = .scaleAspectFit
        applyDecodedVideoFrameOutput(to: player)
        if isAutoPlay {
            prepareToPlay()
        }
        #if canImport(UIKit)
        runOnMainThread { [weak self] in
            guard let self else { return }
            NotificationCenter.default.addObserver(self, selector: #selector(enterBackground), name: UIApplication.didEnterBackgroundNotification, object: nil)
            NotificationCenter.default.addObserver(self, selector: #selector(enterForeground), name: UIApplication.willEnterForegroundNotification, object: nil)
        }
        #if !os(xrOS)
        NotificationCenter.default.addObserver(self, selector: #selector(wirelessRouteActiveDidChange(notification:)), name: .MPVolumeViewWirelessRouteActiveDidChange, object: nil)
        #endif
        #endif
        #if !os(macOS)
        NotificationCenter.default.addObserver(self, selector: #selector(audioInterrupted), name: AVAudioSession.interruptionNotification, object: nil)
        #endif
    }

    @available(*, unavailable)
    public required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    deinit {
        cancelPrewarmContext()
        (player as? DecodedVideoFrameOutputConfigurable)?.decodedVideoFrameOutput = nil
        if #available(iOS 15.0, tvOS 15.0, macOS 12.0, *) {
            player.pipController?.contentSource = nil
        }
        NotificationCenter.default.removeObserver(self)
        MPNowPlayingInfoCenter.default().nowPlayingInfo = nil
        MPRemoteCommandCenter.shared().playCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().pauseCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().togglePlayPauseCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().stopCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().nextTrackCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().previousTrackCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().changeRepeatModeCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().changePlaybackRateCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().skipForwardCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().skipBackwardCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().changePlaybackPositionCommand.removeTarget(nil)
        MPRemoteCommandCenter.shared().enableLanguageOptionCommand.removeTarget(nil)
        options.playerLayerDeinit()
    }

    public func set(url: URL, options: KSOptions) {
        set(url: url, audioURL: nil, options: options)
    }

    public func set(url: URL, audioURL: URL?, options: KSOptions) {
        let previousUpscaling = self.options.videoUpscaling
        let previousColorAdjustment = self.options.videoColorAdjustment
        let previous2DTo3DMode = self.options.video2DTo3DMode
        let previous2DTo3DDepthStrength = self.options.video2DTo3DDepthStrength
        let previous2DTo3DDepthDistance = self.options.video2DTo3DDepthDistance
        let previous2DTo3DDepthCurvature = self.options.video2DTo3DDepthCurvature
        let previous2DTo3DDepthSmoothingFactor = self.options.video2DTo3DDepthSmoothingFactor
        let previous2DTo3DOutputLayout = self.options.video2DTo3DOutputLayout
        let previousDepthProvider = self.options.videoDepthEstimationProvider
        let previous2DTo3DRequiresMetal = Video2DTo3DPolicy.requiresMetalRenderPath(mode: previous2DTo3DMode)
        let current2DTo3DRequiresMetal = Video2DTo3DPolicy.requiresMetalRenderPath(mode: options.video2DTo3DMode)
        let previousAudioURL = self.audioURL
        self.options = options
        runOnMainThread {
            self.cancelPrewarmContext()
            if self.url == url {
                if previousUpscaling != options.videoUpscaling ||
                    previousColorAdjustment != options.videoColorAdjustment ||
                    previous2DTo3DRequiresMetal != current2DTo3DRequiresMetal ||
                    (current2DTo3DRequiresMetal && (
                        previous2DTo3DMode != options.video2DTo3DMode ||
                            previous2DTo3DDepthStrength != options.video2DTo3DDepthStrength ||
                            previous2DTo3DDepthDistance != options.video2DTo3DDepthDistance ||
                            previous2DTo3DDepthCurvature != options.video2DTo3DDepthCurvature ||
                            previous2DTo3DDepthSmoothingFactor != options.video2DTo3DDepthSmoothingFactor ||
                            previous2DTo3DOutputLayout != options.video2DTo3DOutputLayout ||
                            !Self.sameDepthProvider(previousDepthProvider, options.videoDepthEstimationProvider)
                    )) ||
                    previousAudioURL != audioURL
                {
                    self.audioURL = audioURL
                    self.replaceCurrentURLForOptionChange(url: url, audioURL: audioURL, options: options)
                } else if self.isAutoPlay {
                    self.play()
                }
            } else {
                self.audioURL = audioURL
                self.url = url
            }
        }
    }

    public func set(url: URL, options: KSOptions, preservingCurrentTime targetTime: TimeInterval) {
        set(url: url, audioURL: nil, options: options, preservingCurrentTime: targetTime)
    }

    public func set(url: URL, audioURL: URL?, options: KSOptions, preservingCurrentTime targetTime: TimeInterval) {
        runOnMainThread {
            self.replaceURLPreservingPlaybackTime(url: url, audioURL: audioURL, options: options, targetTime: targetTime)
        }
    }

    public func set(urls: [URL], options: KSOptions) {
        let previousUpscaling = self.options.videoUpscaling
        let previousColorAdjustment = self.options.videoColorAdjustment
        let previous2DTo3DMode = self.options.video2DTo3DMode
        let previous2DTo3DDepthStrength = self.options.video2DTo3DDepthStrength
        let previous2DTo3DDepthDistance = self.options.video2DTo3DDepthDistance
        let previous2DTo3DDepthCurvature = self.options.video2DTo3DDepthCurvature
        let previous2DTo3DDepthSmoothingFactor = self.options.video2DTo3DDepthSmoothingFactor
        let previous2DTo3DOutputLayout = self.options.video2DTo3DOutputLayout
        let previousDepthProvider = self.options.videoDepthEstimationProvider
        let previous2DTo3DRequiresMetal = Video2DTo3DPolicy.requiresMetalRenderPath(mode: previous2DTo3DMode)
        let current2DTo3DRequiresMetal = Video2DTo3DPolicy.requiresMetalRenderPath(mode: options.video2DTo3DMode)
        self.options = options
        self.audioURL = nil
        self.urls.removeAll()
        self.urls.append(contentsOf: urls)
        if let first = urls.first {
            runOnMainThread {
                self.cancelPrewarmContext()
                let didRendererOptionsChange = previousUpscaling != options.videoUpscaling ||
                    previousColorAdjustment != options.videoColorAdjustment ||
                    previous2DTo3DRequiresMetal != current2DTo3DRequiresMetal ||
                    (current2DTo3DRequiresMetal && (
                        previous2DTo3DMode != options.video2DTo3DMode ||
                            previous2DTo3DDepthStrength != options.video2DTo3DDepthStrength ||
                            previous2DTo3DDepthDistance != options.video2DTo3DDepthDistance ||
                            previous2DTo3DDepthCurvature != options.video2DTo3DDepthCurvature ||
                            previous2DTo3DDepthSmoothingFactor != options.video2DTo3DDepthSmoothingFactor ||
                            previous2DTo3DOutputLayout != options.video2DTo3DOutputLayout ||
                            !Self.sameDepthProvider(previousDepthProvider, options.videoDepthEstimationProvider)
                    ))
                if self.url == first, didRendererOptionsChange {
                    self.replaceCurrentURLForOptionChange(url: first, audioURL: nil, options: options)
                } else {
                    self.url = first
                }
            }
        }
    }

    open func play() {
        Task { @MainActor in
            UIApplication.shared.isIdleTimerDisabled = true
        }
        isAutoPlay = true
        if state == .error || state == .initialized {
            prepareToPlay()
        }
        if player.isReadyToPlay {
            if state == .playedToTheEnd {
                player.seek(time: 0) { [weak self] finished in
                    guard let self else { return }
                    if finished {
                        self.player.play()
                    }
                }
            } else {
                player.play()
            }
            timer.fireDate = Date.distantPast
        }
        state = player.loadState == .playable ? .bufferFinished : .buffering
        MPNowPlayingInfoCenter.default().playbackState = .playing
        if #available(tvOS 14.0, *) {
            KSPictureInPictureController.mute()
        }
    }

    open func pause() {
        isAutoPlay = false
        player.pause()
        timer.fireDate = Date.distantFuture
        state = .paused
        MPNowPlayingInfoCenter.default().playbackState = .paused
        Task { @MainActor in
            UIApplication.shared.isIdleTimerDisabled = false
        }
    }

    public func stop() {
        KSLog("stop Player")
        cancelPrewarmContext()
        state = .initialized
        player.shutdown()
        bufferedCount = 0
        shouldSeekTo = 0
        player.playbackRate = 1
        player.playbackVolume = 1
        MPNowPlayingInfoCenter.default().nowPlayingInfo = nil
        Task { @MainActor in
            UIApplication.shared.isIdleTimerDisabled = false
        }
    }

    open func seek(time: TimeInterval, autoPlay: Bool, completion: @escaping ((Bool) -> Void)) {
        guard let resolvedTime = MediaSeekTimeResolver.resolvedSeekTime(time, seekableTimeRange: player.seekableTimeRange) else {
            completion(false)
            return
        }
        if player.isReadyToPlay, player.seekable {
            player.seek(time: resolvedTime) { [weak self] finished in
                guard let self else { return }
                if finished, autoPlay {
                    self.play()
                }
                completion(finished)
            }
        } else {
            isAutoPlay = autoPlay
            shouldSeekTo = resolvedTime
            completion(false)
        }
    }
}

// MARK: - MediaPlayerDelegate

extension KSPlayerLayer: MediaPlayerDelegate {
    public func readyToPlay(player: some MediaPlayerProtocol) {
        state = .readyToPlay
        #if os(macOS)
        if let window = player.view?.window {
            window.isMovableByWindowBackground = true
            if options.automaticWindowResize {
                let naturalSize = player.naturalSize
                if naturalSize.width > 0, naturalSize.height > 0 {
                    window.aspectRatio = naturalSize
                    var frame = window.frame
                    frame.size.height = frame.width * naturalSize.height / naturalSize.width
                    window.setFrame(frame, display: true)
                }
            }
        }
        #endif
        #if !os(macOS) && !os(tvOS)
        if #available(iOS 14.2, *) {
            if options.canStartPictureInPictureAutomaticallyFromInline {
                player.pipController?.canStartPictureInPictureAutomaticallyFromInline = true
            }
        }
        #endif
        updateNowPlayingInfo()
        if shouldSeekTo > 0 {
            seek(time: shouldSeekTo, autoPlay: isAutoPlay) { [weak self] _ in
                guard let self else { return }
                self.shouldSeekTo = 0
            }
        } else if isAutoPlay {
            play()
        }
    }

    public func changeLoadState(player: some MediaPlayerProtocol) {
        guard player.playbackState != .seeking else { return }
        if player.playbackState == .paused {
            state = .paused
            timer.fireDate = Date.distantFuture
            MPNowPlayingInfoCenter.default().playbackState = .paused
            return
        }
        if player.playbackState == .playing, !state.isPlaying {
            state = player.loadState == .playable ? .bufferFinished : .buffering
            timer.fireDate = Date.distantPast
            MPNowPlayingInfoCenter.default().playbackState = .playing
        }
        if player.loadState == .playable, startTime > 0 {
            let diff = CACurrentMediaTime() - startTime
            Task { @MainActor [weak self] in
                guard let self else { return }
                delegate?.player(layer: self, bufferedCount: bufferedCount, consumeTime: diff)
            }
            if bufferedCount == 0 {
                var dic = ["firstTime": diff]
                if options.tcpConnectedTime > 0 {
                    dic["initTime"] = options.dnsStartTime - startTime
                    dic["dnsTime"] = options.tcpStartTime - options.dnsStartTime
                    dic["tcpTime"] = options.tcpConnectedTime - options.tcpStartTime
                    dic["openTime"] = options.openTime - options.tcpConnectedTime
                    dic["findTime"] = options.findTime - options.openTime
                } else {
                    dic["openTime"] = options.openTime - startTime
                }
                dic["findTime"] = options.findTime - options.openTime
                dic["readyTime"] = options.readyTime - options.findTime
                dic["readVideoTime"] = options.readVideoTime - options.readyTime
                dic["readAudioTime"] = options.readAudioTime - options.readyTime
                dic["decodeVideoTime"] = options.decodeVideoTime - options.readVideoTime
                dic["decodeAudioTime"] = options.decodeAudioTime - options.readAudioTime
                KSLog(dic)
            }
            bufferedCount += 1
            startTime = 0
        }
        guard state.isPlaying else { return }
        if player.loadState == .playable {
            state = .bufferFinished
        } else {
            if state == .bufferFinished {
                startTime = CACurrentMediaTime()
            }
            state = .buffering
        }
    }

    public func changeBuffering(player _: some MediaPlayerProtocol, progress: Int) {
        bufferingProgress = progress
    }

    public func playBack(player _: some MediaPlayerProtocol, loopCount: Int) {
        self.loopCount = loopCount
    }

    public func finish(player: some MediaPlayerProtocol, error: Error?) {
        if let error {
            if audioURL == nil, type(of: player) != KSOptions.secondPlayerType, let secondPlayerType = KSOptions.secondPlayerType {
                self.player = secondPlayerType.init(url: url, audioURL: nil, options: options)
                return
            }
            state = .error
            KSLog(error as CustomStringConvertible)
        } else {
            let duration = player.duration
            Task { @MainActor [weak self] in
                guard let self else { return }
                delegate?.player(layer: self, currentTime: duration, totalTime: duration)
            }
            state = .playedToTheEnd
        }
        timer.fireDate = Date.distantFuture
        bufferedCount = 1
        Task { @MainActor [weak self] in
            guard let self else { return }
            delegate?.player(layer: self, finish: error)
        }
        if error == nil {
            nextPlayer()
        }
    }
}

// MARK: - AVPictureInPictureControllerDelegate

@available(tvOS 14.0, *)
extension KSPlayerLayer: AVPictureInPictureControllerDelegate {
    public func pictureInPictureControllerDidStartPictureInPicture(_: AVPictureInPictureController) {
        setPictureInPictureActiveFromController(true)
    }

    public func pictureInPictureController(_ pictureInPictureController: AVPictureInPictureController, failedToStartPictureInPictureWithError error: Error) {
        KSLog("[pip] failed to start Picture in Picture: \(error.localizedDescription)")
        (pictureInPictureController as? KSPictureInPictureController)?.stop(restoreUserInterface: false)
        setPictureInPictureActiveFromController(false)
    }

    public func pictureInPictureControllerDidStopPictureInPicture(_ pictureInPictureController: AVPictureInPictureController) {
        setPictureInPictureActiveFromController(false)
        (pictureInPictureController as? KSPictureInPictureController)?.stop(restoreUserInterface: false)
    }

    public func pictureInPictureController(_: AVPictureInPictureController, restoreUserInterfaceForPictureInPictureStopWithCompletionHandler completionHandler: @escaping (Bool) -> Void) {
        isPipActive = false
        completionHandler(true)
    }
}

// MARK: - private functions

extension KSPlayerLayer {
    private func updateDecodedVideoFrameOutput() {
        decodedVideoFrameOutput?.flush()
        if let videoOutput {
            decodedVideoFrameOutput = KSVideoFrameOutput(
                configuration: videoOutputConfiguration,
                handler: videoOutput
            )
        } else {
            decodedVideoFrameOutput = nil
        }
        applyDecodedVideoFrameOutput(to: player)
    }

    private func applyDecodedVideoFrameOutput(to player: any MediaPlayerProtocol) {
        (player as? DecodedVideoFrameOutputConfigurable)?.decodedVideoFrameOutput = decodedVideoFrameOutput
    }

    private static func sameDepthProvider(_ lhs: (any VideoDepthEstimationProvider)?, _ rhs: (any VideoDepthEstimationProvider)?) -> Bool {
        switch (lhs, rhs) {
        case let (lhs?, rhs?):
            return lhs === rhs
        case (nil, nil):
            return true
        default:
            return false
        }
    }

    fileprivate func setPictureInPictureActiveFromController(_ active: Bool) {
        guard isPipActive != active else {
            return
        }
        (player as? KSMEPlayer)?.setPictureInPictureActive(active)
        isUpdatingPictureInPictureState = true
        defer {
            isUpdatingPictureInPictureState = false
        }
        isPipActive = active
    }

    @available(tvOS 14.0, *)
    fileprivate func stopPictureInPictureForPlayerReplacement(oldPlayer: any MediaPlayerProtocol) {
        (oldPlayer as? KSMEPlayer)?.setPictureInPictureActive(false)
        oldPlayer.pipController?.stop(restoreUserInterface: true)
        setPictureInPictureActiveFromController(false)
        KSLog("[pip] stopped Picture in Picture because the player was replaced")
    }

    static func preferredPlayerType(for url: URL, audioURL: URL? = nil, options: KSOptions) -> MediaPlayerProtocol.Type {
        if audioURL != nil {
            return KSAVPlayer.self
        }
        if let reason = Video2DTo3DPolicy.unavailableReason(mode: options.video2DTo3DMode) {
            options.video2DTo3DDiagnostic = .unavailable(reason: reason)
            KSLog("[video] \(reason)")
        }
        if options.display != .plane ||
            options.panoramaMode != .disabled ||
            options.stereoscopicVideoMode != .disabled ||
            Video2DTo3DPolicy.requiresMetalRenderPath(mode: options.video2DTo3DMode) ||
            options.requiresDecodedVideoFrameOutput ||
            options.videoFrameOutput != nil ||
            url.isBluRayInputCandidate ||
            url.isFFmpegOnlyInputScheme ||
            url.isMatroskaContainer ||
            !options.videoColorAdjustment.isNeutral ||
            options.videoUpscaling.isEnabled ||
            (options.isOfflineSubtitleGenerationEnabled && options.offlineSubtitleGenerator != nil)
        {
            return KSMEPlayer.self
        }
        return KSOptions.firstPlayerType
    }

    private func preferredPlayerType(for url: URL, audioURL: URL?, respectsWirelessRoute: Bool) -> MediaPlayerProtocol.Type {
        if respectsWirelessRoute, isWirelessRouteActive, !url.isBluRayInputCandidate, !url.isFFmpegOnlyInputScheme, !url.isMatroskaContainer {
            // airplay的话，默认使用KSAVPlayer
            if options.videoUpscaling.isEnabled {
                options.videoUpscalingState = .unavailable(reason: "video upscaling is unavailable during wireless route playback")
            }
            if !options.videoColorAdjustment.isNeutral {
                KSLog("[video] color adjustment is unavailable during wireless route playback")
            }
            if options.panoramaMode != .disabled {
                KSLog("[video] panorama rendering is unavailable during wireless route playback")
            }
            if options.stereoscopicVideoMode != .disabled {
                KSLog("[video] stereoscopic 3D rendering is unavailable during wireless route playback")
            }
            if options.video2DTo3DMode != .disabled {
                let reason = Video2DTo3DPolicy.unavailableReason(mode: options.video2DTo3DMode) ?? "2D-to-3D conversion is unavailable during wireless route playback"
                options.video2DTo3DDiagnostic = .unavailable(reason: reason)
                KSLog("[video] \(reason)")
            }
            if options.isOfflineSubtitleGenerationEnabled {
                KSLog("offline subtitle generation is unavailable during wireless route playback")
            }
            return KSAVPlayer.self
        }
        return Self.preferredPlayerType(for: url, audioURL: audioURL, options: options)
    }

    private func replaceCurrentURLForOptionChange(url: URL, audioURL: URL?, options: KSOptions) {
        let playerType = preferredPlayerType(for: url, audioURL: audioURL, respectsWirelessRoute: true)
        stop()
        if type(of: player) == playerType {
            player.replace(url: url, audioURL: audioURL, options: options)
            if isAutoPlay {
                prepareToPlay()
            }
        } else {
            player = playerType.init(url: url, audioURL: audioURL, options: options)
        }
    }

    private func cancelPrewarmContext() {
        prewarmContext?.cancel()
        prewarmContext = nil
    }

    #if canImport(UIKit) && !os(xrOS)
    @MainActor
    private func switchToWirelessRoutePlayerIfNeeded() {
        guard type(of: player) != KSAVPlayer.self,
              preferredPlayerType(for: url, audioURL: audioURL, respectsWirelessRoute: true) == KSAVPlayer.self
        else {
            return
        }
        let oldPlayer = player
        let targetTime = oldPlayer.currentPlaybackTime
        if targetTime.isFinite, targetTime > 0 {
            shouldSeekTo = targetTime
        }
        let nextPlayer = KSAVPlayer(url: url, audioURL: audioURL, options: options)
        nextPlayer.playbackRate = oldPlayer.playbackRate
        nextPlayer.playbackVolume = oldPlayer.playbackVolume
        nextPlayer.isMuted = oldPlayer.isMuted
        nextPlayer.allowsExternalPlayback = true
        nextPlayer.usesExternalPlaybackWhileExternalScreenIsActive = true
        nextPlayer.contentMode = oldPlayer.contentMode
        player = nextPlayer
        oldPlayer.shutdown()
    }
    #endif

    public func prepareToPlay() {
        state = .preparing
        startTime = CACurrentMediaTime()
        bufferedCount = 0
        player.prepareToPlay()
    }

    private func replaceURLPreservingPlaybackTime(url: URL, audioURL: URL?, options: KSOptions, targetTime: TimeInterval) {
        let shouldAutoPlayAfterSwitch = isAutoPlay
        let canPrewarm = DefinitionSwitchPrewarmPolicy.canPrewarm(
            isEnabled: options.isDefinitionSwitchPrewarmingEnabled,
            duration: player.duration,
            targetTime: targetTime,
            isSeekable: player.seekable,
            isExternalPlaybackActive: player.isExternalPlaybackActive,
            isPictureInPictureActive: isPipActive
        )
        guard canPrewarm else {
            replaceURLWithoutPrewarm(url: url, audioURL: audioURL, options: options, targetTime: targetTime, autoPlayAfterSwitch: shouldAutoPlayAfterSwitch)
            return
        }

        cancelPrewarmContext()
        let playerType = preferredPlayerType(for: url, audioURL: audioURL, respectsWirelessRoute: true)
        let nextPlayer: any MediaPlayerProtocol = playerType.init(url: url, audioURL: audioURL, options: options)
        let prewarmDelegate = DefinitionSwitchPrewarmDelegate()
        let context = DefinitionSwitchPrewarmContext(
            player: nextPlayer,
            delegate: prewarmDelegate,
            url: url,
            audioURL: audioURL,
            options: options,
            requestedTime: targetTime,
            shouldAutoPlayAfterSwitch: shouldAutoPlayAfterSwitch,
            playbackRate: player.playbackRate,
            playbackVolume: player.playbackVolume,
            selectedTrackSelections: currentTrackSelections()
        )
        prewarmContext = context

        nextPlayer.playbackRate = player.playbackRate
        nextPlayer.playbackVolume = player.playbackVolume
        nextPlayer.isMuted = player.isMuted
        nextPlayer.allowsExternalPlayback = player.allowsExternalPlayback
        nextPlayer.usesExternalPlaybackWhileExternalScreenIsActive = player.usesExternalPlaybackWhileExternalScreenIsActive
        nextPlayer.contentMode = player.contentMode
        nextPlayer.delegate = prewarmDelegate

        prewarmDelegate.readyToPlayHandler = { [weak self, weak context] prewarmedPlayer in
            guard let self, let context, self.isCurrentPrewarmPlayer(prewarmedPlayer, context: context) else { return }
            self.seekPrewarmedPlayer(context: context)
        }
        prewarmDelegate.loadStateHandler = { [weak self, weak context] prewarmedPlayer in
            guard let self, let context, self.isCurrentPrewarmPlayer(prewarmedPlayer, context: context) else { return }
            self.commitPrewarmedPlayerIfReady(context: context)
        }
        prewarmDelegate.finishHandler = { [weak self, weak context] prewarmedPlayer, error in
            guard let self, let context, self.isCurrentPrewarmPlayer(prewarmedPlayer, context: context), error != nil else { return }
            self.fallbackToNormalDefinitionSwitch(context: context)
        }

        let timeout = max(options.definitionSwitchPrewarmTimeout, 0.1)
        let timeoutWorkItem = DispatchWorkItem { [weak self, weak context] in
            guard let self, let context, self.prewarmContext === context else { return }
            self.fallbackToNormalDefinitionSwitch(context: context)
        }
        context.timeoutWorkItem = timeoutWorkItem
        DispatchQueue.main.asyncAfter(deadline: .now() + timeout, execute: timeoutWorkItem)

        nextPlayer.prepareToPlay()
    }

    private func isCurrentPrewarmPlayer(_ player: any MediaPlayerProtocol, context: DefinitionSwitchPrewarmContext) -> Bool {
        prewarmContext === context && ObjectIdentifier(player) == ObjectIdentifier(context.player)
    }

    private func seekPrewarmedPlayer(context: DefinitionSwitchPrewarmContext) {
        let seekTime = DefinitionSwitchPrewarmPolicy.handoffTime(
            requestedTime: context.requestedTime,
            elapsed: CACurrentMediaTime() - context.startedAt,
            playbackRate: context.playbackRate,
            wasPlaying: context.shouldAutoPlayAfterSwitch,
            duration: context.player.duration
        )
        context.player.seek(time: seekTime) { [weak self, weak context] finished in
            guard let self, let context, self.prewarmContext === context else { return }
            guard finished else {
                self.fallbackToNormalDefinitionSwitch(context: context)
                return
            }
            context.didSeek = true
            self.commitPrewarmedPlayerIfReady(context: context)
        }
    }

    private func commitPrewarmedPlayerIfReady(context: DefinitionSwitchPrewarmContext) {
        guard prewarmContext === context, context.didSeek, context.player.loadState == .playable else {
            return
        }
        context.timeoutWorkItem?.cancel()
        prewarmContext = nil

        let oldPlayer = player
        isCommittingPrewarmedPlayer = true
        self.options = context.options
        self.audioURL = context.audioURL
        self.url = context.url
        applyTrackSelections(context.selectedTrackSelections, to: context.player)
        self.player = context.player
        isCommittingPrewarmedPlayer = false
        oldPlayer.shutdown()

        player.playbackRate = context.playbackRate
        player.playbackVolume = context.playbackVolume
        if context.shouldAutoPlayAfterSwitch {
            isAutoPlay = true
            player.play()
            timer.fireDate = Date.distantPast
            state = player.loadState == .playable ? .bufferFinished : .buffering
            MPNowPlayingInfoCenter.default().playbackState = .playing
            if #available(tvOS 14.0, *) {
                KSPictureInPictureController.mute()
            }
        } else {
            isAutoPlay = false
            player.pause()
            timer.fireDate = Date.distantFuture
            state = .paused
            MPNowPlayingInfoCenter.default().playbackState = .paused
        }
        updateNowPlayingInfo()
    }

    private func fallbackToNormalDefinitionSwitch(context: DefinitionSwitchPrewarmContext) {
        let fallbackTime = player.currentPlaybackTime.isFinite ? player.currentPlaybackTime : context.requestedTime
        replaceURLWithoutPrewarm(url: context.url, audioURL: context.audioURL, options: context.options, targetTime: fallbackTime, autoPlayAfterSwitch: context.shouldAutoPlayAfterSwitch)
    }

    private func replaceURLWithoutPrewarm(url: URL, audioURL: URL?, options: KSOptions, targetTime: TimeInterval, autoPlayAfterSwitch: Bool? = nil) {
        let shouldAutoPlayAfterSwitch = autoPlayAfterSwitch ?? isAutoPlay
        let playbackRate = player.playbackRate
        let playbackVolume = player.playbackVolume
        let isMuted = player.isMuted
        let allowsExternalPlayback = player.allowsExternalPlayback
        let usesExternalPlaybackWhileExternalScreenIsActive = player.usesExternalPlaybackWhileExternalScreenIsActive
        let contentMode = player.contentMode
        cancelPrewarmContext()
        self.options = options
        self.audioURL = audioURL
        isAutoPlay = shouldAutoPlayAfterSwitch
        if self.url == url {
            replaceCurrentURLForOptionChange(url: url, audioURL: audioURL, options: options)
        } else {
            self.url = url
        }
        player.playbackRate = playbackRate
        player.playbackVolume = playbackVolume
        player.isMuted = isMuted
        player.allowsExternalPlayback = allowsExternalPlayback
        player.usesExternalPlaybackWhileExternalScreenIsActive = usesExternalPlaybackWhileExternalScreenIsActive
        player.contentMode = contentMode
        if !shouldAutoPlayAfterSwitch, !player.isReadyToPlay {
            prepareToPlay()
        }
        if targetTime > 0 {
            seek(time: targetTime, autoPlay: shouldAutoPlayAfterSwitch) { _ in }
        }
    }

    private func currentTrackSelections() -> [DefinitionSwitchTrackSelection] {
        [.audio, .video, .subtitle].compactMap { mediaType in
            DefinitionSwitchTrackSelection(track: player.tracks(mediaType: mediaType).first { $0.isEnabled })
        }
    }

    private func applyTrackSelections(_ selections: [DefinitionSwitchTrackSelection], to player: any MediaPlayerProtocol) {
        for selection in selections {
            guard let track = selection.matchingTrack(in: player) else {
                continue
            }
            player.select(track: track)
        }
    }

    private func updateNowPlayingInfo() {
        if MPNowPlayingInfoCenter.default().nowPlayingInfo == nil {
            MPNowPlayingInfoCenter.default().nowPlayingInfo = [MPMediaItemPropertyPlaybackDuration: player.duration]
        } else {
            MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPMediaItemPropertyPlaybackDuration] = player.duration
        }
        if MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPMediaItemPropertyTitle] == nil, let title = player.dynamicInfo?.metadata["title"] {
            MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPMediaItemPropertyTitle] = title
        }
        if MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPMediaItemPropertyArtist] == nil, let artist = player.dynamicInfo?.metadata["artist"] {
            MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPMediaItemPropertyArtist] = artist
        }
        var current: [MPNowPlayingInfoLanguageOption] = []
        var langs: [MPNowPlayingInfoLanguageOptionGroup] = []
        for track in player.tracks(mediaType: .audio) {
            if let lang = track.language {
                let audioLang = MPNowPlayingInfoLanguageOption(type: .audible, languageTag: lang, characteristics: nil, displayName: track.name, identifier: track.name)
                let audioGroup = MPNowPlayingInfoLanguageOptionGroup(languageOptions: [audioLang], defaultLanguageOption: nil, allowEmptySelection: false)
                langs.append(audioGroup)
                if track.isEnabled {
                    current.append(audioLang)
                }
            }
        }
        if !langs.isEmpty {
            MPRemoteCommandCenter.shared().enableLanguageOptionCommand.isEnabled = true
        }
        MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPNowPlayingInfoPropertyAvailableLanguageOptions] = langs
        MPNowPlayingInfoCenter.default().nowPlayingInfo?[MPNowPlayingInfoPropertyCurrentLanguageOptions] = current
    }

    private func nextPlayer() {
        if urls.count > 1, let index = urls.firstIndex(of: url), index < urls.count - 1 {
            isAutoPlay = true
            url = urls[index + 1]
        }
    }

    private func previousPlayer() {
        if urls.count > 1, let index = urls.firstIndex(of: url), index > 0 {
            isAutoPlay = true
            url = urls[index - 1]
        }
    }

    func seek(time: TimeInterval) {
        seek(time: time, autoPlay: options.isSeekedAutoPlay) { _ in
        }
    }

    public func registerRemoteControllEvent() {
        let remoteCommand = MPRemoteCommandCenter.shared()
        remoteCommand.playCommand.addTarget { [weak self] _ in
            guard let self else {
                return .commandFailed
            }
            self.play()
            return .success
        }
        remoteCommand.pauseCommand.addTarget { [weak self] _ in
            guard let self else {
                return .commandFailed
            }
            self.pause()
            return .success
        }
        remoteCommand.togglePlayPauseCommand.addTarget { [weak self] _ in
            guard let self else {
                return .commandFailed
            }
            if self.state.isPlaying {
                self.pause()
            } else {
                self.play()
            }
            return .success
        }
        remoteCommand.stopCommand.addTarget { [weak self] _ in
            guard let self else {
                return .commandFailed
            }
            self.player.shutdown()
            return .success
        }
        remoteCommand.nextTrackCommand.addTarget { [weak self] _ in
            guard let self else {
                return .commandFailed
            }
            self.nextPlayer()
            return .success
        }
        remoteCommand.previousTrackCommand.addTarget { [weak self] _ in
            guard let self else {
                return .commandFailed
            }
            self.previousPlayer()
            return .success
        }
        remoteCommand.changeRepeatModeCommand.addTarget { [weak self] event in
            guard let self, let event = event as? MPChangeRepeatModeCommandEvent else {
                return .commandFailed
            }
            self.options.isLoopPlay = event.repeatType != .off
            return .success
        }
        remoteCommand.changeShuffleModeCommand.isEnabled = false
        // remoteCommand.changeShuffleModeCommand.addTarget {})
        remoteCommand.changePlaybackRateCommand.supportedPlaybackRates = [0.5, 1, 1.5, 2]
        remoteCommand.changePlaybackRateCommand.addTarget { [weak self] event in
            guard let self, let event = event as? MPChangePlaybackRateCommandEvent else {
                return .commandFailed
            }
            self.player.playbackRate = event.playbackRate
            return .success
        }
        remoteCommand.skipForwardCommand.preferredIntervals = [15]
        remoteCommand.skipForwardCommand.addTarget { [weak self] event in
            guard let self, let event = event as? MPSkipIntervalCommandEvent else {
                return .commandFailed
            }
            self.seek(time: self.player.currentPlaybackTime + event.interval)
            return .success
        }
        remoteCommand.skipBackwardCommand.preferredIntervals = [15]
        remoteCommand.skipBackwardCommand.addTarget { [weak self] event in
            guard let self, let event = event as? MPSkipIntervalCommandEvent else {
                return .commandFailed
            }
            self.seek(time: self.player.currentPlaybackTime - event.interval)
            return .success
        }
        remoteCommand.changePlaybackPositionCommand.addTarget { [weak self] event in
            guard let self, let event = event as? MPChangePlaybackPositionCommandEvent else {
                return .commandFailed
            }
            self.seek(time: event.positionTime)
            return .success
        }
        remoteCommand.enableLanguageOptionCommand.addTarget { [weak self] event in
            guard let self, let event = event as? MPChangeLanguageOptionCommandEvent else {
                return .commandFailed
            }
            let selectLang = event.languageOption
            if selectLang.languageOptionType == .audible,
               let trackToSelect = self.player.tracks(mediaType: .audio).first(where: { $0.name == selectLang.displayName })
            {
                self.player.select(track: trackToSelect)
            }
            return .success
        }
    }

    @objc private func enterBackground() {
        guard state.isPlaying, !player.isExternalPlaybackActive else {
            return
        }
        if #available(tvOS 14.0, *), player.pipController?.isPictureInPictureActive == true {
            return
        }

        if KSOptions.canBackgroundPlay {
            player.enterBackground()
            return
        }
        pause()
    }

    @objc private func enterForeground() {
        if KSOptions.canBackgroundPlay {
            player.enterForeground()
        }
    }

    #if canImport(UIKit) && !os(xrOS)
    @MainActor
    @objc private func wirelessRouteActiveDidChange(notification: Notification) {
        guard let volumeView = notification.object as? MPVolumeView, isWirelessRouteActive != volumeView.isWirelessRouteActive else { return }
        isWirelessRouteActive = volumeView.isWirelessRouteActive
        if volumeView.isWirelessRouteActive {
            player.usesExternalPlaybackWhileExternalScreenIsActive = true
            switchToWirelessRoutePlayerIfNeeded()
        }
    }
    #endif
    #if !os(macOS)
    @objc private func audioInterrupted(notification: Notification) {
        guard let userInfo = notification.userInfo,
              let typeValue = userInfo[AVAudioSessionInterruptionTypeKey] as? UInt,
              let type = AVAudioSession.InterruptionType(rawValue: typeValue)
        else {
            return
        }
        switch type {
        case .began:
            pause()

        case .ended:
            // An interruption ended. Resume playback, if appropriate.

            guard let optionsValue = userInfo[AVAudioSessionInterruptionOptionKey] as? UInt else { return }
            let options = AVAudioSession.InterruptionOptions(rawValue: optionsValue)
            if options.contains(.shouldResume) {
                play()
            }

        default:
            break
        }
    }
    #endif
}

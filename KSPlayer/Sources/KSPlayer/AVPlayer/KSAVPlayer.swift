import AVFoundation
import AVKit
#if canImport(UIKit)
import UIKit
#else
import AppKit

public typealias UIImage = NSImage
#endif
import Combine
import CoreGraphics

enum AVPlayerSeekableRangeResolver {
    static func range(from ranges: [CMTimeRange]) -> MediaPlaybackTimeRange? {
        let secondsRanges = ranges.compactMap { range -> MediaPlaybackTimeRange? in
            MediaPlaybackTimeRange(start: range.start.seconds, duration: range.duration.seconds)
        }
        guard let first = secondsRanges.first else {
            return nil
        }
        let start = secondsRanges.reduce(first.start) { min($0, $1.start) }
        let end = secondsRanges.reduce(first.end) { max($0, $1.end) }
        return MediaPlaybackTimeRange(start: start, end: end)
    }
}

public final class KSAVPlayerView: UIView {
    public let player = AVQueuePlayer()
    override public init(frame: CGRect) {
        super.init(frame: frame)
        #if !canImport(UIKit)
        layer = AVPlayerLayer()
        #endif
        playerLayer.player = player
        player.automaticallyWaitsToMinimizeStalling = false
    }

    @available(*, unavailable)
    public required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override public var contentMode: UIViewContentMode {
        get {
            switch playerLayer.videoGravity {
            case .resize:
                return .scaleToFill
            case .resizeAspect:
                return .scaleAspectFit
            case .resizeAspectFill:
                return .scaleAspectFill
            default:
                return .scaleAspectFit
            }
        }
        set {
            switch newValue {
            case .scaleToFill:
                playerLayer.videoGravity = .resize
            case .scaleAspectFit:
                playerLayer.videoGravity = .resizeAspect
            case .scaleAspectFill:
                playerLayer.videoGravity = .resizeAspectFill
            case .center:
                playerLayer.videoGravity = .resizeAspect
            default:
                break
            }
        }
    }

    #if canImport(UIKit)
    override public class var layerClass: AnyClass { AVPlayerLayer.self }
    #endif
    fileprivate var playerLayer: AVPlayerLayer {
        // swiftlint:disable force_cast
        layer as! AVPlayerLayer
        // swiftlint:enable force_cast
    }
}

@MainActor
public class KSAVPlayer {
    private var cancellable: AnyCancellable?
    private var options: KSOptions {
        didSet {
            player.currentItem?.preferredForwardBufferDuration = options.preferredForwardBufferDuration
            cancellable = options.$preferredForwardBufferDuration.sink { [weak self] newValue in
                self?.player.currentItem?.preferredForwardBufferDuration = newValue
            }
        }
    }

    private let playerView = KSAVPlayerView()
    private var urlAsset: AVURLAsset
    private var audioURLAsset: AVURLAsset?
    private var fileAccess: KSSecurityScopedURLAccess
    private var prepareTask: Task<Void, Never>?
    private var shouldSeekTo = TimeInterval(0)
    private var playerLooper: AVPlayerLooper?
    private var statusObservation: NSKeyValueObservation?
    private var loadedTimeRangesObservation: NSKeyValueObservation?
    private var bufferEmptyObservation: NSKeyValueObservation?
    private var likelyToKeepUpObservation: NSKeyValueObservation?
    private var bufferFullObservation: NSKeyValueObservation?
    private var itemObservation: NSKeyValueObservation?
    private var loopCountObservation: NSKeyValueObservation?
    private var loopStatusObservation: NSKeyValueObservation?
    private var manualLoopCount = 0
    private var mediaPlayerTracks = [AVMediaPlayerTrack]()
    private var error: Error? {
        didSet {
            if let error {
                delegate?.finish(player: self, error: error)
            }
        }
    }

    private lazy var _pipController: Any? = {
        if #available(tvOS 14.0, *) {
            let pip = KSPictureInPictureController(playerLayer: playerView.playerLayer)
            return pip
        } else {
            return nil
        }
    }()

    @available(tvOS 14.0, *)
    public var pipController: KSPictureInPictureController? {
        _pipController as? KSPictureInPictureController
    }

    public var naturalSize: CGSize = .zero
    public let dynamicInfo: DynamicInfo? = nil
    @available(macOS 12.0, iOS 15.0, tvOS 15.0, *)
    public var playbackCoordinator: AVPlaybackCoordinator {
        playerView.player.playbackCoordinator
    }

    public private(set) var bufferingProgress = 0 {
        didSet {
            delegate?.changeBuffering(player: self, progress: bufferingProgress)
        }
    }

    public weak var delegate: MediaPlayerDelegate?
    public private(set) var duration: TimeInterval = 0
    public private(set) var fileSize: Double = 0
    public private(set) var playableTime: TimeInterval = 0
    public let chapters: [Chapter] = []
    public var playbackRate: Float = 1 {
        didSet {
            if playbackState == .playing {
                player.rate = playbackRate
            }
        }
    }

    public var playbackVolume: Float = 1.0 {
        didSet {
            if player.volume != playbackVolume {
                player.volume = playbackVolume
            }
        }
    }

    public private(set) var loadState = MediaLoadState.idle {
        didSet {
            if loadState != oldValue {
                playOrPause()
                if loadState == .loading || loadState == .idle {
                    bufferingProgress = 0
                }
            }
        }
    }

    public private(set) var playbackState = MediaPlaybackState.idle {
        didSet {
            if playbackState != oldValue {
                playOrPause()
                if playbackState == .finished {
                    delegate?.finish(player: self, error: nil)
                }
            }
        }
    }

    public private(set) var isReadyToPlay = false {
        didSet {
            if isReadyToPlay != oldValue {
                if isReadyToPlay {
                    options.readyTime = CACurrentMediaTime()
                    delegate?.readyToPlay(player: self)
                }
            }
        }
    }

    #if os(xrOS)
    public var allowsExternalPlayback = false
    public var usesExternalPlaybackWhileExternalScreenIsActive = false
    public let isExternalPlaybackActive = false
    #else
    public var allowsExternalPlayback: Bool {
        get {
            player.allowsExternalPlayback
        }
        set {
            player.allowsExternalPlayback = newValue
        }
    }

    #if os(macOS)
    public var usesExternalPlaybackWhileExternalScreenIsActive = false
    #else
    public var usesExternalPlaybackWhileExternalScreenIsActive: Bool {
        get {
            player.usesExternalPlaybackWhileExternalScreenIsActive
        }
        set {
            player.usesExternalPlaybackWhileExternalScreenIsActive = newValue
        }
    }
    #endif

    public var isExternalPlaybackActive: Bool {
        player.isExternalPlaybackActive
    }
    #endif

    public required convenience init(url: URL, options: KSOptions) {
        self.init(url: url, audioURL: nil, options: options)
    }

    public init(url: URL, audioURL: URL?, options: KSOptions) {
        KSOptions.setAudioSession(options: options)
        let playbackURL = KSDiskPrecache.playbackURL(for: url, options: options)
        let playbackAudioURL = audioURL.map { KSDiskPrecache.playbackURL(for: $0, options: options) }
        fileAccess = KSSecurityScopedURLAccess(urls: [playbackURL] + (playbackAudioURL.map { [$0] } ?? []))
        urlAsset = AVURLAsset(url: playbackURL, options: options.avOptions)
        audioURLAsset = playbackAudioURL.map { AVURLAsset(url: $0, options: options.avOptions) }
        self.options = options
        itemObservation = player.observe(\.currentItem) { [weak self] player, _ in
            guard let self else { return }
            self.observer(playerItem: player.currentItem)
        }
    }
}

extension KSAVPlayer {
    public var player: AVQueuePlayer { playerView.player }
    public var playerLayer: AVPlayerLayer { playerView.playerLayer }
    @objc private func moviePlayDidEnd(notification _: Notification) {
        if options.isLoopPlay {
            loopFromBeginning()
        } else {
            playbackState = .finished
        }
    }

    @objc private func playerItemFailedToPlayToEndTime(notification: Notification) {
        var playError: Error?
        if let userInfo = notification.userInfo {
            if let error = userInfo["error"] as? Error {
                playError = error
            } else if let error = userInfo[AVPlayerItemFailedToPlayToEndTimeErrorKey] as? NSError {
                playError = error
            } else if let errorCode = (userInfo["error"] as? NSNumber)?.intValue {
                playError = NSError(domain: "AVMoviePlayer", code: errorCode, userInfo: nil)
            }
        }
        delegate?.finish(player: self, error: playError)
    }

    private func updateStatus(item: AVPlayerItem) {
        if item.status == .readyToPlay {
            options.findTime = CACurrentMediaTime()
            mediaPlayerTracks = item.tracks.map {
                AVMediaPlayerTrack(track: $0)
            }
            let playableVideo = mediaPlayerTracks.first {
                $0.mediaType == .video && $0.isPlayable
            }
            if let playableVideo {
                naturalSize = playableVideo.naturalSize
            } else {
                error = NSError(errorCode: .videoTracksUnplayable)
                return
            }
            // 默认选择第一个声道
            item.tracks.filter { $0.assetTrack?.mediaType.rawValue == AVMediaType.audio.rawValue }.dropFirst().forEach { $0.isEnabled = false }
            let itemDuration = item.duration.seconds
            duration = itemDuration.isFinite ? max(itemDuration, 0) : 0
            let estimatedDataRates = item.tracks.compactMap { $0.assetTrack?.estimatedDataRate }
            fileSize = Double(estimatedDataRates.reduce(0, +)) * duration / 8
            isReadyToPlay = true
        } else if item.status == .failed {
            error = item.error
        }
    }

    private func updatePlayableDuration(item: AVPlayerItem) {
        let first = item.loadedTimeRanges.first { CMTimeRangeContainsTime($0.timeRangeValue, time: item.currentTime()) }
        if let first {
            playableTime = first.timeRangeValue.end.seconds
            guard playableTime > 0 else { return }
            let loadedTime = playableTime - currentPlaybackTime
            guard loadedTime > 0 else { return }
            bufferingProgress = Int(min(loadedTime * 100 / item.preferredForwardBufferDuration, 100))
            if bufferingProgress >= 100 {
                loadState = .playable
            }
        }
    }

    private func playOrPause() {
        if playbackState == .playing {
            if loadState == .playable {
                player.play()
                player.rate = playbackRate
            }
        } else {
            player.pause()
        }
        delegate?.changeLoadState(player: self)
    }

    private func replaceCurrentItem(playerItem: AVPlayerItem?) {
        player.currentItem?.cancelPendingSeeks()
        if options.isLoopPlay, options.isSeamlessLoopEnabled {
            loopCountObservation?.invalidate()
            loopStatusObservation?.invalidate()
            playerLooper?.disableLooping()
            player.removeAllItems()
            guard let playerItem else {
                playerLooper = nil
                return
            }
            playerLooper = AVPlayerLooper(player: player, templateItem: playerItem)
            loopCountObservation = playerLooper?.observe(\.loopCount) { [weak self] playerLooper, _ in
                guard let self else { return }
                self.delegate?.playBack(player: self, loopCount: playerLooper.loopCount)
            }
            loopStatusObservation = playerLooper?.observe(\.status) { [weak self] playerLooper, _ in
                guard let self else { return }
                if playerLooper.status == .failed {
                    self.error = playerLooper.error
                }
            }
        } else {
            loopCountObservation?.invalidate()
            loopStatusObservation?.invalidate()
            playerLooper?.disableLooping()
            playerLooper = nil
            player.replaceCurrentItem(with: playerItem)
        }
    }

    private func loopFromBeginning() {
        player.seek(to: .zero, toleranceBefore: .zero, toleranceAfter: .zero) { [weak self] finished in
            guard finished else { return }
            Task { @MainActor [weak self] in
                guard let self else { return }
                self.manualLoopCount += 1
                self.delegate?.playBack(player: self, loopCount: self.manualLoopCount)
                if self.playbackState == .playing {
                    self.player.play()
                    self.player.rate = self.playbackRate
                }
            }
        }
    }

    private func observer(playerItem: AVPlayerItem?) {
        NotificationCenter.default.removeObserver(self, name: .AVPlayerItemDidPlayToEndTime, object: playerItem)
        NotificationCenter.default.removeObserver(self, name: .AVPlayerItemFailedToPlayToEndTime, object: playerItem)
        statusObservation?.invalidate()
        loadedTimeRangesObservation?.invalidate()
        bufferEmptyObservation?.invalidate()
        likelyToKeepUpObservation?.invalidate()
        bufferFullObservation?.invalidate()
        guard let playerItem else { return }
        NotificationCenter.default.addObserver(self, selector: #selector(moviePlayDidEnd), name: .AVPlayerItemDidPlayToEndTime, object: playerItem)
        NotificationCenter.default.addObserver(self, selector: #selector(playerItemFailedToPlayToEndTime), name: .AVPlayerItemFailedToPlayToEndTime, object: playerItem)
        statusObservation = playerItem.observe(\.status) { [weak self] item, _ in
            guard let self else { return }
            self.updateStatus(item: item)
        }
        loadedTimeRangesObservation = playerItem.observe(\.loadedTimeRanges) { [weak self] item, _ in
            guard let self else { return }
            // 计算缓冲进度
            self.updatePlayableDuration(item: item)
        }

        let changeHandler: (AVPlayerItem, NSKeyValueObservedChange<Bool>) -> Void = { [weak self] _, _ in
            guard let self else { return }
            // 在主线程更新进度
            if playerItem.isPlaybackBufferEmpty {
                self.loadState = .loading
            } else if playerItem.isPlaybackLikelyToKeepUp || playerItem.isPlaybackBufferFull {
                self.loadState = .playable
            }
        }
        bufferEmptyObservation = playerItem.observe(\.isPlaybackBufferEmpty, changeHandler: changeHandler)
        likelyToKeepUpObservation = playerItem.observe(\.isPlaybackLikelyToKeepUp, changeHandler: changeHandler)
        bufferFullObservation = playerItem.observe(\.isPlaybackBufferFull, changeHandler: changeHandler)
    }

    private func playerItem() async throws -> AVPlayerItem {
        guard let audioURLAsset else {
            return AVPlayerItem(asset: urlAsset)
        }
        let composition = AVMutableComposition()
        async let loadedVideoTracks = urlAsset.loadTracks(withMediaType: .video)
        async let loadedAudioTracks = audioURLAsset.loadTracks(withMediaType: .audio)
        async let loadedVideoDuration = urlAsset.load(.duration)
        async let loadedAudioDuration = audioURLAsset.load(.duration)
        let (videoTracks, audioTracks, videoDuration, audioDuration) = try await (loadedVideoTracks, loadedAudioTracks, loadedVideoDuration, loadedAudioDuration)
        guard let sourceVideoTrack = videoTracks.first else {
            throw NSError(errorCode: .videoTracksUnplayable)
        }
        guard let sourceAudioTrack = audioTracks.first else {
            throw NSError(description: "No playable audio track was found in the separate audio URL.")
        }
        guard videoDuration.isNumeric, videoDuration.seconds > 0 else {
            throw NSError(description: "Separate audio/video URL playback requires a finite video duration when using AVFoundation composition.")
        }
        if let videoTrack = composition.addMutableTrack(withMediaType: .video, preferredTrackID: kCMPersistentTrackID_Invalid) {
            try videoTrack.insertTimeRange(CMTimeRange(start: .zero, duration: videoDuration), of: sourceVideoTrack, at: .zero)
            videoTrack.preferredTransform = try await sourceVideoTrack.load(.preferredTransform)
        }
        if let audioTrack = composition.addMutableTrack(withMediaType: .audio, preferredTrackID: kCMPersistentTrackID_Invalid) {
            let audioTimeRangeDuration = audioDuration.isNumeric && audioDuration < videoDuration ? audioDuration : videoDuration
            try audioTrack.insertTimeRange(CMTimeRange(start: .zero, duration: audioTimeRangeDuration), of: sourceAudioTrack, at: .zero)
        }
        return AVPlayerItem(asset: composition)
    }
}

extension KSAVPlayer: @preconcurrency MediaPlayerProtocol {
    public var subtitleDataSouce: SubtitleDataSouce? {
        options.pictureInPictureSubtitlePolicy == .disabled ? nil : self
    }
    public var isPlaying: Bool { player.rate > 0 ? true : playbackState == .playing }
    public var view: UIView? { playerView }
    public var currentPlaybackTime: TimeInterval {
        get {
            if shouldSeekTo > 0 {
                return TimeInterval(shouldSeekTo)
            } else {
                // 防止卡主
                return isReadyToPlay ? player.currentTime().seconds : 0
            }
        }
        set {
            seek(time: newValue) { _ in
            }
        }
    }

    public var numberOfBytesTransferred: Int64 {
        guard let playerItem = player.currentItem, let accesslog = playerItem.accessLog(), let event = accesslog.events.first else {
            return 0
        }
        return event.numberOfBytesTransferred
    }

    public func thumbnailImageAtCurrentTime() async -> CGImage? {
        guard let playerItem = player.currentItem, isReadyToPlay else {
            return nil
        }
        return await withCheckedContinuation { continuation in
            urlAsset.thumbnailImage(currentTime: playerItem.currentTime()) { result in
                continuation.resume(returning: result)
            }
        }
    }

    public func seek(time: TimeInterval, completion: @escaping ((Bool) -> Void)) {
        let time = max(time, 0)
        shouldSeekTo = time
        playbackState = .seeking
        runOnMainThread { [weak self] in
            self?.bufferingProgress = 0
        }
        let tolerance = SeamlessLoopPlaybackPolicy.avPlayerSeekTolerance(isAccurateSeek: options.isAccurateSeek, isLoopRestart: options.isLoopPlay && time == 0)
        player.seek(to: CMTime(seconds: time), toleranceBefore: tolerance, toleranceAfter: tolerance) {
            [weak self] finished in
            guard let self else { return }
            self.shouldSeekTo = 0
            completion(finished)
        }
    }

    public func prepareToPlay() {
        KSLog("prepareToPlay \(self)")
        options.prepareTime = CACurrentMediaTime()
        fileAccess = KSSecurityScopedURLAccess(urls: [urlAsset.url] + (audioURLAsset.map(\.url).map { [$0] } ?? []))
        prepareTask?.cancel()
        prepareTask = Task { [weak self] in
            guard let self else { return }
            do {
                let playerItem = try await self.playerItem()
                guard !Task.isCancelled else { return }
                self.bufferingProgress = 0
                self.options.openTime = CACurrentMediaTime()
                self.replaceCurrentItem(playerItem: playerItem)
                self.player.actionAtItemEnd = SeamlessLoopPlaybackPolicy.avPlayerActionAtItemEnd(isLoopPlay: self.options.isLoopPlay, isSeamlessLoopEnabled: self.options.isSeamlessLoopEnabled)
                self.player.volume = self.playbackVolume
            } catch {
                guard !Task.isCancelled else { return }
                self.error = error
            }
        }
    }

    public func play() {
        KSLog("play \(self)")
        playbackState = .playing
    }

    public func pause() {
        KSLog("pause \(self)")
        playbackState = .paused
    }

    public func shutdown() {
        KSLog("shutdown \(self)")
        prepareTask?.cancel()
        prepareTask = nil
        isReadyToPlay = false
        playbackState = .stopped
        loadState = .idle
        manualLoopCount = 0
        urlAsset.cancelLoading()
        audioURLAsset?.cancelLoading()
        replaceCurrentItem(playerItem: nil)
        fileAccess.stop()
    }

    public func replace(url: URL, options: KSOptions) {
        replace(url: url, audioURL: nil, options: options)
    }

    public func replace(url: URL, audioURL: URL?, options: KSOptions) {
        KSLog("replaceUrl \(self)")
        KSOptions.setAudioSession(options: options)
        shutdown()
        let playbackURL = KSDiskPrecache.playbackURL(for: url, options: options)
        let playbackAudioURL = audioURL.map { KSDiskPrecache.playbackURL(for: $0, options: options) }
        fileAccess = KSSecurityScopedURLAccess(urls: [playbackURL] + (playbackAudioURL.map { [$0] } ?? []))
        urlAsset = AVURLAsset(url: playbackURL, options: options.avOptions)
        audioURLAsset = playbackAudioURL.map { AVURLAsset(url: $0, options: options.avOptions) }
        self.options = options
    }

    public var contentMode: UIViewContentMode {
        get {
            playerView.contentMode
        }
        set {
            playerView.contentMode = newValue
        }
    }

    public func enterBackground() {
        playerView.playerLayer.player = nil
    }

    public func enterForeground() {
        playerView.playerLayer.player = playerView.player
    }

    public var seekable: Bool {
        seekableTimeRange != nil
    }

    public var seekableTimeRange: MediaPlaybackTimeRange? {
        guard let ranges = player.currentItem?.seekableTimeRanges, !ranges.isEmpty else {
            return nil
        }
        return AVPlayerSeekableRangeResolver.range(from: ranges.map(\.timeRangeValue))
    }

    public var isMuted: Bool {
        get {
            player.isMuted
        }
        set {
            player.isMuted = newValue
        }
    }

    public func tracks(mediaType: AVFoundation.AVMediaType) -> [MediaPlayerTrack] {
        guard let item = player.currentItem else {
            return []
        }
        let itemTracks = item.tracks.filter { $0.assetTrack?.mediaType == mediaType }.map { AVMediaPlayerTrack(track: $0) }
        guard mediaType == .subtitle else {
            return itemTracks
        }
        guard options.pictureInPictureSubtitlePolicy != .disabled else {
            return []
        }
        return legibleMediaSelectionTracks(for: item) + itemTracks
    }

    public func select(track: some MediaPlayerTrack) {
        player.currentItem?.tracks.filter { $0.assetTrack?.mediaType == track.mediaType }.forEach { $0.isEnabled = false }
        track.isEnabled = true
    }

    private func legibleMediaSelectionTracks(for item: AVPlayerItem) -> [AVMediaPlayerTrack] {
        #if os(xrOS)
        return []
        #else
        guard let group = item.asset.mediaSelectionGroup(forMediaCharacteristic: .legible) else {
            return []
        }
        return group.options.map { option in
            AVMediaPlayerTrack(mediaSelectionOption: option, group: group, playerItem: item)
        }
        #endif
    }
}

extension KSAVPlayer: @preconcurrency SubtitleDataSouce {
    public var infos: [any SubtitleInfo] {
        tracks(mediaType: .subtitle).compactMap { $0 as? (any SubtitleInfo) }
    }
}

extension AVFoundation.AVMediaType {
    var mediaCharacteristic: AVMediaCharacteristic {
        switch self {
        case .video:
            return .visual
        case .audio:
            return .audible
        case .subtitle:
            return .legible
        default:
            return .easyToRead
        }
    }
}

extension AVAssetTrack {
    func toMediaPlayerTrack() {}
}

enum AVLegibleSubtitlePolicy {
    static let unknownLanguageCode = "und"

    static func subtitleID(languageCode: String?, displayName: String) -> String {
        "av-legible:\(languageCode ?? unknownLanguageCode):\(displayName)"
    }
}

class AVMediaPlayerTrack: @preconcurrency MediaPlayerTrack, SubtitleKindProviding {
    let formatDescription: CMFormatDescription?
    let description: String
    private let track: AVPlayerItemTrack?
    private weak var playerItem: AVPlayerItem?
    private let mediaSelectionGroup: AVMediaSelectionGroup?
    private let mediaSelectionOption: AVMediaSelectionOption?
    var nominalFrameRate: Float
    let trackID: Int32
    let rotation: Int16 = 0
    let bitDepth: Int32
    let bitRate: Int64
    let name: String
    let languageCode: String?
    let mediaType: AVFoundation.AVMediaType
    let isImageSubtitle = false
    let subtitleKind: SubtitleKind
    var dovi: DOVIDecoderConfigurationRecord?
    let fieldOrder: FFmpegFieldOrder = .unknown
    var isPlayable: Bool
    @MainActor
    var isEnabled: Bool {
        get {
            if let playerItem, let mediaSelectionGroup, let mediaSelectionOption {
                return playerItem.currentMediaSelection.selectedMediaOption(in: mediaSelectionGroup) == mediaSelectionOption
            }
            return track?.isEnabled ?? false
        }
        set {
            if let playerItem, let mediaSelectionGroup, let mediaSelectionOption {
                if newValue {
                    playerItem.select(mediaSelectionOption, in: mediaSelectionGroup)
                } else if playerItem.currentMediaSelection.selectedMediaOption(in: mediaSelectionGroup) == mediaSelectionOption {
                    playerItem.select(nil, in: mediaSelectionGroup)
                }
            } else {
                track?.isEnabled = newValue
            }
        }
    }

    init(track: AVPlayerItemTrack) {
        self.track = track
        playerItem = nil
        mediaSelectionGroup = nil
        mediaSelectionOption = nil
        trackID = track.assetTrack?.trackID ?? 0
        let assetMediaType = track.assetTrack?.mediaType ?? .video
        mediaType = assetMediaType == .closedCaption ? .subtitle : assetMediaType
        name = track.assetTrack?.languageCode ?? ""
        languageCode = track.assetTrack?.languageCode
        nominalFrameRate = track.assetTrack?.nominalFrameRate ?? 24.0
        bitRate = Int64(track.assetTrack?.estimatedDataRate ?? 0)
        #if os(xrOS)
        isPlayable = false
        #else
        isPlayable = track.assetTrack?.isPlayable ?? false
        #endif
        // swiftlint:disable force_cast
        if let first = track.assetTrack?.formatDescriptions.first {
            formatDescription = (first as! CMFormatDescription)
        } else {
            formatDescription = nil
        }
        bitDepth = formatDescription?.bitDepth ?? 0
        // swiftlint:enable force_cast
        subtitleKind = Self.subtitleKind(mediaType: assetMediaType, formatDescription: formatDescription)
        description = (formatDescription?.mediaSubType ?? .boxed).rawValue.string
    }

    init(mediaSelectionOption: AVMediaSelectionOption, group: AVMediaSelectionGroup, playerItem: AVPlayerItem) {
        track = nil
        self.playerItem = playerItem
        mediaSelectionGroup = group
        self.mediaSelectionOption = mediaSelectionOption
        trackID = Int32(truncatingIfNeeded: mediaSelectionOption.hash)
        mediaType = .subtitle
        name = mediaSelectionOption.displayName
        languageCode = mediaSelectionOption.extendedLanguageTag ?? mediaSelectionOption.locale?.identifier
        nominalFrameRate = 0
        bitRate = 0
        isPlayable = mediaSelectionOption.isPlayable
        formatDescription = nil
        bitDepth = 0
        subtitleKind = Self.subtitleKind(mediaSelectionOption: mediaSelectionOption)
        description = mediaSelectionOption.displayName
    }

    func load() {}

    private static func subtitleKind(mediaType: AVFoundation.AVMediaType, formatDescription: CMFormatDescription?) -> SubtitleKind {
        if mediaType == .closedCaption {
            return .closedCaption
        }
        guard let formatDescription else {
            return mediaType == .subtitle ? .text : .unknown
        }
        let coreMediaType = CMFormatDescriptionGetMediaType(formatDescription)
        let subtype = CMFormatDescriptionGetMediaSubType(formatDescription)
        if coreMediaType == kCMMediaType_ClosedCaption ||
            subtype == kCMClosedCaptionFormatType_CEA608 ||
            subtype == kCMClosedCaptionFormatType_CEA708
        {
            return .closedCaption
        }
        return .text
    }

    private static func subtitleKind(mediaSelectionOption: AVMediaSelectionOption) -> SubtitleKind {
        if mediaSelectionOption.mediaSubTypes.contains(where: { subtype in
            let rawValue = FourCharCode(truncating: subtype)
            return rawValue == kCMClosedCaptionFormatType_CEA608 || rawValue == kCMClosedCaptionFormatType_CEA708
        }) {
            return .closedCaption
        }
        return .text
    }
}

extension AVMediaPlayerTrack: @preconcurrency SubtitleInfo {
    var subtitleID: String {
        if let mediaSelectionOption {
            let language = mediaSelectionOption.extendedLanguageTag ?? mediaSelectionOption.locale?.identifier ?? "und"
            return AVLegibleSubtitlePolicy.subtitleID(languageCode: language, displayName: mediaSelectionOption.displayName)
        }
        return "av-track:\(trackID)"
    }

    var delay: TimeInterval {
        get { 0 }
        set {}
    }

    func search(for _: TimeInterval) -> [SubtitlePart] {
        []
    }
}

public extension AVAsset {
    func createImageGenerator() -> AVAssetImageGenerator {
        let imageGenerator = AVAssetImageGenerator(asset: self)
        imageGenerator.requestedTimeToleranceBefore = .zero
        imageGenerator.requestedTimeToleranceAfter = .zero
        return imageGenerator
    }

    func thumbnailImage(currentTime: CMTime, handler: @escaping (CGImage?) -> Void) {
        let imageGenerator = createImageGenerator()
        imageGenerator.requestedTimeToleranceBefore = .zero
        imageGenerator.requestedTimeToleranceAfter = .zero
        imageGenerator.generateCGImagesAsynchronously(forTimes: [NSValue(time: currentTime)]) { _, cgImage, _, _, _ in
            if let cgImage {
                handler(cgImage)
            } else {
                handler(nil)
            }
        }
    }
}

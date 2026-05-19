//
//  MEPlayerItem.swift
//  KSPlayer
//
//  Created by kintan on 2018/3/9.
//

import AVFoundation
import FFmpegKit
import Libavcodec
import Libavfilter
import Libavformat
import Libavutil

private final class SeekCompletion: @unchecked Sendable {
    private let handler: (Bool) -> Void

    init(_ handler: @escaping (Bool) -> Void) {
        self.handler = handler
    }

    func callAsFunction(_ result: Bool) {
        handler(result)
    }
}

enum FFmpegSeekabilityPolicy {
    static func isPlaylistFormat(_ formatName: String) -> Bool {
        formatName.lowercased().split(separator: ",").contains { name in
            let name = String(name)
            return name == "hls" || name == "dash" || name == "applehttp"
        }
    }

    static func seekableTimeRange(duration: TimeInterval, ioSeekable: Bool?, formatName: String) -> MediaPlaybackTimeRange? {
        guard duration > 0 else {
            return nil
        }
        if ioSeekable == false, !isPlaylistFormat(formatName) {
            return nil
        }
        return MediaPlaybackTimeRange(start: 0, duration: duration)
    }
}

enum MEPlayerStreamRecordingError: Error, Equatable, LocalizedError {
    case unsafeDestination(URL)
    case destinationExists(URL)
    case sourceAndDestinationMatch(URL)
    case noRecordableStreams

    var errorDescription: String? {
        switch self {
        case let .unsafeDestination(url):
            return "Recording destination must be a writable file URL: \(url.path)."
        case let .destinationExists(url):
            return "Recording destination already exists: \(url.path)."
        case let .sourceAndDestinationMatch(url):
            return "Recording source and destination must be different files: \(url.path)."
        case .noRecordableStreams:
            return "No active audio, active video, or compatible subtitle streams can be recorded."
        }
    }
}

enum MEPlayerStreamRecordingMediaKind: Equatable {
    case audio
    case video
    case subtitle
    case other
}

enum MEPlayerStreamRecordingPathPolicy {
    static func temporaryDestination(for destination: URL, uuid: UUID = UUID()) -> URL {
        let basename = destination.deletingPathExtension().lastPathComponent
        let pathExtension = destination.pathExtension
        let filename = ".\(basename).recording.\(uuid.uuidString)"
        let temporary = destination.deletingLastPathComponent().appendingPathComponent(filename)
        return pathExtension.isEmpty ? temporary.appendingPathExtension("tmp") : temporary.appendingPathExtension(pathExtension)
    }

    static func prepareDestination(_ destination: URL, sourceURL: URL, fileManager: FileManager = .default) throws {
        guard destination.isFileURL,
              !destination.path.isEmpty,
              !destination.lastPathComponent.isEmpty
        else {
            throw MEPlayerStreamRecordingError.unsafeDestination(destination)
        }

        let standardizedDestination = destination.standardizedFileURL
        if sourceURL.isFileURL, sourceURL.standardizedFileURL == standardizedDestination {
            throw MEPlayerStreamRecordingError.sourceAndDestinationMatch(destination)
        }

        let parent = standardizedDestination.deletingLastPathComponent()
        do {
            try fileManager.createDirectory(at: parent, withIntermediateDirectories: true)
        } catch {
            throw MEPlayerStreamRecordingError.unsafeDestination(destination)
        }

        var isDirectory = ObjCBool(false)
        if fileManager.fileExists(atPath: standardizedDestination.path, isDirectory: &isDirectory) {
            if isDirectory.boolValue {
                throw MEPlayerStreamRecordingError.unsafeDestination(destination)
            }
            throw MEPlayerStreamRecordingError.destinationExists(destination)
        }
    }

    static func prepareTemporaryDestination(_ temporaryDestination: URL, finalDestination: URL, fileManager: FileManager = .default) throws {
        guard temporaryDestination.isFileURL,
              temporaryDestination.deletingLastPathComponent().standardizedFileURL == finalDestination.deletingLastPathComponent().standardizedFileURL,
              temporaryDestination.standardizedFileURL != finalDestination.standardizedFileURL
        else {
            throw MEPlayerStreamRecordingError.unsafeDestination(temporaryDestination)
        }
        if fileManager.fileExists(atPath: temporaryDestination.path) {
            try? fileManager.removeItem(at: temporaryDestination)
        }
    }
}

enum MEPlayerStreamRecordingTrackPolicy {
    static func mediaKind(for codecType: Libavutil.AVMediaType) -> MEPlayerStreamRecordingMediaKind {
        switch codecType {
        case AVMEDIA_TYPE_AUDIO:
            return .audio
        case AVMEDIA_TYPE_VIDEO:
            return .video
        case AVMEDIA_TYPE_SUBTITLE:
            return .subtitle
        default:
            return .other
        }
    }

    static func isQuickTimeContainer(formatName: String?) -> Bool {
        formatName?.lowercased().split(separator: ",").contains { name in
            name == "mp4" || name == "mov"
        } ?? false
    }

    static func shouldCreateStream(
        mediaKind: MEPlayerStreamRecordingMediaKind,
        isEnabledPlaybackTrack: Bool,
        isQuickTimeContainer: Bool,
        isMovTextSubtitle: Bool,
        hasRecordedAudio: Bool,
        hasRecordedVideo: Bool
    ) -> Bool {
        switch mediaKind {
        case .audio:
            return isEnabledPlaybackTrack && !hasRecordedAudio
        case .video:
            return isEnabledPlaybackTrack && !hasRecordedVideo
        case .subtitle:
            return !isQuickTimeContainer || isMovTextSubtitle
        case .other:
            return false
        }
    }

    static func mediaTypeDescription(for mediaKind: MEPlayerStreamRecordingMediaKind) -> String {
        switch mediaKind {
        case .audio:
            return "audio"
        case .video:
            return "video"
        case .subtitle:
            return "subtitle"
        case .other:
            return "other"
        }
    }

    static func skipReason(
        mediaKind: MEPlayerStreamRecordingMediaKind,
        isEnabledPlaybackTrack: Bool,
        isQuickTimeContainer: Bool,
        isMovTextSubtitle: Bool,
        hasRecordedAudio: Bool,
        hasRecordedVideo: Bool
    ) -> String? {
        switch mediaKind {
        case .audio:
            if hasRecordedAudio {
                return "only the currently selected audio stream is recorded"
            }
            return isEnabledPlaybackTrack ? nil : "audio stream is not the active playback track"
        case .video:
            if hasRecordedVideo {
                return "only the currently selected video stream is recorded"
            }
            return isEnabledPlaybackTrack ? nil : "video stream is not the active playback track"
        case .subtitle:
            if isQuickTimeContainer, !isMovTextSubtitle {
                return "QuickTime containers can only stream-copy mov_text subtitles; KSPlayer external overlay subtitles are not muxed by live recording"
            }
            return nil
        case .other:
            return "stream type is not audio, video, or subtitle"
        }
    }

    static func externalSubtitlePolicyDiagnostic(isTextSubtitle: Bool, isMuxerSupported: Bool) -> String {
        if isTextSubtitle, isMuxerSupported {
            return "External text subtitles can be muxed only when they are provided as FFmpeg input streams; active KSPlayer overlay subtitles are not connected to the live stream recorder."
        }
        return "Active external subtitles are KSPlayer overlay data and are not merged by live stream recording."
    }
}

public final class MEPlayerItem: @unchecked Sendable {
    private let url: URL
    private let options: KSOptions
    private let operationQueue = OperationQueue()
    private let condition = NSCondition()
    private var formatCtx: UnsafeMutablePointer<AVFormatContext>?
    private var outputFormatCtx: UnsafeMutablePointer<AVFormatContext>?
    private var outputPacket: UnsafeMutablePointer<AVPacket>?
    private var streamMapping = [Int: Int]()
    private var recordFinalURL: URL?
    private var recordTemporaryURL: URL?
    private var recordProgressHandler: (@Sendable (StreamRecordingProgress) -> Void)?
    private var recordStreams = [StreamRecordingStreamDiagnostic]()
    private var recordPacketsWritten = Int64(0)
    private var recordPacketsSkipped = Int64(0)
    private var recordDuration = TimeInterval(0)
    private var recordFirstPacketTimestamp: TimeInterval?
    private var recordLastProgressEmitTime = 0.0
    private var fileAccess: KSSecurityScopedURLAccess?
    private var embeddedFontStore: EmbeddedFontAttachmentStore?
    private var openOperation: BlockOperation?
    private var readOperation: BlockOperation?
    private var closeOperation: BlockOperation?
    private var seekingCompletionHandler: ((Bool) -> Void)?
    // 没有音频数据可以渲染
    private var isAudioStalled = true
    private var audioClock = KSClock()
    private var videoClock = KSClock()
    private var isFirst = true
    private var isSeek = false
    private var allPlayerItemTracks = [PlayerItemTrackProtocol]()
    private var maxFrameDuration = 10.0
    private var videoAudioTracks = [CapacityProtocol]()
    private var videoTrack: SyncPlayerItemTrack<VideoVTBFrame>?
    private var audioTrack: SyncPlayerItemTrack<AudioFrame>?
    private(set) var assetTracks = [FFmpegAssetTrack]()
    private let memorySeekCache = MemorySeekCache<Packet>()
    private var videoAdaptation: VideoAdaptationState?
    private var videoDisplayCount = UInt8(0)
    private var seekByBytes = false
    private var lastVideoDisplayTime = CACurrentMediaTime()
    private var firstVideoRenderTime = 0.0
    private var firstAudioRenderTime = 0.0
    private var renderedVideoFrameCount = UInt64(0)
    private var audioRenderUpdateCount = UInt64(0)
    private var lastLowLatencyLoadingState: LoadingState?
    private var lastLowLatencyDiagnosticUpdateTime = 0.0
    private var lowLatencyLiveDiagnosticAggregator = LowLatencyLiveDiagnosticAggregator()
    public private(set) var chapters: [Chapter] = []
    public var currentPlaybackTime: TimeInterval {
        state == .seeking ? seekTime : (mainClock().time - startTime).seconds
    }

    private var seekTime = TimeInterval(0)
    private var startTime = CMTime.zero
    public private(set) var duration: TimeInterval = 0
    public private(set) var fileSize: Double = 0
    public private(set) var naturalSize = CGSize.zero
    var canRestartLoopPlayback: Bool {
        SeamlessLoopPlaybackPolicy.canRestartMEPlayerLoop(
            isLoopPlay: options.isLoopPlay,
            duration: duration,
            isSeekable: seekable
        )
    }
    private var error: NSError? {
        didSet {
            if error != nil {
                state = .failed
            }
        }
    }

    private var state = MESourceState.idle {
        didSet {
            switch state {
            case .opened:
                delegate?.sourceDidOpened()
            case .reading:
                timer.fireDate = Date.distantPast
            case .closed:
                timer.invalidate()
            case .failed:
                delegate?.sourceDidFailed(error: error)
                timer.fireDate = Date.distantFuture
            case .idle, .opening, .seeking, .paused, .finished:
                break
            }
        }
    }

    private lazy var timer: Timer = .scheduledTimer(withTimeInterval: 0.05, repeats: true) { [weak self] _ in
        self?.codecDidChangeCapacity()
    }

    lazy var dynamicInfo = DynamicInfo { [weak self] in
        // metadata可能会实时变化。所以把它放在DynamicInfo里面
        toDictionary(self?.formatCtx?.pointee.metadata)
    } bytesRead: { [weak self] in
        self?.formatCtx?.pointee.pb?.pointee.bytes_read ?? 0
    } audioBitrate: { [weak self] in
        Int(8 * (self?.audioTrack?.bitrate ?? 0))
    } videoBitrate: { [weak self] in
        Int(8 * (self?.videoTrack?.bitrate ?? 0))
    }

    private static let onceInitial: Void = {
        var result = avformat_network_init()
        av_log_set_callback { ptr, level, format, args in
            guard let format else {
                return
            }
            var log = String(cString: format)
            let arguments: CVaListPointer? = args
            if let arguments {
                log = NSString(format: log, arguments: arguments) as String
            }
            if let ptr {
                let avclass = ptr.assumingMemoryBound(to: UnsafePointer<AVClass>.self).pointee
                if avclass == avfilter_get_class() {
                    let context = ptr.assumingMemoryBound(to: AVFilterContext.self).pointee
                    if let opaque = context.graph?.pointee.opaque {
                        let options = Unmanaged<KSOptions>.fromOpaque(opaque).takeUnretainedValue()
                        options.filter(log: log)
                    }
                }
            }
            // 找不到解码器
            if log.hasPrefix("parser not found for codec") {
                KSLog(level: .error, log)
            }
            KSLog(level: LogLevel(rawValue: level) ?? .warning, log)
        }
    }()

    weak var delegate: MEPlayerDelegate?
    public init(url: URL, options: KSOptions) {
        self.url = KSDiskPrecache.playbackURL(for: url, options: options)
        self.options = options
        timer.fireDate = Date.distantFuture
        operationQueue.name = "KSPlayer_" + String(describing: self).components(separatedBy: ".").last!
        operationQueue.maxConcurrentOperationCount = 1
        operationQueue.qualityOfService = .userInteractive
        _ = MEPlayerItem.onceInitial
    }

    func select(track: some MediaPlayerTrack) -> Bool {
        if track.isEnabled {
            return false
        }
        guard let assetTrack = track as? FFmpegAssetTrack else {
            return false
        }
        guard assetTrack.isSelectableForFFmpegPlayback else {
            KSLog("[audio] unsupported audio track selection skipped: \(assetTrack.description)")
            return false
        }
        memorySeekCache.invalidate()
        assetTracks.filter { $0.mediaType == track.mediaType }.forEach {
            $0.isEnabled = track === $0
        }
        if assetTrack.mediaType == .video {
            findBestAudio(videoTrack: assetTrack)
        } else if assetTrack.mediaType == .subtitle {
            if assetTrack.isImageSubtitle {
                if !options.isSeekImageSubtitle {
                    return false
                }
            } else {
                return false
            }
        }
        seek(time: currentPlaybackTime) { _ in
        }
        return true
    }
}

// MARK: private functions

extension MEPlayerItem {
    private func openThread() {
        Self.closeCustomIO(in: self.formatCtx)
        avformat_close_input(&self.formatCtx)
        embeddedFontStore?.cleanup()
        embeddedFontStore = nil
        fileAccess = KSSecurityScopedURLAccess(url: url)
        let bluRaySource = KSBluRayURLResolver.source(for: url)
        if !url.isFileURL, url.pathExtension.caseInsensitiveCompare("iso") == .orderedSame {
            error = NSError(description: "Blu-ray ISO playback requires a local file URL. iOS and visionOS cannot mount remote ISO disk images; import the ISO or a BDMV folder into the app sandbox first.")
            return
        }
        if let bluRaySource, bluRaySource.url != url {
            fileAccess = KSSecurityScopedURLAccess(urls: [url, bluRaySource.url])
        }
        formatCtx = avformat_alloc_context()
        guard let formatCtx else {
            error = NSError(errorCode: .formatCreate)
            return
        }
        var interruptCB = AVIOInterruptCB()
        interruptCB.opaque = Unmanaged.passUnretained(self).toOpaque()
        interruptCB.callback = { ctx -> Int32 in
            guard let ctx else {
                return 0
            }
            let formatContext = Unmanaged<MEPlayerItem>.fromOpaque(ctx).takeUnretainedValue()
            switch formatContext.state {
            case .finished, .closed, .failed:
                return 1
            default:
                return 0
            }
        }
        formatCtx.pointee.interrupt_callback = interruptCB
        // avformat_close_input这个函数会调用io_close2。但是自定义协议是不会调用io_close2这个函数
//        formatCtx.pointee.io_close2 = { _, _ -> Int32 in
//            0
//        }
        setHttpProxy()
        options.prepareFormatContextOptions(for: url)
        if let bluRaySource {
            options.prepareFormatContextOptions(for: bluRaySource)
        }
        var avOptions = options.formatContextOptions.avOptions
        if bluRaySource == nil, let pb = options.process(url: url) {
            // 如果要自定义协议的话，那就用avio_alloc_context，对formatCtx.pointee.pb赋值
            formatCtx.pointee.pb = pb.getContext()
            formatCtx.pointee.flags |= AVFMT_FLAG_CUSTOM_IO
        }
        let urlString: String
        if let bluRaySource {
            urlString = bluRaySource.ffmpegURLString
        } else if url.isFileURL {
            urlString = url.path
        } else {
            urlString = url.absoluteString
        }
        var result = avformat_open_input(&self.formatCtx, urlString, nil, &avOptions)
        av_dict_free(&avOptions)
        if result == AVError.eof.code {
            state = .finished
            delegate?.sourceDidFinished()
            return
        }
        guard result == 0 else {
            if let bluRaySource {
                let description: String
                switch bluRaySource.kind {
                case .isoImage:
                    description = "Unable to open Blu-ray ISO with libbluray. iOS and visionOS do not support OS-level ISO mounting, so the ISO must be an unencrypted Blu-ray image readable directly by libbluray/libudfread, or use an imported BDMV folder."
                case .bdmvDirectory:
                    description = "Unable to open Blu-ray BDMV folder with libbluray. Make sure the selected folder contains a readable BDMV directory and that sandbox file access is still active."
                }
                error = NSError(errorCode: .formatOpenInput, userInfo: [
                    NSLocalizedDescriptionKey: description,
                    NSUnderlyingErrorKey: AVError(code: result),
                ])
            } else {
                error = .init(errorCode: .formatOpenInput, avErrorCode: result)
            }
            Self.closeCustomIO(in: self.formatCtx)
            avformat_close_input(&self.formatCtx)
            return
        }
        options.openTime = CACurrentMediaTime()
        formatCtx.pointee.flags |= AVFMT_FLAG_GENPTS
        if options.nobuffer {
            formatCtx.pointee.flags |= AVFMT_FLAG_NOBUFFER
        }
        if let probesize = options.probesize {
            formatCtx.pointee.probesize = probesize
        }
        if let maxAnalyzeDuration = options.maxAnalyzeDuration {
            formatCtx.pointee.max_analyze_duration = maxAnalyzeDuration
        }
        result = avformat_find_stream_info(formatCtx, nil)
        guard result == 0 else {
            error = .init(errorCode: .formatFindStreamInfo, avErrorCode: result)
            avformat_close_input(&self.formatCtx)
            return
        }
        // FIXME: hack, ffplay maybe should not use avio_feof() to test for the end
        formatCtx.pointee.pb?.pointee.eof_reached = 0
        let flags = formatCtx.pointee.iformat.pointee.flags
        maxFrameDuration = flags & AVFMT_TS_DISCONT == AVFMT_TS_DISCONT ? 10.0 : 3600.0
        options.findTime = CACurrentMediaTime()
        options.formatName = String(cString: formatCtx.pointee.iformat.pointee.name)
        seekByBytes = (flags & AVFMT_NO_BYTE_SEEK == 0) && (flags & AVFMT_TS_DISCONT != 0) && options.formatName != "ogg"
        if formatCtx.pointee.start_time != Int64.min {
            startTime = CMTime(value: formatCtx.pointee.start_time, timescale: AV_TIME_BASE)
            videoClock.time = startTime
            audioClock.time = startTime
        }
        duration = TimeInterval(max(formatCtx.pointee.duration, 0) / Int64(AV_TIME_BASE))
        fileSize = Double(formatCtx.pointee.bit_rate) * duration / 8
        embeddedFontStore = EmbeddedFontAttachmentStore()
        embeddedFontStore?.extractAndRegister(from: formatCtx)
        createCodec(formatCtx: formatCtx)
        if formatCtx.pointee.nb_chapters > 0 {
            chapters.removeAll()
            for i in 0 ..< formatCtx.pointee.nb_chapters {
                if let chapter = formatCtx.pointee.chapters[Int(i)]?.pointee {
                    let timeBase = Timebase(chapter.time_base)
                    let start = timeBase.cmtime(for: chapter.start).seconds
                    let end = timeBase.cmtime(for: chapter.end).seconds
                    let metadata = toDictionary(chapter.metadata)
                    let title = metadata["title"] ?? ""
                    chapters.append(Chapter(start: start, end: end, title: title))
                }
            }
        }

        if let outputURL = options.outputURL {
            startRecord(url: outputURL)
        }
        if videoTrack == nil, audioTrack == nil {
            if error == nil {
                let unsupportedTrack = assetTracks.first { !$0.isSelectableForFFmpegPlayback }
                error = NSError(description: unsupportedTrack?.description ?? "No decodable audio or video streams found")
            }
            state = .failed
        } else {
            state = .opened
            read()
        }
    }

    func startRecord(url: URL, progress: (@Sendable (StreamRecordingProgress) -> Void)? = nil) {
        stopRecord()
        let temporaryURL = MEPlayerStreamRecordingPathPolicy.temporaryDestination(for: url)
        let filename = temporaryURL.path
        do {
            try MEPlayerStreamRecordingPathPolicy.prepareDestination(url, sourceURL: self.url)
            try MEPlayerStreamRecordingPathPolicy.prepareTemporaryDestination(temporaryURL, finalDestination: url)
        } catch {
            KSLog(error)
            return
        }
        recordFinalURL = url.standardizedFileURL
        recordTemporaryURL = temporaryURL.standardizedFileURL
        recordProgressHandler = progress
        recordStreams.removeAll()
        recordPacketsWritten = 0
        recordPacketsSkipped = 0
        recordDuration = 0
        recordFirstPacketTimestamp = nil
        recordLastProgressEmitTime = 0
        emitRecordProgress(phase: .starting, force: true, message: "Recording to a temporary file until stopRecord() finalizes the output.")
        var ret = avformat_alloc_output_context2(&outputFormatCtx, nil, nil, filename)
        guard let outputFormatCtx, let formatCtx else {
            let error = NSError(errorCode: .formatOutputCreate, avErrorCode: ret)
            KSLog(error)
            resetRecordOutput(writeTrailer: false, error: error)
            return
        }
        var index = 0
        var hasRecordedAudio = false
        var hasRecordedVideo = false
        let formatName = outputFormatCtx.pointee.oformat.pointee.name.flatMap { String(cString: $0) }
        let isQuickTimeContainer = MEPlayerStreamRecordingTrackPolicy.isQuickTimeContainer(formatName: formatName)
        let enabledPlaybackTrackIDs = Set(assetTracks.filter { $0.isEnabled && ($0.mediaType == .audio || $0.mediaType == .video) }.map { Int($0.trackID) })
        for i in 0 ..< Int(formatCtx.pointee.nb_streams) {
            if let inputStream = formatCtx.pointee.streams[i] {
                let codecType = inputStream.pointee.codecpar.pointee.codec_type
                let codecID = inputStream.pointee.codecpar.pointee.codec_id
                let mediaKind = MEPlayerStreamRecordingTrackPolicy.mediaKind(for: codecType)
                let isEnabledPlaybackTrack = enabledPlaybackTrackIDs.contains(i)
                let isMovTextSubtitle = codecID == AV_CODEC_ID_MOV_TEXT
                if MEPlayerStreamRecordingTrackPolicy.shouldCreateStream(
                    mediaKind: mediaKind,
                    isEnabledPlaybackTrack: isEnabledPlaybackTrack,
                    isQuickTimeContainer: isQuickTimeContainer,
                    isMovTextSubtitle: isMovTextSubtitle,
                    hasRecordedAudio: hasRecordedAudio,
                    hasRecordedVideo: hasRecordedVideo
                ) {
                    if let outStream = avformat_new_stream(outputFormatCtx, nil) {
                        streamMapping[i] = index
                        let outputIndex = index
                        index += 1
                        ret = avcodec_parameters_copy(outStream.pointee.codecpar, inputStream.pointee.codecpar)
                        guard ret >= 0 else {
                            let error = NSError(errorCode: .codecContextSetParam, avErrorCode: ret)
                            KSLog(error)
                            resetRecordOutput(writeTrailer: false, error: error)
                            return
                        }
                        if codecID == AV_CODEC_ID_HEVC {
                            outStream.pointee.codecpar.pointee.codec_tag = CMFormatDescription.MediaSubType.hevc.rawValue.bigEndian
                        } else {
                            outStream.pointee.codecpar.pointee.codec_tag = 0
                        }
                        outStream.pointee.time_base = inputStream.pointee.time_base
                        av_dict_copy(&outStream.pointee.metadata, inputStream.pointee.metadata, 0)
                        recordStreams.append(StreamRecordingStreamDiagnostic(
                            inputIndex: i,
                            outputIndex: outputIndex,
                            mediaType: MEPlayerStreamRecordingTrackPolicy.mediaTypeDescription(for: mediaKind),
                            codecName: Self.codecName(for: codecID),
                            action: .recorded
                        ))
                        if mediaKind == .audio {
                            hasRecordedAudio = true
                        } else if mediaKind == .video {
                            hasRecordedVideo = true
                        }
                    }
                } else if let reason = MEPlayerStreamRecordingTrackPolicy.skipReason(
                    mediaKind: mediaKind,
                    isEnabledPlaybackTrack: isEnabledPlaybackTrack,
                    isQuickTimeContainer: isQuickTimeContainer,
                    isMovTextSubtitle: isMovTextSubtitle,
                    hasRecordedAudio: hasRecordedAudio,
                    hasRecordedVideo: hasRecordedVideo
                ) {
                    recordStreams.append(StreamRecordingStreamDiagnostic(
                        inputIndex: i,
                        outputIndex: nil,
                        mediaType: MEPlayerStreamRecordingTrackPolicy.mediaTypeDescription(for: mediaKind),
                        codecName: Self.codecName(for: codecID),
                        action: .skipped,
                        reason: reason
                    ))
                }
            }
        }
        guard !streamMapping.isEmpty else {
            let error = MEPlayerStreamRecordingError.noRecordableStreams
            KSLog(error)
            resetRecordOutput(writeTrailer: false, error: error)
            return
        }
        av_dict_copy(&outputFormatCtx.pointee.metadata, formatCtx.pointee.metadata, 0)
        ret = avio_open(&(outputFormatCtx.pointee.pb), filename, AVIO_FLAG_WRITE)
        guard ret >= 0 else {
            let error = NSError(errorCode: .formatWriteHeader, avErrorCode: ret)
            KSLog(error)
            resetRecordOutput(writeTrailer: false, error: error)
            return
        }
        ret = avformat_write_header(outputFormatCtx, nil)
        guard ret >= 0 else {
            let error = NSError(errorCode: .formatWriteHeader, avErrorCode: ret)
            KSLog(error)
            resetRecordOutput(writeTrailer: false, error: error)
            return
        }
        outputPacket = av_packet_alloc()
        emitRecordProgress(phase: .recording, force: true)
    }

    private static func codecName(for codecID: AVCodecID) -> String? {
        guard let name = avcodec_get_name(codecID) else {
            return nil
        }
        return String(cString: name)
    }

    private var recordBytesWritten: Int64 {
        outputFormatCtx?.pointee.pb?.pointee.bytes_written ?? 0
    }

    private func emitRecordProgress(phase: StreamRecordingPhase, force: Bool = false, message: String? = nil) {
        let now = CACurrentMediaTime()
        guard force || now - recordLastProgressEmitTime >= 0.25 else {
            return
        }
        recordLastProgressEmitTime = now
        let progress = StreamRecordingProgress(
            phase: phase,
            destinationURL: recordFinalURL,
            temporaryURL: recordTemporaryURL,
            duration: recordDuration,
            bytesWritten: recordBytesWritten,
            packetsWritten: recordPacketsWritten,
            packetsSkipped: recordPacketsSkipped,
            droppedVideoFrameCount: dynamicInfo.droppedVideoFrameCount,
            droppedVideoPacketCount: dynamicInfo.droppedVideoPacketCount,
            streams: recordStreams,
            message: message
        )
        dynamicInfo.streamRecordingProgress = progress
        options.streamRecordingProgressHandler?(progress)
        recordProgressHandler?(progress)
    }

    private func updateRecordDuration(packet: UnsafeMutablePointer<AVPacket>, inputTimeBase: AVRational) {
        let timestamp = packet.pointee.pts == swift_AV_NOPTS_VALUE ? packet.pointee.dts : packet.pointee.pts
        guard timestamp != swift_AV_NOPTS_VALUE else {
            return
        }
        let packetTime = TimeInterval(av_rescale_q(timestamp, inputTimeBase, AVRational(num: 1, den: AV_TIME_BASE))) / TimeInterval(AV_TIME_BASE)
        if recordFirstPacketTimestamp == nil {
            recordFirstPacketTimestamp = packetTime
        }
        if let recordFirstPacketTimestamp {
            recordDuration = max(recordDuration, packetTime - recordFirstPacketTimestamp)
        }
    }

    private func createCodec(formatCtx: UnsafeMutablePointer<AVFormatContext>) {
        allPlayerItemTracks.removeAll()
        assetTracks.removeAll()
        memorySeekCache.invalidate()
        videoAdaptation = nil
        videoTrack = nil
        audioTrack = nil
        videoAudioTracks.removeAll()
        assetTracks = (0 ..< Int(formatCtx.pointee.nb_streams)).compactMap { i in
            if let coreStream = formatCtx.pointee.streams[i] {
                coreStream.pointee.discard = AVDISCARD_ALL
                if let assetTrack = FFmpegAssetTrack(stream: coreStream) {
                    if assetTrack.mediaType == .subtitle {
                        assetTrack.embeddedFontDirectoryURL = embeddedFontStore?.fontsDirectoryURL
                        let subtitle = SyncPlayerItemTrack<SubtitleFrame>(mediaType: .subtitle, frameCapacity: 255, options: options)
                        assetTrack.subtitle = subtitle
                        allPlayerItemTracks.append(subtitle)
                    }
                    assetTrack.seekByBytes = seekByBytes
                    return assetTrack
                }
            }
            return nil
        }
        var videoIndex: Int32 = -1
        if !options.videoDisable {
            let allVideos = assetTracks.filter { $0.mediaType == .video }
            let videos = FFmpegAssetTrack.decodableVideoTracks(allVideos)
            let wantedStreamNb: Int32
            if !videos.isEmpty, let index = options.wantedVideo(tracks: videos) {
                wantedStreamNb = videos[index].trackID
            } else {
                wantedStreamNb = -1
            }
            if !videos.isEmpty {
                videoIndex = av_find_best_stream(formatCtx, AVMEDIA_TYPE_VIDEO, wantedStreamNb, -1, nil, 0)
            }
            let selectedVideo = videos.first {
                videoIndex >= 0 && $0.trackID == videoIndex
            } ?? videos.first
            if let first = selectedVideo {
                videoIndex = first.trackID
                first.isEnabled = true
                let rotation = first.rotation
                if rotation > 0, options.autoRotate {
                    options.hardwareDecode = false
                    if abs(rotation - 90) <= 1 {
                        options.videoFilters.append("transpose=clock")
                    } else if abs(rotation - 180) <= 1 {
                        options.videoFilters.append("hflip")
                        options.videoFilters.append("vflip")
                    } else if abs(rotation - 270) <= 1 {
                        options.videoFilters.append("transpose=cclock")
                    } else if abs(rotation) > 1 {
                        options.videoFilters.append("rotate=\(rotation)*PI/180")
                    }
                }
                naturalSize = abs(rotation - 90) <= 1 || abs(rotation - 270) <= 1 ? first.naturalSize.reverse : first.naturalSize
                options.process(assetTrack: first)
                let frameCapacity = options.videoFrameMaxCount(fps: first.nominalFrameRate, naturalSize: naturalSize, isLive: duration == 0)
                let track = options.syncDecodeVideo ? SyncPlayerItemTrack<VideoVTBFrame>(mediaType: .video, frameCapacity: frameCapacity, options: options) : AsyncPlayerItemTrack<VideoVTBFrame>(mediaType: .video, frameCapacity: frameCapacity, options: options)
                track.delegate = self
                allPlayerItemTracks.append(track)
                videoTrack = track
                if first.codecpar.codec_id != AV_CODEC_ID_MJPEG {
                    videoAudioTracks.append(track)
                }
                let bitRates = videos.map(\.bitRate).filter {
                    $0 > 0
                }
                if bitRates.count > 1, options.videoAdaptable {
                    let bitRateState = VideoAdaptationState.BitRateState(bitRate: first.bitRate, time: CACurrentMediaTime())
                    videoAdaptation = VideoAdaptationState(bitRates: bitRates.sorted(by: <), duration: duration, fps: first.nominalFrameRate, bitRateStates: [bitRateState])
                }
            } else if let unsupportedVideo = allVideos.first(where: { !$0.videoDecodeSupport.isSupported }) {
                KSLog("[video] unsupported video track skipped: \(unsupportedVideo.description)")
            }
        }

        let audios = assetTracks.filter { $0.mediaType == .audio }
        let decodableAudios = FFmpegAssetTrack.decodableAudioTracks(audios)
        let wantedStreamNb: Int32
        if !decodableAudios.isEmpty, let index = options.wantedAudio(tracks: decodableAudios) {
            wantedStreamNb = decodableAudios[index].trackID
        } else {
            wantedStreamNb = -1
        }
        let index = av_find_best_stream(formatCtx, AVMEDIA_TYPE_AUDIO, wantedStreamNb, videoIndex, nil, 0)
        let selectedAudio = decodableAudios.first {
            index >= 0 && $0.trackID == index
        } ?? decodableAudios.first
        if index >= 0, selectedAudio?.trackID != index, let unsupportedAudio = audios.first(where: { $0.trackID == index }) {
            KSLog("[audio] best audio track skipped: \(unsupportedAudio.description)")
        }
        if selectedAudio == nil, let unsupportedAudio = audios.first {
            KSLog("[audio] unsupported audio track skipped: \(unsupportedAudio.description)")
            if videoTrack == nil {
                error = NSError(description: unsupportedAudio.description)
            }
        }
        if let first = selectedAudio, first.codecpar.codec_id != AV_CODEC_ID_NONE {
            first.isEnabled = true
            first.audioDescriptor?.updateAudioFormat(options: options)
            options.process(assetTrack: first)
            // 音频要比较所有的音轨，因为truehd的fps是1200，跟其他的音轨差距太大了
            let fps = audios.map(\.nominalFrameRate).max() ?? 44
            let frameCapacity = options.audioFrameMaxCount(fps: fps, channelCount: Int(first.audioDescriptor?.audioFormat.channelCount ?? 2))
            let track = options.syncDecodeAudio ? SyncPlayerItemTrack<AudioFrame>(mediaType: .audio, frameCapacity: frameCapacity, options: options) : AsyncPlayerItemTrack<AudioFrame>(mediaType: .audio, frameCapacity: frameCapacity, options: options)
            track.delegate = self
            allPlayerItemTracks.append(track)
            audioTrack = track
            videoAudioTracks.append(track)
            isAudioStalled = false
        }
    }

    private func read() {
        readOperation = BlockOperation { [weak self] in
            guard let self else { return }
            Thread.current.name = (self.operationQueue.name ?? "") + "_read"
            Thread.current.stackSize = KSOptions.stackSize
            self.readThread()
        }
        readOperation?.queuePriority = .veryHigh
        readOperation?.qualityOfService = .userInteractive
        if let readOperation {
            operationQueue.addOperation(readOperation)
        }
    }

    private func readThread() {
        if state == .opened {
            if options.startPlayTime > 0 {
                let timestamp = startTime + CMTime(seconds: options.startPlayTime)
                let flags = seekByBytes ? AVSEEK_FLAG_BYTE : 0
                let seekStartTime = CACurrentMediaTime()
                _ = avformat_seek_file(formatCtx, -1, Int64.min, timestamp.value, Int64.max, flags)
                audioClock.time = timestamp
                videoClock.time = timestamp
                KSLog("start PlayTime: \(timestamp.seconds) spend Time: \(CACurrentMediaTime() - seekStartTime)")
            }
            state = .reading
        }
        allPlayerItemTracks.forEach { $0.decode() }
        while [MESourceState.paused, .seeking, .reading].contains(state) {
            if state == .paused {
                condition.wait()
            }
            if state == .seeking {
                let seekToTime = seekTime
                let time = mainClock().time
                var increase = Int64(seekTime + startTime.seconds - time.seconds)
                var seekFlags = options.seekFlags
                let timeStamp: Int64
                if seekByBytes {
                    seekFlags |= AVSEEK_FLAG_BYTE
                    if let bitRate = formatCtx?.pointee.bit_rate {
                        increase = increase * bitRate / 8
                    } else {
                        increase *= 180_000
                    }
                    var position = Int64(-1)
                    if position < 0 {
                        position = videoClock.position
                    }
                    if position < 0 {
                        position = audioClock.position
                    }
                    if position < 0 {
                        position = avio_tell(formatCtx?.pointee.pb)
                    }
                    timeStamp = position + increase
                } else {
                    increase *= Int64(AV_TIME_BASE)
                    timeStamp = Int64(time.seconds) * Int64(AV_TIME_BASE) + increase
                }
                let seekMin = increase > 0 ? timeStamp - increase + 2 : Int64.min
                let seekMax = increase < 0 ? timeStamp - increase - 2 : Int64.max
                // can not seek to key frame
                let seekStartTime = CACurrentMediaTime()
                var result = avformat_seek_file(formatCtx, -1, seekMin, timeStamp, seekMax, seekFlags)
//                var result = av_seek_frame(formatCtx, -1, timeStamp, seekFlags)
                // When seeking before the beginning of the file, and seeking fails,
                // try again without the backwards flag to make it seek to the
                // beginning.
                if result < 0, seekFlags & AVSEEK_FLAG_BACKWARD == AVSEEK_FLAG_BACKWARD {
                    KSLog("seek to \(seekToTime) failed. seekFlags remove BACKWARD")
                    options.seekFlags &= ~AVSEEK_FLAG_BACKWARD
                    seekFlags &= ~AVSEEK_FLAG_BACKWARD
                    result = avformat_seek_file(formatCtx, -1, seekMin, timeStamp, seekMax, seekFlags)
                }
                KSLog("seek to \(seekToTime) spend Time: \(CACurrentMediaTime() - seekStartTime)")
                if state == .closed {
                    break
                }
                if seekToTime != seekTime {
                    continue
                }
                let seekSucceeded = result >= 0
                isSeek = true
                allPlayerItemTracks.forEach { $0.seek(time: seekToTime) }
                DispatchQueue.main.async { [weak self] in
                    guard let self else { return }
                    self.seekingCompletionHandler?(seekSucceeded)
                    self.seekingCompletionHandler = nil
                }
                audioClock.time = CMTime(seconds: seekToTime, preferredTimescale: time.timescale) + startTime
                videoClock.time = CMTime(seconds: seekToTime, preferredTimescale: time.timescale) + startTime
                state = .reading
            } else if state == .reading {
                autoreleasepool {
                    _ = reading()
                }
            }
        }
    }

    private func reading() -> Int32 {
        let packet = Packet()
        guard let corePacket = packet.corePacket else {
            return 0
        }
        let readResult = av_read_frame(formatCtx, corePacket)
        if state == .closed {
            return 0
        }
        if readResult == 0 {
            if let outputFormatCtx, let formatCtx {
                let index = Int(corePacket.pointee.stream_index)
                if let outputIndex = streamMapping[index],
                   let inputTb = formatCtx.pointee.streams[index]?.pointee.time_base,
                   let outputTb = outputFormatCtx.pointee.streams[outputIndex]?.pointee.time_base,
                   let outputPacket
                {
                    av_packet_ref(outputPacket, corePacket)
                    outputPacket.pointee.stream_index = Int32(outputIndex)
                    av_packet_rescale_ts(outputPacket, inputTb, outputTb)
                    outputPacket.pointee.pos = -1
                    let ret = av_interleaved_write_frame(outputFormatCtx, outputPacket)
                    if ret < 0 {
                        recordPacketsSkipped += 1
                        av_packet_unref(outputPacket)
                        KSLog("can not av_interleaved_write_frame")
                    } else {
                        recordPacketsWritten += 1
                        updateRecordDuration(packet: corePacket, inputTimeBase: inputTb)
                        emitRecordProgress(phase: .recording)
                    }
                } else {
                    recordPacketsSkipped += 1
                }
            }
            if corePacket.pointee.size <= 0 {
                return 0
            }
            let first = assetTracks.first { $0.trackID == corePacket.pointee.stream_index }
            if let first, first.isEnabled {
                packet.assetTrack = first
                storeMemorySeekPacketIfNeeded(packet)
                if first.mediaType == .video {
                    first.recordDolbyVisionPacket(
                        packet,
                        compositorAvailability: options.dolbyVisionFELCompositorAvailability,
                        playbackPolicy: options.dolbyVisionFELPlaybackPolicy
                    )
                    if let diagnostic = first.dolbyVisionPlaybackDiagnostic {
                        options.dolbyVisionPlaybackDiagnostic = diagnostic
                        if diagnostic.blocksPlayback {
                            error = NSError(description: diagnostic.description)
                            return 0
                        }
                    }
                    if options.readVideoTime == 0 {
                        options.readVideoTime = CACurrentMediaTime()
                    }
                    videoTrack?.putPacket(packet: packet)
                } else if first.mediaType == .audio {
                    if options.readAudioTime == 0 {
                        options.readAudioTime = CACurrentMediaTime()
                    }
                    audioTrack?.putPacket(packet: packet)
                } else {
                    first.subtitle?.putPacket(packet: packet)
                }
            }
        } else {
            if readResult == AVError.eof.code || avio_feof(formatCtx?.pointee.pb) > 0 {
                let tracksAlreadyLooping = allPlayerItemTracks.contains { $0.isLoopModel }
                let usesAsyncPacketQueue = !options.syncDecodeVideo && !options.syncDecodeAudio
                if SeamlessLoopPlaybackPolicy.shouldUseMEPlayerPacketQueue(
                    isLoopPlay: options.isLoopPlay,
                    isSeamlessLoopEnabled: options.isSeamlessLoopEnabled,
                    usesAsyncPacketQueue: usesAsyncPacketQueue,
                    tracksAlreadyLooping: tracksAlreadyLooping,
                    canSeekToStart: canRestartLoopPlayback
                ), seekDemuxerToLoopStart() {
                    memorySeekCache.invalidate()
                    allPlayerItemTracks.forEach { $0.isLoopModel = true }
                } else {
                    finishReadingAtEndOfFile()
                }
            } else {
                //                        if IS_AVERROR_INVALIDDATA(readResult)
                error = .init(errorCode: .readFrame, avErrorCode: readResult)
            }
        }
        return readResult
    }

    private func seekDemuxerToLoopStart() -> Bool {
        guard let formatCtx else {
            return false
        }
        let result = av_seek_frame(formatCtx, -1, startTime.value, AVSEEK_FLAG_BACKWARD)
        guard result >= 0 else {
            return false
        }
        formatCtx.pointee.pb?.pointee.eof_reached = 0
        return true
    }

    private func finishReadingAtEndOfFile() {
        allPlayerItemTracks.forEach { $0.isEndOfFile = true }
        state = .finished
    }

    private func pause() {
        if state == .reading {
            state = .paused
        }
    }

    private func resume() {
        if state == .paused {
            state = .reading
            condition.signal()
        }
    }

    private var isMemorySeekCacheActive: Bool {
        options.isMemorySeekCacheEnabled
            && options.memorySeekCacheDuration > 0
            && options.memorySeekCacheMaxByteSize > 0
            && duration > 0
            && !seekByBytes
    }

    private func storeMemorySeekPacketIfNeeded(_ packet: Packet) {
        guard isMemorySeekCacheActive, (packet.assetTrack.mediaType == .audio || packet.assetTrack.mediaType == .video) else {
            return
        }
        memorySeekCache.maxDuration = options.memorySeekCacheDuration
        memorySeekCache.maxByteSize = options.memorySeekCacheMaxByteSize
        memorySeekCache.store(packet)
    }

    private func restoreMemorySeekCache(time: TimeInterval, completion: @escaping ((Bool) -> Void)) -> Bool {
        guard isMemorySeekCacheActive else {
            return false
        }
        let requiredTrackIDs = Set(assetTracks.compactMap { track -> Int32? in
            guard track.isEnabled, (track.mediaType == .audio || track.mediaType == .video) else {
                return nil
            }
            return track.trackID
        })
        guard let packets = memorySeekCache.packets(for: time, requiredTrackIDs: requiredTrackIDs), !packets.isEmpty else {
            return false
        }
        isSeek = true
        allPlayerItemTracks.forEach { $0.seek(time: time) }
        packets.forEach { putMemorySeekPacket($0) }
        let clockTime = CMTime(seconds: time, preferredTimescale: mainClock().time.timescale) + startTime
        audioClock.time = clockTime
        videoClock.time = clockTime
        state = .reading
        let completion = SeekCompletion(completion)
        DispatchQueue.main.async {
            completion(true)
        }
        return true
    }

    private func putMemorySeekPacket(_ packet: Packet) {
        switch packet.assetTrack.mediaType {
        case .video:
            videoTrack?.putPacket(packet: packet)
        case .audio:
            audioTrack?.putPacket(packet: packet)
        default:
            break
        }
    }

    private func resetLoopClocksToStart() {
        audioClock.time = startTime
        videoClock.time = startTime
        isSeek = true
        isAudioStalled = audioTrack == nil
        if options.isOfflineSubtitleGenerationEnabled {
            options.offlineSubtitleGenerator?.reset()
        }
    }
}

// MARK: MediaPlayback

extension MEPlayerItem: MediaPlayback {
    var seekable: Bool {
        seekableTimeRange != nil
    }

    var seekableTimeRange: MediaPlaybackTimeRange? {
        guard let formatCtx, duration > 0 else {
            return nil
        }
        let formatName = String(cString: formatCtx.pointee.iformat.pointee.name).lowercased()
        let ioSeekable = formatCtx.pointee.pb.map { $0.pointee.seekable > 0 }
        return FFmpegSeekabilityPolicy.seekableTimeRange(duration: duration, ioSeekable: ioSeekable, formatName: formatName)
    }

    public func prepareToPlay() {
        state = .opening
        openOperation = BlockOperation { [weak self] in
            guard let self else { return }
            Thread.current.name = (self.operationQueue.name ?? "") + "_open"
            Thread.current.stackSize = KSOptions.stackSize
            self.openThread()
        }
        openOperation?.queuePriority = .veryHigh
        openOperation?.qualityOfService = .userInteractive
        if let openOperation {
            operationQueue.addOperation(openOperation)
        }
    }

    public func shutdown() {
        guard state != .closed else { return }
        state = .closed
        memorySeekCache.invalidate()
        av_packet_free(&outputPacket)
        stopRecord()
        // 故意循环引用。等结束了。才释放
        let closeOperation = BlockOperation {
            Thread.current.name = (self.operationQueue.name ?? "") + "_close"
            self.memorySeekCache.invalidate()
            self.allPlayerItemTracks.forEach { $0.shutdown() }
            KSLog("清空formatCtx")
            Self.closeCustomIO(in: self.formatCtx)
            // 不要自己来释放pb。不然第二次播放同一个url会出问题
//            self.formatCtx?.pointee.pb = nil
            self.formatCtx?.pointee.interrupt_callback.opaque = nil
            self.formatCtx?.pointee.interrupt_callback.callback = nil
            self.embeddedFontStore?.cleanup()
            self.embeddedFontStore = nil
            avformat_close_input(&self.formatCtx)
            self.fileAccess?.stop()
            self.fileAccess = nil
            self.duration = 0
            self.closeOperation = nil
            self.operationQueue.cancelAllOperations()
        }
        closeOperation.queuePriority = .veryHigh
        closeOperation.qualityOfService = .userInteractive
        if let readOperation {
            readOperation.cancel()
            closeOperation.addDependency(readOperation)
        } else if let openOperation {
            openOperation.cancel()
            closeOperation.addDependency(openOperation)
        }
        operationQueue.addOperation(closeOperation)
        condition.signal()
        if options.syncDecodeVideo || options.syncDecodeAudio {
            DispatchQueue.global().async { [weak self] in
                self?.allPlayerItemTracks.forEach { $0.shutdown() }
            }
        }
        self.closeOperation = closeOperation
    }

    func stopRecord() {
        resetRecordOutput(writeTrailer: true)
    }

    private func resetRecordOutput(writeTrailer: Bool, error: Error? = nil) {
        let finalURL = recordFinalURL
        let temporaryURL = recordTemporaryURL
        var finalizeError = error
        if writeTrailer, outputFormatCtx != nil {
            emitRecordProgress(phase: .finalizing, force: true)
        }
        if let outputFormatCtx {
            if writeTrailer {
                let result = av_write_trailer(outputFormatCtx)
                if result < 0 {
                    finalizeError = KSPlayerClipExportError.ffmpegFailure(String(avErrorCode: result))
                }
            }
            if outputFormatCtx.pointee.oformat.pointee.flags & AVFMT_NOFILE == 0 {
                avio_closep(&outputFormatCtx.pointee.pb)
            }
            avformat_free_context(outputFormatCtx)
            self.outputFormatCtx = nil
        }
        av_packet_free(&outputPacket)
        if writeTrailer, finalizeError == nil, let finalURL, let temporaryURL {
            do {
                if FileManager.default.fileExists(atPath: finalURL.path) {
                    throw MEPlayerStreamRecordingError.destinationExists(finalURL)
                }
                try FileManager.default.moveItem(at: temporaryURL, to: finalURL)
                emitRecordProgress(phase: .finished, force: true, message: "Recording finalized by moving the temporary file into place.")
            } catch {
                finalizeError = error
            }
        }
        if let temporaryURL, finalizeError != nil || !writeTrailer {
            try? FileManager.default.removeItem(at: temporaryURL)
        }
        if let finalizeError {
            emitRecordProgress(phase: .failed, force: true, message: finalizeError.localizedDescription)
        }
        streamMapping.removeAll()
        recordFinalURL = nil
        recordTemporaryURL = nil
        recordProgressHandler = nil
        recordStreams.removeAll()
        recordPacketsWritten = 0
        recordPacketsSkipped = 0
        recordDuration = 0
        recordFirstPacketTimestamp = nil
        recordLastProgressEmitTime = 0
    }

    public func seek(time: TimeInterval, completion: @escaping ((Bool) -> Void)) {
        if state == .reading || state == .paused {
            seekTime = time
            let isShortMemorySeek = abs(currentPlaybackTime - time) <= options.memorySeekCacheDuration
            if isShortMemorySeek, restoreMemorySeekCache(time: time, completion: completion) {
                if options.isOfflineSubtitleGenerationEnabled {
                    options.offlineSubtitleGenerator?.reset()
                }
                isAudioStalled = audioTrack == nil
                return
            }
            if !isShortMemorySeek {
                memorySeekCache.invalidate()
            }
            state = .seeking
            seekingCompletionHandler = completion
            condition.broadcast()
            allPlayerItemTracks.forEach { $0.seek(time: time) }
        } else if state == .finished {
            seekTime = time
            memorySeekCache.invalidate()
            state = .seeking
            seekingCompletionHandler = completion
            read()
        } else if state == .seeking {
            seekTime = time
            seekingCompletionHandler = completion
        }
        if options.isOfflineSubtitleGenerationEnabled {
            options.offlineSubtitleGenerator?.reset()
        }
        isAudioStalled = audioTrack == nil
    }
}

extension MEPlayerItem: CodecCapacityDelegate {
    func codecDidChangeCapacity() {
        let loadingState = options.playable(capacitys: videoAudioTracks, isFirst: isFirst, isSeek: isSeek)
        updateLowLatencyLiveDiagnostic(loadingState: loadingState)
        delegate?.sourceDidChange(loadingState: loadingState)
        if loadingState.isPlayable {
            isFirst = false
            isSeek = false
            if loadingState.loadedTime > options.maxBufferDuration {
                adaptableVideo(loadingState: loadingState)
                pause()
            } else if loadingState.loadedTime < options.maxBufferDuration / 2 {
                resume()
            }
        } else {
            resume()
            adaptableVideo(loadingState: loadingState)
        }
    }

    private func updateLowLatencyLiveDiagnostic(loadingState: LoadingState) {
        guard let profile = options.lowLatencyLiveProfile else {
            dynamicInfo.lowLatencyLiveDiagnostic = nil
            return
        }
        lastLowLatencyLoadingState = loadingState
        lastLowLatencyDiagnosticUpdateTime = CACurrentMediaTime()
        let firstVideoReadToDecodeDuration = elapsed(from: options.readVideoTime, to: options.decodeVideoTime)
        let firstAudioReadToDecodeDuration = elapsed(from: options.readAudioTime, to: options.decodeAudioTime)
        let firstVideoDecodeToRenderDuration = elapsed(from: options.decodeVideoTime, to: firstVideoRenderTime)
        let firstAudioDecodeToRenderDuration = elapsed(from: options.decodeAudioTime, to: firstAudioRenderTime)
        let firstVideoReadToRenderDuration = elapsed(from: options.readVideoTime, to: firstVideoRenderTime)
        let firstAudioReadToRenderDuration = elapsed(from: options.readAudioTime, to: firstAudioRenderTime)
        let audioLatencyEstimate = LowLatencyLivePlaybackPolicy.audioLatencyEstimate(
            profile: profile,
            audioFrameCount: audioTrack?.frameCount ?? 0,
            audioFPS: audioTrack?.fps ?? 0,
            preferredAudioIOBufferDuration: options.preferredAudioIOBufferDuration
        )
        let rollingMetrics = lowLatencyLiveDiagnosticAggregator.record(
            bufferedDuration: loadingState.loadedTime,
            packetCount: loadingState.packetCount,
            frameCount: loadingState.frameCount,
            audioVideoSyncDiff: dynamicInfo.audioVideoSyncDiff,
            displayFPS: dynamicInfo.displayFPS,
            videoReadToDecodeDuration: firstVideoReadToDecodeDuration,
            videoDecodeToRenderDuration: firstVideoDecodeToRenderDuration,
            videoReadToRenderDuration: firstVideoReadToRenderDuration,
            audioLatencyEstimate: audioLatencyEstimate
        )
        dynamicInfo.lowLatencyLiveDiagnostic = LowLatencyLiveDiagnostic(
            profile: profile,
            bufferedDuration: loadingState.loadedTime,
            packetCount: loadingState.packetCount,
            frameCount: loadingState.frameCount,
            droppedVideoFrameCount: dynamicInfo.droppedVideoFrameCount,
            droppedVideoPacketCount: dynamicInfo.droppedVideoPacketCount,
            renderedVideoFrameCount: renderedVideoFrameCount,
            audioRenderUpdateCount: audioRenderUpdateCount,
            audioVideoSyncDiff: dynamicInfo.audioVideoSyncDiff,
            displayFPS: dynamicInfo.displayFPS,
            audioLatencyEstimate: audioLatencyEstimate,
            timestamps: lowLatencyLivePipelineTimestamps,
            rollingMetrics: rollingMetrics,
            prepareToReadyDuration: elapsed(from: options.prepareTime, to: options.readyTime),
            openToReadyDuration: elapsed(from: options.openTime, to: options.readyTime),
            startupToFirstVideoFrameDuration: elapsed(from: options.prepareTime, to: firstVideoRenderTime),
            startupToFirstAudioFrameDuration: elapsed(from: options.prepareTime, to: firstAudioRenderTime),
            firstVideoReadToDecodeDuration: firstVideoReadToDecodeDuration,
            firstAudioReadToDecodeDuration: firstAudioReadToDecodeDuration,
            firstVideoDecodeToRenderDuration: firstVideoDecodeToRenderDuration,
            firstAudioDecodeToRenderDuration: firstAudioDecodeToRenderDuration,
            firstVideoReadToRenderDuration: firstVideoReadToRenderDuration,
            firstAudioReadToRenderDuration: firstAudioReadToRenderDuration
        )
    }

    private func elapsed(from start: TimeInterval, to end: TimeInterval) -> TimeInterval? {
        guard start > 0, end > 0, end >= start else {
            return nil
        }
        return end - start
    }

    private var lowLatencyLivePipelineTimestamps: LowLatencyLivePipelineTimestamps {
        let base = options.prepareTime > 0 ? options.prepareTime : 0
        return LowLatencyLivePipelineTimestamps(
            prepare: relativeTimestamp(options.prepareTime, base: base),
            open: relativeTimestamp(options.openTime, base: base),
            findStreams: relativeTimestamp(options.findTime, base: base),
            ready: relativeTimestamp(options.readyTime, base: base),
            firstVideoRead: relativeTimestamp(options.readVideoTime, base: base),
            firstAudioRead: relativeTimestamp(options.readAudioTime, base: base),
            firstVideoDecode: relativeTimestamp(options.decodeVideoTime, base: base),
            firstAudioDecode: relativeTimestamp(options.decodeAudioTime, base: base),
            firstVideoRender: relativeTimestamp(firstVideoRenderTime, base: base),
            firstAudioRender: relativeTimestamp(firstAudioRenderTime, base: base)
        )
    }

    private func relativeTimestamp(_ timestamp: TimeInterval, base: TimeInterval) -> TimeInterval? {
        guard timestamp > 0 else {
            return nil
        }
        guard base > 0, timestamp >= base else {
            return timestamp
        }
        return timestamp - base
    }

    private func refreshLowLatencyLiveDiagnosticAfterRender(now: TimeInterval, force: Bool) {
        guard options.lowLatencyLiveProfile != nil,
              let lastLowLatencyLoadingState,
              force || now - lastLowLatencyDiagnosticUpdateTime >= 0.25
        else {
            return
        }
        updateLowLatencyLiveDiagnostic(loadingState: lastLowLatencyLoadingState)
    }

    func codecDidFinished(track: some CapacityProtocol) {
        if track.mediaType == .audio {
            isAudioStalled = true
        }
        let allSatisfy = videoAudioTracks.allSatisfy { $0.isEndOfFile && $0.frameCount == 0 && $0.packetCount == 0 }
        if allSatisfy {
            delegate?.sourceDidFinished()
            timer.fireDate = Date.distantFuture
            if canRestartLoopPlayback {
                resetLoopClocksToStart()
                allPlayerItemTracks.forEach { $0.isLoopModel = false }
                if state == .finished {
                    seek(time: 0) { _ in }
                } else {
                    timer.fireDate = Date.distantPast
                }
            }
        }
    }

    private func adaptableVideo(loadingState: LoadingState) {
        if options.videoDisable || videoAdaptation == nil || loadingState.isEndOfFile || loadingState.isSeek || state == .seeking {
            return
        }
        guard let track = videoTrack else {
            return
        }
        videoAdaptation?.loadedCount = track.packetCount + track.frameCount
        videoAdaptation?.currentPlaybackTime = currentPlaybackTime
        videoAdaptation?.isPlayable = loadingState.isPlayable
        guard let (oldBitRate, newBitrate) = options.adaptable(state: videoAdaptation), oldBitRate != newBitrate,
              let newFFmpegAssetTrack = assetTracks.first(where: { $0.mediaType == .video && $0.bitRate == newBitrate })
        else {
            return
        }
        assetTracks.first { $0.mediaType == .video && $0.bitRate == oldBitRate }?.isEnabled = false
        newFFmpegAssetTrack.isEnabled = true
        findBestAudio(videoTrack: newFFmpegAssetTrack)
        memorySeekCache.invalidate()
        let bitRateState = VideoAdaptationState.BitRateState(bitRate: newBitrate, time: CACurrentMediaTime())
        videoAdaptation?.bitRateStates.append(bitRateState)
        delegate?.sourceDidChange(oldBitRate: oldBitRate, newBitrate: newBitrate)
    }

    private func findBestAudio(videoTrack: FFmpegAssetTrack) {
        let decodableAudios = FFmpegAssetTrack.decodableAudioTracks(assetTracks)
        guard videoAdaptation != nil, let first = decodableAudios.first(where: { $0.isEnabled }) else {
            return
        }
        let index = av_find_best_stream(formatCtx, AVMEDIA_TYPE_AUDIO, -1, videoTrack.trackID, nil, 0)
        guard index >= 0, let next = decodableAudios.first(where: { $0.trackID == index }) else {
            if index >= 0, let unsupportedAudio = assetTracks.first(where: { $0.mediaType == .audio && $0.trackID == index }) {
                KSLog("[audio] adaptive audio track skipped: \(unsupportedAudio.description)")
            }
            return
        }
        if next.trackID != first.trackID {
            first.isEnabled = false
            next.isEnabled = true
        }
    }

    private static func closeCustomIO(in formatContext: UnsafeMutablePointer<AVFormatContext>?) {
        guard let formatContext,
              (formatContext.pointee.flags & AVFMT_FLAG_CUSTOM_IO) != 0,
              let pb = formatContext.pointee.pb,
              let opaque = pb.pointee.opaque
        else {
            return
        }
        let value = Unmanaged<AbstractAVIOContext>.fromOpaque(opaque).takeRetainedValue()
        value.close()
        pb.pointee.opaque = nil
    }
}

extension MEPlayerItem: OutputRenderSourceDelegate {
    func mainClock() -> KSClock {
        isAudioStalled ? videoClock : audioClock
    }

    public func setVideo(time: CMTime, position: Int64) {
//        print("[video] video interval \(CACurrentMediaTime() - videoClock.lastMediaTime) video diff \(time.seconds - videoClock.time.seconds)")
        let now = CACurrentMediaTime()
        renderedVideoFrameCount += 1
        let isFirstVideoRender = firstVideoRenderTime == 0
        if isFirstVideoRender {
            firstVideoRenderTime = now
        }
        videoClock.time = time
        videoClock.position = position
        videoDisplayCount += 1
        let diff = videoClock.lastMediaTime - lastVideoDisplayTime
        if diff > 1 {
            dynamicInfo.displayFPS = Double(videoDisplayCount) / diff
            videoDisplayCount = 0
            lastVideoDisplayTime = videoClock.lastMediaTime
        }
        refreshLowLatencyLiveDiagnosticAfterRender(now: now, force: isFirstVideoRender)
    }

    public func setAudio(time: CMTime, position: Int64) {
//        print("[audio] setAudio: \(time.seconds)")
        let now = CACurrentMediaTime()
        audioRenderUpdateCount += 1
        let isFirstAudioRender = firstAudioRenderTime == 0
        if isFirstAudioRender {
            firstAudioRenderTime = now
        }
        // 切换到主线程的话，那播放起来会更顺滑
        runOnMainThread {
            self.audioClock.time = time
            self.audioClock.position = position
        }
        refreshLowLatencyLiveDiagnosticAfterRender(now: now, force: isFirstAudioRender)
    }

    public func getVideoOutputRender(force: Bool) -> VideoVTBFrame? {
        guard let videoTrack else {
            return nil
        }
        var type: ClockProcessType = force ? .next : .remain
        let predicate: ((VideoVTBFrame, Int) -> Bool)? = force ? nil : { [weak self] frame, count -> Bool in
            guard let self else { return true }
            (self.dynamicInfo.audioVideoSyncDiff, type) = self.options.videoClockSync(main: self.mainClock(), nextVideoTime: frame.seconds, fps: Double(frame.fps), frameCount: count)
            return type != .remain
        }
        let frame = videoTrack.getOutputRender(where: predicate)
        switch type {
        case .remain:
            break
        case .next:
            break
        case .dropNextFrame:
            if videoTrack.getOutputRender(where: nil) != nil {
                dynamicInfo.droppedVideoFrameCount += 1
            }
        case .flush:
            let count = videoTrack.outputRenderQueue.count
            videoTrack.outputRenderQueue.flush()
            dynamicInfo.droppedVideoFrameCount += UInt32(count)
        case .seek:
            videoTrack.outputRenderQueue.flush()
            videoTrack.seekTime = mainClock().time.seconds
        case .dropNextPacket:
            if let videoTrack = videoTrack as? AsyncPlayerItemTrack {
                let packet = videoTrack.packetQueue.pop { item, _ -> Bool in
                    !item.isKeyFrame
                }
                if packet != nil {
                    dynamicInfo.droppedVideoPacketCount += 1
                }
            }
        case .dropGOPPacket:
            if let videoTrack = videoTrack as? AsyncPlayerItemTrack {
                var packet: Packet? = nil
                repeat {
                    packet = videoTrack.packetQueue.pop { item, _ -> Bool in
                        !item.isKeyFrame
                    }
                    if packet != nil {
                        dynamicInfo.droppedVideoPacketCount += 1
                    }
                } while packet != nil
            }
        }
        return frame
    }

    public func getAudioOutputRender() -> AudioFrame? {
        if let frame = audioTrack?.getOutputRender(where: nil) {
            if options.isOfflineSubtitleGenerationEnabled {
                options.offlineSubtitleGenerator?.append(frame: frame)
            } else {
                SubtitleModel.audioRecognizes.first {
                    $0.isEnabled
                }?.append(frame: frame)
            }
            return frame
        } else {
            return nil
        }
    }
}

extension AbstractAVIOContext {
    func getContext() -> UnsafeMutablePointer<AVIOContext> {
        // 需要持有ioContext，不然会被释放掉,等到shutdown在清空
        avio_alloc_context(av_malloc(Int(bufferSize)), bufferSize, writable ? 1 : 0, Unmanaged.passRetained(self).toOpaque()) { opaque, buffer, size -> Int32 in
            let value = Unmanaged<AbstractAVIOContext>.fromOpaque(opaque!).takeUnretainedValue()
            let ret = value.read(buffer: buffer, size: size)
            return Int32(ret)
        } _: { opaque, buffer, size -> Int32 in
            let value = Unmanaged<AbstractAVIOContext>.fromOpaque(opaque!).takeUnretainedValue()
            let ret = value.write(buffer: buffer, size: size)
            return Int32(ret)
        } _: { opaque, offset, whence -> Int64 in
            let value = Unmanaged<AbstractAVIOContext>.fromOpaque(opaque!).takeUnretainedValue()
            if whence == AVSEEK_SIZE {
                return value.fileSize()
            }
            return value.seek(offset: offset, whence: whence)
        }
    }
}

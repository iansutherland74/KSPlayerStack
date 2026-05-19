import CoreMedia
import FFmpegKit
import Foundation
import Libavcodec
import Libavformat

public enum VideoExportError: Error, Equatable, LocalizedError {
    case unsupportedSource(URL)
    case unsupportedLiveOrPlaylist(URL)
    case unsupportedSeparateAudio(URL)
    case unsafeDestination(URL)
    case destinationExists(URL)
    case sourceAndDestinationMatch(URL)
    case noExportableStreams
    case ffmpegFailure(String)
    case downloadFailed(String)

    public var errorDescription: String? {
        switch self {
        case let .unsupportedSource(url):
            return "Video export supports local files and direct HTTP(S) media files, not \(url.absoluteString)."
        case let .unsupportedLiveOrPlaylist(url):
            return "Video export does not download live, playlist, or segmented media URLs: \(url.absoluteString)."
        case let .unsupportedSeparateAudio(url):
            return "Video export does not merge separate audio sources. Export the combined asset yourself or use a single muxed source instead of \(url.absoluteString)."
        case let .unsafeDestination(url):
            return "Video export destination must be a writable file URL: \(url.path)."
        case let .destinationExists(url):
            return "Video export destination already exists: \(url.path)."
        case let .sourceAndDestinationMatch(url):
            return "Video export source and destination must be different files: \(url.path)."
        case .noExportableStreams:
            return "No audio, video, or compatible subtitle streams can be copied to the destination container."
        case let .ffmpegFailure(message):
            return message
        case let .downloadFailed(message):
            return message
        }
    }
}

public enum VideoExportPhase: Equatable, Sendable {
    case validating
    case downloading
    case converting
    case finished
}

/// Progress emitted while a complete media file is downloaded and remuxed.
public struct VideoExportProgress: Equatable, Sendable {
    public let phase: VideoExportPhase
    public let fractionCompleted: Double?
    public let completedBytes: Int64?
    public let totalBytes: Int64?

    public init(phase: VideoExportPhase, fractionCompleted: Double? = nil, completedBytes: Int64? = nil, totalBytes: Int64? = nil) {
        self.phase = phase
        self.fractionCompleted = fractionCompleted
        self.completedBytes = completedBytes
        self.totalBytes = totalBytes
    }
}

/// Container presets for lossless stream-copy exports.
///
/// These presets do not transcode media. Unsupported stream/container combinations are rejected by
/// FFmpeg instead of silently changing codecs, color, HDR, or subtitle representation.
public enum VideoConversionPreset: Equatable, Sendable {
    case remuxMP4
    case remuxMOV
    case remuxMatroska

    public var fileExtension: String {
        switch self {
        case .remuxMP4:
            return "mp4"
        case .remuxMOV:
            return "mov"
        case .remuxMatroska:
            return "mkv"
        }
    }

    var isQuickTimeContainer: Bool {
        switch self {
        case .remuxMP4, .remuxMOV:
            return true
        case .remuxMatroska:
            return false
        }
    }

    public func ffmpegArguments(inputURL: URL, outputURL: URL, overwrite: Bool = true) -> [String] {
        var arguments = ["-hide_banner", overwrite ? "-y" : "-n", "-i", inputURL.ffmpegArgumentValue, "-map", "0", "-c", "copy"]
        if isQuickTimeContainer {
            arguments += ["-movflags", "+faststart"]
        }
        arguments.append(outputURL.ffmpegArgumentValue)
        return arguments
    }
}

public struct VideoExportJob: Equatable, Sendable {
    public let sourceURL: URL
    public let destinationURL: URL
    public let preset: VideoConversionPreset
    public let overwriteExisting: Bool

    /// Creates a complete-file export job.
    ///
    /// `destinationURL` must be a file URL. Existing files are left untouched unless
    /// `overwriteExisting` is true; output is first written to a sibling temporary file and then moved
    /// into place.
    public init(sourceURL: URL, destinationURL: URL, preset: VideoConversionPreset = .remuxMP4, overwriteExisting: Bool = false) {
        self.sourceURL = sourceURL
        self.destinationURL = destinationURL
        self.preset = preset
        self.overwriteExisting = overwriteExisting
    }

    public static func defaultDestination(sourceURL: URL, directory: URL? = nil, preset: VideoConversionPreset = .remuxMP4) -> URL {
        let directory = directory
            ?? FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
            ?? FileManager.default.temporaryDirectory
        let baseName = VideoExportPathPolicy.safeBaseName(for: sourceURL)
        return directory.appendingPathComponent(baseName).appendingPathExtension(preset.fileExtension)
    }
}

public final class VideoExportService: @unchecked Sendable {
    public typealias ProgressHandler = @Sendable (VideoExportProgress) -> Void

    public init() {}

    /// Exports a complete finite media file by downloading direct HTTP(S) media when needed and then
    /// stream-copying supported tracks into the selected container.
    ///
    /// The export path rejects playlist/live/segmented sources, Blu-ray inputs, and unsupported URL
    /// schemes. Custom `KSOptions.process(url:)` IO is allowed for non-live custom schemes. Remux
    /// presets preserve source streams and metadata where the destination container accepts them; they
    /// do not perform codec conversion.
    @discardableResult
    public func export(_ job: VideoExportJob, options: KSOptions = KSOptions(), progress: ProgressHandler? = nil) async throws -> URL {
        progress?(VideoExportProgress(phase: .validating, fractionCompleted: 0))
        try VideoExportPathPolicy.validate(job)

        let source = try await preparedSource(for: job.sourceURL, options: options, progress: progress)
        let exportOptions = VideoExportRemuxOptions(formatContextOptions: options.formatContextOptions)
        let task = Task.detached(priority: .utility) {
            try Self.remux(source: source, destinationURL: job.destinationURL, preset: job.preset, overwriteExisting: job.overwriteExisting, options: exportOptions, progress: progress)
        }

        do {
            try await withTaskCancellationHandler {
                try await task.value
            } onCancel: {
                task.cancel()
            }
        } catch {
            source.cleanup()
            throw error
        }

        source.cleanup()
        progress?(VideoExportProgress(phase: .finished, fractionCompleted: 1))
        return job.destinationURL
    }

    private func preparedSource(for url: URL, options: KSOptions, progress: ProgressHandler?) async throws -> VideoExportSource {
        if VideoExportSourcePolicy.isLiveOrSegmentedURL(url) {
            throw VideoExportError.unsupportedLiveOrPlaylist(url)
        }
        if KSBluRayURLResolver.isBluRayCandidate(url) {
            throw VideoExportError.unsupportedSource(url)
        }
        if url.isFileURL {
            options.prepareFormatContextOptions(for: url)
            return VideoExportSource(url: url, customIO: options.process(url: url))
        }
        if VideoExportSourcePolicy.isDirectHTTPMediaURL(url) {
            if let cachedURL = KSDiskPrecache.cachedURL(for: url, options: options) {
                return VideoExportSource(url: cachedURL)
            }

            let directory = FileManager.default.temporaryDirectory.appendingPathComponent("KSPlayerVideoExports", isDirectory: true)
            let localURL = directory
                .appendingPathComponent(UUID().uuidString)
                .appendingPathExtension(url.pathExtension.isEmpty ? "media" : url.pathExtension)
            var request = URLRequest(url: url, cachePolicy: .reloadIgnoringLocalCacheData)
            Self.applyHeaders(from: options, to: &request)

            let downloadedURL = try await VideoExportDownloader(progress: progress).download(request: request, destination: localURL)
            return VideoExportSource(url: downloadedURL, temporaryURL: downloadedURL)
        }

        options.prepareFormatContextOptions(for: url)
        if let customIO = options.process(url: url) {
            return VideoExportSource(url: url, customIO: customIO)
        }

        throw VideoExportError.unsupportedSource(url)
    }

    private static func applyHeaders(from options: KSOptions, to request: inout URLRequest) {
        if let headers = options.avOptions["AVURLAssetHTTPHeaderFieldsKey"] as? [String: String] {
            for (field, value) in headers {
                request.setValue(value, forHTTPHeaderField: field)
            }
        }
        if request.value(forHTTPHeaderField: "User-Agent") == nil, let userAgent = options.userAgent {
            request.setValue(userAgent, forHTTPHeaderField: "User-Agent")
        }
        if request.value(forHTTPHeaderField: "Referer") == nil, let referer = options.referer {
            request.setValue(referer, forHTTPHeaderField: "Referer")
        }
    }

    private static func remux(source: VideoExportSource, destinationURL: URL, preset: VideoConversionPreset, overwriteExisting: Bool, options: VideoExportRemuxOptions, progress: ProgressHandler?) throws {
        let fileManager = FileManager.default
        let destinationDirectory = destinationURL.deletingLastPathComponent()
        let temporaryDestinationURL = destinationDirectory
            .appendingPathComponent(".\(destinationURL.deletingPathExtension().lastPathComponent).\(UUID().uuidString)")
            .appendingPathExtension(preset.fileExtension)

        do {
            try fileManager.createDirectory(at: destinationDirectory, withIntermediateDirectories: true)
            if fileManager.fileExists(atPath: temporaryDestinationURL.path) {
                try fileManager.removeItem(at: temporaryDestinationURL)
            }
        } catch {
            throw VideoExportError.unsafeDestination(destinationURL)
        }

        let sourceAccess = KSSecurityScopedURLAccess(urls: source.securityScopedURLs)
        let destinationAccess = KSSecurityScopedURLAccess(url: destinationDirectory)
        defer {
            sourceAccess.stop()
            destinationAccess.stop()
            try? fileManager.removeItem(at: temporaryDestinationURL)
        }

        var inputContext = avformat_alloc_context()
        defer {
            if let inputContext, inputContext.pointee.flags & AVFMT_FLAG_CUSTOM_IO != 0,
               let pb = inputContext.pointee.pb,
               let opaque = pb.pointee.opaque
            {
                let customIO = Unmanaged<AbstractAVIOContext>.fromOpaque(opaque).takeRetainedValue()
                customIO.close()
                pb.pointee.opaque = nil
            }
            avformat_close_input(&inputContext)
        }
        guard inputContext != nil else {
            throw NSError(errorCode: .formatCreate)
        }
        inputContext?.pointee.interrupt_callback.callback = { _ in
            Task.isCancelled ? 1 : 0
        }
        if let customIO = source.customIO {
            inputContext?.pointee.pb = customIO.getContext()
            inputContext?.pointee.flags |= AVFMT_FLAG_CUSTOM_IO
        }

        setHttpProxy()
        var inputOptions = options.formatContextOptions.avOptions
        var result = avformat_open_input(&inputContext, source.ffmpegURLString, nil, &inputOptions)
        av_dict_free(&inputOptions)
        guard result == 0, let inputContext else {
            throw NSError(errorCode: .formatOpenInput, avErrorCode: result)
        }

        result = avformat_find_stream_info(inputContext, nil)
        guard result == 0 else {
            throw NSError(errorCode: .formatFindStreamInfo, avErrorCode: result)
        }
        let inputDuration = TimeInterval(max(inputContext.pointee.duration, 0)) / TimeInterval(AV_TIME_BASE)
        guard inputDuration > 0 else {
            throw VideoExportError.unsupportedLiveOrPlaylist(source.url)
        }

        var outputContext: UnsafeMutablePointer<AVFormatContext>?
        let destination = temporaryDestinationURL.path
        result = avformat_alloc_output_context2(&outputContext, nil, nil, destination)
        guard result >= 0, let outputContext else {
            throw NSError(errorCode: .formatOutputCreate, avErrorCode: result)
        }
        defer {
            if outputContext.pointee.oformat.pointee.flags & AVFMT_NOFILE == 0 {
                avio_closep(&outputContext.pointee.pb)
            }
            avformat_free_context(outputContext)
        }

        let streamMapping = try makeOutputStreams(inputContext: inputContext, outputContext: outputContext, preset: preset)
        guard !streamMapping.isEmpty else {
            throw VideoExportError.noExportableStreams
        }
        av_dict_copy(&outputContext.pointee.metadata, inputContext.pointee.metadata, 0)

        if outputContext.pointee.oformat.pointee.flags & AVFMT_NOFILE == 0 {
            result = avio_open(&outputContext.pointee.pb, destination, AVIO_FLAG_WRITE)
            guard result >= 0 else {
                throw VideoExportError.ffmpegFailure(String(avErrorCode: result))
            }
        }

        result = avformat_write_header(outputContext, nil)
        guard result >= 0 else {
            throw NSError(errorCode: .formatWriteHeader, avErrorCode: result)
        }

        var packet = AVPacket()
        defer {
            av_packet_unref(&packet)
        }

        while !Task.isCancelled {
            result = av_read_frame(inputContext, &packet)
            if result == AVError.eof.code {
                break
            }
            guard result >= 0 else {
                throw NSError(errorCode: .readFrame, avErrorCode: result)
            }
            defer {
                av_packet_unref(&packet)
            }
            let inputIndex = Int(packet.stream_index)
            guard let outputIndex = streamMapping[inputIndex],
                  let inputStream = inputContext.pointee.streams[inputIndex],
                  let outputStream = outputContext.pointee.streams[outputIndex]
            else {
                continue
            }

            let packetTimestamp = packet.pts == swift_AV_NOPTS_VALUE ? packet.dts : packet.pts
            if packetTimestamp != swift_AV_NOPTS_VALUE {
                let packetTime = av_rescale_q(packetTimestamp, inputStream.pointee.time_base, AVRational(num: 1, den: AV_TIME_BASE))
                let seconds = Double(packetTime) / Double(AV_TIME_BASE)
                progress?(VideoExportProgress(phase: .converting, fractionCompleted: VideoExportProgressPolicy.clampedFraction(completed: seconds, duration: inputDuration)))
            }

            av_packet_rescale_ts(&packet, inputStream.pointee.time_base, outputStream.pointee.time_base)
            packet.stream_index = Int32(outputIndex)
            packet.pos = -1

            result = av_interleaved_write_frame(outputContext, &packet)
            guard result >= 0 else {
                throw VideoExportError.ffmpegFailure(String(avErrorCode: result))
            }
        }
        try Task.checkCancellation()

        result = av_write_trailer(outputContext)
        guard result >= 0 else {
            throw VideoExportError.ffmpegFailure(String(avErrorCode: result))
        }

        do {
            if fileManager.fileExists(atPath: destinationURL.path) {
                guard overwriteExisting else {
                    throw VideoExportError.destinationExists(destinationURL)
                }
                try fileManager.removeItem(at: destinationURL)
            }
            try fileManager.moveItem(at: temporaryDestinationURL, to: destinationURL)
        } catch let error as VideoExportError {
            throw error
        } catch {
            throw VideoExportError.unsafeDestination(destinationURL)
        }
    }

    private static func makeOutputStreams(inputContext: UnsafeMutablePointer<AVFormatContext>, outputContext: UnsafeMutablePointer<AVFormatContext>, preset: VideoConversionPreset) throws -> [Int: Int] {
        var streamMapping = [Int: Int]()
        for inputIndex in 0 ..< Int(inputContext.pointee.nb_streams) {
            guard let inputStream = inputContext.pointee.streams[inputIndex] else {
                continue
            }
            let codecType = inputStream.pointee.codecpar.pointee.codec_type
            guard codecType == AVMEDIA_TYPE_VIDEO || codecType == AVMEDIA_TYPE_AUDIO || codecType == AVMEDIA_TYPE_SUBTITLE else {
                continue
            }
            if codecType == AVMEDIA_TYPE_SUBTITLE, preset.isQuickTimeContainer,
               inputStream.pointee.codecpar.pointee.codec_id != AV_CODEC_ID_MOV_TEXT
            {
                continue
            }
            guard let outputStream = avformat_new_stream(outputContext, nil) else {
                throw VideoExportError.noExportableStreams
            }
            let result = avcodec_parameters_copy(outputStream.pointee.codecpar, inputStream.pointee.codecpar)
            guard result >= 0 else {
                throw VideoExportError.ffmpegFailure(String(avErrorCode: result))
            }
            if inputStream.pointee.codecpar.pointee.codec_id == AV_CODEC_ID_HEVC, preset.isQuickTimeContainer {
                outputStream.pointee.codecpar.pointee.codec_tag = CMFormatDescription.MediaSubType.hevc.rawValue.bigEndian
            } else {
                outputStream.pointee.codecpar.pointee.codec_tag = 0
            }
            outputStream.pointee.time_base = inputStream.pointee.time_base
            outputStream.pointee.avg_frame_rate = inputStream.pointee.avg_frame_rate
            outputStream.pointee.sample_aspect_ratio = inputStream.pointee.sample_aspect_ratio
            av_dict_copy(&outputStream.pointee.metadata, inputStream.pointee.metadata, 0)
            streamMapping[inputIndex] = Int(outputStream.pointee.index)
        }
        return streamMapping
    }
}

public extension KSPlayerLayer {
    /// Exports the currently loaded video source as a complete file using FFmpeg stream copy.
    ///
    /// This is a whole-source export, not a current-playback-window recorder. Use `recordClip` for a
    /// finite seekable time range. Separate `audioURL` playback is rejected because this remux path
    /// does not merge independent audio and video assets.
    @discardableResult
    func exportVideo(destination: URL? = nil, preset: VideoConversionPreset = .remuxMP4, overwriteExisting: Bool = false, progress: VideoExportService.ProgressHandler? = nil) async throws -> URL {
        if let audioURL {
            throw VideoExportError.unsupportedSeparateAudio(audioURL)
        }
        let destination = destination ?? VideoExportJob.defaultDestination(sourceURL: url, preset: preset)
        let job = VideoExportJob(sourceURL: url, destinationURL: destination, preset: preset, overwriteExisting: overwriteExisting)
        return try await VideoExportService().export(job, options: options, progress: progress)
    }
}

private struct VideoExportRemuxOptions: @unchecked Sendable {
    let formatContextOptions: [String: Any]
}

private struct VideoExportSource: @unchecked Sendable {
    let url: URL
    let ffmpegURLString: String
    let securityScopedURLs: [URL]
    let customIO: AbstractAVIOContext?
    let temporaryURL: URL?

    init(url: URL, customIO: AbstractAVIOContext? = nil, temporaryURL: URL? = nil) {
        self.url = url
        ffmpegURLString = url.isFileURL ? url.path : url.absoluteString
        securityScopedURLs = url.isFileURL ? [url] : []
        self.customIO = customIO
        self.temporaryURL = temporaryURL
    }

    func cleanup() {
        if let temporaryURL {
            try? FileManager.default.removeItem(at: temporaryURL)
        }
    }
}

enum VideoExportSourcePolicy {
    private static let directHTTPSchemes: Set<String> = ["http", "https"]
    private static let liveOrPlaylistExtensions: Set<String> = ["m3u", "m3u8", "mpd", "ism", "isml"]
    private static let liveSchemes: Set<String> = ["rtmp", "rtmps", "rtp", "rtsp", "udp"]

    static func isDirectHTTPMediaURL(_ url: URL) -> Bool {
        guard let scheme = url.scheme?.lowercased(), directHTTPSchemes.contains(scheme) else {
            return false
        }
        return !isLiveOrSegmentedURL(url)
    }

    static func isLiveOrSegmentedURL(_ url: URL) -> Bool {
        if let scheme = url.scheme?.lowercased(), liveSchemes.contains(scheme) {
            return true
        }
        guard let scheme = url.scheme?.lowercased(), directHTTPSchemes.contains(scheme) else {
            return false
        }
        return liveOrPlaylistExtensions.contains(url.pathExtension.lowercased())
    }
}

enum VideoExportProgressPolicy {
    static func clampedFraction(completed: TimeInterval, duration: TimeInterval) -> Double? {
        guard completed.isFinite, duration.isFinite, duration > 0 else {
            return nil
        }
        return min(max(completed / duration, 0), 1)
    }
}

enum VideoExportPathPolicy {
    static func validate(_ job: VideoExportJob) throws {
        guard job.destinationURL.isFileURL,
              !job.destinationURL.path.isEmpty,
              !job.destinationURL.lastPathComponent.isEmpty
        else {
            throw VideoExportError.unsafeDestination(job.destinationURL)
        }

        let destination = job.destinationURL.standardizedFileURL
        if job.sourceURL.isFileURL, job.sourceURL.standardizedFileURL == destination {
            throw VideoExportError.sourceAndDestinationMatch(job.destinationURL)
        }

        var isDirectory: ObjCBool = false
        if FileManager.default.fileExists(atPath: destination.path, isDirectory: &isDirectory) {
            if isDirectory.boolValue {
                throw VideoExportError.unsafeDestination(job.destinationURL)
            }
            if !job.overwriteExisting {
                throw VideoExportError.destinationExists(job.destinationURL)
            }
        }
    }

    static func safeBaseName(for sourceURL: URL) -> String {
        let candidate = sourceURL.deletingPathExtension().lastPathComponent
        let trimmed = candidate.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !trimmed.isEmpty else {
            return "video-export"
        }
        let allowed = CharacterSet.alphanumerics.union(CharacterSet(charactersIn: "-_ "))
        let scalars = trimmed.unicodeScalars.map { allowed.contains($0) ? Character($0) : "-" }
        let name = String(scalars).trimmingCharacters(in: CharacterSet(charactersIn: "-_ "))
        return name.isEmpty ? "video-export" : name
    }
}

private final class VideoExportDownloader: NSObject, URLSessionDownloadDelegate, @unchecked Sendable {
    private let lock = NSLock()
    private let progress: VideoExportService.ProgressHandler?
    private var continuation: CheckedContinuation<URL, Error>?
    private var session: URLSession?
    private var task: URLSessionDownloadTask?
    private var destination: URL?
    private var didResume = false

    init(progress: VideoExportService.ProgressHandler?) {
        self.progress = progress
    }

    func download(request: URLRequest, destination: URL) async throws -> URL {
        try await withTaskCancellationHandler {
            try await withCheckedThrowingContinuation { continuation in
                lock.withLock {
                    self.continuation = continuation
                    self.destination = destination
                    let configuration = URLSessionConfiguration.default
                    configuration.allowsConstrainedNetworkAccess = true
                    configuration.allowsExpensiveNetworkAccess = true
                    let session = URLSession(configuration: configuration, delegate: self, delegateQueue: nil)
                    self.session = session
                    let task = session.downloadTask(with: request)
                    self.task = task
                    task.resume()
                }
            }
        } onCancel: {
            cancel()
        }
    }

    func cancel() {
        lock.withLock {
            task?.cancel()
            session?.invalidateAndCancel()
        }
    }

    func urlSession(_: URLSession, downloadTask _: URLSessionDownloadTask, didWriteData _: Int64, totalBytesWritten: Int64, totalBytesExpectedToWrite: Int64) {
        let totalBytes = totalBytesExpectedToWrite > 0 ? totalBytesExpectedToWrite : nil
        let fraction = totalBytes.map { min(max(Double(totalBytesWritten) / Double($0), 0), 1) }
        progress?(VideoExportProgress(phase: .downloading, fractionCompleted: fraction, completedBytes: totalBytesWritten, totalBytes: totalBytes))
    }

    func urlSession(_: URLSession, downloadTask: URLSessionDownloadTask, didFinishDownloadingTo location: URL) {
        do {
            guard let destination else {
                throw VideoExportError.downloadFailed("Download finished without a destination URL.")
            }
            try Self.validateDownloadResponse(downloadTask.response)
            let fileManager = FileManager.default
            try fileManager.createDirectory(at: destination.deletingLastPathComponent(), withIntermediateDirectories: true)
            if fileManager.fileExists(atPath: destination.path) {
                try fileManager.removeItem(at: destination)
            }
            try fileManager.moveItem(at: location, to: destination)
            guard Self.downloadedFileSize(at: destination) ?? 0 > 0 else {
                try? fileManager.removeItem(at: destination)
                throw VideoExportError.downloadFailed("Download finished without media data.")
            }
            resume(.success(destination))
        } catch {
            if let destination {
                try? FileManager.default.removeItem(at: destination)
            }
            resume(.failure(error))
        }
    }

    func urlSession(_: URLSession, task _: URLSessionTask, didCompleteWithError error: Error?) {
        if let error {
            resume(.failure(VideoExportError.downloadFailed(error.localizedDescription)))
        }
        lock.withLock {
            session?.finishTasksAndInvalidate()
            session = nil
            self.task = nil
        }
    }

    private func resume(_ result: Result<URL, Error>) {
        let continuation: CheckedContinuation<URL, Error>? = lock.withLock {
            guard !didResume else {
                return nil
            }
            didResume = true
            let continuation = self.continuation
            self.continuation = nil
            return continuation
        }

        switch result {
        case let .success(url):
            continuation?.resume(returning: url)
        case let .failure(error):
            continuation?.resume(throwing: error)
        }
    }

    private static func validateDownloadResponse(_ response: URLResponse?) throws {
        guard let response = response as? HTTPURLResponse else {
            return
        }
        guard (200 ..< 300).contains(response.statusCode) else {
            throw VideoExportError.downloadFailed("Download failed with HTTP status \(response.statusCode).")
        }
    }

    private static func downloadedFileSize(at url: URL) -> Int64? {
        guard let values = try? url.resourceValues(forKeys: [.fileSizeKey, .isRegularFileKey]),
              values.isRegularFile == true,
              let size = values.fileSize
        else {
            return nil
        }
        return Int64(size)
    }
}

private extension URL {
    var ffmpegArgumentValue: String {
        isFileURL ? path : absoluteString
    }
}

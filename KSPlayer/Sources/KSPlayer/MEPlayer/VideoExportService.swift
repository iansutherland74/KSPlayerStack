import CoreMedia
import FFmpegKit
import Foundation
import Libavcodec
import Libavformat

public enum VideoExportError: Error, Equatable, LocalizedError {
    case unsupportedSource(URL)
    case unsupportedLiveOrPlaylist(URL)
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

    @discardableResult
    public func export(_ job: VideoExportJob, options: KSOptions = KSOptions(), progress: ProgressHandler? = nil) async throws -> URL {
        progress?(VideoExportProgress(phase: .validating, fractionCompleted: 0))
        try VideoExportPathPolicy.validate(job)

        let sourceURL = try await preparedLocalSource(for: job.sourceURL, options: options, progress: progress)
        let exportOptions = VideoExportRemuxOptions(formatContextOptions: options.formatContextOptions)

        do {
            try await Task.detached(priority: .utility) {
                try Self.remux(sourceURL: sourceURL, destinationURL: job.destinationURL, preset: job.preset, overwriteExisting: job.overwriteExisting, options: exportOptions, progress: progress)
            }.value
        } catch {
            if sourceURL != job.sourceURL {
                try? FileManager.default.removeItem(at: sourceURL)
            }
            throw error
        }

        if sourceURL != job.sourceURL {
            try? FileManager.default.removeItem(at: sourceURL)
        }
        progress?(VideoExportProgress(phase: .finished, fractionCompleted: 1))
        return job.destinationURL
    }

    private func preparedLocalSource(for url: URL, options: KSOptions, progress: ProgressHandler?) async throws -> URL {
        if url.isFileURL {
            return url
        }
        guard Self.isDirectHTTPMediaURL(url) else {
            if Self.isHTTPPlaylistURL(url) {
                throw VideoExportError.unsupportedLiveOrPlaylist(url)
            }
            throw VideoExportError.unsupportedSource(url)
        }
        if let cachedURL = KSDiskPrecache.cachedURL(for: url, options: options) {
            return cachedURL
        }

        let directory = FileManager.default.temporaryDirectory.appendingPathComponent("KSPlayerVideoExports", isDirectory: true)
        let localURL = directory
            .appendingPathComponent(UUID().uuidString)
            .appendingPathExtension(url.pathExtension.isEmpty ? "media" : url.pathExtension)
        var request = URLRequest(url: url, cachePolicy: .reloadIgnoringLocalCacheData)
        Self.applyHeaders(from: options, to: &request)

        return try await VideoExportDownloader(progress: progress).download(request: request, destination: localURL)
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

    private static func isDirectHTTPMediaURL(_ url: URL) -> Bool {
        guard let scheme = url.scheme?.lowercased(), scheme == "http" || scheme == "https" else {
            return false
        }
        return !isHTTPPlaylistURL(url)
    }

    private static func isHTTPPlaylistURL(_ url: URL) -> Bool {
        guard let scheme = url.scheme?.lowercased(), scheme == "http" || scheme == "https" else {
            return false
        }
        let playlistExtensions: Set<String> = ["m3u", "m3u8", "mpd", "ism", "isml"]
        return playlistExtensions.contains(url.pathExtension.lowercased())
    }

    private static func remux(sourceURL: URL, destinationURL: URL, preset: VideoConversionPreset, overwriteExisting: Bool, options: VideoExportRemuxOptions, progress: ProgressHandler?) throws {
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

        let sourceAccess = KSSecurityScopedURLAccess(url: sourceURL)
        let destinationAccess = KSSecurityScopedURLAccess(url: destinationDirectory)
        defer {
            sourceAccess.stop()
            destinationAccess.stop()
            try? fileManager.removeItem(at: temporaryDestinationURL)
        }

        var inputContext = avformat_alloc_context()
        defer {
            avformat_close_input(&inputContext)
        }
        guard inputContext != nil else {
            throw NSError(errorCode: .formatCreate)
        }

        setHttpProxy()
        var inputOptions = options.formatContextOptions.avOptions
        let source = sourceURL.ffmpegArgumentValue
        var result = avformat_open_input(&inputContext, source, nil, &inputOptions)
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
            throw VideoExportError.unsupportedLiveOrPlaylist(sourceURL)
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
                let fraction = min(max(Double(packetTime) / Double(AV_TIME_BASE) / inputDuration, 0), 1)
                progress?(VideoExportProgress(phase: .converting, fractionCompleted: fraction))
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
            streamMapping[inputIndex] = Int(outputStream.pointee.index)
        }
        return streamMapping
    }
}

public extension KSPlayerLayer {
    @discardableResult
    func exportVideo(destination: URL? = nil, preset: VideoConversionPreset = .remuxMP4, overwriteExisting: Bool = false, progress: VideoExportService.ProgressHandler? = nil) async throws -> URL {
        let destination = destination ?? VideoExportJob.defaultDestination(sourceURL: url, preset: preset)
        let job = VideoExportJob(sourceURL: url, destinationURL: destination, preset: preset, overwriteExisting: overwriteExisting)
        return try await VideoExportService().export(job, options: options, progress: progress)
    }
}

private struct VideoExportRemuxOptions: @unchecked Sendable {
    let formatContextOptions: [String: Any]
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

    func urlSession(_: URLSession, downloadTask _: URLSessionDownloadTask, didFinishDownloadingTo location: URL) {
        do {
            guard let destination else {
                throw VideoExportError.downloadFailed("Download finished without a destination URL.")
            }
            let fileManager = FileManager.default
            try fileManager.createDirectory(at: destination.deletingLastPathComponent(), withIntermediateDirectories: true)
            if fileManager.fileExists(atPath: destination.path) {
                try fileManager.removeItem(at: destination)
            }
            try fileManager.moveItem(at: location, to: destination)
            resume(.success(destination))
        } catch {
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
}

private extension URL {
    var ffmpegArgumentValue: String {
        isFileURL ? path : absoluteString
    }
}

import AVFoundation
import CoreMedia
import FFmpegKit
import Foundation
import Libavcodec
import Libavformat

public enum KSPlayerClipExportError: Error, Equatable, LocalizedError {
    case invalidTimeRange
    case unsupportedNonSeekableSource
    case noWritableDestination
    case destinationExists(URL)
    case sourceAndDestinationMatch(URL)
    case noExportableStreams
    case ffmpegFailure(String)

    public var errorDescription: String? {
        switch self {
        case .invalidTimeRange:
            return "Clip start and end times must be finite, and end must be greater than start."
        case .unsupportedNonSeekableSource:
            return "Clip export requires a finite, seekable media range."
        case .noWritableDestination:
            return "Clip export destination is not writable."
        case let .destinationExists(url):
            return "Clip export destination already exists: \(url.path)."
        case let .sourceAndDestinationMatch(url):
            return "Clip export source and destination must be different files: \(url.path)."
        case .noExportableStreams:
            return "No audio, video, or subtitle streams can be copied to the destination container."
        case let .ffmpegFailure(message):
            return message
        }
    }
}

public enum KSPlayerClipExportPhase: Equatable, Sendable {
    case validating
    case exporting
    case finished
}

public struct KSPlayerClipExportProgress: Equatable, Sendable {
    public let phase: KSPlayerClipExportPhase
    public let fractionCompleted: Double?

    public init(phase: KSPlayerClipExportPhase, fractionCompleted: Double? = nil) {
        self.phase = phase
        self.fractionCompleted = fractionCompleted
    }
}

public struct KSPlayerClipExportRequest: Equatable, Sendable {
    public let start: TimeInterval
    public let end: TimeInterval
    public let destination: URL
    public let overwriteExisting: Bool

    public var duration: TimeInterval {
        end - start
    }

    public init(start: TimeInterval, end: TimeInterval, destination: URL, overwriteExisting: Bool = false) throws {
        guard start.isFinite, end.isFinite else {
            throw KSPlayerClipExportError.invalidTimeRange
        }
        let clampedStart = max(start, 0)
        guard end > clampedStart else {
            throw KSPlayerClipExportError.invalidTimeRange
        }
        self.start = clampedStart
        self.end = end
        self.destination = destination
        self.overwriteExisting = overwriteExisting
    }

    static func validated(start: TimeInterval, end: TimeInterval, destination: URL, overwriteExisting: Bool = false, availableRange: MediaPlaybackTimeRange?) throws -> Self {
        guard let availableRange else {
            throw KSPlayerClipExportError.unsupportedNonSeekableSource
        }
        let clampedStart = availableRange.clamped(start)
        let clampedEnd = availableRange.clamped(end)
        return try Self(start: clampedStart, end: clampedEnd, destination: destination, overwriteExisting: overwriteExisting)
    }
}

struct KSPlayerClipDestination {
    static func makeDefault(sourceURL: URL, directory: URL? = nil, start: TimeInterval, end: TimeInterval, fileExtension: String = "mp4") -> URL {
        let directory = directory
            ?? FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
            ?? FileManager.default.temporaryDirectory
        let basename = safeBaseName(for: sourceURL)
        let startText = timeComponent(start)
        let endText = timeComponent(end)
        return directory.appendingPathComponent("\(basename)-\(startText)-\(endText)").appendingPathExtension(fileExtension)
    }

    private static func safeBaseName(for sourceURL: URL) -> String {
        let candidate = sourceURL.deletingPathExtension().lastPathComponent.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !candidate.isEmpty else {
            return "clip"
        }
        let allowed = CharacterSet.alphanumerics.union(CharacterSet(charactersIn: "-_ "))
        let scalars = candidate.unicodeScalars.map { allowed.contains($0) ? Character($0) : "-" }
        let name = String(scalars).trimmingCharacters(in: CharacterSet(charactersIn: "-_ "))
        return name.isEmpty ? "clip" : name
    }

    private static func timeComponent(_ time: TimeInterval) -> String {
        let centiseconds = max(Int((time * 100).rounded()), 0)
        let seconds = centiseconds / 100
        let fraction = centiseconds % 100
        return String(format: "%02d-%02d-%02d-%02d", seconds / 3600, seconds / 60 % 60, seconds % 60, fraction)
    }
}

private struct KSPlayerFFmpegClipExportOptions: @unchecked Sendable {
    let formatContextOptions: [String: Any]
}

public final class KSPlayerClipExporter: @unchecked Sendable {
    public typealias ProgressHandler = @Sendable (KSPlayerClipExportProgress) -> Void

    public init() {}

    /// Exports a finite, seekable clip by stream-copying supported tracks into `destination`.
    ///
    /// Local files, direct FFmpeg-readable remote URLs, FFmpeg-only schemes configured in `KSOptions`,
    /// Blu-ray file inputs, and custom `KSOptions.process(url:)` IO are prepared with the same URL
    /// handling used by `KSMEPlayer`. Live/non-seekable inputs are rejected unless callers provide a
    /// finite seekable range.
    @discardableResult
    public func export(
        sourceURL: URL,
        start: TimeInterval,
        end: TimeInterval,
        destination: URL,
        overwriteExisting: Bool = false,
        options: KSOptions = KSOptions(),
        availableRange: MediaPlaybackTimeRange? = nil,
        progress: ProgressHandler? = nil
    ) async throws -> URL {
        progress?(KSPlayerClipExportProgress(phase: .validating, fractionCompleted: 0))
        let playbackURL = KSDiskPrecache.playbackURL(for: sourceURL, options: options)
        let request: KSPlayerClipExportRequest
        if let availableRange {
            request = try KSPlayerClipExportRequest.validated(start: start, end: end, destination: destination, overwriteExisting: overwriteExisting, availableRange: availableRange)
        } else {
            request = try KSPlayerClipExportRequest(start: start, end: end, destination: destination, overwriteExisting: overwriteExisting)
        }
        try KSPlayerClipPathPolicy.validate(sourceURL: playbackURL, request: request)
        let clipSource = KSPlayerClipSource(sourceURL: playbackURL, options: options)
        let exportOptions = KSPlayerFFmpegClipExportOptions(formatContextOptions: options.formatContextOptions)

        let outputURL = try await Task.detached(priority: .utility) {
            try Self.remux(source: clipSource, request: request, options: exportOptions, progress: progress)
        }.value
        progress?(KSPlayerClipExportProgress(phase: .finished, fractionCompleted: 1))
        return outputURL
    }

    private static func remux(source: KSPlayerClipSource, request: KSPlayerClipExportRequest, options: KSPlayerFFmpegClipExportOptions, progress: ProgressHandler?) throws -> URL {
        let fileManager = FileManager.default
        let destinationDirectory = request.destination.deletingLastPathComponent()
        let temporaryDestination = destinationDirectory
            .appendingPathComponent(".\(request.destination.deletingPathExtension().lastPathComponent).\(UUID().uuidString)")
            .appendingPathExtension(request.destination.pathExtension.isEmpty ? "mp4" : request.destination.pathExtension)
        do {
            try fileManager.createDirectory(at: destinationDirectory, withIntermediateDirectories: true)
            if fileManager.fileExists(atPath: temporaryDestination.path) {
                try fileManager.removeItem(at: temporaryDestination)
            }
        } catch {
            throw KSPlayerClipExportError.noWritableDestination
        }

        let sourceAccess = KSSecurityScopedURLAccess(urls: source.securityScopedURLs)
        let destinationAccess = KSSecurityScopedURLAccess(url: destinationDirectory)
        defer {
            sourceAccess.stop()
            destinationAccess.stop()
            try? fileManager.removeItem(at: temporaryDestination)
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
        guard inputDuration > 0, request.start < inputDuration else {
            throw KSPlayerClipExportError.unsupportedNonSeekableSource
        }
        let effectiveEnd = min(request.end, inputDuration)
        guard effectiveEnd > request.start else {
            throw KSPlayerClipExportError.invalidTimeRange
        }
        let effectiveDuration = effectiveEnd - request.start

        var outputContext: UnsafeMutablePointer<AVFormatContext>?
        let destination = temporaryDestination.path
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

        let streamMapping = try makeOutputStreams(inputContext: inputContext, outputContext: outputContext)
        guard !streamMapping.isEmpty else {
            throw KSPlayerClipExportError.noExportableStreams
        }

        if outputContext.pointee.oformat.pointee.flags & AVFMT_NOFILE == 0 {
            result = avio_open(&outputContext.pointee.pb, destination, AVIO_FLAG_WRITE)
            guard result >= 0 else {
                throw KSPlayerClipExportError.ffmpegFailure(String(avErrorCode: result))
            }
        }

        result = avformat_write_header(outputContext, nil)
        guard result >= 0 else {
            throw NSError(errorCode: .formatWriteHeader, avErrorCode: result)
        }

        let formatStartTime = inputContext.pointee.start_time == swift_AV_NOPTS_VALUE ? 0 : inputContext.pointee.start_time
        let startTimestamp = formatStartTime + Int64(request.start * TimeInterval(AV_TIME_BASE))
        let endTimestamp = formatStartTime + Int64(effectiveEnd * TimeInterval(AV_TIME_BASE))
        result = avformat_seek_file(inputContext, -1, Int64.min, startTimestamp, endTimestamp, AVSEEK_FLAG_BACKWARD)
        guard result >= 0 else {
            throw KSPlayerClipExportError.unsupportedNonSeekableSource
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
                if packetTime > endTimestamp {
                    break
                }
                let seconds = Double(packetTime - startTimestamp) / Double(AV_TIME_BASE)
                progress?(KSPlayerClipExportProgress(phase: .exporting, fractionCompleted: KSPlayerClipProgressPolicy.clampedFraction(completed: seconds, duration: effectiveDuration)))
            }

            let inputTimeBase = inputStream.pointee.time_base
            let outputTimeBase = outputStream.pointee.time_base
            let streamStart = av_rescale_q(startTimestamp, AVRational(num: 1, den: AV_TIME_BASE), inputTimeBase)
            if packet.pts != swift_AV_NOPTS_VALUE {
                packet.pts = max(packet.pts - streamStart, 0)
            }
            if packet.dts != swift_AV_NOPTS_VALUE {
                packet.dts = max(packet.dts - streamStart, 0)
            }
            av_packet_rescale_ts(&packet, inputTimeBase, outputTimeBase)
            packet.stream_index = Int32(outputIndex)
            packet.pos = -1

            result = av_interleaved_write_frame(outputContext, &packet)
            guard result >= 0 else {
                throw KSPlayerClipExportError.ffmpegFailure(String(avErrorCode: result))
            }
        }
        try Task.checkCancellation()

        result = av_write_trailer(outputContext)
        guard result >= 0 else {
            throw KSPlayerClipExportError.ffmpegFailure(String(avErrorCode: result))
        }
        do {
            if fileManager.fileExists(atPath: request.destination.path) {
                guard request.overwriteExisting else {
                    throw KSPlayerClipExportError.destinationExists(request.destination)
                }
                try fileManager.removeItem(at: request.destination)
            }
            try fileManager.moveItem(at: temporaryDestination, to: request.destination)
        } catch let error as KSPlayerClipExportError {
            throw error
        } catch {
            throw KSPlayerClipExportError.noWritableDestination
        }
        return request.destination
    }

    private static func makeOutputStreams(inputContext: UnsafeMutablePointer<AVFormatContext>, outputContext: UnsafeMutablePointer<AVFormatContext>) throws -> [Int: Int] {
        var streamMapping = [Int: Int]()
        let formatName = outputContext.pointee.oformat.pointee.name.flatMap { String(cString: $0).lowercased() } ?? ""
        let isQuickTimeContainer = formatName.split(separator: ",").contains { name in
            name == "mp4" || name == "mov"
        }
        for inputIndex in 0 ..< Int(inputContext.pointee.nb_streams) {
            guard let inputStream = inputContext.pointee.streams[inputIndex] else {
                continue
            }
            let codecType = inputStream.pointee.codecpar.pointee.codec_type
            guard codecType == AVMEDIA_TYPE_VIDEO || codecType == AVMEDIA_TYPE_AUDIO || codecType == AVMEDIA_TYPE_SUBTITLE else {
                continue
            }
            if codecType == AVMEDIA_TYPE_SUBTITLE, isQuickTimeContainer,
               inputStream.pointee.codecpar.pointee.codec_id != AV_CODEC_ID_MOV_TEXT
            {
                continue
            }
            guard let outputStream = avformat_new_stream(outputContext, nil) else {
                throw KSPlayerClipExportError.noExportableStreams
            }
            let result = avcodec_parameters_copy(outputStream.pointee.codecpar, inputStream.pointee.codecpar)
            guard result >= 0 else {
                throw KSPlayerClipExportError.ffmpegFailure(String(avErrorCode: result))
            }
            if inputStream.pointee.codecpar.pointee.codec_id == AV_CODEC_ID_HEVC, isQuickTimeContainer {
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

private struct KSPlayerClipSource: @unchecked Sendable {
    let ffmpegURLString: String
    let securityScopedURLs: [URL]
    let customIO: AbstractAVIOContext?

    init(sourceURL: URL, options: KSOptions) {
        let bluRaySource = KSBluRayURLResolver.source(for: sourceURL)
        options.prepareFormatContextOptions(for: sourceURL)
        if let bluRaySource {
            options.prepareFormatContextOptions(for: bluRaySource)
            ffmpegURLString = bluRaySource.ffmpegURLString
            securityScopedURLs = bluRaySource.url == sourceURL ? [sourceURL] : [sourceURL, bluRaySource.url]
            customIO = nil
        } else {
            ffmpegURLString = sourceURL.isFileURL ? sourceURL.path : sourceURL.absoluteString
            securityScopedURLs = sourceURL.isFileURL ? [sourceURL] : []
            customIO = options.process(url: sourceURL)
        }
    }
}

enum KSPlayerClipPathPolicy {
    static func validate(sourceURL: URL, request: KSPlayerClipExportRequest, fileManager: FileManager = .default) throws {
        guard request.destination.isFileURL,
              !request.destination.path.isEmpty,
              !request.destination.lastPathComponent.isEmpty
        else {
            throw KSPlayerClipExportError.noWritableDestination
        }

        let destination = request.destination.standardizedFileURL
        if sourceURL.isFileURL, sourceURL.standardizedFileURL == destination {
            throw KSPlayerClipExportError.sourceAndDestinationMatch(request.destination)
        }

        var isDirectory = ObjCBool(false)
        if fileManager.fileExists(atPath: destination.path, isDirectory: &isDirectory) {
            if isDirectory.boolValue {
                throw KSPlayerClipExportError.noWritableDestination
            }
            if !request.overwriteExisting {
                throw KSPlayerClipExportError.destinationExists(request.destination)
            }
        }
    }
}

enum KSPlayerClipProgressPolicy {
    static func clampedFraction(completed: TimeInterval, duration: TimeInterval) -> Double? {
        guard completed.isFinite, duration.isFinite, duration > 0 else {
            return nil
        }
        return min(max(completed / duration, 0), 1)
    }
}

public extension KSPlayerLayer {
    /// Records a seekable slice of the current media to a local file using FFmpeg stream copy.
    ///
    /// The player must expose a finite seekable range; non-DVR live streams are rejected. The default
    /// destination is in the user's document directory and existing files are preserved unless
    /// `overwriteExisting` is true.
    @discardableResult
    func recordClip(
        start: TimeInterval,
        end: TimeInterval,
        destination: URL? = nil,
        overwriteExisting: Bool = false,
        progress: KSPlayerClipExporter.ProgressHandler? = nil
    ) async throws -> URL {
        let sourceURL = url
        let options = self.options
        guard let availableRange = player.seekableTimeRange else {
            throw KSPlayerClipExportError.unsupportedNonSeekableSource
        }
        let destination = destination ?? KSPlayerClipDestination.makeDefault(sourceURL: sourceURL, start: start, end: end)
        return try await KSPlayerClipExporter().export(
            sourceURL: sourceURL,
            start: start,
            end: end,
            destination: destination,
            overwriteExisting: overwriteExisting,
            options: options,
            availableRange: availableRange,
            progress: progress
        )
    }
}

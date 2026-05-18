import AVFoundation
import FFmpegKit
import Foundation
import Libavcodec
import Libavformat

public enum KSPlayerClipExportError: Error, Equatable, LocalizedError {
    case invalidTimeRange
    case unsupportedNonSeekableSource
    case noWritableDestination
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
        case .noExportableStreams:
            return "No audio, video, or subtitle streams can be copied to the destination container."
        case let .ffmpegFailure(message):
            return message
        }
    }
}

public struct KSPlayerClipExportRequest: Equatable, Sendable {
    public let start: TimeInterval
    public let end: TimeInterval
    public let destination: URL

    public var duration: TimeInterval {
        end - start
    }

    public init(start: TimeInterval, end: TimeInterval, destination: URL) throws {
        guard start.isFinite, end.isFinite, end > start else {
            throw KSPlayerClipExportError.invalidTimeRange
        }
        self.start = max(start, 0)
        self.end = end
        self.destination = destination
    }

    static func validated(start: TimeInterval, end: TimeInterval, destination: URL, availableRange: MediaPlaybackTimeRange?) throws -> Self {
        guard let availableRange else {
            throw KSPlayerClipExportError.unsupportedNonSeekableSource
        }
        let clampedStart = availableRange.clamped(start)
        let clampedEnd = availableRange.clamped(end)
        return try Self(start: clampedStart, end: clampedEnd, destination: destination)
    }
}

struct KSPlayerClipDestination {
    static func makeDefault(sourceURL: URL, directory: URL? = nil, start: TimeInterval, end: TimeInterval, fileExtension: String = "mp4") -> URL {
        let directory = directory
            ?? FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
            ?? FileManager.default.temporaryDirectory
        let basename = sourceURL.deletingPathExtension().lastPathComponent.isEmpty ? "clip" : sourceURL.deletingPathExtension().lastPathComponent
        let startText = timeComponent(start)
        let endText = timeComponent(end)
        return directory.appendingPathComponent("\(basename)-\(startText)-\(endText)").appendingPathExtension(fileExtension)
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
    public init() {}

    public func export(sourceURL: URL, start: TimeInterval, end: TimeInterval, destination: URL, options: KSOptions = KSOptions(), availableRange: MediaPlaybackTimeRange? = nil) async throws -> URL {
        let playbackURL = KSDiskPrecache.playbackURL(for: sourceURL, options: options)
        let request = try KSPlayerClipExportRequest.validated(
            start: start,
            end: end,
            destination: destination,
            availableRange: availableRange ?? MediaPlaybackTimeRange(start: 0, duration: end)
        )
        let exportOptions = KSPlayerFFmpegClipExportOptions(formatContextOptions: options.formatContextOptions)

        return try await Task.detached(priority: .utility) {
            try Self.remux(sourceURL: playbackURL, request: request, options: exportOptions)
        }.value
    }

    private static func remux(sourceURL: URL, request: KSPlayerClipExportRequest, options: KSPlayerFFmpegClipExportOptions) throws -> URL {
        let fileManager = FileManager.default
        let destinationDirectory = request.destination.deletingLastPathComponent()
        do {
            try fileManager.createDirectory(at: destinationDirectory, withIntermediateDirectories: true)
            if fileManager.fileExists(atPath: request.destination.path) {
                try fileManager.removeItem(at: request.destination)
            }
        } catch {
            throw KSPlayerClipExportError.noWritableDestination
        }

        let sourceAccess = sourceURL.isFileURL ? KSSecurityScopedURLAccess(url: sourceURL) : nil
        let destinationAccess = request.destination.isFileURL ? KSSecurityScopedURLAccess(url: destinationDirectory) : nil
        defer {
            sourceAccess?.stop()
            destinationAccess?.stop()
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
        let source = sourceURL.isFileURL ? sourceURL.path : sourceURL.absoluteString
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
        guard inputDuration > 0, request.start < inputDuration else {
            throw KSPlayerClipExportError.unsupportedNonSeekableSource
        }

        var outputContext: UnsafeMutablePointer<AVFormatContext>?
        let destination = request.destination.isFileURL ? request.destination.path : request.destination.absoluteString
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
        let endTimestamp = formatStartTime + Int64(request.end * TimeInterval(AV_TIME_BASE))
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

        result = av_write_trailer(outputContext)
        guard result >= 0 else {
            throw KSPlayerClipExportError.ffmpegFailure(String(avErrorCode: result))
        }
        return request.destination
    }

    private static func makeOutputStreams(inputContext: UnsafeMutablePointer<AVFormatContext>, outputContext: UnsafeMutablePointer<AVFormatContext>) throws -> [Int: Int] {
        var streamMapping = [Int: Int]()
        let formatName = outputContext.pointee.oformat.pointee.name.flatMap { String(cString: $0).lowercased() } ?? ""
        for inputIndex in 0 ..< Int(inputContext.pointee.nb_streams) {
            guard let inputStream = inputContext.pointee.streams[inputIndex] else {
                continue
            }
            let codecType = inputStream.pointee.codecpar.pointee.codec_type
            guard codecType == AVMEDIA_TYPE_VIDEO || codecType == AVMEDIA_TYPE_AUDIO || codecType == AVMEDIA_TYPE_SUBTITLE else {
                continue
            }
            if codecType == AVMEDIA_TYPE_SUBTITLE, (formatName == "mp4" || formatName == "mov"),
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
            outputStream.pointee.codecpar.pointee.codec_tag = 0
            outputStream.pointee.time_base = inputStream.pointee.time_base
            streamMapping[inputIndex] = Int(outputStream.pointee.index)
        }
        return streamMapping
    }
}

public extension KSPlayerLayer {
    @discardableResult
    func recordClip(start: TimeInterval, end: TimeInterval, destination: URL? = nil) async throws -> URL {
        let sourceURL = url
        let options = self.options
        let availableRange = player.seekableTimeRange
        let destination = destination ?? KSPlayerClipDestination.makeDefault(sourceURL: sourceURL, start: start, end: end)
        return try await KSPlayerClipExporter().export(
            sourceURL: sourceURL,
            start: start,
            end: end,
            destination: destination,
            options: options,
            availableRange: availableRange
        )
    }
}

import CoreGraphics
import FFmpegKit
import Foundation
import Libavcodec
import Libavformat
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

enum ExternalImageSubtitleLoader {
    static func load(url: URL, userAgent: String?) async throws -> [SubtitlePart] {
        try loadSynchronously(url: url, userAgent: userAgent)
    }

    private static func loadSynchronously(url: URL, userAgent: String?) throws -> [SubtitlePart] {
        _ = avformat_network_init()
        let access = KSSecurityScopedURLAccess(url: url)
        defer { access.stop() }

        var formatContext = avformat_alloc_context()
        guard formatContext != nil else {
            throw NSError(errorCode: .formatCreate)
        }
        defer { avformat_close_input(&formatContext) }

        let input = url.isFileURL ? url.path : url.absoluteString
        var inputOptionValues = [String: Any]()
        if let userAgent {
            inputOptionValues["user_agent"] = userAgent
        }
        var options = inputOptionValues.avOptions
        var result = avformat_open_input(&formatContext, input, nil, &options)
        av_dict_free(&options)
        guard result == 0 else {
            throw NSError(errorCode: .formatOpenInput, userInfo: [NSUnderlyingErrorKey: AVError(code: result)])
        }
        result = avformat_find_stream_info(formatContext, nil)
        guard result == 0, let formatContext else {
            throw NSError(errorCode: .formatFindStreamInfo, userInfo: [NSUnderlyingErrorKey: AVError(code: result)])
        }
        guard let streamIndex = imageSubtitleStreamIndex(in: formatContext),
              let stream = formatContext.pointee.streams[streamIndex]
        else {
            throw NSError(errorCode: .subtitleFormatUnSupport)
        }

        var codecParameters = stream.pointee.codecpar.pointee
        var codecContext: UnsafeMutablePointer<AVCodecContext>? = try codecParameters.createContext(options: nil)
        defer {
            if codecContext != nil {
                avcodec_free_context(&codecContext)
            }
        }
        guard let codecContext else {
            throw NSError(errorCode: .codecContextCreate)
        }

        var packet = AVPacket()
        var subtitle = AVSubtitle()
        let timebase = Timebase(stream.pointee.time_base)
        var parts = [SubtitlePart]()
        let scale = VideoSwresample(dstFormat: AV_PIX_FMT_ARGB, isDovi: false)
        defer {
            scale.shutdown()
            av_packet_unref(&packet)
            avsubtitle_free(&subtitle)
        }

        while av_read_frame(formatContext, &packet) >= 0 {
            defer { av_packet_unref(&packet) }
            guard packet.stream_index == streamIndex else {
                continue
            }
            var gotSubtitle = Int32(0)
            result = avcodec_decode_subtitle2(codecContext, &subtitle, &gotSubtitle, &packet)
            guard result >= 0 else {
                throw NSError(errorCode: .codecSubtitleSendPacket, userInfo: [NSUnderlyingErrorKey: AVError(code: result)])
            }
            guard gotSubtitle != 0 else {
                continue
            }

            let start = subtitleStart(packet: packet, subtitle: subtitle, timebase: timebase)
            let duration = subtitleDuration(packet: packet, subtitle: subtitle, timebase: timebase)
            parts.append(contentsOf: imageParts(subtitle: subtitle, start: start, duration: duration, codecContext: codecContext, scale: scale))
            avsubtitle_free(&subtitle)
        }
        return parts.sorted { $0.start < $1.start }
    }

    private static func imageSubtitleStreamIndex(in formatContext: UnsafeMutablePointer<AVFormatContext>) -> Int? {
        for index in 0 ..< Int(formatContext.pointee.nb_streams) {
            guard let stream = formatContext.pointee.streams[index] else {
                continue
            }
            let codecParameters = stream.pointee.codecpar.pointee
            guard codecParameters.codec_type == AVMEDIA_TYPE_SUBTITLE else {
                continue
            }
            if FFmpegAssetTrack.subtitleKind(codecID: codecParameters.codec_id, isImageSubtitle: isImageSubtitleCodec(codecParameters.codec_id)) == .image {
                return index
            }
        }
        return nil
    }

    private static func isImageSubtitleCodec(_ codecID: AVCodecID) -> Bool {
        [AV_CODEC_ID_DVD_SUBTITLE, AV_CODEC_ID_DVB_SUBTITLE, AV_CODEC_ID_DVB_TELETEXT, AV_CODEC_ID_HDMV_PGS_SUBTITLE].contains(codecID)
    }

    private static func subtitleStart(packet: AVPacket, subtitle: AVSubtitle, timebase: Timebase) -> TimeInterval {
        let timestamp = packet.pts == swift_AV_NOPTS_VALUE ? packet.dts : packet.pts
        let packetStart = timestamp == swift_AV_NOPTS_VALUE ? 0 : timebase.cmtime(for: timestamp).seconds
        return packetStart + TimeInterval(subtitle.start_display_time) / 1000.0
    }

    private static func subtitleDuration(packet: AVPacket, subtitle: AVSubtitle, timebase: Timebase) -> TimeInterval {
        if subtitle.end_display_time != UInt32.max, subtitle.end_display_time > subtitle.start_display_time {
            return TimeInterval(subtitle.end_display_time - subtitle.start_display_time) / 1000.0
        }
        if packet.duration > 0 {
            return timebase.cmtime(for: packet.duration).seconds
        }
        return 0
    }

    private static func imageParts(subtitle: AVSubtitle, start: TimeInterval, duration: TimeInterval, codecContext: UnsafeMutablePointer<AVCodecContext>, scale: VideoSwresample) -> [SubtitlePart] {
        var images = [(CGRect, CGImage)]()
        for index in 0 ..< Int(subtitle.num_rects) {
            guard let rect = subtitle.rects[index]?.pointee,
                  rect.type == SUBTITLE_BITMAP,
                  let image = scale.transfer(format: AV_PIX_FMT_PAL8, width: rect.w, height: rect.h, data: Array(tuple: rect.data), linesize: Array(tuple: rect.linesize))?.cgImage()
            else {
                continue
            }
            images.append((CGRect(x: Int(rect.x), y: Int(rect.y), width: Int(rect.w), height: Int(rect.h)), image))
        }
        guard !images.isEmpty else {
            return []
        }

        let origin: CGPoint = images.count > 1 ? .zero : images[0].0.origin
        guard let imageData = CGImage.combine(images: images)?.data(type: .png, quality: 0.2),
              let image = UIImage(data: imageData)
        else {
            return []
        }

        let part = SubtitlePart(start, duration > 0 ? start + duration : .infinity, attributedString: nil)
        part.image = image
        part.origin = origin
        part.imageRect = CGRect(origin: origin, size: image.size)
        part.imageCanvasSize = canvasSize(codecContext: codecContext, images: images)
        return [part]
    }

    private static func canvasSize(codecContext: UnsafeMutablePointer<AVCodecContext>, images: [(CGRect, CGImage)]) -> CGSize? {
        if codecContext.pointee.width > 0, codecContext.pointee.height > 0 {
            return CGSize(width: Int(codecContext.pointee.width), height: Int(codecContext.pointee.height))
        }
        let imageBounds = images.reduce(CGRect.null) { bounds, image in
            bounds.union(image.0)
        }
        guard !imageBounds.isNull, imageBounds.width > 0, imageBounds.height > 0 else {
            return nil
        }
        return CGSize(width: max(CGFloat(1920), imageBounds.maxX), height: max(CGFloat(1080), imageBounds.maxY))
    }
}

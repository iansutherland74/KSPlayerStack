//
//  FFmpegDecode.swift
//  KSPlayer
//
//  Created by kintan on 2018/3/9.
//

import AVFoundation
import Foundation
import Libavcodec

class FFmpegDecode: DecodeProtocol {
    private let options: KSOptions
    private var coreFrame: UnsafeMutablePointer<AVFrame>? = av_frame_alloc()
    private var codecContext: UnsafeMutablePointer<AVCodecContext>?
    private var bestEffortTimestamp = Int64(0)
    private let frameChange: FrameChange
    private let filter: MEFilter
    private let seekByBytes: Bool
    required init(assetTrack: FFmpegAssetTrack, options: KSOptions) {
        self.options = options
        seekByBytes = assetTrack.seekByBytes
        do {
            codecContext = try assetTrack.createContext(options: options)
        } catch {
            KSLog(error as CustomStringConvertible)
        }
        codecContext?.pointee.time_base = assetTrack.timebase.rational
        filter = MEFilter(timebase: assetTrack.timebase, isAudio: assetTrack.mediaType == .audio, nominalFrameRate: assetTrack.nominalFrameRate, options: options)
        if assetTrack.mediaType == .video {
            frameChange = VideoSwresample(
                fps: assetTrack.nominalFrameRate,
                isDovi: assetTrack.dovi != nil,
                dolbyVisionFallbackDynamicRange: assetTrack.dovi?.hdrFallbackDynamicRange
            )
        } else {
            frameChange = AudioSwresample(audioDescriptor: assetTrack.audioDescriptor!, options: options)
        }
    }

    func decodeFrame(from packet: Packet, completionHandler: @escaping (Result<MEFrame, Error>) -> Void) {
        let packetHDR10Plus = packetHDR10PlusMetadata(from: packet)
        if packetHDR10Plus.hasMetadata {
            packet.assetTrack.markHDR10PlusMetadataDetected(packetHDR10Plus.metadata)
        }
        guard let codecContext, avcodec_send_packet(codecContext, packet.corePacket) == 0 else {
            return
        }
        while true {
            let result = avcodec_receive_frame(codecContext, coreFrame)
            if result == 0, let inputFrame = coreFrame {
                let frameInterlacingType: VideoInterlacingType?
                if packet.assetTrack.mediaType == .video {
                    frameInterlacingType = VideoDeinterlacePolicy.detectedInterlacingType(frameFlags: inputFrame.pointee.flags)
                    options.videoInterlacingType = frameInterlacingType
                } else {
                    frameInterlacingType = nil
                }
                var displayData: MasteringDisplayMetadata?
                var contentData: ContentLightMetadata?
                var ambientViewingEnvironment: AmbientViewingEnvironment?
                var hdr10PlusMetadata = packetHDR10Plus.metadata
                var hasHDR10PlusMetadata = packetHDR10Plus.hasMetadata
                // filter之后，side_data信息会丢失，所以放在这里
                if inputFrame.pointee.nb_side_data > 0 {
                    for i in 0 ..< inputFrame.pointee.nb_side_data {
                        if let sideData = inputFrame.pointee.side_data[Int(i)]?.pointee {
                            if sideData.type == AV_FRAME_DATA_A53_CC {
                                let sideDataFormat = FFmpegClosedCaptionRouting.format(
                                    a53CCSideData: sideData.data,
                                    size: Int(sideData.size)
                                )
                                if let closedCaptionsTrack = closedCaptionsTrack(
                                    for: packet.assetTrack,
                                    sideDataFormat: sideDataFormat
                                ),
                                   let subtitle = closedCaptionsTrack.subtitle,
                                   let payload = FFmpegClosedCaptionRouting.eia608Payload(
                                       a53CCSideData: sideData.data,
                                       size: Int(sideData.size)
                                   )
                                {
                                    let closedCaptionsPacket = Packet()
                                    if let corePacket = packet.corePacket {
                                        closedCaptionsPacket.corePacket?.pointee.pts = corePacket.pointee.pts
                                        closedCaptionsPacket.corePacket?.pointee.dts = corePacket.pointee.dts
                                        closedCaptionsPacket.corePacket?.pointee.pos = corePacket.pointee.pos
                                        closedCaptionsPacket.corePacket?.pointee.time_base = corePacket.pointee.time_base
                                        closedCaptionsPacket.corePacket?.pointee.stream_index = corePacket.pointee.stream_index
                                    }
                                    closedCaptionsPacket.corePacket?.pointee.flags |= AV_PKT_FLAG_KEY
                                    if fillClosedCaptionsPacket(closedCaptionsPacket, payload: payload, originalSideData: sideData) {
                                        closedCaptionsPacket.assetTrack = closedCaptionsTrack
                                        subtitle.putPacket(packet: closedCaptionsPacket)
                                    }
                                }
                            } else if sideData.type == AV_FRAME_DATA_SEI_UNREGISTERED {
                                let size = sideData.size
                                if size > AV_UUID_LEN {
                                    let str = String(cString: sideData.data.advanced(by: Int(AV_UUID_LEN)))
                                    options.sei(string: str)
                                }
                            } else if sideData.type == AV_FRAME_DATA_DOVI_RPU_BUFFER {
                                packet.assetTrack.recordDolbyVisionRPUBuffer(Data(bytes: sideData.data, count: sideData.size))
                                options.dolbyVisionPlaybackDiagnostic = packet.assetTrack.dolbyVisionPlaybackDiagnostic
                            } else if sideData.type == AV_FRAME_DATA_DOVI_METADATA { // AVDOVIMetadata
                                let data = sideData.data.withMemoryRebound(to: AVDOVIMetadata.self, capacity: 1) { $0 }
                                _ = av_dovi_get_header(data)
                                let mapping = av_dovi_get_mapping(data)
                                _ = av_dovi_get_color(data)
                                let enhancementLayerKind: DolbyVisionProfile7EnhancementLayerKind?
                                if packet.assetTrack.dovi?.dv_profile == 7,
                                   packet.assetTrack.dovi?.el_present_flag != 0
                                {
                                    enhancementLayerKind = mapping?.pointee.nlq_method_idc == AV_DOVI_NLQ_NONE
                                        ? .minimumEnhancementLayer
                                        : .fullEnhancementLayer
                                } else {
                                    enhancementLayerKind = nil
                                }
                                packet.assetTrack.recordDolbyVisionFrameMetadata(
                                    enhancementLayerKind: enhancementLayerKind,
                                    playbackPolicy: options.dolbyVisionFELPlaybackPolicy
                                )
                                options.dolbyVisionPlaybackDiagnostic = packet.assetTrack.dolbyVisionPlaybackDiagnostic
                                if let diagnostic = options.dolbyVisionPlaybackDiagnostic, diagnostic.blocksPlayback {
                                    completionHandler(.failure(NSError(description: diagnostic.description)))
                                    return
                                }
//                                frame.corePixelBuffer?.transferFunction = kCVImageBufferTransferFunction_ITU_R_2020
                            } else if sideData.type == AV_FRAME_DATA_DYNAMIC_HDR_PLUS { // AVDynamicHDRPlus
                                hasHDR10PlusMetadata = true
                                let metadata = FFmpegAssetTrack.hdr10PlusMetadata(
                                    data: sideData.data,
                                    size: sideData.size,
                                    width: packet.assetTrack.codecpar.width,
                                    height: packet.assetTrack.codecpar.height
                                )
                                if let metadata {
                                    hdr10PlusMetadata = metadata
                                }
                                packet.assetTrack.markHDR10PlusMetadataDetected(metadata)
                            } else if sideData.type == AV_FRAME_DATA_DYNAMIC_HDR_VIVID { // AVDynamicHDRVivid
                                _ = sideData.data.withMemoryRebound(to: AVDynamicHDRVivid.self, capacity: 1) { $0 }.pointee
                            } else if sideData.type == AV_FRAME_DATA_MASTERING_DISPLAY_METADATA {
                                let data = sideData.data.withMemoryRebound(to: AVMasteringDisplayMetadata.self, capacity: 1) { $0 }.pointee
                                displayData = MasteringDisplayMetadata(
                                    display_primaries_r_x: UInt16(data.display_primaries.0.0.num).bigEndian,
                                    display_primaries_r_y: UInt16(data.display_primaries.0.1.num).bigEndian,
                                    display_primaries_g_x: UInt16(data.display_primaries.1.0.num).bigEndian,
                                    display_primaries_g_y: UInt16(data.display_primaries.1.1.num).bigEndian,
                                    display_primaries_b_x: UInt16(data.display_primaries.2.0.num).bigEndian,
                                    display_primaries_b_y: UInt16(data.display_primaries.2.1.num).bigEndian,
                                    white_point_x: UInt16(data.white_point.0.num).bigEndian,
                                    white_point_y: UInt16(data.white_point.1.num).bigEndian,
                                    minLuminance: UInt32(data.min_luminance.num).bigEndian,
                                    maxLuminance: UInt32(data.max_luminance.num).bigEndian
                                )
                            } else if sideData.type == AV_FRAME_DATA_CONTENT_LIGHT_LEVEL {
                                let data = sideData.data.withMemoryRebound(to: AVContentLightMetadata.self, capacity: 1) { $0 }.pointee
                                contentData = ContentLightMetadata(
                                    MaxCLL: UInt16(data.MaxCLL).bigEndian,
                                    MaxFALL: UInt16(data.MaxFALL).bigEndian
                                )
                            } else if sideData.type == AV_FRAME_DATA_AMBIENT_VIEWING_ENVIRONMENT {
                                let data = sideData.data.withMemoryRebound(to: AVAmbientViewingEnvironment.self, capacity: 1) { $0 }.pointee
                                ambientViewingEnvironment = AmbientViewingEnvironment(
                                    ambient_illuminance: UInt32(data.ambient_illuminance.num).bigEndian,
                                    ambient_light_x: UInt16(data.ambient_light_x.num).bigEndian,
                                    ambient_light_y: UInt16(data.ambient_light_y.num).bigEndian
                                )
                            }
                        }
                    }
                }
                filter.filter(options: options, inputFrame: inputFrame) { avframe in
                    do {
                        var frame = try frameChange.change(avframe: avframe)
                        if let videoFrame = frame as? VideoVTBFrame, let pixelBuffer = videoFrame.corePixelBuffer {
                            videoFrame.interlacingType = frameInterlacingType
                            if let pixelBuffer = pixelBuffer as? PixelBuffer {
                                pixelBuffer.formatDescription = packet.assetTrack.formatDescription
                            }
                            applyMissingColorMetadata(to: pixelBuffer, from: packet.assetTrack)
                            if displayData != nil || contentData != nil || ambientViewingEnvironment != nil || hasHDR10PlusMetadata {
                                videoFrame.edrMetaData = EDRMetaData(
                                    displayData: displayData,
                                    contentData: contentData,
                                    ambientViewingEnvironment: ambientViewingEnvironment,
                                    hasHDR10PlusMetadata: hasHDR10PlusMetadata,
                                    hdr10PlusMetadata: hdr10PlusMetadata
                                )
                            }
                        }
                        frame.timebase = filter.timebase
                        //                frame.timebase = Timebase(avframe.pointee.time_base)
                        frame.size = packet.size
                        frame.position = packet.position
                        frame.duration = avframe.pointee.duration
                        if frame.duration == 0, avframe.pointee.sample_rate != 0, frame.timebase.num != 0 {
                            frame.duration = Int64(avframe.pointee.nb_samples) * Int64(frame.timebase.den) / (Int64(avframe.pointee.sample_rate) * Int64(frame.timebase.num))
                        }
                        var timestamp = avframe.pointee.best_effort_timestamp
                        if timestamp < 0 {
                            timestamp = avframe.pointee.pts
                        }
                        if timestamp < 0 {
                            timestamp = avframe.pointee.pkt_dts
                        }
                        if timestamp < 0 {
                            timestamp = bestEffortTimestamp
                        }
                        frame.timestamp = timestamp
                        bestEffortTimestamp = timestamp &+ frame.duration
                        completionHandler(.success(frame))
                    } catch {
                        completionHandler(.failure(error))
                    }
                }
            } else {
                if result == AVError.eof.code {
                    avcodec_flush_buffers(codecContext)
                    break
                } else if result == AVError.tryAgain.code {
                    break
                } else {
                    let error = NSError(errorCode: packet.assetTrack.mediaType == .audio ? .codecAudioReceiveFrame : .codecVideoReceiveFrame, avErrorCode: result)
                    KSLog(error)
                    completionHandler(.failure(error))
                }
            }
        }
    }

    private func closedCaptionsTrack(
        for assetTrack: FFmpegAssetTrack,
        sideDataFormat: FFmpegClosedCaptionFormat
    ) -> FFmpegAssetTrack? {
        guard sideDataFormat.containsCEA608 else {
            assetTrack.recordUnsupportedClosedCaptionFormat(sideDataFormat)
            return nil
        }
        if let closedCaptionsTrack = assetTrack.closedCaptionsTrack {
            closedCaptionsTrack.updateClosedCaptionFormat(sideDataFormat)
            return closedCaptionsTrack
        }
        var codecpar = AVCodecParameters()
        codecpar.codec_type = AVMEDIA_TYPE_SUBTITLE
        codecpar.codec_id = AV_CODEC_ID_EIA_608
        guard let subtitleAssetTrack = FFmpegAssetTrack(codecpar: codecpar) else {
            return nil
        }
        subtitleAssetTrack.configureAsClosedCaptionsTrack(source: assetTrack, format: sideDataFormat)
        let subtitle = SyncPlayerItemTrack<SubtitleFrame>(mediaType: .subtitle, frameCapacity: 255, options: options)
        subtitleAssetTrack.subtitle = subtitle
        assetTrack.closedCaptionsTrack = subtitleAssetTrack
        subtitle.decode()
        return subtitleAssetTrack
    }

    private func fillClosedCaptionsPacket(
        _ packet: Packet,
        payload: Data,
        originalSideData: AVFrameSideData
    ) -> Bool {
        guard let corePacket = packet.corePacket else {
            return false
        }
        corePacket.pointee.size = Int32(payload.count)
        if payload.count == Int(originalSideData.size), let buffer = av_buffer_ref(originalSideData.buf) {
            corePacket.pointee.data = buffer.pointee.data
            corePacket.pointee.buf = buffer
            return true
        }
        guard av_new_packet(corePacket, Int32(payload.count)) == 0 else {
            return false
        }
        guard let packetData = corePacket.pointee.data else {
            return false
        }
        payload.withUnsafeBytes { payloadBytes in
            if let baseAddress = payloadBytes.baseAddress {
                packetData.update(from: baseAddress.assumingMemoryBound(to: UInt8.self), count: payload.count)
            }
        }
        return true
    }

    private func packetHDR10PlusMetadata(from packet: Packet) -> (hasMetadata: Bool, metadata: HDR10PlusMetadata?) {
        guard let corePacket = packet.corePacket else {
            return (false, nil)
        }
        var size = 0
        guard let sideData = av_packet_get_side_data(corePacket, AV_PKT_DATA_DYNAMIC_HDR10_PLUS, &size) else {
            return (false, nil)
        }
        return (true, FFmpegAssetTrack.hdr10PlusMetadata(
            data: sideData,
            size: size,
            width: packet.assetTrack.codecpar.width,
            height: packet.assetTrack.codecpar.height
        ))
    }

    private func applyMissingColorMetadata(to pixelBuffer: PixelBufferProtocol, from assetTrack: FFmpegAssetTrack) {
        guard let formatDescription = assetTrack.formatDescription else {
            return
        }
        var didUpdateColorMetadata = false
        if pixelBuffer.colorPrimaries == nil, let colorPrimaries = formatDescription.colorPrimaries {
            pixelBuffer.colorPrimaries = colorPrimaries as CFString
            didUpdateColorMetadata = true
        }
        if pixelBuffer.transferFunction == nil, let transferFunction = formatDescription.transferFunction {
            pixelBuffer.transferFunction = transferFunction as CFString
            didUpdateColorMetadata = true
        }
        if pixelBuffer.yCbCrMatrix == nil, let yCbCrMatrix = formatDescription.yCbCrMatrix {
            pixelBuffer.yCbCrMatrix = yCbCrMatrix as CFString
            didUpdateColorMetadata = true
        }
        if didUpdateColorMetadata {
            pixelBuffer.colorspace = KSOptions.colorSpace(ycbcrMatrix: pixelBuffer.yCbCrMatrix, transferFunction: pixelBuffer.transferFunction)
        }
    }

    func doFlushCodec() {
        bestEffortTimestamp = Int64(0)
        // seek之后要清空下，不然解码可能还会有缓存，导致返回的数据是之前seek的。
        avcodec_flush_buffers(codecContext)
    }

    func shutdown() {
        av_frame_free(&coreFrame)
        avcodec_free_context(&codecContext)
        frameChange.shutdown()
    }

    func decode() {
        bestEffortTimestamp = Int64(0)
        if codecContext != nil {
            avcodec_flush_buffers(codecContext)
        }
    }
}

//
//  VideoToolboxDecode.swift
//  KSPlayer
//
//  Created by kintan on 2018/3/10.
//

import FFmpegKit
import Foundation
import Libavformat
#if canImport(VideoToolbox)
import VideoToolbox

class VideoToolboxDecode: DecodeProtocol {
    private var session: DecompressionSession {
        didSet {
            VTDecompressionSessionInvalidate(oldValue.decompressionSession)
        }
    }

    private let options: KSOptions
    private var startTime = Int64(0)
    private var lastPosition = Int64(0)
    private var needReconfig = false

    init(options: KSOptions, session: DecompressionSession) {
        self.options = options
        self.session = session
    }

    func decodeFrame(from packet: Packet, completionHandler: @escaping (Result<MEFrame, Error>) -> Void) {
        if needReconfig {
            // 解决从后台切换到前台，解码失败的问题
            session = DecompressionSession(assetTrack: session.assetTrack, options: options)!
            doFlushCodec()
            needReconfig = false
        }
        guard let corePacket = packet.corePacket?.pointee, let data = corePacket.data else {
            return
        }
        do {
            let sampleBuffer = try session.formatDescription.getSampleBuffer(
                packet: packet,
                isConvertNALSize: session.assetTrack.isConvertNALSize,
                data: data,
                size: Int(corePacket.size)
            )
            let flags: VTDecodeFrameFlags = packet.isSafeForAsynchronousVideoToolboxDecode ? [._EnableAsynchronousDecompression] : []
            var flagOut = VTDecodeInfoFlags.frameDropped
            let timestamp = packet.timestamp
            let packetFlags = corePacket.flags
            let duration = corePacket.duration
            let size = corePacket.size
            let status = VTDecompressionSessionDecodeFrame(session.decompressionSession, sampleBuffer: sampleBuffer, flags: flags, infoFlagsOut: &flagOut) { [weak self] status, infoFlags, imageBuffer, _, _ in
                guard let self, !infoFlags.contains(.frameDropped) else {
                    return
                }
                guard status == noErr else {
                    if status == kVTInvalidSessionErr || status == kVTVideoDecoderMalfunctionErr || status == kVTVideoDecoderBadDataErr {
                        if packet.isKeyFrame {
                            completionHandler(.failure(NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: status)))
                        } else {
                            // 解决从后台切换到前台，解码失败的问题
                            self.needReconfig = true
                        }
                    }
                    return
                }
                let frame = VideoVTBFrame(fps: session.assetTrack.nominalFrameRate, isDovi: session.assetTrack.dovi != nil)
                frame.interlacingType = VideoDeinterlacePolicy.detectedInterlacingType(fieldOrder: session.assetTrack.fieldOrder)
                frame.corePixelBuffer = imageBuffer
                frame.timebase = session.assetTrack.timebase
                if packet.isKeyFrame, packetFlags & AV_PKT_FLAG_DISCARD != 0, self.lastPosition > 0 {
                    self.startTime = self.lastPosition - timestamp
                }
                self.lastPosition = max(self.lastPosition, timestamp)
                frame.position = packet.position
                frame.timestamp = self.startTime + timestamp
                frame.duration = duration
                frame.size = size
                self.lastPosition += frame.duration
                completionHandler(.success(frame))
            }
            if status == noErr {
                if !flags.contains(._EnableAsynchronousDecompression) {
                    VTDecompressionSessionWaitForAsynchronousFrames(session.decompressionSession)
                }
            } else if status == kVTInvalidSessionErr || status == kVTVideoDecoderMalfunctionErr || status == kVTVideoDecoderBadDataErr {
                if packet.isKeyFrame {
                    throw NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: status)
                } else {
                    // 解决从后台切换到前台，解码失败的问题
                    needReconfig = true
                }
            }
        } catch {
            completionHandler(.failure(error))
        }
    }

    func doFlushCodec() {
        lastPosition = 0
        startTime = 0
    }

    func shutdown() {
        VTDecompressionSessionInvalidate(session.decompressionSession)
    }

    func decode() {
        lastPosition = 0
        startTime = 0
    }
}

class DecompressionSession {
    fileprivate let formatDescription: CMFormatDescription
    fileprivate let decompressionSession: VTDecompressionSession
    fileprivate var assetTrack: FFmpegAssetTrack
    init?(assetTrack: FFmpegAssetTrack, options: KSOptions) {
        self.assetTrack = assetTrack
        guard let pixelFormatType = assetTrack.pixelFormatType, let formatDescription = assetTrack.formatDescription else {
            KSLog(level: .debug, "[video] VideoToolbox unavailable: missing pixel format or format description")
            return nil
        }
        let codecType = formatDescription.mediaSubType.rawValue
        guard VideoToolboxHardwareDecodePolicy.canAttemptAsynchronousDecompression(codecType: codecType) else {
            KSLog(level: .debug, "[video] VideoToolbox hardware decode unsupported for \(codecType.string)")
            return nil
        }
        self.formatDescription = formatDescription
        #if os(macOS)
        VTRegisterProfessionalVideoWorkflowVideoDecoders()
        if #available(macOS 11.0, *) {
            VTRegisterSupplementalVideoDecoderIfAvailable(formatDescription.mediaSubType.rawValue)
        }
        #endif
//        VTDecompressionSessionCanAcceptFormatDescription(<#T##session: VTDecompressionSession##VTDecompressionSession#>, formatDescription: <#T##CMFormatDescription#>)
        let attributes: NSMutableDictionary = [
            kCVPixelBufferPixelFormatTypeKey: pixelFormatType,
            kCVPixelBufferMetalCompatibilityKey: true,
            kCVPixelBufferWidthKey: assetTrack.codecpar.width,
            kCVPixelBufferHeightKey: assetTrack.codecpar.height,
            kCVPixelBufferIOSurfacePropertiesKey: NSDictionary(),
        ]
        var session: VTDecompressionSession?
        // swiftlint:disable line_length
        let status = VTDecompressionSessionCreate(allocator: kCFAllocatorDefault, formatDescription: formatDescription, decoderSpecification: CMFormatDescriptionGetExtensions(formatDescription), imageBufferAttributes: attributes, outputCallback: nil, decompressionSessionOut: &session)
        // swiftlint:enable line_length
        guard status == noErr, let decompressionSession = session else {
            KSLog(level: .debug, "[video] VideoToolbox session creation failed for \(codecType.string): \(status)")
            return nil
        }
        if #available(iOS 14.0, tvOS 14.0, macOS 11.0, *) {
            VTSessionSetProperty(decompressionSession, key: kVTDecompressionPropertyKey_PropagatePerFrameHDRDisplayMetadata,
                                 value: kCFBooleanTrue)
        }
        let contentDynamicRange = assetTrack.dovi?.hdrFallbackDynamicRange
        if contentDynamicRange?.isHDR == true, let destinationDynamicRange = options.availableDynamicRange(contentDynamicRange) {
            if destinationDynamicRange.isHDR {
                let pixelTransferProperties = [kVTPixelTransferPropertyKey_DestinationColorPrimaries: destinationDynamicRange.colorPrimaries,
                                               kVTPixelTransferPropertyKey_DestinationTransferFunction: destinationDynamicRange.transferFunction,
                                               kVTPixelTransferPropertyKey_DestinationYCbCrMatrix: destinationDynamicRange.yCbCrMatrix]
                VTSessionSetProperty(decompressionSession,
                                     key: kVTDecompressionPropertyKey_PixelTransferProperties,
                                     value: pixelTransferProperties as CFDictionary)
            } else {
                KSLog(level: .debug, "[video] Preserve HDR/Dolby Vision metadata by avoiding SDR VideoToolbox pixel transfer")
            }
        }
        self.decompressionSession = decompressionSession
    }
}

enum VideoToolboxHardwareDecodePolicy {
    static let knownHardwareCodecTypes: Set<CMVideoCodecType> = [
        kCMVideoCodecType_H263,
        kCMVideoCodecType_H264,
        kCMVideoCodecType_HEVC,
        kCMVideoCodecType_HEVCWithAlpha,
        kCMVideoCodecType_DolbyVisionHEVC,
        kCMVideoCodecType_MPEG1Video,
        kCMVideoCodecType_MPEG2Video,
        kCMVideoCodecType_MPEG4Video,
        kCMVideoCodecType_VP9,
        kCMVideoCodecType_AV1,
        kCMVideoCodecType_AppleProRes4444XQ,
        kCMVideoCodecType_AppleProRes4444,
        kCMVideoCodecType_AppleProRes422HQ,
        kCMVideoCodecType_AppleProRes422,
        kCMVideoCodecType_AppleProRes422LT,
        kCMVideoCodecType_AppleProRes422Proxy,
        kCMVideoCodecType_AppleProResRAW,
        kCMVideoCodecType_AppleProResRAWHQ,
    ]

    static func canAttemptAsynchronousDecompression(
        codecType: CMVideoCodecType,
        isHardwareDecodeSupported: (CMVideoCodecType) -> Bool = VTIsHardwareDecodeSupported
    ) -> Bool {
        knownHardwareCodecTypes.contains(codecType) && isHardwareDecodeSupported(codecType)
    }
}
#endif

extension CMFormatDescription {
    fileprivate func getSampleBuffer(packet: Packet, isConvertNALSize: Bool, data: UnsafeMutablePointer<UInt8>, size: Int) throws -> CMSampleBuffer {
        let sampleData = try VideoToolboxSampleData.makeLengthPrefixedSample(
            data: UnsafePointer(data),
            size: size,
            convertsThreeByteNALSize: isConvertNALSize
        )
        return try createSampleBuffer(data: sampleData, timing: packet.videoToolboxSampleTiming)
    }

    private func createSampleBuffer(data: Data, timing: CMSampleTimingInfo?) throws -> CMSampleBuffer {
        var blockBuffer: CMBlockBuffer?
        var sampleBuffer: CMSampleBuffer?
        let size = data.count
        // swiftlint:disable line_length
        var status = CMBlockBufferCreateWithMemoryBlock(allocator: kCFAllocatorDefault, memoryBlock: nil, blockLength: size, blockAllocator: kCFAllocatorDefault, customBlockSource: nil, offsetToData: 0, dataLength: size, flags: kCMBlockBufferAssureMemoryNowFlag, blockBufferOut: &blockBuffer)
        if status == noErr, let blockBuffer {
            status = data.withUnsafeBytes { bytes in
                guard let baseAddress = bytes.baseAddress else {
                    return noErr
                }
                return CMBlockBufferReplaceDataBytes(with: baseAddress, blockBuffer: blockBuffer, offsetIntoDestination: 0, dataLength: size)
            }
        }
        if status == noErr {
            if let timing {
                status = CMSampleBufferCreateReady(allocator: kCFAllocatorDefault, dataBuffer: blockBuffer, formatDescription: self, sampleCount: 1, sampleTimingEntryCount: 1, sampleTimingArray: [timing], sampleSizeEntryCount: 1, sampleSizeArray: [size], sampleBufferOut: &sampleBuffer)
            } else {
                status = CMSampleBufferCreateReady(allocator: kCFAllocatorDefault, dataBuffer: blockBuffer, formatDescription: self, sampleCount: 1, sampleTimingEntryCount: 0, sampleTimingArray: nil, sampleSizeEntryCount: 1, sampleSizeArray: [size], sampleBufferOut: &sampleBuffer)
            }
            if let sampleBuffer {
                return sampleBuffer
            }
        }
        throw NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: status)
        // swiftlint:enable line_length
    }
}

enum VideoToolboxSampleData {
    static func isAnnexB(data: UnsafePointer<UInt8>, size: Int) -> Bool {
        findStartCode(in: data, size: size, from: 0, scanLimit: min(size, 64)) != nil
    }

    static func makeLengthPrefixedSample(data: UnsafePointer<UInt8>, size: Int, convertsThreeByteNALSize: Bool) throws -> Data {
        if isAnnexB(data: data, size: size) {
            return try convertAnnexBToLengthPrefixed(data: data, size: size)
        }
        if convertsThreeByteNALSize {
            return try convertThreeByteNALSizeToFourByte(data: data, size: size)
        }
        return Data(bytes: data, count: size)
    }

    static func convertAnnexBToLengthPrefixed(data: UnsafePointer<UInt8>, size: Int) throws -> Data {
        var output = Data()
        var current = findStartCode(in: data, size: size, from: 0)
        while let startCode = current {
            let nalStart = startCode.offset + startCode.length
            current = findStartCode(in: data, size: size, from: nalStart)
            var nalEnd = current?.offset ?? size
            while nalEnd > nalStart, data[nalEnd - 1] == 0 {
                nalEnd -= 1
            }
            guard nalEnd >= nalStart else {
                throw NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: AVError.invalidData.code)
            }
            let nalSize = nalEnd - nalStart
            if nalSize == 0 {
                continue
            }
            var bigEndianNALSize = UInt32(nalSize).bigEndian
            output.append(Data(bytes: &bigEndianNALSize, count: MemoryLayout<UInt32>.size))
            output.append(data.advanced(by: nalStart), count: nalSize)
        }
        guard !output.isEmpty else {
            throw NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: AVError.invalidData.code)
        }
        return output
    }

    static func convertThreeByteNALSizeToFourByte(data: UnsafePointer<UInt8>, size: Int) throws -> Data {
        var output = Data()
        var offset = 0
        while offset < size {
            guard offset + 3 <= size else {
                throw NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: AVError.invalidData.code)
            }
            let nalSize = Int(data[offset]) << 16 | Int(data[offset + 1]) << 8 | Int(data[offset + 2])
            offset += 3
            guard nalSize > 0, offset + nalSize <= size else {
                throw NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: AVError.invalidData.code)
            }
            var bigEndianNALSize = UInt32(nalSize).bigEndian
            output.append(Data(bytes: &bigEndianNALSize, count: MemoryLayout<UInt32>.size))
            output.append(data.advanced(by: offset), count: nalSize)
            offset += nalSize
        }
        return output
    }

    private static func findStartCode(in data: UnsafePointer<UInt8>, size: Int, from offset: Int, scanLimit: Int? = nil) -> (offset: Int, length: Int)? {
        guard size >= 3, offset < size else {
            return nil
        }
        let end = min(scanLimit ?? size, size)
        var index = offset
        while index + 3 <= end {
            if data[index] == 0, data[index + 1] == 0 {
                if data[index + 2] == 1 {
                    return (index, 3)
                }
                if index + 4 <= end, data[index + 2] == 0, data[index + 3] == 1 {
                    return (index, 4)
                }
            }
            index += 1
        }
        return nil
    }
}

extension Packet {
    fileprivate var isSafeForAsynchronousVideoToolboxDecode: Bool {
        guard let corePacket else {
            return false
        }
        return corePacket.pointee.pts != Int64.min && corePacket.pointee.dts != Int64.min
    }

    fileprivate var videoToolboxSampleTiming: CMSampleTimingInfo? {
        guard let corePacket, isSafeForAsynchronousVideoToolboxDecode else {
            return nil
        }
        let duration: CMTime = self.duration > 0 ? timebase.cmtime(for: self.duration) : .invalid
        let presentationTimeStamp = timebase.cmtime(for: corePacket.pointee.pts)
        let decodeTimeStamp = timebase.cmtime(for: corePacket.pointee.dts)
        return CMSampleTimingInfo(duration: duration, presentationTimeStamp: presentationTimeStamp, decodeTimeStamp: decodeTimeStamp)
    }
}

extension CMVideoCodecType {
    var sampleDescriptionExtensionAtomName: String? {
        switch self {
        case kCMVideoCodecType_MPEG4Video:
            return "esds"
        case kCMVideoCodecType_H264:
            return "avcC"
        case kCMVideoCodecType_HEVC, kCMVideoCodecType_HEVCWithAlpha, kCMVideoCodecType_DolbyVisionHEVC:
            return "hvcC"
        case kCMVideoCodecType_AV1:
            return "av1C"
        case kCMVideoCodecType_VP9:
            return "vpcC"
        default:
            return nil
        }
    }

    var avc: String {
        switch self {
        case kCMVideoCodecType_MPEG4Video:
            return "esds"
        case kCMVideoCodecType_H264:
            return "avcC"
        case kCMVideoCodecType_HEVC:
            return "hvcC"
        case "av01".fourCharCode:
            return "av1C"
        case kCMVideoCodecType_VP9:
            return "vpcC"
        default: return "avcC"
        }
    }
}

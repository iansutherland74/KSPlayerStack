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

private struct SendableDecodeCompletion: @unchecked Sendable {
    let handler: (Result<MEFrame, Error>) -> Void

    func callAsFunction(_ result: Result<MEFrame, Error>) {
        handler(result)
    }
}

final class VideoToolboxDecode: DecodeProtocol, @unchecked Sendable {
    private static let lowLatencyAsyncFrameLimit = 3

    private var session: DecompressionSession {
        didSet {
            VTDecompressionSessionInvalidate(oldValue.decompressionSession)
        }
    }

    private let options: KSOptions
    private let lowLatencyAsyncSemaphore: DispatchSemaphore?
    private let stateLock = NSLock()
    private var startTime = Int64(0)
    private var lastPosition = Int64(0)
    private var needReconfig = false

    init(options: KSOptions, session: DecompressionSession) {
        self.options = options
        self.session = session
        lowLatencyAsyncSemaphore = options.lowLatencyLiveProfile == nil
            ? nil
            : DispatchSemaphore(value: Self.lowLatencyAsyncFrameLimit)
    }

    func decodeFrame(from packet: Packet, completionHandler: @escaping (Result<MEFrame, Error>) -> Void) {
        if consumeNeedsReconfig() {
            // 解决从后台切换到前台，解码失败的问题
            guard let newSession = DecompressionSession(assetTrack: session.assetTrack, options: options) else {
                completionHandler(.failure(NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: kVTInvalidSessionErr)))
                return
            }
            session = newSession
            doFlushCodec()
        }
        guard let corePacket = packet.corePacket?.pointee, let data = corePacket.data else {
            return
        }
        do {
            let sampleBuffer = try session.formatDescription.getSampleBuffer(
                packet: packet,
                isConvertNALSize: session.assetTrack.isConvertNALSize,
                data: data,
                size: Int(corePacket.size),
                codecType: session.formatDescription.mediaSubType.rawValue
            )
            let flags: VTDecodeFrameFlags = packet.isSafeForAsynchronousVideoToolboxDecode ? [._EnableAsynchronousDecompression] : []
            let backpressureSlot = reserveLowLatencyAsyncSlotIfNeeded(for: packet, flags: flags)
            if flags.contains(._EnableAsynchronousDecompression), lowLatencyAsyncSemaphore != nil, backpressureSlot == nil {
                return
            }
            var flagOut = VTDecodeInfoFlags.frameDropped
            let timestamp = packet.timestamp
            let packetFlags = corePacket.flags
            let duration = corePacket.duration
            let size = corePacket.size
            let isKeyFrame = packet.isKeyFrame
            let position = packet.position
            let nominalFrameRate = session.assetTrack.nominalFrameRate
            let isDovi = session.assetTrack.dovi != nil
            let fieldOrder = session.assetTrack.fieldOrder
            let timebase = session.assetTrack.timebase
            let completion = SendableDecodeCompletion(handler: completionHandler)
            let status = VTDecompressionSessionDecodeFrame(session.decompressionSession, sampleBuffer: sampleBuffer, flags: flags, infoFlagsOut: &flagOut) { [weak self] status, infoFlags, imageBuffer, _, _ in
                backpressureSlot?.release()
                guard let self, !infoFlags.contains(.frameDropped) else {
                    return
                }
                guard status == noErr else {
                    if status == kVTInvalidSessionErr || status == kVTVideoDecoderMalfunctionErr || status == kVTVideoDecoderBadDataErr {
                        if isKeyFrame {
                            completion(.failure(NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: status)))
                        } else {
                            // 解决从后台切换到前台，解码失败的问题
                            self.markNeedsReconfig()
                        }
                    }
                    return
                }
                let frame = VideoVTBFrame(fps: nominalFrameRate, isDovi: isDovi)
                frame.interlacingType = VideoDeinterlacePolicy.detectedInterlacingType(fieldOrder: fieldOrder)
                frame.corePixelBuffer = imageBuffer
                frame.timebase = timebase
                frame.position = position
                frame.timestamp = self.updateTiming(timestamp: timestamp, isKeyFrame: isKeyFrame, packetFlags: packetFlags, duration: duration)
                frame.duration = duration
                frame.size = size
                completion(.success(frame))
            }
            if status == noErr {
                if !flags.contains(._EnableAsynchronousDecompression) {
                    VTDecompressionSessionWaitForAsynchronousFrames(session.decompressionSession)
                }
            } else if status == kVTInvalidSessionErr || status == kVTVideoDecoderMalfunctionErr || status == kVTVideoDecoderBadDataErr {
                backpressureSlot?.release()
                if packet.isKeyFrame {
                    throw NSError(errorCode: .codecVideoReceiveFrame, avErrorCode: status)
                } else {
                    // 解决从后台切换到前台，解码失败的问题
                    markNeedsReconfig()
                }
            } else {
                backpressureSlot?.release()
            }
        } catch {
            completionHandler(.failure(error))
        }
    }

    func doFlushCodec() {
        resetTiming()
    }

    func shutdown() {
        VTDecompressionSessionInvalidate(session.decompressionSession)
    }

    func decode() {
        resetTiming()
    }

    private func consumeNeedsReconfig() -> Bool {
        stateLock.lock()
        let value = needReconfig
        needReconfig = false
        stateLock.unlock()
        return value
    }

    private func markNeedsReconfig() {
        stateLock.lock()
        needReconfig = true
        stateLock.unlock()
    }

    private func reserveLowLatencyAsyncSlotIfNeeded(for packet: Packet, flags: VTDecodeFrameFlags) -> AsyncDecodeBackpressureSlot? {
        guard flags.contains(._EnableAsynchronousDecompression), let lowLatencyAsyncSemaphore else {
            return nil
        }
        if packet.isKeyFrame {
            lowLatencyAsyncSemaphore.wait()
            return AsyncDecodeBackpressureSlot(semaphore: lowLatencyAsyncSemaphore)
        }
        if lowLatencyAsyncSemaphore.wait(timeout: .now()) == .timedOut {
            return nil
        }
        return AsyncDecodeBackpressureSlot(semaphore: lowLatencyAsyncSemaphore)
    }

    private func resetTiming() {
        stateLock.lock()
        lastPosition = 0
        startTime = 0
        stateLock.unlock()
    }

    private func updateTiming(timestamp: Int64, isKeyFrame: Bool, packetFlags: Int32, duration: Int64) -> Int64 {
        stateLock.lock()
        if isKeyFrame, packetFlags & AV_PKT_FLAG_DISCARD != 0, lastPosition > 0 {
            startTime = lastPosition - timestamp
        }
        lastPosition = max(lastPosition, timestamp)
        let adjustedTimestamp = startTime + timestamp
        lastPosition += duration
        stateLock.unlock()
        return adjustedTimestamp
    }
}

private final class AsyncDecodeBackpressureSlot: @unchecked Sendable {
    private let semaphore: DispatchSemaphore
    private let lock = NSLock()
    private var isReleased = false

    init(semaphore: DispatchSemaphore) {
        self.semaphore = semaphore
    }

    func release() {
        lock.lock()
        guard !isReleased else {
            lock.unlock()
            return
        }
        isReleased = true
        lock.unlock()
        semaphore.signal()
    }

    deinit {
        release()
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
        let hardwareDecodeAvailability = VideoToolboxHardwareDecodePolicy.availability(codecType: codecType)
        guard hardwareDecodeAvailability.isSupported else {
            KSLog(level: .debug, "[video] VideoToolbox hardware decode unavailable for \(codecType.string): "
                  + "\(hardwareDecodeAvailability)")
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
    enum Availability: Equatable, CustomStringConvertible {
        case supported
        case unsupported(String)

        var isSupported: Bool {
            if case .supported = self {
                return true
            }
            return false
        }

        var description: String {
            switch self {
            case .supported:
                return "supported"
            case let .unsupported(reason):
                return reason
            }
        }
    }

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
        isHardwareDecodeSupported: (CMVideoCodecType) -> Bool = VTIsHardwareDecodeSupported,
        isHardwareDecodeAllowedOnCurrentPlatform: () -> Bool = hardwareDecodeAllowedOnCurrentPlatform
    ) -> Bool {
        availability(
            codecType: codecType,
            isHardwareDecodeSupported: isHardwareDecodeSupported,
            isHardwareDecodeAllowedOnCurrentPlatform: isHardwareDecodeAllowedOnCurrentPlatform
        ).isSupported
    }

    static func availability(
        codecType: CMVideoCodecType,
        isHardwareDecodeSupported: (CMVideoCodecType) -> Bool = VTIsHardwareDecodeSupported,
        isHardwareDecodeAllowedOnCurrentPlatform: () -> Bool = hardwareDecodeAllowedOnCurrentPlatform
    ) -> Availability {
        guard knownHardwareCodecTypes.contains(codecType) else {
            return .unsupported("unknown hardware codec")
        }
        guard isHardwareDecodeAllowedOnCurrentPlatform() else {
            return .unsupported("hardware decode is unavailable on this platform")
        }
        guard isHardwareDecodeSupported(codecType) else {
            return .unsupported("VideoToolbox reports no hardware decoder")
        }
        return .supported
    }

    private static func hardwareDecodeAllowedOnCurrentPlatform() -> Bool {
        #if targetEnvironment(simulator)
        return false
        #else
        return true
        #endif
    }
}
#endif

extension CMFormatDescription {
    fileprivate func getSampleBuffer(
        packet: Packet,
        isConvertNALSize: Bool,
        data: UnsafeMutablePointer<UInt8>,
        size: Int,
        codecType: CMVideoCodecType
    ) throws -> CMSampleBuffer {
        let sampleData = try VideoToolboxSampleData.makeLengthPrefixedSample(
            data: UnsafePointer(data),
            size: size,
            convertsThreeByteNALSize: isConvertNALSize,
            codecType: codecType
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
        guard let startCode = findStartCodeAtSampleStart(in: data, size: size) else {
            return false
        }
        return startCode.length == 4 || !isFourByteLengthPrefixedSample(data: data, size: size)
    }

    static func makeLengthPrefixedSample(
        data: UnsafePointer<UInt8>,
        size: Int,
        convertsThreeByteNALSize: Bool,
        codecType: CMVideoCodecType = kCMVideoCodecType_H264
    ) throws -> Data {
        guard usesNALLengthPrefixes(codecType: codecType) else {
            return Data(bytes: data, count: size)
        }
        if isAnnexB(data: data, size: size) {
            return try convertAnnexBToLengthPrefixed(data: data, size: size)
        }
        if convertsThreeByteNALSize {
            return try convertThreeByteNALSizeToFourByte(data: data, size: size)
        }
        return Data(bytes: data, count: size)
    }

    static func usesNALLengthPrefixes(codecType: CMVideoCodecType) -> Bool {
        switch codecType {
        case kCMVideoCodecType_H264,
             kCMVideoCodecType_HEVC,
             kCMVideoCodecType_HEVCWithAlpha,
             kCMVideoCodecType_DolbyVisionHEVC:
            return true
        default:
            return false
        }
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

    static func isFourByteLengthPrefixedSample(data: UnsafePointer<UInt8>, size: Int) -> Bool {
        var offset = 0
        var hasNALUnit = false
        while offset < size {
            guard offset + MemoryLayout<UInt32>.size <= size else {
                return false
            }
            let nalSize = Int(data[offset]) << 24 | Int(data[offset + 1]) << 16 | Int(data[offset + 2]) << 8 | Int(data[offset + 3])
            offset += MemoryLayout<UInt32>.size
            guard nalSize > 0, offset + nalSize <= size else {
                return false
            }
            offset += nalSize
            hasNALUnit = true
        }
        return hasNALUnit
    }

    private static func findStartCodeAtSampleStart(in data: UnsafePointer<UInt8>, size: Int) -> (offset: Int, length: Int)? {
        guard size >= 3 else {
            return nil
        }
        var index = 0
        while index < size, data[index] == 0 {
            index += 1
        }
        guard index < size, data[index] == 1, index >= 2 else {
            return nil
        }
        return (index > 2 ? index - 3 : 0, index > 2 ? 4 : 3)
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

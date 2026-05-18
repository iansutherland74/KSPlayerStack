//
//  MediaPlayerProtocol.swift
//  KSPlayer-tvOS
//
//  Created by kintan on 2018/3/9.
//

import AVFoundation
import Foundation
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

public protocol MediaPlayback: AnyObject {
    var duration: TimeInterval { get }
    var fileSize: Double { get }
    var naturalSize: CGSize { get }
    var chapters: [Chapter] { get }
    var currentPlaybackTime: TimeInterval { get }
    func prepareToPlay()
    func shutdown()
    func seek(time: TimeInterval, completion: @escaping ((Bool) -> Void))
}

public class DynamicInfo: ObservableObject {
    private let metadataBlock: () -> [String: String]
    private let bytesReadBlock: () -> Int64
    private let audioBitrateBlock: () -> Int
    private let videoBitrateBlock: () -> Int
    public var metadata: [String: String] {
        metadataBlock()
    }

    public var bytesRead: Int64 {
        bytesReadBlock()
    }

    public var audioBitrate: Int {
        audioBitrateBlock()
    }

    public var videoBitrate: Int {
        videoBitrateBlock()
    }

    @Published
    public var displayFPS = 0.0
    public var audioVideoSyncDiff = 0.0
    public var droppedVideoFrameCount = UInt32(0)
    public var droppedVideoPacketCount = UInt32(0)
    init(metadata: @escaping () -> [String: String], bytesRead: @escaping () -> Int64, audioBitrate: @escaping () -> Int, videoBitrate: @escaping () -> Int) {
        metadataBlock = metadata
        bytesReadBlock = bytesRead
        audioBitrateBlock = audioBitrate
        videoBitrateBlock = videoBitrate
    }
}

public struct Chapter {
    public let start: TimeInterval
    public let end: TimeInterval
    public let title: String
}

public struct MediaPlaybackTimeRange: Equatable {
    public let start: TimeInterval
    public let end: TimeInterval

    public var duration: TimeInterval {
        end - start
    }

    public init?(start: TimeInterval, duration: TimeInterval) {
        guard start.isFinite, duration.isFinite, duration > 0 else {
            return nil
        }
        self.start = max(start, 0)
        end = self.start + duration
    }

    public init?(start: TimeInterval, end: TimeInterval) {
        guard start.isFinite, end.isFinite, end > start else {
            return nil
        }
        self.start = max(start, 0)
        self.end = max(end, self.start)
    }

    public func clamped(_ time: TimeInterval) -> TimeInterval {
        min(max(time, start), end)
    }
}

public protocol MediaPlayerProtocol: MediaPlayback {
    var delegate: MediaPlayerDelegate? { get set }
    var view: UIView? { get }
    var playableTime: TimeInterval { get }
    var isReadyToPlay: Bool { get }
    var playbackState: MediaPlaybackState { get }
    var loadState: MediaLoadState { get }
    var isPlaying: Bool { get }
    var seekable: Bool { get }
    var seekableTimeRange: MediaPlaybackTimeRange? { get }
    //    var numberOfBytesTransferred: Int64 { get }
    var isMuted: Bool { get set }
    var allowsExternalPlayback: Bool { get set }
    var usesExternalPlaybackWhileExternalScreenIsActive: Bool { get set }
    var isExternalPlaybackActive: Bool { get }
    var playbackRate: Float { get set }
    var playbackVolume: Float { get set }
    var contentMode: UIViewContentMode { get set }
    var subtitleDataSouce: SubtitleDataSouce? { get }
    @available(macOS 12.0, iOS 15.0, tvOS 15.0, *)
    var playbackCoordinator: AVPlaybackCoordinator { get }
    @available(tvOS 14.0, *)
    var pipController: KSPictureInPictureController? { get }
    var dynamicInfo: DynamicInfo? { get }
    init(url: URL, options: KSOptions)
    func replace(url: URL, options: KSOptions)
    func play()
    func pause()
    func enterBackground()
    func enterForeground()
    func thumbnailImageAtCurrentTime() async -> CGImage?
    func tracks(mediaType: AVFoundation.AVMediaType) -> [MediaPlayerTrack]
    func select(track: some MediaPlayerTrack)
}

public extension MediaPlayerProtocol {
    var nominalFrameRate: Float {
        tracks(mediaType: .video).first { $0.isEnabled }?.nominalFrameRate ?? 0
    }

    var seekableTimeRange: MediaPlaybackTimeRange? {
        guard seekable else {
            return nil
        }
        return MediaPlaybackTimeRange(start: 0, duration: duration)
    }
}

@MainActor
public protocol MediaPlayerDelegate: AnyObject {
    func readyToPlay(player: some MediaPlayerProtocol)
    func changeLoadState(player: some MediaPlayerProtocol)
    // 缓冲加载进度，0-100
    func changeBuffering(player: some MediaPlayerProtocol, progress: Int)
    func playBack(player: some MediaPlayerProtocol, loopCount: Int)
    func finish(player: some MediaPlayerProtocol, error: Error?)
}

public protocol MediaPlayerTrack: AnyObject, CustomStringConvertible {
    var trackID: Int32 { get }
    var name: String { get }
    var languageCode: String? { get }
    var mediaType: AVFoundation.AVMediaType { get }
    var nominalFrameRate: Float { get set }
    var bitRate: Int64 { get }
    var bitDepth: Int32 { get }
    var isEnabled: Bool { get set }
    var isImageSubtitle: Bool { get }
    var rotation: Int16 { get }
    var dovi: DOVIDecoderConfigurationRecord? { get }
    var fieldOrder: FFmpegFieldOrder { get }
    var formatDescription: CMFormatDescription? { get }
}

// public extension MediaPlayerTrack: Identifiable {
//    var id: Int32 { trackID }
// }

public enum MediaPlaybackState: Int {
    case idle
    case playing
    case paused
    case seeking
    case finished
    case stopped
}

public enum MediaLoadState: Int {
    case idle
    case loading
    case playable
}

// swiftlint:disable identifier_name
public struct DOVIDecoderConfigurationRecord {
    public let dv_version_major: UInt8
    public let dv_version_minor: UInt8
    public let dv_profile: UInt8
    public let dv_level: UInt8
    public let rpu_present_flag: UInt8
    public let el_present_flag: UInt8
    public let bl_present_flag: UInt8
    public let dv_bl_signal_compatibility_id: UInt8
}

extension DOVIDecoderConfigurationRecord: CustomStringConvertible {
    public var hdrFallbackDynamicRange: DynamicRange? {
        switch dv_profile {
        case 5:
            return .dolbyVision
        case 8 where dv_bl_signal_compatibility_id == 4:
            return .hlg
        case 7:
            return .hdr10
        case 8 where dv_bl_signal_compatibility_id == 1:
            return .hdr10
        default:
            return nil
        }
    }

    public var profileDescription: String {
        switch dv_profile {
        case 0:
            return "AVC dual-layer early mobile/legacy"
        case 1:
            return "AVC single-layer mobile/legacy"
        case 2:
            return "AVC dual-layer SDR deprecated"
        case 3:
            return "AVC single-layer SDR legacy streaming"
        case 4:
            return "AVC dual-layer SDR legacy"
        case 5:
            return "HEVC single-layer modern streaming"
        case 6:
            return "HEVC dual-layer SDR legacy hybrid"
        case 7:
            return "HEVC dual-layer UHD Blu-ray"
        case 8 where dv_bl_signal_compatibility_id == 1:
            return "HEVC single-layer HDR10-compatible streaming/broadcast"
        case 8 where dv_bl_signal_compatibility_id == 2:
            return "HEVC single-layer SDR-compatible broadcast"
        case 8 where dv_bl_signal_compatibility_id == 4:
            return "HEVC single-layer HLG-compatible broadcast"
        case 8:
            return "HEVC single-layer compatibility \(dv_bl_signal_compatibility_id)"
        case 9:
            return "AVC single-layer mobile low-bandwidth"
        case 10:
            return "HEVC dual-layer studio/mezzanine"
        case 11:
            return "HEVC single-layer studio/mezzanine"
        case 12:
            return "AVC dual-layer legacy mezzanine/studio"
        case 13:
            return "AVC single-layer legacy mezzanine/studio"
        default:
            return "unclassified"
        }
    }

    public var enhancementLayerDescription: String? {
        guard el_present_flag != 0 else {
            return nil
        }
        switch dv_profile {
        case 7:
            return "enhancement layer present; MEL/FEL is not distinguished by this configuration record"
        case 10, 12:
            return "enhancement layer present; full dual-layer composition is not implemented"
        default:
            return "enhancement layer present"
        }
    }

    public var fallbackDescription: String {
        if let hdrFallbackDynamicRange {
            return hdrFallbackDynamicRange.description
        }
        switch dv_profile {
        case 2, 3, 4, 6, 8 where dv_bl_signal_compatibility_id == 2:
            return "source SDR/base metadata"
        case 7 where el_present_flag != 0:
            return "HDR10 base layer; no full enhancement-layer composition"
        case 10, 11, 12, 13:
            return "source/base metadata for studio profile"
        default:
            return "source metadata"
        }
    }

    public var description: String {
        var description = "Dolby Vision profile \(dv_profile) (\(profileDescription)), level \(dv_level), rpu \(rpu_present_flag), " +
            "el \(el_present_flag), bl \(bl_present_flag), compatibility \(dv_bl_signal_compatibility_id)"
        if let enhancementLayerDescription {
            description += ", \(enhancementLayerDescription)"
        }
        description += ", fallback \(fallbackDescription)"
        return description
    }
}

public enum FFmpegFieldOrder: UInt8 {
    case unknown = 0
    case progressive
    case tt // < Top coded_first, top displayed first
    case bb // < Bottom coded first, bottom displayed first
    case tb // < Top coded first, bottom displayed first
    case bt // < Bottom coded first, top displayed first
}

extension FFmpegFieldOrder: CustomStringConvertible {
    public var description: String {
        switch self {
        case .unknown, .progressive:
            return "progressive"
        case .tt:
            return "top first"
        case .bb:
            return "bottom first"
        case .tb:
            return "top coded first (swapped)"
        case .bt:
            return "bottom coded first (swapped)"
        }
    }
}

// swiftlint:enable identifier_name
public extension MediaPlayerTrack {
    var language: String? {
        languageCode.flatMap {
            Locale.current.localizedString(forLanguageCode: $0)
        }
    }

    var codecType: FourCharCode {
        mediaSubType.rawValue
    }

    var dynamicRange: DynamicRange? {
        if let dovi {
            return dovi.hdrFallbackDynamicRange ?? formatDescription?.dynamicRange
        } else {
            return formatDescription?.dynamicRange
        }
    }

    var colorSpace: CGColorSpace? {
        KSOptions.colorSpace(ycbcrMatrix: yCbCrMatrix as CFString?, transferFunction: transferFunction as CFString?)
    }

    var mediaSubType: CMFormatDescription.MediaSubType {
        formatDescription?.mediaSubType ?? .boxed
    }

    var audioStreamBasicDescription: AudioStreamBasicDescription? {
        formatDescription?.audioStreamBasicDescription
    }

    var naturalSize: CGSize {
        formatDescription?.naturalSize ?? .zero
    }

    var colorPrimaries: String? {
        formatDescription?.colorPrimaries
    }

    var transferFunction: String? {
        formatDescription?.transferFunction
    }

    var yCbCrMatrix: String? {
        formatDescription?.yCbCrMatrix
    }
}

public extension CMFormatDescription {
    var dynamicRange: DynamicRange {
        let contentRange: DynamicRange
        if codecType.string == "dvhe" || codecType == kCMVideoCodecType_DolbyVisionHEVC {
            contentRange = .dolbyVision
        } else if bitDepth == 10 || transferFunction == kCVImageBufferTransferFunction_SMPTE_ST_2084_PQ as String { /// HDR
            contentRange = .hdr10
        } else if transferFunction == kCVImageBufferTransferFunction_ITU_R_2100_HLG as String { /// HLG
            contentRange = .hlg
        } else {
            contentRange = .sdr
        }
        return contentRange
    }

    var bitDepth: Int32 {
        codecType.bitDepth
    }

    var codecType: FourCharCode {
        mediaSubType.rawValue
    }

    var colorPrimaries: String? {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCVImageBufferColorPrimariesKey] as? String
        } else {
            return nil
        }
    }

    var transferFunction: String? {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCVImageBufferTransferFunctionKey] as? String
        } else {
            return nil
        }
    }

    var yCbCrMatrix: String? {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCVImageBufferYCbCrMatrixKey] as? String
        } else {
            return nil
        }
    }

    var naturalSize: CGSize {
        let aspectRatio = aspectRatio
        return CGSize(width: Int(dimensions.width), height: Int(CGFloat(dimensions.height) * aspectRatio.height / aspectRatio.width))
    }

    var aspectRatio: CGSize {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            if let ratio = dictionary[kCVImageBufferPixelAspectRatioKey] as? NSDictionary,
               let horizontal = (ratio[kCVImageBufferPixelAspectRatioHorizontalSpacingKey] as? NSNumber)?.intValue,
               let vertical = (ratio[kCVImageBufferPixelAspectRatioVerticalSpacingKey] as? NSNumber)?.intValue,
               horizontal > 0, vertical > 0
            {
                return CGSize(width: horizontal, height: vertical)
            }
        }
        return CGSize(width: 1, height: 1)
    }

    var depth: Int32 {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCMFormatDescriptionExtension_Depth] as? Int32 ?? 24
        } else {
            return 24
        }
    }

    var fullRangeVideo: Bool {
        if let dictionary = CMFormatDescriptionGetExtensions(self) as NSDictionary? {
            return dictionary[kCMFormatDescriptionExtension_FullRangeVideo] as? Bool ?? false
        } else {
            return false
        }
    }
}

func setHttpProxy() {
    guard KSOptions.useSystemHTTPProxy else {
        return
    }
    guard let proxySettings = CFNetworkCopySystemProxySettings()?.takeUnretainedValue() as? NSDictionary else {
        unsetenv("http_proxy")
        return
    }
    guard let proxyHost = proxySettings[kCFNetworkProxiesHTTPProxy] as? String, let proxyPort = proxySettings[kCFNetworkProxiesHTTPPort] as? Int else {
        unsetenv("http_proxy")
        return
    }
    let httpProxy = "http://\(proxyHost):\(proxyPort)"
    setenv("http_proxy", httpProxy, 0)
}

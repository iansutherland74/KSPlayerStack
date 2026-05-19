//
//  PlayerDefines.swift
//  KSPlayer
//
//  Created by kintan on 2018/3/9.
//

import AVFoundation
import CoreMedia
import CoreServices
#if canImport(UIKit)
import UIKit

public extension KSOptions {
    @MainActor
    static var windowScene: UIWindowScene? {
        UIApplication.shared.connectedScenes.first as? UIWindowScene
    }

    @MainActor
    static var sceneSize: CGSize {
        let window = windowScene?.windows.first
        return window?.bounds.size ?? .zero
    }
}
#else
import AppKit
import SwiftUI

public typealias UIView = NSView
public typealias UIPasteboard = NSPasteboard
public extension KSOptions {
    static var sceneSize: CGSize {
        NSScreen.main?.frame.size ?? .zero
    }
}
#endif

// extension MediaPlayerTrack {
//    static func == (lhs: Self, rhs: Self) -> Bool {
//        lhs.trackID == rhs.trackID
//    }
// }

public enum DynamicRange: Int32 {
    case sdr = 0
    case hdr10 = 2
    case hlg = 3
    case dolbyVision = 5

    #if canImport(UIKit)
    var hdrMode: AVPlayer.HDRMode {
        switch self {
        case .sdr:
            return AVPlayer.HDRMode(rawValue: 0)
        case .hdr10:
            return .hdr10 // 2
        case .hlg:
            return .hlg // 1
        case .dolbyVision:
            return .dolbyVision // 4
        }
    }
    #endif
    public static var availableHDRModes: [DynamicRange] {
        #if os(macOS)
        if NSScreen.main?.maximumPotentialExtendedDynamicRangeColorComponentValue ?? 1.0 > 1.0 {
            return [.hdr10]
        } else {
            return [.sdr]
        }
        #else
        let availableHDRModes = AVPlayer.availableHDRModes
        if availableHDRModes == AVPlayer.HDRMode(rawValue: 0) {
            return [.sdr]
        } else {
            var modes = [DynamicRange]()
            if availableHDRModes.contains(.dolbyVision) {
                modes.append(.dolbyVision)
            }
            if availableHDRModes.contains(.hdr10) {
                modes.append(.hdr10)
            }
            if availableHDRModes.contains(.hlg) {
                modes.append(.hlg)
            }
            return modes
        }
        #endif
    }
}

extension DynamicRange: CustomStringConvertible {
    public var description: String {
        switch self {
        case .sdr:
            return "SDR"
        case .hdr10:
            return "HDR10"
        case .hlg:
            return "HLG"
        case .dolbyVision:
            return "Dolby Vision"
        }
    }
}

extension DynamicRange {
    var isHDR: Bool {
        self != .sdr
    }

    var colorPrimaries: CFString {
        switch self {
        case .sdr:
            return kCVImageBufferColorPrimaries_ITU_R_709_2
        case .hdr10, .hlg, .dolbyVision:
            return kCVImageBufferColorPrimaries_ITU_R_2020
        }
    }

    var transferFunction: CFString {
        switch self {
        case .sdr:
            return kCVImageBufferTransferFunction_ITU_R_709_2
        case .hdr10:
            return kCVImageBufferTransferFunction_SMPTE_ST_2084_PQ
        case .dolbyVision:
            return kCVImageBufferTransferFunction_SMPTE_ST_2084_PQ
        case .hlg:
            return kCVImageBufferTransferFunction_ITU_R_2100_HLG
        }
    }

    var yCbCrMatrix: CFString {
        switch self {
        case .sdr:
            return kCVImageBufferYCbCrMatrix_ITU_R_709_2
        case .hdr10, .hlg, .dolbyVision:
            return kCVImageBufferYCbCrMatrix_ITU_R_2020
        }
    }
}

@MainActor
public enum DisplayEnum {
    case plane
    // swiftlint:disable identifier_name
    case vr
    // swiftlint:enable identifier_name
    case vrBox
}

public struct VideoAdaptationState {
    public struct BitRateState {
        let bitRate: Int64
        let time: TimeInterval
    }

    public let bitRates: [Int64]
    public let duration: TimeInterval
    public internal(set) var fps: Float
    public internal(set) var bitRateStates: [BitRateState]
    public internal(set) var currentPlaybackTime: TimeInterval = 0
    public internal(set) var isPlayable: Bool = false
    public internal(set) var loadedCount: Int = 0
}

public struct KSAdaptiveBitrateSwitchingPolicy: Equatable {
    public enum Decision: Equatable {
        case stay
        case switchToLower
        case switchToHigher
    }

    /// Minimum seconds between automatic or manual definition changes.
    public var minimumSwitchInterval: TimeInterval
    /// Buffer seconds that allow an upgrade after stable playback.
    public var upgradeBufferThreshold: TimeInterval
    /// Buffer seconds that trigger a downgrade before playback stalls.
    public var downgradeBufferThreshold: TimeInterval
    /// Consecutive rebuffer recoveries that trigger a downgrade.
    public var downgradeRebufferCount: Int
    /// Seconds of healthy buffer required before upgrading.
    public var upgradeObservationDuration: TimeInterval
    /// Live/DVR streams default to avoiding automatic upgrades to preserve latency.
    public var allowsLiveUpgrades: Bool
    /// Minimum seconds between network throughput samples.
    public var minimumThroughputSampleInterval: TimeInterval
    /// Throughput must exceed the target variant by this factor before upgrading.
    public var upgradeThroughputSafetyFactor: Double
    /// Current throughput below this fraction of the active variant triggers a downgrade.
    public var downgradeThroughputSafetyFactor: Double
    /// Low-latency live streams run with intentionally small buffers, so use a smaller downgrade threshold.
    public var lowLatencyLiveDowngradeBufferThreshold: TimeInterval

    public init(minimumSwitchInterval: TimeInterval = 20,
                upgradeBufferThreshold: TimeInterval = 12,
                downgradeBufferThreshold: TimeInterval = 2,
                downgradeRebufferCount: Int = 1,
                upgradeObservationDuration: TimeInterval = 30,
                allowsLiveUpgrades: Bool = false,
                minimumThroughputSampleInterval: TimeInterval = 1,
                upgradeThroughputSafetyFactor: Double = 1.35,
                downgradeThroughputSafetyFactor: Double = 0.9,
                lowLatencyLiveDowngradeBufferThreshold: TimeInterval = 0.2)
    {
        self.minimumSwitchInterval = minimumSwitchInterval
        self.upgradeBufferThreshold = upgradeBufferThreshold
        self.downgradeBufferThreshold = downgradeBufferThreshold
        self.downgradeRebufferCount = downgradeRebufferCount
        self.upgradeObservationDuration = upgradeObservationDuration
        self.allowsLiveUpgrades = allowsLiveUpgrades
        self.minimumThroughputSampleInterval = minimumThroughputSampleInterval
        self.upgradeThroughputSafetyFactor = upgradeThroughputSafetyFactor
        self.downgradeThroughputSafetyFactor = downgradeThroughputSafetyFactor
        self.lowLatencyLiveDowngradeBufferThreshold = lowLatencyLiveDowngradeBufferThreshold
    }

    public func decision(definitionRank: Int, definitionCount: Int, bufferAhead: TimeInterval?, rebufferCount: Int, stableBufferDuration: TimeInterval,
                         secondsSinceLastSwitch: TimeInterval?, isLive: Bool, isAdaptiveStreamingManifest: Bool) -> Decision
    {
        let targetRank = targetDefinitionRank(
            definitionRank: definitionRank,
            definitionBandwidths: Array(repeating: nil, count: definitionCount),
            bufferAhead: bufferAhead,
            rebufferCount: rebufferCount,
            stableBufferDuration: stableBufferDuration,
            secondsSinceLastSwitch: secondsSinceLastSwitch,
            estimatedThroughput: nil,
            isLive: isLive,
            isLowLatencyLive: false,
            isAdaptiveStreamingManifest: isAdaptiveStreamingManifest
        )
        guard let targetRank else {
            return .stay
        }
        if targetRank < definitionRank {
            return .switchToLower
        }
        if targetRank > definitionRank {
            return .switchToHigher
        }
        return .stay
    }

    public func targetDefinitionRank(definitionRank: Int, definitionBandwidths: [Int64?], bufferAhead: TimeInterval?, rebufferCount: Int,
                                     stableBufferDuration: TimeInterval, secondsSinceLastSwitch: TimeInterval?, estimatedThroughput: Int64?,
                                     isLive: Bool, isLowLatencyLive: Bool = false, isAdaptiveStreamingManifest: Bool) -> Int?
    {
        guard definitionBandwidths.count > 1,
              definitionBandwidths.indices.contains(definitionRank),
              !isAdaptiveStreamingManifest
        else {
            return nil
        }
        if let secondsSinceLastSwitch, secondsSinceLastSwitch < minimumSwitchInterval {
            return nil
        }

        let normalizedRebufferCount = max(downgradeRebufferCount, 1)
        let normalizedEstimatedThroughput = estimatedThroughput.flatMap { $0 > 0 ? $0 : nil }
        let currentBandwidth = normalizedBandwidth(definitionBandwidths[definitionRank])
        let downgradeBufferLimit = isLowLatencyLive
            ? max(0, min(lowLatencyLiveDowngradeBufferThreshold, downgradeBufferThreshold))
            : downgradeBufferThreshold

        if definitionRank > 0, rebufferCount >= normalizedRebufferCount {
            return sustainableDowngradeRank(
                from: definitionRank,
                bandwidths: definitionBandwidths,
                estimatedThroughput: normalizedEstimatedThroughput
            )
        }

        if definitionRank > 0,
           let bufferAhead,
           bufferAhead.isFinite,
           bufferAhead <= downgradeBufferLimit
        {
            return sustainableDowngradeRank(
                from: definitionRank,
                bandwidths: definitionBandwidths,
                estimatedThroughput: normalizedEstimatedThroughput
            )
        }

        if definitionRank > 0,
           let normalizedEstimatedThroughput,
           let currentBandwidth,
           Double(normalizedEstimatedThroughput) < Double(currentBandwidth) * normalizedSafetyFactor(downgradeThroughputSafetyFactor, defaultValue: 0.9)
        {
            return sustainableDowngradeRank(
                from: definitionRank,
                bandwidths: definitionBandwidths,
                estimatedThroughput: normalizedEstimatedThroughput
            )
        }

        guard definitionRank < definitionBandwidths.count - 1,
              !isLive || allowsLiveUpgrades,
              let bufferAhead,
              bufferAhead.isFinite,
              bufferAhead >= upgradeBufferThreshold,
              stableBufferDuration >= upgradeObservationDuration
        else {
            return nil
        }

        return sustainableUpgradeRank(
            from: definitionRank,
            bandwidths: definitionBandwidths,
            estimatedThroughput: normalizedEstimatedThroughput
        )
    }

    private func sustainableDowngradeRank(from definitionRank: Int, bandwidths: [Int64?], estimatedThroughput: Int64?) -> Int {
        guard let estimatedThroughput else {
            return definitionRank - 1
        }
        let safetyFactor = normalizedSafetyFactor(upgradeThroughputSafetyFactor, defaultValue: 1.35)
        for rank in stride(from: definitionRank - 1, through: 0, by: -1) {
            guard let bandwidth = normalizedBandwidth(bandwidths[rank]) else {
                return definitionRank - 1
            }
            if Double(estimatedThroughput) >= Double(bandwidth) * safetyFactor {
                return rank
            }
        }
        return 0
    }

    private func sustainableUpgradeRank(from definitionRank: Int, bandwidths: [Int64?], estimatedThroughput: Int64?) -> Int? {
        guard let estimatedThroughput else {
            return definitionRank + 1
        }
        let safetyFactor = normalizedSafetyFactor(upgradeThroughputSafetyFactor, defaultValue: 1.35)
        var targetRank: Int?
        for rank in (definitionRank + 1) ..< bandwidths.count {
            guard let bandwidth = normalizedBandwidth(bandwidths[rank]) else {
                return definitionRank + 1
            }
            if Double(estimatedThroughput) >= Double(bandwidth) * safetyFactor {
                targetRank = rank
            } else {
                break
            }
        }
        return targetRank
    }

    private func normalizedBandwidth(_ bandwidth: Int64?) -> Int64? {
        guard let bandwidth, bandwidth > 0 else {
            return nil
        }
        return bandwidth
    }

    private func normalizedSafetyFactor(_ value: Double, defaultValue: Double) -> Double {
        guard value.isFinite, value > 0 else {
            return defaultValue
        }
        return value
    }
}

public struct KSAdaptiveBitrateSwitchingDiagnostic: Equatable {
    public let currentDefinitionIndex: Int
    public let currentDefinition: String
    public let currentBandwidth: Int64?
    public let estimatedThroughput: Int64?
    public let bufferAhead: TimeInterval?
    public let rebufferCount: Int
    public let stableBufferDuration: TimeInterval
    public let secondsSinceLastSwitch: TimeInterval?
    public let isLive: Bool
    public let isLowLatencyLive: Bool
    public let isAdaptiveStreamingManifest: Bool
    public let decision: KSAdaptiveBitrateSwitchingPolicy.Decision
}

struct KSAdaptiveBitrateThroughputEstimator: Equatable {
    private var lastBytesRead: Int64?
    private var lastSampleTime: TimeInterval?
    private(set) var estimatedBitsPerSecond: Int64?

    mutating func reset() {
        lastBytesRead = nil
        lastSampleTime = nil
        estimatedBitsPerSecond = nil
    }

    mutating func sample(bytesRead: Int64?, at time: TimeInterval, minimumInterval: TimeInterval) -> Int64? {
        guard let bytesRead, bytesRead >= 0, time.isFinite else {
            return estimatedBitsPerSecond
        }
        guard let previousBytesRead = lastBytesRead, let previousSampleTime = lastSampleTime else {
            lastBytesRead = bytesRead
            lastSampleTime = time
            return estimatedBitsPerSecond
        }

        let elapsed = time - previousSampleTime
        let normalizedMinimumInterval = minimumInterval.isFinite ? max(minimumInterval, 0) : 1
        guard elapsed >= normalizedMinimumInterval else {
            return estimatedBitsPerSecond
        }
        defer {
            lastBytesRead = bytesRead
            lastSampleTime = time
        }
        let byteDelta = bytesRead - previousBytesRead
        guard byteDelta > 0 else {
            if byteDelta < 0 {
                estimatedBitsPerSecond = nil
            }
            return estimatedBitsPerSecond
        }
        let currentBitsPerSecond = Int64(Double(byteDelta * 8) / elapsed)
        if let existingEstimate = estimatedBitsPerSecond {
            estimatedBitsPerSecond = Int64(Double(existingEstimate) * 0.7 + Double(currentBitsPerSecond) * 0.3)
        } else {
            estimatedBitsPerSecond = currentBitsPerSecond
        }
        return estimatedBitsPerSecond
    }
}

public enum ClockProcessType: Equatable {
    case remain
    case next
    case dropNextFrame
    case dropNextPacket
    case dropGOPPacket
    case flush
    case seek
}

// 缓冲情况
public protocol CapacityProtocol {
    var fps: Float { get }
    var packetCount: Int { get }
    var frameCount: Int { get }
    var frameMaxCount: Int { get }
    var isEndOfFile: Bool { get }
    var mediaType: AVFoundation.AVMediaType { get }
}

extension CapacityProtocol {
    var loadedTime: TimeInterval {
        TimeInterval(packetCount + frameCount) / TimeInterval(fps)
    }
}

public struct LoadingState {
    public let loadedTime: TimeInterval
    public let progress: TimeInterval
    public let packetCount: Int
    public let frameCount: Int
    public let isEndOfFile: Bool
    public let isPlayable: Bool
    public let isFirst: Bool
    public let isSeek: Bool
}

public let KSPlayerErrorDomain = "KSPlayerErrorDomain"

public enum KSPlayerErrorCode: Int {
    case unknown
    case formatCreate
    case formatOpenInput
    case formatOutputCreate
    case formatWriteHeader
    case formatFindStreamInfo
    case readFrame
    case codecContextCreate
    case codecContextSetParam
    case codecContextFindDecoder
    case codesContextOpen
    case codecVideoSendPacket
    case codecAudioSendPacket
    case codecVideoReceiveFrame
    case codecAudioReceiveFrame
    case auidoSwrInit
    case codecSubtitleSendPacket
    case videoTracksUnplayable
    case subtitleUnEncoding
    case subtitleUnParse
    case subtitleFormatUnSupport
    case subtitleParamsEmpty
}

extension KSPlayerErrorCode: CustomStringConvertible {
    public var description: String {
        switch self {
        case .formatCreate:
            return "avformat_alloc_context return nil"
        case .formatOpenInput:
            return "avformat can't open input"
        case .formatOutputCreate:
            return "avformat_alloc_output_context2 fail"
        case .formatWriteHeader:
            return "avformat_write_header fail"
        case .formatFindStreamInfo:
            return "avformat_find_stream_info return nil"
        case .codecContextCreate:
            return "avcodec_alloc_context3 return nil"
        case .codecContextSetParam:
            return "avcodec can't set parameters to context"
        case .codesContextOpen:
            return "codesContext can't Open"
        case .codecVideoReceiveFrame:
            return "avcodec can't receive video frame"
        case .codecAudioReceiveFrame:
            return "avcodec can't receive audio frame"
        case .videoTracksUnplayable:
            return "VideoTracks are not even playable."
        case .codecSubtitleSendPacket:
            return "avcodec can't decode subtitle"
        case .subtitleUnEncoding:
            return "Subtitle encoding format is not supported."
        case .subtitleUnParse:
            return "Subtitle parsing error"
        case .subtitleFormatUnSupport:
            return "Current subtitle format is not supported"
        case .subtitleParamsEmpty:
            return "Subtitle Params is empty"
        case .auidoSwrInit:
            return "swr_init swrContext fail"
        default:
            return "unknown"
        }
    }
}

extension NSError {
    convenience init(errorCode: KSPlayerErrorCode, userInfo: [String: Any] = [:]) {
        var userInfo = userInfo
        userInfo[NSLocalizedDescriptionKey] = errorCode.description
        self.init(domain: KSPlayerErrorDomain, code: errorCode.rawValue, userInfo: userInfo)
    }

    convenience init(description: String) {
        var userInfo = [String: Any]()
        userInfo[NSLocalizedDescriptionKey] = description
        self.init(domain: KSPlayerErrorDomain, code: 0, userInfo: userInfo)
    }
}

#if !SWIFT_PACKAGE
extension Bundle {
    static let module = Bundle(for: KSPlayerLayer.self).path(forResource: "KSPlayer_KSPlayer", ofType: "bundle").flatMap { Bundle(path: $0) } ?? Bundle.main
}
#endif

public enum TimeType {
    case min
    case hour
    case minOrHour
    case millisecond
}

public extension TimeInterval {
    func toString(for type: TimeType) -> String {
        Int(ceil(self)).toString(for: type)
    }
}

public extension Int {
    func toString(for type: TimeType) -> String {
        var second = self
        var min = second / 60
        second -= min * 60
        switch type {
        case .min:
            return String(format: "%02d:%02d", min, second)
        case .hour:
            let hour = min / 60
            min -= hour * 60
            return String(format: "%d:%02d:%02d", hour, min, second)
        case .minOrHour:
            let hour = min / 60
            if hour > 0 {
                min -= hour * 60
                return String(format: "%d:%02d:%02d", hour, min, second)
            } else {
                return String(format: "%02d:%02d", min, second)
            }
        case .millisecond:
            var time = self * 100
            let millisecond = time % 100
            time /= 100
            let sec = time % 60
            time /= 60
            let min = time % 60
            time /= 60
            let hour = time % 60
            if hour > 0 {
                return String(format: "%d:%02d:%02d.%02d", hour, min, sec, millisecond)
            } else {
                return String(format: "%02d:%02d.%02d", min, sec, millisecond)
            }
        }
    }
}

public extension FixedWidthInteger {
    var kmFormatted: String {
        Double(self).kmFormatted
    }
}

open class AbstractAVIOContext {
    let bufferSize: Int32
    let writable: Bool
    public init(bufferSize: Int32 = 32 * 1024, writable: Bool = false) {
        self.bufferSize = bufferSize
        self.writable = writable
    }

    open func read(buffer _: UnsafePointer<UInt8>?, size: Int32) -> Int32 {
        size
    }

    open func write(buffer _: UnsafePointer<UInt8>?, size: Int32) -> Int32 {
        size
    }

    /**
     #define SEEK_SET        0       /* set file offset to offset */
     #define SEEK_CUR        1       /* set file offset to current plus offset */
     #define SEEK_END        2       /* set file offset to EOF plus offset */
     */
    open func seek(offset: Int64, whence _: Int32) -> Int64 {
        offset
    }

    open func fileSize() -> Int64 {
        -1
    }

    open func close() {}
    deinit {}
}

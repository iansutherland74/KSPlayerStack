//
//  KSVideoFrameOutput.swift
//  KSPlayer
//

import Foundation
@preconcurrency import CoreMedia
@preconcurrency import CoreVideo

public typealias KSVideoOutputHandler = @Sendable (CVPixelBuffer) -> Void

public enum KSVideoFrameOutputMetadata {
    public static var presentationTimeSecondsKey: CFString {
        "KSPlayer.presentationTimeSeconds" as CFString
    }
}

public final class KSVideoFrameOutput: @unchecked Sendable {
    public enum DropPolicy: Equatable, Sendable {
        /// Keep the newest decoded frame when the callback queue is busy.
        case keepLatest
        /// Preserve queued frames and drop newly decoded frames while the callback queue is full.
        case dropNewest
    }

    public enum CallbackQualityOfService: Equatable, Sendable {
        case userInteractive
        case userInitiated
        case utility
        case background

        fileprivate var qos: DispatchQoS {
            switch self {
            case .userInteractive:
                return .userInteractive
            case .userInitiated:
                return .userInitiated
            case .utility:
                return .utility
            case .background:
                return .background
            }
        }
    }

    public struct Configuration: Equatable, Sendable {
        public let maximumBufferedFrameCount: Int
        public let dropPolicy: DropPolicy
        public let callbackQualityOfService: CallbackQualityOfService
        public let callbackQueueLabel: String

        public init(
            maximumBufferedFrameCount: Int = 1,
            dropPolicy: DropPolicy = .keepLatest,
            callbackQualityOfService: CallbackQualityOfService = .userInitiated,
            callbackQueueLabel: String = "KSPlayer.videoOutput"
        ) {
            self.maximumBufferedFrameCount = max(1, maximumBufferedFrameCount)
            self.dropPolicy = dropPolicy
            self.callbackQualityOfService = callbackQualityOfService
            self.callbackQueueLabel = callbackQueueLabel.isEmpty ? "KSPlayer.videoOutput" : callbackQueueLabel
        }
    }

    private final class RetainedPixelBuffer: @unchecked Sendable {
        let pixelBuffer: CVPixelBuffer

        init(_ pixelBuffer: CVPixelBuffer) {
            self.pixelBuffer = pixelBuffer
        }
    }

    public let configuration: Configuration
    private let handler: KSVideoOutputHandler
    private let callbackQueue: DispatchQueue
    private let lock = NSLock()
    private var pendingFrames = [RetainedPixelBuffer]()
    private var isDraining = false
    private var droppedCount = 0

    public init(configuration: Configuration = Configuration(), handler: @escaping KSVideoOutputHandler) {
        self.configuration = configuration
        self.handler = handler
        callbackQueue = DispatchQueue(
            label: configuration.callbackQueueLabel,
            qos: configuration.callbackQualityOfService.qos
        )
    }

    public var droppedFrameCount: Int {
        lock.lock()
        let count = droppedCount
        lock.unlock()
        return count
    }

    public var pendingFrameCount: Int {
        lock.lock()
        let count = pendingFrames.count
        lock.unlock()
        return count
    }

    func enqueue(_ pixelBuffer: CVPixelBuffer, presentationTime: CMTime? = nil) {
        if let presentationTime {
            let seconds = CMTimeGetSeconds(presentationTime)
            if seconds.isFinite {
                CVBufferSetAttachment(
                    pixelBuffer,
                    KSVideoFrameOutputMetadata.presentationTimeSecondsKey,
                    NSNumber(value: seconds),
                    .shouldNotPropagate
                )
            }
        }
        let retainedFrame = RetainedPixelBuffer(pixelBuffer)
        var shouldScheduleDrain = false

        lock.lock()
        if pendingFrames.count >= configuration.maximumBufferedFrameCount {
            switch configuration.dropPolicy {
            case .keepLatest:
                pendingFrames.removeFirst()
                droppedCount += 1
            case .dropNewest:
                droppedCount += 1
                lock.unlock()
                return
            }
        }
        pendingFrames.append(retainedFrame)
        if !isDraining {
            isDraining = true
            shouldScheduleDrain = true
        }
        lock.unlock()

        if shouldScheduleDrain {
            callbackQueue.async { [weak self] in
                self?.drain()
            }
        }
    }

    func flush() {
        lock.lock()
        pendingFrames.removeAll()
        lock.unlock()
    }

    private func drain() {
        while true {
            let frame: RetainedPixelBuffer?
            lock.lock()
            if pendingFrames.isEmpty {
                isDraining = false
                lock.unlock()
                return
            }
            frame = pendingFrames.removeFirst()
            lock.unlock()

            if let frame {
                autoreleasepool {
                    handler(frame.pixelBuffer)
                }
            }
        }
    }
}

protocol DecodedVideoFrameOutputConfigurable: AnyObject {
    var decodedVideoFrameOutput: KSVideoFrameOutput? { get set }
}

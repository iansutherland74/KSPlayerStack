//
//  AudioRecognize.swift
//  KSPlayer
//
//  Created by kintan on 2023/9/23.
//

import Foundation

public protocol AudioRecognize: SubtitleInfo {
    func append(frame: AudioFrame)
    func reset()
}

public extension AudioRecognize {
    func reset() {}
}

public enum OfflineSubtitleDisplayMode: Sendable {
    case transcription
    case translation
    case bilingual(separator: String)
}

public typealias OfflineSubtitleWord = SubtitleWordTiming

public struct OfflineSubtitleAudioFrame: Sendable {
    public let startTime: TimeInterval
    public let duration: TimeInterval
    public let sampleRate: Double
    public let channelCount: Int
    public let samples: [ContiguousArray<Float>]

    public init(startTime: TimeInterval, duration: TimeInterval, sampleRate: Double, channelCount: Int, samples: [ContiguousArray<Float>]) {
        self.startTime = startTime
        self.duration = duration
        self.sampleRate = sampleRate
        self.channelCount = channelCount
        self.samples = samples
    }
}

public struct OfflineSubtitleSegment: Sendable {
    public let identifier: String?
    public let start: TimeInterval
    public let end: TimeInterval
    public let text: String
    public let translation: String?
    public let words: [OfflineSubtitleWord]

    public init(identifier: String? = nil, start: TimeInterval, end: TimeInterval, text: String, translation: String? = nil, words: [OfflineSubtitleWord] = []) {
        self.identifier = identifier
        self.start = start
        self.end = end
        self.text = text
        self.translation = translation
        self.words = words
    }
}

public protocol OfflineSubtitleGenerationProvider: Sendable {
    func process(frame: OfflineSubtitleAudioFrame) async throws -> [OfflineSubtitleSegment]
    func reset() async
}

public extension OfflineSubtitleGenerationProvider {
    func reset() async {}
}

private actor OfflineSubtitleGenerationWorker {
    private let provider: any OfflineSubtitleGenerationProvider

    init(provider: any OfflineSubtitleGenerationProvider) {
        self.provider = provider
    }

    func process(frame: OfflineSubtitleAudioFrame) async throws -> [OfflineSubtitleSegment] {
        try await provider.process(frame: frame)
    }

    func reset() async {
        await provider.reset()
    }
}

public final class OfflineSubtitleGenerator: AudioRecognize, @unchecked Sendable {
    public let subtitleID: String
    public let name: String
    public var delay: TimeInterval {
        get {
            lock.locked { _delay }
        }
        set {
            lock.locked { _delay = newValue }
        }
    }

    public var isEnabled: Bool {
        get {
            lock.locked { _isEnabled }
        }
        set {
            lock.locked { _isEnabled = newValue }
        }
    }

    public var displayMode: OfflineSubtitleDisplayMode {
        get {
            lock.locked { _displayMode }
        }
        set {
            lock.locked { _displayMode = newValue }
        }
    }

    private let worker: OfflineSubtitleGenerationWorker
    private let lock = NSLock()
    private var _delay: TimeInterval = 0
    private var _isEnabled = false
    private var _displayMode: OfflineSubtitleDisplayMode
    private var parts = [SubtitlePart]()
    private var generation = 0
    private let maxSegmentCount: Int

    public init(
        subtitleID: String = "ksplayer.offline-subtitle",
        name: String = NSLocalizedString("Offline AI Subtitles", comment: ""),
        provider: any OfflineSubtitleGenerationProvider,
        displayMode: OfflineSubtitleDisplayMode = .transcription,
        maxSegmentCount: Int = 500
    ) {
        self.subtitleID = subtitleID
        self.name = name
        self.worker = OfflineSubtitleGenerationWorker(provider: provider)
        _displayMode = displayMode
        self.maxSegmentCount = max(1, maxSegmentCount)
    }

    public func append(frame: AudioFrame) {
        let generation = lock.locked { () -> Int? in
            guard _isEnabled else {
                return nil
            }
            return self.generation
        }
        guard let generation, let audioFrame = OfflineSubtitleAudioFrame(frame: frame) else {
            return
        }
        Task { [weak self, worker] in
            do {
                let segments = try await worker.process(frame: audioFrame)
                guard self?.currentGeneration == generation else {
                    return
                }
                self?.append(segments: segments)
            } catch {
                KSLog(error)
            }
        }
    }

    public func append(segments: [OfflineSubtitleSegment]) {
        guard !segments.isEmpty else {
            return
        }
        lock.locked {
            let displayMode = _displayMode
            for segment in segments where segment.end > segment.start && !segment.text.isEmpty {
                let part = SubtitlePart(segment.start, segment.end, displayText(for: segment, displayMode: displayMode), wordTimings: segment.words)
                if let identifier = segment.identifier {
                    parts.removeAll { $0.identifier == identifier }
                    part.identifier = identifier
                } else {
                    parts.removeAll {
                        abs($0.start - segment.start) < 0.05 && abs($0.end - segment.end) < 0.05
                    }
                }
                parts.append(part)
            }
            parts.sort()
            if parts.count > maxSegmentCount {
                parts.removeFirst(parts.count - maxSegmentCount)
            }
        }
    }

    public func search(for time: TimeInterval) -> [SubtitlePart] {
        lock.locked {
            parts.filter { $0 == time }
        }
    }

    public func reset() {
        lock.locked {
            generation += 1
            parts.removeAll()
        }
        Task { [worker] in
            await worker.reset()
        }
    }

    private func displayText(for segment: OfflineSubtitleSegment, displayMode: OfflineSubtitleDisplayMode) -> String {
        switch displayMode {
        case .transcription:
            return segment.text
        case .translation:
            return segment.translation ?? segment.text
        case let .bilingual(separator):
            if let translation = segment.translation, !translation.isEmpty, translation != segment.text {
                return segment.text + separator + translation
            } else {
                return segment.text
            }
        }
    }

    private var currentGeneration: Int {
        lock.locked {
            generation
        }
    }
}

private extension OfflineSubtitleAudioFrame {
    init?(frame: AudioFrame) {
        let samples = frame.toFloat()
        guard !samples.isEmpty else {
            return nil
        }
        let startTime = frame.seconds
        let sampleRate = frame.audioFormat.sampleRate
        let duration: TimeInterval
        if frame.duration > 0 {
            duration = frame.timebase.cmtime(for: frame.duration).seconds
        } else if sampleRate > 0 {
            duration = TimeInterval(frame.numberOfSamples) / sampleRate
        } else {
            duration = 0
        }
        self.init(
            startTime: startTime,
            duration: duration,
            sampleRate: sampleRate,
            channelCount: Int(frame.audioFormat.channelCount),
            samples: samples
        )
    }
}

private extension NSLock {
    func locked<Result>(_ body: () throws -> Result) rethrows -> Result {
        lock()
        defer {
            unlock()
        }
        return try body()
    }
}

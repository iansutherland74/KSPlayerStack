import AVFoundation
import Foundation

protocol MemorySeekCacheItem: ObjectQueueItem {
    var memorySeekCacheTrackID: Int32 { get }
    var memorySeekCacheMediaType: AVMediaType { get }
    var memorySeekCacheIsKeyFrame: Bool { get }
    func makeMemorySeekCacheCopy() -> (any MemorySeekCacheItem)?
}

final class MemorySeekCache<Item: MemorySeekCacheItem> {
    private struct Entry {
        let item: Item
        let seconds: TimeInterval
        let byteSize: Int
        let insertionIndex: Int
    }

    private var entries = [Entry]()
    private var nextInsertionIndex = 0
    private var _totalByteSize = 0
    private let lock = NSLock()
    var totalByteSize: Int {
        lock.lock()
        defer { lock.unlock() }
        return _totalByteSize
    }
    var maxDuration: TimeInterval
    var maxByteSize: Int

    init(maxDuration: TimeInterval = 0, maxByteSize: Int = 0) {
        self.maxDuration = maxDuration
        self.maxByteSize = maxByteSize
    }

    var isEmpty: Bool {
        lock.lock()
        defer { lock.unlock() }
        return entries.isEmpty
    }

    var timeRange: ClosedRange<TimeInterval>? {
        lock.lock()
        defer { lock.unlock() }
        return unlockedTimeRange
    }

    private var unlockedTimeRange: ClosedRange<TimeInterval>? {
        guard let first = entries.first else {
            return nil
        }
        var lower = first.seconds
        var upper = first.seconds
        for entry in entries.dropFirst() {
            lower = min(lower, entry.seconds)
            upper = max(upper, entry.seconds)
        }
        return lower ... upper
    }

    func store(_ item: Item) {
        guard maxDuration > 0, maxByteSize > 0, item.seconds.isFinite, item.size > 0, let copy = item.makeMemorySeekCacheCopy() as? Item else {
            return
        }
        let byteSize = Int(item.size)
        lock.lock()
        defer { lock.unlock() }
        entries.append(Entry(item: copy, seconds: item.seconds, byteSize: byteSize, insertionIndex: nextInsertionIndex))
        nextInsertionIndex += 1
        _totalByteSize += byteSize
        trim()
    }

    func packets(for time: TimeInterval, requiredTrackIDs: Set<Int32>) -> [Item]? {
        guard time.isFinite, !requiredTrackIDs.isEmpty else {
            return nil
        }
        lock.lock()
        defer { lock.unlock() }
        guard !entries.isEmpty else {
            return nil
        }
        guard let range = unlockedTimeRange, range.contains(time) else {
            return nil
        }

        let requiredEntries = entries.filter { requiredTrackIDs.contains($0.item.memorySeekCacheTrackID) }
        guard !requiredEntries.isEmpty else {
            return nil
        }

        let videoEntries = requiredEntries.filter { $0.item.memorySeekCacheMediaType == .video }
        let startSeconds: TimeInterval
        if videoEntries.isEmpty {
            startSeconds = time
        } else {
            guard let keyFrame = videoEntries
                .filter({ $0.seconds <= time && $0.item.memorySeekCacheIsKeyFrame })
                .max(by: { $0.seconds < $1.seconds })
            else {
                return nil
            }
            startSeconds = keyFrame.seconds
        }

        let selected = entries
            .filter { $0.seconds >= startSeconds && requiredTrackIDs.contains($0.item.memorySeekCacheTrackID) }
            .sorted {
                if $0.seconds == $1.seconds {
                    return $0.insertionIndex < $1.insertionIndex
                }
                return $0.seconds < $1.seconds
            }
        guard requiredTrackIDs.allSatisfy({ trackID in selected.contains { $0.item.memorySeekCacheTrackID == trackID } }) else {
            return nil
        }
        return selected.compactMap { $0.item.makeMemorySeekCacheCopy() as? Item }
    }

    func invalidate() {
        lock.lock()
        defer { lock.unlock() }
        entries.removeAll(keepingCapacity: true)
        _totalByteSize = 0
    }

    private func trim() {
        while _totalByteSize > maxByteSize, let first = entries.first {
            _totalByteSize -= first.byteSize
            entries.removeFirst()
        }

        guard let newestSeconds = entries.map(\.seconds).max() else {
            return
        }
        let minimumSeconds = newestSeconds - maxDuration
        entries.removeAll { entry in
            guard entry.seconds < minimumSeconds else {
                return false
            }
            _totalByteSize -= entry.byteSize
            return true
        }
    }
}

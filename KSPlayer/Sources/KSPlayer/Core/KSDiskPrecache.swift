import CryptoKit
import Foundation

public enum KSDiskPrecache {
    private static let cacheFolderName = "KSPlayerDiskPrecache"
    private static let temporaryExtension = "download"
    private static let supportedSchemes: Set<String> = ["http", "https"]
    private static let unsupportedExtensions: Set<String> = ["m3u", "m3u8", "mpd", "ism", "isml"]
    private static let queue = DispatchQueue(label: "KSPlayer.KSDiskPrecache")
    nonisolated(unsafe) private static var activeDownloads = Set<String>()
    nonisolated(unsafe) private static var activeDownloadTasks = [String: URLSessionTask]()

    public static func playbackURL(for url: URL, options: KSOptions) -> URL {
        guard options.isDiskPrecacheEnabled else {
            return url
        }
        if let cachedURL = cachedURL(for: url, options: options) {
            return cachedURL
        }
        startPrecache(for: url, options: options)
        return url
    }

    public static func startPrecache(for url: URL, options: KSOptions) {
        guard options.isDiskPrecacheEnabled,
              isSupportedRemoteMediaURL(url),
              options.diskPrecacheMaxFileSize > 0,
              options.diskPrecacheMaxCacheSize > 0
        else {
            return
        }

        let key = cacheKey(for: url)
        let directory = cacheDirectoryURL(options: options)
        let targetURL = cacheFileURL(for: url, directory: directory)
        let maxFileSize = options.diskPrecacheMaxFileSize
        let maxCacheSize = options.diskPrecacheMaxCacheSize
        let request = downloadRequest(for: url, options: options)

        guard !FileManager.default.fileExists(atPath: targetURL.path) else {
            touchCachedFile(at: targetURL)
            return
        }
        guard markDownloadStarted(key: key) else {
            return
        }

        trimCache(directory: directory, maxSize: maxCacheSize)
        let task = URLSession.shared.downloadTask(with: request) { temporaryURL, response, error in
            defer {
                markDownloadFinished(key: key)
            }
            guard error == nil,
                  let temporaryURL,
                  isSuccessfulHTTPResponse(response),
                  response.map({ responseAllowsCaching($0, maxFileSize: maxFileSize) }) ?? true
            else {
                return
            }
            storeDownloadedFile(from: temporaryURL, to: targetURL, directory: directory, maxFileSize: maxFileSize, maxCacheSize: maxCacheSize)
        }
        markDownloadTask(task, key: key)
        task.resume()
    }

    static func downloadRequest(for url: URL, options: KSOptions) -> URLRequest {
        var request = URLRequest(url: url, cachePolicy: .reloadIgnoringLocalCacheData)
        if let headers = options.avOptions["AVURLAssetHTTPHeaderFieldsKey"] as? [String: String] {
            for (field, value) in headers {
                request.setValue(value, forHTTPHeaderField: field)
            }
        }
        if request.value(forHTTPHeaderField: "User-Agent") == nil, let userAgent = options.userAgent {
            request.setValue(userAgent, forHTTPHeaderField: "User-Agent")
        }
        if request.value(forHTTPHeaderField: "Referer") == nil, let referer = options.referer {
            request.setValue(referer, forHTTPHeaderField: "Referer")
        }
        return request
    }

    public static func cachedURL(for url: URL, options: KSOptions) -> URL? {
        guard isSupportedRemoteMediaURL(url) else {
            return nil
        }
        let fileURL = cacheFileURL(for: url, directory: cacheDirectoryURL(options: options))
        guard cachedFileSize(at: fileURL) ?? 0 > 0 else {
            return nil
        }
        touchCachedFile(at: fileURL)
        return fileURL
    }

    public static func cacheSize(options: KSOptions) -> Int64 {
        cacheSize(directory: cacheDirectoryURL(options: options))
    }

    public static func trimCache(options: KSOptions) {
        trimCache(directory: cacheDirectoryURL(options: options), maxSize: options.diskPrecacheMaxCacheSize)
    }

    public static func clearCache(options: KSOptions) {
        cancelAllPrecache()
        try? FileManager.default.removeItem(at: cacheDirectoryURL(options: options))
    }

    public static func cancelPrecache(for url: URL) {
        let key = cacheKey(for: url)
        let task = queue.sync {
            activeDownloadTasks[key]
        }
        task?.cancel()
    }

    public static func cancelAllPrecache() {
        let tasks = queue.sync {
            let tasks = Array(activeDownloadTasks.values)
            activeDownloads.removeAll()
            activeDownloadTasks.removeAll()
            return tasks
        }
        tasks.forEach { $0.cancel() }
    }

    public static func cacheKey(for url: URL) -> String {
        let digest = SHA256.hash(data: Data(url.absoluteString.utf8))
        return digest.map { String(format: "%02x", $0) }.joined()
    }

    static func isSupportedRemoteMediaURL(_ url: URL) -> Bool {
        guard let scheme = url.scheme?.lowercased(), supportedSchemes.contains(scheme) else {
            return false
        }
        let pathExtension = url.pathExtension.lowercased()
        return pathExtension.isEmpty || !unsupportedExtensions.contains(pathExtension)
    }

    static func cacheDirectoryURL(options: KSOptions) -> URL {
        if let url = options.diskPrecacheDirectoryURL {
            return url
        }
        let baseURL = FileManager.default.urls(for: .cachesDirectory, in: .userDomainMask).first ?? FileManager.default.temporaryDirectory
        return baseURL.appendingPathComponent(cacheFolderName, isDirectory: true)
    }

    static func cacheFileURL(for url: URL, directory: URL) -> URL {
        let key = cacheKey(for: url)
        let pathExtension = url.pathExtension
        if pathExtension.isEmpty {
            return directory.appendingPathComponent(key)
        } else {
            return directory.appendingPathComponent(key).appendingPathExtension(pathExtension)
        }
    }

    private static func markDownloadStarted(key: String) -> Bool {
        queue.sync {
            guard !activeDownloads.contains(key) else {
                return false
            }
            activeDownloads.insert(key)
            return true
        }
    }

    private static func markDownloadTask(_ task: URLSessionTask, key: String) {
        queue.sync {
            guard activeDownloads.contains(key) else {
                return
            }
            activeDownloadTasks[key] = task
        }
    }

    private static func markDownloadFinished(key: String) {
        queue.sync {
            _ = activeDownloads.remove(key)
            activeDownloadTasks[key] = nil
        }
    }

    private static func isSuccessfulHTTPResponse(_ response: URLResponse?) -> Bool {
        guard let response = response as? HTTPURLResponse else {
            return true
        }
        return (200 ..< 300).contains(response.statusCode)
    }

    private static func responseAllowsCaching(_ response: URLResponse, maxFileSize: Int64) -> Bool {
        let expectedLength = response.expectedContentLength
        return expectedLength <= 0 || expectedLength <= maxFileSize
    }

    private static func storeDownloadedFile(from temporaryURL: URL, to targetURL: URL, directory: URL, maxFileSize: Int64, maxCacheSize: Int64) {
        let fileManager = FileManager.default
        let cacheTemporaryURL = temporaryFileURL(for: targetURL)
        do {
            try fileManager.createDirectory(at: directory, withIntermediateDirectories: true)
            guard let size = cachedFileSize(at: temporaryURL), size > 0, size <= min(maxFileSize, maxCacheSize) else {
                return
            }
            trimCache(directory: directory, maxSize: max(0, maxCacheSize - size))
            if fileManager.fileExists(atPath: targetURL.path) {
                return
            }
            try? fileManager.removeItem(at: cacheTemporaryURL)
            try fileManager.moveItem(at: temporaryURL, to: cacheTemporaryURL)
            defer {
                try? fileManager.removeItem(at: cacheTemporaryURL)
            }
            if fileManager.fileExists(atPath: targetURL.path) {
                return
            }
            try fileManager.moveItem(at: cacheTemporaryURL, to: targetURL)
            touchCachedFile(at: targetURL)
            trimCache(directory: directory, maxSize: maxCacheSize)
        } catch {
            KSLog(level: .debug, "disk precache failed for \(targetURL.lastPathComponent): \(error.localizedDescription)")
        }
    }

    static func trimCache(directory: URL, maxSize: Int64) {
        guard maxSize > 0 else {
            try? FileManager.default.removeItem(at: directory)
            return
        }
        let fileManager = FileManager.default
        guard let files = try? fileManager.contentsOfDirectory(
            at: directory,
            includingPropertiesForKeys: [.contentModificationDateKey, .fileSizeKey, .isRegularFileKey],
            options: [.skipsHiddenFiles]
        ) else {
            return
        }
        var entries = files.compactMap { url -> (url: URL, size: Int64, date: Date)? in
            guard url.pathExtension != temporaryExtension,
                  let values = try? url.resourceValues(forKeys: [.contentModificationDateKey, .fileSizeKey, .isRegularFileKey]),
                  values.isRegularFile == true,
                  let size = values.fileSize
            else {
                return nil
            }
            return (url, Int64(size), values.contentModificationDate ?? .distantPast)
        }
        var totalSize = entries.reduce(Int64(0)) { $0 + $1.size }
        guard totalSize > maxSize else {
            return
        }
        entries.sort { $0.date < $1.date }
        for entry in entries where totalSize > maxSize {
            try? fileManager.removeItem(at: entry.url)
            totalSize -= entry.size
        }
    }

    private static func cacheSize(directory: URL) -> Int64 {
        guard let files = try? FileManager.default.contentsOfDirectory(
            at: directory,
            includingPropertiesForKeys: [.fileSizeKey, .isRegularFileKey],
            options: [.skipsHiddenFiles]
        ) else {
            return 0
        }
        return files.reduce(Int64(0)) { total, url in
            guard url.pathExtension != temporaryExtension,
                  let size = cachedFileSize(at: url)
            else {
                return total
            }
            return total + size
        }
    }

    private static func temporaryFileURL(for targetURL: URL) -> URL {
        targetURL
            .deletingLastPathComponent()
            .appendingPathComponent(".\(targetURL.lastPathComponent).\(UUID().uuidString)")
            .appendingPathExtension(temporaryExtension)
    }

    private static func cachedFileSize(at url: URL) -> Int64? {
        guard let values = try? url.resourceValues(forKeys: [.fileSizeKey, .isRegularFileKey]),
              values.isRegularFile == true,
              let size = values.fileSize
        else {
            return nil
        }
        return Int64(size)
    }

    private static func touchCachedFile(at url: URL) {
        try? FileManager.default.setAttributes([.modificationDate: Date()], ofItemAtPath: url.path)
    }
}

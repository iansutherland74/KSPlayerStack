import Foundation

struct KSBluRaySource: Equatable {
    enum Kind: Equatable {
        case isoImage
        case bdmvDirectory
    }

    let kind: Kind
    let url: URL

    var ffmpegURLString: String {
        "bluray:\(url.standardizedFileURL.path)"
    }
}

enum KSBluRayURLResolver {
    static func source(for url: URL, fileManager: FileManager = .default) -> KSBluRaySource? {
        guard url.isFileURL else {
            return nil
        }
        let shouldStopAccessing = url.startAccessingSecurityScopedResource()
        defer {
            if shouldStopAccessing {
                url.stopAccessingSecurityScopedResource()
            }
        }

        if isISOImage(url, fileManager: fileManager) {
            return KSBluRaySource(kind: .isoImage, url: url)
        }

        guard let directoryURL = directoryURL(for: url, fileManager: fileManager) else {
            return nil
        }

        if isBDMVDirectory(directoryURL, fileManager: fileManager) {
            return KSBluRaySource(kind: .bdmvDirectory, url: directoryURL.deletingLastPathComponent())
        }

        if containsBDMVDirectory(directoryURL, fileManager: fileManager) {
            return KSBluRaySource(kind: .bdmvDirectory, url: directoryURL)
        }

        if let root = rootContainingBDMV(url: directoryURL, fileManager: fileManager) {
            return KSBluRaySource(kind: .bdmvDirectory, url: root)
        }

        return nil
    }

    static func isBluRayCandidate(_ url: URL, fileManager: FileManager = .default) -> Bool {
        if !url.isFileURL, url.pathExtension.caseInsensitiveCompare("iso") == .orderedSame {
            return true
        }
        return source(for: url, fileManager: fileManager) != nil
    }

    private static func directoryURL(for url: URL, fileManager: FileManager) -> URL? {
        var isDirectory = ObjCBool(false)
        guard fileManager.fileExists(atPath: url.path, isDirectory: &isDirectory) else {
            return nil
        }
        return isDirectory.boolValue ? url : url.deletingLastPathComponent()
    }

    private static func isISOImage(_ url: URL, fileManager: FileManager) -> Bool {
        guard url.pathExtension.caseInsensitiveCompare("iso") == .orderedSame else {
            return false
        }
        var isDirectory = ObjCBool(false)
        guard fileManager.fileExists(atPath: url.path, isDirectory: &isDirectory) else {
            return false
        }
        return !isDirectory.boolValue
    }

    private static func rootContainingBDMV(url: URL, fileManager: FileManager) -> URL? {
        var current = url.standardizedFileURL
        while current.pathComponents.count > 1 {
            if isBDMVDirectory(current, fileManager: fileManager) {
                return current.deletingLastPathComponent()
            }
            current.deleteLastPathComponent()
        }
        return nil
    }

    private static func containsBDMVDirectory(_ url: URL, fileManager: FileManager) -> Bool {
        guard let bdmvURL = bdmvDirectoryURL(in: url, fileManager: fileManager) else {
            return false
        }
        return isBDMVDirectory(bdmvURL, fileManager: fileManager)
    }

    private static func isBDMVDirectory(_ url: URL, fileManager: FileManager) -> Bool {
        guard url.lastPathComponent.caseInsensitiveCompare("BDMV") == .orderedSame else {
            return false
        }
        var isDirectory = ObjCBool(false)
        guard fileManager.fileExists(atPath: url.path, isDirectory: &isDirectory), isDirectory.boolValue else {
            return false
        }
        return containsFile(named: "index.bdmv", in: url, fileManager: fileManager)
            || containsFile(named: "MovieObject.bdmv", in: url, fileManager: fileManager)
    }

    private static func bdmvDirectoryURL(in url: URL, fileManager: FileManager) -> URL? {
        let exactURL = url.appendingPathComponent("BDMV", isDirectory: true)
        if isBDMVDirectory(exactURL, fileManager: fileManager) {
            return exactURL
        }
        guard let contents = try? fileManager.contentsOfDirectory(
            at: url,
            includingPropertiesForKeys: [.isDirectoryKey],
            options: [.skipsHiddenFiles]
        ) else {
            return nil
        }
        return contents.first { candidate in
            guard candidate.lastPathComponent.caseInsensitiveCompare("BDMV") == .orderedSame,
                  (try? candidate.resourceValues(forKeys: [.isDirectoryKey]).isDirectory) == true
            else {
                return false
            }
            return true
        }
    }

    private static func containsFile(named name: String, in url: URL, fileManager: FileManager) -> Bool {
        let exactURL = url.appendingPathComponent(name, isDirectory: false)
        var isDirectory = ObjCBool(false)
        if fileManager.fileExists(atPath: exactURL.path, isDirectory: &isDirectory), !isDirectory.boolValue {
            return true
        }
        guard let contents = try? fileManager.contentsOfDirectory(
            at: url,
            includingPropertiesForKeys: [.isRegularFileKey],
            options: [.skipsHiddenFiles]
        ) else {
            return false
        }
        return contents.contains { candidate in
            guard candidate.lastPathComponent.caseInsensitiveCompare(name) == .orderedSame else {
                return false
            }
            return (try? candidate.resourceValues(forKeys: [.isRegularFileKey]).isRegularFile) != false
        }
    }
}

extension KSOptions {
    func prepareFormatContextOptions(for _: KSBluRaySource) {
        guard formatContextOptions["protocol_whitelist"] is String else {
            return
        }
        appendBluRayProtocolWhitelistEntries(["bluray", "file"])
    }

    private func appendBluRayProtocolWhitelistEntries(_ entries: [String]) {
        let existing = formatContextOptions["protocol_whitelist"] as? String ?? ""
        var protocols = existing
            .split(separator: ",")
            .map(String.init)
        for entry in entries where !protocols.contains(entry) {
            protocols.append(entry)
        }
        formatContextOptions["protocol_whitelist"] = protocols.joined(separator: ",")
    }
}

final class KSSecurityScopedURLAccess: @unchecked Sendable {
    private var scopedURLs = [URL]()

    init(urls: [URL]) {
        var seenPaths = Set<String>()
        for url in urls where url.isFileURL {
            let path = url.standardizedFileURL.path
            guard seenPaths.insert(path).inserted else {
                continue
            }
            if url.startAccessingSecurityScopedResource() {
                scopedURLs.append(url)
            }
        }
    }

    convenience init(url: URL) {
        self.init(urls: [url])
    }

    func stop() {
        scopedURLs.forEach { $0.stopAccessingSecurityScopedResource() }
        scopedURLs.removeAll()
    }

    deinit {
        stop()
    }
}

extension URL {
    var isBluRayInputCandidate: Bool {
        KSBluRayURLResolver.isBluRayCandidate(self)
    }
}

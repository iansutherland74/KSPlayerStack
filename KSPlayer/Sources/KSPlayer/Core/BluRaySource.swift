import Foundation

struct KSBluRaySource: Equatable {
    enum Kind: Equatable {
        case isoImage
        case bdmvDirectory
    }

    let kind: Kind
    let url: URL

    var ffmpegURLString: String {
        "bluray:\(url.path)"
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

        if url.pathExtension.caseInsensitiveCompare("iso") == .orderedSame {
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
        if url.pathExtension.caseInsensitiveCompare("iso") == .orderedSame {
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
        isBDMVDirectory(url.appendingPathComponent("BDMV", isDirectory: true), fileManager: fileManager)
    }

    private static func isBDMVDirectory(_ url: URL, fileManager: FileManager) -> Bool {
        guard url.lastPathComponent.caseInsensitiveCompare("BDMV") == .orderedSame else {
            return false
        }
        let indexURL = url.appendingPathComponent("index.bdmv")
        let movieObjectURL = url.appendingPathComponent("MovieObject.bdmv")
        return fileManager.fileExists(atPath: indexURL.path) || fileManager.fileExists(atPath: movieObjectURL.path)
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

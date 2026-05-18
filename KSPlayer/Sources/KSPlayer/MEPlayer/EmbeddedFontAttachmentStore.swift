import CoreText
import Foundation
import Libavcodec
import Libavformat
import Libavutil

final class EmbeddedFontAttachmentStore {
    private static let maxFontAttachmentSize = 32 * 1024 * 1024
    private static let maxTotalFontAttachmentSize = 128 * 1024 * 1024
    private static let supportedFontExtensions: Set<String> = ["otc", "otf", "ttc", "ttf"]

    private let fileManager: FileManager
    private let directoryURL: URL
    private var writtenFontURLs = [URL]()
    private var registeredFontURLs = [URL]()
    var fontsDirectoryURL: URL? {
        writtenFontURLs.isEmpty ? nil : directoryURL
    }

    init(fileManager: FileManager = .default) {
        self.fileManager = fileManager
        directoryURL = fileManager.temporaryDirectory
            .appendingPathComponent("KSPlayerEmbeddedFonts", isDirectory: true)
            .appendingPathComponent(UUID().uuidString, isDirectory: true)
    }

    deinit {
        cleanup()
    }

    func extractAndRegister(from formatContext: UnsafeMutablePointer<AVFormatContext>) {
        var usedFilenames = Set<String>()
        var totalSize = 0

        for index in 0 ..< Int(formatContext.pointee.nb_streams) {
            guard let stream = formatContext.pointee.streams[index] else {
                continue
            }
            let codecParameters = stream.pointee.codecpar.pointee
            guard codecParameters.codec_type == AVMEDIA_TYPE_ATTACHMENT,
                  let preferredExtension = Self.preferredFontExtension(codecID: codecParameters.codec_id, metadata: toDictionary(stream.pointee.metadata)),
                  let extradata = codecParameters.extradata
            else {
                continue
            }

            let size = Int(codecParameters.extradata_size)
            guard size > 0, size <= Self.maxFontAttachmentSize else {
                KSLog("[subtitle] ignored embedded font attachment \(index): invalid size \(size)")
                continue
            }
            guard totalSize + size <= Self.maxTotalFontAttachmentSize else {
                KSLog("[subtitle] ignored embedded font attachment \(index): total size limit exceeded")
                continue
            }

            do {
                try fileManager.createDirectory(at: directoryURL, withIntermediateDirectories: true)
                let metadata = toDictionary(stream.pointee.metadata)
                let filename = Self.uniqueFilename(
                    Self.sanitizedFontFilename(
                        metadata["filename"],
                        fallbackBase: "font-\(index)",
                        preferredExtension: preferredExtension
                    ),
                    usedFilenames: &usedFilenames
                )
                let fontURL = directoryURL.appendingPathComponent(filename, isDirectory: false)
                try Data(bytes: extradata, count: size).write(to: fontURL, options: .atomic)
                writtenFontURLs.append(fontURL)
                totalSize += size
                registerFont(at: fontURL)
            } catch {
                KSLog("[subtitle] failed to extract embedded font attachment \(index): \(error)")
            }
        }
    }

    func cleanup() {
        for fontURL in registeredFontURLs.reversed() {
            var error: Unmanaged<CFError>?
            if !CTFontManagerUnregisterFontsForURL(fontURL as CFURL, .process, &error) {
                let message = error?.takeRetainedValue().localizedDescription ?? "unknown error"
                KSLog("[subtitle] failed to unregister embedded font \(fontURL.lastPathComponent): \(message)")
            }
        }
        registeredFontURLs.removeAll()
        writtenFontURLs.removeAll()
        try? fileManager.removeItem(at: directoryURL)
    }

    private func registerFont(at fontURL: URL) {
        var error: Unmanaged<CFError>?
        if CTFontManagerRegisterFontsForURL(fontURL as CFURL, .process, &error) {
            registeredFontURLs.append(fontURL)
        } else {
            let message = error?.takeRetainedValue().localizedDescription ?? "unknown error"
            KSLog("[subtitle] failed to register embedded font \(fontURL.lastPathComponent): \(message)")
        }
    }

    static func preferredFontExtension(codecID: AVCodecID, metadata: [String: String]) -> String? {
        if codecID == AV_CODEC_ID_TTF {
            return "ttf"
        }
        if codecID == AV_CODEC_ID_OTF {
            return "otf"
        }

        if let extensionHint = metadata["filename"]?.fontFileExtension, supportedFontExtensions.contains(extensionHint) {
            return extensionHint
        }

        let mimeType = (metadata["mimetype"] ?? metadata["mime_type"] ?? "").lowercased()
        if mimeType.contains("opentype") {
            return "otf"
        }
        if mimeType.contains("truetype") || mimeType.contains("font-sfnt") {
            return "ttf"
        }
        return nil
    }

    static func sanitizedFontFilename(_ filename: String?, fallbackBase: String, preferredExtension: String) -> String {
        let rawName = filename?.split(whereSeparator: { $0 == "/" || $0 == "\\" }).last.map(String.init) ?? ""
        let allowedCharacters = CharacterSet.alphanumerics.union(CharacterSet(charactersIn: " ._-"))
        var sanitized = String(rawName.unicodeScalars.map { scalar in
            allowedCharacters.contains(scalar) ? Character(scalar) : "_"
        })
        sanitized = sanitized.trimmingCharacters(in: .whitespacesAndNewlines)
        sanitized = sanitized.trimmingCharacters(in: CharacterSet(charactersIn: "."))

        if sanitized.isEmpty {
            sanitized = fallbackBase
        }

        let nsName = sanitized as NSString
        let extensionHint = nsName.pathExtension.lowercased()
        if !supportedFontExtensions.contains(extensionHint) {
            let base = nsName.deletingPathExtension.trimmingCharacters(in: CharacterSet(charactersIn: ". "))
            sanitized = (base.isEmpty ? fallbackBase : base) + "." + preferredExtension
        }

        let maxLength = 120
        guard sanitized.count > maxLength else {
            return sanitized
        }
        let nsSanitized = sanitized as NSString
        let ext = nsSanitized.pathExtension
        let base = nsSanitized.deletingPathExtension
        let suffix = ext.isEmpty ? "" : ".\(ext)"
        return String(base.prefix(max(1, maxLength - suffix.count))) + suffix
    }

    static func uniqueFilename(_ filename: String, usedFilenames: inout Set<String>) -> String {
        if usedFilenames.insert(filename).inserted {
            return filename
        }

        let nsName = filename as NSString
        let base = nsName.deletingPathExtension
        let ext = nsName.pathExtension
        var index = 1
        while true {
            let candidate = ext.isEmpty ? "\(base)-\(index)" : "\(base)-\(index).\(ext)"
            if usedFilenames.insert(candidate).inserted {
                return candidate
            }
            index += 1
        }
    }
}

private extension String {
    var fontFileExtension: String {
        split(whereSeparator: { $0 == "/" || $0 == "\\" })
            .last
            .map { String($0) as NSString }?
            .pathExtension
            .lowercased() ?? ""
    }
}

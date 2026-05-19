import Foundation

public enum ExternalSubtitleTranslationDisplayMode: Equatable, Sendable {
    case translation
    case bilingual(separator: String)
}

public struct SubtitleTranslationRequest: Equatable, Sendable {
    public var subtitleID: String
    public var subtitleName: String
    public var sourceLanguage: String?
    public var targetLanguage: String?

    public init(subtitleID: String, subtitleName: String, sourceLanguage: String? = nil, targetLanguage: String? = nil) {
        self.subtitleID = subtitleID
        self.subtitleName = subtitleName
        self.sourceLanguage = sourceLanguage
        self.targetLanguage = targetLanguage
    }
}

public protocol SubtitleTranslationProvider: Sendable {
    var providerID: String { get }
    func translateSubtitles(_ texts: [String], request: SubtitleTranslationRequest) async throws -> [String]
}

struct SubtitleTranslationConfiguration: Sendable {
    let provider: any SubtitleTranslationProvider
    let displayMode: ExternalSubtitleTranslationDisplayMode
    let sourceLanguage: String?
    let targetLanguage: String?
}

struct SubtitlePartSnapshot: Sendable {
    let start: TimeInterval
    let end: TimeInterval
    let identifier: String?
    let text: String
    let wordTimings: [SubtitleWordTiming]
    let textPosition: TextPosition?

    init(start: TimeInterval, end: TimeInterval, identifier: String?, text: String, wordTimings: [SubtitleWordTiming], textPosition: TextPosition?) {
        self.start = start
        self.end = end
        self.identifier = identifier
        self.text = text.trimmingCharacters(in: .whitespacesAndNewlines)
        self.wordTimings = wordTimings
        self.textPosition = textPosition
    }

    init?(part: SubtitlePart) {
        guard let text = part.text?.string.trimmingCharacters(in: .whitespacesAndNewlines), !text.isEmpty else {
            return nil
        }
        start = part.start
        end = part.end
        identifier = part.identifier
        self.text = text
        wordTimings = part.wordTimings
        textPosition = part.textPosition
    }

    func part(text displayText: String? = nil, preservingWords: Bool = true) -> SubtitlePart {
        let part = SubtitlePart(
            start,
            end,
            displayText ?? text,
            wordTimings: preservingWords ? wordTimings : []
        )
        part.identifier = identifier
        part.textPosition = textPosition
        return part
    }
}

extension ExternalSubtitleTranslationDisplayMode {
    var cacheComponent: String {
        switch self {
        case .translation:
            return "translation"
        case let .bilingual(separator):
            return "bilingual:\(separator)"
        }
    }

    func displayText(original: String, translation: String) -> String {
        switch self {
        case .translation:
            return translation.isEmpty ? original : translation
        case let .bilingual(separator):
            guard !translation.isEmpty, translation != original else {
                return original
            }
            return original + separator + translation
        }
    }

    var preservesOriginalText: Bool {
        switch self {
        case .translation:
            return false
        case .bilingual:
            return true
        }
    }
}

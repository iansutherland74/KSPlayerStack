//
//  KSSubtitle.swift
//  Pods
//
//  Created by kintan on 2017/4/2.
//
//

import AVFoundation
import CoreFoundation
import CoreGraphics
import Foundation
import SwiftUI
#if canImport(MediaAccessibility)
import CoreText
import MediaAccessibility
#endif

public struct SubtitleWordTiming: Equatable, Sendable {
    public let start: TimeInterval
    public let end: TimeInterval
    public let text: String

    public init(start: TimeInterval, end: TimeInterval, text: String) {
        self.start = start
        self.end = end
        self.text = text
    }
}

public class SubtitlePart: CustomStringConvertible, Identifiable {
    public var start: TimeInterval
    public var end: TimeInterval
    public var identifier: String?
    public var origin: CGPoint = .zero
    public var text: NSAttributedString?
    public let wordTimings: [SubtitleWordTiming]
    public var image: UIImage?
    public var imageRect: CGRect?
    public var imageCanvasSize: CGSize?
    public var textPosition: TextPosition?
    public var description: String {
        "Subtile Group ==========\nstart: \(start)\nend:\(end)\ntext:\(String(describing: text))"
    }

    public convenience init(_ start: TimeInterval, _ end: TimeInterval, _ string: String, wordTimings: [SubtitleWordTiming] = []) {
        var text = string
        text = text.trimmingCharacters(in: .whitespaces)
        text = text.replacingOccurrences(of: "\r", with: "")
        self.init(start, end, attributedString: NSAttributedString(string: text), wordTimings: wordTimings)
    }

    public init(_ start: TimeInterval, _ end: TimeInterval, attributedString: NSAttributedString?, wordTimings: [SubtitleWordTiming] = []) {
        self.start = start
        self.end = end
        text = attributedString
        self.wordTimings = wordTimings
    }
}

public extension SubtitlePart {
    func imageFrame(in bounds: CGRect) -> CGRect? {
        guard let image else {
            return nil
        }
        var rect = imageRect ?? CGRect(origin: origin, size: image.size)
        if rect.width <= 0 || rect.height <= 0 {
            rect.size = image.size
        }
        if let imageCanvasSize, imageCanvasSize.width > 0, imageCanvasSize.height > 0 {
            let scale = min(bounds.width / imageCanvasSize.width, bounds.height / imageCanvasSize.height)
            let canvasSize = CGSize(width: imageCanvasSize.width * scale, height: imageCanvasSize.height * scale)
            let canvasOrigin = CGPoint(
                x: bounds.minX + (bounds.width - canvasSize.width) / 2,
                y: bounds.minY + (bounds.height - canvasSize.height) / 2
            )
            return CGRect(
                x: canvasOrigin.x + rect.minX * scale,
                y: canvasOrigin.y + rect.minY * scale,
                width: rect.width * scale,
                height: rect.height * scale
            )
        }
        let scale = min(min(bounds.width / rect.width, bounds.height / rect.height), 1)
        let size = CGSize(width: rect.width * scale, height: rect.height * scale)
        return CGRect(
            x: bounds.minX + (bounds.width - size.width) / 2,
            y: bounds.maxY - size.height,
            width: size.width,
            height: size.height
        )
    }

    func activeWordIndex(at time: TimeInterval) -> Int? {
        wordTimings.firstIndex { word in
            word.start <= time && word.end > time
        }
    }

    func attributedText(at time: TimeInterval, activeWordAttributes: [NSAttributedString.Key: Any]) -> NSAttributedString? {
        guard let text else {
            return nil
        }
        guard !activeWordAttributes.isEmpty else {
            return text
        }
        guard let activeWordIndex = activeWordIndex(at: time),
              let activeWordRange = wordRange(for: activeWordIndex, in: text.string)
        else {
            return text
        }
        let highlighted = NSMutableAttributedString(attributedString: text)
        highlighted.addAttributes(activeWordAttributes, range: activeWordRange)
        return highlighted
    }

    private func wordRange(for activeWordIndex: Int, in text: String) -> NSRange? {
        var searchStart = text.startIndex
        for index in wordTimings.indices {
            let wordText = wordTimings[index].text
            guard !wordText.isEmpty,
                  let range = text.range(of: wordText, options: [], range: searchStart ..< text.endIndex)
            else {
                return nil
            }
            if index == activeWordIndex {
                return NSRange(range, in: text)
            }
            searchStart = range.upperBound
        }
        return nil
    }
}

public struct TextPosition: Sendable {
    public var verticalAlign: VerticalAlignment = .bottom
    public var horizontalAlign: HorizontalAlignment = .center
    public var leftMargin: CGFloat = 0
    public var rightMargin: CGFloat = 0
    public var verticalMargin: CGFloat = 10
    public var edgeInsets: EdgeInsets {
        var edgeInsets = EdgeInsets()
        if verticalAlign == .bottom {
            edgeInsets.bottom = verticalMargin
        } else if verticalAlign == .top {
            edgeInsets.top = verticalMargin
        }
        if horizontalAlign == .leading {
            edgeInsets.leading = leftMargin
        }
        if horizontalAlign == .trailing {
            edgeInsets.trailing = rightMargin
        }
        return edgeInsets
    }

    public mutating func ass(alignment: String?) {
        switch alignment {
        case "1":
            verticalAlign = .bottom
            horizontalAlign = .leading
        case "2":
            verticalAlign = .bottom
            horizontalAlign = .center
        case "3":
            verticalAlign = .bottom
            horizontalAlign = .trailing
        case "4":
            verticalAlign = .center
            horizontalAlign = .leading
        case "5":
            verticalAlign = .center
            horizontalAlign = .center
        case "6":
            verticalAlign = .center
            horizontalAlign = .trailing
        case "7":
            verticalAlign = .top
            horizontalAlign = .leading
        case "8":
            verticalAlign = .top
            horizontalAlign = .center
        case "9":
            verticalAlign = .top
            horizontalAlign = .trailing
        default:
            break
        }
    }
}

extension SubtitlePart: Comparable {
    public static func == (left: SubtitlePart, right: SubtitlePart) -> Bool {
        if left.start == right.start, left.end == right.end {
            return true
        } else {
            return false
        }
    }

    public static func < (left: SubtitlePart, right: SubtitlePart) -> Bool {
        if left.start < right.start {
            return true
        } else {
            return false
        }
    }
}

extension SubtitlePart: NumericComparable {
    public typealias Compare = TimeInterval
    public static func == (left: SubtitlePart, right: TimeInterval) -> Bool {
        left.start <= right && left.end >= right
    }

    public static func < (left: SubtitlePart, right: TimeInterval) -> Bool {
        left.end < right
    }
}

public protocol KSSubtitleProtocol {
    func search(for time: TimeInterval) -> [SubtitlePart]
}

public enum SubtitleCaptionAppearancePolicy: Sendable {
    /// Keep KSPlayer's existing subtitle styling behavior.
    case never
    /// Use system caption appearance values only when subtitle content does not provide its own styling.
    case contentIfAvailable
    /// Use system caption appearance values even when subtitle content provides embedded styling.
    case alwaysOverride
}

public enum SubtitleTextEdgeStyle: Sendable {
    case none
    case raised
    case depressed
    case uniform
    case dropShadow
}

public enum SubtitleHDREffectPolicy: Sendable {
    /// Render subtitles with the same SDR-oriented styling used before.
    case disabled
    /// Improve subtitle contrast for HDR video when KSPlayer owns the styling.
    case automatic
    /// Use a stronger contrast treatment for apps that prefer a more visible HDR subtitle presentation.
    case enhanced
}

public enum SubtitleKind: Equatable, Sendable {
    case text
    case image
    case closedCaption
    case unknown

    var displaySuffix: String? {
        switch self {
        case .text:
            nil
        case .image:
            NSLocalizedString("Image", comment: "Image subtitle track type")
        case .closedCaption:
            NSLocalizedString("Closed Captions", comment: "Closed caption subtitle track type")
        case .unknown:
            NSLocalizedString("Unknown", comment: "Unknown subtitle track type")
        }
    }
}

public protocol SubtitleKindProviding {
    var subtitleKind: SubtitleKind { get }
}

public protocol SubtitleInfo: KSSubtitleProtocol, AnyObject, Hashable, Identifiable {
    var subtitleID: String { get }
    var name: String { get }
    var delay: TimeInterval { get set }
    //    var userInfo: NSMutableDictionary? { get set }
    //    var subtitleDataSouce: SubtitleDataSouce? { get set }
//    var comment: String? { get }
    var isEnabled: Bool { get set }
}

public extension SubtitleInfo {
    var id: String { subtitleID }
    var subtitleKind: SubtitleKind {
        (self as? any SubtitleKindProviding)?.subtitleKind ?? (self as? MediaPlayerTrack)?.subtitleKind ?? .text
    }

    var displayName: String {
        guard let suffix = subtitleKind.displaySuffix,
              !name.localizedCaseInsensitiveContains(suffix)
        else {
            return name
        }
        return "\(name) (\(suffix))"
    }

    func hash(into hasher: inout Hasher) {
        hasher.combine(subtitleID)
    }

    static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.subtitleID == rhs.subtitleID
    }
}

public class KSSubtitle: @unchecked Sendable {
    public var parts: [SubtitlePart] = []
    public init() {}
}

extension KSSubtitle: KSSubtitleProtocol {
    /// Search for target group for time
    public func search(for time: TimeInterval) -> [SubtitlePart] {
        var result = [SubtitlePart]()
        for part in parts {
            if part == time {
                result.append(part)
            } else if part.start > time {
                break
            }
        }
        return result
    }
}

public extension KSSubtitle {
    func parse(url: URL, userAgent: String? = nil, encoding: String.Encoding? = nil) async throws {
        let data = try await url.data(userAgent: userAgent)
        try parse(data: data, encoding: encoding)
    }

    func parse(data: Data, encoding: String.Encoding? = nil) throws {
        var string: String?
        let encodes = [encoding ?? String.Encoding.utf8,
                       String.Encoding(rawValue: CFStringConvertEncodingToNSStringEncoding(CFStringEncoding(CFStringEncodings.big5.rawValue))),
                       String.Encoding(rawValue: CFStringConvertEncodingToNSStringEncoding(CFStringEncoding(CFStringEncodings.GB_18030_2000.rawValue))),
                       String.Encoding.unicode]
        for encode in encodes {
            string = String(data: data, encoding: encode)
            if string != nil {
                break
            }
        }
        guard let subtitle = string else {
            throw NSError(errorCode: .subtitleUnEncoding)
        }
        let scanner = Scanner(string: subtitle)
        _ = scanner.scanCharacters(from: .controlCharacters)
        let parse = KSOptions.subtitleParses.first { $0.canParse(scanner: scanner) }
        if let parse {
            parts = parse.parse(scanner: scanner)
            if parts.count == 0 {
                throw NSError(errorCode: .subtitleUnParse)
            }
        } else {
            throw NSError(errorCode: .subtitleFormatUnSupport)
        }
    }

//    public static func == (lhs: KSURLSubtitle, rhs: KSURLSubtitle) -> Bool {
//        lhs.url == rhs.url
//    }
}

public protocol NumericComparable {
    associatedtype Compare
    static func < (lhs: Self, rhs: Compare) -> Bool
    static func == (lhs: Self, rhs: Compare) -> Bool
}

extension Collection where Element: NumericComparable {
    func binarySearch(key: Element.Compare) -> Self.Index? {
        var lowerBound = startIndex
        var upperBound = endIndex
        while lowerBound < upperBound {
            let midIndex = index(lowerBound, offsetBy: distance(from: lowerBound, to: upperBound) / 2)
            if self[midIndex] == key {
                return midIndex
            } else if self[midIndex] < key {
                lowerBound = index(lowerBound, offsetBy: 1)
            } else {
                upperBound = midIndex
            }
        }
        return nil
    }
}

open class SubtitleModel: ObservableObject {
    public enum Size {
        case smaller
        case standard
        case large
        public var rawValue: CGFloat {
            switch self {
            case .smaller:
                #if os(tvOS) || os(xrOS)
                return 48
                #elseif os(macOS) || os(xrOS)
                return 20
                #else
                if UI_USER_INTERFACE_IDIOM() == .phone {
                    return 12
                } else {
                    return 20
                }
                #endif
            case .standard:
                #if os(tvOS) || os(xrOS)
                return 58
                #elseif os(macOS) || os(xrOS)
                return 26
                #else
                if UI_USER_INTERFACE_IDIOM() == .phone {
                    return 16
                } else {
                    return 26
                }
                #endif
            case .large:
                #if os(tvOS) || os(xrOS)
                return 68
                #elseif os(macOS) || os(xrOS)
                return 32
                #else
                if UI_USER_INTERFACE_IDIOM() == .phone {
                    return 20
                } else {
                    return 32
                }
                #endif
            }
        }
    }

    nonisolated(unsafe) public static var textColor: Color = .white
    nonisolated(unsafe) public static var textBackgroundColor: Color = .clear
    nonisolated(unsafe) public static var textWindowColor: Color = .clear
    nonisolated(unsafe) public static var textEdgeStyle: SubtitleTextEdgeStyle = .none
    nonisolated(unsafe) public static var activeWordTextColor: Color = .yellow
    nonisolated(unsafe) public static var activeWordBackgroundColor: Color = .clear
    nonisolated(unsafe) public static var isWordHighlightingEnabled = true
    public static var textFont: UIFont {
        textBold ? .boldSystemFont(ofSize: textFontSize) : .systemFont(ofSize: textFontSize)
    }

    public static var activeWordAttributes: [NSAttributedString.Key: Any] {
        guard isWordHighlightingEnabled else {
            return [:]
        }
        return [
            NSAttributedString.Key.foregroundColor: UIColor(activeWordTextColor),
            NSAttributedString.Key.backgroundColor: UIColor(activeWordBackgroundColor),
        ]
    }

    static var textContainerBackgroundColor: Color {
        alpha(of: platformColor(textWindowColor)) > 0 ? textWindowColor : textBackgroundColor
    }

    nonisolated(unsafe) public static var textFontSize = SubtitleModel.Size.standard.rawValue
    nonisolated(unsafe) public static var textBold = false
    nonisolated(unsafe) public static var textItalic = false
    nonisolated(unsafe) public static var textPosition = TextPosition()
    nonisolated(unsafe) public static var audioRecognizes = [any AudioRecognize]()
    private var subtitleDataSouces: [SubtitleDataSouce] = KSOptions.subtitleDataSouces
    @Published
    public private(set) var subtitleInfos = [any SubtitleInfo]()
    @Published
    public private(set) var parts = [SubtitlePart]()
    @Published
    public private(set) var secondaryParts = [SubtitlePart]()
    @Published
    public private(set) var currentSubtitleTime: TimeInterval = 0
    @Published
    public private(set) var currentSecondarySubtitleTime: TimeInterval = 0
    public var captionAppearancePolicy = KSOptions.subtitleCaptionAppearancePolicy {
        didSet {
            if captionAppearancePolicy != oldValue {
                updateSystemCaptionAppearance()
            }
        }
    }
    public var hdrEffectPolicy = KSOptions.subtitleHDREffectPolicy
    @Published
    public var videoDynamicRange: DynamicRange?
    public var subtitleDelay = 0.0 // s
    private var activeWordIndexes = [ObjectIdentifier: Int?]()
    private var secondaryActiveWordIndexes = [ObjectIdentifier: Int?]()
    private var isExternalSubtitleTranslationEnabled = KSOptions.isExternalSubtitleTranslationEnabled
    private var externalSubtitleTranslationProvider: (any SubtitleTranslationProvider)? = KSOptions.externalSubtitleTranslationProvider
    private var externalSubtitleTranslationDisplayMode = KSOptions.externalSubtitleTranslationDisplayMode
    private var externalSubtitleTranslationSourceLanguage = KSOptions.externalSubtitleTranslationSourceLanguage
    private var externalSubtitleTranslationTargetLanguage = KSOptions.externalSubtitleTranslationTargetLanguage
    private var lastAppliedCaptionAppearancePolicy: SubtitleCaptionAppearancePolicy?
    private var lastAppliedHDREffectPolicy: SubtitleHDREffectPolicy?
    private var lastAppliedVideoDynamicRange: DynamicRange?
    public var url: URL? {
        didSet {
            subtitleInfos.removeAll()
            searchSubtitle(query: nil, languages: [])
            if url != nil {
                subtitleInfos.append(contentsOf: SubtitleModel.audioRecognizes)
            }
            for datasouce in subtitleDataSouces {
                addSubtitle(dataSouce: datasouce)
            }
            // 要用async，不能在更新UI的时候，修改Publishe变量
            runOnMainThread { [weak self] in
                self?.parts = []
                self?.secondaryParts = []
                self?.selectedSubtitleInfo = nil
                self?.selectedSecondarySubtitleInfo = nil
            }
        }
    }

    @Published
    public var selectedSubtitleInfo: (any SubtitleInfo)? {
        didSet {
            updateSubtitleSelection(oldValue: oldValue, newValue: selectedSubtitleInfo)
            if let url, let info = selectedSubtitleInfo as? URLSubtitleInfo, !info.downloadURL.isFileURL, let cache = subtitleDataSouces.first(where: { $0 is CacheSubtitleDataSouce }) as? CacheSubtitleDataSouce {
                cache.addCache(fileURL: url, downloadURL: info.downloadURL)
            }
        }
    }

    @Published
    public var selectedSecondarySubtitleInfo: (any SubtitleInfo)? {
        didSet {
            updateSubtitleSelection(oldValue: oldValue, newValue: selectedSecondarySubtitleInfo)
            if let url, let info = selectedSecondarySubtitleInfo as? URLSubtitleInfo, !info.downloadURL.isFileURL, let cache = subtitleDataSouces.first(where: { $0 is CacheSubtitleDataSouce }) as? CacheSubtitleDataSouce {
                cache.addCache(fileURL: url, downloadURL: info.downloadURL)
            }
        }
    }

    public init() {}

    public func apply(options: KSOptions) {
        subtitleDataSouces = KSOptions.subtitleDataSouces
        if !options.onlineSubtitleProviders.isEmpty {
            subtitleDataSouces.append(OnlineSubtitleDataSouce(
                providers: options.onlineSubtitleProviders,
                languages: options.onlineSubtitleLanguages,
                userAgent: options.userAgent
            ))
        }
        captionAppearancePolicy = options.subtitleCaptionAppearancePolicy
        hdrEffectPolicy = options.subtitleHDREffectPolicy
        isExternalSubtitleTranslationEnabled = options.isExternalSubtitleTranslationEnabled
        externalSubtitleTranslationProvider = options.externalSubtitleTranslationProvider
        externalSubtitleTranslationDisplayMode = options.externalSubtitleTranslationDisplayMode
        externalSubtitleTranslationSourceLanguage = options.externalSubtitleTranslationSourceLanguage
        externalSubtitleTranslationTargetLanguage = options.externalSubtitleTranslationTargetLanguage
        subtitleInfos.compactMap { $0 as? URLSubtitleInfo }.forEach(configureExternalSubtitleTranslation)
        updateSystemCaptionAppearance()
    }

    public func addSubtitle(info: any SubtitleInfo) {
        if subtitleInfos.first(where: { $0.subtitleID == info.subtitleID }) == nil {
            if let info = info as? URLSubtitleInfo {
                configureExternalSubtitleTranslation(info)
            }
            subtitleInfos.append(info)
        }
    }

    public func subtitle(currentTime: TimeInterval) -> Bool {
        let primary = subtitleParts(for: selectedSubtitleInfo, currentTime: currentTime, previousParts: parts)
        let secondary = subtitleParts(for: selectedSecondarySubtitleInfo, currentTime: currentTime, previousParts: secondaryParts)
        let newParts = primary.parts
        let newSecondaryParts = secondary.parts
        let newActiveWordIndexes = Self.isWordHighlightingEnabled ? activeWordIndexes(for: newParts, at: primary.time) : [:]
        let newSecondaryActiveWordIndexes = Self.isWordHighlightingEnabled ? activeWordIndexes(for: newSecondaryParts, at: secondary.time) : [:]
        // swiftUI不会判断是否相等。所以需要这边判断下。
        if newParts != parts ||
            newSecondaryParts != secondaryParts ||
            newActiveWordIndexes != activeWordIndexes ||
            newSecondaryActiveWordIndexes != secondaryActiveWordIndexes ||
            hasSubtitleStyleConfigurationChanged
        {
            style(parts: newParts)
            style(parts: newSecondaryParts)
            currentSubtitleTime = primary.time
            currentSecondarySubtitleTime = secondary.time
            activeWordIndexes = newActiveWordIndexes
            secondaryActiveWordIndexes = newSecondaryActiveWordIndexes
            lastAppliedCaptionAppearancePolicy = captionAppearancePolicy
            lastAppliedHDREffectPolicy = hdrEffectPolicy
            lastAppliedVideoDynamicRange = videoDynamicRange
            parts = newParts
            secondaryParts = newSecondaryParts
            return true
        } else {
            return false
        }
    }

    public func searchSubtitle(query: String?, languages: [String]) {
        for dataSouce in subtitleDataSouces {
            if let dataSouce = dataSouce as? SearchSubtitleDataSouce {
                subtitleInfos.removeAll { info in
                    dataSouce.infos.contains {
                        $0 === info
                    }
                }
                Task { @MainActor in
                    try? await dataSouce.searchSubtitle(query: query, languages: languages)
                    dataSouce.infos.compactMap { $0 as? URLSubtitleInfo }.forEach(configureExternalSubtitleTranslation)
                    subtitleInfos.append(contentsOf: dataSouce.infos)
                }
            }
        }
    }

    public func addSubtitle(dataSouce: SubtitleDataSouce) {
        if let dataSouce = dataSouce as? FileURLSubtitleDataSouce {
            Task { @MainActor in
                try? await dataSouce.searchSubtitle(fileURL: url)
                    dataSouce.infos.compactMap { $0 as? URLSubtitleInfo }.forEach(configureExternalSubtitleTranslation)
                subtitleInfos.append(contentsOf: dataSouce.infos)
            }
        } else {
                dataSouce.infos.compactMap { $0 as? URLSubtitleInfo }.forEach(configureExternalSubtitleTranslation)
            subtitleInfos.append(contentsOf: dataSouce.infos)
        }
    }

    private func activeWordIndexes(for parts: [SubtitlePart], at time: TimeInterval) -> [ObjectIdentifier: Int?] {
        Dictionary(uniqueKeysWithValues: parts.map { part in
            (ObjectIdentifier(part), part.activeWordIndex(at: time))
        })
    }

    private func subtitleParts(for info: (any SubtitleInfo)?, currentTime: TimeInterval, previousParts: [SubtitlePart]) -> (time: TimeInterval, parts: [SubtitlePart]) {
        guard let info else {
            return (currentTime, [])
        }
        let subtitleTime = currentTime - info.delay - subtitleDelay
        var newParts = info.search(for: subtitleTime)
        if newParts.isEmpty {
            newParts = previousParts.filter { part in
                part == subtitleTime
            }
        }
        return (subtitleTime, newParts)
    }

    private func style(parts: [SubtitlePart]) {
        for part in parts {
            if let text = part.text {
                let styledText = NSMutableAttributedString(attributedString: text)
                styledText.applySubtitleStyle(policy: captionAppearancePolicy, dynamicRange: videoDynamicRange, hdrEffectPolicy: hdrEffectPolicy)
                part.text = styledText
            }
        }
    }

    private func updateSubtitleSelection(oldValue: (any SubtitleInfo)?, newValue: (any SubtitleInfo)?) {
        if let oldValue, !isSubtitleSelected(oldValue) {
            oldValue.isEnabled = false
        }
        newValue?.isEnabled = true
    }

    private func isSubtitleSelected(_ info: any SubtitleInfo) -> Bool {
        selectedSubtitleInfo === info || selectedSecondarySubtitleInfo === info
    }

    private func configureExternalSubtitleTranslation(_ info: URLSubtitleInfo) {
        info.configureExternalSubtitleTranslation(
            isEnabled: isExternalSubtitleTranslationEnabled,
            provider: externalSubtitleTranslationProvider,
            displayMode: externalSubtitleTranslationDisplayMode,
            sourceLanguage: externalSubtitleTranslationSourceLanguage,
            targetLanguage: externalSubtitleTranslationTargetLanguage
        )
    }

    private func updateSystemCaptionAppearance() {
        guard captionAppearancePolicy != .never else {
            return
        }
        SubtitleModel.updateSystemCaptionAppearance()
    }

    private var hasSubtitleStyleConfigurationChanged: Bool {
        lastAppliedCaptionAppearancePolicy != captionAppearancePolicy ||
            lastAppliedHDREffectPolicy != hdrEffectPolicy ||
            lastAppliedVideoDynamicRange != videoDynamicRange
    }
}

struct SubtitleResolvedStyle {
    var font: UIFont
    var foregroundColor: UIColor
    var backgroundColor: UIColor
    var windowColor: UIColor
    var edgeStyle: SubtitleTextEdgeStyle
    var overrideContentAttributes: Bool

    init(font: UIFont, foregroundColor: UIColor, backgroundColor: UIColor, windowColor: UIColor = .clear, edgeStyle: SubtitleTextEdgeStyle, overrideContentAttributes: Bool) {
        self.font = font
        self.foregroundColor = foregroundColor
        self.backgroundColor = backgroundColor
        self.windowColor = windowColor
        self.edgeStyle = edgeStyle
        self.overrideContentAttributes = overrideContentAttributes
    }
}

struct SubtitleHDRStyleResolver {
    static func resolvedStyle(
        base: SubtitleResolvedStyle,
        dynamicRange: DynamicRange?,
        hdrEffectPolicy: SubtitleHDREffectPolicy,
        captionAppearancePolicy: SubtitleCaptionAppearancePolicy
    ) -> SubtitleResolvedStyle {
        guard shouldApplyHDREffects(dynamicRange: dynamicRange, hdrEffectPolicy: hdrEffectPolicy, captionAppearancePolicy: captionAppearancePolicy) else {
            return base
        }

        var style = base
        if SubtitleModel.alpha(of: style.windowColor) == 0 {
            style.windowColor = UIColor.black.withAlphaComponent(hdrEffectPolicy == .enhanced ? 0.55 : 0.42)
        }
        if SubtitleModel.alpha(of: style.backgroundColor) == 0, hdrEffectPolicy == .enhanced {
            style.backgroundColor = UIColor.black.withAlphaComponent(0.28)
        }
        if style.edgeStyle == .none || style.edgeStyle == .dropShadow {
            style.edgeStyle = hdrEffectPolicy == .enhanced ? .uniform : .dropShadow
        }
        return style
    }

    static func activeWordAttributes(
        base: [NSAttributedString.Key: Any],
        dynamicRange: DynamicRange?,
        hdrEffectPolicy: SubtitleHDREffectPolicy,
        captionAppearancePolicy: SubtitleCaptionAppearancePolicy
    ) -> [NSAttributedString.Key: Any] {
        guard shouldApplyHDREffects(dynamicRange: dynamicRange, hdrEffectPolicy: hdrEffectPolicy, captionAppearancePolicy: captionAppearancePolicy) else {
            return base
        }

        var attributes = base
        attributes[.foregroundColor] = UIColor.black
        attributes[.backgroundColor] = UIColor(red: 1.0, green: 0.86, blue: 0.25, alpha: hdrEffectPolicy == .enhanced ? 0.95 : 0.85)
        return attributes
    }

    private static func shouldApplyHDREffects(
        dynamicRange: DynamicRange?,
        hdrEffectPolicy: SubtitleHDREffectPolicy,
        captionAppearancePolicy: SubtitleCaptionAppearancePolicy
    ) -> Bool {
        guard hdrEffectPolicy != .disabled, dynamicRange?.isHDR == true else {
            return false
        }
        return captionAppearancePolicy == .never
    }
}

extension SubtitleModel {
    static func resolvedStyle(policy: SubtitleCaptionAppearancePolicy) -> SubtitleResolvedStyle {
        resolvedStyle(policy: policy, dynamicRange: nil, hdrEffectPolicy: .disabled)
    }

    static func resolvedStyle(policy: SubtitleCaptionAppearancePolicy, dynamicRange: DynamicRange?, hdrEffectPolicy: SubtitleHDREffectPolicy) -> SubtitleResolvedStyle {
        if policy != .never {
            updateSystemCaptionAppearance()
        }
        let baseStyle = SubtitleResolvedStyle(
            font: textFont,
            foregroundColor: platformColor(textColor),
            backgroundColor: platformColor(textBackgroundColor),
            windowColor: platformColor(textWindowColor),
            edgeStyle: textEdgeStyle,
            overrideContentAttributes: policy == .alwaysOverride
        )
        return SubtitleHDRStyleResolver.resolvedStyle(base: baseStyle, dynamicRange: dynamicRange, hdrEffectPolicy: hdrEffectPolicy, captionAppearancePolicy: policy)
    }

    func resolvedStyle() -> SubtitleResolvedStyle {
        Self.resolvedStyle(policy: captionAppearancePolicy, dynamicRange: videoDynamicRange, hdrEffectPolicy: hdrEffectPolicy)
    }

    var activeWordAttributes: [NSAttributedString.Key: Any] {
        Self.activeWordAttributes(dynamicRange: videoDynamicRange, hdrEffectPolicy: hdrEffectPolicy, captionAppearancePolicy: captionAppearancePolicy)
    }

    static func activeWordAttributes(dynamicRange: DynamicRange?, hdrEffectPolicy: SubtitleHDREffectPolicy, captionAppearancePolicy: SubtitleCaptionAppearancePolicy) -> [NSAttributedString.Key: Any] {
        SubtitleHDRStyleResolver.activeWordAttributes(
            base: activeWordAttributes,
            dynamicRange: dynamicRange,
            hdrEffectPolicy: hdrEffectPolicy,
            captionAppearancePolicy: captionAppearancePolicy
        )
    }

    func updateVideoDynamicRange(from player: MediaPlayerProtocol) {
        let dynamicRange = Self.activeVideoDynamicRange(from: player.tracks(mediaType: .video))
        if videoDynamicRange != dynamicRange {
            videoDynamicRange = dynamicRange
        }
    }

    static func activeVideoDynamicRange(from tracks: [MediaPlayerTrack]) -> DynamicRange? {
        tracks.first { $0.isEnabled }?.dynamicRange
    }

    static func updateSystemCaptionAppearance() {
        #if canImport(MediaAccessibility)
        let domain = MACaptionAppearanceDomain.user
        var behavior = MACaptionAppearanceBehavior.useValue

        let foregroundColor = MACaptionAppearanceCopyForegroundColor(domain, &behavior).takeRetainedValue()
        if shouldUseSystemCaptionValue(behavior: behavior) {
            textColor = color(from: foregroundColor, opacity: MACaptionAppearanceGetForegroundOpacity(domain, &behavior))
        }

        behavior = .useValue
        let backgroundColor = MACaptionAppearanceCopyBackgroundColor(domain, &behavior).takeRetainedValue()
        if shouldUseSystemCaptionValue(behavior: behavior) {
            textBackgroundColor = color(from: backgroundColor, opacity: MACaptionAppearanceGetBackgroundOpacity(domain, &behavior))
        }

        behavior = .useValue
        let windowColor = MACaptionAppearanceCopyWindowColor(domain, &behavior).takeRetainedValue()
        if shouldUseSystemCaptionValue(behavior: behavior) {
            textWindowColor = color(from: windowColor, opacity: MACaptionAppearanceGetWindowOpacity(domain, &behavior))
        }

        behavior = .useValue
        let relativeSize = MACaptionAppearanceGetRelativeCharacterSize(domain, &behavior)
        let descriptor = MACaptionAppearanceCopyFontDescriptorForStyle(domain, &behavior, .default).takeRetainedValue()
        if shouldUseSystemCaptionValue(behavior: behavior) {
            let fontSize = Size.standard.rawValue * max(relativeSize, 0.1)
            let font = CTFontCreateWithFontDescriptor(descriptor, fontSize, nil)
            let fontName = CTFontCopyPostScriptName(font) as String
            if let captionFont = UIFont(name: fontName, size: fontSize) {
                textFontSize = captionFont.pointSize
                textBold = captionFont.fontDescriptor.symbolicTraits.contains(.traitBold)
                textItalic = captionFont.fontDescriptor.symbolicTraits.contains(.traitItalic)
            } else {
                textFontSize = fontSize
            }
        }

        behavior = .useValue
        textEdgeStyle = SubtitleTextEdgeStyle(MACaptionAppearanceGetTextEdgeStyle(domain, &behavior))
        #endif
    }

    #if canImport(MediaAccessibility)
    static func shouldUseSystemCaptionValue(behavior: MACaptionAppearanceBehavior) -> Bool {
        behavior == .useValue || behavior == .useContentIfAvailable
    }
    #endif

    static func color(from cgColor: CGColor, opacity: CGFloat) -> Color {
        #if canImport(UIKit)
        return Color(UIColor(cgColor: cgColor).withAlphaComponent(opacity))
        #else
        return Color(nsColor: (UIColor(cgColor: cgColor) ?? .white).withAlphaComponent(opacity))
        #endif
    }

    static func platformColor(_ color: Color) -> UIColor {
        #if canImport(UIKit)
        return UIColor(color)
        #else
        return UIColor(color)
        #endif
    }

    static func swiftUIColor(_ color: UIColor) -> Color {
        #if canImport(UIKit)
        return Color(color)
        #else
        return Color(nsColor: color)
        #endif
    }

    static func alpha(of color: UIColor) -> CGFloat {
        #if canImport(UIKit)
        return color.cgColor.alpha
        #else
        return color.cgColor.alpha
        #endif
    }
}

private extension SubtitleTextEdgeStyle {
    #if canImport(MediaAccessibility)
    init(_ style: MACaptionAppearanceTextEdgeStyle) {
        switch style {
        case .raised:
            self = .raised
        case .depressed:
            self = .depressed
        case .uniform:
            self = .uniform
        case .dropShadow:
            self = .dropShadow
        default:
            self = .none
        }
    }
    #endif
}

extension NSMutableAttributedString {
    func applySubtitleStyle(policy: SubtitleCaptionAppearancePolicy) {
        applySubtitleStyle(policy: policy, dynamicRange: nil, hdrEffectPolicy: .disabled)
    }

    func applySubtitleStyle(policy: SubtitleCaptionAppearancePolicy, dynamicRange: DynamicRange?, hdrEffectPolicy: SubtitleHDREffectPolicy) {
        let style = SubtitleModel.resolvedStyle(policy: policy, dynamicRange: dynamicRange, hdrEffectPolicy: hdrEffectPolicy)
        applySubtitleStyle(style)
    }

    func applySubtitleStyle(_ style: SubtitleResolvedStyle) {
        applySubtitleAttribute(.font, value: style.font, overrideExisting: style.overrideContentAttributes)
        applySubtitleAttribute(.foregroundColor, value: style.foregroundColor, overrideExisting: style.overrideContentAttributes)
        if SubtitleModel.alpha(of: style.backgroundColor) > 0 {
            applySubtitleAttribute(.backgroundColor, value: style.backgroundColor, overrideExisting: style.overrideContentAttributes)
        }
        applySubtitleEdgeStyle(style.edgeStyle, overrideExisting: style.overrideContentAttributes)
    }

    private func applySubtitleAttribute(_ key: NSAttributedString.Key, value: Any, overrideExisting: Bool) {
        let fullRange = NSRange(location: 0, length: length)
        guard !overrideExisting else {
            addAttribute(key, value: value, range: fullRange)
            return
        }
        enumerateAttribute(key, in: fullRange) { existingValue, range, _ in
            if existingValue == nil {
                addAttribute(key, value: value, range: range)
            }
        }
    }

    private func applySubtitleEdgeStyle(_ edgeStyle: SubtitleTextEdgeStyle, overrideExisting: Bool) {
        switch edgeStyle {
        case .none:
            if overrideExisting {
                removeAttribute(.shadow, range: NSRange(location: 0, length: length))
                removeAttribute(.strokeWidth, range: NSRange(location: 0, length: length))
            }
        case .dropShadow:
            let shadow = NSShadow()
            shadow.shadowColor = UIColor.black.withAlphaComponent(0.9)
            shadow.shadowOffset = CGSize(width: 1, height: 1)
            shadow.shadowBlurRadius = 1
            applySubtitleAttribute(.shadow, value: shadow, overrideExisting: overrideExisting)
        case .uniform:
            applySubtitleAttribute(.strokeWidth, value: -2, overrideExisting: overrideExisting)
            applySubtitleAttribute(.strokeColor, value: UIColor.black, overrideExisting: overrideExisting)
        case .raised:
            let shadow = NSShadow()
            shadow.shadowColor = UIColor.black.withAlphaComponent(0.9)
            shadow.shadowOffset = CGSize(width: -1, height: -1)
            applySubtitleAttribute(.shadow, value: shadow, overrideExisting: overrideExisting)
        case .depressed:
            let shadow = NSShadow()
            shadow.shadowColor = UIColor.black.withAlphaComponent(0.9)
            shadow.shadowOffset = CGSize(width: 1, height: 1)
            applySubtitleAttribute(.shadow, value: shadow, overrideExisting: overrideExisting)
        }
    }
}

@testable import KSPlayer
import AVFoundation
import Libavcodec
import XCTest
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

class SubtitleTest: XCTestCase {
    func testAVLegibleSubtitleIDUsesStablePrefixAndFallbackLanguage() {
        XCTAssertEqual(
            AVLegibleSubtitlePolicy.subtitleID(languageCode: "en", displayName: "English"),
            "av-legible:en:English"
        )
        XCTAssertEqual(
            AVLegibleSubtitlePolicy.subtitleID(languageCode: nil, displayName: "Forced"),
            "av-legible:und:Forced"
        )
    }

    func testPictureInPictureSubtitlePolicyDefaultsToAutomatic() {
        XCTAssertEqual(KSOptions().pictureInPictureSubtitlePolicy, .automatic)
    }

    func testSrt() {
        let string = """
        1
        00:00:00,050 --> 00:00:11,000
        <font color="#4096d1">本字幕仅供学习交流，严禁用于商业用途</font>

        2
        00:00:13,000 --> 00:00:18,000
        <font color=#4096d1>-=破烂熊字幕组=-
        翻译:风铃
        校对&时间轴:小白</font>

        3
        00:01:00,840 --> 00:01:02,435
        你现在必须走了吗?

        4
        00:01:02,680 --> 00:01:04,318
        我说过我会去找他的

        5
        00:01:07,194 --> 00:01:08,239
        - 很多事情我们都说过
        - 我承诺过他

        907
        00:59:47,520 --> 00:59:49,720
        有两个人在我们镇上
        There were two men in my hometown

        908
        00:59:51,370 --> 00:59:55,170
        被判4F不合格，他们就自杀了，因为不能服役
        Declared 4-F unfit, they killed themselves cause they couldn't serve.

        909
        00:59:55,750 --> 00:59:58,360
        注：4-F，二战服役有关的物理，心理，或道德标准。
        https://en.wikipedia.org/wiki/Selective_Service_System

         http://www.apd.army.mil/pdffiles/r40_501.pdf

        910
        00:59:59,220 --> 01:00:01,140
        为何？我在国防工厂有份工作
        Why, I had a job in a defense plant.

        """
        let scanner = Scanner(string: string)
        let parse = SrtParse()
        XCTAssertEqual(parse.canParse(scanner: scanner), true)
        let parts = parse.parse(scanner: scanner)
        XCTAssertEqual(parts.count, 9)
        XCTAssertEqual(parts[8].end, 3601.14)
    }

    func testVtt() {
        let string = """
        WEBVTT
        1
        00:00:00,050 --> 00:00:11,000
        <font color="#4096d1">本字幕仅供学习交流，严禁用于商业用途</font>

        2
        00:00:13,000 --> 00:00:18,000
        <font color=#4096d1>-=破烂熊字幕组=-
        翻译:风铃
        校对&时间轴:小白</font>

        3
        00:01:00,840 --> 00:01:02,435
        你现在必须走了吗?

        4
        00:01:02,680 --> 00:01:04,318
        我说过我会去找他的

        5
        00:01:07,194 --> 00:01:08,239
        - 很多事情我们都说过
        - 我承诺过他

        6
        00:01:08,280 --> 00:01:10,661
        我希望你明白

        7
        00:01:12,814 --> 00:01:14,702
        等等! 你是不可能活着回来的!

        """
        let scanner = Scanner(string: string)
        let parse = VTTParse()
        XCTAssertEqual(parse.canParse(scanner: scanner), true)
        let parts = parse.parse(scanner: scanner)
        XCTAssertEqual(parts.count, 7)
    }

    func testSubtitleStylePreservesEmbeddedAttributes() {
        let embeddedFont = UIFont.boldSystemFont(ofSize: 12)
        let embeddedColor = UIColor.red
        let systemFont = UIFont.systemFont(ofSize: 24)
        let attributed = NSMutableAttributedString(string: "hello world")
        attributed.addAttributes([
            .font: embeddedFont,
            .foregroundColor: embeddedColor,
        ], range: NSRange(location: 0, length: 5))

        attributed.applySubtitleStyle(SubtitleResolvedStyle(
            font: systemFont,
            foregroundColor: .white,
            backgroundColor: .clear,
            edgeStyle: .none,
            overrideContentAttributes: false
        ))

        XCTAssertEqual((attributed.attribute(.font, at: 0, effectiveRange: nil) as? UIFont)?.pointSize, embeddedFont.pointSize)
        XCTAssertEqual(attributed.attribute(.foregroundColor, at: 0, effectiveRange: nil) as? UIColor, embeddedColor)
        XCTAssertEqual((attributed.attribute(.font, at: 6, effectiveRange: nil) as? UIFont)?.pointSize, systemFont.pointSize)
        XCTAssertEqual(attributed.attribute(.foregroundColor, at: 6, effectiveRange: nil) as? UIColor, UIColor.white)
    }

    func testSubtitleStyleCanOverrideEmbeddedAttributes() {
        let embeddedFont = UIFont.boldSystemFont(ofSize: 12)
        let systemFont = UIFont.systemFont(ofSize: 24)
        let attributed = NSMutableAttributedString(string: "hello")
        attributed.addAttributes([
            .font: embeddedFont,
            .foregroundColor: UIColor.red,
        ], range: NSRange(location: 0, length: attributed.length))

        attributed.applySubtitleStyle(SubtitleResolvedStyle(
            font: systemFont,
            foregroundColor: .white,
            backgroundColor: .clear,
            edgeStyle: .none,
            overrideContentAttributes: true
        ))

        XCTAssertEqual((attributed.attribute(.font, at: 0, effectiveRange: nil) as? UIFont)?.pointSize, systemFont.pointSize)
        XCTAssertEqual(attributed.attribute(.foregroundColor, at: 0, effectiveRange: nil) as? UIColor, UIColor.white)
    }

    func testHDRSubtitleStyleAddsContrastForHDRVideo() {
        let baseStyle = SubtitleResolvedStyle(
            font: .systemFont(ofSize: 24),
            foregroundColor: .white,
            backgroundColor: .clear,
            edgeStyle: .none,
            overrideContentAttributes: false
        )

        let style = SubtitleHDRStyleResolver.resolvedStyle(
            base: baseStyle,
            dynamicRange: .hdr10,
            hdrEffectPolicy: .automatic,
            captionAppearancePolicy: .never
        )

        XCTAssertGreaterThan(SubtitleModel.alpha(of: style.windowColor), 0)
        XCTAssertEqual(style.edgeStyle, .dropShadow)
    }

    func testHDRSubtitleStyleRespectsSystemCaptionAppearance() {
        let baseStyle = SubtitleResolvedStyle(
            font: .systemFont(ofSize: 24),
            foregroundColor: .white,
            backgroundColor: .clear,
            edgeStyle: .none,
            overrideContentAttributes: false
        )

        let style = SubtitleHDRStyleResolver.resolvedStyle(
            base: baseStyle,
            dynamicRange: .dolbyVision,
            hdrEffectPolicy: .automatic,
            captionAppearancePolicy: .contentIfAvailable
        )

        XCTAssertEqual(SubtitleModel.alpha(of: style.windowColor), 0)
        XCTAssertEqual(style.edgeStyle, .none)
    }

    func testHDRActiveWordHighlightUsesReadableContrast() {
        let attributes = SubtitleHDRStyleResolver.activeWordAttributes(
            base: [
                .foregroundColor: UIColor.yellow,
                .backgroundColor: UIColor.clear,
            ],
            dynamicRange: .hlg,
            hdrEffectPolicy: .automatic,
            captionAppearancePolicy: .never
        )

        XCTAssertEqual(attributes[.foregroundColor] as? UIColor, UIColor.black)
        XCTAssertGreaterThan(SubtitleModel.alpha(of: attributes[.backgroundColor] as? UIColor ?? .clear), 0)
    }

    func testOfflineSubtitleGeneratorStoresOrdersAndTrimsSegments() {
        let generator = OfflineSubtitleGenerator(
            provider: EmptyOfflineSubtitleProvider(),
            displayMode: .bilingual(separator: "\n"),
            maxSegmentCount: 2
        )
        generator.append(segments: [
            OfflineSubtitleSegment(identifier: "1", start: 0, end: 1, text: "hello", translation: "你好"),
            OfflineSubtitleSegment(identifier: "2", start: 1, end: 2, text: "world"),
            OfflineSubtitleSegment(identifier: "3", start: 2, end: 3, text: "again"),
        ])
        XCTAssertTrue(generator.search(for: 0.5).isEmpty)
        XCTAssertEqual(generator.search(for: 1.5).first?.text?.string, "world")
        XCTAssertEqual(generator.search(for: 2.5).first?.text?.string, "again")
    }

    func testOfflineSubtitleGeneratorReplacesSegmentWithSameIdentifier() {
        let generator = OfflineSubtitleGenerator(provider: EmptyOfflineSubtitleProvider())
        generator.append(segments: [
            OfflineSubtitleSegment(identifier: "same", start: 0, end: 1, text: "old"),
            OfflineSubtitleSegment(identifier: "same", start: 0, end: 1, text: "new"),
        ])
        let parts = generator.search(for: 0.5)
        XCTAssertEqual(parts.count, 1)
        XCTAssertEqual(parts.first?.text?.string, "new")
    }

    func testOfflineSubtitleGeneratorTranslationDisplayModes() {
        let generator = OfflineSubtitleGenerator(
            provider: EmptyOfflineSubtitleProvider(),
            displayMode: .translation
        )
        generator.append(segments: [
            OfflineSubtitleSegment(start: 0, end: 1, text: "hello", translation: "你好"),
        ])
        XCTAssertEqual(generator.search(for: 0.5).first?.text?.string, "你好")

        generator.displayMode = .bilingual(separator: " / ")
        generator.append(segments: [
            OfflineSubtitleSegment(identifier: "bilingual", start: 1, end: 2, text: "world", translation: "世界"),
        ])
        XCTAssertEqual(generator.search(for: 1.5).first?.text?.string, "world / 世界")
    }

    func testSubtitlePartSelectsAndHighlightsActiveWord() {
        let part = SubtitlePart(
            0,
            2,
            "hello world",
            wordTimings: [
                SubtitleWordTiming(start: 0, end: 1, text: "hello"),
                SubtitleWordTiming(start: 1, end: 2, text: "world"),
            ]
        )

        XCTAssertEqual(part.activeWordIndex(at: 0.5), 0)
        XCTAssertEqual(part.activeWordIndex(at: 1.0), 1)
        XCTAssertNil(part.activeWordIndex(at: 2.0))

        let highlighted = part.attributedText(at: 1.5, activeWordAttributes: [.backgroundColor: "active"])
        XCTAssertEqual(highlighted?.string, "hello world")
        XCTAssertEqual(highlighted?.attribute(.backgroundColor, at: 6, effectiveRange: nil) as? String, "active")
        XCTAssertNil(highlighted?.attribute(.backgroundColor, at: 0, effectiveRange: nil))
    }

    func testSubtitlePartFallsBackToNormalTextWithoutWords() {
        let part = SubtitlePart(0, 2, "hello world")

        XCTAssertNil(part.activeWordIndex(at: 0.5))
        let rendered = part.attributedText(at: 0.5, activeWordAttributes: [.backgroundColor: "active"])
        XCTAssertEqual(rendered?.string, "hello world")
        XCTAssertNil(rendered?.attribute(.backgroundColor, at: 0, effectiveRange: nil))
    }

    func testImageSubtitleFrameUsesCanvasCoordinatesAndTiming() {
        let part = SubtitlePart(1, 3, attributedString: nil)
        part.image = makeSubtitleImage(width: 100, height: 20)
        part.origin = CGPoint(x: 100, y: 800)
        part.imageRect = CGRect(x: 100, y: 800, width: 100, height: 20)
        part.imageCanvasSize = CGSize(width: 1920, height: 1080)

        let subtitle = KSSubtitle()
        subtitle.parts = [part]

        XCTAssertEqual(subtitle.search(for: 0.5).count, 0)
        XCTAssertEqual(subtitle.search(for: 2).first, part)
        XCTAssertEqual(part.imageFrame(in: CGRect(x: 0, y: 0, width: 960, height: 540)), CGRect(x: 50, y: 400, width: 50, height: 10))
    }

    func testSubtitleURLDetectsImageSubtitleExtensions() {
        XCTAssertTrue(URL(fileURLWithPath: "/tmp/movie.sup").isImageSubtitle)
        XCTAssertTrue(URL(fileURLWithPath: "/tmp/movie.pgs").isSubtitle)
        XCTAssertTrue(URL(fileURLWithPath: "/tmp/movie.srt").isTextSubtitle)
    }

    func testSubtitleURLClassifiesSubtitleKind() {
        XCTAssertEqual(URL(fileURLWithPath: "/tmp/movie.srt").subtitleKind, .text)
        XCTAssertEqual(URL(fileURLWithPath: "/tmp/movie.ass").subtitleKind, .text)
        XCTAssertEqual(URL(fileURLWithPath: "/tmp/movie.sup").subtitleKind, .image)
        XCTAssertEqual(URL(fileURLWithPath: "/tmp/movie.mkv").subtitleKind, .unknown)
    }

    func testFFmpegSubtitleKindClassifiesClosedCaptionsAndImages() {
        XCTAssertEqual(FFmpegAssetTrack.subtitleKind(codecID: AV_CODEC_ID_EIA_608, isImageSubtitle: false), .closedCaption)
        XCTAssertEqual(FFmpegAssetTrack.subtitleKind(codecID: AV_CODEC_ID_HDMV_PGS_SUBTITLE, isImageSubtitle: true), .image)
        XCTAssertEqual(FFmpegAssetTrack.subtitleKind(codecID: AV_CODEC_ID_SUBRIP, isImageSubtitle: false), .text)
    }

    func testSubtitleDisplayNameLabelsNonTextKinds() {
        let imageInfo = URLSubtitleInfo(url: URL(fileURLWithPath: "/tmp/movie.sup"))
        let textInfo = URLSubtitleInfo(url: URL(fileURLWithPath: "/tmp/movie.srt"))

        XCTAssertEqual(imageInfo.displayName, "movie.sup (Image)")
        XCTAssertEqual(textInfo.displayName, "movie.srt")
    }

    func testShooterOnlineSubtitleRequestUsesHashAndSafeFilename() throws {
        let fileURL = URL(fileURLWithPath: "/Users/private/Movies/Movie Name.mkv")
        let request = try XCTUnwrap(ShooterOnlineSubtitleProvider.makeSearchRequest(fileURL: fileURL, fileHash: "a;b;c;d"))
        let components = try XCTUnwrap(URLComponents(url: request.url!, resolvingAgainstBaseURL: false))
        let queryItems = Dictionary(uniqueKeysWithValues: (components.queryItems ?? []).map { ($0.name, $0.value ?? "") })

        XCTAssertEqual(request.httpMethod, "POST")
        XCTAssertEqual(queryItems["filehash"], "a;b;c;d")
        XCTAssertEqual(queryItems["pathinfo"], "Movie Name.mkv")
        XCTAssertFalse(request.url!.absoluteString.contains("/Users/private"))
    }

    func testAssrtOnlineSubtitleRequestUsesBearerToken() throws {
        let request = try XCTUnwrap(AssrtOnlineSubtitleProvider.makeSearchRequest(query: "My Movie", token: "user-token"))
        let components = try XCTUnwrap(URLComponents(url: request.url!, resolvingAgainstBaseURL: false))
        let queryItems = Dictionary(uniqueKeysWithValues: (components.queryItems ?? []).map { ($0.name, $0.value ?? "") })

        XCTAssertEqual(request.httpMethod, "POST")
        XCTAssertEqual(request.value(forHTTPHeaderField: "Authorization"), "Bearer user-token")
        XCTAssertEqual(queryItems["q"], "My Movie")
    }

    func testOpenSubtitlesOnlineSubtitleSearchRequestUsesAppCredentialAndIDs() throws {
        let search = OnlineSubtitleSearchRequest(
            query: "Movie",
            languages: ["en", "zh-cn"],
            movieHash: "hash-value",
            imdbID: 12345,
            tmdbID: 67890
        )
        let request = try XCTUnwrap(OpenSubtitlesOnlineSubtitleProvider.makeSearchRequest(request: search, apiKey: "app-key", token: "session-token"))
        let components = try XCTUnwrap(URLComponents(url: request.url!, resolvingAgainstBaseURL: false))
        let queryItems = Dictionary(uniqueKeysWithValues: (components.queryItems ?? []).map { ($0.name, $0.value ?? "") })

        XCTAssertEqual(request.value(forHTTPHeaderField: "Api-Key"), "app-key")
        XCTAssertEqual(request.value(forHTTPHeaderField: "Authorization"), "Bearer session-token")
        XCTAssertEqual(queryItems["query"], "Movie")
        XCTAssertEqual(queryItems["languages"], "en,zh-cn")
        XCTAssertEqual(queryItems["moviehash"], "hash-value")
        XCTAssertEqual(queryItems["imdb_id"], "12345")
        XCTAssertNil(queryItems["imbd_id"])
        XCTAssertEqual(queryItems["tmdb_id"], "67890")
    }

    func testOpenSubtitlesDownloadRequestUsesJSONBody() throws {
        let request = try XCTUnwrap(OpenSubtitlesOnlineSubtitleProvider.makeDownloadRequest(fileID: 42, apiKey: "app-key"))
        let body = try XCTUnwrap(request.httpBody)
        let json = try XCTUnwrap(JSONSerialization.jsonObject(with: body) as? [String: Int])

        XCTAssertEqual(request.httpMethod, "POST")
        XCTAssertEqual(request.value(forHTTPHeaderField: "Api-Key"), "app-key")
        XCTAssertEqual(request.value(forHTTPHeaderField: "Content-Type"), "application/json")
        XCTAssertEqual(json["file_id"], 42)
    }

    func testOnlineSubtitleDataSourceMapsProviderResultsWithoutNetwork() async throws {
        let provider = StaticOnlineSubtitleProvider(results: [
            OnlineSubtitleSearchResult(
                providerID: "test",
                subtitleID: "1",
                name: "English.srt",
                downloadURL: URL(string: "https://example.com/English.srt")!,
                comment: "fixture",
                delay: 0.5
            ),
        ])
        let dataSource = OnlineSubtitleDataSouce(providers: [provider], languages: ["en"], userAgent: "UnitTest")

        try await dataSource.searchSubtitle(query: "Movie", languages: [])

        let info = try XCTUnwrap(dataSource.infos.first as? URLSubtitleInfo)
        XCTAssertEqual(info.subtitleID, "test:1")
        XCTAssertEqual(info.name, "English.srt")
        XCTAssertEqual(info.delay, 0.5)
        XCTAssertEqual(info.comment, "fixture")
    }

    func testExternalSubtitleTranslationModeReplacesParsedText() async throws {
        let provider = RecordingSubtitleTranslationProvider(translations: ["你好"])
        let info = URLSubtitleInfo(url: try makeSubtitleFile(extension: "srt", text: simpleSrt(text: "hello")))
        info.configureExternalSubtitleTranslation(
            isEnabled: true,
            provider: provider,
            displayMode: .translation,
            sourceLanguage: "en",
            targetLanguage: "zh-Hans"
        )

        try await info.loadIfNeeded()
        await info.waitForExternalSubtitleTranslation()

        let part = try XCTUnwrap(info.search(for: 0.5).first)
        XCTAssertEqual(part.text?.string, "你好")
        XCTAssertEqual(part.identifier, nil)
        XCTAssertEqual(part.start, 0)
        XCTAssertEqual(part.end, 1)
        let requests = await provider.requests()
        XCTAssertEqual(requests, [["hello"]])
    }

    func testExternalSubtitleBilingualModePreservesOriginalAndWords() async throws {
        let provider = RecordingSubtitleTranslationProvider(translations: ["世界"])
        let info = URLSubtitleInfo(url: try makeSubtitleFile(extension: "srt", text: simpleSrt(text: "world")))
        info.parts = [
            SubtitlePart(
                0,
                1,
                "world",
                wordTimings: [SubtitleWordTiming(start: 0, end: 1, text: "world")]
            ),
        ]
        info.configureExternalSubtitleTranslation(
            isEnabled: true,
            provider: provider,
            displayMode: .bilingual(separator: "\n"),
            sourceLanguage: "en",
            targetLanguage: "zh-Hans"
        )

        info.translateParsedPartsIfNeeded()
        await info.waitForExternalSubtitleTranslation()

        let part = try XCTUnwrap(info.search(for: 0.5).first)
        XCTAssertEqual(part.text?.string, "world\n世界")
        XCTAssertEqual(part.activeWordIndex(at: 0.5), 0)
    }

    func testExternalSubtitleTranslationCachesProviderWork() async throws {
        let provider = RecordingSubtitleTranslationProvider(translations: ["hola"])
        let info = URLSubtitleInfo(url: try makeSubtitleFile(extension: "srt", text: simpleSrt(text: "hello")))
        info.configureExternalSubtitleTranslation(
            isEnabled: true,
            provider: provider,
            displayMode: .translation,
            sourceLanguage: "en",
            targetLanguage: "es"
        )

        try await info.loadIfNeeded()
        await info.waitForExternalSubtitleTranslation()
        info.translateParsedPartsIfNeeded()
        await info.waitForExternalSubtitleTranslation()

        XCTAssertEqual(info.search(for: 0.5).first?.text?.string, "hola")
        let requestCount = await provider.requestCount()
        XCTAssertEqual(requestCount, 1)
    }

    func testExternalSubtitleTranslationSkipsImageSubtitles() async throws {
        let provider = RecordingSubtitleTranslationProvider(translations: ["ignored"])
        let info = URLSubtitleInfo(url: URL(fileURLWithPath: "/tmp/movie.sup"))
        info.configureExternalSubtitleTranslation(
            isEnabled: true,
            provider: provider,
            displayMode: .translation,
            sourceLanguage: nil,
            targetLanguage: "en"
        )

        try await info.loadIfNeeded()
        await info.waitForExternalSubtitleTranslation()

        XCTAssertTrue(info.parts.isEmpty)
        let requestCount = await provider.requestCount()
        XCTAssertEqual(requestCount, 0)
    }

    func testAssImageSubtitleRenderPolicyUsesCodecCanvasBeforePlayRes() {
        let header = """
        [Script Info]
        PlayResX: 1280
        PlayResY: 720
        """

        XCTAssertEqual(
            AssImageSubtitleRenderPolicy.canvasSize(codecWidth: 1920, codecHeight: 1080, subtitleHeader: header),
            CGSize(width: 1920, height: 1080)
        )
    }

    func testAssImageSubtitleRenderPolicyFallsBackToPlayRes() {
        let header = """
        [Script Info]
        PlayResX: 1280
        PlayResY: 720
        """

        XCTAssertEqual(
            AssImageSubtitleRenderPolicy.canvasSize(codecWidth: 0, codecHeight: 0, subtitleHeader: header),
            CGSize(width: 1280, height: 720)
        )
    }

    func testEmbeddedFontAttachmentFilenamesAreSandboxSafe() {
        XCTAssertEqual(
            EmbeddedFontAttachmentStore.sanitizedFontFilename("../Fonts/Anime:Title?.ttf", fallbackBase: "font-1", preferredExtension: "ttf"),
            "Anime_Title_.ttf"
        )
        XCTAssertEqual(
            EmbeddedFontAttachmentStore.sanitizedFontFilename("../../bad.ass", fallbackBase: "font-2", preferredExtension: "otf"),
            "bad.otf"
        )
        XCTAssertEqual(
            EmbeddedFontAttachmentStore.sanitizedFontFilename(nil, fallbackBase: "font-3", preferredExtension: "ttf"),
            "font-3.ttf"
        )
    }

    func testEmbeddedFontAttachmentFilenamesAreUnique() {
        var usedFilenames = Set<String>()

        XCTAssertEqual(EmbeddedFontAttachmentStore.uniqueFilename("font.ttf", usedFilenames: &usedFilenames), "font.ttf")
        XCTAssertEqual(EmbeddedFontAttachmentStore.uniqueFilename("font.ttf", usedFilenames: &usedFilenames), "font-1.ttf")
        XCTAssertEqual(EmbeddedFontAttachmentStore.uniqueFilename("font.ttf", usedFilenames: &usedFilenames), "font-2.ttf")
    }

    func testOfflineSubtitleGeneratorCarriesWordTimingsAndSubtitleModelRefreshesActiveWord() {
        let generator = OfflineSubtitleGenerator(provider: EmptyOfflineSubtitleProvider())
        generator.append(segments: [
            OfflineSubtitleSegment(
                start: 0,
                end: 2,
                text: "hello world",
                words: [
                    OfflineSubtitleWord(start: 0, end: 1, text: "hello"),
                    OfflineSubtitleWord(start: 1, end: 2, text: "world"),
                ]
            ),
        ])

        let model = SubtitleModel()
        model.addSubtitle(info: generator)
        model.selectedSubtitleInfo = generator

        XCTAssertTrue(model.subtitle(currentTime: 0.5))
        XCTAssertEqual(model.parts.first?.activeWordIndex(at: model.currentSubtitleTime), 0)
        XCTAssertTrue(model.subtitle(currentTime: 1.5))
        XCTAssertEqual(model.parts.first?.activeWordIndex(at: model.currentSubtitleTime), 1)
        XCTAssertFalse(model.subtitle(currentTime: 1.75))
    }

    func testSubtitleModelRendersPrimaryAndSecondaryWithIndependentTiming() {
        let primary = StaticSubtitleInfo(
            subtitleID: "primary",
            name: "English",
            parts: [SubtitlePart(0, 1, "primary")]
        )
        let secondary = StaticSubtitleInfo(
            subtitleID: "secondary",
            name: "Spanish",
            delay: 1,
            parts: [SubtitlePart(0, 1, "secondary")]
        )
        let model = SubtitleModel()
        model.selectedSubtitleInfo = primary
        model.selectedSecondarySubtitleInfo = secondary

        XCTAssertTrue(model.subtitle(currentTime: 1.5))
        XCTAssertTrue(model.parts.isEmpty)
        XCTAssertEqual(model.secondaryParts.first?.text?.string, "secondary")
        XCTAssertEqual(model.currentSecondarySubtitleTime, 0.5)

        XCTAssertTrue(model.subtitle(currentTime: 2.5))
        XCTAssertTrue(model.secondaryParts.isEmpty)
    }

    func testSubtitleModelFallbackKeepsPrimaryAndSecondaryPartsIndependently() {
        let primaryPart = SubtitlePart(0, 3, "primary")
        let secondaryPart = SubtitlePart(0, 3, "secondary")
        let primary = OneShotSubtitleInfo(subtitleID: "primary", name: "English", part: primaryPart)
        let secondary = OneShotSubtitleInfo(subtitleID: "secondary", name: "French", part: secondaryPart)
        let model = SubtitleModel()
        model.selectedSubtitleInfo = primary
        model.selectedSecondarySubtitleInfo = secondary

        XCTAssertTrue(model.subtitle(currentTime: 1))
        XCTAssertEqual(model.parts.first?.text?.string, "primary")
        XCTAssertEqual(model.secondaryParts.first?.text?.string, "secondary")

        XCTAssertFalse(primary.hasSearchResult)
        XCTAssertFalse(secondary.hasSearchResult)
        XCTAssertFalse(model.subtitle(currentTime: 2))
        XCTAssertEqual(model.parts.first?.text?.string, "primary")
        XCTAssertEqual(model.secondaryParts.first?.text?.string, "secondary")
    }

    func testOfflineSubtitleGeneratorResetClearsSegments() async throws {
        let provider = RecordingOfflineSubtitleProvider()
        let generator = OfflineSubtitleGenerator(provider: provider)
        generator.append(segments: [
            OfflineSubtitleSegment(start: 0, end: 1, text: "visible"),
        ])
        XCTAssertFalse(generator.search(for: 0.5).isEmpty)

        generator.reset()
        try await Task.sleep(nanoseconds: 10_000_000)

        XCTAssertTrue(generator.search(for: 0.5).isEmpty)
        let resetCount = await provider.resetCount()
        XCTAssertEqual(resetCount, 1)
    }

    func testOfflineSubtitleGeneratorIgnoresStaleProviderResultsAfterReset() async throws {
        let provider = DelayedOfflineSubtitleProvider()
        let generator = OfflineSubtitleGenerator(provider: provider)
        generator.isEnabled = true

        generator.append(frame: makeAudioFrame())
        generator.reset()
        try await Task.sleep(nanoseconds: 100_000_000)

        XCTAssertTrue(generator.search(for: 0.01).isEmpty)
        let processCount = await provider.processCount()
        let resetCount = await provider.resetCount()
        XCTAssertEqual(processCount, 1)
        XCTAssertEqual(resetCount, 1)
    }
}

private struct EmptyOfflineSubtitleProvider: OfflineSubtitleGenerationProvider {
    func process(frame _: OfflineSubtitleAudioFrame) async throws -> [OfflineSubtitleSegment] {
        []
    }
}

private struct StaticOnlineSubtitleProvider: OnlineSubtitleProvider {
    let providerID = "test"
    let displayName = "Test"
    let results: [OnlineSubtitleSearchResult]

    func searchSubtitles(request _: OnlineSubtitleSearchRequest) async throws -> [OnlineSubtitleSearchResult] {
        results
    }
}

private actor RecordingSubtitleTranslationProvider: SubtitleTranslationProvider {
    let providerID = "recording"
    private let translations: [String]
    private var recordedRequests = [[String]]()

    init(translations: [String]) {
        self.translations = translations
    }

    func translateSubtitles(_ texts: [String], request _: SubtitleTranslationRequest) async throws -> [String] {
        recordedRequests.append(texts)
        return translations
    }

    func requests() -> [[String]] {
        recordedRequests
    }

    func requestCount() -> Int {
        recordedRequests.count
    }
}

private final class StaticSubtitleInfo: SubtitleInfo {
    let subtitleID: String
    let name: String
    var delay: TimeInterval
    var isEnabled = false
    private let parts: [SubtitlePart]

    init(subtitleID: String, name: String, delay: TimeInterval = 0, parts: [SubtitlePart]) {
        self.subtitleID = subtitleID
        self.name = name
        self.delay = delay
        self.parts = parts
    }

    func search(for time: TimeInterval) -> [SubtitlePart] {
        parts.filter { $0 == time }
    }
}

private final class OneShotSubtitleInfo: SubtitleInfo {
    let subtitleID: String
    let name: String
    var delay: TimeInterval = 0
    var isEnabled = false
    var hasSearchResult = true
    private let part: SubtitlePart

    init(subtitleID: String, name: String, part: SubtitlePart) {
        self.subtitleID = subtitleID
        self.name = name
        self.part = part
    }

    func search(for time: TimeInterval) -> [SubtitlePart] {
        guard hasSearchResult, part == time else {
            return []
        }
        hasSearchResult = false
        return [part]
    }
}

private actor RecordingOfflineSubtitleProvider: OfflineSubtitleGenerationProvider {
    private var resets = 0

    func process(frame _: OfflineSubtitleAudioFrame) async throws -> [OfflineSubtitleSegment] {
        []
    }

    func reset() async {
        resets += 1
    }

    func resetCount() -> Int {
        resets
    }
}

private actor DelayedOfflineSubtitleProvider: OfflineSubtitleGenerationProvider {
    private var processes = 0
    private var resets = 0

    func process(frame: OfflineSubtitleAudioFrame) async throws -> [OfflineSubtitleSegment] {
        processes += 1
        try await Task.sleep(nanoseconds: 30_000_000)
        return [
            OfflineSubtitleSegment(start: frame.startTime, end: frame.startTime + 1, text: "stale"),
        ]
    }

    func reset() async {
        resets += 1
    }

    func processCount() -> Int {
        processes
    }

    func resetCount() -> Int {
        resets
    }
}

private func makeAudioFrame() -> AudioFrame {
    let format = AVAudioFormat(commonFormat: .pcmFormatFloat32, sampleRate: 16_000, channels: 1, interleaved: false)!
    let samples: [Float] = [0, 0.1, -0.1, 0]
    let frame = AudioFrame(dataSize: samples.count * MemoryLayout<Float>.size, audioFormat: format)
    frame.numberOfSamples = UInt32(samples.count)
    frame.timebase = Timebase(num: 1, den: 16_000)
    frame.timestamp = 0
    frame.duration = Int64(samples.count)
    frame.data[0]?.withMemoryRebound(to: Float.self, capacity: samples.count) { pointer in
        pointer.update(from: samples, count: samples.count)
    }
    return frame
}

private func simpleSrt(text: String) -> String {
    """
    1
    00:00:00,000 --> 00:00:01,000
    \(text)

    """
}

private func makeSubtitleFile(extension fileExtension: String, text: String) throws -> URL {
    let url = FileManager.default.temporaryDirectory
        .appendingPathComponent(UUID().uuidString)
        .appendingPathExtension(fileExtension)
    try text.write(to: url, atomically: true, encoding: .utf8)
    return url
}

private func makeSubtitleImage(width: Int, height: Int) -> UIImage {
    let colorSpace = CGColorSpaceCreateDeviceRGB()
    let context = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: CGImageAlphaInfo.premultipliedLast.rawValue
    )!
    context.setFillColor(CGColor(red: 1, green: 1, blue: 1, alpha: 1))
    context.fill(CGRect(x: 0, y: 0, width: width, height: height))
    let cgImage = context.makeImage()!
    #if canImport(UIKit)
    return UIImage(cgImage: cgImage)
    #else
    return UIImage(cgImage: cgImage, size: CGSize(width: width, height: height))
    #endif
}

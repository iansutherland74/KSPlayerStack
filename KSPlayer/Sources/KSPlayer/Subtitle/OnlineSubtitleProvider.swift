//
//  OnlineSubtitleProvider.swift
//  KSPlayer
//
//  Created by Cursor on 2026/5/18.
//

import Foundation

public struct OnlineSubtitleSearchRequest: Sendable, Equatable {
    public var query: String?
    public var languages: [String]
    public var fileURL: URL?
    public var movieHash: String?
    public var imdbID: Int?
    public var tmdbID: Int?
    public var userAgent: String?

    public init(
        query: String? = nil,
        languages: [String] = [],
        fileURL: URL? = nil,
        movieHash: String? = nil,
        imdbID: Int? = nil,
        tmdbID: Int? = nil,
        userAgent: String? = nil
    ) {
        let normalizedQuery = query?.trimmingCharacters(in: .whitespacesAndNewlines)
        self.query = normalizedQuery?.isEmpty == false ? normalizedQuery : nil
        self.languages = Self.normalizedLanguages(languages)
        self.fileURL = fileURL
        let normalizedMovieHash = movieHash?.trimmingCharacters(in: .whitespacesAndNewlines)
        self.movieHash = normalizedMovieHash?.isEmpty == false ? normalizedMovieHash : nil
        self.imdbID = imdbID
        self.tmdbID = tmdbID
        self.userAgent = userAgent
    }

    private static func normalizedLanguages(_ languages: [String]) -> [String] {
        var seen = Set<String>()
        return languages.compactMap { language in
            let normalized = language.trimmingCharacters(in: .whitespacesAndNewlines)
            guard !normalized.isEmpty else {
                return nil
            }
            let key = normalized.lowercased()
            guard seen.insert(key).inserted else {
                return nil
            }
            return normalized
        }
    }
}

public struct OnlineSubtitleSearchResult: Sendable, Equatable {
    public var providerID: String
    public var subtitleID: String
    public var name: String
    public var downloadURL: URL
    public var language: String?
    public var format: String?
    public var comment: String?
    public var delay: TimeInterval

    public init(
        providerID: String,
        subtitleID: String,
        name: String,
        downloadURL: URL,
        language: String? = nil,
        format: String? = nil,
        comment: String? = nil,
        delay: TimeInterval = 0
    ) {
        self.providerID = providerID
        self.subtitleID = subtitleID
        self.name = name
        self.downloadURL = downloadURL
        self.language = language
        self.format = format
        self.comment = comment
        self.delay = delay
    }
}

public protocol OnlineSubtitleProvider: Sendable {
    var providerID: String { get }
    var displayName: String { get }
    func searchSubtitles(request: OnlineSubtitleSearchRequest) async throws -> [OnlineSubtitleSearchResult]
}

public enum OnlineSubtitleProviderError: Error, Sendable, Equatable {
    case invalidHTTPResponse
    case httpStatus(Int)
    case rateLimited(retryAfter: TimeInterval?)
}

public final class OnlineSubtitleDataSouce: FileURLSubtitleDataSouce, SearchSubtitleDataSouce {
    public var infos = [any SubtitleInfo]()
    private let providers: [any OnlineSubtitleProvider]
    private let defaultLanguages: [String]
    private let userAgent: String?

    public init(providers: [any OnlineSubtitleProvider], languages: [String] = [], userAgent: String? = nil) {
        self.providers = providers
        let normalizedLanguages = Self.normalizedLanguages(languages)
        defaultLanguages = normalizedLanguages.isEmpty ? Self.normalizedLanguages(KSOptions.onlineSubtitleLanguages) : normalizedLanguages
        self.userAgent = userAgent
    }

    public func searchSubtitle(fileURL: URL?) async throws {
        infos = []
        guard let fileURL else {
            return
        }
        let query = fileURL.deletingPathExtension().lastPathComponent
        let movieHash = fileURL.isFileURL ? fileURL.shooterFilehash : nil
        let request = OnlineSubtitleSearchRequest(query: query, languages: defaultLanguages, fileURL: fileURL, movieHash: movieHash, userAgent: userAgent)
        try await updateInfos(request: request)
    }

    public func searchSubtitle(query: String?, languages: [String]) async throws {
        infos = []
        let request = OnlineSubtitleSearchRequest(query: query, languages: languages.isEmpty ? defaultLanguages : languages, userAgent: userAgent)
        try await updateInfos(request: request)
    }

    public func searchSubtitles(request: OnlineSubtitleSearchRequest) async throws -> [OnlineSubtitleSearchResult] {
        var results = [OnlineSubtitleSearchResult]()
        var firstError: Error?
        for provider in providers {
            try Task.checkCancellation()
            do {
                results.append(contentsOf: try await provider.searchSubtitles(request: request))
            } catch is CancellationError {
                throw CancellationError()
            } catch {
                if firstError == nil {
                    firstError = error
                }
            }
        }
        let uniqueResults = Self.rankedUniqueResults(results, request: request)
        if uniqueResults.isEmpty, let firstError {
            throw firstError
        }
        return uniqueResults
    }

    private func updateInfos(request: OnlineSubtitleSearchRequest) async throws {
        infos = try await searchSubtitles(request: request).map { result in
            let name = result.name.isEmpty ? result.downloadURL.lastPathComponent : result.name
            let info = URLSubtitleInfo(subtitleID: "\(result.providerID):\(result.subtitleID)", name: name, url: result.downloadURL, userAgent: request.userAgent)
            info.delay = result.delay
            info.comment = Self.comment(for: result)
            return info
        }
    }

    private static func normalizedLanguages(_ languages: [String]) -> [String] {
        OnlineSubtitleSearchRequest(query: nil, languages: languages).languages
    }

    private static func rankedUniqueResults(_ results: [OnlineSubtitleSearchResult], request: OnlineSubtitleSearchRequest) -> [OnlineSubtitleSearchResult] {
        var seenKeys = Set<String>()
        var rankedResults = [(result: OnlineSubtitleSearchResult, index: Int)]()
        for (index, originalResult) in results.enumerated() {
            var result = originalResult
            let providerID = result.providerID.trimmingCharacters(in: .whitespacesAndNewlines)
            let subtitleID = result.subtitleID.trimmingCharacters(in: .whitespacesAndNewlines)
            guard !providerID.isEmpty else {
                continue
            }
            result.providerID = providerID
            result.subtitleID = subtitleID.isEmpty ? result.downloadURL.absoluteString : subtitleID
            result.name = result.name.trimmingCharacters(in: .whitespacesAndNewlines)
            let idKey = "\(result.providerID.lowercased()):\(result.subtitleID.lowercased())"
            let urlKey = "url:\(result.downloadURL.absoluteString.lowercased())"
            guard seenKeys.insert(idKey).inserted, seenKeys.insert(urlKey).inserted else {
                continue
            }
            rankedResults.append((result, index))
        }
        return rankedResults.sorted { left, right in
            let leftScore = score(left.result, request: request)
            let rightScore = score(right.result, request: request)
            if leftScore == rightScore {
                return left.index < right.index
            }
            return leftScore > rightScore
        }.map(\.result)
    }

    private static func score(_ result: OnlineSubtitleSearchResult, request: OnlineSubtitleSearchRequest) -> Int {
        let name = result.name.isEmpty ? result.downloadURL.lastPathComponent : result.name
        let lowercasedName = name.lowercased()
        var score = 0
        if let query = request.query?.lowercased(), !query.isEmpty, lowercasedName.contains(query) {
            score += 20
        }
        if let fileURL = request.fileURL {
            let baseName = fileURL.deletingPathExtension().lastPathComponent.lowercased()
            if !baseName.isEmpty, lowercasedName.contains(baseName) {
                score += 40
            }
        }
        if let language = result.language?.lowercased(), request.languages.map({ $0.lowercased() }).contains(language) {
            score += 30
        }
        if let format = result.format?.lowercased(), ["srt", "ass", "ssa", "vtt"].contains(format) {
            score += 10
        }
        return score
    }

    private static func comment(for result: OnlineSubtitleSearchResult) -> String? {
        let metadata = [result.language, result.format, result.comment].compactMap { value -> String? in
            let normalized = value?.trimmingCharacters(in: .whitespacesAndNewlines)
            return normalized?.isEmpty == false ? normalized : nil
        }
        return metadata.isEmpty ? nil : metadata.joined(separator: " - ")
    }
}

public final class ShooterOnlineSubtitleProvider: OnlineSubtitleProvider, @unchecked Sendable {
    public let providerID = "shooter"
    public let displayName = "Shooter"

    public init() {}

    public func searchSubtitles(request: OnlineSubtitleSearchRequest) async throws -> [OnlineSubtitleSearchResult] {
        let fileHash = request.movieHash ?? request.fileURL?.shooterFilehash
        guard let urlRequest = Self.makeSearchRequest(fileURL: request.fileURL, fileHash: fileHash, userAgent: request.userAgent) else {
            return []
        }
        let data = try await OnlineSubtitleHTTPClient.data(for: urlRequest)
        return try Self.decodeSearchResults(data: data)
    }

    public static func makeSearchRequest(fileURL: URL?, fileHash: String?, userAgent: String? = nil) -> URLRequest? {
        guard let fileHash = fileHash?.trimmingCharacters(in: .whitespacesAndNewlines), !fileHash.isEmpty,
              let searchApi = URL(string: "https://www.shooter.cn/api/subapi.php")?
              .add(queryItems: ["format": "json", "pathinfo": fileURL?.lastPathComponent ?? "", "filehash": fileHash])
        else {
            return nil
        }
        var request = URLRequest(url: searchApi)
        request.httpMethod = "POST"
        request.applyOnlineSubtitleUserAgent(userAgent)
        return request
    }

    public static func decodeSearchResults(data: Data) throws -> [OnlineSubtitleSearchResult] {
        guard let json = try JSONSerialization.jsonObject(with: data) as? [[String: Any]] else {
            return []
        }
        return json.flatMap { subtitle -> [OnlineSubtitleSearchResult] in
            let delay = TimeInterval(subtitle["Delay"] as? Int ?? 0) / 1000.0
            guard let files = subtitle["Files"] as? [[String: String]] else {
                return []
            }
            return files.compactMap { file in
                guard let link = file["Link"], let url = URL(string: link) else {
                    return nil
                }
                let name = url.lastPathComponent
                return OnlineSubtitleSearchResult(providerID: "shooter", subtitleID: link, name: name, downloadURL: url, format: name.subtitleFormat, delay: delay)
            }
        }
    }
}

public final class AssrtOnlineSubtitleProvider: OnlineSubtitleProvider, @unchecked Sendable {
    public let providerID = "assrt"
    public let displayName = "ASSRT"
    private let token: String

    public init(token: String) {
        self.token = token
    }

    public func searchSubtitles(request: OnlineSubtitleSearchRequest) async throws -> [OnlineSubtitleSearchResult] {
        guard let query = request.query,
              let searchRequest = Self.makeSearchRequest(query: query, token: token, userAgent: request.userAgent)
        else {
            return []
        }
        let data = try await OnlineSubtitleHTTPClient.data(for: searchRequest)
        let ids = try Self.decodeSearchIDs(data: data)
        var results = [OnlineSubtitleSearchResult]()
        for id in ids {
            try Task.checkCancellation()
            guard let detailRequest = Self.makeDetailRequest(id: id, token: token, userAgent: request.userAgent) else {
                continue
            }
            let detailData = try await OnlineSubtitleHTTPClient.data(for: detailRequest)
            results.append(contentsOf: try Self.decodeDetailResults(data: detailData))
        }
        return results
    }

    public static func makeSearchRequest(query: String, token: String, userAgent: String? = nil) -> URLRequest? {
        let normalizedQuery = query.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !normalizedQuery.isEmpty,
              let searchApi = URL(string: "https://api.assrt.net/v1/sub/search")?.add(queryItems: ["q": normalizedQuery])
        else {
            return nil
        }
        return authorizedPostRequest(url: searchApi, token: token, userAgent: userAgent)
    }

    public static func makeDetailRequest(id: String, token: String, userAgent: String? = nil) -> URLRequest? {
        let normalizedID = id.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !normalizedID.isEmpty,
              let detailApi = URL(string: "https://api.assrt.net/v1/sub/detail")?.add(queryItems: ["id": normalizedID])
        else {
            return nil
        }
        return authorizedPostRequest(url: detailApi, token: token, userAgent: userAgent)
    }

    public static func decodeSearchIDs(data: Data) throws -> [String] {
        guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
              json["status"] as? Int == 0,
              let subDict = json["sub"] as? [String: Any],
              let subArray = subDict["subs"] as? [[String: Any]]
        else {
            return []
        }
        return subArray.compactMap { subtitle in
            if let id = subtitle["id"] as? Int {
                return String(id)
            }
            return subtitle["id"] as? String
        }
    }

    public static func decodeDetailResults(data: Data) throws -> [OnlineSubtitleSearchResult] {
        guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
              json["status"] as? Int == 0,
              let subDict = json["sub"] as? [String: Any],
              let subArray = subDict["subs"] as? [[String: Any]],
              let subtitle = subArray.first
        else {
            return []
        }
        if let fileList = subtitle["filelist"] as? [[String: String]] {
            return fileList.compactMap { file in
                guard let urlString = file["url"], let filename = file["f"], let url = URL(string: urlString) else {
                    return nil
                }
                return OnlineSubtitleSearchResult(providerID: "assrt", subtitleID: urlString, name: filename, downloadURL: url, format: filename.subtitleFormat)
            }
        }
        guard let urlString = subtitle["url"] as? String,
              let filename = subtitle["filename"] as? String,
              let url = URL(string: urlString)
        else {
            return []
        }
        return [OnlineSubtitleSearchResult(providerID: "assrt", subtitleID: urlString, name: filename, downloadURL: url, format: filename.subtitleFormat)]
    }

    private static func authorizedPostRequest(url: URL, token: String, userAgent: String?) -> URLRequest? {
        let normalizedToken = token.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !normalizedToken.isEmpty else {
            return nil
        }
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.addValue("Bearer \(normalizedToken)", forHTTPHeaderField: "Authorization")
        request.applyOnlineSubtitleUserAgent(userAgent)
        return request
    }
}

public final class OpenSubtitlesOnlineSubtitleProvider: OnlineSubtitleProvider, @unchecked Sendable {
    public let providerID = "opensubtitles"
    public let displayName = "OpenSubtitles"
    private let apiKey: String
    private let token: String?

    public init(apiKey: String, token: String? = nil) {
        self.apiKey = apiKey
        self.token = token
    }

    public func searchSubtitles(request: OnlineSubtitleSearchRequest) async throws -> [OnlineSubtitleSearchResult] {
        guard let searchRequest = Self.makeSearchRequest(request: request, apiKey: apiKey, token: token, userAgent: request.userAgent) else {
            return []
        }
        let data = try await OnlineSubtitleHTTPClient.data(for: searchRequest)
        let files = try Self.decodeSearchFiles(data: data)
        var results = [OnlineSubtitleSearchResult]()
        for file in files {
            try Task.checkCancellation()
            guard let downloadRequest = Self.makeDownloadRequest(fileID: file.fileID, apiKey: apiKey, token: token, userAgent: request.userAgent) else {
                continue
            }
            let downloadData = try await OnlineSubtitleHTTPClient.data(for: downloadRequest)
            if let result = try Self.decodeDownloadResult(data: downloadData, fallbackFileID: file.fileID, fallbackName: file.fileName) {
                results.append(result)
            }
        }
        return results
    }

    public static func makeSearchRequest(request: OnlineSubtitleSearchRequest, apiKey: String, token: String? = nil, userAgent: String? = nil) -> URLRequest? {
        var queryItems = [String: String]()
        if let query = request.query, !query.isEmpty {
            queryItems["query"] = query
        }
        if let imdbID = request.imdbID, imdbID != 0 {
            queryItems["imdb_id"] = String(imdbID)
        }
        if let tmdbID = request.tmdbID, tmdbID != 0 {
            queryItems["tmdb_id"] = String(tmdbID)
        }
        if let movieHash = request.movieHash, !movieHash.isEmpty {
            queryItems["moviehash"] = movieHash
        }
        if !request.languages.isEmpty {
            queryItems["languages"] = request.languages.joined(separator: ",")
        }
        guard !apiKey.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty,
              !queryItems.isEmpty,
              let searchApi = URL(string: "https://api.opensubtitles.com/api/v1/subtitles")?.add(queryItems: queryItems)
        else {
            return nil
        }
        return apiRequest(url: searchApi, apiKey: apiKey, token: token, userAgent: userAgent)
    }

    public static func makeDownloadRequest(fileID: Int, apiKey: String, token: String? = nil, userAgent: String? = nil) -> URLRequest? {
        guard fileID > 0,
              !apiKey.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty,
              let downloadApi = URL(string: "https://api.opensubtitles.com/api/v1/download")
        else {
            return nil
        }
        var request = apiRequest(url: downloadApi, apiKey: apiKey, token: token, userAgent: userAgent)
        request.httpMethod = "POST"
        request.addValue("application/json", forHTTPHeaderField: "Content-Type")
        request.httpBody = try? JSONSerialization.data(withJSONObject: ["file_id": fileID])
        return request
    }

    public static func decodeSearchFiles(data: Data) throws -> [(fileID: Int, fileName: String)] {
        guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
              let dataArray = json["data"] as? [[String: Any]]
        else {
            return []
        }
        return dataArray.flatMap { subtitle -> [(fileID: Int, fileName: String)] in
            guard let attributes = subtitle["attributes"] as? [String: Any],
                  let files = attributes["files"] as? [[String: Any]]
            else {
                return []
            }
            return files.compactMap { file in
                guard let fileID = file["file_id"] as? Int else {
                    return nil
                }
                return (fileID, file["file_name"] as? String ?? "")
            }
        }
    }

    public static func decodeDownloadResult(data: Data, fallbackFileID: Int, fallbackName: String) throws -> OnlineSubtitleSearchResult? {
        guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
              let link = json["link"] as? String,
              let url = URL(string: link)
        else {
            return nil
        }
        let fileName = json["file_name"] as? String ?? fallbackName
        return OnlineSubtitleSearchResult(providerID: "opensubtitles", subtitleID: String(fallbackFileID), name: fileName, downloadURL: url, format: fileName.subtitleFormat)
    }

    private static func apiRequest(url: URL, apiKey: String, token: String?, userAgent: String?) -> URLRequest {
        var request = URLRequest(url: url)
        request.addValue(apiKey.trimmingCharacters(in: .whitespacesAndNewlines), forHTTPHeaderField: "Api-Key")
        if let token = token?.trimmingCharacters(in: .whitespacesAndNewlines), !token.isEmpty {
            request.addValue("Bearer \(token)", forHTTPHeaderField: "Authorization")
        }
        request.applyOnlineSubtitleUserAgent(userAgent)
        return request
    }
}

private enum OnlineSubtitleHTTPClient {
    private static let retryableStatusCodes: Set<Int> = [408, 429, 500, 502, 503, 504]

    static func data(for request: URLRequest, retries: Int = 1) async throws -> Data {
        var attempt = 0
        while true {
            try Task.checkCancellation()
            do {
                let (data, response) = try await URLSession.shared.data(for: request)
                guard let httpResponse = response as? HTTPURLResponse else {
                    throw OnlineSubtitleProviderError.invalidHTTPResponse
                }
                guard (200..<300).contains(httpResponse.statusCode) else {
                    if attempt < retries, retryableStatusCodes.contains(httpResponse.statusCode) {
                        attempt += 1
                        try await waitBeforeRetry(httpResponse: httpResponse)
                        continue
                    }
                    if httpResponse.statusCode == 429 {
                        throw OnlineSubtitleProviderError.rateLimited(retryAfter: retryAfter(from: httpResponse))
                    }
                    throw OnlineSubtitleProviderError.httpStatus(httpResponse.statusCode)
                }
                return data
            } catch is CancellationError {
                throw CancellationError()
            } catch let error as OnlineSubtitleProviderError {
                throw error
            } catch {
                if attempt < retries {
                    attempt += 1
                    try await waitBeforeRetry(httpResponse: nil)
                    continue
                }
                throw error
            }
        }
    }

    private static func waitBeforeRetry(httpResponse: HTTPURLResponse?) async throws {
        let delay = min(retryAfter(from: httpResponse) ?? 0.25, 2)
        try await Task.sleep(nanoseconds: UInt64(delay * 1_000_000_000))
    }

    private static func retryAfter(from response: HTTPURLResponse?) -> TimeInterval? {
        guard let value = response?.value(forHTTPHeaderField: "Retry-After") else {
            return nil
        }
        if let seconds = TimeInterval(value) {
            return seconds
        }
        if let date = HTTPDateFormatter.formatter.date(from: value) {
            return max(0, date.timeIntervalSinceNow)
        }
        return nil
    }
}

private enum HTTPDateFormatter {
    static let formatter: DateFormatter = {
        let formatter = DateFormatter()
        formatter.locale = Locale(identifier: "en_US_POSIX")
        formatter.timeZone = TimeZone(secondsFromGMT: 0)
        formatter.dateFormat = "EEE',' dd MMM yyyy HH':'mm':'ss z"
        return formatter
    }()
}

private extension URLRequest {
    mutating func applyOnlineSubtitleUserAgent(_ userAgent: String?) {
        let normalized = userAgent?.trimmingCharacters(in: .whitespacesAndNewlines)
        if let normalized, !normalized.isEmpty {
            addValue(normalized, forHTTPHeaderField: "User-Agent")
        }
    }
}

private extension String {
    var subtitleFormat: String? {
        let pathExtension = (self as NSString).pathExtension.trimmingCharacters(in: .whitespacesAndNewlines)
        return pathExtension.isEmpty ? nil : pathExtension.lowercased()
    }
}

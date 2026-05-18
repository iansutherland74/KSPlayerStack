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
        self.query = query
        self.languages = languages
        self.fileURL = fileURL
        self.movieHash = movieHash
        self.imdbID = imdbID
        self.tmdbID = tmdbID
        self.userAgent = userAgent
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

public final class OnlineSubtitleDataSouce: FileURLSubtitleDataSouce, SearchSubtitleDataSouce {
    public var infos = [any SubtitleInfo]()
    private let providers: [any OnlineSubtitleProvider]
    private let defaultLanguages: [String]
    private let userAgent: String?

    public init(providers: [any OnlineSubtitleProvider], languages: [String] = [], userAgent: String? = nil) {
        self.providers = providers
        defaultLanguages = languages
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
        for provider in providers {
            try Task.checkCancellation()
            results.append(contentsOf: try await provider.searchSubtitles(request: request))
        }
        return results
    }

    private func updateInfos(request: OnlineSubtitleSearchRequest) async throws {
        infos = try await searchSubtitles(request: request).map { result in
            let name = result.name.isEmpty ? result.downloadURL.lastPathComponent : result.name
            let info = URLSubtitleInfo(subtitleID: "\(result.providerID):\(result.subtitleID)", name: name, url: result.downloadURL, userAgent: request.userAgent)
            info.delay = result.delay
            info.comment = result.comment
            return info
        }
    }
}

public final class ShooterOnlineSubtitleProvider: OnlineSubtitleProvider, @unchecked Sendable {
    public let providerID = "shooter"
    public let displayName = "Shooter"

    public init() {}

    public func searchSubtitles(request: OnlineSubtitleSearchRequest) async throws -> [OnlineSubtitleSearchResult] {
        let fileHash = request.movieHash ?? request.fileURL?.shooterFilehash
        guard let urlRequest = Self.makeSearchRequest(fileURL: request.fileURL, fileHash: fileHash) else {
            return []
        }
        let (data, _) = try await URLSession.shared.data(for: urlRequest)
        return try Self.decodeSearchResults(data: data)
    }

    public static func makeSearchRequest(fileURL: URL?, fileHash: String?) -> URLRequest? {
        guard let fileHash, !fileHash.isEmpty,
              let searchApi = URL(string: "https://www.shooter.cn/api/subapi.php")?
              .add(queryItems: ["format": "json", "pathinfo": fileURL?.lastPathComponent ?? "", "filehash": fileHash])
        else {
            return nil
        }
        var request = URLRequest(url: searchApi)
        request.httpMethod = "POST"
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
                return OnlineSubtitleSearchResult(providerID: "shooter", subtitleID: link, name: url.lastPathComponent, downloadURL: url, delay: delay)
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
              let searchRequest = Self.makeSearchRequest(query: query, token: token)
        else {
            return []
        }
        let (data, _) = try await URLSession.shared.data(for: searchRequest)
        let ids = try Self.decodeSearchIDs(data: data)
        var results = [OnlineSubtitleSearchResult]()
        for id in ids {
            try Task.checkCancellation()
            guard let detailRequest = Self.makeDetailRequest(id: id, token: token) else {
                continue
            }
            let (detailData, _) = try await URLSession.shared.data(for: detailRequest)
            results.append(contentsOf: try Self.decodeDetailResults(data: detailData))
        }
        return results
    }

    public static func makeSearchRequest(query: String, token: String) -> URLRequest? {
        guard !query.isEmpty,
              let searchApi = URL(string: "https://api.assrt.net/v1/sub/search")?.add(queryItems: ["q": query])
        else {
            return nil
        }
        return authorizedPostRequest(url: searchApi, token: token)
    }

    public static func makeDetailRequest(id: String, token: String) -> URLRequest? {
        guard !id.isEmpty,
              let detailApi = URL(string: "https://api.assrt.net/v1/sub/detail")?.add(queryItems: ["id": id])
        else {
            return nil
        }
        return authorizedPostRequest(url: detailApi, token: token)
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
                return OnlineSubtitleSearchResult(providerID: "assrt", subtitleID: urlString, name: filename, downloadURL: url)
            }
        }
        guard let urlString = subtitle["url"] as? String,
              let filename = subtitle["filename"] as? String,
              let url = URL(string: urlString)
        else {
            return []
        }
        return [OnlineSubtitleSearchResult(providerID: "assrt", subtitleID: urlString, name: filename, downloadURL: url)]
    }

    private static func authorizedPostRequest(url: URL, token: String) -> URLRequest {
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.addValue("Bearer \(token)", forHTTPHeaderField: "Authorization")
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
        guard let searchRequest = Self.makeSearchRequest(request: request, apiKey: apiKey, token: token) else {
            return []
        }
        let (data, _) = try await URLSession.shared.data(for: searchRequest)
        let files = try Self.decodeSearchFiles(data: data)
        var results = [OnlineSubtitleSearchResult]()
        for file in files {
            try Task.checkCancellation()
            guard let downloadRequest = Self.makeDownloadRequest(fileID: file.fileID, apiKey: apiKey, token: token) else {
                continue
            }
            let (downloadData, _) = try await URLSession.shared.data(for: downloadRequest)
            if let result = try Self.decodeDownloadResult(data: downloadData, fallbackFileID: file.fileID, fallbackName: file.fileName) {
                results.append(result)
            }
        }
        return results
    }

    public static func makeSearchRequest(request: OnlineSubtitleSearchRequest, apiKey: String, token: String? = nil) -> URLRequest? {
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
        guard !queryItems.isEmpty,
              let searchApi = URL(string: "https://api.opensubtitles.com/api/v1/subtitles")?.add(queryItems: queryItems)
        else {
            return nil
        }
        return apiRequest(url: searchApi, apiKey: apiKey, token: token)
    }

    public static func makeDownloadRequest(fileID: Int, apiKey: String, token: String? = nil) -> URLRequest? {
        guard fileID > 0,
              let downloadApi = URL(string: "https://api.opensubtitles.com/api/v1/download")
        else {
            return nil
        }
        var request = apiRequest(url: downloadApi, apiKey: apiKey, token: token)
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
        return OnlineSubtitleSearchResult(providerID: "opensubtitles", subtitleID: String(fallbackFileID), name: fileName, downloadURL: url)
    }

    private static func apiRequest(url: URL, apiKey: String, token: String?) -> URLRequest {
        var request = URLRequest(url: url)
        request.addValue(apiKey, forHTTPHeaderField: "Api-Key")
        if let token {
            request.addValue("Bearer \(token)", forHTTPHeaderField: "Authorization")
        }
        return request
    }
}

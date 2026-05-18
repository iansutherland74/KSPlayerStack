//
//  SSL.swift
//
//
//  Created by kintan on 12/26/23.
//

import Foundation

class BuildOpenSSL: BaseBuild {
    init() {
        super.init(library: .openssl)
    }

    override func frameworks() throws -> [String] {
        ["libssl", "libcrypto"]
    }

    override func arguments(platform: PlatformType, arch: ArchType) -> [String] {
        var array = [
            "--prefix=\(thinDir(platform: platform, arch: arch).path)",
            "no-async", "no-shared", "no-dso", "no-engine", "no-tests",
            arch == .x86_64 ? "darwin64-x86_64" : arch == .arm64e ? "iphoneos-cross" : "darwin64-arm64",
        ]
        if [PlatformType.tvos, .tvsimulator, .watchos, .watchsimulator].contains(platform) {
            array.append("-DHAVE_FORK=0")
        }
        return array
    }
}

class BuildBoringSSL: BaseBuild {
    init() {
        super.init(library: .boringssl)
        if Utility.shell("which go") == nil {
            Utility.shell("brew install go")
        }
    }
}

class BuildLibreSSL: BaseBuild {
    init() {
        super.init(library: .libtls)
        generateReleaseFilesIfNeeded()
    }

    private func generateReleaseFilesIfNeeded() {
        let generatedFiles = [
            "VERSION",
            "crypto/VERSION",
            "crypto/crypto.sym",
            "ssl/VERSION",
            "ssl/ssl.sym",
            "tls/VERSION",
            "tls/tls.sym",
        ]
        if generatedFiles.allSatisfy({ FileManager.default.fileExists(atPath: (directoryURL + $0).path) }) {
            return
        }
        try! prepareOpenBSDCheckout()
        try! Utility.launch(executableURL: directoryURL + "update.sh", arguments: [], currentDirectoryURL: directoryURL)
    }

    private func prepareOpenBSDCheckout() throws {
        let openbsdBranch = try String(contentsOf: directoryURL + "OPENBSD_BRANCH")
            .trimmingCharacters(in: .whitespacesAndNewlines)
        let openbsdURL = directoryURL + "openbsd"
        let openbsdRepository = (ProcessInfo.processInfo.environment["LIBRESSL_GIT"] ?? "https://github.com/libressl") + "/openbsd"
        if !FileManager.default.fileExists(atPath: openbsdURL.path) {
            try Utility.launch(path: "/usr/bin/git", arguments: ["clone", "--depth", "8", "--branch", openbsdBranch, openbsdRepository, openbsdURL.path], currentDirectoryURL: directoryURL)
        }
        try Utility.launch(path: "/usr/bin/git", arguments: ["fetch", "origin", openbsdBranch, "--depth", "8"], currentDirectoryURL: openbsdURL)
        try Utility.launch(path: "/usr/bin/git", arguments: ["checkout", "-B", openbsdBranch, "FETCH_HEAD"], currentDirectoryURL: openbsdURL)
        let openbsdTag = "libressl-\(library.version)"
        try Utility.launch(path: "/usr/bin/git", arguments: ["fetch", "origin", "refs/tags/\(openbsdTag):refs/tags/\(openbsdTag)", "--depth", "1"], currentDirectoryURL: openbsdURL)
    }

    override func arguments(platform _: PlatformType, arch _: ArchType) -> [String] {
        [
            "-DLIBRESSL_APPS=OFF",
            "-DLIBRESSL_TESTS=OFF",
        ]
    }

    override func cFlags(platform: PlatformType, arch: ArchType) -> [String] {
        var cFlags = super.cFlags(platform: platform, arch: arch)
        if [PlatformType.tvos, .tvsimulator, .watchos, .watchsimulator].contains(platform) {
            cFlags.append("-DOPENSSL_NO_SPEED=1")
        }
        return cFlags
    }

    override func environment(platform: PlatformType, arch: ArchType) -> [String: String] {
        var env = super.environment(platform: platform, arch: arch)
        if [PlatformType.tvos, .tvsimulator, .watchos, .watchsimulator].contains(platform) {
            env["CFLAGS"]? += " -DOPENSSL_NO_SPEED=1"
        }
        return env
    }
}

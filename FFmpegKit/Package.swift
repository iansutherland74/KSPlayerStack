// swift-tools-version: 5.9
// Use 5.9 so the BuildFFmpeg command plugin compiles under Xcode 26.
// Built xcframeworks still target visionOS 26 via Plugins/BuildFFmpeg (minVersion).
import Foundation
import PackageDescription

func pkgConfigTokens(_ arguments: [String]) -> [String] {
    let process = Process()
    process.executableURL = URL(fileURLWithPath: "/usr/bin/env")
    process.arguments = ["pkg-config"] + arguments

    let output = Pipe()
    process.standardOutput = output
    process.standardError = Pipe()

    do {
        try process.run()
        process.waitUntilExit()
    } catch {
        return []
    }

    guard process.terminationStatus == 0 else {
        return []
    }

    let data = output.fileHandleForReading.readDataToEndOfFile()
    let string = String(decoding: data, as: UTF8.self)
    return string.split(whereSeparator: { $0 == " " || $0 == "\n" || $0 == "\t" }).map(String.init)
}

// SwiftPM rejects -D_THREAD_SAFE from pkg-config on Apple platforms. Keep the
// rest of SDL2's pkg-config output intact for ffplay.
let sdl2CFlags = pkgConfigTokens(["--cflags", "sdl2"]).filter { $0 != "-D_THREAD_SAFE" }
let sdl2Libs = pkgConfigTokens(["--libs", "sdl2"])
let sdl2CSettings: [CSetting] = sdl2CFlags.isEmpty ? [] : [.unsafeFlags(sdl2CFlags)]
let sdl2LinkerSettings: [LinkerSetting] = sdl2Libs.isEmpty ? [] : [.unsafeFlags(sdl2Libs)]
let gmpLibs = pkgConfigTokens(["--libs", "gmp"])
let gmpLinkerSettings: [LinkerSetting] = gmpLibs.isEmpty ? [] : [.unsafeFlags(gmpLibs, .when(platforms: [.macOS]))]

let package = Package(
    name: "FFmpegKit",
    defaultLocalization: "en",
    platforms: [.macOS(.v10_15), .macCatalyst(.v14), .iOS(.v13), .tvOS(.v13),
                .visionOS(.v1)],
    products: [
        .library(
            name: "FFmpegKit",
//            type: .static,
            targets: ["FFmpegKit"]
        ),
        .library(name: "Libavcodec", targets: ["Libavcodec"]),
        .library(name: "Libavfilter", targets: ["Libavfilter"]),
        .library(name: "Libavformat", targets: ["Libavformat"]),
        .library(name: "Libavutil", targets: ["Libavutil"]),
        .library(name: "Libswresample", targets: ["Libswresample"]),
        .library(name: "Libswscale", targets: ["Libswscale"]),
        // Crypto/TLS libraries exposed for consumers that link Libav*
        // individually without the full FFmpegKit target
        .library(name: "gmp", targets: ["gmp"]),
        .library(name: "nettle", targets: ["nettle"]),
        .library(name: "hogweed", targets: ["hogweed"]),
        .library(name: "gnutls", targets: ["gnutls"]),
        .library(name: "libass", targets: ["libfreetype", "libfribidi", "libharfbuzz", "libunibreak", "libass"]),
        .library(name: "libmpv", targets: ["FFmpegKit", "libass", "libmpv"]),
        .executable(name: "ffmpeg", targets: ["ffmpeg"]),
        .executable(name: "ffplay", targets: ["ffplay"]),
        .executable(name: "ffprobe", targets: ["ffprobe"]),
        .plugin(name: "BuildFFmpeg", targets: ["BuildFFmpeg"]),
    ],
    dependencies: [
        // Dependencies declare other packages that this package depends on.
    ],
    targets: [
        .target(
            name: "FFmpegKit",
            dependencies: [
                "MoltenVK",
                "libshaderc_combined",
                "lcms2",
                "libdav1d",
                "libplacebo",
                .target(name: "libzvbi", condition: .when(platforms: [.macOS, .iOS, .tvOS, .visionOS])),
                "libsrt",
                "libfreetype", "libfribidi", "libharfbuzz", "libunibreak", "libass",
                "libfontconfig",
                .target(name: "libudfread", condition: .when(platforms: [.macOS, .iOS, .tvOS, .visionOS])),
                .target(name: "libbluray", condition: .when(platforms: [.macOS, .iOS, .tvOS, .visionOS])),
                "gmp", "nettle", "hogweed", "gnutls",
                "libsmbclient",
                "Libavcodec", "Libavdevice", "Libavfilter", "Libavformat", "Libavutil", "Libswresample", "Libswscale",
            ],
            linkerSettings: [
                .linkedFramework("AudioToolbox"),
                .linkedFramework("AVFAudio"),
                .linkedFramework("AVFoundation"),
                .linkedFramework("CoreAudio"),
                .linkedFramework("CoreVideo"),
                .linkedFramework("CoreFoundation"),
                .linkedFramework("CoreGraphics"),
                .linkedFramework("CoreMedia"),
                .linkedFramework("Cocoa", .when(platforms: [.macOS])),
                .linkedFramework("DiskArbitration", .when(platforms: [.macOS])),
                .linkedFramework("Foundation"),
                .linkedFramework("Metal"),
                .linkedFramework("IOKit", .when(platforms: [.macOS, .iOS, .visionOS, .macCatalyst])),
                .linkedFramework("IOSurface"),
                .linkedFramework("QuartzCore"),
                .linkedFramework("Security"),
                .linkedFramework("UIKit", .when(platforms: [.iOS, .tvOS, .visionOS, .macCatalyst])),
                .linkedFramework("VideoToolbox"),
                .linkedLibrary("bz2"),
                .linkedLibrary("c++"),
                .linkedLibrary("expat", .when(platforms: [.macOS])),
                .linkedLibrary("iconv"),
                .linkedLibrary("resolv"),
                .linkedLibrary("xml2"),
                .linkedLibrary("z"),
            ] + gmpLinkerSettings
        ),
        .executableTarget(
            name: "ffplay",
            dependencies: [
                "fftools",
                "SDL2",
            ],
            cSettings: sdl2CSettings,
            linkerSettings: sdl2LinkerSettings
        ),
        .executableTarget(
            name: "ffprobe",
            dependencies: [
                "fftools",
            ]
        ),
        .executableTarget(
            name: "ffmpeg",
            dependencies: [
                "fftools",
            ]
        ),
        .target(
            name: "fftools",
            dependencies: [
                "FFmpegKit",
            ]
        ),
        .systemLibrary(
            name: "SDL2",
            providers: [
                .brew(["sdl2"]),
            ]
        ),
//        .target(
//            name: "libavutil",
//            cSettings: [.headerSearchPath("../")]
//        ),
//        .executableTarget(
//            name: "BuildFFmpegPlugin",
//            path: "Plugins/BuildFFmpeg"
//        ),
        .plugin(
            name: "BuildFFmpeg", capability: .command(
                intent: .custom(
                    verb: "BuildFFmpeg",
                    description: "You can customize FFmpeg and then compile FFmpeg"
                ),
                permissions: [
                    //                    .writeToPackageDirectory(reason: "This command compile FFmpeg and generate xcframework. compile FFmpeg need brew install nasm sdl2 cmake. So you need add --allow-writing-to-directory /usr/local/ --allow-writing-to-directory ~/Library/ or add --disable-sandbox"),
//                    .allowNetworkConnections(scope: .all(), reason: "The plugin must connect to a remote server to brew install nasm sdl2 cmake"),
                ]
            )
        ),
        .binaryTarget(
            name: "MoltenVK",
            path: "Sources/MoltenVK.xcframework"
        ),
        .binaryTarget(
            name: "libshaderc_combined",
            path: "Sources/libshaderc_combined.xcframework"
        ),

        .binaryTarget(
            name: "lcms2",
            path: "Sources/lcms2.xcframework"
        ),
        .binaryTarget(
            name: "libplacebo",
            path: "Sources/libplacebo.xcframework"
        ),
        .binaryTarget(
            name: "libdav1d",
            path: "Sources/libdav1d.xcframework"
        ),
        .binaryTarget(
            name: "Libavcodec",
            path: "Sources/Libavcodec.xcframework"
        ),
        .binaryTarget(
            name: "Libavdevice",
            path: "Sources/Libavdevice.xcframework"
        ),
        .binaryTarget(
            name: "Libavfilter",
            path: "Sources/Libavfilter.xcframework"
        ),
        .binaryTarget(
            name: "Libavformat",
            path: "Sources/Libavformat.xcframework"
        ),
        .binaryTarget(
            name: "Libavutil",
            path: "Sources/Libavutil.xcframework"
        ),
        .binaryTarget(
            name: "Libswresample",
            path: "Sources/Libswresample.xcframework"
        ),
        .binaryTarget(
            name: "Libswscale",
            path: "Sources/Libswscale.xcframework"
        ),
        .binaryTarget(
            name: "libsrt",
            path: "Sources/libsrt.xcframework"
        ),
        .binaryTarget(
            name: "libzvbi",
            path: "Sources/libzvbi.xcframework"
        ),
        .binaryTarget(
            name: "libfreetype",
            path: "Sources/libfreetype.xcframework"
        ),
        .binaryTarget(
            name: "libfribidi",
            path: "Sources/libfribidi.xcframework"
        ),
        .binaryTarget(
            name: "libharfbuzz",
            path: "Sources/libharfbuzz.xcframework"
        ),
        .binaryTarget(
            name: "libunibreak",
            path: "Sources/libunibreak.xcframework"
        ),
        .binaryTarget(
            name: "libass",
            path: "Sources/libass.xcframework"
        ),
        .binaryTarget(
            name: "libmpv",
            path: "Sources/libmpv.xcframework"
        ),
        .binaryTarget(
            name: "gmp",
            path: "Sources/gmp.xcframework"
        ),
        .binaryTarget(
            name: "nettle",
            path: "Sources/nettle.xcframework"
        ),
        .binaryTarget(
            name: "hogweed",
            path: "Sources/hogweed.xcframework"
        ),
        .binaryTarget(
            name: "libfontconfig",
            path: "Sources/libfontconfig.xcframework"
        ),
        .binaryTarget(
            name: "libudfread",
            path: "Sources/libudfread.xcframework"
        ),
        .binaryTarget(
            name: "libbluray",
            path: "Sources/libbluray.xcframework"
        ),
        .binaryTarget(
            name: "gnutls",
            path: "Sources/gnutls.xcframework"
        ),
        .binaryTarget(
            name: "libsmbclient",
            path: "Sources/libsmbclient.xcframework"
        ),
//        .binaryTarget(
//            name: "libssl",
//            path: "Sources/libssl.xcframework"
//        ),
//        .binaryTarget(
//            name: "libcrypto",
//            path: "Sources/libcrypto.xcframework"
//        ),
    ]
)

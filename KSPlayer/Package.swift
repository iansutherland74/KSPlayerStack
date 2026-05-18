// swift-tools-version: 6.2
import PackageDescription

let package = Package(
    name: "KSPlayer",
    defaultLocalization: "en",
    platforms: [.macOS(.v26), .macCatalyst(.v26), .iOS(.v26), .tvOS(.v26),
                .visionOS(.v26)],
    products: [
        .library(
            name: "KSPlayer",
            targets: ["KSPlayer"]
        ),
    ],
    dependencies: [
        .package(path: "../FFmpegKit"),
    ],
    targets: [
        .target(
            name: "KSPlayer",
            dependencies: [
                .product(name: "FFmpegKit", package: "FFmpegKit"),
                .product(name: "Libavcodec", package: "FFmpegKit"),
                .product(name: "Libavfilter", package: "FFmpegKit"),
                .product(name: "Libavformat", package: "FFmpegKit"),
                .product(name: "Libavutil", package: "FFmpegKit"),
                .product(name: "Libswresample", package: "FFmpegKit"),
                .product(name: "Libswscale", package: "FFmpegKit"),
                "DisplayCriteria",
            ],
            resources: [.process("Metal/Shaders.metal")],
            swiftSettings: [
                .enableExperimentalFeature("StrictConcurrency"),
            ]
        ),
        .target(
            name: "DisplayCriteria"
        ),
        .testTarget(
            name: "KSPlayerTests",
            dependencies: ["KSPlayer"],
            resources: [.process("Resources")]
        ),
    ]
)

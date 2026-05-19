![Build Status](https://img.shields.io/badge/build-%20passing%20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-%20iOS%20macOS%20tvOS%20visionOS%20-blue.svg)
![License](https://img.shields.io/badge/license-GPL-blue.svg)
# KSPlayer

KSPlayer is a powerful media play framework for iOS, tvOS, macOS, xrOS, visionOS, Mac Catalyst. based on AVPlayer and FFmpeg, support AppKit/UIKit/SwiftUI.

English | [简体中文](./README_CN.md)

## Communication

If you have a commercial project that requires a custom player, or would like to receive a paid consultation, please email me.

- Email : kingslay@icloud.com

## License
KSPlayer defaults to the GPL license (requires open-sourcing your own project code), and we hope everyone will consciously respect the licensing agreement of the KSPlayer project. Additionally, there is a paid version that adopts the LGPL license (contact us).

If due to commercial reasons, you prefer not to adhere to the GPL license  or the LGPL license, you can contact us. Through our authorization, you can obtain a more flexible licensing agreement.

## Features
Functional differences between GPL version and LGPL version.
Some features of the LGPL version require a one-time payment, which I have used 💰 to mark them out.

To experience the powerful features of the LGPL version, you can download the app from the App Store. [App Store Link](https://apps.apple.com/app/tracyplayer/id6450770064)


| Feature     | LGPL      | GPL    |
| ----------- | --------- | ------ |
|Video upscaling |💰|❌|
|ProgressBar Preview |💰|❌|
|Precache data to Hard Drive|💰|❌|
|Video switching with zero delay|💰|❌|
|Audio Passthrough Output by Wi-Fi|💰|❌|
|Dovi P5 displays HDR (not overheating)|💰|❌|
|Live streaming supports rewind viewing|💰|✅|
|ISO Blu-ray disc playback on all Apple platforms|💰|❌|
|Simultaneous playback of separate audio and video URLs|✅|❌|
|Offline AI real-time subtitle generation and translation|💰|❌|
|ProAVPlayer supports MKV, native Dolby Vision and Dolby Atmos.|💰|❌|
|Play videos in a small window in the App (resumable, supports iOS, tvOS and visionOS)|💰|❌|
|Dolby AC-4 detection and metadata|✅|❌|
|Swift Concurrency|✅|❌|
|AV1 hardware decoding|✅|❌|
|Word-by-word subtitles|✅|❌|
|Text subtitle translation|✅|❌|
|Use System Caption Appearance|✅|❌|
|Record video clips at any time|✅|❌|
|Smoothly Play 8K or 120 FPS Video|✅|❌|
|Display Subtitles with HDR Effects|✅|❌|
|Video download and format conversion|✅|❌|
|External image subtitles, such as SUP|✅|❌|
|Main subtitles and Secondary subtitles|✅|❌|
|Adjust Saturation, Brightness, and Contrast|✅|❌|
|Picture in Picture supports native subtitle display|✅|❌|
|Annex-B async hardware decoding(Live Stream)|✅|❌|
|Use the fonts in the video to render subtitles|✅|❌|
|Use memory cache for fast seek in short time range|✅|❌|
|KSMEPlayer supports all demuxing and decoding formats|✅|❌|
|Full display of ass subtitles effect(Render as image using libass)|✅|❌|
|FFmpeg version|8.1.0|6.1.0|
|Record video|✅|✅|
|360° panorama video|✅|✅|
|Opt-in 2D-to-3D pseudo-stereo/depth provider hook|✅|✅|
|Picture in Picture|✅|✅|
|Hardware accelerator|✅|✅|
|Seamless loop playback|✅|✅|
|De-interlace auto detect|✅|✅|
|Multichannel Audio/Spatial Audio|✅|✅|
|4k/HDR/HDR10/HDR10+/Dolby Vision|✅|✅|
|Custom url protocols such as nfs/smb/UPnP |✅|✅|
|Text subtitle/Image subtitle/Closed Captions|✅|✅|
|Search Online Subtitles(shooter/assrt/opensubtitles)|✅|✅|
|Low latency 4K live video streaming (less than 200ms on LAN)|✅|✅|
|Automatically switch to multi-bitrate streams based on network|✅|✅|


## Playback routing notes

KSPlayer chooses between AVFoundation (`KSAVPlayer`) and FFmpeg/Metal (`KSMEPlayer`) by capability. Separate audio/video URLs, AirPlay-oriented wireless routes, and AVFoundation-supported containers stay on the native path so system Dolby Vision, Dolby Atmos, external playback, and route sharing can work where Apple supports them. Matroska/WebM containers (`.mkv`, `.mk3d`, `.mka`, `.mks`, `.webm`), Blu-ray sources, FFmpeg-only URL schemes, VR display, 360° panorama mode, stereoscopic 3D mode, 2D-to-3D conversion, offline subtitle generation, color adjustment, and VideoToolbox upscaling use `KSMEPlayer`.

Custom network protocols such as `smb`, `smb2`, `nfs`, `upnp`, `dlna`, `rtsp`, `rtmp`, `ftp`, `sftp`, `srt`, and `udp` are treated as FFmpeg-only inputs and avoid AVPlayer fallback. FFmpeg protocol whitelists are extended only when an app has supplied a constrained whitelist. Apps that need their own resolver or authenticated byte source can subclass `KSOptions` and override `process(url:)` to return an `AbstractAVIOContext`; disk precache and whole-file HTTP export remain limited to direct HTTP(S) media, and diagnostics redact URL userinfo and query values.

## Adaptive bitrate switching

For separate `KSPlayerResourceDefinition` entries, opt in with `KSOptions.isAdaptiveBitrateSwitchingEnabled = true` and provide each definition's `bandwidth` in bits per second when known. `VideoPlayerView` estimates network throughput from FFmpeg byte counters, combines it with buffer health, rebuffer count, live/low-latency state, and `KSAdaptiveBitrateSwitchingPolicy`, then switches to the highest sustainable definition. The policy uses cooldown and throughput safety margins to avoid quality oscillation, and `adaptiveBitrateSwitchingDiagnostic` exposes the latest decision inputs.

HLS, DASH, Smooth Streaming, and HLS-like manifest URLs (`.m3u8`, `.mpd`, `.ism`, `.isml`) are not switched by KSPlayer's resource-level ABR; their variant selection is left to AVFoundation or the demuxer. Low-latency live playback keeps automatic upgrades disabled by default and uses a smaller downgrade buffer threshold so intentionally shallow live buffers do not immediately force lower quality.

## Subtitle support notes

KSPlayer supports external text subtitles (`.srt`, `.vtt`, `.ass`, `.ssa`), external image subtitles (`.sup`, `.pgs`), embedded FFmpeg subtitle streams, AVFoundation legible selections, and FFmpeg-discovered CEA-608 closed captions. ASS/SSA can use the text parser fallback or libass bitmap rendering when the libass module is linked and `KSOptions.isAssSubtitleImageRenderingEnabled` is true. External image subtitles and KSPlayer-owned text/image overlays render in the player view; AVFoundation native legible selections are the default subtitle path that can remain visible in system Picture in Picture. Apps that need KSPlayer overlay subtitles in KSMEPlayer sample-buffer PiP can opt in with `options.pictureInPictureSubtitlePolicy = .burnIn`; the current burn-in path composites primary/secondary text, external image subtitles, and libass bitmap subtitles into SDR CoreGraphics-readable sample buffers while PiP is active. Native AVPlayer PiP still cannot burn KSPlayer overlay layers into video frames, and Metal-only or HDR frames report an unavailable burn-in diagnostic instead of claiming subtitle visibility.

Online subtitle search is opt-in. Configure `KSOptions.onlineSubtitleProviders` with `ShooterOnlineSubtitleProvider`, `AssrtOnlineSubtitleProvider(token:)`, and/or `OpenSubtitlesOnlineSubtitleProvider(apiKey:token:)`; KSPlayer does not ship provider secrets. `KSOptions.onlineSubtitleLanguages` supplies default language hints when the search UI does not pass explicit languages, and downloaded provider results are exposed as normal external subtitle choices so primary/secondary selection, caching, and external subtitle translation keep using the existing subtitle pipeline.

For Dolby media, this means MP4/MOV/HLS Dolby Vision or Dolby Atmos content can remain native when AVFoundation supports the source and output route. Native tracks read Dolby Vision `dvcC`/`dvvC`/`dvwC` configuration atoms when AVFoundation exposes them, including profile/level/compatibility diagnostics for profiles such as P5, P7, P8, P10, and P11. MKV Dolby Vision/Atmos falls back to `KSMEPlayer`: Dolby Vision metadata and HDR fallback state are preserved for rendering diagnostics, while supported FFmpeg audio is output as decoded PCM. AC-4 tracks are labeled and preserved as Dolby metadata when demuxed, but FFmpeg 8.1 does not expose a public AC-4 decoder/parser, so KSPlayer marks AC-4 as unsupported on the FFmpeg path and leaves any native AC-4 playback to Apple's AVPlayer capabilities.

Dolby Vision Profile 7 UHD Blu-ray sources are reported honestly through `DolbyVisionPlaybackDiagnostic` and `KSOptions.dolbyVisionPlaybackDiagnostic`. KSPlayer records FFmpeg DOVI configuration side data, packet-level RPU NAL units, enhancement-layer NAL observations, FFmpeg frame RPU buffers, and `AVDOVIMetadata` when FFmpeg/libdovi exposes it. The open pipeline can split interleaved HEVC samples into BL, EL, and RPU payloads and has timestamp-alignment groundwork for dual-layer frames. RPU metadata can distinguish MEL-compatible Profile 7 from FEL when the decoded mapping reports no NLQ residual mapping versus FEL NLQ residual mapping. MEL is treated as an HDR10 base-layer fallback with RPU metadata preserved for diagnostics. By default, FEL full composition is not claimed: when no compositor is configured, KSPlayer keeps the explicit HDR10 base-layer fallback with a public "composition unavailable" diagnostic.

Private/education builds can require FEL composition instead of allowing BL-only fallback:

```swift
let options = KSOptions()
options.dolbyVisionFELPlaybackPolicy = .requireFullComposition
options.dolbyVisionFELCompositorBackend = FelBakerDolbyVisionFELCompositorBackend(
    libraryURL: felBakerShimURL
)
```

The optional `FelBakerDolbyVisionFELCompositorBackend` adapter looks for a local dynamic shim with the C ABI documented in `Documents/KSPlayerFelBakerShim.h`. A buildable private/education scaffold lives in `Tools/FelBakerShim`: run `make clean smoke` there to build and load-test the default `build/libKSPlayerFelBakerShim.dylib`. The default shim is intentionally a stub: it exports the ABI, reports zero FEL capabilities, and returns `KSPLAYER_FELBAKER_STATUS_NOT_IMPLEMENTED` from composition so KSPlayer never pretends FEL residual reconstruction occurred. Private builds can use `make clean real-direct` after placing a FelBaker checkout and libdovi artifacts under `Tools/FelBakerShim/external/felbaker`, or `make clean real-custom PRIVATE_SOURCES=/path/to/adapter.cpp` for a private FelBaker-derived core. Report `KSPLAYER_FELBAKER_CAPABILITY_FULL_FEL_COMPOSITION` only when the shim actually writes a fully composed frame into the supplied output `CVPixelBuffer`. If `.requireFullComposition` is set and no available backend is configured, KSPlayer reports `fullEnhancementLayerCompositionRequiredUnavailable` and blocks misleading fallback playback once Profile 7 EL/FEL inputs are detected. No proprietary Dolby or Apple private APIs are used or required by this hook, and KSPlayer does not vendor FelBaker GPL source in the core package.

HDR10+ dynamic metadata is parsed from FFmpeg packet/frame side data into `HDR10PlusMetadata` on the `KSMEPlayer` path when FFmpeg exposes `AVDynamicHDRPlus`. The default keeps the system display-layer path available so platform HDR10+ handling can be used where Apple supports it. Apps that set `hdr10PlusToneMappingPolicy = .metalDynamicToneMapping` force the Metal path and apply KSPlayer's conservative PQ-domain HDR10+ shader using target-display luminance, maxSCL/average maxRGB, knee point, Bezier anchors, rectangular processing windows, elliptical pixel selectors, actual peak luminance grid caps, color saturation weights, and weighted/layered overlap modes. The window metadata is sent through a GPU buffer with a practical cap of 64 processing windows per frame; additional windows are ignored until the cap is raised or replaced with a larger allocation policy.

## Seamless loop playback

Use `KSOptions.isLoopPlay` with `KSOptions.isSeamlessLoopEnabled` for short, finite, seekable media when the loop boundary should avoid an end-of-item pause:

```swift
let options = KSOptions()
options.isLoopPlay = true
options.isSeamlessLoopEnabled = true
playerView.set(url: url, options: options)
```

`KSAVPlayer` uses AVFoundation queue looping when available. `KSMEPlayer` prebuffers the next demux pass only for async audio/video packet queues and falls back to normal end handling for live or nonseekable sources.

## 360° panorama video

Use `KSOptions.panoramaMode` to opt in to equirectangular 360° rendering:

```swift
let options = KSOptions()
options.panoramaMode = .automatic
playerView.set(url: url, options: options)
```

`.automatic` routes single-URL playback through `KSMEPlayer`, reads FFmpeg spherical side data plus common projection metadata, and switches recognized equirectangular video from flat `.plane` display to the Metal sphere renderer. `.equirectangular` forces sphere rendering when the source lacks usable metadata. Use `options.panoramaStereoLayout` and `options.panoramaFieldOfView` to force side-by-side/top-and-bottom and 180°/360° behavior when metadata is missing. Cubemap, tiled, unknown, missing, or explicitly flat metadata stays flat unless the app forces `.equirectangular`.

Panorama rendering uses the existing VR display controls: drag gestures adjust view direction, and `KSOptions.enableSensor` controls device-motion look-around on platforms with UIKit and CoreMotion. Separate audio/video playback and wireless route playback stay on `KSAVPlayer`, so panorama rendering is not applied there.

## Stereoscopic 3D video

Use `KSOptions.stereoscopicVideoMode` for regular packed 3D video that is not panorama/XR:

```swift
let options = KSOptions()
options.stereoscopicVideoMode = .automatic
options.stereoscopicVideoEye = .left
playerView.set(url: url, options: options)
```

`.automatic` routes single-URL playback through `KSMEPlayer`, reads common side-by-side/top-and-bottom metadata, and crops the selected eye in the Metal renderer so flat 3D sources are not shown as squashed packed frames on normal 2D displays. Use `.sideBySide` or `.topAndBottom` when metadata is absent. Full platform XR stereo presentation is still limited to the existing `.vrBox` panorama path; flat 3D uses a selected-eye 2D fallback.

## 2D-to-3D conversion

2D-to-3D conversion is an Apple Vision Pro / visionOS-only feature. On iOS, tvOS, and macOS, these options report an unavailable diagnostic and normal playback routing is preserved. Existing side-by-side/top-and-bottom 3D video and panorama stereo rendering remain separate cross-platform features.

On visionOS, use `KSOptions.video2DTo3DMode` to opt in ordinary 2D video to generated stereo output:

```swift
let options = KSOptions()
options.video2DTo3DMode = .pseudoStereo
options.video2DTo3DDepthStrength = 0.35
options.video2DTo3DDepthDistance = 1.0
options.video2DTo3DDepthCurvature = 1.0
options.video2DTo3DOutputLayout = .sideBySide
playerView.set(url: url, options: options)
```

`.pseudoStereo` applies a conservative Metal-side horizontal disparity approximation; it is not AI depth reconstruction. `.depthMapPreferred` asks an app-provided `VideoDepthEstimationProvider` for a normalized depth/disparity map and falls back to pseudo-stereo when the provider is absent, throws, or returns nil. Generated output can be a selected eye for normal 2D displays, or packed side-by-side/top-and-bottom for displays or export paths that expect stereo frames. Existing side-by-side/top-and-bottom media and panorama/VR rendering take priority, so 2D-to-3D conversion is ignored for already-stereo or panorama content.

Depth controls are intentionally small: `video2DTo3DDepthStrength` controls horizontal disparity, `video2DTo3DDepthDistance` scales the generated inter-eye distance, and `video2DTo3DDepthCurvature` reshapes normalized depth around the neutral plane. Depth estimation controls live on `VideoDepthProcessingConfiguration`: `maximumInferenceFPS` throttles model execution and `temporalSmoothingFactor` smooths consecutive depth maps.

### Vision Pro Depth Anything 3 setup

Depth Anything V2 and Depth Anything 3 can be integrated by visionOS app code without KSPlayer bundling model weights. For Apple Vision Pro real-time playback, prefer Core ML with `.cpuAndNeuralEngine` or `.all` compute units so Core ML can use ANE/GPU where the converted model permits it. visionOS can run Core ML and ANE; ONNX Runtime is optional and works only when the app bundles or loads a visionOS-compatible ONNX Runtime C library/framework. Do not assume an iOS/macOS ONNX package is loadable on visionOS.

The app-owned model pipeline is:

1. Export DA3 from PyTorch to a stable TorchScript/ExportedProgram/Core ML-compatible graph, or export ONNX for an ONNX Runtime path.
2. Convert the PyTorch graph to Core ML with `coremltools`, float16 compute precision, and an image input feature named consistently with your app configuration. A helper template lives at `Tools/convert_depth_anything_to_coreml.py` with `Tools/depth_anything_coreml_config.json`.
3. Validate the compiled model before shipping: input should be an RGB/BGRA-compatible image feature (commonly `image`) at the chosen inference size, and the model should emit one depth/disparity feature (commonly `depth`, `disparity`, or `predicted_depth`) as `MLMultiArray` or image output. If your DA3 export requires ImageNet mean/std normalization, include that preprocessing in the converted model or wrapper; KSPlayer only resizes and hands frames to the Core ML image input.
4. Add the compiled `.mlmodelc` to the app bundle or a downloaded model directory. Do not commit weights or compiled model packages to KSPlayer.
5. Use a `@MainActor` provider/adapter boundary. KSPlayer calls `VideoDepthEstimationProvider.makeDepthMap` during the Metal frame handoff, so the provider must be strict-concurrency safe and should rely on adapter throttling/caching instead of running every display frame.

```swift
let modelURL = Bundle.main.url(
    forResource: "DepthAnything3Small",
    withExtension: "mlmodelc"
)!

let modelConfiguration = MLModelConfiguration()
modelConfiguration.computeUnits = .cpuAndNeuralEngine

let adapter = try DepthAnythingDepthEstimationAdapter(
    modelFamily: .depthAnything3,
    modelVariant: .small,
    modelURL: modelURL,
    modelConfiguration: modelConfiguration,
    processingConfiguration: VideoDepthProcessingConfiguration(
        inputSize: VideoDepthInputSize(width: 518, height: 518),
        aspectPolicy: .scaleAspectFit,
        normalization: .minMax,
        invertDepth: false,
        maximumInferenceFPS: 12,
        temporalSmoothingFactor: 0.6,
        preferredOutputFeatureName: "depth"
    )
)

options.video2DTo3DMode = .depthMapPreferred
options.video2DTo3DDepthStrength = 0.35
options.video2DTo3DDepthDistance = 1.0
options.video2DTo3DDepthCurvature = 1.0
options.videoDepthEstimationProvider = adapter
```

The Core ML adapter resizes input frames according to the configured aspect policy, normalizes finite depth output to `0 ... 1`, replaces invalid values with neutral disparity, optionally inverts the depth direction, throttles model execution to `maximumInferenceFPS`, and temporally smooths consecutive maps. Larger normalized values are treated as closer to the viewer. The normalized depth map is uploaded as an `r32Float` Metal texture and sampled by the stereo renderer; missing depth falls back to pseudo-stereo for that frame. `DepthAnythingV2DepthEstimationAdapter` remains available as a compatibility type alias.

Apps that own an asynchronous DA3 actor can also consume decoded `CVPixelBuffer` frames directly from the `KSMEPlayer` path without blocking decode or render:

```swift
player.videoOutputConfiguration = KSVideoFrameOutput.Configuration(
    maximumBufferedFrameCount: 1,
    dropPolicy: .keepLatest
)
player.videoOutput = { [weak self] pixelBuffer in
    Task {
        await self?.handleFrame(pixelBuffer)
    }
}
```

`videoOutput` is a `KSPlayerLayer` callback. It is delivered on a private serial queue, not the main thread, and defaults to keeping only the latest pending frame so a slow ML pipeline does not build an unbounded backlog. KSPlayer retains each pixel buffer until the callback is invoked; Swift captures of the `CVPixelBuffer` keep it alive for app-created tasks. If the callback is attached while the selected route is `KSAVPlayer`, no decoded frames are emitted because AVFoundation playback does not expose this FFmpeg/Metal frame queue.

A custom app-owned wrapper can still conform directly when it needs its own model queue or cache:

```swift
@MainActor
final class VisionProDA3DepthProvider: VideoDepthEstimationProvider {
    let providerID = "app-da3-coreml"
    private let adapter: DepthAnythingDepthEstimationAdapter

    init(compiledModelURL: URL) throws {
        let configuration = MLModelConfiguration()
        configuration.computeUnits = .cpuAndNeuralEngine
        adapter = try DepthAnythingDepthEstimationAdapter(
            modelFamily: .depthAnything3,
            modelVariant: .small,
            modelURL: compiledModelURL,
            modelConfiguration: configuration
        )
    }

    func makeDepthMap(request: VideoDepthEstimationRequest) throws -> VideoDepthMap? {
        try adapter.makeDepthMap(request: request)
    }
}
```

For ONNX, KSPlayer includes `ONNXDepthEstimationProvider` and `DepthAnythingONNXRuntimeBackend`. Microsoft’s official `onnxruntime-swift-package-manager` release `1.24.2` declares iOS 15 and macOS 14 only, not visionOS, so KSPlayer does not add it as a hard SwiftPM dependency. A visionOS app should instead bundle or otherwise load a compatible ONNX Runtime C library/framework and pass its URL:

```swift
let modelURL = Bundle.main.url(forResource: "DepthAnything3Small", withExtension: "onnx")!
let runtimeURL = Bundle.main.url(
    forResource: "onnxruntime",
    withExtension: nil,
    subdirectory: "Frameworks/onnxruntime.framework"
)

let provider = try ONNXDepthEstimationProvider(
    modelFamily: .depthAnything3,
    modelVariant: .small,
    modelURL: modelURL,
    runtimeConfiguration: DepthAnythingONNXRuntimeConfiguration(
        runtimeLibraryURL: runtimeURL,
        preferredInputName: "image",
        preferredOutputName: "depth",
        tensorConfiguration: DepthAnythingONNXTensorConfiguration(layout: .nchw)
    ),
    processingConfiguration: VideoDepthProcessingConfiguration(
        inputSize: VideoDepthInputSize(width: 518, height: 518),
        aspectPolicy: .scaleAspectFit,
        normalization: .minMax,
        maximumInferenceFPS: 12,
        temporalSmoothingFactor: 0.6
    )
)

options.video2DTo3DMode = .depthMapPreferred
options.videoDepthEstimationProvider = provider
```

For real-time IPTV on Vision Pro, start with DA3 Small/Base or a distilled app-owned model, 518x518 or lower input, float16 Core ML, `.cpuAndNeuralEngine` compute units, `maximumInferenceFPS` around `8 ... 12`, and `temporalSmoothingFactor` around `0.5 ... 0.7`. Avoid large DA3 variants for live streams unless the app has measured headroom on device. Keep HLS/IPTV decoding and Metal rendering on the existing `KSMEPlayer` path; 2D-to-3D does not change playback routing beyond forcing the visionOS Metal renderer for eligible mono plane video.

Depth Anything 3 does not change the playback routing: conversion remains Vision Pro / visionOS-only. DA3 may be useful for more consistent video or multi-view depth, and its streaming pipeline targets long videos, but the upstream repo is Python/PyTorch-first (`torch>=2`, `torchvision`, `xformers`, OpenCV/Open3D and related tooling). Public DA3 docs do not provide a drop-in Apple Vision Pro model package; apps must own export/conversion, operator compatibility, quantization, license review, and on-device performance validation. DA3 Small/Base and Metric/Mono Large weights are listed as Apache-2.0, while DA3 Large/Giant/Nested any-view weights are CC-BY-NC-4.0. Depth Anything V2 Small is Apache-2.0; V2 Base/Large/Giant are CC-BY-NC-4.0.

## Video color adjustment

Use `KSOptions.videoColorAdjustment` to apply Metal-side saturation, brightness, and contrast changes:

```swift
let options = KSOptions()
options.videoColorAdjustment = VideoColorAdjustment(saturation: 1.1, brightness: 0.05, contrast: 1.05)
```

Neutral values are saturation `1`, brightness `0`, and contrast `1`. Inputs are clamped to safe ranges, and non-finite values fall back to the neutral channel default. Color adjustment requires the `KSMEPlayer`/Metal path; native AVPlayer playback, separate audio/video URLs, and wireless-route playback cannot apply it. SDR content is adjusted by default, while HDR10, HLG, and Dolby Vision are preserved unless `hdrPolicy: .allowHDR` is explicitly requested.

## Requirements

- iOS 13+, macOS 10.15+, tvOS 13+, xrOS 1+

## List of Apps Licensed to Use this SDK

This table does not list all licensed apps. If you would like to have your app listed above, please send me an email.
| App Store Link | Logo |
| -------------- | ---- |
|[APTV](https://apps.apple.com/app/aptv/id1630403500)||
|[homeTV IPTV Player](https://apps.apple.com/app/hometv-iptv-player/id1636701357)||
|[IPTV +](https://apps.apple.com/app/iptv-my-smart-iptv-player/id1525121231)||
|[LillyPlayer Video Player](https://apps.apple.com/app/lillyplayer-video-player/id1446967273)||
|[SenPlayer](https://apps.apple.com/app/senplayer-hdr-media-player/id6443975850)||
|[Smart IPTV](https://apps.apple.com/app/smart-iptv-tv-and-movies-ott/id1492738910)||
|[Snappier IPTV](https://apps.apple.com/app/snappier-iptv/id1579702567)||
|[Spatial Video Studio](https://apps.apple.com/app/id6523429904)||
|[SWIPTV - IPTV Smart Player](https://apps.apple.com/app/swiptv-iptv-smart-player/id1658538188)||
|[TracyPlayer](https://apps.apple.com/app/tracyplayer/id6450770064)||
|[UHF - Love your IPTV](https://apps.apple.com/app/uhf-love-your-iptv/id6443751726)||
|[Zen IPTV](https://apps.apple.com/fr/app/zen-iptv/id6458223193)||


## Demo

```bash
cd Demo
pod install
```
- Open Demo/Demo.xcworkspace with Xcode.

## Quick Start

#### CocoaPods

Make sure to use the latest version **cocoapods 1.10.1+**, which can be installed using the command `brew install cocoapods`

```ruby
target 'ProjectName' do
    use_frameworks!
    pod 'KSPlayer',:git => 'https://github.com/kingslay/KSPlayer.git', :branch => 'main'
    pod 'DisplayCriteria',:git => 'https://github.com/kingslay/KSPlayer.git', :branch => 'main'
    pod 'FFmpegKit',:git => 'https://github.com/kingslay/FFmpegKit.git', :branch => 'main'
    pod 'Libass',:git => 'https://github.com/kingslay/FFmpegKit.git', :branch => 'main'
end
```

### Swift Package Manager

```swift
dependencies: [
    .package(url: "https://github.com/kingslay/KSPlayer.git", .branch("main"))
]
```

## Usage

#### Initialization

```swift
KSOptions.secondPlayerType = KSMEPlayer.self
playerView = IOSVideoPlayerView()
view.addSubview(playerView)
playerView.translatesAutoresizingMaskIntoConstraints = false
NSLayoutConstraint.activate([
    playerView.topAnchor.constraint(equalTo: view.readableContentGuide.topAnchor),
    playerView.leftAnchor.constraint(equalTo: view.leftAnchor),
    playerView.rightAnchor.constraint(equalTo: view.rightAnchor),
    playerView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
])
playerView.backBlock = { [unowned self] in
    if UIApplication.shared.statusBarOrientation.isLandscape {
        self.playerView.updateUI(isLandscape: false)
    } else {
        self.navigationController?.popViewController(animated: true)
    }
}
```

#### Setting up a regular video

```swift
playerView.set(url:URL(string: "http://baobab.wdjcdn.com/14525705791193.mp4")!)
playerView.set(resource: KSPlayerResource(url: url, name: name!, cover: URL(string: "http://img.wdjimg.com/image/video/447f973848167ee5e44b67c8d4df9839_0_0.jpeg"), subtitleURL: URL(string: "http://example.ksplay.subtitle")))
```

#### Multi-definition, with cover video

```swift
let res0 = KSPlayerResourceDefinition(url: URL(string: "http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4")!,
                                      definition: "高清")
let res1 = KSPlayerResourceDefinition(url: URL(string: "http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4")!,
                                      definition: "标清")

let asset = KSPlayerResource(name: "Big Buck Bunny",
                             definitions: [res0, res1],
                             cover: URL(string: "https://upload.wikimedia.org/wikipedia/commons/thumb/c/c5/Big_buck_bunny_poster_big.jpg/848px-Big_buck_bunny_poster_big.jpg"))
playerView.set(resource: asset)
```

#### Setting up an HTTP header

```swift
let options = KSOptions()
options.appendHeader(["Referer":"https:www.xxx.com"])
let definition = KSPlayerResourceDefinition(url: URL(string: "http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4")!,
                                            definition: "高清",
                                            options: options)
let asset = KSPlayerResource(name: "Video Name",
                             definitions: [definition])
playerView.set(resource: asset)
```

#### Audio AirPlay / Wi-Fi output

```swift
let options = KSOptions()
options.audioRouteSharingPolicy = .longFormAudio
playerView.set(url: url, options: options)
```

Use `.longFormAudio` when the app should expose audio-only AirPlay/Wi-Fi routes. Native encoded Dolby passthrough depends on Apple's AVPlayer route and the current hardware output; `KSMEPlayer`'s FFmpeg path decodes supported audio to PCM before output and does not bitstream Atmos, AC-4, or TrueHD.

#### Multichannel / Spatial Audio

```swift
let options = KSOptions()
options.audioSpatializationPreference = .automatic
options.multichannelAudioPreference = .automatic
playerView.set(url: url, options: options)
```

`.automatic` preserves multichannel PCM when the current platform and route expose Spatial Audio or a multichannel output. Use `.enabled` / `.multichannel` to advertise multichannel content more eagerly, or `.disabled` / `.stereo` when the app should avoid system spatialization or force a stereo decoded output. MP4/MOV/HLS Dolby Atmos playback should stay on `KSAVPlayer` when native route behavior or encoded passthrough matters; MKV/FFmpeg-only sources use `KSMEPlayer`, preserve channel layout metadata, and output decoded PCM.

Observe `options.audioRouteDiagnostic` at runtime to see the current public route snapshot: output port names/types, route classification (AirPlay, Bluetooth, HDMI, built-in, and so on), reported channel counts, Spatial Audio capability flags when available, configured multichannel-content support, external-playback flags, and the encoded-passthrough policy reason. This diagnostic reports what the OS exposes for the active route; actual Spatial Audio, AirPlay, Bluetooth, HDMI, Atmos, AC-4, and TrueHD behavior still requires validation on the target hardware route.

#### High-performance 8K / high-FPS playback

KSPlayer automatically applies its high-performance policy when the selected MEPlayer video track is 8K or 90+ FPS: synchronous video decode is avoided, VideoToolbox asynchronous decompression is enabled when hardware decode is still allowed, frame queues are expanded for high FPS, and 8K queues stay capped to limit memory pressure. Live streams keep smaller queues for latency.

Apps can query the same decisions with `HighPerformanceVideoPlaybackPolicy.isHighWorkload(fps:naturalSize:)`, `frameCapacity(fps:naturalSize:isLive:)`, and `displayFrameRateRange(fps:)`. Deinterlacing filters, rotation filters, simulator/runtime hardware availability, and unsupported codecs can still force software fallback.

#### Low-latency LAN live playback

```swift
let options = KSOptions()
options.applyLowLatencyLiveProfile(.lan)
playerView.set(url: URL(string: "rtsp://camera.local/live")!, options: options)
```

`KSLowLatencyLiveProfile.lan` tunes the FFmpeg/MEPlayer path for latency-sensitive LAN feeds: reduced probe/analyze windows, `fflags=nobuffer`, direct AVIO, low-delay decoder flags, bounded async audio/video packet queues, small live frame queues, disabled seek/cache/prewarm features, preferred hardware decode, asynchronous VideoToolbox decompression, stricter late-frame dropping, and a small preferred audio I/O buffer where the platform exposes it. RTSP defaults to UDP transport with short network timeouts and no RTP reorder queue unless the app has already set those options.

This profile does not guarantee sub-200ms glass-to-glass latency. Encoder GOP/B-frames, camera buffering, RTSP/RTP server behavior, Wi-Fi/Ethernet jitter, display refresh, audio route latency, and device hardware decode capacity still need validation on the target network and devices.

For verification, inspect `player.dynamicInfo?.lowLatencyLiveDiagnostic` during playback. It reports local buffered duration, queued packet/frame counts, dropped video frames/packets, rendered frame/update counts, A/V sync diff, display FPS, audio latency estimate, relative startup/read/decode/render timestamps, startup-to-first-frame timings, first read-to-decode/render timings, and rolling averages/max values for local pipeline health. `KSLowLatencyLiveProfile.lan.sourceRecommendations` exposes matching encoder, RTSP, RTP, and measurement checklist guidance. These metrics describe KSPlayer's local pipeline only and should be paired with the repeatable checklist in `Tools/LowLatencyLANValidation.md` plus camera/server/display measurements for true end-to-end latency.

#### Hardware decode acceleration

`KSOptions.hardwareDecode` defaults to enabled for the FFmpeg/Metal path, while `KSOptions.asynchronousDecompression` defaults to off unless an app, low-latency live profile, or high-performance 8K/high-FPS policy enables KSPlayer's direct asynchronous VideoToolbox path. VideoToolbox is attempted only for codec families with a runtime hardware decoder, including H.264, HEVC/Dolby Vision HEVC, AV1, VP9, MPEG-family codecs, and Apple ProRes variants; unsupported codecs, simulators, missing format descriptions, decoder creation failures, deinterlace filters, and rotation filters fall back to FFmpeg software decode.

#### De-interlace auto detect

`KSOptions.deinterlaceMode` defaults to `.automatic` for the FFmpeg/MEPlayer path. Automatic mode enables the software `yadif` filter only when stream or CoreMedia format metadata clearly reports top-field-first or bottom-field-first interlacing, skips progressive or unknown sources, and avoids high-workload 8K/high-FPS sources unless `.force` is selected. Adding a deinterlace filter disables VideoToolbox hardware/asynchronous decode for that item; native AVPlayer playback keeps Apple's system-managed output.

#### Video upscaling

```swift
let options = KSOptions()
options.videoUpscaling = .appleSuperResolution(scaleFactor: 2)
playerView.set(url: url, options: options)
```

Video upscaling uses the MEPlayer/Metal path because AVPlayer does not expose per-frame super-resolution output. The requested scale factor is clamped to the supported range and then matched to the closest VideoToolbox-supported scale for the source. HDR and Dolby Vision sources are preserved by default; use `.appleSuperResolution(scaleFactor: 2, hdrPolicy: .allowHDR)` only after validating output on your target devices. Upscaling is skipped for high-workload sources such as 8K or 90+ FPS video.

Check `VideoUpscalingMode.isAppleSuperResolutionRuntimeAvailable` before presenting the option as available, and observe `options.videoUpscalingState` to learn whether the renderer is active or why the request is unavailable. The state resets to `.inactive` on source replacement, flush, and renderer teardown; wireless-route playback falls back to the native AVPlayer path and reports upscaling as unavailable.

#### Progress preview thumbnails

```swift
let options = KSOptions()
options.isProgressPreviewEnabled = true
options.progressPreviewThumbnailMode = .localOnly
playerView.set(url: url, options: options)
```

Progress previews show a scrubber time bubble and can warm thumbnail images for finite, seekable VOD. The default `.localOnly` mode avoids network work; use `.always` only when remote thumbnail warming is acceptable. Live and DVR streams keep the time preview but skip thumbnail warming because generation performs background seeks.

#### Main and secondary subtitles

KSPlayer can render two selected subtitle tracks at the same playback time. Main subtitles default to the bottom safe area and secondary subtitles default to the top safe area, while authored ASS/image positioning is preserved. Text, external, online, cached, embedded FFmpeg, and offline-generated subtitles share the same selection model; native AVFoundation legible tracks remain system-rendered and are limited by AVPlayer's single active legible media selection.

#### ASS/SSA image rendering

```swift
let options = KSOptions()
options.isAssSubtitleImageRenderingEnabled = true
playerView.set(url: url, options: options)
```

When the `libass` module is linked for the current platform, embedded ASS/SSA subtitle packets are rendered first as authored bitmap overlays so positioning, vector drawing, karaoke styling, and embedded Matroska fonts can be preserved more faithfully than the text fallback. If libass is unavailable, disabled, or cannot render a packet, KSPlayer falls back to the existing parsed text subtitle path. These bitmap overlays keep authored pixels and are not restyled by system caption appearance, HDR subtitle effects, or external subtitle translation.

#### Picture in Picture subtitles

Use `KSOptions.pictureInPictureSubtitlePolicy` to control subtitle exposure for Picture in Picture. The default `.automatic` keeps AVFoundation legible subtitle tracks available so Apple's native AVPlayer PiP window can render them. Set `.disabled` to hide those native PiP subtitle choices.

Apple Picture in Picture does not display arbitrary UIKit/AppKit overlay views. KSPlayer-owned overlays, including external text subtitles, image/SUP subtitles, secondary subtitles, and KSMEPlayer/libass-rendered ASS bitmap subtitles, remain inline-only and are hidden from the inline view while system PiP is active. The KSMEPlayer sample-buffer PiP path currently sends video frames only; subtitles are not burned into those frames.

#### Text subtitle translation

```swift
let options = KSOptions()
options.isExternalSubtitleTranslationEnabled = true
options.externalSubtitleTranslationProvider = appSubtitleTranslator
options.externalSubtitleTranslationDisplayMode = .bilingual(separator: "\n")
options.externalSubtitleTranslationSourceLanguage = "en"
options.externalSubtitleTranslationTargetLanguage = "zh-Hans"
playerView.set(url: url, options: options)
```

External subtitle translation is opt-in and only runs for parsed text subtitles (`.srt`, `.vtt`, `.ass`, `.ssa`). Image subtitle files such as SUP/PGS are skipped. KSPlayer batches parsed cue text through the app-supplied `SubtitleTranslationProvider`, keeps timing and positioning metadata, and falls back to the original text if translation fails.

#### External image subtitles

External bitmap subtitles (`.sup`/`.pgs`) are exposed as image subtitle tracks and loaded through the FFmpeg subtitle decoder when selected. Bitmap cues render as authored images in the primary or secondary subtitle overlay; text-only features such as translation, system caption appearance, word highlighting, and HDR subtitle styling are skipped for these cues so their pixels are not restyled. When a standalone image subtitle file does not carry an explicit video canvas, KSPlayer falls back to a 1920x1080 canvas while preserving decoded bitmap offsets.

#### System caption appearance

```swift
let options = KSOptions()
options.subtitleCaptionAppearancePolicy = .contentIfAvailable
playerView.set(url: url, options: options)
```

Use `.contentIfAvailable` to apply Apple system caption appearance to unstyled text subtitle ranges while preserving embedded subtitle styling. Use `.alwaysOverride` only when your app should replace embedded text subtitle font, color, background, and edge styling with the user's system caption settings. Image subtitles and libass-rendered ASS images keep their authored bitmap appearance.

#### HDR subtitle effects

```swift
let options = KSOptions()
options.subtitleHDREffectPolicy = .automatic
playerView.set(url: url, options: options)
```

Use `.automatic` to add contrast for text subtitles over HDR10, HLG, and Dolby Vision video when KSPlayer owns subtitle styling. Use `.enhanced` for a stronger window/background and outline treatment, or `.disabled` to keep SDR-era subtitle rendering. These effects are UI-overlay styling only; image subtitles and libass-rendered ASS bitmaps keep their authored pixels, and system caption appearance takes precedence when enabled.

#### In-app compact playback

```swift
let layout = KSPlayerCompactLayout(corner: .bottomTrailing,
                                   size: CGSize(width: 320, height: 180),
                                   margin: 16)
playerView.enterInAppCompactMode(in: view, layout: layout)

let resumeState = playerView.resumeState
playerView.exitInAppCompactMode()

if let resumeState {
    playerView.restorePlayback(from: resumeState, options: KSOptions())
}
```

Compact playback keeps the existing `KSPlayerLayer` attached, so playback continues without replacing the player. The compact view is constrained to the container safe area and updates its size when the container changes, which keeps it usable during rotation, split view, tvOS focus-safe layouts, and visionOS window resizing. Use `resumeState?.resumableTime` to persist a safe restore point; `restorePlayback(from:options:)` reuses the current player when it is already on the same URL. Avoid entering compact mode while system Picture in Picture is active.

#### Listening status change

```swift
// Listen to play time change
playerView.playTimeDidChange = { (currentTime: TimeInterval, totalTime: TimeInterval) in
    print("playTimeDidChange currentTime: \(currentTime) totalTime: \(totalTime)")
}

// Delegates
public protocol PlayerControllerDelegate: class {
    func playerController(state: KSPlayerState)
    func playerController(currentTime: TimeInterval, totalTime: TimeInterval)
    func playerController(finish error: Error?)
    func playerController(maskShow: Bool)
    func playerController(action: PlayerButtonType)
    // `bufferedCount: 0` indicates first time loading
    func playerController(bufferedCount: Int, consumeTime: TimeInterval)
}
```

## Advanced Usage

- ### Inherits PlayerView's custom play logic and UI.

  ```swift
  class CustomVideoPlayerView: IOSVideoPlayerView {
      override func updateUI(isLandscape: Bool) {
          super.updateUI(isLandscape: isLandscape)
          toolBar.playbackRateButton.isHidden = true
      }

      override func onButtonPressed(type: PlayerButtonType, button: UIButton) {
          if type == .landscape {
              // Your own button press behaviour here
          } else {
              super.onButtonPressed(type: type, button: button)
          }
      }
  }
  ```



- ### Selecting Tracks

  ```swift
     override open func player(layer: KSPlayerLayer, state: KSPlayerState) {
          super.player(layer: layer, state: state)
          if state == .readyToPlay, let player = layer.player {
              let tracks = player.tracks(mediaType: .audio)
              let track = tracks[1]
              /// the name of the track
              let name = track.name
              /// the language of the track
              let language = track.language
              /// selecting the one
              player.select(track: track)
          }
     }
  ```

- ### Set the properties in KSOptions

  ```swift
  open class KSOptions {
    /// 最低缓存视频时间
    @Published
    public var preferredForwardBufferDuration = KSOptions.preferredForwardBufferDuration
    /// 最大缓存视频时间
    public var maxBufferDuration = KSOptions.maxBufferDuration
    /// 是否开启秒开
    public var isSecondOpen = KSOptions.isSecondOpen
    /// 开启精确seek
    public var isAccurateSeek = KSOptions.isAccurateSeek
    /// Applies to short videos only
    public var isLoopPlay = KSOptions.isLoopPlay
    /// 是否自动播放，默认false
    public var isAutoPlay = KSOptions.isAutoPlay
    /// seek完是否自动播放
    public var isSeekedAutoPlay = KSOptions.isSeekedAutoPlay
    /*
     AVSEEK_FLAG_BACKWARD: 1
     AVSEEK_FLAG_BYTE: 2
     AVSEEK_FLAG_ANY: 4
     AVSEEK_FLAG_FRAME: 8
     */
    public var seekFlags = Int32(0)
    // ffmpeg only cache http
    public var cache = false
    /// KSMEPlayer-only live demux recording destination. Existing files are not overwritten.
    public var outputURL: URL?
    public var display = DisplayEnum.plane
    public var avOptions = [String: Any]()
    public var formatContextOptions = [String: Any]()
    public var decoderOptions = [String: Any]()
    public var probesize: Int64?
    public var maxAnalyzeDuration: Int64?
    public var lowres = UInt8(0)
    public var startPlayTime: TimeInterval = 0
    public var startPlayRate: Float = 1.0
    public var registerRemoteControll: Bool = true // 默认支持来自系统控制中心的控制
    public var referer: String?
    public var userAgent: String?
      // audio
    public var audioFilters = [String]()
    public var syncDecodeAudio = false
    // sutile
    public var autoSelectEmbedSubtitle = true
    public var subtitleDisable = false
    public var isSeekImageSubtitle = false
    // video
    public var videoDelay = 0.0 // s
    public var autoDeInterlace = false
    public var autoRotate = true
    public var destinationDynamicRange: DynamicRange?
    public var videoAdaptable = true
    public var videoFilters = [String]()
    public var syncDecodeVideo = false
    public var hardwareDecode = KSOptions.hardwareDecode
    public var asynchronousDecompression = KSOptions.asynchronousDecompression
    public var videoDisable = false
    public var canStartPictureInPictureAutomaticallyFromInline = true
    public var pictureInPictureSubtitlePolicy = PictureInPictureSubtitlePolicy.automatic
  }

  ```

Picture in Picture uses AVPlayer's native PiP path when possible and KSMEPlayer's sample-buffer PiP path on supported Apple platforms. KSPlayer overlay subtitles, external text/image subtitles, secondary subtitles, and libass bitmap rendering remain inline-only during system PiP unless a future renderer explicitly burns them into the video frames.

## Effect

![gif](./Demo/demo.gif)

## Developments and Tests

Any contributing and pull requests are warmly welcome. However, before you plan to implement some features or try to fix an uncertain issue, it is recommended to open a discussion first. It would be appreciated if your pull requests could build and with all tests green. :)


## Backers & Sponsors

Open-source projects cannot live long without your help. If you find KSPlayer to be useful, please consider supporting this
project by becoming a sponsor.

Become a sponsor through [GitHub Sponsors](https://github.com/sponsors/kingslay/). :heart:

Your user icon or company logo shows up this with a link to your home page.
|Name| App name | App Logo |
| ----------- | ----------- |----------- |
|[UnknownCoder807](https://github.com/UnknownCoder807)|[Snappier](https://apps.apple.com/app/snappier-iptv/id1579702567)||
|[skrew](https://github.com/skrew)||
|[Kimentanm](https://github.com/Kimentanm)||
|[nakiostudio](https://github.com/nakiostudio)|[UHF](https://apps.apple.com/app/uhf-love-your-iptv/id6443751726)||
|[CodingByJerez](https://github.com/CodingByJerez)||
|[andrefmsilva](https://github.com/andrefmsilva)||
|[romaingyh](https://github.com/romaingyh)|[Zen IPTV](https://apps.apple.com/fr/app/zen-iptv/id6458223193)||
|[FantasyKingdom](https://github.com/FantasyKingdom)|[Senplayer](https://apps.apple.com/us/app/senplayer-hdr-media-player/id6443975850)||
|[aart-rainey](https://github.com/aart-rainey)||
|[nihalahmed](https://github.com/nihalahmed)||
|[johnil](https://github.com/johnil)||
|[MeloDreek](https://github.com/MeloDreek)||
|[nsplay1990](https://github.com/nsplay1990)||
|[AppleChillVibez](https://github.com/AppleChillVibez)||
|[stekc](https://github.com/stekc)||
|[AstroChivs](https://github.com/AstroChivs)||
|[bmob222](https://github.com/bmob222)||
|[pateltejas](https://github.com/pateltejas)||
|[ewanl2001](https://github.com/ewanl2001)||
|[themisterholliday](https://github.com/themisterholliday)||
|[JulienDev](https://github.com/JulienDev)||
|[Sheinices](https://github.com/Sheinices)||
|[Etheirystech](https://github.com/Etheirystech)||
|[loicleser](https://github.com/loicleser)||


Thanks to [nightfall708](https://github.com/nightfall708) for sponsoring a mac mini

Thanks to [cdguy](https://github.com/cdguy) [UnknownCoder807](https://github.com/UnknownCoder807) [skrew](https://github.com/skrew) and LillyPlayer community for sponsoring a LG S95QR Sound Bar

Thanks to [skrew](https://github.com/skrew) and LillyPlayer community for sponsoring a 2022 Apple TV 4K

Thanks to [bgoncal](https://github.com/bgoncal) for sponsoring a HomePod mini

![1](./Documents/Sponsors.jpg)

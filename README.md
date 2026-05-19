# KSPlayerStack

Fresh workspace for **FFmpeg 8.1.1** + **KSPlayer** (SPM only). Not tied to Visionator.

## Layout

```
KSPlayerStack/
├── FFmpegKit/          # kingslay FFmpegKit + n8.1.1 build config (rebuild required)
├── KSPlayer/           # player library → depends on ../FFmpegKit via SPM path
├── Scripts/            # native FFmpeg 8.1.1 build helpers
└── README.md
```

## Requirements

- macOS Sequoia 15.6+
- Xcode 26.2 (Swift 6.2.3)
- Homebrew: `pkg-config`, `nasm`, `cmake`, `meson`, `sdl2`

## 1. Build FFmpeg 8.1.1 (visionOS first)

Shipped `Sources/*.xcframework` in the download are still **FFmpeg 6.1.1** until you rebuild:

```bash
./Scripts/build-ffmpeg-8.1-visionos.sh
```

This can take **many hours**. Logs: `FFmpegKit/build-ffmpeg-8.1.1.log`.

Verify after `enable-FFmpeg`:

```bash
grep FFMPEG_VERSION FFmpegKit/Sources/Libavutil.xcframework/xros-arm64/Libavutil.framework/Headers/ffversion.h
# expect n8.1.1
```

## 2. Wire your app (SPM)

In Xcode or your app `Package.swift`:

```swift
.package(path: "/Users/sutherland/KSPlayerStack/KSPlayer")
```

Product: **KSPlayer**. Deployment target: **26.0**. Do not use CocoaPods for this stack.

Video upscaling is opt-in via `KSOptions.videoUpscaling = .appleSuperResolution(scaleFactor: 2)`. It routes playback through `KSMEPlayer` because AVPlayer does not expose per-frame super-resolution output; HDR/Dolby Vision and high-workload 8K/90+ FPS sources are skipped by default.

2D-to-3D conversion is Vision Pro / visionOS-only and opt-in via `KSOptions.video2DTo3DMode`. On other platforms it reports an unavailable diagnostic and keeps normal routing; flat side-by-side/top-and-bottom 3D and panorama stereo remain separate features. `.pseudoStereo` uses a conservative Metal disparity approximation, while `.depthMapPreferred` lets visionOS apps provide normalized depth maps through `VideoDepthEstimationProvider` and falls back to pseudo-stereo when unavailable. Depth Anything V2 and Depth Anything 3 are integrated as app-owned Core ML/ONNX adapters; no weights are vendored. The official Microsoft ONNX Runtime SwiftPM package currently declares iOS/macOS, not visionOS, so visionOS apps should bundle a compatible ONNX Runtime C library or use Core ML.

Progress preview thumbnails are enabled by default for local, finite, seekable VOD through `KSOptions.progressPreviewThumbnailMode = .localOnly`; use `.always` only when remote thumbnail warming is acceptable.

Low-latency LAN live playback is opt-in with `KSOptions.applyLowLatencyLiveProfile(.lan)`. It reduces FFmpeg probing/buffering, prefers hardware asynchronous VideoToolbox decode, bounds MEPlayer queues, and exposes `DynamicInfo.lowLatencyLiveDiagnostic` so apps can validate local pipeline latency; sub-200ms glass-to-glass still requires camera, network, route, and display measurements on target hardware.

Dolby Vision Profile 7 FEL is not a full-composition path in public/general builds. The FFmpeg/libdovi build exposes DOVI configuration/RPU metadata, and KSPlayer can split interleaved P7 HEVC samples into BL, EL, and RPU payloads for diagnostics/alignment. It reports BL HDR10 fallback, MEL-compatible fallback, or FEL composition-unavailable diagnostics through `DolbyVisionPlaybackDiagnostic`; public FFmpeg/libdovi APIs still do not provide the BL+EL residual reconstruction compositor needed to claim full FEL display. Private/education builds can provide `KSOptions.dolbyVisionFELCompositorBackend` with the `KSPlayer/Tools/FelBakerShim` real-direct or real-custom FelBaker-derived shim, which uses no proprietary Dolby or Apple private APIs and must only advertise full FEL composition after writing composed BL+EL+RPU output.

## 3. Push to your forks (optional)

```bash
cd FFmpegKit && git remote add origin git@github.com:iansutherland74/FFmpegKit.git
cd ../KSPlayer && git remote add origin git@github.com:iansutherland74/KSPlayer.git
```

Use **Git LFS** for `Sources/*.xcframework` before pushing.

## License

FFmpegKit/KSPlayer default builds are **GPL-heavy**. Review kingslay licenses before App Store shipping.

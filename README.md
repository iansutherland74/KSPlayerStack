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

Progress preview thumbnails are enabled by default for local, finite, seekable VOD through `KSOptions.progressPreviewThumbnailMode = .localOnly`; use `.always` only when remote thumbnail warming is acceptable.

## 3. Push to your forks (optional)

```bash
cd FFmpegKit && git remote add origin git@github.com:iansutherland74/FFmpegKit.git
cd ../KSPlayer && git remote add origin git@github.com:iansutherland74/KSPlayer.git
```

Use **Git LFS** for `Sources/*.xcframework` before pushing.

## License

FFmpegKit/KSPlayer default builds are **GPL-heavy**. Review kingslay licenses before App Store shipping.

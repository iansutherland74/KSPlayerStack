# Viture SpaceWalker (`com.viture.spacewalker` 1.8.4) — IDA Analysis

**Binary:** `/Users/sutherland/Downloads/space mac/Contents/MacOS/SpaceWalker`  
**Role:** Mac companion for Viture XR glasses (screen mirror, IMU, calibration) — **not** a 2D→3D video player.

**Exports:**
- `spacewalker-ida-export.c` — ObjC stubs + ivar layout (~44 blocks)
- `spacewalker-ida-export-deep.c` — Swift implementation subs (~120 functions, 10k lines)

**Re-run:**
```bash
./KSPlayer/Tools/ida_export_spacewalker.sh
export SPACEWALKER_IDA_EXPORT_DEEP=KSPlayer/Docs/reverse/spacewalker-ida-export-deep.c
/Applications/IDA\ Professional\ 9.3.app/Contents/MacOS/idat -A \
  -S"KSPlayer/Tools/ida_export_spacewalker_deep.py" \
  "/Users/sutherland/Downloads/space mac/Contents/MacOS/SpaceWalker"
```

---

## Verdict for KSPlayer / Vision Pro DA3 demo

| Useful? | What |
|--------|------|
| **Yes (pattern)** | Display-linked Metal present path — same *class* of fix as P10 + your `MetalPlayView` drain |
| **Yes (IMU)** | Head smoothing / SceneKit camera update loop |
| **No** | Depth Anything, stereo shift, AVPlayer file playback, CompositorLayer immersive |
| **No** | Copying shaders — vertex/fragment names are Swift `String` literals in binary, not in export |

For **2D→3D video**, keep using **P10 `ExternalMonitor`** (`VITURE_2D3D_ANALYSIS.md`). SpaceWalker is the **Mac→glasses mirror** stack.

---

## Pipeline (from deep export)

```
ScreenCaptureKit SCStream
    │  CMSampleBuffer (SCStreamFrameInfoStatus == complete)
    │  CVPixelBuffer → IOSurface, content rect / scale
    ▼
CaptureEngine / ScreenRecorder.frameUpdateBlock
    │  sub_1000816A8 — sample → CapturedFrame struct
    ▼
WideStripFrameHandler (up to 3 CVPixelBuffers)
    │  os_unfair_lock → latestPixelBuffers swap
    ▼
WideStripMetalRenderer
    │  CVDisplayLink (sub_1000AE704 → sub_1000AE0F0 draw)
    │  For each buffer: CVMetalTextureCacheCreateTextureFromImage (BGRA8)
    │  Horizontal strip layout with spacing (default ~10.0f)
    ▼
CAMetalLayer → XR glasses wide display
```

Parallel path for preview: **SCNCaptureVideoPreview** + **VTCameraController** `renderer:updateAtTime:` driven by **IMU** notifications (`imuDataUpdated:`).

---

## Key decompiled functions

### `sub_1000ADB58` — `WideStripMetalRenderer` init (`WideStripMTKRenderer.swift`)

- `CVMetalTextureCacheCreate`
- `CAMetalLayer`: pixel format **80** (`MTLPixelFormatBGRA8Unorm`), `framebufferOnly = false`
- Default Metal library → vertex + fragment functions (names from Swift strings)
- **`CVDisplayLinkCreateWithActiveCGDisplays`**
- **`CVDisplayLinkSetOutputCallback(..., sub_1000AE704, self)`**
- **`CVDisplayLinkStart`**
- `spacing` ivar initialized to **10.0** (`0x41200000`)

### `sub_1000AE0F0` — display-link draw

- `metalLayer.nextDrawable`
- Under lock: read **`latestPixelBuffers`** array
- For each `CVPixelBuffer`:
  - `CVMetalTextureCacheCreateTextureFromImage` → fragment texture
  - Compute strip width from aspect ratio + drawable height
  - `drawPrimitives:triangle, 6 verts` per buffer (quads as two triangles)
  - Advance horizontal cursor by `width + spacing`
- LoadAction **Clear** (2), StoreAction **Store** (1)

### `sub_1000816A8` — SCStream sample handler

- Requires `SCStreamFrameInfoStatus == 0` (complete frame)
- Reads `CMSampleBuffer` image buffer
- Optional `SCStreamFrameInfoContentRect`, `ContentScale`, `ScaleFactor`
- Builds `CapturedFrame`-like struct with IOSurface + geometry

### `WideStripMetalRenderer` ivars (from `.cxx_destruct`)

`device`, `commandQueue`, `pipeline`, `textureCache`, `metalLayer`, `containerView`, **`latestPixelBuffers`**, **`displayLink`**, `lock`, `spacing`, `drawableSize`

---

## Mapping to KSPlayer demo

| SpaceWalker | Your demo |
|-------------|-----------|
| `CVDisplayLink` → draw only on vsync | `CADisplayLink` on `MetalPlayView` |
| Producer: SCStream pushes buffers under lock | Producer: `immersivePresentVideoFrame` on audio clock |
| Consumer: display link reads `latestPixelBuffers` | Consumer: `ImmersiveVideoFeed` + compositor |
| No decode in render path | **Keep decode off render path** (pre-wire FFmpeg) |
| Multi-buffer horizontal strip | Per-eye compositor (different output) |

**Takeaway:** SpaceWalker confirms Viture’s general rule — **capture/display on a display link, swap latest frames under a lock** — but it does **not** implement depth-based stereo. That lives in P10.

---

## Bundled configs (not in IDA)

- `viture_override_config_N6P.yaml` — per-eye frustum + VAO warp mesh (1920×1080)
- `slam_config_N6_N6P.yaml` — IMU fusion (`atw_correction: 7.2`, 240 Hz IMU)

Useful for **glasses optical model**, not for DA3 tuning.

---

## Related docs

- `VITURE_2D3D_ANALYSIS.md` — P10 video + depth pipeline (primary reference)
- `viture-ida-export-deep.c` — stereo shift, DepthRatioCalculator, AVPlayerVideoOutput

# Viture P10 (`ExternalMonitor` 1.9.25) — 2D→3D Reverse Engineering

Automated IDA Pro 9.3 headless export + manual review.

**Binaries analyzed:**  
`/Users/sutherland/Downloads/com.viture.p10app-1.9.25-Decrypted/ExternalMonitor`

**Exports:**
- `viture-ida-export.c` — class methods + string xrefs (~13k lines)
- `viture-ida-export-deep.c` — implementation subs + address ranges (~13.5k lines)

**Re-run:**
```bash
./KSPlayer/Tools/ida_export_viture_2d3d.sh
# deep only:
/Applications/IDA\ Professional\ 9.3.app/Contents/MacOS/idat -A -S"$(pwd)/KSPlayer/Tools/ida_export_viture_deep.py" \
  ~/Downloads/com.viture.p10app-1.9.25-Decrypted/ExternalMonitor
```

---

## End-to-end pipeline

```
AVPlayer + AVPlayerItemVideoOutput
    │  CADisplayLink (displayLinkDidFire)
    │  hasNewPixelBufferForItemTime → copyPixelBufferForItemTime
    ▼
curPixelBuffer (locked buffer on inference controller)
    │
    ├─► Core ML Depth Anything V2 (float16)
    │     Gaming:  DepthAnythingV2float16_364_280  (364×280 in/out)
    │     Normal:  DepthAnythingV2float16         (518×294 in/out)
    │
    ├─► DepthRatioCalculator (Metal compute)
    │     computeWeightedColorDiff + computeWeightedRatio
    │     temporal stabilization vs previous depth frame
    │
    ├─► Core Image filter graph
    │     IntegratedFilter, DepthImageShiftFilter
    │     CIStereoShiftMetalFilter (CIKernel from bundle)
    │
    ▼
SBSFastView (MTKView + CIContext)
    leftCIImage / rightCIImage → SBS to XR glasses
```

**Output target:** Side-by-side stereo for Viture glasses (external display), **not** visionOS `CompositorLayer`.

---

## Video clock (critical — matches what KSPlayer demo needs)

### `VTDepthVideoPlayer` — `sub_1000423A0` (display link body)

Pseudocode summary from IDA:

1. `AVPlayer.currentItem` must exist.
2. Read `currentTime` from player.
3. `AVPlayerItemVideoOutput.hasNewPixelBufferForItemTime:` — **only proceed if a new frame for that time**.
4. `copyPixelBufferForItemTime:itemTimeForDisplay:` → store in `curPixelBuffer`.

**Implication:** Video is **presentation-time gated**, not decode-queue flooded. This is the same class of fix as `MetalPlayView` + `getVideoOutputRender(force: false)` + `immersivePresentVideoFrame` in KSPlayer.

### `VTVideoDepthInferenceController`

- `NSLock` + `__internalBuffer` / `curPixelBuffer` / `lastRenderedBuffer`
- `setCurPixelBuffer:` (`sub_1001CB524`) swaps buffer under lock, increments `curBufferIndex`
- `FPSController` (Swift actor, 144-byte alloc) — rate limits depth work
- `sequenceKey`, `fpsTimer`, `stereoDisabled`, `isPaused`
- Embeds `SBSFastView` for final composite

---

## Depth models (Core ML)

| Model | Input | Depth output | When |
|-------|--------|--------------|------|
| `DepthAnythingV2float16_364_280` | RGB 364×280 | Grayscale16Half 364×280 | `VTPreferences.isGaming == true` |
| `DepthAnythingV2float16` | RGB 518×294 | Grayscale16Half 518×294 | default |

`sub_10015C79C` — `predictionFromFeatures:options:error:` on bundled `MLModel`, reads `depth` feature as `imageBufferValue`.

**Not DA3** — smaller V2 models, two fixed resolutions, ~48 MB each.

---

## Temporal depth (`DepthRatioCalculator`)

`sub_1001A4828` init:

- `MTLCreateSystemDefaultDevice` → command queue
- Loads Metal source from string, builds pipeline for **`computeWeightedRatio`** (`sub_1001A5108`)
- `CVMetalTextureCacheCreate` for depth textures

`sub_1001A531C` — large dispatch: compares current vs previous depth (when `a2` flag allows), runs compute kernels, callbacks with stabilized depth.

**Takeaway for KSPlayer:** Add a **depth-only temporal pass** (ratio/smooth) before stereo, not only shader-side smoothing.

---

## Stereo (`CIStereoShiftMetalFilter`)

Loaded from app bundle: `CIKernel` via `URLForResource:withExtension:` (`sub_1002177D4`).

**`outputImage`** (`sub_100217498`) passes to kernel:

| Parameter | Default if nil |
|-----------|----------------|
| `inputImage` | (required) |
| `inputDepthImage` | (required) |
| `inputIncrement` | 0 |
| `inputMaxShift` | 0 |
| `incrementMultiple` | 1 |
| `shiftRatio` | 0 |
| `sigmoidCoef` | **5** |

Uses `CIKernel applyWithExtent:roiCallback:arguments:` with color + depth samplers.

**`IntegratedFilter`** (`sub_1001CD6A0`) — chains sampler + `ratio` NSNumber into another CIKernel (`sub_1002171FC`).

---

## Display (`SBSFastView`)

`sub_1000B290C` factory:

- `MTKView` + `CIContext` with Metal device
- `CADisplayLink` → selector **`tick`**
- `leftCIImage`, `rightCIImage`, `lastRenderTime`, `lastPresentedTime`
- `shouldUseDisplayLink`, `isRendering`, serial `queue`
- `Task` for async work

Renders SBS into MTKView for glasses.

---

## Comparison → KSPlayer Vision Pro demo

| Viture | KSPlayer DA3 demo (target) |
|--------|----------------------------|
| DA2 @ 364×280 / 518×294 | DA3 (tune input to ~384×216 class) |
| `AVPlayerItemVideoOutput` + display link | `MetalPlayView` display link + `immersivePresentVideoFrame` |
| `FPSController` actor | `FPSController` / inference FPS cap |
| `DepthRatioCalculator` Metal | Missing — add `ImmersiveDepthTemporal.swift` or Metal pass |
| CIKernel stereo + params | `ImmersiveStereoShaders.metal` + `Video2DTo3DRenderConfiguration` |
| SBS external display | `CompositorLayer` per-eye |
| `NSLock` on pixel buffer | `ImmersiveVideoFeed` + `bufferLock` pattern |

### Priority fixes validated by this RE

1. **Never feed immersive from `KSVideoFrameOutput` decode flood** — use presentation-time path only (already wired: `suppressWindow` + `immersivePresentVideoFrame` + drain `force: false`).
2. **Cap depth inference** to display rate (`FPSController` pattern).
3. **Temporal depth** between frames (`DepthRatioCalculator`) before compositor.
4. **Expose stereo knobs** analogous to `inputMaxShift`, `shiftRatio`, `sigmoidCoef` in compositor uniforms.
5. **Fast model path** for thermal/gaming (small fixed resolution).

---

## Key symbol index (for IDA)

| EA | Symbol | Role |
|----|--------|------|
| `0x1000423A0` | `sub_1000423A0` | Display-link pixel pull |
| `0x1000424A0` | `displayLinkDidFire` | Wrapper |
| `0x10015C79C` | `sub_10015C79C` | Core ML predict |
| `0x1001A4828` | `sub_1001A4828` | DepthRatioCalculator init |
| `0x1001A5108` | `sub_1001A5108` | Load computeWeightedRatio |
| `0x1001A531C` | `sub_1001A531C` | Depth ratio dispatch |
| `0x1001CB524` | `sub_1001CB524` | setCurPixelBuffer |
| `0x1001CD6A0` | `sub_1001CD6A0` | IntegratedFilter output |
| `0x100217498` | `sub_100217498` | CIStereoShift output |
| `0x1002177D4` | `sub_1002177D4` | CIStereoShift init / kernel load |
| `0x1000B290C` | `sub_1000B290C` | SBSFastView factory |
| `0x1000B2CF8` | `sub_1000B2CF8` | MTKView + CIContext setup |

---

## Limits

- Swift internals still appear as `sub_*` for large bodies; full `VTVideoDepthInferenceController.swift` logic needs more xref depth or runtime tracing.
- CineUltra remains encrypted; Viture IPA is the practical reference for **clock + DA2 + CI stereo**.

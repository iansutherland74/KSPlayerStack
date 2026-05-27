# Vision Pro Depth Anything V3 + KSPlayer Scaffold

This folder is an app-target scaffold for wiring a compiled Core ML Depth Anything V3 model to KSPlayer's decoded-frame callback on visionOS. It is intentionally outside the `KSPlayer` Swift package target so `swift build --package-path KSPlayer --target KSPlayer` does not require app-bundled model resources.

## Files

- `Sources/Depth/DepthAnythingV3Engine.swift` dynamically loads the bundled `da3-small.mlmodelc` inside an actor, defaults to Core ML `computeUnits = .cpuAndNeuralEngine` (ANE-preferred), accepts decoded `CVPixelBuffer`s, handles either image or RGB `MLMultiArray` inputs, times preprocessing/inference/output extraction, and converts the depth output (`MLMultiArray` shaped `[1, 1, H, W]`) into `DA3DepthFrame`.
- `Sources/Player/KSPlayer3DIntegration.swift` installs `KSPlayerLayer.videoOutput`, drops overlapping inference work, sends decoded `CVPixelBuffer`s to the actor, reuses `.r32Float` Metal depth textures, and forwards depth frames to a renderer on the main actor.
- `Sources/Rendering/StereoRenderer.swift` is a minimal `StereoRendererProtocol` implementation that records the latest source/depth dimensions and keeps the latest depth texture for a real stereoscopic renderer to consume.
- `Sources/App/ThreeDPlayerRootView.swift` is a SwiftUI entry point using `KSVideoPlayer` and the integration view model.

## App Target Setup

1. Add `da3-small.mlmodelc` to the visionOS app target resources.
2. Add these demo source files to the same visionOS app target.
3. Link/import the local `KSPlayer` package. The reusable `DA3DepthFrame` and `DA3DepthMetalBridge` helpers live in the package target and do not reference app model resources.

## Minimal Usage

```swift
import SwiftUI

struct PlayerScene: View {
    let url: URL

    var body: some View {
        Group {
            if let player = try? ThreeDPlayerRootView(url: url) {
                player
            } else {
                Text("Depth Anything V3 is unavailable")
            }
        }
    }
}
```

`KSPlayerLayer.videoOutput` is emitted by the `KSMEPlayer` decoded-frame path on a private serial queue. Use media/options that route to `KSMEPlayer` when the Depth Anything pipeline must receive frames; AVFoundation-only playback paths do not produce this callback.

The sample app is a 2D SwiftUI window. It can show generated left/right eyes as side-by-side or top/bottom packed debug output, but it does not submit separate headset-native stereo surfaces through an `ImmersiveSpace` or CompositorLayer. `Selected Eye` is only a flat single-eye preview.

The sample normalizes DA3 depth per frame before handing it to KSPlayer's shader and exposes raw/normalized range readouts plus invert-depth, contrast, and Metal temporal smoothing controls. If your renderer consumes the raw `.r32Float` texture from `StereoRenderer`, call `DA3DepthFrame.normalizedDisparityMap(normalization:invertDepth:)` or normalize in the shader intentionally instead of assuming the model output range.

## Performance Diagnostics

The bundled `da3-small.mlmodelc` currently reports a fixed non-flexible `MultiArray (Float16 1 x 1 x 3 x 336 x 336)` input and fixed `336 x 336` depth outputs in `metadata.json`. Requested input-size overrides are reported in the UI, but this compiled model cannot be safely switched to 384x216, 512-wide, or 512x288 input without recompiling a matching fixed-shape or flexible-shape model.

### ANE fallback trap (~5 fps)

If measured Core ML inference is **>150 ms** per frame (~5 fps), the compiled graph is almost certainly **not running fully on the Apple Neural Engine**. Typical causes:

- Custom DA3 layers (cross-view attention, dual DPT ray head) that Core ML splits across ANE ↔ CPU
- FP32 weights or `.all` compute units hiding CPU placement
- Base/Large DA3 variants on Vision Pro

**Diagnose:** Xcode → Instruments → Core ML template → inspect compute-unit breakdown during `prediction`.

**Runtime knobs (Xcode scheme environment):**

| Variable | Effect |
|----------|--------|
| `DA3_COMPUTE_UNITS=ane` | `.cpuAndNeuralEngine`, fall back to `.all` if load fails |
| `DA3_COMPUTE_UNITS=strict_ane` | `.cpuAndNeuralEngine` only — fail load if ANE path cannot compile |
| `DA3_COMPUTE_UNITS=all` | Allow Core ML to place ops on CPU/GPU/ANE |

Swift does not expose a public `.neuralEngineOnly` on all SDKs; `.cpuAndNeuralEngine` is the strictest portable setting.

**Re-export checklist** (`Tools/convert_depth_anything_to_coreml.py`):

1. DA3-**Small** only  
2. PyTorch wrapper exports **depth only** (strip ray-map / pose head for single HLS frames)  
3. `--compute-unit cpuAndNeuralEngine` and `--compute-precision FLOAT16` (defaults)  
4. Validate **<42 ms** inference on device for 24 fps depth; **<17 ms** for 60 fps  

### Depth Anything V2 fallback (smooth playback now)

While retuning DA3 Core ML, use Apple's optimized **Depth Anything V2 Small** Core ML model with KSPlayer's `DepthAnythingV2DepthEstimationAdapter` (see `Video2DTo3DConversion.swift`). Bundle `da2-small.mlmodelc` (or your converted artifact), wire `options.videoDepthEstimationProvider`, and keep perfecting CompositorLayer stereo at 30–60 fps. Swap back to DA3 when the re-export profiles clean on ANE.

The on-screen metrics distinguish the configured inference cap from actual measured inference FPS. They also show preprocessing, Core ML inference, depth texture upload, Metal smoothing, render upload, render duration, stale-depth counts, model input shape, and the effective Core ML compute units. The Vision Pro modes are tuning presets only:

- Stability: 6 fps depth cap, strong smoothing.
- Balanced: 12 fps depth cap, moderate smoothing.
- Performance: 18 fps cap, continuous scheduling (runs Core ML back-to-back while frames arrive).
- Maximum: 30 fps cap, continuous scheduling, minimal smoothing. Uses `cpuAndNeuralEngine` by default. Depth post-processing runs on GPU (`normalizeDepthTexture` + `smoothDepthTexture`); CPU `VideoDepthMap` is built only in **Depth Only** debug mode.

Immersive stereo keeps **video** on the stream PTS timeline (e.g. 24/30/60 fps from the decoder). **DA3 depth** runs only at the sustainable rate the device can measure (often a few fps on Vision Pro); performance modes cap depth attempts (6–18 fps), not video playback. The window Metal path still dequeues decoded frames during immersive (without presenting) so the FFmpeg queue does not stall.

`KSDepth3DPlugin` is the lightweight public plugin shape for app-owned depth providers. The Depth Anything adapter conforms to it, while the VisionProDepthAnythingV3 demo keeps the DA3-specific Core ML loading in the app target because the model resource is private.

## Future 384x216 Export

Use `Tools/convert_depth_anything_to_coreml.py` as the PyTorch-to-Core ML template for a new DA3 Small artifact. Defaults: `1 x 1 x 3 x 216 x 384` multi-array input, **FP16**, `compute_units = cpuAndNeuralEngine`; validate names, shape, depth-only export, ANE placement in Instruments, and measured headset frame time before replacing the current fixed `336 x 336` model.

Core ML Tools 9 does not have a direct ONNX converter. Export or trace the DA3 Small wrapper from PyTorch, then convert that PyTorch artifact to an mlprogram.

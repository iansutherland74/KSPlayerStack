# Vision Pro Depth Anything V3 + KSPlayer Scaffold

This folder is an app-target scaffold for wiring a generated Core ML Depth Anything V3 model to KSPlayer's decoded-frame callback on visionOS. It is intentionally outside the `KSPlayer` Swift package target so `swift build --package-path KSPlayer --target KSPlayer` does not require generated model symbols.

## Files

- `Sources/Depth/DepthAnythingV3Engine.swift` owns the generated `DepthAnythingV3` Core ML model inside an actor and converts `DepthAnythingV3Output.depth` (`MLMultiArray` shaped `[1, 1, H, W]`) into `DA3DepthFrame`.
- `Sources/Player/KSPlayer3DIntegration.swift` installs `KSPlayerLayer.videoOutput`, drops overlapping inference work, sends decoded `CVPixelBuffer`s to the actor, builds a `.r32Float` Metal depth texture, and forwards it to a renderer on the main actor.
- `Sources/Rendering/StereoRenderer.swift` is a minimal `StereoRendererProtocol` implementation that records the latest source/depth dimensions and keeps the latest depth texture for a real stereoscopic renderer to consume.
- `Sources/App/ThreeDPlayerRootView.swift` is a SwiftUI entry point using `KSVideoPlayer` and the integration view model.

## App Target Setup

1. Add `DepthAnythingV3.mlmodel` to the visionOS app target and make sure Xcode generates the Swift types `DepthAnythingV3`, `DepthAnythingV3Input`, and `DepthAnythingV3Output`.
2. Add these demo source files to the same visionOS app target.
3. Add `DEPTH_ANYTHING_V3_GENERATED` to the app target's Active Compilation Conditions. The sample files are gated by this flag so they cannot accidentally compile before the generated model exists.
4. Link/import the local `KSPlayer` package. The reusable `DA3DepthFrame` and `DA3DepthMetalBridge` helpers live in the package target and do not reference generated model symbols.

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

The sample keeps raw DA3 depth values in the `.r32Float` texture. If your renderer expects normalized disparity, call `DA3DepthFrame.normalizedDisparityMap(normalization:invertDepth:)` or normalize in the shader intentionally instead of assuming the model output range.

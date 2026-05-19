# VisionProDepthAnythingV3App

Small installable visionOS host app for the KSPlayer + Depth Anything V3 2D-to-3D scaffold.

## Open

Open `KSPlayer/Demo/VisionProDepthAnythingV3App/VisionProDepthAnythingV3App.xcodeproj` in Xcode and select the `VisionProDepthAnythingV3App` scheme.

The app links the local `KSPlayer` Swift package by relative path (`../..`) and includes the scaffold sources from `KSPlayer/Demo/VisionProDepthAnythingV3/Sources` by reference.

## Model Resource

The target copies `KSPlayer/PrivateArtifacts/DepthAnything3/coreml/DA3-SMALL/compiled/da3-small.mlmodelc` into the app bundle by reference. It does not duplicate the private artifact in this demo folder.

Xcode does not generate Swift model wrapper types from an already compiled `.mlmodelc` bundle. To enable the real DA3 path:

1. Add the source `DepthAnythingV3.mlmodel` or `.mlpackage` to this app target.
2. Confirm Xcode generates `DepthAnythingV3`, `DepthAnythingV3Input`, and `DepthAnythingV3Output`.
3. Add `DEPTH_ANYTHING_V3_GENERATED` to the target's Active Compilation Conditions.

Until that flag is set, the app builds as an installable shell and shows guidance instead of compiling the generated-symbol-dependent player path.

## Signing

The placeholder bundle ID is `com.example.KSPlayer.VisionProDepthAnythingV3`, and `DEVELOPMENT_TEAM` is intentionally blank. For device install, set a unique bundle ID and development team in Xcode.

The app target also patches the copied `libshaderc_combined.framework` bundle identifier in the build output from `com.kintan.ksplayer.libshaderc_combined` to `com.kintan.ksplayer.libshaderccombined`, because visionOS product validation rejects underscores in embedded framework bundle IDs.

## Command-Line Build

```sh
xcodebuild \
  -project KSPlayer/Demo/VisionProDepthAnythingV3App/VisionProDepthAnythingV3App.xcodeproj \
  -scheme VisionProDepthAnythingV3App \
  -configuration Debug \
  -destination 'platform=visionOS Simulator,name=Apple Vision Pro,OS=26.2' \
  ARCHS=arm64 \
  ONLY_ACTIVE_ARCH=YES \
  CODE_SIGNING_ALLOWED=NO \
  build
```

The explicit arm64 simulator settings avoid attempting an x86_64 simulator link against the current arm64-only FFmpeg/private framework slices.

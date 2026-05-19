# Private Artifact Staging

This directory is the landing zone for artifacts that should only be uploaded
after the repository is private. Keep public scaffolding, source links, and
templates tracked; put large binaries, weights, generated model packages, and
hardware result captures under Git LFS.

Do not stage any artifact until its license, provenance, checksum, target
platform, and intended app bundle path are recorded in the matching run notes.

## Suggested Layout

```text
KSPlayer/PrivateArtifacts/
  DepthAnything3/
    weights/<variant>/model.safetensors
    coreml/<variant>/DepthAnything3<Variant>.mlmodelc/
    onnx/<variant>/DepthAnything3<Variant>.onnx
  Runtimes/
    ONNXRuntime/visionOS/onnxruntime.xcframework/
  FelBaker/
    sources/felbaker/                 # Optional private checkout/submodule mirror
    binaries/libKSPlayerFelBakerShim.dylib
  HardwareValidation/
    YYYY-MM-DD-device-os-build/
      RUN.md
      latency/
      routes/
      logs/
      captures/
```

## Depth Anything 3 Sources

Official source repo: `https://github.com/ByteDance-Seed/depth-anything-3`.
Official Hugging Face weights are public and currently ship as
`model.safetensors`, not as Apple-ready `.mlmodelc` packages.

| Variant | Hugging Face repo | License | Approx storage | Notes |
| --- | --- | --- | --- | --- |
| Small | `depth-anything/DA3-SMALL` | Apache-2.0 | 137 MB | Best first Vision Pro realtime candidate. |
| Base | `depth-anything/DA3-BASE` | Apache-2.0 | 542 MB | Higher quality, needs device profiling. |
| Large | `depth-anything/DA3-LARGE` | CC-BY-NC-4.0 | 1.64 GB | Non-commercial license. |
| Giant | `depth-anything/DA3-GIANT` | CC-BY-NC-4.0 | 5.42 GB | Not a realtime Vision Pro starting point. |
| Mono Large | `depth-anything/DA3MONO-LARGE` | Apache-2.0 | 1.34 GB | Monocular large variant. |
| Metric Large | `depth-anything/DA3METRIC-LARGE` | Apache-2.0 | 1.34 GB | Metric depth variant. |

ONNX files are not published by the official DA3 repos above. Public community
exports exist, but should be treated as third-party artifacts: examples include
`onnx-community/depth-anything-v3-small` PR refs, `AXERA-TECH/Depth-Anything-3`,
and `TillBeemelmanns/Depth-Anything-V3-ONNX`. Prefer exporting from the official
PyTorch/Hugging Face weights when reproducibility matters.

Core ML packages are not official release artifacts. Convert from a TorchScript,
`torch.export`, or ONNX graph with `Tools/convert_depth_anything_to_coreml.py`,
then compile with `xcrun coremlcompiler` and validate on Vision Pro before
placing the generated `.mlmodelc` here.

## ONNX Runtime for visionOS

Microsoft ONNX Runtime has merged visionOS framework build support, but the
official SwiftPM package may not include visionOS slices. Build from the ONNX
Runtime source when a public release package does not advertise `xros` and
`xrsimulator` support:

```sh
git clone --recursive https://github.com/microsoft/onnxruntime
cd onnxruntime
python3 tools/ci_build/github/apple/build_apple_framework.py \
  --config Release \
  tools/ci_build/github/apple/default_vision_os_framework_build_settings.json
```

The expected output is `build/apple_framework/framework_out/onnxruntime.xcframework`.
Use `--build_dynamic_framework` if the app needs a dynamic framework. If the DA3
ONNX export uses a limited operator set, add `--include_ops_by_config` to reduce
binary size only after validating the reduced build.

## FelBaker Runtime

Upstream source: `https://github.com/bbeny123/felbaker`. FelBaker is a
GPL-3.0-or-later VapourSynth plugin that builds C++20 code and a static
`libdovi` from `quietvoid/dovi_tool` via Rust `cargo-c`. The repo expects Python
3.12+, Meson, Ninja, Rust, `cargo-c`, Clang for best macOS performance, and the
vendored VapourSynth headers.

The KSPlayer shim can either:

- use `Tools/FelBakerShim` with `make real-direct` after placing a FelBaker
  checkout plus `build/libdovi/rpu_parser.h` and `build/libdovi/libdovi.a` under
  `Tools/FelBakerShim/external/felbaker`, or
- use `make real-custom` with private adapter sources and private link flags.

Do not claim `KSPLAYER_FELBAKER_CAPABILITY_FULL_FEL_COMPOSITION` until the
private shim writes a full BL+EL+RPU composed frame into the output buffer and
passes real Dolby Vision Profile 7 FEL validation material.

## Hardware Validation

Use `HardwareValidation/RUN_TEMPLATE.md` for real device runs. Do not fabricate
results. Store latency logs, route diagnostics, AirPlay/HDMI/Bluetooth/Spatial
Audio notes, device/OS/build metadata, and captures under a dated run folder.

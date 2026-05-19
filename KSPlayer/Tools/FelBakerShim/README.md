# KSPlayer FelBaker Shim

This is a private/education build scaffold for a Dolby Vision Profile 7 FEL compositor shim. It exports the C ABI documented in `../../Documents/KSPlayerFelBakerShim.h` so `FelBakerDolbyVisionFELCompositorBackend` can load it with `dlopen`.

The default build is intentionally a safe stub:

- `ksplayer_felbaker_backend_abi_version()` returns `KSPLAYER_FELBAKER_SHIM_ABI_VERSION`.
- `ksplayer_felbaker_backend_name()` identifies the shim as a stub.
- `ksplayer_felbaker_backend_capabilities()` returns `0`, so KSPlayer will not mark FEL composition as available.
- `ksplayer_felbaker_compose()` returns `KSPLAYER_FELBAKER_STATUS_NOT_IMPLEMENTED` and writes a diagnostic.

Build and smoke-test the stub on macOS:

```sh
cd KSPlayer/Tools/FelBakerShim
make clean smoke
```

This produces `build/libKSPlayerFelBakerShim.dylib` as a universal arm64/x86_64 dynamic library by default. To build only the host architecture, override `ARCH_FLAGS`, for example:

```sh
make clean smoke ARCH_FLAGS="-arch arm64"
```

## Wiring Into KSPlayer

Configure the backend explicitly from the app or private build harness:

```swift
let shimURL = URL(fileURLWithPath: "/absolute/path/to/libKSPlayerFelBakerShim.dylib")

let options = KSOptions()
options.dolbyVisionFELPlaybackPolicy = .requireFullComposition
options.dolbyVisionFELCompositorBackend = FelBakerDolbyVisionFELCompositorBackend(
    libraryURL: shimURL
)
```

The stub will load, but strict mode still blocks FEL playback because it does not advertise `KSPLAYER_FELBAKER_CAPABILITY_FULL_FEL_COMPOSITION`. That is the expected behavior until a real compositor bridge is compiled in.

## Real FelBaker Bridge

KSPlayer does not vendor FelBaker or libdovi. Private builds can compile a real
shim by placing a FelBaker checkout and libdovi build artifacts under the shim:

```text
KSPlayer/Tools/FelBakerShim/
  external/felbaker/              # https://github.com/bbeny123/felbaker
    src/RpuProcessor.h
    src/RpuProcessor.cpp
    src/Utils.h
    build/libdovi/rpu_parser.h
    build/libdovi/libdovi.a
```

One way to produce that layout from a FelBaker checkout is:

```sh
cd KSPlayer/Tools/FelBakerShim
git clone --recurse-submodules https://github.com/bbeny123/felbaker external/felbaker
cd external/felbaker
meson setup build -Dinstall_plugin=false -Dtests=false
meson compile -C build libdovi
cd ../..
make clean real-direct ARCH_FLAGS="-arch arm64"
```

The `real-direct` target builds these pieces into
`build/libKSPlayerFelBakerShim.dylib`:

- `KSPlayerFelBakerRealCompositor.cpp` validates BL/EL/output arguments, locks
  `CVPixelBuffer` planes, maps CoreVideo plane layout, forwards aligned RPU
  bytes and `CMTime`, and returns diagnostics through the C ABI.
- `KSPlayerFelBakerDirectCore.cpp` uses FelBaker's libdovi-backed
  `RpuProcessor` to compose decoded 10-bit 4:2:0 bi-planar BL/EL frames into
  `kCVPixelFormatType_64RGBAHalf` output without Apple or Dolby private APIs.
- The default stub remains available with `make clean smoke` when real
  dependencies are absent.

Current direct-core limits are explicit: BL and EL must already be decoded,
same-dimension, 10-bit 4:2:0 bi-planar CoreVideo buffers, and output must be
`kCVPixelFormatType_64RGBAHalf`. FelBaker's full VapourSynth plugin also has
resampling paths for other source layouts; if a private build needs those, add a
custom core adapter rather than reporting full capability from an incomplete
path.

Custom private adapters can use the concrete KSPlayer bridge and provide only
the core call:

```sh
make clean real-custom \
  PRIVATE_SOURCES="/path/to/KSPlayerFelBakerCustomCore.cpp" \
  PRIVATE_CXXFLAGS="-I/path/to/private/includes" \
  PRIVATE_LDFLAGS="-L/path/to/private/lib -lprivatefelbaker"
```

`PRIVATE_SOURCES` must implement:

```cpp
uint64_t ksplayer_felbaker_core_capabilities(void);

int32_t ksplayer_felbaker_core_compose(
    const KSPlayerFelBakerCompositionRequest &request,
    KSPlayerFelBakerDiagnosticWriter diagnostic);
```

Only return `KSPLAYER_FELBAKER_CAPABILITY_FULL_FEL_COMPOSITION` after the adapter
actually writes a fully composed BL+EL+RPU frame into the supplied output
`CVPixelBuffer` layout.

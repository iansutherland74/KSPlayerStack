/*
 KSPlayer private/education Dolby Vision Profile 7 FEL compositor shim.

 This target intentionally builds a safe stub by default. It exports the C ABI
 expected by FelBakerDolbyVisionFELCompositorBackend, but it does not claim FEL
 composition capability until a private FelBaker-derived compositor bridge is
 compiled in with KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR.
 */

#include "KSPlayerFelBakerShim.h"

#include <cstdio>
#include <cstring>

#if defined(KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR)
#if __has_include("KSPlayerFelBakerRealCompositor.hpp")
#include "KSPlayerFelBakerRealCompositor.hpp"
#else
#error "KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR requires KSPlayerFelBakerRealCompositor.hpp on the include path"
#endif
#endif

#if defined(__GNUC__)
#define KSPLAYER_FELBAKER_EXPORT __attribute__((visibility("default")))
#else
#define KSPLAYER_FELBAKER_EXPORT
#endif

namespace {

#if !defined(KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR)
void writeDiagnostic(char *buffer, size_t bufferSize, const char *message) {
    if (buffer == nullptr || bufferSize == 0) {
        return;
    }
    std::snprintf(buffer, bufferSize, "%s", message);
}
#endif

#if defined(KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR)
// The private FelBaker bridge is expected to provide these functions:
//
// uint64_t ksplayer_felbaker_real_capabilities(void);
// int32_t ksplayer_felbaker_real_compose(
//     CVPixelBufferRef base_layer,
//     CVPixelBufferRef enhancement_layer,
//     const uint8_t *rpu_data,
//     size_t rpu_size,
//     CMTime presentation_time,
//     CVPixelBufferRef output,
//     char *diagnostic_buffer,
//     size_t diagnostic_buffer_size);
//
// Insert BL + EL + RPU parsing, residual reconstruction, mapping, and output
// pixel conversion in that private implementation. Do not report
// KSPLAYER_FELBAKER_CAPABILITY_FULL_FEL_COMPOSITION until that path writes a
// fully composed frame into `output`.
#endif

} // namespace

extern "C" KSPLAYER_FELBAKER_EXPORT uint32_t ksplayer_felbaker_backend_abi_version(void) {
    return KSPLAYER_FELBAKER_SHIM_ABI_VERSION;
}

extern "C" KSPLAYER_FELBAKER_EXPORT const char *ksplayer_felbaker_backend_name(void) {
#if defined(KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR)
    return "KSPlayer FelBaker FEL Compositor";
#else
    return "KSPlayer FelBaker Shim Stub";
#endif
}

extern "C" KSPLAYER_FELBAKER_EXPORT uint64_t ksplayer_felbaker_backend_capabilities(void) {
#if defined(KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR)
    return ksplayer_felbaker_real_capabilities();
#else
    return 0;
#endif
}

extern "C" KSPLAYER_FELBAKER_EXPORT int32_t ksplayer_felbaker_compose(
    CVPixelBufferRef base_layer,
    CVPixelBufferRef enhancement_layer,
    const uint8_t *rpu_data,
    size_t rpu_size,
    CMTime presentation_time,
    CVPixelBufferRef output,
    char *diagnostic_buffer,
    size_t diagnostic_buffer_size
) {
#if defined(KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR)
    return ksplayer_felbaker_real_compose(
        base_layer,
        enhancement_layer,
        rpu_data,
        rpu_size,
        presentation_time,
        output,
        diagnostic_buffer,
        diagnostic_buffer_size
    );
#else
    (void)base_layer;
    (void)enhancement_layer;
    (void)rpu_data;
    (void)rpu_size;
    (void)presentation_time;
    (void)output;
    writeDiagnostic(
        diagnostic_buffer,
        diagnostic_buffer_size,
        "KSPlayer FelBaker shim was built as a stub; rebuild with "
        "KSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR and a private FelBaker-derived "
        "bridge to compose BL+EL+RPU frames."
    );
    return KSPLAYER_FELBAKER_STATUS_NOT_IMPLEMENTED;
#endif
}

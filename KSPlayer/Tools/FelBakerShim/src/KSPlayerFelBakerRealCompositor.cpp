/*
 KSPlayer private/education Dolby Vision Profile 7 FEL compositor bridge.

 This file owns the stable KSPlayer side of the bridge: argument validation,
 CVPixelBuffer locking, plane layout extraction, and forwarding to a
 FelBaker-derived core adapter. The default stub target does not compile this
 file; real targets must provide ksplayer_felbaker_core_compose().
 */

#include "KSPlayerFelBakerRealCompositor.hpp"

#include "KSPlayerFelBakerBridgeTypes.hpp"

#include <CoreVideo/CoreVideo.h>
#include <cstdio>

namespace {

void writeDiagnostic(char *buffer, size_t bufferSize, const char *message) noexcept {
    if (buffer == nullptr || bufferSize == 0) {
        return;
    }
    std::snprintf(buffer, bufferSize, "%s", message);
}

class PixelBufferLock final {
public:
    PixelBufferLock(CVPixelBufferRef pixelBuffer, CVPixelBufferLockFlags flags) noexcept
        : pixelBuffer(pixelBuffer), flags(flags) {
        status = CVPixelBufferLockBaseAddress(pixelBuffer, flags);
    }

    ~PixelBufferLock() noexcept {
        if (status == kCVReturnSuccess) {
            CVPixelBufferUnlockBaseAddress(pixelBuffer, flags);
        }
    }

    PixelBufferLock(const PixelBufferLock &) = delete;
    PixelBufferLock &operator=(const PixelBufferLock &) = delete;

    bool locked() const noexcept {
        return status == kCVReturnSuccess;
    }

private:
    CVPixelBufferRef pixelBuffer{};
    CVPixelBufferLockFlags flags{};
    CVReturn status{};
};

KSPlayerFelBakerFrameView makeFrameView(CVPixelBufferRef pixelBuffer) noexcept {
    KSPlayerFelBakerFrameView frame{};
    auto &layout = frame.layout;
    layout.pixel_format = CVPixelBufferGetPixelFormatType(pixelBuffer);
    layout.width = static_cast<uint32_t>(CVPixelBufferGetWidth(pixelBuffer));
    layout.height = static_cast<uint32_t>(CVPixelBufferGetHeight(pixelBuffer));

    if (CVPixelBufferIsPlanar(pixelBuffer)) {
        const size_t planeCount = CVPixelBufferGetPlaneCount(pixelBuffer);
        layout.plane_count = static_cast<uint32_t>(planeCount > 4 ? 4 : planeCount);
        for (uint32_t plane = 0; plane < layout.plane_count; ++plane) {
            layout.planes[plane].base_address = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, plane);
            layout.planes[plane].bytes_per_row = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, plane);
            layout.planes[plane].width = static_cast<uint32_t>(CVPixelBufferGetWidthOfPlane(pixelBuffer, plane));
            layout.planes[plane].height = static_cast<uint32_t>(CVPixelBufferGetHeightOfPlane(pixelBuffer, plane));
        }
        return frame;
    }

    layout.plane_count = 1;
    layout.planes[0].base_address = CVPixelBufferGetBaseAddress(pixelBuffer);
    layout.planes[0].bytes_per_row = CVPixelBufferGetBytesPerRow(pixelBuffer);
    layout.planes[0].width = layout.width;
    layout.planes[0].height = layout.height;
    return frame;
}

} // namespace

void KSPlayerFelBakerDiagnosticWriter::write(const char *message) const noexcept {
    writeDiagnostic(buffer, size, message);
}

uint64_t ksplayer_felbaker_real_capabilities(void) {
#if defined(KSPLAYER_FELBAKER_HAS_CORE_ADAPTER)
    return ksplayer_felbaker_core_capabilities();
#else
    return 0;
#endif
}

int32_t ksplayer_felbaker_real_compose(
    CVPixelBufferRef base_layer,
    CVPixelBufferRef enhancement_layer,
    const uint8_t *rpu_data,
    size_t rpu_size,
    CMTime presentation_time,
    CVPixelBufferRef output,
    char *diagnostic_buffer,
    size_t diagnostic_buffer_size
) {
    if (base_layer == nullptr || enhancement_layer == nullptr || output == nullptr) {
        writeDiagnostic(diagnostic_buffer, diagnostic_buffer_size, "BL, EL, and output CVPixelBuffer values are required");
        return KSPLAYER_FELBAKER_STATUS_INVALID_ARGUMENT;
    }
    if (rpu_data == nullptr || rpu_size == 0) {
        writeDiagnostic(diagnostic_buffer, diagnostic_buffer_size, "Aligned Dolby Vision RPU bytes are required");
        return KSPLAYER_FELBAKER_STATUS_INVALID_ARGUMENT;
    }

    PixelBufferLock baseLock(base_layer, kCVPixelBufferLock_ReadOnly);
    PixelBufferLock enhancementLock(enhancement_layer, kCVPixelBufferLock_ReadOnly);
    PixelBufferLock outputLock(output, 0);
    if (!baseLock.locked() || !enhancementLock.locked() || !outputLock.locked()) {
        writeDiagnostic(diagnostic_buffer, diagnostic_buffer_size, "Unable to lock one or more CVPixelBuffer planes");
        return KSPLAYER_FELBAKER_STATUS_UNAVAILABLE;
    }

    const KSPlayerFelBakerCompositionRequest request{
        .base_layer = makeFrameView(base_layer),
        .enhancement_layer = makeFrameView(enhancement_layer),
        .rpu = KSPlayerFelBakerRPUView{.data = rpu_data, .size = rpu_size},
        .presentation_time = presentation_time,
        .output = makeFrameView(output),
    };

#if defined(KSPLAYER_FELBAKER_HAS_CORE_ADAPTER)
    return ksplayer_felbaker_core_compose(
        request,
        KSPlayerFelBakerDiagnosticWriter{.buffer = diagnostic_buffer, .size = diagnostic_buffer_size}
    );
#else
#error "Real FelBaker builds must compile a core adapter. Use make real-direct with FELBAKER_ROOT/libdovi, or make real-custom with PRIVATE_SOURCES implementing ksplayer_felbaker_core_compose()."
#endif
}

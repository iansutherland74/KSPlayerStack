/*
 KSPlayer optional Dolby Vision Profile 7 FEL compositor shim ABI.

 This header is documentation for private/education builds that provide their
 own FelBaker-derived dynamic library. KSPlayer does not build or vendor the
 GPL FelBaker implementation in the core package.
 */

#ifndef KSPLAYER_FELBAKER_SHIM_H
#define KSPLAYER_FELBAKER_SHIM_H

#include <CoreMedia/CoreMedia.h>
#include <CoreVideo/CoreVideo.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KSPLAYER_FELBAKER_SHIM_ABI_VERSION 1u
#define KSPLAYER_FELBAKER_CAPABILITY_FULL_FEL_COMPOSITION (1ull << 0)
#define KSPLAYER_FELBAKER_CAPABILITY_PQ12_OUTPUT (1ull << 1)
#define KSPLAYER_FELBAKER_CAPABILITY_RGB48_OUTPUT (1ull << 2)

#define KSPLAYER_FELBAKER_STATUS_OK 0
#define KSPLAYER_FELBAKER_STATUS_UNAVAILABLE -1000
#define KSPLAYER_FELBAKER_STATUS_NOT_IMPLEMENTED -1001
#define KSPLAYER_FELBAKER_STATUS_INVALID_ARGUMENT -1002
#define KSPLAYER_FELBAKER_STATUS_UNSUPPORTED_FORMAT -1003

typedef struct KSPlayerFelBakerPlane {
    void *base_address;
    size_t bytes_per_row;
    uint32_t width;
    uint32_t height;
} KSPlayerFelBakerPlane;

typedef struct KSPlayerFelBakerPixelBufferLayout {
    uint32_t pixel_format;
    uint32_t width;
    uint32_t height;
    uint32_t plane_count;
    KSPlayerFelBakerPlane planes[4];
} KSPlayerFelBakerPixelBufferLayout;

/*
 Required exported symbols:

 uint32_t ksplayer_felbaker_backend_abi_version(void);
 const char *ksplayer_felbaker_backend_name(void);
 uint64_t ksplayer_felbaker_backend_capabilities(void);

 The compose function receives decoded BL and EL CVPixelBufferRef values,
 raw RPU bytes for the aligned presentation timestamp, and an already-created
 output CVPixelBufferRef. The shim should lock/read/write CoreVideo planes as
 needed, or map them into KSPlayerFelBakerPixelBufferLayout internally.

 Return 0 on success. Return a non-zero private error code on failure. If
 diagnostic_buffer is non-null, write a null-terminated human-readable message.
 */
int32_t ksplayer_felbaker_compose(
    CVPixelBufferRef base_layer,
    CVPixelBufferRef enhancement_layer,
    const uint8_t *rpu_data,
    size_t rpu_size,
    CMTime presentation_time,
    CVPixelBufferRef output,
    char *diagnostic_buffer,
    size_t diagnostic_buffer_size
);

#ifdef __cplusplus
}
#endif

#endif /* KSPLAYER_FELBAKER_SHIM_H */

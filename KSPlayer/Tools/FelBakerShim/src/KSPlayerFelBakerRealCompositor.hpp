/*
 KSPlayer private/education FelBaker compositor entry points.
 */

#pragma once

#include "KSPlayerFelBakerShim.h"

uint64_t ksplayer_felbaker_real_capabilities(void);

int32_t ksplayer_felbaker_real_compose(
    CVPixelBufferRef base_layer,
    CVPixelBufferRef enhancement_layer,
    const uint8_t *rpu_data,
    size_t rpu_size,
    CMTime presentation_time,
    CVPixelBufferRef output,
    char *diagnostic_buffer,
    size_t diagnostic_buffer_size
);

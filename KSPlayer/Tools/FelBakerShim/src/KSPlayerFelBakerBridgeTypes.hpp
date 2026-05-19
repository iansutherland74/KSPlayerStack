/*
 KSPlayer private/education FelBaker bridge types.

 These types are intentionally independent from VapourSynth so private builds
 can provide either a trimmed FelBaker core or their own adapter implementation.
 */

#pragma once

#include "KSPlayerFelBakerShim.h"

#include <CoreMedia/CoreMedia.h>
#include <cstddef>
#include <cstdint>

struct KSPlayerFelBakerFrameView {
    KSPlayerFelBakerPixelBufferLayout layout{};
};

struct KSPlayerFelBakerRPUView {
    const uint8_t *data{};
    size_t size{};
};

struct KSPlayerFelBakerCompositionRequest {
    KSPlayerFelBakerFrameView base_layer{};
    KSPlayerFelBakerFrameView enhancement_layer{};
    KSPlayerFelBakerRPUView rpu{};
    CMTime presentation_time{};
    KSPlayerFelBakerFrameView output{};
};

struct KSPlayerFelBakerDiagnosticWriter {
    char *buffer{};
    size_t size{};

    void write(const char *message) const noexcept;
};

uint64_t ksplayer_felbaker_core_capabilities(void);

int32_t ksplayer_felbaker_core_compose(
    const KSPlayerFelBakerCompositionRequest &request,
    KSPlayerFelBakerDiagnosticWriter diagnostic
);

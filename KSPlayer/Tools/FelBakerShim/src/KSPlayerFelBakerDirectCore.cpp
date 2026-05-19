/*
 Direct FelBaker-derived core adapter for private/education builds.

 This adapter uses FelBaker's libdovi-backed RpuProcessor from a caller-provided
 FelBaker source checkout. It supports decoded 10-bit 4:2:0 bi-planar BL/EL
 CVPixelBuffers and writes composed RGB into kCVPixelFormatType_64RGBAHalf.
 */

#include "KSPlayerFelBakerBridgeTypes.hpp"

#include <CoreVideo/CoreVideo.h>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <exception>

#include "RpuProcessor.h"

namespace {

constexpr uint16_t oneHalf = 0x3c00;

const char *fourCC(uint32_t value, char (&storage)[5]) noexcept {
    storage[0] = static_cast<char>((value >> 24) & 0xff);
    storage[1] = static_cast<char>((value >> 16) & 0xff);
    storage[2] = static_cast<char>((value >> 8) & 0xff);
    storage[3] = static_cast<char>(value & 0xff);
    storage[4] = '\0';
    return storage;
}

uint16_t floatToHalfBits(float value) noexcept {
    uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));

    const uint32_t sign = (bits >> 16) & 0x8000;
    int32_t exponent = static_cast<int32_t>((bits >> 23) & 0xff) - 127 + 15;
    uint32_t mantissa = bits & 0x7fffff;

    if (exponent <= 0) {
        if (exponent < -10) {
            return static_cast<uint16_t>(sign);
        }
        mantissa = (mantissa | 0x800000) >> (1 - exponent);
        return static_cast<uint16_t>(sign | ((mantissa + 0x1000) >> 13));
    }

    if (exponent >= 31) {
        return static_cast<uint16_t>(sign | 0x7c00);
    }

    return static_cast<uint16_t>(sign | (static_cast<uint32_t>(exponent) << 10) | ((mantissa + 0x1000) >> 13));
}

uint16_t normalizedHalf(uint16_t value) noexcept {
    return floatToHalfBits(static_cast<float>(value) / 65535.0f);
}

bool isSupportedInputFormat(uint32_t pixelFormat) noexcept {
    return pixelFormat == kCVPixelFormatType_420YpCbCr10BiPlanarVideoRange ||
        pixelFormat == kCVPixelFormatType_420YpCbCr10BiPlanarFullRange;
}

const uint8_t *byteRow(const KSPlayerFelBakerPlane &plane, uint32_t y) noexcept {
    return static_cast<const uint8_t *>(plane.base_address) + (static_cast<size_t>(y) * plane.bytes_per_row);
}

uint8_t *mutableByteRow(const KSPlayerFelBakerPlane &plane, uint32_t y) noexcept {
    return static_cast<uint8_t *>(plane.base_address) + (static_cast<size_t>(y) * plane.bytes_per_row);
}

uint16_t readP010Sample(const KSPlayerFelBakerPlane &plane, uint32_t x, uint32_t y) noexcept {
    const auto *row = reinterpret_cast<const uint16_t *>(byteRow(plane, y));
    return static_cast<uint16_t>(row[x] >> 6);
}

uint16_t readP010Chroma(const KSPlayerFelBakerPlane &plane, uint32_t x, uint32_t y, uint32_t component) noexcept {
    const auto *row = reinterpret_cast<const uint16_t *>(byteRow(plane, y));
    return static_cast<uint16_t>(row[x * 2 + component] >> 6);
}

void writeRGBAHalf(
    const KSPlayerFelBakerPlane &plane,
    uint32_t x,
    uint32_t y,
    uint16_t red,
    uint16_t green,
    uint16_t blue
) noexcept {
    auto *pixel = reinterpret_cast<uint16_t *>(mutableByteRow(plane, y) + (static_cast<size_t>(x) * 8));
    pixel[0] = normalizedHalf(red);
    pixel[1] = normalizedHalf(green);
    pixel[2] = normalizedHalf(blue);
    pixel[3] = oneHalf;
}

bool validateRequest(
    const KSPlayerFelBakerCompositionRequest &request,
    KSPlayerFelBakerDiagnosticWriter diagnostic
) noexcept {
    const auto &bl = request.base_layer.layout;
    const auto &el = request.enhancement_layer.layout;
    const auto &out = request.output.layout;

    if (!isSupportedInputFormat(bl.pixel_format) || el.pixel_format != bl.pixel_format) {
        char blFormat[5]{};
        char elFormat[5]{};
        char message[160]{};
        std::snprintf(
            message,
            sizeof(message),
            "Direct FelBaker core requires matching 10-bit 4:2:0 bi-planar BL/EL buffers; got BL %s, EL %s",
            fourCC(bl.pixel_format, blFormat),
            fourCC(el.pixel_format, elFormat)
        );
        diagnostic.write(message);
        return false;
    }

    if (bl.width == 0 || bl.height == 0 || (bl.width % 2) != 0 || (bl.height % 2) != 0) {
        diagnostic.write("Direct FelBaker core requires non-empty even BL dimensions");
        return false;
    }

    if (bl.width != el.width || bl.height != el.height) {
        diagnostic.write("Direct FelBaker core currently requires BL and EL to have matching decoded dimensions");
        return false;
    }

    if (out.pixel_format != kCVPixelFormatType_64RGBAHalf || out.width != bl.width || out.height != bl.height) {
        diagnostic.write("Direct FelBaker core currently writes only kCVPixelFormatType_64RGBAHalf at BL dimensions");
        return false;
    }

    if (bl.plane_count < 2 || el.plane_count < 2 || out.plane_count < 1) {
        diagnostic.write("Direct FelBaker core received an unexpected CVPixelBuffer plane layout");
        return false;
    }

    if (
        bl.planes[0].base_address == nullptr || bl.planes[1].base_address == nullptr ||
        el.planes[0].base_address == nullptr || el.planes[1].base_address == nullptr ||
        out.planes[0].base_address == nullptr
    ) {
        diagnostic.write("Direct FelBaker core received a null CVPixelBuffer plane address");
        return false;
    }

    if (request.rpu.data == nullptr || request.rpu.size == 0 || request.rpu.size > static_cast<size_t>(INT_MAX)) {
        diagnostic.write("Direct FelBaker core requires a non-empty RPU NAL unit smaller than INT_MAX");
        return false;
    }

    return true;
}

template<bool PolynomialU, bool PolynomialV, bool ApplyEl>
void compose420(
    const KSPlayerFelBakerCompositionRequest &request,
    const RpuProcessor &rpuProcessor
) noexcept {
    const auto &bl = request.base_layer.layout;
    const auto &el = request.enhancement_layer.layout;
    const auto &out = request.output.layout;

    const auto &blY = bl.planes[0];
    const auto &blUV = bl.planes[1];
    const auto &elY = el.planes[0];
    const auto &elUV = el.planes[1];
    const auto &dst = out.planes[0];

    for (uint32_t chromaY = 0; chromaY < bl.height / 2; ++chromaY) {
        for (uint32_t chromaX = 0; chromaX < bl.width / 2; ++chromaX) {
            const uint32_t x = chromaX * 2;
            const uint32_t y = chromaY * 2;

            const uint16_t yTL = readP010Sample(blY, x, y);
            const uint16_t yTR = readP010Sample(blY, x + 1, y);
            const uint16_t yBL = readP010Sample(blY, x, y + 1);
            const uint16_t yBR = readP010Sample(blY, x + 1, y + 1);
            const uint16_t u = readP010Chroma(blUV, chromaX, chromaY, 0);
            const uint16_t v = readP010Chroma(blUV, chromaX, chromaY, 1);

            const uint16_t elYTL = ApplyEl ? readP010Sample(elY, x, y) : 0;
            const uint16_t elYTR = ApplyEl ? readP010Sample(elY, x + 1, y) : 0;
            const uint16_t elYBL = ApplyEl ? readP010Sample(elY, x, y + 1) : 0;
            const uint16_t elYBR = ApplyEl ? readP010Sample(elY, x + 1, y + 1) : 0;
            const uint16_t elU = ApplyEl ? readP010Chroma(elUV, chromaX, chromaY, 0) : 0;
            const uint16_t elV = ApplyEl ? readP010Chroma(elUV, chromaX, chromaY, 1) : 0;

            const uint32_t leftX = x > 0 ? x - 1 : 0;
            const int mmrTop = 2 + 2 * yTL + yTR + readP010Sample(blY, leftX, y);
            const int mmrBottom = 2 + 2 * yBL + yBR + readP010Sample(blY, leftX, y + 1);
            const uint16_t mmrY = static_cast<uint16_t>(((mmrTop >> 2) + (mmrBottom >> 2) + 1) >> 1);

            const uint16_t mappedU = rpuProcessor.mapU<PolynomialU, ApplyEl>(u, elU, mmrY, u, v);
            const uint16_t mappedV = rpuProcessor.mapV<PolynomialV, ApplyEl>(v, elV, mmrY, u, v);

            const uint16_t mappedYTL = rpuProcessor.mapY<ApplyEl>(yTL, elYTL);
            const uint16_t mappedYTR = rpuProcessor.mapY<ApplyEl>(yTR, elYTR);
            const uint16_t mappedYBL = rpuProcessor.mapY<ApplyEl>(yBL, elYBL);
            const uint16_t mappedYBR = rpuProcessor.mapY<ApplyEl>(yBR, elYBR);

            uint16_t red = 0;
            uint16_t green = 0;
            uint16_t blue = 0;

            RpuProcessor::toRGB(rpuProcessor.getYccToRgb(), mappedYTL, mappedU, mappedV, red, green, blue);
            writeRGBAHalf(dst, x, y, red, green, blue);
            RpuProcessor::toRGB(rpuProcessor.getYccToRgb(), mappedYTR, mappedU, mappedV, red, green, blue);
            writeRGBAHalf(dst, x + 1, y, red, green, blue);
            RpuProcessor::toRGB(rpuProcessor.getYccToRgb(), mappedYBL, mappedU, mappedV, red, green, blue);
            writeRGBAHalf(dst, x, y + 1, red, green, blue);
            RpuProcessor::toRGB(rpuProcessor.getYccToRgb(), mappedYBR, mappedU, mappedV, red, green, blue);
            writeRGBAHalf(dst, x + 1, y + 1, red, green, blue);
        }
    }
}

template<bool PolynomialU, bool PolynomialV>
void compose420(
    const KSPlayerFelBakerCompositionRequest &request,
    const RpuProcessor &rpuProcessor
) noexcept {
    if (rpuProcessor.useEl()) {
        compose420<PolynomialU, PolynomialV, true>(request, rpuProcessor);
    } else {
        compose420<PolynomialU, PolynomialV, false>(request, rpuProcessor);
    }
}

void dispatchCompose(
    const KSPlayerFelBakerCompositionRequest &request,
    const RpuProcessor &rpuProcessor
) noexcept {
    if (rpuProcessor.isPolynomialU() && rpuProcessor.isPolynomialV()) {
        compose420<true, true>(request, rpuProcessor);
    } else if (rpuProcessor.isPolynomialU()) {
        compose420<true, false>(request, rpuProcessor);
    } else if (rpuProcessor.isPolynomialV()) {
        compose420<false, true>(request, rpuProcessor);
    } else {
        compose420<false, false>(request, rpuProcessor);
    }
}

} // namespace

uint64_t ksplayer_felbaker_core_capabilities(void) {
    return KSPLAYER_FELBAKER_CAPABILITY_FULL_FEL_COMPOSITION |
        KSPLAYER_FELBAKER_CAPABILITY_RGB48_OUTPUT;
}

int32_t ksplayer_felbaker_core_compose(
    const KSPlayerFelBakerCompositionRequest &request,
    KSPlayerFelBakerDiagnosticWriter diagnostic
) {
    (void)request.presentation_time;

    if (!validateRequest(request, diagnostic)) {
        return KSPLAYER_FELBAKER_STATUS_UNSUPPORTED_FORMAT;
    }

    try {
        const RpuProcessor rpuProcessor(
            request.rpu.data,
            static_cast<int>(request.rpu.size),
            10,
            10,
            false
        );
        dispatchCompose(request, rpuProcessor);
        diagnostic.write("FelBaker direct core composed BL+EL+RPU into RGBA half output");
        return KSPLAYER_FELBAKER_STATUS_OK;
    } catch (const std::exception &error) {
        diagnostic.write(error.what());
        return KSPLAYER_FELBAKER_STATUS_UNAVAILABLE;
    } catch (...) {
        diagnostic.write("FelBaker direct core failed with an unknown error");
        return KSPLAYER_FELBAKER_STATUS_UNAVAILABLE;
    }
}

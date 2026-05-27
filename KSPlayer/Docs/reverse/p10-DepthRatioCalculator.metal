// Recovered from Viture P10 ExternalMonitor 1.9.25 embedded strings + IDA sub_1001A5108.
// Source path in binary: DepthRatioCalculator.metal (VI TURE, 2025/3/13)
// Braces repaired where `strings` truncated the blob.

#include <metal_stdlib>
using namespace metal;

static float calculateWeight(uint2 gid, uint2 size) {
    float2 center = float2(size) / 2.0f;
    float2 pos = float2(gid);
    float distance = length(pos - center);
    float maxDistance = length(center);
    return 1.0f - (distance / maxDistance);
}

float3 getRGB(texture2d<half, access::read> tex, uint2 gid) {
    half4 c = tex.read(gid);
    return float3(c.b, c.g, c.r);
}

kernel void computeWeightedColorDiff(
    texture2d<half, access::read> depthTexture [[texture(0)]],
    texture2d<half, access::read> colorTexture1 [[texture(1)]],
    texture2d<half, access::read> colorTexture2 [[texture(2)]],
    device atomic_uint* resultBuffer [[buffer(0)]],
    uint2 gid [[thread_position_in_grid]]
) {
    const uint kScaleFactor = 100000;
    float depth = float(depthTexture.read(gid).r);
    uint2 gidNearby = uint2(gid.x + 1, gid.y);
    float depthNearBy = float(depthTexture.read(gidNearby).r);
    float curGrad = abs(depth - depthNearBy);
    if (curGrad > 0.01) {
        float3 color1 = getRGB(colorTexture1, gid);
        float3 color2 = getRGB(colorTexture2, gid);
        float3 diff = color1 - color2;
        float colorDiff = abs(diff.r) + abs(diff.g) + abs(diff.b);
        uint scaledColorDiff = uint(colorDiff * curGrad * kScaleFactor);
        atomic_fetch_add_explicit(resultBuffer, scaledColorDiff, memory_order_relaxed);
    }
}

kernel void computeWeightedRatio(
    texture2d<half, access::read> depthTexture1 [[texture(0)]],
    texture2d<half, access::read> depthTexture2 [[texture(1)]],
    device atomic_uint* weightedSum [[buffer(0)]],
    device atomic_uint* totalWeight [[buffer(1)]],
    device atomic_uint* grad [[buffer(2)]],
    device atomic_uint* totalGrad [[buffer(3)]],
    uint2 gid [[thread_position_in_grid]]
) {
    const uint kScaleFactor = 10000;
    float depth1 = float(depthTexture1.read(gid).r);
    float depth2 = float(depthTexture2.read(gid).r);
    uint2 gidNearby = uint2(gid.x + 1, gid.y);
    float depth1NearBy = float(depthTexture1.read(gidNearby).r);
    float curGrad = abs(depth1 - depth1NearBy);
    if (curGrad > 0.5) {
        uint width = depthTexture1.get_width();
        uint height = depthTexture1.get_height();
        float weight = calculateWeight(gid, uint2(width, height));
        atomic_fetch_add_explicit(grad, uint(curGrad * weight * kScaleFactor), memory_order_relaxed);
        atomic_fetch_add_explicit(totalGrad, uint(curGrad * kScaleFactor), memory_order_relaxed);
    }
    if (depth1 >= 0.005f && depth1 <= 1.0f && depth2 >= 0.005f && depth2 <= 1.0f && depth1 >= 0.07f) {
        float ratio = depth1 / depth2;
        uint width = depthTexture1.get_width();
        uint height = depthTexture1.get_height();
        float weight = calculateWeight(gid, uint2(width, height));
        weight = weight * depth1;
        uint scaledRatio = uint(ratio * weight * kScaleFactor);
        uint scaledWeight = uint(weight * kScaleFactor);
        atomic_fetch_add_explicit(weightedSum, scaledRatio, memory_order_relaxed);
        atomic_fetch_add_explicit(totalWeight, scaledWeight, memory_order_relaxed);
    }
}

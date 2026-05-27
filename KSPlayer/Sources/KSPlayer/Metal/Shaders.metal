//
//  Shaders.metal
#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float4 pos [[attribute(0)]];
    float2 uv [[attribute(1)]];
};

struct VertexOut {
    float4 renderedCoordinate [[position]];
    float2 textureCoordinate;
};

vertex VertexOut mapTexture(VertexIn input [[stage_in]]) {
    VertexOut outVertex;
    outVertex.renderedCoordinate = input.pos;
    outVertex.textureCoordinate = input.uv;
    return outVertex;
}

vertex VertexOut mapSphereTexture(VertexIn input [[stage_in]], constant float4x4& uniforms [[ buffer(2) ]]) {
    VertexOut outVertex;
    outVertex.renderedCoordinate = uniforms * input.pos;
    outVertex.textureCoordinate = input.uv;
    return outVertex;
}

half3 applyVideoColorAdjustment(half3 rgb, constant float4& adjustment) {
    if (adjustment.w < 0.5) {
        return rgb;
    }
    half luma = dot(rgb, half3(0.2126, 0.7152, 0.0722));
    rgb = mix(half3(luma), rgb, half(adjustment.x));
    rgb = (rgb - half3(0.5)) * half(adjustment.z) + half3(0.5 + adjustment.y);
    return clamp(rgb, half3(0.0), half3(1.0));
}

float video2DTo3DPseudoDepth(float2 uv) {
    float2 centered = abs(uv - float2(0.5));
    float centerWeight = 1.0 - smoothstep(0.05, 0.75, length(centered));
    float verticalBias = 1.0 - smoothstep(0.15, 1.0, uv.y);
    return clamp(0.35 + 0.35 * centerWeight + 0.15 * verticalBias, 0.0, 1.0);
}

float video2DTo3DCurvedDepth(float depth, float curvature) {
    float centered = clamp(depth, 0.0, 1.0) - 0.5;
    float safeCurvature = clamp(curvature, 0.6, 1.5);
    float magnitude = pow(clamp(abs(centered) * 2.0, 0.0, 1.0), safeCurvature) * 0.5;
    return 0.5 + sign(centered) * magnitude;
}

float video2DTo3DDepth(float2 uv,
                       constant float4& conversion,
                       constant float4& shape,
                       texture2d<float, access::sample> depthTexture,
                       sampler textureSampler) {
    float depth;
    if (conversion.z > 0.5) {
        depth = clamp(depthTexture.sample(textureSampler, uv).r, 0.0, 1.0);
    } else {
        depth = video2DTo3DPseudoDepth(uv);
    }
    return video2DTo3DCurvedDepth(depth, shape.y);
}

float2 applyVideo2DTo3D(float2 uv,
                        constant float4& conversion,
                        constant float4& shape,
                        texture2d<float, access::sample> depthTexture,
                        sampler textureSampler) {
    if (conversion.x < 0.5 || conversion.y <= 0.0) {
        return uv;
    }
    float depth = video2DTo3DDepth(uv, conversion, shape, depthTexture, textureSampler);
    float strength = clamp(conversion.y, 0.0, 1.0);
    float distance = clamp(shape.x, 0.0, 2.0);
    float parallax = clamp((depth - 0.5) * strength * distance * 0.04, -0.035, 0.035) * conversion.w;
    return clamp(uv + float2(parallax, 0.0), float2(0.001, 0.001), float2(0.999, 0.999));
}

struct DepthNormalizeParameters {
    float depthMinimum;
    float depthMaximum;
    float contrast;
    float invertDepth;
};

kernel void normalizeDepthTexture(texture2d<float, access::read> sourceTexture [[ texture(0) ]],
                                  texture2d<float, access::write> outputTexture [[ texture(1) ]],
                                  constant DepthNormalizeParameters& parameters [[ buffer(0) ]],
                                  uint2 gid [[ thread_position_in_grid ]]) {
    if (gid.x >= outputTexture.get_width() || gid.y >= outputTexture.get_height()) {
        return;
    }
    float raw = sourceTexture.read(gid).r;
    float range = max(parameters.depthMaximum - parameters.depthMinimum, 1e-5);
    float normalized = clamp((raw - parameters.depthMinimum) / range, 0.0, 1.0);
    if (parameters.invertDepth > 0.5) {
        normalized = 1.0 - normalized;
    }
    float safeContrast = clamp(parameters.contrast, 0.1, 4.0);
    normalized = clamp(((normalized - 0.5) * safeContrast) + 0.5, 0.0, 1.0);
    outputTexture.write(float4(normalized, 0.0, 0.0, 1.0), gid);
}

kernel void smoothDepthTexture(texture2d<float, access::read> currentTexture [[ texture(0) ]],
                               texture2d<float, access::read> previousTexture [[ texture(1) ]],
                               texture2d<float, access::write> outputTexture [[ texture(2) ]],
                               constant float& previousWeight [[ buffer(0) ]],
                               constant float& maxDisparityChange [[ buffer(1) ]],
                               uint2 gid [[ thread_position_in_grid ]]) {
    if (gid.x >= outputTexture.get_width() || gid.y >= outputTexture.get_height()) {
        return;
    }
    float currentDepth = clamp(currentTexture.read(gid).r, 0.0, 1.0);
    float previousDepth = clamp(previousTexture.read(gid).r, 0.0, 1.0);
    float weight = clamp(previousWeight, 0.0, 0.95);
    float blended = mix(currentDepth, previousDepth, weight);
    float maxChange = clamp(maxDisparityChange, 0.001, 0.25);
    float delta = blended - currentDepth;
    float smoothedDepth = currentDepth + clamp(delta, -maxChange, maxChange);
    outputTexture.write(float4(smoothedDepth, 0.0, 0.0, 1.0), gid);
}

float3 pqToLinear(float3 rgb) {
    rgb = pow(max(rgb, float3(0.0)), float3(4096.0 / (2523.0 * 128.0)));
    rgb = max(rgb - float3(3424.0 / 4096.0), float3(0.0)) /
        (float3(2413.0 / 4096.0 * 32.0) - float3(2392.0 / 4096.0 * 32.0) * rgb);
    rgb = pow(max(rgb, float3(0.0)), float3(4096.0 * 4.0 / 2610.0));
    return rgb;
}

float3 linearToPQ(float3 rgb) {
    rgb = pow(max(rgb, float3(0.0)), float3(2610.0 / 4096.0 / 4.0));
    rgb = (float3(3424.0 / 4096.0) + float3(2413.0 / 4096.0 * 32.0) * rgb) /
        (float3(1.0) + float3(2392.0 / 4096.0 * 32.0) * rgb);
    rgb = pow(max(rgb, float3(0.0)), float3(2523.0 / 4096.0 * 128.0));
    return clamp(rgb, float3(0.0), float3(1.0));
}

constant uint HDR10_PLUS_MAX_WINDOWS = 64;

struct HDR10PlusToneMappingWindow {
    float4 control;
    float4 scene;
    float4 region;
    float4 extra;
    float4 selector;
    float4 selectorAxes;
};

bool hdr10PlusContains(float2 uv, float4 region) {
    return uv.x >= region.x && uv.y >= region.y && uv.x <= region.z && uv.y <= region.w;
}

float hdr10PlusEllipseWeight(float2 uv, float4 extra, float4 selector, float4 axes) {
    if (extra.w < 0.5) {
        return 1.0;
    }
    float2 delta = uv - selector.xy;
    float2 rotated;
    rotated.x = selector.z * delta.x + selector.w * delta.y;
    rotated.y = -selector.w * delta.x + selector.z * delta.y;
    float majorExternal = max(axes.z, 0.0001);
    float minorExternal = max(axes.w, 0.0001);
    float externalDistance = sqrt((rotated.x * rotated.x) / (majorExternal * majorExternal) +
                                  (rotated.y * rotated.y) / (minorExternal * minorExternal));
    if (externalDistance > 1.0) {
        return 0.0;
    }
    float majorInternal = max(axes.x, 0.0001);
    float internalRatio = clamp(majorInternal / majorExternal, 0.0001, 1.0);
    return 1.0 - smoothstep(internalRatio, 1.0, externalDistance);
}

float hdr10PlusWindowWeight(float2 uv, float4 region, float4 extra, float4 selector, float4 axes) {
    if (!hdr10PlusContains(uv, region)) {
        return 0.0;
    }
    return hdr10PlusEllipseWeight(uv, extra, selector, axes);
}

float hdr10PlusMappedMaxRGB(float maxRGB, float4 control, float4 scene, float actualPeakLimit) {
    float kneeX = clamp(control.y, 0.0001, 1.0);
    float kneeY = clamp(control.z, 0.0, 1.0);
    float targetMax = clamp(control.w, max(kneeY, 0.0001), 1.0);
    if (actualPeakLimit > 0.0) {
        targetMax = min(targetMax, max(actualPeakLimit, kneeY));
    }
    float rolloff = clamp(scene.w, 0.5, 8.0);
    if (maxRGB <= kneeX) {
        return maxRGB * kneeY / kneeX;
    }
    float t = clamp((maxRGB - kneeX) / max(1.0 - kneeX, 0.0001), 0.0, 1.0);
    float curved = 1.0 - pow(1.0 - t, rolloff);
    return mix(kneeY, targetMax, curved);
}

float3 hdr10PlusApplySaturation(float3 linearRGB, float saturationWeight) {
    float saturation = clamp(saturationWeight, 0.0, 2.0);
    if (fabs(saturation - 1.0) < 0.0001) {
        return linearRGB;
    }
    float luma = dot(linearRGB, float3(0.2627, 0.6780, 0.0593));
    return max(mix(float3(luma), linearRGB, saturation), float3(0.0));
}

float3 hdr10PlusApplyWindow(float3 linearRGB,
                            float maxRGB,
                            float4 control,
                            float4 scene,
                            float4 extra,
                            float actualPeakLimit) {
    float mappedMaxRGB = hdr10PlusMappedMaxRGB(maxRGB, control, scene, actualPeakLimit);
    float3 mappedRGB = linearRGB * (mappedMaxRGB / maxRGB);
    return hdr10PlusApplySaturation(mappedRGB, extra.x);
}

void hdr10PlusCompositeWindow(float3 mappedRGB,
                              float windowWeight,
                              float overlapMode,
                              thread float3& accumulatedRGB,
                              thread float& accumulatedWeight) {
    if (windowWeight <= 0.0) {
        return;
    }
    if (overlapMode >= 0.5) {
        accumulatedRGB = mix(accumulatedRGB, mappedRGB, clamp(windowWeight, 0.0, 1.0));
        accumulatedWeight = 1.0;
    } else {
        accumulatedRGB += mappedRGB * windowWeight;
        accumulatedWeight += windowWeight;
    }
}

half3 applyHDR10PlusToneMapping(half3 rgb,
                                float2 uv,
                                constant float4& global,
                                constant HDR10PlusToneMappingWindow* windows) {
    if (global.x < 0.5) {
        return rgb;
    }
    uint windowCount = min(uint(global.y), HDR10_PLUS_MAX_WINDOWS);
    if (windowCount == 0) {
        return rgb;
    }
    float3 linearRGB = pqToLinear(float3(rgb));
    float maxRGB = max(max(linearRGB.r, linearRGB.g), linearRGB.b);
    if (maxRGB <= 0.000001) {
        return rgb;
    }
    float globalActualPeakLimit = global.z;
    float3 accumulatedRGB = float3(0.0);
    float accumulatedWeight = 0.0;

    for (uint i = 0; i < windowCount; i++) {
        constant HDR10PlusToneMappingWindow& window = windows[i];
        if (window.control.x < 0.5) {
            continue;
        }
        float windowWeight = hdr10PlusWindowWeight(uv, window.region, window.extra, window.selector, window.selectorAxes);
        if (windowWeight <= 0.0) {
            continue;
        }
        float actualPeak = window.extra.y > 0.0 ? window.extra.y : globalActualPeakLimit;
        float3 mappedRGB = hdr10PlusApplyWindow(linearRGB, maxRGB, window.control, window.scene, window.extra, actualPeak);
        hdr10PlusCompositeWindow(mappedRGB, windowWeight, window.extra.z, accumulatedRGB, accumulatedWeight);
    }
    float3 outputRGB = accumulatedWeight > 0.0 ? accumulatedRGB / accumulatedWeight : linearRGB;
    return half3(linearToPQ(outputRGB));
}

fragment half4 displayTexture(VertexOut mappingVertex [[ stage_in ]],
                              texture2d<half, access::sample> texture [[ texture(0) ]],
                              texture2d<float, access::sample> depthTexture [[ texture(3) ]],
                              constant float4& colorAdjustment [[ buffer(3) ]],
                              constant float4& hdr10PlusGlobal [[ buffer(4) ]],
                              constant HDR10PlusToneMappingWindow* hdr10PlusWindows [[ buffer(5) ]],
                              constant float4& video2DTo3D [[ buffer(6) ]],
                              constant float4& video2DTo3DShape [[ buffer(7) ]]) {
    constexpr sampler s(address::clamp_to_edge, filter::linear);

    float2 textureCoordinate = applyVideo2DTo3D(mappingVertex.textureCoordinate, video2DTo3D, video2DTo3DShape, depthTexture, s);
    half4 color = texture.sample(s, textureCoordinate);
    half3 rgb = applyHDR10PlusToneMapping(color.rgb, textureCoordinate, hdr10PlusGlobal, hdr10PlusWindows);
    return half4(applyVideoColorAdjustment(rgb, colorAdjustment), color.a);
}

fragment half4 displayYUVTexture(VertexOut in [[ stage_in ]],
                                  texture2d<half> yTexture [[ texture(0) ]],
                                  texture2d<half> uTexture [[ texture(1) ]],
                                  texture2d<half> vTexture [[ texture(2) ]],
                                  texture2d<float, access::sample> depthTexture [[ texture(3) ]],
                                  sampler textureSampler [[ sampler(0) ]],
                                  constant float3x3& yuvToBGRMatrix [[ buffer(0) ]],
                                  constant float3& colorOffset [[ buffer(1) ]],
                                  constant uchar3& leftShift [[ buffer(2) ]],
                                  constant float4& colorAdjustment [[ buffer(3) ]],
                                  constant float4& hdr10PlusGlobal [[ buffer(4) ]],
                                  constant HDR10PlusToneMappingWindow* hdr10PlusWindows [[ buffer(5) ]],
                                  constant float4& video2DTo3D [[ buffer(6) ]],
                                  constant float4& video2DTo3DShape [[ buffer(7) ]])
{
    float2 textureCoordinate = applyVideo2DTo3D(in.textureCoordinate, video2DTo3D, video2DTo3DShape, depthTexture, textureSampler);
    half3 yuv;
    yuv.x = yTexture.sample(textureSampler, textureCoordinate).r;
    yuv.y = uTexture.sample(textureSampler, textureCoordinate).r;
    yuv.z = vTexture.sample(textureSampler, textureCoordinate).r;
    half3 rgb = half3x3(yuvToBGRMatrix)*(yuv*half3(leftShift)+half3(colorOffset));
    rgb = applyHDR10PlusToneMapping(rgb, textureCoordinate, hdr10PlusGlobal, hdr10PlusWindows);
    return half4(applyVideoColorAdjustment(rgb, colorAdjustment), 1);
}


fragment half4 displayNV12Texture(VertexOut in [[ stage_in ]],
                                  texture2d<half> lumaTexture [[ texture(0) ]],
                                  texture2d<half> chromaTexture [[ texture(1) ]],
                                  texture2d<float, access::sample> depthTexture [[ texture(3) ]],
                                  sampler textureSampler [[ sampler(0) ]],
                                  constant float3x3& yuvToBGRMatrix [[ buffer(0) ]],
                                  constant float3& colorOffset [[ buffer(1) ]],
                                  constant uchar3& leftShift [[ buffer(2) ]],
                                  constant float4& colorAdjustment [[ buffer(3) ]],
                                  constant float4& hdr10PlusGlobal [[ buffer(4) ]],
                                  constant HDR10PlusToneMappingWindow* hdr10PlusWindows [[ buffer(5) ]],
                                  constant float4& video2DTo3D [[ buffer(6) ]],
                                  constant float4& video2DTo3DShape [[ buffer(7) ]])
{
    float2 textureCoordinate = applyVideo2DTo3D(in.textureCoordinate, video2DTo3D, video2DTo3DShape, depthTexture, textureSampler);
    half3 yuv;
    yuv.x = lumaTexture.sample(textureSampler, textureCoordinate).r;
    yuv.yz = chromaTexture.sample(textureSampler, textureCoordinate).rg;
    half3 rgb = half3x3(yuvToBGRMatrix)*(yuv*half3(leftShift)+half3(colorOffset));
    rgb = applyHDR10PlusToneMapping(rgb, textureCoordinate, hdr10PlusGlobal, hdr10PlusWindows);
    return half4(applyVideoColorAdjustment(rgb, colorAdjustment), 1);
}

half3 shaderLinearize(half3 rgb) {
    rgb = pow(max(rgb,0), half3(4096.0/(2523 * 128)));
    rgb = max(rgb - half3(3424./4096), 0.0) / (half3(2413./4096 * 32) - half3(2392./4096 * 32) * rgb);
    rgb = pow(rgb, half3(4096.0 * 4 / 2610));
    return rgb;
}

half3 shaderDeLinearize(half3 rgb) {
    rgb = pow(max(rgb,0), half3(2610./4096 / 4));
    rgb = (half3(3424./4096) - half3(2413./4096 * 32) * rgb) / (half3(1.0) + half3(2392./4096 * 32) * rgb);
    rgb = pow(rgb, half3(2523./4096 * 128));
    return rgb;
}

fragment half4 displayYCCTexture(VertexOut in [[ stage_in ]],
                                  texture2d<half> lumaTexture [[ texture(0) ]],
                                  texture2d<half> chromaTexture [[ texture(1) ]],
                                  texture2d<float, access::sample> depthTexture [[ texture(3) ]],
                                  sampler textureSampler [[ sampler(0) ]],
                                  constant float3x3& yuvToBGRMatrix [[ buffer(0) ]],
                                  constant float3& colorOffset [[ buffer(1) ]],
                                  constant uchar3& leftShift [[ buffer(2) ]],
                                  constant float4& colorAdjustment [[ buffer(3) ]],
                                  constant float4& hdr10PlusGlobal [[ buffer(4) ]],
                                  constant HDR10PlusToneMappingWindow* hdr10PlusWindows [[ buffer(5) ]],
                                  constant float4& video2DTo3D [[ buffer(6) ]],
                                  constant float4& video2DTo3DShape [[ buffer(7) ]])
{
    float2 textureCoordinate = applyVideo2DTo3D(in.textureCoordinate, video2DTo3D, video2DTo3DShape, depthTexture, textureSampler);
    half3 ipt;
    ipt.x = lumaTexture.sample(textureSampler, textureCoordinate).r;
    ipt.yz = chromaTexture.sample(textureSampler, textureCoordinate).rg;
//    half3x3 ipt2lms = half3x3{{1, 0.1952, 0.4104}, {1, -0.2278, 0.2264}, {1, 0.0652, -1.3538}};
//    half3x3 lms2rgb = half3x3{{3.238998, -0.719461, -0.002862}, {-2.272734, 1.874998, -0.268066}, {0.086733, -0.158947, 1.074494}};
    half3x3 ipt2lms = half3x3{{1, 799/8192, 1681/8192}, {1, -933/8192, 1091/8192}, {1, 267/8192, -5545/8192}};
    half3x3 lms2rgb = half3x3{{3.43661, -0.79133, -0.0259499}, {-2.50645, 1.98360, -0.0989137}, {0.06984, -0.192271, 1.12486}};
    half3 lms = ipt2lms*ipt;
    lms = shaderLinearize(lms);
    half3 rgb = lms2rgb*lms;
    rgb = shaderDeLinearize(rgb);
    rgb = applyHDR10PlusToneMapping(rgb, textureCoordinate, hdr10PlusGlobal, hdr10PlusWindows);
    return half4(applyVideoColorAdjustment(rgb, colorAdjustment), 1);
}

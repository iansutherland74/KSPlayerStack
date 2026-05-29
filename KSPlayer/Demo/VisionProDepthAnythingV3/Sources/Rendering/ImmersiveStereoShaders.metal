#include <metal_stdlib>
using namespace metal;

struct ImmersiveVertexOut {
    float4 position [[position]];
    float2 uv;
};

struct ImmersiveStereoUniforms {
    float4 video2DTo3D;
    float4 video2DTo3DShape;
};

constant float3x3 kImmersiveBT709FullRange = float3x3(
    float3(1.0, 1.0, 1.0),
    float3(0.0, -0.187324, 1.8556),
    float3(1.5748, -0.468124, 0.0)
);

constant float3 kImmersiveColorOffset = float3(0.0, -0.5, -0.5);

/// SDR Rec.709 display-referred → extended linear for rgba16Float compositor drawables.
float3 immersiveSDRToExtendedLinear(half3 srgb) {
    float3 c = max(float3(srgb), 0.0);
    return pow(c, 2.2);
}

float immersivePseudoDepth(float2 uv) {
    float2 centered = uv - 0.5;
    return clamp(1.0 - length(centered) * 1.35, 0.0, 1.0);
}

float immersiveCurvedDepth(float depth, float curvature) {
    float centered = depth - 0.5;
    return clamp(0.5 + centered * curvature, 0.0, 1.0);
}

float immersiveSampleDepth(float2 uv,
                           constant ImmersiveStereoUniforms& uniforms,
                           texture2d<float, access::sample> depthTexture,
                           sampler textureSampler) {
    float4 conversion = uniforms.video2DTo3D;
    float4 shape = uniforms.video2DTo3DShape;
    float depth = immersivePseudoDepth(uv);
    if (conversion.z > 0.5) {
        depth = depthTexture.sample(textureSampler, uv).r;
    }
    return immersiveCurvedDepth(depth, shape.y);
}

float2 immersiveStereoUV(float2 uv,
                         constant ImmersiveStereoUniforms& uniforms,
                         texture2d<float, access::sample> depthTexture,
                         sampler textureSampler) {
    float4 conversion = uniforms.video2DTo3D;
    float4 shape = uniforms.video2DTo3DShape;
    if (conversion.x < 0.5 || conversion.y <= 0.0) {
        return uv;
    }
    float depth = immersiveSampleDepth(uv, uniforms, depthTexture, textureSampler);
    float strength = clamp(conversion.y, 0.0, 1.0);
    float distance = clamp(shape.x, 0.0, 2.0);
    float parallax = clamp((depth - 0.5) * strength * distance * 0.03, -0.015, 0.015) * conversion.w;
    return clamp(uv + float2(parallax, 0.0), float2(0.001, 0.001), float2(0.999, 0.999));
}

/// World-space screen quad (unit XY plane); MVP supplied per eye from Compositor Services.
vertex ImmersiveVertexOut immersiveWorldVertex(uint vertexID [[vertex_id]],
                                               constant float4x4& modelViewProjection [[buffer(0)]]) {
    float2 corners[6] = {
        float2(-1.0, -1.0), float2(1.0, -1.0), float2(-1.0, 1.0),
        float2(1.0, -1.0), float2(1.0, 1.0), float2(-1.0, 1.0)
    };
    float2 uvs[6] = {
        float2(0.0, 1.0), float2(1.0, 1.0), float2(0.0, 0.0),
        float2(1.0, 1.0), float2(1.0, 0.0), float2(0.0, 0.0)
    };
    ImmersiveVertexOut out;
    float3 modelPosition = float3(corners[vertexID], 0.0);
    out.position = modelViewProjection * float4(modelPosition, 1.0);
    out.uv = uvs[vertexID];
    return out;
}

fragment half4 immersiveBGRAFragment(ImmersiveVertexOut in [[stage_in]],
                                     texture2d<half, access::sample> colorTexture [[texture(0)]],
                                     texture2d<float, access::sample> depthTexture [[texture(1)]],
                                     sampler textureSampler [[sampler(0)]],
                                     constant ImmersiveStereoUniforms& uniforms [[buffer(0)]]) {
    float2 uv = immersiveStereoUV(in.uv, uniforms, depthTexture, textureSampler);
    half4 rgba = colorTexture.sample(textureSampler, uv);
    return half4(rgba.rgb, 1.0h);
}

fragment float4 immersiveRGBA16Fragment(ImmersiveVertexOut in [[stage_in]],
                                        texture2d<half, access::sample> colorTexture [[texture(0)]],
                                        texture2d<float, access::sample> depthTexture [[texture(1)]],
                                        sampler textureSampler [[sampler(0)]],
                                        constant ImmersiveStereoUniforms& uniforms [[buffer(0)]]) {
    float2 uv = immersiveStereoUV(in.uv, uniforms, depthTexture, textureSampler);
    half4 srgb = colorTexture.sample(textureSampler, uv);
    return float4(immersiveSDRToExtendedLinear(srgb.rgb), 1.0);
}

fragment half4 immersiveNV12Fragment(ImmersiveVertexOut in [[stage_in]],
                                   texture2d<half> lumaTexture [[texture(0)]],
                                   texture2d<half> chromaTexture [[texture(1)]],
                                   texture2d<float, access::sample> depthTexture [[texture(2)]],
                                   sampler textureSampler [[sampler(0)]],
                                   constant float3x3& yuvToBGRMatrix [[buffer(0)]],
                                   constant float3& colorOffset [[buffer(1)]],
                                   constant uchar3& leftShift [[buffer(2)]],
                                   constant ImmersiveStereoUniforms& uniforms [[buffer(3)]]) {
    float2 uv = immersiveStereoUV(in.uv, uniforms, depthTexture, textureSampler);
    half3 yuv;
    yuv.x = lumaTexture.sample(textureSampler, uv).r;
    yuv.yz = chromaTexture.sample(textureSampler, uv).rg;
    half3 rgb = half3x3(yuvToBGRMatrix) * (yuv * half3(leftShift) + half3(colorOffset));
    return half4(rgb, 1.0h);
}

/// Solid fill for presentation-path debugging (set ImmersiveStereoCompositor.debugSolidPresentation).
fragment half4 immersiveDebugSolidFragment(ImmersiveVertexOut in [[stage_in]]) {
    return half4(1.0h, 0.0h, 1.0h, 1.0h);
}

half4 immersivePlaceholderColor(float2 uv) {
    (void)uv;
    // Transparent placeholder so there is no visible border/frame before video.
    return half4(half3(0.0h, 0.0h, 0.0h), 0.0h);
}

/// Visible standby screen while DA3 has not produced a frame yet.
fragment half4 immersivePlaceholderFragment(ImmersiveVertexOut in [[stage_in]],
                                              constant ImmersiveStereoUniforms& uniforms [[buffer(0)]]) {
    (void)uniforms;
    return immersivePlaceholderColor(in.uv);
}

fragment float4 immersiveNV12RGBA16Fragment(ImmersiveVertexOut in [[stage_in]],
                                          texture2d<half> lumaTexture [[texture(0)]],
                                          texture2d<half> chromaTexture [[texture(1)]],
                                          texture2d<float, access::sample> depthTexture [[texture(2)]],
                                          sampler textureSampler [[sampler(0)]],
                                          constant float3x3& yuvToBGRMatrix [[buffer(0)]],
                                          constant float3& colorOffset [[buffer(1)]],
                                          constant uchar3& leftShift [[buffer(2)]],
                                          constant ImmersiveStereoUniforms& uniforms [[buffer(3)]]) {
    float2 uv = immersiveStereoUV(in.uv, uniforms, depthTexture, textureSampler);
    half3 yuv;
    yuv.x = lumaTexture.sample(textureSampler, uv).r;
    yuv.yz = chromaTexture.sample(textureSampler, uv).rg;
    half3 rgb = half3x3(yuvToBGRMatrix) * (yuv * half3(leftShift) + half3(colorOffset));
    return float4(immersiveSDRToExtendedLinear(rgb), 1.0);
}

fragment float4 immersivePlaceholderRGBA16Fragment(ImmersiveVertexOut in [[stage_in]],
                                                   constant ImmersiveStereoUniforms& uniforms [[buffer(0)]]) {
    (void)uniforms;
    half4 placeholder = immersivePlaceholderColor(in.uv);
    return float4(immersiveSDRToExtendedLinear(placeholder.rgb), float(placeholder.a));
}

fragment float4 immersiveDebugSolidRGBA16Fragment(ImmersiveVertexOut in [[stage_in]]) {
    return float4(immersiveSDRToExtendedLinear(half3(1.0h, 0.0h, 1.0h)), 1.0);
}

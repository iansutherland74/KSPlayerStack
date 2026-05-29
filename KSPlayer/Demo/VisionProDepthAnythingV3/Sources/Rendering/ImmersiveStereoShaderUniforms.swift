#if os(visionOS) && canImport(Metal)
import KSPlayer
@preconcurrency import Metal
import simd

/// CPU-side mirror of `ImmersiveStereoUniforms` in `ImmersiveStereoShaders.metal`.
struct ImmersiveStereoShaderUniforms {
    var video2DTo3D: SIMD4<Float>
    var video2DTo3DShape: SIMD4<Float>

    /// True when a live depth texture should drive parallax (texture is authoritative; ring
    /// `configuration.usesDepthMap` can lag until `updateConfiguration` / `attachDepth` catch up).
    static func effectiveUsesDepthMap(
        configuration: Video2DTo3DRenderConfiguration,
        depthTexture: (any MTLTexture)?,
        isDepthStaleForPresentation: Bool
    ) -> Bool {
        guard depthTexture != nil, !isDepthStaleForPresentation else {
            return false
        }
        return configuration.isEnabled && configuration.usesDepthMap
    }

    static func make(
        configuration: Video2DTo3DRenderConfiguration,
        eye: StereoscopicVideoEye,
        usesDepthMap: Bool
    ) -> ImmersiveStereoShaderUniforms {
        // Without a real depth map, disable parallax (do not use pseudo-depth — it tears at UV edges).
        let parallaxActive = configuration.isEnabled && usesDepthMap
        let eyeSign: Float = eye == .left ? -1 : 1
        return ImmersiveStereoShaderUniforms(
            video2DTo3D: SIMD4<Float>(
                parallaxActive ? 1 : 0,
                parallaxActive ? configuration.depthStrength : 0,
                usesDepthMap ? 1 : 0,
                parallaxActive ? eyeSign : 0
            ),
            video2DTo3DShape: SIMD4<Float>(
                configuration.depthDistance,
                configuration.depthCurvature,
                configuration.depthSmoothingFactor,
                0
            )
        )
    }
}
#endif

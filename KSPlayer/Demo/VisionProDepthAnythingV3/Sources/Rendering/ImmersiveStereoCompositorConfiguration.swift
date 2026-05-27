#if os(visionOS) && canImport(CompositorServices)
import CompositorServices
import Metal
import _CompositorServices_SwiftUI

/// Minimal compositor configuration validated against device capabilities.
struct ImmersiveStereoCompositorConfiguration: CompositorLayerConfiguration {
    func makeConfiguration(
        capabilities: LayerRenderer.Capabilities,
        configuration: inout LayerRenderer.Configuration
    ) {
        let supportsFoveation = capabilities.supportsFoveation
        let layoutOptions: LayerRenderer.Capabilities.SupportedLayoutsOptions = supportsFoveation
            ? [.foveationEnabled]
            : []
        let supportedLayouts = capabilities.supportedLayouts(options: layoutOptions)

        // Prefer dedicated layout: head-locked clip-space quads map cleanly to per-eye drawables.
        if supportedLayouts.contains(.dedicated) {
            configuration.layout = .dedicated
        } else if supportedLayouts.contains(.layered) {
            configuration.layout = .layered
        }
        configuration.isFoveationEnabled = supportsFoveation

        let supportedColorFormats = capabilities.supportedColorFormats(options: [])
        if supportedColorFormats.contains(.bgra8Unorm) {
            configuration.colorFormat = .bgra8Unorm
        } else if supportedColorFormats.contains(.rgba16Float) {
            configuration.colorFormat = .rgba16Float
        }

        let supportedDepthFormats = capabilities.supportedDepthFormats
        if supportedDepthFormats.contains(.depth32Float) {
            configuration.depthFormat = .depth32Float
        } else if supportedDepthFormats.contains(.depth32Float_stencil8) {
            configuration.depthFormat = .depth32Float_stencil8
        }
    }
}
#endif

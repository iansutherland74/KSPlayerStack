#if os(visionOS) && canImport(CoreML) && canImport(Metal)
import Foundation
import KSPlayer
import SwiftUI

@MainActor
public struct ThreeDPlayerRootView: View {
    @Environment(\.openImmersiveSpace) private var openImmersiveSpace
    @Environment(\.dismissImmersiveSpace) private var dismissImmersiveSpace
    @StateObject private var viewModel: KSPlayer3DIntegrationViewModel
    @State private var windowVideoAspect: CGFloat = 16.0 / 9.0

    public init(viewModel: KSPlayer3DIntegrationViewModel) {
        _viewModel = StateObject(wrappedValue: viewModel)
    }

    public init(url: URL, options: KSOptions = KSOptions()) throws {
        let viewModel = try KSPlayer3DIntegrationViewModel(url: url, options: options)
        self.init(viewModel: viewModel)
    }

    public var body: some View {
        GeometryReader { proxy in
            let isWideLayout = proxy.size.width >= 980

            Group {
                if isWideLayout {
                    HStack(alignment: .top, spacing: 24) {
                        videoSurface
                            .frame(maxWidth: .infinity, maxHeight: .infinity)

                        controlsPanel
                            .frame(width: min(max(proxy.size.width * 0.32, 360), 440))
                            .zIndex(1)
                    }
                } else {
                    VStack(spacing: 20) {
                        videoSurface
                            .frame(maxWidth: .infinity)
                            .frame(maxHeight: max(min(proxy.size.height * 0.58, proxy.size.height - 280), 260))

                        controlsPanel
                            .layoutPriority(1)
                            .zIndex(1)
                    }
                }
            }
            .padding(24)
        }
        .onAppear {
            viewModel.resetImmersivePresentationForWindowLaunch()
            viewModel.ensure3DPreviewActivatedOnLaunch()
            viewModel.bindImmersiveStereo(
                open: {
                    await MainActor.run {
                        viewModel.clearImmersiveOpenFailureReason()
                    }
                    let result = await openImmersiveSpace(id: ImmersiveStereoSpace.id)
                    switch result {
                    case .opened:
                        Depth3DDebug.log("openImmersiveSpace returned opened", phase: "presentation-mode")
                        print("KSPlayer.DA3: openImmersiveSpace returned .opened")
                        return true
                    case .userCancelled:
                        await MainActor.run {
                            viewModel.setImmersiveOpenFailureReason(
                                "Immersive space open was cancelled."
                            )
                        }
                        Depth3DDebug.warn("openImmersiveSpace cancelled by system/user", phase: "presentation-mode")
                        return false
                    case .error:
                        await MainActor.run {
                            viewModel.setImmersiveOpenFailureReason(
                                "The system could not open the immersive space. Close other immersive apps and try again."
                            )
                        }
                        Depth3DDebug.warn("openImmersiveSpace returned error", phase: "presentation-mode")
                        return false
                    @unknown default:
                        await MainActor.run {
                            viewModel.setImmersiveOpenFailureReason(
                                "Unknown immersive open result."
                            )
                        }
                        return false
                    }
                },
                dismiss: {
                    await dismissImmersiveSpace()
                }
            )
        }
    }

    private var videoSurface: some View {
        KSVideoPlayer(
            coordinator: viewModel.coordinator,
            url: viewModel.url,
            options: viewModel.options
        )
        .onStateChanged { layer, state in
            viewModel.handleStateChanged(layer: layer, state: state)
        }
        .onPlay { currentTime, _ in
            if let layer = viewModel.coordinator.playerLayer {
                viewModel.handlePlaybackTimeUpdate(layer: layer, currentTime: currentTime)
            }
        }
        .onFinish { _, error in
            viewModel.handleFinish(error: error)
        }
        .onDisappear {
            viewModel.stop()
        }
        .aspectRatio(windowVideoAspect, contentMode: .fit)
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(.black, in: RoundedRectangle(cornerRadius: 28))
        .clipShape(RoundedRectangle(cornerRadius: 28))
        .opacity(viewModel.hidesWindowVideoSurface ? 0 : 1)
        .allowsHitTesting(!viewModel.hidesWindowVideoSurface)
        .accessibilityHidden(viewModel.hidesWindowVideoSurface)
    }

    private var controlsPanel: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 12) {
                if let lastErrorMessage = viewModel.lastErrorMessage {
                    Text(lastErrorMessage)
                        .font(.caption)
                        .foregroundStyle(.white)
                        .padding(12)
                        .frame(maxWidth: .infinity, alignment: .leading)
                        .background(.red.opacity(0.72), in: RoundedRectangle(cornerRadius: 14))
                }

                DA3PlaybackControlsView(viewModel: viewModel)
            }
            .frame(maxWidth: .infinity, alignment: .leading)
        }
        .scrollIndicators(.visible)
    }
}

private struct DA3PlaybackControlsView: View {
    @ObservedObject var viewModel: KSPlayer3DIntegrationViewModel

    var body: some View {
        VStack(alignment: .leading, spacing: 18) {
            da3StatusView

            HStack(spacing: 12) {
                Button {
                    viewModel.play()
                } label: {
                    Label(viewModel.playerState == .playedToTheEnd ? "Replay" : "Play", systemImage: "play.fill")
                }
                .buttonStyle(.borderedProminent)

                Button {
                    viewModel.restart()
                } label: {
                    Label("Restart", systemImage: "backward.end.fill")
                }
                .buttonStyle(.bordered)

                Button {
                    viewModel.pause()
                } label: {
                    Label("Pause", systemImage: "pause.fill")
                }
                .buttonStyle(.bordered)

                Spacer(minLength: 0)
            }

            VStack(alignment: .leading, spacing: 10) {
                HStack(spacing: 12) {
                    Button {
                        viewModel.handleEnable3DPreviewButtonTap()
                    } label: {
                        Label("Enable 3D Preview", systemImage: "cube.transparent")
                    }
                    .buttonStyle(.borderedProminent)

                    Button {
                        viewModel.set2DTo3DEnabled(false)
                    } label: {
                        Label("Disable 3D", systemImage: "rectangle")
                    }
                    .buttonStyle(.bordered)
                }

                Toggle(
                    "2D-to-3D preview",
                    isOn: Binding(
                        get: { viewModel.is3DPreviewRequested },
                        set: { viewModel.set2DTo3DEnabled($0, source: "2D-to-3D toggle") }
                    )
                )
                .toggleStyle(.switch)

                Text(previewToggleHelpText)
                    .font(.caption)
                    .foregroundStyle(.secondary)

                Toggle(
                    "Loop short demo streams",
                    isOn: Binding(
                        get: { viewModel.isLoopEnabled },
                        set: { viewModel.setLoopEnabled($0) }
                    )
                )
                .toggleStyle(.switch)
            }

            VStack(alignment: .leading, spacing: 8) {
                Text("Vision Pro performance mode")
                    .font(.subheadline)

                HStack(spacing: 8) {
                    ForEach(DA3VisionProPerformanceMode.allCases, id: \.self) { mode in
                        performanceModeButton(mode)
                    }
                }

                Text("Use Maximum for fastest depth. Video follows source PTS; DA3 actual is measured Core ML throughput.")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }

            VStack(alignment: .leading, spacing: 8) {
                Text("Pipeline debug mode")
                    .font(.subheadline)

                HStack(spacing: 8) {
                    ForEach(DA3DebugRenderMode.allCases, id: \.self) { mode in
                        debugModeButton(mode)
                    }
                }
            }

            VStack(alignment: .leading, spacing: 8) {
                Text("Presentation mode")
                    .font(.subheadline)

                HStack(spacing: 8) {
                    presentationModeButton(.native2D)
                    presentationModeButton(.windowSelectedEye)
                    presentationModeButton(.packedPreview)
                    presentationModeButton(.immersiveStereo)
                }

                Text(viewModel.presentationModeStatus)
                    .font(.caption)
                    .foregroundStyle(.secondary)

                Text("Enable 3D first, then Immersive Stereo. This demo uses ARKit world tracking (not Surroundings), so the app may not appear under Settings > Surroundings. Immersive Stereo only works on Apple Vision Pro hardware. Check Last action if nothing changes.")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }

            VStack(alignment: .leading, spacing: 8) {
                Text("Packed debug layout")
                    .font(.subheadline)

                HStack(spacing: 8) {
                    layoutButton(.selectedEye)
                    layoutButton(.sideBySide)
                    layoutButton(.topAndBottom)
                }
            }

            HStack(spacing: 12) {
                Button("Comfort Packed") {
                    viewModel.applyComfortPreviewPreset()
                }
                .buttonStyle(.bordered)

                Button("Strong Packed") {
                    viewModel.applyStrongPreviewPreset()
                }
                .buttonStyle(.borderedProminent)
            }

            DA3SliderRow(
                title: "Depth strength",
                value: depthStrengthBinding,
                range: doubleRange(viewModel.depthStrengthRange)
            )
            DA3SliderRow(
                title: "Depth distance",
                value: depthDistanceBinding,
                range: doubleRange(viewModel.depthDistanceRange)
            )
            DA3SliderRow(
                title: "Depth curvature",
                value: depthCurvatureBinding,
                range: doubleRange(viewModel.depthCurvatureRange)
            )

            DA3SliderRow(
                title: "Depth contrast",
                value: depthContrastBinding,
                range: doubleRange(viewModel.depthContrastRange)
            )

            DA3SliderRow(
                title: "Metal smoothing",
                value: temporalSmoothingBinding,
                range: doubleRange(viewModel.temporalSmoothingFactorRange)
            )

            DA3SliderRow(
                title: "Screen distance",
                value: immersiveScreenDistanceBinding,
                range: doubleRange(viewModel.immersiveScreenDistanceRange),
                valueLabel: { String(format: "%.1f m", $0) }
            )

            Toggle(
                "Invert DA3 depth",
                isOn: Binding(
                    get: { viewModel.isDepthInverted },
                    set: { viewModel.setDepthInverted($0) }
                )
            )
            .toggleStyle(.switch)

            debugStatusView
        }
        .padding(20)
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(.regularMaterial, in: RoundedRectangle(cornerRadius: 24))
    }

    private var da3StatusView: some View {
        let metrics = viewModel.metrics
        let fps = metrics.actualInferenceFPS > 0 ? String(format: "%.1f", metrics.actualInferenceFPS) : "--"
        let depthSize = metrics.depthWidth > 0 && metrics.depthHeight > 0
            ? "\(metrics.depthWidth)x\(metrics.depthHeight)"
            : "--"
        let frameDuration = metrics.lastFrameDuration.map { String(format: "%.0f ms", $0 * 1_000) } ?? "--"
        let preprocessingDuration = formattedDuration(metrics.preprocessingDuration)
        let inferenceDuration = formattedDuration(metrics.inferenceDuration)
        let textureUploadDuration = formattedDuration(metrics.textureUploadDuration)
        let renderUploadDuration = formattedDuration(metrics.renderDepthTextureUploadDuration)
        let renderSmoothingDuration = formattedDuration(metrics.renderDepthSmoothingDuration)
        let renderDuration = formattedDuration(metrics.renderDuration)
        let depthOffset = metrics.depthFrameOffset.map { String(format: "%+.2fs", $0) } ?? "--"
        let smoothing = String(format: "%.2f", Double(metrics.temporalSmoothingFactor))
        let sourceFPS = metrics.sourceVideoFrameRate > 0 ? String(format: "%.1f", metrics.sourceVideoFrameRate) : "--"
        let inferenceCap = metrics.maximumInferenceFPS > 0 ? String(format: "%.1f", metrics.maximumInferenceFPS) : "--"
        let rawDepthRange = formattedRange(minimum: metrics.rawDepthMinimum, maximum: metrics.rawDepthMaximum)
        let normalizedDepthRange = formattedRange(minimum: metrics.normalizedDepthMinimum, maximum: metrics.normalizedDepthMaximum)

        return VStack(alignment: .leading, spacing: 8) {
            HStack(alignment: .firstTextBaseline) {
                Text("DA3 Controls")
                    .font(.headline)

                Spacer(minLength: 12)

                Text(previewBadgeTitle)
                    .font(.caption.weight(.semibold))
                    .foregroundStyle(previewBadgeColor)
                    .padding(.horizontal, 10)
                    .padding(.vertical, 5)
                    .background(.thinMaterial, in: Capsule())
            }

            VStack(alignment: .leading, spacing: 4) {
                Text("Pipeline \(metrics.state.displayName)")
                    .font(.subheadline)

                Text("Activation \(viewModel.activationStatusMessage)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                if !viewModel.depthWarmupStatusMessage.isEmpty {
                    Text(viewModel.depthWarmupStatusMessage)
                        .font(.caption.monospacedDigit())
                        .foregroundStyle(.orange)
                }

                Text("Preview \(viewModel.previewModeDescription)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Source \(sourceFPS) fps  DA3 actual \(fps)  depth cap \(inferenceCap)  Last \(frameDuration)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Depth frames \(metrics.processedFrameCount)  Coalesced \(metrics.coalescedFrameCount)  Throttled \(metrics.throttledFrameCount)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Depth \(depthSize)  Offset \(depthOffset)  Stale \(metrics.staleDepthFrameCount)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Raw \(rawDepthRange)  Normalized \(normalizedDepthRange)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Pre \(preprocessingDuration)  Infer \(inferenceDuration)  Upload \(textureUploadDuration)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Render upload \(renderUploadDuration)  Smooth \(renderSmoothingDuration)  Render \(renderDuration)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Stereo passes \(metrics.renderStereoPassCount)  Depth map smoothing runs in Metal")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                if viewModel.presentationMode == .immersiveStereo,
                   viewModel.isImmersiveStereoPresented || viewModel.isOpeningImmersiveStereo
                {
                    immersiveTimingDashboard
                    Text(viewModel.immersiveTemporalStatusLine)
                        .font(.caption.monospacedDigit())
                        .foregroundStyle(.secondary)
                }

                Text("Mode \(metrics.performanceMode.displayName)  Debug \(metrics.debugRenderMode.displayName)  Smoothing \(smoothing)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Model \(metrics.modelDiagnostics.inputSizeSummary) \(metrics.modelDiagnostics.inputShapeNote)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text(metrics.modelDiagnostics.computeUnitsNote)
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)
            }
        }
    }

    private var debugStatusView: some View {
        VStack(alignment: .leading, spacing: 4) {
            Text("Status")
                .font(.subheadline.weight(.semibold))

            Text("Last action: \(viewModel.lastActionMessage)")
            Text("Last 3D control: \(viewModel.controlEventMessage)")
            Text("Player state: \(viewModel.lastPlayerStateMessage)")
            if viewModel.playerState == .playedToTheEnd {
                Text("End of stream: press Replay/Restart, or enable Loop for continuous testing.")
            }
            Text("URL: \(viewModel.url.absoluteString)")
                .lineLimit(2)
            Text(
                String(
                    format: "Options: mode %@, strength %.2f, distance %.2f, curvature %.2f, contrast %.2f, smoothing %.2f",
                    viewModel.is2DTo3DEnabled ? "depthMapPreferred" : "disabled",
                    Double(viewModel.depthStrength),
                    Double(viewModel.depthDistance),
                    Double(viewModel.depthCurvature),
                    Double(viewModel.depthContrast),
                    Double(viewModel.temporalSmoothingFactor)
                )
            )
            Text("Output: \(outputLayoutTitle(viewModel.outputLayout))  Window: 2D SwiftUI")
            Text("Presentation: \(viewModel.presentationMode.displayName) - \(viewModel.presentationModeStatus)")
            Text("Shader: \(shaderStatus)  Depth: \(depthProviderStatus)")
            Text("Eye: \(eyeTitle(viewModel.options.stereoscopicVideoEye))  Inverted: \(viewModel.isDepthInverted ? "yes" : "no")")
            Text("Applied mode: \(viewModel.lastAppliedMode)  Perf: \(viewModel.performanceMode.displayName)  Loop: \(viewModel.isLoopEnabled ? "on" : "off")")
            Text("Debug phase: \(viewModel.debugPhaseMessage)")
            Text("Debug last: \(viewModel.lastDebugMessage)")
        }
        .font(.caption.monospacedDigit())
        .foregroundStyle(.secondary)
        .padding(12)
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(.thinMaterial, in: RoundedRectangle(cornerRadius: 14))
    }

    private var previewBadgeTitle: String {
        if viewModel.is2DTo3DEnabled {
            return "Enabled"
        }
        if viewModel.is3DPreviewRequested, viewModel.lastErrorMessage != nil {
            return "Unavailable"
        }
        if viewModel.is3DPreviewRequested || viewModel.isApplying2DTo3DMode {
            return "Preparing"
        }
        return "Disabled"
    }

    private var previewBadgeColor: Color {
        if viewModel.is2DTo3DEnabled {
            return .green
        }
        if viewModel.is3DPreviewRequested, viewModel.lastErrorMessage != nil {
            return .red
        }
        if viewModel.is3DPreviewRequested || viewModel.isApplying2DTo3DMode {
            return .orange
        }
        return .secondary
    }

    private var previewToggleHelpText: String {
        if viewModel.is2DTo3DEnabled {
            if viewModel.presentationMode == .packedPreview {
                return "Packed Preview is active in a 2D SwiftUI window. This shows both eyes together for debugging and is not headset stereo."
            }
            if viewModel.presentationMode == .immersiveStereo, viewModel.isImmersiveStereoPresented {
                return "Immersive Stereo is active. Use the controls in the window to adjust depth, then look around the full-screen stereo scene."
            }
            return "Window Eye applies DA3 depth in a flat window. Choose Immersive Stereo for real per-eye headset output. Raise Depth strength above 0.35 if the flat preview looks too subtle."
        }
        if viewModel.is3DPreviewRequested, viewModel.lastErrorMessage != nil {
            return "3D was requested, but this source cannot enter the packed 3D renderer. Disable 3D to clear the request, or use the guidance in the status message."
        }
        if viewModel.is3DPreviewRequested || viewModel.isApplying2DTo3DMode {
            return "3D preview was requested and is preparing. Playback controls and sliders remain usable while DA3 initializes."
        }
        if viewModel.lastErrorMessage != nil {
            return "3D preview is unavailable for the current source. See the status message above for the exact reason."
        }
        return "Normal 2D playback is the default reliable path. Tap Enable 3D Preview only after Play/Load works."
    }

    private var shaderStatus: String {
        guard viewModel.is2DTo3DEnabled else {
            return "off"
        }
        if viewModel.debugRenderMode == .depthDisabled {
            return "disabled for timing"
        }
        if viewModel.debugRenderMode == .depthOnly {
            return "depth capture only"
        }
        if viewModel.debugRenderMode == .stereoOnly {
            return "pseudo stereo only"
        }
        return viewModel.metrics.depthWidth > 0 ? "disparity on, DA3 depth" : "disparity on, pseudo fallback"
    }

    private var depthProviderStatus: String {
        let metrics = viewModel.metrics
        guard viewModel.is2DTo3DEnabled else {
            return "provider disabled"
        }
        guard metrics.processedFrameCount > 0 else {
            return "waiting for frames"
        }
        return "frames \(metrics.processedFrameCount), \(metrics.depthWidth)x\(metrics.depthHeight)"
    }

    private var depthStrengthBinding: Binding<Double> {
        Binding(
            get: { Double(viewModel.depthStrength) },
            set: { viewModel.setDepthStrength(Float($0)) }
        )
    }

    private var depthDistanceBinding: Binding<Double> {
        Binding(
            get: { Double(viewModel.depthDistance) },
            set: { viewModel.setDepthDistance(Float($0)) }
        )
    }

    private var depthCurvatureBinding: Binding<Double> {
        Binding(
            get: { Double(viewModel.depthCurvature) },
            set: { viewModel.setDepthCurvature(Float($0)) }
        )
    }

    private var depthContrastBinding: Binding<Double> {
        Binding(
            get: { Double(viewModel.depthContrast) },
            set: { viewModel.setDepthContrast(Float($0)) }
        )
    }

    private var temporalSmoothingBinding: Binding<Double> {
        Binding(
            get: { Double(viewModel.temporalSmoothingFactor) },
            set: { viewModel.setTemporalSmoothingFactor(Float($0)) }
        )
    }

    private var immersiveScreenDistanceBinding: Binding<Double> {
        Binding(
            get: { Double(viewModel.immersiveScreenDistanceMeters) },
            set: { viewModel.setImmersiveScreenDistanceMeters(Float($0)) }
        )
    }

    private func doubleRange(_ range: ClosedRange<Float>) -> ClosedRange<Double> {
        Double(range.lowerBound) ... Double(range.upperBound)
    }

    private func formattedRange(minimum: Float?, maximum: Float?) -> String {
        guard let minimum, let maximum else {
            return "--"
        }
        return String(format: "%.3f...%.3f", Double(minimum), Double(maximum))
    }

    private func formattedDuration(_ duration: TimeInterval?) -> String {
        duration.map { String(format: "%.1f ms", $0 * 1_000) } ?? "--"
    }

    #if os(visionOS)
    private var immersiveTimingDashboard: some View {
        VStack(alignment: .leading, spacing: 4) {
            Text("Immersive timing")
                .font(.caption.weight(.semibold))
            ForEach(Array(viewModel.immersivePlaybackDiagnostics.summaryLines.enumerated()), id: \.offset) { _, line in
                Text(line)
                    .font(.caption2.monospacedDigit())
                    .foregroundStyle(.secondary)
            }
        }
        .padding(10)
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(.ultraThinMaterial, in: RoundedRectangle(cornerRadius: 10))
    }
    #endif

    @ViewBuilder
    private func performanceModeButton(_ mode: DA3VisionProPerformanceMode) -> some View {
        let isSelected = viewModel.performanceMode == mode
        if isSelected {
            Button(mode.displayName) {
                viewModel.setPerformanceMode(mode)
            }
            .buttonStyle(.borderedProminent)
        } else {
            Button(mode.displayName) {
                viewModel.setPerformanceMode(mode)
            }
            .buttonStyle(.bordered)
        }
    }

    @ViewBuilder
    private func presentationModeButton(_ mode: DA3PresentationMode) -> some View {
        let isSelected = viewModel.presentationMode == mode
        if isSelected {
            Button(mode.displayName) {
                viewModel.setPresentationMode(mode)
            }
            .buttonStyle(.borderedProminent)
        } else {
            Button(mode.displayName) {
                viewModel.setPresentationMode(mode)
            }
            .buttonStyle(.bordered)
        }
    }

    @ViewBuilder
    private func debugModeButton(_ mode: DA3DebugRenderMode) -> some View {
        let isSelected = viewModel.debugRenderMode == mode
        if isSelected {
            Button(mode.displayName) {
                viewModel.setDebugRenderMode(mode)
            }
            .buttonStyle(.borderedProminent)
        } else {
            Button(mode.displayName) {
                viewModel.setDebugRenderMode(mode)
            }
            .buttonStyle(.bordered)
        }
    }

    @ViewBuilder
    private func layoutButton(_ layout: Video2DTo3DOutputLayout) -> some View {
        let isSelected = viewModel.outputLayout == layout
        if isSelected {
            Button(outputLayoutTitle(layout)) {
                viewModel.setOutputLayout(layout)
            }
            .buttonStyle(.borderedProminent)
        } else {
            Button(outputLayoutTitle(layout)) {
                viewModel.setOutputLayout(layout)
            }
            .buttonStyle(.bordered)
        }
    }

    private func outputLayoutTitle(_ layout: Video2DTo3DOutputLayout) -> String {
        switch layout {
        case .selectedEye:
            return "Selected Eye"
        case .sideBySide:
            return "Side-by-Side"
        case .topAndBottom:
            return "Top/Bottom"
        }
    }

    private func eyeTitle(_ eye: StereoscopicVideoEye) -> String {
        switch eye {
        case .left:
            return "Left"
        case .right:
            return "Right"
        }
    }
}

private struct DA3SliderRow: View {
    let title: String
    @Binding var value: Double
    let range: ClosedRange<Double>
    var valueLabel: (Double) -> String = { value in
        value.formatted(.number.precision(.fractionLength(2)))
    }

    var body: some View {
        HStack(spacing: 14) {
            Text(title)
                .font(.subheadline)
                .frame(width: 140, alignment: .leading)

            Slider(value: $value, in: range)

            Text(valueLabel(value))
                .font(.subheadline.monospacedDigit())
                .foregroundStyle(.secondary)
                .frame(width: 56, alignment: .trailing)
        }
    }
}
#endif

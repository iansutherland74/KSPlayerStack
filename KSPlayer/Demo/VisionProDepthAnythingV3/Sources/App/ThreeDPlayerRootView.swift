#if os(visionOS) && canImport(CoreML) && canImport(Metal)
import Foundation
import KSPlayer
import SwiftUI

@MainActor
public struct ThreeDPlayerRootView: View {
    @StateObject private var viewModel: KSPlayer3DIntegrationViewModel

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
        .onFinish { _, error in
            viewModel.handleFinish(error: error)
        }
        .onDisappear {
            viewModel.stop()
        }
        .aspectRatio(16.0 / 9.0, contentMode: .fit)
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(.black, in: RoundedRectangle(cornerRadius: 28))
        .clipShape(RoundedRectangle(cornerRadius: 28))
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
                .disabled(viewModel.playerState.isPlaying)

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
                .disabled(!viewModel.playerState.isPlaying)

                Spacer(minLength: 0)
            }

            VStack(alignment: .leading, spacing: 10) {
                Toggle(
                    "2D to 3D",
                    isOn: Binding(
                        get: { viewModel.is2DTo3DEnabled },
                        set: { viewModel.set2DTo3DEnabled($0) }
                    )
                )
                .toggleStyle(.switch)
                .disabled(viewModel.isApplying2DTo3DMode)

                Text(viewModel.is2DTo3DEnabled ? "Conversion enabled. DA3 frames feed KSPlayer's depth-map renderer." : "Conversion disabled. Video is rendered normally.")
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

            debugStatusView
        }
        .padding(20)
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(.regularMaterial, in: RoundedRectangle(cornerRadius: 24))
    }

    private var da3StatusView: some View {
        let metrics = viewModel.metrics
        let fps = metrics.processingFPS > 0 ? String(format: "%.1f", metrics.processingFPS) : "--"
        let depthSize = metrics.depthWidth > 0 && metrics.depthHeight > 0
            ? "\(metrics.depthWidth)x\(metrics.depthHeight)"
            : "--"
        let frameDuration = metrics.lastFrameDuration.map { String(format: "%.0f ms", $0 * 1_000) } ?? "--"
        let depthOffset = metrics.depthFrameOffset.map { String(format: "%+.2fs", $0) } ?? "--"
        let smoothing = String(format: "%.2f", Double(metrics.temporalSmoothingFactor))
        let inferenceCap = metrics.maximumInferenceFPS > 0 ? String(format: "%.1f", metrics.maximumInferenceFPS) : "--"

        return VStack(alignment: .leading, spacing: 8) {
            HStack(alignment: .firstTextBaseline) {
                Text("DA3 Controls")
                    .font(.headline)

                Spacer(minLength: 12)

                Text(viewModel.is2DTo3DEnabled ? "Enabled" : "Disabled")
                    .font(.caption.weight(.semibold))
                    .foregroundStyle(viewModel.is2DTo3DEnabled ? Color.green : Color.secondary)
                    .padding(.horizontal, 10)
                    .padding(.vertical, 5)
                    .background(.thinMaterial, in: Capsule())
            }

            VStack(alignment: .leading, spacing: 4) {
                Text("Pipeline \(metrics.state.displayName)")
                    .font(.subheadline)

                Text("FPS \(fps)  Last \(frameDuration)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Frames \(metrics.processedFrameCount)  Dropped \(metrics.droppedFrameCount)  Throttled \(metrics.throttledFrameCount)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Depth \(depthSize)  Offset \(depthOffset)  Stale \(metrics.staleDepthFrameCount)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)

                Text("Cap \(inferenceCap) fps  Smoothing \(smoothing)")
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
            Text("Player state: \(viewModel.playerState.description)")
            if viewModel.playerState == .playedToTheEnd {
                Text("End of stream: press Replay/Restart, or enable Loop for continuous testing.")
            }
            Text("URL: \(viewModel.url.absoluteString)")
                .lineLimit(2)
            Text(
                String(
                    format: "Options: mode %@, strength %.2f, distance %.2f, curvature %.2f",
                    viewModel.is2DTo3DEnabled ? "depthMapPreferred" : "disabled",
                    Double(viewModel.depthStrength),
                    Double(viewModel.depthDistance),
                    Double(viewModel.depthCurvature)
                )
            )
            Text("Applied mode: \(viewModel.lastAppliedMode)  Loop: \(viewModel.isLoopEnabled ? "on" : "off")")
        }
        .font(.caption.monospacedDigit())
        .foregroundStyle(.secondary)
        .padding(12)
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(.thinMaterial, in: RoundedRectangle(cornerRadius: 14))
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

    private func doubleRange(_ range: ClosedRange<Float>) -> ClosedRange<Double> {
        Double(range.lowerBound) ... Double(range.upperBound)
    }
}

private struct DA3SliderRow: View {
    let title: String
    @Binding var value: Double
    let range: ClosedRange<Double>

    var body: some View {
        HStack(spacing: 14) {
            Text(title)
                .font(.subheadline)
                .frame(width: 140, alignment: .leading)

            Slider(value: $value, in: range)

            Text(value, format: .number.precision(.fractionLength(2)))
                .font(.subheadline.monospacedDigit())
                .foregroundStyle(.secondary)
                .frame(width: 48, alignment: .trailing)
        }
    }
}
#endif

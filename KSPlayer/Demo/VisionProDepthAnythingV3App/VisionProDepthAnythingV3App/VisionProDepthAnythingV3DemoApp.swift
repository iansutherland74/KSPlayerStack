import KSPlayer
import SwiftUI
#if os(visionOS)
import CompositorServices
import _CompositorServices_SwiftUI
import os
#endif

@main
struct VisionProDepthAnythingV3DemoApp: App {
    var body: some Scene {
        WindowGroup {
            ImmersiveLaunchGuard {
                VisionProDepthAnythingV3DemoRootView()
            }
        }

        #if os(visionOS)
        ImmersiveSpace(id: ImmersiveStereoSpace.id) {
            CompositorLayer(configuration: ImmersiveStereoCompositorConfiguration()) { layerRenderer in
                ImmersiveStereoCompositorLauncher.start(layerRenderer)
            }
        }
        .immersionStyle(
            selection: .constant(
                ImmersiveStereoDebugPresentation.prefersMixedImmersion ? .mixed : .full
            ),
            in: .mixed,
            .full
        )
        .upperLimbVisibility(.automatic)
        #endif
    }
}

#if os(visionOS)
/// Closes any immersive space restored from a previous session so launch stays in the window.
private struct ImmersiveLaunchGuard<Content: View>: View {
    @Environment(\.dismissImmersiveSpace) private var dismissImmersiveSpace
    @ViewBuilder var content: () -> Content

    var body: some View {
        content()
            .task {
                #if DEBUG
                if ImmersiveStereoDebugPresentation.enablesMagentaSolid {
                    ImmersiveStereoCompositorLauncher.setDebugSolidPresentationEnabled(true)
                }
                #endif
                guard !ImmersiveLaunchState.didDismissStaleSpace else {
                    return
                }
                ImmersiveLaunchState.markDidDismissStaleSpace()
                ImmersiveStereoSession.setUserRequestedActive(false)
                await dismissImmersiveSpace()
            }
    }
}

private enum ImmersiveLaunchState: Sendable {
    private static let dismissed = OSAllocatedUnfairLock(initialState: false)

    static var didDismissStaleSpace: Bool {
        dismissed.withLock { $0 }
    }

    static func markDidDismissStaleSpace() {
        dismissed.withLock { $0 = true }
    }
}
#endif

private struct VisionProDepthAnythingV3DemoRootView: View {
    private struct DemoStream: Identifiable, Hashable {
        let name: String
        let urlString: String

        var id: String { urlString }
    }

    private static let defaultStream = DemoStream(
        name: "Mux HLS",
        urlString: "https://test-streams.mux.dev/x36xhzz/x36xhzz.m3u8"
    )

    private static let demoStreams = [
        defaultStream,
        DemoStream(
            name: "Apple BipBop HLS",
            urlString: "https://devstreaming-cdn.apple.com/videos/streaming/examples/img_bipbop_adv_example_ts/master.m3u8"
        ),
    ]

    @State private var mediaURLString = Self.defaultStream.urlString
    @State private var committedURLString = Self.defaultStream.urlString
    @State private var loadErrorMessage: String?
    @State private var lastLoadActionMessage = "Ready"

    var body: some View {
        VStack(alignment: .leading, spacing: 16) {
            Text("KSPlayer Vision Pro DA3 Demo")
                .font(.largeTitle.bold())

            Text("A minimal installable visionOS host for the Depth Anything V3 2D-to-3D scaffold.")
                .foregroundStyle(.secondary)

            streamControls

            modelStatusView

            demoContent
                .frame(maxWidth: .infinity, maxHeight: .infinity)
        }
        .padding(32)
    }

    private var streamControls: some View {
        VStack(alignment: .leading, spacing: 10) {
            HStack(spacing: 10) {
                TextField("Media URL", text: $mediaURLString)
                    .textFieldStyle(.roundedBorder)
                    .textInputAutocapitalization(.never)

                Button {
                    loadTypedURL()
                } label: {
                    Label("Load", systemImage: "arrow.clockwise")
                }
                .buttonStyle(.borderedProminent)
            }

            HStack(spacing: 10) {
                ForEach(Self.demoStreams) { stream in
                    if stream.urlString == committedURLString {
                        Button(stream.name) {
                            load(stream)
                        }
                        .buttonStyle(.borderedProminent)
                    } else {
                        Button(stream.name) {
                            load(stream)
                        }
                        .buttonStyle(.bordered)
                    }
                }

                Text("Loaded: \(committedURLString)")
                    .font(.caption.monospacedDigit())
                    .foregroundStyle(.secondary)
                    .lineLimit(1)
                    .truncationMode(.middle)
            }

            Text("Load status: \(lastLoadActionMessage)")
                .font(.caption.monospacedDigit())
                .foregroundStyle(.secondary)

            if let loadErrorMessage {
                Text(loadErrorMessage)
                    .font(.caption)
                    .foregroundStyle(.red)
            }
        }
    }

    private var modelStatusView: some View {
        let message = Bundle.main.url(forResource: "da3-small", withExtension: "mlmodelc") == nil
            ? "DA3-SMALL compiled model resource was not found in the app bundle."
            : "DA3-SMALL compiled model resource is present in the app bundle."

        return Text(message)
            .font(.footnote)
            .foregroundStyle(.secondary)
    }

    @ViewBuilder
    private var demoContent: some View {
        #if os(visionOS) && canImport(CoreML) && canImport(Metal)
        if Bundle.main.url(forResource: "da3-small", withExtension: "mlmodelc") == nil {
            ContentUnavailableView(
                "DA3-SMALL Model Missing",
                systemImage: "cube.transparent",
                description: Text("The compiled da3-small.mlmodelc resource was not found in the app bundle.")
            )
        } else if let url = URL(string: committedURLString), let player = try? ThreeDPlayerRootView(url: url, options: playbackOptions()) {
            player
                .id(committedURLString)
                .clipShape(RoundedRectangle(cornerRadius: 24))
        } else {
            ContentUnavailableView(
                "Unable to Start Player",
                systemImage: "exclamationmark.triangle",
                description: Text("Enter a valid media URL and confirm the bundled da3-small.mlmodelc can be loaded on this device.")
            )
        }
        #else
        ContentUnavailableView {
            Label("Depth Anything V3 Runtime Unavailable", systemImage: "cube.transparent")
        } description: {
            Text("This demo requires visionOS, Core ML, and Metal.")
        }
        #endif
    }

    private func load(_ stream: DemoStream) {
        mediaURLString = stream.urlString
        lastLoadActionMessage = "Loading preset: \(stream.name)"
        committedURLString = stream.urlString
        loadErrorMessage = nil
    }

    private func loadTypedURL() {
        let trimmedURLString = mediaURLString.trimmingCharacters(in: .whitespacesAndNewlines)
        guard let url = URL(string: trimmedURLString), url.scheme != nil, url.host != nil else {
            lastLoadActionMessage = "Load rejected: invalid URL"
            loadErrorMessage = "Enter a fully qualified media URL before loading."
            return
        }
        mediaURLString = url.absoluteString
        lastLoadActionMessage = "Loading typed URL"
        committedURLString = url.absoluteString
        loadErrorMessage = nil
    }

    private func playbackOptions() -> KSOptions {
        let options = KSOptions()
        let defaults = DA3VisionProPerformanceMode.maximum
        options.prefersHighestVideoVariant = true
        options.videoAdaptable = false
        options.video2DTo3DMode = .depthMapPreferred
        options.video2DTo3DDepthStrength = defaults.depthStrength
        options.video2DTo3DDepthDistance = defaults.depthDistance
        options.video2DTo3DDepthCurvature = defaults.depthCurvature
        options.video2DTo3DDepthSmoothingFactor = defaults.temporalSmoothingFactor
        options.video2DTo3DOutputLayout = .selectedEye
        // Loop seeks to 0 and fights immersive timeline sync; enable explicitly when testing loop.
        options.isLoopPlay = ProcessInfo.processInfo.environment["DA3_LOOP_PLAY"] == "1"
        options.isSeamlessLoopEnabled = false
        return options
    }
}

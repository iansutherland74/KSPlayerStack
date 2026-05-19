import KSPlayer
import SwiftUI

@main
struct VisionProDepthAnythingV3DemoApp: App {
    var body: some Scene {
        WindowGroup {
            VisionProDepthAnythingV3DemoRootView()
        }
    }
}

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
        committedURLString = stream.urlString
        loadErrorMessage = nil
    }

    private func loadTypedURL() {
        let trimmedURLString = mediaURLString.trimmingCharacters(in: .whitespacesAndNewlines)
        guard let url = URL(string: trimmedURLString), url.scheme != nil, url.host != nil else {
            loadErrorMessage = "Enter a fully qualified media URL before loading."
            return
        }
        mediaURLString = url.absoluteString
        committedURLString = url.absoluteString
        loadErrorMessage = nil
    }

    private func playbackOptions() -> KSOptions {
        let options = KSOptions()
        options.video2DTo3DMode = .disabled
        options.videoDepthEstimationProvider = nil
        options.video2DTo3DDepthStrength = 0.18
        options.video2DTo3DDepthDistance = 0.65
        options.video2DTo3DDepthCurvature = 0.9
        options.isLoopPlay = true
        options.isSeamlessLoopEnabled = false
        return options
    }
}

#if os(visionOS) && DEPTH_ANYTHING_V3_GENERATED
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
        ZStack(alignment: .top) {
            KSVideoPlayer(
                coordinator: viewModel.coordinator,
                url: viewModel.url,
                options: viewModel.options
            )
            .onStateChanged { layer, state in
                viewModel.handleStateChanged(layer: layer, state: state)
            }
            .onDisappear {
                viewModel.stop()
            }
            .ignoresSafeArea()

            if let lastErrorMessage = viewModel.lastErrorMessage {
                Text(lastErrorMessage)
                    .font(.caption)
                    .foregroundStyle(.white)
                    .padding(12)
                    .background(.black.opacity(0.65), in: RoundedRectangle(cornerRadius: 12))
                    .padding()
            }
        }
    }
}
#endif

//
//  KSPictureInPictureController.swift
//  KSPlayer
//
//  Created by kintan on 2023/1/28.
//

import AVKit

enum PictureInPictureStartPolicy {
    static func unavailableReason(isSystemSupported: Bool, hasController: Bool, isPossible: Bool) -> String? {
        guard isSystemSupported else {
            return "system Picture in Picture support is unavailable"
        }
        guard hasController else {
            return "player does not expose a Picture in Picture controller"
        }
        guard isPossible else {
            return "Picture in Picture is not possible for the current player state"
        }
        return nil
    }
}

@available(tvOS 14.0, *)
public class KSPictureInPictureController: AVPictureInPictureController {
    nonisolated(unsafe) private static var pipController: KSPictureInPictureController?
    private var originalViewController: UIViewController?
    private var view: KSPlayerLayer?
    private weak var viewController: UIViewController?
    private weak var presentingViewController: UIViewController?
    #if canImport(UIKit)
    private weak var navigationController: UINavigationController?
    #endif

    func stop(restoreUserInterface: Bool) {
        stopPictureInPicture()
        delegate = nil
        if KSPictureInPictureController.pipController === self {
            KSPictureInPictureController.pipController = nil
        }
        let restoreViewController = viewController
        let restoreOriginalViewController = originalViewController
        let restorePresentingViewController = presentingViewController
        #if canImport(UIKit)
        let restoreNavigationController = navigationController
        #endif
        defer {
            originalViewController = nil
            viewController = nil
            presentingViewController = nil
            #if canImport(UIKit)
            navigationController = nil
            #endif
            view = nil
        }
        guard KSOptions.isPipPopViewController else {
            return
        }
        if restoreUserInterface {
            #if canImport(UIKit)
            runOnMainThread {
                guard let restoreViewController, let restoreOriginalViewController else { return }
                if let nav = restoreViewController as? UINavigationController,
                   nav.viewControllers.isEmpty || (nav.viewControllers.count == 1 && nav.viewControllers[0] != restoreOriginalViewController)
                {
                    nav.viewControllers = [restoreOriginalViewController]
                }
                if let restoreNavigationController {
                    var viewControllers = restoreNavigationController.viewControllers
                    if viewControllers.count > 1, let last = viewControllers.last, type(of: last) == type(of: restoreViewController) {
                        viewControllers[viewControllers.count - 1] = restoreViewController
                        restoreNavigationController.viewControllers = viewControllers
                    }
                    if viewControllers.firstIndex(of: restoreViewController) == nil {
                        // 新的swiftUI push之后。view会变成是emptyView。所以页面就空白了。
                        restoreNavigationController.pushViewController(restoreViewController, animated: true)
                    }
                } else {
                    restorePresentingViewController?.present(restoreOriginalViewController, animated: true)
                }
            }
            #endif
            view?.player.isMuted = false
            view?.play()
        }
    }

    @discardableResult
    func start(view: KSPlayerLayer) -> Bool {
        delegate = view
        if let reason = PictureInPictureStartPolicy.unavailableReason(
            isSystemSupported: AVPictureInPictureController.isPictureInPictureSupported(),
            hasController: true,
            isPossible: isPictureInPicturePossible
        ) {
            KSLog("[pip] start skipped: \(reason)")
            delegate = nil
            return false
        }
        self.view = view
        guard !isPictureInPictureActive else {
            return true
        }
        startPictureInPicture()
        guard KSOptions.isPipPopViewController else {
            #if canImport(UIKit)
            // 直接退到后台
            runOnMainThread {
                UIControl().sendAction(#selector(URLSessionTask.suspend), to: UIApplication.shared, for: nil)
            }
            #endif
            KSPictureInPictureController.pipController = self
            return true
        }
        #if canImport(UIKit)
        runOnMainThread { [weak self] in
            guard let self, let viewController = view.player.view?.viewController else { return }

            originalViewController = viewController
            if let navigationController = viewController.navigationController, navigationController.viewControllers.count == 1 {
                self.viewController = navigationController
            } else {
                self.viewController = viewController
            }
            navigationController = self.viewController?.navigationController
            if let pre = KSPictureInPictureController.pipController {
                view.player.isMuted = true
                pre.view?.isPipActive = false
            } else {
                if let navigationController {
                    navigationController.popViewController(animated: true)
                    #if os(iOS)
                    if navigationController.tabBarController != nil, navigationController.viewControllers.count == 1 {
                        DispatchQueue.main.async { [weak self] in
                            self?.navigationController?.setToolbarHidden(false, animated: true)
                        }
                    }
                    #endif
                } else {
                    presentingViewController = originalViewController?.presentingViewController
                    originalViewController?.dismiss(animated: true)
                }
            }
        }
        #endif
        KSPictureInPictureController.pipController = self
        return true
    }

    static func mute() {
        pipController?.view?.player.isMuted = true
    }
}

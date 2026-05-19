//
//  InAppCompactPlayer.swift
//  KSPlayer
//
//  Created by Cursor on 2026/5/18.
//

import Foundation
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

public enum KSPlayerCompactCorner: Sendable {
    case topLeading
    case topTrailing
    case bottomLeading
    case bottomTrailing
}

public struct KSPlayerCompactLayout: Sendable {
    public var corner: KSPlayerCompactCorner
    public var size: CGSize
    public var margin: CGFloat
    public var automaticallyHidesControls: Bool

    public init(corner: KSPlayerCompactCorner = .bottomTrailing, size: CGSize = CGSize(width: 320, height: 180), margin: CGFloat = 16, automaticallyHidesControls: Bool = true) {
        self.corner = corner
        self.size = size
        self.margin = margin
        self.automaticallyHidesControls = automaticallyHidesControls
    }

    public func resolvedSize(containerSize: CGSize) -> CGSize {
        let maxWidth = max(containerSize.width - margin * 2, 1)
        let maxHeight = max(containerSize.height - margin * 2, 1)
        return CGSize(width: min(max(size.width, 1), maxWidth), height: min(max(size.height, 1), maxHeight))
    }

    @MainActor
    func resolvedSize(in containerView: UIView) -> CGSize {
        #if canImport(UIKit)
        let safeAreaSize = containerView.safeAreaLayoutGuide.layoutFrame.size
        #else
        let safeAreaSize = containerView.safeAreaRect.size
        #endif
        let containerSize = safeAreaSize.width > 0 && safeAreaSize.height > 0 ? safeAreaSize : containerView.bounds.size
        return resolvedSize(containerSize: containerSize)
    }

    @MainActor
    func positionConstraints(for view: UIView, in containerView: UIView) -> [NSLayoutConstraint] {
        switch corner {
        case .topLeading:
            return [
                view.topAnchor.constraint(equalTo: containerView.safeTopAnchor, constant: margin),
                view.leadingAnchor.constraint(equalTo: containerView.safeLeadingAnchor, constant: margin),
            ]
        case .topTrailing:
            return [
                view.topAnchor.constraint(equalTo: containerView.safeTopAnchor, constant: margin),
                view.trailingAnchor.constraint(equalTo: containerView.safeTrailingAnchor, constant: -margin),
            ]
        case .bottomLeading:
            return [
                view.bottomAnchor.constraint(equalTo: containerView.safeBottomAnchor, constant: -margin),
                view.leadingAnchor.constraint(equalTo: containerView.safeLeadingAnchor, constant: margin),
            ]
        case .bottomTrailing:
            return [
                view.bottomAnchor.constraint(equalTo: containerView.safeBottomAnchor, constant: -margin),
                view.trailingAnchor.constraint(equalTo: containerView.safeTrailingAnchor, constant: -margin),
            ]
        }
    }

    @MainActor
    func constraints(for view: UIView, in containerView: UIView) -> (constraints: [NSLayoutConstraint], width: NSLayoutConstraint, height: NSLayoutConstraint) {
        let resolvedSize = resolvedSize(in: containerView)
        let width = view.widthAnchor.constraint(equalToConstant: resolvedSize.width)
        let height = view.heightAnchor.constraint(equalToConstant: resolvedSize.height)
        return ([width, height] + positionConstraints(for: view, in: containerView), width, height)
    }
}

struct KSPlayerCompactPresentation {
    weak var originalSuperview: UIView?
    weak var originalNextSibling: UIView?
    let originalConstraints: [NSLayoutConstraint]
    let originalFrame: CGRect
    let originalTranslatesAutoresizingMaskIntoConstraints: Bool
    let compactConstraints: [NSLayoutConstraint]
    let compactWidthConstraint: NSLayoutConstraint
    let compactHeightConstraint: NSLayoutConstraint
    let layout: KSPlayerCompactLayout
    let wasMaskShow: Bool
    let accessibilityLabel: String?
    let accessibilityHint: String?
    let wasAccessibilityElement: Bool
}

public struct KSPlayerResumeState: Equatable, Sendable {
    public let url: URL
    public let currentTime: TimeInterval
    public let duration: TimeInterval
    public let wasPlaying: Bool
    public let playbackRate: Float

    public init(url: URL, currentTime: TimeInterval, duration: TimeInterval, wasPlaying: Bool, playbackRate: Float = 1.0) {
        self.url = url
        self.currentTime = currentTime
        self.duration = duration
        self.wasPlaying = wasPlaying
        self.playbackRate = playbackRate
    }

    public var resumableTime: TimeInterval {
        Self.resumableTime(currentTime: currentTime, duration: duration)
    }

    public static func resumableTime(currentTime: TimeInterval, duration: TimeInterval) -> TimeInterval {
        guard currentTime.isFinite else {
            return 0
        }
        let currentTime = max(currentTime, 0)
        guard duration.isFinite, duration > 0 else {
            return currentTime
        }
        return min(currentTime, duration)
    }
}

public extension KSPlayerLayer {
    @MainActor
    var resumeState: KSPlayerResumeState {
        KSPlayerResumeState(
            url: url,
            currentTime: player.currentPlaybackTime,
            duration: player.duration,
            wasPlaying: state.isPlaying || player.isPlaying,
            playbackRate: player.playbackRate
        )
    }
}

public extension VideoPlayerView {
    @MainActor
    var isInAppCompactMode: Bool {
        compactPresentation != nil
    }

    @MainActor
    var resumeState: KSPlayerResumeState? {
        playerLayer?.resumeState
    }

    @MainActor
    @discardableResult
    func restorePlayback(from state: KSPlayerResumeState, options: KSOptions) -> Bool {
        if playerLayer?.url != state.url {
            set(url: state.url, options: options)
        }
        guard let playerLayer else {
            return false
        }
        playerLayer.player.playbackRate = state.playbackRate
        if state.resumableTime > 0 {
            playerLayer.seek(time: state.resumableTime, autoPlay: state.wasPlaying) { _ in }
        } else if state.wasPlaying {
            playerLayer.play()
        }
        return true
    }

    @MainActor
    @discardableResult
    func enterInAppCompactMode(in containerView: UIView? = nil, layout: KSPlayerCompactLayout = KSPlayerCompactLayout()) -> Bool {
        guard compactPresentation == nil else {
            return true
        }
        guard playerLayer?.isPipActive != true else {
            return false
        }
        guard let targetContainer = containerView ?? superview, targetContainer !== self, !targetContainer.isDescendant(of: self) else {
            return false
        }

        let originalConstraints = frameConstraints
        NSLayoutConstraint.deactivate(originalConstraints)
        let originalNextSibling = nextSibling
        let wasMaskShow = isMaskShow
        let originalTranslatesAutoresizingMaskIntoConstraints = translatesAutoresizingMaskIntoConstraints
        let originalFrame = frame
        let originalSuperview = superview
        let originalAccessibilityLabel = compactAccessibilityLabel
        let originalAccessibilityHint = compactAccessibilityHint
        let wasAccessibilityElement = compactIsAccessibilityElement

        targetContainer.addSubview(self)
        translatesAutoresizingMaskIntoConstraints = false
        let compactLayout = layout.constraints(for: self, in: targetContainer)
        let compactConstraints = compactLayout.constraints
        NSLayoutConstraint.activate(compactConstraints)

        compactPresentation = KSPlayerCompactPresentation(
            originalSuperview: originalSuperview,
            originalNextSibling: originalNextSibling,
            originalConstraints: originalConstraints,
            originalFrame: originalFrame,
            originalTranslatesAutoresizingMaskIntoConstraints: originalTranslatesAutoresizingMaskIntoConstraints,
            compactConstraints: compactConstraints,
            compactWidthConstraint: compactLayout.width,
            compactHeightConstraint: compactLayout.height,
            layout: layout,
            wasMaskShow: wasMaskShow,
            accessibilityLabel: originalAccessibilityLabel,
            accessibilityHint: originalAccessibilityHint,
            wasAccessibilityElement: wasAccessibilityElement
        )

        compactIsAccessibilityElement = true
        compactAccessibilityLabel = NSLocalizedString("Compact video player", comment: "")
        compactAccessibilityHint = NSLocalizedString("Double-tap to show or hide playback controls.", comment: "")
        updateInAppCompactLayout()

        if layout.automaticallyHidesControls {
            isMaskShow = false
        }
        return true
    }

    @MainActor
    func updateInAppCompactLayout() {
        guard let presentation = compactPresentation, let containerView = superview else {
            return
        }
        let resolvedSize = presentation.layout.resolvedSize(in: containerView)
        presentation.compactWidthConstraint.constant = resolvedSize.width
        presentation.compactHeightConstraint.constant = resolvedSize.height
    }

    @MainActor
    @discardableResult
    func exitInAppCompactMode() -> Bool {
        guard let presentation = compactPresentation else {
            return true
        }

        NSLayoutConstraint.deactivate(presentation.compactConstraints)
        if let originalSuperview = presentation.originalSuperview {
            restore(to: originalSuperview, before: presentation.originalNextSibling)
        } else {
            removeFromSuperview()
        }
        translatesAutoresizingMaskIntoConstraints = presentation.originalTranslatesAutoresizingMaskIntoConstraints
        if presentation.originalConstraints.isEmpty {
            frame = presentation.originalFrame
        } else {
            NSLayoutConstraint.activate(presentation.originalConstraints)
        }
        isMaskShow = presentation.wasMaskShow
        compactIsAccessibilityElement = presentation.wasAccessibilityElement
        compactAccessibilityLabel = presentation.accessibilityLabel
        compactAccessibilityHint = presentation.accessibilityHint
        compactPresentation = nil
        return true
    }
}

private extension VideoPlayerView {
    var compactAccessibilityLabel: String? {
        get {
            #if canImport(UIKit)
            accessibilityLabel
            #else
            accessibilityLabel()
            #endif
        }
        set {
            #if canImport(UIKit)
            accessibilityLabel = newValue
            #else
            setAccessibilityLabel(newValue)
            #endif
        }
    }

    var compactAccessibilityHint: String? {
        get {
            #if canImport(UIKit)
            accessibilityHint
            #else
            accessibilityHelp()
            #endif
        }
        set {
            #if canImport(UIKit)
            accessibilityHint = newValue
            #else
            setAccessibilityHelp(newValue)
            #endif
        }
    }

    var compactIsAccessibilityElement: Bool {
        get {
            #if canImport(UIKit)
            isAccessibilityElement
            #else
            isAccessibilityElement()
            #endif
        }
        set {
            #if canImport(UIKit)
            isAccessibilityElement = newValue
            #else
            setAccessibilityElement(newValue)
            #endif
        }
    }

    var nextSibling: UIView? {
        guard let superview, let index = superview.subviews.firstIndex(of: self) else {
            return nil
        }
        let nextIndex = superview.subviews.index(after: index)
        return nextIndex < superview.subviews.endIndex ? superview.subviews[nextIndex] : nil
    }

    func restore(to originalSuperview: UIView, before sibling: UIView?) {
        guard let sibling, sibling.superview === originalSuperview else {
            originalSuperview.addSubview(self)
            return
        }
        #if canImport(UIKit)
        originalSuperview.insertSubview(self, belowSubview: sibling)
        #else
        originalSuperview.addSubview(self, positioned: .below, relativeTo: sibling)
        #endif
    }
}

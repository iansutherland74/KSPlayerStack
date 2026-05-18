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

    func constraints(for view: UIView, in containerView: UIView) -> [NSLayoutConstraint] {
        let resolvedSize = resolvedSize(containerSize: containerView.bounds.size)
        var constraints = [
            view.widthAnchor.constraint(equalToConstant: resolvedSize.width),
            view.heightAnchor.constraint(equalToConstant: resolvedSize.height),
        ]

        switch corner {
        case .topLeading:
            constraints.append(view.topAnchor.constraint(equalTo: containerView.safeTopAnchor, constant: margin))
            constraints.append(view.leadingAnchor.constraint(equalTo: containerView.safeLeadingAnchor, constant: margin))
        case .topTrailing:
            constraints.append(view.topAnchor.constraint(equalTo: containerView.safeTopAnchor, constant: margin))
            constraints.append(view.trailingAnchor.constraint(equalTo: containerView.safeTrailingAnchor, constant: -margin))
        case .bottomLeading:
            constraints.append(view.bottomAnchor.constraint(equalTo: containerView.safeBottomAnchor, constant: -margin))
            constraints.append(view.leadingAnchor.constraint(equalTo: containerView.safeLeadingAnchor, constant: margin))
        case .bottomTrailing:
            constraints.append(view.bottomAnchor.constraint(equalTo: containerView.safeBottomAnchor, constant: -margin))
            constraints.append(view.trailingAnchor.constraint(equalTo: containerView.safeTrailingAnchor, constant: -margin))
        }
        return constraints
    }
}

struct KSPlayerCompactPresentation {
    weak var originalSuperview: UIView?
    let originalConstraints: [NSLayoutConstraint]
    let originalFrame: CGRect
    let originalTranslatesAutoresizingMaskIntoConstraints: Bool
    let compactConstraints: [NSLayoutConstraint]
    let wasMaskShow: Bool
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
    func enterInAppCompactMode(in containerView: UIView? = nil, layout: KSPlayerCompactLayout = KSPlayerCompactLayout()) -> Bool {
        guard compactPresentation == nil else {
            return true
        }
        guard let targetContainer = containerView ?? superview, targetContainer !== self, !targetContainer.isDescendant(of: self) else {
            return false
        }

        let originalConstraints = frameConstraints
        NSLayoutConstraint.deactivate(originalConstraints)
        let wasMaskShow = isMaskShow
        let originalTranslatesAutoresizingMaskIntoConstraints = translatesAutoresizingMaskIntoConstraints
        let originalFrame = frame
        let originalSuperview = superview

        targetContainer.addSubview(self)
        translatesAutoresizingMaskIntoConstraints = false
        let compactConstraints = layout.constraints(for: self, in: targetContainer)
        NSLayoutConstraint.activate(compactConstraints)

        compactPresentation = KSPlayerCompactPresentation(
            originalSuperview: originalSuperview,
            originalConstraints: originalConstraints,
            originalFrame: originalFrame,
            originalTranslatesAutoresizingMaskIntoConstraints: originalTranslatesAutoresizingMaskIntoConstraints,
            compactConstraints: compactConstraints,
            wasMaskShow: wasMaskShow
        )

        if layout.automaticallyHidesControls {
            isMaskShow = false
        }
        return true
    }

    @MainActor
    @discardableResult
    func exitInAppCompactMode() -> Bool {
        guard let presentation = compactPresentation else {
            return true
        }

        NSLayoutConstraint.deactivate(presentation.compactConstraints)
        if let originalSuperview = presentation.originalSuperview {
            originalSuperview.addSubview(self)
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
        compactPresentation = nil
        return true
    }
}

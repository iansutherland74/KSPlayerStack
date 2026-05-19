//
//  ProgressPreviewView.swift
//  KSPlayer
//
//  Created by Cursor on 2026/5/18.
//

#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

enum ProgressPreviewResolver {
    static func shouldGenerateThumbnails(
        url: URL,
        mode: ProgressPreviewThumbnailMode,
        duration: TimeInterval,
        isSeekable: Bool,
        isLiveStream: Bool
    ) -> Bool {
        guard duration.isFinite, duration > 0, isSeekable, !isLiveStream else {
            return false
        }
        switch mode {
        case .disabled:
            return false
        case .localOnly:
            return url.isFileURL
        case .always:
            return true
        }
    }

    static func shouldResetThumbnailState(currentURL: URL?, newURL: URL) -> Bool {
        currentURL != newURL
    }

    static func clampedFraction(time: TimeInterval, totalTime: TimeInterval) -> CGFloat {
        guard time.isFinite, totalTime.isFinite, totalTime > 0 else {
            return 0
        }
        return min(max(CGFloat(time / totalTime), 0), 1)
    }

    static func previewCenterX(time: TimeInterval, totalTime: TimeInterval, sliderWidth: CGFloat, previewWidth: CGFloat) -> CGFloat {
        guard sliderWidth > 0 else { return 0 }
        let rawCenter = clampedFraction(time: time, totalTime: totalTime) * sliderWidth
        let halfWidth = min(previewWidth / 2, sliderWidth / 2)
        return min(max(rawCenter, halfWidth), sliderWidth - halfWidth)
    }

    static func nearestThumbnailIndex(times: [TimeInterval], target: TimeInterval) -> Int? {
        guard target.isFinite, !times.isEmpty else {
            return nil
        }
        return times.enumerated().min { lhs, rhs in
            abs(lhs.element - target) < abs(rhs.element - target)
        }?.offset
    }
}

final class ProgressPreviewView: UIView {
    static let preferredWidth: CGFloat = 150
    static let preferredHeight: CGFloat = 112

    private let imageView = UIImageView()
    private let placeholderLabel = UILabel()
    private let timeLabel = UILabel()

    override init(frame: CGRect) {
        super.init(frame: frame)
        setup()
    }

    @available(*, unavailable)
    required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    func set(timeText: String, image: UIImage?, isLoading: Bool) {
        timeLabel.text = timeText
        imageView.image = image
        imageView.isHidden = image == nil
        let placeholderText = isLoading ? NSLocalizedString("Loading preview", comment: "") : NSLocalizedString("Preview unavailable", comment: "")
        placeholderLabel.text = placeholderText
        placeholderLabel.isHidden = image != nil
        #if canImport(UIKit)
        accessibilityLabel = image == nil ? "\(timeText), \(placeholderText)" : timeText
        #else
        setAccessibilityLabel(image == nil ? "\(timeText), \(placeholderText)" : timeText)
        #endif
    }

    private func setup() {
        #if canImport(UIKit)
        backgroundColor = UIColor.black.withAlphaComponent(0.78)
        #else
        backingLayer?.backgroundColor = UIColor.black.withAlphaComponent(0.78).cgColor
        #endif
        cornerRadius = 8
        clipsToBounds = true
        isHidden = true

        imageView.contentMode = .scaleAspectFill
        imageView.clipsToBounds = true
        imageView.backgroundColor = UIColor.black.withAlphaComponent(0.25)
        imageView.isHidden = true

        placeholderLabel.font = UIFont.systemFont(ofSize: 12, weight: .medium)
        placeholderLabel.textColor = UIColor.white.withAlphaComponent(0.72)
        placeholderLabel.textAlignment = .center
        placeholderLabel.numberOfLines = 2
        placeholderLabel.isHidden = true

        timeLabel.font = UIFont.monospacedDigitSystemFont(ofSize: 13, weight: .medium)
        timeLabel.textColor = .white
        timeLabel.textAlignment = .center

        addSubview(imageView)
        addSubview(placeholderLabel)
        addSubview(timeLabel)
        imageView.translatesAutoresizingMaskIntoConstraints = false
        placeholderLabel.translatesAutoresizingMaskIntoConstraints = false
        timeLabel.translatesAutoresizingMaskIntoConstraints = false
        #if canImport(UIKit)
        isAccessibilityElement = true
        #else
        setAccessibilityElement(true)
        #endif

        NSLayoutConstraint.activate([
            imageView.topAnchor.constraint(equalTo: topAnchor, constant: 8),
            imageView.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 8),
            imageView.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -8),
            imageView.heightAnchor.constraint(equalToConstant: 72),
            placeholderLabel.topAnchor.constraint(equalTo: imageView.topAnchor),
            placeholderLabel.leadingAnchor.constraint(equalTo: imageView.leadingAnchor, constant: 8),
            placeholderLabel.trailingAnchor.constraint(equalTo: imageView.trailingAnchor, constant: -8),
            placeholderLabel.bottomAnchor.constraint(equalTo: imageView.bottomAnchor),
            timeLabel.topAnchor.constraint(equalTo: imageView.bottomAnchor, constant: 6),
            timeLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 8),
            timeLabel.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -8),
            timeLabel.bottomAnchor.constraint(equalTo: bottomAnchor, constant: -8),
        ])
    }
}

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
    private let timeLabel = UILabel()

    override init(frame: CGRect) {
        super.init(frame: frame)
        setup()
    }

    @available(*, unavailable)
    required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    func set(timeText: String, image: UIImage?) {
        timeLabel.text = timeText
        imageView.image = image
        imageView.isHidden = image == nil
    }

    private func setup() {
        backgroundColor = UIColor.black.withAlphaComponent(0.78)
        cornerRadius = 8
        clipsToBounds = true
        isHidden = true

        imageView.contentMode = .scaleAspectFill
        imageView.clipsToBounds = true
        imageView.backgroundColor = UIColor.black.withAlphaComponent(0.25)
        imageView.isHidden = true

        timeLabel.font = UIFont.monospacedDigitSystemFont(ofSize: 13, weight: .medium)
        timeLabel.textColor = .white
        timeLabel.textAlignment = .center

        addSubview(imageView)
        addSubview(timeLabel)
        imageView.translatesAutoresizingMaskIntoConstraints = false
        timeLabel.translatesAutoresizingMaskIntoConstraints = false

        NSLayoutConstraint.activate([
            imageView.topAnchor.constraint(equalTo: topAnchor, constant: 8),
            imageView.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 8),
            imageView.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -8),
            imageView.heightAnchor.constraint(equalToConstant: 72),
            timeLabel.topAnchor.constraint(equalTo: imageView.bottomAnchor, constant: 6),
            timeLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 8),
            timeLabel.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -8),
            timeLabel.bottomAnchor.constraint(equalTo: bottomAnchor, constant: -8),
        ])
    }
}

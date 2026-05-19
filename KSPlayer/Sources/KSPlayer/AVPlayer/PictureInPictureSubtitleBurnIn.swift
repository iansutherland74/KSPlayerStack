import AVFoundation
import CoreGraphics
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

enum PictureInPictureSubtitleRole: Equatable {
    case primary
    case secondary
}

struct PictureInPictureSubtitleCue {
    let role: PictureInPictureSubtitleRole
    let attributedText: NSAttributedString?
    let image: UIImage?
    let imageRect: CGRect?
    let imageCanvasSize: CGSize?

    static func text(_ attributedText: NSAttributedString, role: PictureInPictureSubtitleRole) -> PictureInPictureSubtitleCue {
        PictureInPictureSubtitleCue(role: role, attributedText: attributedText, image: nil, imageRect: nil, imageCanvasSize: nil)
    }

    static func image(_ image: UIImage, role: PictureInPictureSubtitleRole, imageRect: CGRect?, imageCanvasSize: CGSize?) -> PictureInPictureSubtitleCue {
        PictureInPictureSubtitleCue(role: role, attributedText: nil, image: image, imageRect: imageRect, imageCanvasSize: imageCanvasSize)
    }
}

struct PictureInPictureSubtitleSnapshot {
    let cues: [PictureInPictureSubtitleCue]

    var isEmpty: Bool {
        cues.isEmpty
    }

    static func make(
        primaryParts: [SubtitlePart],
        primaryTime: TimeInterval,
        secondaryParts: [SubtitlePart],
        secondaryTime: TimeInterval,
        activeWordAttributes: [NSAttributedString.Key: Any]
    ) -> PictureInPictureSubtitleSnapshot? {
        let cues = [
            cue(parts: primaryParts, role: .primary, time: primaryTime, activeWordAttributes: activeWordAttributes),
            cue(parts: secondaryParts, role: .secondary, time: secondaryTime, activeWordAttributes: activeWordAttributes),
        ].compactMap { $0 }
        guard !cues.isEmpty else {
            return nil
        }
        return PictureInPictureSubtitleSnapshot(cues: cues)
    }

    private static func cue(
        parts: [SubtitlePart],
        role: PictureInPictureSubtitleRole,
        time: TimeInterval,
        activeWordAttributes: [NSAttributedString.Key: Any]
    ) -> PictureInPictureSubtitleCue? {
        guard let part = parts.first else {
            return nil
        }
        if let image = part.image {
            return .image(
                image,
                role: role,
                imageRect: part.imageRect ?? CGRect(origin: part.origin, size: image.size),
                imageCanvasSize: part.imageCanvasSize
            )
        }
        guard let attributedText = part.attributedText(at: time, activeWordAttributes: activeWordAttributes),
              !attributedText.string.isEmpty
        else {
            return nil
        }
        return .text(attributedText, role: role)
    }
}

struct PictureInPictureSubtitleRenderCommand {
    enum Content {
        case text(NSAttributedString)
        case image(UIImage)
    }

    let content: Content
    let rect: CGRect
    let backgroundRect: CGRect?
}

enum PictureInPictureSubtitleComposer {
    static let unsupportedDynamicRangeReason = "PiP subtitle burn-in is limited to SDR sample-buffer frames in this renderer pass."
    static let unsupportedPixelBufferReason = "PiP subtitle burn-in needs a CoreGraphics-readable CVPixelBuffer frame."

    static func layout(snapshot: PictureInPictureSubtitleSnapshot, videoSize: CGSize) -> [PictureInPictureSubtitleRenderCommand] {
        guard videoSize.width > 0, videoSize.height > 0 else {
            return []
        }
        let bounds = CGRect(origin: .zero, size: videoSize).insetBy(dx: 5, dy: 5)
        var commands = imageCommands(snapshot: snapshot, bounds: bounds)
        commands.append(contentsOf: textCommands(snapshot: snapshot, bounds: bounds))
        return commands
    }

    static func compositedPixelBuffer(source: CVPixelBuffer, snapshot: PictureInPictureSubtitleSnapshot) -> CVPixelBuffer? {
        let videoSize = CGSize(width: CVPixelBufferGetWidth(source), height: CVPixelBufferGetHeight(source))
        let commands = layout(snapshot: snapshot, videoSize: videoSize)
        guard !commands.isEmpty, let sourceImage = source.cgImage() else {
            return nil
        }

        var output: CVPixelBuffer?
        let attributes: [CFString: Any] = [
            kCVPixelBufferCGImageCompatibilityKey: true,
            kCVPixelBufferCGBitmapContextCompatibilityKey: true,
            kCVPixelBufferIOSurfacePropertiesKey: [:],
        ]
        let status = CVPixelBufferCreate(
            kCFAllocatorDefault,
            source.width,
            source.height,
            kCVPixelFormatType_32BGRA,
            attributes as CFDictionary,
            &output
        )
        guard status == kCVReturnSuccess, let output else {
            return nil
        }

        CVPixelBufferLockBaseAddress(output, [])
        defer {
            CVPixelBufferUnlockBaseAddress(output, [])
        }
        guard let baseAddress = CVPixelBufferGetBaseAddress(output) else {
            return nil
        }
        let colorSpace = source.colorspace ?? CGColorSpaceCreateDeviceRGB()
        let bitmapInfo = CGBitmapInfo.byteOrder32Little.rawValue | CGImageAlphaInfo.premultipliedFirst.rawValue
        guard let context = CGContext(
            data: baseAddress,
            width: source.width,
            height: source.height,
            bitsPerComponent: 8,
            bytesPerRow: CVPixelBufferGetBytesPerRow(output),
            space: colorSpace,
            bitmapInfo: bitmapInfo
        ) else {
            return nil
        }
        draw(sourceImage: sourceImage, commands: commands, videoSize: videoSize, in: context)
        copyColorAttachments(from: source, to: output)
        return output
    }

    private static func imageCommands(snapshot: PictureInPictureSubtitleSnapshot, bounds: CGRect) -> [PictureInPictureSubtitleRenderCommand] {
        snapshot.cues.compactMap { cue in
            guard let image = cue.image else {
                return nil
            }
            let rect = imageFrame(for: cue, image: image, bounds: bounds)
            return PictureInPictureSubtitleRenderCommand(content: .image(image), rect: rect, backgroundRect: nil)
        }
    }

    private static func textCommands(snapshot: PictureInPictureSubtitleSnapshot, bounds: CGRect) -> [PictureInPictureSubtitleRenderCommand] {
        let textCues = snapshot.cues.filter { $0.attributedText != nil }.sorted { lhs, rhs in
            if lhs.role == rhs.role {
                return false
            }
            return lhs.role == .primary
        }
        let textHorizontalInset = CGFloat(10)
        let textVerticalInset = CGFloat(2)
        let spacing = CGFloat(5)
        let maxTextWidth = max(bounds.width - textHorizontalInset * 2, 1)
        var bottom = bounds.maxY
        return textCues.compactMap { cue in
            guard let text = cue.attributedText else {
                return nil
            }
            let measured = text.boundingRect(
                with: CGSize(width: maxTextWidth, height: CGFloat.greatestFiniteMagnitude),
                options: [.usesLineFragmentOrigin, .usesFontLeading],
                context: nil
            ).integral
            guard measured.width > 0, measured.height > 0 else {
                return nil
            }
            let textSize = CGSize(width: min(measured.width, maxTextWidth), height: measured.height)
            let backgroundSize = CGSize(width: textSize.width + textHorizontalInset * 2, height: textSize.height + textVerticalInset * 2)
            let backgroundRect = CGRect(
                x: bounds.midX - backgroundSize.width / 2,
                y: bottom - backgroundSize.height,
                width: backgroundSize.width,
                height: backgroundSize.height
            )
            bottom = backgroundRect.minY - spacing
            let textRect = backgroundRect.insetBy(dx: textHorizontalInset, dy: textVerticalInset)
            return PictureInPictureSubtitleRenderCommand(content: .text(text), rect: textRect, backgroundRect: backgroundRect)
        }
    }

    private static func imageFrame(for cue: PictureInPictureSubtitleCue, image: UIImage, bounds: CGRect) -> CGRect {
        var rect = cue.imageRect ?? CGRect(origin: .zero, size: image.size)
        if rect.width <= 0 || rect.height <= 0 {
            rect.size = image.size
        }
        if let canvasSize = cue.imageCanvasSize, canvasSize.width > 0, canvasSize.height > 0 {
            let scale = min(bounds.width / canvasSize.width, bounds.height / canvasSize.height)
            let canvasSize = CGSize(width: canvasSize.width * scale, height: canvasSize.height * scale)
            let canvasOrigin = CGPoint(
                x: bounds.minX + (bounds.width - canvasSize.width) / 2,
                y: bounds.minY + (bounds.height - canvasSize.height) / 2
            )
            return CGRect(
                x: canvasOrigin.x + rect.minX * scale,
                y: canvasOrigin.y + rect.minY * scale,
                width: rect.width * scale,
                height: rect.height * scale
            )
        }
        let scale = min(min(bounds.width / rect.width, bounds.height / rect.height), 1)
        let size = CGSize(width: rect.width * scale, height: rect.height * scale)
        return CGRect(
            x: bounds.minX + (bounds.width - size.width) / 2,
            y: bounds.maxY - size.height,
            width: size.width,
            height: size.height
        )
    }

    private static func draw(
        sourceImage: CGImage,
        commands: [PictureInPictureSubtitleRenderCommand],
        videoSize: CGSize,
        in context: CGContext
    ) {
        let bounds = CGRect(origin: .zero, size: videoSize)
        #if canImport(UIKit)
        UIGraphicsPushContext(context)
        UIImage(cgImage: sourceImage).draw(in: bounds)
        for command in commands {
            draw(command: command)
        }
        UIGraphicsPopContext()
        #else
        NSGraphicsContext.saveGraphicsState()
        NSGraphicsContext.current = NSGraphicsContext(cgContext: context, flipped: false)
        UIImage(cgImage: sourceImage).draw(in: bounds)
        for command in commands {
            draw(command: command)
        }
        NSGraphicsContext.restoreGraphicsState()
        #endif
    }

    private static func draw(command: PictureInPictureSubtitleRenderCommand) {
        if let backgroundRect = command.backgroundRect {
            UIColor.black.withAlphaComponent(0.35).setFill()
            #if canImport(UIKit)
            UIBezierPath(roundedRect: backgroundRect, cornerRadius: 2).fill()
            #else
            NSBezierPath(roundedRect: backgroundRect, xRadius: 2, yRadius: 2).fill()
            #endif
        }
        switch command.content {
        case let .text(text):
            text.draw(with: command.rect, options: [.usesLineFragmentOrigin, .usesFontLeading], context: nil)
        case let .image(image):
            image.draw(in: command.rect)
        }
    }

    private static func copyColorAttachments(from source: CVPixelBuffer, to output: CVPixelBuffer) {
        let keys: [CFString] = [
            kCVImageBufferColorPrimariesKey,
            kCVImageBufferTransferFunctionKey,
            kCVImageBufferCGColorSpaceKey,
            kCVImageBufferGammaLevelKey,
            kCVImageBufferPixelAspectRatioKey,
        ]
        for key in keys {
            if let value = CVBufferCopyAttachment(source, key, nil) {
                CVBufferSetAttachment(output, key, value, .shouldPropagate)
            }
        }
    }
}

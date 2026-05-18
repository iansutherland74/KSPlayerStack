import CoreGraphics
import Foundation
#if canImport(UIKit)
import UIKit
#else
import AppKit
#endif

#if canImport(libass)
import libass

final class LibassSubtitleRenderer {
    private let library: OpaquePointer
    private let renderer: OpaquePointer
    private let track: UnsafeMutablePointer<ASS_Track>
    private let canvasSize: CGSize

    init?(subtitleHeader: Data?, canvasSize: CGSize, fontDirectoryURL: URL?) {
        guard canvasSize.width > 0, canvasSize.height > 0,
              let library = ass_library_init(),
              let renderer = ass_renderer_init(library),
              let track = ass_new_track(library)
        else {
            return nil
        }
        self.library = library
        self.renderer = renderer
        self.track = track
        self.canvasSize = canvasSize

        if let fontDirectoryURL {
            ass_set_fonts_dir(library, fontDirectoryURL.path)
        }
        ass_set_frame_size(renderer, Int32(canvasSize.width), Int32(canvasSize.height))
        ass_set_storage_size(renderer, Int32(canvasSize.width), Int32(canvasSize.height))
        ass_set_fonts(renderer, nil, "sans-serif", 1, nil, 1)

        if let subtitleHeader, !subtitleHeader.isEmpty {
            subtitleHeader.withUnsafeBytes { buffer in
                guard let baseAddress = buffer.bindMemory(to: CChar.self).baseAddress else {
                    return
                }
                ass_process_codec_private(track, baseAddress, Int32(buffer.count))
            }
        }
    }

    deinit {
        ass_free_track(track)
        ass_renderer_done(renderer)
        ass_library_done(library)
    }

    func flush() {
        ass_flush_events(track)
    }

    func render(packetData: Data, start: TimeInterval, duration: TimeInterval) -> SubtitlePart? {
        guard !packetData.isEmpty else {
            return nil
        }
        let startMilliseconds = Int64((start * 1000).rounded())
        let durationMilliseconds = duration > 0 ? Int64((duration * 1000).rounded()) : 0
        packetData.withUnsafeBytes { buffer in
            guard let baseAddress = buffer.bindMemory(to: CChar.self).baseAddress else {
                return
            }
            ass_process_chunk(track, baseAddress, Int32(buffer.count), startMilliseconds, durationMilliseconds)
        }

        var detectChange = Int32(0)
        guard let imageList = ass_render_frame(renderer, track, startMilliseconds, &detectChange),
              let image = makeImage(from: imageList)
        else {
            return nil
        }

        let part = SubtitlePart(start, duration > 0 ? start + duration : .infinity, attributedString: nil)
        part.image = image
        part.origin = .zero
        part.imageRect = CGRect(origin: .zero, size: canvasSize)
        part.imageCanvasSize = canvasSize
        return part
    }

    private func makeImage(from imageList: UnsafeMutablePointer<ASS_Image>) -> UIImage? {
        let width = Int(canvasSize.width.rounded())
        let height = Int(canvasSize.height.rounded())
        guard width > 0, height > 0 else {
            return nil
        }

        var pixels = [UInt8](repeating: 0, count: width * height * 4)
        var renderedAnyPixel = false
        var current: UnsafeMutablePointer<ASS_Image>? = imageList
        while let imagePointer = current {
            let image = imagePointer.pointee
            blend(image: image, into: &pixels, canvasWidth: width, canvasHeight: height, renderedAnyPixel: &renderedAnyPixel)
            current = image.next
        }

        guard renderedAnyPixel else {
            return nil
        }
        let data = Data(pixels)
        guard let provider = CGDataProvider(data: data as CFData),
              let cgImage = CGImage(
                  width: width,
                  height: height,
                  bitsPerComponent: 8,
                  bitsPerPixel: 32,
                  bytesPerRow: width * 4,
                  space: CGColorSpaceCreateDeviceRGB(),
                  bitmapInfo: CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue),
                  provider: provider,
                  decode: nil,
                  shouldInterpolate: false,
                  intent: .defaultIntent
              )
        else {
            return nil
        }

        #if canImport(UIKit)
        return UIImage(cgImage: cgImage)
        #else
        return UIImage(cgImage: cgImage, size: canvasSize)
        #endif
    }

    private func blend(image: ASS_Image, into pixels: inout [UInt8], canvasWidth: Int, canvasHeight: Int, renderedAnyPixel: inout Bool) {
        guard image.w > 0, image.h > 0, let bitmap = image.bitmap else {
            return
        }

        let color = image.color
        let red = Int((color >> 24) & 0xff)
        let green = Int((color >> 16) & 0xff)
        let blue = Int((color >> 8) & 0xff)
        let colorAlpha = 255 - Int(color & 0xff)
        let minX = max(0, Int(image.dst_x))
        let minY = max(0, Int(image.dst_y))
        let maxX = min(canvasWidth, Int(image.dst_x + image.w))
        let maxY = min(canvasHeight, Int(image.dst_y + image.h))

        guard minX < maxX, minY < maxY else {
            return
        }

        for y in minY ..< maxY {
            let sourceY = y - Int(image.dst_y)
            let row = bitmap.advanced(by: sourceY * Int(image.stride))
            for x in minX ..< maxX {
                let sourceX = x - Int(image.dst_x)
                let coverage = Int(row[sourceX])
                let alpha = colorAlpha * coverage / 255
                guard alpha > 0 else {
                    continue
                }
                renderedAnyPixel = true
                let offset = (y * canvasWidth + x) * 4
                let inverseAlpha = 255 - alpha
                pixels[offset] = UInt8(min(255, red * alpha / 255 + Int(pixels[offset]) * inverseAlpha / 255))
                pixels[offset + 1] = UInt8(min(255, green * alpha / 255 + Int(pixels[offset + 1]) * inverseAlpha / 255))
                pixels[offset + 2] = UInt8(min(255, blue * alpha / 255 + Int(pixels[offset + 2]) * inverseAlpha / 255))
                pixels[offset + 3] = UInt8(min(255, alpha + Int(pixels[offset + 3]) * inverseAlpha / 255))
            }
        }
    }
}
#endif

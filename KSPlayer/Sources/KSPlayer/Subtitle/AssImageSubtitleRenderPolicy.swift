import CoreGraphics
import Foundation
import Libavcodec

enum AssImageSubtitleRenderPolicy {
    static func canRender(codecID: AVCodecID) -> Bool {
        codecID == AV_CODEC_ID_ASS || codecID == AV_CODEC_ID_SSA
    }

    static func canvasSize(codecWidth: Int32, codecHeight: Int32, subtitleHeader: String?, fallback: CGSize = CGSize(width: 1920, height: 1080)) -> CGSize {
        if codecWidth > 0, codecHeight > 0 {
            return CGSize(width: Int(codecWidth), height: Int(codecHeight))
        }
        if let playRes = playResSize(from: subtitleHeader), playRes.width > 0, playRes.height > 0 {
            return playRes
        }
        return fallback
    }

    private static func playResSize(from subtitleHeader: String?) -> CGSize? {
        guard let subtitleHeader else {
            return nil
        }
        var width: Double?
        var height: Double?
        for line in subtitleHeader.components(separatedBy: .newlines) {
            let parts = line.split(separator: ":", maxSplits: 1).map {
                $0.trimmingCharacters(in: .whitespacesAndNewlines)
            }
            guard parts.count == 2 else {
                continue
            }
            switch parts[0].lowercased() {
            case "playresx":
                width = Double(parts[1])
            case "playresy":
                height = Double(parts[1])
            default:
                continue
            }
        }
        guard let width, let height else {
            return nil
        }
        return CGSize(width: width, height: height)
    }
}

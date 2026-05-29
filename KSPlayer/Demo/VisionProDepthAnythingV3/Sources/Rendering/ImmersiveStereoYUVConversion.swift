#if os(visionOS) && canImport(Metal)
import Accelerate
import CoreVideo
import KSPlayer
@preconcurrency import Metal
import simd

/// NV12 fragment color conversion matching `MetalRender.setFragmentBuffer` (kvImage BGR matrices).
enum ImmersiveStereoYUVConversion {
    private static let device = MTLCreateSystemDefaultDevice()!

    private static let colorMatrix601 = vImage_YpCbCrToARGBMatrix(Kr: 0.299, Kb: 0.114)
    private static let colorMatrix709 = vImage_YpCbCrToARGBMatrix(Kr: 0.2126, Kb: 0.0722)

    private static let colorConversion601VideoRangeMatrixBuffer: MTLBuffer? =
        colorMatrix601.videoRange.metalBuffer(device: device, label: "ImmersiveYUV601VideoRange")
    private static let colorConversion601FullRangeMatrixBuffer: MTLBuffer? =
        colorMatrix601.metalBuffer(device: device, label: "ImmersiveYUV601FullRange")
    private static let colorConversion709VideoRangeMatrixBuffer: MTLBuffer? =
        colorMatrix709.videoRange.metalBuffer(device: device, label: "ImmersiveYUV709VideoRange")
    private static let colorConversion709FullRangeMatrixBuffer: MTLBuffer? =
        colorMatrix709.metalBuffer(device: device, label: "ImmersiveYUV709FullRange")
    private static let colorConversionSMPTE240MVideoRangeMatrixBuffer: MTLBuffer? =
        kvImage_YpCbCrToARGBMatrix_SMPTE_240M_1995.videoRange.metalBuffer(device: device, label: "ImmersiveYUVSMPTE240MVideoRange")
    private static let colorConversionSMPTE240MFullRangeMatrixBuffer: MTLBuffer? =
        kvImage_YpCbCrToARGBMatrix_SMPTE_240M_1995.metalBuffer(device: device, label: "ImmersiveYUVSMPTE240MFullRange")
    private static let colorConversion2020VideoRangeMatrixBuffer: MTLBuffer? =
        kvImage_YpCbCrToARGBMatrix_ITU_R_2020.videoRange.metalBuffer(device: device, label: "ImmersiveYUV2020VideoRange")
    private static let colorConversion2020FullRangeMatrixBuffer: MTLBuffer? =
        kvImage_YpCbCrToARGBMatrix_ITU_R_2020.metalBuffer(device: device, label: "ImmersiveYUV2020FullRange")

    private static let colorOffsetVideoRangeMatrixBuffer: MTLBuffer? = {
        var offset = SIMD3<Float>(-16.0 / 255.0, -128.0 / 255.0, -128.0 / 255.0)
        let buffer = device.makeBuffer(bytes: &offset, length: MemoryLayout<SIMD3<Float>>.size)
        buffer?.label = "ImmersiveYUVColorOffsetVideoRange"
        return buffer
    }()

    private static let colorOffsetFullRangeMatrixBuffer: MTLBuffer? = {
        var offset = SIMD3<Float>(0, -128.0 / 255.0, -128.0 / 255.0)
        let buffer = device.makeBuffer(bytes: &offset, length: MemoryLayout<SIMD3<Float>>.size)
        buffer?.label = "ImmersiveYUVColorOffsetFullRange"
        return buffer
    }()

    private static let leftShiftMatrixBuffer: MTLBuffer? = {
        var leftShift = SIMD3<UInt8>(1, 1, 1)
        let buffer = device.makeBuffer(bytes: &leftShift, length: MemoryLayout<SIMD3<UInt8>>.size)
        buffer?.label = "ImmersiveYUVLeftShift"
        return buffer
    }()

    private static let leftShiftSixMatrixBuffer: MTLBuffer? = {
        var leftShift = SIMD3<UInt8>(64, 64, 64)
        let buffer = device.makeBuffer(bytes: &leftShift, length: MemoryLayout<SIMD3<UInt8>>.size)
        buffer?.label = "ImmersiveYUVLeftShiftSix"
        return buffer
    }()

    /// Binds matrix (0), offset (1), and leftShift (2) for `immersiveNV12Fragment` / `displayNV12Texture` math.
    static func bind(to encoder: MTLRenderCommandEncoder, pixelBuffer: CVPixelBuffer) {
        let yCbCrMatrix = pixelBuffer.yCbCrMatrix
        let isFullRangeVideo = pixelBuffer.isFullRangeVideo

        let matrixBuffer: MTLBuffer?
        if yCbCrMatrix == kCVImageBufferYCbCrMatrix_ITU_R_709_2 {
            matrixBuffer = isFullRangeVideo
                ? colorConversion709FullRangeMatrixBuffer
                : colorConversion709VideoRangeMatrixBuffer
        } else if yCbCrMatrix == kCVImageBufferYCbCrMatrix_SMPTE_240M_1995 {
            matrixBuffer = isFullRangeVideo
                ? colorConversionSMPTE240MFullRangeMatrixBuffer
                : colorConversionSMPTE240MVideoRangeMatrixBuffer
        } else if yCbCrMatrix == kCVImageBufferYCbCrMatrix_ITU_R_2020 {
            matrixBuffer = isFullRangeVideo
                ? colorConversion2020FullRangeMatrixBuffer
                : colorConversion2020VideoRangeMatrixBuffer
        } else {
            matrixBuffer = isFullRangeVideo
                ? colorConversion601FullRangeMatrixBuffer
                : colorConversion601VideoRangeMatrixBuffer
        }

        let offsetBuffer = isFullRangeVideo
            ? colorOffsetFullRangeMatrixBuffer
            : colorOffsetVideoRangeMatrixBuffer
        let leftShiftBuffer = pixelBuffer.leftShift == 0
            ? leftShiftMatrixBuffer
            : leftShiftSixMatrixBuffer

        if let matrixBuffer {
            encoder.setFragmentBuffer(matrixBuffer, offset: 0, index: 0)
        }
        if let offsetBuffer {
            encoder.setFragmentBuffer(offsetBuffer, offset: 0, index: 1)
        }
        if let leftShiftBuffer {
            encoder.setFragmentBuffer(leftShiftBuffer, offset: 0, index: 2)
        }
    }
}

private let kvImage_YpCbCrToARGBMatrix_SMPTE_240M_1995 = vImage_YpCbCrToARGBMatrix(Kr: 0.212, Kb: 0.087)
private let kvImage_YpCbCrToARGBMatrix_ITU_R_2020 = vImage_YpCbCrToARGBMatrix(Kr: 0.2627, Kb: 0.0593)

private extension vImage_YpCbCrToARGBMatrix {
    init(Kr: Float, Kb: Float) {
        let kg = 1 - Kr - Kb
        self.init(
            Yp: 1,
            Cr_R: 2 - 2 * Kr,
            Cr_G: -Kr * (2 - 2 * Kr) / kg,
            Cb_G: -Kb * (2 - 2 * Kb) / kg,
            Cb_B: 2 - 2 * Kb
        )
    }

    var videoRange: vImage_YpCbCrToARGBMatrix {
        vImage_YpCbCrToARGBMatrix(
            Yp: 255 / 219 * Yp,
            Cr_R: 255 / 224 * Cr_R,
            Cr_G: 255 / 224 * Cr_G,
            Cb_G: 255 / 224 * Cb_G,
            Cb_B: 255 / 224 * Cb_B
        )
    }

    func metalBuffer(device: MTLDevice, label: String) -> MTLBuffer? {
        var matrix = simd_float3x3([Yp, Yp, Yp], [0.0, Cb_G, Cb_B], [Cr_R, Cr_G, 0.0])
        let buffer = device.makeBuffer(bytes: &matrix, length: MemoryLayout<simd_float3x3>.size)
        buffer?.label = label
        return buffer
    }
}
#endif

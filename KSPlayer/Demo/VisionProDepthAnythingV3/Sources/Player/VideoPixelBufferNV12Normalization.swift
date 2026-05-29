#if canImport(CoreVideo)
import CoreVideo
import Foundation
import KSPlayer

/// Converts planar 420 (`y420`) into NV12 for Core Image, DA3, and the immersive compositor.
enum VideoPixelBufferNV12Normalization {
  private static let lock = NSLock()
  nonisolated(unsafe) private static var pool: CVPixelBufferPool?
  nonisolated(unsafe) private static var poolSize = (width: 0, height: 0, fullRange: false)

  /// Independent copy for ring-buffer storage (decoder may reuse its backing buffer immediately).
  static func retainCopyForVideoFeed(from source: CVPixelBuffer) -> CVPixelBuffer? {
    if let normalized = nv12VideoRangeCopyIfNeeded(from: source) {
      return normalized
    }
    let format = CVPixelBufferGetPixelFormatType(source)
    switch format {
    case kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange:
      return copyBiPlanar420(from: source, fullRange: false)
    case kCVPixelFormatType_420YpCbCr8BiPlanarFullRange:
      return copyBiPlanar420(from: source, fullRange: true)
    case kCVPixelFormatType_32BGRA:
      return copyBGRA(from: source)
    default:
      return nil
    }
  }

  /// Returns an NV12 buffer when `source` is planar 420; otherwise `nil` (caller keeps `source`).
  static func nv12VideoRangeCopyIfNeeded(from source: CVPixelBuffer) -> CVPixelBuffer? {
    let format = CVPixelBufferGetPixelFormatType(source)
    switch format {
    case kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange,
         kCVPixelFormatType_420YpCbCr8BiPlanarFullRange,
         kCVPixelFormatType_32BGRA,
         kCVPixelFormatType_32ARGB:
      return nil
    case kCVPixelFormatType_420YpCbCr8Planar:
      return copyPlanar420ToNV12(from: source, fullRange: false)
    case kCVPixelFormatType_420YpCbCr8PlanarFullRange:
      return copyPlanar420ToNV12(from: source, fullRange: true)
    default:
      return nil
    }
  }

  private static func copyBiPlanar420(from source: CVPixelBuffer, fullRange: Bool) -> CVPixelBuffer? {
    let width = CVPixelBufferGetWidth(source)
    let height = CVPixelBufferGetHeight(source)
    guard width > 0, height > 0,
          let destination = makePooledPixelBuffer(width: width, height: height, fullRange: fullRange)
    else {
      return nil
    }

    CVPixelBufferLockBaseAddress(source, .readOnly)
    CVPixelBufferLockBaseAddress(destination, [])
    defer {
      CVPixelBufferUnlockBaseAddress(source, .readOnly)
      CVPixelBufferUnlockBaseAddress(destination, [])
    }

    guard let sourceY = CVPixelBufferGetBaseAddressOfPlane(source, 0),
          let sourceUV = CVPixelBufferGetBaseAddressOfPlane(source, 1),
          let destY = CVPixelBufferGetBaseAddressOfPlane(destination, 0),
          let destUV = CVPixelBufferGetBaseAddressOfPlane(destination, 1)
    else {
      return nil
    }

    let sourceYStride = CVPixelBufferGetBytesPerRowOfPlane(source, 0)
    let sourceUVStride = CVPixelBufferGetBytesPerRowOfPlane(source, 1)
    let destYStride = CVPixelBufferGetBytesPerRowOfPlane(destination, 0)
    let destUVStride = CVPixelBufferGetBytesPerRowOfPlane(destination, 1)
    let chromaHeight = height / 2

    for row in 0 ..< height {
      memcpy(
        destY.advanced(by: row * destYStride),
        sourceY.advanced(by: row * sourceYStride),
        width
      )
    }
    for row in 0 ..< chromaHeight {
      memcpy(
        destUV.advanced(by: row * destUVStride),
        sourceUV.advanced(by: row * sourceUVStride),
        width
      )
    }

    propagateAttachments(from: source, to: destination)
    return destination
  }

  private static func copyBGRA(from source: CVPixelBuffer) -> CVPixelBuffer? {
    let width = CVPixelBufferGetWidth(source)
    let height = CVPixelBufferGetHeight(source)
    guard width > 0, height > 0 else {
      return nil
    }
    var copy: CVPixelBuffer?
    let status = CVPixelBufferCreate(
      kCFAllocatorDefault,
      width,
      height,
      kCVPixelFormatType_32BGRA,
      [
        kCVPixelBufferMetalCompatibilityKey as String: true,
        kCVPixelBufferIOSurfacePropertiesKey as String: [:] as [String: Any],
      ] as CFDictionary,
      &copy
    )
    guard status == kCVReturnSuccess, let copy else {
      return nil
    }
    CVPixelBufferLockBaseAddress(source, .readOnly)
    CVPixelBufferLockBaseAddress(copy, [])
    defer {
      CVPixelBufferUnlockBaseAddress(source, .readOnly)
      CVPixelBufferUnlockBaseAddress(copy, [])
    }
    guard let src = CVPixelBufferGetBaseAddress(source),
          let dst = CVPixelBufferGetBaseAddress(copy)
    else {
      return nil
    }
    let srcStride = CVPixelBufferGetBytesPerRow(source)
    let dstStride = CVPixelBufferGetBytesPerRow(copy)
    let rowBytes = min(srcStride, dstStride)
    for row in 0 ..< height {
      memcpy(dst.advanced(by: row * dstStride), src.advanced(by: row * srcStride), rowBytes)
    }
    propagateAttachments(from: source, to: copy)
    return copy
  }

  private static func copyPlanar420ToNV12(from source: CVPixelBuffer, fullRange: Bool) -> CVPixelBuffer? {
    let width = CVPixelBufferGetWidth(source)
    let height = CVPixelBufferGetHeight(source)
    guard width > 0, height > 0 else {
      return nil
    }
    guard let destination = makePooledPixelBuffer(width: width, height: height, fullRange: fullRange) else {
      return nil
    }

    CVPixelBufferLockBaseAddress(source, .readOnly)
    CVPixelBufferLockBaseAddress(destination, [])
    defer {
      CVPixelBufferUnlockBaseAddress(source, .readOnly)
      CVPixelBufferUnlockBaseAddress(destination, [])
    }

    guard let sourceY = CVPixelBufferGetBaseAddressOfPlane(source, 0),
          let sourceU = CVPixelBufferGetBaseAddressOfPlane(source, 1),
          let sourceV = CVPixelBufferGetBaseAddressOfPlane(source, 2),
          let destY = CVPixelBufferGetBaseAddressOfPlane(destination, 0),
          let destUV = CVPixelBufferGetBaseAddressOfPlane(destination, 1)
    else {
      return nil
    }

    let sourceYStride = CVPixelBufferGetBytesPerRowOfPlane(source, 0)
    let sourceUStride = CVPixelBufferGetBytesPerRowOfPlane(source, 1)
    let sourceVStride = CVPixelBufferGetBytesPerRowOfPlane(source, 2)
    let destYStride = CVPixelBufferGetBytesPerRowOfPlane(destination, 0)
    let destUVStride = CVPixelBufferGetBytesPerRowOfPlane(destination, 1)
    let chromaWidth = width / 2
    let chromaHeight = height / 2

    for row in 0 ..< height {
      memcpy(
        destY.advanced(by: row * destYStride),
        sourceY.advanced(by: row * sourceYStride),
        width
      )
    }

    for row in 0 ..< chromaHeight {
      let destRow = destUV.advanced(by: row * destUVStride).assumingMemoryBound(to: UInt8.self)
      let uRow = sourceU.advanced(by: row * sourceUStride).assumingMemoryBound(to: UInt8.self)
      let vRow = sourceV.advanced(by: row * sourceVStride).assumingMemoryBound(to: UInt8.self)
      for column in 0 ..< chromaWidth {
        destRow[column * 2] = uRow[column]
        destRow[column * 2 + 1] = vRow[column]
      }
    }

    propagateAttachments(from: source, to: destination)
    return destination
  }

  private static func makePooledPixelBuffer(width: Int, height: Int, fullRange: Bool) -> CVPixelBuffer? {
    lock.lock()
    defer {
      lock.unlock()
    }
    if pool == nil || poolSize.width != width || poolSize.height != height || poolSize.fullRange != fullRange {
      let pixelFormat = fullRange
        ? kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
        : kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange
      let attributes: [String: Any] = [
        kCVPixelBufferPixelFormatTypeKey as String: pixelFormat,
        kCVPixelBufferWidthKey as String: width,
        kCVPixelBufferHeightKey as String: height,
        kCVPixelBufferMetalCompatibilityKey as String: true,
        kCVPixelBufferIOSurfacePropertiesKey as String: [:] as [String: Any],
      ]
      var newPool: CVPixelBufferPool?
      let status = CVPixelBufferPoolCreate(kCFAllocatorDefault, nil, attributes as CFDictionary, &newPool)
      guard status == kCVReturnSuccess, let newPool else {
        pool = nil
        return nil
      }
      pool = newPool
      poolSize = (width, height, fullRange)
    }
    guard let pool else {
      return nil
    }
    var pixelBuffer: CVPixelBuffer?
    let status = CVPixelBufferPoolCreatePixelBuffer(kCFAllocatorDefault, pool, &pixelBuffer)
    guard status == kCVReturnSuccess else {
      return nil
    }
    return pixelBuffer
  }

  private static func propagateAttachments(from source: CVPixelBuffer, to destination: CVPixelBuffer) {
    let keys: [CFString] = [
      KSVideoFrameOutputMetadata.presentationTimeSecondsKey,
      kCVImageBufferColorPrimariesKey,
      kCVImageBufferTransferFunctionKey,
      kCVImageBufferYCbCrMatrixKey,
      kCVImageBufferGammaLevelKey,
    ]
    for key in keys {
      if let value = CVBufferCopyAttachment(source, key, nil) {
        CVBufferSetAttachment(destination, key, value, .shouldPropagate)
      }
    }
  }
}
#endif

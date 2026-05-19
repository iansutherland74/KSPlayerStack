//
//  MetalPlayView.swift
//  KSPlayer
//
//  Created by kintan on 2018/3/11.
//

@preconcurrency import AVFoundation
import Combine
import CoreMedia
#if canImport(MetalKit)
import MetalKit
#endif
public protocol DisplayLayerDelegate: NSObjectProtocol {
    func change(displayLayer: AVSampleBufferDisplayLayer)
}

public protocol VideoOutput: FrameOutput {
    var displayLayerDelegate: DisplayLayerDelegate? { get set }
    var options: KSOptions { get set }
    var displayLayer: AVSampleBufferDisplayLayer { get }
    var pixelBuffer: PixelBufferProtocol? { get }
    init(options: KSOptions)
    func invalidate()
    func readNextFrame()
}

protocol PictureInPictureSubtitleBurnInRendering: AnyObject {
    var isPictureInPictureActive: Bool { get set }
    var pictureInPictureSubtitleSnapshot: PictureInPictureSubtitleSnapshot? { get set }
}

public final class MetalPlayView: UIView, @preconcurrency VideoOutput, @preconcurrency PictureInPictureSubtitleBurnInRendering {
    private struct UpscalingSourceKey: Equatable {
        let width: Int
        let height: Int
        let pixelFormat: OSType?
        let dynamicRange: DynamicRange?
        let colorPrimaries: String?
        let transferFunction: String?
        let yCbCrMatrix: String?

        init(pixelBuffer: PixelBufferProtocol, dynamicRange: DynamicRange?) {
            width = pixelBuffer.width
            height = pixelBuffer.height
            pixelFormat = pixelBuffer.cvPixelBuffer.map(CVPixelBufferGetPixelFormatType)
            self.dynamicRange = dynamicRange
            colorPrimaries = pixelBuffer.colorPrimaries.map { $0 as String }
            transferFunction = pixelBuffer.transferFunction.map { $0 as String }
            yCbCrMatrix = pixelBuffer.yCbCrMatrix.map { $0 as String }
        }
    }

    public var displayLayer: AVSampleBufferDisplayLayer {
        displayView.displayLayer
    }

    private var isDovi: Bool = false
    private var currentVideoDynamicRange: DynamicRange?
    private var formatDescription: CMFormatDescription? {
        didSet {
            updateDisplayCriteria()
        }
    }

    private var fps = Float(60) {
        didSet {
            if fps != oldValue {
                updateDisplayLinkFrameRate()
                updateDisplayCriteria()
            }
        }
    }

    public private(set) var pixelBuffer: PixelBufferProtocol?
    /// 用displayLink会导致锁屏无法draw，
    /// 用DispatchSourceTimer的话，在播放4k视频的时候repeat的时间会变长,
    /// 用MTKView的draw(in:)也是不行，会卡顿
    private var displayLink: CADisplayLink!
//    private let timer = DispatchSource.makeTimerSource(queue: DispatchQueue.main)
    public var options: KSOptions {
        didSet {
            guard oldValue !== options || oldValue.videoUpscaling != options.videoUpscaling else {
                return
            }
            resetUpscalingState()
            oldValue.videoUpscalingState = .inactive
        }
    }
    public weak var renderSource: OutputRenderSourceDelegate?
    // AVSampleBufferAudioRenderer AVSampleBufferRenderSynchronizer AVSampleBufferDisplayLayer
    var displayView = AVSampleBufferDisplayView() {
        didSet {
            displayLayerDelegate?.change(displayLayer: displayView.displayLayer)
        }
    }

    private let metalView = MetalView()
    private let videoUpscaler = VideoUpscaler()
    private var sourceUpscalingKey: UpscalingSourceKey?
    private var loggedHighPerformanceMessages = Set<String>()
    var isPictureInPictureActive = false
    var pictureInPictureSubtitleSnapshot: PictureInPictureSubtitleSnapshot?
    public weak var displayLayerDelegate: DisplayLayerDelegate?
    public init(options: KSOptions) {
        self.options = options
        super.init(frame: .zero)
        addSubview(displayView)
        addSubview(metalView)
        metalView.isHidden = true
        //        displayLink = CADisplayLink(block: renderFrame)
        displayLink = CADisplayLink(target: self, selector: #selector(renderFrame))
        // 一定要用common。不然在视频上面操作view的话，那就会卡顿了。
        displayLink.add(to: .main, forMode: .common)
        pause()
    }

    public func play() {
        displayLink.isPaused = false
    }

    public func pause() {
        displayLink.isPaused = true
    }

    @available(*, unavailable)
    required init(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override public func didAddSubview(_ subview: UIView) {
        super.didAddSubview(subview)
        subview.translatesAutoresizingMaskIntoConstraints = false
        NSLayoutConstraint.activate([
            subview.leftAnchor.constraint(equalTo: leftAnchor),
            subview.topAnchor.constraint(equalTo: topAnchor),
            subview.bottomAnchor.constraint(equalTo: bottomAnchor),
            subview.rightAnchor.constraint(equalTo: rightAnchor),
        ])
    }

    override public var contentMode: UIViewContentMode {
        didSet {
            metalView.contentMode = contentMode
            switch contentMode {
            case .scaleToFill:
                displayView.displayLayer.videoGravity = .resize
            case .scaleAspectFit, .center:
                displayView.displayLayer.videoGravity = .resizeAspect
            case .scaleAspectFill:
                displayView.displayLayer.videoGravity = .resizeAspectFill
            default:
                break
            }
        }
    }

    #if canImport(UIKit)
    override public func touchesMoved(_ touches: Set<UITouch>, with: UIEvent?) {
        if options.display == .plane {
            super.touchesMoved(touches, with: with)
        } else {
            options.display.touchesMoved(touch: touches.first!)
        }
    }
    #else
    override public func touchesMoved(with event: NSEvent) {
        if options.display == .plane {
            super.touchesMoved(with: event)
        } else {
            options.display.touchesMoved(touch: event.allTouches().first!)
        }
    }
    #endif

    public func flush() {
        pixelBuffer = nil
        resetUpscalingState()
        if displayView.isHidden {
            metalView.clear()
        } else {
            displayView.displayLayer.flushAndRemoveImage()
        }
    }

    public func invalidate() {
        pixelBuffer = nil
        resetUpscalingState()
        displayLink.invalidate()
    }

    public func readNextFrame() {
        draw(force: true)
    }

//    deinit {
//        print()
//    }
}

extension MetalPlayView {
    @objc private func renderFrame() {
        draw(force: false)
    }

    private func draw(force: Bool) {
        autoreleasepool {
            guard let frame = renderSource?.getVideoOutputRender(force: force) else {
                return
            }
            pixelBuffer = frame.corePixelBuffer
            guard let sourcePixelBuffer = pixelBuffer else {
                videoUpscaler.reset()
                sourceUpscalingKey = nil
                updateVideoUpscalingState(.inactive)
                return
            }
            var renderPixelBuffer: PixelBufferProtocol = sourcePixelBuffer
            isDovi = frame.isDovi
            fps = frame.fps
            let cmtime = frame.cmtime
            let sourcePar = sourcePixelBuffer.size
            if let cvPixelBuffer = sourcePixelBuffer.cvPixelBuffer {
                if let dar = options.customizeDar(sar: sourcePixelBuffer.aspectRatio, par: sourcePar) {
                    cvPixelBuffer.aspectRatio = CGSize(width: dar.width, height: dar.height * sourcePar.width / sourcePar.height)
                }
                let sourceDynamicRange = videoDynamicRange(pixelBuffer: sourcePixelBuffer, frame: frame)
                currentVideoDynamicRange = sourceDynamicRange
                updateDisplayCriteria()
                resetUpscalerIfSourceChanged(sourcePixelBuffer, dynamicRange: sourceDynamicRange)
                let upscalingSkipReason = HighPerformanceVideoPlaybackPolicy.upscalingSkipReason(
                    mode: options.videoUpscaling,
                    sourceSize: sourcePar,
                    fps: frame.fps,
                    dynamicRange: sourceDynamicRange
                )
                if upscalingSkipReason == nil, let upscaledPixelBuffer = videoUpscaler.upscale(pixelBuffer: cvPixelBuffer, time: cmtime, mode: options.videoUpscaling) {
                    renderPixelBuffer = upscaledPixelBuffer
                    pixelBuffer = upscaledPixelBuffer
                    updateVideoUpscalingState(videoUpscaler.state)
                } else if options.videoUpscaling != .none {
                    if let reason = upscalingSkipReason {
                        logHighPerformanceOnce("[video] Skip video upscaling for \(Int(sourcePar.width))x\(Int(sourcePar.height)) @ \(String(format: "%.2f", frame.fps))fps: \(reason)")
                        videoUpscaler.reset()
                        updateVideoUpscalingState(.unavailable(reason: reason))
                    } else {
                        updateVideoUpscalingState(videoUpscaler.state)
                    }
                } else {
                    videoUpscaler.reset()
                    updateVideoUpscalingState(.inactive)
                }
            } else {
                videoUpscaler.reset()
                sourceUpscalingKey = nil
                updateVideoUpscalingState(.inactive)
            }
            let par = renderPixelBuffer.size
            let sar = renderPixelBuffer.aspectRatio
            let dynamicRange = videoDynamicRange(pixelBuffer: renderPixelBuffer, frame: frame)
            currentVideoDynamicRange = dynamicRange
            updateDisplayCriteria()
            let hdr10PlusMetadata = frame.edrMetaData?.hdr10PlusMetadata
            let hasHDR10PlusMetadata = frame.edrMetaData?.hasHDR10PlusMetadata == true || hdr10PlusMetadata != nil
            let usesDisplayLayer = renderPixelBuffer.cvPixelBuffer != nil && options.isUseDisplayLayer(
                dynamicRange: dynamicRange,
                hasHDR10PlusMetadata: hasHDR10PlusMetadata
            )
            if let diagnostic = options.hdr10PlusPlaybackDiagnostic(
                hasHDR10PlusMetadata: hasHDR10PlusMetadata,
                usesDisplayLayer: usesDisplayLayer,
                metadata: hdr10PlusMetadata
            ) {
                logHighPerformanceOnce("[video] \(diagnostic.description)")
            }
            let hdr10PlusToneMapping = options.hdr10PlusMetalToneMappingUniform(
                metadata: hdr10PlusMetadata,
                dynamicRange: dynamicRange
            )
            let depthMap = video2DTo3DDepthMap(pixelBuffer: renderPixelBuffer, time: cmtime, dynamicRange: dynamicRange)
            let video2DTo3D = options.video2DTo3DRenderConfiguration(hasDepthMap: depthMap != nil)
            if let pixelBuffer = renderPixelBuffer.cvPixelBuffer, usesDisplayLayer {
                if displayView.isHidden {
                    displayView.isHidden = false
                    metalView.isHidden = true
                    metalView.clear()
                }
                let displayPixelBuffer = pictureInPictureSubtitlePixelBuffer(
                    source: pixelBuffer,
                    dynamicRange: dynamicRange
                )
                checkFormatDescription(pixelBuffer: displayPixelBuffer)
                set(pixelBuffer: displayPixelBuffer, time: cmtime)
            } else {
                updatePictureInPictureSubtitleDiagnosticForCurrentFrame(
                    reason: usesDisplayLayer ? nil : "the current KSMEPlayer frame is rendered through Metal and is not emitted as a sample buffer for PiP"
                )
                if !displayView.isHidden {
                    displayView.isHidden = true
                    metalView.isHidden = false
                    displayView.displayLayer.flushAndRemoveImage()
                }
                var size: CGSize
                if options.display == .plane {
                    if let dar = options.customizeDar(sar: sar, par: par) {
                        size = CGSize(width: par.width, height: par.width * dar.height / dar.width)
                    } else {
                        size = CGSize(width: par.width, height: par.height * sar.height / sar.width)
                    }
                    size = video2DTo3D.drawableSize(for: size)
                } else {
                    size = KSOptions.sceneSize
                }
                checkFormatDescription(pixelBuffer: renderPixelBuffer)
                #if !os(tvOS)
                if #available(iOS 16, *) {
                    metalView.metalLayer.edrMetadata = frame.edrMetadata
                }
                #endif
                metalView.draw(
                    pixelBuffer: renderPixelBuffer,
                    display: options.display,
                    size: size,
                    colorAdjustment: options.videoColorAdjustment,
                    dynamicRange: dynamicRange,
                    hdr10PlusToneMapping: hdr10PlusToneMapping,
                    video2DTo3D: video2DTo3D,
                    depthMap: depthMap,
                    stereoscopicVideoLayout: options.stereoscopicVideoLayout,
                    stereoscopicVideoEye: options.stereoscopicVideoEye,
                    panoramaStereoLayout: options.panoramaStereoLayout,
                    panoramaFieldOfView: options.panoramaFieldOfView
                )
            }
            renderSource?.setVideo(time: cmtime, position: frame.position)
        }
    }

    private func video2DTo3DDepthMap(pixelBuffer: PixelBufferProtocol, time: CMTime, dynamicRange: DynamicRange?) -> VideoDepthMap? {
        guard Video2DTo3DPolicy.isSupportedPlatform,
              options.video2DTo3DMode == .depthMapPreferred,
              options.display == .plane,
              options.stereoscopicVideoLayout == .mono,
              let provider = options.videoDepthEstimationProvider,
              let cvPixelBuffer = pixelBuffer.cvPixelBuffer
        else {
            return nil
        }
        let request = VideoDepthEstimationRequest(
            pixelBuffer: cvPixelBuffer,
            presentationTime: time,
            naturalSize: pixelBuffer.size,
            dynamicRange: dynamicRange
        )
        do {
            return try provider.makeDepthMap(request: request)
        } catch {
            logHighPerformanceOnce("[video] 2D-to-3D depth provider \(provider.providerID) failed: \(error.localizedDescription). Falling back to pseudo-stereo.")
            return nil
        }
    }

    private func resetUpscalerIfSourceChanged(_ pixelBuffer: PixelBufferProtocol, dynamicRange: DynamicRange?) {
        let sourceKey = UpscalingSourceKey(pixelBuffer: pixelBuffer, dynamicRange: dynamicRange)
        if let previous = sourceUpscalingKey, previous == sourceKey {
            return
        }
        if sourceUpscalingKey != nil {
            videoUpscaler.reset()
            updateVideoUpscalingState(.inactive)
        }
        sourceUpscalingKey = sourceKey
    }

    private func updateDisplayCriteria() {
        options.updateVideo(
            refreshRate: fps,
            dynamicRange: currentVideoDynamicRange ?? formatDescription?.dynamicRange
        )
    }

    private func videoDynamicRange(pixelBuffer: PixelBufferProtocol, frame: VideoVTBFrame) -> DynamicRange? {
        if frame.isDovi, let fallbackDynamicRange = frame.dolbyVisionFallbackDynamicRange {
            return fallbackDynamicRange
        }
        if let dynamicRange = pixelBuffer.formatDescription?.dynamicRange {
            return dynamicRange
        }
        if pixelBuffer.transferFunction == kCVImageBufferTransferFunction_ITU_R_2100_HLG {
            return .hlg
        }
        if pixelBuffer.transferFunction == kCVImageBufferTransferFunction_SMPTE_ST_2084_PQ || pixelBuffer.bitDepth == 10 {
            return .hdr10
        }
        return .sdr
    }

    private func checkFormatDescription(pixelBuffer: PixelBufferProtocol) {
        if formatDescription == nil || !pixelBuffer.matche(formatDescription: formatDescription!) {
            if formatDescription != nil {
                displayView.removeFromSuperview()
                displayView = AVSampleBufferDisplayView()
                displayView.frame = frame
                addSubview(displayView)
            }
            formatDescription = pixelBuffer.formatDescription
        }
    }

    private func set(pixelBuffer: CVPixelBuffer, time: CMTime) {
        guard let formatDescription else { return }
        displayView.enqueue(imageBuffer: pixelBuffer, formatDescription: formatDescription, time: time)
    }

    private func updateDisplayLinkFrameRate() {
        guard KSOptions.preferredFrame else {
            return
        }
        let range = HighPerformanceVideoPlaybackPolicy.displayFrameRateRange(fps: fps)
        if #available(iOS 15.0, tvOS 15.0, macOS 14.0, *) {
            displayLink.preferredFrameRateRange = CAFrameRateRange(minimum: range.minimum, maximum: range.maximum, __preferred: range.preferred)
        } else {
            displayLink.preferredFramesPerSecond = Int(range.maximum)
        }
    }

    private func logHighPerformanceOnce(_ message: String) {
        if loggedHighPerformanceMessages.insert(message).inserted {
            KSLog(message)
        }
    }

    private func updateVideoUpscalingState(_ state: VideoUpscalingState) {
        if options.videoUpscalingState != state {
            options.videoUpscalingState = state
        }
    }

    private func pictureInPictureSubtitlePixelBuffer(source pixelBuffer: CVPixelBuffer, dynamicRange: DynamicRange?) -> CVPixelBuffer {
        guard isPictureInPictureActive, options.pictureInPictureSubtitlePolicy == .burnIn else {
            return pixelBuffer
        }
        guard let snapshot = pictureInPictureSubtitleSnapshot, !snapshot.isEmpty else {
            updatePictureInPictureSubtitleDiagnosticForCurrentFrame(reason: nil)
            return pixelBuffer
        }
        guard dynamicRange == nil || dynamicRange == .sdr else {
            updatePictureInPictureSubtitleDiagnosticForCurrentFrame(reason: PictureInPictureSubtitleComposer.unsupportedDynamicRangeReason)
            return pixelBuffer
        }
        guard let compositedPixelBuffer = PictureInPictureSubtitleComposer.compositedPixelBuffer(source: pixelBuffer, snapshot: snapshot) else {
            updatePictureInPictureSubtitleDiagnosticForCurrentFrame(reason: PictureInPictureSubtitleComposer.unsupportedPixelBufferReason)
            return pixelBuffer
        }
        updatePictureInPictureSubtitleDiagnosticForCurrentFrame(reason: nil)
        return compositedPixelBuffer
    }

    private func updatePictureInPictureSubtitleDiagnosticForCurrentFrame(reason: String?) {
        guard isPictureInPictureActive, options.pictureInPictureSubtitlePolicy == .burnIn else {
            return
        }
        let diagnostic = PictureInPictureSubtitlePolicyResolver.diagnostic(
            policy: options.pictureInPictureSubtitlePolicy,
            usesNativeLegibleSelection: false,
            supportsSampleBufferBurnIn: reason == nil,
            burnInUnavailableReason: reason
        )
        if options.pictureInPictureSubtitleDiagnostic != diagnostic {
            options.pictureInPictureSubtitleDiagnostic = diagnostic
            KSLog("[pip] subtitle policy: \(diagnostic.description)")
        }
    }

    private func resetUpscalingState() {
        sourceUpscalingKey = nil
        videoUpscaler.reset()
        updateVideoUpscalingState(.inactive)
    }
}

class MetalView: UIView {
    private let render = MetalRender()
    private var neutralDepthTexture: MTLTexture?
    #if canImport(UIKit)
    override public class var layerClass: AnyClass { CAMetalLayer.self }
    #endif
    var metalLayer: CAMetalLayer {
        // swiftlint:disable force_cast
        layer as! CAMetalLayer
        // swiftlint:enable force_cast
    }

    init() {
        super.init(frame: .zero)
        #if !canImport(UIKit)
        layer = CAMetalLayer()
        #endif
        metalLayer.device = MetalRender.device
        metalLayer.framebufferOnly = true
//        metalLayer.displaySyncEnabled = false
    }

    @available(*, unavailable)
    required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    func clear() {
        if let drawable = metalLayer.nextDrawable() {
            render.clear(drawable: drawable)
        }
    }

    func draw(
        pixelBuffer: PixelBufferProtocol,
        display: DisplayEnum,
        size: CGSize,
        colorAdjustment: VideoColorAdjustment,
        dynamicRange: DynamicRange?,
        hdr10PlusToneMapping: HDR10PlusMetalToneMappingUniform?,
        video2DTo3D: Video2DTo3DRenderConfiguration,
        depthMap: VideoDepthMap?,
        stereoscopicVideoLayout: StereoscopicVideoLayout,
        stereoscopicVideoEye: StereoscopicVideoEye,
        panoramaStereoLayout: PanoramaStereoLayout,
        panoramaFieldOfView: PanoramaFieldOfView
    ) {
        metalLayer.drawableSize = size
        metalLayer.pixelFormat = KSOptions.colorPixelFormat(bitDepth: pixelBuffer.bitDepth)
        let colorspace = pixelBuffer.colorspace
        if colorspace != nil, metalLayer.colorspace != colorspace {
            metalLayer.colorspace = colorspace
            KSLog("[video] CAMetalLayer colorspace \(String(describing: colorspace))")
            #if !os(tvOS)
            if #available(iOS 16.0, *) {
                if let name = colorspace?.name, name != CGColorSpace.sRGB {
                    #if os(macOS)
                    metalLayer.wantsExtendedDynamicRangeContent = window?.screen?.maximumPotentialExtendedDynamicRangeColorComponentValue ?? 1.0 > 1.0
                    #else
                    metalLayer.wantsExtendedDynamicRangeContent = true
                    #endif
                } else {
                    metalLayer.wantsExtendedDynamicRangeContent = false
                }
                KSLog("[video] CAMetalLayer wantsExtendedDynamicRangeContent \(metalLayer.wantsExtendedDynamicRangeContent)")
            }
            #endif
        }
        guard let drawable = metalLayer.nextDrawable() else {
            KSLog("[video] CAMetalLayer not readyForMoreMediaData")
            return
        }
        render.draw(
            pixelBuffer: pixelBuffer,
            display: display,
            drawable: drawable,
            colorAdjustment: colorAdjustment,
            dynamicRange: dynamicRange,
            hdr10PlusToneMapping: hdr10PlusToneMapping,
            video2DTo3D: video2DTo3D,
            depthTexture: makeDepthTexture(depthMap: depthMap),
            stereoscopicVideoLayout: stereoscopicVideoLayout,
            stereoscopicVideoEye: stereoscopicVideoEye,
            panoramaStereoLayout: panoramaStereoLayout,
            panoramaFieldOfView: panoramaFieldOfView
        )
    }

    private func makeDepthTexture(depthMap: VideoDepthMap?) -> MTLTexture? {
        guard let depthMap else {
            if let neutralDepthTexture {
                return neutralDepthTexture
            }
            neutralDepthTexture = makeDepthTexture(width: 1, height: 1, values: [0.5], label: "neutralDepth")
            return neutralDepthTexture
        }
        return makeDepthTexture(
            width: depthMap.width,
            height: depthMap.height,
            values: depthMap.normalizedDisparity,
            label: "videoDepth"
        )
    }

    private func makeDepthTexture(width: Int, height: Int, values: [Float], label: String) -> MTLTexture? {
        let descriptor = MTLTextureDescriptor.texture2DDescriptor(pixelFormat: .r32Float, width: width, height: height, mipmapped: false)
        descriptor.usage = [.shaderRead]
        guard let texture = MetalRender.device.makeTexture(descriptor: descriptor) else {
            return nil
        }
        texture.label = label
        values.withUnsafeBytes { rawBuffer in
            guard let baseAddress = rawBuffer.baseAddress else {
                return
            }
            texture.replace(
                region: MTLRegionMake2D(0, 0, width, height),
                mipmapLevel: 0,
                withBytes: baseAddress,
                bytesPerRow: width * MemoryLayout<Float>.stride
            )
        }
        return texture
    }
}

class AVSampleBufferDisplayView: UIView {
    #if canImport(UIKit)
    override public class var layerClass: AnyClass { AVSampleBufferDisplayLayer.self }
    #endif
    var displayLayer: AVSampleBufferDisplayLayer {
        // swiftlint:disable force_cast
        layer as! AVSampleBufferDisplayLayer
        // swiftlint:enable force_cast
    }

    override init(frame: CGRect) {
        super.init(frame: frame)
        #if !canImport(UIKit)
        layer = AVSampleBufferDisplayLayer()
        #endif
        var controlTimebase: CMTimebase?
        CMTimebaseCreateWithSourceClock(allocator: kCFAllocatorDefault, sourceClock: CMClockGetHostTimeClock(), timebaseOut: &controlTimebase)
        if let controlTimebase {
            displayLayer.controlTimebase = controlTimebase
            CMTimebaseSetTime(controlTimebase, time: .zero)
            CMTimebaseSetRate(controlTimebase, rate: 1.0)
        }
    }

    @available(*, unavailable)
    required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    func enqueue(imageBuffer: CVPixelBuffer, formatDescription: CMVideoFormatDescription, time: CMTime) {
        let timing = CMSampleTimingInfo(duration: .invalid, presentationTimeStamp: .zero, decodeTimeStamp: .invalid)
        //        var timing = CMSampleTimingInfo(duration: .invalid, presentationTimeStamp: time, decodeTimeStamp: .invalid)
        var sampleBuffer: CMSampleBuffer?
        CMSampleBufferCreateReadyWithImageBuffer(allocator: kCFAllocatorDefault, imageBuffer: imageBuffer, formatDescription: formatDescription, sampleTiming: [timing], sampleBufferOut: &sampleBuffer)
        if let sampleBuffer {
            propagateVideoColorAttachments(from: imageBuffer, to: sampleBuffer)
            if let attachmentsArray = CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, createIfNecessary: true) as? [NSMutableDictionary], let dic = attachmentsArray.first {
                dic[kCMSampleAttachmentKey_DisplayImmediately] = true
            }
            if displayLayer.isReadyForMoreMediaData {
                displayLayer.enqueue(sampleBuffer)
            } else {
                KSLog("[video] AVSampleBufferDisplayLayer not readyForMoreMediaData. video time \(time), controlTime \(displayLayer.timebase.time) ")
                displayLayer.enqueue(sampleBuffer)
            }
            if #available(macOS 11.0, iOS 14, tvOS 14, *) {
                if displayLayer.requiresFlushToResumeDecoding {
                    KSLog("[video] AVSampleBufferDisplayLayer requiresFlushToResumeDecoding so flush")
                    displayLayer.flush()
                }
            }
            if displayLayer.status == .failed {
                KSLog("[video] AVSampleBufferDisplayLayer status failed so flush")
                displayLayer.flush()
                //                    if let error = displayLayer.error as NSError?, error.code == -11847 {
                //                        displayLayer.stopRequestingMediaData()
                //                    }
            }
        }
    }

    private func propagateVideoColorAttachments(from imageBuffer: CVPixelBuffer, to sampleBuffer: CMSampleBuffer) {
        let keys: [CFString] = [
            kCVImageBufferColorPrimariesKey,
            kCVImageBufferTransferFunctionKey,
            kCVImageBufferYCbCrMatrixKey,
            kCVImageBufferCGColorSpaceKey,
            kCVImageBufferGammaLevelKey,
        ]
        for key in keys {
            if let value = CVBufferCopyAttachment(imageBuffer, key, nil) {
                CMSetAttachment(sampleBuffer, key: key, value: value, attachmentMode: kCMAttachmentMode_ShouldPropagate)
            }
        }
    }
}

#if os(macOS)
import CoreVideo

class CADisplayLink {
    private let displayLink: CVDisplayLink
    private var runloop: RunLoop?
    private var mode = RunLoop.Mode.default
    public var preferredFramesPerSecond = 60
    @available(macOS 12.0, *)
    public var preferredFrameRateRange: CAFrameRateRange {
        get {
            CAFrameRateRange()
        }
        set {}
    }

    public var timestamp: TimeInterval {
        var timeStamp = CVTimeStamp()
        if CVDisplayLinkGetCurrentTime(displayLink, &timeStamp) == kCVReturnSuccess, (timeStamp.flags & CVTimeStampFlags.hostTimeValid.rawValue) != 0 {
            return TimeInterval(timeStamp.hostTime / NSEC_PER_SEC)
        }
        return 0
    }

    public var duration: TimeInterval {
        CVDisplayLinkGetActualOutputVideoRefreshPeriod(displayLink)
    }

    public var targetTimestamp: TimeInterval {
        duration + timestamp
    }

    public var isPaused: Bool {
        get {
            !CVDisplayLinkIsRunning(displayLink)
        }
        set {
            if newValue {
                CVDisplayLinkStop(displayLink)
            } else {
                CVDisplayLinkStart(displayLink)
            }
        }
    }

    public init(target: NSObject, selector: Selector) {
        var displayLink: CVDisplayLink?
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink)
        self.displayLink = displayLink!
        CVDisplayLinkSetOutputHandler(self.displayLink) { [weak self] _, _, _, _, _ in
            guard let self else { return kCVReturnSuccess }
            self.runloop?.perform(selector, target: target, argument: self, order: 0, modes: [self.mode])
            return kCVReturnSuccess
        }
        CVDisplayLinkStart(self.displayLink)
    }

    public init(block: @escaping (() -> Void)) {
        var displayLink: CVDisplayLink?
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink)
        self.displayLink = displayLink!
        CVDisplayLinkSetOutputHandler(self.displayLink) { _, _, _, _, _ in
            block()
            return kCVReturnSuccess
        }
        CVDisplayLinkStart(self.displayLink)
    }

    open func add(to runloop: RunLoop, forMode mode: RunLoop.Mode) {
        self.runloop = runloop
        self.mode = mode
    }

    public func invalidate() {
        isPaused = true
        runloop = nil
        CVDisplayLinkSetOutputHandler(displayLink) { _, _, _, _, _ in
            kCVReturnError
        }
    }
}
#endif

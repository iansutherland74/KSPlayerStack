//
//  FelBakerDolbyVisionFELCompositorBackend.swift
//  KSPlayer
//
//  Optional private-build Dolby Vision Profile 7 FEL compositor adapter.
//

import AVFoundation
import Foundation
#if canImport(Darwin)
import Darwin
#endif

public struct FelBakerDolbyVisionFELCompositorCapabilities: OptionSet, Sendable {
    public let rawValue: UInt64

    public init(rawValue: UInt64) {
        self.rawValue = rawValue
    }

    public static let fullEnhancementLayerComposition = Self(rawValue: 1 << 0)
    public static let pq12Output = Self(rawValue: 1 << 1)
    public static let rgb48Output = Self(rawValue: 1 << 2)
}

public enum FelBakerDolbyVisionFELCompositorError: Error, CustomStringConvertible {
    case unavailable(String)
    case outputPixelBufferCreationFailed(OSStatus)
    case compositionFailed(Int32)

    public var description: String {
        switch self {
        case let .unavailable(reason):
            return reason
        case let .outputPixelBufferCreationFailed(status):
            return "Unable to create FEL compositor output pixel buffer: \(status)"
        case let .compositionFailed(status):
            return "FelBaker FEL compositor shim failed: \(status)"
        }
    }
}

public protocol FelBakerDolbyVisionFELCompositorShim: AnyObject {
    var backendName: String { get }
    var capabilities: FelBakerDolbyVisionFELCompositorCapabilities { get }

    func compose(
        input: DolbyVisionFELCompositorInput,
        outputPixelBuffer: CVPixelBuffer
    ) throws -> String?
}

public final class FelBakerDolbyVisionFELCompositorBackend: DolbyVisionFELCompositorBackend {
    public let outputPixelFormat: OSType
    private let shim: (any FelBakerDolbyVisionFELCompositorShim)?
    private let unavailableReason: String?

    public var availability: DolbyVisionFELCompositorAvailability {
        guard let shim else {
            return .unavailable(reason: unavailableReason ?? "FelBaker FEL compositor shim is not configured")
        }
        guard shim.capabilities.contains(.fullEnhancementLayerComposition) else {
            return .unavailable(reason: "\(shim.backendName) does not report FEL composition capability")
        }
        return .available(backend: shim.backendName)
    }

    public init(
        shim: any FelBakerDolbyVisionFELCompositorShim,
        outputPixelFormat: OSType = kCVPixelFormatType_64RGBAHalf
    ) {
        self.shim = shim
        self.outputPixelFormat = outputPixelFormat
        unavailableReason = nil
    }

    public init(
        libraryURL: URL,
        outputPixelFormat: OSType = kCVPixelFormatType_64RGBAHalf
    ) {
        self.outputPixelFormat = outputPixelFormat
        do {
            shim = try DynamicFelBakerDolbyVisionFELCompositorShim(libraryURL: libraryURL)
            unavailableReason = nil
        } catch {
            shim = nil
            unavailableReason = String(describing: error)
        }
    }

    public func compose(input: DolbyVisionFELCompositorInput) throws -> DolbyVisionFELCompositorOutput {
        guard case .available = availability, let shim else {
            throw FelBakerDolbyVisionFELCompositorError.unavailable(availability.description)
        }
        let outputPixelBuffer = try Self.makeOutputPixelBuffer(
            matching: input.baseLayerPixelBuffer,
            pixelFormat: outputPixelFormat
        )
        CVBufferPropagateAttachments(input.baseLayerPixelBuffer, outputPixelBuffer)
        let message = try shim.compose(input: input, outputPixelBuffer: outputPixelBuffer)
        return DolbyVisionFELCompositorOutput(pixelBuffer: outputPixelBuffer, diagnosticMessage: message)
    }

    private static func makeOutputPixelBuffer(matching pixelBuffer: CVPixelBuffer, pixelFormat: OSType) throws -> CVPixelBuffer {
        let attributes: [String: Any] = [
            kCVPixelBufferPixelFormatTypeKey as String: pixelFormat,
            kCVPixelBufferWidthKey as String: CVPixelBufferGetWidth(pixelBuffer),
            kCVPixelBufferHeightKey as String: CVPixelBufferGetHeight(pixelBuffer),
            kCVPixelBufferMetalCompatibilityKey as String: true,
            kCVPixelBufferIOSurfacePropertiesKey as String: [String: String](),
        ]
        var outputPixelBuffer: CVPixelBuffer?
        let status = CVPixelBufferCreate(
            kCFAllocatorDefault,
            CVPixelBufferGetWidth(pixelBuffer),
            CVPixelBufferGetHeight(pixelBuffer),
            pixelFormat,
            attributes as CFDictionary,
            &outputPixelBuffer
        )
        guard status == kCVReturnSuccess, let outputPixelBuffer else {
            throw FelBakerDolbyVisionFELCompositorError.outputPixelBufferCreationFailed(status)
        }
        return outputPixelBuffer
    }
}

private final class DynamicFelBakerDolbyVisionFELCompositorShim: FelBakerDolbyVisionFELCompositorShim {
    private typealias VersionFunction = @convention(c) () -> UInt32
    private typealias NameFunction = @convention(c) () -> UnsafePointer<CChar>?
    private typealias CapabilitiesFunction = @convention(c) () -> UInt64
    private typealias ComposeFunction = @convention(c) (
        CVPixelBuffer,
        CVPixelBuffer,
        UnsafePointer<UInt8>?,
        Int,
        CMTime,
        CVPixelBuffer,
        UnsafeMutablePointer<CChar>?,
        Int
    ) -> Int32

    private let handle: UnsafeMutableRawPointer
    private let nameFunction: NameFunction
    private let capabilitiesFunction: CapabilitiesFunction
    private let composeFunction: ComposeFunction

    var backendName: String {
        nameFunction().map { String(cString: $0) } ?? "FelBaker"
    }

    var capabilities: FelBakerDolbyVisionFELCompositorCapabilities {
        FelBakerDolbyVisionFELCompositorCapabilities(rawValue: capabilitiesFunction())
    }

    init(libraryURL: URL) throws {
        #if canImport(Darwin)
        guard let handle = dlopen(libraryURL.path, RTLD_NOW | RTLD_LOCAL) else {
            throw FelBakerDolbyVisionFELCompositorError.unavailable(Self.lastDynamicLoaderError())
        }
        do {
            let version: VersionFunction = try Self.loadSymbol(
                "ksplayer_felbaker_backend_abi_version",
                from: handle
            )
            guard version() == 1 else {
                throw FelBakerDolbyVisionFELCompositorError.unavailable("Unsupported FelBaker shim ABI version \(version())")
            }
            self.handle = handle
            nameFunction = try Self.loadSymbol("ksplayer_felbaker_backend_name", from: handle)
            capabilitiesFunction = try Self.loadSymbol("ksplayer_felbaker_backend_capabilities", from: handle)
            composeFunction = try Self.loadSymbol("ksplayer_felbaker_compose", from: handle)
        } catch {
            dlclose(handle)
            throw error
        }
        #else
        throw FelBakerDolbyVisionFELCompositorError.unavailable("Dynamic FelBaker FEL compositor loading is unavailable on this platform")
        #endif
    }

    deinit {
        #if canImport(Darwin)
        dlclose(handle)
        #endif
    }

    func compose(input: DolbyVisionFELCompositorInput, outputPixelBuffer: CVPixelBuffer) throws -> String? {
        var message = [CChar](repeating: 0, count: 512)
        let status = input.rpuData.withUnsafeBytes { rpuBytes in
            composeFunction(
                input.baseLayerPixelBuffer,
                input.enhancementLayerPixelBuffer,
                rpuBytes.bindMemory(to: UInt8.self).baseAddress,
                input.rpuData.count,
                input.presentationTime,
                outputPixelBuffer,
                &message,
                message.count
            )
        }
        guard status == 0 else {
            throw FelBakerDolbyVisionFELCompositorError.compositionFailed(status)
        }
        let diagnosticMessage = message.withUnsafeBufferPointer { buffer in
            buffer.baseAddress.map { String(cString: $0) } ?? ""
        }
        return diagnosticMessage.isEmpty ? nil : diagnosticMessage
    }

    #if canImport(Darwin)
    private static func loadSymbol<Function>(_ name: String, from handle: UnsafeMutableRawPointer) throws -> Function {
        guard let symbol = dlsym(handle, name) else {
            throw FelBakerDolbyVisionFELCompositorError.unavailable("FelBaker shim missing symbol \(name)")
        }
        return unsafeBitCast(symbol, to: Function.self)
    }

    private static func lastDynamicLoaderError() -> String {
        dlerror().map { String(cString: $0) } ?? "Unable to load FelBaker FEL compositor shim"
    }
    #endif
}

import Foundation
#if canImport(Metal)
@preconcurrency import Metal
#endif

public enum Depth3DDebugLevel: String, Sendable {
    case info
    case warning
    case error
    case assertion
}

public struct Depth3DDebugEvent: Equatable, Sendable {
    public let level: Depth3DDebugLevel
    public let phase: String
    public let message: String
    public let file: String
    public let line: UInt
    public let timestamp: TimeInterval

    public var summary: String {
        "[\(phase)] \(message)"
    }
}

public enum Depth3DDebug {
    #if DEBUG
    public nonisolated(unsafe) static var isEnabled = true
    #else
    public nonisolated(unsafe) static var isEnabled = false
    #endif
    public nonisolated(unsafe) static var isVerbose = false
    private nonisolated(unsafe) static var handler: (@MainActor @Sendable (Depth3DDebugEvent) -> Void)?

    public static func setEventHandler(_ eventHandler: (@MainActor @Sendable (Depth3DDebugEvent) -> Void)?) {
        handler = eventHandler
    }

    public static func log(
        _ message: @autoclosure () -> String,
        level: Depth3DDebugLevel = .info,
        phase: String,
        verboseOnly: Bool = false,
        file: String = #fileID,
        line: UInt = #line
    ) {
        guard isEnabled, !verboseOnly || isVerbose else {
            return
        }
        let event = Depth3DDebugEvent(
            level: level,
            phase: phase,
            message: message(),
            file: file,
            line: line,
            timestamp: Date().timeIntervalSinceReferenceDate
        )
        if let handler {
            Task { @MainActor in
                handler(event)
            }
        }
        let prefix = "[Depth3D][\(event.level.rawValue)][\(event.phase)]"
        let logLevel: LogLevel = switch level {
        case .info:
            .debug
        case .warning:
            .warning
        case .error, .assertion:
            .error
        }
        KSLog(level: logLevel, "\(prefix) \(event.message) (\(event.file):\(event.line))")
    }

    public static func warn(
        _ message: @autoclosure () -> String,
        phase: String,
        file: String = #fileID,
        line: UInt = #line
    ) {
        log(message(), level: .warning, phase: phase, file: file, line: line)
    }

    public static func fail(
        _ message: @autoclosure () -> String,
        phase: String,
        file: String = #fileID,
        line: UInt = #line
    ) {
        log(message(), level: .error, phase: phase, file: file, line: line)
    }

    #if canImport(Metal)
    public static func assertTexture(
        _ texture: (any MTLTexture)?,
        label: String,
        expectedPixelFormat: MTLPixelFormat? = nil,
        phase: String,
        file: String = #fileID,
        line: UInt = #line
    ) -> Bool {
        guard let texture else {
            Depth3DAssert.check(false, "\(label) texture is nil", phase: phase, file: file, line: line)
            return false
        }
        var isValid = true
        if texture.width <= 0 || texture.height <= 0 {
            isValid = false
            Depth3DAssert.check(false, "\(label) texture has invalid size \(texture.width)x\(texture.height)", phase: phase, file: file, line: line)
        }
        if let expectedPixelFormat, texture.pixelFormat != expectedPixelFormat {
            isValid = false
            Depth3DAssert.check(false, "\(label) texture format \(texture.pixelFormat) does not match \(expectedPixelFormat)", phase: phase, file: file, line: line)
        }
        if isValid {
            log("\(label) texture \(texture.width)x\(texture.height) format \(texture.pixelFormat)", phase: phase, verboseOnly: true, file: file, line: line)
        }
        return isValid
    }

    public static func logCommandBuffer(_ commandBuffer: any MTLCommandBuffer, label: String, phase: String) {
        commandBuffer.addCompletedHandler { buffer in
            if let error = buffer.error {
                Depth3DDebug.fail("\(label) command buffer failed: \(error.localizedDescription)", phase: phase)
            } else if Depth3DDebug.isVerbose {
                Depth3DDebug.log("\(label) command buffer completed with status \(buffer.status.rawValue)", phase: phase, verboseOnly: true)
            }
        }
    }
    #endif
}

public enum Depth3DAssert {
    @discardableResult
    public static func check(
        _ condition: @autoclosure () -> Bool,
        _ message: @autoclosure () -> String,
        phase: String,
        file: String = #fileID,
        line: UInt = #line
    ) -> Bool {
        let isValid = condition()
        guard !isValid else {
            return true
        }
        Depth3DDebug.log(message(), level: .assertion, phase: phase, file: file, line: line)
        assertionFailure(message())
        return false
    }
}

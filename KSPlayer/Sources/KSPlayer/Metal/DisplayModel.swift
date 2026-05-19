//
//  DisplayModel.swift
//  KSPlayer-iOS
//
//  Created by kintan on 2020/1/11.
//

import Foundation
import Metal
import simd
#if canImport(UIKit)
import UIKit
#endif

extension DisplayEnum {
    private static var planeDisplay = PlaneDisplayModel()
    private static var vrDiaplay = VRDisplayModel()
    private static var vrBoxDiaplay = VRBoxDisplayModel()

    func set(encoder: MTLRenderCommandEncoder, panoramaStereoLayout: PanoramaStereoLayout, panoramaFieldOfView: PanoramaFieldOfView) {
        switch self {
        case .plane:
            DisplayEnum.planeDisplay.set(encoder: encoder)
        case .vr:
            DisplayEnum.vrDiaplay.set(encoder: encoder, panoramaStereoLayout: panoramaStereoLayout, panoramaFieldOfView: panoramaFieldOfView)
        case .vrBox:
            DisplayEnum.vrBoxDiaplay.set(encoder: encoder, panoramaStereoLayout: panoramaStereoLayout, panoramaFieldOfView: panoramaFieldOfView)
        }
    }

    func pipeline(planeCount: Int, bitDepth: Int32) -> MTLRenderPipelineState {
        switch self {
        case .plane:
            return DisplayEnum.planeDisplay.pipeline(planeCount: planeCount, bitDepth: bitDepth)
        case .vr:
            return DisplayEnum.vrDiaplay.pipeline(planeCount: planeCount, bitDepth: bitDepth)
        case .vrBox:
            return DisplayEnum.vrBoxDiaplay.pipeline(planeCount: planeCount, bitDepth: bitDepth)
        }
    }

    func touchesMoved(touch: UITouch) {
        switch self {
        case .vr:
            DisplayEnum.vrDiaplay.touchesMoved(touch: touch)
        case .vrBox:
            DisplayEnum.vrBoxDiaplay.touchesMoved(touch: touch)
        default:
            break
        }
    }
}

private class PlaneDisplayModel {
    private lazy var yuv = MetalRender.makePipelineState(fragmentFunction: "displayYUVTexture")
    private lazy var yuvp010LE = MetalRender.makePipelineState(fragmentFunction: "displayYUVTexture", bitDepth: 10)
    private lazy var nv12 = MetalRender.makePipelineState(fragmentFunction: "displayNV12Texture")
    private lazy var p010LE = MetalRender.makePipelineState(fragmentFunction: "displayNV12Texture", bitDepth: 10)
    private lazy var bgra = MetalRender.makePipelineState(fragmentFunction: "displayTexture")
    let indexCount: Int
    let indexType = MTLIndexType.uint16
    let primitiveType = MTLPrimitiveType.triangleStrip
    let indexBuffer: MTLBuffer
    let posBuffer: MTLBuffer?
    let uvBuffer: MTLBuffer?

    fileprivate init() {
        let (indices, positions, uvs) = PlaneDisplayModel.genSphere()
        let device = MetalRender.device
        indexCount = indices.count
        indexBuffer = device.makeBuffer(bytes: indices, length: MemoryLayout<UInt16>.size * indexCount)!
        posBuffer = device.makeBuffer(bytes: positions, length: MemoryLayout<simd_float4>.size * positions.count)
        uvBuffer = device.makeBuffer(bytes: uvs, length: MemoryLayout<simd_float2>.size * uvs.count)
    }

    private static func genSphere() -> ([UInt16], [simd_float4], [simd_float2]) {
        let indices: [UInt16] = [0, 1, 2, 3]
        let positions: [simd_float4] = [
            [-1.0, -1.0, 0.0, 1.0],
            [-1.0, 1.0, 0.0, 1.0],
            [1.0, -1.0, 0.0, 1.0],
            [1.0, 1.0, 0.0, 1.0],
        ]
        let uvs: [simd_float2] = [
            [0.0, 1.0],
            [0.0, 0.0],
            [1.0, 1.0],
            [1.0, 0.0],
        ]
        return (indices, positions, uvs)
    }

    func set(encoder: MTLRenderCommandEncoder) {
        encoder.setFrontFacing(.clockwise)
        encoder.setVertexBuffer(posBuffer, offset: 0, index: 0)
        encoder.setVertexBuffer(uvBuffer, offset: 0, index: 1)
        encoder.drawIndexedPrimitives(type: primitiveType, indexCount: indexCount, indexType: indexType, indexBuffer: indexBuffer, indexBufferOffset: 0)
    }

    func pipeline(planeCount: Int, bitDepth: Int32) -> MTLRenderPipelineState {
        switch planeCount {
        case 3:
            if bitDepth == 10 {
                return yuvp010LE
            } else {
                return yuv
            }
        case 2:
            if bitDepth == 10 {
                return p010LE
            } else {
                return nv12
            }
        case 1:
            return bgra
        default:
            return bgra
        }
    }
}

@MainActor
private class SphereDisplayModel {
    private struct SphereMeshKey: Hashable {
        let fieldOfView: PanoramaFieldOfView
        let stereoLayout: PanoramaStereoLayout
        let eye: PanoramaTextureEye
    }

    private struct SphereMesh {
        let indexCount: Int
        let indexBuffer: MTLBuffer
        let posBuffer: MTLBuffer?
        let uvBuffer: MTLBuffer?
    }

    private lazy var yuv = MetalRender.makePipelineState(fragmentFunction: "displayYUVTexture", isSphere: true)
    private lazy var yuvp010LE = MetalRender.makePipelineState(fragmentFunction: "displayYUVTexture", isSphere: true, bitDepth: 10)
    private lazy var nv12 = MetalRender.makePipelineState(fragmentFunction: "displayNV12Texture", isSphere: true)
    private lazy var p010LE = MetalRender.makePipelineState(fragmentFunction: "displayNV12Texture", isSphere: true, bitDepth: 10)
    private lazy var bgra = MetalRender.makePipelineState(fragmentFunction: "displayTexture", isSphere: true)
    private var fingerRotationX = Float(0)
    private var fingerRotationY = Float(0)
    fileprivate var modelViewMatrix = matrix_identity_float4x4
    let indexType = MTLIndexType.uint16
    let primitiveType = MTLPrimitiveType.triangle
    private var meshes = [SphereMeshKey: SphereMesh]()
    @MainActor
    fileprivate init() {
        #if canImport(UIKit) && canImport(CoreMotion)
        if KSOptions.enableSensor {
            MotionSensor.shared.start()
        }
        #endif
    }

    func set(
        encoder: MTLRenderCommandEncoder,
        panoramaStereoLayout: PanoramaStereoLayout,
        panoramaFieldOfView: PanoramaFieldOfView,
        eye: PanoramaTextureEye
    ) -> SphereMesh {
        let mesh = mesh(fieldOfView: panoramaFieldOfView, stereoLayout: panoramaStereoLayout, eye: eye)
        encoder.setFrontFacing(.clockwise)
        encoder.setVertexBuffer(mesh.posBuffer, offset: 0, index: 0)
        encoder.setVertexBuffer(mesh.uvBuffer, offset: 0, index: 1)
        #if canImport(UIKit) && canImport(CoreMotion)
        if KSOptions.enableSensor, let matrix = MotionSensor.shared.matrix() {
            modelViewMatrix = matrix
        }
        #endif
        return mesh
    }

    @MainActor
    func touchesMoved(touch: UITouch) {
        #if canImport(UIKit)
        let view = touch.view
        #else
        let view: UIView? = nil
        #endif
        var distX = Float(touch.location(in: view).x - touch.previousLocation(in: view).x)
        var distY = Float(touch.location(in: view).y - touch.previousLocation(in: view).y)
        distX *= 0.005
        distY *= 0.005
        fingerRotationX -= distY * 60 / 100
        fingerRotationY -= distX * 60 / 100
        modelViewMatrix = matrix_identity_float4x4.rotateX(radians: fingerRotationX).rotateY(radians: fingerRotationY)
    }

    func reset() {
        fingerRotationX = 0
        fingerRotationY = 0
        modelViewMatrix = matrix_identity_float4x4
    }

    private func mesh(fieldOfView: PanoramaFieldOfView, stereoLayout: PanoramaStereoLayout, eye: PanoramaTextureEye) -> SphereMesh {
        let key = SphereMeshKey(fieldOfView: fieldOfView, stereoLayout: stereoLayout, eye: eye)
        if let mesh = meshes[key] {
            return mesh
        }
        let (indices, positions, uvs) = SphereDisplayModel.genSphere(fieldOfView: fieldOfView, stereoLayout: stereoLayout, eye: eye)
        let device = MetalRender.device
        let mesh = SphereMesh(
            indexCount: indices.count,
            indexBuffer: device.makeBuffer(bytes: indices, length: MemoryLayout<UInt16>.size * indices.count)!,
            posBuffer: device.makeBuffer(bytes: positions, length: MemoryLayout<simd_float4>.size * positions.count),
            uvBuffer: device.makeBuffer(bytes: uvs, length: MemoryLayout<simd_float2>.size * uvs.count)
        )
        meshes[key] = mesh
        return mesh
    }

    private static func genSphere(
        fieldOfView: PanoramaFieldOfView,
        stereoLayout: PanoramaStereoLayout,
        eye: PanoramaTextureEye
    ) -> ([UInt16], [simd_float4], [simd_float2]) {
        let slicesCount = UInt16(200)
        let parallelsCount = slicesCount / 2
        let indicesCount = Int(slicesCount) * Int(parallelsCount) * 6
        var indices = [UInt16](repeating: 0, count: indicesCount)
        var positions = [simd_float4]()
        var uvs = [simd_float2]()
        var runCount = 0
        let radius = Float(1.0)
        let longitudeStart: Float
        let longitudeSpan: Float
        switch fieldOfView {
        case .degrees180:
            longitudeStart = Float.pi
            longitudeSpan = Float.pi
        case .degrees360:
            longitudeStart = 0
            longitudeSpan = 2.0 * Float.pi
        }
        let longitudeStep = longitudeSpan / Float(slicesCount)
        let latitudeStep = Float.pi / Float(parallelsCount)
        let textureBounds = stereoLayout.textureCoordinateBounds(for: eye)
        var i = UInt16(0)
        while i <= parallelsCount {
            var j = UInt16(0)
            while j <= slicesCount {
                let latitude = latitudeStep * Float(i)
                let longitude = longitudeStart + longitudeStep * Float(j)
                let vertex0 = radius * sinf(latitude) * cosf(longitude)
                let vertex1 = radius * cosf(latitude)
                let vertex2 = radius * sinf(latitude) * sinf(longitude)
                let vertex3 = Float(1.0)
                let textureX = CGFloat(Float(j) / Float(slicesCount))
                let textureY = CGFloat(Float(i) / Float(parallelsCount))
                let vertex4 = Float(textureBounds.minX + textureBounds.width * textureX)
                let vertex5 = Float(textureBounds.minY + textureBounds.height * textureY)
                positions.append([vertex0, vertex1, vertex2, vertex3])
                uvs.append([vertex4, vertex5])
                if i < parallelsCount, j < slicesCount {
                    indices[runCount] = i * (slicesCount + 1) + j
                    runCount += 1
                    indices[runCount] = UInt16((i + 1) * (slicesCount + 1) + j)
                    runCount += 1
                    indices[runCount] = UInt16((i + 1) * (slicesCount + 1) + (j + 1))
                    runCount += 1
                    indices[runCount] = UInt16(i * (slicesCount + 1) + j)
                    runCount += 1
                    indices[runCount] = UInt16((i + 1) * (slicesCount + 1) + (j + 1))
                    runCount += 1
                    indices[runCount] = UInt16(i * (slicesCount + 1) + (j + 1))
                    runCount += 1
                }
                j += 1
            }
            i += 1
        }
        return (indices, positions, uvs)
    }

    func pipeline(planeCount: Int, bitDepth: Int32) -> MTLRenderPipelineState {
        switch planeCount {
        case 3:
            if bitDepth == 10 {
                return yuvp010LE
            } else {
                return yuv
            }
        case 2:
            if bitDepth == 10 {
                return p010LE
            } else {
                return nv12
            }
        case 1:
            return bgra
        default:
            return bgra
        }
    }
}

private class VRDisplayModel: SphereDisplayModel {
    private let modelViewProjectionMatrix: simd_float4x4

    override required init() {
        let size = KSOptions.sceneSize
        let aspect = Float(size.width / size.height)
        let projectionMatrix = simd_float4x4(perspective: Float.pi / 3, aspect: aspect, nearZ: 0.1, farZ: 400.0)
        let viewMatrix = simd_float4x4(lookAt: SIMD3<Float>.zero, center: [0, 0, -1000], up: [0, 1, 0])
        modelViewProjectionMatrix = projectionMatrix * viewMatrix
        super.init()
    }

    func set(encoder: MTLRenderCommandEncoder, panoramaStereoLayout: PanoramaStereoLayout, panoramaFieldOfView: PanoramaFieldOfView) {
        let mesh = super.set(encoder: encoder, panoramaStereoLayout: panoramaStereoLayout, panoramaFieldOfView: panoramaFieldOfView, eye: .mono)
        var matrix = modelViewProjectionMatrix * modelViewMatrix
        let matrixBuffer = MetalRender.device.makeBuffer(bytes: &matrix, length: MemoryLayout<simd_float4x4>.size)
        encoder.setVertexBuffer(matrixBuffer, offset: 0, index: 2)
        encoder.drawIndexedPrimitives(type: primitiveType, indexCount: mesh.indexCount, indexType: indexType, indexBuffer: mesh.indexBuffer, indexBufferOffset: 0)
    }
}

private class VRBoxDisplayModel: SphereDisplayModel {
    private let modelViewProjectionMatrixLeft: simd_float4x4
    private let modelViewProjectionMatrixRight: simd_float4x4
    override required init() {
        let size = KSOptions.sceneSize
        let aspect = Float(size.width / size.height) / 2
        let viewMatrixLeft = simd_float4x4(lookAt: [-0.012, 0, 0], center: [0, 0, -1000], up: [0, 1, 0])
        let viewMatrixRight = simd_float4x4(lookAt: [0.012, 0, 0], center: [0, 0, -1000], up: [0, 1, 0])
        let projectionMatrix = simd_float4x4(perspective: Float.pi / 3, aspect: aspect, nearZ: 0.1, farZ: 400.0)
        modelViewProjectionMatrixLeft = projectionMatrix * viewMatrixLeft
        modelViewProjectionMatrixRight = projectionMatrix * viewMatrixRight
        super.init()
    }

    func set(encoder: MTLRenderCommandEncoder, panoramaStereoLayout: PanoramaStereoLayout, panoramaFieldOfView: PanoramaFieldOfView) {
        let layerSize = KSOptions.sceneSize
        let width = Double(layerSize.width / 2)
        [
            (modelViewProjectionMatrixLeft, PanoramaTextureEye.left, MTLViewport(originX: 0, originY: 0, width: width, height: Double(layerSize.height), znear: 0, zfar: 0)),
            (modelViewProjectionMatrixRight, PanoramaTextureEye.right, MTLViewport(originX: width, originY: 0, width: width, height: Double(layerSize.height), znear: 0, zfar: 0)),
        ].forEach { modelViewProjectionMatrix, eye, viewport in
            let mesh = super.set(encoder: encoder, panoramaStereoLayout: panoramaStereoLayout, panoramaFieldOfView: panoramaFieldOfView, eye: eye)
            encoder.setViewport(viewport)
            var matrix = modelViewProjectionMatrix * modelViewMatrix
            let matrixBuffer = MetalRender.device.makeBuffer(bytes: &matrix, length: MemoryLayout<simd_float4x4>.size)
            encoder.setVertexBuffer(matrixBuffer, offset: 0, index: 2)
            encoder.drawIndexedPrimitives(type: primitiveType, indexCount: mesh.indexCount, indexType: indexType, indexBuffer: mesh.indexBuffer, indexBufferOffset: 0)
        }
    }
}

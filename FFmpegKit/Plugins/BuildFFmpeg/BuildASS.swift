//
//  BuildASS.swift
//
//
//  Created by kintan on 12/26/23.
//

import Foundation

class BuildFribidi: BaseBuild {
    init() {
        super.init(library: .libfribidi)
    }

    override func arguments(platform _: PlatformType, arch _: ArchType) -> [String] {
        [
            "-Ddeprecated=false",
            "-Ddocs=false",
            "-Dtests=false",
        ]
    }
}

class BuildHarfbuzz: BaseBuild {
    init() {
        super.init(library: .libharfbuzz)
    }

    override func build(platform: PlatformType, arch: ArchType, buildURL: URL) throws {
        try super.build(platform: platform, arch: arch, buildURL: buildURL)
        let prefix = thinDir(platform: platform, arch: arch)
        let pcFile = prefix + "lib/pkgconfig/harfbuzz.pc"
        if let data = FileManager.default.contents(atPath: pcFile.path), var text = String(data: data, encoding: .utf8) {
            let freetype = thinDir(library: .libfreetype, platform: platform, arch: arch).path
            text = text.replacingOccurrences(of: "Requires: freetype2\n", with: "")
            text = text.replacingOccurrences(
                of: "Libs: -L${libdir} -lharfbuzz -lm\n",
                with: "Libs: -L${libdir} -lharfbuzz -L\(freetype)/lib -lfreetype -lz -lbz2 -lm\n"
            )
            text = text.replacingOccurrences(
                of: "Cflags: -I${includedir}/harfbuzz\n",
                with: "Cflags: -I${includedir}/harfbuzz -I\(freetype)/include/freetype2\n"
            )
            try text.write(to: pcFile, atomically: true, encoding: .utf8)
        }
    }

    override func cFlags(platform: PlatformType, arch: ArchType) -> [String] {
        var cFlags = super.cFlags(platform: platform, arch: arch)
        cFlags.append("-Wno-cast-function-type-strict")
        cFlags.append("-Wno-error=cast-function-type-strict")
        return cFlags
    }

    override func arguments(platform _: PlatformType, arch _: ArchType) -> [String] {
        [
            "-Dglib=disabled",
            "-Ddocs=disabled",
        ]
    }
}

class BuildFreetype: BaseBuild {
    init() {
        super.init(library: .libfreetype)
    }

    override func build(platform: PlatformType, arch: ArchType, buildURL: URL) throws {
        try super.build(platform: platform, arch: arch, buildURL: buildURL)
        let pcFile = thinDir(platform: platform, arch: arch) + "lib/pkgconfig/freetype2.pc"
        if let data = FileManager.default.contents(atPath: pcFile.path), var text = String(data: data, encoding: .utf8) {
            text = text.replacingOccurrences(of: "Requires: zlib\n", with: "")
            text = text.replacingOccurrences(of: "Libs: -L${libdir} -lfreetype -lbz2\n", with: "Libs: -L${libdir} -lfreetype -lz -lbz2\n")
            try text.write(to: pcFile, atomically: true, encoding: .utf8)
        }
    }

    override func arguments(platform _: PlatformType, arch _: ArchType) -> [String] {
        [
            "-Dbrotli=disabled",
            "-Dharfbuzz=disabled",
            "-Dpng=disabled",
        ]
    }
}

class BuildPng: BaseBuild {
    init() {
        super.init(library: .libpng)
    }

    override func arguments(platform _: PlatformType, arch _: ArchType) -> [String] {
        ["-DPNG_HARDWARE_OPTIMIZATIONS=yes"]
    }
}

class BuildUnibreak: BaseBuild {
    init() {
        super.init(library: .libunibreak)
    }

    override func arguments(platform: PlatformType, arch: ArchType) -> [String] {
        [
            "--disable-shared",
            "--enable-static",
            "--with-pic",
            "--host=\(platform.host(arch: arch))",
            "--prefix=\(thinDir(platform: platform, arch: arch).path)",
        ]
    }
}

class BuildASS: BaseBuild {
    init() {
        super.init(library: .libass)
    }

    override func flagsDependencelibrarys() -> [Library] {
        [.libunibreak]
    }

    override func build(platform: PlatformType, arch: ArchType, buildURL: URL) throws {
        try super.build(platform: platform, arch: arch, buildURL: buildURL)
        let prefix = thinDir(platform: platform, arch: arch)
        let pcFile = prefix + "lib/pkgconfig/libass.pc"
        if let data = FileManager.default.contents(atPath: pcFile.path), var text = String(data: data, encoding: .utf8) {
            let freetype = thinDir(library: .libfreetype, platform: platform, arch: arch).path
            let fribidi = thinDir(library: .libfribidi, platform: platform, arch: arch).path
            let harfbuzz = thinDir(library: .libharfbuzz, platform: platform, arch: arch).path
            let unibreak = thinDir(library: .libunibreak, platform: platform, arch: arch).path
            text = text.replacingOccurrences(of: "Requires: libunibreak >= 1.1, harfbuzz >= 1.2.3, fribidi >= 0.19.1, freetype2 >= 9.17.3\n", with: "")
            text = text.replacingOccurrences(
                of: "Libs: -L${libdir} -lass -liconv  -framework CoreText -framework CoreFoundation\n",
                with: "Libs: -L${libdir} -lass -L\(unibreak)/lib -lunibreak -L\(harfbuzz)/lib -lharfbuzz -L\(fribidi)/lib -lfribidi -L\(freetype)/lib -lfreetype -lz -lbz2 -liconv -framework CoreText -framework CoreFoundation -lm\n"
            )
            text = text.replacingOccurrences(
                of: "Cflags: -I${includedir}\n",
                with: "Cflags: -I${includedir} -I\(unibreak)/include -I\(harfbuzz)/include/harfbuzz -I\(fribidi)/include/fribidi -I\(freetype)/include/freetype2\n"
            )
            try text.write(to: pcFile, atomically: true, encoding: .utf8)
        }
    }

    override func arguments(platform: PlatformType, arch: ArchType) -> [String] {
        var result =
            [
                "--disable-libtool-lock",
                "--disable-fontconfig",
                "--disable-require-system-font-provider",
                "--disable-test",
                "--disable-profile",
                "--with-pic",
                "--enable-static",
                "--disable-shared",
                "--disable-fast-install",
                "--disable-dependency-tracking",
                "--host=\(platform.host(arch: arch))",
                "--prefix=\(thinDir(platform: platform, arch: arch).path)",
            ]
        if arch == .x86_64 {
            result.append("--enable-asm")
        }
        return result
    }
}

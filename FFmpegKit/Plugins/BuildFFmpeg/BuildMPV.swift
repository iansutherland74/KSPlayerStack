//
//  BuildMPV.swift
//
//
//  Created by kintan on 12/26/23.
//

import Foundation

class BuildMPV: BaseBuild {
    init() {
        super.init(library: .libmpv)
        let path = directoryURL + "meson.build"
        if let data = FileManager.default.contents(atPath: path.path), var str = String(data: data, encoding: .utf8) {
            str = str.replacingOccurrences(of: "# ffmpeg", with: """
            add_languages('objc')
            #ffmpeg
            """)
            str = str.replacingOccurrences(of: """
            subprocess_source = files('osdep/subprocess-posix.c')
            """, with: """
            if host_machine.subsystem() == 'tvos' or host_machine.subsystem() == 'tvos-simulator'
                subprocess_source = files('osdep/subprocess-dummy.c')
            else
                subprocess_source =files('osdep/subprocess-posix.c')
            endif
            """)
            str = str.replacingOccurrences(of: """
            if features['avfoundation']
                sources += files('audio/out/ao_coreaudio_properties.c')
            endif
            """, with: """
            if features['avfoundation'] and host_machine.subsystem() != 'xros' and host_machine.subsystem() != 'xros-simulator'
                sources += files('audio/out/ao_coreaudio_properties.c')
            endif
            """)
            try! str.write(toFile: path.path, atomically: true, encoding: .utf8)
        }
        patchVisionOSAudioOutput()
    }

    private func patchVisionOSAudioOutput() {
        replace(in: "audio/out/ao_coreaudio_utils.h", [
            ("#include <AudioToolbox/AudioToolbox.h>\n", "#include <AudioToolbox/AudioToolbox.h>\n#include <TargetConditionals.h>\n"),
            ("#if HAVE_COREAUDIO || HAVE_AVFOUNDATION\nOSStatus ca_select_device(struct ao *ao, char* name, AudioDeviceID *device);\n#endif\n", "#if (HAVE_COREAUDIO || HAVE_AVFOUNDATION) && !TARGET_OS_VISION\nOSStatus ca_select_device(struct ao *ao, char* name, AudioDeviceID *device);\n#endif\n"),
            ("#if HAVE_COREAUDIO || HAVE_AVFOUNDATION\nbool ca_stream_supports_compressed(struct ao *ao, AudioStreamID stream);\nOSStatus ca_lock_device(AudioDeviceID device, pid_t *pid);\nOSStatus ca_unlock_device(AudioDeviceID device, pid_t *pid);\nOSStatus ca_disable_mixing(struct ao *ao, AudioDeviceID device, bool *changed);\nOSStatus ca_enable_mixing(struct ao *ao, AudioDeviceID device, bool changed);\nint64_t ca_get_device_latency_ns(struct ao *ao, AudioDeviceID device);\nbool ca_change_physical_format_sync(struct ao *ao, AudioStreamID stream,\n                                    AudioStreamBasicDescription change_format);\n#endif\n", "#if (HAVE_COREAUDIO || HAVE_AVFOUNDATION) && !TARGET_OS_VISION\nbool ca_stream_supports_compressed(struct ao *ao, AudioStreamID stream);\nOSStatus ca_lock_device(AudioDeviceID device, pid_t *pid);\nOSStatus ca_unlock_device(AudioDeviceID device, pid_t *pid);\nOSStatus ca_disable_mixing(struct ao *ao, AudioDeviceID device, bool *changed);\nOSStatus ca_enable_mixing(struct ao *ao, AudioDeviceID device, bool changed);\nint64_t ca_get_device_latency_ns(struct ao *ao, AudioDeviceID device);\nbool ca_change_physical_format_sync(struct ao *ao, AudioStreamID stream,\n                                    AudioStreamBasicDescription change_format);\n#endif\n"),
        ])
        replace(in: "audio/out/ao_coreaudio_utils.c", [
            ("#include \"audio/out/ao_coreaudio_utils.h\"\n", "#include \"audio/out/ao_coreaudio_utils.h\"\n#include <TargetConditionals.h>\n"),
            ("#if HAVE_COREAUDIO || HAVE_AVFOUNDATION\n#include \"audio/out/ao_coreaudio_properties.h\"\n#include <CoreAudio/HostTime.h>\n#else\n#include <mach/mach_time.h>\n#endif\n", "#if (HAVE_COREAUDIO || HAVE_AVFOUNDATION) && !TARGET_OS_VISION\n#include \"audio/out/ao_coreaudio_properties.h\"\n#include <CoreAudio/HostTime.h>\n#else\n#include <mach/mach_time.h>\n#endif\n"),
            ("#if HAVE_COREAUDIO || HAVE_AVFOUNDATION\nstatic bool ca_is_output_device", "#if (HAVE_COREAUDIO || HAVE_AVFOUNDATION) && !TARGET_OS_VISION\nstatic bool ca_is_output_device"),
            ("#if HAVE_COREAUDIO || HAVE_AVFOUNDATION\n    uint64_t out = AudioConvertHostTimeToNanos(ts->mHostTime);\n    uint64_t now = AudioConvertHostTimeToNanos(AudioGetCurrentHostTime());", "#if (HAVE_COREAUDIO || HAVE_AVFOUNDATION) && !TARGET_OS_VISION\n    uint64_t out = AudioConvertHostTimeToNanos(ts->mHostTime);\n    uint64_t now = AudioConvertHostTimeToNanos(AudioGetCurrentHostTime());"),
            ("#if HAVE_COREAUDIO || HAVE_AVFOUNDATION\nbool ca_stream_supports_compressed", "#if (HAVE_COREAUDIO || HAVE_AVFOUNDATION) && !TARGET_OS_VISION\nbool ca_stream_supports_compressed"),
        ])
        replace(in: "audio/out/ao_coreaudio_chmap.h", [
            ("#include \"ao_coreaudio_utils.h\"\n", "#include \"ao_coreaudio_utils.h\"\n#include <TargetConditionals.h>\n"),
            ("bool ca_init_chmap(struct ao *ao, AudioDeviceID device);\nvoid ca_get_active_chmap(struct ao *ao, AudioDeviceID device, int channel_count,\n                         struct mp_chmap *out_map);\n", "#if !TARGET_OS_VISION\nbool ca_init_chmap(struct ao *ao, AudioDeviceID device);\nvoid ca_get_active_chmap(struct ao *ao, AudioDeviceID device, int channel_count,\n                         struct mp_chmap *out_map);\n#endif\n"),
        ])
        replace(in: "audio/out/ao_coreaudio_chmap.c", [
            ("#include <Availability.h>\n", "#include <Availability.h>\n#include <TargetConditionals.h>\n"),
            ("static AudioChannelLayout* ca_query_layout", "#if !TARGET_OS_VISION\nstatic AudioChannelLayout* ca_query_layout"),
            ("void ca_get_active_chmap(struct ao *ao, AudioDeviceID device, int channel_count,\n                         struct mp_chmap *out_map)\n{", "void ca_get_active_chmap(struct ao *ao, AudioDeviceID device, int channel_count,\n                         struct mp_chmap *out_map)\n{"),
            ("    MP_WARN(ao, \"mismatching channels - falling back to %s\\n\",\n            mp_chmap_to_str(out_map));\n}\n#endif\n", "    MP_WARN(ao, \"mismatching channels - falling back to %s\\n\",\n            mp_chmap_to_str(out_map));\n}\n#endif\n#endif\n"),
        ])
        replace(in: "audio/out/ao_avfoundation.m", [
            ("#import <AVFoundation/AVFoundation.h>\n", "#import <AVFoundation/AVFoundation.h>\n#import <TargetConditionals.h>\n"),
            ("    if (ao->device && ao->device[0]) {\n        [p->renderer setAudioOutputDeviceUniqueID:(NSString*)cfstr_from_cstr(ao->device)];\n    }\n", "    if (ao->device && ao->device[0]) {\n#if !TARGET_OS_VISION\n        [p->renderer setAudioOutputDeviceUniqueID:(NSString*)cfstr_from_cstr(ao->device)];\n#else\n        MP_WARN(ao, \"selecting an output device is unavailable on visionOS\\n\");\n#endif\n    }\n"),
        ])
    }

    private func replace(in relativePath: String, _ replacements: [(String, String)]) {
        let file = directoryURL + relativePath
        guard let data = FileManager.default.contents(atPath: file.path), var text = String(data: data, encoding: .utf8) else {
            return
        }
        for (old, new) in replacements {
            if text.contains(new) {
                continue
            }
            text = text.replacingOccurrences(of: old, with: new)
        }
        try! text.write(toFile: file.path, atomically: true, encoding: .utf8)
    }

    override func flagsDependencelibrarys() -> [Library] {
        [.gmp, .libsmbclient]
    }

    override func arguments(platform: PlatformType, arch: ArchType) -> [String] {
        var array = [
            "-Dlibmpv=true",
            "-Dgl=enabled",
            "-Dplain-gl=enabled",
            "-Diconv=enabled",
        ]
        if BaseBuild.disableGPL {
            array.append("-Dgpl=false")
        }
        if !(platform == .macos && arch.executable) {
            array.append("-Dcplayer=false")
        }
        if platform == .macos {
            array.append("-Dswift-flags=-sdk \(platform.isysroot) -target \(platform.deploymentTarget(arch: arch))")
            array.append("-Dcocoa=enabled")
            array.append("-Dcoreaudio=enabled")
            array.append("-Dgl-cocoa=enabled")
            array.append("-Dvideotoolbox-gl=enabled")
        } else {
            array.append("-Dvideotoolbox-gl=disabled")
            array.append("-Dswift-build=disabled")
            array.append("-Daudiounit=enabled")
            if platform == .maccatalyst {
                array.append("-Dcocoa=disabled")
                array.append("-Dcoreaudio=disabled")
            } else if platform == .xros || platform == .xrsimulator {
                array.append("-Dios-gl=disabled")
            } else {
                array.append("-Dios-gl=enabled")
            }
        }
        return array
    }
}

# Hardware Validation Run Template

Copy this file to `YYYY-MM-DD-device-os-build/RUN.md` for each real hardware
run. Keep the copied file with the logs and captures it describes. Redact serial
numbers, account identifiers, SSIDs, and private stream URLs before upload.

## Run Metadata

- Date:
- Tester:
- KSPlayer commit:
- App build/version:
- Device model:
- Device OS build:
- Renderer path: `KSAVPlayer` / `KSMEPlayer` / Vision Pro Metal 2D-to-3D
- Source media or stream:
- Codec, resolution, frame rate, bitrate:
- Network path:
- Output route: built-in / AirPlay / HDMI / Bluetooth / Spatial Audio
- External devices, receivers, displays, or headphones:

## Artifact Checklist

- `latency/`: KSPlayer low-latency diagnostics, startup timings, frame/drop
  counters, glass-to-glass measurements, and camera/server timing logs.
- `routes/`: `audioRouteDiagnostic` snapshots, AVAudioSession route changes,
  AirPlay/HDMI/Bluetooth/Spatial Audio observations, passthrough policy notes.
- `logs/`: app logs, crash reports, system logs, FFmpeg/RTSP/RTP logs, ONNX/Core
  ML inference timing logs, thermal/power notes.
- `captures/`: screenshots, high-speed camera clips, packet captures, short
  sample clips when license permits private storage.
- `models/`: exact model/runtime identifiers used for the run, including
  checksums and bundle paths. Prefer references to shared artifacts instead of
  duplicating large model files.

## Latency Notes

- Cold startup to first frame:
- Steady-state one-minute average:
- Worst observed spike:
- Reconnect behavior:
- Packet loss/jitter/RSSI/channel width:
- Thermal state and sustained FPS:

## Audio And Route Notes

- Route selected by the OS:
- Route selected by KSPlayer:
- Channel count reported:
- Spatial Audio capability reported:
- Atmos/AC-4/TrueHD passthrough behavior:
- Lip-sync or drift observations:

## Vision Pro 2D-to-3D Notes

- Depth runtime: Core ML / ONNX Runtime / pseudo-stereo fallback
- Model variant and checksum:
- Input size:
- Inference FPS target and observed timing:
- Depth normalization/inversion:
- Visual comfort notes:
- Dropped frame or renderer fallback notes:

## Result Summary

- Pass/fail:
- Blocking issues:
- Follow-up measurements required:
- Links to supporting files in this run folder:

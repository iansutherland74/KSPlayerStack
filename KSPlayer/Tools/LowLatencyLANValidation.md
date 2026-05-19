# Low-Latency LAN Live Validation

KSPlayer's `.lan` low-latency profile reduces local FFmpeg, queue, decode, and render buffering. It does not validate or guarantee sub-200ms glass-to-glass latency. Use this workflow to measure the complete camera/server/network/device path.

## Recommended Source Settings

Use `KSLowLatencyLiveProfile.lan.sourceRecommendations` in app diagnostics or test tooling to show the same recommendations at runtime.

- Encoder: H.264 hardware profile supported by the target device, or HEVC only after decode validation. Use IP-only/all-P GOPs, keep GOP duration at or below 0.5s, set B-frames to 0, disable lookahead, and prefer constrained bitrate over bursty VBR.
- Camera: disable extra image enhancement, cloud relay, long pre-record buffers, and multi-second jitter buffers when latency matters.
- RTSP: prefer UDP/RTP on trusted wired LAN; use TCP only when measured packet loss is worse than TCP head-of-line blocking. Disable large server pre-roll and jitter buffers. Keep RTP reorder queue at 0 unless loss/jitter measurements prove it is needed.
- RTP/UDP: use unicast on wired Ethernet first, keep socket/fifo buffers short, and expose RTCP sender reports or equivalent wall-clock timestamps for log correlation.

## Runtime Metrics To Capture

During each run, poll `player.dynamicInfo?.lowLatencyLiveDiagnostic` and persist the fields below alongside camera/server logs:

- Snapshot metrics: `bufferedDuration`, `packetCount`, `frameCount`, `droppedVideoFrameCount`, `droppedVideoPacketCount`, `audioVideoSyncDiff`, `displayFPS`, `audioLatencyEstimate`.
- Startup and pipeline timings: `timestamps`, `prepareToReadyDuration`, `openToReadyDuration`, `startupToFirstVideoFrameDuration`, `firstVideoReadToDecodeDuration`, `firstVideoDecodeToRenderDuration`, `firstVideoReadToRenderDuration`.
- Rolling local health: `rollingMetrics.bufferedDuration`, `rollingMetrics.absoluteAudioVideoSyncDiff`, `rollingMetrics.displayFPS`, `rollingMetrics.videoReadToRenderDuration`, and `rollingMetrics.audioLatencyEstimate`.

These are local player measurements. Pair them with camera capture timestamps, RTSP/RTP server ingress/egress timestamps, and a high-speed camera or LED/timecode glass-to-glass measurement.

## Repeatable Checklist

1. Record the device model, OS version, KSPlayer commit, camera model, codec, resolution, frame rate, bitrate, GOP duration, B-frame count, transport, RTSP/RTP server, network path, and audio route.
2. Start with wired Ethernet and a single client. Measure cold startup, one steady-state minute, and a reconnect.
3. Repeat over the intended Wi-Fi path. Capture packet loss, jitter, RSSI/channel width, and whether TCP fallback was needed.
4. For each run, collect KSPlayer diagnostics once per second and capture server logs with RTP/RTCP timestamps enabled.
5. Measure glass-to-glass with a visible timecode or LED driven at the camera source and a high-speed recording of the playback display.
6. Compare local KSPlayer timings with server/camera timings. If local buffers are low but glass-to-glass is high, investigate encoder GOP/lookahead, camera buffering, server jitter buffers, or network retransmission.
7. Do not publish a latency target until the actual camera, server, network, device, display, and audio route have passed the measurement above.

## Minimal Diagnostic Polling Sketch

```swift
Timer.scheduledTimer(withTimeInterval: 1, repeats: true) { _ in
    guard let diagnostic = player.dynamicInfo?.lowLatencyLiveDiagnostic else { return }
    print("buffer=\(diagnostic.bufferedDuration)s")
    print("drops video/frame=\(diagnostic.droppedVideoFrameCount) packet=\(diagnostic.droppedVideoPacketCount)")
    print("startup first video=\(String(describing: diagnostic.startupToFirstVideoFrameDuration))")
    print("rolling buffer avg=\(String(describing: diagnostic.rollingMetrics.bufferedDuration.average))")
}
```

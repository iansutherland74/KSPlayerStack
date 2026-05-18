#!/usr/bin/env bash
# Resume FFmpeg 8.1.1 build from harfbuzz. KSPlayerStack only.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
FFMPEGKIT="$ROOT/FFmpegKit"
LOG="$FFMPEGKIT/build-ffmpeg-8.1.1.log"
cd "$FFMPEGKIT"

BUILD=(swift package --disable-sandbox BuildFFmpeg platforms=xros,xrsimulator notRecompile)

run_step() {
  echo "==> $*"
  echo "==> $*" >>"$LOG"
  "${BUILD[@]}" "$@" >>"$LOG" 2>&1
}

prepare_ffmpeg_rebuild() {
  echo "==> Preparing clean FFmpeg 8.1.1 rebuild" | tee -a "$LOG"
  rm -rf .Script/FFmpeg .Script/FFmpeg-n8.1 .Script/FFmpeg-n8.1.1

  # SwiftPM must be able to load Package.swift before the BuildFFmpeg plugin can
  # replace these generated binary targets.
  for name in Libavcodec Libavdevice Libavfilter Libavformat Libavutil Libswresample Libswscale; do
    if [ ! -d "Sources/${name}.xcframework" ]; then
      cp -R Sources/lcms2.xcframework "Sources/${name}.xcframework"
    fi
  done
}

: >>"$LOG"
run_step enable-libharfbuzz
run_step enable-libass
run_step enable-libfontconfig
run_step enable-libzvbi
run_step enable-libsrt

prepare_ffmpeg_rebuild
run_step enable-FFmpeg

grep -r FFMPEG_VERSION Sources/Libavutil.xcframework/*/Libavutil.framework/Headers/ffversion.h 2>/dev/null | head -3 || true

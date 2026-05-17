#!/usr/bin/env bash
# Resume FFmpeg 8.1 build from gnutls (after nettle/gmp). KSPlayerStack only.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
FFMPEGKIT="$ROOT/FFmpegKit"
LOG="$FFMPEGKIT/build-ffmpeg-8.1.log"
cd "$FFMPEGKIT"

BUILD=(swift package --disable-sandbox BuildFFmpeg disableGPL platforms=xros,xrsimulator notRecompile)

run_step() {
  echo "==> $*"
  echo "==> $*" >>"$LOG"
  "${BUILD[@]}" "$@" >>"$LOG" 2>&1
}

: >>"$LOG"
run_step enable-gnutls
run_step enable-libdav1d
run_step enable-libfreetype enable-libfribidi enable-libharfbuzz enable-libass
run_step enable-libzvbi
run_step enable-libsrt

echo "==> Removing Libav* 6.1 xcframeworks" | tee -a "$LOG"
rm -rf Sources/Libavcodec.xcframework Sources/Libavdevice.xcframework Sources/Libavfilter.xcframework \
       Sources/Libavformat.xcframework Sources/Libavutil.xcframework Sources/Libswresample.xcframework \
       Sources/Libswscale.xcframework

run_step enable-FFmpeg

grep -r FFMPEG_VERSION Sources/Libavutil.xcframework/*/Libavutil.framework/Headers/ffversion.h 2>/dev/null | head -3 || true

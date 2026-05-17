#!/usr/bin/env bash
# B1 visionOS build: FFmpeg 8.1 via kingslay BuildFFmpeg plugin.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
FFMPEGKIT="$ROOT/FFmpegKit"
LOG="$FFMPEGKIT/build-ffmpeg-8.1.log"

cd "$FFMPEGKIT"

if ! command -v brew >/dev/null; then
  echo "Homebrew required." >&2
  exit 1
fi

for tool in pkg-config nasm cmake meson; do
  if ! command -v "$tool" >/dev/null; then
    brew install "$tool"
  fi
done
brew list sdl2 >/dev/null 2>&1 || brew install sdl2

echo "==> FFmpegKit at $FFMPEGKIT"
echo "==> Logging to $LOG"
# Keep Libav* until enable-FFmpeg so `swift package` can load Package.swift.

BUILD=(swift package --disable-sandbox BuildFFmpeg disableGPL platforms=xros,xrsimulator)
COMMON=(notRecompile)

run_step() {
  echo ""
  echo "==> $*"
  echo "==> $*" >>"$LOG"
  "${BUILD[@]}" "${COMMON[@]}" "$@" >>"$LOG" 2>&1
}

: >"$LOG"

# Force re-clone after adding patches under Plugins/BuildFFmpeg/patch/
rm -rf "$FFMPEGKIT/.Script/gmp-"* "$FFMPEGKIT/.Script/nettle-"* 2>/dev/null || true

run_step enable-gmp
run_step enable-nettle
run_step enable-gnutls
run_step enable-libdav1d
run_step enable-libfreetype enable-libfribidi enable-libharfbuzz enable-libass
run_step enable-libzvbi
run_step enable-libsrt

echo "==> Removing Libav* 6.1 xcframeworks before FFmpeg 8.1 build" | tee -a "$LOG"
rm -rf Sources/Libavcodec.xcframework \
       Sources/Libavdevice.xcframework \
       Sources/Libavfilter.xcframework \
       Sources/Libavformat.xcframework \
       Sources/Libavutil.xcframework \
       Sources/Libswresample.xcframework \
       Sources/Libswscale.xcframework

run_step enable-FFmpeg

echo ""
echo "Done. Check ffversion.h:"
find Sources/Libavutil.xcframework -name ffversion.h 2>/dev/null | head -1 | xargs grep FFMPEG_VERSION || true

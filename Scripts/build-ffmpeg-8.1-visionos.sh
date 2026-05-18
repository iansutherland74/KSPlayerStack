#!/usr/bin/env bash
# B1 visionOS build: FFmpeg 8.1.1 via kingslay BuildFFmpeg plugin.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
FFMPEGKIT="$ROOT/FFmpegKit"
LOG="$FFMPEGKIT/build-ffmpeg-8.1.1.log"

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

BUILD=(swift package --disable-sandbox BuildFFmpeg platforms=xros,xrsimulator)
COMMON=(notRecompile)

run_step() {
  echo ""
  echo "==> $*"
  echo "==> $*" >>"$LOG"
  "${BUILD[@]}" "${COMMON[@]}" "$@" >>"$LOG" 2>&1
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

: >"$LOG"

# Force re-clone after adding patches under Plugins/BuildFFmpeg/patch/
rm -rf "$FFMPEGKIT/.Script/gmp-"* \
       "$FFMPEGKIT/.Script/nettle-"* \
       "$FFMPEGKIT/.Script/gnutls-"* \
       "$FFMPEGKIT/.Script/libsmbclient-"* \
       "$FFMPEGKIT/.Script/libharfbuzz-"* \
       "$FFMPEGKIT/.Script/libudfread-"* \
       "$FFMPEGKIT/.Script/libbluray-"* 2>/dev/null || true

run_step enable-gmp
run_step enable-nettle
run_step enable-gnutls
run_step enable-readline
run_step enable-libsmbclient
run_step enable-libshaderc
run_step enable-vulkan
run_step enable-lcms2
run_step enable-libdav1d
run_step enable-libplacebo
run_step enable-libfreetype enable-libfribidi enable-libharfbuzz enable-libass
run_step enable-libfontconfig
run_step enable-libudfread
run_step enable-libbluray
run_step enable-libzvbi
run_step enable-libsrt

prepare_ffmpeg_rebuild
run_step enable-FFmpeg

echo ""
echo "Done. Check ffversion.h:"
find Sources/Libavutil.xcframework -name ffversion.h 2>/dev/null | head -1 | xargs grep FFMPEG_VERSION || true

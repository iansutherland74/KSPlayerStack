#!/usr/bin/env bash
# Profile depth/video on a paired Apple Vision Pro over Wi‑Fi or USB.
# Usage:
#   ./benchmark-visionpro-device.sh cineultra   # com.cinemax.player / CineUltra
#   ./benchmark-visionpro-device.sh ksplayer    # KSPlayer DA3 Demo
#
# Before running: Developer Mode on headset, same Wi‑Fi as Mac, device paired in Xcode.
# During the 90s capture: launch the app, play a test clip, enable 3D / immersive.

set -euo pipefail

TARGET="${1:-ksplayer}"
DURATION="${2:-90s}"
OUT_DIR="${OUT_DIR:-$HOME/Desktop/visionpro-benchmark-$(date +%Y%m%d-%H%M%S)}"
mkdir -p "$OUT_DIR"

echo "Finding Vision Pro…"
DEVICE_ID="$(xcrun xctrace list devices 2>/dev/null | grep -i 'Vision Pro' | grep -v Simulator | head -1 | sed -n 's/.*(\([0-9A-Fa-f-]\{24,\}\)).*/\1/p')"
if [[ -z "$DEVICE_ID" ]]; then
  echo "No physical Vision Pro in 'xcrun xctrace list devices'. Pair in Xcode → Devices and Simulators."
  exit 1
fi
echo "Device: $DEVICE_ID"

case "$TARGET" in
  cineultra|cine|cinemax)
    BUNDLE="com.cinemax.player"
    LABEL="CineUltra"
    ;;
  ksplayer|da3|demo)
    BUNDLE="com.iansutherland.KSPlayer.VisionProDepthAnythingV3"
    LABEL="KSPlayer_DA3_Demo"
    ;;
  *)
    echo "Unknown target: $TARGET (use cineultra or ksplayer)"
    exit 1
    ;;
esac

echo "Checking $LABEL ($BUNDLE) is installed…"
if ! xcrun devicectl device info apps --device "$DEVICE_ID" 2>/dev/null | grep -q "$BUNDLE"; then
  echo "ERROR: $LABEL is not installed on this Vision Pro."
  echo "Install from App Store on the headset, then re-run."
  xcrun devicectl device info apps --device "$DEVICE_ID" 2>/dev/null | head -20 || true
  exit 1
fi

echo ""
echo "=== NEXT: on the headset (next $DURATION) ==="
echo "  1. Open $LABEL"
echo "  2. Play your standard test video"
echo "  3. Turn ON 2D→3D / immersive mode"
echo "  4. Leave it running"
echo ""
read -r -p "Press Enter when ready to start recording…"

COREML_TRACE="$OUT_DIR/${LABEL}_CoreML.trace"
METAL_TRACE="$OUT_DIR/${LABEL}_Metal.trace"
LOG_FILE="$OUT_DIR/${LABEL}_console.log"

echo "Recording Core ML ($DURATION)…"
xcrun xctrace record \
  --device "$DEVICE_ID" \
  --template "Core ML" \
  --append-run \
  --output "$COREML_TRACE" \
  --time-limit "$DURATION" \
  --launch "$BUNDLE" \
  2>&1 | tee "$OUT_DIR/coreml-record.log" || true

echo "Recording Metal ($DURATION) — if app closed, reopen on headset…"
read -r -p "Press Enter when $LABEL is playing 3D again…"
xcrun xctrace record \
  --device "$DEVICE_ID" \
  --template "Metal System Trace" \
  --append-run \
  --output "$METAL_TRACE" \
  --time-limit "$DURATION" \
  --attach "$BUNDLE" \
  2>&1 | tee "$OUT_DIR/metal-record.log" || true

echo "Streaming device logs (30s)…"
timeout 30 xcrun devicectl device console --device "$DEVICE_ID" 2>&1 | tee "$LOG_FILE" || true

echo ""
echo "Done. Open traces in Instruments:"
echo "  $COREML_TRACE"
echo "  $METAL_TRACE"
echo "  $LOG_FILE"
echo ""
echo "In Core ML trace note: median prediction time (ms), compute units (ANE vs CPU/GPU), predictions/sec."

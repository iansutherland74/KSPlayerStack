#!/usr/bin/env bash
# Capture runtime signals from CineUltra on a paired Apple Vision Pro.
#
# Prerequisites:
#   - Vision Pro paired in Xcode (Devices and Simulators)
#   - Developer Mode on the headset
#   - CineUltra installed from the App Store (com.cinemax.player)
#
# Usage:
#   ./capture-cineultra-live.sh logs          # 60s filtered log collect
#   ./capture-cineultra-live.sh metal         # 60s Metal System Trace (attach)
#   ./capture-cineultra-live.sh coreml        # 60s Core ML trace (attach)
#   ./capture-cineultra-live.sh container     # try app container copy (if allowed)
#   ./capture-cineultra-live.sh all           # logs + metal (interactive)
#
# While capturing: open CineUltra, play video, enable 2D→3D / immersive.

set -euo pipefail

MODE="${1:-logs}"
DURATION="${2:-60s}"
BUNDLE="com.cinemax.player"
LABEL="CineUltra"
OUT_DIR="${OUT_DIR:-$HOME/Desktop/cineultra-live-$(date +%Y%m%d-%H%M%S)}"
mkdir -p "$OUT_DIR"

find_device() {
  xcrun xctrace list devices 2>/dev/null \
    | grep -i 'Vision Pro' \
    | grep -v Simulator \
    | head -1 \
    | sed -n 's/.*(\([0-9A-Fa-f-]\{24,\}\)).*/\1/p'
}

DEVICE_ID="$(find_device)"
if [[ -z "$DEVICE_ID" ]]; then
  echo "No physical Vision Pro found. Pair the headset in Xcode → Window → Devices and Simulators."
  exit 1
fi
echo "Vision Pro: $DEVICE_ID"
echo "Output: $OUT_DIR"
echo ""

if ! xcrun devicectl device info apps --device "$DEVICE_ID" 2>/dev/null | grep -q "$BUNDLE"; then
  echo "ERROR: $LABEL ($BUNDLE) is not installed on this headset."
  exit 1
fi

echo "On the headset: open CineUltra, start playback, enable 3D/immersive."
read -r -p "Press Enter when CineUltra is playing in 3D…"

record_metal() {
  local out="$OUT_DIR/${LABEL}_Metal.trace"
  echo "Recording Metal System Trace ($DURATION)…"
  xcrun xctrace record \
    --device "$DEVICE_ID" \
    --template "Metal System Trace" \
    --append-run \
    --output "$out" \
    --time-limit "$DURATION" \
    --attach "$BUNDLE" \
    2>&1 | tee "$OUT_DIR/metal-record.log" || true
  echo "  → $out"
}

record_coreml() {
  local out="$OUT_DIR/${LABEL}_CoreML.trace"
  echo "Recording Core ML ($DURATION)…"
  xcrun xctrace record \
    --device "$DEVICE_ID" \
    --template "Core ML" \
    --append-run \
    --output "$out" \
    --time-limit "$DURATION" \
    --attach "$BUNDLE" \
    2>&1 | tee "$OUT_DIR/coreml-record.log" || true
  echo "  → $out"
}

collect_logs() {
  local archive="$OUT_DIR/${LABEL}_device.logarchive"
  echo "Collecting last 2 minutes of device logs (filtered)…"
  # Predicate: CineUltra process, Metal, Core ML, CompositorServices, RealityKit
  log collect \
    --device-udid "$DEVICE_ID" \
    --last 2m \
    --size 50m \
    --predicate 'subsystem CONTAINS "cinemax" OR subsystem CONTAINS "Cine" OR process CONTAINS "CineUltra" OR subsystem CONTAINS "metal" OR subsystem CONTAINS "coreml" OR subsystem CONTAINS "compositor" OR subsystem CONTAINS "RealityKit" OR category CONTAINS "Metal"' \
    --output "$archive" \
    2>&1 | tee "$OUT_DIR/log-collect.log" || true
  echo "  → $archive (open in Console.app)"
  if [[ -d "$archive" ]]; then
    log show "$archive" --info --debug 2>/dev/null \
      | head -500 \
      > "$OUT_DIR/${LABEL}_log_snippet.txt" || true
    echo "  → $OUT_DIR/${LABEL}_log_snippet.txt (first 500 lines)"
  fi
}

try_container_copy() {
  local dest="$OUT_DIR/app_container"
  mkdir -p "$dest"
  echo "Attempting app container copy (requires file-sharing / developer access)…"
  if xcrun devicectl device copy from \
    --device "$DEVICE_ID" \
    --domain-type appDataContainer \
    --domain-identifier "$BUNDLE" \
    --source "." \
    --destination "$dest" \
    2>&1 | tee "$OUT_DIR/container-copy.log"; then
    echo "  → $dest"
    du -sh "$dest" 2>/dev/null || true
  else
    echo "  Container copy failed (normal for many App Store apps). See container-copy.log"
  fi
}

case "$MODE" in
  logs|log)
    collect_logs
    ;;
  metal)
    record_metal
    ;;
  coreml|core-ml)
    record_coreml
    ;;
  container|files)
    try_container_copy
    ;;
  all)
    collect_logs
    record_metal
    ;;
  *)
    echo "Unknown mode: $MODE"
    echo "Use: logs | metal | coreml | container | all"
    exit 1
    ;;
esac

cat <<EOF

Done.

What you can learn from a running App Store app:
  • Metal trace: GPU workload, command buffers, shader pipeline activity (stereo/depth timing).
  • Core ML trace: prediction rate, ANE vs GPU, model invocation frequency.
  • Logs: only if the app/frameworks emit os_log (often sparse in release builds).
  • Container: settings/cache only if copy succeeds (not the encrypted .mlmodelc in RAM).

What you cannot pull from a stock headset:
  • Decrypted app binary, Swift source, or the encrypted depth model weights.
  • Arbitrary memory dumps without a jailbreak / special entitlements.

Open traces in Instruments.app. Compare with KSPlayer using:
  ./benchmark-visionpro-device.sh ksplayer

EOF

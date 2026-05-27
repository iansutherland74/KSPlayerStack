#!/usr/bin/env python3
"""Template converter for app-owned Depth Anything Core ML exports.

This script intentionally does not download model weights. Point it at a
TorchScript or torch.export artifact that your app team has already produced
from Depth Anything V2/V3, then validate the generated model on Vision Pro.
coremltools 9 converts PyTorch programs; it does not provide a direct ONNX
converter path.

Vision Pro performance checklist (DA3):
- Export **DA3-Small** only; Base/Large will not hit 24–60 depth fps on device.
- Strip the **ray-map / pose head** in PyTorch before export — single-view stereo
  reprojection only needs relative depth (roughly halves decoder work).
- Convert with **FP16** weights (`--compute-precision` default) and
  `--compute-unit cpuAndNeuralEngine`.
- Profile the compiled `.mlmodelc` in Xcode Instruments (Core ML template). If
  transformer blocks show CPU, the graph has ANE-incompatible ops (attention /
  custom fusion) and will stall around ~5 fps until the export is fixed.
- For immediate smooth playback while retuning DA3, bundle Apple's optimized
  Depth Anything V2 Small Core ML model and use KSPlayer's
  `DepthAnythingV2DepthEstimationAdapter` (see demo README).
"""

from __future__ import annotations

import argparse
import pathlib
import subprocess
import sys


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Convert an app-owned Depth Anything PyTorch export to a Core ML package."
    )
    parser.add_argument("source_model", type=pathlib.Path, help="TorchScript .pt/.pth or torch.export artifact")
    parser.add_argument("output_model", type=pathlib.Path, help="Output .mlpackage or .mlmodel path")
    parser.add_argument("--source-format", choices=("torchscript", "exportedprogram"), default="torchscript")
    parser.add_argument("--input-name", default="image", help="Core ML input feature name")
    parser.add_argument("--output-name", default="depth", help="Rename the first model output to this feature")
    parser.add_argument("--width", type=int, default=384, help="Model input width")
    parser.add_argument("--height", type=int, default=216, help="Model input height")
    parser.add_argument(
        "--input-kind",
        choices=("multiarray", "image"),
        default="multiarray",
        help="Use multiarray for the DA3 Small 1x1x3xHxW wrapper, or image for Core ML image-input wrappers.",
    )
    parser.add_argument("--image-scale", type=float, default=1.0 / 255.0, help="Core ML image scale")
    parser.add_argument(
        "--image-bias",
        type=float,
        nargs=3,
        default=(0.0, 0.0, 0.0),
        metavar=("R", "G", "B"),
        help="Core ML RGB image bias. Put DA mean/std preprocessing in the exported model when per-channel scale is required.",
    )
    parser.add_argument(
        "--compute-unit",
        choices=("all", "cpuOnly", "cpuAndGPU", "cpuAndNeuralEngine"),
        default="cpuAndNeuralEngine",
        help="Compute unit hint used when saving/checking the model (ANE-preferred on Apple Silicon).",
    )
    parser.add_argument(
        "--compute-precision",
        choices=("FLOAT16", "FLOAT32"),
        default="FLOAT16",
        help="Weight/activation precision for the mlprogram (FP16 is ANE-friendly).",
    )
    parser.add_argument(
        "--palettize-weights",
        action="store_true",
        help="Apply coremltools weight palettization after conversion (optional extra compression).",
    )
    parser.add_argument(
        "--minimum-deployment-target",
        default=None,
        help="Optional coremltools target symbol, for example iOS17. Leave unset for coremltools default.",
    )
    parser.add_argument("--compile", action="store_true", help="Run xcrun coremlcompiler compile after conversion")
    return parser.parse_args()


def load_source_model(args: argparse.Namespace):
    import torch

    if args.source_format == "exportedprogram":
        try:
            return torch.export.load(args.source_model)
        except AttributeError as error:
            raise RuntimeError("This PyTorch install does not expose torch.export.load.") from error
    return torch.jit.load(args.source_model, map_location="cpu").eval()


def compute_unit(coremltools_module, name: str):
    mapping = {
        "all": coremltools_module.ComputeUnit.ALL,
        "cpuOnly": coremltools_module.ComputeUnit.CPU_ONLY,
        "cpuAndGPU": coremltools_module.ComputeUnit.CPU_AND_GPU,
        "cpuAndNeuralEngine": coremltools_module.ComputeUnit.CPU_AND_NE,
    }
    return mapping[name]


def deployment_target(coremltools_module, name: str | None):
    if name is None:
        return None
    try:
        return getattr(coremltools_module.target, name)
    except AttributeError as error:
        raise RuntimeError(f"Unknown coremltools deployment target: {name}") from error


def convert_model(args: argparse.Namespace) -> None:
    import coremltools as ct
    import numpy as np

    source = load_source_model(args)
    target = deployment_target(ct, args.minimum_deployment_target)
    if args.input_kind == "multiarray":
        inputs = [
            ct.TensorType(
                name=args.input_name,
                shape=(1, 1, 3, args.height, args.width),
                dtype=np.float16,
            )
        ]
    else:
        inputs = [
            ct.ImageType(
                name=args.input_name,
                shape=(1, 3, args.height, args.width),
                scale=args.image_scale,
                bias=list(args.image_bias),
                color_layout=ct.colorlayout.RGB,
            )
        ]
    precision = ct.precision.FLOAT16 if args.compute_precision == "FLOAT16" else ct.precision.FLOAT32
    convert_options = {
        "source": "pytorch",
        "inputs": inputs,
        "compute_units": compute_unit(ct, args.compute_unit),
        "compute_precision": precision,
        "convert_to": "mlprogram",
    }
    if target is not None:
        convert_options["minimum_deployment_target"] = target

    model = ct.convert(source, **convert_options)
    spec = model.get_spec()
    if args.output_name and spec.description.output:
        original_output = spec.description.output[0].name
        if original_output != args.output_name:
            ct.utils.rename_feature(spec, original_output, args.output_name)
            model = ct.models.MLModel(spec, compute_units=compute_unit(ct, args.compute_unit))

    args.output_model.parent.mkdir(parents=True, exist_ok=True)
    if args.palettize_weights:
        try:
            from coremltools.optimize.coreml import (
                OpPalettizerConfig,
                OptimizationConfig,
                palettize_weights,
            )

            palettizer_config = OpPalettizerConfig(mode="uniform", nbits=8)
            model = palettize_weights(model, config=OptimizationConfig(global_config=palettizer_config))
            print("Applied 8-bit weight palettization")
        except Exception as error:
            print(f"Palettization skipped: {error}", file=sys.stderr)
    model.save(args.output_model)


def compile_model(args: argparse.Namespace) -> None:
    compiled_dir = args.output_model.with_suffix(".mlmodelc")
    subprocess.run(
        ["xcrun", "coremlcompiler", "compile", str(args.output_model), str(compiled_dir.parent)],
        check=True,
    )
    print(f"Compiled model: {compiled_dir}")


def main() -> int:
    args = parse_args()
    if not args.source_model.exists():
        print(f"Source model does not exist: {args.source_model}", file=sys.stderr)
        return 2
    convert_model(args)
    print(f"Saved Core ML model: {args.output_model}")
    if args.compile:
        compile_model(args)
    print("Validate input/output names, depth direction, ANE/GPU placement, and frame time on Vision Pro before shipping.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

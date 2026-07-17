#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.core import Harness


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Generic project-adapted LLM experiment harness")
    parser.add_argument("--config", type=Path, default=Path("llm-harness.json"))
    sub = parser.add_subparsers(dest="command", required=True)
    for name in ("new", "improve", "tu-complete"):
        command = sub.add_parser(name)
        command.add_argument(
            "target_id",
            nargs="?",
            help="Target ID, or unit name for tu-complete",
        )
        if name in {"new", "improve", "tu-complete"}:
            command.add_argument(
                "--number",
                type=int,
                help=f"Automatically select this many {name} targets",
            )
        if name in {"improve", "tu-complete"}:
            command.add_argument(
                "--random",
                action="store_true",
                help="Shuffle eligible targets before applying --number",
            )
        if name in {"new", "improve"}:
            command.add_argument(
                "--certified-funcs",
                action="store_true",
                help="Only select functions whose called functions are FULL_MATCH or EQUIVALENT+certified",
            )
        if name == "new":
            command.add_argument(
                "--ignore-called-functions",
                action="store_true",
                help="Allow automatic selection of functions before their callees are matched",
            )
        command.add_argument("--runs", type=int)
        command.add_argument("--dry-run", action="store_true")
        command.add_argument("--resume", type=Path, help="Resume an experiment directory")
        command.add_argument("--max-cost", type=float, help="Stop before the next call after this recorded cost")
        command.add_argument("--max-tokens", type=int, help="Stop before the next call after this token total")
        command.add_argument("--max-parallel", type=int, help="Override configured parallel candidate count")
        command.add_argument("--retry-errors", action="store_true", help="On resume, rerun model/run records with errors")
        if name == "tu-complete":
            command.add_argument(
                "--full-context",
                action="store_true",
                help="Opt in to complete-TU input/output instead of bounded TU slots",
            )
    batch = sub.add_parser(
        "batch", help="Run a workflow for multiple functions or TUs concurrently"
    )
    batch.add_argument("workflow", choices=("new", "improve", "tu-complete"))
    batch.add_argument("target_ids", nargs="+")
    batch.add_argument("--runs", type=int)
    batch.add_argument("--dry-run", action="store_true")
    batch.add_argument("--full-context", action="store_true")
    batch.add_argument(
        "--max-target-parallel",
        type=int,
        help="Maximum functions active at once",
    )
    batch.add_argument(
        "--model-parallel",
        type=int,
        help="Maximum model/run calls active within each function",
    )
    prepare = sub.add_parser("prepare", help="Preview or add markers around an existing target function")
    prepare.add_argument("target_id")
    prepare.add_argument("--write", action="store_true")
    slot = sub.add_parser("slot", help="Preview or insert an empty marker slot at an exact anchor")
    slot.add_argument("target_id")
    slot.add_argument("--file", type=Path, required=True)
    anchor = slot.add_mutually_exclusive_group(required=True)
    anchor.add_argument("--before")
    anchor.add_argument("--after")
    slot.add_argument("--unit", default="", help="Objdiff unit; required for unassigned targets")
    slot.add_argument("--write", action="store_true")
    tu_slot = sub.add_parser(
        "tu-slot", help="Preview or add a bounded source slot for TU completion"
    )
    tu_slot.add_argument("unit")
    tu_slot.add_argument("slot_id")
    tu_slot.add_argument("--file", type=Path, required=True)
    tu_slot.add_argument("--before", help="Insert an empty slot before this exact anchor")
    tu_slot.add_argument("--after", help="Insert an empty slot after this exact anchor")
    tu_slot.add_argument("--start", help="Wrap a region beginning at this exact anchor")
    tu_slot.add_argument("--end", help="Wrap a region ending with this exact anchor")
    tu_slot.add_argument("--write", action="store_true")
    promote = sub.add_parser("promote", help="Preview or apply an experiment's best candidate")
    promote.add_argument("experiment", type=Path)
    promote.add_argument("--write", action="store_true")
    rescore = sub.add_parser("rescore", help="Re-evaluate saved candidates without new model calls")
    rescore.add_argument("experiment", type=Path)
    rescore.add_argument("--max-parallel", type=int)
    sub.add_parser("stats")
    args = parser.parse_args(argv)
    harness = Harness(args.config)
    if args.command == "stats":
        print(json.dumps(harness.stats(), indent=2))
        return 0
    if args.command == "batch":
        print(harness.run_batch(
            args.workflow,
            args.target_ids,
            runs=args.runs,
            dry_run=args.dry_run,
            max_target_parallel=args.max_target_parallel,
            model_parallel=args.model_parallel,
            full_context=args.full_context,
        ))
        return 0
    if args.command == "prepare":
        prepare_fn = getattr(harness.adapter, "prepare", None)
        if prepare_fn is None:
            parser.error("Configured project adapter does not support prepare")
        print(prepare_fn(args.target_id, write=args.write))
        return 0
    if args.command == "slot":
        slot_fn = getattr(harness.adapter, "create_slot", None)
        if slot_fn is None:
            parser.error("Configured project adapter does not support marker slots")
        print(slot_fn(
            args.target_id,
            args.file,
            before=args.before or "",
            after=args.after or "",
            unit=args.unit,
            write=args.write,
        ))
        return 0
    if args.command == "tu-slot":
        slot_fn = getattr(harness.adapter, "create_tu_slot", None)
        if slot_fn is None:
            parser.error("Configured project adapter does not support TU marker slots")
        print(slot_fn(
            args.unit,
            args.slot_id,
            args.file,
            before=args.before or "",
            after=args.after or "",
            start=args.start or "",
            end=args.end or "",
            write=args.write,
        ))
        return 0
    if args.command == "promote":
        print(harness.promote(args.experiment, write=args.write))
        return 0
    if args.command == "rescore":
        print(harness.rescore(args.experiment, max_parallel=args.max_parallel))
        return 0
    if args.command in {"new", "improve", "tu-complete"} and args.number is not None:
        if args.target_id is not None:
            parser.error(f"{args.command} accepts either target_id or --number, not both")
        if args.number < 1:
            parser.error("--number must be positive")
        if args.resume or args.max_cost is not None or args.max_tokens is not None or args.retry_errors:
            parser.error(
                f"{args.command} --number does not support --resume, --max-cost, "
                "--max-tokens, or --retry-errors"
            )
        try:
            if args.command == "new":
                target_ids = harness.select_new_targets(
                    args.number,
                    ignore_called_functions=args.ignore_called_functions,
                    certified_funcs=args.certified_funcs,
                )
            else:
                target_ids = harness.select_targets(
                    args.command, args.number,
                    randomize=args.random,
                    certified_funcs=args.certified_funcs,
                )
        except (TypeError, ValueError) as exc:
            parser.error(str(exc))
        print(harness.run_batch(
            args.command,
            target_ids,
            runs=args.runs,
            dry_run=args.dry_run,
            model_parallel=args.max_parallel,
            full_context=getattr(args, "full_context", False),
        ))
        return 0
    if getattr(args, "random", False):
        parser.error("--random requires --number")
    if args.target_id is None:
        parser.error(f"{args.command} requires target_id or --number")
    output = harness.run(
        args.command,
        args.target_id,
        runs=args.runs,
        dry_run=args.dry_run,
        resume=args.resume,
        max_cost=args.max_cost,
        max_tokens=args.max_tokens,
        max_parallel=args.max_parallel,
        retry_errors=args.retry_errors,
        full_context=getattr(args, "full_context", False),
    )
    print(output)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

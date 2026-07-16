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
        command.add_argument("target_id", help="Target ID, or unit name for tu-complete")
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
    prepare = sub.add_parser("prepare", help="Preview or add markers around an existing target function")
    prepare.add_argument("target_id")
    prepare.add_argument("--write", action="store_true")
    prepare.add_argument("--owner", default="")
    slot = sub.add_parser("slot", help="Preview or insert an empty marker slot at an exact anchor")
    slot.add_argument("target_id")
    slot.add_argument("--file", type=Path, required=True)
    anchor = slot.add_mutually_exclusive_group(required=True)
    anchor.add_argument("--before")
    anchor.add_argument("--after")
    slot.add_argument("--unit", default="", help="Objdiff unit; required for unassigned targets")
    slot.add_argument("--write", action="store_true")
    slot.add_argument("--owner", default="")
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
    tu_slot.add_argument("--owner", default="")
    promote = sub.add_parser("promote", help="Preview or apply an experiment's best candidate")
    promote.add_argument("experiment", type=Path)
    promote.add_argument("--write", action="store_true")
    promote.add_argument("--owner", default="")
    rescore = sub.add_parser("rescore", help="Re-evaluate saved candidates without new model calls")
    rescore.add_argument("experiment", type=Path)
    rescore.add_argument("--max-parallel", type=int)
    sub.add_parser("stats")
    args = parser.parse_args(argv)
    harness = Harness(args.config)
    if args.command == "stats":
        print(json.dumps(harness.stats(), indent=2))
        return 0
    if args.command == "prepare":
        prepare_fn = getattr(harness.adapter, "prepare", None)
        if prepare_fn is None:
            parser.error("Configured project adapter does not support prepare")
        print(prepare_fn(args.target_id, write=args.write, owner=args.owner))
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
            owner=args.owner,
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
            owner=args.owner,
        ))
        return 0
    if args.command == "promote":
        print(harness.promote(args.experiment, write=args.write, owner=args.owner))
        return 0
    if args.command == "rescore":
        print(harness.rescore(args.experiment, max_parallel=args.max_parallel))
        return 0
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

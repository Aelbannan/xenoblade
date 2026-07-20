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


def _print_selected_frontier(harness: Harness, target_ids: list[str], selection: str) -> None:
    """Print selected target IDs (and leaf/ready kind when available) for --number runs."""
    describe = getattr(getattr(harness, "adapter", None), "describe_frontier", None)
    rows = None
    if callable(describe):
        try:
            candidate = describe(target_ids)
            if isinstance(candidate, list):
                rows = candidate
        except Exception:
            rows = None
    print(f"selected frontier selection={selection} count={len(target_ids)}")
    if rows:
        for row in rows:
            if isinstance(row, dict):
                print(f"  {row.get('id')} ({row.get('kind', 'unknown')})")
            else:
                print(f"  {row}")
    else:
        for target_id in target_ids:
            print(f"  {target_id}")


def main(argv: list[str] | None = None) -> int:
    argv = list(sys.argv[1:] if argv is None else argv)
    # Check for --show-config early without requiring a command
    show_config = False
    config_path = Path("llm-harness.json")
    for i, arg in enumerate(argv):
        if arg == "--config" and i + 1 < len(argv):
            config_path = Path(argv[i + 1])
        elif arg == "--show-config":
            show_config = True

    if show_config:
        harness = Harness(config_path)
        print(json.dumps(harness.effective_config, indent=2))
        return 0

    parser = argparse.ArgumentParser(description="Generic project-adapted LLM experiment harness")
    parser.add_argument("--config", type=Path, default=Path("llm-harness.json"))
    parser.add_argument(
        "--show-config",
        action="store_true",
        help="Print effective configuration and exit",
    )
    sub = parser.add_subparsers(dest="command", required=True)
    for name in ("new", "improve", "tu-complete", "solve"):
        command = sub.add_parser(name)
        command.add_argument(
            "target_id",
            nargs="?",
            help="Target ID, or unit name for tu-complete",
        )
        if name in {"new", "improve", "tu-complete", "solve"}:
            command.add_argument(
                "--number",
                type=int,
                help=(
                    "Automatically select this many fresh placeholder targets"
                    if name == "new"
                    else "Automatically select this many ready-frontier mismatching targets"
                    if name == "solve"
                    else "Automatically select this many non-accepted (not FULL/EQUIVALENT) targets"
                    if name == "improve"
                    else f"Automatically select this many {name} targets"
                ),
            )
        if name in {"improve", "tu-complete"}:
            command.add_argument(
                "--random",
                action="store_true",
                help="Shuffle eligible targets before applying --number",
            )
        if name in {"new", "improve", "solve"}:
            command.add_argument(
                "--certified-funcs",
                action="store_true",
                help="Only select functions whose called functions are FULL_MATCH or EQUIVALENT+certified",
            )
            command.add_argument(
                "--tu",
                help="Filter to functions in a specific translation unit (e.g. kyoshin/cf/CfPadTask)",
            )
        if name in {"improve", "solve"}:
            command.add_argument(
                "--selection",
                choices=("leaf", "ready", "callees-accepted", "pending"),
                default="ready" if name == "solve" else "pending",
                help=(
                    "Call-graph frontier for automatic --number selection "
                    "(solve default: ready; improve default: pending)"
                ),
            )
        if name == "new":
            command.add_argument(
                "--ignore-called-functions",
                action="store_true",
                help="Allow automatic selection of functions before their callees are matched",
            )
        if name != "solve":
            command.add_argument("--runs", type=int)
        command.add_argument("--dry-run", action="store_true")
        command.add_argument("--resume", type=Path, help="Resume an experiment directory")
        if name != "solve":
            command.add_argument("--max-cost", type=float, help="Stop before the next call after this recorded cost")
            command.add_argument("--max-tokens", type=int, help="Stop before the next call after this token total")
            command.add_argument("--retry-errors", action="store_true", help="On resume, rerun model/run records with errors")
        command.add_argument("--max-parallel", type=int, help="Override configured parallel candidate count")
        if name == "tu-complete":
            command.add_argument(
                "--full-context",
                action="store_true",
                help="Opt in to complete-TU input/output instead of bounded TU slots",
            )
    sample = sub.add_parser(
        "sample",
        help="Blind identical-prompt sampling for model research (not the default decompilation path)",
    )
    sample.add_argument("workflow", choices=("new", "improve", "tu-complete"))
    sample.add_argument("target_id")
    sample.add_argument("--runs", type=int, required=True)
    sample.add_argument("--dry-run", action="store_true")
    sample.add_argument("--max-parallel", type=int)
    batch = sub.add_parser(
        "batch", help="Run a workflow for multiple functions or TUs concurrently"
    )
    batch.add_argument("workflow", choices=("new", "improve", "tu-complete", "solve"))
    batch.add_argument("target_ids", nargs="*")
    batch.add_argument(
        "--tu",
        help="Expand to eligible targets in a translation unit",
    )
    batch.add_argument("--runs", type=int)
    batch.add_argument("--dry-run", action="store_true")
    batch.add_argument("--max-target-parallel", type=int)
    batch.add_argument("--model-parallel", type=int)
    batch.add_argument("--full-context", action="store_true")
    prepare = sub.add_parser("prepare", help="Add function markers around an existing definition")
    prepare.add_argument("target_id")
    prepare.add_argument("--write", action="store_true")
    slot = sub.add_parser("slot", help="Create a function marker slot")
    slot.add_argument("target_id")
    slot.add_argument("file", type=Path)
    slot.add_argument("--before", default="")
    slot.add_argument("--after", default="")
    slot.add_argument("--unit", default="")
    slot.add_argument("--write", action="store_true")
    tu_slot = sub.add_parser("tu-slot", help="Create a TU marker slot")
    tu_slot.add_argument("unit")
    tu_slot.add_argument("slot_id")
    tu_slot.add_argument("file", type=Path)
    tu_slot.add_argument("--before", default="")
    tu_slot.add_argument("--after", default="")
    tu_slot.add_argument("--start", default="")
    tu_slot.add_argument("--end", default="")
    tu_slot.add_argument("--write", action="store_true")
    promote = sub.add_parser(
        "promote",
        help="Explicitly promote a saved experiment candidate into canonical source",
    )
    promote.add_argument("experiment", type=Path)
    promote.add_argument("--write", action="store_true")
    promote.add_argument("--owner", help="Owner/claimant required for --write")
    promote_accepted = sub.add_parser(
        "promote-accepted",
        help="Promote all saved FULL_MATCH / EQUIVALENT_MATCH winners into canonical source",
    )
    promote_accepted.add_argument(
        "--dry-run",
        action="store_true",
        help="List accepted winners that would be promoted without writing",
    )
    rescore = sub.add_parser("rescore", help="Re-evaluate saved candidates without new model calls")
    rescore.add_argument("experiment", type=Path)
    rescore.add_argument("--max-parallel", type=int)
    repair = sub.add_parser("repair", help="Run compile-repair loop on a COMPILE_ERROR experiment")
    repair.add_argument("experiment", type=Path)
    repair.add_argument("--budget", type=int, default=3, help="Max repair iterations")
    repair.add_argument("--dry-run", action="store_true")
    benchmark = sub.add_parser(
        "benchmark",
        help="Aggregate solve-vs-sample metrics from saved experiment dirs or emit corpus summary",
    )
    benchmark.add_argument(
        "--corpus",
        type=Path,
        default=Path("tools/llm_harness/tests/benchmark/golden_corpus.json"),
        help="Golden corpus JSON",
    )
    benchmark.add_argument(
        "--solve-experiment",
        type=Path,
        action="append",
        default=[],
        help="solve experiment directory (repeatable)",
    )
    benchmark.add_argument(
        "--sample-experiment",
        type=Path,
        action="append",
        default=[],
        help="sample/new experiment directory (repeatable)",
    )
    benchmark.add_argument(
        "--output",
        type=Path,
        help="Write comparison JSON report",
    )
    sub.add_parser("stats")
    args = parser.parse_args(argv)

    harness = Harness(args.config)
    if args.command == "stats":
        print(json.dumps(harness.stats(), indent=2))
        return 0
    if args.command == "sample":
        print(harness.run(
            args.workflow,
            args.target_id,
            runs=args.runs,
            dry_run=args.dry_run,
            max_parallel=args.max_parallel,
        ))
        return 0
    if args.command == "batch":
        target_ids = args.target_ids
        if args.tu:
            if target_ids:
                parser.error("batch accepts either target_ids or --tu, not both")
            target_ids = harness.target_ids_for_unit(args.tu, args.workflow)
        if not target_ids:
            parser.error("batch requires target_ids or --tu")
        print(harness.run_batch(
            args.workflow,
            target_ids,
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
        print(harness.promote(args.experiment, write=args.write, owner=args.owner))
        return 0
    if args.command == "promote-accepted":
        results = harness.promote_accepted(dry_run=args.dry_run)
        print(json.dumps({"count": len(results), "results": results}, indent=2))
        if any(row.get("action") == "failed" for row in results):
            return 1
        return 0
    if args.command == "rescore":
        print(harness.rescore(args.experiment, max_parallel=args.max_parallel))
        return 0
    if args.command == "repair":
        print(harness.repair(
            args.experiment, budget=args.budget, dry_run=args.dry_run
        ))
        return 0
    if args.command == "benchmark":
        from tools.llm_harness.benchmark import (
            BenchmarkReport,
            check_rollout_gates,
            compare_workflows,
            load_corpus,
            metrics_from_experiment_state,
        )

        corpus = load_corpus(args.corpus) if args.corpus.is_file() else []
        by_id = {e.target_id: e for e in corpus}
        solve_report = BenchmarkReport(name="solve", workflow="solve")
        sample_report = BenchmarkReport(name="sample", workflow="sample")

        def _ingest(paths: list, report: BenchmarkReport, workflow: str) -> None:
            for path in paths:
                state = json.loads((path / "state.json").read_text(encoding="utf-8"))
                target_id = str(state.get("target_id", ""))
                entry = by_id.get(target_id)
                prompt_path = path / "prompt.md"
                prompt_chars = (
                    len(prompt_path.read_text(encoding="utf-8"))
                    if prompt_path.is_file()
                    else 0
                )
                baseline = ((state.get("baseline") or {}).get("evaluation") or {})
                report.add(
                    metrics_from_experiment_state(
                        state,
                        workflow=workflow,
                        category=entry.category if entry else "",
                        size_bucket=entry.size_bucket if entry else "",
                        baseline_match_percent=float(baseline.get("match_percent") or 0.0),
                        prompt_chars=prompt_chars,
                    )
                )

        _ingest(args.solve_experiment, solve_report, "solve")
        _ingest(args.sample_experiment, sample_report, "sample")

        payload: dict = {
            "corpus_summary": {
                "entries": len(corpus),
                "categories": sorted({e.category for e in corpus}),
                "size_buckets": sorted({e.size_bucket for e in corpus}),
            }
        }
        if solve_report.targets or sample_report.targets:
            comparison = compare_workflows(solve_report, sample_report)
            payload["comparison"] = comparison
            payload["gates"] = check_rollout_gates(comparison)
            payload["solve"] = solve_report.to_dict()
            payload["sample"] = sample_report.to_dict()
        print(json.dumps(payload, indent=2))
        if args.output:
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
        return 0
    if args.command == "solve":
        if getattr(args, "number", None) is not None:
            if args.target_id is not None:
                parser.error("solve accepts either target_id or --number, not both")
            if args.number < 1:
                parser.error("--number must be positive")
            if args.resume:
                parser.error("solve --number does not support --resume")
            selection = getattr(args, "selection", "ready")
            try:
                target_ids = harness.select_targets(
                    "solve",
                    args.number,
                    certified_funcs=getattr(args, "certified_funcs", False),
                    tu=getattr(args, "tu", None),
                    selection=selection,
                )
            except (TypeError, ValueError) as exc:
                parser.error(str(exc))
            if args.dry_run:
                _print_selected_frontier(harness, target_ids, selection)
            print(harness.run_batch(
                "solve",
                target_ids,
                dry_run=args.dry_run,
                model_parallel=args.max_parallel,
            ))
            return 0
        tu = getattr(args, "tu", None)
        if tu:
            if args.target_id is not None:
                parser.error("solve accepts either target_id or --tu, not both")
            target_ids = harness.target_ids_for_unit(tu, "solve")
            if not target_ids:
                parser.error(f"No eligible solve targets found in unit {tu!r}")
            print(harness.run_batch(
                "solve",
                target_ids,
                dry_run=args.dry_run,
                model_parallel=args.max_parallel,
            ))
            return 0
        if args.target_id is None:
            parser.error("solve requires target_id, --number, or --tu")
        print(harness.solve(
            args.target_id,
            dry_run=args.dry_run,
            resume=args.resume,
            max_parallel=args.max_parallel,
        ))
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
                    certified_funcs=getattr(args, "certified_funcs", False),
                    tu=getattr(args, "tu", None),
                )
                if args.dry_run:
                    _print_selected_frontier(harness, target_ids, "ready")
            else:
                selection = getattr(args, "selection", "pending")
                select_kwargs = {
                    "randomize": args.random,
                    "certified_funcs": getattr(args, "certified_funcs", False),
                    "tu": getattr(args, "tu", None),
                }
                if args.command == "improve":
                    select_kwargs["selection"] = selection
                target_ids = harness.select_targets(
                    args.command, args.number, **select_kwargs
                )
                if args.dry_run:
                    _print_selected_frontier(
                        harness,
                        target_ids,
                        selection if args.command == "improve" else "pending",
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
    tu = getattr(args, "tu", None)
    if tu:
        if args.target_id is not None:
            parser.error(f"{args.command} accepts either target_id or --tu, not both")
        try:
            target_ids = harness.target_ids_for_unit(tu, args.command)
        except (TypeError, ValueError) as exc:
            parser.error(str(exc))
        if not target_ids:
            parser.error(f"No eligible {args.command} targets found in unit {tu!r}")
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
        parser.error(f"{args.command} requires target_id, --number, or --tu")
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

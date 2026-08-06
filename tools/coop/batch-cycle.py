#!/usr/bin/env python3
"""
Batch-cycle: mass-cycle multiple targets after matching work.

Processes a list of target IDs sequentially, running the full cycle
(ctx → build → objdiff diff → equivalence probe → JSONL log) for each,
continuing on failure, and producing a summary report.

Usage:
  python3 tools/coop/batch-cycle.py <target-id> [<target-id> ...] [options]

  # Basic — same hypothesis/next-change for all targets
  python3 tools/coop/batch-cycle.py us-80345678 us-80345680 \\
      --default-hypothesis "high-level C reconstruction complete" \\
      --default-next-change "verify static match and equivalence"

  # Per-target hypothesis/next-change via JSON map
  python3 tools/coop/batch-cycle.py us-80345678 us-80345680 \\
      --hypothesis-map batch-map.json

  # Dry-run to preview what would be processed
  python3 tools/coop/batch-cycle.py us-80345678 \\
      --hypothesis-map batch-map.json --dry-run

  # Write a summary report for CI or agent handoff
  python3 tools/coop/batch-cycle.py us-80345678 us-80345680 \\
      --default-hypothesis "batch cleanup" \\
      --default-next-change "accept if pass" \\
      --summary /tmp/batch-summary.json

  # Allow linked-DOL/ELF fallback for the SMT equivalence probe
  python3 tools/coop/batch-cycle.py us-80345678 \\
      --hypothesis-map batch-map.json --linked

Hypothesis map JSON format (target_id → per-target overrides):
  {
    "us-80345678": {
      "hypothesis": "specific hypothesis text",
      "next_change": "specific next change text",
      "runtime_test": "behaviour:<test-id>"
    }
  }

Return code: 0 if all targets pass, 1 if any fail.
"""

from __future__ import annotations

import argparse
import json
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, Optional

# ---------------------------------------------------------------------------
# Path setup — import from sibling modules
# ---------------------------------------------------------------------------
ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import CoopConfig, load_config
from tools.coop.lib.project import Project
from tools.coop.lib.targets import get_target, load_targets


# ---------------------------------------------------------------------------
# Data structures
# ---------------------------------------------------------------------------

@dataclass
class TargetHypothesis:
    hypothesis: str = ""
    next_change: str = ""
    runtime_test: str = ""

    @classmethod
    def from_dict(cls, d: dict) -> TargetHypothesis:
        return cls(
            hypothesis=d.get("hypothesis", ""),
            next_change=d.get("next_change", ""),
            runtime_test=d.get("runtime_test", ""),
        )


@dataclass
class BatchResult:
    target_id: str
    function: str
    status: str                    # cycle exit status string
    instruction_match: Optional[float] = None
    equivalence_status: Optional[str] = None
    passed: bool = False
    error: Optional[str] = None
    duration_s: float = 0.0


@dataclass
class BatchSummary:
    total: int = 0
    passed: int = 0
    failed: int = 0
    skipped: int = 0
    results: list[BatchResult] = field(default_factory=list)
    duration_s: float = 0.0


# ---------------------------------------------------------------------------
# Core logic
# ---------------------------------------------------------------------------

def _build_hypothesis_map(hypothesis_map_path: Optional[Path]) -> Dict[str, TargetHypothesis]:
    if hypothesis_map_path is None:
        return {}
    with hypothesis_map_path.open(encoding="utf-8") as f:
        raw: Dict[str, dict] = json.load(f)
    return {
        tid: TargetHypothesis.from_dict(d)
        for tid, d in raw.items()
    }


def _get_hypothesis(
    hyp_map: Dict[str, TargetHypothesis],
    target_id: str,
    default: TargetHypothesis,
) -> TargetHypothesis:
    per_target = hyp_map.get(target_id)
    if per_target is not None:
        return TargetHypothesis(
            hypothesis=per_target.hypothesis or default.hypothesis,
            next_change=per_target.next_change or default.next_change,
            runtime_test=per_target.runtime_test or default.runtime_test,
        )
    return default


def _run_single_cycle(
    project: Project,
    config: CoopConfig,
    target_id: str,
    hyp: TargetHypothesis,
    *,
    linked: bool = False,
    dry_run: bool = False,
    smt: bool = False,
    witness_timeout_ms: int = 0,
    witness_enabled: bool = True,
) -> BatchResult:
    """Run one cycle via the co-op runner's subprocess CLI."""
    from tools.coop.run import cmd_cycle

    # Re-load targets inside so the registry is fresh per iteration
    targets = load_targets(config)
    try:
        target = get_target(targets, target_id)
    except KeyError:
        return BatchResult(
            target_id=target_id,
            function="",
            status="NOT_FOUND",
            passed=False,
            error=f"Unknown target id: {target_id}",
        )

    if not target.buildable:
        return BatchResult(
            target_id=target_id,
            function=target.function,
            status="NOT_BUILDABLE",
            passed=False,
            error=f"Target '{target_id}' is not buildable (missing source or unit)",
        )

    function = target.function
    print(f"\n{'='*60}")
    print(f"  [{target_id}] {function}")
    print(f"{'='*60}")

    if dry_run:
        print(f"  DRY RUN — would cycle with: hypothesis={hyp.hypothesis!r}, "
              f"next_change={hyp.next_change!r}, runtime_test={hyp.runtime_test!r}"
              f", smt={smt!r}")
        return BatchResult(
            target_id=target_id,
            function=function,
            status="DRY_RUN",
            passed=True,
        )

    start = time.monotonic()
    try:
        rc = cmd_cycle(
            project,
            config,
            target_id,
            hypothesis=hyp.hypothesis,
            next_change=hyp.next_change,
            runtime_test=hyp.runtime_test,
            linked=linked,
            smt=smt,
            witness_timeout_ms=witness_timeout_ms,
            witness_enabled=witness_enabled,
        )
        elapsed = time.monotonic() - start
        passed = rc == 0

        # Reload the target to read updated instruction_match/equivalence
        targets = load_targets(config)
        updated = get_target(targets, target_id)
        return BatchResult(
            target_id=target_id,
            function=function,
            status="PASS" if passed else "FAIL",
            instruction_match=updated.extra.get("instruction_match"),
            equivalence_status=updated.extra.get("equivalence_status"),
            passed=passed,
            error=None if passed else f"cycle returned exit code {rc}",
            duration_s=elapsed,
        )
    except Exception as exc:
        elapsed = time.monotonic() - start
        return BatchResult(
            target_id=target_id,
            function=function,
            status="ERROR",
            passed=False,
            error=str(exc),
            duration_s=elapsed,
        )


def _emit_summary(summary: BatchSummary) -> None:
    """Pretty-print the batch summary to stderr (keeps stdout clean)."""
    lines = [
        "",
        "=" * 60,
        "  BATCH CYCLE SUMMARY",
        "=" * 60,
        f"  Total:    {summary.total}",
        f"  Passed:   {summary.passed}",
        f"  Failed:   {summary.failed}",
        f"  Skipped:  {summary.skipped}",
        f"  Duration: {summary.duration_s:.1f}s",
    ]
    if summary.failed:
        lines.append("")
        lines.append("  Failures:")
        for r in summary.results:
            if not r.passed:
                err = r.error or "unknown error"
                lines.append(f"    ✗ {r.target_id} ({r.function}): {err}")
    lines.append("=" * 60)
    print("\n".join(lines), file=sys.stderr)


def _write_summary_json(summary: BatchSummary, path: Path) -> None:
    """Write a structured JSON summary for CI / agent handoff."""
    path.parent.mkdir(parents=True, exist_ok=True)
    data = {
        "tool": "batch-cycle",
        "total": summary.total,
        "passed": summary.passed,
        "failed": summary.failed,
        "skipped": summary.skipped,
        "duration_s": round(summary.duration_s, 2),
        "results": [
            {
                "target_id": r.target_id,
                "function": r.function,
                "status": r.status,
                "passed": r.passed,
                "error": r.error,
                "instruction_match": r.instruction_match,
                "equivalence_status": r.equivalence_status,
                "duration_s": round(r.duration_s, 2),
            }
            for r in summary.results
        ],
    }
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    print(f"summary written to {path}", file=sys.stderr)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Batch-cycle: mass-cycle multiple targets after matching work.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    parser.add_argument(
        "target_ids",
        nargs="+",
        metavar="TARGET_ID",
        help="One or more target IDs from targets.json to cycle",
    )
    parser.add_argument(
        "--config",
        type=Path,
        help="Path to coop.json or coop.yaml (default: ./coop.json)",
    )
    parser.add_argument(
        "--hypothesis-map",
        type=Path,
        metavar="JSON_FILE",
        help="Path to JSON file mapping target_id → {hypothesis, next_change, runtime_test}",
    )
    parser.add_argument(
        "--default-hypothesis",
        default="batch-cycle: re-verify after matching work",
        help="Default hypothesis for targets not in the hypothesis map",
    )
    parser.add_argument(
        "--default-next-change",
        default="batch-cycle: automated mass cycle",
        help="Default next-change description for targets not in the hypothesis map",
    )
    parser.add_argument(
        "--default-runtime-test",
        default="",
        help="Default runtime-test for targets not in the hypothesis map",
    )
    parser.add_argument(
        "--linked",
        action="store_true",
        help=(
            "Allow the SMT equivalence probe to fall back to linked DOL/ELF bytes "
            "when the unlinked .o pair has unresolved relocations."
        ),
    )
    parser.add_argument(
        "--smt",
        action="store_true",
        help=(
            "Run the full SMT equivalence probe on every cycled target (opt-in; "
            "default runs only the register-renaming witness). Required for "
            "EQUIVALENT_MATCH acceptance of functions the witness cannot certify."
        ),
    )
    parser.add_argument(
        "--witness-timeout",
        type=int,
        default=0,
        metavar="MS",
        help=(
            "Hard timeout (milliseconds) for the register-renaming witness's z3 "
            "rewriter calls (0 = no timeout). The harness passes a bounded value "
            "so a pathological z3.simplify is interrupted instead of spinning "
            "under the build lock."
        ),
    )
    parser.add_argument(
        "--no-witness",
        action="store_true",
        help=(
            "Disable the register-renaming witness probe on every cycled "
            "target: only byte-identical FULL_MATCH can be accepted (never "
            "EQUIVALENT_MATCH). The harness sets this when pi-harness.json "
            "has witnessEnabled=false."
        ),
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print what would be done without actually running cycles",
    )
    parser.add_argument(
        "--summary",
        type=Path,
        metavar="PATH",
        help="Write a structured JSON summary to PATH",
    )
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    # Load configuration and project
    config: CoopConfig = load_config(args.config, ROOT)
    project: Project = Project(config)

    # Load and validate target IDs
    all_targets = load_targets(config)
    unknown_ids = [tid for tid in args.target_ids if tid not in {t.id for t in all_targets}]
    if unknown_ids:
        print(
            f"ERROR: unknown target IDs: {', '.join(unknown_ids)}",
            file=sys.stderr,
        )
        return 1

    # Build hypothesis map
    hyp_map = _build_hypothesis_map(args.hypothesis_map)
    default_hyp = TargetHypothesis(
        hypothesis=args.default_hypothesis,
        next_change=args.default_next_change,
        runtime_test=args.default_runtime_test,
    )

    summary = BatchSummary(total=len(args.target_ids))
    batch_start = time.monotonic()

    for target_id in args.target_ids:
        hyp = _get_hypothesis(hyp_map, target_id, default_hyp)
        result = _run_single_cycle(
            project,
            config,
            target_id,
            hyp,
            linked=args.linked,
            dry_run=args.dry_run,
            smt=args.smt,
            witness_timeout_ms=args.witness_timeout,
            witness_enabled=not args.no_witness,
        )
        summary.results.append(result)
        if result.passed:
            summary.passed += 1
        else:
            summary.failed += 1

        # Print inline result line
        status_char = "✓" if result.passed else "✗"
        duration_str = f" [{result.duration_s:.1f}s]" if result.duration_s > 0 else ""
        print(f"  {status_char} {target_id}: {result.status}{duration_str}")

    summary.duration_s = time.monotonic() - batch_start

    # Emit summary
    _emit_summary(summary)

    if args.summary:
        _write_summary_json(summary, args.summary)

    return 1 if summary.failed > 0 else 0


if __name__ == "__main__":
    sys.exit(main())

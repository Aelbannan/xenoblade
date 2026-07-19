#!/usr/bin/env python3
"""
Apply all llm-harness winners to the repo.

For each experiment whose best candidate reached COMPILES or better:
  1. Promote the candidate into source (--write)
  2. Run coop cycle to verify and update targets.json

Usage:
  python3 tools/coop/apply-harness-winners.py [--dry-run] [--min-status STATUS]

  STATUS can be: COMPILES, CODE_MATCH, FULL_MATCH (default: COMPILES)
"""
from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HARNESS_LOG = ROOT / "build" / "llm-harness" / "experiments.jsonl"

STATUS_RANK = {"COMPILES": 0, "CODE_MATCH": 1, "FULL_MATCH": 2}


def run(*args: str, **kwargs) -> subprocess.CompletedProcess:
    return subprocess.run(args, cwd=ROOT, text=True, capture_output=True, **kwargs)


def load_winners(min_status: str = "COMPILES") -> list[dict]:
    winners: dict[str, dict] = {}
    with open(HARNESS_LOG) as f:
        for line in f:
            entry = json.loads(line.strip())
            if not entry.get("winner"):
                continue
            ev = entry.get("evaluation", {}) or {}
            status = ev.get("status", "")
            if not status:
                continue
            target_id = entry["target_id"]
            # Keep the best result per target
            old = winners.get(target_id)
            if old:
                old_rank = STATUS_RANK.get(old["evaluation"].get("status", ""), -1)
                new_rank = STATUS_RANK.get(status, -1)
                if new_rank <= old_rank:
                    continue
            winners[target_id] = entry

    # Filter by min_status
    min_rank = STATUS_RANK.get(min_status, 0)
    result = [
        w for w in sorted(winners.values(), key=lambda x: x["target_id"])
        if STATUS_RANK.get(w.get("evaluation", {}).get("status", ""), -1) >= min_rank
    ]
    return result


def get_experiment_dir(entry: dict) -> Path:
    """Determine the experiment directory from the artifact path."""
    artifact = entry.get("artifact", "")
    if not artifact:
        target_id = entry["target_id"]
        exp_id = entry["experiment_id"]
        return ROOT / "build" / "llm-harness" / target_id / exp_id
    return ROOT / Path(artifact).parent


def promote_target(target_id: str, exp_dir: Path, owner: str) -> str:
    result = run(
        sys.executable, "tools/llm_harness/run.py", "promote",
        str(exp_dir), "--write", "--owner", owner,
    )
    out = (result.stdout or "").strip() + (result.stderr or "").strip()
    return out or f"promoted {target_id}"


def cycle_target(target_id: str, status: str) -> tuple[str, bool]:
    result = run(
        sys.executable, "tools/coop/run.py", "cycle", target_id,
        "--hypothesis", f"Harness {status} auto-promotion",
        "--next-change", "None",
    )
    out = (result.stdout or "").strip() + (result.stderr or "").strip()
    # Return key lines
    lines = [l.strip() for l in out.split("\n") if l.strip()]
    summary = "\n".join(
        l for l in lines
        if any(kw in l for kw in ("status:", "size:", "equivalence:", "certificate:", "symbol:", "FAIL", "PASS"))
    )
    ok = result.returncode == 0
    return summary or f"cycle complete (exit={result.returncode})", ok


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Apply all llm-harness winners to source + update targets.json"
    )
    parser.add_argument("--dry-run", action="store_true",
                        help="Show what would be done without making changes")
    parser.add_argument("--min-status", choices=["COMPILES", "CODE_MATCH", "FULL_MATCH"],
                        default="COMPILES",
                        help="Minimum evaluation status to apply (default: COMPILES)")
    parser.add_argument(
        "--owner",
        default="llm-harness",
        help="Claim owner used for promote --write (default: llm-harness)",
    )
    args = parser.parse_args()

    winners = load_winners(args.min_status)
    if not winners:
        print("No harness winners found.", file=sys.stderr)
        return 1

    print(f"Found {len(winners)} winners with status >= {args.min_status}:\n")
    for w in winners:
        ev = w.get("evaluation", {}) or {}
        print(f"  {w['target_id']:25s} {ev.get('status','?'):15s} "
              f"match={ev.get('match_percent',0):>6.1f}%  "
              f"model={w.get('model','?'):30s}")
    print()

    if args.dry_run:
        print("[DRY RUN — no changes made]")
        return 0

    promoted = []
    failed = []

    for w in winners:
        target_id = w["target_id"]
        ev = w.get("evaluation", {}) or {}
        status = ev.get("status", "?")
        exp_dir = get_experiment_dir(w)

        print(f"\n{'='*70}")
        print(f"  {target_id} ({status})")
        print(f"{'='*70}")

        # Step 1: Promote
        print(f"  [1/2] Promoting ...", end=" ")
        sys.stdout.flush()
        try:
            promote_out = promote_target(target_id, exp_dir, args.owner)
            print()
            for ln in promote_out.split("\n"):
                print(f"         {ln}")
        except Exception as e:
            print(f"FAILED: {e}")
            failed.append((target_id, "promote", str(e)))
            continue

        # Step 2: Cycle
        print(f"  [2/2] Cycle ...")
        sys.stdout.flush()
        try:
            summary, ok = cycle_target(target_id, status)
            print(f"         {summary}")
        except Exception as e:
            print(f"         FAILED: {e}")
            failed.append((target_id, "cycle", str(e)))
            continue

        promoted.append(target_id)

    print(f"\n{'='*70}")
    print(f"  Summary: {len(promoted)} promoted, {len(failed)} failed")
    print(f"{'='*70}")
    if promoted:
        print(f"\nPromoted targets:")
        for t in promoted:
            print(f"  ✓ {t}")
    if failed:
        print(f"\nFailed targets:")
        for f_id, step, err in failed:
            print(f"  ✗ {f_id} (at {step}): {err[:120]}")
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())

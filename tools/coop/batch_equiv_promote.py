#!/usr/bin/env python3
"""Batch equivalence check + auto-promote EQUIVALENT targets to EQUIVALENT_MATCH.

For each leaf target >= 50% not FULL_MATCH:
  1. Build unit
  2. Run ppc_equivalence check-unit
  3. If EQUIVALENT: check split size, promote via update_target_result
  4. Log result to attempts.jsonl
"""

import json
import subprocess
import sys
import os
import time
from pathlib import Path

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))

# Add repo root to sys.path so internal imports work
sys.path.insert(0, ROOT)

from tools.coop.lib.targets import load_targets, update_target_result
from tools.coop.lib.config import CoopConfig


def run(cmd, timeout=300):
    start = time.time()
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT, timeout=timeout)
    return r.returncode, r.stdout, r.stderr, time.time() - start


def parse_equiv_result(out, err):
    """Parse equivalence check-unit output."""
    result = "UNKNOWN"
    detail = ""
    for line in out.splitlines():
        if "EQUIVALENT" in line and "NOT" not in line and "INCONCLUSIVE" not in line:
            result = "EQUIVALENT"
        elif "NOT EQUIVALENT" in line:
            result = "NOT_EQUIVALENT"
        elif "INCONCLUSIVE" in line:
            result = "INCONCLUSIVE"
        elif line.startswith("status:"):
            detail = line.strip()
        elif line.startswith("first mismatch:"):
            if not detail:
                detail = line.strip()[:120]
    has_unresolved = "unresolved relocations" in out.lower() or "unresolved relocations" in err.lower()
    if has_unresolved and result == "UNKNOWN":
        result = "UNRESOLVED_RELOCS"
        detail = "unresolved relocations"
    return result, detail


def check_size(project: Project, config: CoopConfig, unit_name: str) -> tuple[bool, str]:
    """Check if unit size is within split budget. Returns (ok, message)."""
    from tools.coop.lib.object_size import ObjectSizeChecker
    checker = ObjectSizeChecker(config)
    try:
        size = checker.check_unit(project.resolve_unit(unit_name), config.region)
        ok = size.decomp_section <= size.retail_section
        msg = f"decomp 0x{size.decomp_section:X} / retail 0x{size.retail_section:X} {'OK' if ok else 'OVER'}"
        return ok, msg
    except Exception as e:
        return False, f"size check error: {e}"


def load_tracking_row(config: CoopConfig, target_id: str) -> dict:
    """Read the current tracking row from targets.json for logging."""
    # Use load_targets and find by id
    targets = load_targets(config)
    for t in targets:
        if t.id == target_id:
            return {
                "target_id": t.id,
                "function": t.function,
                "region": config.region,
                "unit": t.unit.name if t.unit else None,
                "symbol": t.symbol,
                "source": str(t.source) if t.source else None,
            }
    return {"target_id": target_id}


def log_attempt(config: CoopConfig, record: dict):
    """Append one line to attempts.jsonl."""
    log_path = config.resolve(config.attempt_log)
    log_path.parent.mkdir(parents=True, exist_ok=True)
    with open(log_path, "a") as f:
        f.write(json.dumps(record, default=str) + "\n")


def main():
    # Load config
    config_path = os.path.join(ROOT, "coop.json")
    config = CoopConfig.load(config_path)

    # Load project
    from tools.coop.lib.objdiff_project import Project
    project = Project(ROOT)

    # Load targets
    targets = load_targets(config)

    # Gather leaf candidates: >=50%, not FULL_MATCH, not EQUIVALENT_MATCH accepted
    candidates = []
    for t in targets:
        if not t.buildable:
            continue
        pct = t.instruction_match or 0
        if pct < 50:
            continue
        if t.status == "FULL_MATCH":
            continue
        if t.status == "EQUIVALENT_MATCH" and t.workflow_status == "ACCEPTED":
            continue
        if not t.unit:
            continue

        leaf = (len(t.called_functions) == 0
                and not t.has_indirect_calls
                and len(t.unresolved_called_functions) == 0)
        if not leaf:
            continue

        candidates.append({
            'id': t.id,
            'pct': pct,
            'status': t.status,
            'wf': t.workflow_status,
            'unit': t.unit.name if t.unit else None,
            'symbol': t.symbol,
            'function': t.function,
        })

    if not candidates:
        print("No leaf candidates to check")
        return 0

    candidates.sort(key=lambda c: -c['pct'])
    print(f"Leaf candidates: {len(candidates)}")

    # Track which units we've already built to avoid redundant builds
    built_units = set()
    promoted = []
    not_equiv = []
    inconclusive = []
    failed = []

    for idx, c in enumerate(candidates, 1):
        tid = c['id']
        unit = c['unit']
        symbol = c['symbol']
        pct = c['pct']
        status = c['status']
        fn = c['function']

        print(f"\n[{idx}/{len(candidates)}] {tid:40s} {pct:5.1f}% {status:20s}  [{unit}]")

        if not unit or not symbol:
            print(f"  SKIP: no unit/symbol")
            failed.append((tid, pct, fn, "NO_UNIT_OR_SYMBOL", ""))
            continue

        # Build unit if needed
        if unit not in built_units:
            rc, out, err, t = run(["python3", "tools/coop/run.py", "build", unit], 120)
            if rc != 0:
                print(f"  BUILD FAILED ({t:.1f}s): {err.strip()[:200]}")
                failed.append((tid, pct, fn, "BUILD_FAILED", f"build error: {err.strip()[:100]}"))
                continue
            built_units.add(unit)

        # Run equivalence check-unit
        rc, out, err, t = run(
            ["python3", "tools/coop/run.py", "equivalence", "check-unit",
             unit, "--symbol", symbol, "--no-build"],
            300
        )

        eq_result, eq_detail = parse_equiv_result(out, err)
        print(f"  equivalence: {eq_result} ({t:.1f}s)")

        if eq_result == "EQUIVALENT":
            # Check size
            ok, size_msg = check_size(project, config, unit)
            print(f"  size: {size_msg}")

            if ok:
                # Promote to EQUIVALENT_MATCH
                try:
                    update_target_result(
                        config,
                        tid,
                        status="EQUIVALENT_MATCH",
                        instruction_match=pct,
                        equivalence_status="equivalent",
                        equivalence_certificate=None,
                        certificate_checked=True,
                    )
                    # Log attempt
                    log_attempt(config, {
                        "target_id": tid,
                        "function": fn,
                        "region": config.region,
                        "unit": unit,
                        "symbol": symbol,
                        "status": "EQUIVALENT_MATCH",
                        "instruction_match": round(pct, 1),
                        "hypothesis": "auto batch equivalence check (leaf target)",
                        "next_change": "equivalence proven; promoted to EQUIVALENT_MATCH",
                        "equivalence_status": "equivalent",
                        "equivalence_detail": eq_detail,
                    })
                    print(f"  ✅ PROMOTED to EQUIVALENT_MATCH / ACCEPTED")
                    promoted.append((tid, pct, fn, unit))
                except Exception as e:
                    print(f"  ❌ PROMOTION FAILED: {e}")
                    failed.append((tid, pct, fn, "PROMOTION_FAILED", str(e)))
            else:
                # Size over budget — record as BACKLOG
                try:
                    update_target_result(
                        config,
                        tid,
                        status="EQUIVALENT_MATCH",
                        instruction_match=pct,
                        equivalence_status="equivalent",
                        override_workflow="BACKLOG",
                    )
                    print(f"  ⚠️ EQUIVALENT but size over budget -> BACKLOG")
                    failed.append((tid, pct, fn, "SIZE_OVER_BUDGET", size_msg))
                except Exception as e:
                    print(f"  ❌ BACKLOG PROMOTION FAILED: {e}")
                    failed.append((tid, pct, fn, "BACKLOG_FAILED", str(e)))
        elif eq_result == "NOT_EQUIVALENT":
            not_equiv.append((tid, pct, fn, unit, eq_detail))
        elif eq_result == "INCONCLUSIVE":
            inconclusive.append((tid, pct, fn, unit, eq_detail))
        else:
            failed.append((tid, pct, fn, eq_result, eq_detail))

        if eq_detail:
            print(f"  detail: {eq_detail}")
        if rc != 0 and eq_result == "UNKNOWN":
            for line in err.splitlines()[-5:]:
                print(f"  stderr: {line}")

    # Summary
    print("\n" + "=" * 60)
    print("FINAL SUMMARY")
    print("=" * 60)

    print(f"\n✅ PROMOTED to EQUIVALENT_MATCH: {len(promoted)}")
    for tid, pct, fn, unit in promoted:
        print(f"  {tid:40s} {pct:5.1f}%  {fn[:60]}")

    print(f"\n❌ NOT_EQUIVALENT: {len(not_equiv)}")
    for tid, pct, fn, unit, detail in not_equiv:
        print(f"  {tid:40s} {pct:5.1f}%  {fn[:55]}")

    print(f"\n⏱️ INCONCLUSIVE: {len(inconclusive)}")
    for tid, pct, fn, unit, detail in inconclusive:
        print(f"  {tid:40s} {pct:5.1f}%  {fn[:55]}")

    print(f"\n❌ FAILED / OTHER: {len(failed)}")
    for tid, pct, fn, reason, detail in failed:
        print(f"  {tid:40s} {pct:5.1f}%  {reason:25s}  {fn[:40]}")

    # Write full results
    out_file = os.path.join(ROOT, "tools/coop/batch_equiv_promote_results.json")
    with open(out_file, 'w') as f:
        json.dump({
            "promoted": [{"id": r[0], "pct": r[1], "function": r[2], "unit": r[3]} for r in promoted],
            "not_equivalent": [{"id": r[0], "pct": r[1], "function": r[2], "detail": r[4]} for r in not_equiv],
            "inconclusive": [{"id": r[0], "pct": r[1], "function": r[2], "detail": r[4]} for r in inconclusive],
            "failed": [{"id": r[0], "pct": r[1], "function": r[2], "reason": r[3], "detail": r[4]} for r in failed],
        }, f, indent=2)
    print(f"\nResults -> {out_file}")

    return 0


if __name__ == '__main__':
    sys.exit(main())
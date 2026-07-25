#!/usr/bin/env python3
"""Batch equivalence check for all >=50% non-FULL_MATCH targets.

Groups by unit, builds once, runs ppc_equivalence check-unit per function.
Outputs structured summary sorted: leaf first, then by match percent desc.
"""

import json
import subprocess
import sys
import os
import time
from collections import defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))


def run(cmd, timeout=300):
    start = time.time()
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT, timeout=timeout)
    elapsed = time.time() - start
    return r.returncode, r.stdout, r.stderr, elapsed


def parse_result(out, err):
    """Parse equivalence check-unit output for result status."""
    result = "UNKNOWN"
    detail = ""
    has_unresolved = False

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
        if "unresolved relocations" in line.lower():
            has_unresolved = True

    if has_unresolved and result == "UNKNOWN":
        result = "UNRESOLVED_RELOCS"
        detail = "unlinked .o has unresolved relocations"

    if rc3 := None:  # placeholder - real logic below
        pass

    return result, detail


def main():
    with open(os.path.join(ROOT, 'tools/coop/targets.json')) as f:
        data = json.load(f)
    targets = data['targets']

    # Gather candidates: >=50%, not FULL_MATCH, not already ACCEPTED EQUIVALENT_MATCH
    candidates = []
    for t in targets:
        pct = t.get('instruction_match', 0) or 0
        ms = t.get('status', '')
        wf = t.get('workflow_status', '')
        if pct < 50:
            continue
        if ms == 'FULL_MATCH':
            continue
        if ms == 'EQUIVALENT_MATCH' and wf == 'ACCEPTED':
            continue

        calls = t.get('called_functions', [])
        indirect = t.get('has_indirect_calls', False)
        unresolved = t.get('unresolved_called_functions', [])
        leaf = len(calls) == 0 and not indirect and len(unresolved) == 0

        unit = t.get('unit', '')
        symbol = t.get('symbol', '')
        if not unit or not symbol:
            continue

        candidates.append({
            'id': t['id'],
            'pct': pct,
            'status': ms,
            'wf': wf,
            'leaf': leaf,
            'symbol': symbol,
            'unit': unit,
            'function': t.get('function', t['id']),
        })

    # Only leaf targets for now
    candidates = [c for c in candidates if c['leaf']]
    # Sort by pct desc
    candidates.sort(key=lambda c: -c['pct'])

    print(f"Leaf candidates: {len(candidates)}")
    print()

    results = []
    built_units = set()
    total = len(candidates)

    for idx, c in enumerate(candidates, 1):
        tid = c['id']
        unit = c['unit']
        symbol = c['symbol']
        pct = c['pct']
        status = c['status']
        fn = c['function']

        print(f"[{idx}/{total}] {tid:40s} {pct:5.1f}% {status:20s}  [{unit}]")

        # Build unit if needed
        if unit not in built_units:
            rc, out, err, t = run(["python3", "tools/coop/run.py", "build", unit], timeout=120)
            if rc != 0:
                print(f"  BUILD FAILED ({t:.1f}s): {err.strip()[:200]}")
                results.append((tid, pct, fn, unit, "BUILD_FAILED", "", t))
                continue
            built_units.add(unit)

        # Run equivalence check-unit
        rc, out, err, t = run(
            ["python3", "tools/coop/run.py", "equivalence", "check-unit",
             unit, "--symbol", symbol, "--no-build"],
            timeout=300
        )

        # Parse result
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

        # Check for unresolved relocs
        has_unresolved = "unresolved relocations" in out.lower() or "unresolved relocations" in err.lower()
        if has_unresolved and result == "UNKNOWN":
            result = "UNRESOLVED_RELOCS"
            detail = "unlinked .o has unresolved relocations"

        print(f"  {result} ({t:.1f}s)")
        if detail:
            print(f"  {detail}")
        if rc != 0 and result == "UNKNOWN":
            for line in err.splitlines()[-5:]:
                print(f"  stderr: {line}")

        results.append((tid, pct, fn, unit, result, detail, t))

    # Summary
    print()
    print("=" * 60)
    print("SUMMARY")
    print("=" * 60)

    categories = {
        "EQUIVALENT": [],
        "NOT_EQUIVALENT": [],
        "INCONCLUSIVE": [],
        "UNRESOLVED_RELOCS": [],
        "BUILD_FAILED": [],
        "UNKNOWN": [],
    }

    for r in results:
        cat = r[4] if r[4] in categories else "UNKNOWN"
        categories[cat].append(r)

    for cat_name, cat_list in categories.items():
        if cat_list:
            print(f"\n--- {cat_name} ({len(cat_list)}) ---")
            for tid, pct, fn, unit, result, detail, t in cat_list:
                print(f"  {tid:40s} {pct:5.1f}%  {fn}")
                if detail:
                    print(f"  {' ' * 45}{detail[:120]}")

    # Write JSON
    out_file = os.path.join(ROOT, "tools/coop/batch_equiv_results.json")
    with open(out_file, 'w') as f:
        json.dump(categories, f, indent=2, default=str)
    print(f"\nFull results -> {out_file}")


if __name__ == '__main__':
    main()
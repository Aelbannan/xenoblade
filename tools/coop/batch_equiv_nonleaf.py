#!/usr/bin/env python3
"""Run equivalence checks on non-leaf named P0/P1 targets."""
import json, subprocess, os, time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
TARGETS_JSON = os.path.join(ROOT, 'tools/coop/targets.json')

def run(cmd, timeout=300):
    start = time.time()
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT, timeout=timeout)
    return r.returncode, r.stdout, r.stderr, time.time() - start

# Named P0/P1/P2 non-ACCEPTED targets that are NOT leaf
named_nonleaf = [
    # (target_id, tier)
    ("occ-cull-helper", "P1"),
    ("menu-bps-move", "P1"),
    ("uicf-func-80133324", "P2"),
    ("menu-enemy-cbrender", "P1"),
    ("battlestate-vfunc10", "P1"),
    ("battlestate-vfunc8", "P1"),
    ("cview-render-view", "P0"),
    ("menu-arts-cbrender", "P1"),
    ("battle-mgr-move", "P1"),
    ("uicf-init", "P2"),
    ("uicf-move", "P2"),
    ("menu-enemy-move", "P1"),
    ("occ-set-frustum", "P1"),
    ("menu-bps-ctor", "P1"),
    ("menu-arts-move", "P1"),
    ("battlestate-vfunc26", "P1"),
    ("battlestate-vfunc11", "P1"),
    ("view-update-msg", "P0"),
]

with open(TARGETS_JSON) as f:
    data = json.load(f)

target_map = {t['id']: t for t in data['targets']}

results = []
built_units = set()

for tid, tier in named_nonleaf:
    t = target_map.get(tid)
    if not t:
        print(f"SKIP {tid}: not found in targets.json")
        results.append((tid, 0, "NOT_FOUND", "", "", 0))
        continue

    pct = t.get('instruction_match', 0) or 0
    ms = t.get('status', '')
    unit = t.get('unit', '')
    symbol = t.get('symbol', '')
    fn = t.get('function', tid)

    print(f"\n[{tid}] {pct:5.1f}% {ms:20s} unit={unit}")

    if not unit or not symbol:
        print(f"  SKIP: no unit/symbol")
        results.append((tid, pct, "NO_UNIT_OR_SYMBOL", fn, "", 0))
        continue

    # Build unit if needed
    if unit not in built_units:
        rc, out, err, t_ = run(["python3", "tools/coop/run.py", "build", unit], 120)
        if rc != 0:
            print(f"  BUILD FAILED: {err.strip()[:200]}")
            results.append((tid, pct, "BUILD_FAILED", fn, "", t_))
            continue
        built_units.add(unit)

    # Run equivalence check-unit
    rc, out, err, t_ = run(
        ["python3", "tools/coop/run.py", "equivalence", "check-unit",
         unit, "--symbol", symbol, "--no-build"],
        300
    )

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

    if "unresolved relocations" in out.lower() or "unresolved relocations" in err.lower():
        if result == "UNKNOWN":
            result = "UNRESOLVED_RELOCS"
            detail = "unresolved relocations"

    print(f"  {result} ({t_:.1f}s)")
    if detail:
        print(f"  {detail}")
    if rc != 0 and result == "UNKNOWN":
        for line in err.splitlines()[-5:]:
            print(f"  stderr: {line}")

    results.append((tid, pct, result, fn, detail, t_))

# Summary
print("\n" + "=" * 60)
print("NON-LEAF NAMED TARGETS SUMMARY")
print("=" * 60)
for tid, pct, result, fn, detail, t_ in results:
    print(f"  {result:20s} {tid:40s} {pct:5.1f}%  {fn[:55]}")

out_file = os.path.join(ROOT, "tools/coop/batch_equiv_nonleaf.json")
with open(out_file, 'w') as f:
    json.dump(results, f, indent=2)
print(f"\nResults -> {out_file}")
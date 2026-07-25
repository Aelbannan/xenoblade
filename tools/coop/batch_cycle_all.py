#!/usr/bin/env python3
"""Run cycle on all >=50% non-leaf targets not yet ACCEPTED."""
import json, subprocess, sys, os, time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))

def run(cmd, timeout=120):
    start = time.time()
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT, timeout=timeout)
    return r.returncode, r.stdout, r.stderr, time.time() - start

# Load targets
with open(os.path.join(ROOT, 'tools/coop/targets.json')) as f:
    data = json.load(f)

targets = [t for t in data['targets']
           if (t.get('instruction_match', 0) or 0) >= 50
           and t.get('status') not in ('FULL_MATCH',)
           and not (t.get('status') == 'EQUIVALENT_MATCH' and t.get('workflow_status') == 'ACCEPTED')
           and bool(t.get('source'))
           and bool(t.get('unit'))
           and bool(t.get('symbol'))]

# Filter to non-leaf only
nonleaf = []
for t in targets:
    calls = t.get('called_functions', [])
    indirect = t.get('has_indirect_calls', False)
    unresolved = t.get('unresolved_called_functions', [])
    leaf = len(calls) == 0 and not indirect and len(unresolved) == 0
    if not leaf:
        nonleaf.append((t['id'], t.get('instruction_match', 0), t.get('status', ''),
                        t.get('workflow_status', ''), t.get('function', '?')[:55]))

nonleaf.sort(key=lambda x: -x[1])
print(f"Non-leaf targets to cycle: {len(nonleaf)}")

results = []
for idx, (tid, pct, ms, wf, fn) in enumerate(nonleaf, 1):
    print(f"\n[{idx}/{len(nonleaf)}] {tid:40s} {pct:5.1f}% {ms:20s} {wf:10s}  {fn[:55]}", flush=True)
    rc, out, err, t = run([
        "python3", "tools/coop/run.py", "cycle", tid,
        "--hypothesis", "batch cycle all non-leaf >=50%",
        "--next-change", "equivalence check via cycle",
        "--runtime-test", ""
    ], timeout=300)

    # Parse result
    for line in out.splitlines():
        if "PASS:" in line:
            print(f"  ✅ PASS ({(t):.1f}s)")
            results.append((tid, pct, "PASS", fn, "", t))
            break
        if "FAIL:" in line:
            fail_reason = line.strip()
            print(f"  ❌ {fail_reason} ({(t):.1f}s)")
            results.append((tid, pct, "FAIL", fn, fail_reason, t))
            break
    else:
        # No PASS/FAIL found — check for errors
        err_lines = [l for l in err.splitlines() if l.strip()][-3:]
        print(f"  ❓ RC={rc} ({(t):.1f}s)")
        for l in err_lines:
            print(f"     {l}")
        results.append((tid, pct, f"RC={rc}", fn, err_lines[0] if err_lines else "", t))

# Summary
print("\n" + "=" * 60)
print("SUMMARY")
print("=" * 60)
passes = [r for r in results if r[2] == "PASS"]
fails = [r for r in results if r[2].startswith("FAIL")]
others = [r for r in results if r[2] not in ("PASS",) and not r[2].startswith("FAIL")]

print(f"\nPASS: {len(passes)}")
for tid, pct, res, fn, detail, t in passes:
    print(f"  {tid:40s} {pct:5.1f}%  {fn[:50]}")

print(f"\nFAIL: {len(fails)}")
for tid, pct, res, fn, detail, t in fails:
    print(f"  {tid:40s} {pct:5.1f}%  {detail[:80]}")

if others:
    print(f"\nOTHER: {len(others)}")
    for tid, pct, res, fn, detail, t in others:
        print(f"  {tid:40s} {pct:5.1f}%  {res:10s}  {detail[:80]}")

out_file = os.path.join(ROOT, "tools/coop/batch_cycle_nonleaf_results.json")
with open(out_file, 'w') as f:
    json.dump([{"id": r[0], "pct": r[1], "result": r[2], "detail": r[4], "time": r[5]} for r in results], f, indent=2)
print(f"\nResults -> {out_file}")
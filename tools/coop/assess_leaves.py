#!/usr/bin/env python3
"""Quickly assess all NOT_STARTED CriWare leaves for match quality."""
import json, subprocess, sys, os
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TARGETS = ROOT / "tools/coop/targets.json"

with open(TARGETS) as f:
    data = json.load(f)

cri = [t for t in data["targets"] 
       if "CriWare" in t.get("unit","") and t.get("status")=="NOT_STARTED"]
leaves = [t for t in cri 
          if not t.get("called_functions") and not t.get("has_indirect_calls",False)]
leaves.sort(key=lambda x: int(x.get("size","0x0"),16))

results = []
for t in leaves:
    tid = t["id"]
    fn = t["function"]
    unit = t["unit"]
    size = t.get("size","?")
    
    # Build
    r = subprocess.run(
        [sys.executable, "tools/coop/run.py", "build", unit],
        cwd=ROOT, capture_output=True, text=True, timeout=60
    )
    if r.returncode != 0:
        results.append((tid, fn, size, unit, "BUILD_FAIL", 0, 0, 0))
        print(f"BUILD_FAIL {tid} {fn}")
        continue
    
    # Hexdiff
    r = subprocess.run(
        [sys.executable, "tools/coop/hexdiff.py", unit, "--symbol", fn, "--json"],
        cwd=ROOT, capture_output=True, text=True, timeout=30
    )
    if r.returncode not in (0, 5):
        results.append((tid, fn, size, unit, "DIFF_FAIL", 0, 0, 0))
        print(f"DIFF_FAIL {tid} {fn}: {r.stderr[:100]}")
        continue
    
    try:
        out = json.loads(r.stdout)
    except json.JSONDecodeError:
        results.append((tid, fn, size, unit, "JSON_FAIL", 0, 0, 0))
        print(f"JSON_FAIL {tid} {fn}")
        continue
    
    retail_sz = out.get("retail_size", 0)
    decomp_sz = out.get("decomp_size", 0)
    total = out.get("mismatch_count", 0)
    reg = out.get("reg_swap_count", 0)
    structural = out.get("structural_count", 0)
    
    results.append((tid, fn, size, unit, "OK", total, reg, structural))
    print(f"{tid:20s} {fn:35s} size={size:6s} total={total:3d} reg={reg:3d} struct={structural:3d} retail={retail_sz} decomp={decomp_sz}")

print("\n--- Best candidates (struct=0, size match) ---")
for tid, fn, size, unit, status, total, reg, struct in results:
    if status == "OK" and struct == 0:
        print(f"{tid} {fn} total={total} reg={reg} size={size}")

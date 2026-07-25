#!/usr/bin/env python3
"""Batch hexdiff all EQUIVALENT_MATCH targets and print a summary."""

import json
import subprocess
import sys
import os
from pathlib import Path

COOP_DIR = Path(__file__).parent
TARGETS_FILE = COOP_DIR / "targets.json"
PROJECT_DIR = COOP_DIR.parent.parent

os.chdir(str(PROJECT_DIR))

# Load targets
with open(TARGETS_FILE) as f:
    data = json.load(f)

targets = data["targets"]
eq_targets = [t for t in targets if t.get("status") == "EQUIVALENT_MATCH"]

print(f"EQUIVALENT_MATCH targets to check: {len(eq_targets)}")
print()

results = []

for t in eq_targets:
    tid = t["id"]
    symbol_name = t.get("function", "?")
    unit = t.get("unit", "")
    fuzzy = t.get("instruction_match", 0)
    has_cert = t.get("equivalence_certificate") is not None
    
    # Determine the best symbol name for hexdiff
    # Use mangled symbol from target, or function name
    mangled = symbol_name
    
    # Build unit path for hexdiff
    if not unit:
        print(f"  {tid}: SKIP (no unit)")
        continue
    
    # Run hexdiff
    try:
        r = subprocess.run(
            ["python3", "tools/coop/hexdiff.py", unit, 
             "--symbol", mangled, "--no-build", "--json"],
            capture_output=True, text=True, timeout=120
        )
        if r.returncode == 0 or r.returncode == 5:
            diff = json.loads(r.stdout)
            mc = diff["mismatch_count"]
            total = diff["total_instructions"]
            rs = diff["reg_swap_count"]
            sc = diff["structural_count"]
            rsize = diff["retail_size"]
            dsize = diff["decomp_size"]
            size_ok = "OK" if rsize == dsize else f"OVERFLOW ({rsize}B -> {dsize}B)"
            results.append({
                "id": tid, "fuzzy": fuzzy, "unit": unit,
                "mismatches": mc, "total": total, 
                "reg_swaps": rs, "structural": sc,
                "size": size_ok, "has_cert": has_cert,
                "mangled": symbol_name
            })
        else:
            print(f"  {tid}: hexdiff error (exit {r.returncode})")
            if r.stderr:
                print(f"    stderr: {r.stderr[:200]}")
    except subprocess.TimeoutExpired:
        print(f"  {tid}: TIMEOUT")
    except json.JSONDecodeError as e:
        print(f"  {tid}: JSON parse error: {e}")
    except Exception as e:
        print(f"  {tid}: ERROR: {e}")

# Summary
print()
print("=" * 80)
print("SUMMARY")
print("=" * 80)

# Sort by problematic status
bad_structural = [r for r in results if r["structural"] > 0]
pure_regswap = [r for r in results if r["structural"] == 0 and r["reg_swaps"] > 0]
perfect = [r for r in results if r["mismatches"] == 0]

print(f"\nPerfect (0 mismatches): {len(perfect)}")
for r in perfect:
    print(f"  ✅ {r['id']} ({r['fuzzy']}%)")

print(f"\nPure reg-swap only: {len(pure_regswap)}")
for r in pure_regswap:
    print(f"  🔄 {r['id']} ({r['fuzzy']}%) - {r['reg_swaps']} reg-swaps, size={r['size']}")

print(f"\nStructural mismatches (NOT equivalent): {len(bad_structural)}")
for r in sorted(bad_structural, key=lambda x: -x["structural"]):
    cert_str = "no-cert" if not r["has_cert"] else "stale-cert"
    print(f"  ❌ {r['id']} ({r['fuzzy']}%, {r['structural']} structural + {r['reg_swaps']} regswap, size={r['size']}, {cert_str})")

# Also print by category
print()
print("=" * 80)
print("DETAILED BY SEVERITY")
print("=" * 80)

# High severity: structural > 0 AND size overflow
high_sev = [r for r in bad_structural if "OVERFLOW" in str(r["size"])]
med_sev = [r for r in bad_structural if "OVERFLOW" not in str(r["size"])]

if high_sev:
    print(f"\nHIGH SEVERITY (structural + size overflow):")
    for r in high_sev:
        print(f"  ❌ {r['id']}")

if med_sev:
    print(f"\nMEDIUM SEVERITY (structural, size OK):")
    for r in med_sev:
        print(f"  ❌ {r['id']}")
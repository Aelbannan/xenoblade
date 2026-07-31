#!/usr/bin/env python3
"""Summarize hexdiff --json output for quick iteration."""
import json
import subprocess
import sys

unit = sys.argv[1]
sym = sys.argv[2]
no_build = "--no-build" in sys.argv

cmd = [".venv/bin/python3", "tools/coop/hexdiff.py", unit, "--symbol", sym, "--json"]
if no_build:
    cmd.append("--no-build")
out = subprocess.run(cmd, capture_output=True, text=True, cwd="/Users/ahmedelbannan/Ahmed/xenoblade")
try:
    start = out.stdout.index("{")
    d = json.loads(out.stdout[start:])
except Exception:
    print("STDOUT:", out.stdout[-3000:])
    print("STDERR:", out.stderr[-3000:])
    sys.exit(1)

print(f"== {sym}: mismatch_count={d.get('mismatch_count')} reg_swaps={d.get('reg_swap_count')} structural={d.get('structural_count')}")
for ins in d.get("instructions", []):
    if ins.get("mismatch"):
        print(f"  @0x{ins['offset']:04x} {ins.get('retail_asm',''):45s} || {ins.get('decomp_asm','')}")

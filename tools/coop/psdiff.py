#!/usr/bin/env python3
"""Small helper: run hexdiff.py for a symbol and print a compact mismatch summary."""
import json
import subprocess
import sys

unit, symbol = sys.argv[1], sys.argv[2]
extra = sys.argv[3:]  # e.g. --no-build
out = subprocess.run(
    [".venv/bin/python3", "tools/coop/hexdiff.py", unit, "--symbol", symbol, "--json", *extra],
    capture_output=True, text=True,
)
lines = out.stdout.splitlines()
try:
    start = next(i for i, l in enumerate(lines) if l.lstrip().startswith("{"))
    d = json.loads("\n".join(lines[start:]))
except Exception as e:
    print("parse error:", e)
    print(out.stdout[-2000:])
    print(out.stderr[-2000:])
    sys.exit(1)

mc = d.get("mismatch_count", len(d.get("mismatches", [])))
print(f"{symbol}: mismatch={mc} reg_swap={d.get('reg_swap_count')} structural={d.get('structural_count')}")
for it in d.get("mismatches", []):
    print(f"  {it['offset']:04x}: {it.get('retail_asm')} || {it.get('decomp_asm')}")

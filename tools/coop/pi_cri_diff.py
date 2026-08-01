#!/usr/bin/env python3
"""Helper: hexdiff one or more symbols in sfd_mpv, print match stats."""
import json
import subprocess
import sys

UNIT = "CriWare/src/sofdec/sfdcore/sfd/sfd_mpv"


def diff(sym, no_build=False):
    cmd = [".venv/bin/python3", "tools/coop/hexdiff.py", UNIT, "--symbol", sym, "--json"]
    if no_build:
        cmd.append("--no-build")
    r = subprocess.run(cmd, capture_output=True, text=True)
    out = r.stdout
    idx = out.find("{")
    if idx < 0:
        print(f"ERROR {sym}: rc={r.returncode} {r.stderr[-2000:]}")
        return None
    try:
        d = json.loads(out[idx:])
    except json.JSONDecodeError as e:
        print(f"ERROR {sym}: {e}")
        return None
    ins = d.get("instructions", [])
    m = sum(1 for i in ins if i.get("match"))
    reg_swap = sum(1 for i in ins if i.get("reg_swap"))
    struct = sum(1 for i in ins if i.get("structural"))
    relocs_retail = len(d.get("retail_relocations", []))
    relocs_decomp = len(d.get("decomp_relocations", []))
    print(f"{sym}: {m}/{len(ins)} matched ({100.0*m/len(ins) if ins else 0:.1f}%) "
          f"reg_swap={reg_swap} structural={struct} relocs {relocs_retail}/{relocs_decomp}")
    return d


if __name__ == "__main__":
    args = sys.argv[1:]
    no_build = "--no-build" in args
    args = [a for a in args if a != "--no-build"]
    if not args:
        print("usage: pi_cri_diff.py <symbol> [<symbol>...] [--no-build]")
        sys.exit(1)
    for s in args:
        diff(s, no_build=no_build)

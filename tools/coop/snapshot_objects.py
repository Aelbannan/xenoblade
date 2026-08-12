#!/usr/bin/env python3
"""Snapshot the matching-relevant surface of every build object.

Records per object:
  - sha256 of .text contents
  - sha256 of the .rela.text relocation table (via readelf -r)
  - section sizes (text/data/bss/sdata/sdata2/rodata) via readelf -SW
  - defined data symbols (nm) — verifies no definition moved

Usage: snapshot_objects.py <region> <out.json>
"""
import hashlib
import json
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BIN = ROOT / "build" / "binutils"


def run(args):
    return subprocess.run([str(a) for a in args], capture_output=True, text=True)


def sha(b: bytes):
    return hashlib.sha256(b).hexdigest()


def snapshot(region):
    out = {}
    objs = []
    for tree in ("src", "obj"):
        base = ROOT / "build" / region / tree
        if base.exists():
            objs += sorted(base.rglob("*.o"))
    for o in objs:
        rel = str(o.relative_to(ROOT))
        entry = {}
        # .text bytes
        r = run([BIN / "powerpc-eabi-objcopy", "-O", "binary",
                 "--only-section=.text", o, "/tmp/_lbls_snap.bin"])
        if r.returncode == 0 and os.path.exists("/tmp/_lbls_snap.bin"):
            entry["text_sha"] = sha(open("/tmp/_lbls_snap.bin", "rb").read())
        # relocations on .text
        r = run([BIN / "powerpc-eabi-readelf", "-r", o])
        entry["rela"] = sha(r.stdout.encode())
        # section sizes (name -> size)
        r = run([BIN / "powerpc-eabi-readelf", "-SW", o])
        sizes = {}
        for m in re.finditer(r"\[\s*\d+\]\s+(\S+)\s+\S+\s+\S+\s+\S+\s+([0-9a-fA-F]+)\s", r.stdout):
            sizes[m.group(1)] = m.group(2)
        entry["sections"] = {k: v for k, v in sizes.items()
                             if k in (".text", ".data", ".bss", ".sdata",
                                      ".sdata2", ".sbss", ".rodata", ".ctors",
                                      ".dtors", ".init", ".extab", ".extabindex")}
        # defined data symbols
        r = run([BIN / "powerpc-eabi-nm", "-D", o])
        entry["nm"] = sha(r.stdout.encode())
        out[rel] = entry
    return out


if __name__ == "__main__":
    region = sys.argv[1] if len(sys.argv) > 1 else "us"
    out_path = sys.argv[2] if len(sys.argv) > 2 else "/tmp/lbls_snapshot.json"
    snap = snapshot(region)
    Path(out_path).write_text(json.dumps(snap, indent=0, sort_keys=True))
    print(f"{len(snap)} objects -> {out_path}")

#!/usr/bin/env python3
"""Shrink-only ``.text`` function-order gate (``run.py text-order-gate``).

hexdiff and the data gates compare functions one at a time, so a unit whose
functions all byte-match but are defined in a different order than retail
passes them all, even though the linked DOL would lay its functions out in a
different order.  MWCC emits functions in definition order, with deferred
inline/weak functions at the end of the TU.

For every completed unit (objdiff ``metadata.complete``) this lists the
``.text`` function symbols of the retail and decomp objects by address and
compares the order of the names that both sides define.  The diff count is
the number of functions that have to move (common count minus the longest
common subsequence).  The result is checked against
``docs/evidence/text_order_baseline.json``:

* a unit that is out of order and not in the baseline -> FAIL
* a baselined unit whose count rises -> FAIL

``--ratchet`` rewrites the baseline, but only by removing units or lowering
counts, and it refuses when anything regressed.  The gate reads existing
objects, so build first under the lock.

Pure comparison logic lives in module functions, so the tests need no build.
"""
from __future__ import annotations

import json
import struct
import sys
from bisect import bisect_left
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BASELINE = ROOT / "docs" / "evidence" / "text_order_baseline.json"

STT_FUNC = 2


def text_functions(path: Path) -> list[str]:
    """Names of the STT_FUNC symbols in ``.text``, ordered by address."""
    data = Path(path).read_bytes()
    shoff = struct.unpack_from(">I", data, 0x20)[0]
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)

    def shdr(i: int):
        return struct.unpack_from(">IIIIIIIIII", data, shoff + i * shentsize)

    def cstr(base: int, off: int) -> str:
        end = data.index(b"\0", base + off)
        return data[base + off:end].decode(errors="replace")

    shstr_off = shdr(shstrndx)[4]
    text_idx = {i for i in range(shnum) if cstr(shstr_off, shdr(i)[0]) == ".text"}
    out: list[tuple[int, int, str]] = []
    for i in range(shnum):
        sh = shdr(i)
        if sh[1] != 2:  # SHT_SYMTAB
            continue
        str_off = shdr(sh[6])[4]
        for n, so in enumerate(range(sh[4], sh[4] + sh[5], 16)):
            st_name, st_value, _sz, st_info, _oth, st_shndx = struct.unpack_from(
                ">IIIBBH", data, so)
            if st_name and st_info & 0xF == STT_FUNC and st_shndx in text_idx:
                out.append((st_value, n, cstr(str_off, st_name)))
    return [name for _v, _n, name in sorted(out)]


def order_diffs(retail: list[str], decomp: list[str]) -> int:
    """Functions out of place among the names both sides define (0 = same order)."""
    common = set(retail) & set(decomp)
    pos = {n: i for i, n in enumerate(x for x in decomp if x in common)}
    seq = [pos[n] for n in retail if n in common]
    tails: list[int] = []  # longest increasing subsequence == LCS (names unique)
    for v in seq:
        k = bisect_left(tails, v)
        if k == len(tails):
            tails.append(v)
        else:
            tails[k] = v
    return len(seq) - len(tails)


def compare(baseline: dict, current: dict) -> tuple[list[str], list[str]]:
    """(regressions, improvements) of *current* against *baseline* (unit -> count)."""
    regress: list[str] = []
    improve: list[str] = []
    for unit in sorted(set(baseline) | set(current)):
        b, c = baseline.get(unit), current.get(unit)
        if b is None:
            regress.append(f"new .text order mismatch: {unit} ({c} function(s) out of place)")
        elif c is None:
            improve.append(f".text order now matches: {unit}")
        elif c > b:
            regress.append(f".text order diffs rose in {unit}: {b} -> {c}")
        elif c < b:
            improve.append(f".text order diffs fell in {unit}: {b} -> {c}")
    return regress, improve


def ratchet(baseline: dict, current: dict) -> dict:
    """Shrink-only update: keep units still failing, at the lower count."""
    return {u: min(n, current[u]) for u, n in baseline.items() if u in current}


def scan(root: Path = ROOT, verbose: bool = False) -> tuple[dict, int, list[str]]:
    """(out-of-order unit -> count, units audited, missing objects)."""
    from tools.coop.extab_gate import completed_units

    failing: dict[str, int] = {}
    missing: list[str] = []
    audited = 0
    for unit in completed_units(root):
        retail, decomp = root / unit["target_path"], root / unit["base_path"]
        if not retail.is_file() or not decomp.is_file():
            missing.append(unit["name"])
            continue
        audited += 1
        r, d = text_functions(retail), text_functions(decomp)
        n = order_diffs(r, d)
        if n:
            failing[unit["name"]] = n
            if verbose:
                common = set(r) & set(d)
                print(f"{unit['name']}: {n} function(s) out of place")
                for a, b in zip([x for x in r if x in common], [x for x in d if x in common]):
                    print(f"    {'  ' if a == b else '!='} {a:<60} {b}")
    return failing, audited, missing


def load_baseline(path: Path = BASELINE) -> dict:
    return json.loads(path.read_text())["units"]


def write_baseline(units: dict, path: Path = BASELINE) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    doc = {
        "_comment": "Shrink-only. Completed units whose .text function order differs from "
                    "retail (value: functions out of place). See tools/coop/text_order_gate.py.",
        "units": dict(sorted(units.items())),
    }
    # Deliberate, reviewed raises are logged with a reason; keep that log.
    if path.exists():
        raises = json.loads(path.read_text()).get("_raises")
        if raises:
            doc["_raises"] = raises
    path.write_text(json.dumps(doc, indent=1) + "\n")


def gate(do_ratchet: bool = False, init: bool = False, verbose: bool = False) -> int:
    current, audited, missing = scan(ROOT, verbose)
    print(f"text-order-gate: {audited} completed unit(s) audited, {len(current)} out of order")
    if missing:
        print(f"text-order-gate: {len(missing)} unit(s) without a built object (build first): "
              + ", ".join(missing[:5]), file=sys.stderr)
    if init:
        write_baseline(current)
        print(f"text-order-gate: wrote baseline {BASELINE.relative_to(ROOT)}")
        return 0
    baseline = load_baseline()
    regress, improve = compare(baseline, current)
    for line in improve:
        print(f"  improved: {line}")
    if regress or missing:
        for line in regress:
            print(f"FAIL: {line}", file=sys.stderr)
        if missing:
            print("FAIL: missing decomp objects", file=sys.stderr)
        print("  (diagnose: run.py text-order-gate --verbose)", file=sys.stderr)
        return 1
    if do_ratchet and improve:
        write_baseline(ratchet(baseline, current))
        print(f"text-order-gate: ratcheted baseline ({len(improve)} change(s))")
    elif improve:
        print("text-order-gate: OK; run with --ratchet to lock in the improvements")
    else:
        print("text-order-gate: OK")
    return 0


if __name__ == "__main__":
    import argparse

    sys.path.insert(0, str(ROOT))
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--ratchet", action="store_true")
    ap.add_argument("--init", action="store_true")
    ap.add_argument("--verbose", action="store_true")
    a = ap.parse_args()
    sys.exit(gate(a.ratchet, a.init, a.verbose))

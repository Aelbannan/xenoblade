#!/usr/bin/env python3
"""Per-unit source-faithfulness status and shrink-only gate (``run.py faithful``).

Byte matching says a unit compiles to the retail bytes.  It does not say the
source looks like what the original programmers wrote: a member function
written as an ``extern "C"`` free function over raw offsets scores the same as
the real method.  This tool measures the second property.

A unit is **faithful** when all of these are zero:

  label         ``lbl_<region>_XXXXXXXX`` address labels used as identifiers
  mangled_c     ``extern "C"`` declarations/definitions of mangled C++ names
                (member functions written as C free functions)
  section       ``__declspec(section ...)`` hand-placed data
  offset_cast   ``(u8*)p + 0x..`` raw offset arithmetic
  view_struct   offset "view" structs (``...View``, ``...At3E9C``, ``...Off``);
                classes retail names via a ctor/dtor/vtable symbol (``CView``) are exempt
  rule          the unit has a tools/postprocess_reloc_names.py UNIT_RULES entry
  rtti_off      configure.py compiles the unit with ``-RTTI off`` (retail has
                RTTI; the flag exists to make room for hand-built typeinfo)
  foreign_data  retail data symbols that relocation evidence assigns to this
                unit but splits.txt gives to another unit (or the reverse);
                see tools/coop/section_owners.py

Counts cover the unit's source file plus the header with the same stem, with
comments stripped.  Naming debt that does not change the program's shape
(``field_0x..``, ``Unk...``, ``func_XXXXXXXX``) is reported as ``soft`` and
does not gate.

  noinline      ``__declspec(noinline)`` / ``#pragma dont_inline`` /
                ``#pragma auto_inline off`` forcing a call the original source
                did not need to force.  Check the real causes first: wrong TU
                boundary, definition order, unit-wide inline flags.  A use that
                survives those checks is exempt when its line carries
                ``// noinline-ok: <what was tried>``.

The gate compares against ``docs/evidence/faithful_baseline.json``: a unit's
score may not rise and a faithful unit may not stop being faithful.
``--ratchet`` lowers the baseline (shrink-only).
"""
from __future__ import annotations

import argparse
import collections
import functools
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BASELINE = ROOT / "docs" / "evidence" / "faithful_baseline.json"

HARD = {
    "label": re.compile(r"\blbl_(?:eu|us|jp)_[0-9A-Fa-f]{8}\b"),
    "mangled_c": re.compile(
        r'extern\s+"C"[^\n;{]*?\b(?!__RTTI__|__vt__)[A-Za-z_]\w*?__(?:Q\d|\d+[A-Za-z_])\w*F[A-Za-z0-9_]+'),
    "section": re.compile(r"__declspec\s*\(\s*section"),
    "offset_cast": re.compile(r"\(\s*(?:const\s+)?(?:u8|s8|char|unsigned char)\s*\*\s*\)\s*\(?\s*\w+\s*\)?\s*[+-]\s*0x[0-9A-Fa-f]+"),
    "view_struct": re.compile(r"\b\w*(?:View|At[0-9A-F]{2,}|Off)\s*\*"),
    "noinline": re.compile(r"\bnoinline\b|#\s*pragma\s+(?:dont_inline\b|auto_inline\s+off\b)"),
}
SOFT = {
    "field_0x": re.compile(r"\bfield_0x[0-9A-Fa-f]+\b"),
    "unk_name": re.compile(r"\bUnk(?:Class|VirtualFunc|Struct)?_?[0-9A-Fa-f]+\b"),
    "addr_func": re.compile(r"\b(?:func|fn)_[0-9A-Fa-f]{8}\b"),
}
COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
HEADER_EXT = (".hpp", ".h")
SOURCE_ROOTS = ("src", "include", "libs")


def strip_comments(text: str) -> str:
    return COMMENT.sub(" ", text)


NOINLINE_OK = re.compile(r"//\s*noinline-ok:\s*\S")


def exempt_justified_noinline(text: str) -> str:
    """Drop the noinline smell from lines annotated ``// noinline-ok: <reason>``."""
    return "\n".join(
        HARD["noinline"].sub(" ", line) if NOINLINE_OK.search(line) else line
        for line in text.split("\n"))


_REAL_CLASS_SYM = re.compile(r"^__(?:ct|dt|vt)__(\d+)([A-Za-z_]\w*)", re.M)


@functools.lru_cache(maxsize=None)
def real_classes(region: str = "us") -> frozenset[str]:
    """Class names that retail itself names via a ctor/dtor/vtable symbol.

    A pointer to such a class (``CView*``) is a real type, not an offset view,
    even though its name matches the ``view_struct`` pattern."""
    path = ROOT / "config" / region / "symbols.txt"
    if not path.exists():
        return frozenset()
    return frozenset(m.group(2)[:int(m.group(1))]
                     for m in _REAL_CLASS_SYM.finditer(path.read_text(errors="replace")))


def _count(key: str, pat: re.Pattern, code: str) -> int:
    if key != "view_struct":
        return len(pat.findall(code))
    real = real_classes()
    return sum(1 for m in pat.finditer(code)
               if m.group(0).rstrip("* \t\n") not in real)


def count_smells(text: str) -> tuple[dict[str, int], dict[str, int]]:
    code = strip_comments(exempt_justified_noinline(text))
    return ({k: _count(k, p, code) for k, p in HARD.items()},
            {k: len(p.findall(code)) for k, p in SOFT.items()})


def header_index() -> dict[str, list[Path]]:
    idx: dict[str, list[Path]] = collections.defaultdict(list)
    for root in SOURCE_ROOTS:
        for p in (ROOT / root).rglob("*"):
            if p.suffix in HEADER_EXT and p.is_file():
                idx[p.stem].append(p)
    return idx


def pick_header(src: Path, idx: dict[str, list[Path]]) -> Path | None:
    cands = idx.get(src.stem, [])
    if not cands:
        return None

    def shared_tail(p: Path) -> int:
        a, b = src.parent.parts[::-1], p.parent.parts[::-1]
        n = 0
        while n < min(len(a), len(b)) and a[n] == b[n]:
            n += 1
        return n

    best = max(cands, key=shared_tail)
    # A stem shared by unrelated directories (types.h, util.h) is not "the
    # unit's header" unless the paths actually line up.
    if len(cands) > 1 and shared_tail(best) == 0:
        return None
    return best


def foreign_data_counts(region: str) -> dict[str, int]:
    from tools.coop import section_owners

    out: collections.Counter = collections.Counter()
    # Only anonymous symbols count: a named global used by one other unit is
    # not misplaced (section_owners.tier).
    for _sec, r, _tier in section_owners.disagreements(region, ("A", "B")):
        out[r["vote"]] += 1
        if r["cur"]:
            out[r["cur"]] += 1
    return dict(out)


RTTI_OFF = re.compile(r'Object\(\s*\w+\s*,\s*"([^"]+)"[^\n]*?-RTTI off')


def rtti_off_units() -> set[str]:
    """Source paths (as written in configure.py) compiled with -RTTI off."""
    return set(RTTI_OFF.findall((ROOT / "configure.py").read_text()))


def measure(region: str = "us") -> dict[str, dict]:
    import tools.postprocess_reloc_names as P
    from tools.coop.lib import weak_dedup as W

    ranges, _ = W._layout(ROOT, region)
    foreign = foreign_data_counts(region)
    idx = header_index()
    no_rtti = rtti_off_units()
    out: dict[str, dict] = {}
    for u in json.loads((ROOT / "objdiff.json").read_text())["units"]:
        sp = (u.get("metadata") or {}).get("source_path")
        if not sp or not (ROOT / sp).is_file():
            continue
        src = ROOT / sp
        hard, soft = count_smells(src.read_text(errors="replace"))
        hdr = pick_header(src, idx)
        if hdr is not None:
            h2, s2 = count_smells(hdr.read_text(errors="replace"))
            for k in hard:
                hard[k] += h2[k]
            for k in soft:
                soft[k] += s2[k]
        base = u.get("base_path") or ""
        hard["rule"] = 1 if base and P.unit_has_rules(Path(base).name) else 0
        hard["rtti_off"] = 1 if any(sp.endswith("/" + n) for n in no_rtti) else 0
        split = W.resolve_unit(ranges, u["name"])
        hard["foreign_data"] = foreign.get(split, 0) if split else 0
        out[u["name"]] = {
            "source": sp,
            "header": str(hdr.relative_to(ROOT)) if hdr else None,
            "complete": bool((u.get("metadata") or {}).get("complete")),
            "hard": hard,
            "soft": soft,
            "score": sum(hard.values()),
        }
    return out


def compare(baseline: dict[str, int], current: dict[str, int]) -> tuple[list[str], list[str]]:
    """Return (failures, improvements).  Units absent from a map score 0."""
    fails, better = [], []
    for name in sorted(set(baseline) | set(current)):
        b, c = baseline.get(name, 0), current.get(name, 0)
        if c > b:
            fails.append(f"{name}: {b} -> {c}")
        elif c < b:
            better.append(f"{name}: {b} -> {c}")
    return fails, better


def gate(region: str, ratchet: bool, init: bool, verbose: bool, unit: str | None = None) -> int:
    res = measure(region)
    cur = {n: r["score"] for n, r in res.items() if r["score"]}
    faithful = [n for n, r in res.items() if not r["score"]]
    totals: collections.Counter = collections.Counter()
    soft_totals: collections.Counter = collections.Counter()
    for r in res.values():
        totals.update(r["hard"])
        soft_totals.update(r["soft"])
    if unit:
        for n, r in res.items():
            if unit in n:
                print(n, json.dumps({k: v for k, v in r.items() if k != "score"}, indent=1))
    print(f"faithful: {len(faithful)} of {len(res)} units with source "
          f"({sum(1 for n in faithful if res[n]['complete'])} of them complete)")
    print("  hard: " + "  ".join(f"{k} {v}" for k, v in totals.items()))
    print("  soft: " + "  ".join(f"{k} {v}" for k, v in soft_totals.items()))
    if verbose:
        for n in sorted(cur, key=cur.get, reverse=True)[:40]:
            print(f"    {cur[n]:6} {n}")
    if init:
        BASELINE.write_text(json.dumps({"region": region, "units": cur}, indent=1, sort_keys=True) + "\n")
        print(f"faithful: wrote baseline ({len(cur)} unfaithful unit(s))")
        return 0
    if not BASELINE.is_file():
        print("faithful: no baseline; run with --init")
        return 1
    base = json.loads(BASELINE.read_text())["units"]
    fails, better = compare(base, cur)
    for f in fails:
        print(f"FAIL: faithfulness score rose in {f}")
    if fails:
        return 1
    if ratchet and better:
        BASELINE.write_text(json.dumps({"region": region, "units": cur}, indent=1, sort_keys=True) + "\n")
        print(f"faithful: ratcheted baseline ({len(better)} change(s))")
    elif better:
        print(f"faithful: OK; {len(better)} unit(s) improved, run with --ratchet to lock them in")
    else:
        print("faithful: OK")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--region", default="us")
    ap.add_argument("--ratchet", action="store_true")
    ap.add_argument("--init", action="store_true")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--unit")
    a = ap.parse_args()
    sys.path.insert(0, str(ROOT))
    return gate(a.region, a.ratchet, a.init, a.verbose, a.unit)


if __name__ == "__main__":
    sys.path.insert(0, str(ROOT))
    sys.exit(main())

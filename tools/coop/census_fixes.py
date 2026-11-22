#!/usr/bin/env python3
"""Mechanical-fix censuses: candidates the orchestrator can land as presteps
without a matching agent.

    python tools/coop/census_fixes.py splits      # US vs JP section ownership + forced-section padding
    python tools/coop/census_fixes.py sigs        # local re-declarations whose arity differs elsewhere
    python tools/coop/census_fixes.py all --json OUT

Each finding is a *candidate*, not a fix: verify against retail (hexdiff,
symbols.txt, the other region) before writing a prestep.  Wave-1 examples:

* splits — nw4hbm lyt_material owned nw4r's 8-byte .sbss2 in US (JP gives it to
  nw4r) and padded it with a forced-section dummy array; moving the claim fixed
  three functions (LYTMATERIAL-SBSS2).
* sigs — CREvtModelMap re-declared ``scnLgtRemoveLgtItem`` with one parameter;
  the canonical header has two, and retail passes the second (W1-00).
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SRC_EXT = (".c", ".cpp", ".cp")
HDR_EXT = (".h", ".hpp")
CODE_ROOTS = ("src", "libs", "include")


# --------------------------------------------------------------------------- splits
def parse_splits(region: str) -> dict[str, dict[str, tuple[int, int]]]:
    units: dict[str, dict[str, tuple[int, int]]] = {}
    cur = None
    for line in (ROOT / "config" / region / "splits.txt").read_text().splitlines():
        if line and not line[0].isspace() and line.endswith(":"):
            cur = line[:-1]
            units.setdefault(cur, {})
            continue
        m = re.match(r"\s+(\S+)\s+start:(0x[0-9A-Fa-f]+)\s+end:(0x[0-9A-Fa-f]+)", line)
        if m and cur and cur != "Sections":
            units[cur][m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
    return units


_GENERIC = re.compile(r"^(?:lbl_|@|\.\.|jumptable_|float_|double_|string_|switch_|gap_|pad_)")


def _symbols(region: str) -> dict[str, tuple[str, int]]:
    """{real symbol name: (section, address)} — generated/address names skipped."""
    out: dict[str, tuple[str, int]] = {}
    pat = re.compile(r"^(\S+)\s*=\s*(\.\w+|extab\w*):(0x[0-9A-Fa-f]+);")
    seen: dict[str, int] = {}
    for line in (ROOT / "config" / region / "symbols.txt").read_text().splitlines():
        m = pat.match(line)
        if not m or _GENERIC.match(m.group(1)) or re.search(r"_[89][0-9A-Fa-f]{7}$", m.group(1)):
            continue
        n = m.group(1)
        seen[n] = seen.get(n, 0) + 1
        out[n] = (m.group(2), int(m.group(3), 16))
    return {n: v for n, v in out.items() if seen[n] == 1}  # ambiguous duplicates dropped


_FORCED = re.compile(r'__declspec\s*\(\s*section\s+"([^"]+)"\s*\)[^;\n]*?\b(\w*(?:dummy|pad|fill|_sbss|_sdata|_bss)\w*)',
                     re.I)


def splits_census() -> dict:
    us, jp = parse_splits("us"), parse_splits("jp")
    diffs = []
    for unit in sorted(set(us) & set(jp)):
        a, b = set(us[unit]), set(jp[unit])
        for sec in sorted(b - a):
            lo, hi = jp[unit][sec]
            diffs.append({"unit": unit, "section": sec, "jp_has": True, "size": hi - lo})
        for sec in sorted(a - b):
            lo, hi = us[unit][sec]
            diffs.append({"unit": unit, "section": sec, "jp_has": False, "size": hi - lo})
    # misassignment shape: unit A lacks section S in US that JP gives it, while
    # unit B has S in US only, with the same size (LYTMATERIAL-SBSS2)
    # and the same real symbol name lives in both ranges (sizes alone collide)
    syms = {r: _symbols(r) for r in ("us", "jp")}

    def names(region, sec, rng):
        lo, hi = rng
        return {n for n, (s_, a) in syms[region].items() if s_ == sec and lo <= a < hi}

    pairs = []
    jp_only = [d for d in diffs if d["jp_has"]]
    us_only = [d for d in diffs if not d["jp_has"]]
    for a in jp_only:
        for b in us_only:
            if a["section"] != b["section"] or a["size"] != b["size"] or a["unit"] == b["unit"]:
                continue
            sec = a["section"]
            common = names("jp", sec, jp[a["unit"]][sec]) & names("us", sec, us[b["unit"]][sec])
            if common:
                pairs.append({"section": sec, "size": a["size"], "owner_jp": a["unit"], "owner_us": b["unit"],
                              "us_range": [hex(x) for x in us[b["unit"]][sec]], "symbols": sorted(common)})
    forced = []
    for root in CODE_ROOTS:
        for p in (ROOT / root).rglob("*"):
            if p.suffix not in SRC_EXT + HDR_EXT or ".ctx." in p.name or not p.is_file():
                continue
            if "PowerPC_EABI_Support" in p.parts:
                continue  # real linker-generated section symbols
            text = p.read_text(encoding="utf-8", errors="replace")
            for m in _FORCED.finditer(text):
                forced.append({"file": str(p.relative_to(ROOT)), "line": text.count("\n", 0, m.start()) + 1,
                               "section": m.group(1), "name": m.group(2)})
    return {"section_set_diffs": diffs, "misassigned_pairs": pairs, "forced_padding": forced}


# --------------------------------------------------------------------------- signatures
_COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
_DECL = re.compile(
    r"(?m)^(?P<indent>[ \t]*)(?:extern\s+(?:\"C\"\s+)?)?(?:static\s+|inline\s+|virtual\s+)*"
    r"(?P<ret>[A-Za-z_][\w:<>]*(?:\s*[\*&])*)\s+(?P<ptr>[\*&]\s*)?(?P<name>[A-Za-z_]\w*)\s*"
    r"\((?P<params>[^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*(?:const\s*)?;")
_KEYWORDS = {"return", "else", "new", "delete", "goto", "case", "sizeof", "typedef", "using", "throw"}


def _arity(params: str) -> int | None:
    p = params.strip()
    if p in ("", "void"):
        return 0
    if "..." in p:
        return None
    depth, n = 0, 1
    for ch in p:
        if ch in "<(":
            depth += 1
        elif ch in ">)":
            depth -= 1
        elif ch == "," and depth == 0:
            n += 1
    return n


def sigs_census(min_len: int = 8) -> list[dict]:
    decls: dict[str, list[tuple[str, int, int, bool]]] = defaultdict(list)
    for root in CODE_ROOTS:
        for p in (ROOT / root).rglob("*"):
            if p.suffix not in SRC_EXT + HDR_EXT or ".ctx." in p.name or not p.is_file():
                continue
            text = _COMMENT.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)),
                                p.read_text(encoding="utf-8", errors="replace"))
            rel = str(p.relative_to(ROOT))
            for m in _DECL.finditer(text):
                name, ret = m.group("name"), m.group("ret")
                if name in _KEYWORDS or ret in _KEYWORDS or len(name) < min_len:
                    continue
                n = _arity(m.group("params"))
                if n is None:
                    continue
                decls[name].append((rel, text.count("\n", 0, m.start()) + 1, n, p.suffix in HDR_EXT))
    out = []
    for name, ds in decls.items():
        arities = {d[2] for d in ds}
        if len(arities) < 2 or len({d[0] for d in ds}) < 2:
            continue  # overloads inside one file are not drift
        hdr = {d[2] for d in ds if d[3]}
        # a free-function spelling of a member takes `self` first: local == header + 1
        # is mangled_c debt, not arity drift
        local = [d for d in ds if not d[3] and (not hdr or (d[2] not in hdr and d[2] - 1 not in hdr))]
        if not hdr and len({d[2] for d in local}) < 2:
            continue
        if not local:
            continue
        out.append({"name": name, "header_arity": sorted(hdr), "local": [f"{f}:{l} ({n})" for f, l, n, _ in local],
                    "all": [f"{f}:{l} ({n})" for f, l, n, _ in ds]})
    return sorted(out, key=lambda r: r["name"])


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("what", choices=("splits", "sigs", "all"))
    ap.add_argument("--json", type=Path)
    a = ap.parse_args(argv)
    res: dict = {}
    if a.what in ("splits", "all"):
        res["splits"] = s = splits_census()
        print(f"section-set diffs US vs JP: {len(s['section_set_diffs'])}; forced padding: {len(s['forced_padding'])}")
        print(f"probable misassignments (same section+size, JP owner != US owner): {len(s['misassigned_pairs'])}")
        for d in s["misassigned_pairs"]:
            print(f"  {d['section']:<8} {d['size']:#6x}  US {d['owner_us']} -> JP {d['owner_jp']}  {d['us_range']}")
        for d in s["section_set_diffs"][:0]:
            print(f"  {'JP-only' if d['jp_has'] else 'US-only'} {d['section']:<10} {d['size']:#7x}  {d['unit']}")
        for f in s["forced_padding"][:40]:
            print(f"  forced {f['section']:<8} {f['name']}  {f['file']}:{f['line']}")
    if a.what in ("sigs", "all"):
        res["sigs"] = g = sigs_census()
        print(f"arity drift candidates: {len(g)}")
        for r in g[:40]:
            print(f"  {r['name']}: header {r['header_arity']} local {r['local'][:3]}")
    if a.json:
        a.json.write_text(json.dumps(res, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())

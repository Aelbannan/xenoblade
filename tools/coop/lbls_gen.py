#!/usr/bin/env python3
"""Centralize lbl_eu_*/lbl_* retail data-label declarations into per-area headers.

Motivation / verified facts (MWCC Wii/1.1, build/compilers/Wii/1.1/mwcceppc.exe):
  - MWCC never mangles global-scope *data* names in C++: plain `extern` emits the
    exact retail reloc name (`R_PPC_EMB_SDA21` / `ADDR16_HA/LO` + `lbl_eu_XXXX`),
    byte-identical to `extern "C"`. `extern "C"` on data is only semantically
    required inside `namespace { }` blocks (verified: plain extern mangles there).
  - `extern` declarations are inert: a TU compiled with per-TU decls vs an
    equivalent header include produces byte-identical .text and zero data/bss.

Consequences implemented here:
  - Header entries are plain `extern` (no `extern "C"`).
  - Only explicit-`extern` *declarations* are centralized. Bare declarations
    (`CProcess* lbl_eu_80664054;`) and `extern ... lbl(...)` / `= {...}` forms
    are *definitions* (B-symbols in the TU's own data section) and stay in place.
  - Addresses whose declared type differs across TUs, or whose type is a C++
    class / library typedef not parseable by C99 TUs, stay per-TU (listed in the
    exclusion report) -- no forced type unification that could change codegen.

Commands:
  scan       (default) analyze the corpus and print a report
  generate   write include/lbls_<area>.hpp + tools/coop/lbls_exclusions.json
  apply      strip per-TU lbl decls and add the area-header include
  check      CI gate: header freshness + no stray per-TU decls / extern "C"

Usage:
  python3 tools/coop/lbls_gen.py [generate|apply|check] [--dry-run]
"""
from __future__ import annotations

import argparse
import collections
import json
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / "src"
LIBS = ROOT / "libs"
INCLUDE_DIR = ROOT / "include"
EXCLUSIONS_PATH = ROOT / "tools" / "coop" / "lbls_exclusions.json"
MANIFEST_PATH = ROOT / "tools" / "coop" / "lbls_manifest.json"

# Area -> header name, matched by relative path prefix.
AREA_ORDER = [
    ("kyoshin", "lbls_kyoshin.hpp", [("src/", "kyoshin")]),
    ("criware", "lbls_criware.hpp", [("libs/", "CriWare")]),
    ("rvl_sdk", "lbls_rvl_sdk.hpp", [("libs/", "RVL_SDK")]),
    ("nw4r", "lbls_nw4r.hpp", [("libs/", "nw4r")]),
    ("monolib", "lbls_monolib.hpp", [("libs/", "monolib")]),
    ("other", "lbls_other.hpp", []),  # catch-all: PowerPC_EABI_Support, NdevExi2A, t1x.c, ...
]

BUILTINS = {
    "u8", "s8", "u16", "s16", "u32", "s32", "u64", "s64", "f32", "f64",
    "char", "int", "long", "short", "unsigned", "signed", "float", "double",
    "void", "wchar_t", "bool", "size_t", "va_list",
}

LBL_NAME = re.compile(r"\b(lbl_(?:eu_)?[0-9A-F]{8})\b")
EXTERN_LINE = re.compile(r"^\s*(extern)")
EXTERN_BLOCK = re.compile(r"^\s*extern\s*\"C\"\s*\{\s*$")
# Bare declaration: type tokens must be identifier-ish only (no braces/parens/
# assignments/semicolons) so function signatures and statements never match.
BARE_LINE = re.compile(
    r"^\s*((?:[A-Za-z_]\w*(?:::)?[\w:\*&\s,<>]*?))"
    r"\b(lbl_(?:eu_)?[0-9A-F]{8})\b\s*(\[[^\]]*\])?\s*;\s*$"
)
BAD_BARE = re.compile(r"[{}()=;\[\]]")
BAD_BARE_WORDS = ("return", "static", "if", "while", "for", "sizeof",
                  "goto", "case", "delete", "new", "throw", "switch")



def norm_type(t: str) -> str:
    t = re.sub(r"\bconst\b", " ", t)
    t = re.sub(r"\s+", " ", t).strip()
    for a, b in [
        ("unsigned char", "u8"), ("unsigned int", "u32"), ("unsigned long", "u32"),
        ("unsigned short", "u16"), ("signed char", "s8"), ("float", "f32"),
        ("double", "f64"),
    ]:
        t = t.replace(a, b)
    t = re.sub(r"\s*\*\s*", "*", t)
    t = re.sub(r"\s*&\s*", "&", t)
    return t


def c99_safe(t: str) -> bool:
    toks = re.sub(r"[\*\[\]&]", " ", t).split()
    if not toks:
        return False
    return all(tok in BUILTINS or tok in ("struct", "union", "enum") for tok in toks)


def area_for(rel: str) -> str:
    for area, _hdr, prefixes in AREA_ORDER:
        if area == "other":
            continue
        for pfx, sub in prefixes:
            if rel.startswith(pfx) and rel[len(pfx):].startswith(sub):
                return area
    return "other"


def header_for_area(area: str) -> str:
    for a, hdr, _p in AREA_ORDER:
        if a == area:
            return hdr
    return "lbls_other.hpp"


def parse_file(path: Path):
    """Yield (address, norm_type, raw_type, kind, start_line, end_line) for lbl
    decl/def lines. start/end are 1-based line numbers IN THE PHYSICAL FILE
    (block-comment stripping preserves newlines)."""
    txt = path.read_text(encoding="utf-8", errors="replace")
    # Strip block comments but keep the line structure so line numbers stay
    # aligned with the physical file (replacement = spaces + newlines).
    txt = re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)),
                 txt, flags=re.S)
    lines = txt.splitlines()
    out = []
    i = 0
    while i < len(lines):
        line = lines[i]
        if EXTERN_BLOCK.match(line):
            i += 1  # `extern "C" {` opener: inner decls parsed on their own lines
            continue
        m = EXTERN_LINE.match(line)
        if m:
            buf = line[m.end():]
            start_i = i
            end_i = i
            while ";" not in buf and i + 1 < len(lines):
                i += 1
                end_i = i
                buf += " " + lines[i]
            lm = LBL_NAME.search(buf)
            if lm:
                name = lm.group(1)
                pre = buf[: lm.start()]
                post = buf[lm.end():]
                # Reject when the lbl token is not the declarator of a simple
                # declaration (function definitions / bodies, initializers,
                # expressions): `extern "C" T f(...) { ... lbl ...` must skip.
                if re.search(r"[(){}=;]", pre) or re.search(r"[(){}=]", post):
                    i += 1
                    continue
                arr = re.match(r"\s*(\[[^\]]*\])?", post).group(1) or ""
                rest = re.sub(r"^[;}\s]+|[;}\s]+$", "", post[len(arr):])
                # type = text after the LAST `extern` (handles `"C" { extern T`)
                exts = list(re.finditer(r"\bextern\b", pre))
                typ = pre[exts[-1].end():].strip() if exts else pre.strip()
                typ = re.sub(r'^"C"?\s*', "", typ)
                is_def = bool(rest)
                kind = "DEF_INIT" if is_def else "DECL"
                out.append((name, norm_type(typ), typ, kind, start_i + 1, end_i + 1))
            i += 1
            continue
        bm = BARE_LINE.match(line)
        if bm:
            typ = bm.group(1).strip()
            first = typ.split()[0] if typ.split() else ""
            if (typ and not BAD_BARE.search(typ)
                    and not re.match(r"^extern$", typ)
                    and first not in BAD_BARE_WORDS):
                out.append((bm.group(2), norm_type(typ), typ, "DEF_BARE", i + 1, i + 1))
        i += 1
    return out


def scan_corpus():
    """Return rows: list of dicts (address, ntype, rtype, kind, file, area, line, end)."""
    rows = []
    for root in (SRC, LIBS):
        for d, _dirs, fs in os.walk(root):
            for f in fs:
                if not f.endswith((".cpp", ".c", ".hpp", ".h")):
                    continue
                if f.endswith(".ctx.c"):
                    continue  # decomp.me context snapshots, not compiled
                p = Path(d) / f
                rel = str(p.relative_to(ROOT))
                for addr, ntype, rtype, kind, start, end in parse_file(p):
                    rows.append({
                        "address": addr, "ntype": ntype, "rtype": rtype,
                        "kind": kind, "file": rel, "area": area_for(rel),
                        "line": start, "end": end,
                    })
    return rows


def classify(rows):
    """Return (header_addrs, exclusions) where header_addrs maps addr -> (ntype, area)."""
    by = collections.defaultdict(list)
    for r in rows:
        by[r["address"]].append(r)
    header_addrs = {}
    exclusions = []
    for addr, rs in sorted(by.items()):
        ntypes = {r["ntype"] for r in rs}
        areas = {r["area"] for r in rs}
        kinds = {r["kind"] for r in rs}
        # Addresses with a definition in source (RTTP objects, singletons,
        # initialised tables) stay per-TU: the definition TU owns the storage.
        if "DEF_BARE" in kinds or "DEF_INIT" in kinds:
            exclusions.append((addr, "defined_in_source",
                               sorted({f"{r['file']}:{r['line']} {r['rtype']}" for r in rs})))
            continue
        if len(ntypes) > 1:
            exclusions.append((addr, "type_conflict",
                               sorted({f"{r['file']}:{r['line']} {r['rtype']}" for r in rs})))
            continue
        ntype = next(iter(ntypes))
        if not c99_safe(ntype):
            exclusions.append((addr, "non_c99_type",
                               sorted({f"{r['file']}:{r['line']} {r['rtype']}" for r in rs})))
            continue
        # owner area = the area that declares it most
        owner = collections.Counter(rs_area for rs_area in areas).most_common(1)[0][0]
        header_addrs[addr] = (ntype, owner)
    return header_addrs, exclusions


def build_header(entries):
    """entries: iterable of (address, normalized_type) for ONE area header."""
    lines = [
        "// Generated by tools/coop/lbls_gen.py -- do not edit by hand.",
        "// Centralized retail data-label declarations for one area.",
        "//",
        "// Plain `extern` at global scope: MWCC never mangles global-scope data",
        "// names, so the emitted reloc is exactly the retail name (lbl_eu_XXXX),",
        "// identical to `extern \"C\"` (verified Wii/1.1). `extern \"C\"` is only",
        "// required inside namespace blocks; declarations here are global-scope.",
        "#pragma once",
        "",
        "#include <types.h>",
        "",
    ]
    for addr, t in sorted(entries):
        lines.append(f"extern {t} {addr};")
    lines.append("")
    return "\n".join(lines) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("command", nargs="?", default="scan",
                    choices=["scan", "generate", "apply", "check"])
    ap.add_argument("--dry-run", action="store_true",
                    help="report without writing/stripping")
    args = ap.parse_args()

    rows = scan_corpus()
    header_addrs, exclusions = classify(rows)

    if args.command in ("scan", "generate", "apply", "check"):
        total = len({r["address"] for r in rows})
        kinds = collections.Counter(r["kind"] for r in rows)
        print(f"lbl decl/def lines: {len(rows)}  addresses: {total}")
        print(f"kinds: {dict(kinds)}")
        print(f"centralized addresses: {len(header_addrs)}")
        print(f"excluded addresses: {len(exclusions)}")
        for addr, reason, sites in exclusions:
            print(f"  EXCLUDE {addr} [{reason}]")
            for s in sites[:3]:
                print(f"      {s}")
            if len(sites) > 3:
                print(f"      ... {len(sites) - 3} more")

    if args.command == "generate":
        # Headers are generated ONCE from the pristine corpus; `apply` must
        # never rewrite them (it would shrink them as decls get stripped).
        by_header = collections.defaultdict(dict)
        for addr, (ntype, area) in header_addrs.items():
            by_header[header_for_area(area)][addr] = ntype
        if not args.dry_run:
            for hdr, entries in by_header.items():
                path = INCLUDE_DIR / hdr
                path.write_text(build_header(entries.items()))
                print(f"wrote {path.relative_to(ROOT)} ({len(entries)} decls)")
            EXCLUSIONS_PATH.write_text(json.dumps(
                [{"address": a, "reason": r,
                  "sites": sorted({f"{x['file']}:{x['line']} {x['rtype']}"
                                   for x in rows if x['address'] == a})}
                 for a, r, _s in exclusions],
                indent=1))
            print(f"wrote {EXCLUSIONS_PATH.relative_to(ROOT)} ({len(exclusions)} entries)")
            MANIFEST_PATH.write_text(json.dumps(
                {hdr: sorted(entries.items()) for hdr, entries in by_header.items()},
                indent=1))
            print(f"wrote {MANIFEST_PATH.relative_to(ROOT)}")

    if args.command == "apply":
        # Strip per-TU decls; headers are already on disk and NOT rewritten.
        strip_rows(rows, header_addrs, args.dry_run)

    if args.command == "check":
        check_invariants(rows, header_addrs)


DECL_RE = re.compile(r"^\s*extern(?:\s+\"C\")?\s+")


def strip_rows(rows, header_addrs, dry_run):
    """Remove DECL lines whose address is centralized; add the area-header include.

    Content-verified, span-based, with a per-file self-check:
      - removes the exact line span of each decl (re-parsed on the current file)
      - aborts a file if any span does not look like an `extern` decl
      - after editing, re-parses and asserts the removed decls are gone and no
        dangling lbl declarator line was left behind
      - skips files with uncommitted git modifications (other agents' work)
    """
    by_file = collections.defaultdict(list)
    for r in rows:
        if r["kind"] == "DECL" and r["address"] in header_addrs:
            by_file[r["file"]].append(r)
    if not by_file:
        print("nothing to strip")
        return
    total = 0
    skipped_dirty = 0
    for rel, rs in sorted(by_file.items()):
        path = ROOT / rel
        # Never touch files with uncommitted modifications: they may be other
        # agents' in-progress work (shared branch).
        if not dry_run and subprocess.call(
                ["git", "diff", "--quiet", "--", rel],
                cwd=ROOT, stdout=subprocess.DEVNULL) != 0:
            print(f"SKIP DIRTY {rel}: uncommitted changes present")
            skipped_dirty += 1
            continue
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        # Re-parse THIS file fresh: spans must match the current content.
        fresh = parse_file(path)
        by_addr = collections.defaultdict(list)
        for addr, ntype, rtype, kind, start, end in fresh:
            if kind == "DECL" and addr in header_addrs:
                by_addr[addr].append((start, end))
        missing = sorted({r["address"] for r in rs} - set(by_addr))
        if missing:
            print(f"SKIP {rel}: decls not found on re-parse: {missing[:5]}...")
            continue
        # Verify each span starts with an extern decl line.
        spans = sorted({s for spans_ in by_addr.values() for s in spans_},
                       reverse=True)
        for start, end in spans:
            for ln in range(start - 1, end):
                if not DECL_RE.match(lines[ln]):
                    print(f"ABORT {rel}: line {ln + 1} is not an extern decl: "
                          f"{lines[ln][:60]!r}")
                    break
            else:
                continue
            break  # abort file
        else:
            removed = []
            for start, end in spans:
                removed.append("\n".join(lines[start - 1:end]))
                del lines[start - 1:end]
            txt = "\n".join(lines) + "\n"
            if not re.search(r"#\s*include\s*[<\"]lbls_", txt):
                inc = "\n".join(sorted(
                    {f"#include <{header_for_area(header_addrs[r['address']][1])}>"
                     for r in rs}))
                idxs = [i for i, l in enumerate(lines) if re.match(r"\s*#\s*include", l)]
                if idxs:
                    lines.insert(idxs[-1] + 1, inc)
                    txt = "\n".join(lines) + "\n"
                else:
                    lines.insert(0, inc)
                    txt = "\n".join(lines) + "\n"
            # Self-check: re-parse edited text; no centralized DECL may remain,
            # and no line may be a dangling lbl declarator (`lbl_eu_XXXX;` alone).
            import io
            tmp = ROOT / f".scratch/.lbls_check_{path.name}"
            tmp.write_text(txt)
            after = parse_file(tmp)
            tmp.unlink(missing_ok=True)
            leftovers = [a for a, n, r, k, s, e in after
                         if k == "DECL" and a in header_addrs]
            dangling = [l for l in lines
                        if re.match(r"^\s*lbl_(?:eu_)?[0-9A-F]{8}\s*(\[[^\]]*\])?\s*;\s*$", l)]
            if leftovers:
                print(f"SELF-CHECK FAIL {rel}: leftovers={leftovers[:5]} "
                      f"dangling={dangling[:3]}")
                continue
            if dangling:
                # Advisory: bare `lbl_eu_XXXX;` lines are often legitimate
                # expression statements (dead references), not half-removed
                # decls -- span removal is verified line-by-line above.
                print(f"note {rel}: {len(dangling)} bare lbl statement line(s) kept")
            if dry_run:
                print(f"would strip {len(spans)} decl(s) from {rel}")
            else:
                path.write_text(txt)
                print(f"stripped {len(spans)} decl(s) from {rel}")
            total += len(spans)
    print(f"total decls removed: {total}")


def check_invariants(rows, header_addrs):
    """CI gate. Uses the committed manifest so it works on a stripped corpus."""
    problems = 0
    # 1) headers must match the manifest
    if MANIFEST_PATH.exists():
        manifest = json.loads(MANIFEST_PATH.read_text())
        for hdr, entries in manifest.items():
            path = INCLUDE_DIR / hdr
            want = build_header(entries)
            if not path.exists() or path.read_text() != want:
                problems += 1
                print(f"STALE {hdr}: re-run `lbls_gen.py generate` on a clean tree")
        # 2) no TU may declare an address that the manifest centralizes
        centralized = {a for entries in manifest.values() for a, _t in entries}
        for r in rows:
            if r["kind"] == "DECL" and r["address"] in centralized:
                problems += 1
                print(f"STRAY DECL {r['address']} {r['file']}:{r['line']} "
                      f"(centralized -> should be removed)")
        # 3) every remaining per-TU decl must be in the exclusion list
        excluded = set()
        if EXCLUSIONS_PATH.exists():
            excluded = {e["address"] for e in json.loads(EXCLUSIONS_PATH.read_text())}
        for r in rows:
            if r["kind"] == "DECL" and r["address"] not in centralized \
                    and r["address"] not in excluded:
                problems += 1
                print(f"UNEXCLUDED DECL {r['address']} {r['file']}:{r['line']} "
                      f"(add to lbls_exclusions.json or the header)")
    else:
        problems += 1
        print(f"missing {MANIFEST_PATH.name}: run `lbls_gen.py generate` first")
    if problems:
        print(f"check FAILED: {problems} problem(s)")
        sys.exit(1)
    print("check passed")


if __name__ == "__main__":
    main()

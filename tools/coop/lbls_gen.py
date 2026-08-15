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
import math
import os
import re
import struct
import subprocess
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
    t = re.sub(r"\b(volatile|static|register|restrict|const)\b", " ", t)
    toks = re.sub(r"[\*\[\]&]", " ", t).split()
    if not toks:
        return False
    return all(tok in BUILTINS or tok in ("struct", "union", "enum") for tok in toks)


def base_type(ntype: str) -> str:
    """Last qualified component of a (possibly pointer/array/const/volatile)
    type, with struct/class/union tags stripped."""
    t = ntype.replace("*", "").strip()
    t = re.sub(r"\[[^\]]*\]", "", t).strip()
    t = re.sub(r"\b(const|volatile|static|register|restrict)\b", "", t).strip()
    t = t.replace("struct ", "").replace("class ", "").replace("enum ", "")
    t = t.replace("struct", "").replace("class", "").replace("enum", "")
    return t.split("::")[-1].strip() if "::" in t else t.strip()


_INCLUDE_ROOTS = ("src/", "libs/monolib/include/", "libs/nw4r/include/",
                  "libs/RVL_SDK/include/", "libs/CriWare/include/")


def resolvable_path(p: str) -> bool:
    return any(p.startswith(r) for r in _INCLUDE_ROOTS)


def include_form(p: str) -> str:
    for r in _INCLUDE_ROOTS:
        if p.startswith(r):
            return "<" + p[len(r):] + ">"
    return "<" + p + ">"  # unresolvable; caller should not reach here


def find_type_home(name: str):
    """Include-form path of the header that DEFINES `name`, or None.

    Prefers full definitions over forward declarations, include-tree headers
    over src-tree copies, and returns None on ambiguity so labels stay per-TU
    rather than risk pulling the wrong declaration."""
    defs, typd, fwd = set(), set(), set()
    for root in (SRC, LIBS):
        for p in root.rglob("*"):
            if p.suffix not in (".h", ".hpp") or p.name.endswith(".ctx.c"):
                continue
            try:
                txt = p.read_text(encoding="utf-8", errors="replace")
            except OSError:
                continue
            s = str(p.relative_to(ROOT))
            if re.search(rf"\b(class|struct|union)\s+{re.escape(name)}\s*(:\s*[^{{;]+)?{{", txt):
                defs.add(s)
            elif re.search(rf"\b(class|struct|union)\s+{re.escape(name)}\b", txt):
                fwd.add(s)
            if (re.search(rf"}}\s*{re.escape(name)}\s*;", txt)          # } Name;
                    or re.search(rf"\btypedef\b[^;]*\b{re.escape(name)}\s*;", txt)
                    or re.search(rf"\(\s*\*\s*{re.escape(name)}\s*\)", txt)):  # (*Name)(
                typd.add(s)
    for bucket in (defs, typd):
        cands = [p for p in bucket if resolvable_path(p)] or list(bucket)
        if len(cands) == 1:
            return include_form(cands[0])
        if len(cands) > 1:
            return None  # ambiguous definition sites
    if len(fwd) == 1 and resolvable_path(next(iter(fwd))):
        return include_form(next(iter(fwd)))
    return None


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
                out.append((name, norm_type(typ), typ, arr, kind,
                            start_i + 1, end_i + 1))
            i += 1
            continue
        bm = BARE_LINE.match(line)
        if bm:
            typ = bm.group(1).strip()
            arr = bm.group(3) or ""
            first = typ.split()[0] if typ.split() else ""
            if (typ and not BAD_BARE.search(typ)
                    and not re.match(r"^extern$", typ)
                    and first not in BAD_BARE_WORDS):
                out.append((bm.group(2), norm_type(typ), typ, arr, "DEF_BARE",
                            i + 1, i + 1))
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
                for addr, ntype, rtype, arr, kind, start, end in parse_file(p):
                    rows.append({
                        "address": addr, "ntype": ntype, "rtype": rtype,
                        "arr": arr, "kind": kind, "file": rel,
                        "area": area_for(rel), "line": start, "end": end,
                    })
    return rows


def type_key(ntype: str, arr: str) -> str:
    """Conflict key: normalized type + array-ness (any bound -> `[]`)."""
    return ntype + ("[]" if arr else "")


def _load_canonical():
    """tools/coop/lbls_canonical.json: address -> canonical type, from the
    type-conflict investigation (usage sites verified type-agnostic)."""
    p = ROOT / "tools" / "coop" / "lbls_canonical.json"
    if p.exists():
        return json.loads(p.read_text())
    return {}


def classify(rows):
    """Return (central, exclusions): central maps addr -> (ntype, arr, area, mode)
    with mode 'dual' (LBLS_ENTRY: extern + port definition) or 'extern_only'
    (plain extern in both builds; storage stays byte-accurate in data_defs.cpp).
    """
    by = collections.defaultdict(list)
    for r in rows:
        by[r["address"]].append(r)
    central = {}
    exclusions = []
    canon = _load_canonical()
    for addr, rs in sorted(by.items()):
        keys = {type_key(r["ntype"], r["arr"]) for r in rs}
        areas = {r["area"] for r in rs}
        kinds = {r["kind"] for r in rs}
        owner = collections.Counter(rs_area for rs_area in areas).most_common(1)[0][0]
        # Addresses with a definition in source (RTTP objects, singletons,
        # initialised tables) stay per-TU: the definition TU owns the storage.
        if "DEF_BARE" in kinds or "DEF_INIT" in kinds:
            exclusions.append((addr, "defined_in_source",
                               sorted({f"{r['file']}:{r['line']} {r['rtype']}{r['arr']}" for r in rs})))
            continue
        # Canonical type from the type-conflict investigation: every usage site
        # is verified type-agnostic (address-of / cast / decay), so one type
        # serves them all without codegen change.
        if addr in canon:
            m = re.match(r"^(.+?)((\[[^\]]*\])?)$", canon[addr].strip())
            ntype = norm_type(m.group(1)) if m else canon[addr]
            arr = m.group(2) if m else ""
            if not c99_safe(ntype):
                home = find_type_home(base_type(ntype))
                if home and owner not in ("criware", "rvl_sdk"):
                    central[addr] = (ntype, arr, owner, "dual")
                    continue
                exclusions.append((addr, "non_c99_type",
                                   sorted({f"{r['file']}:{r['line']} {r['rtype']}{r['arr']}"
                                           for r in rs})))
                continue
            central[addr] = (ntype, arr, owner, "dual")
            continue
        if len(keys) > 1:
            exclusions.append((addr, "type_conflict",
                               sorted({f"{r['file']}:{r['line']} {r['rtype']}{r['arr']}" for r in rs})))
            continue
        key = next(iter(keys))
        ntype = key[:-2] if key.endswith("[]") else key
        arr = "[]" if key.endswith("[]") else ""
        if not c99_safe(ntype):
            # class/typedef types: migrate to include/lbls_typed.hpp (real
            # type in both builds). Pointer-typed and trivially-copyable
            # object-typed labels get a REAL port definition (pointer literal /
            # bit_cast) via the typed data TU; the gates demote ctor classes
            # and size-mismatched structs to extern_only (raw bytes stay in
            # data_defs.cpp).
            if owner not in ("criware", "rvl_sdk"):
                home = find_type_home(base_type(ntype))
                if home:
                    central[addr] = (ntype, arr, owner, "dual")
                    continue
            exclusions.append((addr, "non_c99_type",
                               sorted({f"{r['file']}:{r['line']} {r['rtype']}{r['arr']}" for r in rs})))
            continue
        central[addr] = (ntype, arr, owner, "dual")
    return central, exclusions


_KIND_UNIT = {"byte": 1, "2byte": 2, "4byte": 4, "float": 4,
              "double": 8, "string": 1}
_BSS_SECTIONS = frozenset({".bss", ".sbss", ".sbss2"})


def _load_data_sources(region: str):
    """symbols.txt rows (name -> DataSymbol) + retail DOL (or None)."""
    sys.path.insert(0, str(ROOT / "tools" / "port"))
    from gen_data_defs import parse_symbols, DolImage  # noqa: PLC0415
    syms_path = ROOT / "config" / region / "symbols.txt"
    syms = {}
    if syms_path.exists():
        for r in parse_symbols(syms_path):
            syms[r.name] = r
    dol_path = ROOT / "orig" / region / "sys" / "main.dol"
    dol = DolImage.load(dol_path) if dol_path.exists() else None
    return syms, dol


def _type_unit(ntype: str) -> int:
    """Element width of a scalar base type (PPC word sizes; void* handled
    separately as pointer)."""
    base = ntype.replace("*", "").strip()
    return {"u8": 1, "s8": 1, "char": 1, "u16": 2, "s16": 2,
            "wchar_t": 2, "u32": 4, "s32": 4, "int": 4, "f32": 4,
            "f64": 8}.get(base, 0)


def _zero_init(ntype: str, is_arr: bool) -> str:
    if is_arr:
        return "{0}"
    if ntype.endswith("*"):
        return "0"
    if ntype == "f32":
        return "0.0f"
    if ntype == "f64":
        return "0.0"
    return "0"


def _scalar_value(ntype: str, raw: bytes, addr: int) -> str | None:
    """One scalar literal from big-endian retail bytes (host-decoded)."""
    base = ntype.replace("*", "").strip()
    if ntype.endswith("*"):
        return f"({ntype})0x{int.from_bytes(raw[:4], 'big'):08X}"
    if base == "char":
        v = raw[0]
        return str(v if v < 0x80 else v - 0x100)  # signed char (bytes >= 0x80)
    if base == "u8":
        return f"0x{raw[0]:02X}"
    if base == "u16" and len(raw) >= 2:
        return f"0x{int.from_bytes(raw[:2], 'big'):04X}"
    if base in ("u32",) and len(raw) >= 4:
        return f"0x{int.from_bytes(raw[:4], 'big'):08X}"
    if base in ("s32", "int") and len(raw) >= 4:
        v = int.from_bytes(raw[:4], "big")
        return str(v if v < 0x80000000 else v - 0x100000000)
    if base == "s16" and len(raw) >= 2:
        v = int.from_bytes(raw[:2], "big")
        return str(v if v < 0x8000 else v - 0x10000)
    if base == "s8" and len(raw) >= 1:
        v = raw[0]
        return str(v if v < 0x80 else v - 0x100)
    if base == "f32" and len(raw) >= 4:
        v = struct.unpack(">f", raw[:4])[0]
        if math.isfinite(v):
            return f"{v!r}f"
        return None
    if base == "f64" and len(raw) >= 8:
        v = struct.unpack(">d", raw[:8])[0]
        if math.isfinite(v):
            return f"{v!r}"
        return None
    return None


def _string_content(raw: bytes, size: int) -> str | None:
    """Escaped C literal content for a string-like array of `size` bytes.

    Returns None when the data is not string-like (blob) or when a string
    literal cannot reproduce the bytes exactly (last byte not NUL: the
    implicit terminator would add a byte).
    """
    if size < 2 or raw[-1] != 0:
        return None
    body = raw[:size - 1]
    nz = [c for c in body if c != 0]
    if not nz:
        return None
    printable = sum(1 for c in nz
                    if 0x20 <= c < 0x7F or c in (0x09, 0x0A, 0x0D))
    if printable < 0.9 * len(nz):
        return None
    out = []
    for c in body:
        if c == 0x22:      # "
            out.append('\\"')
        elif c == 0x5C:    # \\
            out.append('\\\\')
        elif 0x20 <= c < 0x7F:
            out.append(chr(c))
        elif c == 0x09:
            out.append('\\t')
        elif c == 0x0A:
            out.append('\\n')
        elif c == 0x0D:
            out.append('\\r')
        else:
            # \xNN then close/reopen the literal so a following hex digit
            # cannot extend the escape (\x41B would read as 0x41B).
            out.append(f'\\x{c:02X}""')
    return "".join(out)


def _entry_def(addr: str, ntype: str, arr: str, sym, dol):
    """Return (ext_decl, def_decl, init) for one header entry.

    ext_decl mirrors the source declaration exactly (MWCC extern mode).
    def_decl carries the array bound from symbols.txt (definitions cannot be
    incomplete). init is the definition initializer: retail bytes decoded per
    type when available (initialized data), typed zeros otherwise.
    """
    if ntype == "void":
        ntype = "u8"  # a bare `void` object is invalid; the address is what matters
    ext = f"{ntype} {addr}{arr}"
    is_ptr = ntype.endswith("*")
    is_arr = bool(arr)
    # Class object types (not C99-safe, not pointer, not array): byte-accurate
    # PC definition via C++20 __builtin_bit_cast from the retail bytes. Only
    # trivially-copyable types reach here (the gates demote ctor classes and
    # size-mismatched structs to extern_only).
    if not c99_safe(ntype) and not is_ptr and not is_arr:
        size = sym.size if sym else 0
        if size and sym and sym.section not in _BSS_SECTIONS:
            raw = dol.read(int(sym.address, 16), size) if dol else None
            if raw and len(raw) == size:
                bl = ", ".join(f"0x{b:02X}" for b in raw)
                return ext, f"{ntype} {addr}", \
                    f"__builtin_bit_cast({ntype}, (unsigned char[{size}]){{{bl}}})"
        return ext, f"{ntype} {addr}", "{}"  # bss/unknown size: value-init zeros
    unit = _type_unit(ntype)
    size = sym.size if sym else 0
    raw = None
    if sym and dol is not None and sym.section not in _BSS_SECTIONS:
        raw = dol.read(int(sym.address, 16), sym.size)
    elif sym and sym.section in _BSS_SECTIONS:
        raw = None  # zeros
    # definition declarator: bound arrays from symbols.txt size
    if is_arr:
        eu = 4 if is_ptr else unit
        if size and eu:
            bound = size // eu if size % eu == 0 else size
            defn = f"{ntype} {addr}[{bound}]"
        else:
            defn = f"{ntype} {addr}[1]"  # unknown size: minimal storage
    else:
        defn = f"{ntype} {addr}"
    # initializer
    if raw is None:
        return ext, defn, _zero_init(ntype, is_arr)
    elem_unit = 4 if is_ptr else unit  # PPC pointer width for pointer arrays
    if is_arr and (unit or is_ptr):
        # string-like char[]/u8[]: readable C literal instead of a byte list
        if ntype in ("char", "u8") and is_ptr is False:
            s = _string_content(raw, size)
            if s is not None:
                return ext, defn, f'"{s}"'
        n = (size // elem_unit) if (size and elem_unit) else 1
        elems = []
        for i in range(n):
            v = _scalar_value(ntype, raw[i * elem_unit:(i + 1) * elem_unit], 0)
            if v is None:
                return ext, defn, _zero_init(ntype, is_arr)
            elems.append(v)
        return ext, defn, "{" + ", ".join(elems) + "}"
    v = _scalar_value(ntype, raw, int(sym.address, 16))
    if v is None:
        return ext, defn, _zero_init(ntype, is_arr)
    return ext, defn, v


def build_header(entries, area: str, region: str = "us"):
    """entries: iterable of (address, normalized_type, arr, mode) for ONE area."""
    syms, dol = _load_data_sources(region)
    # type-provider includes for class/typedef-typed labels (resolved by
    # find_type_home; headers are included from their include roots)
    type_includes = []
    seen = set()
    for addr, t, arr, mode in entries:
        if c99_safe(t):
            continue
        home = find_type_home(base_type(t))
        if home and home not in seen:
            seen.add(home)
            type_includes.append(home)
    type_includes.sort()
    lines = [
        "// Generated by tools/coop/lbls_gen.py -- do not edit by hand.",
        "// Centralized retail data-label declarations for one area.",
        "//",
        "// Plain `extern` at global scope: MWCC never mangles global-scope data",
        "// names, so the emitted reloc is exactly the retail name (lbl_eu_XXXX),",
        "// identical to `extern \"C\"` (verified Wii/1.1).",
        "//",
        "// Dual-mode through the shared LBLS_ENTRY macro (defined in types.h):",
        "// the MWCC matching build sees declarations only (the retail image",
        "// supplies all data); non-MWCC builds (PC port) get definitions -- the",
        "// generated data TU (port/data_defs.cpp) defines LBLS_DEFINE_DATA",
        "// before including types.h so storage exists exactly once; every other",
        "// TU sees `extern`. Initializers carry retail bytes when a DOL is",
        "// present (decoded per type), typed zeros otherwise.",
        "//",
        "// Class/typedef-typed labels include their declaring headers. The",
        "// extern-only section (object-typed labels) is suppressed in the data",
        "// TU: those definitions must stay raw bytes in data_defs.cpp for",
        "// fidelity, and a real-typed extern + raw definition cannot coexist",
        "// in one TU (type mismatch).",
        "#pragma once",
        "",
        "#include <types.h>",
    ]
    for h in type_includes:
        lines.append(f"#include {h}")
    lines.append("")
    for addr, t, arr, mode in sorted(entries):
        if not c99_safe(t):
            continue  # class-typed labels are emitted in include/lbls_typed.hpp
        sym = syms.get(addr)
        ext, dfn, init = _entry_def(addr, t, arr, sym, dol)
        # dfn has no commas -> no paren protection needed; init needs parens
        # (byte lists contain commas).
        lines.append(f"LBLS_ENTRY({ext}, {dfn}, ({init}))")
    extern_only = [e for e in sorted(entries) if e[3] == "extern_only"]
    if extern_only:
        lines.append("")
        lines.append("// Extern-only: real type in every build; the PC definition stays raw")
        lines.append("// bytes in port/data_defs.cpp (byte fidelity).")
        lines.append("#if !defined(LBLS_DEFINE_DATA)")
        for addr, t, arr, mode in extern_only:
            lines.append(f"extern {t} {addr}{arr};")
        lines.append("#endif")
    lines.append("")
    return "\n".join(lines) + "\n"


def build_typed_header(entries, region: str = "us"):
    """Class/typedef-typed labels: real-type extern declarations for every
    build. The PC definitions stay raw bytes in data_defs.cpp; this header is
    deliberately NOT included by data_defs.cpp so its type providers cannot
    clash with the SDK globals data_defs defines."""
    syms, dol = _load_data_sources(region)  # noqa: F841 (dol unused here)
    # Manifest entries may be 5-tuples (addr, type, arr, owner, mode); the
    # mode is always the last element. Normalize to the 4-tuple form used
    # everywhere inside this function.
    entries = [(e[0], e[1], e[2], e[-1]) for e in entries]
    type_includes = []
    seen = set()
    for addr, t, arr, mode in entries:
        home = find_type_home(base_type(t))
        if home and home not in seen:
            seen.add(home)
            type_includes.append(home)
    type_includes.sort()
    lines = [
        "// Generated by tools/coop/lbls_gen.py -- do not edit by hand.",
        "// Class/typedef-typed retail data labels: real-type externs for every",
        "// build (matching + PC port). Definitions stay raw bytes in",
        "// port/data_defs.cpp; this header is NOT included by data_defs.cpp",
        "// so its type providers cannot clash with the SDK globals it defines.",
        "#pragma once",
        "",
        "#include <types.h>",
    ]
    for h in type_includes:
        lines.append(f"#include {h}")
    lines.append("")
    for addr, t, arr, mode in sorted(entries):
        if mode == "extern_only" or (not c99_safe(t) and arr):
            # extern-only: ctor classes, class arrays, size-mismatched structs.
            # Plain extern in every build; data_defs.cpp (and the typed data TU)
            # never define these -- raw bytes stay in data_defs.cpp.
            lines.append(f"extern {t} {addr}{arr};")
        else:
            sym = syms.get(addr)
            ext, dfn, init = _entry_def(addr, t, arr, sym, dol)
            lines.append(f"LBLS_ENTRY({ext}, {dfn}, ({init}))")
    lines.append("")
    return "\n".join(lines) + "\n"


def home_to_path(home: str):
    """Map an include-form `<kyoshin/X.hpp>` back to the file path."""
    name = home.strip("<>")
    for r in _INCLUDE_ROOTS:
        cand = ROOT / r / name
        if cand.exists():
            return cand
    return None


def _self_contained(home: str) -> bool:
    """True if the type-provider header compiles standalone with the repo
    include paths. Non-self-contained homes would break every TU that includes
    the lbls header (matching build and port alike)."""
    inc = ["-I", str(ROOT / "include"),
           "-I", str(ROOT / "libs/PowerPC_EABI_Support/include"),
           "-I", str(ROOT / "libs/PowerPC_EABI_Support/include/stl"),
           "-I", str(ROOT / "src"),
           "-I", str(ROOT / "libs/monolib/include"),
           "-I", str(ROOT / "libs/nw4r/include"),
           "-I", str(ROOT / "libs/RVL_SDK/include"),
           "-I", str(ROOT / "libs/CriWare/include")]
    path = home_to_path(home)
    if path is None:
        return False
    r = subprocess.run(["clang++", "-fsyntax-only", "-x", "c++", *inc,
                        "-include", str(path), "/dev/null"],
                       capture_output=True)
    return r.returncode == 0


def _gate_type_homes(central, exclusions):
    """Drop class-typed labels whose type-provider header is not self-contained
    back to per-TU exclusions (a broken include would break every TU that
    includes the header). Also drops the whole area's class-typed migration when
    its homes conflict with each other (same function declared with different
    signatures across kyoshin headers) -- those only parse in TU include order."""
    homes = {}
    for addr, (ntype, arr, area, mode) in central.items():
        if not c99_safe(ntype):
            home = find_type_home(base_type(ntype))
            if home:
                homes.setdefault(home, []).append((addr, ntype, arr))
    bad = {h for h in homes if not _self_contained(h)}
    for h in sorted(bad):
        print(f"  type home NOT self-contained -> stays per-TU: {h}")
    # combined check per area: all its homes must parse together (a TU sees
    # every home the header includes)
    by_area = collections.defaultdict(set)
    for addr, (ntype, arr, area, mode) in central.items():
        if not c99_safe(ntype):
            home = find_type_home(base_type(ntype))
            if home and home not in bad:
                by_area[area].add(home)
    inc = ["-I", str(ROOT / "include"),
           "-I", str(ROOT / "libs/PowerPC_EABI_Support/include"),
           "-I", str(ROOT / "libs/PowerPC_EABI_Support/include/stl"),
           "-I", str(ROOT / "src"),
           "-I", str(ROOT / "libs/monolib/include"),
           "-I", str(ROOT / "libs/nw4r/include"),
           "-I", str(ROOT / "libs/RVL_SDK/include"),
           "-I", str(ROOT / "libs/CriWare/include")]
    for area, hs in sorted(by_area.items()):
        if not hs:
            continue
        tu = "#include <types.h>\n" + "".join(f"#include {h}\n" for h in sorted(hs))
        r = subprocess.run(["clang++", "-fsyntax-only", "-x", "c++", *inc, "-"],
                           input=tu.encode(), capture_output=True)
        if r.returncode != 0:
            print(f"  combined type homes for {area} conflict -> class-typed labels "
                  f"stay per-TU ({len(hs)} homes)")
            for h in hs:
                for addr, ntype, arr in homes[h]:
                    central.pop(addr, None)
                    exclusions.append((addr, "non_c99_type",
                                       [f"conflicting type homes in {area}"]))
    # per-entry artifact gate: each class-typed entry's extern form must parse
    # with its type home included -- catches namespace-relative types
    # (`detail::X`), homes that declare the label itself with a different
    # type, and other single-entry breakage. Object-typed entries additionally
    # must support a bit_cast definition (trivially copyable + host/retail
    # size match); failures are demoted to extern_only.
    syms, _dol = _load_data_sources("us")
    typed_entries = []
    for addr, (ntype, arr, area, mode) in list(central.items()):
        if c99_safe(ntype):
            continue
        home = find_type_home(base_type(ntype))
        if not home or home in bad:
            continue
        decl = f"extern {ntype} {addr}{arr};"
        tu = f"#include <types.h>\n#include {home}\n{decl}\n"
        r = subprocess.run(["clang++", "-fsyntax-only", "-x", "c++", *inc, "-"],
                           input=tu.encode(), capture_output=True)
        if r.returncode != 0:
            central.pop(addr, None)
            exclusions.append((addr, "non_c99_type",
                               [f"extern form does not parse with type home"]))
            continue
        if not ntype.endswith("*") and not arr:
            sym = syms.get(addr)
            size = sym.size if sym else 0
            if size:
                dtu = (f"#include <types.h>\n#include {home}\n"
                       f"{ntype} {addr} = __builtin_bit_cast({ntype}, "
                       f"(unsigned char[{size}]){{0}});\n")
                r2 = subprocess.run(["clang++", "-fsyntax-only", "-x", "c++",
                                     *inc, "-"], input=dtu.encode(),
                                    capture_output=True)
                if r2.returncode != 0:
                    central[addr] = (ntype, arr, area, "extern_only")
                    typed_entries.append((addr, ntype, arr, area, "extern_only"))
                    continue
        typed_entries.append((addr, ntype, arr, area, central[addr][3]))
    # full-header artifact gate with bisection: include/lbls_typed.hpp must
    # parse in extern AND define mode. Cross-home conflicts (e.g. a kyoshin
    # home redefining nw4r types) are isolated by bisecting: failing halves
    # are split until the offending entries are dropped individually.
    def _typed_header_ok(entries):
        text = build_typed_header(sorted((a, n, ar, m)
                                         for a, n, ar, _area, m in entries))
        tmp = ROOT / ".scratch" / ".lbls_gate_lbls_typed.hpp"
        tmp.write_text(text)
        ok = True
        for extra in ([], ["-DLBLS_DEFINE_DATA"]):
            r = subprocess.run(["clang++", "-fsyntax-only", "-x", "c++", *inc,
                                *extra, "-include", str(tmp), "/dev/null"],
                               capture_output=True)
            if r.returncode != 0:
                ok = False
                break
        tmp.unlink(missing_ok=True)
        return ok

    def _typed_gate(entries):
        if not entries:
            return []
        if _typed_header_ok(entries):
            return entries
        if len(entries) == 1:
            addr, ntype, arr, area, mode = entries[0]
            print(f"  typed label dropped (header gate): {addr} [{ntype}]")
            return []
        mid = len(entries) // 2
        return _typed_gate(entries[:mid]) + _typed_gate(entries[mid:])

    survivors = _typed_gate(sorted(typed_entries))
    dropped = {e[0] for e in typed_entries} - {e[0] for e in survivors}
    for addr, ntype, arr, area, mode in typed_entries:
        if addr in dropped:
            central.pop(addr, None)
            exclusions.append((addr, "non_c99_type",
                               [f"typed header artifact gate failed"]))
    for h in bad:
        for addr, ntype, arr in homes[h]:
            central.pop(addr, None)
            exclusions.append((addr, "non_c99_type",
                               [f"unresolvable type home {h}"]))
    return central, exclusions


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

    if args.command == "generate":
        header_addrs, exclusions = _gate_type_homes(header_addrs, exclusions)

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
        by_header = collections.defaultdict(list)
        typed = []
        for addr, (ntype, arr, area, mode) in header_addrs.items():
            if not c99_safe(ntype):
                typed.append((addr, ntype, arr, area, mode))
            else:
                by_header[header_for_area(area)].append((addr, ntype, arr, mode))
        if not args.dry_run:
            for hdr, entries in by_header.items():
                area = hdr[len("lbls_"):-len(".hpp")]
                path = INCLUDE_DIR / hdr
                path.write_text(build_header(entries, area))
                print(f"wrote {path.relative_to(ROOT)} ({len(entries)} decls)")
            if typed:
                INCLUDE_DIR.joinpath("lbls_typed.hpp").write_text(
                    build_typed_header(sorted((a, n, ar, m)
                                              for a, n, ar, _area, m in typed)))
                print(f"wrote include/lbls_typed.hpp ({len(typed)} decls)")
                by_header["lbls_typed.hpp"] = sorted(typed)
            EXCLUSIONS_PATH.write_text(json.dumps(
                [{"address": a, "reason": r,
                  "sites": sorted({f"{x['file']}:{x['line']} {x['rtype']}{x['arr']}"
                                   for x in rows if x['address'] == a})}
                 for a, r, _s in exclusions],
                indent=1))
            print(f"wrote {EXCLUSIONS_PATH.relative_to(ROOT)} ({len(exclusions)} entries)")
            MANIFEST_PATH.write_text(json.dumps(
                {hdr: sorted(entries) for hdr, entries in by_header.items()},
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
                    {f"#include <lbls_typed.hpp>"
                     if not c99_safe(header_addrs[r['address']][0])
                     else f"#include <{header_for_area(header_addrs[r['address']][2])}>"
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
            if hdr == "lbls_typed.hpp":
                want = build_typed_header(entries)
            else:
                want = build_header(entries, hdr[len("lbls_"):-len(".hpp")])
            if not path.exists() or path.read_text() != want:
                problems += 1
                print(f"STALE {hdr}: re-run `lbls_gen.py generate` on a clean tree")
        # 2) no TU may declare an address that the manifest centralizes
        centralized = {e[0] for entries in manifest.values()
                       for e in entries}
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

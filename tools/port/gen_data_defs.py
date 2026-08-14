#!/usr/bin/env python3
"""Generate a PC-port data-definition TU from the retail symbol map.

The MWCC matching build must keep *referencing* retail data (extern
declarations with the exact `lbl_*` names) and must NOT define it — defining
`u32 lbl_eu_80664184;` in a matched TU would add a data section retail doesn't
have and break the objdiff reloc match. So the definitions live in this
generated, port-only translation unit, compiled only when `__MWERKS__` is
undefined (or `NONMATCHING` is set), exactly like the PLAN.md §17.6 non-MWCC
fallbacks.

Every data symbol in config/<region>/symbols.txt (type:object, nameable)
becomes a definition. When a retail main.dol is available (default:
orig/<region>/sys/main.dol), initialized data (.data/.rodata/.sdata/.sdata2)
carries the actual RETAIL BYTES in big-endian order; bss-family symbols
(.bss/.sbss/.sbss2) are typed zeros. Without a DOL, everything is typed
zeros. Scalar kinds whose width matches decode to typed host values (bytes
are byte-swapped from the big-endian retail order).

Usage:
    .venv/bin/python3 tools/port/gen_data_defs.py --region us [--dry-run]
    .venv/bin/python3 tools/port/gen_data_defs.py --only-referenced --dry-run

Options:
    --region REGION       symbols.txt region (default: us)
    --out PATH            output file (default: port/data_defs.cpp)
    --only-referenced     emit only symbols actually referenced by src/ + libs/
    --from-dol PATH       retail main.dol (default: orig/<region>/sys/main.dol)
    --no-dol              typed zeros only, even if a DOL is present
    --dry-run             print stats, do not write
"""

from __future__ import annotations

import argparse
import json
import math
import re
import struct
import sys
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

_REPO = Path(__file__).resolve().parent.parent.parent

# Sections that are code or boot metadata — never data definitions.
_SKIP_SECTIONS = frozenset({".text", ".init", ".ctors", ".dtors"})

# data-kind -> scalar C type (from symbols.txt `data:` comment field).
_KIND_TYPES = {
    "byte": "u8",
    "2byte": "u16",
    "4byte": "u32",
    "float": "f32",
    "double": "f64",
    "string": "char",
}

# scope preference for duplicate names (recovered locals can repeat across
# units with the same name — e.g. `Enabled`, `BootInfo`).
_SCOPE_ORDER = {"global": 0, "weak": 1, "local": 2}

_IDENT_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")

# Sections with no file content (zero-fill) — typed zero definitions only.
_BSS_SECTIONS = frozenset({".bss", ".sbss", ".sbss2"})

# Host-typed scalar emission needs the kind -> width mapping (raw bytes are
# big-endian retail order; values are decoded for the host).
_KIND_UNIT = {"byte": 1, "2byte": 2, "4byte": 4, "float": 4,
              "double": 8, "string": 1}

# Identifiers that are macros or libc globals on common PC platforms (or C++
# keywords) — defining them in the port build would fail to compile or clash
# with the host CRT (e.g. `errno` is `#define errno (*__error())` on macOS/
# glibc). Retail MetroWerks CRT internals are otherwise harmless: on PC the
# host CRT replaces MWCRT, so their symbols are just unused globals.
_RESERVED = frozenset({
    "errno", "environ", "stdin", "stdout", "stderr", "optarg", "optind",
    "optopt", "EOF", "NULL", "offsetof", "va_list", "va_start", "va_end",
    "alignas", "alignof", "sizeof", "typeof", "asm", "bool", "true",
    "false", "nullptr", "new", "delete", "class", "struct", "union",
    "namespace", "template", "typename", "virtual", "friend", "operator",
    "explicit", "mutable", "constexpr", "noexcept", "thread_local",
})


@dataclass
class DataSymbol:
    name: str
    section: str
    address: str
    size: int
    scope: str
    align: int = 0
    kind: str = ""
    hidden: bool = False


@dataclass
class DolImage:
    """Retail main.dol with an address -> file-offset slice map."""
    segments: list[tuple[int, int, int]]  # (vaddr, file_off, size)
    bss_addr: int
    bss_size: int
    _data: bytes

    @classmethod
    def load(cls, path: Path) -> "DolImage":
        raw = path.read_bytes()
        hdr = raw[:0x100]
        off = struct.unpack(">18I", hdr[0x00:0x48])
        addr = struct.unpack(">18I", hdr[0x48:0x90])
        size = struct.unpack(">18I", hdr[0x90:0xD8])
        bss_a, bss_s = struct.unpack(">II", hdr[0xD8:0xE0])
        segments = [(a, o, s) for o, a, s in zip(off, addr, size) if s]
        return cls(segments, bss_a, bss_s, raw)

    def read(self, vaddr: int, size: int) -> bytes | None:
        """Raw retail bytes for [vaddr, vaddr+size), or None if not in a
        file-backed segment (bss/zero-fill or out of range)."""
        for a, o, s in self.segments:
            if a <= vaddr and vaddr + size <= a + s:
                return self._data[o + (vaddr - a): o + (vaddr - a) + size]
        return None

    def in_bss(self, vaddr: int, size: int) -> bool:
        return self.bss_addr <= vaddr and vaddr + size <= self.bss_addr + self.bss_size



def parse_symbols(symbols_path: Path) -> list[DataSymbol]:
    """Parse config/<region>/symbols.txt rows of the form:
    name = .section:0xADDR; // type:object size:0xN scope:global [align:N] [data:kind]
    """
    out: list[DataSymbol] = []
    row_re = re.compile(
        r"^(?P<name>\S+) = (?P<section>\.[A-Za-z0-9_]+):0x(?P<addr>[0-9A-Fa-f]+);"
        r"\s*//\s*(?P<comment>.*)$"
    )
    for line in symbols_path.read_text(encoding="utf-8").splitlines():
        if not line.strip():
            continue
        m = row_re.match(line)
        if not m:
            continue
        comment = m.group("comment")
        kind_m = re.search(r"\btype:(\w+)", comment)
        if not kind_m or kind_m.group(1) != "object":
            continue  # functions, labels, etc.
        size_m = re.search(r"\bsize:0x([0-9A-Fa-f]+)", comment)
        if not size_m:
            continue
        scope_m = re.search(r"\bscope:(\w+)", comment)
        align_m = re.search(r"\balign:(\d+)", comment)
        data_m = re.search(r"\bdata:(\w+)", comment)
        hidden = "hidden" in comment
        out.append(DataSymbol(
            name=m.group("name"),
            section=m.group("section"),
            address=m.group("addr"),
            size=int(size_m.group(1), 16),
            scope=scope_m.group(1) if scope_m else "local",
            align=int(align_m.group(1)) if align_m else 0,
            kind=data_m.group(1) if data_m else "",
            hidden=hidden,
        ))
    return out


def select(rows: list[DataSymbol]) -> list[DataSymbol]:
    """Filter to nameable, linkable data and dedupe by name."""
    kept: dict[str, DataSymbol] = {}
    skipped = defaultdict(int)
    for r in rows:
        if r.size <= 0:
            skipped["size 0"] += 1
            continue
        if r.section in _SKIP_SECTIONS:
            skipped[f"section {r.section}"] += 1
            continue
        if not _IDENT_RE.match(r.name):
            skipped["non-identifier name"] += 1
            continue
        if r.name in _RESERVED:
            skipped["reserved on PC (macro/keyword/libc)"] += 1
            continue
        prev = kept.get(r.name)
        if prev is None or _SCOPE_ORDER[r.scope] < _SCOPE_ORDER[prev.scope]:
            kept[r.name] = r
        elif r.scope == prev.scope:
            skipped["duplicate name (kept first)"] += 1
        else:
            skipped["duplicate name (kept better scope)"] += 1
    return list(kept.values()), dict(skipped)


def references(rows: list[DataSymbol], roots: list[Path], exclude: Path) -> set[str]:
    """Symbols whose name appears as an identifier anywhere under src/ + libs/.

    Fast path: tokenise each file ONCE and intersect with the name set, instead
    of running one regex search per symbol per file (13k names x files was
    ~65M regex scans).
    """
    names = {r.name for r in rows}
    token_re = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
    found: set[str] = set()
    file_re = re.compile(r"\.(?:cpp|hpp|h|inl|c)$")
    for root in roots:
        for p in root.rglob("*"):
            if p == exclude or not file_re.search(p.name) or not p.is_file():
                continue
            try:
                text = p.read_text(encoding="utf-8", errors="replace")
            except OSError:
                continue
            found |= names.intersection(token_re.findall(text))
    return found


def _hex(data: bytes) -> str:
    return ", ".join(f"0x{b:02X}" for b in data)


def _byte_array_decl(name: str, data: bytes, indent: str = "    ") -> str:
    """Raw byte array, one line when short, wrapped otherwise."""
    if len(data) <= 16:
        return f"u8 {name}[{len(data)}] = {{{_hex(data)}}};"
    lines = [f"u8 {name}[{len(data)}] = {{"]
    for i in range(0, len(data), 16):
        chunk = data[i:i + 16]
        tail = "," if i + 16 < len(data) else ""
        lines.append(f"{indent}{_hex(chunk)}{tail}")
    lines.append("};")
    return "\n".join(lines)


def _scalar_decl(ctype: str, name: str, raw: bytes) -> str | None:
    """Typed scalar from big-endian retail bytes; None when not representable
    as a plain literal (non-finite floats) — caller falls back to raw bytes."""
    if ctype == "u8":
        return f"{ctype} {name} = 0x{raw[0]:02X};"
    if ctype == "u16":
        return f"{ctype} {name} = 0x{int.from_bytes(raw, 'big'):04X};"
    if ctype == "u32":
        return f"{ctype} {name} = 0x{int.from_bytes(raw, 'big'):08X};"
    if ctype == "f32" and len(raw) == 4:
        v = struct.unpack(">f", raw)[0]
        if math.isfinite(v):
            return f"{ctype} {name} = {v!r}f;"
        return None
    if ctype == "f64" and len(raw) == 8:
        v = struct.unpack(">d", raw)[0]
        if math.isfinite(v):
            return f"{ctype} {name} = {v!r};"
        return None
    return None


def emit(sym: DataSymbol, raw: bytes | None) -> str:
    """One definition. `raw` is the retail bytes when the DOL was available
    and the symbol is file-backed (initialized data); None for bss/zero-fill."""
    ctype = _KIND_TYPES.get(sym.kind, "u8")
    unit = _KIND_UNIT.get(sym.kind, 1)
    size = sym.size
    align = f"alignas({sym.align}) " if sym.align >= 8 else ""
    note = f"  // {sym.section}:0x{sym.address} size:0x{sym.size:X} scope:{sym.scope}"
    if sym.align > 1:
        note += f" align:{sym.align}"
    if sym.kind:
        note += f" data:{sym.kind}"

    if raw is not None:
        note += f" retail:BE {_hex(raw)}" if len(raw) <= 16 else ""
        # Typed scalar when the kind/width line up and the value is a plain
        # literal (bytes are big-endian retail order — decode for the host).
        if ctype != "char" and size % unit == 0 and size // unit == 1:
            decl = _scalar_decl(ctype, sym.name, raw)
            if decl is not None:
                return f"{align}{decl}{note}"
        # Everything else: raw byte array in retail order (multi-byte values
        # inside remain big-endian; embedded pointers still hold retail
        # addresses — the port must interpret/relocate those).
        return f"{align}{_byte_array_decl(sym.name, raw)}{note}"

    # bss / zero-fill / no DOL: typed zero definitions.
    count = size // unit if size % unit == 0 else 0
    if count > 1:
        decl = f"{ctype} {sym.name}[{count}] = {{0}};"
    elif count == 1 and ctype != "char":
        if ctype == "f32":
            decl = f"{ctype} {sym.name} = 0.0f;"
        elif ctype == "f64":
            decl = f"{ctype} {sym.name} = 0.0;"
        else:
            decl = f"{ctype} {sym.name} = 0;"
    else:
        decl = f"{ctype} {sym.name}[{size}] = {{0}};"
    return f"{align}{decl}{note}"


def generate(rows: list[DataSymbol], region: str, symbols_path: Path,
             only_referenced: bool, exclude: Path, dol: DolImage | None,
             manifest: dict | None = None):
    """manifest: lbls_manifest.json content; its addresses are defined by the
    dual-mode area headers (LBLS_DEFINE_DATA + includes), not this TU."""
    kept, skipped = select(rows)
    if only_referenced:
        wanted = references(kept, [_REPO / "src", _REPO / "libs"], exclude)
        before = len(kept)
        kept = [r for r in kept if r.name in wanted]
        skipped["not referenced by src/libs"] = \
            skipped.get("not referenced by src/libs", 0) + before - len(kept)
    if manifest:
        covered = {a for entries in manifest.values()
                   for a, _t, _arr in entries}
        before = len(kept)
        kept = [r for r in kept if r.name not in covered]
        skipped["covered by lbl area headers (lbls_manifest.json)"] = \
            skipped.get("covered by lbl area headers (lbls_manifest.json)", 0) + before - len(kept)

    by_section: dict[str, list[DataSymbol]] = defaultdict(list)
    for r in kept:
        by_section[r.section].append(r)

    def retail_bytes(r: DataSymbol) -> bytes | None:
        if dol is None or r.section in _BSS_SECTIONS:
            return None
        return dol.read(int(r.address, 16), r.size)

    lines: list[str] = []
    lines.append("// GENERATED FILE — do not edit. Regenerate with:")
    lines.append(f"//   .venv/bin/python3 tools/port/gen_data_defs.py --region {region}"
                 + (" --only-referenced" if only_referenced else ""))
    lines.append(f"// Source: {symbols_path.relative_to(_REPO)}")
    lines.append("//")
    lines.append("// PC-port data definitions for the Xenoblade co-op fork.")
    lines.append("// The MWCC matching build must NOT define this data (decompiled TUs")
    lines.append("// reference the retail symbols via extern declarations; defining them")
    lines.append("// would add data sections retail does not have). So this TU compiles")
    lines.append("// only outside the MWCC matching build, like the PLAN.md §17.6")
    lines.append("// non-MWCC fallbacks:")
    if dol is not None:
        lines.append("//")
        lines.append("// Initialized data carries the RETAIL BYTES (big-endian order).")
        lines.append("// Multi-byte values inside byte arrays remain big-endian and")
        lines.append("// embedded pointers still hold retail addresses — the port must")
        lines.append("// interpret/relocate those. bss-family symbols are typed zeros.")
    lines.append("#if defined(__MWERKS__) && !defined(NONMATCHING)")
    lines.append("// Matching build: the retail image supplies all data — nothing defined.")
    lines.append("#else")
    lines.append("#include \"types.h\"")
    if manifest:
        lines.append("")
        lines.append("// Data owned by the dual-mode area lbl headers: define it once here")
        lines.append("// (LBLS_DEFINE_DATA makes the headers emit definitions instead of")
        lines.append("// extern; every other TU sees extern).")
        lines.append("#define LBLS_DEFINE_DATA")
        for hdr in sorted(manifest):
            lines.append(f"#include <{hdr}>")
        lines.append("#undef LBLS_DEFINE_DATA")
    lines.append("")
    lines.append("extern \"C\" {")
    lines.append(f"// {len(kept)} data symbol(s) defined here.")
    with_bytes = 0
    total_bytes = 0
    for section in sorted(by_section, key=lambda s: -len(by_section[s])):
        lines.append("")
        lines.append(f"// ---- {section} ({len(by_section[section])} symbols) ----")
        for r in sorted(by_section[section], key=lambda r: int(r.address, 16)):
            raw = retail_bytes(r)
            if raw is not None:
                with_bytes += 1
                total_bytes += len(raw)
            lines.append(emit(r, raw))
    lines.append("}")
    lines.append("#endif")
    return "\n".join(lines) + "\n", dict(skipped), len(kept), with_bytes, total_bytes


def _auto_dol(region: str) -> Path | None:
    p = _REPO / "orig" / region / "sys" / "main.dol"
    return p if p.is_file() else None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--region", default="us")
    ap.add_argument("--out", default=str(_REPO / "port" / "data_defs.cpp"))
    ap.add_argument("--only-referenced", action="store_true")
    ap.add_argument("--from-dol", metavar="PATH", default=None,
                    help="retail main.dol (default: orig/<region>/sys/main.dol if present)")
    ap.add_argument("--no-dol", action="store_true",
                    help="typed zeros only, even if a DOL is present")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    symbols_path = _REPO / "config" / args.region / "symbols.txt"
    if not symbols_path.is_file():
        print(f"error: {symbols_path} not found", file=sys.stderr)
        return 1
    out_path = Path(args.out)

    dol: DolImage | None = None
    if not args.no_dol:
        dol_path = Path(args.from_dol) if args.from_dol else _auto_dol(args.region)
        if dol_path and dol_path.is_file():
            dol = DolImage.load(dol_path)
        elif args.from_dol:
            print(f"error: {dol_path} not found", file=sys.stderr)
            return 1

    rows = parse_symbols(symbols_path)
    kept, skipped = select(rows)
    manifest = None
    manifest_path = _REPO / "tools" / "coop" / "lbls_manifest.json"
    if manifest_path.is_file():
        manifest = json.loads(manifest_path.read_text())
    text, skip2, emitted, with_bytes, total_bytes = generate(
        rows, args.region, symbols_path, args.only_referenced, out_path, dol,
        manifest,
    )

    total_refs = references(kept, [_REPO / "src", _REPO / "libs"], out_path)
    print(f"symbols.txt data rows (type:object):        {len(rows)}")
    print(f"nameable after filter:                     {len(kept)}")
    for why, n in sorted(skipped.items(), key=lambda kv: -kv[1]):
        print(f"  skipped ({why}):                          {n}")
    print(f"referenced by src/ + libs/:                {len(total_refs)}")
    print(f"definitions emitted:                       {emitted}")
    print(f"  with retail bytes from DOL:               {with_bytes} ({total_bytes} bytes)")
    print(f"  bss/zero-fill (typed zeros):             {emitted - with_bytes}")

    if args.dry_run:
        print(f"(dry-run — not writing {out_path})")
        return 0
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(text, encoding="utf-8")
    print(f"wrote {out_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

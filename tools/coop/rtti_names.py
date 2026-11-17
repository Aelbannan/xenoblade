#!/usr/bin/env python3
"""Recover ``__RTTI__`` symbol names from the retail objects.

MWCC emits one type_info record per polymorphic class: 8 bytes
``{ const char* name; const void* bases; }``.  The name string is the class
name as written in source, so the record's mangled symbol name can be rebuilt
from the retail data alone.  Most of these records are still anonymous
``lbl_<region>_XXXXXXXX`` symbols, which hides them from the linker weak-dedup
view (tools/coop/lib/weak_dedup.py): a unit that naturally emits a weak copy of
a base class's RTTI cannot be compared until the retail copy has its name.

    rtti_names.py              list recoverable names
    rtti_names.py --apply      rename them in config/<region>/symbols.txt

A label still spelled in source (a hand-built vtable that points at it) is
skipped and listed, so applying never breaks the link; such units get the
rename when their hand-built data is replaced.  Template classes are skipped:
their string is the demangled spelling and the argument mangling cannot be
rebuilt from it safely.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))

from elftools.elf.elffile import ELFFile  # noqa: E402

LABEL = re.compile(r"^lbl_(?:eu|us|jp)_[0-9A-Fa-f]{8}$")
IDENT = re.compile(r"^[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*$")
SOURCE_ROOTS = ("src", "include", "libs")
SOURCE_EXT = (".c", ".cpp", ".h", ".hpp", ".inl")


def mangle_class(name: str) -> str | None:
    """``ml::CVec3`` -> ``Q22ml5CVec3``; None when the name is not plain."""
    if not IDENT.match(name):
        return None
    parts = name.split("::")
    body = "".join(f"{len(p)}{p}" for p in parts)
    return body if len(parts) == 1 else f"Q{len(parts)}{body}"


def _cstring(data: bytes, off: int) -> str | None:
    end = data.find(b"\0", off)
    if end <= off:
        return None
    try:
        return data[off:end].decode("ascii")
    except UnicodeDecodeError:
        return None


def collect_strings(path: Path, out: dict[str, str]) -> None:
    """Record every labelled NUL-terminated string an object defines.

    The name string normally sits in the same unit as its type_info record, but
    where splits.txt gives the two sections to different units the record
    reaches its string through an undefined symbol.
    """
    with open(path, "rb") as fh:
        elf = ELFFile(fh)
        secs = list(elf.iter_sections())
        symtab = elf.get_section_by_name(".symtab")
        if symtab is None:
            return
        for sym in symtab.iter_symbols():
            shndx = sym["st_shndx"]
            if (isinstance(shndx, int) and LABEL.match(sym.name) and sym["st_size"]
                    and secs[shndx].name in (".rodata", ".data")
                    and secs[shndx]["sh_type"] == "SHT_PROGBITS"):
                raw = secs[shndx].data()[sym["st_value"]: sym["st_value"] + sym["st_size"]]
                body = raw.rstrip(b"\0")
                if body and len(body) < len(raw) and b"\0" not in body:
                    try:
                        out[sym.name] = body.decode("ascii")
                    except UnicodeDecodeError:
                        pass


def scan_object(path: Path, data_refs: set[str], strings: dict[str, str]):
    """Yield (label, class name) for anonymous RTTI records in one object.

    Every label referenced from a ``.data`` relocation is added to *data_refs*:
    a real type_info record is pointed at by a vtable or by a derived class's
    base list, which tells it apart from a ``const char*`` variable that
    happens to hold an identifier-like string.
    """
    with open(path, "rb") as fh:
        elf = ELFFile(fh)
        secs = list(elf.iter_sections())
        symtab = elf.get_section_by_name(".symtab")
        if symtab is None:
            return
        syms = list(symtab.iter_symbols())
        data = {i: s.data() for i, s in enumerate(secs) if s["sh_type"] == "SHT_PROGBITS"}
        relocs: dict[int, dict[int, tuple]] = {}
        for rs in secs:
            if rs["sh_type"] != "SHT_RELA":
                continue
            tab = relocs.setdefault(rs["sh_info"], {})
            from_data = secs[rs["sh_info"]].name == ".data"
            for r in rs.iter_relocations():
                tab[r["r_offset"]] = (syms[r["r_info_sym"]], r["r_addend"])
                if from_data and r["r_addend"] == 0:
                    data_refs.add(syms[r["r_info_sym"]].name)
        for sym in syms:
            shndx = sym["st_shndx"]
            if not isinstance(shndx, int) or sym["st_size"] != 8 or not LABEL.match(sym.name):
                continue
            if secs[shndx].name not in (".sdata", ".data"):
                continue
            ref = relocs.get(shndx, {}).get(sym["st_value"])
            if ref is None:
                continue
            tgt, addend = ref
            tsh = tgt["st_shndx"]
            # The second word is the base list (a reloc) or null; anything else
            # is not a type_info record.
            second = relocs.get(shndx, {}).get(sym["st_value"] + 4)
            raw = data[shndx][sym["st_value"] + 4: sym["st_value"] + 8]
            if second is None and raw != b"\0\0\0\0":
                continue
            if tsh == "SHN_UNDEF":
                if addend == 0 and tgt.name in strings:
                    yield sym.name, strings[tgt.name]
                continue
            if not isinstance(tsh, int) or tsh not in data or secs[tsh].name not in (".rodata", ".data"):
                continue
            off = tgt["st_value"] + (addend if tgt["st_info"]["type"] == "STT_SECTION" else 0)
            # A class-name string is its own symbol, not the middle of a table.
            if tgt["st_info"]["type"] != "STT_SECTION" and tgt["st_size"] and addend:
                continue
            text = _cstring(data[tsh], off)
            if text:
                yield sym.name, text


def source_labels() -> set[str]:
    used: set[str] = set()
    pat = re.compile(r"\blbl_(?:eu|us|jp)_[0-9A-Fa-f]{8}\b")
    for root in SOURCE_ROOTS:
        for p in (ROOT / root).rglob("*"):
            if p.suffix in SOURCE_EXT and p.is_file() and not p.name.endswith(".ctx.c"):
                used.update(pat.findall(p.read_text(errors="replace")))
    return used


def recover(region: str = "us"):
    """Return (renames, skipped): {label: new name}, [(label, class, reason)]."""
    existing = set()
    for line in (ROOT / "config" / region / "symbols.txt").read_text().splitlines():
        existing.add(line.split(" = ", 1)[0])
    found: dict[str, str] = {}
    data_refs: set[str] = set()
    targets = [ROOT / u["target_path"] for u in json.loads((ROOT / "objdiff.json").read_text())["units"]
               if u.get("target_path") and (ROOT / u["target_path"]).is_file()]
    strings: dict[str, str] = {}
    for path in targets:
        collect_strings(path, strings)
    for path in targets:
        for label, cls in scan_object(path, data_refs, strings):
            found[label] = cls
    in_source = source_labels()
    renames: dict[str, str] = {}
    skipped = []
    taken: dict[str, str] = {}
    for label, cls in sorted(found.items()):
        m = mangle_class(cls)
        if m is None:
            skipped.append((label, cls, "not a plain class name"))
            continue
        new = "__RTTI__" + m
        if label not in data_refs:
            skipped.append((label, cls, "no vtable or base list points at it"))
        elif new in existing or new in taken:
            skipped.append((label, cls, f"{new} already exists"))
        elif label in in_source:
            skipped.append((label, cls, "label still used in source"))
        else:
            renames[label] = new
            taken[new] = label
    return renames, skipped


def apply(region: str, renames: dict[str, str]) -> int:
    path = ROOT / "config" / region / "symbols.txt"
    out, n = [], 0
    for line in path.read_text().split("\n"):
        name, sep, rest = line.partition(" = ")
        if sep and name in renames:
            if "scope:" not in rest:
                rest = rest.rstrip() + " scope:weak"
            line = f"{renames[name]} = {rest}"
            n += 1
        out.append(line)
    path.write_text("\n".join(out))
    return n


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--region", default="us")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--verbose", action="store_true")
    a = ap.parse_args()
    renames, skipped = recover(a.region)
    if a.verbose:
        for label, new in sorted(renames.items()):
            print(f"  {label} -> {new}")
        for label, cls, why in skipped:
            print(f"  skip {label} {cls!r}: {why}")
    reasons: dict[str, int] = {}
    for _, _, why in skipped:
        key = "already exists" if why.endswith("already exists") else why
        reasons[key] = reasons.get(key, 0) + 1
    print(f"rtti-names: {len(renames)} recoverable; skipped: "
          + (", ".join(f"{k} {v}" for k, v in sorted(reasons.items())) or "none"))
    if a.apply:
        print(f"rtti-names: renamed {apply(a.region, renames)} symbol(s) in config/{a.region}/symbols.txt")
    return 0


if __name__ == "__main__":
    sys.exit(main())

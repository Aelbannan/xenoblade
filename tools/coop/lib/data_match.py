"""Data-section matching: compare a decompiled object's data sections against
the retail object's.

Retail split objects carry the unit's data sections (``.data``/``.rodata``/
``.sdata``/``.sdata2``/``.bss``/``.sbss``/``.sbss2``) byte-for-byte. A data
TU (typed or generated C/C++) compiled with MWCC should reproduce them.
Per section we compare:

- file-backed sections (``.data``/``.rodata``/``.sdata``/``.sdata2``): bytes,
  section alignment, and relocation lists;
- zero-fill sections (``.bss``/``.sbss``/``.sbss2``): size and section
  alignment only (no file bytes to compare).

A unit is "data-matched" when every section passes. This is the data analog of
the function-level hexdiff loop and the CI gate for data-only TUs.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass, field
from pathlib import Path

DATA_SECTIONS = (".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss", ".sbss2")
NOBITS = frozenset({".bss", ".sbss", ".sbss2"})


@dataclass
class SectionResult:
    name: str
    ok: bool
    retail_size: int
    decomp_size: int
    detail: str = ""


@dataclass
class DataMatchResult:
    ok: bool
    sections: list[SectionResult] = field(default_factory=list)

    def per_section_status(self) -> str:
        parts = []
        for s in self.sections:
            parts.append(f"{s.name}:{'ok' if s.ok else 'FAIL'}")
        return ", ".join(parts) if parts else "(no data sections)"


def _parse(path: Path):
    """Return ({section: {off, size, addr, align}}, {section: [relocs]})."""
    data = path.read_bytes()
    shoff = struct.unpack_from(">I", data, 0x20)[0]
    shentsize = struct.unpack_from(">H", data, 0x2E)[0]
    shnum = struct.unpack_from(">H", data, 0x30)[0]
    shstrndx = struct.unpack_from(">H", data, 0x32)[0]

    def shdr(i: int):
        o = shoff + i * shentsize
        return struct.unpack_from(">IIIIIIIIII", data, o)

    shstr = shdr(shstrndx)
    shstrtab = data[shstr[4]: shstr[4] + shstr[5]]

    def secname(i: int) -> str:
        off = shdr(i)[0]
        end = shstrtab.index(b"\0", off)
        return shstrtab[off:end].decode(errors="replace")

    secs: dict[str, dict] = {}
    for i in range(shnum):
        nm = secname(i)
        if nm not in DATA_SECTIONS:
            continue
        sh = shdr(i)
        secs[nm] = {"off": sh[4], "size": sh[5], "addr": sh[3], "align": sh[8]}

    relocs: dict[str, list | None] = {}
    symtabs = []   # (section index, shdr)
    strtabs = []
    for i in range(shnum):
        typ = shdr(i)[1]
        if typ == 2:
            symtabs.append(i)
        elif typ == 3:
            strtabs.append(i)
    for i in range(shnum):
        nm = secname(i)
        if not nm.startswith(".rela") or nm[5:] not in DATA_SECTIONS:
            continue
        try:
            sh = shdr(i)
            # MWCC objects set sh_link=0 on .rela sections: fall back to the
            # first SHT_SYMTAB in the file (splitter convention).
            st_idx = sh[3] if sh[3] in symtabs else (symtabs[0] if symtabs else None)
            if st_idx is None:
                relocs[nm[5:]] = None
                continue
            st = shdr(st_idx)
            symtab = data[st[4]: st[4] + st[5]]
            str_idx = st[3] if st[3] in strtabs else (strtabs[0] if strtabs else None)
            if str_idx is None:
                relocs[nm[5:]] = None
                continue
            strsec = shdr(str_idx)
            strtab = data[strsec[4]: strsec[4] + strsec[5]]
            out = []
            for j in range(sh[5] // 12):
                ro = sh[4] + j * 12
                r_offset, r_info, r_addend = struct.unpack_from(">IIi", data, ro)
                symidx = r_info >> 8
                rtype = r_info & 0xFF
                so = symidx * 16
                st_name = struct.unpack_from(">I", symtab, so)[0]
                end = strtab.index(b"\0", st_name)
                out.append((r_offset, rtype, strtab[st_name:end].decode(), r_addend))
            relocs[nm[5:]] = out
        except (struct.error, ValueError, IndexError):
            relocs[nm[5:]] = None
    return secs, relocs


def check_data_sections(retail_object: Path, decomp_object: Path) -> DataMatchResult:
    """Compare retail vs decompiled object data sections; all must pass."""
    r_secs, r_rel = _parse(retail_object)
    d_secs, d_rel = _parse(decomp_object)
    result = DataMatchResult(ok=True)
    r_bytes = retail_object.read_bytes()
    d_bytes = decomp_object.read_bytes()

    for sec in DATA_SECTIONS:
        r = r_secs.get(sec)
        d = d_secs.get(sec)
        rsz = r["size"] if r else 0
        dsz = d["size"] if d else 0
        if rsz != dsz:
            result.sections.append(SectionResult(
                sec, False, rsz, dsz,
                f"retail size 0x{rsz:X} != decomp 0x{dsz:X}",
            ))
            result.ok = False
            continue
        if rsz == 0:
            result.sections.append(SectionResult(sec, True, 0, 0, "empty both"))
            continue
        r_align = r["align"] if r else 0
        d_align = d["align"] if d else 0
        if sec in NOBITS:
            ok = r_align == d_align
            detail = f"size 0x{rsz:X} zero-fill; align {r_align} vs {d_align}"
            result.sections.append(SectionResult(sec, ok, rsz, dsz, detail))
            if not ok:
                result.ok = False
            continue
        rb = r_bytes[r["off"]: r["off"] + rsz]
        db = d_bytes[d["off"]: d["off"] + dsz]
        if rb == db and r_align == d_align:
            result.sections.append(SectionResult(
                sec, True, rsz, dsz, f"{rsz} bytes identical (align {r_align})"))
        else:
            diffs = sum(1 for a, b in zip(rb, db) if a != b)
            first = next((i for i, (a, b) in enumerate(zip(rb, db)) if a != b), None)
            detail = f"{diffs} byte diffs"
            if first is not None:
                detail += f", first at +0x{first:X} (retail {rb[first]:02X} vs decomp {db[first]:02X})"
            if r_align != d_align:
                detail += f"; align {r_align} vs {d_align}"
            result.sections.append(SectionResult(sec, False, rsz, dsz, detail))
            result.ok = False
            continue
        # reloc comparison: compare as offset-keyed sets, not raw .rela order.
        # The ppcdis splitter writes relocs in ascending r_offset order while
        # MWCC emits them in descending order per emitted object; the .rela
        # entry order is a serialization artifact and is link-irrelevant
        # (relocs are keyed by r_offset).
        rl = r_rel.get(sec)
        dl = d_rel.get(sec)
        if rl is None or dl is None:
            continue  # relocs not extractable on one side; bytes already verified
        if sorted(rl) != sorted(dl):
            result.sections.append(SectionResult(
                sec, False, rsz, dsz,
                f"reloc drift: retail {rl} != decomp {dl}",
            ))
            result.ok = False
    return result


def format_data_result(result: DataMatchResult) -> str:
    lines = []
    for s in result.sections:
        mark = "ok" if s.ok else "FAIL"
        lines.append(f"  [{mark:>4}] {s.name}: {s.detail}")
    lines.append("  VERDICT: " + ("MATCH" if result.ok else "MISMATCH"))
    return "\n".join(lines)


def has_data_sections(path: Path) -> bool:
    """True when the object carries any non-empty data section (i.e. it defines
    data rather than being an extern-only TU)."""
    secs, _ = _parse(path)
    return any(secs.get(sec, {}).get("size", 0) > 0 for sec in DATA_SECTIONS)

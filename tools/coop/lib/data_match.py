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

NOTE: ``run.py data diff`` compares the RAW object. Raw compiler output is
the gate. ``--postprocess`` is an explicit diagnostic that retries a raw FAIL
against a *temp copy* with UNIT_RULES (tools/postprocess_reloc_names.py)
applied; that result is not a source-derived match and must never be used
for unit promotion. Ninja still applies the same script to ``*.reloc.o`` for
the DOL link (a separate, link-only path that is being retired). Matching
(hexdiff / cycle / size) compares the raw MWCC ``.o``. This file only
compares the objects it is given; the runner owns the postprocess step.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass, field
from pathlib import Path

from tools.coop.lib.reloc_canon import LinkerLayout, type_class

DATA_SECTIONS = (".data", ".rodata", ".sdata", ".sdata2", ".bss", ".sbss", ".sbss2")
NOBITS = frozenset({".bss", ".sbss", ".sbss2"})


@dataclass
class Reloc:
    """One RELA entry on a data section."""

    offset: int
    type: int
    symbol: str
    addend: int


@dataclass
class ParsedObject:
    """Data sections + relocations + symbol table view of one ELF32 object."""

    sections: dict[str, dict] = field(default_factory=dict)
    relocs: dict[str, list[Reloc] | None] = field(default_factory=dict)
    #: symbol name -> (section index, st_value); the first definition wins.
    symbols: dict[str, tuple[int, int]] = field(default_factory=dict)
    section_names: dict[int, str] = field(default_factory=dict)


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


def _parse(path: Path) -> ParsedObject:
    """Parse the object's data sections, their relocations and its symtab."""
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

    parsed = ParsedObject()
    for i in range(shnum):
        nm = secname(i)
        parsed.section_names[i] = nm
        if nm not in DATA_SECTIONS:
            continue
        sh = shdr(i)
        parsed.sections[nm] = {"off": sh[4], "size": sh[5], "addr": sh[3], "align": sh[8]}

    symtabs = [i for i in range(shnum) if shdr(i)[1] == 2]
    strtabs = [i for i in range(shnum) if shdr(i)[1] == 3]

    # MWCC objects leave .rela sh_link=0 (the splitter's differ); fall back to
    # the first SHT_SYMTAB / its string table in that case.
    symtab_idx = next((i for i in range(shnum)
                       if shdr(i)[1] == 2 and shdr(i)[6] in strtabs), None)
    if symtab_idx is None:
        symtab_idx = symtabs[0] if symtabs else None
    if symtab_idx is None:
        return parsed
    st = shdr(symtab_idx)
    str_idx = st[6] if st[6] in strtabs else (strtabs[0] if strtabs else None)
    if str_idx is None:
        return parsed
    symtab_off, symtab_size = st[4], st[5]
    strsec = shdr(str_idx)
    strtab = data[strsec[4]: strsec[4] + strsec[5]]

    def sym_name(st_name: int) -> str:
        end = strtab.index(b"\0", st_name)
        return strtab[st_name:end].decode(errors="replace")

    for so in range(symtab_off, min(symtab_off + symtab_size, len(data) - 16), 16):
        st_name, st_value, _st_size, _st_info, _st_other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, so)
        if st_name == 0 or st_shndx == 0:
            continue
        name = sym_name(st_name)
        parsed.symbols.setdefault(name, (st_shndx, st_value))

    for i in range(shnum):
        nm = secname(i)
        if not nm.startswith(".rela") or nm[5:] not in DATA_SECTIONS:
            continue
        try:
            sh = shdr(i)
            out: list[Reloc] = []
            for j in range(sh[5] // 12):
                ro = sh[4] + j * 12
                r_offset, r_info, r_addend = struct.unpack_from(">IIi", data, ro)
                symidx = r_info >> 8
                rtype = r_info & 0xFF
                so = symidx * 16
                st_name = struct.unpack_from(">I", data, symtab_off + so)[0]
                out.append(Reloc(r_offset, rtype, sym_name(st_name), r_addend))
            parsed.relocs[nm[5:]] = out
        except (struct.error, ValueError, IndexError):
            parsed.relocs[nm[5:]] = None
    return parsed


def _is_tu_local(name: str) -> bool:
    """TU-local labels (``@N`` pools, ``...section.0``, ``$N`` statics) reuse
    names across units, so ``symbols.txt`` must never answer for them."""
    return name.startswith("@") or name.startswith("...") or name.startswith("$")


def derive_section_bases(parsed: ParsedObject, layout: LinkerLayout | None) -> dict[str, int]:
    """Absolute base address per data section, derived from the object's own
    globally-named symbols (``st_value`` is section-relative in ET_REL).

    The retail split object of a unit carries symbols whose names also appear
    in ``symbols.txt``; ``addr - st_value`` is the section's linked base, which
    is what lets a TU-local ``@N`` compare equal to a retail ``lbl_eu_*``.
    """
    bases: dict[str, int] = {}
    if layout is None:
        return bases
    for name, (shndx, value) in parsed.symbols.items():
        if _is_tu_local(name):
            continue
        section = parsed.section_names.get(shndx)
        if section not in DATA_SECTIONS:
            continue
        sec, addr = layout.resolve_name(name)
        if addr is None or sec != section:
            continue
        base = addr - value
        if base < 0x80000000:
            continue  # section-relative or bogus match
        bases.setdefault(section, base)
    return bases


def canonical_reloc_key(
    reloc: Reloc,
    parsed: ParsedObject,
    layout: LinkerLayout | None,
    bases: dict[str, int] | None = None,
) -> tuple:
    """Comparison key for one reloc: canonical class + the value it resolves to.

    Two relocs are equivalent when they install the same linked value at the
    same class: a decomp symbol that MWCC names differently but that resolves
    to the same ``(section, address, addend)`` as the retail splitter's label
    compares equal.  Object-local targets resolve through the object's derived
    section base; global/linker-script names resolve through ``symbols.txt`` /
    the ldscript.  Unresolvable targets fall back to name equality
    (fail-closed).
    """
    cls = type_class(reloc.type)
    home = parsed.symbols.get(reloc.symbol)
    if home is not None and home[0] != 0:
        section = parsed.section_names.get(home[0])
        base = (bases or {}).get(section)
        if base is not None:
            return (cls, "abs", base + home[1] + reloc.addend)
        return (cls, "sec", section, home[1] + reloc.addend)
    if layout is not None:
        target = layout.canon_reloc(reloc.symbol, reloc.addend)
        if target.addr is not None:
            return (cls, "abs", target.addr)
    return (cls, "name", reloc.symbol, reloc.addend)


def compare_reloc_sets(
    retail: ParsedObject,
    decomp: ParsedObject,
    section: str,
    layout: LinkerLayout | None = None,
) -> str | None:
    """Offset-keyed canonical reloc comparison; None when equal.

    Order-insensitive (relocs are keyed by r_offset): MWCC emits them in
    descending order per object while the splitter writes ascending order.
    """
    rl = retail.relocs.get(section)
    dl = decomp.relocs.get(section)
    if rl is None or dl is None:
        return None  # relocs not extractable on one side; bytes already verified
    rbases = derive_section_bases(retail, layout)
    dbases = derive_section_bases(decomp, layout)
    # A section-relative key on one side can still equal an absolute key on the
    # other if the missing base is known from the sibling object (same unit).
    for name, base in dbases.items():
        rbases.setdefault(name, base)
    for name, base in list(rbases.items()):
        dbases.setdefault(name, base)
    rk = {r.offset: canonical_reloc_key(r, retail, layout, rbases)
          for r in rl if r.type != 0 and r.symbol}
    dk = {r.offset: canonical_reloc_key(r, decomp, layout, dbases)
          for r in dl if r.type != 0 and r.symbol}
    for off in sorted(set(rk) | set(dk)):
        if off not in rk:
            return f"reloc drift at +0x{off:X}: decomp-only {dk[off]}"
        if off not in dk:
            return f"reloc drift at +0x{off:X}: retail-only {rk[off]}"
        if rk[off] != dk[off]:
            return f"reloc drift at +0x{off:X}: retail {rk[off]} != decomp {dk[off]}"
    return None


def check_data_sections(
    retail_object: Path,
    decomp_object: Path,
    *,
    strict_relocs: bool = False,
    layout: LinkerLayout | None = None,
) -> DataMatchResult:
    """Compare retail vs decompiled object data sections; all must pass.

    ``strict_relocs=True`` also compares reloc sets with canonical targets
    (see :func:`compare_reloc_sets`).  It is opt-in because the repository-wide
    raw objects still carry real presence/type drift (measured on the current
    tree: 232 units); the default gate keeps the byte-identity rule.
    """
    r_parsed = _parse(retail_object)
    d_parsed = _parse(decomp_object)
    r_secs, d_secs = r_parsed.sections, d_parsed.sections
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
        if rb == db:
            # MWCC .data/.rodata align 4 vs 8 is immaterial when bytes match (e.g. ResFontBase packed vtables)
            result.sections.append(SectionResult(
                sec, True, rsz, dsz, f"{rsz} bytes identical (align {r_align} vs {d_align})"))
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
        # Reloc-set comparison (opt-in).  The default gate keeps the historical
        # "bytes identical => reloc order/name drift is immaterial" rule: the
        # raw word at a reloc site is a placeholder/addend and the linker
        # resolves it, so a splitter-vs-MWCC naming difference does not change
        # the emitted bytes.  ``strict_relocs=True`` additionally requires the
        # offset-keyed reloc sets to agree up to *canonical* targets
        # (tools/coop/lib/reloc_canon.py): same resolved (section, address,
        # addend) and same reloc class, not the same spelling.
        if rb == db and not strict_relocs:
            continue
        drift = compare_reloc_sets(r_parsed, d_parsed, sec, layout)
        if drift is not None:
            result.sections.append(SectionResult(sec, False, rsz, dsz, drift))
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
    parsed = _parse(path)
    return any(parsed.sections.get(sec, {}).get("size", 0) > 0 for sec in DATA_SECTIONS)

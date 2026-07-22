"""Extract MWCC small-data bases and named symbol VAs from linked ELF images.

MWCC uses:
- ``r13`` / ``_SDA_BASE_`` for ``.sdata`` / ``.sbss``
- ``r2`` / ``_SDA2_BASE_`` for ``.sdata2`` / ``.sbss2``

Helpers are fail-closed: missing evidence returns empty maps rather than
inventing addresses.
"""

from __future__ import annotations

import struct
from pathlib import Path

from .elf_symbols import (
    ET_REL,
    SHN_ABS,
    SHN_COMMON,
    SHN_UNDEF,
    ElfSymbolError,
    _require_elf32_be,
    _section_table,
    _symbol_name,
    _symtab_context,
    list_allocatable_sections,
)

# EABI / MWCC canonical linker symbols.
_SDA_BASE_SYMBOL = "_SDA_BASE_"
_SDA2_BASE_SYMBOL = "_SDA2_BASE_"

# Section-end fallbacks when the linker symbols are absent (MWCC convention:
# SDA base sits at the end of the small-data region).
_SDA_SECTIONS = (".sdata", ".sbss")
_SDA2_SECTIONS = (".sdata2", ".sbss2")


def extract_symbol_addresses(
    path: Path | str,
    *,
    names: frozenset[str] | set[str] | None = None,
) -> dict[str, int]:
    """Return ``{symbol: linked VA}`` for defined non-local symbols.

    When ``names`` is provided, only those symbols are returned. Relocatable
    objects (``ET_REL``) yield an empty map — callers need a linked image for
    absolute VAs.
    """
    obj = Path(path)
    try:
        data = obj.read_bytes()
        e_type = _require_elf32_be(data, obj)
    except (OSError, ElfSymbolError):
        return {}
    if e_type == ET_REL:
        return {}
    try:
        sections, by_name = _section_table(data)
        symtab, _sym_idx, str_off, str_size, entsize = _symtab_context(
            data, obj, sections, by_name,
        )
    except ElfSymbolError:
        return {}

    wanted = set(names) if names is not None else None
    results: dict[str, int] = {}
    for index in range(int(symtab["size"]) // entsize):
        ent = int(symtab["offset"]) + index * entsize
        if ent + 16 > len(data):
            break
        st_name, st_value, _st_size, st_info, _st_other, st_shndx = struct.unpack_from(
            ">IIIBBH", data, ent,
        )
        del st_info  # include locals and globals; callers filter by name
        if st_shndx in (SHN_UNDEF, SHN_COMMON):
            continue
        name = _symbol_name(data, str_off, str_size, st_name)
        if not name:
            continue
        if wanted is not None and name not in wanted:
            continue
        if st_shndx == SHN_ABS or st_shndx < len(sections):
            results[name] = int(st_value) & 0xFFFFFFFF
    return results


def _section_end_base(path: Path, section_names: tuple[str, ...]) -> int | None:
    try:
        sections = {
            item.name: item
            for item in list_allocatable_sections(path)
            if item.name in section_names
        }
    except (OSError, ElfSymbolError):
        return None
    if not sections:
        return None
    end = 0
    for name in section_names:
        item = sections.get(name)
        if item is None:
            continue
        end = max(end, (int(item.addr) + int(item.size)) & 0xFFFFFFFF)
    return end if end else None


def extract_sda_bases(path: Path | str) -> dict[int, int]:
    """Return ``{2: SDA2, 13: SDA}`` when resolvable from a linked ELF.

    Prefer ``_SDA_BASE_`` / ``_SDA2_BASE_`` symbols; fall back to section-end
    addresses for the corresponding small-data sections.
    """
    obj = Path(path)
    symbols = extract_symbol_addresses(
        obj,
        names={_SDA_BASE_SYMBOL, _SDA2_BASE_SYMBOL},
    )
    bases: dict[int, int] = {}
    sda = symbols.get(_SDA_BASE_SYMBOL)
    sda2 = symbols.get(_SDA2_BASE_SYMBOL)
    if sda is None:
        sda = _section_end_base(obj, _SDA_SECTIONS)
    if sda2 is None:
        sda2 = _section_end_base(obj, _SDA2_SECTIONS)
    if sda is not None:
        bases[13] = int(sda) & 0xFFFFFFFF
    if sda2 is not None:
        bases[2] = int(sda2) & 0xFFFFFFFF
    return bases


def collect_reloc_symbol_names(*reloc_groups: tuple) -> set[str]:
    """Collect canonical / raw symbol names from decode relocation tuples."""
    names: set[str] = set()
    for group in reloc_groups:
        for item in group or ():
            for attr in ("canonical_symbol", "symbol", "name"):
                value = getattr(item, attr, None)
                if isinstance(value, str) and value:
                    names.add(value)
                    break
            if isinstance(item, (tuple, list)) and len(item) >= 3:
                symbol = item[2]
                if isinstance(symbol, str) and symbol:
                    names.add(symbol)
    return names

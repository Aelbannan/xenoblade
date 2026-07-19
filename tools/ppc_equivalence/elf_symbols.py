"""Extract named .text function bytes from big-endian PowerPC ELF32 objects."""

from __future__ import annotations

import struct
from dataclasses import dataclass
from pathlib import Path


STT_NOTYPE = 0
STT_OBJECT = 1
STT_FUNC = 2
ET_REL = 1
ET_EXEC = 2
ET_DYN = 3
EM_PPC = 20
SHT_PROGBITS = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_NOBITS = 8
SHT_REL = 9
SHN_UNDEF = 0
SHN_ABS = 0xFFF1
SHN_COMMON = 0xFFF2
SHF_WRITE = 0x1
SHF_ALLOC = 0x2
SHF_EXECINSTR = 0x4


@dataclass(frozen=True)
class FunctionRelocation:
    """One unresolved relocation whose write offset lies inside a function."""

    offset: int
    relocation_type: int
    symbol: str
    addend: int | None


@dataclass(frozen=True)
class FunctionBytes:
    """A contiguous instruction span taken from a relocatable (or linked) object."""

    name: str
    path: Path
    code: bytes
    """Raw big-endian instruction bytes; length is a multiple of four."""

    base: int
    """Decode base address: section address + ET_REL offset, or linked st_value."""

    value: int
    """Symbol st_value (offset within its section for ET_REL)."""

    size: int
    section_index: int
    section_name: str
    symbol_type: int
    relocations: tuple[FunctionRelocation, ...] = ()


@dataclass(frozen=True)
class SectionRelocation:
    """One relocation that patches an allocatable (often data) section."""

    offset: int
    """Byte offset within the target section (not a linked VA)."""

    relocation_type: int
    symbol: str
    addend: int | None
    target_section: str
    relocation_section: str


@dataclass(frozen=True)
class AllocatableSection:
    """One SHF_ALLOC PROGBITS/NOBITS section with optional attached relocations."""

    name: str
    path: Path
    index: int
    addr: int
    size: int
    flags: int
    sh_type: int
    data: bytes
    """Raw section bytes; empty for SHT_NOBITS."""

    relocations: tuple[SectionRelocation, ...] = ()

    @property
    def writable(self) -> bool:
        return bool(self.flags & SHF_WRITE)

    @property
    def executable(self) -> bool:
        return bool(self.flags & SHF_EXECINSTR)

    @property
    def is_nobits(self) -> bool:
        return self.sh_type == SHT_NOBITS


class ElfSymbolError(ValueError):
    """Invalid ELF or missing/ambiguous function symbol."""


class RelocationsPresent(ElfSymbolError):
    """``require_relocation_free`` rejected a function carrying unresolved relocations.

    Distinct from a symbol lookup failure so callers can retry with linked bytes
    (see ``tools/ppc_equivalence.dol_symbols``) without masking genuine
    "symbol not found" / "not an ELF file" errors.
    """


def _require_elf32_be(data: bytes, path: Path) -> int:
    if len(data) < 52 or data[:4] != b"\x7fELF":
        raise ElfSymbolError(f"not an ELF file: {path}")
    if data[4] != 1:
        raise ElfSymbolError(f"expected ELF32: {path}")
    if data[5] != 2:
        raise ElfSymbolError(f"expected big-endian ELF: {path}")
    if data[6] != 1 or struct.unpack_from(">I", data, 0x14)[0] != 1:
        raise ElfSymbolError(f"unsupported ELF version: {path}")
    e_type, e_machine = struct.unpack_from(">HH", data, 0x10)
    if e_type not in (ET_REL, ET_EXEC, ET_DYN):
        raise ElfSymbolError(f"unsupported ELF type {e_type}: {path}")
    if e_machine != EM_PPC:
        raise ElfSymbolError(f"expected PowerPC ELF (machine {EM_PPC}), got {e_machine}: {path}")
    return e_type


def _section_table(data: bytes) -> tuple[list[dict[str, int | str]], dict[str, int]]:
    e_shoff = struct.unpack_from(">I", data, 0x20)[0]
    e_shentsize = struct.unpack_from(">H", data, 0x2E)[0]
    e_shnum = struct.unpack_from(">H", data, 0x30)[0]
    e_shstrndx = struct.unpack_from(">H", data, 0x32)[0]
    if e_shentsize < 40 or e_shnum == 0 or e_shstrndx >= e_shnum:
        raise ElfSymbolError("invalid ELF section header table")
    if e_shoff > len(data) or e_shnum > (len(data) - e_shoff) // e_shentsize:
        raise ElfSymbolError("ELF section header table out of range")

    def shdr(index: int) -> tuple[int, int, int, int, int, int, int, int, int, int]:
        off = e_shoff + index * e_shentsize
        if off + 40 > len(data):
            raise ElfSymbolError("section header out of range")
        return struct.unpack_from(">IIIIIIIIII", data, off)

    _, _, _, _, shstr_off, shstr_size, _, _, _, _ = shdr(e_shstrndx)
    if shstr_off + shstr_size > len(data):
        raise ElfSymbolError("section name string table out of range")
    shstr = data[shstr_off : shstr_off + shstr_size]

    sections: list[dict[str, int | str]] = []
    by_name: dict[str, int] = {}
    for index in range(e_shnum):
        name_idx, sh_type, flags, addr, sh_off, sh_size, link, info, align, entsize = shdr(index)
        if name_idx >= len(shstr):
            raise ElfSymbolError("section name index out of range")
        end = shstr.find(b"\x00", name_idx)
        if end < 0:
            raise ElfSymbolError("unterminated section name")
        name = shstr[name_idx:end].decode("ascii", errors="replace")
        entry = {
            "index": index,
            "name": name,
            "type": sh_type,
            "flags": flags,
            "addr": addr,
            "offset": sh_off,
            "size": sh_size,
            "link": link,
            "info": info,
            "align": align,
            "entsize": entsize,
        }
        if sh_type != SHT_NOBITS and (sh_off > len(data) or sh_size > len(data) - sh_off):
            raise ElfSymbolError(f"section {name!r} data out of range")
        sections.append(entry)
        # First wins for duplicate names (unusual; keep deterministic).
        by_name.setdefault(name, index)
    return sections, by_name


def _symbol_name(data: bytes, str_off: int, str_size: int, st_name: int) -> str:
    if st_name >= str_size:
        raise ElfSymbolError("symbol name offset outside string table")
    start = str_off + st_name
    limit = str_off + str_size
    if start >= limit or limit > len(data):
        raise ElfSymbolError("symbol name out of range")
    end = data.find(b"\x00", start, limit)
    if end < 0:
        raise ElfSymbolError("unterminated symbol name")
    return data[start:end].decode("ascii", errors="replace")


def _symtab_context(
    data: bytes,
    path: Path,
    sections: list[dict[str, int | str]],
    by_name: dict[str, int],
) -> tuple[dict[str, int | str], int, int, int, int]:
    sym_idx = by_name.get(".symtab")
    if sym_idx is None:
        raise ElfSymbolError(f"missing .symtab: {path}")
    symtab = sections[sym_idx]
    str_idx = int(symtab["link"])
    if str_idx >= len(sections):
        raise ElfSymbolError(f"symtab link out of range: {path}")
    strtab = sections[str_idx]
    str_off = int(strtab["offset"])
    str_size = int(strtab["size"])
    entsize = int(symtab["entsize"]) or 16
    if entsize < 16:
        raise ElfSymbolError(f"unsupported symtab entsize {entsize}: {path}")
    return symtab, sym_idx, str_off, str_size, entsize


def _iter_section_relocations(
    data: bytes,
    path: Path,
    *,
    e_type: int,
    sections: list[dict[str, int | str]],
    symtab: dict[str, int | str],
    sym_idx: int,
    str_off: int,
    str_size: int,
    entsize: int,
    target_section_index: int | None = None,
) -> list[SectionRelocation]:
    """Parse REL/RELA entries targeting allocatable (or selected) sections."""
    results: list[SectionRelocation] = []
    for relocation_section in sections:
        section_type = int(relocation_section["type"])
        if section_type not in (SHT_REL, SHT_RELA):
            continue
        target_idx = int(relocation_section["info"])
        if target_section_index is not None and target_idx != target_section_index:
            continue
        if target_idx < 0 or target_idx >= len(sections):
            raise ElfSymbolError(
                f"{relocation_section['name']} target section index out of range: {path}"
            )
        target = sections[target_idx]
        if int(relocation_section["link"]) != sym_idx:
            raise ElfSymbolError(
                f"{relocation_section['name']} does not link to .symtab: {path}"
            )
        target_addr = int(target["addr"])
        target_size = int(target["size"])
        relocation_size = 12 if section_type == SHT_RELA else 8
        relocation_entsize = int(relocation_section["entsize"]) or relocation_size
        if relocation_entsize < relocation_size or int(relocation_section["size"]) % relocation_entsize:
            raise ElfSymbolError(
                f"invalid relocation entry size in {relocation_section['name']}: {path}"
            )
        for index in range(int(relocation_section["size"]) // relocation_entsize):
            entry_offset = int(relocation_section["offset"]) + index * relocation_entsize
            r_offset, r_info = struct.unpack_from(">II", data, entry_offset)
            addend = (
                struct.unpack_from(">i", data, entry_offset + 8)[0]
                if section_type == SHT_RELA
                else None
            )
            symbol_index = r_info >> 8
            symbol_entry = int(symtab["offset"]) + symbol_index * entsize
            if symbol_index >= int(symtab["size"]) // entsize or symbol_entry + 16 > len(data):
                raise ElfSymbolError(
                    f"relocation symbol index out of range in {relocation_section['name']}: {path}"
                )
            symbol_name_offset = struct.unpack_from(">I", data, symbol_entry)[0]
            symbol_name = _symbol_name(data, str_off, str_size, symbol_name_offset)
            section_offset = r_offset if e_type == ET_REL else r_offset - target_addr
            if target_size == 0:
                continue
            if not (0 <= section_offset < target_size):
                if e_type == ET_REL:
                    raise ElfSymbolError(
                        f"relocation offset 0x{r_offset:x} outside "
                        f"{target['name']} in {relocation_section['name']}: {path}"
                    )
                continue
            results.append(
                SectionRelocation(
                    offset=section_offset,
                    relocation_type=r_info & 0xFF,
                    symbol=symbol_name,
                    addend=addend,
                    target_section=str(target["name"]),
                    relocation_section=str(relocation_section["name"]),
                )
            )
    return results


def list_allocatable_sections(path: Path | str) -> list[AllocatableSection]:
    """Return every SHF_ALLOC PROGBITS/NOBITS section with attached relocations.

    Includes ``.text``, ``.rodata``, ``.data``, and related allocatable sections.
    Jump-table work should filter to non-executable read-only images as needed.
    """
    obj = Path(path)
    data = obj.read_bytes()
    e_type = _require_elf32_be(data, obj)
    sections, by_name = _section_table(data)
    symtab, sym_idx, str_off, str_size, entsize = _symtab_context(
        data, obj, sections, by_name,
    )
    all_relocs = _iter_section_relocations(
        data,
        obj,
        e_type=e_type,
        sections=sections,
        symtab=symtab,
        sym_idx=sym_idx,
        str_off=str_off,
        str_size=str_size,
        entsize=entsize,
    )
    by_target: dict[str, list[SectionRelocation]] = {}
    for reloc in all_relocs:
        by_target.setdefault(reloc.target_section, []).append(reloc)

    results: list[AllocatableSection] = []
    for section in sections:
        flags = int(section["flags"])
        sh_type = int(section["type"])
        if not (flags & SHF_ALLOC):
            continue
        if sh_type not in (SHT_PROGBITS, SHT_NOBITS):
            continue
        name = str(section["name"])
        if not name:
            continue
        size = int(section["size"])
        if sh_type == SHT_NOBITS:
            payload = b""
        else:
            off = int(section["offset"])
            payload = data[off : off + size]
        results.append(
            AllocatableSection(
                name=name,
                path=obj,
                index=int(section["index"]),
                addr=int(section["addr"]),
                size=size,
                flags=flags,
                sh_type=sh_type,
                data=payload,
                relocations=tuple(by_target.get(name, ())),
            )
        )
    return results


def extract_allocatable_section(path: Path | str, section_name: str) -> AllocatableSection:
    """Load one named allocatable section (e.g. ``.rodata``) from an ELF object."""
    if not section_name:
        raise ElfSymbolError("section name must be non-empty")
    matches = [item for item in list_allocatable_sections(path) if item.name == section_name]
    if not matches:
        raise ElfSymbolError(f"allocatable section {section_name!r} not found in {path}")
    if len(matches) > 1:
        raise ElfSymbolError(f"duplicate allocatable section {section_name!r} in {path}")
    return matches[0]


def list_text_functions(path: Path | str) -> list[FunctionBytes]:
    """Return every sized .text symbol (FUNC or NOTYPE) from an ELF32 BE object."""
    obj = Path(path)
    data = obj.read_bytes()
    e_type = _require_elf32_be(data, obj)
    sections, by_name = _section_table(data)

    text_idx = by_name.get(".text")
    if text_idx is None:
        raise ElfSymbolError(f"missing .text section: {obj}")
    text = sections[text_idx]
    symtab, sym_idx, str_off, str_size, entsize = _symtab_context(
        data, obj, sections, by_name,
    )

    text_off = int(text["offset"])
    text_size = int(text["size"])
    text_addr = int(text["addr"])
    text_section_relocs = _iter_section_relocations(
        data,
        obj,
        e_type=e_type,
        sections=sections,
        symtab=symtab,
        sym_idx=sym_idx,
        str_off=str_off,
        str_size=str_size,
        entsize=entsize,
        target_section_index=text_idx,
    )
    text_relocations = [
        (item.offset, item.relocation_type, item.symbol, item.addend)
        for item in text_section_relocs
    ]
    results: list[FunctionBytes] = []

    for index in range(int(symtab["size"]) // entsize):
        ent = int(symtab["offset"]) + index * entsize
        if ent + 16 > len(data):
            raise ElfSymbolError("symbol entry out of range")
        st_name, st_value, st_size, st_info, _st_other, st_shndx = struct.unpack_from(">IIIBBH", data, ent)
        if st_shndx != text_idx or st_size == 0:
            continue
        if st_shndx in (SHN_UNDEF, SHN_ABS, SHN_COMMON):
            continue
        symbol_type = st_info & 0xF
        if symbol_type not in (STT_FUNC, STT_NOTYPE):
            continue
        symbol_offset = st_value if e_type == ET_REL else st_value - text_addr
        symbol_base = (text_addr + st_value) & 0xFFFFFFFF if e_type == ET_REL else st_value & 0xFFFFFFFF
        if symbol_offset < 0 or symbol_offset + st_size > text_size:
            raise ElfSymbolError(
                f"symbol spans past .text ({_symbol_name(data, str_off, str_size, st_name)!r} "
                f"value=0x{st_value:x} size=0x{st_size:x} text=0x{text_size:x}): {obj}"
            )
        if st_size % 4:
            raise ElfSymbolError(
                f"symbol size is not a multiple of 4 ({_symbol_name(data, str_off, str_size, st_name)!r}): {obj}"
            )
        name = _symbol_name(data, str_off, str_size, st_name)
        if not name:
            continue
        code = data[text_off + symbol_offset : text_off + symbol_offset + st_size]
        relocations = tuple(
            FunctionRelocation(offset - symbol_offset, relocation_type, target, addend)
            for offset, relocation_type, target, addend in text_relocations
            if symbol_offset <= offset < symbol_offset + st_size
        )
        results.append(
            FunctionBytes(
                name=name,
                path=obj,
                code=code,
                base=symbol_base,
                value=st_value,
                size=st_size,
                section_index=text_idx,
                section_name=str(text["name"]),
                symbol_type=symbol_type,
                relocations=relocations,
            )
        )
    return results


def _resolve_candidates(functions: list[FunctionBytes], symbol: str) -> list[FunctionBytes]:
    exact = [item for item in functions if item.name == symbol]
    if exact:
        return exact
    # Case-insensitive exact (objdiff demangled lookups are usually already mangled).
    lowered = symbol.lower()
    ci = [item for item in functions if item.name.lower() == lowered]
    if ci:
        return ci
    # Substring fallback for mangled stubs when the caller passes a short unique token.
    partial = [item for item in functions if lowered in item.name.lower()]
    return partial


def extract_function(path: Path | str, symbol: str) -> FunctionBytes:
    """Load one named function's instruction bytes from an ELF32 BE object."""
    obj = Path(path)
    if not symbol:
        raise ElfSymbolError("symbol name must be non-empty")
    functions = list_text_functions(obj)
    matches = _resolve_candidates(functions, symbol)
    if not matches:
        raise ElfSymbolError(f"symbol {symbol!r} not found in {obj}")
    if len(matches) > 1:
        names = ", ".join(item.name for item in matches[:8])
        raise ElfSymbolError(f"ambiguous symbol {symbol!r} in {obj}; matches: {names}")
    return matches[0]


def require_relocation_free(*functions: FunctionBytes) -> None:
    """Reject unresolved object code rather than proving placeholder immediates."""
    affected = [item for item in functions if item.relocations]
    if not affected:
        return
    details = []
    for function in affected:
        sample = ", ".join(
            f"+0x{reloc.offset:x}:type={reloc.relocation_type}:symbol={reloc.symbol or '<section>'}"
            for reloc in function.relocations[:4]
        )
        if len(function.relocations) > 4:
            sample += f", ... ({len(function.relocations)} total)"
        details.append(f"{function.path}:{function.name} [{sample}]")
    raise RelocationsPresent(
        "function contains unresolved ELF relocations; linked relocation values must be "
        "supplied before a sound semantic proof: " + "; ".join(details)
    )


def extract_function_pair(
    original: Path | str,
    candidate: Path | str,
    symbol: str,
    *,
    candidate_symbol: str | None = None,
) -> tuple[FunctionBytes, FunctionBytes]:
    """Extract the same logical symbol from a retail/decomp object pair."""
    left = extract_function(original, symbol)
    right = extract_function(candidate, candidate_symbol or symbol)
    return left, right


def rom_image_from_allocatable_section(
    section: AllocatableSection,
    *,
    base: int | None = None,
    label: str | None = None,
):
    """Build an AddressSpace ROM_IMAGE region from a non-empty PROGBITS section.

    ``base`` defaults to the section's ``addr`` (0 for typical ET_REL objects);
    callers proving against linked images should pass the linked VA.
    """
    from tools.ppc_equivalence.address_space import rom_image_region

    if section.is_nobits or not section.data:
        raise ElfSymbolError(
            f"section {section.name!r} has no image bytes for a ROM binding"
        )
    if section.executable:
        raise ElfSymbolError(
            f"refusing ROM_IMAGE binding for executable section {section.name!r}"
        )
    return rom_image_region(
        base if base is not None else section.addr,
        section.data,
        label=label if label is not None else section.name,
    )

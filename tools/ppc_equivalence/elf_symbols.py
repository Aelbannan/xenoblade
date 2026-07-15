"""Extract named .text function bytes from big-endian PowerPC ELF32 objects."""

from __future__ import annotations

import struct
from dataclasses import dataclass
from pathlib import Path


STT_NOTYPE = 0
STT_FUNC = 2
SHN_UNDEF = 0
SHN_ABS = 0xFFF1
SHN_COMMON = 0xFFF2


@dataclass(frozen=True)
class FunctionBytes:
    """A contiguous instruction span taken from a relocatable (or linked) object."""

    name: str
    path: Path
    code: bytes
    """Raw big-endian instruction bytes; length is a multiple of four."""

    base: int
    """Decode base address: section virtual address + symbol value."""

    value: int
    """Symbol st_value (offset within its section for ET_REL)."""

    size: int
    section_index: int
    section_name: str
    symbol_type: int


class ElfSymbolError(ValueError):
    """Invalid ELF or missing/ambiguous function symbol."""


def _require_elf32_be(data: bytes, path: Path) -> None:
    if len(data) < 52 or data[:4] != b"\x7fELF":
        raise ElfSymbolError(f"not an ELF file: {path}")
    if data[4] != 1:
        raise ElfSymbolError(f"expected ELF32: {path}")
    if data[5] != 2:
        raise ElfSymbolError(f"expected big-endian ELF: {path}")


def _section_table(data: bytes) -> tuple[list[dict[str, int | str]], dict[str, int]]:
    e_shoff = struct.unpack_from(">I", data, 0x20)[0]
    e_shentsize = struct.unpack_from(">H", data, 0x2E)[0]
    e_shnum = struct.unpack_from(">H", data, 0x30)[0]
    e_shstrndx = struct.unpack_from(">H", data, 0x32)[0]
    if e_shentsize < 40 or e_shnum == 0 or e_shstrndx >= e_shnum:
        raise ElfSymbolError("invalid ELF section header table")

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
        sections.append(entry)
        # First wins for duplicate names (unusual; keep deterministic).
        by_name.setdefault(name, index)
    return sections, by_name


def _symbol_name(data: bytes, str_off: int, st_name: int) -> str:
    start = str_off + st_name
    if start >= len(data):
        raise ElfSymbolError("symbol name out of range")
    end = data.find(b"\x00", start)
    if end < 0:
        raise ElfSymbolError("unterminated symbol name")
    return data[start:end].decode("ascii", errors="replace")


def list_text_functions(path: Path | str) -> list[FunctionBytes]:
    """Return every sized .text symbol (FUNC or NOTYPE) from an ELF32 BE object."""
    obj = Path(path)
    data = obj.read_bytes()
    _require_elf32_be(data, obj)
    sections, by_name = _section_table(data)

    text_idx = by_name.get(".text")
    if text_idx is None:
        raise ElfSymbolError(f"missing .text section: {obj}")
    text = sections[text_idx]

    sym_idx = by_name.get(".symtab")
    if sym_idx is None:
        raise ElfSymbolError(f"missing .symtab: {obj}")
    symtab = sections[sym_idx]
    str_idx = int(symtab["link"])
    if str_idx >= len(sections):
        raise ElfSymbolError(f"symtab link out of range: {obj}")
    strtab = sections[str_idx]
    str_off = int(strtab["offset"])
    entsize = int(symtab["entsize"]) or 16
    if entsize < 16:
        raise ElfSymbolError(f"unsupported symtab entsize {entsize}: {obj}")

    text_off = int(text["offset"])
    text_size = int(text["size"])
    text_addr = int(text["addr"])
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
        if st_value + st_size > text_size:
            raise ElfSymbolError(
                f"symbol spans past .text ({_symbol_name(data, str_off, st_name)!r} "
                f"value=0x{st_value:x} size=0x{st_size:x} text=0x{text_size:x}): {obj}"
            )
        if st_size % 4:
            raise ElfSymbolError(
                f"symbol size is not a multiple of 4 ({_symbol_name(data, str_off, st_name)!r}): {obj}"
            )
        name = _symbol_name(data, str_off, st_name)
        if not name:
            continue
        code = data[text_off + st_value : text_off + st_value + st_size]
        results.append(
            FunctionBytes(
                name=name,
                path=obj,
                code=code,
                base=(text_addr + st_value) & 0xFFFFFFFF,
                value=st_value,
                size=st_size,
                section_index=text_idx,
                section_name=str(text["name"]),
                symbol_type=symbol_type,
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

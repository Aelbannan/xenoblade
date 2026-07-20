"""Hydrate readonly u32 words for memory-loop CTR ``lwz`` materialization.

Reuses the same linked DOL / ELF slice rules as ``jump_table_image`` (non-executable
sections only). Fail closed when the image is missing or the address is not covered.
"""

from __future__ import annotations

import struct
from collections.abc import Iterable, Mapping, Sequence
from pathlib import Path

from tools.ppc_equivalence.dol_symbols import DolSymbolError, extract_data_by_address
from tools.ppc_equivalence.elf_symbols import (
    ET_DYN,
    ET_EXEC,
    ET_REL,
    ElfSymbolError,
    elf_type,
    list_allocatable_sections,
)
from tools.ppc_equivalence.ir import Instruction
from tools.ppc_equivalence.memory_loop import collect_memory_loop_ctr_lwz_addresses

_WORD_SIZE = 4


class MemoryLoopImageError(ValueError):
    """Missing or invalid readonly image bytes for CTR ``lwz`` hydration."""


def hydrate_readonly_words(
    addresses: Iterable[int],
    *,
    dol_path: Path | None = None,
    elf_path: Path | None = None,
) -> dict[int, int]:
    """Load big-endian u32 values at ``addresses`` from a linked image."""
    unique = sorted({int(address) & 0xFFFFFFFF for address in addresses})
    if not unique:
        return {}
    if dol_path is None and elf_path is None:
        raise MemoryLoopImageError(
            "no linked image supplied: provide elf_path (ET_EXEC/ET_DYN) or dol_path"
        )

    words: dict[int, int] = {}
    for address in unique:
        words[address] = _load_u32_word(address, dol_path=dol_path, elf_path=elf_path)
    return words


def try_build_memory_loop_readonly_words(
    instructions: Sequence[Instruction],
    *,
    dol_path: Path | None = None,
    elf_path: Path | None = None,
) -> dict[int, int] | None:
    """Hydrate CTR ``lwz`` words when every required VA is present in the image."""
    addresses = collect_memory_loop_ctr_lwz_addresses(instructions)
    if not addresses:
        return None
    try:
        return hydrate_readonly_words(
            addresses,
            dol_path=dol_path,
            elf_path=elf_path,
        )
    except MemoryLoopImageError:
        return None


def merge_memory_loop_readonly_words(
    *maps: Mapping[int, int] | None,
) -> dict[int, int] | None:
    """Merge side-specific readonly maps; return ``None`` when all inputs are empty."""
    merged: dict[int, int] = {}
    for mapping in maps:
        if not mapping:
            continue
        merged.update(
            {
                int(key) & 0xFFFFFFFF: int(value) & 0xFFFFFFFF
                for key, value in mapping.items()
            },
        )
    return merged or None


def _load_u32_word(
    address: int,
    *,
    dol_path: Path | None,
    elf_path: Path | None,
) -> int:
    if address & (_WORD_SIZE - 1):
        raise MemoryLoopImageError(f"readonly address 0x{address:08X} is not word-aligned")

    if elf_path is not None:
        try:
            payload = _slice_linked_elf_word(Path(elf_path), address=address)
            if payload is not None:
                (word,) = struct.unpack(">I", payload)
                return word & 0xFFFFFFFF
        except MemoryLoopImageError:
            raise
        except ElfSymbolError as exc:
            raise MemoryLoopImageError(str(exc)) from exc

    if dol_path is not None:
        try:
            slice_ = extract_data_by_address(dol_path, address, _WORD_SIZE)
        except DolSymbolError as exc:
            raise MemoryLoopImageError(str(exc)) from exc
        (word,) = struct.unpack(">I", slice_.code)
        return word & 0xFFFFFFFF

    raise MemoryLoopImageError(
        f"address 0x{address:08X} not available from supplied linked images"
    )


def _slice_linked_elf_word(path: Path, *, address: int) -> bytes | None:
    try:
        e_type = elf_type(path)
    except ElfSymbolError as exc:
        raise MemoryLoopImageError(str(exc)) from exc

    if e_type not in (ET_EXEC, ET_DYN, ET_REL):
        return None

    sections = list_allocatable_sections(path)
    for section in sections:
        section_end = section.addr + section.size
        if section.addr <= address and address + _WORD_SIZE <= section_end:
            if section.executable:
                raise MemoryLoopImageError(
                    f"refusing CTR lwz hydration from executable section "
                    f"{section.name!r} at 0x{address:x} in {path}"
                )
            if section.is_nobits or not section.data:
                raise MemoryLoopImageError(
                    f"section {section.name!r} has no image bytes at 0x{address:x} in {path}"
                )
            offset = address - section.addr
            payload = section.data[offset : offset + _WORD_SIZE]
            if len(payload) != _WORD_SIZE:
                raise MemoryLoopImageError(
                    f"partial readonly word at 0x{address:x} in {section.name!r}: {path}"
                )
            return payload
    raise MemoryLoopImageError(
        f"virtual address 0x{address:08X} not covered by any allocatable ELF section in {path}"
    )

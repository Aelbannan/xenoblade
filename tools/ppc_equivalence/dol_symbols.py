"""Extract linked .text and .data bytes from a GameCube/Wii DOL by virtual address.

The DOL executable format (broadway) is a fixed-position header describing up
to 7 text sections and 11 data sections. The 0x100-byte header layout used
here matches retail ``main.dol`` for every Xenoblade region:

- ``0x00`` — 7 ``u32`` text-section file offsets
- ``0x1C`` — 11 ``u32`` data-section file offsets
- ``0x48`` — 7 ``u32`` text-section virtual addresses
- ``0x6C`` — 11 ``u32`` data-section virtual addresses
- ``0x90`` — 7 ``u32`` text-section sizes
- ``0xB8`` — 11 ``u32`` data-section sizes
- ``0xE0`` — entry-point ``u32``

The SMT equivalence checker accepts raw big-endian instruction bytes plus a
decode base (see ``cli.cmd_check``). This module exists so ``prove_unit_symbol``
can fall back to linked DOL bytes when an unlinked ``.o`` pair carries
unresolved ``.rela.text`` entries (``require_relocation_free`` raises).

Only the bytes for one named function are returned, addressed by the
``(address, size)`` tuple stored in ``config/<region>/symbols.txt`` (parsed by
``tools/symbolrecover.lib.parser``).
"""

from __future__ import annotations

import struct
from dataclasses import dataclass
from pathlib import Path

# DOL header positions, in bytes, all big-endian u32.
_TEXT_OFFSETS = slice(0x00, 0x1C)
_DATA_OFFSETS = slice(0x1C, 0x48)
_TEXT_ADDRS = slice(0x48, 0x64)
_DATA_ADDRS = slice(0x6C, 0x98)
_TEXT_SIZES = slice(0x90, 0xAC)
_DATA_SIZES = slice(0xB8, 0xE4)
_HEADER_SIZE = 0x100


class DolSymbolError(ValueError):
    """Invalid DOL or symbol address outside any mapped section."""


@dataclass(frozen=True)
class DolSlice:
    """A contiguous instruction span taken from a linked DOL."""

    code: bytes
    """Raw big-endian instruction bytes; length is a multiple of four."""

    base: int
    """Virtual address of the first byte."""


@dataclass(frozen=True)
class DolSection:
    index: int
    file_offset: int
    address: int
    size: int

    @property
    def end(self) -> int:
        return self.address + self.size


DolTextSection = DolSection
DolDataSection = DolSection


def _read_dol(path: Path) -> bytes:
    data = path.read_bytes()
    if len(data) < _HEADER_SIZE:
        raise DolSymbolError(f"DOL too small ({len(data)} bytes): {path}")
    return data


def _list_sections(
    path: Path,
    *,
    offsets_start: int,
    addrs_start: int,
    sizes_start: int,
    slot_count: int,
    kind: str,
) -> list[DolSection]:
    data = _read_dol(path)
    fmt = f">{slot_count}I"
    offsets = struct.unpack_from(fmt, data, offsets_start)
    addrs = struct.unpack_from(fmt, data, addrs_start)
    sizes = struct.unpack_from(fmt, data, sizes_start)
    sections: list[DolSection] = []
    for index, (off, addr, size) in enumerate(zip(offsets, addrs, sizes)):
        if off == 0 and size == 0:
            continue
        if addr == 0:
            continue
        end = off + size
        if end > len(data):
            raise DolSymbolError(
                f"{kind} section {index} spans past end-of-file "
                f"(off=0x{off:x}, size=0x{size:x}): {path}"
            )
        sections.append(
            DolSection(index=index, file_offset=off, address=addr, size=size)
        )
    return sections


def text_sections(path: Path | str) -> list[DolTextSection]:
    """Return every non-empty text section declared in the DOL header."""
    return _list_sections(
        Path(path),
        offsets_start=_TEXT_OFFSETS.start,
        addrs_start=_TEXT_ADDRS.start,
        sizes_start=_TEXT_SIZES.start,
        slot_count=7,
        kind="text",
    )


def data_sections(path: Path | str) -> list[DolDataSection]:
    """Return every non-empty data section declared in the DOL header."""
    return _list_sections(
        Path(path),
        offsets_start=_DATA_OFFSETS.start,
        addrs_start=_DATA_ADDRS.start,
        sizes_start=_DATA_SIZES.start,
        slot_count=11,
        kind="data",
    )


def _extract_from_sections(
    path: Path,
    address: int,
    size: int,
    sections: list[DolSection],
    *,
    kind: str,
) -> DolSlice:
    if size <= 0:
        raise DolSymbolError(f"size must be positive, got 0x{size:x}")
    if size % 4:
        raise DolSymbolError(f"size must be a multiple of 4, got 0x{size:x}")
    data = _read_dol(path)
    for section in sections:
        if section.address <= address < section.end:
            available = section.end - address
            if size > available:
                raise DolSymbolError(
                    f"slice [0x{address:x}, +0x{size:x}) overruns {kind} section "
                    f"{section.index} [0x{section.address:x}, +0x{section.size:x}): {path}"
                )
            start = section.file_offset + (address - section.address)
            return DolSlice(code=data[start : start + size], base=address)
    raise DolSymbolError(
        f"address 0x{address:x} not in any {kind} section of {path}"
    )


def extract_by_address(path: Path | str, address: int, size: int) -> DolSlice:
    """Return ``size`` bytes of text starting at virtual ``address``."""
    dol = Path(path)
    return _extract_from_sections(
        dol,
        address,
        size,
        text_sections(dol),
        kind="text",
    )


def extract_data_by_address(path: Path | str, address: int, size: int) -> DolSlice:
    """Return ``size`` bytes of data starting at virtual ``address``."""
    dol = Path(path)
    return _extract_from_sections(
        dol,
        address,
        size,
        data_sections(dol),
        kind="data",
    )

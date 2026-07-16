"""Extract linked .text bytes from a GameCube/Wii DOL by virtual address.

The DOL executable format (broadway) is a fixed-position header describing up
to 7 text sections and 11 data sections. The 0x100-byte header layout used
here matches retail ``main.dol`` for every Xenoblade region:

- ``0x00`` — 7 ``u32`` text-section file offsets
- ``0x48`` — 7 ``u32`` text-section virtual addresses
- ``0x90`` — 7 ``u32`` text-section sizes
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
_TEXT_ADDRS = slice(0x48, 0x64)
_TEXT_SIZES = slice(0x90, 0xAC)
_HEADER_SIZE = 0x100


class DolSymbolError(ValueError):
    """Invalid DOL or symbol address outside any text section."""


@dataclass(frozen=True)
class DolSlice:
    """A contiguous instruction span taken from a linked DOL."""

    code: bytes
    """Raw big-endian instruction bytes; length is a multiple of four."""

    base: int
    """Virtual address of the first byte."""


@dataclass(frozen=True)
class DolTextSection:
    index: int
    file_offset: int
    address: int
    size: int

    @property
    def end(self) -> int:
        return self.address + self.size


def _read_dol(path: Path) -> bytes:
    data = path.read_bytes()
    if len(data) < _HEADER_SIZE:
        raise DolSymbolError(f"DOL too small ({len(data)} bytes): {path}")
    return data


def text_sections(path: Path | str) -> list[DolTextSection]:
    """Return every non-empty text section declared in the DOL header."""
    dol = Path(path)
    data = _read_dol(dol)
    offsets = struct.unpack_from(">7I", data, _TEXT_OFFSETS.start)
    addrs = struct.unpack_from(">7I", data, _TEXT_ADDRS.start)
    sizes = struct.unpack_from(">7I", data, _TEXT_SIZES.start)
    sections: list[DolTextSection] = []
    for index, (off, addr, size) in enumerate(zip(offsets, addrs, sizes)):
        if off == 0 and size == 0:
            continue
        if addr == 0:
            continue
        end = off + size
        if end > len(data):
            raise DolSymbolError(
                f"text section {index} spans past end-of-file "
                f"(off=0x{off:x}, size=0x{size:x}): {dol}"
            )
        sections.append(
            DolTextSection(index=index, file_offset=off, address=addr, size=size)
        )
    return sections


def extract_by_address(path: Path | str, address: int, size: int) -> DolSlice:
    """Return ``size`` bytes of text starting at virtual ``address``."""
    if size <= 0:
        raise DolSymbolError(f"size must be positive, got 0x{size:x}")
    if size % 4:
        raise DolSymbolError(f"size must be a multiple of 4, got 0x{size:x}")
    dol = Path(path)
    data = _read_dol(dol)
    for section in text_sections(dol):
        if section.address <= address < section.end:
            available = section.end - address
            if size > available:
                raise DolSymbolError(
                    f"slice [0x{address:x}, +0x{size:x}) overruns text section "
                    f"{section.index} [0x{section.address:x}, +0x{section.size:x}): {dol}"
                )
            start = section.file_offset + (address - section.address)
            return DolSlice(code=data[start : start + size], base=address)
    raise DolSymbolError(
        f"address 0x{address:x} not in any text section of {dol}"
    )

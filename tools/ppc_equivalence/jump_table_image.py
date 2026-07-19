"""Hydrate MWCC jump-table words from linked ELF or DOL images.

Per-TU ``.o`` files usually carry unresolved ``R_PPC_ADDR32`` slots in
``.rodata`` / ``.data`` (see ``docs/ppc_equiv_work/27-elf-data-reloc-census.md``).
Sound table closure therefore requires **linked** section bytes at the table VA,
not relocatable object placeholders.

Assumption: each table entry is a big-endian u32 code pointer; every loaded word
must be 4-byte aligned when interpreted as a branch target.
"""

from __future__ import annotations

import hashlib
import struct
from dataclasses import dataclass
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

_ADDR32_ENTRY_SIZE = 4


class JumpTableImageError(ValueError):
    """Missing, partial, or invalid jump-table image bytes."""


@dataclass(frozen=True)
class JumpTableImage:
    base: int
    """Linked VA of table[0]."""

    entry_count: int
    """Number of u32 entries (typically ``bound_imm + 1`` when ``cmplwi`` is present)."""

    entry_size: int
    """Byte width of one entry (4 for ``R_PPC_ADDR32`` tables)."""

    words: tuple[int, ...]
    """Big-endian u32 target addresses."""

    image_sha256: str
    source: str
    """``linked-elf`` | ``linked-dol`` | ``object-rodata``."""

    artifact_path: str


def hydrate_jump_table(
    *,
    base: int,
    entry_count: int,
    elf_path: Path | None = None,
    dol_path: Path | None = None,
    entry_size: int = _ADDR32_ENTRY_SIZE,
) -> JumpTableImage:
    """Load concrete table words at linked virtual ``base``.

    Prefers ``elf_path`` when it names an ET_EXEC/ET_DYN image. Falls back to
    ``dol_path`` data sections. ET_REL ``elf_path`` values may hydrate
    ``object-rodata`` only when the slice is fully present in-file (still
    unsound if ADDR32 relocs remain unresolved in that range). Raises
    ``JumpTableImageError`` when no complete image is available.
    """
    if entry_count <= 0:
        raise JumpTableImageError(f"entry_count must be positive, got {entry_count}")
    if entry_size != _ADDR32_ENTRY_SIZE:
        raise JumpTableImageError(
            f"only { _ADDR32_ENTRY_SIZE }-byte ADDR32 entries are supported, got {entry_size}"
        )
    if base & (_ADDR32_ENTRY_SIZE - 1):
        raise JumpTableImageError(f"table base 0x{base:x} is not word-aligned")

    byte_count = entry_count * entry_size
    last = base + byte_count - 1
    if last < base or last > 0xFFFFFFFF:
        raise JumpTableImageError(
            f"table span [{base:#x}, {last:#x}] wraps or exceeds 32-bit address space"
        )

    if elf_path is not None:
        image = _hydrate_from_elf(
            Path(elf_path),
            base=base,
            byte_count=byte_count,
            entry_count=entry_count,
            entry_size=entry_size,
        )
        if image is not None:
            return image

    if dol_path is not None:
        return _hydrate_from_dol(
            Path(dol_path),
            base=base,
            byte_count=byte_count,
            entry_count=entry_count,
            entry_size=entry_size,
        )

    raise JumpTableImageError(
        "no linked image supplied: provide elf_path (ET_EXEC/ET_DYN) or dol_path"
    )


def readonly_image_obligation(image: JumpTableImage) -> dict[str, object]:
    """Build an ``address_space`` obligation fragment for ``readonly-image`` proofs.

    Shape matches ``jump_table_obligations.validate_readonly_image_obligation``.
    """
    end = image.base + image.entry_count * image.entry_size - 1
    byte_count = image.entry_count * image.entry_size
    return {
        "kind": "rom-image",
        "base": image.base,
        "end": end,
        "image_sha256": image.image_sha256,
        "byte_count": byte_count,
        "entry_count": image.entry_count,
        "entry_size": image.entry_size,
        "source": image.source,
        "artifact_path": image.artifact_path,
        "words": list(image.words),
        "no_write": "pending",
        "algorithm": "rom-image-v1",
    }


def jump_table_words_from_image(image: JumpTableImage):
    """Adapt a hydrated image into ``JumpTableWords`` for SMT obligation helpers."""
    from tools.ppc_equivalence.jump_table_obligations import JumpTableWords

    return JumpTableWords(
        base=image.base,
        words=image.words,
        source=image.source,
        artifact_path=image.artifact_path,
        entry_size=image.entry_size,
    )


def _hydrate_from_elf(
    path: Path,
    *,
    base: int,
    byte_count: int,
    entry_count: int,
    entry_size: int,
) -> JumpTableImage | None:
    try:
        e_type = elf_type(path)
    except ElfSymbolError as exc:
        raise JumpTableImageError(str(exc)) from exc

    if e_type in (ET_EXEC, ET_DYN):
        source = "linked-elf"
    elif e_type == ET_REL:
        source = "object-rodata"
    else:
        return None

    payload, section_name = _slice_linked_elf(path, base=base, byte_count=byte_count)
    if e_type == ET_REL and _rodata_slice_has_unresolved_relocs(path, base, byte_count):
        raise JumpTableImageError(
            f"object {path} table slice [{base:#x}, +{byte_count:#x}) still has "
            f"unresolved ADDR32 relocations in {section_name!r}; use a linked ELF or DOL"
        )
    return _build_image(
        base=base,
        payload=payload,
        entry_count=entry_count,
        entry_size=entry_size,
        source=source,
        artifact_path=str(path.resolve()),
    )


def _hydrate_from_dol(
    path: Path,
    *,
    base: int,
    byte_count: int,
    entry_count: int,
    entry_size: int,
) -> JumpTableImage:
    try:
        slice_ = extract_data_by_address(path, base, byte_count)
    except DolSymbolError as exc:
        raise JumpTableImageError(str(exc)) from exc
    return _build_image(
        base=base,
        payload=slice_.code,
        entry_count=entry_count,
        entry_size=entry_size,
        source="linked-dol",
        artifact_path=str(path.resolve()),
    )


def _slice_linked_elf(path: Path, *, base: int, byte_count: int) -> tuple[bytes, str]:
    sections = list_allocatable_sections(path)
    for section in sections:
        section_end = section.addr + section.size
        if section.addr <= base and base + byte_count <= section_end:
            if section.executable:
                raise JumpTableImageError(
                    f"refusing jump-table hydration from executable section "
                    f"{section.name!r} at 0x{base:x} in {path}"
                )
            if section.is_nobits or not section.data:
                raise JumpTableImageError(
                    f"section {section.name!r} has no image bytes at 0x{base:x} in {path}"
                )
            offset = base - section.addr
            payload = section.data[offset : offset + byte_count]
            if len(payload) != byte_count:
                raise JumpTableImageError(
                    f"partial table bytes in {section.name!r} at 0x{base:x} "
                    f"(wanted {byte_count}, got {len(payload)}): {path}"
                )
            return payload, section.name
    raise JumpTableImageError(
        f"virtual range [0x{base:x}, +0x{byte_count:x}) not covered by any "
        f"allocatable ELF section in {path}"
    )


def _rodata_slice_has_unresolved_relocs(path: Path, base: int, byte_count: int) -> bool:
    from tools.ppc_equivalence.ir import R_PPC_ADDR32

    sections = list_allocatable_sections(path)
    for section in sections:
        section_end = section.addr + section.size
        if not (section.addr <= base and base + byte_count <= section_end):
            continue
        slice_start = base - section.addr
        slice_end = slice_start + byte_count
        for reloc in section.relocations:
            if reloc.relocation_type != R_PPC_ADDR32:
                continue
            entry_start = reloc.offset
            entry_end = entry_start + _ADDR32_ENTRY_SIZE
            if entry_start < slice_end and entry_end > slice_start:
                return True
    return False


def _build_image(
    *,
    base: int,
    payload: bytes,
    entry_count: int,
    entry_size: int,
    source: str,
    artifact_path: str,
) -> JumpTableImage:
    if len(payload) != entry_count * entry_size:
        raise JumpTableImageError(
            f"expected {entry_count * entry_size} image bytes, got {len(payload)}"
        )
    words = _decode_u32_words(payload, entry_count)
    _validate_code_pointer_alignment(words)
    return JumpTableImage(
        base=base,
        entry_count=entry_count,
        entry_size=entry_size,
        words=words,
        image_sha256=hashlib.sha256(payload).hexdigest(),
        source=source,
        artifact_path=artifact_path,
    )


def _decode_u32_words(payload: bytes, entry_count: int) -> tuple[int, ...]:
    if len(payload) != entry_count * _ADDR32_ENTRY_SIZE:
        raise JumpTableImageError("payload length is not an exact multiple of entry size")
    words: list[int] = []
    for index in range(entry_count):
        offset = index * _ADDR32_ENTRY_SIZE
        (word,) = struct.unpack_from(">I", payload, offset)
        words.append(word & 0xFFFFFFFF)
    return tuple(words)


def _validate_code_pointer_alignment(words: tuple[int, ...]) -> None:
    for index, word in enumerate(words):
        if word & (_ADDR32_ENTRY_SIZE - 1):
            raise JumpTableImageError(
                f"table entry {index} target 0x{word:x} is not word-aligned "
                "(assumed code pointer)"
            )

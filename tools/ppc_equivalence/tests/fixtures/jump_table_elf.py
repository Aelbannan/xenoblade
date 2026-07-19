"""Synthetic linked ELF fixtures for MWCC cmplwi/lwzx/mtctr/bctr jump tables.

Switch dispatch encoding (big-endian PPC, 4-byte aligned):

| Offset | Instruction            | Hex word   |
|--------|------------------------|------------|
| +0x00  | ``cmplwi r0, N``       | ``280000NN`` |
| +0x04  | ``slwi r0, r0, 2``     | ``5400103a`` (``rlwinm r0,r0,2,0,29``) |
| +0x08  | ``lwzx r3, r3, r0``     | ``7c63002e`` |
| +0x0C  | ``mtctr r3``           | ``7c6903a6`` |
| +0x10  | ``bctr``               | ``4e800420`` |

``N`` is the inclusive upper bound on the case index (``entry_count - 1``).
``r3`` must hold the linked virtual address of the ADDR32 table at the ``lwzx``.
Table entries are big-endian u32 code pointers (``R_PPC_ADDR32`` after link).
"""

from __future__ import annotations

import struct
from typing import Sequence

from tools.ppc_equivalence.elf_symbols import ET_EXEC
from tools.ppc_equivalence.tests.test_allocatable_sections import build_elf_with_rodata

# Default layout for linked ET_EXEC fixtures.
DEFAULT_TEXT_ADDR = 0x80100000
DEFAULT_TABLE_BASE = 0x80200000
DEFAULT_CASE_BASE = 0x80101000

# cmplwi r0,3; slwi; lwzx r3,r3,r0; mtctr r3; bctr  (4 cases, indices 0..3)
MWCC_JUMP_TABLE_SWITCH_4 = bytes.fromhex(
    "28000003 5400103a 7c63002e 7c6903a6 4e800420",
)

# Same shape with bound 8 (indices 0..8) — matches retail snippet in test_jump_table.py.
MWCC_JUMP_TABLE_SWITCH_9 = bytes.fromhex(
    "28000008 5400103a 7c63002e 7c6903a6 4e800420",
)


def cmplwi_bound_hex(entry_count: int) -> str:
    """Return switch-body hex with ``cmplwi r0, entry_count-1``."""
    if entry_count <= 0:
        raise ValueError("entry_count must be positive")
    bound = entry_count - 1
    return f"2800{bound:04x} 5400103a 7c63002e 7c6903a6 4e800420"


def table_bytes_from_targets(targets: Sequence[int]) -> bytes:
    """Pack ADDR32 jump-table words (big-endian u32)."""
    payload = bytearray()
    for target in targets:
        word = int(target) & 0xFFFFFFFF
        if word & 3:
            raise ValueError(f"target {word:#x} is not word-aligned")
        payload.extend(struct.pack(">I", word))
    return bytes(payload)


def sequential_case_targets(
    *,
    entry_count: int,
    case_base: int = DEFAULT_CASE_BASE,
    stride: int = 4,
) -> tuple[int, ...]:
    """Build ``entry_count`` word-aligned handler PCs at ``case_base + i*stride``."""
    return tuple(case_base + index * stride for index in range(entry_count))


def build_linked_jump_table_elf(
    *,
    table_targets: Sequence[int],
    switch_body: bytes = MWCC_JUMP_TABLE_SWITCH_4,
    text_addr: int = DEFAULT_TEXT_ADDR,
    table_base: int = DEFAULT_TABLE_BASE,
    e_type: int = ET_EXEC,
) -> bytes:
    """Minimal ET_EXEC with MWCC switch body in ``.text`` and resolved table in ``.rodata``."""
    if len(switch_body) % 4:
        raise ValueError("switch_body must be word-aligned")
    rodata = table_bytes_from_targets(table_targets)
    return build_elf_with_rodata(
        text=switch_body,
        rodata=rodata,
        text_addr=text_addr,
        rodata_addr=table_base,
        e_type=e_type,
    )


def identical_table_fixture(entry_count: int = 4) -> tuple[bytes, tuple[int, ...], int]:
    """Linked ELF where retail and candidate would share the same table image."""
    targets = sequential_case_targets(entry_count=entry_count)
    elf = build_linked_jump_table_elf(table_targets=targets)
    return elf, targets, DEFAULT_TABLE_BASE


def differing_case_fixture(
    *,
    entry_count: int = 4,
    differing_index: int = 1,
    candidate_override: int = 0x80102000,
) -> tuple[bytes, bytes, tuple[int, ...], tuple[int, ...], int]:
    """Two linked ELFs: same shape, one case target relocated on the candidate."""
    if differing_index < 0 or differing_index >= entry_count:
        raise ValueError("differing_index out of range")
    original_targets = sequential_case_targets(entry_count=entry_count)
    candidate_targets = list(original_targets)
    candidate_targets[differing_index] = candidate_override
    candidate_tuple = tuple(candidate_targets)
    table_base = DEFAULT_TABLE_BASE
    original_elf = build_linked_jump_table_elf(table_targets=original_targets)
    candidate_elf = build_linked_jump_table_elf(table_targets=candidate_tuple)
    return original_elf, candidate_elf, original_targets, candidate_tuple, table_base


def mismatched_entry_count_fixtures() -> tuple[bytes, bytes, int, int]:
    """Two linked ELFs with different table lengths (pairing must fail closed)."""
    table_base = DEFAULT_TABLE_BASE
    four = build_linked_jump_table_elf(
        table_targets=sequential_case_targets(entry_count=4),
    )
    three = build_linked_jump_table_elf(
        table_targets=sequential_case_targets(entry_count=3),
    )
    return four, three, 4, 3

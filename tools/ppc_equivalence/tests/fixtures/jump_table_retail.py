"""Retail US main.dol jump-table fixture (cmplwi / slwi / lwzx / mtctr / bctr).

Discovered by scanning ``orig/us/sys/main.dol`` text sections for unconditional
``bctr`` (``0x4e800420``) and running ``find_jump_table_candidates`` with a
48-byte lookback window (12 instructions — enough for ``cmplwi`` up to 8 slots
before the scaled index load).

Selected fixture (exact-pattern, moderate entry count):

| Field | Value |
|-------|-------|
| Branch PC | ``0x800e0d8c`` (``bctr``) |
| Switch start | ``0x800e0d74`` (``cmplwi r0, 13``) |
| Table base VA | ``0x8052b1bc`` |
| Bound imm | ``13`` (cases ``0..13``, 14 entries) |
| Base reg | ``r3`` via ``addis r23,-32685`` + ``addi r3,r23,-20036`` |

Table base is recovered manually from the ``addi`` immediately preceding the
``slwi`` (``0x800e0d7c: addi r3,r23,-20036`` after ``0x800e0d34: addis r23,r0,-32685``).
Automatic lis/addi recovery is tracked separately; this fixture hardcodes the
linked VA until that lands.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.dol_symbols import extract_by_address
from tools.ppc_equivalence.ir import UnsupportedInstruction
from tools.ppc_equivalence.jump_table import JumpTableCandidate, find_jump_table_candidates

# Conservative scan window: cmplwi may sit up to 8 insns before slwi/lwzx.
_LOOKBACK_BYTES = 48
_BCTR = bytes.fromhex("4e800420")

DEFAULT_RETAIL_DOL = Path("orig/us/sys/main.dol")

# Documented fixture at 0x800e0d8c (no symbol in config/us/symbols.txt).
RETAIL_BRANCH_PC = 0x800E0D8C
RETAIL_SWITCH_START = 0x800E0D74
RETAIL_TABLE_BASE = 0x8052B1BC
RETAIL_BOUND_IMM = 13
RETAIL_ENTRY_COUNT = RETAIL_BOUND_IMM + 1

# Golden words extracted from orig/us/sys/main.dol @ 0x8052b1bc (56 bytes).
RETAIL_TABLE_WORDS: tuple[int, ...] = (
    0x800E0D90,
    0x800E0DA0,
    0x800E0DB0,
    0x800E0DC0,
    0x800E0DCC,
    0x800E0DCC,
    0x800E0DCC,
    0x800E0DCC,
    0x800E0DCC,
    0x800E0DCC,
    0x800E0D90,
    0x800E0DA0,
    0x800E0DB0,
    0x800E0DC0,
)


@dataclass(frozen=True)
class RetailJumpTableFixture:
    branch_pc: int
    switch_start: int
    table_base: int
    bound_imm: int
    entry_count: int
    candidate: JumpTableCandidate
    table_words: tuple[int, ...]


def find_retail_jump_table_candidates(
    dol_path: Path | str,
    *,
    lookback_bytes: int = _LOOKBACK_BYTES,
) -> list[JumpTableCandidate]:
    """Scan linked DOL text for MWCC-style jump-table dispatch tails ending in ``bctr``."""
    from tools.ppc_equivalence.dol_symbols import text_sections

    path = Path(dol_path)
    data = path.read_bytes()
    found: list[JumpTableCandidate] = []
    for section in text_sections(path):
        blob = data[section.file_offset : section.file_offset + section.size]
        for offset in range(0, len(blob) - 4, 4):
            if blob[offset : offset + 4] != _BCTR:
                continue
            start = max(0, offset - lookback_bytes)
            window = blob[start : offset + 4]
            base = section.address + start
            try:
                instructions = decode_block(
                    window,
                    base=base,
                    validate_with_capstone=False,
                )
            except UnsupportedInstruction:
                continue
            branch_pc = section.address + offset
            for candidate in find_jump_table_candidates(instructions):
                if candidate.branch_pc == branch_pc:
                    found.append(candidate)
    return found


def load_documented_fixture(dol_path: Path | str) -> RetailJumpTableFixture:
    """Return the documented retail fixture, verifying pattern recognition in DOL."""
    path = Path(dol_path)
    switch_bytes = extract_by_address(
        path,
        RETAIL_SWITCH_START,
        RETAIL_BRANCH_PC + 4 - RETAIL_SWITCH_START,
    ).code
    instructions = decode_block(
        switch_bytes,
        base=RETAIL_SWITCH_START,
        validate_with_capstone=False,
    )
    candidates = find_jump_table_candidates(instructions)
    matching = [c for c in candidates if c.branch_pc == RETAIL_BRANCH_PC]
    if len(matching) != 1:
        raise ValueError(
            f"expected one jump-table candidate at {RETAIL_BRANCH_PC:#x}, "
            f"found {len(matching)}"
        )
    candidate = matching[0]
    if candidate.confidence != "exact-pattern":
        raise ValueError(
            f"fixture at {RETAIL_BRANCH_PC:#x} is no longer exact-pattern: "
            f"{candidate.confidence} {candidate.notes}"
        )
    if candidate.bound_imm != RETAIL_BOUND_IMM:
        raise ValueError(
            f"bound_imm mismatch: expected {RETAIL_BOUND_IMM}, got {candidate.bound_imm}"
        )
    return RetailJumpTableFixture(
        branch_pc=RETAIL_BRANCH_PC,
        switch_start=RETAIL_SWITCH_START,
        table_base=RETAIL_TABLE_BASE,
        bound_imm=RETAIL_BOUND_IMM,
        entry_count=RETAIL_ENTRY_COUNT,
        candidate=candidate,
        table_words=RETAIL_TABLE_WORDS,
    )

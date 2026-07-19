"""Affine / CTR closed-form loop summaries (descriptors + formulas; not yet engine-wired).

Target shape (MWCC / EABI counted loops)::

    li / addi  rT, _, N     # concrete trip count N >= 1
    mtctr      rT
  header:
    addi       rD, rD, K    # zero or more constant-stride GPR updates
    bdnz       header       # BO=16: decrement CTR, branch if CTR != 0

PPC corners encoded in the summary notes:

- ``bdnz`` decrements CTR **before** the zero test.
- Loading CTR with ``0`` makes ``bdnz`` wrap to ``0xffffffff`` (not a zero-trip loop).
- Closed forms assume no unsigned wrap of the affine accumulators over ``N`` steps.

``find_ctr_affine_loop_candidates`` / ``summarize_ctr_affine_loop`` are descriptive
only. Applying a summary inside ``execute_cfg`` / claiming ``EQUIVALENT`` via
``proof_features: [\"affine-loop-summary\"]`` requires a later engine discharge
step (feature remains in ``UNSUPPORTED_FOR_EQUIVALENT`` until then).
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass

from tools.ppc_equivalence.ir import Instruction, Opcode

_CTR_SPR = 9
_BDNZ_BO = 16  # decrement CTR; branch if CTR != 0 after decrement


@dataclass(frozen=True)
class AffineGprUpdate:
    """One loop-body step ``addi rD, rD, K`` (constant stride on a single GPR)."""

    reg: int
    addend: int


@dataclass(frozen=True)
class CtrAffineLoopCandidate:
    """Recognized ``mtctr`` / body / ``bdnz`` counted loop (pattern only)."""

    mtctr_pc: int
    header_pc: int
    latch_pc: int
    exit_pc: int
    trip_count: int | None
    trip_count_reg: int | None
    body_updates: tuple[AffineGprUpdate, ...]
    instruction_indexes: tuple[int, ...]
    confidence: str
    notes: tuple[str, ...]


@dataclass(frozen=True)
class LoopSummary:
    """Closed-form summary for a recognized CTR affine loop.

    ``final_gpr[reg] = (entry_reg, stride)`` means
    ``GPR[reg]_exit = GPR[entry_reg]_entry + trip_count * stride`` when
    ``trip_count`` is concrete and wrap-free. ``final_ctr`` is the CTR after exit.
    """

    header_pc: int
    latch_pc: int
    exit_pc: int
    trip_count: int | None
    final_gpr: dict[int, tuple[int, int]]
    final_ctr: int
    ranking: str
    proof_kind: str
    invariant_notes: tuple[str, ...]
    entry_condition: str
    exit_condition: str


def find_ctr_affine_loop_candidates(
    instructions: Sequence[Instruction],
) -> list[CtrAffineLoopCandidate]:
    """Scan for concrete ``mtctr`` + affine body + ``bdnz`` back-edge loops."""
    if not instructions:
        return []

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    candidates: list[CtrAffineLoopCandidate] = []

    for index, insn in enumerate(instructions):
        if not _is_bdnz(insn):
            continue
        target = int(insn.operands[2]) & 0xFFFFFFFC
        header_index = by_address.get(target)
        if header_index is None or header_index >= index:
            continue  # not a back-edge into this block

        body = list(instructions[header_index:index])
        updates, body_notes = _parse_affine_body(body)
        if body_notes and updates is None:
            continue

        mtctr_index = header_index - 1
        if mtctr_index < 0:
            continue
        mtctr = instructions[mtctr_index]
        if not _is_mtctr(mtctr):
            continue
        trip_reg = int(mtctr.operands[0])
        trip_count, trip_notes = _concrete_trip_count(instructions, mtctr_index, trip_reg)

        notes = list(trip_notes)
        notes.extend(body_notes)
        if trip_count == 0:
            notes.append("CTR load of 0 wraps under bdnz (not a zero-trip loop)")
        confidence = "exact-pattern" if trip_count is not None and trip_count >= 1 else "partial"

        candidates.append(
            CtrAffineLoopCandidate(
                mtctr_pc=mtctr.address,
                header_pc=instructions[header_index].address,
                latch_pc=insn.address,
                exit_pc=insn.address + 4,
                trip_count=trip_count,
                trip_count_reg=trip_reg,
                body_updates=tuple(updates or ()),
                instruction_indexes=tuple(range(mtctr_index, index + 1)),
                confidence=confidence,
                notes=tuple(notes),
            ),
        )
    return candidates


def summarize_ctr_affine_loop(candidate: CtrAffineLoopCandidate) -> LoopSummary | None:
    """Build a closed-form ``LoopSummary`` when the trip count is a positive constant."""
    if candidate.trip_count is None or candidate.trip_count < 1:
        return None

    final_gpr = {
        update.reg: (update.reg, update.addend)
        for update in candidate.body_updates
    }
    # Collapse multiple updates to the same register (sum strides).
    collapsed: dict[int, tuple[int, int]] = {}
    for update in candidate.body_updates:
        _entry, stride = collapsed.get(update.reg, (update.reg, 0))
        collapsed[update.reg] = (update.reg, stride + update.addend)

    return LoopSummary(
        header_pc=candidate.header_pc,
        latch_pc=candidate.latch_pc,
        exit_pc=candidate.exit_pc,
        trip_count=candidate.trip_count,
        final_gpr=collapsed,
        final_ctr=0,
        ranking="ctr-descending",
        proof_kind="affine-closed-form",
        invariant_notes=tuple(candidate.notes),
        entry_condition=f"CTR == {candidate.trip_count}",
        exit_condition="CTR == 0 after bdnz exhaust",
    )


def closed_form_gpr_value(entry_value: int, stride: int, trip_count: int) -> int:
    """Evaluate ``entry + trip_count * stride`` in 32-bit two's complement."""
    return (entry_value + trip_count * stride) & 0xFFFFFFFF


def _is_bdnz(insn: Instruction) -> bool:
    if insn.opcode != Opcode.BC or insn.link:
        return False
    bo, _bi, _target, _aa = insn.operands
    return int(bo) == _BDNZ_BO


def _is_mtctr(insn: Instruction) -> bool:
    return (
        insn.opcode == Opcode.MTSPR
        and len(insn.operands) == 2
        and int(insn.operands[1]) == _CTR_SPR
    )


def _parse_affine_body(
    body: Sequence[Instruction],
) -> tuple[list[AffineGprUpdate] | None, list[str]]:
    if not body:
        return [], ["empty loop body"]
    updates: list[AffineGprUpdate] = []
    notes: list[str] = []
    for insn in body:
        if insn.opcode != Opcode.ADDI:
            return None, [f"unsupported body opcode {insn.opcode.value}"]
        rt, ra, imm = insn.operands
        if int(rt) != int(ra):
            return None, [f"non-affine addi r{rt}, r{ra}, {imm}"]
        if int(rt) == 0:
            notes.append("body writes r0 (volatile under EABI)")
        updates.append(AffineGprUpdate(reg=int(rt), addend=int(imm)))
    return updates, notes


def _concrete_trip_count(
    instructions: Sequence[Instruction],
    mtctr_index: int,
    trip_reg: int,
) -> tuple[int | None, list[str]]:
    """Recover ``N`` from ``li``/``addi rT, 0, N`` immediately before ``mtctr``."""
    if mtctr_index < 1:
        return None, ["missing CTR materialization before mtctr"]
    prev = instructions[mtctr_index - 1]
    if prev.opcode != Opcode.ADDI:
        return None, ["CTR source is not a concrete addi/li"]
    rt, ra, imm = prev.operands
    if int(rt) != trip_reg:
        return None, [f"addi destination r{rt} != mtctr source r{trip_reg}"]
    if int(ra) != 0:
        return None, ["CTR materialization is not an immediate li-form addi"]
    return int(imm) & 0xFFFFFFFF, []

"""Affine / CTR closed-form loop summaries.

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

When ``execute_cfg`` is given a summary map (or auto-builds one), matching
headers are discharged in closed form and ``affine-loop-summary`` may authorize
``EQUIVALENT``.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass, replace
from typing import Any

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


def build_affine_summary_map(
    instructions: Sequence[Instruction],
) -> dict[int, LoopSummary]:
    """Map loop header PC → closed-form summary for exact-pattern CTR affine loops."""
    mapping: dict[int, LoopSummary] = {}
    for candidate in find_ctr_affine_loop_candidates(instructions):
        if candidate.confidence != "exact-pattern":
            continue
        summary = summarize_ctr_affine_loop(candidate)
        if summary is None:
            continue
        # One summary per header; overlapping headers are fail-closed (skip).
        if summary.header_pc in mapping:
            del mapping[summary.header_pc]
            continue
        mapping[summary.header_pc] = summary
    return mapping


def apply_affine_loop_summary(state: Any, summary: LoopSummary, ops: Any) -> Any:
    """Return a post-loop state under the closed-form summary (concrete trip count)."""
    if summary.trip_count is None or summary.trip_count < 1:
        raise ValueError("affine summary requires a positive concrete trip count")
    gprs = list(state.gpr)
    for reg, (entry_reg, stride) in summary.final_gpr.items():
        delta = (int(summary.trip_count) * int(stride)) & 0xFFFFFFFF
        gprs[reg] = ops.add(state.gpr[entry_reg], ops.const(delta))
    return replace(state, gpr=tuple(gprs), ctr=ops.const(int(summary.final_ctr) & 0xFFFFFFFF))


def build_loop_summary_obligation(
    summary: LoopSummary,
    *,
    coverage: str = "pending",
) -> dict[str, Any]:
    """Obligation block for ``proof_features: [\"affine-loop-summary\"]``."""
    return {
        "proof_kind": summary.proof_kind,
        "header_pc": summary.header_pc,
        "latch_pc": summary.latch_pc,
        "exit_pc": summary.exit_pc,
        "trip_count": summary.trip_count,
        "final_ctr": summary.final_ctr,
        "ranking": summary.ranking,
        "entry_condition": summary.entry_condition,
        "exit_condition": summary.exit_condition,
        "final_gpr": [
            {"reg": reg, "entry_reg": entry_reg, "stride": stride}
            for reg, (entry_reg, stride) in sorted(summary.final_gpr.items())
        ],
        "coverage": coverage,
        "algorithm": "ctr-affine-v1",
    }


def validate_loop_summary_obligation(data: dict[str, Any]) -> str | None:
    """Return None when a ``loop_summary`` obligation object is well-formed."""
    required = (
        "proof_kind",
        "header_pc",
        "latch_pc",
        "exit_pc",
        "trip_count",
        "final_ctr",
        "ranking",
        "final_gpr",
    )
    for key in required:
        if key not in data:
            return f"loop_summary missing {key!r}"
    if not isinstance(data["proof_kind"], str) or not data["proof_kind"]:
        return "loop_summary.proof_kind must be a non-empty string"
    for key in ("header_pc", "latch_pc", "exit_pc", "final_ctr"):
        if not isinstance(data[key], int):
            return f"loop_summary.{key} must be an int"
    if data["trip_count"] is not None and not isinstance(data["trip_count"], int):
        return "loop_summary.trip_count must be an int or null"
    if not isinstance(data["ranking"], str) or not data["ranking"]:
        return "loop_summary.ranking must be a non-empty string"
    raw_gpr = data["final_gpr"]
    if not isinstance(raw_gpr, list):
        return "loop_summary.final_gpr must be a list"
    for index, item in enumerate(raw_gpr):
        if not isinstance(item, dict):
            return f"loop_summary.final_gpr[{index}] must be an object"
        for field in ("reg", "entry_reg", "stride"):
            if field not in item or not isinstance(item[field], int):
                return f"loop_summary.final_gpr[{index}].{field} must be an int"
    return None


def closed_form_gpr_value(entry_value: int, stride: int, trip_count: int) -> int:
    """Evaluate ``entry + trip_count * stride`` in 32-bit two's complement."""
    return (entry_value + trip_count * stride) & 0xFFFFFFFF


REQUIRED_LOOP_SUMMARY_KEYS = frozenset({
    "proof_kind",
    "header_pc",
    "latch_pc",
    "exit_pc",
    "trip_count",
    "final_ctr",
    "ranking",
    "final_gpr",
})


def build_loop_summary_obligation(
    summary: LoopSummary,
    *,
    coverage: str = "pending",
) -> dict[str, Any]:
    """Obligation block for ``proof_features: [\"affine-loop-summary\"]``."""
    final_gpr = [
        {
            "reg": reg,
            "entry_reg": entry_reg,
            "stride": stride,
        }
        for reg, (entry_reg, stride) in sorted(summary.final_gpr.items())
    ]
    return {
        "proof_kind": summary.proof_kind,
        "header_pc": summary.header_pc,
        "latch_pc": summary.latch_pc,
        "exit_pc": summary.exit_pc,
        "trip_count": summary.trip_count,
        "final_ctr": summary.final_ctr,
        "ranking": summary.ranking,
        "final_gpr": final_gpr,
        "coverage": coverage,
    }


def validate_loop_summary_obligation(obligation: dict[str, Any]) -> str | None:
    """Return None when a loop-summary obligation is structurally well-formed."""
    missing = sorted(REQUIRED_LOOP_SUMMARY_KEYS - obligation.keys())
    if missing:
        return "loop_summary missing " + ", ".join(missing)

    proof_kind = obligation.get("proof_kind")
    if not isinstance(proof_kind, str) or not proof_kind:
        return "loop_summary.proof_kind must be a nonempty string"

    for key in ("header_pc", "latch_pc", "exit_pc", "final_ctr"):
        value = obligation.get(key)
        if not isinstance(value, int) or value < 0 or value > 0xFFFFFFFF:
            return f"loop_summary.{key} must be a u32 int"

    trip_count = obligation.get("trip_count")
    if not isinstance(trip_count, int) or trip_count < 1 or trip_count > 0xFFFFFFFF:
        return "loop_summary.trip_count must be a positive u32 int"

    ranking = obligation.get("ranking")
    if not isinstance(ranking, str) or not ranking:
        return "loop_summary.ranking must be a nonempty string"

    return _validate_final_gpr(obligation.get("final_gpr"))


def _validate_final_gpr(final_gpr: Any) -> str | None:
    if isinstance(final_gpr, list):
        seen: set[int] = set()
        for index, entry in enumerate(final_gpr):
            if not isinstance(entry, dict):
                return f"loop_summary.final_gpr[{index}] must be an object"
            reg = entry.get("reg")
            entry_reg = entry.get("entry_reg")
            stride = entry.get("stride")
            if not isinstance(reg, int) or reg < 0 or reg > 31:
                return f"loop_summary.final_gpr[{index}].reg must be a GPR index 0..31"
            if not isinstance(entry_reg, int) or entry_reg < 0 or entry_reg > 31:
                return (
                    f"loop_summary.final_gpr[{index}].entry_reg must be a GPR index 0..31"
                )
            if not isinstance(stride, int):
                return f"loop_summary.final_gpr[{index}].stride must be an int"
            if reg in seen:
                return f"duplicate loop_summary.final_gpr reg r{reg}"
            seen.add(reg)
        return None

    if isinstance(final_gpr, dict):
        seen_regs: set[int] = set()
        for reg_key, entry in final_gpr.items():
            if not isinstance(reg_key, str) or not reg_key.isdigit():
                return "loop_summary.final_gpr dict keys must be decimal reg strings"
            reg = int(reg_key)
            if reg < 0 or reg > 31:
                return f"loop_summary.final_gpr[{reg_key!r}] reg out of range 0..31"
            if reg in seen_regs:
                return f"duplicate loop_summary.final_gpr reg r{reg}"
            seen_regs.add(reg)
            if not isinstance(entry, dict):
                return f"loop_summary.final_gpr[{reg_key!r}] must be an object"
            entry_reg = entry.get("entry_reg")
            stride = entry.get("stride")
            if not isinstance(entry_reg, int) or entry_reg < 0 or entry_reg > 31:
                return (
                    f"loop_summary.final_gpr[{reg_key!r}].entry_reg must be a GPR index 0..31"
                )
            if not isinstance(stride, int):
                return f"loop_summary.final_gpr[{reg_key!r}].stride must be an int"
        return None

    return "loop_summary.final_gpr must be a list or object keyed by reg string"


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

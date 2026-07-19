"""Constant-stride store loop recognition and closed-form discharge.

Typical MWCC counted-loop shapes::

    li / addi  rT, _, N
    mtctr      rT
  header:
    stw / stwu / stb / sth  rS, disp(rB)   # D-form store
    addi       rB, rB, K                     # optional when not stwu
    bdnz       header

``stwu`` advances the base register by ``disp``; otherwise a single
``addi rB, rB, K`` on the store base register must match the store width
and equal the memory stride. Indexed stores and multi-store bodies are
rejected (prefer false negatives).

When ``execute_cfg`` is given a summary map (or the engine auto-builds one),
matching headers with a positive concrete trip count are discharged by
applying the N stores in closed form and advancing the base/CTR.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass, replace
from typing import Any

from tools.ppc_equivalence.ir import Instruction, Opcode

_CTR_SPR = 9
_BDNZ_BO = 16  # decrement CTR; branch if CTR != 0 after decrement

# Fail closed above this trip count: closed-form applies N explicit stores.
MAX_MEMORY_LOOP_TRIPS = 4096

_STORE_WIDTH: dict[Opcode, int] = {
    Opcode.STB: 1,
    Opcode.STH: 2,
    Opcode.STW: 4,
    Opcode.STWU: 4,
}


@dataclass(frozen=True)
class ConstantStrideStoreLoop:
    """Recognized CTR counted loop whose body performs constant-stride stores."""

    header_pc: int
    latch_pc: int
    exit_pc: int
    mtctr_pc: int
    base_reg: int
    index_reg: int | None
    stride: int
    store_width: int
    source_reg: int
    store_kind: str  # "stwu" | "d-form-addi"
    trip_count: int | None
    trip_count_reg: int | None
    confidence: str
    notes: tuple[str, ...]


@dataclass(frozen=True)
class MemoryLoopSummary:
    """Closed-form summary for a constant-stride CTR store loop."""

    header_pc: int
    latch_pc: int
    exit_pc: int
    trip_count: int
    base_reg: int
    source_reg: int
    stride: int
    store_width: int
    store_kind: str
    final_ctr: int
    ranking: str
    proof_kind: str
    invariant_notes: tuple[str, ...]


def find_constant_stride_store_loops(
    instructions: Sequence[Instruction],
) -> list[ConstantStrideStoreLoop]:
    """Scan for ``mtctr`` / store body / ``bdnz`` constant-stride store loops."""
    if not instructions:
        return []

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    loops: list[ConstantStrideStoreLoop] = []

    for index, insn in enumerate(instructions):
        if not _is_bdnz(insn):
            continue
        target = int(insn.operands[2]) & 0xFFFFFFFC
        header_index = by_address.get(target)
        if header_index is None or header_index >= index:
            continue

        body = list(instructions[header_index:index])
        parsed, body_notes = _parse_constant_stride_store_body(body)
        if parsed is None:
            continue

        mtctr_index = header_index - 1
        if mtctr_index < 0:
            continue
        mtctr = instructions[mtctr_index]
        if not _is_mtctr(mtctr):
            continue
        trip_reg = int(mtctr.operands[0])
        trip_count, trip_notes = _concrete_trip_count(instructions, mtctr_index, trip_reg)

        notes = list(body_notes)
        notes.extend(trip_notes)
        if trip_count == 0:
            notes.append("CTR load of 0 wraps under bdnz (not a zero-trip loop)")
        confidence = (
            "exact-pattern"
            if trip_count is not None and trip_count >= 1
            else "partial"
        )

        loops.append(
            ConstantStrideStoreLoop(
                header_pc=instructions[header_index].address,
                latch_pc=insn.address,
                exit_pc=insn.address + 4,
                mtctr_pc=mtctr.address,
                base_reg=parsed.base_reg,
                index_reg=parsed.index_reg,
                stride=parsed.stride,
                store_width=parsed.store_width,
                source_reg=parsed.source_reg,
                store_kind=parsed.store_kind,
                trip_count=trip_count,
                trip_count_reg=trip_reg,
                confidence=confidence,
                notes=tuple(notes),
            ),
        )
    return loops


def summarize_constant_stride_store_loop(
    loop: ConstantStrideStoreLoop,
) -> MemoryLoopSummary | None:
    """Build a closed-form summary when the trip count is a positive constant."""
    if loop.confidence != "exact-pattern":
        return None
    if loop.trip_count is None or loop.trip_count < 1:
        return None
    if loop.trip_count > MAX_MEMORY_LOOP_TRIPS:
        return None
    span = int(loop.trip_count) * int(loop.stride)
    if span > 0xFFFFFFFF:
        return None
    if loop.store_kind not in ("stwu", "d-form-addi"):
        return None

    return MemoryLoopSummary(
        header_pc=loop.header_pc,
        latch_pc=loop.latch_pc,
        exit_pc=loop.exit_pc,
        trip_count=int(loop.trip_count),
        base_reg=loop.base_reg,
        source_reg=loop.source_reg,
        stride=loop.stride,
        store_width=loop.store_width,
        store_kind=loop.store_kind,
        final_ctr=0,
        ranking="ctr-descending",
        proof_kind="constant-stride-store",
        invariant_notes=tuple(loop.notes),
    )


def build_memory_loop_summary_map(
    instructions: Sequence[Instruction],
) -> dict[int, MemoryLoopSummary]:
    """Map loop header PC → closed-form memory-loop summary."""
    mapping: dict[int, MemoryLoopSummary] = {}
    for loop in find_constant_stride_store_loops(instructions):
        summary = summarize_constant_stride_store_loop(loop)
        if summary is None:
            continue
        if summary.header_pc in mapping:
            del mapping[summary.header_pc]
            continue
        mapping[summary.header_pc] = summary
    return mapping


def apply_memory_loop_summary(state: Any, summary: MemoryLoopSummary, ops: Any) -> Any:
    """Return a post-loop state under the closed-form store summary."""
    if summary.trip_count < 1:
        raise ValueError("memory-loop summary requires a positive concrete trip count")

    memory = state.memory
    base = state.gpr[summary.base_reg]
    value = state.gpr[summary.source_reg]
    width = int(summary.store_width)
    stride = int(summary.stride)

    for index in range(summary.trip_count):
        if summary.store_kind == "stwu":
            # First store at base+stride; subsequent at base+2*stride, ...
            offset = (index + 1) * stride
        else:
            # d-form store at disp 0 then addi: stores at base, base+stride, ...
            offset = index * stride
        address = ops.add(base, ops.const(offset & 0xFFFFFFFF))
        memory = _store_bytes(memory, address, value, width, ops)

    gprs = list(state.gpr)
    delta = (int(summary.trip_count) * stride) & 0xFFFFFFFF
    gprs[summary.base_reg] = ops.add(base, ops.const(delta))
    return replace(
        state,
        gpr=tuple(gprs),
        memory=memory,
        ctr=ops.const(int(summary.final_ctr) & 0xFFFFFFFF),
    )


REQUIRED_MEMORY_LOOP_KEYS = frozenset({
    "proof_kind",
    "header_pc",
    "latch_pc",
    "exit_pc",
    "trip_count",
    "base_reg",
    "source_reg",
    "stride",
    "store_width",
    "store_kind",
    "final_ctr",
    "ranking",
})


def build_memory_loop_obligation(
    summary: MemoryLoopSummary,
    *,
    coverage: str = "pending",
) -> dict[str, Any]:
    """Obligation block for ``proof_features: [\"memory-loop-summary\"]``."""
    return {
        "proof_kind": summary.proof_kind,
        "header_pc": summary.header_pc,
        "latch_pc": summary.latch_pc,
        "exit_pc": summary.exit_pc,
        "trip_count": summary.trip_count,
        "base_reg": summary.base_reg,
        "source_reg": summary.source_reg,
        "stride": summary.stride,
        "store_width": summary.store_width,
        "store_kind": summary.store_kind,
        "final_ctr": summary.final_ctr,
        "ranking": summary.ranking,
        "coverage": coverage,
    }


def validate_memory_loop_obligation(obligation: dict[str, Any]) -> str | None:
    """Return None when a memory-loop obligation is structurally well-formed."""
    missing = sorted(REQUIRED_MEMORY_LOOP_KEYS - obligation.keys())
    if missing:
        return "memory_loop missing " + ", ".join(missing)

    proof_kind = obligation.get("proof_kind")
    if not isinstance(proof_kind, str) or not proof_kind:
        return "memory_loop.proof_kind must be a nonempty string"

    for key in ("header_pc", "latch_pc", "exit_pc", "final_ctr"):
        value = obligation.get(key)
        if not isinstance(value, int) or value < 0 or value > 0xFFFFFFFF:
            return f"memory_loop.{key} must be a u32 int"

    trip_count = obligation.get("trip_count")
    if not isinstance(trip_count, int) or trip_count < 1 or trip_count > 0xFFFFFFFF:
        return "memory_loop.trip_count must be a positive u32 int"

    for key in ("base_reg", "source_reg"):
        value = obligation.get(key)
        if not isinstance(value, int) or value < 0 or value > 31:
            return f"memory_loop.{key} must be a GPR index 0..31"

    stride = obligation.get("stride")
    if not isinstance(stride, int) or stride < 1 or stride > 0xFFFFFFFF:
        return "memory_loop.stride must be a positive u32 int"

    store_width = obligation.get("store_width")
    if store_width not in (1, 2, 4):
        return "memory_loop.store_width must be 1, 2, or 4"

    store_kind = obligation.get("store_kind")
    if store_kind not in ("stwu", "d-form-addi"):
        return "memory_loop.store_kind must be 'stwu' or 'd-form-addi'"

    ranking = obligation.get("ranking")
    if not isinstance(ranking, str) or not ranking:
        return "memory_loop.ranking must be a nonempty string"

    return None


@dataclass(frozen=True)
class _ParsedStoreBody:
    base_reg: int
    index_reg: int | None
    stride: int
    store_width: int
    source_reg: int
    store_kind: str


def _parse_constant_stride_store_body(
    body: Sequence[Instruction],
) -> tuple[_ParsedStoreBody | None, list[str]]:
    if not body:
        return None, ["empty loop body"]

    store_insn: Instruction | None = None
    pointer_addi: tuple[int, int] | None = None
    notes: list[str] = []

    for insn in body:
        if insn.opcode in _STORE_WIDTH:
            if store_insn is not None:
                return None, ["multiple stores in loop body"]
            store_insn = insn
            continue
        if insn.opcode == Opcode.ADDI:
            rt, ra, imm = insn.operands
            if int(rt) != int(ra):
                return None, [f"non-pointer addi r{rt}, r{ra}, {imm}"]
            if pointer_addi is not None:
                return None, ["multiple pointer addi in loop body"]
            pointer_addi = (int(rt), int(imm))
            continue
        return None, [f"unsupported body opcode {insn.opcode.value}"]

    if store_insn is None:
        return None, ["no store in loop body"]

    width = _STORE_WIDTH[store_insn.opcode]
    source_reg, base_reg, disp = (int(v) for v in store_insn.operands)
    if base_reg == 0:
        return None, ["store uses r0 as base"]

    if store_insn.opcode == Opcode.STWU:
        if pointer_addi is not None:
            return None, ["stwu body must not include separate addi"]
        stride = int(disp)
        if stride <= 0:
            return None, [f"stwu disp {stride} not positive"]
        if stride != width:
            return None, [f"stwu disp {stride} != store width {width}"]
        return (
            _ParsedStoreBody(
                base_reg=base_reg,
                index_reg=None,
                stride=stride,
                store_width=width,
                source_reg=source_reg,
                store_kind="stwu",
            ),
            notes,
        )

    if disp != 0:
        return None, [f"store disp {disp} != 0 without indexed addressing"]

    if pointer_addi is None:
        return None, ["store without pointer advance"]

    addi_reg, addi_imm = pointer_addi
    if addi_reg != base_reg:
        return None, [f"addi r{addi_reg} does not match store base r{base_reg}"]

    stride = int(addi_imm)
    if stride <= 0:
        return None, [f"stride {stride} not positive"]
    if stride != width:
        return None, [f"stride {stride} != store width {width}"]

    return (
        _ParsedStoreBody(
            base_reg=base_reg,
            index_reg=None,
            stride=stride,
            store_width=width,
            source_reg=source_reg,
            store_kind="d-form-addi",
        ),
        notes,
    )


def _store_bytes(memory: Any, address: Any, value: Any, width: int, ops: Any) -> Any:
    result = memory
    for offset in range(width):
        shift = (width - 1 - offset) * 8
        byte = ops.band(ops.lshr(value, ops.const(shift)), ops.const(0xFF))
        result = ops.store_byte(result, ops.add(address, ops.const(offset)), byte)
    return result


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

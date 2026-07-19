"""Constant-stride store loop recognition (scaffold only).

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

Not wired into ``execute_cfg`` / ``check_equivalence``; ``memory-loop-summary``
stays in ``UNSUPPORTED_FOR_EQUIVALENT`` until closed-form discharge exists.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass

from tools.ppc_equivalence.ir import Instruction, Opcode

_CTR_SPR = 9
_BDNZ_BO = 16  # decrement CTR; branch if CTR != 0 after decrement

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
    trip_count: int | None
    trip_count_reg: int | None
    confidence: str
    notes: tuple[str, ...]


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
                trip_count=trip_count,
                trip_count_reg=trip_reg,
                confidence=confidence,
                notes=tuple(notes),
            ),
        )
    return loops


@dataclass(frozen=True)
class _ParsedStoreBody:
    base_reg: int
    index_reg: int | None
    stride: int
    store_width: int
    source_reg: int


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
        ),
        notes,
    )


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

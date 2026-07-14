from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Protocol

from .ir import Instruction, Opcode, UnsupportedInstruction
from .model import MachineState

MASK32 = 0xFFFFFFFF


class WordOps(Protocol):
    def const(self, value: int) -> Any: ...
    def bool(self, value: bool) -> Any: ...
    def add(self, left: Any, right: Any) -> Any: ...
    def sub(self, left: Any, right: Any) -> Any: ...
    def mul(self, left: Any, right: Any) -> Any: ...
    def band(self, left: Any, right: Any) -> Any: ...
    def bor(self, left: Any, right: Any) -> Any: ...
    def bxor(self, left: Any, right: Any) -> Any: ...
    def bnot(self, value: Any) -> Any: ...
    def shl(self, value: Any, amount: int) -> Any: ...
    def lshr(self, value: Any, amount: int) -> Any: ...
    def signed_lt(self, left: Any, right: Any) -> Any: ...
    def unsigned_lt(self, left: Any, right: Any) -> Any: ...
    def eq(self, left: Any, right: Any) -> Any: ...
    def ite(self, condition: Any, yes: Any, no: Any) -> Any: ...


@dataclass(frozen=True, slots=True)
class ConcreteOps:
    def const(self, value: int) -> int:
        return value & MASK32

    def bool(self, value: bool) -> bool:
        return value

    def add(self, left: int, right: int) -> int:
        return (left + right) & MASK32

    def sub(self, left: int, right: int) -> int:
        return (left - right) & MASK32

    def mul(self, left: int, right: int) -> int:
        return (left * right) & MASK32

    def band(self, left: int, right: int) -> int:
        return left & right

    def bor(self, left: int, right: int) -> int:
        return left | right

    def bxor(self, left: int, right: int) -> int:
        return left ^ right

    def bnot(self, value: int) -> int:
        return (~value) & MASK32

    def shl(self, value: int, amount: int) -> int:
        return (value << amount) & MASK32

    def lshr(self, value: int, amount: int) -> int:
        return (value & MASK32) >> amount

    def signed_lt(self, left: int, right: int) -> bool:
        signed = lambda value: value - 0x100000000 if value & 0x80000000 else value
        return signed(left) < signed(right)

    def unsigned_lt(self, left: int, right: int) -> bool:
        return (left & MASK32) < (right & MASK32)

    def eq(self, left: int, right: int) -> bool:
        return (left & MASK32) == (right & MASK32)

    def ite(self, condition: bool, yes: Any, no: Any) -> Any:
        return yes if condition else no


class SymbolicOps:
    def __init__(self) -> None:
        try:
            import z3
        except ImportError as exc:
            raise RuntimeError(
                "z3-solver is required; install tools/ppc_equivalence/requirements.txt"
            ) from exc
        self.z3 = z3

    def const(self, value: int) -> Any:
        return self.z3.BitVecVal(value & MASK32, 32)

    def bool(self, value: bool) -> Any:
        return self.z3.BoolVal(value)

    def add(self, left: Any, right: Any) -> Any:
        return left + right

    def sub(self, left: Any, right: Any) -> Any:
        return left - right

    def mul(self, left: Any, right: Any) -> Any:
        return left * right

    def band(self, left: Any, right: Any) -> Any:
        return left & right

    def bor(self, left: Any, right: Any) -> Any:
        return left | right

    def bxor(self, left: Any, right: Any) -> Any:
        return left ^ right

    def bnot(self, value: Any) -> Any:
        return ~value

    def shl(self, value: Any, amount: int) -> Any:
        return value << amount

    def lshr(self, value: Any, amount: int) -> Any:
        return self.z3.LShR(value, amount)

    def signed_lt(self, left: Any, right: Any) -> Any:
        return left < right

    def unsigned_lt(self, left: Any, right: Any) -> Any:
        return self.z3.ULT(left, right)

    def eq(self, left: Any, right: Any) -> Any:
        return left == right

    def ite(self, condition: Any, yes: Any, no: Any) -> Any:
        return self.z3.If(condition, yes, no)


def rotate_mask(mb: int, me: int) -> int:
    if not 0 <= mb < 32 or not 0 <= me < 32:
        raise ValueError("rotate mask endpoints must be in [0, 31]")
    mask = 0
    index = mb
    while True:
        mask |= 1 << (31 - index)
        if index == me:
            return mask
        index = (index + 1) & 31


def _rotl(value: Any, amount: int, ops: WordOps) -> Any:
    amount &= 31
    if amount == 0:
        return value
    return ops.bor(ops.shl(value, amount), ops.lshr(value, 32 - amount))


def _set_cr_field(state: MachineState, field: int, nibble: Any, ops: WordOps) -> MachineState:
    shift = (7 - field) * 4
    mask = ops.const(0xF << shift)
    value = ops.bor(ops.band(state.cr, ops.bnot(mask)), ops.shl(nibble, shift))
    return state.with_cr(value)


def _comparison_nibble(left: Any, right: Any, signed: bool, state: MachineState, ops: WordOps) -> Any:
    lt = ops.signed_lt(left, right) if signed else ops.unsigned_lt(left, right)
    gt = ops.signed_lt(right, left) if signed else ops.unsigned_lt(right, left)
    return ops.bor(
        ops.bor(ops.ite(lt, ops.const(8), ops.const(0)), ops.ite(gt, ops.const(4), ops.const(0))),
        ops.bor(ops.ite(ops.eq(left, right), ops.const(2), ops.const(0)), ops.ite(state.xer.so, ops.const(1), ops.const(0))),
    )


def _record_cr0(state: MachineState, value: Any, ops: WordOps) -> MachineState:
    return _set_cr_field(state, 0, _comparison_nibble(value, ops.const(0), True, state, ops), ops)


def execute_instruction(state: MachineState, insn: Instruction, ops: WordOps) -> MachineState:
    op = insn.opcode
    operands = insn.operands
    result: Any | None = None
    destination: int | None = None

    if op in (Opcode.ADDI, Opcode.ADDIS, Opcode.MULLI):
        rd, ra, immediate = operands
        base = ops.const(0) if ra == 0 and op in (Opcode.ADDI, Opcode.ADDIS) else state.gpr[ra]
        immediate_value = immediate << 16 if op == Opcode.ADDIS else immediate
        result = ops.mul(base, ops.const(immediate_value)) if op == Opcode.MULLI else ops.add(base, ops.const(immediate_value))
        destination = rd
    elif op in (Opcode.ORI, Opcode.ORIS, Opcode.XORI, Opcode.XORIS, Opcode.ANDI_DOT, Opcode.ANDIS_DOT):
        ra, rs, immediate = operands
        value = immediate << 16 if op in (Opcode.ORIS, Opcode.XORIS, Opcode.ANDIS_DOT) else immediate
        if op in (Opcode.ORI, Opcode.ORIS):
            result = ops.bor(state.gpr[rs], ops.const(value))
        elif op in (Opcode.XORI, Opcode.XORIS):
            result = ops.bxor(state.gpr[rs], ops.const(value))
        else:
            result = ops.band(state.gpr[rs], ops.const(value))
        destination = ra
    elif op in (Opcode.RLWINM, Opcode.RLWIMI):
        ra, rs, sh, mb, me = operands
        mask = ops.const(rotate_mask(mb, me))
        rotated = _rotl(state.gpr[rs], sh, ops)
        result = ops.band(rotated, mask)
        if op == Opcode.RLWIMI:
            result = ops.bor(result, ops.band(state.gpr[ra], ops.bnot(mask)))
        destination = ra
    elif op in (Opcode.AND, Opcode.OR, Opcode.XOR, Opcode.NOR):
        ra, rs, rb = operands
        left, right = state.gpr[rs], state.gpr[rb]
        if op == Opcode.AND:
            result = ops.band(left, right)
        elif op == Opcode.OR:
            result = ops.bor(left, right)
        elif op == Opcode.XOR:
            result = ops.bxor(left, right)
        else:
            result = ops.bnot(ops.bor(left, right))
        destination = ra
    elif op in (Opcode.ADD, Opcode.SUBF):
        rd, ra, rb = operands
        result = ops.add(state.gpr[ra], state.gpr[rb]) if op == Opcode.ADD else ops.sub(state.gpr[rb], state.gpr[ra])
        destination = rd
    elif op in (Opcode.EXTSB, Opcode.EXTSH, Opcode.CNTLZW):
        ra, rs = operands
        source = state.gpr[rs]
        if op == Opcode.EXTSB:
            byte = ops.band(source, ops.const(0xFF))
            result = ops.ite(ops.eq(ops.band(byte, ops.const(0x80)), ops.const(0)), byte, ops.bor(byte, ops.const(0xFFFFFF00)))
        elif op == Opcode.EXTSH:
            half = ops.band(source, ops.const(0xFFFF))
            result = ops.ite(ops.eq(ops.band(half, ops.const(0x8000)), ops.const(0)), half, ops.bor(half, ops.const(0xFFFF0000)))
        else:
            count = ops.const(32)
            for bit in range(32):
                bit_set = ops.eq(ops.band(source, ops.const(1 << (31 - bit))), ops.const(0))
                count = ops.ite(ops.eq(count, ops.const(32)), ops.ite(bit_set, count, ops.const(bit)), count)
            result = count
        destination = ra
    elif op in (Opcode.CMPWI, Opcode.CMPLWI):
        field, ra, immediate = operands
        right = ops.const(immediate)
        return _set_cr_field(state, field, _comparison_nibble(state.gpr[ra], right, op == Opcode.CMPWI, state, ops), ops)
    elif op in (Opcode.CMPW, Opcode.CMPLW):
        field, ra, rb = operands
        return _set_cr_field(state, field, _comparison_nibble(state.gpr[ra], state.gpr[rb], op == Opcode.CMPW, state, ops), ops)
    else:
        raise UnsupportedInstruction(insn.address, insn.raw, f"semantics not implemented for {op.value}")

    assert destination is not None and result is not None
    state = state.with_gpr(destination, result)
    if insn.record:
        state = _record_cr0(state, result, ops)
    return state


def execute_block(state: MachineState, instructions: list[Instruction], ops: WordOps, max_instructions: int = 256) -> MachineState:
    if len(instructions) > max_instructions:
        raise ValueError(f"instruction limit exceeded ({len(instructions)} > {max_instructions})")
    for insn in instructions:
        state = execute_instruction(state, insn, ops)
    return state


def read_gprs(instructions: list[Instruction]) -> set[int]:
    reads: set[int] = set()
    for insn in instructions:
        op = insn.opcode
        a = insn.operands
        if op in (Opcode.ADDI, Opcode.ADDIS):
            if a[1] != 0:
                reads.add(a[1])
        elif op == Opcode.MULLI:
            reads.add(a[1])
        elif op in (Opcode.ORI, Opcode.ORIS, Opcode.XORI, Opcode.XORIS, Opcode.ANDI_DOT, Opcode.ANDIS_DOT):
            reads.add(a[1])
        elif op in (Opcode.RLWINM,):
            reads.add(a[1])
        elif op == Opcode.RLWIMI:
            reads.update((a[0], a[1]))
        elif op in (Opcode.AND, Opcode.OR, Opcode.XOR, Opcode.NOR):
            reads.update((a[1], a[2]))
        elif op in (Opcode.ADD, Opcode.SUBF):
            reads.update((a[1], a[2]))
        elif op in (Opcode.EXTSB, Opcode.EXTSH, Opcode.CNTLZW):
            reads.add(a[1])
        elif op in (Opcode.CMPWI, Opcode.CMPLWI):
            reads.add(a[1])
        elif op in (Opcode.CMPW, Opcode.CMPLW):
            reads.update((a[1], a[2]))
    return reads

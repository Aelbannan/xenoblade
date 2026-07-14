from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any, Protocol

from .ir import ExecutionInconclusive, Instruction, Opcode, UnsupportedInstruction
from .model import ConcreteMemory, MachineState

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
    def shl(self, value: Any, amount: Any) -> Any: ...
    def lshr(self, value: Any, amount: Any) -> Any: ...
    def ashr(self, value: Any, amount: Any) -> Any: ...
    def signed_lt(self, left: Any, right: Any) -> Any: ...
    def unsigned_lt(self, left: Any, right: Any) -> Any: ...
    def eq(self, left: Any, right: Any) -> Any: ...
    def ite(self, condition: Any, yes: Any, no: Any) -> Any: ...
    def land(self, left: Any, right: Any) -> Any: ...
    def lor(self, left: Any, right: Any) -> Any: ...
    def lnot(self, value: Any) -> Any: ...
    def signed_div(self, left: Any, right: Any) -> Any: ...
    def unsigned_div(self, left: Any, right: Any) -> Any: ...
    def mul_high(self, left: Any, right: Any, signed: bool) -> Any: ...
    def add_overflow(self, left: Any, right: Any, carry: Any) -> Any: ...
    def sub_overflow(self, left: Any, right: Any, borrow: Any) -> Any: ...
    def load_byte(self, memory: Any, address: Any) -> Any: ...
    def store_byte(self, memory: Any, address: Any, value: Any) -> Any: ...


@dataclass(frozen=True, slots=True)
class ConcreteOps:
    def const(self, value: int) -> int: return value & MASK32
    def bool(self, value: bool) -> bool: return value
    def add(self, left: int, right: int) -> int: return (left + right) & MASK32
    def sub(self, left: int, right: int) -> int: return (left - right) & MASK32
    def mul(self, left: int, right: int) -> int: return (left * right) & MASK32
    def band(self, left: int, right: int) -> int: return left & right
    def bor(self, left: int, right: int) -> int: return left | right
    def bxor(self, left: int, right: int) -> int: return left ^ right
    def bnot(self, value: int) -> int: return (~value) & MASK32
    def shl(self, value: int, amount: int) -> int: return (value << amount) & MASK32
    def lshr(self, value: int, amount: int) -> int: return (value & MASK32) >> amount
    def ashr(self, value: int, amount: int) -> int:
        signed = value - 0x100000000 if value & 0x80000000 else value
        return (signed >> amount) & MASK32
    def signed_lt(self, left: int, right: int) -> bool:
        signed = lambda value: value - 0x100000000 if value & 0x80000000 else value
        return signed(left) < signed(right)
    def unsigned_lt(self, left: int, right: int) -> bool: return (left & MASK32) < (right & MASK32)
    def eq(self, left: int, right: int) -> bool: return (left & MASK32) == (right & MASK32)
    def ite(self, condition: bool, yes: Any, no: Any) -> Any: return yes if condition else no
    def land(self, left: bool, right: bool) -> bool: return left and right
    def lor(self, left: bool, right: bool) -> bool: return left or right
    def lnot(self, value: bool) -> bool: return not value
    def signed_div(self, left: int, right: int) -> int:
        a = left - 0x100000000 if left & 0x80000000 else left
        b = right - 0x100000000 if right & 0x80000000 else right
        if b == 0 or (a == -0x80000000 and b == -1): return 0
        return (abs(a) // abs(b) * (-1 if (a < 0) != (b < 0) else 1)) & MASK32
    def unsigned_div(self, left: int, right: int) -> int:
        return 0 if (right & MASK32) == 0 else (left & MASK32) // (right & MASK32)
    def mul_high(self, left: int, right: int, signed: bool) -> int:
        if signed:
            left = left - 0x100000000 if left & 0x80000000 else left
            right = right - 0x100000000 if right & 0x80000000 else right
        return ((left * right) >> 32) & MASK32
    def add_overflow(self, left: int, right: int, carry: int) -> bool:
        signed = lambda value: value - 0x100000000 if value & 0x80000000 else value
        total = signed(left) + signed(right) + carry
        return total < -0x80000000 or total > 0x7FFFFFFF
    def sub_overflow(self, left: int, right: int, borrow: int) -> bool:
        signed = lambda value: value - 0x100000000 if value & 0x80000000 else value
        total = signed(left) - signed(right) - borrow
        return total < -0x80000000 or total > 0x7FFFFFFF
    def load_byte(self, memory: ConcreteMemory, address: int) -> int: return memory.read(address)
    def store_byte(self, memory: ConcreteMemory, address: int, value: int) -> ConcreteMemory:
        return memory.write(address, value)


class SymbolicOps:
    def __init__(self) -> None:
        try:
            import z3
        except ImportError as exc:
            raise RuntimeError("z3-solver is required; install tools/ppc_equivalence/requirements.txt") from exc
        self.z3 = z3

    def const(self, value: int) -> Any: return self.z3.BitVecVal(value & MASK32, 32)
    def bool(self, value: bool) -> Any: return self.z3.BoolVal(value)
    def add(self, left: Any, right: Any) -> Any: return left + right
    def sub(self, left: Any, right: Any) -> Any: return left - right
    def mul(self, left: Any, right: Any) -> Any: return left * right
    def band(self, left: Any, right: Any) -> Any: return left & right
    def bor(self, left: Any, right: Any) -> Any: return left | right
    def bxor(self, left: Any, right: Any) -> Any: return left ^ right
    def bnot(self, value: Any) -> Any: return ~value
    def shl(self, value: Any, amount: Any) -> Any: return value << amount
    def lshr(self, value: Any, amount: Any) -> Any: return self.z3.LShR(value, amount)
    def ashr(self, value: Any, amount: Any) -> Any: return value >> amount
    def signed_lt(self, left: Any, right: Any) -> Any: return left < right
    def unsigned_lt(self, left: Any, right: Any) -> Any: return self.z3.ULT(left, right)
    def eq(self, left: Any, right: Any) -> Any: return left == right
    def ite(self, condition: Any, yes: Any, no: Any) -> Any: return self.z3.If(condition, yes, no)
    def land(self, left: Any, right: Any) -> Any: return self.z3.And(left, right)
    def lor(self, left: Any, right: Any) -> Any: return self.z3.Or(left, right)
    def lnot(self, value: Any) -> Any: return self.z3.Not(value)
    def signed_div(self, left: Any, right: Any) -> Any: return left / right
    def unsigned_div(self, left: Any, right: Any) -> Any: return self.z3.UDiv(left, right)
    def mul_high(self, left: Any, right: Any, signed: bool) -> Any:
        a = self.z3.SignExt(32, left) if signed else self.z3.ZeroExt(32, left)
        b = self.z3.SignExt(32, right) if signed else self.z3.ZeroExt(32, right)
        return self.z3.Extract(63, 32, a * b)
    def add_overflow(self, left: Any, right: Any, carry: Any) -> Any:
        total = self.z3.SignExt(32, left) + self.z3.SignExt(32, right) + self.z3.ZeroExt(32, carry)
        return self.z3.Or(total < self.z3.BitVecVal(-0x80000000, 64), total > self.z3.BitVecVal(0x7FFFFFFF, 64))
    def sub_overflow(self, left: Any, right: Any, borrow: Any) -> Any:
        total = self.z3.SignExt(32, left) - self.z3.SignExt(32, right) - self.z3.ZeroExt(32, borrow)
        return self.z3.Or(total < self.z3.BitVecVal(-0x80000000, 64), total > self.z3.BitVecVal(0x7FFFFFFF, 64))
    def load_byte(self, memory: Any, address: Any) -> Any: return self.z3.ZeroExt(24, self.z3.Select(memory, address))
    def store_byte(self, memory: Any, address: Any, value: Any) -> Any:
        return self.z3.Store(memory, address, self.z3.Extract(7, 0, value))


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


def _rotl(value: Any, amount: Any, ops: WordOps) -> Any:
    amount32 = ops.band(amount, ops.const(31))
    inverse = ops.band(ops.sub(ops.const(32), amount32), ops.const(31))
    return ops.bor(ops.shl(value, amount32), ops.lshr(value, inverse))


def _set_cr_field(state: MachineState, field: int, nibble: Any, ops: WordOps) -> MachineState:
    shift = (7 - field) * 4
    mask = ops.const(0xF << shift)
    return state.with_cr(ops.bor(ops.band(state.cr, ops.bnot(mask)), ops.shl(nibble, ops.const(shift))))


def _comparison_nibble(left: Any, right: Any, signed: bool, state: MachineState, ops: WordOps) -> Any:
    lt = ops.signed_lt(left, right) if signed else ops.unsigned_lt(left, right)
    gt = ops.signed_lt(right, left) if signed else ops.unsigned_lt(right, left)
    return ops.bor(
        ops.bor(ops.ite(lt, ops.const(8), ops.const(0)), ops.ite(gt, ops.const(4), ops.const(0))),
        ops.bor(ops.ite(ops.eq(left, right), ops.const(2), ops.const(0)), ops.ite(state.xer.so, ops.const(1), ops.const(0))),
    )


def _record_cr0(state: MachineState, value: Any, ops: WordOps) -> MachineState:
    return _set_cr_field(state, 0, _comparison_nibble(value, ops.const(0), True, state, ops), ops)


def _carry_add(left: Any, right: Any, result: Any, ops: WordOps) -> Any:
    return ops.unsigned_lt(result, left)


def _signed_overflow_add(left: Any, right: Any, result: Any, ops: WordOps) -> Any:
    sign = ops.const(0x80000000)
    return ops.lnot(ops.eq(ops.band(ops.band(ops.bnot(ops.bxor(left, right)), ops.bxor(left, result)), sign), ops.const(0)))


def _signed_overflow_sub(left: Any, right: Any, result: Any, ops: WordOps) -> Any:
    # result = left - right
    sign = ops.const(0x80000000)
    return ops.lnot(ops.eq(ops.band(ops.band(ops.bxor(left, right), ops.bxor(left, result)), sign), ops.const(0)))


def _apply_oe(state: MachineState, overflow: Any, enabled: bool, ops: WordOps) -> MachineState:
    if not enabled:
        return state
    return state.with_xer(ov=overflow, so=ops.lor(state.xer.so, overflow))


def _load(memory: Any, address: Any, width: int, ops: WordOps, *, reverse: bool = False) -> Any:
    result = ops.const(0)
    order = list(range(width))
    if reverse:
        order.reverse()
    for offset in order:
        result = ops.bor(ops.shl(result, ops.const(8)), ops.load_byte(memory, ops.add(address, ops.const(offset))))
    return result


def _store(memory: Any, address: Any, value: Any, width: int, ops: WordOps, *, reverse: bool = False) -> Any:
    result = memory
    for offset in range(width):
        shift = offset * 8 if reverse else (width - 1 - offset) * 8
        byte = ops.band(ops.lshr(value, ops.const(shift)), ops.const(0xFF))
        result = ops.store_byte(result, ops.add(address, ops.const(offset)), byte)
    return result


def _sign_extend(value: Any, bits: int, ops: WordOps) -> Any:
    low_mask = (1 << bits) - 1
    low = ops.band(value, ops.const(low_mask))
    return ops.ite(ops.eq(ops.band(low, ops.const(1 << (bits - 1))), ops.const(0)), low, ops.bor(low, ops.const(~low_mask)))


def _touch_memory(state: MachineState, address: Any, width: int, ops: WordOps) -> MachineState:
    if width > 1:
        aligned = ops.eq(ops.band(address, ops.const(width - 1)), ops.const(0))
        state = replace(state, valid=ops.land(state.valid, aligned))
    touches = state.memory_touches + tuple(ops.add(address, ops.const(offset)) for offset in range(width))
    return replace(state, memory_touches=touches)


LOADS = {
    Opcode.LBZ: (1, False, False, False), Opcode.LBZU: (1, False, True, False),
    Opcode.LHZ: (2, False, False, False), Opcode.LHZU: (2, False, True, False),
    Opcode.LHA: (2, True, False, False), Opcode.LHAU: (2, True, True, False),
    Opcode.LWZ: (4, False, False, False), Opcode.LWZU: (4, False, True, False),
    Opcode.LBZX: (1, False, False, False), Opcode.LBZUX: (1, False, True, False),
    Opcode.LHZX: (2, False, False, False), Opcode.LHZUX: (2, False, True, False),
    Opcode.LHAX: (2, True, False, False), Opcode.LHAUX: (2, True, True, False),
    Opcode.LWZX: (4, False, False, False), Opcode.LWZUX: (4, False, True, False),
    Opcode.LHBRX: (2, False, False, True), Opcode.LWBRX: (4, False, False, True),
}
STORES = {
    Opcode.STB: (1, False, False), Opcode.STBU: (1, True, False),
    Opcode.STH: (2, False, False), Opcode.STHU: (2, True, False),
    Opcode.STW: (4, False, False), Opcode.STWU: (4, True, False),
    Opcode.STBX: (1, False, False), Opcode.STBUX: (1, True, False),
    Opcode.STHX: (2, False, False), Opcode.STHUX: (2, True, False),
    Opcode.STWX: (4, False, False), Opcode.STWUX: (4, True, False),
    Opcode.STHBRX: (2, False, True), Opcode.STWBRX: (4, False, True),
}
INDEXED_MEMORY = {
    Opcode.LBZX, Opcode.LBZUX, Opcode.LHZX, Opcode.LHZUX, Opcode.LHAX, Opcode.LHAUX,
    Opcode.LWZX, Opcode.LWZUX, Opcode.LHBRX, Opcode.LWBRX, Opcode.STBX, Opcode.STBUX,
    Opcode.STHX, Opcode.STHUX, Opcode.STWX, Opcode.STWUX, Opcode.STHBRX, Opcode.STWBRX,
}


def execute_instruction(state: MachineState, insn: Instruction, ops: WordOps) -> MachineState:
    op, a = insn.opcode, insn.operands
    result: Any | None = None
    destination: int | None = None
    overflow = ops.bool(False)

    if op in (Opcode.ADDI, Opcode.ADDIS, Opcode.MULLI, Opcode.ADDIC, Opcode.ADDIC_DOT, Opcode.SUBFIC):
        rd, ra, immediate = a
        base = ops.const(0) if ra == 0 and op in (Opcode.ADDI, Opcode.ADDIS) else state.gpr[ra]
        immediate_value = immediate << 16 if op == Opcode.ADDIS else immediate
        right = ops.const(immediate_value)
        if op == Opcode.MULLI:
            result = ops.mul(base, right)
        elif op == Opcode.SUBFIC:
            result = ops.sub(right, base)
            state = state.with_xer(ca=ops.lnot(ops.unsigned_lt(right, base)))
        else:
            result = ops.add(base, right)
            if op in (Opcode.ADDIC, Opcode.ADDIC_DOT):
                state = state.with_xer(ca=_carry_add(base, right, result, ops))
        destination = rd
    elif op in (Opcode.ORI, Opcode.ORIS, Opcode.XORI, Opcode.XORIS, Opcode.ANDI_DOT, Opcode.ANDIS_DOT):
        ra, rs, immediate = a
        value = immediate << 16 if op in (Opcode.ORIS, Opcode.XORIS, Opcode.ANDIS_DOT) else immediate
        if op in (Opcode.ORI, Opcode.ORIS): result = ops.bor(state.gpr[rs], ops.const(value))
        elif op in (Opcode.XORI, Opcode.XORIS): result = ops.bxor(state.gpr[rs], ops.const(value))
        else: result = ops.band(state.gpr[rs], ops.const(value))
        destination = ra
    elif op in (Opcode.RLWINM, Opcode.RLWIMI, Opcode.RLWNM):
        if op == Opcode.RLWNM:
            ra, rs, rb, mb, me = a
            amount = state.gpr[rb]
        else:
            ra, rs, shift, mb, me = a
            amount = ops.const(shift)
        mask = ops.const(rotate_mask(mb, me))
        result = ops.band(_rotl(state.gpr[rs], amount, ops), mask)
        if op == Opcode.RLWIMI:
            result = ops.bor(result, ops.band(state.gpr[ra], ops.bnot(mask)))
        destination = ra
    elif op in (Opcode.AND, Opcode.ANDC, Opcode.EQV, Opcode.NAND, Opcode.NOR, Opcode.OR, Opcode.ORC, Opcode.XOR):
        ra, rs, rb = a
        left, right = state.gpr[rs], state.gpr[rb]
        if op == Opcode.AND: result = ops.band(left, right)
        elif op == Opcode.ANDC: result = ops.band(left, ops.bnot(right))
        elif op == Opcode.EQV: result = ops.bnot(ops.bxor(left, right))
        elif op == Opcode.NAND: result = ops.bnot(ops.band(left, right))
        elif op == Opcode.NOR: result = ops.bnot(ops.bor(left, right))
        elif op == Opcode.OR: result = ops.bor(left, right)
        elif op == Opcode.ORC: result = ops.bor(left, ops.bnot(right))
        else: result = ops.bxor(left, right)
        destination = ra
    elif op in (Opcode.SLW, Opcode.SRW, Opcode.SRAW, Opcode.SRAWI):
        ra, rs, rb_or_shift = a
        source = state.gpr[rs]
        amount6 = ops.const(rb_or_shift) if op == Opcode.SRAWI else ops.band(state.gpr[rb_or_shift], ops.const(0x3F))
        amount5 = ops.band(amount6, ops.const(31))
        large = ops.lnot(ops.eq(ops.band(amount6, ops.const(32)), ops.const(0)))
        if op == Opcode.SLW:
            result = ops.ite(large, ops.const(0), ops.shl(source, amount5))
        elif op == Opcode.SRW:
            result = ops.ite(large, ops.const(0), ops.lshr(source, amount5))
        else:
            sign_fill = ops.ite(ops.signed_lt(source, ops.const(0)), ops.const(MASK32), ops.const(0))
            result = ops.ite(large, sign_fill, ops.ashr(source, amount5))
            shifted_mask = ops.sub(ops.shl(ops.const(1), amount5), ops.const(1))
            lost = ops.ite(large, source, ops.band(source, shifted_mask))
            ca = ops.land(ops.signed_lt(source, ops.const(0)), ops.lnot(ops.eq(lost, ops.const(0))))
            state = state.with_xer(ca=ca)
        destination = ra
    elif op in (Opcode.ADD, Opcode.ADDC, Opcode.ADDE, Opcode.ADDME, Opcode.ADDZE,
                 Opcode.SUBF, Opcode.SUBFC, Opcode.SUBFE, Opcode.SUBFME, Opcode.SUBFZE, Opcode.NEG):
        rd, ra = a[0], a[1]
        left = state.gpr[ra]
        carry_in = ops.ite(state.xer.ca, ops.const(1), ops.const(0))
        if op in (Opcode.ADD, Opcode.ADDC):
            right = state.gpr[a[2]]
            result = ops.add(left, right)
            carry = _carry_add(left, right, result, ops)
            overflow = ops.add_overflow(left, right, ops.const(0))
        elif op == Opcode.ADDE:
            right = state.gpr[a[2]]
            partial = ops.add(left, right)
            result = ops.add(partial, carry_in)
            carry = ops.lor(_carry_add(left, right, partial, ops), _carry_add(partial, carry_in, result, ops))
            overflow = ops.add_overflow(left, right, carry_in)
        elif op in (Opcode.ADDME, Opcode.ADDZE):
            right = ops.const(MASK32 if op == Opcode.ADDME else 0)
            partial = ops.add(left, right)
            result = ops.add(partial, carry_in)
            carry = ops.lor(_carry_add(left, right, partial, ops), _carry_add(partial, carry_in, result, ops))
            overflow = ops.add_overflow(left, right, carry_in)
        elif op == Opcode.NEG:
            result = ops.sub(ops.const(0), left)
            carry = state.xer.ca
            overflow = ops.eq(left, ops.const(0x80000000))
        else:
            minuend = state.gpr[a[2]] if len(a) == 3 else ops.const(MASK32 if op == Opcode.SUBFME else 0)
            if op in (Opcode.SUBFE, Opcode.SUBFME, Opcode.SUBFZE):
                inverted = ops.bnot(left)
                partial = ops.add(inverted, minuend)
                result = ops.add(partial, carry_in)
                carry = ops.lor(_carry_add(inverted, minuend, partial, ops), _carry_add(partial, carry_in, result, ops))
            else:
                result = ops.sub(minuend, left)
                carry = ops.lnot(ops.unsigned_lt(minuend, left))
            borrow = ops.sub(ops.const(1), carry_in) if op in (Opcode.SUBFE, Opcode.SUBFME, Opcode.SUBFZE) else ops.const(0)
            overflow = ops.sub_overflow(minuend, left, borrow)
        if op in (Opcode.ADDC, Opcode.ADDE, Opcode.ADDME, Opcode.ADDZE, Opcode.SUBFC, Opcode.SUBFE, Opcode.SUBFME, Opcode.SUBFZE):
            state = state.with_xer(ca=carry)
        state = _apply_oe(state, overflow, insn.overflow, ops)
        destination = rd
    elif op in (Opcode.MULHW, Opcode.MULHWU, Opcode.MULLW, Opcode.DIVW, Opcode.DIVWU):
        rd, ra, rb = a
        left, right = state.gpr[ra], state.gpr[rb]
        if op in (Opcode.MULHW, Opcode.MULHWU): result = ops.mul_high(left, right, op == Opcode.MULHW)
        elif op == Opcode.MULLW:
            result = ops.mul(left, right)
            if insn.overflow:
                high = ops.mul_high(left, right, True)
                overflow = ops.lnot(ops.eq(high, ops.ite(ops.signed_lt(result, ops.const(0)), ops.const(MASK32), ops.const(0))))
        else:
            zero = ops.eq(right, ops.const(0))
            signed_overflow = ops.land(ops.eq(left, ops.const(0x80000000)), ops.eq(right, ops.const(MASK32)))
            valid = ops.lnot(ops.lor(zero, signed_overflow if op == Opcode.DIVW else ops.bool(False)))
            state = replace(state, valid=ops.land(state.valid, valid))
            result = ops.ite(valid, ops.signed_div(left, right) if op == Opcode.DIVW else ops.unsigned_div(left, right), ops.const(0))
            overflow = ops.lnot(valid)
        state = _apply_oe(state, overflow, insn.overflow, ops)
        destination = rd
    elif op in (Opcode.EXTSB, Opcode.EXTSH, Opcode.CNTLZW):
        ra, rs = a
        source = state.gpr[rs]
        if op == Opcode.EXTSB: result = _sign_extend(source, 8, ops)
        elif op == Opcode.EXTSH: result = _sign_extend(source, 16, ops)
        else:
            result = ops.const(32)
            for bit in range(32):
                unset = ops.eq(ops.band(source, ops.const(1 << (31 - bit))), ops.const(0))
                result = ops.ite(ops.eq(result, ops.const(32)), ops.ite(unset, result, ops.const(bit)), result)
        destination = ra
    elif op in (Opcode.CMPWI, Opcode.CMPLWI):
        field, ra, immediate = a
        return _set_cr_field(state, field, _comparison_nibble(state.gpr[ra], ops.const(immediate), op == Opcode.CMPWI, state, ops), ops)
    elif op in (Opcode.CMPW, Opcode.CMPLW):
        field, ra, rb = a
        return _set_cr_field(state, field, _comparison_nibble(state.gpr[ra], state.gpr[rb], op == Opcode.CMPW, state, ops), ops)
    elif op in (Opcode.CRAND, Opcode.CRANDC, Opcode.CREQV, Opcode.CRNAND, Opcode.CRNOR, Opcode.CROR, Opcode.CRORC, Opcode.CRXOR):
        bt, ba, bb = a
        get = lambda bit: ops.lnot(ops.eq(ops.band(state.cr, ops.const(1 << (31 - bit))), ops.const(0)))
        left, right = get(ba), get(bb)
        if op == Opcode.CRAND: value = ops.land(left, right)
        elif op == Opcode.CRANDC: value = ops.land(left, ops.lnot(right))
        elif op == Opcode.CREQV: value = ops.lnot(ops.lor(ops.land(left, ops.lnot(right)), ops.land(ops.lnot(left), right)))
        elif op == Opcode.CRNAND: value = ops.lnot(ops.land(left, right))
        elif op == Opcode.CRNOR: value = ops.lnot(ops.lor(left, right))
        elif op == Opcode.CROR: value = ops.lor(left, right)
        elif op == Opcode.CRORC: value = ops.lor(left, ops.lnot(right))
        else: value = ops.lor(ops.land(left, ops.lnot(right)), ops.land(ops.lnot(left), right))
        mask = ops.const(1 << (31 - bt))
        return state.with_cr(ops.ite(value, ops.bor(state.cr, mask), ops.band(state.cr, ops.bnot(mask))))
    elif op == Opcode.MFCR:
        destination, result = a[0], state.cr
    elif op == Opcode.MTCRF:
        rs, fxm = a
        mask_value = 0
        for field in range(8):
            if fxm & (1 << (7 - field)): mask_value |= 0xF << ((7 - field) * 4)
        mask = ops.const(mask_value)
        return state.with_cr(ops.bor(ops.band(state.cr, ops.bnot(mask)), ops.band(state.gpr[rs], mask)))
    elif op in (Opcode.MFSPR, Opcode.MTSPR):
        reg, spr = a
        if spr == 1:
            packed = ops.bor(ops.ite(state.xer.so, ops.const(1 << 31), ops.const(0)), ops.bor(ops.ite(state.xer.ov, ops.const(1 << 30), ops.const(0)), ops.ite(state.xer.ca, ops.const(1 << 29), ops.const(0))))
            if op == Opcode.MFSPR: destination, result = reg, packed
            else:
                source = state.gpr[reg]
                return state.with_xer(
                    so=ops.lnot(ops.eq(ops.band(source, ops.const(1 << 31)), ops.const(0))),
                    ov=ops.lnot(ops.eq(ops.band(source, ops.const(1 << 30)), ops.const(0))),
                    ca=ops.lnot(ops.eq(ops.band(source, ops.const(1 << 29)), ops.const(0))),
                )
        elif spr == 8:
            if op == Opcode.MFSPR: destination, result = reg, state.lr
            else: return replace(state, lr=state.gpr[reg])
        else:
            if op == Opcode.MFSPR: destination, result = reg, state.ctr
            else: return replace(state, ctr=state.gpr[reg])
    elif op in LOADS or op in STORES:
        reg, ra, third = a
        if op in INDEXED_MEMORY:
            address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], state.gpr[third])
        else:
            address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], ops.const(third))
        if op in LOADS:
            width, signed, update, reverse_bytes = LOADS[op]
            state = _touch_memory(state, address, width, ops)
            result = _load(state.memory, address, width, ops, reverse=reverse_bytes)
            if signed: result = _sign_extend(result, width * 8, ops)
            state = state.with_gpr(reg, result)
            if update: state = state.with_gpr(ra, address)
            return state
        width, update, reverse_bytes = STORES[op]
        state = _touch_memory(state, address, width, ops)
        state = replace(state, memory=_store(state.memory, address, state.gpr[reg], width, ops, reverse=reverse_bytes))
        if update: state = state.with_gpr(ra, address)
        return state
    elif op in (Opcode.LMW, Opcode.STMW):
        reg, ra, displacement = a
        address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], ops.const(displacement))
        for index in range(reg, 32):
            state = _touch_memory(state, address, 4, ops)
            if op == Opcode.LMW:
                state = state.with_gpr(index, _load(state.memory, address, 4, ops))
            else:
                state = replace(state, memory=_store(state.memory, address, state.gpr[index], 4, ops))
            address = ops.add(address, ops.const(4))
        return state
    elif op in (Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR):
        raise AssertionError("control-flow instruction passed to execute_instruction")
    else:
        raise UnsupportedInstruction(insn.address, insn.raw, f"semantics not implemented for {op.value}")

    assert destination is not None and result is not None
    state = state.with_gpr(destination, result)
    if insn.record or op == Opcode.ADDIC_DOT:
        state = _record_cr0(state, result, ops)
    return state


@dataclass(frozen=True, slots=True)
class Terminal:
    condition: Any
    state: MachineState
    exit_kind: str
    exit_target: Any | None = None


def _branch_condition(state: MachineState, bo: int, bi: int, ops: WordOps, *, allow_ctr: bool) -> tuple[Any, MachineState]:
    bo0, bo1, bo2, bo3 = bool(bo & 16), bool(bo & 8), bool(bo & 4), bool(bo & 2)
    if not bo2 and allow_ctr:
        state = replace(state, ctr=ops.sub(state.ctr, ops.const(1)))
        ctr_nonzero = ops.lnot(ops.eq(state.ctr, ops.const(0)))
        ctr_ok = ctr_nonzero if not bo3 else ops.lnot(ctr_nonzero)
    else:
        ctr_ok = ops.bool(True)
    cr_set = ops.lnot(ops.eq(ops.band(state.cr, ops.const(1 << (31 - bi))), ops.const(0)))
    cond_ok = ops.bool(True) if bo0 else (cr_set if bo1 else ops.lnot(cr_set))
    return ops.land(ctr_ok, cond_ok), state


def execute_cfg(
    state: MachineState,
    instructions: list[Instruction],
    ops: WordOps,
    *,
    max_instructions: int = 512,
    max_paths: int = 128,
) -> list[Terminal]:
    if not instructions:
        raise ValueError("cannot execute an empty block")
    by_address = {item.address: item for item in instructions}
    start = instructions[0].address
    end = instructions[-1].address + 4
    work: list[tuple[int, MachineState, Any, frozenset[int], int]] = [(start, state, ops.bool(True), frozenset(), 0)]
    terminals: list[Terminal] = []
    while work:
        pc, current, condition, visited, steps = work.pop()
        if steps >= max_instructions:
            raise ExecutionInconclusive(f"instruction limit exceeded ({max_instructions})")
        if pc == end:
            terminals.append(Terminal(condition, current, "fallthrough"))
            continue
        insn = by_address.get(pc)
        if insn is None:
            terminals.append(Terminal(condition, current, "direct-branch", ops.const(pc)))
            continue
        if pc in visited:
            raise ExecutionInconclusive(f"loop/back-edge encountered at 0x{pc:08x}")
        new_visited = visited | {pc}
        if insn.opcode not in (Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR):
            work.append((pc + 4, execute_instruction(current, insn, ops), condition, new_visited, steps + 1))
            continue

        old_lr, old_ctr = current.lr, current.ctr
        linked = replace(current, lr=ops.const(pc + 4)) if insn.link else current
        if insn.opcode == Opcode.B:
            target = insn.operands[0]
            if target in by_address or target == end:
                work.append((target, linked, condition, new_visited, steps + 1))
            else:
                terminals.append(Terminal(condition, linked, "call" if insn.link else "direct-branch", ops.const(target)))
            continue

        bo, bi = insn.operands[:2]
        predicate, branched_state = _branch_condition(linked, bo, bi, ops, allow_ctr=insn.opcode != Opcode.BCCTR)
        taken_condition = ops.land(condition, predicate)
        fall_condition = ops.land(condition, ops.lnot(predicate))
        if insn.opcode == Opcode.BC:
            target: Any = insn.operands[2]
            kind = "call" if insn.link else "direct-branch"
        elif insn.opcode == Opcode.BCLR:
            target = ops.band(old_lr, ops.const(0xFFFFFFFC))
            kind = "call-indirect" if insn.link else "return"
        else:
            target = ops.band(old_ctr, ops.const(0xFFFFFFFC))
            kind = "call-indirect" if insn.link else "indirect-branch"

        if isinstance(target, int) and (target in by_address or target == end):
            work.append((target, branched_state, taken_condition, new_visited, steps + 1))
        else:
            terminals.append(Terminal(taken_condition, branched_state, kind, ops.const(target) if isinstance(target, int) else target))
        work.append((pc + 4, branched_state, fall_condition, new_visited, steps + 1))
        if len(work) + len(terminals) > max_paths:
            raise ExecutionInconclusive(f"path limit exceeded ({max_paths})")
    return terminals


def execute_block(state: MachineState, instructions: list[Instruction], ops: WordOps, max_instructions: int = 512) -> MachineState:
    terminals = execute_cfg(state, instructions, ops, max_instructions=max_instructions)
    if len(terminals) != 1 or terminals[0].exit_kind != "fallthrough":
        raise ExecutionInconclusive("concrete replay has multiple paths or a non-fallthrough exit")
    return terminals[0].state


def register_effects(insn: Instruction) -> tuple[set[str], set[str]]:
    """Conservative architectural read/write sets used by live-out analysis."""
    reads: set[str] = set()
    writes: set[str] = set()
    op, a = insn.opcode, insn.operands
    # GPR effects are deliberately conservative; false liveness only makes proofs stricter.
    for index in range(32):
        marker = f"r{index}"
        if marker in str(a):
            pass
    if op in LOADS:
        writes.add(f"r{a[0]}")
        if a[1]: reads.add(f"r{a[1]}")
        if op in INDEXED_MEMORY: reads.add(f"r{a[2]}")
        if LOADS[op][2]: writes.add(f"r{a[1]}")
        reads.add("memory")
    elif op in STORES:
        reads.add(f"r{a[0]}")
        if a[1]: reads.add(f"r{a[1]}")
        if op in INDEXED_MEMORY: reads.add(f"r{a[2]}")
        if STORES[op][1]: writes.add(f"r{a[1]}")
        writes.add("memory")
    elif op in (Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR):
        if op != Opcode.B: reads.add("cr")
        if op != Opcode.B and not (a[0] & 4):
            reads.add("ctr")
            writes.add("ctr")
        if op == Opcode.BCLR: reads.add("lr")
        if op == Opcode.BCCTR: reads.add("ctr")
        if insn.link: writes.add("lr")
    else:
        # A full operand-role table is less useful than the decoder/semantics source of truth;
        # all operand GPRs are conservatively live-in and the conventional destination is killed.
        for value in a:
            if 0 <= value < 32: reads.add(f"r{value}")
        destination_ops = set(LOADS) | {
            Opcode.ADDI, Opcode.ADDIS, Opcode.ADDIC, Opcode.ADDIC_DOT, Opcode.SUBFIC, Opcode.MULLI,
            Opcode.RLWINM, Opcode.RLWIMI, Opcode.RLWNM, Opcode.AND, Opcode.ANDC, Opcode.EQV,
            Opcode.NAND, Opcode.NOR, Opcode.OR, Opcode.ORC, Opcode.XOR, Opcode.SLW, Opcode.SRW,
            Opcode.SRAW, Opcode.SRAWI, Opcode.ADD, Opcode.ADDC, Opcode.ADDE, Opcode.ADDME,
            Opcode.ADDZE, Opcode.SUBF, Opcode.SUBFC, Opcode.SUBFE, Opcode.SUBFME, Opcode.SUBFZE,
            Opcode.NEG, Opcode.MULHW, Opcode.MULHWU, Opcode.MULLW, Opcode.DIVW, Opcode.DIVWU,
            Opcode.EXTSB, Opcode.EXTSH, Opcode.CNTLZW, Opcode.MFCR, Opcode.MFSPR,
        }
        if op in destination_ops: writes.add(f"r{a[0]}")
        if insn.record or op in (Opcode.ADDIC_DOT, Opcode.ANDI_DOT, Opcode.ANDIS_DOT): writes.add("cr0")
    return reads, writes


def automatic_live_out(instructions: list[Instruction]) -> tuple[str, ...]:
    """Conservative live-out over-approximation: every architectural component written."""
    written: set[str] = set()
    for insn in instructions:
        op, a = insn.opcode, insn.operands
        if op in LOADS:
            written.add(f"r{a[0]}")
            if LOADS[op][2]: written.add(f"r{a[1]}")
        elif op in STORES:
            written.add("memory")
            if STORES[op][1]: written.add(f"r{a[1]}")
        elif op == Opcode.LMW:
            written |= {f"r{i}" for i in range(a[0], 32)}
        elif op == Opcode.STMW:
            written.add("memory")
        elif op in (Opcode.CMPWI, Opcode.CMPLWI, Opcode.CMPW, Opcode.CMPLW):
            written.add(f"cr{a[0]}")
        elif op in (Opcode.CRAND, Opcode.CRANDC, Opcode.CREQV, Opcode.CRNAND, Opcode.CRNOR, Opcode.CROR, Opcode.CRORC, Opcode.CRXOR, Opcode.MTCRF):
            written.add("cr")
        elif op == Opcode.MTSPR:
            if a[1] == 1: written |= {"xer.ca", "xer.ov", "xer.so"}
            elif a[1] == 8: written.add("lr")
            elif a[1] == 9: written.add("ctr")
        elif op in (Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR):
            if insn.link: written.add("lr")
            if op in (Opcode.BC, Opcode.BCLR) and not (a[0] & 4): written.add("ctr")
        else:
            written.add(f"r{a[0]}")
            if op in (Opcode.ADDIC, Opcode.ADDIC_DOT, Opcode.SUBFIC, Opcode.ADDC, Opcode.ADDE, Opcode.ADDME, Opcode.ADDZE, Opcode.SUBFC, Opcode.SUBFE, Opcode.SUBFME, Opcode.SUBFZE, Opcode.SRAW, Opcode.SRAWI):
                written.add("xer.ca")
            if insn.overflow:
                written |= {"xer.ov", "xer.so"}
        if insn.record or op in (Opcode.ADDIC_DOT, Opcode.ANDI_DOT, Opcode.ANDIS_DOT):
            written.add("cr0")
    order = [*(f"r{i}" for i in range(32)), "cr", *(f"cr{i}" for i in range(8)), "xer.ca", "xer.ov", "xer.so", "lr", "ctr", "memory"]
    return tuple(item for item in order if item in written)


def read_gprs(instructions: list[Instruction]) -> set[int]:
    result: set[int] = set()
    for insn in instructions:
        reads, _ = register_effects(insn)
        result |= {int(name[1:]) for name in reads if name.startswith("r") and name[1:].isdigit()}
    return result

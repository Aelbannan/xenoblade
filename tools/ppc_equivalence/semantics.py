from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any, Protocol

from .ir import ExecutionInconclusive, Instruction, Opcode, SUPPORTED_OPCODES, UnsupportedInstruction
from .model import ConcreteMemory, MachineState

MASK32 = 0xFFFFFFFF

# FPSCR numeric masks (Gekko.h UReg_FPSCR bitfield layout).
FPSCR_FX = 1 << 31
FPSCR_FEX = 1 << 30
FPSCR_VX = 1 << 29
FPSCR_OX = 1 << 28
FPSCR_UX = 1 << 27
FPSCR_ZX = 1 << 26
FPSCR_XX = 1 << 25
FPSCR_VXSNAN = 1 << 24
FPSCR_VXISI = 1 << 23
FPSCR_VXIDI = 1 << 22
FPSCR_VXZDZ = 1 << 21
FPSCR_VXIMZ = 1 << 20
FPSCR_VXVC = 1 << 19
FPSCR_FR = 1 << 18
FPSCR_FI = 1 << 17
FPSCR_FPRF_MASK = 0x1F << 12
FPSCR_VXSOFT = 1 << 10
FPSCR_VXSQRT = 1 << 9
FPSCR_VXCVI = 1 << 8
FPSCR_VE = 1 << 7
FPSCR_OE = 1 << 6
FPSCR_UE = 1 << 5
FPSCR_ZE = 1 << 4
FPSCR_XE = 1 << 3
FPSCR_VX_ANY = (
    FPSCR_VXSNAN | FPSCR_VXISI | FPSCR_VXIDI | FPSCR_VXZDZ | FPSCR_VXIMZ
    | FPSCR_VXVC | FPSCR_VXSOFT | FPSCR_VXSQRT | FPSCR_VXCVI
)
FPSCR_ANY_ENABLE = FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE


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

    # Floating-point helpers — FPRs are stored as 64-bit bitvectors internally.
    def fp_const64(self, value: int) -> Any: ...
    def fp_low_word(self, bits: Any) -> Any: ...
    def fp_high_word(self, bits: Any) -> Any: ...
    def fp_join_words(self, high: Any, low: Any) -> Any: ...
    def fp_xor_sign(self, bits: Any) -> Any: ...
    def fp_clear_sign(self, bits: Any) -> Any: ...
    def fp_set_sign(self, bits: Any) -> Any: ...
    def fp_fprf(self, bits: Any) -> Any: ...
    def fp_force_25bit(self, bits: Any) -> Any: ...
    def fp_is_snan_bits(self, bits: Any) -> Any: ...
    def fp_bits_to_double(self, bits: Any) -> Any: ...
    def fp_double_to_bits(self, value: Any) -> Any: ...
    def fp_rm_rne(self) -> Any: ...
    def fp_rm_rtz(self) -> Any: ...
    def fp_rm_rtp(self) -> Any: ...
    def fp_rm_rtn(self) -> Any: ...
    def fp_rm_from_rn(self, rn: Any) -> Any: ...
    def fp_add(self, rm: Any, a: Any, b: Any) -> Any: ...
    def fp_sub(self, rm: Any, a: Any, b: Any) -> Any: ...
    def fp_mul(self, rm: Any, a: Any, b: Any) -> Any: ...
    def fp_div(self, rm: Any, a: Any, b: Any) -> Any: ...
    def fp_sqrt(self, rm: Any, a: Any) -> Any: ...
    def fp_fma(self, rm: Any, a: Any, b: Any, c: Any) -> Any: ...
    def fp_fms(self, rm: Any, a: Any, b: Any, c: Any) -> Any: ...
    def fp_fnma(self, rm: Any, a: Any, b: Any, c: Any) -> Any: ...
    def fp_fnms(self, rm: Any, a: Any, b: Any, c: Any) -> Any: ...
    def fp_neg(self, a: Any) -> Any: ...
    def fp_abs(self, a: Any) -> Any: ...
    def fp_sel(self, a: Any, c: Any, b: Any) -> Any: ...
    def fp_round_to_single(self, rm: Any, a: Any) -> Any: ...
    def fp_to_sint32(self, rm: Any, a: Any) -> Any: ...
    def fp_to_sint32_trunc(self, a: Any) -> Any: ...
    def fp_is_lt(self, a: Any, b: Any) -> Any: ...
    def fp_is_gt(self, a: Any, b: Any) -> Any: ...
    def fp_is_eq(self, a: Any, b: Any) -> Any: ...
    def fp_is_nan(self, a: Any) -> Any: ...
    def fp_is_inf(self, a: Any) -> Any: ...
    def fp_is_ge_zero(self, a: Any) -> Any: ...
    def fp_const_f32(self, value: float) -> Any: ...
    def fp_double_to_f32_bits(self, rm: Any, value: Any) -> Any: ...
    def fp_bits32_to_double(self, bits: Any) -> Any: ...


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

    def fp_const64(self, value: int) -> int: return value & 0xFFFFFFFFFFFFFFFF
    def fp_low_word(self, bits: int) -> int: return bits & MASK32
    def fp_high_word(self, bits: int) -> int: return (bits >> 32) & MASK32
    def fp_join_words(self, high: int, low: int) -> int:
        return ((high & MASK32) << 32) | (low & MASK32)
    def fp_xor_sign(self, bits: int) -> int: return bits ^ (1 << 63)
    def fp_clear_sign(self, bits: int) -> int: return bits & ~(1 << 63)
    def fp_set_sign(self, bits: int) -> int: return bits | (1 << 63)
    def fp_fprf(self, bits: int) -> int:
        bits &= 0xFFFFFFFFFFFFFFFF
        sign = bool(bits & (1 << 63))
        exponent = (bits >> 52) & 0x7FF
        fraction = bits & ((1 << 52) - 1)
        if exponent == 0x7FF:
            if fraction: return 0x11
            return 0x09 if sign else 0x05
        if exponent == 0:
            if not fraction: return 0x12 if sign else 0x02
            return 0x18 if sign else 0x14
        return 0x08 if sign else 0x04
    def fp_force_25bit(self, bits: int) -> int:
        bits &= 0xFFFFFFFFFFFFFFFF
        exponent = bits & 0x7FF0000000000000
        fraction = bits & 0x000FFFFFFFFFFFFF
        if exponent == 0 and fraction:
            shift = (64 - fraction.bit_length()) - 11
            signed_mask = 0xFFFFFFFFF8000000 - (1 << 64)
            keep_mask = (signed_mask >> shift) & 0xFFFFFFFFFFFFFFFF
            round_bit = 0x08000000 >> shift
            return ((bits & keep_mask) + (bits & round_bit)) & 0xFFFFFFFFFFFFFFFF
        return ((bits & 0xFFFFFFFFF8000000) + (bits & 0x08000000)) & 0xFFFFFFFFFFFFFFFF

    def fp_is_snan_bits(self, bits: int) -> bool:
        bits &= 0xFFFFFFFFFFFFFFFF
        exponent = bits & 0x7FF0000000000000
        fraction = bits & 0x000FFFFFFFFFFFFF
        quiet = bits & 0x0008000000000000
        return exponent == 0x7FF0000000000000 and fraction != 0 and quiet == 0

    def fp_bits_to_double(self, bits: int) -> float:
        import struct
        return struct.unpack(">d", struct.pack(">Q", bits & 0xFFFFFFFFFFFFFFFF))[0]

    def fp_double_to_bits(self, value: float) -> int:
        import struct
        return struct.unpack(">Q", struct.pack(">d", value))[0]

    def fp_rm_rne(self) -> str: return "rne"
    def fp_rm_rtz(self) -> str: return "rtz"
    def fp_rm_rtp(self) -> str: return "rtp"
    def fp_rm_rtn(self) -> str: return "rtn"

    def fp_rm_from_rn(self, rn: int) -> str:
        return {0: "rne", 1: "rtz", 2: "rtp", 3: "rtn"}.get(rn & 3, "rne")

    def _fp_round_int(self, value: float, rm: str) -> float:
        """Round a double to the nearest integer following the given IEEE mode."""
        import math
        if rm == "rtz":
            return float(int(value)) if value >= 0 else float(int(value))
        if rm == "rtp":
            return math.ceil(value)
        if rm == "rtn":
            return math.floor(value)
        return round(value)

    def _fp_to_single(self, value: float) -> float:
        import math
        import struct
        try:
            return struct.unpack(">f", struct.pack(">f", float(value)))[0]
        except OverflowError:
            return math.copysign(float("inf"), value)

    def fp_add(self, rm: str, a: float, b: float) -> float: return a + b
    def fp_sub(self, rm: str, a: float, b: float) -> float: return a - b
    def fp_mul(self, rm: str, a: float, b: float) -> float: return a * b
    def fp_div(self, rm: str, a: float, b: float) -> float:
        import math
        if math.isnan(a) or math.isnan(b): return float("nan")
        if b == 0.0:
            if a == 0.0: return float("nan")
            return math.copysign(float("inf"), a * b if b else a * math.copysign(1.0, b))
        return a / b
    def fp_sqrt(self, rm: str, a: float) -> float:
        import math
        return math.sqrt(a) if a >= 0 else float("nan")
    def fp_fma(self, rm: str, a: float, b: float, c: float) -> float: return a * b + c
    def fp_fms(self, rm: str, a: float, b: float, c: float) -> float: return a * b - c
    def fp_fnma(self, rm: str, a: float, b: float, c: float) -> float: return -(a * b + c)
    def fp_fnms(self, rm: str, a: float, b: float, c: float) -> float: return -(a * b - c)
    def fp_neg(self, a: float) -> float: return -a
    def fp_abs(self, a: float) -> float: return abs(a)
    def fp_sel(self, a: float, c: float, b: float) -> float: return c if a >= 0.0 else b
    def fp_round_to_single(self, rm: str, a: float) -> float: return self._fp_to_single(a)
    def fp_to_sint32(self, rm: str, a: float) -> int:
        import math
        if math.isnan(a) or math.isinf(a): return 0x80000000
        return int(self._fp_round_int(a, rm)) & 0xFFFFFFFF
    def fp_to_sint32_trunc(self, a: float) -> int:
        import math
        if math.isnan(a) or math.isinf(a): return 0x80000000
        return int(a) & 0xFFFFFFFF
    def fp_is_lt(self, a: float, b: float) -> bool: return a < b
    def fp_is_gt(self, a: float, b: float) -> bool: return a > b
    def fp_is_eq(self, a: float, b: float) -> bool: return a == b
    def fp_is_nan(self, a: float) -> bool:
        import math
        return math.isnan(a)
    def fp_is_inf(self, a: float) -> bool:
        import math
        return math.isinf(a)
    def fp_is_ge_zero(self, a: float) -> bool: return a >= 0.0
    def fp_const_f32(self, value: float) -> float: return value
    def fp_double_to_f32_bits(self, rm: str, value: float) -> int:
        import struct
        return struct.unpack(">I", struct.pack(">f", self._fp_to_single(value)))[0]
    def fp_bits32_to_double(self, bits: int) -> float:
        import struct
        return struct.unpack(">f", struct.pack(">I", bits & 0xFFFFFFFF))[0]


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

    def fp_const64(self, value: int) -> Any: return self.z3.BitVecVal(value & 0xFFFFFFFFFFFFFFFF, 64)
    def fp_low_word(self, bits: Any) -> Any: return self.z3.Extract(31, 0, bits)
    def fp_high_word(self, bits: Any) -> Any: return self.z3.Extract(63, 32, bits)
    def fp_join_words(self, high: Any, low: Any) -> Any: return self.z3.Concat(high, low)
    def fp_xor_sign(self, bits: Any) -> Any:
        return bits ^ self.z3.BitVecVal(1 << 63, 64)
    def fp_clear_sign(self, bits: Any) -> Any:
        return bits & self.z3.BitVecVal((1 << 63) - 1, 64)
    def fp_set_sign(self, bits: Any) -> Any:
        return bits | self.z3.BitVecVal(1 << 63, 64)
    def fp_fprf(self, bits: Any) -> Any:
        z3 = self.z3
        sign = z3.Extract(63, 63, bits) == z3.BitVecVal(1, 1)
        exponent = z3.Extract(62, 52, bits)
        fraction = z3.Extract(51, 0, bits)
        exponent_zero = exponent == z3.BitVecVal(0, 11)
        exponent_ones = exponent == z3.BitVecVal(0x7FF, 11)
        fraction_zero = fraction == z3.BitVecVal(0, 52)
        return z3.If(
            exponent_ones,
            z3.If(fraction_zero, z3.If(sign, self.const(0x09), self.const(0x05)), self.const(0x11)),
            z3.If(
                exponent_zero,
                z3.If(fraction_zero, z3.If(sign, self.const(0x12), self.const(0x02)), z3.If(sign, self.const(0x18), self.const(0x14))),
                z3.If(sign, self.const(0x08), self.const(0x04)),
            ),
        )
    def fp_force_25bit(self, bits: Any) -> Any:
        z3 = self.z3
        normal = (
            (bits & z3.BitVecVal(0xFFFFFFFFF8000000, 64))
            + (bits & z3.BitVecVal(0x08000000, 64))
        )
        fraction = z3.Extract(51, 0, bits)
        subnormal = normal
        # Force25Bit shifts its keep/round masks according to the highest set
        # fraction bit for double subnormals.  Build the finite 52-way choice
        # explicitly so the result stays a pure bit-vector expression.
        signed_mask = 0xFFFFFFFFF8000000 - (1 << 64)
        for highest_bit in range(52):
            shift = 52 - highest_bit
            keep_mask = (signed_mask >> shift) & 0xFFFFFFFFFFFFFFFF
            round_bit = 0x08000000 >> shift
            rounded = (
                (bits & z3.BitVecVal(keep_mask, 64))
                + (bits & z3.BitVecVal(round_bit, 64))
            )
            is_highest = z3.And(
                z3.Extract(highest_bit, highest_bit, fraction) == z3.BitVecVal(1, 1),
                z3.Extract(51, highest_bit + 1, fraction) == z3.BitVecVal(0, 51 - highest_bit)
                if highest_bit < 51 else z3.BoolVal(True),
            )
            subnormal = z3.If(is_highest, rounded, subnormal)
        exponent_zero = z3.Extract(62, 52, bits) == z3.BitVecVal(0, 11)
        fraction_nonzero = fraction != z3.BitVecVal(0, 52)
        return z3.If(z3.And(exponent_zero, fraction_nonzero), subnormal, normal)
    def fp_is_snan_bits(self, bits: Any) -> Any:
        z3 = self.z3
        exponent = z3.Extract(62, 52, bits)
        fraction = z3.Extract(51, 0, bits)
        quiet = z3.Extract(51, 51, bits)
        return z3.And(
            exponent == z3.BitVecVal(0x7FF, 11),
            fraction != z3.BitVecVal(0, 52),
            quiet == z3.BitVecVal(0, 1),
        )
    def fp_bits_to_double(self, bits: Any) -> Any:
        return self.z3.fpBVToFP(bits, self.z3.Float64())
    def fp_double_to_bits(self, value: Any) -> Any:
        return self.z3.fpToIEEEBV(value)

    def fp_rm_rne(self) -> Any: return self.z3.RNE()
    def fp_rm_rtz(self) -> Any: return self.z3.RTZ()
    def fp_rm_rtp(self) -> Any: return self.z3.RTP()
    def fp_rm_rtn(self) -> Any: return self.z3.RTN()

    def fp_rm_from_rn(self, rn: Any) -> Any:
        return self.ite(
            self.eq(rn, self.const(1)), self.fp_rm_rtz(),
            self.ite(self.eq(rn, self.const(2)), self.fp_rm_rtp(),
                     self.ite(self.eq(rn, self.const(3)), self.fp_rm_rtn(), self.fp_rm_rne())))

    def fp_add(self, rm: Any, a: Any, b: Any) -> Any: return self.z3.fpAdd(rm, a, b)
    def fp_sub(self, rm: Any, a: Any, b: Any) -> Any: return self.z3.fpSub(rm, a, b)
    def fp_mul(self, rm: Any, a: Any, b: Any) -> Any: return self.z3.fpMul(rm, a, b)
    def fp_div(self, rm: Any, a: Any, b: Any) -> Any: return self.z3.fpDiv(rm, a, b)
    def fp_sqrt(self, rm: Any, a: Any) -> Any: return self.z3.fpSqrt(rm, a)
    def fp_fma(self, rm: Any, a: Any, b: Any, c: Any) -> Any: return self.z3.fpFMA(rm, a, b, c)
    def fp_fms(self, rm: Any, a: Any, b: Any, c: Any) -> Any:
        return self.z3.fpFMA(rm, a, b, self.z3.fpNeg(c))
    def fp_fnma(self, rm: Any, a: Any, b: Any, c: Any) -> Any:
        return self.z3.fpNeg(self.z3.fpFMA(rm, a, b, c))
    def fp_fnms(self, rm: Any, a: Any, b: Any, c: Any) -> Any:
        return self.z3.fpNeg(self.z3.fpFMA(rm, a, b, self.z3.fpNeg(c)))
    def fp_neg(self, a: Any) -> Any: return self.z3.fpNeg(a)
    def fp_abs(self, a: Any) -> Any: return self.z3.fpAbs(a)
    def fp_sel(self, a: Any, c: Any, b: Any) -> Any:
        return self.z3.If(self.fp_is_ge_zero(a), c, b)
    def fp_round_to_single(self, rm: Any, a: Any) -> Any:
        single = self.z3.fpFPToFP(rm, a, self.z3.Float32())
        return self.z3.fpFPToFP(self.z3.RNE(), single, self.z3.Float64())
    def fp_to_sint32(self, rm: Any, a: Any) -> Any:
        bv = self.z3.fpToSBV(rm, a, self.z3.BitVecSort(32))
        ext = self.z3.ZeroExt(32, bv)
        return ext
    def fp_to_sint32_trunc(self, a: Any) -> Any:
        bv = self.z3.fpToSBV(self.z3.RTZ(), a, self.z3.BitVecSort(32))
        ext = self.z3.ZeroExt(32, bv)
        return ext
    def fp_is_lt(self, a: Any, b: Any) -> Any: return self.z3.fpLT(a, b)
    def fp_is_gt(self, a: Any, b: Any) -> Any: return self.z3.fpGT(a, b)
    def fp_is_eq(self, a: Any, b: Any) -> Any: return self.z3.fpEQ(a, b)
    def fp_is_nan(self, a: Any) -> Any: return self.z3.fpIsNaN(a)
    def fp_is_inf(self, a: Any) -> Any: return self.z3.fpIsInf(a)
    def fp_is_ge_zero(self, a: Any) -> Any:
        return self.z3.And(
            self.z3.Not(self.fp_is_nan(a)),
            self.z3.Not(self.fp_is_lt(a, self.z3.FPVal(-0.0, self.z3.Float64()))),
        )
    def fp_const_f32(self, value: float) -> Any:
        return self.z3.FPVal(value, self.z3.Float32())
    def fp_double_to_f32_bits(self, rm: Any, value: Any) -> Any:
        f32 = self.z3.fpFPToFP(rm, value, self.z3.Float32())
        return self.z3.fpToIEEEBV(f32)
    def fp_bits32_to_double(self, bits: Any) -> Any:
        f32 = self.z3.fpBVToFP(bits, self.z3.Float32())
        return self.z3.fpFPToFP(self.z3.RNE(), f32, self.z3.Float64())


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
    Opcode.LFSX, Opcode.LFSUX, Opcode.LFDX, Opcode.LFDUX,
    Opcode.STFSX, Opcode.STFSUX, Opcode.STFDX, Opcode.STFDUX, Opcode.STFIWX,
}

FP_D_LOADS = {
    Opcode.LFS: (4, False), Opcode.LFSU: (4, True), Opcode.LFD: (8, False), Opcode.LFDU: (8, True),
}
FP_X_LOADS = {
    Opcode.LFSX: 4, Opcode.LFSUX: 4, Opcode.LFDX: 8, Opcode.LFDUX: 8,
}
FP_D_STORES = {
    Opcode.STFS: (4, False), Opcode.STFSU: (4, True), Opcode.STFD: (8, False), Opcode.STFDU: (8, True),
}
FP_X_STORES = {
    Opcode.STFSX: 4, Opcode.STFSUX: 4, Opcode.STFDX: 8, Opcode.STFDUX: 8, Opcode.STFIWX: 4,
}
FP_D_MEM = {**FP_D_LOADS, **FP_D_STORES}
FP_X_MEM = {**FP_X_LOADS, **FP_X_STORES}
FP_UPDATES = {Opcode.LFSU, Opcode.LFDU, Opcode.LFSUX, Opcode.LFDUX, Opcode.STFSU, Opcode.STFDU, Opcode.STFSUX, Opcode.STFDUX}

_FP_SINGLE_ARITH = {
    Opcode.FADDS, Opcode.FSUBS, Opcode.FMULS, Opcode.FDIVS, Opcode.FSQRTS, Opcode.FRES,
    Opcode.FMADDS, Opcode.FMSUBS, Opcode.FNMADDS, Opcode.FNMSUBS, Opcode.FRSP,
}
_FP_DOUBLE_ARITH = {
    Opcode.FADD, Opcode.FSUB, Opcode.FMUL, Opcode.FDIV, Opcode.FSQRT, Opcode.FRSQRTE, Opcode.FSEL,
    Opcode.FMADD, Opcode.FMSUB, Opcode.FNMADD, Opcode.FNMSUB,
}
_FP_AUX_OPS = {
    Opcode.FCMPU, Opcode.FCMPO, Opcode.FNEG, Opcode.FMR, Opcode.FNABS, Opcode.FABS,
    Opcode.FCTIW, Opcode.FCTIWZ, Opcode.MFFS, Opcode.MTFSF, Opcode.MTFSFI,
    Opcode.MTFSB0, Opcode.MTFSB1, Opcode.MCRFS,
}
_FP_PS_ARITH = {
    Opcode.PS_DIV, Opcode.PS_SUB, Opcode.PS_ADD, Opcode.PS_SEL, Opcode.PS_RES, Opcode.PS_MUL,
    Opcode.PS_RSQRTE, Opcode.PS_MSUB, Opcode.PS_MADD, Opcode.PS_NMSUB, Opcode.PS_NMADD,
    Opcode.PS_SUM0, Opcode.PS_SUM1, Opcode.PS_MULS0, Opcode.PS_MULS1,
    Opcode.PS_MADDS0, Opcode.PS_MADDS1, Opcode.PS_NEG, Opcode.PS_MR, Opcode.PS_NABS, Opcode.PS_ABS,
}
_FP_PS_CMP_MERGE = {
    Opcode.PS_CMPU0, Opcode.PS_CMPO0, Opcode.PS_CMPU1, Opcode.PS_CMPO1,
    Opcode.PS_MERGE00, Opcode.PS_MERGE01, Opcode.PS_MERGE10, Opcode.PS_MERGE11,
}
_FP_PSQ_OPS = {
    Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU,
    Opcode.PSQ_LX, Opcode.PSQ_LUX, Opcode.PSQ_STX, Opcode.PSQ_STUX,
}
_FP_SCALAR_ARITH = _FP_SINGLE_ARITH | _FP_DOUBLE_ARITH | _FP_AUX_OPS
_FP_VALUE_ARITH = {
    Opcode.FADDS, Opcode.FSUBS, Opcode.FMULS, Opcode.FDIVS,
    Opcode.FADD, Opcode.FSUB, Opcode.FMUL, Opcode.FDIV, Opcode.FRSP,
}
_FP_ROUNDING_SENSITIVE = _FP_VALUE_ARITH | {
    Opcode.STFS, Opcode.STFSU, Opcode.STFSX, Opcode.STFSUX,
}


def _ps_extract0(fpr_bits: Any, ops: WordOps) -> Any:
    if isinstance(ops, ConcreteOps):
        import struct
        return struct.unpack(">f", struct.pack(">I", fpr_bits & 0xFFFFFFFF))[0]
    z3 = ops.z3
    low32 = z3.Extract(31, 0, fpr_bits)
    return z3.fpFPToFP(z3.RNE(), low32, z3.Float32())


def _ps_extract1(fpr_bits: Any, ops: WordOps) -> Any:
    if isinstance(ops, ConcreteOps):
        import struct
        return struct.unpack(">f", struct.pack(">I", (fpr_bits >> 32) & 0xFFFFFFFF))[0]
    z3 = ops.z3
    high32 = z3.Extract(63, 32, fpr_bits)
    return z3.fpFPToFP(z3.RNE(), high32, z3.Float32())


def _ps_merge(lo_f32: Any, hi_f32: Any, ops: WordOps) -> Any:
    if isinstance(ops, ConcreteOps):
        import struct
        lo_bits = struct.unpack(">I", struct.pack(">f", float(lo_f32)))[0]
        hi_bits = struct.unpack(">I", struct.pack(">f", float(hi_f32)))[0]
        return ((hi_bits & 0xFFFFFFFF) << 32) | (lo_bits & 0xFFFFFFFF)
    z3 = ops.z3
    lo_bits = z3.fpToIEEEBV(lo_f32)
    hi_bits = z3.fpToIEEEBV(hi_f32)
    return z3.Concat(hi_bits, lo_bits)


def _fp_compare_nibble(a: Any, b: Any, ops: WordOps) -> Any:
    lt = ops.fp_is_lt(a, b)
    gt = ops.fp_is_gt(a, b)
    eq = ops.fp_is_eq(a, b)
    un = ops.lor(ops.fp_is_nan(a), ops.fp_is_nan(b))
    return ops.bor(
        ops.bor(ops.ite(lt, ops.const(8), ops.const(0)), ops.ite(gt, ops.const(4), ops.const(0))),
        ops.bor(ops.ite(eq, ops.const(2), ops.const(0)), ops.ite(un, ops.const(1), ops.const(0))),
    )


def _fpcc_nibble(state: MachineState, value: Any, ops: WordOps) -> Any:
    zero = ops.fp_bits_to_double(ops.fp_const64(0))
    return _fp_compare_nibble(value, zero, ops)


def _fpscr_replace_mask(fpscr: Any, mask_value: int, value: Any, ops: WordOps) -> Any:
    mask = ops.const(mask_value)
    return ops.bor(ops.band(fpscr, ops.bnot(mask)), ops.band(value, mask))


def _fpscr_set_fprf(state: MachineState, fpr_bits: Any, ops: WordOps) -> MachineState:
    value = ops.shl(ops.fp_fprf(fpr_bits), ops.const(12))
    return state.with_fpscr(_fpscr_replace_mask(state.fpscr, FPSCR_FPRF_MASK, value, ops))


def _fpscr_cr1(state: MachineState, ops: WordOps) -> Any:
    return ops.band(ops.lshr(state.fpscr, ops.const(28)), ops.const(0xF))


def _fpscr_raise(state: MachineState, exception_mask: int, ops: WordOps) -> MachineState:
    """Set a sticky FPSCR exception and recompute FX/VX/FEX summaries."""
    mask = ops.const(exception_mask)
    already_set = ops.eq(ops.band(state.fpscr, mask), mask)
    value = ops.bor(state.fpscr, mask)
    value = ops.ite(already_set, value, ops.bor(value, ops.const(FPSCR_FX)))
    vx_set = ops.lnot(ops.eq(ops.band(value, ops.const(FPSCR_VX_ANY)), ops.const(0)))
    value = ops.ite(vx_set, ops.bor(value, ops.const(FPSCR_VX)), ops.band(value, ops.bnot(ops.const(FPSCR_VX))))
    enabled = ops.band(ops.lshr(value, ops.const(22)), ops.band(value, ops.const(FPSCR_ANY_ENABLE)))
    fex_set = ops.lnot(ops.eq(enabled, ops.const(0)))
    value = ops.ite(fex_set, ops.bor(value, ops.const(FPSCR_FEX)), ops.band(value, ops.bnot(ops.const(FPSCR_FEX))))
    return state.with_fpscr(value)


def _fpscr_raise_if(
    state: MachineState, condition: Any, exception_mask: int, ops: WordOps,
) -> MachineState:
    raised = _fpscr_raise(state, exception_mask, ops)
    return state.with_fpscr(ops.ite(condition, raised.fpscr, state.fpscr))


def execute_instruction(state: MachineState, insn: Instruction, ops: WordOps) -> MachineState:
    op, a = insn.opcode, insn.operands

    # Defense in depth for callers that construct Instruction objects without
    # going through decoder.py.
    if op not in SUPPORTED_OPCODES:
        raise UnsupportedInstruction(insn.address, insn.raw, f"semantics are unsupported for {op.value}")
    if op in _FP_ROUNDING_SENSITIVE:
        # ConcreteOps is validated against Broadway with RN=nearest-even and
        # NI disabled.  Keep the symbolic proof on that same explicit domain.
        standard_fp_mode = ops.eq(ops.band(state.fpscr, ops.const(7)), ops.const(0))
        if isinstance(ops, ConcreteOps) and not standard_fp_mode:
            raise ExecutionInconclusive("FP ConcreteOps requires FPSCR.RN=nearest-even and NI=0")
        state = replace(state, valid=ops.land(state.valid, standard_fp_mode))
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
            if op == Opcode.ADDME:
                # Dolphin/PowerPC Helper_Carry(left, CA - 1): with CA=1
                # this form always clears CA; with CA=0 it sets CA iff RA!=0.
                carry = ops.land(
                    ops.lnot(state.xer.ca),
                    ops.lnot(ops.eq(left, ops.const(0))),
                )
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
                if op == Opcode.SUBFME:
                    # Dolphin/PowerPC Helper_Carry(~RA, CA - 1): with CA=1
                    # this form always clears CA; with CA=0 it sets CA unless
                    # RA is 0xffffffff.
                    carry = ops.land(
                        ops.lnot(state.xer.ca),
                        ops.lnot(ops.eq(left, ops.const(MASK32))),
                    )
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
    elif op in FP_D_MEM or op in FP_X_MEM:
        rt, ra, third = a
        if op in INDEXED_MEMORY:
            address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], state.gpr[third])
        else:
            address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], ops.const(third))
        is_load = op in FP_D_LOADS or op in FP_X_LOADS
        if is_load:
            width = FP_D_LOADS[op][0] if op in FP_D_LOADS else FP_X_LOADS[op]
        else:
            width = FP_D_STORES[op][0] if op in FP_D_STORES else FP_X_STORES[op]
        if op == Opcode.STFIWX:
            fpr_bits = state.fpr[rt]
            value32 = ops.fp_low_word(fpr_bits)
            state = _touch_memory(state, address, 4, ops)
            state = replace(state, memory=_store(state.memory, address, value32, 4, ops))
            return state
        if is_load:
            update = op in (Opcode.LFSU, Opcode.LFDU, Opcode.LFSUX, Opcode.LFDUX)
            state = _touch_memory(state, address, width, ops)
            if width == 4:
                raw = _load(state.memory, address, 4, ops)
                result = ops.fp_double_to_bits(ops.fp_bits32_to_double(raw))
            else:
                high = _load(state.memory, address, 4, ops)
                low = _load(state.memory, ops.add(address, ops.const(4)), 4, ops)
                result = ops.fp_join_words(high, low)
            state = state.with_fpr(rt, result)
            if update: state = state.with_gpr(ra, address)
            return state
        else:
            update = op in (Opcode.STFSU, Opcode.STFDU, Opcode.STFSUX, Opcode.STFDUX)
            state = _touch_memory(state, address, width, ops)
            fpr_bits = state.fpr[rt]
            if width == 4:
                rm = ops.fp_rm_from_rn(ops.band(state.fpscr, ops.const(3)))
                bits32 = ops.fp_double_to_f32_bits(rm, ops.fp_bits_to_double(fpr_bits))
                state = replace(state, memory=_store(state.memory, address, bits32, 4, ops))
            else:
                state = replace(state, memory=_store(state.memory, address, ops.fp_high_word(fpr_bits), 4, ops))
                state = replace(state, memory=_store(
                    state.memory, ops.add(address, ops.const(4)), ops.fp_low_word(fpr_bits), 4, ops,
                ))
            if update: state = state.with_gpr(ra, address)
            return state

    # -- Floating-point arithmetic (scalar single, double, paired-single, FPSCR) --
    elif op in _FP_SCALAR_ARITH or op in _FP_PS_ARITH:
        fd, fa = a[0], a[1] if len(a) > 1 else 0
        fb = a[2] if len(a) > 2 else 0
        fc = a[3] if len(a) > 3 else 0
        rn_bits = ops.band(state.fpscr, ops.const(3))
        rm = ops.fp_rm_from_rn(rn_bits)
        is_single = op in _FP_SINGLE_ARITH
        is_ps = op in _FP_PS_ARITH
        ni = ops.lnot(ops.eq(ops.band(state.fpscr, ops.const(4)), ops.const(0)))

        if is_ps:
            _fa_bits = state.fpr[fa]
            _fb_bits = state.fpr[fb]
            _fc_bits = state.fpr[fc]
            fa_ps0 = _ps_extract0(_fa_bits, ops)
            fa_ps1 = _ps_extract1(_fa_bits, ops)
            fb_ps0 = _ps_extract0(_fb_bits, ops)
            fb_ps1 = _ps_extract1(_fb_bits, ops)
            fc_ps0 = _ps_extract0(_fc_bits, ops)
            fc_ps1 = _ps_extract1(_fc_bits, ops)
            rm_ps = ops.fp_rm_rne()
            if op == Opcode.PS_DIV:
                r0, r1 = ops.fp_div(rm_ps, fa_ps0, fb_ps0), ops.fp_div(rm_ps, fa_ps1, fb_ps1)
            elif op == Opcode.PS_SUB:
                r0, r1 = ops.fp_sub(rm_ps, fa_ps0, fb_ps0), ops.fp_sub(rm_ps, fa_ps1, fb_ps1)
            elif op == Opcode.PS_ADD:
                r0, r1 = ops.fp_add(rm_ps, fa_ps0, fb_ps0), ops.fp_add(rm_ps, fa_ps1, fb_ps1)
            elif op == Opcode.PS_SEL:
                r0 = ops.fp_sel(fa_ps0, fc_ps0, fb_ps0)
                r1 = ops.fp_sel(fa_ps1, fc_ps1, fb_ps1)
            elif op == Opcode.PS_RES:
                r0 = ops.fp_div(rm_ps, ops.fp_const_f32(1.0), fa_ps0)
                r1 = ops.fp_div(rm_ps, ops.fp_const_f32(1.0), fa_ps1)
            elif op == Opcode.PS_MUL:
                r0, r1 = ops.fp_mul(rm_ps, fa_ps0, fc_ps0), ops.fp_mul(rm_ps, fa_ps1, fc_ps1)
            elif op == Opcode.PS_RSQRTE:
                r0 = ops.fp_div(rm_ps, ops.fp_const_f32(1.0), ops.fp_sqrt(rm_ps, fa_ps0))
                r1 = ops.fp_div(rm_ps, ops.fp_const_f32(1.0), ops.fp_sqrt(rm_ps, fa_ps1))
            elif op == Opcode.PS_MSUB:
                r0, r1 = ops.fp_fms(rm_ps, fa_ps0, fc_ps0, fb_ps0), ops.fp_fms(rm_ps, fa_ps1, fc_ps1, fb_ps1)
            elif op == Opcode.PS_MADD:
                r0, r1 = ops.fp_fma(rm_ps, fa_ps0, fc_ps0, fb_ps0), ops.fp_fma(rm_ps, fa_ps1, fc_ps1, fb_ps1)
            elif op == Opcode.PS_NMSUB:
                r0, r1 = ops.fp_fnms(rm_ps, fa_ps0, fc_ps0, fb_ps0), ops.fp_fnms(rm_ps, fa_ps1, fc_ps1, fb_ps1)
            elif op == Opcode.PS_NMADD:
                r0, r1 = ops.fp_fnma(rm_ps, fa_ps0, fc_ps0, fb_ps0), ops.fp_fnma(rm_ps, fa_ps1, fc_ps1, fb_ps1)
            elif op == Opcode.PS_SUM0:
                r0 = ops.fp_add(rm_ps, fa_ps0, fb_ps0)
                r1 = fc_ps1
            elif op == Opcode.PS_SUM1:
                r0 = fc_ps0
                r1 = ops.fp_add(rm_ps, fa_ps1, fb_ps1)
            elif op == Opcode.PS_MULS0:
                r0 = ops.fp_mul(rm_ps, fa_ps0, fc_ps0)
                r1 = ops.fp_mul(rm_ps, fa_ps1, fc_ps0)
            elif op == Opcode.PS_MULS1:
                r0 = ops.fp_mul(rm_ps, fa_ps0, fc_ps1)
                r1 = ops.fp_mul(rm_ps, fa_ps1, fc_ps1)
            elif op == Opcode.PS_MADDS0:
                r0 = ops.fp_fma(rm_ps, fa_ps0, fc_ps0, fb_ps0)
                r1 = ops.fp_fma(rm_ps, fa_ps1, fc_ps0, fb_ps1)
            elif op == Opcode.PS_MADDS1:
                r0 = ops.fp_fma(rm_ps, fa_ps0, fc_ps1, fb_ps0)
                r1 = ops.fp_fma(rm_ps, fa_ps1, fc_ps1, fb_ps1)
            elif op == Opcode.PS_NEG:
                r0, r1 = ops.fp_neg(fa_ps0), ops.fp_neg(fa_ps1)
            elif op == Opcode.PS_MR:
                r0, r1 = fa_ps0, fa_ps1
            elif op == Opcode.PS_NABS:
                r0 = ops.fp_neg(ops.fp_abs(fa_ps0))
                r1 = ops.fp_neg(ops.fp_abs(fa_ps1))
            elif op == Opcode.PS_ABS:
                r0, r1 = ops.fp_abs(fa_ps0), ops.fp_abs(fa_ps1)
            else:
                raise UnsupportedInstruction(insn.address, insn.raw, f"unhandled PS opcode {op.value}")
            result = _ps_merge(r0, r1, ops)
            state = state.with_fpr(fd, result)
            return state

        # Scalar single instructions consume the register's double value and
        # round the result to binary32; they do not pre-round every operand.
        op_fa = ops.fp_bits_to_double(state.fpr[fa])
        op_fb = ops.fp_bits_to_double(state.fpr[fb])
        fc_bits = ops.fp_force_25bit(state.fpr[fc]) if op == Opcode.FMULS else state.fpr[fc]
        op_fc = ops.fp_bits_to_double(fc_bits)

        if op == Opcode.FSEL:
            result_bits = ops.ite(ops.fp_is_ge_zero(op_fa), state.fpr[fc], state.fpr[fb])
            state = state.with_fpr(fd, result_bits)
            if insn.record:
                cr1 = ops.band(ops.lshr(state.fpscr, ops.const(28)), ops.const(0xF))
                state = _set_cr_field(state, 1, cr1, ops)
            return state

        if op in (Opcode.FADDS, Opcode.FADD):
            d = ops.fp_add(rm, op_fa, op_fb)
        elif op in (Opcode.FSUBS, Opcode.FSUB):
            d = ops.fp_sub(rm, op_fa, op_fb)
        elif op in (Opcode.FMULS, Opcode.FMUL):
            d = ops.fp_mul(rm, op_fa, op_fc)
        elif op in (Opcode.FDIVS, Opcode.FDIV):
            d = ops.fp_div(rm, op_fa, op_fb)
        elif op in (Opcode.FSQRTS, Opcode.FSQRT):
            d = ops.fp_sqrt(rm, op_fb)
        elif op == Opcode.FRES:
            d = ops.fp_div(rm, ops.fp_round_to_single(rm, ops.fp_bits_to_double(ops.fp_const64(0x3FF0000000000000))), op_fb)
        elif op == Opcode.FRSQRTE:
            d = ops.fp_div(rm, ops.fp_bits_to_double(ops.fp_const64(0x3FF0000000000000)), ops.fp_sqrt(rm, op_fb))
        elif op in (Opcode.FMADDS, Opcode.FMADD):
            d = ops.fp_fma(rm, op_fa, op_fc, op_fb)
        elif op in (Opcode.FMSUBS, Opcode.FMSUB):
            d = ops.fp_fms(rm, op_fa, op_fc, op_fb)
        elif op in (Opcode.FNMADDS, Opcode.FNMADD):
            d = ops.fp_fnma(rm, op_fa, op_fc, op_fb)
        elif op in (Opcode.FNMSUBS, Opcode.FNMSUB):
            d = ops.fp_fnms(rm, op_fa, op_fc, op_fb)
        elif op in (Opcode.FNEG, Opcode.FNABS, Opcode.FABS, Opcode.FMR):
            source_bits = state.fpr[fb]
            if op == Opcode.FNEG:
                result_bits = ops.fp_xor_sign(source_bits)
            elif op == Opcode.FNABS:
                result_bits = ops.fp_set_sign(source_bits)
            elif op == Opcode.FABS:
                result_bits = ops.fp_clear_sign(source_bits)
            else:
                result_bits = source_bits
            state = state.with_fpr(fd, result_bits)
            if insn.record:
                cr1 = ops.band(ops.lshr(state.fpscr, ops.const(28)), ops.const(0xF))
                state = _set_cr_field(state, 1, cr1, ops)
            return state
        elif op == Opcode.FRSP:
            d = ops.fp_round_to_single(rm, op_fb)
        elif op == Opcode.FCTIW:
            result_bits = ops.fp_to_sint32(rm, op_fb)
            state = state.with_fpr(fd, result_bits)
            if insn.record:
                state = _set_cr_field(state, 1, _fpcc_nibble(state, result_bits, ops), ops)
            return state
        elif op == Opcode.FCTIWZ:
            result_bits = ops.fp_to_sint32_trunc(op_fb)
            state = state.with_fpr(fd, result_bits)
            if insn.record:
                state = _set_cr_field(state, 1, _fpcc_nibble(state, result_bits, ops), ops)
            return state
        elif op in (Opcode.FCMPU, Opcode.FCMPO):
            bf = fd
            cr_nibble = _fp_compare_nibble(op_fa, op_fb, ops)
            is_snan = ops.lor(
                ops.fp_is_snan_bits(state.fpr[fa]),
                ops.fp_is_snan_bits(state.fpr[fb]),
            )
            is_nan = ops.lor(ops.fp_is_nan(op_fa), ops.fp_is_nan(op_fb))
            state = _fpscr_raise_if(state, is_snan, FPSCR_VXSNAN, ops)
            if op == Opcode.FCMPO:
                invalid_enabled = ops.lnot(ops.eq(
                    ops.band(state.fpscr, ops.const(FPSCR_VE)), ops.const(0),
                ))
                # Ordered compare raises VXVC for every quiet NaN, and for a
                # signaling NaN only when invalid exceptions are disabled.
                raise_vxvc = ops.land(
                    is_nan,
                    ops.lor(ops.lnot(is_snan), ops.lnot(invalid_enabled)),
                )
                state = _fpscr_raise_if(state, raise_vxvc, FPSCR_VXVC, ops)
            # FPCC is the low four bits of FPSCR.FPRF (numeric bits 15..12).
            new_fpscr = ops.bor(
                ops.band(state.fpscr, ops.bnot(ops.const(0xF000))),
                ops.shl(cr_nibble, ops.const(12)),
            )
            return _set_cr_field(state.with_fpscr(new_fpscr), bf, cr_nibble, ops)
        elif op == Opcode.MFFS:
            fpscr_bits = ops.fp_double_to_bits(ops.fp_bits_to_double(
                ops.shl(state.fpscr, ops.const(32))))
            state = state.with_fpr(fd, fpscr_bits)
            if insn.record:
                state = _set_cr_field(state, 1, _fpcc_nibble(state, fpscr_bits, ops), ops)
            return state
        elif op == Opcode.MTFSF:
            fm, rb_fpr = a
            source = state.fpr[rb_fpr]
            mask = 0
            for field in range(8):
                if fm & (1 << (7 - field)):
                    mask |= 0xF << ((7 - field) * 4)
            src32 = ops.band(source, ops.const(0xFFFFFFFF))
            new_fpscr = ops.bor(ops.band(state.fpscr, ops.bnot(ops.const(mask))),
                                ops.band(src32, ops.const(mask)))
            state = state.with_fpscr(new_fpscr)
            if insn.record:
                state = _set_cr_field(state, 1, _fpcc_nibble(state, new_fpscr, ops), ops)
            return state
        elif op == Opcode.MTFSFI:
            bf, imm4 = a
            shift = (7 - bf) * 4
            new_fpscr = ops.bor(
                ops.band(state.fpscr, ops.bnot(ops.const(0xF << shift))),
                ops.shl(ops.const(imm4 & 0xF), ops.const(shift)))
            state = state.with_fpscr(new_fpscr)
            if insn.record:
                state = _set_cr_field(state, 1, _fpcc_nibble(state, new_fpscr, ops), ops)
            return state
        elif op in (Opcode.MTFSB0, Opcode.MTFSB1):
            bt = fd
            bit_val = 1 if op == Opcode.MTFSB1 else 0
            new_fpscr = ops.bor(
                ops.band(state.fpscr, ops.bnot(ops.const(1 << (31 - bt)))),
                ops.const(bit_val << (31 - bt)) if bit_val else ops.const(0))
            state = state.with_fpscr(new_fpscr)
            if insn.record:
                state = _set_cr_field(state, 1, _fpcc_nibble(state, new_fpscr, ops), ops)
            return state
        elif op == Opcode.MCRFS:
            bf, bfa = a
            shift = (7 - bfa) * 4
            cr_nibble = ops.band(ops.lshr(state.fpscr, ops.const(shift)), ops.const(0xF))
            new_fpscr = ops.band(state.fpscr, ops.bnot(ops.const(0xF << shift)))
            state = state.with_fpscr(new_fpscr)
            return _set_cr_field(state, bf, cr_nibble, ops)
        elif op in (Opcode.PS_CMPU0, Opcode.PS_CMPO0, Opcode.PS_CMPU1, Opcode.PS_CMPO1):
            bf = fd
            fa_bits = state.fpr[fa]
            fb_bits = state.fpr[fb]
            if op in (Opcode.PS_CMPU0, Opcode.PS_CMPO0):
                fa_s = _ps_extract0(fa_bits, ops)
                fb_s = _ps_extract0(fb_bits, ops)
            else:
                fa_s = _ps_extract1(fa_bits, ops)
                fb_s = _ps_extract1(fb_bits, ops)
            cr_nibble = _fp_compare_nibble(fa_s, fb_s, ops)
            return _set_cr_field(state, bf, cr_nibble, ops)
        elif op in (Opcode.PS_MERGE00, Opcode.PS_MERGE01, Opcode.PS_MERGE10, Opcode.PS_MERGE11):
            fa_bits = state.fpr[fa]
            fb_bits = state.fpr[fb]
            if op == Opcode.PS_MERGE00:
                lo = _ps_extract0(fa_bits, ops)
                hi = _ps_extract0(fb_bits, ops)
            elif op == Opcode.PS_MERGE01:
                lo = _ps_extract0(fa_bits, ops)
                hi = _ps_extract1(fb_bits, ops)
            elif op == Opcode.PS_MERGE10:
                lo = _ps_extract1(fa_bits, ops)
                hi = _ps_extract0(fb_bits, ops)
            else:
                lo = _ps_extract1(fa_bits, ops)
                hi = _ps_extract1(fb_bits, ops)
            result = _ps_merge(lo, hi, ops)
            state = state.with_fpr(fd, result)
            return state
        elif op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU,
                     Opcode.PSQ_LX, Opcode.PSQ_LUX, Opcode.PSQ_STX, Opcode.PSQ_STUX):
            if op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU):
                rs, ra, disp, w, i = a
                address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], ops.const(disp))
            else:
                rs, ra, rb, w, i = a
                address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], state.gpr[rb])
            is_psq_load = op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LX, Opcode.PSQ_LUX)
            if is_psq_load:
                state = _touch_memory(state, address, 8, ops)
                lo_raw = _load(state.memory, address, 4, ops)
                hi_raw = _load(state.memory, ops.add(address, ops.const(4)), 4, ops)
                lo_s = ops.fp_bits_to_double(ops.bor(ops.shl(lo_raw, ops.const(32)), ops.const(0)))
                hi_s = ops.fp_bits_to_double(ops.bor(ops.shl(hi_raw, ops.const(32)), ops.const(0)))
                result = _ps_merge(lo_s, hi_s, ops)
                state = state.with_fpr(rs, result)
            else:
                fpr_bits = state.fpr[rs]
                lo_bits = ops.band(fpr_bits, ops.const(0xFFFFFFFF))
                hi_bits = ops.band(ops.lshr(fpr_bits, ops.const(32)), ops.const(0xFFFFFFFF))
                state = _touch_memory(state, address, 8, ops)
                state = replace(state, memory=_store(state.memory, address, lo_bits, 4, ops))
                state = replace(state, memory=_store(state.memory, ops.add(address, ops.const(4)), hi_bits, 4, ops))
            update = op in (Opcode.PSQ_LU, Opcode.PSQ_STU, Opcode.PSQ_LUX, Opcode.PSQ_STUX)
            if update:
                state = state.with_gpr(ra, address)
            return state
        else:
            raise UnsupportedInstruction(insn.address, insn.raw, f"semantics not implemented for {op.value}")

        if is_single and op not in (Opcode.FRSP, Opcode.FNEG, Opcode.FNABS, Opcode.FABS, Opcode.FMR):
            d = ops.fp_round_to_single(rm, d)
        if op in _FP_VALUE_ARITH:
            finite = lambda value: ops.lnot(ops.lor(ops.fp_is_nan(value), ops.fp_is_inf(value)))
            if op in (Opcode.FADDS, Opcode.FSUBS, Opcode.FDIVS, Opcode.FADD, Opcode.FSUB, Opcode.FDIV):
                inputs_finite = ops.land(finite(op_fa), finite(op_fb))
            elif op in (Opcode.FMULS, Opcode.FMUL):
                inputs_finite = ops.land(finite(op_fa), finite(op_fc))
            else:
                inputs_finite = finite(op_fb)
            state = replace(state, valid=ops.land(state.valid, ops.land(inputs_finite, finite(d))))
        d_bits = ops.fp_double_to_bits(d)
        state = _fpscr_set_fprf(state.with_fpr(fd, d_bits), d_bits, ops)
        if op in (Opcode.FMULS, Opcode.FMUL):
            state = state.with_fpscr(ops.band(state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR))))
        if insn.record:
            # Rc copies FPSCR[FX,FEX,VX,OX] into CR1; it does not classify the
            # arithmetic result.
            cr1 = _fpscr_cr1(state, ops)
            state = _set_cr_field(state, 1, cr1, ops)
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
    if op in FP_D_LOADS or op in FP_X_LOADS:
        writes.add(f"f{a[0]}")
        if a[1]: reads.add(f"r{a[1]}")
        if op in INDEXED_MEMORY: reads.add(f"r{a[2]}")
        if op in FP_UPDATES: writes.add(f"r{a[1]}")
        reads.add("memory")
    elif op in FP_D_STORES or op in FP_X_STORES:
        reads.add(f"f{a[0]}")
        if a[1]: reads.add(f"r{a[1]}")
        if op in INDEXED_MEMORY: reads.add(f"r{a[2]}")
        if op in FP_UPDATES: writes.add(f"r{a[1]}")
        writes.add("memory")
    elif op in (_FP_SCALAR_ARITH | _FP_PS_ARITH):
        writes.add(f"f{a[0]}")
        for idx in a[1:]:
            if 0 <= idx < 32: reads.add(f"f{idx}")
        if insn.record: writes.add("cr1")
    elif op in _FP_PS_CMP_MERGE:
        writes.add(f"f{a[0]}")
        for idx in a[1:]:
            if 0 <= idx < 32: reads.add(f"f{idx}")
    elif op in (Opcode.FCMPU, Opcode.FCMPO):
        for idx in a[1:]:
            if 0 <= idx < 32: reads.add(f"f{idx}")
        reads.add("fpscr")
        writes.add(f"cr{a[0]}")
        writes.add("fpscr")
    elif op in (Opcode.PS_CMPU0, Opcode.PS_CMPO0, Opcode.PS_CMPU1, Opcode.PS_CMPO1):
        for idx in a[1:]:
            if 0 <= idx < 32: reads.add(f"f{idx}")
        writes.add(f"cr{a[0]}")
    elif op in (Opcode.MFFS, Opcode.FCTIW, Opcode.FCTIWZ, Opcode.FRSP, Opcode.FNEG,
                 Opcode.FMR, Opcode.FNABS, Opcode.FABS):
        reads.add("fpscr" if op == Opcode.MFFS else f"f{a[1]}" if len(a) > 1 else "")
        writes.add(f"f{a[0]}")
        if insn.record: writes.add("cr1")
    elif op in (Opcode.MTFSF, Opcode.MTFSFI, Opcode.MTFSB0, Opcode.MTFSB1):
        reads.add("fpscr")
        if op == Opcode.MTFSF: reads.add(f"f{a[1]}")
        writes.add("fpscr")
        if insn.record: writes.add("cr1")
    elif op == Opcode.MCRFS:
        reads.add("fpscr")
        writes.add("fpscr")
        writes.add(f"cr{a[0]}")
    elif op in _FP_PSQ_OPS:
        is_load = op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LX, Opcode.PSQ_LUX)
        if is_load: writes.add(f"f{a[0]}")
        else: reads.add(f"f{a[0]}")
        if a[1]: reads.add(f"r{a[1]}")
        if len(a) > 2 and not isinstance(a[2], bool): reads.add(f"r{a[2]}")
        if is_load: reads.add("memory")
        else: writes.add("memory")
        if op in (Opcode.PSQ_LU, Opcode.PSQ_STU, Opcode.PSQ_LUX, Opcode.PSQ_STUX):
            writes.add(f"r{a[1]}")
    elif op in LOADS:
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
        elif op in FP_D_LOADS or op in FP_X_LOADS:
            written.add(f"f{a[0]}")
            if op in FP_UPDATES: written.add(f"r{a[1]}")
        elif op in FP_D_STORES or op in FP_X_STORES:
            written.add("memory")
            if op in FP_UPDATES: written.add(f"r{a[1]}")
        elif op in (Opcode.FCMPU, Opcode.FCMPO, Opcode.PS_CMPU0, Opcode.PS_CMPO0,
                     Opcode.PS_CMPU1, Opcode.PS_CMPO1):
            written.add(f"cr{a[0]}")
            written.add("fpscr")
        elif op in (_FP_SCALAR_ARITH | _FP_PS_ARITH | _FP_PS_CMP_MERGE):
            written.add(f"f{a[0]}")
            if op not in (Opcode.FSEL, Opcode.FNEG, Opcode.FMR, Opcode.FNABS, Opcode.FABS):
                written.add("fpscr")
        elif op in (Opcode.FCTIW, Opcode.FCTIWZ, Opcode.FRSP, Opcode.FNEG, Opcode.FMR,
                     Opcode.FNABS, Opcode.FABS, Opcode.MFFS):
            written.add(f"f{a[0]}")
        elif op in (Opcode.MTFSF, Opcode.MTFSFI, Opcode.MTFSB0, Opcode.MTFSB1, Opcode.MCRFS):
            written.add("fpscr")
            if op == Opcode.MCRFS: written.add(f"cr{a[0]}")
        elif op in _FP_PSQ_OPS:
            if op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LX, Opcode.PSQ_LUX):
                written.add(f"f{a[0]}")
            else:
                written.add("memory")
            if op in (Opcode.PSQ_LU, Opcode.PSQ_STU, Opcode.PSQ_LUX, Opcode.PSQ_STUX):
                written.add(f"r{a[1]}")
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
        if insn.record:
            written.add("cr1" if op in (_FP_SCALAR_ARITH | _FP_PS_ARITH) else "cr0")
        elif op in (Opcode.ADDIC_DOT, Opcode.ANDI_DOT, Opcode.ANDIS_DOT):
            written.add("cr0")
    order = [*(f"r{i}" for i in range(32)), *(f"f{i}" for i in range(32)), "cr", *(f"cr{i}" for i in range(8)), "fpscr", "xer.ca", "xer.ov", "xer.so", "lr", "ctr", "memory"]
    return tuple(item for item in order if item in written)


def read_gprs(instructions: list[Instruction]) -> set[int]:
    result: set[int] = set()
    for insn in instructions:
        reads, _ = register_effects(insn)
        result |= {int(name[1:]) for name in reads if name.startswith("r") and name[1:].isdigit()}
    return result

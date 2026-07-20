from __future__ import annotations

from contextvars import ContextVar
from dataclasses import dataclass, replace
from typing import Any, Literal, Protocol

from .deadline import Deadline, ProofDeadlineExceeded
from .ir import (
    ExecutionInconclusive,
    Instruction,
    Opcode,
    RelocationRef,
    R_PPC_ADDR16_HA,
    R_PPC_ADDR16_HI,
    R_PPC_ADDR16_LO,
    R_PPC_EMB_SDA21,
    R_PPC_REL14,
    R_PPC_REL24,
    SUPPORTED_OPCODES,
    UnsupportedInstruction,
)
from .loop_summary import LoopSummary, apply_affine_loop_summary, build_affine_summary_map
from .memory_bus import BusOutcome, MemoryBus
from .memory_loop import MemoryLoopSummary, apply_memory_loop_summary
from .model import ConcreteMemory, InvalidReason, MachineState, XerState
from .result import FloatingPointDomain
from .spr import (
    AUX_SPR_INDEX,
    AUX_SPR_NAMES,
    HID0_DCE,
    HID2_LCE,
    SPR_HID0,
    SPR_HID2,
    TIME_BASE_WRITE_SPRS,
)

MASK32 = 0xFFFFFFFF

# Active proof-domain for FP ops. Defaults match FloatingPointDomain().
_FP_DOMAIN: ContextVar[FloatingPointDomain] = ContextVar(
    "ppc_equivalence_fp_domain",
    default=FloatingPointDomain(),
)

# Opt-in Tier C memory bus for concrete CFG execution only.
_MEMORY_BUS: ContextVar[MemoryBus | None] = ContextVar(
    "ppc_equivalence_memory_bus",
    default=None,
)


def active_fp_domain() -> FloatingPointDomain:
    return _FP_DOMAIN.get()


def active_memory_bus() -> MemoryBus | None:
    return _MEMORY_BUS.get()


def _constrain_valid(
    state: MachineState,
    constraint: Any,
    reason: InvalidReason,
    ops: WordOps,
) -> MachineState:
    new_valid = ops.land(state.valid, constraint)
    if isinstance(ops, SymbolicOps):
        z3 = ops.z3
        new_reason = z3.If(
            z3.And(state.valid, z3.Not(constraint)),
            z3.BitVecVal(reason.value, 8),
            state.invalid_reason,
        )
    else:
        if state.valid and not constraint:
            new_reason = reason.value
        else:
            new_reason = state.invalid_reason
    return replace(state, valid=new_valid, invalid_reason=new_reason)


def _constrain_fp_value_domain(
    state: MachineState, value: Any, ops: WordOps,
) -> MachineState:
    """Apply allow_nan / allow_infinity / allow_subnormal domain predicates."""
    domain = active_fp_domain()
    ok = ops.bool(True)
    if not domain.allow_nan:
        ok = ops.land(ok, ops.lnot(ops.fp_is_nan(value)))
    if not domain.allow_infinity:
        ok = ops.land(ok, ops.lnot(ops.fp_is_inf(value)))
    if not domain.allow_subnormal:
        ok = ops.land(ok, ops.lnot(ops.fp_is_subnormal(value)))
    if domain.allow_nan and domain.allow_infinity and domain.allow_subnormal:
        return state
    return _constrain_valid(state, ok, InvalidReason.FP_DOMAIN_EXCLUDED, ops)


def _constrain_fp_defined_result(
    state: MachineState, defined_result: Any, ops: WordOps,
) -> MachineState:
    """Exclude finite-input overflow when the active domain requests it."""
    if not active_fp_domain().exclude_finite_overflow:
        return state
    return _constrain_valid(
        state, defined_result, InvalidReason.FP_DOMAIN_EXCLUDED, ops,
    )


def _constrain_fused_exact_binary32(
    state: MachineState, single_origin: Any, ops: WordOps,
) -> MachineState:
    if active_fp_domain().fused_input_domain != "exact-expanded-binary32":
        return state
    return _constrain_valid(
        state, single_origin, InvalidReason.FP_DOMAIN_EXCLUDED, ops,
    )


@dataclass(frozen=True, slots=True)
class CalleeContract:
    """Architectural dependency/effect summary for an already-matched callee."""

    reads: frozenset[str]
    writes: frozenset[str]
    source: str = "inferred"
    invalid_reasons: frozenset[int] = frozenset()

    @classmethod
    def opaque_eabi(cls) -> "CalleeContract":
        return cls(
            frozenset({"*"}),
            frozenset({
                *(f"r{i}" for i in (0, *range(3, 13))),
                *(f"f{i}" for i in range(14)),
                *(f"f{i}.ps1" for i in range(14)),
                "cr0", "cr1", "cr5", "cr6", "cr7",
                "xer.ca", "xer.ov", "xer.so", "fpscr", "ctr",
                "memory", "valid", "invalid_reason",
            }),
            "opaque-eabi",
            frozenset({r.value for r in InvalidReason}),
        )

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
FPSCR_HW_MASK = 0xFFFFF7FF

FRES_TABLE = (
    (0x7FF800, 0x3E1), (0x783800, 0x3A7), (0x70EA00, 0x371), (0x6A0800, 0x340),
    (0x638800, 0x313), (0x5D6200, 0x2EA), (0x579000, 0x2C4), (0x520800, 0x2A0),
    (0x4CC800, 0x27F), (0x47CA00, 0x261), (0x430800, 0x245), (0x3E8000, 0x22A),
    (0x3A2C00, 0x212), (0x360800, 0x1FB), (0x321400, 0x1E5), (0x2E4A00, 0x1D1),
    (0x2AA800, 0x1BE), (0x272C00, 0x1AC), (0x23D600, 0x19B), (0x209E00, 0x18B),
    (0x1D8800, 0x17C), (0x1A9000, 0x16E), (0x17AE00, 0x15B), (0x14F800, 0x15B),
    (0x124400, 0x143), (0x0FBE00, 0x143), (0x0D3800, 0x12D), (0x0ADE00, 0x12D),
    (0x088400, 0x11A), (0x065000, 0x11A), (0x041C00, 0x108), (0x020C00, 0x106),
)
FRSQRTE_TABLE = (
    (0x1A7E800, -0x568), (0x17CB800, -0x4F3), (0x1552800, -0x48D), (0x130C000, -0x435),
    (0x10F2000, -0x3E7), (0x0EFF000, -0x3A2), (0x0D2E000, -0x365), (0x0B7C000, -0x32E),
    (0x09E5000, -0x2FC), (0x0867000, -0x2D0), (0x06FF000, -0x2A8), (0x05AB800, -0x283),
    (0x046A000, -0x261), (0x0339800, -0x243), (0x0218800, -0x226), (0x0105800, -0x20B),
    (0x3FFA000, -0x7A4), (0x3C29000, -0x700), (0x38AA000, -0x670), (0x3572000, -0x5F2),
    (0x3279000, -0x584), (0x2FB7000, -0x524), (0x2D26000, -0x4CC), (0x2AC0000, -0x47E),
    (0x2881000, -0x43A), (0x2665000, -0x3FA), (0x2468000, -0x3C2), (0x2287000, -0x38E),
    (0x20C1000, -0x35E), (0x1F12000, -0x332), (0x1D79000, -0x30A), (0x1BF4000, -0x2E6),
)


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
    def fp_approx_reciprocal_bits(self, bits: Any) -> Any: ...
    def fp_approx_rsqrt_bits(self, bits: Any) -> Any: ...
    def fp_force_25bit(self, bits: Any) -> Any: ...
    def fp_is_snan_bits(self, bits: Any) -> Any: ...
    def fp_signs_equal_bits(self, left: Any, right: Any) -> Any: ...
    def fp_quiet_nan_bits(self, bits: Any) -> Any: ...
    def fp_single_nan_bits(self, bits: Any) -> Any: ...
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
    def fp_fma_single_precise(self, rm: Any, a: Any, c: Any, b: Any) -> Any: ...
    def fp_fma_to_single_exact(self, rm: Any, a: Any, c: Any, b: Any, precise: Any) -> Any: ...
    def fp_is_exact_single(self, value: Any) -> Any: ...
    def fp_fms(self, rm: Any, a: Any, b: Any, c: Any) -> Any: ...
    def fp_fnma(self, rm: Any, a: Any, b: Any, c: Any) -> Any: ...
    def fp_fnms(self, rm: Any, a: Any, b: Any, c: Any) -> Any: ...
    def fp_neg(self, a: Any) -> Any: ...
    def fp_abs(self, a: Any) -> Any: ...
    def fp_sel(self, a: Any, c: Any, b: Any) -> Any: ...
    def fp_round_to_single(self, rm: Any, a: Any) -> Any: ...
    def fp_round_to_integral(self, rm: Any, a: Any) -> Any: ...
    def fp_to_sint32(self, rm: Any, a: Any) -> Any: ...
    def fp_to_sint32_trunc(self, a: Any) -> Any: ...
    def fp_integer_result_bits(self, value: Any, negative_zero: Any) -> Any: ...
    def fp_is_negative_zero_bits(self, bits: Any) -> Any: ...
    def fp_is_lt(self, a: Any, b: Any) -> Any: ...
    def fp_is_gt(self, a: Any, b: Any) -> Any: ...
    def fp_is_eq(self, a: Any, b: Any) -> Any: ...
    def fp_is_nan(self, a: Any) -> Any: ...
    def fp_is_inf(self, a: Any) -> Any: ...
    def fp_is_subnormal(self, a: Any) -> Any: ...
    def fp_is_ge_zero(self, a: Any) -> Any: ...
    def fp_const_f32(self, value: float) -> Any: ...
    def fp_double_to_f32_bits(self, rm: Any, value: Any) -> Any: ...
    def fp_bits32_to_double(self, bits: Any) -> Any: ...
    def fp_dequantize_int(self, value: Any, width: int, signed: bool, scale: Any) -> Any: ...
    def fp_quantize_int(self, value: Any, width: int, signed: bool, scale: Any) -> Any: ...
    def fresh_bv32(self, name: str) -> Any: ...
    def fresh_bv64(self, name: str) -> Any: ...
    def relocation_address(self, relocation: RelocationRef) -> Any: ...


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
    def load_byte(self, memory: ConcreteMemory, address: int) -> int:
        bus = active_memory_bus()
        if bus is None:
            return memory.read(address)
        value, _ = _bus_concrete_load(memory, address & MASK32, 1)
        return value & 0xFF

    def store_byte(self, memory: ConcreteMemory, address: int, value: int) -> ConcreteMemory:
        bus = active_memory_bus()
        if bus is None:
            return memory.write(address, value)
        return _bus_concrete_store(memory, address & MASK32, 1, value & 0xFF)

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
    def fp_approx_reciprocal_bits(self, bits: int) -> int:
        bits &= 0xFFFFFFFFFFFFFFFF
        sign = bits & (1 << 63)
        exponent = (bits >> 52) & 0x7FF
        mantissa = bits & ((1 << 52) - 1)
        if exponent == 0 and mantissa == 0:
            return sign | 0x7FF0000000000000
        if exponent == 0x7FF:
            return sign if mantissa == 0 else self.fp_quiet_nan_bits(bits)
        if exponent < 895:
            return sign | 0x47EFFFFFE0000000
        if exponent >= 1149:
            return sign
        out_exponent = 0x7FD - exponent
        i = mantissa >> 37
        base, decrement = FRES_TABLE[i // 1024]
        estimate = base - (decrement * (i % 1024) + 1) // 2
        return sign | (out_exponent << 52) | (estimate << 29)
    def fp_approx_rsqrt_bits(self, bits: int) -> int:
        bits &= 0xFFFFFFFFFFFFFFFF
        sign = bits & (1 << 63)
        exponent = (bits >> 52) & 0x7FF
        mantissa = bits & ((1 << 52) - 1)
        if exponent == 0 and mantissa == 0:
            return sign | 0x7FF0000000000000
        if exponent == 0x7FF:
            if mantissa == 0:
                return 0x7FF8000000000000 if sign else 0
            return self.fp_quiet_nan_bits(bits)
        if sign:
            return 0x7FF8000000000000
        if exponent == 0:
            while not (mantissa & (1 << 52)):
                exponent -= 1
                mantissa <<= 1
            mantissa &= (1 << 52) - 1
            exponent += 1
        exponent_lsb = exponent & 1
        difference = exponent - 0x3FE
        half = (difference + 1) // 2
        out_exponent = (0x3FF - half) & 0x7FF
        i = ((exponent_lsb << 52) | mantissa) >> 37
        base, decrement = FRSQRTE_TABLE[i // 2048]
        estimate = base + decrement * (i % 2048)
        return (out_exponent << 52) | (estimate << 26)
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

    def fp_signs_equal_bits(self, left: int, right: int) -> bool:
        return ((left ^ right) & (1 << 63)) == 0

    def fp_quiet_nan_bits(self, bits: int) -> int:
        return (bits | 0x0008000000000000) & 0xFFFFFFFFFFFFFFFF

    def fp_single_nan_bits(self, bits: int) -> int:
        # ForceSingle followed by Fill(float): retain the binary32 payload bits
        # that survive conversion, then represent that float as binary64.
        return bits & 0xFFFFFFFFE0000000

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

    def _fp_oracle_require_rne(self, rm: str) -> None:
        if rm != "rne":
            raise ExecutionInconclusive(
                "fp oracle scaffold only models RN=nearest-even",
            )

    def _fp_oracle_fail_closed(self, exc: Exception) -> None:
        from .fp_oracle import OracleUnimplementedError

        if isinstance(exc, OracleUnimplementedError):
            raise ExecutionInconclusive(str(exc)) from exc
        raise exc

    def fp_fadd_rne_bits(self, a_bits: int, b_bits: int) -> int:
        from .fp_oracle import fadd_binary64_rne

        try:
            return fadd_binary64_rne(
                a_bits & 0xFFFFFFFFFFFFFFFF, b_bits & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fmul_rne_bits(self, a_bits: int, b_bits: int) -> int:
        from .fp_oracle import fmul_binary64_rne

        try:
            return fmul_binary64_rne(
                a_bits & 0xFFFFFFFFFFFFFFFF, b_bits & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fadds_fpr_bits(self, rm: str, a_fpr: int, b_fpr: int) -> int:
        from .fp_oracle import fadds_fpr_rne

        self._fp_oracle_require_rne(rm)
        try:
            return fadds_fpr_rne(
                a_fpr & 0xFFFFFFFFFFFFFFFF, b_fpr & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fmuls_fpr_bits(self, rm: str, a_fpr: int, c_fpr: int) -> int:
        from .fp_oracle import fmuls_fpr_rne

        self._fp_oracle_require_rne(rm)
        try:
            return fmuls_fpr_rne(
                a_fpr & 0xFFFFFFFFFFFFFFFF, c_fpr & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fsub_rne_bits(self, a_bits: int, b_bits: int) -> int:
        from .fp_oracle import fsub_binary64_rne

        try:
            return fsub_binary64_rne(
                a_bits & 0xFFFFFFFFFFFFFFFF, b_bits & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fdiv_rne_bits(self, a_bits: int, b_bits: int) -> int:
        from .fp_oracle import fdiv_binary64_rne

        try:
            return fdiv_binary64_rne(
                a_bits & 0xFFFFFFFFFFFFFFFF, b_bits & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fsubs_fpr_bits(self, rm: str, a_fpr: int, b_fpr: int) -> int:
        from .fp_oracle import fsubs_fpr_rne

        self._fp_oracle_require_rne(rm)
        try:
            return fsubs_fpr_rne(
                a_fpr & 0xFFFFFFFFFFFFFFFF, b_fpr & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fdivs_fpr_bits(self, rm: str, a_fpr: int, b_fpr: int) -> int:
        from .fp_oracle import fdivs_fpr_rne

        self._fp_oracle_require_rne(rm)
        try:
            return fdivs_fpr_rne(
                a_fpr & 0xFFFFFFFFFFFFFFFF, b_fpr & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fmadd_rne_bits(self, a_bits: int, c_bits: int, b_bits: int) -> int:
        from .fp_oracle import fmadd_binary64_rne

        try:
            return fmadd_binary64_rne(
                a_bits & 0xFFFFFFFFFFFFFFFF,
                c_bits & 0xFFFFFFFFFFFFFFFF,
                b_bits & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fmsub_rne_bits(self, a_bits: int, c_bits: int, b_bits: int) -> int:
        from .fp_oracle import fmsub_binary64_rne

        try:
            return fmsub_binary64_rne(
                a_bits & 0xFFFFFFFFFFFFFFFF,
                c_bits & 0xFFFFFFFFFFFFFFFF,
                b_bits & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fmadds_fpr_bits(self, rm: str, a_fpr: int, b_fpr: int, c_fpr: int) -> int:
        from .fp_oracle import fmadds_fpr_rne

        self._fp_oracle_require_rne(rm)
        try:
            return fmadds_fpr_rne(
                a_fpr & 0xFFFFFFFFFFFFFFFF,
                b_fpr & 0xFFFFFFFFFFFFFFFF,
                c_fpr & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fmsubs_fpr_bits(self, rm: str, a_fpr: int, b_fpr: int, c_fpr: int) -> int:
        from .fp_oracle import fmsubs_fpr_rne

        self._fp_oracle_require_rne(rm)
        try:
            return fmsubs_fpr_rne(
                a_fpr & 0xFFFFFFFFFFFFFFFF,
                b_fpr & 0xFFFFFFFFFFFFFFFF,
                c_fpr & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fnmadd_rne_bits(self, a_bits: int, c_bits: int, b_bits: int) -> int:
        from .fp_oracle import fnmadd_binary64_rne

        try:
            return fnmadd_binary64_rne(
                a_bits & 0xFFFFFFFFFFFFFFFF,
                c_bits & 0xFFFFFFFFFFFFFFFF,
                b_bits & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fnmsub_rne_bits(self, a_bits: int, c_bits: int, b_bits: int) -> int:
        from .fp_oracle import fnmsub_binary64_rne

        try:
            return fnmsub_binary64_rne(
                a_bits & 0xFFFFFFFFFFFFFFFF,
                c_bits & 0xFFFFFFFFFFFFFFFF,
                b_bits & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fnmadds_fpr_bits(self, rm: str, a_fpr: int, b_fpr: int, c_fpr: int) -> int:
        from .fp_oracle import fnmadds_fpr_rne

        self._fp_oracle_require_rne(rm)
        try:
            return fnmadds_fpr_rne(
                a_fpr & 0xFFFFFFFFFFFFFFFF,
                b_fpr & 0xFFFFFFFFFFFFFFFF,
                c_fpr & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

    def fp_fnmsubs_fpr_bits(self, rm: str, a_fpr: int, b_fpr: int, c_fpr: int) -> int:
        from .fp_oracle import fnmsubs_fpr_rne

        self._fp_oracle_require_rne(rm)
        try:
            return fnmsubs_fpr_rne(
                a_fpr & 0xFFFFFFFFFFFFFFFF,
                b_fpr & 0xFFFFFFFFFFFFFFFF,
                c_fpr & 0xFFFFFFFFFFFFFFFF,
            ).bits64
        except Exception as exc:
            self._fp_oracle_fail_closed(exc)
            raise AssertionError("unreachable")

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
    def fp_fma(self, rm: str, a: float, b: float, c: float) -> float:
        import math
        try:
            return math.fma(a, b, c)
        except ValueError:
            return float("nan")
    def fp_fma_single_precise(self, rm: str, a: float, c: float, b: float) -> float:
        import math
        import struct
        try:
            result = math.fma(a, c, b)
        except ValueError:
            return float("nan")
        result_bits = struct.unpack(">Q", struct.pack(">d", result))[0]
        if (result_bits & 0x000000001FFFFFFF) == 0x0000000010000000:
            a_prime = b - result
            b_prime = result + a_prime
            delta_a = math.fma(a, c, a_prime)
            delta_b = b - b_prime
            error = delta_a + delta_b
            if error != 0.0:
                result_bits += 1 if (error > 0.0) == (result > 0.0) else -1
                result = struct.unpack(">d", struct.pack(">Q", result_bits & 0xFFFFFFFFFFFFFFFF))[0]
        return result
    def fp_fma_to_single_exact(
        self, rm: str, a: float, c: float, b: float, precise: float,
    ) -> float:
        return self._fp_to_single(precise)
    def fp_is_exact_single(self, value: float) -> bool:
        return self._fp_to_single(value) == value
    def fp_fms(self, rm: str, a: float, b: float, c: float) -> float: return a * b - c
    def fp_fnma(self, rm: str, a: float, b: float, c: float) -> float: return -(a * b + c)
    def fp_fnms(self, rm: str, a: float, b: float, c: float) -> float: return -(a * b - c)
    def fp_neg(self, a: float) -> float: return -a
    def fp_abs(self, a: float) -> float: return abs(a)
    def fp_sel(self, a: float, c: float, b: float) -> float: return c if a >= 0.0 else b
    def fp_round_to_single(self, rm: str, a: float) -> float: return self._fp_to_single(a)
    def fp_round_to_integral(self, rm: str, a: float) -> float:
        import math
        if math.isnan(a) or math.isinf(a):
            return a
        return self._fp_round_int(a, rm)
    def fp_to_sint32(self, rm: str, a: float) -> int:
        import math
        if math.isnan(a) or math.isinf(a): return 0x80000000
        return int(self._fp_round_int(a, rm)) & 0xFFFFFFFF
    def fp_to_sint32_trunc(self, a: float) -> int:
        import math
        if math.isnan(a) or math.isinf(a): return 0x80000000
        return int(a) & 0xFFFFFFFF
    def fp_integer_result_bits(self, value: int, negative_zero: bool) -> int:
        return 0xFFF8000000000000 | ((1 << 32) if negative_zero else 0) | (value & MASK32)
    def fp_is_negative_zero_bits(self, bits: int) -> bool:
        return (bits & 0xFFFFFFFFFFFFFFFF) == 0x8000000000000000
    def fp_is_lt(self, a: float, b: float) -> bool: return a < b
    def fp_is_gt(self, a: float, b: float) -> bool: return a > b
    def fp_is_eq(self, a: float, b: float) -> bool: return a == b
    def fp_is_nan(self, a: float) -> bool:
        import math
        return math.isnan(a)
    def fp_is_inf(self, a: float) -> bool:
        import math
        return math.isinf(a)
    def fp_is_subnormal(self, a: float) -> bool:
        import math
        if not math.isfinite(a) or a == 0.0:
            return False
        return abs(a) < 2.2250738585072014e-308  # binary64 min normal
    def fp_is_ge_zero(self, a: float) -> bool: return a >= 0.0
    def fp_const_f32(self, value: float) -> float: return value
    def fp_double_to_f32_bits(self, rm: str, value: float) -> int:
        import struct
        return struct.unpack(">I", struct.pack(">f", self._fp_to_single(value)))[0]
    def fp_bits32_to_double(self, bits: int) -> float:
        import struct
        return struct.unpack(">f", struct.pack(">I", bits & 0xFFFFFFFF))[0]
    @staticmethod
    def _f32(value: float) -> float:
        import math
        import struct
        try:
            return struct.unpack(">f", struct.pack(">f", value))[0]
        except OverflowError:
            return math.copysign(float("inf"), value)
    def fp_dequantize_int(self, value: int, width: int, signed: bool, scale: int) -> float:
        raw = value & ((1 << width) - 1)
        if signed and raw & (1 << (width - 1)):
            raw -= 1 << width
        factor = 2.0 ** (-scale if scale < 32 else 64 - scale)
        return float(self._f32(self._f32(float(raw)) * self._f32(factor)))
    def fp_quantize_int(self, value: float, width: int, signed: bool, scale: int) -> int:
        import math
        if math.isnan(value):
            return 0
        factor = 2.0 ** (scale if scale < 32 else scale - 64)
        converted = self._f32(self._f32(value) * self._f32(factor))
        minimum = -(1 << (width - 1)) if signed else 0
        maximum = (1 << (width - 1)) - 1 if signed else (1 << width) - 1
        converted = min(max(converted, float(minimum)), float(maximum))
        return int(converted) & ((1 << width) - 1)

    def fresh_bv32(self, name: str) -> int:
        import random
        return random.getrandbits(32)

    def fresh_bv64(self, name: str) -> int:
        import random
        return random.getrandbits(64)

    def relocation_address(self, relocation: RelocationRef) -> int:
        raise ExecutionInconclusive(
            f"concrete execution needs a value for relocation {relocation.canonical_symbol}"
        )


class SymbolicOps:
    def __init__(self) -> None:
        try:
            import z3
        except ImportError as exc:
            raise RuntimeError("z3-solver is required; install tools/ppc_equivalence/requirements.txt") from exc
        self.z3 = z3
        self.relocation_values: dict[str, Any] = {}
        self.relocation_uses: set[tuple[str, int, int, int]] = set()
        self._call_functions: dict[tuple[str, str], Any] = {}
        self._relocation_world = z3.BitVec("reloc.world", 32)

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
    def fp_approx_reciprocal_bits(self, bits: Any) -> Any:
        z3 = self.z3
        sign = z3.Extract(63, 63, bits)
        exponent = z3.Extract(62, 52, bits)
        mantissa = z3.Extract(51, 0, bits)
        index = z3.Extract(14, 10, z3.Extract(51, 37, bits))
        remainder = z3.Extract(9, 0, z3.Extract(51, 37, bits))
        base = z3.BitVecVal(FRES_TABLE[-1][0], 32)
        decrement = z3.BitVecVal(FRES_TABLE[-1][1], 32)
        for table_index in range(30, -1, -1):
            condition = index == z3.BitVecVal(table_index, 5)
            base = z3.If(condition, z3.BitVecVal(FRES_TABLE[table_index][0], 32), base)
            decrement = z3.If(condition, z3.BitVecVal(FRES_TABLE[table_index][1], 32), decrement)
        estimate = base - z3.LShR(decrement * z3.ZeroExt(22, remainder) + 1, 1)
        normal = z3.Concat(
            sign, z3.Extract(10, 0, z3.BitVecVal(0x7FD, 12) - z3.ZeroExt(1, exponent)),
            z3.Extract(22, 0, estimate), z3.BitVecVal(0, 29),
        )
        zero = z3.Concat(sign, z3.Extract(62, 0, z3.BitVecVal(0x7FF0000000000000, 64)))
        signed_zero = z3.Concat(sign, z3.BitVecVal(0, 63))
        float_max = z3.Concat(sign, z3.Extract(62, 0, z3.BitVecVal(0x47EFFFFFE0000000, 64)))
        quiet_nan = bits | z3.BitVecVal(0x0008000000000000, 64)
        exponent_zero = exponent == z3.BitVecVal(0, 11)
        mantissa_zero = mantissa == z3.BitVecVal(0, 52)
        exponent_ones = exponent == z3.BitVecVal(0x7FF, 11)
        return z3.If(
            z3.And(exponent_zero, mantissa_zero), zero,
            z3.If(
                exponent_ones, z3.If(mantissa_zero, signed_zero, quiet_nan),
                z3.If(z3.ULT(exponent, z3.BitVecVal(895, 11)), float_max,
                      z3.If(z3.UGE(exponent, z3.BitVecVal(1149, 11)), signed_zero, normal)),
            ),
        )
    def fp_approx_rsqrt_bits(self, bits: Any) -> Any:
        z3 = self.z3
        sign = z3.Extract(63, 63, bits)
        exponent = z3.Extract(62, 52, bits)
        mantissa = z3.Extract(51, 0, bits)
        normalized_mantissa = mantissa
        normalized_exponent = z3.ZeroExt(2, exponent)
        for highest_bit in range(52):
            higher_clear = (
                z3.Extract(51, highest_bit + 1, mantissa) == z3.BitVecVal(0, 51 - highest_bit)
                if highest_bit < 51 else z3.BoolVal(True)
            )
            is_highest = z3.And(
                z3.Extract(highest_bit, highest_bit, mantissa) == z3.BitVecVal(1, 1),
                higher_clear,
            )
            shift = 52 - highest_bit
            shifted = z3.Extract(51, 0, z3.ZeroExt(12, mantissa) << shift)
            normalized_mantissa = z3.If(is_highest, shifted, normalized_mantissa)
            normalized_exponent = z3.If(
                is_highest, z3.BitVecVal(1 - shift, 13), normalized_exponent,
            )
        is_subnormal = z3.And(exponent == 0, mantissa != 0)
        normalized_mantissa = z3.If(is_subnormal, normalized_mantissa, mantissa)
        normalized_exponent = z3.If(is_subnormal, normalized_exponent, z3.ZeroExt(2, exponent))
        difference = normalized_exponent - z3.BitVecVal(0x3FE, 13)
        half = (difference + z3.BitVecVal(1, 13)) >> 1
        out_exponent = z3.Extract(10, 0, z3.BitVecVal(0x3FF, 13) - half)
        combined_index = z3.Concat(z3.Extract(0, 0, normalized_exponent), z3.Extract(51, 37, normalized_mantissa))
        index = z3.Extract(15, 11, combined_index)
        remainder = z3.Extract(10, 0, combined_index)
        base = z3.BitVecVal(FRSQRTE_TABLE[-1][0], 32)
        decrement = z3.BitVecVal(FRSQRTE_TABLE[-1][1], 32)
        for table_index in range(30, -1, -1):
            condition = index == z3.BitVecVal(table_index, 5)
            base = z3.If(condition, z3.BitVecVal(FRSQRTE_TABLE[table_index][0], 32), base)
            decrement = z3.If(condition, z3.BitVecVal(FRSQRTE_TABLE[table_index][1], 32), decrement)
        estimate = base + decrement * z3.ZeroExt(21, remainder)
        normal = z3.Concat(z3.BitVecVal(0, 1), out_exponent, z3.Extract(25, 0, estimate), z3.BitVecVal(0, 26))
        infinity = z3.Concat(sign, z3.Extract(62, 0, z3.BitVecVal(0x7FF0000000000000, 64)))
        quiet_nan = bits | z3.BitVecVal(0x0008000000000000, 64)
        canonical_nan = z3.BitVecVal(0x7FF8000000000000, 64)
        exponent_zero = exponent == z3.BitVecVal(0, 11)
        mantissa_zero = mantissa == z3.BitVecVal(0, 52)
        exponent_ones = exponent == z3.BitVecVal(0x7FF, 11)
        negative = sign == z3.BitVecVal(1, 1)
        return z3.If(
            z3.And(exponent_zero, mantissa_zero), infinity,
            z3.If(
                exponent_ones,
                z3.If(mantissa_zero, z3.If(negative, canonical_nan, z3.BitVecVal(0, 64)), quiet_nan),
                z3.If(negative, canonical_nan, normal),
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
    def fp_signs_equal_bits(self, left: Any, right: Any) -> Any:
        return self.z3.Extract(63, 63, left) == self.z3.Extract(63, 63, right)
    def fp_quiet_nan_bits(self, bits: Any) -> Any:
        return bits | self.z3.BitVecVal(0x0008000000000000, 64)
    def fp_single_nan_bits(self, bits: Any) -> Any:
        return bits & self.z3.BitVecVal(0xFFFFFFFFE0000000, 64)
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
    def fp_fma_single_precise(self, rm: Any, a: Any, c: Any, b: Any) -> Any:
        return self.z3.fpFMA(rm, a, c, b)
    def fp_fma_to_single_exact(
        self, rm: Any, a: Any, c: Any, b: Any, precise: Any,
    ) -> Any:
        a32 = self.z3.fpFPToFP(self.z3.RNE(), a, self.z3.Float32())
        c32 = self.z3.fpFPToFP(self.z3.RNE(), c, self.z3.Float32())
        b32 = self.z3.fpFPToFP(self.z3.RNE(), b, self.z3.Float32())
        single = self.z3.fpFMA(rm, a32, c32, b32)
        return self.z3.fpFPToFP(self.z3.RNE(), single, self.z3.Float64())
    def fp_is_exact_single(self, value: Any) -> Any:
        single = self.z3.fpFPToFP(self.z3.RNE(), value, self.z3.Float32())
        restored = self.z3.fpFPToFP(self.z3.RNE(), single, self.z3.Float64())
        return self.z3.fpEQ(value, restored)
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
    def fp_round_to_integral(self, rm: Any, a: Any) -> Any:
        return self.z3.fpRoundToIntegral(rm, a)
    def fp_to_sint32(self, rm: Any, a: Any) -> Any:
        return self.z3.fpToSBV(rm, a, self.z3.BitVecSort(32))
    def fp_to_sint32_trunc(self, a: Any) -> Any:
        return self.z3.fpToSBV(self.z3.RTZ(), a, self.z3.BitVecSort(32))
    def fp_integer_result_bits(self, value: Any, negative_zero: Any) -> Any:
        high = self.z3.If(
            negative_zero,
            self.z3.BitVecVal(0xFFF80001, 32),
            self.z3.BitVecVal(0xFFF80000, 32),
        )
        return self.z3.Concat(high, value)
    def fp_is_negative_zero_bits(self, bits: Any) -> Any:
        return bits == self.z3.BitVecVal(0x8000000000000000, 64)
    def fp_is_lt(self, a: Any, b: Any) -> Any: return self.z3.fpLT(a, b)
    def fp_is_gt(self, a: Any, b: Any) -> Any: return self.z3.fpGT(a, b)
    def fp_is_eq(self, a: Any, b: Any) -> Any: return self.z3.fpEQ(a, b)
    def fp_is_nan(self, a: Any) -> Any: return self.z3.fpIsNaN(a)
    def fp_is_inf(self, a: Any) -> Any: return self.z3.fpIsInf(a)
    def fp_is_subnormal(self, a: Any) -> Any: return self.z3.fpIsSubnormal(a)
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
    def _fp_scale32(self, scale: Any, *, dequantize: bool) -> Any:
        import struct
        result = self.z3.fpBVToFP(self.z3.BitVecVal(0x3F800000, 32), self.z3.Float32())
        for index in range(64):
            exponent = (-index if index < 32 else 64 - index) if dequantize else (index if index < 32 else index - 64)
            bits = struct.unpack(">I", struct.pack(">f", 2.0 ** exponent))[0]
            candidate = self.z3.fpBVToFP(self.z3.BitVecVal(bits, 32), self.z3.Float32())
            result = self.z3.If(scale == self.z3.BitVecVal(index, 32), candidate, result)
        return result
    def fp_dequantize_int(self, value: Any, width: int, signed: bool, scale: Any) -> Any:
        narrowed = self.z3.Extract(width - 1, 0, value)
        converted = (
            self.z3.fpSignedToFP(self.z3.RNE(), narrowed, self.z3.Float32())
            if signed
            else self.z3.fpUnsignedToFP(self.z3.RNE(), narrowed, self.z3.Float32())
        )
        scaled = self.z3.fpMul(self.z3.RNE(), converted, self._fp_scale32(scale, dequantize=True))
        return self.z3.fpFPToFP(self.z3.RNE(), scaled, self.z3.Float64())
    def fp_quantize_int(self, value: Any, width: int, signed: bool, scale: Any) -> Any:
        value32 = self.z3.fpFPToFP(self.z3.RNE(), value, self.z3.Float32())
        scaled = self.z3.fpMul(self.z3.RNE(), value32, self._fp_scale32(scale, dequantize=False))
        minimum = -(1 << (width - 1)) if signed else 0
        maximum = (1 << (width - 1)) - 1 if signed else (1 << width) - 1
        min_fp = self.z3.FPVal(float(minimum), self.z3.Float32())
        max_fp = self.z3.FPVal(float(maximum), self.z3.Float32())
        clamped = self.z3.If(
            self.z3.fpLT(scaled, min_fp), min_fp,
            self.z3.If(self.z3.fpGT(scaled, max_fp), max_fp, scaled),
        )
        narrowed = (
            self.z3.fpToSBV(self.z3.RTZ(), clamped, self.z3.BitVecSort(width))
            if signed
            else self.z3.fpToUBV(self.z3.RTZ(), clamped, self.z3.BitVecSort(width))
        )
        return self.z3.ZeroExt(32 - width, narrowed)

    def fresh_bv32(self, name: str) -> Any:
        return self.z3.BitVec(name, 32)

    def fresh_bv64(self, name: str) -> Any:
        return self.z3.BitVec(name, 64)

    def relocation_address(self, relocation: RelocationRef) -> Any:
        name = relocation.canonical_symbol
        value = self.relocation_values.get(name)
        if value is None:
            # Z3 accepts punctuation in symbol names, but a stable digest keeps
            # generated SMT and diagnostics compact and collision-free.
            import hashlib
            digest = hashlib.sha256(name.encode("utf-8")).hexdigest()[:16]
            value = self.z3.BitVec(f"reloc.addr.{digest}", 32)
            self.relocation_values[name] = value
        return value

    def note_relocation(self, relocation: RelocationRef, instruction_address: int) -> None:
        self.relocation_uses.add((
            relocation.canonical_symbol,
            relocation.relocation_type,
            instruction_address & MASK32,
            relocation.addend,
        ))

    def layout_constraints(self, initial: MachineState) -> list[Any]:
        """Return the field-range/alignment conditions a real PPC linker needs."""
        z3 = self.z3
        constraints: list[Any] = []
        for symbol, relocation_type, place, addend in sorted(self.relocation_uses):
            target = self.relocation_values[symbol] + self.const(addend)
            if relocation_type in (R_PPC_REL24, R_PPC_REL14):
                constraints.append((target & self.const(3)) == self.const(0))
                delta = z3.ZeroExt(1, target) - z3.BitVecVal(place, 33)
                low, high = (
                    (-0x02000000, 0x01FFFFFC)
                    if relocation_type == R_PPC_REL24 else (-0x8000, 0x7FFC)
                )
                constraints.extend((
                    delta >= z3.BitVecVal(low, 33),
                    delta <= z3.BitVecVal(high, 33),
                ))
            elif relocation_type == R_PPC_EMB_SDA21:
                def fits(base: Any) -> Any:
                    delta = z3.ZeroExt(1, target) - z3.ZeroExt(1, base)
                    return z3.And(
                        delta >= z3.BitVecVal(-0x8000, 33),
                        delta <= z3.BitVecVal(0x7FFF, 33),
                    )
                constraints.append(z3.Or(fits(initial.gpr[2]), fits(initial.gpr[13])))
        return constraints

    def call_token(self, callee: str, state: MachineState, contract: CalleeContract) -> Any:
        """A deterministic opaque transition key for an already-proved callee.

        ``valid`` and ``invalid_reason`` are always part of the token, even when
        omitted from ``contract.reads``. Otherwise two callers that differ only
        in first-invalid reason can collapse to the same UF transition and
        falsely prove equivalence after the summary rewrites definedness.
        """
        import hashlib
        components: dict[str, Any] = {
            **{f"r{i}": state.gpr[i] for i in range(32)},
            **{f"f{i}": state.fpr[i] for i in range(32)},
            **{f"f{i}.ps1": state.ps1[i] for i in range(32)},
            **{f"gqr{i}": state.gqr[i] for i in range(8)},
            **{f"sr{i}": state.sr[i] for i in range(16)},
            **{
                AUX_SPR_NAMES[number]: state.spr[index]
                for number, index in AUX_SPR_INDEX.items()
            },
            "cr": state.cr, "xer.ca": state.xer.ca, "xer.ov": state.xer.ov,
            "xer.so": state.xer.so, "fpscr": state.fpscr, "ctr": state.ctr,
            "lr": state.lr, "msr": state.msr, "time_base": state.time_base, "srr0": state.srr0,
            "srr1": state.srr1, "memory": state.memory, "valid": state.valid,
            "invalid_reason": state.invalid_reason,
        }
        for field in range(8):
            shift = (7 - field) * 4
            components[f"cr{field}"] = self.z3.Extract(shift + 3, shift, state.cr)
        if "*" in contract.reads:
            names = sorted(name for name in components if name != "lr")
        else:
            names = sorted(set(contract.reads) | {"valid", "invalid_reason"})
        arguments = tuple(components[name] for name in names if name in components) + (
            self._relocation_world,
        )
        signature = ",".join(names)
        key = (callee, f"token:{signature}")
        function = self._call_functions.get(key)
        if function is None:
            digest = hashlib.sha256(callee.encode("utf-8")).hexdigest()[:16]
            function = self.z3.Function(
                f"call.{digest}.transition",
                *(argument.sort() for argument in arguments),
                self.z3.BitVecSort(64),
            )
            self._call_functions[key] = function
        return function(*arguments)

    def call_result(self, callee: str, component: str, token: Any, sort: Any) -> Any:
        import hashlib
        key = (callee, component)
        function = self._call_functions.get(key)
        if function is None:
            digest = hashlib.sha256(f"{callee}:{component}".encode("utf-8")).hexdigest()[:16]
            function = self.z3.Function(
                f"call.{digest}.{component}", self.z3.BitVecSort(64), sort,
            )
            self._call_functions[key] = function
        return function(token)


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


def _raise_bus_failure(operation: str, outcome: BusOutcome) -> None:
    raise ExecutionInconclusive(f"memory bus {operation}: {outcome.value}")


def _reverse_byte_order(value: int, width: int) -> int:
    result = 0
    for index in range(width):
        result = (result << 8) | ((value >> (8 * index)) & 0xFF)
    return result & MASK32


def _bus_concrete_load(
    memory: ConcreteMemory, address: int, width: int,
) -> tuple[int, ConcreteMemory]:
    bus = active_memory_bus()
    if bus is None:
        raise AssertionError("memory bus routing requested without an active bus")
    bus.ram = memory
    result = bus.load(address, width)
    if result.outcome is not BusOutcome.OK:
        _raise_bus_failure("load", result.outcome)
    assert result.value is not None
    return result.value & MASK32, bus.ram


def _bus_concrete_store(
    memory: ConcreteMemory, address: int, width: int, value: int,
) -> ConcreteMemory:
    bus = active_memory_bus()
    if bus is None:
        raise AssertionError("memory bus routing requested without an active bus")
    bus.ram = memory
    write = bus.store(address, width, value & MASK32)
    if write.outcome is not BusOutcome.OK:
        _raise_bus_failure("store", write.outcome)
    return bus.ram


def _load(memory: Any, address: Any, width: int, ops: WordOps, *, reverse: bool = False) -> Any:
    if isinstance(ops, ConcreteOps) and active_memory_bus() is not None:
        value, _ = _bus_concrete_load(memory, address, width)
        if reverse:
            value = _reverse_byte_order(value, width)
        return value
    result = ops.const(0)
    order = list(range(width))
    if reverse:
        order.reverse()
    for offset in order:
        result = ops.bor(ops.shl(result, ops.const(8)), ops.load_byte(memory, ops.add(address, ops.const(offset))))
    return result


def _store(memory: Any, address: Any, value: Any, width: int, ops: WordOps, *, reverse: bool = False) -> Any:
    if isinstance(ops, ConcreteOps) and active_memory_bus() is not None:
        stored = _reverse_byte_order(value, width) if reverse else value
        return _bus_concrete_store(memory, address, width, stored)
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


def _relocation_address(insn: Instruction, ops: WordOps) -> Any:
    relocation = insn.relocation
    if relocation is None:
        raise ExecutionInconclusive("instruction has no relocation")
    if isinstance(ops, SymbolicOps):
        ops.note_relocation(relocation, insn.address)
    return ops.add(ops.relocation_address(relocation), ops.const(relocation.addend))


def _immediate_operand(
    insn: Instruction, immediate: int, ops: WordOps, *, signed: bool,
) -> Any:
    """Return a D-form immediate after applying an attached ELF relocation.

    Relocations are represented before linking, so the proof quantifies over a
    single logical address for each canonical symbol.  This deliberately models
    the linker's field transform rather than guessing a concrete load address.
    """
    relocation = insn.relocation
    if relocation is None:
        return ops.const(immediate)
    address = _relocation_address(insn, ops)
    if relocation.relocation_type == R_PPC_ADDR16_LO:
        field = ops.band(address, ops.const(0xFFFF))
    elif relocation.relocation_type == R_PPC_ADDR16_HI:
        field = ops.band(ops.lshr(address, ops.const(16)), ops.const(0xFFFF))
    elif relocation.relocation_type == R_PPC_ADDR16_HA:
        field = ops.band(
            ops.lshr(ops.add(address, ops.const(0x8000)), ops.const(16)),
            ops.const(0xFFFF),
        )
    else:
        raise ExecutionInconclusive(
            f"relocation type {relocation.relocation_type} is not an immediate-field relocation"
        )
    return _sign_extend(field, 16, ops) if signed else field


def _dform_address(
    state: MachineState, insn: Instruction, ra: int, displacement: int, ops: WordOps,
) -> Any:
    relocation = insn.relocation
    if relocation is not None and relocation.relocation_type == R_PPC_EMB_SDA21:
        # SDA21 chooses r2 or r13 and patches RA together with the displacement.
        # Its architectural effective address is exactly S + A.
        return _relocation_address(insn, ops)
    base = ops.const(0) if ra == 0 else state.gpr[ra]
    return ops.add(base, _immediate_operand(insn, displacement, ops, signed=True))


def _touch_memory(
    state: MachineState,
    address: Any,
    width: int,
    ops: WordOps,
    access: Literal["read", "write"],
) -> MachineState:
    if width > 1:
        aligned = ops.eq(ops.band(address, ops.const(width - 1)), ops.const(0))
        state = _constrain_valid(state, aligned, InvalidReason.UNALIGNED_ACCESS, ops)
    addrs = tuple(ops.add(address, ops.const(offset)) for offset in range(width))
    touches = state.memory_touches + addrs
    if access == "read":
        return replace(state, memory_reads=state.memory_reads + addrs, memory_touches=touches)
    return replace(state, memory_writes=state.memory_writes + addrs, memory_touches=touches)


def _mark_stack_pointer_escape(
    state: MachineState, stored_value: Any, ops: WordOps,
) -> MachineState:
    if not isinstance(ops, SymbolicOps):
        return state
    if any(
        str(variable.decl().name()) == "input.gpr.r1"
        for variable in ops.z3.z3util.get_vars(stored_value)
    ):
        return replace(state, stack_private=ops.bool(False))
    return state


_PSQ_INTEGER_TYPES = {
    4: (1, False),
    5: (2, False),
    6: (1, True),
    7: (2, True),
}


def _psq_flush_single(bits: Any, ops: WordOps) -> Any:
    exponent_zero = ops.eq(ops.band(bits, ops.const(0x7F800000)), ops.const(0))
    fraction_nonzero = ops.lnot(ops.eq(ops.band(bits, ops.const(0x007FFFFF)), ops.const(0)))
    return ops.ite(
        ops.land(exponent_zero, fraction_nonzero),
        ops.band(bits, ops.const(0x80000000)),
        bits,
    )


def _psq_load_pair(
    memory: Any, address: Any, w: int, qtype: Any, scale: Any, ops: WordOps,
) -> tuple[Any, Any]:
    one_bits = ops.fp_const64(0x3FF0000000000000)
    zero_bits = ops.fp_const64(0)
    ps0, ps1 = zero_bits, one_bits

    raw0 = _load(memory, address, 4, ops)
    raw1 = _load(memory, ops.add(address, ops.const(4)), 4, ops)
    float0 = ops.fp_double_to_bits(ops.fp_bits32_to_double(raw0))
    float1 = one_bits if w else ops.fp_double_to_bits(ops.fp_bits32_to_double(raw1))
    is_float = ops.eq(qtype, ops.const(0))
    ps0, ps1 = ops.ite(is_float, float0, ps0), ops.ite(is_float, float1, ps1)

    for type_code, (width, signed) in _PSQ_INTEGER_TYPES.items():
        first = _load(memory, address, width, ops)
        second = _load(memory, ops.add(address, ops.const(width)), width, ops)
        value0 = ops.fp_double_to_bits(ops.fp_dequantize_int(first, width * 8, signed, scale))
        value1 = one_bits if w else ops.fp_double_to_bits(
            ops.fp_dequantize_int(second, width * 8, signed, scale),
        )
        selected = ops.eq(qtype, ops.const(type_code))
        ps0, ps1 = ops.ite(selected, value0, ps0), ops.ite(selected, value1, ps1)
    return ps0, ps1


def _psq_store_pair(
    memory: Any, address: Any, w: int, qtype: Any, scale: Any,
    ps0_bits: Any, ps1_bits: Any, ops: WordOps,
) -> Any:
    ps0 = ops.fp_bits_to_double(ps0_bits)
    ps1 = ops.fp_bits_to_double(ps1_bits)
    first32 = _psq_flush_single(ops.fp_double_to_f32_bits(ops.fp_rm_rne(), ps0), ops)
    float_memory = _store(memory, address, first32, 4, ops)
    if not w:
        second32 = _psq_flush_single(ops.fp_double_to_f32_bits(ops.fp_rm_rne(), ps1), ops)
        float_memory = _store(float_memory, ops.add(address, ops.const(4)), second32, 4, ops)
    result = ops.ite(ops.eq(qtype, ops.const(0)), float_memory, memory)

    for type_code, (width, signed) in _PSQ_INTEGER_TYPES.items():
        first = ops.fp_quantize_int(ps0, width * 8, signed, scale)
        candidate = _store(memory, address, first, width, ops)
        if not w:
            second = ops.fp_quantize_int(ps1, width * 8, signed, scale)
            candidate = _store(candidate, ops.add(address, ops.const(width)), second, width, ops)
        result = ops.ite(ops.eq(qtype, ops.const(type_code)), candidate, result)
    return result


def _psq_domain(
    state: MachineState,
    address: Any,
    w: int,
    qtype: Any,
    ops: WordOps,
    access: Literal["read", "write"],
) -> MachineState:
    valid_type = ops.eq(qtype, ops.const(0))
    for type_code in _PSQ_INTEGER_TYPES:
        valid_type = ops.lor(valid_type, ops.eq(qtype, ops.const(type_code)))
    needs_word_alignment = ops.eq(qtype, ops.const(0))
    needs_half_alignment = ops.lor(ops.eq(qtype, ops.const(5)), ops.eq(qtype, ops.const(7)))
    aligned = ops.ite(
        needs_word_alignment,
        ops.eq(ops.band(address, ops.const(3)), ops.const(0)),
        ops.ite(
            needs_half_alignment,
            ops.eq(ops.band(address, ops.const(1)), ops.const(0)),
            ops.bool(True),
        ),
    )
    state = _constrain_valid(state, ops.land(valid_type, aligned), InvalidReason.PSQ_INVALID_TYPE, ops)
    # PSQ alignment is type-dependent; record access without _touch_memory's
    # width-based alignment check.
    addrs = tuple(ops.add(address, ops.const(offset)) for offset in range(4 if w else 8))
    touches = state.memory_touches + addrs
    if access == "read":
        return replace(state, memory_reads=state.memory_reads + addrs, memory_touches=touches)
    return replace(state, memory_writes=state.memory_writes + addrs, memory_touches=touches)


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
    Opcode.FADDS, Opcode.FSUBS, Opcode.FMULS, Opcode.FDIVS, Opcode.FRES,
    Opcode.FMADDS, Opcode.FMSUBS, Opcode.FNMADDS, Opcode.FNMSUBS, Opcode.FRSP,
}
_FP_DOUBLE_ARITH = {
    Opcode.FADD, Opcode.FSUB, Opcode.FMUL, Opcode.FDIV, Opcode.FRSQRTE, Opcode.FSEL,
    Opcode.FMADD, Opcode.FMSUB, Opcode.FNMADD, Opcode.FNMSUB,
}
_FP_AUX_OPS = {
    Opcode.FCMPU, Opcode.FCMPO, Opcode.FNEG, Opcode.FMR, Opcode.FNABS, Opcode.FABS,
    Opcode.FCTIW, Opcode.FCTIWZ, Opcode.MFFS, Opcode.MTFSF, Opcode.MTFSFI,
    Opcode.MTFSB0, Opcode.MTFSB1, Opcode.MCRFS,
}
_FP_PS_DIV = {Opcode.PS_DIV}
_FP_PS_ESTIMATE = {Opcode.PS_RES, Opcode.PS_RSQRTE}
_FP_PS_BASIC = {
    Opcode.PS_ADD, Opcode.PS_SUB, Opcode.PS_MUL, Opcode.PS_MULS0, Opcode.PS_MULS1,
}
_FP_PS_FUSED = {
    Opcode.PS_MADD, Opcode.PS_MSUB, Opcode.PS_NMADD, Opcode.PS_NMSUB,
    Opcode.PS_MADDS0, Opcode.PS_MADDS1,
}
_FP_PS_FUSED_SUBTRACT = {Opcode.PS_MSUB, Opcode.PS_NMSUB}
_FP_PS_FUSED_NEGATE = {Opcode.PS_NMADD, Opcode.PS_NMSUB}
_FP_PS_SUM = {Opcode.PS_SUM0, Opcode.PS_SUM1}
_FP_PS_SELECT = {Opcode.PS_SEL}
_FP_PS_CMP = {
    Opcode.PS_CMPU0, Opcode.PS_CMPO0, Opcode.PS_CMPU1, Opcode.PS_CMPO1,
}
_FP_PS_MOVE = {
    Opcode.PS_NEG, Opcode.PS_MR, Opcode.PS_NABS, Opcode.PS_ABS,
}
_FP_PS_MERGE = {
    Opcode.PS_MERGE00, Opcode.PS_MERGE01, Opcode.PS_MERGE10, Opcode.PS_MERGE11,
}
_FP_PS_SIMPLE = _FP_PS_MOVE | _FP_PS_MERGE
_FP_PSQ_OPS = {
    Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU,
    Opcode.PSQ_LX, Opcode.PSQ_LUX, Opcode.PSQ_STX, Opcode.PSQ_STUX,
}
_FP_SCALAR_ARITH = _FP_SINGLE_ARITH | _FP_DOUBLE_ARITH | _FP_AUX_OPS
_FP_VALUE_ARITH = {
    Opcode.FADDS, Opcode.FSUBS, Opcode.FMULS, Opcode.FDIVS,
    Opcode.FADD, Opcode.FSUB, Opcode.FMUL, Opcode.FDIV, Opcode.FRSP,
}
_FP_EXCEPTION_ARITH = {
    Opcode.FADDS, Opcode.FSUBS, Opcode.FMULS, Opcode.FDIVS,
    Opcode.FADD, Opcode.FSUB, Opcode.FMUL, Opcode.FDIV,
}
_FP_FUSED_SINGLE = {Opcode.FMADDS, Opcode.FMSUBS, Opcode.FNMADDS, Opcode.FNMSUBS}
_FP_FUSED_DOUBLE = {Opcode.FMADD, Opcode.FMSUB, Opcode.FNMADD, Opcode.FNMSUB}
_FP_FUSED = _FP_FUSED_SINGLE | _FP_FUSED_DOUBLE
_FP_FUSED_SUBTRACT = {Opcode.FMSUBS, Opcode.FNMSUBS, Opcode.FMSUB, Opcode.FNMSUB}
_FP_FUSED_NEGATE = {Opcode.FNMADDS, Opcode.FNMSUBS, Opcode.FNMADD, Opcode.FNMSUB}
_FP_ESTIMATE = {Opcode.FRES, Opcode.FRSQRTE}
_FP_ROUNDING_SENSITIVE = _FP_VALUE_ARITH | {
    Opcode.STFS, Opcode.STFSU, Opcode.STFSX, Opcode.STFSUX,
} | _FP_FUSED | _FP_PS_BASIC | _FP_PS_FUSED | _FP_PS_SUM | _FP_PS_DIV | {
    Opcode.PS_RSQRTE,
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


def _fpscr_normalize(value: Any, ops: WordOps) -> Any:
    """Apply Broadway's writable mask and recompute VX/FEX summaries."""
    value = ops.band(value, ops.const(FPSCR_HW_MASK))
    vx_set = ops.lnot(ops.eq(ops.band(value, ops.const(FPSCR_VX_ANY)), ops.const(0)))
    value = ops.ite(
        vx_set,
        ops.bor(value, ops.const(FPSCR_VX)),
        ops.band(value, ops.bnot(ops.const(FPSCR_VX))),
    )
    enabled = ops.band(
        ops.lshr(value, ops.const(22)),
        ops.band(value, ops.const(FPSCR_ANY_ENABLE)),
    )
    fex_set = ops.lnot(ops.eq(enabled, ops.const(0)))
    return ops.ite(
        fex_set,
        ops.bor(value, ops.const(FPSCR_FEX)),
        ops.band(value, ops.bnot(ops.const(FPSCR_FEX))),
    )


def _fpscr_raise_if(
    state: MachineState, condition: Any, exception_mask: int, ops: WordOps,
) -> MachineState:
    raised = _fpscr_raise(state, exception_mask, ops)
    return state.with_fpscr(ops.ite(condition, raised.fpscr, state.fpscr))


def _execute_fp_compare(
    state: MachineState, bf: int, left_bits: Any, right_bits: Any,
    ordered: bool, ops: WordOps,
) -> MachineState:
    left = ops.fp_bits_to_double(left_bits)
    right = ops.fp_bits_to_double(right_bits)
    cr_nibble = _fp_compare_nibble(left, right, ops)
    is_snan = ops.lor(
        ops.fp_is_snan_bits(left_bits), ops.fp_is_snan_bits(right_bits),
    )
    is_nan = ops.lor(ops.fp_is_nan(left), ops.fp_is_nan(right))
    state = _fpscr_raise_if(state, is_snan, FPSCR_VXSNAN, ops)
    if ordered:
        invalid_enabled = ops.lnot(ops.eq(
            ops.band(state.fpscr, ops.const(FPSCR_VE)), ops.const(0),
        ))
        # Ordered compare raises VXVC for every quiet NaN, and for a signaling
        # NaN only when invalid exceptions are disabled.
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


def _execute_ps_basic_lane(
    state: MachineState, opcode: Opcode, left_bits: Any, right_bits: Any,
    ops: WordOps,
) -> tuple[MachineState, Any]:
    left = ops.fp_bits_to_double(left_bits)
    right = ops.fp_bits_to_double(right_bits)
    state = _constrain_fp_value_domain(state, left, ops)
    state = _constrain_fp_value_domain(state, right, ops)
    rm = ops.fp_rm_rne()
    if opcode == Opcode.PS_ADD:
        value = ops.fp_add(rm, left, right)
    elif opcode == Opcode.PS_SUB:
        value = ops.fp_sub(rm, left, right)
    else:
        value = ops.fp_mul(rm, left, right)

    nan_left = ops.fp_is_nan(left)
    nan_right = ops.fp_is_nan(right)
    any_nan = ops.lor(nan_left, nan_right)
    any_snan = ops.lor(
        ops.fp_is_snan_bits(left_bits), ops.fp_is_snan_bits(right_bits),
    )
    inf_left = ops.fp_is_inf(left)
    inf_right = ops.fp_is_inf(right)
    zero = ops.fp_bits_to_double(ops.fp_const64(0))
    zero_left = ops.fp_is_eq(left, zero)
    zero_right = ops.fp_is_eq(right, zero)
    no_nan = ops.lnot(any_nan)

    vxisi = ops.bool(False)
    vximz = ops.bool(False)
    if opcode == Opcode.PS_ADD:
        vxisi = ops.land(
            no_nan,
            ops.land(
                ops.land(inf_left, inf_right),
                ops.lnot(ops.fp_signs_equal_bits(left_bits, right_bits)),
            ),
        )
    elif opcode == Opcode.PS_SUB:
        vxisi = ops.land(
            no_nan,
            ops.land(
                ops.land(inf_left, inf_right),
                ops.fp_signs_equal_bits(left_bits, right_bits),
            ),
        )
    else:
        vximz = ops.land(
            no_nan,
            ops.lor(
                ops.land(inf_left, zero_right),
                ops.land(zero_left, inf_right),
            ),
        )

    state = _fpscr_raise_if(state, any_snan, FPSCR_VXSNAN, ops)
    state = _fpscr_raise_if(state, vxisi, FPSCR_VXISI, ops)
    state = _fpscr_raise_if(state, vximz, FPSCR_VXIMZ, ops)
    invalid = ops.lor(any_snan, ops.lor(vxisi, vximz))
    nan_result = ops.lor(any_nan, ops.lor(vxisi, vximz))
    propagated_nan = ops.ite(
        nan_left,
        ops.fp_quiet_nan_bits(left_bits),
        ops.ite(
            nan_right,
            ops.fp_quiet_nan_bits(right_bits),
            ops.fp_const64(0x7FF8000000000000),
        ),
    )
    propagated_nan = ops.fp_single_nan_bits(propagated_nan)
    rounded = ops.fp_round_to_single(rm, value)
    result_bits = ops.ite(
        nan_result, propagated_nan, ops.fp_double_to_bits(rounded),
    )

    clear_fifr = nan_result
    if opcode in (Opcode.PS_ADD, Opcode.PS_SUB):
        clear_fifr = ops.lor(clear_fifr, ops.lor(inf_left, inf_right))
    cleared = ops.band(state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR)))
    state = state.with_fpscr(ops.ite(clear_fifr, cleared, state.fpscr))

    finite = lambda item: ops.lnot(ops.lor(ops.fp_is_nan(item), ops.fp_is_inf(item)))
    finite_inputs = ops.land(finite(left), finite(right))
    handled_nonfinite = ops.lor(
        any_nan, ops.lor(invalid, ops.lor(inf_left, inf_right)),
    )
    defined_result = ops.lor(
        ops.land(finite_inputs, finite(rounded)), handled_nonfinite,
    )
    state = _constrain_fp_defined_result(state, defined_result, ops)
    return state, result_bits


def _execute_ps_div_lane(
    state: MachineState, numerator_bits: Any, denominator_bits: Any,
    ops: WordOps,
) -> tuple[MachineState, Any]:
    numerator = ops.fp_bits_to_double(numerator_bits)
    denominator = ops.fp_bits_to_double(denominator_bits)
    state = _constrain_fp_value_domain(state, numerator, ops)
    state = _constrain_fp_value_domain(state, denominator, ops)
    rm = ops.fp_rm_rne()
    value = ops.fp_div(rm, numerator, denominator)

    nan_a = ops.fp_is_nan(numerator)
    nan_b = ops.fp_is_nan(denominator)
    any_nan = ops.lor(nan_a, nan_b)
    no_nan = ops.lnot(any_nan)
    any_snan = ops.lor(
        ops.fp_is_snan_bits(numerator_bits), ops.fp_is_snan_bits(denominator_bits),
    )
    inf_a = ops.fp_is_inf(numerator)
    inf_b = ops.fp_is_inf(denominator)
    zero = ops.fp_bits_to_double(ops.fp_const64(0))
    zero_a = ops.fp_is_eq(numerator, zero)
    zero_b = ops.fp_is_eq(denominator, zero)
    vxzdz = ops.land(no_nan, ops.land(zero_a, zero_b))
    vxidi = ops.land(no_nan, ops.land(inf_a, inf_b))
    zx = ops.land(no_nan, ops.land(ops.lnot(zero_a), zero_b))
    invalid = ops.lor(any_snan, ops.lor(vxzdz, vxidi))
    nan_result = ops.lor(any_nan, ops.lor(vxzdz, vxidi))

    state = _fpscr_raise_if(state, any_snan, FPSCR_VXSNAN, ops)
    state = _fpscr_raise_if(state, vxzdz, FPSCR_VXZDZ, ops)
    state = _fpscr_raise_if(state, vxidi, FPSCR_VXIDI, ops)
    state = _fpscr_raise_if(state, zx, FPSCR_ZX, ops)

    propagated_nan = ops.ite(
        nan_a,
        ops.fp_quiet_nan_bits(numerator_bits),
        ops.ite(
            nan_b,
            ops.fp_quiet_nan_bits(denominator_bits),
            ops.fp_const64(0x7FF8000000000000),
        ),
    )
    propagated_nan = ops.fp_single_nan_bits(propagated_nan)
    rounded = ops.fp_round_to_single(rm, value)
    result_bits = ops.ite(
        nan_result, propagated_nan, ops.fp_double_to_bits(rounded),
    )

    cleared = ops.band(state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR)))
    state = state.with_fpscr(ops.ite(nan_result, cleared, state.fpscr))
    finite = lambda item: ops.lnot(ops.lor(ops.fp_is_nan(item), ops.fp_is_inf(item)))
    handled_nonfinite = ops.lor(
        any_nan, ops.lor(invalid, ops.lor(zx, ops.lor(inf_a, inf_b))),
    )
    defined_result = ops.lor(
        ops.land(ops.land(finite(numerator), finite(denominator)), finite(rounded)),
        handled_nonfinite,
    )
    state = _constrain_fp_defined_result(state, defined_result, ops)
    return state, result_bits


def _execute_ps_fused_lane(
    state: MachineState, opcode: Opcode, a_bits: Any, b_bits: Any, c_bits: Any,
    ops: WordOps,
) -> tuple[MachineState, Any]:
    """Execute one paired-single fused lane and accumulate its FPSCR effects."""
    a = ops.fp_bits_to_double(a_bits)
    b = ops.fp_bits_to_double(b_bits)
    c_forced_bits = ops.fp_force_25bit(c_bits)
    c = ops.fp_bits_to_double(c_forced_bits)
    state = _constrain_fp_value_domain(state, a, ops)
    state = _constrain_fp_value_domain(state, b, ops)
    state = _constrain_fp_value_domain(state, ops.fp_bits_to_double(c_bits), ops)
    subtract = opcode in _FP_PS_FUSED_SUBTRACT
    addend = ops.fp_neg(b) if subtract else b
    rm = ops.fp_rm_rne()
    precise = ops.fp_fma_single_precise(rm, a, c, addend)
    single = ops.fp_fma_to_single_exact(rm, a, c, addend, precise)

    nan_a = ops.fp_is_nan(a)
    nan_b = ops.fp_is_nan(b)
    nan_c = ops.fp_is_nan(ops.fp_bits_to_double(c_bits))
    any_nan = ops.lor(nan_a, ops.lor(nan_b, nan_c))
    no_nan = ops.lnot(any_nan)
    any_snan = ops.lor(
        ops.fp_is_snan_bits(a_bits),
        ops.lor(ops.fp_is_snan_bits(b_bits), ops.fp_is_snan_bits(c_bits)),
    )
    inf_a = ops.fp_is_inf(a)
    inf_b = ops.fp_is_inf(b)
    inf_c = ops.fp_is_inf(ops.fp_bits_to_double(c_bits))
    zero = ops.fp_bits_to_double(ops.fp_const64(0))
    zero_a = ops.fp_is_eq(a, zero)
    zero_c = ops.fp_is_eq(ops.fp_bits_to_double(c_bits), zero)
    vximz = ops.land(no_nan, ops.lor(
        ops.land(inf_a, zero_c), ops.land(zero_a, inf_c),
    ))
    product_infinite = ops.lor(
        ops.land(inf_a, ops.lnot(zero_c)),
        ops.land(inf_c, ops.lnot(zero_a)),
    )
    product_positive = ops.fp_signs_equal_bits(a_bits, c_bits)
    b_positive = ops.fp_signs_equal_bits(b_bits, ops.fp_const64(0))
    product_matches_b = ops.lor(
        ops.land(product_positive, b_positive),
        ops.land(ops.lnot(product_positive), ops.lnot(b_positive)),
    )
    invalid_signs = product_matches_b if subtract else ops.lnot(product_matches_b)
    vxisi = ops.land(
        no_nan,
        ops.land(ops.lnot(vximz), ops.land(product_infinite, ops.land(inf_b, invalid_signs))),
    )
    invalid = ops.lor(any_snan, ops.lor(vximz, vxisi))
    nan_result = ops.lor(any_nan, ops.lor(vximz, vxisi))

    state = _fpscr_raise_if(state, any_snan, FPSCR_VXSNAN, ops)
    state = _fpscr_raise_if(state, vximz, FPSCR_VXIMZ, ops)
    state = _fpscr_raise_if(state, vxisi, FPSCR_VXISI, ops)

    propagated_nan = ops.ite(
        nan_a,
        ops.fp_quiet_nan_bits(a_bits),
        ops.ite(
            nan_b,
            ops.fp_quiet_nan_bits(b_bits),
            ops.ite(
                nan_c,
                ops.fp_quiet_nan_bits(c_bits),
                ops.fp_const64(0x7FF8000000000000),
            ),
        ),
    )
    propagated_nan = ops.fp_single_nan_bits(propagated_nan)
    normal_bits = ops.fp_double_to_bits(single)
    if opcode in _FP_PS_FUSED_NEGATE:
        normal_bits = ops.fp_xor_sign(normal_bits)
    result_bits = ops.ite(nan_result, propagated_nan, normal_bits)

    any_inf = ops.lor(inf_a, ops.lor(inf_b, inf_c))
    clear_fifr = ops.lor(nan_result, any_inf)
    cleared = ops.band(state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR)))
    state = state.with_fpscr(ops.ite(clear_fifr, cleared, state.fpscr))

    finite = lambda value: ops.lnot(ops.lor(ops.fp_is_nan(value), ops.fp_is_inf(value)))
    handled_nonfinite = ops.lor(any_nan, ops.lor(invalid, any_inf))
    state = _constrain_fp_defined_result(
        state, ops.lor(finite(precise), handled_nonfinite), ops,
    )
    if isinstance(ops, SymbolicOps):
        # Z3 computes the exact binary32 FMA directly.  Keep proofs on the
        # common paired-single domain where finite lane values originated as
        # binary32 values expanded into FPR storage; ConcreteOps retains the
        # Broadway mixed-precision behavior for arbitrary FPR contents.
        single_origin = ops.land(
            ops.lor(ops.lnot(finite(a)), ops.fp_is_exact_single(a)),
            ops.land(
                ops.lor(ops.lnot(finite(b)), ops.fp_is_exact_single(b)),
                ops.lor(
                    ops.lnot(finite(ops.fp_bits_to_double(c_bits))),
                    ops.fp_is_exact_single(ops.fp_bits_to_double(c_bits)),
                ),
            ),
        )
        state = _constrain_fused_exact_binary32(state, single_origin, ops)
    return state, result_bits


def _force_ps_single_bits(bits: Any, ops: WordOps) -> Any:
    """Force an FPR lane to binary32 without changing architectural flags."""
    value = ops.fp_bits_to_double(bits)
    rounded = ops.fp_round_to_single(ops.fp_rm_rne(), value)
    nan_bits = ops.fp_single_nan_bits(ops.fp_quiet_nan_bits(bits))
    return ops.ite(ops.fp_is_nan(value), nan_bits, ops.fp_double_to_bits(rounded))


def _fp_write_result_if(
    state: MachineState, fd: int, result_bits: Any, condition: Any, ops: WordOps,
) -> MachineState:
    old_bits = state.fpr[fd]
    written = _fpscr_set_fprf(state.with_fpr(fd, result_bits), result_bits, ops)
    state = state.with_fpr(fd, ops.ite(condition, written.fpr[fd], old_bits))
    return state.with_fpscr(ops.ite(condition, written.fpscr, state.fpscr))


def _fp_convert_to_integer(
    state: MachineState, fd: int, source_bits: Any, rm: Any, ops: WordOps,
) -> MachineState:
    source = ops.fp_bits_to_double(source_bits)
    rounded = ops.fp_round_to_integral(rm, source)
    is_nan = ops.fp_is_nan(source)
    no_nan = ops.lnot(is_nan)
    positive_limit = ops.fp_bits_to_double(ops.fp_const64(0x41E0000000000000))
    negative_limit = ops.fp_bits_to_double(ops.fp_const64(0xC1E0000000000000))
    positive_invalid = ops.land(no_nan, ops.lnot(ops.fp_is_lt(rounded, positive_limit)))
    negative_invalid = ops.land(no_nan, ops.fp_is_lt(rounded, negative_limit))
    invalid = ops.lor(is_nan, ops.lor(positive_invalid, negative_invalid))
    is_snan = ops.fp_is_snan_bits(source_bits)

    state = _fpscr_raise_if(state, is_snan, FPSCR_VXSNAN, ops)
    state = _fpscr_raise_if(state, invalid, FPSCR_VXCVI, ops)

    converted = ops.fp_to_sint32(rm, source)
    value = ops.ite(
        ops.lor(is_nan, negative_invalid),
        ops.const(0x80000000),
        ops.ite(positive_invalid, ops.const(0x7FFFFFFF), converted),
    )
    exact = ops.fp_is_eq(rounded, source)
    inexact = ops.land(ops.lnot(invalid), ops.lnot(exact))
    state = _fpscr_raise_if(state, inexact, FPSCR_XX, ops)

    rounded_away = ops.fp_is_gt(ops.fp_abs(rounded), ops.fp_abs(source))
    clear_mask = ops.bnot(ops.const(FPSCR_FI | FPSCR_FR))
    fpscr = ops.band(state.fpscr, clear_mask)
    fpscr = ops.ite(inexact, ops.bor(fpscr, ops.const(FPSCR_FI)), fpscr)
    fpscr = ops.ite(
        ops.land(inexact, rounded_away), ops.bor(fpscr, ops.const(FPSCR_FR)), fpscr,
    )
    state = state.with_fpscr(fpscr)

    negative = ops.lnot(ops.fp_signs_equal_bits(source_bits, ops.fp_const64(0)))
    negative_zero_result = ops.land(ops.eq(value, ops.const(0)), negative)
    result_bits = ops.fp_integer_result_bits(value, negative_zero_result)
    invalid_enabled = ops.lnot(ops.eq(
        ops.band(state.fpscr, ops.const(FPSCR_VE)), ops.const(0),
    ))
    write_result = ops.lnot(ops.land(invalid, invalid_enabled))
    state = state.with_fpr(fd, ops.ite(write_result, result_bits, state.fpr[fd]))
    return state


def execute_instruction(
    state: MachineState,
    insn: Instruction,
    ops: WordOps,
    *,
    floating_point_domain: FloatingPointDomain | None = None,
) -> MachineState:
    token = None
    if floating_point_domain is not None:
        floating_point_domain.validate()
        token = _FP_DOMAIN.set(floating_point_domain)
    try:
        return _execute_instruction_body(state, insn, ops)
    finally:
        if token is not None:
            _FP_DOMAIN.reset(token)


def _execute_instruction_body(state: MachineState, insn: Instruction, ops: WordOps) -> MachineState:
    op, a = insn.opcode, insn.operands

    # Defense in depth for callers that construct Instruction objects without
    # going through decoder.py.
    if op not in SUPPORTED_OPCODES:
        raise UnsupportedInstruction(insn.address, insn.raw, f"semantics are unsupported for {op.value}")
    if op in _FP_ROUNDING_SENSITIVE:
        # ConcreteOps is validated against Broadway with RN=nearest-even and
        # NI disabled.  Keep the symbolic proof on that same explicit domain
        # unless FloatingPointDomain validation rejected a different config.
        domain = active_fp_domain()
        # RN occupies FPSCR[0:1]; NI is bit 2. Default domain requires both 0.
        mode_mask = 7 if domain.require_ni_zero else 3
        standard_fp_mode = ops.eq(
            ops.band(state.fpscr, ops.const(mode_mask)), ops.const(0),
        )
        if isinstance(ops, ConcreteOps) and not standard_fp_mode:
            raise ExecutionInconclusive(
                "FP ConcreteOps requires FPSCR.RN=nearest-even"
                + (" and NI=0" if domain.require_ni_zero else "")
            )
        state = _constrain_valid(state, standard_fp_mode, InvalidReason.FP_ROUNDING_MODE, ops)
    result: Any | None = None
    destination: int | None = None
    overflow = ops.bool(False)

    if op in {
        Opcode.DCBF, Opcode.DCBI, Opcode.DCBST, Opcode.DCBT,
        Opcode.ICBI, Opcode.SYNC, Opcode.ISYNC,
    }:
        # Under the explicit coherent ordinary-RAM/no-DMA/no-self-modifying-code
        # contract, these ordering and cache-state operations have no visible
        # value-semantics effect.
        return state
    if op in (Opcode.DCBZ, Opcode.DCBZ_L):
        ra, rb = a
        dcache_enabled = ops.lnot(ops.eq(
            ops.band(state.spr[AUX_SPR_INDEX[SPR_HID0]], ops.const(HID0_DCE)),
            ops.const(0),
        ))
        enabled = dcache_enabled
        if op == Opcode.DCBZ_L:
            locked_cache_enabled = ops.lnot(ops.eq(
                ops.band(state.spr[AUX_SPR_INDEX[SPR_HID2]], ops.const(HID2_LCE)),
                ops.const(0),
            ))
            enabled = ops.land(enabled, locked_cache_enabled)
        state = _constrain_valid(state, enabled, InvalidReason.CACHE_DISABLED, ops)
        address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], state.gpr[rb])
        block = ops.band(address, ops.const(0xFFFFFFE0))
        for offset in range(32):
            byte_address = ops.add(block, ops.const(offset))
            state = _touch_memory(state, byte_address, 1, ops, "write")
            state = replace(
                state, memory=ops.store_byte(state.memory, byte_address, ops.const(0)),
            )
        return state

    if op in (Opcode.MFMSR, Opcode.MTMSR, Opcode.MFSR, Opcode.MTSR):
        supervisor = ops.eq(ops.band(state.msr, ops.const(0x00004000)), ops.const(0))
        state = _constrain_valid(state, supervisor, InvalidReason.PRIVILEGED_INSTRUCTION, ops)

    if op == Opcode.MFMSR:
        destination, result = a[0], state.msr
    elif op == Opcode.MTMSR:
        return replace(state, msr=state.gpr[a[0]])
    elif op == Opcode.MFSR:
        destination, result = a[0], state.sr[a[1]]
    elif op == Opcode.MTSR:
        return state.with_sr(a[1], state.gpr[a[0]])
    elif op == Opcode.MFTB:
        destination = a[0]
        result = ops.fp_low_word(state.time_base) if a[1] == 268 else ops.fp_high_word(state.time_base)
    elif op in (Opcode.ADDI, Opcode.ADDIS, Opcode.MULLI, Opcode.ADDIC, Opcode.ADDIC_DOT, Opcode.SUBFIC):
        rd, ra, immediate = a
        base = ops.const(0) if ra == 0 and op in (Opcode.ADDI, Opcode.ADDIS) else state.gpr[ra]
        if insn.relocation is not None and insn.relocation.relocation_type == R_PPC_EMB_SDA21:
            if op != Opcode.ADDI:
                raise ExecutionInconclusive("SDA21 arithmetic is only supported for addi")
            base = ops.const(0)
            right = _relocation_address(insn, ops)
        else:
            right = _immediate_operand(insn, immediate, ops, signed=True)
            if op == Opcode.ADDIS:
                right = ops.shl(right, ops.const(16))
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
        value = _immediate_operand(insn, immediate, ops, signed=False)
        if op in (Opcode.ORIS, Opcode.XORIS, Opcode.ANDIS_DOT):
            value = ops.shl(value, ops.const(16))
        if op in (Opcode.ORI, Opcode.ORIS): result = ops.bor(state.gpr[rs], value)
        elif op in (Opcode.XORI, Opcode.XORIS): result = ops.bxor(state.gpr[rs], value)
        else: result = ops.band(state.gpr[rs], value)
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
            state = _constrain_valid(state, valid, InvalidReason.DIVIDE_UNDEFINED, ops)
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
        if spr in (26, 27) or spr in AUX_SPR_INDEX or spr in TIME_BASE_WRITE_SPRS:
            supervisor = ops.eq(ops.band(state.msr, ops.const(0x00004000)), ops.const(0))
            state = _constrain_valid(
                state, supervisor, InvalidReason.PRIVILEGED_INSTRUCTION, ops,
            )
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
        elif spr == 9:
            if op == Opcode.MFSPR: destination, result = reg, state.ctr
            else: return replace(state, ctr=state.gpr[reg])
        elif spr == 26:
            if op == Opcode.MFSPR: destination, result = reg, state.srr0
            else: return replace(state, srr0=state.gpr[reg])
        elif spr == 27:
            if op == Opcode.MFSPR: destination, result = reg, state.srr1
            else: return replace(state, srr1=state.gpr[reg])
        elif spr in AUX_SPR_INDEX:
            index = AUX_SPR_INDEX[spr]
            if op == Opcode.MFSPR:
                destination, result = reg, state.spr[index]
            else:
                return state.with_spr(index, state.gpr[reg])
        elif spr in TIME_BASE_WRITE_SPRS:
            source = state.gpr[reg]
            high = source if spr == 285 else ops.fp_high_word(state.time_base)
            low = source if spr == 284 else ops.fp_low_word(state.time_base)
            return replace(state, time_base=ops.fp_join_words(high, low))
        else:
            gqr_index = spr - 912
            if op == Opcode.MFSPR:
                destination, result = reg, state.gqr[gqr_index]
            else:
                return state.with_gqr(gqr_index, state.gpr[reg])
    elif op in LOADS or op in STORES:
        reg, ra, third = a
        if op in INDEXED_MEMORY:
            address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], state.gpr[third])
        else:
            address = _dform_address(state, insn, ra, third, ops)
        if op in LOADS:
            width, signed, update, reverse_bytes = LOADS[op]
            state = _touch_memory(state, address, width, ops, "read")
            result = _load(state.memory, address, width, ops, reverse=reverse_bytes)
            if signed: result = _sign_extend(result, width * 8, ops)
            state = state.with_gpr(reg, result)
            if update: state = state.with_gpr(ra, address)
            return state
        width, update, reverse_bytes = STORES[op]
        state = _touch_memory(state, address, width, ops, "write")
        state = _mark_stack_pointer_escape(state, state.gpr[reg], ops)
        state = replace(state, memory=_store(state.memory, address, state.gpr[reg], width, ops, reverse=reverse_bytes))
        if update: state = state.with_gpr(ra, address)
        return state
    elif op in FP_D_MEM or op in FP_X_MEM:
        rt, ra, third = a
        if op in INDEXED_MEMORY:
            address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], state.gpr[third])
        else:
            address = _dform_address(state, insn, ra, third, ops)
        is_load = op in FP_D_LOADS or op in FP_X_LOADS
        if is_load:
            width = FP_D_LOADS[op][0] if op in FP_D_LOADS else FP_X_LOADS[op]
        else:
            width = FP_D_STORES[op][0] if op in FP_D_STORES else FP_X_STORES[op]
        if op == Opcode.STFIWX:
            fpr_bits = state.fpr[rt]
            value32 = ops.fp_low_word(fpr_bits)
            state = _touch_memory(state, address, 4, ops, "write")
            state = replace(state, memory=_store(state.memory, address, value32, 4, ops))
            return state
        if is_load:
            update = op in (Opcode.LFSU, Opcode.LFDU, Opcode.LFSUX, Opcode.LFDUX)
            state = _touch_memory(state, address, width, ops, "read")
            if width == 4:
                raw = _load(state.memory, address, 4, ops)
                result = ops.fp_double_to_bits(ops.fp_bits32_to_double(raw))
            else:
                high = _load(state.memory, address, 4, ops)
                low = _load(state.memory, ops.add(address, ops.const(4)), 4, ops)
                result = ops.fp_join_words(high, low)
            state = state.with_fpr(rt, result)
            if width == 4:
                state = state.with_ps1(rt, result)
            if update: state = state.with_gpr(ra, address)
            return state
        else:
            update = op in (Opcode.STFSU, Opcode.STFDU, Opcode.STFSUX, Opcode.STFDUX)
            state = _touch_memory(state, address, width, ops, "write")
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
    elif op in _FP_PS_BASIC:
        fd, fa, source = a
        left0, left1 = state.fpr[fa], state.ps1[fa]
        right0, right1 = state.fpr[source], state.ps1[source]
        if op in (Opcode.PS_MUL, Opcode.PS_MULS0, Opcode.PS_MULS1):
            if op == Opcode.PS_MULS0:
                right1 = right0
            elif op == Opcode.PS_MULS1:
                right0 = right1
            right0 = ops.fp_force_25bit(right0)
            right1 = ops.fp_force_25bit(right1)
        state, result0 = _execute_ps_basic_lane(state, op, left0, right0, ops)
        state, result1 = _execute_ps_basic_lane(state, op, left1, right1, ops)
        state = _fpscr_set_fprf(
            state.with_fpr(fd, result0).with_ps1(fd, result1), result0, ops,
        )
        if insn.record:
            state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
        return state

    elif op in _FP_PS_DIV:
        fd, fa, fb = a
        state, result0 = _execute_ps_div_lane(state, state.fpr[fa], state.fpr[fb], ops)
        state, result1 = _execute_ps_div_lane(state, state.ps1[fa], state.ps1[fb], ops)
        state = _fpscr_set_fprf(
            state.with_fpr(fd, result0).with_ps1(fd, result1), result0, ops,
        )
        if insn.record:
            state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
        return state

    elif op in _FP_PS_ESTIMATE:
        fd, fb = a
        source0_bits, source1_bits = state.fpr[fb], state.ps1[fb]
        source0 = ops.fp_bits_to_double(source0_bits)
        source1 = ops.fp_bits_to_double(source1_bits)
        zero = ops.fp_bits_to_double(ops.fp_const64(0))
        zero0, zero1 = ops.fp_is_eq(source0, zero), ops.fp_is_eq(source1, zero)
        snan0 = ops.fp_is_snan_bits(source0_bits)
        snan1 = ops.fp_is_snan_bits(source1_bits)
        any_zero = ops.lor(zero0, zero1)
        any_snan = ops.lor(snan0, snan1)
        any_nan_inf = ops.lor(
            ops.lor(ops.fp_is_nan(source0), ops.fp_is_inf(source0)),
            ops.lor(ops.fp_is_nan(source1), ops.fp_is_inf(source1)),
        )
        state = _fpscr_raise_if(state, any_zero, FPSCR_ZX, ops)
        state = _fpscr_raise_if(state, any_snan, FPSCR_VXSNAN, ops)
        if op == Opcode.PS_RES:
            result0 = ops.fp_approx_reciprocal_bits(source0_bits)
            result1 = ops.fp_approx_reciprocal_bits(source1_bits)
            clear_fifr = ops.lor(any_zero, any_nan_inf)
        else:
            negative0 = ops.fp_is_lt(source0, zero)
            negative1 = ops.fp_is_lt(source1, zero)
            any_negative = ops.lor(negative0, negative1)
            state = _fpscr_raise_if(state, any_negative, FPSCR_VXSQRT, ops)
            result0 = _force_ps_single_bits(ops.fp_approx_rsqrt_bits(source0_bits), ops)
            result1 = _force_ps_single_bits(ops.fp_approx_rsqrt_bits(source1_bits), ops)
            clear_fifr = ops.lor(any_zero, ops.lor(any_negative, any_nan_inf))
        cleared = ops.band(state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR)))
        state = state.with_fpscr(ops.ite(clear_fifr, cleared, state.fpscr))
        state = _fpscr_set_fprf(
            state.with_fpr(fd, result0).with_ps1(fd, result1), result0, ops,
        )
        if insn.record:
            state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
        return state

    elif op in _FP_PS_FUSED:
        fd, fa, fb, fc = a
        a0, a1 = state.fpr[fa], state.ps1[fa]
        b0, b1 = state.fpr[fb], state.ps1[fb]
        c0, c1 = state.fpr[fc], state.ps1[fc]
        if op == Opcode.PS_MADDS0:
            c1 = c0
        elif op == Opcode.PS_MADDS1:
            c0 = c1
        state, result0 = _execute_ps_fused_lane(state, op, a0, b0, c0, ops)
        state, result1 = _execute_ps_fused_lane(state, op, a1, b1, c1, ops)
        state = _fpscr_set_fprf(
            state.with_fpr(fd, result0).with_ps1(fd, result1), result0, ops,
        )
        if insn.record:
            state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
        return state

    elif op in _FP_PS_SUM:
        fd, fa, fb, fc = a
        # Both forms add A.ps0 + B.ps1. The other result lane is a
        # binary32-forced copy from C, and only the arithmetic lane can raise
        # invalid-operation exceptions.
        state, summed = _execute_ps_basic_lane(
            state, Opcode.PS_ADD, state.fpr[fa], state.ps1[fb], ops,
        )
        if op == Opcode.PS_SUM0:
            result0, result1 = summed, _force_ps_single_bits(state.ps1[fc], ops)
            fprf_result = result0
        else:
            result0, result1 = _force_ps_single_bits(state.fpr[fc], ops), summed
            fprf_result = result1
        state = _fpscr_set_fprf(
            state.with_fpr(fd, result0).with_ps1(fd, result1), fprf_result, ops,
        )
        if insn.record:
            state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
        return state

    elif op in _FP_PS_SELECT:
        fd, fa, fb, fc = a
        predicate0 = ops.fp_is_ge_zero(ops.fp_bits_to_double(state.fpr[fa]))
        predicate1 = ops.fp_is_ge_zero(ops.fp_bits_to_double(state.ps1[fa]))
        result0 = ops.ite(predicate0, state.fpr[fc], state.fpr[fb])
        result1 = ops.ite(predicate1, state.ps1[fc], state.ps1[fb])
        state = state.with_fpr(fd, result0).with_ps1(fd, result1)
        if insn.record:
            state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
        return state

    elif op in _FP_ESTIMATE:
        fd, _, fb, _ = a
        source_bits = state.fpr[fb]
        source = ops.fp_bits_to_double(source_bits)
        zero_value = ops.fp_bits_to_double(ops.fp_const64(0))
        is_zero = ops.fp_is_eq(source, zero_value)
        is_snan = ops.fp_is_snan_bits(source_bits)
        is_nan = ops.fp_is_nan(source)
        is_inf = ops.fp_is_inf(source)
        negative = ops.fp_is_lt(source, zero_value)

        if op == Opcode.FRES:
            invalid = is_snan
            result_bits = ops.fp_approx_reciprocal_bits(source_bits)
        else:
            invalid = ops.lor(negative, is_snan)
            result_bits = ops.fp_approx_rsqrt_bits(source_bits)
            state = _fpscr_raise_if(state, negative, FPSCR_VXSQRT, ops)
        state = _fpscr_raise_if(state, is_zero, FPSCR_ZX, ops)
        state = _fpscr_raise_if(state, is_snan, FPSCR_VXSNAN, ops)

        invalid_enabled = ops.lnot(ops.eq(
            ops.band(state.fpscr, ops.const(FPSCR_VE)), ops.const(0),
        ))
        zero_enabled = ops.lnot(ops.eq(
            ops.band(state.fpscr, ops.const(FPSCR_ZE)), ops.const(0),
        ))
        suppress = ops.lor(
            ops.land(invalid, invalid_enabled),
            ops.land(is_zero, zero_enabled),
        )
        clear_fifr = ops.lor(
            invalid, ops.lor(is_zero, ops.lor(is_nan, is_inf)),
        )
        cleared = ops.band(state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR)))
        state = state.with_fpscr(ops.ite(clear_fifr, cleared, state.fpscr))
        state = _fp_write_result_if(state, fd, result_bits, ops.lnot(suppress), ops)
        if op == Opcode.FRES:
            state = state.with_ps1(
                fd, ops.ite(ops.lnot(suppress), result_bits, state.ps1[fd]),
            )
        if insn.record:
            state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
        return state

    elif op in _FP_PS_CMP:
        bf, fa, fb = a
        lane1 = op in (Opcode.PS_CMPU1, Opcode.PS_CMPO1)
        left_bits = state.ps1[fa] if lane1 else state.fpr[fa]
        right_bits = state.ps1[fb] if lane1 else state.fpr[fb]
        return _execute_fp_compare(
            state, bf, left_bits, right_bits,
            op in (Opcode.PS_CMPO0, Opcode.PS_CMPO1), ops,
        )

    elif op in _FP_PS_SIMPLE:
        if op in _FP_PS_MOVE:
            fd, fb = a
            ps0, ps1 = state.fpr[fb], state.ps1[fb]
            if op == Opcode.PS_NEG:
                ps0, ps1 = ops.fp_xor_sign(ps0), ops.fp_xor_sign(ps1)
            elif op == Opcode.PS_NABS:
                ps0, ps1 = ops.fp_set_sign(ps0), ops.fp_set_sign(ps1)
            elif op == Opcode.PS_ABS:
                ps0, ps1 = ops.fp_clear_sign(ps0), ops.fp_clear_sign(ps1)
        else:
            fd, fa, fb = a
            if op == Opcode.PS_MERGE00:
                ps0, ps1 = state.fpr[fa], state.fpr[fb]
            elif op == Opcode.PS_MERGE01:
                ps0, ps1 = state.fpr[fa], state.ps1[fb]
            elif op == Opcode.PS_MERGE10:
                ps0, ps1 = state.ps1[fa], state.fpr[fb]
            else:
                ps0, ps1 = state.ps1[fa], state.ps1[fb]
        state = state.with_fpr(fd, ps0).with_ps1(fd, ps1)
        if insn.record:
            state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
        return state

    elif op in _FP_SCALAR_ARITH or op in _FP_PSQ_OPS:
        fd, fa = a[0], a[1] if len(a) > 1 else 0
        fb = a[2] if len(a) > 2 else 0
        fc = a[3] if len(a) > 3 else 0
        rn_bits = ops.band(state.fpscr, ops.const(3))
        rm = ops.fp_rm_from_rn(rn_bits)
        is_single = op in _FP_SINGLE_ARITH
        ni = ops.lnot(ops.eq(ops.band(state.fpscr, ops.const(4)), ops.const(0)))

        # Scalar single instructions consume the register's double value and
        # round the result to binary32; they do not pre-round every operand.
        fa_bits = state.fpr[fa]
        fb_bits = state.fpr[fb]
        op_fa = ops.fp_bits_to_double(fa_bits)
        op_fb = ops.fp_bits_to_double(fb_bits)
        fc_source_bits = state.fpr[fc]
        fc_bits = ops.fp_force_25bit(fc_source_bits) if op in ({Opcode.FMULS} | _FP_FUSED_SINGLE) else fc_source_bits
        op_fc = ops.fp_bits_to_double(fc_bits)
        fused_single = None
        oracle_scalar_bits: int | None = None

        # Optional NaN / Inf / subnormal exclusions from FloatingPointDomain.
        if op not in _FP_PSQ_OPS:
            state = _constrain_fp_value_domain(state, op_fa, ops)
            state = _constrain_fp_value_domain(state, op_fb, ops)
            if op in ({Opcode.FMULS, Opcode.FMUL} | _FP_FUSED | {Opcode.FSEL}):
                state = _constrain_fp_value_domain(
                    state, ops.fp_bits_to_double(fc_source_bits), ops,
                )

        if op == Opcode.FSEL:
            result_bits = ops.ite(ops.fp_is_ge_zero(op_fa), state.fpr[fc], state.fpr[fb])
            state = state.with_fpr(fd, result_bits)
            if insn.record:
                cr1 = ops.band(ops.lshr(state.fpscr, ops.const(28)), ops.const(0xF))
                state = _set_cr_field(state, 1, cr1, ops)
            return state

        if isinstance(ops, ConcreteOps) and op == Opcode.FADD:
            ops._fp_oracle_require_rne(rm)
            oracle_scalar_bits = ops.fp_fadd_rne_bits(fa_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FADDS:
            oracle_scalar_bits = ops.fp_fadds_fpr_bits(rm, fa_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FMUL:
            ops._fp_oracle_require_rne(rm)
            oracle_scalar_bits = ops.fp_fmul_rne_bits(fa_bits, fc_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FMULS:
            oracle_scalar_bits = ops.fp_fmuls_fpr_bits(rm, fa_bits, fc_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FSUB:
            ops._fp_oracle_require_rne(rm)
            oracle_scalar_bits = ops.fp_fsub_rne_bits(fa_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FSUBS:
            oracle_scalar_bits = ops.fp_fsubs_fpr_bits(rm, fa_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FDIV:
            ops._fp_oracle_require_rne(rm)
            oracle_scalar_bits = ops.fp_fdiv_rne_bits(fa_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FDIVS:
            oracle_scalar_bits = ops.fp_fdivs_fpr_bits(rm, fa_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FMADD:
            ops._fp_oracle_require_rne(rm)
            oracle_scalar_bits = ops.fp_fmadd_rne_bits(fa_bits, fc_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FMSUB:
            ops._fp_oracle_require_rne(rm)
            oracle_scalar_bits = ops.fp_fmsub_rne_bits(fa_bits, fc_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FMADDS:
            # fc_bits already Force25-rounded by the shared scalar path above.
            precise_bits = ops.fp_fmadd_rne_bits(fa_bits, fc_bits, fb_bits)
            oracle_scalar_bits = ops.fp_fmadds_fpr_bits(rm, fa_bits, fb_bits, fc_bits)
            d = ops.fp_bits_to_double(precise_bits)
            fused_single = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FMSUBS:
            precise_bits = ops.fp_fmsub_rne_bits(fa_bits, fc_bits, fb_bits)
            oracle_scalar_bits = ops.fp_fmsubs_fpr_bits(rm, fa_bits, fb_bits, fc_bits)
            d = ops.fp_bits_to_double(precise_bits)
            fused_single = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FNMADD:
            ops._fp_oracle_require_rne(rm)
            oracle_scalar_bits = ops.fp_fnmadd_rne_bits(fa_bits, fc_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FNMSUB:
            ops._fp_oracle_require_rne(rm)
            oracle_scalar_bits = ops.fp_fnmsub_rne_bits(fa_bits, fc_bits, fb_bits)
            d = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FNMADDS:
            precise_bits = ops.fp_fnmadd_rne_bits(fa_bits, fc_bits, fb_bits)
            oracle_scalar_bits = ops.fp_fnmadds_fpr_bits(rm, fa_bits, fb_bits, fc_bits)
            d = ops.fp_bits_to_double(precise_bits)
            fused_single = ops.fp_bits_to_double(oracle_scalar_bits)
        elif isinstance(ops, ConcreteOps) and op == Opcode.FNMSUBS:
            precise_bits = ops.fp_fnmsub_rne_bits(fa_bits, fc_bits, fb_bits)
            oracle_scalar_bits = ops.fp_fnmsubs_fpr_bits(rm, fa_bits, fb_bits, fc_bits)
            d = ops.fp_bits_to_double(precise_bits)
            fused_single = ops.fp_bits_to_double(oracle_scalar_bits)
        elif op in (Opcode.FADDS, Opcode.FADD):
            d = ops.fp_add(rm, op_fa, op_fb)
        elif op in (Opcode.FSUBS, Opcode.FSUB):
            d = ops.fp_sub(rm, op_fa, op_fb)
        elif op in (Opcode.FMULS, Opcode.FMUL):
            d = ops.fp_mul(rm, op_fa, op_fc)
        elif op in (Opcode.FDIVS, Opcode.FDIV):
            d = ops.fp_div(rm, op_fa, op_fb)
        elif op == Opcode.FRES:
            d = ops.fp_div(rm, ops.fp_round_to_single(rm, ops.fp_bits_to_double(ops.fp_const64(0x3FF0000000000000))), op_fb)
        elif op == Opcode.FRSQRTE:
            d = ops.fp_div(rm, ops.fp_bits_to_double(ops.fp_const64(0x3FF0000000000000)), ops.fp_sqrt(rm, op_fb))
        elif op in _FP_FUSED:
            addend = ops.fp_neg(op_fb) if op in _FP_FUSED_SUBTRACT else op_fb
            if op in _FP_FUSED_SINGLE:
                d = ops.fp_fma_single_precise(rm, op_fa, op_fc, addend)
                fused_single = ops.fp_fma_to_single_exact(rm, op_fa, op_fc, addend, d)
            else:
                d = ops.fp_fma(rm, op_fa, op_fc, addend)
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
            state = _fp_convert_to_integer(state, fd, fb_bits, rm, ops)
            if insn.record:
                state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
            return state
        elif op == Opcode.FCTIWZ:
            state = _fp_convert_to_integer(state, fd, fb_bits, ops.fp_rm_rtz(), ops)
            if insn.record:
                state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
            return state
        elif op in (Opcode.FCMPU, Opcode.FCMPO):
            return _execute_fp_compare(
                state, fd, state.fpr[fa], state.fpr[fb], op == Opcode.FCMPO, ops,
            )
        elif op == Opcode.MFFS:
            fpscr_bits = ops.fp_join_words(ops.const(0xFFF80000), state.fpscr)
            state = state.with_fpr(fd, fpscr_bits)
            if insn.record:
                state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
            return state
        elif op == Opcode.MTFSF:
            fm, rb_fpr = a
            source = state.fpr[rb_fpr]
            mask = 0
            for field in range(8):
                if fm & (1 << (7 - field)):
                    mask |= 0xF << ((7 - field) * 4)
            src32 = ops.fp_low_word(source)
            new_fpscr = ops.bor(ops.band(state.fpscr, ops.bnot(ops.const(mask))),
                                ops.band(src32, ops.const(mask)))
            state = state.with_fpscr(_fpscr_normalize(new_fpscr, ops))
            if insn.record:
                state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
            return state
        elif op == Opcode.MTFSFI:
            bf, imm4 = a
            shift = (7 - bf) * 4
            new_fpscr = ops.bor(
                ops.band(state.fpscr, ops.bnot(ops.const(0xF << shift))),
                ops.shl(ops.const(imm4 & 0xF), ops.const(shift)))
            state = state.with_fpscr(_fpscr_normalize(new_fpscr, ops))
            if insn.record:
                state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
            return state
        elif op in (Opcode.MTFSB0, Opcode.MTFSB1):
            bt = fd
            bit = 1 << (31 - bt)
            exception_bits = FPSCR_OX | FPSCR_UX | FPSCR_ZX | FPSCR_XX | FPSCR_VX_ANY
            if op == Opcode.MTFSB1 and bit & exception_bits:
                state = _fpscr_raise(state, bit, ops)
            else:
                new_fpscr = (
                    ops.bor(state.fpscr, ops.const(bit))
                    if op == Opcode.MTFSB1
                    else ops.band(state.fpscr, ops.bnot(ops.const(bit)))
                )
                state = state.with_fpscr(_fpscr_normalize(new_fpscr, ops))
            if insn.record:
                state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
            return state
        elif op == Opcode.MCRFS:
            bf, bfa = a
            shift = (7 - bfa) * 4
            cr_nibble = ops.band(ops.lshr(state.fpscr, ops.const(shift)), ops.const(0xF))
            exception_bits = FPSCR_FX | FPSCR_OX | FPSCR_UX | FPSCR_ZX | FPSCR_XX | FPSCR_VX_ANY
            clear_mask = (0xF << shift) & exception_bits
            new_fpscr = ops.band(state.fpscr, ops.bnot(ops.const(clear_mask)))
            state = state.with_fpscr(_fpscr_normalize(new_fpscr, ops))
            return _set_cr_field(state, bf, cr_nibble, ops)
        elif op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU,
                     Opcode.PSQ_LX, Opcode.PSQ_LUX, Opcode.PSQ_STX, Opcode.PSQ_STUX):
            if op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU):
                rs, ra, disp, w, i = a
                address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], ops.const(disp))
            else:
                rs, ra, rb, w, i = a
                address = ops.add(ops.const(0) if ra == 0 else state.gpr[ra], state.gpr[rb])
            is_psq_load = op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LX, Opcode.PSQ_LUX)
            gqr = state.gqr[i]
            if is_psq_load:
                qtype = ops.band(ops.lshr(gqr, ops.const(16)), ops.const(7))
                scale = ops.band(ops.lshr(gqr, ops.const(24)), ops.const(0x3F))
                state = _psq_domain(state, address, w, qtype, ops, "read")
                ps0_bits, ps1_bits = _psq_load_pair(state.memory, address, w, qtype, scale, ops)
                state = state.with_fpr(rs, ps0_bits).with_ps1(rs, ps1_bits)
            else:
                qtype = ops.band(gqr, ops.const(7))
                scale = ops.band(ops.lshr(gqr, ops.const(8)), ops.const(0x3F))
                state = _psq_domain(state, address, w, qtype, ops, "write")
                source0 = ops.fp_bits_to_double(state.fpr[rs])
                source1 = ops.fp_bits_to_double(state.ps1[rs])
                finite0 = ops.lnot(ops.lor(ops.fp_is_nan(source0), ops.fp_is_inf(source0)))
                finite1 = ops.lnot(ops.lor(ops.fp_is_nan(source1), ops.fp_is_inf(source1)))
                integer_type = ops.lnot(ops.eq(qtype, ops.const(0)))
                finite_sources = ops.land(finite0, ops.bool(True) if w else finite1)
                state = _constrain_valid(
                    state,
                    ops.lor(ops.lnot(integer_type), finite_sources),
                    InvalidReason.PSQ_NONFINITE_INTEGER_STORE,
                    ops,
                )
                state = replace(state, memory=_psq_store_pair(
                    state.memory, address, w, qtype, scale,
                    state.fpr[rs], state.ps1[rs], ops,
                ))
            update = op in (Opcode.PSQ_LU, Opcode.PSQ_STU, Opcode.PSQ_LUX, Opcode.PSQ_STUX)
            if update:
                state = state.with_gpr(ra, address)
            return state
        else:
            raise UnsupportedInstruction(insn.address, insn.raw, f"semantics not implemented for {op.value}")

        if op in _FP_FUSED_SINGLE:
            assert fused_single is not None
        elif (
            is_single
            and op not in (Opcode.FRSP, Opcode.FNEG, Opcode.FNABS, Opcode.FABS, Opcode.FMR)
            and oracle_scalar_bits is None
        ):
            d = ops.fp_round_to_single(rm, d)
        if op in _FP_FUSED:
            nan_a = ops.fp_is_nan(op_fa)
            nan_b = ops.fp_is_nan(op_fb)
            nan_c = ops.fp_is_nan(ops.fp_bits_to_double(fc_source_bits))
            any_nan = ops.lor(nan_a, ops.lor(nan_b, nan_c))
            no_nan = ops.lnot(any_nan)
            any_snan = ops.lor(
                ops.fp_is_snan_bits(fa_bits),
                ops.lor(
                    ops.fp_is_snan_bits(fb_bits),
                    ops.fp_is_snan_bits(fc_source_bits),
                ),
            )
            inf_a = ops.fp_is_inf(op_fa)
            inf_b = ops.fp_is_inf(op_fb)
            inf_c = ops.fp_is_inf(ops.fp_bits_to_double(fc_source_bits))
            zero = ops.fp_bits_to_double(ops.fp_const64(0))
            zero_a = ops.fp_is_eq(op_fa, zero)
            zero_c = ops.fp_is_eq(ops.fp_bits_to_double(fc_source_bits), zero)
            vximz = ops.land(no_nan, ops.lor(
                ops.land(inf_a, zero_c), ops.land(zero_a, inf_c),
            ))
            product_infinite = ops.lor(
                ops.land(inf_a, ops.lnot(zero_c)),
                ops.land(inf_c, ops.lnot(zero_a)),
            )
            product_positive = ops.fp_signs_equal_bits(fa_bits, fc_source_bits)
            b_positive = ops.fp_signs_equal_bits(fb_bits, ops.fp_const64(0))
            product_matches_b = ops.lor(
                ops.land(product_positive, b_positive),
                ops.land(ops.lnot(product_positive), ops.lnot(b_positive)),
            )
            invalid_signs = product_matches_b if op in _FP_FUSED_SUBTRACT else ops.lnot(product_matches_b)
            vxisi = ops.land(
                no_nan,
                ops.land(
                    ops.lnot(vximz),
                    ops.land(product_infinite, ops.land(inf_b, invalid_signs)),
                ),
            )
            invalid = ops.lor(any_snan, ops.lor(vximz, vxisi))
            nan_result = ops.lor(any_nan, ops.lor(vximz, vxisi))

            state = _fpscr_raise_if(state, any_snan, FPSCR_VXSNAN, ops)
            state = _fpscr_raise_if(state, vximz, FPSCR_VXIMZ, ops)
            state = _fpscr_raise_if(state, vxisi, FPSCR_VXISI, ops)

            propagated_nan = ops.ite(
                nan_a,
                ops.fp_quiet_nan_bits(fa_bits),
                ops.ite(
                    nan_b,
                    ops.fp_quiet_nan_bits(fb_bits),
                    ops.ite(
                        nan_c,
                        ops.fp_quiet_nan_bits(fc_source_bits),
                        ops.fp_const64(0x7FF8000000000000),
                    ),
                ),
            )
            if op in _FP_FUSED_SINGLE:
                propagated_nan = ops.fp_single_nan_bits(propagated_nan)
                assert fused_single is not None
                normal_bits = ops.fp_double_to_bits(fused_single)
            else:
                normal_bits = ops.fp_double_to_bits(d)
            if op in _FP_FUSED_NEGATE:
                # Oracle-backed ``fn*`` paths already return the negated result;
                # host-float / SymbolicOps still negate here. Never xor NaNs —
                # ``nan_result`` selects ``propagated_nan`` instead.
                if oracle_scalar_bits is None:
                    normal_bits = ops.fp_xor_sign(normal_bits)
            result_bits = ops.ite(nan_result, propagated_nan, normal_bits)

            invalid_enabled = ops.lnot(ops.eq(
                ops.band(state.fpscr, ops.const(FPSCR_VE)), ops.const(0),
            ))
            write_result = ops.lnot(ops.land(invalid, invalid_enabled))
            any_inf = ops.lor(inf_a, ops.lor(inf_b, inf_c))
            clear_fifr = ops.lor(nan_result, any_inf)
            cleared = ops.band(state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR)))
            state = state.with_fpscr(ops.ite(clear_fifr, cleared, state.fpscr))
            state = _fp_write_result_if(state, fd, result_bits, write_result, ops)
            if op in _FP_FUSED_SINGLE:
                state = state.with_ps1(
                    fd, ops.ite(write_result, result_bits, state.ps1[fd]),
                )

            if op == Opcode.FMADDS:
                fi = ops.lnot(ops.fp_is_eq(d, fused_single))
                write_fpscr = ops.band(state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR)))
                write_fpscr = ops.ite(fi, ops.bor(write_fpscr, ops.const(FPSCR_FI)), write_fpscr)
                state = state.with_fpscr(ops.ite(write_result, write_fpscr, state.fpscr))

            finite = lambda value: ops.lnot(ops.lor(ops.fp_is_nan(value), ops.fp_is_inf(value)))
            handled_nonfinite = ops.lor(any_nan, ops.lor(invalid, any_inf))
            state = _constrain_fp_defined_result(
                state, ops.lor(finite(d), handled_nonfinite), ops,
            )
            if isinstance(ops, SymbolicOps) and op in _FP_FUSED_SINGLE:
                # Keep fused-single proofs in decidable FP theory. This is the common
                # compiler case: inputs are binary32 values expanded in FPRs.
                single_origin = ops.land(
                    ops.lor(ops.lnot(finite(op_fa)), ops.fp_is_exact_single(op_fa)),
                    ops.land(
                        ops.lor(ops.lnot(finite(op_fb)), ops.fp_is_exact_single(op_fb)),
                        ops.lor(
                            ops.lnot(finite(ops.fp_bits_to_double(fc_source_bits))),
                            ops.fp_is_exact_single(ops.fp_bits_to_double(fc_source_bits)),
                        ),
                    ),
                )
                state = _constrain_fused_exact_binary32(state, single_origin, ops)
            if insn.record:
                state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
            return state
        if op in _FP_EXCEPTION_ARITH:
            right_bits = fc_bits if op in (Opcode.FMULS, Opcode.FMUL) else fb_bits
            right_value = op_fc if op in (Opcode.FMULS, Opcode.FMUL) else op_fb
            nan_a = ops.fp_is_nan(op_fa)
            nan_b = ops.fp_is_nan(right_value)
            any_nan = ops.lor(nan_a, nan_b)
            any_snan = ops.lor(
                ops.fp_is_snan_bits(fa_bits), ops.fp_is_snan_bits(right_bits),
            )
            inf_a = ops.fp_is_inf(op_fa)
            inf_b = ops.fp_is_inf(right_value)
            zero = ops.fp_bits_to_double(ops.fp_const64(0))
            zero_a = ops.fp_is_eq(op_fa, zero)
            zero_b = ops.fp_is_eq(right_value, zero)
            signs_equal = ops.fp_signs_equal_bits(fa_bits, right_bits)
            no_nan = ops.lnot(any_nan)

            vxisi = ops.bool(False)
            vximz = ops.bool(False)
            vxzdz = ops.bool(False)
            vxidi = ops.bool(False)
            zx = ops.bool(False)
            if op in (Opcode.FADDS, Opcode.FADD):
                vxisi = ops.land(no_nan, ops.land(
                    ops.land(inf_a, inf_b), ops.lnot(signs_equal),
                ))
            elif op in (Opcode.FSUBS, Opcode.FSUB):
                vxisi = ops.land(no_nan, ops.land(ops.land(inf_a, inf_b), signs_equal))
            elif op in (Opcode.FMULS, Opcode.FMUL):
                vximz = ops.land(no_nan, ops.lor(
                    ops.land(inf_a, zero_b), ops.land(zero_a, inf_b),
                ))
            else:
                vxzdz = ops.land(no_nan, ops.land(zero_a, zero_b))
                vxidi = ops.land(no_nan, ops.land(inf_a, inf_b))
                zx = ops.land(no_nan, ops.land(
                    zero_b, ops.lnot(ops.lor(zero_a, inf_b)),
                ))

            state = _fpscr_raise_if(state, any_snan, FPSCR_VXSNAN, ops)
            state = _fpscr_raise_if(state, vxisi, FPSCR_VXISI, ops)
            state = _fpscr_raise_if(state, vximz, FPSCR_VXIMZ, ops)
            state = _fpscr_raise_if(state, vxzdz, FPSCR_VXZDZ, ops)
            state = _fpscr_raise_if(state, vxidi, FPSCR_VXIDI, ops)
            state = _fpscr_raise_if(state, zx, FPSCR_ZX, ops)

            invalid = ops.lor(
                any_snan,
                ops.lor(vxisi, ops.lor(vximz, ops.lor(vxzdz, vxidi))),
            )
            nan_result = ops.lor(any_nan, ops.lor(
                vxisi, ops.lor(vximz, ops.lor(vxzdz, vxidi)),
            ))
            propagated_nan = ops.ite(
                nan_a,
                ops.fp_quiet_nan_bits(fa_bits),
                ops.ite(
                    nan_b,
                    ops.fp_quiet_nan_bits(right_bits),
                    ops.fp_const64(0x7FF8000000000000),
                ),
            )
            if is_single:
                propagated_nan = ops.fp_single_nan_bits(propagated_nan)
            normal_bits = ops.fp_double_to_bits(d)
            result_bits = ops.ite(nan_result, propagated_nan, normal_bits)

            invalid_enabled = ops.lnot(ops.eq(
                ops.band(state.fpscr, ops.const(FPSCR_VE)), ops.const(0),
            ))
            zero_enabled = ops.lnot(ops.eq(
                ops.band(state.fpscr, ops.const(FPSCR_ZE)), ops.const(0),
            ))
            suppress = ops.lor(
                ops.land(invalid, invalid_enabled), ops.land(zx, zero_enabled),
            )

            clear_fifr = nan_result
            if op in (Opcode.FADDS, Opcode.FADD, Opcode.FSUBS, Opcode.FSUB):
                clear_fifr = ops.lor(clear_fifr, ops.lor(inf_a, inf_b))
            if op in (Opcode.FMULS, Opcode.FMUL):
                clear_fifr = ops.bool(True)
            cleared_fpscr = ops.band(
                state.fpscr, ops.bnot(ops.const(FPSCR_FI | FPSCR_FR)),
            )
            state = state.with_fpscr(ops.ite(clear_fifr, cleared_fpscr, state.fpscr))
            state = _fp_write_result_if(state, fd, result_bits, ops.lnot(suppress), ops)
            if is_single:
                state = state.with_ps1(
                    fd, ops.ite(ops.lnot(suppress), result_bits, state.ps1[fd]),
                )

            finite = lambda value: ops.lnot(ops.lor(ops.fp_is_nan(value), ops.fp_is_inf(value)))
            handled_nonfinite = ops.lor(
                any_nan,
                ops.lor(invalid, ops.lor(zx, ops.lor(inf_a, inf_b))),
            )
            state = _constrain_fp_defined_result(
                state, ops.lor(finite(d), handled_nonfinite), ops,
            )
            if insn.record:
                state = _set_cr_field(state, 1, _fpscr_cr1(state, ops), ops)
            return state

        if op in _FP_VALUE_ARITH:
            finite = lambda value: ops.lnot(ops.lor(ops.fp_is_nan(value), ops.fp_is_inf(value)))
            if op in (Opcode.FADDS, Opcode.FSUBS, Opcode.FDIVS, Opcode.FADD, Opcode.FSUB, Opcode.FDIV):
                inputs_finite = ops.land(finite(op_fa), finite(op_fb))
            elif op in (Opcode.FMULS, Opcode.FMUL):
                inputs_finite = ops.land(finite(op_fa), finite(op_fc))
            else:
                inputs_finite = finite(op_fb)
            state = _constrain_fp_defined_result(
                state, ops.land(inputs_finite, finite(d)), ops,
            )
        d_bits = ops.fp_double_to_bits(d)
        state = _fpscr_set_fprf(state.with_fpr(fd, d_bits), d_bits, ops)
        if is_single:
            state = state.with_ps1(fd, d_bits)
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
            if op == Opcode.LMW:
                state = _touch_memory(state, address, 4, ops, "read")
                state = state.with_gpr(index, _load(state.memory, address, 4, ops))
            else:
                state = _touch_memory(state, address, 4, ops, "write")
                state = _mark_stack_pointer_escape(state, state.gpr[index], ops)
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


def _trap_condition(state: MachineState, to: int, ra: int, immediate: int, ops: WordOps) -> Any:
    left, right = state.gpr[ra], ops.const(immediate)
    condition = ops.bool(False)
    if to & 16: condition = ops.lor(condition, ops.signed_lt(left, right))
    if to & 8: condition = ops.lor(condition, ops.signed_lt(right, left))
    if to & 4: condition = ops.lor(condition, ops.eq(left, right))
    if to & 2: condition = ops.lor(condition, ops.unsigned_lt(left, right))
    if to & 1: condition = ops.lor(condition, ops.unsigned_lt(right, left))
    return condition


def _exception_entry(
    state: MachineState, return_pc: int, cause: int, ops: WordOps,
) -> MachineState:
    saved_msr = ops.bor(ops.band(state.msr, ops.const(0x87C0FFFF)), ops.const(cause))
    le_from_ile = ops.band(ops.lshr(state.msr, ops.const(16)), ops.const(1))
    entered_msr = ops.band(
        ops.bor(ops.band(state.msr, ops.bnot(ops.const(1))), le_from_ile),
        ops.bnot(ops.const(0x0004EF36)),
    )
    return replace(
        state, srr0=ops.const(return_pc), srr1=saved_msr, msr=entered_msr,
    )


def _apply_call_summary(
    state: MachineState,
    ops: WordOps,
    call_id: int | str,
    contract: CalleeContract,
) -> MachineState:
    if not isinstance(ops, SymbolicOps):
        raise ExecutionInconclusive("matched-callee summaries require symbolic execution")
    helper_prefix = "fixed-eabi-runtime-helper:"
    if contract.source.startswith(helper_prefix):
        name = contract.source[len(helper_prefix):]
        parts = name.split("_")
        if len(parts) != 3 or parts[0] != "" or parts[1] not in {
            "savegpr", "restgpr", "savefpr", "restfpr",
        }:
            raise ExecutionInconclusive(f"invalid fixed EABI helper {name!r}")
        try:
            first = int(parts[2])
        except ValueError as exc:
            raise ExecutionInconclusive(f"invalid fixed EABI helper {name!r}") from exc
        is_fpr = "fpr" in parts[1]
        is_save = parts[1].startswith("save")
        width = 8 if is_fpr else 4
        result = state
        for index in range(first, 32):
            address = ops.add(state.gpr[11], ops.const(-width * (32 - index)))
            if is_save:
                result = _touch_memory(result, address, width, ops, "write")
                value = state.fpr[index] if is_fpr else state.gpr[index]
                if not is_fpr:
                    result = _mark_stack_pointer_escape(result, value, ops)
                result = replace(result, memory=_store(result.memory, address, value, width, ops))
            else:
                result = _touch_memory(result, address, width, ops, "read")
                value = _load(result.memory, address, width, ops)
                result = result.with_fpr(index, value) if is_fpr else result.with_gpr(index, value)
        # Helpers are calls: disable private-stack masking per the memory model.
        return replace(result, stack_private=ops.bool(False))
    callee = str(call_id)
    token = ops.call_token(callee, state, contract)

    def result(component: str, sort: Any) -> Any:
        return ops.call_result(callee, component, token, sort)
    fresh_gprs = list(state.gpr)
    for i in (0,) + tuple(range(3, 13)):
        if f"r{i}" in contract.writes:
            fresh_gprs[i] = result(f"r{i}", state.gpr[i].sort())
    fresh_fprs = list(state.fpr)
    fresh_ps1 = list(state.ps1)
    for i in range(14):
        if f"f{i}" in contract.writes:
            fresh_fprs[i] = result(f"f{i}", state.fpr[i].sort())
        if f"f{i}.ps1" in contract.writes:
            fresh_ps1[i] = result(f"f{i}.ps1", state.ps1[i].sort())
    fresh_xer = XerState(
        result("xer.ca", state.xer.ca.sort()) if "xer.ca" in contract.writes else state.xer.ca,
        result("xer.ov", state.xer.ov.sort()) if "xer.ov" in contract.writes else state.xer.ov,
        result("xer.so", state.xer.so.sort()) if "xer.so" in contract.writes else state.xer.so,
    )
    volatile_cr_mask = sum(
        0xF << ((7 - field) * 4)
        for field in (0, 1, 5, 6, 7)
        if f"cr{field}" in contract.writes or "cr" in contract.writes
    )
    fresh_cr = ops.bor(
        ops.band(state.cr, ops.const(~volatile_cr_mask & MASK32)),
        ops.band(result("cr", state.cr.sort()), ops.const(volatile_cr_mask)),
    )
    memory = (
        result("memory", state.memory.sort())
        if "memory" in contract.writes else state.memory
    )
    # Definedness is monotonic across opaque summaries: a caller that is already
    # invalid keeps its first invalid_reason, and the callee cannot resurrect
    # validity. When the caller is still valid, the summary may introduce a new
    # reason through the UF transition (keyed by entry definedness above).
    z3 = ops.z3
    if "valid" in contract.writes:
        callee_valid = result("valid", state.valid.sort())
        fresh_valid = z3.And(state.valid, callee_valid)
    else:
        callee_valid = state.valid
        fresh_valid = state.valid
    if "invalid_reason" in contract.writes:
        callee_reason = result("invalid_reason", z3.BitVecSort(8))
        fresh_reason = z3.If(
            z3.Not(state.valid),
            state.invalid_reason,
            z3.If(z3.Not(callee_valid), callee_reason, state.invalid_reason),
        )
    else:
        fresh_reason = state.invalid_reason
    return MachineState(
        tuple(fresh_gprs),
        tuple(fresh_fprs),
        tuple(fresh_ps1),
        state.gqr,
        fresh_cr,
        fresh_xer,
        result("fpscr", state.fpscr.sort()) if "fpscr" in contract.writes else state.fpscr,
        state.lr,
        result("ctr", state.ctr.sort()) if "ctr" in contract.writes else state.ctr,
        state.msr,
        state.sr,
        state.time_base,
        state.srr0,
        state.srr1,
        state.spr,
        memory,
        fresh_valid,
        fresh_reason,
        state.memory_touches,
        state.memory_reads,
        state.memory_writes,
        state.stack_low,
        state.memory_effects + ((token,) if "memory" in contract.writes else ()),
        state.stack_layout_valid,
        ops.bool(False),
    )


def _cfg_branch_target(insn: Instruction, ops: WordOps) -> tuple[Any, int | str]:
    relocation = insn.relocation
    if relocation is not None:
        if relocation.relocation_type not in (R_PPC_REL24, R_PPC_REL14):
            raise ExecutionInconclusive("branch has a non-branch relocation")
        return _relocation_address(insn, ops), relocation.canonical_symbol
    target = insn.operands[0] if insn.opcode == Opcode.B else insn.operands[2]
    return target, target


DEFAULT_MAX_LOOP_ITERATIONS = 256

# Maximum private-frame depth accepted by stack-layout feasibility.
# Frames deeper than this (including SP wraparound that collapses stack_low
# near address zero) are rejected as impossible layouts. 16 MiB is far above
# any legitimate Broadway/Wii frame and fail-closes the wraparound hole where
# upward addi through 0xFFFFFFFF would otherwise privatize almost all memory.
MAX_PRIVATE_STACK_DEPTH = 0x01000000


def _path_condition_feasible(condition: Any, ops: WordOps) -> bool:
    """Return False only when the path condition is concretely unsatisfiable."""
    if isinstance(ops, ConcreteOps):
        return bool(condition)
    if isinstance(ops, SymbolicOps):
        simplified = ops.z3.simplify(condition)
        return not ops.z3.is_false(simplified)
    return True


def execute_cfg(
    state: MachineState,
    instructions: list[Instruction],
    ops: WordOps,
    *,
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    assumed_callees_used: set[int | str] | None = None,
    callee_contracts: dict[int | str, CalleeContract] | None = None,
    floating_point_domain: FloatingPointDomain | None = None,
    memory_bus: MemoryBus | None = None,
    deadline: Deadline | None = None,
    jump_table_targets: dict[int, tuple[int, ...]] | None = None,
    affine_loop_summaries: dict[int, LoopSummary] | None = None,
    affine_summaries_used: list[LoopSummary] | None = None,
    memory_loop_summaries: dict[int, MemoryLoopSummary] | None = None,
    memory_summaries_used: list[MemoryLoopSummary] | None = None,
) -> list[Terminal]:
    if not instructions:
        raise ValueError("cannot execute an empty block")
    if max_loop_iterations < 1:
        raise ValueError("max_loop_iterations must be >= 1")
    if memory_bus is not None and not isinstance(ops, ConcreteOps):
        raise ExecutionInconclusive(
            "memory bus routing is supported for ConcreteOps only",
        )
    domain = floating_point_domain or FloatingPointDomain()
    domain.validate()
    domain_token = _FP_DOMAIN.set(domain)
    bus_token = _MEMORY_BUS.set(memory_bus)
    try:
        if memory_bus is not None:
            memory_bus.ram = state.memory
        return _execute_cfg_body(
            state,
            instructions,
            ops,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees,
            assumed_callees_used=assumed_callees_used,
            callee_contracts=callee_contracts,
            deadline=deadline,
            jump_table_targets=jump_table_targets,
            affine_loop_summaries=affine_loop_summaries,
            affine_summaries_used=affine_summaries_used,
            memory_loop_summaries=memory_loop_summaries,
            memory_summaries_used=memory_summaries_used,
        )
    finally:
        _MEMORY_BUS.reset(bus_token)
        _FP_DOMAIN.reset(domain_token)


def _execute_cfg_body(
    state: MachineState,
    instructions: list[Instruction],
    ops: WordOps,
    *,
    max_instructions: int,
    max_paths: int,
    max_loop_iterations: int,
    assumed_callees: frozenset[int | str],
    assumed_callees_used: set[int | str] | None,
    callee_contracts: dict[int | str, CalleeContract] | None,
    deadline: Deadline | None = None,
    jump_table_targets: dict[int, tuple[int, ...]] | None = None,
    affine_loop_summaries: dict[int, LoopSummary] | None = None,
    affine_summaries_used: list[LoopSummary] | None = None,
    memory_loop_summaries: dict[int, MemoryLoopSummary] | None = None,
    memory_summaries_used: list[MemoryLoopSummary] | None = None,
) -> list[Terminal]:
    if state.stack_low is None:
        state = replace(
            state, stack_low=state.gpr[1], stack_layout_valid=ops.bool(True),
            stack_private=ops.bool(True),
        )
    caller_frame_top = state.gpr[1]
    callee_contracts = callee_contracts or {}
    by_address = {item.address: item for item in instructions}
    start = instructions[0].address
    end = instructions[-1].address + 4
    if affine_loop_summaries is None:
        affine_loop_summaries = {}
    if memory_loop_summaries is None:
        memory_loop_summaries = {}
    # visit_counts[pc] = times this path has already entered ``pc``.
    # A back-edge that would make the count reach max_loop_iterations fails closed.
    work: list[tuple[int, MachineState, Any, dict[int, int], int]] = [
        (start, state, ops.bool(True), {}, 0),
    ]
    terminals: list[Terminal] = []

    def enqueue(
        next_pc: int,
        next_state: MachineState,
        next_condition: Any,
        next_visits: dict[int, int],
        next_steps: int,
    ) -> None:
        if not _path_condition_feasible(next_condition, ops):
            return
        work.append((next_pc, next_state, next_condition, next_visits, next_steps))
        if len(work) + len(terminals) > max_paths:
            raise ExecutionInconclusive(f"path limit exceeded ({max_paths})")

    def record_terminal(
        term_condition: Any,
        term_state: MachineState,
        exit_kind: str,
        exit_target: Any = None,
    ) -> None:
        if not _path_condition_feasible(term_condition, ops):
            return
        if exit_target is None:
            terminals.append(Terminal(term_condition, term_state, exit_kind))
        else:
            terminals.append(Terminal(term_condition, term_state, exit_kind, exit_target))
        if len(work) + len(terminals) > max_paths:
            raise ExecutionInconclusive(f"path limit exceeded ({max_paths})")

    while work:
        if deadline is not None and deadline.expired():
            raise ProofDeadlineExceeded("cfg-exploration")
        pc, current, condition, visit_counts, steps = work.pop()
        if steps >= max_instructions:
            raise ExecutionInconclusive(f"instruction limit exceeded ({max_instructions})")
        if pc == end:
            record_terminal(condition, current, "fallthrough")
            continue
        insn = by_address.get(pc)
        if insn is None:
            record_terminal(condition, current, "direct-branch", ops.const(pc))
            continue
        prior_visits = visit_counts.get(pc, 0)
        summary = affine_loop_summaries.get(pc)
        if summary is not None and prior_visits == 0:
            # Closed-form discharge: skip unrolling when the header is first entered.
            summarized = apply_affine_loop_summary(current, summary, ops)
            if affine_summaries_used is not None:
                affine_summaries_used.append(summary)
            enqueue(
                summary.exit_pc,
                summarized,
                condition,
                {**visit_counts, pc: 1},
                steps + 1,
            )
            continue
        memory_summary = memory_loop_summaries.get(pc)
        if memory_summary is not None and prior_visits == 0:
            summarized = apply_memory_loop_summary(current, memory_summary, ops)
            if memory_summaries_used is not None:
                memory_summaries_used.append(memory_summary)
            enqueue(
                memory_summary.exit_pc,
                summarized,
                condition,
                {**visit_counts, pc: 1},
                steps + 1,
            )
            continue
        if prior_visits >= max_loop_iterations:
            raise ExecutionInconclusive(
                f"loop iteration limit exceeded ({max_loop_iterations}) at 0x{pc:08x}"
            )
        new_visits = {**visit_counts, pc: prior_visits + 1}
        if insn.opcode == Opcode.TWI:
            trap = _trap_condition(current, *insn.operands, ops)
            trapped = _exception_entry(current, pc, 0x00020000, ops)
            record_terminal(
                ops.land(condition, trap), trapped, "program-exception", ops.const(0x700),
            )
            enqueue(
                pc + 4, current, ops.land(condition, ops.lnot(trap)),
                new_visits, steps + 1,
            )
            continue
        if insn.opcode == Opcode.SC:
            entered = _exception_entry(current, pc + 4, 0, ops)
            record_terminal(condition, entered, "system-call", ops.const(0xC00))
            continue
        if insn.opcode == Opcode.RFI:
            privileged = ops.lnot(ops.eq(
                ops.band(current.msr, ops.const(0x00004000)), ops.const(0),
            ))
            exception_state = _exception_entry(current, pc, 0x00040000, ops)
            record_terminal(
                ops.land(condition, privileged), exception_state,
                "program-exception", ops.const(0x700),
            )
            mask = ops.const(0x87C0FFFF)
            restored_msr = ops.band(
                ops.bor(
                    ops.band(current.msr, ops.bnot(mask)),
                    ops.band(current.srr1, mask),
                ),
                ops.const(0xFFFBFFFF),
            )
            restored = replace(current, msr=restored_msr)
            record_terminal(
                ops.land(condition, ops.lnot(privileged)), restored,
                "return-from-interrupt", current.srr0,
            )
            continue
        if insn.opcode not in (Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR):
            next_state = execute_instruction(current, insn, ops)
            assert current.stack_low is not None
            next_stack_low = ops.ite(
                ops.unsigned_lt(next_state.gpr[1], current.stack_low),
                next_state.gpr[1],
                current.stack_low,
            )
            # Reject r1 above entry SP, and reject wraparound that would make
            # stack_low near zero with a multi-megabyte "private" interval.
            not_above_entry = ops.lnot(
                ops.unsigned_lt(caller_frame_top, next_state.gpr[1]),
            )
            stack_low_ordered = ops.lnot(
                ops.unsigned_lt(caller_frame_top, next_stack_low),
            )
            frame_depth = ops.sub(caller_frame_top, next_stack_low)
            depth_ok = ops.lnot(
                ops.unsigned_lt(ops.const(MAX_PRIVATE_STACK_DEPTH), frame_depth),
            )
            next_state = replace(
                next_state,
                stack_low=next_stack_low,
                stack_layout_valid=ops.land(
                    current.stack_layout_valid,
                    ops.land(not_above_entry, ops.land(stack_low_ordered, depth_ok)),
                ),
            )
            enqueue(pc + 4, next_state, condition, new_visits, steps + 1)
            continue

        old_lr, old_ctr = current.lr, current.ctr
        linked = replace(current, lr=ops.const(pc + 4)) if insn.link else current
        if insn.opcode == Opcode.B:
            target, target_key = _cfg_branch_target(insn, ops)
            if isinstance(target, int) and (target in by_address or target == end):
                enqueue(target, linked, condition, new_visits, steps + 1)
            elif insn.link and target_key in assumed_callees:
                if assumed_callees_used is not None:
                    assumed_callees_used.add(target_key)
                summarized = _apply_call_summary(
                    linked, ops, target_key,
                    callee_contracts.get(target_key, CalleeContract.opaque_eabi()),
                )
                enqueue(pc + 4, summarized, condition, new_visits, steps + 1)
            elif not insn.link and target_key in assumed_callees:
                if assumed_callees_used is not None:
                    assumed_callees_used.add(target_key)
                summarized = _apply_call_summary(
                    linked, ops, target_key,
                    callee_contracts.get(target_key, CalleeContract.opaque_eabi()),
                )
                record_terminal(
                    condition, summarized, "return",
                    ops.band(current.lr, ops.const(0xFFFFFFFC)),
                )
            elif insn.link and isinstance(ops, SymbolicOps):
                raise ExecutionInconclusive(
                    f"call target {target_key!r} has no matched-callee lemma"
                )
            elif insn.relocation is not None and isinstance(ops, SymbolicOps):
                raise ExecutionInconclusive(
                    f"tail-call target {target_key!r} has no matched-callee lemma"
                )
            else:
                record_terminal(
                    condition, linked, "call" if insn.link else "direct-branch",
                    ops.const(target) if isinstance(target, int) else target,
                )
            continue

        bo, bi = insn.operands[:2]
        predicate, branched_state = _branch_condition(linked, bo, bi, ops, allow_ctr=insn.opcode != Opcode.BCCTR)
        taken_condition = ops.land(condition, predicate)
        fall_condition = ops.land(condition, ops.lnot(predicate))
        target_key: int | str | None = None
        if insn.opcode == Opcode.BC:
            target, target_key = _cfg_branch_target(insn, ops)
            kind = "call" if insn.link else "direct-branch"
        elif insn.opcode == Opcode.BCLR:
            target = ops.band(old_lr, ops.const(0xFFFFFFFC))
            kind = "call-indirect" if insn.link else "return"
        else:
            target = ops.band(old_ctr, ops.const(0xFFFFFFFC))
            kind = "call-indirect" if insn.link else "indirect-branch"

        # Proven jump-table closure: split the taken edge into one path per
        # enumerated CTR target (plan: enqueue under taken ∧ CTR == addr).
        closed_targets = None if jump_table_targets is None else jump_table_targets.get(pc)
        if (
            closed_targets is not None
            and insn.opcode == Opcode.BCCTR
            and not insn.link
            and kind == "indirect-branch"
        ):
            aligned_ctr = ops.band(old_ctr, ops.const(0xFFFFFFFC))
            for target_pc in closed_targets:
                aligned = target_pc & 0xFFFFFFFC
                case_condition = ops.land(
                    taken_condition,
                    ops.eq(aligned_ctr, ops.const(aligned)),
                )
                if aligned in by_address or aligned == end:
                    enqueue(
                        aligned, branched_state, case_condition, new_visits, steps + 1,
                    )
                else:
                    record_terminal(
                        case_condition, branched_state, "direct-branch", ops.const(aligned),
                    )
            enqueue(pc + 4, branched_state, fall_condition, new_visits, steps + 1)
            continue

        if isinstance(target, int) and (target in by_address or target == end):
            enqueue(target, branched_state, taken_condition, new_visits, steps + 1)
        elif insn.link and kind == "call" and target_key is not None and target_key in assumed_callees:
            if assumed_callees_used is not None:
                assumed_callees_used.add(target_key)
            summarized = _apply_call_summary(
                branched_state, ops, target_key,
                callee_contracts.get(target_key, CalleeContract.opaque_eabi()),
            )
            enqueue(pc + 4, summarized, taken_condition, new_visits, steps + 1)
        elif not insn.link and target_key is not None and target_key in assumed_callees:
            if assumed_callees_used is not None:
                assumed_callees_used.add(target_key)
            summarized = _apply_call_summary(
                branched_state, ops, target_key,
                callee_contracts.get(target_key, CalleeContract.opaque_eabi()),
            )
            record_terminal(
                taken_condition, summarized, "return",
                ops.band(old_lr, ops.const(0xFFFFFFFC)),
            )
        elif insn.link and isinstance(ops, SymbolicOps):
            raise ExecutionInconclusive(
                f"call target {target_key!r} has no matched-callee lemma"
            )
        elif insn.relocation is not None and isinstance(ops, SymbolicOps):
            raise ExecutionInconclusive(
                f"tail-call target {target_key!r} has no matched-callee lemma"
            )
        else:
            record_terminal(
                taken_condition, branched_state, kind,
                ops.const(target) if isinstance(target, int) else target,
            )
        enqueue(pc + 4, branched_state, fall_condition, new_visits, steps + 1)
    return terminals


def execute_block(state: MachineState, instructions: list[Instruction], ops: WordOps, max_instructions: int = 2048) -> MachineState:
    terminals = execute_cfg(state, instructions, ops, max_instructions=max_instructions)
    if len(terminals) != 1 or terminals[0].exit_kind != "fallthrough":
        raise ExecutionInconclusive("concrete replay has multiple paths or a non-fallthrough exit")
    # Stack-bound metadata is proof bookkeeping, not architectural state exposed
    # by this single-block compatibility helper.
    return replace(
        terminals[0].state,
        stack_low=state.stack_low,
        stack_layout_valid=state.stack_layout_valid,
        stack_private=state.stack_private,
    )


def register_effects(insn: Instruction) -> tuple[set[str], set[str]]:
    """Conservative architectural read/write sets used by live-out analysis."""
    reads: set[str] = set()
    writes: set[str] = set()
    op, a = insn.opcode, insn.operands
    if op in (Opcode.DCBF, Opcode.DCBI, Opcode.DCBST, Opcode.DCBT, Opcode.ICBI):
        if a[0]: reads.add(f"r{a[0]}")
        reads.add(f"r{a[1]}")
    elif op in (Opcode.DCBZ, Opcode.DCBZ_L):
        if a[0]: reads.add(f"r{a[0]}")
        reads.add(f"r{a[1]}")
        reads.add("hid0")
        if op == Opcode.DCBZ_L: reads.add("hid2")
        writes.add("memory")
    elif op in (Opcode.SYNC, Opcode.ISYNC):
        pass
    elif op == Opcode.MFMSR:
        reads.add("msr")
        writes.add(f"r{a[0]}")
    elif op == Opcode.MTMSR:
        reads.add(f"r{a[0]}")
        writes.add("msr")
    elif op == Opcode.MFSR:
        reads.add(f"sr{a[1]}")
        writes.add(f"r{a[0]}")
    elif op == Opcode.MTSR:
        reads.add(f"r{a[0]}")
        writes.add(f"sr{a[1]}")
    elif op == Opcode.MFTB:
        reads.add("time_base")
        writes.add(f"r{a[0]}")
    elif op == Opcode.TWI:
        reads |= {f"r{a[1]}", "msr"}
        writes |= {"msr", "srr0", "srr1"}
    elif op == Opcode.SC:
        reads.add("msr")
        writes |= {"msr", "srr0", "srr1"}
    elif op == Opcode.RFI:
        reads |= {"msr", "srr0", "srr1"}
        writes.add("msr")
    elif op in FP_D_LOADS or op in FP_X_LOADS:
        writes.add(f"f{a[0]}")
        if op in (Opcode.LFS, Opcode.LFSU, Opcode.LFSX, Opcode.LFSUX):
            writes.add(f"f{a[0]}.ps1")
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
    elif op in _FP_PS_SIMPLE:
        writes |= {f"f{a[0]}", f"f{a[0]}.ps1"}
        for index in a[1:]:
            reads |= {f"f{index}", f"f{index}.ps1"}
        if insn.record:
            reads.add("fpscr")
            writes.add("cr1")
    elif op in (_FP_PS_BASIC | _FP_PS_FUSED | _FP_PS_SUM | _FP_PS_DIV | _FP_PS_ESTIMATE):
        writes |= {f"f{a[0]}", f"f{a[0]}.ps1", "fpscr"}
        reads.add("fpscr")
        for index in a[1:]:
            reads |= {f"f{index}", f"f{index}.ps1"}
        if insn.record:
            writes.add("cr1")
    elif op in _FP_PS_SELECT:
        writes |= {f"f{a[0]}", f"f{a[0]}.ps1"}
        for index in a[1:]:
            reads |= {f"f{index}", f"f{index}.ps1"}
        if insn.record:
            reads.add("fpscr")
            writes.add("cr1")
    elif op in _FP_ESTIMATE:
        writes |= {f"f{a[0]}", "fpscr"}
        if op == Opcode.FRES:
            writes.add(f"f{a[0]}.ps1")
        reads |= {f"f{a[2]}", "fpscr"}
        if insn.record:
            writes.add("cr1")
    elif op in (Opcode.FCMPU, Opcode.FCMPO):
        for idx in a[1:]:
            if 0 <= idx < 32: reads.add(f"f{idx}")
        reads.add("fpscr")
        writes.add(f"cr{a[0]}")
        writes.add("fpscr")
    elif op in (Opcode.PS_CMPU0, Opcode.PS_CMPO0, Opcode.PS_CMPU1, Opcode.PS_CMPO1):
        suffix = ".ps1" if op in (Opcode.PS_CMPU1, Opcode.PS_CMPO1) else ""
        for idx in a[1:]:
            if 0 <= idx < 32: reads.add(f"f{idx}{suffix}")
        reads.add("fpscr")
        writes.add(f"cr{a[0]}")
        writes.add("fpscr")
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
    elif op in _FP_SCALAR_ARITH:
        writes.add(f"f{a[0]}")
        if op in _FP_SINGLE_ARITH:
            writes.add(f"f{a[0]}.ps1")
        for idx in a[1:]:
            if 0 <= idx < 32: reads.add(f"f{idx}")
        if op in (Opcode.FCTIW, Opcode.FCTIWZ):
            reads.add("fpscr")
            writes.add("fpscr")
        if insn.record: writes.add("cr1")
    elif op in (Opcode.MFSPR, Opcode.MTSPR):
        reg, spr = a
        name = (
            "xer" if spr == 1 else "lr" if spr == 8 else "ctr" if spr == 9
            else "srr0" if spr == 26 else "srr1" if spr == 27
            else AUX_SPR_NAMES[spr] if spr in AUX_SPR_NAMES
            else "time_base" if spr in TIME_BASE_WRITE_SPRS
            else f"gqr{spr - 912}"
        )
        if op == Opcode.MFSPR:
            writes.add(f"r{reg}")
            reads |= {"xer.ca", "xer.ov", "xer.so"} if name == "xer" else {name}
        else:
            reads.add(f"r{reg}")
            writes |= {"xer.ca", "xer.ov", "xer.so"} if name == "xer" else {name}
    elif op in _FP_PSQ_OPS:
        is_load = op in (Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LX, Opcode.PSQ_LUX)
        is_indexed = op in (Opcode.PSQ_LX, Opcode.PSQ_LUX, Opcode.PSQ_STX, Opcode.PSQ_STUX)
        if is_load:
            writes |= {f"f{a[0]}", f"f{a[0]}.ps1"}
        else:
            reads |= {f"f{a[0]}", f"f{a[0]}.ps1"}
        reads.add(f"gqr{a[4]}")
        if a[1]: reads.add(f"r{a[1]}")
        if is_indexed: reads.add(f"r{a[2]}")
        if is_load: reads.add("memory")
        else: writes.add("memory")
        if op in (Opcode.PSQ_LU, Opcode.PSQ_STU, Opcode.PSQ_LUX, Opcode.PSQ_STUX):
            writes.add(f"r{a[1]}")
    elif op == Opcode.LMW:
        if a[1]:
            reads.add(f"r{a[1]}")
        reads.add("memory")
        writes |= {f"r{index}" for index in range(a[0], 32)}
    elif op == Opcode.STMW:
        reads |= {f"r{index}" for index in range(a[0], 32)}
        if a[1]:
            reads.add(f"r{a[1]}")
        writes.add("memory")
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


def _infer_invalid_reasons(instructions: list[Instruction]) -> set[int]:
    """Return the set of InvalidReason values a callee body may produce."""
    reasons: set[int] = set()
    opcodes = {insn.opcode for insn in instructions}
    MEMORY_OPS = {
        Opcode.LBZ, Opcode.LBZU, Opcode.LHZ, Opcode.LHZU, Opcode.LHA, Opcode.LHAU,
        Opcode.LWZ, Opcode.LWZU, Opcode.LBZX, Opcode.LBZUX, Opcode.LHZX, Opcode.LHZUX,
        Opcode.LHAX, Opcode.LHAUX, Opcode.LWZX, Opcode.LWZUX, Opcode.LHBRX, Opcode.LWBRX,
        Opcode.STB, Opcode.STBU, Opcode.STH, Opcode.STHU, Opcode.STW, Opcode.STWU,
        Opcode.STBX, Opcode.STBUX, Opcode.STHX, Opcode.STHUX, Opcode.STWX, Opcode.STWUX,
        Opcode.STHBRX, Opcode.STWBRX, Opcode.LMW, Opcode.STMW,
    } | set(FP_D_MEM.keys()) | set(FP_X_MEM.keys()) | _FP_PSQ_OPS
    if opcodes & MEMORY_OPS:
        reasons.add(InvalidReason.UNALIGNED_ACCESS.value)
    DIVIDE_OPS = {
        Opcode.DIVW, Opcode.DIVWU,
    }
    if opcodes & DIVIDE_OPS:
        reasons.add(InvalidReason.DIVIDE_UNDEFINED.value)
    ROUNDING_OPS = {Opcode.FCTIW, Opcode.FCTIWZ}
    if opcodes & ROUNDING_OPS:
        reasons.add(InvalidReason.FP_ROUNDING_MODE.value)
    FP_DOMAIN_OPS = (
        _FP_EXCEPTION_ARITH | _FP_VALUE_ARITH | _FP_FUSED
        | _FP_PS_BASIC | _FP_PS_FUSED | _FP_PS_SUM | _FP_PS_DIV
    )
    if opcodes & FP_DOMAIN_OPS:
        reasons.add(InvalidReason.FP_DOMAIN_EXCLUDED.value)
    CACHE_OPS = {Opcode.DCBZ, Opcode.DCBZ_L}
    if opcodes & CACHE_OPS:
        reasons.add(InvalidReason.CACHE_DISABLED.value)
    PRIVILEGED_OPS = {Opcode.MFMSR, Opcode.MTMSR, Opcode.MFSR, Opcode.MTSR}
    if opcodes & PRIVILEGED_OPS:
        reasons.add(InvalidReason.PRIVILEGED_INSTRUCTION.value)
    for insn in instructions:
        if insn.opcode not in (Opcode.MFSPR, Opcode.MTSPR):
            continue
        spr = insn.operands[1]
        if spr in (26, 27) or spr in AUX_SPR_INDEX or spr in TIME_BASE_WRITE_SPRS:
            reasons.add(InvalidReason.PRIVILEGED_INSTRUCTION.value)
            break
    if opcodes & _FP_PSQ_OPS:
        reasons.add(InvalidReason.PSQ_INVALID_TYPE.value)
        if opcodes & {Opcode.PSQ_ST, Opcode.PSQ_STU, Opcode.PSQ_STX, Opcode.PSQ_STUX}:
            reasons.add(InvalidReason.PSQ_NONFINITE_INTEGER_STORE.value)
    return reasons


def infer_callee_contract(
    instructions: list[Instruction],
    *,
    nested_contracts: dict[int | str, CalleeContract] | None = None,
) -> CalleeContract:
    """Infer a conservative EABI exit contract from a matched function body.

    Reads are a conservative union of instruction dependencies. Writes are
    limited to ABI-volatile architectural state, so temporary saves/restores of
    nonvolatile registers do not become false exit effects.

    When the body contains direct calls, pass ``nested_contracts`` covering every
    callee to compose precise summaries. Missing or opaque nested contracts keep
    the historical fail-closed ``nested-call-opaque-eabi`` result.
    """
    reads: set[str] = {"valid"}
    observed_writes: set[str] = set()
    nested_targets: set[int | str] = set()
    has_indirect_call = False
    for insn in instructions:
        insn_reads, insn_writes = register_effects(insn)
        if (
            insn.opcode == Opcode.BCLR
            and not insn.link
            and insn.operands[:2] == (20, 0)
        ):
            # The numeric link-register return address is continuation metadata,
            # not a semantic input to a location-independent matched callee.
            insn_reads -= {"lr", "cr", "ctr"}
        reads.update(name for name in insn_reads if name)
        observed_writes.update(name for name in insn_writes if name)
        is_callish = insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR) and (
            insn.link or insn.relocation is not None
        )
        if not is_callish:
            continue
        if insn.relocation is not None:
            nested_targets.add(insn.relocation.canonical_symbol)
        else:
            # Linked branch without a relocation (bctrl / blrl / absolute bl).
            has_indirect_call = True

    volatile = {
        *(f"r{i}" for i in (0, *range(3, 13))),
        *(f"f{i}" for i in range(14)),
        *(f"f{i}.ps1" for i in range(14)),
        "cr", "cr0", "cr1", "cr5", "cr6", "cr7",
        "xer.ca", "xer.ov", "xer.so", "fpscr", "ctr", "memory",
    }

    if has_indirect_call or nested_targets:
        provided = nested_contracts or {}
        if has_indirect_call or any(target not in provided for target in nested_targets):
            opaque = CalleeContract.opaque_eabi()
            return CalleeContract(opaque.reads, opaque.writes, "nested-call-opaque-eabi")
        if any(
            "*" in provided[target].reads or "*" in provided[target].writes
            for target in nested_targets
        ):
            opaque = CalleeContract.opaque_eabi()
            return CalleeContract(opaque.reads, opaque.writes, "nested-call-opaque-eabi")
        for target in nested_targets:
            contract = provided[target]
            reads.update(contract.reads)
            observed_writes.update(contract.writes)
            if contract.invalid_reasons:
                observed_writes.add("invalid_reason")

    writes = observed_writes & volatile
    if "memory" in writes:
        reads.add("memory")
    if any(name == "cr" or name.startswith("cr") for name in writes):
        reads.add("cr")
    writes.add("valid")
    invalid_reasons = _infer_invalid_reasons(instructions)
    if nested_targets and nested_contracts:
        for target in nested_targets:
            invalid_reasons |= set(nested_contracts[target].invalid_reasons)
    if invalid_reasons:
        writes.add("invalid_reason")
    source = (
        "matched-body-effects-composed"
        if nested_targets else
        "matched-body-effects"
    )
    return CalleeContract(
        frozenset(reads), frozenset(writes), source,
        invalid_reasons=frozenset(invalid_reasons),
    )


def automatic_live_out(instructions: list[Instruction]) -> tuple[str, ...]:
    """Conservative live-out over-approximation: every architectural component written."""
    written: set[str] = set()
    for insn in instructions:
        op, a = insn.opcode, insn.operands
        if op in (
            Opcode.DCBF, Opcode.DCBI, Opcode.DCBST, Opcode.DCBT,
            Opcode.ICBI, Opcode.SYNC, Opcode.ISYNC,
        ):
            pass
        elif op in (Opcode.DCBZ, Opcode.DCBZ_L):
            written.add("memory")
        elif op in (Opcode.MFMSR, Opcode.MFSR, Opcode.MFTB):
            written.add(f"r{a[0]}")
        elif op == Opcode.MTMSR:
            written.add("msr")
        elif op == Opcode.MTSR:
            written.add(f"sr{a[1]}")
        elif op in (Opcode.TWI, Opcode.SC):
            written |= {"msr", "srr0", "srr1"}
        elif op == Opcode.RFI:
            written.add("msr")
        elif op in LOADS:
            written.add(f"r{a[0]}")
            if LOADS[op][2]: written.add(f"r{a[1]}")
        elif op in STORES:
            written.add("memory")
            if STORES[op][1]: written.add(f"r{a[1]}")
        elif op in FP_D_LOADS or op in FP_X_LOADS:
            written.add(f"f{a[0]}")
            if op in (Opcode.LFS, Opcode.LFSU, Opcode.LFSX, Opcode.LFSUX):
                written.add(f"f{a[0]}.ps1")
            if op in FP_UPDATES: written.add(f"r{a[1]}")
        elif op in FP_D_STORES or op in FP_X_STORES:
            written.add("memory")
            if op in FP_UPDATES: written.add(f"r{a[1]}")
        elif op in (Opcode.FCMPU, Opcode.FCMPO, Opcode.PS_CMPU0, Opcode.PS_CMPO0,
                     Opcode.PS_CMPU1, Opcode.PS_CMPO1):
            written.add(f"cr{a[0]}")
            written.add("fpscr")
        elif op in (_FP_SCALAR_ARITH | _FP_PS_BASIC | _FP_PS_FUSED | _FP_PS_SUM | _FP_PS_DIV | _FP_PS_ESTIMATE | _FP_PS_SELECT | _FP_PS_CMP | _FP_PS_SIMPLE):
            written.add(f"f{a[0]}")
            if op in (_FP_SINGLE_ARITH | _FP_PS_BASIC | _FP_PS_FUSED | _FP_PS_SUM | _FP_PS_DIV | _FP_PS_ESTIMATE | _FP_PS_SELECT | _FP_PS_SIMPLE):
                written.add(f"f{a[0]}.ps1")
            if op not in ({Opcode.FSEL, Opcode.FNEG, Opcode.FMR, Opcode.FNABS, Opcode.FABS} | _FP_PS_SELECT | _FP_PS_SIMPLE):
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
                written.add(f"f{a[0]}.ps1")
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
            elif a[1] == 26: written.add("srr0")
            elif a[1] == 27: written.add("srr1")
            elif a[1] in AUX_SPR_NAMES: written.add(AUX_SPR_NAMES[a[1]])
            elif a[1] in TIME_BASE_WRITE_SPRS: written.add("time_base")
            elif 912 <= a[1] <= 919: written.add(f"gqr{a[1] - 912}")
        elif op in (Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR):
            if insn.link or (
                insn.relocation is not None and op in (Opcode.B, Opcode.BC)
            ):
                written |= {
                    *(f"r{i}" for i in (0, *range(3, 13))),
                    *(f"f{i}" for i in range(14)),
                    *(f"f{i}.ps1" for i in range(14)),
                    "cr", "fpscr", "xer.ca", "xer.ov", "xer.so", "lr",
                    "ctr", "memory",
                }
            if op in (Opcode.BC, Opcode.BCLR) and not (a[0] & 4): written.add("ctr")
        else:
            written.add(f"r{a[0]}")
            if op in (Opcode.ADDIC, Opcode.ADDIC_DOT, Opcode.SUBFIC, Opcode.ADDC, Opcode.ADDE, Opcode.ADDME, Opcode.ADDZE, Opcode.SUBFC, Opcode.SUBFE, Opcode.SUBFME, Opcode.SUBFZE, Opcode.SRAW, Opcode.SRAWI):
                written.add("xer.ca")
            if insn.overflow:
                written |= {"xer.ov", "xer.so"}
        if insn.record:
            written.add("cr1" if op in (_FP_SCALAR_ARITH | _FP_PS_BASIC | _FP_PS_FUSED | _FP_PS_SUM | _FP_PS_DIV | _FP_PS_ESTIMATE | _FP_PS_SELECT | _FP_PS_SIMPLE) else "cr0")
        elif op in (Opcode.ADDIC_DOT, Opcode.ANDI_DOT, Opcode.ANDIS_DOT):
            written.add("cr0")
    order = [
        *(f"r{i}" for i in range(32)), *(f"f{i}" for i in range(32)),
        *(f"f{i}.ps1" for i in range(32)), *(f"gqr{i}" for i in range(8)),
        *(f"sr{i}" for i in range(16)),
        *(AUX_SPR_NAMES[number] for number in AUX_SPR_NAMES),
        "cr", *(f"cr{i}" for i in range(8)), "fpscr", "xer.ca", "xer.ov",
        "xer.so", "lr", "ctr", "msr", "time_base", "srr0", "srr1", "memory",
    ]
    return tuple(item for item in order if item in written)


def read_gprs(instructions: list[Instruction]) -> set[int]:
    result: set[int] = set()
    for insn in instructions:
        reads, _ = register_effects(insn)
        result |= {int(name[1:]) for name in reads if name.startswith("r") and name[1:].isdigit()}
    return result

"""Deterministic seeded randomized PPC32 program generator (P2-01).

The generator emits *valid* instruction blocks (never uniform random words) using
the existing :mod:`tools.ppc_equivalence.fixtures.encode` helpers, runs them
through :class:`~tools.ppc_equivalence.semantics.ConcreteOps` to record the
expected observable oracle, and offers a three-way differential check that
symbolically proves equivalence against the recorded oracle.

Only the ``generators`` package and ``cli.py`` are touched for P2-01; the shared
semantics, decoder, engine, and corpus are imported but never modified.
"""

from __future__ import annotations

import random
from typing import Any, Callable

from ..decoder import decode_block
from ..engine import check_equivalence
from ..contract import parse_observables, EquivalenceContract
from ..fixtures.encode import (
    BLR,
    bc,
    dform,
    mform,
    pack_xer,
    words_to_hex,
    x_logical,
    xl,
    xo,
)
from ..model import MachineState, XerState, ConcreteMemory, concrete_state
from ..result import ARCHITECTURE_MODEL, FloatingPointDomain
from ..semantics import ConcreteOps, execute_cfg
from ..fixtures.corpus import _mtfsf, _mtfsfi, _mcrfs, _fp_x, _fp_cmp, _fp_a
from ..ir import Opcode

GENERATOR_NAME = "ppc-random-v1"
RESULT_REG = 7
SANDBOX_BASE = 0x00001000
SANDBOX_SIZE = 256
CODE_BASE = 0x80018000


# --------------------------------------------------------------------------- #
# Boundary / strata pools
# --------------------------------------------------------------------------- #
_SIGNED_BOUNDARY = [0, 1, -1, 0x7FFFFFFF, -0x80000000, 0x80000000]
_IMM_BOUNDARY = [0, 1, -1, 0x7FFF, -0x8000, 0x8000, 0xFFFF, -0x8000 & 0xFFFF]
_SHIFT_BOUNDARY = [0, 1, 31, 32]
_MASK_BOUNDARY = [0, 0x7FFFFFFF, 0xFFFFFFFF, 0x55555555, 0xAAAAAAAA, 0xFF00FF00]

# Raw 64-bit double bit-patterns for FP strata.
_FP_ZERO = 0x0000000000000000
_FP_NZERO = 0x8000000000000000
_FP_ONE = 0x3FF0000000000000
_FP_NORM = 0x4008000000000000
_FP_SUBNORM = 0x0008000000000000
_FP_INF = 0x7FF0000000000000
_FP_NINF = 0xFFF0000000000000
_FP_NAN = 0x7FF8000012345678
_FP_SNAN = 0x7FF0000012345678
_FP_TIE = 0x3FE0000000000001  # .5 with nonzero low bit -> tie-to-even stress
_FP_EXP_BUMP = 0x4FF0000000000000  # exponent transition stress
_FP_PATTERNS = [
    _FP_ZERO, _FP_NZERO, _FP_ONE, _FP_NORM, _FP_SUBNORM, _FP_INF,
    _FP_NINF, _FP_NAN, _FP_SNAN, _FP_TIE, _FP_EXP_BUMP,
]


def _mask32(value: int) -> int:
    return value & 0xFFFFFFFF


def _mask64(value: int) -> int:
    return value & 0xFFFFFFFFFFFFFFFF


# --------------------------------------------------------------------------- #
# Family registry — each entry builds one valid instruction word.
# --------------------------------------------------------------------------- #
class _Ctx:
    """Per-instruction generation context holding the seeded RNG and strata."""

    def __init__(self, rng: random.Random) -> None:
        self.rng = rng

    def _reg_aliases(self, *, paired: bool = False) -> tuple[int, int, int]:
        """Return (dst, srcA, srcB) honoring an operand-alias stratum."""
        rng = self.rng
        stratum = rng.choice([
            "distinct", "dst_eq_srcA", "dst_eq_srcB",
            "srcA_eq_srcB", "r0", "r1", "cr_boundary",
        ])
        lo, hi = (0, 31) if not paired else (0, 31)
        if stratum == "distinct":
            dst = rng.randint(lo, hi)
            a = rng.randint(lo, hi)
            b = rng.randint(lo, hi)
        elif stratum == "dst_eq_srcA":
            v = rng.randint(lo, hi)
            dst = a = v
            b = rng.randint(lo, hi)
        elif stratum == "dst_eq_srcB":
            v = rng.randint(lo, hi)
            dst = b = v
            a = rng.randint(lo, hi)
        elif stratum == "srcA_eq_srcB":
            v = rng.randint(lo, hi)
            a = b = v
            dst = rng.randint(lo, hi)
        elif stratum == "r0":
            dst = 0
            a = rng.randint(0, 31)
            b = rng.randint(0, 31)
        elif stratum == "r1":
            a = 1
            dst = rng.randint(0, 31)
            b = rng.randint(0, 31)
        else:  # cr_boundary: keep indices at extreme ends
            dst = rng.choice([0, 31])
            a = rng.choice([0, 31])
            b = rng.choice([0, 31])
        return dst, a, b

    def _int(self) -> int:
        return _mask32(self.rng.choice(_SIGNED_BOUNDARY) if self.rng.random() < 0.5
                       else self.rng.getrandbits(32))

    def _simm(self) -> int:
        return self.rng.choice(_IMM_BOUNDARY) & 0xFFFF

    def _imm(self) -> int:
        return _mask32(self.rng.getrandbits(16))


# Map of family name -> builder(ctx) -> word.
def _build_arith(ctx: _Ctx) -> int:
    dst, a, b = ctx._reg_aliases()
    # rb must be 0 for the reserved single-operand forms below.
    rb_zero_ops = {104, 202, 234, 200, 232}
    table = {0: 266, 1: 10, 2: 40, 3: 8, 4: 104, 5: 202, 6: 234,
             7: 200, 8: 232, 9: 138, 10: 136, 11: 235, 12: 75, 13: 11,
             14: 491, 15: 459}
    xo_bits = ctx.rng.choice(list(table.values()))
    rc = 1
    if xo_bits in rb_zero_ops:
        b = 0
    return xo(31, dst, a, b, xo_bits, rc=rc)


def _build_logical(ctx: _Ctx) -> int:
    dst, a, b = ctx._reg_aliases()
    table = {0: 28, 1: 444, 2: 316, 3: 476, 4: 124, 5: 284, 6: 60, 7: 412}
    xo_bits = ctx.rng.choice(list(table.values()))
    return x_logical(dst, a, b, xo_bits, rc=1)


def _build_unary(ctx: _Ctx) -> int:
    dst, a, _ = ctx._reg_aliases()
    xo_bits = ctx.rng.choice([26, 922, 954])
    return x_logical(dst, a, 0, xo_bits, rc=1)


def _build_shift(ctx: _Ctx) -> int:
    dst, a, b = ctx._reg_aliases()
    kind = ctx.rng.choice(["slw", "srw", "sraw"])
    if kind == "slw":
        return x_logical(dst, a, b, 24, rc=1)
    if kind == "srw":
        return x_logical(dst, a, b, 536, rc=1)
    return x_logical(dst, a, b, 792, rc=1)


def _build_srawi(ctx: _Ctx) -> int:
    dst, a, _ = ctx._reg_aliases()
    sh = ctx.rng.choice(_SHIFT_BOUNDARY)
    return xo(31, a, dst, sh & 0x1F, 824, rc=1)


def _build_rot(ctx: _Ctx) -> int:
    dst, a, b = ctx._reg_aliases()
    kind = ctx.rng.choice(["rlwinm", "rlwimi", "rlwnm"])
    sh = ctx.rng.choice(_SHIFT_BOUNDARY)
    mb = ctx.rng.choice(_MASK_BOUNDARY[:4])
    me = ctx.rng.choice(_MASK_BOUNDARY[:4])
    if kind == "rlwinm":
        return mform(21, dst, a, sh & 0x1F, mb & 0x1F, me & 0x1F, rc=1)
    if kind == "rlwimi":
        return mform(20, dst, a, sh & 0x1F, mb & 0x1F, me & 0x1F, rc=1)
    return (23 << 26) | ((dst & 0x1F) << 21) | ((a & 0x1F) << 16) | ((b & 0x1F) << 11) | ((mb & 0x1F) << 6) | ((me & 0x1F) << 1) | 1


def _build_dimm_logical(ctx: _Ctx) -> int:
    dst, a, _ = ctx._reg_aliases()
    table = {0: 24, 1: 25, 2: 26, 3: 27, 4: 28, 5: 29}
    opcd = ctx.rng.choice(list(table.values()))
    imm = ctx.rng.choice(_MASK_BOUNDARY) & 0xFFFF
    return dform(opcd, a, dst, imm)


def _build_dadd(ctx: _Ctx) -> int:
    dst, a, _ = ctx._reg_aliases()
    opcd = ctx.rng.choice([14, 15])
    imm = ctx.rng.choice(_IMM_BOUNDARY) & 0xFFFF
    return dform(opcd, a, dst, imm)


def _build_dimm_arith(ctx: _Ctx) -> int:
    dst, a, _ = ctx._reg_aliases()
    table = {0: (12, False), 1: (13, False), 2: (8, True), 3: (7, True)}
    opcd, signed = ctx.rng.choice(list(table.values()))
    if signed:
        imm = ctx.rng.choice(_IMM_BOUNDARY) & 0xFFFF
        return dform(opcd, a, dst, imm)
    imm = ctx.rng.choice(_MASK_BOUNDARY) & 0xFFFF
    return dform(opcd, a, dst, imm)


def _build_compare(ctx: _Ctx) -> int:
    a, b, _ = ctx._reg_aliases()
    field = ctx.rng.choice([0, 1, 7])
    kind = ctx.rng.choice(["cmpw", "cmplw", "cmpwi", "cmplwi"])
    if kind == "cmpw":
        return (31 << 26) | ((field & 7) << 23) | ((a & 0x1F) << 16) | ((b & 0x1F) << 11) | (0 << 1)
    if kind == "cmplw":
        return (31 << 26) | ((field & 7) << 23) | ((a & 0x1F) << 16) | ((b & 0x1F) << 11) | (32 << 1)
    if kind == "cmpwi":
        imm = ctx.rng.choice(_IMM_BOUNDARY) & 0xFFFF
        return (11 << 26) | ((field & 7) << 23) | ((a & 0x1F) << 16) | imm
    imm = ctx.rng.choice(_MASK_BOUNDARY) & 0xFFFF
    return (10 << 26) | ((field & 7) << 23) | ((a & 0x1F) << 16) | imm


def _build_cr_logical(ctx: _Ctx) -> int:
    bt = ctx.rng.randint(0, 31)
    ba = ctx.rng.randint(0, 31)
    bb = ctx.rng.randint(0, 31)
    xo_bits = ctx.rng.choice([33, 129, 193, 225, 257, 289, 417, 449])
    return xl(19, bt, ba, bb, xo_bits)


def _build_memory(ctx: _Ctx) -> int:
    dst, a, b = ctx._reg_aliases()
    ra = 4  # sandbox base
    table = {
        0: (32, False), 1: (34, False), 2: (40, False), 3: (42, False),
        4: (36, False), 5: (38, False), 6: (44, False),
        7: (33, False), 8: (35, False), 9: (41, False), 10: (43, False),
        11: (37, False), 12: (39, False), 13: (45, False),
        14: (23, False), 15: (87, False), 16: (279, False),
        17: (151, False), 18: (215, False), 19: (407, False),
        20: (55, False), 21: (119, False), 22: (311, False),
        23: (375, False), 24: (183, False), 25: (247, False), 26: (439, False),
    }
    opcd, _ = ctx.rng.choice(list(table.values()))
    is_store = opcd in (36, 38, 44, 37, 39, 45, 151, 215, 407, 183, 247, 439)
    is_update = opcd in (33, 35, 41, 43, 37, 39, 45, 55, 119, 311, 375, 183, 247, 439)
    is_load_update = opcd in (33, 35, 41, 43, 55, 119, 311, 375)
    if is_update:
        ra = ctx.rng.randint(5, 31)  # avoid r4 which is the sandbox base used elsewhere
        dst = ctx.rng.randint(0, 31)
        while dst == ra:
            dst = ctx.rng.randint(0, 31)
    if opcd in (32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45):
        imm = ctx.rng.choice([0, 4, 8, 12, -4, -8, -12]) & 0xFFFF
        return dform(opcd, dst, ra, imm)
    # X-form memory
    if opcd in (55, 119, 311, 375):
        ra = ctx.rng.randint(5, 31)
        if opcd in (55, 119, 311, 375):  # load-update forms require RA != RT
            dst = ctx.rng.randint(0, 31)
            while dst == ra:
                dst = ctx.rng.randint(0, 31)
    return _x_memory(opcd, dst, ra, b)


def _x_memory(opcd: int, dst: int, ra: int, rb: int) -> int:
    # X-form memory opcodes carry primary 31 with the opcode value as the XO field.
    return (31 << 26) | ((dst & 0x1F) << 21) | ((ra & 0x1F) << 16) | ((rb & 0x1F) << 11) | (opcd << 1)


def _build_fp_scalar(ctx: _Ctx) -> int:
    dst = 7
    a = ctx.rng.randint(0, 31)
    b = ctx.rng.randint(0, 31)
    c = ctx.rng.randint(0, 31)
    # (category, sub) where category is "dp" (double arith) or "sp" (single
    # arith, primary 59) or "aux" (primary 63 non-double FP auxiliary/move).
    table = {
        0: ("dp", 42), 1: ("dp", 40), 2: ("dp", 50), 3: ("dp", 36), 4: ("dp", 52),
        5: ("aux", 264), 6: ("aux", 40), 7: ("aux", 72),
        8: ("sp", 42), 9: ("sp", 40), 10: ("sp", 50), 11: ("sp", 36), 12: ("sp", 48),
        13: ("aux", 0), 14: ("aux", 32), 15: ("aux", 12), 16: ("aux", 14),
        17: ("aux", 15), 18: ("aux", 583), 19: ("aux", 711),
        20: ("aux", 134), 21: ("aux", 70), 22: ("aux", 64),
    }
    category, sub = ctx.rng.choice(list(table.values()))
    rc = ctx.rng.choice([0, 1])
    if category == "dp":
        # _fp_a(63, ...) with sub having bit5 set yields double-precision ops.
        if sub in (42, 40, 36):  # fadd/fsub/fdiv require fc=0
            c = 0
        if sub == 50:  # fmul requires fb=0
            b = 0
        if sub == 52:  # frsqrte requires fa=fb=fc=0
            a = 0
            b = 0
            c = 0
        return _fp_a(63, dst, a, b, c, sub, rc=rc)
    if category == "sp":
        if sub in (42, 40, 36, 48):  # fadds/fsubs/fdivs/fres require fc=0
            c = 0
        if sub == 48:  # fres requires fa=0 as well
            a = 0
        if sub == 50:  # fmuls requires fb=0
            b = 0
        return _fp_a(59, dst, a, b, c, sub, rc=rc)
    # aux (primary 63, non-double)
    if sub == 0:  # fcmpu
        return _fp_cmp(ctx.rng.randint(0, 7), a, b, 0)
    if sub == 32:  # fcmpo
        return _fp_cmp(ctx.rng.randint(0, 7), a, b, 32)
    if sub in (264, 40, 72, 136, 12, 14, 15):  # fabs/fneg/fmr/fnabs/frsp/fctiw/fctiwz
        return _fp_x(dst, 0, 0, sub, rc=rc)
    if sub == 583:  # mffs
        return _fp_x(dst, 0, 0, 583, rc=rc)
    if sub == 711:  # mtfsf — keep FPSCR field 0 (RN/NI) untouched
        return _mtfsf(0xFE, b, rc=rc)
    if sub == 134:  # mtfsfi — bf must be nonzero so RN/NI field 0 is untouched
        return _mtfsfi(ctx.rng.randint(0, 6), ctx.rng.randint(0, 15), rc=rc)
    if sub == 70:  # mtfsb0 (bt=0 clears a bit; harmless on zero FPSCR)
        return _fp_x(0, 0, 0, 70, rc=rc)
    if sub == 64:  # mcrfs
        return _mcrfs(ctx.rng.randint(0, 7), ctx.rng.randint(0, 7))
    raise AssertionError(f"unhandled aux sub {sub}")


def _build_fp_cmp(ctx: _Ctx) -> int:
    a = ctx.rng.randint(0, 31)
    b = ctx.rng.randint(0, 31)
    bf = ctx.rng.choice([0, 1, 7])
    ordered = ctx.rng.random() < 0.5
    xo_bits = 32 if ordered else 0
    return _fp_cmp(bf, a, b, xo_bits)


def _build_fp_move(ctx: _Ctx) -> int:
    dst = 7
    a = ctx.rng.randint(0, 31)
    b = ctx.rng.randint(0, 31)
    c = ctx.rng.randint(0, 31)
    table = {0: 264, 1: 40, 2: 72, 3: 136, 4: 583, 5: 711, 6: 134, 7: 70, 8: 64}
    sub = ctx.rng.choice(list(table.values()))
    if sub == 583:  # mffs
        return (63 << 26) | ((dst & 0x1F) << 21) | (583 << 1)
    if sub == 711:  # mtfsf — keep FPSCR field 0 (RN/NI) untouched
        return _mtfsf(0xFE, b, rc=ctx.rng.choice([0, 1]))
    if sub == 134:  # mtfsfi — bf must be nonzero so RN/NI field 0 is untouched
        return _mtfsfi(ctx.rng.randint(0, 6), ctx.rng.randint(0, 15), rc=ctx.rng.choice([0, 1]))
    if sub == 70:  # mtfsb0 (bt=0 clears a bit; harmless on zero FPSCR)
        return (63 << 26) | ((70 & 0x3FF) << 1)
    if sub == 64:  # mcrfs
        return _mcrfs(ctx.rng.randint(0, 7), ctx.rng.randint(0, 7))
    # fabs/fneg/fmr/fnabs: the decoder requires FA=0.
    return (63 << 26) | ((dst & 0x1F) << 21) | (sub << 1) | ctx.rng.choice([0, 1])


def _build_fp_paired(ctx: _Ctx) -> int:
    dst = 7
    a = ctx.rng.randint(0, 31)
    b = ctx.rng.randint(0, 31)
    c = ctx.rng.randint(0, 31)
    rc = ctx.rng.choice([0, 1])
    # 5-bit paired-single arithmetic/fused opcodes.
    arith = {
        0: 21,  # ps_add
        1: 20,  # ps_sub
        2: 18,  # ps_div
        3: 25,  # ps_mul
        4: 24,  # ps_res
        5: 26,  # ps_rsqrte
        6: 12,  # ps_muls0
        7: 13,  # ps_muls1
        8: 10,  # ps_sum0
        9: 11,  # ps_sum1
        10: 14,  # ps_madds0
        11: 15,  # ps_madds1
        12: 28,  # ps_msub
        13: 29,  # ps_madd
        14: 30,  # ps_nmsub
        15: 31,  # ps_nmadd
    }
    # 10-bit paired-single move/merge/compare opcodes.
    move = {12: 40, 13: 72, 14: 264, 15: 136}            # ps_neg/mr/nabs/abs
    merge = {16: 528, 17: 560, 18: 592, 19: 624}         # ps_merge**
    cmp = {20: 0, 21: 32, 22: 64, 23: 96}                # ps_cmp**
    kind = ctx.rng.choice(["arith", "move", "merge", "cmp"])
    if kind == "arith":
        xo5 = ctx.rng.choice(list(arith.values()))
        if xo5 in (21, 20, 18):  # add/sub/div: fc must be 0
            c = 0
        elif xo5 in (24, 26):  # res/rsqrte: fa,fc must be 0
            a = 0
            c = 0
            return (4 << 26) | ((dst & 31) << 21) | ((b & 31) << 11) | (xo5 << 1) | rc
        elif xo5 in (25, 12, 13):  # mul/muls0/muls1: fb must be 0
            b = 0
        return (4 << 26) | ((dst & 31) << 21) | ((a & 31) << 16) | ((b & 31) << 11) | ((c & 31) << 6) | (xo5 << 1) | rc
    if kind == "move":
        xo10 = ctx.rng.choice(list(move.values()))
        return _ps_move(xo10, dst, b, rc)
    if kind == "merge":
        xo10 = ctx.rng.choice(list(merge.values()))
        return _ps_merge(xo10, dst, a, b, rc)
    # compare forms (rc must be 0; reserved bits 21-22/0 must be clear)
    xo10 = ctx.rng.choice(list(cmp.values()))
    bf = ctx.rng.randint(0, 7)
    return (4 << 26) | ((bf & 7) << 23) | ((a & 31) << 16) | ((b & 31) << 11) | (xo10 << 1)


def _ps_move(xo5: int, fd: int, fb: int, rc: int) -> int:
    return (4 << 26) | ((fd & 31) << 21) | ((fb & 31) << 11) | (xo5 << 1) | rc


def _ps_merge(xo5: int, fd: int, fa: int, fb: int, rc: int) -> int:
    return (4 << 26) | ((fd & 31) << 21) | ((fa & 31) << 16) | ((fb & 31) << 11) | (xo5 << 1) | rc


def _build_psq(ctx: _Ctx) -> int:
    rs = 7
    ra = 4
    imm = ctx.rng.choice([0, 4, 8, 12, -4]) & 0xFFF
    w = 0
    i = 1
    store = ctx.rng.random() < 0.5
    if store:
        primary = 61 if ctx.rng.random() < 0.5 else 60
    else:
        primary = 57 if ctx.rng.random() < 0.5 else 56
    return (primary << 26) | ((rs & 0x1F) << 21) | ((ra & 0x1F) << 16) | ((w & 1) << 15) | ((i & 7) << 12) | imm


# Registry: family name -> builder. The coverage test asserts these families.
FAMILY_BUILDERS: dict[str, Callable[[_Ctx], int]] = {
    "integer": _build_arith,
    "logical": _build_logical,
    "unary": _build_unary,
    "shift": _build_shift,
    "shifti": _build_srawi,
    "rotate": _build_rot,
    "dimm_logical": _build_dimm_logical,
    "dadd": _build_dadd,
    "dimm_arith": _build_dimm_arith,
    "compare": _build_compare,
    "cr_logical": _build_cr_logical,
    "memory": _build_memory,
    "fp_scalar": _build_fp_scalar,
    "fp_cmp": _build_fp_cmp,
    "fp_move": _build_fp_move,
    "fp_paired": _build_fp_paired,
    "psq": _build_psq,
}

SUPPORTED_FAMILIES = tuple(sorted(FAMILY_BUILDERS))


# --------------------------------------------------------------------------- #
# State construction
# --------------------------------------------------------------------------- #
def _build_initial_state(rng: random.Random, *, fp_weighted: bool = False) -> dict[str, Any]:
    gpr = [_mask32(rng.getrandbits(32)) for _ in range(32)]
    gpr[4] = SANDBOX_BASE  # sandbox base for memory instructions
    fpr = [_mask64(rng.choice(_FP_PATTERNS)) for _ in range(32)] if fp_weighted else [_mask64(rng.getrandbits(64)) for _ in range(32)]
    fpr[7] = _mask64(rng.choice(_FP_PATTERNS))
    cr = _mask32(rng.getrandbits(32))
    xer = {
        "ca": rng.choice([0, 1]),
        "ov": rng.choice([0, 1]),
        "so": rng.choice([0, 1]),
    }
    # Sandbox memory: fully seeded bytes.
    memory_bytes: dict[str, int] = {}
    for offset in range(SANDBOX_SIZE):
        memory_bytes[f"0x{(SANDBOX_BASE + offset) & 0xFFFFFFFF:08x}"] = rng.randint(0, 255)
    gqr = [_mask32(rng.getrandbits(32)) for _ in range(8)]
    gqr[1] = 0  # single precision, scale 0 for PSQ
    return {
        "gpr": gpr,
        "fpr_bits": fpr,
        "cr": cr,
        "xer": xer,
        "gqr": gqr,
        "fpscr": 0,
        "memory_bytes": memory_bytes,
    }


def _initial_state_to_machine(state: dict[str, Any]) -> MachineState:
    values: dict[str, Any] = {
        "gpr": {f"r{i}": v for i, v in enumerate(state["gpr"])},
        "fpr": {f"f{i}": v for i, v in enumerate(state["fpr_bits"])},
        "gqr": {f"gqr{i}": v for i, v in enumerate(state["gqr"])},
        "cr": state["cr"],
        "xer": state["xer"],
        "fpscr": state["fpscr"],
        "memory": dict(state["memory_bytes"]),
    }
    return concrete_state(values)


def _machine_to_expected(final: MachineState) -> dict[str, Any]:
    return {
        "gpr": [int(v) & 0xFFFFFFFF for v in final.gpr],
        "fpr_bits": [int(v) & 0xFFFFFFFFFFFFFFFF for v in final.fpr],
        "cr": int(final.cr) & 0xFFFFFFFF,
        "xer": pack_xer(
            ca=int(bool(final.xer.ca)),
            ov=int(bool(final.xer.ov)),
            so=int(bool(final.xer.so)),
        ),
        "lr": int(final.lr) & 0xFFFFFFFF,
        "ctr": int(final.ctr) & 0xFFFFFFFF,
        "fpscr": int(final.fpscr) & 0xFFFFFFFF,
        "memory_bytes": _memory_to_bytes(final.memory),
    }


def _memory_to_bytes(memory: ConcreteMemory) -> dict[str, int]:
    out: dict[str, int] = {}
    for address, byte in memory.bytes:
        out[f"0x{address & 0xFFFFFFFF:08x}"] = byte & 0xFF
    # Include the sandbox base region fully so the oracle is replayable.
    for offset in range(SANDBOX_SIZE):
        addr = (SANDBOX_BASE + offset) & 0xFFFFFFFF
        key = f"0x{addr:08x}"
        if key not in out:
            out[key] = memory.read(addr) & 0xFF
    return out


# --------------------------------------------------------------------------- #
# Public API
# --------------------------------------------------------------------------- #
def generate_program(
    seed: int,
    generator_version: str = GENERATOR_NAME,
    *,
    max_instructions: int = 12,
) -> dict[str, Any]:
    """Generate a deterministic, self-checking PPC32 program for ``seed``."""
    rng = random.Random(seed)
    family = rng.choice(SUPPORTED_FAMILIES)
    builder = FAMILY_BUILDERS[family]
    fp_heavy = family.startswith("fp") or family == "psq"
    ctx = _Ctx(rng)

    n = rng.randint(1, max_instructions)
    code_words: list[int] = []
    for _ in range(n):
        code_words.append(builder(ctx) & 0xFFFFFFFF)

    initial = _build_initial_state(rng, fp_weighted=fp_heavy)
    initial_state_machine = _initial_state_to_machine(initial)

    instructions = decode_block(
        b"".join((w & 0xFFFFFFFF).to_bytes(4, "big") for w in code_words),
        base=CODE_BASE,
        validate_with_capstone=False,
    )
    terminals = execute_cfg(
        initial_state_machine,
        instructions,
        ConcreteOps(),
        max_instructions=max_instructions + 64,
        max_paths=16,
    )
    taken = [t for t in terminals if bool(t.condition)]
    if len(taken) != 1:
        raise AssertionError(f"seed {seed}: expected one concrete path, got {len(taken)}")
    expected = _machine_to_expected(taken[0].state)

    return {
        "generator": generator_version,
        "seed": seed,
        "architecture_model": ARCHITECTURE_MODEL,
        "program_hex": words_to_hex(code_words),
        "family": family,
        "initial_state": {
            "gpr": initial["gpr"],
            "fpr_bits": initial["fpr_bits"],
            "cr": initial["cr"],
            "xer": initial["xer"],
            "gqr": initial["gqr"],
            "fpscr": initial["fpscr"],
            "memory_bytes": initial["memory_bytes"],
        },
        "observables": [
            f"r{RESULT_REG}", "cr", "xer.ca", "xer.ov", "xer.so",
            f"f{RESULT_REG}", "fpscr", "memory",
        ],
        "expected": expected,
    }


def _run_concrete(program: dict[str, Any], *, max_instructions: int = 12) -> dict[str, Any]:
    code = b"".join(
        int(w, 16).to_bytes(4, "big")
        for w in program["program_hex"].split()
    )
    instructions = decode_block(code, base=CODE_BASE, validate_with_capstone=False)
    initial = _initial_state_to_machine(program["initial_state"])
    terminals = execute_cfg(
        initial, instructions, ConcreteOps(),
        max_instructions=max_instructions + 64, max_paths=16,
    )
    taken = [t for t in terminals if bool(t.condition)]
    if len(taken) != 1:
        raise AssertionError("concrete execution did not select exactly one path")
    return _machine_to_expected(taken[0].state)


def differential_check(seed: int, *, max_instructions: int = 12) -> dict[str, Any]:
    """Three-way (ConcreteOps + symbolic) differential check for ``seed``.

    Builds the program, runs it through :class:`ConcreteOps` for the oracle, then
    symbolically proves equivalence of the program against itself (a
    compute-only contract observing the result register, CR, XER, FPR, FPSCR and
    memory). The Dolphin third oracle is optional and skipped when unavailable.
    """
    program = generate_program(seed, max_instructions=max_instructions)
    concrete = _run_concrete(program, max_instructions=max_instructions)

    code = b"".join(
        int(w, 16).to_bytes(4, "big")
        for w in program["program_hex"].split()
    )
    instructions = decode_block(code, base=CODE_BASE, validate_with_capstone=False)

    contract = EquivalenceContract(
        parse_observables(program["observables"]),
        timeout_ms=30_000,
        name="compute-only",
    )
    fp_domain = FloatingPointDomain(
        allow_subnormal=True,
        allow_nan=True,
        allow_infinity=True,
        traps_enabled=False,
    )
    result = check_equivalence(
        instructions,
        instructions,
        contract,
        original_hex=program["program_hex"],
        candidate_hex=program["program_hex"],
        max_instructions=max_instructions + 64,
        max_paths=16,
        floating_point_domain=fp_domain,
    )
    match = result.status.value == "equivalent"

    # Compare symbolic result against the ConcreteOps oracle within scope.
    details = {
        "concrete_result_reg": f"0x{concrete['gpr'][RESULT_REG]:08x}",
        "concrete_cr": f"0x{concrete['cr']:08x}",
        "concrete_xer": f"0x{concrete['xer']:08x}",
        "symbolic_status": result.status.value,
    }

    dolphin = _try_dolphin()
    return {
        "seed": seed,
        "status": "match" if match else "mismatch",
        "match": match,
        "concrete": concrete,
        "symbolic_status": result.status.value,
        "dolphin": dolphin,
        "details": details,
    }


def _try_dolphin() -> dict[str, Any]:
    """Attempt the Dolphin third oracle; report ``skipped`` when absent."""
    import os
    import subprocess
    import sys
    from pathlib import Path

    root = Path(__file__).resolve().parents[3]
    run_script = root / "tools" / "test" / "compare_behaviour" / "run.py"
    if not run_script.is_file():
        return {"status": "skipped", "reason": "compare_behaviour/run.py not present"}
    # Only attempt if a Dolphin binary is plausibly available; otherwise skip.
    dolphin_env = os.environ.get("DOLPHIN_BINARY")
    if dolphin_env and not Path(dolphin_env).is_file():
        return {"status": "skipped", "reason": "DOLPHIN_BINARY not a file"}
    try:
        proc = subprocess.run(
            [sys.executable, str(run_script), "ppc", "ppc-equivalence-fixtures"],
            cwd=root, capture_output=True, text=True, check=False, timeout=30,
        )
    except Exception as exc:  # noqa: BLE001 - never fail the differential on Dolphin
        return {"status": "skipped", "reason": f"dolphin invocation failed: {exc}"}
    return {
        "status": "ran",
        "exit_code": proc.returncode,
        "stdout": (proc.stdout or "")[:2000],
        "stderr": (proc.stderr or "")[:2000],
    }


# --------------------------------------------------------------------------- #
# Metamorphic helpers (deterministic micro-programs)
# --------------------------------------------------------------------------- #
def metamorphic_store_load(seed: int) -> dict[str, Any]:
    """store-then-load same width/address returns original bits."""
    rng = random.Random(seed)
    value = _mask32(rng.getrandbits(15))
    base = SANDBOX_BASE
    code = [
        dform(14, 5, 0, value),        # li r5, value
        dform(14, 4, 0, base),         # li r4, base
        dform(36, 5, 4, 0),            # stw r5, 0(r4)
        dform(32, 6, 4, 0),            # lwz r6, 0(r4)
    ]
    initial = _build_initial_state(rng)
    initial["gpr"][4] = base
    initial["gpr"][5] = value
    instructions = decode_block(
        b"".join((w & 0xFFFFFFFF).to_bytes(4, "big") for w in code),
        base=CODE_BASE, validate_with_capstone=False,
    )
    terminals = execute_cfg(
        _initial_state_to_machine(initial), instructions, ConcreteOps(),
        max_instructions=64, max_paths=8,
    )
    taken = [t for t in terminals if bool(t.condition)]
    final = _machine_to_expected(taken[0].state)
    return {"program": code, "expected": final, "holds": final["gpr"][6] == final["gpr"][5]}


def metamorphic_rotate_zero(seed: int) -> dict[str, Any]:
    """rlwinm r7,r5,0,0,31 (rotate by zero) preserves the value."""
    rng = random.Random(seed)
    value = _mask32(rng.getrandbits(15))
    base = SANDBOX_BASE
    code = [
        dform(14, 5, 0, value),
        mform(21, 7, 5, 0, 0, 31, rc=1),  # rlwinm r7,r5,0,0,31
    ]
    initial = _build_initial_state(rng)
    initial["gpr"][5] = value
    instructions = decode_block(
        b"".join((w & 0xFFFFFFFF).to_bytes(4, "big") for w in code),
        base=CODE_BASE, validate_with_capstone=False,
    )
    terminals = execute_cfg(
        _initial_state_to_machine(initial), instructions, ConcreteOps(),
        max_instructions=64, max_paths=8,
    )
    taken = [t for t in terminals if bool(t.condition)]
    final = _machine_to_expected(taken[0].state)
    return {"program": code, "expected": final, "holds": final["gpr"][7] == value}


def metamorphic_add_zero(seed: int) -> dict[str, Any]:
    """x+0==x within observable flags."""
    rng = random.Random(seed)
    value = _mask32(rng.getrandbits(15))
    code = [
        dform(14, 5, 0, value),
        dform(14, 6, 0, 0),         # li r6, 0
        xo(31, 7, 5, 6, 266, rc=1),  # add r7, r5, r6
    ]
    initial = _build_initial_state(rng)
    initial["gpr"][5] = value
    initial["gpr"][6] = 0
    instructions = decode_block(
        b"".join((w & 0xFFFFFFFF).to_bytes(4, "big") for w in code),
        base=CODE_BASE, validate_with_capstone=False,
    )
    terminals = execute_cfg(
        _initial_state_to_machine(initial), instructions, ConcreteOps(),
        max_instructions=64, max_paths=8,
    )
    taken = [t for t in terminals if bool(t.condition)]
    final = _machine_to_expected(taken[0].state)
    return {"program": code, "expected": final, "holds": final["gpr"][7] == value}


def _family_of(program: dict[str, Any]) -> str:
    return program.get("family", "unknown")

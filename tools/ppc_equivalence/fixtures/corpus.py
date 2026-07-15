from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from .encode import BLR, bc, dform, mform, pack_xer, parse_int, words_to_hex, x_logical, xl, xo

SANDBOX_BASE = 0x00001000
CODE_BASE = 0x80018000
BLR_WORD = BLR


@dataclass(frozen=True, slots=True)
class FixtureCase:
    id: str
    tags: tuple[str, ...]
    initial: dict[str, Any]
    code_words: tuple[int, ...]
    result_reg: int
    expected_result: int
    expected_cr: int
    expected_xer: int
    expected_memory: dict[int, int] = field(default_factory=dict)
    expected_fpr: dict[int, int] = field(default_factory=dict)
    expected_fpscr: int | None = None
    max_instructions: int = 32
    allow_paths: int = 1

    @property
    def payload_hex(self) -> str:
        return words_to_hex(list(self.code_words))

    def to_json(self) -> dict[str, Any]:
        return {
            "id": self.id,
            "tags": list(self.tags),
            "initial": self.initial,
            "code": {"base": f"0x{CODE_BASE:08x}", "hex": self.payload_hex},
            "bounds": {"max_instructions": self.max_instructions, "allow_paths": self.allow_paths},
            "observe": {
                "result_reg": self.result_reg,
                "cr": True,
                "xer": True,
                "memory": {
                    f"0x{offset:08x}": f"0x{value:08x}"
                    for offset, value in sorted(self.expected_memory.items())
                },
                "fpr": [f"f{index}" for index in sorted(self.expected_fpr)],
                "fpscr": self.expected_fpscr is not None,
            },
            "expected": {
                "result": f"0x{self.expected_result & 0xFFFFFFFF:08x}",
                "cr": f"0x{self.expected_cr & 0xFFFFFFFF:08x}",
                "xer": f"0x{self.expected_xer & 0xFFFFFFFF:08x}",
                "fpr": {
                    f"f{index}": f"0x{value & 0xFFFFFFFFFFFFFFFF:016x}"
                    for index, value in sorted(self.expected_fpr.items())
                },
                "fpscr": (
                    f"0x{self.expected_fpscr & 0xFFFFFFFF:08x}"
                    if self.expected_fpscr is not None else None
                ),
            },
        }


def _gpr(**regs: int) -> dict[str, str]:
    # Call as _gpr(r5=..., r6=...); keep the kwarg names as register keys.
    return {name: f"0x{value & 0xFFFFFFFF:08x}" for name, value in regs.items()}


def _mem_bytes(words: dict[int, int]) -> dict[str, str]:
    out: dict[str, str] = {}
    for offset, value in words.items():
        value &= 0xFFFFFFFF
        for byte_index in range(4):
            addr = SANDBOX_BASE + offset + byte_index
            out[f"0x{addr:08x}"] = f"0x{(value >> (24 - 8 * byte_index)) & 0xFF:02x}"
    return out


def _state(
    gpr: dict[str, str] | None = None,
    *,
    cr: int = 0,
    xer: int = 0,
    lr: int = 0,
    ctr: int = 0,
    memory_words: dict[int, int] | None = None,
    fpr: dict[str, int] | None = None,
    fpscr: int = 0,
) -> dict[str, Any]:
    payload: dict[str, Any] = {
        "gpr": dict(gpr or {}),
        "cr": f"0x{cr & 0xFFFFFFFF:08x}",
        "xer": {
            "ca": (xer >> 29) & 1,
            "ov": (xer >> 30) & 1,
            "so": (xer >> 31) & 1,
        },
        "lr": f"0x{lr & 0xFFFFFFFF:08x}",
        "ctr": f"0x{ctr & 0xFFFFFFFF:08x}",
        "fpr": {name: f"0x{value & 0xFFFFFFFFFFFFFFFF:016x}" for name, value in (fpr or {}).items()},
        "fpscr": f"0x{fpscr & 0xFFFFFFFF:08x}",
    }
    if memory_words is not None:
        payload["memory"] = {"bytes": _mem_bytes(memory_words)}
        payload["gpr"].setdefault("r4", f"0x{SANDBOX_BASE:08x}")
    return payload


def _case(
    id_: str,
    tags: tuple[str, ...],
    initial: dict[str, Any],
    code: list[int],
    *,
    result: int,
    cr: int,
    xer: int,
    result_reg: int = 7,
    expected_memory: dict[int, int] | None = None,
    expected_fpr: dict[int, int] | None = None,
    expected_fpscr: int | None = None,
    allow_paths: int = 1,
) -> FixtureCase:
    return FixtureCase(
        id=id_,
        tags=tags,
        initial=initial,
        code_words=tuple(word & 0xFFFFFFFF for word in code),
        result_reg=result_reg,
        expected_result=result & 0xFFFFFFFF,
        expected_cr=cr & 0xFFFFFFFF,
        expected_xer=xer & 0xFFFFFFFF,
        expected_memory=dict(expected_memory or {}),
        expected_fpr=dict(expected_fpr or {}),
        expected_fpscr=expected_fpscr,
        allow_paths=allow_paths,
    )


def _mtspr(rs: int, spr: int) -> int:
    return xo(31, rs, spr & 0x1F, (spr >> 5) & 0x1F, 467)


def _mfspr(rt: int, spr: int) -> int:
    return xo(31, rt, spr & 0x1F, (spr >> 5) & 0x1F, 339)


def _rlwnm(ra: int, rs: int, rb: int, mb: int, me: int, *, rc: int = 0) -> int:
    return (
        (23 << 26)
        | ((rs & 0x1F) << 21)
        | ((ra & 0x1F) << 16)
        | ((rb & 0x1F) << 11)
        | ((mb & 0x1F) << 6)
        | ((me & 0x1F) << 1)
        | (rc & 1)
    )


def _cmp(ra: int, rb: int, *, unsigned: bool = False, field: int = 0) -> int:
    # Primary 31; xo 0 (cmp) / 32 (cmpl); L=0 for word; crfD in bits 6..8.
    xo_bits = 32 if unsigned else 0
    return (31 << 26) | ((field & 7) << 23) | ((ra & 0x1F) << 16) | ((rb & 0x1F) << 11) | (xo_bits << 1)


def _fp_a(primary: int, fd: int, fa: int, fb: int, fc: int, sub: int, *, rc: int = 0) -> int:
    return (
        ((primary & 0x3F) << 26) | ((fd & 31) << 21) | ((fa & 31) << 16)
        | ((fb & 31) << 11) | ((fc & 31) << 6) | (sub & 0x3E) | (rc & 1)
    )


def _fp_x(fd: int, fa: int, fb: int, xo_bits: int, *, rc: int = 0) -> int:
    return (
        (63 << 26) | ((fd & 31) << 21) | ((fa & 31) << 16)
        | ((fb & 31) << 11) | ((xo_bits & 0x3FF) << 1) | (rc & 1)
    )


def _fp_cmp(bf: int, fa: int, fb: int, xo_bits: int) -> int:
    # Compare encodes BF in bits 23..25; bits 21..22 are reserved and zero.
    return _fp_x((bf & 7) << 2, fa, fb, xo_bits)


def _psq_d(primary: int, fr: int, ra: int, disp: int, w: int, i: int) -> int:
    return (
        ((primary & 0x3F) << 26) | ((fr & 31) << 21) | ((ra & 31) << 16)
        | ((w & 1) << 15) | ((i & 7) << 12) | (disp & 0xFFF)
    )


def _psq_x(load: bool, update: bool, fr: int, ra: int, rb: int, w: int, i: int) -> int:
    return (
        (4 << 26) | ((fr & 31) << 21) | ((ra & 31) << 16) | ((rb & 31) << 11)
        | ((w & 1) << 10) | ((i & 7) << 7) | (int(update) << 6)
        | ((6 if load else 7) << 1)
    )


def _ps_x(fd: int, fa: int, fb: int, xo_bits: int, *, rc: int = 0) -> int:
    return (
        (4 << 26) | ((fd & 31) << 21) | ((fa & 31) << 16)
        | ((fb & 31) << 11) | ((xo_bits & 0x3FF) << 1) | (rc & 1)
    )


def _ps_a(
    fd: int, fa: int, fb: int, fc: int, xo5: int, *, rc: int = 0,
) -> int:
    return (
        (4 << 26) | ((fd & 31) << 21) | ((fa & 31) << 16)
        | ((fb & 31) << 11) | ((fc & 31) << 6)
        | ((xo5 & 31) << 1) | (rc & 1)
    )


def _mtfsf(fm: int, fb: int, *, rc: int = 0) -> int:
    return (63 << 26) | ((fm & 0xFF) << 17) | ((fb & 31) << 11) | (711 << 1) | (rc & 1)


def _mtfsfi(bf: int, imm4: int, *, rc: int = 0) -> int:
    return (63 << 26) | ((bf & 7) << 23) | ((imm4 & 0xF) << 12) | (134 << 1) | (rc & 1)


def _mcrfs(bf: int, bfa: int) -> int:
    return (63 << 26) | ((bf & 7) << 23) | ((bfa & 7) << 18) | (64 << 1)


FIXTURES: tuple[FixtureCase, ...] = (
    _case(
        "addc",
        ("integer", "xer.ca", "record"),
        _state(_gpr(r5=0xFFFFFFFF, r6=1)),
        [xo(31, 7, 5, 6, 10, rc=1)],
        result=0,
        cr=0x20000000,
        xer=pack_xer(ca=1),
    ),
    _case(
        "addeo",
        ("integer", "xer.ov", "xer.so", "record"),
        _state(_gpr(r5=0x7FFFFFFF, r6=0), xer=pack_xer(ca=1)),
        [xo(31, 7, 5, 6, 138, rc=1, oe=1)],
        result=0x80000000,
        cr=0x90000000,
        xer=pack_xer(ov=1, so=1),
    ),
    _case(
        "addmeo",
        ("integer", "xer.ca", "xer.ov", "record"),
        _state(_gpr(r5=0x80000000)),
        [xo(31, 7, 5, 0, 234, rc=1, oe=1)],
        result=0x7FFFFFFF,
        cr=0x50000000,
        xer=pack_xer(ca=1, ov=1, so=1),
    ),
    _case(
        "subfc",
        ("integer", "xer.ca", "record"),
        _state(_gpr(r5=1, r6=0)),
        [xo(31, 7, 5, 6, 8, rc=1)],
        result=0xFFFFFFFF,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "subfeo",
        ("integer", "record"),
        _state(_gpr(r5=0, r6=0)),
        [xo(31, 7, 5, 6, 136, rc=1, oe=1)],
        result=0xFFFFFFFF,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "nego",
        ("integer", "xer.ov", "record"),
        _state(_gpr(r5=0x80000000), xer=pack_xer(ca=1)),
        [xo(31, 7, 5, 0, 104, rc=1, oe=1)],
        result=0x80000000,
        cr=0x90000000,
        xer=pack_xer(ca=1, ov=1, so=1),
    ),
    _case(
        "mulhw",
        ("integer", "record"),
        _state(_gpr(r5=0xFFFFFFFE, r6=3)),
        [xo(31, 7, 5, 6, 75, rc=1)],
        result=0xFFFFFFFF,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "mulhwu",
        ("integer", "record"),
        _state(_gpr(r5=0xFFFFFFFF, r6=2)),
        [xo(31, 7, 5, 6, 11, rc=1)],
        result=1,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "mullwo",
        ("integer", "xer.ov", "record"),
        _state(_gpr(r5=0x40000000, r6=4)),
        [xo(31, 7, 5, 6, 235, rc=1, oe=1)],
        result=0,
        cr=0x30000000,
        xer=pack_xer(ov=1, so=1),
    ),
    _case(
        "divw",
        ("integer", "record"),
        _state(_gpr(r5=0xFFFFFFF9, r6=3)),
        [xo(31, 7, 5, 6, 491, rc=1)],
        result=0xFFFFFFFE,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "divwu",
        ("integer", "record"),
        _state(_gpr(r5=0xFFFFFFFF, r6=2)),
        [xo(31, 7, 5, 6, 459, rc=1)],
        result=0x7FFFFFFF,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "slw31",
        ("shift", "record"),
        _state(_gpr(r5=1, r6=31)),
        [x_logical(7, 5, 6, 24, rc=1)],
        result=0x80000000,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "slw32",
        ("shift", "record"),
        _state(_gpr(r5=1, r6=32)),
        [x_logical(7, 5, 6, 24, rc=1)],
        result=0,
        cr=0x20000000,
        xer=0,
    ),
    _case(
        "srw",
        ("shift", "record"),
        _state(_gpr(r5=0x80000000, r6=31)),
        [x_logical(7, 5, 6, 536, rc=1)],
        result=1,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "sraw",
        ("shift", "xer.ca", "record"),
        _state(_gpr(r5=0xFFFFFFFF, r6=1)),
        [x_logical(7, 5, 6, 792, rc=1)],
        result=0xFFFFFFFF,
        cr=0x80000000,
        xer=pack_xer(ca=1),
    ),
    _case(
        "srawi",
        ("shift", "xer.ca", "record"),
        _state(_gpr(r5=0x80000001)),
        # srawi embeds SH in the rB field.
        [xo(31, 5, 7, 1, 824, rc=1)],
        result=0xC0000000,
        cr=0x80000000,
        xer=pack_xer(ca=1),
    ),
    _case(
        "rlwinm",
        ("rotate", "record"),
        _state(_gpr(r5=3)),
        [mform(21, 7, 5, 2, 0, 29, rc=1)],
        result=0xC,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "rlwimi",
        ("rotate", "record"),
        _state(_gpr(r5=0x12345678, r7=0xAABBCCDD)),
        [mform(20, 7, 5, 8, 8, 15, rc=1)],
        result=0xAA56CCDD,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "rlwnm",
        ("rotate", "record"),
        _state(_gpr(r5=0x80000001, r6=1)),
        [_rlwnm(7, 5, 6, 0, 31, rc=1)],
        result=3,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "andc",
        ("logical", "record"),
        _state(_gpr(r5=0xF0F0F0F0, r6=0xFF00FF00)),
        [x_logical(7, 5, 6, 60, rc=1)],
        result=0x00F000F0,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "eqv",
        ("logical", "record"),
        _state(_gpr(r5=0xAAAAAAAA, r6=0x0F0F0F0F)),
        [x_logical(7, 5, 6, 284, rc=1)],
        result=0x5A5A5A5A,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "nand",
        ("logical", "record"),
        _state(_gpr(r5=0xFFFF0000, r6=0x0F0F0F0F)),
        [x_logical(7, 5, 6, 476, rc=1)],
        result=0xF0F0FFFF,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "nor",
        ("logical", "record"),
        _state(_gpr(r5=0xF0000000, r6=0x0F000000)),
        [x_logical(7, 5, 6, 124, rc=1)],
        result=0x00FFFFFF,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "orc",
        ("logical", "record"),
        _state(_gpr(r5=0x12345678, r6=0xFFFF0000)),
        [x_logical(7, 5, 6, 412, rc=1)],
        result=0x1234FFFF,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "xor",
        ("logical", "record"),
        _state(_gpr(r5=0xAAAA5555, r6=0xFFFF0000)),
        [x_logical(7, 5, 6, 316, rc=1)],
        result=0x55555555,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "extsb",
        ("integer", "record"),
        _state(_gpr(r5=0x80)),
        [x_logical(7, 5, 0, 954, rc=1)],
        result=0xFFFFFF80,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "extsh",
        ("integer", "record"),
        _state(_gpr(r5=0x8001)),
        [x_logical(7, 5, 0, 922, rc=1)],
        result=0xFFFF8001,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "cntlzw",
        ("integer", "record"),
        _state(_gpr(r5=0x00010000)),
        [x_logical(7, 5, 0, 26, rc=1)],
        result=15,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "cmpw",
        ("compare",),
        _state(_gpr(r5=0xFFFFFFFF, r6=1, r7=0)),
        [_cmp(5, 6)],
        result=0,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "cmplw",
        ("compare",),
        _state(_gpr(r5=0xFFFFFFFF, r6=1, r7=0)),
        [_cmp(5, 6, unsigned=True)],
        result=0,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "cr-logical",
        ("cr",),
        _state(_gpr(r7=0), cr=0x40000000),
        [
            xl(19, 0, 1, 2, 257),
            xl(19, 3, 1, 2, 417),
            xl(19, 4, 1, 2, 289),
            xl(19, 5, 1, 2, 193),
            xo(31, 7, 0, 0, 19),
        ],
        result=0x54000000,
        cr=0x54000000,
        xer=0,
    ),
    _case(
        "xer-transfer",
        ("xer", "spr"),
        _state(_gpr(r5=0xE0000000, r7=0)),
        [_mtspr(5, 1), _mfspr(7, 1)],
        result=0xE0000000,
        cr=0,
        xer=0xE0000000,
    ),
    _case(
        "stw-lbz-be",
        ("memory",),
        _state(_gpr(r5=0x11223344), memory_words={}),
        [dform(36, 5, 4, 0), dform(34, 7, 4, 0)],
        result=0x11,
        cr=0,
        xer=0,
        expected_memory={0: 0x11223344},
    ),
    _case(
        "lhz-be",
        ("memory",),
        _state(_gpr(r7=0), memory_words={0: 0x11223344}),
        [dform(40, 7, 4, 2)],
        result=0x3344,
        cr=0,
        xer=0,
        expected_memory={0: 0x11223344},
    ),
    _case(
        "lha",
        ("memory",),
        _state(_gpr(r5=0x8001), memory_words={}),
        [dform(44, 5, 4, 4), dform(42, 7, 4, 4)],
        result=0xFFFF8001,
        cr=0,
        xer=0,
    ),
    _case(
        "byte-reverse",
        ("memory",),
        _state(_gpr(r5=0x11223344, r6=8), memory_words={}),
        [xo(31, 5, 4, 6, 662), xo(31, 7, 4, 6, 534)],
        result=0x11223344,
        cr=0,
        xer=0,
        expected_memory={8: 0x44332211},
    ),
    _case(
        "stwu",
        ("memory", "update"),
        _state(_gpr(r5=0x11223344, r10=SANDBOX_BASE + 16), memory_words={}),
        [dform(37, 5, 10, (-4) & 0xFFFF), xo(31, 7, 4, 10, 40)],
        result=0xC,
        cr=0,
        xer=0,
        expected_memory={12: 0x11223344},
    ),
    _case(
        "lmw-stmw",
        ("memory",),
        _state(_gpr(), memory_words={16: 1, 20: 2, 24: 3, 28: 4}),
        [
            dform(46, 28, 4, 16),
            dform(47, 28, 4, 48),
            x_logical(7, 28, 29, 316),
            x_logical(7, 7, 30, 316),
            x_logical(7, 7, 31, 316),
        ],
        result=1 ^ 2 ^ 3 ^ 4,
        cr=0,
        xer=0,
        expected_memory={48: 1, 52: 2, 56: 3, 60: 4},
    ),
    _case(
        "bc-cr",
        ("branch",),
        _state(_gpr(r7=0), cr=0x80000000),
        [bc(12, 0, 2), dform(14, 7, 0, 1)],
        result=0,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "bdnz",
        ("branch", "ctr"),
        _state(_gpr(r7=0), ctr=2),
        [
            bc(16, 0, 2),
            dform(14, 7, 0, 1),
            mform(21, 7, 7, 8, 0, 23),
            _mfspr(8, 9),
            x_logical(7, 7, 8, 444),
        ],
        result=0x00000001,
        cr=0,
        xer=0,
    ),
    # --- Edge expansions (thin families / corner rules) ---
    _case(
        "li",
        ("integer",),
        _state(_gpr(r7=0xDEAD)),
        [dform(14, 7, 0, 0x1234)],
        result=0x1234,
        cr=0,
        xer=0,
    ),
    _case(
        "addze",
        ("integer", "xer.ca", "record"),
        _state(_gpr(r5=1), xer=pack_xer(ca=1)),
        [xo(31, 7, 5, 0, 202, rc=1)],
        result=2,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "add-so-sticky",
        ("integer", "xer.so", "xer.ov", "record"),
        _state(_gpr(r5=1, r6=2), xer=pack_xer(so=1)),
        [xo(31, 7, 5, 6, 266, rc=1, oe=1)],
        result=3,
        cr=0x50000000,  # GT | SO
        xer=pack_xer(so=1),
    ),
    _case(
        "sraw32",
        ("shift", "xer.ca", "record"),
        _state(_gpr(r5=0x80000000, r6=32)),
        [x_logical(7, 5, 6, 792, rc=1)],
        result=0xFFFFFFFF,
        cr=0x80000000,
        xer=pack_xer(ca=1),
    ),
    _case(
        "and.",
        ("logical", "record"),
        _state(_gpr(r5=0xFF00, r6=0x0FF0)),
        [x_logical(7, 5, 6, 28, rc=1)],
        result=0x0F00,
        cr=0x40000000,
        xer=0,
    ),
    _case(
        "cmpwi",
        ("compare",),
        _state(_gpr(r5=0, r7=0)),
        [(11 << 26) | (0 << 23) | (5 << 16) | 1],
        result=0,
        cr=0x80000000,
        xer=0,
    ),
    _case(
        "mtcrf-cr0",
        ("cr",),
        _state(_gpr(r5=0x12345678, r7=0), cr=0),
        [(31 << 26) | (5 << 21) | (0x80 << 12) | (144 << 1), xo(31, 7, 0, 0, 19)],
        result=0x10000000,
        cr=0x10000000,
        xer=0,
    ),
    _case(
        "lbzu",
        ("memory", "update"),
        _state(_gpr(r10=SANDBOX_BASE), memory_words={0: 0x11223344}),
        [
            dform(35, 8, 10, 1),  # lbzu r8, 1(r10)
            xo(31, 7, 4, 10, 40),  # subf r7, r4, r10 => updated base delta
        ],
        result=1,
        cr=0,
        xer=0,
        expected_memory={0: 0x11223344},
    ),
    _case(
        "lhbrx",
        ("memory",),
        _state(_gpr(r5=0x1122, r6=8), memory_words={}),
        [dform(44, 5, 4, 8), xo(31, 7, 4, 6, 790)],
        result=0x2211,
        cr=0,
        xer=0,
    ),
    _case(
        "bc-cr-fallthrough",
        ("branch",),
        _state(_gpr(r7=0), cr=0),
        [bc(12, 0, 2), dform(14, 7, 0, 1)],
        result=1,
        cr=0,
        xer=0,
    ),
    _case(
        "subfze",
        ("integer", "xer.ca", "record"),
        _state(_gpr(r5=0), xer=pack_xer(ca=1)),
        [xo(31, 7, 5, 0, 200, rc=1)],
        # ~rA + CA with rA=0, CA=1 -> 0; CA stays set (no borrow-out in this form).
        result=0,
        cr=0x20000000,
        xer=pack_xer(ca=1),
    ),
)


# One interpreter-backed smoke case for every otherwise-unrepresented opcode.
# Edge-heavy family cases remain above; this block prevents accepted decoder
# growth from silently outrunning the ConcreteOps/Dolphin corpus.
FIXTURES += (
    _case("addic", ("integer", "xer.ca"), _state(_gpr(r5=0xFFFFFFFF)), [dform(12, 7, 5, 1)], result=0, cr=0, xer=pack_xer(ca=1)),
    _case("addic-dot", ("integer", "xer.ca", "record"), _state(_gpr(r5=0xFFFFFFFF)), [dform(13, 7, 5, 1)], result=0, cr=0x20000000, xer=pack_xer(ca=1)),
    _case("addis", ("integer",), _state(_gpr(r5=2)), [dform(15, 7, 5, 1)], result=0x00010002, cr=0, xer=0),
    _case("andi-dot", ("logical", "record"), _state(_gpr(r5=0x1234)), [dform(28, 5, 7, 0x00FF)], result=0x34, cr=0x40000000, xer=0),
    _case("andis-dot", ("logical", "record"), _state(_gpr(r5=0x12345678)), [dform(29, 5, 7, 0x00FF)], result=0x00340000, cr=0x40000000, xer=0),
    _case("cmplwi", ("compare",), _state(_gpr(r5=0xFFFFFFFF, r7=0)), [(10 << 26) | (5 << 16) | 1], result=0, cr=0x40000000, xer=0),
    _case("mulli", ("integer",), _state(_gpr(r5=3)), [dform(7, 7, 5, 0xFFFE)], result=0xFFFFFFFA, cr=0, xer=0),
    _case("ori", ("logical",), _state(_gpr(r5=0x1000)), [dform(24, 5, 7, 0x23)], result=0x1023, cr=0, xer=0),
    _case("oris", ("logical",), _state(_gpr(r5=0x1000)), [dform(25, 5, 7, 0x23)], result=0x00231000, cr=0, xer=0),
    _case("subfic", ("integer", "xer.ca"), _state(_gpr(r5=1)), [dform(8, 7, 5, 0)], result=0xFFFFFFFF, cr=0, xer=0),
    _case("subfme", ("integer", "xer.ca", "record"), _state(_gpr(r5=0), xer=pack_xer(ca=1)), [xo(31, 7, 5, 0, 232, rc=1)], result=0xFFFFFFFF, cr=0x80000000, xer=0),
    _case("addme-carry-in", ("integer", "xer.ca", "record"), _state(_gpr(r5=0), xer=pack_xer(ca=1)), [xo(31, 7, 5, 0, 234, rc=1)], result=0, cr=0x20000000, xer=0),
    _case("subfme-carry-clear", ("integer", "xer.ca", "record"), _state(_gpr(r5=0)), [xo(31, 7, 5, 0, 232, rc=1)], result=0xFFFFFFFE, cr=0x80000000, xer=pack_xer(ca=1)),
    _case("xori", ("logical",), _state(_gpr(r5=0x1234)), [dform(26, 5, 7, 0x00FF)], result=0x12CB, cr=0, xer=0),
    _case("xoris", ("logical",), _state(_gpr(r5=0x12345678)), [dform(27, 5, 7, 0x00FF)], result=0x12CB5678, cr=0, xer=0),

    _case("crandc", ("cr",), _state(_gpr(r7=0), cr=0x80000000), [xl(19, 2, 0, 1, 129), xo(31, 7, 0, 0, 19)], result=0xA0000000, cr=0xA0000000, xer=0),
    _case("crnand", ("cr",), _state(_gpr(r7=0), cr=0xC0000000), [xl(19, 2, 0, 1, 225), xo(31, 7, 0, 0, 19)], result=0xC0000000, cr=0xC0000000, xer=0),
    _case("crnor", ("cr",), _state(_gpr(r7=0), cr=0), [xl(19, 2, 0, 1, 33), xo(31, 7, 0, 0, 19)], result=0x20000000, cr=0x20000000, xer=0),
    _case("cror", ("cr",), _state(_gpr(r7=0), cr=0x80000000), [xl(19, 2, 0, 1, 449), xo(31, 7, 0, 0, 19)], result=0xA0000000, cr=0xA0000000, xer=0),

    _case("b-direct", ("branch",), _state(_gpr(r7=0)), [(18 << 26) | (2 << 2), dform(14, 7, 0, 1), dform(14, 7, 0, 2)], result=2, cr=0, xer=0),
    _case("bclr-not-taken", ("branch",), _state(_gpr(r7=0), cr=0), [xl(19, 12, 0, 0, 16), dform(14, 7, 0, 1)], result=1, cr=0, xer=0),
    _case("bcctr-not-taken", ("branch",), _state(_gpr(r7=0), cr=0, ctr=4), [xl(19, 12, 0, 0, 528), dform(14, 7, 0, 1)], result=1, cr=0, xer=0),

    _case("lwz", ("memory",), _state(_gpr(r7=0), memory_words={4: 0x89ABCDEF}), [dform(32, 7, 4, 4)], result=0x89ABCDEF, cr=0, xer=0, expected_memory={4: 0x89ABCDEF}),
    _case("lwzu", ("memory", "update"), _state(_gpr(r7=0), memory_words={4: 0x89ABCDEF}), [dform(33, 7, 4, 4)], result=0x89ABCDEF, cr=0, xer=0, expected_memory={4: 0x89ABCDEF}),
    _case("lbzx", ("memory",), _state(_gpr(r6=1, r7=0), memory_words={0: 0x11223344}), [xo(31, 7, 4, 6, 87)], result=0x22, cr=0, xer=0, expected_memory={0: 0x11223344}),
    _case("lbzux", ("memory", "update"), _state(_gpr(r6=1, r7=0), memory_words={0: 0x11223344}), [xo(31, 7, 4, 6, 119)], result=0x22, cr=0, xer=0, expected_memory={0: 0x11223344}),
    _case("lhzu", ("memory", "update"), _state(_gpr(r7=0), memory_words={0: 0x11223344}), [dform(41, 7, 4, 2)], result=0x3344, cr=0, xer=0, expected_memory={0: 0x11223344}),
    _case("lhau", ("memory", "update"), _state(_gpr(r7=0), memory_words={0: 0x11228001}), [dform(43, 7, 4, 2)], result=0xFFFF8001, cr=0, xer=0, expected_memory={0: 0x11228001}),
    _case("lhzx", ("memory",), _state(_gpr(r6=2, r7=0), memory_words={0: 0x11223344}), [xo(31, 7, 4, 6, 279)], result=0x3344, cr=0, xer=0, expected_memory={0: 0x11223344}),
    _case("lhzux", ("memory", "update"), _state(_gpr(r6=2, r7=0), memory_words={0: 0x11223344}), [xo(31, 7, 4, 6, 311)], result=0x3344, cr=0, xer=0, expected_memory={0: 0x11223344}),
    _case("lhax", ("memory",), _state(_gpr(r6=2, r7=0), memory_words={0: 0x11228001}), [xo(31, 7, 4, 6, 343)], result=0xFFFF8001, cr=0, xer=0, expected_memory={0: 0x11228001}),
    _case("lhaux", ("memory", "update"), _state(_gpr(r6=2, r7=0), memory_words={0: 0x11228001}), [xo(31, 7, 4, 6, 375)], result=0xFFFF8001, cr=0, xer=0, expected_memory={0: 0x11228001}),
    _case("lwzx", ("memory",), _state(_gpr(r6=4, r7=0), memory_words={4: 0x89ABCDEF}), [xo(31, 7, 4, 6, 23)], result=0x89ABCDEF, cr=0, xer=0, expected_memory={4: 0x89ABCDEF}),
    _case("lwzux", ("memory", "update"), _state(_gpr(r6=4, r7=0), memory_words={4: 0x89ABCDEF}), [xo(31, 7, 4, 6, 55)], result=0x89ABCDEF, cr=0, xer=0, expected_memory={4: 0x89ABCDEF}),

    _case("stb", ("memory",), _state(_gpr(r5=0xAA), memory_words={0: 0}), [dform(38, 5, 4, 1)], result=0, cr=0, xer=0, expected_memory={0: 0x00AA0000}),
    _case("stbu", ("memory", "update"), _state(_gpr(r5=0xAA), memory_words={0: 0}), [dform(39, 5, 4, 1)], result=0, cr=0, xer=0, expected_memory={0: 0x00AA0000}),
    _case("stbx", ("memory",), _state(_gpr(r5=0xAA, r6=1), memory_words={0: 0}), [xo(31, 5, 4, 6, 215)], result=0, cr=0, xer=0, expected_memory={0: 0x00AA0000}),
    _case("stbux", ("memory", "update"), _state(_gpr(r5=0xAA, r6=1), memory_words={0: 0}), [xo(31, 5, 4, 6, 247)], result=0, cr=0, xer=0, expected_memory={0: 0x00AA0000}),
    _case("sthu", ("memory", "update"), _state(_gpr(r5=0xAABB), memory_words={0: 0}), [dform(45, 5, 4, 2)], result=0, cr=0, xer=0, expected_memory={0: 0x0000AABB}),
    _case("sthx", ("memory",), _state(_gpr(r5=0xAABB, r6=2), memory_words={0: 0}), [xo(31, 5, 4, 6, 407)], result=0, cr=0, xer=0, expected_memory={0: 0x0000AABB}),
    _case("sthux", ("memory", "update"), _state(_gpr(r5=0xAABB, r6=2), memory_words={0: 0}), [xo(31, 5, 4, 6, 439)], result=0, cr=0, xer=0, expected_memory={0: 0x0000AABB}),
    _case("sthbrx", ("memory",), _state(_gpr(r5=0x1122, r6=2), memory_words={0: 0}), [xo(31, 5, 4, 6, 918)], result=0, cr=0, xer=0, expected_memory={0: 0x00002211}),
    _case("stwx", ("memory",), _state(_gpr(r5=0x11223344, r6=4), memory_words={4: 0}), [xo(31, 5, 4, 6, 151)], result=0, cr=0, xer=0, expected_memory={4: 0x11223344}),
    _case("stwux", ("memory", "update"), _state(_gpr(r5=0x11223344, r6=4), memory_words={4: 0}), [xo(31, 5, 4, 6, 183)], result=0, cr=0, xer=0, expected_memory={4: 0x11223344}),
)


# Scalar floating-point value-semantics corpus.  These cases are also executed
# by Dolphin's Broadway interpreter and compare FPR/FPSCR state directly.
_F15 = 0x3FF8000000000000
_F1 = 0x3FF0000000000000
_F2 = 0x4000000000000000
_F4 = 0x4010000000000000
_QNAN = 0x7FF8000012345678
_SNAN = 0x7FF0000012345678
_PINF = 0x7FF0000000000000
_NINF = 0xFFF0000000000000
_NZERO = 0x8000000000000000
_FP_INPUTS = {"f1": _F15, "f2": _F2, "f3": _F4}


def _paired_simple_case(
    id_: str, xo_bits: int, expected_ps0: int, expected_words: tuple[int, int],
    *, merge: bool = False, rc: int = 0,
) -> FixtureCase:
    # OX is directly writable through the harness mtfsf initialization path;
    # derived FX/VX summary bits cannot be injected without their causes.
    initial_fpscr = 0x10000000 if rc else 0
    return _case(
        id_, ("fp", "paired", "move" if not merge else "merge", "lane-bits"),
        _state(
            _gpr(r5=0), fpscr=initial_fpscr,
            memory_words={
                0: 0x3FC00000, 4: 0xC0000000,
                8: 0xC0400000, 12: 0x40800000,
                16: 0, 20: 0,
            },
        ),
        [
            _mtspr(5, 913),
            _psq_d(56, 1, 4, 0, 0, 1),
            _psq_d(56, 2, 4, 8, 0, 1),
            _ps_x(7, 1 if merge else 0, 2 if merge else 1, xo_bits, rc=rc),
            _psq_d(60, 7, 4, 16, 0, 1),
        ],
        result=0, cr=0x01000000 if rc else 0, xer=0,
        expected_memory={16: expected_words[0], 20: expected_words[1]},
        expected_fpr={7: expected_ps0},
        expected_fpscr=initial_fpscr,
    )


def _paired_arithmetic_case(
    id_: str, xo5: int, source_words: tuple[int, int],
    expected_words: tuple[int, int], expected_ps0: int,
    *, multiply: bool = False, rc: int = 0,
) -> FixtureCase:
    initial_fpscr = 0x10000000 if rc else 0
    expected_fprf = 0x8000 if expected_ps0 & (1 << 63) else 0x4000
    return _case(
        id_, ("fp", "paired", "arithmetic", "round-single", "lanes"),
        _state(
            _gpr(r5=0), fpscr=initial_fpscr,
            memory_words={
                0: 0x3FC00000, 4: 0xC0000000,
                8: source_words[0], 12: source_words[1],
                16: 0, 20: 0,
            },
        ),
        [
            _mtspr(5, 913),
            _psq_d(56, 1, 4, 0, 0, 1),
            _psq_d(56, 2, 4, 8, 0, 1),
            _ps_a(7, 1, 0 if multiply else 2, 2 if multiply else 0, xo5, rc=rc),
            _psq_d(60, 7, 4, 16, 0, 1),
        ],
        result=0, cr=0x01000000 if rc else 0, xer=0,
        expected_memory={16: expected_words[0], 20: expected_words[1]},
        expected_fpr={7: expected_ps0},
        expected_fpscr=initial_fpscr | expected_fprf,
    )


def _paired_fused_case(
    id_: str, xo5: int, expected_words: tuple[int, int], expected_ps0: int,
    *, rc: int = 0,
) -> FixtureCase:
    initial_fpscr = 0x10000000 if rc else 0
    expected_fprf = 0x8000 if expected_ps0 & (1 << 63) else 0x4000
    return _case(
        id_, ("fp", "paired", "fused", "round-single", "lanes"),
        _state(
            _gpr(r5=0), fpscr=initial_fpscr,
            memory_words={
                0: 0x3FC00000, 4: 0xC0000000,
                8: 0x40000000, 12: 0x40800000,
                16: 0x40800000, 20: 0xBF000000,
                24: 0, 28: 0,
            },
        ),
        [
            _mtspr(5, 913),
            _psq_d(56, 1, 4, 0, 0, 1),
            _psq_d(56, 2, 4, 8, 0, 1),
            _psq_d(56, 3, 4, 16, 0, 1),
            _ps_a(7, 1, 2, 3, xo5, rc=rc),
            _psq_d(60, 7, 4, 24, 0, 1),
        ],
        result=0, cr=0x01000000 if rc else 0, xer=0,
        expected_memory={24: expected_words[0], 28: expected_words[1]},
        expected_fpr={7: expected_ps0},
        expected_fpscr=initial_fpscr | expected_fprf,
    )


def _paired_sum_case(
    id_: str, xo5: int, expected_words: tuple[int, int], expected_ps0: int,
    expected_fprf: int, *, rc: int = 0,
) -> FixtureCase:
    initial_fpscr = 0x10000000 if rc else 0
    return _case(
        id_, ("fp", "paired", "sum", "cross-lane", "copy", "round-single"),
        _state(
            _gpr(r5=0), fpscr=initial_fpscr,
            memory_words={
                0: 0x3FC00000, 4: 0x42C60000,
                8: 0x41200000, 12: 0xC0000000,
                16: 0x40400000, 20: 0x40800000,
                24: 0, 28: 0,
            },
        ),
        [
            _mtspr(5, 913),
            _psq_d(56, 1, 4, 0, 0, 1),
            _psq_d(56, 2, 4, 8, 0, 1),
            _psq_d(56, 3, 4, 16, 0, 1),
            _ps_a(7, 1, 2, 3, xo5, rc=rc),
            _psq_d(60, 7, 4, 24, 0, 1),
        ],
        result=0, cr=0x01000000 if rc else 0, xer=0,
        expected_memory={24: expected_words[0], 28: expected_words[1]},
        expected_fpr={7: expected_ps0},
        expected_fpscr=initial_fpscr | expected_fprf,
    )


FIXTURES += (
    _case("mffs", ("fp", "fpscr", "read"), _state(fpscr=0xA0100000), [_fp_x(7, 0, 0, 583)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF80000A0100000}, expected_fpscr=0xA0100000),
    _case("mtfsf", ("fp", "fpscr", "write", "record"), _state(fpr={"f2": 0xFFF8000000100880}), [_mtfsf(0xFF, 2, rc=1)], result=0, cr=0x06000000, xer=0, expected_fpscr=0x60100080),
    _case("mtfsfi", ("fp", "fpscr", "immediate"), _state(), [_mtfsfi(7, 3)], result=0, cr=0, xer=0, expected_fpscr=3),
    _case("mtfsb1", ("fp", "fpscr", "bit", "exception"), _state(fpscr=0x80), [_fp_x(11, 0, 0, 38)], result=0, cr=0, xer=0, expected_fpscr=0xE0100080),
    _case("mtfsb0", ("fp", "fpscr", "bit", "summary"), _state(fpscr=0xE0100080), [_fp_x(11, 0, 0, 70)], result=0, cr=0, xer=0, expected_fpscr=0x80000080),
    _case("mcrfs", ("fp", "fpscr", "cr", "clear"), _state(fpscr=0xA0100000), [_mcrfs(3, 2)], result=0, cr=0x00010000, xer=0, expected_fpscr=0x80000000),
    _paired_simple_case("ps_neg", 40, 0xBFF8000000000000, (0xBFC00000, 0x40000000), rc=1),
    _paired_simple_case("ps_mr", 72, _F15, (0x3FC00000, 0xC0000000)),
    _paired_simple_case("ps_nabs", 136, 0xBFF8000000000000, (0xBFC00000, 0xC0000000)),
    _paired_simple_case("ps_abs", 264, _F15, (0x3FC00000, 0x40000000)),
    _paired_simple_case("ps_merge00", 528, _F15, (0x3FC00000, 0xC0400000), merge=True),
    _paired_simple_case("ps_merge01", 560, _F15, (0x3FC00000, 0x40800000), merge=True),
    _paired_simple_case("ps_merge10", 592, 0xC000000000000000, (0xC0000000, 0xC0400000), merge=True),
    _paired_simple_case("ps_merge11", 624, 0xC000000000000000, (0xC0000000, 0x40800000), merge=True),
    _case("ps_cmpu0", ("fp", "paired", "compare", "unordered", "ps0"), _state(fpr={"f1": _F15, "f2": _F2}), [_ps_x(3 << 2, 1, 2, 0)], result=0, cr=0x00080000, xer=0, expected_fpscr=0x00008000),
    _case("ps_cmpo0", ("fp", "paired", "compare", "ordered", "ps0", "qnan"), _state(fpr={"f1": _QNAN, "f2": _F2}), [_ps_x(3 << 2, 1, 2, 32)], result=0, cr=0x00010000, xer=0, expected_fpscr=0xA0081000),
    _case("ps_cmpu1", ("fp", "paired", "compare", "unordered", "ps1", "snan"), _state(fpr={"f1": _SNAN, "f2": _F2}), [_ps_x(3, 2, 1, 528), _ps_x(4, 2, 2, 528), _ps_x(3 << 2, 3, 4, 64)], result=0, cr=0x00010000, xer=0, expected_fpscr=0xA1001000),
    _case("ps_cmpo1", ("fp", "paired", "compare", "ordered", "ps1", "snan", "ve"), _state(fpr={"f1": _SNAN, "f2": _F2}, fpscr=0x80), [_ps_x(3, 2, 1, 528), _ps_x(4, 2, 2, 528), _ps_x(3 << 2, 3, 4, 96)], result=0, cr=0x00010000, xer=0, expected_fpscr=0xE1001080),
    _paired_arithmetic_case("ps_add", 21, (0x40000000, 0x40800000), (0x40600000, 0x40000000), 0x400C000000000000, rc=1),
    _paired_arithmetic_case("ps_sub", 20, (0x40000000, 0x40800000), (0xBF000000, 0xC0C00000), 0xBFE0000000000000),
    _paired_arithmetic_case("ps_mul", 25, (0x40800000, 0xBF000000), (0x40C00000, 0x3F800000), 0x4018000000000000, multiply=True),
    _paired_arithmetic_case("ps_muls0", 12, (0x40800000, 0xBF000000), (0x40C00000, 0xC1000000), 0x4018000000000000, multiply=True),
    _paired_arithmetic_case("ps_muls1", 13, (0x40800000, 0xBF000000), (0xBF400000, 0x3F800000), 0xBFE8000000000000, multiply=True),
    _case("ps_mul-force25", ("fp", "paired", "multiply", "force25", "preserve-fifr"), _state(fpr={"f1": _F1, "f3": 0x3FF0000010000001}, fpscr=0x00060000), [_ps_x(1, 1, 1, 528), _ps_x(3, 3, 3, 528), _ps_a(7, 1, 0, 3, 25)], result=0, cr=0, xer=0, expected_fpr={7: _F1}, expected_fpscr=0x00064000),
    _case("ps_add-lane-exceptions", ("fp", "paired", "add", "invalid", "lanes", "record", "clear-fifr"), _state(fpr={"f1": _PINF, "f2": _NINF, "f3": _SNAN, "f4": _F2}, fpscr=0x00060000), [_ps_x(5, 1, 3, 528), _ps_x(6, 2, 4, 528), _ps_a(7, 5, 6, 0, 21, rc=1)], result=0, cr=0x0A000000, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA1811000),
    _case("ps_add-ve-writes", ("fp", "paired", "add", "invalid", "ve", "write-result"), _state(fpr={"f1": _PINF, "f2": _F1, "f3": _NINF, "f4": _F2, "f7": _F1}, fpscr=0x80), [_ps_x(5, 1, 2, 528), _ps_x(6, 3, 4, 528), _ps_a(7, 5, 6, 0, 21)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xE0811080),
    _case("ps_mul-vximz-ps1", ("fp", "paired", "multiply", "invalid", "ps1"), _state(fpr={"f1": _F1, "f2": 0, "f3": _F2, "f4": _PINF}), [_ps_x(5, 1, 2, 528), _ps_x(6, 3, 4, 528), _ps_a(7, 5, 0, 6, 25)], result=0, cr=0, xer=0, expected_fpr={7: _F2}, expected_fpscr=0xA0104000),
    _paired_fused_case("ps_madd", 29, (0x41000000, 0x40A00000), 0x4020000000000000, rc=1),
    _paired_fused_case("ps_msub", 28, (0x40800000, 0xC0400000), 0x4010000000000000),
    _paired_fused_case("ps_nmadd", 31, (0xC1000000, 0xC0A00000), 0xC020000000000000),
    _paired_fused_case("ps_nmsub", 30, (0xC0800000, 0x40400000), 0xC010000000000000),
    _paired_fused_case("ps_madds0", 14, (0x41000000, 0xC0800000), 0x4020000000000000),
    _paired_fused_case("ps_madds1", 15, (0x3FA00000, 0x40A00000), 0x3FF4000000000000),
    _case("ps_nmadd-lane-invalid-ve", ("fp", "paired", "fused", "snan", "vximz", "ve", "write-result"), _state(_gpr(r5=0), fpr={"f1": 0xFFF0000012345678, "f2": 0, "f3": 0x7FF80000ABCDEF01, "f4": _NINF, "f8": _F2, "f9": _PINF}, fpscr=0x80, memory_words={0: 0, 4: 0}), [_mtspr(5, 913), _ps_x(5, 1, 2, 528), _ps_x(6, 3, 4, 528), _ps_x(10, 8, 9, 528), _ps_a(7, 5, 6, 10, 31), _psq_d(60, 7, 4, 0, 0, 1)], result=0, cr=0, xer=0, expected_memory={0: 0xFFC00000, 4: 0x7FC00000}, expected_fpr={7: 0xFFF8000000000000}, expected_fpscr=0xE1111080),
    _paired_sum_case("ps_sum0", 10, (0xBF000000, 0x40800000), 0xBFE0000000000000, 0x8000),
    _paired_sum_case("ps_sum1", 11, (0x40400000, 0xBF000000), 0x4008000000000000, 0x8000, rc=1),
    _case("ps_sum0-vxisi", ("fp", "paired", "sum", "cross-lane", "invalid"), _state(_gpr(r5=0), fpr={"f1": _PINF, "f2": _NINF, "f3": _F2}, memory_words={0: 0, 4: 0}), [_mtspr(5, 913), _ps_x(5, 1, 1, 528), _ps_x(6, 2, 2, 528), _ps_x(8, 3, 3, 528), _ps_a(7, 5, 6, 8, 10), _psq_d(60, 7, 4, 0, 0, 1)], result=0, cr=0, xer=0, expected_memory={0: 0x7FC00000, 4: 0x40000000}, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0811000),
    _case("ps_sel-signed-zero-nan", ("fp", "paired", "select", "signed-zero", "nan", "lane-bits", "record"), _state(_gpr(r5=0), fpscr=0x10000000, memory_words={0: 0x80000000, 4: 0x7FC00001, 8: 0xFFC12345, 12: 0x80000000, 16: 0x3FC00000, 20: 0x40800000, 24: 0, 28: 0}), [_mtspr(5, 913), _psq_d(56, 1, 4, 0, 0, 1), _psq_d(56, 2, 4, 8, 0, 1), _psq_d(56, 3, 4, 16, 0, 1), _ps_a(7, 1, 2, 3, 23, rc=1), _psq_d(60, 7, 4, 24, 0, 1)], result=0, cr=0x01000000, xer=0, expected_memory={24: 0x3FC00000, 28: 0x80000000}, expected_fpr={7: _F15}, expected_fpscr=0x10000000),
    _case("psq-l-st-quantized", ("fp", "psq", "gqr", "quantized", "pair", "memory"), _state(_gpr(r5=0x01060204), memory_words={0: 0xFE080000, 8: 0xAAAAAAAA}), [_mtspr(5, 913), _psq_d(56, 7, 4, 0, 0, 1), _psq_d(60, 7, 4, 8, 0, 1)], result=0, cr=0, xer=0, expected_memory={8: 0x0010AAAA}, expected_fpr={7: 0xBFF0000000000000}),
    _case("psq-u8-load-s8-store", ("fp", "psq", "gqr", "u8", "s8", "scale", "pair"), _state(_gpr(r5=0x02040106), memory_words={0: 0x080CAAAA, 4: 0xAAAAAAAA}), [_mtspr(5, 913), _psq_d(56, 7, 4, 0, 0, 1), _psq_d(60, 7, 4, 4, 0, 1)], result=0, cr=0, xer=0, expected_memory={4: 0x0406AAAA}, expected_fpr={7: _F2}),
    _case("psq-u16-load-float-store", ("fp", "psq", "gqr", "u16", "float", "scale", "pair"), _state(_gpr(r5=0x01050000), memory_words={0: 0x00020008, 8: 0, 12: 0}), [_mtspr(5, 913), _psq_d(56, 7, 4, 0, 0, 1), _psq_d(60, 7, 4, 8, 0, 1)], result=0, cr=0, xer=0, expected_memory={8: 0x3F800000, 12: 0x40800000}, expected_fpr={7: _F1}),
    _case("psq-st-float-ftz", ("fp", "psq", "gqr", "float", "subnormal", "ftz", "w1"), _state(_gpr(r5=0), memory_words={0: 0}, fpr={"f7": 0xB6A0000000000000}), [_mtspr(5, 913), _psq_d(60, 7, 4, 0, 1, 1)], result=0, cr=0, xer=0, expected_memory={0: 0x80000000}),
    _case("psq-lu-w1", ("fp", "psq", "load", "update", "w1"), _state(_gpr(r5=0), memory_words={4: 0x3FC00000}), [_mtspr(5, 913), _psq_d(57, 7, 4, 4, 1, 1)], result=0, cr=0, xer=0, expected_memory={4: 0x3FC00000}, expected_fpr={7: _F15}),
    _case("psq-stu-w1", ("fp", "psq", "store", "update", "w1"), _state(_gpr(r5=0), memory_words={4: 0}, fpr={"f7": _F15}), [_mtspr(5, 913), _psq_d(61, 7, 4, 4, 1, 1)], result=0, cr=0, xer=0, expected_memory={4: 0x3FC00000}),
    _case("psq-lx-s16", ("fp", "psq", "load", "indexed", "s16"), _state(_gpr(r5=0x01070000, r6=0), memory_words={0: 0xFFFE0008}), [_mtspr(5, 913), _psq_x(True, False, 7, 4, 6, 0, 1)], result=0, cr=0, xer=0, expected_memory={0: 0xFFFE0008}, expected_fpr={7: 0xBFF0000000000000}),
    _case("psq-lux-s8", ("fp", "psq", "load", "indexed", "update", "s8"), _state(_gpr(r5=0x01060000, r6=4), memory_words={4: 0xFE080000}), [_mtspr(5, 913), _psq_x(True, True, 7, 4, 6, 0, 1)], result=0, cr=0, xer=0, expected_memory={4: 0xFE080000}, expected_fpr={7: 0xBFF0000000000000}),
    _case("psq-stx-u16", ("fp", "psq", "store", "indexed", "u16", "w1"), _state(_gpr(r5=0x00000105, r6=4), memory_words={4: 0xAAAAAAAA}, fpr={"f7": _F2}), [_mtspr(5, 913), _psq_x(False, False, 7, 4, 6, 1, 1)], result=0, cr=0, xer=0, expected_memory={4: 0x0004AAAA}),
    _case("psq-stux-s16", ("fp", "psq", "store", "indexed", "update", "s16", "w1"), _state(_gpr(r5=0x00000107, r6=4), memory_words={4: 0xAAAAAAAA}, fpr={"f7": 0xC000000000000000}), [_mtspr(5, 913), _psq_x(False, True, 7, 4, 6, 1, 1)], result=0, cr=0, xer=0, expected_memory={4: 0xFFFCAAAA}),
    _case("lfs", ("fp", "memory"), _state(memory_words={0: 0x3FC00000}), [dform(48, 7, 4, 0)], result=0, cr=0, xer=0, expected_memory={0: 0x3FC00000}, expected_fpr={7: _F15}),
    _case("lfsu", ("fp", "memory", "update"), _state(memory_words={4: 0x3FC00000}), [dform(49, 7, 4, 4)], result=0, cr=0, xer=0, expected_memory={4: 0x3FC00000}, expected_fpr={7: _F15}),
    _case("lfd", ("fp", "memory"), _state(memory_words={0: 0x3FF80000, 4: 0}), [dform(50, 7, 4, 0)], result=0, cr=0, xer=0, expected_memory={0: 0x3FF80000, 4: 0}, expected_fpr={7: _F15}),
    _case("lfdu", ("fp", "memory", "update"), _state(memory_words={8: 0x3FF80000, 12: 0}), [dform(51, 7, 4, 8)], result=0, cr=0, xer=0, expected_memory={8: 0x3FF80000, 12: 0}, expected_fpr={7: _F15}),
    _case("stfs", ("fp", "memory"), _state(memory_words={0: 0}, fpr={"f5": _F15}), [dform(52, 5, 4, 0)], result=0, cr=0, xer=0, expected_memory={0: 0x3FC00000}),
    _case("stfsu", ("fp", "memory", "update"), _state(memory_words={4: 0}, fpr={"f5": _F15}), [dform(53, 5, 4, 4)], result=0, cr=0, xer=0, expected_memory={4: 0x3FC00000}),
    _case("stfd", ("fp", "memory"), _state(memory_words={0: 0, 4: 0}, fpr={"f5": _F15}), [dform(54, 5, 4, 0)], result=0, cr=0, xer=0, expected_memory={0: 0x3FF80000, 4: 0}),
    _case("stfdu", ("fp", "memory", "update"), _state(memory_words={8: 0, 12: 0}, fpr={"f5": _F15}), [dform(55, 5, 4, 8)], result=0, cr=0, xer=0, expected_memory={8: 0x3FF80000, 12: 0}),
    _case("lfsx", ("fp", "memory"), _state(_gpr(r6=0), memory_words={0: 0x3FC00000}), [xo(31, 7, 4, 6, 535)], result=0, cr=0, xer=0, expected_memory={0: 0x3FC00000}, expected_fpr={7: _F15}),
    _case("lfsux", ("fp", "memory", "update"), _state(_gpr(r6=0), memory_words={0: 0x3FC00000}), [xo(31, 7, 4, 6, 567)], result=0, cr=0, xer=0, expected_memory={0: 0x3FC00000}, expected_fpr={7: _F15}),
    _case("lfdx", ("fp", "memory"), _state(_gpr(r6=0), memory_words={0: 0x3FF80000, 4: 0}), [xo(31, 7, 4, 6, 599)], result=0, cr=0, xer=0, expected_memory={0: 0x3FF80000, 4: 0}, expected_fpr={7: _F15}),
    _case("lfdux", ("fp", "memory", "update"), _state(_gpr(r6=0), memory_words={0: 0x3FF80000, 4: 0}), [xo(31, 7, 4, 6, 631)], result=0, cr=0, xer=0, expected_memory={0: 0x3FF80000, 4: 0}, expected_fpr={7: _F15}),
    _case("stfsx", ("fp", "memory"), _state(_gpr(r6=0), memory_words={0: 0}, fpr={"f5": _F15}), [xo(31, 5, 4, 6, 663)], result=0, cr=0, xer=0, expected_memory={0: 0x3FC00000}),
    _case("stfsux", ("fp", "memory", "update"), _state(_gpr(r6=0), memory_words={0: 0}, fpr={"f5": _F15}), [xo(31, 5, 4, 6, 695)], result=0, cr=0, xer=0, expected_memory={0: 0x3FC00000}),
    _case("stfdx", ("fp", "memory"), _state(_gpr(r6=0), memory_words={0: 0, 4: 0}, fpr={"f5": _F15}), [xo(31, 5, 4, 6, 727)], result=0, cr=0, xer=0, expected_memory={0: 0x3FF80000, 4: 0}),
    _case("stfdux", ("fp", "memory", "update"), _state(_gpr(r6=0), memory_words={0: 0, 4: 0}, fpr={"f5": _F15}), [xo(31, 5, 4, 6, 759)], result=0, cr=0, xer=0, expected_memory={0: 0x3FF80000, 4: 0}),
    _case("stfiwx", ("fp", "memory"), _state(_gpr(r6=0), memory_words={0: 0}, fpr={"f5": 0x1122334455667788}), [xo(31, 5, 4, 6, 983)], result=0, cr=0, xer=0, expected_memory={0: 0x55667788}),

    _case("fadds", ("fp", "single", "record"), _state(fpr=_FP_INPUTS, fpscr=0x80000000), [_fp_a(59, 7, 1, 2, 0, 42, rc=1)], result=0, cr=0x08000000, xer=0, expected_fpr={7: 0x400C000000000000}, expected_fpscr=0x80004000),
    _case("fsubs", ("fp", "single"), _state(fpr=_FP_INPUTS), [_fp_a(59, 7, 1, 2, 0, 40)], result=0, cr=0, xer=0, expected_fpr={7: 0xBFE0000000000000}, expected_fpscr=0x00008000),
    _case("fmuls", ("fp", "single", "record"), _state(fpr=_FP_INPUTS, fpscr=0x80060000), [_fp_a(59, 7, 1, 0, 3, 50, rc=1)], result=0, cr=0x08000000, xer=0, expected_fpr={7: 0x4018000000000000}, expected_fpscr=0x80004000),
    _case("fmuls-force25", ("fp", "single", "force25"), _state(fpr={"f1": _F1, "f3": 0x3FF0000010000001}), [_fp_a(59, 7, 1, 0, 3, 50)], result=0, cr=0, xer=0, expected_fpr={7: _F1}, expected_fpscr=0x00004000),
    _case("fmuls-force25-subnormal", ("fp", "single", "force25", "subnormal"), _state(fpr={"f1": 0xFF5A52F1A05885AC, "f3": 0x0000000002000001}), [_fp_a(59, 7, 1, 0, 3, 50)], result=0, cr=0, xer=0, expected_fpr={7: 0xBDCA52F1C0000000}, expected_fpscr=0x00008000),
    _case("fdivs", ("fp", "single"), _state(fpr=_FP_INPUTS), [_fp_a(59, 7, 1, 2, 0, 36)], result=0, cr=0, xer=0, expected_fpr={7: 0x3FE8000000000000}, expected_fpscr=0x00004000),
    _case("fadd", ("fp", "double"), _state(fpr=_FP_INPUTS), [_fp_a(63, 7, 1, 2, 0, 42)], result=0, cr=0, xer=0, expected_fpr={7: 0x400C000000000000}, expected_fpscr=0x00004000),
    _case("fsub", ("fp", "double"), _state(fpr=_FP_INPUTS), [_fp_a(63, 7, 1, 2, 0, 40)], result=0, cr=0, xer=0, expected_fpr={7: 0xBFE0000000000000}, expected_fpscr=0x00008000),
    _case("fmul", ("fp", "double"), _state(fpr=_FP_INPUTS), [_fp_a(63, 7, 1, 0, 3, 50)], result=0, cr=0, xer=0, expected_fpr={7: 0x4018000000000000}, expected_fpscr=0x00004000),
    _case("fdiv", ("fp", "double"), _state(fpr=_FP_INPUTS), [_fp_a(63, 7, 1, 2, 0, 36)], result=0, cr=0, xer=0, expected_fpr={7: 0x3FE8000000000000}, expected_fpscr=0x00004000),
    _case("fadd-vxisi", ("fp", "double", "invalid", "record"), _state(fpr={"f1": _PINF, "f2": _NINF}), [_fp_a(63, 7, 1, 2, 0, 42, rc=1)], result=0, cr=0x0A000000, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0811000),
    _case("fadd-vxisi-ve", ("fp", "double", "invalid", "ve", "suppress", "record"), _state(fpr={"f1": _PINF, "f2": _NINF, "f7": _F1}, fpscr=0x80), [_fp_a(63, 7, 1, 2, 0, 42, rc=1)], result=0, cr=0x0E000000, xer=0, expected_fpr={7: _F1}, expected_fpscr=0xE0800080),
    _case("fadds-snan", ("fp", "single", "invalid", "snan"), _state(fpr={"f1": 0xFFF0000012345678, "f2": _F2}), [_fp_a(59, 7, 1, 2, 0, 42)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000000000000}, expected_fpscr=0xA1011000),
    _case("fadds-snan-ve", ("fp", "single", "invalid", "snan", "ve", "suppress"), _state(fpr={"f1": _SNAN, "f2": _F2, "f7": _F1}, fpscr=0x80), [_fp_a(59, 7, 1, 2, 0, 42)], result=0, cr=0, xer=0, expected_fpr={7: _F1}, expected_fpscr=0xE1000080),
    _case("fadd-qnan", ("fp", "double", "qnan"), _state(fpr={"f1": _QNAN, "f2": _F2}), [_fp_a(63, 7, 1, 2, 0, 42)], result=0, cr=0, xer=0, expected_fpr={7: _QNAN}, expected_fpscr=0x00011000),
    _case("fsub-vxisi", ("fp", "double", "invalid"), _state(fpr={"f1": _PINF, "f2": _PINF}), [_fp_a(63, 7, 1, 2, 0, 40)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0811000),
    _case("fmul-vximz", ("fp", "double", "invalid"), _state(fpr={"f1": 0, "f3": _PINF}), [_fp_a(63, 7, 1, 0, 3, 50)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0111000),
    _case("fmuls-vximz", ("fp", "single", "invalid"), _state(fpr={"f1": 0, "f3": _PINF}), [_fp_a(59, 7, 1, 0, 3, 50)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0111000),
    _case("fdiv-vxzdz", ("fp", "double", "invalid"), _state(fpr={"f1": 0, "f2": _NZERO}), [_fp_a(63, 7, 1, 2, 0, 36)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0211000),
    _case("fdiv-vxidi", ("fp", "double", "invalid"), _state(fpr={"f1": _PINF, "f2": _NINF}), [_fp_a(63, 7, 1, 2, 0, 36)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0411000),
    _case("fdiv-zx", ("fp", "double", "divide-zero"), _state(fpr={"f1": _F2, "f2": _NZERO}), [_fp_a(63, 7, 1, 2, 0, 36)], result=0, cr=0, xer=0, expected_fpr={7: _NINF}, expected_fpscr=0x84009000),
    _case("fdiv-zx-ze", ("fp", "double", "divide-zero", "ze", "suppress"), _state(fpr={"f1": _F2, "f2": _NZERO, "f7": _F1}, fpscr=0x10), [_fp_a(63, 7, 1, 2, 0, 36)], result=0, cr=0, xer=0, expected_fpr={7: _F1}, expected_fpscr=0xC4000010),
    _case("fdivs-zx", ("fp", "single", "divide-zero"), _state(fpr={"f1": _F2, "f2": _NZERO}), [_fp_a(59, 7, 1, 2, 0, 36)], result=0, cr=0, xer=0, expected_fpr={7: _NINF}, expected_fpscr=0x84009000),
    _case("fdivs-zx-ze", ("fp", "single", "divide-zero", "ze", "suppress"), _state(fpr={"f1": _F2, "f2": _NZERO, "f7": _F1}, fpscr=0x10), [_fp_a(59, 7, 1, 2, 0, 36)], result=0, cr=0, xer=0, expected_fpr={7: _F1}, expected_fpscr=0xC4000010),
    _case("fctiw-rne-away", ("fp", "convert", "rne", "inexact", "record"), _state(fpr={"f2": _F15}), [_fp_x(7, 0, 2, 14, rc=1)], result=0, cr=0x08000000, xer=0, expected_fpr={7: 0xFFF8000000000002}, expected_fpscr=0x82060000),
    _case("fctiw-rne-even", ("fp", "convert", "rne", "inexact"), _state(fpr={"f2": 0x4004000000000000}), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000000000002}, expected_fpscr=0x82020000),
    _case("fctiw-rtz", ("fp", "convert", "rtz", "inexact"), _state(fpr={"f2": 0xBFFE666666666666}, fpscr=1), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF80000FFFFFFFF}, expected_fpscr=0x82020001),
    _case("fctiw-rtp", ("fp", "convert", "rtp", "inexact"), _state(fpr={"f2": 0x3FF199999999999A}, fpscr=2), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000000000002}, expected_fpscr=0x82060002),
    _case("fctiw-rtn", ("fp", "convert", "rtn", "inexact"), _state(fpr={"f2": 0xBFF199999999999A}, fpscr=3), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF80000FFFFFFFE}, expected_fpscr=0x82060003),
    _case("fctiwz-negative-zero-result", ("fp", "convert", "rtz", "negative-zero", "inexact"), _state(fpr={"f2": 0xBFD999999999999A}, fpscr=3), [_fp_x(7, 0, 2, 15)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000100000000}, expected_fpscr=0x82020003),
    _case("fctiw-exact-clears-fifr", ("fp", "convert", "exact"), _state(fpr={"f2": 0x4008000000000000}, fpscr=0x00060000), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000000000003}, expected_fpscr=0),
    _case("fctiw-negative-zero", ("fp", "convert", "exact", "negative-zero"), _state(fpr={"f2": _NZERO}), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000100000000}, expected_fpscr=0),
    _case("fctiw-qnan", ("fp", "convert", "qnan", "invalid"), _state(fpr={"f2": _QNAN}), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000080000000}, expected_fpscr=0xA0000100),
    _case("fctiw-snan", ("fp", "convert", "snan", "invalid"), _state(fpr={"f2": _SNAN}), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000080000000}, expected_fpscr=0xA1000100),
    _case("fctiw-positive-overflow", ("fp", "convert", "overflow", "invalid"), _state(fpr={"f2": 0x41E0000000000000}), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF800007FFFFFFF}, expected_fpscr=0xA0000100),
    _case("fctiw-negative-overflow", ("fp", "convert", "overflow", "invalid"), _state(fpr={"f2": 0xC1E0000000200000}), [_fp_x(7, 0, 2, 14)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000080000000}, expected_fpscr=0xA0000100),
    _case("fctiw-ve", ("fp", "convert", "invalid", "ve", "suppress", "record"), _state(fpr={"f2": _QNAN, "f7": _F1}, fpscr=0x80), [_fp_x(7, 0, 2, 14, rc=1)], result=0, cr=0x0E000000, xer=0, expected_fpr={7: _F1}, expected_fpscr=0xE0000180),
    _case("fctiw-xe", ("fp", "convert", "inexact", "xe", "record"), _state(fpr={"f2": _F15}, fpscr=8), [_fp_x(7, 0, 2, 14, rc=1)], result=0, cr=0x0C000000, xer=0, expected_fpr={7: 0xFFF8000000000002}, expected_fpscr=0xC2060008),
    _case("fmadds", ("fp", "fused", "single", "record"), _state(fpr=_FP_INPUTS), [_fp_a(59, 7, 1, 2, 3, 58, rc=1)], result=0, cr=0, xer=0, expected_fpr={7: 0x4020000000000000}, expected_fpscr=0x00004000),
    _case("fmadds-force25", ("fp", "fused", "single", "force25", "inexact"), _state(fpr={"f1": _F1, "f2": 0, "f3": 0x3FF0000010000001}), [_fp_a(59, 7, 1, 2, 3, 58)], result=0, cr=0, xer=0, expected_fpr={7: _F1}, expected_fpscr=0x00024000),
    _case("fmadds-single-rounding", ("fp", "fused", "single", "double-rounding", "inexact"), _state(fpr={"f1": 0x4049000000000000, "f2": 0x3B638E5400000000, "f3": 0xBF91198700000000}), [_fp_a(59, 7, 1, 2, 3, 58)], result=0, cr=0, xer=0, expected_fpr={7: 0xBFEAB7E2E0000000}, expected_fpscr=0x00028000),
    _case("fmadds-nan-order", ("fp", "fused", "single", "snan", "operand-order"), _state(fpr={"f1": 0x7FF1000000000001, "f2": 0x7FF2000000000001, "f3": 0x7FF3000000000001}), [_fp_a(59, 7, 1, 2, 3, 58)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF9000000000000}, expected_fpscr=0xA1031000),
    _case("fmadds-vximz", ("fp", "fused", "single", "invalid"), _state(fpr={"f1": 0, "f2": _F1, "f3": _PINF}), [_fp_a(59, 7, 1, 2, 3, 58)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0131000),
    _case("fmadds-vximz-ve", ("fp", "fused", "single", "invalid", "ve", "suppress", "record"), _state(fpr={"f1": 0, "f2": _F1, "f3": _PINF, "f7": _F2}, fpscr=0x80), [_fp_a(59, 7, 1, 2, 3, 58, rc=1)], result=0, cr=0x0E000000, xer=0, expected_fpr={7: _F2}, expected_fpscr=0xE0100080),
    _case("fmadds-vxisi", ("fp", "fused", "single", "invalid"), _state(fpr={"f1": _PINF, "f2": _NINF, "f3": _F1}), [_fp_a(59, 7, 1, 2, 3, 58)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0831000),
    _case("fmsubs", ("fp", "fused", "single", "subtract", "preserve-fifr"), _state(fpr=_FP_INPUTS, fpscr=0x00060000), [_fp_a(59, 7, 1, 2, 3, 56)], result=0, cr=0, xer=0, expected_fpr={7: _F4}, expected_fpscr=0x00064000),
    _case("fnmsubs", ("fp", "fused", "single", "subtract", "negate", "preserve-fifr"), _state(fpr=_FP_INPUTS, fpscr=0x00060000), [_fp_a(59, 7, 1, 2, 3, 60)], result=0, cr=0, xer=0, expected_fpr={7: 0xC010000000000000}, expected_fpscr=0x00068000),
    _case("fmsubs-positive-zero", ("fp", "fused", "single", "subtract", "zero"), _state(fpr={"f1": _F1, "f2": _F1, "f3": _F1}), [_fp_a(59, 7, 1, 2, 3, 56)], result=0, cr=0, xer=0, expected_fpr={7: 0}, expected_fpscr=0x00002000),
    _case("fnmsubs-negative-zero", ("fp", "fused", "single", "subtract", "negate", "zero"), _state(fpr={"f1": _F1, "f2": _F1, "f3": _F1}), [_fp_a(59, 7, 1, 2, 3, 60)], result=0, cr=0, xer=0, expected_fpr={7: _NZERO}, expected_fpscr=0x00012000),
    _case("fmsubs-single-rounding", ("fp", "fused", "single", "subtract", "double-rounding"), _state(fpr={"f1": 0x4049000000000000, "f2": 0xBB638E5400000000, "f3": 0xBF91198700000000}), [_fp_a(59, 7, 1, 2, 3, 56)], result=0, cr=0, xer=0, expected_fpr={7: 0xBFEAB7E2E0000000}, expected_fpscr=0x00008000),
    _case("fnmsubs-single-rounding", ("fp", "fused", "single", "subtract", "negate", "double-rounding"), _state(fpr={"f1": 0x4049000000000000, "f2": 0xBB638E5400000000, "f3": 0xBF91198700000000}), [_fp_a(59, 7, 1, 2, 3, 60)], result=0, cr=0, xer=0, expected_fpr={7: 0x3FEAB7E2E0000000}, expected_fpscr=0x00004000),
    _case("fnmsubs-nan-not-negated", ("fp", "fused", "single", "subtract", "negate", "qnan"), _state(fpr={"f1": 0xFFF8000012345678, "f2": _F2, "f3": _F4}), [_fp_a(59, 7, 1, 2, 3, 60)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000000000000}, expected_fpscr=0x00011000),
    _case("fmsubs-vxisi", ("fp", "fused", "single", "subtract", "invalid"), _state(fpr={"f1": _PINF, "f2": _PINF, "f3": _F1}), [_fp_a(59, 7, 1, 2, 3, 56)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0811000),
    _case("fnmsubs-vximz", ("fp", "fused", "single", "subtract", "negate", "invalid"), _state(fpr={"f1": 0, "f2": _F1, "f3": _PINF}), [_fp_a(59, 7, 1, 2, 3, 60)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0111000),
    _case("fnmsubs-vxisi-ve", ("fp", "fused", "single", "subtract", "negate", "invalid", "ve", "suppress", "record"), _state(fpr={"f1": _PINF, "f2": _PINF, "f3": _F1, "f7": _F2}, fpscr=0x80), [_fp_a(59, 7, 1, 2, 3, 60, rc=1)], result=0, cr=0x0E000000, xer=0, expected_fpr={7: _F2}, expected_fpscr=0xE0800080),
    _case("fnmadds", ("fp", "fused", "single", "add", "negate", "preserve-fifr"), _state(fpr=_FP_INPUTS, fpscr=0x00060000), [_fp_a(59, 7, 1, 2, 3, 62)], result=0, cr=0, xer=0, expected_fpr={7: 0xC020000000000000}, expected_fpscr=0x00068000),
    _case("fnmadds-nan-not-negated", ("fp", "fused", "single", "add", "negate", "qnan"), _state(fpr={"f1": 0xFFF8000012345678, "f2": _F2, "f3": _F4}), [_fp_a(59, 7, 1, 2, 3, 62)], result=0, cr=0, xer=0, expected_fpr={7: 0xFFF8000000000000}, expected_fpscr=0x00011000),
    _case("fmadd", ("fp", "fused", "double", "add", "preserve-fifr"), _state(fpr=_FP_INPUTS, fpscr=0x00060000), [_fp_a(63, 7, 1, 2, 3, 58)], result=0, cr=0, xer=0, expected_fpr={7: 0x4020000000000000}, expected_fpscr=0x00064000),
    _case("fmadd-single-rounding", ("fp", "fused", "double", "add", "single-rounding"), _state(fpr={"f1": 0x3FF0000002000000, "f2": 0xBFF0000000000000, "f3": 0x3FEFFFFFFC000000}), [_fp_a(63, 7, 1, 2, 3, 58)], result=0, cr=0, xer=0, expected_fpr={7: 0xBC90000000000000}, expected_fpscr=0x00008000),
    _case("fmadd-nan-payload", ("fp", "fused", "double", "add", "qnan", "payload"), _state(fpr={"f1": 0x7FF8000012345678, "f2": _F2, "f3": _F4}), [_fp_a(63, 7, 1, 2, 3, 58)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000012345678}, expected_fpscr=0x00011000),
    _case("fmsub", ("fp", "fused", "double", "subtract", "preserve-fifr"), _state(fpr=_FP_INPUTS, fpscr=0x00060000), [_fp_a(63, 7, 1, 2, 3, 56)], result=0, cr=0, xer=0, expected_fpr={7: _F4}, expected_fpscr=0x00064000),
    _case("fnmadd", ("fp", "fused", "double", "add", "negate", "preserve-fifr"), _state(fpr=_FP_INPUTS, fpscr=0x00060000), [_fp_a(63, 7, 1, 2, 3, 62)], result=0, cr=0, xer=0, expected_fpr={7: 0xC020000000000000}, expected_fpscr=0x00068000),
    _case("fnmadd-vxisi-ve", ("fp", "fused", "double", "add", "negate", "invalid", "ve", "suppress", "record"), _state(fpr={"f1": _PINF, "f2": _NINF, "f3": _F1, "f7": _F2}, fpscr=0x80), [_fp_a(63, 7, 1, 2, 3, 62, rc=1)], result=0, cr=0x0E000000, xer=0, expected_fpr={7: _F2}, expected_fpscr=0xE0800080),
    _case("fnmsub", ("fp", "fused", "double", "subtract", "negate", "preserve-fifr"), _state(fpr=_FP_INPUTS, fpscr=0x00060000), [_fp_a(63, 7, 1, 2, 3, 60)], result=0, cr=0, xer=0, expected_fpr={7: 0xC010000000000000}, expected_fpscr=0x00068000),
    _case("fnmsub-vximz", ("fp", "fused", "double", "subtract", "negate", "invalid"), _state(fpr={"f1": 0, "f2": _F1, "f3": _PINF}), [_fp_a(63, 7, 1, 2, 3, 60)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000000000000}, expected_fpscr=0xA0111000),
    _case("fsel", ("fp", "double"), _state(fpr=_FP_INPUTS), [_fp_a(63, 7, 1, 2, 3, 46)], result=0, cr=0, xer=0, expected_fpr={7: _F4}, expected_fpscr=0),
    _case("fcmpu", ("fp", "compare"), _state(fpr=_FP_INPUTS), [_fp_x(0, 1, 2, 0)], result=0, cr=0x80000000, xer=0, expected_fpscr=0x00008000),
    _case("fcmpu-snan", ("fp", "compare", "snan"), _state(fpr={"f1": _SNAN, "f2": _F2}), [_fp_cmp(3, 1, 2, 0)], result=0, cr=0x00010000, xer=0, expected_fpscr=0xA1001000),
    _case("fcmpo", ("fp", "compare", "ordered"), _state(fpr=_FP_INPUTS), [_fp_cmp(3, 1, 2, 32)], result=0, cr=0x00080000, xer=0, expected_fpscr=0x00008000),
    _case("fcmpo-qnan", ("fp", "compare", "ordered", "qnan"), _state(fpr={"f1": _QNAN, "f2": _F2}), [_fp_cmp(3, 1, 2, 32)], result=0, cr=0x00010000, xer=0, expected_fpscr=0xA0081000),
    _case("fcmpo-qnan-ve", ("fp", "compare", "ordered", "qnan", "ve"), _state(fpr={"f1": _QNAN, "f2": _F2}, fpscr=0x80), [_fp_cmp(3, 1, 2, 32)], result=0, cr=0x00010000, xer=0, expected_fpscr=0xE0081080),
    _case("fcmpo-snan", ("fp", "compare", "ordered", "snan"), _state(fpr={"f1": _SNAN, "f2": _F2}), [_fp_cmp(3, 1, 2, 32)], result=0, cr=0x00010000, xer=0, expected_fpscr=0xA1081000),
    _case("fcmpo-snan-ve", ("fp", "compare", "ordered", "snan", "ve"), _state(fpr={"f1": _SNAN, "f2": _F2}, fpscr=0x80), [_fp_cmp(3, 1, 2, 32)], result=0, cr=0x00010000, xer=0, expected_fpscr=0xE1001080),
    _case("frsp", ("fp", "single"), _state(fpr={"f2": _F15}), [_fp_x(7, 0, 2, 12)], result=0, cr=0, xer=0, expected_fpr={7: _F15}, expected_fpscr=0x00004000),
    _case("fneg", ("fp", "move"), _state(fpr={"f2": _F15}), [_fp_x(7, 0, 2, 40)], result=0, cr=0, xer=0, expected_fpr={7: 0xBFF8000000000000}),
    _case("fmr", ("fp", "move"), _state(fpr={"f2": 0x7FF8000012345678}), [_fp_x(7, 0, 2, 72)], result=0, cr=0, xer=0, expected_fpr={7: 0x7FF8000012345678}),
    _case("fnabs", ("fp", "move"), _state(fpr={"f2": _F15}), [_fp_x(7, 0, 2, 136)], result=0, cr=0, xer=0, expected_fpr={7: 0xBFF8000000000000}),
    _case("fabs", ("fp", "move"), _state(fpr={"f2": 0xBFF8000000000000}), [_fp_x(7, 0, 2, 264)], result=0, cr=0, xer=0, expected_fpr={7: _F15}),
)


def load_fixtures(path: Path | None = None) -> tuple[FixtureCase, ...]:
    if path is None:
        return FIXTURES
    cases: list[FixtureCase] = []
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        payload = json.loads(line)
        words = tuple(int(part, 16) for part in payload["code"]["hex"].split())
        expected = payload["expected"]
        observe = payload.get("observe", {})
        memory_exp = {
            parse_int(key) if parse_int(key) < SANDBOX_BASE else parse_int(key) - SANDBOX_BASE: parse_int(value)
            for key, value in (observe.get("memory") or {}).items()
        }
        fpr_exp = {
            int(name[1:]): parse_int(value)
            for name, value in (expected.get("fpr") or {}).items()
        }
        cases.append(
            FixtureCase(
                id=payload["id"],
                tags=tuple(payload.get("tags", [])),
                initial=payload["initial"],
                code_words=words,
                result_reg=int(observe.get("result_reg", 7)),
                expected_result=parse_int(expected["result"]),
                expected_cr=parse_int(expected["cr"]),
                expected_xer=parse_int(expected["xer"]),
                expected_memory=memory_exp,
                expected_fpr=fpr_exp,
                expected_fpscr=(
                    parse_int(expected["fpscr"])
                    if expected.get("fpscr") is not None else None
                ),
                max_instructions=int(payload.get("bounds", {}).get("max_instructions", 32)),
                allow_paths=int(payload.get("bounds", {}).get("allow_paths", 1)),
            )
        )
    return tuple(cases)


def export_jsonl(path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        "".join(json.dumps(case.to_json(), sort_keys=True) + "\n" for case in FIXTURES),
        encoding="utf-8",
    )

"""Harvest scalar_fp_v2 corpus rows from Broadway/Dolphin fixture sources.

Maps ``tools/ppc_equivalence/fixtures/corpus.py`` (exported as
``fixtures/broadway.jsonl``) into independent validation rows. Expected
result/FPSCR/CR bits come from the fixture oracle — not from regenerating
via the exact kernel under test at harvest time.

NI=1 rows without Broadway NI fixtures are filled via exact-kernel replay and
marked ``exact_kernel_v2`` honestly.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any

from tools.ppc_equivalence.fixtures.corpus import FIXTURES, FixtureCase
from tools.ppc_equivalence.fixtures.encode import parse_int
from tools.ppc_equivalence.fp_exact import exact_scalar_arith
from tools.ppc_equivalence.fp_exact_outcome import fp_outcome_from_scalar_outcome
from tools.ppc_equivalence.fp_fpscr import FPSCR_NI
from tools.ppc_equivalence.fp_oracle import mask32, mask64
from tools.ppc_equivalence.fp_traps import MSR_FE0, MSR_FE1, plan_fp_trap_delivery
from tools.ppc_equivalence.fp_exact_compare import exact_fcmpo, exact_fcmpu
from tools.ppc_equivalence.fp_exact_convert import exact_frsp
from tools.ppc_equivalence.fp_capabilities import set_scalar_fp_exact_v2_module_flag
from tools.ppc_equivalence.scalar_fp_v2_corpus import (
    CORPUS_DIR,
    CORPUS_FILES,
    check_corpora,
    replay_row,
    validate_corpus_row,
)

PROVENANCE_BROADWAY = "fixtures-broadway-jsonl"
PROVENANCE_EXACT = "exact_kernel_v2"

_SCALAR_ARITH = frozenset(
    {"fadd", "fadds", "fsub", "fsubs", "fmul", "fmuls", "fdiv", "fdivs"}
)
_FUSED = frozenset(
    {"fmadd", "fmadds", "fmsub", "fmsubs", "fnmadd", "fnmadds", "fnmsub", "fnmsubs"}
)
_COMPARE = frozenset({"fcmpu", "fcmpo"})
_CONVERT = frozenset({"frsp"})

# Broadway compare fixtures encode BF in the instruction; map id → CR field.
_COMPARE_BF: dict[str, int] = {
    "fcmpu": 0,
    "fcmpo": 3,
    "fcmpu-snan": 3,
    "fcmpo-qnan": 3,
    "fcmpo-snan": 3,
    "fcmpo-qnan-ve": 3,
    "fcmpo-snan-ve": 3,
}

_CORPUS_HEADERS: dict[str, dict[str, Any]] = {
    "scalar_rn.jsonl": {
        "type": "scalar_fp_v2_corpus",
        "schema_version": 1,
        "corpus_id": "scalar_rn",
        "capability": "fp-scalar-arithmetic",
        "notes": "Non-fused scalar result bits; Broadway RNE harvest + exact_kernel_v2 RTZ/RIP/RIM/NI supplements",
    },
    "fpscr.jsonl": {
        "type": "scalar_fp_v2_corpus",
        "schema_version": 1,
        "corpus_id": "fpscr",
        "capability": "fp-scalar-arithmetic",
        "notes": "FPSCR transition vectors; Broadway RNE + exact_kernel_v2 non-RNE supplements",
    },
    "ni.jsonl": {
        "type": "scalar_fp_v2_corpus",
        "schema_version": 1,
        "corpus_id": "ni",
        "capability": "fp-scalar-arithmetic",
        "notes": "NI=0 Broadway harvest + NI=1 exact_kernel_v2 (no NI Dolphin fixtures yet)",
    },
    "compare_convert_control.jsonl": {
        "type": "scalar_fp_v2_corpus",
        "schema_version": 1,
        "corpus_id": "compare_convert_control",
        "capability": "fp-compare",
        "notes": "Compare / convert; Broadway harvest + exact_kernel_v2 NI=1 / non-RNE frsp supplements",
    },
    "traps_fe.jsonl": {
        "type": "scalar_fp_v2_corpus",
        "schema_version": 1,
        "corpus_id": "traps_fe",
        "capability": "fp-traps",
        "notes": "FE0/FE1 trap delivery derived from Broadway VE/ZE/OE fixtures",
    },
    "fused_residual.jsonl": {
        "type": "scalar_fp_v2_corpus",
        "schema_version": 1,
        "corpus_id": "fused_residual",
        "capability": "fp-fused-arithmetic",
        "notes": "Fused midpoint/residue cases from fixtures/broadway.jsonl",
    },
}

_MIN_ROWS = 20
_MIN_NON_RNE_PER_MODE = 5

# Broadway FPSCR[0:1] RN encodings (exact-v2 kernel; no live Dolphin rows yet).
_NON_RNE_MODES: tuple[tuple[int, str, str], ...] = (
    (1, "toward-zero", "rtz"),
    (2, "toward-plus-infinity", "rip"),
    (3, "toward-minus-infinity", "rim"),
)

# (opcode, a, b, c, label) — curated for directed overflow / RN-sensitive ties.
_NON_RNE_SCALAR_VECTORS: dict[str, list[tuple[str, int, int, int, str]]] = {
    "toward-zero": [
        ("fadd", 0x7FEFFFFFFFFFFFFF, 0x7FE0000000000000, 0, "overflow-max-finite"),
        ("fadd", 0x3FF0000000000000, 0x3FEFFFFFFFFFFFFF, 0, "add-truncate"),
        ("fsub", 0x3FF0000000000000, 0x3FEFFFFFFFFFFFFF, 0, "sub-truncate"),
        ("fdiv", 0x3FF8000000000000, 0x4000000000000000, 0, "div"),
        ("fadds", 0x3FF0000000000000, 0x3FEFFFFF00000000, 0, "adds-truncate"),
    ],
    "toward-plus-infinity": [
        ("fsub", 0x0010000000000000, 0x0008000000000000, 0, "subnormal-underflow"),
        ("fadd", 0x3FEFFFFFFFFFFFFF, 0x0000000000000001, 0, "tiny-add"),
        ("fdiv", 0x3FF0000000000000, 0x4000000000000000, 0, "div"),
        ("fadd", 0x8000000000000000, 0x0000000000000000, 0, "negzero-plus-zero"),
        ("fadds", 0x3FEFFFFF00000000, 0x0000000100000000, 0, "adds"),
    ],
    "toward-minus-infinity": [
        ("fsub", 0xBFEFFFFFFFFFFFFF, 0x0000000000000001, 0, "sub"),
        ("fadd", 0xC000000000000000, 0x3FF0000000000000, 0, "add"),
        ("fdiv", 0xBFF0000000000000, 0x4000000000000000, 0, "div"),
        ("fadd", 0x8000000000000000, 0x8000000000000000, 0, "negzero-add"),
        ("fadds", 0xBFEFFFFF00000000, 0x0000000100000000, 0, "adds"),
    ],
}

# FPSCR delta rows for non-RNE (subset with sticky OX/UX/XX transitions).
_NON_RNE_FPSCR_VECTORS: dict[str, list[tuple[str, int, int, int, str]]] = {
    "toward-zero": [
        ("fadd", 0x7FEFFFFFFFFFFFFF, 0x7FE0000000000000, 0, "overflow-max-finite"),
        ("fadd", 0x3FF0000000000000, 0x3FEFFFFFFFFFFFFF, 0, "add-truncate"),
        ("fsub", 0x3FF0000000000000, 0x3FEFFFFFFFFFFFFF, 0, "sub-truncate"),
        ("fadds", 0x3FF0000000000000, 0x3FEFFFFF00000000, 0, "adds-truncate"),
        ("fdiv", 0x3FF8000000000000, 0x4000000000000000, 0, "div"),
    ],
    "toward-plus-infinity": [
        ("fsub", 0x0010000000000000, 0x0008000000000000, 0, "subnormal-underflow"),
        ("fadd", 0x3FEFFFFFFFFFFFFF, 0x0000000000000001, 0, "tiny-add"),
        ("fadds", 0x3FEFFFFF00000000, 0x0000000100000000, 0, "adds"),
        ("fdiv", 0x3FF0000000000000, 0x4000000000000000, 0, "div"),
        ("fadd", 0x8000000000000000, 0x0000000000000000, 0, "negzero-plus-zero"),
    ],
    "toward-minus-infinity": [
        ("fsub", 0xBFEFFFFFFFFFFFFF, 0x0000000000000001, 0, "sub"),
        ("fadd", 0xC000000000000000, 0x3FF0000000000000, 0, "add"),
        ("fadds", 0xBFEFFFFF00000000, 0x0000000100000000, 0, "adds"),
        ("fdiv", 0xBFF0000000000000, 0x4000000000000000, 0, "div"),
        ("fadd", 0x8000000000000000, 0x8000000000000000, 0, "negzero-add"),
    ],
}

_NI1_SCALAR_VECTORS: list[tuple[str, int, int, int, str]] = [
    ("fadd", 0x0000000000000001, 0x0000000000000001, 0, "subnormal-add"),
    ("fmuls", 0x3810000000000000, 0x3810000000000000, 0, "subnormal-mul"),
    ("fadd", 0x8000000000000000, 0x0000000000000000, 0, "signed-zero"),
    ("fadds", 0x0000000000000001, 0x0000000000000001, 0, "subnormal-adds"),
]

_NI1_COMPARE_CONVERT_VECTORS: list[tuple[str, dict[str, str], str]] = [
    ("frsp", {"a": "0x0000000000000001"}, "ni1-frsp-subnormal-flush"),
    ("frsp", {"a": "0x3810000000000000"}, "ni1-frsp-smallest-normal-flush"),
    ("frsp", {"a": "0x0000000002000001"}, "ni1-frsp-denorm-flush"),
    (
        "fcmpu",
        {"a": "0x0000000000000001", "b": "0x0000000000000000"},
        "ni1-fcmpu-subnorm-vs-zero",
    ),
    (
        "frsp",
        {"a": "0x3ff0000000400000"},
        "ni1-frsp-rn-tie",
    ),
]

_NON_RNE_FRSP_VECTORS: list[tuple[int, str, str]] = [
    (0x3FF0000000400000, "toward-zero", "rtz-frsp-tie"),
    (0x3FF0000000400000, "toward-plus-infinity", "rip-frsp-tie"),
    (0x3FF0000000400000, "toward-minus-infinity", "rim-frsp-tie"),
    (0x400921FB54442D18, "toward-zero", "rtz-frsp-pi"),
    (0x400921FB54442D18, "toward-plus-infinity", "rip-frsp-pi"),
]


def _hex64(value: int) -> str:
    return f"0x{value & 0xFFFFFFFFFFFFFFFF:016x}"


def _hex32(value: int) -> str:
    return f"0x{value & 0xFFFFFFFF:08x}"


def _parse_fpr(initial: dict[str, Any]) -> dict[int, int]:
    out: dict[int, int] = {}
    for name, text in (initial.get("fpr") or {}).items():
        if name.startswith("f"):
            out[int(name[1:])] = parse_int(text)
    return out


def _initial_fpscr(initial: dict[str, Any]) -> int:
    text = initial.get("fpscr")
    if text is None:
        return 0
    return parse_int(text)


def _fixture_opcode(case_id: str) -> str | None:
    for op in sorted(_SCALAR_ARITH | _FUSED | _COMPARE | _CONVERT, key=len, reverse=True):
        if case_id == op or case_id.startswith(f"{op}-"):
            return op
    return None


def _operands(case: FixtureCase, opcode: str) -> dict[str, str]:
    fpr = _parse_fpr(case.initial)
    if opcode in {"fmul", "fmuls"}:
        return {
            "a": _hex64(fpr.get(1, 0)),
            "c": _hex64(fpr.get(3, fpr.get(2, 0))),
            "b": _hex64(fpr.get(2, 0)),
        }
    if opcode in _FUSED:
        return {
            "a": _hex64(fpr.get(1, 0)),
            "c": _hex64(fpr.get(3, 0)),
            "b": _hex64(fpr.get(2, 0)),
        }
    if opcode == "frsp":
        return {"a": _hex64(fpr.get(2, 0))}
    return {
        "a": _hex64(fpr.get(1, 0)),
        "b": _hex64(fpr.get(2, 0)),
    }


def _fpcr_nibble(cr: int, bf: int) -> int:
    shift = 28 - 4 * bf
    return (cr >> shift) & 0xF


def _row_base(case: FixtureCase, opcode: str) -> dict[str, Any]:
    row: dict[str, Any] = {
        "id": f"bb-{case.id}",
        "opcode": opcode,
        "operands": _operands(case, opcode),
        "provenance": PROVENANCE_BROADWAY,
        "source_fixture": case.id,
    }
    pre = _initial_fpscr(case.initial)
    if pre:
        row["pre_fpscr"] = _hex32(pre)
    if int(pre) & FPSCR_NI:
        row["ni"] = 1
    return row


def _skip_scalar_mismatch_tags(case: FixtureCase) -> bool:
    skip = {"ve", "suppress", "force25", "concrete-oracle-inconclusive"}
    return bool(skip.intersection(case.tags))


def harvest_scalar_rn(cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    for case in cases:
        opcode = _fixture_opcode(case.id)
        if opcode not in _SCALAR_ARITH:
            continue
        if 7 not in case.expected_fpr:
            continue
        if _skip_scalar_mismatch_tags(case):
            continue
        row = _row_base(case, opcode)
        row["rn"] = "nearest-even"
        row["expected_bits"] = _hex64(case.expected_fpr[7])
        rows.append(row)
    return rows


def harvest_fpscr(cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    for case in cases:
        opcode = _fixture_opcode(case.id)
        if opcode is None:
            continue
        if case.expected_fpscr is None:
            continue
        pre = _initial_fpscr(case.initial)
        post = case.expected_fpscr
        if pre == post:
            continue
        if opcode not in _SCALAR_ARITH | _CONVERT:
            continue
        row = _row_base(case, opcode if opcode != "frsp" else "frsp")
        row["expected_post_fpscr"] = _hex32(post)
        delta = mask32(post ^ pre)
        if delta:
            row["expected_fpscr_mask"] = _hex32(delta)
        rows.append(row)
    return rows


def harvest_compare_convert(cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    for case in cases:
        opcode = _fixture_opcode(case.id)
        if opcode == "frsp" and 7 in case.expected_fpr:
            row = _row_base(case, "frsp")
            row["expected_bits"] = _hex64(case.expected_fpr[7])
            rows.append(row)
        elif opcode in _COMPARE:
            bf = _COMPARE_BF.get(case.id, 0)
            row = _row_base(case, opcode)
            row["expected_fpcr_nibble"] = f"0x{_fpcr_nibble(case.expected_cr, bf):x}"
            rows.append(row)
    return rows


def harvest_fused(cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    for case in cases:
        opcode = _fixture_opcode(case.id)
        if opcode not in _FUSED:
            continue
        if 7 not in case.expected_fpr:
            continue
        if "invalid" in case.tags or "snan" in case.tags or "concrete-oracle-inconclusive" in case.tags:
            continue
        row = _row_base(case, opcode)
        row["expected_bits"] = _hex64(case.expected_fpr[7])
        rows.append(row)
    return rows


def _msr_from_fe(fe: str) -> int:
    msr = 0
    if fe[0] == "1":
        msr |= MSR_FE0
    if fe[1] == "1":
        msr |= MSR_FE1
    return msr


def _trap_row_from_fixture(
    case: FixtureCase,
    *,
    opcode: str,
    msr_fe: str,
    suffix: str,
) -> dict[str, Any] | None:
    pre = _initial_fpscr(case.initial)
    operands = _operands(case, opcode)
    a = parse_int(operands["a"])
    b = parse_int(operands.get("b", "0"))
    c = parse_int(operands.get("c", b))
    outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=pre)
    if outcome is None or not outcome.supported:
        return None
    fp_outcome = fp_outcome_from_scalar_outcome(outcome)
    plan = plan_fp_trap_delivery(
        fp_outcome,
        pre,
        msr=_msr_from_fe(msr_fe),
        domain_traps_enabled=True,
    )
    row = _row_base(case, opcode)
    row["id"] = f"bb-trap-{case.id}-{suffix}"
    row["msr_fe"] = msr_fe
    row["domain_traps_enabled"] = True
    row["expected_writeback"] = bool(plan.writeback)
    row["expected_trap"] = bool(plan.trap)
    row["expected_enabled_exception"] = bool(plan.enabled_exception)
    return row


def harvest_traps(cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    trap_candidates = (
        "fdiv-zx",
        "fdivs-zx",
        "fadd-vxisi",
        "fadds-snan",
        "fsub-vxisi",
        "fmul-vximz",
        "fmuls-vximz",
        "fmadds-vximz",
        "fmadds-vxisi",
        "fnmadd-vxisi-ve",
        "fadd-vxisi-ve",
        "fadds-snan-ve",
    )
    by_id = {case.id: case for case in cases}
    for case_id in trap_candidates:
        case = by_id.get(case_id)
        if case is None:
            continue
        opcode = _fixture_opcode(case.id)
        if opcode is None or opcode not in _SCALAR_ARITH | _FUSED:
            continue
        for msr_fe, suffix in (("10", "fe0-precise"), ("00", "fe00-imprecise")):
            row = _trap_row_from_fixture(case, opcode=opcode, msr_fe=msr_fe, suffix=suffix)
            if row is not None:
                rows.append(row)
    # Deduplicate by id
    seen: set[str] = set()
    unique: list[dict[str, Any]] = []
    for row in rows:
        if row["id"] in seen:
            continue
        seen.add(row["id"])
        unique.append(row)
    return unique


def _exact_ni_rows(seed_cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    """NI=1 rows: operands from Broadway subnormal/edge fixtures, bits from exact kernel."""
    seeds = [
        case
        for case in seed_cases
        if _fixture_opcode(case.id) in _SCALAR_ARITH
        and any(tag in case.tags for tag in ("subnormal", "zero", "invalid", "snan", "qnan"))
    ]
    if len(seeds) < 10:
        seeds = [
            case
            for case in seed_cases
            if _fixture_opcode(case.id) in _SCALAR_ARITH and 7 in case.expected_fpr
        ]
    rows: list[dict[str, Any]] = []
    for case in seeds:
        opcode = _fixture_opcode(case.id)
        if opcode not in _SCALAR_ARITH:
            continue
        for ni in (0, 1):
            if ni == 0:
                continue  # Broadway harvest covers NI=0
            operands = _operands(case, opcode)
            pre = _initial_fpscr(case.initial) | FPSCR_NI
            a = parse_int(operands["a"])
            b = parse_int(operands.get("b", "0"))
            c = parse_int(operands.get("c", b))
            outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=pre)
            if outcome is None or not outcome.supported:
                continue
            row = {
                "id": f"exact-ni1-{case.id}",
                "opcode": opcode,
                "ni": 1,
                "operands": operands,
                "expected_bits": _hex64(outcome.result_bits),
                "provenance": PROVENANCE_EXACT,
                "source_fixture": case.id,
            }
            if pre & ~FPSCR_NI:
                row["pre_fpscr"] = _hex32(pre & ~FPSCR_NI)
            rows.append(row)
            if len(rows) >= _MIN_ROWS:
                break
        if len(rows) >= _MIN_ROWS:
            break
    return rows


def _operands_from_triple(opcode: str, a: int, b: int, c: int) -> dict[str, str]:
    if opcode in {"fmul", "fmuls"}:
        return {"a": _hex64(a), "b": _hex64(b), "c": _hex64(c)}
    return {"a": _hex64(a), "b": _hex64(b)}


def _exact_scalar_row_from_vector(
    *,
    opcode: str,
    a: int,
    b: int,
    c: int,
    fpscr: int,
    row_id: str,
    rn: str | None = None,
    ni: int | None = None,
) -> dict[str, Any] | None:
    outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=fpscr)
    if outcome is None or not outcome.supported:
        return None
    row: dict[str, Any] = {
        "id": row_id,
        "opcode": opcode,
        "operands": _operands_from_triple(opcode, a, b, c),
        "expected_bits": _hex64(outcome.result_bits),
        "provenance": PROVENANCE_EXACT,
    }
    base_fpscr = fpscr & ~FPSCR_NI
    if base_fpscr:
        row["pre_fpscr"] = _hex32(base_fpscr)
    if rn is not None:
        row["rn"] = rn
    if ni is not None:
        row["ni"] = ni
    return row


def harvest_non_rne_scalar() -> list[dict[str, Any]]:
    """RTZ / RIP / RIM scalar result rows via exact kernel (no Dolphin fixtures)."""
    set_scalar_fp_exact_v2_module_flag(True)
    try:
        rows: list[dict[str, Any]] = []
        for rn_bits, rn_name, slug in _NON_RNE_MODES:
            for opcode, a, b, c, label in _NON_RNE_SCALAR_VECTORS[rn_name]:
                row = _exact_scalar_row_from_vector(
                    opcode=opcode,
                    a=a,
                    b=b,
                    c=c,
                    fpscr=rn_bits,
                    row_id=f"exact-{slug}-{label}",
                    rn=rn_name,
                )
                if row is not None:
                    rows.append(row)
        return rows
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def harvest_non_rne_fpscr() -> list[dict[str, Any]]:
    """Non-RNE FPSCR transition rows where post != pre."""
    from tools.ppc_equivalence.fp_fpscr import apply_fpscr_transition

    set_scalar_fp_exact_v2_module_flag(True)
    try:
        rows: list[dict[str, Any]] = []
        for rn_bits, rn_name, slug in _NON_RNE_MODES:
            count = 0
            for opcode, a, b, c, label in _NON_RNE_FPSCR_VECTORS[rn_name]:
                pre = rn_bits
                outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=pre)
                if outcome is None or not outcome.supported:
                    continue
                post = apply_fpscr_transition(pre, opcode, outcome)
                delta = mask32(post ^ pre)
                if delta == 0:
                    continue
                rows.append(
                    {
                        "id": f"exact-fpscr-{slug}-{label}",
                        "opcode": opcode,
                        "operands": _operands_from_triple(opcode, a, b, c),
                        "pre_fpscr": _hex32(pre),
                        "rn": rn_name,
                        "expected_post_fpscr": _hex32(post),
                        "expected_fpscr_mask": _hex32(delta),
                        "provenance": PROVENANCE_EXACT,
                    }
                )
                count += 1
                if count >= _MIN_NON_RNE_PER_MODE:
                    break
        return rows
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def harvest_ni_scalar_extras() -> list[dict[str, Any]]:
    """NI=1 scalar rows: flush-to-zero and signed-zero edge cases."""
    set_scalar_fp_exact_v2_module_flag(True)
    try:
        rows: list[dict[str, Any]] = []
        for opcode, a, b, c, label in _NI1_SCALAR_VECTORS:
            row = _exact_scalar_row_from_vector(
                opcode=opcode,
                a=a,
                b=b,
                c=c,
                fpscr=FPSCR_NI,
                row_id=f"exact-ni1-{label}",
                ni=1,
            )
            if row is not None:
                rows.append(row)
        return rows
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def harvest_ni_compare_convert() -> list[dict[str, Any]]:
    """NI=1 compare/convert rows (flush + RN-sensitive frsp under NI)."""
    set_scalar_fp_exact_v2_module_flag(True)
    try:
        rows: list[dict[str, Any]] = []
        for opcode, operands, label in _NI1_COMPARE_CONVERT_VECTORS:
            a = parse_int(operands["a"])
            b = parse_int(operands.get("b", "0"))
            fpscr = FPSCR_NI
            if opcode == "frsp":
                outcome = exact_frsp(a, fpscr=fpscr)
                if not outcome.supported:
                    continue
                rows.append(
                    {
                        "id": f"exact-{label}",
                        "opcode": "frsp",
                        "ni": 1,
                        "operands": {"a": _hex64(a)},
                        "expected_bits": _hex64(outcome.result_bits),
                        "provenance": PROVENANCE_EXACT,
                    }
                )
            elif opcode in _COMPARE:
                fn = exact_fcmpu if opcode == "fcmpu" else exact_fcmpo
                outcome = fn(a, b, fpscr=fpscr)
                if not outcome.scalar.supported:
                    continue
                rows.append(
                    {
                        "id": f"exact-{label}",
                        "opcode": opcode,
                        "ni": 1,
                        "operands": {"a": _hex64(a), "b": _hex64(b)},
                        "expected_fpcr_nibble": f"0x{int(outcome.fpcc) & 0xF:x}",
                        "provenance": PROVENANCE_EXACT,
                    }
                )
        return rows
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def harvest_non_rne_compare_convert() -> list[dict[str, Any]]:
    """Non-RNE ``frsp`` rows showing RN-dependent tie / pi rounding."""
    set_scalar_fp_exact_v2_module_flag(True)
    try:
        rows: list[dict[str, Any]] = []
        rn_by_name = {name: bits for bits, name, _slug in _NON_RNE_MODES}
        for input_bits, rn_name, label in _NON_RNE_FRSP_VECTORS:
            rn_bits = rn_by_name[rn_name]
            outcome = exact_frsp(input_bits, fpscr=rn_bits)
            if not outcome.supported:
                continue
            rows.append(
                {
                    "id": f"exact-{label}",
                    "opcode": "frsp",
                    "operands": {"a": _hex64(input_bits)},
                    "pre_fpscr": _hex32(rn_bits),
                    "rn": rn_name,
                    "expected_bits": _hex64(outcome.result_bits),
                    "provenance": PROVENANCE_EXACT,
                }
            )
        return rows
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def harvest_ni(cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    for case in cases:
        opcode = _fixture_opcode(case.id)
        if opcode not in _SCALAR_ARITH:
            continue
        if 7 not in case.expected_fpr:
            continue
        if _skip_scalar_mismatch_tags(case):
            continue
        row = _row_base(case, opcode)
        row["ni"] = 0
        row["expected_bits"] = _hex64(case.expected_fpr[7])
        rows.append(row)
    rows.extend(_exact_ni_rows(cases))
    rows.extend(harvest_ni_scalar_extras())
    return rows


def _filter_passing(
    rows: list[dict[str, Any]],
    *,
    corpus_file: str,
) -> list[dict[str, Any]]:
    header = _CORPUS_HEADERS[corpus_file]
    set_scalar_fp_exact_v2_module_flag(True)
    try:
        passing: list[dict[str, Any]] = []
        for row in rows:
            err = validate_corpus_row(row, corpus_file=corpus_file, header=header)
            if err is not None:
                continue
            try:
                result = replay_row(row, header, corpus_file=corpus_file)
            except (ValueError, TypeError):
                continue
            if result.passed:
                passing.append(row)
        return passing
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def _exact_scalar_supplement(
    *,
    corpus_id: str,
    need: int,
    seed_cases: tuple[FixtureCase, ...],
) -> list[dict[str, Any]]:
    """Fill short corpora with exact-kernel rows (honest provenance)."""
    set_scalar_fp_exact_v2_module_flag(True)
    try:
        rows: list[dict[str, Any]] = []
        if corpus_id == "fused_residual":
            for case in seed_cases:
                opcode = _fixture_opcode(case.id)
                if opcode not in _FUSED:
                    continue
                if "invalid" in case.tags or "snan" in case.tags:
                    continue
                operands = _operands(case, opcode)
                a = parse_int(operands["a"])
                b = parse_int(operands["b"])
                c = parse_int(operands["c"])
                from tools.ppc_equivalence.fp_exact_fused import dispatch_exact_fused

                try:
                    outcome = dispatch_exact_fused(opcode, a, c, b)
                except (ValueError, TypeError):
                    continue
                if not outcome.supported:
                    continue
                rows.append(
                    {
                        "id": f"exact-fused-{case.id}",
                        "opcode": opcode,
                        "operands": operands,
                        "expected_bits": _hex64(outcome.bits64),
                        "provenance": PROVENANCE_EXACT,
                    }
                )
                if len(rows) >= need:
                    return rows
            return rows

        edge_ids = (
            "fadd-qnan",
            "fadd-vxisi",
            "fsub-vxisi",
            "fmul-vximz",
            "fdiv-vxzdz",
            "fdiv-zx",
            "fdiv-vxidi",
            "fmuls-vximz",
            "fsubs-negative-zero",
            "fnmsubs-negative-zero",
            "fmsubs-positive-zero",
            "fadd",
            "fsub",
            "fmul",
            "fdiv",
            "fadds",
            "fsubs",
            "fmuls",
            "fdivs",
        )
        by_id = {case.id: case for case in seed_cases}
        for case_id in edge_ids:
            case = by_id.get(case_id)
            if case is None:
                continue
            opcode = _fixture_opcode(case.id)
            if opcode is None:
                continue
            if corpus_id == "scalar_rn" and opcode not in _SCALAR_ARITH:
                continue
            operands = _operands(case, opcode)
            pre = _initial_fpscr(case.initial)
            a = parse_int(operands["a"])
            b = parse_int(operands.get("b", "0"))
            c = parse_int(operands.get("c", b))
            outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=pre)
            if outcome is None or not outcome.supported:
                continue
            row: dict[str, Any] = {
                "id": f"exact-supplement-{corpus_id}-{case_id}",
                "opcode": opcode,
                "operands": operands,
                "expected_bits": _hex64(outcome.result_bits),
                "provenance": PROVENANCE_EXACT,
            }
            if corpus_id == "scalar_rn":
                row["rn"] = "nearest-even"
            if corpus_id == "ni":
                row["ni"] = 1
                row["id"] = f"exact-ni1-supplement-{case_id}"
                outcome_ni = exact_scalar_arith(
                    opcode,
                    a,
                    b,
                    c_bits=c,
                    fpscr=pre | FPSCR_NI,
                )
                if outcome_ni is None or not outcome_ni.supported:
                    continue
                row["expected_bits"] = _hex64(outcome_ni.result_bits)
            rows.append(row)
            if len(rows) >= need:
                break
        return rows
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def _exact_compare_convert_supplement(need: int) -> list[dict[str, Any]]:
    set_scalar_fp_exact_v2_module_flag(True)
    try:
        rows: list[dict[str, Any]] = []
        frsp_inputs = (
            (0x400921FB54442D18, "pi-double"),
            (0x7FF8000000000000, "qnan"),
            (0x7FF0000000000000, "inf"),
            (0x8000000000000000, "neg-zero"),
            (0x0000000000000001, "subnormal"),
            (0x3FF0000000000000, "one"),
            (0xFFF0000000000000, "neg-inf"),
            (0x7FF0000012345678, "snan"),
            (0x3FF8000000000000, "one-point-five"),
            (0x4010000000000000, "four"),
        )
        for bits, label in frsp_inputs:
            outcome = exact_frsp(bits, fpscr=0)
            if not outcome.supported:
                continue
            rows.append(
                {
                    "id": f"exact-frsp-{label}",
                    "opcode": "frsp",
                    "operands": {"a": _hex64(bits)},
                    "expected_bits": _hex64(outcome.result_bits),
                    "provenance": PROVENANCE_EXACT,
                }
            )
        compare_pairs = (
            (0x3FF0000000000000, 0x4000000000000000, "fcmpu", "fcmpu-lt"),
            (0x3FF0000000000000, 0x3FF0000000000000, "fcmpu", "fcmpu-eq"),
            (0x7FF8000000000000, 0x3FF0000000000000, "fcmpu", "fcmpu-unord"),
            (0x4000000000000000, 0x3FF0000000000000, "fcmpo", "fcmpo-gt"),
            (0x7FF8000012345678, 0x3FF0000000000000, "fcmpo", "fcmpo-qnan"),
            (0x7FF0000012345678, 0x4000000000000000, "fcmpo", "fcmpo-snan"),
            (0xFFF0000000000000, 0xFFF0000000000000, "fcmpu", "fcmpu-inf-eq"),
            (0x8000000000000000, 0x0000000000000000, "fcmpu", "fcmpu-signed-zero"),
            (0x0000000000000001, 0x0000000000000001, "fcmpu", "fcmpu-subnorm-eq"),
            (0x3FF8000000000000, 0xBFF8000000000000, "fcmpo", "fcmpo-mirror"),
        )
        for a, b, opcode, label in compare_pairs:
            fn = exact_fcmpu if opcode == "fcmpu" else exact_fcmpo
            outcome = fn(a, b, fpscr=0)
            if not outcome.scalar.supported:
                continue
            rows.append(
                {
                    "id": f"exact-{label}",
                    "opcode": opcode,
                    "operands": {"a": _hex64(a), "b": _hex64(b)},
                    "expected_fpcr_nibble": f"0x{int(outcome.fpcc) & 0xF:x}",
                    "provenance": PROVENANCE_EXACT,
                }
            )
        return rows[:need]
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def _exact_fpscr_supplement(need: int, seed_cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    set_scalar_fp_exact_v2_module_flag(True)
    try:
        rows: list[dict[str, Any]] = []
        for case in seed_cases:
            opcode = _fixture_opcode(case.id)
            if opcode not in _SCALAR_ARITH:
                continue
            if _skip_scalar_mismatch_tags(case):
                continue
            operands = _operands(case, opcode)
            pre = _initial_fpscr(case.initial)
            a = parse_int(operands["a"])
            b = parse_int(operands.get("b", "0"))
            c = parse_int(operands.get("c", b))
            outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=pre)
            if outcome is None or not outcome.supported:
                continue
            from tools.ppc_equivalence.fp_fpscr import apply_fpscr_transition

            post = apply_fpscr_transition(pre, opcode, outcome)
            if post == pre:
                continue
            row = {
                "id": f"exact-fpscr-{case.id}",
                "opcode": opcode,
                "operands": operands,
                "pre_fpscr": _hex32(pre),
                "expected_post_fpscr": _hex32(post),
                "expected_fpscr_mask": _hex32(mask32(post ^ pre)),
                "provenance": PROVENANCE_EXACT,
            }
            rows.append(row)
            if len(rows) >= need:
                break
        return rows
    finally:
        set_scalar_fp_exact_v2_module_flag(None)


def _pad_corpus(
    rows: list[dict[str, Any]],
    *,
    corpus_file: str,
    seed_cases: tuple[FixtureCase, ...],
) -> list[dict[str, Any]]:
    header_name = corpus_file
    passing = _filter_passing(rows, corpus_file=header_name)
    if len(passing) >= _MIN_ROWS:
        return passing[: max(len(passing), _MIN_ROWS)]
    corpus_id = _CORPUS_HEADERS[corpus_file]["corpus_id"]
    need = _MIN_ROWS - len(passing)
    if corpus_id == "compare_convert_control":
        passing.extend(_exact_compare_convert_supplement(need))
    elif corpus_id == "fpscr":
        passing.extend(_exact_fpscr_supplement(need, seed_cases))
    elif corpus_id == "traps_fe":
        passing.extend(_exact_trap_supplement(need, seed_cases))
    elif corpus_id in {"scalar_rn", "ni", "fused_residual"}:
        passing.extend(
            _exact_scalar_supplement(corpus_id=corpus_id, need=need, seed_cases=seed_cases)
        )
    passing = _filter_passing(passing, corpus_file=header_name)
    return passing


def _exact_trap_supplement(need: int, seed_cases: tuple[FixtureCase, ...]) -> list[dict[str, Any]]:
    rows: list[dict[str, Any]] = []
    for msr_fe in ("10", "00", "01", "11"):
        for case in seed_cases:
            if not any(
                tag in case.tags
                for tag in ("divide-zero", "invalid", "snan", "overflow")
            ):
                continue
            opcode = _fixture_opcode(case.id)
            if opcode is None or opcode not in _SCALAR_ARITH:
                continue
            row = _trap_row_from_fixture(
                case,
                opcode=opcode,
                msr_fe=msr_fe,
                suffix=f"supplement-{msr_fe}",
            )
            if row is None:
                continue
            row["id"] = f"exact-trap-{case.id}-{msr_fe}"
            row["provenance"] = PROVENANCE_EXACT
            rows.append(row)
            if len(rows) >= need:
                return rows
    return rows


def build_all_corpora(cases: tuple[FixtureCase, ...] | None = None) -> dict[str, list[dict[str, Any]]]:
    source = cases if cases is not None else FIXTURES
    return {
        "scalar_rn.jsonl": _pad_corpus(
            harvest_scalar_rn(source)
            + harvest_non_rne_scalar()
            + harvest_ni_scalar_extras(),
            corpus_file="scalar_rn.jsonl",
            seed_cases=source,
        ),
        "fpscr.jsonl": _pad_corpus(
            harvest_fpscr(source) + harvest_non_rne_fpscr(),
            corpus_file="fpscr.jsonl",
            seed_cases=source,
        ),
        "ni.jsonl": _pad_corpus(
            harvest_ni(source),
            corpus_file="ni.jsonl",
            seed_cases=source,
        ),
        "compare_convert_control.jsonl": _pad_corpus(
            harvest_compare_convert(source)
            + harvest_ni_compare_convert()
            + harvest_non_rne_compare_convert(),
            corpus_file="compare_convert_control.jsonl",
            seed_cases=source,
        ),
        "traps_fe.jsonl": _pad_corpus(
            harvest_traps(source), corpus_file="traps_fe.jsonl", seed_cases=source
        ),
        "fused_residual.jsonl": _pad_corpus(
            harvest_fused(source), corpus_file="fused_residual.jsonl", seed_cases=source
        ),
    }


def write_corpora(
    corpora: dict[str, list[dict[str, Any]]],
    *,
    corpus_dir: Path | None = None,
) -> None:
    root = corpus_dir or CORPUS_DIR
    root.mkdir(parents=True, exist_ok=True)
    for name in CORPUS_FILES:
        header = _CORPUS_HEADERS[name]
        rows = corpora[name]
        lines = [json.dumps(header, separators=(",", ":"), sort_keys=True)]
        for row in sorted(rows, key=lambda item: item["id"]):
            payload = dict(row)
            payload.pop("source_fixture", None)
            lines.append(json.dumps(payload, separators=(",", ":"), sort_keys=True))
        path = root / name
        path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def inventory(cases: tuple[FixtureCase, ...] | None = None) -> dict[str, Any]:
    source = cases if cases is not None else FIXTURES
    fp_cases = [case for case in source if "fp" in case.tags]
    harvestable = {
        "scalar_arith": len(harvest_scalar_rn(source)),
        "scalar_non_rne": len(harvest_non_rne_scalar()),
        "fpscr_delta": len(harvest_fpscr(source)),
        "fpscr_non_rne": len(harvest_non_rne_fpscr()),
        "compare_convert": len(harvest_compare_convert(source)),
        "compare_convert_ni1": len(harvest_ni_compare_convert()),
        "compare_convert_non_rne": len(harvest_non_rne_compare_convert()),
        "fused": len(harvest_fused(source)),
        "traps_derived": len(harvest_traps(source)),
        "ni_broadway_ni0": len(
            [c for c in harvest_ni(source) if c.get("provenance") == PROVENANCE_BROADWAY]
        ),
        "ni1_exact": len([c for c in harvest_ni(source) if c.get("ni") == 1]),
    }
    return {
        "fixture_sources": [
            "tools/ppc_equivalence/fixtures/corpus.py",
            "tools/ppc_equivalence/fixtures/broadway.jsonl",
        ],
        "fp_fixture_cases": len(fp_cases),
        "harvestable_by_family": harvestable,
        "gaps": [
            "No Broadway/Dolphin fixtures with FPSCR.NI=1 — NI=1 rows use exact_kernel_v2",
            "No live Dolphin rows for RTZ/RIP/RIM — non-RNE rows use exact_kernel_v2",
            "Trap rows derive FE expectations from exact trap planner, not live Dolphin MSR capture",
            "fctiw/fctiwz convert family not in scalar_fp_v2 compare_convert corpus schema",
            "Record-form fcmp* CR1 shadowing not harvested",
        ],
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Harvest scalar_fp_v2 corpora from Broadway fixtures")
    parser.add_argument("--write", action="store_true", help="Write corpora JSONL files")
    parser.add_argument("--inventory", action="store_true", help="Print harvest inventory JSON")
    parser.add_argument("--corpus-dir", type=Path, default=None)
    args = parser.parse_args(argv)

    if args.inventory:
        print(json.dumps(inventory(), indent=2, sort_keys=True))
        return 0

    if not args.write:
        parser.error("--write or --inventory required")

    corpora = build_all_corpora()
    write_corpora(corpora, corpus_dir=args.corpus_dir)
    report = check_corpora(corpus_dir=args.corpus_dir)
    counts = {name: len(corpora[name]) for name in CORPUS_FILES}
    print(json.dumps({"row_counts": counts, "passed": report.passed}, indent=2))
    if not report.passed:
        for item in report.row_results:
            if not item.passed:
                print(f"FAIL {item.corpus_file}:{item.row_id}: {item.reason}")
        return 1
    print("OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())

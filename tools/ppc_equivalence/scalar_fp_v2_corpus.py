"""Phase 11 scalar FP v2 independent corpus loader and replay grader.

Loads versioned JSONL corpora under ``corpora/scalar_fp_v2/``, replays each data
row through the exact-v2 dispatchers (arith, fused, compare, convert, NI,
FPSCR, traps), and returns pass/fail plus aggregate digests for obligation
binding.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.fp_capabilities import (
    set_scalar_fp_exact_v2_module_flag,
)
from tools.ppc_equivalence.fp_exact import exact_scalar_arith
from tools.ppc_equivalence.fp_exact_compare import exact_fcmpo, exact_fcmpu
from tools.ppc_equivalence.fp_exact_convert import exact_frsp
from tools.ppc_equivalence.fp_exact_fused import dispatch_exact_fused
from tools.ppc_equivalence.fp_exact_outcome import fp_outcome_from_scalar_outcome
from tools.ppc_equivalence.fp_fpscr import FPSCR_NI, apply_fpscr_transition
from tools.ppc_equivalence.fp_oracle import mask32, mask64
from tools.ppc_equivalence.fp_traps import MSR_FE0, MSR_FE1, plan_fp_trap_delivery
from tools.ppc_equivalence.provenance import canonical_json_sha256

CORPUS_DIR = Path(__file__).resolve().parent / "corpora" / "scalar_fp_v2"

CORPUS_FILES: tuple[str, ...] = (
    "scalar_rn.jsonl",
    "fpscr.jsonl",
    "ni.jsonl",
    "compare_convert_control.jsonl",
    "traps_fe.jsonl",
    "fused_residual.jsonl",
)

ALLOWED_PROVENANCE = frozenset(
    {
        "oracle_rne_interim",
        "exact_kernel_v2",
        "fixtures-broadway-jsonl",
        "dolphin-capture",
    }
)

_SCALAR_ARITH_OPS = frozenset(
    {"fadd", "fadds", "fsub", "fsubs", "fmul", "fmuls", "fdiv", "fdivs"}
)
_FUSED_OPS = frozenset(
    {"fmadd", "fmadds", "fmsub", "fmsubs", "fnmadd", "fnmadds", "fnmsub", "fnmsubs"}
)
_COMPARE_OPS = frozenset({"fcmpu", "fcmpo"})
_CONVERT_OPS = frozenset({"frsp"})


@dataclass(frozen=True, slots=True)
class RowReplayResult:
    """Outcome of replaying one corpus data row."""

    corpus_file: str
    row_id: str
    passed: bool
    reason: str = ""
    actual: dict[str, Any] = field(default_factory=dict)
    expected: dict[str, Any] = field(default_factory=dict)


@dataclass(frozen=True, slots=True)
class CorpusReplayReport:
    """Aggregate replay report for all corpora."""

    passed: bool
    corpus_sha256: str
    total_rows: int
    failed_rows: int
    row_results: tuple[RowReplayResult, ...]
    schema_errors: tuple[str, ...] = ()


def parse_hex_literal(value: Any, *, bits: int = 64) -> int:
    """Parse ``0x`` hex strings or integers; mask to ``bits`` width."""
    if isinstance(value, int):
        parsed = int(value)
    elif isinstance(value, str):
        text = value.strip().lower()
        if text.startswith("0x"):
            parsed = int(text, 16)
        else:
            parsed = int(text, 16 if any(c in "abcdef" for c in text) else 10)
    else:
        raise TypeError(f"expected int or hex string, got {type(value).__name__}")
    mask = (1 << bits) - 1
    return parsed & mask


def parse_msr_fe(value: Any) -> int:
    """Decode ``msr_fe`` corpus field (``"10"`` → FE0=1 FE1=0)."""
    if isinstance(value, int):
        fe0 = bool(value & 2)
        fe1 = bool(value & 1)
    else:
        text = str(value).strip()
        if len(text) != 2 or not text.isdigit():
            raise ValueError(f"invalid msr_fe {value!r}")
        fe0 = text[0] == "1"
        fe1 = text[1] == "1"
    msr = 0
    if fe0:
        msr |= MSR_FE0
    if fe1:
        msr |= MSR_FE1
    return msr


def load_corpus_file(path: Path) -> tuple[dict[str, Any], list[dict[str, Any]]]:
    """Return ``(header, data_rows)`` from one JSONL corpus file."""
    lines = path.read_text(encoding="utf-8").splitlines()
    if len(lines) < 2:
        raise ValueError(f"{path}: expected schema header plus data rows")
    header = json.loads(lines[0])
    rows = [json.loads(line) for line in lines[1:] if line.strip()]
    return header, rows


def load_all_corpora(
    corpus_dir: Path | None = None,
) -> dict[str, tuple[dict[str, Any], list[dict[str, Any]]]]:
    """Load every known scalar FP v2 corpus file."""
    root = corpus_dir or CORPUS_DIR
    loaded: dict[str, tuple[dict[str, Any], list[dict[str, Any]]]] = {}
    for name in CORPUS_FILES:
        path = root / name
        loaded[name] = load_corpus_file(path)
    return loaded


def validate_corpus_header(
    header: Mapping[str, Any],
    *,
    corpus_file: str,
) -> str | None:
    """Validate schema header; return error string or ``None``."""
    if header.get("type") != "scalar_fp_v2_corpus":
        return f"{corpus_file}: header.type must be scalar_fp_v2_corpus"
    if header.get("schema_version") != 1:
        return f"{corpus_file}: header.schema_version must be 1"
    if not header.get("corpus_id"):
        return f"{corpus_file}: header.corpus_id is required"
    if not header.get("capability"):
        return f"{corpus_file}: header.capability is required"
    return None


def validate_corpus_row(
    row: Mapping[str, Any],
    *,
    corpus_file: str,
    header: Mapping[str, Any],
) -> str | None:
    """Validate one data row before replay."""
    if not row.get("id"):
        return f"{corpus_file}: row missing id"
    provenance = row.get("provenance", "")
    if provenance not in ALLOWED_PROVENANCE:
        return (
            f"{corpus_file}:{row.get('id')}: unsupported provenance {provenance!r}"
        )
    if header.get("corpus_id") == "fused_residual":
        if row.get("opcode") not in _FUSED_OPS:
            return f"{corpus_file}:{row.get('id')}: fused corpus requires fused opcode"
    elif row.get("opcode") not in (
        _SCALAR_ARITH_OPS | _COMPARE_OPS | _CONVERT_OPS | _FUSED_OPS
    ):
        return f"{corpus_file}:{row.get('id')}: unknown opcode {row.get('opcode')!r}"
    operands = row.get("operands")
    if not isinstance(operands, Mapping):
        return f"{corpus_file}:{row.get('id')}: operands must be an object"
    return None


def compute_scalar_fp_v2_corpus_sha256(corpus_dir: Path | None = None) -> str:
    """Canonical digest over per-file content hashes for obligation binding."""
    root = corpus_dir or CORPUS_DIR
    payload: dict[str, str] = {}
    for name in sorted(CORPUS_FILES):
        text = (root / name).read_text(encoding="utf-8")
        payload[name] = hashlib.sha256(text.encode("utf-8")).hexdigest()
    return canonical_json_sha256(payload)


def _operand_bits(row: Mapping[str, Any]) -> tuple[int, int, int]:
    operands = row.get("operands") or {}
    a = parse_hex_literal(operands.get("a", 0))
    b = parse_hex_literal(operands.get("b", 0))
    c = parse_hex_literal(operands.get("c", b))
    return a, b, c


def _fpscr_from_row(row: Mapping[str, Any]) -> int:
    fpscr = parse_hex_literal(row.get("pre_fpscr", 0), bits=32)
    if int(row.get("ni", 0)) == 1:
        fpscr |= FPSCR_NI
    return mask32(fpscr)


def _replay_scalar_arith(row: Mapping[str, Any]) -> RowReplayResult:
    opcode = str(row["opcode"])
    a, b, c = _operand_bits(row)
    fpscr = _fpscr_from_row(row)
    outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=fpscr)
    if outcome is None:
        return _fail(row, "unsupported scalar arithmetic opcode")
    if not outcome.supported:
        return _fail(
            row,
            outcome.unsupported_reason or "exact scalar arith unsupported",
        )

    actual: dict[str, Any] = {"result_bits": mask64(outcome.result_bits)}
    expected: dict[str, Any] = {}
    if "expected_bits" in row:
        expected["result_bits"] = parse_hex_literal(row["expected_bits"])
        if actual["result_bits"] != expected["result_bits"]:
            return _row_result(row, False, "result_bits mismatch", actual, expected)
    return _row_result(row, True, "", actual, expected)


def _replay_fpscr(row: Mapping[str, Any]) -> RowReplayResult:
    opcode = str(row["opcode"])
    a, b, c = _operand_bits(row)
    pre_fpscr = _fpscr_from_row(row)
    if opcode in _CONVERT_OPS:
        if opcode == "frsp":
            outcome = exact_frsp(a, fpscr=pre_fpscr)
        else:
            return _fail(row, f"unsupported convert opcode {opcode!r} for fpscr row")
    else:
        outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=pre_fpscr)
    if outcome is None or not outcome.supported:
        return _fail(row, "exact op failed for fpscr row")
    post_fpscr = apply_fpscr_transition(pre_fpscr, opcode, outcome)
    delta = mask32(post_fpscr ^ pre_fpscr)
    actual = {"post_fpscr": post_fpscr, "delta": delta}
    expected: dict[str, Any] = {}

    if "expected_post_fpscr" in row:
        expected["post_fpscr"] = parse_hex_literal(row["expected_post_fpscr"], bits=32)
        if actual["post_fpscr"] != expected["post_fpscr"]:
            return _row_result(row, False, "post_fpscr mismatch", actual, expected)

    if "expected_fpscr_mask" in row:
        mask = parse_hex_literal(row["expected_fpscr_mask"], bits=32)
        expected["delta_mask"] = mask
        if (delta & mask) != mask:
            return _row_result(row, False, "fpscr delta mask mismatch", actual, expected)

    if "expected_fpscr" in row:
        expected["post_fpscr"] = parse_hex_literal(row["expected_fpscr"], bits=32)
        if actual["post_fpscr"] != expected["post_fpscr"]:
            return _row_result(row, False, "expected_fpscr mismatch", actual, expected)

    if not expected:
        return _fail(row, "fpscr row missing expected_post_fpscr or expected_fpscr_mask")
    return _row_result(row, True, "", actual, expected)


def _replay_ni(row: Mapping[str, Any]) -> RowReplayResult:
    return _replay_scalar_arith(row)


def _replay_compare_convert(row: Mapping[str, Any]) -> RowReplayResult:
    opcode = str(row["opcode"])
    a, b, _c = _operand_bits(row)
    fpscr = _fpscr_from_row(row)
    actual: dict[str, Any] = {}
    expected: dict[str, Any] = {}

    if opcode in _COMPARE_OPS:
        fn = exact_fcmpu if opcode == "fcmpu" else exact_fcmpo
        outcome = fn(a, b, fpscr=fpscr)
        if not outcome.scalar.supported:
            return _fail(row, "compare unsupported")
        actual["fpcr_nibble"] = int(outcome.fpcc) & 0xF
        if "expected_fpcr_nibble" in row:
            expected["fpcr_nibble"] = parse_hex_literal(
                row["expected_fpcr_nibble"],
                bits=4,
            )
            if actual["fpcr_nibble"] != expected["fpcr_nibble"]:
                return _row_result(row, False, "fpcr_nibble mismatch", actual, expected)
        else:
            return _fail(row, "compare row missing expected_fpcr_nibble")
        return _row_result(row, True, "", actual, expected)

    if opcode in _CONVERT_OPS:
        outcome = exact_frsp(a, fpscr=fpscr)
        if not outcome.supported:
            return _fail(row, outcome.unsupported_reason or "convert unsupported")
        actual["result_bits"] = mask64(outcome.result_bits)
        if "expected_bits" in row:
            expected["result_bits"] = parse_hex_literal(row["expected_bits"])
            if actual["result_bits"] != expected["result_bits"]:
                return _row_result(row, False, "result_bits mismatch", actual, expected)
        else:
            return _fail(row, "convert row missing expected_bits")
        return _row_result(row, True, "", actual, expected)

    return _fail(row, f"unsupported compare/convert opcode {opcode!r}")


def _replay_traps(row: Mapping[str, Any]) -> RowReplayResult:
    opcode = str(row["opcode"])
    a, b, c = _operand_bits(row)
    pre_fpscr = _fpscr_from_row(row)
    msr = parse_msr_fe(row.get("msr_fe", "00"))
    domain_traps = bool(row.get("domain_traps_enabled", True))

    outcome = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=pre_fpscr)
    if outcome is None or not outcome.supported:
        return _fail(row, "scalar arith failed for trap row")

    fp_outcome = fp_outcome_from_scalar_outcome(outcome)
    plan = plan_fp_trap_delivery(
        fp_outcome,
        pre_fpscr,
        msr=msr,
        domain_traps_enabled=domain_traps,
    )
    actual = {
        "writeback": bool(plan.writeback),
        "trap": bool(plan.trap),
        "enabled_exception": bool(plan.enabled_exception),
    }
    expected: dict[str, Any] = {}

    if "expected_writeback" in row:
        expected["writeback"] = bool(row["expected_writeback"])
        if actual["writeback"] != expected["writeback"]:
            return _row_result(row, False, "writeback mismatch", actual, expected)
    if "expected_trap" in row:
        expected["trap"] = bool(row["expected_trap"])
        if actual["trap"] != expected["trap"]:
            return _row_result(row, False, "trap mismatch", actual, expected)
    if "expected_enabled_exception" in row:
        expected["enabled_exception"] = bool(row["expected_enabled_exception"])
        if actual["enabled_exception"] != expected["enabled_exception"]:
            return _row_result(
                row,
                False,
                "enabled_exception mismatch",
                actual,
                expected,
            )

    if "expected_pending" in row:
        has_pending = plan.pending is not None
        expected["pending"] = bool(row["expected_pending"])
        actual["pending"] = has_pending
        if actual["pending"] != expected["pending"]:
            return _row_result(row, False, "pending mismatch", actual, expected)
        if has_pending and plan.pending is not None:
            if "expected_pending_recoverable" in row:
                expected["pending_recoverable"] = bool(row["expected_pending_recoverable"])
                actual["pending_recoverable"] = bool(plan.pending.recoverability)
                if actual["pending_recoverable"] != expected["pending_recoverable"]:
                    return _row_result(
                        row,
                        False,
                        "pending_recoverable mismatch",
                        actual,
                        expected,
                    )
            if "expected_pending_delivery" in row:
                expected["pending_delivery"] = str(row["expected_pending_delivery"])
                actual["pending_delivery"] = plan.pending.delivery_class.value
                if actual["pending_delivery"] != expected["pending_delivery"]:
                    return _row_result(
                        row,
                        False,
                        "pending_delivery mismatch",
                        actual,
                        expected,
                    )

    if not expected:
        return _fail(row, "trap row missing expected_writeback or expected_trap")
    return _row_result(row, True, "", actual, expected)


def _replay_fused(row: Mapping[str, Any]) -> RowReplayResult:
    opcode = str(row["opcode"])
    a, b, c = _operand_bits(row)
    if opcode.endswith("s"):
        fused = dispatch_exact_fused(opcode, a, c, b)
    else:
        fused = dispatch_exact_fused(opcode, a, c, b)
    if not fused.supported:
        return _fail(row, fused.unsupported_reason or "fused unsupported")
    actual = {"result_bits": mask64(fused.bits64)}
    expected: dict[str, Any] = {}
    if "expected_bits" in row:
        expected["result_bits"] = parse_hex_literal(row["expected_bits"])
        if actual["result_bits"] != expected["result_bits"]:
            return _row_result(row, False, "result_bits mismatch", actual, expected)
    else:
        return _fail(row, "fused row missing expected_bits")
    return _row_result(row, True, "", actual, expected)


def _fail(row: Mapping[str, Any], reason: str) -> RowReplayResult:
    return RowReplayResult(
        corpus_file=str(row.get("_corpus_file", "")),
        row_id=str(row.get("id", "")),
        passed=False,
        reason=reason,
    )


def _row_result(
    row: Mapping[str, Any],
    passed: bool,
    reason: str,
    actual: Mapping[str, Any],
    expected: Mapping[str, Any],
) -> RowReplayResult:
    return RowReplayResult(
        corpus_file=str(row.get("_corpus_file", "")),
        row_id=str(row.get("id", "")),
        passed=passed,
        reason=reason,
        actual=dict(actual),
        expected=dict(expected),
    )


def replay_row(
    row: Mapping[str, Any],
    header: Mapping[str, Any],
    *,
    corpus_file: str,
) -> RowReplayResult:
    """Replay one corpus row through the appropriate exact dispatcher."""
    tagged = dict(row)
    tagged["_corpus_file"] = corpus_file
    corpus_id = str(header.get("corpus_id", ""))
    dispatch = {
        "scalar_rn": _replay_scalar_arith,
        "fpscr": _replay_fpscr,
        "ni": _replay_ni,
        "compare_convert_control": _replay_compare_convert,
        "traps_fe": _replay_traps,
        "fused_residual": _replay_fused,
    }
    handler = dispatch.get(corpus_id)
    if handler is None:
        return _fail(tagged, f"unknown corpus_id {corpus_id!r}")
    return handler(tagged)


def replay_corpora(
    *,
    corpus_dir: Path | None = None,
    enable_exact_v2: bool = True,
) -> CorpusReplayReport:
    """Replay all corpora and return an aggregate pass/fail report."""
    if enable_exact_v2:
        set_scalar_fp_exact_v2_module_flag(True)
    try:
        root = corpus_dir or CORPUS_DIR
        schema_errors: list[str] = []
        row_results: list[RowReplayResult] = []
        for name in CORPUS_FILES:
            path = root / name
            header, rows = load_corpus_file(path)
            header_error = validate_corpus_header(header, corpus_file=name)
            if header_error is not None:
                schema_errors.append(header_error)
                continue
            for row in rows:
                row_error = validate_corpus_row(row, corpus_file=name, header=header)
                if row_error is not None:
                    schema_errors.append(row_error)
                    row_results.append(
                        RowReplayResult(
                            corpus_file=name,
                            row_id=str(row.get("id", "")),
                            passed=False,
                            reason=row_error,
                        )
                    )
                    continue
                row_results.append(replay_row(row, header, corpus_file=name))
        failed = sum(1 for item in row_results if not item.passed)
        total = len(row_results)
        passed = not schema_errors and failed == 0
        return CorpusReplayReport(
            passed=passed,
            corpus_sha256=compute_scalar_fp_v2_corpus_sha256(root),
            total_rows=total,
            failed_rows=failed + len(schema_errors),
            row_results=tuple(row_results),
            schema_errors=tuple(schema_errors),
        )
    finally:
        if enable_exact_v2:
            set_scalar_fp_exact_v2_module_flag(None)


def check_corpora(*, corpus_dir: Path | None = None) -> CorpusReplayReport:
    """Run replay and fail closed on any schema or row mismatch."""
    return replay_corpora(corpus_dir=corpus_dir, enable_exact_v2=True)


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Scalar FP v2 corpus replay grader")
    parser.add_argument(
        "--check",
        action="store_true",
        help="Replay all corpora and exit non-zero on failure",
    )
    parser.add_argument(
        "--corpus-dir",
        type=Path,
        default=None,
        help="Override corpus root (default: corpora/scalar_fp_v2)",
    )
    args = parser.parse_args(list(argv) if argv is not None else None)

    if not args.check:
        parser.error("--check is required")

    report = check_corpora(corpus_dir=args.corpus_dir)
    print(f"corpus_sha256={report.corpus_sha256}")
    print(f"rows={report.total_rows} failed={report.failed_rows}")
    for error in report.schema_errors:
        print(f"SCHEMA {error}")
    for item in report.row_results:
        if not item.passed:
            print(
                f"FAIL {item.corpus_file}:{item.row_id}: {item.reason} "
                f"actual={item.actual} expected={item.expected}",
            )
    if report.passed:
        print("OK")
        return 0
    return 1


if __name__ == "__main__":
    sys.exit(main())

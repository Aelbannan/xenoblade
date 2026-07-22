"""Independent GX FIFO Tier-A corpus loader and replay grader (``gx_fifo_v1``).

Loads versioned JSONL corpora under ``corpora/gx_fifo_v1/`` covering the three
``GX_FIFO_TIER_A.md`` capabilities:

- ``write_trace.jsonl``   -> ``gx-fifo-write-trace``  (device write behavior)
- ``read_policy.jsonl``   -> ``gx-fifo-read``          (outcome-3 fail-closed policy)
- ``loop_emission.jsonl`` -> ``mmio-loop-emission``    (CTR loop recognition/discharge)

Independence rule (see the task that created this module): every row's
``expected`` block is a hand-authored architectural-contract claim
(``oracle: contract-v2`` — what the reviewed Tier-A model *claims*, per
``GX_FIFO_TIER_A.md``), never a value captured by first invoking
``GxFifoStreamDevice`` / ``gx_fifo_loop`` / ``gx_fifo_loop_discharge`` and
saving whatever came out. Grading *does* execute those implementations
under test (there is no other way to grade them) and compares their actual
behavior against the corpus's declared expectations — this mirrors
``scalar_fp_v2_corpus.py``'s replay pattern exactly. A corpus row that
encodes a forged/mutated expectation must make ``--check`` fail; that is
what proves this grader is not a rubber stamp (see
``tests/test_gx_fifo_corpus.py``).

This module never generates ``expected_*`` fields from the model under test
and never treats the model's own output as ground truth when authoring
fixtures.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.device_model import AccessOutcome, GxFifoStreamDevice
from tools.ppc_equivalence.gx_fifo_loop import (
    DEFAULT_GX_BASE,
    build_gx_fifo_loop_plans,
    recognize_gx_fifo_loops,
)
from tools.ppc_equivalence.gx_fifo_loop_discharge import discharge_gx_fifo_loop_plan
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.provenance import canonical_json_sha256

CORPUS_DIR = Path(__file__).resolve().parent / "corpora" / "gx_fifo_v1"

CORPUS_FILES: tuple[str, ...] = (
    "write_trace.jsonl",
    "read_policy.jsonl",
    "loop_emission.jsonl",
)

HEADER_TYPES: dict[str, str] = {
    "write_trace.jsonl": "gx_fifo_v1_write_trace_corpus",
    "read_policy.jsonl": "gx_fifo_v1_read_policy_corpus",
    "loop_emission.jsonl": "gx_fifo_v1_loop_emission_corpus",
}

CORPUS_ID_BY_FILE: dict[str, str] = {
    "write_trace.jsonl": "write_trace",
    "read_policy.jsonl": "read_policy",
    "loop_emission.jsonl": "loop_emission",
}

CAPABILITY_BY_FILE: dict[str, str] = {
    "write_trace.jsonl": "gx-fifo-write-trace",
    "read_policy.jsonl": "gx-fifo-read",
    "loop_emission.jsonl": "mmio-loop-emission",
}

ALLOWED_ORACLE = frozenset({"contract-v2"})

_STORE_OPCODES: dict[str, Opcode] = {
    "stb": Opcode.STB,
    "sth": Opcode.STH,
    "stw": Opcode.STW,
}
_WIDTH_BY_STORE_OPCODE: dict[str, int] = {"stb": 1, "sth": 2, "stw": 4}

_LOOP_PROGRAM_SHAPES = frozenset(
    {
        "standard",
        "unresolved_trip",
        "extra_ram_store",
        "insert_compare",
        "address_outside_region",
        "swap_body_order",
        "affine_target_mismatch",
    }
)


@dataclass(frozen=True, slots=True)
class RowReplayResult:
    """Outcome of replaying one corpus data row against the model under test."""

    corpus_file: str
    row_id: str
    passed: bool
    reason: str = ""
    actual: dict[str, Any] = field(default_factory=dict)
    expected: dict[str, Any] = field(default_factory=dict)


@dataclass(frozen=True, slots=True)
class CorpusReplayReport:
    """Aggregate replay report for all ``gx_fifo_v1`` corpora."""

    passed: bool
    corpus_sha256: str
    total_rows: int
    failed_rows: int
    row_results: tuple[RowReplayResult, ...]
    schema_errors: tuple[str, ...] = ()


# --------------------------------------------------------------------------
# Loading / schema validation
# --------------------------------------------------------------------------


def parse_hex_literal(value: Any, *, bits: int = 64) -> int:
    """Parse ``0x``-prefixed hex strings or plain ints; mask to ``bits`` width."""
    if isinstance(value, bool):
        raise TypeError("expected int or hex string, got bool")
    if isinstance(value, int):
        parsed = int(value)
    elif isinstance(value, str):
        text = value.strip().lower()
        parsed = int(text, 16) if text.startswith("0x") else int(text, 10)
    else:
        raise TypeError(f"expected int or hex string, got {type(value).__name__}")
    mask = (1 << bits) - 1
    return parsed & mask


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
    """Load every known ``gx_fifo_v1`` corpus file."""
    root = corpus_dir or CORPUS_DIR
    return {name: load_corpus_file(root / name) for name in CORPUS_FILES}


def validate_corpus_header(header: Mapping[str, Any], *, corpus_file: str) -> str | None:
    """Validate schema header; return an error string or ``None``."""
    expected_type = HEADER_TYPES.get(corpus_file)
    if header.get("type") != expected_type:
        return f"{corpus_file}: header.type must be {expected_type!r}"
    if header.get("schema_version") != 1:
        return f"{corpus_file}: header.schema_version must be 1"
    if header.get("corpus_id") != CORPUS_ID_BY_FILE.get(corpus_file):
        return f"{corpus_file}: header.corpus_id must be {CORPUS_ID_BY_FILE.get(corpus_file)!r}"
    if header.get("capability") != CAPABILITY_BY_FILE.get(corpus_file):
        return f"{corpus_file}: header.capability must be {CAPABILITY_BY_FILE.get(corpus_file)!r}"
    if header.get("oracle") not in ALLOWED_ORACLE:
        return f"{corpus_file}: header.oracle must be one of {sorted(ALLOWED_ORACLE)}"
    if header.get("hardware_profile") != "wii-broadway-xenoblade-us-v2":
        return f"{corpus_file}: header.hardware_profile must be wii-broadway-xenoblade-us-v2"
    return None


def validate_corpus_row(row: Mapping[str, Any], *, corpus_file: str) -> str | None:
    """Validate one data row's shape before replay."""
    if not row.get("id"):
        return f"{corpus_file}: row missing id"
    if row.get("capability") != CAPABILITY_BY_FILE.get(corpus_file):
        return f"{corpus_file}:{row.get('id')}: row.capability mismatch"
    if row.get("oracle") not in ALLOWED_ORACLE:
        return f"{corpus_file}:{row.get('id')}: row.oracle must be one of {sorted(ALLOWED_ORACLE)}"
    if not isinstance(row.get("expected"), Mapping):
        return f"{corpus_file}:{row.get('id')}: row.expected must be an object"
    corpus_id = CORPUS_ID_BY_FILE.get(corpus_file)
    if corpus_id in ("write_trace", "read_policy"):
        ops = row.get("ops")
        if not isinstance(ops, list) or not ops:
            return f"{corpus_file}:{row.get('id')}: row.ops must be a nonempty list"
        for op in ops:
            if not isinstance(op, Mapping) or op.get("kind") not in ("read", "write"):
                return f"{corpus_file}:{row.get('id')}: each op needs kind in read|write"
    elif corpus_id == "loop_emission":
        program = row.get("program")
        if not isinstance(program, Mapping):
            return f"{corpus_file}:{row.get('id')}: row.program must be an object"
        shape = program.get("shape", "standard")
        if shape not in _LOOP_PROGRAM_SHAPES:
            return f"{corpus_file}:{row.get('id')}: unknown program.shape {shape!r}"
        opcode = program.get("store_opcode", "stw")
        if opcode not in _STORE_OPCODES:
            return f"{corpus_file}:{row.get('id')}: unknown program.store_opcode {opcode!r}"
    return None


def compute_gx_fifo_v1_corpus_sha256(corpus_dir: Path | None = None) -> str:
    """Canonical digest over per-file content hashes for obligation binding."""
    root = corpus_dir or CORPUS_DIR
    payload: dict[str, str] = {}
    for name in sorted(CORPUS_FILES):
        text = (root / name).read_text(encoding="utf-8")
        payload[name] = hashlib.sha256(text.encode("utf-8")).hexdigest()
    return canonical_json_sha256(payload)


# --------------------------------------------------------------------------
# write_trace.jsonl replay
# --------------------------------------------------------------------------


_HARDWARE_PROFILE_V2_SPAN = 128  # wii-broadway-xenoblade-us-v2 gx-fifo region: 0xcc008000-0xcc00807f


def _build_device(row: Mapping[str, Any]) -> GxFifoStreamDevice:
    overrides = dict(row.get("device") or {})
    kwargs: dict[str, Any] = {"base": DEFAULT_GX_BASE, "span": _HARDWARE_PROFILE_V2_SPAN}
    if "span" in overrides:
        kwargs["span"] = int(overrides["span"])
    if "max_fifo_events" in overrides:
        kwargs["max_fifo_events"] = int(overrides["max_fifo_events"])
    return GxFifoStreamDevice(**kwargs)


def _replay_write_trace_row(row: Mapping[str, Any], *, corpus_file: str) -> RowReplayResult:
    device = _build_device(row)
    ops = row.get("ops") or []
    actual_outcomes: list[str] = []
    for op in ops:
        addr = parse_hex_literal(op["address"], bits=32)
        width = int(op["width"])
        value = parse_hex_literal(op.get("value", 0))
        result = device.write(addr, width, value)
        actual_outcomes.append(result.outcome.value)

    expected_outcomes = [str(op.get("expected_outcome", "")) for op in ops]
    actual: dict[str, Any] = {
        "outcomes": actual_outcomes,
        "final_cursor": device.event_cursor,
        "final_write_count": len(device.write_events),
        "events": [
            {
                "event_index": event["event_index"],
                "address": event["address"],
                "width": event["width"],
                "value": event["value"],
            }
            for event in device.write_events
        ],
    }
    expected = dict(row["expected"])
    expected["outcomes"] = expected_outcomes

    if actual_outcomes != expected_outcomes:
        return _row_result(row, corpus_file, False, "op outcomes mismatch", actual, expected)
    if device.event_cursor != int(expected.get("final_cursor", -1)):
        return _row_result(row, corpus_file, False, "final_cursor mismatch", actual, expected)
    if len(device.write_events) != int(expected.get("final_write_count", -1)):
        return _row_result(row, corpus_file, False, "final_write_count mismatch", actual, expected)
    expected_events = list(expected.get("events") or [])
    if len(expected_events) != len(device.write_events):
        return _row_result(row, corpus_file, False, "event count mismatch", actual, expected)
    for actual_event, expected_event in zip(device.write_events, expected_events):
        if int(actual_event["event_index"]) != int(expected_event["event_index"]):
            return _row_result(row, corpus_file, False, "event_index mismatch", actual, expected)
        if parse_hex_literal(actual_event["address"], bits=32) != parse_hex_literal(
            expected_event["address"], bits=32,
        ):
            return _row_result(row, corpus_file, False, "event address mismatch", actual, expected)
        if int(actual_event["width"]) != int(expected_event["width"]):
            return _row_result(row, corpus_file, False, "event width mismatch", actual, expected)
        if parse_hex_literal(actual_event["value"]) != parse_hex_literal(expected_event["value"]):
            return _row_result(row, corpus_file, False, "event value mismatch", actual, expected)
    return _row_result(row, corpus_file, True, "", actual, expected)


# --------------------------------------------------------------------------
# read_policy.jsonl replay
# --------------------------------------------------------------------------


def _replay_read_policy_row(row: Mapping[str, Any], *, corpus_file: str) -> RowReplayResult:
    device = _build_device(row)
    ops = row.get("ops") or []
    cursor_before = device.event_cursor
    actual_outcomes: list[str] = []
    for op in ops:
        addr = parse_hex_literal(op["address"], bits=32)
        width = int(op["width"])
        kind = op.get("kind", "read")
        if kind == "write":
            value = parse_hex_literal(op.get("value", 0))
            result = device.write(addr, width, value)
        else:
            result = device.read(addr, width)
        actual_outcomes.append(result.outcome.value)

    expected_outcomes = [str(op.get("expected_outcome", "")) for op in ops]
    expected = dict(row["expected"])
    expected["outcomes"] = expected_outcomes
    actual: dict[str, Any] = {
        "outcomes": actual_outcomes,
        "cursor_delta": device.event_cursor - cursor_before,
        "read_event_count": len(device.read_events),
        "final_write_count": len(device.write_events),
    }

    if actual_outcomes != expected_outcomes:
        return _row_result(row, corpus_file, False, "op outcomes mismatch", actual, expected)
    if actual["cursor_delta"] != int(expected.get("cursor_delta", -1)):
        return _row_result(row, corpus_file, False, "cursor_delta mismatch", actual, expected)
    if actual["read_event_count"] != int(expected.get("read_event_count", -1)):
        return _row_result(row, corpus_file, False, "read_event_count mismatch", actual, expected)
    if "final_write_count" in expected and actual["final_write_count"] != int(
        expected["final_write_count"],
    ):
        return _row_result(row, corpus_file, False, "final_write_count mismatch", actual, expected)
    return _row_result(row, corpus_file, True, "", actual, expected)


# --------------------------------------------------------------------------
# loop_emission.jsonl replay
# --------------------------------------------------------------------------


def _materialize_gx_base(reg: int, address: int) -> list[Instruction]:
    """``addis reg,0,0xCC00; ori reg,reg,0x8000`` -> ``reg = DEFAULT_GX_BASE``."""
    return [
        Instruction(address, 0, Opcode.ADDIS, (reg, 0, 0xCC00)),
        Instruction(address + 4, 0, Opcode.ORI, (reg, reg, 0x8000)),
    ]


def build_loop_program(spec: Mapping[str, Any]) -> list[Instruction]:
    """Build a small PPC instruction program from ``loop_emission.jsonl`` row parameters.

    This is an independent instruction-sequence builder written for this
    grader (not imported from the test suite); it never consults
    ``gx_fifo_loop``/``gx_fifo_loop_discharge`` to decide what to emit — the
    row's ``expected`` block (hand-authored) is the sole ground truth graded
    against whatever this program actually recognizes/discharges.
    """
    shape = str(spec.get("shape", "standard"))
    base_reg = int(spec.get("base_reg", 5))
    source_reg = int(spec.get("source_reg", 3))
    store_opcode = _STORE_OPCODES[str(spec.get("store_opcode", "stw"))]
    count = int(spec.get("count", 1))
    affine_step = spec.get("affine_step")

    instructions: list[Instruction] = []
    pc = 0

    if shape == "address_outside_region":
        # Base resolves concretely, but to a RAM address, not the GX region.
        instructions.append(Instruction(pc, 0, Opcode.ADDI, (base_reg, 0, 0x1000)))
        pc += 4
    else:
        for insn in _materialize_gx_base(base_reg, pc):
            instructions.append(insn)
            pc += 4

    if shape == "unresolved_trip":
        trip_reg = int(spec.get("trip_reg", 7))
        # Deliberately no materialization of trip_reg before mtctr.
    else:
        trip_reg = 0
        instructions.append(Instruction(pc, 0, Opcode.ADDI, (trip_reg, 0, count)))
        pc += 4

    instructions.append(Instruction(pc, 0, Opcode.MTSPR, (trip_reg, 9)))
    pc += 4

    header_pc = pc
    store_insn = Instruction(pc, 0, store_opcode, (source_reg, base_reg, 0))

    if shape == "swap_body_order" and affine_step is not None:
        addi_first = Instruction(pc, 0, Opcode.ADDI, (source_reg, source_reg, int(affine_step)))
        pc += 4
        store_second = Instruction(pc, 0, store_opcode, (source_reg, base_reg, 0))
        pc += 4
        body = [addi_first, store_second]
    else:
        pc += 4
        body = [store_insn]
        if shape == "extra_ram_store":
            body.append(Instruction(pc, 0, Opcode.STW, (source_reg, 6, 0)))
            pc += 4
        elif shape == "insert_compare":
            body.append(Instruction(pc, 0, Opcode.CMPWI, (0, source_reg, 0)))
            pc += 4
        elif shape == "affine_target_mismatch" and affine_step is not None:
            wrong_reg = source_reg + 1
            body.append(
                Instruction(pc, 0, Opcode.ADDI, (wrong_reg, wrong_reg, int(affine_step))),
            )
            pc += 4
        elif affine_step is not None:
            body.append(
                Instruction(pc, 0, Opcode.ADDI, (source_reg, source_reg, int(affine_step))),
            )
            pc += 4

    instructions.extend(body)
    latch_pc = pc
    instructions.append(Instruction(latch_pc, 0, Opcode.BC, (16, 0, header_pc, 0)))
    instructions.append(Instruction(latch_pc + 4, 0, Opcode.BCLR, (20, 0, 0)))
    return instructions


def _replay_loop_emission_row(row: Mapping[str, Any], *, corpus_file: str) -> RowReplayResult:
    program = dict(row.get("program") or {})
    max_events = int(program.get("max_events", 256))
    instructions = build_loop_program(program)

    candidates = recognize_gx_fifo_loops(instructions, max_events=max_events)
    plans = build_gx_fifo_loop_plans(instructions, max_events=max_events)

    expected = dict(row["expected"])
    actual: dict[str, Any] = {"candidate_found": bool(candidates)}
    if candidates:
        actual["confidence"] = candidates[0].confidence

    if actual["candidate_found"] != bool(expected.get("candidate_found")):
        return _row_result(row, corpus_file, False, "candidate_found mismatch", actual, expected)
    if "confidence" in expected and actual.get("confidence") != expected["confidence"]:
        return _row_result(row, corpus_file, False, "confidence mismatch", actual, expected)

    actual["plan_built"] = bool(plans)
    if actual["plan_built"] != bool(expected.get("plan_built")):
        return _row_result(row, corpus_file, False, "plan_built mismatch", actual, expected)

    if plans:
        plan = next(iter(plans.values()))
        summary = plan.summary
        actual["trip_count"] = summary.trip_count
        actual["address"] = hex(summary.address)
        actual["width"] = summary.width
        actual["value_kind"] = summary.value_kind

        if "trip_count" in expected and actual["trip_count"] != int(expected["trip_count"]):
            return _row_result(row, corpus_file, False, "trip_count mismatch", actual, expected)
        if "address" in expected and parse_hex_literal(actual["address"], bits=32) != parse_hex_literal(
            expected["address"], bits=32,
        ):
            return _row_result(row, corpus_file, False, "address mismatch", actual, expected)
        if "width" in expected and actual["width"] != int(expected["width"]):
            return _row_result(row, corpus_file, False, "width mismatch", actual, expected)
        if "value_kind" in expected and actual["value_kind"] != expected["value_kind"]:
            return _row_result(row, corpus_file, False, "value_kind mismatch", actual, expected)

        if "discharge_status" in expected:
            result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
            actual["discharge_status"] = result.status
            if actual["discharge_status"] != expected["discharge_status"]:
                return _row_result(
                    row, corpus_file, False, "discharge_status mismatch", actual, expected,
                )
    return _row_result(row, corpus_file, True, "", actual, expected)


def _row_result(
    row: Mapping[str, Any],
    corpus_file: str,
    passed: bool,
    reason: str,
    actual: Mapping[str, Any],
    expected: Mapping[str, Any],
) -> RowReplayResult:
    return RowReplayResult(
        corpus_file=corpus_file,
        row_id=str(row.get("id", "")),
        passed=passed,
        reason=reason,
        actual=dict(actual),
        expected=dict(expected),
    )


def replay_row(row: Mapping[str, Any], *, corpus_file: str) -> RowReplayResult:
    """Replay one corpus row through the appropriate model-under-test path."""
    corpus_id = CORPUS_ID_BY_FILE.get(corpus_file)
    if corpus_id == "write_trace":
        return _replay_write_trace_row(row, corpus_file=corpus_file)
    if corpus_id == "read_policy":
        return _replay_read_policy_row(row, corpus_file=corpus_file)
    if corpus_id == "loop_emission":
        return _replay_loop_emission_row(row, corpus_file=corpus_file)
    return _row_result(row, corpus_file, False, f"unknown corpus_id {corpus_id!r}", {}, {})


def replay_corpora(*, corpus_dir: Path | None = None) -> CorpusReplayReport:
    """Replay all ``gx_fifo_v1`` corpora and return an aggregate pass/fail report."""
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
            row_error = validate_corpus_row(row, corpus_file=name)
            if row_error is not None:
                schema_errors.append(row_error)
                row_results.append(
                    RowReplayResult(
                        corpus_file=name,
                        row_id=str(row.get("id", "")),
                        passed=False,
                        reason=row_error,
                    ),
                )
                continue
            row_results.append(replay_row(row, corpus_file=name))

    failed = sum(1 for item in row_results if not item.passed)
    total = len(row_results)
    passed = not schema_errors and failed == 0
    return CorpusReplayReport(
        passed=passed,
        corpus_sha256=compute_gx_fifo_v1_corpus_sha256(root),
        total_rows=total,
        failed_rows=failed + len(schema_errors),
        row_results=tuple(row_results),
        schema_errors=tuple(schema_errors),
    )


def check_corpora(*, corpus_dir: Path | None = None) -> CorpusReplayReport:
    """Run replay and fail closed on any schema or row mismatch."""
    return replay_corpora(corpus_dir=corpus_dir)


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="GX FIFO v1 corpus replay grader")
    parser.add_argument(
        "--check",
        action="store_true",
        help="Replay all corpora and exit non-zero on failure",
    )
    parser.add_argument(
        "--corpus-dir",
        type=Path,
        default=None,
        help="Override corpus root (default: corpora/gx_fifo_v1)",
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

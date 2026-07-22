"""Live Dolphin capture for scalar_fp_v2 NI=1 and non-RNE corpus rows.

Generates a table-driven PPC harness, runs it under headless Dolphin (CPUCore=0
interpreter), reads ``g_scalar_fp_v2_capture_results`` via the GDB stub, and
optionally upgrades matching ``exact_kernel_v2`` corpus rows to
``dolphin-capture`` provenance.
"""

from __future__ import annotations

import argparse
import json
import re
import struct
import subprocess
import sys
import time
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Mapping, Sequence

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.ppc_equivalence.fixtures.corpus import _fp_a, _fp_cmp, _fp_x  # noqa: PLC2701
from tools.ppc_equivalence.fixtures.encode import parse_int
from tools.ppc_equivalence.fp_fpscr import FPSCR_NI, FPSCR_VE, apply_fpscr_transition
from tools.ppc_equivalence.fp_exact import exact_scalar_arith
from tools.ppc_equivalence.fp_exact_compare import exact_fcmpo, exact_fcmpu
from tools.ppc_equivalence.fp_exact_convert import exact_frsp
from tools.ppc_equivalence.fp_capabilities import set_scalar_fp_exact_v2_module_flag
from tools.ppc_equivalence.fp_oracle import mask32, mask64
from tools.ppc_equivalence.scalar_fp_v2_corpus import (
    CORPUS_DIR,
    CORPUS_FILES,
    check_corpora,
    load_corpus_file,
    parse_hex_literal,
)

PPC_DIR = ROOT / "tools" / "test" / "compare_behaviour" / "ppc"
CASES_INC = PPC_DIR / "scalar_fp_v2_capture_cases.inc"
PAYLOADS_C = PPC_DIR / "scalar_fp_v2_capture_payloads.c"
CAPTURE_JSONL = CORPUS_DIR / "dolphin_capture.jsonl"
PLAN_MD = CORPUS_DIR / "dolphin_capture_plan.md"

PROVENANCE_CAPTURE = "dolphin-capture"
PROVENANCE_EXACT = "exact_kernel_v2"

_COMPARE_OPS = frozenset({"fcmpu", "fcmpo"})
_CONVERT_OPS = frozenset({"frsp"})
_SCALAR_OPS = frozenset(
    {"fadd", "fadds", "fsub", "fsubs", "fmul", "fmuls", "fdiv", "fdivs"}
)

_CAPTURE_OBS = struct.Struct(">IIQII")  # magic, pad, fpr7, fpscr, cr
_CAPTURE_OBS_SIZE = _CAPTURE_OBS.size


@dataclass(frozen=True, slots=True)
class CaptureTarget:
    corpus_file: str
    row_id: str
    opcode: str
    operands: dict[str, str]
    fpscr_pre: int
    ni: int
    rn: str | None
    expected: dict[str, Any]


@dataclass(frozen=True, slots=True)
class CaptureResult:
    row_id: str
    corpus_file: str
    result_bits: int | None
    post_fpscr: int | None
    fpcr_nibble: int | None
    dolphin_ok: bool
    error: str = ""


def _hex64(value: int) -> str:
    return f"0x{value & 0xFFFFFFFFFFFFFFFF:016x}"


def _hex32(value: int) -> str:
    return f"0x{value & 0xFFFFFFFF:08x}"


def _fpscr_from_row(row: Mapping[str, Any]) -> int:
    fpscr = parse_hex_literal(row.get("pre_fpscr", 0), bits=32)
    if int(row.get("ni", 0)) == 1:
        fpscr |= FPSCR_NI
    return mask32(fpscr)


def _is_non_rne_row(row: Mapping[str, Any]) -> bool:
    if row.get("rn") in {"toward-zero", "toward-plus-infinity", "toward-minus-infinity"}:
        return True
    pre = row.get("pre_fpscr")
    if pre is None:
        return False
    return bool(parse_hex_literal(pre, bits=32) & 3)


def _is_capture_candidate(row: Mapping[str, Any]) -> bool:
    if row.get("provenance") != PROVENANCE_EXACT:
        return False
    if int(row.get("ni", 0)) == 1:
        return True
    return _is_non_rne_row(row)


def _expected_from_row(row: Mapping[str, Any]) -> dict[str, Any]:
    expected: dict[str, Any] = {}
    if "expected_bits" in row:
        expected["result_bits"] = parse_hex_literal(row["expected_bits"])
    if "expected_post_fpscr" in row:
        expected["post_fpscr"] = parse_hex_literal(row["expected_post_fpscr"], bits=32)
    if "expected_fpscr_mask" in row:
        expected["fpscr_mask"] = parse_hex_literal(row["expected_fpscr_mask"], bits=32)
    if "expected_fpcr_nibble" in row:
        expected["fpcr_nibble"] = parse_hex_literal(row["expected_fpcr_nibble"], bits=4)
    return expected


def load_capture_targets(*, corpus_dir: Path | None = None) -> list[CaptureTarget]:
    root = corpus_dir or CORPUS_DIR
    targets: list[CaptureTarget] = []
    seen: set[str] = set()
    for name in CORPUS_FILES:
        _header, rows = load_corpus_file(root / name)
        for row in rows:
            if not _is_capture_candidate(row):
                continue
            row_id = str(row["id"])
            if row_id in seen:
                continue
            seen.add(row_id)
            targets.append(
                CaptureTarget(
                    corpus_file=name,
                    row_id=str(row["id"]),
                    opcode=str(row["opcode"]),
                    operands=dict(row.get("operands") or {}),
                    fpscr_pre=_fpscr_from_row(row),
                    ni=int(row.get("ni", 0)),
                    rn=row.get("rn"),
                    expected=_expected_from_row(row),
                )
            )
    return sorted(targets, key=lambda item: (item.corpus_file, item.row_id))


def _operand_values(target: CaptureTarget) -> tuple[int, int, int]:
    ops = target.operands
    a = parse_hex_literal(ops.get("a", 0))
    b = parse_hex_literal(ops.get("b", 0))
    c = parse_hex_literal(ops.get("c", b))
    return a, b, c


def _fpr_slots_for_opcode(opcode: str, a: int, b: int, c: int) -> tuple[int, int, int]:
    """Return (fpr1,fpr2,fpr3) guest register payloads for the encoded insn."""
    if opcode == "frsp":
        return 0, a, 0
    if opcode in {"fmul", "fmuls"}:
        return a, b, c
    return a, b, c


def encode_opcode_word(opcode: str) -> int:
    if opcode == "fadd":
        return _fp_a(63, 7, 1, 2, 0, 42)
    if opcode == "fadds":
        return _fp_a(59, 7, 1, 2, 0, 42)
    if opcode == "fsub":
        return _fp_a(63, 7, 1, 2, 0, 40)
    if opcode == "fsubs":
        return _fp_a(59, 7, 1, 2, 0, 40)
    if opcode == "fmul":
        return _fp_a(63, 7, 1, 0, 3, 50)
    if opcode == "fmuls":
        return _fp_a(59, 7, 1, 0, 3, 50)
    if opcode == "fdiv":
        return _fp_a(63, 7, 1, 2, 0, 36)
    if opcode == "fdivs":
        return _fp_a(59, 7, 1, 2, 0, 36)
    if opcode == "frsp":
        return _fp_x(7, 0, 2, 12)
    if opcode == "fcmpu":
        return _fp_cmp(0, 1, 2, 0)
    if opcode == "fcmpo":
        return _fp_cmp(0, 1, 2, 32)
    raise ValueError(f"unsupported capture opcode {opcode!r}")


def generate_guest_sources(targets: Sequence[CaptureTarget]) -> None:
    payload_lines = [
        "/* Generated by tools/ppc_equivalence/scalar_fp_v2_capture.py --gen */",
        "#include <types.h>",
        "",
    ]
    case_lines = [
        "/* Generated by tools/ppc_equivalence/scalar_fp_v2_capture.py --gen */",
        "#ifndef SCALAR_FP_V2_CAPTURE_CASES_INC",
        "#define SCALAR_FP_V2_CAPTURE_CASES_INC",
        "",
        "typedef void (*CapturePayloadFn)(void);",
        "",
        "typedef struct ScalarFpV2CaptureCase {",
        "    const char* id;",
        "    u64 fpr1;",
        "    u64 fpr2;",
        "    u64 fpr3;",
        "    u64 fpr7_pre;",
        "    u64 fpscr_pre;",
        "    CapturePayloadFn payload;",
        "} ScalarFpV2CaptureCase;",
        "",
        f"#define SCALAR_FP_V2_CAPTURE_COUNT {len(targets)}",
        "",
    ]
    for index, target in enumerate(targets):
        a, b, c = _operand_values(target)
        fpr1, fpr2, fpr3 = _fpr_slots_for_opcode(target.opcode, a, b, c)
        word = encode_opcode_word(target.opcode)
        payload_lines.extend(
            [
                f"asm void scalar_fp_v2_capture_payload_{index}(void) {{",
                "    nofralloc",
                f"    opword 0x{word:08X}",
                "    blr",
                "}",
                "",
            ]
        )
        case_lines.append(f"void scalar_fp_v2_capture_payload_{index}(void);")
    case_lines.append("")
    case_lines.append("static const ScalarFpV2CaptureCase g_scalar_fp_v2_capture_cases[] = {")
    for index, target in enumerate(targets):
        a, b, c = _operand_values(target)
        fpr1, fpr2, fpr3 = _fpr_slots_for_opcode(target.opcode, a, b, c)
        fpr7_pre = 0
        if target.fpscr_pre & FPSCR_VE and "result_bits" in target.expected:
            fpr7_pre = int(target.expected["result_bits"])
        case_lines.append("    {")
        case_lines.append(f'        "{target.row_id}",')
        case_lines.append(f"        0x{fpr1:016X}ull,")
        case_lines.append(f"        0x{fpr2:016X}ull,")
        case_lines.append(f"        0x{fpr3:016X}ull,")
        case_lines.append(f"        0x{fpr7_pre:016X}ull,")
        case_lines.append(f"        0x{target.fpscr_pre:016X}ull,")
        case_lines.append(f"        scalar_fp_v2_capture_payload_{index},")
        case_lines.append("    },")
    case_lines.extend(["};", "", "#endif", ""])
    PAYLOADS_C.write_text("\n".join(payload_lines), encoding="utf-8")
    CASES_INC.write_text("\n".join(case_lines), encoding="utf-8")


def _dolphin_version(dolphin: str) -> str:
    try:
        proc = subprocess.run(
            [dolphin, "--version"],
            capture_output=True,
            text=True,
            check=False,
            timeout=10,
        )
        text = (proc.stdout or proc.stderr or "").strip()
        return text.splitlines()[0] if text else "unknown"
    except (OSError, subprocess.TimeoutExpired):
        return "unknown"


def run_dolphin_capture(
    targets: Sequence[CaptureTarget],
    *,
    dolphin: str | None = None,
) -> tuple[list[CaptureResult], str]:
    from tools.test.compare_behaviour.lib.manifest import load_manifest
    from tools.test.compare_behaviour.lib.ppc_runner import find_dolphin, run_ppc_compare

    generate_guest_sources(targets)
    _, tests = load_manifest()
    test = next(item for item in tests if item.id == "scalar-fp-v2-capture")
    result = run_ppc_compare(test, require_dolphin=True)
    dolphin_bin = dolphin or result.dolphin or find_dolphin()
    if dolphin_bin is None:
        raise RuntimeError("Dolphin not found")
    version = _dolphin_version(dolphin_bin)

    if not result.ok and result.passed is None:
        raise RuntimeError(f"Dolphin capture failed: {result.output[-2000:]}")

    from tools.test.compare_behaviour.lib.map_lookup import lookup_symbol_address
    from tools.test.compare_behaviour.lib.ppc_builder import build_ppc_test, ppc_build_dir
    from tools.test.compare_behaviour.lib.gdb_stub import gdb_read_memory_at_exit

    build = build_ppc_test(test)
    if not build.ok or build.map_path is None:
        raise RuntimeError(f"PPC build failed: {build.output}")

    obs_addr = lookup_symbol_address(build.map_path, "g_scalar_fp_v2_capture_results")
    exit_addr = lookup_symbol_address(build.map_path, "exit")
    if obs_addr is None or exit_addr is None:
        raise RuntimeError("capture symbols missing from PPC map")

    # Re-run with explicit GDB read if the runner did not leave memory behind.
    user_dir = ppc_build_dir(test.region, test.id) / "dolphin_user_capture"
    log_path = ppc_build_dir(test.region, test.id) / "dolphin_capture.log"
    user_dir.mkdir(parents=True, exist_ok=True)
    from tools.test.compare_behaviour.lib.ppc_runner import _write_dolphin_user_ini

    gdb_port = 2162
    if log_path.is_file():
        log_path.unlink()
    _write_dolphin_user_ini(
        user_dir,
        gdb_port=gdb_port,
        log_path=log_path,
        cpu_core=test.ppc_cpu_core,
    )
    proc = subprocess.Popen(
        [dolphin_bin, "--exec", str(build.dol), "--batch", f"--user={user_dir}"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    mem: dict[int, bytes] | None = None
    try:
        time.sleep(3.0)
        mem = gdb_read_memory_at_exit(
            "127.0.0.1",
            gdb_port,
            exit_addr=exit_addr,
            reads=[(obs_addr, _CAPTURE_OBS_SIZE * len(targets))],
            timeout_s=20.0,
        )
    finally:
        proc.terminate()
        try:
            proc.communicate(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.communicate(timeout=1)

    if mem is None or obs_addr not in mem:
        raise RuntimeError("GDB stub did not return capture observations")

    blob = mem[obs_addr]
    results: list[CaptureResult] = []
    for index, target in enumerate(targets):
        chunk = blob[index * _CAPTURE_OBS_SIZE : (index + 1) * _CAPTURE_OBS_SIZE]
        if len(chunk) != _CAPTURE_OBS_SIZE:
            results.append(
                CaptureResult(
                    row_id=target.row_id,
                    corpus_file=target.corpus_file,
                    result_bits=None,
                    post_fpscr=None,
                    fpcr_nibble=None,
                    dolphin_ok=False,
                    error="short read",
                )
            )
            continue
        magic, _pad, fpr7, post_fpscr, post_cr = _CAPTURE_OBS.unpack(chunk)
        if magic != (0xCAFE0000 | index):
            results.append(
                CaptureResult(
                    row_id=target.row_id,
                    corpus_file=target.corpus_file,
                    result_bits=None,
                    post_fpscr=None,
                    fpcr_nibble=None,
                    dolphin_ok=False,
                    error=f"bad magic 0x{magic:08x}",
                )
            )
            continue
        fpcr = (post_cr >> 28) & 0xF if target.opcode in _COMPARE_OPS else None
        results.append(
            CaptureResult(
                row_id=target.row_id,
                corpus_file=target.corpus_file,
                result_bits=mask64(fpr7),
                post_fpscr=mask32(post_fpscr),
                fpcr_nibble=fpcr,
                dolphin_ok=True,
            )
        )
    return results, version


def _matches_expected(target: CaptureTarget, captured: CaptureResult) -> tuple[bool, str]:
    if not captured.dolphin_ok:
        return False, captured.error or "capture failed"
    expected = target.expected
    if "result_bits" in expected:
        if captured.result_bits != expected["result_bits"]:
            return (
                False,
                f"result_bits dolphin={_hex64(captured.result_bits or 0)} "
                f"expected={_hex64(expected['result_bits'])}",
            )
    if "fpscr_mask" in expected and captured.post_fpscr is not None:
        delta = mask32(captured.post_fpscr ^ target.fpscr_pre)
        if (delta & expected["fpscr_mask"]) != expected["fpscr_mask"]:
            return False, f"fpscr delta mask mismatch delta={_hex32(delta)}"
    elif "post_fpscr" in expected:
        if captured.post_fpscr != expected["post_fpscr"]:
            return (
                False,
                f"post_fpscr dolphin={_hex32(captured.post_fpscr or 0)} "
                f"expected={_hex32(expected['post_fpscr'])}",
            )
    if "fpcr_nibble" in expected:
        if captured.fpcr_nibble != expected["fpcr_nibble"]:
            return (
                False,
                f"fpcr_nibble dolphin=0x{captured.fpcr_nibble:x} "
                f"expected=0x{expected['fpcr_nibble']:x}",
            )
    return True, ""


def softfloat_crosscheck(targets: Sequence[CaptureTarget]) -> list[dict[str, Any]]:
    """Cross-check RNE-eligible rows against SoftFloat oracle (non-RNE/NI skipped)."""
    from tools.ppc_equivalence.fp_oracle import (
        fadd_binary64_rne,
        fadds_fpr_rne,
        fdiv_binary64_rne,
        fdivs_fpr_rne,
        fmul_binary64_rne,
        fmuls_fpr_rne,
        fsub_binary64_rne,
        fsubs_fpr_rne,
    )

    oracle = {
        "fadd": fadd_binary64_rne,
        "fsub": fsub_binary64_rne,
        "fmul": fmul_binary64_rne,
        "fdiv": fdiv_binary64_rne,
        "fadds": fadds_fpr_rne,
        "fsubs": fsubs_fpr_rne,
        "fmuls": fmuls_fpr_rne,
        "fdivs": fdivs_fpr_rne,
    }
    report: list[dict[str, Any]] = []
    for target in targets:
        if target.ni == 1 or _is_non_rne_row({"rn": target.rn, "pre_fpscr": _hex32(target.fpscr_pre)}):
            report.append(
                {
                    "row_id": target.row_id,
                    "status": "skipped",
                    "reason": "SoftFloat oracle is RNE-only",
                }
            )
            continue
        fn = oracle.get(target.opcode)
        if fn is None:
            report.append(
                {
                    "row_id": target.row_id,
                    "status": "skipped",
                    "reason": f"no SoftFloat adapter for {target.opcode}",
                }
            )
            continue
        a, b, c = _operand_values(target)
        if target.opcode in {"fmul", "fmuls"}:
            result = fn(a, c)
        else:
            result = fn(a, b)
        bits = mask64(result.bits64)
        ok = "result_bits" not in target.expected or bits == target.expected["result_bits"]
        report.append(
            {
                "row_id": target.row_id,
                "status": "pass" if ok else "fail",
                "softfloat_bits": _hex64(bits),
            }
        )
    return report


def write_capture_jsonl(
    targets: Sequence[CaptureTarget],
    results: Sequence[CaptureResult],
    *,
    dolphin_version: str,
    path: Path | None = None,
) -> Path:
    out = path or CAPTURE_JSONL
    lines = [
        json.dumps(
            {
                "type": "scalar_fp_v2_dolphin_capture",
                "schema_version": 1,
                "captured_at": datetime.now(timezone.utc).isoformat(),
                "dolphin_version": dolphin_version,
                "row_count": len(results),
            },
            separators=(",", ":"),
            sort_keys=True,
        )
    ]
    by_id = {item.row_id: item for item in results}
    for target in targets:
        captured = by_id[target.row_id]
        match, detail = _matches_expected(target, captured)
        payload: dict[str, Any] = {
            "corpus_file": target.corpus_file,
            "id": target.row_id,
            "opcode": target.opcode,
            "operands": target.operands,
            "provenance": PROVENANCE_CAPTURE,
            "dolphin_version": dolphin_version,
            "matches_exact_kernel": match,
        }
        if target.ni:
            payload["ni"] = target.ni
        if target.rn:
            payload["rn"] = target.rn
        if target.fpscr_pre & ~FPSCR_NI:
            payload["pre_fpscr"] = _hex32(target.fpscr_pre & ~FPSCR_NI)
        if captured.result_bits is not None:
            payload["captured_bits"] = _hex64(captured.result_bits)
        if captured.post_fpscr is not None:
            payload["captured_post_fpscr"] = _hex32(captured.post_fpscr)
        if captured.fpcr_nibble is not None:
            payload["captured_fpcr_nibble"] = f"0x{captured.fpcr_nibble:x}"
        if not match:
            payload["mismatch_detail"] = detail
        lines.append(json.dumps(payload, separators=(",", ":"), sort_keys=True))
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return out


def apply_capture_to_corpora(
    targets: Sequence[CaptureTarget],
    results: Sequence[CaptureResult],
    *,
    corpus_dir: Path | None = None,
    dolphin_version: str,
) -> dict[str, Any]:
    root = corpus_dir or CORPUS_DIR
    by_id = {item.row_id: item for item in results}
    upgraded: list[str] = []
    mismatches: list[str] = []
    upgrade_ids: set[str] = set()
    for target in targets:
        captured = by_id[target.row_id]
        match, detail = _matches_expected(target, captured)
        if not match:
            mismatches.append(f"{target.row_id}: {detail}")
            continue
        upgrade_ids.add(target.row_id)
        upgraded.append(target.row_id)

    rows_by_file: dict[str, list[dict[str, Any]]] = {}
    headers: dict[str, dict[str, Any]] = {}
    for name in CORPUS_FILES:
        header, rows = load_corpus_file(root / name)
        headers[name] = header
        rows_by_file[name] = rows

    for name, rows in rows_by_file.items():
        changed = False
        for index, row in enumerate(rows):
            row_id = str(row.get("id", ""))
            if row_id not in upgrade_ids:
                continue
            new_row = dict(row)
            new_row["provenance"] = PROVENANCE_CAPTURE
            new_row["dolphin_version"] = dolphin_version
            rows[index] = new_row
            changed = True
        if changed:
            lines = [json.dumps(headers[name], separators=(",", ":"), sort_keys=True)]
            lines.extend(
                json.dumps(row, separators=(",", ":"), sort_keys=True) for row in rows
            )
            (root / name).write_text("\n".join(lines) + "\n", encoding="utf-8")

    return {
        "upgraded": upgraded,
        "upgraded_count": len(upgraded),
        "mismatches": mismatches,
        "mismatch_count": len(mismatches),
    }


def write_capture_plan(
    targets: Sequence[CaptureTarget],
    results: Sequence[CaptureResult] | None = None,
    *,
    softfloat: Sequence[Mapping[str, Any]] | None = None,
    blockers: Sequence[str] | None = None,
) -> None:
    by_id = {item.row_id: item for item in results} if results else {}
    lines = [
        "# Scalar FP v2 Dolphin capture plan",
        "",
        "Rows still on ``exact_kernel_v2`` that need live Broadway/Dolphin attestation.",
        "",
        "## Inventory",
        "",
        f"- Capture candidates: **{len(targets)}** (NI=1 + non-RNE ``exact_kernel_v2`` rows)",
        f"- Broadway ``broadway.jsonl`` NI=1 fixtures: **0**",
        f"- Broadway non-RNE fixtures (scalar): **0** (only ``fctiw*`` convert tags)",
        "",
    ]
    if blockers:
        lines.extend(["## Blockers", ""])
        for item in blockers:
            lines.append(f"- {item}")
        lines.append("")
    if results is not None:
        matched = sum(
            1
            for target in targets
            if _matches_expected(target, by_id[target.row_id])[0]
        )
        lines.extend(
            [
                "## Last capture run",
                "",
                f"- Dolphin-attested (match exact kernel): **{matched}**",
                f"- Mismatches / failures: **{len(targets) - matched}**",
                "",
            ]
        )
    lines.extend(["## Remaining gaps (by theme)", ""])
    themes: dict[str, list[str]] = {}
    for target in targets:
        if results and _matches_expected(target, by_id[target.row_id])[0]:
            continue
        if target.ni == 1:
            theme = "NI=1"
        elif target.opcode in _COMPARE_OPS:
            theme = "non-RNE compare"
        elif target.opcode in _CONVERT_OPS:
            theme = "non-RNE convert (frsp)"
        elif target.corpus_file == "fpscr.jsonl":
            theme = "non-RNE FPSCR transitions"
        else:
            theme = "non-RNE scalar result"
        themes.setdefault(theme, []).append(target.row_id)
    for theme, ids in sorted(themes.items()):
        lines.append(f"### {theme} ({len(ids)})")
        lines.append("")
        for row_id in ids[:12]:
            lines.append(f"- `{row_id}`")
        if len(ids) > 12:
            lines.append(f"- … and {len(ids) - 12} more")
        lines.append("")
    lines.extend(
        [
            "## Exact inputs to capture next",
            "",
            "Re-run:",
            "",
            "```bash",
            "python3 -m tools.ppc_equivalence.scalar_fp_v2_capture --gen",
            "python3 -m tools.ppc_equivalence.scalar_fp_v2_capture --capture --apply",
            "python3 -m tools.ppc_equivalence.scalar_fp_v2_corpus --check",
            "```",
            "",
            "Harness: ``tools/test/compare_behaviour/ppc/scalar_fp_v2_capture.c`` "
            "(manifest id ``scalar-fp-v2-capture``, CPUCore=0 interpreter).",
            "",
        ]
    )
    if softfloat:
        sf_fail = [item for item in softfloat if item.get("status") == "fail"]
        if sf_fail:
            lines.extend(["## SoftFloat cross-check failures", ""])
            for item in sf_fail:
                lines.append(f"- `{item['row_id']}`: {item.get('softfloat_bits')}")
            lines.append("")
    PLAN_MD.write_text("\n".join(lines), encoding="utf-8")


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Dolphin capture for scalar_fp_v2 NI/non-RNE rows")
    parser.add_argument("--gen", action="store_true", help="Generate PPC capture tables only")
    parser.add_argument("--capture", action="store_true", help="Run headless Dolphin capture")
    parser.add_argument("--apply", action="store_true", help="Upgrade matching rows to dolphin-capture")
    parser.add_argument("--softfloat-check", action="store_true", help="RNE SoftFloat cross-check")
    parser.add_argument("--write-plan", action="store_true", help="Write dolphin_capture_plan.md")
    parser.add_argument("--corpus-dir", type=Path, default=None)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args(list(argv) if argv is not None else None)

    if not any(
        (
            args.gen,
            args.capture,
            args.apply,
            args.softfloat_check,
            args.write_plan,
        )
    ):
        parser.error("one of --gen, --capture, --apply, --softfloat-check, --write-plan required")

    targets = load_capture_targets(corpus_dir=args.corpus_dir)
    payload: dict[str, Any] = {"targets": len(targets)}

    if args.gen or args.capture:
        generate_guest_sources(targets)
        payload["generated"] = [str(CASES_INC), str(PAYLOADS_C)]

    softfloat_report: list[dict[str, Any]] | None = None
    if args.softfloat_check:
        softfloat_report = softfloat_crosscheck(targets)
        payload["softfloat"] = softfloat_report

    results: list[CaptureResult] | None = None
    dolphin_version = "not-run"
    blockers: list[str] = []

    if args.capture:
        try:
            results, dolphin_version = run_dolphin_capture(targets)
            write_capture_jsonl(targets, results, dolphin_version=dolphin_version)
            payload["dolphin_version"] = dolphin_version
            payload["capture_jsonl"] = str(CAPTURE_JSONL)
            result_by_id = {item.row_id: item for item in results}
            matched = sum(
                1
                for target in targets
                if _matches_expected(target, result_by_id[target.row_id])[0]
            )
            payload["dolphin_matched"] = matched
            payload["dolphin_mismatches"] = len(targets) - matched
        except RuntimeError as exc:
            blockers.append(str(exc))
            payload["capture_error"] = str(exc)

    if args.apply:
        if results is None:
            if not CAPTURE_JSONL.is_file():
                blockers.append("no capture JSONL; run --capture first")
            else:
                blockers.append("--apply requires --capture in the same invocation")
        else:
            apply_report = apply_capture_to_corpora(
                targets,
                results,
                corpus_dir=args.corpus_dir,
                dolphin_version=dolphin_version,
            )
            payload["apply"] = apply_report
            report = check_corpora(corpus_dir=args.corpus_dir)
            payload["corpus_check_passed"] = report.passed
            if not report.passed:
                return 1

    if args.write_plan or args.capture:
        write_capture_plan(
            targets,
            results,
            softfloat=softfloat_report,
            blockers=blockers or None,
        )
        payload["plan"] = str(PLAN_MD)

    if args.json:
        print(json.dumps(payload, indent=2, sort_keys=True))
    else:
        print(json.dumps(payload, indent=2, sort_keys=True))
    return 0 if not blockers else 1


if __name__ == "__main__":
    sys.exit(main())

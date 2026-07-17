from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any

from . import __version__
from .contract import CONTRACT_PRESETS, Observable, make_contract, parse_observables
from .decoder import decode_block, parse_hex
from .elf_symbols import (
    ElfSymbolError,
    extract_function,
    extract_function_pair,
    list_text_functions,
)
from .engine import check_equivalence
from .ir import DecodeError, ExecutionInconclusive, Opcode, UnsupportedInstruction
from .memory_profile import (
    PROFILE_CHOICES,
    MemoryEnvironment,
    MemoryProfile,
    parse_ranges,
)
from .model import MachineState, concrete_state
from .result import ProofResult, ProofStatus
from .semantics import ConcreteOps, automatic_live_out, execute_cfg


def _parse_int(value: str) -> int:
    return int(value, 0)


def _format_hex(code: bytes) -> str:
    return " ".join(code[index : index + 4].hex() for index in range(0, len(code), 4))


def _observable_concrete(state: MachineState, observable: Observable) -> object:
    if observable.kind == "gpr":
        assert observable.index is not None
        return f"0x{int(state.gpr[observable.index]) & 0xFFFFFFFF:08x}"
    if observable.kind == "sr":
        assert observable.index is not None
        return f"0x{int(state.sr[observable.index]) & 0xFFFFFFFF:08x}"
    if observable.kind == "spr":
        assert observable.index is not None
        return f"0x{int(state.spr[observable.index]) & 0xFFFFFFFF:08x}"
    if observable.kind == "cr_field":
        assert observable.index is not None
        return f"0x{(int(state.cr) >> ((7 - observable.index) * 4)) & 0xF:x}"
    if observable.kind in ("cr", "lr", "ctr", "msr", "srr0", "srr1"):
        return f"0x{int(getattr(state, observable.kind)) & 0xFFFFFFFF:08x}"
    if observable.kind == "time_base":
        return f"0x{int(state.time_base) & 0xFFFFFFFFFFFFFFFF:016x}"
    if observable.kind == "fpscr":
        return f"0x{int(state.fpscr) & 0xFFFFFFFF:08x}"
    if observable.kind in ("fpr", "ps1", "gqr"):
        assert observable.index is not None
        value = getattr(state, observable.kind)[observable.index]
        width = 16 if observable.kind in ("fpr", "ps1") else 8
        return f"0x{int(value) & ((1 << (width * 4)) - 1):0{width}x}"
    if observable.kind == "xer":
        return int(bool(getattr(state.xer, observable.name.split(".", 1)[1])))
    if observable.kind == "memory":
        return state.memory
    raise AssertionError(observable.kind)


def _terminal_mismatches_concrete(
    left: Any,
    right: Any,
    observables: tuple[Observable, ...],
) -> list[dict[str, object]]:
    """Mirror the symbolic terminal comparison for counterexample replay."""
    mismatches: list[dict[str, object]] = []
    if left.exit_kind != right.exit_kind:
        mismatches.append({
            "name": "exit.kind", "original": left.exit_kind, "candidate": right.exit_kind,
        })
        return mismatches
    if left.exit_kind != "fallthrough" and left.exit_target != right.exit_target:
        def target(value: object) -> object:
            return f"0x{int(value) & 0xFFFFFFFF:08x}" if value is not None else None
        mismatches.append({
            "name": "exit.target",
            "original": target(left.exit_target),
            "candidate": target(right.exit_target),
        })
        return mismatches
    if bool(left.state.valid) != bool(right.state.valid):
        mismatches.append({
            "name": "defined-domain",
            "original": int(bool(left.state.valid)),
            "candidate": int(bool(right.state.valid)),
        })
        return mismatches
    if not bool(left.state.valid):
        return mismatches
    for observable in observables:
        original = _observable_concrete(left.state, observable)
        candidate = _observable_concrete(right.state, observable)
        if original != candidate:
            mismatches.append({
                "name": observable.name, "original": original, "candidate": candidate,
            })
    return mismatches


def _print_result(result: ProofResult) -> None:
    if result.status == ProofStatus.EQUIVALENT:
        print("EQUIVALENT UNDER CONTRACT")
    elif result.status == ProofStatus.NOT_EQUIVALENT:
        print("NOT EQUIVALENT")
    else:
        print("INCONCLUSIVE")
    print(f"status: {result.status.value}")
    print(f"contract: {result.contract}")
    if result.contract_resolution:
        print(f"contract base: {result.contract_resolution['base']}")
        added = result.contract_resolution.get("added", [])
        print(f"auto-added: {', '.join(added) if added else '(none)'}")
        for reason in result.contract_resolution.get("reasons", []):
            print(f"contract reason: {reason}")
    if result.observables:
        print(f"compared: {', '.join(result.observables)}")
    if result.mismatch:
        print(
            "first mismatch: "
            f"{result.mismatch['name']} "
            f"({result.mismatch['original']} != {result.mismatch['candidate']})"
        )
    if result.counterexample:
        registers = result.counterexample["initial_state"]["gpr"]
        if registers:
            print("counterexample inputs:")
            for name, value in registers.items():
                print(f"  {name} = {value}")
    if result.repair_hint:
        rh = result.repair_hint
        if "diverged_register" in rh:
            print(
                f"repair hint: {rh['diverged_register']} diverged at insn {rh['instruction_index']}"
            )
            print(f"  original:  {rh.get('original_insn', '?')}  →  {rh.get('original_value', '?')}")
            print(f"  candidate: {rh.get('candidate_insn', '?')}  →  {rh.get('candidate_value', '?')}")
        elif "detail" in rh:
            print(f"repair hint: {rh['detail']}")
    if result.assumed_callees:
        print("assumed callees: " + ", ".join(
            f"0x{target:08x}" if isinstance(target, int) else target
            for target in result.assumed_callees
        ))
        for callee, contract in result.callee_contracts.items():
            print(f"callee contract: {callee} ({contract.get('source', 'unknown')})")
    for warning in result.warnings:
        print(f"warning: {warning}")
    for item in result.unsupported:
        print(f"unsupported: {item}")


def _exit_for_status(status: ProofStatus) -> int:
    if status == ProofStatus.EQUIVALENT:
        return 0
    if status == ProofStatus.NOT_EQUIVALENT:
        return 1
    if status in (
        ProofStatus.INCONCLUSIVE_TIMEOUT,
        ProofStatus.INCONCLUSIVE_UNKNOWN,
        ProofStatus.INCONCLUSIVE_UNSUPPORTED,
        ProofStatus.INCONCLUSIVE_ABSTRACTION,
        ProofStatus.INCONCLUSIVE_LAYOUT,
        ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE,
    ):
        return 2
    if status == ProofStatus.INVALID_INPUT:
        return 3
    return 4


def _emit(result: ProofResult, json_output: bool, result_file: Path | None, replay_file: Path | None) -> int:
    payload = result.to_dict()
    if result_file:
        result_file.parent.mkdir(parents=True, exist_ok=True)
        result_file.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    if replay_file and result.replay:
        replay_file.parent.mkdir(parents=True, exist_ok=True)
        replay_file.write_text(json.dumps(result.replay, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    if json_output:
        print(json.dumps(payload, indent=2, sort_keys=True))
    else:
        _print_result(result)
        if replay_file and result.replay:
            print(f"replay: {replay_file}")
    return _exit_for_status(result.status)


def _run_check(
    args: argparse.Namespace,
    original_code: bytes,
    candidate_code: bytes,
    *,
    base_original: int | None = None,
    base_candidate: int | None = None,
    original_relocations: tuple[Any, ...] = (),
    candidate_relocations: tuple[Any, ...] = (),
    original_symbol: str | None = None,
    candidate_symbol: str | None = None,
) -> int:
    original_hex = _format_hex(original_code)
    candidate_hex = _format_hex(candidate_code)
    contract = None
    requested_contract = args.contract
    if requested_contract is None and not args.observe:
        requested_contract = getattr(args, "default_contract", None)
    timeout_ms = args.timeout_ms
    if timeout_ms <= 0:
        instr_count = max(len(original_code), len(candidate_code)) // 4
        timeout_ms = max(5_000, min(120_000, instr_count * 20))
    memory_env = None
    if hasattr(args, "memory_profile") and args.memory_profile:
        profile = MemoryProfile(args.memory_profile)
        ranges = parse_ranges(args.memory_ranges) if args.memory_ranges else []
        memory_env = MemoryEnvironment(profile=profile, ranges=ranges)
    try:
        original = decode_block(
            original_code,
            args.base_original if base_original is None else base_original,
            relocations=original_relocations,
            local_symbol=original_symbol,
        )
        candidate = decode_block(
            candidate_code,
            args.base_candidate if base_candidate is None else base_candidate,
            relocations=candidate_relocations,
            local_symbol=candidate_symbol,
        )
        assumed_callees = frozenset(
            insn.relocation.canonical_symbol
            for insn in (*original, *candidate)
            if insn.opcode in (Opcode.B, Opcode.BC) and insn.relocation is not None
        )
        original_live_out = automatic_live_out(original)
        candidate_live_out = automatic_live_out(candidate)
        live_out = None
        if requested_contract == "live-out":
            live_out = tuple(dict.fromkeys(original_live_out + candidate_live_out))
        contract = make_contract(
            preset=requested_contract,
            observe=args.observe,
            timeout_ms=timeout_ms,
            live_out=live_out,
            original_live_out=original_live_out,
            candidate_live_out=candidate_live_out,
        )
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex=original_hex,
            candidate_hex=candidate_hex,
            smt_output=str(args.smt_out) if args.smt_out else None,
            max_instructions=args.max_instructions,
            max_paths=args.max_paths,
            assumed_callees=assumed_callees,
            memory_environment=memory_env,
        )
    except (UnsupportedInstruction, ExecutionInconclusive) as exc:
        contract_name = requested_contract or "manual"
        observable_names = (
            [item.name for item in contract.observables]
            if contract else list(args.observe or ())
        )
        result = ProofResult(
            status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            contract=contract_name,
            contract_resolution=contract.resolution_dict() if contract else None,
            observables=observable_names,
            unsupported=[str(exc)],
        )
    return _emit(result, args.json, args.result, args.replay_out)


def cmd_decode(args: argparse.Namespace) -> int:
    code = parse_hex(args.hex) if args.hex else args.file.read_bytes()
    instructions = decode_block(code, args.base)
    payload = [instruction.to_dict() for instruction in instructions]
    if args.json:
        print(json.dumps(payload, indent=2))
    else:
        for instruction in payload:
            suffix = "." if instruction["record"] and not str(instruction["opcode"]).endswith(".") else ""
            print(
                f"{instruction['address']}: {instruction['bytes']}  "
                f"{instruction['opcode']}{suffix} {instruction['operands']}"
            )
    return 0


def cmd_replay(args: argparse.Namespace) -> int:
    case: dict[str, Any] = json.loads(args.case.read_text(encoding="utf-8"))
    original_code = parse_hex(str(case["original_hex"]))
    candidate_code = parse_hex(str(case["candidate_hex"]))
    observables = parse_observables(list(case["observables"]))
    initial = concrete_state(case.get("initial_state"))
    ops = ConcreteOps()
    original = decode_block(original_code, int(case.get("base_original", 0)), validate_with_capstone=False)
    candidate = decode_block(candidate_code, int(case.get("base_candidate", 0)), validate_with_capstone=False)
    original_exits = [item for item in execute_cfg(initial, original, ops) if item.condition]
    candidate_exits = [item for item in execute_cfg(initial, candidate, ops) if item.condition]
    if len(original_exits) != 1 or len(candidate_exits) != 1:
        raise ValueError("replay did not select exactly one path per program")
    mismatches = _terminal_mismatches_concrete(
        original_exits[0], candidate_exits[0], observables,
    )
    expected = case.get("expected_mismatch")
    reproduced = (
        any(item["name"] == expected for item in mismatches)
        if expected is not None else bool(mismatches)
    )
    payload = {
        "reproduced": reproduced,
        "expected_mismatch": expected,
        "mismatches": mismatches,
    }
    if args.json:
        print(json.dumps(payload, indent=2))
    else:
        print("COUNTEREXAMPLE REPRODUCED" if reproduced else "COUNTEREXAMPLE NOT REPRODUCED")
        for mismatch in mismatches:
            print(f"{mismatch['name']}: {mismatch['original']} != {mismatch['candidate']}")
    return 0 if reproduced else 4


def _add_check_options(
    parser: argparse.ArgumentParser,
    *,
    base_original_default: int | None = 0,
    base_candidate_default: int | None = 0,
    default_contract: str | None = None,
) -> None:
    parser.add_argument(
        "--base-original",
        type=_parse_int,
        default=base_original_default,
        help="decode base for original (default: 0, or ELF symbol address for check-objects)",
    )
    parser.add_argument(
        "--base-candidate",
        type=_parse_int,
        default=base_candidate_default,
        help="decode base for candidate (default: 0, or ELF symbol address for check-objects)",
    )
    contract = parser.add_mutually_exclusive_group(required=default_contract is None)
    contract.add_argument(
        "--contract",
        choices=CONTRACT_PRESETS,
        help="named observable-state contract",
    )
    if default_contract is not None:
        parser.set_defaults(default_contract=default_contract)
    contract.add_argument(
        "--observe",
        action="append",
        help=(
            "manual observables: r0..r31, f0..f31[.ps1], gqr0..gqr7, "
            "sr0..sr15, cr/cr0..cr7, fpscr, xer.ca/ov/so, lr, ctr, msr, "
            "time_base, srr0, srr1, named SPRs, memory; comma-separated or repeated"
        ),
    )
    parser.add_argument("--timeout-ms", type=int, default=0, help="solver timeout in ms (0 = auto-scale: 20ms/insn, floor 5s, ceiling 120s)")
    parser.add_argument("--max-instructions", type=int, default=2048, help="per-path execution bound")
    parser.add_argument("--max-paths", type=int, default=256, help="symbolic path bound")
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--result", type=Path, help="write the complete JSON proof result")
    parser.add_argument("--replay-out", type=Path, help="write a replayable counterexample when inequivalent")
    parser.add_argument("--smt-out", type=Path, help="write the generated SMT-LIB query")
    parser.add_argument(
        "--memory-profile",
        choices=PROFILE_CHOICES,
        default=None,
        help="memory-environment profile (default: assumed-ordinary-ram)",
    )
    parser.add_argument(
        "--memory-ranges",
        nargs="*",
        default=[],
        help="valid RAM ranges for bounded profiles: low,high hex pairs",
    )


def cmd_extract(args: argparse.Namespace) -> int:
    if args.list:
        functions = list_text_functions(args.object)
        payload = [
            {
                "name": item.name,
                "size": item.size,
                "value": f"0x{item.value:x}",
                "base": f"0x{item.base:08x}",
                "section": item.section_name,
                "relocations": len(item.relocations),
            }
            for item in functions
        ]
        if args.json:
            print(json.dumps(payload, indent=2))
        else:
            for item in payload:
                print(
                    f"{item['value']}  {item['size']:6}  {item['name']}"
                    f"  relocations={item['relocations']}"
                )
        return 0

    if not args.symbol:
        raise ValueError("--symbol is required unless --list is set")
    function = extract_function(args.object, args.symbol)
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_bytes(function.code)
    payload = {
        "name": function.name,
        "path": str(function.path),
        "size": function.size,
        "value": f"0x{function.value:x}",
        "base": f"0x{function.base:08x}",
        "section": function.section_name,
        "relocations": [
            {
                "offset": f"0x{item.offset:x}",
                "type": item.relocation_type,
                "symbol": item.symbol,
                "addend": item.addend,
            }
            for item in function.relocations
        ],
        "hex": _format_hex(function.code),
    }
    if args.json:
        print(json.dumps(payload, indent=2))
    else:
        print(f"{function.name}: {function.size} bytes @ 0x{function.base:08x}")
        print(payload["hex"])
        if args.out:
            print(f"wrote: {args.out}")
    return 0


def cmd_check_objects(args: argparse.Namespace) -> int:
    left, right = extract_function_pair(
        args.original,
        args.candidate,
        args.symbol,
        candidate_symbol=args.candidate_symbol,
    )
    if not args.json:
        print(
            f"original:  {left.name}  {left.size} bytes @ 0x{left.base:08x}  ({left.path})"
        )
        print(
            f"candidate: {right.name}  {right.size} bytes @ 0x{right.base:08x}  ({right.path})"
        )
    base_original = left.base if args.base_original is None else args.base_original
    base_candidate = right.base if args.base_candidate is None else args.base_candidate
    return _run_check(
        args,
        left.code,
        right.code,
        base_original=base_original,
        base_candidate=base_candidate,
        original_relocations=left.relocations,
        candidate_relocations=right.relocations,
        original_symbol=left.name,
        candidate_symbol=right.name,
    )


def cmd_differential(args: argparse.Namespace) -> int:
    from tools.ppc_equivalence.fixtures.corpus import FIXTURES, load_fixtures
    from tools.ppc_equivalence.fixtures.runner import compare_fixture
    from tools.ppc_equivalence.gen_fixture_blob import DEFAULT_HEADER, generate_header

    cases = load_fixtures(args.fixtures) if args.fixtures else FIXTURES
    if args.id:
        cases = tuple(case for case in cases if case.id == args.id or case.id.startswith(args.id.rstrip("*")))
        if not cases:
            raise ValueError(f"no fixtures matched id {args.id!r}")

    failures: list[dict[str, Any]] = []
    for case in cases:
        mismatches = compare_fixture(case)
        if mismatches:
            failures.append({"id": case.id, "mismatches": mismatches})

    header_stale = DEFAULT_HEADER.read_text(encoding="utf-8") != generate_header() if DEFAULT_HEADER.is_file() else True
    payload = {
        "format": 1,
        "checked": len(cases),
        "failed": len(failures),
        "failures": failures,
        "header_stale": header_stale,
    }

    dolphin_status = None
    if args.dolphin:
        import subprocess
        from pathlib import Path as _Path

        root = _Path(__file__).resolve().parents[2]
        proc = subprocess.run(
            [
                sys.executable,
                str(root / "tools" / "test" / "compare_behaviour" / "run.py"),
                "ppc",
                "ppc-equivalence-fixtures",
            ],
            cwd=root,
            capture_output=True,
            text=True,
            check=False,
        )
        dolphin_status = {
            "exit_code": proc.returncode,
            "stdout": proc.stdout,
            "stderr": proc.stderr,
        }
        payload["dolphin"] = {"exit_code": proc.returncode}

    if args.json:
        print(json.dumps(payload, indent=2, sort_keys=True))
    else:
        print(f"concrete fixtures: {len(cases) - len(failures)}/{len(cases)} passed")
        for failure in failures:
            print(f"  FAIL {failure['id']}: {', '.join(failure['mismatches'])}")
        if header_stale:
            print("warning: ppc_fixture_cases.inc is stale; run tools/ppc_equivalence/gen_fixture_blob.py")
        if dolphin_status is not None:
            print(f"dolphin exit: {dolphin_status['exit_code']}")
            text = (dolphin_status["stdout"] or "") + (dolphin_status["stderr"] or "")
            if text.strip():
                print(text.strip())

    if failures or header_stale:
        return 1
    if dolphin_status is not None and dolphin_status["exit_code"] not in (0,):
        # Return Dolphin's code when concrete fixtures passed; SKIP often uses 0.
        return dolphin_status["exit_code"] if dolphin_status["exit_code"] else 1
    return 0


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Xenoblade Wii Broadway PPC32 equivalence checker")
    parser.add_argument("--version", action="version", version=__version__)
    sub = parser.add_subparsers(dest="command", required=True)

    decode = sub.add_parser("decode", help="decode and normalize PPC32 big-endian words")
    source = decode.add_mutually_exclusive_group(required=True)
    source.add_argument("--hex")
    source.add_argument("--file", type=Path)
    decode.add_argument("--base", type=_parse_int, default=0)
    decode.add_argument("--json", action="store_true")

    check_hex = sub.add_parser("check-hex", help="prove or refute two hexadecimal blocks")
    check_hex.add_argument("--original", required=True)
    check_hex.add_argument("--candidate", required=True)
    _add_check_options(check_hex)

    check = sub.add_parser("check", help="prove or refute two raw binary blocks")
    check.add_argument("--original", type=Path, required=True)
    check.add_argument("--candidate", type=Path, required=True)
    _add_check_options(check)

    check_objects = sub.add_parser(
        "check-objects",
        help="prove or refute a named function from two ELF32 BE object files",
    )
    check_objects.add_argument("--original", type=Path, required=True, help="retail / target .o")
    check_objects.add_argument("--candidate", type=Path, required=True, help="decomp / base .o")
    check_objects.add_argument("--symbol", required=True, help="mangled symbol name (or unique substring)")
    check_objects.add_argument(
        "--candidate-symbol",
        help="candidate symbol when names differ (default: same as --symbol)",
    )
    _add_check_options(
        check_objects,
        base_original_default=None,
        base_candidate_default=None,
        default_contract="auto",
    )

    extract = sub.add_parser("extract", help="extract named .text bytes from an ELF32 BE object")
    extract.add_argument("--object", type=Path, required=True)
    extract.add_argument("--symbol", help="mangled symbol name (or unique substring)")
    extract.add_argument("--list", action="store_true", help="list sized .text symbols")
    extract.add_argument("--out", type=Path, help="write raw instruction bytes")
    extract.add_argument("--json", action="store_true")

    replay = sub.add_parser("replay", help="replay an inequivalence counterexample concretely")
    replay.add_argument("case", type=Path)
    replay.add_argument("--json", action="store_true")

    differential = sub.add_parser(
        "differential",
        help="run the shared Broadway fixture corpus against ConcreteOps (+ optional Dolphin)",
    )
    differential.add_argument("--fixtures", type=Path, help="optional JSONL override (default: in-repo corpus)")
    differential.add_argument("--id", help="run a single fixture id (or prefix*)")
    differential.add_argument(
        "--dolphin",
        action="store_true",
        help="also build/run the ppc-equivalence-fixtures Dolphin DOL",
    )
    differential.add_argument("--json", action="store_true")
    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    try:
        if args.command == "decode":
            return cmd_decode(args)
        if args.command == "check-hex":
            return _run_check(args, parse_hex(args.original), parse_hex(args.candidate))
        if args.command == "check":
            return _run_check(args, args.original.read_bytes(), args.candidate.read_bytes())
        if args.command == "check-objects":
            return cmd_check_objects(args)
        if args.command == "extract":
            return cmd_extract(args)
        if args.command == "replay":
            return cmd_replay(args)
        if args.command == "differential":
            return cmd_differential(args)
    except (DecodeError, ElfSymbolError, ValueError, OSError, json.JSONDecodeError) as exc:
        if getattr(args, "json", False):
            print(json.dumps({"format": 1, "status": "invalid_input", "error": str(exc)}))
        else:
            print(f"ERROR: {exc}", file=sys.stderr)
        return 3
    except RuntimeError as exc:
        if getattr(args, "json", False):
            print(json.dumps({"format": 1, "status": "internal_error", "error": str(exc)}))
        else:
            print(f"ERROR: {exc}", file=sys.stderr)
        return 4
    parser.error(f"unknown command: {args.command}")
    return 3

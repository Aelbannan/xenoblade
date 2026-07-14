from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any

from . import __version__
from .contract import CONTRACT_PRESETS, Observable, make_contract, parse_observables
from .decoder import decode_block, parse_hex
from .engine import check_equivalence
from .ir import DecodeError, ExecutionInconclusive, UnsupportedInstruction
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
    if observable.kind == "cr_field":
        assert observable.index is not None
        return f"0x{(int(state.cr) >> ((7 - observable.index) * 4)) & 0xF:x}"
    if observable.kind in ("cr", "lr", "ctr"):
        return f"0x{int(getattr(state, observable.kind)) & 0xFFFFFFFF:08x}"
    if observable.kind == "xer":
        return int(bool(getattr(state.xer, observable.name.split(".", 1)[1])))
    if observable.kind == "memory":
        return state.memory
    raise AssertionError(observable.kind)


def _print_result(result: ProofResult) -> None:
    if result.status == ProofStatus.EQUIVALENT:
        print("EQUIVALENT UNDER CONTRACT")
    elif result.status == ProofStatus.NOT_EQUIVALENT:
        print("NOT EQUIVALENT")
    else:
        print("INCONCLUSIVE")
    print(f"status: {result.status.value}")
    print(f"contract: {result.contract}")
    if result.observables:
        print(f"compared: {', '.join(result.observables)}")
    if result.mismatch:
        print(
            "first observable mismatch: "
            f"{result.mismatch['name']} "
            f"({result.mismatch['original']} != {result.mismatch['candidate']})"
        )
    if result.counterexample:
        registers = result.counterexample["initial_state"]["gpr"]
        if registers:
            print("counterexample inputs:")
            for name, value in registers.items():
                print(f"  {name} = {value}")
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


def _run_check(args: argparse.Namespace, original_code: bytes, candidate_code: bytes) -> int:
    original_hex = _format_hex(original_code)
    candidate_hex = _format_hex(candidate_code)
    try:
        original = decode_block(original_code, args.base_original)
        candidate = decode_block(candidate_code, args.base_candidate)
        live_out = None
        if args.contract == "live-out":
            live_out = tuple(dict.fromkeys(automatic_live_out(original) + automatic_live_out(candidate)))
        contract = make_contract(
            preset=args.contract,
            observe=args.observe,
            timeout_ms=args.timeout_ms,
            live_out=live_out,
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
        )
    except (UnsupportedInstruction, ExecutionInconclusive) as exc:
        contract_name = args.contract or "manual"
        observable_names = list(args.observe or ())
        result = ProofResult(
            status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            contract=contract_name,
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
    original_final, candidate_final = original_exits[0].state, candidate_exits[0].state
    mismatches = []
    for observable in observables:
        left = _observable_concrete(original_final, observable)
        right = _observable_concrete(candidate_final, observable)
        if left != right:
            mismatches.append({"name": observable.name, "original": left, "candidate": right})
    payload = {"reproduced": bool(mismatches), "mismatches": mismatches}
    if args.json:
        print(json.dumps(payload, indent=2))
    else:
        print("COUNTEREXAMPLE REPRODUCED" if mismatches else "COUNTEREXAMPLE NOT REPRODUCED")
        for mismatch in mismatches:
            print(f"{mismatch['name']}: {mismatch['original']} != {mismatch['candidate']}")
    return 0 if mismatches else 4


def _add_check_options(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--base-original", type=_parse_int, default=0)
    parser.add_argument("--base-candidate", type=_parse_int, default=0)
    contract = parser.add_mutually_exclusive_group(required=True)
    contract.add_argument(
        "--contract",
        choices=CONTRACT_PRESETS,
        help="named observable-state contract",
    )
    contract.add_argument(
        "--observe",
        action="append",
        help="manual observables: r0..r31, cr/cr0..cr7, xer.ca/ov/so, lr, ctr, memory; comma-separated or repeated",
    )
    parser.add_argument("--timeout-ms", type=int, default=10_000)
    parser.add_argument("--max-instructions", type=int, default=512, help="per-path execution bound")
    parser.add_argument("--max-paths", type=int, default=128, help="symbolic path bound")
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--result", type=Path, help="write the complete JSON proof result")
    parser.add_argument("--replay-out", type=Path, help="write a replayable counterexample when inequivalent")
    parser.add_argument("--smt-out", type=Path, help="write the generated SMT-LIB query")


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

    replay = sub.add_parser("replay", help="replay an inequivalence counterexample concretely")
    replay.add_argument("case", type=Path)
    replay.add_argument("--json", action="store_true")
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
        if args.command == "replay":
            return cmd_replay(args)
    except (DecodeError, ValueError, OSError, json.JSONDecodeError) as exc:
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

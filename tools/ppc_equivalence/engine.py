from __future__ import annotations

import time
from pathlib import Path
from typing import Any

from .contract import EquivalenceContract, Observable
from .ir import Instruction
from .model import MachineState, XerState
from .result import ProofResult, ProofStatus
from .semantics import SymbolicOps, Terminal, execute_cfg, read_gprs


def _symbolic_initial(ops: SymbolicOps) -> MachineState:
    z3 = ops.z3
    return MachineState(
        tuple(z3.BitVec(f"input.gpr.r{i}", 32) for i in range(32)),
        tuple(z3.BitVec(f"input.fpr.f{i}", 64) for i in range(32)),
        tuple(z3.BitVec(f"input.ps1.f{i}", 64) for i in range(32)),
        tuple(z3.BitVec(f"input.gqr.gqr{i}", 32) for i in range(8)),
        z3.BitVec("input.cr", 32),
        XerState(z3.Bool("input.xer.ca"), z3.Bool("input.xer.ov"), z3.Bool("input.xer.so")),
        z3.BitVec("input.fpscr", 32),
        z3.BitVec("input.lr", 32),
        z3.BitVec("input.ctr", 32),
        z3.Array("input.memory", z3.BitVecSort(32), z3.BitVecSort(8)),
        z3.BoolVal(True),
    )


def _observable_value(state: MachineState, observable: Observable, ops: SymbolicOps) -> Any:
    if observable.kind == "gpr":
        assert observable.index is not None
        return state.gpr[observable.index]
    if observable.kind == "fpr":
        assert observable.index is not None
        return state.fpr[observable.index]
    if observable.kind == "ps1":
        assert observable.index is not None
        return state.ps1[observable.index]
    if observable.kind == "gqr":
        assert observable.index is not None
        return state.gqr[observable.index]
    if observable.kind == "cr_field":
        assert observable.index is not None
        shift = (7 - observable.index) * 4
        return ops.z3.Extract(shift + 3, shift, state.cr)
    if observable.kind in ("cr", "lr", "ctr", "memory"):
        return getattr(state, observable.kind)
    if observable.kind == "fpscr":
        return state.fpscr
    if observable.kind == "xer":
        return getattr(state.xer, observable.name.split(".", 1)[1])
    raise AssertionError(f"unknown observable kind: {observable.kind}")


def _terminal_difference(left: Terminal, right: Terminal, contract: EquivalenceContract, ops: SymbolicOps) -> Any:
    z3 = ops.z3
    kind_difference = z3.BoolVal(left.exit_kind != right.exit_kind)
    target_difference = z3.BoolVal(False)
    if left.exit_kind != "fallthrough" and right.exit_kind != "fallthrough":
        if left.exit_target is None or right.exit_target is None:
            target_difference = z3.BoolVal(left.exit_target is not right.exit_target)
        else:
            target_difference = left.exit_target != right.exit_target
    values = [
        _observable_value(left.state, item, ops) != _observable_value(right.state, item, ops)
        for item in contract.observables
    ]
    valid_difference = left.state.valid != right.state.valid
    defined_value_difference = z3.And(left.state.valid, right.state.valid, z3.Or(*values))
    return z3.And(
        left.condition,
        right.condition,
        z3.Or(kind_difference, target_difference, valid_difference, defined_value_difference),
    )


def _hex_value(model: Any, expression: Any) -> str:
    value = model.eval(expression, model_completion=True)
    return f"0x{value.as_long() & 0xFFFFFFFF:08x}"


def _hex_value64(model: Any, expression: Any) -> str:
    value = model.eval(expression, model_completion=True)
    return f"0x{value.as_long() & 0xFFFFFFFFFFFFFFFF:016x}"


def _bool_value(model: Any, expression: Any, z3: Any) -> int:
    return 1 if z3.is_true(model.eval(expression, model_completion=True)) else 0


def _memory_entries(model: Any, memory: Any, touches: tuple[Any, ...], z3: Any) -> dict[str, object]:
    """Extract the finite stores selected by Z3; unspecified bytes use its array default."""
    value = model.eval(memory, model_completion=True)
    entries: dict[str, str] = {}
    try:
        items = value.as_list()
    except (AttributeError, TypeError):
        items = []
    for item in items[:-1]:
        if len(item) == 2:
            address, byte = item
            entries[f"0x{address.as_long() & 0xFFFFFFFF:08x}"] = f"0x{byte.as_long() & 0xFF:02x}"
    default = items[-1].as_long() & 0xFF if items and not isinstance(items[-1], list) else 0
    for expression in touches:
        address = model.eval(expression, model_completion=True).as_long() & 0xFFFFFFFF
        byte = model.eval(z3.Select(memory, expression), model_completion=True).as_long() & 0xFF
        if byte == default:
            entries.pop(f"0x{address:08x}", None)
        else:
            entries[f"0x{address:08x}"] = f"0x{byte:02x}"
    return {"default": f"0x{default:02x}", "bytes": entries}


def check_equivalence(
    original: list[Instruction],
    candidate: list[Instruction],
    contract: EquivalenceContract,
    *,
    original_hex: str,
    candidate_hex: str,
    smt_output: str | None = None,
    max_instructions: int = 512,
    max_paths: int = 128,
) -> ProofResult:
    ops = SymbolicOps()
    z3 = ops.z3
    initial = _symbolic_initial(ops)
    original_exits = execute_cfg(initial, original, ops, max_instructions=max_instructions, max_paths=max_paths)
    candidate_exits = execute_cfg(initial, candidate, ops, max_instructions=max_instructions, max_paths=max_paths)
    pair_differences = [
        _terminal_difference(left, right, contract, ops)
        for left in original_exits
        for right in candidate_exits
    ]
    solver = z3.Solver()
    solver.set(timeout=contract.timeout_ms)
    solver.add(z3.Or(*pair_differences))
    if smt_output is not None:
        Path(smt_output).write_text(solver.to_smt2(), encoding="utf-8")
    started = time.perf_counter()
    answer = solver.check()
    elapsed_ms = round((time.perf_counter() - started) * 1000, 3)
    result = ProofResult(
        status=ProofStatus.INCONCLUSIVE_UNKNOWN,
        contract=contract.name,
        observables=[item.name for item in contract.observables],
        original_instruction_count=len(original),
        candidate_instruction_count=len(candidate),
        solver={
            "name": "z3", "version": z3.get_version_string(), "result": str(answer),
            "elapsed_ms": elapsed_ms, "timeout_ms": contract.timeout_ms,
            "original_paths": len(original_exits), "candidate_paths": len(candidate_exits),
        },
    )
    if answer == z3.unsat:
        result.status = ProofStatus.EQUIVALENT
        return result
    if answer == z3.unknown:
        reason = solver.reason_unknown()
        result.status = ProofStatus.INCONCLUSIVE_TIMEOUT if "timeout" in reason.lower() else ProofStatus.INCONCLUSIVE_UNKNOWN
        result.warnings.append(reason)
        return result

    result.status = ProofStatus.NOT_EQUIVALENT
    model = solver.model()
    selected: tuple[Terminal, Terminal] | None = None
    for left in original_exits:
        for right in candidate_exits:
            if z3.is_true(model.eval(_terminal_difference(left, right, contract, ops), model_completion=True)):
                selected = (left, right)
                break
        if selected:
            break
    assert selected is not None
    left_exit, right_exit = selected
    mismatch_observable: Observable | None = None
    if left_exit.exit_kind != right_exit.exit_kind:
        result.mismatch = {"kind": "exit", "name": "exit.kind", "original": left_exit.exit_kind, "candidate": right_exit.exit_kind}
    elif left_exit.exit_kind != "fallthrough" and left_exit.exit_target is not None and right_exit.exit_target is not None and z3.is_true(model.eval(left_exit.exit_target != right_exit.exit_target, model_completion=True)):
        result.mismatch = {"kind": "exit", "name": "exit.target", "original": _hex_value(model, left_exit.exit_target), "candidate": _hex_value(model, right_exit.exit_target)}
    elif z3.is_true(model.eval(left_exit.state.valid != right_exit.state.valid, model_completion=True)):
        result.mismatch = {"kind": "definedness", "name": "defined-domain", "original": _bool_value(model, left_exit.state.valid, z3), "candidate": _bool_value(model, right_exit.state.valid, z3)}
    else:
        for observable in contract.observables:
            left = _observable_value(left_exit.state, observable, ops)
            right = _observable_value(right_exit.state, observable, ops)
            if z3.is_true(model.eval(left != right, model_completion=True)):
                mismatch_observable = observable
                if observable.kind == "xer":
                    left_value: object = _bool_value(model, left, z3)
                    right_value: object = _bool_value(model, right, z3)
                elif observable.kind == "memory":
                    left_value, right_value = "different final arrays", "different final arrays"
                elif observable.kind in ("fpr", "ps1", "fpscr"):
                    left_value, right_value = _hex_value64(model, left), _hex_value64(model, right)
                else:
                    left_value, right_value = _hex_value(model, left), _hex_value(model, right)
                result.mismatch = {"kind": observable.kind, "name": observable.name, "original": left_value, "candidate": right_value}
                break

    relevant_gprs = sorted(read_gprs(original) | read_gprs(candidate))
    initial_state = {
        "gpr": {f"r{i}": _hex_value(model, initial.gpr[i]) for i in relevant_gprs},
        "fpr": {f"f{i}": _hex_value64(model, initial.fpr[i]) for i in range(32)},
        "ps1": {f"f{i}": _hex_value64(model, initial.ps1[i]) for i in range(32)},
        "gqr": {f"gqr{i}": _hex_value(model, initial.gqr[i]) for i in range(8)},
        "cr": _hex_value(model, initial.cr),
        "fpscr": _hex_value(model, initial.fpscr),
        "xer": {name: _bool_value(model, getattr(initial.xer, name), z3) for name in ("ca", "ov", "so")},
        "lr": _hex_value(model, initial.lr),
        "ctr": _hex_value(model, initial.ctr),
        "memory": _memory_entries(model, initial.memory, left_exit.state.memory_touches + right_exit.state.memory_touches, z3),
    }
    result.counterexample = {"initial_state": initial_state}
    result.replay = {
        "format": 2, "architecture": "broadway-ppc32-be-v10", "contract": contract.name,
        "original_hex": original_hex, "candidate_hex": candidate_hex,
        "base_original": original[0].address, "base_candidate": candidate[0].address,
        "observables": [item.name for item in contract.observables], "initial_state": initial_state,
        "expected_mismatch": result.mismatch["name"] if result.mismatch else (mismatch_observable.name if mismatch_observable else None),
    }
    return result

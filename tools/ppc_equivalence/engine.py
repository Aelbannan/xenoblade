from __future__ import annotations

import time
from typing import Any

from .contract import EquivalenceContract, Observable
from .ir import Instruction
from .model import MachineState, XerState
from .result import ProofResult, ProofStatus
from .semantics import SymbolicOps, execute_block, read_gprs


def _symbolic_initial(ops: SymbolicOps) -> MachineState:
    z3 = ops.z3
    return MachineState(
        tuple(z3.BitVec(f"input.gpr.r{i}", 32) for i in range(32)),
        z3.BitVec("input.cr", 32),
        XerState(z3.Bool("input.xer.ca"), z3.Bool("input.xer.ov"), z3.Bool("input.xer.so")),
        z3.BitVec("input.lr", 32),
        z3.BitVec("input.ctr", 32),
    )


def _observable_value(state: MachineState, observable: Observable, ops: SymbolicOps) -> Any:
    if observable.kind == "gpr":
        assert observable.index is not None
        return state.gpr[observable.index]
    if observable.kind == "cr_field":
        assert observable.index is not None
        shift = (7 - observable.index) * 4
        return ops.z3.Extract(shift + 3, shift, state.cr)
    if observable.kind == "cr":
        return state.cr
    if observable.kind == "lr":
        return state.lr
    if observable.kind == "ctr":
        return state.ctr
    if observable.kind == "xer":
        return getattr(state.xer, observable.name.split(".", 1)[1])
    raise AssertionError(f"unknown observable kind: {observable.kind}")


def _hex_value(model: Any, expression: Any) -> str:
    value = model.eval(expression, model_completion=True)
    return f"0x{value.as_long() & 0xFFFFFFFF:08x}"


def _bool_value(model: Any, expression: Any, z3: Any) -> int:
    return 1 if z3.is_true(model.eval(expression, model_completion=True)) else 0


def check_equivalence(
    original: list[Instruction],
    candidate: list[Instruction],
    contract: EquivalenceContract,
    *,
    original_hex: str,
    candidate_hex: str,
    smt_output: str | None = None,
) -> ProofResult:
    ops = SymbolicOps()
    z3 = ops.z3
    initial = _symbolic_initial(ops)
    original_final = execute_block(initial, original, ops)
    candidate_final = execute_block(initial, candidate, ops)
    differences = [
        _observable_value(original_final, observable, ops)
        != _observable_value(candidate_final, observable, ops)
        for observable in contract.observables
    ]
    solver = z3.Solver()
    solver.set(timeout=contract.timeout_ms)
    solver.add(z3.Or(*differences))
    if smt_output is not None:
        from pathlib import Path

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
            "name": "z3",
            "version": z3.get_version_string(),
            "result": str(answer),
            "elapsed_ms": elapsed_ms,
            "timeout_ms": contract.timeout_ms,
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
    mismatch_observable: Observable | None = None
    for observable in contract.observables:
        left = _observable_value(original_final, observable, ops)
        right = _observable_value(candidate_final, observable, ops)
        if z3.is_true(model.eval(left != right, model_completion=True)):
            mismatch_observable = observable
            if observable.kind == "xer":
                left_value: object = _bool_value(model, left, z3)
                right_value: object = _bool_value(model, right, z3)
            else:
                left_value = _hex_value(model, left)
                right_value = _hex_value(model, right)
            result.mismatch = {
                "kind": observable.kind,
                "name": observable.name,
                "original": left_value,
                "candidate": right_value,
            }
            break

    relevant_gprs = sorted(read_gprs(original) | read_gprs(candidate))
    initial_gpr = {f"r{index}": _hex_value(model, initial.gpr[index]) for index in relevant_gprs}
    initial_state = {
        "gpr": initial_gpr,
        "cr": _hex_value(model, initial.cr),
        "xer": {
            "ca": _bool_value(model, initial.xer.ca, z3),
            "ov": _bool_value(model, initial.xer.ov, z3),
            "so": _bool_value(model, initial.xer.so, z3),
        },
        "lr": _hex_value(model, initial.lr),
        "ctr": _hex_value(model, initial.ctr),
    }
    result.counterexample = {"initial_state": initial_state}
    result.replay = {
        "format": 1,
        "architecture": "ppc32-be-v1",
        "contract": contract.name,
        "original_hex": original_hex,
        "candidate_hex": candidate_hex,
        "observables": [item.name for item in contract.observables],
        "initial_state": initial_state,
        "expected_mismatch": mismatch_observable.name if mismatch_observable else None,
    }
    return result

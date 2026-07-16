from __future__ import annotations

import random
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
from typing import Any

from .contract import EquivalenceContract, Observable
from .ir import Instruction, Opcode
from .model import ConcreteMemory, MachineState, XerState, concrete_state
from .result import ARCHITECTURE_MODEL, RESULT_FORMAT, ProofResult, ProofStatus
from .semantics import (
    ConcreteOps,
    SymbolicOps,
    Terminal,
    execute_block,
    execute_cfg,
    execute_instruction,
    read_gprs,
)
from .spr import AUX_SPR_OBSERVABLES


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
        z3.BitVec("input.msr", 32),
        tuple(z3.BitVec(f"input.sr.sr{i}", 32) for i in range(16)),
        z3.BitVec("input.time_base", 64),
        z3.BitVec("input.srr0", 32),
        z3.BitVec("input.srr1", 32),
        tuple(z3.BitVec(f"input.spr.{name}", 32) for name in AUX_SPR_OBSERVABLES),
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
    if observable.kind == "sr":
        assert observable.index is not None
        return state.sr[observable.index]
    if observable.kind == "spr":
        assert observable.index is not None
        return state.spr[observable.index]
    if observable.kind == "cr_field":
        assert observable.index is not None
        shift = (7 - observable.index) * 4
        return ops.z3.Extract(shift + 3, shift, state.cr)
    if observable.kind in ("cr", "lr", "ctr", "msr", "time_base", "srr0", "srr1", "memory"):
        return getattr(state, observable.kind)
    if observable.kind == "fpscr":
        return state.fpscr
    if observable.kind == "xer":
        return getattr(state.xer, observable.name.split(".", 1)[1])
    raise AssertionError(f"unknown observable kind: {observable.kind}")


def _observable_concrete(state: MachineState, observable: Observable) -> object:
    if observable.kind == "gpr":
        assert observable.index is not None
        return int(state.gpr[observable.index]) & 0xFFFFFFFF
    if observable.kind == "fpr":
        assert observable.index is not None
        return int(state.fpr[observable.index]) & 0xFFFFFFFFFFFFFFFF
    if observable.kind == "ps1":
        assert observable.index is not None
        return int(state.ps1[observable.index]) & 0xFFFFFFFFFFFFFFFF
    if observable.kind == "gqr":
        assert observable.index is not None
        return int(state.gqr[observable.index]) & 0xFFFFFFFF
    if observable.kind == "sr":
        assert observable.index is not None
        return int(state.sr[observable.index]) & 0xFFFFFFFF
    if observable.kind == "spr":
        assert observable.index is not None
        return int(state.spr[observable.index]) & 0xFFFFFFFF
    if observable.kind == "cr_field":
        assert observable.index is not None
        return (int(state.cr) >> ((7 - observable.index) * 4)) & 0xF
    if observable.kind in ("cr", "lr", "ctr", "msr", "srr0", "srr1"):
        return int(getattr(state, observable.kind)) & 0xFFFFFFFF
    if observable.kind == "time_base":
        return int(state.time_base) & 0xFFFFFFFFFFFFFFFF
    if observable.kind == "fpscr":
        return int(state.fpscr) & 0xFFFFFFFF
    if observable.kind == "xer":
        return int(bool(getattr(state.xer, observable.name.split(".", 1)[1])))
    if observable.kind == "memory":
        return state.memory
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


_RANDOM_CHECK_SAMPLES = 5


def _random_concrete_check(
    original: list[Instruction],
    candidate: list[Instruction],
    contract: EquivalenceContract,
) -> bool:
    ops = ConcreteOps()
    for _ in range(_RANDOM_CHECK_SAMPLES):
        rng = random.Random()
        initial = MachineState(
            tuple(rng.getrandbits(32) for _ in range(32)),
            tuple(rng.getrandbits(64) for _ in range(32)),
            tuple(rng.getrandbits(64) for _ in range(32)),
            tuple(rng.getrandbits(32) for _ in range(8)),
            rng.getrandbits(32),
            XerState(rng.choice([True, False]), rng.choice([True, False]), rng.choice([True, False])),
            rng.getrandbits(32),
            rng.getrandbits(32),
            rng.getrandbits(32),
            rng.getrandbits(32),
            tuple(rng.getrandbits(32) for _ in range(16)),
            rng.getrandbits(64),
            rng.getrandbits(32),
            rng.getrandbits(32),
            tuple(rng.getrandbits(32) for _ in range(66)),
            ConcreteMemory(),
            True,
        )
        try:
            orig_state = execute_block(initial, original, ops, max_instructions=len(original) + 256)
            cand_state = execute_block(initial, candidate, ops, max_instructions=len(candidate) + 256)
        except Exception:
            continue
        if not bool(orig_state.valid) and not bool(cand_state.valid):
            continue
        if bool(orig_state.valid) != bool(cand_state.valid):
            return True
        for observable in contract.observables:
            try:
                o_val = _observable_concrete(orig_state, observable)
                c_val = _observable_concrete(cand_state, observable)
            except Exception:
                continue
            if o_val != c_val:
                return True
    return False


def _instruction_level_diff(
    original: list[Instruction],
    candidate: list[Instruction],
    initial_state: dict[str, Any],
) -> dict[str, Any] | None:
    try:
        from .model import concrete_state as _cs
        state = _cs(initial_state)
    except Exception:
        return None
    ops = ConcreteOps()
    orig_state = state
    cand_state = state
    min_len = min(len(original), len(candidate))
    for i in range(min_len):
        o_insn, c_insn = original[i], candidate[i]
        try:
            orig_state = execute_instruction(orig_state, o_insn, ops)
            cand_state = execute_instruction(cand_state, c_insn, ops)
        except Exception:
            return {"instruction_index": i, "detail": "execution error during instruction-level diff"}
        for reg_idx in range(32):
            if orig_state.gpr[reg_idx] != cand_state.gpr[reg_idx]:
                return {
                    "instruction_index": i,
                    "diverged_register": f"r{reg_idx}",
                    "original_value": f"0x{int(orig_state.gpr[reg_idx]) & 0xFFFFFFFF:08x}",
                    "candidate_value": f"0x{int(cand_state.gpr[reg_idx]) & 0xFFFFFFFF:08x}",
                    "original_insn": f"{o_insn.opcode.value} {o_insn.operands}",
                    "candidate_insn": f"{c_insn.opcode.value} {c_insn.operands}",
                }
        for reg_idx in range(32):
            if orig_state.fpr[reg_idx] != cand_state.fpr[reg_idx]:
                return {
                    "instruction_index": i,
                    "diverged_register": f"f{reg_idx}",
                    "original_value": f"0x{int(orig_state.fpr[reg_idx]) & 0xFFFFFFFFFFFFFFFF:016x}",
                    "candidate_value": f"0x{int(cand_state.fpr[reg_idx]) & 0xFFFFFFFFFFFFFFFF:016x}",
                    "original_insn": f"{o_insn.opcode.value} {o_insn.operands}",
                    "candidate_insn": f"{c_insn.opcode.value} {c_insn.operands}",
                }
    return None


def _run_with_tactics(z3_module: Any, build_solver, timeout_ms: int) -> tuple[Any, Any, float, str]:
    solver = build_solver()
    solver.set(timeout=timeout_ms)
    started = time.perf_counter()
    try:
        answer = solver.check()
        elapsed = round((time.perf_counter() - started) * 1000, 3)

        try:
            tactic_solver = build_solver()
            t = z3_module.Then("simplify", "bit-blast", "sat")
            t_start = time.perf_counter()
            try:
                bit_solver = t(tactic_solver)
                bit_answer = bit_solver.check()
                if bit_answer == z3_module.sat or bit_answer == z3_module.unsat:
                    return bit_solver, bit_answer, round((time.perf_counter() - t_start) * 1000, 3), "bit-blast"
            except Exception:
                pass
        except Exception:
            pass

        return solver, answer, elapsed, "default"
    except Exception:
        elapsed = round((time.perf_counter() - started) * 1000, 3)
        return solver, z3_module.unknown, elapsed, "default"


def check_equivalence(
    original: list[Instruction],
    candidate: list[Instruction],
    contract: EquivalenceContract,
    *,
    original_hex: str,
    candidate_hex: str,
    smt_output: str | None = None,
    max_instructions: int = 2048,
    max_paths: int = 256,
    assumed_callees: frozenset[int] = frozenset(),
    assumed_callees_used: set[int] | None = None,
) -> ProofResult:
    ops = SymbolicOps()
    z3 = ops.z3
    initial = _symbolic_initial(ops)
    callees_used: set[int] = set()
    original_exits = execute_cfg(
        initial, original, ops,
        max_instructions=max_instructions, max_paths=max_paths,
        assumed_callees=assumed_callees, assumed_callees_used=callees_used,
    )
    candidate_exits = execute_cfg(
        initial, candidate, ops,
        max_instructions=max_instructions, max_paths=max_paths,
        assumed_callees=assumed_callees, assumed_callees_used=callees_used,
    )
    if assumed_callees_used is not None:
        assumed_callees_used.update(callees_used)

    pair_differences = [
        _terminal_difference(left, right, contract, ops)
        for left in original_exits
        for right in candidate_exits
    ]

    def _build_solver() -> Any:
        s = z3.Solver()
        s.add(z3.Or(*pair_differences))
        return s

    solver = _build_solver()
    if smt_output is not None:
        Path(smt_output).write_text(solver.to_smt2(), encoding="utf-8")

    winning_solver, answer, elapsed_ms, tactic = _run_with_tactics(
        z3, _build_solver, contract.timeout_ms,
    )

    result = ProofResult(
        status=ProofStatus.INCONCLUSIVE_UNKNOWN,
        contract=contract.name,
        contract_resolution=contract.resolution_dict(),
        observables=[item.name for item in contract.observables],
        original_instruction_count=len(original),
        candidate_instruction_count=len(candidate),
        solver={
            "name": "z3", "version": z3.get_version_string(), "result": str(answer),
            "elapsed_ms": elapsed_ms, "timeout_ms": contract.timeout_ms,
            "original_paths": len(original_exits), "candidate_paths": len(candidate_exits),
            "tactic": tactic,
        },
        assumed_callees=sorted(callees_used),
    )
    if answer == z3.unsat:
        result.status = ProofStatus.EQUIVALENT
        return result
    if answer == z3.unknown:
        reason = winning_solver.reason_unknown()
        result.status = ProofStatus.INCONCLUSIVE_TIMEOUT if "timeout" in reason.lower() else ProofStatus.INCONCLUSIVE_UNKNOWN
        result.warnings.append(reason)
        return result

    result.status = ProofStatus.NOT_EQUIVALENT
    model = winning_solver.model()
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
                elif observable.kind in ("fpr", "ps1", "time_base"):
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
        "msr": _hex_value(model, initial.msr),
        "sr": {f"sr{i}": _hex_value(model, initial.sr[i]) for i in range(16)},
        "time_base": _hex_value64(model, initial.time_base),
        "srr0": _hex_value(model, initial.srr0),
        "srr1": _hex_value(model, initial.srr1),
        "spr": {
            name: _hex_value(model, initial.spr[index])
            for index, name in enumerate(AUX_SPR_OBSERVABLES)
        },
        "memory": _memory_entries(model, initial.memory, left_exit.state.memory_touches + right_exit.state.memory_touches, z3),
    }
    result.counterexample = {"initial_state": initial_state}

    repair_hint = _instruction_level_diff(original, candidate, initial_state)
    if repair_hint:
        result.repair_hint = repair_hint

    result.replay = {
        "format": RESULT_FORMAT, "architecture": ARCHITECTURE_MODEL,
        "contract": contract.name, "contract_resolution": contract.resolution_dict(),
        "original_hex": original_hex, "candidate_hex": candidate_hex,
        "base_original": original[0].address, "base_candidate": candidate[0].address,
        "observables": [item.name for item in contract.observables], "initial_state": initial_state,
        "expected_mismatch": result.mismatch["name"] if result.mismatch else (mismatch_observable.name if mismatch_observable else None),
    }
    return result

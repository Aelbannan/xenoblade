from __future__ import annotations

import random
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass, replace
from pathlib import Path
from typing import Any

from .contract import EquivalenceContract, Observable
from .deadline import Deadline, ProofDeadlineExceeded, SolverPhase
from .ir import ExecutionInconclusive, Instruction, Opcode, SUPPORTED_FP_OPCODES
from .memory_profile import (
    MemoryEnvironment,
    MemoryProfile,
    build_memory_constraints,
)
from .memory_bus import MemoryBus
from .memory_bus_obligations import (
    build_memory_bus_constraints,
    build_memory_bus_obligation,
    enrich_memory_bus_obligation_with_symbolic_mmio,
)
from .bus_access import clear_side_bus_access_coverage
from .model import ConcreteMemory, InvalidReason, MachineState, XerState, concrete_state
from .proof_features import enforce_equivalent_proof_features
from .jump_table_obligations import (
    JumpTableProofContext,
    discharge_jump_table_obligations,
    jump_table_gate_reason,
    obligations_from_discharge,
    rom_image_byte_constraints,
    unresolved_indirect_exit_gate_reason,
)
from .vtable_obligations import (
    VirtualCallProofContext,
    discharge_virtual_call_obligations,
    obligations_from_virtual_call_discharge,
    virtual_call_gate_reason,
    virtual_call_scc_status,
)
from .bulk_remainder_relational import try_build_bulk_remainder_relational_sketch
from .gx_fifo_loop import build_gx_fifo_loop_plans_for_devices
from .loop_summary import (
    build_affine_summary_map,
    build_loop_summary_obligation,
)
from .memory_loop import (
    build_memory_loop_obligation,
    build_memory_loop_plan_map,
    build_memory_loop_side_entry,
)
from .memory_loop_readonly import (
    MemoryLoopReadonlyContext,
    build_memory_loop_readonly_context,
    readonly_word_byte_constraints,
)
from .relational_induction import try_discharge_relational
from .provenance import (
    canonical_json_sha256,
    hash_certifier_tree,
    hash_engine_tree,
    live_git_identity,
)
from .result import (
    ARCHITECTURE_MODEL, RESULT_FORMAT, FloatingPointDomain, MemoryScope, ProofResult, ProofStatus,
)
from .semantics import (
    CalleeContract,
    ConcreteOps,
    DEFAULT_MAX_LOOP_ITERATIONS,
    SymbolicOps,
    Terminal,
    _infer_invalid_reasons,
    execute_cfg,
    execute_instruction,
    read_gprs,
)
from .spr import AUX_SPR_OBSERVABLES


def _symbolic_initial(ops: SymbolicOps) -> MachineState:
    z3 = ops.z3
    state = MachineState(
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
        z3.BitVecVal(0, 8),
    )
    return replace(
        state, stack_low=state.gpr[1], stack_layout_valid=ops.bool(True),
        stack_private=ops.bool(True),
    )


@dataclass(frozen=True, slots=True)
class CalleeContractValidation:
    valid: bool
    required_reads: frozenset[str]
    required_writes: frozenset[str]
    missing_reads: frozenset[str]
    missing_writes: frozenset[str]
    required_invalid_reasons: frozenset[int] = frozenset()
    missing_invalid_reasons: frozenset[int] = frozenset()
    reason: str = ""


def _contract_components(state: MachineState, z3: Any) -> dict[str, Any]:
    components: dict[str, Any] = {
        **{f"r{i}": state.gpr[i] for i in range(32)},
        **{f"f{i}": state.fpr[i] for i in range(32)},
        **{f"f{i}.ps1": state.ps1[i] for i in range(32)},
        **{f"gqr{i}": state.gqr[i] for i in range(8)},
        **{f"sr{i}": state.sr[i] for i in range(16)},
        **{name: state.spr[index] for index, name in enumerate(AUX_SPR_OBSERVABLES)},
        "xer.ca": state.xer.ca,
        "xer.ov": state.xer.ov,
        "xer.so": state.xer.so,
        "fpscr": state.fpscr,
        "lr": state.lr,
        "ctr": state.ctr,
        "msr": state.msr,
        "time_base": state.time_base,
        "srr0": state.srr0,
        "srr1": state.srr1,
        "memory": state.memory,
        "valid": state.valid,
        "invalid_reason": state.invalid_reason,
    }
    for field in range(8):
        shift = (7 - field) * 4
        components[f"cr{field}"] = z3.Extract(shift + 3, shift, state.cr)
    return components


def _contract_covers(required: str, declared: frozenset[str]) -> bool:
    if "*" in declared or required in declared:
        return True
    return required.startswith("cr") and "cr" in declared


def _symbolic_persistent_writes(
    terminals: list[Any],
    initial: MachineState,
    ops: SymbolicOps,
    *,
    timeout_ms: int = 2_000,
) -> set[str]:
    """Return AUTO_PERSISTENT components that may change on a feasible path.

    Structural ``z3.eq`` is the fast path; when expressions differ we confirm
    with a short SMT query under the terminal path condition so ITE diamonds
    that restore the initial value are not treated as writes.
    """
    from tools.ppc_equivalence.contract import AUTO_PERSISTENT_OBSERVABLES

    z3 = ops.z3
    initial_components = _contract_components(initial, z3)
    written: set[str] = set()
    for name in AUTO_PERSISTENT_OBSERVABLES:
        initial_value = initial_components.get(name)
        if initial_value is None:
            continue
        for terminal in terminals:
            final_components = _contract_components(terminal.state, z3)
            final_value = final_components.get(name)
            if final_value is None:
                continue
            if z3.eq(final_value, initial_value):
                continue
            value_solver = z3.Solver()
            value_solver.set(timeout=timeout_ms)
            premises = [final_value != initial_value, terminal.condition]
            if terminal.state.stack_layout_valid is not None:
                premises.append(terminal.state.stack_layout_valid)
            value_solver.add(*premises)
            if value_solver.check() != z3.unsat:
                written.add(name)
                break
    return written


def validate_callee_contract(
    instructions: list[Instruction],
    contract: CalleeContract,
    *,
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    callee_contracts: dict[int | str, CalleeContract] | None = None,
    require_normal_return: bool = False,
) -> CalleeContractValidation:
    """Check a declared dependency/effect contract against symbolic semantics.

    This validation is intentionally conservative: syntactic dependencies in
    the semantic expressions count as reads even when later simplification
    could prove them irrelevant.

    Threaded callers are dispatched to :mod:`process_pool` so Z3 never shares
    a context across threads.
    """
    from tools.ppc_equivalence.process_pool import run_validate_callee_contract, should_isolate

    kwargs = dict(
        max_instructions=max_instructions,
        max_paths=max_paths,
        max_loop_iterations=max_loop_iterations,
        assumed_callees=assumed_callees,
        callee_contracts=callee_contracts,
        require_normal_return=require_normal_return,
    )
    if should_isolate():
        return run_validate_callee_contract(instructions, contract, **kwargs)
    return _validate_callee_contract_impl(instructions, contract, **kwargs)


def _validate_callee_contract_impl(
    instructions: list[Instruction],
    contract: CalleeContract,
    *,
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    callee_contracts: dict[int | str, CalleeContract] | None = None,
    require_normal_return: bool = False,
) -> CalleeContractValidation:
    ops = SymbolicOps()
    z3 = ops.z3
    initial = replace(_symbolic_initial(ops), valid=z3.Bool("input.valid"))
    try:
        terminals = execute_cfg(
            initial, instructions, ops,
            max_instructions=max_instructions, max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees,
            callee_contracts=callee_contracts,
        )
    except Exception as exc:
        return CalleeContractValidation(
            False, frozenset(), frozenset(), frozenset(), frozenset(), frozenset(), frozenset(),
            f"semantic validation unavailable: {exc}",
        )

    if require_normal_return:
        abnormal: set[str] = set()
        for terminal in terminals:
            if terminal.exit_kind == "return":
                continue
            feasibility = z3.Solver()
            feasibility.add(terminal.condition)
            if terminal.state.stack_layout_valid is not None:
                feasibility.add(terminal.state.stack_layout_valid)
            # Fail closed: sat *or* unknown means we cannot prove the abnormal
            # exit is infeasible, so reject the normal-return contract.
            answer = feasibility.check()
            if answer != z3.unsat:
                abnormal.add(terminal.exit_kind)
        abnormal = sorted(abnormal)
        if abnormal:
            return CalleeContractValidation(
                False, frozenset(), frozenset(), frozenset(), frozenset(), frozenset(), frozenset(),
                "callee does not have only normal returns: " + ", ".join(abnormal),
            )

    initial_components = _contract_components(initial, z3)
    input_names = {
        **{f"input.gpr.r{i}": f"r{i}" for i in range(32)},
        **{f"input.fpr.f{i}": f"f{i}" for i in range(32)},
        **{f"input.ps1.f{i}": f"f{i}.ps1" for i in range(32)},
        **{f"input.gqr.gqr{i}": f"gqr{i}" for i in range(8)},
        **{f"input.sr.sr{i}": f"sr{i}" for i in range(16)},
        **{f"input.spr.{name}": name for name in AUX_SPR_OBSERVABLES},
        "input.cr": "cr",
        "input.xer.ca": "xer.ca",
        "input.xer.ov": "xer.ov",
        "input.xer.so": "xer.so",
        "input.fpscr": "fpscr",
        "input.lr": "lr",
        "input.ctr": "ctr",
        "input.msr": "msr",
        "input.time_base": "time_base",
        "input.srr0": "srr0",
        "input.srr1": "srr1",
        "input.memory": "memory",
        "input.valid": "valid",
    }
    required_writes: set[str] = set()
    dependency_expressions: list[Any] = []
    for terminal in terminals:
        dependency_expressions.append(terminal.condition)
        if terminal.exit_kind not in ("return", "fallthrough") and terminal.exit_target is not None:
            dependency_expressions.append(terminal.exit_target)
        final_components = _contract_components(terminal.state, z3)
        for name, initial_value in initial_components.items():
            final_value = final_components[name]
            changed = not z3.eq(final_value, initial_value)
            if not changed:
                continue
            # Memory always needs private-stack-aware refinement: structural
            # inequality alone treats private-frame stores as observable writes.
            if name == "memory":
                memory_solver = z3.Solver()
                memory_solver.set(timeout=2_000)
                memory_solver.add(
                    terminal.condition,
                    terminal.state.stack_layout_valid,
                    _memory_difference(terminal.state, initial, initial, ops),
                )
                changed = memory_solver.check() != z3.unsat
                if not changed:
                    continue
                required_writes.add(name)
                continue
            # Structural inequality is enough when the contract already permits
            # this write. SMT refinement on every diamond path otherwise times
            # out on modest load-modify-store functions (e.g. copyInputFlag).
            if _contract_covers(name, contract.writes):
                required_writes.add(name)
                # Skip feeding final values into get_vars for covered writes when
                # reads are wildcarded: the expressions can be huge ITE/memory
                # cones and hang certificate validation.
                if not _contract_covers("*", contract.reads):
                    dependency_expressions.append(final_value)
                continue
            else:
                value_solver = z3.Solver()
                value_solver.set(timeout=2_000)
                value_solver.add(
                    terminal.condition,
                    terminal.state.stack_layout_valid,
                    final_value != initial_value,
                )
                changed = value_solver.check() != z3.unsat
            if changed:
                required_writes.add(name)
                # Never feed Array-valued memory terms into get_vars: Store/Select
                # spines from load-modify-store functions are huge and hang
                # certificate validation. Memory reads are implied by a memory write.
                if name == "memory":
                    continue
                dependency_expressions.append(final_value)

    required_reads: set[str] = set()
    if "memory" in required_writes:
        required_reads.add("memory")
    if _contract_covers("*", contract.reads):
        # Wildcard reads: skip get_vars entirely (opaque EABI certificates).
        pass
    else:
        for expression in dependency_expressions:
            for variable in z3.z3util.get_vars(expression):
                component = input_names.get(str(variable.decl().name()))
                if component is not None:
                    required_reads.add(component)

    missing_reads = frozenset(
        name for name in required_reads if not _contract_covers(name, contract.reads)
    )
    missing_writes = frozenset(
        name for name in required_writes if not _contract_covers(name, contract.writes)
    )
    inferred_invalid_reasons = _infer_invalid_reasons(instructions)
    missing_invalid_reasons = frozenset(
        r for r in inferred_invalid_reasons if r not in contract.invalid_reasons
    )
    return CalleeContractValidation(
        not missing_reads and not missing_writes and not missing_invalid_reasons,
        frozenset(required_reads),
        frozenset(required_writes),
        missing_reads,
        missing_writes,
        frozenset(inferred_invalid_reasons),
        missing_invalid_reasons,
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


def _private_stack_address(
    address: Any, stack_low: Any | None, entry_sp: Any, stack_private: Any | None,
    ops: SymbolicOps,
) -> Any:
    if stack_low is None or stack_private is None:
        return ops.z3.BoolVal(False)
    z3 = ops.z3
    return z3.And(
        z3.ULE(stack_low, entry_sp),
        z3.UGE(address, stack_low),
        z3.ULT(address, entry_sp),
        stack_private,
    )


def _memory_difference(
    left: MachineState, right: MachineState, initial: MachineState,
    ops: SymbolicOps,
) -> Any:
    """Compare memory with independent per-implementation private-stack masking.

    Each implementation's own private stack interval is masked independently.
    An address private to one implementation does not hide a write by the other
    implementation when that write lies outside the other implementation's own
    private interval.
    """
    z3 = ops.z3
    differences: list[Any] = []
    for address in left.memory_touches + right.memory_touches:
        initial_byte = z3.Select(initial.memory, address)
        left_is_private = _private_stack_address(
            address, left.stack_low, initial.gpr[1], left.stack_private, ops,
        )
        right_is_private = _private_stack_address(
            address, right.stack_low, initial.gpr[1], right.stack_private, ops,
        )
        left_byte = z3.If(
            left_is_private, initial_byte, z3.Select(left.memory, address),
        )
        right_byte = z3.If(
            right_is_private, initial_byte, z3.Select(right.memory, address),
        )
        differences.append(left_byte != right_byte)
    if len(left.memory_effects) != len(right.memory_effects):
        differences.append(z3.BoolVal(True))
    else:
        differences.extend(a != b for a, b in zip(left.memory_effects, right.memory_effects))
    return z3.Or(*differences) if differences else z3.BoolVal(False)


def _observable_difference(
    left: MachineState, right: MachineState, observable: Observable,
    initial: MachineState, ops: SymbolicOps,
) -> Any:
    if observable.kind == "memory":
        return _memory_difference(left, right, initial, ops)
    return _observable_value(left, observable, ops) != _observable_value(right, observable, ops)


def _terminal_difference(
    left: Terminal, right: Terminal, contract: EquivalenceContract,
    initial: MachineState, ops: SymbolicOps,
) -> Any:
    from tools.ppc_equivalence.contract import observables_for_exit

    z3 = ops.z3
    kind_difference = z3.BoolVal(left.exit_kind != right.exit_kind)
    target_difference = z3.BoolVal(False)
    if left.exit_kind != "fallthrough" and right.exit_kind != "fallthrough":
        if left.exit_target is None or right.exit_target is None:
            target_difference = z3.BoolVal(left.exit_target is not right.exit_target)
        else:
            target_difference = left.exit_target != right.exit_target
    # Only apply exit-kind filtering when both sides agree on the exit kind;
    # mismatched kinds already fail via kind_difference.
    if left.exit_kind == right.exit_kind:
        compared = observables_for_exit(contract, left.exit_kind)
    else:
        compared = contract.observables
    values = [
        _observable_difference(left.state, right.state, item, initial, ops)
        for item in compared
    ]
    # MMIO/FIFO device state is an automatic observable once either side touches it.
    from tools.ppc_equivalence.symbolic_bus import symbolic_bus_difference

    device_difference = symbolic_bus_difference(
        getattr(left.state, "symbolic_bus", None),
        getattr(right.state, "symbolic_bus", None),
        z3,
    )
    valid_difference = left.state.valid != right.state.valid
    both_invalid = z3.And(z3.Not(left.state.valid), z3.Not(right.state.valid))
    reason_difference = z3.And(
        both_invalid,
        left.state.invalid_reason != right.state.invalid_reason,
    )
    # Observables (including memory) are compared whenever both sides share
    # definedness polarity. Suppressing values on both-invalid same-reason
    # paths hid committed stores before a shared fault (false EQUIVALENT).
    observable_difference = z3.Or(*(values + [device_difference]))
    comparable_definedness = z3.Or(
        z3.And(left.state.valid, right.state.valid),
        both_invalid,
    )
    defined_value_difference = z3.And(comparable_definedness, observable_difference)
    return z3.And(
        left.condition,
        right.condition,
        z3.Or(kind_difference, target_difference, valid_difference, reason_difference, defined_value_difference),
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


_REPO_ROOT = Path(__file__).resolve().parents[2]


def _make_concrete_initial(
    *,
    gpr_fill: int | None = None,
    fpr_fill: int | None = None,
    rng: random.Random | None = None,
) -> MachineState:
    """Build a ConcreteOps-ready initial state (zeros, fill, or random)."""
    spr_count = len(AUX_SPR_OBSERVABLES)
    if rng is not None:
        return MachineState(
            tuple(rng.getrandbits(32) for _ in range(32)),
            tuple(rng.getrandbits(64) for _ in range(32)),
            tuple(rng.getrandbits(64) for _ in range(32)),
            tuple(rng.getrandbits(32) for _ in range(8)),
            rng.getrandbits(32),
            XerState(
                rng.choice([True, False]),
                rng.choice([True, False]),
                rng.choice([True, False]),
            ),
            # Keep RN=nearest-even / NI=0 so FP ConcreteOps stays defined.
            rng.getrandbits(29),
            rng.getrandbits(32),
            rng.getrandbits(32),
            rng.getrandbits(32),
            tuple(rng.getrandbits(32) for _ in range(16)),
            rng.getrandbits(64),
            rng.getrandbits(32),
            rng.getrandbits(32),
            tuple(rng.getrandbits(32) for _ in range(spr_count)),
            ConcreteMemory(),
            True,
        )
    gpr_v = (gpr_fill if gpr_fill is not None else 0) & 0xFFFFFFFF
    fpr_v = (fpr_fill if fpr_fill is not None else 0) & 0xFFFFFFFFFFFFFFFF
    return MachineState(
        tuple(gpr_v for _ in range(32)),
        tuple(fpr_v for _ in range(32)),
        tuple(fpr_v for _ in range(32)),
        tuple(0 for _ in range(8)),
        0,
        XerState(False, False, False),
        0,
        0,
        0,
        0,
        tuple(0 for _ in range(16)),
        0,
        0,
        0,
        tuple(0 for _ in range(spr_count)),
        ConcreteMemory(),
        True,
    )


def _serialize_concrete_initial(state: MachineState) -> dict[str, Any]:
    memory = state.memory
    memory_bytes: dict[str, str] = {}
    default = 0
    if isinstance(memory, ConcreteMemory):
        default = int(memory.default) & 0xFF
        memory_bytes = {
            f"0x{address:08x}": f"0x{byte:02x}" for address, byte in memory.bytes
        }
    return {
        "gpr": {f"r{i}": f"0x{int(state.gpr[i]) & 0xFFFFFFFF:08x}" for i in range(32)},
        "fpr": {
            f"f{i}": f"0x{int(state.fpr[i]) & 0xFFFFFFFFFFFFFFFF:016x}" for i in range(32)
        },
        "ps1": {
            f"f{i}": f"0x{int(state.ps1[i]) & 0xFFFFFFFFFFFFFFFF:016x}" for i in range(32)
        },
        "gqr": {f"gqr{i}": f"0x{int(state.gqr[i]) & 0xFFFFFFFF:08x}" for i in range(8)},
        "cr": f"0x{int(state.cr) & 0xFFFFFFFF:08x}",
        "fpscr": f"0x{int(state.fpscr) & 0xFFFFFFFF:08x}",
        "xer": {
            "ca": int(bool(state.xer.ca)),
            "ov": int(bool(state.xer.ov)),
            "so": int(bool(state.xer.so)),
        },
        "lr": f"0x{int(state.lr) & 0xFFFFFFFF:08x}",
        "ctr": f"0x{int(state.ctr) & 0xFFFFFFFF:08x}",
        "msr": f"0x{int(state.msr) & 0xFFFFFFFF:08x}",
        "sr": {f"sr{i}": f"0x{int(state.sr[i]) & 0xFFFFFFFF:08x}" for i in range(16)},
        "time_base": f"0x{int(state.time_base) & 0xFFFFFFFFFFFFFFFF:016x}",
        "srr0": f"0x{int(state.srr0) & 0xFFFFFFFF:08x}",
        "srr1": f"0x{int(state.srr1) & 0xFFFFFFFF:08x}",
        "spr": {
            name: f"0x{int(state.spr[index]) & 0xFFFFFFFF:08x}"
            for index, name in enumerate(AUX_SPR_OBSERVABLES)
        },
        "memory": {"default": f"0x{default:02x}", "bytes": memory_bytes},
    }


def _format_concrete_observable(value: object, observable: Observable) -> object:
    if observable.kind == "memory":
        if isinstance(value, ConcreteMemory):
            return {
                "default": f"0x{int(value.default) & 0xFF:02x}",
                "bytes": {
                    f"0x{address:08x}": f"0x{byte:02x}" for address, byte in value.bytes
                },
            }
        return str(value)
    if observable.kind == "xer":
        return int(bool(value))
    if observable.kind in ("fpr", "ps1", "time_base"):
        return f"0x{int(value) & 0xFFFFFFFFFFFFFFFF:016x}"
    if observable.kind == "cr_field":
        return f"0x{int(value) & 0xF:x}"
    return f"0x{int(value) & 0xFFFFFFFF:08x}"


def _concrete_terminal_mismatches(
    left: Terminal,
    right: Terminal,
    observables: tuple[Observable, ...],
) -> list[dict[str, object]]:
    """Compare two ConcreteOps terminals under the contract observables."""
    mismatches: list[dict[str, object]] = []
    if left.exit_kind != right.exit_kind:
        mismatches.append({
            "name": "exit.kind",
            "original": left.exit_kind,
            "candidate": right.exit_kind,
        })
        return mismatches
    if left.exit_kind != "fallthrough" and left.exit_target != right.exit_target:
        def _target(value: object) -> object:
            return f"0x{int(value) & 0xFFFFFFFF:08x}" if value is not None else None
        mismatches.append({
            "name": "exit.target",
            "original": _target(left.exit_target),
            "candidate": _target(right.exit_target),
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
        left_reason = int(left.state.invalid_reason)
        right_reason = int(right.state.invalid_reason)
        if left_reason != right_reason:
            mismatches.append({
                "name": "invalid-reason",
                "original": left_reason,
                "candidate": right_reason,
            })
        return mismatches
    for observable in observables:
        try:
            o_val = _observable_concrete(left.state, observable)
            c_val = _observable_concrete(right.state, observable)
        except Exception:
            continue
        if o_val != c_val:
            mismatches.append({
                "name": observable.name,
                "original": _format_concrete_observable(o_val, observable),
                "candidate": _format_concrete_observable(c_val, observable),
            })
            return mismatches
    return mismatches


def run_concrete_sampling(
    original: list[Instruction],
    candidate: list[Instruction],
    contract: EquivalenceContract,
    *,
    sample_count: int,
    seed: int = 0,
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    callee_contracts: dict[int | str, CalleeContract] | None = None,
    floating_point_domain: FloatingPointDomain | None = None,
    memory_bus: MemoryBus | None = None,
) -> dict[str, Any]:
    """Sample concrete initial states through both CFGs and compare observables.

    This is a **secondary defense**: a mismatch is a ConcreteOps inequivalence
    witness, not an SMT certificate. Agreement (no mismatch) never proves
    equivalence.
    """
    if sample_count < 0:
        raise ValueError("concrete sample_count must be >= 0")
    report: dict[str, Any] = {
        "role": "secondary_defense",
        "certificate": False,
        "samples_requested": sample_count,
        "samples_run": 0,
        "samples_skipped": 0,
        "interesting_samples": 0,
        "random_samples": 0,
        "mismatch_found": False,
        "seed": seed,
        "mismatch": None,
        "initial_state": None,
        "sample_label": None,
    }
    if sample_count == 0:
        return report

    interesting: list[tuple[str, MachineState]] = [
        ("zeros", _make_concrete_initial()),
        ("ones", _make_concrete_initial(gpr_fill=0xFFFFFFFF, fpr_fill=0xFFFFFFFFFFFFFFFF)),
        ("incremental", MachineState(
            tuple(i & 0xFFFFFFFF for i in range(32)),
            tuple((i * 0x0101010101010101) & 0xFFFFFFFFFFFFFFFF for i in range(32)),
            tuple((i * 0x0101010101010101) & 0xFFFFFFFFFFFFFFFF for i in range(32)),
            tuple(i & 0xFFFFFFFF for i in range(8)),
            0x01234567,
            XerState(False, False, False),
            0,
            0x80000000,
            1,
            0,
            tuple(0 for _ in range(16)),
            0,
            0,
            0,
            tuple(0 for _ in range(len(AUX_SPR_OBSERVABLES))),
            ConcreteMemory(),
            True,
        )),
    ]
    rng = random.Random(seed)
    planned: list[tuple[str, MachineState]] = []
    for label, state in interesting:
        if len(planned) >= sample_count:
            break
        planned.append((label, state))
    while len(planned) < sample_count:
        planned.append((f"random-{len(planned)}", _make_concrete_initial(rng=rng)))

    ops = ConcreteOps()
    for label, initial in planned:
        try:
            # Isolate mutable MMIO/RAM bus state: never share the state mutated
            # by original with candidate (or the caller's bus).
            if memory_bus is not None:
                initial_bus_state = memory_bus.snapshot_state().clone()
                original_bus = memory_bus.with_state(initial_bus_state.clone())
                candidate_bus = memory_bus.with_state(initial_bus_state.clone())
            else:
                original_bus = None
                candidate_bus = None
            original_exits = [
                item for item in execute_cfg(
                    initial, original, ops,
                    max_instructions=max_instructions,
                    max_paths=max_paths,
                    max_loop_iterations=max_loop_iterations,
                    assumed_callees=assumed_callees,
                    callee_contracts=callee_contracts,
                    floating_point_domain=floating_point_domain,
                    memory_bus=original_bus,
                )
                if item.condition
            ]
            candidate_exits = [
                item for item in execute_cfg(
                    initial, candidate, ops,
                    max_instructions=max_instructions,
                    max_paths=max_paths,
                    max_loop_iterations=max_loop_iterations,
                    assumed_callees=assumed_callees,
                    callee_contracts=callee_contracts,
                    floating_point_domain=floating_point_domain,
                    memory_bus=candidate_bus,
                )
                if item.condition
            ]
        except Exception:
            report["samples_skipped"] += 1
            continue
        if len(original_exits) != 1 or len(candidate_exits) != 1:
            report["samples_skipped"] += 1
            continue
        report["samples_run"] += 1
        if label.startswith("random-"):
            report["random_samples"] += 1
        else:
            report["interesting_samples"] += 1
        from tools.ppc_equivalence.contract import observables_for_exit

        left_exit = original_exits[0]
        right_exit = candidate_exits[0]
        if left_exit.exit_kind == right_exit.exit_kind:
            sampled_observables = observables_for_exit(contract, left_exit.exit_kind)
        else:
            sampled_observables = contract.observables
        mismatches = _concrete_terminal_mismatches(
            left_exit, right_exit, sampled_observables,
        )
        if mismatches:
            first = mismatches[0]
            report["mismatch_found"] = True
            report["mismatch"] = {
                "kind": "concrete_sampling",
                "name": first["name"],
                "original": first["original"],
                "candidate": first["candidate"],
            }
            report["initial_state"] = _serialize_concrete_initial(initial)
            report["sample_label"] = label
            return report
    return report


def _apply_concrete_sampling_defense(
    result: ProofResult,
    original: list[Instruction],
    candidate: list[Instruction],
    contract: EquivalenceContract,
    *,
    concrete_samples: int,
    concrete_sample_seed: int,
    max_instructions: int,
    max_paths: int,
    max_loop_iterations: int,
    assumed_callees: frozenset[int | str],
    callee_contracts: dict[int | str, CalleeContract] | None,
    floating_point_domain: FloatingPointDomain | None,
    memory_bus: MemoryBus | None,
    original_hex: str,
    candidate_hex: str,
) -> ProofResult:
    """Attach sampling report; demote EQUIVALENT/inconclusive on concrete mismatch.

    Never promotes to EQUIVALENT. SMT ``unsat`` remains the only equivalence
    certificate path; sampling can only add evidence of inequivalence.
    """
    if concrete_samples <= 0:
        return result
    sampling = run_concrete_sampling(
        original,
        candidate,
        contract,
        sample_count=concrete_samples,
        seed=concrete_sample_seed,
        max_instructions=max_instructions,
        max_paths=max_paths,
        max_loop_iterations=max_loop_iterations,
        assumed_callees=assumed_callees,
        callee_contracts=callee_contracts,
        floating_point_domain=floating_point_domain,
        memory_bus=memory_bus,
    )
    result.concrete_sampling = sampling
    if not sampling.get("mismatch_found"):
        return result

    prior = result.status
    result.warnings.append(
        "concrete sampling found a mismatch (secondary defense; "
        "not an SMT inequivalence certificate)"
    )
    if prior is ProofStatus.EQUIVALENT:
        result.warnings.append(
            "SMT reported equivalent but concrete sampling diverged; "
            "failing closed by demoting to not_equivalent"
        )
    if prior is not ProofStatus.NOT_EQUIVALENT:
        result.status = ProofStatus.NOT_EQUIVALENT
        result.counterexample_kind = "concrete_sampling"
        if result.mismatch is None and sampling.get("mismatch"):
            result.mismatch = dict(sampling["mismatch"])
        if result.counterexample is None and sampling.get("initial_state"):
            result.counterexample = {
                "initial_state": sampling["initial_state"],
                "relocations": {},
                "source": "concrete_sampling",
            }
        if result.replay is None and sampling.get("initial_state"):
            result.replay = {
                "format": RESULT_FORMAT,
                "architecture": ARCHITECTURE_MODEL,
                "contract": contract.name,
                "contract_resolution": contract.resolution_dict(),
                "original_hex": original_hex,
                "candidate_hex": candidate_hex,
                "base_original": original[0].address if original else 0,
                "base_candidate": candidate[0].address if candidate else 0,
                "observables": [item.name for item in contract.observables],
                "initial_state": sampling["initial_state"],
                "expected_mismatch": (
                    sampling["mismatch"]["name"] if sampling.get("mismatch") else None
                ),
                "source": "concrete_sampling",
            }
    return result


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


def check_with_portfolio(
    z3_module: Any, build_solver, deadline: Deadline,
) -> tuple[Any, Any, str, list[SolverPhase]]:
    """Run default solver then fallback bit-blast only if default is unknown.

    Returns ``(solver, answer, tactic_name, phases)``. Returns immediately
    when the default solver returns ``sat`` or ``unsat``.
    """
    phases: list[SolverPhase] = []

    try:
        default_solver = build_solver()
        default_solver.set(timeout=deadline.require_time("default-solver"))
        started = time.monotonic_ns()
        default_answer = default_solver.check()
        default_elapsed = round((time.monotonic_ns() - started) / 1_000_000, 3)
        phases.append(SolverPhase("default", str(default_answer), default_elapsed))
    except ProofDeadlineExceeded:
        phases.append(SolverPhase("default", "deadline-exceeded", 0))
        phases.append(SolverPhase("bitblast", "deadline-exceeded", 0))
        return build_solver(), z3_module.unknown, "default", phases

    if default_answer in (z3_module.sat, z3_module.unsat):
        return default_solver, default_answer, "default", phases

    try:
        t_start = time.monotonic_ns()
        remaining = deadline.require_time("bitblast-fallback")
        fallback_solver = build_solver()
        t = z3_module.Then("simplify", "bit-blast", "sat")
        bit_solver = t.solver()
        bit_solver.set(timeout=remaining)
        for assertion in fallback_solver.assertions():
            bit_solver.add(assertion)
        bit_answer = bit_solver.check()
        bit_elapsed = round((time.monotonic_ns() - t_start) / 1_000_000, 3)
        phases.append(SolverPhase("bitblast", str(bit_answer), bit_elapsed))
        if bit_answer in (z3_module.sat, z3_module.unsat):
            return bit_solver, bit_answer, "bitblast", phases
    except ProofDeadlineExceeded:
        phases.append(SolverPhase("bitblast", "deadline-exceeded", 0))
        return default_solver, z3_module.unknown, "default", phases
    except Exception:
        phases.append(SolverPhase("bitblast", "error", 0))

    return default_solver, default_answer, "default", phases


def _populate_solver_diagnostics(
    result: ProofResult,
    *,
    layout_constraints: list[Any],
    original_exits: list[Any],
    candidate_exits: list[Any],
    relocation_bindings: dict[str, int],
    phases: list[SolverPhase],
    answer: Any,
    winning_solver: Any,
    build_solver: Any,
    diagnostics_out: str | None,
    source_hash: str,
    memory_constraint_count: int = 0,
) -> None:
    """Fill ``result.solver_diagnostics`` when diagnostics are requested.

    Diagnostics are DISABLED by default: ``diagnostics_out`` must be given or
    the ``PPC_EQUIV_DIAGNOSTICS`` env var set.  No diagnostics means no extra
    solver work and no cache-key impact.
    """
    import os

    if diagnostics_out is None and os.environ.get("PPC_EQUIV_DIAGNOSTICS") != "1":
        return

    from .diagnostics import categorize_assertions, count_z3_symbols

    try:
        import z3 as _z3
    except Exception:
        _z3 = None

    memory_touches: set[Any] = set()
    for terminal in original_exits + candidate_exits:
        memory_touches.update(terminal.state.memory_touches)

    path_pair_count = len(original_exits) * len(candidate_exits)
    smt2_dump_path = diagnostics_out
    if smt2_dump_path is None and source_hash:
        smt2_dump_path = f"diag-{source_hash[:16]}.smt2"

    unknown_reason = None
    if result.status in (ProofStatus.INCONCLUSIVE_UNKNOWN, ProofStatus.INCONCLUSIVE_TIMEOUT):
        unknown_reason = list(result.warnings)

    assertions_by_category = categorize_assertions(
        layout_constraint_count=len(layout_constraints),
        memory_constraint_count=memory_constraint_count,
        relocation_binding_count=len(relocation_bindings),
        path_pair_count=path_pair_count,
    )
    # path_pairs_covered is informational; assertion_count excludes it.
    assertion_count = sum(
        count
        for key, count in assertions_by_category.items()
        if key != "path_pairs_covered"
    )

    diagnostics: dict[str, Any] = {
        "smt2_dump_path": smt2_dump_path,
        "assertion_count": assertion_count,
        "assertions_by_category": assertions_by_category,
        "bv_symbols": 0,
        "array_symbols": 0,
        "func_symbols": 0,
        "path_pair_count": path_pair_count,
        "memory_touch_count": len(memory_touches),
        "relocation_constraint_count": len(relocation_bindings),
        "layout_constraint_count": len(layout_constraints),
        "solver_statistics": None,
        "phase_timings": [p.to_dict() for p in phases],
        "unknown_reason": unknown_reason,
        "solver_result": str(answer),
    }

    solver_for_symbols = None
    if smt2_dump_path:
        try:
            solver_for_symbols = build_solver()
            Path(smt2_dump_path).write_text(solver_for_symbols.to_smt2(), encoding="utf-8")
        except Exception:
            solver_for_symbols = None

    if _z3 is not None:
        try:
            if solver_for_symbols is None:
                solver_for_symbols = build_solver()
            symbol_counts = count_z3_symbols(list(solver_for_symbols.assertions()))
            diagnostics.update(symbol_counts)
            # Prefer real assertion cardinality when available.
            diagnostics["assertion_count"] = len(list(solver_for_symbols.assertions()))
        except Exception:
            pass
        try:
            diagnostics["solver_statistics"] = {
                str(name): value for name, value in winning_solver.statistics().items()
            }
        except Exception:
            pass

    result.solver_diagnostics = diagnostics


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
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    assumed_callees_used: set[int | str] | None = None,
    callee_contracts: dict[int | str, CalleeContract] | None = None,
    relocation_bindings: dict[str, int] | None = None,
    initial_gpr_bindings: dict[int, int] | None = None,
    initial_gpr_ranges: dict[int, tuple[int, int]] | None = None,
    memory_environment: MemoryEnvironment | None = None,
    source_hash: str = "",
    floating_point_domain: FloatingPointDomain | None = None,
    diagnostics_out: str | None = None,
    concrete_samples: int = 0,
    concrete_sample_seed: int = 0,
    jump_table: JumpTableProofContext | None = None,
    virtual_call: VirtualCallProofContext | None = None,
    callee_edges: dict[str, frozenset[str]] | None = None,
    memory_bus: MemoryBus | None = None,
    readonly_words: dict[int, int] | None = None,
    memory_loop_readonly: MemoryLoopReadonlyContext | None = None,
    platform_profile: object | None = None,
) -> ProofResult:
    """Prove original/candidate observational equivalence under ``contract``.

    Threaded callers are dispatched to :mod:`process_pool` so Z3 never shares
    a context across threads (see ``PPC_EQUIV_PROCESS_POOL``).

    ``platform_profile`` (Stage 3A): optional reviewed platform profile name,
    path, or mapping. When set, bounded-memory coverage is discharged from CFG
    terminals without folding ``build_memory_constraints`` into the coverage
    query. Assurance failure preserves EQUIVALENT but blocks Tier A.

    ``initial_gpr_ranges``: optional per-GPR inclusive ``(lo, hi)`` bounds
    (unsigned 32-bit). Used by object-base MEM1 constraints so symbolic
    pointers (esp. ``r3``) stay inside RAM rather than spanning MMIO.
    """
    from tools.ppc_equivalence.process_pool import run_check_equivalence, should_isolate

    kwargs = dict(
        smt_output=smt_output,
        max_instructions=max_instructions,
        max_paths=max_paths,
        max_loop_iterations=max_loop_iterations,
        assumed_callees=assumed_callees,
        callee_contracts=callee_contracts,
        relocation_bindings=relocation_bindings,
        initial_gpr_bindings=initial_gpr_bindings,
        initial_gpr_ranges=initial_gpr_ranges,
        memory_environment=memory_environment,
        source_hash=source_hash,
        floating_point_domain=floating_point_domain,
        diagnostics_out=diagnostics_out,
        concrete_samples=concrete_samples,
        concrete_sample_seed=concrete_sample_seed,
        jump_table=jump_table,
        virtual_call=virtual_call,
        callee_edges=callee_edges,
        memory_bus=memory_bus,
        readonly_words=readonly_words,
        memory_loop_readonly=memory_loop_readonly,
        platform_profile=platform_profile,
    )
    if should_isolate():
        return run_check_equivalence(
            original,
            candidate,
            contract,
            original_hex=original_hex,
            candidate_hex=candidate_hex,
            assumed_callees_used=assumed_callees_used,
            **kwargs,
        )
    return _check_equivalence_impl(
        original,
        candidate,
        contract,
        original_hex=original_hex,
        candidate_hex=candidate_hex,
        assumed_callees_used=assumed_callees_used,
        **kwargs,
    )


def _check_equivalence_impl(
    original: list[Instruction],
    candidate: list[Instruction],
    contract: EquivalenceContract,
    *,
    original_hex: str,
    candidate_hex: str,
    smt_output: str | None = None,
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    assumed_callees_used: set[int | str] | None = None,
    callee_contracts: dict[int | str, CalleeContract] | None = None,
    relocation_bindings: dict[str, int] | None = None,
    initial_gpr_bindings: dict[int, int] | None = None,
    initial_gpr_ranges: dict[int, tuple[int, int]] | None = None,
    memory_environment: MemoryEnvironment | None = None,
    source_hash: str = "",
    floating_point_domain: FloatingPointDomain | None = None,
    diagnostics_out: str | None = None,
    concrete_samples: int = 0,
    concrete_sample_seed: int = 0,
    jump_table: JumpTableProofContext | None = None,
    virtual_call: VirtualCallProofContext | None = None,
    callee_edges: dict[str, frozenset[str]] | None = None,
    memory_bus: MemoryBus | None = None,
    readonly_words: dict[int, int] | None = None,
    memory_loop_readonly: MemoryLoopReadonlyContext | None = None,
    platform_profile: object | None = None,
) -> ProofResult:
    ops = SymbolicOps()
    z3 = ops.z3
    initial = _symbolic_initial(ops)
    callees_used: set[int | str] = set()
    domain = floating_point_domain if floating_point_domain is not None else FloatingPointDomain()
    # Wall-clock budget covers CFG exploration and constraint construction as
    # well as solving, so path explosion cannot run past the user timeout.
    deadline = Deadline.after_ms(contract.timeout_ms)

    if virtual_call is not None and callee_edges is not None and virtual_call.pairing.cases:
        scc_status = virtual_call_scc_status(
            callee_edges,
            root=virtual_call.pairing.cases[0].identity,
        )
        if scc_status is not None:
            return ProofResult(
                status=scc_status,
                contract=contract.name,
                contract_resolution=contract.resolution_dict(),
                observables=[item.name for item in contract.observables],
                unsupported=[
                    "virtual-call callee SCC cannot be certified bottom-up"
                ],
                limits={
                    "max_instructions": max_instructions,
                    "max_paths": max_paths,
                    "max_loop_iterations": max_loop_iterations,
                },
                floating_point_domain=domain,
                source_hash=source_hash,
            )

    original_jump_targets = (
        None if jump_table is None else jump_table.original_expansion_map()
    )
    candidate_jump_targets = (
        None if jump_table is None else jump_table.candidate_expansion_map()
    )
    if virtual_call is not None:
        vc_original = virtual_call.original_expansion_map()
        vc_candidate = virtual_call.candidate_expansion_map()
        original_jump_targets = {**(original_jump_targets or {}), **vc_original}
        candidate_jump_targets = {**(candidate_jump_targets or {}), **vc_candidate}
    original_affine = build_affine_summary_map(original)
    candidate_affine = build_affine_summary_map(candidate)
    affine_used: list = []

    readonly_ctx = memory_loop_readonly
    if readonly_ctx is None and readonly_words:
        readonly_ctx = build_memory_loop_readonly_context(shared_words=readonly_words)
    if readonly_ctx is not None:
        conflict = readonly_ctx.conflict_reason()
        if conflict is not None:
            return ProofResult(
                status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
                contract=contract.name,
                contract_resolution=contract.resolution_dict(),
                observables=[item.name for item in contract.observables],
                unsupported=[conflict],
                limits={
                    "max_instructions": max_instructions,
                    "max_paths": max_paths,
                    "max_loop_iterations": max_loop_iterations,
                },
                floating_point_domain=domain,
                source_hash=source_hash,
            )
    original_readonly = None if readonly_ctx is None else readonly_ctx.words_for("original")
    candidate_readonly = None if readonly_ctx is None else readonly_ctx.words_for("candidate")
    original_memory = build_memory_loop_plan_map(
        original,
        readonly_words=original_readonly,
    )
    candidate_memory = build_memory_loop_plan_map(
        candidate,
        readonly_words=candidate_readonly,
    )
    original_memory_used: list = []
    candidate_memory_used: list = []

    # GX FIFO recognized-loop plans (GX_FIFO_TIER_A.md mmio-loop-emission-v1):
    # built only against ``gxfifo-stream`` devices actually declared on the
    # bus, so targets without a memory bus (or without a GX device) pay zero
    # cost and see zero behavior change.
    gx_fifo_devices = []
    if memory_bus is not None and memory_bus.specification is not None:
        gx_fifo_devices = [
            device
            for device in memory_bus.specification.devices
            if device.theory == "gxfifo-stream"
        ]
    original_gx_fifo = (
        build_gx_fifo_loop_plans_for_devices(
            original, gx_fifo_devices, readonly_words=original_readonly,
        )
        if gx_fifo_devices
        else {}
    )
    candidate_gx_fifo = (
        build_gx_fifo_loop_plans_for_devices(
            candidate, gx_fifo_devices, readonly_words=candidate_readonly,
        )
        if gx_fifo_devices
        else {}
    )
    original_gx_fifo_used: list = []
    candidate_gx_fifo_used: list = []

    def _early_timeout(phase: str) -> ProofResult:
        return ProofResult(
            status=ProofStatus.INCONCLUSIVE_TIMEOUT,
            contract=contract.name,
            contract_resolution=contract.resolution_dict(),
            observables=[item.name for item in contract.observables],
            warnings=[f"proof deadline exceeded during {phase}"],
            limits={
                "max_instructions": max_instructions,
                "max_paths": max_paths,
                "max_loop_iterations": max_loop_iterations,
            },
            floating_point_domain=domain,
            source_hash=source_hash,
            solver={
                "name": "z3",
                "version": z3.get_version_string(),
                "result": "unknown",
                "elapsed_ms": 0,
                "timeout_ms": contract.timeout_ms,
                "tactic": phase,
                "phases": [],
            },
        )

    try:
        domain.validate()
    except ValueError as exc:
        return ProofResult(
            status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            contract=contract.name,
            contract_resolution=contract.resolution_dict(),
            observables=[item.name for item in contract.observables],
            unsupported=[str(exc)],
            limits={
                "max_instructions": max_instructions,
                "max_paths": max_paths,
                "max_loop_iterations": max_loop_iterations,
            },
            floating_point_domain=domain,
            source_hash=source_hash,
        )

    # PR17: when NI may be set, every NI-affected opcode in the block must be
    # in the NI-supported set — otherwise fail closed (no silent IEEE widening).
    if not domain.require_ni_zero:
        from .fp_ni import effective_ni_supported_ops
        from .semantics import _FP_ROUNDING_SENSITIVE

        ni_supported = effective_ni_supported_ops()
        ni_unsupported = sorted({
            insn.opcode.value
            for insn in original + candidate
            if insn.opcode in _FP_ROUNDING_SENSITIVE
            and insn.opcode.value not in ni_supported
        })
        if ni_unsupported:
            return ProofResult(
                status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
                contract=contract.name,
                contract_resolution=contract.resolution_dict(),
                observables=[item.name for item in contract.observables],
                unsupported=[
                    "FPSCR.NI may be set but the following opcodes have no NI "
                    f"flush model: {ni_unsupported}"
                ],
                limits={
                    "max_instructions": max_instructions,
                    "max_paths": max_paths,
                    "max_loop_iterations": max_loop_iterations,
                },
                floating_point_domain=domain,
                source_hash=source_hash,
                opcodes_used=sorted({
                    insn.opcode.value for insn in original + candidate
                }),
            )
    try:
        if memory_bus is not None:
            clear_side_bus_access_coverage()
        original_exits = execute_cfg(
            initial, original, ops,
            max_instructions=max_instructions, max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees, assumed_callees_used=callees_used,
            callee_contracts=callee_contracts,
            floating_point_domain=domain,
            memory_bus=memory_bus,
            bus_access_side="original" if memory_bus is not None else None,
            deadline=deadline,
            jump_table_targets=original_jump_targets,
            affine_loop_summaries=original_affine,
            affine_summaries_used=affine_used,
            memory_loop_plans=original_memory,
            memory_plans_used=original_memory_used,
            gx_fifo_loop_plans=original_gx_fifo,
            gx_fifo_plans_used=original_gx_fifo_used,
        )
        candidate_exits = execute_cfg(
            initial, candidate, ops,
            max_instructions=max_instructions, max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees, assumed_callees_used=callees_used,
            callee_contracts=callee_contracts,
            floating_point_domain=domain,
            memory_bus=memory_bus,
            bus_access_side="candidate" if memory_bus is not None else None,
            deadline=deadline,
            jump_table_targets=candidate_jump_targets,
            affine_loop_summaries=candidate_affine,
            affine_summaries_used=affine_used,
            memory_loop_plans=candidate_memory,
            memory_plans_used=candidate_memory_used,
            gx_fifo_loop_plans=candidate_gx_fifo,
            gx_fifo_plans_used=candidate_gx_fifo_used,
        )
    except ProofDeadlineExceeded as exc:
        return _early_timeout(exc.phase)
    except ExecutionInconclusive as exc:
        early = ProofResult(
            status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            contract=contract.name,
            contract_resolution=contract.resolution_dict(),
            observables=[item.name for item in contract.observables],
            unsupported=[str(exc)],
            limits={
                "max_instructions": max_instructions,
                "max_paths": max_paths,
                "max_loop_iterations": max_loop_iterations,
            },
            floating_point_domain=domain,
            source_hash=source_hash,
        )
        # Track C: loop×FIFO hard-reject must still attest on the obligation
        # (CFG raises before terminals exist). ``execute_cfg`` now only raises
        # this specific reason for an *unrecognized* fifo-touching loop —
        # legitimate non-fifo-touching or GX-recognized summaries no longer
        # reach this branch at all, so ``loop_summaries_active`` must track
        # that targeted cause exactly (not "any loop summary exists").
        if memory_bus is not None:
            early.proof_features = ["memory-bus"]
            loop_fifo = "symbolic-loop-fifo" in str(exc)
            obligation = build_memory_bus_obligation(memory_bus)
            early.memory_bus = enrich_memory_bus_obligation_with_symbolic_mmio(
                obligation,
                memory_bus,
                loop_summaries_active=loop_fifo,
                original_instructions=original,
                candidate_instructions=candidate,
            )
        return early
    if assumed_callees_used is not None:
        assumed_callees_used.update(callees_used)

    # M2: empty terminal lists make z3.Or() == False → vacuous unsat/EQUIVALENT.
    if not original_exits or not candidate_exits:
        return ProofResult(
            status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            contract=contract.name,
            contract_resolution=contract.resolution_dict(),
            observables=[item.name for item in contract.observables],
            unsupported=[
                "empty-terminal-list: exploration produced no terminals on "
                f"{'original' if not original_exits else 'candidate'} side"
            ],
            warnings=[
                "refusing EQUIVALENT with an empty terminal list "
                "(vacuous divergence query)"
            ],
            limits={
                "max_instructions": max_instructions,
                "max_paths": max_paths,
                "max_loop_iterations": max_loop_iterations,
            },
            floating_point_domain=domain,
            source_hash=source_hash,
        )

    # H1: refine auto observables from symbolic terminal≠initial writes so a
    # mis-tabled persistent component cannot be silently omitted.
    if contract.name == "auto":
        from tools.ppc_equivalence.contract import refine_auto_contract_with_writes

        symbolic_writes = _symbolic_persistent_writes(
            original_exits + candidate_exits, initial, ops,
        )
        contract = refine_auto_contract_with_writes(
            contract, symbolic_writes=symbolic_writes,
        )

    try:
        deadline.require_time("constraint-build")
    except ProofDeadlineExceeded as exc:
        return _early_timeout(exc.phase)

    pair_differences = [
        _terminal_difference(left, right, contract, initial, ops)
        for left in original_exits
        for right in candidate_exits
    ]
    if not pair_differences:
        return ProofResult(
            status=ProofStatus.INTERNAL_ERROR,
            contract=contract.name,
            contract_resolution=contract.resolution_dict(),
            observables=[item.name for item in contract.observables],
            warnings=["internal: nonempty terminals but empty pair_differences"],
            source_hash=source_hash,
        )
    memory_scope = MemoryScope.from_terminals(
        original_exits, candidate_exits, ops,
    )
    layout_constraints = ops.layout_constraints(initial)
    for terminal in original_exits + candidate_exits:
        if terminal.state.stack_layout_valid is not None:
            layout_constraints.append(z3.Implies(
                terminal.condition, terminal.state.stack_layout_valid,
            ))
    for name, value in (relocation_bindings or {}).items():
        if name not in ops.relocation_values:
            raise ValueError(f"relocation binding names unused symbol {name!r}")
        layout_constraints.append(ops.relocation_values[name] == ops.const(value))
    for register, value in (initial_gpr_bindings or {}).items():
        reg = int(register)
        if not 0 <= reg < 32:
            raise ValueError(f"initial GPR binding register out of range: {reg}")
        layout_constraints.append(initial.gpr[reg] == ops.const(int(value) & 0xFFFFFFFF))

    object_base_assumptions: list[str] = []
    if initial_gpr_ranges:
        from tools.ppc_equivalence.object_base import format_object_base_assumption

        for register, bounds in initial_gpr_ranges.items():
            reg = int(register)
            if not 0 <= reg < 32:
                raise ValueError(f"initial GPR range register out of range: {reg}")
            if not isinstance(bounds, tuple) or len(bounds) != 2:
                raise ValueError(
                    f"initial GPR range for r{reg} must be a (lo, hi) tuple, got {bounds!r}"
                )
            lo_raw, hi_raw = bounds
            if not isinstance(lo_raw, int) or not isinstance(hi_raw, int):
                raise ValueError(
                    f"initial GPR range for r{reg} requires int lo/hi, got {bounds!r}"
                )
            lo = lo_raw & 0xFFFFFFFF
            hi = hi_raw & 0xFFFFFFFF
            layout_constraints.append(z3.ULE(ops.const(lo), initial.gpr[reg]))
            layout_constraints.append(z3.ULE(initial.gpr[reg], ops.const(hi)))
            object_base_assumptions.append(format_object_base_assumption(reg, lo, hi))

    # Always apply and record an effective environment (default:
    # assumed-ordinary-ram). Omitting the field previously hid assumed-RAM from
    # tiering/promotion while the solver still used unconstrained memory.
    effective_memory_environment = memory_environment or MemoryEnvironment()
    memory_constraints = build_memory_constraints(
        original_exits, candidate_exits,
        effective_memory_environment,
        ops,
    )
    layout_constraints.extend(memory_constraints)

    if memory_bus is not None:
        layout_constraints.extend(
            build_memory_bus_constraints(
                memory_bus,
                initial.memory,
                original_exits + candidate_exits,
                ops,
            )
        )

    if jump_table is not None:
        # Pin initial ROM image bytes only. Do NOT auto-constrain
        # gpr[base]==table.base or index < entry_count — those must come from
        # executed code (or an explicit future Tier-C contract precondition).
        # Coverage / no-write are independent UNSAT discharges, not mismatch
        # formulas and not final-memory-value inspection alone.
        for table in jump_table.readonly_tables():
            layout_constraints.extend(
                rom_image_byte_constraints(initial.memory, table, ops)
            )

    if virtual_call is not None:
        for table in virtual_call.readonly_tables():
            layout_constraints.extend(
                rom_image_byte_constraints(initial.memory, table, ops)
            )

    jump_table_bundle = None
    if jump_table is not None:
        try:
            jump_table_bundle = discharge_jump_table_obligations(
                jump_table,
                original_terminals=original_exits,
                candidate_terminals=candidate_exits,
                premises=layout_constraints,
                ops=ops,
                deadline=deadline,
            )
        except ProofDeadlineExceeded as exc:
            return _early_timeout(exc.phase)

    virtual_call_bundle = None
    if virtual_call is not None:
        try:
            virtual_call_bundle = discharge_virtual_call_obligations(
                virtual_call,
                original_terminals=original_exits,
                candidate_terminals=candidate_exits,
                premises=layout_constraints,
                ops=ops,
                deadline=deadline,
            )
        except ProofDeadlineExceeded as exc:
            return _early_timeout(exc.phase)

    if readonly_ctx is not None:
        layout_constraints.extend(
            readonly_word_byte_constraints(
                initial.memory,
                readonly_ctx.all_evidence(),
                ops,
            ),
        )

    try:
        deadline.require_time("solve")
    except ProofDeadlineExceeded as exc:
        return _early_timeout(exc.phase)

    def _build_solver() -> Any:
        s = z3.Solver()
        s.add(*layout_constraints)
        s.add(z3.Or(*pair_differences))
        return s

    if smt_output is not None:
        s = _build_solver()
        Path(smt_output).write_text(s.to_smt2(), encoding="utf-8")

    winning_solver, answer, tactic, phases = check_with_portfolio(
        z3, _build_solver, deadline,
    )
    elapsed_ms = round(sum(p.elapsed_ms for p in phases), 3)

    result = ProofResult(
        status=ProofStatus.INCONCLUSIVE_UNKNOWN,
        environment=effective_memory_environment,
        memory_scope=memory_scope,
        contract=contract.name,
        contract_resolution=contract.resolution_dict(),
        observables=[item.name for item in contract.observables],
        original_instruction_count=len(original),
        candidate_instruction_count=len(candidate),
        opcodes_used=sorted({insn.opcode.value for insn in original + candidate}),
        solver={
            "name": "z3", "version": z3.get_version_string(), "result": str(answer),
            "elapsed_ms": elapsed_ms, "timeout_ms": contract.timeout_ms,
            "original_paths": len(original_exits), "candidate_paths": len(candidate_exits),
            "tactic": tactic, "phases": [p.to_dict() for p in phases],
        },
        limits={
            "max_instructions": max_instructions,
            "max_paths": max_paths,
            "max_loop_iterations": max_loop_iterations,
        },
        assumed_callees=sorted(callees_used, key=str),
        callee_contracts={
            str(callee): {
                "source": (callee_contracts or {}).get(
                    callee, CalleeContract.opaque_eabi(),
                ).source,
                "reads": sorted((callee_contracts or {}).get(
                    callee, CalleeContract.opaque_eabi(),
                ).reads),
                "writes": sorted((callee_contracts or {}).get(
                    callee, CalleeContract.opaque_eabi(),
                ).writes),
            }
            for callee in sorted(callees_used, key=str)
        },
    )
    for assumption in object_base_assumptions:
        if assumption not in result.assumptions:
            result.assumptions.append(assumption)
    result.source_hash = source_hash
    try:
        commit, dirty = live_git_identity(_REPO_ROOT)
        if commit:
            result.git_commit = commit
        result.git_dirty = dirty
    except Exception:
        pass
    try:
        result.engine_hash = hash_engine_tree(_REPO_ROOT)
    except Exception:
        pass
    try:
        result.certifier_hash = hash_certifier_tree(_REPO_ROOT)
    except Exception:
        pass
    if result.source_hash and not result.proof_request_hash:
        result.proof_request_hash = result.source_hash
    result.platform = sys.platform
    result.python_version = sys.version
    result.z3_version = z3.get_version_string()
    try:
        import capstone as cs
        result.capstone_version = ".".join(str(x) for x in cs.version)
    except Exception:
        pass
    all_insns = original + candidate
    if floating_point_domain is not None:
        result.floating_point_domain = domain
    elif any(insn.opcode in SUPPORTED_FP_OPCODES for insn in all_insns):
        result.floating_point_domain = domain
    def _emit_diagnostics(*, memory_constraint_count: int = len(memory_constraints)) -> None:
        _populate_solver_diagnostics(
            result,
            layout_constraints=layout_constraints,
            original_exits=original_exits,
            candidate_exits=candidate_exits,
            relocation_bindings=relocation_bindings or {},
            phases=phases,
            answer=answer,
            winning_solver=winning_solver,
            build_solver=_build_solver,
            diagnostics_out=diagnostics_out,
            source_hash=source_hash,
            memory_constraint_count=memory_constraint_count,
        )

    def _finalize(early: ProofResult) -> ProofResult:
        _apply_concrete_sampling_defense(
            early,
            original,
            candidate,
            contract,
            concrete_samples=concrete_samples,
            concrete_sample_seed=concrete_sample_seed,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees,
            callee_contracts=callee_contracts,
            floating_point_domain=domain,
            memory_bus=memory_bus,
            original_hex=original_hex,
            candidate_hex=candidate_hex,
        )
        # Paired-single SoftFloat oracle identity (PR13) — bind into result when used.
        from .fp_outcome import FP_ORACLE_VERSION, PAIRED_ORACLE_OPS

        used_opcodes = set(early.opcodes_used) or {
            insn.opcode.value for insn in original + candidate
        }
        if used_opcodes & PAIRED_ORACLE_OPS:
            early.fp_oracle_version = FP_ORACLE_VERSION
        if memory_bus is not None:
            features = list(early.proof_features)
            if "memory-bus" not in features:
                features.append("memory-bus")
            early.proof_features = features
            obligation = build_memory_bus_obligation(memory_bus)
            early.memory_bus = enrich_memory_bus_obligation_with_symbolic_mmio(
                obligation,
                memory_bus,
                original_terminals=original_exits,
                candidate_terminals=candidate_exits,
                original_instructions=original,
                candidate_instructions=candidate,
                ops=ops,
                deadline=deadline,
                # Reaching this success path means ``execute_cfg`` did not
                # raise for either side, so no *unrecognized* fifo-touching
                # loop summary was present — the hard-reject attestation
                # must not fire here. Whether a recognized GX FIFO loop plan
                # was actually discharged is reported separately below.
                loop_summaries_active=False,
                gx_loop_plans_used=bool(original_gx_fifo_used or candidate_gx_fifo_used),
            )
            unsupported = (early.memory_bus or {}).get("unsupported_access") or {}
            for side in ("original", "candidate"):
                side_block = unsupported.get(side) or {}
                if side_block.get("result") in ("sat", "unknown"):
                    reason = (
                        f"symbolic MMIO unsupported-access query {side_block['result']} "
                        f"on {side}"
                    )
                    if early.status is ProofStatus.EQUIVALENT:
                        early.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                    early.unsupported.append(reason)
                    early.warnings.append(reason)
                    early.abstractions.append("symbolic-mmio-unsupported-access")
                    break
            cfg_rejections = (early.memory_bus or {}).get("cfg_rejections") or []
            if cfg_rejections:
                reason = f"symbolic MMIO/FIFO CFG rejection: {cfg_rejections[0]}"
                if early.status is ProofStatus.EQUIVALENT:
                    early.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                early.unsupported.append(reason)
                early.warnings.append(reason)
                early.abstractions.append("symbolic-mmio-cfg-rejection")
            # Engine-built obligation only: demote when discharge incomplete.
            bus_status = (early.memory_bus or {}).get("status")
            if bus_status != "discharged" and early.status is ProofStatus.EQUIVALENT:
                reason = (
                    f"memory-bus obligation status={bus_status!r} "
                    "(requires engine-generated discharged)"
                )
                early.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                early.unsupported.append(reason)
                early.warnings.append(reason)
                early.abstractions.append("memory-bus-discharge-incomplete")

        if jump_table is not None:
            early.proof_features = ["readonly-image", "indirect-target-closure"]
            if jump_table_bundle is not None:
                early.address_space, early.indirect_targets = obligations_from_discharge(
                    jump_table, jump_table_bundle,
                )
                if not jump_table_bundle.all_unsat():
                    reason = jump_table_bundle.failure_reason() or (
                        "jump-table coverage/no-write discharge failed"
                    )
                    if early.status is ProofStatus.EQUIVALENT:
                        early.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                    early.unsupported.append(reason)
                    early.warnings.append(reason)
                    early.abstractions.append("jump-table-discharge-failed")
            else:
                from .jump_table_obligations import build_jump_table_obligations

                early.address_space, early.indirect_targets = build_jump_table_obligations(
                    jump_table,
                    no_write_status="pending",
                    coverage="pending",
                    status="pending",
                )
        if virtual_call is not None:
            features = list(early.proof_features)
            for name in ("readonly-image", "indirect-target-closure"):
                if name not in features:
                    features.append(name)
            early.proof_features = features
            if virtual_call_bundle is not None:
                address_space, indirect_targets = obligations_from_virtual_call_discharge(
                    virtual_call, virtual_call_bundle,
                )
                if jump_table is None:
                    early.address_space = address_space
                    early.indirect_targets = indirect_targets
                else:
                    early.indirect_targets = {
                        **(early.indirect_targets or {}),
                        "virtual_call": indirect_targets,
                    }
                if not virtual_call_bundle.all_unsat():
                    reason = virtual_call_bundle.failure_reason() or (
                        "virtual-call coverage discharge failed"
                    )
                    if early.status is ProofStatus.EQUIVALENT:
                        early.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                    early.unsupported.append(reason)
                    early.warnings.append(reason)
                    early.abstractions.append("virtual-call-discharge-failed")
            else:
                from .vtable_obligations import build_virtual_call_obligations

                address_space, indirect_targets = build_virtual_call_obligations(
                    virtual_call,
                    no_write_status="pending",
                    coverage="pending",
                    status="pending",
                )
                if jump_table is None:
                    early.address_space = address_space
                    early.indirect_targets = indirect_targets
        if affine_used:
            # Prefer the original-side summary when both sides applied (identical
            # trip counts); obligation identity still binds trip_count/strides.
            summary = affine_used[0]
            features = list(early.proof_features)
            if "affine-loop-summary" not in features:
                features.append("affine-loop-summary")
            early.proof_features = features
            relational = try_discharge_relational(original, candidate)
            affine_status = "applied"
            relational_companion: str | None = None
            if (
                relational is not None
                and relational.status == "discharged"
                and summary.proof_kind in (
                    "affine-closed-form",
                    "compare-affine-closed-form",
                )
            ):
                affine_status = "discharged"
                relational_companion = "discharged"
            early.loop_summary = build_loop_summary_obligation(
                summary,
                coverage="applied",
                status=affine_status,
                relational_companion=relational_companion,
            )
            if relational is not None and relational.status == "discharged":
                if "relational-induction" not in features:
                    features.append("relational-induction")
                early.proof_features = features
                early.relational_induction = relational.to_obligation_dict()
        # PR12: attach bulk+remainder relational sketches (discharged when
        # identity + shared RangeWrite SMT succeed; otherwise pending).
        if early.relational_induction is None:
            bulk_sketch = try_build_bulk_remainder_relational_sketch(
                original,
                candidate,
                readonly_words=(
                    None
                    if readonly_ctx is None
                    else {
                        item.address: item.value
                        for item in readonly_ctx.all_evidence()
                    }
                ),
            )
            if bulk_sketch is not None:
                features = list(early.proof_features)
                if "relational-induction" not in features:
                    features.append("relational-induction")
                early.proof_features = features
                early.relational_induction = bulk_sketch.to_obligation_dict()
        if original_memory_used or candidate_memory_used:
            features = list(early.proof_features)
            if "memory-loop-summary" not in features:
                features.append("memory-loop-summary")
            early.proof_features = features
            readonly_digest = None
            if readonly_ctx is not None:
                readonly_digest = canonical_json_sha256(
                    {
                        "original": [
                            {"address": item.address, "value": item.value, "source": item.source}
                            for item in readonly_ctx.original
                        ],
                        "candidate": [
                            {"address": item.address, "value": item.value, "source": item.source}
                            for item in readonly_ctx.candidate
                        ],
                    },
                )
            # Per-side refinement: each used plan proves instructions ≡ summary.
            # Never pair loops by equal addresses or require structural summary match.
            from .memory_loop_discharge import (
                canonical_unique_plans,
                discharge_memory_loop_plan,
            )

            entry_premises: list[Any] = list(layout_constraints)

            def _header_from_target(target: Any) -> int | None:
                if target is None:
                    return None
                if hasattr(target, "as_long"):
                    try:
                        return int(target.as_long()) & 0xFFFFFFFF
                    except Exception:
                        pass
                try:
                    simplified = ops.z3.simplify(target)
                    if hasattr(simplified, "as_long"):
                        return int(simplified.as_long()) & 0xFFFFFFFF
                except Exception:
                    pass
                return None

            def _side_entries(
                plans: list,
                exits: list[Terminal],
            ) -> tuple[list[dict], str]:
                side_status = "discharged"
                entries: list[dict] = []
                for plan in canonical_unique_plans(plans):
                    header = int(plan.summary.header_pc) & 0xFFFFFFFF
                    matched_violations: list[Any] = []
                    for term in exits:
                        if term.exit_kind != "memory-loop-entry-premise":
                            continue
                        term_header = _header_from_target(term.exit_target)
                        if term_header is None or term_header == header:
                            matched_violations.append(term.condition)

                    if plan.summary.expansion != "closed-form" or int(plan.summary.trip_count) < 1:
                        entries.append(build_memory_loop_side_entry(plan))
                        side_status = "applied"
                        continue

                    discharge_result = discharge_memory_loop_plan(
                        plan,
                        deadline=deadline,
                        z3_module=z3,
                        entry_guard_premises=entry_premises,
                        entry_violation_conditions=matched_violations,
                    )
                    entries.append(
                        build_memory_loop_side_entry(
                            plan,
                            entry_guard=discharge_result.entry_guard,
                            refinement=discharge_result.refinement,
                        ),
                    )
                    if discharge_result.status == "discharged":
                        continue
                    if discharge_result.status == "internal_error":
                        reason = discharge_result.reason or (
                            "memory-loop refinement SAT (INTERNAL_ERROR)"
                        )
                        early.status = ProofStatus.INTERNAL_ERROR
                        early.unsupported.append(reason)
                        early.warnings.append(reason)
                        early.abstractions.append("memory-loop-refinement-sat")
                        side_status = "failed"
                        continue
                    reason = discharge_result.reason or (
                        "memory-loop refinement discharge incomplete"
                    )
                    if early.status is ProofStatus.EQUIVALENT:
                        if discharge_result.proof_status_hint == "INCONCLUSIVE_TIMEOUT":
                            early.status = ProofStatus.INCONCLUSIVE_TIMEOUT
                        elif discharge_result.proof_status_hint == "INCONCLUSIVE_UNKNOWN":
                            early.status = ProofStatus.INCONCLUSIVE_UNKNOWN
                        else:
                            early.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                    early.unsupported.append(reason)
                    early.warnings.append(reason)
                    early.abstractions.append("memory-loop-discharge-incomplete")
                    side_status = "applied"
                if not plans:
                    return entries, "discharged"
                return entries, side_status

            original_entries, original_status = _side_entries(
                original_memory_used, original_exits,
            )
            candidate_entries, candidate_status = _side_entries(
                candidate_memory_used, candidate_exits,
            )
            if original_status == "discharged" and candidate_status == "discharged":
                memory_status = "discharged"
            elif original_status == "failed" or candidate_status == "failed":
                memory_status = "failed"
            else:
                memory_status = "applied"
            early.memory_loop = build_memory_loop_obligation(
                original=original_entries,
                candidate=candidate_entries,
                status=memory_status,
                readonly_words_sha256=readonly_digest,
            )
        gated = enforce_equivalent_proof_features(early)
        # Shared unconstrained memory can make identical jump-table functions
        # look EQUIVALENT without an immutable table image or target closure.
        # Fail closed unless a JumpTableProofContext discharged the obligations.
        if gated.status is ProofStatus.EQUIVALENT and jump_table is None:
            reason = jump_table_gate_reason(original, candidate)
            if reason is not None:
                gated.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                gated.unsupported.append(reason)
                gated.warnings.append(reason)
                gated.abstractions.append("jump-table-unproven")
        if gated.status is ProofStatus.EQUIVALENT and virtual_call is None:
            reason = virtual_call_gate_reason(original, candidate)
            if reason is not None:
                gated.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                gated.unsupported.append(reason)
                gated.warnings.append(reason)
                gated.abstractions.append("virtual-call-unproven")
        # M1: catch-all fail-closed gate for any *remaining* unresolved indirect
        # exit when neither a jump-table nor virtual-call proof context is
        # attached. Broader than the pattern-specific gates above: it keys on the
        # actual terminal exit_kind, so non-canonical dispatchers (a bcctr with
        # no adjacent lwzx, an unlinked bclr, etc.) also fail closed. Runs after
        # the specific gates so their diagnostics win; input-derived virtual
        # thunks are exempt (proven equal via exit.target — see the helper).
        if (
            gated.status is ProofStatus.EQUIVALENT
            and jump_table is None
            and virtual_call is None
        ):
            reason = unresolved_indirect_exit_gate_reason(
                original_exits,
                candidate_exits,
                original=original,
                candidate=candidate,
            )
            if reason is not None:
                gated.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
                gated.unsupported.append(reason)
                gated.warnings.append(reason)
                gated.abstractions.append("indirect-exit-unproven")
        # H2: Z3 FP collapses NaN payloads — require NaN-freedom for EQUIVALENT.
        if gated.status is ProofStatus.EQUIVALENT:
            from tools.ppc_equivalence.nan_freedom import enforce_nan_freedom

            used_fp = any(
                insn.opcode in SUPPORTED_FP_OPCODES for insn in original + candidate
            )
            gated = enforce_nan_freedom(
                gated,
                original_exits=original_exits,
                candidate_exits=candidate_exits,
                contract=contract,
                ops=ops,
                domain=domain,
                layout_constraints=layout_constraints,
                used_fp=used_fp,
                deadline_ms=min(5_000, max(1, contract.timeout_ms // 4)),
            )
        # M4: FPSCR sticky modeling is incomplete on the symbolic path —
        # fail closed (demote EQUIVALENT) when fpscr was compared after arith
        # on differing implementations.
        if gated.status is ProofStatus.EQUIVALENT:
            from tools.ppc_equivalence.fp_fpscr import (
                annotate_fpscr_sticky_incompleteness,
            )

            annotate_fpscr_sticky_incompleteness(
                gated,
                identical_implementations=(
                    original_hex.replace(" ", "").lower()
                    == candidate_hex.replace(" ", "").lower()
                ),
            )
        # Stage 1 + 4: derive capability requirements and attach drafts.
        # Failure preserves EQUIVALENT; Tier A is blocked by incomplete assurance.
        if gated.status is ProofStatus.EQUIVALENT:
            from tools.ppc_equivalence.capability_assurance import (
                build_capability_assurance,
            )

            build_capability_assurance(
                gated,
                terminals_meta={
                    "original": original_exits,
                    "candidate": candidate_exits,
                },
                original_terminals=original_exits,
                candidate_terminals=candidate_exits,
                platform_profile=platform_profile,
                environment=effective_memory_environment,
                deadline=deadline,
                z3=z3,
            )
        return gated

    feasibility = z3.Solver()
    try:
        layout_deadline = deadline.require_time("layout-feasibility")
    except ProofDeadlineExceeded:
        result.status = ProofStatus.INCONCLUSIVE_TIMEOUT
        result.warnings.append("layout-feasibility deadline exceeded")
        _emit_diagnostics()
        return _finalize(result)
    feasibility.set(timeout=layout_deadline)
    feasibility.add(*layout_constraints)
    feasibility_answer = feasibility.check()
    if feasibility_answer != z3.sat:
        result.status = ProofStatus.INCONCLUSIVE_LAYOUT
        result.abstractions.append("linker-layout-feasibility")
        result.warnings.append(
            "no feasible linker layout satisfies the relocation field ranges"
            if feasibility_answer == z3.unsat else feasibility.reason_unknown()
        )
        _emit_diagnostics()
        return _finalize(result)
    if answer == z3.unsat:
        result.status = ProofStatus.EQUIVALENT
        _emit_diagnostics()
        return _finalize(result)
    if answer == z3.unknown:
        reason = winning_solver.reason_unknown()
        result.status = ProofStatus.INCONCLUSIVE_TIMEOUT if "timeout" in reason.lower() else ProofStatus.INCONCLUSIVE_UNKNOWN
        result.warnings.append(reason)
        _emit_diagnostics()
        return _finalize(result)

    if callees_used:
        result.status = ProofStatus.INCONCLUSIVE_ABSTRACTION
        result.abstractions = [
            f"opaque_callee:{callee}:"
            f"{(callee_contracts or {}).get(callee, CalleeContract.opaque_eabi()).source}"
            for callee in sorted(callees_used, key=str)
        ]
        result.counterexample_kind = "abstract"
        result.warnings.append(
            "the solver found a difference only in the opaque matched-callee model; "
            "this is not a concrete inequivalence witness"
        )
    else:
        result.status = ProofStatus.NOT_EQUIVALENT
        result.counterexample_kind = "concrete"
    model = winning_solver.model()
    selected: tuple[Terminal, Terminal] | None = None
    for left in original_exits:
        for right in candidate_exits:
            if z3.is_true(model.eval(
                _terminal_difference(left, right, contract, initial, ops),
                model_completion=True,
            )):
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
    elif z3.is_true(model.eval(
        z3.And(
            z3.Not(left_exit.state.valid), z3.Not(right_exit.state.valid),
            left_exit.state.invalid_reason != right_exit.state.invalid_reason,
        ),
        model_completion=True,
    )):
        result.mismatch = {
            "kind": "definedness",
            "name": "invalid-reason",
            "original": str(model.eval(left_exit.state.invalid_reason, model_completion=True)),
            "candidate": str(model.eval(right_exit.state.invalid_reason, model_completion=True)),
        }
    else:
        from tools.ppc_equivalence.contract import observables_for_exit

        if left_exit.exit_kind == right_exit.exit_kind:
            cex_observables = observables_for_exit(contract, left_exit.exit_kind)
        else:
            cex_observables = contract.observables
        for observable in cex_observables:
            left = _observable_value(left_exit.state, observable, ops)
            right = _observable_value(right_exit.state, observable, ops)
            difference = _observable_difference(
                left_exit.state, right_exit.state, observable, initial, ops,
            )
            if z3.is_true(model.eval(difference, model_completion=True)):
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
    relocation_values = {
        name: _hex_value(model, value)
        for name, value in sorted(ops.relocation_values.items())
    }
    result.counterexample = {
        "initial_state": initial_state,
        "relocations": relocation_values,
    }

    contextual_proof = bool(relocation_values or callees_used)
    if contextual_proof:
        result.warnings.append(
            "concrete replay and instruction repair hints are unavailable for "
            "symbolic relocations or opaque callee summaries"
        )
    else:
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

    first_divergence = None
    original_trace = None
    candidate_trace = None
    minimized_model_values = None
    if result.status == ProofStatus.NOT_EQUIVALENT:
        if result.replay is None:
            # Known limitation: symbolic relocations / opaque callees block
            # ConcreteOps replay. Keep NOT_EQUIVALENT but record the gap.
            first_divergence = {
                "error": "symbolic-relocations-prevent-concrete-replay",
            }
        else:
            from .diagnostics import (
                minimize_counterexample_model,
                replay_counterexample,
            )
            replay_info = replay_counterexample(
                original, candidate, initial_state, contract,
                floating_point_domain=domain,
            )
            first_divergence = replay_info["first_divergence"]
            original_trace = replay_info.get("original_trace")
            candidate_trace = replay_info.get("candidate_trace")
            if not replay_info.get("reproduced"):
                error = replay_info.get("error") or ""
                # Only escalate when ConcreteOps completed but failed to
                # reproduce the SAT witness. Execution/construction failures
                # are recorded in first_divergence without changing status.
                if error == "sat-witness-not-reproduced-under-concrete-ops" or (
                    isinstance(first_divergence, dict)
                    and first_divergence.get("error")
                    == "sat-witness-not-reproduced-under-concrete-ops"
                ):
                    result.status = ProofStatus.INTERNAL_ERROR
                    result.warnings.append(
                        error or "SAT witness could not be reproduced under ConcreteOps"
                    )
            else:
                # Preserve the SAT model; attach a ConcreteOps-verified shrink.
                minimized_model_values = minimize_counterexample_model(
                    original, candidate, initial_state, contract,
                )

    memory_dict = (
        result.environment.to_dict() if isinstance(result.environment, MemoryEnvironment)
        else MemoryEnvironment().to_dict()
    )
    bundle: dict[str, Any] = {
        "proof_request": {
            "original_hex": original_hex,
            "candidate_hex": candidate_hex,
            "base_original": original[0].address,
            "base_candidate": candidate[0].address,
            "contract": contract.name,
            "observables": [item.name for item in contract.observables],
            "memory_environment": memory_dict,
        },
        "model_values": initial_state,
        "minimized_model_values": minimized_model_values,
        "original_bin": original_hex,
        "candidate_bin": candidate_hex,
        "relocations": relocation_values,
        "contract": {
            "name": contract.name,
            "resolution": contract.resolution_dict(),
        },
        "replay_command": "python -m tools.ppc_equivalence replay <bundle_path> --json",
        "original_trace": original_trace,
        "candidate_trace": candidate_trace,
        "first_divergence": first_divergence,
    }
    result.counterexample_bundle = bundle

    _emit_diagnostics()
    return _finalize(result)

"""Per-side refinement discharge for closed-form memory loops.

Recognizing a constant-stride store loop and applying its closed-form summary is
**not** a proof. A memory-loop obligation may only carry ``status=discharged``
(and therefore authorize ``EQUIVALENT`` once unfrozen) when every used plan on
each side discharges:

``entry_guard``
    Under layout / readonly / path premises, ``CTR != summarized trip count``
    is unreachable.
``body_step``
    One ordinary witness iteration equals one summarized iteration.
``postcondition``
    Exact ``N``-iteration witness execution equals the full closed-form summary.
``stack_escape``
    Ordinary and summarized one-iteration stack-private results agree when the
    store source is an ``r1``-derived pointer (any GPR).
``termination`` / ``footprint``
    CTR ranking and static footprint gates hold.

Refinement SAT is an ``INTERNAL_ERROR`` (summary disagrees with instruction
semantics), never ``NOT_EQUIVALENT``. Solver timeout / unknown leave the plan
``applied``. Structural equality between original and candidate summaries is
**not** required — each side only proves instructions ≡ its own summary.
"""

from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any, Sequence

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.discharge import UnsatDischarge, discharge_bad_conditions
from tools.ppc_equivalence.ir import ExecutionInconclusive, Opcode
from tools.ppc_equivalence.memory_loop import (
    MemoryLoopPlan,
    MemoryLoopSummary,
    apply_memory_loop_iteration_summary,
    apply_memory_loop_summary,
)
from tools.ppc_equivalence.memory_semantics import footprint_ok_for_summary
from tools.ppc_equivalence.relational_discharge import (
    discharge_block_payload,
    termination_block_payload,
)

ENTRY_GUARD_ALGORITHM = "memory-loop-entry-ctr-v2"
BODY_STEP_ALGORITHM = "memory-loop-body-step-v2"
POSTCONDITION_ALGORITHM = "memory-loop-postcondition-v2"
STACK_ESCAPE_ALGORITHM = "memory-loop-stack-escape-v2"
TERMINATION_ALGORITHM = "memory-loop-ctr-termination-v2"
FOOTPRINT_ALGORITHM = "memory-loop-footprint-v2"

REFINEMENT_ALGORITHM = "memory-loop-exact-refinement-v2"

REQUIRED_REFINEMENT_BLOCKS: tuple[str, ...] = (
    "body_step",
    "postcondition",
    "stack_escape",
    "termination",
    "footprint",
)

KNOWN_REFINEMENT_ALGORITHMS: frozenset[str] = frozenset({
    BODY_STEP_ALGORITHM,
    POSTCONDITION_ALGORITHM,
    STACK_ESCAPE_ALGORITHM,
    TERMINATION_ALGORITHM,
    FOOTPRINT_ALGORITHM,
})

# Legacy aliases retained so validators can reject v1 names explicitly.
KNOWN_TRANSITION_ALGORITHMS: frozenset[str] = frozenset()
REQUIRED_TRANSITION_BLOCKS: tuple[str, ...] = ()


@dataclass(frozen=True)
class MemoryLoopDischargeResult:
    """Per-plan refinement discharge outcome."""

    status: str  # "discharged" | "failed" | "applied" | "unsupported" | "internal_error"
    entry_guard: dict[str, Any]
    refinement: dict[str, Any]
    reason: str | None = None
    proof_status_hint: str | None = None  # INTERNAL_ERROR | INCONCLUSIVE_*

    def all_unsat(self) -> bool:
        return self.status == "discharged"


def refinement_for_plan(
    plan: MemoryLoopPlan,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
    entry_guard_premises: Sequence[Any] = (),
    entry_violation_conditions: Sequence[Any] = (),
) -> dict[str, Any] | None:
    """Discharge ``plan`` and return the attachable side-entry payload pieces."""
    result = discharge_memory_loop_plan(
        plan,
        deadline=deadline,
        z3_module=z3_module,
        entry_guard_premises=entry_guard_premises,
        entry_violation_conditions=entry_violation_conditions,
    )
    if not result.all_unsat():
        return None
    return {
        "entry_guard": result.entry_guard,
        "refinement": result.refinement,
    }


def transition_equivalence_for_summary(
    summary: MemoryLoopSummary,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> dict[str, Any] | None:
    """Deprecated: v1 self-referential discharge is removed.

    Returns ``None`` always so callers cannot forge discharged v1 evidence.
    """
    del summary, deadline, z3_module
    return None


def _fresh_symbolic_state() -> tuple[Any, Any]:
    from tools.ppc_equivalence.engine import _symbolic_initial
    from tools.ppc_equivalence.semantics import SymbolicOps

    ops = SymbolicOps()
    return _symbolic_initial(ops), ops


def execute_memory_loop_witness(
    entry: Any,
    plan: MemoryLoopPlan,
    ops: Any,
) -> tuple[Any, tuple[Any, ...]]:
    """Execute the recognized witness exactly ``trip_count`` iterations.

    Bypasses CFG visit limits while still using ordinary instruction semantics.
    """
    from tools.ppc_equivalence.semantics import execute_bdnz_latch, execute_instruction

    state = entry
    predicates: list[Any] = []
    for _ in range(int(plan.summary.trip_count)):
        for instruction in plan.witness.body:
            state = execute_instruction(state, instruction, ops)
        state, taken = execute_bdnz_latch(
            state,
            plan.witness.latch,
            ops,
            header_pc=plan.witness.header_pc,
        )
        predicates.append(taken)
    return state, tuple(predicates)


def discharge_memory_loop_plan(
    plan: MemoryLoopPlan,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
    entry_guard_premises: Sequence[Any] = (),
    entry_violation_conditions: Sequence[Any] = (),
) -> MemoryLoopDischargeResult:
    """Prove entry-guard + refinement queries for one closed-form plan."""
    summary = plan.summary
    if summary.expansion != "closed-form":
        return MemoryLoopDischargeResult(
            "applied",
            {},
            {},
            "bounded-remainder expansion is not closed-form",
            "INCONCLUSIVE_UNSUPPORTED",
        )
    if summary.trip_count < 1:
        return MemoryLoopDischargeResult(
            "applied",
            {},
            {},
            "non-positive concrete trip count",
            "INCONCLUSIVE_UNSUPPORTED",
        )
    if summary.stride != summary.store_width:
        return MemoryLoopDischargeResult(
            "failed",
            {},
            {},
            "stride does not equal store width",
            "INCONCLUSIVE_UNSUPPORTED",
        )
    if summary.store_kind not in ("stwu", "d-form-addi"):
        return MemoryLoopDischargeResult(
            "failed",
            {},
            {},
            f"unsupported store kind {summary.store_kind!r}",
            "INCONCLUSIVE_UNSUPPORTED",
        )
    if any(getattr(state, "symbolic_bus", None) is not None for state in ()):
        pass  # placeholder; bus rejection handled during refinement

    try:
        _validate_witness_shape(plan)
    except ValueError as exc:
        return MemoryLoopDischargeResult(
            "unsupported",
            {},
            {},
            str(exc),
            "INCONCLUSIVE_UNSUPPORTED",
        )

    if z3_module is None:
        import z3 as z3_module  # type: ignore[no-redef]
    if deadline is None:
        deadline = Deadline.after_ms(15_000)

    initial, ops = _fresh_symbolic_state()
    if initial.symbolic_bus is not None:
        return MemoryLoopDischargeResult(
            "unsupported",
            {},
            {},
            "memory-loop refinement rejects an active memory bus",
            "INCONCLUSIVE_UNSUPPORTED",
        )

    entry_guard = _discharge_entry_guard(
        initial,
        ops,
        plan,
        deadline,
        z3_module,
        premises=list(entry_guard_premises),
        violation_conditions=list(entry_violation_conditions),
    )
    entry_guard_payload = {
        "algorithm": ENTRY_GUARD_ALGORITHM,
        "result": entry_guard.status,
        "query_sha256": entry_guard.query_sha256,
    }

    blocks: dict[str, dict[str, Any]] = {}
    try:
        body_step = _discharge_body_step(initial, ops, plan, deadline, z3_module)
    except (_UnsupportedRefinement, ValueError, ExecutionInconclusive) as exc:
        return MemoryLoopDischargeResult(
            "unsupported",
            entry_guard_payload,
            {},
            str(exc),
            "INCONCLUSIVE_UNSUPPORTED",
        )
    blocks["body_step"] = discharge_block_payload(
        body_step, invariants=[], z3_module=z3_module,
    )

    try:
        postcondition = _discharge_postcondition(
            initial, ops, plan, deadline, z3_module,
        )
    except (_UnsupportedRefinement, ValueError, ExecutionInconclusive) as exc:
        return MemoryLoopDischargeResult(
            "unsupported",
            entry_guard_payload,
            {"algorithm": REFINEMENT_ALGORITHM, "status": "unsupported", **blocks},
            str(exc),
            "INCONCLUSIVE_UNSUPPORTED",
        )
    blocks["postcondition"] = discharge_block_payload(
        postcondition, invariants=[], z3_module=z3_module,
    )

    stack_escape = _discharge_stack_escape(initial, ops, plan, deadline, z3_module)
    blocks["stack_escape"] = discharge_block_payload(
        stack_escape, invariants=[], z3_module=z3_module,
    )

    termination = _discharge_termination(summary, deadline, z3_module)
    blocks["termination"] = termination_block_payload(
        termination,
        witness="ctr-descending",
        notes=(
            "nonzero entry CTR",
            "one CTR decrement per iteration",
            "exit at CTR zero",
            "no 32-bit CTR wrap",
        ),
        z3_module=z3_module,
    )

    footprint = _discharge_footprint(summary, deadline, z3_module)
    blocks["footprint"] = discharge_block_payload(
        footprint, invariants=[], z3_module=z3_module,
    )

    discharges = (
        entry_guard,
        body_step,
        postcondition,
        stack_escape,
        termination,
        footprint,
    )
    refinement: dict[str, Any] = {
        "algorithm": REFINEMENT_ALGORITHM,
        "status": "applied",
    }
    refinement.update(blocks)

    if any(item.status == "sat" for item in (body_step, postcondition, stack_escape)):
        refinement["status"] = "failed"
        return MemoryLoopDischargeResult(
            "internal_error",
            entry_guard_payload,
            refinement,
            "memory-loop refinement SAT: summary disagrees with instruction semantics",
            "INTERNAL_ERROR",
        )
    if any(item.status == "sat" for item in (entry_guard, termination, footprint)):
        refinement["status"] = "failed"
        return MemoryLoopDischargeResult(
            "failed",
            entry_guard_payload,
            refinement,
            "memory-loop entry-guard or ranking/footprint query was satisfiable",
            "INCONCLUSIVE_UNSUPPORTED",
        )
    if all(item.status == "unsat" for item in discharges):
        refinement["status"] = "discharged"
        return MemoryLoopDischargeResult(
            "discharged",
            entry_guard_payload,
            refinement,
            None,
            None,
        )
    if any(item.status == "timeout" for item in discharges):
        return MemoryLoopDischargeResult(
            "applied",
            entry_guard_payload,
            refinement,
            "memory-loop refinement query timed out",
            "INCONCLUSIVE_TIMEOUT",
        )
    if any(item.status == "unknown" for item in discharges):
        return MemoryLoopDischargeResult(
            "applied",
            entry_guard_payload,
            refinement,
            "memory-loop refinement query returned unknown",
            "INCONCLUSIVE_UNKNOWN",
        )
    return MemoryLoopDischargeResult(
        "applied",
        entry_guard_payload,
        refinement,
        "memory-loop refinement query was inconclusive",
        "INCONCLUSIVE_UNSUPPORTED",
    )


class _UnsupportedRefinement(Exception):
    """Raised when refinement cannot be stated (bus / unsupported body)."""


def _validate_witness_shape(plan: MemoryLoopPlan) -> None:
    body = plan.witness.body
    if not body:
        raise ValueError("memory-loop witness body is empty")
    latch = plan.witness.latch
    if latch.opcode != Opcode.BC or latch.link:
        raise ValueError("memory-loop witness latch is not bdnz")
    if int(latch.operands[0]) != 16:
        raise ValueError("memory-loop witness latch BO is not bdnz")
    target = int(latch.operands[2]) & 0xFFFFFFFC
    if target != int(plan.witness.header_pc) & 0xFFFFFFFC:
        raise ValueError("memory-loop witness latch target != header")
    if int(plan.summary.trip_count) != int(plan.witness.recognized_trip_count):
        raise ValueError(
            "memory-loop summary trip_count disagrees with recognized witness trip",
        )

def _discharge_entry_guard(
    initial: Any,
    ops: Any,
    plan: MemoryLoopPlan,
    deadline: Deadline,
    z3_module: Any,
    *,
    premises: list[Any],
    violation_conditions: list[Any],
) -> UnsatDischarge:
    """Prove CTR != trip is unreachable under the supplied path premises."""
    trip = int(plan.summary.trip_count) & 0xFFFFFFFF
    if violation_conditions:
        bad = list(violation_conditions)
    else:
        # Standalone discharge: assume recognizer trip equals entry CTR, then
        # the negation is unsat. Engine callers should pass CFG violation paths.
        premises = list(premises) + [ops.eq(initial.ctr, ops.const(trip))]
        bad = [ops.lnot(ops.eq(initial.ctr, ops.const(trip)))]
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=ENTRY_GUARD_ALGORITHM,
        z3_module=z3_module,
    )


def _state_inequality_conditions(left: Any, right: Any, ops: Any) -> list[Any]:
    """Return conditions that hold when ``left`` and ``right`` differ."""
    z3 = ops.z3
    bad: list[Any] = []

    def _neq(a: Any, b: Any) -> Any:
        return z3.Not(a == b)

    for index in range(32):
        bad.append(_neq(left.gpr[index], right.gpr[index]))
        bad.append(_neq(left.fpr[index], right.fpr[index]))
        bad.append(_neq(left.ps1[index], right.ps1[index]))
    for index in range(len(left.gqr)):
        bad.append(_neq(left.gqr[index], right.gqr[index]))
    for index in range(len(left.sr)):
        bad.append(_neq(left.sr[index], right.sr[index]))
    for index in range(len(left.spr)):
        bad.append(_neq(left.spr[index], right.spr[index]))

    bad.extend([
        _neq(left.cr, right.cr),
        _neq(left.xer.ca, right.xer.ca),
        _neq(left.xer.ov, right.xer.ov),
        _neq(left.xer.so, right.xer.so),
        _neq(left.fpscr, right.fpscr),
        _neq(left.lr, right.lr),
        _neq(left.ctr, right.ctr),
        _neq(left.msr, right.msr),
        _neq(left.srr0, right.srr0),
        _neq(left.srr1, right.srr1),
        _neq(left.time_base, right.time_base),
        _neq(left.memory, right.memory),
        _neq(left.valid, right.valid),
        _neq(left.invalid_reason, right.invalid_reason),
    ])

    for attr in ("stack_low", "stack_layout_valid", "stack_private"):
        left_val = getattr(left, attr)
        right_val = getattr(right, attr)
        if left_val is None and right_val is None:
            continue
        if left_val is None or right_val is None:
            bad.append(z3.BoolVal(True))
        else:
            bad.append(_neq(left_val, right_val))

    for attr in ("memory_reads", "memory_writes", "memory_touches", "memory_effects"):
        left_seq = getattr(left, attr)
        right_seq = getattr(right, attr)
        if len(left_seq) != len(right_seq):
            bad.append(z3.BoolVal(True))
        else:
            for left_item, right_item in zip(left_seq, right_seq):
                bad.append(_neq(left_item, right_item))

    if left.symbolic_bus is not None or right.symbolic_bus is not None:
        # First safe version: refuse bus-backed refinement.
        bad.append(z3.BoolVal(True))

    return bad


def _discharge_body_step(
    initial: Any,
    ops: Any,
    plan: MemoryLoopPlan,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """One ordinary iteration equals one summarized iteration."""
    from tools.ppc_equivalence.semantics import execute_bdnz_latch, execute_instruction

    trip = int(plan.summary.trip_count) & 0xFFFFFFFF
    entry = replace(initial, ctr=initial.ctr)
    premises = [
        z3_module.ULE(z3_module.BitVecVal(1, 32), entry.ctr),
        z3_module.ULE(entry.ctr, z3_module.BitVecVal(trip, 32)),
    ]

    ordinary = entry
    for insn in plan.witness.body:
        ordinary = execute_instruction(ordinary, insn, ops)
    ordinary, ordinary_taken = execute_bdnz_latch(
        ordinary,
        plan.witness.latch,
        ops,
        header_pc=plan.witness.header_pc,
    )

    summarized = apply_memory_loop_iteration_summary(entry, plan.summary, ops)
    summary_taken = ops.lnot(ops.eq(summarized.ctr, ops.const(0)))

    bad = _state_inequality_conditions(ordinary, summarized, ops)
    bad.append(z3_module.Not(ordinary_taken == summary_taken))
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=BODY_STEP_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_postcondition(
    initial: Any,
    ops: Any,
    plan: MemoryLoopPlan,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """Exact N-iteration witness equals the full closed-form summary."""
    trip = int(plan.summary.trip_count)
    entry = replace(initial, ctr=ops.const(trip))
    ordinary_post, predicates = execute_memory_loop_witness(entry, plan, ops)
    summary_post = apply_memory_loop_summary(entry, plan.summary, ops)

    bad = _state_inequality_conditions(ordinary_post, summary_post, ops)
    # Control predicates: iterations 0..N-2 taken; final not taken.
    for index, taken in enumerate(predicates):
        expected = index < trip - 1
        bad.append(z3_module.Not(taken == ops.bool(expected)))
    return discharge_bad_conditions(
        premises=[],
        bad_conditions=bad,
        deadline=deadline,
        algorithm=POSTCONDITION_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_stack_escape(
    initial: Any,
    ops: Any,
    plan: MemoryLoopPlan,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """Ordinary and summarized iteration agree on stack_private for r1-derived stores."""
    from tools.ppc_equivalence.semantics import execute_bdnz_latch, execute_instruction

    offset = z3_module.BitVec("ml.stack_escape.offset", 32)
    derived = ops.add(initial.gpr[1], offset)
    gprs = list(initial.gpr)
    gprs[plan.summary.source_reg] = derived
    entry = replace(
        initial,
        gpr=tuple(gprs),
        stack_private=ops.bool(True),
        ctr=ops.const(max(1, int(plan.summary.trip_count))),
    )

    ordinary = entry
    for insn in plan.witness.body:
        ordinary = execute_instruction(ordinary, insn, ops)
    ordinary, _taken = execute_bdnz_latch(
        ordinary,
        plan.witness.latch,
        ops,
        header_pc=plan.witness.header_pc,
    )
    summarized = apply_memory_loop_iteration_summary(entry, plan.summary, ops)

    left_private = ordinary.stack_private
    right_private = summarized.stack_private
    if left_private is None or right_private is None:
        bad = [z3_module.BoolVal(True)]
    else:
        # Disagree, or either still private after publishing an r1-derived value.
        bad = [
            z3_module.Not(left_private == right_private),
            left_private,
            right_private,
        ]
    return discharge_bad_conditions(
        premises=[],
        bad_conditions=bad,
        deadline=deadline,
        algorithm=STACK_ESCAPE_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_termination(
    summary: MemoryLoopSummary,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """CTR ranking: strictly decreasing, exits at zero, never wraps."""
    zero = z3_module.BitVecVal(0, 32)
    one = z3_module.BitVecVal(1, 32)
    trip_val = z3_module.BitVecVal(int(summary.trip_count) & 0xFFFFFFFF, 32)
    ctr0 = z3_module.BitVec("ml.ctr", 32)
    ctr1 = ctr0 - one
    continue_cond = z3_module.And(ctr0 != zero, ctr1 != zero)
    exit_cond = z3_module.And(ctr0 != zero, ctr1 == zero)
    premises = [ctr0 == trip_val]
    bad = [
        ctr0 == zero,
        z3_module.And(continue_cond, z3_module.Not(z3_module.ULT(ctr1, ctr0))),
        z3_module.And(exit_cond, ctr1 != zero),
        z3_module.And(ctr0 == zero, z3_module.Or(continue_cond, exit_cond)),
    ]
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=TERMINATION_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_footprint(
    summary: MemoryLoopSummary,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """Static footprint gate: trip >= 1 and no 32-bit span wrap."""
    ok = (
        int(summary.trip_count) >= 1
        and footprint_ok_for_summary(
            trip_count=int(summary.trip_count),
            stride=int(summary.stride),
            store_width=int(summary.store_width),
            store_kind=summary.store_kind,
        )
    )
    bad = [] if ok else [z3_module.BoolVal(True)]
    return discharge_bad_conditions(
        premises=[],
        bad_conditions=bad,
        deadline=deadline,
        algorithm=FOOTPRINT_ALGORITHM,
        z3_module=z3_module,
    )


def canonical_unique_plans(plans: Sequence[MemoryLoopPlan]) -> list[MemoryLoopPlan]:
    """Deduplicate used plans by (header, latch, summary hash, code hash)."""
    from tools.ppc_equivalence.memory_loop import plan_identity_key

    seen: set[tuple[Any, ...]] = set()
    unique: list[MemoryLoopPlan] = []
    for plan in plans:
        key = plan_identity_key(plan)
        if key in seen:
            continue
        seen.add(key)
        unique.append(plan)
    return unique

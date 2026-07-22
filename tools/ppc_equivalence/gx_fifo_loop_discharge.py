"""Per-side refinement discharge for recognized GX FIFO write-emission loops.

Recognizing a natural single-header CTR loop whose body emits one GX FIFO
write (``gx_fifo_loop.recognize_gx_fifo_loops`` / ``build_gx_fifo_loop_plan``)
and applying its closed-form ``RepeatedEmission`` summary is **not** a proof.
Per ``GX_FIFO_TIER_A.md`` (``mmio-loop-emission-v1``), a GX-FIFO-loop
obligation may only carry ``status=discharged`` when every used plan on each
side discharges:

``entry_guard``
    Under caller-supplied path premises, ``CTR != summarized trip count`` is
    unreachable; no unguarded zero/wrap.
``body_step``
    One ordinary witness iteration (real store [+ affine ``addi``] + ``bdnz``)
    equals one summarized iteration: registers, CTR, and the latch taken
    predicate agree.
``event_identity``
    The real store's emitted address/width/value agree byte-for-byte with the
    closed-form per-iteration template (device id, single-write order, and
    cursor advance are structural — guaranteed by witness-shape validation —
    and are recorded as notes, not solver queries).
``postcondition``
    Exact ``N``-iteration witness execution equals the full closed-form
    summary (``apply_gx_fifo_loop_summary``).
``termination``
    CTR ranking: strictly decreasing, exits at zero, never wraps.
``bound``
    ``N * writes_per_iteration <= max_events``.
``footprint``
    Static shape gate: single narrow store body, no unmodeled RAM/MMIO,
    reads, calls, DMA, interrupts, or extra FIFO writes.

Refinement SAT (on ``body_step`` / ``event_identity`` / ``postcondition``) is
an ``INTERNAL_ERROR`` (the summary disagrees with instruction semantics),
never ``NOT_EQUIVALENT``. Solver timeout / unknown leave the plan ``applied``.
This module never authorizes original-vs-candidate equivalence — each side
only proves its own instructions ``≡`` its own summary.

The GX write-gather pipe is modeled here without an active ``MemoryBus``: the
closed-form summary (``apply_gx_fifo_loop_summary``) intentionally never
mutates ``state.memory`` (a FIFO write is not a RAM write), while ordinary
``execute_instruction`` *does* write the concrete GX address into
``state.memory`` absent a live device route. State comparisons in this module
therefore deliberately exclude ``memory`` / ``memory_reads`` /
``memory_writes`` / ``memory_touches`` / ``memory_effects`` /
``stack_low`` / ``stack_layout_valid`` / ``stack_private`` / ``symbolic_bus``
— those fields are not part of the GX-FIFO-loop contract (see
``GX_FIFO_TIER_A.md``; unlike ``memory_loop_discharge``, this v1 domain has no
required ``stack_escape`` block). ``event_identity`` instead independently
proves the emitted byte-level address/value footprint of the real store.
"""

from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any, Sequence

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.discharge import UnsatDischarge, discharge_bad_conditions
from tools.ppc_equivalence.gx_fifo_loop import (
    GxFifoLoopPlan,
    GxFifoLoopSummary,
    apply_gx_fifo_loop_summary,
)
from tools.ppc_equivalence.ir import ExecutionInconclusive, Opcode
from tools.ppc_equivalence.relational_discharge import (
    discharge_block_payload,
    termination_block_payload,
)

ENTRY_GUARD_ALGORITHM = "gx-fifo-loop-entry-ctr-v2"
BODY_STEP_ALGORITHM = "gx-fifo-loop-body-step-v2"
EVENT_IDENTITY_ALGORITHM = "gx-fifo-loop-event-identity-v2"
POSTCONDITION_ALGORITHM = "gx-fifo-loop-postcondition-v2"
TERMINATION_ALGORITHM = "gx-fifo-loop-ctr-termination-v2"
BOUND_ALGORITHM = "gx-fifo-loop-bound-v2"
FOOTPRINT_ALGORITHM = "gx-fifo-loop-footprint-v2"

REFINEMENT_ALGORITHM = "gx-fifo-loop-exact-refinement-v2"

REQUIRED_REFINEMENT_BLOCKS: tuple[str, ...] = (
    "body_step",
    "event_identity",
    "postcondition",
    "termination",
    "bound",
    "footprint",
)

KNOWN_REFINEMENT_ALGORITHMS: frozenset[str] = frozenset({
    BODY_STEP_ALGORITHM,
    EVENT_IDENTITY_ALGORITHM,
    POSTCONDITION_ALGORITHM,
    TERMINATION_ALGORITHM,
    BOUND_ALGORITHM,
    FOOTPRINT_ALGORITHM,
})

# The old digest-only shape from GX_FIFO_TIER_A.md's rollout notes; never
# promotion-grade regardless of what a caller injects for its blocks.
STALE_REFINEMENT_ALGORITHMS: frozenset[str] = frozenset({"gx-fifo-loop-refinement-v1"})

_PLACEHOLDER_DIGEST = "0" * 64


@dataclass(frozen=True)
class GxFifoLoopDischargeResult:
    """Per-plan refinement discharge outcome."""

    status: str  # "discharged" | "failed" | "applied" | "unsupported" | "internal_error"
    entry_guard: dict[str, Any]
    refinement: dict[str, Any]
    reason: str | None = None
    proof_status_hint: str | None = None  # INTERNAL_ERROR | INCONCLUSIVE_*

    def all_unsat(self) -> bool:
        return self.status == "discharged"


def refinement_for_plan(
    plan: GxFifoLoopPlan,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
    entry_guard_premises: Sequence[Any] = (),
    entry_violation_conditions: Sequence[Any] = (),
    require_entry_violations: bool = True,
) -> dict[str, Any] | None:
    """Discharge ``plan`` and return the attachable side-entry payload pieces."""
    result = discharge_gx_fifo_loop_plan(
        plan,
        deadline=deadline,
        z3_module=z3_module,
        entry_guard_premises=entry_guard_premises,
        entry_violation_conditions=entry_violation_conditions,
        require_entry_violations=require_entry_violations,
    )
    if not result.all_unsat():
        return None
    return {
        "entry_guard": result.entry_guard,
        "refinement": result.refinement,
    }


class _UnsupportedRefinement(Exception):
    """Raised when refinement cannot be stated (unsupported witness shape)."""


def _fresh_symbolic_state() -> tuple[Any, Any]:
    from tools.ppc_equivalence.engine import _symbolic_initial
    from tools.ppc_equivalence.semantics import SymbolicOps

    ops = SymbolicOps()
    return _symbolic_initial(ops), ops


def _sign_extend16(value: int) -> int:
    word = int(value) & 0xFFFF
    if word >= 0x8000:
        word -= 0x10000
    return word & 0xFFFFFFFF


def _value_expr_reg(summary: GxFifoLoopSummary) -> int:
    reg = summary.value_expr.get("reg")
    if reg is None:
        raise _UnsupportedRefinement("gx-fifo-loop summary.value_expr is missing 'reg'")
    return int(reg)


def _validate_witness_shape(plan: GxFifoLoopPlan) -> None:
    witness = plan.witness
    summary = plan.summary
    body = witness.body
    if not body:
        raise ValueError("gx-fifo-loop witness body is empty")
    if len(body) not in (1, 2):
        raise ValueError("gx-fifo-loop witness body must be store or store-then-affine-addi")

    store_insn = body[0]
    width_by_opcode = {Opcode.STB: 1, Opcode.STH: 2, Opcode.STW: 4}
    if store_insn.opcode not in width_by_opcode:
        raise ValueError("gx-fifo-loop witness first instruction must be stb/sth/stw")
    if width_by_opcode[store_insn.opcode] != int(summary.width):
        raise ValueError("gx-fifo-loop witness store width disagrees with summary.width")
    source_reg, base_reg, _disp = (int(v) for v in store_insn.operands)
    if source_reg == base_reg:
        raise ValueError("gx-fifo-loop witness store source equals base register")
    if source_reg != _value_expr_reg(summary):
        raise ValueError("gx-fifo-loop witness store source disagrees with summary.value_expr")

    if len(body) == 2:
        addi_insn = body[1]
        if addi_insn.opcode != Opcode.ADDI:
            raise ValueError("gx-fifo-loop witness second instruction must be a value-affine addi")
        rt, ra, _imm = (int(v) for v in addi_insn.operands)
        if rt != source_reg or ra != source_reg:
            raise ValueError(
                "gx-fifo-loop witness affine addi does not self-increment the store source",
            )
        if rt == base_reg:
            raise ValueError("gx-fifo-loop witness affine addi must not target the base register")
        if summary.value_kind != "affine":
            raise ValueError(
                "gx-fifo-loop witness has a trailing addi but summary.value_kind != 'affine'",
            )
    elif summary.value_kind != "invariant":
        raise ValueError(
            "gx-fifo-loop witness has no trailing addi but summary.value_kind != 'invariant'",
        )

    latch = witness.latch
    if latch.opcode != Opcode.BC or latch.link:
        raise ValueError("gx-fifo-loop witness latch is not bdnz")
    if int(latch.operands[0]) != 16:
        raise ValueError("gx-fifo-loop witness latch BO is not bdnz")
    target = int(latch.operands[2]) & 0xFFFFFFFC
    if target != int(witness.header_pc) & 0xFFFFFFFC:
        raise ValueError("gx-fifo-loop witness latch target != header")
    if int(summary.trip_count) != int(witness.recognized_trip_count):
        raise ValueError(
            "gx-fifo-loop summary trip_count disagrees with recognized witness trip",
        )
    if summary.writes_per_iteration != 1:
        raise ValueError("gx-fifo-loop v1 refinement supports writes_per_iteration == 1 only")


def _base_register_premise(entry: Any, plan: GxFifoLoopPlan, ops: Any) -> Any | None:
    """Bind the store's base register to the address recognition already proved.

    Recognition (``recover_gpr_constant``'s bounded straight-line
    materialization, outside this witness) deterministically established that
    the base register holds ``summary.address - sign_extend(disp)`` at loop
    entry. The witness replayed here starts at the header, so that fact is
    recorded as an explicit premise rather than re-derived from instructions
    this discharge does not have.
    """
    store_insn = plan.witness.body[0]
    _source_reg, base_reg, disp = (int(v) for v in store_insn.operands)
    if base_reg == 0:
        return None
    required = (int(plan.summary.address) - _sign_extend16(disp)) & 0xFFFFFFFF
    return ops.eq(entry.gpr[base_reg], ops.const(required))


def _restricted_state_inequality_conditions(left: Any, right: Any, ops: Any) -> list[Any]:
    """Bad-condition terms for observable state, excluding memory/stack/bus.

    The GX write-gather pipe is not part of the RAM/stack model (see module
    docstring); this intentionally mirrors
    ``memory_loop_discharge._state_inequality_conditions`` minus the fields
    that a device write does not own.
    """
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
        _neq(left.valid, right.valid),
        _neq(left.invalid_reason, right.invalid_reason),
    ])
    return bad


def _iteration_reference_state(
    entry: Any,
    summary: GxFifoLoopSummary,
    ops: Any,
) -> tuple[Any, Any]:
    """One-iteration closed-form register/CTR transition (no memory write).

    Mirrors ``apply_gx_fifo_loop_summary``'s GPR update for exactly one
    iteration: the source register advances by one affine step (or stays
    invariant), and CTR decrements by exactly one instead of collapsing to
    ``final_ctr``.
    """
    source_reg = _value_expr_reg(summary)
    write_value = entry.gpr[source_reg]
    gprs = list(entry.gpr)
    if summary.value_kind == "affine":
        step = int(summary.value_expr.get("step", 0)) & 0xFFFFFFFF
        gprs[source_reg] = ops.add(write_value, ops.const(step))
    elif summary.value_kind != "invariant":
        raise _UnsupportedRefinement(f"unsupported value_kind {summary.value_kind!r}")
    stepped = replace(entry, gpr=tuple(gprs), ctr=ops.sub(entry.ctr, ops.const(1)))
    return stepped, write_value


def _discharge_entry_guard(
    initial: Any,
    ops: Any,
    plan: GxFifoLoopPlan,
    deadline: Deadline,
    z3_module: Any,
    *,
    premises: list[Any],
    violation_conditions: list[Any],
    allow_standalone_assumption: bool = False,
) -> UnsatDischarge:
    """Prove CTR != trip is unreachable under the supplied path premises.

    Empty ``violation_conditions`` is refused unless
    ``allow_standalone_assumption`` is set (unit tests only). The standalone
    path assumes recognizer trip equals entry CTR; that must never authorize
    engine EQUIVALENT on its own.
    """
    trip = int(plan.summary.trip_count) & 0xFFFFFFFF
    if violation_conditions:
        bad = list(violation_conditions)
    elif allow_standalone_assumption:
        premises = list(premises) + [ops.eq(initial.ctr, ops.const(trip))]
        bad = [ops.lnot(ops.eq(initial.ctr, ops.const(trip)))]
    else:
        raise ValueError(
            "gx-fifo-loop entry violation conditions required; "
            "refusing vacuous entry_guard discharge"
        )
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=ENTRY_GUARD_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_body_step(
    initial: Any,
    ops: Any,
    plan: GxFifoLoopPlan,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """One ordinary iteration (real store [+ affine] + bdnz) equals one summarized iteration."""
    from tools.ppc_equivalence.semantics import execute_bdnz_latch, execute_instruction

    trip = int(plan.summary.trip_count) & 0xFFFFFFFF
    entry = initial
    premises = [
        z3_module.ULE(z3_module.BitVecVal(1, 32), entry.ctr),
        z3_module.ULE(entry.ctr, z3_module.BitVecVal(trip, 32)),
    ]
    base_premise = _base_register_premise(entry, plan, ops)
    if base_premise is not None:
        premises.append(base_premise)

    ordinary = entry
    for insn in plan.witness.body:
        ordinary = execute_instruction(ordinary, insn, ops)
    ordinary, ordinary_taken = execute_bdnz_latch(
        ordinary,
        plan.witness.latch,
        ops,
        header_pc=plan.witness.header_pc,
    )

    summarized, _write_value = _iteration_reference_state(entry, plan.summary, ops)
    summary_taken = ops.lnot(ops.eq(summarized.ctr, ops.const(0)))

    bad = _restricted_state_inequality_conditions(ordinary, summarized, ops)
    bad.append(z3_module.Not(ordinary_taken == summary_taken))
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=BODY_STEP_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_event_identity(
    initial: Any,
    ops: Any,
    plan: GxFifoLoopPlan,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """The real store's emitted (address, width, value) matches the summary template.

    Executes only the witness's store instruction (the affine ``addi``, if
    any, does not touch memory) and proves the newly touched byte addresses
    are exactly ``[summary.address, summary.address + width)`` with content
    equal to the entry source-register value, byte for byte and big-endian —
    the same value template ``apply_gx_fifo_loop_summary`` records in its
    ``RepeatedEmission``/``SymbolicFifoEvent`` descriptor. Device id, write
    order (exactly one write group per iteration), and cursor advance are
    structural facts already enforced by ``_validate_witness_shape`` /
    recognition and are recorded as notes on the returned block, not further
    SMT queries.
    """
    from tools.ppc_equivalence.semantics import execute_instruction

    summary = plan.summary
    trip = int(summary.trip_count) & 0xFFFFFFFF
    entry = initial
    premises = [
        z3_module.ULE(z3_module.BitVecVal(1, 32), entry.ctr),
        z3_module.ULE(entry.ctr, z3_module.BitVecVal(trip, 32)),
    ]
    base_premise = _base_register_premise(entry, plan, ops)
    if base_premise is not None:
        premises.append(base_premise)

    source_reg = _value_expr_reg(summary)
    entry_value = entry.gpr[source_reg]

    baseline_writes = len(entry.memory_writes)
    store_insn = plan.witness.body[0]
    stored = execute_instruction(entry, store_insn, ops)

    width = int(summary.width)
    new_writes = stored.memory_writes[baseline_writes:baseline_writes + width]

    bad: list[Any] = []
    if len(new_writes) != width:
        bad.append(z3_module.BoolVal(True))
    else:
        for offset in range(width):
            expected_addr = ops.const((int(summary.address) + offset) & 0xFFFFFFFF)
            bad.append(z3_module.Not(ops.eq(new_writes[offset], expected_addr)))
            shift = (width - 1 - offset) * 8
            expected_byte = ops.band(ops.lshr(entry_value, ops.const(shift)), ops.const(0xFF))
            actual_byte = ops.load_byte(stored.memory, new_writes[offset])
            bad.append(z3_module.Not(ops.eq(actual_byte, expected_byte)))
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=EVENT_IDENTITY_ALGORITHM,
        z3_module=z3_module,
    )


def _execute_gx_fifo_loop_witness(
    entry: Any,
    plan: GxFifoLoopPlan,
    ops: Any,
) -> tuple[Any, tuple[Any, ...]]:
    """Execute the recognized witness exactly ``trip_count`` iterations."""
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


def _discharge_postcondition(
    initial: Any,
    ops: Any,
    plan: GxFifoLoopPlan,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """Exact N-iteration witness equals the full closed-form summary (registers/CTR)."""
    trip = int(plan.summary.trip_count)
    entry = replace(initial, ctr=ops.const(trip))
    premises: list[Any] = []
    base_premise = _base_register_premise(entry, plan, ops)
    if base_premise is not None:
        premises.append(base_premise)

    ordinary_post, predicates = _execute_gx_fifo_loop_witness(entry, plan, ops)
    summary_post, _descriptor = apply_gx_fifo_loop_summary(entry, plan, ops)

    bad = _restricted_state_inequality_conditions(ordinary_post, summary_post, ops)
    for index, taken in enumerate(predicates):
        expected = index < trip - 1
        bad.append(z3_module.Not(taken == ops.bool(expected)))
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=POSTCONDITION_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_termination(
    summary: GxFifoLoopSummary,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """CTR ranking: strictly decreasing, exits at zero, never wraps."""
    zero = z3_module.BitVecVal(0, 32)
    one = z3_module.BitVecVal(1, 32)
    trip_val = z3_module.BitVecVal(int(summary.trip_count) & 0xFFFFFFFF, 32)
    ctr0 = z3_module.BitVec("gxl.ctr", 32)
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


def _discharge_bound(
    summary: GxFifoLoopSummary,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """Static bound gate: N * writes_per_iteration <= max_events."""
    ok = (
        int(summary.trip_count) >= 1
        and int(summary.writes_per_iteration) >= 1
        and int(summary.trip_count) * int(summary.writes_per_iteration) <= int(summary.max_events)
    )
    bad = [] if ok else [z3_module.BoolVal(True)]
    return discharge_bad_conditions(
        premises=[],
        bad_conditions=bad,
        deadline=deadline,
        algorithm=BOUND_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_footprint(
    plan: GxFifoLoopPlan,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """Static footprint gate: single narrow store body, no other effects."""
    summary = plan.summary
    ok = (
        summary.width in (1, 2, 4)
        and int(summary.address) % int(summary.width) == 0
        and summary.writes_per_iteration == 1
        and len(plan.witness.body) in (1, 2)
        and int(summary.trip_count) >= 1
        and summary.value_kind in ("invariant", "affine")
    )
    bad = [] if ok else [z3_module.BoolVal(True)]
    return discharge_bad_conditions(
        premises=[],
        bad_conditions=bad,
        deadline=deadline,
        algorithm=FOOTPRINT_ALGORITHM,
        z3_module=z3_module,
    )


def discharge_gx_fifo_loop_plan(
    plan: GxFifoLoopPlan,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
    entry_guard_premises: Sequence[Any] = (),
    entry_violation_conditions: Sequence[Any] = (),
    require_entry_violations: bool = True,
) -> GxFifoLoopDischargeResult:
    """Prove entry-guard + refinement queries for one recognized GX FIFO loop plan.

    ``require_entry_violations`` defaults True so engine authorization cannot
    discharge via a vacuous/synthesized CTR equality. Unit tests that exercise
    refinement without a CFG may pass ``require_entry_violations=False``.
    """
    summary = plan.summary
    if summary.trip_count < 1:
        return GxFifoLoopDischargeResult(
            "applied",
            {},
            {},
            "non-positive concrete trip count",
            "INCONCLUSIVE_UNSUPPORTED",
        )
    if int(summary.trip_count) * int(summary.writes_per_iteration) > int(summary.max_events):
        return GxFifoLoopDischargeResult(
            "failed",
            {},
            {},
            "gx-fifo-loop bound exceeded: N * writes_per_iteration > max_events",
            "INCONCLUSIVE_UNSUPPORTED",
        )
    if summary.value_kind not in ("invariant", "affine"):
        return GxFifoLoopDischargeResult(
            "unsupported",
            {},
            {},
            f"unsupported value_kind {summary.value_kind!r}",
            "INCONCLUSIVE_UNSUPPORTED",
        )

    try:
        _validate_witness_shape(plan)
    except ValueError as exc:
        return GxFifoLoopDischargeResult(
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
        return GxFifoLoopDischargeResult(
            "unsupported",
            {},
            {},
            "gx-fifo-loop refinement rejects an active memory bus",
            "INCONCLUSIVE_UNSUPPORTED",
        )

    if require_entry_violations and not entry_violation_conditions:
        return GxFifoLoopDischargeResult(
            "applied",
            {},
            {},
            "gx-fifo-loop entry violation conditions required; "
            "refusing vacuous entry_guard discharge",
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
        allow_standalone_assumption=not require_entry_violations,
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
        return GxFifoLoopDischargeResult(
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
        event_identity = _discharge_event_identity(initial, ops, plan, deadline, z3_module)
    except (_UnsupportedRefinement, ValueError, ExecutionInconclusive) as exc:
        return GxFifoLoopDischargeResult(
            "unsupported",
            entry_guard_payload,
            {"algorithm": REFINEMENT_ALGORITHM, "status": "unsupported", **blocks},
            str(exc),
            "INCONCLUSIVE_UNSUPPORTED",
        )
    blocks["event_identity"] = discharge_block_payload(
        event_identity,
        invariants=[],
        z3_module=z3_module,
        extra={
            "device_id": summary.device_id,
            "order": "single-write-group-per-iteration",
            "cursor": "event-count-advances-by-writes_per_iteration",
        },
    )

    try:
        postcondition = _discharge_postcondition(initial, ops, plan, deadline, z3_module)
    except (_UnsupportedRefinement, ValueError, ExecutionInconclusive) as exc:
        return GxFifoLoopDischargeResult(
            "unsupported",
            entry_guard_payload,
            {"algorithm": REFINEMENT_ALGORITHM, "status": "unsupported", **blocks},
            str(exc),
            "INCONCLUSIVE_UNSUPPORTED",
        )
    blocks["postcondition"] = discharge_block_payload(
        postcondition, invariants=[], z3_module=z3_module,
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

    bound = _discharge_bound(summary, deadline, z3_module)
    blocks["bound"] = discharge_block_payload(
        bound,
        invariants=[],
        z3_module=z3_module,
        extra={
            "trip_count": int(summary.trip_count),
            "writes_per_iteration": int(summary.writes_per_iteration),
            "max_events": int(summary.max_events),
        },
    )

    footprint = _discharge_footprint(plan, deadline, z3_module)
    blocks["footprint"] = discharge_block_payload(
        footprint, invariants=[], z3_module=z3_module,
    )

    discharges = (
        entry_guard,
        body_step,
        event_identity,
        postcondition,
        termination,
        bound,
        footprint,
    )
    refinement: dict[str, Any] = {
        "algorithm": REFINEMENT_ALGORITHM,
        "status": "applied",
    }
    refinement.update(blocks)

    if any(item.status == "sat" for item in (body_step, event_identity, postcondition)):
        refinement["status"] = "failed"
        return GxFifoLoopDischargeResult(
            "internal_error",
            entry_guard_payload,
            refinement,
            "gx-fifo-loop refinement SAT: summary disagrees with instruction semantics",
            "INTERNAL_ERROR",
        )
    if any(item.status == "sat" for item in (entry_guard, termination, bound, footprint)):
        refinement["status"] = "failed"
        return GxFifoLoopDischargeResult(
            "failed",
            entry_guard_payload,
            refinement,
            "gx-fifo-loop entry-guard or ranking/bound/footprint query was satisfiable",
            "INCONCLUSIVE_UNSUPPORTED",
        )
    if all(item.status == "unsat" for item in discharges):
        refinement["status"] = "discharged"
        return GxFifoLoopDischargeResult(
            "discharged",
            entry_guard_payload,
            refinement,
            None,
            None,
        )
    if any(item.status == "timeout" for item in discharges):
        return GxFifoLoopDischargeResult(
            "applied",
            entry_guard_payload,
            refinement,
            "gx-fifo-loop refinement query timed out",
            "INCONCLUSIVE_TIMEOUT",
        )
    if any(item.status == "unknown" for item in discharges):
        return GxFifoLoopDischargeResult(
            "applied",
            entry_guard_payload,
            refinement,
            "gx-fifo-loop refinement query returned unknown",
            "INCONCLUSIVE_UNKNOWN",
        )
    return GxFifoLoopDischargeResult(
        "applied",
        entry_guard_payload,
        refinement,
        "gx-fifo-loop refinement query was inconclusive",
        "INCONCLUSIVE_UNSUPPORTED",
    )


def _validate_query_digest(label: str, digest: Any) -> str | None:
    if not isinstance(digest, str) or len(digest) != 64 or digest != digest.lower():
        return f"{label} must be a lowercase 64-hex digest"
    try:
        int(digest, 16)
    except ValueError:
        return f"{label} must be a lowercase 64-hex digest"
    if digest == _PLACEHOLDER_DIGEST:
        return f"{label} must not be a placeholder digest"
    return None


def validate_gx_fifo_loop_refinement_v2(obligation: dict[str, Any]) -> str | None:
    """Return ``None`` when a ``refinement`` payload is structurally well-formed.

    Rejects: non-recomputable / placeholder (``"0" * 64``) digests, unknown
    block algorithms, any legacy ``-v1`` block or top-level algorithm name
    (``gx-fifo-loop-refinement-v1`` is never promotion-grade — see
    ``GX_FIFO_TIER_A.md``), and a ``status=discharged`` claim whose blocks are
    not all ``result=unsat``.
    """
    if not isinstance(obligation, dict):
        return "gx_fifo_loop refinement must be an object"

    algorithm = obligation.get("algorithm")
    if algorithm in STALE_REFINEMENT_ALGORITHMS or (
        isinstance(algorithm, str) and algorithm.endswith("-v1")
    ):
        return (
            f"gx_fifo_loop refinement.algorithm {algorithm!r} is a stale v1 shape "
            "and is never promotion-grade"
        )
    if algorithm != REFINEMENT_ALGORITHM:
        return f"gx_fifo_loop refinement.algorithm must be {REFINEMENT_ALGORITHM!r}"

    status = obligation.get("status")
    if status not in ("discharged", "applied", "failed", "unsupported"):
        return (
            "gx_fifo_loop refinement.status must be "
            "discharged|applied|failed|unsupported"
        )
    require_discharged = status == "discharged"

    for name in REQUIRED_REFINEMENT_BLOCKS:
        block = obligation.get(name)
        if not isinstance(block, dict):
            if require_discharged:
                return f"gx_fifo_loop refinement.{name} block is required"
            continue

        block_algorithm = block.get("algorithm")
        if block_algorithm in STALE_REFINEMENT_ALGORITHMS or (
            isinstance(block_algorithm, str) and block_algorithm.endswith("-v1")
        ):
            return (
                f"gx_fifo_loop refinement.{name}.algorithm {block_algorithm!r} "
                "must not be a legacy v1 algorithm"
            )
        if block_algorithm not in KNOWN_REFINEMENT_ALGORITHMS:
            return (
                f"gx_fifo_loop refinement.{name}.algorithm {block_algorithm!r} "
                "is not a known refinement algorithm"
            )

        digest = block.get("query_sha256")
        if digest is not None or require_discharged:
            reason = _validate_query_digest(
                f"gx_fifo_loop refinement.{name}.query_sha256", digest,
            )
            if reason is not None:
                return reason

        result = block.get("result")
        if require_discharged and result != "unsat":
            return f"gx_fifo_loop refinement.{name}.result must be 'unsat'"
        if result is not None and result not in ("unsat", "sat", "unknown", "timeout"):
            return (
                f"gx_fifo_loop refinement.{name}.result must be "
                "unsat|sat|unknown|timeout"
            )
    return None

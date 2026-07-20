"""Independent transition-equivalence discharge for closed-form memory loops.

Recognizing a constant-stride store loop and applying its closed-form summary is
**not** a proof. A memory-loop obligation may only carry ``status=discharged``
(and therefore authorize ``EQUIVALENT``) when every query below returns UNSAT
under an independent solver run that never includes the main equivalence
mismatch formula:

``body_step``
    One typed store plus the base/CTR update agrees with the summary metadata.
``postcondition``
    Applying the closed-form transitions from equal entry states yields equal
    exit states (base GPR, CTR, validity, invalid reason, stack-private flag,
    memory on the written footprint, and the recorded memory writes/touches).
``stack_escape``
    Storing an ``r1``-derived value through a summarized store clears the
    private-stack flag exactly as an ordinary store would.
``termination``
    The CTR ranking decreases by one per iteration and never wraps (reuses the
    relational CTR termination corner cases).
``footprint``
    The concrete trip count is >= 1 and the static footprint gate holds without
    32-bit span wrap.

This mirrors ``relational_discharge`` (``discharge_bad_conditions`` /
``discharge_block_payload`` / ``termination_block_payload``); recognition or
``coverage=applied`` alone never authorizes ``EQUIVALENT``.
"""

from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.discharge import UnsatDischarge, discharge_bad_conditions
from tools.ppc_equivalence.memory_loop import MemoryLoopSummary, apply_memory_loop_summary
from tools.ppc_equivalence.memory_semantics import (
    build_memory_loop_transition,
    apply_memory_loop_transition,
    footprint_ok_for_summary,
)
from tools.ppc_equivalence.relational_discharge import (
    discharge_block_payload,
    termination_block_payload,
)

BODY_STEP_ALGORITHM = "memory-loop-body-step-v1"
POSTCONDITION_ALGORITHM = "memory-loop-postcondition-v1"
STACK_ESCAPE_ALGORITHM = "memory-loop-stack-escape-v1"
TERMINATION_ALGORITHM = "memory-loop-ctr-termination-v1"
FOOTPRINT_ALGORITHM = "memory-loop-footprint-v1"

TRANSITION_EQUIVALENCE_ALGORITHM = "memory-loop-transition-equivalence-v1"

# Blocks that must all be UNSAT for a discharged transition-equivalence proof.
REQUIRED_TRANSITION_BLOCKS: tuple[str, ...] = (
    "body_step",
    "postcondition",
    "stack_escape",
    "termination",
    "footprint",
)

KNOWN_TRANSITION_ALGORITHMS: frozenset[str] = frozenset({
    BODY_STEP_ALGORITHM,
    POSTCONDITION_ALGORITHM,
    STACK_ESCAPE_ALGORITHM,
    TERMINATION_ALGORITHM,
    FOOTPRINT_ALGORITHM,
})


@dataclass(frozen=True)
class MemoryLoopDischargeResult:
    """Per-query discharge outcome for a closed-form memory-loop transition."""

    status: str  # "discharged" | "failed" | "applied" | "unsupported"
    blocks: dict[str, dict[str, Any]]
    reason: str | None = None

    def all_unsat(self) -> bool:
        return self.status == "discharged"

    def transition_equivalence(self) -> dict[str, Any] | None:
        """The ``transition_equivalence`` payload to attach to the obligation."""
        if not self.blocks:
            return None
        payload: dict[str, Any] = {
            "algorithm": TRANSITION_EQUIVALENCE_ALGORITHM,
            "status": self.status,
        }
        payload.update(self.blocks)
        return payload


def transition_equivalence_for_summary(
    summary: MemoryLoopSummary,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> dict[str, Any] | None:
    """Discharge ``summary`` against itself and return the attachable payload.

    Convenience for building a discharged obligation from a single closed-form
    summary (both sides identical). Returns ``None`` when the summary cannot be
    discharged (e.g. bounded-remainder expansion).
    """
    result = discharge_memory_loop_summary(
        summary, summary, deadline=deadline, z3_module=z3_module,
    )
    if not result.all_unsat():
        return None
    return result.transition_equivalence()


def _summaries_match(original: MemoryLoopSummary, candidate: MemoryLoopSummary) -> bool:
    keys = (
        "trip_count",
        "base_reg",
        "source_reg",
        "stride",
        "store_width",
        "store_kind",
        "final_ctr",
        "expansion",
    )
    return all(getattr(original, key) == getattr(candidate, key) for key in keys)


def _fresh_symbolic_state() -> Any:
    # Lazy import breaks the engine -> memory_loop_discharge import cycle.
    from tools.ppc_equivalence.engine import _symbolic_initial
    from tools.ppc_equivalence.semantics import SymbolicOps

    ops = SymbolicOps()
    return _symbolic_initial(ops), ops


def discharge_memory_loop_summary(
    original: MemoryLoopSummary,
    candidate: MemoryLoopSummary,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> MemoryLoopDischargeResult:
    """Prove the five transition-equivalence queries for a closed-form loop pair.

    ``status=discharged`` only when *every* query returns UNSAT. Any SAT query
    fails closed (``failed``); solver ``unknown`` / ``timeout`` / unsupported
    premises stay ``applied`` (execute, but never authorize ``EQUIVALENT``).
    """
    if original.expansion != "closed-form" or candidate.expansion != "closed-form":
        return MemoryLoopDischargeResult(
            "applied", {}, "bounded-remainder expansion is not closed-form",
        )
    if original.trip_count < 1 or candidate.trip_count < 1:
        return MemoryLoopDischargeResult(
            "applied", {}, "non-positive concrete trip count",
        )
    if not _summaries_match(original, candidate):
        return MemoryLoopDischargeResult(
            "failed", {}, "original and candidate summaries do not match",
        )
    # Structural body-step preconditions (stride == width, supported store kind).
    if original.stride != original.store_width:
        return MemoryLoopDischargeResult(
            "failed", {}, "stride does not equal store width",
        )
    if original.store_kind not in ("stwu", "d-form-addi"):
        return MemoryLoopDischargeResult(
            "failed", {}, f"unsupported store kind {original.store_kind!r}",
        )

    if z3_module is None:
        import z3 as z3_module  # type: ignore[no-redef]
    if deadline is None:
        deadline = Deadline.after_ms(15_000)

    initial, ops = _fresh_symbolic_state()

    blocks: dict[str, dict[str, Any]] = {}

    body_step = _discharge_body_step(initial, ops, original, deadline, z3_module)
    blocks["body_step"] = discharge_block_payload(
        body_step, invariants=[], z3_module=z3_module,
    )

    postcondition = _discharge_postcondition(
        initial, ops, original, candidate, deadline, z3_module,
    )
    blocks["postcondition"] = discharge_block_payload(
        postcondition, invariants=[], z3_module=z3_module,
    )

    stack_escape = _discharge_stack_escape(initial, ops, original, deadline, z3_module)
    blocks["stack_escape"] = discharge_block_payload(
        stack_escape, invariants=[], z3_module=z3_module,
    )

    termination = _discharge_termination(original, deadline, z3_module)
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

    footprint = _discharge_footprint(original, deadline, z3_module)
    blocks["footprint"] = discharge_block_payload(
        footprint, invariants=[], z3_module=z3_module,
    )

    discharges = (body_step, postcondition, stack_escape, termination, footprint)
    if all(item.status == "unsat" for item in discharges):
        status = "discharged"
        reason = None
    elif any(item.status == "sat" for item in discharges):
        status = "failed"
        reason = "a transition-equivalence query was satisfiable (bad reachable)"
    else:
        status = "applied"
        reason = "a transition-equivalence query was inconclusive (unknown/timeout)"
    return MemoryLoopDischargeResult(status, blocks, reason)


def _observable_equality(left: Any, right: Any, base_reg: int, ops: Any) -> list[Any]:
    z3 = ops.z3
    parts: list[Any] = [
        left.gpr[base_reg] == right.gpr[base_reg],
        left.ctr == right.ctr,
        left.valid == right.valid,
        left.invalid_reason == right.invalid_reason,
        left.stack_private == right.stack_private,
        left.memory == right.memory,
    ]
    if len(left.memory_writes) == len(right.memory_writes):
        parts.extend(
            lw == rw for lw, rw in zip(left.memory_writes, right.memory_writes)
        )
    else:
        parts.append(z3.BoolVal(False))
    if len(left.memory_touches) == len(right.memory_touches):
        parts.extend(
            lt == rt for lt, rt in zip(left.memory_touches, right.memory_touches)
        )
    else:
        parts.append(z3.BoolVal(False))
    return parts


def _discharge_body_step(
    initial: Any,
    ops: Any,
    summary: MemoryLoopSummary,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """One-iteration transition matches summary metadata (store, base, CTR)."""
    stepped = apply_memory_loop_transition(
        initial,
        build_memory_loop_transition(
            initial,
            trip_count=1,
            base_reg=summary.base_reg,
            source_reg=summary.source_reg,
            stride=int(summary.stride),
            store_width=int(summary.store_width),
            store_kind=summary.store_kind,
            final_ctr=int(summary.final_ctr),
            ops=ops,
        ),
        ops,
    )
    stride = int(summary.stride)
    base = initial.gpr[summary.base_reg]
    value = initial.gpr[summary.source_reg]
    if summary.store_kind == "stwu":
        store_addr = ops.add(base, ops.const(stride & 0xFFFFFFFF))
    else:
        store_addr = base
    # Reconstruct the big-endian word actually written at store_addr.
    read_word = None
    for offset in range(int(summary.store_width)):
        byte = ops.load_byte(stepped.memory, ops.add(store_addr, ops.const(offset)))
        read_word = byte if read_word is None else ops.bor(ops.shl(read_word, ops.const(8)), byte)
    expected_value = ops.band(
        value, ops.const((1 << (8 * int(summary.store_width))) - 1),
    )
    good = [
        stepped.gpr[summary.base_reg] == ops.add(base, ops.const(stride & 0xFFFFFFFF)),
        stepped.ctr == ops.const(int(summary.final_ctr) & 0xFFFFFFFF),
        read_word == expected_value,
    ]
    bad = [z3_module.Not(z3_module.And(*good))]
    return discharge_bad_conditions(
        premises=[],
        bad_conditions=bad,
        deadline=deadline,
        algorithm=BODY_STEP_ALGORITHM,
        z3_module=z3_module,
    )


def _discharge_postcondition(
    initial: Any,
    ops: Any,
    original: MemoryLoopSummary,
    candidate: MemoryLoopSummary,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """Equal entry states yield equal exit observables under both summaries."""
    left = apply_memory_loop_summary(initial, original, ops)
    right = apply_memory_loop_summary(initial, candidate, ops)
    parts = _observable_equality(left, right, original.base_reg, ops)
    bad = [z3_module.Not(z3_module.And(*parts))]
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
    summary: MemoryLoopSummary,
    deadline: Deadline,
    z3_module: Any,
) -> UnsatDischarge:
    """Storing an r1-derived value through the summary clears stack_private."""
    # Route the entry stack pointer (input.gpr.r1) through the store source reg.
    gprs = list(initial.gpr)
    gprs[summary.source_reg] = initial.gpr[1]
    escaping = replace(initial, gpr=tuple(gprs), stack_private=ops.bool(True))
    result = apply_memory_loop_summary(escaping, summary, ops)
    # bad: the private-stack flag is still set after publishing r1.
    bad = [result.stack_private]
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
    # Vacuous UNSAT when the static gate holds; otherwise a satisfiable bad.
    bad = [] if ok else [z3_module.BoolVal(True)]
    return discharge_bad_conditions(
        premises=[],
        bad_conditions=bad,
        deadline=deadline,
        algorithm=FOOTPRINT_ALGORITHM,
        z3_module=z3_module,
    )

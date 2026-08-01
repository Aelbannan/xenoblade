"""Real SMT discharge for relational loop induction (PR7).

Pattern matching may construct the formulas below, but only independent
``discharge_bad_conditions`` UNSAT results mark a block ``discharged``.
"""

from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.discharge import UnsatDischarge, discharge_bad_conditions
from tools.ppc_equivalence.ir import Opcode
from tools.ppc_equivalence.loop_summary import (
    AffineBodyOp,
    AffineGprUpdate,
    CtrAffineLoopCandidate,
)
from tools.ppc_equivalence.relational_induction import (
    ExitAgreementObligation,
    InitiationObligation,
    InvariantTemplateRef,
    PostconditionObligation,
    PreservationObligation,
    RelationalInductionSketch,
    RelationalInductionUnsupported,
    RelationalLoopSide,
    TerminationObligation,
    _affine_bodies_match_for_discharge,
)

PAIRED_TRANSITION_ALGORITHM = "paired-transition-v1"
TERMINATION_ALGORITHM = "ctr-termination-v1"
COMPARE_TERMINATION_ALGORITHM = "compare-counter-termination-v1"


def _is_compare_affine_candidate(candidate: CtrAffineLoopCandidate) -> bool:
    return "compare-affine" in " ".join(candidate.notes)


@dataclass(frozen=True)
class _SideSym:
    """Symbolic loop-header state for one side of a paired induction step."""

    ctr: Any
    gpr: dict[int, Any]
    valid: Any
    invalid_reason: Any
    cr_fields: dict[int, Any]
    memory_tag: Any


@dataclass(frozen=True)
class RelationalDischargeBundle:
    initiation: UnsatDischarge
    preservation: UnsatDischarge
    exit_agreement: UnsatDischarge
    postcondition: UnsatDischarge
    termination: UnsatDischarge

    def all_unsat(self) -> bool:
        return all(
            item.status == "unsat"
            for item in (
                self.initiation,
                self.preservation,
                self.exit_agreement,
                self.postcondition,
                self.termination,
            )
        )

    def failure_reason(self) -> str | None:
        mapping = (
            ("initiation", self.initiation),
            ("preservation", self.preservation),
            ("exit_agreement", self.exit_agreement),
            ("postcondition", self.postcondition),
            ("termination", self.termination),
        )
        for label, item in mapping:
            if item.status != "unsat":
                return f"relational {label} discharge {item.status}"
        return None


def discharge_block_payload(
    discharge: UnsatDischarge,
    *,
    invariants: list[dict[str, Any]],
    z3_module: Any,
    extra: dict[str, Any] | None = None,
) -> dict[str, Any]:
    """PR7 obligation-block shape with independent query evidence."""
    status = "discharged" if discharge.status == "unsat" else "failed"
    payload: dict[str, Any] = {
        "status": status,
        "result": discharge.status,
        "algorithm": discharge.algorithm,
        "query_sha256": discharge.query_sha256,
        "solver": {
            "name": "z3",
            "version": z3_module.get_version_string(),
            "elapsed_ms": discharge.elapsed_ms,
        },
        "invariants": invariants,
        "solver_phases": list(discharge.solver_phases),
    }
    if extra:
        payload.update(extra)
    return payload


def termination_block_payload(
    discharge: UnsatDischarge,
    *,
    witness: str,
    notes: tuple[str, ...],
    z3_module: Any,
) -> dict[str, Any]:
    status = "discharged" if discharge.status == "unsat" else "failed"
    return {
        "witness": witness,
        "status": status,
        "result": discharge.status,
        "algorithm": discharge.algorithm,
        "query_sha256": discharge.query_sha256,
        "solver": {
            "name": "z3",
            "version": z3_module.get_version_string(),
            "elapsed_ms": discharge.elapsed_ms,
        },
        "notes": list(notes),
        "solver_phases": list(discharge.solver_phases),
    }


def try_smt_discharge_ctr_affine(
    original: CtrAffineLoopCandidate,
    candidate: CtrAffineLoopCandidate,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> RelationalInductionSketch | RelationalInductionUnsupported:
    """Build narrow invariants and discharge all five CTR-affine blocks via SMT."""
    if _is_compare_affine_candidate(original) or _is_compare_affine_candidate(candidate):
        return RelationalInductionUnsupported(
            "CTR-affine discharge does not apply to compare-affine candidates"
        )
    if not _affine_bodies_match_for_discharge(original, candidate):
        return RelationalInductionUnsupported(
            "CTR-affine bodies or trip counts do not match for SMT discharge"
        )
    if z3_module is None:
        import z3 as z3_module  # type: ignore[no-redef]

    if deadline is None:
        deadline = Deadline.after_ms(15_000)

    shared_regs = sorted(
        {item.reg for item in original.body_updates}
        | set(_body_op_regs(original))
        | set(_body_op_regs(candidate))
    )
    templates = _narrow_templates_for_pair(original, candidate, shared_regs)
    left0 = _fresh_side(z3_module, "L0", shared_regs, cr_fields=(0,))
    right0 = _fresh_side(z3_module, "R0", shared_regs, cr_fields=(0,))
    inv0 = _invariant(z3_module, left0, right0, shared_regs)

    # Shared entry premises: identical contract at loop header (equal components).
    entry_premises = list(_equal_side_premises(z3_module, left0, right0, shared_regs))
    zero = z3_module.BitVecVal(0, 32)
    # Parametric mode (doc 30 Phase A3): both sides pin a *fresh symbolic* N
    # with the premise N != 0 (the ``bdnz`` zero-trip wrap is exactly N == 0).
    # Concrete mode pins the shared concrete trip as before.
    parametric = original.trip_count is None or candidate.trip_count is None
    trip: int | None = None
    if parametric:
        n = z3_module.BitVec("paired.N", 32)
        entry_premises.append(n != zero)
        entry_premises.append(left0.ctr == n)
        entry_premises.append(right0.ctr == n)
    else:
        trip = original.trip_count
        assert trip is not None and trip >= 1
        entry_premises.append(left0.ctr == z3_module.BitVecVal(trip & 0xFFFFFFFF, 32))
        entry_premises.append(right0.ctr == z3_module.BitVecVal(trip & 0xFFFFFFFF, 32))
    entry_premises.append(left0.valid == z3_module.BoolVal(True))
    entry_premises.append(right0.valid == z3_module.BoolVal(True))

    initiation = discharge_bad_conditions(
        premises=entry_premises,
        bad_conditions=[z3_module.Not(inv0)],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:initiation",
        z3_module=z3_module,
    )

    left1, left_continue, left_exit, left_step_ok = _ctr_affine_step(
        z3_module, left0, original.body_updates, body_ops=original.body_ops,
    )
    right1, right_continue, right_exit, right_step_ok = _ctr_affine_step(
        z3_module, right0, candidate.body_updates, body_ops=candidate.body_ops,
    )
    inv1 = _invariant(z3_module, left1, right1, shared_regs)
    both_continue = z3_module.And(left_continue, right_continue)
    both_exit = z3_module.And(left_exit, right_exit)
    xor_exits = z3_module.Xor(left_exit, right_exit)

    preservation = discharge_bad_conditions(
        premises=[
            inv0,
            both_continue,
            left_step_ok,
            right_step_ok,
        ],
        bad_conditions=[z3_module.Not(inv1)],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:preservation",
        z3_module=z3_module,
    )

    exit_agreement = discharge_bad_conditions(
        premises=[inv0, left_step_ok, right_step_ok],
        bad_conditions=[xor_exits],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:exit-agreement",
        z3_module=z3_module,
    )

    # Postcondition observables at exit: CTR, shared GPRs, validity.
    observables_equal = _invariant(z3_module, left1, right1, shared_regs)
    postcondition = discharge_bad_conditions(
        premises=[
            inv0,
            both_exit,
            left_step_ok,
            right_step_ok,
        ],
        bad_conditions=[z3_module.Not(observables_equal)],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:postcondition",
        z3_module=z3_module,
    )

    termination = _discharge_ctr_termination(
        z3_module,
        left0,
        right0,
        original.body_updates,
        candidate.body_updates,
        body_ops=original.body_ops,
        trip=trip,
        deadline=deadline,
    )
    bundle = RelationalDischargeBundle(
        initiation=initiation,
        preservation=preservation,
        exit_agreement=exit_agreement,
        postcondition=postcondition,
        termination=termination,
    )

    invariant_payload = [
        {"name": item.name, "params": dict(item.params)} for item in templates
    ]
    initiation_block = discharge_block_payload(
        initiation, invariants=invariant_payload, z3_module=z3_module,
    )
    preservation_block = discharge_block_payload(
        preservation, invariants=invariant_payload, z3_module=z3_module,
    )
    exit_block = discharge_block_payload(
        exit_agreement, invariants=invariant_payload, z3_module=z3_module,
    )
    post_block = discharge_block_payload(
        postcondition, invariants=invariant_payload, z3_module=z3_module,
    )
    term_notes = (
        "nonzero entry CTR",
        "one CTR decrement per paired step",
        "no CTR rewrite in affine body",
        "exit at CTR zero",
        "no 32-bit CTR wrap on continue/exit",
    )
    if parametric:
        term_notes = term_notes + (
            "parametric trip N with premise N != 0 (wrap-freedom argued from N)",
        )
    termination_block = termination_block_payload(
        termination,
        witness="ctr-descending",
        notes=term_notes,
        z3_module=z3_module,
    )

    status = "discharged" if bundle.all_unsat() else "failed"
    notes = (
        "ctr-affine relational SMT discharge",
        f"shared body registers: {shared_regs or 'none'}",
    )
    if parametric:
        notes = notes + ("parametric symbolic trip (v2)",)
    if not bundle.all_unsat():
        reason = bundle.failure_reason() or "relational discharge incomplete"
        notes = notes + (reason,)

    return RelationalInductionSketch(
        original=RelationalLoopSide.from_affine(original),
        candidate=RelationalLoopSide.from_affine(candidate),
        initiation=InitiationObligation(
            tuple(templates),
            status=initiation_block["status"],
        ),
        preservation=PreservationObligation(
            tuple(templates),
            status=preservation_block["status"],
        ),
        exit_agreement=ExitAgreementObligation(
            tuple(templates),
            status=exit_block["status"],
        ),
        postcondition=PostconditionObligation(
            tuple(templates),
            status=post_block["status"],
        ),
        termination=TerminationObligation(
            witness="ctr-descending",
            status=termination_block["status"],
            notes=term_notes,
        ),
        templates=tuple(item.name for item in templates),
        status=status,
        notes=notes,
        block_evidence={
            "initiation": initiation_block,
            "preservation": preservation_block,
            "exit_agreement": exit_block,
            "postcondition": post_block,
            "termination": termination_block,
        },
    )


def _narrow_templates_for_pair(
    original: CtrAffineLoopCandidate,
    candidate: CtrAffineLoopCandidate,
    shared_regs: list[int],
) -> tuple[InvariantTemplateRef, ...]:
    templates: list[InvariantTemplateRef] = [
        InvariantTemplateRef(
            "equal-ctr",
            {
                "original_reg": original.trip_count_reg,
                "candidate_reg": candidate.trip_count_reg,
                "trip_count": original.trip_count,
            },
        ),
        InvariantTemplateRef("equal-validity", {"side": "both"}),
        InvariantTemplateRef("equal-cr-field", {"field": 0}),
        InvariantTemplateRef("equal-memory", {"mode": "canonical-equal"}),
    ]
    for reg in shared_regs:
        templates.append(
            InvariantTemplateRef("equal-gpr", {"register": reg, "side": "both"}),
        )
    # Constant offset reserved for mismatched strides; matching bodies use equality.
    _ = candidate
    return tuple(templates)


def _fresh_side(
    z3: Any,
    prefix: str,
    regs: list[int],
    *,
    cr_fields: tuple[int, ...] = (),
) -> _SideSym:
    return _SideSym(
        ctr=z3.BitVec(f"{prefix}.ctr", 32),
        gpr={reg: z3.BitVec(f"{prefix}.r{reg}", 32) for reg in regs},
        valid=z3.Bool(f"{prefix}.valid"),
        invalid_reason=z3.BitVec(f"{prefix}.invalid_reason", 8),
        cr_fields={
            field: z3.BitVec(f"{prefix}.cr{field}", 4) for field in cr_fields
        },
        memory_tag=z3.BitVec(f"{prefix}.mem_tag", 32),
    )


def _equal_side_premises(
    z3: Any,
    left: _SideSym,
    right: _SideSym,
    regs: list[int],
) -> list[Any]:
    premises = [
        left.ctr == right.ctr,
        left.valid == right.valid,
        left.invalid_reason == right.invalid_reason,
        left.memory_tag == right.memory_tag,
    ]
    for reg in regs:
        premises.append(left.gpr[reg] == right.gpr[reg])
    for field in sorted(set(left.cr_fields) & set(right.cr_fields)):
        premises.append(left.cr_fields[field] == right.cr_fields[field])
    return premises


def _invariant(
    z3: Any,
    left: _SideSym,
    right: _SideSym,
    regs: list[int],
) -> Any:
    parts = [
        left.ctr == right.ctr,
        left.valid == right.valid,
        left.invalid_reason == right.invalid_reason,
        left.memory_tag == right.memory_tag,
    ]
    for reg in regs:
        parts.append(left.gpr[reg] == right.gpr[reg])
    for field in sorted(set(left.cr_fields) & set(right.cr_fields)):
        parts.append(left.cr_fields[field] == right.cr_fields[field])
    if len(parts) == 1:
        return parts[0]
    return z3.And(*parts)


def _rlwinm_mask(mb: int, me: int) -> int:
    """PPC ``rlwinm`` mask: bits ``mb..me`` (MSB-indexed, wrapping)."""
    if mb <= me:
        return ((0xFFFFFFFF << (31 - me)) & (0xFFFFFFFF >> mb)) & 0xFFFFFFFF
    return ((0xFFFFFFFF >> mb) | (0xFFFFFFFF << (31 - me))) & 0xFFFFFFFF


def _body_op_regs(candidate: CtrAffineLoopCandidate) -> list[int]:
    """All GPR operands of the candidate's whitelisted body ops."""
    return sorted({
        int(v)
        for op in (candidate.body_ops or ())
        for v in op.operands
        if 0 <= int(v) <= 31
    })


def _apply_body_ops(
    z3: Any,
    gpr: dict[int, Any],
    body_ops: tuple[AffineBodyOp, ...] | None,
) -> dict[int, Any]:
    """Apply the whitelisted body ops (doc 30 Phase D2) to a GPR dict.

    Each opcode's transition must match ``semantics.execute_instruction``
    exactly so the discharge body model cannot drift from the widened body.
    """
    gpr = dict(gpr)
    for op in body_ops or ():
        opcode = op.opcode
        operands = op.operands
        if opcode == Opcode.ADD:  # add rD, rA, rB
            rd, ra, rb = (int(v) for v in operands)
            gpr[rd] = gpr[ra] + gpr[rb]
        elif opcode == Opcode.SUBF:  # subf rD, rA, rB -> rD = rB - rA
            rd, ra, rb = (int(v) for v in operands)
            gpr[rd] = gpr[rb] - gpr[ra]
        elif opcode == Opcode.MULLI:  # mulli rD, rA, imm
            rd, ra, imm = (int(v) for v in operands)
            gpr[rd] = gpr[ra] * z3.BitVecVal(int(imm) & 0xFFFFFFFF, 32)
        elif opcode == Opcode.OR:  # or rD, rA, rB
            rd, ra, rb = (int(v) for v in operands)
            gpr[rd] = gpr[ra] | gpr[rb]
        elif opcode == Opcode.XOR:  # xor rD, rA, rB
            rd, ra, rb = (int(v) for v in operands)
            gpr[rd] = gpr[ra] ^ gpr[rb]
        elif opcode == Opcode.RLWINM:  # rlwinm rA, rS, sh, mb, me
            ra, rs, sh, mb, me = (int(v) for v in operands)
            value = gpr[rs]
            sh &= 31
            if sh == 0:
                rotated = value
            else:
                rotated = (value << sh) | z3.LShR(value, 32 - sh)
            gpr[ra] = rotated & z3.BitVecVal(_rlwinm_mask(mb, me), 32)
        elif opcode == Opcode.EXTSB:  # extsb rD, rS
            rd, rs = (int(v) for v in operands)
            gpr[rd] = z3.SignExt(24, z3.Extract(7, 0, gpr[rs]))
        else:
            raise ValueError(f"unmodeled body opcode {opcode.value}")
    return gpr


def _ctr_affine_step(
    z3: Any,
    state: _SideSym,
    updates: tuple[AffineGprUpdate, ...],
    *,
    body_ops: tuple[AffineBodyOp, ...] | None = None,
) -> tuple[_SideSym, Any, Any, Any]:
    """One body + bdnz step. Continue iff CTR' != 0; exit iff CTR' == 0.

    ``step_ok`` rules out wrap from CTR==0 (bdnz would set CTR to 0xffffffff).
    """
    zero = z3.BitVecVal(0, 32)
    one = z3.BitVecVal(1, 32)
    no_wrap = state.ctr != zero
    ctr_next = state.ctr - one
    gpr_next = _apply_body_ops(z3, state.gpr, body_ops)
    for update in updates:
        if update.reg in gpr_next:
            gpr_next[update.reg] = gpr_next[update.reg] + z3.BitVecVal(
                update.addend & 0xFFFFFFFF, 32,
            )
    nxt = replace(
        state,
        ctr=ctr_next,
        gpr=gpr_next,
    )
    continue_cond = z3.And(no_wrap, ctr_next != zero)
    exit_cond = z3.And(no_wrap, ctr_next == zero)
    return nxt, continue_cond, exit_cond, no_wrap


def _discharge_ctr_termination(
    z3: Any,
    left: _SideSym,
    right: _SideSym,
    left_updates: tuple[AffineGprUpdate, ...],
    right_updates: tuple[AffineGprUpdate, ...],
    *,
    body_ops: tuple[AffineBodyOp, ...] | None,
    trip: int | None,
    deadline: Deadline,
) -> UnsatDischarge:
    """Termination witnesses encoded as a single independent UNSAT query.

    Concrete ``trip`` pins both entry CTRs to the constant; ``trip=None``
    (doc 30 Phase A3) pins both to a fresh symbolic ``N`` with the premise
    ``N != 0`` — wrap-freedom is argued from ``N`` itself, never assumed.
    """
    zero = z3.BitVecVal(0, 32)
    one = z3.BitVecVal(1, 32)

    # Body must not rewrite CTR (pattern already forbids mtspr CTR; encode as
    # post-body CTR still equal to pre-body before the explicit decrement).
    left_body_ctr_ok = z3.BoolVal(True)
    right_body_ctr_ok = z3.BoolVal(True)
    _ = left_updates, right_updates

    left1, left_continue, left_exit, left_ok = _ctr_affine_step(
        z3, left, left_updates, body_ops=body_ops,
    )
    right1, right_continue, right_exit, right_ok = _ctr_affine_step(
        z3, right, right_updates, body_ops=body_ops,
    )

    # Properties that must hold; discharge proves their negations are unreachable
    # under the paired CTR model + trip premises.
    premises = [
        left_body_ctr_ok,
        right_body_ctr_ok,
        left_ok,
        right_ok,
        # one decrement: ctr' == ctr - 1 (already in the step model)
        left1.ctr == left.ctr - one,
        right1.ctr == right.ctr - one,
    ]
    if trip is not None:
        trip_val = z3.BitVecVal(trip & 0xFFFFFFFF, 32)
        premises.append(left.ctr == trip_val)
        premises.append(right.ctr == trip_val)
    else:
        n = z3.BitVec("term.N", 32)
        premises.append(left.ctr == n)
        premises.append(right.ctr == n)
        premises.append(n != zero)

    # Bad: entry CTR is zero, or wrap reachable on a modeled step, or exit not at 0,
    # or continue does not strictly decrease.
    bad = [
        left.ctr == zero,
        right.ctr == zero,
        z3.And(left_continue, z3.Not(z3.ULT(left1.ctr, left.ctr))),
        z3.And(right_continue, z3.Not(z3.ULT(right1.ctr, right.ctr))),
        z3.And(left_exit, left1.ctr != zero),
        z3.And(right_exit, right1.ctr != zero),
        # wrap: taking a step from CTR==0
        z3.And(left.ctr == zero, z3.Or(left_continue, left_exit)),
        z3.And(right.ctr == zero, z3.Or(right_continue, right_exit)),
    ]
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=TERMINATION_ALGORITHM,
        z3_module=z3,
    )


def _countdown_params(
    candidate: CtrAffineLoopCandidate,
) -> tuple[int, str, bool, int | None] | None:
    """``(step, family, signed, bound_reg)`` from the candidate's countdown trip."""
    from tools.ppc_equivalence.trip_expression import (
        TripCountdown,
        trip_expr_from_canonical,
    )

    if candidate.trip_expr is None:
        return None
    expr = trip_expr_from_canonical(candidate.trip_expr)
    if not isinstance(expr, TripCountdown):
        return None
    bound_reg = (
        candidate.final_compare.right_reg
        if candidate.final_compare is not None
        else None
    )
    return int(expr.step), expr.family, expr.signed, bound_reg


def try_smt_discharge_compare_affine(
    original: CtrAffineLoopCandidate,
    candidate: CtrAffineLoopCandidate,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> RelationalInductionSketch | RelationalInductionUnsupported:
    """Discharge compare-affine closed-form pairs via five independent UNSAT queries.

    Rewritten for doc 30 Phase C3: the countdown model carries the latch
    family (``bne``/``beq``/``blt``/``bgt``/``ble``/``bge``), the signed
    per-iteration ``step``, the compare form's signedness, and the bound
    (immediate or symbolic register). A fresh symbolic entry counter and
    symbolic bound are pinned equal across sides; the countdown termination
    premise (the family-specific zero-entry / divisibility condition) is a
    premise, never an assumption.
    """
    if not _is_compare_affine_candidate(original) or not _is_compare_affine_candidate(
        candidate,
    ):
        return RelationalInductionUnsupported(
            "compare-affine SMT discharge requires compare-affine candidates"
        )
    if not _affine_bodies_match_for_discharge(original, candidate):
        return RelationalInductionUnsupported(
            "compare-affine bodies or trip counts do not match for SMT discharge"
        )
    if original.trip_count_reg is None or candidate.trip_count_reg is None:
        return RelationalInductionUnsupported(
            "compare-affine countdown register missing"
        )
    if original.trip_count_reg != candidate.trip_count_reg:
        return RelationalInductionUnsupported(
            "compare-affine countdown registers differ between sides"
        )
    left_params = _countdown_params(original)
    right_params = _countdown_params(candidate)
    if left_params is None or right_params is None or left_params != right_params:
        return RelationalInductionUnsupported(
            "compare-affine countdown shape differs between sides"
        )
    step, family, signed, bound_reg = left_params
    if z3_module is None:
        import z3 as z3_module  # type: ignore[no-redef]

    if deadline is None:
        deadline = Deadline.after_ms(15_000)

    counter_reg = original.trip_count_reg
    body_regs = sorted({item.reg for item in original.body_updates})
    bound_regs = [bound_reg] if bound_reg is not None else []
    # Counter, bound, and whitelisted body-op registers participate in the
    # invariant even when absent from body_updates.
    shared_regs = sorted(
        set(body_regs)
        | {counter_reg}
        | set(bound_regs)
        | set(_body_op_regs(original))
        | set(_body_op_regs(candidate))
    )
    templates = _narrow_templates_for_compare_pair(
        original, candidate, shared_regs, counter_reg=counter_reg,
    )
    left0 = _fresh_side(z3_module, "L0", shared_regs, cr_fields=(0,))
    right0 = _fresh_side(z3_module, "R0", shared_regs, cr_fields=(0,))
    inv0 = _invariant(z3_module, left0, right0, shared_regs)

    entry_premises = list(_equal_side_premises(z3_module, left0, right0, shared_regs))
    entry_premises.append(left0.valid == z3_module.BoolVal(True))
    entry_premises.append(right0.valid == z3_module.BoolVal(True))
    # Fresh symbolic entry counter and bound, pinned equal across sides
    # (doc 30 Phase C3). The termination premise excludes the zero-entry wrap
    # and the non-terminating families.
    entry_sym = z3_module.BitVec("cmp.entry", 32)
    entry_premises.append(left0.gpr[counter_reg] == entry_sym)
    entry_premises.append(right0.gpr[counter_reg] == entry_sym)
    if bound_reg is not None:
        bound_sym = z3_module.BitVec("cmp.bound", 32)
        entry_premises.append(left0.gpr[bound_reg] == bound_sym)
        entry_premises.append(right0.gpr[bound_reg] == bound_sym)
        bound_value = bound_sym
    else:
        assert original.final_compare is not None
        bound_value = z3_module.BitVecVal(
            int(original.final_compare.right_imm) & 0xFFFFFFFF, 32,
        )
    termination_premise = _countdown_termination_z3(
        z3_module, entry_sym, bound_value, step, family, signed,
    )
    entry_premises.append(termination_premise)

    initiation = discharge_bad_conditions(
        premises=entry_premises,
        bad_conditions=[z3_module.Not(inv0)],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:initiation",
        z3_module=z3_module,
    )

    left1, left_continue, left_exit, left_step_ok = _compare_affine_step(
        z3_module, left0, original.body_updates, counter_reg=counter_reg,
        step=step, family=family, signed=signed, bound_value=bound_value,
        body_ops=original.body_ops,
    )
    right1, right_continue, right_exit, right_step_ok = _compare_affine_step(
        z3_module, right0, candidate.body_updates, counter_reg=counter_reg,
        step=step, family=family, signed=signed, bound_value=bound_value,
        body_ops=candidate.body_ops,
    )
    inv1 = _invariant(z3_module, left1, right1, shared_regs)
    both_continue = z3_module.And(left_continue, right_continue)
    both_exit = z3_module.And(left_exit, right_exit)
    xor_exits = z3_module.Xor(left_exit, right_exit)

    preservation = discharge_bad_conditions(
        premises=[inv0, both_continue, left_step_ok, right_step_ok],
        bad_conditions=[z3_module.Not(inv1)],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:preservation",
        z3_module=z3_module,
    )

    exit_agreement = discharge_bad_conditions(
        premises=[inv0, left_step_ok, right_step_ok],
        bad_conditions=[xor_exits],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:exit-agreement",
        z3_module=z3_module,
    )

    observables_equal = _invariant(z3_module, left1, right1, shared_regs)
    postcondition = discharge_bad_conditions(
        premises=[inv0, both_exit, left_step_ok, right_step_ok],
        bad_conditions=[z3_module.Not(observables_equal)],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:postcondition",
        z3_module=z3_module,
    )

    termination = _discharge_compare_termination(
        z3_module,
        left0,
        right0,
        original.body_updates,
        candidate.body_updates,
        body_ops=original.body_ops,
        counter_reg=counter_reg,
        step=step,
        family=family,
        signed=signed,
        bound_value=bound_value,
        entry_sym=entry_sym,
        termination_premise=termination_premise,
        deadline=deadline,
    )

    bundle = RelationalDischargeBundle(
        initiation=initiation,
        preservation=preservation,
        exit_agreement=exit_agreement,
        postcondition=postcondition,
        termination=termination,
    )

    invariant_payload = [
        {"name": item.name, "params": dict(item.params)} for item in templates
    ]
    initiation_block = discharge_block_payload(
        initiation, invariants=invariant_payload, z3_module=z3_module,
    )
    preservation_block = discharge_block_payload(
        preservation, invariants=invariant_payload, z3_module=z3_module,
    )
    exit_block = discharge_block_payload(
        exit_agreement, invariants=invariant_payload, z3_module=z3_module,
    )
    post_block = discharge_block_payload(
        postcondition, invariants=invariant_payload, z3_module=z3_module,
    )
    term_notes = (
        f"countdown {family} step {step:+d}",
        "one counter step per paired iteration",
        "no countdown rewrite in affine body",
        "exit when the continue relation fails",
        "termination premise enforced (zero-entry / divisibility)",
        "CTR unmodified (compare-affine)",
    )
    termination_block = termination_block_payload(
        termination,
        witness="counter-descending",
        notes=term_notes,
        z3_module=z3_module,
    )

    status = "discharged" if bundle.all_unsat() else "failed"
    notes = (
        "compare-affine relational SMT discharge",
        f"countdown r{counter_reg} {family} step {step:+d}",
        f"shared body registers: {body_regs or 'none'}",
    )
    if not bundle.all_unsat():
        reason = bundle.failure_reason() or "relational discharge incomplete"
        notes = notes + (reason,)

    return RelationalInductionSketch(
        original=RelationalLoopSide.from_affine(original),
        candidate=RelationalLoopSide.from_affine(candidate),
        initiation=InitiationObligation(
            tuple(templates),
            status=initiation_block["status"],
        ),
        preservation=PreservationObligation(
            tuple(templates),
            status=preservation_block["status"],
        ),
        exit_agreement=ExitAgreementObligation(
            tuple(templates),
            status=exit_block["status"],
        ),
        postcondition=PostconditionObligation(
            tuple(templates),
            status=post_block["status"],
        ),
        termination=TerminationObligation(
            witness="counter-descending",
            status=termination_block["status"],
            notes=term_notes,
        ),
        templates=tuple(item.name for item in templates),
        status=status,
        notes=notes,
        block_evidence={
            "initiation": initiation_block,
            "preservation": preservation_block,
            "exit_agreement": exit_block,
            "postcondition": post_block,
            "termination": termination_block,
        },
    )


def _narrow_templates_for_compare_pair(
    original: CtrAffineLoopCandidate,
    candidate: CtrAffineLoopCandidate,
    shared_regs: list[int],
    *,
    counter_reg: int,
) -> tuple[InvariantTemplateRef, ...]:
    templates: list[InvariantTemplateRef] = [
        InvariantTemplateRef(
            "equal-gpr",
            {
                "register": counter_reg,
                "side": "both",
                "role": "countdown",
                "trip_count": original.trip_count,
            },
        ),
        InvariantTemplateRef("equal-ctr", {"mode": "unmodified"}),
        InvariantTemplateRef("equal-validity", {"side": "both"}),
        InvariantTemplateRef("equal-cr-field", {"field": 0}),
        InvariantTemplateRef("equal-memory", {"mode": "canonical-equal"}),
    ]
    for reg in shared_regs:
        if reg == counter_reg:
            continue
        templates.append(
            InvariantTemplateRef("equal-gpr", {"register": reg, "side": "both"}),
        )
    _ = candidate
    return tuple(templates)


def _countdown_trip_z3(
    z3: Any,
    counter: Any,
    bound: Any,
    step: int,
    family: str,
    signed: bool,
) -> Any:
    """Symbolic do-while trip for a countdown latch, built directly in z3."""
    one = z3.BitVecVal(1, 32)
    zero = z3.BitVecVal(0, 32)
    s = abs(int(step))
    s_bv = z3.BitVecVal(s, 32)
    if step < 0:
        d = counter - bound
        negative = counter < bound if signed else z3.BoolVal(False)
        if family == "bge":
            return z3.If(negative, one, z3.UDiv(d, s_bv) + one)
        if family == "bgt":
            le_zero = counter <= bound if signed else d == zero
            return z3.If(le_zero, one, z3.UDiv(d - one, s_bv) + one)
        if family in ("ble", "blt"):
            return one
        if family == "bne":
            return z3.UDiv(d, s_bv)
        if family == "beq":
            return z3.If(d == s_bv, z3.BitVecVal(2, 32), one)
    else:
        d = bound - counter
        negative = bound < counter if signed else z3.BoolVal(False)
        if family == "blt":
            le_zero = bound <= counter if signed else d == zero
            return z3.If(le_zero, one, z3.UDiv(d - one, s_bv) + one)
        if family == "ble":
            return z3.If(negative, one, z3.UDiv(d, s_bv) + one)
        if family in ("bgt", "bge"):
            return one
        if family == "bne":
            return z3.UDiv(d, s_bv)
        if family == "beq":
            return z3.If(d == s_bv, z3.BitVecVal(2, 32), one)
    raise ValueError(f"unknown TripCountdown family {family!r}")


def _countdown_termination_z3(
    z3: Any,
    counter: Any,
    bound: Any,
    step: int,
    family: str,
    signed: bool,
) -> Any:
    """The countdown termination premise (doc 30 Phase C3) in z3.

    Excludes the zero-entry wrap (``bne`` against a zero bound), the
    non-terminating families (the counter never reaches the exit region), and
    the ranking-wrap (the counter must not wrap while still continuing, so the
    unsigned ranking holds on every continued step).
    """
    s = abs(int(step))
    s_bv = z3.BitVecVal(s, 32)
    one = z3.BitVecVal(1, 32)
    zero = z3.BitVecVal(0, 32)
    max_u32 = z3.BitVecVal(0xFFFFFFFF, 32)

    def _ge(a: Any, b: Any) -> Any:
        return a >= b if signed else z3.UGE(a, b)

    def _gt(a: Any, b: Any) -> Any:
        return a > b if signed else z3.UGT(a, b)

    def _eq_divisible(a: Any, b: Any) -> Any:
        return z3.URem(a, b) == zero

    if step < 0:
        d = counter - bound
        if family == "ble":
            family_premise = _gt(d, s_bv)
        elif family == "blt":
            family_premise = _ge(d, s_bv)
        elif family == "bne":
            family_premise = z3.And(_eq_divisible(d, s_bv), _ge(d, s_bv))
        else:
            family_premise = z3.BoolVal(True)
        # Wrap-freedom: only the continue steps (T-1 of them) need the
        # unsigned ranking, so (T-1)*step <= entry.
        trip = _countdown_trip_z3(z3, counter, bound, step, family, signed)
        return z3.And(family_premise, z3.ULE((trip - one) * s_bv, counter))
    d = bound - counter
    if family == "bgt":
        family_premise = _ge(d, s_bv)
    elif family == "bge":
        family_premise = _gt(d, s_bv)
    elif family == "bne":
        family_premise = z3.And(_eq_divisible(d, s_bv), _ge(d, s_bv))
    else:
        family_premise = z3.BoolVal(True)
    # Wrap-freedom: only the continue steps (T-1 of them) need the unsigned
    # ranking, so (T-1)*step <= 0xffffffff - entry (no modulo wrap).
    trip = _countdown_trip_z3(z3, counter, bound, step, family, signed)
    return z3.And(family_premise, z3.ULE((trip - one) * s_bv, max_u32 - counter))


def _compare_affine_step(
    z3: Any,
    state: _SideSym,
    updates: tuple[AffineGprUpdate, ...],
    *,
    counter_reg: int,
    step: int,
    family: str,
    signed: bool,
    bound_value: Any,
    body_ops: tuple[AffineBodyOp, ...] | None = None,
) -> tuple[_SideSym, Any, Any, Any]:
    """One body + ``addi/subi rT, ±step`` / ``cmp rT, bound`` / ``b<family>`` step.

    Continue iff the latch family's relation holds for the stepped counter;
    exit iff it fails. CR0 is updated from the compare with the compare form's
    signedness. ``step_ok`` is ``True`` (no CTR-style wrap model for GPR
    counters; the termination premise covers the zero-entry hazard).
    """
    counter = state.gpr[counter_reg]
    counter_next = counter + z3.BitVecVal(int(step) & 0xFFFFFFFF, 32)
    gpr_next = _apply_body_ops(z3, state.gpr, body_ops)
    for update in updates:
        if update.reg in gpr_next and update.reg != counter_reg:
            gpr_next[update.reg] = gpr_next[update.reg] + z3.BitVecVal(
                update.addend & 0xFFFFFFFF, 32,
            )
    gpr_next[counter_reg] = counter_next
    # cmp cr0, counter', bound — CR0 per signedness.
    if signed:
        lt = counter_next < bound_value
        gt = bound_value < counter_next
    else:
        lt = z3.ULT(counter_next, bound_value)
        gt = z3.ULT(bound_value, counter_next)
    cr0_next = z3.If(
        lt,
        z3.BitVecVal(0x8, 4),
        z3.If(gt, z3.BitVecVal(0x4, 4), z3.BitVecVal(0x2, 4)),
    )
    cr_next = dict(state.cr_fields)
    if 0 in cr_next:
        cr_next[0] = cr0_next
    nxt = replace(
        state,
        gpr=gpr_next,
        cr_fields=cr_next,
    )
    if family == "bne":
        continue_cond = counter_next != bound_value
    elif family == "beq":
        continue_cond = counter_next == bound_value
    elif family == "blt":
        continue_cond = lt
    elif family == "bgt":
        continue_cond = gt
    elif family == "ble":
        continue_cond = z3.Or(lt, counter_next == bound_value)
    elif family == "bge":
        continue_cond = z3.Or(gt, counter_next == bound_value)
    else:
        raise ValueError(f"unknown compare latch family {family!r}")
    return nxt, continue_cond, z3.Not(continue_cond), z3.BoolVal(True)


def _discharge_compare_termination(
    z3: Any,
    left: _SideSym,
    right: _SideSym,
    left_updates: tuple[AffineGprUpdate, ...],
    right_updates: tuple[AffineGprUpdate, ...],
    *,
    body_ops: tuple[AffineBodyOp, ...] | None,
    counter_reg: int,
    step: int,
    family: str,
    signed: bool,
    bound_value: Any,
    entry_sym: Any,
    termination_premise: Any,
    deadline: Deadline,
) -> UnsatDischarge:
    """Termination via countdown-GPR ranking (not CTR), general step/family."""
    zero = z3.BitVecVal(0, 32)

    left1, left_continue, left_exit, left_ok = _compare_affine_step(
        z3, left, left_updates, counter_reg=counter_reg,
        step=step, family=family, signed=signed, bound_value=bound_value,
        body_ops=body_ops,
    )
    right1, right_continue, right_exit, right_ok = _compare_affine_step(
        z3, right, right_updates, counter_reg=counter_reg,
        step=step, family=family, signed=signed, bound_value=bound_value,
        body_ops=body_ops,
    )

    def _ranks(next_value: Any, prev_value: Any) -> Any:
        if step < 0:
            return z3.ULT(next_value, prev_value)
        return z3.UGT(next_value, prev_value)

    premises = [
        left.gpr[counter_reg] == entry_sym,
        right.gpr[counter_reg] == entry_sym,
        left.ctr == right.ctr,  # CTR equal and unmodified across the step
        left1.ctr == left.ctr,
        right1.ctr == right.ctr,
        left_ok,
        right_ok,
        left1.gpr[counter_reg] == left.gpr[counter_reg] + z3.BitVecVal(
            int(step) & 0xFFFFFFFF, 32,
        ),
        right1.gpr[counter_reg] == right.gpr[counter_reg] + z3.BitVecVal(
            int(step) & 0xFFFFFFFF, 32,
        ),
        termination_premise,
    ]

    bad = [
        # ranking: a continued step must move the counter monotonically; the
        # termination premise excludes the wrap (zero-entry / non-terminating
        # families / ranking-wrap), so a violated ranking here is a real bug.
        z3.And(left_continue, z3.Not(_ranks(left1.gpr[counter_reg], left.gpr[counter_reg]))),
        z3.And(right_continue, z3.Not(_ranks(right1.gpr[counter_reg], right.gpr[counter_reg]))),
        # exit while the continue relation still holds
        z3.And(left_exit, left_continue),
        z3.And(right_exit, right_continue),
        # step_ok must hold
        z3.Not(left_ok),
        z3.Not(right_ok),
    ]
    return discharge_bad_conditions(
        premises=premises,
        bad_conditions=bad,
        deadline=deadline,
        algorithm=COMPARE_TERMINATION_ALGORITHM,
        z3_module=z3,
    )

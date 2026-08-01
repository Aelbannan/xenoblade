"""Affine / CTR closed-form loop summaries.

Target shape (MWCC / EABI counted loops)::

    li / addi  rT, _, N     # concrete trip count N >= 1
    mtctr      rT
  header:
    addi       rD, rD, K    # zero or more constant-stride GPR updates
    bdnz       header       # BO=16: decrement CTR, branch if CTR != 0

PPC corners encoded in the summary notes:

- ``bdnz`` decrements CTR **before** the zero test.
- Loading CTR with ``0`` makes ``bdnz`` wrap to ``0xffffffff`` (not a zero-trip loop).
- Closed forms assume no unsigned wrap of the affine accumulators over ``N``
  steps. **This caveat applies to store-address arithmetic only** (the
  memory-loop footprint gate): for GPR-affine accumulators, ``entry +
  trip * stride`` is exact in 32-bit BV semantics for any trip count —
  repeated ``addi`` accumulates mod 2^32 identically to one ``mul`` + ``add``.
  Symbolic-trip summaries (``trip_expr``, doc 30 Phase A) rely on exactly this
  identity, so the parametric form is exact for any symbolic trip.

Compare-affine countdown loops also record a ``FinalCompare`` so CR (including
XER.SO) matches the last ``cmpwi`` after the closed-form GPR updates.

When ``execute_cfg`` is given a summary map (or auto-builds one), matching
headers are applied in closed form. ``affine-loop-summary`` may authorize
``EQUIVALENT`` only when the obligation carries ``status=discharged`` with a
matching ``summary_sha256`` (and, for CTR-affine, a discharged relational
companion). Pattern recognition or ``coverage=applied`` alone never authorizes.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass, replace
from typing import Any

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.provenance import canonical_json_sha256
from tools.ppc_equivalence.skip_guard import (
    SkipGuardInfo,
    find_mtctr_with_guard,
    guard_family,
)
from tools.ppc_equivalence.trip_expression import (
    TripConstant,
    TripCountdown,
    TripEntryReg,
    TripExpr,
    canonical_dict,
    evaluate_concrete,
    normalize_trip_expr,
    recognize_trip_expr,
)

_BDNZ_BO = 16  # decrement CTR; branch if CTR != 0 after decrement


@dataclass(frozen=True)
class AffineGprUpdate:
    """One loop-body step ``addi rD, rD, K`` (constant stride on a single GPR)."""

    reg: int
    addend: int


@dataclass(frozen=True)
class AffineBodyOp:
    """One whitelisted GPR-pure body instruction (doc 30 Phase D2).

    The summary folds the constant-stride ``addi``/``subi`` updates; every
    other whitelisted opcode is modeled exactly in the relational discharge
    step and must be dead/invariant for the closed form to discharge.
    """

    opcode: Opcode
    operands: tuple[int, ...]


@dataclass(frozen=True)
class FinalCompare:
    """Final CR-field compare applied after closed-form GPR updates.

    Models the last ``cmpwi`` / ``cmpw`` (signed) or ``cmplwi`` / ``cmplw``
    (unsigned) that executes on loop exit before the failing branch.
    """

    field: int
    left_reg: int
    right_imm: int | None
    right_reg: int | None
    signed: bool


@dataclass(frozen=True)
class CtrAffineLoopCandidate:
    """Recognized ``mtctr`` / body / ``bdnz`` counted loop (pattern only)."""

    mtctr_pc: int
    header_pc: int
    latch_pc: int
    exit_pc: int
    trip_count: int | None
    trip_count_reg: int | None
    body_updates: tuple[AffineGprUpdate, ...]
    instruction_indexes: tuple[int, ...]
    confidence: str
    notes: tuple[str, ...]
    final_compare: FinalCompare | None = None
    trip_expr: dict[str, Any] | None = None
    skip_guard: dict[str, Any] | None = None
    body_ops: tuple[AffineBodyOp, ...] | None = None


@dataclass(frozen=True)
class LoopSummary:
    """Closed-form summary for a recognized CTR affine loop.

    ``final_gpr[reg] = (entry_reg, stride)`` means
    ``GPR[reg]_exit = GPR[entry_reg]_entry + trip_count * stride`` when
    ``trip_count`` is concrete and wrap-free, or ``GPR[reg]_exit =
    GPR[entry_reg]_entry + trip_expr * stride`` (parametric, doc 30 Phase A)
    when ``trip_count`` is absent and ``trip_expr`` is set. ``final_ctr`` is
    the CTR after exit. ``final_compare`` (when set) is applied to CR after the
    GPR updates.
    """

    header_pc: int
    latch_pc: int
    exit_pc: int
    trip_count: int | None
    final_gpr: dict[int, tuple[int, int]]
    final_ctr: int
    ranking: str
    proof_kind: str
    invariant_notes: tuple[str, ...]
    entry_condition: str
    exit_condition: str
    final_compare: FinalCompare | None = None
    trip_expr: dict[str, Any] | None = None
    zero_guard: str | None = None
    skip_guard: dict[str, Any] | None = None


def apply_compare_to_cr(
    state: Any,
    compare: FinalCompare,
    ops: Any,
) -> Any:
    """Apply a PPC integer compare into ``CR[field]``, including XER.SO in bit 0."""
    left = state.gpr[compare.left_reg]
    if compare.right_imm is not None:
        right = ops.const(int(compare.right_imm) & 0xFFFFFFFF)
    elif compare.right_reg is not None:
        right = state.gpr[compare.right_reg]
    else:
        raise ValueError("FinalCompare requires right_imm or right_reg")

    if compare.signed:
        lt = ops.signed_lt(left, right)
        gt = ops.signed_lt(right, left)
    else:
        lt = ops.unsigned_lt(left, right)
        gt = ops.unsigned_lt(right, left)
    nibble = ops.bor(
        ops.bor(
            ops.ite(lt, ops.const(8), ops.const(0)),
            ops.ite(gt, ops.const(4), ops.const(0)),
        ),
        ops.bor(
            ops.ite(ops.eq(left, right), ops.const(2), ops.const(0)),
            ops.ite(state.xer.so, ops.const(1), ops.const(0)),
        ),
    )
    field = int(compare.field)
    shift = (7 - field) * 4
    mask = ops.const(0xF << shift)
    return state.with_cr(
        ops.bor(ops.band(state.cr, ops.bnot(mask)), ops.shl(nibble, ops.const(shift))),
    )


def find_ctr_affine_loop_candidates(
    instructions: Sequence[Instruction],
) -> list[CtrAffineLoopCandidate]:
    """Scan for concrete ``mtctr`` + affine body + ``bdnz`` back-edge loops."""
    if not instructions:
        return []

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    candidates: list[CtrAffineLoopCandidate] = []

    for index, insn in enumerate(instructions):
        if not _is_bdnz(insn):
            continue
        target = int(insn.operands[2]) & 0xFFFFFFFC
        header_index = by_address.get(target)
        if header_index is None or header_index >= index:
            continue  # not a back-edge into this block

        body = list(instructions[header_index:index])
        updates, body_notes, body_ops = _parse_affine_body(body)
        if body_notes and updates is None:
            continue
        if any(
            not _body_op_fidelity_ok(instructions, header_index, index, op)
            for op in body_ops
        ):
            continue

        mtctr_index, guard = find_mtctr_with_guard(instructions, header_index)
        if mtctr_index is None:
            continue
        mtctr = instructions[mtctr_index]
        trip_reg = int(mtctr.operands[0])
        # A1: recognize the symbolic trip expression first; concrete
        # materializations (``li``, ``lis+ori``, ``addis+ori``, ``lwz`` of
        # readonly) fold to ``TripConstant`` and keep ``exact-pattern``.
        trip_expr, expr_notes = recognize_trip_expr(
            instructions,
            mtctr_index,
            trip_reg,
        )
        trip_count = (
            evaluate_concrete(trip_expr, {}) if trip_expr is not None else None
        )

        notes = list(expr_notes)
        notes.extend(body_notes)
        if guard is not None:
            notes.append(f"skip-guard candidate ({guard.family} @ 0x{guard.target_pc:08X})")
        if trip_count == 0:
            notes.append("CTR load of 0 wraps under bdnz (not a zero-trip loop)")
        if trip_count is not None and trip_count >= 1:
            confidence = "exact-pattern"
        elif trip_count == 0:
            # Concrete zero trip: ``bdnz`` wraps — never summarizable.
            confidence = "partial"
        elif trip_expr is not None:
            confidence = "symbolic-trip"
        else:
            confidence = "partial"

        candidates.append(
            CtrAffineLoopCandidate(
                mtctr_pc=mtctr.address,
                header_pc=instructions[header_index].address,
                latch_pc=insn.address,
                exit_pc=insn.address + 4,
                trip_count=trip_count,
                trip_count_reg=trip_reg,
                body_updates=tuple(updates or ()),
                instruction_indexes=tuple(range(mtctr_index, index + 1)),
                confidence=confidence,
                notes=tuple(notes),
                trip_expr=canonical_dict(trip_expr) if trip_expr is not None else None,
                skip_guard=guard.to_dict() if guard is not None else None,
                body_ops=tuple(body_ops) or None,
            ),
        )
    return candidates


def summarize_ctr_affine_loop(candidate: CtrAffineLoopCandidate) -> LoopSummary | None:
    """Build a closed-form ``LoopSummary`` (concrete) or a parametric one (symbolic).

    Concrete trips (``trip_count >= 1``) build the classic closed form. Symbolic
    trips (``trip_expr`` present) build a parametric summary — ``final =
    entry + trip_expr * stride`` — only when the zero-trip premise is
    established (a skip guard on the trip materialization; doc 30 Phase A1).
    The entry premise (``ctr == trip_expr AND trip_expr != 0``) is enforced at
    apply time; nothing here authorizes equivalence.
    """
    if candidate.trip_count is not None and candidate.trip_count >= 1:
        collapsed: dict[int, tuple[int, int]] = {}
        for update in candidate.body_updates:
            _entry, stride = collapsed.get(update.reg, (update.reg, 0))
            collapsed[update.reg] = (update.reg, stride + update.addend)
        return LoopSummary(
            header_pc=candidate.header_pc,
            latch_pc=candidate.latch_pc,
            exit_pc=candidate.exit_pc,
            trip_count=candidate.trip_count,
            final_gpr=collapsed,
            final_ctr=0,
            ranking="ctr-descending",
            proof_kind="affine-closed-form",
            invariant_notes=tuple(candidate.notes),
            entry_condition=f"CTR == {candidate.trip_count}",
            exit_condition="CTR == 0 after bdnz exhaust",
            final_compare=candidate.final_compare,
            trip_expr=candidate.trip_expr,
            zero_guard=(
                "concrete-nonzero"
                if candidate.skip_guard is None
                else "skip-branch"
            ),
            skip_guard=candidate.skip_guard,
        )

    if candidate.trip_expr is None or candidate.skip_guard is None:
        return None
    collapsed = {}
    for update in candidate.body_updates:
        _entry, stride = collapsed.get(update.reg, (update.reg, 0))
        collapsed[update.reg] = (update.reg, stride + update.addend)
    return LoopSummary(
        header_pc=candidate.header_pc,
        latch_pc=candidate.latch_pc,
        exit_pc=candidate.exit_pc,
        trip_count=None,
        final_gpr=collapsed,
        final_ctr=0,
        ranking="ctr-descending",
        proof_kind="affine-closed-form",
        invariant_notes=tuple(candidate.notes),
        entry_condition="CTR == trip_expr AND trip_expr != 0 (skip-guarded)",
        exit_condition="CTR == 0 after bdnz exhaust",
        final_compare=candidate.final_compare,
        trip_expr=candidate.trip_expr,
        zero_guard="skip-branch",
        skip_guard=candidate.skip_guard,
    )


def find_compare_affine_loop_candidates(
    instructions: Sequence[Instruction],
) -> list[CtrAffineLoopCandidate]:
    """Scan for countdown/count-up loops: ``addi/subi rT, ±step; cmp rT,
    bound; b<family> latch`` (doc 30 Phase C1).

    Latch families: ``bne (4,2)``, ``beq (12,2)``, ``blt (12,0)``,
    ``bgt (12,1)``, ``ble (4,1)``, ``bge (4,0)`` (with the static-prediction
    hint bits masked), ``AA=0``. The prelude is a self-``addi/subi`` with a
    nonzero step followed by ``cmpwi``/``cmplwi`` (immediate bound) or
    ``cmpw``/``cmplw`` (symbolic bound) on the same register. The trip is a
    do-while ``TripCountdown`` (C2): the body executes at least once whenever
    the header is reached.
    """
    if not instructions:
        return []

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    candidates: list[CtrAffineLoopCandidate] = []

    for index, insn in enumerate(instructions):
        family = _compare_latch_family(insn)
        if family is None:
            continue
        target = int(insn.operands[2]) & 0xFFFFFFFC
        header_index = by_address.get(target)
        if header_index is None or header_index >= index:
            continue

        body = list(instructions[header_index:index])
        parsed, body_notes = _parse_compare_affine_body(body, family)
        if parsed is None:
            continue
        counter_reg, step, updates, bound_imm, bound_reg, signed, body_ops = parsed
        if any(
            not _body_op_fidelity_ok(instructions, header_index, index, op)
            for op in body_ops
        ):
            continue

        # The counter's entry value (materialization before the header).
        counter_expr, counter_notes = recognize_trip_expr(
            instructions,
            header_index,
            counter_reg,
        )
        if counter_expr is None:
            counter_expr = TripEntryReg(counter_reg)
        if bound_reg is not None:
            bound_expr: TripExpr = TripEntryReg(bound_reg)
        else:
            assert bound_imm is not None
            bound_expr = TripConstant(bound_imm & 0xFFFFFFFF)
        trip_expr = TripCountdown(
            counter_expr,
            bound_expr,
            step,
            family,
            signed,
        )
        # Preserve the countdown *shape* in the candidate's trip_expr (the
        # folded constant is only for ``trip_count``); canonical_dict would
        # normalize a constant countdown into a TripConstant.
        trip_expr_dict: dict[str, Any] = {
            "kind": "countdown",
            "entry": canonical_dict(counter_expr),
            "bound": canonical_dict(bound_expr),
            "step": int(step),
            "family": family,
            "signed": bool(signed),
        }
        normalized = normalize_trip_expr(trip_expr)
        trip_count = (
            int(normalized.value) if isinstance(normalized, TripConstant) else None
        )

        notes = list(body_notes)
        notes.extend(counter_notes)
        notes.append(
            f"compare-affine countdown: {family} r{counter_reg} {step:+d}"
        )
        if trip_count is None and isinstance(normalized, TripCountdown):
            if (
                isinstance(normalized.entry, TripConstant)
                and isinstance(normalized.bound, TripConstant)
            ):
                notes.append("countdown non-terminating for the concrete entry/bound")
        if trip_count is not None and trip_count >= 1:
            confidence = "exact-pattern"
        elif isinstance(normalized, TripCountdown) and (
            not isinstance(normalized.entry, TripConstant)
            or not isinstance(normalized.bound, TripConstant)
        ):
            confidence = "symbolic-trip"
        else:
            confidence = "partial"
        final_compare = FinalCompare(
            field=0,
            left_reg=counter_reg,
            right_imm=bound_imm,
            right_reg=bound_reg,
            signed=signed,
        )
        candidates.append(
            CtrAffineLoopCandidate(
                mtctr_pc=instructions[header_index - 1].address,
                header_pc=instructions[header_index].address,
                latch_pc=insn.address,
                exit_pc=insn.address + 4,
                trip_count=trip_count,
                trip_count_reg=counter_reg,
                body_updates=tuple(updates),
                instruction_indexes=tuple(range(header_index - 1, index + 1)),
                confidence=confidence,
                notes=tuple(notes),
                final_compare=final_compare,
                trip_expr=trip_expr_dict,
                body_ops=tuple(body_ops) or None,
            ),
        )
    return candidates


def summarize_compare_affine_loop(candidate: CtrAffineLoopCandidate) -> LoopSummary | None:
    """Closed form for a compare-affine countdown loop (concrete or parametric).

    Concrete trips (positive ``trip_count``) build the classic closed form;
    symbolic trips (doc 30 Phase C2) build a parametric summary whose
    ``trip_expr`` is a do-while ``TripCountdown``. The countdown register's
    final value is ``entry + trip * step`` and the exit CR comes from the
    ``FinalCompare`` — both part of the exit agreement.
    """
    if "compare-affine" not in " ".join(candidate.notes):
        return None
    if candidate.trip_count is None and candidate.trip_expr is None:
        return None
    if any("non-terminating" in note for note in candidate.notes):
        # A concretely non-terminating countdown can never be summarized.
        return None

    collapsed: dict[int, tuple[int, int]] = {}
    for update in candidate.body_updates:
        _entry, stride = collapsed.get(update.reg, (update.reg, 0))
        collapsed[update.reg] = (update.reg, stride + update.addend)
    # Countdown register: entry N, stride = step → exit value N + trip*step.
    assert candidate.trip_count_reg is not None
    step = _countdown_step(candidate)
    collapsed[candidate.trip_count_reg] = (candidate.trip_count_reg, step)
    symbolic = candidate.trip_count is None

    return LoopSummary(
        header_pc=candidate.header_pc,
        latch_pc=candidate.latch_pc,
        exit_pc=candidate.exit_pc,
        trip_count=candidate.trip_count,
        final_gpr=collapsed,
        final_ctr=0,  # unused; apply preserves CTR for compare-affine
        ranking="counter-descending",
        proof_kind="compare-affine-closed-form",
        invariant_notes=tuple(candidate.notes),
        entry_condition=(
            f"r{candidate.trip_count_reg} == {candidate.trip_count}"
            if candidate.trip_count is not None
            else f"r{candidate.trip_count_reg} == trip_expr (do-while countdown)"
        ),
        exit_condition=(
            f"r{candidate.trip_count_reg} == 0 after bne exhaust"
            if candidate.trip_count is not None
            else "countdown exhausted at trip_expr iterations"
        ),
        final_compare=candidate.final_compare,
        trip_expr=candidate.trip_expr,
        zero_guard="concrete-nonzero" if not symbolic else None,
    )


def _countdown_step(candidate: CtrAffineLoopCandidate) -> int:
    """The signed per-iteration step of the countdown register.

    The step is the ``addi/subi`` immediate of the latch prelude; when the
    trip expression is a ``TripCountdown`` its ``step`` field is authoritative.
    """
    if candidate.trip_expr is not None:
        expr = _expr_from_dict(candidate.trip_expr)
        if isinstance(expr, TripCountdown):
            return int(expr.step)
    return -1


def _expr_from_dict(data: dict) -> TripExpr:
    from tools.ppc_equivalence.trip_expression import trip_expr_from_canonical

    return trip_expr_from_canonical(data)


def build_affine_summary_map(
    instructions: Sequence[Instruction],
) -> dict[int, LoopSummary]:
    """Map loop header PC → closed-form CTR or compare-affine summaries.

    Symbolic-trip CTR-affine summaries (parametric, doc 30 Phase A) are
    included when the candidate carries a skip guard; the apply-time entry
    premise and guard discharge gate their use.
    """
    mapping: dict[int, LoopSummary] = {}

    def _insert(summary: LoopSummary) -> None:
        if summary.header_pc in mapping:
            del mapping[summary.header_pc]
            return
        mapping[summary.header_pc] = summary

    for candidate in find_ctr_affine_loop_candidates(instructions):
        if candidate.confidence not in ("exact-pattern", "symbolic-trip"):
            continue
        summary = summarize_ctr_affine_loop(candidate)
        if summary is not None:
            _insert(summary)

    for candidate in find_compare_affine_loop_candidates(instructions):
        if candidate.confidence not in ("exact-pattern", "symbolic-trip"):
            continue
        summary = summarize_compare_affine_loop(candidate)
        if summary is not None:
            _insert(summary)

    return mapping


def apply_affine_loop_summary(state: Any, summary: LoopSummary, ops: Any) -> Any:
    """Return a post-loop state under the closed-form summary.

    Concrete trips use ``trip_count * stride``; symbolic trips (doc 30 Phase A)
    evaluate the ``trip_expr`` against the header-entry state and apply
    ``trip_value * stride`` — exact in 32-bit BV semantics for any symbolic
    trip (repeated ``addi`` accumulates mod 2^32 identically to one ``mul``).
    """
    if summary.trip_count is not None and summary.trip_count >= 1:
        trip_value = ops.const(int(summary.trip_count) & 0xFFFFFFFF)
    elif summary.trip_expr is not None:
        from tools.ppc_equivalence.trip_expression import trip_expr_from_canonical

        trip_value = evaluate_symbolic_affine(
            trip_expr_from_canonical(summary.trip_expr),
            state.gpr,
            ops,
        )
    else:
        raise ValueError(
            "affine summary requires a positive concrete trip count or trip_expr",
        )
    gprs = list(state.gpr)
    for reg, (entry_reg, stride) in summary.final_gpr.items():
        delta = ops.mul(trip_value, ops.const(int(stride) & 0xFFFFFFFF))
        gprs[reg] = ops.add(state.gpr[entry_reg], delta)
    if summary.proof_kind == "compare-affine-closed-form":
        # Compare-counted loops do not update CTR.
        result = replace(state, gpr=tuple(gprs))
    else:
        result = replace(
            state,
            gpr=tuple(gprs),
            ctr=ops.const(int(summary.final_ctr) & 0xFFFFFFFF),
        )
    if summary.final_compare is not None:
        result = apply_compare_to_cr(result, summary.final_compare, ops)
    return result


def evaluate_symbolic_affine(expr: Any, entry_gpr: Sequence[Any], ops: Any) -> Any:
    """Evaluate a trip expression against a GPR sequence (used by apply)."""
    from tools.ppc_equivalence.trip_expression import evaluate_symbolic

    return evaluate_symbolic(expr, dict(enumerate(entry_gpr)), ops)

def closed_form_gpr_value(entry_value: int, stride: int, trip_count: int) -> int:
    """Evaluate ``entry + trip_count * stride`` in 32-bit two's complement."""
    return (entry_value + trip_count * stride) & 0xFFFFFFFF


REQUIRED_LOOP_SUMMARY_KEYS = frozenset({
    "proof_kind",
    "header_pc",
    "latch_pc",
    "exit_pc",
    "final_ctr",
    "ranking",
    "final_gpr",
    "status",
    "algorithm",
    "summary_sha256",
})

# v1 algorithm ids are retained for concrete-trip obligations (existing
# certificates stay valid); v2 ids are added for symbolic-trip obligations
# (doc 30 Phase A3 / F-12).
_AFFINE_ALGORITHMS = frozenset({
    "affine-closed-form-v1",
    "compare-affine-closed-form-v1",
    "affine-closed-form-v2",
    "compare-affine-closed-form-v2",
})

_LOOP_SUMMARY_IDENTITY_KEYS = (
    "proof_kind",
    "header_pc",
    "latch_pc",
    "exit_pc",
    "trip_count",
    "final_ctr",
    "ranking",
    "final_gpr",
    "algorithm",
    "relational_companion",
    "trip_expr",
    "zero_guard",
)


def _affine_algorithm_for(proof_kind: str, *, symbolic: bool = False) -> str:
    if proof_kind == "compare-affine-closed-form":
        return "compare-affine-closed-form-v2" if symbolic else "compare-affine-closed-form-v1"
    return "affine-closed-form-v2" if symbolic else "affine-closed-form-v1"


def loop_summary_identity_payload(obligation: dict[str, Any]) -> dict[str, Any]:
    """Canonical fields hashed into ``summary_sha256`` (excludes status/coverage)."""
    payload: dict[str, Any] = {}
    for key in _LOOP_SUMMARY_IDENTITY_KEYS:
        if key not in obligation:
            continue
        value = obligation[key]
        if value is None:
            continue
        payload[key] = value
    return payload


def compute_loop_summary_sha256(obligation: dict[str, Any]) -> str:
    """SHA-256 over the canonical affine summary identity payload."""
    return canonical_json_sha256(loop_summary_identity_payload(obligation))


def build_loop_summary_obligation(
    summary: LoopSummary,
    *,
    coverage: str = "pending",
    status: str = "pending",
    relational_companion: str | None = None,
) -> dict[str, Any]:
    """Obligation block for ``proof_features: [\"affine-loop-summary\"]``.

    ``status=discharged`` is reserved for engine paths that also attach matching
    relational-induction UNSAT evidence (CTR-affine or compare-affine).
    Recognition or closed-form application alone must use ``pending`` / ``applied``.
    """
    final_gpr = [
        {
            "reg": reg,
            "entry_reg": entry_reg,
            "stride": stride,
        }
        for reg, (entry_reg, stride) in sorted(summary.final_gpr.items())
    ]
    symbolic = summary.trip_count is None and summary.trip_expr is not None
    algorithm = _affine_algorithm_for(summary.proof_kind, symbolic=symbolic)
    payload: dict[str, Any] = {
        "proof_kind": summary.proof_kind,
        "header_pc": summary.header_pc,
        "latch_pc": summary.latch_pc,
        "exit_pc": summary.exit_pc,
        "trip_count": summary.trip_count,
        "final_ctr": summary.final_ctr,
        "ranking": summary.ranking,
        "final_gpr": final_gpr,
        "coverage": coverage,
        "status": status,
        "algorithm": algorithm,
    }
    # Additive optional fields on symbolic-trip obligations only (doc 30 §4.6):
    # ``loop_summary_identity_payload`` skips ``None``, so v1 certs keep their
    # ``summary_sha256`` unchanged.
    if summary.trip_expr is not None:
        payload["trip_expr"] = dict(summary.trip_expr)
    if summary.zero_guard is not None:
        payload["zero_guard"] = summary.zero_guard
    if relational_companion is not None:
        payload["relational_companion"] = relational_companion
    payload["summary_sha256"] = compute_loop_summary_sha256(payload)
    return payload


def validate_loop_summary_obligation(obligation: dict[str, Any]) -> str | None:
    """Return None when a loop-summary obligation is structurally well-formed."""
    missing = sorted(REQUIRED_LOOP_SUMMARY_KEYS - obligation.keys())
    if missing:
        return "loop_summary missing " + ", ".join(missing)

    proof_kind = obligation.get("proof_kind")
    if not isinstance(proof_kind, str) or not proof_kind:
        return "loop_summary.proof_kind must be a nonempty string"

    for key in ("header_pc", "latch_pc", "exit_pc", "final_ctr"):
        value = obligation.get(key)
        if not isinstance(value, int) or value < 0 or value > 0xFFFFFFFF:
            return f"loop_summary.{key} must be a u32 int"

    trip_count = obligation.get("trip_count")
    if trip_count is None:
        # Symbolic-trip obligations (doc 30 Phase A) carry ``trip_expr`` instead.
        if obligation.get("trip_expr") is None:
            return "loop_summary requires trip_count or trip_expr"
    elif not isinstance(trip_count, int) or trip_count < 1 or trip_count > 0xFFFFFFFF:
        return "loop_summary.trip_count must be a positive u32 int"

    trip_expr = obligation.get("trip_expr")
    if trip_expr is not None:
        if not isinstance(trip_expr, dict) or "kind" not in trip_expr:
            return "loop_summary.trip_expr must be a canonical trip AST dict"

    zero_guard = obligation.get("zero_guard")
    if zero_guard is not None and zero_guard not in (
        "concrete-nonzero",
        "skip-branch",
    ):
        return "loop_summary.zero_guard must be concrete-nonzero|skip-branch"

    ranking = obligation.get("ranking")
    if not isinstance(ranking, str) or not ranking:
        return "loop_summary.ranking must be a nonempty string"

    status = obligation.get("status")
    if not isinstance(status, str) or status not in (
        "pending",
        "applied",
        "discharged",
        "failed",
    ):
        return "loop_summary.status must be pending|applied|discharged|failed"

    algorithm = obligation.get("algorithm")
    if algorithm not in _AFFINE_ALGORITHMS:
        return "loop_summary.algorithm must be a known affine closed-form id"

    digest = obligation.get("summary_sha256")
    if not isinstance(digest, str) or len(digest) != 64:
        return "loop_summary.summary_sha256 must be a 64-hex digest"
    try:
        int(digest, 16)
    except ValueError:
        return "loop_summary.summary_sha256 must be a 64-hex digest"
    expected = compute_loop_summary_sha256(obligation)
    if digest != expected:
        return "loop_summary.summary_sha256 does not match obligation identity"

    companion = obligation.get("relational_companion")
    if companion is not None and companion not in ("pending", "discharged", "failed"):
        return "loop_summary.relational_companion must be pending|discharged|failed"

    gpr_reason = _validate_final_gpr(obligation.get("final_gpr"))
    if gpr_reason is not None:
        return gpr_reason

    if status == "discharged":
        if proof_kind not in ("affine-closed-form", "compare-affine-closed-form"):
            return (
                "loop_summary.discharged requires proof_kind "
                "affine-closed-form|compare-affine-closed-form"
            )
        if companion != "discharged":
            return (
                "loop_summary.status=discharged requires "
                "relational_companion=discharged"
            )
        coverage = obligation.get("coverage")
        if coverage not in ("applied", "discharged"):
            return "loop_summary.status=discharged requires coverage applied|discharged"

    return None


def _validate_final_gpr(final_gpr: Any) -> str | None:
    if isinstance(final_gpr, list):
        seen: set[int] = set()
        for index, entry in enumerate(final_gpr):
            if not isinstance(entry, dict):
                return f"loop_summary.final_gpr[{index}] must be an object"
            reg = entry.get("reg")
            entry_reg = entry.get("entry_reg")
            stride = entry.get("stride")
            if not isinstance(reg, int) or reg < 0 or reg > 31:
                return f"loop_summary.final_gpr[{index}].reg must be a GPR index 0..31"
            if not isinstance(entry_reg, int) or entry_reg < 0 or entry_reg > 31:
                return (
                    f"loop_summary.final_gpr[{index}].entry_reg must be a GPR index 0..31"
                )
            if not isinstance(stride, int):
                return f"loop_summary.final_gpr[{index}].stride must be an int"
            if reg in seen:
                return f"duplicate loop_summary.final_gpr reg r{reg}"
            seen.add(reg)
        return None

    if isinstance(final_gpr, dict):
        seen_regs: set[int] = set()
        for reg_key, entry in final_gpr.items():
            if not isinstance(reg_key, str) or not reg_key.isdigit():
                return "loop_summary.final_gpr dict keys must be decimal reg strings"
            reg = int(reg_key)
            if reg < 0 or reg > 31:
                return f"loop_summary.final_gpr[{reg_key!r}] reg out of range 0..31"
            if reg in seen_regs:
                return f"duplicate loop_summary.final_gpr reg r{reg}"
            seen_regs.add(reg)
            if not isinstance(entry, dict):
                return f"loop_summary.final_gpr[{reg_key!r}] must be an object"
            entry_reg = entry.get("entry_reg")
            stride = entry.get("stride")
            if not isinstance(entry_reg, int) or entry_reg < 0 or entry_reg > 31:
                return (
                    f"loop_summary.final_gpr[{reg_key!r}].entry_reg must be a GPR index 0..31"
                )
            if not isinstance(stride, int):
                return f"loop_summary.final_gpr[{reg_key!r}].stride must be an int"
        return None

    return "loop_summary.final_gpr must be a list or object keyed by reg string"


_BNE_BO = 4  # branch if condition false
_CR0_EQ_BI = 2  # CR0 EQ bit


def _is_bdnz(insn: Instruction) -> bool:
    if insn.opcode != Opcode.BC or insn.link:
        return False
    bo, _bi, _target, _aa = insn.operands
    return int(bo) == _BDNZ_BO


def _compare_latch_family(insn: Instruction) -> str | None:
    """Decode a CR0 compare latch family (C1), or ``None``.

    Accepts ``bne (4,2)``, ``beq (12,2)``, ``blt (12,0)``, ``bgt (12,1)``,
    ``ble (4,1)``, ``bge (4,0)`` with ``AA=0``, including the
    static-prediction hint bits (BO values 13/15 and 5/7 alongside 12 and 4).
    """
    if insn.opcode != Opcode.BC or insn.link:
        return None
    bo, bi, _target, aa = (int(v) for v in insn.operands)
    if aa != 0:
        return None
    return guard_family(bo, bi)


def _body_op_result_reg(op: AffineBodyOp) -> int | None:
    """The GPR written by a whitelisted body op, or ``None``."""
    if op.opcode in (
        Opcode.ADD,
        Opcode.SUBF,
        Opcode.MULLI,
        Opcode.OR,
        Opcode.XOR,
        Opcode.RLWINM,
        Opcode.EXTSB,
    ):
        return int(op.operands[0])
    return None


def _body_op_fidelity_ok(
    instructions: Sequence[Instruction],
    header_index: int,
    latch_index: int,
    op: AffineBodyOp,
) -> bool:
    """The no-op summary treatment is fidelity-safe only for dead, callee-saved
    scratch registers (doc 30 Phase D2): the result register must be
    ``r13..r31`` and must not appear anywhere in the function outside the loop
    body (its value at loop entry and after the latch is provably irrelevant).
    EABI-volatile registers (``r0..r12``) can carry observable values across
    the function boundary (return channel, argument regs), so they are never
    no-op'd.
    """
    result_reg = _body_op_result_reg(op)
    if result_reg is None or result_reg <= 12:
        return False
    for index, insn in enumerate(instructions):
        if header_index <= index <= latch_index:
            continue
        if any(
            0 <= int(operand) <= 31 and int(operand) == result_reg
            for operand in insn.operands
        ):
            return False
    return True


def _parse_affine_body(
    body: Sequence[Instruction],
) -> tuple[list[AffineGprUpdate] | None, list[str], list[AffineBodyOp]]:
    """Parse a GPR-pure loop body (doc 30 Phase D2 whitelist).

    Returns ``(updates, notes, body_ops)``. Constant-stride self-``addi``/
    ``subi`` fold into :class:`AffineGprUpdate`; every other whitelisted
    opcode is recorded as an :class:`AffineBodyOp` modeled exactly in the
    relational discharge step. FP bodies and side-effecting opcodes are
    rejected.
    """
    if not body:
        return [], ["empty loop body"], []
    updates: list[AffineGprUpdate] = []
    body_ops: list[AffineBodyOp] = []
    notes: list[str] = []
    for insn in body:
        if insn.opcode == Opcode.ADDI:
            rt, ra, imm = (int(v) for v in insn.operands)
            if rt == ra and rt != 0:
                updates.append(AffineGprUpdate(reg=rt, addend=int(imm)))
                continue
            if rt == ra and rt == 0:
                notes.append("body writes r0 (volatile under EABI)")
                updates.append(AffineGprUpdate(reg=rt, addend=int(imm)))
                continue
            return None, [f"non-affine addi r{rt}, r{ra}, {imm}"], []
        if insn.opcode in _AFFINE_BODY_WHITELIST:
            operands = tuple(int(v) for v in insn.operands)
            # The whitelist ops must operate on dead/invariant registers only:
            # if any operand is an affine-accumulating register, the closed
            # form's fold would be unsound (the step model would diverge).
            if any(operand in {update.reg for update in updates} for operand in operands):
                return None, [
                    f"body op {insn.opcode.value} reads an affine register",
                ], []
            body_ops.append(AffineBodyOp(opcode=insn.opcode, operands=operands))
            continue
        return None, [f"unsupported body opcode {insn.opcode.value}"], []
    return updates, notes, body_ops


# GPR-pure, side-effect-free body opcodes (doc 30 Phase D2): each needs exact
# transition semantics in ``relational_discharge``'s step models. ``slwi`` /
# ``srwi`` decode as RLWINM; ``mr`` decodes as OR with equal sources.
_AFFINE_BODY_WHITELIST = frozenset({
    Opcode.ADD,
    Opcode.SUBF,
    Opcode.MULLI,
    Opcode.OR,
    Opcode.XOR,
    Opcode.RLWINM,
    Opcode.EXTSB,
})


def _parse_compare_affine_body(
    body: Sequence[Instruction],
    family: str,
) -> tuple[
    tuple[
        int, int, list[AffineGprUpdate], int | None, int | None, bool,
        list[AffineBodyOp],
    ]
    | None,
    list[str],
]:
    """Parse ``[addi…]; addi/subi rT, ±step; cmp rT, bound`` body (latch excluded).

    Returns ``(counter_reg, step, updates, bound_imm, bound_reg, signed)``.
    ``step`` is the signed per-iteration delta; ``bound_imm`` XOR
    ``bound_reg`` carries the compare operand; ``signed`` is True for
    ``cmpwi``/``cmpw`` and False for ``cmplwi``/``cmplw``.
    """
    del family  # the continue relation is carried on the candidate via the latch
    if len(body) < 2:
        return None, ["compare-affine body too short"]

    cmp_insn = body[-1]
    dec_insn = body[-2]
    if cmp_insn.opcode not in (Opcode.CMPWI, Opcode.CMPLWI, Opcode.CMPW, Opcode.CMPLW):
        return None, ["compare-affine latch prelude is not a compare"]
    field, cmp_ra, cmp_imm = (int(v) for v in cmp_insn.operands[:3])
    if field != 0:
        return None, ["compare-affine requires CR0 compare"]
    signed = cmp_insn.opcode in (Opcode.CMPWI, Opcode.CMPW)
    if cmp_insn.opcode in (Opcode.CMPW, Opcode.CMPLW):
        bound_imm: int | None = None
        bound_reg: int | None = cmp_imm
    else:
        bound_imm = cmp_imm & 0xFFFFFFFF
        bound_reg = None

    if dec_insn.opcode != Opcode.ADDI:
        return None, ["compare-affine missing addi/subi before compare"]
    rt, ra, imm = (int(v) for v in dec_insn.operands)
    if rt != ra:
        return None, ["compare-affine step is not addi rT, rT, imm"]
    step = _sign_extend_16(imm)
    if step == 0:
        return None, ["compare-affine step is zero"]
    if rt != cmp_ra:
        return None, ["compare-affine compare register mismatch"]
    if rt == 0:
        return None, ["compare-affine counter cannot be r0"]

    rest = body[:-2]
    updates, notes, body_ops = _parse_affine_body(rest)
    if updates is None:
        return None, notes
    for update in updates:
        if update.reg == rt:
            return None, ["compare-affine body updates the counter register"]
    for op in body_ops:
        if any(operand == rt for operand in op.operands):
            return None, ["compare-affine body op reads the counter register"]
    return (rt, step, updates, bound_imm, bound_reg, signed, body_ops), notes


_COMPARE_FAMILIES = frozenset({"bne", "beq", "blt", "bgt", "ble", "bge"})


def _sign_extend_16(value: int) -> int:
    word = int(value) & 0xFFFF
    if word >= 0x8000:
        return word - 0x10000
    return word
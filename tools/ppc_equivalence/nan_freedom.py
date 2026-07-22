"""NaN-freedom gate for symbolic FP EQUIVALENT proofs (H2).

Z3's SMT-LIB floating-point theory collapses every NaN payload to a single
abstract NaN, so ``fpToIEEEBV`` cannot distinguish two Broadway sequences that
differ only in NaN payload bits. A divergence query over such collapsed values
can therefore report ``unsat`` (EQUIVALENT) even though the real hardware would
produce different payloads.

Policy A: before authorizing EQUIVALENT with ``allow_nan=True`` on a proof that
used FP, prove that no feasible, mutually-defined terminal pair can carry a NaN
in a *compared* FPR / PS1 lane **whose value was produced through the FP
theory** and **differs between the two implementations**. If such a NaN is
reachable, fail closed (``INCONCLUSIVE_UNSUPPORTED``); otherwise keep
EQUIVALENT and record the ``nan-freedom:proven`` assumption.

Two important exemptions keep the gate precise (no false demotions):

* **Syntactically identical results** are the same deterministic function of
  the shared symbolic inputs. z3 canonicalising NaN cannot hide a divergence
  that does not exist, so same-code / structurally-equal observables are
  skipped. This preserves ``f(x) ≡ f(x)`` FP proofs.
* **Bit-preserving values** (``fmr``/``fabs``/``fneg``/``fnabs`` sign ops, raw
  ``lfd`` loads, integer moves) never route through Z3's FP theory. Their
  payload bits are compared exactly by the divergence query, so no collapse is
  possible and they are skipped. This preserves ``fp-bitwise`` proofs.
"""

from __future__ import annotations

from typing import Any, Sequence

from .contract import EquivalenceContract, Observable
from .result import FloatingPointDomain, ProofResult, ProofStatus


NAN_FREEDOM_ASSUMPTION = "nan-freedom:proven"
NAN_FREEDOM_UNSUPPORTED = (
    "nan-freedom:feasible NaN in a differing FP-theory FPR observable under "
    "allow_nan=True; symbolic FP cannot certify NaN payload identity "
    "(Z3 single-NaN theory)"
)


def _observable_bits(state: Any, observable: Observable) -> Any:
    assert observable.index is not None
    if observable.kind == "ps1":
        return state.ps1[observable.index]
    return state.fpr[observable.index]


def _uses_fp_theory(expr: Any, z3: Any) -> bool:
    """True when ``expr`` contains a floating-point-sort subterm.

    Values built only from bit-vector operations (sign-bit moves, raw ``lfd``
    concatenations, integer copies) have no FP-sort node in their DAG and so
    are never subject to the single-NaN collapse; arithmetic / convert /
    compare / single-load results carry an ``fpToIEEEBV(fp …)`` cone.
    """
    fp_kind = getattr(z3, "Z3_FLOATING_POINT_SORT", None)
    if fp_kind is None:  # pragma: no cover - z3 always exposes the enum
        return True
    seen: set[int] = set()
    stack = [expr]
    while stack:
        node = stack.pop()
        try:
            node_id = node.get_id()
        except Exception:
            continue
        if node_id in seen:
            continue
        seen.add(node_id)
        try:
            if node.sort_kind() == fp_kind:
                return True
        except Exception:
            pass
        try:
            stack.extend(node.children())
        except Exception:
            pass
    return False


def observable_fpr_nan_predicate(
    original_exits: Sequence[Any],
    candidate_exits: Sequence[Any],
    contract: EquivalenceContract,
    ops: Any,
) -> Any | None:
    """Return a predicate true when a collapse-prone NaN is reachable.

    The predicate is satisfiable iff some feasible, mutually-defined terminal
    pair exposes a NaN in a compared FPR / PS1 lane whose value is produced by
    the FP theory and differs syntactically between the two sides.  ``None``
    means no such lane exists (skip the gate).
    """
    from .contract import observables_for_exit

    z3 = ops.z3
    clauses: list[Any] = []
    for left in original_exits:
        for right in candidate_exits:
            if left.exit_kind == right.exit_kind:
                compared = observables_for_exit(contract, left.exit_kind)
            else:
                # Mismatched exit kinds already diverge and never reach this
                # EQUIVALENT gate; compare the full set defensively.
                compared = contract.observables
            lane_nans: list[Any] = []
            for observable in compared:
                if observable.kind not in ("fpr", "ps1"):
                    continue
                if observable.index is None:
                    continue
                left_bits = _observable_bits(left.state, observable)
                right_bits = _observable_bits(right.state, observable)
                # Identical deterministic result of shared inputs: no collapse
                # can manufacture a false equivalence.
                if left_bits is right_bits or z3.eq(left_bits, right_bits):
                    continue
                # Pure bit-vector values are compared exactly by the divergence
                # query; only FP-theory values can be collapsed.
                if not (
                    _uses_fp_theory(left_bits, z3)
                    or _uses_fp_theory(right_bits, z3)
                ):
                    continue
                lane_nans.append(ops.fp_is_nan(ops.fp_bits_to_double(left_bits)))
                lane_nans.append(ops.fp_is_nan(ops.fp_bits_to_double(right_bits)))
            if not lane_nans:
                continue
            guard = [left.condition, right.condition, z3.Or(*lane_nans)]
            # NaN payloads only matter where both sides are defined: an invalid
            # side already suppresses value comparison in the divergence query.
            if left.state.valid is not None:
                guard.append(left.state.valid)
            if right.state.valid is not None:
                guard.append(right.state.valid)
            clauses.append(z3.And(*guard))
    if not clauses:
        return None
    return z3.Or(*clauses)


def enforce_nan_freedom(
    result: ProofResult,
    *,
    original_exits: Sequence[Any],
    candidate_exits: Sequence[Any],
    contract: EquivalenceContract,
    ops: Any,
    domain: FloatingPointDomain,
    layout_constraints: Sequence[Any],
    used_fp: bool,
    deadline_ms: int = 5_000,
) -> ProofResult:
    """Demote EQUIVALENT when a collapse-prone NaN is reachable under allow_nan."""
    if result.status is not ProofStatus.EQUIVALENT:
        return result
    if not used_fp:
        return result
    if not domain.allow_nan:
        # Domain already excludes NaN inputs via definedness; record honesty.
        if "nan-freedom:domain-allow_nan=false" not in result.assumptions:
            result.assumptions.append("nan-freedom:domain-allow_nan=false")
        return result

    predicate = observable_fpr_nan_predicate(
        original_exits, candidate_exits, contract, ops,
    )
    if predicate is None:
        return result

    z3 = ops.z3
    solver = z3.Solver()
    solver.set(timeout=max(1, int(deadline_ms)))
    for constraint in layout_constraints:
        solver.add(constraint)
    solver.add(predicate)
    answer = solver.check()
    if answer == z3.unsat:
        if NAN_FREEDOM_ASSUMPTION not in result.assumptions:
            result.assumptions.append(NAN_FREEDOM_ASSUMPTION)
        return result
    if answer == z3.sat:
        result.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
        result.unsupported.append(NAN_FREEDOM_UNSUPPORTED)
        result.warnings.append(NAN_FREEDOM_UNSUPPORTED)
        result.abstractions.append("nan-payload-unmodeled")
        return result
    # unknown / timeout: fail closed (never keep EQUIVALENT on an undecided gate).
    reason = (
        "nan-freedom query inconclusive"
        f" ({solver.reason_unknown()})"
        if answer == z3.unknown
        else "nan-freedom query inconclusive (timeout)"
    )
    result.status = (
        ProofStatus.INCONCLUSIVE_TIMEOUT
        if answer != z3.unknown or "timeout" in solver.reason_unknown().lower()
        else ProofStatus.INCONCLUSIVE_UNKNOWN
    )
    result.warnings.append(reason)
    result.unsupported.append(reason)
    return result

"""Acceptance gates for non-match session types.

Each gate builds the unit once (via verify.build_unit), evaluates, and returns
a contracts.Verdict.  For non-match types ``target_symbol`` is always '' and
``target_mismatches`` is None.

Design reference: docs/llm_decomp_design.md §10 catalog.
"""
from __future__ import annotations

from pathlib import Path

from tools.llm_decomp import verify
from tools.llm_decomp.contracts import (
    Baseline,
    Regression,
    SessionMeta,
    Verdict,
    VerdictKind,
)

# ── dispatcher ─────────────────────────────────────────────────────────────


def gate_for(policy_gate: str):
    """Return the gate function for a SessionPolicy.gate string.

    Returns ``None`` for ``'match'`` and ``'rename'`` (these session types use
    ``verify.verdict_for_submit`` or are tool-driven respectively).

    Raises ``ValueError`` for unknown gate names.
    """
    _gates = {
        "byte_identical": gate_byte_identical,
        "type_recovery": gate_type_recovery,
        "size_trim": gate_size_trim,
    }
    if policy_gate in ("match", "rename"):
        return None
    if policy_gate not in _gates:
        raise ValueError(
            f"unknown gate {policy_gate!r}; expected one of "
            f"{list(_gates)} | match | rename"
        )
    return _gates[policy_gate]


# ── shared helpers ─────────────────────────────────────────────────────────


def _build_and_sweep(
    repo_root: Path, meta: SessionMeta, baseline: Baseline
) -> tuple[bool, str, Path | None, list[Regression], list[str]]:
    """Build the unit, locate the object, run sweep and added-symbols check.

    Returns ``(success, diagnostics, current_obj, regressions, added)``.
    When *success* is False the remaining fields are empty / None.
    """
    success, diagnostics = verify.build_unit(repo_root, meta.unit)
    if not success:
        return False, diagnostics, None, [], []

    obj = verify.current_object_path(repo_root, meta.unit)
    regressions = verify.sweep(repo_root, meta.unit, baseline, obj)
    added = verify.added_symbols(baseline, obj)
    return True, "", obj, regressions, added


def _verdict_compile_error(diagnostics: str) -> Verdict:
    return Verdict(
        kind=VerdictKind.COMPILE_ERROR,
        accepted=False,
        target_symbol="",
        target_mismatches=None,
        rule="compile_error",
        diagnostics=diagnostics,
    )


# ── gate_byte_identical (tu-cleanup) ──────────────────────────────────────


def gate_byte_identical(
    repo_root: Path, meta: SessionMeta, baseline: Baseline
) -> Verdict:
    """tu-cleanup gate: every baseline symbol byte-identical, no added
    symbols, text size <= budget."""
    success, diagnostics = verify.build_unit(repo_root, meta.unit)
    if not success:
        return _verdict_compile_error(diagnostics)

    obj = verify.current_object_path(repo_root, meta.unit)
    regressions = verify.sweep(repo_root, meta.unit, baseline, obj)
    added = verify.added_symbols(baseline, obj)
    size_check = verify.check_size(repo_root, meta.unit)

    if regressions:
        return Verdict(
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            target_symbol="",
            target_mismatches=None,
            regressions=regressions,
            rule="sibling_regression",
        )
    if added:
        return Verdict(
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            target_symbol="",
            target_mismatches=None,
            rule="added_symbol",
        )

    size_ok = (
        size_check.budget is None
        or (
            size_check.decomp_text is not None
            and size_check.decomp_text <= size_check.budget
        )
    )
    if not size_ok:
        return Verdict(
            kind=VerdictKind.SOFT_REJECT,
            accepted=False,
            target_symbol="",
            target_mismatches=None,
            text_size=size_check.decomp_text or 0,
            text_budget=size_check.budget,
            rule="size_overflow",
        )

    return Verdict(
        kind=VerdictKind.ACCEPTED,
        accepted=True,
        target_symbol="",
        target_mismatches=None,
        text_size=size_check.decomp_text or 0,
        text_budget=size_check.budget,
        rule="byte_identical",
    )


# ── gate_type_recovery ────────────────────────────────────────────────────


def gate_type_recovery(
    repo_root: Path, meta: SessionMeta, baseline: Baseline
) -> Verdict:
    """type-recovery gate: builds, zero regressions, no added EXPORTED
    symbols.  Size unchanged or smaller is fine — do NOT fail on size."""
    success, diagnostics = verify.build_unit(repo_root, meta.unit)
    if not success:
        return _verdict_compile_error(diagnostics)

    obj = verify.current_object_path(repo_root, meta.unit)
    regressions = verify.sweep(repo_root, meta.unit, baseline, obj)
    added = verify.added_symbols(baseline, obj)

    if regressions:
        return Verdict(
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            target_symbol="",
            target_mismatches=None,
            regressions=regressions,
            rule="sibling_regression",
        )
    if added:
        return Verdict(
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            target_symbol="",
            target_mismatches=None,
            rule="added_symbol",
        )

    return Verdict(
        kind=VerdictKind.ACCEPTED,
        accepted=True,
        target_symbol="",
        target_mismatches=None,
        rule="types_ok",
    )


# ── gate_size_trim ────────────────────────────────────────────────────────


def gate_size_trim(
    repo_root: Path, meta: SessionMeta, baseline: Baseline
) -> Verdict:
    """size-trim gate: matched symbols stay byte-identical, unmatched symbols
    do not get worse, no added symbols, and current .text is strictly smaller
    than baseline.text_size."""
    success, diagnostics = verify.build_unit(repo_root, meta.unit)
    if not success:
        return _verdict_compile_error(diagnostics)

    obj = verify.current_object_path(repo_root, meta.unit)
    regressions = verify.sweep(repo_root, meta.unit, baseline, obj)
    added = verify.added_symbols(baseline, obj)

    # Check matched-symbol regressions first (hard reject)
    for r in regressions:
        if r.was_matched:
            return Verdict(
                kind=VerdictKind.HARD_REJECT,
                accepted=False,
                target_symbol="",
                target_mismatches=None,
                regressions=regressions,
                rule="sibling_regression",
            )

    # Unmatched symbols that got worse
    for r in regressions:
        if r.current_mismatches > r.baseline_mismatches:
            return Verdict(
                kind=VerdictKind.SOFT_REJECT,
                accepted=False,
                target_symbol="",
                target_mismatches=None,
                regressions=regressions,
                rule="sibling_worse",
            )

    if added:
        return Verdict(
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            target_symbol="",
            target_mismatches=None,
            rule="added_symbol",
        )

    # Check current .text size < baseline.text_size (strictly smaller)
    size_check = verify.check_size(repo_root, meta.unit)
    current_text = size_check.decomp_text or 0

    if current_text >= baseline.text_size:
        return Verdict(
            kind=VerdictKind.SOFT_REJECT,
            accepted=False,
            target_symbol="",
            target_mismatches=None,
            text_size=current_text,
            text_budget=baseline.text_size,
            rule="not_reduced",
        )

    return Verdict(
        kind=VerdictKind.ACCEPTED,
        accepted=True,
        target_symbol="",
        target_mismatches=None,
        text_size=current_text,
        text_budget=baseline.text_size,
        rule="size_reduced",
    )
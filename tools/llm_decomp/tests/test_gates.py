"""Tests for tools.llm_decomp.gates — non-match acceptance gates.

All external dependencies (verify.build_unit, verify.sweep, etc.) are mocked.
No real toolchain is required.
"""
from __future__ import annotations

import unittest
from pathlib import Path
from unittest.mock import patch

from tools.coop.lib.object_size import ObjectSizeCheck
from tools.llm_decomp.contracts import (
    Baseline,
    Regression,
    SessionMeta,
    SessionStatus,
    SymbolBaseline,
    VerdictKind,
)

# ── Test parameters ────────────────────────────────────────────────────────

REPO_ROOT = Path("/fake/repo")
UNIT = "test/unit"

BASELINE = Baseline(
    unit=UNIT,
    symbols={
        "sym_a": SymbolBaseline(byte_hash="aa", mismatch_count=0),  # matched
        "sym_b": SymbolBaseline(byte_hash="bb", mismatch_count=5),  # unmatched
        "sym_c": SymbolBaseline(byte_hash="cc", mismatch_count=2),  # unmatched
    },
    text_size=100,
    text_budget=120,
    object_path="/tmp/baseline.o",
)

META = SessionMeta(
    target_id="test",
    session_id="test-session",
    session_type="tu-cleanup",
    unit=UNIT,
    symbol="",
    status=SessionStatus.MATCHING,
    writable=["test/unit.cpp"],
    owner="test",
    created_at="2026-01-01T00:00:00",
)

SIZE_OK = ObjectSizeCheck(
    unit_hint=UNIT,
    split_path="test/unit.cpp",
    budget=120,
    retail_text=90,
    decomp_text=100,
    ok=True,
    notes="ok",
)

SIZE_OVERFLOW = ObjectSizeCheck(
    unit_hint=UNIT,
    split_path="test/unit.cpp",
    budget=120,
    retail_text=90,
    decomp_text=150,
    ok=False,
    notes="over budget",
)

SIZE_EQUAL = ObjectSizeCheck(
    unit_hint=UNIT,
    split_path="test/unit.cpp",
    budget=120,
    retail_text=90,
    decomp_text=100,
    ok=True,
    notes="equal to baseline",
)

SIZE_SMALLER = ObjectSizeCheck(
    unit_hint=UNIT,
    split_path="test/unit.cpp",
    budget=120,
    retail_text=90,
    decomp_text=80,
    ok=True,
    notes="smaller than baseline",
)

SIZE_LARGER = ObjectSizeCheck(
    unit_hint=UNIT,
    split_path="test/unit.cpp",
    budget=120,
    retail_text=90,
    decomp_text=110,
    ok=True,
    notes="larger than baseline",
)

# ── Helpers ────────────────────────────────────────────────────────────────


def _regression(
    symbol: str,
    baseline_mismatches: int,
    current_mismatches: int,
) -> Regression:
    return Regression(
        symbol=symbol,
        baseline_mismatches=baseline_mismatches,
        current_mismatches=current_mismatches,
        was_matched=baseline_mismatches == 0,
    )


class GateTestBase(unittest.TestCase):
    """Base class providing helpers for patching verify.* calls."""

    def setUp(self) -> None:
        self.patches: list = []

    def tearDown(self) -> None:
        for p in self.patches:
            p.stop()

    def _patch_verify(
        self,
        build_result: tuple[bool, str] = (True, ""),
        regressions: list[Regression] | None = None,
        added: list[str] | None = None,
        size_check: ObjectSizeCheck | None = None,
    ) -> None:
        """Patch all verify functions that gates.py imports."""
        if regressions is None:
            regressions = []
        if added is None:
            added = []
        if size_check is None:
            size_check = SIZE_OK

        p_build = patch(
            "tools.llm_decomp.gates.verify.build_unit",
            return_value=build_result,
        )
        p_obj = patch(
            "tools.llm_decomp.gates.verify.current_object_path",
            return_value=Path("/tmp/current.o"),
        )
        p_sweep = patch(
            "tools.llm_decomp.gates.verify.sweep",
            return_value=regressions,
        )
        p_added = patch(
            "tools.llm_decomp.gates.verify.added_symbols",
            return_value=added,
        )
        p_size = patch(
            "tools.llm_decomp.gates.verify.check_size",
            return_value=size_check,
        )

        for p in (p_build, p_obj, p_sweep, p_added, p_size):
            p.start()
            self.patches.append(p)

    def _assert_verdict(
        self,
        verdict,
        *,
        kind: VerdictKind,
        accepted: bool,
        rule: str,
        text_size: int = 0,
        text_budget: int | None = None,
        target_symbol: str = "",
        target_mismatches=None,
    ) -> None:
        self.assertEqual(verdict.kind, kind)
        self.assertEqual(verdict.accepted, accepted)
        self.assertEqual(verdict.rule, rule)
        self.assertEqual(verdict.text_size, text_size)
        self.assertEqual(verdict.text_budget, text_budget)
        self.assertEqual(verdict.target_symbol, target_symbol)
        self.assertEqual(verdict.target_mismatches, target_mismatches)


# ── gate_for ───────────────────────────────────────────────────────────────


class TestGateFor(unittest.TestCase):
    """gate_for dispatches to the right gate or returns None."""

    def test_byte_identical(self) -> None:
        from tools.llm_decomp.gates import gate_for, gate_byte_identical

        self.assertIs(gate_for("byte_identical"), gate_byte_identical)

    def test_type_recovery(self) -> None:
        from tools.llm_decomp.gates import gate_for, gate_type_recovery

        self.assertIs(gate_for("type_recovery"), gate_type_recovery)

    def test_size_trim(self) -> None:
        from tools.llm_decomp.gates import gate_for, gate_size_trim

        self.assertIs(gate_for("size_trim"), gate_size_trim)

    def test_match_returns_none(self) -> None:
        from tools.llm_decomp.gates import gate_for

        self.assertIsNone(gate_for("match"))

    def test_rename_returns_none(self) -> None:
        from tools.llm_decomp.gates import gate_for

        self.assertIsNone(gate_for("rename"))

    def test_unknown_raises(self) -> None:
        from tools.llm_decomp.gates import gate_for

        with self.assertRaises(ValueError):
            gate_for("bogus")


# ── gate_byte_identical ────────────────────────────────────────────────────


class TestGateByteIdentical(GateTestBase):
    """tu-cleanup gate tests."""

    def test_compile_error(self) -> None:
        from tools.llm_decomp.gates import gate_byte_identical

        self._patch_verify(build_result=(False, "syntax error"))
        verdict = gate_byte_identical(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.COMPILE_ERROR,
            accepted=False,
            rule="compile_error",
        )
        self.assertIn("syntax error", verdict.diagnostics)

    def test_added_symbol_rejected(self) -> None:
        from tools.llm_decomp.gates import gate_byte_identical

        self._patch_verify(added=["new_func"])
        verdict = gate_byte_identical(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            rule="added_symbol",
        )

    def test_matched_regression_rejected(self) -> None:
        from tools.llm_decomp.gates import gate_byte_identical

        regs = [_regression("sym_a", 0, 3)]
        self._patch_verify(regressions=regs)
        verdict = gate_byte_identical(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            rule="sibling_regression",
        )

    def test_any_regression_rejected_even_unmatched(self) -> None:
        """tu-cleanup rejects even when only unmatched symbols regress."""
        from tools.llm_decomp.gates import gate_byte_identical

        regs = [_regression("sym_b", 5, 7)]
        self._patch_verify(regressions=regs)
        verdict = gate_byte_identical(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            rule="sibling_regression",
        )

    def test_size_overflow_soft_reject(self) -> None:
        from tools.llm_decomp.gates import gate_byte_identical

        self._patch_verify(size_check=SIZE_OVERFLOW)
        verdict = gate_byte_identical(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.SOFT_REJECT,
            accepted=False,
            rule="size_overflow",
            text_size=150,
            text_budget=120,
        )

    def test_accepts_clean(self) -> None:
        from tools.llm_decomp.gates import gate_byte_identical

        self._patch_verify()
        verdict = gate_byte_identical(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.ACCEPTED,
            accepted=True,
            rule="byte_identical",
            text_size=100,
            text_budget=120,
        )


# ── gate_type_recovery ─────────────────────────────────────────────────────


class TestGateTypeRecovery(GateTestBase):
    """type-recovery gate tests."""

    def test_compile_error(self) -> None:
        from tools.llm_decomp.gates import gate_type_recovery

        self._patch_verify(build_result=(False, "no such file"))
        verdict = gate_type_recovery(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.COMPILE_ERROR,
            accepted=False,
            rule="compile_error",
        )

    def test_added_exported_symbol_rejected(self) -> None:
        from tools.llm_decomp.gates import gate_type_recovery

        self._patch_verify(added=["new_global_fn"])
        verdict = gate_type_recovery(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            rule="added_symbol",
        )

    def test_regression_rejected(self) -> None:
        from tools.llm_decomp.gates import gate_type_recovery

        regs = [_regression("sym_a", 0, 2)]
        self._patch_verify(regressions=regs)
        verdict = gate_type_recovery(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            rule="sibling_regression",
        )

    def test_accepts_clean_with_unchanged_size(self) -> None:
        """type-recovery tolerates unchanged (or any) size."""
        from tools.llm_decomp.gates import gate_type_recovery

        self._patch_verify(size_check=SIZE_EQUAL)
        verdict = gate_type_recovery(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.ACCEPTED,
            accepted=True,
            rule="types_ok",
        )


# ── gate_size_trim ─────────────────────────────────────────────────────────


class TestGateSizeTrim(GateTestBase):
    """size-trim gate tests."""

    def test_compile_error(self) -> None:
        from tools.llm_decomp.gates import gate_size_trim

        self._patch_verify(build_result=(False, "fail"))
        verdict = gate_size_trim(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.COMPILE_ERROR,
            accepted=False,
            rule="compile_error",
        )

    def test_matched_regression_rejected(self) -> None:
        from tools.llm_decomp.gates import gate_size_trim

        regs = [_regression("sym_a", 0, 2)]
        self._patch_verify(regressions=regs, size_check=SIZE_SMALLER)
        verdict = gate_size_trim(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            rule="sibling_regression",
        )

    def test_added_symbol_rejected(self) -> None:
        from tools.llm_decomp.gates import gate_size_trim

        self._patch_verify(added=["new_func"], size_check=SIZE_SMALLER)
        verdict = gate_size_trim(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.HARD_REJECT,
            accepted=False,
            rule="added_symbol",
        )

    def test_size_equal_rejected(self) -> None:
        """size-trim rejects when .text size equals baseline."""
        from tools.llm_decomp.gates import gate_size_trim

        self._patch_verify(size_check=SIZE_EQUAL)
        verdict = gate_size_trim(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.SOFT_REJECT,
            accepted=False,
            rule="not_reduced",
            text_size=100,
            text_budget=100,
        )

    def test_size_larger_rejected(self) -> None:
        """size-trim rejects when .text size is larger than baseline."""
        from tools.llm_decomp.gates import gate_size_trim

        self._patch_verify(size_check=SIZE_LARGER)
        verdict = gate_size_trim(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.SOFT_REJECT,
            accepted=False,
            rule="not_reduced",
            text_size=110,
            text_budget=100,
        )

    def test_unmatched_got_worse_rejected(self) -> None:
        """size-trim rejects when an unmatched symbol regresses (more mismatches)."""
        from tools.llm_decomp.gates import gate_size_trim

        regs = [_regression("sym_b", 5, 8)]
        self._patch_verify(regressions=regs, size_check=SIZE_SMALLER)
        verdict = gate_size_trim(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.SOFT_REJECT,
            accepted=False,
            rule="sibling_worse",
        )

    def test_unmatched_improved_accepted(self) -> None:
        """size-trim tolerates unmatched symbols that improved (fewer mismatches)."""
        from tools.llm_decomp.gates import gate_size_trim

        # sym_b improved from 5 to 3; sweep reports it as a hash change
        regs = [_regression("sym_b", 5, 3)]
        self._patch_verify(regressions=regs, size_check=SIZE_SMALLER)
        verdict = gate_size_trim(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.ACCEPTED,
            accepted=True,
            rule="size_reduced",
            text_size=80,
            text_budget=100,
        )

    def test_matched_identical_unmatched_improved_size_smaller_accepted(
        self,
    ) -> None:
        """Full acceptance: matched symbols identical, unmatched improved,
        size strictly smaller."""
        from tools.llm_decomp.gates import gate_size_trim

        regs = [
            _regression("sym_b", 5, 3),  # unmatched improved
            _regression("sym_c", 2, 1),  # unmatched improved
        ]
        self._patch_verify(regressions=regs, size_check=SIZE_SMALLER)
        verdict = gate_size_trim(REPO_ROOT, META, BASELINE)
        self._assert_verdict(
            verdict,
            kind=VerdictKind.ACCEPTED,
            accepted=True,
            rule="size_reduced",
            text_size=80,
            text_budget=100,
        )
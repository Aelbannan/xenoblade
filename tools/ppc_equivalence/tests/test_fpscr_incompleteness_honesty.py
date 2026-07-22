"""M4: FPSCR sticky incompleteness fails closed.

The SymbolicOps equivalence path does not SMT-model the OX/UX/XX FPSCR
stickies. When ``fpscr`` is a compared observable after real FP arithmetic,
EQUIVALENT must be demoted to ``INCONCLUSIVE_ABSTRACTION`` rather than
silently claiming a full FPSCR equivalence.
"""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_fpscr import (
    FPSCR_STICKIES_UNSUPPORTED,
    FPSCR_SYMBOLIC_STICKIES_ASSUMPTION,
    annotate_fpscr_sticky_incompleteness,
    fpscr_sticky_modeling_status,
    symbolic_fpscr_stickies_incomplete,
)
from tools.ppc_equivalence.result import ProofResult, ProofStatus


class FpscrModelingStatusTests(unittest.TestCase):
    def test_default_symbolic_status_is_honest_about_ox_ux_xx(self) -> None:
        status = fpscr_sticky_modeling_status()
        self.assertFalse(status["ox"])
        self.assertFalse(status["ux"])
        self.assertFalse(status["xx"])

    def test_symbolic_incomplete_flag_true(self) -> None:
        self.assertTrue(symbolic_fpscr_stickies_incomplete())


class AnnotateFpscrIncompletenessTests(unittest.TestCase):
    def _has_assumption(self, result: ProofResult) -> bool:
        return FPSCR_SYMBOLIC_STICKIES_ASSUMPTION in (result.assumptions or [])

    def test_demotes_when_fpscr_compared_after_arithmetic(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            observables=["fpscr", "f1"],
            opcodes_used=["fadd"],
        )
        annotate_fpscr_sticky_incompleteness(result)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertTrue(self._has_assumption(result))
        self.assertIn(FPSCR_STICKIES_UNSUPPORTED, result.unsupported or [])
        self.assertIn("fpscr-symbolic-stickies-unmodeled", result.abstractions or [])
        self.assertTrue(
            any("OX/UX/XX" in str(w) for w in (result.warnings or [])),
        )

    def test_identical_implementations_keep_equivalent(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            observables=["fpscr", "f1"],
            opcodes_used=["fadd"],
        )
        annotate_fpscr_sticky_incompleteness(
            result, identical_implementations=True,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertTrue(self._has_assumption(result))
        self.assertNotIn(FPSCR_STICKIES_UNSUPPORTED, result.unsupported or [])

    def test_skips_bitwise_only_fp(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            observables=["fpscr", "f1"],
            opcodes_used=["fmr"],
        )
        annotate_fpscr_sticky_incompleteness(result)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertFalse(self._has_assumption(result))

    def test_skips_when_fpscr_not_observable(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            observables=["r3"],
            opcodes_used=["fadd"],
        )
        annotate_fpscr_sticky_incompleteness(result)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertFalse(self._has_assumption(result))

    def test_skips_when_not_equivalent(self) -> None:
        result = ProofResult(
            status=ProofStatus.NOT_EQUIVALENT,
            observables=["fpscr"],
            opcodes_used=["fadd"],
        )
        annotate_fpscr_sticky_incompleteness(result)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertFalse(self._has_assumption(result))

    def test_idempotent(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            observables=["fpscr"],
            opcodes_used=["fadd"],
        )
        annotate_fpscr_sticky_incompleteness(result)
        annotate_fpscr_sticky_incompleteness(result)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        count = (result.assumptions or []).count(FPSCR_SYMBOLIC_STICKIES_ASSUMPTION)
        self.assertEqual(count, 1)
        warn_count = sum(
            1 for w in (result.warnings or []) if "OX/UX/XX" in str(w)
        )
        self.assertEqual(warn_count, 1)


if __name__ == "__main__":  # pragma: no cover
    unittest.main()

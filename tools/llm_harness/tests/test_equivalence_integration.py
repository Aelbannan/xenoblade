"""Tests for equivalence integration (§20.6)."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path
from typing import Any, Dict

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.counterexample import (
    Counterexample,
    ProofResult,
    classify_proof_status,
)
from tools.llm_harness.ranking import rank_candidate, _semantic_proof_tier, ProofSimilarity
from tools.llm_harness.types import (
    CandidateEvaluation,
    CandidateStatus,
    CompileReport,
    StructuralComponent,
    StructuralReport,
)


def _make_eval(**kw: Any) -> CandidateEvaluation:
    defaults: Dict[str, Any] = dict(
        status=CandidateStatus.COMPILES,
        compile_report=CompileReport(succeeded=True),
        match_percent=50.0,
        structural_report=StructuralReport(
            total_score=0.5,
            calls=StructuralComponent(score=0.5, matched=1, expected=2),
            relocations=StructuralComponent(score=0.5, matched=1, expected=2),
            memory_accesses=StructuralComponent(score=0.5, matched=2, expected=4),
            cfg=StructuralComponent(score=0.5, matched=2, expected=3),
            constants=StructuralComponent(score=0.5, matched=1, expected=2),
            returns=StructuralComponent(score=0.5, matched=1, expected=1),
            instruction_classes=StructuralComponent(score=0.5, matched=4, expected=8),
        ),
        function_size=100,
        retail_size=100,
        object_regressions=[],
        warnings=[],
    )
    defaults.update(kw)
    return CandidateEvaluation(**defaults)


class TestEquivalenceIntegration(unittest.TestCase):
    """§20.6 — Equivalence integration tests"""

    def test_proven_equivalence_sets_semantic_proof_status(self):
        """1. Proven equivalence sets semantic proof status."""
        eval = _make_eval(
            status=CandidateStatus.EQUIVALENT_MATCH,
            equivalence_status="proved",
        )
        self.assertEqual(eval.equivalence_status, "proved")

    def test_counterexample_sets_disproved(self):
        """2. Counterexample sets disproved status."""
        ce = Counterexample(
            input_registers={"r3": "0x0"},
            differing_observable="return value",
            retail_value="0x1",
            candidate_value="0x0",
        )
        pr = ProofResult(status="disproved", counterexample=ce)
        self.assertEqual(pr.status, "disproved")
        self.assertIsNotNone(pr.counterexample)

    def test_timeout_sets_inconclusive(self):
        """3. Timeout sets inconclusive status."""
        status = classify_proof_status("timeout")
        self.assertIn("inconclusive", status)

    def test_unsupported_instruction_set_unsupported(self):
        """4. Unsupported instruction sets unsupported status."""
        status = classify_proof_status("unsupported")
        self.assertEqual(status, "unsupported")

    def test_missing_callee_certificate_inconclusive(self):
        """5. Missing callee certificate remains inconclusive."""
        status = classify_proof_status("missing_callee")
        self.assertIn("inconclusive", status)

    def test_inconclusive_proof_cannot_promote_as_equivalent(self):
        """6. Inconclusive proof cannot promote as equivalent."""
        eval = _make_eval(
            status=CandidateStatus.EQUIVALENT_MATCH,
            equivalence_status="inconclusive_timeout",
        )
        from tools.llm_harness.promotion import passes_promotion_gate
        from tools.llm_harness.types import PromotionPolicy
        policy = PromotionPolicy()
        baseline = _make_eval()
        ok, _ = passes_promotion_gate(baseline, eval, policy, False)
        self.assertFalse(ok)

    def test_counterexample_summary_persisted(self):
        """7. Counterexample summary is persisted."""
        ce = Counterexample(
            input_registers={"r3": "0xABCD"},
            differing_observable="store offset",
            retail_value="0x10",
            candidate_value="0x14",
        )
        from tools.llm_harness.counterexample import summarize_counterexample
        summary = summarize_counterexample(ce)
        self.assertIn("store offset", summary)
        self.assertIn("0x10", summary)
        self.assertIn("0x14", summary)

    def test_proof_outranks_fuzzy_match(self):
        """8. Proof outranks fuzzy match in ranking."""
        proved = _make_eval(
            status=CandidateStatus.EQUIVALENT_MATCH,
            match_percent=80.0,
            equivalence_status="proved",
        )
        fuzzy = _make_eval(match_percent=95.0)
        self.assertGreater(rank_candidate(proved), rank_candidate(fuzzy))


if __name__ == "__main__":
    raise SystemExit(unittest.main())

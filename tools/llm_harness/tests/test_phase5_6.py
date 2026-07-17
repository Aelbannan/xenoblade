from __future__ import annotations

import sys
import json
from pathlib import Path
from collections import Counter

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

import unittest

from tools.llm_harness.match_improve import (
    InstructionDiff,
    MatchImproveResponse,
    normalize_objdiff_feedback,
    format_objdiff_feedback_text,
    passes_non_regression,
    NON_REGRESSION_TOLERANCE,
    ImproveAttempt,
    ImproveBranch,
)
from tools.llm_harness.counterexample import (
    Counterexample,
    ProofResult,
    ProofRecord,
    summarize_counterexample,
    classify_proof_status,
    PROOF_STATUS_LABELS,
)
from tools.llm_harness.ranking import (
    ProofSimilarity,
    extract_proof_similarity,
    rank_candidate,
    _semantic_proof_tier,
)
from tools.llm_harness.types import (
    CandidateEvaluation,
    CandidateStatus,
    CompileReport,
    StructuralReport,
    StructuralComponent,
)
from tools.ppc_equivalence.elf_symbols import FunctionBytes
from tools.ppc_equivalence.decoder import decode_block

SIMPLE_FN_CODE = bytes.fromhex(
    '9421fff0'
    '7c0802a6'
    '93e1000c'
    '90010014'
    '48000001'
    '80010014'
    '83e1000c'
    '7c0803a6'
    '38210010'
    '4e800020'
)

SIMPLE_FN = FunctionBytes(
    name='test', path=Path('.'), code=SIMPLE_FN_CODE,
    base=0x80000000, value=0x80000000, size=40,
    section_index=0, section_name='.text', symbol_type=0,
)


class TestMatchImproveFeedback(unittest.TestCase):
    """§13.3 — normalize objdiff feedback"""

    def test_self_comparison_no_diffs(self):
        feedback = normalize_objdiff_feedback(SIMPLE_FN, SIMPLE_FN)
        self.assertEqual(feedback["num_differences"], 0)
        self.assertIsNone(feedback["first_difference_offset"])

    def test_feedback_structure(self):
        feedback = normalize_objdiff_feedback(SIMPLE_FN, SIMPLE_FN)
        self.assertIn("instruction_windows", feedback)
        self.assertIn("differences", feedback)
        self.assertIn("relocation_differences", feedback)
        self.assertIn("stack_frame", feedback)
        self.assertIn("structural_summary", feedback)

    def test_stack_frame_detection(self):
        feedback = normalize_objdiff_feedback(SIMPLE_FN, SIMPLE_FN)
        sf = feedback["stack_frame"]
        self.assertEqual(sf["retail"], 16)
        self.assertEqual(sf["candidate"], 16)

    def test_format_text(self):
        feedback = normalize_objdiff_feedback(SIMPLE_FN, SIMPLE_FN)
        text = format_objdiff_feedback_text(feedback)
        self.assertIn("Objdiff feedback", text)

    def test_different_functions_have_diffs(self):
        other_code = bytes.fromhex(
            '9421ffe0'
            '7c0802a6'
            '90010024'
            '48000001'
            '80010024'
            '7c0803a6'
            '38210020'
            '4e800020'
        )
        other = FunctionBytes(
            name='other', path=Path('.'), code=other_code,
            base=0x80000000, value=0x80000000, size=32,
            section_index=0, section_name='.text', symbol_type=0,
        )
        feedback = normalize_objdiff_feedback(SIMPLE_FN, other)
        self.assertGreater(feedback["num_differences"], 0)
        self.assertIsNotNone(feedback["first_difference_offset"])


class TestNonRegression(unittest.TestCase):
    """§13.5 — non-regression guard"""

    def test_improvement_passes(self):
        self.assertTrue(passes_non_regression(0.80, 0.85))

    def test_slight_drop_tolerated(self):
        self.assertTrue(passes_non_regression(0.80, 0.79))  # 0.79 + 0.02 >= 0.80

    def test_excessive_drop_fails(self):
        self.assertFalse(passes_non_regression(0.80, 0.75))

    def test_edge_case(self):
        self.assertTrue(passes_non_regression(0.80, 0.78))
        self.assertFalse(passes_non_regression(0.80, 0.77))


class TestMatchImproveResponse(unittest.TestCase):
    """§13.4 — improvement response schema"""

    def test_parse_minimal(self):
        resp = MatchImproveResponse.from_json(
            '{"source":"void f(){}","stage":"match_improve","bounded_change":"test","evidence":[],"expected_instruction_effect":[]}'
        )
        self.assertIsNotNone(resp)
        self.assertEqual(resp.source, "void f(){}")
        self.assertEqual(resp.bounded_change, "test")

    def test_parse_fenced(self):
        resp = MatchImproveResponse.from_json(
            '```json\n{"source":"int x;","bounded_change":"x","evidence":["a"],"expected_instruction_effect":["b"]}\n```'
        )
        self.assertIsNotNone(resp)
        self.assertEqual(resp.evidence, ["a"])

    def test_parse_empty_source_rejected(self):
        resp = MatchImproveResponse.from_json('{"source":"  ","bounded_change":"x"}')
        self.assertIsNone(resp)

    def test_parse_invalid_json(self):
        resp = MatchImproveResponse.from_json("{invalid")
        self.assertIsNone(resp)


class TestImproveBranch(unittest.TestCase):
    """Improvement loop types"""

    def test_branch_creation(self):
        branch = ImproveBranch(target_id="t", initial_source="src", initial_score=0.5)
        self.assertEqual(branch.target_id, "t")
        self.assertFalse(branch.blocked)

    def test_branch_block(self):
        branch = ImproveBranch(target_id="t", initial_source="src", initial_score=0.5)
        branch.block("test reason")
        self.assertTrue(branch.blocked)
        self.assertEqual(branch.blocked_reason, "test reason")


class TestCounterexample(unittest.TestCase):
    """§14.4 — counterexample storage"""

    def test_counterexample_creation(self):
        ce = Counterexample(
            input_registers={"r3": "0x1234", "r4": "0x0000"},
            differing_observable="store at this+0x21",
            retail_value="0x01",
            candidate_value="0x00",
        )
        self.assertEqual(ce.differing_observable, "store at this+0x21")
        self.assertEqual(ce.retail_value, "0x01")

    def test_summarize(self):
        ce = Counterexample(
            input_registers={"r3": "0xABCD"},
            differing_observable="return value",
            retail_value="0x01",
            candidate_value="0x00",
        )
        summary = summarize_counterexample(ce)
        self.assertIn("return value", summary)
        self.assertIn("0x01", summary)

    def test_proof_result(self):
        pr = ProofResult(status="disproved", counterexample=Counterexample(
            differing_observable="branch outcome",
        ))
        self.assertEqual(pr.status, "disproved")
        self.assertIsNotNone(pr.counterexample)

    def test_proof_record(self):
        rec = ProofRecord(
            target_id="test",
            candidate_source_hash="abc123",
            proof_status="inconclusive_timeout",
            structural_score=0.85,
        )
        self.assertEqual(rec.target_id, "test")
        self.assertEqual(rec.proof_status, "inconclusive_timeout")

    def test_classify_proof_status(self):
        self.assertEqual(classify_proof_status("proved"), "proved")
        self.assertEqual(classify_proof_status("disproved"), "disproved")
        self.assertEqual(classify_proof_status("unsupported"), "unsupported")
        self.assertIn("inconclusive", classify_proof_status("timeout"))
        self.assertIn("inconclusive", classify_proof_status("unknown external effect"))

    def test_proof_status_labels(self):
        self.assertIn("proved", PROOF_STATUS_LABELS)
        self.assertIn("disproved", PROOF_STATUS_LABELS)


class TestProofAwareRanking(unittest.TestCase):
    """§14.5 — proof-aware ranking"""

    def _make_evaluation(self, **overrides) -> CandidateEvaluation:
        defaults = dict(
            status=CandidateStatus.COMPILES,
            compile_report=CompileReport(succeeded=True),
            match_percent=50.0,
            structural_report=StructuralReport(
                total_score=0.5,
                calls=StructuralComponent(score=0.5, matched=1, expected=2),
                relocations=StructuralComponent(score=0.5, matched=1, expected=2),
                memory_accesses=StructuralComponent(score=0.5, matched=1, expected=2),
                cfg=StructuralComponent(score=0.5, matched=1, expected=2),
                constants=StructuralComponent(score=0.5, matched=1, expected=2),
                returns=StructuralComponent(score=0.5, matched=1, expected=2),
                instruction_classes=StructuralComponent(score=0.5, matched=1, expected=2),
            ),
            function_size=100,
            retail_size=100,
            object_regressions=[],
            warnings=[],
        )
        defaults.update(overrides)
        return CandidateEvaluation(**defaults)

    def test_full_match_outranks_proved(self):
        full = self._make_evaluation(
            status=CandidateStatus.FULL_MATCH, match_percent=100.0,
        )
        proved = self._make_evaluation(
            status=CandidateStatus.EQUIVALENT_MATCH, match_percent=80.0,
            equivalence_status="proved",
        )
        self.assertGreater(
            rank_candidate(full),
            rank_candidate(proved),
        )

    def test_proved_outranks_unproven(self):
        proved = self._make_evaluation(
            status=CandidateStatus.EQUIVALENT_MATCH, match_percent=80.0,
            equivalence_status="proved",
        )
        unproven = self._make_evaluation(match_percent=95.0)
        self.assertGreater(
            rank_candidate(proved),
            rank_candidate(unproven),
        )

    def test_higher_match_within_tier(self):
        high = self._make_evaluation(match_percent=90.0)
        low = self._make_evaluation(match_percent=50.0)
        self.assertGreater(
            rank_candidate(high),
            rank_candidate(low),
        )

    def test_size_delta_tiebreak(self):
        smaller = self._make_evaluation(
            function_size=100, retail_size=100,
        )
        larger = self._make_evaluation(
            function_size=120, retail_size=100,
        )
        self.assertGreater(
            rank_candidate(smaller),
            rank_candidate(larger),
        )

    def test_proof_similarity_extraction(self):
        eval = self._make_evaluation(
            status=CandidateStatus.EQUIVALENT_MATCH,
            equivalence_status="proved",
            structural_report=StructuralReport(
                total_score=0.85,
                calls=StructuralComponent(score=0.8, matched=1, expected=1),
                relocations=StructuralComponent(score=1.0, matched=1, expected=1),
                memory_accesses=StructuralComponent(score=0.8, matched=2, expected=2),
                cfg=StructuralComponent(score=0.8, matched=3, expected=3),
                constants=StructuralComponent(score=0.9, matched=2, expected=2),
                returns=StructuralComponent(score=1.0, matched=1, expected=1),
                instruction_classes=StructuralComponent(score=0.9, matched=4, expected=4),
            ),
        )
        ps = extract_proof_similarity(eval)
        self.assertEqual(ps.semantic_proof, "proved")
        self.assertAlmostEqual(ps.structural_score, 0.85)
        self.assertAlmostEqual(ps.match_percent, 50.0)

    def test_size_acceptability(self):
        ok = self._make_evaluation(function_size=100, retail_size=100)
        too_big = self._make_evaluation(function_size=120, retail_size=100)
        ok_rank = rank_candidate(ok)
        big_rank = rank_candidate(too_big)
        # lower _size_is_acceptable → worse rank for too_big
        self.assertNotEqual(ok_rank, big_rank)

    def test_regression_penalty(self):
        clean = self._make_evaluation()
        regressed = self._make_evaluation(object_regressions=["funcA regressed"])
        self.assertGreater(
            rank_candidate(clean),
            rank_candidate(regressed),
        )

    def test_semantic_proof_tier_ordering(self):
        self.assertEqual(_semantic_proof_tier(ProofSimilarity(full_match=True)), 6)
        self.assertEqual(_semantic_proof_tier(ProofSimilarity(semantic_proof="proved")), 5)
        self.assertEqual(
            _semantic_proof_tier(ProofSimilarity(structural_score=0.90)),
            4,
        )
        self.assertEqual(
            _semantic_proof_tier(ProofSimilarity(structural_score=0.75)),
            3,
        )
        self.assertEqual(
            _semantic_proof_tier(ProofSimilarity(structural_score=0.50)),
            2,
        )
        self.assertEqual(
            _semantic_proof_tier(ProofSimilarity(structural_score=0.10)),
            1,
        )
        self.assertEqual(
            _semantic_proof_tier(ProofSimilarity(semantic_proof="disproved")),
            1,
        )
        self.assertEqual(
            _semantic_proof_tier(ProofSimilarity()),
            0,
        )


if __name__ == "__main__":
    raise SystemExit(unittest.main())

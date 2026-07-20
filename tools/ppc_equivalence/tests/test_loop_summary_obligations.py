"""Loop-summary proof obligation build/validate tests."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.loop_summary import (
    build_loop_summary_obligation,
    find_ctr_affine_loop_candidates,
    summarize_ctr_affine_loop,
    validate_loop_summary_obligation,
)
from tools.ppc_equivalence.proof_features import validate_proof_features
from tools.ppc_equivalence.tests.test_loop_summary import _ctr_counted_loop


class LoopSummaryObligationTests(unittest.TestCase):
    def test_build_validate_round_trip(self) -> None:
        program = _ctr_counted_loop(count=4, addend=3)
        candidate = find_ctr_affine_loop_candidates(program)[0]
        summary = summarize_ctr_affine_loop(candidate)
        self.assertIsNotNone(summary)
        assert summary is not None

        obligation = build_loop_summary_obligation(summary, coverage="pending")
        self.assertIsNone(validate_loop_summary_obligation(obligation))
        self.assertEqual(obligation["proof_kind"], "affine-closed-form")
        self.assertEqual(obligation["trip_count"], 4)
        self.assertEqual(
            obligation["final_gpr"],
            [{"reg": 3, "entry_reg": 3, "stride": 3}],
        )

    def test_dict_keyed_final_gpr_validates(self) -> None:
        from tools.ppc_equivalence.loop_summary import compute_loop_summary_sha256

        obligation = {
            "proof_kind": "affine-closed-form",
            "header_pc": 8,
            "latch_pc": 12,
            "exit_pc": 16,
            "trip_count": 2,
            "final_ctr": 0,
            "ranking": "ctr-descending",
            "final_gpr": {
                "3": {"entry_reg": 3, "stride": 1},
            },
            "status": "applied",
            "algorithm": "affine-closed-form-v1",
            "coverage": "applied",
        }
        obligation["summary_sha256"] = compute_loop_summary_sha256(obligation)
        self.assertIsNone(validate_loop_summary_obligation(obligation))

    def test_missing_required_key_fails(self) -> None:
        self.assertIsNotNone(
            validate_loop_summary_obligation({"proof_kind": "affine-closed-form"}),
        )
        self.assertIn("missing", validate_loop_summary_obligation({"proof_kind": "x"}) or "")

    def test_validate_proof_features_rejects_malformed_loop_summary(self) -> None:
        reason = validate_proof_features(
            {
                "proof_features": ["affine-loop-summary"],
                "loop_summary": {"proof_kind": "affine-closed-form"},
            },
        )
        self.assertIsNotNone(reason)
        self.assertIn("loop_summary missing", reason or "")

    def test_validate_proof_features_accepts_well_formed_loop_summary(self) -> None:
        program = _ctr_counted_loop(count=2, addend=1)
        summary = summarize_ctr_affine_loop(find_ctr_affine_loop_candidates(program)[0])
        self.assertIsNotNone(summary)
        assert summary is not None
        obligation = build_loop_summary_obligation(summary)
        reason = validate_proof_features(
            {
                "proof_features": ["affine-loop-summary"],
                "loop_summary": obligation,
            },
        )
        self.assertIsNone(reason)

    def test_discharged_requires_relational_companion(self) -> None:
        program = _ctr_counted_loop(count=2, addend=1)
        summary = summarize_ctr_affine_loop(find_ctr_affine_loop_candidates(program)[0])
        assert summary is not None
        bad = build_loop_summary_obligation(
            summary, coverage="applied", status="discharged",
        )
        self.assertIsNotNone(validate_loop_summary_obligation(bad))
        good = build_loop_summary_obligation(
            summary,
            coverage="applied",
            status="discharged",
            relational_companion="discharged",
        )
        self.assertIsNone(validate_loop_summary_obligation(good))
        self.assertEqual(len(good["summary_sha256"]), 64)


if __name__ == "__main__":
    unittest.main()

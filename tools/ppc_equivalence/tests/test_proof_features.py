from __future__ import annotations

import unittest

from tools.ppc_equivalence.proof_features import (
    KNOWN_PROOF_FEATURES,
    UNSUPPORTED_FOR_EQUIVALENT,
    enforce_equivalent_proof_features,
    validate_proof_features,
)
from tools.ppc_equivalence.result import ProofResult, ProofStatus


class ProofFeaturesValidationTests(unittest.TestCase):
    def test_omit_features_is_ok(self) -> None:
        self.assertIsNone(validate_proof_features({}))
        self.assertIsNone(validate_proof_features(ProofResult(status=ProofStatus.EQUIVALENT)))

    def test_to_dict_omits_empty_proof_features(self) -> None:
        payload = ProofResult(status=ProofStatus.EQUIVALENT).to_dict()
        self.assertNotIn("proof_features", payload)
        self.assertNotIn("address_space", payload)
        self.assertNotIn("indirect_targets", payload)

    def test_declared_feature_without_obligation_fails(self) -> None:
        reason = validate_proof_features({"proof_features": ["readonly-image"]})
        self.assertIsNotNone(reason)
        self.assertIn("address_space", reason)

    def test_unknown_feature_fails(self) -> None:
        reason = validate_proof_features({"proof_features": ["jump-table"]})
        self.assertIsNotNone(reason)
        self.assertIn("unknown", reason)

    def test_orphan_obligation_without_features_fails(self) -> None:
        reason = validate_proof_features({"address_space": {}})
        self.assertIsNotNone(reason)
        self.assertIn("without proof_features", reason)

    def test_malformed_proof_features_type_fails(self) -> None:
        reason = validate_proof_features({"proof_features": "readonly-image"})
        self.assertIsNotNone(reason)
        self.assertIn("list", reason)

    def test_malformed_obligation_type_fails(self) -> None:
        reason = validate_proof_features({
            "proof_features": ["readonly-image"],
            "address_space": [],
        })
        self.assertIsNotNone(reason)
        self.assertIn("object", reason)

    def test_supported_obligations_validate_structurally(self) -> None:
        payload = {
            "proof_features": ["readonly-image", "indirect-target-closure"],
            "address_space": {},
            "indirect_targets": {},
        }
        self.assertIsNone(validate_proof_features(payload))

    def test_supported_features_can_stay_equivalent(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["readonly-image", "indirect-target-closure"],
            address_space={},
            indirect_targets={},
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.EQUIVALENT)
        self.assertEqual(UNSUPPORTED_FOR_EQUIVALENT, frozenset())

    def test_affine_loop_summary_with_obligation_stays_equivalent(self) -> None:
        from tools.ppc_equivalence.loop_summary import (
            build_loop_summary_obligation,
            summarize_ctr_affine_loop,
            find_ctr_affine_loop_candidates,
        )
        from tools.ppc_equivalence.ir import Instruction, Opcode

        program = [
            Instruction(0, 0, Opcode.ADDI, (0, 0, 4)),
            Instruction(4, 0, Opcode.MTSPR, (0, 9)),
            Instruction(8, 0, Opcode.ADDI, (3, 3, 1)),
            Instruction(12, 0, Opcode.BC, (16, 0, 8, 0)),
        ]
        summary = summarize_ctr_affine_loop(find_ctr_affine_loop_candidates(program)[0])
        assert summary is not None
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["affine-loop-summary"],
            loop_summary=build_loop_summary_obligation(summary, coverage="applied"),
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.EQUIVALENT)

    def test_non_equivalent_status_is_not_demoted(self) -> None:
        result = ProofResult(
            status=ProofStatus.NOT_EQUIVALENT,
            proof_features=["readonly-image"],
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.NOT_EQUIVALENT)

    def test_to_dict_round_trip_includes_declared_features(self) -> None:
        result = ProofResult(
            status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            proof_features=["readonly-image"],
            address_space={"regions": []},
        )
        payload = result.to_dict()
        self.assertEqual(payload["proof_features"], ["readonly-image"])
        self.assertEqual(payload["address_space"], {"regions": []})

    def test_registry_lists_known_features(self) -> None:
        self.assertIn("readonly-image", KNOWN_PROOF_FEATURES)
        self.assertIn("indirect-target-closure", KNOWN_PROOF_FEATURES)
        self.assertIn("affine-loop-summary", KNOWN_PROOF_FEATURES)


if __name__ == "__main__":
    unittest.main()

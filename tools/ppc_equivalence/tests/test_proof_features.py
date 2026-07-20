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
        from tools.ppc_equivalence.jump_table_obligations import (
            JumpTableWords,
            build_indirect_targets_obligation,
            build_readonly_image_obligation,
        )

        table = JumpTableWords(
            base=0x80010000,
            words=(0x80020000,),
            source="test",
        )
        address_space = build_readonly_image_obligation(table, no_write_status="pending")
        indirect_targets = build_indirect_targets_obligation(
            branch_pc=0x80001000,
            targets=(("case-0", 0x80020000),),
            source="test",
            artifact_hashes=(table.image_sha256,),
            coverage="pending",
        )
        payload = {
            "proof_features": ["readonly-image", "indirect-target-closure"],
            "address_space": address_space,
            "indirect_targets": indirect_targets,
        }
        self.assertIsNone(validate_proof_features(payload))

    def test_unsupported_for_equivalent_excludes_relational_after_pr7(self) -> None:
        self.assertEqual(
            UNSUPPORTED_FOR_EQUIVALENT,
            frozenset({
                "memory-bus",
            }),
        )
        self.assertNotIn("relational-induction", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("affine-loop-summary", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("memory-loop-summary", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("readonly-image", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("indirect-target-closure", UNSUPPORTED_FOR_EQUIVALENT)

    def test_supported_features_can_stay_equivalent(self) -> None:
        # PR3: jump-table features may authorize EQUIVALENT when obligations
        # validate (coverage/no-write digests optional as string status).
        from tools.ppc_equivalence.jump_table_obligations import (
            JumpTableWords,
            build_indirect_targets_obligation,
            build_readonly_image_obligation,
        )

        table = JumpTableWords(
            base=0x80010000,
            words=(0x80020000,),
            source="test",
        )
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["readonly-image", "indirect-target-closure"],
            address_space=build_readonly_image_obligation(table, no_write_status="unsat"),
            indirect_targets=build_indirect_targets_obligation(
                branch_pc=0x80001000,
                targets=(("case-0", 0x80020000),),
                source="test",
                artifact_hashes=(table.image_sha256,),
                coverage="unsat",
            ),
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.EQUIVALENT)
        self.assertEqual(
            list(gated.proof_features),
            ["readonly-image", "indirect-target-closure"],
        )

    def test_affine_loop_summary_requires_discharged_relational_companion(self) -> None:
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
        # Applied-only (no relational companion) must demote.
        applied = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["affine-loop-summary"],
            loop_summary=build_loop_summary_obligation(
                summary, coverage="applied", status="applied",
            ),
        )
        gated = enforce_equivalent_proof_features(applied)
        self.assertEqual(gated.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertIn("affine-loop-summary", gated.proof_features)

        # Forged discharged without relational companion must fail validation.
        forged = build_loop_summary_obligation(
            summary,
            coverage="applied",
            status="discharged",
            relational_companion="discharged",
        )
        reason = validate_proof_features(
            {
                "proof_features": ["affine-loop-summary"],
                "loop_summary": forged,
            },
            require_equivalent_ready=True,
        )
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("relational-induction", reason)

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
        self.assertIn("relational-induction", KNOWN_PROOF_FEATURES)
        self.assertIn("memory-loop-summary", KNOWN_PROOF_FEATURES)
        self.assertIn("memory-bus", KNOWN_PROOF_FEATURES)

    def test_pr0_freeze_blocks_remaining_expanded_features(self) -> None:
        """Wave 5: affine/memory-loop unfrozen; memory-bus remains blocked."""
        self.assertTrue(UNSUPPORTED_FOR_EQUIVALENT)
        self.assertTrue(UNSUPPORTED_FOR_EQUIVALENT.issubset(KNOWN_PROOF_FEATURES))
        self.assertEqual(
            UNSUPPORTED_FOR_EQUIVALENT,
            frozenset({
                "memory-bus",
            }),
        )


if __name__ == "__main__":
    unittest.main()

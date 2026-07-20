from __future__ import annotations

import unittest

from tools.ppc_equivalence.proof_features import (
    KNOWN_PROOF_FEATURES,
    UNSUPPORTED_FOR_EQUIVALENT,
    enforce_equivalent_proof_features,
    validate_proof_features,
)
from tools.ppc_equivalence.result import ProofResult, ProofStatus


class ProofFeaturesTests(unittest.TestCase):
    def test_empty_payload_is_ok(self) -> None:
        self.assertIsNone(validate_proof_features({}))
        self.assertIsNone(validate_proof_features(ProofResult(status=ProofStatus.EQUIVALENT)))

    def test_to_dict_omits_empty_proof_features(self) -> None:
        payload = ProofResult(status=ProofStatus.EQUIVALENT).to_dict()
        self.assertNotIn("proof_features", payload)
        self.assertNotIn("address_space", payload)
        self.assertNotIn("indirect_targets", payload)

    def test_unknown_feature_rejected(self) -> None:
        reason = validate_proof_features({"proof_features": ["not-a-real-feature"]})
        self.assertIsNotNone(reason)
        self.assertIn("unknown", reason or "")

    def test_obligation_without_feature_rejected(self) -> None:
        reason = validate_proof_features({"address_space": {"regions": []}})
        self.assertIsNotNone(reason)
        self.assertIn("without proof_features", reason or "")

    def test_feature_requires_matching_obligation(self) -> None:
        reason = validate_proof_features({"proof_features": ["readonly-image"]})
        self.assertIsNotNone(reason)
        self.assertIn("address_space", reason or "")

    def test_malformed_proof_features_type_fails(self) -> None:
        reason = validate_proof_features({"proof_features": "readonly-image"})
        self.assertIsNotNone(reason)
        self.assertIn("list", reason or "")

    def test_malformed_obligation_type_fails(self) -> None:
        reason = validate_proof_features({
            "proof_features": ["readonly-image"],
            "address_space": [],
        })
        self.assertIsNotNone(reason)
        self.assertIn("object", reason or "")

    def test_jump_table_pair_validates(self) -> None:
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
        address_space = build_readonly_image_obligation(table, no_write_status="unsat")
        indirect_targets = build_indirect_targets_obligation(
            branch_pc=0x80001000,
            targets=(("case-0", 0x80020000),),
            source="test",
            artifact_hashes=(table.image_sha256,),
            coverage="unsat",
        )
        payload = {
            "proof_features": ["readonly-image", "indirect-target-closure"],
            "address_space": address_space,
            "indirect_targets": indirect_targets,
        }
        self.assertIsNone(validate_proof_features(payload))

    def test_unsupported_for_equivalent_keeps_other_features_clear(self) -> None:
        # memory-bus unfrozen; freeze set must stay empty.
        self.assertEqual(UNSUPPORTED_FOR_EQUIVALENT, frozenset())
        self.assertNotIn("relational-induction", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("affine-loop-summary", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("memory-loop-summary", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("readonly-image", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("indirect-target-closure", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertNotIn("memory-bus", UNSUPPORTED_FOR_EQUIVALENT)

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

    def test_pr0_freeze_cleared_for_memory_bus(self) -> None:
        """Track D: memory-bus unfrozen; no remaining reserved features."""
        self.assertEqual(UNSUPPORTED_FOR_EQUIVALENT, frozenset())
        self.assertTrue(UNSUPPORTED_FOR_EQUIVALENT.issubset(KNOWN_PROOF_FEATURES))

    def test_memory_bus_require_equivalent_ready_needs_discharged(self) -> None:
        """Track A/D: require_equivalent_ready demands status=discharged."""
        from tools.ppc_equivalence.address_space import AddressSpace, mmio_region
        from tools.ppc_equivalence.device_model import RegisterBankDevice, RegisterSpec
        from tools.ppc_equivalence.memory_bus import build_memory_bus
        from tools.ppc_equivalence.memory_bus_obligations import (
            build_access_coverage_attestation,
            build_device_state_in_compare_attestation,
            build_memory_bus_obligation,
            validate_memory_bus_obligation,
        )
        from tools.ppc_equivalence.provenance import canonical_json_sha256

        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})
        pending = build_memory_bus_obligation(bus)
        reason = validate_proof_features(
            {"proof_features": ["memory-bus"], "memory_bus": pending},
            require_equivalent_ready=True,
        )
        self.assertIsNotNone(reason)
        self.assertIn("discharged", reason or "")

        coverage = build_access_coverage_attestation(
            attested=True,
            status="complete",
            opcode_families=["integer-load-store"],
        )
        cfg_sha = canonical_json_sha256({"kind": "test-cfg"})
        vacuous = {
            "status": "vacuously-discharged",
            "reason": "no-unsupported-predicates",
            "cfg_trace_sha256": cfg_sha,
            "access_coverage_sha256": coverage["original"]["sha256"],
        }
        observability = {
            "register_banks": {"test-bank": {"0x0": "0xab"}},
            "fifo_traces": {},
            "touches": {"original": [], "candidate": []},
            "symbolic": {
                "original": {"register_banks": {}, "fifo_traces": {}},
                "candidate": {"register_banks": {}, "fifo_traces": {}},
            },
        }
        discharged = dict(pending)
        discharged.update(
            {
                "status": "discharged",
                "access_coverage": coverage,
                "unsupported_access": {
                    "original": vacuous,
                    "candidate": {
                        **vacuous,
                        "access_coverage_sha256": coverage["candidate"]["sha256"],
                    },
                },
                "register_bank_theory": {
                    "status": "present",
                    "devices": [{"device_id": "test-bank", "theory": "register-bank"}],
                },
                "fifo_theory": {"status": "none", "devices": []},
                "observability": observability,
                "device_state_in_compare": build_device_state_in_compare_attestation(
                    included=True,
                    observability=observability,
                ),
                "cfg_rejection_reasons": [],
                "loop_fifo_reject_markers": [],
            }
        )
        self.assertIsNone(validate_memory_bus_obligation(discharged))
        # Structurally valid discharged authorizes (engine path builds this).
        ready = validate_proof_features(
            {"proof_features": ["memory-bus"], "memory_bus": discharged},
            require_equivalent_ready=True,
        )
        self.assertIsNone(ready)
        self.assertNotIn("memory-bus", UNSUPPORTED_FOR_EQUIVALENT)

        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["memory-bus"],
            memory_bus=discharged,
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.EQUIVALENT)

        # Weak forge without digests still demotes.
        weak = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["memory-bus"],
            memory_bus={
                "algorithm": "memory-bus-v1",
                "status": "discharged",
                "regions": [{"kind": "ram", "start": 0, "end": 1}],
            },
        )
        demoted = enforce_equivalent_proof_features(weak)
        self.assertEqual(demoted.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)


if __name__ == "__main__":
    unittest.main()

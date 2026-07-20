"""Capability-assurance schema and evaluation (Wave 1)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.capability_assurance import (
    CAPABILITY_ASSURANCE_POLICY,
    CAPABILITY_ASSURANCE_SCHEMA_VERSION,
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    STATUS_UNMODELED,
    CapabilityAssurance,
    CapabilityAttestation,
    CapabilityManifest,
    attestation_digest,
    build_attestation,
    compute_confidence_tier_from_assurance,
    evaluate_capability_assurance,
)
from tools.ppc_equivalence.memory_profile import MemoryEnvironment, MemoryProfile
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    MASKING_SEMANTICS,
    RESULT_FORMAT,
    FloatingPointDomain,
    MemoryScope,
    PrivateStackInfo,
    ProofResult,
    ProofStatus,
)


def _ledger(*opcodes: str):
    from tools.coop.lib.equivalence_policy import ValidationLedger

    return ValidationLedger(frozenset(opcodes), intentionally_loaded=True)


def _equivalent(**kwargs) -> ProofResult:
    defaults = dict(
        status=ProofStatus.EQUIVALENT,
        architecture_model=ARCHITECTURE_MODEL,
        format=RESULT_FORMAT,
        observables=["r3"],
        engine_hash="a" * 64,
        source_hash="b" * 64,
        git_commit="c" * 40,
        opcodes_used=["addi", "blr"],
        memory_scope=MemoryScope(
            masking_semantics=MASKING_SEMANTICS,
            original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
        ),
    )
    defaults.update(kwargs)
    return ProofResult(**defaults)


def _manifest(**kwargs) -> CapabilityManifest:
    defaults = dict(
        allowed_tier_a_capabilities={"integer-core": ("integer-core-v1",)},
        shadow_mode=True,
    )
    defaults.update(kwargs)
    return CapabilityManifest(**defaults)


class SchemaTests(unittest.TestCase):
    def test_round_trip(self) -> None:
        attestation = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            status=STATUS_INCOMPLETE,
            evidence={"opcodes": ["addi", "blr"], "ledger_sha256": "d" * 64},
        )
        assurance = CapabilityAssurance(
            schema_version=CAPABILITY_ASSURANCE_SCHEMA_VERSION,
            policy=CAPABILITY_ASSURANCE_POLICY,
            capabilities=(attestation,),
        )
        restored = CapabilityAssurance.from_dict(assurance.to_dict())
        restored.validate_structure()
        self.assertEqual(restored.to_dict(), assurance.to_dict())
        self.assertEqual(
            restored.capabilities[0].attestation_sha256,
            attestation_digest(attestation),
        )

    def test_forged_promotion_grade_ignored(self) -> None:
        forged = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            status=STATUS_PROMOTION_GRADE,
            evidence={"opcodes": ["addi"]},  # incomplete vs ledger
        )
        proof = _equivalent(
            capability_assurance=CapabilityAssurance(
                capabilities=(forged,),
            ).to_dict(),
            opcodes_used=["addi"],
        )
        result = evaluate_capability_assurance(
            proof,
            _ledger(),  # empty → missing opcodes
            _manifest(),
        )
        self.assertEqual(
            result.recomputed_statuses["integer-core"],
            STATUS_INCOMPLETE,
        )
        self.assertTrue(result.has_unmodeled_or_unproven_capability)
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_malformed_schema_fail_closed(self) -> None:
        with self.assertRaises(ValueError):
            CapabilityAssurance(
                schema_version=99,
                policy=CAPABILITY_ASSURANCE_POLICY,
                capabilities=(),
            ).validate_structure()

    def test_unknown_capability_fail_closed(self) -> None:
        bad = build_attestation(
            capability="not-a-real-capability",
            model_version="v1",
            algorithm="opcode-ledger-v2",
        )
        with self.assertRaises(ValueError):
            CapabilityAssurance(capabilities=(bad,)).validate_structure()

    def test_unknown_algorithm_fail_closed(self) -> None:
        bad = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="forged-algo-v9",
        )
        with self.assertRaises(ValueError):
            CapabilityAssurance(capabilities=(bad,)).validate_structure()

    def test_digest_mismatch_fail_closed(self) -> None:
        good = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            evidence={"opcodes": ["addi"]},
        )
        tampered = CapabilityAttestation(
            capability=good.capability,
            model_version=good.model_version,
            algorithm=good.algorithm,
            status=good.status,
            assumptions=good.assumptions,
            unsupported=good.unsupported,
            evidence=dict(good.evidence),
            attestation_sha256="0" * 64,
        )
        with self.assertRaises(ValueError):
            CapabilityAssurance(capabilities=(tampered,)).validate_structure()


class EvaluationTests(unittest.TestCase):
    def test_legacy_without_assurance_is_unproven(self) -> None:
        proof = _equivalent()
        result = evaluate_capability_assurance(proof, _ledger("addi", "blr"), _manifest())
        self.assertTrue(result.has_unmodeled_or_unproven_capability)
        self.assertEqual(result.attestations, ())
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_integer_core_allowlisted_complete_ledger_is_a(self) -> None:
        attestation = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            status=STATUS_INCOMPLETE,
            evidence={"opcodes": ["addi", "blr"]},
        )
        proof = _equivalent(
            capability_assurance=CapabilityAssurance(
                capabilities=(attestation,),
            ).to_dict(),
        )
        result = evaluate_capability_assurance(
            proof,
            _ledger("addi", "blr"),
            _manifest(),
        )
        self.assertEqual(
            result.recomputed_statuses["integer-core"],
            STATUS_PROMOTION_GRADE,
        )
        self.assertTrue(result.all_used_capabilities_promotion_grade)
        self.assertEqual(compute_confidence_tier_from_assurance(result), "A")

    def test_has_fp_without_fp_attestation_is_c(self) -> None:
        attestation = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            evidence={"opcodes": ["addi", "fadds"]},
        )
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["addi", "fadds"],
            capability_assurance=CapabilityAssurance(
                capabilities=(attestation,),
            ).to_dict(),
        )
        result = evaluate_capability_assurance(
            proof,
            _ledger("addi", "fadds"),
            _manifest(),
        )
        self.assertEqual(
            result.recomputed_statuses.get("fp-scalar-arithmetic"),
            STATUS_UNMODELED,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_has_memory_bus_is_c(self) -> None:
        attestation = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            evidence={"opcodes": ["lwz"]},
        )
        proof = _equivalent(
            proof_features=["memory-bus"],
            memory_bus={"schema_version": 1},
            capability_assurance=CapabilityAssurance(
                capabilities=(attestation,),
            ).to_dict(),
            opcodes_used=["lwz"],
        )
        result = evaluate_capability_assurance(
            proof,
            _ledger("lwz"),
            _manifest(),
        )
        self.assertEqual(
            result.recomputed_statuses.get("mmio-register-bank"),
            STATUS_UNMODELED,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_assumed_ram_is_c(self) -> None:
        attestation = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            evidence={"opcodes": ["lwz"]},
        )
        proof = _equivalent(
            observables=["memory"],
            environment=MemoryEnvironment(profile=MemoryProfile.ASSUMED_ORDINARY_RAM),
            capability_assurance=CapabilityAssurance(
                capabilities=(attestation,),
            ).to_dict(),
            opcodes_used=["lwz"],
        )
        result = evaluate_capability_assurance(
            proof,
            _ledger("lwz"),
            _manifest(),
        )
        self.assertIn("assumed-ordinary-ram", result.recomputed_statuses)
        self.assertNotEqual(
            result.recomputed_statuses["assumed-ordinary-ram"],
            STATUS_PROMOTION_GRADE,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")


if __name__ == "__main__":
    unittest.main()

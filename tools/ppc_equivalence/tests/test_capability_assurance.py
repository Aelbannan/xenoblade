"""Capability-assurance schema and evaluation (Wave 1–2)."""

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
    infer_used_capabilities,
)
from tools.ppc_equivalence.fp_bitwise import (
    FP_BITWISE_ALGORITHM,
    FP_BITWISE_MODEL_VERSION,
    FP_BITWISE_OPS,
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


def _ledger(*opcodes: str, capabilities: dict | None = None):
    from tools.coop.lib.equivalence_policy import ValidationLedger

    return ValidationLedger(
        frozenset(opcodes),
        intentionally_loaded=True,
        capabilities=dict(capabilities or {}),
    )


def _fp_bitwise_ledger_capabilities(*opcodes: str) -> dict:
    return {
        "fp-bitwise": {
            "model_version": FP_BITWISE_MODEL_VERSION,
            "opcodes": {
                op: {
                    "result_bits": True,
                    "dolphin_interpreter": True,
                    "host_float": False,
                }
                for op in opcodes
            },
        }
    }


def _equivalent(**kwargs) -> ProofResult:
    defaults = dict(
        status=ProofStatus.EQUIVALENT,
        architecture_model=ARCHITECTURE_MODEL,
        format=RESULT_FORMAT,
        observables=["r3"],
        engine_hash="a" * 64,
        certifier_hash="d" * 64,
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
        allowed_tier_a_capabilities={
            "integer-core": ("integer-core-v1",),
            "provenance": ("provenance-v1",),
            "fp-bitwise": (FP_BITWISE_MODEL_VERSION,),
        },
        shadow_mode=True,
    )
    defaults.update(kwargs)
    return CapabilityManifest(**defaults)


def _provenance_attestation(**overrides) -> CapabilityAttestation:
    evidence = {
        "engine_hash": "a" * 64,
        "certifier_hash": "d" * 64,
        "source_hash": "b" * 64,
        "git_commit": "c" * 40,
        "git_dirty": False,
    }
    evidence.update(overrides.pop("evidence", {}))
    return build_attestation(
        capability="provenance",
        model_version="provenance-v1",
        algorithm="provenance-binding-v1",
        status=STATUS_INCOMPLETE,
        evidence=evidence,
        **overrides,
    )


def _integer_attestation(*opcodes: str) -> CapabilityAttestation:
    return build_attestation(
        capability="integer-core",
        model_version="integer-core-v1",
        algorithm="opcode-ledger-v2",
        status=STATUS_INCOMPLETE,
        evidence={"opcodes": list(opcodes), "ledger_sha256": "e" * 64},
    )


def _fp_bitwise_attestation(
    *opcodes: str,
    host_float: bool | None = False,
    ledger_sha256: str | None = "e" * 64,
    unsupported: tuple[str, ...] = (),
    status: str = STATUS_INCOMPLETE,
) -> CapabilityAttestation:
    evidence: dict = {"opcodes": list(opcodes)}
    if host_float is not None:
        evidence["host_float"] = host_float
    if ledger_sha256 is not None:
        evidence["ledger_sha256"] = ledger_sha256
    return build_attestation(
        capability="fp-bitwise",
        model_version=FP_BITWISE_MODEL_VERSION,
        algorithm=FP_BITWISE_ALGORITHM,
        status=status,
        unsupported=unsupported,
        evidence=evidence,
    )


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

    def test_fp_bitwise_certificate_round_trip(self) -> None:
        assurance = CapabilityAssurance(
            capabilities=(
                _integer_attestation("addi", "blr"),
                _fp_bitwise_attestation("fmr", "fabs"),
                _provenance_attestation(),
            ),
        )
        restored = CapabilityAssurance.from_dict(assurance.to_dict())
        restored.validate_structure()
        self.assertEqual(restored.to_dict(), assurance.to_dict())
        names = {item.capability for item in restored.capabilities}
        self.assertEqual(names, {"integer-core", "fp-bitwise", "provenance"})

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
        proof = _equivalent(
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("addi", "blr"),
                    _provenance_attestation(),
                ),
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
        self.assertEqual(
            result.recomputed_statuses["provenance"],
            STATUS_PROMOTION_GRADE,
        )
        self.assertTrue(result.all_used_capabilities_promotion_grade)
        self.assertEqual(compute_confidence_tier_from_assurance(result), "A")

    def test_has_fp_without_fp_attestation_is_c(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["addi", "fadds"],
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("addi", "fadds"),
                    _provenance_attestation(),
                ),
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
        proof = _equivalent(
            proof_features=["memory-bus"],
            memory_bus={"schema_version": 1},
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("lwz"),
                    _provenance_attestation(),
                ),
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
        proof = _equivalent(
            observables=["memory"],
            environment=MemoryEnvironment(profile=MemoryProfile.ASSUMED_ORDINARY_RAM),
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("lwz"),
                    _provenance_attestation(),
                ),
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


class FpBitwiseAssuranceTests(unittest.TestCase):
    def test_fp_bitwise_ops_frozenset(self) -> None:
        self.assertEqual(FP_BITWISE_OPS, frozenset({"fmr", "fabs", "fneg", "fnabs"}))

    def test_fmr_fabs_self_equiv_promotion_grade(self) -> None:
        opcodes = ["fmr", "fabs", "blr"]
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=opcodes,
            observables=["f1"],
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("blr"),
                    _fp_bitwise_attestation("fmr", "fabs"),
                    _provenance_attestation(),
                ),
            ).to_dict(),
        )
        used = infer_used_capabilities(proof)
        self.assertIn("fp-bitwise", used)
        self.assertNotIn("fp-scalar-arithmetic", used)

        ledger = _ledger(
            *opcodes,
            capabilities=_fp_bitwise_ledger_capabilities("fmr", "fabs"),
        )
        result = evaluate_capability_assurance(proof, ledger, _manifest())
        self.assertEqual(
            result.recomputed_statuses["fp-bitwise"],
            STATUS_PROMOTION_GRADE,
        )
        self.assertTrue(result.all_used_capabilities_promotion_grade)
        self.assertEqual(compute_confidence_tier_from_assurance(result), "A")

    def test_fadd_not_fp_bitwise_promotion_grade(self) -> None:
        opcodes = ["fadd", "blr"]
        # Even a forged promotion-grade fp-bitwise attestation with fadd in
        # evidence must recompute incomplete; demand is scalar-arithmetic.
        forged = _fp_bitwise_attestation(
            "fadd",
            status=STATUS_PROMOTION_GRADE,
        )
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=opcodes,
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("blr"),
                    forged,
                    _provenance_attestation(),
                ),
            ).to_dict(),
        )
        used = infer_used_capabilities(proof)
        self.assertIn("fp-scalar-arithmetic", used)
        self.assertNotIn("fp-bitwise", used)

        result = evaluate_capability_assurance(
            proof,
            _ledger(*opcodes),
            _manifest(),
        )
        self.assertEqual(
            result.recomputed_statuses["fp-bitwise"],
            STATUS_INCOMPLETE,
        )
        self.assertEqual(
            result.recomputed_statuses.get("fp-scalar-arithmetic"),
            STATUS_UNMODELED,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_forged_fp_bitwise_without_ledger_evidence_ignored(self) -> None:
        forged = _fp_bitwise_attestation(
            "fmr",
            status=STATUS_PROMOTION_GRADE,
            ledger_sha256=None,
            host_float=None,
        )
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fmr", "blr"],
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("blr"),
                    forged,
                    _provenance_attestation(),
                ),
            ).to_dict(),
        )
        result = evaluate_capability_assurance(
            proof,
            _ledger("fmr", "blr"),
            _manifest(),
        )
        self.assertEqual(
            result.recomputed_statuses["fp-bitwise"],
            STATUS_INCOMPLETE,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_mixed_bitwise_and_fadd_demands_both_caps(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fmr", "fadd", "blr"],
        )
        used = infer_used_capabilities(proof)
        self.assertEqual(
            used & {"fp-bitwise", "fp-scalar-arithmetic"},
            frozenset({"fp-bitwise", "fp-scalar-arithmetic"}),
        )


if __name__ == "__main__":
    unittest.main()

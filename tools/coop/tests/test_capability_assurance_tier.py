"""Capability-assurance tiering integration (Wave 1 shadow mode)."""

from __future__ import annotations

import unittest

from tools.coop.lib.equivalence_check import _cache_key
from tools.coop.lib.equivalence_policy import (
    ValidationLedger,
    compute_confidence_tier,
    proof_result_from_certificate,
)
from tools.coop.lib.targets import (
    EQUIVALENCE_CERTIFICATE_VERSION,
    equivalence_certificate_error,
    equivalence_certificate_hash,
)
from tools.ppc_equivalence.capability_assurance import (
    CapabilityAssurance,
    CapabilityManifest,
    build_attestation,
    compute_confidence_tier_from_assurance,
    evaluate_capability_assurance,
)
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
from tools.ppc_equivalence.provenance import hash_engine_tree, hash_certifier_tree
from pathlib import Path


_REPO = Path(__file__).resolve().parents[3]


def _open_ledger(*opcodes: str) -> ValidationLedger:
    return ValidationLedger(frozenset(opcodes), intentionally_loaded=True)


def _equivalent(**kwargs) -> ProofResult:
    defaults = dict(
        status=ProofStatus.EQUIVALENT,
        architecture_model=ARCHITECTURE_MODEL,
        format=RESULT_FORMAT,
        observables=["r3"],
        engine_hash="a" * 64,
        certifier_hash="d" * 64,
        source_hash="b" * 64,
        proof_request_hash="b" * 64,
        validation_ledger_hash="e" * 64,
        git_commit="c" * 40,
        git_dirty=False,
        opcodes_used=["addi", "blr"],
        memory_scope=MemoryScope(
            masking_semantics=MASKING_SEMANTICS,
            original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
        ),
    )
    defaults.update(kwargs)
    return ProofResult(**defaults)


class ShadowTierTests(unittest.TestCase):
    def test_legacy_cert_shadow_new_tier_c_legacy_may_a(self) -> None:
        proof = _equivalent()
        assurance_box: list = []
        tier = compute_confidence_tier(
            proof,
            _open_ledger("addi", "blr"),
            manifest=CapabilityManifest(
                allowed_tier_a_capabilities={"integer-core": ("integer-core-v1",)},
                shadow_mode=True,
            ),
            assurance_out=assurance_box,
        )
        self.assertEqual(tier, "A")
        self.assertTrue(assurance_box)
        self.assertEqual(
            compute_confidence_tier_from_assurance(assurance_box[0]),
            "C",
        )
        self.assertTrue(
            any(
                item.startswith("capability-assurance-shadow-tier-C")
                for item in proof.warnings
            )
        )

    def test_assurance_authoritative_when_not_shadow(self) -> None:
        from tools.ppc_equivalence.capability_requirements import (
            derive_capability_requirements,
        )

        proof = _equivalent()
        requirements = derive_capability_requirements(proof)
        by_cap = requirements.by_capability()
        attestation = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            evidence={
                "opcodes": ["addi", "blr"],
                "requirement_sha256": by_cap["integer-core"].requirement_sha256,
                "requirements_sha256": requirements.requirements_sha256,
            },
        )
        provenance = build_attestation(
            capability="provenance",
            model_version="provenance-v1",
            algorithm="provenance-binding-v1",
            evidence={
                "architecture_model": ARCHITECTURE_MODEL,
                "result_format": RESULT_FORMAT,
                "engine_hash": "a" * 64,
                "certifier_hash": "d" * 64,
                "source_hash": "b" * 64,
                "proof_request_hash": "b" * 64,
                "validation_ledger_hash": "e" * 64,
                "git_commit": "c" * 40,
                "git_dirty": False,
                "requirement_sha256": by_cap["provenance"].requirement_sha256,
                "requirements_sha256": requirements.requirements_sha256,
            },
        )
        proof.capability_requirements = requirements.to_dict()
        proof.capability_assurance = CapabilityAssurance(
            capabilities=(attestation, provenance),
        ).to_dict()
        tier = compute_confidence_tier(
            proof,
            _open_ledger("addi", "blr"),
            manifest=CapabilityManifest(
                allowed_tier_a_capabilities={
                    "integer-core": ("integer-core-v1",),
                    "provenance": ("provenance-v1",),
                },
                shadow_mode=False,
                require_capability_assurance=True,
            ),
        )
        self.assertEqual(tier, "A")

        fp_proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
        )
        fp_requirements = derive_capability_requirements(fp_proof)
        fp_proof.capability_requirements = fp_requirements.to_dict()
        fp_proof.capability_assurance = CapabilityAssurance(
            capabilities=(attestation, provenance),
        ).to_dict()
        self.assertEqual(
            compute_confidence_tier(
                fp_proof,
                _open_ledger("addi", "blr"),
                manifest=CapabilityManifest(
                    allowed_tier_a_capabilities={
                        "integer-core": ("integer-core-v1",),
                        "provenance": ("provenance-v1",),
                    },
                    shadow_mode=False,
                    require_capability_assurance=True,
                ),
            ),
            "C",
        )

    def test_fp_bitwise_shadow_assurance_a_legacy_c(self) -> None:
        from tools.ppc_equivalence.fp_bitwise import (
            FP_BITWISE_ALGORITHM,
            FP_BITWISE_MODEL_VERSION,
        )

        assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence={"opcodes": ["blr"], "ledger_sha256": "e" * 64},
                ),
                build_attestation(
                    capability="fp-bitwise",
                    model_version=FP_BITWISE_MODEL_VERSION,
                    algorithm=FP_BITWISE_ALGORITHM,
                    evidence={
                        "opcodes": ["fmr"],
                        "ledger_sha256": "e" * 64,
                        "host_float": False,
                    },
                ),
                build_attestation(
                    capability="provenance",
                    model_version="provenance-v1",
                    algorithm="provenance-binding-v1",
                    evidence={
                        "architecture_model": ARCHITECTURE_MODEL,
                        "result_format": RESULT_FORMAT,
                        "engine_hash": "a" * 64,
                        "certifier_hash": "d" * 64,
                        "source_hash": "b" * 64,
                        "proof_request_hash": "b" * 64,
                        "validation_ledger_hash": "e" * 64,
                        "git_commit": "c" * 40,
                        "git_dirty": False,
                    },
                ),
            ),
        ).to_dict()
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fmr", "blr"],
            observables=["f1"],
            capability_assurance=assurance,
        )
        assurance_box: list = []
        tier = compute_confidence_tier(
            proof,
            ValidationLedger(
                frozenset({"fmr", "blr"}),
                intentionally_loaded=True,
                capabilities={
                    "fp-bitwise": {
                        "model_version": FP_BITWISE_MODEL_VERSION,
                        "opcodes": {
                            "fmr": {
                                "result_bits": True,
                                "dolphin_interpreter": True,
                                "host_float": False,
                            }
                        },
                    }
                },
            ),
            manifest=CapabilityManifest(
                allowed_tier_a_capabilities={
                    "integer-core": ("integer-core-v1",),
                    "fp-bitwise": (FP_BITWISE_MODEL_VERSION,),
                    "provenance": ("provenance-v1",),
                },
                shadow_mode=True,
            ),
            assurance_out=assurance_box,
        )
        # Legacy effect gate still forces C for any FP under shadow mode.
        self.assertEqual(tier, "C")
        self.assertTrue(assurance_box)
        self.assertEqual(
            compute_confidence_tier_from_assurance(assurance_box[0]),
            "A",
        )


class CacheAndCertificateTests(unittest.TestCase):
    def test_cache_key_changes_when_assurance_changes(self) -> None:
        common = dict(
            contract_name="ppc-eabi",
            observables=("r3",),
            original_hex="38600001",
            candidate_hex="38600001",
            original_base=0x80000000,
            candidate_base=0x80000000,
            engine_hash="e" * 64,
            certifier_hash="c" * 64,
        )
        left_assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence={"opcodes": ["addi"]},
                ),
            ),
        ).to_dict()
        right_assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence={"opcodes": ["addi", "blr"]},
                ),
            ),
        ).to_dict()
        self.assertNotEqual(
            _cache_key(**common, capability_assurance=left_assurance),
            _cache_key(**common, capability_assurance=right_assurance),
        )
        self.assertNotEqual(
            _cache_key(**common),
            _cache_key(**common, capability_assurance=left_assurance),
        )

    def test_certificate_round_trip(self) -> None:
        from tools.ppc_equivalence.capability_requirements import (
            derive_capability_requirements,
        )

        draft = proof_result_from_certificate(
            ProofStatus.EQUIVALENT,
            {
                "architecture": ARCHITECTURE_MODEL,
                "result_format": RESULT_FORMAT,
                "opcodes_used": ["addi", "blr"],
                "observables": ["r3"],
                "engine_hash": hash_engine_tree(_REPO),
                "certifier_hash": hash_certifier_tree(_REPO),
                "source_hash": "b" * 64,
                "proof_request_hash": "b" * 64,
                "validation_ledger_hash": "e" * 64,
                "git_commit": "c" * 40,
                "git_dirty": False,
            },
        )
        requirements = derive_capability_requirements(draft)
        by_cap = requirements.by_capability()
        assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence={
                        "opcodes": list(by_cap["integer-core"].required_opcodes),
                        "requirement_sha256": by_cap["integer-core"].requirement_sha256,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
                build_attestation(
                    capability="provenance",
                    model_version="provenance-v1",
                    algorithm="provenance-binding-v1",
                    evidence={
                        "architecture_model": ARCHITECTURE_MODEL,
                        "result_format": RESULT_FORMAT,
                        "engine_hash": hash_engine_tree(_REPO),
                        "certifier_hash": hash_certifier_tree(_REPO),
                        "source_hash": "b" * 64,
                        "proof_request_hash": "b" * 64,
                        "validation_ledger_hash": "e" * 64,
                        "git_commit": "c" * 40,
                        "git_dirty": False,
                        "requirement_sha256": by_cap["provenance"].requirement_sha256,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
            ),
        ).to_dict()
        certificate = {
            "version": EQUIVALENCE_CERTIFICATE_VERSION,
            "status": "SEMANTIC_CERTIFIED",
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "target_id": "demo",
            "evidence": "symbolic-equivalence",
            "retail_sha256": "1" * 64,
            "candidate_sha256": "2" * 64,
            "summary": {
                "reads": ["r3"],
                "writes": ["r3"],
                "invalid_reasons": [],
                "return_behavior": "normal",
            },
            "callees": [],
            "helpers": [],
            "engine_hash": hash_engine_tree(_REPO),
            "certifier_hash": hash_certifier_tree(_REPO),
            "capability_assurance": assurance,
            "capability_requirements": requirements.to_dict(),
            "opcodes_used": ["addi", "blr"],
            "observables": ["r3"],
            "source_hash": "b" * 64,
            "proof_request_hash": "b" * 64,
            "validation_ledger_hash": "e" * 64,
            "git_commit": "c" * 40,
            "git_dirty": False,
        }
        certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)

        restored = proof_result_from_certificate(
            ProofStatus.EQUIVALENT, certificate
        )
        self.assertEqual(restored.capability_assurance, assurance)
        self.assertEqual(restored.certifier_hash, certificate["certifier_hash"])
        self.assertEqual(restored.proof_request_hash, "b" * 64)
        self.assertEqual(restored.validation_ledger_hash, "e" * 64)
        self.assertFalse(restored.git_dirty)

        row = {"id": "demo", "status": "EQUIVALENT_MATCH", "equivalence_certificate": certificate}
        error = equivalence_certificate_error(row, {"demo": row})
        self.assertIsNone(error)

        # Malformed assurance fails closed.
        bad = dict(certificate)
        bad["capability_assurance"] = {
            "schema_version": 1,
            "policy": "capability-assurance-v1",
            "capabilities": [
                {
                    "capability": "integer-core",
                    "model_version": "integer-core-v1",
                    "algorithm": "unknown-algo",
                    "status": "incomplete",
                    "assumptions": [],
                    "unsupported": [],
                    "evidence": {},
                    "attestation_sha256": "0" * 64,
                }
            ],
        }
        bad["certificate_sha256"] = equivalence_certificate_hash(bad)
        bad_row = {"id": "demo", "status": "EQUIVALENT_MATCH", "equivalence_certificate": bad}
        self.assertIn(
            "capability_assurance invalid",
            equivalence_certificate_error(bad_row, {"demo": bad_row}) or "",
        )

    def test_fp_bitwise_certificate_round_trip(self) -> None:
        from tools.ppc_equivalence.capability_requirements import (
            derive_capability_requirements,
        )
        from tools.ppc_equivalence.fp_bitwise import (
            FP_BITWISE_ALGORITHM,
            FP_BITWISE_MODEL_VERSION,
        )

        draft = proof_result_from_certificate(
            ProofStatus.EQUIVALENT,
            {
                "architecture": ARCHITECTURE_MODEL,
                "result_format": RESULT_FORMAT,
                "opcodes_used": ["fmr", "fabs", "blr"],
                "observables": ["f1"],
                "engine_hash": hash_engine_tree(_REPO),
                "certifier_hash": hash_certifier_tree(_REPO),
                "source_hash": "b" * 64,
                "proof_request_hash": "b" * 64,
                "validation_ledger_hash": "e" * 64,
                "git_commit": "c" * 40,
                "git_dirty": False,
                "floating_point_domain": FloatingPointDomain().to_dict(),
            },
        )
        requirements = derive_capability_requirements(draft)
        by_cap = requirements.by_capability()
        assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence={
                        "opcodes": list(by_cap["integer-core"].required_opcodes),
                        "ledger_sha256": "e" * 64,
                        "requirement_sha256": by_cap["integer-core"].requirement_sha256,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
                build_attestation(
                    capability="fp-bitwise",
                    model_version=FP_BITWISE_MODEL_VERSION,
                    algorithm=FP_BITWISE_ALGORITHM,
                    evidence={
                        "opcodes": list(by_cap["fp-bitwise"].required_opcodes),
                        "ledger_sha256": "e" * 64,
                        "host_float": False,
                        "requirement_sha256": by_cap["fp-bitwise"].requirement_sha256,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
                build_attestation(
                    capability="provenance",
                    model_version="provenance-v1",
                    algorithm="provenance-binding-v1",
                    evidence={
                        "architecture_model": ARCHITECTURE_MODEL,
                        "result_format": RESULT_FORMAT,
                        "engine_hash": hash_engine_tree(_REPO),
                        "certifier_hash": hash_certifier_tree(_REPO),
                        "source_hash": "b" * 64,
                        "proof_request_hash": "b" * 64,
                        "validation_ledger_hash": "e" * 64,
                        "git_commit": "c" * 40,
                        "git_dirty": False,
                        "requirement_sha256": by_cap["provenance"].requirement_sha256,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
            ),
        ).to_dict()
        certificate = {
            "version": EQUIVALENCE_CERTIFICATE_VERSION,
            "status": "SEMANTIC_CERTIFIED",
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "target_id": "fp-bitwise-demo",
            "evidence": "symbolic-equivalence",
            "retail_sha256": "1" * 64,
            "candidate_sha256": "2" * 64,
            "summary": {
                "reads": ["f1"],
                "writes": ["f1"],
                "invalid_reasons": [],
                "return_behavior": "normal",
            },
            "callees": [],
            "helpers": [],
            "engine_hash": hash_engine_tree(_REPO),
            "certifier_hash": hash_certifier_tree(_REPO),
            "capability_assurance": assurance,
            "capability_requirements": requirements.to_dict(),
            "opcodes_used": ["fmr", "fabs", "blr"],
            "observables": ["f1"],
            "source_hash": "b" * 64,
            "proof_request_hash": "b" * 64,
            "validation_ledger_hash": "e" * 64,
            "git_commit": "c" * 40,
            "git_dirty": False,
            "floating_point_domain": FloatingPointDomain().to_dict(),
        }
        certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)

        restored = proof_result_from_certificate(
            ProofStatus.EQUIVALENT, certificate
        )
        self.assertEqual(restored.capability_assurance, assurance)
        caps = {
            item["capability"]
            for item in restored.capability_assurance["capabilities"]
        }
        self.assertIn("fp-bitwise", caps)

        row = {
            "id": "fp-bitwise-demo",
            "status": "EQUIVALENT_MATCH",
            "equivalence_certificate": certificate,
        }
        self.assertIsNone(
            equivalence_certificate_error(row, {"fp-bitwise-demo": row})
        )


if __name__ == "__main__":
    unittest.main()

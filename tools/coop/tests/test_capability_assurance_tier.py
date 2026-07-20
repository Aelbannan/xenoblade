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
        attestation = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            evidence={"opcodes": ["addi", "blr"]},
        )
        proof = _equivalent(
            capability_assurance=CapabilityAssurance(
                capabilities=(attestation,),
            ).to_dict(),
        )
        tier = compute_confidence_tier(
            proof,
            _open_ledger("addi", "blr"),
            manifest=CapabilityManifest(
                allowed_tier_a_capabilities={"integer-core": ("integer-core-v1",)},
                shadow_mode=False,
            ),
        )
        self.assertEqual(tier, "A")

        fp_proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            capability_assurance=CapabilityAssurance(
                capabilities=(attestation,),
            ).to_dict(),
        )
        self.assertEqual(
            compute_confidence_tier(
                fp_proof,
                _open_ledger("addi", "blr"),
                manifest=CapabilityManifest(
                    allowed_tier_a_capabilities={
                        "integer-core": ("integer-core-v1",),
                    },
                    shadow_mode=False,
                ),
            ),
            "C",
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
        attestation = build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            evidence={"opcodes": ["addi", "blr"]},
        )
        assurance = CapabilityAssurance(capabilities=(attestation,)).to_dict()
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
            "opcodes_used": ["addi", "blr"],
            "observables": ["r3"],
            "source_hash": "b" * 64,
            "git_commit": "c" * 40,
        }
        certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)

        restored = proof_result_from_certificate(
            ProofStatus.EQUIVALENT, certificate
        )
        self.assertEqual(restored.capability_assurance, assurance)

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


if __name__ == "__main__":
    unittest.main()

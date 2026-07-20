"""Wave 5 capability-assurance rollout / authoritative canary tests."""

from __future__ import annotations

import unittest
from pathlib import Path

from tools.coop.lib.equivalence_policy import (
    PromotionPolicy,
    ValidationLedger,
    classify_for_promotion,
    compute_confidence_tier,
)
from tools.ppc_equivalence.capability_assurance import (
    CapabilityAssurance,
    CapabilityManifest,
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    _recompute_attestation_status,
    authoritative_capability_manifest_path,
    build_attestation,
    build_capability_assurance_audit,
    compute_confidence_tier_from_assurance,
    default_capability_manifest_path,
    load_capability_manifest,
    maybe_attach_integer_core_draft,
    maybe_attach_provenance_draft,
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


_REPO = Path(__file__).resolve().parents[3]
_AUTH_MANIFEST = _REPO / "tools/coop/capability_manifest.authoritative.json"
_SHADOW_MANIFEST = _REPO / "tools/coop/capability_manifest.json"


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


def _provenance(**evidence_overrides) -> object:
    evidence = {
        "architecture_model": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "engine_hash": "a" * 64,
        "certifier_hash": "d" * 64,
        "source_hash": "b" * 64,
        "proof_request_hash": "b" * 64,
        "validation_ledger_hash": "e" * 64,
        "git_commit": "c" * 40,
        "git_dirty": False,
    }
    evidence.update(evidence_overrides)
    return build_attestation(
        capability="provenance",
        model_version="provenance-v1",
        algorithm="provenance-binding-v1",
        evidence=evidence,
    )


def _integer(*opcodes: str) -> object:
    return build_attestation(
        capability="integer-core",
        model_version="integer-core-v1",
        algorithm="opcode-ledger-v2",
        evidence={"opcodes": list(opcodes) or ["addi", "blr"]},
    )


def _authoritative_manifest() -> CapabilityManifest:
    return load_capability_manifest(_AUTH_MANIFEST)


def _shadow_manifest() -> CapabilityManifest:
    return load_capability_manifest(_SHADOW_MANIFEST)


class ManifestFilesTests(unittest.TestCase):
    def test_default_stays_shadow(self) -> None:
        loaded = load_capability_manifest(default_capability_manifest_path())
        self.assertTrue(loaded.shadow_mode)
        self.assertFalse(loaded.require_capability_assurance)

    def test_authoritative_file_is_not_shadow(self) -> None:
        loaded = load_capability_manifest(authoritative_capability_manifest_path())
        self.assertFalse(loaded.shadow_mode)
        self.assertTrue(loaded.require_capability_assurance)
        # Safe Wave 5 set only — unfinished Wave 3/4 caps stay empty.
        self.assertEqual(
            loaded.allowed_versions("integer-core"),
            ("integer-core-v1",),
        )
        self.assertEqual(loaded.allowed_versions("provenance"), ("provenance-v1",))
        self.assertEqual(loaded.allowed_versions("fp-bitwise"), ("fp-bitwise-v1",))
        self.assertEqual(
            loaded.allowed_versions("bounded-memory"),
            ("bounded-memory-v2",),
        )
        self.assertEqual(
            loaded.allowed_versions("certified-calls"),
            ("certified-calls-v2",),
        )
        self.assertEqual(loaded.allowed_versions("fp-scalar-arithmetic"), ())
        self.assertEqual(loaded.allowed_versions("mmio-register-bank"), ())
        self.assertEqual(loaded.allowed_versions("gx-fifo-write-trace"), ())
        self.assertEqual(loaded.allowed_versions("gx-fifo-read"), ())


class AuthoritativeCanaryTierTests(unittest.TestCase):
    def test_integer_core_provenance_complete_ledger_is_tier_a(self) -> None:
        proof = _equivalent(
            capability_assurance=CapabilityAssurance(
                capabilities=(_integer("addi", "blr"), _provenance()),
            ).to_dict(),
        )
        audit_box: list = []
        tier = compute_confidence_tier(
            proof,
            _open_ledger("addi", "blr"),
            manifest=_authoritative_manifest(),
            audit_out=audit_box,
        )
        self.assertEqual(tier, "A")
        self.assertTrue(audit_box)
        self.assertFalse(audit_box[0]["shadow_mode"])
        self.assertEqual(audit_box[0]["authoritative_tier"], "A")
        self.assertIn("integer-core", audit_box[0]["capabilities_used"])
        self.assertIn("provenance", audit_box[0]["capabilities_used"])
        self.assertTrue(
            any(
                str(item).startswith("capability-assurance-audit:")
                for item in proof.warnings
            )
        )

    def test_any_fp_arithmetic_is_tier_c(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fadd", "blr"],
            observables=["f1"],
            capability_assurance=CapabilityAssurance(
                capabilities=(_integer("blr"), _provenance()),
            ).to_dict(),
        )
        self.assertEqual(
            compute_confidence_tier(
                proof,
                _open_ledger("fadd", "blr"),
                manifest=_authoritative_manifest(),
            ),
            "C",
        )

    def test_memory_bus_is_tier_c(self) -> None:
        proof = _equivalent(
            proof_features=["memory-bus"],
            memory_bus={"schema_version": 2, "status": "pending"},
            capability_assurance=CapabilityAssurance(
                capabilities=(_integer("addi", "blr"), _provenance()),
            ).to_dict(),
        )
        self.assertEqual(
            compute_confidence_tier(
                proof,
                _open_ledger("addi", "blr"),
                manifest=_authoritative_manifest(),
            ),
            "C",
        )

    def test_assumed_ram_with_memory_observable_is_tier_c(self) -> None:
        proof = _equivalent(
            observables=["r3", "memory"],
            environment=MemoryEnvironment(
                profile=MemoryProfile.ASSUMED_ORDINARY_RAM,
            ),
            capability_assurance=CapabilityAssurance(
                capabilities=(_integer("addi", "blr"), _provenance()),
            ).to_dict(),
        )
        self.assertEqual(
            compute_confidence_tier(
                proof,
                _open_ledger("addi", "blr"),
                manifest=_authoritative_manifest(),
            ),
            "C",
        )

    def test_legacy_cert_without_assurance_is_tier_c_not_inferred(self) -> None:
        proof = _equivalent()  # no capability_assurance block
        assurance_box: list = []
        tier = compute_confidence_tier(
            proof,
            _open_ledger("addi", "blr"),
            manifest=_authoritative_manifest(),
            assurance_out=assurance_box,
        )
        self.assertEqual(tier, "C")
        self.assertTrue(assurance_box)
        self.assertEqual(assurance_box[0].attestations, ())
        self.assertTrue(assurance_box[0].has_unmodeled_or_unproven_capability)
        self.assertEqual(
            compute_confidence_tier_from_assurance(assurance_box[0]),
            "C",
        )


class ShadowStillAuthoritativeLegacyTests(unittest.TestCase):
    def test_shadow_manifest_legacy_effect_gate_still_authoritative(self) -> None:
        # Legacy would be A; assurance without attestations would be C.
        proof = _equivalent()
        assurance_box: list = []
        audit_box: list = []
        tier = compute_confidence_tier(
            proof,
            _open_ledger("addi", "blr"),
            manifest=_shadow_manifest(),
            assurance_out=assurance_box,
            audit_out=audit_box,
        )
        self.assertEqual(tier, "A")
        self.assertTrue(assurance_box)
        self.assertEqual(
            compute_confidence_tier_from_assurance(assurance_box[0]),
            "C",
        )
        self.assertTrue(audit_box[0]["shadow_mode"])
        self.assertEqual(audit_box[0]["authoritative_tier"], "A")
        self.assertEqual(audit_box[0]["assurance_tier"], "C")
        self.assertTrue(
            any(
                str(item).startswith("capability-assurance-shadow-tier-C")
                for item in proof.warnings
            )
        )


class ProvenanceAndPromotionTests(unittest.TestCase):
    def test_unrelated_dirty_does_not_force_provenance_incomplete(self) -> None:
        """Scoped git_dirty=False keeps provenance promotion-grade.

        Whole-repo dirt outside the engine/certifier trust boundary must not
        force incomplete provenance when evidence and result both report
        git_dirty=False (as live_git_identity / git_trust_boundary_dirty do).
        """
        from tools.ppc_equivalence.provenance import is_trust_boundary_relative_path

        self.assertFalse(is_trust_boundary_relative_path("src/kyoshin/CTaskGame.cpp"))
        proof = _equivalent(
            git_dirty=False,
            capability_assurance=CapabilityAssurance(
                capabilities=(_integer("addi", "blr"), _provenance(git_dirty=False)),
            ).to_dict(),
        )
        status = _recompute_attestation_status(
            _provenance(git_dirty=False),
            result=proof,
            ledger=_open_ledger("addi", "blr"),
            manifest=_authoritative_manifest(),
        )
        self.assertEqual(status, STATUS_PROMOTION_GRADE)
        self.assertEqual(
            compute_confidence_tier(
                proof,
                _open_ledger("addi", "blr"),
                manifest=_authoritative_manifest(),
            ),
            "A",
        )

    def test_missing_provenance_fields_block_promotion(self) -> None:
        proof = _equivalent(
            engine_hash="",
            certifier_hash="",
            source_hash="",
            git_commit="",
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer("addi", "blr"),
                    _provenance(
                        engine_hash="",
                        certifier_hash="",
                        source_hash="",
                        git_commit="",
                    ),
                ),
            ).to_dict(),
        )
        auth = _authoritative_manifest()
        # Authoritative tier classification demotes on incomplete provenance.
        self.assertEqual(
            compute_confidence_tier(
                proof,
                _open_ledger("addi", "blr"),
                manifest=auth,
            ),
            "C",
        )
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(
                automatic_promotion=False,
                allowed_tier_a_capabilities=dict(auth.allowed_tier_a_capabilities),
                capability_assurance_shadow_mode=False,
                require_capability_assurance=True,
                allowed_engine_sha256="a" * 64,
            ),
            _open_ledger("addi", "blr"),
        )
        self.assertFalse(decision.allowed)
        self.assertIn("missing-engine-provenance", decision.blockers)
        self.assertIn("missing-certifier-hash", decision.blockers)
        self.assertIn("missing-source-hash", decision.blockers)
        self.assertIn("missing-git-commit", decision.blockers)
        self.assertIsNotNone(decision.assurance_audit)
        assert decision.assurance_audit is not None
        self.assertEqual(decision.assurance_audit["authoritative_tier"], "C")
        self.assertEqual(
            decision.assurance_audit["recomputed_statuses"].get("provenance"),
            STATUS_INCOMPLETE,
        )

    def test_classify_records_assurance_audit(self) -> None:
        proof = _equivalent(
            capability_assurance=CapabilityAssurance(
                capabilities=(_integer("addi", "blr"), _provenance()),
            ).to_dict(),
        )
        auth = _authoritative_manifest()
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(
                automatic_promotion=False,
                allowed_tier_a_capabilities=dict(auth.allowed_tier_a_capabilities),
                capability_assurance_shadow_mode=False,
                require_capability_assurance=True,
                allowed_engine_sha256=proof.engine_hash,
            ),
            _open_ledger("addi", "blr"),
        )
        self.assertEqual(decision.confidence_tier, "A")
        self.assertIsNotNone(decision.assurance_audit)
        assert decision.assurance_audit is not None
        self.assertFalse(decision.assurance_audit["shadow_mode"])
        self.assertEqual(decision.assurance_audit["authoritative_tier"], "A")
        self.assertIn("integer-core", decision.assurance_audit["capabilities_used"])
        self.assertTrue(
            any(
                str(item).startswith("capability-assurance-audit:")
                for item in decision.warnings
            )
        )


class EndToEndDraftAttachTests(unittest.TestCase):
    def test_drafts_attached_evaluate_tier_a_under_authoritative(self) -> None:
        proof = _equivalent()
        ledger = _open_ledger("addi", "blr")
        maybe_attach_integer_core_draft(proof, ledger=ledger, ledger_sha256="e" * 64)
        maybe_attach_provenance_draft(proof, ledger_sha256="e" * 64)
        self.assertIsNotNone(proof.capability_assurance)
        audit_box: list = []
        tier = compute_confidence_tier(
            proof,
            ledger,
            manifest=_authoritative_manifest(),
            audit_out=audit_box,
        )
        self.assertEqual(tier, "A")
        summary = build_capability_assurance_audit(
            None,
            shadow_mode=False,
            authoritative_tier=tier,
        )
        self.assertEqual(summary["authoritative_tier"], "A")
        self.assertTrue(audit_box)
        self.assertEqual(audit_box[0]["assurance_tier"], "A")


if __name__ == "__main__":
    unittest.main()

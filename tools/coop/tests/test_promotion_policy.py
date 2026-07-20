"""P0-02 promotion safety gate tests."""

from __future__ import annotations

import unittest
from pathlib import Path
from unittest import mock

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.equivalence_policy import (
    PromotionDecision,
    PromotionPolicy,
    ValidationLedger,
    classify_for_promotion,
    classify_for_promotion_legacy,
    compute_confidence_tier,
    proof_result_from_certificate,
)
from tools.coop.lib.objdiff_report import UnitReport, classify_status, meets_required_level
from tools.coop.lib import targets as targets_mod
from tools.coop.lib.targets import (
    EQUIVALENCE_CERTIFICATE_VERSION,
    equivalence_certificate_error,
    equivalence_certificate_hash,
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


def _open_ledger(*opcodes: str) -> ValidationLedger:
    """Construct a non-absent ledger for promotion tests."""
    return ValidationLedger(frozenset(opcodes), intentionally_loaded=True)


def _unit(**kwargs) -> UnitReport:
    defaults = dict(
        unit_name="u",
        code_match_percent=0.0,
        data_match_percent=0.0,
        fuzzy_match_percent=0.0,
        total_functions=1,
        matched_functions=0,
        functions=[],
    )
    defaults.update(kwargs)
    return UnitReport(**defaults)


def _config(**kwargs) -> CoopConfig:
    defaults = dict(
        project_root=".",
        automatic_promotion=True,
        reject_architecture_models=("broadway-ppc32-be-v18",),
        allowed_confidence_tiers=frozenset({"A", "B"}),
    )
    defaults.update(kwargs)
    return CoopConfig(**defaults)


def _equivalent_proof(**kwargs) -> ProofResult:
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
        memory_scope=MemoryScope(
            masking_semantics=MASKING_SEMANTICS,
            original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
        ),
    )
    defaults.update(kwargs)
    return ProofResult(**defaults)


class ClassifyForPromotionTests(unittest.TestCase):
    def test_equivalent_status_is_not_sufficient_for_promotion(self) -> None:
        proof = _equivalent_proof(
            architecture_model="broadway-ppc32-be-v18",
            memory_scope=MemoryScope(masking_semantics="union-mask-legacy"),
        )
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(),
            _open_ledger(),
        )
        self.assertFalse(decision.allowed)
        self.assertTrue(
            any("architecture-model-" in item and "rejected" in item
                for item in decision.blockers)
        )
        self.assertTrue(
            any("unsafe-or-unknown-private-stack-masking" in item
                for item in decision.blockers)
        )

    def test_kill_switch_blocks_even_strong_proof(self) -> None:
        proof = _equivalent_proof()
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(automatic_promotion=False),
            _open_ledger(),
        )
        self.assertFalse(decision.allowed)
        self.assertIn("automatic-promotion-disabled-by-config", decision.blockers)

    def test_strong_proof_allowed_when_promotion_enabled(self) -> None:
        proof = _equivalent_proof()
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(
                automatic_promotion=True,
                allowed_engine_sha256=proof.engine_hash,
            ),
            _open_ledger(),
        )
        self.assertTrue(decision.allowed)
        self.assertEqual(decision.confidence_tier, "A")

    def test_missing_provenance_fields_block_promotion(self) -> None:
        proof = _equivalent_proof(
            engine_hash="",
            certifier_hash="",
            source_hash="",
            git_commit="",
        )
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(automatic_promotion=False),
            _open_ledger(),
        )
        self.assertFalse(decision.allowed)
        self.assertIn("missing-engine-provenance", decision.blockers)
        self.assertIn("missing-certifier-hash", decision.blockers)
        self.assertIn("missing-source-hash", decision.blockers)
        self.assertIn("missing-git-commit", decision.blockers)
        self.assertNotIn("missing-allowed-engine-sha256", decision.blockers)

    def test_git_dirty_blocks_promotion(self) -> None:
        proof = _equivalent_proof(git_dirty=True)
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(automatic_promotion=False),
            _open_ledger(),
        )
        self.assertFalse(decision.allowed)
        self.assertIn("git-dirty", decision.blockers)

    def test_stale_architecture_or_format_blocks_promotion(self) -> None:
        stale_arch = classify_for_promotion(
            _equivalent_proof(architecture_model="broadway-ppc32-be-v36"),
            PromotionPolicy(automatic_promotion=False),
            _open_ledger(),
        )
        self.assertFalse(stale_arch.allowed)
        self.assertTrue(
            any(
                item.startswith("architecture-model-broadway-ppc32-be-v36!=live-")
                for item in stale_arch.blockers
            )
        )

        stale_format = classify_for_promotion(
            _equivalent_proof(format=RESULT_FORMAT - 1),
            PromotionPolicy(automatic_promotion=False),
            _open_ledger(),
        )
        self.assertFalse(stale_format.allowed)
        self.assertTrue(
            any(
                item.startswith(f"result-format-{RESULT_FORMAT - 1}!=live-")
                for item in stale_format.blockers
            )
        )

    def test_automatic_promotion_requires_allowed_engine_sha256(self) -> None:
        proof = _equivalent_proof()
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(automatic_promotion=True, allowed_engine_sha256=None),
            _open_ledger(),
        )
        self.assertFalse(decision.allowed)
        self.assertIn("missing-allowed-engine-sha256", decision.blockers)

    def test_certificate_provenance_hash_round_trip(self) -> None:
        certificate = {
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "engine_hash": "a" * 64,
            "certifier_hash": "d" * 64,
            "source_hash": "b" * 64,
            "proof_request_hash": "b" * 64,
            "validation_ledger_hash": "e" * 64,
            "git_commit": "c" * 40,
            "git_dirty": False,
            "observables": ["r3"],
            "memory_scope": MemoryScope(
                masking_semantics=MASKING_SEMANTICS,
                original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
                candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            ).to_dict(),
            "summary": {"reads": ["r3"], "writes": ["r3"], "invalid_reasons": []},
        }
        result = proof_result_from_certificate(ProofStatus.EQUIVALENT, certificate)
        self.assertEqual(result.certifier_hash, "d" * 64)
        self.assertEqual(result.proof_request_hash, "b" * 64)
        self.assertEqual(result.validation_ledger_hash, "e" * 64)
        self.assertFalse(result.git_dirty)

    def test_absent_ledger_blocks_promotion(self) -> None:
        proof = _equivalent_proof(opcodes_used=["add"])
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(automatic_promotion=True),
            ValidationLedger(frozenset()),
        )
        self.assertFalse(decision.allowed)
        self.assertIn("validation-ledger-absent", decision.blockers)
        self.assertEqual(decision.confidence_tier, "C")

    def test_tier_c_fp_not_allowed_by_default(self) -> None:
        proof = _equivalent_proof(floating_point_domain=FloatingPointDomain())
        decision = classify_for_promotion(
            proof,
            PromotionPolicy(automatic_promotion=True),
            _open_ledger(),
        )
        self.assertFalse(decision.allowed)
        self.assertEqual(decision.confidence_tier, "C")
        self.assertIn("confidence-tier-C-not-allowed", decision.blockers)

    def test_assumed_ram_with_memory_observable_is_tier_c(self) -> None:
        proof = _equivalent_proof(
            observables=["memory"],
            environment=MemoryEnvironment(),
        )
        self.assertEqual(
            compute_confidence_tier(proof, _open_ledger()),
            "C",
        )

    def test_assumed_ram_register_only_stays_tier_a(self) -> None:
        proof = _equivalent_proof(
            observables=["r3"],
            environment=MemoryEnvironment(),
        )
        self.assertEqual(
            compute_confidence_tier(proof, _open_ledger()),
            "A",
        )

    def test_require_bounded_ram_blocks_assumed_and_empty(self) -> None:
        ledger = _open_ledger()
        engine = "a" * 64
        policy = PromotionPolicy(
            automatic_promotion=True,
            require_bounded_ram=True,
            allowed_engine_sha256=engine,
        )

        missing = classify_for_promotion(
            _equivalent_proof(engine_hash=engine), policy, ledger
        )
        self.assertFalse(missing.allowed)
        self.assertIn("unconstrained-symbolic-memory-domain", missing.blockers)

        assumed = classify_for_promotion(
            _equivalent_proof(engine_hash=engine, environment=MemoryEnvironment()),
            policy,
            ledger,
        )
        self.assertFalse(assumed.allowed)
        self.assertIn("unconstrained-symbolic-memory-domain", assumed.blockers)

        empty_bounded = classify_for_promotion(
            _equivalent_proof(
                engine_hash=engine,
                environment=MemoryEnvironment(
                    profile=MemoryProfile.BOUNDED_ORDINARY_RAM, ranges=[],
                ),
            ),
            policy,
            ledger,
        )
        self.assertFalse(empty_bounded.allowed)
        self.assertIn(
            "unconstrained-symbolic-memory-domain", empty_bounded.blockers,
        )

        bounded = classify_for_promotion(
            _equivalent_proof(
                engine_hash=engine,
                environment=MemoryEnvironment(
                    profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
                    ranges=[(0x80000000, 0x817FFFFF)],
                ),
            ),
            policy,
            ledger,
        )
        self.assertTrue(bounded.allowed)

    def test_legacy_wrapper_accepts_old_objdiff_signature(self) -> None:
        config = _config(automatic_promotion=False)
        decision = classify_for_promotion_legacy(
            ProofStatus.EQUIVALENT,
            85.0,
            config,
            certificate=None,
        )
        self.assertFalse(decision.allowed)
        self.assertIn("automatic-promotion-disabled-by-config", decision.blockers)

    def test_legacy_wrapper_uses_live_proof(self) -> None:
        proof = _equivalent_proof()
        config = _config(
            automatic_promotion=True,
            allowed_engine_sha256=proof.engine_hash,
        )
        decision = classify_for_promotion_legacy(
            ProofStatus.EQUIVALENT,
            85.0,
            config,
            proof=proof,
        )
        self.assertTrue(decision.allowed)


class ClassifyStatusPolicyTests(unittest.TestCase):
    def test_full_match_does_not_depend_on_equivalence_promotion(self) -> None:
        status = classify_status(
            100.0,
            _unit(),
            symbol="f",
            equivalence=None,
            policy=_config(automatic_promotion=False),
        )
        self.assertEqual(status, "FULL_MATCH")

    def test_policy_blocks_equivalent_match_when_promotion_disabled(self) -> None:
        status = classify_status(
            85.0,
            _unit(),
            symbol="f",
            equivalence=ProofStatus.EQUIVALENT,
            policy=_config(automatic_promotion=False),
            proof=_equivalent_proof(),
        )
        self.assertEqual(status, "HIGH_MATCH")

    def test_policy_allows_equivalent_match_when_promotion_enabled(self) -> None:
        proof = _equivalent_proof()
        status = classify_status(
            85.0,
            _unit(),
            symbol="f",
            equivalence=ProofStatus.EQUIVALENT,
            policy=_config(
                automatic_promotion=True,
                allowed_engine_sha256=proof.engine_hash,
            ),
            proof=proof,
        )
        self.assertEqual(status, "EQUIVALENT_MATCH")

    def test_meets_required_level_uses_promotion_decision(self) -> None:
        unit = _unit()
        proof = _equivalent_proof()
        self.assertFalse(
            meets_required_level(
                "EQUIVALENT_MATCH",
                "HIGH_MATCH",
                function_match=85.0,
                unit=unit,
                symbol="f",
                equivalence=ProofStatus.EQUIVALENT,
                policy=_config(automatic_promotion=False),
                proof=proof,
            )
        )
        self.assertTrue(
            meets_required_level(
                "EQUIVALENT_MATCH",
                "HIGH_MATCH",
                function_match=85.0,
                unit=unit,
                symbol="f",
                equivalence=ProofStatus.EQUIVALENT,
                policy=_config(
                    automatic_promotion=True,
                    allowed_engine_sha256=proof.engine_hash,
                ),
                proof=proof,
            )
        )


class MemoryConfigHelpersTests(unittest.TestCase):
    def test_memory_environment_from_config(self) -> None:
        from tools.coop.lib.config import memory_environment_from_config

        self.assertIsNone(memory_environment_from_config(_config()))
        bounded = memory_environment_from_config(
            _config(
                memory_profile="bounded-ordinary-ram",
                memory_ranges=["0x80000000,0x817fffff"],
            )
        )
        assert bounded is not None
        env = MemoryEnvironment.from_dict(bounded)
        self.assertEqual(env.profile, MemoryProfile.BOUNDED_ORDINARY_RAM)
        self.assertEqual(env.ranges, [(0x80000000, 0x817FFFFF)])
        empty = memory_environment_from_config(
            _config(memory_profile="bounded-ordinary-ram", memory_ranges=[]),
        )
        assert empty is not None
        self.assertTrue(MemoryEnvironment.from_dict(empty).is_fail_closed_empty())


class CertificateReconstructionTests(unittest.TestCase):
    def test_proof_result_from_certificate_round_trip(self) -> None:
        scope = MemoryScope(
            masking_semantics=MASKING_SEMANTICS,
            original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            candidate=PrivateStackInfo(enabled_on_all_terminal_paths=False),
        )
        certificate = {
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "engine_hash": "a" * 64,
            "source_hash": "b" * 64,
            "git_commit": "c" * 40,
            "contract": "ppc-eabi",
            "assumptions": ["assumption-one"],
            "callee_contracts": {
                "leaf": {"source": "certified:abc", "reads": ["r3"], "writes": ["r3"]},
            },
            "limits": {"max_instructions": 64, "max_paths": 8, "max_loop_iterations": 4},
            "memory_scope": scope.to_dict(),
            "environment": MemoryEnvironment(
                profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
                ranges=[(0x80000000, 0x817FFFFF)],
            ).to_dict(),
            "floating_point_domain": FloatingPointDomain().to_dict(),
            "observables": ["r3", "memory"],
            "summary": {"reads": ["r3"], "writes": ["r3"], "invalid_reasons": []},
        }
        result = proof_result_from_certificate(ProofStatus.EQUIVALENT, certificate)
        self.assertEqual(result.architecture_model, ARCHITECTURE_MODEL)
        self.assertEqual(result.engine_hash, "a" * 64)
        self.assertEqual(result.contract, "ppc-eabi")
        self.assertEqual(result.assumptions, ["assumption-one"])
        self.assertEqual(result.callee_contracts["leaf"]["source"], "certified:abc")
        self.assertEqual(result.limits["max_loop_iterations"], 4)
        self.assertIsNotNone(result.memory_scope)
        assert result.memory_scope is not None
        self.assertEqual(result.memory_scope.masking_semantics, MASKING_SEMANTICS)
        self.assertIsNotNone(result.floating_point_domain)
        self.assertEqual(result.environment.profile, MemoryProfile.BOUNDED_ORDINARY_RAM)
        self.assertEqual(compute_confidence_tier(result), "C")

    def test_invalid_reasons_reconstruct_as_tier_c(self) -> None:
        certificate = {
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "engine_hash": "a" * 64,
            "source_hash": "b" * 64,
            "git_commit": "c" * 40,
            "contract": "ppc-eabi",
            "observables": ["r3"],
            "memory_scope": MemoryScope(
                masking_semantics=MASKING_SEMANTICS,
                original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
                candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            ).to_dict(),
            "summary": {
                "reads": ["r3"],
                "writes": ["r3"],
                "invalid_reasons": [1],
            },
        }
        result = proof_result_from_certificate(ProofStatus.EQUIVALENT, certificate)
        self.assertEqual(result.invalid_reasons, [1])
        self.assertEqual(result.counterexample_kind, "definedness")
        self.assertEqual(compute_confidence_tier(result), "C")

    def test_fp_oracle_version_round_trips(self) -> None:
        from tools.ppc_equivalence.fp_outcome import FP_ORACLE_VERSION

        certificate = {
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "engine_hash": "a" * 64,
            "source_hash": "b" * 64,
            "git_commit": "c" * 40,
            "contract": "ppc-eabi",
            "observables": ["f1"],
            "fp_oracle_version": FP_ORACLE_VERSION,
            "memory_scope": MemoryScope(
                masking_semantics=MASKING_SEMANTICS,
                original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
                candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            ).to_dict(),
            "summary": {"reads": ["f1"], "writes": ["f1"], "invalid_reasons": []},
        }
        result = proof_result_from_certificate(ProofStatus.EQUIVALENT, certificate)
        self.assertEqual(result.fp_oracle_version, FP_ORACLE_VERSION)
        # A certificate without the field must not fabricate one.
        certificate.pop("fp_oracle_version")
        bare = proof_result_from_certificate(ProofStatus.EQUIVALENT, certificate)
        self.assertIsNone(bare.fp_oracle_version)


def _registry_certificate(target_id: str, callees: list[dict[str, str]] | None = None) -> dict:
    from tools.ppc_equivalence.provenance import hash_certifier_tree, hash_engine_tree

    repo = Path(__file__).resolve().parents[3]
    certificate = {
        "version": EQUIVALENCE_CERTIFICATE_VERSION,
        "status": "SEMANTIC_CERTIFIED",
        "architecture": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "target_id": target_id,
        "evidence": "symbolic-equivalence",
        "retail_sha256": "1" * 64,
        "candidate_sha256": "2" * 64,
        "summary": {
            "reads": ["r3"],
            "writes": ["r3"],
            "return_behavior": "normal",
        },
        "callees": callees or [],
        "helpers": [],
        "engine_hash": hash_engine_tree(repo),
        "certifier_hash": hash_certifier_tree(repo),
    }
    certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)
    return certificate


class MutationKillTests(unittest.TestCase):
    """P2-02 targeted mutants for certificate / promotion policy."""

    def test_accept_stale_transitive_certificate_is_killed(self) -> None:
        # Callee hash matches, but the callee certificate itself is stale
        # (rejected architecture). Correct validation recurses and rejects;
        # the mutant skips transitive checks and wrongly accepts.
        callee_cert = _registry_certificate("callee")
        callee_cert["architecture"] = "broadway-ppc32-be-v18"
        callee_cert["certificate_sha256"] = equivalence_certificate_hash(callee_cert)
        caller_cert = _registry_certificate("caller", [{
            "target_id": "callee",
            "certificate_sha256": callee_cert["certificate_sha256"],
        }])
        rows_by_id = {
            "callee": {"id": "callee", "equivalence_certificate": callee_cert},
            "caller": {"id": "caller", "equivalence_certificate": caller_cert},
        }

        base = equivalence_certificate_error(rows_by_id["caller"], rows_by_id)
        self.assertIsNotNone(base)
        self.assertIn("callee", base or "")
        self.assertIn("architecture", base or "")

        orig = targets_mod.equivalence_certificate_error

        def _skip_transitive(row, rows_by_id, *, _checking=None):
            if _checking is not None:
                return None  # mutant: trust callee once the hash matches
            return orig(row, rows_by_id, _checking=_checking)

        with mock.patch.object(
            targets_mod, "equivalence_certificate_error", new=_skip_transitive,
        ):
            mut = targets_mod.equivalence_certificate_error(
                rows_by_id["caller"], rows_by_id,
            )
        self.assertIsNone(mut, "mutant accepts a stale transitive certificate")
        self.assertIsNotNone(base, "correct policy rejects the stale transitive cert")

    def test_promotion_on_status_alone_is_killed(self) -> None:
        # EQUIVALENT status with a rejected architecture + unsafe masking.
        # Correct policy blocks; status-alone mutant wrongly allows.
        proof = _equivalent_proof(
            architecture_model="broadway-ppc32-be-v18",
            memory_scope=MemoryScope(masking_semantics="union-mask-legacy"),
        )
        policy = PromotionPolicy()
        ledger = _open_ledger()

        base = classify_for_promotion(proof, policy, ledger)
        self.assertFalse(base.allowed)
        self.assertTrue(
            any("architecture-model-" in item and "rejected" in item
                for item in base.blockers)
        )

        def _status_alone(result, _policy, _ledger):
            return PromotionDecision(
                allowed=result.status is ProofStatus.EQUIVALENT,
                confidence_tier="A",
                blockers=(),
                warnings=(),
            )

        mut = _status_alone(proof, policy, ledger)
        self.assertTrue(mut.allowed, "status-alone mutant wrongly promotes")
        self.assertFalse(base.allowed, "correct policy blocks unsafe promotion")


if __name__ == "__main__":
    unittest.main()

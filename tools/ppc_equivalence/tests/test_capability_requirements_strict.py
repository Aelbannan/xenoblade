"""Strict capability-requirements validation and trust-inversion fixes."""

from __future__ import annotations

import copy
import unittest
from types import SimpleNamespace

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_UNMODELED,
    CapabilityAssurance,
    CapabilityManifest,
    build_attestation,
    compute_confidence_tier_from_assurance,
    evaluate_capability_assurance,
)
from tools.ppc_equivalence.capability_requirements import (
    CAPABILITY_REQUIREMENTS_SCHEMA_VERSION,
    CapabilityRequirements,
    build_requirement,
    build_requirements,
    derive_capability_requirements,
    requirement_digest,
    requirements_digest,
    validate_capability_requirements_dict,
)
from tools.ppc_equivalence.fp_bitwise import FP_BITWISE_ALGORITHM, FP_BITWISE_MODEL_VERSION
from tools.ppc_equivalence.memory_profile import MemoryEnvironment, MemoryProfile
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    RESULT_FORMAT,
    FloatingPointDomain,
    ProofResult,
    ProofStatus,
)


def _ledger(*opcodes: str):
    from tools.coop.lib.equivalence_policy import ValidationLedger

    return ValidationLedger(frozenset(opcodes), intentionally_loaded=True)


def _authoritative_manifest(**overrides) -> CapabilityManifest:
    defaults = dict(
        allowed_tier_a_capabilities={
            "integer-core": ("integer-core-v1",),
            "provenance": ("provenance-v1",),
            "fp-bitwise": (FP_BITWISE_MODEL_VERSION,),
            "bounded-memory": ("bounded-memory-v2",),
            "certified-calls": ("certified-calls-v2",),
        },
        shadow_mode=False,
        require_capability_assurance=True,
    )
    defaults.update(overrides)
    return CapabilityManifest(**defaults)


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
    )
    defaults.update(kwargs)
    return ProofResult(**defaults)


def _touch_terminals() -> dict:
    addr = object()
    terminal = SimpleNamespace(
        state=SimpleNamespace(
            memory_touches=(addr,),
            memory_reads=(addr,),
            memory_writes=(),
            memory_effects=None,
        )
    )
    return {"original": [terminal], "candidate": [terminal]}


def _bind_evidence(capability: str, evidence: dict, requirements: CapabilityRequirements) -> dict:
    req = requirements.by_capability()[capability]
    bound = dict(evidence)
    bound["requirement_sha256"] = req.requirement_sha256
    bound["requirements_sha256"] = requirements.requirements_sha256
    return bound


def _register_only_assurance(proof: ProofResult) -> tuple[CapabilityRequirements, CapabilityAssurance]:
    requirements = derive_capability_requirements(proof)
    integer_req = requirements.by_capability()["integer-core"]
    assurance = CapabilityAssurance(
        capabilities=(
            build_attestation(
                capability="integer-core",
                model_version="integer-core-v1",
                algorithm="opcode-ledger-v2",
                evidence=_bind_evidence(
                    "integer-core",
                    {
                        "opcodes": list(integer_req.required_opcodes),
                        "ledger_sha256": "e" * 64,
                    },
                    requirements,
                ),
            ),
            build_attestation(
                capability="provenance",
                model_version="provenance-v1",
                algorithm="provenance-binding-v1",
                evidence=_bind_evidence(
                    "provenance",
                    {
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
                    requirements,
                ),
            ),
        ),
    )
    return requirements, assurance


def _omit_capability(
    requirements: CapabilityRequirements,
    capability: str,
) -> CapabilityRequirements:
    remaining = [
        item for item in requirements.requirements if item.capability != capability
    ]
    return build_requirements(remaining, schema_version=requirements.schema_version)


class StrictStructureTests(unittest.TestCase):
    def test_duplicate_capability_invalid(self) -> None:
        req = build_requirement(capability="integer-core", required_opcodes=["blr"])
        payload = {
            "schema_version": CAPABILITY_REQUIREMENTS_SCHEMA_VERSION,
            "requirements": [req.to_dict(), req.to_dict()],
            "requirements_sha256": "0" * 64,
        }
        with self.assertRaises(ValueError):
            validate_capability_requirements_dict(payload)

    def test_unknown_capability_and_schema_invalid(self) -> None:
        with self.assertRaises(ValueError):
            validate_capability_requirements_dict(
                {
                    "schema_version": 99,
                    "requirements": [],
                    "requirements_sha256": "0" * 64,
                }
            )
        req = build_requirement(capability="integer-core", required_opcodes=["blr"])
        payload = req.to_dict()
        payload["capability"] = "not-a-real-capability"
        payload["requirement_sha256"] = requirement_digest(
            {k: v for k, v in payload.items() if k != "requirement_sha256"}
        )
        block = {
            "schema_version": CAPABILITY_REQUIREMENTS_SCHEMA_VERSION,
            "requirements": [payload],
            "requirements_sha256": "0" * 64,
        }
        with self.assertRaises(ValueError):
            validate_capability_requirements_dict(block)

    def test_mutate_requirement_contents_without_hash_invalid(self) -> None:
        proof = _equivalent()
        requirements = derive_capability_requirements(proof)
        payload = requirements.to_dict()
        payload["requirements"][0]["required_opcodes"] = ["addi"]
        with self.assertRaises(ValueError):
            validate_capability_requirements_dict(payload)

    def test_update_individual_hash_but_not_aggregate_invalid(self) -> None:
        proof = _equivalent()
        requirements = derive_capability_requirements(proof)
        payload = requirements.to_dict()
        entry = payload["requirements"][0]
        entry["required_opcodes"] = ["addi"]
        entry["requirement_sha256"] = requirement_digest(
            {k: v for k, v in entry.items() if k != "requirement_sha256"}
        )
        with self.assertRaises(ValueError):
            validate_capability_requirements_dict(payload)


class TrustInversionTests(unittest.TestCase):
    def test_addi_fmr_omit_fp_bitwise_incomplete(self) -> None:
        proof = _equivalent(
            opcodes_used=["addi", "fmr", "blr"],
            floating_point_domain=FloatingPointDomain(),
            observables=["r3", "f1"],
        )
        full = derive_capability_requirements(proof)
        omitted = _omit_capability(full, "fp-bitwise")
        integer_req = omitted.by_capability()["integer-core"]
        assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence=_bind_evidence(
                        "integer-core",
                        {
                            "opcodes": list(integer_req.required_opcodes),
                            "ledger_sha256": "e" * 64,
                        },
                        omitted,
                    ),
                ),
                build_attestation(
                    capability="provenance",
                    model_version="provenance-v1",
                    algorithm="provenance-binding-v1",
                    evidence=_bind_evidence(
                        "provenance",
                        {
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
                        omitted,
                    ),
                ),
            ),
        )
        proof.capability_requirements = omitted.to_dict()
        proof.capability_assurance = assurance.to_dict()
        result = evaluate_capability_assurance(
            proof,
            _ledger("addi", "fmr", "blr"),
            _authoritative_manifest(),
        )
        self.assertFalse(result.all_used_capabilities_promotion_grade)
        self.assertTrue(result.has_unmodeled_or_unproven_capability)
        self.assertIn("capability-requirements-incomplete:fp-bitwise", result.blockers)
        self.assertIn("capability-attestation-missing:fp-bitwise", result.blockers)
        self.assertEqual(
            result.recomputed_statuses.get("fp-bitwise"),
            STATUS_UNMODELED,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_fp_memory_omit_bounded_memory_incomplete(self) -> None:
        proof = _equivalent(
            opcodes_used=["lwz", "fmr", "blr"],
            floating_point_domain=FloatingPointDomain(),
            environment=MemoryEnvironment(
                profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
                ranges=[(0x80000000, 0x817FFFFF)],
            ),
            observables=["r3", "f1"],
        )
        full = derive_capability_requirements(proof, terminals_meta=_touch_terminals())
        self.assertIn("bounded-memory", full.by_capability())
        omitted = _omit_capability(full, "bounded-memory")
        # Keep remaining attestations promotion-shaped for non-memory caps.
        caps = []
        for capability, model, algorithm, evidence in (
            (
                "integer-core",
                "integer-core-v1",
                "opcode-ledger-v2",
                {
                    "opcodes": list(omitted.by_capability()["integer-core"].required_opcodes),
                    "ledger_sha256": "e" * 64,
                },
            ),
            (
                "fp-bitwise",
                FP_BITWISE_MODEL_VERSION,
                FP_BITWISE_ALGORITHM,
                {
                    "opcodes": list(omitted.by_capability()["fp-bitwise"].required_opcodes),
                    "host_float": False,
                    "ledger_sha256": "e" * 64,
                },
            ),
            (
                "provenance",
                "provenance-v1",
                "provenance-binding-v1",
                {
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
        ):
            if capability not in omitted.by_capability():
                continue
            caps.append(
                build_attestation(
                    capability=capability,
                    model_version=model,
                    algorithm=algorithm,
                    evidence=_bind_evidence(capability, evidence, omitted),
                )
            )
        proof.capability_requirements = omitted.to_dict()
        proof.capability_assurance = CapabilityAssurance(capabilities=tuple(caps)).to_dict()
        result = evaluate_capability_assurance(
            proof,
            _ledger("lwz", "fmr", "blr"),
            _authoritative_manifest(),
        )
        self.assertFalse(result.all_used_capabilities_promotion_grade)
        self.assertIn(
            "capability-requirements-incomplete:bounded-memory",
            result.blockers,
        )

    def test_call_omit_certified_calls_incomplete(self) -> None:
        proof = _equivalent(
            opcodes_used=["bl", "blr"],
            assumed_callees=["0x80001234"],
        )
        full = derive_capability_requirements(proof)
        self.assertIn("certified-calls", full.by_capability())
        omitted = _omit_capability(full, "certified-calls")
        requirements, assurance = _register_only_assurance(proof)
        # Rebuild assurance against the omitted block digests.
        del requirements
        caps = []
        for item in assurance.capabilities:
            caps.append(
                build_attestation(
                    capability=item.capability,
                    model_version=item.model_version,
                    algorithm=item.algorithm,
                    evidence=_bind_evidence(
                        item.capability,
                        {
                            key: value
                            for key, value in item.evidence.items()
                            if key
                            not in {"requirement_sha256", "requirements_sha256"}
                        },
                        omitted,
                    ),
                )
            )
        proof.capability_requirements = omitted.to_dict()
        proof.capability_assurance = CapabilityAssurance(capabilities=tuple(caps)).to_dict()
        result = evaluate_capability_assurance(
            proof,
            _ledger("bl", "blr"),
            _authoritative_manifest(),
        )
        self.assertFalse(result.all_used_capabilities_promotion_grade)
        self.assertIn(
            "capability-requirements-incomplete:certified-calls",
            result.blockers,
        )

    def test_recompute_hashes_after_deleting_capability_still_incomplete(self) -> None:
        proof = _equivalent(
            opcodes_used=["addi", "fmr", "blr"],
            floating_point_domain=FloatingPointDomain(),
            observables=["r3", "f1"],
        )
        full = derive_capability_requirements(proof)
        omitted = _omit_capability(full, "fp-bitwise")
        # Hashes are valid for the reduced set — semantic incompleteness remains.
        omitted.validate_structure()
        validate_capability_requirements_dict(omitted.to_dict())
        proof.capability_requirements = omitted.to_dict()
        _, assurance = _register_only_assurance(proof)
        # Re-bind to omitted digests.
        caps = []
        for item in assurance.capabilities:
            if item.capability not in omitted.by_capability():
                continue
            caps.append(
                build_attestation(
                    capability=item.capability,
                    model_version=item.model_version,
                    algorithm=item.algorithm,
                    evidence=_bind_evidence(
                        item.capability,
                        {
                            key: value
                            for key, value in item.evidence.items()
                            if key
                            not in {"requirement_sha256", "requirements_sha256"}
                        },
                        omitted,
                    ),
                )
            )
        proof.capability_assurance = CapabilityAssurance(capabilities=tuple(caps)).to_dict()
        result = evaluate_capability_assurance(
            proof,
            _ledger("addi", "fmr", "blr"),
            _authoritative_manifest(),
        )
        self.assertFalse(result.all_used_capabilities_promotion_grade)
        self.assertIn("capability-requirements-incomplete:fp-bitwise", result.blockers)

    def test_remove_entire_block_authoritative_incomplete(self) -> None:
        proof = _equivalent()
        requirements, assurance = _register_only_assurance(proof)
        proof.capability_assurance = assurance.to_dict()
        proof.capability_requirements = None
        result = evaluate_capability_assurance(
            proof,
            _ledger("addi", "blr"),
            _authoritative_manifest(),
        )
        self.assertIn("capability-requirements-missing", result.blockers)
        self.assertFalse(result.all_used_capabilities_promotion_grade)
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")
        del requirements

    def test_valid_register_only_still_tier_a(self) -> None:
        proof = _equivalent()
        requirements, assurance = _register_only_assurance(proof)
        proof.capability_requirements = requirements.to_dict()
        proof.capability_assurance = assurance.to_dict()
        result = evaluate_capability_assurance(
            proof,
            _ledger("addi", "blr"),
            _authoritative_manifest(),
        )
        self.assertTrue(result.all_used_capabilities_promotion_grade)
        self.assertFalse(result.has_unmodeled_or_unproven_capability)
        self.assertEqual(result.blockers, ())
        self.assertEqual(compute_confidence_tier_from_assurance(result), "A")

    def test_used_set_never_trusts_requirements_block(self) -> None:
        """Regression: omitting a required capability must not shrink ``used``."""
        proof = _equivalent(
            opcodes_used=["addi", "fmr", "blr"],
            floating_point_domain=FloatingPointDomain(),
            observables=["r3", "f1"],
        )
        full = derive_capability_requirements(proof)
        omitted = _omit_capability(full, "fp-bitwise")
        proof.capability_requirements = omitted.to_dict()
        proof.capability_assurance = CapabilityAssurance(capabilities=()).to_dict()
        result = evaluate_capability_assurance(
            proof,
            _ledger("addi", "fmr", "blr"),
            _authoritative_manifest(),
        )
        self.assertIn("fp-bitwise", result.recomputed_statuses)
        self.assertNotEqual(set(omitted.by_capability()), set(result.recomputed_statuses))


class AggregateDigestTests(unittest.TestCase):
    def test_requirements_digest_matches_block(self) -> None:
        proof = _equivalent()
        block = derive_capability_requirements(proof)
        self.assertEqual(block.requirements_sha256, requirements_digest(block))
        clone = CapabilityRequirements.from_dict(copy.deepcopy(block.to_dict()))
        clone.validate_structure()


if __name__ == "__main__":
    unittest.main()

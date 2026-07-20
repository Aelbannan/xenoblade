"""Stage 1 capability-requirements derivation and Stage 2 binding."""

from __future__ import annotations

import unittest
from types import SimpleNamespace

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    CapabilityAssurance,
    CapabilityManifest,
    build_attestation,
    evaluate_capability_assurance,
)
from tools.ppc_equivalence.capability_requirements import (
    CapabilityRequirement,
    CapabilityRequirements,
    build_requirement,
    derive_capability_requirements,
    requirement_digest,
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


def _manifest(**overrides) -> CapabilityManifest:
    defaults = dict(
        allowed_tier_a_capabilities={
            "integer-core": ("integer-core-v1",),
            "provenance": ("provenance-v1",),
            "fp-bitwise": (FP_BITWISE_MODEL_VERSION,),
            "bounded-memory": ("bounded-memory-v2",),
        },
        shadow_mode=True,
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
        opcodes_used=["blr"],
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


def _provenance_attestation() -> object:
    return build_attestation(
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
    )


class DeriveRequirementsTests(unittest.TestCase):
    def test_lwz_fmr_under_bounded_profile_requires_memory_and_fp(self) -> None:
        proof = _equivalent(
            opcodes_used=["lwz", "fmr", "blr"],
            floating_point_domain=FloatingPointDomain(),
            environment=MemoryEnvironment(
                profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
                ranges=[(0x80000000, 0x817FFFFF)],
            ),
            observables=["r3", "f1"],
        )
        reqs = derive_capability_requirements(
            proof,
            terminals_meta=_touch_terminals(),
        )
        names = {item.capability for item in reqs.requirements}
        self.assertIn("bounded-memory", names)
        self.assertIn("fp-bitwise", names)
        self.assertIn("integer-core", names)
        self.assertIn("provenance", names)
        fp = next(item for item in reqs.requirements if item.capability == "fp-bitwise")
        self.assertEqual(fp.required_opcodes, ("fmr",))

    def test_fp_only_bitwise_without_touches_skips_bounded_memory(self) -> None:
        proof = _equivalent(
            opcodes_used=["fmr", "fabs", "blr"],
            floating_point_domain=FloatingPointDomain(),
            observables=["f1"],
        )
        reqs = derive_capability_requirements(proof, terminals_meta=None)
        names = {item.capability for item in reqs.requirements}
        self.assertIn("fp-bitwise", names)
        self.assertNotIn("bounded-memory", names)
        self.assertNotIn("assumed-ordinary-ram", names)

    def test_memory_observable_untouched_does_not_invent_memory_req(self) -> None:
        proof = _equivalent(
            opcodes_used=["addi", "blr"],
            observables=["r3", "memory"],
            environment=MemoryEnvironment(
                profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
                ranges=[(0x80000000, 0x817FFFFF)],
            ),
        )
        reqs = derive_capability_requirements(proof, terminals_meta=None)
        names = {item.capability for item in reqs.requirements}
        self.assertNotIn("bounded-memory", names)
        self.assertNotIn("assumed-ordinary-ram", names)

    def test_load_opcode_without_terminals_implies_memory_req(self) -> None:
        """FULL_MATCH synthesis has opcodes but no CFG terminals."""
        proof = _equivalent(
            opcodes_used=["lhz", "addi", "blr"],
            observables=["r3"],
        )
        reqs = derive_capability_requirements(proof, terminals_meta=None)
        names = {item.capability for item in reqs.requirements}
        self.assertIn("assumed-ordinary-ram", names)
        self.assertNotIn("bounded-memory", names)


class RequirementBindingTests(unittest.TestCase):
    def test_partial_opcode_attestation_is_incomplete(self) -> None:
        proof = _equivalent(
            opcodes_used=["fmr", "fneg", "blr"],
            floating_point_domain=FloatingPointDomain(),
            observables=["f1"],
        )
        requirements = derive_capability_requirements(proof)
        fp_req = next(
            item for item in requirements.requirements if item.capability == "fp-bitwise"
        )
        self.assertEqual(set(fp_req.required_opcodes), {"fmr", "fneg"})

        integer_req = next(
            item
            for item in requirements.requirements
            if item.capability == "integer-core"
        )
        assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence={
                        "opcodes": list(integer_req.required_opcodes),
                        "ledger_sha256": "e" * 64,
                        "requirement_sha256": integer_req.requirement_sha256,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
                build_attestation(
                    capability="fp-bitwise",
                    model_version=FP_BITWISE_MODEL_VERSION,
                    algorithm=FP_BITWISE_ALGORITHM,
                    evidence={
                        "opcodes": ["fmr"],  # missing fneg
                        "host_float": False,
                        "ledger_sha256": "e" * 64,
                        "requirement_sha256": fp_req.requirement_sha256,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
                _provenance_attestation(),
            ),
        )
        proof.capability_requirements = requirements.to_dict()
        proof.capability_assurance = assurance.to_dict()
        result = evaluate_capability_assurance(
            proof,
            _ledger("fmr", "fneg", "blr"),
            _manifest(),
        )
        self.assertEqual(
            result.recomputed_statuses["fp-bitwise"],
            STATUS_INCOMPLETE,
        )

    def test_requirement_hash_mutation_is_incomplete(self) -> None:
        proof = _equivalent(opcodes_used=["addi", "blr"])
        requirements = derive_capability_requirements(proof)
        integer_req = next(
            item
            for item in requirements.requirements
            if item.capability == "integer-core"
        )
        mutated = "0" * 64
        self.assertNotEqual(mutated, integer_req.requirement_sha256)
        assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence={
                        "opcodes": list(integer_req.required_opcodes),
                        "ledger_sha256": "e" * 64,
                        "requirement_sha256": mutated,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
                _provenance_attestation(),
            ),
        )
        proof.capability_requirements = requirements.to_dict()
        proof.capability_assurance = assurance.to_dict()
        result = evaluate_capability_assurance(
            proof,
            _ledger("addi", "blr"),
            _manifest(),
        )
        self.assertEqual(
            result.recomputed_statuses["integer-core"],
            STATUS_INCOMPLETE,
        )

    def test_exact_requirement_binding_can_be_promotion_grade(self) -> None:
        proof = _equivalent(opcodes_used=["addi", "blr"])
        requirements = derive_capability_requirements(proof)
        integer_req = next(
            item
            for item in requirements.requirements
            if item.capability == "integer-core"
        )
        prov_req = next(
            item for item in requirements.requirements if item.capability == "provenance"
        )
        assurance = CapabilityAssurance(
            capabilities=(
                build_attestation(
                    capability="integer-core",
                    model_version="integer-core-v1",
                    algorithm="opcode-ledger-v2",
                    evidence={
                        "opcodes": list(integer_req.required_opcodes),
                        "ledger_sha256": "e" * 64,
                        "requirement_sha256": integer_req.requirement_sha256,
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
                        "engine_hash": "a" * 64,
                        "certifier_hash": "d" * 64,
                        "source_hash": "b" * 64,
                        "proof_request_hash": "b" * 64,
                        "validation_ledger_hash": "e" * 64,
                        "git_commit": "c" * 40,
                        "git_dirty": False,
                        "requirement_sha256": prov_req.requirement_sha256,
                        "requirements_sha256": requirements.requirements_sha256,
                    },
                ),
            ),
        )
        proof.capability_requirements = requirements.to_dict()
        proof.capability_assurance = assurance.to_dict()
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

    def test_requirement_digest_stable(self) -> None:
        req = build_requirement(
            capability="fp-bitwise",
            required_opcodes=["fmr", "fabs"],
        )
        self.assertEqual(req.requirement_sha256, requirement_digest(req))
        restored = CapabilityRequirement.from_dict(req.to_dict())
        self.assertEqual(restored.requirement_sha256, req.requirement_sha256)
        block = CapabilityRequirements.from_dict(
            derive_capability_requirements(
                _equivalent(opcodes_used=["addi", "blr"])
            ).to_dict()
        )
        self.assertTrue(block.requirements_sha256)


if __name__ == "__main__":
    unittest.main()

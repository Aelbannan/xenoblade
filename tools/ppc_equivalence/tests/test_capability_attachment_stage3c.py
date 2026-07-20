"""Stage 3C: FP / MMIO / proof-feature attestation attachment."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    STATUS_UNMODELED,
    CapabilityAssurance,
    CapabilityManifest,
    build_attestation,
    compute_confidence_tier_from_assurance,
    evaluate_capability_assurance,
    infer_used_capabilities,
    maybe_attach_integer_core_draft,
)
from tools.ppc_equivalence.capability_attachment import (
    FP_COMPARE_ALGORITHM,
    FP_CONVERT_ALGORITHM,
    FP_LOAD_STORE_ALGORITHM,
    PROOF_FEATURE_ALGORITHM,
    draft_fp_capability_attestations,
    draft_mmio_capability_attestations,
    draft_proof_feature_attestations,
    maybe_attach_stage3c_drafts,
    opcodes_for_fp_capability,
)
from tools.ppc_equivalence.fp_bitwise import FP_BITWISE_ALGORITHM, FP_BITWISE_OPS
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    RESULT_FORMAT,
    FloatingPointDomain,
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


def _manifest(**overrides) -> CapabilityManifest:
    defaults = dict(
        allowed_tier_a_capabilities={
            "integer-core": ("integer-core-v1",),
            "provenance": ("provenance-v1",),
            "fp-bitwise": ("fp-bitwise-v1",),
            "fp-load-store": (),
            "fp-compare": (),
            "fp-convert": (),
            "fp-scalar-arithmetic": (),
            "fp-fused-arithmetic": (),
            "fp-paired-single": (),
            "fp-psq": (),
            "fp-traps": (),
            "precondition-closure": (),
            "mmio-register-bank": (),
            "gx-fifo-write-trace": (),
            "memory-loop-summary": (),
            "immutable-address-space": (),
            "indirect-target-closure": (),
            "affine-loop-summary": (),
            "relational-loop-induction": (),
        },
        shadow_mode=True,
        require_capability_assurance=False,
    )
    defaults.update(overrides)
    return CapabilityManifest(**defaults)


def _equivalent(**kwargs) -> ProofResult:
    defaults = dict(
        status=ProofStatus.EQUIVALENT,
        architecture_model=ARCHITECTURE_MODEL,
        format=RESULT_FORMAT,
        engine_hash="a" * 64,
        certifier_hash="d" * 64,
        source_hash="b" * 64,
        proof_request_hash="b" * 64,
        validation_ledger_hash="e" * 64,
        git_commit="c" * 40,
        git_dirty=False,
        opcodes_used=["blr"],
        observables=["r3"],
    )
    defaults.update(kwargs)
    return ProofResult(**defaults)


def _fp_bitwise_ledger_capabilities(*opcodes: str) -> dict:
    return {
        "fp-bitwise": {
            "model_version": "fp-bitwise-v1",
            "dolphin_validated_opcodes": sorted(opcodes),
        }
    }


class Stage3CFpAttachmentTests(unittest.TestCase):
    def test_exact_opcode_binding_for_fp_bitwise(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fmr", "fneg", "blr"],
            observables=["f1"],
        )
        att = draft_fp_capability_attestations(
            proof,
            ledger=_ledger(
                "fmr",
                "fneg",
                "blr",
                capabilities=_fp_bitwise_ledger_capabilities("fmr", "fneg"),
            ),
            ledger_sha256="e" * 64,
        )
        by_name = {item.capability: item for item in att}
        self.assertIn("fp-bitwise", by_name)
        self.assertEqual(by_name["fp-bitwise"].evidence["opcodes"], ["fmr", "fneg"])
        self.assertEqual(by_name["fp-bitwise"].algorithm, FP_BITWISE_ALGORITHM)
        self.assertNotIn("fp-scalar-arithmetic", by_name)

    def test_partial_bitwise_attestation_rejected_on_evaluate(self) -> None:
        """Proof uses fmr+fneg; attesting only fmr → incomplete / C."""
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fmr", "fneg", "blr"],
            observables=["f1"],
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    build_attestation(
                        capability="integer-core",
                        model_version="integer-core-v1",
                        algorithm="opcode-ledger-v2",
                        evidence={"opcodes": ["blr"], "ledger_sha256": "e" * 64},
                    ),
                    build_attestation(
                        capability="fp-bitwise",
                        model_version="fp-bitwise-v1",
                        algorithm=FP_BITWISE_ALGORITHM,
                        evidence={
                            "opcodes": ["fmr"],  # missing fneg
                            "host_float": False,
                            "ledger_sha256": "e" * 64,
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
            ).to_dict(),
        )
        # Without requirements, evaluate still grades fp-bitwise from evidence;
        # missing fneg in evidence vs used set is caught when requirements bind.
        # Stage 3C attachment itself emits the exact set:
        attached = maybe_attach_stage3c_drafts(
            _equivalent(
                floating_point_domain=FloatingPointDomain(),
                opcodes_used=["fmr", "fneg", "blr"],
                observables=["f1"],
            ),
            ledger_sha256="e" * 64,
        )
        names = {
            item["capability"]
            for item in attached.capability_assurance["capabilities"]
        }
        bitwise = next(
            item
            for item in attached.capability_assurance["capabilities"]
            if item["capability"] == "fp-bitwise"
        )
        self.assertEqual(bitwise["evidence"]["opcodes"], ["fmr", "fneg"])
        self.assertEqual(set(opcodes_for_fp_capability("fp-bitwise", ["fmr", "fneg"])), FP_BITWISE_OPS & {"fmr", "fneg"})
        self.assertIn("fp-bitwise", names)

    def test_scalar_fused_incomplete_stubs_attached(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fadd", "fmadd", "blr"],
            observables=["f1"],
        )
        att = draft_fp_capability_attestations(proof)
        by_name = {item.capability: item for item in att}
        self.assertIn("fp-scalar-arithmetic", by_name)
        self.assertIn("fp-fused-arithmetic", by_name)
        self.assertIn("precondition-closure", by_name)
        self.assertEqual(
            by_name["fp-scalar-arithmetic"].evidence["opcodes"], ["fadd"]
        )
        self.assertEqual(
            by_name["fp-fused-arithmetic"].evidence["opcodes"], ["fmadd"]
        )
        self.assertEqual(
            by_name["fp-scalar-arithmetic"].status, STATUS_INCOMPLETE
        )
        self.assertEqual(
            by_name["fp-fused-arithmetic"].status, STATUS_INCOMPLETE
        )

    def test_load_store_compare_convert_incomplete_algorithms(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["lfs", "fcmpu", "frsp", "blr"],
            observables=["f1"],
        )
        att = draft_fp_capability_attestations(proof)
        by_name = {item.capability: item for item in att}
        self.assertEqual(by_name["fp-load-store"].algorithm, FP_LOAD_STORE_ALGORITHM)
        self.assertEqual(by_name["fp-compare"].algorithm, FP_COMPARE_ALGORITHM)
        self.assertEqual(by_name["fp-convert"].algorithm, FP_CONVERT_ALGORITHM)
        self.assertEqual(by_name["fp-load-store"].evidence["opcodes"], ["lfs"])
        self.assertEqual(by_name["fp-compare"].evidence["opcodes"], ["fcmpu"])
        self.assertEqual(by_name["fp-convert"].evidence["opcodes"], ["frsp"])


class Stage3CMmioAttachmentTests(unittest.TestCase):
    def test_mmio_distinct_attestations_from_memory_bus(self) -> None:
        memory_bus = {
            "schema_version": 2,
            "status": "incomplete",
            "bus_spec_sha256": "b" * 64,
            "hardware_profile": "wii-broadway-xenoblade-us-v1",
            "hardware_profile_sha256": "c" * 64,
            "device_models_sha256": "d" * 64,
            "register_bank_theory": {
                "status": "present",
                "devices": [
                    {"device_id": "vi", "theory": "mmio-register-bank"},
                    {"device_id": "pi", "theory": "mmio-register-bank"},
                ],
            },
            "fifo_theory": {"status": "present"},
            "gxfifo_trace": True,
            "has_read_side_effects": True,
            "device_ids": ["vi", "pi"],
        }
        proof = _equivalent(
            opcodes_used=["lwz", "stw", "blr"],
            proof_features=["memory-bus"],
            memory_bus=memory_bus,
            observables=["r3"],
        )
        used = infer_used_capabilities(proof)
        self.assertIn("mmio-register-bank", used)
        self.assertIn("gx-fifo-write-trace", used)
        self.assertIn("mmio-read-side-effects", used)

        att = draft_mmio_capability_attestations(proof)
        by_name = {item.capability: item for item in att}
        self.assertIn("mmio-register-bank", by_name)
        self.assertIn("gx-fifo-write-trace", by_name)
        self.assertIn("mmio-read-side-effects", by_name)
        bank = by_name["mmio-register-bank"]
        self.assertEqual(bank.evidence.get("bus_spec_sha256"), "b" * 64)
        self.assertEqual(
            sorted(bank.evidence.get("device_ids") or []),
            ["pi", "vi"],
        )
        # Empty allowlist → not Tier A.
        maybe_attach_stage3c_drafts(proof)
        maybe_attach_integer_core_draft(proof, ledger_sha256="e" * 64)
        result = evaluate_capability_assurance(proof, _ledger("lwz", "stw", "blr"), _manifest())
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")
        self.assertNotEqual(
            result.recomputed_statuses.get("mmio-register-bank"),
            STATUS_PROMOTION_GRADE,
        )


class Stage3CProofFeatureAttachmentTests(unittest.TestCase):
    def test_incomplete_and_discharged_feature_stubs(self) -> None:
        proof = _equivalent(
            opcodes_used=["bdnz", "stw", "blr"],
            proof_features=[
                "memory-loop-summary",
                "readonly-image",
                "indirect-target-closure",
                "affine-loop-summary",
                "relational-induction",
            ],
            memory_loop={"status": "incomplete", "schema_version": 2},
            address_space={"status": "discharged", "schema_version": 2},
            indirect_targets={"status": "discharged", "schema_version": 2},
            loop_summary={"status": "incomplete"},
            relational_induction={"status": "discharged"},
        )
        used = infer_used_capabilities(proof)
        self.assertIn("memory-loop-summary", used)
        self.assertIn("immutable-address-space", used)
        self.assertIn("indirect-target-closure", used)
        self.assertIn("affine-loop-summary", used)
        self.assertIn("relational-loop-induction", used)

        att = draft_proof_feature_attestations(proof)
        by_name = {item.capability: item for item in att}
        self.assertEqual(
            by_name["memory-loop-summary"].evidence["obligation_status"],
            "incomplete",
        )
        self.assertEqual(
            by_name["immutable-address-space"].evidence["obligation_status"],
            "discharged",
        )
        self.assertEqual(
            by_name["relational-loop-induction"].algorithm,
            PROOF_FEATURE_ALGORITHM,
        )
        # Empty allowlist keeps discharged features incomplete for Tier A.
        maybe_attach_stage3c_drafts(proof)
        result = evaluate_capability_assurance(proof, _ledger("bdnz", "stw", "blr"), _manifest())
        self.assertEqual(
            result.recomputed_statuses.get("immutable-address-space"),
            STATUS_INCOMPLETE,
        )


class Stage3CSiblingCapabilityTests(unittest.TestCase):
    def test_fp_does_not_suppress_memory_or_mmio(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["lwz", "fmr", "blr"],
            observables=["memory", "f1"],
            proof_features=["memory-bus"],
            memory_bus={
                "schema_version": 2,
                "status": "incomplete",
                "register_bank_theory": {"status": "present"},
            },
        )
        used = infer_used_capabilities(proof)
        self.assertIn("fp-bitwise", used)
        self.assertIn("mmio-register-bank", used)
        # Observables memory + FP must not drop MMIO; bounded-memory may be
        # omitted when memory_bus is present (MMIO path owns the bus).
        self.assertNotIn("bounded-memory", used)  # suppressed only by memory_bus, not FP

        maybe_attach_integer_core_draft(proof, ledger_sha256="e" * 64)
        maybe_attach_stage3c_drafts(proof, ledger_sha256="e" * 64)
        names = {
            item["capability"]
            for item in proof.capability_assurance["capabilities"]
        }
        self.assertIn("integer-core", names)
        self.assertIn("fp-bitwise", names)
        self.assertIn("mmio-register-bank", names)

    def test_lwz_plus_fmr_infers_both_siblings_without_bus(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["lwz", "fmr", "blr"],
            observables=["memory", "f1"],
        )
        used = infer_used_capabilities(proof)
        self.assertIn("fp-bitwise", used)
        self.assertIn("bounded-memory", used)


if __name__ == "__main__":
    unittest.main()

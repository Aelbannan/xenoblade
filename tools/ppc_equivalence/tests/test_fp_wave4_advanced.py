"""Wave 4 advanced FP / MMIO capability-assurance scaffolding."""

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
    load_capability_manifest,
)
from tools.ppc_equivalence.fp_advanced_obligations import (
    FP_FUSED_ALGORITHM,
    FP_FUSED_CAPABILITY,
    FP_PAIRED_CAPABILITY,
    FP_PSQ_CAPABILITY,
    FP_TRAPS_ALGORITHM,
    FP_TRAPS_CAPABILITY,
    FUSED_INCOMPLETE_DIMENSIONS,
    TRAPS_INCOMPLETE_DIMENSIONS,
    build_fp_advanced_attestation,
    build_fp_advanced_obligation,
    evaluate_fp_advanced_obligation_status,
    recompute_fp_advanced_attestation_status,
    validate_fp_advanced_obligation,
)
from tools.ppc_equivalence.fp_outcome import capability_tags_for_opcodes
from tools.ppc_equivalence.fp_traps import (
    capability_tags_for_trap_domain,
    fe0_fe1_modeling_status,
)
from tools.ppc_equivalence.mmio_capability_obligations import (
    DMA_INTERRUPT_ALGORITHM,
    DMA_INTERRUPT_CAPABILITY,
    DMA_INTERRUPT_MODEL_VERSION,
    GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
    GX_FIFO_READ_CAPABILITY,
    GX_FIFO_TRACE_MODEL_VERSION,
    GX_FIFO_WRITE_TRACE_CAPABILITY,
    MMIO_EXTERNAL_INPUT_CAPABILITY,
    MMIO_READ_SIDE_EFFECTS_CAPABILITY,
    ALWAYS_INCOMPLETE_MMIO_CAPABILITIES,
    build_always_incomplete_mmio_attestation,
    build_gx_fifo_loop_refinement_attestation,
    build_gx_fifo_loop_refinement_obligation,
    loop_refinement_has_real_unsat,
    recompute_mmio_attestation_status,
    validate_gx_fifo_loop_refinement_obligation,
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


def _ledger(*opcodes: str, capabilities: dict | None = None):
    from tools.coop.lib.equivalence_policy import ValidationLedger

    return ValidationLedger(
        frozenset(opcodes),
        intentionally_loaded=True,
        capabilities=dict(capabilities or {}),
    )


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
            "fp-bitwise": ("fp-bitwise-v1",),
            "fp-fused-arithmetic": (),
            "fp-paired-single": (),
            "fp-psq": (),
            "fp-traps": (),
            "gx-fifo-write-trace": (),
            "gx-fifo-read": (),
            "mmio-read-side-effects": (),
            "mmio-external-input": (),
            "dma-interrupt-effects": (),
        },
        shadow_mode=True,
    )
    defaults.update(kwargs)
    return CapabilityManifest(**defaults)


def _provenance_attestation():
    return build_attestation(
        capability="provenance",
        model_version="provenance-v1",
        algorithm="provenance-binding-v1",
        status=STATUS_INCOMPLETE,
        evidence={
            "engine_hash": "a" * 64,
            "certifier_hash": "d" * 64,
            "source_hash": "b" * 64,
            "git_commit": "c" * 40,
            "git_dirty": False,
        },
    )


def _integer_attestation(*opcodes: str):
    return build_attestation(
        capability="integer-core",
        model_version="integer-core-v1",
        algorithm="opcode-ledger-v2",
        status=STATUS_INCOMPLETE,
        evidence={"opcodes": list(opcodes), "ledger_sha256": "e" * 64},
    )


class FpAdvancedDemandTests(unittest.TestCase):
    def test_fused_paired_psq_demand_caps_not_promotion(self) -> None:
        cases = (
            (["fmadd"], FP_FUSED_CAPABILITY),
            (["ps_add"], FP_PAIRED_CAPABILITY),
            (["psq_l"], FP_PSQ_CAPABILITY),
        )
        for opcodes, capability in cases:
            with self.subTest(capability=capability):
                attestation = build_fp_advanced_attestation(
                    capability, opcodes=opcodes
                )
                # Forge promotion-grade — evaluator must reject.
                forged = build_attestation(
                    capability=attestation.capability,
                    model_version=attestation.model_version,
                    algorithm=attestation.algorithm,
                    status=STATUS_PROMOTION_GRADE,
                    evidence=dict(attestation.evidence),
                    unsupported=(),
                )
                proof = _equivalent(
                    opcodes_used=opcodes + ["blr"],
                    floating_point_domain=FloatingPointDomain(),
                    capability_assurance=CapabilityAssurance(
                        capabilities=(
                            _integer_attestation("blr"),
                            forged,
                            _provenance_attestation(),
                        ),
                    ).to_dict(),
                )
                used = infer_used_capabilities(proof)
                self.assertIn(capability, used)
                result = evaluate_capability_assurance(
                    proof, _ledger(*opcodes, "blr"), _manifest()
                )
                self.assertEqual(
                    result.recomputed_statuses.get(capability),
                    STATUS_INCOMPLETE,
                )
                self.assertEqual(
                    compute_confidence_tier_from_assurance(result), "C"
                )

    def test_traps_enabled_demands_fp_traps(self) -> None:
        attestation = build_fp_advanced_attestation(FP_TRAPS_CAPABILITY)
        forged = build_attestation(
            capability=attestation.capability,
            model_version=attestation.model_version,
            algorithm=attestation.algorithm,
            status=STATUS_PROMOTION_GRADE,
            evidence=dict(attestation.evidence),
            unsupported=(),
        )
        proof = _equivalent(
            opcodes_used=["fadd", "blr"],
            floating_point_domain=FloatingPointDomain(traps_enabled=True),
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("blr"),
                    forged,
                    _provenance_attestation(),
                ),
            ).to_dict(),
        )
        used = infer_used_capabilities(proof)
        self.assertIn(FP_TRAPS_CAPABILITY, used)
        self.assertEqual(
            capability_tags_for_trap_domain(traps_enabled=True),
            frozenset({"fp-traps"}),
        )
        result = evaluate_capability_assurance(
            proof, _ledger("fadd", "blr"), _manifest()
        )
        self.assertEqual(
            result.recomputed_statuses.get(FP_TRAPS_CAPABILITY),
            STATUS_INCOMPLETE,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_outcome_tagging_hooks(self) -> None:
        self.assertEqual(
            capability_tags_for_opcodes(["fmadd", "ps_mul", "psq_st"]),
            frozenset(
                {
                    "fp-fused-arithmetic",
                    "fp-paired-single",
                    "fp-psq",
                }
            ),
        )
        self.assertEqual(capability_tags_for_opcodes(["fadd"]), frozenset())

    def test_fe0_fe1_and_fused_dimensions_incomplete(self) -> None:
        status = fe0_fe1_modeling_status()
        self.assertFalse(status["fe0"])
        self.assertFalse(status["fe1"])
        self.assertFalse(status["imprecise_modes_modeled"])
        self.assertFalse(FUSED_INCOMPLETE_DIMENSIONS["midpoint_residual"])
        self.assertFalse(FUSED_INCOMPLETE_DIMENSIONS["sticky_residue"])
        self.assertFalse(TRAPS_INCOMPLETE_DIMENSIONS["fe0_fe1"])
        self.assertFalse(TRAPS_INCOMPLETE_DIMENSIONS["traps"])

    def test_malformed_fp_advanced_obligation_fail_closed(self) -> None:
        bad = build_fp_advanced_obligation(FP_FUSED_CAPABILITY, opcodes=["fmadd"])
        bad["extra_field"] = True
        self.assertIsNotNone(validate_fp_advanced_obligation(bad))
        self.assertEqual(
            evaluate_fp_advanced_obligation_status(bad),
            STATUS_INCOMPLETE,
        )
        # Wrong opcode family.
        wrong = build_fp_advanced_obligation(
            FP_FUSED_CAPABILITY, opcodes=["fadd"]
        )
        self.assertIsNotNone(validate_fp_advanced_obligation(wrong))

    def test_allowlisted_still_never_promotion_grade(self) -> None:
        attestation = build_fp_advanced_attestation(
            FP_FUSED_CAPABILITY, opcodes=["fmadd"]
        )
        # Even with allowlist + forged clean unsupported, stay incomplete.
        status = recompute_fp_advanced_attestation_status(
            {**attestation.evidence, "host_float": False},
            capability=FP_FUSED_CAPABILITY,
            algorithm=FP_FUSED_ALGORITHM,
            model_version=attestation.model_version,
            unsupported=(),
            allowed_versions=(attestation.model_version,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

        traps = build_fp_advanced_attestation(FP_TRAPS_CAPABILITY)
        status = recompute_fp_advanced_attestation_status(
            traps.evidence,
            capability=FP_TRAPS_CAPABILITY,
            algorithm=FP_TRAPS_ALGORITHM,
            model_version=traps.model_version,
            unsupported=(),
            allowed_versions=(traps.model_version,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)


class MmioAdvancedTests(unittest.TestCase):
    def test_gx_fifo_read_always_incomplete(self) -> None:
        self.assertIn(GX_FIFO_READ_CAPABILITY, ALWAYS_INCOMPLETE_MMIO_CAPABILITIES)
        attestation = build_always_incomplete_mmio_attestation(
            GX_FIFO_READ_CAPABILITY
        )
        forged = build_attestation(
            capability=attestation.capability,
            model_version=attestation.model_version,
            algorithm=attestation.algorithm,
            status=STATUS_PROMOTION_GRADE,
            evidence=dict(attestation.evidence),
            unsupported=(),
        )
        proof = _equivalent(
            proof_features=["memory-bus"],
            memory_bus={
                "schema_version": 2,
                "fifo_read_attempted": True,
                "fifo_theory": {"status": "present"},
            },
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("lwz"),
                    forged,
                    _provenance_attestation(),
                ),
            ).to_dict(),
            opcodes_used=["lwz"],
        )
        used = infer_used_capabilities(proof)
        self.assertIn(GX_FIFO_READ_CAPABILITY, used)
        result = evaluate_capability_assurance(
            proof, _ledger("lwz"), _manifest()
        )
        self.assertEqual(
            result.recomputed_statuses.get(GX_FIFO_READ_CAPABILITY),
            STATUS_INCOMPLETE,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_dma_attestation_never_promotion_grade(self) -> None:
        attestation = build_always_incomplete_mmio_attestation(
            DMA_INTERRUPT_CAPABILITY
        )
        status = recompute_mmio_attestation_status(
            attestation.evidence,
            capability=DMA_INTERRUPT_CAPABILITY,
            algorithm=DMA_INTERRUPT_ALGORITHM,
            model_version=DMA_INTERRUPT_MODEL_VERSION,
            unsupported=(),
            allowed_versions=(DMA_INTERRUPT_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

        for capability in (
            MMIO_READ_SIDE_EFFECTS_CAPABILITY,
            MMIO_EXTERNAL_INPUT_CAPABILITY,
            DMA_INTERRUPT_CAPABILITY,
        ):
            stub = build_always_incomplete_mmio_attestation(capability)
            forged = build_attestation(
                capability=stub.capability,
                model_version=stub.model_version,
                algorithm=stub.algorithm,
                status=STATUS_PROMOTION_GRADE,
                evidence=dict(stub.evidence),
                unsupported=(),
            )
            proof = _equivalent(
                proof_features=["memory-bus"],
                memory_bus={
                    "schema_version": 2,
                    "has_dma_interrupt": capability == DMA_INTERRUPT_CAPABILITY,
                    "has_read_side_effects": (
                        capability == MMIO_READ_SIDE_EFFECTS_CAPABILITY
                    ),
                    "has_external_input": (
                        capability == MMIO_EXTERNAL_INPUT_CAPABILITY
                    ),
                },
                capability_assurance=CapabilityAssurance(
                    capabilities=(
                        _integer_attestation("lwz"),
                        forged,
                        _provenance_attestation(),
                    ),
                ).to_dict(),
                opcodes_used=["lwz"],
            )
            used = infer_used_capabilities(proof)
            self.assertIn(capability, used)
            result = evaluate_capability_assurance(
                proof, _ledger("lwz"), _manifest()
            )
            self.assertEqual(
                result.recomputed_statuses.get(capability),
                STATUS_INCOMPLETE,
            )
            self.assertEqual(
                compute_confidence_tier_from_assurance(result), "C"
            )

    def test_loop_refinement_incomplete_without_real_unsat(self) -> None:
        obligation = build_gx_fifo_loop_refinement_obligation()
        self.assertIsNone(validate_gx_fifo_loop_refinement_obligation(obligation))
        self.assertFalse(loop_refinement_has_real_unsat(obligation))
        attestation = build_gx_fifo_loop_refinement_attestation(obligation)
        status = recompute_mmio_attestation_status(
            attestation.evidence,
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            unsupported=(),
            allowed_versions=(GX_FIFO_TRACE_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

        # Placeholder all-zero digests with unsat still incomplete.
        fake = build_gx_fifo_loop_refinement_obligation(
            original={"result": "unsat", "query_sha256": "0" * 64},
            candidate={"result": "unsat", "query_sha256": "0" * 64},
            status="promotion-grade",
        )
        self.assertFalse(loop_refinement_has_real_unsat(fake))

    def test_malformed_loop_refinement_fail_closed(self) -> None:
        bad = build_gx_fifo_loop_refinement_obligation()
        bad["unknown"] = 1
        self.assertIsNotNone(validate_gx_fifo_loop_refinement_obligation(bad))
        status = recompute_mmio_attestation_status(
            {"loop_refinement": bad},
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            unsupported=(),
            allowed_versions=(GX_FIFO_TRACE_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

        # Missing attestation for demanded fifo write → unmodeled / C.
        proof = _equivalent(
            proof_features=["memory-bus"],
            memory_bus={
                "schema_version": 2,
                "fifo_theory": {"status": "present"},
                "gxfifo_trace": {"status": "pending"},
            },
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("stw"),
                    _provenance_attestation(),
                ),
            ).to_dict(),
            opcodes_used=["stw"],
        )
        used = infer_used_capabilities(proof)
        self.assertIn(GX_FIFO_WRITE_TRACE_CAPABILITY, used)
        result = evaluate_capability_assurance(
            proof, _ledger("stw"), _manifest()
        )
        self.assertEqual(
            result.recomputed_statuses.get(GX_FIFO_WRITE_TRACE_CAPABILITY),
            STATUS_UNMODELED,
        )


class ManifestAndLedgerTests(unittest.TestCase):
    def test_wave4_allowlists_empty_and_shadow(self) -> None:
        loaded = load_capability_manifest()
        self.assertTrue(loaded.shadow_mode)
        for cap in (
            "fp-fused-arithmetic",
            "fp-paired-single",
            "fp-psq",
            "fp-traps",
            "gx-fifo-write-trace",
            "gx-fifo-read",
            "mmio-read-side-effects",
            "mmio-external-input",
            "dma-interrupt-effects",
            "mmio-loop-emission",
        ):
            self.assertEqual(loaded.allowed_versions(cap), ())

    def test_ledger_skeletons_declare_incomplete_dimensions(self) -> None:
        from tools.coop.lib.equivalence_policy import (
            ValidationLedger,
            default_validation_ledger_path,
        )

        ledger = ValidationLedger.load(default_validation_ledger_path())
        fused = ledger.capabilities.get("fp-fused-arithmetic") or {}
        dims = fused.get("dimensions") or {}
        self.assertFalse(dims.get("midpoint_residual"))
        self.assertFalse(dims.get("sticky_residue"))
        self.assertFalse(dims.get("traps"))
        traps = ledger.capabilities.get("fp-traps") or {}
        trap_dims = traps.get("dimensions") or {}
        self.assertFalse(trap_dims.get("fe0_fe1"))
        self.assertFalse(trap_dims.get("traps"))
        self.assertEqual(ARCHITECTURE_MODEL, "broadway-ppc32-be-v41")
        self.assertEqual(RESULT_FORMAT, 21)


if __name__ == "__main__":
    unittest.main()

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
    GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY,
    GX_FIFO_READ_ALGORITHM,
    GX_FIFO_READ_CAPABILITY,
    GX_FIFO_READ_MODEL_VERSION,
    GX_FIFO_TRACE_MODEL_VERSION,
    GX_FIFO_WRITE_TRACE_CAPABILITY,
    MMIO_EXTERNAL_INPUT_CAPABILITY,
    MMIO_LOOP_EMISSION_ALGORITHM,
    MMIO_LOOP_EMISSION_CAPABILITY,
    MMIO_LOOP_EMISSION_MODEL_VERSION,
    MMIO_READ_SIDE_EFFECTS_CAPABILITY,
    ALWAYS_INCOMPLETE_MMIO_CAPABILITIES,
    build_always_incomplete_mmio_attestation,
    build_gx_fifo_loop_exact_refinement_attestation,
    build_gx_fifo_loop_exact_refinement_obligation,
    build_gx_fifo_loop_refinement_obligation,
    build_gx_fifo_read_obligation,
    build_mmio_loop_emission_obligation,
    gx_fifo_read_is_promotion_grade,
    loop_exact_refinement_has_real_unsat,
    loop_refinement_has_real_unsat,
    mmio_loop_emission_is_promotion_grade,
    recompute_mmio_attestation_status,
    validate_gx_fifo_loop_exact_refinement_obligation,
    validate_gx_fifo_loop_refinement_obligation,
    validate_gx_fifo_read_obligation,
    validate_mmio_loop_emission_obligation,
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


def _real_discharged_gx_fifo_loop_side(*, count: int = 4) -> dict:
    """Recognize + discharge a real GX FIFO CTR loop; return the attachable side.

    Uses the actual ``gx_fifo_loop.py`` / ``gx_fifo_loop_discharge.py``
    production modules so tests exercise the real per-block UNSAT structure
    rather than a hand-crafted stand-in.
    """
    from tools.ppc_equivalence.gx_fifo_loop import build_gx_fifo_loop_plans
    from tools.ppc_equivalence.gx_fifo_loop_discharge import (
        discharge_gx_fifo_loop_plan,
    )
    from tools.ppc_equivalence.ir import Instruction, Opcode

    def insn(opcode, operands, *, address=0, link=False):
        return Instruction(address, 0, opcode, operands, link=link)

    base_reg = 5
    source_reg = 3
    prologue = [
        insn(Opcode.ADDIS, (base_reg, 0, 0xCC00), address=0),
        insn(Opcode.ORI, (base_reg, base_reg, 0x8000), address=4),
    ]
    pc = len(prologue) * 4
    prologue.append(insn(Opcode.ADDI, (0, 0, count), address=pc))
    pc += 4
    prologue.append(insn(Opcode.MTSPR, (0, 9), address=pc))
    pc += 4
    header_pc = pc
    body = [insn(Opcode.STW, (source_reg, base_reg, 0), address=pc)]
    pc += 4
    latch_pc = pc
    program = [
        *prologue,
        *body,
        insn(Opcode.BC, (16, 0, header_pc, 0), address=latch_pc),
        insn(Opcode.BCLR, (20, 0, 0), address=latch_pc + 4),
    ]
    plans = build_gx_fifo_loop_plans(program)
    plan = next(iter(plans.values()))
    result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
    assert result.all_unsat(), result.reason
    return {"entry_guard": result.entry_guard, "refinement": result.refinement}


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
    def test_gx_fifo_read_not_always_incomplete_but_fails_closed(self) -> None:
        # Wave 5: gx-fifo-read is promotable in principle (unsupported-read
        # *policy*, never a value model) but still grades incomplete without
        # an allowlist entry or a caller-supplied promotion-grade obligation.
        self.assertNotIn(GX_FIFO_READ_CAPABILITY, ALWAYS_INCOMPLETE_MMIO_CAPABILITIES)

        default_obligation = build_gx_fifo_read_obligation()
        self.assertIsNone(validate_gx_fifo_read_obligation(default_obligation))
        self.assertFalse(gx_fifo_read_is_promotion_grade(default_obligation))
        attestation = build_attestation(
            capability=GX_FIFO_READ_CAPABILITY,
            model_version=GX_FIFO_READ_MODEL_VERSION,
            algorithm=GX_FIFO_READ_ALGORITHM,
            status=STATUS_INCOMPLETE,
            evidence={"gx_fifo_read": default_obligation},
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
        # Empty allowlist (default _manifest()) → incomplete even though the
        # forged attestation claims promotion-grade.
        result = evaluate_capability_assurance(
            proof, _ledger("lwz"), _manifest()
        )
        self.assertEqual(
            result.recomputed_statuses.get(GX_FIFO_READ_CAPABILITY),
            STATUS_INCOMPLETE,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_gx_fifo_read_vacuous_promotion_grade_when_allowlisted(self) -> None:
        vacuous = build_gx_fifo_read_obligation(vacuous=True, status="promotion-grade")
        self.assertIsNone(
            validate_gx_fifo_read_obligation(vacuous, require_promotion_shape=True)
        )
        self.assertTrue(gx_fifo_read_is_promotion_grade(vacuous))
        status = recompute_mmio_attestation_status(
            {"gx_fifo_read": vacuous},
            capability=GX_FIFO_READ_CAPABILITY,
            algorithm=GX_FIFO_READ_ALGORITHM,
            model_version=GX_FIFO_READ_MODEL_VERSION,
            allowed_versions=(GX_FIFO_READ_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_PROMOTION_GRADE)

        # Same obligation without an allowlist entry stays incomplete.
        status_unallowed = recompute_mmio_attestation_status(
            {"gx_fifo_read": vacuous},
            capability=GX_FIFO_READ_CAPABILITY,
            algorithm=GX_FIFO_READ_ALGORITHM,
            model_version=GX_FIFO_READ_MODEL_VERSION,
            allowed_versions=(),
        )
        self.assertEqual(status_unallowed, STATUS_INCOMPLETE)

    def test_gx_fifo_read_reviewed_profile_requires_real_unsat_digest(self) -> None:
        # No path_and_gx_read digest yet → not promotion-grade.
        pending = build_gx_fifo_read_obligation(
            hardware_profile="wii-broadway-xenoblade-us-v2",
            status="promotion-grade",
        )
        self.assertFalse(gx_fifo_read_is_promotion_grade(pending))

        placeholder = build_gx_fifo_read_obligation(
            hardware_profile="wii-broadway-xenoblade-us-v2",
            path_and_gx_read={"result": "unsat", "query_sha256": "0" * 64},
            status="promotion-grade",
        )
        self.assertFalse(gx_fifo_read_is_promotion_grade(placeholder))

        real = build_gx_fifo_read_obligation(
            hardware_profile="wii-broadway-xenoblade-us-v2",
            path_and_gx_read={"result": "unsat", "query_sha256": "e" * 64},
            status="promotion-grade",
        )
        self.assertTrue(gx_fifo_read_is_promotion_grade(real))

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

    def test_v1_legacy_loop_refinement_never_promotion_grade(self) -> None:
        # Legacy digest-only shape: real unsat on both sides structurally,
        # but the algorithm tag alone permanently rejects promotion.
        legacy = build_gx_fifo_loop_refinement_obligation(
            original={"result": "unsat", "query_sha256": "e" * 64},
            candidate={"result": "unsat", "query_sha256": "e" * 64},
            status="promotion-grade",
        )
        self.assertIsNone(validate_gx_fifo_loop_refinement_obligation(legacy))
        self.assertTrue(loop_refinement_has_real_unsat(legacy))
        status = recompute_mmio_attestation_status(
            {"loop_refinement": legacy},
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            unsupported=(),
            allowed_versions=(GX_FIFO_TRACE_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

    def test_loop_exact_refinement_incomplete_without_per_block_real_unsat(self) -> None:
        obligation = build_gx_fifo_loop_exact_refinement_obligation()
        self.assertIsNone(validate_gx_fifo_loop_exact_refinement_obligation(obligation))
        self.assertFalse(loop_exact_refinement_has_real_unsat(obligation))
        attestation = build_gx_fifo_loop_exact_refinement_attestation(obligation)
        status = recompute_mmio_attestation_status(
            attestation.evidence,
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            unsupported=(),
            allowed_versions=(GX_FIFO_TRACE_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

        # Placeholder all-zero entry_guard digest with unsat still incomplete.
        real_side = _real_discharged_gx_fifo_loop_side()
        placeholder_side = {
            "entry_guard": {"result": "unsat", "query_sha256": "0" * 64},
            "refinement": real_side["refinement"],
        }
        fake = build_gx_fifo_loop_exact_refinement_obligation(
            original=placeholder_side,
            candidate=placeholder_side,
            status="promotion-grade",
        )
        self.assertFalse(loop_exact_refinement_has_real_unsat(fake))

        # One side pending (not yet discharged) still fails closed.
        pending_side = {
            "entry_guard": {"result": "pending"},
            "refinement": {"algorithm": GX_FIFO_LOOP_REFINEMENT_ALGORITHM, "status": "applied"},
        }
        partial = build_gx_fifo_loop_exact_refinement_obligation(
            original=real_side,
            candidate=pending_side,
            status="promotion-grade",
        )
        self.assertIsNone(validate_gx_fifo_loop_exact_refinement_obligation(partial))
        self.assertFalse(loop_exact_refinement_has_real_unsat(partial))

    def test_loop_exact_refinement_promotion_grade_when_all_blocks_real_unsat(self) -> None:
        real_side = _real_discharged_gx_fifo_loop_side()
        obligation = build_gx_fifo_loop_exact_refinement_obligation(
            original=real_side,
            candidate=real_side,
            status="promotion-grade",
        )
        self.assertIsNone(validate_gx_fifo_loop_exact_refinement_obligation(obligation))
        self.assertTrue(loop_exact_refinement_has_real_unsat(obligation))
        status = recompute_mmio_attestation_status(
            {"loop_refinement": obligation},
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            unsupported=(),
            allowed_versions=(GX_FIFO_TRACE_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_PROMOTION_GRADE)

        # Without an allowlist entry the same obligation stays incomplete.
        status_unallowed = recompute_mmio_attestation_status(
            {"loop_refinement": obligation},
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            unsupported=(),
            allowed_versions=(),
        )
        self.assertEqual(status_unallowed, STATUS_INCOMPLETE)

        # A caller-supplied result=unsat with no per-block structure at all
        # (a forged flat dict) must not slip through as promotion-grade.
        forged_flat = {
            "schema_version": 2,
            "capability": GX_FIFO_WRITE_TRACE_CAPABILITY,
            "model_version": GX_FIFO_TRACE_MODEL_VERSION,
            "algorithm": GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
            "claim": obligation["claim"],
            "original": {"result": "unsat", "query_sha256": "9" * 64},
            "candidate": {"result": "unsat", "query_sha256": "9" * 64},
            "status": "promotion-grade",
        }
        self.assertIsNotNone(
            validate_gx_fifo_loop_exact_refinement_obligation(forged_flat)
        )
        status_forged = recompute_mmio_attestation_status(
            {"loop_refinement": forged_flat},
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            unsupported=(),
            allowed_versions=(GX_FIFO_TRACE_MODEL_VERSION,),
        )
        self.assertEqual(status_forged, STATUS_INCOMPLETE)

        # Wrapping a legacy v1 "refinement" (stale block algorithm) inside
        # the v2 envelope must also stay rejected — never promotion-grade.
        legacy_wrapped_side = {
            "entry_guard": real_side["entry_guard"],
            "refinement": {**real_side["refinement"], "algorithm": "gx-fifo-loop-refinement-v1"},
        }
        legacy_wrapped = build_gx_fifo_loop_exact_refinement_obligation(
            original=legacy_wrapped_side,
            candidate=legacy_wrapped_side,
            status="promotion-grade",
        )
        reason = validate_gx_fifo_loop_exact_refinement_obligation(legacy_wrapped)
        self.assertIsNotNone(reason)
        self.assertIn("stale", reason or "")

    def test_malformed_loop_refinement_fail_closed(self) -> None:
        bad = build_gx_fifo_loop_exact_refinement_obligation()
        bad["unknown"] = 1
        self.assertIsNotNone(validate_gx_fifo_loop_exact_refinement_obligation(bad))
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


class MmioLoopEmissionTests(unittest.TestCase):
    def test_not_always_incomplete_but_fails_closed_without_allowlist(self) -> None:
        self.assertNotIn(
            MMIO_LOOP_EMISSION_CAPABILITY, ALWAYS_INCOMPLETE_MMIO_CAPABILITIES
        )
        real_side = _real_discharged_gx_fifo_loop_side()
        loop_refinement = build_gx_fifo_loop_exact_refinement_obligation(
            original=real_side, candidate=real_side, status="promotion-grade",
        )
        obligation = build_mmio_loop_emission_obligation(
            loop_refinement=loop_refinement,
            shape={"device_id": "gx-fifo", "width": 4},
            status="promotion-grade",
        )
        self.assertIsNone(validate_mmio_loop_emission_obligation(obligation))
        self.assertTrue(mmio_loop_emission_is_promotion_grade(obligation))

        status_allowed = recompute_mmio_attestation_status(
            {"mmio_loop_emission": obligation},
            capability=MMIO_LOOP_EMISSION_CAPABILITY,
            algorithm=MMIO_LOOP_EMISSION_ALGORITHM,
            model_version=MMIO_LOOP_EMISSION_MODEL_VERSION,
            allowed_versions=(MMIO_LOOP_EMISSION_MODEL_VERSION,),
        )
        self.assertEqual(status_allowed, STATUS_PROMOTION_GRADE)

        status_unallowed = recompute_mmio_attestation_status(
            {"mmio_loop_emission": obligation},
            capability=MMIO_LOOP_EMISSION_CAPABILITY,
            algorithm=MMIO_LOOP_EMISSION_ALGORITHM,
            model_version=MMIO_LOOP_EMISSION_MODEL_VERSION,
            allowed_versions=(),
        )
        self.assertEqual(status_unallowed, STATUS_INCOMPLETE)

    def test_wrapped_legacy_or_incomplete_refinement_stays_incomplete(self) -> None:
        default_obligation = build_mmio_loop_emission_obligation()
        self.assertIsNone(validate_mmio_loop_emission_obligation(default_obligation))
        self.assertFalse(mmio_loop_emission_is_promotion_grade(default_obligation))
        status = recompute_mmio_attestation_status(
            {"mmio_loop_emission": default_obligation},
            capability=MMIO_LOOP_EMISSION_CAPABILITY,
            algorithm=MMIO_LOOP_EMISSION_ALGORITHM,
            model_version=MMIO_LOOP_EMISSION_MODEL_VERSION,
            allowed_versions=(MMIO_LOOP_EMISSION_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

        # Wrapping a legacy v1-algorithm-tagged refinement is malformed for
        # the v2-only wrapper and must fail validation.
        legacy = build_gx_fifo_loop_refinement_obligation(
            original={"result": "unsat", "query_sha256": "1" * 64},
            candidate={"result": "unsat", "query_sha256": "1" * 64},
        )
        bad_wrap = build_mmio_loop_emission_obligation(loop_refinement=legacy)
        self.assertIsNotNone(validate_mmio_loop_emission_obligation(bad_wrap))
        self.assertFalse(mmio_loop_emission_is_promotion_grade(bad_wrap))


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
        self.assertEqual(ARCHITECTURE_MODEL, "broadway-ppc32-be-v44")
        self.assertEqual(RESULT_FORMAT, 23)


if __name__ == "__main__":
    unittest.main()

"""Wave 3 FP foundations: symbolic FPOutcome, obligations, RN/precondition-closure."""

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
)
from tools.ppc_equivalence.fp_capabilities import (
    FP_COMPARE_OPS,
    FP_CONVERT_OPS,
    FP_FUSED_ARITH_OPS,
    FP_LOAD_STORE_OPS,
    FP_OUTCOME_UNIFY_ALGORITHM,
    FP_PAIRED_SINGLE_OPS,
    FP_PSQ_OPS,
    FP_SCALAR_ARITH_OPS,
    FP_SCALAR_MODEL_VERSION,
    FP_SCALAR_ORACLE_ALGORITHM,
    classify_fp_capabilities,
)
from tools.ppc_equivalence.fp_outcome import (
    SYMBOLIC_SCALAR_ARITHMETIC_STATUS,
    symbolic_bitwise_outcome,
    symbolic_fp_outcome,
    symbolic_scalar_arithmetic_outcome,
)
from tools.ppc_equivalence.fp_rounding import (
    BROADWAY_RN_MODES,
    PRECONDITION_CLOSURE_ALGORITHM,
    PRECONDITION_CLOSURE_CAPABILITY,
    PRECONDITION_CLOSURE_MODEL_VERSION,
    RN_MODES_MODELED,
    evaluate_precondition_closure_status,
    fpscr_sticky_modeling_status,
    tier_a_rounding_satisfied,
)
from tools.ppc_equivalence.fp_scalar_obligations import (
    build_fp_scalar_attestation,
    build_fp_scalar_obligation,
    evaluate_fp_scalar_obligation_status,
    validate_fp_scalar_obligation,
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
            "fp-scalar-arithmetic": (),
            "precondition-closure": (),
        },
        shadow_mode=True,
    )
    defaults.update(kwargs)
    return CapabilityManifest(**defaults)


_NEG_ONE = 0xBFF0000000000000
_POS_ONE = 0x3FF0000000000000
_QNAN = 0x7FF8000012345678
_NEG_QNAN = 0xFFF8000012345678


class SymbolicFpOutcomeTests(unittest.TestCase):
    def test_fmr_fabs_concrete_parity(self) -> None:
        for bits in (_NEG_ONE, _POS_ONE, _QNAN, _NEG_QNAN, 0, 1 << 63):
            fmr = symbolic_bitwise_outcome("fmr", bits)
            self.assertTrue(fmr.supported)
            self.assertEqual(fmr.primary_bits(), bits & 0xFFFFFFFFFFFFFFFF)

            fabs = symbolic_bitwise_outcome("fabs", bits)
            self.assertTrue(fabs.supported)
            self.assertEqual(fabs.primary_bits(), bits & ~(1 << 63))

    def test_fneg_fnabs_concrete(self) -> None:
        self.assertEqual(
            symbolic_bitwise_outcome("fneg", _POS_ONE).primary_bits(),
            _NEG_ONE,
        )
        self.assertEqual(
            symbolic_bitwise_outcome("fnabs", _POS_ONE).primary_bits(),
            _NEG_ONE,
        )

    def test_symbolic_bitwise_z3_parity(self) -> None:
        try:
            import z3
        except ImportError:  # pragma: no cover
            self.skipTest("z3 not available")
        bits = z3.BitVecVal(_NEG_ONE, 64)
        fabs = symbolic_bitwise_outcome("fabs", bits, z3=z3)
        self.assertTrue(fabs.supported)
        solver = z3.Solver()
        solver.add(fabs.primary_bits() != z3.BitVecVal(_POS_ONE, 64))
        self.assertEqual(solver.check(), z3.unsat)

        fmr = symbolic_bitwise_outcome("fmr", bits, z3=z3)
        solver = z3.Solver()
        solver.add(fmr.primary_bits() != bits)
        self.assertEqual(solver.check(), z3.unsat)

    def test_symbolic_fadd_incomplete(self) -> None:
        outcome = symbolic_scalar_arithmetic_outcome("fadd", _POS_ONE, _POS_ONE)
        self.assertFalse(outcome.supported)
        self.assertIn(SYMBOLIC_SCALAR_ARITHMETIC_STATUS, outcome.unsupported_reason or "")
        self.assertEqual(
            evaluate_fp_scalar_obligation_status(
                build_fp_scalar_obligation(opcodes=["fadd"]),
            ),
            STATUS_INCOMPLETE,
        )

    def test_host_float_not_tier_a(self) -> None:
        outcome = symbolic_fp_outcome(
            _POS_ONE,
            host_float_participated=True,
        )
        self.assertFalse(outcome.supported)
        self.assertIn("host float", outcome.unsupported_reason or "")

        bitwise = symbolic_bitwise_outcome(
            "fabs",
            _NEG_ONE,
            host_float_participated=True,
        )
        self.assertFalse(bitwise.supported)

        status = evaluate_fp_scalar_obligation_status(
            build_fp_scalar_obligation(opcodes=["fadd"]),
            host_float=True,
        )
        self.assertEqual(status, STATUS_INCOMPLETE)
        self.assertFalse(
            tier_a_rounding_satisfied(modeled_rn=RN_MODES_MODELED),
        )


class FpCapabilitySplitTests(unittest.TestCase):
    def test_distinct_caps_for_families(self) -> None:
        self.assertEqual(
            classify_fp_capabilities(["lfs", "blr"]),
            frozenset({"fp-load-store"}),
        )
        self.assertEqual(
            classify_fp_capabilities(["fcmpu"]),
            frozenset({"fp-compare"}),
        )
        self.assertEqual(
            classify_fp_capabilities(["fctiwz"]),
            frozenset({"fp-convert"}),
        )
        self.assertEqual(
            classify_fp_capabilities(["fadd"]),
            frozenset({"fp-scalar-arithmetic"}),
        )
        self.assertEqual(
            classify_fp_capabilities(["fmadd"]),
            frozenset({"fp-fused-arithmetic"}),
        )
        self.assertEqual(
            classify_fp_capabilities(["ps_add"]),
            frozenset({"fp-paired-single"}),
        )
        self.assertEqual(
            classify_fp_capabilities(["psq_l"]),
            frozenset({"fp-psq"}),
        )
        self.assertEqual(
            classify_fp_capabilities(["fmr"], traps_enabled=True),
            frozenset({"fp-bitwise", "fp-traps"}),
        )

    def test_unknown_fp_subcapability_fail_closed(self) -> None:
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fadd", "blr"],
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    build_attestation(
                        capability="integer-core",
                        model_version="integer-core-v1",
                        algorithm="opcode-ledger-v2",
                        evidence={"opcodes": ["blr"], "ledger_sha256": "e" * 64},
                    ),
                    build_attestation(
                        capability="provenance",
                        model_version="provenance-v1",
                        algorithm="provenance-binding-v1",
                        evidence={
                            "engine_hash": "a" * 64,
                            "certifier_hash": "d" * 64,
                            "source_hash": "b" * 64,
                            "git_commit": "c" * 40,
                            "git_dirty": False,
                        },
                    ),
                ),
            ).to_dict(),
        )
        used = infer_used_capabilities(proof)
        self.assertIn("fp-scalar-arithmetic", used)
        result = evaluate_capability_assurance(proof, _ledger("fadd", "blr"), _manifest())
        self.assertEqual(
            result.recomputed_statuses.get("fp-scalar-arithmetic"),
            STATUS_UNMODELED,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_opcode_sets_nonempty(self) -> None:
        for name, ops in (
            ("load-store", FP_LOAD_STORE_OPS),
            ("compare", FP_COMPARE_OPS),
            ("convert", FP_CONVERT_OPS),
            ("scalar", FP_SCALAR_ARITH_OPS),
            ("fused", FP_FUSED_ARITH_OPS),
            ("paired", FP_PAIRED_SINGLE_OPS),
            ("psq", FP_PSQ_OPS),
        ):
            self.assertTrue(ops, msg=name)


class FpScalarObligationTests(unittest.TestCase):
    def test_obligation_schema_valid(self) -> None:
        obligation = build_fp_scalar_obligation(opcodes=["fadd", "fadds"])
        self.assertIsNone(validate_fp_scalar_obligation(obligation))
        self.assertEqual(obligation["capability"], "fp-scalar-arithmetic")
        self.assertEqual(obligation["model_version"], FP_SCALAR_MODEL_VERSION)
        self.assertEqual(obligation["modes"]["rn"], list(BROADWAY_RN_MODES))
        self.assertEqual(obligation["modes"]["ni"], [0, 1])

    def test_obligation_rejects_malformed(self) -> None:
        self.assertIsNotNone(validate_fp_scalar_obligation(None))
        self.assertIsNotNone(validate_fp_scalar_obligation({"capability": "nope"}))
        bad = build_fp_scalar_obligation(opcodes=["fadd"])
        bad["extra_field"] = 1
        self.assertIsNotNone(validate_fp_scalar_obligation(bad))
        bad2 = build_fp_scalar_obligation(opcodes=["fadd"])
        bad2["modes"]["rn"] = ["not-a-mode"]
        self.assertIsNotNone(validate_fp_scalar_obligation(bad2))

    def test_scalar_fadd_not_promotion_grade(self) -> None:
        obligation = build_fp_scalar_obligation(opcodes=["fadd"])
        attestation = build_fp_scalar_attestation(obligation)
        self.assertEqual(attestation.algorithm, FP_SCALAR_ORACLE_ALGORITHM)
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["fadd", "blr"],
            capability_assurance=CapabilityAssurance(
                capabilities=(attestation,),
            ).to_dict(),
        )
        # Allowlist empty → incomplete even if we forged promotion status.
        result = evaluate_capability_assurance(
            proof,
            _ledger("fadd", "blr"),
            _manifest(
                allowed_tier_a_capabilities={
                    "fp-scalar-arithmetic": (FP_SCALAR_MODEL_VERSION,),
                    "integer-core": ("integer-core-v1",),
                    "provenance": ("provenance-v1",),
                }
            ),
        )
        self.assertEqual(
            result.recomputed_statuses.get("fp-scalar-arithmetic"),
            STATUS_INCOMPLETE,
        )
        self.assertNotEqual(
            result.recomputed_statuses.get("fp-scalar-arithmetic"),
            STATUS_PROMOTION_GRADE,
        )

    def test_host_float_attestation_incomplete(self) -> None:
        obligation = build_fp_scalar_obligation(opcodes=["fadd"])
        attestation = build_attestation(
            capability="fp-scalar-arithmetic",
            model_version=FP_SCALAR_MODEL_VERSION,
            algorithm=FP_OUTCOME_UNIFY_ALGORITHM,
            evidence={"opcodes": ["fadd"], "host_float": True},
        )
        status = evaluate_capability_assurance(
            _equivalent(
                floating_point_domain=FloatingPointDomain(),
                opcodes_used=["fadd"],
                capability_assurance=CapabilityAssurance(
                    capabilities=(attestation,),
                ).to_dict(),
            ),
            _ledger("fadd"),
            _manifest(
                allowed_tier_a_capabilities={
                    "fp-scalar-arithmetic": (FP_SCALAR_MODEL_VERSION,),
                }
            ),
        ).recomputed_statuses.get("fp-scalar-arithmetic")
        self.assertEqual(status, STATUS_INCOMPLETE)
        del obligation


class PreconditionClosureTests(unittest.TestCase):
    def test_without_unsat_not_promotion_grade(self) -> None:
        self.assertEqual(
            evaluate_precondition_closure_status({}),
            STATUS_INCOMPLETE,
        )
        self.assertEqual(
            evaluate_precondition_closure_status(
                {
                    "violation_query_result": "sat",
                    "violation_query_sha256": "a" * 64,
                }
            ),
            STATUS_INCOMPLETE,
        )
        self.assertEqual(
            evaluate_precondition_closure_status(
                {
                    "violation_query_result": "incomplete",
                    "violation_query_sha256": "a" * 64,
                }
            ),
            STATUS_INCOMPLETE,
        )

    def test_unsat_is_promotion_grade_when_allowlisted(self) -> None:
        evidence = {
            "violation_query_result": "unsat",
            "violation_query_sha256": "ab" * 32,
        }
        self.assertEqual(
            evaluate_precondition_closure_status(evidence),
            STATUS_PROMOTION_GRADE,
        )
        attestation = build_attestation(
            capability=PRECONDITION_CLOSURE_CAPABILITY,
            model_version=PRECONDITION_CLOSURE_MODEL_VERSION,
            algorithm=PRECONDITION_CLOSURE_ALGORITHM,
            evidence=evidence,
        )
        # Empty allowlist → incomplete.
        empty = evaluate_capability_assurance(
            _equivalent(
                capability_assurance=CapabilityAssurance(
                    capabilities=(attestation,),
                ).to_dict(),
            ),
            _ledger("addi"),
            _manifest(),
        )
        self.assertEqual(
            empty.recomputed_statuses.get(PRECONDITION_CLOSURE_CAPABILITY),
            STATUS_INCOMPLETE,
        )
        # Allowlisted + UNSAT → promotion-grade on the attestation itself.
        allowlisted = evaluate_capability_assurance(
            _equivalent(
                capability_assurance=CapabilityAssurance(
                    capabilities=(attestation,),
                ).to_dict(),
            ),
            _ledger("addi"),
            _manifest(
                allowed_tier_a_capabilities={
                    "integer-core": ("integer-core-v1",),
                    "provenance": ("provenance-v1",),
                    "fp-bitwise": ("fp-bitwise-v1",),
                    "precondition-closure": (PRECONDITION_CLOSURE_MODEL_VERSION,),
                }
            ),
        )
        self.assertEqual(
            allowlisted.recomputed_statuses.get(PRECONDITION_CLOSURE_CAPABILITY),
            STATUS_PROMOTION_GRADE,
        )

    def test_default_rn_not_tier_a(self) -> None:
        self.assertFalse(tier_a_rounding_satisfied(modeled_rn=RN_MODES_MODELED))
        self.assertTrue(
            tier_a_rounding_satisfied(modeled_rn=BROADWAY_RN_MODES),
        )
        self.assertTrue(
            tier_a_rounding_satisfied(
                modeled_rn=RN_MODES_MODELED,
                precondition_closure_status=STATUS_PROMOTION_GRADE,
            ),
        )

    def test_fpscr_fx_fex_incomplete(self) -> None:
        status = fpscr_sticky_modeling_status()
        self.assertFalse(status["fx"])
        self.assertFalse(status["fex"])
        self.assertFalse(status["sticky_preservation_complete"])


class ManifestShadowModeTests(unittest.TestCase):
    def test_manifest_keeps_shadow_and_empty_scalar(self) -> None:
        from tools.ppc_equivalence.capability_assurance import (
            load_capability_manifest,
        )

        loaded = load_capability_manifest()
        self.assertTrue(loaded.shadow_mode)
        self.assertEqual(loaded.allowed_versions("fp-scalar-arithmetic"), ())
        self.assertEqual(loaded.allowed_versions("fp-bitwise"), ("fp-bitwise-v1",))
        for cap in (
            "fp-load-store",
            "fp-compare",
            "fp-convert",
            "fp-fused-arithmetic",
            "fp-paired-single",
            "fp-psq",
            "fp-traps",
        ):
            self.assertEqual(loaded.allowed_versions(cap), ())


if __name__ == "__main__":
    unittest.main()

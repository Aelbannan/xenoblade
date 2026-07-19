from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.model import InvalidReason, concrete_state
from tools.ppc_equivalence.result import (
    FP_COVERAGE_ASSUMED,
    FP_COVERAGE_PROVEN,
    FP_COVERAGE_STATUS_ASSUMED,
    FP_COVERAGE_STATUS_NONE,
    FP_COVERAGE_UNSUPPORTED,
    FloatingPointDomain,
    ProofStatus,
    fp_coverage_status,
    proof_fp_coverage_status,
)
from tools.ppc_equivalence.semantics import ConcreteOps, execute_instruction


def _insn(opcode: Opcode, operands: tuple[int, ...]) -> Instruction:
    return Instruction(0, 0, opcode, operands)


class FloatingPointDomainCoverageTests(unittest.TestCase):
    def test_to_dict_includes_coverage(self):
        d = FloatingPointDomain().to_dict()
        self.assertTrue(d["used"])
        self.assertIn("coverage", d)
        self.assertEqual(d["coverage"]["proven"], list(FP_COVERAGE_PROVEN))
        self.assertEqual(d["coverage"]["assumed"], list(FP_COVERAGE_ASSUMED))
        self.assertEqual(d["coverage"]["unsupported"], list(FP_COVERAGE_UNSUPPORTED))
        self.assertEqual(d["coverage"]["status"], FP_COVERAGE_STATUS_ASSUMED)
        self.assertTrue(d["allow_subnormal"])
        self.assertIn("fpscr_flags", d)
        self.assertIn("modeled", d["fpscr_flags"])
        self.assertIn("rounding_mode_modeling", d)

    def test_default_domain_coverage_status_is_assumed(self):
        domain = FloatingPointDomain()
        self.assertEqual(domain.coverage_status(), FP_COVERAGE_STATUS_ASSUMED)
        self.assertIn("traps-disabled", domain.coverage_dict()["assumed"])

    def test_fp_coverage_status_none_without_domain(self):
        self.assertEqual(fp_coverage_status(None), FP_COVERAGE_STATUS_NONE)
        self.assertEqual(
            fp_coverage_status(None, used_fp=True),
            FP_COVERAGE_STATUS_ASSUMED,
        )

    def test_from_dict_roundtrip_core_fields(self):
        original = FloatingPointDomain(allow_subnormal=False, allow_nan=False)
        restored = FloatingPointDomain.from_dict(original.to_dict())
        self.assertEqual(restored.rounding_modes, original.rounding_modes)
        self.assertFalse(restored.allow_subnormal)
        self.assertFalse(restored.allow_nan)
        self.assertFalse(restored.traps_enabled)

    def test_from_dict_accepts_constructor_kwargs(self):
        domain = FloatingPointDomain.from_dict({
            "rounding_modes": ["nearest-even"],
            "require_ni_zero": True,
            "traps_enabled": False,
            "exclude_finite_overflow": True,
            "fused_input_domain": "exact-expanded-binary32",
        })
        self.assertTrue(domain.require_ni_zero)
        self.assertTrue(domain.exclude_finite_overflow)

    def test_validate_rejects_traps_enabled(self):
        domain = FloatingPointDomain(traps_enabled=True)
        with self.assertRaises(ValueError) as ctx:
            domain.validate()
        self.assertIn("trap", str(ctx.exception).lower())

    def test_validate_rejects_non_rne_rounding(self):
        domain = FloatingPointDomain(rounding_modes=("toward-zero",))
        with self.assertRaises(ValueError):
            domain.validate()

    def test_validate_rejects_flag_modeling(self):
        with self.assertRaises(ValueError):
            FloatingPointDomain(model_underflow_flag=True).validate()
        with self.assertRaises(ValueError):
            FloatingPointDomain(model_inexact_flag=True).validate()

    def test_parse_fail_closed(self):
        with self.assertRaises(ValueError):
            FloatingPointDomain.parse('{"traps_enabled": true}')


class FloatingPointDomainConstraintTests(unittest.TestCase):
    def test_allow_nan_false_excludes_nan_operand(self):
        domain = FloatingPointDomain(allow_nan=False)
        state = execute_instruction(
            concrete_state({"fpr": {"f1": "0x7ff8000000000000", "f2": "0x3ff0000000000000"}}),
            _insn(Opcode.FADDS, (1, 1, 2)),
            ConcreteOps(),
            floating_point_domain=domain,
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.FP_DOMAIN_EXCLUDED.value)

    def test_allow_subnormal_false_excludes_subnormal(self):
        # Smallest positive binary64 subnormal.
        domain = FloatingPointDomain(allow_subnormal=False)
        state = execute_instruction(
            concrete_state({"fpr": {"f1": "0x0000000000000001", "f2": "0x3ff0000000000000"}}),
            _insn(Opcode.FADDS, (1, 1, 2)),
            ConcreteOps(),
            floating_point_domain=domain,
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.FP_DOMAIN_EXCLUDED.value)

    def test_allow_infinity_false_excludes_infinity_operand(self):
        domain = FloatingPointDomain(allow_infinity=False)
        state = execute_instruction(
            concrete_state({"fpr": {"f1": "0x7ff0000000000000", "f2": "0x3ff0000000000000"}}),
            _insn(Opcode.FADDS, (1, 1, 2)),
            ConcreteOps(),
            floating_point_domain=domain,
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.FP_DOMAIN_EXCLUDED.value)
        self.assertIn("infinity-excluded", domain.coverage_dict()["proven"])

    def test_exclude_finite_overflow_false_keeps_overflow_valid(self):
        huge = 0x47EFFFFFE0000000
        domain = FloatingPointDomain(exclude_finite_overflow=False)
        state = execute_instruction(
            concrete_state({"fpr": {"f1": f"0x{huge:016x}", "f2": f"0x{huge:016x}"}}),
            _insn(Opcode.FADDS, (1, 1, 2)),
            ConcreteOps(),
            floating_point_domain=domain,
        )
        self.assertTrue(state.valid)

    def test_default_domain_still_excludes_finite_overflow(self):
        huge = 0x47EFFFFFE0000000
        state = execute_instruction(
            concrete_state({"fpr": {"f1": f"0x{huge:016x}", "f2": f"0x{huge:016x}"}}),
            _insn(Opcode.FADDS, (1, 1, 2)),
            ConcreteOps(),
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.FP_DOMAIN_EXCLUDED.value)

    def test_engine_rejects_unsupported_domain(self):
        code = parse_hex("ec21102a")  # fadds f1, f1, f2
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=5000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            floating_point_domain=FloatingPointDomain(traps_enabled=True),
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertTrue(any("trap" in item.lower() for item in result.unsupported))
        self.assertIsNotNone(result.floating_point_domain)

    def test_engine_records_domain_on_fp_proof(self):
        code = parse_hex("ec21102a")  # fadds f1, f1, f2
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=10000)
        domain = FloatingPointDomain(allow_subnormal=False)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            floating_point_domain=domain,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertIsNotNone(result.floating_point_domain)
        assert result.floating_point_domain is not None
        self.assertFalse(result.floating_point_domain.allow_subnormal)
        self.assertIn("subnormals-excluded", result.floating_point_domain.coverage_dict()["proven"])


class FloatingPointTierClassificationTests(unittest.TestCase):
    def test_assumed_fp_domain_is_tier_c(self):
        from tools.coop.lib.equivalence_policy import compute_confidence_tier
        from tools.coop.tests.test_promotion_policy import _equivalent_proof, _open_ledger

        proof = _equivalent_proof(floating_point_domain=FloatingPointDomain())
        self.assertEqual(proof_fp_coverage_status(proof), FP_COVERAGE_STATUS_ASSUMED)
        self.assertEqual(compute_confidence_tier(proof, _open_ledger()), "C")

    def test_register_only_proof_without_fp_stays_tier_a(self):
        from tools.coop.lib.equivalence_policy import compute_confidence_tier
        from tools.coop.tests.test_promotion_policy import _equivalent_proof, _open_ledger

        proof = _equivalent_proof()
        self.assertIsNone(proof.floating_point_domain)
        self.assertEqual(compute_confidence_tier(proof, _open_ledger()), "A")

    def test_certificate_with_fp_domain_coverage_is_tier_c(self):
        from tools.coop.lib.equivalence_policy import compute_confidence_tier

        certificate = {
            "summary": {"reads": ["r3"], "writes": ["r3"], "invalid_reasons": []},
            "floating_point_domain": FloatingPointDomain().to_dict(),
        }
        self.assertEqual(compute_confidence_tier(certificate), "C")


if __name__ == "__main__":
    unittest.main()

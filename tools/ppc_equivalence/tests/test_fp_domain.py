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
    FP_COVERAGE_UNSUPPORTED,
    FloatingPointDomain,
    ProofStatus,
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
        self.assertTrue(d["allow_subnormal"])

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


if __name__ == "__main__":
    unittest.main()

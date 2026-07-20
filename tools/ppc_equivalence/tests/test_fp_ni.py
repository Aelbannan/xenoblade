"""Wave 4 Track B / PR17: FPSCR.NI flush-to-zero tests."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.fp_oracle import (
    NI_SCALAR_SUPPORTED_OPS,
    flush_denormal_binary64,
    ni_flush_operand_binary64,
    ni_force_single_result_bits,
)
from tools.ppc_equivalence.fp_outcome import NI_SUPPORTED_OPS, PAIRED_ORACLE_OPS
from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import (
    FP_COVERAGE_PROVEN_NI_MODELED,
    FloatingPointDomain,
    ProofStatus,
)
from tools.ppc_equivalence.semantics import ConcreteOps, FPSCR_NI, execute_instruction

# Smallest positive binary64 subnormal.
_SUB_D = 0x0000000000000001
# Smallest positive binary32 subnormal, expanded into FPR storage.
_SUB_S_FPR = 0x36A0000000000000  # 2^-149
# +1.0
_ONE = 0x3FF0000000000000
# Tiny normals whose product is a binary64 subnormal.
_TINY_A = 0x0010000000000000  # 2^-1022
_TINY_B = 0x3FE0000000000000  # 0.5 → product 2^-1023 (subnormal)


def _insn(opcode: Opcode, operands: tuple[int, ...]) -> Instruction:
    return Instruction(0, 0, opcode, operands)


def _run(
    opcode: Opcode,
    operands: tuple[int, ...],
    fprs: dict[str, str],
    *,
    ni: bool,
    domain: FloatingPointDomain | None = None,
) -> object:
    fpscr = FPSCR_NI if ni else 0
    state = concrete_state({"fpr": fprs, "fpscr": fpscr})
    if domain is None:
        domain = FloatingPointDomain(require_ni_zero=not ni)
    return execute_instruction(
        state,
        _insn(opcode, operands),
        ConcreteOps(),
        floating_point_domain=domain,
    )


class NiHelperTests(unittest.TestCase):
    def test_flush_denormal_preserves_sign(self) -> None:
        self.assertEqual(flush_denormal_binary64(_SUB_D), 0)
        self.assertEqual(flush_denormal_binary64(_SUB_D | (1 << 63)), 1 << 63)
        self.assertEqual(flush_denormal_binary64(_ONE), _ONE)

    def test_ni_force_single_flushes_sub_single_normal(self) -> None:
        self.assertEqual(ni_force_single_result_bits(_SUB_S_FPR, ni=True), 0)
        self.assertEqual(ni_force_single_result_bits(_SUB_S_FPR, ni=False), _SUB_S_FPR)
        self.assertEqual(ni_force_single_result_bits(_ONE, ni=True), _ONE)

    def test_supported_opcode_identity(self) -> None:
        self.assertTrue(NI_SCALAR_SUPPORTED_OPS <= NI_SUPPORTED_OPS)
        self.assertTrue(PAIRED_ORACLE_OPS <= NI_SUPPORTED_OPS)
        self.assertIn("fadd", NI_SUPPORTED_OPS)
        self.assertIn("ps_add", NI_SUPPORTED_OPS)
        self.assertNotIn("fres", NI_SUPPORTED_OPS)
        self.assertNotIn("frsp", NI_SUPPORTED_OPS)


class NiOperandFlushTests(unittest.TestCase):
    def test_subnormal_operand_differs_under_ni(self) -> None:
        # sub + sub → 2*sub under IEEE; both operands flush to 0 under NI.
        ieee = _run(
            Opcode.FADD, (1, 1, 2),
            {"f1": f"0x{_SUB_D:016x}", "f2": f"0x{_SUB_D:016x}"},
            ni=False,
            domain=FloatingPointDomain(require_ni_zero=True),
        )
        ni = _run(
            Opcode.FADD, (1, 1, 2),
            {"f1": f"0x{_SUB_D:016x}", "f2": f"0x{_SUB_D:016x}"},
            ni=True,
            domain=FloatingPointDomain(require_ni_zero=False),
        )
        self.assertTrue(ieee.valid)
        self.assertTrue(ni.valid)
        self.assertEqual(ieee.fpr[1], 0x0000000000000002)
        self.assertEqual(ni.fpr[1], 0)

    def test_single_subnormal_operand_differs_under_ni(self) -> None:
        ieee = _run(
            Opcode.FADDS, (1, 1, 2),
            {"f1": f"0x{_SUB_S_FPR:016x}", "f2": f"0x{_SUB_S_FPR:016x}"},
            ni=False,
            domain=FloatingPointDomain(require_ni_zero=True),
        )
        ni = _run(
            Opcode.FADDS, (1, 1, 2),
            {"f1": f"0x{_SUB_S_FPR:016x}", "f2": f"0x{_SUB_S_FPR:016x}"},
            ni=True,
            domain=FloatingPointDomain(require_ni_zero=False),
        )
        self.assertTrue(ieee.valid)
        self.assertTrue(ni.valid)
        self.assertNotEqual(ieee.fpr[1], 0)
        self.assertEqual(ni.fpr[1], 0)


class NiResultFlushTests(unittest.TestCase):
    def test_subnormal_double_product_flushed_under_ni(self) -> None:
        # fmul frD, frA, frC  → operands (fd, fa, fb_unused, fc)
        ieee = _run(
            Opcode.FMUL, (1, 1, 0, 2),
            {"f1": f"0x{_TINY_A:016x}", "f2": f"0x{_TINY_B:016x}"},
            ni=False,
            domain=FloatingPointDomain(require_ni_zero=True),
        )
        ni = _run(
            Opcode.FMUL, (1, 1, 0, 2),
            {"f1": f"0x{_TINY_A:016x}", "f2": f"0x{_TINY_B:016x}"},
            ni=True,
            domain=FloatingPointDomain(require_ni_zero=False),
        )
        self.assertTrue(ieee.valid)
        self.assertTrue(ni.valid)
        # 2^-1022 * 0.5 = 2^-1023 → binary64 subnormal under IEEE.
        self.assertEqual(ieee.fpr[1] & 0x7FF0000000000000, 0)
        self.assertNotEqual(ieee.fpr[1] & 0x000FFFFFFFFFFFFF, 0)
        self.assertEqual(ni.fpr[1], 0)

    def test_normal_inputs_unchanged_when_ni_set(self) -> None:
        ieee = _run(
            Opcode.FADD, (1, 1, 2),
            {"f1": f"0x{_ONE:016x}", "f2": f"0x{_ONE:016x}"},
            ni=False,
            domain=FloatingPointDomain(require_ni_zero=True),
        )
        ni = _run(
            Opcode.FADD, (1, 1, 2),
            {"f1": f"0x{_ONE:016x}", "f2": f"0x{_ONE:016x}"},
            ni=True,
            domain=FloatingPointDomain(require_ni_zero=False),
        )
        self.assertEqual(ieee.fpr[1], ni.fpr[1])
        self.assertEqual(ni.fpr[1], 0x4000000000000000)  # 2.0


class NiUnsupportedOpcodeTests(unittest.TestCase):
    def test_concrete_frsp_with_ni_raises(self) -> None:
        # frsp is NI-affected (ForceSingle) but not in NI_SUPPORTED_OPS.
        with self.assertRaises(ExecutionInconclusive) as ctx:
            _run(
                Opcode.FRSP, (1, 0, 1),
                {"f1": f"0x{_SUB_S_FPR:016x}"},
                ni=True,
                domain=FloatingPointDomain(require_ni_zero=False),
            )
        self.assertIn("NI", str(ctx.exception))

    def test_engine_unsupported_opcode_under_ni_domain_inconclusive(self) -> None:
        # frsp f1, f1
        code = parse_hex("fc200018")
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=5000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            floating_point_domain=FloatingPointDomain(require_ni_zero=False),
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertTrue(any("NI" in item or "ni" in item.lower() for item in result.unsupported))

    def test_engine_supported_opcode_under_ni_domain_not_rejected_early(self) -> None:
        # fadds f1, f1, f2
        code = parse_hex("ec21102a")
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=10000)
        domain = FloatingPointDomain(require_ni_zero=False)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            floating_point_domain=domain,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        assert result.floating_point_domain is not None
        self.assertEqual(
            result.floating_point_domain.to_dict()["ni"],
            "modeled-flush-to-zero",
        )
        self.assertIn("fadds", result.floating_point_domain.ni_supported_opcodes())


class NiDomainCoverageTests(unittest.TestCase):
    def test_modeled_ni_coverage_labels(self) -> None:
        domain = FloatingPointDomain(require_ni_zero=False)
        domain.validate()  # must not reject
        coverage = domain.coverage_dict()
        self.assertEqual(coverage["proven"][:2], list(FP_COVERAGE_PROVEN_NI_MODELED)[:2])
        self.assertIn("ni-flush-to-zero", coverage["proven"])
        self.assertNotIn("ni-required-zero", coverage["proven"])
        self.assertIn("ni-estimates-converts-stores-non-oracle-paired", coverage["unsupported"])
        payload = domain.to_dict()
        self.assertEqual(payload["ni"], "modeled-flush-to-zero")
        self.assertEqual(payload["ni_supported_opcodes"], sorted(NI_SUPPORTED_OPS))

    def test_default_domain_still_requires_ni_zero(self) -> None:
        domain = FloatingPointDomain()
        self.assertTrue(domain.require_ni_zero)
        self.assertIn("ni-required-zero", domain.coverage_dict()["proven"])

    def test_tier_c_unchanged_with_ni_modeled(self) -> None:
        from tools.coop.lib.equivalence_policy import compute_confidence_tier
        from tools.coop.tests.test_promotion_policy import _equivalent_proof, _open_ledger

        proof = _equivalent_proof(
            floating_point_domain=FloatingPointDomain(require_ni_zero=False),
        )
        self.assertEqual(compute_confidence_tier(proof, _open_ledger()), "C")


class NiPairedOracleTests(unittest.TestCase):
    def test_ps_add_subnormal_operand_flushed(self) -> None:
        ieee = _run(
            Opcode.PS_ADD, (1, 1, 2),
            {
                "f1": f"0x{_SUB_S_FPR:016x}",
                "f2": f"0x{_SUB_S_FPR:016x}",
            },
            ni=False,
            domain=FloatingPointDomain(require_ni_zero=True),
        )
        # Seed both lanes (ps1 mirrors fpr for concrete_state unless set).
        state_ni = concrete_state({
            "fpr": {
                "f1": f"0x{_SUB_S_FPR:016x}",
                "f2": f"0x{_SUB_S_FPR:016x}",
            },
            "fpscr": FPSCR_NI,
        })
        # Ensure ps1 lanes also carry the subnormal.
        state_ni = state_ni.with_ps1(1, _SUB_S_FPR).with_ps1(2, _SUB_S_FPR)
        ni = execute_instruction(
            state_ni,
            _insn(Opcode.PS_ADD, (1, 1, 2)),
            ConcreteOps(),
            floating_point_domain=FloatingPointDomain(require_ni_zero=False),
        )
        self.assertTrue(ieee.valid)
        self.assertTrue(ni.valid)
        self.assertNotEqual(ieee.fpr[1], 0)
        self.assertEqual(ni.fpr[1], 0)
        self.assertEqual(ni.ps1[1], 0)


class NiTrapCompositionTests(unittest.TestCase):
    def test_ni_and_traps_supported_op_equivalence(self) -> None:
        # fadds under NI-modeled + traps_enabled domain must not fail closed.
        code = parse_hex("ec21102a")
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=10000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            floating_point_domain=FloatingPointDomain(
                require_ni_zero=False,
                traps_enabled=True,
            ),
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)

    def test_ni_unsupported_frsp_with_traps_inconclusive(self) -> None:
        code = parse_hex("fc200018")  # frsp f1, f1
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=5000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            floating_point_domain=FloatingPointDomain(
                require_ni_zero=False,
                traps_enabled=True,
            ),
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)


class NiFlushOracleUnitTests(unittest.TestCase):
    def test_operand_helper_passthrough(self) -> None:
        self.assertEqual(ni_flush_operand_binary64(_SUB_D, ni=False), _SUB_D)
        self.assertEqual(ni_flush_operand_binary64(_SUB_D, ni=True), 0)


if __name__ == "__main__":
    unittest.main()

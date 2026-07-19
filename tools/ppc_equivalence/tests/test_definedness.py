from __future__ import annotations

import unittest

from dataclasses import replace

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.model import InvalidReason, MachineState, concrete_state
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, execute_block, execute_instruction


def _insn(opcode: Opcode, operands: tuple[int, ...]) -> Instruction:
    return Instruction(0, 0, opcode, operands)


class InvalidReasonConcreteTests(unittest.TestCase):
    """Verify that concrete execution sets the correct InvalidReason."""

    def test_valid_state_has_reason_none(self):
        state = execute_instruction(
            concrete_state(), _insn(Opcode.ADDI, (3, 3, 4)), ConcreteOps(),
        )
        self.assertTrue(state.valid)
        self.assertEqual(state.invalid_reason, 0)

    def test_unaligned_lwz_sets_reason(self):
        state = execute_instruction(
            concrete_state({"gpr": {"r3": 0x101}}),
            _insn(Opcode.LWZ, (4, 3, 0)), ConcreteOps(),
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.UNALIGNED_ACCESS.value)

    def test_unaligned_stw_sets_reason(self):
        state = execute_instruction(
            concrete_state({"gpr": {"r3": 0x103}}),
            _insn(Opcode.STW, (4, 3, 0)), ConcreteOps(),
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.UNALIGNED_ACCESS.value)

    def test_aligned_access_keeps_reason_none(self):
        state = execute_instruction(
            concrete_state({"gpr": {"r3": 0x100}}),
            _insn(Opcode.LWZ, (4, 3, 0)), ConcreteOps(),
        )
        self.assertTrue(state.valid)
        self.assertEqual(state.invalid_reason, 0)

    def test_divide_by_zero_sets_reason(self):
        state = execute_instruction(
            concrete_state({"gpr": {"r4": 7, "r5": 0}}),
            _insn(Opcode.DIVWU, (3, 4, 5)), ConcreteOps(),
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.DIVIDE_UNDEFINED.value)

    def test_divide_signed_overflow_sets_reason(self):
        state = execute_instruction(
            concrete_state({"gpr": {"r4": 0x80000000, "r5": 0xFFFFFFFF}}),
            _insn(Opcode.DIVW, (3, 4, 5)), ConcreteOps(),
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.DIVIDE_UNDEFINED.value)

    def test_divide_valid_keeps_reason_none(self):
        state = execute_instruction(
            concrete_state({"gpr": {"r4": 12, "r5": 3}}),
            _insn(Opcode.DIVW, (3, 4, 5)), ConcreteOps(),
        )
        self.assertTrue(state.valid)
        self.assertEqual(state.invalid_reason, 0)

    def test_unaligned_via_decode_block(self):
        state = concrete_state({"gpr": {"r3": 0x101}})
        insns = decode_block(parse_hex("80830000"), 0x80000000)
        state = execute_block(state, insns, ConcreteOps())
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.UNALIGNED_ACCESS.value)

    def test_dcbz_cache_disabled_sets_reason(self):
        state = concrete_state({
            "gpr": {"r3": 0x100},
            "spr": {"hid0": 0},
        })
        insns = decode_block(parse_hex("7c001fec"), 0x80000000)  # dcbz r0,r3
        state = execute_block(state, insns, ConcreteOps())
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.CACHE_DISABLED.value)

    def test_dcbz_cache_enabled_keeps_valid(self):
        state = concrete_state({
            "gpr": {"r3": 0x100},
            "spr": {"hid0": 0x4000},  # HID0.DCE=1
        })
        insns = decode_block(parse_hex("7c001fec"), 0x80000000)  # dcbz r0,r3
        state = execute_block(state, insns, ConcreteOps())
        self.assertTrue(state.valid)

    def test_mfmsr_user_mode_sets_reason(self):
        state = concrete_state({"msr": 0x00004000})  # MSR.PR=1 (user mode)
        insns = decode_block(parse_hex("7c6000a6"), 0x80000000)  # mfmsr r3
        state = execute_block(state, insns, ConcreteOps())
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.PRIVILEGED_INSTRUCTION.value)

    def test_mfmsr_supervisor_keeps_valid(self):
        state = concrete_state({"msr": 0x00000000})  # MSR.PR=0 (supervisor)
        insns = decode_block(parse_hex("7c6000a6"), 0x80000000)
        state = execute_block(state, insns, ConcreteOps())
        self.assertTrue(state.valid)

    def test_mtsr_user_mode_sets_reason(self):
        state = concrete_state({"msr": 0x00004000})
        insns = decode_block(parse_hex("7c6001a4"), 0x80000000)  # mtsr r3, sr0
        state = execute_block(state, insns, ConcreteOps())
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.PRIVILEGED_INSTRUCTION.value)

    def test_mfspr_hid0_user_mode_sets_reason(self):
        state = execute_instruction(
            concrete_state({"msr": 0x00004000}),
            _insn(Opcode.MFSPR, (3, 1008)),
            ConcreteOps(),
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.PRIVILEGED_INSTRUCTION.value)

    def test_mfspr_hid0_supervisor_keeps_valid(self):
        state = execute_instruction(
            concrete_state({"msr": 0x00000000}),
            _insn(Opcode.MFSPR, (3, 1008)),
            ConcreteOps(),
        )
        self.assertTrue(state.valid)
        self.assertEqual(state.invalid_reason, 0)

    def test_mflr_user_mode_stays_valid(self):
        state = execute_instruction(
            concrete_state({"msr": 0x00004000, "lr": 0x80001000}),
            _insn(Opcode.MFSPR, (3, 8)),
            ConcreteOps(),
        )
        self.assertTrue(state.valid)
        self.assertEqual(state.gpr[3], 0x80001000)

    def test_fadds_finite_overflow_sets_fp_domain_reason(self):
        # binary64 encodings of large finite singles that overflow when added.
        huge = 0x47EFFFFFE0000000  # ~1.7e38 as expanded binary32
        state = execute_instruction(
            concrete_state({"fpr": {"f1": f"0x{huge:016x}", "f2": f"0x{huge:016x}"}}),
            _insn(Opcode.FADDS, (1, 1, 2)),
            ConcreteOps(),
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.FP_DOMAIN_EXCLUDED.value)

    def test_frsp_nonfinite_source_sets_fp_domain_reason(self):
        state = execute_instruction(
            concrete_state({"fpr": {"f1": "0x7ff0000000000000"}}),  # +Inf
            _insn(Opcode.FRSP, (1, 0, 1, 0)),
            ConcreteOps(),
        )
        self.assertFalse(state.valid)
        self.assertEqual(state.invalid_reason, InvalidReason.FP_DOMAIN_EXCLUDED.value)

    def test_rfi_user_mode_produces_program_exception(self):
        state = concrete_state({"msr": 0x00004000})
        insns = decode_block(parse_hex("4c000064"), 0x80000000)  # rfi
        from tools.ppc_equivalence.semantics import execute_cfg
        terminals = execute_cfg(state, insns, ConcreteOps())
        exception_terminals = [t for t in terminals if t.condition]
        self.assertTrue(exception_terminals)
        for t in exception_terminals:
            self.assertEqual(t.exit_kind, "program-exception")


class InvalidReasonSymbolicEquivalence(unittest.TestCase):
    """Verify symbolic equivalence respects invalid_reason comparison."""

    def test_both_same_invalid_reason_equivalent(self):
        code = parse_hex("80830000")  # lwz r4, 0(r3)
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["r4"], timeout_ms=10000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_both_same_divide_invalid_equivalent(self):
        code = parse_hex("7c641bd6")  # divwu r3, r4, r5
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=10000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_fp_domain_vs_privileged_spr_not_equivalent(self):
        """FP domain exit and privileged SPR must not collapse to the same reason.

        Before v20 both paths cleared ``valid`` while leaving ``invalid_reason``
        at 0.  Distinct first-failure codes keep those exits distinguishable.
        """
        from tools.ppc_equivalence.engine import _symbolic_initial, _terminal_difference
        from tools.ppc_equivalence.semantics import SymbolicOps, Terminal

        ops = SymbolicOps()
        z3 = ops.z3
        initial = _symbolic_initial(ops)
        left = Terminal(
            ops.bool(True),
            replace(
                initial,
                valid=ops.bool(False),
                invalid_reason=z3.BitVecVal(InvalidReason.FP_DOMAIN_EXCLUDED.value, 8),
                fpr=tuple(
                    z3.BitVecVal(0x7FF0000000000000, 64) if i == 1 else r
                    for i, r in enumerate(initial.fpr)
                ),
            ),
            "fallthrough",
            None,
        )
        right = Terminal(
            ops.bool(True),
            replace(
                initial,
                valid=ops.bool(False),
                invalid_reason=z3.BitVecVal(InvalidReason.PRIVILEGED_INSTRUCTION.value, 8),
            ),
            "fallthrough",
            None,
        )
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=10000)
        difference = _terminal_difference(left, right, contract, initial, ops)
        solver = z3.Solver()
        solver.add(difference)
        self.assertEqual(solver.check(), z3.sat)

    def test_identical_fadds_still_equivalent(self):
        insns = [_insn(Opcode.FADDS, (1, 1, 2))]
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=10000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex="00",
            candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)


class DefinednessSymmetricTests(unittest.TestCase):
    """Tests for definedness-preserving partial equivalence."""

    def test_valid_vs_invalid_not_equivalent(self):
        code_a = parse_hex("80830000")  # lwz r4, 0(r3)
        code_b = parse_hex("38630004")  # addi r3, r3, 4
        insns_a = decode_block(code_a, 0x80000000)
        insns_b = decode_block(code_b, 0x80000000)
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=10000)
        result = check_equivalence(
            insns_a, insns_b, contract,
            original_hex=code_a.hex(),
            candidate_hex=code_b.hex(),
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_both_valid_different_outputs_not_equivalent(self):
        code_a = parse_hex("38630004")  # addi r3, r3, 4
        code_b = parse_hex("38630005")  # addi r3, r3, 5
        insns_a = decode_block(code_a, 0x80000000)
        insns_b = decode_block(code_b, 0x80000000)
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=10000)
        result = check_equivalence(
            insns_a, insns_b, contract,
            original_hex=code_a.hex(),
            candidate_hex=code_b.hex(),
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_both_valid_identical_equivalent(self):
        code = parse_hex("38630004")  # addi r3, r3, 4
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=10000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)


class InvalidReasonEnums(unittest.TestCase):
    def test_enum_values(self):
        self.assertEqual(InvalidReason.NONE.value, 0)
        self.assertEqual(InvalidReason.UNALIGNED_ACCESS.value, 1)
        self.assertEqual(InvalidReason.DIVIDE_UNDEFINED.value, 2)
        self.assertEqual(InvalidReason.FP_DOMAIN_EXCLUDED.value, 3)
        self.assertEqual(InvalidReason.FP_ROUNDING_MODE.value, 4)
        self.assertEqual(InvalidReason.CACHE_DISABLED.value, 5)
        self.assertEqual(InvalidReason.PRIVILEGED_INSTRUCTION.value, 6)
        self.assertEqual(InvalidReason.PSQ_INVALID_TYPE.value, 7)
        self.assertEqual(InvalidReason.PSQ_NONFINITE_INTEGER_STORE.value, 8)
        self.assertEqual(InvalidReason.UNSUPPORTED_SIDE_EFFECT.value, 9)
        self.assertEqual(InvalidReason.MEMORY_PROFILE_VIOLATION.value, 10)

    def test_enum_names_unique(self):
        names = [m.name for m in InvalidReason]
        self.assertEqual(len(names), len(set(names)))


if __name__ == "__main__":
    unittest.main()

"""Bounded back-edge / constant-trip loop support (architecture v21)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=link)


def _ctr_counted_loop(*, count: int, addend: int = 1) -> list[Instruction]:
    """li r0,count; mtctr r0; loop: addi r3,r3,addend; bdnz loop; blr"""
    return [
        _insn(Opcode.ADDI, (0, 0, count), address=0),
        _insn(Opcode.MTSPR, (0, 9), address=4),
        _insn(Opcode.ADDI, (3, 3, addend), address=8),
        _insn(Opcode.BC, (16, 0, 8, 0), address=12),
        _insn(Opcode.BCLR, (20, 0, 0), address=16),
    ]


class BoundedLoopConcreteTests(unittest.TestCase):
    def test_constant_ctr_loop_executes(self) -> None:
        program = _ctr_counted_loop(count=3)
        # LR must be outside the block so blr is a return, not a jump to entry.
        terminals = [
            t for t in execute_cfg(
                concrete_state({"gpr": {"r3": 10}, "lr": 0x80001000}),
                program,
                ConcreteOps(),
            )
            if t.condition
        ]
        self.assertEqual(len(terminals), 1)
        self.assertEqual(terminals[0].exit_kind, "return")
        self.assertEqual(terminals[0].state.gpr[3], 13)
        self.assertEqual(terminals[0].state.ctr, 0)

    def test_infinite_loop_hits_iteration_limit(self) -> None:
        program = [_insn(Opcode.B, (0, 0), address=0)]
        with self.assertRaisesRegex(ExecutionInconclusive, "loop iteration limit exceeded"):
            execute_cfg(
                concrete_state(), program, ConcreteOps(), max_loop_iterations=4,
            )

    def test_max_loop_iterations_one_rejects_any_back_edge(self) -> None:
        program = [_insn(Opcode.B, (0, 0), address=0)]
        with self.assertRaisesRegex(ExecutionInconclusive, "loop iteration limit exceeded \\(1\\)"):
            execute_cfg(
                concrete_state(), program, ConcreteOps(), max_loop_iterations=1,
            )

    def test_ctr_loop_exhausts_when_bound_too_small(self) -> None:
        program = _ctr_counted_loop(count=5)
        with self.assertRaisesRegex(ExecutionInconclusive, "loop iteration limit exceeded"):
            execute_cfg(
                concrete_state({"gpr": {"r3": 0}, "lr": 0x80001000}),
                program,
                ConcreteOps(),
                max_loop_iterations=3,
            )


class BoundedLoopSymbolicTests(unittest.TestCase):
    def test_identical_constant_ctr_loops_equivalent(self) -> None:
        program = _ctr_counted_loop(count=4)
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=10_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertEqual(result.limits.get("max_loop_iterations"), 256)

    def test_different_loop_bodies_not_equivalent(self) -> None:
        left = _ctr_counted_loop(count=3, addend=1)
        right = _ctr_counted_loop(count=3, addend=2)
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=10_000)
        result = check_equivalence(
            left, right, contract,
            original_hex="00", candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_iteration_limit_is_inconclusive_not_truncated(self) -> None:
        program = [_insn(Opcode.B, (0, 0), address=0)]
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=8,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertTrue(any("loop iteration limit" in item for item in result.unsupported))
        self.assertEqual(result.limits.get("max_loop_iterations"), 8)


if __name__ == "__main__":
    unittest.main()

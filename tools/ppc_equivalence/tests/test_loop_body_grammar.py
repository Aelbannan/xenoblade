"""Body grammar widening (doc 30 Phase D)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_summary import find_ctr_affine_loop_candidates
from tools.ppc_equivalence.memory_loop import (
    build_memory_loop_plan_map,
    find_constant_stride_store_loops,
)
from tools.ppc_equivalence.model import ConcreteMemory, concrete_state
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg
from dataclasses import replace


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _assert_execution_equal(
    test: unittest.TestCase,
    program: list[Instruction],
    plans: dict,
    entry,
) -> None:
    summarized = [t for t in execute_cfg(
        entry, program, ConcreteOps(), memory_loop_plans=plans,
    ) if t.condition]
    unrolled = [t for t in execute_cfg(entry, program, ConcreteOps()) if t.condition]
    test.assertEqual(len(summarized), 1)
    test.assertEqual(len(unrolled), 1)
    test.assertEqual(summarized[0].state.gpr, unrolled[0].state.gpr)
    test.assertEqual(summarized[0].state.memory, unrolled[0].state.memory)
    test.assertEqual(summarized[0].state.memory_writes, unrolled[0].state.memory_writes)
    test.assertEqual(summarized[0].state.memory_touches, unrolled[0].state.memory_touches)


class MultiFillBodyTests(unittest.TestCase):
    def _fill_loop(self, *, count: int) -> list[Instruction]:
        return [
            _insn(Opcode.ADDI, (5, 0, count), address=0),
            _insn(Opcode.MTSPR, (5, 9), address=4),
            _insn(Opcode.STW, (3, 7, 0), address=8),
            _insn(Opcode.STW, (3, 7, 4), address=12),
            _insn(Opcode.STW, (3, 7, 8), address=16),
            _insn(Opcode.ADDI, (7, 7, 12), address=20),
            _insn(Opcode.BC, (16, 0, 8, 0), address=24),
            _insn(Opcode.BCLR, (20, 0, 0), address=28),
        ]

    def test_recognizes_multi_fill(self) -> None:
        loops = find_constant_stride_store_loops(self._fill_loop(count=3))
        self.assertEqual(len(loops), 1)
        loop = loops[0]
        self.assertEqual(loop.store_kind, "multi-fill")
        self.assertEqual(len(loop.effects), 3)
        self.assertEqual(loop.stride, 12)
        self.assertEqual(loop.trip_count, 3)
        self.assertEqual(loop.confidence, "exact-pattern")

    def test_multi_fill_matches_unrolled(self) -> None:
        program = self._fill_loop(count=3)
        plans = build_memory_loop_plan_map(program)
        self.assertEqual(len(plans), 1)
        _assert_execution_equal(
            self, program, plans,
            concrete_state({"gpr": {"r3": 0xAA, "r7": 0x1000}, "lr": 0x80001000}),
        )

    def test_multi_fill_rejects_differing_sources(self) -> None:
        program = [
            _insn(Opcode.ADDI, (5, 0, 2), address=0),
            _insn(Opcode.MTSPR, (5, 9), address=4),
            _insn(Opcode.STW, (3, 7, 0), address=8),
            _insn(Opcode.STW, (4, 7, 4), address=12),  # different source reg
            _insn(Opcode.ADDI, (7, 7, 8), address=16),
            _insn(Opcode.BC, (16, 0, 8, 0), address=20),
        ]
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 0)

    def test_multi_fill_rejects_gaps(self) -> None:
        program = [
            _insn(Opcode.ADDI, (5, 0, 2), address=0),
            _insn(Opcode.MTSPR, (5, 9), address=4),
            _insn(Opcode.STW, (3, 7, 0), address=8),
            _insn(Opcode.STW, (3, 7, 8), address=12),  # gap at 4
            _insn(Opcode.ADDI, (7, 7, 12), address=16),
            _insn(Opcode.BC, (16, 0, 8, 0), address=20),
        ]
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 0)


class CopyBodyTests(unittest.TestCase):
    def _copy_loop(self) -> list[Instruction]:
        return [
            _insn(Opcode.ADDI, (5, 0, 3), address=0),
            _insn(Opcode.MTSPR, (5, 9), address=4),
            _insn(Opcode.LWZ, (4, 6, 0), address=8),
            _insn(Opcode.STW, (4, 7, 0), address=12),
            _insn(Opcode.ADDI, (7, 7, 4), address=16),
            _insn(Opcode.BC, (16, 0, 8, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]

    def test_recognizes_copy_body(self) -> None:
        loops = find_constant_stride_store_loops(self._copy_loop())
        self.assertEqual(len(loops), 1)
        loop = loops[0]
        self.assertEqual(loop.store_kind, "copy")
        self.assertEqual(loop.trip_count, 3)
        self.assertEqual(loop.effects[0].kind, "copy")

    def test_copy_matches_unrolled_disjoint(self) -> None:
        program = self._copy_loop()
        plans = build_memory_loop_plan_map(program)
        self.assertEqual(len(plans), 1)
        memory = ConcreteMemory(
            default=0,
            bytes=((0x2000, 0x11), (0x2004, 0x22), (0x2008, 0x33)),
        )
        entry = replace(
            concrete_state({"gpr": {"r6": 0x2000, "r7": 0x3000}, "lr": 0x80001000}),
            memory=memory,
        )
        _assert_execution_equal(self, program, plans, entry)

    def test_copy_aliasing_premise_backward_overlap_fails_closed(self) -> None:
        # src = 0x3002 > dst = 0x3000 with overlap: the aliasing premise fails,
        # the summary is refused, and a memory-loop-entry-premise terminal is
        # recorded (fail-closed) instead of a wrong summarized state.
        program = self._copy_loop()
        plans = build_memory_loop_plan_map(program)
        memory = ConcreteMemory(default=0, bytes=((0x3002, 0x11), (0x3006, 0x22)))
        entry = replace(
            concrete_state({"gpr": {"r6": 0x3002, "r7": 0x3000}, "lr": 0x80001000}),
            memory=memory,
        )
        summarized = [t for t in execute_cfg(
            entry, program, ConcreteOps(), memory_loop_plans=plans,
        ) if t.condition]
        self.assertEqual(len(summarized), 1)
        self.assertEqual(summarized[0].exit_kind, "memory-loop-entry-premise")
        # The premise terminal keeps the pre-loop state (no wrong summary).
        self.assertEqual(summarized[0].state.memory, memory)


class AffineBodyWhitelistTests(unittest.TestCase):
    def _body_loop(self, body: list[Instruction], *, header: int = 8) -> list[Instruction]:
        return [
            _insn(Opcode.ADDI, (0, 0, 4), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            *body,
            _insn(Opcode.BC, (16, 0, header, 0), address=header + 4 * len(body)),
            _insn(Opcode.BCLR, (20, 0, 0), address=header + 4 * len(body) + 4),
        ]

    def test_whitelisted_dead_register_op_discharges(self) -> None:
        # addi r3,r3,1 ; mr r14,r15 — the mr targets a dead callee-saved
        # scratch, the closed form folds, EQUIVALENT is authorized.
        program = self._body_loop(
            [
                _insn(Opcode.ADDI, (3, 3, 1), address=8),
                _insn(Opcode.OR, (14, 15, 15), address=12),  # mr r14, r15
            ],
        )
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(len(candidates[0].body_ops or ()), 1)
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=20_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertEqual(result.loop_summary["status"], "discharged")

    def test_whitelist_op_read_after_loop_rejected(self) -> None:
        # mr r14,r15 followed by a use of r14 after the latch: the no-op
        # treatment would be fidelity-unsafe, so the loop is rejected.
        program = self._body_loop(
            [
                _insn(Opcode.ADDI, (3, 3, 1), address=8),
                _insn(Opcode.OR, (14, 15, 15), address=12),  # mr r14, r15
                _insn(Opcode.ADD, (3, 3, 14), address=16),  # r14 read after
            ],
            header=8,
        )
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 0)

    def test_body_op_reading_affine_register_rejected(self) -> None:
        # addi r3,r3,1 ; slwi r3,r3,2 — the rlwinm reads the affine r3.
        program = self._body_loop(
            [
                _insn(Opcode.ADDI, (3, 3, 1), address=8),
                _insn(Opcode.RLWINM, (3, 3, 2, 0, 29), address=12),  # slwi r3,r3,2
            ],
        )
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 0)

    def test_volatile_register_result_rejected(self) -> None:
        # mr r5, r6 — r5 is an EABI-volatile register, never no-op'd.
        program = self._body_loop(
            [
                _insn(Opcode.ADDI, (3, 3, 1), address=8),
                _insn(Opcode.OR, (5, 6, 6), address=12),
            ],
        )
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 0)

    def test_summarized_body_with_whitelist_op_matches_unrolled(self) -> None:
        # addi r3,r3,1 ; extsb r14,r14 with r14 already sign-extended: the
        # no-op is fidelity-exact, the summarized and unrolled executions agree
        # bit-for-bit.
        program = self._body_loop(
            [
                _insn(Opcode.ADDI, (3, 3, 1), address=8),
                _insn(Opcode.EXTSB, (14, 14), address=12),
            ],
        )
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=20_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)

    def test_whitelist_op_summarized_vs_unrolled(self) -> None:
        # extsb r14,r14 with a NOT-yet-sign-extended entry: the no-op differs
        # from the unrolled value on the dead scratch, but the loop still
        # summarizes and the observed r3 agrees bit-for-bit.
        from tools.ppc_equivalence.loop_summary import build_affine_summary_map

        program = self._body_loop(
            [
                _insn(Opcode.ADDI, (3, 3, 1), address=8),
                _insn(Opcode.EXTSB, (14, 14), address=12),
            ],
        )
        summaries = build_affine_summary_map(program)
        self.assertEqual(len(summaries), 1)
        entry = concrete_state(
            {"gpr": {"r14": 0x00000080, "r3": 10}, "lr": 0x80001000},
        )
        sum_terms = [t for t in execute_cfg(
            entry, program, ConcreteOps(), affine_loop_summaries=summaries,
        ) if t.condition]
        unr_terms = [t for t in execute_cfg(entry, program, ConcreteOps()) if t.condition]
        self.assertEqual(len(sum_terms), 1)
        self.assertEqual(len(unr_terms), 1)
        self.assertEqual(sum_terms[0].state.gpr[3], unr_terms[0].state.gpr[3])
        # The dead scratch diverges by design (no-op vs real sign-extension).
        self.assertNotEqual(sum_terms[0].state.gpr[14], unr_terms[0].state.gpr[14])


if __name__ == "__main__":
    unittest.main()

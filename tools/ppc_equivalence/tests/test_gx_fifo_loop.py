"""GX FIFO CTR loop recognition and closed-form emission tests."""

from __future__ import annotations

import unittest
from dataclasses import replace

from tools.ppc_equivalence.gx_fifo_loop import (
    DEFAULT_GX_BASE,
    GxFifoLoopCandidate,
    apply_gx_fifo_loop_summary,
    build_gx_fifo_loop_plan,
    build_gx_fifo_loop_plans,
    compute_summary_identity_sha256,
    compute_witness_code_sha256,
    recognize_gx_fifo_loops,
)
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=link)


def _materialize_gx_base(reg: int, *, address: int = 0) -> list[Instruction]:
    """``addis reg,0,0xCC00; ori reg,reg,0x8000`` -> ``reg = 0xCC00_8000``."""
    return [
        _insn(Opcode.ADDIS, (reg, 0, 0xCC00), address=address),
        _insn(Opcode.ORI, (reg, reg, 0x8000), address=address + 4),
    ]


def _gx_fifo_program(
    *,
    count: int,
    store: tuple[Opcode, int],  # (opcode, source_reg); base reg is fixed at 5
    affine_step: int | None = None,
    base_reg: int = 5,
    extra_body: list[Instruction] | None = None,
) -> list[Instruction]:
    """base-materialize; li count; mtctr; header:[store][affine addi]; bdnz; blr."""
    prologue = _materialize_gx_base(base_reg)
    pc = len(prologue) * 4
    prologue.append(_insn(Opcode.ADDI, (0, 0, count), address=pc))
    pc += 4
    prologue.append(_insn(Opcode.MTSPR, (0, 9), address=pc))
    pc += 4

    header_pc = pc
    opcode, source_reg = store
    body = [_insn(opcode, (source_reg, base_reg, 0), address=pc)]
    pc += 4
    if affine_step is not None:
        body.append(_insn(Opcode.ADDI, (source_reg, source_reg, affine_step), address=pc))
        pc += 4
    if extra_body is not None:
        for insn in extra_body:
            body.append(replace(insn, address=pc))
            pc += 4

    latch_pc = pc
    return [
        *prologue,
        *body,
        _insn(Opcode.BC, (16, 0, header_pc, 0), address=latch_pc),
        _insn(Opcode.BCLR, (20, 0, 0), address=latch_pc + 4),
    ]


class RecognizeGxFifoLoopTests(unittest.TestCase):
    def test_recognizes_simple_stw_to_gx_with_concrete_ctr(self) -> None:
        program = _gx_fifo_program(count=4, store=(Opcode.STW, 3))
        candidates = recognize_gx_fifo_loops(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertIsInstance(candidate, GxFifoLoopCandidate)
        self.assertEqual(candidate.confidence, "exact-pattern")
        self.assertEqual(candidate.trip_count, 4)
        self.assertEqual(candidate.address, DEFAULT_GX_BASE)
        self.assertEqual(candidate.width, 4)
        self.assertEqual(candidate.writes_per_iteration, 1)
        self.assertEqual(candidate.base_reg, 5)
        self.assertEqual(candidate.source_reg, 3)
        self.assertEqual(candidate.value_kind, "invariant")

    def test_recognizes_stb_and_sth_widths(self) -> None:
        for opcode, width in ((Opcode.STB, 1), (Opcode.STH, 2)):
            with self.subTest(opcode=opcode.value):
                program = _gx_fifo_program(count=2, store=(opcode, 4))
                candidates = recognize_gx_fifo_loops(program)
                self.assertEqual(len(candidates), 1)
                self.assertEqual(candidates[0].width, width)

    def test_recognizes_affine_value_with_trailing_addi(self) -> None:
        program = _gx_fifo_program(count=5, store=(Opcode.STW, 3), affine_step=1)
        candidates = recognize_gx_fifo_loops(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.value_kind, "affine")
        self.assertEqual(candidate.value_step, 1)

    def test_builds_plan_map_keyed_by_header_pc(self) -> None:
        program = _gx_fifo_program(count=4, store=(Opcode.STW, 3))
        plans = build_gx_fifo_loop_plans(program)
        candidates = recognize_gx_fifo_loops(program)
        header_pc = candidates[0].header_pc
        self.assertIn(header_pc, plans)
        plan = plans[header_pc]
        self.assertEqual(plan.summary.trip_count, 4)
        self.assertEqual(plan.summary.address, DEFAULT_GX_BASE)
        self.assertEqual(plan.summary.writes_per_iteration, 1)
        self.assertEqual(plan.summary.proof_kind, "gx-fifo-loop-emission-v1")
        self.assertEqual(plan.witness.recognized_trip_count, 4)
        self.assertEqual(len(plan.witness.code_sha256), 64)

    def test_build_plan_none_when_trip_count_unknown(self) -> None:
        program = [
            *_materialize_gx_base(5),
            _insn(Opcode.MTSPR, (7, 9), address=8),  # unresolved trip reg r7
            _insn(Opcode.STW, (3, 5, 0), address=12),
            _insn(Opcode.BC, (16, 0, 12, 0), address=16),
            _insn(Opcode.BCLR, (20, 0, 0), address=20),
        ]
        candidates = recognize_gx_fifo_loops(program)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].confidence, "partial")
        self.assertIsNone(build_gx_fifo_loop_plan(candidates[0], program))
        self.assertEqual(build_gx_fifo_loop_plans(program), {})

    def test_never_raises_on_empty_or_unrelated(self) -> None:
        self.assertEqual(recognize_gx_fifo_loops([]), [])
        self.assertEqual(
            recognize_gx_fifo_loops([_insn(Opcode.BCLR, (20, 0, 0), address=0)]),
            [],
        )


class RejectGxFifoLoopTests(unittest.TestCase):
    def test_rejects_loop_with_additional_ram_store(self) -> None:
        """A second store (to RAM, not an affine addi) must reject the whole body."""
        program = _gx_fifo_program(
            count=2,
            store=(Opcode.STW, 3),
            extra_body=[_insn(Opcode.STW, (3, 6, 0))],  # RAM store via r6
        )
        self.assertEqual(recognize_gx_fifo_loops(program), [])

    def test_rejects_loop_with_load_from_gx(self) -> None:
        """A FIFO read inside the body must reject (GX FIFO reads are unsupported)."""
        program = _gx_fifo_program(
            count=2,
            store=(Opcode.STW, 3),
            extra_body=[_insn(Opcode.LWZ, (4, 5, 0))],  # load from GX base
        )
        self.assertEqual(recognize_gx_fifo_loops(program), [])

    def test_rejects_pure_load_only_body(self) -> None:
        program = [
            *_materialize_gx_base(5),
            _insn(Opcode.ADDI, (0, 0, 2), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.LWZ, (4, 5, 0), address=16),
            _insn(Opcode.BC, (16, 0, 16, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]
        self.assertEqual(recognize_gx_fifo_loops(program), [])

    def test_rejects_store_outside_gx_region(self) -> None:
        """Store base resolves concretely, but the address lands in RAM."""
        program = [
            _insn(Opcode.ADDI, (6, 0, 0x1000), address=0),
            _insn(Opcode.ADDI, (0, 0, 2), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
            _insn(Opcode.STW, (3, 6, 0), address=12),
            _insn(Opcode.BC, (16, 0, 12, 0), address=16),
            _insn(Opcode.BCLR, (20, 0, 0), address=20),
        ]
        self.assertEqual(recognize_gx_fifo_loops(program), [])

    def test_rejects_unresolved_base_register(self) -> None:
        """Store base is a bare parameter register with no materialization."""
        program = [
            _insn(Opcode.ADDI, (0, 0, 2), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.STW, (3, 31, 0), address=8),
            _insn(Opcode.BC, (16, 0, 8, 0), address=12),
            _insn(Opcode.BCLR, (20, 0, 0), address=16),
        ]
        self.assertEqual(recognize_gx_fifo_loops(program), [])

    def test_rejects_stwu(self) -> None:
        """``stwu`` would move the base register — the FIFO address must stay fixed."""
        program = [
            *_materialize_gx_base(5),
            _insn(Opcode.ADDI, (0, 0, 2), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.STWU, (3, 5, 0), address=16),
            _insn(Opcode.BC, (16, 0, 16, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]
        self.assertEqual(recognize_gx_fifo_loops(program), [])

    def test_rejects_source_equals_base(self) -> None:
        program = _gx_fifo_program(count=2, store=(Opcode.STW, 5))
        self.assertEqual(recognize_gx_fifo_loops(program), [])

    def test_mtctr_zero_is_unsupported_not_summarized(self) -> None:
        program = _gx_fifo_program(count=0, store=(Opcode.STW, 3))
        candidates = recognize_gx_fifo_loops(program)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].confidence, "unsupported")
        self.assertIsNone(build_gx_fifo_loop_plan(candidates[0], program))

    def test_rejects_when_bound_exceeded(self) -> None:
        program = _gx_fifo_program(count=10, store=(Opcode.STW, 3))
        candidates = recognize_gx_fifo_loops(program, max_events=4)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].trip_count, 10)
        self.assertIsNone(build_gx_fifo_loop_plan(candidates[0], program))
        self.assertEqual(build_gx_fifo_loop_plans(program, max_events=4), {})


class HashStabilityTests(unittest.TestCase):
    def test_witness_hash_stable_and_sensitive(self) -> None:
        program = _gx_fifo_program(count=4, store=(Opcode.STW, 3))
        plans = build_gx_fifo_loop_plans(program)
        header_pc = next(iter(plans))
        plan = plans[header_pc]

        again = compute_witness_code_sha256(plan.witness.body, plan.witness.latch, plan.witness.mtctr)
        self.assertEqual(plan.witness.code_sha256, again)

        mutated_body = tuple(
            replace(insn, operands=(9, 5, 0)) if index == 0 else insn
            for index, insn in enumerate(plan.witness.body)
        )
        mutated_hash = compute_witness_code_sha256(
            mutated_body, plan.witness.latch, plan.witness.mtctr,
        )
        self.assertNotEqual(plan.witness.code_sha256, mutated_hash)

    def test_summary_hash_stable_and_sensitive(self) -> None:
        program = _gx_fifo_program(count=4, store=(Opcode.STW, 3))
        plans = build_gx_fifo_loop_plans(program)
        header_pc = next(iter(plans))
        plan = plans[header_pc]

        digest = compute_summary_identity_sha256(plan.summary)
        self.assertEqual(len(digest), 64)
        self.assertEqual(digest, compute_summary_identity_sha256(plan.summary))

        mutated = replace(plan.summary, trip_count=5)
        self.assertNotEqual(digest, compute_summary_identity_sha256(mutated))

        mutated_addr = replace(plan.summary, address=plan.summary.address + 4)
        self.assertNotEqual(digest, compute_summary_identity_sha256(mutated_addr))

    def test_different_trip_counts_produce_different_summary_hashes(self) -> None:
        # The trip-count materialization (``li``) lives before ``mtctr`` and is
        # not part of the witness fingerprint (mirrors ``memory_loop.py``); the
        # recovered value is instead carried by the summary/witness fields, so
        # only the *summary* identity distinguishes count=4 from count=6 here.
        program_a = _gx_fifo_program(count=4, store=(Opcode.STW, 3))
        program_b = _gx_fifo_program(count=6, store=(Opcode.STW, 3))
        plan_a = next(iter(build_gx_fifo_loop_plans(program_a).values()))
        plan_b = next(iter(build_gx_fifo_loop_plans(program_b).values()))
        self.assertNotEqual(
            compute_summary_identity_sha256(plan_a.summary),
            compute_summary_identity_sha256(plan_b.summary),
        )

    def test_different_store_shapes_produce_different_witness_hashes(self) -> None:
        program_a = _gx_fifo_program(count=4, store=(Opcode.STW, 3))
        program_b = _gx_fifo_program(count=4, store=(Opcode.STB, 3))
        plan_a = next(iter(build_gx_fifo_loop_plans(program_a).values()))
        plan_b = next(iter(build_gx_fifo_loop_plans(program_b).values()))
        self.assertNotEqual(plan_a.witness.code_sha256, plan_b.witness.code_sha256)


class ApplyGxFifoLoopSummaryTests(unittest.TestCase):
    def test_apply_invariant_sets_ctr_and_builds_repeated_emission(self) -> None:
        from tools.ppc_equivalence.symbolic_event_trace import RepeatedEmission

        program = _gx_fifo_program(count=4, store=(Opcode.STW, 3))
        plan = next(iter(build_gx_fifo_loop_plans(program).values()))
        state = concrete_state({"gpr": {"r3": 0x11223344, "r5": DEFAULT_GX_BASE}})
        next_state, descriptor = apply_gx_fifo_loop_summary(state, plan, ConcreteOps())

        self.assertEqual(next_state.ctr, 0)
        self.assertEqual(next_state.gpr[5], DEFAULT_GX_BASE)  # base untouched
        self.assertEqual(next_state.gpr[3], 0x11223344)  # invariant value untouched
        self.assertIsInstance(descriptor, RepeatedEmission)
        self.assertEqual(descriptor.count, 4)
        self.assertEqual(descriptor.writes_per_iteration, 1)
        self.assertEqual(len(descriptor.writes), 1)
        self.assertEqual(descriptor.writes[0].addr, DEFAULT_GX_BASE)
        self.assertEqual(descriptor.writes[0].width, 4)
        self.assertEqual(descriptor.writes[0].value, 0x11223344)

    def test_apply_affine_advances_source_register_and_defers_template(self) -> None:
        program = _gx_fifo_program(count=3, store=(Opcode.STW, 3), affine_step=2)
        plan = next(iter(build_gx_fifo_loop_plans(program).values()))
        state = concrete_state({"gpr": {"r3": 10, "r5": DEFAULT_GX_BASE}})
        next_state, descriptor = apply_gx_fifo_loop_summary(state, plan, ConcreteOps())

        self.assertEqual(next_state.ctr, 0)
        self.assertEqual(next_state.gpr[3], 10 + 2 * 3)
        self.assertIsInstance(descriptor, dict)
        self.assertEqual(descriptor["kind"], "affine-unsupported-template")
        self.assertEqual(descriptor["step"], 2)
        self.assertEqual(descriptor["initial_value"], 10)

    def test_apply_calls_symbolic_bus_append_fn(self) -> None:
        program = _gx_fifo_program(count=2, store=(Opcode.STW, 3))
        plan = next(iter(build_gx_fifo_loop_plans(program).values()))
        state = concrete_state({"gpr": {"r3": 7, "r5": DEFAULT_GX_BASE}})
        seen = []
        apply_gx_fifo_loop_summary(
            state, plan, ConcreteOps(), symbolic_bus_append_fn=seen.append,
        )
        self.assertEqual(len(seen), 1)


if __name__ == "__main__":
    unittest.main()

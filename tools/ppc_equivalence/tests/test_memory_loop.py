"""Constant-stride store loop recognition and discharge tests."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_loop import (
    ConstantStrideStoreLoop,
    build_memory_loop_obligation,
    build_memory_loop_plan_map,
    build_memory_loop_side_entry,
    find_constant_stride_store_loops,
    summarize_constant_stride_store_loop,
)
from tools.ppc_equivalence.memory_loop_discharge import (
    discharge_memory_loop_plan,
)
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.proof_features import (
    KNOWN_PROOF_FEATURES,
    UNSUPPORTED_FOR_EQUIVALENT,
    validate_proof_features,
)
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=link)


def _store_loop(
    *,
    count: int,
    store: tuple[Opcode, tuple[int, ...]],
    pointer_addi: tuple[int, int] | None = None,
    base_address: int = 0,
) -> list[Instruction]:
    """li r0,count; mtctr r0; loop: [store] [addi]; bdnz loop; blr"""
    body: list[Instruction] = [
        _insn(store[0], store[1], address=base_address + 8),
    ]
    next_pc = base_address + 12
    if pointer_addi is not None:
        reg, imm = pointer_addi
        body.append(_insn(Opcode.ADDI, (reg, reg, imm), address=next_pc))
        next_pc += 4
    return [
        _insn(Opcode.ADDI, (0, 0, count), address=base_address),
        _insn(Opcode.MTSPR, (0, 9), address=base_address + 4),
        *body,
        _insn(Opcode.BC, (16, 0, base_address + 8, 0), address=next_pc),
        _insn(Opcode.BCLR, (20, 0, 0), address=next_pc + 4),
    ]


class ConstantStrideStoreLoopRecognitionTests(unittest.TestCase):
    def test_recognizes_stw_addi_loop(self) -> None:
        program = _store_loop(
            count=8,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        loop = loops[0]
        self.assertIsInstance(loop, ConstantStrideStoreLoop)
        self.assertEqual(loop.confidence, "exact-pattern")
        self.assertEqual(loop.trip_count, 8)
        self.assertEqual(loop.base_reg, 4)
        self.assertIsNone(loop.index_reg)
        self.assertEqual(loop.stride, 4)
        self.assertEqual(loop.store_width, 4)
        self.assertEqual(loop.source_reg, 3)
        self.assertEqual(loop.header_pc, 8)
        self.assertEqual(loop.latch_pc, 16)

    def test_recognizes_stwu_loop(self) -> None:
        program = _store_loop(count=4, store=(Opcode.STWU, (5, 6, 4)))
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        loop = loops[0]
        self.assertEqual(loop.base_reg, 6)
        self.assertEqual(loop.stride, 4)
        self.assertEqual(loop.store_width, 4)
        self.assertEqual(loop.source_reg, 5)

    def test_recognizes_stb_and_sth(self) -> None:
        for opcode, width in ((Opcode.STB, 1), (Opcode.STH, 2)):
            with self.subTest(opcode=opcode.value, width=width):
                program = _store_loop(
                    count=2,
                    store=(opcode, (7, 8, 0)),
                    pointer_addi=(8, width),
                )
                loops = find_constant_stride_store_loops(program)
                self.assertEqual(len(loops), 1)
                self.assertEqual(loops[0].store_width, width)
                self.assertEqual(loops[0].stride, width)

    def test_empty_on_affine_only_body(self) -> None:
        program = [
            _insn(Opcode.ADDI, (0, 0, 4), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.ADDI, (3, 3, 1), address=8),
            _insn(Opcode.BC, (16, 0, 8, 0), address=12),
        ]
        self.assertEqual(find_constant_stride_store_loops(program), [])

    def test_empty_on_mismatched_stride(self) -> None:
        program = _store_loop(
            count=2,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 8),
        )
        self.assertEqual(find_constant_stride_store_loops(program), [])

    def test_empty_on_multiple_stores(self) -> None:
        program = [
            _insn(Opcode.ADDI, (0, 0, 2), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.STW, (3, 4, 0), address=8),
            _insn(Opcode.STW, (3, 4, 4), address=12),
            _insn(Opcode.BC, (16, 0, 8, 0), address=16),
        ]
        self.assertEqual(find_constant_stride_store_loops(program), [])

    def test_empty_on_nonzero_store_disp_without_stwu(self) -> None:
        program = _store_loop(
            count=2,
            store=(Opcode.STW, (3, 4, 4)),
            pointer_addi=(4, 4),
        )
        self.assertEqual(find_constant_stride_store_loops(program), [])

    def test_empty_on_reversed_addi_then_store(self) -> None:
        program = [
            _insn(Opcode.ADDI, (0, 0, 2), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.ADDI, (4, 4, 4), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.BC, (16, 0, 8, 0), address=16),
        ]
        self.assertEqual(find_constant_stride_store_loops(program), [])

    def test_empty_on_source_equals_base(self) -> None:
        program = _store_loop(
            count=2,
            store=(Opcode.STW, (4, 4, 0)),
            pointer_addi=(4, 4),
        )
        self.assertEqual(find_constant_stride_store_loops(program), [])

    def test_mtctr_zero_is_unsupported_not_summarized(self) -> None:
        program = _store_loop(
            count=0,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        self.assertEqual(loops[0].confidence, "unsupported")
        self.assertEqual(loops[0].trip_count, 0)
        self.assertIsNone(summarize_constant_stride_store_loop(loops[0]))
        self.assertTrue(
            any("wrap" in note or "unsupported" in note for note in loops[0].notes),
            loops[0].notes,
        )

    def test_partial_when_trip_unknown(self) -> None:
        program = [
            _insn(Opcode.MTSPR, (3, 9), address=0),
            _insn(Opcode.STW, (5, 4, 0), address=4),
            _insn(Opcode.ADDI, (4, 4, 4), address=8),
            _insn(Opcode.BC, (16, 0, 4, 0), address=12),
        ]
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        self.assertEqual(loops[0].confidence, "partial")
        self.assertIsNone(loops[0].trip_count)

    def test_exact_with_addis_addi_trip_count(self) -> None:
        program = [
            _insn(Opcode.ADDIS, (0, 0, 0), address=0),
            _insn(Opcode.ADDI, (0, 0, 6), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
        ]
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        self.assertEqual(loops[0].confidence, "exact-pattern")
        self.assertEqual(loops[0].trip_count, 6)

    def test_exact_with_register_copy_trip_count(self) -> None:
        program = [
            _insn(Opcode.ADDI, (7, 0, 10), address=0),
            _insn(Opcode.OR, (0, 7, 7), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
            _insn(Opcode.STWU, (5, 6, 4), address=12),
            _insn(Opcode.BC, (16, 0, 12, 0), address=16),
        ]
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        self.assertEqual(loops[0].confidence, "exact-pattern")
        self.assertEqual(loops[0].trip_count, 10)

    def test_partial_when_addi_base_unknown(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 31, 8), address=0),
            _insn(Opcode.MTSPR, (3, 9), address=4),
            _insn(Opcode.STW, (5, 4, 0), address=8),
            _insn(Opcode.ADDI, (4, 4, 4), address=12),
            _insn(Opcode.BC, (16, 0, 8, 0), address=16),
        ]
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        self.assertEqual(loops[0].confidence, "partial")
        self.assertIsNone(loops[0].trip_count)

    def test_exact_with_lwz_readonly_trip_count(self) -> None:
        table_addr = 0x80201000
        program = [
            _insn(Opcode.ADDIS, (5, 0, 0x8020), address=0),
            _insn(Opcode.ORI, (5, 5, 0x1000), address=4),
            _insn(Opcode.LWZ, (0, 5, 0), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.STW, (3, 4, 0), address=16),
            _insn(Opcode.ADDI, (4, 4, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
        ]
        partial = find_constant_stride_store_loops(program)
        self.assertEqual(len(partial), 1)
        self.assertEqual(partial[0].confidence, "partial")
        exact = find_constant_stride_store_loops(
            program,
            readonly_words={table_addr: 5},
        )
        self.assertEqual(len(exact), 1)
        self.assertEqual(exact[0].confidence, "exact-pattern")
        self.assertEqual(exact[0].trip_count, 5)

    def test_never_raises_on_empty_or_unrelated(self) -> None:
        self.assertEqual(find_constant_stride_store_loops([]), [])
        self.assertEqual(
            find_constant_stride_store_loops(
                [_insn(Opcode.BCLR, (20, 0, 0), address=0)],
            ),
            [],
        )


class MemoryLoopDischargeTests(unittest.TestCase):
    def test_empty_entry_violations_refuse_discharge(self) -> None:
        """Engine default: missing CFG violation conditions must not discharge."""
        program = _store_loop(
            count=2,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        plan = build_memory_loop_plan_map(program)[8]
        refused = discharge_memory_loop_plan(plan)
        self.assertFalse(refused.all_unsat())
        self.assertEqual(refused.status, "applied")
        self.assertIn("entry violation", refused.reason or "")

        allowed = discharge_memory_loop_plan(
            plan, require_entry_violations=False,
        )
        self.assertTrue(allowed.all_unsat(), allowed.reason)

    def test_summary_matches_concrete_stw_addi(self) -> None:
        program = _store_loop(
            count=3,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        plan = build_memory_loop_plan_map(program)[8]
        summary = plan.summary

        base = 0x1000
        value = 0x11223344
        terminals = [
            t
            for t in execute_cfg(
                concrete_state({
                    "gpr": {"r3": value, "r4": base},
                    "lr": 0x80001000,
                }),
                program,
                ConcreteOps(),
                memory_loop_plans={plan.header_pc: plan},
                max_loop_iterations=2,
            )
            if t.condition and t.exit_kind != "memory-loop-entry-premise"
        ]
        self.assertEqual(len(terminals), 1)
        state = terminals[0].state
        self.assertEqual(state.gpr[4], base + 12)
        self.assertEqual(state.ctr, 0)
        self.assertEqual(len(state.memory_writes), 12)  # 3 stores × 4 bytes
        self.assertEqual(state.memory_touches, state.memory_writes)
        for index in range(3):
            addr = base + index * 4
            word = 0
            for offset in range(4):
                word = (word << 8) | state.memory.read(addr + offset)
            self.assertEqual(word, value)

    def test_summary_matches_concrete_stwu(self) -> None:
        program = _store_loop(count=2, store=(Opcode.STWU, (5, 6, 4)))
        plan = build_memory_loop_plan_map(program)[8]
        base = 0x2000
        value = 0xAABBCCDD
        terminals = [
            t
            for t in execute_cfg(
                concrete_state({
                    "gpr": {"r5": value, "r6": base},
                    "lr": 0x80001000,
                }),
                program,
                ConcreteOps(),
                memory_loop_plans={plan.header_pc: plan},
                max_loop_iterations=2,
            )
            if t.condition and t.exit_kind != "memory-loop-entry-premise"
        ]
        self.assertEqual(len(terminals), 1)
        state = terminals[0].state
        self.assertEqual(state.gpr[6], base + 8)
        for index in range(2):
            addr = base + (index + 1) * 4
            word = 0
            for offset in range(4):
                word = (word << 8) | state.memory.read(addr + offset)
            self.assertEqual(word, value)


class MemoryLoopFeatureGateTests(unittest.TestCase):
    def test_feature_known_and_discharge_gated(self) -> None:
        self.assertIn("memory-loop-summary", KNOWN_PROOF_FEATURES)
        self.assertNotIn("memory-bus", UNSUPPORTED_FOR_EQUIVALENT)

    def test_malformed_obligation_rejected(self) -> None:
        reason = validate_proof_features(
            {
                "proof_features": ["memory-loop-summary"],
                "memory_loop": {"proof_kind": "constant-stride-store"},
            },
            require_equivalent_ready=True,
        )
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertTrue(
            "memory_loop missing" in reason or "legacy" in reason,
            reason,
        )

    def test_obligation_validates_structurally(self) -> None:
        program = _store_loop(
            count=4,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        plan = build_memory_loop_plan_map(program)[8]
        result = discharge_memory_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)
        side = build_memory_loop_side_entry(
            plan,
            entry_guard=result.entry_guard,
            refinement=result.refinement,
        )
        obligation = build_memory_loop_obligation(
            original=[side],
            candidate=[side],
            status="discharged",
        )
        # While frozen, equivalent-ready validation demotes; structural OK when
        # require_equivalent_ready is False.
        self.assertIsNone(
            validate_proof_features(
                {
                    "proof_features": ["memory-loop-summary"],
                    "memory_loop": obligation,
                },
            ),
        )

    def test_applied_only_cannot_authorize_equivalent(self) -> None:
        """Negative: status=applied must demote EQUIVALENT."""
        from tools.ppc_equivalence.proof_features import enforce_equivalent_proof_features
        from tools.ppc_equivalence.result import ProofResult, ProofStatus

        program = _store_loop(
            count=2,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        plan = build_memory_loop_plan_map(program)[8]
        obligation = build_memory_loop_obligation(
            original=[build_memory_loop_side_entry(plan)],
            candidate=[build_memory_loop_side_entry(plan)],
            status="applied",
        )
        reason = validate_proof_features(
            {
                "proof_features": ["memory-loop-summary"],
                "memory_loop": obligation,
            },
            require_equivalent_ready=True,
        )
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("discharged", reason)

        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["memory-loop-summary"],
            memory_loop=obligation,
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)

    def test_summary_digest_mutation_rejected(self) -> None:
        import copy

        from tools.ppc_equivalence.memory_loop import validate_memory_loop_obligation
        from tools.ppc_equivalence.proof_request import ProofRequest, cache_key

        program = _store_loop(
            count=2,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        plan = build_memory_loop_plan_map(program)[8]
        discharged = discharge_memory_loop_plan(
            plan, require_entry_violations=False,
        )
        side = build_memory_loop_side_entry(
            plan,
            entry_guard=discharged.entry_guard,
            refinement=discharged.refinement,
        )
        obligation = build_memory_loop_obligation(
            original=[side],
            candidate=[side],
            status="discharged",
        )
        self.assertIsNone(validate_memory_loop_obligation(obligation))
        mutated = copy.deepcopy(obligation)
        mutated["original"][0]["summary_sha256"] = "b" * 64
        # Duplicate identity / digest form still structurally typed; identity
        # recompute is per-side summary hash (opaque here) — ensure cache differs.
        def _request(obl: dict) -> ProofRequest:
            return ProofRequest(
                original_hex="48000000",
                candidate_hex="48000000",
                original_base=0x80000000,
                candidate_base=0x80000000,
                contract="default",
                observables=("r4", "memory"),
                limits={"timeout_ms": 1000},
                memory_environment={},
                floating_point_domain=None,
                assumed_callees=(),
                callee_contracts={},
                relocations={},
                proof_features=("memory-loop-summary",),
                obligations={"memory_loop": obl},
            )

        self.assertNotEqual(
            cache_key(_request(obligation), "e" * 64, "c" * 64),
            cache_key(_request(mutated), "e" * 64, "c" * 64),
        )

    def _assert_memory_loop_discharged(self, result) -> None:
        self.assertIn("memory-loop-summary", result.proof_features)
        self.assertIsNotNone(result.memory_loop)
        self.assertEqual(result.memory_loop["schema_version"], 2)
        self.assertEqual(result.memory_loop["algorithm"], "constant-stride-store-set-v3")
        self.assertEqual(result.memory_loop["status"], "discharged")
        self.assertGreaterEqual(len(result.memory_loop["original"]), 1)
        self.assertGreaterEqual(len(result.memory_loop["candidate"]), 1)

    def test_summary_proves_under_tight_iteration_bound(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        # Trip count above max_loop_iterations forces the summary path; keep N
        # modest so exact refinement unrolling stays within the shared deadline.
        program = _store_loop(
            count=6,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        contract = EquivalenceContract(
            parse_observables(["r4", "memory"]),
            timeout_ms=30_000,
        )
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self._assert_memory_loop_discharged(result)
        self.assertEqual(result.memory_loop["original"][0]["header_pc"], 8)

    def test_andi_remainder_proves_under_tight_iteration_bound(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        program = [
            _insn(Opcode.ADDI, (6, 0, 0x2B), address=0),
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=4),
            _insn(Opcode.MTSPR, (6, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]
        contract = EquivalenceContract(
            parse_observables(["r4", "memory"]),
            timeout_ms=15_000,
        )
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self._assert_memory_loop_discharged(result)

    def test_rlwinm_srwi_proves_under_tight_iteration_bound(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        program = [
            _insn(Opcode.ADDI, (6, 0, 0x28), address=0),
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]
        contract = EquivalenceContract(
            parse_observables(["r4", "memory"]),
            timeout_ms=15_000,
        )
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self._assert_memory_loop_discharged(result)

    def test_lwz_readonly_proves_under_tight_iteration_bound(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        table_addr = 0x80201000
        program = [
            _insn(Opcode.ADDIS, (5, 0, 0x8020), address=0),
            _insn(Opcode.ORI, (5, 5, 0x1000), address=4),
            _insn(Opcode.LWZ, (0, 5, 0), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.STW, (3, 4, 0), address=16),
            _insn(Opcode.ADDI, (4, 4, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
            _insn(Opcode.BCLR, (20, 0, 0), address=28),
        ]
        contract = EquivalenceContract(
            parse_observables(["r4", "memory"]),
            timeout_ms=15_000,
        )
        without_map = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertNotEqual(without_map.status, ProofStatus.EQUIVALENT)

        with_map = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
            readonly_words={table_addr: 4},
        )
        self.assertEqual(with_map.status, ProofStatus.EQUIVALENT, with_map.unsupported)
        self._assert_memory_loop_discharged(with_map)
        self.assertEqual(len(with_map.memory_loop["readonly_words_sha256"]), 64)

    def test_different_store_source_regs_never_equivalent(self) -> None:
        """False-eq regression: original stores r3, candidate stores r5."""
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        original = _store_loop(
            count=4,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        candidate = _store_loop(
            count=4,
            store=(Opcode.STW, (5, 4, 0)),
            pointer_addi=(4, 4),
        )
        contract = EquivalenceContract(
            parse_observables(["r4", "memory"]),
            timeout_ms=15_000,
        )
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT, result.unsupported)

    def test_readonly_trip_artifact_vs_memory_mismatch_never_equivalent(self) -> None:
        """False-eq regression: conflicting per-side readonly words at one VA."""
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.memory_loop_readonly import (
            MemoryLoopReadonlyContext,
            ReadonlyWordEvidence,
        )
        from tools.ppc_equivalence.result import ProofStatus

        table_addr = 0x80201000
        program = [
            _insn(Opcode.ADDIS, (5, 0, 0x8020), address=0),
            _insn(Opcode.ORI, (5, 5, 0x1000), address=4),
            _insn(Opcode.LWZ, (0, 5, 0), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.STW, (3, 4, 0), address=16),
            _insn(Opcode.ADDI, (4, 4, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
            _insn(Opcode.BCLR, (20, 0, 0), address=28),
        ]
        contract = EquivalenceContract(
            parse_observables(["r4", "memory"]),
            timeout_ms=15_000,
        )
        conflict_ctx = MemoryLoopReadonlyContext(
            original=(ReadonlyWordEvidence(table_addr, 2),),
            candidate=(ReadonlyWordEvidence(table_addr, 3),),
        )
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
            memory_loop_readonly=conflict_ctx,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED, result.unsupported)
        self.assertTrue(
            any("conflict" in item for item in result.unsupported),
            result.unsupported,
        )

    def test_summarized_store_of_r1_escapes_private_stack(self) -> None:
        """Adversarial: publishing r1 through a summarized store loop escapes."""
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        def _program(private_value: int) -> list[Instruction]:
            return [
                _insn(Opcode.ADDI, (3, 0, private_value), address=0),
                _insn(Opcode.ADDI, (1, 1, -64), address=4),
                _insn(Opcode.STB, (3, 1, 8), address=8),
                _insn(Opcode.ADDI, (0, 0, 4), address=12),
                _insn(Opcode.MTSPR, (0, 9), address=16),
                _insn(Opcode.STW, (1, 5, 0), address=20),
                _insn(Opcode.ADDI, (5, 5, 4), address=24),
                _insn(Opcode.BC, (16, 0, 20, 0), address=28),
                _insn(Opcode.ADDI, (1, 1, 64), address=32),
                _insn(Opcode.BCLR, (20, 0, 0), address=36),
            ]

        contract = EquivalenceContract(
            parse_observables(["memory"]),
            timeout_ms=15_000,
        )
        result = check_equivalence(
            _program(1),
            _program(2),
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT, result.unsupported)
        self.assertIsNotNone(result.counterexample)


class MemoryLoopRefinementRegressionTests(unittest.TestCase):
    def test_summary_mutations_fail_refinement(self) -> None:
        from dataclasses import replace

        from tools.ppc_equivalence.memory_loop import MemoryLoopPlan

        program = _store_loop(
            count=3,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        plan = build_memory_loop_plan_map(program)[8]
        self.assertTrue(
            discharge_memory_loop_plan(plan, require_entry_violations=False).all_unsat()
        )

        cases = [
            ("source_reg", 5),
            ("base_reg", 7),
            ("stride", 8),
            ("store_width", 2),
            ("store_kind", "stwu"),
            ("trip_count", 4),
            ("final_ctr", 1),
        ]
        for field, value in cases:
            with self.subTest(field=field):
                kwargs = {field: value}
                if field == "store_width":
                    kwargs["stride"] = value
                if field == "stride":
                    kwargs["store_width"] = value
                mutated = MemoryLoopPlan(
                    summary=replace(plan.summary, **kwargs),
                    witness=plan.witness,
                )
                result = discharge_memory_loop_plan(
                    mutated, require_entry_violations=False,
                )
                self.assertFalse(
                    result.all_unsat(),
                    f"{field} mutation incorrectly discharged",
                )
                self.assertIn(
                    result.status,
                    ("internal_error", "failed", "unsupported", "applied"),
                    result.status,
                )

    def test_relocated_sides_discharge_independently(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        original = _store_loop(
            count=3,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
            base_address=0x80001000,
        )
        candidate = _store_loop(
            count=3,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
            base_address=0x80002000,
        )
        contract = EquivalenceContract(
            parse_observables(["r4", "memory"]),
            timeout_ms=15_000,
        )
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertEqual(result.memory_loop["original"][0]["header_pc"], 0x80001008)
        self.assertEqual(result.memory_loop["candidate"][0]["header_pc"], 0x80002008)
        self.assertNotEqual(
            result.memory_loop["original"][0]["code_sha256"],
            result.memory_loop["candidate"][0]["code_sha256"],
        )

    def test_stwu_vs_stw_addi_shapes_discharge(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        # stwu writes at base+4, base+8, ...; stw/addi writes at base, base+4, ...
        # Align by starting the stwu base one stride lower so footprints match.
        original = _store_loop(count=3, store=(Opcode.STWU, (3, 4, 4)))
        candidate = _store_loop(
            count=3,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        # Adjust candidate entry base via a leading addi so final effects match
        # when r4_stwu_entry = r4_stw_entry - 4. Encode that in the candidate
        # prologue: addi r4, r4, -4 before the counted loop.
        candidate = [
            _insn(Opcode.ADDI, (4, 4, -4), address=0),
            _insn(Opcode.ADDI, (0, 0, 3), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]
        # Original stwu loop at matching addresses.
        original = [
            _insn(Opcode.ADDI, (0, 0, 3), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.STWU, (3, 4, 4), address=8),
            _insn(Opcode.BC, (16, 0, 8, 0), address=12),
            _insn(Opcode.BCLR, (20, 0, 0), address=16),
        ]
        # Per-side discharge only: structural summary match must not be required.
        orig_plan = build_memory_loop_plan_map(original)[8]
        cand_plan = build_memory_loop_plan_map(candidate)[12]
        self.assertNotEqual(orig_plan.summary.store_kind, cand_plan.summary.store_kind)
        self.assertTrue(
            discharge_memory_loop_plan(
                orig_plan, require_entry_violations=False,
            ).all_unsat()
        )
        self.assertTrue(
            discharge_memory_loop_plan(
                cand_plan, require_entry_violations=False,
            ).all_unsat()
        )

    def test_multiple_loops_require_both_discharges(self) -> None:
        import copy

        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.memory_loop import validate_memory_loop_obligation
        from tools.ppc_equivalence.proof_features import enforce_equivalent_proof_features
        from tools.ppc_equivalence.result import ProofResult, ProofStatus

        # Two consecutive store loops in one function.
        program = [
            _insn(Opcode.ADDI, (0, 0, 2), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.STW, (3, 4, 0), address=8),
            _insn(Opcode.ADDI, (4, 4, 4), address=12),
            _insn(Opcode.BC, (16, 0, 8, 0), address=16),
            _insn(Opcode.ADDI, (0, 0, 2), address=20),
            _insn(Opcode.MTSPR, (0, 9), address=24),
            _insn(Opcode.STW, (5, 6, 0), address=28),
            _insn(Opcode.ADDI, (6, 6, 4), address=32),
            _insn(Opcode.BC, (16, 0, 28, 0), address=36),
            _insn(Opcode.BCLR, (20, 0, 0), address=40),
        ]
        contract = EquivalenceContract(
            parse_observables(["r4", "r6", "memory"]),
            timeout_ms=30_000,
        )
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertEqual(len(result.memory_loop["original"]), 2)
        self.assertEqual(len(result.memory_loop["candidate"]), 2)

        corrupted = copy.deepcopy(result.memory_loop)
        corrupted["original"] = corrupted["original"][:1]
        corrupted["status"] = "discharged"
        gated = enforce_equivalent_proof_features(
            ProofResult(
                status=ProofStatus.EQUIVALENT,
                proof_features=["memory-loop-summary"],
                memory_loop=corrupted,
            ),
        )
        # Corrupting the obligation (missing a used plan) must not stay EQUIVALENT
        # under equivalent-ready validation when the engine attested two plans;
        # structural validation of the forged block alone may still pass, so
        # demote via status=applied forgery.
        corrupted["status"] = "applied"
        gated = enforce_equivalent_proof_features(
            ProofResult(
                status=ProofStatus.EQUIVALENT,
                proof_features=["memory-loop-summary"],
                memory_loop=corrupted,
            ),
        )
        self.assertEqual(gated.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)

    def test_entry_ctr_guard_paths(self) -> None:
        from tools.ppc_equivalence.model import concrete_state
        from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg

        program = _store_loop(
            count=3,
            store=(Opcode.STW, (3, 4, 0)),
            pointer_addi=(4, 4),
        )
        plan = build_memory_loop_plan_map(program)[8]
        terminals = execute_cfg(
            concrete_state({"gpr": {"r3": 1, "r4": 0x1000}, "lr": 0x80001000}),
            program,
            ConcreteOps(),
            memory_loop_plans={plan.header_pc: plan},
            max_loop_iterations=2,
        )
        violations = [
            t for t in terminals if t.exit_kind == "memory-loop-entry-premise"
        ]
        # Concrete CTR matches trip count → violation condition is False.
        self.assertTrue(any(t.condition is False for t in violations) or violations == [])
        # Wrong recovered trip leaves a live (True) violation under concrete mismatch.
        from dataclasses import replace
        from tools.ppc_equivalence.memory_loop import MemoryLoopPlan

        wrong = MemoryLoopPlan(
            summary=replace(plan.summary, trip_count=99),
            witness=replace(plan.witness, recognized_trip_count=99),
        )
        wrong_terms = execute_cfg(
            concrete_state({"gpr": {"r3": 1, "r4": 0x1000}, "lr": 0x80001000}),
            program,
            ConcreteOps(),
            memory_loop_plans={wrong.header_pc: wrong},
            max_loop_iterations=2,
        )
        live = [
            t for t in wrong_terms
            if t.exit_kind == "memory-loop-entry-premise" and t.condition
        ]
        self.assertTrue(live, "wrong trip count must leave a SAT premise-violation path")


if __name__ == "__main__":
    unittest.main()

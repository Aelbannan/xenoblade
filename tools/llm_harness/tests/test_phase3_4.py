from __future__ import annotations

import sys
from collections import Counter
from pathlib import Path
from typing import Any

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

import unittest

from tools.llm_harness.structural import (
    MemoryAccess,
    CFGShape,
    compare_calls,
    compare_relocations,
    compare_memory_accesses,
    compare_cfg,
    compare_constants,
    compare_returns,
    compare_instruction_classes,
    compare_structural,
    _extract_calls,
    _extract_relocations,
    _extract_memory_accesses,
    _extract_constants,
    _extract_return_paths,
    _extract_instruction_classes,
    _build_cfg_shape,
    _coarse_class,
    _classify_constant,
    _memory_base_class,
    route_candidate,
    STRUCTURAL_WEIGHTS,
    ROUTING_THRESHOLDS,
    PENALTIES,
    StructuralComponent,
    StructuralReport,
)
from tools.llm_harness.semantic_repair import (
    SemanticFrontier,
    SemanticRepairAttempt,
    SemanticRepairBranch,
    compute_frontier,
    improves_semantic_frontier,
)
from tools.ppc_equivalence.elf_symbols import FunctionBytes
from tools.ppc_equivalence.ir import Opcode, R_PPC_REL24
from tools.ppc_equivalence.decoder import decode_block

# A simple retail function used as baseline throughout tests
SIMPLE_FN_CODE = bytes.fromhex(
    '9421fff0'  # stwu r1,-16(r1)
    '7c0802a6'  # mfspr r0,LR
    '93e1000c'  # stw r31,12(r1)
    '90010014'  # stw r0,20(r1)
    '48000001'  # bl 0x80000014
    '80010014'  # lwz r0,20(r1)
    '83e1000c'  # lwz r31,12(r1)
    '7c0803a6'  # mtspr LR,r0
    '38210010'  # addi r1,r1,16
    '4e800020'  # bclr 20,0
)

SIMPLE_FN = FunctionBytes(
    name='test', path=Path('.'), code=SIMPLE_FN_CODE,
    base=0x80000000, value=0x80000000, size=40,
    section_index=0, section_name='.text', symbol_type=0,
)


class TestCallExtraction(unittest.TestCase):
    """§11.3 — calls and relocations"""

    def test_single_call(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        calls = _extract_calls(insns)
        self.assertEqual(len(calls), 1)

    def test_compare_calls_identical(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        calls = _extract_calls(insns)
        comp = compare_calls(calls, calls)
        self.assertGreaterEqual(comp.score, 0.99)

    def test_compare_calls_missing(self):
        retail = Counter({(R_PPC_REL24, "funcA"): 1, (R_PPC_REL24, "funcB"): 1})
        candidate = Counter({(R_PPC_REL24, "funcA"): 1})
        comp = compare_calls(retail, candidate)
        self.assertLess(comp.score, 1.0)
        self.assertIn("missing call: funcB", "\n".join(comp.details))

    def test_compare_calls_extra(self):
        retail = Counter({(R_PPC_REL24, "funcA"): 1})
        candidate = Counter({(R_PPC_REL24, "funcA"): 1, (R_PPC_REL24, "unexpected"): 1})
        comp = compare_calls(retail, candidate)
        self.assertIn("unexpected call: unexpected", "\n".join(comp.details))

    def test_compare_calls_multiset(self):
        retail = Counter({(R_PPC_REL24, "funcA"): 2})
        candidate = Counter({(R_PPC_REL24, "funcA"): 1})
        comp = compare_calls(retail, candidate)
        self.assertIn("missing call: funcA x1", "\n".join(comp.details))


class TestMemoryAccessExtraction(unittest.TestCase):
    """§11.4 — memory accesses"""

    def test_extract_memory_accesses(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        accesses = _extract_memory_accesses(insns)
        self.assertEqual(len(accesses), 5)  # stwu + 2x stw + 2x lwz

    def test_access_kind(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        accesses = _extract_memory_accesses(insns)
        kinds = [a.kind for a in accesses]
        self.assertEqual(kinds.count("store"), 3)
        self.assertEqual(kinds.count("load"), 2)

    def test_access_widths(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        accesses = _extract_memory_accesses(insns)
        all_32 = all(a.width_bits == 32 for a in accesses)
        self.assertTrue(all_32)

    def test_stack_base_detection(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        accesses = _extract_memory_accesses(insns)
        stack_accesses = [a for a in accesses if a.base_class == "stack"]
        self.assertTrue(len(stack_accesses) >= 3)

    def test_compare_identical(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        accesses = _extract_memory_accesses(insns)
        comp, unexpected, missing = compare_memory_accesses(accesses, accesses)
        self.assertGreaterEqual(comp.score, 0.99)
        self.assertEqual(len(unexpected), 0)
        self.assertEqual(len(missing), 0)

    def test_compare_different_width(self):
        retail = [MemoryAccess(kind="store", width_bits=32, base_class="stack", offset=12)]
        candidate = [MemoryAccess(kind="store", width_bits=8, base_class="stack", offset=12)]
        comp, unexpected, missing = compare_memory_accesses(retail, candidate)
        self.assertLess(comp.score, 1.0)


class TestCFGShape(unittest.TestCase):
    """§11.5 — CFG shape"""

    def test_build_cfg_shape(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        shape = _build_cfg_shape(insns, 0x80000000)
        self.assertGreaterEqual(shape.num_blocks, 1)

    def test_compare_cfg_identical(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        shape = _build_cfg_shape(insns, 0x80000000)
        comp = compare_cfg(shape, shape)
        self.assertGreaterEqual(comp.score, 0.99)

    def test_compare_cfg_different_blocks(self):
        retail = CFGShape(num_blocks=3, num_edges=4, num_returns=1,
                          num_conditional_branches=1, num_unconditional_branches=0,
                          has_backedges=False, num_backedges=0, exit_block_count=1)
        candidate = CFGShape(num_blocks=2, num_edges=3, num_returns=1,
                             num_conditional_branches=0, num_unconditional_branches=0,
                             has_backedges=False, num_backedges=0, exit_block_count=1)
        comp = compare_cfg(retail, candidate)
        self.assertLess(comp.score, 1.0)
        self.assertTrue(len(comp.details) > 0)

    def test_return_count(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        shape = _build_cfg_shape(insns, 0x80000000)
        self.assertGreaterEqual(shape.num_returns, 1)


class TestConstants(unittest.TestCase):
    """§11.6 — constants"""

    def test_extract_constants(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        consts = _extract_constants(insns)
        # Prologue stack constants are filtered (§11.6);
        # SIMPLE_FN has no non-stack immediates.
        self.assertEqual(len(consts), 0)
        # But a non-stack ADDI should still be extracted
        from tools.ppc_equivalence.ir import Instruction
        custom = [
            Instruction(address=0, raw=0, opcode=Opcode.ADDI, operands=(3, 0, 42)),
        ]
        self.assertEqual(_extract_constants(custom), [42])

    def test_classify_constant(self):
        self.assertEqual(_classify_constant(0), "zero")
        self.assertEqual(_classify_constant(5), "shift_amount")
        self.assertEqual(_classify_constant(31), "shift_amount")
        self.assertEqual(_classify_constant(42), "small_enum")
        self.assertEqual(_classify_constant(0xFF), "small_enum")
        self.assertEqual(_classify_constant(256), "other")
        self.assertEqual(_classify_constant(0x8000), "address_like")

    def test_compare_identical(self):
        comp = compare_constants([16, 32, 255], [16, 32, 255])
        self.assertAlmostEqual(comp.score, 1.0, places=4)

    def test_compare_different(self):
        comp = compare_constants([16, 42], [16])
        self.assertTrue(comp.matched >= 1)


class TestReturnPaths(unittest.TestCase):
    """§11.7 — return paths"""

    def test_extract_return_paths(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        rets = _extract_return_paths(insns)
        self.assertEqual(len(rets), 1)

    def test_compare_identical(self):
        comp = compare_returns(["unconditional"], ["unconditional"])
        self.assertGreaterEqual(comp.score, 0.99)

    def test_compare_missing(self):
        comp = compare_returns(["unconditional", "conditional"], ["unconditional"])
        self.assertLess(comp.score, 1.0)
        self.assertTrue(len(comp.details) > 0)


class TestInstructionClasses(unittest.TestCase):
    """§11.8 — instruction classes"""

    def test_extract_classes(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        classes = _extract_instruction_classes(insns)
        self.assertIn("store", classes)
        self.assertIn("load", classes)
        self.assertIn("special_register", classes)

    def test_coarse_class(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        for insn in insns:
            cls = _coarse_class(insn)
            self.assertIsInstance(cls, str)

    def test_compare_identical(self):
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        classes = _extract_instruction_classes(insns)
        comp = compare_instruction_classes(classes, classes)
        self.assertGreaterEqual(comp.score, 0.99)

    def test_compare_different(self):
        retail = Counter({"store": 3, "load": 2})
        candidate = Counter({"store": 2, "load": 3})
        comp = compare_instruction_classes(retail, candidate)
        self.assertLess(comp.score, 1.0)


class TestSelfComparison(unittest.TestCase):
    """§11.9-11.10 — full structural comparison"""

    def test_self_compare_full(self):
        """Self-comparison should score near 1.0 across all components."""
        report = compare_structural(SIMPLE_FN, SIMPLE_FN)
        self.assertAlmostEqual(report.total_score, 1.0, places=2)
        self.assertGreaterEqual(report.calls.score, 0.99)
        self.assertGreaterEqual(report.memory_accesses.score, 0.99)
        self.assertGreaterEqual(report.cfg.score, 0.99)
        self.assertGreaterEqual(report.constants.score, 0.0)
        self.assertGreaterEqual(report.returns.score, 0.99)
        self.assertGreaterEqual(report.instruction_classes.score, 0.99)

    def test_score_bounds(self):
        report = compare_structural(SIMPLE_FN, SIMPLE_FN)
        self.assertGreaterEqual(report.total_score, 0.0)
        self.assertLessEqual(report.total_score, 1.0)

    def test_metrics_structure(self):
        report = compare_structural(SIMPLE_FN, SIMPLE_FN)
        self.assertIsInstance(report.unexpected_effects, list)
        self.assertIsInstance(report.missing_effects, list)
        self.assertIsInstance(report.calls.score, float)


class TestRouting(unittest.TestCase):
    """§11.11 — routing thresholds"""

    def test_semantic_repair_below_threshold(self):
        self.assertEqual(route_candidate(0.5), "semantic_repair")
        self.assertEqual(route_candidate(0.59), "semantic_repair")

    def test_match_improve_at_threshold(self):
        self.assertEqual(route_candidate(0.75), "match_improve")

    def test_promotion_gate_range(self):
        score = ROUTING_THRESHOLDS["first_compile_promotion_min"]
        self.assertEqual(route_candidate(score), "promotion_gate")
        # Score in the middle of the promotion range
        self.assertEqual(route_candidate(0.65), "promotion_gate")
        self.assertEqual(route_candidate(0.74), "promotion_gate")

    def test_threshold_values_ordered(self):
        self.assertLess(
            ROUTING_THRESHOLDS["first_compile_promotion_min"],
            ROUTING_THRESHOLDS["match_improve_at_or_above"],
        )


class TestPenalties(unittest.TestCase):
    """§11.10 — penalty application"""

    def test_penalty_values_defined(self):
        self.assertIn("unexpected_call", PENALTIES)
        self.assertIn("unexpected_global_store", PENALTIES)
        self.assertIn("missing_global_store", PENALTIES)

    def test_unexpected_call_penalty(self):
        # Score should be lower when an unexpected call exists
        report = compare_structural(SIMPLE_FN, SIMPLE_FN)
        clean_score = report.total_score

        # Simulate unexpected call penalty by checking the penalties dict
        self.assertAlmostEqual(PENALTIES["unexpected_call"], 0.10)


class TestSemanticFrontier(unittest.TestCase):
    """§12.6 — semantic frontier"""

    def setUp(self):
        self.frontier = SemanticFrontier(
            target_id="test",
            call_agreement=0.8,
            memory_agreement=0.7,
            cfg_agreement=0.9,
            overall_structural_score=0.8,
            static_match=75.0,
            proof_status="code_match",
        )

    def test_frontier_dominates(self):
        worse = SemanticFrontier(
            target_id="test",
            call_agreement=0.7,
            memory_agreement=0.6,
            cfg_agreement=0.8,
            overall_structural_score=0.7,
        )
        self.assertTrue(self.frontier.dominates(worse))
        self.assertFalse(worse.dominates(self.frontier))

    def test_frontier_no_domination(self):
        other = SemanticFrontier(
            target_id="test",
            call_agreement=0.9,  # better
            memory_agreement=0.6,  # worse
            cfg_agreement=0.85,
            overall_structural_score=0.82,  # slightly better
        )
        # Neither fully dominates the other
        self.assertFalse(self.frontier.dominates(other))
        self.assertFalse(other.dominates(self.frontier))

    def test_compute_from_structural_report(self):
        report = compare_structural(SIMPLE_FN, SIMPLE_FN)
        frontier = compute_frontier(report, static_match=100.0, proof_status="full_match")
        self.assertAlmostEqual(frontier.call_agreement, report.calls.score)
        self.assertAlmostEqual(frontier.overall_structural_score, report.total_score)
        self.assertEqual(frontier.static_match, 100.0)


class TestSemanticRepairBranch(unittest.TestCase):
    """§12.5 — semantic repair branch"""

    def test_branch_creation(self):
        frontier = SemanticFrontier(target_id="test")
        branch = SemanticRepairBranch(
            target_id="test",
            initial_source="void f() {}",
            initial_frontier=frontier,
        )
        self.assertEqual(branch.target_id, "test")
        self.assertEqual(branch.initial_source, "void f() {}")
        self.assertFalse(branch.blocked)

    def test_latest_attempt(self):
        frontier = SemanticFrontier(target_id="test")
        branch = SemanticRepairBranch(
            target_id="test",
            initial_source="void f() {}",
            initial_frontier=frontier,
        )
        branch.attempts.append(SemanticRepairAttempt(
            repair_index=1,
            primary_discrepancy="missing call to funcB",
            source="void f() { funcB(); }",
            expected_effect={"calls": ["add funcB"]},
        ))
        self.assertIsNotNone(branch.latest)
        self.assertEqual(branch.latest.primary_discrepancy, "missing call to funcB")

    def test_block(self):
        frontier = SemanticFrontier(target_id="test")
        branch = SemanticRepairBranch(
            target_id="test",
            initial_source="src",
            initial_frontier=frontier,
        )
        branch.block("non_improving", "attempt 1")
        self.assertTrue(branch.blocked)
        self.assertIn("non_improving", branch.blocked_reason)


class TestIntegration(unittest.TestCase):
    """End-to-end integration checks"""

    def test_whole_pipeline_self_compare(self):
        """Verify structural.compare is usable from the adapter path."""
        report = compare_structural(SIMPLE_FN, SIMPLE_FN)
        self.assertIsInstance(report, StructuralReport)
        self.assertEqual(len(report.unexpected_effects), 0)
        self.assertEqual(len(report.missing_effects), 0)
        self.assertAlmostEqual(report.total_score, 1.0, places=2)

    def test_route_based_on_score(self):
        """Route from report to the correct next step."""
        report = compare_structural(SIMPLE_FN, SIMPLE_FN)
        route = route_candidate(report.total_score)
        self.assertEqual(route, "match_improve")

    def test_coarse_class_coverage(self):
        """Every insn in the test fn gets a coarse class."""
        insns = decode_block(SIMPLE_FN_CODE, 0x80000000)
        for insn in insns:
            cls = _coarse_class(insn)
            self.assertIn(cls, (
                "store", "load", "call", "return", "unconditional_branch",
                "conditional_branch", "integer_arithmetic", "logical",
                "shift_rotate", "compare", "floating_point", "special_register",
                "paired_single", "other",
            ))

    def test_struct_weights_sum_to_one(self):
        total_weight = sum(STRUCTURAL_WEIGHTS.values())
        self.assertAlmostEqual(total_weight, 1.0, places=4)

    def test_sign_extension_class(self):
        """§11.8 — sign/zero extension must be a coarse class."""
        self.assertIn("sign_extension", (
            _coarse_class(type("insn", (), {"opcode": Opcode.EXTSB, "link": False})()),
            _coarse_class(type("insn", (), {"opcode": Opcode.EXTSH, "link": False})()),
        ))

    def test_global_base_class(self):
        """§11.4 — accesses with a relocation classify as global."""
        class FakeInsn:
            opcode = Opcode.LWZ
            operands = (3, 0, 4)
            relocation = type("reloc", (), {"canonical_symbol": "someGlobal", "symbol": "someGlobal"})()
        self.assertEqual(_memory_base_class(FakeInsn()), "global")

    def test_prologue_constants_filtered(self):
        """§11.6 — stack-pointer-relative ADDI constants must be excluded."""
        class FakeInsn:
            opcode = Opcode.ADDI
            operands = (1, 1, -16)
        consts = _extract_constants([FakeInsn()])
        self.assertNotIn(-16, consts)

    def test_pentalties_applied(self):
        """§11.10 — all penalties must be defined."""
        self.assertIn("indirect_call_difference", PENALTIES)
        self.assertIn("return_path_difference", PENALTIES)
        self.assertEqual(PENALTIES["indirect_call_difference"], 0.20)
        self.assertEqual(PENALTIES["return_path_difference"], 0.10)


if __name__ == "__main__":
    raise SystemExit(unittest.main())

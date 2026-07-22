"""SymbolicOps exact-v2 semantics integration (Phase 7 batch)."""

from __future__ import annotations

import ast
import inspect
import unittest
from unittest import mock

import z3

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.fp_capabilities import set_scalar_fp_exact_v2_module_flag
from tools.ppc_equivalence.fp_exact import exact_fadd, exact_fdiv, exact_fdivs, exact_fmul
from tools.ppc_equivalence.fp_exact_fused import fmadd_binary64_rne, fmadds_fpr_rne
from tools.ppc_equivalence.fp_exact_symbolic import (
    scalar_fp_unsupported_predicate,
    scalar_fp_unsupported_query,
    try_concrete_bv64,
    try_dispatch_exact_scalar_v2,
)
from tools.ppc_equivalence.fp_exact_symbolic_arith import (
    verify_exact_arith_bv_concrete,
    verify_exact_fused_bv_concrete,
)
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, SymbolicOps, execute_instruction

_ONE = 0x3FF0000000000000
_TWO = 0x4000000000000000
_F15 = 0x3FF8000000000000
_F2 = 0x4000000000000000
_F4 = 0x4010000000000000


def _insn(opcode: Opcode, operands: tuple[int, ...]) -> Instruction:
    return Instruction(0, 0, opcode, operands)


class SymbolicExactSemanticsV2Tests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_payload_symbolic_fadd_matches_exact_at_one_plus_two(self) -> None:
        ops = SymbolicOps()
        f1 = z3.BitVec("f1", 64)
        f2 = z3.BitVec("f2", 64)
        dispatch = try_dispatch_exact_scalar_v2(
            opcode="fadd",
            a_bits=f1,
            b_bits=f2,
            c_bits=ops.fp_const64(0),
            fpscr=ops.const(0),
            msr=ops.const(0),
            ops=ops,
            scalar_outcome_from_fused=lambda *a, **k: None,  # type: ignore[return-value]
        )
        self.assertIsNotNone(dispatch)
        assert dispatch is not None
        solver = z3.Solver()
        out = z3.BitVec("out", 64)
        solver.add(f1 == _ONE)
        solver.add(f2 == _TWO)
        solver.add(out == dispatch.result_bits)
        self.assertEqual(solver.check(), z3.sat)
        model = solver.model()
        expected = exact_fadd(_ONE, _TWO, fpscr=0).result_bits
        self.assertEqual(int(model.eval(out).as_long()), expected)

    def test_payload_symbolic_fmul_matches_exact_corpus(self) -> None:
        pairs = [
            (_ONE, _TWO),
            (_TWO, _TWO),
        ]
        for a, b in pairs:
            with self.subTest(a=a, b=b):
                self.assertTrue(verify_exact_arith_bv_concrete("fmul", a, b))

    def test_payload_symbolic_fdiv_matches_exact_corpus(self) -> None:
        pairs = [
            (_F15, _F2),
            (_ONE, _TWO),
            (_F2, _F4),
        ]
        for a, b in pairs:
            with self.subTest(a=a, b=b):
                self.assertTrue(verify_exact_arith_bv_concrete("fdiv", a, b))

    def test_payload_symbolic_fdivs_matches_exact_corpus(self) -> None:
        pairs = [
            (_F15, _F2),
            (_ONE, _TWO),
            (0x4000000000000000, 0x8000000000000000),
        ]
        for a, b in pairs:
            with self.subTest(a=a, b=b):
                self.assertTrue(verify_exact_arith_bv_concrete("fdivs", a, b))

    def test_payload_symbolic_fmadd_matches_exact_corpus(self) -> None:
        cases = [
            (_F15, _F4, _F2),
            (_ONE, _TWO, _ONE),
        ]
        for a, c, b in cases:
            with self.subTest(a=a, c=c, b=b):
                self.assertTrue(verify_exact_fused_bv_concrete("fmadd", a, c, b))

    def test_payload_symbolic_single_fused_matches_exact_corpus(self) -> None:
        cases = [
            ("fmadds", _F15, _F2, _F4),
            ("fmsubs", _F15, _F2, _F4),
            ("fnmadds", _F15, _F2, _F4),
            ("fnmsubs", _F15, _F2, _F4),
            ("fmsubs", 0x3FF0000000000000, 0x3FF0000000000000, 0x3FF0000000000000),
        ]
        for opcode, a, b, c in cases:
            with self.subTest(opcode=opcode, a=a, b=b, c=c):
                self.assertTrue(verify_exact_fused_bv_concrete(opcode, a, c, b))

    def test_symbolic_neighbor_fadd_can_differ(self) -> None:
        ops = SymbolicOps()
        f1 = z3.BitVec("f1", 64)
        f2 = z3.BitVec("f2", 64)
        same = try_dispatch_exact_scalar_v2(
            opcode="fadd",
            a_bits=f1,
            b_bits=f2,
            c_bits=ops.fp_const64(0),
            fpscr=ops.const(0),
            msr=ops.const(0),
            ops=ops,
            scalar_outcome_from_fused=lambda *a, **k: None,  # type: ignore[return-value]
        )
        flipped = try_dispatch_exact_scalar_v2(
            opcode="fadd",
            a_bits=f1,
            b_bits=f2 ^ ops.fp_const64(1),
            c_bits=ops.fp_const64(0),
            fpscr=ops.const(0),
            msr=ops.const(0),
            ops=ops,
            scalar_outcome_from_fused=lambda *a, **k: None,  # type: ignore[return-value]
        )
        assert same is not None and flipped is not None
        solver = z3.Solver()
        solver.set("timeout", 10000)
        solver.add(f1 == _ONE)
        solver.add(f2 == _TWO)
        solver.add(same.result_bits != flipped.result_bits)
        self.assertEqual(solver.check(), z3.sat)

    def test_identical_fmul_equivalent_with_flag_on(self) -> None:
        insns = [_insn(Opcode.FMUL, (1, 1, 0, 2))]
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=15000)
        result = check_equivalence(
            insns,
            insns,
            contract,
            original_hex="00",
            candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)

    def test_identical_fdiv_equivalent_with_flag_on(self) -> None:
        insns = [_insn(Opcode.FDIV, (1, 1, 2))]
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=20000)
        result = check_equivalence(
            insns,
            insns,
            contract,
            original_hex="00",
            candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)

    def test_identical_fmadd_equivalent_with_flag_on(self) -> None:
        insns = [_insn(Opcode.FMADD, (7, 1, 2, 3))]
        contract = make_contract(preset=None, observe=["f7"], timeout_ms=20000)
        result = check_equivalence(
            insns,
            insns,
            contract,
            original_hex="00",
            candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)

    def test_different_addend_fadd_not_equivalent(self) -> None:
        original = [_insn(Opcode.FADD, (1, 1, 2))]
        candidate = [_insn(Opcode.FADD, (1, 1, 3))]
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=30000)
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex="00",
            candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT, result.unsupported)

    def test_fixed_input_symbolic_fadd_matches_concrete_exact(self) -> None:
        ops = SymbolicOps()
        state = concrete_state({
            "fpr": {"f1": _ONE, "f2": _TWO, "f3": 0},
            "fpscr": 0,
        })
        # Bind concrete values as BitVecVal constants (fixed-input symbolic).
        state = state.with_fpr(1, ops.fp_const64(_ONE))
        state = state.with_fpr(2, ops.fp_const64(_TWO))
        final = execute_instruction(
            state,
            _insn(Opcode.FADD, (3, 1, 2)),
            ops,
        )
        expected = exact_fadd(_ONE, _TWO, fpscr=0).result_bits
        self.assertEqual(try_concrete_bv64(final.fpr[3]), expected)

    def test_fixed_input_symbolic_fmul_matches_concrete_exact(self) -> None:
        ops = SymbolicOps()
        state = concrete_state({
            "fpr": {"f1": _ONE, "f2": _TWO, "f3": 0},
            "fpscr": 0,
        })
        state = state.with_fpr(1, ops.fp_const64(_ONE))
        state = state.with_fpr(2, ops.fp_const64(_TWO))
        final = execute_instruction(
            state,
            _insn(Opcode.FMUL, (3, 1, 0, 2)),
            ops,
        )
        expected = exact_fmul(_ONE, _TWO, fpscr=0).result_bits
        self.assertEqual(try_concrete_bv64(final.fpr[3]), expected)

    def test_fixed_input_symbolic_fdiv_matches_concrete_exact(self) -> None:
        ops = SymbolicOps()
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": 0},
            "fpscr": 0,
        })
        state = state.with_fpr(1, ops.fp_const64(_F15))
        state = state.with_fpr(2, ops.fp_const64(_F2))
        final = execute_instruction(
            state,
            _insn(Opcode.FDIV, (3, 1, 2)),
            ops,
        )
        expected = exact_fdiv(_F15, _F2, fpscr=0).result_bits
        self.assertEqual(try_concrete_bv64(final.fpr[3]), expected)

    def test_fixed_input_symbolic_fdivs_matches_concrete_exact(self) -> None:
        ops = SymbolicOps()
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f7": 0},
            "fpscr": 0,
        })
        state = state.with_fpr(1, ops.fp_const64(_F15))
        state = state.with_fpr(2, ops.fp_const64(_F2))
        final = execute_instruction(
            state,
            _insn(Opcode.FDIVS, (7, 1, 2)),
            ops,
        )
        expected = exact_fdivs(_F15, _F2, fpscr=0).result_bits
        self.assertEqual(try_concrete_bv64(final.fpr[7]), expected)

    def test_fixed_input_symbolic_fmadd_matches_concrete_exact(self) -> None:
        ops = SymbolicOps()
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        state = state.with_fpr(1, ops.fp_const64(_F15))
        state = state.with_fpr(2, ops.fp_const64(_F2))
        state = state.with_fpr(3, ops.fp_const64(_F4))
        final = execute_instruction(
            state,
            _insn(Opcode.FMADD, (7, 1, 2, 3)),
            ops,
        )
        expected = fmadd_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(try_concrete_bv64(final.fpr[7]), expected)

    def test_fixed_input_symbolic_fmadds_matches_concrete_exact(self) -> None:
        ops = SymbolicOps()
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        state = state.with_fpr(1, ops.fp_const64(_F15))
        state = state.with_fpr(2, ops.fp_const64(_F2))
        state = state.with_fpr(3, ops.fp_const64(_F4))
        final = execute_instruction(
            state,
            _insn(Opcode.FMADDS, (7, 1, 2, 3)),
            ops,
        )
        expected = fmadds_fpr_rne(_F15, _F2, _F4).bits64
        self.assertEqual(try_concrete_bv64(final.fpr[7]), expected)

    def test_identical_fadd_equivalent_with_flag_on(self) -> None:
        insns = [_insn(Opcode.FADD, (1, 1, 2))]
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=10000)
        result = check_equivalence(
            insns,
            insns,
            contract,
            original_hex="00",
            candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)

    def test_flag_off_still_uses_legacy_z3_fp_path(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        ops = SymbolicOps()
        fp_add = mock.Mock(wraps=ops.fp_add)
        ops.fp_add = fp_add
        state = concrete_state({
            "fpr": {"f1": _ONE, "f2": _TWO, "f3": 0},
            "fpscr": 0,
        })
        state = state.with_fpr(1, ops.fp_const64(_ONE))
        state = state.with_fpr(2, ops.fp_const64(_TWO))
        execute_instruction(state, _insn(Opcode.FADD, (3, 1, 2)), ops)
        fp_add.assert_called_once()

    def test_flag_on_does_not_call_symbolic_ops_fp_add(self) -> None:
        ops = SymbolicOps()
        fp_add = mock.Mock(wraps=ops.fp_add)
        ops.fp_add = fp_add
        state = concrete_state({
            "fpr": {"f1": _ONE, "f2": _TWO, "f3": 0},
            "fpscr": 0,
        })
        state = state.with_fpr(1, ops.fp_const64(_ONE))
        state = state.with_fpr(2, ops.fp_const64(_TWO))
        execute_instruction(state, _insn(Opcode.FADD, (3, 1, 2)), ops)
        fp_add.assert_not_called()

    def test_semantics_execute_body_has_no_fp_add_when_flag_on(self) -> None:
        from tools.ppc_equivalence import semantics

        source = inspect.getsource(semantics._execute_instruction_body)
        tree = ast.parse(source)
        fp_add_calls = [
            node
            for node in ast.walk(tree)
            if isinstance(node, ast.Call)
            and isinstance(getattr(node.func, "attr", None), str)
            and node.func.attr == "fp_add"
        ]
        # Native Z3 FP fallthrough must remain gated behind flag-off branches.
        for call in fp_add_calls:
            self.assertTrue(
                any(
                    isinstance(parent, ast.If)
                    and any(
                        "scalar_fp_exact_v2_enabled" in ast.unparse(test)
                        for test in _if_tests(parent)
                    )
                    for parent in _if_ancestors(tree, call)
                ),
                msg="fp_add call is not guarded by scalar_fp_exact_v2_enabled()",
            )

    def test_unsupported_query_unsat_for_supported_dispatch(self) -> None:
        ops = SymbolicOps()
        dispatch = try_dispatch_exact_scalar_v2(
            opcode="fadd",
            a_bits=ops.fp_const64(_ONE),
            b_bits=ops.fp_const64(_TWO),
            c_bits=ops.fp_const64(0),
            fpscr=ops.const(0),
            msr=ops.const(0),
            ops=ops,
            scalar_outcome_from_fused=lambda *a, **k: None,  # type: ignore[return-value]
        )
        self.assertIsNotNone(dispatch)
        assert dispatch is not None
        feasible = [z3.BoolVal(True)]
        bad = scalar_fp_unsupported_predicate(supported=dispatch.outcome.supported)
        result = scalar_fp_unsupported_query(feasible, bad)
        self.assertEqual(result.status, "unsat")

    def test_concrete_ops_unchanged_with_flag_on(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _ONE, "f2": _TWO, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            _insn(Opcode.FADD, (7, 1, 2)),
            ConcreteOps(),
        )
        self.assertEqual(final.fpr[7], exact_fadd(_ONE, _TWO, fpscr=0).result_bits)


def _if_tests(node: ast.If) -> list[ast.expr]:
    tests = [node.test]
    current = node
    while True:
        orelse = current.orelse
        if len(orelse) == 1 and isinstance(orelse[0], ast.If):
            current = orelse[0]
            tests.append(current.test)
            continue
        break
    return tests


def _if_ancestors(tree: ast.AST, target: ast.AST) -> list[ast.If]:
    found: list[ast.If] = []

    class Visitor(ast.NodeVisitor):
        def __init__(self) -> None:
            self.stack: list[ast.If] = []

        def visit_If(self, node: ast.If) -> None:
            self.stack.append(node)
            self.generic_visit(node)
            self.stack.pop()

        def generic_visit(self, node: ast.AST) -> None:
            if node is target:
                found.extend(self.stack)
            super().generic_visit(node)

    Visitor().visit(tree)
    return found


if __name__ == "__main__":
    unittest.main()

"""Phase 7 unit tests — Z3BitVecBackend vs IntBackend, symbolic obligations."""

from __future__ import annotations

import ast
import unittest
from pathlib import Path

import z3

from tools.ppc_equivalence.fp_backend import IntBackend, Z3BitVecBackend
from tools.ppc_equivalence.fp_capabilities import set_scalar_fp_exact_v2_module_flag
from tools.ppc_equivalence.fp_exact import exact_fadd, exact_fmul
from tools.ppc_equivalence.fp_exact_symbolic import (
    prove_backend_agrees,
    prove_exact_arithmetic_corpus,
    scalar_fp_unsupported_query,
)

_PKG = Path(__file__).resolve().parent.parent
_BACKEND_MODULE = _PKG / "fp_backend.py"


class BackendAgreementTests(unittest.TestCase):
    def test_add_agrees_on_corpus(self) -> None:
        pairs = [(1, 2), (0xFFFF, 1), (0x7FFFFFFF, 0x7FFFFFFF)]
        for a, b in pairs:
            self.assertTrue(prove_backend_agrees("add", a, b))

    def test_mul_agrees_on_corpus(self) -> None:
        pairs = [(3, 5), (0x10000, 0x10000), (0xFFFFFFFF, 2)]
        for a, b in pairs:
            self.assertTrue(prove_backend_agrees("mul", a, b))

    def test_int_and_z3_ite_agree(self) -> None:
        int_b = IntBackend()
        z3_b = Z3BitVecBackend(64)
        cond_int = int_b.lt(int_b.const(1), int_b.const(5))
        int_result = int_b.ite(cond_int, int_b.const(9), int_b.const(3))
        a = z3_b.const(1)
        b = z3_b.const(5)
        cond_z3 = z3_b.lt(a, b)
        z3_result = z3_b.ite(cond_z3, z3_b.const(9), z3_b.const(3))
        solver = z3_b.z3.Solver()
        out = z3_b.z3.BitVec("out", 64)
        solver.add(out == z3_result)
        self.assertEqual(solver.check(), z3_b.z3.sat)
        self.assertEqual(int(solver.model()[out].as_long()), int_result)


class ExactArithmeticCorpusTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_exact_fadd_stable(self) -> None:
        corpus = [
            (0x3FF0000000000000, 0x3FF0000000000000),
            (0x3FF8000000000000, 0x3FF0000000000000),
        ]
        for a, b, ok in prove_exact_arithmetic_corpus(corpus, op="fadd"):
            self.assertTrue(ok, msg=f"fadd({a:#x}, {b:#x}) unstable")

    def test_exact_fmul_stable(self) -> None:
        corpus = [(0x3FF0000000000000, 0x4000000000000000)]
        for _a, _b, ok in prove_exact_arithmetic_corpus(corpus, op="fmul"):
            self.assertTrue(ok)


class UnsupportedQueryTests(unittest.TestCase):
    def test_unsupported_unsat_when_infeasible(self) -> None:
        x = z3.BitVec("x", 32)
        feasible = [x == z3.BitVecVal(0, 32)]
        bad = x != z3.BitVecVal(0, 32)
        result = scalar_fp_unsupported_query(feasible, bad)
        self.assertEqual(result.status, "unsat")

    def test_unsupported_sat_when_reachable(self) -> None:
        x = z3.BitVec("x", 32)
        feasible = [x == z3.BitVecVal(1, 32)]
        bad = x == z3.BitVecVal(1, 32)
        result = scalar_fp_unsupported_query(feasible, bad)
        self.assertEqual(result.status, "sat")

    def test_vacuous_when_no_predicate(self) -> None:
        result = scalar_fp_unsupported_query([], None)
        self.assertEqual(result.status, "vacuous")


class NoHostFloatBackendTests(unittest.TestCase):
    def test_fp_backend_module_has_no_host_float(self) -> None:
        tree = ast.parse(_BACKEND_MODULE.read_text(encoding="utf-8"))
        for node in ast.walk(tree):
            if isinstance(node, ast.Import):
                for alias in node.names:
                    self.assertNotIn("struct", alias.name)
            if isinstance(node, ast.ImportFrom):
                self.assertNotIn(node.module or "", ("struct", "math"))


class ExactAddConcreteTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_exact_fadd_one_plus_one(self) -> None:
        one = 0x3FF0000000000000
        outcome = exact_fadd(one, one, fpscr=0)
        self.assertTrue(outcome.supported)
        self.assertEqual(outcome.result_bits, 0x4000000000000000)

    def test_exact_fmul_two_times_two(self) -> None:
        two = 0x4000000000000000
        outcome = exact_fmul(two, two, fpscr=0)
        self.assertTrue(outcome.supported)
        self.assertEqual(outcome.result_bits, 0x4010000000000000)


if __name__ == "__main__":
    unittest.main()

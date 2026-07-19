"""P2-04 solver diagnostics tests."""

from __future__ import annotations

import os
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.diagnostics import categorize_assertions, count_z3_symbols
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.result import ProofStatus

R3 = "r3"


def decode(text: str):
    return decode_block(parse_hex(text), validate_with_capstone=False)


class SolverDiagnosticsTests(unittest.TestCase):
    def test_diagnostics_off_by_default(self) -> None:
        original = decode("38630001")
        candidate = decode("38630002")
        contract = make_contract(preset=None, observe=(R3,), timeout_ms=10_000)
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex="38630001",
            candidate_hex="38630002",
        )
        self.assertIsNone(result.solver_diagnostics)

    def test_diagnostics_populated_with_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            smt_path = Path(tmp) / "out.smt2"
            original = decode("38630001")
            candidate = decode("38630002")
            contract = make_contract(preset=None, observe=(R3,), timeout_ms=10_000)
            result = check_equivalence(
                original,
                candidate,
                contract,
                original_hex="38630001",
                candidate_hex="38630002",
                diagnostics_out=str(smt_path),
            )
            diag = result.solver_diagnostics
            self.assertIsNotNone(diag)
            self.assertGreater(diag["assertion_count"], 0)
            self.assertGreater(diag["path_pair_count"], 0)
            self.assertGreaterEqual(diag["layout_constraint_count"], 0)
            self.assertGreaterEqual(diag["relocation_constraint_count"], 0)
            self.assertIn("phase_timings", diag)
            self.assertIn("assertions_by_category", diag)
            self.assertIsInstance(diag["bv_symbols"], int)
            self.assertGreater(diag["bv_symbols"], 0)
            self.assertIsInstance(diag["array_symbols"], int)
            self.assertGreaterEqual(diag["array_symbols"], 0)
            self.assertIsInstance(diag["func_symbols"], int)
            self.assertTrue(smt_path.is_file())
            self.assertGreater(smt_path.stat().st_size, 0)
            self.assertEqual(diag["smt2_dump_path"], str(smt_path))

    def test_diagnostics_via_env_var(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            old = os.environ.get("PPC_EQUIV_DIAGNOSTICS")
            old_cwd = os.getcwd()
            os.environ["PPC_EQUIV_DIAGNOSTICS"] = "1"
            os.chdir(tmp)
            try:
                original = decode("38630001")
                candidate = decode("38630002")
                contract = make_contract(preset=None, observe=(R3,), timeout_ms=10_000)
                result = check_equivalence(
                    original,
                    candidate,
                    contract,
                    original_hex="38630001",
                    candidate_hex="38630002",
                    source_hash="0123456789abcdef",
                )
            finally:
                os.chdir(old_cwd)
                if old is None:
                    os.environ.pop("PPC_EQUIV_DIAGNOSTICS", None)
                else:
                    os.environ["PPC_EQUIV_DIAGNOSTICS"] = old
            self.assertIsNotNone(result.solver_diagnostics)
            self.assertGreater(result.solver_diagnostics["assertion_count"], 0)

    def test_diagnostics_on_equivalent_path(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            smt_path = Path(tmp) / "eq.smt2"
            original = decode("38630001")
            candidate = decode("38630001")
            contract = make_contract(preset=None, observe=(R3,), timeout_ms=10_000)
            result = check_equivalence(
                original,
                candidate,
                contract,
                original_hex="38630001",
                candidate_hex="38630001",
                diagnostics_out=str(smt_path),
            )
            self.assertEqual(result.status, ProofStatus.EQUIVALENT)
            diag = result.solver_diagnostics
            self.assertIsNotNone(diag)
            self.assertGreater(diag["assertion_count"], 0)
            self.assertGreater(diag["bv_symbols"], 0)
            self.assertEqual(diag["solver_result"], "unsat")

    def test_diagnostics_on_unknown_path(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            smt_path = Path(tmp) / "unknown.smt2"
            original = decode("38630001")
            candidate = decode("38630002")
            contract = make_contract(preset=None, observe=(R3,), timeout_ms=10_000)

            class _FakeSolver:
                def reason_unknown(self) -> str:
                    return "interrupted"

                def statistics(self):
                    return {}

            import z3

            with mock.patch(
                "tools.ppc_equivalence.engine.check_with_portfolio",
                return_value=(_FakeSolver(), z3.unknown, "default", []),
            ):
                result = check_equivalence(
                    original,
                    candidate,
                    contract,
                    original_hex="38630001",
                    candidate_hex="38630002",
                    diagnostics_out=str(smt_path),
                )
            self.assertIn(
                result.status,
                (ProofStatus.INCONCLUSIVE_UNKNOWN, ProofStatus.INCONCLUSIVE_TIMEOUT),
            )
            diag = result.solver_diagnostics
            self.assertIsNotNone(diag)
            self.assertGreater(diag["assertion_count"], 0)
            self.assertIsNotNone(diag["unknown_reason"])

    def test_count_z3_symbols_and_categories(self) -> None:
        import z3

        x = z3.BitVec("x", 32)
        mem = z3.Array("mem", z3.BitVecSort(32), z3.BitVecSort(8))
        counts = count_z3_symbols([x + 1 == 0, mem[x] == 0])
        self.assertGreaterEqual(counts["bv_symbols"], 1)
        self.assertGreaterEqual(counts["array_symbols"], 1)
        cats = categorize_assertions(
            layout_constraint_count=5,
            memory_constraint_count=2,
            relocation_binding_count=1,
            path_pair_count=3,
        )
        self.assertEqual(cats["layout"], 3)
        self.assertEqual(cats["memory"], 2)
        self.assertEqual(cats["relocation_bindings"], 1)
        self.assertEqual(cats["inequivalence_disjunction"], 1)
        self.assertEqual(cats["path_pairs_covered"], 3)


if __name__ == "__main__":
    unittest.main()

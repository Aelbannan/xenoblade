"""Tests for independent UNSAT discharge queries."""

from __future__ import annotations

import importlib.util
import unittest

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.discharge import discharge_bad_conditions


_HAS_Z3 = importlib.util.find_spec("z3") is not None


@unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
class DischargeTests(unittest.TestCase):
    def test_vacuous_bad_conditions_are_unsat(self) -> None:
        import z3

        result = discharge_bad_conditions(
            premises=[z3.BoolVal(True)],
            bad_conditions=[],
            deadline=Deadline.after_ms(5_000),
            algorithm="test-vacuous",
            z3_module=z3,
        )
        self.assertEqual(result.status, "unsat")
        self.assertEqual(len(result.query_sha256), 64)

    def test_reachable_bad_condition_is_sat(self) -> None:
        import z3

        x = z3.BitVec("x", 32)
        result = discharge_bad_conditions(
            premises=[x == 1],
            bad_conditions=[x == 1],
            deadline=Deadline.after_ms(5_000),
            algorithm="test-sat",
            z3_module=z3,
        )
        self.assertEqual(result.status, "sat")

    def test_unreachable_bad_condition_is_unsat(self) -> None:
        import z3

        x = z3.BitVec("x", 32)
        result = discharge_bad_conditions(
            premises=[x == 1],
            bad_conditions=[x == 2],
            deadline=Deadline.after_ms(5_000),
            algorithm="test-unsat",
            z3_module=z3,
        )
        self.assertEqual(result.status, "unsat")
        digest = result.to_obligation_digest()
        self.assertEqual(digest["result"], "unsat")
        self.assertEqual(digest["query_sha256"], result.query_sha256)


if __name__ == "__main__":
    unittest.main()

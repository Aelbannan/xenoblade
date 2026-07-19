from __future__ import annotations

import importlib
import time
import unittest
from unittest import mock

from tools.ppc_equivalence.deadline import Deadline, ProofDeadlineExceeded, SolverPhase


class DeadlineUnitTests(unittest.TestCase):
    def test_after_ms_creates_future_deadline(self) -> None:
        d = Deadline.after_ms(10_000)
        self.assertGreater(d.end_ns, time.monotonic_ns())

    def test_after_ms_rejects_non_positive(self) -> None:
        with self.assertRaises(ValueError):
            Deadline.after_ms(0)
        with self.assertRaises(ValueError):
            Deadline.after_ms(-1)

    def test_remaining_ms_positive_while_fresh(self) -> None:
        d = Deadline.after_ms(60_000)
        self.assertGreater(d.remaining_ms(), 0)

    def test_remaining_ms_zero_after_expiry(self) -> None:
        d = Deadline(time.monotonic_ns() - 1)
        self.assertEqual(d.remaining_ms(), 0)

    def test_require_time_returns_positive_while_fresh(self) -> None:
        d = Deadline.after_ms(60_000)
        t = d.require_time("test")
        self.assertGreater(t, 0)

    def test_require_time_raises_when_expired(self) -> None:
        d = Deadline(time.monotonic_ns() - 1)
        with self.assertRaises(ProofDeadlineExceeded) as ctx:
            d.require_time("late-phase")
        self.assertEqual(ctx.exception.phase, "late-phase")

    def test_expired_true_after_deadline(self) -> None:
        d = Deadline(time.monotonic_ns() - 1)
        self.assertTrue(d.expired())

    def test_expired_false_before_deadline(self) -> None:
        d = Deadline.after_ms(60_000)
        self.assertFalse(d.expired())

    def test_solver_phase_to_dict(self) -> None:
        p = SolverPhase("default", "unsat", 12.345)
        d = p.to_dict()
        self.assertEqual(d["name"], "default")
        self.assertEqual(d["result"], "unsat")
        self.assertEqual(d["elapsed_ms"], 12.345)


@unittest.skipUnless(importlib.util.find_spec("z3"), "z3-solver is not installed")
class CfgDeadlineTests(unittest.TestCase):
    def test_execute_cfg_raises_when_deadline_already_expired(self) -> None:
        from tools.ppc_equivalence.decoder import decode_block, parse_hex
        from tools.ppc_equivalence.semantics import SymbolicOps, execute_cfg
        from tools.ppc_equivalence.engine import _symbolic_initial

        ops = SymbolicOps()
        insns = decode_block(parse_hex("4e800020"), validate_with_capstone=False)
        deadline = Deadline(time.monotonic_ns() - 1)
        with self.assertRaises(ProofDeadlineExceeded) as ctx:
            execute_cfg(
                _symbolic_initial(ops),
                insns,
                ops,
                deadline=deadline,
            )
        self.assertEqual(ctx.exception.phase, "cfg-exploration")

    def test_check_equivalence_cfg_timeout_is_inconclusive(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.decoder import decode_block, parse_hex
        from tools.ppc_equivalence.engine import check_equivalence

        original_hex = "4e800020"
        deadline = Deadline(time.monotonic_ns() - 1)
        with mock.patch(
            "tools.ppc_equivalence.engine.Deadline.after_ms",
            return_value=deadline,
        ):
            result = check_equivalence(
                decode_block(parse_hex(original_hex), validate_with_capstone=False),
                decode_block(parse_hex(original_hex), validate_with_capstone=False),
                EquivalenceContract(parse_observables(["r3"]), timeout_ms=10_000),
                original_hex=original_hex,
                candidate_hex=original_hex,
            )
        self.assertEqual(result.status.value, "inconclusive_timeout")
        self.assertTrue(
            any("cfg-exploration" in warning for warning in result.warnings),
            result.warnings,
        )
        self.assertEqual(result.solver.get("timeout_ms"), 10_000)

    def test_deadline_is_created_before_cfg_and_threaded_through(self) -> None:
        from tools.ppc_equivalence import semantics as semantics_mod
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.decoder import decode_block, parse_hex
        from tools.ppc_equivalence.engine import check_equivalence

        original_hex = "4e800020"
        seen: dict[str, object] = {"cfg_calls": 0}
        real_after_ms = Deadline.after_ms
        real_execute_cfg = semantics_mod.execute_cfg

        def tracking_after_ms(timeout_ms: int) -> Deadline:
            deadline = real_after_ms(timeout_ms)
            seen["deadline"] = deadline
            return deadline

        def tracking_execute_cfg(*args, **kwargs):
            self.assertIs(kwargs.get("deadline"), seen["deadline"])
            seen["cfg_calls"] = int(seen["cfg_calls"]) + 1
            return real_execute_cfg(*args, **kwargs)

        with mock.patch(
            "tools.ppc_equivalence.engine.Deadline.after_ms",
            side_effect=tracking_after_ms,
        ), mock.patch(
            "tools.ppc_equivalence.engine.execute_cfg",
            side_effect=tracking_execute_cfg,
        ):
            result = check_equivalence(
                decode_block(parse_hex(original_hex), validate_with_capstone=False),
                decode_block(parse_hex(original_hex), validate_with_capstone=False),
                EquivalenceContract(parse_observables(["r3"]), timeout_ms=10_000),
                original_hex=original_hex,
                candidate_hex=original_hex,
            )

        self.assertEqual(seen["cfg_calls"], 2)
        self.assertEqual(result.status.value, "equivalent")

    def test_check_equivalence_timeout_after_cfg_before_solve(self) -> None:
        from tools.ppc_equivalence import semantics as semantics_mod
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.decoder import decode_block, parse_hex
        from tools.ppc_equivalence.engine import check_equivalence

        original_hex = "4e800020"
        expired = Deadline(time.monotonic_ns() - 1)
        real_execute_cfg = semantics_mod.execute_cfg

        def execute_cfg_ignore_deadline(*args, **kwargs):
            kwargs = {**kwargs, "deadline": None}
            return real_execute_cfg(*args, **kwargs)

        with mock.patch(
            "tools.ppc_equivalence.engine.Deadline.after_ms",
            return_value=expired,
        ), mock.patch(
            "tools.ppc_equivalence.engine.execute_cfg",
            side_effect=execute_cfg_ignore_deadline,
        ):
            result = check_equivalence(
                decode_block(parse_hex(original_hex), validate_with_capstone=False),
                decode_block(parse_hex(original_hex), validate_with_capstone=False),
                EquivalenceContract(parse_observables(["r3"]), timeout_ms=10_000),
                original_hex=original_hex,
                candidate_hex=original_hex,
            )

        self.assertEqual(result.status.value, "inconclusive_timeout")
        self.assertTrue(
            any("constraint-build" in warning for warning in result.warnings),
            result.warnings,
        )


@unittest.skipUnless(importlib.util.find_spec("z3"), "z3-solver is not installed")
class PortfolioSolverTests(unittest.TestCase):
    def _prove(self, original_hex: str, candidate_hex: str, timeout_ms: int = 10_000):
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.decoder import decode_block, parse_hex
        from tools.ppc_equivalence.engine import check_equivalence

        ct = EquivalenceContract(parse_observables(["r3"]), timeout_ms=timeout_ms)
        return check_equivalence(
            decode_block(parse_hex(original_hex), validate_with_capstone=False),
            decode_block(parse_hex(candidate_hex), validate_with_capstone=False),
            ct,
            original_hex=original_hex,
            candidate_hex=candidate_hex,
        )

    def test_unsat_does_not_invoke_fallback(self) -> None:
        result = self._prove("4e800020", "4e800020")
        phases = result.solver.get("phases", [])
        self.assertEqual(len(phases), 1, "only default phase expected")
        self.assertEqual(phases[0]["result"], "unsat")
        self.assertEqual(result.status.value, "equivalent")

    def test_sat_does_not_invoke_fallback(self) -> None:
        result = self._prove("38630004 4e800020", "38630005 4e800020")
        phases = result.solver.get("phases", [])
        self.assertEqual(len(phases), 1, "only default phase expected")
        self.assertEqual(phases[0]["result"], "sat")
        self.assertEqual(result.status.value, "not_equivalent")

    def test_phases_list_structure(self) -> None:
        result = self._prove("4e800020", "4e800020")
        phases = result.solver.get("phases", [])
        self.assertIsInstance(phases, list)
        for phase in phases:
            self.assertIn("name", phase)
            self.assertIn("result", phase)
            self.assertIn("elapsed_ms", phase)

    def test_dataflow_contract_timeout_to_solver_dict(self) -> None:
        result = self._prove("4e800020", "4e800020", timeout_ms=5000)
        self.assertEqual(result.solver["timeout_ms"], 5000)

    def test_solver_contains_both_path_counts(self) -> None:
        result = self._prove("4e800020", "4e800020")
        self.assertGreater(result.solver["original_paths"], 0)
        self.assertGreater(result.solver["candidate_paths"], 0)

    # --- Portfolio logic (mocked solver) ---

    @staticmethod
    def _mock_solver(z3, results: list):
        """Create a z3.Solver whose .check() yields results in order.

        Assertions are deliberately unsatisfiable so a real tactic/solver
        fallback (which ignores the mock) still returns ``unsat``.
        """
        it = iter(results)
        def mock_check(*_a):
            try:
                return next(it)
            except StopIteration:
                return z3.unknown
        s = z3.Solver()
        x = z3.BitVec("x", 32)
        s.add(x != x)
        s.check = mock_check
        return s

    def test_unknown_invokes_fallback_with_reduced_budget(self) -> None:
        import z3
        from tools.ppc_equivalence.deadline import Deadline
        from tools.ppc_equivalence.engine import check_with_portfolio

        deadline = Deadline.after_ms(60_000)

        def build_solver():
            return self._mock_solver(z3, [z3.unknown, z3.unsat])

        solver, answer, tactic, phases = check_with_portfolio(
            z3, build_solver, deadline,
        )

        self.assertEqual(answer, z3.unsat)
        self.assertEqual(tactic, "bitblast")
        self.assertEqual(len(phases), 2)
        self.assertEqual(phases[0].name, "default")
        self.assertEqual(phases[0].result, "unknown")
        self.assertEqual(phases[1].name, "bitblast")
        self.assertEqual(phases[1].result, "unsat")
        # Fallback budget must be <= total deadline
        self.assertLess(phases[1].elapsed_ms, 60_000)

    def test_expired_deadline_before_fallback_returns_unknown(self) -> None:
        import z3
        from tools.ppc_equivalence.deadline import Deadline, ProofDeadlineExceeded
        from tools.ppc_equivalence.engine import check_with_portfolio

        # End_ns so tight that default solver call consumes the window
        deadline = Deadline(time.monotonic_ns() + 50_000)  # 50 µs

        def build_solver():
            return self._mock_solver(z3, [z3.unknown])

        solver, answer, tactic, phases = check_with_portfolio(
            z3, build_solver, deadline,
        )

        self.assertEqual(answer, z3.unknown)
        self.assertEqual(tactic, "default")
        self.assertGreaterEqual(len(phases), 2)
        self.assertEqual(phases[1].name, "bitblast")
        self.assertEqual(phases[1].result, "deadline-exceeded")

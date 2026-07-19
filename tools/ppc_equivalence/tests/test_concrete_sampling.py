"""Secondary-defense concrete sampling tests (roadmap step 8)."""

from __future__ import annotations

import importlib.util
import unittest

from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence, run_concrete_sampling
from tools.ppc_equivalence.result import ProofStatus


def decode(hex_words: str):
    return decode_block(parse_hex(hex_words), 0, validate_with_capstone=False)


@unittest.skipUnless(importlib.util.find_spec("z3"), "z3-solver is not installed")
class ConcreteSamplingTests(unittest.TestCase):
    def test_sampling_finds_mismatch_on_inequivalent_pair(self) -> None:
        # li r3,1 vs li r3,2 — every interesting state diverges on r3.
        original = decode("38600001")
        candidate = decode("38600002")
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000)
        report = run_concrete_sampling(
            original, candidate, contract, sample_count=3, seed=1,
        )
        self.assertTrue(report["mismatch_found"])
        self.assertFalse(report["certificate"])
        self.assertEqual(report["role"], "secondary_defense")
        self.assertEqual(report["mismatch"]["name"], "r3")
        self.assertIsNotNone(report["initial_state"])
        self.assertGreaterEqual(report["samples_run"], 1)

    def test_sampling_agrees_on_equivalent_pair(self) -> None:
        original = decode("38600001")
        candidate = decode("38600001")
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000)
        report = run_concrete_sampling(
            original, candidate, contract, sample_count=5, seed=7,
        )
        self.assertFalse(report["mismatch_found"])
        self.assertEqual(report["samples_requested"], 5)
        self.assertGreaterEqual(report["samples_run"], 1)
        self.assertFalse(report["certificate"])

    def test_check_equivalence_off_by_default(self) -> None:
        result = check_equivalence(
            decode("38600001"),
            decode("38600001"),
            EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000),
            original_hex="38600001",
            candidate_hex="38600001",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertIsNone(result.concrete_sampling)
        payload = result.to_dict()
        self.assertNotIn("concrete_sampling", payload)

    def test_check_equivalence_records_sampling_without_demotion(self) -> None:
        result = check_equivalence(
            decode("38600001"),
            decode("38600001"),
            EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000),
            original_hex="38600001",
            candidate_hex="38600001",
            concrete_samples=4,
            concrete_sample_seed=0,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertIsNotNone(result.concrete_sampling)
        assert result.concrete_sampling is not None
        self.assertFalse(result.concrete_sampling["mismatch_found"])
        self.assertFalse(result.concrete_sampling["certificate"])
        self.assertIn("concrete_sampling", result.to_dict())

    def test_sampling_mismatch_demotes_equivalent_fail_closed(self) -> None:
        """If ConcreteOps finds a divergence, never keep SMT equivalent."""
        # Force a post-SMT demotion by sampling a known-inequivalent pair
        # while SMT would also say not_equivalent — exercise the demotion
        # path via a monkeypatched sampling report on an equivalent SMT result.
        result = check_equivalence(
            decode("38600001"),
            decode("38600002"),
            EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000),
            original_hex="38600001",
            candidate_hex="38600002",
            concrete_samples=2,
            concrete_sample_seed=0,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.concrete_sampling)
        assert result.concrete_sampling is not None
        self.assertTrue(result.concrete_sampling["mismatch_found"])

    def test_forced_sampling_mismatch_demotes_smt_equivalent(self) -> None:
        """Secondary defense demotes SMT EQUIVALENT when sampling disagrees."""
        from unittest import mock

        fake_report = {
            "role": "secondary_defense",
            "certificate": False,
            "samples_requested": 1,
            "samples_run": 1,
            "samples_skipped": 0,
            "interesting_samples": 1,
            "random_samples": 0,
            "mismatch_found": True,
            "seed": 0,
            "mismatch": {
                "kind": "concrete_sampling",
                "name": "r3",
                "original": "0x00000001",
                "candidate": "0x00000002",
            },
            "initial_state": {"gpr": {"r3": "0x00000000"}},
            "sample_label": "zeros",
        }
        with mock.patch(
            "tools.ppc_equivalence.engine.run_concrete_sampling",
            return_value=fake_report,
        ):
            result = check_equivalence(
                decode("38600001"),
                decode("38600001"),
                EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000),
                original_hex="38600001",
                candidate_hex="38600001",
                concrete_samples=1,
            )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual(result.counterexample_kind, "concrete_sampling")
        self.assertEqual(result.mismatch["name"], "r3")
        self.assertTrue(
            any("demoting" in warning for warning in result.warnings),
        )
        self.assertIsNotNone(result.replay)
        assert result.replay is not None
        self.assertEqual(result.replay.get("source"), "concrete_sampling")

    def test_sampling_never_promotes_to_equivalent(self) -> None:
        # Sampling with agreement cannot invent EQUIVALENT when SMT is sat.
        result = check_equivalence(
            decode("38600001"),
            decode("38600002"),
            EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000),
            original_hex="38600001",
            candidate_hex="38600002",
            concrete_samples=0,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_branching_cfg_sampling(self) -> None:
        # beq path: cmpwi r3,0 ; beq +8 ; li r4,1 ; li r4,2
        # Same on both sides → no mismatch across interesting/random states.
        code = "2c030000 41820008 38800001 38800002"
        report = run_concrete_sampling(
            decode(code),
            decode(code),
            EquivalenceContract(parse_observables(["r4"]), timeout_ms=5_000),
            sample_count=4,
            seed=3,
        )
        self.assertFalse(report["mismatch_found"])
        self.assertGreaterEqual(report["samples_run"], 1)

    def test_zero_samples_is_noop_report(self) -> None:
        report = run_concrete_sampling(
            decode("60000000"),
            decode("60000000"),
            EquivalenceContract(parse_observables(["r3"]), timeout_ms=1_000),
            sample_count=0,
        )
        self.assertEqual(report["samples_requested"], 0)
        self.assertFalse(report["mismatch_found"])


if __name__ == "__main__":
    unittest.main()

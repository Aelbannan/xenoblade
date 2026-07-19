"""P2-03 counterexample reproduction bundle tests."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.diagnostics import (
    compute_first_divergence,
    differential_query_check,
    replay_counterexample,
)
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.result import ProofStatus

R3 = "r3"


def decode(text: str):
    return decode_block(parse_hex(text), validate_with_capstone=False)


def prove(original_hex: str, candidate_hex: str, *, observe=(R3,)):
    original = decode(original_hex)
    candidate = decode(candidate_hex)
    contract = make_contract(preset=None, observe=observe, timeout_ms=10_000)
    return check_equivalence(
        original,
        candidate,
        contract,
        original_hex=original_hex,
        candidate_hex=candidate_hex,
    )


class CounterexampleReproTests(unittest.TestCase):
    def test_bundle_present_for_addi_inequivalence(self) -> None:
        result = prove("38630001", "38630002")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample_bundle)
        bundle = result.counterexample_bundle
        self.assertEqual(bundle["original_bin"], "38630001")
        self.assertEqual(bundle["candidate_bin"], "38630002")
        self.assertIn("proof_request", bundle)
        self.assertIn("model_values", bundle)
        self.assertIn("relocations", bundle)
        self.assertIn("contract", bundle)
        self.assertIn("first_divergence", bundle)
        self.assertEqual(
            bundle["replay_command"],
            "python -m tools.ppc_equivalence replay <bundle_path> --json",
        )
        self.assertIsInstance(bundle["original_trace"], list)
        self.assertIsInstance(bundle["candidate_trace"], list)
        self.assertEqual(len(bundle["original_trace"]), 1)
        self.assertEqual(len(bundle["candidate_trace"]), 1)
        self.assertEqual(bundle["original_trace"][0]["step"], 0)
        self.assertIn("gpr", bundle["original_trace"][0])

    def test_first_divergence_identifies_step(self) -> None:
        result = prove("38630001", "38630002")
        div = result.counterexample_bundle["first_divergence"]
        self.assertIsNotNone(div)
        self.assertNotIn("error", div)
        self.assertEqual(div["step"], 0)
        self.assertEqual(div["component"], R3)
        # original = base+1, candidate = base+2; candidate - original should be 1
        orig = int(str(div["original"]), 0)
        cand = int(str(div["candidate"]), 0)
        self.assertEqual((cand - orig) & 0xFFFFFFFF, 1)

    def test_bundle_written_to_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / "bundle.json"
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
            self.assertIsNotNone(result.counterexample_bundle)
            out.write_text(json.dumps(result.counterexample_bundle))
            loaded = json.loads(out.read_text())
            self.assertEqual(loaded["original_bin"], "38630001")
            self.assertEqual(loaded["first_divergence"]["component"], R3)
            self.assertIsInstance(loaded["original_trace"], list)

    def test_symbolic_relocations_record_limitation(self) -> None:
        from tools.ppc_equivalence.elf_symbols import FunctionRelocation

        R = FunctionRelocation
        original = decode_block(
            parse_hex("386d0000"),
            relocations=(R(0, 109, "left", 0),),
            validate_with_capstone=False,
        )
        candidate = decode_block(
            parse_hex("386d0000"),
            relocations=(R(0, 109, "right", 0),),
            validate_with_capstone=False,
        )
        contract = make_contract(preset=None, observe=(R3,), timeout_ms=10_000)
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex="386d0000",
            candidate_hex="386d0000",
        )
        # Symbolic relocations block ConcreteOps; still a model-backed
        # inequivalence, with the limitation recorded in first_divergence.
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNone(result.replay)
        self.assertIsNotNone(result.counterexample_bundle)
        div = result.counterexample_bundle["first_divergence"]
        self.assertIn("error", div)
        self.assertEqual(div["error"], "symbolic-relocations-prevent-concrete-replay")
        self.assertIsNone(result.counterexample_bundle["original_trace"])

    def test_replay_helper_compares_states_across_sides(self) -> None:
        original = decode("38630001")
        candidate = decode("38630002")
        contract = make_contract(preset=None, observe=(R3,), timeout_ms=10_000)
        replay = replay_counterexample(
            original,
            candidate,
            {"gpr": {"r3": "0x10"}},
            contract,
        )
        self.assertTrue(replay["reproduced"])
        self.assertEqual(replay["first_divergence"]["step"], 0)
        self.assertEqual(replay["first_divergence"]["component"], R3)
        # Compatibility wrapper still returns the divergence dict.
        div = compute_first_divergence(
            original, candidate, {"gpr": {"r3": "0x10"}}, contract
        )
        self.assertEqual(div["component"], R3)

    def test_unreproduced_witness_becomes_internal_error(self) -> None:
        from unittest import mock

        original = decode("38630001")
        candidate = decode("38630002")
        contract = make_contract(preset=None, observe=(R3,), timeout_ms=10_000)
        fake_replay = {
            "reproduced": False,
            "error": "sat-witness-not-reproduced-under-concrete-ops",
            "original_trace": [{"step": 0}],
            "candidate_trace": [{"step": 0}],
            "first_divergence": {
                "error": "sat-witness-not-reproduced-under-concrete-ops",
                "step": -1,
                "component": None,
            },
        }
        with mock.patch(
            "tools.ppc_equivalence.diagnostics.replay_counterexample",
            return_value=fake_replay,
        ):
            result = check_equivalence(
                original,
                candidate,
                contract,
                original_hex="38630001",
                candidate_hex="38630002",
            )
        self.assertEqual(result.status, ProofStatus.INTERNAL_ERROR)
        self.assertIsNotNone(result.counterexample_bundle)
        self.assertEqual(
            result.counterexample_bundle["first_divergence"]["error"],
            "sat-witness-not-reproduced-under-concrete-ops",
        )
        self.assertIsNotNone(result.counterexample_bundle["original_trace"])

    def test_differential_query_checker_detects_disagreement(self) -> None:
        original = decode("38630001")
        candidate = decode("38630002")
        check = differential_query_check(original, candidate, bit_width=4)
        self.assertTrue(check["applicable"])
        self.assertFalse(check["agreed"])
        self.assertGreater(check["disagreement_count"], 0)

    def test_differential_query_checker_agrees_on_identical(self) -> None:
        original = decode("38630001")
        candidate = decode("38630001")
        check = differential_query_check(original, candidate, bit_width=4)
        self.assertTrue(check["applicable"])
        self.assertTrue(check["agreed"])
        self.assertEqual(check["disagreement_count"], 0)


if __name__ == "__main__":
    unittest.main()

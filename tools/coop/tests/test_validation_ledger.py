"""P1-06 validation ledger + promotion Gate 2/3 wiring."""

from __future__ import annotations

import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.equivalence_policy import (
    PromotionPolicy,
    ValidationLedger,
    classify_for_promotion,
    compute_confidence_tier,
    default_validation_ledger_path,
)
from tools.coop.tests.test_promotion_policy import _equivalent_proof
from tools.ppc_equivalence.ir import SUPPORTED_OPCODES, Opcode
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL
from tools.ppc_equivalence.tests.test_fixtures import PRIVILEGED_DECODE_ONLY


class ValidationLedgerLoadTests(unittest.TestCase):
    def test_default_yaml_ledger_loads(self) -> None:
        path = default_validation_ledger_path()
        self.assertTrue(path.is_file(), f"missing ledger at {path}")
        ledger = ValidationLedger.load(path)
        expected_dolphin = frozenset(
            op.value for op in (SUPPORTED_OPCODES - PRIVILEGED_DECODE_ONLY)
        )
        self.assertEqual(ledger.dolphin_validated_opcodes, expected_dolphin)
        self.assertEqual(len(ledger.dolphin_validated_opcodes), 200)
        self.assertEqual(ledger.architecture_model, ARCHITECTURE_MODEL)
        self.assertEqual(ledger.corpus_version, 1)
        self.assertIsNotNone(ledger.corpus_hash)
        corpus = Path("tools/ppc_equivalence/fixtures/broadway.jsonl")
        self.assertEqual(
            ledger.corpus_hash,
            hashlib.sha256(corpus.read_bytes()).hexdigest(),
        )

    def test_decode_only_opcodes_not_in_dolphin_set(self) -> None:
        ledger = ValidationLedger.load(default_validation_ledger_path())
        for op in PRIVILEGED_DECODE_ONLY:
            self.assertNotIn(op.value, ledger.dolphin_validated_opcodes)
            self.assertFalse(ledger.validate_opcode(op.value))

    def test_load_legacy_json_list(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "ledger.json"
            path.write_text(
                '{"dolphin_validated_opcodes": ["add", "addi"],'
                ' "dolphin_version": "x", "corpus_hash": "abc"}',
                encoding="utf-8",
            )
            ledger = ValidationLedger.load(path)
            self.assertEqual(ledger.dolphin_validated_opcodes, frozenset({"add", "addi"}))
            self.assertEqual(ledger.dolphin_version, "x")
            self.assertEqual(ledger.corpus_hash, "abc")

    def test_load_yaml_opcodes_shape(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "ledger.yaml"
            path.write_text(
                "\n".join(
                    [
                        f"architecture_model: {ARCHITECTURE_MODEL}",
                        "corpus_version: 1",
                        "opcodes:",
                        "  add:",
                        "    dolphin_interpreter: true",
                        "  sc:",
                        "    dolphin_interpreter: false",
                    ]
                )
                + "\n",
                encoding="utf-8",
            )
            ledger = ValidationLedger.load(path)
            self.assertEqual(ledger.dolphin_validated_opcodes, frozenset({"add"}))
            self.assertEqual(ledger.architecture_model, ARCHITECTURE_MODEL)


class LedgerPromotionTests(unittest.TestCase):
    def test_missing_opcode_demotes_tier(self) -> None:
        ledger = ValidationLedger(frozenset({"add"}))
        proof = _equivalent_proof(opcodes_used=["add", "psq_l"])
        self.assertEqual(compute_confidence_tier(proof, ledger), "C")

    def test_validated_opcodes_keep_tier_a(self) -> None:
        ledger = ValidationLedger(frozenset({"add", "addi"}))
        proof = _equivalent_proof(opcodes_used=["add", "addi"])
        self.assertEqual(compute_confidence_tier(proof, ledger), "A")

    def test_empty_opcodes_used_does_not_blunt_demote(self) -> None:
        """Legacy proofs without opcodes_used must not hit the old hack."""
        ledger = ValidationLedger(frozenset({"add"}))
        proof = _equivalent_proof(opcodes_used=[])
        self.assertEqual(compute_confidence_tier(proof, ledger), "A")

    def test_promotion_blocked_when_opcode_lacks_dolphin_evidence(self) -> None:
        ledger = ValidationLedger(frozenset({"add"}))
        proof = _equivalent_proof(opcodes_used=["add", Opcode.SC.value])
        decision = classify_for_promotion(proof, PromotionPolicy(), ledger)
        self.assertFalse(decision.allowed)
        self.assertEqual(decision.confidence_tier, "C")
        self.assertIn("opcode-sc-not-dolphin-validated", decision.blockers)
        self.assertIn("confidence-tier-C-not-allowed", decision.blockers)

    def test_default_ledger_blocks_decode_only_opcode(self) -> None:
        ledger = ValidationLedger.load(default_validation_ledger_path())
        proof = _equivalent_proof(opcodes_used=["add", "rfi"])
        decision = classify_for_promotion(proof, PromotionPolicy(), ledger)
        self.assertFalse(decision.allowed)
        self.assertIn("opcode-rfi-not-dolphin-validated", decision.blockers)


if __name__ == "__main__":
    unittest.main()

"""Certified call composition: precise summaries, widen, CLI assume hygiene (v22)."""

from __future__ import annotations

import importlib.util
import io
import json
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path

from tools.ppc_equivalence import cli
from tools.ppc_equivalence.callee_inference import infer_matched_callee_contracts
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.elf_symbols import FunctionRelocation
from tools.ppc_equivalence.semantics import infer_callee_contract
from tools.ppc_equivalence.tests.test_elf_symbols import build_reloc_elf

_HAS_Z3 = importlib.util.find_spec("z3") is not None
R = FunctionRelocation


def decode(text: str, relocations: tuple[FunctionRelocation, ...] = ()):
    return decode_block(
        parse_hex(text), relocations=relocations, validate_with_capstone=False,
    )


class InferCalleeCompositionTests(unittest.TestCase):
    def test_nested_precise_contracts_compose(self) -> None:
        leaf = infer_callee_contract(decode("38630001 4e800020"))
        self.assertEqual(leaf.source, "matched-body-effects")
        caller = infer_callee_contract(
            decode("48000001 4e800020", (R(0, 10, "leaf", 0),)),
            nested_contracts={"leaf": leaf},
        )
        self.assertEqual(caller.source, "matched-body-effects-composed")
        self.assertTrue(caller.reads >= leaf.reads)
        self.assertTrue(caller.writes >= (leaf.writes & caller.writes))

    def test_missing_nested_stays_opaque(self) -> None:
        caller = infer_callee_contract(
            decode("48000001 4e800020", (R(0, 10, "leaf", 0),)),
        )
        self.assertEqual(caller.source, "nested-call-opaque-eabi")

    def test_indirect_call_stays_opaque(self) -> None:
        # bctrl
        caller = infer_callee_contract(decode("4e800421"))
        self.assertEqual(caller.source, "nested-call-opaque-eabi")


@unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
class MatchedPairInferenceTests(unittest.TestCase):
    def test_leaf_pair_validates(self) -> None:
        leaf_bytes = bytes.fromhex("38630001 4e800020")
        elf = build_reloc_elf({"leaf": leaf_bytes})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "pair.o"
            path.write_bytes(elf)
            contracts = infer_matched_callee_contracts(
                frozenset({"leaf"}), path, path,
            )
        self.assertIn("leaf", contracts)
        self.assertEqual(
            contracts["leaf"].source,
            "validated-matched-pair-semantic-effects",
        )

    def test_divergent_leaf_bodies_not_authorized(self) -> None:
        """Effect-summary merge must not authorize functionally different leaves.

        Retail leaf: addi r3,r3,1; blr. Candidate: addi r3,r3,2; blr.
        Callers are both bl leaf; blr. Composition must not report equivalent.
        """
        leaf_retail = bytes.fromhex("38630001 4e800020")
        leaf_candidate = bytes.fromhex("38630002 4e800020")
        caller_bytes = bytes.fromhex("48000001 4e800020")
        retail_elf = build_reloc_elf(
            {"leaf": leaf_retail, "f": caller_bytes},
            relocations=((len(leaf_retail), "leaf", 10, 0),),
        )
        candidate_elf = build_reloc_elf(
            {"leaf": leaf_candidate, "f": caller_bytes},
            relocations=((len(leaf_candidate), "leaf", 10, 0),),
        )
        with tempfile.TemporaryDirectory() as tmp:
            left = Path(tmp) / "retail.o"
            right = Path(tmp) / "candidate.o"
            left.write_bytes(retail_elf)
            right.write_bytes(candidate_elf)
            contracts = infer_matched_callee_contracts(
                frozenset({"leaf"}), left, right,
            )
            self.assertNotIn(
                "leaf",
                contracts,
                "divergent leaves must not enter assumed_callees via effect merge",
            )
            out = io.StringIO()
            err = io.StringIO()
            with redirect_stdout(out), redirect_stderr(err):
                exit_code = cli.main([
                    "check-objects",
                    "--original", str(left),
                    "--candidate", str(right),
                    "--symbol", "f",
                    "--json",
                ])
            payload = json.loads(out.getvalue())
        self.assertNotEqual(payload["status"], "equivalent")
        self.assertNotEqual(exit_code, 0)
        self.assertEqual(payload.get("assumed_callees") or [], [])
        self.assertIn(
            payload["status"],
            {"inconclusive_unsupported", "inconclusive_unvalidated_callee"},
        )

    def test_nested_caller_recomposes_after_leaf(self) -> None:
        leaf_bytes = bytes.fromhex("38630001 4e800020")
        # bl leaf ; blr
        caller_bytes = bytes.fromhex("48000001 4e800020")
        elf = build_reloc_elf(
            {"leaf": leaf_bytes, "caller": caller_bytes},
            relocations=((len(leaf_bytes), "leaf", 10, 0),),
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "pair.o"
            path.write_bytes(elf)
            contracts = infer_matched_callee_contracts(
                frozenset({"leaf", "caller"}), path, path,
            )
        self.assertEqual(
            contracts["leaf"].source,
            "validated-matched-pair-semantic-effects",
        )
        self.assertNotEqual(contracts["caller"].source, "nested-call-opaque-eabi")
        self.assertNotIn("*", contracts["caller"].reads)
        self.assertNotIn("*", contracts["caller"].writes)
        # Leaf validates; caller may validate-compose or widen after recompose.
        self.assertTrue(
            contracts["caller"].source.endswith("composed")
            or contracts["caller"].source in {
                "matched-pair-body-effects",
                "validation-widened-matched-pair-effects",
            },
            contracts["caller"].source,
        )


@unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
class CliAssumeHygieneTests(unittest.TestCase):
    def test_check_objects_infers_leaf_without_assume_flag(self) -> None:
        relocated = build_reloc_elf(
            {
                "f": bytes.fromhex("48000001 38630004 4e800020"),
                "leaf": bytes.fromhex("4e800020"),
            },
            relocations=((0, "leaf", 10, 0),),
        )
        with tempfile.TemporaryDirectory() as tmp:
            left = Path(tmp) / "a.o"
            right = Path(tmp) / "b.o"
            left.write_bytes(relocated)
            right.write_bytes(relocated)
            out = io.StringIO()
            err = io.StringIO()
            with redirect_stdout(out), redirect_stderr(err):
                exit_code = cli.main([
                    "check-objects",
                    "--original", str(left),
                    "--candidate", str(right),
                    "--symbol", "f",
                    "--json",
                ])
            payload = json.loads(out.getvalue())
        self.assertEqual(exit_code, 0)
        self.assertEqual(payload["status"], "equivalent")
        self.assertEqual(payload["assumed_callees"], ["leaf"])
        source = payload["callee_contracts"]["leaf"]["source"]
        self.assertNotEqual(source, "opaque-eabi")
        self.assertNotIn("assuming opaque EABI", err.getvalue())

    def test_hex_check_does_not_silently_assume(self) -> None:
        # bl leaf ; addi r3,r3,4 ; blr — bodies exist but inference is off.
        relocated = build_reloc_elf(
            {
                "f": bytes.fromhex("48000001 38630004 4e800020"),
                "leaf": bytes.fromhex("4e800020"),
            },
            relocations=((0, "leaf", 10, 0),),
        )
        with tempfile.TemporaryDirectory() as tmp:
            left = Path(tmp) / "a.o"
            right = Path(tmp) / "b.o"
            left.write_bytes(relocated)
            right.write_bytes(relocated)
            out = io.StringIO()
            err = io.StringIO()
            with redirect_stdout(out), redirect_stderr(err):
                exit_code = cli.main([
                    "check-objects",
                    "--original", str(left),
                    "--candidate", str(right),
                    "--symbol", "f",
                    "--no-infer-matched-callees",
                    "--json",
                ])
            payload = json.loads(out.getvalue())
        self.assertEqual(exit_code, 2)
        self.assertEqual(payload["status"], "inconclusive_unsupported")
        self.assertIn("will be inconclusive", err.getvalue())
        self.assertEqual(payload.get("assumed_callees") or [], [])

    def test_assume_relocated_callees_opt_in(self) -> None:
        relocated = build_reloc_elf(
            {
                "f": bytes.fromhex("48000001 38630004 4e800020"),
                "leaf": bytes.fromhex("4e800020"),
            },
            relocations=((0, "leaf", 10, 0),),
        )
        with tempfile.TemporaryDirectory() as tmp:
            left = Path(tmp) / "a.o"
            right = Path(tmp) / "b.o"
            left.write_bytes(relocated)
            right.write_bytes(relocated)
            out = io.StringIO()
            err = io.StringIO()
            with redirect_stdout(out), redirect_stderr(err):
                exit_code = cli.main([
                    "check-objects",
                    "--original", str(left),
                    "--candidate", str(right),
                    "--symbol", "f",
                    "--no-infer-matched-callees",
                    "--assume-relocated-callees",
                    "--json",
                ])
            payload = json.loads(out.getvalue())
        self.assertEqual(exit_code, 0)
        self.assertEqual(payload["status"], "equivalent")
        self.assertEqual(payload["assumed_callees"], ["leaf"])
        self.assertEqual(
            payload["callee_contracts"]["leaf"]["source"],
            "opaque-eabi",
        )
        self.assertIn("assuming opaque EABI", err.getvalue())


if __name__ == "__main__":
    unittest.main()


class StoreSourceReadCaptureTests(unittest.TestCase):
    """F1 (round-8 adversarial review): a store's source register must appear
    in the validated contract's reads.

    ``_validate_callee_contract_impl`` collects reads from the final values of
    WRITTEN components plus terminal conditions; a memory write's final value
    is deliberately excluded from that scan (huge Store/Select cones), so a
    store-only callee (``stw r11, 0(r3); blr``) validated to
    reads={memory, r3, valid} — the ADDRESS r3 was captured only via the
    store's definedness constraint, but the stored VALUE r11 was dropped.
    The witness's gate-5 call-observed rule then left r11 free to rename at a
    call site and the F3 token canonicalization hid the divergence: a caller
    pair renaming r11<->r12 (dead at the call) around such a callee certified
    while the physical substitution stores a different value.  The validation
    now enumerates the register reads of memory-writing instructions
    directly (register_effects), so the store source is captured.
    """

    def test_store_source_register_in_validated_reads(self) -> None:
        leaf_bytes = bytes.fromhex("91630000 4e800020")  # stw r11,0(r3); blr
        elf = build_reloc_elf({"leaf": leaf_bytes})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "pair.o"
            path.write_bytes(elf)
            contracts = infer_matched_callee_contracts(
                frozenset({"leaf"}), path, path,
            )
        self.assertIn("leaf", contracts)
        contract = contracts["leaf"]
        self.assertIn("memory", contract.reads)
        self.assertIn("r11", contract.reads, contract)
        self.assertIn("r3", contract.reads)

    def test_fpr_and_ps1_store_sources_captured(self) -> None:
        # stfd f20,8(r1); blr  ->  reads f20 (FPR store source)
        leaf_bytes = bytes.fromhex("da810008 4e800020")
        elf = build_reloc_elf({"leaf": leaf_bytes})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "pair.o"
            path.write_bytes(elf)
            contracts = infer_matched_callee_contracts(
                frozenset({"leaf"}), path, path,
            )
        self.assertIn("leaf", contracts)
        self.assertIn("f20", contracts["leaf"].reads, contracts["leaf"])

    def test_store_constant_callee_unchanged(self) -> None:
        # li r11,7; stw r11,0(r3); blr — r11 written before the store is a
        # def, not a read; the contract must NOT list r11 as an input.
        leaf_bytes = bytes.fromhex("39600007 90830000 4e800020")
        elf = build_reloc_elf({"leaf": leaf_bytes})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "pair.o"
            path.write_bytes(elf)
            contracts = infer_matched_callee_contracts(
                frozenset({"leaf"}), path, path,
            )
        self.assertIn("leaf", contracts)
        self.assertNotIn("r11", contracts["leaf"].reads, contracts["leaf"])
        self.assertIn("memory", contracts["leaf"].reads)

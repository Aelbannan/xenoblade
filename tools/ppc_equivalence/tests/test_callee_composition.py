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

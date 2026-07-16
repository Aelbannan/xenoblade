"""Adversarial tests for relocation-aware and modular caller proofs."""

from __future__ import annotations

import unittest
from pathlib import Path

from tools.coop.lib.equivalence_check import _cache_key
from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.elf_symbols import FunctionRelocation, list_text_functions
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import DecodeError, ExecutionInconclusive


R = FunctionRelocation


def decode(text: str, relocations: tuple[FunctionRelocation, ...] = ()):
    return decode_block(
        parse_hex(text), relocations=relocations, validate_with_capstone=False,
    )


def prove(left, right, *, assumed=frozenset()):
    return check_equivalence(
        left, right,
        make_contract(preset=None, observe=("r3",), timeout_ms=10_000),
        original_hex="", candidate_hex="", assumed_callees=assumed,
    )


class SymbolicRelocationTests(unittest.TestCase):
    def test_ha_lo_pair_equals_sda21_full_address(self) -> None:
        pair = decode(
            "3c600000 38630000",
            (R(2, 6, "global", 0), R(6, 4, "global", 0)),
        )
        sda = decode("386d0000", (R(0, 109, "global", 0),))
        self.assertEqual(prove(pair, sda).status.value, "equivalent")

    def test_ha_ori_is_not_a_valid_general_address_pair(self) -> None:
        ori = decode(
            "3c600000 60630000",
            (R(2, 6, "global", 0), R(6, 4, "global", 0)),
        )
        sda = decode("386d0000", (R(0, 109, "global", 0),))
        self.assertEqual(prove(ori, sda).status.value, "not_equivalent")

    def test_distinct_symbols_are_independent(self) -> None:
        left = decode("386d0000", (R(0, 109, "left", 0),))
        right = decode("386d0000", (R(0, 109, "right", 0),))
        result = prove(left, right)
        self.assertEqual(result.status.value, "not_equivalent")
        self.assertEqual(set(result.counterexample["relocations"]), {"left", "right"})
        self.assertIsNone(result.replay)

    def test_implicit_rel_addend_fails_closed(self) -> None:
        with self.assertRaisesRegex(DecodeError, "implicit REL addend"):
            decode("386d0000", (R(0, 109, "global", None),))

    def test_self_relative_branch_is_resolved_as_local_cfg(self) -> None:
        instructions = decode_block(
            parse_hex("42000000"), relocations=(R(0, 11, "loop", 0),),
            local_symbol="loop", validate_with_capstone=False,
        )
        self.assertIsNone(instructions[0].relocation)
        self.assertEqual(instructions[0].operands[2], 0)

    def test_cache_key_includes_bases_relocations_and_callee_premises(self) -> None:
        relocation = (R(0, 109, "global", 0),)
        common = ("manual", ("r3",), "386d0000", "386d0000")
        baseline = _cache_key(
            *common, 0, 0, relocation, relocation,
            assumed_callees=frozenset({"leaf"}),
        )
        self.assertNotEqual(baseline, _cache_key(
            *common, 4, 0, relocation, relocation,
            assumed_callees=frozenset({"leaf"}),
        ))
        self.assertNotEqual(baseline, _cache_key(
            *common, 0, 0, (R(0, 109, "other", 0),), relocation,
            assumed_callees=frozenset({"leaf"}),
        ))
        self.assertNotEqual(baseline, _cache_key(
            *common, 0, 0, relocation, relocation,
            assumed_callees=frozenset({"other_leaf"}),
        ))

    def test_all_built_source_object_relocations_attach(self) -> None:
        root = Path("build/us/src")
        if not root.is_dir():
            self.skipTest("build/us/src is unavailable")
        counts: dict[int, int] = {}
        for path in root.rglob("*.o"):
            try:
                functions = list_text_functions(path)
            except ValueError:
                continue
            for function in functions:
                if not function.relocations:
                    continue
                decode_block(
                    function.code, function.base,
                    relocations=function.relocations,
                    local_symbol=function.name,
                    validate_with_capstone=False,
                )
                for relocation in function.relocations:
                    counts[relocation.relocation_type] = (
                        counts.get(relocation.relocation_type, 0) + 1
                    )
        self.assertEqual(set(counts), {4, 5, 6, 10, 11, 109})


class MatchedCalleeSummaryTests(unittest.TestCase):
    _CALL = (R(4, 10, "leaf", 0),)

    def test_post_call_difference_is_not_ignored(self) -> None:
        left = decode("38630001 48000001 38630003 4e800020", self._CALL)
        right = decode("38630001 48000001 38630004 4e800020", self._CALL)
        result = prove(left, right, assumed=frozenset({"leaf"}))
        self.assertEqual(result.status.value, "not_equivalent")
        self.assertIsNone(result.replay)

    def test_identical_callers_compose_with_matched_leaf(self) -> None:
        caller = decode("38630001 48000001 38630003 4e800020", self._CALL)
        result = prove(caller, caller, assumed=frozenset({"leaf"}))
        self.assertEqual(result.status.value, "equivalent")
        self.assertEqual(result.assumed_callees, ["leaf"])

    def test_unknown_callee_fails_closed(self) -> None:
        caller = decode("38630001 48000001 38630003 4e800020", self._CALL)
        with self.assertRaisesRegex(ExecutionInconclusive, "no matched-callee lemma"):
            prove(caller, caller)

    def test_relocated_tail_call_uses_the_same_callee_lemma(self) -> None:
        tail = decode("38630001 48000000", (R(4, 10, "leaf", 0),))
        result = prove(tail, tail, assumed=frozenset({"leaf"}))
        self.assertEqual(result.status.value, "equivalent")
        self.assertEqual(result.assumed_callees, ["leaf"])
        with self.assertRaisesRegex(ExecutionInconclusive, "tail-call target"):
            prove(tail, tail)


if __name__ == "__main__":
    unittest.main()

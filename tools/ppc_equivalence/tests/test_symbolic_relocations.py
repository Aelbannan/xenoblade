"""Adversarial tests for relocation-aware and modular caller proofs."""

from __future__ import annotations

import unittest
from pathlib import Path

from tools.coop.lib.equivalence_check import _cache_key
from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex, specialize_relocations
from tools.ppc_equivalence.elf_symbols import FunctionRelocation, list_text_functions
from tools.ppc_equivalence.engine import check_equivalence, validate_callee_contract
from tools.ppc_equivalence.ir import (
    DecodeError,
    ExecutionInconclusive,
    RelocationRef,
    R_PPC_ADDR16_HA,
    R_PPC_ADDR16_LO,
    R_PPC_EMB_SDA21,
    R_PPC_REL24,
)
from tools.ppc_equivalence.semantics import CalleeContract, infer_callee_contract


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
        contract = CalleeContract(frozenset({"r3"}), frozenset({"r3"}), "test")
        self.assertNotEqual(baseline, _cache_key(
            *common, 0, 0, relocation, relocation,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": contract},
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

    def test_symbolic_pair_matches_independently_linked_boundary_values(self) -> None:
        raw = parse_hex("3c600000 38630000")
        refs = (
            RelocationRef(2, R_PPC_ADDR16_HA, "global", "global", 0),
            RelocationRef(6, R_PPC_ADDR16_LO, "global", "global", 0),
        )
        symbolic = decode(
            "3c600000 38630000",
            (R(2, R_PPC_ADDR16_HA, "global", 0), R(6, R_PPC_ADDR16_LO, "global", 0)),
        )
        for address in (0x12347FFF, 0x12348000, 0xFFFF8000):
            linked = decode_block(
                specialize_relocations(raw, 0, refs, {"global": address}),
                validate_with_capstone=False,
            )
            result = check_equivalence(
                symbolic, linked,
                make_contract(preset=None, observe=("r3",), timeout_ms=10_000),
                original_hex="", candidate_hex="",
                relocation_bindings={"global": address},
            )
            self.assertEqual(result.status.value, "equivalent")

    def test_concrete_linker_rejects_impossible_branch_layout(self) -> None:
        ref = RelocationRef(0, R_PPC_REL24, "far", "far", 0)
        with self.assertRaisesRegex(DecodeError, "outside"):
            specialize_relocations(parse_hex("48000000"), 0, (ref,), {"far": 0x02000000})

    def test_symbolic_impossible_branch_binding_is_inconclusive_layout(self) -> None:
        caller = decode("48000001 4e800020", (R(0, R_PPC_REL24, "far", 0),))
        result = check_equivalence(
            caller, caller,
            make_contract(preset=None, observe=("r3",), timeout_ms=10_000),
            original_hex="", candidate_hex="", assumed_callees=frozenset({"far"}),
            relocation_bindings={"far": 0x02000000},
        )
        self.assertEqual(result.status.value, "inconclusive_layout")

    def test_sda21_specialization_patches_base_register_and_displacement(self) -> None:
        ref = RelocationRef(0, R_PPC_EMB_SDA21, "small", "small", 0)
        linked = specialize_relocations(
            parse_hex("80600000"), 0, (ref,), {"small": 0x80400020},
            sda_bases={13: 0x80400000},
        )
        word = int.from_bytes(linked, "big")
        self.assertEqual((word >> 16) & 0x1F, 13)
        self.assertEqual(word & 0xFFFF, 0x20)


class MatchedCalleeSummaryTests(unittest.TestCase):
    _CALL = (R(4, 10, "leaf", 0),)

    def test_post_call_difference_is_not_ignored(self) -> None:
        left = decode("38630001 48000001 38630003 4e800020", self._CALL)
        right = decode("38630001 48000001 38630004 4e800020", self._CALL)
        result = prove(left, right, assumed=frozenset({"leaf"}))
        self.assertEqual(result.status.value, "inconclusive_abstraction")
        self.assertEqual(result.counterexample_kind, "abstract")
        self.assertIsNone(result.replay)

    def test_identical_callers_compose_with_matched_leaf(self) -> None:
        caller = decode("38630001 48000001 38630003 4e800020", self._CALL)
        result = prove(caller, caller, assumed=frozenset({"leaf"}))
        self.assertEqual(result.status.value, "equivalent")
        self.assertEqual(result.callee_contracts["leaf"]["source"], "opaque-eabi")
        self.assertEqual(result.assumed_callees, ["leaf"])

    def test_unknown_callee_fails_closed(self) -> None:
        caller = decode("38630001 48000001 38630003 4e800020", self._CALL)
        with self.assertRaisesRegex(ExecutionInconclusive, "no matched-callee lemma"):
            prove(caller, caller)

    def test_relocated_tail_call_uses_the_same_callee_lemma(self) -> None:
        tail = decode("38630001 48000000", (R(4, 10, "leaf", 0),))
        result = prove(tail, tail, assumed=frozenset({"leaf"}))
        self.assertEqual(result.status.value, "equivalent")
        self.assertEqual(result.callee_contracts["leaf"]["source"], "opaque-eabi")
        self.assertEqual(result.assumed_callees, ["leaf"])
        with self.assertRaisesRegex(ExecutionInconclusive, "tail-call target"):
            prove(tail, tail)

    def test_read_sensitive_contract_ignores_irrelevant_input(self) -> None:
        left = decode("38800001 48000001 4e800020", self._CALL)
        right = decode("38800002 48000001 4e800020", self._CALL)
        contract = CalleeContract(
            frozenset({"r3"}), frozenset({"r3", "valid"}), "test-leaf",
        )
        result = check_equivalence(
            left, right,
            make_contract(preset=None, observe=("r3",), timeout_ms=10_000),
            original_hex="", candidate_hex="", assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": contract},
        )
        self.assertEqual(result.status.value, "equivalent")
        self.assertEqual(result.callee_contracts["leaf"]["source"], "test-leaf")

    def test_contract_inference_tracks_leaf_effects_and_rejects_tail_composition(self) -> None:
        leaf = infer_callee_contract(decode("38630001 4e800020"))
        self.assertIn("r3", leaf.reads)
        self.assertIn("r3", leaf.writes)
        self.assertNotIn("memory", leaf.writes)
        tail = infer_callee_contract(decode("48000000", (R(0, 10, "other", 0),)))
        self.assertEqual(tail.source, "nested-call-opaque-eabi")
        self.assertIn("*", tail.reads)

    def test_semantic_contract_validation_rejects_missing_reads_and_writes(self) -> None:
        instructions = decode("7c632214 4e800020")  # add r3,r3,r4 ; blr
        inferred = infer_callee_contract(instructions)
        self.assertTrue(validate_callee_contract(instructions, inferred).valid)

        missing_read = CalleeContract(
            frozenset({"r3", "valid"}), frozenset({"r3", "valid"}), "bad-read",
        )
        read_validation = validate_callee_contract(instructions, missing_read)
        self.assertFalse(read_validation.valid)
        self.assertEqual(read_validation.missing_reads, frozenset({"r4"}))

        missing_write = CalleeContract(
            frozenset({"r3", "r4", "valid"}), frozenset({"valid"}), "bad-write",
        )
        write_validation = validate_callee_contract(instructions, missing_write)
        self.assertFalse(write_validation.valid)
        self.assertEqual(write_validation.missing_writes, frozenset({"r3"}))

    def test_certificate_validation_requires_normal_return(self) -> None:
        fallthrough = decode("38630001")
        validation = validate_callee_contract(
            fallthrough, CalleeContract.opaque_eabi(), require_normal_return=True,
        )
        self.assertFalse(validation.valid)
        self.assertIn("normal returns", validation.reason)

    def test_certificate_validation_composes_with_supplied_callee_contract(self) -> None:
        caller = decode(
            "48000000",
            (R(0, R_PPC_REL24, "leaf", 0),),
        )
        validation = validate_callee_contract(
            caller,
            CalleeContract.opaque_eabi(),
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": CalleeContract.opaque_eabi()},
            require_normal_return=True,
        )
        self.assertTrue(validation.valid, validation.reason)

    def test_semantic_validation_distinguishes_private_and_escaped_stack_memory(self) -> None:
        private = decode("3821fff0 98610008 38210010 4e800020")
        private_validation = validate_callee_contract(
            private, infer_callee_contract(private),
        )
        self.assertTrue(private_validation.valid)
        self.assertNotIn("memory", private_validation.required_writes)

        escaped = decode(
            "3821fff0 38810008 90850000 38210010 4e800020"
        )
        escaped_validation = validate_callee_contract(
            escaped, infer_callee_contract(escaped),
        )
        self.assertTrue(escaped_validation.valid)
        self.assertIn("memory", escaped_validation.required_writes)

    def test_memory_writing_opaque_call_does_not_preserve_aliased_caller_frame(self) -> None:
        left = decode(
            "9421fff0 39c00007 99c10008 48000001 88610008 38210010 4e800020",
            (R(12, 10, "leaf", 0),),
        )
        right = decode(
            "9421fff0 39c00007 99c10008 48000001 38600007 38210010 4e800020",
            (R(12, 10, "leaf", 0),),
        )
        result = prove(left, right, assumed=frozenset({"leaf"}))
        self.assertEqual(result.status.value, "inconclusive_abstraction")

    def test_memory_free_contract_can_preserve_caller_frame(self) -> None:
        left = decode(
            "9421fff0 39c00007 99c10008 48000001 88610008 38210010 4e800020",
            (R(12, 10, "leaf", 0),),
        )
        right = decode(
            "9421fff0 39c00007 99c10008 48000001 38600007 38210010 4e800020",
            (R(12, 10, "leaf", 0),),
        )
        contract = CalleeContract(
            frozenset({"r3", "valid"}), frozenset({"r3", "valid"}), "memory-free-test",
        )
        result = check_equivalence(
            left, right,
            make_contract(preset=None, observe=("r3",), timeout_ms=10_000),
            original_hex="", candidate_hex="", assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": contract},
        )
        self.assertEqual(result.status.value, "equivalent")


class PrivateStackMemoryTests(unittest.TestCase):
    def test_private_stack_bytes_are_not_observable(self) -> None:
        left = decode("3821fff0 98610008 38210010 4e800020")
        right = decode("4e800020")
        result = check_equivalence(
            left, right,
            make_contract(preset=None, observe=("memory",), timeout_ms=10_000),
            original_hex="", candidate_hex="",
        )
        self.assertEqual(result.status.value, "equivalent")

    def test_write_above_entry_stack_pointer_remains_observable(self) -> None:
        left = decode("98610008 4e800020")
        right = decode("4e800020")
        result = check_equivalence(
            left, right,
            make_contract(preset=None, observe=("memory",), timeout_ms=10_000),
            original_hex="", candidate_hex="",
        )
        self.assertEqual(result.status.value, "not_equivalent")

    def test_escaped_stack_pointer_disables_private_stack_mask(self) -> None:
        left = decode(
            "3821fff0 38600001 98610008 38810008 90850000 38210010 4e800020"
        )
        right = decode(
            "3821fff0 38600002 98610008 38810008 90850000 38210010 4e800020"
        )
        result = check_equivalence(
            left, right,
            make_contract(preset=None, observe=("memory",), timeout_ms=10_000),
            original_hex="", candidate_hex="",
        )
        self.assertEqual(result.status.value, "not_equivalent")


if __name__ == "__main__":
    unittest.main()

from __future__ import annotations

import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.run import _equivalence_args_with_default_contract
from tools.ppc_equivalence.contract import (
    EquivalenceContract,
    make_contract,
    parse_observables,
    preset_observable_names,
)
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import Opcode, UnsupportedInstruction
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, execute_block, rotate_mask


def decode(hex_words: str):
    return decode_block(parse_hex(hex_words), validate_with_capstone=False)


class DecoderTests(unittest.TestCase):
    def test_known_encodings(self) -> None:
        fixture = (
            "38630004 3c630004 1c630004 "
            "60630004 64630004 68630004 6c630004 70630004 74630004 "
            "5063103a 5463103a "
            "7c632038 7c632378 7c632278 7c6320f8 7c632214 7c632050 "
            "2c030004 28030004 7c032000 7c032040 "
            "7c830774 7c830734 7c830034"
        )
        instructions = decode(fixture)
        self.assertEqual(
            [item.opcode for item in instructions],
            [
                Opcode.ADDI,
                Opcode.ADDIS,
                Opcode.MULLI,
                Opcode.ORI,
                Opcode.ORIS,
                Opcode.XORI,
                Opcode.XORIS,
                Opcode.ANDI_DOT,
                Opcode.ANDIS_DOT,
                Opcode.RLWIMI,
                Opcode.RLWINM,
                Opcode.AND,
                Opcode.OR,
                Opcode.XOR,
                Opcode.NOR,
                Opcode.ADD,
                Opcode.SUBF,
                Opcode.CMPWI,
                Opcode.CMPLWI,
                Opcode.CMPW,
                Opcode.CMPLW,
                Opcode.EXTSB,
                Opcode.EXTSH,
                Opcode.CNTLZW,
            ],
        )
        self.assertEqual(instructions[0].operands, (3, 3, 4))
        self.assertEqual(instructions[10].operands, (3, 3, 2, 0, 29))

    @unittest.skipUnless(importlib.util.find_spec("capstone"), "Capstone is not installed")
    def test_capstone_cross_checks_every_supported_family(self) -> None:
        code = parse_hex(
            "38630004 1c630004 60630004 70630004 5063103a 5463103a "
            "7c632038 7c632378 7c632278 7c6320f8 7c632214 7c632050 "
            "2c030004 28030004 7c032000 7c032040 7c830774 7c830734 7c830034"
        )
        self.assertEqual(len(decode_block(code)), 19)

    def test_unsupported_branch_is_explicit(self) -> None:
        with self.assertRaises(UnsupportedInstruction):
            decode("48000000")

    def test_reserved_compare_form_is_rejected(self) -> None:
        # cmpw r3,r4 with reserved Rc bit set.
        with self.assertRaises(UnsupportedInstruction):
            decode("7c032001")

    def test_rejects_partial_word(self) -> None:
        with self.assertRaises(ValueError):
            parse_hex("386300")
        with self.assertRaises(ValueError):
            parse_hex("3863zzzz")

    def test_rejects_wrapping_address_range(self) -> None:
        with self.assertRaises(ValueError):
            decode_block(parse_hex("38630004 38630004"), 0xFFFFFFFC, validate_with_capstone=False)


class PrimitiveTests(unittest.TestCase):
    def test_rotate_masks_exhaustive_against_reference(self) -> None:
        for mb in range(32):
            for me in range(32):
                expected = 0
                active = mb
                while True:
                    expected |= 1 << (31 - active)
                    if active == me:
                        break
                    active = (active + 1) & 31
                self.assertEqual(rotate_mask(mb, me), expected)


class ConcreteSemanticsTests(unittest.TestCase):
    def test_addi_ra_zero_does_not_read_r0(self) -> None:
        state = concrete_state({"gpr": {"r0": "0xffffffff"}})
        final = execute_block(state, decode("38600004"), ConcreteOps())
        self.assertEqual(final.gpr[3], 4)
        self.assertEqual(final.gpr[0], 0xFFFFFFFF)

    def test_rotate_and_multiply_agree_on_edges(self) -> None:
        original = decode("5463103a")
        candidate = decode("1c630004")
        for value in (0, 1, 0x3FFFFFFF, 0x40000000, 0x80000000, 0xFFFFFFFF):
            state = concrete_state({"gpr": {"r3": value}})
            left = execute_block(state, original, ConcreteOps())
            right = execute_block(state, candidate, ConcreteOps())
            self.assertEqual(left.gpr[3], right.gpr[3])

    def test_record_form_updates_only_cr0_field(self) -> None:
        # andi. r3,r4,0xffff
        state = concrete_state({"gpr": {"r4": 0}, "cr": "0x01234567", "xer": {"so": 1}})
        final = execute_block(state, decode("7083ffff"), ConcreteOps())
        self.assertEqual(final.gpr[3], 0)
        self.assertEqual(final.cr & 0x0FFFFFFF, 0x01234567)
        self.assertEqual((final.cr >> 28) & 0xF, 0x3)  # EQ | SO


@unittest.skipUnless(importlib.util.find_spec("z3"), "z3-solver is not installed")
class SymbolicEquivalenceTests(unittest.TestCase):
    def check(self, original: str, candidate: str, observe: str):
        return check_equivalence(
            decode(original),
            decode(candidate),
            EquivalenceContract(parse_observables([observe]), timeout_ms=5_000),
            original_hex=original,
            candidate_hex=candidate,
        )

    def test_byte_different_shift_and_multiply_are_equivalent(self) -> None:
        result = self.check("5463103a", "1c630004", "r3")
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_off_by_one_produces_replayable_counterexample(self) -> None:
        result = self.check("38630004", "38630005", "r3")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual(result.mismatch["name"], "r3")
        self.assertIsNotNone(result.replay)

        assert result.replay is not None
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "case.json"
            path.write_text(json.dumps(result.replay), encoding="utf-8")
            case = json.loads(path.read_text(encoding="utf-8"))
            initial = concrete_state(case["initial_state"])
            left = execute_block(initial, decode(case["original_hex"]), ConcreteOps())
            right = execute_block(initial, decode(case["candidate_hex"]), ConcreteOps())
            self.assertNotEqual(left.gpr[3], right.gpr[3])

    def test_dead_register_is_contract_controlled(self) -> None:
        # Candidate additionally writes r4. r3 remains identical.
        equivalent = self.check("38630004", "38630004 38800001", "r3")
        inequivalent = self.check("38630004", "38630004 38800001", "r4")
        self.assertEqual(equivalent.status, ProofStatus.EQUIVALENT)
        self.assertEqual(inequivalent.status, ProofStatus.NOT_EQUIVALENT)

    def test_abi_and_strict_presets_treat_volatile_scratch_differently(self) -> None:
        original = decode("38630004")
        candidate = decode("38630004 38a00001")  # additionally writes volatile r5
        abi = check_equivalence(
            original,
            candidate,
            make_contract(preset="ppc-eabi", observe=None, timeout_ms=5_000),
            original_hex="38630004",
            candidate_hex="38630004 38a00001",
        )
        strict = check_equivalence(
            original,
            candidate,
            make_contract(preset="strict", observe=None, timeout_ms=5_000),
            original_hex="38630004",
            candidate_hex="38630004 38a00001",
        )
        self.assertEqual(abi.status, ProofStatus.EQUIVALENT)
        self.assertEqual(strict.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual(abi.contract, "ppc-eabi")
        self.assertEqual(strict.contract, "strict")


class ContractTests(unittest.TestCase):
    def test_observables_are_versioned_and_validated(self) -> None:
        values = parse_observables(["r3,cr0", "xer.ca", "r3"])
        self.assertEqual([value.name for value in values], ["r3", "cr0", "xer.ca"])
        with self.assertRaises(ValueError):
            parse_observables(["memory"])
        with self.assertRaises(ValueError):
            EquivalenceContract(values, timeout_ms=0)

    def test_ppc_eabi_preset(self) -> None:
        names = preset_observable_names("ppc-eabi")
        self.assertEqual(names[:5], ("r1", "r2", "r3", "r4", "r13"))
        self.assertIn("r31", names)
        self.assertEqual(names[-3:], ("cr2", "cr3", "cr4"))
        self.assertNotIn("r5", names)
        self.assertNotIn("cr0", names)

    def test_strict_preset_covers_all_modeled_state(self) -> None:
        names = preset_observable_names("strict")
        self.assertEqual(names[:32], tuple(f"r{index}" for index in range(32)))
        self.assertEqual(names[32:], ("cr", "xer.ca", "xer.ov", "xer.so", "lr", "ctr"))

    def test_coop_runner_defaults_to_ppc_eabi(self) -> None:
        args = _equivalence_args_with_default_contract(
            ["check-hex", "--original", "38630004", "--candidate", "38630004"]
        )
        self.assertEqual(args[1:3], ["--contract", "ppc-eabi"])
        manual = _equivalence_args_with_default_contract(
            ["check-hex", "--observe", "r3", "--original", "38630004", "--candidate", "38630004"]
        )
        self.assertNotIn("--contract", manual)
        strict = _equivalence_args_with_default_contract(
            ["check-hex", "--contract=strict", "--original", "38630004", "--candidate", "38630004"]
        )
        self.assertEqual(strict.count("--contract=strict"), 1)


if __name__ == "__main__":
    unittest.main()

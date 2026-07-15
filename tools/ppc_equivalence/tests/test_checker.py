from __future__ import annotations

import importlib.util
import json
import random
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
from tools.ppc_equivalence.ir import (
    ExecutionInconclusive,
    Instruction,
    Opcode,
    SUPPORTED_OPCODES,
    UnsupportedInstruction,
)
from tools.ppc_equivalence.model import MachineState, concrete_state
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import (
    ConcreteOps,
    automatic_live_out,
    execute_block,
    execute_cfg,
    execute_instruction,
    rotate_mask,
)


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

    def test_branch_is_normalized(self) -> None:
        instruction = decode("48000008")[0]
        self.assertEqual(instruction.opcode, Opcode.B)
        self.assertEqual(instruction.operands[0], 8)

    def test_reserved_compare_form_is_rejected(self) -> None:
        # cmpw r3,r4 with reserved Rc bit set.
        with self.assertRaises(UnsupportedInstruction):
            decode("7c032001")

    def test_paired_single_and_unmodeled_fp_fail_closed(self) -> None:
        words = (
            0x1000000C,  # psq_lx according to GekkoDisassembler.cpp
            0x1000004C,  # psq_lux
            0x1000000E,  # psq_stx
            0x1000004E,  # psq_stux
            0xE0030000,  # psq_l p0,0(r3),0,qr0
            0xEC000030,  # fres (hardware estimate)
            0xFC000034,  # frsqrte (hardware estimate)
            0xEC00182C,  # fsqrts (not implemented by Broadway interpreter)
            0xFC00182C,  # fsqrt (not implemented by Broadway interpreter)
        )
        for word in words:
            with self.subTest(word=f"0x{word:08x}"):
                try:
                    decoded = decode(f"{word:08x}")
                except UnsupportedInstruction:
                    continue
                with self.assertRaises(UnsupportedInstruction):
                    execute_block(concrete_state(), decoded, ConcreteOps())

    def test_scalar_fp_encodings_match_gekko_disassembler(self) -> None:
        words = "c0030000 d8030000 ec00002a fc00002a fc000050 fc000090 fc000210"
        self.assertEqual(
            [item.opcode for item in decode(words)],
            [Opcode.LFS, Opcode.STFD, Opcode.FADDS, Opcode.FADD,
             Opcode.FNEG, Opcode.FMR, Opcode.FABS],
        )

    def test_reserved_fp_operand_fields_are_rejected(self) -> None:
        # fadd has no FC operand; GekkoDisassembler::fdabc rejects it.
        with self.assertRaises(UnsupportedInstruction):
            decode("fc00006a")

    def test_direct_fp_instruction_execution_fails_closed(self) -> None:
        state = concrete_state()
        unsupported = set(Opcode) - SUPPORTED_OPCODES
        self.assertTrue(unsupported)
        for opcode in unsupported:
            with self.subTest(opcode=opcode.value), self.assertRaises(UnsupportedInstruction):
                execute_instruction(state, instruction(opcode, ()), ConcreteOps())

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


class FloatingPointConcreteTests(unittest.TestCase):
    def test_arithmetic_rounding_compare_and_fprf(self) -> None:
        state = concrete_state({"fpr": {"f1": 1.5, "f2": 2.0, "f3": 4.0}})
        added = execute_block(state, decode("fce1102a"), ConcreteOps())
        self.assertEqual(added.fpr[7], 0x400C000000000000)
        self.assertEqual(added.fpscr & 0x1F000, 0x4000)
        compared = execute_block(state, decode("fc011000"), ConcreteOps())
        self.assertEqual(compared.cr >> 28, 8)
        self.assertEqual(compared.fpscr & 0xF000, 0x8000)

    def test_moves_preserve_nan_payload_and_sign_bits(self) -> None:
        payload = 0x7FF8000012345678
        state = concrete_state({"fpr": {"f2": payload}})
        moved = execute_block(state, decode("fce01090"), ConcreteOps())
        negated = execute_block(state, decode("fce01050"), ConcreteOps())
        self.assertEqual(moved.fpr[7], payload)
        self.assertEqual(negated.fpr[7], payload ^ (1 << 63))

    def test_big_endian_fp_load_store_round_trip(self) -> None:
        state = concrete_state({
            "gpr": {"r3": 0x100},
            "fpr": {"f2": 1.5},
        })
        stored = execute_block(state, decode("d8430000"), ConcreteOps())
        loaded = execute_block(stored, decode("c8630000"), ConcreteOps())
        self.assertEqual(loaded.fpr[3], 0x3FF8000000000000)
        self.assertEqual(
            [loaded.memory.read(0x100 + index) for index in range(8)],
            [0x3F, 0xF8, 0, 0, 0, 0, 0, 0],
        )

    def test_concrete_arithmetic_rejects_unvalidated_rounding_modes(self) -> None:
        state = concrete_state({"fpr": {"f1": 1.5, "f2": 2.0}, "fpscr": 1})
        with self.assertRaises(ExecutionInconclusive):
            execute_block(state, decode("fce1102a"), ConcreteOps())


@unittest.skipUnless(importlib.util.find_spec("z3"), "z3-solver is not installed")
class FloatingPointSymbolicTests(unittest.TestCase):
    def test_fp_automatic_live_out_tracks_fpr_fpscr_and_cr1(self) -> None:
        self.assertEqual(
            automatic_live_out(decode("fce1102b")),
            ("f7", "cr1", "fpscr"),
        )
        self.assertEqual(
            automatic_live_out(decode("fc011000")),
            ("cr0", "fpscr"),
        )

    def test_identical_fp_arithmetic_is_proved(self) -> None:
        code = decode("fce1102a")
        result = check_equivalence(
            code, code, EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=5_000),
            original_hex="fce1102a", candidate_hex="fce1102a",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_add_and_subtract_are_distinguished(self) -> None:
        result = check_equivalence(
            decode("fce1102a"), decode("fce11028"),
            EquivalenceContract(parse_observables(["f7"]), timeout_ms=5_000),
            original_hex="fce1102a", candidate_hex="fce11028",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)


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
        self.assertEqual(parse_observables(["memory"])[0].kind, "memory")
        with self.assertRaises(ValueError):
            EquivalenceContract(values, timeout_ms=0)

    def test_ppc_eabi_preset(self) -> None:
        names = preset_observable_names("ppc-eabi")
        self.assertEqual(names[:5], ("r1", "r2", "r3", "r4", "r13"))
        self.assertIn("r31", names)
        self.assertIn("f1", names)
        self.assertIn("f14", names)
        self.assertIn("f31", names)
        self.assertEqual(names[-4:], ("cr2", "cr3", "cr4", "memory"))
        self.assertNotIn("r5", names)
        self.assertNotIn("cr0", names)

    def test_strict_preset_covers_all_modeled_state(self) -> None:
        names = preset_observable_names("strict")
        self.assertEqual(names[:32], tuple(f"r{index}" for index in range(32)))
        self.assertEqual(names[32:64], tuple(f"f{index}" for index in range(32)))
        self.assertEqual(names[64:], ("cr", "fpscr", "xer.ca", "xer.ov", "xer.so", "lr", "ctr", "memory"))

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


def instruction(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    record: bool = False,
    overflow: bool = False,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, record, overflow, link)


class PhaseOneDecoderTests(unittest.TestCase):
    @staticmethod
    def x(rt: int, ra: int, rb: int, xo: int, rc: int = 0) -> str:
        return f"{((31 << 26) | (rt << 21) | (ra << 16) | (rb << 11) | (xo << 1) | rc):08x}"

    @staticmethod
    def xo(rt: int, ra: int, rb: int, xo: int, oe: int = 0, rc: int = 0) -> str:
        return f"{((31 << 26) | (rt << 21) | (ra << 16) | (rb << 11) | (oe << 10) | (xo << 1) | rc):08x}"

    def test_integer_x_and_xo_families_decode(self) -> None:
        fixtures = [
            (self.xo(3, 4, 5, 10), Opcode.ADDC),
            (self.xo(3, 4, 5, 138, 1, 1), Opcode.ADDE),
            (self.xo(3, 4, 0, 234), Opcode.ADDME),
            (self.xo(3, 4, 5, 8), Opcode.SUBFC),
            (self.xo(3, 4, 5, 136), Opcode.SUBFE),
            (self.xo(3, 4, 0, 104), Opcode.NEG),
            (self.xo(3, 4, 5, 75), Opcode.MULHW),
            (self.xo(3, 4, 5, 11), Opcode.MULHWU),
            (self.xo(3, 4, 5, 235), Opcode.MULLW),
            (self.xo(3, 4, 5, 491), Opcode.DIVW),
            (self.xo(3, 4, 5, 459), Opcode.DIVWU),
            (self.x(4, 3, 5, 60), Opcode.ANDC),
            (self.x(4, 3, 5, 284), Opcode.EQV),
            (self.x(4, 3, 5, 476), Opcode.NAND),
            (self.x(4, 3, 5, 412), Opcode.ORC),
            (self.x(4, 3, 5, 24), Opcode.SLW),
            (self.x(4, 3, 5, 536), Opcode.SRW),
            (self.x(4, 3, 5, 792), Opcode.SRAW),
            (self.x(4, 3, 5, 824), Opcode.SRAWI),
        ]
        for encoded, expected in fixtures:
            with self.subTest(opcode=expected.value):
                self.assertEqual(decode(encoded)[0].opcode, expected)

    def test_integer_memory_families_decode(self) -> None:
        d_form = {
            32: Opcode.LWZ, 33: Opcode.LWZU, 34: Opcode.LBZ, 35: Opcode.LBZU,
            36: Opcode.STW, 37: Opcode.STWU, 38: Opcode.STB, 39: Opcode.STBU,
            40: Opcode.LHZ, 41: Opcode.LHZU, 42: Opcode.LHA, 43: Opcode.LHAU,
            44: Opcode.STH, 45: Opcode.STHU, 46: Opcode.LMW, 47: Opcode.STMW,
        }
        for primary, expected in d_form.items():
            ra = 4
            rt = 28 if expected == Opcode.LMW else 3
            word = (primary << 26) | (rt << 21) | (ra << 16) | 0xFFF0
            with self.subTest(opcode=expected.value):
                self.assertEqual(decode(f"{word:08x}")[0].opcode, expected)
        x_form = {
            23: Opcode.LWZX, 55: Opcode.LWZUX, 87: Opcode.LBZX, 119: Opcode.LBZUX,
            151: Opcode.STWX, 183: Opcode.STWUX, 215: Opcode.STBX, 247: Opcode.STBUX,
            279: Opcode.LHZX, 311: Opcode.LHZUX, 343: Opcode.LHAX, 375: Opcode.LHAUX,
            407: Opcode.STHX, 439: Opcode.STHUX, 534: Opcode.LWBRX, 662: Opcode.STWBRX,
            790: Opcode.LHBRX, 918: Opcode.STHBRX,
        }
        for xo, expected in x_form.items():
            with self.subTest(opcode=expected.value):
                self.assertEqual(decode(self.x(3, 4, 5, xo))[0].opcode, expected)

    def test_update_form_constraints_are_rejected(self) -> None:
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{((33 << 26) | (3 << 21)):08x}")
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{((33 << 26) | (3 << 21) | (3 << 16)):08x}")
        with self.assertRaises(UnsupportedInstruction):
            decode(self.xo(3, 4, 5, 75, oe=1))
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{((46 << 26) | (28 << 21) | (29 << 16)):08x}")

    def test_branch_and_special_register_families_decode(self) -> None:
        self.assertEqual(decode("7c0802a6")[0].operands, (0, 8))  # mflr r0
        self.assertEqual(decode("7c0903a6")[0].operands, (0, 9))  # mtctr r0
        self.assertEqual(decode("4e800020")[0].opcode, Opcode.BCLR)  # blr
        self.assertEqual(decode("4e800420")[0].opcode, Opcode.BCCTR)  # bctr


class PhaseOneConcreteSemanticsTests(unittest.TestCase):
    def test_big_endian_store_and_load_widths(self) -> None:
        state = concrete_state({"gpr": {"r3": 0x100, "r4": 0x89ABCDEF}})
        state = execute_instruction(state, instruction(Opcode.STW, (4, 3, 0)), ConcreteOps())
        self.assertEqual([state.memory.read(0x100 + i) for i in range(4)], [0x89, 0xAB, 0xCD, 0xEF])
        state = execute_instruction(state, instruction(Opcode.LHZ, (5, 3, 2)), ConcreteOps())
        self.assertEqual(state.gpr[5], 0xCDEF)
        state = execute_instruction(state, instruction(Opcode.LHA, (6, 3, 0)), ConcreteOps())
        self.assertEqual(state.gpr[6], 0xFFFF89AB)

    def test_indexed_update_and_byte_reverse(self) -> None:
        state = concrete_state({"gpr": {"r3": 0x100, "r4": 4, "r5": 0x11223344}})
        state = execute_instruction(state, instruction(Opcode.STWBRX, (5, 3, 4)), ConcreteOps())
        self.assertEqual([state.memory.read(0x104 + i) for i in range(4)], [0x44, 0x33, 0x22, 0x11])
        state = execute_instruction(state, instruction(Opcode.LWBRX, (6, 3, 4)), ConcreteOps())
        self.assertEqual(state.gpr[6], 0x11223344)
        state = execute_instruction(state, instruction(Opcode.LBZUX, (7, 3, 4)), ConcreteOps())
        self.assertEqual(state.gpr[3], 0x104)
        self.assertEqual(state.gpr[7], 0x44)

    def test_unaligned_multibyte_access_is_outside_defined_domain(self) -> None:
        state = concrete_state({"gpr": {"r3": 0x101}})
        state = execute_instruction(state, instruction(Opcode.LWZ, (4, 3, 0)), ConcreteOps())
        self.assertFalse(state.valid)

    def test_lmw_and_stmw_round_trip(self) -> None:
        registers = {f"r{i}": i * 0x01010101 for i in range(28, 32)}
        registers["r3"] = 0x200
        state = concrete_state({"gpr": registers})
        state = execute_instruction(state, instruction(Opcode.STMW, (28, 3, 0)), ConcreteOps())
        for index in range(28, 32):
            state = state.with_gpr(index, 0)
        state = execute_instruction(state, instruction(Opcode.LMW, (28, 3, 0)), ConcreteOps())
        self.assertEqual(state.gpr[28:32], tuple(i * 0x01010101 for i in range(28, 32)))

    def test_carry_overflow_and_sticky_so_edges(self) -> None:
        state = concrete_state({"gpr": {"r4": 0xFFFFFFFF, "r5": 1}})
        state = execute_instruction(state, instruction(Opcode.ADDC, (3, 4, 5)), ConcreteOps())
        self.assertEqual(state.gpr[3], 0)
        self.assertTrue(state.xer.ca)
        state = concrete_state({"gpr": {"r4": 0x7FFFFFFF, "r5": 0}, "xer": {"ca": 1}})
        state = execute_instruction(state, instruction(Opcode.ADDE, (3, 4, 5), overflow=True), ConcreteOps())
        self.assertEqual(state.gpr[3], 0x80000000)
        self.assertTrue(state.xer.ov)
        self.assertTrue(state.xer.so)

    def test_addme_and_subfme_special_carry_rules(self) -> None:
        addme = execute_instruction(
            concrete_state({"gpr": {"r4": 0}, "xer": {"ca": 1}}),
            instruction(Opcode.ADDME, (3, 4)),
            ConcreteOps(),
        )
        self.assertEqual(addme.gpr[3], 0)
        self.assertFalse(addme.xer.ca)

        subfme_with_carry = execute_instruction(
            concrete_state({"gpr": {"r4": 0}, "xer": {"ca": 1}}),
            instruction(Opcode.SUBFME, (3, 4)),
            ConcreteOps(),
        )
        self.assertEqual(subfme_with_carry.gpr[3], 0xFFFFFFFF)
        self.assertFalse(subfme_with_carry.xer.ca)

        subfme_without_carry = execute_instruction(
            concrete_state({"gpr": {"r4": 0}, "xer": {"ca": 0}}),
            instruction(Opcode.SUBFME, (3, 4)),
            ConcreteOps(),
        )
        self.assertEqual(subfme_without_carry.gpr[3], 0xFFFFFFFE)
        self.assertTrue(subfme_without_carry.xer.ca)

    def test_randomized_addme_and_subfme_reference(self) -> None:
        rng = random.Random(0xADD5F)
        for carry_in in (0, 1):
            for _ in range(256):
                value = rng.getrandbits(32)
                state = concrete_state({"gpr": {"r4": value}, "xer": {"ca": carry_in}})

                added = execute_instruction(state, instruction(Opcode.ADDME, (3, 4)), ConcreteOps())
                self.assertEqual(added.gpr[3], (value - 1 + carry_in) & 0xFFFFFFFF)
                self.assertEqual(added.xer.ca, carry_in == 0 and value != 0)

                subtracted = execute_instruction(state, instruction(Opcode.SUBFME, (3, 4)), ConcreteOps())
                self.assertEqual(subtracted.gpr[3], ((~value) - 1 + carry_in) & 0xFFFFFFFF)
                self.assertEqual(subtracted.xer.ca, carry_in == 0 and value != 0xFFFFFFFF)

    def test_shift_edge_rules_and_sraw_carry(self) -> None:
        state = concrete_state({"gpr": {"r3": 0xFFFFFFFF, "r4": 32}})
        shifted = execute_instruction(state, instruction(Opcode.SLW, (5, 3, 4)), ConcreteOps())
        self.assertEqual(shifted.gpr[5], 0)
        arithmetic = execute_instruction(state, instruction(Opcode.SRAWI, (5, 3, 1)), ConcreteOps())
        self.assertEqual(arithmetic.gpr[5], 0xFFFFFFFF)
        self.assertTrue(arithmetic.xer.ca)

    def test_division_defined_domain_is_tracked(self) -> None:
        valid = execute_instruction(
            concrete_state({"gpr": {"r4": 0xFFFFFFF9, "r5": 3}}),
            instruction(Opcode.DIVW, (3, 4, 5)), ConcreteOps(),
        )
        self.assertEqual(valid.gpr[3], 0xFFFFFFFE)
        self.assertTrue(valid.valid)
        invalid = execute_instruction(
            concrete_state({"gpr": {"r4": 7, "r5": 0}}),
            instruction(Opcode.DIVWU, (3, 4, 5)), ConcreteOps(),
        )
        self.assertFalse(invalid.valid)

    def test_randomized_carry_overflow_and_high_multiply_reference(self) -> None:
        rng = random.Random(0x750C1)
        signed = lambda value: value - 0x100000000 if value & 0x80000000 else value
        for _ in range(500):
            left, right, carry = rng.getrandbits(32), rng.getrandbits(32), rng.getrandbits(1)
            state = concrete_state({"gpr": {"r4": left, "r5": right}, "xer": {"ca": carry}})

            added = execute_instruction(state, instruction(Opcode.ADDE, (3, 4, 5), overflow=True), ConcreteOps())
            total = left + right + carry
            signed_total = signed(left) + signed(right) + carry
            self.assertEqual(added.gpr[3], total & 0xFFFFFFFF)
            self.assertEqual(added.xer.ca, total > 0xFFFFFFFF)
            self.assertEqual(added.xer.ov, not (-0x80000000 <= signed_total <= 0x7FFFFFFF))

            subtracted = execute_instruction(state, instruction(Opcode.SUBFE, (3, 4, 5), overflow=True), ConcreteOps())
            raw_sub = right + ((~left) & 0xFFFFFFFF) + carry
            signed_sub = signed(right) - signed(left) - (1 - carry)
            self.assertEqual(subtracted.gpr[3], raw_sub & 0xFFFFFFFF)
            self.assertEqual(subtracted.xer.ca, raw_sub > 0xFFFFFFFF)
            self.assertEqual(subtracted.xer.ov, not (-0x80000000 <= signed_sub <= 0x7FFFFFFF))

            mul_signed = execute_instruction(state, instruction(Opcode.MULHW, (3, 4, 5)), ConcreteOps())
            mul_unsigned = execute_instruction(state, instruction(Opcode.MULHWU, (3, 4, 5)), ConcreteOps())
            self.assertEqual(mul_signed.gpr[3], ((signed(left) * signed(right)) >> 32) & 0xFFFFFFFF)
            self.assertEqual(mul_unsigned.gpr[3], ((left * right) >> 32) & 0xFFFFFFFF)

    def test_randomized_shift_reference(self) -> None:
        rng = random.Random(0xB04D)
        for amount in range(64):
            for _ in range(8):
                value = rng.getrandbits(32)
                state = concrete_state({"gpr": {"r3": value, "r4": amount}})
                slw = execute_instruction(state, instruction(Opcode.SLW, (5, 3, 4)), ConcreteOps())
                srw = execute_instruction(state, instruction(Opcode.SRW, (5, 3, 4)), ConcreteOps())
                expected_left = 0 if amount & 32 else (value << (amount & 31)) & 0xFFFFFFFF
                expected_right = 0 if amount & 32 else value >> (amount & 31)
                self.assertEqual(slw.gpr[5], expected_left)
                self.assertEqual(srw.gpr[5], expected_right)


class PhaseTwoControlFlowTests(unittest.TestCase):
    def test_conditional_branch_uses_cr_and_has_two_symbolic_exits(self) -> None:
        program = [instruction(Opcode.BC, (12, 2, 0x20, 0), address=0)]
        taken_state = concrete_state({"cr": 1 << (31 - 2)})
        exits = [item for item in execute_cfg(taken_state, program, ConcreteOps()) if item.condition]
        self.assertEqual((exits[0].exit_kind, exits[0].exit_target), ("direct-branch", 0x20))
        not_taken = [item for item in execute_cfg(concrete_state(), program, ConcreteOps()) if item.condition]
        self.assertEqual(not_taken[0].exit_kind, "fallthrough")

    def test_bdnz_decrements_ctr_before_testing(self) -> None:
        program = [instruction(Opcode.BC, (16, 0, 0x20, 0), address=0)]
        one = [item for item in execute_cfg(concrete_state({"ctr": 1}), program, ConcreteOps()) if item.condition][0]
        two = [item for item in execute_cfg(concrete_state({"ctr": 2}), program, ConcreteOps()) if item.condition][0]
        self.assertEqual(one.exit_kind, "fallthrough")
        self.assertEqual(two.exit_kind, "direct-branch")
        self.assertEqual(one.state.ctr, 0)
        self.assertEqual(two.state.ctr, 1)

    def test_link_and_return_use_old_lr_and_aligned_target(self) -> None:
        branch = [instruction(Opcode.B, (0x40, 0), address=0x10, link=True)]
        exit_state = execute_cfg(concrete_state(), branch, ConcreteOps())[0]
        self.assertEqual(exit_state.exit_kind, "call")
        self.assertEqual(exit_state.state.lr, 0x14)
        ret = [instruction(Opcode.BCLR, (20, 0, 0), address=0)]
        exit_state = [item for item in execute_cfg(concrete_state({"lr": 0x123}), ret, ConcreteOps()) if item.condition][0]
        self.assertEqual((exit_state.exit_kind, exit_state.exit_target), ("return", 0x120))

    def test_loop_and_path_limits_are_inconclusive(self) -> None:
        with self.assertRaises(ExecutionInconclusive):
            execute_cfg(concrete_state(), [instruction(Opcode.B, (0, 0), address=0)], ConcreteOps())
        branches = [instruction(Opcode.BC, (12, 2, 4, 0), address=0)]
        with self.assertRaises(ExecutionInconclusive):
            execute_cfg(concrete_state(), branches, ConcreteOps(), max_paths=1)

    def test_automatic_live_out_includes_memory_and_updated_base(self) -> None:
        program = [instruction(Opcode.STWU, (4, 3, -16))]
        live = automatic_live_out(program)
        self.assertIn("r3", live)
        self.assertIn("memory", live)


@unittest.skipUnless(importlib.util.find_spec("z3"), "z3-solver is not installed")
class PhaseOneTwoSymbolicTests(unittest.TestCase):
    def test_abi_contract_detects_memory_only_difference(self) -> None:
        result = check_equivalence(
            decode("98630000"), decode("60000000"),
            make_contract(preset="ppc-eabi", observe=None, timeout_ms=5_000),
            original_hex="98630000", candidate_hex="60000000",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual(result.mismatch["name"], "memory")
        assert result.replay is not None
        initial = concrete_state(result.replay["initial_state"])
        left = execute_block(initial, decode(result.replay["original_hex"]), ConcreteOps())
        right = execute_block(initial, decode(result.replay["candidate_hex"]), ConcreteOps())
        self.assertNotEqual(left.memory, right.memory)

    def test_store_then_load_equals_register_move_and_same_store(self) -> None:
        result = check_equivalence(
            decode("90830000 80a30000"), decode("90830000 7c852378"),
            EquivalenceContract(parse_observables(["r5,memory"]), timeout_ms=5_000),
            original_hex="90830000 80a30000", candidate_hex="90830000 7c852378",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_equivalent_conditional_cfgs_are_proved(self) -> None:
        original = decode_block(parse_hex("2c030000 41820008 48000018 48000018"), 0x100, validate_with_capstone=False)
        candidate = decode_block(parse_hex("2c030000 40820008 4800001c 48000014"), 0x100, validate_with_capstone=False)
        result = check_equivalence(
            original, candidate, EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000),
            original_hex="2c030000 41820008 48000018 48000018",
            candidate_hex="2c030000 40820008 4800001c 48000014",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)


if __name__ == "__main__":
    unittest.main()

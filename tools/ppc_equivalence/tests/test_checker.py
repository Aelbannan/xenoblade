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
    FPSCR_FEX,
    FPSCR_FI,
    FPSCR_FR,
    FPSCR_FX,
    FPSCR_VE,
    FPSCR_VX,
    FPSCR_VXIDI,
    FPSCR_VXIMZ,
    FPSCR_VXISI,
    FPSCR_VXSNAN,
    FPSCR_VXSQRT,
    FPSCR_VXVC,
    FPSCR_VXZDZ,
    FPSCR_VXCVI,
    FPSCR_XE,
    FPSCR_XX,
    FPSCR_ZE,
    FPSCR_ZX,
    SymbolicOps,
    _fpscr_raise,
    automatic_live_out,
    execute_block,
    execute_cfg,
    execute_instruction,
    register_effects,
    rotate_mask,
)


def decode(hex_words: str):
    return decode_block(parse_hex(hex_words), validate_with_capstone=False)


def ps_x(fd: int, fa: int, fb: int, xo: int, *, rc: int = 0) -> int:
    return (
        (4 << 26) | ((fd & 31) << 21) | ((fa & 31) << 16)
        | ((fb & 31) << 11) | ((xo & 0x3FF) << 1) | (rc & 1)
    )


def ps_a(
    fd: int, fa: int, fb: int, fc: int, xo5: int, *, rc: int = 0,
) -> int:
    return (
        (4 << 26) | ((fd & 31) << 21) | ((fa & 31) << 16)
        | ((fb & 31) << 11) | ((fc & 31) << 6)
        | ((xo5 & 31) << 1) | (rc & 1)
    )


class DecoderTests(unittest.TestCase):
    def test_broadway_cache_privileged_and_exception_encodings(self) -> None:
        words = (
            "7c0000ac 7c0003ac 7c00006c 7c00222c 7c001fec 10061fec "
            "7c0017ac 7c0004ac 4c00012c 7c6000a6 7c600124 "
            "7c6004a6 7c6001a4 7c6d42e6 0fe00000 44000002 4c000064"
        )
        self.assertEqual(
            [(item.opcode, item.operands) for item in decode(words)],
            [
                (Opcode.DCBF, (0, 0)), (Opcode.DCBI, (0, 0)),
                (Opcode.DCBST, (0, 0)), (Opcode.DCBT, (0, 4)),
                (Opcode.DCBZ, (0, 3)), (Opcode.DCBZ_L, (6, 3)),
                (Opcode.ICBI, (0, 2)), (Opcode.SYNC, ()),
                (Opcode.ISYNC, ()), (Opcode.MFMSR, (3,)),
                (Opcode.MTMSR, (3,)), (Opcode.MFSR, (3, 0)),
                (Opcode.MTSR, (3, 0)), (Opcode.MFTB, (3, 269)),
                (Opcode.TWI, (31, 0, 0)), (Opcode.SC, ()),
                (Opcode.RFI, ()),
            ],
        )
        with self.assertRaises(UnsupportedInstruction):
            decode("7c2004ac")
        with self.assertRaises(UnsupportedInstruction):
            decode("4c200064")

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

    def test_non_broadway_sqrt_encodings_are_rejected(self) -> None:
        words = (
            0xEC00182C,  # fsqrts encoding from later PowerPC ISA revisions
            0xFC00182C,  # fsqrt encoding from later PowerPC ISA revisions
        )
        for word in words:
            with self.subTest(word=f"0x{word:08x}"):
                with self.assertRaises(UnsupportedInstruction):
                    decode(f"{word:08x}")

    def test_scalar_fp_encodings_match_gekko_disassembler(self) -> None:
        words = (
            "c0030000 d8030000 ec00002a ece110f8 ece110fa ece110fc ece110fe "
            "fce110f8 fce110fa fce110fc fce110fe fc00002a fc000050 fc000090 fc000210 "
            "ece01030 fce01034"
        )
        self.assertEqual(
            [item.opcode for item in decode(words)],
            [Opcode.LFS, Opcode.STFD, Opcode.FADDS, Opcode.FMSUBS, Opcode.FMADDS,
             Opcode.FNMSUBS, Opcode.FNMADDS,
             Opcode.FMSUB, Opcode.FMADD, Opcode.FNMSUB, Opcode.FNMADD, Opcode.FADD,
             Opcode.FNEG, Opcode.FMR, Opcode.FABS, Opcode.FRES, Opcode.FRSQRTE],
        )

    def test_paired_move_and_merge_encodings_match_gekko_disassembler(self) -> None:
        words = (
            ps_x(7, 0, 2, 40), ps_x(7, 0, 2, 72),
            ps_x(7, 0, 2, 136), ps_x(7, 0, 2, 264),
            ps_x(7, 1, 2, 528), ps_x(7, 1, 2, 560),
            ps_x(7, 1, 2, 592), ps_x(7, 1, 2, 624),
            ps_x(3 << 2, 1, 2, 0), ps_x(3 << 2, 1, 2, 32),
            ps_x(3 << 2, 1, 2, 64), ps_x(3 << 2, 1, 2, 96),
        )
        decoded = decode(" ".join(f"{word:08x}" for word in words))
        self.assertEqual(
            [(item.opcode, item.operands) for item in decoded],
            [
                (Opcode.PS_NEG, (7, 2)), (Opcode.PS_MR, (7, 2)),
                (Opcode.PS_NABS, (7, 2)), (Opcode.PS_ABS, (7, 2)),
                (Opcode.PS_MERGE00, (7, 1, 2)), (Opcode.PS_MERGE01, (7, 1, 2)),
                (Opcode.PS_MERGE10, (7, 1, 2)), (Opcode.PS_MERGE11, (7, 1, 2)),
                (Opcode.PS_CMPU0, (3, 1, 2)), (Opcode.PS_CMPO0, (3, 1, 2)),
                (Opcode.PS_CMPU1, (3, 1, 2)), (Opcode.PS_CMPO1, (3, 1, 2)),
            ],
        )
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{ps_x(7, 1, 2, 72):08x}")
        self.assertTrue(decode(f"{ps_x(7, 0, 2, 72, rc=1):08x}")[0].record)
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{ps_x(7, 1, 2, 0, rc=1):08x}")
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{ps_x(3, 1, 2, 0):08x}")

    def test_basic_paired_arithmetic_encodings_and_reserved_fields(self) -> None:
        words = (
            ps_a(7, 1, 2, 0, 18),
            ps_a(7, 0, 2, 0, 24), ps_a(7, 0, 2, 0, 26, rc=1),
            ps_a(7, 1, 2, 0, 21), ps_a(7, 1, 2, 0, 20),
            ps_a(7, 1, 0, 3, 25), ps_a(7, 1, 0, 3, 12),
            ps_a(7, 1, 0, 3, 13, rc=1),
            ps_a(7, 1, 2, 3, 29), ps_a(7, 1, 2, 3, 28),
            ps_a(7, 1, 2, 3, 31), ps_a(7, 1, 2, 3, 30),
            ps_a(7, 1, 2, 3, 14), ps_a(7, 1, 2, 3, 15, rc=1),
            ps_a(7, 1, 2, 3, 10), ps_a(7, 1, 2, 3, 11, rc=1),
            ps_a(7, 1, 2, 3, 23),
        )
        decoded = decode(" ".join(f"{word:08x}" for word in words))
        self.assertEqual(
            [(item.opcode, item.operands, item.record) for item in decoded],
            [
                (Opcode.PS_DIV, (7, 1, 2), False),
                (Opcode.PS_RES, (7, 2), False),
                (Opcode.PS_RSQRTE, (7, 2), True),
                (Opcode.PS_ADD, (7, 1, 2), False),
                (Opcode.PS_SUB, (7, 1, 2), False),
                (Opcode.PS_MUL, (7, 1, 3), False),
                (Opcode.PS_MULS0, (7, 1, 3), False),
                (Opcode.PS_MULS1, (7, 1, 3), True),
                (Opcode.PS_MADD, (7, 1, 2, 3), False),
                (Opcode.PS_MSUB, (7, 1, 2, 3), False),
                (Opcode.PS_NMADD, (7, 1, 2, 3), False),
                (Opcode.PS_NMSUB, (7, 1, 2, 3), False),
                (Opcode.PS_MADDS0, (7, 1, 2, 3), False),
                (Opcode.PS_MADDS1, (7, 1, 2, 3), True),
                (Opcode.PS_SUM0, (7, 1, 2, 3), False),
                (Opcode.PS_SUM1, (7, 1, 2, 3), True),
                (Opcode.PS_SEL, (7, 1, 2, 3), False),
            ],
        )
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{ps_a(7, 1, 2, 3, 21):08x}")
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{ps_a(7, 1, 2, 3, 25):08x}")
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{ps_a(7, 1, 2, 0, 24):08x}")
        with self.assertRaises(UnsupportedInstruction):
            decode(f"{ps_a(7, 0, 2, 3, 26):08x}")

    def test_reserved_fp_operand_fields_are_rejected(self) -> None:
        # fadd has no FC operand; GekkoDisassembler::fdabc rejects it.
        with self.assertRaises(UnsupportedInstruction):
            decode("fc00006a")
        # fcmpo encodes BF in bits 23..25 and requires bits 21..22 and Rc zero.
        compare = decode("fd811040")[0]
        self.assertEqual((compare.opcode, compare.operands), (Opcode.FCMPO, (3, 1, 2)))
        with self.assertRaises(UnsupportedInstruction):
            decode("fc611040")
        conversion = decode("fce0101c")[0]
        self.assertEqual((conversion.opcode, conversion.operands), (Opcode.FCTIW, (7, 0, 2, 0)))
        with self.assertRaises(UnsupportedInstruction):
            decode("fce1101c")

    def test_every_decoded_opcode_has_semantics(self) -> None:
        self.assertEqual(set(Opcode) - SUPPORTED_OPCODES, set())

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

    def test_paired_moves_and_merges_preserve_both_lane_bits(self) -> None:
        ps0_a = 0x7FF8000012345678
        ps1_a = 0x8000000000000000
        ps0_b = 0xFFF00000ABCDEF01
        ps1_b = 0x3FF8000000000000
        state = concrete_state({
            "fpr": {"f1": ps0_a, "f2": ps0_b},
            "ps1": {"f1": ps1_a, "f2": ps1_b},
            "fpscr": 0xA0000000,
        })

        move_expected = {
            Opcode.PS_MR: (ps0_a, ps1_a),
            Opcode.PS_NEG: (ps0_a ^ (1 << 63), ps1_a ^ (1 << 63)),
            Opcode.PS_NABS: (ps0_a | (1 << 63), ps1_a | (1 << 63)),
            Opcode.PS_ABS: (ps0_a & ~(1 << 63), ps1_a & ~(1 << 63)),
        }
        for opcode, expected in move_expected.items():
            with self.subTest(opcode=opcode.value):
                final = execute_instruction(
                    state, Instruction(0, 0, opcode, (7, 1), record=True), ConcreteOps(),
                )
                self.assertEqual((final.fpr[7], final.ps1[7]), expected)
                self.assertEqual(final.cr & 0x0F000000, 0x0A000000)
                self.assertEqual(final.fpscr, state.fpscr)

        merge_expected = {
            Opcode.PS_MERGE00: (ps0_a, ps0_b),
            Opcode.PS_MERGE01: (ps0_a, ps1_b),
            Opcode.PS_MERGE10: (ps1_a, ps0_b),
            Opcode.PS_MERGE11: (ps1_a, ps1_b),
        }
        for opcode, expected in merge_expected.items():
            with self.subTest(opcode=opcode.value):
                final = execute_instruction(
                    state, Instruction(0, 0, opcode, (1, 1, 2)), ConcreteOps(),
                )
                self.assertEqual((final.fpr[1], final.ps1[1]), expected)

    def test_paired_compares_select_lane_and_share_scalar_exception_rules(self) -> None:
        finite = concrete_state({
            "fpr": {"f1": 1.0, "f2": 2.0},
            "ps1": {"f1": 3.0, "f2": 2.0},
        })
        low = execute_instruction(
            finite, Instruction(0, 0, Opcode.PS_CMPU0, (3, 1, 2)), ConcreteOps(),
        )
        high = execute_instruction(
            finite, Instruction(0, 0, Opcode.PS_CMPU1, (3, 1, 2)), ConcreteOps(),
        )
        self.assertEqual((low.cr, low.fpscr), (0x00080000, 0x00008000))
        self.assertEqual((high.cr, high.fpscr), (0x00040000, 0x00004000))

        ordered_qnan = execute_instruction(concrete_state({
            "ps1": {"f1": "0x7ff8000012345678", "f2": 2.0},
        }), Instruction(0, 0, Opcode.PS_CMPO1, (3, 1, 2)), ConcreteOps())
        self.assertEqual(ordered_qnan.cr, 0x00010000)
        self.assertEqual(
            ordered_qnan.fpscr,
            FPSCR_FX | FPSCR_VX | FPSCR_VXVC | 0x1000,
        )

        unordered_snan = execute_instruction(concrete_state({
            "ps1": {"f1": "0x7ff0000012345678", "f2": 2.0},
        }), Instruction(0, 0, Opcode.PS_CMPU1, (3, 1, 2)), ConcreteOps())
        self.assertEqual(unordered_snan.cr, 0x00010000)
        self.assertEqual(
            unordered_snan.fpscr,
            FPSCR_FX | FPSCR_VX | FPSCR_VXSNAN | 0x1000,
        )

    def test_basic_paired_arithmetic_results_broadcast_and_force25(self) -> None:
        state = concrete_state({
            "fpr": {"f1": 1.5, "f2": 2.0, "f3": 4.0},
            "ps1": {"f1": -2.0, "f2": 4.0, "f3": -0.5},
            "fpscr": FPSCR_FI | FPSCR_FR,
        })
        expected = {
            Opcode.PS_ADD: (0x400C000000000000, 0x4000000000000000, 2),
            Opcode.PS_SUB: (0xBFE0000000000000, 0xC018000000000000, 2),
            Opcode.PS_MUL: (0x4018000000000000, 0x3FF0000000000000, 3),
            Opcode.PS_MULS0: (0x4018000000000000, 0xC020000000000000, 3),
            Opcode.PS_MULS1: (0xBFE8000000000000, 0x3FF0000000000000, 3),
        }
        for opcode, (ps0, ps1, source) in expected.items():
            with self.subTest(opcode=opcode.value):
                final = execute_instruction(
                    state, Instruction(0, 0, opcode, (7, 1, source)), ConcreteOps(),
                )
                self.assertEqual((final.fpr[7], final.ps1[7]), (ps0, ps1))
                self.assertEqual(final.fpscr & 0x1F000, 0x4000 if ps0 >> 63 == 0 else 0x8000)
                self.assertEqual(final.fpscr & (FPSCR_FI | FPSCR_FR), FPSCR_FI | FPSCR_FR)

        force25 = execute_instruction(concrete_state({
            "fpr": {"f1": 1.0, "f3": "0x3ff0000010000001"},
            "ps1": {"f1": 1.0, "f3": "0x3ff0000010000001"},
        }), Instruction(0, 0, Opcode.PS_MUL, (7, 1, 3)), ConcreteOps())
        self.assertEqual((force25.fpr[7], force25.ps1[7]),
                         (0x3FF0000000000000, 0x3FF0000000000000))

    def test_basic_paired_arithmetic_aggregates_lane_exceptions(self) -> None:
        final = execute_instruction(concrete_state({
            "fpr": {"f1": "0x7ff0000000000000", "f2": "0xfff0000000000000"},
            "ps1": {"f1": "0x7ff0000012345678", "f2": 2.0},
        }), Instruction(0, 0, Opcode.PS_ADD, (7, 1, 2), record=True), ConcreteOps())
        self.assertEqual(final.fpr[7], 0x7FF8000000000000)
        self.assertEqual(final.ps1[7], 0x7FF8000000000000)
        self.assertEqual(final.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXISI | FPSCR_VXSNAN | 0x11000)
        self.assertEqual(final.cr & 0x0F000000, 0x0A000000)

        multiplied = execute_instruction(concrete_state({
            "fpr": {"f1": 1.0, "f3": 2.0},
            "ps1": {"f1": 0.0, "f3": "0x7ff0000000000000"},
        }), Instruction(0, 0, Opcode.PS_MUL, (7, 1, 3)), ConcreteOps())
        self.assertEqual(multiplied.ps1[7], 0x7FF8000000000000)
        self.assertEqual(multiplied.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXIMZ | 0x4000)

    def test_paired_division_rounds_both_lanes_and_aggregates_exceptions(self) -> None:
        final = execute_instruction(concrete_state({
            "fpr": {"f1": 6.0, "f2": 2.0},
            "ps1": {"f1": 0.0, "f2": 0.0},
        }), Instruction(0, 0, Opcode.PS_DIV, (7, 1, 2), record=True), ConcreteOps())
        self.assertEqual(final.fpr[7], 0x4008000000000000)
        self.assertEqual(final.ps1[7], 0x7FF8000000000000)
        self.assertEqual(final.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXZDZ | 0x4000)
        self.assertEqual(final.cr & 0x0F000000, 0x0A000000)

        # Unlike scalar division, paired division writes both lanes even when
        # an enabled exception was raised by either lane.
        enabled = execute_instruction(concrete_state({
            "fpr": {"f1": 1.0, "f2": 0.0, "f7": 3.0},
            "ps1": {"f1": "0x7ff0000012345678", "f2": 2.0, "f7": 4.0},
            "fpscr": FPSCR_VE | FPSCR_ZE,
        }), Instruction(0, 0, Opcode.PS_DIV, (7, 1, 2)), ConcreteOps())
        self.assertEqual(enabled.fpr[7], 0x7FF0000000000000)
        self.assertEqual(enabled.ps1[7], 0x7FF8000000000000)
        self.assertEqual(
            enabled.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXSNAN |
            FPSCR_ZX | FPSCR_VE | FPSCR_ZE | 0x5000,
        )

    def test_paired_estimates_use_tables_and_broadway_exception_writeback(self) -> None:
        state = concrete_state({
            "fpr": {"f2": 1.0}, "ps1": {"f2": 2.0},
        })
        reciprocal = execute_instruction(
            state, Instruction(0, 0, Opcode.PS_RES, (7, 2), record=True), ConcreteOps(),
        )
        self.assertEqual(
            (reciprocal.fpr[7], reciprocal.ps1[7]),
            (0x3FEFFF0000000000, 0x3FDFFF0000000000),
        )
        self.assertEqual(reciprocal.fpscr, 0x4000)

        rsqrt = execute_instruction(
            state, Instruction(0, 0, Opcode.PS_RSQRTE, (7, 2)), ConcreteOps(),
        )
        self.assertEqual(
            (rsqrt.fpr[7], rsqrt.ps1[7]),
            (0x3FEFFE8000000000, 0x3FE69FA000000000),
        )
        self.assertEqual(rsqrt.fpscr, 0x4000)

        # VE/ZE do not suppress paired-estimate destinations on Broadway.
        exceptional = execute_instruction(concrete_state({
            "fpr": {"f2": 0.0, "f7": 3.0},
            "ps1": {"f2": "0x7ff0000012345678", "f7": 4.0},
            "fpscr": FPSCR_VE | FPSCR_ZE | FPSCR_FI | FPSCR_FR,
        }), Instruction(0, 0, Opcode.PS_RES, (7, 2)), ConcreteOps())
        self.assertEqual(exceptional.fpr[7], 0x7FF0000000000000)
        self.assertEqual(exceptional.ps1[7], 0x7FF8000012345678)
        self.assertEqual(
            exceptional.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXSNAN |
            FPSCR_ZX | FPSCR_VE | FPSCR_ZE | 0x5000,
        )

        negative_zero = execute_instruction(concrete_state({
            "fpr": {"f2": -1.0}, "ps1": {"f2": 0.0},
            "fpscr": FPSCR_VE | FPSCR_ZE | FPSCR_FI | FPSCR_FR,
        }), Instruction(0, 0, Opcode.PS_RSQRTE, (7, 2), record=True), ConcreteOps())
        self.assertEqual(
            (negative_zero.fpr[7], negative_zero.ps1[7]),
            (0x7FF8000000000000, 0x7FF0000000000000),
        )
        self.assertEqual(
            negative_zero.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXSQRT |
            FPSCR_ZX | FPSCR_VE | FPSCR_ZE | 0x11000,
        )
        self.assertEqual(negative_zero.cr & 0x0F000000, 0x0E000000)

    def test_paired_fused_results_negation_broadcast_and_record(self) -> None:
        state = concrete_state({
            "fpr": {"f1": 1.5, "f2": 2.0, "f3": 4.0},
            "ps1": {"f1": -2.0, "f2": 4.0, "f3": -0.5},
        })
        expected = {
            Opcode.PS_MADD: (8.0, 5.0),
            Opcode.PS_MSUB: (4.0, -3.0),
            Opcode.PS_NMADD: (-8.0, -5.0),
            Opcode.PS_NMSUB: (-4.0, 3.0),
            Opcode.PS_MADDS0: (8.0, -4.0),
            Opcode.PS_MADDS1: (1.25, 5.0),
        }
        expected_bits = {
            value: concrete_state({"fpr": {"f1": value}}).fpr[1]
            for pair in expected.values() for value in pair
        }
        for opcode, pair in expected.items():
            with self.subTest(opcode=opcode.value):
                final = execute_instruction(
                    state, Instruction(0, 0, opcode, (7, 1, 2, 3), record=True),
                    ConcreteOps(),
                )
                self.assertEqual(
                    (final.fpr[7], final.ps1[7]),
                    (expected_bits[pair[0]], expected_bits[pair[1]]),
                )
                self.assertEqual(final.cr & 0x0F000000, 0)
                self.assertEqual(
                    final.fpscr & 0x1F000,
                    0x8000 if pair[0] < 0 else 0x4000,
                )

    def test_paired_fused_nan_priority_lane_exceptions_and_ve_writeback(self) -> None:
        snan_a = 0xFFF0000012345678
        qnan_b = 0x7FF80000ABCDEF01
        final = execute_instruction(concrete_state({
            "fpr": {"f1": snan_a, "f2": qnan_b, "f3": 2.0, "f7": 1.0},
            "ps1": {
                "f1": 0.0, "f2": "0xfff0000000000000",
                "f3": "0x7ff0000000000000", "f7": 1.0,
            },
            "fpscr": FPSCR_VE,
        }), Instruction(0, 0, Opcode.PS_NMADD, (7, 1, 2, 3)), ConcreteOps())
        # NaN results are quieted in A/B/C priority and are not sign-negated.
        self.assertEqual(final.fpr[7], 0xFFF8000000000000)
        self.assertEqual(final.ps1[7], 0x7FF8000000000000)
        self.assertEqual(
            final.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXSNAN |
            FPSCR_VXIMZ | FPSCR_VE | 0x11000,
        )

        lane_vxisi = execute_instruction(concrete_state({
            "fpr": {"f1": 1.0, "f2": 2.0, "f3": 3.0},
            "ps1": {
                "f1": "0x7ff0000000000000",
                "f2": "0xfff0000000000000",
                "f3": 1.0,
            },
        }), Instruction(0, 0, Opcode.PS_MADD, (7, 1, 2, 3)), ConcreteOps())
        self.assertEqual(lane_vxisi.fpr[7], 0x4014000000000000)
        self.assertEqual(lane_vxisi.ps1[7], 0x7FF8000000000000)
        self.assertEqual(
            lane_vxisi.fpscr,
            FPSCR_FX | FPSCR_VX | FPSCR_VXISI | 0x4000,
        )

    def test_paired_sums_use_cross_lanes_copy_c_and_select_fprf_lane(self) -> None:
        state = concrete_state({
            "fpr": {"f1": 1.5, "f2": 10.0, "f3": 3.0},
            "ps1": {"f1": 99.0, "f2": -2.0, "f3": 4.0},
        })
        sum0 = execute_instruction(
            state, Instruction(0, 0, Opcode.PS_SUM0, (7, 1, 2, 3), record=True),
            ConcreteOps(),
        )
        self.assertEqual(
            (sum0.fpr[7], sum0.ps1[7]),
            (0xBFE0000000000000, 0x4010000000000000),
        )
        self.assertEqual(sum0.fpscr & 0x1F000, 0x8000)

        sum1 = execute_instruction(
            state, Instruction(0, 0, Opcode.PS_SUM1, (7, 1, 2, 3), record=True),
            ConcreteOps(),
        )
        self.assertEqual(
            (sum1.fpr[7], sum1.ps1[7]),
            (0x4008000000000000, 0xBFE0000000000000),
        )
        # ps_sum1 classifies its arithmetic result from PS1, not copied PS0.
        self.assertEqual(sum1.fpscr & 0x1F000, 0x8000)

    def test_paired_sum_only_arithmetic_lane_raises_invalid(self) -> None:
        copied_snan = 0x7FF0000012345678
        copied = execute_instruction(concrete_state({
            "fpr": {"f1": 1.0, "f3": copied_snan},
            "ps1": {"f2": 2.0},
        }), Instruction(0, 0, Opcode.PS_SUM1, (7, 1, 2, 3)), ConcreteOps())
        self.assertEqual(copied.fpr[7], 0x7FF8000000000000)
        self.assertEqual(copied.ps1[7], 0x4008000000000000)
        self.assertEqual(copied.fpscr, 0x4000)

        invalid = execute_instruction(concrete_state({
            "fpr": {"f1": "0x7ff0000000000000", "f3": 1.0},
            "ps1": {"f2": "0xfff0000000000000", "f3": 2.0},
        }), Instruction(0, 0, Opcode.PS_SUM0, (7, 1, 2, 3)), ConcreteOps())
        self.assertEqual(invalid.fpr[7], 0x7FF8000000000000)
        self.assertEqual(invalid.ps1[7], 0x4000000000000000)
        self.assertEqual(invalid.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXISI | 0x11000)

    def test_ps_sel_preserves_bits_and_handles_signed_zero_and_nan_predicates(self) -> None:
        b0, b1 = 0xFFF8000012345678, 0x8000000000000000
        c0, c1 = 0x7FF80000ABCDEF01, 0x3FF8000000000000
        state = concrete_state({
            "fpr": {"f1": "0x8000000000000000", "f2": b0, "f3": c0},
            "ps1": {"f1": "0x7ff8000000000001", "f2": b1, "f3": c1},
            "fpscr": 0xA0000000,
        })
        final = execute_instruction(
            state, Instruction(0, 0, Opcode.PS_SEL, (7, 1, 2, 3), record=True),
            ConcreteOps(),
        )
        # -0.0 selects C; a NaN predicate selects B.
        self.assertEqual((final.fpr[7], final.ps1[7]), (c0, b1))
        self.assertEqual(final.fpscr, state.fpscr)
        self.assertEqual(final.cr & 0x0F000000, 0x0A000000)

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

        single = execute_block(concrete_state({
            "gpr": {"r4": 0x100},
            "memory": {"bytes": {"0x100": 0x3F, "0x101": 0xC0}},
        }), decode("c0e40000"), ConcreteOps())
        self.assertEqual(single.fpr[7], 0x3FF8000000000000)
        self.assertEqual(single.ps1[7], 0x3FF8000000000000)

    def test_concrete_arithmetic_rejects_unvalidated_rounding_modes(self) -> None:
        state = concrete_state({"fpr": {"f1": 1.5, "f2": 2.0}, "fpscr": 1})
        with self.assertRaises(ExecutionInconclusive):
            execute_block(state, decode("fce1102a"), ConcreteOps())
        with self.assertRaises(ExecutionInconclusive):
            execute_instruction(
                state, Instruction(0, 0, Opcode.PS_RSQRTE, (7, 2)), ConcreteOps(),
            )

    def test_fmuls_force25_and_clears_fi_fr(self) -> None:
        state = concrete_state({
            "fpr": {"f1": 1.0, "f3": "0x3ff0000010000001"},
            "fpscr": 0x00060000,
        })
        final = execute_block(state, decode("ece100f2"), ConcreteOps())
        self.assertEqual(final.fpr[7], 0x3FF0000000000000)
        self.assertEqual(final.ps1[7], 0x3FF0000000000000)
        self.assertEqual(final.fpscr, 0x00004000)

    def test_reciprocal_estimates_use_broadway_tables_and_destination_shapes(self) -> None:
        state = concrete_state({
            "fpr": {"f2": 1.0, "f7": 3.0}, "ps1": {"f7": 4.0},
        })
        reciprocal = execute_block(state, decode("ece01030"), ConcreteOps())
        self.assertEqual(reciprocal.fpr[7], 0x3FEFFF0000000000)
        self.assertEqual(reciprocal.ps1[7], 0x3FEFFF0000000000)
        self.assertEqual(reciprocal.fpscr, 0x4000)

        rsqrt = execute_block(state, decode("fce01034"), ConcreteOps())
        self.assertEqual(rsqrt.fpr[7], 0x3FEFFE8000000000)
        self.assertEqual(rsqrt.ps1[7], state.ps1[7])
        self.assertEqual(rsqrt.fpscr, 0x4000)

        vectors = {
            0x4000000000000000: (0x3FDFFF0000000000, 0x3FE69FA000000000),
            0x0000000000000001: (0x47EFFFFFE0000000, 0x617FFE8000000000),
            0x7FF0000000000000: (0, 0),
            0xFFF0000000000000: (0x8000000000000000, 0x7FF8000000000000),
            0x7FF0000012345678: (0x7FF8000012345678, 0x7FF8000012345678),
        }
        for source, (fres_bits, frsqrte_bits) in vectors.items():
            with self.subTest(source=f"0x{source:016x}"):
                input_state = concrete_state({"fpr": {"f2": f"0x{source:016x}"}})
                fres = execute_block(input_state, decode("ece01030"), ConcreteOps())
                frsqrte = execute_block(input_state, decode("fce01034"), ConcreteOps())
                self.assertEqual(fres.fpr[7], fres_bits)
                self.assertEqual(frsqrte.fpr[7], frsqrte_bits)

    def test_reciprocal_estimate_exceptions_enables_and_record(self) -> None:
        original0 = 0x4008000000000000
        original1 = 0x4010000000000000
        zero_suppressed = execute_block(concrete_state({
            "fpr": {"f2": 0.0, "f7": original0}, "ps1": {"f7": original1},
            "fpscr": FPSCR_ZE,
        }), decode("ece01031"), ConcreteOps())
        self.assertEqual((zero_suppressed.fpr[7], zero_suppressed.ps1[7]), (original0, original1))
        self.assertEqual(
            zero_suppressed.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_ZX | FPSCR_ZE,
        )
        self.assertEqual(zero_suppressed.cr & 0x0F000000, 0x0C000000)

        snan_suppressed = execute_block(concrete_state({
            "fpr": {"f2": "0x7ff0000012345678", "f7": original0},
            "fpscr": FPSCR_VE,
        }), decode("fce01035"), ConcreteOps())
        self.assertEqual(snan_suppressed.fpr[7], original0)
        self.assertEqual(
            snan_suppressed.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXSNAN | FPSCR_VE,
        )

        negative = execute_block(concrete_state({
            "fpr": {"f2": -1.0}, "fpscr": FPSCR_FI | FPSCR_FR,
        }), decode("fce01034"), ConcreteOps())
        self.assertEqual(negative.fpr[7], 0x7FF8000000000000)
        self.assertEqual(
            negative.fpscr,
            FPSCR_FX | FPSCR_VX | FPSCR_VXSQRT | 0x11000,
        )

    def test_fpscr_exception_summary_foundation(self) -> None:
        ops = ConcreteOps()
        raised = _fpscr_raise(concrete_state(), FPSCR_VXIMZ, ops)
        self.assertEqual(raised.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXIMZ)
        enabled = _fpscr_raise(concrete_state({"fpscr": FPSCR_VE}), FPSCR_VXIMZ, ops)
        self.assertEqual(
            enabled.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXIMZ | FPSCR_VE,
        )
        already_sticky = _fpscr_raise(concrete_state({"fpscr": FPSCR_VXIMZ}), FPSCR_VXIMZ, ops)
        self.assertEqual(already_sticky.fpscr, FPSCR_VX | FPSCR_VXIMZ)

    def test_ordered_compare_nan_exception_behavior(self) -> None:
        ordered = decode("fd811040")
        qnan = execute_block(concrete_state({
            "fpr": {"f1": "0x7ff8000012345678", "f2": 2.0},
        }), ordered, ConcreteOps())
        self.assertEqual(qnan.cr, 0x00010000)
        self.assertEqual(qnan.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXVC | 0x1000)

        snan_enabled = execute_block(concrete_state({
            "fpr": {"f1": "0x7ff0000012345678", "f2": 2.0},
            "fpscr": FPSCR_VE,
        }), ordered, ConcreteOps())
        self.assertEqual(snan_enabled.cr, 0x00010000)
        self.assertEqual(
            snan_enabled.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXSNAN | FPSCR_VE | 0x1000,
        )

    def test_unordered_compare_signaling_nan_sets_vxsnan(self) -> None:
        final = execute_block(concrete_state({
            "fpr": {"f1": "0x7ff0000000000001", "f2": 2.0},
        }), decode("fc011000"), ConcreteOps())
        self.assertEqual(final.cr, 0x10000000)
        self.assertEqual(final.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXSNAN | 0x1000)

    def test_arithmetic_invalid_causes_and_nan_results(self) -> None:
        vectors = (
            ("fce1102a", {"f1": "0x7ff0000000000000", "f2": "0xfff0000000000000"}, FPSCR_VXISI),
            ("fce11028", {"f1": "0x7ff0000000000000", "f2": "0x7ff0000000000000"}, FPSCR_VXISI),
            ("fce100f2", {"f1": 0.0, "f3": "0x7ff0000000000000"}, FPSCR_VXIMZ),
            ("fce11024", {"f1": 0.0, "f2": "0x8000000000000000"}, FPSCR_VXZDZ),
            ("fce11024", {"f1": "0x7ff0000000000000", "f2": "0xfff0000000000000"}, FPSCR_VXIDI),
        )
        for code, fpr, cause in vectors:
            with self.subTest(code=code, cause=f"0x{cause:08x}"):
                final = execute_block(concrete_state({"fpr": fpr}), decode(code), ConcreteOps())
                self.assertEqual(final.fpr[7], 0x7FF8000000000000)
                self.assertEqual(final.fpscr, FPSCR_FX | FPSCR_VX | cause | 0x11000)

    def test_arithmetic_enable_bits_suppress_destination_and_fprf(self) -> None:
        original = 0x3FF0000000000000
        invalid = execute_block(concrete_state({
            "fpr": {
                "f1": "0x7ff0000000000000",
                "f2": "0xfff0000000000000",
                "f7": f"0x{original:016x}",
            },
            "fpscr": FPSCR_VE,
        }), decode("fce1102a"), ConcreteOps())
        self.assertEqual(invalid.fpr[7], original)
        self.assertEqual(invalid.fpscr, FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXISI | FPSCR_VE)

        divide_zero = execute_block(concrete_state({
            "fpr": {"f1": 2.0, "f2": "0x8000000000000000", "f7": original},
            "fpscr": FPSCR_ZE,
        }), decode("fce11024"), ConcreteOps())
        self.assertEqual(divide_zero.fpr[7], original)
        self.assertEqual(divide_zero.fpscr, FPSCR_FX | FPSCR_FEX | FPSCR_ZX | FPSCR_ZE)

    def test_signaling_nan_payload_is_quieted_and_single_is_truncated(self) -> None:
        double = execute_block(concrete_state({
            "fpr": {"f1": "0xfff0000012345678", "f2": 2.0},
        }), decode("fce1102a"), ConcreteOps())
        self.assertEqual(double.fpr[7], 0xFFF8000012345678)
        self.assertEqual(double.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXSNAN | 0x11000)

        single = execute_block(concrete_state({
            "fpr": {"f1": "0xfff0000012345678", "f2": 2.0},
        }), decode("ece1102a"), ConcreteOps())
        self.assertEqual(single.fpr[7], 0xFFF8000000000000)
        self.assertEqual(single.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXSNAN | 0x11000)

        quiet = execute_block(concrete_state({
            "fpr": {"f1": "0x7ff8000012345678", "f2": 2.0},
        }), decode("fce1102a"), ConcreteOps())
        self.assertEqual(quiet.fpr[7], 0x7FF8000012345678)
        self.assertEqual(quiet.fpscr, 0x11000)

    def test_float_to_integer_rounding_flags_and_packed_result(self) -> None:
        vectors = (
            ("fce0101c", 1.5, 0, 0xFFF8000000000002, FPSCR_FX | FPSCR_XX | 0x60000),
            ("fce0101c", 2.5, 0, 0xFFF8000000000002, FPSCR_FX | FPSCR_XX | FPSCR_FI),
            ("fce0101c", -1.4, 3, 0xFFF80000FFFFFFFE, FPSCR_FX | FPSCR_XX | 0x60003),
            ("fce0101e", -0.4, 3, 0xFFF8000100000000, FPSCR_FX | FPSCR_XX | FPSCR_FI | 3),
        )
        for code, source, fpscr, expected_bits, expected_fpscr in vectors:
            with self.subTest(code=code, source=source, fpscr=fpscr):
                final = execute_block(concrete_state({
                    "fpr": {"f2": source}, "fpscr": fpscr,
                }), decode(code), ConcreteOps())
                self.assertEqual(final.fpr[7], expected_bits)
                self.assertEqual(final.fpscr, expected_fpscr)

    def test_float_to_integer_invalid_and_enabled_suppression(self) -> None:
        cases = (
            ("0x7ff8000012345678", FPSCR_VXCVI),
            ("0x7ff0000012345678", FPSCR_VXSNAN | FPSCR_VXCVI),
            (2147483648.0, FPSCR_VXCVI),
            (-2147483649.0, FPSCR_VXCVI),
        )
        for source, causes in cases:
            with self.subTest(source=source):
                final = execute_block(concrete_state({"fpr": {"f2": source}}), decode("fce0101c"), ConcreteOps())
                self.assertEqual(final.fpscr, FPSCR_FX | FPSCR_VX | causes)

        original = 0x3FF0000000000000
        suppressed = execute_block(concrete_state({
            "fpr": {"f2": "0x7ff8000012345678", "f7": original},
            "fpscr": FPSCR_VE,
        }), decode("fce0101d"), ConcreteOps())
        self.assertEqual(suppressed.fpr[7], original)
        self.assertEqual(
            suppressed.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXCVI | FPSCR_VE,
        )
        self.assertEqual(suppressed.cr, 0x0E000000)

    def test_float_to_integer_inexact_enable_sets_fex(self) -> None:
        final = execute_block(concrete_state({
            "fpr": {"f2": 1.5}, "fpscr": FPSCR_XE,
        }), decode("fce0101c"), ConcreteOps())
        self.assertEqual(
            final.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_XX | FPSCR_FI | FPSCR_FR | FPSCR_XE,
        )

    def test_fmadds_force25_and_single_rounding_correction(self) -> None:
        force25 = execute_block(concrete_state({
            "fpr": {"f1": 1.0, "f2": 0.0, "f3": "0x3ff0000010000001"},
        }), decode("ece110fa"), ConcreteOps())
        self.assertEqual(force25.fpr[7], 0x3FF0000000000000)
        self.assertEqual(force25.fpscr, FPSCR_FI | 0x4000)

        # Dolphin's documented single-rounding counterexample: a double FMA
        # followed by a float cast gives ...18, Broadway fmadds gives ...17.
        corrected = execute_block(concrete_state({
            "fpr": {
                "f1": "0x4049000000000000",
                "f2": "0x3b638e5400000000",
                "f3": "0xbf91198700000000",
            },
        }), decode("ece110fa"), ConcreteOps())
        self.assertEqual(corrected.fpr[7], 0xBFEAB7E2E0000000)
        self.assertEqual(corrected.fpscr, FPSCR_FI | 0x8000)

    def test_fmadds_nan_order_invalid_and_suppression(self) -> None:
        nan_order = execute_block(concrete_state({
            "fpr": {
                "f1": "0x7ff1000000000001",
                "f2": "0x7ff2000000000001",
                "f3": "0x7ff3000000000001",
            },
        }), decode("ece110fa"), ConcreteOps())
        self.assertEqual(nan_order.fpr[7], 0x7FF9000000000000)
        self.assertEqual(
            nan_order.fpscr,
            FPSCR_FX | FPSCR_VX | FPSCR_VXSNAN | FPSCR_FI | 0x11000,
        )

        vximz = execute_block(concrete_state({
            "fpr": {"f1": 0.0, "f2": 1.0, "f3": "0x7ff0000000000000"},
        }), decode("ece110fa"), ConcreteOps())
        self.assertEqual(vximz.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXIMZ | FPSCR_FI | 0x11000)

        original = 0x4000000000000000
        suppressed = execute_block(concrete_state({
            "fpr": {
                "f1": 0.0,
                "f2": 1.0,
                "f3": "0x7ff0000000000000",
                "f7": original,
            },
            "fpscr": FPSCR_VE,
        }), decode("ece110fb"), ConcreteOps())
        self.assertEqual(suppressed.fpr[7], original)
        self.assertEqual(
            suppressed.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXIMZ | FPSCR_VE,
        )
        self.assertEqual(suppressed.cr, 0x0E000000)

    def test_fused_subtract_and_negative_subtract_results(self) -> None:
        initial_fpscr = FPSCR_FI | FPSCR_FR
        state = concrete_state({
            "fpr": {"f1": 1.5, "f2": 2.0, "f3": 4.0},
            "fpscr": initial_fpscr,
        })
        subtracted = execute_block(state, decode("ece110f8"), ConcreteOps())
        negated = execute_block(state, decode("ece110fc"), ConcreteOps())
        self.assertEqual(subtracted.fpr[7], 0x4010000000000000)
        self.assertEqual(subtracted.fpscr, initial_fpscr | 0x4000)
        self.assertEqual(negated.fpr[7], 0xC010000000000000)
        self.assertEqual(negated.fpscr, initial_fpscr | 0x8000)

        zero_state = concrete_state({"fpr": {"f1": 1.0, "f2": 1.0, "f3": 1.0}})
        positive_zero = execute_block(zero_state, decode("ece110f8"), ConcreteOps())
        negative_zero = execute_block(zero_state, decode("ece110fc"), ConcreteOps())
        self.assertEqual(positive_zero.fpr[7], 0x0000000000000000)
        self.assertEqual(positive_zero.fpscr, 0x2000)
        self.assertEqual(negative_zero.fpr[7], 0x8000000000000000)
        self.assertEqual(negative_zero.fpscr, 0x12000)

    def test_fused_subtract_nan_is_not_negated_and_invalid_signs(self) -> None:
        nan_state = concrete_state({
            "fpr": {"f1": "0xfff8000012345678", "f2": 2.0, "f3": 4.0},
        })
        for encoded in ("ece110f8", "ece110fc"):
            with self.subTest(encoded=encoded, case="nan"):
                final = execute_block(nan_state, decode(encoded), ConcreteOps())
                self.assertEqual(final.fpr[7], 0xFFF8000000000000)
                self.assertEqual(final.fpscr, 0x11000)

        invalid_state = concrete_state({
            "fpr": {
                "f1": "0x7ff0000000000000",
                "f2": "0x7ff0000000000000",
                "f3": 1.0,
            },
        })
        for encoded in ("ece110f8", "ece110fc"):
            with self.subTest(encoded=encoded, case="vxisi"):
                final = execute_block(invalid_state, decode(encoded), ConcreteOps())
                self.assertEqual(final.fpr[7], 0x7FF8000000000000)
                self.assertEqual(final.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXISI | 0x11000)

    def test_complete_fused_family_results_flags_and_signed_zero(self) -> None:
        cases = {
            "fce110fa": (0x4020000000000000, 0x4000, -1.0, 0),
            "fce110f8": (0x4010000000000000, 0x4000, 1.0, 0),
            "fce110fe": (0xC020000000000000, 0x8000, -1.0, 0x8000000000000000),
            "fce110fc": (0xC010000000000000, 0x8000, 1.0, 0x8000000000000000),
            "ece110fe": (0xC020000000000000, 0x8000, -1.0, 0x8000000000000000),
        }
        for encoded, (expected, fprf, zero_b, expected_zero) in cases.items():
            with self.subTest(encoded=encoded, case="finite"):
                state = concrete_state({
                    "fpr": {"f1": 1.5, "f2": 2.0, "f3": 4.0},
                    "fpscr": FPSCR_FI | FPSCR_FR,
                })
                final = execute_block(state, decode(encoded), ConcreteOps())
                self.assertEqual(final.fpr[7], expected)
                self.assertEqual(final.fpscr, FPSCR_FI | FPSCR_FR | fprf)
            with self.subTest(encoded=encoded, case="zero"):
                state = concrete_state({"fpr": {"f1": 1.0, "f2": zero_b, "f3": 1.0}})
                final = execute_block(state, decode(encoded), ConcreteOps())
                self.assertEqual(final.fpr[7], expected_zero)

    def test_double_fused_family_rounds_only_once(self) -> None:
        # (1 + 2^-27) * (1 - 2^-27) is exactly 1 - 2^-54. A separate
        # multiply rounds that to 1 before the add/subtract; an FMA retains it.
        product_inputs = {
            "f1": "0x3ff0000002000000",
            "f3": "0x3feffffffc000000",
        }
        cases = {
            "fce110fa": (-1.0, 0xBC90000000000000),
            "fce110f8": (1.0, 0xBC90000000000000),
            "fce110fe": (-1.0, 0x3C90000000000000),
            "fce110fc": (1.0, 0x3C90000000000000),
        }
        for encoded, (addend, expected) in cases.items():
            with self.subTest(encoded=encoded):
                state = concrete_state({"fpr": {**product_inputs, "f2": addend}})
                final = execute_block(state, decode(encoded), ConcreteOps())
                self.assertEqual(final.fpr[7], expected)

    def test_complete_fused_family_preserves_nan_payload_and_never_negates_nan(self) -> None:
        for encoded in ("fce110fa", "fce110f8", "fce110fe", "fce110fc"):
            with self.subTest(encoded=encoded):
                state = concrete_state({
                    "fpr": {"f1": "0xfff8000012345678", "f2": 2.0, "f3": 4.0},
                })
                final = execute_block(state, decode(encoded), ConcreteOps())
                self.assertEqual(final.fpr[7], 0xFFF8000012345678)
                self.assertEqual(final.fpscr, 0x11000)

        single = execute_block(concrete_state({
            "fpr": {"f1": "0xfff8000012345678", "f2": 2.0, "f3": 4.0},
        }), decode("ece110fe"), ConcreteOps())
        self.assertEqual(single.fpr[7], 0xFFF8000000000000)
        self.assertEqual(single.fpscr, 0x11000)

    def test_double_fused_invalid_signs_and_enabled_suppression(self) -> None:
        positive_inf = "0x7ff0000000000000"
        negative_inf = "0xfff0000000000000"
        cases = (
            ("fce110fa", negative_inf), ("fce110fe", negative_inf),
            ("fce110f8", positive_inf), ("fce110fc", positive_inf),
        )
        for encoded, addend in cases:
            with self.subTest(encoded=encoded):
                state = concrete_state({
                    "fpr": {"f1": positive_inf, "f2": addend, "f3": 1.0},
                })
                final = execute_block(state, decode(encoded), ConcreteOps())
                self.assertEqual(final.fpr[7], 0x7FF8000000000000)
                self.assertEqual(final.fpscr, FPSCR_FX | FPSCR_VX | FPSCR_VXISI | 0x11000)

        original = 0x4000000000000000
        suppressed = execute_block(concrete_state({
            "fpr": {"f1": positive_inf, "f2": negative_inf, "f3": 1.0, "f7": original},
            "fpscr": FPSCR_VE,
        }), decode("fce110ff"), ConcreteOps())
        self.assertEqual(suppressed.fpr[7], original)
        self.assertEqual(
            suppressed.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXISI | FPSCR_VE,
        )
        self.assertEqual(suppressed.cr, 0x0E000000)

    def test_fpscr_access_and_summary_recomputation(self) -> None:
        mffs = execute_block(
            concrete_state({"fpscr": 0xA0100080}), decode("fce0048e"), ConcreteOps(),
        )
        self.assertEqual(mffs.fpr[7], 0xFFF80000A0100080)
        recorded = execute_block(
            concrete_state({"fpscr": 0x80000000}), decode("fce0048f"), ConcreteOps(),
        )
        self.assertEqual(recorded.cr, 0x08000000)

        # Copy VXIMZ, VE, and the reserved bit. VX/FEX are recomputed and the
        # reserved bit is forced to zero; Rc copies the final summary to CR1.
        mtfsf = execute_block(concrete_state({
            "fpr": {"f2": "0xfff8000000100880"},
        }), decode("fdfe158f"), ConcreteOps())
        self.assertEqual(mtfsf.fpscr, 0x60100080)
        self.assertEqual(mtfsf.cr, 0x06000000)

    def test_fpscr_bit_immediate_and_mcrfs_behavior(self) -> None:
        raised = execute_block(
            concrete_state({"fpscr": FPSCR_VE}), decode("fd60004c"), ConcreteOps(),
        )
        self.assertEqual(
            raised.fpscr,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXIMZ | FPSCR_VE,
        )

        cleared = execute_block(raised, decode("fd60008c"), ConcreteOps())
        self.assertEqual(cleared.fpscr, FPSCR_FX | FPSCR_VE)

        moved = execute_block(
            concrete_state({"fpscr": FPSCR_FX | FPSCR_VX | FPSCR_VXIMZ}),
            decode("fd880080"), ConcreteOps(),
        )
        self.assertEqual(moved.cr, 0x00010000)
        self.assertEqual(moved.fpscr, FPSCR_FX)

        rounding = execute_block(concrete_state(), decode("ff80310c"), ConcreteOps())
        self.assertEqual(rounding.fpscr, 3)
        reserved = execute_block(concrete_state(), decode("fe80810c"), ConcreteOps())
        self.assertEqual(reserved.fpscr, 0)

    def test_gqr_spr_round_trip_and_psq_decoder_forms(self) -> None:
        def spr_word(reg: int, spr: int, xo_bits: int) -> int:
            return (
                (31 << 26) | (reg << 21) | ((spr & 31) << 16)
                | (((spr >> 5) & 31) << 11) | (xo_bits << 1)
            )

        code = f"{spr_word(5, 913, 467):08x} {spr_word(7, 913, 339):08x}"
        final = execute_block(
            concrete_state({"gpr": {"r5": "0x01060204"}}), decode(code), ConcreteOps(),
        )
        self.assertEqual(final.gqr[1], 0x01060204)
        self.assertEqual(final.gpr[7], 0x01060204)

        words = "e0e41000 e4e41000 f0e41000 f4e41000 10e4288c 10e428cc 10e4288e 10e428ce"
        self.assertEqual(
            [item.opcode for item in decode(words)],
            [Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU,
             Opcode.PSQ_LX, Opcode.PSQ_LUX, Opcode.PSQ_STX, Opcode.PSQ_STUX],
        )

    def test_psq_load_all_gqr_formats_and_w_lane(self) -> None:
        def psq_l(i: int, *, w: int = 0) -> str:
            return f"{((56 << 26) | (7 << 21) | (4 << 16) | (w << 15) | (i << 12)):08x}"

        cases = (
            (0x00000000, [0x3F, 0x80, 0, 0, 0xC0, 0x20, 0, 0], 0x3FF0000000000000, 0xC004000000000000),
            (0x01040000, [2, 8], 0x3FF0000000000000, 0x4010000000000000),
            (0x01050000, [0, 2, 0, 8], 0x3FF0000000000000, 0x4010000000000000),
            (0x01060000, [0xFE, 8], 0xBFF0000000000000, 0x4010000000000000),
            (0x01070000, [0xFF, 0xFE, 0, 8], 0xBFF0000000000000, 0x4010000000000000),
            (0x3F040000, [2, 8], 0x4010000000000000, 0x4030000000000000),
        )
        for gqr, values, expected0, expected1 in cases:
            with self.subTest(gqr=f"0x{gqr:08x}"):
                memory = {f"0x{0x1000 + index:08x}": value for index, value in enumerate(values)}
                final = execute_block(concrete_state({
                    "gpr": {"r4": 0x1000}, "gqr": {"gqr1": gqr},
                    "memory": {"bytes": memory},
                }), decode(psq_l(1)), ConcreteOps())
                self.assertEqual(final.fpr[7], expected0)
                self.assertEqual(final.ps1[7], expected1)

        one_lane = execute_block(concrete_state({
            "gpr": {"r4": 0x1000}, "gqr": {"gqr1": 0x01060000},
            "memory": {"bytes": {"0x1000": 0xFE}},
        }), decode(psq_l(1, w=1)), ConcreteOps())
        self.assertEqual(one_lane.fpr[7], 0xBFF0000000000000)
        self.assertEqual(one_lane.ps1[7], 0x3FF0000000000000)

    def test_psq_store_quantization_clamping_ftz_and_update(self) -> None:
        u8_store = execute_block(concrete_state({
            "gpr": {"r4": 0x1000}, "gqr": {"gqr1": 0x00000204},
            "fpr": {"f7": 100.0}, "ps1": {"f7": -1.0},
        }), decode("f0e41000"), ConcreteOps())
        self.assertEqual([u8_store.memory.read(0x1000 + i) for i in range(2)], [0xFF, 0])

        s16_store = execute_block(concrete_state({
            "gpr": {"r4": 0x1000}, "gqr": {"gqr1": 0x00003F07},
            "fpr": {"f7": 4.0}, "ps1": {"f7": -4.0},
        }), decode("f0e41000"), ConcreteOps())
        self.assertEqual(
            [s16_store.memory.read(0x1000 + i) for i in range(4)],
            [0, 2, 0xFF, 0xFE],
        )

        ftz = execute_block(concrete_state({
            "gpr": {"r4": 0x1000}, "fpr": {"f7": "0x36a0000000000000"},
            "ps1": {"f7": "0xb6a0000000000000"},
        }), decode("f0e40000"), ConcreteOps())
        self.assertEqual(
            [ftz.memory.read(0x1000 + i) for i in range(8)],
            [0, 0, 0, 0, 0x80, 0, 0, 0],
        )

        updated = execute_block(concrete_state({
            "gpr": {"r4": 0x1000}, "gqr": {"gqr1": 0x01060000},
            "memory": {"bytes": {"0x1004": 2, "0x1005": 4}},
        }), decode("e4e41004"), ConcreteOps())
        self.assertEqual(updated.gpr[4], 0x1004)
        self.assertEqual(updated.fpr[7], 0x3FF0000000000000)

    def test_psq_invalid_type_and_nonfinite_integer_store_are_outside_domain(self) -> None:
        invalid_type = execute_block(concrete_state({
            "gpr": {"r4": 0x1000}, "gqr": {"gqr1": 0x00010000},
        }), decode("e0e41000"), ConcreteOps())
        self.assertFalse(invalid_type.valid)

        nan_store = execute_block(concrete_state({
            "gpr": {"r4": 0x1000}, "gqr": {"gqr1": 4},
            "fpr": {"f7": "0x7ff8000000000000"}, "ps1": {"f7": 1.0},
        }), decode("f0e41000"), ConcreteOps())
        self.assertFalse(nan_store.valid)


@unittest.skipUnless(importlib.util.find_spec("z3"), "z3-solver is not installed")
class FloatingPointSymbolicTests(unittest.TestCase):
    def test_signaling_nan_classification_matches_concrete(self) -> None:
        symbolic = SymbolicOps()
        concrete = ConcreteOps()
        vectors = {
            0x7FF0000000000000: False,
            0x7FF0000000000001: True,
            0xFFF0000012345678: True,
            0x7FF8000000000000: False,
            0x7FF8000012345678: False,
            0x3FF0000000000000: False,
        }
        for bits, expected in vectors.items():
            with self.subTest(bits=f"0x{bits:016x}"):
                self.assertEqual(concrete.fp_is_snan_bits(bits), expected)
                expression = symbolic.fp_is_snan_bits(symbolic.fp_const64(bits))
                self.assertEqual(symbolic.z3.is_true(symbolic.z3.simplify(expression)), expected)

    def test_identical_fcmpo_is_proved_for_nan_domain(self) -> None:
        code = decode("fd811040")
        result = check_equivalence(
            code, code, EquivalenceContract(parse_observables(["cr3,fpscr"]), timeout_ms=5_000),
            original_hex="fd811040", candidate_hex="fd811040",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_ordered_and_unordered_compare_differ_on_nan_exceptions(self) -> None:
        result = check_equivalence(
            decode("fc011040"), decode("fc011000"),
            EquivalenceContract(parse_observables(["cr0,fpscr"]), timeout_ms=5_000),
            original_hex="fc011040", candidate_hex="fc011000",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_identical_exceptional_arithmetic_is_proved(self) -> None:
        for encoded in ("fce1102a", "fce100f2", "fce11024"):
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(parse_observables(["f7,fpscr,cr1"]), timeout_ms=5_000),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_reciprocal_estimate_symbolic_proofs(self) -> None:
        for encoded, observables in (
            ("ece01031", ["f7,f7.ps1,fpscr,cr1"]),
            ("fce01035", ["f7,fpscr,cr1"]),
        ):
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(parse_observables(observables), timeout_ms=10_000),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        different = check_equivalence(
            decode("ece01030"), decode("fce01034"),
            EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=10_000),
            original_hex="ece01030", candidate_hex="fce01034",
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_float_to_integer_symbolic_proofs(self) -> None:
        for encoded in ("fce0101c", "fce0101e"):
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=5_000),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        different = check_equivalence(
            decode("fce0101c"), decode("fce0101e"),
            EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=5_000),
            original_hex="fce0101c", candidate_hex="fce0101e",
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_fmadds_symbolic_proofs_on_single_origin_domain(self) -> None:
        fused = decode("ece110fa")
        identical = check_equivalence(
            fused, fused,
            EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=5_000),
            original_hex="ece110fa", candidate_hex="ece110fa",
        )
        self.assertEqual(identical.status, ProofStatus.EQUIVALENT)

        different = check_equivalence(
            fused, decode("ece100f2"),
            EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=5_000),
            original_hex="ece110fa", candidate_hex="ece100f2",
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_fused_subtract_symbolic_proofs(self) -> None:
        for encoded in ("ece110f8", "ece110fc"):
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                identical = check_equivalence(
                    code, code,
                    EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=5_000),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(identical.status, ProofStatus.EQUIVALENT)

        sign_difference = check_equivalence(
            decode("ece110f8"), decode("ece110fc"),
            EquivalenceContract(parse_observables(["f7"]), timeout_ms=10_000),
            original_hex="ece110f8", candidate_hex="ece110fc",
        )
        self.assertEqual(sign_difference.status, ProofStatus.NOT_EQUIVALENT)

    def test_complete_fused_family_symbolic_proofs(self) -> None:
        for encoded in ("ece110fe", "fce110fa", "fce110f8", "fce110fe", "fce110fc"):
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                identical = check_equivalence(
                    code, code,
                    EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=5_000),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(identical.status, ProofStatus.EQUIVALENT)

        different = check_equivalence(
            decode("ece110fa"), decode("ece110fe"),
            EquivalenceContract(parse_observables(["f7"]), timeout_ms=15_000),
            original_hex="ece110fa", candidate_hex="ece110fe",
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_fpscr_control_symbolic_proofs(self) -> None:
        for encoded in ("fce0048e", "fdfe158f", "ff80310c", "fd60004c", "fd60008c", "fd880080"):
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(parse_observables(["f7,cr3,fpscr"]), timeout_ms=5_000),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_psq_symbolic_identity_and_lane_distinction(self) -> None:
        for encoded, observe in (("e0e41000", "f7,f7.ps1"), ("f0e41000", "memory")):
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(parse_observables([observe]), timeout_ms=5_000),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        def mtspr(reg: int, spr: int) -> int:
            return (
                (31 << 26) | (reg << 21) | ((spr & 31) << 16)
                | (((spr >> 5) & 31) << 11) | (467 << 1)
            )

        prefix = f"38a00000 {mtspr(5, 913):08x} "
        pair = prefix + "e0e41000"
        one = prefix + "e0e49000"
        different = check_equivalence(
            decode(pair), decode(one),
            EquivalenceContract(parse_observables(["f7.ps1"]), timeout_ms=5_000),
            original_hex=pair, candidate_hex=one,
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_paired_move_and_merge_symbolic_proofs(self) -> None:
        words = (
            ps_x(7, 0, 2, 40, rc=1), ps_x(7, 0, 2, 72),
            ps_x(7, 0, 2, 136), ps_x(7, 0, 2, 264),
            ps_x(7, 1, 2, 528), ps_x(7, 1, 2, 560),
            ps_x(7, 1, 2, 592), ps_x(7, 1, 2, 624),
        )
        for word in words:
            encoded = f"{word:08x}"
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(
                        parse_observables(["f7,f7.ps1,cr1"]), timeout_ms=5_000,
                    ),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        merge00 = f"{ps_x(7, 1, 2, 528):08x}"
        merge01 = f"{ps_x(7, 1, 2, 560):08x}"
        different = check_equivalence(
            decode(merge00), decode(merge01),
            EquivalenceContract(parse_observables(["f7.ps1"]), timeout_ms=5_000),
            original_hex=merge00, candidate_hex=merge01,
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_paired_compare_symbolic_proofs(self) -> None:
        words = (
            ps_x(3 << 2, 1, 2, 0), ps_x(3 << 2, 1, 2, 32),
            ps_x(3 << 2, 1, 2, 64), ps_x(3 << 2, 1, 2, 96),
        )
        for word in words:
            encoded = f"{word:08x}"
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(parse_observables(["cr3,fpscr"]), timeout_ms=5_000),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        cmp0 = f"{ps_x(3 << 2, 1, 2, 0):08x}"
        cmp1 = f"{ps_x(3 << 2, 1, 2, 64):08x}"
        lane_difference = check_equivalence(
            decode(cmp0), decode(cmp1),
            EquivalenceContract(parse_observables(["cr3,fpscr"]), timeout_ms=5_000),
            original_hex=cmp0, candidate_hex=cmp1,
        )
        self.assertEqual(lane_difference.status, ProofStatus.NOT_EQUIVALENT)

        ordered = f"{ps_x(3 << 2, 1, 2, 32):08x}"
        ordering_difference = check_equivalence(
            decode(ordered), decode(cmp0),
            EquivalenceContract(parse_observables(["fpscr"]), timeout_ms=5_000),
            original_hex=ordered, candidate_hex=cmp0,
        )
        self.assertEqual(ordering_difference.status, ProofStatus.NOT_EQUIVALENT)

    def test_basic_paired_arithmetic_symbolic_proofs(self) -> None:
        words = (
            ps_a(7, 1, 2, 0, 21), ps_a(7, 1, 2, 0, 20),
            ps_a(7, 1, 0, 3, 25), ps_a(7, 1, 0, 3, 12),
            ps_a(7, 1, 0, 3, 13),
        )
        for word in words:
            encoded = f"{word:08x}"
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(
                        parse_observables(["f7,f7.ps1,fpscr"]), timeout_ms=10_000,
                    ),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        add = f"{ps_a(7, 1, 2, 0, 21):08x}"
        sub = f"{ps_a(7, 1, 2, 0, 20):08x}"
        different = check_equivalence(
            decode(add), decode(sub),
            EquivalenceContract(parse_observables(["f7,f7.ps1,fpscr"]), timeout_ms=10_000),
            original_hex=add, candidate_hex=sub,
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_paired_division_and_estimate_symbolic_proofs(self) -> None:
        words = (
            ps_a(7, 1, 2, 0, 18, rc=1),
            ps_a(7, 0, 2, 0, 24, rc=1),
            ps_a(7, 0, 2, 0, 26, rc=1),
        )
        for word in words:
            encoded = f"{word:08x}"
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(
                        parse_observables(["f7,f7.ps1,fpscr,cr1"]), timeout_ms=15_000,
                    ),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        reciprocal = f"{ps_a(7, 0, 2, 0, 24):08x}"
        rsqrt = f"{ps_a(7, 0, 2, 0, 26):08x}"
        different = check_equivalence(
            decode(reciprocal), decode(rsqrt),
            EquivalenceContract(
                parse_observables(["f7,f7.ps1,fpscr"]), timeout_ms=15_000,
            ),
            original_hex=reciprocal, candidate_hex=rsqrt,
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_paired_fused_symbolic_proofs(self) -> None:
        words = tuple(ps_a(7, 1, 2, 3, xo) for xo in (29, 28, 31, 30, 14, 15))
        for word in words:
            encoded = f"{word:08x}"
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(
                        parse_observables(["f7,f7.ps1,fpscr"]), timeout_ms=10_000,
                    ),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        madd = f"{ps_a(7, 1, 2, 3, 29):08x}"
        msub = f"{ps_a(7, 1, 2, 3, 28):08x}"
        different = check_equivalence(
            decode(madd), decode(msub),
            EquivalenceContract(parse_observables(["f7,f7.ps1"]), timeout_ms=20_000),
            original_hex=madd, candidate_hex=msub,
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_paired_sum_and_select_symbolic_proofs(self) -> None:
        words = tuple(ps_a(7, 1, 2, 3, xo) for xo in (10, 11, 23))
        for word in words:
            encoded = f"{word:08x}"
            with self.subTest(encoded=encoded):
                code = decode(encoded)
                result = check_equivalence(
                    code, code,
                    EquivalenceContract(
                        parse_observables(["f7,f7.ps1,cr1,fpscr"]), timeout_ms=10_000,
                    ),
                    original_hex=encoded, candidate_hex=encoded,
                )
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        sum0 = f"{ps_a(7, 1, 2, 3, 10):08x}"
        sum1 = f"{ps_a(7, 1, 2, 3, 11):08x}"
        different = check_equivalence(
            decode(sum0), decode(sum1),
            EquivalenceContract(parse_observables(["f7,f7.ps1,fpscr"]), timeout_ms=10_000),
            original_hex=sum0, candidate_hex=sum1,
        )
        self.assertEqual(different.status, ProofStatus.NOT_EQUIVALENT)

    def test_force25_symbolic_matches_concrete_edge(self) -> None:
        ops = SymbolicOps()
        vectors = {
            0x0000000000000000: 0x0000000000000000,
            0x8000000000000000: 0x8000000000000000,
            0x0000000000000001: 0x0000000000000001,
            0x0000000002000001: 0x0000000002000002,
            0x3FF0000010000001: 0x3FF0000010000000,
            0xBFF0000010000001: 0xBFF0000010000000,
            0x7FF0000000000000: 0x7FF0000000000000,
        }
        concrete = ConcreteOps()
        for source, expected in vectors.items():
            with self.subTest(source=f"0x{source:016x}"):
                self.assertEqual(concrete.fp_force_25bit(source), expected)
                expression = ops.fp_force_25bit(ops.fp_const64(source))
                self.assertEqual(ops.z3.simplify(expression).as_long(), expected)

    def test_identical_fmuls_is_proved(self) -> None:
        code = decode("ece100f2")
        result = check_equivalence(
            code, code, EquivalenceContract(parse_observables(["f7,fpscr"]), timeout_ms=5_000),
            original_hex="ece100f2", candidate_hex="ece100f2",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_fp_automatic_live_out_tracks_fpr_fpscr_and_cr1(self) -> None:
        self.assertEqual(
            automatic_live_out(decode("fce1102b")),
            ("f7", "cr1", "fpscr"),
        )
        self.assertEqual(
            automatic_live_out(decode("fc011000")),
            ("cr0", "fpscr"),
        )
        self.assertEqual(
            automatic_live_out(decode("fce0101d")),
            ("f7", "cr1", "fpscr"),
        )
        self.assertEqual(
            automatic_live_out(decode("e0e41000")),
            ("f7", "f7.ps1"),
        )
        self.assertEqual(
            automatic_live_out(decode(f"{ps_x(7, 1, 2, 560, rc=1):08x}")),
            ("f7", "f7.ps1", "cr1"),
        )
        paired_reads, paired_writes = register_effects(
            decode(f"{ps_x(7, 1, 2, 560):08x}")[0],
        )
        self.assertEqual(
            paired_reads,
            {"f1", "f1.ps1", "f2", "f2.ps1"},
        )
        self.assertEqual(paired_writes, {"f7", "f7.ps1"})
        paired_compare = decode(f"{ps_x(3 << 2, 1, 2, 64):08x}")[0]
        self.assertEqual(
            automatic_live_out([paired_compare]),
            ("cr3", "fpscr"),
        )
        compare_reads, compare_writes = register_effects(paired_compare)
        self.assertEqual(compare_reads, {"f1.ps1", "f2.ps1", "fpscr"})
        self.assertEqual(compare_writes, {"cr3", "fpscr"})
        paired_arithmetic = decode(f"{ps_a(7, 1, 0, 3, 25, rc=1):08x}")[0]
        self.assertEqual(
            automatic_live_out([paired_arithmetic]),
            ("f7", "f7.ps1", "cr1", "fpscr"),
        )
        arithmetic_reads, arithmetic_writes = register_effects(paired_arithmetic)
        self.assertEqual(
            arithmetic_reads,
            {"f1", "f1.ps1", "f3", "f3.ps1", "fpscr"},
        )
        self.assertEqual(arithmetic_writes, {"f7", "f7.ps1", "fpscr", "cr1"})
        paired_division = decode(f"{ps_a(7, 1, 2, 0, 18, rc=1):08x}")[0]
        self.assertEqual(
            automatic_live_out([paired_division]),
            ("f7", "f7.ps1", "cr1", "fpscr"),
        )
        division_reads, division_writes = register_effects(paired_division)
        self.assertEqual(
            division_reads,
            {"f1", "f1.ps1", "f2", "f2.ps1", "fpscr"},
        )
        self.assertEqual(division_writes, {"f7", "f7.ps1", "fpscr", "cr1"})
        paired_estimate = decode(f"{ps_a(7, 0, 2, 0, 24, rc=1):08x}")[0]
        estimate_reads, estimate_writes = register_effects(paired_estimate)
        self.assertEqual(estimate_reads, {"f2", "f2.ps1", "fpscr"})
        self.assertEqual(estimate_writes, {"f7", "f7.ps1", "fpscr", "cr1"})
        paired_fused = decode(f"{ps_a(7, 1, 2, 3, 29, rc=1):08x}")[0]
        self.assertEqual(
            automatic_live_out([paired_fused]),
            ("f7", "f7.ps1", "cr1", "fpscr"),
        )
        fused_reads, fused_writes = register_effects(paired_fused)
        self.assertEqual(
            fused_reads,
            {
                "f1", "f1.ps1", "f2", "f2.ps1", "f3", "f3.ps1",
                "fpscr",
            },
        )
        self.assertEqual(fused_writes, {"f7", "f7.ps1", "fpscr", "cr1"})
        paired_sum = decode(f"{ps_a(7, 1, 2, 3, 11, rc=1):08x}")[0]
        sum_reads, sum_writes = register_effects(paired_sum)
        self.assertEqual(
            sum_reads,
            {
                "f1", "f1.ps1", "f2", "f2.ps1", "f3", "f3.ps1",
                "fpscr",
            },
        )
        self.assertEqual(sum_writes, {"f7", "f7.ps1", "fpscr", "cr1"})
        paired_select = decode(f"{ps_a(7, 1, 2, 3, 23, rc=1):08x}")[0]
        select_reads, select_writes = register_effects(paired_select)
        self.assertEqual(
            select_reads,
            {
                "f1", "f1.ps1", "f2", "f2.ps1", "f3", "f3.ps1",
                "fpscr",
            },
        )
        self.assertEqual(select_writes, {"f7", "f7.ps1", "cr1"})
        immediate_reads, _ = register_effects(
            Instruction(0, 0, Opcode.PSQ_L, (7, 4, 12, 0, 1)),
        )
        indexed_reads, _ = register_effects(
            Instruction(0, 0, Opcode.PSQ_LX, (7, 4, 12, 0, 1)),
        )
        self.assertEqual(immediate_reads & {"r4", "r12"}, {"r4"})
        self.assertEqual(indexed_reads & {"r4", "r12"}, {"r4", "r12"})

        mtspr_gqr1 = (31 << 26) | (5 << 21) | ((913 & 31) << 16) | (((913 >> 5) & 31) << 11) | (467 << 1)
        self.assertEqual(
            automatic_live_out(decode(f"{mtspr_gqr1:08x}")),
            ("gqr1",),
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
    def test_cache_barriers_and_cache_block_zero_value_semantics(self) -> None:
        initial = concrete_state({
            "gpr": {"r3": 0x1013},
            "memory": {"bytes": {f"0x{address:x}": 0xAA for address in range(0x1000, 0x1040)}},
        })
        unchanged = execute_block(initial, decode("7c0004ac 4c00012c 7c001a2c"), ConcreteOps())
        self.assertEqual(unchanged, initial)
        final = execute_block(initial, decode("7c001fec"), ConcreteOps())
        self.assertEqual([final.memory.read(a) for a in range(0x1000, 0x1020)], [0] * 32)
        self.assertEqual([final.memory.read(a) for a in range(0x1020, 0x1040)], [0xAA] * 32)

    def test_privileged_register_and_stable_time_base_semantics(self) -> None:
        state = concrete_state({
            "gpr": {"r5": 0x12345678}, "msr": 0x00002000,
            "sr": {"sr3": 0xABCDEF01}, "time_base": 0x1122334455667788,
            "srr0": 0x80001234, "srr1": 0x87654321,
        })
        self.assertEqual(execute_block(state, decode("7ce000a6"), ConcreteOps()).gpr[7], 0x2000)
        self.assertEqual(execute_block(state, decode("7ce304a6"), ConcreteOps()).gpr[7], 0xABCDEF01)
        self.assertEqual(execute_block(state, decode("7cec42e6"), ConcreteOps()).gpr[7], 0x55667788)
        self.assertEqual(execute_block(state, decode("7ced42e6"), ConcreteOps()).gpr[7], 0x11223344)
        self.assertEqual(execute_block(state, decode("7cba03a6 7cfa02a6"), ConcreteOps()).gpr[7], 0x12345678)
        self.assertEqual(execute_block(state, decode("7ca301a4"), ConcreteOps()).sr[3], 0x12345678)
        self.assertEqual(execute_block(state, decode("7ca00124"), ConcreteOps()).msr, 0x12345678)
        user = execute_block(concrete_state({"msr": 0x4000}), decode("7ce000a6"), ConcreteOps())
        self.assertFalse(user.valid)

    def test_trap_system_call_and_rfi_exception_state(self) -> None:
        state = concrete_state({"gpr": {"r3": 0}, "msr": 0x00010001})
        trapped = [x for x in execute_cfg(state, decode("0c830000"), ConcreteOps()) if x.condition]
        self.assertEqual(len(trapped), 1)
        trap = trapped[0]
        self.assertEqual((trap.exit_kind, trap.exit_target), ("program-exception", 0x700))
        self.assertEqual(trap.state.srr0, 0)
        self.assertEqual(trap.state.srr1, (state.msr & 0x87C0FFFF) | 0x00020000)
        expected_msr = (((state.msr & ~1) | ((state.msr >> 16) & 1)) & ~0x0004EF36) & 0xFFFFFFFF
        self.assertEqual(trap.state.msr, expected_msr)

        syscall = execute_cfg(state, decode("44000002"), ConcreteOps())[0]
        self.assertEqual((syscall.exit_kind, syscall.exit_target), ("system-call", 0xC00))
        self.assertEqual(syscall.state.srr0, 4)
        self.assertEqual(syscall.state.srr1, state.msr & 0x87C0FFFF)

        rfi_state = concrete_state({"msr": 0x1000, "srr0": 0x80001234, "srr1": 0x87C05678})
        rfi = [x for x in execute_cfg(rfi_state, decode("4c000064"), ConcreteOps()) if x.condition][0]
        self.assertEqual((rfi.exit_kind, rfi.exit_target), ("return-from-interrupt", 0x80001234))
        self.assertEqual(
            rfi.state.msr,
            (((rfi_state.msr & ~0x87C0FFFF) | (rfi_state.srr1 & 0x87C0FFFF)) & 0xFFFBFFFF),
        )

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

    def test_system_cache_and_exception_symbolic_proofs(self) -> None:
        for encoded, observe in (
            ("7c001fec", "memory"), ("7ce000a6", "r7"),
            ("7ce304a6", "r7"), ("7cec42e6", "r7"),
            ("0c830000", "msr,srr0,srr1"),
            ("44000002", "msr,srr0,srr1"),
            ("4c000064", "msr"),
        ):
            with self.subTest(encoded=encoded):
                result = self.check(encoded, encoded, observe)
                self.assertEqual(result.status, ProofStatus.EQUIVALENT)

        difference = self.check("7c001fec", "7c0004ac", "memory")
        self.assertEqual(difference.status, ProofStatus.NOT_EQUIVALENT)

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
        values = parse_observables(["r3,cr0", "xer.ca", "r3", "msr,sr3,time_base,srr0"])
        self.assertEqual(
            [value.name for value in values],
            ["r3", "cr0", "xer.ca", "msr", "sr3", "time_base", "srr0"],
        )
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
        self.assertEqual(names[64:96], tuple(f"f{index}.ps1" for index in range(32)))
        self.assertEqual(names[96:104], tuple(f"gqr{index}" for index in range(8)))
        self.assertEqual(names[104:120], tuple(f"sr{index}" for index in range(16)))
        self.assertEqual(
            names[120:],
            ("cr", "fpscr", "xer.ca", "xer.ov", "xer.so", "lr", "ctr",
             "msr", "time_base", "srr0", "srr1", "memory"),
        )

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

    def test_system_instruction_effects_and_live_out_are_complete(self) -> None:
        cases = (
            (instruction(Opcode.DCBZ, (3, 4)), {"r3", "r4"}, {"memory"}),
            (instruction(Opcode.MFMSR, (5,)), {"msr"}, {"r5"}),
            (instruction(Opcode.MTMSR, (6,)), {"r6"}, {"msr"}),
            (instruction(Opcode.MFSR, (7, 3)), {"sr3"}, {"r7"}),
            (instruction(Opcode.MTSR, (8, 4)), {"r8"}, {"sr4"}),
            (instruction(Opcode.MFTB, (9, 269)), {"time_base"}, {"r9"}),
            (
                instruction(Opcode.TWI, (31, 10, 0), address=0x100),
                {"r10", "msr"}, {"msr", "srr0", "srr1"},
            ),
            (
                instruction(Opcode.SC, (), address=0x104),
                {"msr"}, {"msr", "srr0", "srr1"},
            ),
            (
                instruction(Opcode.RFI, (), address=0x108),
                {"msr", "srr0", "srr1"}, {"msr"},
            ),
        )
        for insn, expected_reads, expected_writes in cases:
            with self.subTest(opcode=insn.opcode):
                reads, writes = register_effects(insn)
                self.assertTrue(expected_reads <= reads)
                self.assertTrue(expected_writes <= writes)
                self.assertTrue(expected_writes <= set(automatic_live_out([insn])))


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

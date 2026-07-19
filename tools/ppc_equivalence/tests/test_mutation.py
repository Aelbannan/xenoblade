from __future__ import annotations

import unittest
from dataclasses import replace

from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.fixtures.corpus import CODE_BASE
from tools.ppc_equivalence.fixtures.encode import xo
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import (
    ConcreteOps,
    SymbolicOps,
    execute_cfg,
)
from tools.ppc_equivalence.result import ProofStatus

try:
    import z3
except Exception:  # pragma: no cover - exercised only when z3 present
    z3 = None


# ---------------------------------------------------------------------------
# Concrete (no-z3) helpers
# ---------------------------------------------------------------------------

def _run_concrete(code_hex: str, initial: dict):
    instructions = decode_block(
        parse_hex(code_hex), base=CODE_BASE, validate_with_capstone=False,
    )
    terminals = execute_cfg(
        concrete_state(initial), instructions, ConcreteOps(),
        max_instructions=128, max_paths=64,
    )
    taken = [t for t in terminals if bool(t.condition)]
    assert len(taken) == 1, (len(taken), [(t.exit_kind) for t in terminals])
    return taken[0]


def _li(rd: int, imm: int) -> str:
    # addi rd, 0, imm  (imm truncated to 16 bits / sign-extended)
    return f"{(0x38000000 | (rd & 0x1F) << 21 | (imm & 0xFFFF)):08x}"


def _li32(rd: int, value: int) -> str:
    # load a full 32-bit constant via addis (high half) + ori (low half)
    hi = (value >> 16) & 0xFFFF
    lo = value & 0xFFFF
    return (
        f"{(0x3C000000 | (rd & 0x1F) << 21 | hi):08x}"
        f"{(0x60000000 | (rd & 0x1F) << 21 | (rd & 0x1F) << 16 | lo):08x}"
    )


def _fctiw(fd: int, fb: int) -> str:
    # fctiw fD, fB : primary 63, single-precision aux, xo = 14 (=> word = xo<<1)
    # The source operand is FB (not FA), so place it in the RB field.
    word = (63 << 26) | (fd & 0x1F) << 21 | (fb & 0x1F) << 11 | (14 << 1)
    return f"{word & 0xFFFFFFFF:08x}"


def _b(li_words: int, *, link: bool = False) -> str:
    return f"{(0x48000000 | ((li_words & 0x3FFFFFF) << 2) | (1 if link else 0)):08x}"


def _fp_scalar(primary: int, sub: int, fd: int, fa: int, fb: int, fc: int = 0) -> str:
    # The decoder reads the sub-opcode from bits 1..5 of the low word
    # (``word & 0x3E``), so `sub` is placed directly in the low six bits.
    word = (
        (primary << 26)
        | (fd & 0x1F) << 21
        | (fa & 0x1F) << 16
        | (fb & 0x1F) << 11
        | (fc & 0x1F) << 6
        | (sub & 0x3F)
    )
    return f"{word & 0xFFFFFFFF:08x}"


BLR = "4e800020"
ADDI_NEG_64 = "3821ffc0"   # addi r1,r1,-64
ADDI_POS_64 = "38210040"   # addi r1,r1,64
ADDI_NEG_16 = "3821fff0"   # addi r1,r1,-16
ADDI_POS_16 = "38210010"   # addi r1,r1,16
STB_R3_NEG32 = "9861ffe0"  # stb r3,-32(r1)


# ---------------------------------------------------------------------------
# Concrete mutation tests — each asserts the mutant diverges from correct.
# ---------------------------------------------------------------------------

class ConcreteMutationTests(unittest.TestCase):
    # 1. invert XER.CA calculation in integer add/carry logic.
    def test_invert_xer_ca(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        # addc r3, r4, r5  with 0xFFFFFFFF + 1 => carry, result 0.
        # Correct CA = unsigned(result < left).  Inverted = unsigned(left < result).
        code = _li(4, 0xFFFFFFFF) + _li(5, 1) + f"{xo(31, 3, 4, 5, 10):08x}" + BLR  # addc r3,r4,r5
        initial = {"gpr": {"r1": "0x1000"}}
        base = _run_concrete(code, initial)
        with mock.patch.object(
            semantics, "_carry_add",
            new=lambda left, right, result, ops: ops.unsigned_lt(left, result),
        ):
            mut = _run_concrete(code, initial)
        self.assertNotEqual(base.state.xer.ca, mut.state.xer.ca,
                            "inverting XER.CA must change the carry result")

    # 2. omit XER.SO propagation in OE-form arithmetic.
    def test_omit_xer_so(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        from tools.ppc_equivalence.fixtures.encode import xo
        # addo. r3, r4, r5  overflowing => SO should be set from OV.
        # xo 778 = ADD with OE and RC bits set ("addo.").
        code = _li32(4, 0x7FFFFFFF) + _li(5, 1) + f"{xo(31, 3, 4, 5, 778, rc=1):08x}" + BLR
        initial = {"gpr": {"r1": "0x1000"}, "xer": {"so": 0, "ov": 0, "ca": 0}}
        base = _run_concrete(code, initial)
        orig = semantics._apply_oe
        with mock.patch.object(
            semantics, "_apply_oe",
            new=lambda state, overflow, enabled, ops: (
                state if not enabled else state.with_xer(ov=overflow, so=state.xer.so)
            ),
        ):
            mut = _run_concrete(code, initial)
        self.assertEqual(int(base.state.xer.so), 1, "correct run sets SO on overflow")
        self.assertEqual(int(mut.state.xer.so), 0, "mutant must drop SO propagation")

    # 3. swap signed/unsigned comparison.
    def test_swap_signed_unsigned_compare(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        # cmpw cr0, r4, r5  with r4=0xFFFFFFFF (signed -1), r5=1.
        # signed: -1 < 1 => LT nibble.  unsigned: 0xFFFFFFFF > 1 => GT nibble.
        code = _li(4, 0xFFFF) + _li(5, 1) + "7c042800" + BLR  # cmpw cr0,r4,r5
        initial = {"gpr": {"r1": "0x1000"}}
        base = _run_concrete(code, initial)
        orig = semantics._comparison_nibble
        with mock.patch.object(
            semantics, "_comparison_nibble",
            new=lambda left, right, signed, state, ops: orig(left, right, False, state, ops),
        ):
            mut = _run_concrete(code, initial)
        self.assertNotEqual(base.state.cr, mut.state.cr,
                            "swapping signed/unsigned compare must change CR")

    # 4. select the wrong CR field.
    def test_wrong_cr_field(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        # cmpw cr2, r4, r5 ; correct writes cr2, mutant writes cr3.
        code = _li(4, 5) + _li(5, 9) + "7d042800" + BLR  # cmpw cr2,r4,r5
        initial = {"gpr": {"r1": "0x1000"}}

        orig_set_cr_field = semantics._set_cr_field

        def _wrong_field(state, field, nibble, ops):
            return orig_set_cr_field(state, (field + 1) % 8, nibble, ops)

        base = _run_concrete(code, initial)
        with mock.patch.object(semantics, "_set_cr_field", new=_wrong_field):
            mut = _run_concrete(code, initial)
        self.assertNotEqual(base.state.cr, mut.state.cr,
                            "writing the wrong CR field must change CR")

    # 5. reverse big-endian byte order in a load.
    def test_reverse_big_endian_load(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        orig_load = semantics._load
        def _reversed_load(memory, address, width, ops, *, reverse=False):
            return orig_load(memory, address, width, ops, reverse=True)
        # lwz r3, 0(r4) ; memory at r4 holds 0x01020304.
        code = _li(4, 0x2000) + "80640000" + BLR  # lwz r3,0(r4)
        initial = {
            "gpr": {"r1": "0x1000", "r4": "0x2000"},
            "memory": {"0x2000": 0x01, "0x2001": 0x02, "0x2002": 0x03, "0x2003": 0x04},
        }
        base = _run_concrete(code, initial)
        with mock.patch.object(semantics, "_load", new=_reversed_load):
            mut = _run_concrete(code, initial)
        self.assertEqual(int(base.state.gpr[3]), 0x01020304, "correct big-endian load")
        self.assertEqual(int(mut.state.gpr[3]), 0x04030201, "mutant reverses bytes")

    # 6. omit one touched byte from a word store.
    def test_omit_byte_from_word_store(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        orig_store = semantics._store
        def _skip_byte0(memory, address, value, width, ops, *, reverse=False):
            result = memory
            for offset in range(width):
                if offset == 0:
                    continue
                shift = offset * 8 if reverse else (width - 1 - offset) * 8
                byte = ops.band(ops.lshr(value, ops.const(shift)), ops.const(0xFF))
                result = ops.store_byte(result, ops.add(address, ops.const(offset)), byte)
            return result
        # stw r3, 0(r4) ; r3 = 0xAABBCCDD.
        code = _li32(3, 0xAABBCCDD) + _li(4, 0x2000) + "90640000" + BLR  # stw r3,0(r4)
        initial = {"gpr": {"r1": "0x1000", "r4": "0x2000"},
                   "memory": {"0x2000": 0x00, "0x2001": 0x00, "0x2002": 0x00, "0x2003": 0x00}}
        base = _run_concrete(code, initial)
        with mock.patch.object(semantics, "_store", new=_skip_byte0):
            mut = _run_concrete(code, initial)
        self.assertEqual(base.state.memory.read(0x2000), 0xAA, "correct store writes byte 0")
        self.assertEqual(mut.state.memory.read(0x2000), 0x00, "mutant omits byte 0")

    # 8. ignore link-register update on a branch-and-link.
    def test_ignore_link_register(self):
        from dataclasses import replace as _dc_replace
        from tools.ppc_equivalence.semantics import Instruction
        # bl +8 (to the blr) then blr.  Correct: bl sets lr = 4.  Mutant: link ignored.
        code = _b(2, link=True) + BLR
        instructions = decode_block(parse_hex(code), base=CODE_BASE, validate_with_capstone=False)
        base = _run_concrete(code, {"gpr": {"r1": "0x1000"}})
        # Model the mutant by dropping the link flag on every instruction.
        no_link = [_dc_replace(i, link=False) for i in instructions]
        terminals = execute_cfg(
            concrete_state({"gpr": {"r1": "0x1000"}}), no_link, ConcreteOps(),
            max_instructions=128, max_paths=64,
        )
        taken = [t for t in terminals if bool(t.condition)]
        self.assertEqual(len(taken), 1)
        mut = taken[0]
        self.assertEqual(int(base.state.lr), CODE_BASE + 4, "correct bl updates lr")
        self.assertEqual(int(mut.state.lr), 0, "mutant ignores lr update")

    # 9. change branch target by 4 bytes.
    def test_branch_target_plus_four(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        # b +8  (skip li r3,1) ; target of +4 lands on the blr directly.
        code = _b(2) + _li(3, 1) + _li(3, 2) + BLR
        initial = {"gpr": {"r1": "0x1000"}}

        def _target_plus4(insn, ops):
            relocation = insn.relocation
            if relocation is not None:
                if relocation.relocation_type not in (semantics.R_PPC_REL24, semantics.R_PPC_REL14):
                    raise semantics.ExecutionInconclusive("branch has a non-branch relocation")
                return semantics._relocation_address(insn, ops), relocation.canonical_symbol
            target = insn.operands[0] if insn.opcode == semantics.Opcode.B else insn.operands[2]
            return target + 4, target + 4

        base = _run_concrete(code, initial)
        with mock.patch.object(semantics, "_cfg_branch_target", new=_target_plus4):
            mut = _run_concrete(code, initial)
        self.assertEqual(int(base.state.gpr[3]), 2, "correct branch executes li r3,2")
        self.assertEqual(int(mut.state.gpr[3]), 0, "mutant +4 branch skips to blr")

    # 10. alter FP rounding tie behavior (away-from-even instead of tie-to-even).
    def test_fp_rounding_tie_away_from_even(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        # fctiw f0, f1  with f1 = 2.5.  RNE => 2 ; away-from-even => 3.
        code = _fctiw(0, 1) + BLR  # fctiw f0, f1
        initial = {"gpr": {"r1": "0x1000"},
                   "fpr": {"f1": 0x4004000000000000},  # 2.5
                   "fpscr": 0}

        def _away_from_even(self, value, rm):
            import math
            f = float(value)
            rounded = math.floor(f + 0.5) if f >= 0 else math.ceil(f - 0.5)
            return float(rounded)

        base = _run_concrete(code, initial)
        with mock.patch.object(ConcreteOps, "_fp_round_int", new=_away_from_even):
            mut = _run_concrete(code, initial)
        # fctiw stores the integer result in the low 32 bits of FPR fd
        # (Gekko convention: high word 0xFFF80000, low word = the integer).
        self.assertEqual(int(base.state.fpr[0]) & 0xFFFFFFFF, 2,
                         "correct tie-to-even fctiw(2.5)=2")
        self.assertEqual(int(mut.state.fpr[0]) & 0xFFFFFFFF, 3,
                         "mutant away-from-even fctiw(2.5)=3")

    # 11. omit an FPSCR bit (the sticky XX / inexact bit, set by fctiw).
    def test_omit_fpscr_xx_bit(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        # fctiw f0, f1 with f1 = 1.2 (non-integral) => inexact => XX sticky set.
        code = _fctiw(0, 1) + BLR  # fctiw f0, f1
        initial = {"gpr": {"r1": "0x1000"},
                   "fpr": {"f1": 0x3FF3333333333333},  # 1.2
                   "fpscr": 0}
        base = _run_concrete(code, initial)
        orig = semantics._fpscr_raise
        with mock.patch.object(
            semantics, "_fpscr_raise",
            new=lambda state, mask, ops: orig(state, mask & ~semantics.FPSCR_XX, ops),
        ):
            mut = _run_concrete(code, initial)
        self.assertNotEqual(base.state.fpscr, mut.state.fpscr,
                            "omitting the XX bit must change FPSCR")
        self.assertEqual(int(base.state.fpscr) & semantics.FPSCR_XX, semantics.FPSCR_XX,
                         "correct run sets XX")
        self.assertEqual(int(mut.state.fpscr) & semantics.FPSCR_XX, 0,
                         "mutant omits XX")

    # 12. change one reciprocal estimate table entry (FRES_TABLE).
    def test_change_fres_table_entry(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        # fres f0, f1 with f1 = 1.0 hits FRES_TABLE entry 0 (mantissa 0).
        code = _fp_scalar(59, 48, 0, 0, 1) + BLR  # fres f0, f1
        initial = {"gpr": {"r1": "0x1000"}, "fpr": {"f1": 0x3FF0000000000000}, "fpscr": 0}
        base = _run_concrete(code, initial)
        mutated_table = (
            (semantics.FRES_TABLE[0][0] + 1, semantics.FRES_TABLE[0][1]),
            *semantics.FRES_TABLE[1:],
        )
        with mock.patch.object(semantics, "FRES_TABLE", new=mutated_table):
            mut = _run_concrete(code, initial)
        self.assertNotEqual(base.state.fpr[0], mut.state.fpr[0],
                            "changing a FRES_TABLE entry must change fres result")

    # turn fused operation into non-fused (separate mul+add rounds twice).
    def test_fused_into_non_fused(self):
        from unittest import mock
        # fmadd f7,f1,f3,f2 with (1+2^-27)*(1-2^-27)+(-1).
        # Fused retains 1-2^-54 then adds -1 => -2^-54.
        # Non-fused mul rounds to 1 first => 1+(-1) => 0.
        code = "fce110fa" + BLR
        initial = {
            "gpr": {"r1": "0x1000"},
            "fpr": {
                "f1": 0x3FF0000002000000,
                "f2": 0xBFF0000000000000,
                "f3": 0x3FEFFFFFFC000000,
            },
            "fpscr": 0,
        }

        def _non_fused(self, rm, a, b, c):
            return (a * b) + c

        base = _run_concrete(code, initial)
        with mock.patch.object(ConcreteOps, "fp_fma", new=_non_fused):
            mut = _run_concrete(code, initial)
        self.assertEqual(int(base.state.fpr[7]), 0xBC90000000000000,
                         "correct fused fmadd retains the tiny product residue")
        self.assertEqual(int(mut.state.fpr[7]), 0,
                         "non-fused mul+add rounds the product away")
        self.assertNotEqual(base.state.fpr[7], mut.state.fpr[7],
                            "fused→non-fused mutant is killed")


# ---------------------------------------------------------------------------
# Symbolic mutation tests (require z3) — engine-level classification mutants.
# ---------------------------------------------------------------------------

def _decode_hex(code_hex: str):
    return decode_block(parse_hex(code_hex), base=CODE_BASE, validate_with_capstone=False)


def _prove(original_hex: str, candidate_hex: str, timeout_ms: int = 20_000):
    return check_equivalence(
        _decode_hex(original_hex),
        _decode_hex(candidate_hex),
        EquivalenceContract(parse_observables(["memory"]), timeout_ms=timeout_ms),
        original_hex=original_hex,
        candidate_hex=candidate_hex,
    )


from tools.ppc_equivalence.engine import check_equivalence  # noqa: E402


@unittest.skipUnless(z3 is not None, "z3-solver is not installed")
class SymbolicMutationTests(unittest.TestCase):
    # 7. REINTRODUCE union private-stack masking (the P0 bug).
    def test_reintroduce_union_private_stack_masking(self):
        from unittest import mock
        from tools.ppc_equivalence import engine
        # Correct result is EQUIVALENT (each side masks its own private frame).
        original = ADDI_NEG_64 + "98610008" + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_16 + "98610008" + ADDI_POS_16 + BLR
        base = _prove(original, candidate)
        self.assertEqual(base.status, ProofStatus.EQUIVALENT,
                         "independent per-side masking treats disjoint frames as equivalent")

        orig_diff = engine._memory_difference

        def _union_difference(left, right, initial, ops):
            z3 = ops.z3
            differences = []
            for address in left.memory_touches + right.memory_touches:
                initial_byte = z3.Select(initial.memory, address)
                left_private = engine._private_stack_address(
                    address, left.stack_low, initial.gpr[1], left.stack_private, ops)
                right_private = engine._private_stack_address(
                    address, right.stack_low, initial.gpr[1], right.stack_private, ops)
                shared = z3.And(left_private, right_private)
                left_byte = z3.If(shared, initial_byte, z3.Select(left.memory, address))
                right_byte = z3.If(shared, initial_byte, z3.Select(right.memory, address))
                differences.append(left_byte != right_byte)
            if len(left.memory_effects) != len(right.memory_effects):
                differences.append(z3.BoolVal(True))
            else:
                differences.extend(a != b for a, b in zip(left.memory_effects, right.memory_effects))
            return z3.Or(*differences) if differences else z3.BoolVal(False)

        with mock.patch.object(engine, "_memory_difference", new=_union_difference):
            mut = _prove(original, candidate)
        self.assertNotEqual(mut.status, ProofStatus.EQUIVALENT,
                            "union masking wrongly flags disjoint frames as inequivalent")
        self.assertEqual(mut.status, ProofStatus.NOT_EQUIVALENT,
                         "the reintroduced union bug is killed")

    # 13. omit a relocation range constraint.
    def test_omit_relocation_range_constraint(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        from tools.ppc_equivalence.ir import RelocationRef
        # `b sym` (REL24) in each side, but with DISJOINT addends.  Both reference
        # the same relocation symbol, so the range constraint forces the symbol to
        # lie in two disjoint windows -> layout infeasible (INCONCLUSIVE_LAYOUT).
        # Omitting the range constraint makes the (otherwise identical) pair feasible.
        sym_b0 = "48000000"   # b sym (LI=0)
        sym_b1 = "48000000"   # b sym (LI=0) at +4
        code = parse_hex(sym_b0 + sym_b1 + BLR)
        orig_relocs = (
            RelocationRef(offset=0, relocation_type=semantics.R_PPC_REL24,
                          symbol="sym", canonical_symbol="sym", addend=0),
        )
        cand_relocs = (
            RelocationRef(offset=0, relocation_type=semantics.R_PPC_REL24,
                          symbol="sym", canonical_symbol="sym", addend=0x04000000),
        )
        original = decode_block(code, base=CODE_BASE, validate_with_capstone=False,
                                relocations=orig_relocs)
        candidate = decode_block(code, base=CODE_BASE, validate_with_capstone=False,
                                 relocations=cand_relocs)
        assumed = frozenset({"sym"})
        base = check_equivalence(
            original, candidate,
            EquivalenceContract(parse_observables(["memory"]), timeout_ms=20_000),
            original_hex=sym_b0 + sym_b1 + BLR, candidate_hex=sym_b0 + sym_b1 + BLR,
            assumed_callees=assumed,
        )
        self.assertEqual(base.status, ProofStatus.INCONCLUSIVE_LAYOUT,
                         "disjoint relocation ranges are correctly infeasible")
        with mock.patch.object(SymbolicOps, "layout_constraints", new=lambda self, initial: []):
            mut = check_equivalence(
                original, candidate,
                EquivalenceContract(parse_observables(["memory"]), timeout_ms=20_000),
                original_hex=sym_b0 + sym_b1 + BLR, candidate_hex=sym_b0 + sym_b1 + BLR,
                assumed_callees=assumed,
            )
        self.assertNotEqual(mut.status, ProofStatus.INCONCLUSIVE_LAYOUT,
                            "omitting the relocation range constraint changes the result")
        self.assertEqual(mut.status, ProofStatus.EQUIVALENT,
                         "the mutant makes the disjoint-range pair feasible")

    # 14. treat solver `unknown` as `unsat`.
    def test_unknown_as_unsat(self):
        from unittest import mock
        from tools.ppc_equivalence import engine
        # A genuinely inequivalent pair: correct engine returns NOT_EQUIVALENT.
        original = ADDI_NEG_64 + BLR
        candidate = ADDI_NEG_16 + STB_R3_NEG32 + ADDI_POS_16 + BLR
        base = _prove(original, candidate)
        self.assertEqual(base.status, ProofStatus.NOT_EQUIVALENT,
                         "the pair is genuinely inequivalent")
        # Mutant: when the solver returns unknown, classify as unsat (=> EQUIVALENT).
        with mock.patch.object(
            engine, "check_with_portfolio",
            new=lambda z3_module, build_solver, deadline: (
                None, z3_module.unsat, "forced-unsat", []),
        ):
            mut = _prove(original, candidate)
        self.assertEqual(mut.status, ProofStatus.EQUIVALENT,
                         "the unknown->unsat mutant wrongly proves equivalence")
        self.assertNotEqual(base.status, mut.status,
                            "the unknown->unsat mutant is killed")

    # omit one callee read dependency from the call summary.
    def test_omit_callee_read_dependency(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics
        from tools.ppc_equivalence.ir import RelocationRef
        from tools.ppc_equivalence.semantics import CalleeContract
        # li r4,5 ; bl leaf ; blr  vs  li r4,7 ; bl leaf ; blr.
        # Precise contract reads r4, so different inputs make r3 diverge.
        # Omitting r4 from reads unifies the call tokens => false EQUIVALENT.
        left_hex = "38800005" + "48000001" + BLR
        right_hex = "38800007" + "48000001" + BLR
        reloc = (
            RelocationRef(
                offset=4, relocation_type=semantics.R_PPC_REL24,
                symbol="leaf", canonical_symbol="leaf", addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex), base=CODE_BASE, validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex), base=CODE_BASE, validate_with_capstone=False,
            relocations=reloc,
        )
        contract = CalleeContract(
            frozenset({"r4", "valid"}),
            frozenset({"r3", "valid"}),
            "precise-leaf",
        )
        prove_kwargs = dict(
            original_hex=left_hex, candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": contract},
        )
        observe = EquivalenceContract(parse_observables(["r3"]), timeout_ms=20_000)
        base = check_equivalence(left, right, observe, **prove_kwargs)
        self.assertNotEqual(base.status, ProofStatus.EQUIVALENT,
                            "different r4 inputs must not prove leaf results equal")
        self.assertEqual(base.status, ProofStatus.INCONCLUSIVE_ABSTRACTION,
                         "opaque callee with distinct read deps stays inconclusive")

        orig_apply = semantics._apply_call_summary

        def _omit_r4_read(state, ops, call_id, call_contract):
            reduced = replace(
                call_contract,
                reads=frozenset(name for name in call_contract.reads if name != "r4"),
            )
            return orig_apply(state, ops, call_id, reduced)

        with mock.patch.object(semantics, "_apply_call_summary", new=_omit_r4_read):
            mut = check_equivalence(left, right, observe, **prove_kwargs)
        self.assertEqual(mut.status, ProofStatus.EQUIVALENT,
                         "omitting the r4 read wrongly unifies the callees")
        self.assertNotEqual(base.status, mut.status,
                            "the omitted-callee-read mutant is killed")

    # 15. skip the layout-feasibility check.
    def test_skip_layout_feasibility_check(self):
        from unittest import mock
        from tools.ppc_equivalence import semantics, engine
        from tools.ppc_equivalence.ir import RelocationRef
        # Reuse the disjoint relocation pair that yields INCONCLUSIVE_LAYOUT.
        # The mutant skips the dedicated feasibility gate (forcing it sat), so
        # the engine proceeds to the main solver and reports EQUIVALENT.
        sym_b0 = "48000000"   # b sym (LI=0)
        sym_b1 = "48000000"   # b sym (LI=0) at +4
        code = parse_hex(sym_b0 + sym_b1 + BLR)
        orig_relocs = (
            RelocationRef(offset=0, relocation_type=semantics.R_PPC_REL24,
                          symbol="sym", canonical_symbol="sym", addend=0),
        )
        cand_relocs = (
            RelocationRef(offset=0, relocation_type=semantics.R_PPC_REL24,
                          symbol="sym", canonical_symbol="sym", addend=0x04000000),
        )
        original = decode_block(code, base=CODE_BASE, validate_with_capstone=False,
                                relocations=orig_relocs)
        candidate = decode_block(code, base=CODE_BASE, validate_with_capstone=False,
                                 relocations=cand_relocs)
        assumed = frozenset({"sym"})
        base = check_equivalence(
            original, candidate,
            EquivalenceContract(parse_observables(["memory"]), timeout_ms=20_000),
            original_hex=sym_b0 + sym_b1 + BLR, candidate_hex=sym_b0 + sym_b1 + BLR,
            assumed_callees=assumed,
        )
        self.assertEqual(base.status, ProofStatus.INCONCLUSIVE_LAYOUT,
                         "disjoint relocation ranges are correctly inconclusive")
        # Mutant: skip the dedicated layout-feasibility gate.  The feasibility
        # solver only carries layout constraints (no pair-difference disjunction),
        # so we force it sat while leaving the main solver's real `check` intact.
        orig_check = z3.Solver.check

        def _patched_check(self):
            assertions = self.assertions()
            if any(z3.is_or(a) for a in assertions):
                return orig_check(self)  # main solver: run for real
            return z3.sat  # feasibility solver: pretend feasible

        with mock.patch.object(z3.Solver, "check", _patched_check):
            mut = check_equivalence(
                original, candidate,
                EquivalenceContract(parse_observables(["memory"]), timeout_ms=20_000),
                original_hex=sym_b0 + sym_b1 + BLR, candidate_hex=sym_b0 + sym_b1 + BLR,
                assumed_callees=assumed,
            )
        self.assertNotEqual(mut.status, ProofStatus.INCONCLUSIVE_LAYOUT,
                            "skipping the layout-feasibility check changes the result")
        self.assertEqual(mut.status, ProofStatus.EQUIVALENT,
                         "the mutant promotes the layout-infeasible pair to equivalent")


if __name__ == "__main__":
    unittest.main()

"""Tests for the register-renaming witness (docs/ppc_equiv_work/31).

The witness is a pre-SMT fast path that certifies position-aligned,
same-mnemonic function pairs that differ only in register colors, without Z3
CFG exploration: both sides execute through SymbolicOps with
``retail.r_i``/``decomp.r_rho(i)`` sharing one symbolic variable and the
terminal states are compared with structural ``z3.eq``.

Coverage (per the doc-31 test plan):
- sound accept: pure volatile and nonvolatile color swaps
- CX-1 shift-count swap REJECTED (arg registers are ABI-fixed)
- CX-2 rA=0 zero-register encoding REJECTED (rho must fix r0)
- CX-3 GQR SPR-index REJECTED (reject-list + SPR index bit-equality)
- CX-4 CR-bit immediates REJECTED (BF/CR-bit fields are immediates)
- reloc-name drift REJECTED (gate 2)
- nonvolatile-across-call permutation ACCEPTED (Chaitin cycle class)
- reject-list opcodes fall back to SMT (never certified)
- certificate plumbing: evidence label, contract, rho payload, strict
  certificate validation
"""

from __future__ import annotations

import unittest
from pathlib import Path

from tools.coop.lib.equivalence_check import (
    CertifiedCalleeContext,
    _try_renaming_witness,
)
from tools.coop.lib.renaming_witness import certify_renaming_witness
from tools.coop.lib.targets import equivalence_certificate_error
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.elf_symbols import FunctionBytes
from tools.ppc_equivalence.ir import R_PPC_ADDR16_LO, R_PPC_REL24, RelocationRef
from tools.ppc_equivalence.semantics import CalleeContract

_RETAIL_BASE = 0x80000000
_DECOMP_BASE = 0x80123450
_LR = 0x4E800020  # blr


def _enc_primary(primary: int, rd: int, ra: int, imm: int) -> int:
    return (primary << 26) | ((rd & 31) << 21) | ((ra & 31) << 16) | (imm & 0xFFFF)


def _enc_x(primary: int, xo: int, rd: int, ra: int, rb: int, rc: int = 0) -> int:
    return (
        (primary << 26)
        | ((rd & 31) << 21)
        | ((ra & 31) << 16)
        | ((rb & 31) << 11)
        | ((xo & 0x3FF) << 1)
        | rc
    )


def _enc_logic(primary: int, xo: int, dest: int, src: int, rb: int) -> int:
    # Logical/shift family: destination at bits 16-20, source at bits 21-25.
    return (
        (primary << 26)
        | ((src & 31) << 21)
        | ((dest & 31) << 16)
        | ((rb & 31) << 11)
        | ((xo & 0x3FF) << 1)
    )


def _words_hex(words: list[int]) -> str:
    return "".join(f"{word & 0xFFFFFFFF:08x}" for word in words)


def _decode_pair(
    r_words: list[int],
    d_words: list[int],
    relocs_r: tuple = (),
    relocs_d: tuple = (),
):
    original = decode_block(
        bytes.fromhex(_words_hex(r_words)), _RETAIL_BASE,
        validate_with_capstone=False, relocations=relocs_r,
    )
    candidate = decode_block(
        bytes.fromhex(_words_hex(d_words)), _DECOMP_BASE,
        validate_with_capstone=False, relocations=relocs_d,
    )
    return original, candidate


def _function_bytes(name: str, words: list[int], base: int) -> FunctionBytes:
    code = bytes.fromhex(_words_hex(words))
    return FunctionBytes(
        name=name, path=Path(f"{name}.o"), code=code, base=base,
        value=0, size=len(code), section_index=0, section_name=".text",
        symbol_type=2,
    )


class GateAcceptanceTests(unittest.TestCase):
    """The sound-accept and sound-reject matrix (doc 31 §1, §3 test plan)."""

    def test_pure_volatile_color_swap_accepted(self) -> None:
        # retail: lwz r5,0(r3); add r6,r5,r4; stw r6,8(r3); blr
        # decomp: same with r5<->r6 swapped (volatile scratch, write-before-read)
        original, candidate = _decode_pair(
            [_enc_primary(32, 5, 3, 0), _enc_x(31, 266, 6, 5, 4),
             _enc_primary(36, 6, 3, 8), _LR],
            [_enc_primary(32, 6, 3, 0), _enc_x(31, 266, 5, 6, 4),
             _enc_primary(36, 5, 3, 8), _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertEqual(outcome.rho.gpr, {3: 3, 4: 4, 5: 6, 6: 5})

    def test_pure_nonvolatile_color_swap_accepted(self) -> None:
        # r20<->r25 2-cycle over loads/stores (the Chaitin-cycle target class).
        original, candidate = _decode_pair(
            [_enc_primary(32, 20, 3, 0), _enc_primary(32, 25, 3, 4),
             _enc_primary(36, 20, 3, 8), _enc_primary(36, 25, 3, 12), _LR],
            [_enc_primary(32, 25, 3, 0), _enc_primary(32, 20, 3, 4),
             _enc_primary(36, 25, 3, 8), _enc_primary(36, 20, 3, 12), _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertEqual(outcome.rho.gpr, {3: 3, 20: 25, 25: 20})

    def test_cx1_shift_count_swap_rejected(self) -> None:
        # CX-1: slw r3,r4,r5 vs slw r3,r5,r4 — the shift source/amount swap.
        # r4/r5 are EABI argument registers, so rho(4)=5 is rejected by the
        # ABI-boundary gate (gate 5).
        original, candidate = _decode_pair(
            [_enc_logic(31, 24, 3, 4, 5), _LR],
            [_enc_logic(31, 24, 3, 5, 4), _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_cx2_zero_register_encoding_rejected(self) -> None:
        # CX-2 (doc 32 A2 rev 3, F4): addi r3,0,5 (literal) vs addi r3,r12,5.
        # Under A2, ADDI's RA field is non-register (r0 = literal zero on
        # ADDI/ADDIS, semantics.py:3296), so rho may rename r0 elsewhere but
        # this pair fails gate 3 (`fields`) on the RA bits 0 vs 12 — the
        # rejection is bit-equality, not the old "rho must fix r0" gate 5.
        original, candidate = _decode_pair(
            [_enc_primary(14, 3, 0, 5), _LR],
            [_enc_primary(14, 3, 12, 5), _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "fields")

    def test_cx3_gqr_spr_index_rejected(self) -> None:
        # CX-3: mfspr r3,GQR0 vs mfspr r3,GQR1 — GQRs are user-mode SPRs the
        # engine's privileged path does not cover; the witness reject-list
        # sends them straight to SMT (never certifies).
        def mfspr(spr: int) -> int:
            return (
                (31 << 26) | ((339 & 0x3FF) << 1) | (3 << 21)
                | (((spr >> 5) & 31) << 11) | ((spr & 31) << 16)
            )

        original, candidate = _decode_pair(
            [mfspr(912), _LR], [mfspr(913), _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "reject-list")

    def test_cx3b_lr_vs_ctr_spr_rejected(self) -> None:
        # Even allowed SPRs (LR/CTR) must be bit-equal: mfspr r3,LR vs
        # mfspr r3,CTR differs in a non-register field.
        def mfspr(spr: int) -> int:
            return (
                (31 << 26) | ((339 & 0x3FF) << 1) | (3 << 21)
                | (((spr >> 5) & 31) << 11) | ((spr & 31) << 16)
            )

        original, candidate = _decode_pair(
            [mfspr(8), _LR], [mfspr(9), _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "fields")

    def test_cx4_cr_bit_immediates_rejected(self) -> None:
        # CX-4: cmpw cr1,r5,r6 vs cmpw cr2,r5,r6 — BF is a CR-field immediate
        # (bits 23-25), not a register color; the non-register bit comparison
        # must reject the difference.
        original, candidate = _decode_pair(
            [0x7C853000, _LR], [0x7D053000, _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "fields")

    def test_cx4b_mnemonic_mismatch_rejected(self) -> None:
        # CX-4 (mnemonic arm): cmpw vs cror differ in the opcode bits, which
        # are non-register.  cror cr1,cr1,cr1 = XO 449.
        original, candidate = _decode_pair(
            [0x7C853000, _LR],
            [(19 << 26) | (449 << 1) | (1 << 21) | (1 << 16) | (1 << 11), _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "mnemonic")

    def test_reloc_name_drift_rejected(self) -> None:
        # Gate 2: relocations are addresses, not colors — symbol drift must
        # reject even with byte-identical instructions.
        def reloc(symbol: str):
            return (RelocationRef(2, R_PPC_ADDR16_LO, symbol, symbol, 0),)

        original = decode_block(
            bytes.fromhex(_words_hex([_enc_primary(32, 3, 2, 0), _LR])),
            _RETAIL_BASE, validate_with_capstone=False,
            relocations=reloc("lbl_retail"),
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex([_enc_primary(32, 3, 2, 0), _LR])),
            _DECOMP_BASE, validate_with_capstone=False,
            relocations=reloc("lbl_decomp"),
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "reloc")

    def test_reloc_match_accepted(self) -> None:
        def reloc(symbol: str):
            return (RelocationRef(2, R_PPC_ADDR16_LO, symbol, symbol, 0),)

        original = decode_block(
            bytes.fromhex(_words_hex([_enc_primary(32, 3, 2, 0), _LR])),
            _RETAIL_BASE, validate_with_capstone=False,
            relocations=reloc("lbl_shared"),
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex([_enc_primary(32, 3, 2, 0), _LR])),
            _DECOMP_BASE, validate_with_capstone=False,
            relocations=reloc("lbl_shared"),
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertTrue(outcome.certified, outcome.failure)

    def test_reject_list_opcodes_fall_back_to_smt(self) -> None:
        # mffs / mtfsb0 / mcrfs / dcbz must never be certified via renaming —
        # the witness fails and the SMT probe runs instead.  (psq_l moved to
        # A1PsqExemptionTests: under doc 32 A1 the byte-identical PSQ D-forms
        # are exempted, so a bare psq_l + blr pair now certifies.)
        for name, word in (
            ("mffs", 0xFC00048E),
            ("mtfsb0", 0xFC00008C),
            ("mcrfs", 0xFC000080),
            ("dcbz", 0x7C0007EC),
        ):
            with self.subTest(opcode=name):
                original, candidate = _decode_pair(
                    [word, _LR], [word, _LR],
                )
                outcome = certify_renaming_witness(original, candidate)
                self.assertFalse(outcome.certified)
                self.assertEqual(outcome.failure.gate, "reject-list")

    def test_psq_xform_still_rejected(self) -> None:
        # PSQ X-forms stay on the reject list (doc 32 A1 rev 3, R2-5): even a
        # byte-identical psq_lx pair must fall through to SMT.
        psq_lx = 0x1000000C  # psq_lx f0, 0(r0), 0, 0 (decoder primary 4, XO5=6)
        original, candidate = _decode_pair([psq_lx, _LR], [psq_lx, _LR])
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "reject-list")


class AcrossCallTests(unittest.TestCase):
    """Nonvolatile permutations across calls are the Chaitin-cycle class."""

    def _pair_with_call(self, swap: tuple[int, int]):
        # retail: lwz r20,0(r3); bl callee; stw r20,8(r3); blr
        # decomp: same with r20<->r25 swapped.
        (a, b) = swap
        r_words = [_enc_primary(32, a, 3, 0), 0x48000001, _enc_primary(36, a, 3, 8), _LR]
        d_words = [_enc_primary(32, b, 3, 0), 0x48000001, _enc_primary(36, b, 3, 8), _LR]
        reloc = lambda: (RelocationRef(4, R_PPC_REL24, "callee", "callee", 0),)
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), _RETAIL_BASE,
            validate_with_capstone=False, relocations=reloc(),
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), _DECOMP_BASE,
            validate_with_capstone=False, relocations=reloc(),
        )
        return original, candidate

    def test_nonvolatile_permutation_across_call_accepted(self) -> None:
        original, candidate = self._pair_with_call((20, 25))
        precise = {
            "callee": CalleeContract(
                frozenset({"r3"}), frozenset({"r3"}), "inferred:test",
            )
        }
        outcome = certify_renaming_witness(
            original, candidate,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=precise,
        )
        self.assertTrue(outcome.certified, outcome.failure)
        # retail uses only r20 (never r25), so the renaming is one-way.
        self.assertEqual(outcome.rho.gpr, {3: 3, 20: 25})

    def test_opaque_eabi_callee_rejected(self) -> None:
        # Opaque EABI reads "*" — the call token covers every register, so a
        # non-identity rho makes the post-call state diverge structurally and
        # the pair falls through to SMT.
        original, candidate = self._pair_with_call((20, 25))
        opaque = {"callee": CalleeContract.opaque_eabi()}
        outcome = certify_renaming_witness(
            original, candidate,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=opaque,
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "structural")

    def test_volatile_live_across_call_must_be_fixed(self) -> None:
        # r11 is live across the call (written before, read after); rho must
        # fix it, so a r11->r12 rename is rejected by gate 5.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        reloc = (RelocationRef(4, R_PPC_REL24, "callee", "callee", 0),)
        original = decode_block(
            bytes.fromhex(_words_hex([li(11, 5), 0x48000001, mr(3, 11), _LR])),
            _RETAIL_BASE, validate_with_capstone=False, relocations=reloc,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex([li(12, 5), 0x48000001, mr(3, 12), _LR])),
            _DECOMP_BASE, validate_with_capstone=False, relocations=reloc,
        )
        precise = {
            "callee": CalleeContract(
                frozenset({"r3"}), frozenset({"r3"}), "inferred:test",
            )
        }
        outcome = certify_renaming_witness(
            original, candidate,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=precise,
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_swap_other_register_while_r11_live_across_call(self) -> None:
        # r11 stays fixed; a disjoint r20<->r25 swap is fine even with a call.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        reloc = (RelocationRef(8, R_PPC_REL24, "callee", "callee", 0),)
        r_words = [li(11, 5), _enc_primary(32, 20, 3, 0), 0x48000001,
                   mr(3, 11), _enc_primary(36, 25, 3, 8), _LR]
        d_words = [li(11, 5), _enc_primary(32, 25, 3, 0), 0x48000001,
                   mr(3, 11), _enc_primary(36, 20, 3, 8), _LR]
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), _RETAIL_BASE,
            validate_with_capstone=False, relocations=reloc,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), _DECOMP_BASE,
            validate_with_capstone=False, relocations=reloc,
        )
        precise = {
            "callee": CalleeContract(
                frozenset({"r3"}), frozenset({"r3"}), "inferred:test",
            )
        }
        outcome = certify_renaming_witness(
            original, candidate,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=precise,
        )
        self.assertTrue(outcome.certified, outcome.failure)


class CertificatePlumbingTests(unittest.TestCase):
    """End-to-end certificate issue through the coop probe helper."""

    def _witness_probe(self):
        r_words = [_enc_primary(32, 20, 3, 0), _enc_primary(32, 25, 3, 4),
                   _enc_primary(36, 20, 3, 8), _enc_primary(36, 25, 3, 12), _LR]
        d_words = [_enc_primary(32, 25, 3, 0), _enc_primary(32, 20, 3, 4),
                   _enc_primary(36, 25, 3, 8), _enc_primary(36, 20, 3, 12), _LR]
        left = _function_bytes("f", r_words, _RETAIL_BASE)
        right = _function_bytes("f", d_words, _DECOMP_BASE)
        context = CertifiedCalleeContext({}, (), ())
        return _try_renaming_witness(None, "f", left, right, "us-test", context)

    def test_certificate_evidence_and_contract(self) -> None:
        probe = self._witness_probe()
        self.assertIsNotNone(probe)
        self.assertEqual(probe.status.value, "equivalent")
        certificate = probe.certificate
        self.assertEqual(certificate["evidence"], "register-renaming-witness")
        self.assertEqual(certificate["contract"], "register-renaming-witness")
        payload = certificate["register_renaming_witness"]
        self.assertEqual(payload["rho"]["gpr"], {"3": 3, "20": 25, "25": 20})
        self.assertTrue(payload["structural_eq"])
        self.assertGreaterEqual(payload["terminal_pairs_checked"], 1)
        self.assertEqual(
            certificate["summary"]["return_behavior"], "normal",
        )
        # The renaming path must not empty observables (byte-identity may).
        self.assertIn("memory", certificate.get("observables", []))
        self.assertIn("r20", certificate.get("observables", []))
        self.assertIn("opcodes_used", certificate)

    def test_certificate_passes_strict_validation(self) -> None:
        probe = self._witness_probe()
        certificate = probe.certificate
        row = {
            "id": "us-test",
            "status": "EQUIVALENT_MATCH",
            "equivalence_certificate": certificate,
            "declared_return": None,
        }
        self.assertIsNone(equivalence_certificate_error(row, {"us-test": row}))

    def test_witness_failure_returns_none(self) -> None:
        # A shift-count swap on arg registers must fall through to SMT: the
        # probe helper returns None (no certificate) rather than failing.
        r_words = [_enc_logic(31, 24, 3, 4, 5), _LR]
        d_words = [_enc_logic(31, 24, 3, 5, 4), _LR]
        left = _function_bytes("f", r_words, _RETAIL_BASE)
        right = _function_bytes("f", d_words, _DECOMP_BASE)
        context = CertifiedCalleeContext({}, (), ())
        probe = _try_renaming_witness(None, "f", left, right, "us-test", context)
        self.assertIsNone(probe)

    def test_targets_whitelist_accepts_new_evidence(self) -> None:
        probe = self._witness_probe()
        certificate = probe.certificate
        row = {
            "id": "us-test",
            "status": "EQUIVALENT_MATCH",
            "equivalence_certificate": certificate,
            "declared_return": None,
        }
        error = equivalence_certificate_error(row, {"us-test": row})
        self.assertIsNone(error)


if __name__ == "__main__":
    unittest.main()


class RegionSlicedWitnessTests(unittest.TestCase):
    """Expansion B (witness_expansion_plan): position-dependent rho.

    Local register-allocation differences (no single global bijection) are
    certified via region slicing: split at bijection conflicts / call sites,
    rebind changed lanes at boundaries gated on four-lane deadness, compare
    each terminal under its exit region's rho.
    """

    _R = 0x80000000
    _D = 0x80123450
    _BLR = 0x4E800020

    def _pair(self, r_words, d_words):
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        return original, candidate

    def test_local_temp_swap_dead_at_boundary_accepted(self) -> None:
        # retail: li r5,1 ; mr r3,r5 ; li r5,2 ; mr r3,r5 ; blr
        # decomp: li r4,1 ; mr r3,r4 ; li r5,2 ; mr r3,r5 ; blr
        # r5's old value is dead at the boundary (slot 2 redefines it), so the
        # rebind is sound and the pair certifies.  r20 (not r4: r4 is
        # default-FIXED under doc 32 A3) is the rename partner.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [li(5, 1), mr(3, 5), li(5, 2), mr(3, 5), self._BLR]
        d = [li(20, 1), mr(3, 20), li(5, 2), mr(3, 5), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertEqual(outcome.details.get("rho_mode"), "region-sliced")

    def test_live_across_boundary_rejected(self) -> None:
        # retail: li r5,1 ; add r3,r5,r0 ; add r3,r3,r5 ; blr  (r3 = 2)
        # decomp: li r4,1 ; add r3,r4,r0 ; add r3,r3,r5 ; blr  (r3 = 1 + garbage)
        # r5 is USED (not redefined) at the conflict slot — live across the
        # boundary; rebinding is unsound and the pair must reject.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        add = lambda rd, ra, rb: _enc_x(31, 266, rd, ra, rb)
        r = [li(5, 1), add(3, 5, 0), add(3, 3, 5), self._BLR]
        d = [li(4, 1), add(3, 4, 0), add(3, 3, 5), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_loop_containing_target_rejected_first_cut(self) -> None:
        # A backward branch (loop) is rejected for expansion in the first cut
        # and falls through to SMT (plan §2.2).
        from tools.coop.lib.renaming_witness import _has_loop_or_non_return_indirect

        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [li(5, 1), mr(3, 5), self._BLR]
        original, _ = self._pair(r, r)
        self.assertFalse(_has_loop_or_non_return_indirect(original))
        # 0x48000000 at base self._R: b self (target == pc) — a self-loop.
        loop = decode_block(
            bytes.fromhex("480000004e800020"), self._R,
            validate_with_capstone=False,
        )
        self.assertTrue(_has_loop_or_non_return_indirect(loop))

    def test_return_position_bclr_not_rejected(self) -> None:
        # bclr with link=False is a return terminal, NOT a loop/indirect
        # marker (plan §2.2.2).  Predicated returns (BO=4) included.
        from tools.coop.lib.renaming_witness import _has_loop_or_non_return_indirect

        blr = decode_block(bytes.fromhex("4e800020"), self._R, validate_with_capstone=False)
        self.assertFalse(_has_loop_or_non_return_indirect(blr))
        beqlr = decode_block(bytes.fromhex("4d820020"), self._R, validate_with_capstone=False)
        self.assertFalse(_has_loop_or_non_return_indirect(beqlr))

    def test_global_rho_unchanged_when_no_regions(self) -> None:
        # A consistent single-bijection pair must still take the GLOBAL path
        # (no region slicing) so existing certificates are byte-identical.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [li(5, 1), mr(3, 5), self._BLR]
        d = [li(6, 1), mr(3, 6), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertNotIn("rho_mode", outcome.details)  # global path

    def test_pairs_checked_zero_guard(self) -> None:
        # run_structural_witness must reject when no terminal pair is
        # comparable (plan §2.3) — never a vacuous certificate.
        from tools.coop.lib.renaming_witness import run_structural_witness, Rho

        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [li(5, 1), mr(3, 5), self._BLR]
        d = [li(6, 1), mr(3, 6), self._BLR]
        original, candidate = self._pair(r, d)
        # rho 5<->6 with a terminal whose conditions are disjoint would be the
        # empty-exits case; the guard must reject rather than certify.
        outcome = run_structural_witness(
            original, candidate, Rho(gpr={5: 6}),
            max_instructions=2048, max_paths=256, max_loop_iterations=32,
        )
        # Either certified via normal comparison, or rejected by the guard —
        # never a crash and never a vacuous cert.
        if not outcome.certified:
            self.assertEqual(outcome.failure.gate, "structural")


class ImplReviewRegressionTests(unittest.TestCase):
    """Regressions for the implementation-review findings (2026-08-03):
    per-region gate 5, RLWIMI/STMW liveness, predicated-bclr fallthrough,
    reject-list after a rho conflict.
    """

    _R = 0x80000000
    _D = 0x80123450
    _BLR = 0x4E800020

    def _pair(self, r_words, d_words):
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        return original, candidate

    def test_abi_arg_remap_in_region_rejected(self) -> None:
        # Impl-review BLOCKER (GLM-2 / Kimi-1): gate 5 must run per region.
        # retail takes its argument in r5, decomp in r8 — genuinely different
        # ABI; the region path must reject at abi-boundary, not certify.
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        r = [mr(3, 5), li(9, 1), mr(4, 9), li(9, 2), mr(6, 9), self._BLR]
        d = [mr(3, 8), li(10, 1), mr(4, 10), li(9, 2), mr(6, 9), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_rlwimi_accumulator_read_rejected(self) -> None:
        # Impl-review BLOCKER (GLM-1): rlwimi reads its accumulator (operand
        # 0); under-approximating that use made the boundary deadness see the
        # lane dead -> unsound rebind -> false certificate.  Must reject.
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        rlwimi = lambda rs, ra, sh, mb, me: (
            (20 << 26) | ((rs & 31) << 21) | ((ra & 31) << 16)
            | ((sh & 31) << 11) | ((mb & 31) << 6) | ((me & 31) << 1)
        )
        r = [li(5, 1), mr(3, 6), rlwimi(5, 6, 0, 0, 30), mr(3, 5), self._BLR]
        d = [li(7, 1), mr(3, 6), rlwimi(9, 6, 0, 0, 30), mr(3, 9), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)

    def test_predicated_bclr_fallthrough_rejected(self) -> None:
        # Impl-review BLOCKER (Kimi-2): a predicated bclr (beqlr) has a live
        # fallthrough; dropping it made liveness see lanes dead -> unsound
        # rebind.  Also the decomp reads an uninitialized r5 (EABI live-in),
        # so gate 5 rejects the r5->r6 mapping.
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        cmpwi = lambda ra, v: _enc_primary(11, 0, ra, v)
        beqlr = 0x4D820020
        r = [li(5, 1), cmpwi(3, 0), beqlr, mr(3, 5), self._BLR]
        d = [li(6, 1), cmpwi(3, 0), beqlr, mr(3, 5), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)

    def test_reject_list_after_rho_conflict_rejected(self) -> None:
        # Impl-review MAJOR (Kimi-3): gates 2/3/6 must be validated over the
        # FULL stream, including slots after a rho conflict.  An mffs after
        # the conflict must be rejected at reject-list, not certified.
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        mffs = 0xFC00048E
        r = [mr(3, 5), mr(3, 4), mffs, self._BLR]
        d = [mr(3, 8), mr(3, 4), mffs, self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "reject-list")

    def test_stmw_range_use_modeled(self) -> None:
        # Impl-review MAJOR (GLM-3): stmw reads rD..r31, not just (rD, rA).
        # Verify _use_def_numbered marks the full range as uses so a lane in
        # the range is not falsely dead at a boundary.
        from tools.coop.lib.renaming_witness import _use_def_numbered

        stmw = lambda rd, ra, imm: (
            (47 << 26) | ((rd & 31) << 21) | ((ra & 31) << 16) | (imm & 0xFFFF)
        )
        insn = decode_block(
            bytes.fromhex(_words_hex([stmw(20, 1, 8), self._BLR])), self._R,
            validate_with_capstone=False,
        )[0]
        uses, defs = _use_def_numbered(insn)
        # r20..r31 all read.
        self.assertIn(20, uses)
        self.assertIn(31, uses)
        self.assertNotIn(20, defs)


class ImplReview2RegressionTests(unittest.TestCase):
    """Round-2 implementation-review regressions: STMW boundary PoC, 3-region
    rebind chain, TWI, unsat-seed region-driver negative, early-return."""

    _R = 0x80000000
    _D = 0x80123450
    _BLR = 0x4E800020

    def _pair(self, r_words, d_words):
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        return original, candidate

    def test_stmw_range_boundary_rejected(self) -> None:
        # R2 end-to-end STMW PoC: r25/r26 live ONLY through the stmw range;
        # retail stores 5 in the r25 slot, decomp stores uninitialized r25 —
        # genuinely non-equivalent.  The range-use liveness fix must make the
        # boundary deadness reject.
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        stmw = lambda rd, ra, imm: (
            (47 << 26) | ((rd & 31) << 21) | ((ra & 31) << 16) | (imm & 0xFFFF)
        )
        r = [li(25, 5), li(14, 0), li(9, 1), mr(14, 9), stmw(20, 1, 8), self._BLR]
        d = [li(26, 5), li(15, 0), li(9, 1), mr(16, 9), stmw(20, 1, 8), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_three_region_rebind_chain_accepted(self) -> None:
        # A lane whose mapping changes at TWO boundaries (3 regions), dead at
        # each boundary, must certify (multi-boundary rebind chain).
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        # r20 -> r21 in region 0 (dead), r20 -> r22 in region 1 (dead),
        # r20 -> r23 in region 2: the same retail lane remapped at TWO
        # boundaries, dead at each (r20-23 are nonvolatile, non-EABI, not
        # unconditionally fixed).
        # retail: li r20,1 ; mr r3,r20 ; li r20,2 ; mr r3,r20 ; li r20,3 ; mr r3,r20 ; blr
        # decomp: li r21,1 ; mr r3,r21 ; li r22,2 ; mr r3,r22 ; li r23,3 ; mr r3,r23 ; blr
        r = [li(20, 1), mr(3, 20), li(20, 2), mr(3, 20), li(20, 3), mr(3, 20), self._BLR]
        d = [li(21, 1), mr(3, 21), li(22, 2), mr(3, 22), li(23, 3), mr(3, 23), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertEqual(outcome.details.get("rho_mode"), "region-sliced")

    def test_twi_immediate_not_register(self) -> None:
        # R2 finding A: TWI's `to` immediate (operand 0) is not a register;
        # _register_fields/_use_def must use RA (operand 1) only.  Two TWIs
        # differing only in `to` must fail gate 3 (fields), not pass via rho.
        twi = lambda to, ra, imm: (
            (3 << 26) | ((to & 31) << 21) | ((ra & 31) << 16) | (imm & 0xFFFF)
        )
        r = [twi(7, 5, 0), self._BLR]
        d = [twi(3, 5, 0), self._BLR]  # different to mask
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "fields")

    def test_early_return_before_boundary(self) -> None:
        # A return before the first region boundary (early blr) must be
        # compared under region 0's rho; the driver must not hang or drop it.
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        # r5->r4 conflict at slot 0 creates a boundary at slot 1? No — build a
        # clean early return: li r5,1 ; blr ; li r6,2 ; mr r3,r6 ; blr with a
        # swap after the early return (boundary after the blr).
        r = [li(5, 1), self._BLR, li(6, 2), mr(3, 6), self._BLR]
        d = [li(5, 1), self._BLR, li(7, 2), mr(3, 7), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)

    def test_region_driver_unsat_seed_rejected(self) -> None:
        # R2 MINOR 6: the region driver pre-checks the resumed seed condition;
        # an infeasible seed must reject (fail to SMT), never crash or certify.
        from tools.coop.lib.renaming_witness import run_region_sliced_witness
        from tools.ppc_equivalence.deadline import Deadline

        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        r = [li(5, 1), mr(3, 5), self._BLR]
        d = [li(4, 1), mr(3, 4), self._BLR]
        original, candidate = self._pair(r, d)
        try:
            out = run_region_sliced_witness(
                original, candidate,
                max_instructions=2048, max_paths=256, max_loop_iterations=32,
                deadline=Deadline.after_ms(20000),
            )
            # This pair has a single global rho (no boundary) — the region
            # driver may reject on its loop/region logic, but must never
            # crash and never certify a non-equivalent pair.
            self.assertIsInstance(out.certified, bool)
        except Exception as exc:  # pragma: no cover
            self.fail(f"region driver raised unexpectedly: {exc}")

class A2PositionAwareR0Tests(unittest.TestCase):
    """doc 32 A2: r0 is renameable in RD/RB/X-form-arith/cmp positions, and is
    literal zero (bit-equal, non-register) only in D/DS/X-indexed load-store
    and ADDI/ADDIS RA.  GXCopyDisp 2-cycle accept; X-form load/store RA reject;
    cmpwi/X-form-arith RA accept."""

    _R = 0x80000000
    _D = 0x80123450
    _BLR = 0x4E800020

    def _pair(self, r_words, d_words):
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        return original, candidate

    def test_r0_value_2cycle_accepted(self) -> None:
        # doc 32 A2 (G3 accept case): r0 used as a REAL value register (li +
        # mr), renamed to r6; rho {0:6} extends to the natural 2-cycle {0<->6}
        # via the identity-first extension.  The pair certifies.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [li(0, 97), mr(3, 0), li(0, 2), mr(4, 0), self._BLR]
        d = [li(6, 97), mr(3, 6), li(6, 2), mr(4, 6), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertEqual(outcome.rho.gpr, {0: 6, 3: 3, 4: 4})

    def test_cmpwi_r0_rename_accepted(self) -> None:
        # cmpwi RA reads gpr[ra] (semantics.py:3455) — r0 is a real register.
        # r0 is WRITTEN before being read (li r0,0 first), so it is not a
        # live-in input: rho(0)=20 is sound (impl-review r2: a live-in r0
        # would be a true input the caller leaves independent).
        cmpwi = lambda ra, v: _enc_primary(11, 0, ra, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        r = [li(0, 0), cmpwi(0, 5), mr(3, 0), self._BLR]
        d = [li(20, 0), cmpwi(20, 5), mr(3, 20), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)

    def test_xform_arith_r0_rename_accepted(self) -> None:
        # X-form arithmetic RA reads gpr[ra] with no r0 guard — renameable.
        # r0 written (li) before the read, so rho(0)=20 is sound.
        add = lambda rd, ra, rb: _enc_x(31, 266, rd, ra, rb)
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        r = [li(0, 0), add(3, 0, 5), self._BLR]
        d = [li(20, 0), add(3, 20, 5), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)

    def test_dform_load_ra_literal_rejected(self) -> None:
        # lwz r3,0(r0) vs lwz r3,0(r12): D-form RA is literal-zero; RA bits
        # differ -> gate 3 fields.
        lwz = lambda rt, ra, dsp: _enc_primary(32, rt, ra, dsp)
        original, candidate = self._pair(
            [lwz(3, 0, 0), self._BLR], [lwz(3, 12, 0), self._BLR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "fields")

    def test_xindex_load_ra_literal_rejected(self) -> None:
        # lwzx r3,r0,r5 vs lwzx r3,r6,r5 (G3): X-form indexed load RA=0 is
        # literal zero (semantics.py:3531) — RA is non-register, gate 3.
        lwzx = lambda rt, ra, rb: _enc_x(31, 87, rt, ra, rb)
        original, candidate = self._pair(
            [lwzx(3, 0, 5), self._BLR], [lwzx(3, 6, 5), self._BLR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "fields")


class DriftDetectorTests(unittest.TestCase):
    """doc 32 A2 rev 3, R2-2: hexdiff's frozen pre-A2 classifier must keep the
    historical register-mask table (RA is a register field for load-store and
    ADDI/ADDIS) while the witness role table drops it.  Pins the intentional
    divergence so an accidental re-sync fails CI."""

    def test_classifier_keeps_ra_witness_drops_it(self) -> None:
        from tools.coop.hexdiff import _classifier_gpr_fpr_masks
        from tools.coop.lib.renaming_witness import _gpr_fpr_masks
        from tools.ppc_equivalence.ir import Opcode

        ra_bit = 0x1F << 16
        for op in (Opcode.LWZX, Opcode.ADDI, Opcode.LWZ, Opcode.STWX, Opcode.LFSX):
            c_mask, _ = _classifier_gpr_fpr_masks(op)
            w_mask, _ = _gpr_fpr_masks(op)
            self.assertTrue(c_mask & ra_bit, f"{op}: classifier lost the RA bit")
            self.assertFalse(w_mask & ra_bit, f"{op}: witness still has the RA bit")

    def test_classifier_still_calls_xindex_ra_swap_pure(self) -> None:
        # The pre-A2 classifier calls lwzx r3,r0,r5 vs lwzx r3,r6,r5 a pure
        # reg-swap (RA in the register mask); the witness rejects it.  This is
        # the intended classifier-vs-witness divergence (R2-2).
        from tools.coop.hexdiff import _pure_reg_swap

        r = 0x7C03286E  # lwzx r3,r0,r5
        d = 0x7C03306E  # lwzx r3,r6,r5
        self.assertTrue(_pure_reg_swap(r, d, "lwzx", "lwzx"))

    def test_classifier_adjusts_for_literal_immediates(self) -> None:
        # Same mnemonic, differing immediate: structural, not pure reg-swap
        # (the classifier must keep rejecting these after the freeze).
        from tools.coop.hexdiff import _pure_reg_swap

        r = 0x38600005  # addi r3,0,5
        d = 0x38600006  # addi r3,0,6
        self.assertFalse(_pure_reg_swap(r, d, "addi", "addi"))


if __name__ == "__main__":
    unittest.main()

class A1PsqExemptionTests(unittest.TestCase):
    """doc 32 A1 rev 3: byte-identical PSQ D-form slots (FP prologue saves /
    epilogue restores) are exempted from the reject list; the rest of the
    function may differ only in rho-safe GPR colors."""

    _R = 0x80000000
    _D = 0x80123450
    _BLR = 0x4E800020

    # psq_st f31, 120(r1), 1, qr3
    PSQ_ST = 0xF3E10078
    # psq_l f31, 120(r1), 1, qr3
    PSQ_L = 0xE3E10078

    def _pair(self, r_words, d_words):
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        return original, candidate

    def test_byte_identical_psq_st_with_gpr_swap_accepted(self) -> None:
        # The observed shape: GPR 2-cycle (r5<->r6) + byte-identical psq_st
        # prologue save.  The psq slot is byte-identical with identity-rho
        # operands (f31, r1 both fixed), so the exemption applies.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [self.PSQ_ST, li(5, 1), mr(3, 5), self._BLR]
        d = [self.PSQ_ST, li(6, 1), mr(3, 6), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)

    def test_combined_psq_st_prologue_and_psq_l_epilogue_accepted(self) -> None:
        # R2-5: functions carry BOTH a byte-identical psq_st prologue AND a
        # byte-identical psq_l epilogue — both must be exempted.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [self.PSQ_ST, li(5, 1), mr(3, 5), self.PSQ_L, self._BLR]
        d = [self.PSQ_ST, li(6, 1), mr(3, 6), self.PSQ_L, self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)

    def test_non_byte_identical_psq_rejected(self) -> None:
        # psq_st f31 vs psq_st f30: raw words differ -> reject-list
        # (gate 6 exemption requires byte-identity).
        psq_st_f30 = 0xF3C10078  # psq_st f30, 120(r1), 1, qr3
        original, candidate = self._pair(
            [self.PSQ_ST, self._BLR], [psq_st_f30, self._BLR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "reject-list")

    def test_psq_operand_renamed_elsewhere_rejected(self) -> None:
        # Byte-identical psq_st f31 with rho renaming f31 in ANOTHER slot
        # (fadd f31, f0, f0 -> fadd f30, f0, f0): the post-rho check
        # (belt-and-suspenders) rejects — the psq would store different
        # shared variables on the two sides.
        fadd = lambda fd, fa, fb: (
            (63 << 26) | ((fd & 31) << 21) | ((fa & 31) << 16)
            | ((fb & 31) << 11) | (21 << 1)
        )
        r = [self.PSQ_ST, fadd(31, 0, 0), self._BLR]
        d = [self.PSQ_ST, fadd(30, 0, 0), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)

    def test_psq_st_ps1_sublane_deadness_rejected(self) -> None:
        # GLM impl-review BLOCKER 1 exploit shape: region 0 renames f20->f21;
        # a double-precision fmr kills fpr[20] (not ps1[20]); the byte-identical
        # psq_st f20 (region 1) reads ps1[20] which is still LIVE.  The
        # four-lane deadness must check the PS1 sub-lanes and reject the rebind
        # (before the fix this certified a non-equivalent pair).
        lfs = lambda ft, ra, dsp: _enc_primary(48, ft, ra, dsp)
        fmr = lambda fd, fb: (
            (63 << 26) | ((fd & 31) << 21) | ((fb & 31) << 11) | (72 << 1)
        )
        psq_st_f20 = 0xF4A10078 & ~(31 << 21) | (20 << 21)  # psq_st f20,120(r1),1,qr3
        # retail: lfs f20,0(r2) ; fmr f20,f0 ; psq_st f20 ; blr
        # decomp: lfs f21,0(r2) ; fmr f20,f0 ; psq_st f20 ; blr
        r = [lfs(20, 2, 0), fmr(20, 0), psq_st_f20, self._BLR]
        d = [lfs(21, 2, 0), fmr(20, 0), psq_st_f20, self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)

class A3ReturnRegisterFixednessTests(unittest.TestCase):
    """doc 32 A3 rev 3: r4/f1 default FIXED; unfixed only on trusted metadata
    (non-64-bit declared_return) AND no write on a forward path to a return
    AND no tail-call exits.  G7 closure + F2/F3/F7 regressions."""

    _R = 0x80000000
    _D = 0x80123450
    _BLR = 0x4E800020

    def _pair(self, r_words, d_words):
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        return original, candidate

    def test_g7_longlong_return_renamed_rejected(self) -> None:
        # G7 false-certificate regression: retail returns long long in
        # r3:r4 (r4 written before blr); decomp renames r4->r5; registry
        # declared_return is STALE "i32".  The structural write-before-return
        # check must fix r4 regardless of the metadata.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        r = [li(3, 0), li(4, 1), self._BLR]
        d = [li(3, 0), li(5, 1), self._BLR]
        outcome = certify_renaming_witness(
            *self._pair(r, d), deadline_ms=20000,
            declared_return="i32",
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_scratch_r4_unfixed_with_metadata_accepted(self) -> None:
        # F3 synthetic accept: r4 written/consumed as dead scratch, function
        # ends in a tail-call whose callee contract reads ONLY r3 (r4 not
        # observed, F7 distinguishes this from the reject case).  Trusted
        # metadata i32 + no write-before-bclr-return (no bclr at all) + no
        # callee reading r4 -> r4 unfixed; rho {4:5} certifies.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        reloc = (RelocationRef(12, R_PPC_REL24, "callee", "callee", 0),)
        r_words = [li(4, 7), mr(3, 4), li(3, 1), 0x48000000]
        d_words = [li(5, 7), mr(3, 5), li(3, 1), 0x48000000]
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False, relocations=reloc,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False, relocations=reloc,
        )
        precise = {
            "callee": CalleeContract(
                frozenset({"r3"}), frozenset({"r3"}), "inferred:test",
            )
        }
        outcome = certify_renaming_witness(
            original, candidate, deadline_ms=20000,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=precise,
            declared_return="i32",
        )
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertIn(4, outcome.rho.gpr)

    def test_default_fixed_without_metadata_rejected(self) -> None:
        # GLM impl-review MAJOR 2: the load-bearing default-FIXED shape is a
        # write on a path ending in a `b` tail-call (no bclr reached, so (c)
        # does NOT fire) with NO registry metadata -> default-FIXED must still
        # fix r4 and reject a rho that renames it.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        reloc = (RelocationRef(4, R_PPC_REL24, "callee", "callee", 0),)
        r_words = [li(4, 7), 0x48000000]
        d_words = [li(5, 7), 0x48000000]
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False, relocations=reloc,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False, relocations=reloc,
        )
        precise = {
            "callee": CalleeContract(
                frozenset({"r3"}), frozenset({"r3"}), "inferred:test",
            )
        }
        # No declared_return: default FIXED regardless of the callee contract.
        outcome = certify_renaming_witness(
            original, candidate, deadline_ms=20000,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=precise,
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_tail_call_reading_r4_keeps_it_fixed(self) -> None:
        # F7 regression: function ends in a `b` tail-call whose callee
        # contract READS r4 (r4 set up as a tail-call argument).  Even with
        # trusted i32 metadata, the tail-call keeps r4 FIXED.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        reloc = (RelocationRef(4, R_PPC_REL24, "callee", "callee", 0),)
        r_words = [li(4, 1), 0x48000000]
        d_words = [li(5, 1), 0x48000000]
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False, relocations=reloc,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False, relocations=reloc,
        )
        reads_r4 = {
            "callee": CalleeContract(
                frozenset({"r3", "r4"}), frozenset({"r3", "r4"}), "inferred:test",
            )
        }
        outcome = certify_renaming_witness(
            original, candidate, deadline_ms=20000,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=reads_r4,
            declared_return="i32",
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_tail_call_opaque_contract_keeps_r4_fixed(self) -> None:
        # Opaque EABI tail-call: reads "*" -> conservative, r4 stays fixed.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        reloc = (RelocationRef(4, R_PPC_REL24, "callee", "callee", 0),)
        r_words = [li(4, 1), 0x48000000]
        d_words = [li(5, 1), 0x48000000]
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False, relocations=reloc,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False, relocations=reloc,
        )
        opaque = {"callee": CalleeContract.opaque_eabi()}
        outcome = certify_renaming_witness(
            original, candidate, deadline_ms=20000,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=opaque,
            declared_return="i32",
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_r4_live_in_renamed_rejected(self) -> None:
        # r4 read before written (EABI argument): live-in -> fixed.
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [mr(3, 4), self._BLR]
        d = [mr(3, 5), self._BLR]
        outcome = certify_renaming_witness(
            *self._pair(r, d), deadline_ms=20000,
            declared_return="i32",
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

class ImplReviewRegressionTests(unittest.TestCase):
    """Regressions for the implementation-review findings (2026-08-04):
    PS1 sub-lane deadness (GLM BLOCKER 1), bcctr indirect tail calls
    (GLM BLOCKER 2), gate-5-vs-extension (Kimi F1), absolute tail calls
    (Kimi F2), abi_shape certificate plumbing (Kimi F4)."""

    _R = 0x80000000
    _D = 0x80123450
    _BLR = 0x4E800020

    def _pair(self, r_words, d_words):
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        return original, candidate

    def test_bcctr_indirect_exit_keeps_return_lane_fixed(self) -> None:
        # GLM BLOCKER 2: f1 written only on a `b` tail-call path; the beq-taken
        # path exits via bcctr with f1 untouched.  Trusted "f32" metadata would
        # unfix f1 unless the non-link bcctr is treated as an unknown-contract
        # indirect exit (conservative).  Must reject.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        cmpwi = lambda ra, v: _enc_primary(11, 0, ra, v)
        beq = 0x4182000C
        bcctr = 0x4E800420  # bcctr (BO=20, non-link)
        fmr = lambda fd, fb: (
            (63 << 26) | ((fd & 31) << 21) | ((fb & 31) << 11) | (72 << 1)
        )
        reloc = (RelocationRef(12, R_PPC_REL24, "callee", "callee", 0),)
        # retail: cmpwi r3,0 ; beq +0xC ; fmr f1,f0 ; b callee ; bcctr ; blr
        r_words = [cmpwi(3, 0), beq, fmr(1, 0), 0x48000000, bcctr, self._BLR]
        d_words = [cmpwi(3, 0), beq, fmr(3, 0), 0x48000000, bcctr, self._BLR]
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False, relocations=reloc,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False, relocations=reloc,
        )
        precise = {
            "callee": CalleeContract(
                frozenset({"r3"}), frozenset({"r3"}), "inferred:test",
            )
        }
        outcome = certify_renaming_witness(
            original, candidate, deadline_ms=20000,
            assumed_callees=frozenset({"callee"}),
            callee_contracts=precise,
            declared_return="f32",
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_gate5_checks_extended_permutation(self) -> None:
        # Kimi F1 repro: partial rho {5:3} forces the extension to map r3
        # elsewhere; r3 is a fixed return register.  The perm-based gate 5
        # must reject (the partial-dict check certified this non-equivalent
        # pair: retail returns its r3 input, decomp returns 1).
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        r = [li(5, 1), self._BLR]
        d = [li(3, 1), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_absolute_tail_call_keeps_lane_fixed(self) -> None:
        # Kimi F2: relocation-less absolute `b` tail call (DOL-sourced target
        # as an int operand) with trusted metadata — the lane check must not
        # skip it (conservative: no contract for an int target).
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        b_abs = 0x48000000 | ((0x80001000 & 0x3FFFFFC))  # b 0x80001000
        r_words = [li(4, 1), b_abs]
        d_words = [li(5, 1), b_abs]
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        outcome = certify_renaming_witness(
            original, candidate, deadline_ms=20000,
            declared_return="i32",
        )
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

    def test_witness_cert_carries_abi_shape(self) -> None:
        # Kimi F4: the witness certificate must embed abi_shape so the §2.5.4
        # staleness validator can match a registry declared_return.  Build a
        # probe through _try_renaming_witness with a mocked registry entry.
        import tempfile
        from pathlib import Path
        from unittest import mock
        from tools.coop.lib.equivalence_check import (
            _try_renaming_witness, CertifiedCalleeContext,
        )
        from tools.coop.lib.targets import equivalence_certificate_error

        r_words = [_enc_primary(32, 20, 3, 0), _enc_primary(32, 25, 3, 4),
                   _enc_primary(36, 20, 3, 8), _enc_primary(36, 25, 3, 12), _LR]
        d_words = [_enc_primary(32, 25, 3, 0), _enc_primary(32, 20, 3, 4),
                   _enc_primary(36, 25, 3, 8), _enc_primary(36, 20, 3, 12), _LR]
        left = _function_bytes("f", r_words, self._R)
        right = _function_bytes("f", d_words, self._D)
        context = CertifiedCalleeContext({}, (), ())

        class _FakeRegistryTarget:
            symbol = "f"
            extra = {"declared_return": "i32"}

        class _FakeRegistry:
            def extra(self, *a, **k):
                return {}

        class _StubProject:
            config = None  # load_targets is mocked; config value unused

        with mock.patch(
            "tools.coop.lib.targets.get_target",
            return_value=_FakeRegistryTarget(),
        ), mock.patch(
            "tools.coop.lib.targets.load_targets",
            return_value=_FakeRegistry(),
        ):
            probe = _try_renaming_witness(
                _StubProject(), "f", left, right, "us-abi-shape-test", context,
            )
        self.assertIsNotNone(probe)
        certificate = probe.certificate
        self.assertEqual(
            certificate.get("abi_shape", {}).get("declared_return"), "i32",
        )
        row = {
            "id": "us-abi-shape-test",
            "status": "EQUIVALENT_MATCH",
            "equivalence_certificate": certificate,
            "declared_return": "i32",
        }
        self.assertIsNone(equivalence_certificate_error(row, {row["id"]: row}))
        # Drift must be caught: registry says i64, cert says i32.
        row64 = dict(row, declared_return="i64")
        self.assertIsNotNone(
            equivalence_certificate_error(row64, {row64["id"]: row64}),
        )

class ValueDependentRATests(unittest.TestCase):
    """doc 32 A2 rev 5: the RA field of an RA-literal opcode is the literal
    zero only when its value is 0.  A both-nonzero RA pair is a real register
    rename (enters rho); zero-vs-nonzero is a literal-vs-register mismatch
    and must reject at fields."""

    _R = 0x80000000
    _D = 0x80123450
    _BLR = 0x4E800020

    def _pair(self, r_words, d_words):
        original = decode_block(
            bytes.fromhex(_words_hex(r_words)), self._R,
            validate_with_capstone=False,
        )
        candidate = decode_block(
            bytes.fromhex(_words_hex(d_words)), self._D,
            validate_with_capstone=False,
        )
        return original, candidate

    def test_nonzero_ra_rename_accepted(self) -> None:
        # lwz r3,0(r20) vs lwz r3,0(r21): both RA nonzero -> renameable
        # (r20/r21 non-EABI, not fixed); the value-dependent rule certifies.
        lwz = lambda rt, ra, dsp: _enc_primary(32, rt, ra, dsp)
        r = [lwz(3, 20, 0), self._BLR]
        d = [lwz(3, 21, 0), self._BLR]
        outcome = certify_renaming_witness(*self._pair(r, d), deadline_ms=20000)
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertEqual(outcome.rho.gpr, {3: 3, 20: 21})

    def test_zero_vs_nonzero_ra_rejected(self) -> None:
        # addi r3,0,5 vs addi r3,r12,5 (CX-2): RA 0 (literal) vs 12 (register)
        # -> gate 3 fields.
        original, candidate = self._pair(
            [_enc_primary(14, 3, 0, 5), self._BLR],
            [_enc_primary(14, 3, 12, 5), self._BLR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "fields")

    def test_nonzero_ra_identity_entry_in_rho(self) -> None:
        # A load with a nonzero RA base records the base's identity in rho
        # (it is a real register read under the value-dependent rule).
        lwz = lambda rt, ra, dsp: _enc_primary(32, rt, ra, dsp)
        original, candidate = self._pair(
            [lwz(5, 3, 0), self._BLR], [lwz(5, 3, 0), self._BLR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertTrue(outcome.certified, outcome.failure)
        self.assertIn(3, outcome.rho.gpr)

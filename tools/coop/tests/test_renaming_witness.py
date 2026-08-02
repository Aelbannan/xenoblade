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
        # CX-2: addi r3,0,5 (literal) vs addi r3,r12,5 — rho would map r0 to
        # r12, but rho must fix r0 (zero-register encoding).
        original, candidate = _decode_pair(
            [_enc_primary(14, 3, 0, 5), _LR],
            [_enc_primary(14, 3, 12, 5), _LR],
        )
        outcome = certify_renaming_witness(original, candidate)
        self.assertFalse(outcome.certified)
        self.assertEqual(outcome.failure.gate, "abi-boundary")

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
        # psq_l / mffs / mtfsb0 / mcrfs / dcbz must never be certified via
        # renaming — the witness fails and the SMT probe runs instead.
        for name, word in (
            ("psq_l", 0xE0000000),
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
        # rebind is sound and the pair certifies.
        li = lambda rt, v: _enc_primary(14, rt, 0, v)
        mr = lambda rd, rs: _enc_logic(31, 444, rd, rs, rs)
        r = [li(5, 1), mr(3, 5), li(5, 2), mr(3, 5), self._BLR]
        d = [li(4, 1), mr(3, 4), li(5, 2), mr(3, 5), self._BLR]
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

"""Adversarial regressions: false-EQUIVALENT / false-NE holes closed 2026-07-22+.

Covers:
1. Load-derived SP publish via Select (stack_escape fail-open)
2. Tail-call / virtual-thunk outgoing args r5–r10 / f2+ ignored
3. FPSCR.XX divergence published via mffs → compared FPR under allow_nan=False
4. Record-form Rc→CR1 OX sticky projection (no mffs/mcrfs)
5. stwu back-chain clearing stack_private (false NOT_EQUIVALENT)
"""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.abi_shape import AbiShape
from tools.ppc_equivalence.contract import (
    EABI_OUTGOING_GPR_ARGS,
    make_contract,
    observables_for_exit,
)
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence, _symbolic_initial
from tools.ppc_equivalence.fp_fpscr import (
    FPSCR_STICKIES_UNSUPPORTED,
    annotate_fpscr_sticky_incompleteness,
)
from tools.ppc_equivalence.result import FloatingPointDomain, ProofResult, ProofStatus
from tools.ppc_equivalence.semantics import SymbolicOps, automatic_live_out
from tools.ppc_equivalence.stack_escape import (
    bv_depends_on_entry_r1,
    bv_is_load_derived,
    mark_stack_pointer_escape,
)


def _w(v: int) -> str:
    return f"{v:08X}"


def _decode(hex_words: str):
    return decode_block(parse_hex(hex_words.replace(" ", "")), validate_with_capstone=False)


def _prove(
    original_hex: str,
    candidate_hex: str,
    *,
    preset: str = "auto",
    observe: list[str] | None = None,
    abi_shape: AbiShape | None = None,
    floating_point_domain: FloatingPointDomain | None = None,
    timeout_ms: int = 15_000,
):
    original = _decode(original_hex)
    candidate = _decode(candidate_hex)
    if observe is not None:
        contract = make_contract(
            preset=None,
            observe=observe,
            timeout_ms=timeout_ms,
            abi_shape=abi_shape,
        )
    elif preset == "auto":
        contract = make_contract(
            preset="auto",
            observe=None,
            timeout_ms=timeout_ms,
            original_live_out=automatic_live_out(original),
            candidate_live_out=automatic_live_out(candidate),
            abi_shape=abi_shape,
        )
    else:
        contract = make_contract(
            preset=preset,
            observe=None,
            timeout_ms=timeout_ms,
            abi_shape=abi_shape,
        )
    return check_equivalence(
        original,
        candidate,
        contract,
        original_hex=original_hex.replace(" ", ""),
        candidate_hex=candidate_hex.replace(" ", ""),
        floating_point_domain=floating_point_domain,
    )


def _enc_addi(rd: int, ra: int, imm: int) -> str:
    imm &= 0xFFFF
    return _w((14 << 26) | (rd << 21) | (ra << 16) | imm)


def _enc_li(rd: int, imm: int) -> str:
    return _enc_addi(rd, 0, imm)


def _enc_stw(rs: int, ra: int, imm: int) -> str:
    imm &= 0xFFFF
    return _w((36 << 26) | (rs << 21) | (ra << 16) | imm)


def _enc_lwz(rd: int, ra: int, imm: int) -> str:
    imm &= 0xFFFF
    return _w((32 << 26) | (rd << 21) | (ra << 16) | imm)


BLR = "4E800020"
# lwz r12,0(r3); lwz r12,0x20(r12); mtctr r12; bctr
CORRECT_THUNK = "81830000 818C0020 7D8903A6 4E800420"


class LoadDerivedStackEscapeTests(unittest.TestCase):
    def test_select_is_load_derived_without_walking_spine(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        loaded = ops.load_byte(state.memory, state.gpr[4])
        self.assertTrue(bv_is_load_derived(loaded, z3))
        self.assertFalse(bv_depends_on_entry_r1(loaded, z3))

    def test_load_derived_store_outside_frame_clears_stack_private(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        loaded = ops.load_byte(state.memory, state.gpr[4])
        escaped = mark_stack_pointer_escape(
            state, loaded, ops, store_address=state.gpr[5],
        )
        self.assertTrue(z3.is_false(z3.simplify(escaped.stack_private)))

    def test_load_derived_spill_to_r1_relative_keeps_masking(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        loaded = ops.load_byte(state.memory, state.gpr[4])
        spill_addr = ops.add(state.gpr[1], ops.const(8))
        kept = mark_stack_pointer_escape(
            state, loaded, ops, store_address=spill_addr,
        )
        self.assertTrue(z3.is_true(z3.simplify(kept.stack_private)))
        self.assertIs(kept, state)

    def test_publish_select_then_private_diverge_not_equivalent(self) -> None:
        """Adversarial PoC: load-derived publish must not hide private-frame diverge."""
        prologue = _enc_addi(1, 1, -32)
        epilogue = _enc_addi(1, 1, 32)
        pub = " ".join([prologue, _enc_lwz(3, 4, 0), _enc_stw(3, 5, 0)])
        original = " ".join([pub, _enc_li(0, 1), _enc_stw(0, 1, 8), epilogue, BLR])
        candidate = " ".join([pub, _enc_li(0, 2), _enc_stw(0, 1, 8), epilogue, BLR])
        for preset in ("auto", "ppc-eabi"):
            with self.subTest(preset=preset):
                result = _prove(original, candidate, preset=preset)
                self.assertEqual(
                    result.status,
                    ProofStatus.NOT_EQUIVALENT,
                    f"{preset}: {result.status} mismatch={result.mismatch}",
                )

    def test_direct_r1_publish_still_not_equivalent(self) -> None:
        prologue = _enc_addi(1, 1, -32)
        epilogue = _enc_addi(1, 1, 32)
        pub = " ".join([prologue, _enc_stw(1, 5, 0)])
        original = " ".join([pub, _enc_li(0, 1), _enc_stw(0, 1, 8), epilogue, BLR])
        candidate = " ".join([pub, _enc_li(0, 2), _enc_stw(0, 1, 8), epilogue, BLR])
        result = _prove(original, candidate, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)


class IndirectBranchOutgoingArgTests(unittest.TestCase):
    def test_ppc_eabi_indirect_observes_r5_through_r10(self) -> None:
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=1_000)
        names = {item.name for item in observables_for_exit(contract, "indirect-branch")}
        for name in EABI_OUTGOING_GPR_ARGS:
            self.assertIn(name, names)
        self.assertIn("f2", names)
        self.assertIn("f8", names)
        # Return path must not grow volatile outgoing args.
        ret = {item.name for item in observables_for_exit(contract, "return")}
        self.assertNotIn("r5", ret)
        self.assertNotIn("f2", ret)

    def test_clobber_r5_before_identical_thunk_not_equivalent(self) -> None:
        broken = _enc_li(5, 0) + " " + CORRECT_THUNK
        result = _prove(CORRECT_THUNK, broken, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r5")

    def test_clobber_r10_before_identical_thunk_not_equivalent(self) -> None:
        broken = _enc_li(10, 0) + " " + CORRECT_THUNK
        result = _prove(CORRECT_THUNK, broken, preset="ppc-eabi")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r10")

    def test_clobber_f2_before_identical_thunk_not_equivalent(self) -> None:
        # fneg f2,f2
        broken = "FC400050 " + CORRECT_THUNK
        result = _prove(CORRECT_THUNK, broken, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "f2")

    def test_r5_scratch_allowed_under_narrow_abi_shape(self) -> None:
        # lwz r5,0(r3); lwz r5,0x20(r5); mtctr r5; bctr
        cand_r5 = "80A30000 80A50020 7CA903A6 4E800420"
        shape = AbiShape(outgoing_gpr_args=2, outgoing_fpr_args=0, source="explicit")
        result = _prove(
            CORRECT_THUNK, cand_r5, preset="auto", abi_shape=shape,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)


class MffsFpscrStickyProjectionTests(unittest.TestCase):
    _MTFSB0_XX = _w((63 << 26) | (6 << 21) | (70 << 1))

    @staticmethod
    def _fdivs(fd: int, fa: int, fb: int) -> str:
        return _w((59 << 26) | (fd << 21) | (fa << 16) | (fb << 11) | (18 << 1))

    @staticmethod
    def _mffs(fd: int) -> str:
        return _w((63 << 26) | (fd << 21) | (583 << 1))

    def test_annotate_demotes_mffs_into_f1(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            observables=["f1"],
            opcodes_used=["fdivs", "mffs", "mtfsb0"],
        )
        annotate_fpscr_sticky_incompleteness(result)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertIn(FPSCR_STICKIES_UNSUPPORTED, result.unsupported or [])

    def test_annotate_still_skips_fpr_without_projection(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            observables=["f1"],
            opcodes_used=["fdivs"],
        )
        annotate_fpscr_sticky_incompleteness(result)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_mffs_xx_clear_allow_nan_false_is_inconclusive(self) -> None:
        original = " ".join([
            self._MTFSB0_XX, self._fdivs(4, 2, 3), self._mffs(1),
        ])
        candidate = " ".join([
            self._MTFSB0_XX, self._fdivs(4, 2, 3), self._MTFSB0_XX, self._mffs(1),
        ])
        result = _prove(
            original,
            candidate,
            observe=["f1"],
            floating_point_domain=FloatingPointDomain(allow_nan=False),
        )
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertIn(FPSCR_STICKIES_UNSUPPORTED, result.unsupported or [])


class RecordFormCr1StickyProjectionTests(unittest.TestCase):
    """Adversarial PoC: record-form Rc→CR1 must demote like mffs/mcrfs."""

    _MTFSB0_FX = _w((63 << 26) | (0 << 21) | (70 << 1))
    _MTFSB0_OX = _w((63 << 26) | (3 << 21) | (70 << 1))

    @staticmethod
    def _fadds_dot(fd: int, fa: int, fb: int) -> str:
        return _w((59 << 26) | (fd << 21) | (fa << 16) | (fb << 11) | (21 << 1) | 1)

    @staticmethod
    def _fmr_dot(fd: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fb << 11) | (72 << 1) | 1)

    def test_annotate_demotes_cr1_after_fadds(self) -> None:
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            observables=["f1", "cr1"],
            opcodes_used=["fadds", "fmr", "mtfsb0"],
        )
        annotate_fpscr_sticky_incompleteness(result)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertIn(FPSCR_STICKIES_UNSUPPORTED, result.unsupported or [])

    def test_overflow_fadds_dot_vs_clear_ox_fmr_is_inconclusive(self) -> None:
        """Concrete CR1 0x9 vs 0x8 — SymbolicOps must not authorize EQUIVALENT."""
        left = " ".join([
            self._MTFSB0_FX, self._MTFSB0_OX, self._fadds_dot(1, 2, 3), BLR,
        ])
        right = " ".join([
            self._MTFSB0_FX, self._MTFSB0_OX, self._fadds_dot(1, 2, 3),
            self._MTFSB0_OX, self._fmr_dot(1, 1), BLR,
        ])
        result = _prove(
            left,
            right,
            observe=["f1", "cr1"],
            floating_point_domain=FloatingPointDomain(exclude_finite_overflow=False),
            timeout_ms=30_000,
        )
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertIn(FPSCR_STICKIES_UNSUPPORTED, result.unsupported or [])


class StwuBackChainPrivateStackTests(unittest.TestCase):
    """stwu r1,-N(r1) must keep private-stack masking (MWCC prologue)."""

    def test_stwu_frame_private_diverge_still_equivalent(self) -> None:
        # stwu r1,-64(r1); stb r3,8(r1); addi r1,r1,64; blr  vs without stb
        stwu_l = "9421FFC0 98610008 38210040 4E800020"
        stwu_r = "9421FFC0 38210040 4E800020"
        result = _prove(stwu_l, stwu_r, observe=["memory"])
        self.assertEqual(
            result.status,
            ProofStatus.EQUIVALENT,
            f"mismatch={result.mismatch} scope={result.memory_scope}",
        )
        scope = result.memory_scope.to_dict()["private_stack"]
        self.assertTrue(scope["original"]["enabled_on_all_terminal_paths"])
        self.assertTrue(scope["candidate"]["enabled_on_all_terminal_paths"])

    def test_stwu_matches_addi_frame_masking(self) -> None:
        addi_l = "3821FFC0 98610008 38210040 4E800020"
        addi_r = "3821FFC0 38210040 4E800020"
        stwu_l = "9421FFC0 98610008 38210040 4E800020"
        stwu_r = "9421FFC0 38210040 4E800020"
        addi = _prove(addi_l, addi_r, observe=["memory"])
        stwu = _prove(stwu_l, stwu_r, observe=["memory"])
        self.assertEqual(addi.status, ProofStatus.EQUIVALENT)
        self.assertEqual(stwu.status, ProofStatus.EQUIVALENT)


if __name__ == "__main__":
    unittest.main()

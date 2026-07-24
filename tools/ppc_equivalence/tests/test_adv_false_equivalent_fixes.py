"""Adversarial regressions: false-EQUIVALENT / false-NE holes closed 2026-07-22+.

Covers:
1. Load-derived SP publish via Select (stack_escape fail-open)
2. Tail-call / virtual-thunk outgoing args r5–r10 / f2+ ignored
3. FPSCR.XX divergence published via mffs → compared FPR under allow_nan=False
4. Record-form Rc→CR1 OX sticky projection (no mffs/mcrfs)
5. stwu back-chain clearing stack_private (false NOT_EQUIVALENT)
6. register_effects(MFCR/CMP*) omitting CR → inferred callee false EQUIVALENT
7. Compared-register SP publish (addi r3,r1,N left live) hiding private diverge
8. ``_apply_call_summary`` omitting CR2–CR4 / whole-``cr`` writes (false EQ /
   false refusal); inference must not force a whole-``cr`` read on field overwrites
9. ``register_effects`` omitting ``xer.ca`` on ``adde``/``addic`` → inferred
   callee false EQUIVALENT when callers diverge only in carry
10. ``register_effects`` omitting ``fpscr`` on ``fadds``/``fadd`` → post-call
    ``mffs`` false EQUIVALENT; ``automatic_live_out`` MFFS/MTFS* ordering
11. ``_apply_call_summary`` ignoring ``lr`` writes / inference dropping ``lr``
    → ``mtlr; blr`` trampoline false EQUIVALENT on exit.target
12. ``_apply_call_summary`` ignoring MSR/GQR/SR/aux-SPR writes / inference
    dropping them → ``mtmsr``/``mtspr``/``mtsr`` + observers false EQUIVALENT
13. ``register_effects`` indexing FA (``a[1]``) instead of FB (``a[2]``) for
    ``fmr``/``fneg``/``fabs``/``fnabs``/``frsp``/``fctiw*`` → inferred callee
    false EQUIVALENT when callers diverge only in FB
14. ``register_effects`` omitting ``xer.so`` on record-form CR0 (``add.`` /
    ``andi.`` / …) → inferred callee false EQUIVALENT when callers diverge
    only in SO; logical immediates missing from ``destination_ops``
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
from tools.ppc_equivalence.ir import RelocationRef
from tools.ppc_equivalence.result import FloatingPointDomain, ProofResult, ProofStatus
from tools.ppc_equivalence.semantics import (
    R_PPC_REL24,
    SymbolicOps,
    automatic_live_out,
    infer_callee_contract,
    register_effects,
)
from tools.ppc_equivalence.stack_escape import (
    apply_compared_register_publish_escape,
    bv_depends_on_entry_r1,
    bv_is_load_derived,
    compared_registers_publish_entry_r1,
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


class CrRegisterEffectsCalleeInferenceTests(unittest.TestCase):
    """register_effects must model CR for MFCR/CMP*/MTCRF (callee inference)."""

    _CODE_BASE = 0x80000000

    @staticmethod
    def _mfcr(rd: int) -> str:
        return _w((31 << 26) | (rd << 21) | (19 << 1))

    @staticmethod
    def _mtcrf(rs: int, fxm: int) -> str:
        return _w((31 << 26) | (rs << 21) | (fxm << 12) | (144 << 1))

    @staticmethod
    def _cmpwi(crfd: int, ra: int, imm: int) -> str:
        imm &= 0xFFFF
        return _w((11 << 26) | (crfd << 23) | (ra << 16) | imm)

    @staticmethod
    def _cmpw(crfd: int, ra: int, rb: int) -> str:
        return _w((31 << 26) | (crfd << 23) | (ra << 16) | (rb << 11) | (0 << 1))

    def test_mfcr_effects_read_cr_not_destination(self) -> None:
        insn = _decode(self._mfcr(3))[0]
        reads, writes = register_effects(insn)
        self.assertEqual(reads, {"cr"})
        self.assertEqual(writes, {"r3"})

    def test_mtcrf_effects_rmw_cr(self) -> None:
        insn = _decode(self._mtcrf(3, 0xFF))[0]
        reads, writes = register_effects(insn)
        self.assertEqual(reads, {"r3", "cr"})
        self.assertEqual(writes, {"cr"})

    def test_cmpwi_effects_write_cr_field_not_gpr_from_imm(self) -> None:
        # Operands (2, 3, 0): field=2, rA=3, SIMM=0 — must not treat 0/2 as GPRs.
        insn = _decode(self._cmpwi(2, 3, 0))[0]
        reads, writes = register_effects(insn)
        self.assertEqual(reads, {"r3", "xer.so"})
        self.assertEqual(writes, {"cr2"})
        self.assertNotIn("r0", reads)
        self.assertNotIn("r2", reads)

    def test_cmpw_effects_write_cr_field(self) -> None:
        insn = _decode(self._cmpw(2, 3, 4))[0]
        reads, writes = register_effects(insn)
        self.assertEqual(reads, {"r3", "r4", "xer.so"})
        self.assertEqual(writes, {"cr2"})

    def test_infer_mfcr_leaf_reads_cr(self) -> None:
        leaf = _decode(self._mfcr(3) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("cr", contract.reads)
        self.assertIn("r3", contract.writes)
        self.assertNotIn("r3", contract.reads)

    def test_infer_cmpwi_cr2_leaf_writes_cr2(self) -> None:
        leaf = _decode(self._cmpwi(2, 4, 0) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("cr2", contract.writes)
        self.assertIn("r4", contract.reads)

    def test_mfcr_leaf_summarized_callers_not_false_equivalent(self) -> None:
        """Callers that diverge only in CR2 before mfcr leaf must not prove EQ.

        Historical hole: register_effects(MFCR) reported reads={r3}, so inference
        omitted CR from the call token and unified r3 across CR-diverging callers.
        """
        leaf_hex = (self._mfcr(3) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("cr", inferred.reads)

        left_hex = (_enc_li(5, 0) + self._cmpwi(2, 5, 0) + "48000001" + BLR).replace(" ", "")
        right_hex = (_enc_li(5, 1) + self._cmpwi(2, 5, 0) + "48000001" + BLR).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=8,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via mfcr inference: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )
        # Opaque UF with a correct CR dependency is inconclusive, not a silent EQ.
        self.assertEqual(summarized.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)

        # Same bodies inlined (no summary) must be a concrete NOT_EQUIVALENT.
        oh = (_enc_li(5, 0) + self._cmpwi(2, 5, 0) + self._mfcr(3) + BLR).replace(" ", "")
        ch = (_enc_li(5, 1) + self._cmpwi(2, 5, 0) + self._mfcr(3) + BLR).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            make_contract(preset=None, observe=["r3"], timeout_ms=15_000),
            original_hex=oh,
            candidate_hex=ch,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "r3")

    def test_cmpwi_cr2_leaf_summarized_callers_not_false_equivalent(self) -> None:
        """Leaves that write CR2 must expose that write in inferred summaries."""
        leaf_hex = (self._cmpwi(2, 4, 0) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("cr2", inferred.writes)

        left_hex = (_enc_li(4, 0) + "48000001" + BLR).replace(" ", "")
        right_hex = (_enc_li(4, 1) + "48000001" + BLR).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["cr2"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via cmpwi cr2 inference: {summarized.status} "
            f"contract={inferred}",
        )

    def test_apply_call_summary_refreshes_declared_cr2(self) -> None:
        """PoC: ``writes={cr2}`` must not preserve entry CR2 across the summary.

        Pre-fix ``_apply_call_summary`` only masked volatile CR fields into the
        UF result, so a ``cmpwi cr2`` leaf left entry CR2 untouched. Callers that
        differ only in the leaf's CR2 input then proved EQUIVALENT under
        ``observe=[cr2]`` when ``valid`` was omitted from the write set (and
        refused EQUIVALENT when entry CR2 differed but the leaf overwrote it).
        """
        from dataclasses import replace

        from tools.ppc_equivalence.semantics import (
            CalleeContract,
            SymbolicOps,
            _apply_call_summary,
        )

        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        entry_cr = ops.const(0x00F00000)  # CR2 = 0xF
        state = replace(
            state,
            gpr=tuple(
                ops.const(0) if index == 4 else state.gpr[index]
                for index in range(32)
            ),
            cr=entry_cr,
        )
        contract = CalleeContract(
            frozenset({"r4", "xer.so"}),
            frozenset({"cr2"}),
            "matched-body-effects",
        )
        after = _apply_call_summary(state, ops, "leaf", contract)
        cr2 = z3.simplify(z3.Extract(23, 20, after.cr))
        self.assertFalse(
            z3.is_true(z3.simplify(cr2 == z3.BitVecVal(15, 4))),
            f"declared cr2 write left entry CR2 intact: {cr2}",
        )

    def test_cr2_write_summary_divergent_inputs_not_equivalent(self) -> None:
        """Divergent leaf CR2 inputs must not prove EQUIVALENT under observe=cr2.

        Uses a body-shaped contract that writes ``cr2`` without ``valid`` so the
        unconstrained definedness UF cannot mask the hole as INCONCLUSIVE.
        """
        from tools.ppc_equivalence.semantics import CalleeContract

        leaf_hex = (self._cmpwi(2, 4, 0) + BLR).replace(" ", "")
        left_hex = (_enc_li(4, 0) + "48000001" + BLR).replace(" ", "")
        right_hex = (_enc_li(4, 1) + "48000001" + BLR).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        # Ground truth: inlined bodies differ in CR2.
        oh = (_enc_li(4, 0) + self._cmpwi(2, 4, 0) + BLR).replace(" ", "")
        ch = (_enc_li(4, 1) + self._cmpwi(2, 4, 0) + BLR).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            make_contract(preset=None, observe=["cr2"], timeout_ms=15_000),
            original_hex=oh,
            candidate_hex=ch,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "cr2")

        contract = CalleeContract(
            frozenset({"r4", "xer.so", "cr"}),
            frozenset({"cr2"}),
            "matched-body-effects",
        )
        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["cr2"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": contract},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT: cr2 write not applied in summary "
            f"status={summarized.status} mismatch={summarized.mismatch}",
        )

    def test_cr2_overwrite_summary_ignores_divergent_entry_cr2(self) -> None:
        """Leaf overwrite of CR2 must not refuse EQUIVALENT on entry CR2 diverge."""
        leaf_hex = (self._cmpwi(2, 4, 0) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        # Different entry CR2 via r5, identical leaf input r4=0.
        left_hex = (
            _enc_li(5, 0) + self._cmpwi(2, 5, 0) + _enc_li(4, 0) + "48000001" + BLR
        ).replace(" ", "")
        right_hex = (
            _enc_li(5, 1) + self._cmpwi(2, 5, 0) + _enc_li(4, 0) + "48000001" + BLR
        ).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=12,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        oh = (
            _enc_li(5, 0) + self._cmpwi(2, 5, 0) + _enc_li(4, 0)
            + self._cmpwi(2, 4, 0) + BLR
        ).replace(" ", "")
        ch = (
            _enc_li(5, 1) + self._cmpwi(2, 5, 0) + _enc_li(4, 0)
            + self._cmpwi(2, 4, 0) + BLR
        ).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            make_contract(preset=None, observe=["cr2"], timeout_ms=15_000),
            original_hex=oh,
            candidate_hex=ch,
        )
        self.assertEqual(inlined.status, ProofStatus.EQUIVALENT)

        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["cr2"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false refusal: entry CR2 leaked past cr2-writing summary "
            f"status={summarized.status} mismatch={summarized.mismatch}",
        )

    def test_mtcrf_whole_cr_summary_refreshes_cr2(self) -> None:
        """``writes={cr}`` (mtcrf) must refresh CR2–CR4, not only volatile fields."""
        from dataclasses import replace

        from tools.ppc_equivalence.semantics import (
            CalleeContract,
            SymbolicOps,
            _apply_call_summary,
        )

        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        state = replace(
            state,
            gpr=tuple(
                ops.const(0x12345678) if index == 3 else state.gpr[index]
                for index in range(32)
            ),
            cr=ops.const(0x00F00000),
        )
        contract = CalleeContract(
            frozenset({"r3", "cr"}),
            frozenset({"cr"}),
            "matched-body-effects",
        )
        after = _apply_call_summary(state, ops, "leaf", contract)
        cr2 = z3.simplify(z3.Extract(23, 20, after.cr))
        self.assertFalse(
            z3.is_true(z3.simplify(cr2 == z3.BitVecVal(15, 4))),
            f"whole-cr write left entry CR2 intact: {cr2}",
        )


class XerCaRegisterEffectsCalleeInferenceTests(unittest.TestCase):
    """Carry arithmetic must expose xer.ca in register_effects / inference.

    Adversarial PoC (2026-07-24): ``adde`` omitted ``xer.ca`` from the call
    token, so callers that differed only in CA before ``bl leaf`` proved
    EQUIVALENT on the summarized ``r3`` result under production inference.
    """

    _CODE_BASE = 0x80000000

    @staticmethod
    def _addic(rd: int, ra: int, imm: int) -> str:
        imm &= 0xFFFF
        return _w((12 << 26) | (rd << 21) | (ra << 16) | imm)

    @staticmethod
    def _addic_dot(rd: int, ra: int, imm: int) -> str:
        imm &= 0xFFFF
        return _w((13 << 26) | (rd << 21) | (ra << 16) | imm)

    @staticmethod
    def _adde(rd: int, ra: int, rb: int) -> str:
        return _w((31 << 26) | (rd << 21) | (ra << 16) | (rb << 11) | (138 << 1))

    def test_adde_effects_read_and_write_xer_ca(self) -> None:
        insn = _decode(self._adde(3, 4, 5))[0]
        reads, writes = register_effects(insn)
        self.assertIn("xer.ca", reads)
        self.assertIn("xer.ca", writes)

    def test_addic_effects_write_xer_ca(self) -> None:
        insn = _decode(self._addic(3, 4, 1))[0]
        _reads, writes = register_effects(insn)
        self.assertIn("xer.ca", writes)

    def test_addic_dot_effects_write_xer_ca_and_cr0(self) -> None:
        insn = _decode(self._addic_dot(3, 4, 1))[0]
        _reads, writes = register_effects(insn)
        self.assertIn("xer.ca", writes)
        self.assertIn("cr0", writes)

    def test_infer_adde_leaf_reads_xer_ca(self) -> None:
        leaf = _decode(self._adde(3, 4, 5) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("xer.ca", contract.reads)
        self.assertIn("xer.ca", contract.writes)

    def test_adde_leaf_summarized_ca_diverge_not_false_equivalent(self) -> None:
        """Callers that differ only in CA before adde leaf must not prove EQ."""
        leaf_hex = (self._adde(3, 4, 5) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("xer.ca", inferred.reads)

        # Set CA via addic, then call leaf with r4=r5=0 → r3 should equal CA.
        left_hex = (
            _enc_li(6, -1) + self._addic(0, 6, 1) + _enc_li(4, 0) + _enc_li(5, 0)
            + "48000001" + BLR
        ).replace(" ", "")
        right_hex = (
            _enc_li(6, 0) + self._addic(0, 6, 1) + _enc_li(4, 0) + _enc_li(5, 0)
            + "48000001" + BLR
        ).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=16,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )

        oh = (
            _enc_li(6, -1) + self._addic(0, 6, 1) + _enc_li(4, 0) + _enc_li(5, 0)
            + self._adde(3, 4, 5) + BLR
        ).replace(" ", "")
        ch = (
            _enc_li(6, 0) + self._addic(0, 6, 1) + _enc_li(4, 0) + _enc_li(5, 0)
            + self._adde(3, 4, 5) + BLR
        ).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            make_contract(preset=None, observe=["r3"], timeout_ms=15_000),
            original_hex=oh,
            candidate_hex=ch,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "r3")

        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via adde xer.ca omission: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )

    def test_addic_dot_leaf_then_adde_not_false_equivalent(self) -> None:
        """Leaf that writes CA must refresh xer.ca so a later adde can diverge."""
        leaf_hex = (self._addic_dot(3, 4, 1) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("xer.ca", inferred.writes)

        left_hex = (
            _enc_li(4, -1) + "48000001" + _enc_li(5, 0) + self._adde(3, 5, 5) + BLR
        ).replace(" ", "")
        right_hex = (
            _enc_li(4, 0) + "48000001" + _enc_li(5, 0) + self._adde(3, 5, 5) + BLR
        ).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via addic. xer.ca write omission: "
            f"{summarized.status} contract={inferred}",
        )


class RecordFormCr0XerSoCalleeInferenceTests(unittest.TestCase):
    """Record-form CR0 must read xer.so in register_effects / inference.

    Adversarial PoC (2026-07-24): ``_record_cr0`` embeds ``state.xer.so`` into
    CR0, but ``register_effects`` omitted that read for ``Rc`` / ``andi.`` /
    ``addic.``. Callers of ``add. r3,r4,r5; blr`` that only diverged in SO
    proved EQUIVALENT on summarized ``cr0`` while inlined bodies differed.
    """

    _CODE_BASE = 0x80000000

    @staticmethod
    def _add_dot(rd: int, ra: int, rb: int) -> str:
        return _w((31 << 26) | (rd << 21) | (ra << 16) | (rb << 11) | (266 << 1) | 1)

    @staticmethod
    def _andi_dot(ra: int, rs: int, imm: int) -> str:
        return _w((28 << 26) | (rs << 21) | (ra << 16) | (imm & 0xFFFF))

    @staticmethod
    def _ori(ra: int, rs: int, imm: int) -> str:
        return _w((24 << 26) | (rs << 21) | (ra << 16) | (imm & 0xFFFF))

    @staticmethod
    def _addis(rd: int, ra: int, imm: int) -> str:
        return _w((15 << 26) | (rd << 21) | (ra << 16) | (imm & 0xFFFF))

    @staticmethod
    def _mtspr(reg: int, spr: int) -> str:
        spr_low = spr & 0x1F
        spr_high = (spr >> 5) & 0x1F
        return _w(
            (31 << 26)
            | (reg << 21)
            | (spr_low << 16)
            | (spr_high << 11)
            | (467 << 1)
        )

    def test_add_dot_effects_read_xer_so(self) -> None:
        insn = _decode(self._add_dot(3, 4, 5))[0]
        reads, writes = register_effects(insn)
        self.assertIn("xer.so", reads)
        self.assertIn("cr0", writes)
        self.assertIn("r3", writes)

    def test_andi_dot_effects_read_xer_so_and_write_dest(self) -> None:
        insn = _decode(self._andi_dot(3, 4, 1))[0]
        reads, writes = register_effects(insn)
        self.assertIn("xer.so", reads)
        self.assertIn("cr0", writes)
        self.assertIn("r3", writes)

    def test_ori_effects_write_destination(self) -> None:
        insn = _decode(self._ori(3, 4, 1))[0]
        _reads, writes = register_effects(insn)
        self.assertIn("r3", writes)

    def test_infer_add_dot_leaf_reads_xer_so(self) -> None:
        leaf = _decode(self._add_dot(3, 4, 5) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("xer.so", contract.reads)
        self.assertIn("cr0", contract.writes)

    def test_add_dot_leaf_so_diverge_not_false_equivalent(self) -> None:
        from tools.ppc_equivalence.semantics import CalleeContract

        leaf_hex = (self._add_dot(3, 4, 5) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("xer.so", inferred.reads)

        # mtxer SO=1 vs SO=0, then identical add. via summarized leaf.
        left_hex = (
            self._addis(6, 0, 0x8000)
            + self._mtspr(6, 1)
            + _enc_li(4, 1)
            + _enc_li(5, 2)
            + "48000001"
            + BLR
        ).replace(" ", "")
        right_hex = (
            self._addis(6, 0, 0)
            + self._mtspr(6, 1)
            + _enc_li(4, 1)
            + _enc_li(5, 2)
            + "48000001"
            + BLR
        ).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=16,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        contract = make_contract(preset=None, observe=["cr0"], timeout_ms=15_000)

        oh = (
            self._addis(6, 0, 0x8000)
            + self._mtspr(6, 1)
            + _enc_li(4, 1)
            + _enc_li(5, 2)
            + self._add_dot(3, 4, 5)
            + BLR
        ).replace(" ", "")
        ch = (
            self._addis(6, 0, 0)
            + self._mtspr(6, 1)
            + _enc_li(4, 1)
            + _enc_li(5, 2)
            + self._add_dot(3, 4, 5)
            + BLR
        ).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            contract,
            original_hex=oh,
            candidate_hex=ch,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "cr0")

        buggy_reads = frozenset(r for r in inferred.reads if r != "xer.so")
        buggy = CalleeContract(
            buggy_reads, inferred.writes, "matched-body-effects",
        )
        buggy_result = check_equivalence(
            left,
            right,
            contract,
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": buggy},
        )
        self.assertEqual(
            buggy_result.status,
            ProofStatus.EQUIVALENT,
            "control: xer.so-omitting summary must still demonstrate the hole",
        )

        summarized = check_equivalence(
            left,
            right,
            contract,
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via record-form xer.so omission: "
            f"{summarized.status} mismatch={summarized.mismatch} "
            f"contract={inferred}",
        )


class FpscrRegisterEffectsCalleeInferenceTests(unittest.TestCase):
    """FP arithmetic must expose fpscr in register_effects / inference.

    Adversarial PoC (2026-07-24): ``fadds`` omitted ``fpscr`` from summary
    writes, so callers that diverged only in FP inputs proved EQUIVALENT on a
    post-call ``mffs`` into ``f1`` when ``valid`` was stripped from the write
    set. Production inference now includes ``fpscr``; the no-``valid`` contract
    documents the historical hole.
    """

    _CODE_BASE = 0x80000000

    @staticmethod
    def _fadds(fd: int, fa: int, fb: int) -> str:
        return _w((59 << 26) | (fd << 21) | (fa << 16) | (fb << 11) | (21 << 1))

    @staticmethod
    def _fadd(fd: int, fa: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fa << 16) | (fb << 11) | (21 << 1))

    @staticmethod
    def _mffs(fd: int) -> str:
        return _w((63 << 26) | (fd << 21) | (583 << 1))

    @staticmethod
    def _fmr(fd: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fb << 11) | (72 << 1))

    @staticmethod
    def _fneg(fd: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fb << 11) | (40 << 1))

    @staticmethod
    def _mtfsb0(bt: int) -> str:
        return _w((63 << 26) | (bt << 21) | (70 << 1))

    @staticmethod
    def _frsp(fd: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fb << 11) | (12 << 1))

    def test_fadds_effects_read_and_write_fpscr(self) -> None:
        insn = _decode(self._fadds(4, 2, 3))[0]
        reads, writes = register_effects(insn)
        self.assertIn("fpscr", reads)
        self.assertIn("fpscr", writes)
        self.assertIn("f4", writes)
        self.assertIn("f4.ps1", writes)

    def test_fadd_effects_write_fpscr(self) -> None:
        insn = _decode(self._fadd(1, 2, 3))[0]
        reads, writes = register_effects(insn)
        self.assertIn("fpscr", reads)
        self.assertIn("fpscr", writes)

    def test_frsp_effects_write_ps1_and_fpscr(self) -> None:
        insn = _decode(self._frsp(1, 2))[0]
        _reads, writes = register_effects(insn)
        self.assertIn("f1.ps1", writes)
        self.assertIn("fpscr", writes)

    def test_mffs_live_out_does_not_write_fpscr(self) -> None:
        insn = _decode(self._mffs(1))[0]
        self.assertEqual(list(automatic_live_out([insn])), ["f1"])
        reads, writes = register_effects(insn)
        self.assertIn("fpscr", reads)
        self.assertNotIn("fpscr", writes)

    def test_mtfsb0_live_out_does_not_write_f0(self) -> None:
        insn = _decode(self._mtfsb0(0))[0]
        live = set(automatic_live_out([insn]))
        self.assertEqual(live, {"fpscr"})
        self.assertNotIn("f0", live)

    def test_infer_fadds_leaf_writes_fpscr(self) -> None:
        leaf = _decode(self._fadds(4, 2, 3) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("fpscr", contract.writes)
        self.assertIn("fpscr", contract.reads)

    def test_fadds_leaf_mffs_observer_not_false_equivalent(self) -> None:
        from tools.ppc_equivalence.semantics import CalleeContract

        leaf_hex = (self._fadds(4, 2, 3) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("fpscr", inferred.writes)

        left_hex = (
            self._fmr(2, 5) + "48000001" + self._mffs(1) + BLR
        ).replace(" ", "")
        right_hex = (
            self._fneg(2, 5) + "48000001" + self._mffs(1) + BLR
        ).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        domain = FloatingPointDomain(allow_nan=True)

        oh = (
            self._fmr(2, 5) + self._fadds(4, 2, 3) + self._mffs(1) + BLR
        ).replace(" ", "")
        ch = (
            self._fneg(2, 5) + self._fadds(4, 2, 3) + self._mffs(1) + BLR
        ).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            make_contract(preset=None, observe=["f1"], timeout_ms=20_000),
            original_hex=oh,
            candidate_hex=ch,
            floating_point_domain=domain,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "f1")

        # Historical hole: omit fpscr (and valid) → silent EQUIVALENT on mffs.
        buggy = CalleeContract(
            frozenset({"f0", "f2", "f3"}),
            frozenset({"f4", "f4.ps1"}),
            "matched-body-effects",
        )
        buggy_result = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["f1"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": buggy},
            floating_point_domain=domain,
        )
        self.assertEqual(
            buggy_result.status,
            ProofStatus.EQUIVALENT,
            "control: fpscr-omitting contract must still demonstrate the hole",
        )

        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["f1"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
            floating_point_domain=domain,
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via fadds fpscr omission: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )


class FmrFbRegisterEffectsCalleeInferenceTests(unittest.TestCase):
    """FMR/FNEG/… register_effects must read FB, not reserved FA.

    Adversarial PoC (2026-07-24): decoder emits ``(fd, fa, fb, fc)`` with
    ``fa==0`` for ``fmr``/``fneg``/``fabs``/``fnabs``/``frsp``/``fctiw*``, but
    ``register_effects`` indexed ``a[1]`` (FA). Inference keyed the call token
    on ``f0`` and omitted the real source, so callers of ``fmr f3,f4; blr`` that
    only diverged in ``f4`` proved EQUIVALENT on ``f3`` while inlined bodies
    did not.
    """

    _CODE_BASE = 0x80000000

    @staticmethod
    def _fmr(fd: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fb << 11) | (72 << 1))

    @staticmethod
    def _fneg(fd: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fb << 11) | (40 << 1))

    @staticmethod
    def _fabs(fd: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fb << 11) | (264 << 1))

    @staticmethod
    def _frsp(fd: int, fb: int) -> str:
        return _w((63 << 26) | (fd << 21) | (fb << 11) | (12 << 1))

    def test_fmr_effects_read_fb_not_fa(self) -> None:
        insn = _decode(self._fmr(3, 4))[0]
        self.assertEqual(insn.operands[:3], (3, 0, 4))
        reads, writes = register_effects(insn)
        self.assertEqual(reads, {"f4"})
        self.assertEqual(writes, {"f3"})

    def test_fneg_fabs_frsp_effects_read_fb(self) -> None:
        for enc in (self._fneg(3, 4), self._fabs(3, 4), self._frsp(3, 4)):
            insn = _decode(enc)[0]
            reads, _writes = register_effects(insn)
            self.assertIn("f4", reads)
            self.assertNotIn("f0", reads)

    def test_infer_fmr_leaf_reads_fb(self) -> None:
        leaf = _decode(self._fmr(3, 4) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("f4", contract.reads)
        self.assertNotIn("f0", contract.reads)
        self.assertIn("f3", contract.writes)

    def test_fmr_leaf_summarized_not_false_equivalent(self) -> None:
        from tools.ppc_equivalence.semantics import CalleeContract

        leaf_hex = (self._fmr(3, 4) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("f4", inferred.reads)

        left_hex = (self._fmr(4, 1) + "48000001" + BLR).replace(" ", "")
        right_hex = (self._fmr(4, 2) + "48000001" + BLR).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        domain = FloatingPointDomain()
        contract = make_contract(preset=None, observe=["f3"], timeout_ms=15_000)

        oh = (self._fmr(4, 1) + self._fmr(3, 4) + BLR).replace(" ", "")
        ch = (self._fmr(4, 2) + self._fmr(3, 4) + BLR).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            contract,
            original_hex=oh,
            candidate_hex=ch,
            floating_point_domain=domain,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "f3")

        buggy = CalleeContract(
            frozenset({"f0", "valid"}),
            frozenset({"f3", "valid"}),
            "matched-body-effects",
        )
        buggy_result = check_equivalence(
            left,
            right,
            contract,
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": buggy},
            floating_point_domain=domain,
        )
        self.assertEqual(
            buggy_result.status,
            ProofStatus.EQUIVALENT,
            "control: FA/f0-keyed summary must still demonstrate the hole",
        )

        summarized = check_equivalence(
            left,
            right,
            contract,
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
            floating_point_domain=domain,
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via fmr FA indexing: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )


class LrCallSummaryTrampolineTests(unittest.TestCase):
    """mtlr trampolines must update LR in matched-callee summaries.

    Adversarial PoC (2026-07-24): ``_apply_call_summary`` always preserved
    entry LR, and ``infer_callee_contract`` dropped ``lr`` from the volatile
    write filter. Callers of ``mtlr r4; blr`` that only diverged in ``r4``
    proved EQUIVALENT (same preserved link address) while inlined bodies
    differed on ``exit.target``.
    """

    _CODE_BASE = 0x80000000

    @staticmethod
    def _mtlr(reg: int) -> str:
        spr = 8
        spr_low = spr & 0x1F
        spr_high = (spr >> 5) & 0x1F
        return _w(
            (31 << 26)
            | (reg << 21)
            | (spr_low << 16)
            | (spr_high << 11)
            | (467 << 1)
        )

    def test_infer_mtlr_blr_leaf_writes_lr(self) -> None:
        leaf = _decode(self._mtlr(4) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("lr", contract.writes)
        self.assertIn("r4", contract.reads)

    def test_mtlr_trampoline_summarized_not_false_equivalent(self) -> None:
        from tools.ppc_equivalence.semantics import CalleeContract

        leaf_hex = (self._mtlr(4) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("lr", inferred.writes)

        left_hex = (_enc_li(4, 0x1000) + "48000001" + BLR).replace(" ", "")
        right_hex = (_enc_li(4, 0x2000) + "48000001" + BLR).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )

        oh = (_enc_li(4, 0x1000) + self._mtlr(4) + BLR).replace(" ", "")
        ch = (_enc_li(4, 0x2000) + self._mtlr(4) + BLR).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            make_contract(preset=None, observe=["r1"], timeout_ms=15_000),
            original_hex=oh,
            candidate_hex=ch,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "exit.target")

        buggy = CalleeContract(frozenset({"r4"}), frozenset(), "matched-body-effects")
        buggy_result = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r1"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": buggy},
        )
        self.assertEqual(
            buggy_result.status,
            ProofStatus.EQUIVALENT,
            "control: lr-preserving summary must still demonstrate the hole",
        )

        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r1"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via mtlr lr omission: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )


class MsrGqrCallSummaryApplyTests(unittest.TestCase):
    """Matched-callee summaries must apply MSR/GQR (and kin) writes.

    Adversarial PoC (2026-07-24): ``_apply_call_summary`` hard-preserved entry
    ``msr`` / ``gqr*`` / ``sr*`` / time-base / SRR* / aux SPR even when the
    contract listed them, and inference dropped those names from the volatile
    write filter. Callers of ``mtmsr r4; blr`` (or ``mtspr GQR0,r4; blr``) that
    only diverged in ``r4``, then observed via ``mfmsr`` / ``mfspr``, proved
    EQUIVALENT under an empty-write summary while inlined bodies differed on
    ``r3``.
    """

    _CODE_BASE = 0x80000000

    @staticmethod
    def _mtspr(reg: int, spr: int) -> str:
        spr_low = spr & 0x1F
        spr_high = (spr >> 5) & 0x1F
        return _w(
            (31 << 26)
            | (reg << 21)
            | (spr_low << 16)
            | (spr_high << 11)
            | (467 << 1)
        )

    @staticmethod
    def _mfspr(reg: int, spr: int) -> str:
        spr_low = spr & 0x1F
        spr_high = (spr >> 5) & 0x1F
        return _w(
            (31 << 26)
            | (reg << 21)
            | (spr_low << 16)
            | (spr_high << 11)
            | (339 << 1)
        )

    @staticmethod
    def _mtmsr(reg: int) -> str:
        return _w((31 << 26) | (reg << 21) | (146 << 1))

    @staticmethod
    def _mfmsr(reg: int) -> str:
        return _w((31 << 26) | (reg << 21) | (83 << 1))

    def test_infer_mtmsr_blr_leaf_writes_msr(self) -> None:
        leaf = _decode(self._mtmsr(4) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("msr", contract.writes)
        self.assertIn("r4", contract.reads)

    def test_infer_mtspr_gqr0_blr_leaf_writes_gqr0(self) -> None:
        leaf = _decode(self._mtspr(4, 912) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("gqr0", contract.writes)
        self.assertIn("r4", contract.reads)

    @staticmethod
    def _mtsr(reg: int, sr: int) -> str:
        return _w((31 << 26) | (reg << 21) | (sr << 16) | (210 << 1))

    @staticmethod
    def _mfsr(reg: int, sr: int) -> str:
        return _w((31 << 26) | (reg << 21) | (sr << 16) | (595 << 1))

    def test_infer_mtsr0_blr_leaf_writes_sr0(self) -> None:
        leaf = _decode(self._mtsr(4, 0) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("sr0", contract.writes)
        self.assertIn("r4", contract.reads)

    def test_infer_mtspr_hid0_blr_leaf_writes_hid0(self) -> None:
        leaf = _decode(self._mtspr(4, 1008) + " " + BLR)
        contract = infer_callee_contract(leaf)
        self.assertIn("hid0", contract.writes)
        self.assertIn("r4", contract.reads)

    def test_mtmsr_leaf_mfmsr_observer_not_false_equivalent(self) -> None:
        from tools.ppc_equivalence.semantics import CalleeContract

        leaf_hex = (self._mtmsr(4) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("msr", inferred.writes)

        left_hex = (
            _enc_li(4, 0x1000) + "48000001" + self._mfmsr(3) + BLR
        ).replace(" ", "")
        right_hex = (
            _enc_li(4, 0x2000) + "48000001" + self._mfmsr(3) + BLR
        ).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )

        oh = (
            _enc_li(4, 0x1000) + self._mtmsr(4) + self._mfmsr(3) + BLR
        ).replace(" ", "")
        ch = (
            _enc_li(4, 0x2000) + self._mtmsr(4) + self._mfmsr(3) + BLR
        ).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            make_contract(preset=None, observe=["r3"], timeout_ms=15_000),
            original_hex=oh,
            candidate_hex=ch,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "r3")

        buggy = CalleeContract(frozenset({"r4"}), frozenset(), "matched-body-effects")
        buggy_result = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": buggy},
        )
        self.assertEqual(
            buggy_result.status,
            ProofStatus.EQUIVALENT,
            "control: msr-preserving summary must still demonstrate the hole",
        )

        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via mtmsr msr omission: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )

    def test_mtspr_gqr0_leaf_mfspr_observer_not_false_equivalent(self) -> None:
        from tools.ppc_equivalence.semantics import CalleeContract

        leaf_hex = (self._mtspr(4, 912) + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn("gqr0", inferred.writes)

        left_hex = (
            _enc_li(4, 0x1000) + "48000001" + self._mfspr(3, 912) + BLR
        ).replace(" ", "")
        right_hex = (
            _enc_li(4, 0x2000) + "48000001" + self._mfspr(3, 912) + BLR
        ).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )

        oh = (
            _enc_li(4, 0x1000) + self._mtspr(4, 912) + self._mfspr(3, 912) + BLR
        ).replace(" ", "")
        ch = (
            _enc_li(4, 0x2000) + self._mtspr(4, 912) + self._mfspr(3, 912) + BLR
        ).replace(" ", "")
        inlined = check_equivalence(
            decode_block(parse_hex(oh), validate_with_capstone=False),
            decode_block(parse_hex(ch), validate_with_capstone=False),
            make_contract(preset=None, observe=["r3"], timeout_ms=15_000),
            original_hex=oh,
            candidate_hex=ch,
        )
        self.assertEqual(inlined.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((inlined.mismatch or {}).get("name"), "r3")

        buggy = CalleeContract(frozenset({"r4"}), frozenset(), "matched-body-effects")
        buggy_result = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": buggy},
        )
        self.assertEqual(
            buggy_result.status,
            ProofStatus.EQUIVALENT,
            "control: gqr-preserving summary must still demonstrate the hole",
        )

        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via mtspr gqr omission: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )

    def _summarized_observer_pair(
        self,
        *,
        leaf_insn: str,
        observe_insn: str,
        write_name: str,
    ):
        from tools.ppc_equivalence.semantics import CalleeContract

        leaf_hex = (leaf_insn + BLR).replace(" ", "")
        leaf = decode_block(
            parse_hex(leaf_hex), base=0x80001000, validate_with_capstone=False,
        )
        inferred = infer_callee_contract(leaf)
        self.assertIn(write_name, inferred.writes)

        left_hex = (
            _enc_li(4, 0x1000) + "48000001" + observe_insn + BLR
        ).replace(" ", "")
        right_hex = (
            _enc_li(4, 0x2000) + "48000001" + observe_insn + BLR
        ).replace(" ", "")
        reloc = (
            RelocationRef(
                offset=4,
                relocation_type=R_PPC_REL24,
                symbol="leaf",
                canonical_symbol="leaf",
                addend=0,
            ),
        )
        left = decode_block(
            parse_hex(left_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        right = decode_block(
            parse_hex(right_hex),
            base=self._CODE_BASE,
            validate_with_capstone=False,
            relocations=reloc,
        )
        buggy = CalleeContract(frozenset({"r4"}), frozenset(), "matched-body-effects")
        return left, right, left_hex, right_hex, inferred, buggy

    def test_mtsr0_leaf_mfsr_observer_not_false_equivalent(self) -> None:
        left, right, left_hex, right_hex, inferred, buggy = self._summarized_observer_pair(
            leaf_insn=self._mtsr(4, 0),
            observe_insn=self._mfsr(3, 0),
            write_name="sr0",
        )
        buggy_result = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": buggy},
        )
        self.assertEqual(
            buggy_result.status,
            ProofStatus.EQUIVALENT,
            "control: sr-preserving summary must still demonstrate the hole",
        )
        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via mtsr sr omission: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )

    def test_mtspr_hid0_leaf_mfspr_observer_not_false_equivalent(self) -> None:
        left, right, left_hex, right_hex, inferred, buggy = self._summarized_observer_pair(
            leaf_insn=self._mtspr(4, 1008),
            observe_insn=self._mfspr(3, 1008),
            write_name="hid0",
        )
        buggy_result = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": buggy},
        )
        self.assertEqual(
            buggy_result.status,
            ProofStatus.EQUIVALENT,
            "control: hid0-preserving summary must still demonstrate the hole",
        )
        summarized = check_equivalence(
            left,
            right,
            make_contract(preset=None, observe=["r3"], timeout_ms=20_000),
            original_hex=left_hex,
            candidate_hex=right_hex,
            assumed_callees=frozenset({"leaf"}),
            callee_contracts={"leaf": inferred},
        )
        self.assertNotEqual(
            summarized.status,
            ProofStatus.EQUIVALENT,
            f"false EQUIVALENT via mtspr hid0 omission: {summarized.status} "
            f"mismatch={summarized.mismatch} contract={inferred}",
        )


def _enc_mr(rd: int, rs: int) -> str:
    # or rA,rS,rB with rA=rd, rS=rB=rs  (PPC mr destination is rA)
    return _w((31 << 26) | (rs << 21) | (rd << 16) | (rs << 11) | (444 << 1))


class RegisterPublishStackEscapeTests(unittest.TestCase):
    """Compared-register SP publish must clear private-stack masking.

    Adversarial PoC (2026-07-23): ``addi r3,r1,8`` left live across ``blr`` with
    divergent stores to that slot was falsely ``EQUIVALENT`` under auto / strict
    / ``observe=[r3,memory]`` because escape only fired on public *stores*.
    """

    @staticmethod
    def _pointer_slot_pair(
        *,
        pointer_reg: int,
        clear_pointer: bool = False,
        clear_r0: bool = True,
    ) -> tuple[str, str]:
        prologue = _enc_addi(1, 1, -32)
        epilogue = _enc_addi(1, 1, 32)
        publish = _enc_addi(pointer_reg, 1, 8)
        body_orig = [prologue, publish, _enc_li(0, 1), _enc_stw(0, 1, 8)]
        body_cand = [prologue, publish, _enc_li(0, 2), _enc_stw(0, 1, 8)]
        if clear_r0:
            body_orig.append(_enc_li(0, 0))
            body_cand.append(_enc_li(0, 0))
        if clear_pointer:
            body_orig.append(_enc_li(pointer_reg, 0))
            body_cand.append(_enc_li(pointer_reg, 0))
        body_orig.extend([epilogue, BLR])
        body_cand.extend([epilogue, BLR])
        return " ".join(body_orig), " ".join(body_cand)

    def test_helper_detects_r3_publish_not_r1(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        # After a symbolic addi into r3 from r1.
        published = state.with_gpr(3, ops.add(state.gpr[1], ops.const(8)))
        self.assertTrue(compared_registers_publish_entry_r1(published, (3,), ops))
        self.assertFalse(compared_registers_publish_entry_r1(published, (1,), ops))
        escaped = apply_compared_register_publish_escape(published, (3,), ops)
        self.assertTrue(z3.is_false(z3.simplify(escaped.stack_private)))

    def test_r3_pointer_private_diverge_not_equivalent(self) -> None:
        original, candidate = self._pointer_slot_pair(pointer_reg=3)
        for preset in ("auto", "ppc-eabi", "strict"):
            with self.subTest(preset=preset):
                result = _prove(original, candidate, preset=preset)
                self.assertEqual(
                    result.status,
                    ProofStatus.NOT_EQUIVALENT,
                    f"{preset}: {result.status} mismatch={result.mismatch}",
                )
                scope = result.memory_scope.to_dict()["private_stack"]
                self.assertFalse(scope["original"]["enabled_on_all_terminal_paths"])
                self.assertFalse(scope["candidate"]["enabled_on_all_terminal_paths"])

    def test_r3_pointer_manual_r3_memory_not_equivalent(self) -> None:
        original, candidate = self._pointer_slot_pair(pointer_reg=3)
        result = _prove(original, candidate, observe=["r3", "memory"])
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("kind"), "memory")

    def test_r31_nonvolatile_pointer_not_equivalent(self) -> None:
        original, candidate = self._pointer_slot_pair(pointer_reg=31)
        result = _prove(original, candidate, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_r14_nonvolatile_pointer_not_equivalent(self) -> None:
        original, candidate = self._pointer_slot_pair(pointer_reg=14)
        result = _prove(original, candidate, preset="ppc-eabi")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_mr_r3_r1_publish_not_equivalent(self) -> None:
        prologue = _enc_addi(1, 1, -32)
        epilogue = _enc_addi(1, 1, 32)
        publish = _enc_mr(3, 1)
        original = " ".join([
            prologue, publish, _enc_li(0, 1), _enc_stw(0, 1, 8),
            _enc_li(0, 0), epilogue, BLR,
        ])
        candidate = " ".join([
            prologue, publish, _enc_li(0, 2), _enc_stw(0, 1, 8),
            _enc_li(0, 0), epilogue, BLR,
        ])
        result = _prove(original, candidate, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_pointer_cleared_before_return_keeps_masking(self) -> None:
        """If the frame pointer is killed before exit, private diverge may stay EQ."""
        original, candidate = self._pointer_slot_pair(
            pointer_reg=3, clear_pointer=True,
        )
        result = _prove(original, candidate, preset="auto")
        self.assertEqual(
            result.status,
            ProofStatus.EQUIVALENT,
            f"{result.status} mismatch={result.mismatch}",
        )
        scope = result.memory_scope.to_dict()["private_stack"]
        self.assertTrue(scope["original"]["enabled_on_all_terminal_paths"])

    def test_volatile_r11_temp_frame_pointer_keeps_masking(self) -> None:
        """r11 frame-pointer temp is not compared under auto — must not false-NE."""
        original, candidate = self._pointer_slot_pair(pointer_reg=11)
        result = _prove(original, candidate, preset="auto")
        self.assertEqual(
            result.status,
            ProofStatus.EQUIVALENT,
            f"false NOT_EQUIVALENT via r11 temp: {result.status} "
            f"mismatch={result.mismatch}",
        )
        scope = result.memory_scope.to_dict()["private_stack"]
        self.assertTrue(scope["original"]["enabled_on_all_terminal_paths"])

    def test_private_only_without_publish_still_equivalent(self) -> None:
        prologue = _enc_addi(1, 1, -32)
        epilogue = _enc_addi(1, 1, 32)
        original = " ".join([
            prologue, _enc_li(0, 1), _enc_stw(0, 1, 8), epilogue, BLR,
        ])
        candidate = " ".join([
            prologue, _enc_li(0, 2), _enc_stw(0, 1, 8), epilogue, BLR,
        ])
        result = _prove(original, candidate, preset="auto")
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)


if __name__ == "__main__":
    unittest.main()

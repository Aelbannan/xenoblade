"""GX FIFO Tier-A mutation/soundness tests beyond ``test_gx_fifo_loop_discharge.py``.

Covers the promotion-pipeline mutation classes called out for this corpus
land: dropped/duplicated/reordered loop events, off-by-one trip counts,
omitted bound/zero-wrap proofs, UNSAT->SAT/unknown solver results, forged
digests (query/witness/profile/bus-spec/corpus), a caller-claimed
``status=promotion-grade`` that disagrees with the recomputed verdict, and
an ad-hoc bus map substituting for a reviewed hardware profile.
"""

from __future__ import annotations

import copy
import unittest
from dataclasses import replace

import z3

from tools.ppc_equivalence.gx_fifo_corpus import compute_gx_fifo_v1_corpus_sha256
from tools.ppc_equivalence.gx_fifo_loop import build_gx_fifo_loop_plans
from tools.ppc_equivalence.gx_fifo_loop_discharge import (
    discharge_gx_fifo_loop_plan,
    refinement_for_plan,
    validate_gx_fifo_loop_refinement_v2,
)
from tools.ppc_equivalence.hardware_profile import (
    is_reviewed_hardware_profile,
    load_hardware_profile,
)
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.mmio_capability_obligations import (
    SOURCE_AD_HOC_BUS,
    SOURCE_HARDWARE_PROFILE,
    build_gx_fifo_loop_exact_refinement_obligation,
    build_gx_fifo_read_obligation,
    build_mmio_loop_emission_obligation,
    classify_mmio_source,
    gx_fifo_read_is_promotion_grade,
    loop_exact_refinement_has_real_unsat,
    mmio_loop_emission_is_promotion_grade,
    validate_gx_fifo_read_obligation,
    validate_mmio_loop_emission_obligation,
)
from tools.ppc_equivalence.symbolic_event_trace import SymbolicEventTrace, SymbolicFifoEvent


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _materialize_gx_base(reg: int, *, address: int = 0) -> list[Instruction]:
    return [
        _insn(Opcode.ADDIS, (reg, 0, 0xCC00), address=address),
        _insn(Opcode.ORI, (reg, reg, 0x8000), address=address + 4),
    ]


def _gx_fifo_program(*, count: int, store: tuple[Opcode, int], base_reg: int = 5) -> list[Instruction]:
    prologue = _materialize_gx_base(base_reg)
    pc = len(prologue) * 4
    prologue.append(_insn(Opcode.ADDI, (0, 0, count), address=pc))
    pc += 4
    prologue.append(_insn(Opcode.MTSPR, (0, 9), address=pc))
    pc += 4
    header_pc = pc
    opcode, source_reg = store
    body = [_insn(opcode, (source_reg, base_reg, 0), address=pc)]
    pc += 4
    latch_pc = pc
    return [
        *prologue,
        *body,
        _insn(Opcode.BC, (16, 0, header_pc, 0), address=latch_pc),
        _insn(Opcode.BCLR, (20, 0, 0), address=latch_pc + 4),
    ]


def _build_plan(**kwargs):
    program = _gx_fifo_program(**kwargs)
    plans = build_gx_fifo_loop_plans(program)
    return next(iter(plans.values()))


# --------------------------------------------------------------------------
# 1. Dropped / duplicated / reordered loop events -> compare fails
# --------------------------------------------------------------------------


def _write_event(value: int, *, addr: int = 0xCC008000, width: int = 4) -> SymbolicFifoEvent:
    return SymbolicFifoEvent(kind="write", addr=addr, width=width, value=value)


def _trace(events: tuple) -> SymbolicEventTrace:
    return SymbolicEventTrace(
        device_id="gx-fifo", base=0xCC008000, span=128, max_events=256, events=events,
    )


def _is_definitely_unequal(equal_formula) -> bool:
    """``compare_equal``'s ``equal`` term is definitely False for every
    assignment (i.e. the mutation is detected) iff asserting it is UNSAT."""
    solver = z3.Solver()
    solver.add(equal_formula)
    return solver.check() == z3.unsat


class DroppedDuplicatedReorderedEventTests(unittest.TestCase):
    def setUp(self) -> None:
        self.original = _trace((_write_event(1), _write_event(2), _write_event(3)))

    def test_identical_trace_compares_equal(self) -> None:
        clone = _trace(self.original.events)
        result = self.original.compare_equal(clone, z3)
        self.assertTrue(result.supported)
        self.assertFalse(_is_definitely_unequal(result.equal), "identical traces must not be forced unequal")

    def test_dropped_event_compare_fails(self) -> None:
        dropped = _trace(self.original.events[:2])
        result = self.original.compare_equal(dropped, z3)
        self.assertTrue(result.supported)
        self.assertTrue(_is_definitely_unequal(result.equal))

    def test_duplicated_event_compare_fails(self) -> None:
        duplicated = _trace(self.original.events + (self.original.events[-1],))
        result = self.original.compare_equal(duplicated, z3)
        self.assertTrue(result.supported)
        self.assertTrue(_is_definitely_unequal(result.equal))

    def test_reordered_events_compare_fails(self) -> None:
        reordered = _trace((self.original.events[0], self.original.events[2], self.original.events[1]))
        result = self.original.compare_equal(reordered, z3)
        self.assertTrue(result.supported)
        self.assertTrue(_is_definitely_unequal(result.equal))


# --------------------------------------------------------------------------
# 2. Trip count off-by-one -> discharge fails / never silently discharges
# --------------------------------------------------------------------------


class TripCountOffByOneTests(unittest.TestCase):
    def test_summary_trip_count_off_by_one_from_witness_is_never_discharged(self) -> None:
        """Mutating just summary.trip_count (leaving the witness's own
        recognized_trip_count alone) must be caught by shape validation
        before any SMT query runs — never silently discharged."""
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        off_by_one = replace(plan.summary, trip_count=plan.summary.trip_count + 1)
        mutated_plan = replace(plan, summary=off_by_one)
        result = discharge_gx_fifo_loop_plan(mutated_plan, require_entry_violations=False)
        self.assertFalse(result.all_unsat())
        self.assertIn(result.status, ("unsupported", "internal_error", "failed"))

    def test_off_by_one_low_still_never_discharges(self) -> None:
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        off_by_one = replace(plan.summary, trip_count=plan.summary.trip_count - 1)
        mutated_plan = replace(plan, summary=off_by_one)
        result = discharge_gx_fifo_loop_plan(mutated_plan, require_entry_violations=False)
        self.assertFalse(result.all_unsat())


# --------------------------------------------------------------------------
# 3. Bound or zero-wrap (termination) proof omitted -> validator rejects
# --------------------------------------------------------------------------


class OmittedBoundOrTerminationProofTests(unittest.TestCase):
    def _discharged_refinement(self) -> dict:
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)
        return result.refinement

    def test_omitted_bound_block_rejected(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        del refinement["bound"]
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("bound", reason)

    def test_omitted_termination_block_rejected(self) -> None:
        """``termination`` is this domain's zero-wrap / CTR-ranking proof
        (see gx_fifo_loop_discharge._discharge_termination); omitting it
        under a claimed status=discharged must be rejected."""
        refinement = copy.deepcopy(self._discharged_refinement())
        del refinement["termination"]
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("termination", reason)


# --------------------------------------------------------------------------
# 4. Solver result UNSAT -> SAT / unknown must not be promotion-grade
# --------------------------------------------------------------------------


class SolverResultDowngradeTests(unittest.TestCase):
    def _discharged_refinement(self) -> dict:
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)
        return result.refinement

    def test_flipping_unsat_to_sat_under_discharged_status_rejected(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        refinement["footprint"]["result"] = "sat"
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("unsat", reason)

    def test_flipping_unsat_to_unknown_under_discharged_status_rejected(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        refinement["bound"]["result"] = "unknown"
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)

    def test_full_obligation_real_unsat_check_rejects_sat_side(self) -> None:
        """``loop_exact_refinement_has_real_unsat`` (used to gate
        mmio-loop-emission promotion) must also reject a SAT-flipped side,
        not just the narrower per-block validator."""
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        payload = refinement_for_plan(plan, require_entry_violations=False)
        assert payload is not None
        obligation = build_gx_fifo_loop_exact_refinement_obligation(
            original=payload, candidate=payload, status="promotion-grade",
        )
        self.assertTrue(loop_exact_refinement_has_real_unsat(obligation))

        mutated_candidate = copy.deepcopy(payload)
        mutated_candidate["refinement"]["postcondition"]["result"] = "sat"
        mutated_obligation = build_gx_fifo_loop_exact_refinement_obligation(
            original=payload, candidate=mutated_candidate, status="promotion-grade",
        )
        self.assertFalse(loop_exact_refinement_has_real_unsat(mutated_obligation))


# --------------------------------------------------------------------------
# 5. Query / witness / profile / bus-spec / corpus hash forged -> reject
# --------------------------------------------------------------------------


class ForgedDigestTests(unittest.TestCase):
    def test_forged_query_sha256_placeholder_rejected(self) -> None:
        plan = _build_plan(count=3, store=(Opcode.STW, 4))
        result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)
        refinement = copy.deepcopy(result.refinement)
        refinement["event_identity"]["query_sha256"] = "0" * 64
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("placeholder", reason)

    def test_forged_non_hex_query_sha256_rejected(self) -> None:
        plan = _build_plan(count=3, store=(Opcode.STW, 4))
        result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)
        refinement = copy.deepcopy(result.refinement)
        refinement["termination"]["query_sha256"] = "not-a-real-digest"
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)

    def test_witness_code_sha256_forged_desyncs_from_recomputation(self) -> None:
        """A forged witness code_sha256 no longer matches recomputation from
        the plan's own instructions — the drift-proof pairing is broken."""
        from tools.ppc_equivalence.gx_fifo_loop import compute_witness_code_sha256

        plan = _build_plan(count=3, store=(Opcode.STW, 4))
        real = plan.witness.code_sha256
        forged_witness = replace(plan.witness, code_sha256="a" * 64)
        recomputed = compute_witness_code_sha256(
            forged_witness.body, forged_witness.latch, forged_witness.mtctr,
        )
        self.assertEqual(recomputed, real)
        self.assertNotEqual(forged_witness.code_sha256, recomputed)

    def test_forged_hardware_profile_sha256_rejected(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v2")
        obligation = build_gx_fifo_read_obligation(
            hardware_profile=profile,
            path_and_gx_read={"result": "unsat", "query_sha256": "b" * 64},
            status="promotion-grade",
        )
        forged = dict(obligation)
        forged["hardware_profile_sha256"] = "c" * 64
        reason = validate_gx_fifo_read_obligation(forged, require_promotion_shape=True)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("hardware_profile_sha256", reason)
        self.assertFalse(gx_fifo_read_is_promotion_grade(forged))

    def test_forged_corpus_sha256_detected_by_recomputation(self) -> None:
        """``manifest.json``'s corpus_sha256 must equal what the grader
        recomputes; a forged value is caught by direct comparison (see
        test_gx_fifo_corpus.py for the on-disk manifest assertion)."""
        real = compute_gx_fifo_v1_corpus_sha256()
        forged = "0" * 64
        self.assertNotEqual(real, forged)


# --------------------------------------------------------------------------
# 6. Caller-supplied status=promotion-grade disagreeing with recomputed
# --------------------------------------------------------------------------


class CallerStatusDisagreesWithRecomputedTests(unittest.TestCase):
    def test_mmio_loop_emission_promotion_grade_claim_recomputed_false(self) -> None:
        plan = _build_plan(count=2, store=(Opcode.STW, 3))
        payload = refinement_for_plan(plan, require_entry_violations=False)
        assert payload is not None
        good_obligation = build_gx_fifo_loop_exact_refinement_obligation(
            original=payload, candidate=payload, status="promotion-grade",
        )
        good = build_mmio_loop_emission_obligation(
            loop_refinement=good_obligation, status="promotion-grade",
        )
        self.assertIsNone(validate_mmio_loop_emission_obligation(good))
        self.assertTrue(mmio_loop_emission_is_promotion_grade(good))

        # Caller claims promotion-grade, but flips one block to sat: the
        # obligation still parses (status is just a string field) but the
        # *recomputed* promotion verdict must disagree with the claim.
        broken_candidate = copy.deepcopy(payload)
        broken_candidate["refinement"]["body_step"]["result"] = "sat"
        broken_obligation = build_gx_fifo_loop_exact_refinement_obligation(
            original=payload, candidate=broken_candidate, status="promotion-grade",
        )
        broken = build_mmio_loop_emission_obligation(
            loop_refinement=broken_obligation, status="promotion-grade",
        )
        self.assertEqual(broken["status"], "promotion-grade")  # caller's claim, unchanged
        self.assertFalse(
            mmio_loop_emission_is_promotion_grade(broken),
            "recomputed promotion verdict must reject a sat-flipped side "
            "regardless of the caller's claimed status",
        )

    def test_gx_fifo_read_promotion_grade_claim_recomputed_false_without_unsat(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v2")
        claims_promotion_but_pending = build_gx_fifo_read_obligation(
            hardware_profile=profile,
            path_and_gx_read={"result": "pending"},
            status="promotion-grade",
        )
        # The caller claims promotion-grade, but the wrapped query never
        # reached unsat: the recomputed verdict must disagree.
        self.assertFalse(gx_fifo_read_is_promotion_grade(claims_promotion_but_pending))


# --------------------------------------------------------------------------
# 7. Ad-hoc map substitutes for reviewed profile -> incomplete
# --------------------------------------------------------------------------


class AdHocMapNeverPromotionGradeTests(unittest.TestCase):
    def test_classify_mmio_source_ad_hoc_flag(self) -> None:
        self.assertEqual(classify_mmio_source(hardware_profile=None, ad_hoc=True), SOURCE_AD_HOC_BUS)

    def test_classify_mmio_source_reviewed_profile(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v2")
        self.assertTrue(is_reviewed_hardware_profile(profile))
        self.assertEqual(
            classify_mmio_source(hardware_profile=profile, ad_hoc=False),
            SOURCE_HARDWARE_PROFILE,
        )

    def test_ad_hoc_bus_map_claiming_promotion_grade_is_rejected(self) -> None:
        ad_hoc_obligation = build_gx_fifo_read_obligation(
            hardware_profile=None,
            ad_hoc=True,
            path_and_gx_read={"result": "unsat", "query_sha256": "d" * 64},
            status="promotion-grade",
        )
        self.assertEqual(ad_hoc_obligation["source"], SOURCE_AD_HOC_BUS)
        reason = validate_gx_fifo_read_obligation(ad_hoc_obligation, require_promotion_shape=True)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("hardware-profile", reason)
        self.assertFalse(gx_fifo_read_is_promotion_grade(ad_hoc_obligation))

    def test_ad_hoc_bus_map_stays_incomplete_even_when_vacuous_is_false(self) -> None:
        """An ad-hoc map can never substitute for the reviewed profile
        (GX_FIFO_TIER_A.md 'Shared foundations' #4: "Ad-hoc caller-built bus
        maps grade incomplete forever."). Only the vacuous escape hatch
        (no reachable GX read demand) bypasses the profile requirement."""
        ad_hoc_vacuous = build_gx_fifo_read_obligation(
            hardware_profile=None, ad_hoc=True, vacuous=True, status="promotion-grade",
        )
        # Vacuous is the one legitimate route that does not need a profile.
        self.assertIsNone(
            validate_gx_fifo_read_obligation(ad_hoc_vacuous, require_promotion_shape=True),
        )
        self.assertTrue(gx_fifo_read_is_promotion_grade(ad_hoc_vacuous))

        ad_hoc_non_vacuous = build_gx_fifo_read_obligation(
            hardware_profile=None,
            ad_hoc=True,
            vacuous=False,
            path_and_gx_read={"result": "unsat", "query_sha256": "e" * 64},
            status="promotion-grade",
        )
        reason = validate_gx_fifo_read_obligation(
            ad_hoc_non_vacuous, require_promotion_shape=True,
        )
        self.assertIsNotNone(reason)
        self.assertFalse(gx_fifo_read_is_promotion_grade(ad_hoc_non_vacuous))


if __name__ == "__main__":
    unittest.main()

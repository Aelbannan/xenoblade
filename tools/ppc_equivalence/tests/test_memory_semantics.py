"""Unit tests for typed memory-loop store effects and readonly contexts."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.memory_loop_readonly import (
    MemoryLoopReadonlyContext,
    ReadonlyWordEvidence,
    build_memory_loop_readonly_context,
    merge_side_readonly_words,
    readonly_word_byte_constraints,
)
from tools.ppc_equivalence.memory_semantics import (
    StoreEffect,
    apply_store_effect,
    footprint_ok_for_summary,
    footprint_wraps_u32,
)
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, SymbolicOps


class MemorySemanticsTests(unittest.TestCase):
    def test_apply_store_effect_records_writes(self) -> None:
        state = concrete_state({"gpr": {"r3": 0x1000, "r4": 0xAABBCCDD}})
        effect = StoreEffect(address=0x1000, value=0xAABBCCDD, width=4)
        result = apply_store_effect(state, effect, ConcreteOps())
        self.assertEqual(len(result.memory_writes), 4)
        self.assertEqual(result.memory_touches, result.memory_writes)
        self.assertEqual(result.memory.read(0x1000), 0xAA)
        self.assertEqual(result.memory.read(0x1003), 0xDD)

    def test_footprint_gates(self) -> None:
        self.assertTrue(
            footprint_ok_for_summary(
                trip_count=4, stride=4, store_width=4, store_kind="d-form-addi",
            ),
        )
        self.assertFalse(
            footprint_ok_for_summary(
                trip_count=4, stride=8, store_width=4, store_kind="d-form-addi",
            ),
        )
        self.assertTrue(footprint_wraps_u32(0xFFFFFFF0, 8, 4, "d-form-addi"))
        self.assertFalse(footprint_wraps_u32(0x1000, 4, 4, "d-form-addi"))


class MemoryLoopReadonlyTests(unittest.TestCase):
    def test_conflict_reason_on_disagreeing_values(self) -> None:
        ctx = MemoryLoopReadonlyContext(
            original=(ReadonlyWordEvidence(0x1000, 1),),
            candidate=(ReadonlyWordEvidence(0x1000, 2),),
        )
        reason = ctx.conflict_reason()
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("conflict", reason)

    def test_merge_side_rejects_conflict(self) -> None:
        with self.assertRaises(ValueError):
            merge_side_readonly_words({0x1000: 1}, {0x1000: 2})

    def test_shared_words_clone_both_sides(self) -> None:
        ctx = build_memory_loop_readonly_context(shared_words={0x2000: 7})
        assert ctx is not None
        self.assertEqual(ctx.words_for("original"), {0x2000: 7})
        self.assertEqual(ctx.words_for("candidate"), {0x2000: 7})

    def test_byte_constraints_pin_initial_memory(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        initial = z3.Array("mem", z3.BitVecSort(32), z3.BitVecSort(8))
        evidence = (ReadonlyWordEvidence(0x80201000, 0x01020304),)
        constraints = readonly_word_byte_constraints(initial, evidence, ops)
        self.assertEqual(len(constraints), 4)
        solver = z3.Solver()
        solver.add(*constraints)
        self.assertEqual(solver.check(), z3.sat)
        model = solver.model()
        self.assertEqual(
            model.eval(z3.Select(initial, z3.BitVecVal(0x80201000, 32))).as_long(),
            0x01,
        )


class MemoryLoopReadonlyProvenanceTests(unittest.TestCase):
    def test_dol_evidence_not_shadowed_by_elf(self) -> None:
        # Same address+value, different per-side image origin: the DOL word must
        # not collapse onto the ELF word. The identity payload keeps the image
        # kind / artifact per side so retail (DOL) and candidate (ELF) never
        # conflate, and the identity digest differs from a provenance-free one.
        from tools.ppc_equivalence.memory_loop_readonly import (
            compute_memory_loop_readonly_sha256,
            memory_loop_readonly_identity_payload,
        )

        provenance = MemoryLoopReadonlyContext(
            original=(
                ReadonlyWordEvidence(
                    0x80201000, 0x2A,
                    source="image:dol",
                    artifact_sha256="a" * 64,
                    image_kind="dol",
                    section=".text",
                ),
            ),
            candidate=(
                ReadonlyWordEvidence(
                    0x80201000, 0x2A,
                    source="image:elf",
                    artifact_sha256="b" * 64,
                    image_kind="elf",
                    section=".text",
                ),
            ),
        )
        payload = memory_loop_readonly_identity_payload(provenance)
        self.assertEqual(payload["original"][0]["image_kind"], "dol")
        self.assertEqual(payload["candidate"][0]["image_kind"], "elf")
        self.assertNotEqual(
            payload["original"][0]["artifact_sha256"],
            payload["candidate"][0]["artifact_sha256"],
        )

        bare = MemoryLoopReadonlyContext(
            original=(ReadonlyWordEvidence(0x80201000, 0x2A),),
            candidate=(ReadonlyWordEvidence(0x80201000, 0x2A),),
        )
        self.assertNotEqual(
            compute_memory_loop_readonly_sha256(provenance),
            compute_memory_loop_readonly_sha256(bare),
        )

    def test_readonly_identity_changes_proof_request_hash(self) -> None:
        from tools.ppc_equivalence.memory_loop_readonly import (
            build_memory_loop_readonly_obligation,
        )
        from tools.ppc_equivalence.provenance import proof_request_hash

        def _hash(value: int) -> str:
            ctx = MemoryLoopReadonlyContext(
                original=(
                    ReadonlyWordEvidence(
                        0x80201000, value, image_kind="dol", artifact_sha256="a" * 64,
                    ),
                ),
                candidate=(
                    ReadonlyWordEvidence(
                        0x80201000, value, image_kind="elf", artifact_sha256="b" * 64,
                    ),
                ),
            )
            return proof_request_hash(
                original_hex="aa",
                candidate_hex="bb",
                contract="ppc-eabi",
                memory_loop_readonly=build_memory_loop_readonly_obligation(ctx),
            )

        baseline = proof_request_hash(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
        )
        first = _hash(7)
        second = _hash(9)
        # Presence of readonly evidence changes the request identity, and
        # different hydrated values change it again.
        self.assertNotEqual(baseline, first)
        self.assertNotEqual(first, second)
        self.assertEqual(first, _hash(7))


if __name__ == "__main__":
    unittest.main()

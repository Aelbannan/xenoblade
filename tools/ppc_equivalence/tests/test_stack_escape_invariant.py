"""Focused invariant tests for shared stack-pointer escape marking.

``stack_escape.mark_stack_pointer_escape`` is the single documented gate that
clears per-side private-stack masking (``stack_private``) whenever an
``r1``-derived pointer is published to memory. Both the ordinary
``execute_instruction`` store paths (``semantics``) and the summarized
memory-loop store path (``memory_semantics.apply_store_effect``) must route
through it, otherwise a store that leaks the stack pointer could hide divergent
private-frame bytes.

These tests pin two things:

1. Behaviour — storing ``input.gpr.r1`` clears ``stack_private``; storing an
   r1-independent value leaves it set.
2. Structure — the store paths import and call ``mark_stack_pointer_escape``
   (a cheap static check so a future refactor cannot silently drop the gate).
"""

from __future__ import annotations

import unittest
from pathlib import Path

from tools.ppc_equivalence.engine import _symbolic_initial
from tools.ppc_equivalence.semantics import SymbolicOps
from tools.ppc_equivalence.stack_escape import mark_stack_pointer_escape

_PPC_EQUIV_DIR = Path(__file__).resolve().parents[1]


class StackEscapeBehaviourTests(unittest.TestCase):
    def test_storing_entry_r1_clears_stack_private(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        # Precondition: masking is enabled and the entry SP is bound to the
        # tracked symbolic variable ``input.gpr.r1``.
        self.assertTrue(z3.is_true(z3.simplify(state.stack_private)))
        self.assertEqual(state.gpr[1].decl().name(), "input.gpr.r1")

        escaped = mark_stack_pointer_escape(state, state.gpr[1], ops)

        self.assertTrue(
            z3.is_false(z3.simplify(escaped.stack_private)),
            "storing input.gpr.r1 must clear stack_private",
        )

    def test_storing_r1_derived_pointer_clears_stack_private(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        # addi rD,r1,8 style computed pointer still carries r1 in its cone.
        derived = ops.add(state.gpr[1], ops.const(8))

        escaped = mark_stack_pointer_escape(state, derived, ops)

        self.assertTrue(z3.is_false(z3.simplify(escaped.stack_private)))

    def test_storing_r1_independent_value_keeps_masking(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        # r3 does not depend on r1: masking must remain enabled.
        escaped = mark_stack_pointer_escape(state, state.gpr[3], ops)

        self.assertTrue(z3.is_true(z3.simplify(escaped.stack_private)))
        self.assertIs(escaped, state)

    def test_load_derived_publish_outside_frame_clears_masking(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        loaded = ops.load_byte(state.memory, state.gpr[4])
        escaped = mark_stack_pointer_escape(
            state, loaded, ops, store_address=state.gpr[5],
        )
        self.assertTrue(
            z3.is_false(z3.simplify(escaped.stack_private)),
            "publishing a Select-derived word outside the frame must escape",
        )

    def test_load_derived_spill_to_frame_keeps_masking(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        loaded = ops.load_byte(state.memory, state.gpr[4])
        spill = ops.add(state.gpr[1], ops.const(8))
        kept = mark_stack_pointer_escape(
            state, loaded, ops, store_address=spill,
        )
        self.assertTrue(z3.is_true(z3.simplify(kept.stack_private)))
        self.assertIs(kept, state)

    def test_r1_spill_to_frame_keeps_masking(self) -> None:
        """stwu / stw r1,disp(r1) back-chain stays inside the private frame."""
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        # Effective address of stwu r1,-64(r1) before the update.
        back_chain = ops.add(state.gpr[1], ops.const(0xFFFFFFC0))
        kept = mark_stack_pointer_escape(
            state, state.gpr[1], ops, store_address=back_chain,
        )
        self.assertTrue(z3.is_true(z3.simplify(kept.stack_private)))
        self.assertIs(kept, state)

    def test_r1_publish_outside_frame_still_clears(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)
        escaped = mark_stack_pointer_escape(
            state, state.gpr[1], ops, store_address=state.gpr[5],
        )
        self.assertTrue(z3.is_false(z3.simplify(escaped.stack_private)))

    def test_escape_is_sticky_once_cleared(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        state = _symbolic_initial(ops)

        cleared = mark_stack_pointer_escape(state, state.gpr[1], ops)
        # A later r1-independent store cannot re-enable masking.
        again = mark_stack_pointer_escape(cleared, cleared.gpr[3], ops)

        self.assertIs(again, cleared)
        self.assertTrue(z3.is_false(z3.simplify(again.stack_private)))


class StackEscapeStructuralTests(unittest.TestCase):
    """Static assertions that store paths route through the shared gate."""

    def test_memory_semantics_calls_mark_stack_pointer_escape(self) -> None:
        source = (_PPC_EQUIV_DIR / "memory_semantics.py").read_text(encoding="utf-8")
        self.assertIn(
            "from tools.ppc_equivalence.stack_escape import mark_stack_pointer_escape",
            source,
        )
        self.assertIn("return mark_stack_pointer_escape(", source)
        # The escape gate must live inside apply_store_effect.
        store_effect = source.split("def apply_store_effect", 1)
        self.assertEqual(len(store_effect), 2, "apply_store_effect not found")
        body = store_effect[1].split("\ndef ", 1)[0]
        self.assertIn("mark_stack_pointer_escape(", body)

    def test_semantics_store_paths_call_mark_stack_pointer_escape(self) -> None:
        source = (_PPC_EQUIV_DIR / "semantics.py").read_text(encoding="utf-8")
        # semantics imports the shared implementation (aliased) and wraps it.
        self.assertIn(
            "from .stack_escape import mark_stack_pointer_escape", source,
        )
        # Every ordinary store path calls the wrapper.
        self.assertGreaterEqual(
            source.count("_mark_stack_pointer_escape("),
            2,
            "expected multiple store paths to call the escape gate",
        )


if __name__ == "__main__":  # pragma: no cover
    unittest.main()

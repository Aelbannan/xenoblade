"""Tests for tools/llm_decomp/brief.py."""
from __future__ import annotations

import unittest

from tools.llm_decomp.brief import build_brief
from tools.llm_decomp.contracts import Baseline, SymbolBaseline


class BuildBriefTest(unittest.TestCase):
    """Integration-style tests for the brief builder."""

    maxDiff = None

    def test_sections_order_and_headings(self) -> None:
        """All required headings appear in the correct order."""
        brief = build_brief(
            target_id="CfPadTask__Update",
            symbol="Update__Q2_2ml9CfPadTaskFv",
            demangled="Update__Q2_2ml9CfPadTaskFv",
            signature="void CfPadTask::Update()",
            unit="kyoshin/cf/CfPadTask",
            retail_asm="li r3, 0\nblr",
            writable=["src/kyoshin/cf/CfPadTask.cpp", "src/kyoshin/cf/CfPadTask.h"],
            baseline=None,
            carryover=None,
            session_type="match",
        )

        # Check every required heading appears.
        self.assertIn("# Decompilation session: match", brief)
        self.assertIn("## Target", brief)
        self.assertIn("## Retail ASM", brief)
        self_in = "## Writable scope"
        self.assertIn(self_in, brief)
        self.assertIn("## State", brief)
        self.assertIn("## Rules", brief)

        # Section order: headings should appear in source order.
        headings = [
            "# Decompilation session:",
            "## Target",
            "## Retail ASM",
            "## Writable scope",
            "## State",
            "## Rules",
        ]
        positions = [brief.index(h) for h in headings]
        self.assertEqual(positions, sorted(positions), "headings out of order")

        # Closing line.
        self.assertTrue(brief.strip().endswith(
            "Begin by reviewing the retail ASM and the current state of the target function."
        ))

    def test_locked_signature_present(self) -> None:
        """The locked signature appears in a fenced code block."""
        brief = build_brief(
            target_id="fn",
            symbol="fn__Fv",
            demangled="fn()",
            signature="void fn()",
            unit="test",
            retail_asm="blr",
            writable=["test.cpp"],
        )
        self.assertIn("```c\nvoid fn()\n```", brief)
        self.assertIn("signature is locked", brief)

    def test_carryover_absent_when_none(self) -> None:
        """No Carryover heading when carryover is None."""
        brief = build_brief(
            target_id="fn",
            symbol="fn__Fv",
            demangled="fn()",
            signature="void fn()",
            unit="test",
            retail_asm="blr",
            writable=["test.cpp"],
        )
        self.assertNotIn("## Carryover", brief)

    def test_carryover_present_when_set(self) -> None:
        """Carryover section rendered verbatim when provided."""
        carry_text = "**Previous hypothesis:** the type is `UnkClass_123`."
        brief = build_brief(
            target_id="fn",
            symbol="fn__Fv",
            demangled="fn()",
            signature="void fn()",
            unit="test",
            retail_asm="blr",
            writable=["test.cpp"],
            carryover=carry_text,
        )
        self.assertIn("## Carryover", brief)
        self.assertIn(carry_text, brief)

    def test_truncation_respects_max_chars(self) -> None:
        """Large ASM is truncated; marker appears; first and last lines survive."""
        n_lines = 5000
        lines = [f"/* line {i} */  nop" for i in range(n_lines)]
        big_asm = "\n".join(lines)

        brief = build_brief(
            target_id="fn",
            symbol="fn__Fv",
            demangled="fn()",
            signature="void fn()",
            unit="test",
            retail_asm=big_asm,
            writable=["t.cpp"],
            max_chars=2000,
        )

        # Total should be at or near 2000 chars.
        self.assertLessEqual(len(brief), 2100)

        # Contains the elision marker.
        self.assertIn("elided", brief)
        self.assertIn("# ...", brief)

        # First and last asm lines survive.
        self.assertIn(lines[0], brief)
        self.assertIn(lines[-1], brief)

    def test_truncation_no_op_when_small(self) -> None:
        """Small ASM fits without truncation."""
        small_asm = "li r3, 0\nblr"
        brief = build_brief(
            target_id="fn",
            symbol="fn__Fv",
            demangled="fn()",
            signature="void fn()",
            unit="test",
            retail_asm=small_asm,
            writable=["t.cpp"],
            max_chars=60_000,
        )
        self.assertIn("li r3, 0", brief)
        self.assertIn("blr", brief)
        # No elision marker for such a small listing.
        self.assertNotIn("elided", brief)

    def test_state_no_baseline(self) -> None:
        """"baseline pending" when baseline is None."""
        brief = build_brief(
            target_id="fn",
            symbol="fn__Fv",
            demangled="fn()",
            signature="void fn()",
            unit="test",
            retail_asm="blr",
            writable=["t.cpp"],
            baseline=None,
        )
        self.assertIn("baseline pending", brief)

    def test_state_with_baseline(self) -> None:
        """State section reports mismatch count, totals, matched count, size."""
        baseline = Baseline(
            unit="test",
            symbols={
                "fn__Fv": SymbolBaseline(
                    byte_hash="abc", mismatch_count=3, fingerprint="fp1"
                ),
                "other__Fv": SymbolBaseline(
                    byte_hash="def", mismatch_count=0, fingerprint=""
                ),
            },
            text_size=4096,
            text_budget=8192,
            object_path="build/test.o",
        )
        brief = build_brief(
            target_id="fn",
            symbol="fn__Fv",
            demangled="fn()",
            signature="void fn()",
            unit="test",
            retail_asm="blr",
            writable=["t.cpp"],
            baseline=baseline,
        )
        self.assertIn("current mismatches for `fn__Fv`: 3", brief)
        self.assertIn("total symbols in TU: 2", brief)
        self.assertIn("symbols with 0 mismatches: 1", brief)
        self.assertIn(".text size: 4096", brief)
        self.assertIn("budget: 8192", brief)

    def test_state_baseline_missing_symbol(self) -> None:
        """"not yet compiled" when target symbol absent from baseline."""
        baseline = Baseline(
            unit="test",
            symbols={},
            text_size=512,
            text_budget=None,
            object_path="build/test.o",
        )
        brief = build_brief(
            target_id="fn",
            symbol="missing__Fv",
            demangled="missing()",
            signature="void missing()",
            unit="test",
            retail_asm="blr",
            writable=["t.cpp"],
            baseline=baseline,
        )
        self.assertIn("not yet compiled", brief)
        self.assertIn("total symbols in TU: 0", brief)
        self.assertIn("symbols with 0 mismatches: 0", brief)

    def test_session_type_in_heading(self) -> None:
        """The heading reflects the session type."""
        for st in ("match", "type-recovery", "rename", "tu-cleanup", "size-trim"):
            brief = build_brief(
                target_id="fn",
                symbol="fn__Fv",
                demangled="fn()",
                signature="void fn()",
                unit="test",
                retail_asm="blr",
                writable=["t.cpp"],
                session_type=st,
            )
            self.assertIn(f"# Decompilation session: {st}", brief)

    def test_rules_listed(self) -> None:
        """All eight numbered rules are present."""
        brief = build_brief(
            target_id="fn",
            symbol="fn__Fv",
            demangled="fn()",
            signature="void fn()",
            unit="test",
            retail_asm="blr",
            writable=["t.cpp"],
        )
        for i in range(1, 10):
            self.assertIn(f"{i}.", brief)


if __name__ == "__main__":
    unittest.main()
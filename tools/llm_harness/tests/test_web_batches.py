"""Unit tests for ChatGPT Web batch packing, rendering, and parsing."""
from __future__ import annotations

import unittest

from tools.llm_harness.web_batches import (
    FENCE_POLICY,
    WebBatch,
    WebBatchFormatError,
    WebFunctionResponse,
    WebTarget,
    base_cost,
    complexity_modifier,
    difficulty_cost,
    pack_candidates,
    parse_request_metadata,
    parse_web_response,
    render_batch_request,
    render_completion_receipt,
    render_web_response,
    web_target_from_row,
)


def _target(
    target_id: str,
    *,
    cost: int | None = None,
    unit: str = "main/u",
    insn: int = 10,
    tier: int = 1,
) -> WebTarget:
    retail = insn * 4
    row = {
        "target_id": target_id,
        "unit": unit,
        "symbol": target_id,
        "source_path": f"src/{target_id}.cpp",
        "status": "STUB",
        "instruction_match": None,
        "frontier_kind": "leaf",
        "retail_text_size": retail,
        "retail_instruction_count": insn,
        "direct_call_count": 0,
        "unresolved_call_count": 0,
        "has_indirect_calls": False,
        "tier": tier,
        "workflow": "new",
    }
    target = web_target_from_row(row)
    if cost is not None:
        return WebTarget(
            **{**target.__dict__, "cost": cost, "instruction_count": insn}
        )
    return target


class CostTests(unittest.TestCase):
    def test_base_cost_breakpoints(self) -> None:
        self.assertEqual(base_cost(1), 1)
        self.assertEqual(base_cost(15), 1)
        self.assertEqual(base_cost(16), 2)
        self.assertEqual(base_cost(64), 2)
        self.assertEqual(base_cost(65), 5)
        self.assertEqual(base_cost(192), 5)
        self.assertEqual(base_cost(193), 15)
        self.assertEqual(base_cost(512), 15)
        self.assertEqual(base_cost(513), 41)

    def test_complexity_modifiers(self) -> None:
        self.assertEqual(
            complexity_modifier(direct_call_count=4, unresolved_call_count=0),
            1,
        )
        self.assertEqual(
            complexity_modifier(unresolved_call_count=3),
            4,
        )
        self.assertEqual(
            complexity_modifier(has_indirect_calls=True, frontier_kind="pending"),
            7,
        )

    def test_difficulty_cost_floor(self) -> None:
        self.assertGreaterEqual(
            difficulty_cost(retail_text_size=4, frontier_kind="leaf"),
            1,
        )


class PackingTests(unittest.TestCase):
    def test_exact_batches_and_budget(self) -> None:
        candidates = [_target(f"f{i}", cost=10, insn=20) for i in range(8)]
        batches, oversized, not_packed = pack_candidates(
            candidates, batch_count=4, budget=20
        )
        self.assertEqual(len(batches), 4)
        self.assertEqual(oversized, [])
        for batch in batches:
            self.assertTrue(batch.targets)
            self.assertLessEqual(batch.used_budget, 20)

    def test_deterministic(self) -> None:
        candidates = [
            _target("a", cost=5, unit="u1", insn=30, tier=0),
            _target("b", cost=5, unit="u1", insn=20, tier=0),
            _target("c", cost=5, unit="u2", insn=10, tier=0),
            _target("d", cost=5, unit="u2", insn=5, tier=1),
        ]
        b1, _, _ = pack_candidates(candidates, batch_count=2, budget=15)
        b2, _, _ = pack_candidates(candidates, batch_count=2, budget=15)
        self.assertEqual(
            [[t.target_id for t in b.targets] for b in b1],
            [[t.target_id for t in b.targets] for b in b2],
        )

    def test_same_tu_tiebreak_spreads(self) -> None:
        candidates = [
            _target("a", cost=5, unit="tuA", insn=40),
            _target("b", cost=5, unit="tuA", insn=30),
            _target("c", cost=5, unit="tuB", insn=20),
            _target("d", cost=5, unit="tuB", insn=10),
        ]
        batches, _, _ = pack_candidates(candidates, batch_count=2, budget=20)
        # After seeding largest into each bin, remaining prefer bins with fewer same-TU.
        units0 = {t.unit for t in batches[0].targets}
        units1 = {t.unit for t in batches[1].targets}
        self.assertTrue(units0 or units1)
        # Both batches should not be entirely the same single TU when alternatives exist.
        self.assertFalse(
            len(batches[0].targets) > 1
            and len({t.unit for t in batches[0].targets}) == 1
            and len(batches[1].targets) > 1
            and len({t.unit for t in batches[1].targets}) == 1
            and {t.unit for t in batches[0].targets}
            == {t.unit for t in batches[1].targets}
        )

    def test_oversized_skipped(self) -> None:
        candidates = [
            _target("tiny", cost=1, insn=1),
            _target("huge", cost=99, insn=2000),
            _target("ok", cost=1, insn=2),
        ]
        batches, oversized, _ = pack_candidates(candidates, batch_count=2, budget=10)
        self.assertEqual([t.target_id for t in oversized], ["huge"])
        self.assertEqual(len(batches), 2)

    def test_insufficient_eligible_raises(self) -> None:
        candidates = [_target("only", cost=1)]
        with self.assertRaises(ValueError):
            pack_candidates(candidates, batch_count=2, budget=10)

    def test_not_packed_remainder(self) -> None:
        candidates = [_target(f"f{i}", cost=8, insn=20) for i in range(5)]
        batches, _, not_packed = pack_candidates(
            candidates, batch_count=2, budget=10
        )
        # Seed 2 + maybe one more per bin (8+8 > 10) so remainder not packed.
        placed = sum(len(b.targets) for b in batches)
        self.assertEqual(placed + len(not_packed), 5)
        self.assertTrue(not_packed)


class RenderParseTests(unittest.TestCase):
    def test_initial_request_roundtrip_metadata(self) -> None:
        batch = WebBatch(batch_id="batch_001", index=1, budget_limit=50)
        batch.add(_target("func_a", cost=2, insn=10))
        text = render_batch_request(
            batch, {"func_a": "PROMPT BODY\nint x;\n"}, round_number=0, mode="initial"
        )
        meta = parse_request_metadata(text)
        self.assertEqual(meta["batch_id"], "batch_001")
        self.assertEqual(meta["round"], 0)
        self.assertEqual(meta["mode"], "initial")
        self.assertIn("<<<PROMPT>>>", text)
        self.assertIn("PROMPT BODY", text)

    def test_valid_response_and_skip(self) -> None:
        text = render_web_response(
            batch_id="batch_001",
            round_number=0,
            entries=[
                WebFunctionResponse(
                    target_id="func_a",
                    candidate_source="int func_a() { return 1; }\n",
                    hypothesis="simple return",
                    skip_reason="",
                ),
                WebFunctionResponse(
                    target_id="func_b",
                    candidate_source=None,
                    hypothesis="",
                    skip_reason="insufficient context",
                ),
            ],
        )
        parsed = parse_web_response(text)
        self.assertEqual(parsed.batch_id, "batch_001")
        self.assertEqual(parsed.round, 0)
        self.assertIn("func_a", parsed.functions)
        self.assertEqual(parsed.functions["func_a"].candidate_source.strip()[:3], "int")
        self.assertIsNone(parsed.functions["func_b"].candidate_source)
        self.assertIn("insufficient", parsed.functions["func_b"].skip_reason)

    def test_duplicate_and_fence_rejection(self) -> None:
        text = (
            "WEB_DECOMP_BATCH_RESPONSE_V1\n"
            "batch_id: batch_001\n"
            "round: 0\n\n"
            '<<<FUNCTION id="a">>>\n'
            "<<<CANDIDATE>>>\n"
            "int a(){return 0;}\n"
            "<<<END_CANDIDATE>>>\n"
            "<<<END_FUNCTION>>>\n"
            '<<<FUNCTION id="a">>>\n'
            "<<<CANDIDATE>>>\n"
            "int a(){return 1;}\n"
            "<<<END_CANDIDATE>>>\n"
            "<<<END_FUNCTION>>>\n"
            "END_WEB_DECOMP_BATCH_RESPONSE_V1\n"
        )
        with self.assertRaises(WebBatchFormatError):
            parse_web_response(text)

        fenced = render_web_response(
            batch_id="batch_001",
            round_number=0,
            entries=[
                WebFunctionResponse(
                    target_id="a",
                    candidate_source="```cpp\nint a(){return 0;}\n```\n",
                    hypothesis="",
                    skip_reason="",
                )
            ],
        )
        with self.assertRaises(WebBatchFormatError) as ctx:
            parse_web_response(fenced)
        self.assertIn(FENCE_POLICY, str(ctx.exception))

    def test_sentinel_inside_source_preserved(self) -> None:
        source = (
            "int f() {\n"
            "  // <<<END_CANDIDATE>>> looks like a sentinel but is a comment\n"
            "  return 0;\n"
            "}\n"
        )
        text = render_web_response(
            batch_id="batch_001",
            round_number=0,
            entries=[
                WebFunctionResponse(
                    target_id="f",
                    candidate_source=source,
                    hypothesis="note",
                    skip_reason="",
                )
            ],
        )
        parsed = parse_web_response(text)
        self.assertIn("<<<END_CANDIDATE>>>", parsed.functions["f"].candidate_source or "")

    def test_crlf_normalization(self) -> None:
        text = render_web_response(
            batch_id="batch_001",
            round_number=1,
            entries=[
                WebFunctionResponse(
                    target_id="f",
                    candidate_source="void f(){}\n",
                    hypothesis="h",
                    skip_reason="",
                )
            ],
        ).replace("\n", "\r\n")
        parsed = parse_web_response(text)
        self.assertEqual(parsed.round, 1)
        self.assertTrue((parsed.functions["f"].candidate_source or "").endswith("\n"))

    def test_empty_candidate_rejected(self) -> None:
        text = (
            "WEB_DECOMP_BATCH_RESPONSE_V1\n"
            "batch_id: batch_001\n"
            "round: 0\n\n"
            '<<<FUNCTION id="a">>>\n'
            "<<<CANDIDATE>>>\n"
            "\n"
            "<<<END_CANDIDATE>>>\n"
            "<<<END_FUNCTION>>>\n"
            "END_WEB_DECOMP_BATCH_RESPONSE_V1\n"
        )
        with self.assertRaises(WebBatchFormatError):
            parse_web_response(text)

    def test_completion_receipt(self) -> None:
        text = render_completion_receipt(
            batch_id="batch_001",
            completed_round=2,
            original_function_count=3,
            full_match_count=2,
            equivalent_match_count=1,
            history_path="web_batches/history/batch_001",
        )
        self.assertIn("WEB_DECOMP_BATCH_COMPLETE_V1", text)
        self.assertIn(f"fence_policy: {FENCE_POLICY}", text)
        meta = parse_request_metadata(text)
        self.assertTrue(meta["complete"])


if __name__ == "__main__":
    unittest.main()

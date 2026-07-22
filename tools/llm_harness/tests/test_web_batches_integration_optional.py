"""Optional real-toolchain web-batch end-to-end tests.

Skipped unless XENOBLADE_HARNESS_GOLDEN=1 and the MWCC/retail tree is present.
"""
from __future__ import annotations

import os
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]


def _toolchain_ready() -> bool:
    if os.environ.get("XENOBLADE_HARNESS_GOLDEN") != "1":
        return False
    return (
        (ROOT / "coop.json").is_file()
        and (ROOT / "llm-harness.json").is_file()
        and (ROOT / "orig" / "us").is_dir()
        and (ROOT / "build" / "compilers").is_dir()
    )


@unittest.skipUnless(
    _toolchain_ready(),
    "set XENOBLADE_HARNESS_GOLDEN=1 with MWCC/retail available",
)
class TestWebBatchesRealToolchain(unittest.TestCase):
    def test_export_one_small_target_dry_run(self) -> None:
        from tools.llm_harness.core import Harness
        from tools.llm_harness.web_batches import create_web_batches

        harness = Harness(ROOT / "llm-harness.json")
        result = create_web_batches(
            harness,
            batch_count=1,
            budget=50,
            selection="ready",
            certified_funcs=True,
            dry_run=True,
        )
        self.assertGreaterEqual(result.selected_functions, 1)
        self.assertEqual(len(result.batches), 1)

    def test_ingest_known_full_match_source_optional(self) -> None:
        """Evaluate a known FULL_MATCH body through evaluate_external_candidate."""
        from tools.coop.lib.targets import load_targets
        from tools.llm_harness.core import Harness
        from tools.llm_harness.web_batches import (
            WebFunctionResponse,
            render_web_response,
        )
        from tools.llm_harness import xenoblade_project as xp
        from tools.llm_harness.xenoblade_project import create_adapter

        harness = Harness(ROOT / "llm-harness.json")
        adapter = create_adapter(ROOT, {"coop_config": "coop.json"})
        accepted = [
            t
            for t in load_targets(adapter.config)
            if t.status == "FULL_MATCH"
            and t.buildable
            and t.symbol
            and t.source
            and t.source.is_file()
        ][:5]
        if not accepted:
            self.skipTest("no FULL_MATCH targets available for golden ingest")

        target = accepted[0]
        assert target.source is not None
        source_text = target.source.read_text(encoding="utf-8")
        region = xp._find_function_region(source_text, target)
        body = source_text[region.content_start : region.content_end].strip() + "\n"

        evaluation = harness.evaluate_external_candidate(
            target.id, body, hypothesis="golden full-match body"
        )
        status = str(getattr(evaluation, "status", "")).upper()
        self.assertIn(
            status,
            {"FULL_MATCH", "EQUIVALENT_MATCH", "CODE_MATCH", "COMPILES"},
        )
        rendered = render_web_response(
            batch_id="batch_001",
            round_number=0,
            entries=[
                WebFunctionResponse(
                    target_id=target.id,
                    candidate_source=body,
                    hypothesis="golden",
                    skip_reason="",
                )
            ],
        )
        self.assertIn("WEB_DECOMP_BATCH_RESPONSE_V1", rendered)


if __name__ == "__main__":
    unittest.main()

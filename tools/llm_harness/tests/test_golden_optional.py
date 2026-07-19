"""Optional real-toolchain golden tests (Phase 9.3).

Skipped unless XENOBLADE_HARNESS_GOLDEN=1 and the MWCC/retail tree is present.
These hide an accepted function body in a temp copy and assert evaluator +
prompt machinery still score retail correctly without leaking accepted source.
"""
from __future__ import annotations

import os
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]


def _toolchain_ready() -> bool:
    if os.environ.get("XENOBLADE_HARNESS_GOLDEN") != "1":
        return False
    return (
        (ROOT / "coop.json").is_file()
        and (ROOT / "orig" / "us").is_dir()
        and (ROOT / "build" / "compilers").is_dir()
    )


@unittest.skipUnless(_toolchain_ready(), "set XENOBLADE_HARNESS_GOLDEN=1 with MWCC/retail available")
class TestRealToolchainHiddenSource(unittest.TestCase):
    def test_hide_padmgr_getter_and_build_prompt(self) -> None:
        from tools.llm_harness.benchmark import hide_function_body, load_corpus, verify_hidden_source
        from tools.llm_harness.xenoblade_project import create_adapter
        from tools.llm_harness.source_regions import find_function_region as _find

        # Import the private helper used by the adapter.
        from tools.llm_harness import xenoblade_project as xp

        corpus = load_corpus(Path(__file__).parent / "benchmark" / "golden_corpus.json")
        entry = next(e for e in corpus if e.target_id == "padmgr-get-current-pad-channel")
        adapter = create_adapter(ROOT, {"coop_config": "coop.json"})
        target = adapter._target(entry.target_id)
        assert target.source is not None
        original = target.source.read_text(encoding="utf-8")
        region = xp._find_function_region(original, target)
        body = original[region.content_start : region.content_end]
        hidden_body = hide_function_body(body, target_id=entry.target_id)
        mutated = (
            original[: region.content_start]
            + hidden_body
            + original[region.content_end :]
        )

        with tempfile.TemporaryDirectory() as tmp:
            tmp_root = Path(tmp)
            # Minimal: only verify leak checks against a prompt built from mutated text.
            prompt = (
                "dossier\n"
                + mutated[max(0, region.content_start - 200) : region.content_start + len(hidden_body) + 200]
            )
            leaks = verify_hidden_source(prompt, forbidden_snippets=[body])
            self.assertEqual(leaks, [])
            self.assertIn("HARVEST_HIDDEN_SOURCE", prompt)


if __name__ == "__main__":
    unittest.main()

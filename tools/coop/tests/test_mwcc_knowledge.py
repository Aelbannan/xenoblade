from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.mwcc_knowledge import (
    build_database,
    connect,
    database_is_fresh,
    get_entry,
    parse_reference,
    search,
    stats,
)


REFERENCE = """# Intro

## Patterns that work

### Retail SDA name — `CThing::getInstance`

- **Symptom:** relocation mismatch at an SDA load.
- **Fix:** use `extern "C" lbl_eu_80660000`.
- **Result:** FULL_MATCH at 100%.

An older failed form reached only 81.2%.

### Register coloring — `CThing::Move`

- **Symptom:** Chaitin regalloc swaps r4 and r5.
- **Result:** CODE_MATCH ~98.2%.
"""


class MwccKnowledgeTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.reference = self.root / "MWCC_REFERENCE.md"
        self.attempts = self.root / "attempts.jsonl"
        self.database = self.root / "knowledge.sqlite"
        self.reference.write_text(REFERENCE, encoding="utf-8")
        self.attempts.write_text(
            json.dumps(
                {
                    "target_id": "thing-move",
                    "function": "CThing::Move",
                    "symbol": "Move__6CThingFv",
                    "status": "HIGH_MATCH",
                    "instruction_match": 88.0,
                    "hypothesis": "wrong stack frame size",
                    "next_change": "reduce live ranges",
                }
            )
            + "\n",
            encoding="utf-8",
        )

    def tearDown(self) -> None:
        self.temp.cleanup()

    def test_parses_heading_records_with_evidence(self) -> None:
        entries = list(parse_reference(self.reference))
        sda = next(entry for entry in entries if entry.title.startswith("Retail SDA"))
        self.assertEqual(sda.status, "FULL_MATCH")
        self.assertEqual(sda.match_percent, 100.0)
        self.assertIn("relocation", sda.tags)
        self.assertIn("CThing::getInstance", sda.functions)

    def test_reference_id_survives_unrelated_line_insertion(self) -> None:
        before = {
            entry.title: entry.id for entry in parse_reference(self.reference)
        }
        self.reference.write_text("extra preamble\n\n" + REFERENCE, encoding="utf-8")
        after = {
            entry.title: entry.id for entry in parse_reference(self.reference)
        }
        self.assertEqual(before, after)

    def test_build_search_show_and_stats(self) -> None:
        count = build_database(self.database, self.reference, self.attempts, root=self.root)
        self.assertEqual(count, 4)
        self.assertTrue(database_is_fresh(self.database, [self.reference, self.attempts]))
        with connect(self.database) as connection:
            rows = search(connection, "SDA relocation", tag="relocation")
            self.assertEqual(len(rows), 1)
            self.assertEqual(rows[0]["status"], "FULL_MATCH")
            self.assertIsNotNone(get_entry(connection, rows[0]["id"]))
            summary = stats(connection)
            self.assertEqual(summary["by_kind"], {"attempt": 1, "reference": 3})
            self.assertGreaterEqual(summary["by_tag"]["regalloc"], 1)

    def test_attempts_are_searchable(self) -> None:
        build_database(self.database, self.reference, self.attempts, root=self.root)
        with connect(self.database) as connection:
            rows = search(connection, "stack frame", source_kind="attempt")
            self.assertEqual(len(rows), 1)
            self.assertEqual(rows[0]["target_id"], "thing-move")

    def test_any_mode_broadens_recall(self) -> None:
        build_database(self.database, self.reference, self.attempts, root=self.root)
        with connect(self.database) as connection:
            self.assertEqual(search(connection, "SDA Chaitin"), [])
            rows = search(connection, "SDA Chaitin", mode="any")
            self.assertGreaterEqual(len(rows), 2)


if __name__ == "__main__":
    unittest.main()

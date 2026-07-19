from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.decomp_atlas.lib.config import db_path, index_state_path
from tools.decomp_atlas.lib.database import open_db
from tools.decomp_atlas.lib.indexer import run_index
from tools.decomp_atlas.tests import PROJECT


class IndexerTests(unittest.TestCase):
    def test_fast_index_and_vectors(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            # Copy fixture into an isolated root so build/ writes stay local.
            import shutil

            root = Path(tmp) / "project"
            shutil.copytree(PROJECT, root)
            result = run_index(root, full=True, vectors=True, config_path=root / "coop.json")
            self.assertTrue(result.get("ok"), result)
            self.assertGreaterEqual(result["counts"]["functions"], 7)
            self.assertGreater(result["counts"]["edges"], 0)
            self.assertGreater(result["counts"]["attempts"], 0)
            self.assertGreater(result["counts"]["experiments"], 0)
            self.assertGreater(result["counts"]["projection"], 0)
            self.assertTrue(db_path(root, "us").is_file())
            self.assertTrue(index_state_path(root, "us").is_file())

            conn = open_db(db_path(root, "us"), readonly=True)
            readiness = {
                row["target_id"]: row["readiness"]
                for row in conn.execute("SELECT target_id, readiness FROM functions")
            }
            self.assertEqual(readiness["demo-accepted"], "accepted")
            self.assertEqual(readiness["demo-leaf"], "safe_leaf")
            self.assertEqual(readiness["demo-callees-ready"], "callees_accepted")
            self.assertEqual(readiness["demo-blocked-unresolved"], "blocked_unresolved")
            self.assertEqual(readiness["demo-blocked-indirect"], "blocked_indirect")
            self.assertEqual(readiness["demo-ctor"], "claimed")
            self.assertEqual(readiness["demo-func-placeholder"], "not_buildable")

            artifact = conn.execute(
                "SELECT cpp_source, warnings_json FROM artifacts WHERE target_id = ?",
                ("demo-leaf",),
            ).fetchone()
            self.assertIsNotNone(artifact)
            self.assertIn("gDummy", artifact["cpp_source"])
            conn.close()

    def test_preserves_db_on_failure(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            import shutil

            root = Path(tmp) / "project"
            shutil.copytree(PROJECT, root)
            first = run_index(root, config_path=root / "coop.json")
            self.assertTrue(first.get("ok"))
            db = db_path(root, "us")
            before = db.read_bytes()
            # Break targets.json to force failure.
            (root / "tools" / "coop" / "targets.json").write_text("{not-json", encoding="utf-8")
            second = run_index(root, config_path=root / "coop.json")
            self.assertFalse(second.get("ok"))
            self.assertTrue(second.get("preserved_db"))
            self.assertEqual(db.read_bytes(), before)


if __name__ == "__main__":
    unittest.main()

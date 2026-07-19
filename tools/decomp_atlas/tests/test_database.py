from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.decomp_atlas.lib.database import (
    atomic_replace,
    file_sha256,
    get_meta,
    open_db,
    set_meta,
    text_sha256,
)


class DatabaseTests(unittest.TestCase):
    def test_schema_and_meta(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            db = Path(tmp) / "atlas.db"
            conn = open_db(db)
            set_meta(conn, "region", "us")
            conn.commit()
            self.assertEqual(get_meta(conn, "region"), "us")
            conn.execute(
                "INSERT INTO functions(target_id, display_name, status, workflow_status, readiness, buildable) "
                "VALUES ('a', 'A', 'NOT_STARTED', 'BACKLOG', 'backlog', 1)"
            )
            conn.commit()
            row = conn.execute("SELECT target_id FROM functions").fetchone()
            self.assertEqual(row["target_id"], "a")
            conn.close()

    def test_atomic_replace_and_hash(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src = root / "a.tmp"
            dest = root / "a.txt"
            src.write_text("hello", encoding="utf-8")
            atomic_replace(src, dest)
            self.assertTrue(dest.is_file())
            self.assertFalse(src.exists())
            self.assertEqual(file_sha256(dest), text_sha256("hello"))
            self.assertIsNone(file_sha256(root / "missing"))


if __name__ == "__main__":
    unittest.main()

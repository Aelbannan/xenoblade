"""CLI tests for web-export / web-ingest."""
from __future__ import annotations

import io
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path
from unittest.mock import MagicMock, patch

from tools.llm_harness.run import main
from tools.llm_harness.web_batches import (
    EXIT_FORMAT,
    EXIT_OK,
    EXIT_STALE,
    FENCE_POLICY,
    WebBatchCreateResult,
    WebBatchIngestResult,
    WebBatch,
)


class WebBatchCliTests(unittest.TestCase):
    def test_web_export_requires_batches(self) -> None:
        with redirect_stderr(io.StringIO()) as err:
            with self.assertRaises(SystemExit) as ctx:
                main(["web-export"])
        self.assertNotEqual(ctx.exception.code, 0)
        self.assertIn("--batches", err.getvalue())

    def test_web_export_rejects_nonpositive(self) -> None:
        harness = MagicMock()
        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with patch(
                "tools.llm_harness.web_batches.create_web_batches",
                side_effect=ValueError("--batches must be >= 1"),
            ):
                with redirect_stdout(io.StringIO()), redirect_stderr(io.StringIO()):
                    code = main(["web-export", "--batches", "0"])
        self.assertEqual(code, EXIT_FORMAT)

    def test_web_export_success_output(self) -> None:
        harness = MagicMock()
        batch = WebBatch(batch_id="batch_001", index=1, budget_limit=50)
        result = WebBatchCreateResult(
            output_dir=Path("web_batches"),
            batch_count=1,
            budget=50,
            selected_functions=2,
            skipped_oversized=0,
            not_packed=0,
            batches=[batch],
        )
        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with patch(
                "tools.llm_harness.web_batches.create_web_batches",
                return_value=result,
            ):
                with redirect_stdout(io.StringIO()) as out:
                    code = main(["web-export", "--batches", "1", "--dry-run"])
        self.assertEqual(code, EXIT_OK)
        self.assertIn("web export", out.getvalue())
        self.assertIn("batches=1", out.getvalue())

    def test_web_ingest_missing_file(self) -> None:
        harness = MagicMock()
        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with tempfile.TemporaryDirectory() as tmp:
                missing = Path(tmp) / "nope.response.txt"
                with redirect_stderr(io.StringIO()) as err:
                    code = main(["web-ingest", str(missing)])
        self.assertEqual(code, EXIT_FORMAT)
        self.assertIn("web-ingest error", err.getvalue())

    def test_web_ingest_stale_exit_code(self) -> None:
        from tools.llm_harness.web_batches import WebBatchStaleError

        harness = MagicMock()
        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with patch(
                "tools.llm_harness.web_batches.ingest_web_batch_output",
                side_effect=WebBatchStaleError("stale response round"),
            ):
                with tempfile.TemporaryDirectory() as tmp:
                    path = Path(tmp) / "r.txt"
                    path.write_text("x", encoding="utf-8")
                    with redirect_stderr(io.StringIO()):
                        code = main(["web-ingest", str(path)])
        self.assertEqual(code, EXIT_STALE)

    def test_web_ingest_prints_fence_policy(self) -> None:
        harness = MagicMock()
        result = WebBatchIngestResult(
            batch_id="batch_001",
            round=0,
            response_sha256="abc",
            results=[],
            active_file=Path("web_batches/batch_001.txt"),
            history_dir=Path("web_batches/history/batch_001/round_000"),
            next_round=1,
            exit_code=EXIT_OK,
            summary={
                "batch_id": "batch_001",
                "round": 0,
                "submitted": 1,
                "full_match": 0,
                "equivalent_match": 0,
                "unresolved": 1,
                "compile_error": 0,
                "missing_response": 0,
                "promotion_failed": 0,
                "fence_policy": FENCE_POLICY,
            },
        )
        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with patch(
                "tools.llm_harness.web_batches.ingest_web_batch_output",
                return_value=result,
            ):
                with tempfile.TemporaryDirectory() as tmp:
                    path = Path(tmp) / "r.txt"
                    path.write_text("x", encoding="utf-8")
                    with redirect_stdout(io.StringIO()) as out:
                        code = main(["web-ingest", str(path)])
        self.assertEqual(code, EXIT_OK)
        self.assertIn(FENCE_POLICY, out.getvalue())


if __name__ == "__main__":
    unittest.main()

"""Schema, hashing, and grading-soundness tests for the ``gx_fifo_v1`` corpora.

These tests exist to prove ``gx_fifo_corpus.py`` is not a rubber stamp: a
forged/mutated ``expected`` value in any corpus row must make replay fail,
the on-disk corpus must actually pass ``--check`` today, and
``corpus_sha256`` must be a stable, content-sensitive digest (see
``corpora/gx_fifo_v1/README.md`` "Independence rule").
"""

from __future__ import annotations

import copy
import json
import unittest
from pathlib import Path

from tools.ppc_equivalence.gx_fifo_corpus import (
    CORPUS_DIR,
    CORPUS_FILES,
    check_corpora,
    compute_gx_fifo_v1_corpus_sha256,
    load_all_corpora,
    load_corpus_file,
    replay_row,
    validate_corpus_header,
    validate_corpus_row,
)


class LoadAllCorporaTests(unittest.TestCase):
    def test_loads_every_known_file_with_header_and_rows(self) -> None:
        loaded = load_all_corpora()
        self.assertEqual(set(loaded.keys()), set(CORPUS_FILES))
        for name, (header, rows) in loaded.items():
            self.assertIsNone(validate_corpus_header(header, corpus_file=name))
            self.assertTrue(rows, f"{name} has no data rows")
            for row in rows:
                self.assertIsNone(validate_corpus_row(row, corpus_file=name))

    def test_manifest_row_counts_match_on_disk_rows(self) -> None:
        manifest = json.loads((CORPUS_DIR / "manifest.json").read_text(encoding="utf-8"))
        loaded = load_all_corpora()
        for name, (_, rows) in loaded.items():
            self.assertEqual(
                manifest["row_counts"][name],
                len(rows),
                f"{name}: manifest row_counts stale",
            )

    def test_manifest_corpus_sha256_matches_recomputed(self) -> None:
        manifest = json.loads((CORPUS_DIR / "manifest.json").read_text(encoding="utf-8"))
        self.assertEqual(manifest["corpus_sha256"], compute_gx_fifo_v1_corpus_sha256())


class SchemaHeaderValidationTests(unittest.TestCase):
    def test_wrong_type_rejected(self) -> None:
        header, _ = load_corpus_file(CORPUS_DIR / "write_trace.jsonl")
        mutated = dict(header)
        mutated["type"] = "not-the-right-type"
        reason = validate_corpus_header(mutated, corpus_file="write_trace.jsonl")
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("header.type", reason)

    def test_wrong_schema_version_rejected(self) -> None:
        header, _ = load_corpus_file(CORPUS_DIR / "read_policy.jsonl")
        mutated = dict(header)
        mutated["schema_version"] = 2
        reason = validate_corpus_header(mutated, corpus_file="read_policy.jsonl")
        self.assertIsNotNone(reason)

    def test_wrong_capability_rejected(self) -> None:
        header, _ = load_corpus_file(CORPUS_DIR / "loop_emission.jsonl")
        mutated = dict(header)
        mutated["capability"] = "gx-fifo-write-trace"
        reason = validate_corpus_header(mutated, corpus_file="loop_emission.jsonl")
        self.assertIsNotNone(reason)

    def test_wrong_hardware_profile_rejected(self) -> None:
        header, _ = load_corpus_file(CORPUS_DIR / "write_trace.jsonl")
        mutated = dict(header)
        mutated["hardware_profile"] = "wii-broadway-xenoblade-us-v1"
        reason = validate_corpus_header(mutated, corpus_file="write_trace.jsonl")
        self.assertIsNotNone(reason)

    def test_disallowed_oracle_rejected(self) -> None:
        header, _ = load_corpus_file(CORPUS_DIR / "write_trace.jsonl")
        mutated = dict(header)
        mutated["oracle"] = "device-model-generated"
        reason = validate_corpus_header(mutated, corpus_file="write_trace.jsonl")
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("oracle", reason)


class SchemaRowValidationTests(unittest.TestCase):
    def test_row_missing_id_rejected(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "write_trace.jsonl")
        mutated = dict(rows[0])
        del mutated["id"]
        self.assertIsNotNone(validate_corpus_row(mutated, corpus_file="write_trace.jsonl"))

    def test_row_capability_mismatch_rejected(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "read_policy.jsonl")
        mutated = dict(rows[0])
        mutated["capability"] = "mmio-loop-emission"
        self.assertIsNotNone(validate_corpus_row(mutated, corpus_file="read_policy.jsonl"))

    def test_row_missing_expected_rejected(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "loop_emission.jsonl")
        mutated = dict(rows[0])
        del mutated["expected"]
        self.assertIsNotNone(validate_corpus_row(mutated, corpus_file="loop_emission.jsonl"))

    def test_loop_row_unknown_shape_rejected(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "loop_emission.jsonl")
        mutated = copy.deepcopy(rows[0])
        mutated["program"]["shape"] = "totally-invented-shape"
        self.assertIsNotNone(validate_corpus_row(mutated, corpus_file="loop_emission.jsonl"))

    def test_write_row_empty_ops_rejected(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "write_trace.jsonl")
        mutated = dict(rows[0])
        mutated["ops"] = []
        self.assertIsNotNone(validate_corpus_row(mutated, corpus_file="write_trace.jsonl"))


class CorpusSha256StabilityTests(unittest.TestCase):
    def test_stable_across_calls(self) -> None:
        first = compute_gx_fifo_v1_corpus_sha256()
        second = compute_gx_fifo_v1_corpus_sha256()
        self.assertEqual(first, second)
        self.assertEqual(len(first), 64)

    def test_sensitive_to_content_changes(self) -> None:
        import tempfile

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for name in CORPUS_FILES:
                (root / name).write_text(
                    (CORPUS_DIR / name).read_text(encoding="utf-8"), encoding="utf-8",
                )
            baseline = compute_gx_fifo_v1_corpus_sha256(corpus_dir=root)

            # Flip one byte in one data row of one file.
            target = root / "write_trace.jsonl"
            text = target.read_text(encoding="utf-8")
            mutated_text = text.replace('"0xab"', '"0xac"', 1)
            self.assertNotEqual(text, mutated_text, "fixture text did not contain expected marker")
            target.write_text(mutated_text, encoding="utf-8")

            mutated_digest = compute_gx_fifo_v1_corpus_sha256(corpus_dir=root)
            self.assertNotEqual(baseline, mutated_digest)


class ModelMatchesOnDiskCorpusTests(unittest.TestCase):
    def test_check_corpora_passes_today(self) -> None:
        report = check_corpora()
        self.assertTrue(report.passed, report.schema_errors or [r.reason for r in report.row_results if not r.passed])
        self.assertEqual(report.failed_rows, 0)
        self.assertGreaterEqual(report.total_rows, 38)
        self.assertEqual(len(report.corpus_sha256), 64)


class ReadPolicyFailClosedAcrossCorpusTests(unittest.TestCase):
    def test_every_read_policy_row_declares_only_fail_closed_outcomes(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "read_policy.jsonl")
        allowed = {"unsupported", "misaligned", "ok"}  # "ok" only ever appears for a write op
        for row in rows:
            for op in row["ops"]:
                self.assertIn(op["expected_outcome"], allowed)
                if op["kind"] == "read":
                    self.assertIn(op["expected_outcome"], {"unsupported", "misaligned"})

    def test_replaying_every_read_policy_row_matches_the_real_device(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "read_policy.jsonl")
        for row in rows:
            with self.subTest(row_id=row["id"]):
                result = replay_row(row, corpus_file="read_policy.jsonl")
                self.assertTrue(result.passed, result.reason)


class GraderRejectsForgedExpectationsTests(unittest.TestCase):
    """The core independence-preserving property: a corpus row whose
    ``expected`` block disagrees with the real model must fail replay."""

    def test_forged_write_final_cursor_fails(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "write_trace.jsonl")
        row = next(r for r in rows if r["id"] == "write-repeated-three")
        forged = copy.deepcopy(row)
        forged["expected"]["final_cursor"] = 999
        result = replay_row(forged, corpus_file="write_trace.jsonl")
        self.assertFalse(result.passed)
        self.assertIn("final_cursor", result.reason)

    def test_forged_write_event_value_fails(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "write_trace.jsonl")
        row = next(r for r in rows if r["id"] == "write-width4-single")
        forged = copy.deepcopy(row)
        forged["expected"]["events"][0]["value"] = "0xffffffff"
        result = replay_row(forged, corpus_file="write_trace.jsonl")
        self.assertFalse(result.passed)
        self.assertIn("value", result.reason)

    def test_forged_write_op_outcome_fails(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "write_trace.jsonl")
        row = next(r for r in rows if r["id"] == "write-misaligned-width2")
        forged = copy.deepcopy(row)
        forged["ops"][0]["expected_outcome"] = "ok"
        result = replay_row(forged, corpus_file="write_trace.jsonl")
        self.assertFalse(result.passed)
        self.assertIn("outcomes", result.reason)

    def test_forged_read_cursor_delta_fails(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "read_policy.jsonl")
        row = next(r for r in rows if r["id"] == "read-repeated-same-address")
        forged = copy.deepcopy(row)
        forged["expected"]["cursor_delta"] = 3
        result = replay_row(forged, corpus_file="read_policy.jsonl")
        self.assertFalse(result.passed)
        self.assertIn("cursor_delta", result.reason)

    def test_forged_loop_trip_count_fails(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "loop_emission.jsonl")
        row = next(r for r in rows if r["id"] == "loop-invariant-stw-count1")
        forged = copy.deepcopy(row)
        forged["expected"]["trip_count"] = 42
        result = replay_row(forged, corpus_file="loop_emission.jsonl")
        self.assertFalse(result.passed)
        self.assertIn("trip_count", result.reason)

    def test_forged_loop_candidate_found_fails(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "loop_emission.jsonl")
        row = next(r for r in rows if r["id"] == "loop-extra-hidden-write")
        forged = copy.deepcopy(row)
        forged["expected"]["candidate_found"] = True
        result = replay_row(forged, corpus_file="loop_emission.jsonl")
        self.assertFalse(result.passed)
        self.assertIn("candidate_found", result.reason)

    def test_forged_loop_discharge_status_fails(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "loop_emission.jsonl")
        row = next(r for r in rows if r["id"] == "loop-invariant-stw-count1")
        forged = copy.deepcopy(row)
        forged["expected"]["discharge_status"] = "failed"
        result = replay_row(forged, corpus_file="loop_emission.jsonl")
        self.assertFalse(result.passed)
        self.assertIn("discharge_status", result.reason)


if __name__ == "__main__":
    unittest.main()

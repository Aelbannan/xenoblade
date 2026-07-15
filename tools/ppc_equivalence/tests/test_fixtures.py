from __future__ import annotations

import subprocess
import sys
import unittest
from pathlib import Path

from tools.ppc_equivalence.fixtures.corpus import FIXTURES, export_jsonl, load_fixtures
from tools.ppc_equivalence.fixtures.runner import compare_fixture
from tools.ppc_equivalence.gen_fixture_blob import (
    DEFAULT_HEADER,
    DEFAULT_JSONL,
    DEFAULT_PAYLOADS,
    generate_header,
    generate_payloads,
)

ROOT = Path(__file__).resolve().parents[3]


class FixtureCorpusTests(unittest.TestCase):
    def test_all_fixtures_match_concrete_ops(self) -> None:
        failures = []
        for case in FIXTURES:
            mismatches = compare_fixture(case)
            if mismatches:
                failures.append(f"{case.id}: {', '.join(mismatches)}")
        self.assertEqual(failures, [], msg="\n".join(failures))

    def test_jsonl_round_trip(self) -> None:
        path = ROOT / "build" / "ppc-equivalence" / "broadway.roundtrip.jsonl"
        export_jsonl(path)
        loaded = load_fixtures(path)
        self.assertEqual([case.id for case in loaded], [case.id for case in FIXTURES])
        for left, right in zip(loaded, FIXTURES, strict=True):
            self.assertEqual(left.code_words, right.code_words)
            self.assertEqual(left.expected_result, right.expected_result)

    def test_generated_header_and_jsonl_are_fresh(self) -> None:
        self.assertTrue(DEFAULT_HEADER.is_file(), "missing generated fixture header")
        self.assertTrue(DEFAULT_PAYLOADS.is_file(), "missing generated fixture payloads")
        self.assertTrue(DEFAULT_JSONL.is_file(), "missing generated broadway.jsonl")
        self.assertEqual(DEFAULT_HEADER.read_text(encoding="utf-8"), generate_header())
        self.assertEqual(DEFAULT_PAYLOADS.read_text(encoding="utf-8"), generate_payloads())
        export_jsonl(ROOT / "build" / "ppc-equivalence" / "broadway.check.jsonl")
        self.assertEqual(
            DEFAULT_JSONL.read_text(encoding="utf-8"),
            (ROOT / "build" / "ppc-equivalence" / "broadway.check.jsonl").read_text(encoding="utf-8"),
        )

    def test_gen_fixture_blob_check_passes(self) -> None:
        proc = subprocess.run(
            [sys.executable, str(ROOT / "tools" / "ppc_equivalence" / "gen_fixture_blob.py"), "--check"],
            cwd=ROOT,
            capture_output=True,
            text=True,
            check=False,
        )
        self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)


if __name__ == "__main__":
    unittest.main()

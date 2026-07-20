"""Phase 11 scalar FP v2 corpus replay and mutation-killer tests."""

from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.capability_assurance import STATUS_INCOMPLETE
from tools.ppc_equivalence.fp_fused_obligations import build_fp_fused_obligation_v2
from tools.ppc_equivalence.fp_scalar_obligations_v2 import (
    FUSED_V2_DIMENSIONS,
    evaluate_fp_scalar_obligation_v2_status,
    live_scalar_fp_v2_corpus_sha256,
    recompute_obligation_digests,
    validate_fp_scalar_obligation_v2,
)
from tools.ppc_equivalence.scalar_fp_v2_corpus import (
    CORPUS_DIR,
    CORPUS_FILES,
    check_corpora,
    compute_scalar_fp_v2_corpus_sha256,
    load_corpus_file,
    replay_corpora,
    replay_row,
    validate_corpus_header,
    validate_corpus_row,
)

REQUIRED_CORPUS_MUTATIONS = (
    "schema_version",
    "corpus_id",
    "capability",
    "expected_bits",
    "expected_fpscr",
    "provenance",
    "operands",
)


class ScalarFPV2CorpusTests(unittest.TestCase):
    def test_corpus_files_exist_with_schema_header(self) -> None:
        for name in CORPUS_FILES:
            with self.subTest(file=name):
                path = CORPUS_DIR / name
                self.assertTrue(path.is_file(), msg=str(path))
                lines = path.read_text(encoding="utf-8").splitlines()
                self.assertGreaterEqual(len(lines), 2)
                header = json.loads(lines[0])
                self.assertEqual(header.get("type"), "scalar_fp_v2_corpus")
                self.assertEqual(header.get("schema_version"), 1)
                self.assertGreaterEqual(len(lines) - 1, 5, msg=f"{name} needs >=5 rows")

    def test_interim_rows_marked_allowed_provenance(self) -> None:
        allowed = {
            "oracle_rne_interim",
            "exact_kernel_v2",
            "fixtures-broadway-jsonl",
            "dolphin-capture",
        }
        for name in CORPUS_FILES:
            path = CORPUS_DIR / name
            for line in path.read_text(encoding="utf-8").splitlines()[1:]:
                row = json.loads(line)
                provenance = row.get("provenance", "")
                self.assertIn(provenance, allowed, msg=f"{name}:{row.get('id')}")

    def test_live_replay_passes_all_rows(self) -> None:
        report = check_corpora()
        if not report.passed:
            failures = [
                f"{item.corpus_file}:{item.row_id}:{item.reason}"
                for item in report.row_results
                if not item.passed
            ]
            self.fail(
                "corpus replay failed: "
                + "; ".join(report.schema_errors + failures),
            )
        self.assertGreaterEqual(report.total_rows, 30)

    def test_corpus_sha256_is_stable_and_live(self) -> None:
        digest = compute_scalar_fp_v2_corpus_sha256()
        self.assertEqual(len(digest), 64)
        self.assertEqual(digest, live_scalar_fp_v2_corpus_sha256())

    def test_obligation_rejects_stale_corpus_sha256(self) -> None:
        live = live_scalar_fp_v2_corpus_sha256()
        obl = build_fp_fused_obligation_v2(
            opcodes=["fmadd"],
            dimensions={key: True for key in FUSED_V2_DIMENSIONS},
            corpus_sha256="0" * 64,
        )
        ok, reasons = recompute_obligation_digests(obl, live_corpus_sha256=live)
        self.assertFalse(ok)
        self.assertIn("corpus_sha256 mismatch", reasons)

    def test_mutation_killer_schema_version_fails_closed(self) -> None:
        path = CORPUS_DIR / "fused_residual.jsonl"
        header, rows = load_corpus_file(path)
        bad_header = copy.deepcopy(header)
        bad_header["schema_version"] = 99
        error = validate_corpus_header(bad_header, corpus_file=path.name)
        self.assertIsNotNone(error)
        report = replay_corpora(
            corpus_dir=_write_temp_corpus(path.name, bad_header, rows),
        )
        self.assertFalse(report.passed)

    def test_mutation_killer_corpus_id_mismatch_fails_replay(self) -> None:
        path = CORPUS_DIR / "scalar_rn.jsonl"
        header, rows = load_corpus_file(path)
        bad_header = copy.deepcopy(header)
        bad_header["corpus_id"] = "wrong-id"
        row = copy.deepcopy(rows[0])
        result = replay_row(row, bad_header, corpus_file=path.name)
        self.assertFalse(result.passed)

    def test_mutation_killer_capability_fails_obligation_validate(self) -> None:
        obl = build_fp_fused_obligation_v2(opcodes=["fmadd"])
        mutated = copy.deepcopy(obl)
        mutated["capability"] = "fp-bitwise"
        error = validate_fp_scalar_obligation_v2(mutated)
        self.assertIsNotNone(error)

    def test_mutation_killer_expected_bits_fails_replay(self) -> None:
        path = CORPUS_DIR / "fused_residual.jsonl"
        header, rows = load_corpus_file(path)
        row = copy.deepcopy(rows[0])
        row["expected_bits"] = "0x0000000000000001"
        result = replay_row(row, header, corpus_file=path.name)
        self.assertFalse(result.passed)
        self.assertIn("result_bits mismatch", result.reason)

    def test_mutation_killer_expected_fpscr_fails_replay(self) -> None:
        path = CORPUS_DIR / "fpscr.jsonl"
        header, rows = load_corpus_file(path)
        row = copy.deepcopy(rows[0])
        row["expected_post_fpscr"] = "0xffffffff"
        if "expected_fpscr_mask" in row:
            del row["expected_fpscr_mask"]
        result = replay_row(row, header, corpus_file=path.name)
        self.assertFalse(result.passed)

    def test_mutation_killer_provenance_fails_validation(self) -> None:
        path = CORPUS_DIR / "ni.jsonl"
        header, rows = load_corpus_file(path)
        row = copy.deepcopy(rows[0])
        row["provenance"] = "generated-from-implementation"
        error = validate_corpus_row(row, corpus_file=path.name, header=header)
        self.assertIsNotNone(error)

    def test_mutation_killer_operands_fails_replay(self) -> None:
        path = CORPUS_DIR / "scalar_rn.jsonl"
        header, rows = load_corpus_file(path)
        row = copy.deepcopy(rows[0])
        row["operands"] = dict(row["operands"])
        row["operands"]["b"] = "0x0000000000000001"
        result = replay_row(row, header, corpus_file=path.name)
        self.assertFalse(result.passed)

    def test_mutation_killer_obligation_corpus_digest_fails_promotion(self) -> None:
        live = live_scalar_fp_v2_corpus_sha256()
        obl = build_fp_fused_obligation_v2(
            opcodes=["fmadd", "fmadds"],
            dimensions={key: True for key in FUSED_V2_DIMENSIONS},
            unsupported_remainder_result="unsat",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256=live,
            validation_ledger_hash="c" * 64,
        )
        mutated = copy.deepcopy(obl)
        mutated["coverage"]["corpus_sha256"] = "f" * 64
        self.assertIsNone(validate_fp_scalar_obligation_v2(mutated))
        self.assertEqual(
            evaluate_fp_scalar_obligation_v2_status(
                mutated,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_INCOMPLETE,
        )

    def test_readme_documents_corpora(self) -> None:
        readme = CORPUS_DIR / "README.md"
        self.assertTrue(readme.is_file())
        text = readme.read_text(encoding="utf-8")
        for name in CORPUS_FILES:
            self.assertIn(name, text)


def _write_temp_corpus(
    name: str,
    header: dict,
    rows: list[dict],
) -> Path:
    tmp = Path(tempfile.mkdtemp())
    path = tmp / name
    lines = [json.dumps(header, separators=(",", ":"), sort_keys=True)]
    lines.extend(json.dumps(row, separators=(",", ":"), sort_keys=True) for row in rows)
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    for other in CORPUS_FILES:
        if other == name:
            continue
        src = CORPUS_DIR / other
        (tmp / other).write_text(src.read_text(encoding="utf-8"), encoding="utf-8")
    return tmp


if __name__ == "__main__":
    unittest.main()

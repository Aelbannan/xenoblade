from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.provenance import (
    CERTIFIER_SOURCE_PATHS,
    ENGINE_SOURCE_PATTERNS,
    _collect_certifier_paths,
    _collect_engine_paths,
    canonical_json_sha256,
    hash_certifier_tree,
    hash_engine_tree,
)


class ProvenanceHashTests(unittest.TestCase):
    def test_patterns_include_generators_and_fixtures(self):
        joined = "\n".join(ENGINE_SOURCE_PATTERNS)
        self.assertIn("generators/**/*.py", joined)
        self.assertIn("fixtures/*.py", joined)
        self.assertIn("validation_ledger", joined)

    def test_engine_tree_hash_deterministic(self):
        repo = Path(__file__).resolve().parents[3]
        first = hash_engine_tree(repo)
        second = hash_engine_tree(repo)
        self.assertEqual(first, second)
        self.assertEqual(len(first), 64)

    def test_collect_includes_generators_and_fixtures_py(self):
        repo = Path(__file__).resolve().parents[3]
        paths = _collect_engine_paths(repo)
        rels = [p.relative_to(repo).as_posix() for p in paths]
        self.assertTrue(any(r.startswith("tools/ppc_equivalence/generators/") for r in rels))
        self.assertTrue(any(r.startswith("tools/ppc_equivalence/fixtures/") and r.endswith(".py") for r in rels))
        self.assertFalse(any(r.endswith(".jsonl") for r in rels))

    def test_ledger_hashed_when_present(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            base = root / "tools" / "ppc_equivalence"
            base.mkdir(parents=True)
            (base / "engine_stub.py").write_text("# stub\n", encoding="utf-8")
            (base / "validation_ledger.yaml").write_text("version: 1\n", encoding="utf-8")
            paths = _collect_engine_paths(root)
            rels = [p.relative_to(root).as_posix() for p in paths]
            self.assertIn("tools/ppc_equivalence/validation_ledger.yaml", rels)
            digest = hash_engine_tree(root)
            (base / "validation_ledger.yaml").write_text("version: 2\n", encoding="utf-8")
            self.assertNotEqual(digest, hash_engine_tree(root))

    def test_certifier_tree_hash_deterministic(self):
        repo = Path(__file__).resolve().parents[3]
        first = hash_certifier_tree(repo)
        second = hash_certifier_tree(repo)
        self.assertEqual(first, second)
        self.assertEqual(len(first), 64)

    def test_collect_certifier_paths_cover_trust_boundary(self):
        repo = Path(__file__).resolve().parents[3]
        paths = _collect_certifier_paths(repo)
        rels = [p.relative_to(repo).as_posix() for p in paths]
        self.assertEqual(rels, sorted(CERTIFIER_SOURCE_PATHS))

    def test_certifier_hash_changes_when_policy_edited(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for relative in CERTIFIER_SOURCE_PATHS:
                path = root / relative
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text("# stub\n", encoding="utf-8")
            digest = hash_certifier_tree(root)
            (root / CERTIFIER_SOURCE_PATHS[1]).write_text("# changed\n", encoding="utf-8")
            self.assertNotEqual(digest, hash_certifier_tree(root))

    def test_canonical_json_sha256_stable(self):
        a = canonical_json_sha256({"b": 1, "a": [2, 3]})
        b = canonical_json_sha256({"a": [2, 3], "b": 1})
        self.assertEqual(a, b)

    def test_proof_request_hash_stable_and_sensitive(self):
        from tools.ppc_equivalence.provenance import proof_request_hash

        first = proof_request_hash(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
            assumed_callees=["leaf"],
            callee_contract_sources={"leaf": "opaque-eabi"},
        )
        second = proof_request_hash(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
            assumed_callees=["leaf"],
            callee_contract_sources={"leaf": "opaque-eabi"},
        )
        self.assertEqual(first, second)
        changed = proof_request_hash(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
            assumed_callees=["leaf"],
            callee_contract_sources={"leaf": "inferred:leaf"},
        )
        self.assertNotEqual(first, changed)


if __name__ == "__main__":
    unittest.main()

from __future__ import annotations

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

import unittest

from tools.llm_harness.knowledge_retrieval import (
    KnowledgeIndex,
    RetrievalRecord,
    Provenance,
    retrieve_for_target,
    cluster_attempts,
    AttemptCluster,
    extract_provenance,
    RETRIEVAL_WEIGHTS,
    _target_class,
    _target_unit,
    _tag_hits,
    _recency_factor,
    _normalize_hypothesis,
    _categorize_effect,
)


class TestTargetIdentification(unittest.TestCase):
    """Helper functions for target identity parsing"""

    def test_target_class(self):
        self.assertEqual(_target_class("Foo__bar"), "Foo")
        self.assertEqual(_target_class("SomeClass::method"), "SomeClass")
        self.assertEqual(_target_class("global_func"), "global_func")

    def test_target_unit(self):
        self.assertEqual(_target_unit("Foo__bar"), "Foo")
        self.assertEqual(_target_unit("top_level"), "")
        self.assertEqual(_target_unit(""), "")


class TestTagHits(unittest.TestCase):
    """Tag matching"""

    def test_exact_hit(self):
        self.assertEqual(_tag_hits(("signed", "width"), {"signed"}), 1)

    def test_multiple_hits(self):
        self.assertEqual(_tag_hits(("signed", "width", "load"), {"signed", "load"}), 2)

    def test_no_hits(self):
        self.assertEqual(_tag_hits(("signed",), {"unsigned"}), 0)

    def test_empty_tags(self):
        self.assertEqual(_tag_hits((), {"signed"}), 0)


class TestRecencyFactor(unittest.TestCase):
    """Recency decay factor"""

    def test_recent(self):
        factor = _recency_factor("2026-07-18T00:00:00")
        self.assertGreater(factor, 0.5)

    def test_old(self):
        factor = _recency_factor("2020-01-01T00:00:00")
        self.assertLess(factor, 0.5)

    def test_empty(self):
        factor = _recency_factor("")
        self.assertAlmostEqual(factor, 0.5)

    def test_invalid(self):
        factor = _recency_factor("not-a-date")
        self.assertAlmostEqual(factor, 0.5)


class TestKnowledgeIndex(unittest.TestCase):
    """§15.2 — KnowledgeIndex and retrieval"""

    def setUp(self):
        self.index = KnowledgeIndex()
        self.index.records = [
            RetrievalRecord(
                id="ref1", title="Signed compare", body="cmpwi vs cmplwi",
                target_id="SomeClass__method", tags=("signed", "compare"),
                provenance=Provenance(target="SomeClass__method"),
            ),
            RetrievalRecord(
                id="ref2", title="Load width", body="lwz vs lbz",
                target_id="OtherClass__func", tags=("width", "load"),
                provenance=Provenance(target="OtherClass__func"),
            ),
            RetrievalRecord(
                id="ref3", title="General hint", body="some advice",
                target_id="", tags=(),
                provenance=Provenance(target=""),
            ),
        ]

    def test_retrieve_exact_target(self):
        results = retrieve_for_target(self.index, "SomeClass__method", top_k=5)
        self.assertGreater(len(results), 0)
        best = results[0]
        self.assertEqual(best.id, "ref1")
        self.assertGreater(best.score, 0)

    def test_retrieve_top_k(self):
        results = retrieve_for_target(self.index, "SomeClass__method", top_k=1)
        self.assertEqual(len(results), 1)

    def test_retrieve_unknown_target(self):
        results = retrieve_for_target(self.index, "UnknownFunc", top_k=5)
        self.assertGreaterEqual(len(results), 1)

    def test_retrieve_with_tags(self):
        results = retrieve_for_target(
            self.index, "OtherClass__func", top_k=5,
            query_tags={"width", "load"},
        )
        self.assertEqual(results[0].id, "ref2")

    def test_retrieve_same_class(self):
        self.index.records.append(
            RetrievalRecord(
                id="same_cls", title="Same class", body="body",
                target_id="SomeClass__other", tags=(),
                provenance=Provenance(target="SomeClass__other"),
            )
        )
        results = retrieve_for_target(self.index, "SomeClass__method", top_k=5)
        self.assertGreater(len(results), 0)

    def test_weights_defined(self):
        self.assertIn("exact_target", RETRIEVAL_WEIGHTS)
        self.assertEqual(RETRIEVAL_WEIGHTS["exact_target"], 5.0)
        self.assertIn("same_class", RETRIEVAL_WEIGHTS)
        self.assertIn("same_translation_unit", RETRIEVAL_WEIGHTS)
        self.assertIn("same_diagnostic_category", RETRIEVAL_WEIGHTS)
        self.assertIn("recency_decay", RETRIEVAL_WEIGHTS)

    def test_unimplemented_weights_absent(self):
        """Removed weights must not be defined without backing data."""
        for key in ("same_relocation_shape", "cfg_similarity", "instruction_mismatch_similarity"):
            self.assertNotIn(key, RETRIEVAL_WEIGHTS)


class TestAttemptClustering(unittest.TestCase):
    """§15.3 — attempt clustering"""

    def test_empty_records(self):
        clusters = cluster_attempts([])
        self.assertEqual(len(clusters), 0)

    def test_single_record(self):
        records = [
            {"hypothesis": "change signedness", "evaluation": {"match_percent": 50.0, "metrics": {}}},
        ]
        clusters = cluster_attempts(records)
        self.assertGreaterEqual(len(clusters), 1)

    def test_multiple_same_category(self):
        records = [
            {"hypothesis": "change to unsigned", "evaluation": {"match_percent": 18.2, "metrics": {}}},
            {"hypothesis": "use u32 local", "evaluation": {"match_percent": 18.3, "metrics": {}}},
        ]
        clusters = cluster_attempts(records)
        signedness = [c for c in clusters if c.cluster_id == "signedness"]
        self.assertGreaterEqual(len(signedness), 1)

    def test_avoid_flag_set(self):
        records = [
            {"hypothesis": "change to unsigned", "evaluation": {"match_percent": 18.2, "metrics": {}}},
            {"hypothesis": "use u32 local", "evaluation": {"match_percent": 18.3, "metrics": {}}},
        ]
        clusters = cluster_attempts(records)
        for c in clusters:
            if c.attempt_count >= 2 and c.best_match_percent < 50.0:
                self.assertTrue(c.avoid_without_new_evidence)

    def test_cluster_id_generation(self):
        records = [
            {"hypothesis": "change to unsigned", "evaluation": {"match_percent": 100.0, "metrics": {}}},
        ]
        clusters = cluster_attempts(records)
        for c in clusters:
            self.assertIsInstance(c.cluster_id, str)
            self.assertGreater(len(c.cluster_id), 0)

    def test_best_structural_score(self):
        records = [
            {
                "hypothesis": "fix width",
                "evaluation": {
                    "match_percent": 60.0,
                    "metrics": {"structural": {"total_score": 0.85}},
                },
            },
        ]
        clusters = cluster_attempts(records)
        self.assertGreaterEqual(len(clusters), 1)

    def test_hypothesis_normalization(self):
        """Two different spellings of signedness should cluster together"""
        records = [
            {"hypothesis": "change to signed", "evaluation": {"match_percent": 30.0, "metrics": {}}},
            {"hypothesis": "change to unsigned", "evaluation": {"match_percent": 32.0, "metrics": {}}},
        ]
        clusters = cluster_attempts(records)
        # Both should be in a signedness-related cluster
        signedness = [c for c in clusters if c.cluster_id == "signedness"]
        self.assertGreaterEqual(len(signedness), 1)


class TestKnowledgeProvenance(unittest.TestCase):
    """§15.4 — knowledge provenance"""

    def test_extract_provenance(self):
        record = {
            "experiment_id": "exp123",
            "target_id": "SomeClass__method",
            "timestamp": "2025-06-01T12:00:00Z",
            "model": "deepseek-v4",
            "schema_version": 3,
            "evaluation": {
                "status": "CODE_MATCH",
                "match_percent": 85.5,
                "metrics": {},
            },
        }
        prov = extract_provenance(record)
        self.assertEqual(prov.source_experiment, "exp123")
        self.assertEqual(prov.target, "SomeClass__method")
        self.assertEqual(prov.outcome, "CODE_MATCH")
        self.assertAlmostEqual(prov.confidence, 0.855)

    def test_provenance_to_dict(self):
        prov = Provenance(
            source_experiment="exp456",
            target="test_func",
            outcome="FULL_MATCH",
            confidence=1.0,
            confirmed_count=3,
        )
        d = prov.to_dict()
        self.assertEqual(d["source_experiment"], "exp456")
        self.assertEqual(d["confirmed_count"], 3)
        self.assertEqual(d["outcome"], "FULL_MATCH")

    def test_provenance_defaults(self):
        prov = Provenance()
        self.assertEqual(prov.source_experiment, "")
        self.assertEqual(prov.confirmed_count, 0)


class TestRetrievalRecord(unittest.TestCase):
    """RetrievalRecord dataclass"""

    def test_record_creation(self):
        rec = RetrievalRecord(
            id="test1",
            title="Test hint",
            body="Some useful knowledge",
            tags=("signed", "width"),
            target_id="Foo__bar",
            score=4.5,
        )
        self.assertEqual(rec.id, "test1")
        self.assertEqual(rec.title, "Test hint")
        self.assertAlmostEqual(rec.score, 4.5)

    def test_record_with_provenance(self):
        prov = Provenance(target="Foo__bar", outcome="CODE_MATCH")
        rec = RetrievalRecord(
            id="test2",
            title="Test",
            body="body",
            provenance=prov,
        )
        self.assertEqual(rec.provenance.outcome, "CODE_MATCH")


class TestHypothesisNormalization(unittest.TestCase):
    """Hypothesis normalization for clustering"""

    def test_signedness_normalization(self):
        result = _normalize_hypothesis("change to signed")
        self.assertIn("sint", result.lower())

    def test_type_normalization(self):
        result = _normalize_hypothesis("change u32 to int")
        self.assertIn("u32", result.lower())
        self.assertIn("int", result.lower())

    def test_cast_normalization(self):
        result = _normalize_hypothesis("remove reinterpret cast")
        self.assertIn("reinterpret", result.lower())
        self.assertIn("cast", result.lower())

    def test_empty(self):
        self.assertEqual(_normalize_hypothesis(""), "")


class TestEffectCategorization(unittest.TestCase):
    """Effect category detection"""

    def test_signedness_category(self):
        cats = _categorize_effect("signed comparison issue")
        self.assertIn("signedness", cats)

    def test_width_category(self):
        cats = _categorize_effect("wrong load width lwz vs lbz")
        self.assertIn("width", cats)

    def test_control_flow_category(self):
        cats = _categorize_effect("branch structure wrong")
        self.assertIn("control_flow", cats)

    def test_call_category(self):
        cats = _categorize_effect("missing function call")
        self.assertIn("call", cats)

    def test_mixed_categories(self):
        cats = _categorize_effect("signed compare of lbz value")
        self.assertIn("signedness", cats)
        self.assertIn("width", cats)


class TestBuildFromEntries(unittest.TestCase):
    """KnowledgeIndex.build from KnowledgeEntry-like objects"""

    def test_build_from_list(self):
        class FakeEntry:
            def __init__(self, **kwargs):
                for k, v in kwargs.items():
                    setattr(self, k, v)

        entry = FakeEntry(
            source_kind="reference",
            id="ref123",
            source_path="cookbook.md",
            line_start=10,
            title="Cookbook entry",
            body="Some cookbook guidance",
            tags=("signed",),
            target_id="",
            unit="",
            status="",
            match_percent=None,
            timestamp="",
        )
        index = KnowledgeIndex().build([entry])
        self.assertEqual(len(index.records), 1)
        self.assertEqual(index.records[0].title, "Cookbook entry")

    def test_build_timestamp_propagated(self):
        """Provenance.date should come from entry.timestamp, not source_path."""
        class FakeEntry:
            def __init__(self, **kwargs):
                for k, v in kwargs.items():
                    setattr(self, k, v)

        entry = FakeEntry(
            source_kind="attempt",
            id="attempt:test:1",
            source_path="attempts.jsonl",
            line_start=10,
            title="test func",
            body="test",
            tags=(),
            target_id="SomeClass__method",
            unit="",
            status="CODE_MATCH",
            match_percent=80.0,
            timestamp="2026-07-18T12:00:00",
        )
        index = KnowledgeIndex().build([entry])
        self.assertEqual(len(index.records), 1)
        self.assertEqual(
            index.records[0].provenance.date,
            "2026-07-18T12:00:00",
        )
        self.assertNotEqual(
            index.records[0].provenance.date,
            "attempts.jsonl",
        )


if __name__ == "__main__":
    raise SystemExit(unittest.main())

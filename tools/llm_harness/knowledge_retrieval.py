from __future__ import annotations

import hashlib
from dataclasses import dataclass, field
from datetime import datetime
from typing import Any, Dict, List, Optional, Set, Tuple


# §15.2 — scoring weights
# Three features from the plan (same_relocation_shape, cfg_similarity,
# instruction_mismatch_similarity) are reserved for future use — they
# require structured function-analysis data not yet carried by
# KnowledgeEntry and always contribute 0 today.
RETRIEVAL_WEIGHTS: dict[str, float] = {
    "exact_target": 5.0,
    "same_class": 3.0,
    "same_translation_unit": 2.5,
    "same_diagnostic_category": 2.0,
    "recency_decay": 1.0,
}


@dataclass
class Provenance:
    source_experiment: str = ""
    target: str = ""
    date: str = ""
    model: str = ""
    compiler_hash: str = ""
    dossier_version: str = ""
    outcome: str = ""
    confidence: float = 0.0
    confirmed_count: int = 0

    def to_dict(self) -> Dict[str, Any]:
        return {
            "source_experiment": self.source_experiment,
            "target": self.target,
            "date": self.date,
            "model": self.model,
            "compiler_hash": self.compiler_hash,
            "dossier_version": self.dossier_version,
            "outcome": self.outcome,
            "confidence": self.confidence,
            "confirmed_count": self.confirmed_count,
        }


@dataclass
class RetrievalRecord:
    id: str
    title: str
    body: str
    tags: Tuple[str, ...] = ()
    target_id: str = ""
    unit: str = ""
    provenance: Provenance = field(default_factory=Provenance)
    score: float = 0.0


def _stable_record_id(entry) -> str:
    raw = f"{entry.source_kind}:{entry.id}:{entry.source_path}:{entry.line_start}"
    return hashlib.sha1(raw.encode("utf-8")).hexdigest()[:12]


def _target_class(target_id: str) -> str:
    if "::" in target_id:
        return target_id.rsplit("::", 1)[0]
    return target_id.rsplit("__", 1)[0] if "__" in target_id else target_id


def _target_unit(target_id: str) -> str:
    return target_id.split("__", 1)[0] if "__" in target_id else ""


def _tag_hits(entry_tags: Tuple[str, ...], query_tags: Set[str]) -> int:
    return sum(1 for t in entry_tags if t in query_tags)


def _recency_factor(date_str: str) -> float:
    if not date_str:
        return 0.5
    try:
        dt = datetime.fromisoformat(date_str)
        days = (datetime.now() - dt).days
        return max(0.1, 1.0 - days * 0.002)
    except (ValueError, TypeError):
        return 0.5


class KnowledgeIndex:
    def __init__(self, entries: Optional[List[Any]] = None):
        self.records: List[RetrievalRecord] = []
        if entries:
            for entry in entries:
                self.add_entry(entry)

    def add_entry(self, entry) -> None:
        self.records.append(RetrievalRecord(
            id=_stable_record_id(entry),
            title=entry.title,
            body=entry.body,
            tags=entry.tags,
            target_id=entry.target_id,
            unit=entry.unit,
            provenance=Provenance(
                target=entry.target_id,
                date=entry.timestamp,
                outcome=entry.status,
                confidence=entry.match_percent / 100.0 if entry.match_percent else 0.0,
                confirmed_count=1,
            ),
        ))

    def build(
        self,
        entries: List[Any],
    ) -> KnowledgeIndex:
        result = KnowledgeIndex()
        for entry in entries:
            result.add_entry(entry)
        return result


def retrieve_for_target(
    index: KnowledgeIndex,
    target_id: str,
    *,
    top_k: int = 10,
    diagnostic_category: str = "",
    query_tags: Optional[Set[str]] = None,
) -> List[RetrievalRecord]:
    target_class = _target_class(target_id)
    target_unit = _target_unit(target_id)
    query_tags_set = query_tags or set()
    scored: List[Tuple[float, RetrievalRecord]] = []

    for record in index.records:
        score = 0.0
        exact = record.target_id == target_id
        if exact:
            score += RETRIEVAL_WEIGHTS["exact_target"]
        same_cls = record.target_id and _target_class(record.target_id) == target_class and not exact
        if same_cls:
            score += RETRIEVAL_WEIGHTS["same_class"]
        same_tu = record.unit and record.unit == target_unit and not exact
        if same_tu:
            score += RETRIEVAL_WEIGHTS["same_translation_unit"]
        if diagnostic_category and diagnostic_category in record.tags:
            score += RETRIEVAL_WEIGHTS["same_diagnostic_category"]
        matched_tags = _tag_hits(record.tags, query_tags_set)
        score += 1.0 * matched_tags
        score += _recency_factor(record.provenance.date) * RETRIEVAL_WEIGHTS["recency_decay"]
        scored.append((-score, record))

    scored.sort(key=lambda x: (x[0], x[1].id))
    results: List[RetrievalRecord] = []
    for neg_score, record in scored[:top_k]:
        record.score = abs(neg_score)
        results.append(record)
    return results


# §15.3 — Attempt clustering
@dataclass
class AttemptCluster:
    cluster_id: str
    attempt_count: int
    best_match_percent: float
    best_structural_score: float
    conclusion: str
    avoid_without_new_evidence: bool = False


_HYPOTHESIS_NORMALIZATIONS: list[tuple[str, str]] = [
    ("unsigned", "UINT"),
    ("signed", "SINT"),
    ("u32", "U32"),
    ("s32", "S32"),
    ("int", "INT"),
    ("u8", "U8"),
    ("s8", "S8"),
    ("cast", "CAST"),
    ("reinterpret", "REINTERPRET"),
]


def _normalize_hypothesis(hypothesis: str) -> str:
    h = hypothesis.lower().strip()
    for a, b in _HYPOTHESIS_NORMALIZATIONS:
        h = h.replace(a, b)
    return h


_OBSERVED_EFFECT_CATEGORIES: list[tuple[str, list[str]]] = [
    ("signedness", ["signed", "unsigned", "comparison", "cmpwi", "cmplwi"]),
    ("width", ["load width", "store width", "lbz", "lhz", "lwz", "stb", "sth", "stw"]),
    ("control_flow", ["branch", "if", "else", "return", "loop", "switch"]),
    ("call", ["call", "invoke", "function"]),
    ("constant", ["constant", "immediate", "offset", "mask"]),
    ("expression_order", ["order", "reorder", "temporary", "subexpression"]),
    ("type", ["type", "cast", "conversion", "pointer"]),
    ("global", ["global", "static", "extern"]),
]


def _categorize_effect(text: str) -> List[str]:
    text_lower = text.lower()
    return [
        cat for cat, keywords in _OBSERVED_EFFECT_CATEGORIES
        if any(kw in text_lower for kw in keywords)
    ]


def cluster_attempts(
    records: List[Dict[str, Any]],
    threshold: float = 0.6,
) -> List[AttemptCluster]:
    clusters: Dict[str, List[Dict[str, Any]]] = {}

    for record in records:
        hypothesis = str(record.get("hypothesis", record.get("candidate_summary", {}).get("hypothesis", "")))
        evaluation = record.get("evaluation", {})
        notes = " ".join(
            record.get("candidate_summary", {}).get("notes", [])
            or record.get("notes", [])
            or []
        )

        combined = hypothesis + " " + notes
        effect_cats = _categorize_effect(combined)

        norm_h = _normalize_hypothesis(combined)
        # Use normalized for dedup but original categories for clustering
        cluster_key = "-".join(sorted(effect_cats)) if effect_cats else "other"
        if cluster_key not in clusters:
            clusters[cluster_key] = []
        clusters[cluster_key].append(record)

    result: List[AttemptCluster] = []
    for cluster_key, group in clusters.items():
        if len(group) < 1:
            continue

        best_match = max(
            float(r.get("evaluation", {}).get("match_percent", 0) or 0)
            for r in group
        )
        best_structural = 0.0
        for r in group:
            m = r.get("evaluation", {}).get("metrics", {}).get("structural", {})
            if isinstance(m, dict):
                best_structural = max(best_structural, m.get("total_score", 0.0))

        hypotheses = [_normalize_hypothesis(
            str(r.get("hypothesis", r.get("candidate_summary", {}).get("hypothesis", "")))
        ) for r in group]
        unique_hypotheses = list(dict.fromkeys(h for h in hypotheses if h))

        avoid = len(unique_hypotheses) >= 2 and best_match < 50.0

        if cluster_key == "other":
            conclusion = "No clear pattern across attempts."
        else:
            conclusion = (
                f"Attempted {len(group)} changes in {cluster_key} area. "
                f"Best result: {best_match:.1f}%. "
                f"{'Further changes unlikely to help without new evidence.' if avoid else 'May warrant further exploration.'}"
            )

        result.append(AttemptCluster(
            cluster_id=cluster_key,
            attempt_count=len(group),
            best_match_percent=best_match,
            best_structural_score=best_structural,
            conclusion=conclusion,
            avoid_without_new_evidence=avoid,
        ))

    return result


# §15.4 — Knowledge provenance
def extract_provenance(experiment_record: Dict[str, Any]) -> Provenance:
    evaluation = experiment_record.get("evaluation", {})
    metrics = evaluation.get("metrics", {})
    structural = metrics.get("structural", {}) or {}
    return Provenance(
        source_experiment=experiment_record.get("experiment_id", ""),
        target=experiment_record.get("target_id", ""),
        date=experiment_record.get("timestamp", ""),
        model=experiment_record.get("model", ""),
        compiler_hash=experiment_record.get("compiler_hash", ""),
        dossier_version=str(experiment_record.get("schema_version", "")),
        outcome=evaluation.get("status", ""),
        confidence=float(evaluation.get("match_percent", 0) or 0) / 100.0,
        confirmed_count=1,
    )

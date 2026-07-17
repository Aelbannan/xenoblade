from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Dict, List, Optional

from .types import CandidateEvaluation, CandidateStatus


# §14.1 — Separate proof and similarity dimensions
@dataclass
class ProofSimilarity:
    semantic_proof: str = "unsupported"
    structural_score: float = 0.0
    match_percent: float = 0.0
    full_match: bool = False
    size_delta: int = 0

    def to_dict(self) -> Dict[str, Any]:
        return {
            "semantic_proof": self.semantic_proof,
            "structural_score": self.structural_score,
            "match_percent": self.match_percent,
            "full_match": self.full_match,
            "size_delta": self.size_delta,
        }


def extract_proof_similarity(evaluation: CandidateEvaluation) -> ProofSimilarity:
    eq_status = str(evaluation.equivalence_status or "").lower()
    if eq_status == "proved" or eq_status == "equivalent":
        proof = "proved"
    elif eq_status == "disproved":
        proof = "disproved"
    elif eq_status and eq_status != "none":
        proof = eq_status
    else:
        proof = "unsupported"

    structural = (
        evaluation.structural_report.total_score
        if evaluation.structural_report
        else 0.0
    )
    size_delta = 0
    if evaluation.function_size is not None and evaluation.retail_size is not None:
        size_delta = evaluation.function_size - evaluation.retail_size

    return ProofSimilarity(
        semantic_proof=proof,
        structural_score=structural,
        match_percent=evaluation.match_percent,
        full_match=evaluation.status == CandidateStatus.FULL_MATCH,
        size_delta=size_delta,
    )


# §14.5 — Proof-aware ranking
def rank_candidate(
    evaluation: CandidateEvaluation,
    *,
    proof_similarity: Optional[ProofSimilarity] = None,
) -> tuple:
    ps = proof_similarity or extract_proof_similarity(evaluation)

    return (
        int(not evaluation.object_regressions),
        _semantic_proof_tier(ps),
        _structural_bucket(ps),
        round(ps.match_percent, 4),
        int(_size_is_acceptable(evaluation)),
        -max(0, ps.size_delta),
        -len(evaluation.warnings),
    )


def _semantic_proof_tier(ps: ProofSimilarity) -> int:
    if ps.full_match:
        return 6
    if ps.semantic_proof == "proved":
        return 5
    if ps.semantic_proof == "disproved":
        return 1
    # Unproven candidates sorted by structural score
    if ps.structural_score >= 0.90:
        return 4
    if ps.structural_score >= 0.75:
        return 3
    if ps.structural_score >= 0.50:
        return 2
    return 1 if ps.structural_score > 0 else 0


def _structural_bucket(ps: ProofSimilarity) -> int:
    score = ps.structural_score
    if score >= 0.90:
        return 4
    if score >= 0.75:
        return 3
    if score >= 0.50:
        return 2
    if score >= 0.25:
        return 1
    return 0


def _size_is_acceptable(evaluation: CandidateEvaluation) -> bool:
    if evaluation.function_size is None or evaluation.retail_size is None:
        return True
    return evaluation.function_size <= evaluation.retail_size


def rank_candidate_lexicographic(
    evaluation: CandidateEvaluation,
) -> tuple:
    return rank_candidate(evaluation,
        proof_similarity=extract_proof_similarity(evaluation),
    )

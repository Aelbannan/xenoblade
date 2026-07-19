"""Normalize attempts.jsonl rows for Atlas storage."""

from __future__ import annotations

from typing import Any, Dict, List, Optional


def normalize_attempt(row: Dict[str, Any], *, order: int = 0) -> Optional[Dict[str, Any]]:
    target_id = row.get("target_id")
    if not target_id:
        return None
    match = row.get("instruction_match")
    if match is None:
        match = row.get("code_match_percent")
    try:
        match_f = float(match) if match is not None else None
    except (TypeError, ValueError):
        match_f = None
    policy = row.get("policy_exception") or row.get("policy_exception_flag")
    return {
        "target_id": str(target_id),
        "timestamp": row.get("timestamp") or "",
        "status": row.get("status"),
        "instruction_match": match_f,
        "hypothesis": row.get("hypothesis") or "",
        "next_change": row.get("next_change") or "",
        "runtime_test": row.get("runtime_test") or "",
        "policy_exception": 1 if policy else 0,
        "equivalence_status": row.get("equivalence_status"),
        "order": order,
        "raw": row,
    }


def normalize_attempts(rows: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    out: List[Dict[str, Any]] = []
    for index, row in enumerate(rows):
        if not isinstance(row, dict):
            continue
        normalized = normalize_attempt(row, order=index)
        if normalized is not None:
            out.append(normalized)
    return out


def attempt_stats_by_target(attempts: List[Dict[str, Any]]) -> Dict[str, Dict[str, int]]:
    """Count attempts and non-improving streaks per target (advisory scoring)."""
    stats: Dict[str, Dict[str, int]] = {}
    best: Dict[str, float] = {}
    for row in attempts:
        tid = str(row.get("target_id") or "")
        if not tid:
            continue
        bucket = stats.setdefault(tid, {"attempt_count": 0, "non_improving_attempts": 0})
        bucket["attempt_count"] += 1
        match = row.get("instruction_match")
        try:
            value = float(match) if match is not None else None
        except (TypeError, ValueError):
            value = None
        if value is None:
            bucket["non_improving_attempts"] += 1
            continue
        prev = best.get(tid)
        if prev is not None and value <= prev + 1e-9:
            bucket["non_improving_attempts"] += 1
        best[tid] = value if prev is None else max(prev, value)
    return stats

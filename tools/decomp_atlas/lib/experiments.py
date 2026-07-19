"""Normalize llm-harness experiments.jsonl rows for Atlas storage."""

from __future__ import annotations

from typing import Any, Dict, List, Optional


def _eval(row: Dict[str, Any]) -> Dict[str, Any]:
    evaluation = row.get("evaluation")
    return evaluation if isinstance(evaluation, dict) else {}


def normalize_experiment(row: Dict[str, Any]) -> Optional[Dict[str, Any]]:
    target_id = row.get("target_id")
    if not target_id:
        return None
    evaluation = _eval(row)
    size_ok = evaluation.get("size_ok")
    if size_ok is True:
        size_flag: Optional[int] = 1
    elif size_ok is False:
        size_flag = 0
    else:
        size_flag = None
    accepted = evaluation.get("accepted")
    equivalence = evaluation.get("equivalence")
    if isinstance(equivalence, dict):
        equivalence_result = equivalence.get("status") or str(equivalence)
    elif equivalence is None:
        equivalence_result = None
    else:
        equivalence_result = str(equivalence)
    match = evaluation.get("match_percent")
    try:
        match_f = float(match) if match is not None else None
    except (TypeError, ValueError):
        match_f = None
    return {
        "experiment_id": row.get("experiment_id"),
        "target_id": str(target_id),
        "workflow": row.get("workflow"),
        "model": row.get("model"),
        "model_id": row.get("model_id"),
        "run_index": row.get("run_index"),
        "timestamp": row.get("timestamp"),
        "duration_seconds": row.get("duration_seconds"),
        "input_tokens": row.get("input_tokens"),
        "output_tokens": row.get("output_tokens"),
        "cost": row.get("cost"),
        "result_status": evaluation.get("status") or row.get("error") and "ERROR",
        "match_percent": match_f,
        "equivalence_result": equivalence_result,
        "size_ok": size_flag,
        "accepted": 1 if accepted else 0 if accepted is False else None,
        "winner": 1 if row.get("winner") else 0,
        "error": row.get("error"),
        "artifact": row.get("artifact"),
        "raw": row,
    }


def normalize_experiments(rows: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    out: List[Dict[str, Any]] = []
    for row in rows:
        if not isinstance(row, dict):
            continue
        normalized = normalize_experiment(row)
        if normalized is not None:
            out.append(normalized)
    return out

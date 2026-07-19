"""Readiness classification and advisory difficulty scoring."""

from __future__ import annotations

from typing import Any, Mapping, Optional, Set

from tools.coop.lib.targets import ACCEPTED_MATCH_STATUSES

READINESS_VALUES = (
    "accepted",
    "safe_leaf",
    "callees_accepted",
    "blocked_unresolved",
    "blocked_indirect",
    "claimed",
    "not_buildable",
    "active_unclaimed",
    "backlog",
)


def _as_list(value: Any) -> list[Any]:
    if value is None:
        return []
    if isinstance(value, list):
        return value
    return []


def _truthy(value: Any) -> bool:
    if isinstance(value, bool):
        return value
    if isinstance(value, (int, float)):
        return value != 0
    if isinstance(value, str):
        return value.strip().lower() in {"1", "true", "yes"}
    return bool(value)


def is_leaf_row(row: Mapping[str, Any]) -> bool:
    """Match ``harness_targets`` leaf predicate."""
    return (
        row.get("callgraph_status") == "complete"
        and not _as_list(row.get("called_functions"))
        and not _as_list(row.get("unresolved_called_functions"))
        and not _truthy(row.get("has_indirect_calls"))
    )


def has_accepted_callees_row(row: Mapping[str, Any], certified_ids: Set[str]) -> bool:
    """Match ``harness_targets`` callees-accepted predicate."""
    called = _as_list(row.get("called_functions"))
    return (
        row.get("callgraph_status") == "complete"
        and bool(called)
        and not _as_list(row.get("unresolved_called_functions"))
        and not _truthy(row.get("has_indirect_calls"))
        and all(str(target_id) in certified_ids for target_id in called)
    )


def classify_readiness(target_row_dict: Mapping[str, Any], certified_ids: Set[str]) -> str:
    """Classify a target into a readiness bucket for Atlas UI filters."""
    status = str(target_row_dict.get("status") or "NOT_STARTED")
    workflow = str(target_row_dict.get("workflow_status") or "BACKLOG")
    buildable = _truthy(target_row_dict.get("buildable"))
    claim = target_row_dict.get("claim")
    owner = None
    if isinstance(claim, dict):
        owner = claim.get("owner")
    if not owner:
        owner = target_row_dict.get("owner")

    if status in ACCEPTED_MATCH_STATUSES or workflow == "ACCEPTED":
        return "accepted"
    if not buildable:
        return "not_buildable"
    if workflow == "CLAIMED" or (isinstance(owner, str) and owner.strip()):
        return "claimed"
    if _as_list(target_row_dict.get("unresolved_called_functions")):
        return "blocked_unresolved"
    if _truthy(target_row_dict.get("has_indirect_calls")):
        return "blocked_indirect"
    if is_leaf_row(target_row_dict):
        return "safe_leaf"
    if has_accepted_callees_row(target_row_dict, certified_ids):
        return "callees_accepted"
    if workflow == "ACTIVE":
        return "active_unclaimed"
    return "backlog"


def _num(value: Any, default: float = 0.0) -> float:
    try:
        if value is None or value == "":
            return default
        return float(value)
    except (TypeError, ValueError):
        return default


def difficulty_score(
    *,
    size: Optional[float] = None,
    instruction_count: Optional[float] = None,
    branch_count: Optional[float] = None,
    relocation_count: Optional[float] = None,
    stack_frame: Optional[float] = None,
    direct_call_count: Optional[float] = None,
    unresolved_call_count: Optional[float] = None,
    has_indirect_calls: bool = False,
    instruction_percent: Optional[float] = None,
    attempt_count: int = 0,
    non_improving_attempts: int = 0,
    policy_exception: bool = False,
) -> float:
    """Advisory difficulty in ``[0, 100]``. Higher means harder."""
    size_bytes = _num(size)
    insn = _num(instruction_count)
    if insn <= 0 and size_bytes > 0:
        insn = size_bytes / 4.0

    score = 0.0
    score += min(35.0, insn * 0.35)
    score += min(15.0, _num(branch_count) * 1.5)
    score += min(12.0, _num(relocation_count) * 1.2)
    score += min(10.0, _num(stack_frame) / 4.0)
    score += min(10.0, _num(direct_call_count) * 2.0)
    score += min(12.0, _num(unresolved_call_count) * 4.0)
    if has_indirect_calls:
        score += 10.0

    match = _num(instruction_percent, default=-1.0)
    if match >= 0.0:
        score += max(0.0, (100.0 - match) * 0.15)

    score += min(8.0, float(non_improving_attempts) * 1.5)
    score += min(4.0, float(max(0, attempt_count - non_improving_attempts)) * 0.5)
    if policy_exception:
        score += 5.0

    return round(max(0.0, min(100.0, score)), 2)

"""Shared data contracts for tools/llm_decomp.

Every module imports from here; keep this file dependency-free (stdlib only).
Design reference: docs/llm_decomp_design.md.
"""
from __future__ import annotations

import enum
from dataclasses import dataclass, field


class SessionStatus(str, enum.Enum):
    INIT = "INIT"
    MATCHING = "MATCHING"
    ACCEPTED = "ACCEPTED"
    CLEANUP = "CLEANUP"
    DONE = "DONE"
    ABORTED = "ABORTED"  # set by crash reconciliation


class VerdictKind(str, enum.Enum):
    ACCEPTED = "accepted"
    SOFT_REJECT = "soft_reject"  # unmatched sibling got worse; agent must fix/revert
    HARD_REJECT = "hard_reject"  # matched sibling regressed; harness auto-reverted
    COMPILE_ERROR = "compile_error"


@dataclass
class ToolCall:
    name: str
    args: dict
    id: str = ""  # provider tool_call id, echoed back in tool messages


@dataclass
class ToolResult:
    ok: bool
    content: str  # markdown, LLM-facing
    data: dict = field(default_factory=dict)  # machine-facing (transcript)


@dataclass
class SymbolBaseline:
    byte_hash: str  # hash of the symbol's instruction bytes in the baseline object
    mismatch_count: int  # vs retail at baseline capture
    fingerprint: str = ""  # diff fingerprint for repeat-detection


@dataclass
class Baseline:
    unit: str
    symbols: dict[str, SymbolBaseline]
    text_size: int
    text_budget: int | None
    object_path: str  # saved baseline .o inside the session dir


@dataclass
class Regression:
    symbol: str
    baseline_mismatches: int
    current_mismatches: int
    was_matched: bool  # baseline_mismatches == 0


@dataclass
class Verdict:
    kind: VerdictKind
    accepted: bool
    target_symbol: str
    target_mismatches: int | None = None
    target_fuzzy: float | None = None
    regressions: list[Regression] = field(default_factory=list)
    text_size: int = 0
    text_budget: int | None = None
    rule: str = ""  # machine-readable reason, e.g. "sibling_regression"
    diagnostics: str = ""  # cleaned compile diagnostics on COMPILE_ERROR


@dataclass
class SessionMeta:
    """Persisted to session.json in the session dir (fsynced at INIT)."""

    target_id: str
    session_id: str
    session_type: str  # "match" | "type-recovery" | "rename" | "tu-cleanup" | "size-trim"
    unit: str  # objdiff unit hint, e.g. "kyoshin/cf/CfPadTask"
    symbol: str  # mangled target symbol ("" for non-match session types)
    status: str  # SessionStatus value
    writable: list[str]  # repo-relative paths this session may write
    owner: str
    created_at: str  # ISO-8601

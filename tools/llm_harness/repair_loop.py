from __future__ import annotations

from dataclasses import dataclass, field
from typing import List

from .compile_diagnostic import NormalizedDiagnostic


class BlockedReason:
    REPEATED_DIAGNOSTIC = "repeated_diagnostic"
    MISSING_DECLARATION = "missing_declaration"
    BUDGET_EXHAUSTED = "budget_exhausted"


@dataclass
class RepairAttempt:
    repair_index: int
    source: str
    diagnostics: List[NormalizedDiagnostic] = field(default_factory=list)
    root_fingerprint: str = ""
    succeeded: bool = False
    error: str = ""


@dataclass
class RepairBranch:
    target_id: str
    strategy_label: str
    initial_source: str
    attempts: List[RepairAttempt] = field(default_factory=list)
    blocked: bool = False
    blocked_reason: str = ""

    @property
    def latest(self) -> RepairAttempt | None:
        return self.attempts[-1] if self.attempts else None

    def block(self, reason: str, detail: str = "") -> None:
        self.blocked = True
        self.blocked_reason = f"{reason}: {detail}" if detail else reason


def repairability_tier(diagnostics: List[NormalizedDiagnostic]) -> int:
    if not diagnostics:
        return 5
    for d in diagnostics:
        if d.category == "syntax_error":
            return 1
    for d in diagnostics:
        if d.category in ("unknown_identifier", "unknown_member", "incomplete_type"):
            return 2
    for d in diagnostics:
        if d.category in ("invalid_conversion", "argument_count", "access_control"):
            return 3
    for d in diagnostics:
        if d.category == "linkage_error":
            return 4
    return 0


def compile_repair_priority(branch: RepairBranch) -> Tuple:
    latest = branch.latest
    if latest is None:
        return (0, 0, 0, 0, 0, 0.0)

    has_source = 1 if latest.source.strip() else 0
    tier = repairability_tier(latest.diagnostics)
    count = -len(latest.diagnostics)
    fp_fresh = 1 if (latest.root_fingerprint and (
        not branch.blocked or branch.blocked_reason != BlockedReason.REPEATED_DIAGNOSTIC
    )) else 0
    return (has_source, 1, tier, count, fp_fresh, 0.0)

from __future__ import annotations

from collections import OrderedDict
from dataclasses import dataclass, field
from typing import Any, Dict, List, Optional


@dataclass
class Counterexample:
    input_registers: Dict[str, str] = field(default_factory=dict)
    differing_observable: str = ""
    retail_value: str = ""
    candidate_value: str = ""
    proof_status: str = "disproved"


@dataclass
class ProofResult:
    status: str
    counterexample: Optional[Counterexample] = None
    detail: str = ""


@dataclass
class ProofRecord:
    target_id: str
    candidate_source_hash: str
    proof_status: str
    structural_score: float = 0.0
    match_percent: float = 0.0
    counterexample: Optional[Counterexample] = None
    error: str = ""


COUNTEREXAMPLE_DIMENSIONS: list[str] = [
    "store at",
    "register value",
    "call target",
    "branch outcome",
    "memory offset",
    "return value",
]


def summarize_counterexample(ce: Counterexample, max_lines: int = 3) -> str:
    parts: list[str] = []
    if ce.differing_observable:
        parts.append(f"Observable: {ce.differing_observable}")
    if ce.retail_value or ce.candidate_value:
        parts.append(f"Retail: {ce.retail_value}  Candidate: {ce.candidate_value}")
    if ce.input_registers:
        sample = "; ".join(
            f"{reg}={val}" for reg, val in sorted(ce.input_registers.items())[:4]
        )
        parts.append(f"Inputs: {sample}")
    return "\n".join(parts[:max_lines])


PROOF_STATUS_LABELS: OrderedDict = OrderedDict([
    ("proved", "Semantically proved equivalent"),
    ("disproved", "Counterexample found"),
    ("inconclusive_unsupported", "Unsupported instruction"),
    ("inconclusive_timeout", "Timeout"),
    ("inconclusive_external", "Unknown external effect"),
    ("inconclusive_missing_cert", "Missing callee certificate"),
    ("inconclusive_unknown", "Solver returned unknown"),
    ("unsupported", "Analysis not supported for this function"),
])


def classify_proof_status(status: str) -> str:
    s = status.lower().strip()
    if s == "proved":
        return "proved"
    if s == "disproved":
        return "disproved"
    if s == "unsupported":
        return "unsupported"
    if "timeout" in s:
        return "inconclusive_timeout"
    if "unsupported" in s:
        return "inconclusive_unsupported"
    if "external" in s or "unknown" in s or "missing cert" in s:
        return "inconclusive_external"
    if s.startswith("inconclusive"):
        return s
    return "inconclusive_unknown"

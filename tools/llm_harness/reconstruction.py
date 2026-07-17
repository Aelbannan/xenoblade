from __future__ import annotations

import json
import re
from dataclasses import dataclass, field
from typing import Any, Dict, List, Optional


RECONSTRUCTION_STRATEGIES: dict[str, str] = {
    "A": (
        "Strategy A — literal operations. "
        "Preserve observed calls. Preserve memory offsets and widths. "
        "Preserve branch count and return paths. "
        "Prefer direct, simple C++. Avoid creative abstraction."
    ),
    "B": (
        "Strategy B — type-aware idiomatic source. "
        "Use known members and enums. Follow accepted sibling conventions. "
        "Use repository-compatible types. "
        "Avoid casts unless required by observed widths."
    ),
    "C": (
        "Strategy C — alternative control-flow hypothesis. "
        "Explore a distinct but evidence-supported high-level CFG. "
        "Consider early return versus nested branch. "
        "Consider loop forms only if backedges exist. "
        "Do not change observable effects."
    ),
}


@dataclass
class SemanticSummary:
    calls: List[str] = field(default_factory=list)
    memory_reads: List[Dict[str, Any]] = field(default_factory=list)
    memory_writes: List[Dict[str, Any]] = field(default_factory=list)
    return_paths: List[Dict[str, str]] = field(default_factory=list)
    constants: List[int] = field(default_factory=list)


@dataclass
class ReconstructionResponse:
    response_schema_version: int = 2
    stage: str = "reconstruct"
    source: str = ""
    semantic_summary: SemanticSummary = field(default_factory=SemanticSummary)
    assumptions: List[str] = field(default_factory=list)
    blocked_on: str = ""
    hypothesis: str = ""
    confidence: float = 0.0


_STRATEGY_FENCE_RE = re.compile(
    r'<(STRATEGY\s*[A-C])>', re.IGNORECASE
)


def parse_strategy_label(text: str) -> str | None:
    m = _STRATEGY_FENCE_RE.search(text)
    if m:
        return m.group(1).upper()
    return None


def parse_reconstruction_response(text: str) -> ReconstructionResponse | None:
    cleaned = text.strip()
    fenced = re.search(r"```(?:json)?\s*(\{.*\})\s*```", cleaned, re.DOTALL)
    if fenced:
        cleaned = fenced.group(1)
    else:
        start, end = cleaned.find("{"), cleaned.rfind("}")
        if start >= 0 and end > start:
            cleaned = cleaned[start : end + 1]
    try:
        data = json.loads(cleaned)
    except (json.JSONDecodeError, ValueError):
        return None

    if not isinstance(data.get("source"), str) or not data["source"].strip():
        return None

    summary_data = data.get("semantic_summary", {})
    if not isinstance(summary_data, dict):
        summary_data = {}

    semantic = SemanticSummary(
        calls=summary_data.get("calls", []),
        memory_reads=summary_data.get("memory_reads", []),
        memory_writes=summary_data.get("memory_writes", []),
        return_paths=summary_data.get("return_paths", []),
        constants=summary_data.get("constants", []),
    )

    return ReconstructionResponse(
        response_schema_version=data.get("response_schema_version", 2),
        stage=data.get("stage", "reconstruct"),
        source=data["source"],
        semantic_summary=semantic,
        assumptions=data.get("assumptions", []),
        blocked_on=data.get("blocked_on", ""),
        hypothesis=data.get("hypothesis", ""),
        confidence=float(data.get("confidence", 0.0)),
    )


def get_strategy_prompt_suffix(strategy_key: str | None) -> str:
    if strategy_key and strategy_key.upper() in RECONSTRUCTION_STRATEGIES:
        key = strategy_key.upper()
        return f"\n\nReconstruction approach — {RECONSTRUCTION_STRATEGIES[key]}\n"
    return ""

"""Offline ChatGPT Web batch export / ingest for the LLM harness.

Reuses harness selection, prompt construction, isolated evaluation, and
guarded promotion. ChatGPT-reported statuses are never trusted.
"""
from __future__ import annotations

import hashlib
import json
import math
import os
import random
import re
import shutil
from dataclasses import asdict, dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Literal, Optional, TYPE_CHECKING

from .compile_diagnostic import normalize_compile_output, select_root_diagnostic
from .match_improve import format_objdiff_feedback_text
from .types import Candidate, Evaluation, PromotionResult

if TYPE_CHECKING:
    from .core import Harness

SCHEMA_VERSION = 1
REQUEST_HEADER = "WEB_DECOMP_BATCH_REQUEST_V1"
REQUEST_FOOTER = "END_WEB_DECOMP_BATCH_REQUEST_V1"
RESPONSE_HEADER = "WEB_DECOMP_BATCH_RESPONSE_V1"
RESPONSE_FOOTER = "END_WEB_DECOMP_BATCH_RESPONSE_V1"
COMPLETE_HEADER = "WEB_DECOMP_BATCH_COMPLETE_V1"
COMPLETE_FOOTER = "END_WEB_DECOMP_BATCH_COMPLETE_V1"

# Parser size limits
MAX_RESPONSE_BYTES = 10 * 1024 * 1024
MAX_CANDIDATE_BYTES = 256 * 1024
MAX_HYPOTHESIS_BYTES = 16 * 1024

# Fence policy for v1 (printed on successful ingest completion)
FENCE_POLICY = "strict_reject_outer_code_fences"

EXIT_OK = 0
EXIT_INFRA = 1
EXIT_FORMAT = 2
EXIT_STALE = 3

BatchStatus = Literal["OPEN", "DEBUG", "COMPLETE", "ERROR"]
IngestOutcome = Literal[
    "FULL_MATCH_PROMOTED",
    "EQUIVALENT_MATCH_PROMOTED",
    "UNRESOLVED",
    "COMPILE_ERROR",
    "NO_CANDIDATE",
    "MISSING_RESPONSE",
    "EVALUATION_ERROR",
    "PROMOTION_FAILED",
]


class WebBatchFormatError(ValueError):
    def __init__(self, message: str, *, line: int | None = None) -> None:
        prefix = f"line {line}: " if line is not None else ""
        super().__init__(prefix + message)
        self.line = line


class WebBatchStaleError(ValueError):
    """Stale round, idempotency conflict, or incomplete-round without --resume."""


# ---------------------------------------------------------------------------
# Difficulty cost
# ---------------------------------------------------------------------------


def instruction_count_from_text_size(retail_text_size: int) -> int:
    return max(1, math.ceil(max(0, int(retail_text_size)) / 4))


def base_cost(instruction_count: int) -> int:
    n = max(1, int(instruction_count))
    if n <= 15:
        return 1
    if n <= 64:
        return 2
    if n <= 192:
        return 5
    if n <= 512:
        return 15
    return 40 + math.ceil((n - 512) / 64)


def complexity_modifier(
    *,
    direct_call_count: int = 0,
    unresolved_call_count: int = 0,
    has_indirect_calls: bool = False,
    frontier_kind: str = "leaf",
) -> int:
    cost = 0
    if direct_call_count > 3:
        cost += 1
    if unresolved_call_count:
        cost += min(4, 1 + int(unresolved_call_count))
    if has_indirect_calls:
        cost += 4
    if frontier_kind == "pending":
        cost += 3
    return cost


def difficulty_cost(
    *,
    retail_text_size: int,
    direct_call_count: int = 0,
    unresolved_call_count: int = 0,
    has_indirect_calls: bool = False,
    frontier_kind: str = "leaf",
    instruction_count: int | None = None,
) -> int:
    insn = (
        int(instruction_count)
        if instruction_count is not None
        else instruction_count_from_text_size(retail_text_size)
    )
    return max(
        1,
        base_cost(insn)
        + complexity_modifier(
            direct_call_count=direct_call_count,
            unresolved_call_count=unresolved_call_count,
            has_indirect_calls=has_indirect_calls,
            frontier_kind=frontier_kind,
        ),
    )


def tier_sort_key(tier: int | str | None) -> int:
    if isinstance(tier, int):
        return tier
    text = str(tier or "").upper()
    mapping = {"P0": 0, "P1": 1, "P2": 2, "P3": 3, "P9": 9}
    if text in mapping:
        return mapping[text]
    digits = "".join(ch for ch in text if ch.isdigit())
    return int(digits) if digits else 99


# ---------------------------------------------------------------------------
# Data classes
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class WebTarget:
    target_id: str
    unit: str
    symbol: str
    source_path: str
    status: str
    match_percent: float | None
    frontier_kind: str
    retail_text_size: int
    instruction_count: int
    cost: int
    tier_sort: int
    direct_call_count: int
    unresolved_call_count: int
    has_indirect_calls: bool
    workflow: str = "improve"
    branch_count: int | None = None
    basic_block_count: int | None = None
    loop_count: int | None = None
    jump_table_count: int | None = None


@dataclass
class WebBatch:
    batch_id: str
    index: int
    budget_limit: int
    targets: list[WebTarget] = field(default_factory=list)

    @property
    def used_budget(self) -> int:
        return sum(target.cost for target in self.targets)

    def count_unit(self, unit: str) -> int:
        return sum(1 for target in self.targets if target.unit == unit)

    def add(self, target: WebTarget) -> None:
        self.targets.append(target)


@dataclass(frozen=True)
class WebFunctionResponse:
    target_id: str
    candidate_source: str | None
    hypothesis: str
    skip_reason: str


@dataclass(frozen=True)
class ParsedWebResponse:
    batch_id: str
    round: int
    functions: dict[str, WebFunctionResponse]


@dataclass
class WebFunctionIngestResult:
    target_id: str
    outcome: IngestOutcome
    status: str
    match_percent: float | None
    promoted: bool
    candidate_source: str | None
    detail: str
    artifacts: dict[str, str] = field(default_factory=dict)
    evaluation: dict[str, Any] = field(default_factory=dict)


@dataclass
class WebBatchCreateResult:
    output_dir: Path
    batch_count: int
    budget: int
    selected_functions: int
    skipped_oversized: int
    not_packed: int
    batches: list[WebBatch]
    dry_run: bool = False


@dataclass
class WebBatchIngestResult:
    batch_id: str
    round: int
    response_sha256: str
    results: list[WebFunctionIngestResult]
    active_file: Path
    history_dir: Path
    next_round: int | None
    exit_code: int = EXIT_OK
    idempotent: bool = False
    fence_policy: str = FENCE_POLICY
    summary: dict[str, Any] = field(default_factory=dict)


# ---------------------------------------------------------------------------
# Packing
# ---------------------------------------------------------------------------


def web_target_from_row(row: dict[str, Any]) -> WebTarget:
    retail_text_size = int(row.get("retail_text_size") or 0)
    instruction_count = int(
        row.get("retail_instruction_count")
        or instruction_count_from_text_size(retail_text_size)
    )
    frontier = str(row.get("frontier_kind") or "pending")
    direct = int(row.get("direct_call_count") or 0)
    unresolved = int(row.get("unresolved_call_count") or 0)
    indirect = bool(row.get("has_indirect_calls"))
    cost = difficulty_cost(
        retail_text_size=retail_text_size,
        instruction_count=instruction_count,
        direct_call_count=direct,
        unresolved_call_count=unresolved,
        has_indirect_calls=indirect,
        frontier_kind=frontier,
    )
    match = row.get("instruction_match")
    match_percent = float(match) if match is not None else None
    return WebTarget(
        target_id=str(row["target_id"]),
        unit=str(row.get("unit") or ""),
        symbol=str(row.get("symbol") or ""),
        source_path=str(row.get("source_path") or ""),
        status=str(row.get("status") or ""),
        match_percent=match_percent,
        frontier_kind=frontier,
        retail_text_size=retail_text_size,
        instruction_count=instruction_count,
        cost=cost,
        tier_sort=tier_sort_key(row.get("tier")),
        direct_call_count=direct,
        unresolved_call_count=unresolved,
        has_indirect_calls=indirect,
        workflow=str(row.get("workflow") or "improve"),
        branch_count=row.get("branch_count"),  # type: ignore[arg-type]
        basic_block_count=row.get("basic_block_count"),  # type: ignore[arg-type]
        loop_count=row.get("loop_count"),  # type: ignore[arg-type]
        jump_table_count=row.get("jump_table_count"),  # type: ignore[arg-type]
    )


def pack_candidates(
    candidates: list[WebTarget],
    *,
    batch_count: int,
    budget: int,
) -> tuple[list[WebBatch], list[WebTarget], list[WebTarget]]:
    """Return (batches, oversized, not_packed)."""
    if batch_count < 1:
        raise ValueError("batch_count must be >= 1")
    if budget < 1:
        raise ValueError("budget must be >= 1")

    oversized = [c for c in candidates if c.cost > budget]
    eligible = [c for c in candidates if c.cost <= budget]
    if len(eligible) < batch_count:
        raise ValueError(
            f"Only {len(eligible)} functions fit budget {budget}; "
            f"cannot create {batch_count} non-empty batches"
        )

    eligible = sorted(
        eligible,
        key=lambda c: (-c.cost, c.tier_sort, c.instruction_count, c.target_id),
    )
    batches = [
        WebBatch(batch_id=f"batch_{i + 1:03d}", index=i + 1, budget_limit=budget)
        for i in range(batch_count)
    ]
    for batch, candidate in zip(batches, eligible[:batch_count]):
        batch.add(candidate)

    not_packed: list[WebTarget] = []
    for candidate in eligible[batch_count:]:
        choices = [
            batch
            for batch in batches
            if batch.used_budget + candidate.cost <= budget
        ]
        if not choices:
            not_packed.append(candidate)
            continue
        choices.sort(
            key=lambda batch: (
                batch.used_budget,
                batch.count_unit(candidate.unit),
                batch.index,
            )
        )
        choices[0].add(candidate)

    if any(not batch.targets for batch in batches):
        raise ValueError("Internal packing error: empty batch after seed")
    return batches, oversized, not_packed


# ---------------------------------------------------------------------------
# Request / response text helpers
# ---------------------------------------------------------------------------


def _normalize_newlines(text: str) -> str:
    return text.replace("\r\n", "\n").replace("\r", "\n")


def _normalize_block_text(text: str) -> str:
    body = text.replace("\r\n", "\n").replace("\r", "\n")
    if body.startswith("\n"):
        body = body[1:]
    if body.endswith("\n"):
        body = body[:-1]
    return body + "\n"


def _utc_now() -> str:
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def _sha256_text(text: str) -> str:
    return hashlib.sha256(text.encode("utf-8")).hexdigest()


def _sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _atomic_write_text(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(text, encoding="utf-8", newline="\n")
    os.replace(tmp, path)


def _atomic_write_json(path: Path, payload: dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
    os.replace(tmp, path)


def _match_percent_text(value: float | None) -> str:
    if value is None:
        return "null"
    return f"{value:.4f}".rstrip("0").rstrip(".") if value != int(value) else str(int(value))


def _initial_instructions() -> str:
    return (
        "Return exactly one entry for every function.\n"
        "Return only WEB_DECOMP_BATCH_RESPONSE_V1 format.\n"
        "Do not claim match percentages.\n"
        "Each candidate must be a complete replacement for the requested function slot.\n"
        "Do not return assembly.\n"
        "Do not wrap candidates in Markdown code fences.\n"
        "Do not alter unrelated functions.\n"
    )


def _debug_instructions() -> str:
    return (
        "Improve every unresolved function using the local compiler and objdiff feedback.\n"
        "Return exactly one entry for every function.\n"
        "Return only WEB_DECOMP_BATCH_RESPONSE_V1 format.\n"
        "Each candidate must be a complete function-slot replacement.\n"
        "Do not wrap candidates in Markdown code fences.\n"
        "Do not claim match percentages.\n"
    )


def render_function_request_block(
    target: WebTarget,
    prompt: str,
    *,
    previous_round: int | None = None,
    best_candidate: str | None = None,
    local_evaluation: dict[str, Any] | None = None,
    compiler_feedback: str | None = None,
    objdiff_feedback: str | None = None,
    next_action: str | None = None,
) -> str:
    lines = [
        f'<<<FUNCTION id="{target.target_id}">>>',
        f"unit: {target.unit}",
        f"symbol: {target.symbol}",
        f"source_path: {target.source_path}",
        f"frontier: {target.frontier_kind}",
        f"status: {target.status}",
        f"known_match_percent: {_match_percent_text(target.match_percent)}",
        f"retail_instruction_count: {target.instruction_count}",
        f"difficulty_cost: {target.cost}",
    ]
    if previous_round is not None:
        lines.append(f"previous_round: {previous_round}")
    lines.extend(
        [
            "",
            "<<<PROMPT>>>",
            prompt.rstrip("\n"),
            "<<<END_PROMPT>>>",
        ]
    )
    if best_candidate is not None:
        lines.extend(
            [
                "",
                "<<<BEST_CANDIDATE>>>",
                best_candidate.rstrip("\n"),
                "<<<END_BEST_CANDIDATE>>>",
            ]
        )
    if local_evaluation is not None:
        lines.extend(
            [
                "",
                "<<<LOCAL_EVALUATION>>>",
                f"status: {local_evaluation.get('status', '')}",
                f"match_percent: {_match_percent_text(local_evaluation.get('match_percent'))}",
                f"accepted: {str(bool(local_evaluation.get('accepted'))).lower()}",
                f"blocked_reason: {local_evaluation.get('blocked_reason') or ''}",
                f"equivalence_status: {local_evaluation.get('equivalence') or local_evaluation.get('equivalence_status') or ''}",
                f"binary_fingerprint: {local_evaluation.get('binary_fingerprint') or ''}",
                "<<<END_LOCAL_EVALUATION>>>",
            ]
        )
    if compiler_feedback is not None:
        lines.extend(
            [
                "",
                "<<<COMPILER_FEEDBACK>>>",
                compiler_feedback.rstrip("\n"),
                "<<<END_COMPILER_FEEDBACK>>>",
            ]
        )
    if objdiff_feedback is not None:
        lines.extend(
            [
                "",
                "<<<OBJDIFF_FEEDBACK>>>",
                objdiff_feedback.rstrip("\n"),
                "<<<END_OBJDIFF_FEEDBACK>>>",
            ]
        )
    if next_action is not None:
        lines.extend(
            [
                "",
                "<<<NEXT_ACTION>>>",
                next_action.rstrip("\n"),
                "<<<END_NEXT_ACTION>>>",
            ]
        )
    lines.extend(["", "<<<END_FUNCTION>>>", ""])
    return "\n".join(lines)


def render_batch_request(
    batch: WebBatch,
    prompts: dict[str, str],
    *,
    round_number: int = 0,
    mode: str = "initial",
    debug_blocks: dict[str, dict[str, Any]] | None = None,
) -> str:
    response_filename = f"{batch.batch_id}.response.txt"
    instructions = _debug_instructions() if mode == "debug" else _initial_instructions()
    parts = [
        REQUEST_HEADER,
        f"batch_id: {batch.batch_id}",
        f"round: {round_number}",
        f"mode: {mode}",
        f"budget_limit: {batch.budget_limit}",
        f"budget_used: {batch.used_budget}",
        f"function_count: {len(batch.targets)}",
        f"response_filename: {response_filename}",
        "",
        "INSTRUCTIONS",
        instructions.rstrip("\n"),
        "END_INSTRUCTIONS",
        "",
    ]
    for target in batch.targets:
        prompt = prompts.get(target.target_id, "")
        extras = (debug_blocks or {}).get(target.target_id) or {}
        parts.append(
            render_function_request_block(
                target,
                prompt,
                previous_round=extras.get("previous_round"),
                best_candidate=extras.get("best_candidate"),
                local_evaluation=extras.get("local_evaluation"),
                compiler_feedback=extras.get("compiler_feedback"),
                objdiff_feedback=extras.get("objdiff_feedback"),
                next_action=extras.get("next_action"),
            )
        )
    parts.append(REQUEST_FOOTER)
    parts.append("")
    return "\n".join(parts)


def render_completion_receipt(
    *,
    batch_id: str,
    completed_round: int,
    original_function_count: int,
    full_match_count: int,
    equivalent_match_count: int,
    history_path: str,
    completed_at: str | None = None,
) -> str:
    return "\n".join(
        [
            COMPLETE_HEADER,
            f"batch_id: {batch_id}",
            f"completed_round: {completed_round}",
            f"original_function_count: {original_function_count}",
            f"full_match_count: {full_match_count}",
            f"equivalent_match_count: {equivalent_match_count}",
            f"completed_at: {completed_at or _utc_now()}",
            f"history_path: {history_path}",
            f"fence_policy: {FENCE_POLICY}",
            COMPLETE_FOOTER,
            "",
        ]
    )


def parse_request_metadata(text: str) -> dict[str, Any]:
    """Parse header metadata from an active request file (not full prompts)."""
    text = _normalize_newlines(text)
    lines = text.split("\n")
    if not lines or lines[0].strip() not in {REQUEST_HEADER, COMPLETE_HEADER}:
        raise WebBatchFormatError(
            f"expected {REQUEST_HEADER} or {COMPLETE_HEADER}", line=1
        )
    if lines[0].strip() == COMPLETE_HEADER:
        meta: dict[str, Any] = {"complete": True}
        for idx, raw in enumerate(lines[1:], start=2):
            line = raw.strip()
            if line == COMPLETE_FOOTER:
                break
            if ":" not in line:
                continue
            key, value = line.split(":", 1)
            meta[key.strip()] = value.strip()
        return meta

    meta = {"complete": False}
    for idx, raw in enumerate(lines[1:], start=2):
        line = raw.strip()
        if line.startswith("<<<FUNCTION") or line == "INSTRUCTIONS":
            break
        if ":" not in line:
            continue
        key, value = line.split(":", 1)
        key = key.strip()
        value = value.strip()
        if key in {"round", "budget_limit", "budget_used", "function_count"}:
            meta[key] = int(value)
        else:
            meta[key] = value
    required = {"batch_id", "round", "mode", "budget_limit", "function_count"}
    missing = required - set(meta)
    if missing:
        raise WebBatchFormatError(f"request missing fields: {sorted(missing)}")
    return meta


def parse_request_target_ids(text: str) -> list[str]:
    text = _normalize_newlines(text)
    ids: list[str] = []
    for line in text.split("\n"):
        match = re.fullmatch(r'<<<FUNCTION id="([^"]+)">>>', line.strip())
        if match:
            ids.append(match.group(1))
    return ids


# ---------------------------------------------------------------------------
# Response parser
# ---------------------------------------------------------------------------


def _parse_header_fields(lines: list[str], start: int) -> tuple[dict[str, str], int]:
    fields: dict[str, str] = {}
    idx = start
    while idx < len(lines):
        stripped = lines[idx].strip()
        if stripped.startswith("<<<") or stripped == RESPONSE_FOOTER:
            break
        if not stripped:
            idx += 1
            continue
        if ":" not in stripped:
            raise WebBatchFormatError(f"invalid header line: {stripped!r}", line=idx + 1)
        key, value = stripped.split(":", 1)
        fields[key.strip()] = value.strip()
        idx += 1
    return fields, idx


def _read_block(
    lines: list[str],
    start: int,
    begin: str,
    end: str,
    *,
    max_bytes: int,
) -> tuple[str, int]:
    if start >= len(lines) or lines[start].strip() != begin:
        raise WebBatchFormatError(f"expected {begin}", line=start + 1)
    body: list[str] = []
    idx = start + 1
    while idx < len(lines):
        if lines[idx].strip() == end:
            text = _normalize_block_text("\n".join(body))
            if len(text.encode("utf-8")) > max_bytes:
                raise WebBatchFormatError(
                    f"{begin} exceeds {max_bytes} bytes", line=start + 1
                )
            return text, idx + 1
        body.append(lines[idx])
        idx += 1
    raise WebBatchFormatError(f"unclosed block {begin}", line=start + 1)


def _reject_outer_fence(candidate: str, *, line: int) -> None:
    stripped = candidate.strip()
    if stripped.startswith("```"):
        raise WebBatchFormatError(
            "candidate must not be wrapped in Markdown code fences "
            f"(fence_policy={FENCE_POLICY})",
            line=line,
        )


def parse_web_response(text: str, *, max_functions: int | None = None) -> ParsedWebResponse:
    raw_bytes = text.encode("utf-8")
    if len(raw_bytes) > MAX_RESPONSE_BYTES:
        raise WebBatchFormatError(
            f"response exceeds {MAX_RESPONSE_BYTES} bytes ({len(raw_bytes)})"
        )
    text = _normalize_newlines(text)
    lines = text.split("\n")
    if not lines or lines[0].strip() != RESPONSE_HEADER:
        raise WebBatchFormatError(f"expected {RESPONSE_HEADER}", line=1)

    fields, idx = _parse_header_fields(lines, 1)
    if "batch_id" not in fields or "round" not in fields:
        raise WebBatchFormatError("response missing batch_id or round", line=2)
    try:
        round_number = int(fields["round"])
    except ValueError as exc:
        raise WebBatchFormatError(f"invalid round: {fields['round']!r}", line=2) from exc

    functions: dict[str, WebFunctionResponse] = {}
    while idx < len(lines):
        stripped = lines[idx].strip()
        if not stripped:
            idx += 1
            continue
        if stripped == RESPONSE_FOOTER:
            idx += 1
            break
        match = re.fullmatch(r'<<<FUNCTION id="([^"]+)">>>', stripped)
        if not match:
            raise WebBatchFormatError(
                f"expected function block or {RESPONSE_FOOTER}, got {stripped!r}",
                line=idx + 1,
            )
        target_id = match.group(1)
        if target_id in functions:
            raise WebBatchFormatError(f"duplicate function id {target_id!r}", line=idx + 1)
        idx += 1
        while idx < len(lines) and not lines[idx].strip():
            idx += 1
        if idx >= len(lines):
            raise WebBatchFormatError("unterminated function block", line=idx)

        kind = lines[idx].strip()
        candidate_source: str | None = None
        hypothesis = ""
        skip_reason = ""
        if kind == "<<<CANDIDATE>>>":
            candidate_source, idx = _read_block(
                lines, idx, "<<<CANDIDATE>>>", "<<<END_CANDIDATE>>>",
                max_bytes=MAX_CANDIDATE_BYTES,
            )
            if not candidate_source.strip():
                raise WebBatchFormatError("empty candidate", line=idx)
            _reject_outer_fence(candidate_source, line=idx)
            while idx < len(lines) and not lines[idx].strip():
                idx += 1
            if idx < len(lines) and lines[idx].strip() == "<<<HYPOTHESIS>>>":
                hypothesis, idx = _read_block(
                    lines, idx, "<<<HYPOTHESIS>>>", "<<<END_HYPOTHESIS>>>",
                    max_bytes=MAX_HYPOTHESIS_BYTES,
                )
                hypothesis = hypothesis.rstrip("\n")
        elif kind == "<<<SKIP>>>":
            skip_reason, idx = _read_block(
                lines, idx, "<<<SKIP>>>", "<<<END_SKIP>>>",
                max_bytes=MAX_HYPOTHESIS_BYTES,
            )
            skip_reason = skip_reason.rstrip("\n")
        else:
            raise WebBatchFormatError(
                f"function {target_id!r} must contain CANDIDATE or SKIP",
                line=idx + 1,
            )

        while idx < len(lines) and not lines[idx].strip():
            idx += 1
        if idx >= len(lines) or lines[idx].strip() != "<<<END_FUNCTION>>>":
            raise WebBatchFormatError(
                f"expected <<<END_FUNCTION>>> for {target_id!r}",
                line=idx + 1 if idx < len(lines) else len(lines),
            )
        idx += 1
        functions[target_id] = WebFunctionResponse(
            target_id=target_id,
            candidate_source=candidate_source,
            hypothesis=hypothesis,
            skip_reason=skip_reason,
        )
        if max_functions is not None and len(functions) > max_functions:
            raise WebBatchFormatError(
                f"response has more than {max_functions} functions",
                line=idx,
            )
    else:
        raise WebBatchFormatError(f"missing {RESPONSE_FOOTER}")

    # Trailing blank lines only
    while idx < len(lines):
        if lines[idx].strip():
            raise WebBatchFormatError(
                f"unexpected trailing content: {lines[idx]!r}", line=idx + 1
            )
        idx += 1

    return ParsedWebResponse(
        batch_id=fields["batch_id"],
        round=round_number,
        functions=functions,
    )


def render_web_response(
    *,
    batch_id: str,
    round_number: int,
    entries: list[WebFunctionResponse],
) -> str:
    """Test helper: render a valid response document."""
    parts = [
        RESPONSE_HEADER,
        f"batch_id: {batch_id}",
        f"round: {round_number}",
        "",
    ]
    for entry in entries:
        parts.append(f'<<<FUNCTION id="{entry.target_id}">>>')
        if entry.skip_reason and entry.candidate_source is None:
            parts.extend(
                [
                    "<<<SKIP>>>",
                    entry.skip_reason.rstrip("\n"),
                    "<<<END_SKIP>>>",
                ]
            )
        else:
            parts.extend(
                [
                    "<<<CANDIDATE>>>",
                    (entry.candidate_source or "").rstrip("\n"),
                    "<<<END_CANDIDATE>>>",
                    "<<<HYPOTHESIS>>>",
                    (entry.hypothesis or "").rstrip("\n"),
                    "<<<END_HYPOTHESIS>>>",
                ]
            )
        parts.extend(["<<<END_FUNCTION>>>", ""])
    parts.append(RESPONSE_FOOTER)
    parts.append("")
    return "\n".join(parts)


# ---------------------------------------------------------------------------
# Manifest helpers
# ---------------------------------------------------------------------------


def default_output_dir(harness: "Harness") -> Path:
    return (harness.root / "web_batches").resolve()


def load_manifest(path: Path) -> dict[str, Any]:
    return json.loads(path.read_text(encoding="utf-8"))


def find_batch(manifest: dict[str, Any], batch_id: str) -> dict[str, Any]:
    for batch in manifest.get("batches") or []:
        if batch.get("batch_id") == batch_id:
            return batch
    raise WebBatchStaleError(f"batch_id {batch_id!r} not found in manifest")


def active_batch_files_exist(output_dir: Path) -> bool:
    if not output_dir.is_dir():
        return False
    return any(output_dir.glob("batch_*.txt"))


# ---------------------------------------------------------------------------
# Export
# ---------------------------------------------------------------------------


def create_web_batches(
    harness: "Harness",
    *,
    batch_count: int,
    budget: int = 50,
    selection: str = "ready",
    certified_funcs: bool = True,
    tu: str | None = None,
    randomize: bool = False,
    output_dir: Path | None = None,
    force: bool = False,
    dry_run: bool = False,
    random_seed: int | None = None,
) -> WebBatchCreateResult:
    if batch_count < 1:
        raise ValueError("--batches must be >= 1")
    if budget < 1:
        raise ValueError("--budget must be >= 1")

    out = (output_dir or default_output_dir(harness)).resolve()
    if not dry_run and active_batch_files_exist(out) and not force:
        raise FileExistsError(
            f"Active batch files already exist under {out}; pass --force to replace"
        )

    rows_fn = getattr(harness.adapter, "web_batch_candidates", None)
    if rows_fn is None:
        raise ValueError("Configured project adapter does not support web_batch_candidates")

    with harness._adapter_lock:
        rows = list(
            rows_fn(
                selection=selection,
                certified_funcs=certified_funcs,
                tu=tu,
                randomize=False,
            )
        )

    seed = random_seed
    if randomize:
        if seed is None:
            seed = random.randrange(1 << 31)
        rng = random.Random(seed)
        rng.shuffle(rows)

    targets = [web_target_from_row(row) for row in rows]
    batches, oversized, not_packed = pack_candidates(
        targets, batch_count=batch_count, budget=budget
    )

    if dry_run:
        return WebBatchCreateResult(
            output_dir=out,
            batch_count=batch_count,
            budget=budget,
            selected_functions=sum(len(b.targets) for b in batches),
            skipped_oversized=len(oversized),
            not_packed=len(not_packed),
            batches=batches,
            dry_run=True,
        )

    if force and out.exists():
        archive = out / "history" / f"archived_export_{_utc_now().replace(':', '')}"
        archive.mkdir(parents=True, exist_ok=True)
        for path in out.glob("batch_*.txt"):
            shutil.copy2(path, archive / path.name)
        manifest_path = out / "manifest.json"
        if manifest_path.is_file():
            shutil.copy2(manifest_path, archive / "manifest.json")
        for path in out.glob("batch_*.txt"):
            path.unlink()

    out.mkdir(parents=True, exist_ok=True)
    (out / "history").mkdir(exist_ok=True)

    batch_entries: list[dict[str, Any]] = []
    for batch in batches:
        prompts: dict[str, str] = {}
        for target in batch.targets:
            prompts[target.target_id] = harness.build_external_prompt(
                target.workflow, target.target_id
            )
        request_text = render_batch_request(batch, prompts, round_number=0, mode="initial")
        active_path = out / f"{batch.batch_id}.txt"
        _atomic_write_text(active_path, request_text)
        batch_entries.append(
            {
                "batch_id": batch.batch_id,
                "active_file": str(active_path.relative_to(harness.root))
                if active_path.is_relative_to(harness.root)
                else str(active_path),
                "response_filename": f"{batch.batch_id}.response.txt",
                "round": 0,
                "status": "OPEN",
                "budget_limit": batch.budget_limit,
                "budget_used": batch.used_budget,
                "original_target_ids": [t.target_id for t in batch.targets],
                "remaining_target_ids": [t.target_id for t in batch.targets],
                "promoted_target_ids": [],
                "promoted_full_match_ids": [],
                "promoted_equivalent_match_ids": [],
                "best_by_target": {},
                "last_response_sha256": None,
                "last_ingest_at": None,
                "target_meta": {
                    t.target_id: {
                        "unit": t.unit,
                        "symbol": t.symbol,
                        "source_path": t.source_path,
                        "status": t.status,
                        "match_percent": t.match_percent,
                        "frontier_kind": t.frontier_kind,
                        "retail_text_size": t.retail_text_size,
                        "instruction_count": t.instruction_count,
                        "cost": t.cost,
                        "tier_sort": t.tier_sort,
                        "direct_call_count": t.direct_call_count,
                        "unresolved_call_count": t.unresolved_call_count,
                        "has_indirect_calls": t.has_indirect_calls,
                        "workflow": t.workflow,
                    }
                    for t in batch.targets
                },
            }
        )

    manifest = {
        "schema_version": SCHEMA_VERSION,
        "created_at": _utc_now(),
        "config_path": str(
            harness.config_path.relative_to(harness.root)
            if harness.config_path.is_relative_to(harness.root)
            else harness.config_path
        ),
        "budget_default": budget,
        "selection": selection,
        "certified_funcs": certified_funcs,
        "tu": tu,
        "randomized": randomize,
        "random_seed": seed,
        "fence_policy": FENCE_POLICY,
        "acceptance_policy": "FULL_MATCH_or_certified_EQUIVALENT_MATCH",
        "batches": batch_entries,
        "skipped": [
            {
                "target_id": t.target_id,
                "reason": "cost_exceeds_budget",
                "cost": t.cost,
            }
            for t in oversized
        ],
        "not_packed": [
            {"target_id": t.target_id, "cost": t.cost, "unit": t.unit}
            for t in not_packed
        ],
    }
    _atomic_write_json(out / "manifest.json", manifest)
    return WebBatchCreateResult(
        output_dir=out,
        batch_count=batch_count,
        budget=budget,
        selected_functions=sum(len(b.targets) for b in batches),
        skipped_oversized=len(oversized),
        not_packed=len(not_packed),
        batches=batches,
        dry_run=False,
    )


# ---------------------------------------------------------------------------
# Ingest helpers
# ---------------------------------------------------------------------------


def _evaluation_dict(evaluation: Any) -> dict[str, Any]:
    if evaluation is None:
        return {}
    if isinstance(evaluation, dict):
        return dict(evaluation)
    if hasattr(evaluation, "__dataclass_fields__"):
        return asdict(evaluation)
    payload: dict[str, Any] = {}
    for key in (
        "status",
        "match_percent",
        "accepted",
        "symbol_accepted",
        "detail",
        "equivalence",
        "equivalence_status",
        "blocked_reason",
        "promising",
        "metrics",
        "size_ok",
    ):
        if hasattr(evaluation, key):
            payload[key] = getattr(evaluation, key)
    return payload


def _status_upper(evaluation: dict[str, Any]) -> str:
    return str(evaluation.get("status") or "").upper()


def _is_web_accepted(evaluation: dict[str, Any]) -> bool:
    """FULL_MATCH or certified EQUIVALENT_MATCH (harness symbol_accepted)."""
    if evaluation.get("symbol_accepted"):
        status = _status_upper(evaluation)
        if status == "EQUIVALENT_MATCH":
            # Require proved equivalence; certificate attached at promote time.
            equiv = str(
                evaluation.get("equivalence")
                or evaluation.get("equivalence_status")
                or ""
            ).upper()
            metrics = evaluation.get("metrics") or {}
            if metrics.get("symbol_accepted"):
                return True
            return equiv in {"EQUIVALENT", "EQUIVALENT_MATCH"} or bool(
                evaluation.get("accepted")
            )
        return status == "FULL_MATCH" or bool(evaluation.get("accepted"))
    metrics = evaluation.get("metrics") or {}
    if metrics.get("symbol_accepted"):
        return True
    status = _status_upper(evaluation)
    if status == "FULL_MATCH" and (
        float(evaluation.get("match_percent") or 0.0) >= 100.0
        or bool(evaluation.get("accepted"))
    ):
        return True
    if status == "EQUIVALENT_MATCH" and bool(evaluation.get("accepted")):
        return True
    return False


def _rank_tuple(evaluation: dict[str, Any]) -> tuple:
    accepted = 1 if _is_web_accepted(evaluation) else 0
    compiles = 0 if _status_upper(evaluation) == "COMPILE_ERROR" else 1
    match = float(evaluation.get("match_percent") or 0.0)
    return (accepted, compiles, match)


def _compiler_feedback_text(detail: str, status: str) -> str:
    if status.upper() != "COMPILE_ERROR" and not detail:
        return "Compilation succeeded."
    diagnostics = normalize_compile_output(detail or "")
    root = select_root_diagnostic(diagnostics)
    lines: list[str] = []
    if root is not None:
        loc = ""
        if root.file:
            loc = root.file
            if root.line is not None:
                loc += f":{root.line}"
            loc += ": "
        lines.append("Root diagnostic:")
        lines.append(f"{loc}{root.message}")
        lines.append("")
        lines.append(f"Likely category: {root.category}")
    else:
        lines.append((detail or "Compilation failed.").strip()[:4000])
    return "\n".join(lines).rstrip() + "\n"


def _objdiff_feedback_text(evaluation: dict[str, Any]) -> str:
    metrics = evaluation.get("metrics") or {}
    feedback = metrics.get("binary_feedback")
    if feedback:
        text = format_objdiff_feedback_text(feedback)
        if text:
            return text.rstrip() + "\n"
    detail = str(evaluation.get("detail") or "").strip()
    if detail:
        return detail[:6000].rstrip() + "\n"
    return "No bounded objdiff feedback available.\n"


def _next_action_text(evaluation: dict[str, Any], outcome: str) -> str:
    status = _status_upper(evaluation)
    if outcome == "MISSING_RESPONSE":
        return "Provide a candidate for this function; it was missing from the previous response.\n"
    if outcome == "NO_CANDIDATE":
        return "Previous response skipped this function. Produce a credible candidate.\n"
    if outcome == "PROMOTION_FAILED":
        return (
            "Isolated evaluation looked accepted but guarded promotion failed. "
            "Preserve semantics and re-check the first mismatch window.\n"
        )
    if status == "COMPILE_ERROR":
        return "Fix the root compiler diagnostic first; keep the function-slot replacement complete.\n"
    if evaluation.get("blocked_reason") == "unvalidated_callee":
        return (
            "Do not spend time micro-matching until unresolved callees are accepted. "
            "Preserve semantics and keep declarations consistent.\n"
        )
    return (
        "Preserve current semantics. Focus on the first mismatch window and "
        "any register/operand or relocation differences in the objdiff feedback.\n"
    )


def _relpath(root: Path, path: Path) -> str:
    try:
        return str(path.resolve().relative_to(root.resolve()))
    except ValueError:
        return str(path)


# ---------------------------------------------------------------------------
# Ingest
# ---------------------------------------------------------------------------


def ingest_web_batch_output(
    harness: "Harness",
    response_path: Path,
    *,
    owner: str = "chatgpt-web",
    dry_run: bool = False,
    resume: bool = False,
    output_dir: Path | None = None,
) -> WebBatchIngestResult:
    response_path = response_path.resolve()
    if not response_path.is_file():
        raise FileNotFoundError(f"response file not found: {response_path}")

    out = (output_dir or default_output_dir(harness)).resolve()
    manifest_path = out / "manifest.json"
    if not manifest_path.is_file():
        raise FileNotFoundError(f"manifest not found: {manifest_path}")

    response_bytes = response_path.read_bytes()
    if len(response_bytes) > MAX_RESPONSE_BYTES:
        raise WebBatchFormatError(
            f"response exceeds {MAX_RESPONSE_BYTES} bytes ({len(response_bytes)})"
        )
    response_text = response_bytes.decode("utf-8")
    response_sha = _sha256_bytes(response_bytes)

    # Phase A — validate without mutation
    try:
        parsed = parse_web_response(response_text)
    except WebBatchFormatError:
        raise

    manifest = load_manifest(manifest_path)
    batch = find_batch(manifest, parsed.batch_id)
    expected_round = int(batch.get("round") or 0)

    active_rel = batch.get("active_file") or f"web_batches/{parsed.batch_id}.txt"
    active_path = (harness.root / active_rel).resolve()
    if not active_path.is_file():
        active_path = out / f"{parsed.batch_id}.txt"
    if not active_path.is_file():
        raise FileNotFoundError(f"active batch file missing: {active_path}")

    history_root = out / "history" / parsed.batch_id
    round_dir = history_root / f"round_{parsed.round:03d}"
    summary_path = round_dir / "summary.json"
    state_path = round_dir / "ingest_state.json"

    def _completed_same_sha() -> bool:
        if not summary_path.is_file():
            return False
        if batch.get("last_response_sha256") == response_sha:
            return True
        if not state_path.is_file():
            return False
        try:
            prior = json.loads(state_path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError):
            return False
        return (
            prior.get("status") == "COMPLETE"
            and prior.get("response_sha256") == response_sha
        )

    if _completed_same_sha() and not resume:
        summary = json.loads(summary_path.read_text(encoding="utf-8"))
        return WebBatchIngestResult(
            batch_id=parsed.batch_id,
            round=parsed.round,
            response_sha256=response_sha,
            results=[],
            active_file=active_path,
            history_dir=round_dir,
            next_round=summary.get("next_round"),
            exit_code=EXIT_OK,
            idempotent=True,
            summary=summary,
        )

    if parsed.round != expected_round:
        raise WebBatchStaleError(
            f"stale response round: expected {expected_round}, got {parsed.round}; "
            f"active_file={batch.get('active_file')}"
        )

    remaining = list(batch.get("remaining_target_ids") or [])
    remaining_set = set(remaining)
    unknown = sorted(set(parsed.functions) - remaining_set)
    if unknown:
        raise WebBatchFormatError(f"unknown function ids for batch: {unknown}")

    if state_path.is_file() and not dry_run:
        prior = json.loads(state_path.read_text(encoding="utf-8"))
        prior_status = str(prior.get("status") or "")
        prior_sha = str(prior.get("response_sha256") or "")
        if prior_status == "EVALUATING":
            if not resume:
                raise WebBatchStaleError(
                    f"incomplete ingest at {round_dir}; re-run with --resume "
                    "after inspecting functions/, or delete the incomplete round directory"
                )
            if prior_sha and prior_sha != response_sha:
                raise WebBatchStaleError(
                    "cannot --resume: response SHA differs from incomplete ingest_state.json"
                )
        elif prior_status == "COMPLETE" and prior_sha == response_sha and not resume:
            summary = (
                json.loads(summary_path.read_text(encoding="utf-8"))
                if summary_path.is_file()
                else {}
            )
            return WebBatchIngestResult(
                batch_id=parsed.batch_id,
                round=parsed.round,
                response_sha256=response_sha,
                results=[],
                active_file=active_path,
                history_dir=round_dir,
                next_round=summary.get("next_round"),
                exit_code=EXIT_OK,
                idempotent=True,
                summary=summary,
            )
        elif prior_status == "COMPLETE" and prior_sha != response_sha:
            raise WebBatchStaleError(
                f"round {parsed.round} already completed with a different response; "
                "refusing to fork batch history"
            )

    request_text = active_path.read_text(encoding="utf-8")
    request_ids = parse_request_target_ids(request_text)
    # Prefer stable request order; fall back to manifest remaining
    ordered_ids = [tid for tid in request_ids if tid in remaining_set] or list(remaining)

    if dry_run:
        # Parse-only / evaluate without promote or overwrite is handled below with dry_run flags
        pass

    # Phase B — archive inputs
    if not dry_run:
        round_dir.mkdir(parents=True, exist_ok=True)
        (round_dir / "functions").mkdir(exist_ok=True)
        if not resume or not (round_dir / "request.txt").is_file():
            (round_dir / "request.txt").write_text(request_text, encoding="utf-8", newline="\n")
        if not resume or not (round_dir / "response.txt").is_file():
            (round_dir / "response.txt").write_bytes(response_bytes)
        (round_dir / "response.sha256").write_text(response_sha + "\n", encoding="utf-8")
        if not resume or not state_path.is_file():
            _atomic_write_json(
                state_path,
                {
                    "status": "EVALUATING",
                    "batch_id": parsed.batch_id,
                    "round": parsed.round,
                    "response_sha256": response_sha,
                    "started_at": _utc_now(),
                    "functions": {},
                    "fence_policy": FENCE_POLICY,
                },
            )

    prior_state: dict[str, Any] = {}
    if resume and state_path.is_file():
        prior_state = json.loads(state_path.read_text(encoding="utf-8"))

    results: list[WebFunctionIngestResult] = []
    exit_code = EXIT_OK
    best_by_target: dict[str, Any] = dict(batch.get("best_by_target") or {})
    promoted_ids: list[str] = list(batch.get("promoted_target_ids") or [])
    promoted_full: list[str] = list(batch.get("promoted_full_match_ids") or [])
    promoted_equiv: list[str] = list(batch.get("promoted_equivalent_match_ids") or [])
    unresolved_targets: list[WebTarget] = []
    debug_blocks: dict[str, dict[str, Any]] = {}

    meta_by_id = {
        tid: dict(meta)
        for tid, meta in (batch.get("target_meta") or {}).items()
    }

    def _target_from_meta(target_id: str) -> WebTarget:
        meta = meta_by_id.get(target_id) or {"target_id": target_id}
        meta.setdefault("target_id", target_id)
        return web_target_from_row(meta)

    for target_id in ordered_ids:
        fn_dir = round_dir / "functions" / target_id
        prior_fn = (prior_state.get("functions") or {}).get(target_id) or {}

        # Resume skip when already finalized for this function
        if resume and prior_fn.get("finalized") and (fn_dir / "result.json").is_file():
            saved = json.loads((fn_dir / "result.json").read_text(encoding="utf-8"))
            result = WebFunctionIngestResult(**{
                **saved,
                "artifacts": saved.get("artifacts") or {},
                "evaluation": saved.get("evaluation") or {},
            })
            results.append(result)
            if result.promoted:
                continue
            # Rebuild unresolved debug state from artifacts
            target = _target_from_meta(target_id)
            evaluation = result.evaluation
            best_source = result.candidate_source
            if best_source is None and (fn_dir / "candidate.cpp").is_file():
                best_source = (fn_dir / "candidate.cpp").read_text(encoding="utf-8")
            unresolved_targets.append(
                _refresh_target_status(target, evaluation, best_source)
            )
            debug_blocks[target_id] = _build_debug_block(
                harness,
                target_id,
                target,
                evaluation=evaluation,
                best_source=best_source,
                outcome=result.outcome,
                previous_round=parsed.round,
            )
            continue

        entry = parsed.functions.get(target_id)
        if entry is None:
            result = WebFunctionIngestResult(
                target_id=target_id,
                outcome="MISSING_RESPONSE",
                status="MISSING_RESPONSE",
                match_percent=None,
                promoted=False,
                candidate_source=None,
                detail="function missing from response",
            )
            results.append(result)
            target = _target_from_meta(target_id)
            # Keep previous best if any
            prev_best = best_by_target.get(target_id) or {}
            best_source = prev_best.get("source")
            evaluation = {
                "status": prev_best.get("best_status") or "MISSING_RESPONSE",
                "match_percent": prev_best.get("best_match_percent"),
                "accepted": False,
                "symbol_accepted": False,
            }
            unresolved_targets.append(target)
            debug_blocks[target_id] = _build_debug_block(
                harness,
                target_id,
                target,
                evaluation=evaluation,
                best_source=best_source,
                outcome="MISSING_RESPONSE",
                previous_round=parsed.round,
            )
            _persist_function_result(fn_dir, result, dry_run=dry_run)
            continue

        if entry.candidate_source is None:
            result = WebFunctionIngestResult(
                target_id=target_id,
                outcome="NO_CANDIDATE",
                status="NO_CANDIDATE",
                match_percent=None,
                promoted=False,
                candidate_source=None,
                detail=entry.skip_reason or "skipped",
            )
            results.append(result)
            target = _target_from_meta(target_id)
            prev_best = best_by_target.get(target_id) or {}
            best_source = prev_best.get("source")
            evaluation = {
                "status": prev_best.get("best_status") or "NO_CANDIDATE",
                "match_percent": prev_best.get("best_match_percent"),
                "accepted": False,
                "symbol_accepted": False,
            }
            unresolved_targets.append(target)
            debug_blocks[target_id] = _build_debug_block(
                harness,
                target_id,
                target,
                evaluation=evaluation,
                best_source=best_source,
                outcome="NO_CANDIDATE",
                previous_round=parsed.round,
            )
            _persist_function_result(fn_dir, result, dry_run=dry_run)
            continue

        # Evaluate candidate
        candidate = Candidate(
            source=entry.candidate_source,
            hypothesis=entry.hypothesis or f"web-batch {parsed.batch_id} r{parsed.round}",
        )
        evaluation_obj: Any
        evaluation: dict[str, Any]
        try:
            evaluation_obj = harness.evaluate_external_candidate(
                target_id,
                candidate.source,
                hypothesis=candidate.hypothesis,
            )
            evaluation = _evaluation_dict(evaluation_obj)
        except Exception as exc:
            exit_code = EXIT_INFRA
            evaluation = {
                "status": "EVALUATION_ERROR",
                "match_percent": 0.0,
                "accepted": False,
                "symbol_accepted": False,
                "detail": f"{type(exc).__name__}: {exc}",
            }
            result = WebFunctionIngestResult(
                target_id=target_id,
                outcome="EVALUATION_ERROR",
                status="EVALUATION_ERROR",
                match_percent=0.0,
                promoted=False,
                candidate_source=entry.candidate_source,
                detail=str(exc),
                evaluation=evaluation,
            )
            results.append(result)
            target = _target_from_meta(target_id)
            unresolved_targets.append(target)
            debug_blocks[target_id] = _build_debug_block(
                harness,
                target_id,
                target,
                evaluation=evaluation,
                best_source=entry.candidate_source,
                outcome="EVALUATION_ERROR",
                previous_round=parsed.round,
            )
            _persist_function_result(
                fn_dir,
                result,
                candidate_source=entry.candidate_source,
                dry_run=dry_run,
            )
            continue

        status = _status_upper(evaluation)
        match_percent = (
            float(evaluation["match_percent"])
            if evaluation.get("match_percent") is not None
            else None
        )
        artifacts = _write_eval_artifacts(
            fn_dir,
            candidate_source=entry.candidate_source,
            evaluation=evaluation,
            dry_run=dry_run,
        )

        promoted = False
        outcome: IngestOutcome = "UNRESOLVED"
        detail = str(evaluation.get("detail") or "")
        if status == "COMPILE_ERROR":
            outcome = "COMPILE_ERROR"
        elif _is_web_accepted(evaluation):
            if dry_run:
                outcome = (
                    "FULL_MATCH_PROMOTED"
                    if status == "FULL_MATCH"
                    else "EQUIVALENT_MATCH_PROMOTED"
                )
                promoted = False
                detail = "dry-run: would promote"
            else:
                try:
                    promo = harness.promote_external_accepted(
                        target_id,
                        entry.candidate_source,
                        evaluation,
                        owner=owner,
                        batch_id=parsed.batch_id,
                        batch_round=parsed.round,
                        dry_run=False,
                    )
                    if isinstance(promo, PromotionResult):
                        ok = bool(promo.promoted)
                        reason = promo.reason
                    elif isinstance(promo, dict):
                        ok = bool(promo.get("promoted"))
                        reason = str(promo.get("reason") or "")
                    else:
                        ok = bool(promo)
                        reason = str(promo)
                    if ok:
                        promoted = True
                        if status == "FULL_MATCH":
                            outcome = "FULL_MATCH_PROMOTED"
                            promoted_full.append(target_id)
                        else:
                            outcome = "EQUIVALENT_MATCH_PROMOTED"
                            promoted_equiv.append(target_id)
                        if target_id not in promoted_ids:
                            promoted_ids.append(target_id)
                        detail = reason or "promoted"
                    else:
                        exit_code = EXIT_INFRA
                        outcome = "PROMOTION_FAILED"
                        detail = reason or "promotion failed"
                except Exception as exc:
                    exit_code = EXIT_INFRA
                    outcome = "PROMOTION_FAILED"
                    detail = f"{type(exc).__name__}: {exc}"

        result = WebFunctionIngestResult(
            target_id=target_id,
            outcome=outcome,
            status=status or outcome,
            match_percent=match_percent,
            promoted=promoted,
            candidate_source=entry.candidate_source,
            detail=detail,
            artifacts=artifacts,
            evaluation=evaluation,
        )
        results.append(result)
        _persist_function_result(
            fn_dir,
            result,
            candidate_source=entry.candidate_source,
            evaluation=evaluation,
            dry_run=dry_run,
        )

        # Update best candidate retention for unresolved
        if not promoted:
            prev = best_by_target.get(target_id) or {}
            prev_eval = {
                "status": prev.get("best_status"),
                "match_percent": prev.get("best_match_percent"),
                "accepted": False,
                "symbol_accepted": False,
            }
            keep_source = entry.candidate_source
            keep_eval = evaluation
            if status == "COMPILE_ERROR" and prev.get("source"):
                # Do not regress to a worse candidate
                if _rank_tuple(prev_eval) > _rank_tuple(evaluation):
                    keep_source = prev.get("source")
                    keep_eval = prev_eval
            elif prev.get("source") and _rank_tuple(prev_eval) > _rank_tuple(evaluation):
                keep_source = prev.get("source")
                keep_eval = prev_eval
            best_by_target[target_id] = {
                "best_candidate_sha256": _sha256_text(keep_source or ""),
                "best_status": _status_upper(keep_eval) or status,
                "best_match_percent": keep_eval.get("match_percent"),
                "best_round": parsed.round,
                "source": keep_source,
            }
            target = _refresh_target_status(
                _target_from_meta(target_id), keep_eval, keep_source
            )
            unresolved_targets.append(target)
            debug_blocks[target_id] = _build_debug_block(
                harness,
                target_id,
                target,
                evaluation=keep_eval,
                best_source=keep_source,
                outcome=outcome,
                previous_round=parsed.round,
            )

        if not dry_run:
            state = json.loads(state_path.read_text(encoding="utf-8"))
            state.setdefault("functions", {})[target_id] = {
                "outcome": outcome,
                "promoted": promoted,
                "finalized": True,
            }
            _atomic_write_json(state_path, state)

    # Summary counts
    full_match = sum(1 for r in results if r.outcome == "FULL_MATCH_PROMOTED")
    equiv_match = sum(1 for r in results if r.outcome == "EQUIVALENT_MATCH_PROMOTED")
    compile_error = sum(1 for r in results if r.outcome == "COMPILE_ERROR")
    missing = sum(1 for r in results if r.outcome == "MISSING_RESPONSE")
    promo_failed = sum(1 for r in results if r.outcome == "PROMOTION_FAILED")
    unresolved = len(unresolved_targets)
    next_round = None if unresolved == 0 else parsed.round + 1

    summary = {
        "batch_id": parsed.batch_id,
        "round": parsed.round,
        "submitted": len(ordered_ids),
        "evaluated": sum(
            1
            for r in results
            if r.outcome
            not in {"MISSING_RESPONSE", "NO_CANDIDATE"}
        ),
        "full_match": full_match,
        "equivalent_match": equiv_match,
        "promoted": full_match + equiv_match,
        "unresolved": unresolved,
        "compile_error": compile_error,
        "missing_response": missing,
        "promotion_failed": promo_failed,
        "next_round": next_round,
        "fence_policy": FENCE_POLICY,
        "response_sha256": response_sha,
        "dry_run": dry_run,
    }

    if dry_run:
        return WebBatchIngestResult(
            batch_id=parsed.batch_id,
            round=parsed.round,
            response_sha256=response_sha,
            results=results,
            active_file=active_path,
            history_dir=round_dir,
            next_round=next_round,
            exit_code=exit_code,
            summary=summary,
        )

    # Phase G — overwrite active batch
    history_rel = _relpath(harness.root, history_root)
    if unresolved == 0:
        next_text = render_completion_receipt(
            batch_id=parsed.batch_id,
            completed_round=parsed.round,
            original_function_count=len(batch.get("original_target_ids") or ordered_ids),
            full_match_count=len(
                set(promoted_full)
                | {
                    tid
                    for tid in (batch.get("promoted_full_match_ids") or [])
                }
            ),
            equivalent_match_count=len(set(promoted_equiv)),
            history_path=history_rel,
        )
        new_status: BatchStatus = "COMPLETE"
    else:
        # Preserve original budget_limit; recompute used from remaining costs
        budget_limit = int(batch.get("budget_limit") or 50)
        web_batch = WebBatch(
            batch_id=parsed.batch_id,
            index=int(parsed.batch_id.split("_")[-1])
            if "_" in parsed.batch_id
            else 1,
            budget_limit=budget_limit,
            targets=unresolved_targets,
        )
        prompts: dict[str, str] = {}
        for target in unresolved_targets:
            block = debug_blocks.get(target.target_id) or {}
            prompts[target.target_id] = block.get("prompt") or harness.build_external_prompt(
                "improve",
                target.target_id,
                repair_context=block.get("repair_context"),
            )
        next_text = render_batch_request(
            web_batch,
            prompts,
            round_number=parsed.round + 1,
            mode="debug",
            debug_blocks=debug_blocks,
        )
        # Validate rendered request metadata before overwrite
        parse_request_metadata(next_text)
        new_status = "DEBUG"

    # Archive previous active request copy already saved; overwrite atomically
    try:
        _atomic_write_text(active_path, next_text)
    except OSError as exc:
        exit_code = EXIT_INFRA
        summary["active_overwrite_error"] = str(exc)
        _atomic_write_json(
            state_path,
            {
                "status": "ERROR",
                "batch_id": parsed.batch_id,
                "round": parsed.round,
                "response_sha256": response_sha,
                "error": f"active overwrite failed: {exc}",
                "summary": summary,
            },
        )
        _atomic_write_json(round_dir / "summary.json", summary)
        (round_dir / "summary.txt").write_text(
            _format_summary_text(summary), encoding="utf-8"
        )
        return WebBatchIngestResult(
            batch_id=parsed.batch_id,
            round=parsed.round,
            response_sha256=response_sha,
            results=results,
            active_file=active_path,
            history_dir=round_dir,
            next_round=None,
            exit_code=exit_code,
            summary=summary,
        )

    # Update manifest only after successful overwrite
    batch["round"] = 0 if unresolved == 0 and new_status == "COMPLETE" else (
        parsed.round + 1 if unresolved else parsed.round
    )
    if new_status == "COMPLETE":
        batch["round"] = parsed.round
    else:
        batch["round"] = parsed.round + 1
    batch["status"] = new_status
    batch["remaining_target_ids"] = [t.target_id for t in unresolved_targets]
    batch["promoted_target_ids"] = promoted_ids
    batch["promoted_full_match_ids"] = sorted(set(promoted_full))
    batch["promoted_equivalent_match_ids"] = sorted(set(promoted_equiv))
    batch["best_by_target"] = {
        tid: {k: v for k, v in meta.items() if k != "source"}
        | ({"source": meta["source"]} if meta.get("source") else {})
        for tid, meta in best_by_target.items()
        if tid in batch["remaining_target_ids"]
    }
    batch["budget_used"] = sum(t.cost for t in unresolved_targets)
    batch["last_response_sha256"] = response_sha
    batch["last_ingest_at"] = _utc_now()

    # Persist sources for best candidates under history for resume/debug
    for tid, meta in list(batch["best_by_target"].items()):
        source = best_by_target.get(tid, {}).get("source")
        if source is None:
            continue
        best_path = round_dir / "functions" / tid / "best_candidate.cpp"
        best_path.parent.mkdir(parents=True, exist_ok=True)
        best_path.write_text(source, encoding="utf-8", newline="\n")

    _atomic_write_json(manifest_path, manifest)
    _atomic_write_json(round_dir / "summary.json", summary)
    (round_dir / "summary.txt").write_text(
        _format_summary_text(summary), encoding="utf-8"
    )
    _atomic_write_json(
        state_path,
        {
            "status": "COMPLETE",
            "batch_id": parsed.batch_id,
            "round": parsed.round,
            "response_sha256": response_sha,
            "completed_at": _utc_now(),
            "summary": summary,
            "fence_policy": FENCE_POLICY,
            "functions": {
                r.target_id: {
                    "outcome": r.outcome,
                    "promoted": r.promoted,
                    "finalized": True,
                }
                for r in results
            },
        },
    )

    return WebBatchIngestResult(
        batch_id=parsed.batch_id,
        round=parsed.round,
        response_sha256=response_sha,
        results=results,
        active_file=active_path,
        history_dir=round_dir,
        next_round=next_round,
        exit_code=exit_code,
        summary=summary,
    )


def _persist_function_result(
    fn_dir: Path,
    result: WebFunctionIngestResult,
    *,
    candidate_source: str | None = None,
    evaluation: dict[str, Any] | None = None,
    dry_run: bool = False,
) -> None:
    if dry_run:
        return
    fn_dir.mkdir(parents=True, exist_ok=True)
    if candidate_source is not None:
        (fn_dir / "candidate.cpp").write_text(
            candidate_source, encoding="utf-8", newline="\n"
        )
    if evaluation is not None:
        (fn_dir / "evaluation.json").write_text(
            json.dumps(evaluation, indent=2, default=str) + "\n", encoding="utf-8"
        )
    payload = {
        "target_id": result.target_id,
        "outcome": result.outcome,
        "status": result.status,
        "match_percent": result.match_percent,
        "promoted": result.promoted,
        "candidate_source": result.candidate_source,
        "detail": result.detail,
        "artifacts": result.artifacts,
        "evaluation": result.evaluation or evaluation or {},
    }
    (fn_dir / "result.json").write_text(
        json.dumps(payload, indent=2, default=str) + "\n", encoding="utf-8"
    )


def _write_eval_artifacts(
    fn_dir: Path,
    *,
    candidate_source: str,
    evaluation: dict[str, Any],
    dry_run: bool,
) -> dict[str, str]:
    if dry_run:
        return {}
    fn_dir.mkdir(parents=True, exist_ok=True)
    (fn_dir / "candidate.cpp").write_text(
        candidate_source, encoding="utf-8", newline="\n"
    )
    (fn_dir / "evaluation.json").write_text(
        json.dumps(evaluation, indent=2, default=str) + "\n", encoding="utf-8"
    )
    detail = str(evaluation.get("detail") or "")
    (fn_dir / "compile.txt").write_text(detail + ("\n" if detail else ""), encoding="utf-8")
    objdiff = _objdiff_feedback_text(evaluation)
    (fn_dir / "objdiff.txt").write_text(objdiff, encoding="utf-8")
    equiv = {
        "equivalence": evaluation.get("equivalence")
        or evaluation.get("equivalence_status"),
        "blocked_reason": evaluation.get("blocked_reason"),
        "symbol_accepted": evaluation.get("symbol_accepted"),
    }
    (fn_dir / "equivalence.json").write_text(
        json.dumps(equiv, indent=2) + "\n", encoding="utf-8"
    )
    return {
        "candidate": str(fn_dir / "candidate.cpp"),
        "evaluation": str(fn_dir / "evaluation.json"),
        "compile": str(fn_dir / "compile.txt"),
        "objdiff": str(fn_dir / "objdiff.txt"),
        "equivalence": str(fn_dir / "equivalence.json"),
    }


def _refresh_target_status(
    target: WebTarget,
    evaluation: dict[str, Any],
    best_source: str | None,
) -> WebTarget:
    status = _status_upper(evaluation) or target.status
    match = evaluation.get("match_percent")
    match_percent = float(match) if match is not None else target.match_percent
    return WebTarget(
        target_id=target.target_id,
        unit=target.unit,
        symbol=target.symbol,
        source_path=target.source_path,
        status=status,
        match_percent=match_percent,
        frontier_kind=target.frontier_kind,
        retail_text_size=target.retail_text_size,
        instruction_count=target.instruction_count,
        cost=target.cost,
        tier_sort=target.tier_sort,
        direct_call_count=target.direct_call_count,
        unresolved_call_count=target.unresolved_call_count,
        has_indirect_calls=target.has_indirect_calls,
        workflow="improve",
    )


def _build_debug_block(
    harness: "Harness",
    target_id: str,
    target: WebTarget,
    *,
    evaluation: dict[str, Any],
    best_source: str | None,
    outcome: str,
    previous_round: int,
) -> dict[str, Any]:
    metrics = evaluation.get("metrics") or {}
    fingerprint = metrics.get("mismatch_fingerprint") or ""
    repair_context = {
        "source": best_source or "",
        "candidate_source": best_source or "",
        "status": _status_upper(evaluation),
        "match_percent": evaluation.get("match_percent"),
        "compile_output": str(evaluation.get("detail") or ""),
        "root_diagnostic": None,
        "objdiff_feedback": _objdiff_feedback_text(evaluation),
        "equivalence_status": evaluation.get("equivalence")
        or evaluation.get("equivalence_status"),
        "equivalence_detail": evaluation.get("detail"),
        "binary_fingerprint": fingerprint,
        "evaluation": evaluation,
        "binary_feedback": metrics.get("binary_feedback"),
        "hypothesis": "",
        "rejected_fingerprints": [],
    }
    if _status_upper(evaluation) == "COMPILE_ERROR":
        diagnostics = normalize_compile_output(str(evaluation.get("detail") or ""))
        root = select_root_diagnostic(diagnostics)
        if root is not None:
            repair_context["root_diagnostic"] = {
                "category": root.category,
                "message": root.message,
                "file": root.file,
                "line": root.line,
            }

    try:
        prompt = harness.build_external_prompt(
            "improve",
            target_id,
            repair_context=repair_context,
        )
    except Exception as exc:
        prompt = (
            f"(failed to build improve prompt: {type(exc).__name__}: {exc})\n"
            f"Target: {target_id}\n"
            f"Best candidate follows in BEST_CANDIDATE block.\n"
        )

    return {
        "previous_round": previous_round,
        "best_candidate": best_source,
        "local_evaluation": {
            "status": _status_upper(evaluation),
            "match_percent": evaluation.get("match_percent"),
            "accepted": bool(evaluation.get("accepted") or evaluation.get("symbol_accepted")),
            "blocked_reason": evaluation.get("blocked_reason"),
            "equivalence": evaluation.get("equivalence")
            or evaluation.get("equivalence_status"),
            "binary_fingerprint": fingerprint,
        },
        "compiler_feedback": _compiler_feedback_text(
            str(evaluation.get("detail") or ""),
            _status_upper(evaluation),
        ),
        "objdiff_feedback": _objdiff_feedback_text(evaluation),
        "next_action": _next_action_text(evaluation, outcome),
        "prompt": prompt,
        "repair_context": repair_context,
    }


def _format_summary_text(summary: dict[str, Any]) -> str:
    lines = [
        "web ingest complete",
        f"batch={summary.get('batch_id')}",
        f"round={summary.get('round')}",
        f"submitted={summary.get('submitted')}",
        f"full_match={summary.get('full_match')}",
        f"equivalent_match={summary.get('equivalent_match')}",
        f"unresolved={summary.get('unresolved')}",
        f"compile_error={summary.get('compile_error')}",
        f"missing_response={summary.get('missing_response')}",
        f"promotion_failed={summary.get('promotion_failed')}",
        f"fence_policy={summary.get('fence_policy')}",
        f"next_round={summary.get('next_round')}",
        "",
    ]
    return "\n".join(lines)


def format_export_summary(result: WebBatchCreateResult) -> str:
    lines = [
        "web export complete" if not result.dry_run else "web export dry-run",
        f"batches={result.batch_count}",
        f"budget={result.budget}",
        f"selected_functions={result.selected_functions}",
        f"skipped_oversized={result.skipped_oversized}",
        f"not_packed={result.not_packed}",
        f"output={result.output_dir}",
    ]
    for batch in result.batches:
        lines.append(
            f"  {batch.batch_id}.txt functions={len(batch.targets)} cost={batch.used_budget}"
        )
    return "\n".join(lines)


def format_ingest_summary(result: WebBatchIngestResult) -> str:
    if result.idempotent:
        return "\n".join(
            [
                "web ingest idempotent",
                f"batch={result.batch_id}",
                f"round={result.round}",
                f"history={result.history_dir}",
                f"fence_policy={result.fence_policy}",
                "",
            ]
        )
    summary = result.summary or {}
    lines = [
        "web ingest complete",
        f"batch={result.batch_id}",
        f"round={result.round}",
        f"submitted={summary.get('submitted', len(result.results))}",
        f"full_match={summary.get('full_match', 0)}",
        f"equivalent_match={summary.get('equivalent_match', 0)}",
        f"unresolved={summary.get('unresolved', 0)}",
        f"compile_error={summary.get('compile_error', 0)}",
        f"missing_response={summary.get('missing_response', 0)}",
        f"promotion_failed={summary.get('promotion_failed', 0)}",
        f"active_file={result.active_file}",
        f"history={result.history_dir}",
        f"fence_policy={result.fence_policy}",
        "",
    ]
    return "\n".join(lines)

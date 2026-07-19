from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Callable, Dict, List, Optional, Tuple

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.elf_symbols import FunctionBytes
from tools.ppc_equivalence.ir import Opcode

from .structural import compare_structural


@dataclass
class InstructionDiff:
    offset: int
    retail_mnemonic: str
    candidate_mnemonic: str
    retail_raw: str
    candidate_raw: str
    likely_cause: str = ""


# ---------------------------------------------------------------------------
# §13.3 — Normalize objdiff feedback
# ---------------------------------------------------------------------------

DIFF_HINTS: dict[tuple[str, str], str] = {
    ("cmpwi", "cmplwi"): "signed vs unsigned compare",
    ("cmplwi", "cmpwi"): "unsigned vs signed compare",
    ("lwz", "lbz"): "32-bit vs 8-bit load width",
    ("lbz", "lwz"): "8-bit vs 32-bit load width",
    ("lwz", "lhz"): "32-bit vs 16-bit load width",
    ("lhz", "lwz"): "16-bit vs 32-bit load width",
    ("stw", "stb"): "32-bit vs 8-bit store width",
    ("stb", "stw"): "8-bit vs 32-bit store width",
    ("lha", "lhz"): "signed vs unsigned halfword load",
    ("lhz", "lha"): "unsigned vs signed halfword load",
}


def _mnemonic(insn) -> str:
    return insn.display_mnemonic or insn.opcode.value


def _format_insn_diff(insn) -> str:
    ops = ", ".join(
        f"r{v}" if v < 32 else f"0x{v:X}" for v in insn.operands[:3]
    ) if insn.operands else ""
    return f"{_mnemonic(insn)} {ops}".strip()


def normalize_objdiff_feedback(
    retail_fn: FunctionBytes,
    candidate_fn: FunctionBytes,
    max_window: int = 4,
) -> dict[str, Any]:
    retail_insns = decode_block(
        retail_fn.code, retail_fn.base,
        relocations=list(retail_fn.relocations),
    )
    candidate_insns = decode_block(
        candidate_fn.code, candidate_fn.base,
        relocations=list(candidate_fn.relocations),
    )

    diffs: list[InstructionDiff] = []
    max_len = max(len(retail_insns), len(candidate_insns))

    for i in range(max_len):
        if i >= len(retail_insns) or i >= len(candidate_insns):
            break
        r = retail_insns[i]
        c = candidate_insns[i]
        if r.raw == c.raw:
            continue
        hint = DIFF_HINTS.get((_mnemonic(r), _mnemonic(c)), "")
        diffs.append(InstructionDiff(
            offset=i * 4,
            retail_mnemonic=_mnemonic(r),
            candidate_mnemonic=_mnemonic(c),
            retail_raw=f"0x{r.raw:08X}",
            candidate_raw=f"0x{c.raw:08X}",
            likely_cause=hint,
        ))

    # Build instruction windows around first few differences
    windows: list[dict[str, Any]] = []
    for diff_idx, diff in enumerate(diffs[:3]):
        start = max(0, diff_idx * 2 - 1)
        end = min(max_len, start + max_window)
        retail_window = [
            _format_insn_diff(retail_insns[j])
            for j in range(start, end) if j < len(retail_insns)
        ]
        candidate_window = [
            _format_insn_diff(candidate_insns[j])
            for j in range(start, end) if j < len(candidate_insns)
        ]
        windows.append({
            "start_offset": start * 4,
            "retail": retail_window,
            "candidate": candidate_window,
        })

    # Relocation differences
    retail_relocs = {
        (r.offset, r.relocation_type, r.symbol)
        for r in retail_fn.relocations
    }
    candidate_relocs = {
        (r.offset, r.relocation_type, r.symbol)
        for r in candidate_fn.relocations
    }
    relocation_diffs = {
        "retail_only": sorted(retail_relocs - candidate_relocs, key=str)[:4],
        "candidate_only": sorted(candidate_relocs - retail_relocs, key=str)[:4],
    }

    # Stack frame & structural comparison
    struct = compare_structural(retail_fn, candidate_fn)

    # Detect stack frame differences
    # Stack frame size is typically the immediate in the first stwu
    retail_frame = (
        retail_insns[0].operands[2] if retail_insns and retail_insns[0].opcode == Opcode.STWU else 0
    ) if retail_insns else 0
    candidate_frame = (
        candidate_insns[0].operands[2] if candidate_insns and candidate_insns[0].opcode == Opcode.STWU else 0
    ) if candidate_insns else 0

    return {
        "first_difference_offset": diffs[0].offset if diffs else None,
        "num_differences": len(diffs),
        "instruction_windows": windows[:3],
        "differences": [
            {
                "offset": d.offset,
                "retail": d.retail_raw,
                "candidate": d.candidate_raw,
                "retail_mnemonic": d.retail_mnemonic,
                "candidate_mnemonic": d.candidate_mnemonic,
                "likely_cause": d.likely_cause,
            }
            for d in diffs[:20]
        ],
        "relocation_differences": {
            k: [
                f"+0x{off:X} type={typ} sym={sym}"
                for off, typ, sym in v
            ]
            for k, v in relocation_diffs.items()
            if v
        },
        "function_size": {
            "retail": retail_fn.size,
            "candidate": candidate_fn.size,
        },
        "stack_frame": {
            "retail": abs(retail_frame) if retail_frame else 0,
            "candidate": abs(candidate_frame) if candidate_frame else 0,
        },
        "structural_summary": {
            "total_score": struct.total_score,
            "calls": {"score": struct.calls.score, "details": struct.calls.details},
            "cfg": {"score": struct.cfg.score, "details": struct.cfg.details},
        },
    }


def format_objdiff_feedback_text(feedback: dict[str, Any] | None) -> str:
    if not feedback:
        return ""
    lines: list[str] = ["## Objdiff feedback"]
    diff_count = feedback.get("num_differences", 0)
    lines.append(f"First meaningful difference at offset +0x{feedback.get('first_difference_offset', 0):X}" if feedback.get("first_difference_offset") is not None else "No instruction differences detected.")

    for i, win in enumerate(feedback.get("instruction_windows", [])):
        lines.append(f"\n### Window {i+1} at +0x{win['start_offset']:X}")
        lines.append("Retail:")
        for line in win["retail"]:
            lines.append(f"  {line}")
        lines.append("Candidate:")
        for line in win["candidate"]:
            lines.append(f"  {line}")

    for d in feedback.get("differences", [])[:8]:
        cause_str = f" — {d['likely_cause']}" if d['likely_cause'] else ""
        lines.append(
            f"  +0x{d['offset']:X}: retail {d['retail_mnemonic']} ({d['retail']}) "
            f"candidate {d['candidate_mnemonic']} ({d['candidate']}){cause_str}"
        )

    reloc = feedback.get("relocation_differences", {})
    for key, items in reloc.items():
        for item in items:
            lines.append(f"  relocation {key}: {item}")

    fs = feedback.get("function_size", {})
    if fs.get("retail") != fs.get("candidate"):
        lines.append(f"  function size: retail={fs['retail']}, candidate={fs['candidate']}")

    sf = feedback.get("stack_frame", {})
    if sf.get("retail") != sf.get("candidate"):
        lines.append(f"  stack frame: retail={sf['retail']}, candidate={sf['candidate']}")

    struct = feedback.get("structural_summary", {})
    lines.append(f"  structural score: {struct.get('total_score', 0):.4f}")

    return "\n".join(lines)


# ---------------------------------------------------------------------------
# §13.5 — Non-regression requirement
# ---------------------------------------------------------------------------

NON_REGRESSION_TOLERANCE = 0.02


def passes_non_regression(parent_score: float, candidate_score: float) -> bool:
    return candidate_score + NON_REGRESSION_TOLERANCE >= parent_score


# ---------------------------------------------------------------------------
# §13.4 — Improvement response schema
# ---------------------------------------------------------------------------

@dataclass
class MatchImproveResponse:
    stage: str = "match_improve"
    source: str = ""
    bounded_change: str = ""
    evidence: List[str] = field(default_factory=list)
    expected_instruction_effect: List[str] = field(default_factory=list)

    @classmethod
    def from_json(cls, text: str) -> Optional[MatchImproveResponse]:
        import json, re
        cleaned = text.strip()
        fenced = re.search(r"```(?:json)?\s*(\{.*\})\s*```", cleaned, re.DOTALL)
        if fenced:
            cleaned = fenced.group(1)
        else:
            start, end = cleaned.find("{"), cleaned.rfind("}")
            if start >= 0 and end > start:
                cleaned = cleaned[start:end + 1]
        try:
            data = json.loads(cleaned)
        except (json.JSONDecodeError, ValueError):
            return None
        src = data.get("source", "")
        if not isinstance(src, str) or not src.strip():
            return None
        return cls(
            stage=data.get("stage", "match_improve"),
            source=src,
            bounded_change=str(data.get("bounded_change", "")),
            evidence=list(data.get("evidence", [])),
            expected_instruction_effect=list(data.get("expected_instruction_effect", [])),
        )


# ---------------------------------------------------------------------------
# Improvement loop
# ---------------------------------------------------------------------------

@dataclass
class ImproveAttempt:
    index: int
    source: str
    feedback: dict[str, Any]
    bounded_change: str = ""
    structural_score: float = 0.0
    match_percent: float = 0.0
    non_regression_pass: bool = False
    error: str = ""


@dataclass
class ImproveBranch:
    target_id: str
    initial_source: str
    initial_score: float
    attempts: List[ImproveAttempt] = field(default_factory=list)
    blocked: bool = False
    blocked_reason: str = ""
    full_match: bool = False

    def block(self, reason: str) -> None:
        self.blocked = True
        self.blocked_reason = reason


EvaluateFn = Callable[[str], Tuple[str, Any]]
FeedbackFn = Callable[[str, str], dict[str, Any]]
PromptBuildFn = Callable[[ImproveBranch, int, dict[str, Any]], str]
ModelCallFn = Callable[[str, int], str]


def run_match_improve(
    branch: ImproveBranch,
    budget: int,
    threshold: float,
    evaluate_fn: EvaluateFn,
    feedback_fn: FeedbackFn,
    prompt_fn: PromptBuildFn,
    model_fn: ModelCallFn,
) -> ImproveBranch:
    if branch.full_match:
        return branch

    for index in range(1, budget + 1):
        latest_source = branch.attempts[-1].source if branch.attempts else branch.initial_source
        latest_score = branch.attempts[-1].structural_score if branch.attempts else branch.initial_score

        if latest_score >= threshold:
            break

        feedback = feedback_fn(branch.target_id, latest_source)
        prompt = prompt_fn(branch, index, feedback)
        response = model_fn(prompt, index)
        parsed = MatchImproveResponse.from_json(response)

        if parsed is None:
            branch.block(f"invalid response at index {index}")
            return branch

        status, evaluation = evaluate_fn(parsed.source)
        if status == "COMPILE_ERROR":
            branch.block(f"compile error after improve at index {index}")
            return branch

        new_score = evaluation.get("structural_score", 0.0) if isinstance(evaluation, dict) else 0.0
        match_pct = evaluation.get("match_percent", 0.0) if isinstance(evaluation, dict) else 0.0

        non_reg = passes_non_regression(latest_score, new_score)

        attempt = ImproveAttempt(
            index=index,
            source=parsed.source,
            feedback=feedback,
            bounded_change=parsed.bounded_change,
            structural_score=new_score,
            match_percent=match_pct,
            non_regression_pass=non_reg,
        )
        branch.attempts.append(attempt)

        if not non_reg:
            branch.block(
                f"non-regression failure: score dropped from {latest_score:.4f} to {new_score:.4f}"
            )
            return branch

        if match_pct >= 100.0:
            branch.full_match = True
            return branch

    return branch

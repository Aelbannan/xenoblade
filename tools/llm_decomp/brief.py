"""Build the markdown initial prompt (the 'brief') for an LLM decomp session.

Design reference: docs/llm_decomp_design.md §11.
"""
from __future__ import annotations

import math
from dataclasses import dataclass

from tools.llm_decomp.contracts import Baseline


@dataclass
class TargetBrief:
    """One target's payload for a batch brief."""

    target_id: str
    symbol: str
    demangled: str
    signature: str
    retail_asm: str


def _base_rules() -> list[str]:
    """Rules shared by single-target and batch briefs."""
    return [
        "**High-level C/C++ only.** No `asm` blocks, no inline assembly, no register/stack manipulation, no codegen macros (`DECOMP_*`, `DECOMP_FORCE*`, etc.). Residual opcode gaps are resolved via ppc_equivalence, not intrinsics.",
        "**Proper types.** Use structs and classes. No pointer arithmetic, no `void*` as a struct substitute.",
        "**Naming discipline.** Use human-readable names only when you understand the semantics. Unknown struct fields keep `field_0xNN` offset names — a wrong guess is worse than no name.",
        "**No `extern \"C\"`** except for `lbl_*` / `lbl_*` reloc names in the approved pattern.",
        "**No new `#pragma` or `#if 0`.** Do not add preprocessor scaffolding or dead-code blocks.",
        "**Edits via the patch tool only.** All changes must remain within the writable scope listed above.",
        "**The signature is locked.** Do not change the function signature (see § Target above).",
        "**Follow the project coding style.** See `docs/coding_style_guidelines.md`.",
        "**Acceptance is decided by the harness** — `FULL_MATCH` or proven equivalence + no regressions + size within budget. Calling `submit` is a verification checkpoint, not a finish line.",
    ]


def _truncate_asm(asm_text: str, max_asm_chars: int) -> str:
    """Truncate assembly listing in the middle if it exceeds *max_asm_chars*.

    Keeps roughly the first 60 % and last 30 % of lines with an elision
    marker between them.  If the listing already fits, return it unchanged.
    """
    lines = asm_text.splitlines()
    if not lines:
        return ""

    if len(asm_text) <= max_asm_chars:
        return asm_text

    # Iterative narrowing: keep cutting until it fits.
    total = len(lines)
    first_ratio, last_ratio = 0.6, 0.3

    while True:
        first_n = max(1, math.ceil(total * first_ratio))
        last_n = max(1, math.ceil(total * last_ratio))

        if first_n + last_n >= total:
            return asm_text

        elided = total - first_n - last_n
        truncated = lines[:first_n] + [f"# ... {elided} lines elided ..."] + lines[-last_n:]
        result = "\n".join(truncated)

        if len(result) <= max_asm_chars:
            return result

        # Reduce ratios and try again.
        first_ratio *= 0.85
        last_ratio *= 0.85


def build_brief(
    *,
    target_id: str,
    symbol: str,
    demangled: str,
    signature: str,
    unit: str,
    retail_asm: str,
    writable: list[str],
    baseline: Baseline | None = None,
    carryover: str | None = None,
    session_type: str = "match",
    max_chars: int = 60_000,
    source_content: str | None = None,
    header_content: str | None = None,
) -> str:
    """Build the markdown brief for the decompilation session.

    Parameters
    ----------
    target_id, symbol, demangled, signature, unit
        Target-identification fields from ``targets.json`` / symrecover.
    retail_asm
        Full retail assembly listing for the target symbol.
    writable
        Repo-relative paths the session is allowed to modify.
    baseline
        Compilation baseline for the target's translation unit, or *None*.
    carryover
        Verbatim carry-over text from a prior chained session, or *None*.
    session_type
        One of the registered session-type names.
    max_chars
        Hard character limit for the entire brief.  Only the ASM section
        is truncated to stay within this budget.

    Returns
    -------
    Markdown string (never JSON).
    """
    # -- §11 – fixed sections -------------------------------------------------
    heading = f"# Decompilation session: {session_type}\n\n"
    heading += f"Target: **{target_id}** — `{demangled}`\n\n"

    target_section = "## Target\n\n"
    target_section += f"- target-id: `{target_id}`\n"
    target_section += f"- mangled symbol: `{symbol}`\n"
    target_section += f"- demangled: `{demangled}`\n"
    target_section += f"- unit: `{unit}`\n"
    target_section += "- locked signature:\n"
    target_section += f"```c\n{signature}\n```\n"
    target_section += "\n_The signature is locked and must not change during this session._\n\n"

    # -- ASM section ----------------------------------------------------------
    asm_header = "## Retail ASM\n\n"
    asm_header += "Read-only reference. Decompile the **semantics**, not the registers.\n\n"
    asm_header += "```asm\n"

    asm_footer = "\n```\n\n"
    raw_asm_content = f"{asm_header}{retail_asm}{asm_footer}"

    # -- Writable scope -------------------------------------------------------
    writable_section = "## Writable scope\n\n"
    for w in writable:
        writable_section += f"- `{w}`\n"
    writable_section += "\nEverything else is read-only. Read freely with `read_file` / `grep`.\n\n"

    # -- State ----------------------------------------------------------------
    state_section = "## State\n\n"
    if baseline is not None:
        sym_info = baseline.symbols.get(symbol)
        if sym_info is not None:
            state_section += f"- current mismatches for `{symbol}`: {sym_info.mismatch_count}\n"
        else:
            state_section += f"- current mismatches for `{symbol}`: not yet compiled\n"

        total_syms = len(baseline.symbols)
        matched = sum(1 for s in baseline.symbols.values() if s.mismatch_count == 0)
        state_section += f"- total symbols in TU: {total_syms}\n"
        state_section += f"- symbols with 0 mismatches: {matched}\n"

        size_info = f"- .text size: {baseline.text_size}"
        if baseline.text_budget is not None:
            size_info += f" / budget: {baseline.text_budget}"
        state_section += size_info + "\n\n"
    else:
        state_section += "baseline pending\n\n"

    # -- Source file content --------------------------------------------------
    source_section = ""
    if source_content:
        source_section = "## Current source file\n\n```cpp\n"
        source_section += source_content.rstrip("\n")
        source_section += "\n```\n\n"

    # -- Header content -------------------------------------------------------
    header_section = ""
    if header_content:
        header_section = "## TU header\n\n```cpp\n"
        header_section += header_content.rstrip("\n")
        header_section += "\n```\n\n"

    # -- Rules ----------------------------------------------------------------
    rules_section = "## Rules\n\n"
    rules = _base_rules()
    for i, rule in enumerate(rules, 1):
        rules_section += f"{i}. {rule}\n"
    rules_section += "\n"

    # -- Carryover ------------------------------------------------------------
    carryover_section = ""
    if carryover is not None:
        carryover_section = "## Carryover\n\n"
        carryover_section += carryover.strip() + "\n\n"

    # -- Closing --------------------------------------------------------------
    closing = "Begin by reviewing the retail ASM and the current state of the target function."

    # -- Assemble & enforce size budget ---------------------------------------
    # Build prefix without the raw ASM lines to compute headroom.
    prefix = heading + target_section
    suffix = writable_section + state_section + source_section + header_section + rules_section + carryover_section + closing

    # Estimate overhead of unknown-length section markers.
    asm_overhead = len(asm_header) + len(asm_footer)

    headroom = max_chars - len(prefix) - len(suffix) - asm_overhead

    if headroom <= 0:
        # Extreme edge-case: even the overhead doesn't fit.  Return the brief
        # without the ASM body — the model can request it.
        asm_body = "*[retail ASM omitted — too large for the prompt budget]*\n"
    else:
        asm_body = _truncate_asm(retail_asm, headroom)

    return prefix + asm_header + asm_body + asm_footer + suffix

def build_batch_brief(
    *,
    targets: list[TargetBrief],
    unit: str,
    writable: list[str],
    baseline: Baseline | None = None,
    carryover: str | None = None,
    session_type: str = "batch-match",
    max_chars: int = 60_000,
    source_content: str | None = None,
    header_content: str | None = None,
) -> str:
    """Build the markdown brief for a batch (multi-target) match session.

    Same shape as ``build_brief`` but covers *N* targets of one TU: the
    source file and header appear once, followed by one block per target.
    Each target's retail ASM is truncated independently to an equal share
    of the character budget.
    """
    n = len(targets)

    heading = f"# Decompilation session: {session_type}\n\n"
    heading += (f"This is a **batch** session: {n} targets on unit "
                f"`{unit}`. Work them in order; submit each one "
                "separately.\n\n")

    overview = "## Targets\n\n"
    for i, t in enumerate(targets, 1):
        overview += f"{i}. `{t.target_id}` — {t.demangled} (`{t.symbol}`)\n"
    overview += "\n"

    # -- Writable scope ---------------------------------------------------
    writable_section = "## Writable scope\n\n"
    for w in writable:
        writable_section += f"- `{w}`\n"
    writable_section += ("\nEverything else is read-only. Read freely "
                         "with `read_file` / `grep`.\n\n")

    # -- State --------------------------------------------------------------
    state_section = "## State\n\n"
    if baseline is not None:
        for t in targets:
            sym_info = baseline.symbols.get(t.symbol)
            if sym_info is not None:
                state_section += (f"- current mismatches for `{t.symbol}`: "
                                  f"{sym_info.mismatch_count}\n")
            else:
                state_section += (f"- current mismatches for `{t.symbol}`: "
                                  "not yet compiled\n")
        total_syms = len(baseline.symbols)
        matched = sum(1 for s in baseline.symbols.values()
                      if s.mismatch_count == 0)
        state_section += f"- total symbols in TU: {total_syms}\n"
        state_section += f"- symbols with 0 mismatches: {matched}\n"
        size_info = f"- .text size: {baseline.text_size}"
        if baseline.text_budget is not None:
            size_info += f" / budget: {baseline.text_budget}"
        state_section += size_info + "\n\n"
    else:
        state_section += "baseline pending\n\n"

    # -- Source / header ----------------------------------------------------
    source_section = ""
    if source_content:
        source_section = "## Current source file\n\n```cpp\n"
        source_section += source_content.rstrip("\n")
        source_section += "\n```\n\n"

    header_section = ""
    if header_content:
        header_section = "## TU header\n\n```cpp\n"
        header_section += header_content.rstrip("\n")
        header_section += "\n```\n\n"

    # -- Rules ----------------------------------------------------------------
    rules_section = "## Rules\n\n"
    rules = _base_rules() + [
        "**Submit each target separately** with `submit(target_id)` using "
        "the exact target-id strings above.",
        "**Accepted targets are frozen** — a patch that changes an "
        "accepted target's compiled bytes is reverted.",
    ]
    for i, rule in enumerate(rules, 1):
        rules_section += f"{i}. {rule}\n"
    rules_section += "\n"

    # -- Carryover --------------------------------------------------------------
    carryover_section = ""
    if carryover is not None:
        carryover_section = "## Carryover\n\n"
        carryover_section += carryover.strip() + "\n\n"

    closing = ("Work the targets in order: patch → build → diff for one "
               "target at a time, and call `submit(target_id)` when it "
               "matches.")

    # -- Per-target ASM budget --------------------------------------------------
    asm_header = ("### Retail ASM\n\nRead-only reference. Decompile the "
                  "**semantics**, not the registers.\n\n```asm\n")
    asm_footer = "\n```\n\n"

    def _target_block(i: int, t: TargetBrief, asm_body: str) -> str:
        block = f"## Target {i}: {t.target_id}\n\n"
        block += f"- mangled symbol: `{t.symbol}`\n"
        block += f"- demangled: `{t.demangled}`\n"
        block += "- locked signature:\n"
        block += f"```c\n{t.signature}\n```\n"
        block += ("\n_The signature is locked and must not change during "
                  "this session._\n\n")
        block += asm_header + asm_body + asm_footer
        return block

    fixed = (heading + overview + writable_section + state_section
             + source_section + header_section + rules_section
             + carryover_section + closing)
    overhead = sum(
        len(_target_block(i, t, "")) for i, t in enumerate(targets, 1))
    headroom = max_chars - len(fixed) - overhead
    share = headroom // n if n else 0

    if headroom <= 0 or n == 0:
        bodies = ["*[retail ASM omitted — too large for the prompt "
                  "budget]*\n"] * n
    else:
        bodies = [_truncate_asm(t.retail_asm, share) for t in targets]

    target_blocks = "".join(
        _target_block(i, t, body)
        for i, (t, body) in enumerate(zip(targets, bodies), 1))

    return (heading + overview + writable_section + state_section
            + source_section + header_section + target_blocks
            + rules_section + carryover_section + closing)

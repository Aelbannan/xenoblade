"""Shared C++ source-region locator for the LLM harness and Decomp Atlas.

Resolves function bodies via stable harness markers or demangled-name brace
matching. Both callers must import this module so they never disagree about
constructors, destructors, qualified methods, or nested braces.
"""
from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Optional, Protocol, Sequence, Union


@dataclass(frozen=True)
class SourceRegion:
    start: int
    end: int
    content_start: int
    content_end: int
    marked: bool


@dataclass(frozen=True)
class TuSlot:
    slot_id: str
    kind: str
    start: int
    end: int
    content_start: int
    content_end: int


class _TargetLike(Protocol):
    id: str
    function: str


def begin_marker(target_id: str) -> str:
    return f"// LLM-HARNESS-BEGIN: {target_id}"


def end_marker(target_id: str) -> str:
    return f"// LLM-HARNESS-END: {target_id}"


def tu_begin_marker(slot_id: str) -> str:
    return f"// LLM-HARNESS-TU-BEGIN: {slot_id}"


def tu_end_marker(slot_id: str) -> str:
    return f"// LLM-HARNESS-TU-END: {slot_id}"


# Backwards-compatible private aliases used by existing harness tests/imports.
_begin_marker = begin_marker
_end_marker = end_marker
_tu_begin_marker = tu_begin_marker
_tu_end_marker = tu_end_marker


def find_function_region(source: str, target: _TargetLike) -> SourceRegion:
    begin = begin_marker(target.id)
    end = end_marker(target.id)
    begin_pos = source.find(begin)
    end_pos = source.find(end)
    if begin_pos >= 0 or end_pos >= 0:
        if begin_pos < 0 or end_pos < 0 or end_pos <= begin_pos:
            raise ValueError(f"Malformed harness markers for {target.id}")
        content_start = source.find("\n", begin_pos)
        if content_start < 0:
            raise ValueError(f"Begin marker for {target.id} has no following source")
        content_start += 1
        return SourceRegion(
            start=begin_pos,
            end=end_pos + len(end),
            content_start=content_start,
            content_end=end_pos,
            marked=True,
        )

    function_identity = target.function.split("(", 1)[0].strip()
    parts = function_identity.split("::")
    qualified = "::".join(parts[-2:]) if len(parts) >= 2 else function_identity
    pattern = re.compile(re.escape(qualified) + r"\s*\(")
    for match in pattern.finditer(source):
        brace = signature_open_brace(source, match.end() - 1)
        if brace is None:
            continue
        close = matching_brace(source, brace)
        line_start = source.rfind("\n", 0, match.start()) + 1
        return SourceRegion(
            start=line_start,
            end=close + 1,
            content_start=line_start,
            content_end=close + 1,
            marked=False,
        )
    raise ValueError(
        f"Could not locate {target.function} in source; add stable harness markers first"
    )


# Single-line file-scope ``extern …;`` declarations only (never definitions).
# Reject any match that contains ``{`` so one-liner function bodies are not eaten.
_EXTERN_LINE = re.compile(r"(?m)^[ \t]*extern\b[^\n{;]*;")
_CPP_SUFFIXES = {".cpp", ".cc", ".cxx", ".cp"}

RegionEntry = Union[
    tuple[str, SourceRegion],
    tuple[str, SourceRegion, str, str],
]


def is_mwcc_mangled_symbol(symbol: str) -> bool:
    """True when *symbol* uses MWCC C++ mangling (``foo__5BarFv``, ``__ct__…``)."""
    return "__" in (symbol or "")


def source_is_cpp(source_path: str | Path | None) -> bool:
    if source_path is None:
        return False
    return Path(str(source_path)).suffix.lower() in _CPP_SUFFIXES


def should_keep_extern_c_on_definition(
    symbol: str, source_path: str | Path | None
) -> bool:
    """``extern "C"`` on a definition is only useful for flat symbols in C++ TUs.

    Never used to *add* linkage — only to decide whether an existing prefix is
    redundant and should be removed. When symbol/path are unknown, keep the
    prefix (do not strip).
    """
    if not symbol or source_path is None:
        return True
    if not source_is_cpp(source_path):
        return False
    return not is_mwcc_mangled_symbol(symbol)


def _target_callable_names(function: str, symbol: str = "") -> set[str]:
    """Names that identify the target callable (qualified + unqualified)."""
    names: set[str] = set()
    if function:
        base = function.split("(", 1)[0].strip()
        if base:
            names.add(base)
            names.add(base.split("::")[-1])
    if symbol and not is_mwcc_mangled_symbol(symbol):
        names.add(symbol)
    return {name for name in names if name}


def _names_defined_in_candidate(candidate: str) -> set[str]:
    """Unqualified (+ qualified) names of function *definitions* in *candidate*."""
    names: set[str] = set()
    for match in re.finditer(
        r"([A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*\(([^;{}]*)\)\s*(?:const\s*)?\{",
        candidate,
    ):
        full = match.group(1)
        names.add(full)
        names.add(full.split("::")[-1])
    return names


def _extern_function_proto_symbol(statement: str) -> Optional[str]:
    """Return the function name for ``extern [\"C\"] Ret name(...);``, or None."""
    text = statement.strip()
    if "{" in text or re.search(r"\(\s*\*\s*\w+\s*\)", text):
        return None
    # Params must not span into a body; require ``)`` then optional cv then ``;``.
    match = re.match(
        r'extern(?:\s+"C")?\s+.+?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:const\s*)?;\s*$',
        text,
    )
    return match.group(1) if match else None


def _extern_object_symbol(statement: str) -> Optional[str]:
    """Return the object symbol declared by a single-line ``extern`` stmt, or None.

    Skips function prototypes (handled separately) and ``extern "C" Ret name(``
    definition/decl forms. Handles plain pointers/arrays and ``T (*name)(...)``.
    """
    text = statement.strip()
    if _extern_function_proto_symbol(text) is not None:
        return None
    if re.search(r'\bextern\s+"C"\s+[\w:\s\*\&]+\s+\w+\s*\(', text):
        return None
    pointer = re.search(r"\(\s*\*\s*([A-Za-z_]\w*)\s*\)", text)
    if pointer:
        return pointer.group(1)
    plain = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;\s*$", text)
    if plain:
        return plain.group(1)
    return None


def _symbol_declared_outside(
    tu_source: str,
    region: SourceRegion,
    name: str,
    *,
    earlier_only: bool = False,
) -> bool:
    """True when *name* already has a decl/def in the TU outside *region*.

    When *earlier_only* is set, only declarations that appear before the region
    count. That keeps the first writer of a shared global and is the right
    policy for cleaning already-accepted slots in file order.
    """
    if earlier_only:
        outside = tu_source[: region.content_start]
    else:
        outside = tu_source[: region.content_start] + "\n" + tu_source[region.content_end :]
    escaped = re.escape(name)
    patterns = (
        rf"(?m)^[ \t]*extern\b[^\n;]*\b{escaped}\b",
        rf"\(\s*\*\s*{escaped}\s*\)",
        rf"(?m)^[ \t]*(?!extern\b)[\w:\s\*\&]+?\b{escaped}\s*(?:\[[^\]]*\]\s*)?[;=]",
        # Function definition or prototype (possibly under ``extern "C"``).
        rf"(?m)^[ \t]*(?:extern\s+\"C\"\s+)?[\w:\s\*\&]+?\b{escaped}\s*\([^;{{}}]*\)\s*(?:const\s*)?\{{",
        rf"(?m)^[ \t]*extern(?:\s+\"C\")?\s+[^\n(]+?\b{escaped}\s*\([^;]*\)\s*;",
        rf"(?m)^[ \t]*(?!extern\b)[\w:\s\*\&]+?\b{escaped}\s*\([^;{{}}]*\)\s*(?:const\s*)?[;{{]",
    )
    return any(re.search(pattern, outside) for pattern in patterns)


def _strip_unneeded_extern_c_prefix(candidate: str) -> tuple[str, list[str]]:
    """Remove ``extern "C"`` immediately before a function definition.

    Only the prefix form is handled (``extern "C" Ret name(...) {{``). Block
    form ``extern "C" {{ … }}`` is left alone.
    """
    removed: list[str] = []

    def replacer(match: re.Match[str]) -> str:
        removed.append('extern "C"')
        return match.group(1)

    cleaned = re.sub(
        r'(?m)^([ \t]*)extern\s+"C"\s+'
        r"(?=[\w:\s\*\&]+?\b[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*\s*\([^;]*\)\s*(?:const\s*)?\{)",
        replacer,
        candidate,
    )
    return cleaned, removed


def strip_redundant_extern_decls(
    tu_source: str,
    region: SourceRegion,
    candidate: str,
    *,
    earlier_only: bool = False,
    target_function: str = "",
    target_symbol: str = "",
    source_path: str | Path | None = None,
) -> str:
    """Sanitize LLM function-slot source: drop redundant ``extern`` noise only.

    Never invents or inserts linkage. Removes:

    1. Object ``extern T name;`` when *name* is already declared outside the slot
       (or earlier, when *earlier_only*).
    2. Function prototypes ``extern Ret name(...);`` when *name* is defined in the
       same candidate (same-slot) or already declared/defined outside (cross-slot).
    3. ``extern "C"`` on the target definition when the TU/symbol does not need C
       linkage (``.c`` file, or MWCC-mangled C++ symbol).
    """
    defined = _names_defined_in_candidate(candidate)
    target_names = _target_callable_names(target_function, target_symbol)

    pieces: list[str] = []
    cursor = 0
    for match in _EXTERN_LINE.finditer(candidate):
        statement = match.group(0)
        object_sym = _extern_object_symbol(statement)
        proto_sym = _extern_function_proto_symbol(statement)
        drop = False
        if object_sym and _symbol_declared_outside(
            tu_source, region, object_sym, earlier_only=earlier_only
        ):
            drop = True
        elif proto_sym:
            if proto_sym in defined or proto_sym in target_names:
                drop = True
            elif _symbol_declared_outside(
                tu_source, region, proto_sym, earlier_only=earlier_only
            ):
                drop = True
        if drop:
            pieces.append(candidate[cursor : match.start()])
            cursor = match.end()
            if cursor < len(candidate) and candidate[cursor] == "\n":
                cursor += 1
            continue
    pieces.append(candidate[cursor:])
    cleaned = "".join(pieces)
    removed_any = cleaned != candidate

    if not should_keep_extern_c_on_definition(target_symbol, source_path):
        cleaned, removed_c = _strip_unneeded_extern_c_prefix(cleaned)
        removed_any = removed_any or bool(removed_c)

    if removed_any:
        cleaned = re.sub(r"^\n+", "", cleaned)
        cleaned = re.sub(r"\n{3,}", "\n\n", cleaned)
    return cleaned


def _removed_extern_lines(before: str, after: str) -> list[str]:
    before_lines = {match.group(0).strip() for match in _EXTERN_LINE.finditer(before)}
    after_lines = {match.group(0).strip() for match in _EXTERN_LINE.finditer(after)}
    removed = sorted(before_lines - after_lines)
    # Prefix form on definitions is not a full ``extern …;`` line; count drops.
    if before.count('extern "C"') > after.count('extern "C"'):
        removed.append('extern "C"')
    return removed


def apply_strip_redundant_externs_to_regions(
    source: str,
    regions: Sequence[RegionEntry],
    *,
    earlier_only: bool = True,
    source_path: str | Path | None = None,
) -> tuple[str, list[dict]]:
    """Strip redundant externs from multiple function regions in one TU.

    Each region entry is ``(target_id, region)`` or
    ``(target_id, region, function, symbol)``. Returns ``(updated_source,
    change_rows)``. Change rows use actions ``unchanged`` or ``stripped``.
    Offsets are taken from the original *source*; replacements are applied from
    last region to first so earlier offsets stay valid.
    """
    rows: list[dict] = []
    planned: list[tuple[str, SourceRegion, str, list[str]]] = []
    for entry in regions:
        if len(entry) == 2:
            target_id, region = entry  # type: ignore[misc]
            target_function = ""
            target_symbol = ""
        else:
            target_id, region, target_function, target_symbol = entry  # type: ignore[misc]
        body = source[region.content_start : region.content_end]
        cleaned = strip_redundant_extern_decls(
            source,
            region,
            body,
            earlier_only=earlier_only,
            target_function=target_function,
            target_symbol=target_symbol,
            source_path=source_path,
        )
        removed = _removed_extern_lines(body, cleaned)
        # Ignore whitespace-only churn from blank-line collapse.
        if re.sub(r"\s+", "", cleaned) == re.sub(r"\s+", "", body):
            rows.append(
                {
                    "target_id": target_id,
                    "action": "unchanged",
                    "removed_externs": [],
                }
            )
            continue
        if cleaned.strip() == body.strip():
            rows.append(
                {
                    "target_id": target_id,
                    "action": "unchanged",
                    "removed_externs": [],
                }
            )
            continue
        planned.append((target_id, region, cleaned, removed))
        rows.append(
            {
                "target_id": target_id,
                "action": "stripped",
                "removed_externs": removed,
            }
        )

    updated = source
    for _target_id, region, cleaned, _removed in sorted(
        planned, key=lambda item: item[1].content_start, reverse=True
    ):
        value = cleaned.strip()
        if region.marked:
            value += "\n"
        updated = updated[: region.content_start] + value + updated[region.content_end :]
    return updated, rows


def rewrite_harness_stub_names(
    candidate: str,
    *,
    target_id: str = "",
    target_symbol: str = "",
) -> str:
    """Rewrite placeholder / truncated stub names to the real linker symbol.

    Covers:
    - ``harness_stub_us_…`` placeholders
    - truncated ``UnkVirtualFuncN`` / mangled names missing the ``__Class…`` suffix
    - single-underscore typos (``SetBiquadFilter_Q44…`` vs ``SetBiquadFilter__Q44…``)
    """
    if not candidate or not target_symbol:
        return candidate
    updated = candidate
    if target_id:
        stub = "harness_stub_" + target_id.replace("-", "_")
        if stub in updated:
            updated = re.sub(rf"\b{re.escape(stub)}\b", target_symbol, updated)
    if target_symbol in updated:
        return updated
    # Single-underscore typo of an otherwise exact mangled name.
    typo = target_symbol.replace("__", "_", 1)
    if typo != target_symbol and re.search(rf"\b{re.escape(typo)}\b", updated):
        updated = re.sub(rf"\b{re.escape(typo)}\b", target_symbol, updated)
        return updated
    # Truncated leaf before the MWCC ``__`` class/namespace suffix.
    if "__" in target_symbol:
        leaf = target_symbol.split("__", 1)[0]
        if leaf and re.search(rf"\b{re.escape(leaf)}\b", updated):
            # Only rewrite definition-like occurrences (name followed by '(').
            updated = re.sub(
                rf"\b{re.escape(leaf)}\s*\(",
                target_symbol + "(",
                updated,
                count=1,
            )
    return updated


def replace_function_source(
    source: str,
    region: SourceRegion,
    replacement: str,
    *,
    target_function: str = "",
    target_symbol: str = "",
    source_path: str | Path | None = None,
    target_id: str = "",
    local_mode: bool = False,
) -> str:
    from .candidate_sanitize import sanitize_candidate_source

    scaffold = source[region.content_start : region.content_end]
    replacement = sanitize_candidate_source(
        replacement,
        target_function=target_function,
        target_symbol=target_symbol,
        source_path=source_path,
        scaffold_from=scaffold,
        local_mode=local_mode,
    )
    replacement = rewrite_harness_stub_names(
        replacement, target_id=target_id, target_symbol=target_symbol
    )
    value = strip_redundant_extern_decls(
        source,
        region,
        replacement,
        target_function=target_function,
        target_symbol=target_symbol,
        source_path=source_path,
    ).strip()
    if region.marked:
        value += "\n"
    return source[: region.content_start] + value + source[region.content_end :]


def find_tu_slots(source: str) -> Dict[str, TuSlot]:
    begin_pattern = re.compile(
        r"(?m)^[ \t]*// LLM-HARNESS-TU-BEGIN: ([A-Za-z0-9][A-Za-z0-9._-]*)[ \t]*$"
    )
    end_pattern = re.compile(
        r"(?m)^[ \t]*// LLM-HARNESS-TU-END: ([A-Za-z0-9][A-Za-z0-9._-]*)[ \t]*$"
    )
    begins = list(begin_pattern.finditer(source))
    ends = list(end_pattern.finditer(source))
    slots: Dict[str, TuSlot] = {}
    used_ends: set[int] = set()
    for begin in begins:
        slot_id = begin.group(1)
        if slot_id in slots:
            raise ValueError(f"Duplicate TU begin marker for {slot_id!r}")
        matching = [
            (index, end)
            for index, end in enumerate(ends)
            if index not in used_ends and end.group(1) == slot_id and end.start() > begin.end()
        ]
        if len(matching) != 1:
            raise ValueError(f"TU slot {slot_id!r} must have exactly one following end marker")
        end_index, end = matching[0]
        nested = next(
            (other.group(1) for other in begins if begin.end() < other.start() < end.start()),
            None,
        )
        if nested is not None:
            raise ValueError(f"TU slots may not nest ({slot_id!r} contains {nested!r})")
        content_start = source.find("\n", begin.end())
        if content_start < 0:
            raise ValueError(f"TU begin marker for {slot_id!r} has no following source")
        content_start += 1
        slots[slot_id] = TuSlot(
            slot_id=slot_id,
            kind="tu",
            start=begin.start(),
            end=end.end(),
            content_start=content_start,
            content_end=end.start(),
        )
        used_ends.add(end_index)
    if len(used_ends) != len(ends):
        stray = next(end.group(1) for index, end in enumerate(ends) if index not in used_ends)
        raise ValueError(f"Stray TU end marker for {stray!r}")
    return slots


def signature_open_brace(source: str, opening_paren: int) -> int | None:
    depth = 0
    state = "code"
    index = opening_paren
    while index < len(source):
        char = source[index]
        nxt = source[index + 1] if index + 1 < len(source) else ""
        if state == "line":
            if char == "\n":
                state = "code"
        elif state == "block":
            if char == "*" and nxt == "/":
                state = "code"
                index += 1
        elif state in {"string", "char"}:
            if char == "\\":
                index += 1
            elif (state == "string" and char == '"') or (state == "char" and char == "'"):
                state = "code"
        elif char == "/" and nxt == "/":
            state = "line"
            index += 1
        elif char == "/" and nxt == "*":
            state = "block"
            index += 1
        elif char == '"':
            state = "string"
        elif char == "'":
            state = "char"
        elif char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
        elif depth == 0 and char == "{":
            return index
        elif depth == 0 and char == ";":
            return None
        index += 1
    return None


def matching_brace(source: str, opening_brace: int) -> int:
    depth = 0
    state = "code"
    index = opening_brace
    while index < len(source):
        char = source[index]
        nxt = source[index + 1] if index + 1 < len(source) else ""
        if state == "line":
            if char == "\n":
                state = "code"
        elif state == "block":
            if char == "*" and nxt == "/":
                state = "code"
                index += 1
        elif state in {"string", "char"}:
            if char == "\\":
                index += 1
            elif (state == "string" and char == '"') or (state == "char" and char == "'"):
                state = "code"
        elif char == "/" and nxt == "/":
            state = "line"
            index += 1
        elif char == "/" and nxt == "*":
            state = "block"
            index += 1
        elif char == '"':
            state = "string"
        elif char == "'":
            state = "char"
        elif char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return index
        index += 1
    raise ValueError("Unbalanced braces while locating function")


# Private aliases matching the previous xenoblade_project.py names.
_find_function_region = find_function_region
_replace_function_source = replace_function_source
_find_tu_slots = find_tu_slots
_signature_open_brace = signature_open_brace
_matching_brace = matching_brace

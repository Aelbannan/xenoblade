"""Shared C++ source-region locator for the LLM harness and Decomp Atlas.

Resolves function bodies via stable harness markers or demangled-name brace
matching. Both callers must import this module so they never disagree about
constructors, destructors, qualified methods, or nested braces.
"""
from __future__ import annotations

import re
from dataclasses import dataclass
from typing import Dict, Protocol


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


def replace_function_source(source: str, region: SourceRegion, replacement: str) -> str:
    value = replacement.strip()
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

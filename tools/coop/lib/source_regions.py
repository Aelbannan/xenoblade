"""Shared C++ source-region locator (coop runner).

Resolves function bodies via stable harness markers or demangled-name brace
matching. Formerly part of the retired ``tools/llm_decomp`` package; relocated
here because ``tools/coop/scaffold_missing_slots.py`` depends on it.
"""
from __future__ import annotations

import re
from dataclasses import dataclass
from typing import Optional, Protocol


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

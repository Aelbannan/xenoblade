from __future__ import annotations

import os
import re
from dataclasses import dataclass, field
from typing import List, Optional

DIAGNOSTIC_RULES: list[tuple[str, list[str]]] = [
    ("syntax_error", [
        r"parse error",
        r"expected .+ before",
        r"syntax error",
    ]),
    ("unknown_identifier", [
        r"'(?P<symbol>[^']+)' was not declared",
    ]),
    ("unknown_member", [
        r"has no member named '(?P<symbol>[^']+)'",
    ]),
    ("argument_count", [
        r"too (?:few|many) arguments",
    ]),
    ("invalid_conversion", [
        r"cannot convert",
        r"invalid conversion",
    ]),
    ("incomplete_type", [
        r"incomplete type",
    ]),
    ("access_control", [
        r"is private",
        r"is protected",
    ]),
    ("linkage_error", [
        r"undefined reference",
        r"unresolved external",
    ]),
]


@dataclass(frozen=True)
class NormalizedDiagnostic:
    category: str
    message: str
    file: str | None = None
    line: int | None = None
    column: int | None = None
    symbol: str | None = None
    fingerprint: str = ""


def _extract_symbol(message: str, category: str) -> str | None:
    for cat, patterns in DIAGNOSTIC_RULES:
        if cat != category:
            continue
        for pat in patterns:
            m = re.search(pat, message)
            if m and "symbol" in m.groupdict():
                return m.group("symbol")
    return None


def _classify_message(message: str) -> str:
    for category, patterns in DIAGNOSTIC_RULES:
        for pat in patterns:
            if re.search(pat, message):
                return category
    return "other"


def _build_fingerprint(category: str, symbol: str | None, normalized_message: str) -> str:
    if symbol:
        return f"{category}:{symbol}"
    return f"{category}:{normalized_message[:60]}"


def normalize_diagnostic(line: str, workspace_roots: Optional[list[str]] = None) -> NormalizedDiagnostic | None:
    if workspace_roots is None:
        workspace_roots = []

    stripped = line.strip()
    if not stripped:
        return None

    gcc_re = re.compile(
        r'^'
        r'(?:{.+?:\d+:\d+:\s+)?'  # optional build system prefix
        r'(?P<file>\S+?)'
        r':(?P<line>\d+)'
        r'(?::(?P<column>\d+))?'
        r':\s+'
        r'(?P<message>.+)'
        r'$'
    )

    mwcc_re = re.compile(
        r'^'
        r'(?:{.+?:\d+:\d+:\s+)?'
        r'(?P<file>\S+?)'
        r'\((?P<line>\d+)\)'
        r'(?:\s*:\s*)?'
        r'(?P<message>.+)'
        r'$'
    )

    gcc_match = gcc_re.match(stripped)
    mwcc_match = mwcc_re.match(stripped) if not gcc_match else None
    match = gcc_match or mwcc_match

    if not match:
        return None

    raw_file = match.group("file")
    raw_message = match.group("message")

    is_gcc = gcc_match is not None

    file = raw_file
    for root in workspace_roots:
        root_norm = os.path.normpath(root)
        if raw_file.startswith(root_norm):
            rel = raw_file[len(root_norm):].lstrip("/\\")
            file = rel if rel else raw_file
            break

    normalizations = [
        (r"'[^']+'", lambda m: f"'{m.group()[1:-1]}'"),
        (r'"[^"]+"', lambda m: f'"{m.group()[1:-1]}"'),
        (r'\b0x[0-9a-fA-F]+\b', '<ADDR>'),
        (r'\b(?:unsigned\s+)?(?:long\s+)?(?:int|char|short|float|double|void)\b', '<TYPE>'),
    ]
    normalized_message = raw_message
    for pat, repl in normalizations:
        if callable(repl):
            normalized_message = re.sub(pat, repl, normalized_message)
        else:
            normalized_message = re.sub(pat, repl, normalized_message)

    category = _classify_message(raw_message)
    symbol = _extract_symbol(raw_message, category)
    fp = _build_fingerprint(category, symbol, normalized_message)
    column = int(match.group("column")) if is_gcc and match.group("column") else None

    return NormalizedDiagnostic(
        category=category,
        message=raw_message,
        file=file,
        line=int(match.group("line")),
        column=column,
        symbol=symbol,
        fingerprint=fp,
    )


def normalize_compile_output(
    raw_output: str,
    workspace_roots: Optional[list[str]] = None,
) -> list[NormalizedDiagnostic]:
    diagnostics: list[NormalizedDiagnostic] = []
    for line in raw_output.split("\n"):
        diag = normalize_diagnostic(line, workspace_roots)
        if diag:
            diagnostics.append(diag)
    return diagnostics


TYPE_CASCADE_MARKERS: dict[str, list[str]] = {
    "syntax_error": [],
    "unknown_identifier": [],
    "unknown_member": ["unknown_identifier"],
    "argument_count": ["unknown_identifier"],
    "invalid_conversion": ["unknown_identifier", "unknown_member", "incomplete_type"],
    "incomplete_type": ["unknown_identifier"],
    "access_control": ["unknown_identifier"],
    "linkage_error": [],
}


def _is_cascade(category: str, prior_categories: list[str]) -> bool:
    causers = TYPE_CASCADE_MARKERS.get(category, [])
    if not causers:
        return False
    return any(c in prior_categories for c in causers)


def select_root_diagnostic(diagnostics: list[NormalizedDiagnostic]) -> NormalizedDiagnostic | None:
    if not diagnostics:
        return None

    for d in diagnostics:
        if d.category == "syntax_error":
            return d

    seen_categories: list[str] = []
    for d in diagnostics:
        if not _is_cascade(d.category, seen_categories):
            seen_categories.append(d.category)
            return d

    for d in diagnostics:
        if d.category == "linkage_error":
            return d

    return diagnostics[0]

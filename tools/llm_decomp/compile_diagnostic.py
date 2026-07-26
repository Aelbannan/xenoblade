from __future__ import annotations

import os
import re
from dataclasses import dataclass
from typing import List, Optional

DIAGNOSTIC_RULES: list[tuple[str, list[str]]] = [
    ("syntax_error", [
        r"parse error",
        r"expected .+ before",
        r"syntax error",
        r"\)' expected",
        r"';' expected",
        r"illegal (?:token|character|expression)",
    ]),
    ("unknown_identifier", [
        r"'(?P<symbol>[^']+)' was not declared",
        # MWCC: (10140) undefined identifier 'Foo'
        r"undefined identifier\s+'(?P<symbol>[^']+)'",
        r"undeclared identifier\s+'?(?P<symbol>[A-Za-z_]\w*)'?",
    ]),
    ("unknown_member", [
        r"has no member named '(?P<symbol>[^']+)'",
        r"undefined member\s+'(?P<symbol>[^']+)'",
    ]),
    ("argument_count", [
        r"too (?:few|many) arguments",
        r"function call does not match prototype",
    ]),
    ("invalid_conversion", [
        r"cannot convert",
        r"invalid conversion",
        r"illegal implicit conversion",
    ]),
    ("incomplete_type", [
        r"incomplete type",
    ]),
    ("access_control", [
        r"is private",
        r"is protected",
    ]),
    ("redefinition", [
        r"redefined",
        r"illegal function overloading",
        r"illegal use of 'this'",
    ]),
    ("linkage_error", [
        r"undefined reference",
        r"unresolved external",
    ]),
]

# MWCC pretty-printer lines embedded in ninja output.
_MWCC_ERROR_LINE = re.compile(
    r"^\s*#\s+\((\d+)\)\s+(.*\S)\s*$"
)
_MWCC_CODE_LINE = re.compile(
    r"^\s*#\s+(\d+):\s+(.*)$"
)
_MWCC_FILE_LINE = re.compile(
    r"^\s*#\s+File:\s+(\S+)",
    re.IGNORECASE,
)


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
            if m and "symbol" in m.groupdict() and m.group("symbol"):
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

    # MWCC dump: "#   (10140) undefined identifier 'Foo'"
    mwcc_err = _MWCC_ERROR_LINE.match(stripped)
    if mwcc_err:
        message = mwcc_err.group(2)
        category = _classify_message(message)
        symbol = _extract_symbol(message, category)
        return NormalizedDiagnostic(
            category=category,
            message=message,
            file=None,
            line=None,
            column=None,
            symbol=symbol,
            fingerprint=_build_fingerprint(category, symbol, message),
        )

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
    current_file: str | None = None
    current_line: int | None = None
    for line in raw_output.split("\n"):
        file_hit = _MWCC_FILE_LINE.search(line)
        if file_hit:
            current_file = file_hit.group(1).replace("\\", "/")
            continue
        code_hit = _MWCC_CODE_LINE.match(line.strip())
        if code_hit:
            current_line = int(code_hit.group(1))
            continue
        diag = normalize_diagnostic(line, workspace_roots)
        if diag:
            if diag.file is None and current_file:
                diag = NormalizedDiagnostic(
                    category=diag.category,
                    message=diag.message,
                    file=current_file,
                    line=diag.line if diag.line is not None else current_line,
                    column=diag.column,
                    symbol=diag.symbol,
                    fingerprint=diag.fingerprint,
                )
            elif diag.line is None and current_line is not None:
                diag = NormalizedDiagnostic(
                    category=diag.category,
                    message=diag.message,
                    file=diag.file,
                    line=current_line,
                    column=diag.column,
                    symbol=diag.symbol,
                    fingerprint=diag.fingerprint,
                )
            diagnostics.append(diag)
    return diagnostics


def clean_mwcc_diagnostics(raw_output: str, *, max_lines: int = 40) -> str:
    """Return a short MWCC-focused excerpt (no ninja/configure noise)."""
    lines = (raw_output or "").splitlines()
    keep: list[str] = []
    in_mwcc = False
    for line in lines:
        if "mwcceppc" in line.lower() or "### mwcceppc" in line.lower():
            in_mwcc = True
        if in_mwcc or line.lstrip().startswith("#"):
            keep.append(line.rstrip())
            in_mwcc = True
            if "Too many errors" in line or "aborting program" in line:
                break
        if len(keep) >= max_lines:
            break
    if keep:
        return "\n".join(keep)
    # Fallback: last non-empty lines of the dump.
    nonempty = [ln.rstrip() for ln in lines if ln.strip()]
    return "\n".join(nonempty[-max_lines:])


TYPE_CASCADE_MARKERS: dict[str, list[str]] = {
    "syntax_error": [],
    "unknown_identifier": [],
    "unknown_member": ["unknown_identifier"],
    "argument_count": ["unknown_identifier"],
    "invalid_conversion": ["unknown_identifier", "unknown_member", "incomplete_type"],
    "incomplete_type": ["unknown_identifier"],
    "access_control": ["unknown_identifier"],
    "redefinition": [],
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


# Cookbook lines attached to local compile-repair prompts.
LOCAL_FIX_COOKBOOK: dict[str, str] = {
    "unknown_identifier": (
        "Prefer a symbol already listed in the dossier/ASM relocations. "
        "Do not claim a missing header and leave source unchanged — either use an "
        "allowlisted name, adjust the call, or rewrite the expression without it."
    ),
    "redefinition": (
        "Return exactly one function definition. Do not paste namespace/class "
        "wrappers or redefine types. Rename parameter `this` to `this_`."
    ),
    "syntax_error": (
        "Fix the first syntax error only. MWCC rejects `this->~Class()` trampolines "
        "and mangled names used as C identifiers."
    ),
    "argument_count": (
        "Match the callee prototype / ASM arity; do not invent extra parameters."
    ),
    "invalid_conversion": (
        "Fix the cast/return type; void callees cannot be returned as pointers."
    ),
}


def cookbook_for_diagnostics(diagnostics: list[NormalizedDiagnostic]) -> list[str]:
    tips: list[str] = []
    seen: set[str] = set()
    for diag in diagnostics:
        tip = LOCAL_FIX_COOKBOOK.get(diag.category)
        if tip and diag.category not in seen:
            tips.append(f"[{diag.category}] {tip}")
            seen.add(diag.category)
    return tips

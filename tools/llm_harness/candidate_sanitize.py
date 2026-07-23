"""Deterministic MWCC hygiene for LLM candidate sources.

Takes compilable-C++ chores out of the model's hands: rename illegal ``this``
parameters, strip bogus mangled call suffixes, drop leading forward decls /
TU scaffolding, and optionally wrap a body-only response in a locked scaffold
signature from the current slot.
"""
from __future__ import annotations

import re
from pathlib import Path
from typing import Optional

# Flat func_/lbl_ symbols with a trailing MWCC param encoding and no class chunk
# (e.g. ``func_8049B3FC__Fv``). Real member manglings keep a class/namespace
# segment (``__5CView`` / ``__Q34…``) and are left alone.
_FLAT_MANGLED_CALL = re.compile(
    r"\b((?:func_|lbl_)[0-9A-Fa-f]+)__((?:F|C|Q)[A-Za-z0-9_]*)\b"
)

# Leading single-line forward decls / extern protos before the real definition.
_LEADING_PROTO = re.compile(
    r"(?m)^[ \t]*(?:extern\s+(?:\"C\"\s+)?)?[\w:\s\*\&]+?\b[A-Za-z_]\w*"
    r"\s*\([^;{}]*\)\s*(?:const\s*)?;[ \t]*\n+"
)

# Entire ``namespace X { … class Y { … } }`` wrapper pasted into a function slot.
_NAMESPACE_CLASS_WRAP = re.compile(
    r"(?s)^\s*namespace\s+\w+\s*\{.*\bclass\s+\w+\s*\{.*\}\s*\}\s*(?:\}\s*)+$"
)


def normalize_source_for_compare(source: str) -> str:
    """Whitespace-insensitive compare key for no-op repair detection."""
    return re.sub(r"\s+", " ", (source or "").strip())


def extract_signature_prefix(definition: str) -> Optional[str]:
    """Return the signature text before the function body's opening ``{``."""
    text = (definition or "").strip()
    if not text or "{" not in text:
        return None
    depth = 0
    state = "code"
    i = 0
    # Prefer the ``(`` that belongs to the function declarator, then its ``{``.
    paren = text.find("(")
    if paren < 0:
        return None
    i = paren
    while i < len(text):
        ch = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""
        if state == "code":
            if ch == "/" and nxt == "/":
                state = "line_comment"
                i += 2
                continue
            if ch == "/" and nxt == "*":
                state = "block_comment"
                i += 2
                continue
            if ch in ('"', "'"):
                state = ch
                i += 1
                continue
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth = max(0, depth - 1)
            elif ch == "{" and depth == 0:
                sig = text[:i].rstrip()
                return sig or None
        elif state == "line_comment":
            if ch == "\n":
                state = "code"
        elif state == "block_comment":
            if ch == "*" and nxt == "/":
                state = "code"
                i += 2
                continue
        else:
            if ch == "\\" and nxt:
                i += 2
                continue
            if ch == state:
                state = "code"
        i += 1
    return None


def scaffold_from_body(signature: str, body_or_source: str) -> str:
    """Wrap a body-only ``{ … }`` (or bare stmts) in *signature*."""
    sig = (signature or "").rstrip()
    body = (body_or_source or "").strip()
    if not sig or not body:
        return body_or_source
    if body.startswith("{"):
        return f"{sig} {body}"
    return f"{sig} {{\n{body}\n}}"


def _strip_namespace_class_wrappers(source: str) -> str:
    text = source.strip()
    if not _NAMESPACE_CLASS_WRAP.match(text):
        return source
    # Pull the innermost method-looking definition if present.
    match = re.search(
        r"([A-Za-z_]\w*(?:::[A-Za-z_]\w*)*\s*\([^;{}]*\)\s*(?:const\s*)?\{)",
        text,
    )
    if not match:
        return source
    start = match.start()
    # Walk braces from the matched ``{``.
    brace = text.find("{", match.start())
    if brace < 0:
        return source
    depth = 0
    for i in range(brace, len(text)):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return text[start : i + 1]
    return source


def _keep_primary_definition(source: str) -> str:
    """Drop leading protos; keep the last complete function definition."""
    text = source.strip()
    if not text:
        return source
    text = _LEADING_PROTO.sub("", text, count=8).lstrip()
    defs = list(
        re.finditer(
            r"((?:extern\s+\"C\"\s+)?[\w:\s\*\&]+?\b[A-Za-z_]\w*"
            r"(?:::[A-Za-z_]\w*)*\s*\([^;{}]*\)\s*(?:const\s*)?\{)",
            text,
        )
    )
    if len(defs) <= 1:
        return text
    # Prefer the last definition (models often emit helper + target).
    last = defs[-1]
    brace = text.find("{", last.start())
    if brace < 0:
        return text
    depth = 0
    for i in range(brace, len(text)):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return text[last.start() : i + 1]
    return text[last.start() :]


def _rewrite_flat_mangled_idents(source: str) -> str:
    return _FLAT_MANGLED_CALL.sub(r"\1", source)


def _rewrite_this_params(source: str) -> str:
    """Rename parameter identifier ``this`` → ``this_`` (not ``this->``)."""

    def replacer(match: re.Match[str]) -> str:
        # Skip ``this->`` / ``*this`` member uses: those have non-param neighbors.
        return "this_"

    # Only rewrite inside the first parameter list of the primary definition.
    paren = source.find("(")
    brace = source.find("{")
    if paren < 0 or brace < 0 or brace < paren:
        return source
    head = source[: paren + 1]
    params = source[paren + 1 : brace]
    # Find matching close paren before ``{``.
    depth = 1
    close = None
    for i, ch in enumerate(params):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                close = i
                break
    if close is None:
        return source
    param_text = params[:close]
    rest = source[paren + 1 + close :]
    # Param lists never contain ``this->``; rename every bare ``this`` identifier.
    param_text = re.sub(r"\bthis\b", "this_", param_text)
    return head + param_text + rest


def sanitize_candidate_source(
    source: str,
    *,
    target_function: str = "",
    target_symbol: str = "",
    source_path: str | Path | None = None,
    scaffold_from: str | None = None,
    local_mode: bool = False,
) -> str:
    """Apply deterministic hygiene. Safe transforms always; extras when *local_mode*."""
    del target_function, target_symbol, source_path  # reserved for future policy
    text = (source or "").strip()
    if not text:
        return source

    # Body-only response: always reattach the locked slot signature when known.
    if scaffold_from:
        sig = extract_signature_prefix(scaffold_from)
        if sig and text.startswith("{") and text.rstrip().endswith("}"):
            text = scaffold_from_body(sig, text)

    if local_mode:
        text = _strip_namespace_class_wrappers(text)
        text = _keep_primary_definition(text)

    text = _rewrite_flat_mangled_idents(text)
    text = _rewrite_this_params(text)

    if local_mode:
        text = _LEADING_PROTO.sub("", text, count=4).lstrip()

    return text.rstrip() + "\n"

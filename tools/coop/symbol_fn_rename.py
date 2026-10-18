"""FUNCTION-symbol renames for the symbol-recovery tooling.

Extends the type-only rename flow (``tools/symrecover.py`` +
``tools/symbolrecover/lib/rename*.py``) with member-ization renames such as::

    SchedList_DropItem  ->  SLList::DropItem(CSchedule*)

which changes the emitted symbol from the ``extern "C"`` free-function name
``SchedList_DropItem`` to the MWCC-mangled member name
``DropItem__6SLListFP9CSchedule``.

Why the old flow is type-only (code cites):

* ``tools/symrecover.py::cmd_rename_plan/cmd_rename_all/cmd_rename_apply``
  take ``old_type``/``new_type`` and call ``plan_rename``/``apply_rename``,
  which only know placeholder type tokens (``UnkClass_XXXXXXXX``).
* ``tools/symbolrecover/lib/rename.py::plan_rename`` finds symbol lines by
  substring and rewrites them with ``replace_type_in_symbol`` (a
  length-prefix token swap).  It cannot compute a member mangling, and it
  never touches source.
* ``tools/symbolrecover/lib/rename_source.py::replace_type_text`` is a blind
  ``str.replace`` of the type token: it can rename an identifier but cannot
  flip ``extern "C"`` off, cannot drop the ``self`` parameter, and cannot
  rewrite call sites ``f(self, x)`` into ``self->f(x)``.
* ``rename-all`` refuses whenever the names differ in length
  (``rename_mangling_compatible``), and a member mangling always differs in
  length -- so every genuine function rename errors out.
* ``tools/symbolrecover/lib/mwcc.py`` has no source-type -> mangled-name
  encoder (only the ``decode_args`` demangle helper, which falls back to
  per-character noise for class-pointer args).

This module adds:

* :func:`encode_type` / :func:`encode_class` / :func:`mangle_member_function`
  -- source-level signature to retail MWCC symbol.
* :func:`parse_function_target` -- parses ``Class::method(params)`` specs.
* :func:`plan_function_rename` -- dry-run only, no writes.  Refuses (raises
  :class:`FnRenameRefusal`) whenever it cannot compute the new symbol or
  cannot find exactly one definition.  Never guesses.
* :func:`apply_function_rename` -- applies a plan with per-file backups
  under ``/tmp`` first.

All entry points take an explicit ``project_root`` so tests can run against
temporary fixtures; the CLI passes the real repo root.
"""

from __future__ import annotations

import argparse
import datetime
import re
import shutil
from dataclasses import dataclass, field
from pathlib import Path

from tools.symbolrecover.lib.mwcc import demangle_symbol
from tools.symbolrecover.lib.parser import PLACEHOLDER_RE

REGIONS = ("us", "eu", "jp")
BACKUP_ROOT = Path("/tmp/symbol_fn_rename")

# Files the scanner considers (definition / declaration / call-site search).
SOURCE_SUFFIXES = {".cpp", ".hpp", ".h", ".c", ".inc"}
SOURCE_SCAN_ROOTS = ("src", "libs")
# Generated context dumps: never rewritten, only listed as "regenerate".
GENERATED_SUFFIXES = (".ctx.c",)
SKIP_DIR_NAMES = {".git", "build", "orig", "node_modules", "__pycache__"}


class FnRenameRefusal(Exception):
    """Conservative refusal: the tool cannot proceed safely. Never guesses."""


# ---------------------------------------------------------------------------
# MWCC type encoder
# ---------------------------------------------------------------------------

# Source spelling (after normalisation) -> MWCC encoding for the base type.
_PRIMITIVES: dict[str, str] = {
    "void": "v",
    "bool": "b",
    "char": "c",
    "signed char": "c",
    "unsigned char": "Uc",
    "short": "s",
    "short int": "s",
    "signed short": "s",
    "signed short int": "s",
    "unsigned short": "Us",
    "unsigned short int": "Us",
    "int": "i",
    "signed": "i",
    "signed int": "i",
    "unsigned": "Ui",
    "unsigned int": "Ui",
    "long": "l",
    "long int": "l",
    "signed long": "l",
    "unsigned long": "Ul",
    "unsigned long int": "Ul",
    "long long": "x",
    "long long int": "x",
    "signed long long": "x",
    "unsigned long long": "Ux",
    "float": "f",
    "double": "d",
    "wchar_t": "w",
    # Game / SDK typedefs.
    "u8": "Uc",
    "u16": "Us",
    "u32": "Ui",
    "u64": "Ux",
    "s8": "c",
    "s16": "s",
    "s32": "i",
    "s64": "x",
    "f32": "f",
    "f64": "d",
}

_IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")


def _collapse_ws(text: str) -> str:
    return re.sub(r"\s+", " ", text.strip())


def _split_top_level(text: str, sep: str = ",") -> list[str]:
    """Split on *sep* ignoring separators nested in (), <>, or {}."""
    parts: list[str] = []
    depth_paren = depth_angle = depth_brace = 0
    current: list[str] = []
    for ch in text:
        if ch == "(":
            depth_paren += 1
        elif ch == ")":
            depth_paren -= 1
        elif ch == "<":
            depth_angle += 1
        elif ch == ">":
            depth_angle -= 1
        elif ch == "{":
            depth_brace += 1
        elif ch == "}":
            depth_brace -= 1
        if ch == sep and depth_paren == 0 and depth_angle == 0 and depth_brace == 0:
            parts.append("".join(current))
            current = []
        else:
            current.append(ch)
    parts.append("".join(current))
    return parts


def encode_class(name: str) -> str:
    """Encode a (possibly namespace-qualified) class name.

    ``SLList`` -> ``6SLList``; ``cf::CfObject`` -> ``Q22cf8CfObject``;
    template suffixes pass through literally with the length covering them
    (``ml::FixStr<32>`` -> ``Q22ml10FixStr<32>``, matching retail).
    """
    name = _collapse_ws(name)
    if not name:
        raise FnRenameRefusal("refusing: empty class name in encode_class")
    raw_parts = name.split("::")
    cleaned: list[str] = []
    for part in raw_parts:
        part = part.strip()
        if not part:
            raise FnRenameRefusal(f"refusing: bad class name {name!r}")
        base = part.split("<", 1)[0]
        if not _IDENT_RE.fullmatch(base):
            raise FnRenameRefusal(f"refusing: bad class-name part {part!r} in {name!r}")
        cleaned.append(part)
    if len(cleaned) == 1:
        return f"{len(cleaned[0])}{cleaned[0]}"
    return "Q" + str(len(cleaned)) + "".join(f"{len(p)}{p}" for p in cleaned)


def encode_type(type_str: str) -> str:
    """Encode one source-level parameter type to its MWCC mangling fragment.

    Raises :class:`FnRenameRefusal` for anything outside the known subset --
    the tool must never guess an encoding.
    """
    text = _collapse_ws(type_str)
    if not text:
        raise FnRenameRefusal("refusing: empty parameter type")
    if text == "...":
        raise FnRenameRefusal("refusing: variadic '...' has no stable MWCC encoding here")

    # Reference?
    is_ref = False
    if text.endswith("&"):
        is_ref = True
        text = _collapse_ws(text[:-1])
        if not text or text.endswith("&") or text.endswith("*"):
            raise FnRenameRefusal(f"refusing: cannot encode type {type_str!r}")

    # Pointers (possibly several).  A 'const' immediately before a '*' binds
    # to the pointee (``char const *``); a trailing 'const' after the last '*'
    # is top-level and ignored for mangling.
    pointee_const = False
    ptr_depth = 0
    rest = text
    while True:
        m = re.search(r"\*\s*(const\s*)?$", rest)
        if not m:
            break
        ptr_depth += 1
        rest = _collapse_ws(rest[: m.start()])
        if not rest:
            raise FnRenameRefusal(f"refusing: cannot encode type {type_str!r}")
    # Top-level const on the parameter itself (``T * const`` handled above;
    # ``const T`` / ``T const`` without pointer/reference) does not affect the
    # mangling for by-value params; for references the const-ness is explicit
    # in the spelling and handled below.
    rest_leading_const = False
    if rest.startswith("const "):
        rest_leading_const = True
        rest = _collapse_ws(rest[len("const "):])
    elif rest.startswith("const"):
        raise FnRenameRefusal(f"refusing: cannot encode type {type_str!r}")
    if rest.endswith(" const"):
        rest = _collapse_ws(rest[: -len(" const")])
        if ptr_depth == 0 and not is_ref:
            pass  # top-level const on by-value param: ignored
        elif ptr_depth > 0:
            pointee_const = True  # e.g. 'char const *'
    if rest.startswith("volatile") or "volatile" in rest.split():
        raise FnRenameRefusal(f"refusing: volatile type {type_str!r} has no known encoding")

    if not rest:
        raise FnRenameRefusal(f"refusing: cannot encode type {type_str!r}")
    if rest == "void" and ptr_depth == 0 and not is_ref:
        raise FnRenameRefusal(
            f"refusing: bare 'void' parameter in {type_str!r} (use empty list for ())"
        )

    if is_ref:
        # const T& -> RC<enc>; T& -> R<enc>.
        const_ref = rest_leading_const or rest.endswith(" const")
        if rest.endswith(" const"):
            rest = _collapse_ws(rest[: -len(" const")])
        base_code = _encode_base(rest, type_str)
        return ("RC" if const_ref or rest_leading_const else "R") + base_code

    if ptr_depth:
        if rest_leading_const:
            pointee_const = True
        base_code = _encode_base(rest, type_str)
        return "P" + ("C" if pointee_const else "") + "P" * (ptr_depth - 1) + base_code

    if rest_leading_const:
        # 'const T' by value: top-level const, ignored.
        pass
    return _encode_base(rest, type_str)


def _encode_base(base: str, original: str) -> str:
    base = _collapse_ws(base)
    if base in _PRIMITIVES:
        return _PRIMITIVES[base]
    if base == "void":
        return "v"  # only reachable as a pointer pointee (void*) or reference target
    # Class (possibly qualified / templated) passed by value or pointed to.
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_:]*(<.*>)?", base):
        # Reject function-pointer-ish or otherwise exotic spellings.
        if "(" in base or ")" in base:
            raise FnRenameRefusal(f"refusing: cannot encode type {original!r}")
        return encode_class(base)
    raise FnRenameRefusal(f"refusing: unknown type {original!r} (no MWCC encoding known)")


def mangle_member_function(class_name: str, method: str, params: list[str]) -> str:
    """Build the retail MWCC symbol for ``class_name::method(params)``.

    ``("SLList", "DropItem", ["CSchedule*"])`` ->
    ``"DropItem__6SLListFP9CSchedule"``.
    """
    if not _IDENT_RE.fullmatch(method) and not re.fullmatch(r"~[A-Za-z_][A-Za-z0-9_]*", method):
        raise FnRenameRefusal(f"refusing: bad method name {method!r}")
    if method.startswith("~"):
        class_part = encode_class(class_name)
        args = "F" + "".join(encode_type(p) for p in params) if params else "Fv"
        return f"__dt__{class_part}{args}"
    if method == class_name.split("::")[-1].split("<")[0]:
        # Constructor spelled Class::Class -- retail uses __ct__ form.
        class_part = encode_class(class_name)
        args = "F" + "".join(encode_type(p) for p in params) if params else "Fv"
        return f"__ct__{class_part}{args}"
    args = "F" + "".join(encode_type(p) for p in params) if params else "Fv"
    return f"{method}__{encode_class(class_name)}{args}"


# ---------------------------------------------------------------------------
# Target-spec parsing
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class FunctionTarget:
    class_name: str
    method: str
    params: list[str] | None  # None = derive from the definition
    raw: str

    @property
    def qualified(self) -> str:
        base = f"{self.class_name}::{self.method}"
        if self.params is None:
            return base
        return f"{base}({', '.join(self.params)})"


def parse_function_target(spec: str) -> FunctionTarget:
    """Parse ``Class::method``, ``Class::method(p1, p2)`` or
    ``[ret ]Class::method(...)`` (optional return-type prefix, ignored for
    mangling but kept for display)."""
    text = _collapse_ws(spec)
    if "::" not in text:
        raise FnRenameRefusal(
            f"refusing: function target {spec!r} must look like "
            "'Class::method(Type1, Type2)' (no '::' found)"
        )
    left, _, right = text.partition("::")
    right = right.strip()
    if not right:
        raise FnRenameRefusal(f"refusing: missing method name in {spec!r}")
    left = _collapse_ws(left)
    # Optional return-type prefix on the left: 'void SLList' -> cls 'SLList'.
    left_parts = left.rsplit(" ", 1)
    if len(left_parts) == 2 and not any(ch in left_parts[1] for ch in "*&<>"):
        class_name = left_parts[1]
    else:
        class_name = left
    for part in class_name.split("::"):
        base = part.split("<", 1)[0]
        if not _IDENT_RE.fullmatch(base):
            raise FnRenameRefusal(f"refusing: bad class name {class_name!r} in {spec!r}")
    if "<" in class_name or ">" in class_name:
        raise FnRenameRefusal(
            f"refusing: templated class {class_name!r} cannot be a rename target"
        )
    m = re.match(r"(~?[A-Za-z_][A-Za-z0-9_]*)\s*(\(.*\))?\s*$", right, re.DOTALL)
    if not m:
        raise FnRenameRefusal(f"refusing: bad method clause {right!r} in {spec!r}")
    method, parens = m.group(1), m.group(2)
    params: list[str] | None = None
    if parens is not None:
        inner = _collapse_ws(parens[1:-1])
        if inner in ("", "void"):
            params = []
        else:
            params = [_collapse_ws(p) for p in _split_top_level(inner)]
            if any(p == "" for p in params):
                raise FnRenameRefusal(f"refusing: empty parameter in {spec!r}")
    return FunctionTarget(class_name=class_name, method=method, params=params, raw=spec)


# ---------------------------------------------------------------------------
# Source scanning
# ---------------------------------------------------------------------------

_SELF_PARAM_RE = re.compile(
    r"^\s*(?:const\s+)?[A-Za-z_][A-Za-z0-9_:]*(?:\s+[A-Za-z_][A-Za-z0-9_:]*)*"
    r"\s*\*\s*self\s*$"
)
_VOID_SELF_RE = re.compile(r"^\s*void\s*\*\s*self\s*$")
_SELF_USE_RE = re.compile(r"(?<![A-Za-z0-9_:.>-])self(?![A-Za-z0-9_])")
# A body-local declaration of `self` (shadowing the parameter).  Up to two
# leading type words; control keywords can never start a declaration.
_SHADOW_RE = re.compile(
    r"(?:^|[;{}])\s*"
    r"(?!(?:return|if|for|while|switch|sizeof|delete|new|throw|else|do|case|goto)\b)"
    r"[A-Za-z_][A-Za-z0-9_:<>]*(?:\s+[A-Za-z_][A-Za-z0-9_:<>]*)?\s*(?:[*&]\s*)?"
    r"self\s*(?=[=;,)\]])"
)


def _iter_source_files(project_root: Path) -> list[Path]:
    files: list[Path] = []
    for root_name in SOURCE_SCAN_ROOTS:
        root = project_root / root_name
        if not root.is_dir():
            continue
        for path in sorted(root.rglob("*")):
            if not path.is_file():
                continue
            if any(part in SKIP_DIR_NAMES for part in path.parts):
                continue
            if path.name.endswith(GENERATED_SUFFIXES):
                continue
            if path.suffix in SOURCE_SUFFIXES:
                files.append(path)
    return files


def _code_part(line: str) -> str:
    """Line with the trailing // comment removed (block comments kept)."""
    idx = line.find("//")
    return line[:idx] if idx >= 0 else line


def _find_matching_paren(text: str, open_idx: int) -> int:
    depth = 0
    in_str = in_char = False
    escape = False
    i = open_idx
    while i < len(text):
        ch = text[i]
        if escape:
            escape = False
        elif in_str:
            if ch == "\\":
                escape = True
            elif ch == '"':
                in_str = False
        elif in_char:
            if ch == "\\":
                escape = True
            elif ch == "'":
                in_char = False
        elif ch == '"':
            in_str = True
        elif ch == "'":
            in_char = True
        elif ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


def _find_matching_brace(text: str, open_idx: int) -> int:
    depth = 0
    in_str = in_char = False
    in_line_comment = in_block_comment = False
    escape = False
    i = open_idx
    while i < len(text):
        ch = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""
        if escape:
            escape = False
        elif in_line_comment:
            if ch == "\n":
                in_line_comment = False
        elif in_block_comment:
            if ch == "*" and nxt == "/":
                in_block_comment = False
                i += 1
        elif in_str:
            if ch == "\\":
                escape = True
            elif ch == '"':
                in_str = False
        elif in_char:
            if ch == "\\":
                escape = True
            elif ch == "'":
                in_char = False
        elif ch == "/" and nxt == "/":
            in_line_comment = True
            i += 1
        elif ch == "/" and nxt == "*":
            in_block_comment = True
            i += 1
        elif ch == '"':
            in_str = True
        elif ch == "'":
            in_char = True
        elif ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


@dataclass
class SourceHit:
    path: Path
    start_line: int  # 1-based
    end_line: int  # 1-based, inclusive
    kind: str  # 'decl' | 'defn' | 'call' | 'addr' | 'review'
    old_span: str  # exact source text to replace
    new_span: str  # replacement text ('' when review-only)
    note: str = ""


def _classify_and_rewrite(
    path: Path,
    text: str,
    old: str,
    target: FunctionTarget,
    new_mangled: str,
    free_rename_new: str | None,
) -> list[SourceHit]:
    """Classify every code-part occurrence of *old* in *text*.

    *free_rename_new* is set for plain ``extern "C"`` identifier renames
    (no ``::`` in the target): occurrences are swapped 1:1 with no signature
    change.  Otherwise the member-ization rewrites apply.
    """
    hits: list[SourceHit] = []
    token_re = re.compile(r"(?<![A-Za-z0-9_:~])" + re.escape(old) + r"(?![A-Za-z0-9_])")
    for match in token_re.finditer(text):
        # Skip occurrences inside // comments or string/char literals on this line.
        line_start = text.rfind("\n", 0, match.start()) + 1
        line_end = text.find("\n", match.end())
        if line_end < 0:
            line_end = len(text)
        line = text[line_start:line_end]
        code = _code_part(line)
        col = match.start() - line_start
        if col >= len(code):
            continue  # comment-only occurrence
        before = code[:col]
        after = code[col + len(old):]
        start_line = text.count("\n", 0, match.start()) + 1

        stripped_after = after.lstrip()
        if free_rename_new is not None:
            # Plain identifier swap; no signature or linkage change.
            # old_span covers just the identifier token, so multi-line
            # declarations and calls are handled uniformly.
            if stripped_after.startswith("(") or re.match(r"\s*[;,)]", after) or after.strip() == "":
                hits.append(SourceHit(path, start_line, start_line, "call", old, free_rename_new))
            else:
                hits.append(
                    SourceHit(path, start_line, start_line, "review", old, "",
                              note=f"occurrence of {old} in unrecognized context: {line.strip()[:100]}")
                )
            continue

        # Member-ization path.
        if before.rstrip().endswith("&"):
            # Address reference, e.g. (u32)&Old in dissolved-data vtables.
            amp_col = before.rfind("&")
            amp_abs = line_start + amp_col
            hits.append(
                SourceHit(
                    path, start_line, start_line, "addr",
                    text[amp_abs: match.end()], f"&{target.class_name}::{target.method}",
                    note="member-pointer expression; caller must add a cast if the context needs an address",
                )
            )
            continue
        if stripped_after.startswith("("):
            open_idx = match.end() + (len(after) - len(after.lstrip()))
            close_idx = _find_matching_paren(text, open_idx)
            if close_idx < 0:
                hits.append(
                    SourceHit(path, start_line, start_line, "review", old, "",
                              note="unbalanced parentheses; manual rewrite required")
                )
                continue
            args_text = text[open_idx + 1: close_idx]
            tail = text[close_idx + 1:]
            tail_stripped = tail.lstrip()
            # Is this a declaration/definition (preceded by a return type) or a call?
            # A call's prefix is empty or ends with an operator / keyword.
            stripped_before = before.strip()
            looks_like_decl = (
                bool(stripped_before)
                and re.search(r"[A-Za-z_:0-9*>&\]]\s*$", before) is not None
                and re.search(r"[=,(!?;{}]\s*$", before) is None
                and re.search(r"\b(return|sizeof|decltype|new|delete|throw|case)\s*$", before) is None
            )
            end_line = text.count("\n", 0, close_idx) + 1
            params = _split_top_level(args_text)
            first = _collapse_ws(params[0]) if params and _collapse_ws(params[0]) else ""
            is_self_first = bool(first) and (
                _SELF_PARAM_RE.match(first) is not None or _VOID_SELF_RE.match(first) is not None
            )
            if looks_like_decl:
                # Extend the span back to the statement start so the rewrite
                # sees (and can drop) an 'extern "C"' prefix and keeps the
                # return type.  Stop at statement terminators or line start.
                bol = text.rfind("\n", 0, match.start()) + 1
                stmt_start = bol
                seg = text[bol: match.start()]
                for term in (";", "{", "}"):
                    pos = seg.rfind(term)
                    if pos >= 0:
                        stmt_start = max(stmt_start, bol + pos + 1)
                access = re.search(r"\b(public|private|protected)\s*:\s*$", text[stmt_start: match.start()])
                if access:
                    stmt_start += access.end()
                if not is_self_first:
                    hits.append(
                        SourceHit(path, start_line, end_line, "review", old, "",
                                  note=f"declaration-like use of {old} without a leading 'self' parameter; manual rewrite required")
                    )
                    continue
                if tail_stripped.startswith(";"):
                    kind = "decl"
                elif tail_stripped.startswith("{") or tail_stripped.startswith(":"):
                    kind = "defn"
                else:
                    hits.append(
                        SourceHit(path, start_line, end_line, "review", old, "",
                                  note=f"declaration-like use of {old} followed by unexpected text; manual rewrite required")
                    )
                    continue
                old_span = text[stmt_start: close_idx + 1]
                new_span = _rewrite_decl(old_span, old, target)
                span_start_line = text.count("\n", 0, stmt_start) + 1
                hits.append(SourceHit(path, span_start_line, end_line, kind, old_span, new_span))
            else:
                # Call site: OldName(this_expr, rest...) -> this_expr->method(rest...).
                call_args = _split_top_level(args_text)
                if not call_args or not _collapse_ws(call_args[0]):
                    hits.append(
                        SourceHit(path, start_line, end_line, "review", old, "",
                                  note=f"call of {old} with no arguments; manual rewrite required")
                    )
                    continue
                this_expr = _collapse_ws(call_args[0])
                rest = [_collapse_ws(a) for a in call_args[1:]]
                if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*|this", this_expr):
                    recv = this_expr
                else:
                    recv = f"({this_expr})"
                new_call = f"{recv}->{target.method}({', '.join(rest)})"
                old_span = text[match.start(): close_idx + 1]
                hits.append(SourceHit(path, start_line, end_line, "call", old_span, new_call))
            continue
        # Bare occurrence (macro arg, stringised, header guard, ...).
        hits.append(
            SourceHit(
                path, start_line, start_line, "review", old, "",
                note=f"bare occurrence of {old} outside a call/declaration: {line.strip()[:100]}",
            )
        )
    return hits


def _rewrite_decl(old_span: str, old: str, target: FunctionTarget) -> str:
    """Rewrite one declaration/definition span to member form.

    Drops the leading ``self`` parameter and the ``extern "C"`` prefix, and
    renames ``Old`` to ``Class::method``.  Leading indentation and the return
    type are preserved.
    """
    open_idx = old_span.find("(")
    head, args_text = old_span[:open_idx], old_span[open_idx + 1: old_span.rfind(")")]
    indent_match = re.match(r"[ \t]*", head)
    indent = indent_match.group(0) if indent_match else ""
    head = head[len(indent):]
    head = re.sub(r'extern\s+"C"\s+', "", head, count=1)
    head = re.sub(r"(?<![A-Za-z0-9_:~])" + re.escape(old) + r"(?![A-Za-z0-9_])",
                  f"{target.class_name}::{target.method}", head, count=1)
    params = _split_top_level(args_text)
    params = params[1:]  # drop self
    body_params = [_collapse_ws(p) for p in params]
    return indent + head.rstrip() + "(" + ", ".join(body_params) + ")"


# ---------------------------------------------------------------------------
# Plan
# ---------------------------------------------------------------------------


@dataclass
class SymbolEdit:
    region: str
    line_no: int
    old_line: str
    new_line: str


@dataclass
class FnPlan:
    old: str
    target: FunctionTarget
    new_mangled: str | None  # None for refused plans (never constructed)
    free_rename: bool
    derived_params: bool
    symbol_edits: list[SymbolEdit] = field(default_factory=list)
    regions_missing: list[str] = field(default_factory=list)
    decls: list[SourceHit] = field(default_factory=list)
    defns: list[SourceHit] = field(default_factory=list)
    calls: list[SourceHit] = field(default_factory=list)
    addr_refs: list[SourceHit] = field(default_factory=list)
    reviews: list[SourceHit] = field(default_factory=list)
    body_self_count: int = 0
    configure_hits: list[str] = field(default_factory=list)
    splits_hits: list[str] = field(default_factory=list)
    ctx_files: list[str] = field(default_factory=list)

    @property
    def source_hits(self) -> list[SourceHit]:
        return self.decls + self.defns + self.calls + self.addr_refs


def _read_symbols_name_lines(project_root: Path, region: str) -> list[str] | None:
    path = project_root / "config" / region / "symbols.txt"
    if not path.is_file():
        return None
    return path.read_text(encoding="utf-8").splitlines()


def plan_function_rename(
    project_root: Path,
    old: str,
    new_spec: str,
    regions: list[str] | None = None,
) -> FnPlan:
    """Build a complete deterministic rename plan (dry-run only, no writes).

    Raises :class:`FnRenameRefusal` instead of guessing whenever the new
    symbol cannot be computed or exactly one definition cannot be found.
    """
    regions = list(regions) if regions else list(REGIONS)
    if PLACEHOLDER_RE.match(old):
        raise FnRenameRefusal(
            f"refusing: {old!r} is a placeholder TYPE; use the type rename path"
        )

    free_rename = "::" not in new_spec
    target: FunctionTarget | None = None
    free_new = ""
    if free_rename:
        free_new = _collapse_ws(new_spec)
        if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", free_new):
            raise FnRenameRefusal(
                f"refusing: free-function target {new_spec!r} is not a plain C identifier "
                "(did you mean 'Class::method(...)'?)"
            )
        if free_new == old:
            raise FnRenameRefusal("refusing: old and new names are identical (no-op)")
    else:
        target = parse_function_target(new_spec)
        if target.method == old and target.params == []:
            pass  # still a linkage change; allowed

    # --- symbols.txt entries: exact-name match only (never substring) ---
    symbol_edits: list[SymbolEdit] = []
    regions_missing: list[str] = []
    new_mangled: str
    for region in regions:
        lines = _read_symbols_name_lines(project_root, region)
        if lines is None:
            regions_missing.append(f"{region} (no symbols.txt)")
            continue
        found = False
        for line_no, line in enumerate(lines, start=1):
            stripped = line.strip()
            if not stripped or stripped.startswith("#") or "=" not in stripped:
                continue
            name_part = stripped.split("=", 1)[0].strip()
            if name_part != old:
                continue
            found = True
            if free_rename:
                new_name = free_new
            else:
                assert target is not None
                # New name computed after the definition check below; placeholder.
                new_name = ""
            symbol_edits.append(SymbolEdit(region, line_no, line, new_name))
        if not found:
            regions_missing.append(region)
    if not symbol_edits:
        raise FnRenameRefusal(
            f"refusing: symbol {old!r} not found by exact name in any of "
            f"{', '.join(regions)} (checked config/<region>/symbols.txt)"
        )

    # --- source scan ---
    if free_rename:
        assert free_new
        new_mangled = free_new
        scan_target = FunctionTarget(class_name="", method=free_new, params=[], raw=new_spec)
    else:
        assert target is not None
        scan_target = target
    decls: list[SourceHit] = []
    defns: list[SourceHit] = []
    calls: list[SourceHit] = []
    addr_refs: list[SourceHit] = []
    reviews: list[SourceHit] = []
    for path in _iter_source_files(project_root):
        try:
            text = path.read_text(encoding="utf-8")
        except (OSError, UnicodeDecodeError):
            continue
        if old not in text:
            continue
        for hit in _classify_and_rewrite(
            path, text, old, scan_target, "",
            free_new if free_rename else None,
        ):
            if hit.kind == "decl":
                # _classify used 'decl' for free-rename swaps too; re-file below.
                if free_rename:
                    calls.append(hit)
                else:
                    decls.append(hit)
            elif hit.kind == "defn":
                defns.append(hit)
            elif hit.kind == "call":
                calls.append(hit)
            elif hit.kind == "addr":
                addr_refs.append(hit)
            else:
                reviews.append(hit)

    body_self_count = 0
    derived_params = False
    if free_rename:
        if not (decls or defns or calls or addr_refs) and not reviews:
            raise FnRenameRefusal(
                f"refusing: {old!r} found in symbols.txt but has no source occurrences; "
                "nothing to rewrite (aborting rather than touching symbols.txt alone)"
            )
    else:
        assert target is not None
        if len(defns) == 0:
            raise FnRenameRefusal(
                f"refusing: cannot find the definition of {old!r} "
                "(no 'ret Old(T* self, ...)' definition with a leading self parameter in src/ or libs/)"
            )
        if len(defns) > 1:
            where = ", ".join(
                f"{h.path.relative_to(project_root) if h.path.is_absolute() else h.path}:{h.start_line}"
                for h in defns
            )
            raise FnRenameRefusal(
                f"refusing: {old!r} has {len(defns)} candidate definitions ({where}); "
                "rename is ambiguous"
            )
        defn = defns[0]
        # Verify the self parameter and derive/verify the member signature.
        open_idx = defn.old_span.find("(")
        defn_params = _split_top_level(defn.old_span[open_idx + 1: defn.old_span.rfind(")")])
        self_param = _collapse_ws(defn_params[0])
        if _VOID_SELF_RE.match(self_param):
            self_class: str | None = None  # void* self: class comes from the spec only
        else:
            m = re.match(r"^(?:const\s+)?([A-Za-z_][A-Za-z0-9_:]*)\s*\*\s*self\s*$", self_param)
            if not m:
                raise FnRenameRefusal(
                    f"refusing: definition first parameter {self_param!r} is not a "
                    "'Class* self' pointer (cannot map to 'this')"
                )
            self_class = m.group(1).split("::")[-1]
            if self_class != target.class_name:
                raise FnRenameRefusal(
                    f"refusing: definition self type {self_class!r} != target class "
                    f"{target.class_name!r} (would change the object layout contract)"
                )
        member_params = [_collapse_ws(p) for p in defn_params[1:]]
        if len(member_params) == 1 and member_params[0] == "void":
            member_params = []  # old-style '(Class* self, void)' -> '()'
        # Strip parameter names for encoding (keep types): 'CSchedule* x' -> 'CSchedule*'.
        member_types = [_strip_param_name(p) for p in member_params]
        if target.params is None:
            target = FunctionTarget(
                class_name=target.class_name,
                method=target.method,
                params=member_types,
                raw=target.raw,
            )
            derived_params = True
        else:
            try:
                want = [encode_type(p) for p in target.params]
                have = [encode_type(p) for p in member_types]
            except FnRenameRefusal as exc:
                raise FnRenameRefusal(f"refusing: cannot compute the new symbol ({exc})") from exc
            if want != have:
                raise FnRenameRefusal(
                    f"refusing: target params {target.params!r} do not match the definition "
                    f"params {member_types!r} (after dropping self)"
                )
        assert target.params is not None
        try:
            new_mangled = mangle_member_function(target.class_name, target.method, target.params)
        except FnRenameRefusal as exc:
            raise FnRenameRefusal(f"refusing: cannot compute the new symbol ({exc})") from exc
        if new_mangled == old:
            raise FnRenameRefusal("refusing: old and new symbols are identical (no-op)")
        # Collision check: the new name must not already exist anywhere.
        for region in regions:
            lines = _read_symbols_name_lines(project_root, region)
            if lines is None:
                continue
            for line in lines:
                stripped = line.strip()
                if not stripped or stripped.startswith("#") or "=" not in stripped:
                    continue
                if stripped.split("=", 1)[0].strip() == new_mangled:
                    raise FnRenameRefusal(
                        f"refusing: new symbol {new_mangled!r} already exists in "
                        f"config/{region}/symbols.txt (collision)"
                    )
        # Count body self-uses (definition body span -> self becomes this).
        defn_path_text = defns[0].path.read_text(encoding="utf-8")
        body_self_count, body_has_shadow = _analyze_body(
            defn_path_text, old, defns[0].start_line, defns[0].end_line
        )
        if body_has_shadow:
            where = defns[0].path
            try:
                where_str = str(where.relative_to(project_root))
            except ValueError:
                where_str = str(where)
            raise FnRenameRefusal(
                f"refusing: definition body in {where_str} declares its own "
                "local named 'self' (shadowing); automatic self->this rewrite "
                "would corrupt it -- rename manually"
            )

    # Fill in the symbols.txt new lines (name part only changes).
    filled: list[SymbolEdit] = []
    for edit in symbol_edits:
        line = edit.old_line
        idx = line.find(old)
        assert idx >= 0
        filled.append(
            SymbolEdit(edit.region, edit.line_no, line, line[:idx] + new_mangled + line[idx + len(old):])
        )

    # configure.py / splits.txt impact (function renames must not touch them;
    # any hit is reported and blocks apply).
    configure_hits: list[str] = []
    cfg = project_root / "configure.py"
    if cfg.is_file():
        try:
            for i, line in enumerate(cfg.read_text(encoding="utf-8", errors="ignore").splitlines(), start=1):
                if re.search(r"(?<![A-Za-z0-9_:~])" + re.escape(old) + r"(?![A-Za-z0-9_])", line):
                    configure_hits.append(f"configure.py:{i}: {line.strip()[:120]}")
        except OSError:
            pass
    splits_hits: list[str] = []
    for region in regions:
        sp = project_root / "config" / region / "splits.txt"
        if not sp.is_file():
            continue
        try:
            for i, line in enumerate(sp.read_text(encoding="utf-8", errors="ignore").splitlines(), start=1):
                if old in line:
                    splits_hits.append(f"config/{region}/splits.txt:{i}: {line.strip()[:120]}")
        except OSError:
            pass

    # Generated context dumps mentioning the symbol: list for regeneration.
    ctx_files: list[str] = []
    for root_name in SOURCE_SCAN_ROOTS:
        root = project_root / root_name
        if not root.is_dir():
            continue
        for path in sorted(root.rglob("*.ctx.c")):
            if any(part in SKIP_DIR_NAMES for part in path.parts):
                continue
            try:
                if old in path.read_text(encoding="utf-8", errors="ignore"):
                    ctx_files.append(str(path.relative_to(project_root)))
            except OSError:
                continue

    if free_rename:
        plan_target = FunctionTarget(class_name="", method=free_new, params=[], raw=new_spec)
    else:
        assert target is not None
        plan_target = target
    return FnPlan(
        old=old,
        target=plan_target,
        new_mangled=new_mangled,
        free_rename=free_rename,
        derived_params=derived_params,
        symbol_edits=filled,
        regions_missing=regions_missing,
        decls=decls,
        defns=defns,
        calls=calls,
        addr_refs=addr_refs,
        reviews=reviews,
        body_self_count=body_self_count,
        configure_hits=configure_hits,
        splits_hits=splits_hits,
        ctx_files=ctx_files,
    )


def _strip_param_name(param: str) -> str:
    """Drop the trailing parameter name: 'CSchedule* x' -> 'CSchedule*'."""
    param = _collapse_ws(param)
    # '...' / unnamed / already bare type.
    if param in ("...", "void"):
        return param
    # Peel array declarators (refuse: unknown layout effect).
    if "[" in param:
        raise FnRenameRefusal(f"refusing: array parameter {param!r} has no known encoding")
    # Function pointers: refuse.
    if "(" in param:
        raise FnRenameRefusal(f"refusing: cannot encode parameter {param!r}")
    tokens = param.split()
    if len(tokens) == 1:
        single = tokens[0]
        # Could be 'x' (unnamed? no -- single token is a type) or 'CSchedule*'.
        if single.endswith("*") or single.endswith("&"):
            # 'CSchedule*' -- type only, fine.
            return single
        if _IDENT_RE.fullmatch(single) or single in _PRIMITIVES or "::" in single or "<" in single:
            return single  # bare type, unnamed parameter
        return single
    # Last token may be 'name', '*name', '&name'.
    last = tokens[-1]
    name_candidate = last.lstrip("*&")
    if _IDENT_RE.fullmatch(name_candidate) and (last != name_candidate or len(tokens) > 1):
        # If the whole param is just 'unsigned' etc. handled above (single token).
        # Check the prefix is a plausible type: encode it to be sure.
        prefix = _collapse_ws(" ".join(tokens[:-1]) + (" " + last[: len(last) - len(name_candidate)] if last != name_candidate else ""))
        prefix = _collapse_ws(prefix)
        if prefix == "":
            return name_candidate  # param was just a name?? suspicious
        encode_type(prefix)  # validates; raises on unknown
        return prefix
    # No name to strip (e.g. 'unsigned int', 'const char *').
    encode_type(param)  # validates; raises on unknown
    return param


def _analyze_body(
    file_text: str, old: str, defn_start: int, defn_end: int
) -> tuple[int, bool]:
    """Analyse the definition body: (self-use count, has shadowing local).

    Anchored on the classified definition's line span, so a call site that
    happens to precede the definition can never be mistaken for it.
    """
    token_re = re.compile(r"(?<![A-Za-z0-9_:~])" + re.escape(old) + r"(?![A-Za-z0-9_])")
    for match in token_re.finditer(file_text):
        line_no = file_text.count("\n", 0, match.start()) + 1
        if not (defn_start <= line_no <= defn_end):
            continue
        after = file_text[match.end():].lstrip()
        if not after.startswith("("):
            continue
        open_idx = match.end() + (len(file_text[match.end():]) - len(after))
        close_idx = _find_matching_paren(file_text, open_idx)
        if close_idx < 0:
            continue
        tail = file_text[close_idx + 1:].lstrip()
        if tail.startswith("{") or tail.startswith(":"):
            brace_idx = file_text.find("{", close_idx + 1)
            if brace_idx < 0:
                return 0, False
            end_idx = _find_matching_brace(file_text, brace_idx)
            if end_idx < 0:
                return 0, False
            body = file_text[brace_idx:end_idx]
            return len(_SELF_USE_RE.findall(body)), bool(_SHADOW_RE.search(body))
    return 0, False


# ---------------------------------------------------------------------------
# Rendering
# ---------------------------------------------------------------------------


def _rel(root: Path, path: Path) -> str:
    try:
        return str(path.relative_to(root))
    except ValueError:
        return str(path)


def render_plan_text(plan: FnPlan, project_root: Path, verbose: bool = False, limit: int = 20) -> str:
    lines: list[str] = []
    mode = "free-function rename" if plan.free_rename else "member-ization"
    lines.append(f"function rename plan ({mode}): {plan.old} -> {plan.new_mangled}")
    if not plan.free_rename:
        try:
            info = demangle_symbol(plan.new_mangled or "")
            sig = f"{info.function}{info.args or ''}"
            if info.class_name:
                sig += f"  [class={info.class_name}]"
            lines.append(f"  demangle check: {sig}")
        except Exception:  # never let display break a plan
            pass
        lines.append(
            f"  member signature: {plan.target.class_name}::{plan.target.method}"
            f"({', '.join(plan.target.params or [])})"
            + ("  [params derived from definition]" if plan.derived_params else "")
        )
    lines.append(f"  symbols.txt lines to change: {len(plan.symbol_edits)}")
    for edit in plan.symbol_edits[:limit] if not verbose else plan.symbol_edits:
        lines.append(f"    [{edit.region}:{edit.line_no}]")
        lines.append(f"      - {edit.old_line.strip()[:160]}")
        lines.append(f"      + {edit.new_line.strip()[:160]}")
    if not verbose and len(plan.symbol_edits) > limit:
        lines.append(f"    ... {len(plan.symbol_edits) - limit} more symbol lines")
    if plan.regions_missing:
        lines.append(f"  regions without {plan.old}: {', '.join(plan.regions_missing)}")
    lines.append(f"  definitions: {len(plan.defns)}")
    for hit in plan.defns:
        lines.append(f"    {_rel(project_root, hit.path)}:{hit.start_line}-{hit.end_line}")
        if verbose:
            lines.append(f"      - {hit.old_span[:200]}")
            lines.append(f"      + {hit.new_span[:200]}")
    lines.append(f"  declarations: {len(plan.decls)}")
    for hit in plan.decls:
        lines.append(f"    {_rel(project_root, hit.path)}:{hit.start_line}-{hit.end_line}")
        if verbose:
            lines.append(f"      - {hit.old_span[:200]}")
            lines.append(f"      + {hit.new_span[:200]}")
    lines.append(f"  call sites: {len(plan.calls)}")
    for hit in plan.calls[:limit] if not verbose else plan.calls:
        lines.append(f"    {_rel(project_root, hit.path)}:{hit.start_line}-{hit.end_line}")
        if verbose:
            lines.append(f"      - {hit.old_span[:200]}")
            lines.append(f"      + {hit.new_span[:200]}")
    if not verbose and len(plan.calls) > limit:
        lines.append(f"    ... {len(plan.calls) - limit} more call sites")
    lines.append(f"  address refs (&{plan.old}): {len(plan.addr_refs)}")
    for hit in plan.addr_refs:
        lines.append(f"    {_rel(project_root, hit.path)}:{hit.start_line} NEEDS-REVIEW: {hit.note}")
        if verbose:
            lines.append(f"      - {hit.old_span[:160]}")
            lines.append(f"      + {hit.new_span[:160]}")
    if plan.reviews:
        lines.append(f"  NEEDS-REVIEW occurrences: {len(plan.reviews)}")
        for hit in plan.reviews:
            lines.append(f"    {_rel(project_root, hit.path)}:{hit.start_line}: {hit.note[:160]}")
    if not plan.free_rename:
        lines.append(f"  definition-body 'self' uses (-> 'this'): {plan.body_self_count}")
    lines.append(
        "  configure.py hits: "
        + (", ".join(plan.configure_hits) if plan.configure_hits else "none")
    )
    lines.append(
        "  splits.txt hits: " + (", ".join(plan.splits_hits) if plan.splits_hits else "none")
    )
    if plan.ctx_files:
        lines.append("  generated .ctx.c files mentioning the symbol (regenerate, do not edit):")
        for name in plan.ctx_files:
            lines.append(f"    {name}")
    return "\n".join(lines)


# ---------------------------------------------------------------------------
# Apply
# ---------------------------------------------------------------------------


def _backup_path(stamp: str, project_root: Path, path: Path) -> Path:
    try:
        rel = path.relative_to(project_root)
    except ValueError:
        rel = Path(path.name)
    dest = BACKUP_ROOT / stamp / rel
    dest.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(path, dest)
    return dest


def apply_function_rename(
    project_root: Path,
    old: str,
    new_spec: str,
    regions: list[str] | None = None,
    dry_run: bool = True,
    allow_review: bool = False,
) -> tuple[FnPlan, list[str]]:
    """Apply a function rename.  Returns (plan, log_lines).

    * Always builds the plan first (refusals propagate, nothing written).
    * Refuses to write when configure.py/splits.txt mention the symbol.
    * Refuses to write when NEEDS-REVIEW items exist unless *allow_review*.
    * With *dry_run* True: no writes, log says what would change.
    * Otherwise: backs every touched file up under /tmp/symbol_fn_rename/
      before writing.
    """
    plan = plan_function_rename(project_root, old, new_spec, regions)
    assert plan.new_mangled is not None
    log: list[str] = [render_plan_text(plan, project_root, verbose=False)]
    # Dry-run never refuses: its job is to show the plan.  The gates below
    # apply to real writes only.
    if dry_run:
        if plan.configure_hits or plan.splits_hits:
            log.append("")
            log.append("NOTE: apply would refuse (configure.py/splits.txt hits).")
        if (plan.reviews or plan.addr_refs) and not allow_review:
            log.append("NOTE: apply would refuse without --allow-review "
                        f"({len(plan.reviews)} review + {len(plan.addr_refs)} addr-ref items).")
        log.append("")
        log.append("dry-run: no files written. Re-run without --dry-run to apply.")
        return plan, log
    if plan.configure_hits or plan.splits_hits:
        raise FnRenameRefusal(
            "refusing to apply: configure.py/splits.txt mention "
            f"{old!r} (function renames must not touch build metadata; "
            "resolve manually first)"
        )
    if plan.reviews and not allow_review:
        raise FnRenameRefusal(
            f"refusing to apply: {len(plan.reviews)} NEEDS-REVIEW occurrence(s); "
            "re-run with --allow-review to apply anyway (review items are left untouched)"
        )
    if plan.addr_refs and not allow_review:
        raise FnRenameRefusal(
            f"refusing to apply: {len(plan.addr_refs)} address ref(s) (&{old}) need "
            "a member-pointer cast decision; re-run with --allow-review"
        )

    stamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    # Group source hits by file.
    by_file: dict[Path, list[SourceHit]] = {}
    for hit in plan.source_hits:
        by_file.setdefault(hit.path, []).append(hit)

    # --- symbols.txt ---
    by_region: dict[str, list[SymbolEdit]] = {}
    for edit in plan.symbol_edits:
        by_region.setdefault(edit.region, []).append(edit)
    for region, edits in sorted(by_region.items()):
        sym_path = project_root / "config" / region / "symbols.txt"
        backup = _backup_path(stamp, project_root, sym_path)
        lines = sym_path.read_text(encoding="utf-8").splitlines(keepends=True)
        for edit in edits:
            idx = edit.line_no - 1
            old_line = lines[idx]
            new_line = old_line.replace(old, plan.new_mangled, 1)
            # Preserve the original line ending.
            if old_line.endswith("\n") and not new_line.endswith("\n"):
                new_line += "\n"
            lines[idx] = new_line
        sym_path.write_text("".join(lines), encoding="utf-8")
        log.append(f"updated {sym_path.relative_to(project_root)} ({len(edits)} lines; backup {backup})")

    # --- source files ---
    saw_defn = False
    for path in sorted(by_file):
        hits = sorted(by_file[path], key=lambda h: h.start_line)
        original = path.read_text(encoding="utf-8")
        offset_hits = _locate_spans(original, hits, old)
        backup = _backup_path(stamp, project_root, path)
        # Rebuild forward so coordinates stay exact; record where the
        # rewritten definition ends (anchor for the body self->this fix-up).
        defn_ids = {id(d) for d in plan.defns}
        parts: list[str] = []
        cursor = 0
        defn_anchor: int | None = None
        for hit, (start, end) in sorted(offset_hits, key=lambda p: p[1][0]):
            parts.append(original[cursor:start])
            parts.append(hit.new_span)
            if id(hit) in defn_ids:
                defn_anchor = len("".join(parts))
            cursor = end
        parts.append(original[cursor:])
        updated = "".join(parts)
        if not plan.free_rename and defn_anchor is not None:
            saw_defn = True
            updated = _rewrite_body_self(updated, defn_anchor, plan)
        path.write_text(updated, encoding="utf-8")
        log.append(f"updated {_rel(project_root, path)} ({len(hits)} edit(s); backup {backup})")
    if not plan.free_rename and plan.body_self_count and not saw_defn:
        raise FnRenameRefusal(
            "refusing to apply: definition anchor missing for body fix-up"
        )

    log.append("")
    log.append("Next: python configure.py && ninja")
    log.append("Then: python tools/coop/run.py diff <unit> --symbol " + (plan.new_mangled or ""))
    return plan, log


def _locate_spans(
    text: str, hits: list[SourceHit], old: str
) -> list[tuple[SourceHit, tuple[int, int]]]:
    """Map each hit's old_span back to offsets in the current file text."""
    spans: list[tuple[SourceHit, tuple[int, int]]] = []
    cursor = 0
    for hit in sorted(hits, key=lambda h: h.start_line):
        idx = text.find(hit.old_span, cursor)
        if idx < 0:
            # Fall back to a full-text search (spans may repeat, e.g. '&Old').
            idx = text.find(hit.old_span)
        if idx < 0:
            raise FnRenameRefusal(
                f"refusing to apply: cannot relocate edit {hit.old_span[:80]!r} "
                f"in {hit.path} (file changed since planning?)"
            )
        spans.append((hit, (idx, idx + len(hit.old_span))))
        cursor = idx + len(hit.old_span)
    return spans


def _rewrite_body_self(text: str, defn_anchor: int | None, plan: FnPlan) -> str:
    """Replace word-`self` with `this` inside the (rewritten) definition body.

    *defn_anchor* is the offset right after the rewritten definition header,
    recorded during apply -- never re-searched (a declaration can carry the
    same text, so searching would risk the wrong span).
    """
    if not plan.defns or plan.body_self_count == 0:
        return text
    if defn_anchor is None:
        raise FnRenameRefusal(
            "refusing to apply: definition anchor missing for body fix-up"
        )
    brace_idx = text.find("{", defn_anchor)
    if brace_idx < 0:
        raise FnRenameRefusal("refusing to apply: definition body not found")
    end_idx = _find_matching_brace(text, brace_idx)
    if end_idx < 0:
        raise FnRenameRefusal("refusing to apply: unbalanced braces in definition body")
    body = text[brace_idx: end_idx]
    new_body = _SELF_USE_RE.sub("this", body)
    text = text[:brace_idx] + new_body + text[end_idx:]
    return text


# ---------------------------------------------------------------------------
# CLI (invoked from tools/coop/run.py cmd_symbols interception)
# ---------------------------------------------------------------------------


def _add_common(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--region", choices=REGIONS, default=None)
    parser.add_argument("--verbose", action="store_true")
    parser.add_argument("--limit", type=int, default=20)


def cmd_fn_plan(argv: list[str], project_root: Path) -> int:
    parser = argparse.ArgumentParser(prog="symbols rename-plan <old> <new-spec>")
    parser.add_argument("old")
    parser.add_argument("new_spec")
    _add_common(parser)
    args = parser.parse_args(argv)
    regions = [args.region] if args.region else list(REGIONS)
    try:
        plan = plan_function_rename(project_root, args.old, args.new_spec, regions)
    except FnRenameRefusal as exc:
        print(f"ERROR: {exc}", flush=True)
        return 1
    print(render_plan_text(plan, project_root, verbose=args.verbose, limit=args.limit))
    return 0


def cmd_fn_apply(argv: list[str], project_root: Path) -> int:
    parser = argparse.ArgumentParser(prog="symbols rename-all <old> <new-spec>")
    parser.add_argument("old")
    parser.add_argument("new_spec")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--allow-review", action="store_true")
    parser.add_argument("--force", action="store_true", help="(accepted for CLI parity; unused)")
    parser.add_argument("--no-rename-files", action="store_true", help="(accepted for CLI parity; unused)")
    _add_common(parser)
    args = parser.parse_args(argv)
    regions = [args.region] if args.region else list(REGIONS)
    try:
        _plan, log = apply_function_rename(
            project_root, args.old, args.new_spec, regions,
            dry_run=args.dry_run, allow_review=args.allow_review,
        )
    except FnRenameRefusal as exc:
        print(f"ERROR: {exc}", flush=True)
        return 1
    for line in log:
        print(line)
    return 0


def looks_like_function_rename(project_root: Path, old: str, new: str) -> bool:
    """True when a rename-plan/rename-all invocation means a FUNCTION rename.

    Existing type renames (placeholder old name, plain new name) return False
    so they keep flowing to the untouched type path.
    """
    if "::" in new:
        return True
    if PLACEHOLDER_RE.match(old):
        return False
    # Bare old name that exactly matches a function symbol -> free rename path.
    for region in REGIONS:
        path = project_root / "config" / region / "symbols.txt"
        if not path.is_file():
            continue
        try:
            with path.open(encoding="utf-8") as f:
                for line in f:
                    stripped = line.strip()
                    if not stripped or stripped.startswith("#") or "=" not in stripped:
                        continue
                    if stripped.split("=", 1)[0].strip() == old:
                        return True
        except OSError:
            continue
    return False

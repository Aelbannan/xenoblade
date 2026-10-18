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

Call-receiver casts (Option A -- chosen 2026-10-18, see below):

* When a call ``Fn(recv, args...)`` is member-ized to ``recv->Method(args...)``,
  a receiver that is not provably ``Class*``-typed (a bare identifier, ``this``,
  or an existing cast to the target class) is wrapped in an explicit cast:
  ``((Class*)(recv))->Method(args...)``.  The cast is semantics-preserving
  (identical pointer value; a C-style pointer cast emits no code) and is the
  idiom earlier manual fixes used (``((CSchedule*)node->mItem)->Update(dt)``).
  An already-correctly-cast receiver is never double-wrapped.
* Why Option A over Option B (mark such call sites NEEDS-REVIEW so the plan
  is never CLEAN): the un-casttable shapes are exactly the retail idiom for
  embedded subobjects -- raw pointer arithmetic such as
  ``MiniMapFillMarkerEntry((u8*)self + 0x17C)`` (``CMiniMapGimmickView`` lives
  at ``minimap+0x17C``; ``src/kyoshin/CMiniMap.cpp:1127``) -- and member-izing
  without landing the call leaves the function un-memberized forever.  The
  emitted ``((u8*)self + 0x17C)->FillMarkerEntry()`` is ill-formed MWCC
  (``Error: (10141) expression syntax error``: ``u8*`` has no members), which
  failed the build and blocked 11 units in the bulk wave.  Option A keeps the
  pointer-arithmetic smell (a future struct-field cleanup can replace the
  ``+ 0x17C`` with a real member access) but lands the member with valid,
  byte-identical code.  Option B would keep the source cosmetically clean at
  the cost of never member-izing these functions -- the wrong trade for a
  matching fork where the cast compiles away to nothing.

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

from tools.coop.lib.atomic_write import atomic_write_text
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
    # Game / SDK typedefs.  Under MWCC (include/types.h) u32 is
    # ``unsigned long`` (-> Ul, NOT Ui) and s32 is ``signed long``
    # (-> l, NOT i); retail confirms (setBdatEntry__5CBdatFUlPv(u32),
    # wpadGetInfoCallback__Q22cf9CfPadTaskFll(s32,s32)).  ``unsigned int``
    # stays Ui and plain ``int`` stays i -- they are distinct spellings.
    "u8": "Uc",
    "u16": "Us",
    "u32": "Ul",
    "u64": "Ux",
    "s8": "c",
    "s16": "s",
    "s32": "l",
    "s64": "x",
    "f32": "f",
    "f64": "d",
}

_IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")


def _collapse_ws(text: str) -> str:
    return re.sub(r"\s+", " ", text.strip())


def _split_top_level(text: str, sep: str = ",") -> list[str]:
    """Split on *sep* at top level by paren/bracket/brace depth.

    Only ``()``, ``[]`` and ``{}`` nest: ``<``/``>`` are NOT brackets because
    C++ ``->`` and comparison operators make angle-depth unreliable (``->``
    drove the old angle-depth negative and suppressed the top-level comma,
    corrupting ``Old(recv->member, arg)`` into ``(recv, arg)->Method()``
    with dropped arguments).  Commas inside ``()``/``[]``/``{}`` stay
    intact -- e.g. nested calls ``f(g(a, b), c)`` or template args inside a
    call ``f(std::pair<int, int>)`` (protected by the call parens).
    String/char literals are respected so commas inside ``"a,b"`` do not
    split.
    """
    parts: list[str] = []
    depth_paren = depth_brack = depth_brace = 0
    current: list[str] = []
    in_str = in_char = False
    escape = False
    i = 0
    n = len(text)
    while i < n:
        ch = text[i]
        if escape:
            current.append(ch)
            escape = False
        elif in_str:
            current.append(ch)
            if ch == "\\":
                escape = True
            elif ch == '"':
                in_str = False
        elif in_char:
            current.append(ch)
            if ch == "\\":
                escape = True
            elif ch == "'":
                in_char = False
        elif ch == '"':
            in_str = True
            current.append(ch)
        elif ch == "'":
            in_char = True
            current.append(ch)
        elif ch == "(":
            depth_paren += 1
            current.append(ch)
        elif ch == ")":
            depth_paren -= 1
            current.append(ch)
        elif ch == "[":
            depth_brack += 1
            current.append(ch)
        elif ch == "]":
            depth_brack -= 1
            current.append(ch)
        elif ch == "{":
            depth_brace += 1
            current.append(ch)
        elif ch == "}":
            depth_brace -= 1
            current.append(ch)
        elif ch == sep and depth_paren == 0 and depth_brack == 0 and depth_brace == 0:
            parts.append("".join(current))
            current = []
        else:
            current.append(ch)
        i += 1
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


def _is_single_parenthesized(expr: str) -> bool:
    """True when *expr* is already one outer paren pair: '(...)'."""
    expr = expr.strip()
    if len(expr) < 2 or not expr.startswith("(") or not expr.endswith(")"):
        return False
    depth = 0
    in_str = in_char = False
    escape = False
    for i, ch in enumerate(expr):
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
            if depth == 0 and i != len(expr) - 1:
                return False  # outer '(' closes before the end
    return depth == 0


def _strip_outer_parens(expr: str) -> str:
    """Peel grouping parens that enclose the whole expression.

    ``((CSchedule*)x)`` -> ``(CSchedule*)x``; ``((u8*)self + 0x17C)`` ->
    ``(u8*)self + 0x17C``.  Stops at the first layer that is not a single
    enclosing pair (e.g. ``(CSchedule*)x`` has unbalanced outer parens).
    """
    e = expr.strip()
    while _is_single_parenthesized(e):
        inner = e[1:-1].strip()
        if not inner:
            break
        e = inner
    return e


def _leading_cast_target(expr: str) -> str | None:
    """Pointee-type text when *expr* is a top-level C-style pointer cast.

    ``(CSchedule*)x`` -> ``CSchedule*``; ``(u8*)self + 0x17C`` -> ``u8*``
    (the cast covers only ``self``; the full expression is arithmetic).
    Returns None for non-casts (``f(x)``, ``p->m``, ``a[i]``, ``(a * b)``).
    The inside must look like a pointer TYPE (contains ``*``, no expression
    operators), so ``(a * b)`` (multiplication) never reads as a cast.
    """
    e = expr.strip()
    if not e.startswith("("):
        return None
    close = _find_matching_paren(e, 0)
    if close < 0:
        return None
    inside = e[1:close].strip()
    rest = e[close + 1:].strip()
    if not rest:
        return None  # "(T)" alone casts nothing
    if "*" not in inside:
        return None
    if not re.fullmatch(
        r"(?:const\s+)?[A-Za-z_][A-Za-z0-9_:<>\s]*\*[\*\s]*(?:const)?", inside
    ):
        return None
    return inside


def _type_leaf(type_text: str) -> str:
    """Leaf class name of a (possibly qualified) pointer-type spelling."""
    t = _collapse_ws(type_text)
    t = re.sub(r"\bconst\b", " ", t)
    t = re.sub(r"\s+", "", t).rstrip("*")
    return t.split("::")[-1].split("<")[0]


def _receiver_needs_class_cast(this_expr: str, class_name: str) -> bool:
    """True when a member-ized call receiver needs ``((Class*)...)``.

    Provably ``Class*``-typed (no cast): a bare identifier, ``this`` (even
    under grouping parens), or an expression whose top-level cast already
    targets the class (``((CSchedule*)x)``, ``(CSchedule*)node->mItem``).
    Everything else -- raw pointer arithmetic (``((u8*)self + 0x17C)``),
    ``ptr->member``, ``arr[i]``, call results, ``this->field``, or a cast
    to some OTHER type (``((u8*)p)`` vs target ``Foo``) -- is wrapped.
    (An empty/missing receiver never reaches this helper: the caller
    reviews no-argument calls before splitting.)
    """
    e = this_expr.strip()
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*|this", e):
        return False
    peeled = _strip_outer_parens(e)
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*|this", peeled):
        return False
    cast_to = _leading_cast_target(peeled)
    if cast_to is not None and _type_leaf(cast_to) == _type_leaf(class_name):
        return False
    return True


@dataclass
class SourceHit:
    path: Path
    start_line: int  # 1-based
    end_line: int  # 1-based, inclusive
    kind: str  # 'decl' | 'defn' | 'call' | 'addr' | 'review' | 'demote'
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
                span_start_line = text.count("\n", 0, stmt_start) + 1
                if kind == "decl" and free_rename_new is None:
                    # Member-ization: a bare qualified declaration at file
                    # scope ('Ret Class::method(...);' outside any class) is
                    # MWCC error 10127, so the tool must NOT create one.
                    # File-scope 'extern "C"' import-band declarations are
                    # therefore never rewritten into qualified form; they
                    # become NEEDS-REVIEW items (demote to a comment and rely
                    # on the in-class declaration inserted into the owning
                    # header instead).  Definitions ('{'/':' tail) stay legal
                    # file-scope qualified definitions and are rewritten.
                    hits.append(SourceHit(
                        path, span_start_line, end_line, "review", old, "",
                        note=(f"file-scope declaration of {old} not rewritten: a bare "
                              f"qualified '{target.class_name}::{target.method}(...);' "
                              f"at file scope is MWCC error 10127; declare "
                              f"'{target.method}(...);' inside class "
                              f"{target.class_name} instead (header insert) and demote "
                              f"this import-band line to a comment")))
                    continue
                new_span = _rewrite_decl(old_span, old, target)
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
                if _receiver_needs_class_cast(this_expr, target.class_name):
                    # Option A: not provably Class*-typed (pointer arithmetic,
                    # member/index access, call result, this->field, or a cast
                    # to some OTHER type) -- wrap in an explicit cast to the
                    # target class.  Semantics-preserving (identical pointer
                    # value); without it MWCC rejects e.g.
                    # ((u8*)self + 0x17C)->FillMarkerEntry() with 10141.
                    inner = this_expr if _is_single_parenthesized(this_expr) else f"({this_expr})"
                    recv = f"(({target.class_name}*){inner})"
                elif re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*|this", this_expr):
                    recv = this_expr
                elif _is_single_parenthesized(this_expr):
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
class HeaderEdit:
    """One in-class member declaration to insert into the owning header."""
    path: Path
    class_name: str
    method: str
    decl_line: str  # e.g. 'void Paint(int x, int y);' (no leading indent)
    access: str  # 'public' | 'private' | 'protected'
    already_present: bool = False
    note: str = ""


# Headers that may own a class body (owning header for member-ization).
HEADER_SUFFIXES = {".hpp", ".h"}
# Files searched for a class/struct body: headers plus TU bodies (.cpp).
# Forward declarations ('class X;' / 'struct X;') never count as bodies.
CLASS_BODY_SUFFIXES = {".hpp", ".h", ".cpp"}


def _mask_comments_and_strings(text: str) -> str:
    """Replace comments/string contents with spaces (newlines kept).

    Same length as *text* so regex offsets stay valid against the original.
    Prevents a '// ... class ...' comment glued to a later 'Leaf* self'
    signature from reading as a class head when scanning .cpp files.
    """
    out = list(text)
    n = len(text)
    i = 0
    in_str = in_char = False
    escape = False
    in_line = in_block = False
    while i < n:
        ch = text[i]
        nxt = text[i + 1] if i + 1 < n else ""
        if escape:
            if not in_line and not in_block:
                pass  # escaped char inside string: mask it too
                if in_str or in_char:
                    out[i] = " "
            escape = False
        elif in_line:
            if ch == "\n":
                in_line = False
            else:
                out[i] = " "
        elif in_block:
            if ch == "*" and nxt == "/":
                out[i] = out[i + 1] = " "
                i += 1
                in_block = False
            elif ch != "\n":
                out[i] = " "
        elif in_str:
            if ch == "\\":
                escape = True
                out[i] = " "
            elif ch == '"':
                in_str = False
                out[i] = " "
            elif ch != "\n":
                out[i] = " "
        elif in_char:
            if ch == "\\":
                escape = True
                out[i] = " "
            elif ch == "'":
                in_char = False
                out[i] = " "
            elif ch != "\n":
                out[i] = " "
        elif ch == "/" and nxt == "/":
            out[i] = out[i + 1] = " "
            i += 1
            in_line = True
        elif ch == "/" and nxt == "*":
            out[i] = out[i + 1] = " "
            i += 1
            in_block = True
        elif ch == '"':
            in_str = True
            out[i] = " "
        elif ch == "'":
            in_char = True
            out[i] = " "
        i += 1
    return "".join(out)


def _find_class_bodies(project_root: Path, class_name: str) -> list[tuple[Path, int, int, str, str]]:
    """Locate class/struct bodies for *class_name* in headers and .cpp files.

    Returns a list of (path, brace_open, brace_close, keyword, body_text)
    for each definition 'class Leaf { ... };' / 'struct Leaf { ... };'
    (forward declarations 'class Leaf;' / 'struct Leaf;' are ignored).
    Headers (.hpp/.h) and TU bodies (.cpp) under src/ or libs/ are
    searched; the leaf name (after '::') is matched so 'cf::Vision'
    finds 'class Vision'.  Results are sorted by path for determinism.
    """
    leaf = class_name.split("::")[-1]
    found: list[tuple[Path, int, int, str, str]] = []
    # Parens are excluded from the middle: a function signature such as
    # 'func(CColiObject* self, ...)' following a '// ... class ...' comment
    # must never read as 'class ... CColiObject'.  Real class heads
    # ('class Leaf', 'struct Leaf', 'class Leaf : public Base') never
    # contain '(' or ')' between the keyword and the name.
    name_re = re.compile(
        r"\b(class|struct)\b[^;{}()]*?\b" + re.escape(leaf) + r"\b"
    )
    for path in _iter_source_files(project_root):
        if path.suffix not in CLASS_BODY_SUFFIXES:
            continue
        try:
            text = path.read_text(encoding="utf-8")
        except (OSError, UnicodeDecodeError):
            continue
        if leaf not in text:
            continue
        masked = _mask_comments_and_strings(text)
        for m in name_re.finditer(masked):
            # Skip forward declarations: next significant char is ';'.
            j = m.end()
            while j < len(text) and text[j] in " \t\n\r":
                j += 1
            # Skip inheritance lists / macro tails until '{' or ';'.
            k = j
            while k < len(text) and text[k] not in "{;":
                k += 1
            if k >= len(text) or text[k] != "{":
                continue  # forward decl or no body
            close = _find_matching_brace(text, k)
            if close < 0:
                continue
            keyword = m.group(1)
            found.append((path, k, close, keyword, text[k: close + 1]))
    found.sort(key=lambda b: str(b[0]))
    return found


def _select_class_body(
    bodies: list[tuple[Path, int, int, str, str]],
    defn_path: Path | None,
    class_name: str,
    method: str,
) -> tuple[Path, int, int, str, str]:
    """Pick the single owning class body from *bodies* (headers + .cpp).

    Preference order: (a) the body in the SAME file as the function
    definition being renamed, (b) a single header (.hpp/.h) body,
    (c) a unique .cpp body (even in a different TU).  A lone body of any
    kind wins immediately.  When 2+ real bodies remain and no unique
    preference applies, raises :class:`FnRenameRefusal` loudly instead of
    guessing.
    """
    if len(bodies) == 1:
        return bodies[0]
    # (a) same-file body wins when unique.
    if defn_path is not None:
        same = [b for b in bodies if b[0] == defn_path]
        if len(same) == 1:
            return same[0]
        if len(same) > 1:
            where = ", ".join(sorted(str(p) for p, _a, _b, _c, _d in bodies))
            raise FnRenameRefusal(
                f"refusing: cannot insert member declaration for "
                f"'{class_name}::{method}': ambiguous class "
                f"bodies for '{class_name}' in {where}; "
                f"{len(same)} bodies share the definition file "
                f"{_rel(Path('.'), defn_path) if defn_path is not None else defn_path} "
                f"-- cannot guess the owning body (expected exactly one)"
            )
    # (b) a single header body beats any number of .cpp bodies.
    headers = [b for b in bodies if b[0].suffix in HEADER_SUFFIXES]
    if len(headers) == 1:
        return headers[0]
    if len(headers) > 1:
        where = ", ".join(sorted(str(p) for p, _a, _b, _c, _d in bodies))
        raise FnRenameRefusal(
            f"refusing: cannot insert member declaration for "
            f"'{class_name}::{method}': ambiguous class "
            f"bodies for '{class_name}' in {where}; cannot guess "
            f"the owning header (expected exactly one)"
        )
    # (c) no same-file body and no header body: only a unique .cpp body
    # may be used (even in a different TU).  Anything else is ambiguous.
    where = ", ".join(sorted(str(p) for p, _a, _b, _c, _d in bodies))
    raise FnRenameRefusal(
        f"refusing: cannot insert member declaration for "
        f"'{class_name}::{method}': ambiguous class "
        f"bodies for '{class_name}' in {where}; cannot guess "
        f"the owning body (expected exactly one unambiguous .cpp body)"
    )


_METHOD_DECL_RE = re.compile(
    r"^[ \t]*(?:virtual\s+|static\s+|inline\s+|explicit\s+|constexpr\s+)*"
    r"[A-Za-z_][A-Za-z0-9_:<>*&~]*\s+~?[A-Za-z_][A-Za-z0-9_]*\s*\([^;{}]*\)"
    r"\s*(?:const\s*)?(?:=\s*0\s*)?;\s*(?://.*)?$",
    re.MULTILINE,
)
_ACCESS_RE = re.compile(r"^[ \t]*(public|private|protected)\s*:\s*(?://.*)?$", re.MULTILINE)


def _choose_insert_section(
    body_text: str, class_keyword: str
) -> tuple[str, int, str]:
    """Choose the access section for the new member declaration.

    Returns (access, insert_offset_in_body, indent).  The section containing
    the most method-like declarations wins (preferring 'public' on ties);
    with no methods, the existing 'public' section wins, else the first
    section, else the implicit access ('public' for struct, 'private' for
    class).  The offset is the end of the chosen section (just before the
    next access label or the closing brace).
    """
    labels: list[tuple[str, int, int]] = []  # (access, line_start, line_end)
    for m in _ACCESS_RE.finditer(body_text):
        line_start = body_text.rfind("\n", 0, m.start()) + 1
        labels.append((m.group(1), line_start, m.end()))
    implicit = "public" if class_keyword == "struct" else "private"
    # Build sections: [(access, start, end)].
    sections: list[tuple[str, int, int]] = []
    if not labels:
        sections = [(implicit, 0, len(body_text))]
    else:
        first_start = labels[0][1]
        if first_start > 0:
            sections.append((implicit, 0, first_start))
        for idx, (acc, _s, e) in enumerate(labels):
            end = labels[idx + 1][1] if idx + 1 < len(labels) else len(body_text)
            sections.append((acc, e, end))
    # Count method-like declarations per section.
    counts: list[int] = []
    last_indents: list[str] = []
    for _acc, s, e in sections:
        seg = body_text[s:e]
        found = list(_METHOD_DECL_RE.finditer(seg))
        counts.append(len(found))
        indent = ""
        if found:
            line = found[-1].group(0)
            indent = line[: len(line) - len(line.lstrip())]
        last_indents.append(indent)
    best = -1
    if any(c > 0 for c in counts):
        top = max(counts)
        tied = [i for i, c in enumerate(counts) if c == top]
        if len(tied) == 1:
            best = tied[0]
        else:
            for pref in ("public", "protected", "private", implicit):
                for i in tied:
                    if sections[i][0] == pref:
                        best = i
                        break
                if best >= 0:
                    break
            if best < 0:
                best = tied[0]
    else:
        for i, (acc, _s, _e) in enumerate(sections):
            if acc == "public":
                best = i
                break
        if best < 0:
            best = 0
    access, _s, e = sections[best]
    # Indent: last method in the chosen section, else first method in the
    # body, else 4 spaces.
    indent = last_indents[best]
    if not indent:
        for pat in _METHOD_DECL_RE.finditer(body_text):
            line = pat.group(0)
            indent = line[: len(line) - len(line.lstrip())]
            if indent:
                break
    if not indent:
        indent = "    "
    # Insert at the end of the chosen section's content: before the next
    # access label, or (for the last section) before the closing '}'.
    s_best, e_best = sections[best][1], sections[best][2]
    is_last = (e_best == len(body_text))
    if is_last and body_text.endswith("}"):
        # Exclude the closing brace; insert into the inner content.
        inner = body_text[s_best: len(body_text) - 1]
        insert_in_body = s_best + len(inner.rstrip(" \t\n\r"))
    else:
        seg = body_text[s_best: e_best]
        insert_in_body = s_best + len(seg.rstrip(" \t\n\r"))
    return access, insert_in_body, indent


def _inclass_decl_from_defn(defn_hit: SourceHit, target: FunctionTarget) -> str:
    """Derive 'Ret method(params);' from a rewritten definition header."""
    header = defn_hit.new_span.strip()
    marker = f"{target.class_name}::"
    if marker not in header:
        raise FnRenameRefusal(
            f"refusing: cannot derive in-class declaration from {header!r} "
            f"(missing {marker!r})"
        )
    return header.replace(marker, "", 1).rstrip() + ";"


def _is_inclass_decl_present(body_text: str, decl_line: str) -> bool:
    """Idempotency check: is *decl_line* already in the class body?"""
    norm_body = _collapse_ws(body_text)
    norm_decl = _collapse_ws(decl_line)
    if norm_decl in norm_body:
        return True
    # Fallback: same method name + same normalised parameter list.
    m = re.match(r".*?\b([A-Za-z_][A-Za-z0-9_]*)\s*\((.*)\)\s*;\s*$", norm_decl, re.DOTALL)
    if not m:
        return False
    method, params = m.group(1), _collapse_ws(m.group(2))
    pat = re.compile(r"\b" + re.escape(method) + r"\s*\(" + re.escape(params) + r"\s*\)\s*;")
    return bool(pat.search(norm_body))


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
    header_edits: list[HeaderEdit] = field(default_factory=list)
    header_notes: list[str] = field(default_factory=list)
    no_header_insert: bool = False
    demotions: list[SourceHit] = field(default_factory=list)
    demote_import_band: bool = False

    @property
    def source_hits(self) -> list[SourceHit]:
        return self.decls + self.defns + self.calls + self.addr_refs + self.demotions


def _read_symbols_name_lines(project_root: Path, region: str) -> list[str] | None:
    path = project_root / "config" / region / "symbols.txt"
    if not path.is_file():
        return None
    return path.read_text(encoding="utf-8").splitlines()


# ---------------------------------------------------------------------------
# Import-band demotion (--demote-import-band)
# ---------------------------------------------------------------------------
#
# Flag decision: opt-in, default OFF for both `plan` and `apply`.
#
# * `plan` keeps the default OFF so the 10127 file-scope review stays visible
#   during triage.  Auto-silencing by default would hide the mechanical
#   transform inside bulk `extc scan` waves and make multi-symbol or
#   missing-header cases easy to miss.
# * `apply` also defaults OFF so automated/coop runs never silently comment
#   out source lines.  The operator first sees the NEEDS-REVIEW plan, then
#   consciously re-runs with `--demote-import-band` to accept the line-neutral
#   demotion.  This keeps backward compatibility: existing
#   `--allow-review` flows (leave the import-band line untouched) keep working.
# * The alternative (default-on with `--no-demote-import-band`) was rejected
#   because it would change every existing member-ization plan/apply to
#   rewrite source by default, surprising bulk-run operators and obscuring
#   the safety refusals below.


def _demote_comment(old: str, target: FunctionTarget) -> str:
    """One-line demotion marker (code-free, so re-scans skip it)."""
    return (
        f"// (memberized: {old} -> {target.class_name}::{target.method}; "
        f"declaration moved into class {target.class_name})"
    )


def _is_import_band_review(review: SourceHit, old: str) -> bool:
    """True when *review* is the TU-local extern "C" import-band decl of *old*."""
    return (
        review.kind == "review"
        and f"file-scope declaration of {old} not rewritten" in (review.note or "")
        and "10127" in (review.note or "")
    )


def _line_offsets(text: str) -> list[int]:
    """Start offset of every line in *text* (0-based)."""
    offsets = [0]
    for m in re.finditer(r"\n", text):
        offsets.append(m.end())
    return offsets


def _build_demotion_for_review(
    project_root: Path,
    review: SourceHit,
    old: str,
    target: FunctionTarget,
) -> SourceHit:
    """Build the line-neutral demotion edit for one import-band review.

    The replacement covers whole source lines (start line through the
    terminating `;` line) so `-ipa file` line numbers never shift.  Raises
    :class:`FnRenameRefusal` when the line mentions other symbols too
    (multi-declaration line) instead of guessing.
    """
    path = review.path
    try:
        text = path.read_text(encoding="utf-8")
    except (OSError, UnicodeDecodeError) as exc:
        raise FnRenameRefusal(
            f"refusing: cannot read {path} for --demote-import-band ({exc})"
        ) from exc
    offsets = _line_offsets(text)
    lines = text.splitlines()
    if review.start_line < 1 or review.start_line > len(lines):
        raise FnRenameRefusal(
            f"refusing: import-band review line {review.start_line} out of range "
            f"in {path} (file changed since scan?)"
        )
    token_re = re.compile(r"(?<![A-Za-z0-9_:~])" + re.escape(old) + r"(?![A-Za-z0-9_])")
    candidate: re.Match[str] | None = None
    for m in token_re.finditer(text):
        ln = text.count("\n", 0, m.start()) + 1
        if review.start_line <= ln <= review.end_line:
            after = text[m.end():].lstrip()
            if after.startswith("("):
                candidate = m
                break
    if candidate is None:
        raise FnRenameRefusal(
            f"refusing: cannot relocate import-band declaration of {old!r} "
            f"in {path}:{review.start_line} for --demote-import-band "
            f"(file changed since scan?)"
        )
    open_idx = text.find("(", candidate.end())
    if open_idx < 0:
        raise FnRenameRefusal(
            f"refusing: cannot relocate import-band declaration of {old!r} "
            f"in {path}:{review.start_line} (no opening paren)"
        )
    close_idx = _find_matching_paren(text, open_idx)
    if close_idx < 0:
        raise FnRenameRefusal(
            f"refusing: unbalanced parentheses in import-band declaration of "
            f"{old!r} in {path}:{review.start_line}"
        )
    semi_idx = text.find(";", close_idx + 1)
    if semi_idx < 0:
        raise FnRenameRefusal(
            f"refusing: cannot find terminating ';' for import-band declaration "
            f"of {old!r} in {path}:{review.start_line}"
        )
    # Gap between ')' and ';' must be whitespace-only.  Anything else (a
    # top-level comma + another declarator, e.g. `void Old(...), Other(...);`)
    # means this line mentions other symbols too -> refuse loudly.
    gap = text[close_idx + 1: semi_idx]
    if _collapse_ws(gap) != "":
        raise FnRenameRefusal(
            f"refusing: import-band line for {old!r} in {path} mentions other "
            f"symbols too (unexpected text between ')' and ';': {gap.strip()[:80]!r}); "
            f"demote manually"
        )
    # Statement start (same rule as the classifier): after the last
    # ';'/'{'/'}' on the opening line, else the line start.
    bol = text.rfind("\n", 0, candidate.start()) + 1
    seg = text[bol: candidate.start()]
    stmt_start = bol
    for term in (";", "{", "}"):
        pos = seg.rfind(term)
        if pos >= 0:
            stmt_start = max(stmt_start, bol + pos + 1)
    access = re.search(
        r"\b(public|private|protected)\s*:\s*$", text[stmt_start: candidate.start()]
    )
    if access:
        stmt_start += access.end()
    start_line_no = text.count("\n", 0, stmt_start) + 1
    end_line_no = text.count("\n", 0, semi_idx) + 1
    # Whole-line span: start of the first line through end of the ';' line
    # (excluding the trailing newline so the replacement stays line-neutral).
    line_start_off = offsets[start_line_no - 1]
    if end_line_no - 1 + 1 < len(offsets):
        line_end_off = offsets[end_line_no] - 1  # drop the '\n'
        # Handle \r\n: strip a trailing \r if present.
        if line_end_off > line_start_off and text[line_end_off - 1] == "\r":
            line_end_off -= 1
    else:
        line_end_off = len(text)
        if line_end_off > line_start_off and text.endswith("\r"):
            line_end_off -= 1
    old_span = text[line_start_off: line_end_off]
    # Safety: prefix (line start .. stmt start) must be whitespace/comment
    # only.  Anything else means other code shares the first line, e.g.
    # `void Foo(); void Old(...);` -- the line mentions other symbols too.
    prefix = text[line_start_off: stmt_start]
    if _collapse_ws(_code_part(prefix)) != "":
        raise FnRenameRefusal(
            f"refusing: import-band line for {old!r} in {path}:{start_line_no} "
            f"shares its line with other code ({prefix.strip()[:80]!r}); demote manually"
        )
    # Safety: suffix (after ';' .. end of line) must be whitespace/comment
    # only, e.g. `void Old(...); void Other(...);` on one line.
    suffix = text[semi_idx + 1: line_end_off]
    if _collapse_ws(_code_part(suffix)) != "":
        raise FnRenameRefusal(
            f"refusing: import-band line for {old!r} in {path}:{end_line_no} "
            f"shares its line with other code ({suffix.strip()[:80]!r}); demote manually"
        )
    # Idempotency guard: an already-demoted line is comment-only, so the
    # classifier never emits it as a review.  If we ever get here with a
    # demotion marker already present, refuse rather than stacking comments.
    if "(memberized:" in old_span:
        raise FnRenameRefusal(
            f"refusing: import-band line for {old!r} in {path}:{start_line_no} "
            f"is already demoted; refusing to stack comments"
        )
    n_lines = end_line_no - start_line_no + 1
    first = lines[start_line_no - 1]
    indent_m = re.match(r"[ \t]*", first)
    indent = indent_m.group(0) if indent_m else ""
    base = indent + _demote_comment(old, target)
    # Preserve a trailing `//` comment from the ';' line (still one line).
    trailing_comment = ""
    semi_line = lines[end_line_no - 1]
    semi_col_in_line = semi_idx - offsets[end_line_no - 1]
    comment_idx = semi_line.find("//", max(0, semi_col_in_line))
    if comment_idx >= 0:
        trailing_comment = semi_line[comment_idx:].rstrip()
        if trailing_comment and "(memberized:" not in trailing_comment:
            base = base + " " + trailing_comment
    if n_lines == 1:
        new_span = base
    else:
        fillers = [indent + "//"] * (n_lines - 1)
        new_span = base + "\n" + "\n".join(fillers)
    assert new_span.count("\n") == old_span.count("\n"), (
        "demotion must preserve the line count"
    )
    return SourceHit(
        path=path,
        start_line=start_line_no,
        end_line=end_line_no,
        kind="demote",
        old_span=old_span,
        new_span=new_span,
        note=(
            f"demoted import-band declaration of {old} "
            f"(in-class {target.class_name}::{target.method} supplied by header insert)"
        ),
    )


def plan_function_rename(
    project_root: Path,
    old: str,
    new_spec: str,
    regions: list[str] | None = None,
    no_header_insert: bool = False,
    demote_import_band: bool = False,
) -> FnPlan:
    """Build a complete deterministic rename plan (dry-run only, no writes).

    Raises :class:`FnRenameRefusal` instead of guessing whenever the new
    symbol cannot be computed, exactly one definition cannot be found, or
    (for member-ization without ``no_header_insert``) no single owning
    class body can be located for the in-class declaration.

    When ``demote_import_band`` is True, file-scope TU-local ``extern "C"``
    import-band declarations of *old* are auto-resolved into line-neutral
    ``// (memberized: ...)`` comment edits -- but only when *every* review
    is such a declaration and a header insert supplies the in-class
    declaration.  Any other shape (multi-declaration line, missing header
    body, extra unrelated review) keeps the plan NEEDS-REVIEW or refuses
    loudly; see the flag-rationale note above ``_demote_comment``.
    """
    if regions is None:
        regions = list(REGIONS)
    else:
        # Explicit region list: the rename must never touch a region the
        # caller did not ask for (jp is currently corrupt; US pipeline only).
        regions = list(regions)
        if not regions:
            raise FnRenameRefusal("refusing: empty region list (nothing to rewrite)")
        for r in regions:
            if r not in REGIONS:
                raise FnRenameRefusal(
                    f"refusing: unknown region {r!r} (expected one of {', '.join(REGIONS)})"
                )
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
        # --- in-class member declaration (member-ization only) ---
        header_edits: list[HeaderEdit] = []
        header_notes: list[str] = []
        if not no_header_insert:
            assert target is not None and target.params is not None
            decl_line = _inclass_decl_from_defn(defns[0], target)
            bodies = _find_class_bodies(project_root, target.class_name)
            if len(bodies) == 0:
                raise FnRenameRefusal(
                    f"refusing: cannot insert member declaration for "
                    f"'{target.class_name}::{target.method}': found no "
                    f"'class {target.class_name} {{ ... }};' body in any header "
                    f"or .cpp (.hpp/.h/.cpp under src/ or libs/); cannot guess "
                    f"the owning file -- declare '{decl_line}' in the owning "
                    f"class body manually or re-run with --no-header-insert"
                )
            hpath, bopen, bclose, keyword, body_text = _select_class_body(
                bodies, defns[0].path, target.class_name, target.method
            )
            try:
                htext = hpath.read_text(encoding="utf-8")
            except (OSError, UnicodeDecodeError) as exc:
                raise FnRenameRefusal(
                    f"refusing: cannot read owning file {hpath} ({exc})"
                ) from exc
            # Re-locate the body in the freshly read text (same file, same
            # offsets unless the file changed between scans).
            access, insert_in_body, indent = _choose_insert_section(body_text, keyword)
            if _is_inclass_decl_present(body_text, decl_line):
                header_edits.append(HeaderEdit(
                    path=hpath, class_name=target.class_name, method=target.method,
                    decl_line=decl_line, access=access, already_present=True,
                    note=f"already declared in {_rel(project_root, hpath)} ({access}); no insert",
                ))
                header_notes.append(
                    f"in-class declaration already present in "
                    f"{_rel(project_root, hpath)} ({access}); no insert"
                )
            else:
                header_edits.append(HeaderEdit(
                    path=hpath, class_name=target.class_name, method=target.method,
                    decl_line=decl_line, access=access, already_present=False,
                    note=f"insert '{decl_line}' into {access} section of "
                         f"{_rel(project_root, hpath)}",
                ))
        else:
            header_edits = []
            header_notes = ["header insert skipped (--no-header-insert)"]

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
        header_edits: list[HeaderEdit] = []
        header_notes: list[str] = []
    else:
        assert target is not None
        plan_target = target
        # header_edits/header_notes were computed in the member-ization
        # branch above (strict unless --no-header-insert).
    # --- import-band auto-demotion (opt-in, --demote-import-band) ---
    demotions: list[SourceHit] = []
    if demote_import_band and not free_rename:
        assert target is not None
        if no_header_insert or not header_edits:
            if reviews and any(_is_import_band_review(r, old) for r in reviews):
                raise FnRenameRefusal(
                    f"refusing: --demote-import-band requires the in-class "
                    f"declaration for '{target.class_name}::{target.method}' but "
                    f"the header insert is unavailable (no single class body; "
                    f"re-run without --no-header-insert or declare it manually)"
                )
        elif reviews and all(_is_import_band_review(r, old) for r in reviews):
            # ONLY import-band decl reviews remain: demote each line-neutrally.
            # Any multi-declaration line raises loudly inside the builder.
            built: list[SourceHit] = []
            for rev in reviews:
                built.append(_build_demotion_for_review(project_root, rev, old, target))
            demotions = built
            reviews = []
        # Else: an additional unrelated review exists -> keep NEEDS-REVIEW.
        # (No partial demotion; the operator must triage the other item.)
    elif demote_import_band and free_rename:
        # Free renames have no member declaration to move into a class;
        # the flag is a no-op (kept for CLI uniformity).
        pass
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
        header_edits=header_edits,
        header_notes=header_notes,
        no_header_insert=no_header_insert,
        demotions=demotions,
        demote_import_band=demote_import_band,
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
    if not plan.free_rename:
        lines.append(f"  in-class declarations: {len(plan.header_edits)}")
        for he in plan.header_edits:
            state = "already present" if he.already_present else f"insert into {he.access}"
            lines.append(f"    {_rel(project_root, he.path)} [{he.class_name}::{he.method}]: {state}")
            lines.append(f"      + {he.decl_line.strip()[:160]}")
            if he.note:
                lines.append(f"        ({he.note[:160]})")
        for note in plan.header_notes:
            lines.append(f"    (header: {note[:160]})")
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
    if plan.demotions:
        lines.append(f"  demoted import-band declarations: {len(plan.demotions)}")
        for hit in plan.demotions:
            lines.append(f"    {_rel(project_root, hit.path)}:{hit.start_line}-{hit.end_line}: {hit.note[:160]}")
            if verbose:
                lines.append(f"      - {hit.old_span[:200]}")
                lines.append(f"      + {hit.new_span[:200]}")
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


def _apply_header_edits(
    project_root: Path,
    plan: FnPlan,
    stamp: str,
    log: list[str],
) -> None:
    """Insert in-class member declarations (idempotent, no duplicates)."""
    for he in plan.header_edits:
        if he.already_present:
            log.append(f"header already declares {he.class_name}::{he.method} "
                       f"in {_rel(project_root, he.path)} ({he.access}); no insert")
            continue
        try:
            file_text = he.path.read_text(encoding="utf-8")
        except (OSError, UnicodeDecodeError) as exc:
            raise FnRenameRefusal(f"refusing to apply: cannot read {he.path} ({exc})") from exc
        bodies = _find_class_bodies(project_root, he.class_name)
        cur_match: tuple[int, int, str, str] | None = None
        for p, co, _cc, kw, bd in bodies:
            if p == he.path:
                cur_match = (co, _cc, kw, bd)
                break
        if cur_match is None:
            raise FnRenameRefusal(
                f"refusing to apply: owning file {_rel(project_root, he.path)} no longer "
                f"declares 'class {he.class_name} {{ ... }};' (file changed since planning?)"
            )
        co, _cc, kw, bd = cur_match
        _access, ins, indent = _choose_insert_section(bd, kw)
        # Idempotency against the CURRENT file (plan may be stale; the same
        # header may also have been rewritten above).
        if _is_inclass_decl_present(bd, he.decl_line):
            log.append(f"header already declares {he.class_name}::{he.method} "
                       f"in {_rel(project_root, he.path)} on re-check; no insert")
            continue
        backup = _backup_path(stamp, project_root, he.path)
        insert_at = co + ins
        # insert_at sits at the end of the stripped section (right after the
        # last ';'); the suffix starts with the section's trailing newlines.
        new_text = file_text[:insert_at] + "\n" + indent + he.decl_line.rstrip() + file_text[insert_at:]
        # Atomic: temp file in the same directory + os.replace, so a
        # crash mid-write can never truncate the owning header.
        atomic_write_text(he.path, new_text)
        log.append(f"updated {_rel(project_root, he.path)} "
                   f"(in-class {he.class_name}::{he.method} into {he.access}; backup {backup})")


def apply_function_rename(
    project_root: Path,
    old: str,
    new_spec: str,
    regions: list[str] | None = None,
    dry_run: bool = True,
    allow_review: bool = False,
    no_header_insert: bool = False,
    demote_import_band: bool = False,
) -> tuple[FnPlan, list[str]]:
    """Apply a function rename.  Returns (plan, log_lines).

    * Always builds the plan first (refusals propagate, nothing written).
    * Refuses to write when configure.py/splits.txt mention the symbol.
    * Refuses to write when NEEDS-REVIEW items exist unless *allow_review*.
      Demoted import-band lines (``--demote-import-band``) are resolved
      edits, not reviews, so a plan whose only reviews were demotable
      needs neither ``--allow-review`` nor leftover manual demotion.
    * With *dry_run* True: no writes, log says what would change.
    * Otherwise: backs every touched file up under /tmp/symbol_fn_rename/
      before writing.
    * Member-ization also inserts the in-class declaration unless
      *no_header_insert* (``--no-header-insert``).
    """
    plan = plan_function_rename(project_root, old, new_spec, regions,
                                no_header_insert=no_header_insert,
                                demote_import_band=demote_import_band)
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
        # Atomic (incident hardening): temp file in the SAME directory +
        # os.replace, so an interrupted write can never truncate symbols.txt.
        atomic_write_text(sym_path, "".join(lines))
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
        # Atomic: temp file in the same directory + os.replace.
        atomic_write_text(path, updated)
        log.append(f"updated {_rel(project_root, path)} ({len(hits)} edit(s); backup {backup})")
    if not plan.free_rename and plan.body_self_count and not saw_defn:
        raise FnRenameRefusal(
            "refusing to apply: definition anchor missing for body fix-up"
        )

    # --- in-class header declaration (member-ization only) ---
    if not plan.free_rename and not plan.no_header_insert:
        _apply_header_edits(project_root, plan, stamp, log)

    log.append("")
    log.append("Next: python configure.py && ninja")
    log.append("Then: python tools/coop/run.py diff <unit> --symbol " + (plan.new_mangled or ""))
    return plan, log


def _locate_spans(
    text: str, hits: list[SourceHit], old: str
) -> list[tuple[SourceHit, tuple[int, int]]]:
    """Map each hit's old_span back to offsets in the current file text.

    Anchored on the hit's start line: a definition header such as
    ``void Old(T* self, ...)`` also appears as a substring of its own
    ``extern "C"`` import-band forward declaration two lines above, so a
    naive forward search from the cursor can latch onto the wrong line
    (notably when the owning class body lives in the same .cpp and the
    file holds no earlier hit to advance the cursor past the import band).
    Candidates whose line does not match ``hit.start_line`` are skipped.
    """
    spans: list[tuple[SourceHit, tuple[int, int]]] = []
    cursor = 0
    for hit in sorted(hits, key=lambda h: h.start_line):
        idx = text.find(hit.old_span, cursor)
        # Skip occurrences on the wrong line (e.g. the import-band decl
        # sharing the definition header as a substring).
        while idx >= 0 and text.count("\n", 0, idx) + 1 != hit.start_line:
            idx = text.find(hit.old_span, idx + 1)
        if idx < 0:
            # Fall back to a full-text search for the right line (spans may
            # repeat, e.g. '&Old'; cursor may already sit past the hit).
            search_from = 0
            while True:
                cand = text.find(hit.old_span, search_from)
                if cand < 0:
                    break
                if text.count("\n", 0, cand) + 1 == hit.start_line:
                    idx = cand
                    break
                search_from = cand + 1
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
    parser.add_argument("--no-header-insert", action="store_true",
                        help="skip the in-class member-declaration insert (plan only)")
    parser.add_argument("--demote-import-band", action="store_true",
                        help=("auto-resolve TU-local extern \"C\" import-band decls of the "
                              "old symbol into line-neutral '// (memberized: ...)' comments "
                              "when (and only when) every review is such a decl and a "
                              "header insert supplies the in-class declaration; "
                              "default OFF so triage still surfaces the 10127 review"))
    _add_common(parser)
    args = parser.parse_args(argv)
    regions = [args.region] if args.region else list(REGIONS)
    try:
        plan = plan_function_rename(project_root, args.old, args.new_spec, regions,
                                    no_header_insert=args.no_header_insert,
                                    demote_import_band=args.demote_import_band)
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
    parser.add_argument("--no-header-insert", action="store_true",
                        help="skip the in-class member-declaration insert")
    parser.add_argument("--demote-import-band", action="store_true",
                        help=("auto-resolve TU-local extern \"C\" import-band decls of the "
                              "old symbol into line-neutral '// (memberized: ...)' comments; "
                              "opt-in (default OFF) so bulk runs never silently comment lines"))
    _add_common(parser)
    args = parser.parse_args(argv)
    regions = [args.region] if args.region else list(REGIONS)
    try:
        _plan, log = apply_function_rename(
            project_root, args.old, args.new_spec, regions,
            dry_run=args.dry_run, allow_review=args.allow_review,
            no_header_insert=args.no_header_insert,
            demote_import_band=args.demote_import_band,
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

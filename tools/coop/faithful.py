#!/usr/bin/env python3
"""Per-unit source-faithfulness status and shrink-only gate (``run.py faithful``).

Byte matching says a unit compiles to the retail bytes.  It does not say the
source looks like what the original programmers wrote: a member function
written as an ``extern "C"`` free function over raw offsets scores the same as
the real method.  This tool measures the second property.

A unit is **faithful** when all of these are zero:

  label         ``lbl_<region>_XXXXXXXX`` address labels used as identifiers
  mangled_c     ``extern "C"`` declarations/definitions of mangled C++ names
                (member functions written as C free functions)
  section       ``__declspec(section ...)`` hand-placed data
  offset_cast   ``(u8*)p + 0x..`` raw offset arithmetic
  view_struct   offset "view" structs (``...View``, ``...At3E9C``, ``...Off``);
                classes retail names via a ctor/dtor/vtable symbol (``CView``) are exempt.
                A whole-file structural pass also counts the *definitions* of
                pad overlays (byte-array padding positioning a few fields at
                large offsets) and pad-positioned union views, which the name
                regex cannot see.  No ``// faithful-ok:`` marker exempts them.
                One hit per definition, attributed to the source/header that
                defines it exactly like the regex hits.
  rule          the unit has a tools/postprocess_reloc_names.py UNIT_RULES entry
  rtti_off      configure.py compiles the unit with ``-RTTI off`` (retail has
                RTTI; the flag exists to make room for hand-built typeinfo)
  foreign_data  retail data symbols that relocation evidence assigns to this
                unit but splits.txt gives to another unit (or the reverse);
                see tools/coop/section_owners.py

Counts cover the unit's source file plus the header with the same stem, with
comments stripped.  Headers under ``src/``, ``include/`` and ``libs/monolib/``
that no unit selects as its paired header are counted too, as pseudo-units
named ``hdr/<repo-relative path>``; this keeps side headers (*Imports.hpp,
*Api.hpp, ...) from vanishing from the census.  SDK/vendor headers under the
other ``libs/`` trees and generated ``*.ctx.c`` files are not enumerated.
Naming debt that does not change the program's shape (``field_0x..``,
``Unk...``, ``func_XXXXXXXX``) is reported as ``soft`` and does not gate.

Soft report-only gaps (never scored):

  volatile      report-only census of *every* volatile spelling left in the
                source: field/object declarations (``volatile T x;``),
                pointer declarations (``volatile T* p``, ``T* volatile* p``)
                and suffix casts (``(T volatile*)&obj``) the hard counter
                misses.  SDK hardware registers (WGPIPE/GXWGFifo, absolute
                ``0xCC00....`` addresses) and ``// volatile-ok:`` lines are
                excluded.  The hard ``volatile`` score is unchanged.
  blob_offset   named data blob / string pool read at a fixed offset
                (``lbl_eu_XXXXXXXX + 0x..``, ``&sPool[0x..]``); the original
                source spelled the string as a literal.
  sdata2_extern scalar float/double constant declared ``extern`` instead of
                being written as a source literal.
  type_pun_union plain unions of differently typed scalar/array arms (e.g.
                ``union { u32 w[2]; u8 b[8]; }``).  One hit per definition.
                These are compiler-shape tricks but have no offset-padding
                debt, so they are report-only; pad-positioned union views are
                hard ``view_struct`` hits instead.  Never ``faithful-ok``-able.
  packed_string string literal with an explicit NUL escape used to pad or
                pack data: ``"thread\0"``, ``"x\0\0\0"``, ``"a\0b"``.
                Adjacent literals are treated as one expression (one hit), and
                single-character ``'\0'`` / NUL-free strings are not counted.
                Never ``faithful-ok``-able.

Hard smells (scored):

  noinline      ``__declspec(noinline)`` / ``__attribute__((never_inline))`` /
                ``DECOMP_DONT_INLINE`` / ``#pragma dont_inline`` /
                ``#pragma auto_inline off`` forcing a call the original source
                did not need to force.  Check the real causes first: wrong TU
                boundary, definition order, unit-wide inline flags.  A use that
                survives those checks is exempt when its line carries
                ``// noinline-ok: <what was tried>``.
  self_alias    a reference bound to a plain local declared on the immediately
                preceding line (``T storage; T& alias = storage;``).  Neither
                spelling exists in retail source: it only forces MWCC to home
                a value in the stack frame.  A real out-param / real volatile
                object is not a self-alias and is not counted here.
  decl_shield   ``#define name name_xxx`` / ``#define name name_hidden`` renames
                that hide a conflicting declaration (or alias a static member to
                its mangled name).  Fix the declaration drift instead: one
                canonical declaration, shared by every TU.
  volatile      ``(volatile T*)&obj`` re-reads of a non-volatile object to defeat
                CSE / force a load (hardware FIFO writes such as ``&WGPIPE`` are
                not counted).  If the retail object really is volatile, declare it
                volatile at its definition; a use that must stay is exempt with
                ``// volatile-ok: <why the object is volatile>``.

The gate compares against ``docs/evidence/faithful_baseline.json``: a unit's
score may not rise and a faithful unit may not stop being faithful.
``--ratchet`` lowers the baseline (shrink-only).  A header pseudo-unit that
is absent from the baseline is warn-only (``unbaselined header``);
``--seed-headers`` adds just those missing ``hdr/`` keys, leaving every
existing baseline entry untouched.

``--finish-line`` (also available as ``run.py faithful --finish-line``) prints
the PLAN_TO_DONE §1 table: current count / target / status per category, plus
the separate ``// faithful-ok:`` exception list and ``// name-guess:`` count.
It uses its own marker-aware counters and never reads or writes the baseline.
Structural overlay hits (hard ``view_struct`` definitions and soft
``type_pun_union``) are added after the per-line marker exemptions, so neither
``// faithful-ok:`` nor ``// name-guess:`` can hide them.
"""
from __future__ import annotations

import argparse
import ast
import collections
import functools
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BASELINE = ROOT / "docs" / "evidence" / "faithful_baseline.json"

HARD = {
    "label": re.compile(r"\blbl_(?:eu|us|jp)_[0-9A-Fa-f]{8}\b"),
    "mangled_c": re.compile(
        r'extern\s+"C"[^\n;{]*?\b(?!__RTTI__|__vt__)[A-Za-z_]\w*?__(?:Q\d|\d+[A-Za-z_])\w*F[A-Za-z0-9_]+'),
    "section": re.compile(r"__declspec\s*\(\s*section"),
    "offset_cast": re.compile(r"\(\s*(?:const\s+)?(?:u8|s8|char|unsigned char)\s*\*\s*\)\s*\(?\s*\w+\s*\)?\s*[+-]\s*0x[0-9A-Fa-f]+"),
    "view_struct": re.compile(r"\b\w*(?:View|At[0-9A-F]{2,}|Off)\s*\*"),
    "noinline": re.compile(r"\bnoinline\b|\bnever_inline\b|\bDECOMP_DONT_INLINE\b|#\s*pragma\s+(?:dont_inline\b|auto_inline\s+off\b)"),
    "decl_shield": re.compile(r"^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+(?:\1_\w+|\w+_hidden)\b", re.M),
    # Two adjacent same-scope declarations: `u32 x; u32& r = x;` (including
    # backslash-continued macro bodies).  Only a *plain* local counts: a
    # volatile/global/static object is not this codegen-forcing trick.
    "self_alias": re.compile(
        r"(?m)^(?P<indent>[ \t]*)"
        r"(?P<type>(?:const\s+|unsigned\s+|signed\s+)*[A-Za-z_]\w*(?:::\w+)*"
        r"(?:\s*<[^;\n{}]*?>)?(?:\s*\*)*)\s+(?P<name>[A-Za-z_]\w*)\s*;"
        r"[ \t]*(?:\\?[ \t]*)\r?\n"
        r"(?P=indent)(?P=type)\s*&\s*[A-Za-z_]\w*\s*=\s*(?P=name)\s*;"),
    "volatile": re.compile(
        r"\(\s*(?:const\s+)?volatile\s+[\w:]+(?:\s+\w+)?\s*\*\s*\)\s*&\s*(?!WGPIPE\b|GXWGFifo\b|__)"),    # Counted by the whole-file string-literal pass (adjacent concatenation
    # and char-vs-string discrimination cannot be done with one line regex).
    "packed_string": re.compile(r"(?!)"),
}
VOLATILE_TOKEN = re.compile(r"\bvolatile\b")
HW_REG_LINE = re.compile(r"\b(?:WGPIPE|GXWGFifo|DECL_HW_REGS|DECL_ADDRESS)\b|0xCC0[0-9A-Fa-f]{5}")
BLOB_NAME = (r"(?:lbl_eu_[0-9A-Fa-f]{8}|\w*(?:[Pp]ool|[Bb]lob|[Tt]able|"
             r"[Ss]trings?|[Dd]ata|[Nn]ames|[Bb]uffer))")
BLOB_OFFSET = re.compile(
    r"\b" + BLOB_NAME + r"\s*\+\s*0x[0-9A-Fa-f]+"
    r"|&\s*" + BLOB_NAME
)
FLOAT_EXTERN_DECL = re.compile(
    r"\b(?:const\s+)?(?:float|double|f32|f64)\s+(?:const\s+)?(?P<name>[A-Za-z_]\w*)\s*;")
LINKAGE_OPEN = re.compile(r'\bextern\s+"C"\s*\{')

# ``type_pun_union`` (plain type-punning unions) is added by count_smells()
# from the structural pass below; there is no single-line regex for it.
SOFT = {
    "field_0x": re.compile(r"\bfield_0x[0-9A-Fa-f]+\b"),
    "unk_name": re.compile(r"\bUnk(?:Class|VirtualFunc|Struct)?_?[0-9A-Fa-f]+\b"),
    "addr_func": re.compile(r"\b(?:func|fn)_[0-9A-Fa-f]{8}\b"),
    "volatile": VOLATILE_TOKEN,
    "blob_offset": BLOB_OFFSET,
    "sdata2_extern": FLOAT_EXTERN_DECL,
}

COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
HEADER_EXT = (".hpp", ".h")
SOURCE_ROOTS = ("src", "include", "libs")
# Only these trees contribute unpaired header pseudo-units; the other libs/
# trees are SDK/vendor code and are not part of the decompilation census.
HEADER_ROOTS = ("src", "include", "libs/monolib")
HEADER_PSEUDO_PREFIX = "hdr/"
SOURCE_EXT = (".c", ".cc", ".cpp", ".h", ".hpp", ".inl")

# Finish-line census (docs/handoff/PLAN_TO_DONE.md §1).  These patterns are
# intentionally separate from HARD/SOFT above: the census honours the inline
# exception markers and counts every spelling of a name debt, while the gate
# and its ratchet baseline keep their existing (unchanged) semantics.
FAITHFUL_OK = re.compile(r"//\s*faithful-ok:\s*(?P<reason>[^\n]*)")
NAME_GUESS = re.compile(r"//\s*name-guess:\s*(?P<reason>[^\n]*)")
VOLATILE_OK_MARKER = re.compile(r"//\s*volatile-ok:\s*(?P<reason>[^\n]*)")

# Address-named functions: func_800xxxxx / fn_800xxxxx plus address suffixes
# on mangled or member spellings (__dt__801C18F4, Class_8045F858, ...).
# Data labels (lbl_*, code_*, float_* pools, vtables) belong to other
# categories and are filtered out by is_addr_func_name().
ADDR_FUNC_NAME = re.compile(
    r"\b(?:func|fn)_[0-9A-Fa-f]{8}\b|\b[A-Za-z_]\w*_[89][0-9A-Fa-f]{7}\b")
DATA_LABEL_PREFIX = ("lbl_", "code_", "float_", "double_", "criware_",
                     "__vt__", "__RTTI__", "Unk", "unk_")
UNK_NAME = re.compile(r"\b(?:Unk\w*|unk_\w*|UNK_\w*)\b")
FIELD_0X = re.compile(r"\bfield_0x[0-9A-Fa-f]+\b")
# A declarator name is preceded by a type/qualifier token; calls are not.
DECL_CONTEXT_STOP = {"return", "new", "delete", "if", "for", "while",
                     "switch", "case", "sizeof", "throw", "else", "do",
                     "goto"}

FINISH_CATEGORIES = (
    ("label", "hard", "hard/label"),
    ("mangled_c", "hard", "hard/mangled_c"),
    ("section", "hard", "hard/section"),
    ("offset_cast", "hard", "hard/offset_cast"),
    ("view_struct", "hard", "hard/view_struct"),
    ("decl_shield", "hard", "hard/decl_shield"),
    ("self_alias", "hard", "hard/self_alias"),
    ("rule", "hard", "hard/rule"),
    ("rtti_off", "hard", "hard/rtti_off"),
    ("foreign_data", "hard", "hard/foreign_data"),
    ("noinline", "hard", "hard/noinline"),
    ("volatile", "hard", "hard/volatile"),
    ("packed_string", "hard", "hard/packed_string"),
    ("volatile (all spellings)", "soft", "soft/volatile"),
    ("blob_offset", "soft", "soft/blob_offset"),
    ("type_pun_union", "soft", "soft/type_pun_union"),
    ("sdata2_extern", "soft", "soft/sdata2_extern"),
    ("addr_func", "soft", "soft/addr_func"),
    ("unk_name", "soft", "soft/unk_name"),
    ("field_0x", "soft", "soft/field_0x"),
)


def strip_comments(text: str) -> str:
    return COMMENT.sub(" ", text)


def strip_comments_keep_lines(text: str) -> str:
    """Blank out comments while keeping line numbers (and offsets) intact."""
    return COMMENT.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), text)


# --- Structural offset-overlay definitions ---------------------------------
# HARD['view_struct'] only sees a *use* of a ``...View*`` / ``...Off*`` type.
# A second, whole-file pass counts the definitions that the regex cannot see:
#   * pad overlay: a struct/class that exists to place a few named fields at
#     large offsets with byte arrays named pad_/blob_/unk_/...;
#   * union view:  a union with an arm that is such an offset-positioned
#     struct (usually alongside a raw array arm);
#   * type pun:    a plain array/scalar union, report-only (soft), because it
#     is still a compiler-shape trick but has no offset-padding debt.
# The pass is deliberately syntactic and conservative: a class with many real
# members and a few unk_ gaps is a recovered layout, not an overlay.  Nothing
# here honours ``// faithful-ok:``: the finish line allows that marker for
# other categories, but not for view overlays in disguise.

# Plain classes, not dataclasses: callers may import this file through
# importlib.spec_from_file_location without registering it in sys.modules,
# which would make @dataclass fail at import time.
class OverlayHit:
    __slots__ = ("kind", "name", "line", "start", "end")

    def __init__(self, kind: str, name: str, line: int, start: int, end: int):
        self.kind = kind          # pad_struct | union_view | type_pun_union
        self.name = name
        self.line = line
        self.start = start
        self.end = end

    def __repr__(self) -> str:
        return (f"OverlayHit({self.kind!r}, {self.name!r}, {self.line}, "
                f"{self.start}, {self.end})")


class _Field:
    __slots__ = ("name", "type_text", "size", "is_pad", "is_byte_array")

    def __init__(self, name: str, type_text: str, size: int | None,
                 is_pad: bool, is_byte_array: bool = False):
        self.name = name
        self.type_text = type_text
        self.size = size
        self.is_pad = is_pad
        self.is_byte_array = is_byte_array

    @property
    def type_key(self) -> str:
        return re.sub(r"\s+", " ", re.sub(r"[*&\s]+", " ", self.type_text)).strip()


class _Record:
    __slots__ = ("kind", "name", "line", "start", "body_start", "end",
                 "fields", "arms")

    def __init__(self, kind: str, name: str | None, line: int, start: int,
                 body_start: int, end: int):
        self.kind = kind
        self.name = name
        self.line = line
        self.start = start
        self.body_start = body_start
        self.end = end
        self.fields: list[_Field] = []
        self.arms: list[_Record] = []


# An explicit pad/blob name is unambiguous.  ``unk_`` only counts as
# padding when it is a sizeable byte array: unk-named scalars are recovered
# fields of a real class (``u8 unk0; u32 unk4; ...``), not offset fillers.
PAD_FIELD_NAME = re.compile(
    r"^_?(?:pad|padding|filler|fill|blob|reserved|reserve|spare|gap|dummy)"
    r"(?:[_\da-fA-FxX]*)?$")
UNK_PAD_NAME = re.compile(r"^_?(?:unk|unused)(?:[_\da-fA-FxX]*)?$")

VIEW_HINT = re.compile(r"\b(?:view|overlay|mirror)\b", re.I)
RECORD_KEYWORD = re.compile(r"\b(struct|class|union)\b")
ACCESS_PREFIX = re.compile(r"^\s*(?:public|private|protected)\s*:\s*")
DECL_QUALIFIERS = re.compile(
    r"\b(?:const|volatile|static|mutable|inline|virtual|explicit|typename|struct|class)\b")
TYPE_SIZE = {
    "u8": 1, "s8": 1, "char": 1, "uchar": 1, "schar": 1, "bool": 1,
    "unsigned char": 1, "signed char": 1,
    "u16": 2, "s16": 2, "short": 2, "ushort": 2, "wchar_t": 2,
    "unsigned short": 2, "signed short": 2,
    "u32": 4, "s32": 4, "int": 4, "uint": 4, "f32": 4, "float": 4,
    "long": 4, "unsigned": 4, "signed": 4, "unsigned int": 4,
    "signed int": 4, "unsigned long": 4, "signed long": 4,
    "u64": 8, "s64": 8, "f64": 8, "double": 8, "long long": 8,
    "unsigned long long": 8, "signed long long": 8,
}
INT_EXPR = re.compile(r"^[0-9A-Fa-fxX+\-*/()<>&|\s]+$")


def _eval_int(expr: str) -> int | None:
    """Evaluate a C array bound (``0x3F00 - 0xA2``); None if not a constant."""
    expr = expr.strip()
    if not expr or not INT_EXPR.fullmatch(expr):
        return None
    try:
        node = ast.parse(expr, mode="eval").body
    except (SyntaxError, ValueError):
        return None

    def walk(n: ast.AST) -> int | None:
        if isinstance(n, ast.Constant) and isinstance(n.value, int):
            return n.value
        if isinstance(n, ast.UnaryOp) and isinstance(n.op, (ast.UAdd, ast.USub)):
            v = walk(n.operand)
            return None if v is None else (v if isinstance(n.op, ast.UAdd) else -v)
        if isinstance(n, ast.BinOp) and isinstance(
                n.op, (ast.Add, ast.Sub, ast.Mult, ast.FloorDiv, ast.LShift, ast.RShift)):
            a, b = walk(n.left), walk(n.right)
            if a is None or b is None or (isinstance(n.op, (ast.FloorDiv, ast.LShift, ast.RShift)) and b == 0):
                return None
            try:
                return {
                    ast.Add: lambda: a + b, ast.Sub: lambda: a - b,
                    ast.Mult: lambda: a * b, ast.FloorDiv: lambda: a // b,
                    ast.LShift: lambda: a << b, ast.RShift: lambda: a >> b,
                }[type(n.op)]()
            except (OverflowError, ValueError):
                return None
        return None

    return walk(node)


def _blank_preprocessor(code: str) -> str:
    """Drop #directives (and their continuations) without moving offsets."""
    out: list[str] = []
    continued = False
    for line in code.splitlines(keepends=True):
        if continued or line.lstrip().startswith("#"):
            out.append(re.sub(r"[^\r\n]", " ", line))
            continued = line.rstrip("\r\n").rstrip().endswith("\\")
        else:
            out.append(line)
    return "".join(out)


def _skip_quoted(code: str, i: int) -> int:
    quote = code[i]
    i += 1
    while i < len(code):
        if code[i] == "\\":
            i += 2
        elif code[i] == quote:
            return i + 1
        else:
            i += 1
    return i


def _record_body(code: str, pos: int, limit: int) -> int | None:
    """Index of the definition's '{', or None for a forward declaration."""
    par = brk = 0
    i = pos
    while i < limit:
        c = code[i]
        if c in "\"'":
            i = _skip_quoted(code, i)
            continue
        if c == "(":
            par += 1
        elif c == ")":
            par = max(0, par - 1)
        elif c == "[":
            brk += 1
        elif c == "]":
            brk = max(0, brk - 1)
        elif c == "{" and par == brk == 0:
            return i
        elif c == ";" and par == brk == 0:
            return None
        i += 1
    return None


def _close_brace(code: str, open_at: int, limit: int) -> int:
    depth = 0
    i = open_at
    while i < limit:
        c = code[i]
        if c in "\"'":
            i = _skip_quoted(code, i)
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return limit - 1


def _split_chunks(code: str, start: int, end: int, sep: str = ";") -> list[str]:
    """Split on a separator at bracket depth zero (nested bodies stay opaque)."""
    chunks: list[str] = []
    depth = 0
    buf: list[str] = []
    i = start
    while i < end:
        c = code[i]
        if c in "\"'":
            j = _skip_quoted(code, i)
            buf.append(code[i:j])
            i = j
            continue
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == sep and depth == 0:
            chunks.append("".join(buf))
            buf = []
        elif c != ";" or depth != 0:
            buf.append(c)
        i += 1
    tail = "".join(buf)
    if tail.strip():
        chunks.append(tail)
    return chunks


def _parse_field(chunk: str) -> list[_Field]:
    """Best-effort declarators of one member declaration (methods -> [])."""
    text = ACCESS_PREFIX.sub("", chunk).strip()
    if not text or "typedef" in text.split() or "friend" in text.split():
        return []
    if RECORD_KEYWORD.search(text):
        return []  # nested record arm, handled by _parse_records
    text = text.split("=", 1)[0]
    # A (member) function pointer is a real positioned field; retain it with
    # pointer width so vtable/hook overlay structs keep their named member.
    fnptr = re.search(r"\(\s*(?:[\w:]+\s*::\s*)?\*\s*([A-Za-z_]\w*)\s*\)\s*\(", text)
    if fnptr:
        return [_Field(fnptr.group(1), "void*", 4, False)]
    if "(" in text or ")" in text:
        return []  # method, constructor or other function type
    out: list[_Field] = []
    for part in _split_chunks(text + ";", 0, len(text) + 1, ","):
        head = part.strip()
        arrays = []
        while True:
            m = re.search(r"\[([^\[\]]*)\]\s*$", head)
            if not m:
                break
            arrays.insert(0, m.group(1))
            head = head[:m.start()].rstrip()
        m = re.search(r"([A-Za-z_]\w*)\s*$", head)
        if not m:
            continue
        name = m.group(1)
        type_text = head[:m.start()].strip()
        if not type_text or name in {"public", "private", "protected"}:
            continue
        bare = DECL_QUALIFIERS.sub(" ", type_text)
        bare = re.sub(r"\s+", " ", bare.replace("*", " ").replace("&", " ")).strip()
        base_size = 4 if "*" in type_text else TYPE_SIZE.get(bare)
        size = base_size
        if size is not None:
            for bound in arrays:
                n = _eval_int(bound)
                size = None if n is None else size * n
                if size is None:
                    break
        byte_array = bool(arrays) and base_size == 1 and (size or 0) >= 4
        is_pad = bool(PAD_FIELD_NAME.match(name)) or (
            byte_array and bool(UNK_PAD_NAME.match(name)))
        out.append(_Field(name, type_text, size, is_pad, byte_array))
    return out


def _parse_records(code: str, start: int, end: int) -> list[_Record]:
    """Find struct/class/union definitions and their direct data members."""
    records: list[_Record] = []
    i = start
    while i < end:
        m = RECORD_KEYWORD.search(code, i, end)
        if not m:
            break
        body = _record_body(code, m.end(), end)
        if body is None:
            i = m.end()
            continue
        close = _close_brace(code, body, end)
        name_m = re.search(r"\b([A-Za-z_]\w*)\b", code[m.end():body])
        name = None if not name_m or name_m.group(1) == "final" else name_m.group(1)
        rec = _Record(m.group(1), name, code.count("\n", 0, m.start()) + 1,
                      m.start(), body, close + 1)
        for chunk in _split_chunks(code, body + 1, close):
            rec.fields.extend(_parse_field(chunk))
        rec.arms = _parse_records(code, body + 1, close)
        records.append(rec)
        i = close + 1
    return records


def _has_view_hint(rec: _Record, raw_lines: list[str] | None) -> bool:
    """Did the author call this record a view/overlay/mirror?"""
    if rec.name and VIEW_HINT.search(rec.name):
        return True
    if raw_lines and rec.line <= len(raw_lines):
        lo = max(0, rec.line - 3)
        return bool(VIEW_HINT.search("\n".join(raw_lines[lo:rec.line])))
    return False


def _pad_positioned(rec: _Record, raw_lines: list[str] | None = None,
                    *, arm: bool = False) -> bool:
    """Does this record exist solely to locate a few fields at offsets?"""
    data = rec.fields
    pads = [f for f in data if f.is_pad and f.size is not None]
    named = [f for f in data if not f.is_pad]
    if not pads or not named:
        return False
    # Padding at the very end is plain storage, not offset positioning
    # (``struct Init { s16 ids[6]; u8 blob[192]; };``).
    last_pad = max(i for i, f in enumerate(data) if f.is_pad)
    if not any(i > last_pad and not f.is_pad for i, f in enumerate(data)):
        return False
    hinted = _has_view_hint(rec, raw_lines)
    # A named field of 0x80+ bytes is real array storage (a vertex table, a
    # record array...), not one of the few scalars an overlay positions.
    if any((f.size or 0) >= 0x80 for f in named) and not hinted:
        return False
    pad_bytes = sum(f.size or 0 for f in pads)
    if arm:
        return (data[0].is_pad and data[0].size is not None and data[0].size >= 4
                and len(named) <= 3)
    if data[0].is_pad and data[0].size is not None and data[0].size >= 0x10:
        if len(named) <= 4:
            return True
    known = pad_bytes + sum(f.size or 0 for f in named)
    if pad_bytes >= 0x10 and known and pad_bytes * 4 > known * 3 and len(named) <= 6:
        return True
    # The author's own words count when there is real padding to go with them.
    if pad_bytes >= 0x10 and len(named) <= 8 and hinted:
        return True
    return False


def _type_pun_union(rec: _Record) -> bool:
    """Union of differently typed scalar/array views, with no offset pads."""
    arms = [f for f in rec.fields if not f.is_pad]
    if len(arms) + len(rec.arms) < 2:
        return False
    keys = {f.type_key for f in arms} | {arm.name or arm.kind for arm in rec.arms}
    return len(keys) >= 2


def detect_pad_overlays(text: str) -> list[OverlayHit]:
    """Structural overlay hits in one file (comment-stripped for parsing).

    Each definition counts once.  A union counted as a view swallows its
    anonymous pad-positioned arms so one pattern is not charged twice.
    ``kind`` is ``pad_struct``/``union_view`` (hard) or ``type_pun_union``
    (soft); ``line`` is 1-based for reporting.
    """
    raw_lines = text.split("\n")
    code = _blank_preprocessor(strip_comments_keep_lines(text))
    records = _parse_records(code, 0, len(code))
    hits: list[OverlayHit] = []
    for rec in records:
        if rec.kind == "union":
            pad_arms = [a for a in rec.arms if _pad_positioned(a, raw_lines, arm=True)]
            if pad_arms and len(rec.arms) + len(rec.fields) >= 2:
                hits.append(OverlayHit("union_view", rec.name or "<anonymous>",
                                       rec.line, rec.start, rec.end))
                continue
            if not pad_arms and _type_pun_union(rec):
                hits.append(OverlayHit("type_pun_union", rec.name or "<anonymous>",
                                       rec.line, rec.start, rec.end))
        elif _pad_positioned(rec, raw_lines):
            hits.append(OverlayHit("pad_struct", rec.name or "<anonymous>",
                                   rec.line, rec.start, rec.end))
    hard = [(i, h.start, h.end) for i, h in enumerate(hits)
            if h.kind != "type_pun_union"]
    return [h for i, h in enumerate(hits)
            if h.kind == "type_pun_union"
            or not any(j != i and s <= h.start < e for j, s, e in hard)]


def count_pad_overlays(text: str) -> tuple[int, int]:
    """Return (hard view_struct additions, soft type_pun_union count)."""
    hits = detect_pad_overlays(text)
    return (sum(h.kind != "type_pun_union" for h in hits),
            sum(h.kind == "type_pun_union" for h in hits))


# --- Packed string literals ------------------------------------------------
# A string that writes its own NUL bytes (``"thread\0"``, ``"x\0\0\0"``,
# ``"name\0value"``) is another way of spelling a struct overlay/padding in
# the source: the original would have used a char array or a named string.
# Count one hit per literal *expression*, including adjacent-literal
# concatenation.  ``'\0'`` is a char constant, not a string, and ``"\\0"``
# is a backslash plus '0', so neither counts.

NUL_ESCAPE = re.compile(r"\\(?:0(?![1-7])|x0*0(?![0-9A-Fa-f]))")
STRING_START = re.compile(r"(?:u8|[uUL])?\"")
class PackedLiteral:
    __slots__ = ("line", "text")

    def __init__(self, line: int, text: str):
        self.line = line
        self.text = text

    def __repr__(self) -> str:
        return f"PackedLiteral({self.line}, {self.text!r})"


def strip_comments_respecting_strings(text: str) -> str:
    """Blank out comments but never a ``//`` or ``/*`` inside a literal."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            for k in range(i, j):
                out[k] = " "
            i = j
            continue
        if text.startswith("/*", i):
            end = text.find("*/", i + 2)
            j = n if end < 0 else end + 2
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j
            continue
        if c in "\"'":
            quote = c
            i += 1
            while i < n:
                if text[i] == "\\":
                    i += 2
                elif text[i] == quote:
                    i += 1
                    break
                elif text[i] == "\n":
                    break
                else:
                    i += 1
            continue
        i += 1
    return "".join(out)


def _string_literal_end(code: str, quote: int) -> tuple[int, bool]:
    """Index just after a double-quoted literal and whether it spells NUL."""
    i, nul = quote + 1, False
    while i < len(code):
        c = code[i]
        if c == "\\":
            if NUL_ESCAPE.match(code, i):
                nul = True
            i += 2
        elif c == "\"":
            return i + 1, nul
        elif c == "\n":
            return i, nul  # unterminated: stop before the newline
        else:
            i += 1
    return i, nul


def detect_packed_strings(text: str) -> list[PackedLiteral]:
    """Every literal expression that explicitly writes a NUL byte."""
    code = strip_comments_respecting_strings(text)
    hits: list[PackedLiteral] = []
    i = 0
    while i < len(code):
        c = code[i]
        if c == "'":  # char literal: skip as a whole
            j = i + 1
            while j < len(code):
                if code[j] == "\\":
                    j += 2
                elif code[j] == "'":
                    j += 1
                    break
                else:
                    j += 1
            i = j
            continue
        m = STRING_START.match(code, i)
        if not m or (i and (code[i - 1].isalnum() or code[i - 1] == "_")):
            i += 1
            continue
        line = code.count("\n", 0, i) + 1
        start = i
        has_nul = False
        while True:
            quote = m.end() - 1
            end, nul = _string_literal_end(code, quote)
            has_nul |= nul
            i = end
            k = end
            while k < len(code):
                if code[k] in " \t\r\n":
                    k += 1
                elif code[k] == "\\" and k + 1 < len(code) and code[k + 1] == "\n":
                    k += 2  # backslash-newline inside a macro
                else:
                    break
            nxt = STRING_START.match(code, k)
            if not nxt:
                break
            m = nxt
        if has_nul:
            hits.append(PackedLiteral(line, code[start:i].strip()))
    return hits


def count_packed_strings(text: str) -> int:
    """One hard ``packed_string`` hit per packed literal expression."""
    return len(detect_packed_strings(text))


NOINLINE_OK = re.compile(r"//\s*noinline-ok:\s*\S")
VOLATILE_OK = re.compile(r"//\s*volatile-ok:\s*\S")


def exempt_justified_noinline(text: str) -> str:
    """Drop the noinline / volatile smell from lines annotated
    ``// noinline-ok: <reason>`` / ``// volatile-ok: <reason>``."""
    out = []
    for line in text.split("\n"):
        if NOINLINE_OK.search(line):
            line = HARD["noinline"].sub(" ", line)
        if VOLATILE_OK.search(line):
            line = HARD["volatile"].sub(" ", line)
            line = VOLATILE_TOKEN.sub(" ", line)  # soft census follows the exemption
        out.append(line)
    return "\n".join(out)


_REAL_CLASS_SYM = re.compile(r"^__(?:ct|dt|vt)__(\d+)([A-Za-z_]\w*)", re.M)


@functools.lru_cache(maxsize=None)
def real_classes(region: str = "us") -> frozenset[str]:
    """Class names that retail itself names via a ctor/dtor/vtable symbol.

    A pointer to such a class (``CView*``) is a real type, not an offset view,
    even though its name matches the ``view_struct`` pattern."""
    path = ROOT / "config" / region / "symbols.txt"
    if not path.exists():
        return frozenset()
    return frozenset(m.group(2)[:int(m.group(1))]
                     for m in _REAL_CLASS_SYM.finditer(path.read_text(errors="replace")))


def _count(key: str, pat: re.Pattern, code: str) -> int:
    if key != "view_struct":
        return len(pat.findall(code))
    real = real_classes()
    return sum(1 for m in pat.finditer(code)
               if m.group(0).rstrip("* \t\n") not in real)


def count_volatile_all(code: str) -> int:
    """Every remaining volatile spelling, including declarations and casts."""
    return sum(len(VOLATILE_TOKEN.findall(line)) for line in code.split("\n")
               if not HW_REG_LINE.search(line))


def sdata2_extern_line_counts(code: str) -> list[int]:
    """Per-line version of count_sdata2_externs (linkage blocks are stateful)."""
    counts: list[int] = []
    in_linkage = False
    for line in code.splitlines():
        text = line.strip()
        n = 0
        if not in_linkage:
            opening = LINKAGE_OPEN.search(text)
            if opening:
                body = text[opening.end():]
                n += len(FLOAT_EXTERN_DECL.findall(body))
                in_linkage = "}" not in body
                counts.append(n)
                continue
        else:
            if "}" in text:
                in_linkage = False
            n += len(FLOAT_EXTERN_DECL.findall(text))
            counts.append(n)
            continue
        if text.startswith("extern "):
            n += len(FLOAT_EXTERN_DECL.findall(text))
        counts.append(n)
    return counts


def count_sdata2_externs(code: str) -> int:
    """Scalar float/double objects declared extern (sdata2 constant reads).

    A named pool constant (``extern const f32 lbl_eu_...;``) is a symbol the
    original source would have spelled as a literal.  Array pools and SDK
    hardware registers are not counted.  ``extern "C" { ... }`` blocks are
    tracked so the one-line and multi-line spellings behave the same.
    """
    return sum(sdata2_extern_line_counts(code))


def _count_soft(key: str, pat: re.Pattern, code: str) -> int:
    if key == "volatile":
        return count_volatile_all(code)
    if key == "sdata2_extern":
        return count_sdata2_externs(code)
    return len(pat.findall(code))


def count_smells(text: str, *, structural: bool = True) -> tuple[dict[str, int], dict[str, int]]:
    """Line-regex smells plus (unless disabled) whole-file overlay definitions.

    ``structural=False`` is used by the finish-line census, which calls this
    once per line and adds the whole-file overlay result itself so that a
    ``// faithful-ok:`` marker cannot exempt a pad overlay.
    """
    code = strip_comments(exempt_justified_noinline(text))
    hard = {k: _count(k, p, code) for k, p in HARD.items()}
    soft = {k: _count_soft(k, p, code) for k, p in SOFT.items()}
    if structural:
        n_hard, n_soft = count_pad_overlays(text)
        hard["view_struct"] += n_hard
        soft["type_pun_union"] = n_soft
        hard["packed_string"] = count_packed_strings(text)
    return hard, soft


def is_addr_func_name(name: str) -> bool:
    """True for an address-named function, false for data labels/types."""
    if name.startswith(DATA_LABEL_PREFIX) or "lbl_" in name:
        return False
    if re.match(r"L[0-9A-Fa-f]", name):  # L8008F8D8-style local labels
        return False
    return True


def count_addr_func_defs(line: str) -> int:
    """Address placeholders used as function declarators on this source line.

    Calls are not name debt: the debt is the definition/declaration that has
    to be renamed (and the retail symbol it stands for).  The shape follows
    frontier_refs.base_symbol()/HARD['label']: an 8-hex-digit address attached
    to the identifier, with C++ mangling and region prefixes allowed.
    """
    n = 0
    for m in ADDR_FUNC_NAME.finditer(line):
        if not is_addr_func_name(m.group(0)):
            continue
        if not re.match(r"\s*\(", line[m.end():]):
            continue  # a use, not a declarator
        before = re.sub(r"::\s*$", "", line[:m.start()].rstrip()).rstrip()
        if not re.search(r"[A-Za-z_]\w*$", before):
            continue
        if before.split()[-1] in DECL_CONTEXT_STOP:
            continue
        n += 1
    return n


def count_finish_line_file(
    text: str, path: str | None = None
) -> tuple[collections.Counter, list[tuple], int, int]:
    """Finish-line counts for one file (plus reviewed exceptions and guesses).

    ``// faithful-ok:`` exempts every count on its line and is returned as a
    ``(path, line, kind, reason)`` record; ``// name-guess:`` exempts only the
    ``unk_name`` / ``field_0x`` counts and increments the guess count.  The
    pre-existing ``// volatile-ok:`` / ``// noinline-ok:`` exemptions still
    apply exactly as they do in count_smells().
    """
    code = strip_comments_keep_lines(text)
    raw_lines, code_lines = text.split("\n"), code.split("\n")
    sdata_lines = sdata2_extern_line_counts(code)
    totals: collections.Counter = collections.Counter()
    exceptions: list[tuple] = []
    guesses = volatile_exempt = 0
    for i, (raw, line) in enumerate(zip(raw_lines, code_lines)):
        ok, guess, vok = (FAITHFUL_OK.search(raw), NAME_GUESS.search(raw),
                          VOLATILE_OK_MARKER.search(raw))
        if ok:
            exceptions.append((path or "<text>", i + 1, "faithful-ok",
                                ok.group("reason").strip()))
        if guess:
            guesses += 1
        if vok:
            volatile_exempt += 1
        if NOINLINE_OK.search(raw):
            line = HARD["noinline"].sub(" ", line)
        if vok:
            line = HARD["volatile"].sub(" ", line)
            line = VOLATILE_TOKEN.sub(" ", line)
        hard, soft = count_smells(line, structural=False)
        values = {"hard/" + k: v for k, v in hard.items()}
        values.update({"soft/" + k: v for k, v in soft.items()})
        values["soft/addr_func"] = count_addr_func_defs(line)
        values["soft/unk_name"] = len(UNK_NAME.findall(line))
        values["soft/field_0x"] = len(FIELD_0X.findall(line))

        if ok:
            values = dict.fromkeys(values, 0)
        elif guess:
            values["soft/unk_name"] = 0
            values["soft/field_0x"] = 0
        totals.update(values)
    # Whole-file structural hits are added after the per-line exemptions:
    # // faithful-ok: never suppresses a view overlay in disguise.
    n_hard, n_soft = count_pad_overlays(text)
    totals["hard/view_struct"] += n_hard
    totals["soft/type_pun_union"] += n_soft
    totals["hard/packed_string"] += count_packed_strings(text)
    return totals, exceptions, guesses, volatile_exempt


def header_index() -> dict[str, list[Path]]:
    idx: dict[str, list[Path]] = collections.defaultdict(list)
    for root in SOURCE_ROOTS:
        for p in (ROOT / root).rglob("*"):
            if p.suffix in HEADER_EXT and p.is_file():
                idx[p.stem].append(p)
    return idx


def pick_header(src: Path, idx: dict[str, list[Path]]) -> Path | None:
    cands = idx.get(src.stem, [])
    if not cands:
        return None

    def shared_tail(p: Path) -> int:
        a, b = src.parent.parts[::-1], p.parent.parts[::-1]
        n = 0
        while n < min(len(a), len(b)) and a[n] == b[n]:
            n += 1
        return n

    best = max(cands, key=shared_tail)
    # A stem shared by unrelated directories (types.h, util.h) is not "the
    # unit's header" unless the paths actually line up.
    if len(cands) > 1 and shared_tail(best) == 0:
        return None
    return best


def paired_header_paths(measure_result: dict[str, dict]) -> set[str]:
    """Repo-relative header paths selected as a unit's paired header."""
    return {r["header"] for r in measure_result.values() if r.get("header")}


def header_pseudo_name(path: Path | str) -> str:
    """Pseudo-unit key for a header (``hdr/<repo-relative path>``)."""
    rel = Path(path)
    if rel.is_absolute():
        rel = rel.relative_to(ROOT)
    return HEADER_PSEUDO_PREFIX + rel.as_posix()


def unpaired_header_paths(measure_result: dict[str, dict]) -> list[Path]:
    """Headers in HEADER_ROOTS that no unit picks as its paired header.

    Non-header files (including generated ``*.ctx.c``) and SDK/vendor trees
    outside HEADER_ROOTS are never enumerated.
    """
    paired = paired_header_paths(measure_result)
    out: list[Path] = []
    for root in HEADER_ROOTS:
        base = ROOT / root
        if not base.is_dir():
            continue
        for path in sorted(base.rglob("*")):
            if path.suffix not in HEADER_EXT or not path.is_file():
                continue
            if path.name.endswith(".ctx.c"):
                continue
            if str(path.relative_to(ROOT)) in paired:
                continue
            out.append(path)
    return out


def measure_unpaired_headers(measure_result: dict[str, dict]) -> dict[str, dict]:
    """Score each unpaired header as a ``hdr/...`` pseudo-unit."""
    out: dict[str, dict] = {}
    for path in unpaired_header_paths(measure_result):
        rel = path.relative_to(ROOT).as_posix()
        hard, soft = count_smells(path.read_text(errors="replace"))
        out[header_pseudo_name(path)] = {
            "source": rel,
            "header": None,
            "complete": False,
            "hard": hard,
            "soft": soft,
            "score": sum(hard.values()),
        }
    return out


def foreign_data_counts(region: str) -> dict[str, int]:
    from tools.coop import section_owners

    out: collections.Counter = collections.Counter()
    # Only anonymous symbols count: a named global used by one other unit is
    # not misplaced (section_owners.tier).
    for _sec, r, _tier in section_owners.disagreements(region, ("A", "B")):
        out[r["vote"]] += 1
        if r["cur"]:
            out[r["cur"]] += 1
    return dict(out)


RTTI_OFF = re.compile(r'Object\(\s*\w+\s*,\s*"([^"]+)"[^\n]*?-RTTI off')


def rtti_off_units() -> set[str]:
    """Source paths (as written in configure.py) compiled with -RTTI off."""
    return set(RTTI_OFF.findall((ROOT / "configure.py").read_text()))


def measure(region: str = "us") -> dict[str, dict]:
    import tools.postprocess_reloc_names as P
    from tools.coop.lib import weak_dedup as W

    ranges, _ = W._layout(ROOT, region)
    foreign = foreign_data_counts(region)
    idx = header_index()
    no_rtti = rtti_off_units()
    out: dict[str, dict] = {}
    for u in json.loads((ROOT / "objdiff.json").read_text())["units"]:
        sp = (u.get("metadata") or {}).get("source_path")
        if not sp or sp.endswith(".ctx.c") or not (ROOT / sp).is_file():
            continue
        src = ROOT / sp
        hard, soft = count_smells(src.read_text(errors="replace"))
        hdr = pick_header(src, idx)
        if hdr is not None:
            h2, s2 = count_smells(hdr.read_text(errors="replace"))
            for k in hard:
                hard[k] += h2[k]
            for k in soft:
                soft[k] += s2[k]
        base = u.get("base_path") or ""
        hard["rule"] = 1 if base and P.unit_has_rules(Path(base).name) else 0
        hard["rtti_off"] = 1 if any(sp.endswith("/" + n) for n in no_rtti) else 0
        split = W.resolve_unit(ranges, u["name"])
        hard["foreign_data"] = foreign.get(split, 0) if split else 0
        out[u["name"]] = {
            "source": sp,
            "header": str(hdr.relative_to(ROOT)) if hdr else None,
            "complete": bool((u.get("metadata") or {}).get("complete")),
            "hard": hard,
            "soft": soft,
            "score": sum(hard.values()),
        }
    return out


def compare(baseline: dict[str, int], current: dict[str, int]) -> tuple[list[str], list[str]]:
    """Return (failures, improvements).  Units absent from a map score 0."""
    fails, better = [], []
    for name in sorted(set(baseline) | set(current)):
        b, c = baseline.get(name, 0), current.get(name, 0)
        if c > b:
            fails.append(f"{name}: {b} -> {c}")
        elif c < b:
            better.append(f"{name}: {b} -> {c}")
    return fails, better


def gate(region: str, ratchet: bool, init: bool, verbose: bool, unit: str | None = None) -> int:
    res = measure(region)
    headers = measure_unpaired_headers(res)
    res.update(headers)
    # Keep zero-score pseudo-units in `cur`: seeding them explicitly must not
    # later look like a phantom improvement because they are absent from cur.
    cur = {n: r["score"] for n, r in res.items()
           if r["score"] or n in headers}
    faithful = [n for n, r in res.items() if not r["score"]]
    totals: collections.Counter = collections.Counter()
    soft_totals: collections.Counter = collections.Counter()
    for r in res.values():
        totals.update(r["hard"])
        soft_totals.update(r["soft"])
    if unit:
        for n, r in res.items():
            if unit in n:
                print(n, json.dumps({k: v for k, v in r.items() if k != "score"}, indent=1))
    print(f"faithful: {len(faithful)} of {len(res)} units with source "
          f"({sum(1 for n in faithful if res[n]['complete'])} of them complete; "
          f"{len(headers)} unpaired header pseudo-units)")
    print("  hard: " + "  ".join(f"{k} {v}" for k, v in totals.items()))
    print("  soft: " + "  ".join(f"{k} {v}" for k, v in soft_totals.items()))
    if verbose:
        for n in sorted(cur, key=cur.get, reverse=True)[:40]:
            print(f"    {cur[n]:6} {n}")
    if init:
        BASELINE.write_text(json.dumps({"region": region, "units": cur}, indent=1, sort_keys=True) + "\n")
        print(f"faithful: wrote baseline ({len(cur)} tracked unit(s))")
        return 0
    if not BASELINE.is_file():
        print("faithful: no baseline; run with --init")
        return 1
    base = json.loads(BASELINE.read_text())["units"]
    # A header deleted from the tree just disappears: drop stale hdr/ keys
    # from the comparison instead of reporting them as fake improvements.
    active_base = {n: v for n, v in base.items()
                   if not (n.startswith(HEADER_PSEUDO_PREFIX) and n not in headers)}
    unbaselined = {n for n in headers if n not in base}
    for n in sorted(unbaselined):
        print(f"WARN: unbaselined header {n} (score {headers[n]['score']}); "
              "run with --seed-headers to add it")
    # Unbaselined pseudo-units are warn-only: neither the baseline nor the
    # current map is compared for them until they are seeded explicitly.
    active_cur = {n: v for n, v in cur.items() if n not in unbaselined}
    fails, better = compare(active_base, active_cur)
    for f in fails:
        print(f"FAIL: faithfulness score rose in {f}")
    if fails:
        return 1
    if ratchet and better:
        BASELINE.write_text(json.dumps({"region": region, "units": cur}, indent=1, sort_keys=True) + "\n")
        print(f"faithful: ratcheted baseline ({len(better)} change(s))")
    elif better:
        print(f"faithful: OK; {len(better)} unit(s) improved, run with --ratchet to lock them in")
    else:
        print("faithful: OK")
    return 0


def seed_headers(region: str) -> int:
    """Add missing ``hdr/`` pseudo-units to the baseline; never rewrite others.

    Existing entries keep their recorded scores, including stale headers that
    have since been deleted and headers whose score has improved.
    """
    headers = measure_unpaired_headers(measure(region))
    if BASELINE.is_file():
        data = json.loads(BASELINE.read_text())
    else:
        data = {}
    units = dict(data.get("units") or {})
    added = 0
    for name, r in sorted(headers.items()):
        if name not in units:
            units[name] = r["score"]
            added += 1
    data["units"] = units
    BASELINE.write_text(json.dumps(data, indent=1, sort_keys=True) + "\n")
    print(f"faithful: seeded {added} unpaired header(s) into the baseline")
    return 0


def finish_line_markers(text: str, path: str) -> tuple[list[tuple], int, int]:
    """Marker census over every source file, including unselected headers."""
    exceptions: list[tuple] = []
    guesses = volatile_exempt = 0
    for i, raw in enumerate(text.split("\n")):
        ok = FAITHFUL_OK.search(raw)
        if ok:
            exceptions.append((path, i + 1, "faithful-ok",
                                ok.group("reason").strip()))
        if NAME_GUESS.search(raw):
            guesses += 1
        if VOLATILE_OK_MARKER.search(raw):
            volatile_exempt += 1
    return exceptions, guesses, volatile_exempt


def finish_line_report(region: str, totals: collections.Counter,
                       exceptions: list[tuple], guesses: int,
                       volatile_exempt: int,
                       header_totals: collections.Counter | None = None) -> list[str]:
    """Render the PLAN_TO_DONE §1 table (all targets are zero).

    ``header_totals`` is the unpaired-header contribution already included in
    ``totals``; each category gets its own ``of which unpaired headers`` line.
    """
    ht = header_totals or collections.Counter()
    lines = [
        f"faithful finish line ({region}) - docs/handoff/PLAN_TO_DONE.md §1",
        f"{'category':26} {'kind':5} {'current':>8} {'target':>6}  status",
    ]
    for label, kind, key in FINISH_CATEGORIES:
        count = totals.get(key, 0)
        lines.append(f"{label:26} {kind:5} {count:8} {0:6}  "
                     f"{'DONE' if count == 0 else 'OPEN'}")
        lines.append(f"  of which unpaired headers: {ht.get(key, 0)}")
    lines += [
        f"guessed names (// name-guess:): {guesses}",
        f"volatile definitions (// volatile-ok:): {volatile_exempt}",
        f"reviewed exceptions (// faithful-ok:): {len(exceptions)}",
    ]
    for path, lineno, _kind, reason in exceptions:
        lines.append(f"  {path}:{lineno}: {reason}")
    return lines


def finish_line(region: str = "us") -> int:
    """Print the §1 finish-line table.  Never reads or writes the baseline."""
    res = measure(region)
    headers = measure_unpaired_headers(res)
    totals: collections.Counter = collections.Counter()
    header_totals: collections.Counter = collections.Counter()
    # rule / rtti_off / foreign_data are unit-level, not source-line, counts.
    for r in res.values():
        for key in ("rule", "rtti_off", "foreign_data"):
            totals["hard/" + key] += r["hard"][key]
    # Same file coverage as the gate (each unit's source + picked header).
    for r in res.values():
        for key in ("source", "header"):
            rel = r[key]
            if not rel:
                continue
            path = ROOT / rel
            if not path.is_file():
                continue
            counts, _, _, _ = count_finish_line_file(
                path.read_text(errors="replace"), rel)
            totals.update(counts)
    # Unpaired headers use the same marker-aware census as paired headers, as
    # hdr/ pseudo-units; they are included in every category's current total.
    for r in headers.values():
        path = ROOT / r["source"]
        if not path.is_file():
            continue
        counts, _, _, _ = count_finish_line_file(
            path.read_text(errors="replace"), r["source"])
        header_totals.update(counts)
        totals.update(counts)
    # Markers live in comments, including headers no unit selects.
    exceptions: list[tuple] = []
    guesses = volatile_exempt = 0
    for root in SOURCE_ROOTS:
        for path in sorted((ROOT / root).rglob("*")):
            if path.suffix not in SOURCE_EXT or not path.is_file():
                continue
            exs, g, v = finish_line_markers(
                path.read_text(errors="replace"), str(path.relative_to(ROOT)))
            exceptions.extend(exs)
            guesses += g
            volatile_exempt += v
    for line in finish_line_report(region, totals, exceptions, guesses,
                                   volatile_exempt, header_totals):
        print(line)
    return 0


def build_parser() -> argparse.ArgumentParser:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--region", default="us")
    ap.add_argument("--ratchet", action="store_true")
    ap.add_argument("--init", action="store_true")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--unit")
    ap.add_argument("--finish-line", action="store_true",
                    help="print the §1 finish-line table; no baseline access")
    ap.add_argument("--seed-headers", action="store_true",
                    help="add missing hdr/ pseudo-units to the baseline; "
                         "never rewrite existing entries")
    return ap


def main() -> int:
    ap = build_parser()
    a = ap.parse_args()
    sys.path.insert(0, str(ROOT))
    if a.finish_line:
        if a.ratchet or a.init or a.seed_headers:
            ap.error("--finish-line cannot be combined with "
                     "--ratchet/--init/--seed-headers")
        return finish_line(a.region)
    if a.seed_headers:
        if a.ratchet or a.init:
            ap.error("--seed-headers cannot be combined with --ratchet/--init")
        return seed_headers(a.region)
    return gate(a.region, a.ratchet, a.init, a.verbose, a.unit)


if __name__ == "__main__":
    sys.path.insert(0, str(ROOT))
    sys.exit(main())

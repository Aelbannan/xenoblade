#!/usr/bin/env python3
"""layout_audit.py -- static struct/class layout-offset audit (pure text, no build).

Finds the class of header layout bug where a member's declared type occupies
fewer bytes than the gap to the next member's declared offset, so every member
after it sits at the wrong offset (e.g. the old CObjectParam unk14 bug: a
``u8 unk14[...]`` that was really 12 bytes while the next member was annotated
``// 0x30``).

Method: parse struct/class/union bodies from .hpp/.h/.cpp, read each member's
declared type + array extent + offset annotation (``// 0xNN`` comment or an
offset-encoding name such as ``pad_74``/``field_30``/``unk15DC``), compute each
member's size from a PPC32 type table (recursively resolving named structs,
cycle-guarded), then compare each member's span
(``next.declared - declared``) against its ``sizeof``.

Only structs that carry offset annotations are checked (no spam for
un-annotated structs). Unknown-type members are reported as ``size?`` and
skipped, never guessed.

Usage:
    .venv/bin/python3 tools/coop/layout_audit.py --roots include src libs
    .venv/bin/python3 tools/coop/layout_audit.py --roots include src libs --json /tmp/layout.json
    .venv/bin/python3 tools/coop/layout_audit.py --class CObjectParam
"""

from __future__ import annotations

import argparse
import fnmatch
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

# ---------------------------------------------------------------------------
# PPC32 type table (sizes only; alignment derived as min(size, 8) capped by pack)
# ---------------------------------------------------------------------------

_PRIM_SIZES = {
    # 1 byte
    "u8": 1, "s8": 1, "char": 1, "signed char": 1, "unsigned char": 1,
    "bool": 1, "char8_t": 1,
    # 2 byte
    "u16": 2, "s16": 2, "short": 2, "short int": 2, "signed short": 2,
    "signed short int": 2, "unsigned short": 2, "unsigned short int": 2,
    "char16_t": 2, "wchar_t": 2,
    # 4 byte (PPC32: pointers, long, int, float are all 4)
    "u32": 4, "s32": 4, "f32": 4, "float": 4, "int": 4, "signed": 4,
    "signed int": 4, "unsigned": 4, "unsigned int": 4, "uint": 4,
    "long": 4, "long int": 4, "signed long": 4, "signed long int": 4,
    "unsigned long": 4, "unsigned long int": 4, "int32_t": 4, "uint32_t": 4,
    "BOOL": 4, "UNKWORD": 4, "size_t": 4, "ptrdiff_t": 4, "intptr_t": 4,
    "uintptr_t": 4, "char32_t": 4,
    # 8 byte
    "u64": 8, "s64": 8, "f64": 8, "double": 8, "long long": 8,
    "long long int": 8, "signed long long": 8, "unsigned long long": 8,
    "unsigned long long int": 8, "int64_t": 8, "uint64_t": 8,
}

_CV_QUALIFIERS = {"const", "volatile", "constexpr", "mutable", "register"}
_ELABORATED = {"struct", "class", "union", "enum", "typename"}

# Verdict severities (higher = worse; default table sorts worst first).
SEV_OVERLAP = 100
SEV_NON_MONOTONIC = 90
SEV_UNDERSIZED = 100
SEV_OVERLAP = 90
SEV_EXTENT_MISMATCH = 85
SEV_NON_MONOTONIC = 80
SEV_NAME_COMMENT_MISMATCH = 70
SEV_TOTAL_SIZE = 40
SEV_MISSING_OFFSET = 10
SEV_ALIGN_PAD = 5
SEV_OVERLAP_VIEW = 25  # giant-struct-in-small-slot, likely a deliberate view
SEV_BIG_HOLE = 20  # multi-KB unmapped region, not a member shift


# ---------------------------------------------------------------------------
# Tokenizer (line-tagged; // comments preserved, block comments dropped)
# ---------------------------------------------------------------------------

@dataclass
class Tok:
    kind: str  # 'id' | 'num' | 'str' | 'sym' | 'comment'
    text: str
    line: int


_TOKEN_RE = re.compile(
    r"(?P<id>[A-Za-z_][A-Za-z0-9_]*)"
    r"|(?P<num>0[xX][0-9a-fA-F]+|\d[\w.]*)"
    r"|(?P<str>\"(?:\\.|[^\"\\])*\")"
    r"|(?P<ch>'(?:\\.|[^'\\])*')"
    r"|(?P<sym>::|->|\.\.\.|<<|>>|<=|>=|==|!=|&&|\|\||\+\+|--|[-+*/%&|^~!?=<>#{}\(\)\[\];:,\.])"
)

_PACK_PUSH_RE = re.compile(r"pack\s*\(\s*push\s*(?:,\s*(\d+))?\s*\)")
_PACK_POP_RE = re.compile(r"pack\s*\(\s*pop\s*\)")
_PACK_SET_RE = re.compile(r"pack\s*\(\s*(\d+)\s*\)")
_PACK_RESET_RE = re.compile(r"pack\s*\(\s*\)")


def _emit_block(toks: list[Tok], comments: dict[int, list[str]],
               parts: list[str], start_line: int):
    text = "".join(parts).strip()
    if text:
        comments.setdefault(start_line, []).append(("block", text))
        toks.append(Tok("comment", text, start_line))


def tokenize_file(path: Path):
    """Tokenize a file. Returns (tokens, comments_by_line, pack_ops).

    pack_ops is a list of (line, op) with op in ('push', n) / ('pop',) /
    ('set', n) / ('reset',) in file order.
    """
    text = path.read_text(encoding="utf-8", errors="replace")
    lines = text.split("\n")
    toks: list[Tok] = []
    comments: dict[int, list[str]] = {}
    pack_ops: list[tuple[int, tuple]] = []
    in_block = False
    in_block_start = 0
    in_block_text: list[str] = []
    for lineno0, raw in enumerate(lines):
        lineno = lineno0 + 1
        line = raw
        stripped = line.strip()
        if not in_block and stripped.startswith("#"):
            m = _PACK_PUSH_RE.search(stripped)
            if m:
                pack_ops.append((lineno, ("push", int(m.group(1)) if m.group(1) else 8)))
            elif _PACK_POP_RE.search(stripped):
                pack_ops.append((lineno, ("pop",)))
            elif _PACK_SET_RE.search(stripped):
                pack_ops.append((lineno, ("set", int(_PACK_SET_RE.search(stripped).group(1)))))
            elif _PACK_RESET_RE.search(stripped):
                pack_ops.append((lineno, ("reset",)))
            continue
        pos = 0
        mangled = []
        # strip block comments (possibly multi-line), keep // comments as token.
        # block comment text is ALSO recorded (same-line /* 0xNN */ offsets).
        while True:
            if in_block:
                end = line.find("*/", pos)
                if end == -1:
                    in_block_text.append(line[pos:])
                    pos = len(line)
                    break
                in_block_text.append(line[pos:end])
                in_block = False
                _emit_block(toks, comments, in_block_text, in_block_start)
                in_block_text = []
                pos = end + 2
                continue
            start_block = line.find("/*", pos)
            start_line = line.find("//", pos)
            if start_line != -1 and (start_block == -1 or start_line < start_block):
                mangled.append(line[pos:start_line])
                comments.setdefault(lineno, []).append(("line", line[start_line + 2:].strip()))
                toks.append(Tok("comment", line[start_line + 2:].strip(), lineno))
                pos = len(line)
                break
            if start_block == -1:
                mangled.append(line[pos:])
                break
            mangled.append(line[pos:start_block])
            end = line.find("*/", start_block + 2)
            if end == -1:
                in_block = True
                in_block_start = lineno
                in_block_text = [line[start_block + 2:]]
                break
            block_text = line[start_block + 2:end]
            comments.setdefault(lineno, []).append(("block", block_text.strip()))
            toks.append(Tok("comment", block_text.strip(), lineno))
            pos = end + 2
        code = "".join(mangled)
        for m in _TOKEN_RE.finditer(code):
            kind = m.lastgroup
            toks.append(Tok(kind, m.group(), lineno))
    return toks, comments, pack_ops


# ---------------------------------------------------------------------------
# Constant-expression evaluator for array extents (hex/dec arithmetic only)
# ---------------------------------------------------------------------------

class _ConstExpr:
    def __init__(self, text: str):
        self.toks = re.findall(r"0[xX][0-9a-fA-F]+|\d+|<<|>>|[+\-*/%()&|^]", text)
        self.pos = 0

    def peek(self):
        return self.toks[self.pos] if self.pos < len(self.toks) else None

    def next(self):
        t = self.peek()
        self.pos += 1
        return t

    def parse(self):
        if not self.toks:
            return None
        v = self._or()
        return v if self.peek() is None else None

    def _num(self):
        t = self.next()
        if t is None:
            raise ValueError("eof")
        if t.startswith(("0x", "0X")):
            return int(t, 16)
        if t.isdigit():
            return int(t)
        raise ValueError(f"bad token {t}")

    def _atom(self):
        if self.peek() == "(":
            self.next()
            v = self._or()
            if self.next() != ")":
                raise ValueError("unbalanced")
            return v
        if self.peek() == "-":
            self.next()
            return -self._atom()
        if self.peek() == "+":
            self.next()
            return self._atom()
        return self._num()

    def _mul(self):
        v = self._atom()
        while self.peek() in ("*", "/", "%"):
            op = self.next()
            r = self._atom()
            v = {"*": v * r, "/": v // r if r else None, "%": v % r if r else None}[op]
            if v is None:
                raise ValueError("div0")
        return v

    def _add(self):
        v = self._mul()
        while self.peek() in ("+", "-"):
            op = self.next()
            r = self._mul()
            v = v + r if op == "+" else v - r
        return v

    def _shift(self):
        v = self._add()
        while self.peek() in ("<<", ">>"):
            op = self.next()
            r = self._add()
            v = v << r if op == "<<" else v >> r
        return v

    def _and(self):
        v = self._shift()
        while self.peek() == "&":
            self.next()
            v = v & self._shift()
        return v

    def _xor(self):
        v = self._and()
        while self.peek() == "^":
            self.next()
            v = v ^ self._and()
        return v

    def _or(self):
        v = self._xor()
        while self.peek() == "|":
            self.next()
            v = v | self._xor()
        return v


def eval_const_expr(text: str):
    """Evaluate an integer constant expression; None if not pure."""
    try:
        v = _ConstExpr(text).parse()
    except (ValueError, IndexError, OverflowError):
        return None
    return v if isinstance(v, int) and v >= 0 else None


SIZEOF_RE = re.compile(r"sizeof\s*\(([^()]*)\)")


def eval_extent(text: str, reg, ctx) -> int | None:
    """Evaluate an array extent, resolving sizeof(T) for known types."""
    t = text or ""
    for _ in range(4):
        m = SIZEOF_RE.search(t)
        if not m:
            break
        r = _resolve_base(_normalize_type(m.group(1)), reg, ctx, frozenset())
        if r is None:
            return None
        t = t[: m.start()] + str(r[0]) + t[m.end():]
    if "sizeof" in t:
        return None
    return eval_const_expr(t)


# ---------------------------------------------------------------------------
# Records
# ---------------------------------------------------------------------------

@dataclass
class Member:
    name: str
    type_str: str  # normalized C++ type, e.g. 'u32', 'CVec3*', 'u8[4]'
    base_type: str  # without array dims
    array_dims: list[str] = field(default_factory=list)
    bits: str | None = None  # bitfield width expr
    declared: int | None = None  # declared offset (comment wins over name)
    decl_source: str = ""  # 'comment' | 'name' | ''
    size: int | None = None  # computed later
    computed: int | None = None  # computed later
    line: int = 0
    comment: str = ""
    is_union: bool = False
    union_size: int | None = None
    union_align: int = 1
    uncertain: bool = False  # True when layout cursor re-synced / base unknown
    size_partial: bool = False  # numeric size from a partially-resolved type
    weak_declared: int | None = None  # name-looks-like-offset but counter-suspect
    end_extra: int | None = None  # silent N-extent alternative (never reported)
    same_prev: bool = False  # same `;` statement as previous member (a, b, c;)
    stmt_end: int | None = None  # range end from the statement's line comment
    range_end: int | None = None  # end hex from a `0xA-0xB` / `..0xB` comment
    conflict: dict = field(default_factory=dict)  # distinct declared candidates


@dataclass
class StructRec:
    qname: str
    bare: str
    kind: str  # struct | class | union
    path: Path
    start_line: int
    open_idx: int = -1
    close_idx: int = -1
    close_line: int = 0
    bases: list[str] = field(default_factory=list)
    pack: int = 8
    members: list[Member] = field(default_factory=list)
    has_virtual: bool = False
    aliases: dict[str, str] = field(default_factory=dict)
    size_claim: int | None = None
    size_claim_min: bool = False  # True for 'min size:' style claims
    size_claim_line: int = 0
    trailing_decls: list[tuple[str, list[str]]] = field(default_factory=list)
    # layout cache
    _layout: tuple | None = None


# Offset comment: first 0xNN in the trailing comment is the declared offset.
OFFSET_RE = re.compile(r"0[xX]([0-9a-fA-F]+)")
# Name-encoded offsets: unk15DC / pad_74 / field_30 / field_0xC / _pad1C / pad2A.
NAME_OFF_RE = re.compile(
    r"^(?:unk|pad|padding|gap|field|mPtr|mPad|mField)_?(?:0[xX])?([0-9a-fA-F]+)$"
)
# Single-letter prefixes (f17CC, w18?): counters collide (`f6`, `fd`), so
# require at least 3 hex digits. Union arms never reach this check.
NAME1_OFF_RE = re.compile(r"^(?:f|w|u|q|s)_?(?:0[xX])?([0-9a-fA-F]{3,})$")
RANGE_RE = re.compile(
    r"0[xX]([0-9a-fA-F]+)\s*(?:\.\.|-)\s*0[xX]([0-9a-fA-F]+)"
)
END_ONLY_RE = re.compile(r"^\s*[.\-\s]*\.\.\s*0[xX]([0-9a-fA-F]+)")
SIZE_COMMENT_RE = re.compile(
    r"(?P<min>min(?:imum)?\s+)?size\s*:\s*(?P<val>0[xX][0-9a-fA-F]+|\d+)(?P<q>\?)?",
    re.IGNORECASE,
)
STATIC_ASSERT_RE = re.compile(
    r"static_assert\s*\(\s*sizeof\s*\(\s*([\w:]+)\s*\)\s*==\s*(0[xX][0-9a-fA-F]+|\d+)"
)


def offset_from_comment(comment: str):
    m = OFFSET_RE.search(comment or "")
    return int(m.group(1), 16) if m else None


def offset_from_name(name: str):
    m = NAME_OFF_RE.match(name or "")
    if not m:
        return None
    try:
        return int(m.group(1), 16)
    except ValueError:
        return None


# ---------------------------------------------------------------------------
# Phase A: find every struct/class/union/enum definition
# ---------------------------------------------------------------------------

_DECLSPEC_RE = re.compile(r"__(attribute|declspec)\s*\(\s*\(.*?\)\s*\)")


def _strip_declspec(text: str) -> str:
    return _DECLSPEC_RE.sub("", text)


@dataclass
class _EnumRec:
    qname: str
    bare: str
    size: int = 4


def find_definitions(toks: list[Tok], path: Path, pack_ops):
    """Walk tokens; return (structs, enums, file_aliases, file_static_asserts).

    Records every struct/class/union definition at any depth (namespace,
    global, function-local, nested) with its body token range.
    """
    structs: list[StructRec] = []
    enums: list[_EnumRec] = []
    by_pos: dict[tuple[str, int], StructRec] = {}
    ns_stack: list[str] = []
    struct_stack: list[str] = []  # bare names of enclosing structs
    brace_stack: list[tuple[str, object]] = []  # (kind, payload)
    pending_ns: str | None = None
    pending_struct: tuple[str, str, int] | None = None  # (kind, name-or-'', idx)
    pending_enum: tuple[str, int] | None = None
    i = 0
    n = len(toks)
    ops = sorted(pack_ops)
    op_ptr = 0
    pack_stack = [8]

    def cur_pack():
        return pack_stack[-1]

    def apply_ops(upto_line: int):
        nonlocal op_ptr
        while op_ptr < len(ops) and ops[op_ptr][0] <= upto_line:
            op = ops[op_ptr][1]
            if op[0] == "push":
                pack_stack.append(op[1])
            elif op[0] == "pop" and len(pack_stack) > 1:
                pack_stack.pop()
            elif op[0] == "set":
                pack_stack[-1] = op[1]
            elif op[0] == "reset":
                pack_stack[-1] = 8
            op_ptr += 1

    def toks_text(a: int, b: int) -> str:
        return " ".join(t.text for t in toks[a:b])

    while i < n:
        t = toks[i]
        apply_ops(t.line)
        if t.kind == "id" and t.text == "namespace" and not pending_struct and not pending_enum:
            # namespace Foo {  or  namespace {  -- find the '{'
            j = i + 1
            name = ""
            if j < n and toks[j].kind == "id":
                name = toks[j].text
                j += 1
            # skip `= alias` (namespace alias) -- not a scope
            if j < n and toks[j].text == "=":
                i = j + 1
                while i < n and toks[i].text != ";":
                    i += 1
                i += 1
                continue
            if j < n and toks[j].text == "{":
                pending_ns = name
                i = j
                continue
            i += 1
            continue
        if t.kind == "id" and t.text in ("struct", "class", "union") and pending_struct is None and pending_enum is None:
            kind = t.text
            j = i + 1
            while j < n and toks[j].kind == "comment":
                j += 1
            # skip compiler decorations between the keyword and the name:
            # __declspec(novtable) / __attribute__((...)) / alignas(...)
            while j < n and toks[j].kind == "id" and toks[j].text in (
                    "__attribute__", "__declspec", "_declspec", "alignas",
                    "__unaligned"):
                j += 1
                if j < n and toks[j].text == "(":
                    depth = 0
                    while j < n:
                        if toks[j].text == "(":
                            depth += 1
                        elif toks[j].text == ")":
                            depth -= 1
                            if depth <= 0:
                                j += 1
                                break
                        j += 1
                while j < n and toks[j].kind == "comment":
                    j += 1
            name = ""
            # optional name (may be qualified in elaborated contexts; take last)
            if j < n and toks[j].kind == "id" and toks[j].text not in ("{", ":", ";"):
                # could be __attribute__/__declspec -- skip macro-ish id + (...)
                if toks[j].text in ("__attribute__", "__declspec", "_declspec"):
                    depth = 0
                    j += 1
                    while j < n:
                        if toks[j].text == "(":
                            depth += 1
                        elif toks[j].text == ")":
                            depth -= 1
                            if depth <= 0:
                                j += 1
                                break
                        j += 1
                else:
                    parts = [toks[j].text]
                    j += 1
                    while j + 1 < n and toks[j].text == "::" and toks[j + 1].kind == "id":
                        parts.append(toks[j + 1].text)
                        j += 2
                    name = parts[-1]
            # bases / template-args / attributes until '{' or ';'
            k = j
            header_toks = []
            angle = 0
            paren = 0
            is_fwd = False
            while k < n:
                tx = toks[k].text
                if tx == "<":
                    angle += 1
                elif tx == ">":
                    angle = max(0, angle - 1)
                elif tx == "(":
                    paren += 1
                elif tx == ")":
                    paren = max(0, paren - 1)
                if angle == 0 and paren == 0 and tx in ("{", ";"):
                    is_fwd = tx == ";"
                    break
                if toks[k].kind != "comment":
                    header_toks.append(toks[k])
                k += 1
            if is_fwd:
                i = k + 1
                continue
            if k < n and toks[k].text == "{":
                pending_struct = (kind, name, i)
                i = k  # let '{' handling below create the record
                continue
            i = k
            continue
        if t.kind == "id" and t.text == "enum" and pending_struct is None and pending_enum is None:
            j = i + 1
            while j < n and toks[j].kind == "comment":
                j += 1
            if j < n and toks[j].text in ("class", "struct"):
                j += 1
            while j < n and toks[j].kind == "comment":
                j += 1
            name = ""
            if j < n and toks[j].kind == "id":
                name = toks[j].text
                j += 1
            # find '{' or ';'
            k = j
            while k < n and toks[k].text not in ("{", ";"):
                k += 1
            if k < n and toks[k].text == "{":
                pending_enum = (name, i)
                i = k
                continue
            i = k + 1
            continue
        if t.text == "{":
            if pending_ns is not None:
                brace_stack.append(("ns", pending_ns))
                if pending_ns:
                    ns_stack.append(pending_ns)
                pending_ns = None
            elif pending_struct is not None:
                kind, name, kw_idx = pending_struct
                pending_struct = None
                if not name:
                    name = f"anon@{path.name}:{toks[kw_idx].line}"
                qname = "::".join([*(ns_stack), *[s for s in struct_stack], name])
                rec = StructRec(
                    qname=qname, bare=name, kind=kind, path=path,
                    start_line=toks[kw_idx].line, open_idx=i + 1, pack=cur_pack(),
                )
                # bases from header toks between name and '{'
                structs.append(rec)
                by_pos[(str(path), toks[kw_idx].line)] = rec
                brace_stack.append(("struct", rec))
                struct_stack.append(name)
                # stash raw header text for base parsing
                rec._header_text = toks_text(kw_idx + 1, i)  # type: ignore[attr-defined]
            elif pending_enum is not None:
                name, kw_idx = pending_enum
                pending_enum = None
                if name:
                    qname = "::".join([*(ns_stack), *[s for s in struct_stack], name])
                    # underlying type after ':' ?
                    enums.append(_EnumRec(qname=qname, bare=name))
                brace_stack.append(("enum", None))
            else:
                brace_stack.append(("other", None))
            i += 1
            continue
        if t.text == "}":
            if brace_stack:
                kind, payload = brace_stack.pop()
                if kind == "ns" and payload and ns_stack and ns_stack[-1] == payload:
                    ns_stack.pop()
                elif kind == "struct":
                    rec = payload
                    rec.close_idx = i
                    rec.close_line = t.line
                    if struct_stack:
                        struct_stack.pop()
                    # trailing declarators `} name, name2[4];`
                    j = i + 1
                    trail = []
                    while j < n and toks[j].text != ";":
                        trail.append(toks[j])
                        j += 1
                    rec.trailing_decls = _parse_trailing(trail)
                    i = j + 1
                    continue
            i += 1
            continue
        i += 1
    # parse bases now (needs full header text)
    for rec in structs:
        rec.bases = _parse_bases(getattr(rec, "_header_text", ""))
    return structs, enums, by_pos


def _parse_trailing(trail: list[Tok]):
    """Parse `name, name2[4]` declarators after a closing brace."""
    trail = [t for t in trail if t.kind != "comment"]
    out: list[tuple[str, list[str]]] = []
    cur: list[Tok] = []
    groups = [cur]
    depth = 0
    for t in trail:
        if t.text in ("(", "[", "{"):
            depth += 1
        elif t.text in (")", "]", "}"):
            depth = max(0, depth - 1)
        if t.text == "," and depth == 0:
            cur = []
            groups.append(cur)
            continue
        cur.append(t)
    for g in groups:
        # strip attributes/macros: keep last id + bracket dims
        ids = [t for t in g if t.kind == "id"]
        if not ids:
            continue
        name = ids[-1].text
        dims = _bracket_groups(g)
        out.append((name, dims))
    return out


def _bracket_groups(toks_list: list[Tok]) -> list[str]:
    dims: list[str] = []
    j = 0
    while j < len(toks_list):
        if toks_list[j].text == "[":
            depth = 1
            k = j + 1
            buf = []
            while k < len(toks_list) and depth:
                if toks_list[k].text == "[":
                    depth += 1
                elif toks_list[k].text == "]":
                    depth -= 1
                    if not depth:
                        break
                buf.append(toks_list[k].text)
                k += 1
            dims.append(" ".join(buf))
            j = k + 1
        else:
            j += 1
    return dims


def _parse_bases(header_text: str) -> list[str]:
    """Extract base-class names from the text between the struct name and '{'."""
    text = _strip_declspec(header_text)
    if ":" not in text:
        return []
    after = text.split(":", 1)[1]
    # split top-level commas (respect <> () )
    parts, depth_a, depth_p, cur = [], 0, 0, []
    for ch in after:
        if ch == "<":
            depth_a += 1
        elif ch == ">":
            depth_a = max(0, depth_a - 1)
        elif ch == "(":
            depth_p += 1
        elif ch == ")":
            depth_p = max(0, depth_p - 1)
        if ch == "," and depth_a == 0 and depth_p == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(ch)
    parts.append("".join(cur))
    bases = []
    for p in parts:
        p = re.sub(r"\b(virtual|public|protected|private|struct|class)\b", " ", p)
        p = p.strip()
        p = re.sub(r"<.*", "", p).strip()  # template base -> unresolvable; drop args
        p = p.strip(": ")
        if p and re.fullmatch(r"[A-Za-z_][\w:]*", p):
            bases.append(p.split("::")[-1])
    return bases


# ---------------------------------------------------------------------------
# Phase B: parse one struct body into members
# ---------------------------------------------------------------------------

def _split_top_commas(toks_list: list[Tok]):
    """Split on top-level commas; <> (templates) nest like brackets."""
    groups: list[list[Tok]] = [[]]
    d = {"(": 0, "[": 0, "{": 0, "<": 0}
    pairs = {")": "(", "]": "[", "}": "{", ">": "<"}
    for t in toks_list:
        if t.text in d:
            d[t.text] += 1
        elif t.text == ">>":
            d["<"] = max(0, d["<"] - 2)
        elif t.text in pairs:
            d[pairs[t.text]] = max(0, d[pairs[t.text]] - 1)
        if t.text == "," and all(v == 0 for v in d.values()):
            groups.append([])
            continue
        groups[-1].append(t)
    return groups


def _strip_top_init(dtoks: list[Tok]):
    """Cut a trailing top-level `= ...` initializer (call AFTER paren check)."""
    depth = 0
    for idx, t in enumerate(dtoks):
        if t.text in ("(", "[", "{"):
            depth += 1
        elif t.text in (")", "]", "}"):
            depth = max(0, depth - 1)
        if depth == 0 and t.text == "=":
            return dtoks[:idx]
    return dtoks


def _has_top_paren(toks_list: list[Tok]) -> bool:
    d = 0
    for t in toks_list:
        if t.text in ("(", "[", "{"):
            d += 1
        elif t.text in (")", "]", "}"):
            d = max(0, d - 1)
        elif t.text == "(" and d == 0:
            pass
        if t.text == "(" and d == 1:
            # check whether this paren opened at top level: depth before incr
            pass
    # simpler second pass
    depth = 0
    for t in toks_list:
        if t.text == "(" and depth == 0:
            return True
        if t.text in ("(", "[", "{"):
            depth += 1
        elif t.text in (")", "]", "}"):
            depth = max(0, depth - 1)
    return False


def _parse_declarator(dtoks: list[Tok], base_type: str | None):
    """Parse one declarator (no top-level commas). Returns (name, type, dims, bits)."""
    # strip default init: top-level '=' ... or top-level '{...}'
    cut = None
    depth = 0
    for idx, t in enumerate(dtoks):
        if t.text in ("(", "[", "{"):
            depth += 1
        elif t.text in (")", "]", "}"):
            depth = max(0, depth - 1)
        if depth == 0 and t.text in ("=", "{"):
            cut = idx
            break
    core = dtoks[:cut] if cut is not None else dtoks
    # bitfield width: top-level ':' ...  (watch out for '::')
    bits = None
    depth = 0
    for idx, t in enumerate(core):
        if t.text in ("(", "[", "{"):
            depth += 1
        elif t.text in (")", "]", "}"):
            depth = max(0, depth - 1)
        if t.text == ":" and depth == 0:
            nxt = core[idx + 1] if idx + 1 < len(core) else None
            if nxt is None or nxt.text != ":":
                bits = " ".join(x.text for x in core[idx + 1 :])
                core = core[:idx]
                break
    # array dims: trailing top-level [...] groups
    dims: list[str] = []
    while len(core) >= 2 and core[-1].text == "]":
        depth = 0
        k = len(core) - 1
        while k >= 0:
            if core[k].text == "]":
                depth += 1
            elif core[k].text == "[":
                depth -= 1
                if depth == 0:
                    break
            k -= 1
        if k < 0:
            break
        dims.insert(0, " ".join(t.text for t in core[k + 1 : -1]))
        core = core[:k]
    # name = last identifier; leading '*'/'&' attach to type
    name = None
    name_idx = -1
    for idx in range(len(core) - 1, -1, -1):
        if core[idx].kind == "id" and core[idx].text not in _ELABORATED:
            name = core[idx].text
            name_idx = idx
            break
    if name is None:
        return None
    stars = "".join(t.text for t in core[name_idx + 1 :] if t.text in ("*", "&"))
    if base_type is None:
        type_toks = [t.text for t in core[:name_idx] if t.text not in _CV_QUALIFIERS]
        # drop elaborated keyword (struct/class/union/enum prefix)
        if type_toks and type_toks[0] in _ELABORATED:
            type_toks = type_toks[1:]
        base_type = " ".join(type_toks).replace(" :: ", "::")
        base_type = re.sub(r"\s+([*&])", r"\1", base_type).strip()
    full_type = (base_type + stars) if base_type else stars
    full_type = full_type.replace(" :: ", "::").strip()
    return name, full_type, dims, bits


def parse_body(toks: list[Tok], a: int, b: int, comments: dict[int, list[str]],
               outer_qname: str, by_pos: dict, path: Path):
    """Parse struct body tokens [a, b) into (members, has_virtual, aliases, enums).

    Nested type definitions are skipped here (recorded in phase A); a trailing
    declarator after a nested definition becomes a member of the nested type.
    """
    members: list[Member] = []
    aliases: dict[str, str] = {}
    local_enums: list[_EnumRec] = []
    has_virtual = False
    i = a
    n = b

    def skip_balanced(idx: int, op: str, cl: str):
        depth = 0
        while idx < n:
            if toks[idx].text == op:
                depth += 1
            elif toks[idx].text == cl:
                depth -= 1
                if depth == 0:
                    return idx + 1
            idx += 1
        return idx

    while i < n:
        # skip access labels / stray semicolons / lone qualifiers / comments
        if toks[i].text == ";":
            i += 1
            continue
        if toks[i].kind == "comment":
            i += 1
            continue
        if (toks[i].kind == "id" and toks[i].text in ("public", "private", "protected")
                and i + 1 < n and toks[i + 1].text == ":"):
            i += 2
            continue
        # template<...> : skip angle list, parse what follows normally
        if toks[i].kind == "id" and toks[i].text == "template" and i + 1 < n and toks[i + 1].text == "<":
            j = i + 1
            depth = 0
            while j < n:
                if toks[j].text == "<":
                    depth += 1
                elif toks[j].text == ">":
                    depth -= 1
                    if depth == 0:
                        j += 1
                        break
                j += 1
            i = j
            continue
        # typedef / using-alias
        if toks[i].kind == "id" and toks[i].text in ("typedef", "using"):
            j = i
            while j < n and toks[j].text != ";":
                if toks[j].text == "{":
                    j = skip_balanced(j, "{", "}")
                else:
                    j += 1
            stmt = [t for t in toks[i + 1 : j] if t.kind != "comment"]
            if toks[i].text == "typedef":
                for grp in _split_top_commas(stmt):
                    if _has_top_paren(grp):
                        continue  # function-pointer typedef: size unknown, ignore
                    r = _parse_declarator(grp, None)
                    if r:
                        nm, ty, _dd, _bb = r
                        aliases[nm] = ty
            else:  # using X = T;
                eq = [k for k, t in enumerate(stmt) if t.text == "=" and True]
                # top-level '=' only
                depth = 0
                eqpos = -1
                for k, t in enumerate(stmt):
                    if t.text in ("(", "[", "{", "<"):
                        pass
                    if t.text == "=" and depth == 0:
                        eqpos = k
                        break
                    if t.text in ("(", "[", "{"):
                        depth += 1
                    elif t.text in (")", "]", "}"):
                        depth = max(0, depth - 1)
                if eqpos > 0 and stmt[0].kind == "id":
                    aliases[stmt[0].text] = " ".join(t.text for t in stmt[eqpos + 1 :]).replace(" :: ", "::")
            i = j + 1
            continue
        if toks[i].kind == "id" and toks[i].text == "friend":
            j = i
            while j < n and toks[j].text != ";" and toks[j].text != "{":
                j += 1
            if j < n and toks[j].text == "{":
                j = skip_balanced(j, "{", "}")
            else:
                j += 1
            i = j
            continue
        if toks[i].kind == "id" and toks[i].text == "static_assert":
            j = i
            while j < n and toks[j].text != ";":
                j += 1
            i = j + 1
            continue
        # nested type definition? (consumes through `;`; may yield members)
        nested = _try_nested(toks, i, n, skip_balanced, by_pos, path,
                             outer_qname, comments)
        if nested is not None:
            i, new_members, nested_aliases, nested_enums = nested
            members.extend(new_members)
            aliases.update(nested_aliases)
            local_enums.extend(nested_enums)
            continue
        # enum definition?
        if toks[i].kind == "id" and toks[i].text == "enum":
            j = i + 1
            if j < n and toks[j].text in ("class", "struct"):
                j += 1
            ename = ""
            if j < n and toks[j].kind == "id":
                ename = toks[j].text
                j += 1
            usize = 4
            if j < n and toks[j].text == ":":
                j += 1
                ut: list[str] = []
                while j < n and toks[j].text != "{":
                    ut.append(toks[j].text)
                    j += 1
                usize = _PRIM_SIZES.get(" ".join(ut).strip(), 4)
            if j < n and toks[j].text == "{":
                j = skip_balanced(j, "{", "}")
                if ename:
                    qn = (outer_qname + "::" + ename) if outer_qname else ename
                    local_enums.append(_EnumRec(qname=qn, bare=ename, size=usize))
                tstart = j
                while j < n and toks[j].text != ";":
                    j += 1
                eline = toks[j].line if j < n else toks[i].line
                for nm, dims in _parse_trailing(toks[tstart:j]):
                    mm = Member(name=nm, type_str="int", base_type="int",
                                array_dims=dims, line=eline)
                    _assign_declared(mm, comments)
                    members.append(mm)
                i = j + 1
                continue
            # else: `enum Foo x;` -- fall through to generic parsing
        # generic declaration: find terminator (';' or top-level '{')
        j = i
        brace_at = -1
        depth = 0
        while j < n:
            tx = toks[j].text
            if tx in ("(", "[", "{"):
                if tx == "{" and depth == 0:
                    brace_at = j
                    break
                depth += 1
            elif tx in (")", "]", "}"):
                depth = max(0, depth - 1)
            elif tx == ";" and depth == 0:
                break
            j += 1
        if brace_at != -1:
            stmt = toks[i:brace_at]
            if any(t.kind == "id" and t.text == "virtual" for t in stmt):
                has_virtual = True
            i = skip_balanced(brace_at, "{", "}")
            if i < n and toks[i].text == ";":
                i += 1
            continue
        if j >= n:
            break
        stmt = [t for t in toks[i:j] if t.kind != "comment"]
        semi_line = toks[j].line
        if (stmt and stmt[0].kind == "id"
                and stmt[0].text in ("static", "thread_local", "extern")):
            i = j + 1  # static data: not part of the instance layout
            continue
        if _has_top_paren(stmt):
            if any(t.kind == "id" and t.text == "virtual" for t in stmt):
                has_virtual = True
        elif len(stmt) == 2 and stmt[0].kind == "id" and stmt[1].text == ":":
            pass  # stray label
        else:
            groups = _split_top_commas(_strip_top_init(stmt))
            first = _parse_declarator(groups[0], None)
            if first is not None and first[1]:
                _, shared_ty, _, _ = first
                made: list[Member] = []
                for grp in groups:
                    r = _parse_declarator(grp, shared_ty)
                    if r is None or not r[1]:
                        continue
                    nm, ty, dd, bb = r
                    mm = Member(name=nm, type_str=ty, base_type=ty,
                                array_dims=dd, bits=bb, line=semi_line,
                                comment=" // ".join(t for _k, t in comments.get(semi_line, [])))
                    _assign_declared(mm, comments, use_comment=(not made))
                    made.append(mm)
                if len(made) > 1:
                    ends = [e for _k, c in (comments.get(semi_line, []) or [])
                            for e in [_comment_range_end(c)] if e is not None]
                    for k in range(1, len(made)):
                        made[k].same_prev = True
                        made[k].stmt_end = ends[0] if ends else None
                    made[0].stmt_end = ends[0] if ends else None
                members.extend(made)
        i = j + 1
    return members, has_virtual, aliases, local_enums


OF_REL_RE = re.compile(r"0[xX][0-9a-fA-F]+\s+of\s+(the|each|its|this)\b", re.IGNORECASE)


def _comment_range_end(c: str):
    _s, e = _comment_candidates(c)
    return e


def _comment_candidates(text: str):
    """Split one comment into (start_hex_or_None, range_end_or_None).

    The start hex must sit at the very front of the comment (offset-first
    convention); a leading `..0xB` marks an end-only range (`// ..0x86C`).
    Anything at/above 0x1000000 is an address/magic, not a member offset.
    A hex qualified as "of the <thing>" is relative, not an offset.
    """
    c = text or ""
    me = END_ONLY_RE.match(c)
    if me:
        try:
            v = int(me.group(1), 16)
        except ValueError:
            return None, None
        return None, (v if v < 0x1000000 else None)
    if OF_REL_RE.search(c):
        return None, None
    start = None
    m = OFFSET_RE.search(c)
    if m and m.start() <= 6:
        try:
            v = int(m.group(1), 16)
        except ValueError:
            v = None
        if v is not None and v < 0x1000000:
            start = v
    end = None
    r = RANGE_RE.search(c)
    if r:
        try:
            v = int(r.group(2), 16)
        except ValueError:
            v = None
        if v is not None and v < 0x1000000:
            end = v
    return start, end


def _assign_declared(mm: Member, comments: dict[int, list], use_comment: bool = True):
    """Fill mm.declared by voting over block/line/name offset candidates.

    Priority on conflict: block comment > member name > line comment (line
    comments routinely carry bit numbers, addresses and sizes that are not
    the member offset). Conflicts are recorded on mm.conflict for the
    NAME_COMMENT_MISMATCH verdict. Single-hex-digit decimal-looking names
    (pad2, unk1, f6) are counter-suspect and stored as mm.weak_declared.
    A name that may give an END (`u8 gap_B0[0xB0-0x04]`) yields a silent
    mm.end_extra alternative for the neighbor fit (never reported alone).
    """
    starts: dict[int, set] = {}
    order: list[int] = []
    ends: list[int] = []
    if use_comment:
        line_comments = comments.get(mm.line, [])
    else:
        # shared `;` line: the trailing comment belongs to the first
        # declarator; later ones only get name-derived offsets here (any
        # sequential spread happens post-layout).
        line_comments = []
    for kind, c in line_comments:
        s, e = _comment_candidates(c)
        if s is not None and s not in starts:
            starts[s] = set()
            order.append(s)
        if s is not None:
            starts[s].add("block" if kind == "block" else "comment")
        if e is not None:
            ends.append(e)
    hexpart = None
    m = NAME_OFF_RE.match(mm.name or "")
    if m:
        hexpart = m.group(1)
    else:
        m1 = NAME1_OFF_RE.match(mm.name or "")
        if m1:
            hexpart = m1.group(1)
    if hexpart is not None:
        try:
            v = int(hexpart, 16)
        except ValueError:
            v = None
        if v is not None and v < 0x1000000:
            if len(hexpart) == 1 and hexpart.isdigit():
                mm.weak_declared = v
            elif v not in starts:
                starts[v] = {"name"}
                order.append(v)
            else:
                starts[v].add("name")
            if len(mm.array_dims) == 1:
                e0 = eval_const_expr(mm.array_dims[0])
                if e0 is not None and 0 < e0 < v:
                    mm.end_extra = v - e0
    mm.range_end = ends[0] if ends else None
    if not starts:
        mm.declared, mm.decl_source, mm.conflict = None, "", {}
        return
    if len(starts) == 1:
        v = order[0]
        mm.declared = v
        mm.decl_source = "+".join(sorted(starts[v]))
        mm.conflict = {}
        return
    for want in ("block", "name", "comment"):
        for v in order:
            if want in starts[v]:
                mm.declared = v
                mm.decl_source = want + "!"
                mm.conflict = {x: sorted(starts[x]) for x in order}
                return


def _try_nested(toks: list[Tok], i: int, n: int, skip_balanced, by_pos: dict,
                path: Path, outer_qname: str, comments: dict[int, list[str]]):
    """Consume a nested struct/class/union definition starting at toks[i].

    Returns (new_i, members, aliases, enums) or None when toks[i] is not a
    definition (caller falls through to generic declaration parsing). Pure
    named type definitions yield nothing; `} var;` yields a member of the
    nested type; anonymous unions yield a single union member; anonymous
    structs splice their members inline (they ARE direct members).
    """
    if not (toks[i].kind == "id" and toks[i].text in ("struct", "class", "union")):
        return None
    nkind = toks[i].text
    kw_line = toks[i].line
    j = i + 1
    if j < n and toks[j].kind == "id" and toks[j].text not in ("{", ":"):
        j += 1  # nested name
    k = j
    angle = paren = 0
    found = None
    while k < n:
        tx = toks[k].text
        if tx == "<":
            angle += 1
        elif tx == ">":
            angle = max(0, angle - 1)
        elif tx == "(":
            paren += 1
        elif tx == ")":
            paren = max(0, paren - 1)
        if angle == 0 and paren == 0:
            if tx in ("{", ";"):
                found = tx
                break
            if tx in ("(", ")", ",", "="):
                return None  # not a definition (e.g. elaborated use)
        k += 1
    if found != "{":
        return None  # forward decl or `struct Foo* p;` -> generic path
    open_idx = k
    k = skip_balanced(k, "{", "}")
    close_idx = k - 1
    tstart = k
    while k < n and toks[k].text != ";":
        k += 1
    trail = toks[tstart:k]
    semi_line = toks[k].line if k < n else kw_line
    k += 1
    nrec = by_pos.get((str(path), kw_line))
    decls = _parse_trailing(trail) if trail else []
    out: list[Member] = []
    sub_aliases: dict[str, str] = {}
    sub_enums: list = []
    if nrec is not None and decls:
        for nm, dims in decls:
            mm = Member(name=nm or nrec.bare, type_str=nrec.qname,
                        base_type=nrec.qname, array_dims=dims, line=semi_line)
            _assign_declared(mm, comments)
            out.append(mm)
    elif nrec is not None and not decls and nkind == "union":
        mm = Member(name=f"union@{kw_line}", type_str=nrec.qname,
                    base_type=nrec.qname, line=kw_line)
        _assign_declared(mm, comments)
        out.append(mm)
    elif not decls and nkind in ("struct", "class"):
        # anonymous aggregate: members live directly in the parent
        sub_members, _hv, sub_aliases, sub_enums = parse_body(
            toks, open_idx + 1, close_idx, comments, outer_qname,
            by_pos, path)
        out.extend(sub_members)
    # pure nested type definition (no declarator): no member here; the
    # nested record itself is parsed in the all-records pass.
    return k, out, sub_aliases, sub_enums


class Registry:
    def __init__(self):
        self.structs: dict[str, list[StructRec]] = {}
        self.bare: dict[str, list[str]] = {}
        self.enums: dict[str, int] = {}
        self.enum_bare: dict[str, list[str]] = {}
        self.file_aliases: dict[str, dict[str, str]] = {}
        self.static_asserts: dict[str, dict[str, int]] = {}
        self.incomplete: set[str] = set()

    def add_struct(self, rec: StructRec):
        self.structs.setdefault(rec.qname, []).append(rec)
        self.bare.setdefault(rec.bare, [])
        if rec.qname not in self.bare[rec.bare]:
            self.bare[rec.bare].append(rec.qname)

    def canonical(self, qname: str):
        """Preferred definition for sizing: most data members, tie -> first."""
        lst = self.structs.get(qname, [])
        if not lst:
            return None
        return max(lst, key=lambda r: (len(r.members), r.start_line))

    def all_structs(self):
        for lst in self.structs.values():
            for rec in lst:
                yield rec

    def add_enum(self, qname: str, bare: str, size: int):
        if qname not in self.enums:
            self.enums[qname] = size
        self.enum_bare.setdefault(bare, []).append(qname)

    def lookup(self, name: str, ctx_qname: str):
        """Resolve a type name to a qualified struct name (or None)."""
        if "::" in name:
            if name in self.structs:
                return name
            short = name.split("::")[-1]
            return self._bare_unique(short)
        # walk outward through the context scopes
        scopes = ctx_qname.split("::") if ctx_qname else []
        for depth in range(len(scopes), -1, -1):
            cand = "::".join(scopes[:depth] + [name]) if depth else name
            if cand in self.structs:
                return cand
        return self._bare_unique(name)

    def lookup_enum(self, name: str, ctx_qname: str):
        if "::" in name and name in self.enums:
            return self.enums[name]
        scopes = ctx_qname.split("::") if ctx_qname else []
        for depth in range(len(scopes), -1, -1):
            cand = "::".join(scopes[:depth] + [name]) if depth else name
            if cand in self.enums:
                return self.enums[cand]
        lst = self.enum_bare.get(name.split("::")[-1], [])
        if len(lst) == 1:
            return self.enums[lst[0]]
        return None

    def _bare_unique(self, name: str):
        lst = self.bare.get(name, [])
        if len(lst) == 1:
            return lst[0]
        return None


def _normalize_type(t: str) -> str:
    t = re.sub(r"\s+", " ", t or "").strip()
    t = t.replace(" :: ", "::")
    t = re.sub(r"\s+([*&])", r"\1", t)
    return t


def _resolve_base(t: str, reg: Registry, ctx: StructRec, seen: frozenset):
    """Resolve a non-pointer, non-array base type to (size, align)."""
    t = _normalize_type(t)
    if not t or t == "void":
        return None
    if t.endswith("*") or t.endswith("&"):
        return (4, 4)  # PPC32 pointers/references
    if "<" in t or ">" in t or "(" in t:
        return None  # template-dependent / function type: unknown
    if t in _PRIM_SIZES:
        s = _PRIM_SIZES[t]
        return (s, min(s, 4))
    # alias chains (struct scope, then file scope)
    if t in seen:
        return None
    if ctx is not None and t in ctx.aliases:
        return _resolve_base(ctx.aliases[t], reg, ctx, seen | {t})
    falt = reg.file_aliases.get(str(ctx.path) if ctx is not None else "", {})
    if t in falt:
        return _resolve_base(falt[t], reg, ctx, seen | {t})
    esz = reg.lookup_enum(t, ctx.qname if ctx else "")
    if esz is not None:
        return (esz, min(esz, 4))
    qn = reg.lookup(t, ctx.qname if ctx else "")
    if qn is None:
        return None
    target = reg.canonical(qn)
    if target is None:
        return None
    lay = struct_layout(target, reg, seen)
    if lay is None:
        return None
    return (lay["size"], lay["align"])


def _resolve_member_size(mm: Member, reg: Registry, ctx: StructRec,
                          seen: frozenset = frozenset()):
    t = _normalize_type(mm.base_type)
    anchor = t
    if anchor.endswith("*") or anchor.endswith("&"):
        base, palign = 4, 4
    else:
        r = _resolve_base(anchor, reg, ctx, seen)
        if r is None:
            return None
        base, palign = r
    mult = 1
    for d in mm.array_dims:
        v = eval_extent(d, reg, ctx)
        if v is None:
            return None
        mult *= v
    if base == 0 or mult == 0:
        return None  # empty-struct member / zero-length array: unknown, not 0
    return base * mult


def struct_layout(rec: StructRec, reg: Registry, seen: frozenset):
    """Compute (size, align, has_vptr, base_end, members) for a struct.

    Cycle-guarded: returns None when the struct (transitively) contains
    itself by value. Results are memoized on the record.
    """
    if rec._layout is not None:
        return rec._layout
    if rec.qname in seen:
        return None
    if rec.kind == "union":
        return _union_layout(rec, reg, seen)
    children = seen | {rec.qname}
    pack = rec.pack or 8
    cursor = 0
    maxalign = 1
    base_has_vptr = False
    base_ok = True
    for b in rec.bases:
        qn = reg.lookup(b, rec.qname)
        if qn is None:
            base_ok = False
            continue
        blay = struct_layout(reg.canonical(qn), reg, children)
        if blay is None:
            base_ok = False
            continue
        bal = min(blay["align"], pack)
        cursor = _align_up(cursor, bal)
        cursor += blay["size"]
        maxalign = max(maxalign, bal)
        base_has_vptr = base_has_vptr or blay["has_vptr"]
    has_vptr = base_has_vptr or rec.has_virtual
    if rec.has_virtual and not base_has_vptr:
        cursor = _align_up(cursor, 4)
        cursor += 4
        maxalign = max(maxalign, 4)
    base_end = cursor
    bit_run: tuple | None = None  # (type_str, bits_used, type_size)
    for mm in rec.members:
        if mm.bits is not None:
            w = eval_const_expr(mm.bits)
            r = _resolve_base(mm.base_type, reg, rec, children)
            if r is None or w is None:
                mm.size = None
                mm.computed = cursor
                bit_run = None
                continue
            bsize, bal = r
            tbits = bsize * 8
            if bit_run is not None and bit_run[0] == mm.base_type and bit_run[1] + w <= tbits:
                mm.size = 0
                mm.computed = cursor
                bit_run = (mm.base_type, bit_run[1] + w, bsize)
            else:
                bal = min(bal, pack)
                cursor = _align_up(cursor, bal)
                mm.size = bsize
                mm.computed = cursor
                cursor += bsize
                maxalign = max(maxalign, bal)
                bit_run = (mm.base_type, w, bsize)
            continue
        bit_run = None
        if mm.is_union:
            r = _union_size(mm, rec, reg, children)
            if r is None:
                mm.size = None
                mm.computed = cursor
                continue
            usize, ual = r
            ual = min(ual, pack)
            cursor = _align_up(cursor, ual)
            mm.computed = cursor
            mm.size = usize
            cursor += usize
            maxalign = max(maxalign, ual)
            continue
        sz = _resolve_member_size(mm, reg, rec, children)
        if sz is None:
            mm.size = None
            mm.computed = cursor
            continue
        mm.size_partial = not _type_complete(mm.base_type, reg, rec, children)
        r = _resolve_base(mm.base_type, reg, rec, children)
        al = min(r[1] if r else 1, pack)
        cursor = _align_up(cursor, al)
        mm.computed = cursor
        mm.size = sz
        cursor += sz
        maxalign = max(al, maxalign)
    struct_align = min(maxalign, pack)
    total = _align_up(cursor, struct_align)
    complete = base_ok and all(m.size is not None for m in rec.members)
    lay = {"size": total, "align": struct_align, "has_vptr": has_vptr,
           "base_end": base_end, "base_ok": base_ok, "complete": complete}
    rec._layout = lay
    return lay


def _align_up(v: int, a: int) -> int:
    a = a or 1
    return ((v + a - 1) // a) * a


def _union_size(mm: Member, ctx: StructRec, reg: Registry, seen: frozenset):
    """Size/align of an anonymous-union member from its recorded arms."""
    qn = reg.lookup(mm.base_type, ctx.qname)
    if qn is None:
        return None
    nrec = reg.canonical(qn)
    if nrec is None:
        return None
    pack = nrec.pack or 8
    best = 0
    maxalign = 1
    for arm in getattr(nrec, "_arms", []):
        if arm.get("anon"):
            sub = reg.canonical(arm["anon"])
            if sub is None:
                return None
            lay = struct_layout(sub, reg, seen | {nrec.qname})
            if lay is None:
                return None
            best = max(best, lay["size"])
            maxalign = max(maxalign, lay["align"])
            continue
        r = _resolve_base(arm["type"], reg, ctx, seen | {nrec.qname})
        if r is None:
            return None
        base, _al = r
        mult = 1
        for d in arm.get("dims", []):
            v = eval_extent(d, reg, ctx)
            if v is None:
                return None
            mult *= v
        best = max(best, base * mult)
        maxalign = max(maxalign, _al)
    ual = min(maxalign, pack)
    return (_align_up(best, ual), ual)


def _union_layout(rec: StructRec, reg: Registry, seen: frozenset):
    """Union layout: size = max arm size (aligned up), used by struct_layout."""
    # NOTE: no `rec.qname in seen` check here -- struct_layout already guards
    # entry (memo + seen); this function only extends seen for arm recursion.
    if rec._layout is not None:
        return rec._layout
    seen = seen | {rec.qname}
    pack = rec.pack or 8
    arms = getattr(rec, "_arms", None)
    if not arms:
        return None
    best = 0
    maxalign = 1
    for arm in arms:
        if arm.get("anon"):
            sub = reg.canonical(arm["anon"])
            lay = struct_layout(sub, reg, seen) if sub is not None else None
            if lay is None:
                return None
            best = max(best, lay["size"])
            maxalign = max(maxalign, lay["align"])
            continue
        r = _resolve_base(arm["type"], reg, rec, seen)
        if r is None:
            return None
        base, al = r
        mult = 1
        for d in arm.get("dims", []):
            v = eval_extent(d, reg, rec)
            if v is None:
                return None
            mult *= v
        best = max(best, base * mult)
        maxalign = max(maxalign, al)
    ual = min(maxalign, pack)
    lay = {"size": _align_up(best, ual), "align": ual, "has_vptr": False,
           "base_end": 0, "base_ok": True, "complete": True}
    rec._layout = lay
    return lay


def _parse_union_arms(toks: list[Tok], a: int, b: int, by_pos: dict, path: Path):
    """Split a union body into arm descriptors for size computation."""
    arms: list[dict] = []
    i = a
    n = b
    while i < n:
        if toks[i].text == ";":
            i += 1
            continue
        # statement extent: to ';' at depth 0 (respecting nested braces)
        j = i
        depth = 0
        while j < n:
            tx = toks[j].text
            if tx in ("(", "[", "{"):
                depth += 1
            elif tx in (")", "]", "}"):
                depth = max(0, depth - 1)
            elif tx == ";" and depth == 0:
                break
            j += 1
        stmt = [t for t in toks[i:j] if t.kind != "comment"]
        i = j + 1
        if not stmt:
            continue
        if stmt[0].kind == "id" and stmt[0].text in ("struct", "class", "union"):
            kw_line = stmt[0].line
            # find nested '{' ... '}' then trailing declarators
            k = 0
            angle = paren = 0
            open_at = -1
            while k < len(stmt):
                tx = stmt[k].text
                if tx == "<":
                    angle += 1
                elif tx == ">":
                    angle = max(0, angle - 1)
                elif tx == "(":
                    paren += 1
                elif tx == ")":
                    paren = max(0, paren - 1)
                if angle == 0 and paren == 0 and tx == "{":
                    open_at = k
                    break
                k += 1
            if open_at == -1:
                continue
            depth2 = 0
            close_at = -1
            for k2 in range(open_at, len(stmt)):
                if stmt[k2].text == "{":
                    depth2 += 1
                elif stmt[k2].text == "}":
                    depth2 -= 1
                    if depth2 == 0:
                        close_at = k2
                        break
            trail = stmt[close_at + 1:] if close_at != -1 else []
            nrec = by_pos.get((str(path), kw_line))
            if nrec is None:
                continue
            decls = _parse_trailing(trail) if trail else []
            if decls:
                for _nm, dims in decls:
                    arms.append({"type": nrec.qname, "dims": dims})
            elif stmt[0].text == "union":
                arms.append({"type": nrec.qname, "dims": []})
            else:
                arms.append({"anon": nrec.qname})
            continue
        if stmt[0].kind == "id" and stmt[0].text in (
                "typedef", "using", "friend", "static_assert", "public",
                "private", "protected", "template", "enum"):
            continue
        if _has_top_paren(stmt):
            continue
        groups = _split_top_commas(stmt)
        first = _parse_declarator(groups[0], None)
        if first is None or not first[1]:
            continue
        _, shared_ty, _, _ = first
        for grp in groups:
            r = _parse_declarator(grp, shared_ty)
            if r is None or not r[1]:
                continue
            _nm, ty, dd, bb = r
            arms.append({"type": ty, "dims": dd})
    return arms


def inherit_union_offsets(reg: Registry):
    """Give an offset-less union-typed member the unanimous arm offset.

    Anonymous unions (`union { u8 unk3378[4]; s16 field_3378; };`) carry no
    declarator of their own, but every arm starts at the same offset, so
    when all annotated arms agree the member inherits it (source 'inherit').
    Only union-kind targets: struct members are sequential, not overlaid.
    """
    for rec in reg.all_structs():
        for m in rec.members:
            if m.declared is not None or m.conflict or m.bits is not None:
                continue
            base = _normalize_type(m.base_type)
            if base.endswith("*") or base.endswith("&"):
                continue
            qn = reg.lookup(base, rec.qname)
            if qn is None:
                continue
            target = reg.canonical(qn)
            if target is None or target.kind != "union":
                continue
            offs = {a.declared for a in target.members if a.declared is not None}
            if len(offs) == 1:
                m.declared = next(iter(offs))
                m.decl_source = "inherit"


def resolve_conflicts_by_fit(reg: Registry):
    """Resolve offset-candidate conflicts by neighbor fit.

    When block/name/comment disagree, the reading that overlaps neither
    neighbor (where sizes are known) wins; among those, the tightest fit.
    Clean resolutions clear the conflict silently; genuine ambiguity keeps
    the NAME_COMMENT_MISMATCH verdict (spans then use block>name>comment).
    """
    order = {"block": 0, "name": 1, "comment": 2, "name-end": 3}
    tiebreak = {"block": 0, "comment": 1, "name": 2, "name-end": 3}
    for rec in reg.all_structs():
        members = rec.members
        bounds = {mm.declared for mm in members if mm.declared is not None}
        # fixpoint: resolving one member creates clean anchors for the next.
        # Only NON-conflicted neighbors constrain a fit: conflicted ones
        # still hold stale priority picks that would poison the test.
        for _round in range(5):
            changed = False
            for idx, m in enumerate(members):
                if m.declared is None:
                    continue
                if not m.conflict and m.end_extra is None:
                    continue
                prev = next((members[k] for k in range(idx - 1, -1, -1)
                             if members[k].declared is not None
                             and not members[k].conflict), None)
                nxt = next((members[k] for k in range(idx + 1, len(members))
                            if members[k].declared is not None
                            and not members[k].conflict), None)
                if prev is None and nxt is None:
                    continue
                if m.conflict:
                    cands = sorted(m.conflict.keys(),
                                   key=lambda v: (min(order[s] for s in m.conflict[v]), v))
                elif m.end_extra is not None:
                    # end-named pads need corroboration: the name value must
                    # appear as another member's candidate/offset, else the
                    # "end" reading is just a cheaper left hole.
                    name_val = m.declared
                    others = set(bounds)
                    others.discard(name_val)
                    for mm2 in members:
                        if mm2 is not m and mm2.conflict:
                            others.update(mm2.conflict.keys())
                    if name_val not in others:
                        m.end_extra = None
                        continue
                    cands = [m.declared, m.end_extra]
                else:
                    continue

                def _known_size(mm):
                    s = mm.size
                    if s is None or getattr(mm, "size_partial", False):
                        return None
                    return s

                def _pad_like(mm):
                    base = _normalize_type(mm.base_type)
                    if base not in ("u8", "s8", "char", "unsigned char"):
                        return False
                    return bool(re.search(r"(?i)pad|gap|fill", mm.name or ""))

                def fits(v: int) -> bool:
                    if prev is not None:
                        ps = _known_size(prev)
                        if ps is not None:
                            if v < prev.declared + ps:
                                if not (_pad_like(m) and m.size and v + m.size in bounds):
                                    return False
                        elif v < prev.declared:
                            return False
                    if nxt is not None:
                        ms = _known_size(m)
                        if ms is not None:
                            if nxt.declared != v and nxt.declared < v + ms:
                                return False
                        elif nxt.declared < v:
                            return False
                    return True

                passing = [v for v in cands if fits(v)]
                if not passing:
                    continue

                def slack(v: int) -> int:
                    s = 0
                    if prev is not None:
                        ps = _known_size(prev)
                        base = prev.declared + (ps or 0)
                        s += max(0, v - base)
                    if nxt is not None:
                        fill = 0
                        for k in range(idx + 1, len(members)):
                            if members[k] is nxt:
                                break
                            ks = _known_size(members[k])
                            if ks is not None:
                                fill += ks
                        ms = _known_size(m)
                        span = nxt.declared - v
                        eff = span - fill
                        if ms is not None:
                            s += abs(eff - ms)
                        else:
                            s += max(0, -eff)
                        # strong preference: member end landing exactly on the
                        # next declared offset (tight packing / end-named pad)
                        if ms and v + ms == nxt.declared:
                            s -= 0x10000000
                    return s

                if m.conflict:
                    best = min(passing, key=lambda v: (
                        slack(v), min(tiebreak[s] for s in m.conflict[v])))
                else:
                    best = min(passing, key=lambda v: (
                        slack(v), 0 if v == m.declared else 1))
                if best != m.declared:
                    changed = True
                    m.declared = best
                    if m.conflict and best in m.conflict:
                        m.decl_source = min(m.conflict[best], key=lambda s: order[s])
                    else:
                        m.decl_source = "name-end"
                m.conflict = {}
                m.end_extra = None
            if not changed:
                break


def spread_stmt_offsets(reg: Registry):
    """Spread one `;` statement's offset over comma-separated declarators.

    `float f5C, f60, f64, f68, f6C; // +0x5C..0x6C` puts every member on the
    same source line, so all would inherit the range start. Instead assign
    sequentially (prev declared + prev computed size); when the statement
    carries a range end, keep the spread only if the last member lands on it.
    Runs after layout (needs member sizes).
    """
    for rec in reg.all_structs():
        mems = rec.members
        i = 0
        while i < len(mems):
            j = i
            while j + 1 < len(mems) and mems[j + 1].same_prev:
                j += 1
            if j > i:
                seqd: list[Member] = []
                for k in range(i + 1, j + 1):
                    prev_m, m = mems[k - 1], mems[k]
                    if (m.declared is None and m.bits is None
                            and prev_m.bits is None and prev_m.declared is not None
                            and prev_m.size is not None):
                        m.declared = prev_m.declared + prev_m.size
                        m.decl_source = "seq"
                        seqd.append(m)
                end = mems[j].stmt_end
                if end is not None and seqd and mems[j].declared != end:
                    for m in seqd:
                        m.declared = None
                        m.decl_source = ""
                i = j + 1
            else:
                i += 1


def _parse_file_aliases(toks: list[Tok]):
    """Collect file-scope `typedef` / `using X =` aliases (brace depth 0)."""
    aliases: dict[str, str] = {}
    i = 0
    n = len(toks)
    depth = 0
    while i < n:
        t = toks[i]
        if t.text in ("{",):
            depth += 1
            i += 1
            continue
        if t.text == "}":
            depth = max(0, depth - 1)
            i += 1
            continue
        if depth == 0 and t.kind == "id" and t.text in ("typedef", "using"):
            j = i + 1
            while j < n and toks[j].text != ";":
                j += 1
            stmt = [t for t in toks[i + 1 : j] if t.kind != "comment"]
            if t.text == "typedef":
                for grp in _split_top_commas(stmt):
                    if _has_top_paren(grp):
                        continue
                    r = _parse_declarator(grp, None)
                    if r and r[1]:
                        aliases[r[0]] = r[1]
            else:
                dd2 = 0
                eqpos = -1
                for k2, tt in enumerate(stmt):
                    if tt.text == "=" and dd2 == 0:
                        eqpos = k2
                        break
                    if tt.text in ("(", "[", "{"):
                        dd2 += 1
                    elif tt.text in (")", "]", "}"):
                        dd2 = max(0, dd2 - 1)
                if eqpos > 0 and stmt[0].kind == "id":
                    aliases[stmt[0].text] = " ".join(x.text for x in stmt[eqpos + 1 :]).replace(" :: ", "::")
            i = j + 1
            continue
        i += 1
    return aliases


# ---------------------------------------------------------------------------
# Size claims
# ---------------------------------------------------------------------------

def attach_size_claims(rec: StructRec, file_lines: list[str],
                       file_asserts: dict[str, int]):
    if rec.bare in file_asserts:
        rec.size_claim = file_asserts[rec.bare]
        rec.size_claim_min = False
        return
    for ln in range(max(1, rec.start_line - 6), rec.start_line):
        if ln - 1 < len(file_lines):
            m = SIZE_COMMENT_RE.search(file_lines[ln - 1])
            if m and not m.group("q"):
                try:
                    rec.size_claim = int(m.group("val"), 0)
                except ValueError:
                    continue
                rec.size_claim_min = bool(m.group("min"))
                rec.size_claim_line = ln
    # closing-brace trailing comment
    for c in _comments_on_line(rec, file_lines):
        m = SIZE_COMMENT_RE.search(c)
        if m and not m.group("q"):
            try:
                rec.size_claim = int(m.group("val"), 0)
            except ValueError:
                continue
            rec.size_claim_min = bool(m.group("min")) or rec.size_claim_min
            rec.size_claim_line = rec.close_line
            return
        if re.fullmatch(r"\s*(0[xX][0-9a-fA-F]+|\d+)\s*(total|bytes)?\s*", c or ""):
            try:
                rec.size_claim = int(c.strip().split()[0], 0)
            except ValueError:
                continue
            rec.size_claim_line = rec.close_line
            return


def _comments_on_line(rec: StructRec, file_lines: list[str]):
    if 1 <= rec.close_line <= len(file_lines):
        line = file_lines[rec.close_line - 1]
        idx = line.find("//")
        if idx != -1:
            return [line[idx + 2 :].strip()]
    return []


# ---------------------------------------------------------------------------
# Audit
# ---------------------------------------------------------------------------

def audit_struct(rec: StructRec, rel: str, reg=None):
    """Return findings for one struct (only if it carries offset annotations)."""
    if sum(1 for m in rec.members if m.declared is not None) < 2:
        return []
    out: list[dict] = []
    # comment/name/block disagreements (resolved block > name > comment)
    for m in rec.members:
        if m.conflict:
            parts = ", ".join(f"0x{v:X} ({'/'.join(srcs)})"
                                   for v, srcs in m.conflict.items())
            out.append({
                "class": rec.qname, "header": rel, "member": m.name,
                "declared": f"0x{m.declared:X}", "span": "",
                "sizeof": _fmt_size(m.size),
                "verdict": "NAME_COMMENT_MISMATCH",
                "severity": SEV_NAME_COMMENT_MISMATCH,
                "line": m.line,
                "detail": (f"offset candidates disagree ({parts}); "
                             f"using 0x{m.declared:X} (block > name > comment)"),
                "hole": 0,
            })
    decl_idx = [i for i, m in enumerate(rec.members) if m.declared is not None]
    boundaries = {m.declared for m in rec.members if m.declared is not None}
    # array extent vs the member's own `0xA-0xB` comment range:
    # flag only a double contradiction (extent matches neither the range
    # nor the span to the next annotated member).
    for pos, i in enumerate(decl_idx):
        a = rec.members[i]
        if (len(a.array_dims) == 1 and a.size is not None
                and not a.size_partial
                and a.range_end is not None and a.range_end >= a.declared):
            e = eval_const_expr(a.array_dims[0])
            if e is None:
                continue
            base_one = _normalize_type(a.base_type) in _PRIM_SIZES and _PRIM_SIZES.get(_normalize_type(a.base_type), 0) == 1
            if not base_one:
                continue  # T[N] extents are element counts, not bytes
            opts = {a.range_end - a.declared, a.range_end - a.declared + 1}
            nxt = rec.members[decl_idx[pos + 1]] if pos + 1 < len(decl_idx) else None
            span = (nxt.declared - a.declared) if nxt is not None else None
            if e not in opts and (nxt is None or e != span):
                out.append({
                    "class": rec.qname, "header": rel, "member": a.name,
                    "declared": f"0x{a.declared:X}",
                    "span": f"0x{span:X}" if span is not None else "",
                    "sizeof": _fmt_size(a.size),
                    "verdict": "EXTENT_MISMATCH",
                    "severity": SEV_EXTENT_MISMATCH,
                    "line": a.line,
                    "detail": (f"array extent 0x{e:X} matches neither its comment "
                                 f"range (0x{a.declared:X}-0x{a.range_end:X}) nor "
                                 f"the span to {nxt.name + ' ' if nxt else ''}"
                                 f"({('0x%X' % span) if span is not None else 'no next'})"),
                    "hole": abs(e - (span if span is not None else e)),
                })
    # fill-aware spans between consecutive annotated members
    for pos in range(len(decl_idx) - 1):
        i, j = decl_idx[pos], decl_idx[pos + 1]
        a, b = rec.members[i], rec.members[j]
        if a.bits is not None or b.bits is not None:
            continue
        if a.size is None or a.size_partial:
            continue  # unknown or partially-resolved size: never flag spans
        fill = 0
        for k in range(i + 1, j):
            if rec.members[k].size is not None:
                fill += rec.members[k].size
        span = b.declared - a.declared
        if span == 0:
            continue  # intentional overlay (register-view style structs)
        if span < 0:
            if b.size is not None and b.declared + b.size in boundaries:
                continue  # backwards overlay pad ending on a real boundary
            out.append({
                "class": rec.qname, "header": rel, "member": a.name,
                "declared": f"0x{a.declared:X}", "span": f"0x{span:X}",
                "sizeof": _fmt_size(a.size),
                "verdict": "NON_MONOTONIC", "severity": SEV_NON_MONOTONIC,
                "line": b.line,
                "detail": (f"{b.name} declared at 0x{b.declared:X} is not after "
                           f"{a.name} at 0x{a.declared:X}"),
                "hole": -span,
            })
            continue
        eff = span - fill
        if eff < a.size:
            if a.declared + a.size in boundaries and a.declared + a.size != b.declared:
                continue  # forward overlay ending exactly on a real boundary
            ov = a.size - eff
            sev, note = SEV_OVERLAP, ""
            if (not a.array_dims and reg is not None
                    and _is_named_type(a, rec, reg)
                    and ov * 4 >= a.size * 3):
                sev, note = SEV_OVERLAP_VIEW, " (likely intentional view-prefix?)"
            fill_note = f", 0x{fill:X} intervening" if fill else ""
            eff_s = f"0x{eff:X}" if eff >= 0 else f"-0x{-eff:X}"
            out.append({
                "class": rec.qname, "header": rel, "member": a.name,
                "declared": f"0x{a.declared:X}", "span": f"0x{span:X}",
                "sizeof": _fmt_size(a.size),
                "verdict": "OVERLAP", "severity": sev,
                "line": b.line,
                "detail": (f"{a.name} needs 0x{a.size:X} but only {eff_s} "
                           f"fits before {b.name} (span 0x{span:X}"
                           f"{fill_note}){note}"),
                "hole": ov,
            })
        elif eff > a.size:
            hole = eff - a.size
            if (reg is not None and not a.array_dims
                    and _type_incomplete(a, rec, reg)):
                continue  # named type is known-partial; tail lives past it
            rb = _resolve_base_cached(b, rec)
            bal = min(rb[1], rec.pack or 8) if rb else 4
            if hole <= 3 and b.declared % bal == 0:
                out.append({
                    "class": rec.qname, "header": rel, "member": a.name,
                    "declared": f"0x{a.declared:X}", "span": f"0x{span:X}",
                    "sizeof": _fmt_size(a.size),
                    "verdict": "ALIGN_PAD", "severity": SEV_ALIGN_PAD,
                    "line": b.line,
                    "detail": (f"{hole}-byte gap before {b.name} looks like "
                               f"natural alignment padding"),
                    "hole": hole,
                })
                continue
            sev, note = SEV_UNDERSIZED, ""
            if hole > 0x1000:
                sev, note = SEV_BIG_HOLE, " (large unmapped region, not a member shift?)"
            out.append({
                "class": rec.qname, "header": rel, "member": a.name,
                "declared": f"0x{a.declared:X}", "span": f"0x{span:X}",
                "sizeof": _fmt_size(a.size),
                "verdict": "UNDER_SIZED", "severity": sev,
                "line": a.line,
                "detail": (f"{a.name} is 0x{a.size:X} but the gap to "
                           f"{b.name} is 0x{span:X} "
                           f"(undeclared hole 0x{hole:X}){note}"),
                "hole": hole,
            })
    # missing offsets
    for m in rec.members:
        if m.declared is None and m.bits is None:
            out.append({
                "class": rec.qname, "header": rel, "member": m.name,
                "declared": "", "span": "",
                "sizeof": _fmt_size(m.size),
                "verdict": "MISSING_OFFSET", "severity": SEV_MISSING_OFFSET,
                "line": m.line,
                "detail": f"{m.name} has no offset annotation",
                "hole": 0,
            })
    # total size claim: only a member sticking out past it is actionable
    # (a smaller layout usually means unannotated tail, not a bug).
    if rec.size_claim is not None and rec.members:
        last = rec.members[-1]
        if last.declared is not None and last.size:
            last_end = last.declared + last.size
            if last_end > rec.size_claim:
                out.append(_total_finding(rec, rel, last, last_end,
                                          f"last member ends at 0x{last_end:X}, "
                                          f"past stated size 0x{rec.size_claim:X}"))
    return out


def _type_incomplete(mm: Member, ctx: StructRec, reg) -> bool:
    base = _normalize_type(mm.base_type)
    if base.endswith("*") or base.endswith("&") or base in _PRIM_SIZES:
        return False
    qn = reg.lookup(base, ctx.qname)
    return qn is not None and qn in getattr(reg, "incomplete", set())


def _type_complete(t: str, reg: Registry, ctx: StructRec, seen: frozenset) -> bool:
    """True when a type's size is fully trustworthy (no unknown parts)."""
    t = _normalize_type(t)
    if not t or t in _PRIM_SIZES:
        return True
    if t.endswith("*") or t.endswith("&"):
        return True
    if "<" in t or ">" in t or "(" in t:
        return False
    if t in seen:
        return False
    seen = seen | {t}
    if ctx is not None and t in ctx.aliases:
        return _type_complete(ctx.aliases[t], reg, ctx, seen)
    falt = reg.file_aliases.get(str(ctx.path) if ctx is not None else "", {})
    if t in falt:
        return _type_complete(falt[t], reg, ctx, seen)
    if reg.lookup_enum(t, ctx.qname if ctx else "") is not None:
        return True
    qn = reg.lookup(t, ctx.qname if ctx else "")
    if qn is None:
        return False
    target = reg.canonical(qn)
    if target is None:
        return False
    lay = struct_layout(target, reg, seen)
    return bool(lay is not None and lay.get("complete", False))


def _is_named_type(mm: Member, ctx: StructRec, reg) -> bool:
    base = _normalize_type(mm.base_type)
    if base.endswith("*") or base.endswith("&"):
        return False
    if base in _PRIM_SIZES:
        return False
    return reg.lookup(base, ctx.qname) is not None


def _total_finding(rec, rel, last, last_end, why):
    return {
        "class": rec.qname, "header": rel, "member": last.name,
        "declared": f"0x{last.declared:X}", "span": "",
        "sizeof": _fmt_size(last.size),
        "verdict": "TOTAL_SIZE", "severity": SEV_TOTAL_SIZE,
        "line": rec.close_line,
        "detail": (f"stated size 0x{rec.size_claim:X} disagrees: {why}"),
        "hole": abs(rec.size_claim - last_end),
    }


def _fmt_size(s):
    return f"0x{s:X}" if isinstance(s, int) else "size?"


_resolve_cache: dict = {}


def _resolve_base_cached(mm: Member, ctx: StructRec):
    key = (ctx.qname, mm.base_type)
    if key not in _resolve_cache:
        _resolve_cache[key] = _resolve_base(mm.base_type, _ACTIVE_REG, ctx,
                                            frozenset())
    return _resolve_cache[key]


_ACTIVE_REG: Registry | None = None


# ---------------------------------------------------------------------------
# Scan driver
# ---------------------------------------------------------------------------

EXTS = {".hpp", ".h", ".cpp"}


VENDORED_EXCLUDES = [
    "libs/nw4r/*", "libs/RVL_SDK/*", "libs/CriWare/*",
    "libs/PowerPC_EABI_Support/*", "libs/NdevExi2A/*",
]


def scan_files(roots: list[str], excludes: list[str], min_sev: int = 0):
    """Parse all headers/sources under roots. Returns (Registry, findings)."""
    global _ACTIVE_REG
    reg = Registry()
    _ACTIVE_REG = reg
    files: list[Path] = []
    for r in roots:
        rp = Path(r)
        if rp.is_file():
            files.append(rp)
        elif rp.is_dir():
            for ext in EXTS:
                files.extend(sorted(rp.rglob(f"*{ext}")))
    files = sorted(set(files))
    rel_cache: dict[str, str] = {}

    def rel(p: Path) -> str:
        k = str(p)
        if k not in rel_cache:
            try:
                rel_cache[k] = str(p.relative_to(Path.cwd()))
            except ValueError:
                rel_cache[k] = k
        return rel_cache[k]

    def excluded(p: Path) -> bool:
        rp = rel(p).replace("\\", "/")
        return any(fnmatch.fnmatch(rp, pat) for pat in excludes)

    # pass 1: tokenize + discover + file aliases + static asserts
    per_file: list[tuple[Path, list[Tok], dict, list[StructRec], dict]] = []
    for f in files:
        if excluded(f):
            continue
        try:
            toks, comments, pack_ops = tokenize_file(f)
        except OSError:
            continue
        if not any(t.kind == "id" and t.text in ("struct", "class", "union") for t in toks):
            continue
        structs, enums, by_pos = find_definitions(toks, f, pack_ops)
        if not structs:
            continue
        try:
            raw = f.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        asserts: dict[str, int] = {}
        for m in STATIC_ASSERT_RE.finditer(raw):
            try:
                asserts.setdefault(m.group(1).split("::")[-1], int(m.group(2), 0))
            except ValueError:
                pass
        reg.file_aliases[str(f)] = _parse_file_aliases(toks)
        reg.static_asserts[str(f)] = asserts
        for e in enums:
            reg.add_enum(e.qname, e.bare, e.size)
        for rec in structs:
            reg.add_struct(rec)
        per_file.append((f, toks, comments, structs, by_pos))
    # pass 2: parse bodies + union arms + size claims
    file_lines_cache: dict[str, list[str]] = {}
    for f, toks, comments, structs, by_pos in per_file:
        for rec in structs:
            members, hv, aliases, local_enums = parse_body(
                toks, rec.open_idx, rec.close_idx, comments, rec.qname,
                by_pos, f)
            rec.members = members
            rec.has_virtual = hv
            rec.aliases = aliases
            for e in local_enums:
                reg.add_enum(e.qname, e.bare, e.size)
            if rec.kind == "union":
                rec._arms = _parse_union_arms(toks, rec.open_idx,
                                              rec.close_idx, by_pos, f)
        if str(f) not in file_lines_cache:
            try:
                file_lines_cache[str(f)] = f.read_text(encoding="utf-8",
                                                        errors="replace").split("\n")
            except OSError:
                file_lines_cache[str(f)] = []
        for rec in structs:
            attach_size_claims(rec, file_lines_cache[str(f)],
                               reg.static_asserts.get(str(f), {}))
    # pass 2b: union-offset inheritance (needs all bodies parsed)
    inherit_union_offsets(reg)
    # pass 3: sizes + conflict resolution + audit
    findings: list[dict] = []
    for _f, _t, _c, structs, _b in per_file:
        for rec in structs:
            struct_layout(rec, reg, frozenset())
    spread_stmt_offsets(reg)
    reg.incomplete = set()
    for qn, lst in reg.structs.items():
        for rec in lst:
            lay = rec._layout or {}
            if (rec.size_claim is not None and not rec.size_claim_min
                    and isinstance(lay.get("size"), int)
                    and rec.size_claim > lay["size"]):
                reg.incomplete.add(qn)
                break
    resolve_conflicts_by_fit(reg)
    # mark members above unknown sizes as uncertain (re-sync display)
    for _f, _t, _c, structs, _b in per_file:
        for rec in structs:
            sticky = not (rec._layout or {}).get("base_ok", True)
            for mm in rec.members:
                if mm.size is None:
                    sticky = True
                mm.uncertain = sticky
    for f, _t, _c, structs, _b in per_file:
        for rec in structs:
            findings.extend(audit_struct(rec, rel(f), reg))
    findings = [d for d in findings if d["severity"] >= min_sev]
    findings.sort(key=lambda d: (-d["severity"], -d.get("hole", 0),
                                  d["header"], d["class"], d["member"]))
    return reg, findings, [rel(f) for f, _, _, _, _ in per_file]


# ---------------------------------------------------------------------------
# Reporting
# ---------------------------------------------------------------------------

def print_table(findings: list[dict], out=None):
    out = out or sys.stdout
    if not findings:
        print("layout_audit: no findings -- all annotated structs are consistent.",
              file=out)
        return
    rows = [["class", "header", "member", "declared", "span", "sizeof", "verdict"]]
    for d in findings:
        rows.append([d["class"], d["header"], d["member"], d["declared"],
                     d["span"], d["sizeof"], d["verdict"]])
    widths = [max(len(r[i]) for r in rows) for i in range(7)]
    for ri, r in enumerate(rows):
        print(" | ".join(c.ljust(widths[i]) for i, c in enumerate(r)), file=out)
        if ri == 0:
            print("-+-".join("-" * w for w in widths), file=out)


def print_class(reg: Registry, name: str, out=None):
    out = out or sys.stdout
    qs = ([q for q in reg.structs if q == name]
          + [q for q in reg.structs if q.split("::")[-1] == name and q != name])
    if not qs:
        subs = sorted({q for q in reg.structs if name.lower() in q.lower()})
        print(f"layout_audit: no struct named {name!r}.", file=out)
        if subs[:20]:
            print("did you mean:", file=out)
            for s in subs[:20]:
                print(f"  {s}", file=out)
        return 1
    if len(qs) > 1:
        print(f"layout_audit: {name!r} is ambiguous:", file=out)
        for s in sorted(qs):
            print(f"  {s}", file=out)
        return 1
    for rec in reg.structs[qs[0]]:
        rc = _print_one_class(rec, out, reg)
    return 0


def _print_one_class(rec: StructRec, out, reg=None):
    lay = rec._layout or {}
    print(f"{rec.kind} {rec.qname}  [{rec.path}:{rec.start_line}]", file=out)
    print(f"pack={rec.pack} vptr={'yes' if lay.get('has_vptr') else 'no'} "
          f"bases=[{', '.join(rec.bases) or '-'}] "
          f"layout_size={('0x%X' % lay['size']) if lay.get('size') is not None else '?'} "
          f"claim={('0x%X' % rec.size_claim) if rec.size_claim is not None else '-'}"
          f"{' (min)' if rec.size_claim_min else ''}", file=out)
    print("", file=out)
    hdr = ["member", "type", "declared", "computed", "size", "span>next", "note"]
    rows = [hdr]
    mems = rec.members
    for idx, m in enumerate(mems):
        nxt = mems[idx + 1] if idx + 1 < len(mems) else None
        if nxt is not None and m.declared is not None and nxt.declared is not None and m.size:
            span = f"0x{nxt.declared - m.declared:X}"
        else:
            span = "-"
        comp = ("~" if m.uncertain else "") + (
            f"0x{m.computed:X}" if m.computed is not None else "?")
        decl = ""
        if m.declared is not None:
            init = "".join(sorted({s[0] for s in m.decl_source.replace("!", "").split("+") if s}))
            decl = f"0x{m.declared:X}({init}{'!' if m.conflict else ''})"
        elif m.weak_declared is not None:
            decl = f"~0x{m.weak_declared:X}(w)"
        note = ""
        if m.is_union:
            note = "union"
        if m.range_end is not None:
            note = (note + " " if note else "") + f"range:0x{m.range_end:X}"
        if m.bits is not None:
            note = (note + " " if note else "") + f"bitfield:{m.bits}"
        if m.size is None:
            note = (note + " " if note else "") + "size?"
        rows.append([m.name, m.type_str, decl, comp, _fmt_size(m.size), span,
                     note.strip()])
    widths = [max(len(r[i]) for r in rows) for i in range(7)]
    for ri, r in enumerate(rows):
        print(" | ".join(c.ljust(widths[i]) for i, c in enumerate(r)), file=out)
        if ri == 0:
            print("-+-".join("-" * w for w in widths), file=out)
    print("", file=out)
    try:
        rel = str(rec.path.relative_to(Path.cwd()))
    except ValueError:
        rel = str(rec.path)
    for d in audit_struct(rec, rel, reg):
        print(f"[{d['verdict']}] {d['member']}: {d['detail']}", file=out)
    if not audit_struct(rec, rel, reg):
        print("verdict: CLEAN -- no layout findings for this struct.", file=out)
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description="Audit struct/class member offsets vs sizes.")
    ap.add_argument("--roots", nargs="*", default=["include", "src", "libs"],
                    help="roots to scan (default: include src libs)")
    ap.add_argument("--exclude", nargs="*", default=None,
                      help="fnmatch patterns (repo-relative) to skip "
                           "(default: vendored libs)")
    ap.add_argument("--min-severity", type=int, default=0,
                      help="hide findings below this severity")
    ap.add_argument("--class", dest="klass", default=None,
                    help="print the member table for one struct and exit")
    ap.add_argument("--json", dest="json_path", default=None,
                    help="write machine-readable findings to PATH")
    args = ap.parse_args(argv)
    excludes = args.exclude if args.exclude is not None else VENDORED_EXCLUDES
    reg, findings, files = scan_files(args.roots, excludes, args.min_severity)
    if args.json_path:
        with open(args.json_path, "w") as fh:
            by_verdict: dict[str, int] = {}
            for d in findings:
                by_verdict[d["verdict"]] = by_verdict.get(d["verdict"], 0) + 1
            json.dump({"files_scanned": len(files),
                       "structs": sum(len(v) for v in reg.structs.values()),
                       "findings": findings,
                       "by_verdict": by_verdict}, fh, indent=2)
    if args.klass:
        return print_class(reg, args.klass)
    print_table(findings)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

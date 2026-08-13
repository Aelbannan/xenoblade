#!/usr/bin/env python3
"""struct_layout.py - show C/C++ struct/class layouts (MWCC PowerPC rules).

Computes field offsets/sizes for retail-derived structs in this repo and
cross-checks them against the `/* 0xNN */` / `//0xNN` offset comments and
`/* size */` / `//size:` totals written on those structs, so it doubles as a
layout-consistency checker (offset mismatches usually mean a missing field or
wrong member type).

Run with the project venv:  .venv/bin/python3 tools/struct_layout.py ...

Commands:
  list   <file>...                  enumerate structs/classes/unions in files
  show   <file> <Type>              dump the layout of one type
  search <pattern> [--root DIR]     find files defining matching types
  check  [dir/file...]              lint all structs/classes: report offsets
                                    that contradict retail comments, overlaps,
                                    size mismatches, unplaceable members
  vtable <sym|addr|ClassName>       dump a retail vtable's slot names from the
                                    build/us/asm data splits (RTTI/offset slots
                                    marked; slot symbols are the real methods)

Options (show):
  -r, --recursive        expand nested types (same-file or --include) inline
  -i, --include FILE     extra files to parse for type resolution (bases,
                         nested member types); repeatable
  --sizes JSON           override type sizes, e.g.
                         '{"FixStr<32>":32, "CProc":156, "WORK_ID":4}'
  --json                 machine-readable output (one JSON object on stdout)
  -v, --verbose          diagnostics for unknown types / unresolvable fields

Options (check):
  --include-ctx          also check .ctx.c scaffold files (skipped by default)
  --jobs N               parallel workers (default: CPU count)
  --quiet                print only problem lines
  --json                 machine-readable findings + summary

Bases and member types in other files are resolved automatically by locating
and parsing their headers under src/ libs/ include/ (templates are excluded:
their instantiations carry their own sizes, so comment anchors are used).

Layout rules (MWCC -align powerpc):
  char/bool:1, short:2, int/long/float/ptr:4, long long/double:8; arrays and
  structs align to their member max; bitfields pack into 4-byte units; a
  polymorphic class gets a 4-byte vtable pointer at +0; #pragma pack(push,N)
  is honored per file.
"""

import argparse
import ast
import json
import os
import re
import sys

# --------------------------------------------------------------------------
# Tokenizer
# --------------------------------------------------------------------------

KEYWORDS = {
    "struct", "class", "union", "enum", "typedef", "static", "virtual",
    "const", "volatile", "unsigned", "signed", "inline", "constexpr",
    "mutable", "register", "friend", "explicit", "extern", "thread_local",
    "final", "override",
}

_MULTI_PUNCT = (
    "::", "->", "<<", ">>", "<=", ">=", "==", "!=", "&&", "||", "++", "--",
    "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "->*", ".*", "...",
)


def tokenize(text):
    """Return a list of {kind, value, line} tokens (comments and strings kept)."""
    tokens = []
    i, n = 0, len(text)
    line = 1
    while i < n:
        c = text[i]
        if c == "\n":
            line += 1
            i += 1
        elif c == "#":
            # preprocessor directive (only at line start): skip the whole line
            back = text.rfind("\n", 0, i)
            if text[back + 1:i].strip() == "":
                j = text.find("\n", i)
                if j < 0:
                    j = n
                i = j
            else:
                i += 1
        elif c in " \t\r":
            i += 1
        elif text.startswith("//", i):
            j = text.find("\n", i)
            if j < 0:
                j = n
            tokens.append({"kind": "comment", "value": text[i:j], "line": line})
            i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            if j < 0:
                j = n
            tokens.append({"kind": "comment", "value": text[i : j + 2], "line": line})
            line += text.count("\n", i, j)
            i = j + 2
        elif c in "\"'":
            q = c
            j = i + 1
            while j < n:
                if text[j] == "\\":
                    j += 2
                    continue
                if text[j] == q:
                    j += 1
                    break
                if text[j] == "\n":
                    line += 1
                j += 1
            tokens.append({"kind": "str", "value": text[i:j], "line": line})
            i = j
        else:
            m = re.match(r"[A-Za-z_][A-Za-z0-9_]*", text[i:])
            if m:
                tokens.append({"kind": "id", "value": m.group(0), "line": line})
                i += m.end()
                continue
            m = re.match(r"0[xX][0-9a-fA-F]+|\d+\.\d+(?:[eE][+-]?\d+)?|\.\d+|"
                         r"\d+[uUlL]*", text[i:])
            if m:
                tokens.append({"kind": "num", "value": m.group(0), "line": line})
                i += m.end()
                continue
            for op in _MULTI_PUNCT:
                if text.startswith(op, i):
                    tokens.append({"kind": "punct", "value": op, "line": line})
                    i += len(op)
                    break
            else:
                tokens.append({"kind": "punct", "value": c, "line": line})
                i += 1
    return tokens


# --------------------------------------------------------------------------
# Type definition extraction
# --------------------------------------------------------------------------

class TypeDef:
    def __init__(self, kind, name, line, body, bases, size_comment, src_file):
        self.kind = kind          # 'struct' | 'class' | 'union' | 'enum'
        self.name = name          # None for anonymous (typedef-only) types
        self.line = line
        self.body = body          # list of tokens inside { ... }
        self.bases = bases        # list of (access, typename)
        self.size_comment = size_comment  # int or None (from `/* size */` / `//size:`)
        self.src_file = src_file
        self.members = None       # filled by parse_body()


def find_matching(tokens, i, op="{", cl="}"):
    """Return index of the closing token matching tokens[i], or None."""
    depth = 0
    for j in range(i, len(tokens)):
        if tokens[j]["kind"] != "punct":
            continue
        if tokens[j]["value"] == op:
            depth += 1
        elif tokens[j]["value"] == cl:
            depth -= 1
            if depth == 0:
                return j
    return None


def skip_attrs(tokens, i):
    """Skip __declspec(...) / __attribute__((...)) / inline runs; return index of first
    token that is not an attribute prefix."""
    while i < len(tokens):
        t = tokens[i]
        if t["kind"] == "id" and t["value"] in ("__declspec", "__attribute__",
                                                "__attribute", "__forceinline"):
            if i + 1 < len(tokens) and tokens[i + 1]["value"] == "(":
                close = find_matching(tokens, i + 1, "(", ")")
                if close is None:
                    return i
                i = close + 1
                continue
        if t["kind"] == "id" and t["value"] in ("inline", "final", "sealed"):
            i += 1
            continue
        break
    return i


def hexval(s):
    m = re.search(r"0[xX]([0-9a-fA-F]+)", s)
    return int(m.group(1), 16) if m else None


def parse_size_comment(comment_value):
    m = re.search(r"size\s*:?\s*0[xX]([0-9a-fA-F]+)", comment_value)
    return int(m.group(1), 16) if m else None


def parse_offset_comment(comment_value):
    # First bare 0xNN in a trailing comment = retail offset.
    return hexval(comment_value)


def split_segments(body):
    """Split body tokens into declaration segments. A segment ends at a depth-0
    ';' or at a depth-0 '}' that closes an inline method body (methods have no
    trailing ';'). Nested type definitions (enum/struct/class/union {...}) do
    NOT split at their '}' so the trailing declarator stays attached.
    Comments immediately after the terminator attach to the segment."""
    segs = []
    depth = 0
    brace_kind = []   # stack: True = this '{' opened a nested type body
    cur = []
    i = 0
    n = len(body)
    while i < n:
        t = body[i]
        if t["kind"] == "punct":
            if t["value"] == "{":
                nested = any(
                    x["kind"] == "id" and x["value"] in ("enum", "struct", "class", "union")
                    for x in cur)
                depth += 1
                brace_kind.append(nested)
            elif t["value"] == "}":
                is_nested_close = bool(brace_kind) and brace_kind.pop()
                depth -= 1
                if depth == 0 and not is_nested_close:
                    # inline method body (or braced initializer) close
                    j = i + 1
                    comment = None
                    while j < n and body[j]["kind"] == "comment" \
                            and body[j]["line"] == t["line"]:
                        comment = body[j]
                        j += 1
                    segs.append((cur, comment))
                    cur = []
                    i = j
                    continue
            elif t["value"] == ";" and depth == 0:
                j = i + 1
                comment = None
                while j < n and body[j]["kind"] == "comment" \
                        and body[j]["line"] == t["line"]:
                    comment = body[j]
                    j += 1
                segs.append((cur, comment))
                cur = []
                i = j
                continue
        cur.append(t)
        i += 1
    if cur:
        segs.append((cur, None))
    return segs


def strip_comments(tokens):
    return [t for t in tokens if t["kind"] != "comment"]


def parse_bases(tokens, i):
    """Parse `: public A, private B` starting at tokens[i] == ':'. Returns
    (list of (access, name), index_after_bases)."""
    bases = []
    depth = 0
    cur = []
    j = i + 1
    while j < len(tokens):
        t = tokens[j]
        if t["kind"] == "punct":
            if t["value"] == "{" and depth == 0:
                break
            if t["value"] in "({[":
                depth += 1
            elif t["value"] in ")}]":
                depth -= 1
            elif t["value"] == "," and depth == 0:
                bases.append(_base_from(cur))
                cur = []
                j += 1
                continue
        cur.append(t)
        j += 1
    if cur:
        bases.append(_base_from(cur))
    return bases, j


def _base_from(toks):
    toks = strip_comments(toks)
    names = []
    for t in toks:
        if t["kind"] == "id" and t["value"] in ("public", "private", "protected", "virtual"):
            continue
        if t["kind"] in ("id", "punct"):
            names.append(t["value"])
    return ("".join(names)).strip(":")


def parse_file(path, text=None):
    """Extract TypeDefs + typedef aliases + enum names from one file.
    Returns FileInfo(types, aliases, enum_names, pack_ranges)."""
    if text is None:
        with open(path, "r", errors="replace") as f:
            text = f.read()
    tokens = tokenize(text)
    info = FileInfo(path)

    # pack pragmas: raw-line scan (approximation, line-based)
    for m in re.finditer(r"#pragma\s+pack\s*\(\s*push\s*,\s*([1248])\s*\)", text):
        line = text[: m.start()].count("\n") + 1
        info.pack_events.append((line, int(m.group(1))))
    for m in re.finditer(r"#pragma\s+pack\s*\(\s*pop\s*\)", text):
        line = text[: m.start()].count("\n") + 1
        info.pack_events.append((line, 0))  # 0 = restore default

    i = 0
    n = len(tokens)
    while i < n:
        t = tokens[i]
        if t["kind"] != "id":
            i += 1
            continue
        v = t["value"]
        if v in ("struct", "class", "union"):
            prev = None
            for k in range(i - 1, -1, -1):
                if tokens[k]["kind"] == "comment":
                    continue
                prev = tokens[k]
                break
            # register nested/anonymous types during body parse, so skip if
            # this keyword opens a member type inside an outer body: handled by
            # the outer body parser; here only top-level defs matter. To keep
            # it simple we accept duplicates (same name re-registered).
            j = skip_attrs(tokens, i + 1)
            name = None
            if j < n and tokens[j]["kind"] == "id" and tokens[j]["value"] not in ("{", ":"):
                name = tokens[j]["value"]
                j += 1
            # attributes can appear after the name too (rare) - skip
            j = skip_attrs(tokens, j)
            bases = []
            if v == "class" and j < n and tokens[j]["value"] == ":":
                bases, j = parse_bases(tokens, j)
            elif j < n and tokens[j]["value"] == ":" and _looks_like_bases(tokens, j):
                bases, j = parse_bases(tokens, j)
            if j < n and tokens[j]["value"] == "{":
                close = find_matching(tokens, j)
                if close is not None:
                    body = tokens[j + 1 : close]
                    # size comment: after '}' up to ';'
                    size_comment = None
                    for k in range(close + 1, min(close + 6, n)):
                        if tokens[k]["kind"] == "comment":
                            size_comment = parse_size_comment(tokens[k]["value"])
                            if size_comment:
                                break
                        if tokens[k]["kind"] == "punct" and tokens[k]["value"] == ";":
                            break
                    if size_comment is None:
                        # `//size:` immediately above the definition
                        for k in range(i - 1, max(i - 8, -1), -1):
                            if tokens[k]["kind"] == "comment" and "size" in tokens[k]["value"]:
                                size_comment = parse_size_comment(tokens[k]["value"])
                                if size_comment:
                                    break
                            if tokens[k]["kind"] == "punct" and tokens[k]["value"] in (";", "}"):
                                break
                    td = TypeDef(v, name, t["line"], body, bases, size_comment, path)
                    info.register(td)
                    # nested anonymous member types get registered by parse_body
                    i = close + 1
                    continue
            # forward declaration or plain use - skip
            i = j + 1 if j > i else i + 1
            continue
        elif v == "enum":
            j = i + 1
            name = None
            if j < n and tokens[j]["kind"] == "id" and tokens[j]["value"] != "{":
                name = tokens[j]["value"]
                j += 1
            if j < n and tokens[j]["value"] == "{":
                close = find_matching(tokens, j)
                if close is not None:
                    if name:
                        info.enum_names.add(name)
                        td = TypeDef("enum", name, t["line"], tokens[j + 1 : close], [], None, path)
                        info.register(td)
                    i = close + 1
                    continue
            i = j
            continue
        elif v == "typedef":
            j = _parse_typedef(tokens, i + 1, info, t["line"])
            i = j if j > i else i + 1
            continue
        i += 1

    info.resolve_bodies()
    return info


def _looks_like_bases(tokens, j):
    k = j + 1
    while k < len(tokens) and tokens[k]["kind"] != "punct":
        k += 1
    return k < len(tokens) and tokens[k]["value"] == "{"


def _parse_typedef(tokens, i, info, line):
    """Handle one typedef statement starting after the 'typedef' token.
    Supports:
      typedef struct NAME { ... } ALIAS;
      typedef struct { ... } ALIAS;
      typedef struct NAME ALIAS;
      typedef enum {...} ALIAS;  / typedef enum NAME ALIAS;
      typedef <type> ALIAS;      (type alias)
    Returns index after the statement."""
    n = len(tokens)
    j = i
    depth = 0
    # find the ';' at depth 0
    semi = None
    for k in range(i, n):
        if tokens[k]["kind"] == "punct":
            if tokens[k]["value"] in "({[":
                depth += 1
            elif tokens[k]["value"] in ")}]":
                depth -= 1
            elif tokens[k]["value"] == ";" and depth == 0:
                semi = k
                break
    if semi is None:
        return i
    stmt = tokens[i:semi]
    stripped = strip_comments(stmt)
    # locate struct/class/union/enum keyword
    kw_idx = None
    for k, t in enumerate(stripped):
        if t["kind"] == "id" and t["value"] in ("struct", "class", "union", "enum"):
            kw_idx = k
            break
    if kw_idx is None:
        # plain alias: typedef X ALIAS;  (X may be multi-token / templated)
        alias = _last_id(stripped)
        if alias:
            ty = stripped[:-1]
            if ty:
                info.aliases[alias] = ty
        return semi + 1
    kw = stripped[kw_idx]["value"]
    p = skip_attrs(stripped, kw_idx + 1)
    # optional name before '{'
    name = None
    if p < len(stripped) and stripped[p]["kind"] == "id" and stripped[p]["value"] != "{":
        name = stripped[p]["value"]
        p += 1
    if p < len(stripped) and stripped[p]["value"] == "{":
        close = find_matching(stripped, p)
        if close is None:
            return semi + 1
        # slice the ORIGINAL token list (comments included) for the body so
        # `/* 0xNN */` offset comments survive into the member parse
        body = stmt[_map_idx(stmt, stripped, p) + 1 : _map_idx(stmt, stripped, close)]
        # aliases after '}'
        tail = stripped[close + 1 :]
        aliases = []
        for t in tail:
            if t["kind"] == "id" and t["value"] not in KEYWORDS:
                aliases.append(t["value"])
        if name is None and aliases:
            name = aliases[0]
        size_comment = None
        for t in tokens[semi + 1 : min(semi + 5, n)]:
            if t["kind"] == "comment":
                size_comment = parse_size_comment(t["value"])
                if size_comment:
                    break
        td = TypeDef(kw, name, line, body, [], size_comment, info.path)
        info.register(td)
        for a in aliases:
            info.aliases[a] = [{"kind": "id", "value": a if a == name else a, "line": line}]
            # alias points at the type itself; layout resolver maps it
            info.alias_of_type[a] = td
        return semi + 1
    # typedef struct NAME ALIAS; / typedef enum NAME ALIAS;
    if name:
        alias = _last_id(stripped)
        if alias:
            info.aliases[alias] = [{"kind": "id", "value": name, "line": line}]
    return semi + 1


def _map_idx(stmt, stripped, s_idx):
    """Map an index into `stripped` (comment-free view of `stmt`) back to the
    corresponding index in `stmt`."""
    si = 0
    for oi, t in enumerate(stmt):
        if t["kind"] == "comment":
            continue
        if si == s_idx:
            return oi
        si += 1
    return len(stmt)


def _last_id(tokens):
    for t in reversed(tokens):
        if t["kind"] == "id" and t["value"] not in KEYWORDS:
            return t["value"]
    return None


class FileInfo:
    def __init__(self, path):
        self.path = path
        self.types = {}       # name -> TypeDef
        self.aliases = {}     # alias -> type tokens
        self.alias_of_type = {}  # alias -> TypeDef (for typedef struct {...} A;)
        self.enum_names = set()
        self.pack_events = [] # [(line, align), ...] 0 = pop

    def register(self, td):
        if td.name:
            self.types[td.name] = td

    def resolve_bodies(self):
        done = set()
        while True:
            pending = [td for td in self.types.values() if id(td) not in done]
            if not pending:
                break
            for td in pending:
                done.add(id(td))
                if td.members is None:
                    td.members = parse_body(td, self)

    def pack_at(self, line):
        align = None
        for ln, a in self.pack_events:
            if ln <= line:
                align = a
        return align or None


def parse_body(td, info):
    """Fill td.members with field dicts. Also registers nested anonymous types."""
    members = []
    has_virtual = False
    virtuals = []
    static_seen = False
    for seg_toks, trail_comment in split_segments(td.body):
        toks = strip_comments(seg_toks)
        if not toks:
            continue
        first = toks[0]
        if first["kind"] == "id" and first["value"] in ("public", "private", "protected"):
            # access specifier; the rest of the segment may hold a declaration
            # (e.g. `public: enum X {...};` when split boundaries fall there)
            if len(toks) > 1 and toks[1]["value"] == ":":
                toks = toks[2:]
                if not toks:
                    continue
                first = toks[0]
            else:
                continue
        if first["kind"] == "id" and first["value"] in ("typedef", "using", "friend", "template"):
            continue
        if first["kind"] == "id" and first["value"] == "static":
            static_seen = True
            continue
        if first["kind"] == "id" and first["value"] == "enum":
            # enum definition or enum member
            rest = toks[1:]
            if any(t["value"] == "{" for t in rest if t["kind"] == "punct"):
                # enum def: register name, and if a declarator follows '}', it's a member
                close = find_matching(rest, next(k for k, t in enumerate(rest) if t["value"] == "{"))
                if close is None:
                    continue
                e_name = None
                if rest and rest[0]["kind"] == "id":
                    e_name = rest[0]["value"]
                if e_name:
                    info.enum_names.add(e_name)
                tail = rest[close + 1 :]
                if tail and _last_id(tail):
                    m = _make_field(tail, info, trail_comment)
                    if m:
                        members.append(m)
                continue
            # enum-typed member: enum NAME m;  -> treat NAME as 4-byte type
            m = _make_field(toks, info, trail_comment)
            if m:
                members.append(m)
            continue
        # virtuals: mark vtable, record the slot (name + signature), skip body
        if first["kind"] == "id" and first["value"] == "virtual":
            has_virtual = True
            rest = toks[1:]
            # signature text up to the first '('
            name = None
            for k, t in enumerate(rest):
                if t["value"] == "(":
                    for k2 in range(k - 1, -1, -1):
                        if rest[k2]["kind"] == "id":
                            name = rest[k2]["value"]
                            break
                    break
            if name:
                virtuals.append({"name": name,
                                 "sig": _join_tokens(rest).strip()})
            continue
        if first["kind"] == "id" and first["value"] == "operator":
            continue  # method
        # nested struct/class/union definition
        if first["kind"] == "id" and first["value"] in ("struct", "class", "union"):
            inner = _parse_nested(toks, info, td, trail_comment)
            if inner is not None:
                members.append(inner)
            continue
        # methods: NAME(...) with ';' or '{', and ~NAME(...)
        if _is_method(toks):
            if any(t["kind"] == "id" and t["value"] == "virtual" for t in toks):
                has_virtual = True
            continue
        m = _make_field(toks, info, trail_comment, seg_toks)
        if m:
            members.append(m)
    td.members = members
    td.has_virtual = has_virtual
    td.virtuals = virtuals
    return members


def _parse_nested(toks, info, outer_td, trail_comment):
    """Handle a segment starting with struct/class/union. Returns a member dict
    (nested type definition as the member type) or None to skip."""
    kw = toks[0]["value"]
    p = skip_attrs(toks, 1)
    name = None
    if p < len(toks) and toks[p]["kind"] == "id" and toks[p]["value"] != "{":
        name = toks[p]["value"]
        p += 1
    if p < len(toks) and toks[p]["value"] == "{":
        close = find_matching(toks, p)
        if close is None:
            return None
        body = toks[p + 1 : close]
        mname = None
        tail = toks[close + 1 :]
        for t in reversed(tail):
            if t["kind"] == "id" and t["value"] not in KEYWORDS:
                mname = t["value"]
                break
        td = TypeDef(kw, name, toks[0]["line"], body, [], None, outer_td.src_file)
        info.register(td)
        td.members = parse_body(td, info)
        m = {
            "kind": "field",
            "name": mname or "(anon)",
            "type_tokens": [{"kind": "id", "value": name or "", "line": toks[0]["line"]}],
            "type_name": name or f"<anonymous {kw}>",
            "nested_td": td,
            "bitfield": None,
            "comment_off": parse_offset_comment(trail_comment["value"]) if trail_comment else None,
            "line": toks[0]["line"],
        }
        return m
    # forward declaration `struct X;` - skip; `struct X *p;` -> data member
    if name is None:
        return None
    m = _make_field(toks, info, trail_comment)
    return m


def _is_method(toks):
    """Heuristic: segment is a function/method declaration (not a data member)."""
    # ~Name(...)
    if any(t["kind"] == "id" and t["value"].startswith("~") for t in toks):
        return True
    depth = 0
    first_open = None
    for k, t in enumerate(toks):
        if t["kind"] != "punct":
            continue
        if t["value"] == "(":
            if depth == 0 and first_open is None:
                first_open = k
            depth += 1
        elif t["value"] == ")":
            depth -= 1
            if depth == 0 and first_open is not None:
                after = first_open + 1
                is_fptr = (after < len(toks) and toks[after]["value"] == "*")
                # if the paren group contains a '*' immediately after '(', it's a
                # function-pointer declarator (data member), not a method
                return not is_fptr
    return False


def _split_type_name(toks):
    """Split a data-member segment into (type_tokens, name, array_dims, bits).
    Handles pointers, refs, arrays, function pointers, bitfields."""
    toks = strip_comments(toks)
    # initializer: cut at top-level '='
    depth = 0
    for k, t in enumerate(toks):
        if t["kind"] == "punct":
            if t["value"] in "({[":
                depth += 1
            elif t["value"] in ")}]":
                depth -= 1
            elif t["value"] == "=" and depth == 0:
                toks = toks[:k]
                break
    if not toks:
        return [], None, [], None
    # bitfield: top-level ':'
    bits = None
    depth = 0
    for k, t in enumerate(toks):
        if t["kind"] == "punct":
            if t["value"] in "({[":
                depth += 1
            elif t["value"] in ")}]":
                depth -= 1
            elif t["value"] == ":" and depth == 0:
                bits = _join_tokens(toks[k + 1 :]).strip()
                toks = toks[:k]
                break
    if not toks:
        return [], None, [], bits, "", ""
    # function pointer: (*name)(...) or (*name[N])(...)
    depth = 0
    for k, t in enumerate(toks):
        if t["kind"] == "punct" and t["value"] == "(":
            if k + 1 < len(toks) and toks[k + 1]["value"] == "*":
                # find matching ')'
                d = 0
                for j in range(k, len(toks)):
                    if toks[j]["value"] == "(":
                        d += 1
                    elif toks[j]["value"] == ")":
                        d -= 1
                        if d == 0:
                            inner = toks[k + 1 : j]
                            star = [x for x in inner if x["value"] == "*"]
                            if not star:
                                break
                            name = None
                            for x in inner:
                                if x["kind"] == "id" and x["value"] not in KEYWORDS:
                                    name = x["value"]
                                    break
                            dims = []
                            for x in inner:
                                if x["value"] == "[":
                                    dims.append(_join_tokens(inner[inner.index(x) + 1 : _arr_close(inner, inner.index(x))]))
                            # parameter list: next '(' ... ')' after the
                            # declarator close; include in the displayed type
                            params = ""
                            p2 = j + 1
                            if p2 < len(toks) and toks[p2]["value"] == "(":
                                d2 = 0
                                for j2 in range(p2, len(toks)):
                                    if toks[j2]["value"] == "(":
                                        d2 += 1
                                    elif toks[j2]["value"] == ")":
                                        d2 -= 1
                                        if d2 == 0:
                                            params = _join_tokens(toks[p2 + 1 : j2])
                                            break
                            ret = _join_tokens(toks[:k])
                            return toks[:k] + [{"kind": "punct", "value": "*", "line": 0}], \
                                   name, dims, bits, ret, params
    # plain declarator: find last top-level id before ';' '[' ':' 
    # strip trailing array dims
    dims = []
    while toks and toks[-1]["kind"] == "punct" and toks[-1]["value"] == "]":
        close = len(toks) - 1
        d = 0
        j = close
        while j >= 0:
            if toks[j]["value"] == "]":
                d += 1
            elif toks[j]["value"] == "[":
                d -= 1
                if d == 0:
                    dims.insert(0, _join_tokens(toks[j + 1 : close]))
                    toks = toks[:j]
                    break
            j -= 1
        if j < 0:
            break
    # strip trailing '&'
    while toks and toks[-1]["value"] == "&":
        toks = toks[:-1]
    # strip trailing '*'s from the type, then name = last id
    star_count = 0
    while toks and toks[-1]["kind"] == "punct" and toks[-1]["value"] == "*":
        star_count += 1
        toks = toks[:-1]
    name = None
    if toks and toks[-1]["kind"] == "id" and toks[-1]["value"] not in KEYWORDS:
        name = toks[-1]["value"]
        toks = toks[:-1]
    if star_count:
        toks.append({"kind": "punct", "value": "*" * star_count, "line": 0})
    return toks, name, dims, bits, "", ""


def _arr_close(tokens, open_idx):
    d = 0
    for j in range(open_idx, len(tokens)):
        if tokens[j]["value"] == "[":
            d += 1
        elif tokens[j]["value"] == "]":
            d -= 1
            if d == 0:
                return j
    return len(tokens)


def _join_tokens(toks):
    s = ""
    for t in toks:
        if t["kind"] == "punct" and t["value"] in "([{.,*&+-/<>=!:":
            s += t["value"]
        else:
            s += " " + t["value"]
    return s.strip()


def _make_field(toks, info, trail_comment, raw_toks=None):
    raw_toks = raw_toks if raw_toks is not None else toks
    lead_off = None
    for t in raw_toks:
        if t["kind"] == "comment":
            o = parse_offset_comment(t["value"])
            if o is not None:
                lead_off = o
        else:
            break
    ty, name, dims, bits, fn_ret, fn_params = _split_type_name(toks)
    if name is None and not bits:
        # anonymous union `union {...}` handled in _parse_nested; a bare
        # `{...}` here would be an inline init - ignore
        if not ty:
            return None
        return None
    comment_off = None
    if trail_comment:
        comment_off = parse_offset_comment(trail_comment["value"])
    if comment_off is None and lead_off is not None:
        comment_off = lead_off  # `/* 0xNN */ u8 x;` leading-comment style
    # static data member (skipped earlier via 'static' prefix, but a static
    # member function returns here too) - guard:
    return {
        "kind": "field",
        "name": name,
        "type_tokens": ty,
        "type_name": (f"{fn_ret}(*)({fn_params})" if fn_params
                       else _join_tokens(ty)),
        "array_dims": dims,
        "bitfield": bits,
        "comment_off": comment_off,
        "line": toks[0]["line"] if toks else 0,
        "nested_td": None,
    }


# --------------------------------------------------------------------------
# Layout engine (MWCC PowerPC)
# --------------------------------------------------------------------------

PRIMITIVES = {
    "char": (1, 1), "signed char": (1, 1), "unsigned char": (1, 1),
    "s8": (1, 1), "u8": (1, 1), "bool": (1, 1),
    "short": (2, 2), "short int": (2, 2), "unsigned short": (2, 2),
    "s16": (2, 2), "u16": (2, 2), "wchar_t": (2, 2),
    "int": (4, 4), "unsigned int": (4, 4), "signed int": (4, 4),
    "long": (4, 4), "unsigned long": (4, 4), "s32": (4, 4), "u32": (4, 4),
    "long long": (8, 8), "unsigned long long": (8, 8), "s64": (8, 8), "u64": (8, 8),
    "float": (4, 4), "f32": (4, 4), "double": (8, 8), "f64": (8, 8),
}


def align_up(v, a):
    if a is None or a <= 1:
        return v
    return (v + a - 1) // a * a


class LayoutEnv:
    def __init__(self, infos, sizes, verbose, auto_include=True, roots=None):
        self.infos = infos           # list of FileInfo (first = primary)
        self.sizes = sizes or {}     # overrides: name -> int
        self.verbose = verbose
        self.warnings = []
        self.auto_include = auto_include
        self.roots = roots or [d for d in ("src", "libs", "include")
                               if os.path.isdir(d)]
        self._miss_cache = set()

    def find_type(self, name):
        for info in self.infos:
            if name in info.types:
                return info.types[name]
        if self.auto_include:
            return self._auto_include(name)
        return None

    def find_local(self, name):
        """Look up `name` only in already-parsed files (no auto-include).
        Used for template base names: a template's generic definition does not
        describe a given instantiation's size (reslist<A> vs reslist<B>), so
        resolving through it is wrong - let comments anchor those instead."""
        for info in self.infos:
            if name in info.types:
                return info.types[name]
        return None

    def _auto_include(self, name):
        """Best-effort: locate and parse a header defining `name` so cross-file
        bases / member types resolve without explicit --include. Uses a shared
        global name->file index; when several files register the type, prefers
        the most substantial definition (real classes over forward decls)."""
        if name in self._miss_cache or len(self.infos) > 24:
            return None
        best = None
        best_score = -1
        for path in _global_type_index().get(name, []):
            if any(os.path.abspath(path) == os.path.abspath(i.path) for i in self.infos):
                continue
            try:
                info = parse_file(path)
            except Exception:
                continue
            self.infos.append(info)
            td = info.types.get(name)
            if td is not None:
                score = len(td.members or []) + len(td.bases) * 4
                if score > best_score:
                    best = td
                    best_score = score
        if best is not None:
            return best
        self._miss_cache.add(name)
        return None

    def resolve_type_tokens(self, ty_toks, in_file, seen=None, alias_chain=None):
        """Return (size, align) for a member type, or (None, None)."""
        seen = seen or set()
        alias_chain = alias_chain or set()
        s = _join_tokens(ty_toks).replace("* *", "**").strip()
        base = s
        star = 0
        while base.endswith("*"):
            star += 1
            base = base[:-1].rstrip()
        base = base.strip()
        if star:
            return 4, 4
        if base.endswith("&"):
            return 4, 4
        if base in PRIMITIVES:
            return PRIMITIVES[base]
        if base == "":
            return None, None
        if base in self.sizes:
            return self.sizes[base], self.sizes[base]
        # template: try base name (local definitions only - see find_local)
        m = re.match(r"([\w:]+)<", base)
        if m:
            tn = m.group(1)
            if tn in self.sizes:
                return self.sizes[tn], self.sizes[tn]
            td = self.find_local(tn)
            if td is not None:
                lay = layout_type(td, self, seen=seen)
                return lay["size"], lay["align"]
        # elaborated struct/class/union keyword
        for kw in ("struct", "class", "union", "enum"):
            if base.startswith(kw):
                base = base[len(kw):].strip()
                break
        if base in self.sizes:
            return self.sizes[base], self.sizes[base]
        if base in self.enum_names():
            return 4, 4
        # typedef / alias
        alias_toks = None
        alias_td = None
        for info in self.infos:
            if base in info.aliases:
                alias_toks = info.aliases[base]
                if base in info.alias_of_type:
                    alias_td = info.alias_of_type[base]
                break
        if alias_td is not None:
            lay = layout_type(alias_td, self, seen=seen)
            return lay["size"], lay["align"]
        if alias_toks is not None:
            if base in alias_chain:
                return None, None
            return self.resolve_type_tokens(alias_toks, in_file, seen,
                                            alias_chain | {base})
        td = self.find_type(base)
        if td is not None:
            lay = layout_type(td, self, seen=seen)
            return lay["size"], lay["align"]
        return None, None

    def enum_names(self):
        names = set()
        for info in self.infos:
            names |= info.enum_names
        return names


_GLOBAL_INDEX = None
_GLOBAL_INDEX_LOCK = None


def _global_type_index():
    """Shared name -> [file paths] index over src/ libs/ include/, built once.
    Cheap regex pass; the parse in _auto_include verifies each hit."""
    global _GLOBAL_INDEX
    if _GLOBAL_INDEX is not None:
        return _GLOBAL_INDEX
    idx = {}
    for root in ("src", "libs", "include"):
        if not os.path.isdir(root):
            continue
        for dirpath, _, files in os.walk(root):
            for fn in files:
                if not fn.endswith((".hpp", ".h", ".hxx")) or ".ctx." in fn:
                    continue
                p = os.path.join(dirpath, fn)
                try:
                    with open(p, errors="replace") as f:
                        text = f.read()
                except OSError:
                    continue
                for m in re.finditer(
                        r"\b(?:struct|class)\s+"
                        r"(?:__declspec\s*\(\s*[\w, ]+\s*\)\s*)?"
                        r"(?:__attribute__\s*\([^)]*\)\s*)?"
                        r"([A-Za-z_]\w*)\b", text):
                    idx.setdefault(m.group(1), []).append(p)
                for m in re.finditer(r"\benum\s+([A-Za-z_]\w*)\s*\{", text):
                    idx.setdefault(m.group(1), []).append(p)
                for m in re.finditer(r"\btypedef\b[^;{]*?([A-Za-z_]\w*)\s*;", text):
                    idx.setdefault(m.group(1), []).append(p)
    _GLOBAL_INDEX = idx
    return idx

def _layout_union(td, env, seen):
    """Union: all members share offset 0; size = max member end, aligned."""
    members = []
    size = 0
    align = 1
    pack = None
    for info in env.infos:
        p = info.pack_at(td.line)
        if p:
            pack = p
    for m in td.members:
        if m["kind"] != "field":
            continue
        if m.get("nested_td") is not None:
            bl = layout_type(m["nested_td"], env, seen)
            sz, al = bl["size"], bl["align"]
        else:
            sz, al = env.resolve_type_tokens(m["type_tokens"], td.src_file, seen)
        dims = m.get("array_dims") or []
        if sz is not None:
            for d in dims:
                n = _arr_len(d)
                if n is None:
                    sz = None
                    break
                sz *= n
        if m.get("bitfield"):
            sz, al = 4, 4
        if pack is not None and al is not None:
            al = min(al, pack)
        members.append({
            "offset": 0, "size": sz, "align": al,
            "type": m["type_name"], "name": m["name"],
            "bitfield": m.get("bitfield"),
            "comment": m.get("comment_off"),
            "flags": ["union"],
            "inferred": False,
        })
        if sz is not None:
            size = max(size, sz)
        if al is not None:
            align = max(align, al)
    size = align_up(size, align)
    return {"size": size, "align": align, "members": members,
            "cycle": False, "td": td}


def layout_type(td, env, seen=None):
    """Compute the layout of a TypeDef. Returns dict with size/align/members."""
    seen = seen or set()
    key = (td.src_file, td.name)
    if key in seen:
        # recursion guard: report cycle, treat as unknown
        return {"size": None, "align": None, "members": [], "cycle": True}
    seen = seen | {key}

    if td.kind == "enum":
        return {"size": 4, "align": 4, "members": [], "cycle": False}

    if td.kind == "union":
        return _layout_union(td, env, seen)

    members = []
    end_max = 0          # highest known end offset so far (fields, bases, vtable)
    bit_unit = None      # (unit_offset, bits_used)
    has_virtual = getattr(td, "has_virtual", False)

    # vtable pointer: class declares virtuals and no base already provides one
    base_virtual = False
    for bname in td.bases:
        btd = env.find_type(bname)
        if btd is not None:
            bl = layout_type(btd, env, seen)
            if bl.get("has_virtual"):
                base_virtual = True

    if has_virtual and not base_virtual:
        members.append({
            "offset": 0, "size": 4, "align": 4, "type": "(vtable)",
            "name": "(vtable)", "bitfield": None, "comment": None,
            "flags": ["vtable"], "inferred": False,
        })
        end_max = 4

    pack = None
    for info in env.infos:
        p = info.pack_at(td.line)
        if p:
            pack = p

    # base classes first
    for bname in td.bases:
        btd = env.find_type(bname)
        if btd is None:
            sz = env.sizes.get(bname)
            if sz is None:
                env.warnings.append(
                    f"{td.name}: base {bname!r} unresolved (use --sizes "
                    f"'{{\"{bname}\": N}}' or --include its file)")
                continue
            bsize = balign = sz
        else:
            bl = layout_type(btd, env, seen)
            bsize, balign = bl["size"], bl["align"]
            if bsize is None:
                env.warnings.append(f"{td.name}: base {bname} has unknown size")
                continue
        off = align_up(end_max, balign or 4)
        members.append({
            "offset": off, "size": bsize, "align": balign or 4,
            "type": bname, "name": f"(base {bname})", "bitfield": None,
            "comment": None, "flags": ["base"], "inferred": False,
        })
        end_max = max(end_max, off + bsize)

    anchored = False
    for m in td.members:
        if m["kind"] != "field":
            continue
        bits = m.get("bitfield")
        if bits is not None and bits != "":
            bval = _bits_int(bits)
            if bval is None or bval == 0:
                # zero-width: force new unit
                bit_unit = (align_up(end_max, 4), 0)
                end_max = bit_unit[0]
                continue
            if bit_unit is None or bit_unit[1] + bval > 32:
                bit_unit = (align_up(end_max, 4), 0)
            off = bit_unit[0]
            members.append({
                "offset": off, "size": 4, "align": 4, "type": m["type_name"],
                "name": m["name"], "bitfield": f"{bit_unit[1]}:{bval}",
                "comment": m.get("comment_off"),
                "flags": ["bitfield"],
                "inferred": False,
            })
            bit_unit = (bit_unit[0], bit_unit[1] + bval)
            end_max = max(end_max, bit_unit[0] + 4)
            continue
        # non-bitfield member
        bit_unit = None
        if m.get("nested_td") is not None:
            bl = layout_type(m["nested_td"], env, seen)
            size, align = bl["size"], bl["align"]
        else:
            size, align = env.resolve_type_tokens(m["type_tokens"], td.src_file, seen)
        dims = m.get("array_dims") or []
        if size is not None:
            for d in dims:
                n = _arr_len(d)
                if n is None:
                    size = None
                    break
                size *= n
        a = align if pack is None else (min(align, pack) if align else None)
        if align is not None and a is None:
            a = align
        comment = m.get("comment_off")
        flags = []
        inferred = False
        if size is not None and not anchored:
            off = align_up(end_max, a or 1)
            if comment is not None and comment != off:
                flags.append("MISMATCH")
        elif comment is not None:
            # comment is ground truth for retail structs; once one member is
            # anchored (unknown type), the rest of the walk is comment-driven
            off = comment
            inferred = size is None
            anchored = True
        else:
            # no comment and the walk is anchored (or the type is unknown):
            # defer; a post-pass places it from neighbor/size comments
            anchored = True
            members.append({
                "offset": None, "size": size,
                "align": a if size is not None else None,
                "type": m["type_name"], "name": m["name"],
                "bitfield": None, "comment": None,
                "flags": ["UNPLACED"], "inferred": False,
            })
            continue
        if m.get("nested_td"):
            flags.append("nested")
        members.append({
            "offset": off, "size": size, "align": a if size is not None else None,
            "type": m["type_name"], "name": m["name"],
            "bitfield": None, "comment": comment, "flags": flags,
            "inferred": inferred,
        })
        if size is not None:
            end_max = max(end_max, off + size)

    # post-pass: place deferred members from neighbor/size comments (fixpoint,
    # so trailing runs resolve right-to-left)
    changed = True
    while changed:
        changed = False
        for i, mm in enumerate(members):
            if mm.get("offset") is not None:
                continue
            nxt = next((x for x in members[i + 1:] if x.get("offset") is not None), None)
            if mm["size"] is not None and nxt is not None and nxt.get("offset") is not None:
                off = nxt["offset"] - mm["size"]
                if off >= 0:
                    mm["offset"] = off
                    mm["inferred"] = True
                    mm["flags"] = [f for f in mm["flags"] if f != "UNPLACED"]
                    changed = True
                    continue
            if (mm["size"] is not None and td.size_comment is not None
                    and i == len(members) - 1):
                off = td.size_comment - mm["size"]
                if off >= 0:
                    mm["offset"] = off
                    mm["inferred"] = True
                    mm["flags"] = [f for f in mm["flags"] if f != "UNPLACED"]
                    changed = True
                    continue
    for mm in members:
        if mm.get("offset") is None:
            env.warnings.append(
                f"{td.name}.{mm['name']}: cannot place (type {mm['type']!r}, "
                f"no offset comment, no anchor)")

    # consistency pass: comment-anchored members must not overlap their
    # successors, and gaps should be explained by the next member's alignment
    for i, mm in enumerate(members):
        if mm["size"] is None or mm.get("comment") is None or mm.get("inferred"):
            continue
        nxt = next((x for x in members[i + 1:] if x.get("offset") is not None), None)
        if nxt is None or nxt.get("offset") is None:
            continue
        end = mm["offset"] + mm["size"]
        if end > nxt["offset"]:
            mm.setdefault("flags", []).append("OVERLAP")
        elif nxt.get("align"):
            exp = align_up(end, nxt["align"])
            if exp != nxt["offset"]:
                mm.setdefault("flags", []).append("GAP?")

    # infer sizes for unknown fields from neighbor offsets (or the trailing
    # `/* size */` comment for the last member)
    infer_sizes(members, td.size_comment)

    # total size
    known_ends = [m["offset"] + m["size"] for m in members
                  if m["size"] is not None and m["offset"] is not None]
    align = 1
    for m in members:
        if m.get("align"):
            align = max(align, m["align"])
    size = align_up(max(known_ends), align) if known_ends else None
    if td.size_comment is not None and size is not None and size != td.size_comment:
        env.warnings.append(
            f"{td.name}: computed sizeof 0x{size:x} != commented 0x{td.size_comment:x}")

    return {
        "size": size,
        "align": align,
        "members": members,
        "cycle": False,
        "anchored": anchored,
        "has_virtual": has_virtual or base_virtual,
        "td": td,
    }


def _bits_int(s):
    m = re.search(r"\d+", s)
    return int(m.group(0)) if m else None


def _arr_len(d):
    return _eval_expr(d)


def _eval_expr(s):
    """Evaluate simple integer expressions used for array sizes, e.g.
    '0x40 - 0x29' or '16 * 2'. Only hex/dec literals and + - * / ( ) are
    allowed; anything else returns None."""
    s = s.strip()
    if not s:
        return None
    if not re.fullmatch(r"[0-9a-fA-FxX+\-*/().\s]+", s):
        return None
    try:
        tree = ast.parse(s, mode="eval")
    except SyntaxError:
        return None

    def ev(node):
        if isinstance(node, ast.Expression):
            return ev(node.body)
        if isinstance(node, ast.Constant) and isinstance(node.value, (int, float)):
            return int(node.value)
        if isinstance(node, ast.BinOp) and isinstance(node.op, (ast.Add, ast.Sub,
                                                                ast.Mult, ast.FloorDiv, ast.Div)):
            l, r = ev(node.left), ev(node.right)
            if l is None or r is None:
                return None
            if isinstance(node.op, ast.Add):
                return l + r
            if isinstance(node.op, ast.Sub):
                return l - r
            if isinstance(node.op, ast.Mult):
                return l * r
            if r == 0:
                return None
            return (l // r) if isinstance(node.op, ast.FloorDiv) else int(l / r)
        if isinstance(node, ast.UnaryOp) and isinstance(node.op, ast.USub):
            v = ev(node.operand)
            return -v if v is not None else None
        return None

    return ev(tree)


def infer_sizes(members, size_comment=None):
    """For members with size None but known offsets, infer the size from the
    next known offset (or the struct size comment for the last member)."""
    for i, m in enumerate(members):
        if m["size"] is not None:
            continue
        if m.get("offset") is None:
            continue
        nxt = None
        for j in range(i + 1, len(members)):
            if members[j].get("offset") is not None:
                nxt = members[j]
                break
        if nxt is not None and nxt.get("offset") is not None:
            m["size"] = max(0, nxt["offset"] - m["offset"])
            m["inferred"] = True
        elif size_comment is not None:
            m["size"] = max(0, size_comment - m["offset"])
            m["inferred"] = True


# --------------------------------------------------------------------------
# Reporting
# --------------------------------------------------------------------------

def fmt_hex(v):
    return f"0x{v:X}" if v is not None else "?"


def print_layout(name, lay, td, env, recursive=False, indent="", depth=0):
    size = lay["size"]
    pad = None
    if size is not None:
        used = sum(m["size"] for m in lay["members"] if m["size"] is not None)
        pad = size - used
    head = f"{indent}{td.kind} {name}"
    loc = f"  ({td.src_file}:{td.line})"
    print(f"{head}{loc}")
    info = f"  sizeof {fmt_hex(size)}"
    if td.size_comment is not None:
        tag = "" if size == td.size_comment else "  <-- commented size differs"
        info += f"  (retail {fmt_hex(td.size_comment)}{tag})"
    info += f"  align {lay['align'] or '?'}"
    if pad is not None and pad > 0:
        info += f"  padding 0x{pad:X}"
    print(info)
    if lay.get("cycle"):
        print("  (recursive/cyclic type)")
        return
    print(f"  {'offset':>7} {'size':>5} {'align':>5}  {'type':<28} {'name':<24} {'retail':>7} flags")
    for m in lay["members"]:
        flags = ",".join(m.get("flags") or [])
        if m["size"] is None:
            sz = "?" if not m.get("inferred") else f"(inf)"
            sz = "?"
        else:
            sz = fmt_hex(m["size"]) + ("*" if m.get("inferred") else "")
        r = fmt_hex(m.get("comment")) if m.get("comment") is not None else ""
        al = fmt_hex(m.get("align")) if m.get("align") else "?"
        print(f"  {fmt_hex(m['offset']):>7} {sz:>5} {al:>5}  "
              f"{m['type']:<28} {m['name']:<24} {r:>7} {flags}")
        if recursive and m.get("flags") and ("nested" in m["flags"] or "base" in m["flags"]):
            sub = m.get("sub_layout")
            if sub is not None and depth < 6:
                sub_td = m.get("sub_td")
                if sub_td is not None:
                    print()
                    print_layout(sub_td.name, sub, sub_td, env, recursive, indent + "    ", depth + 1)
    if size is not None:
        print(f"  {fmt_hex(size):>7}  <end>")


def to_json(lay, td):
    return {
        "name": td.name,
        "kind": td.kind,
        "file": td.src_file,
        "line": td.line,
        "size": lay["size"],
        "align": lay["align"],
        "retail_size": td.size_comment,
        "members": [
            {
                "offset": m["offset"],
                "size": m["size"],
                "align": m.get("align"),
                "type": m["type"],
                "name": m["name"],
                "bitfield": m.get("bitfield"),
                "retail_offset": m.get("comment"),
                "inferred": m.get("inferred", False),
                "flags": m.get("flags") or [],
            }
            for m in lay["members"]
        ],
    }


def attach_sublayouts(lay, env, seen=None):
    """Recursively attach resolved layouts for base/nested members."""
    seen = seen or set()
    td = lay.get("td")
    if td is None:
        return
    for m in lay["members"]:
        if "base" in (m.get("flags") or []):
            btd = env.find_type(m["type"])
            if btd is not None and (btd.src_file, btd.name) not in seen:
                bl = layout_type(btd, env, seen)
                m["sub_layout"] = bl
                m["sub_td"] = btd
                attach_sublayouts(bl, env, seen | {(btd.src_file, btd.name)})
        if m.get("nested_td") is not None:
            bl = layout_type(m["nested_td"], env, seen)
            m["sub_layout"] = bl
            m["sub_td"] = m["nested_td"]
            attach_sublayouts(bl, env, seen | {(m["nested_td"].src_file, m["nested_td"].name)})


def collect_splits(root):
    """Find retail asm data files under build/us/asm (split1.s, criware_data.s,
    per-TU .s files, ...). Returns list of paths."""
    out = []
    for dirpath, _, files in os.walk(root):
        for fn in files:
            if fn.endswith(".s"):
                out.append(os.path.join(dirpath, fn))
    return sorted(out)


def index_vtables(split_paths):
    """Build addr -> (file, label, [slot tokens]) for every .obj data symbol.
    Returns (by_addr, by_label)."""
    by_addr = {}
    by_label = {}
    for path in split_paths:
        with open(path, errors="replace") as f:
            lines = f.read().splitlines()
        cur_addr = None
        cur_label = None
        cur_slots = None
        for i, line in enumerate(lines):
            m = re.match(r"# \.[a-z0-9]+:0x[0-9A-Fa-f]+ \| (0x[0-9A-Fa-f]+) \| size: (0x[0-9A-Fa-f]+)", line)
            if m:
                cur_addr = int(m.group(1), 16)
                continue
            m = re.match(r"\.obj (\S+), global", line)
            if m:
                cur_label = m.group(1)
                cur_slots = []
                continue
            if cur_slots is not None:
                if line.strip().startswith(".endobj"):
                    if cur_addr is not None and cur_label is not None:
                        by_addr[cur_addr] = (path, cur_label, cur_slots)
                        by_label[cur_label] = (path, cur_addr, cur_slots)
                    cur_addr = None
                    cur_label = None
                    cur_slots = None
                else:
                    m = re.match(r"\s*\.4byte\s+(\S+)", line)
                    if m:
                        cur_slots.append(m.group(1))
    return by_addr, by_label


def cmd_vtable(args):
    """Resolve and dump a retail vtable from the split asm data."""
    root = args.asm_root or os.path.join("build", "us", "asm")
    by_addr, by_label = index_vtables(collect_splits(root))
    splits = collect_splits(root)

    target = args.symbol_or_addr
    found = []
    if target.lower().startswith("0x"):
        addr = int(target, 16)
        if addr in by_addr:
            found.append((addr, by_addr[addr]))
    elif target in by_label:
        path, addr, slots = by_label[target]
        found.append((addr, (path, target, slots)))
    else:
        # class-name lookup. Prefer named __RTTI__<class> symbols; fall back
        # to the typeinfo-name string -> RTTI struct -> vtable chain.
        rtti_candidates = [lbl for lbl in by_label
                           if lbl.startswith("__RTTI__") and target in lbl]
        if not rtti_candidates:
            name_label = None
            for path in splits:
                with open(path, errors="replace") as f:
                    lines = f.read().splitlines()
                for i, line in enumerate(lines):
                    m = re.match(r"\s*\.string \"([^\"]*" + re.escape(target)
                                 + r"[^\"]*)\"", line)
                    if m:
                        for k in range(i, -1, -1):
                            m2 = re.match(r"\.obj (\S+), global", lines[k])
                            if m2:
                                name_label = m2.group(1)
                                break
                        break
                if name_label:
                    break
            if name_label:
                rtti_candidates = _refs_of(name_label, splits)
        for rtti in rtti_candidates:
            if rtti not in by_label:
                continue
            # vtable = data symbol referencing this RTTI struct
            vrefs = _refs_of(rtti, splits)
            for vt in vrefs:
                if vt in by_label:
                    found.append((by_label[vt][1],
                                  (by_label[vt][0], vt, by_label[vt][2])))
            if not vrefs:
                # no vtable refs: show the RTTI struct itself for inspection
                found.append((by_label[rtti][1],
                              (by_label[rtti][0], rtti, by_label[rtti][2])))

    if not found:
        print(f"no vtable found for {target!r} (tried address, symbol, class name)",
              file=sys.stderr)
        return 2
    seen = set()
    for addr, (path, label, slots) in found:
        if addr in seen:
            continue
        seen.add(addr)
        if args.json:
            print(json.dumps({
                "address": addr, "label": label, "file": path,
                "slots": [{"offset": i * 4, "symbol": s} for i, s in enumerate(slots)],
            }, indent=2))
        else:
            print(f"vtable 0x{addr:X}  ({label}, {os.path.relpath(path)})")
            print(f"  {'slot':>4} {'offset':>6}  symbol")
            for i, s in enumerate(slots):
                tag = ""
                if i == 0:
                    tag = "  (RTTI)"
                elif i == 1:
                    tag = "  (offset-to-top)"
                print(f"  {i:>4} 0x{i * 4:02X}  {s}{tag}")
    return 0


def _refs_of(label, split_paths):
    """Labels of .obj data symbols whose bodies reference `label` (via .4byte)."""
    out = []
    for path in split_paths:
        with open(path, errors="replace") as f:
            lines = f.read().splitlines()
        cur = None
        for line in lines:
            m = re.match(r"\.obj (\S+), global", line)
            if m:
                cur = m.group(1)
                continue
            if line.strip().startswith(".endobj"):
                cur = None
            elif cur is not None and re.match(rf"\s*\.4byte\s+{re.escape(label)}\s*$", line):
                out.append(cur)
    return out


# --------------------------------------------------------------------------
# Batch check
# --------------------------------------------------------------------------

CHECK_EXT = (".c", ".h", ".hpp", ".cpp")


def _check_file(path, include_ctx=False):
    """Parse + layout every type in one file. Returns (path, list of findings)."""
    findings = []
    try:
        info = parse_file(path)
    except Exception as e:  # noqa: BLE001
        return path, [("ERROR", 0, "parse", str(e))]
    env = LayoutEnv([info], {}, verbose=False)
    for name in sorted(info.types, key=lambda n: (info.types[n].line, n)):
        td = info.types[name]
        n0 = len(env.warnings)
        try:
            lay = layout_type(td, env)
        except Exception as e:  # noqa: BLE001
            findings.append(("ERROR", td.line, name, f"layout exception: {e}"))
            continue
        warns = env.warnings[n0:]
        # member-level flags
        for m in lay.get("members", []):
            for fl in m.get("flags") or []:
                if fl in ("MISMATCH", "OVERLAP", "GAP?"):
                    findings.append((fl, td.line, name,
                                    f"{m['name']}: {fl} (offset 0x{m['offset']:X}, "
                                    f"retail 0x{m.get('comment'):X})" if m.get('comment') is not None
                                    else f"{m['name']}: {fl}"))
            if m.get("offset") is None:
                findings.append(("UNPLACED", td.line, name,
                                f"{m['name']}: unplaced"))
        if td.size_comment is not None and lay.get("size") not in (None, td.size_comment):
            findings.append(("SIZE", td.line, name,
                            f"computed 0x{lay['size']:X} != retail 0x{td.size_comment:X}"))
        # warnings: separate real problems from expected (unresolved types)
        for w in warns:
            if "cannot place" in w or "layout stops" in w:
                findings.append(("INFO", td.line, name, w))
            elif "computed sizeof" in w:
                # reported as the SIZE finding below
                pass
            elif "unresolved" in w or "unknown type" in w:
                # expected without a full include closure
                pass
            else:
                findings.append(("WARN", td.line, name, w))
    return path, findings


def cmd_check(args):
    import concurrent.futures as cfut
    import functools
    paths = []
    for p in args.paths or ["."]:
        if os.path.isdir(p):
            for dirpath, dirnames, filenames in os.walk(p):
                dirnames[:] = [d for d in dirnames
                               if d not in (".git", "build", "node_modules", ".venv")]
                for fn in filenames:
                    if fn.endswith(CHECK_EXT) and (args.include_ctx or ".ctx." not in fn):
                        paths.append(os.path.join(dirpath, fn))
        elif os.path.isfile(p):
            paths.append(p)
    paths = sorted(set(paths))
    jobs = args.jobs or os.cpu_count() or 4
    results = []
    if jobs > 1 and len(paths) > 1:
        with cfut.ProcessPoolExecutor(max_workers=jobs) as ex:
            for res in ex.map(functools.partial(_check_file, include_ctx=args.include_ctx),
                              paths):
                results.append(res)
    else:
        for p in paths:
            results.append(_check_file(p, args.include_ctx))

    total_types = 0
    problem_files = 0
    sev_counts = {}
    lines = []
    for path, findings in results:
        real = [f for f in findings if f[0] != "INFO"]
        if not findings:
            continue
        total_types += 1
        if real:
            problem_files += 1
        for sev, ln, name, msg in findings:
            sev_counts[sev] = sev_counts.get(sev, 0) + 1
            if args.json:
                lines.append({"file": path, "line": ln, "type": name,
                              "severity": sev, "message": msg})
            elif args.quiet:
                if sev != "INFO":
                    print(f"{path}:{ln}: {name}: {msg}")
            else:
                print(f"{path}:{ln}: [{sev}] {name}: {msg}")
    if args.json:
        print(json.dumps({"results": lines, "summary": {
            "files": len(paths), "files_with_issues": problem_files,
            "severity_counts": sev_counts}}, indent=2))
    else:
        print(f"\nchecked {len(paths)} files: {problem_files} with issues "
              f"(severities: {sev_counts or 'none'})")
    return 1 if problem_files else 0


# --------------------------------------------------------------------------
# mapvt: map cast-if slots to retail vtable symbols
# --------------------------------------------------------------------------


def _is_funptr(member):
    return "(*" in member.get("type", "")


def _named_slot(sym):
    """A retail vtable slot counts as a real method if it is a named symbol:
    not a raw hex word, not an unnamed lbl_eu_ data label, not a string
    constant, not RTTI."""
    if not sym:
        return False
    if re.fullmatch(r"0[xX][0-9a-fA-F]+", sym):
        return False
    if sym.startswith("lbl_eu_") or sym.startswith("gap_"):
        return False
    if sym.startswith("__RTTI__") or sym.startswith("@") or sym.startswith("."):
        return False
    return True


def _castif_slots(td, env, seen=None):
    """Return [(offset, name, sig)] for a cast-if type: fn-pointer members of a
    struct, or the declared virtuals of an all-virtual class (vtable slot i at
    (2 + i) * 4)."""
    slots = []
    if getattr(td, "virtuals", None):
        for i, v in enumerate(td.virtuals):
            slots.append((0x08 + i * 4, v["name"], v["sig"]))
        return slots
    lay = layout_type(td, env, seen)
    for m in lay.get("members", []):
        if _is_funptr(m) and m.get("offset") is not None:
            slots.append((m["offset"], m["name"], m["type"]))
    return slots


def _map_slots(slots, by_label, by_addr):
    """For each slot offset, collect candidate retail vtable labels that have a
    named method there. Returns dict label -> {addr, matches, total, per-slot}."""
    cands = {}
    for label, (path, addr, syms) in by_label.items():
        hits = {}
        for off, name, sig in slots:
            idx = off // 4
            if idx < len(syms) and _named_slot(syms[idx]):
                hits[off] = syms[idx]
        if hits:
            mangled = sum(1 for s in syms if _named_slot(s) and "__" in s)
            cands[label] = {"label": label, "addr": addr, "path": path,
                            "hits": hits, "mangled": mangled}
    return cands


def cmd_mapvt(args):
    env = LayoutEnv([parse_file(args.file)], args.sizes, args.verbose)
    splits = collect_splits(args.asm_root or os.path.join("build", "us", "asm"))
    by_addr, by_label = index_vtables(splits)

    if args.type:
        types = [env.find_type(args.type)] if env.find_type(args.type) else []
        if not types:
            print(f"type {args.type!r} not found in {args.file}", file=sys.stderr)
            return 2
    else:
        types = list(env.infos[0].types.values())

    report = []
    for td in types:
        slots = _castif_slots(td, env)
        if not slots:
            continue
        cands = _map_slots(slots, by_label, by_addr)
        # rank by coverage
        ranked = sorted(cands.values(),
                        key=lambda c: (-len(c["hits"]), -c["mangled"], -c["addr"]))
        best = ranked[0] if ranked else None
        entry = {"file": td.src_file, "line": td.line, "type": td.name,
                 "slots": [{"offset": off, "declared": name, "sig": sig}
                            for off, name, sig in slots],
                 "vtable": ({"label": best["label"], "address": best["addr"],
                              "coverage": len(best["hits"]), "total": len(slots),
                              "mangled": best["mangled"]}
                             if best else None),
                 "resolved": ({"offset": off, "retail": sym}
                               for off, sym in (best["hits"].items() if best else {}))}
        entry["resolved"] = list(entry["resolved"])
        report.append(entry)
        if args.json:
            continue
        if not best:
            print(f"{td.src_file}:{td.line}: {td.name}: no matching retail vtable")
            continue
        print(f"{td.src_file}:{td.line}: {td.name}")
        print(f"  -> {best['label']} (0x{best['addr']:X})  "
              f"{len(best['hits'])}/{len(slots)} slots resolved")
        for off, name, sig in slots:
            sym = best["hits"].get(off)
            if sym:
                print(f"     0x{off:02X}  {name:<18} -> {sym}")
            else:
                print(f"     0x{off:02X}  {name:<18} -> (no named slot)")
    if args.json:
        print(json.dumps(report, indent=2))
    return 0


# --------------------------------------------------------------------------
# CLI
# --------------------------------------------------------------------------

def cmd_list(files):
    for path in files:
        info = parse_file(path)
        names = sorted(info.types.keys(), key=lambda n: (info.types[n].line, n))
        if not names:
            print(f"{path}: (no struct/class/union definitions)")
            continue
        print(f"{path}:")
        for n in names:
            td = info.types[n]
            sz = f"  size 0x{td.size_comment:X}" if td.size_comment is not None else ""
            print(f"  {td.kind:<7} {n:<32} :{td.line}{sz}")


def cmd_search(pattern, root):
    rx = re.compile(pattern, re.IGNORECASE)
    hits = []
    for dirpath, _, files in os.walk(root):
        for fn in files:
            if not fn.endswith((".c", ".h", ".hpp", ".cpp")):
                continue
            if ".ctx." in fn or fn.endswith(".bak"):
                continue
            p = os.path.join(dirpath, fn)
            try:
                with open(p, errors="replace") as f:
                    text = f.read()
            except OSError:
                continue
            info = parse_file(p, text)
            for n, td in info.types.items():
                if rx.search(n):
                    hits.append((p, td.line, td.kind, n))
    if not hits:
        print(f"no types matching {pattern!r} under {root}")
        return 1
    for p, ln, kind, n in sorted(hits):
        print(f"{p}:{ln}: {kind} {n}")
    return 0


def cmd_show(args):
    path = args.file
    primary = parse_file(path)
    infos = [primary]
    for inc in args.include or []:
        infos.append(parse_file(inc))
    env = LayoutEnv(infos, args.sizes, args.verbose)
    td = env.find_type(args.type)
    if td is None:
        print(f"type {args.type!r} not found in {path}", file=sys.stderr)
        cands = [n for n in primary.types if args.type.lower() in n.lower()]
        if cands:
            print(f"did you mean: {', '.join(sorted(cands)[:8])}?", file=sys.stderr)
        return 2
    lay = layout_type(td, env)
    if args.recursive:
        attach_sublayouts(lay, env)
    if args.json:
        out = to_json(lay, td)
        out["warnings"] = env.warnings
        print(json.dumps(out, indent=2))
    else:
        print_layout(td.name, lay, td, env, recursive=args.recursive)
        if env.warnings:
            print("\nwarnings:")
            for w in env.warnings:
                print(f"  - {w}")
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(
        description="Show C/C++ struct/class layouts (MWCC PowerPC rules) and "
                    "cross-check against retail offset comments.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    sub = ap.add_subparsers(dest="cmd", required=True)

    p_list = sub.add_parser("list", help="enumerate types in files")
    p_list.add_argument("files", nargs="+")
    p_list.set_defaults(func=cmd_list)

    p_show = sub.add_parser("show", help="dump layout of one type")
    p_show.add_argument("file")
    p_show.add_argument("type")
    p_show.add_argument("-r", "--recursive", action="store_true")
    p_show.add_argument("-i", "--include", action="append",
                        help="extra file to parse for type resolution (repeatable)")
    p_show.add_argument("--sizes", type=json.loads, default=None,
                        help="JSON map of type-name -> byte size overrides")
    p_show.add_argument("--json", action="store_true")
    p_show.add_argument("-v", "--verbose", action="store_true")
    p_show.set_defaults(func=cmd_show)

    p_s = sub.add_parser("search", help="find files defining matching types")
    p_s.add_argument("pattern")
    p_s.add_argument("--root", default=".")
    p_s.set_defaults(func=cmd_search)

    p_c = sub.add_parser("check", help="lint all structs/classes in files or dirs")
    p_c.add_argument("paths", nargs="*", default=None,
                     help="files or dirs (default: current dir)")
    p_c.add_argument("--include-ctx", action="store_true",
                     help="also check .ctx.c scaffold files")
    p_c.add_argument("--jobs", type=int, default=None,
                     help="parallel workers (default: CPU count)")
    p_c.add_argument("--quiet", action="store_true", help="only print problems")
    p_c.add_argument("--json", action="store_true")
    p_c.set_defaults(func=cmd_check)

    p_v = sub.add_parser("vtable", help="dump a retail vtable and its slot names")
    p_v.add_argument("symbol_or_addr",
                     help="vtable symbol (lbl_eu_80528870), address (0x80528870), "
                          "or class name (CfCollCircleImpl)")
    p_v.add_argument("--asm-root", default=None,
                     help="path to retail asm splits (default: build/us/asm)")
    p_v.add_argument("--json", action="store_true")
    p_v.set_defaults(func=cmd_vtable)

    p_m = sub.add_parser("mapvt", help="map cast-if slots to retail vtable symbols")
    p_m.add_argument("file")
    p_m.add_argument("type", nargs="?", default=None,
                     help="specific type; default: all cast-if types in the file")
    p_m.add_argument("--asm-root", default=None)
    p_m.add_argument("--sizes", type=json.loads, default=None)
    p_m.add_argument("--json", action="store_true")
    p_m.add_argument("-v", "--verbose", action="store_true")
    p_m.set_defaults(func=cmd_mapvt)

    args = ap.parse_args(argv)
    if args.cmd == "show":
        return cmd_show(args)
    if args.cmd == "list":
        return cmd_list(args.files)
    if args.cmd == "search":
        return cmd_search(args.pattern, args.root)
    if args.cmd == "check":
        return cmd_check(args)
    if args.cmd == "vtable":
        return cmd_vtable(args)
    if args.cmd == "mapvt":
        return cmd_mapvt(args)
    return 2


if __name__ == "__main__":
    sys.exit(main())

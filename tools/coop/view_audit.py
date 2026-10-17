#!/usr/bin/env python3
"""Audit TU-local "view" structs: debt, duplicate-member, or legitimate?

A *view* struct is a file-scope ``struct XView { u8 _00[N]; <fields> };`` declared
inside a ``.cpp`` to type a raw byte offset into some object. It is the standard
interim idiom when the target layout has no declared member (see the pointer-
arithmetic burndown). Views are not all bad — but they are a *debt marker* when
the wrapped object is a project class whose header we own, because the real fix is
a member declared once in that class (PLAN.md / SKILL.md "prefer the structural
fix").

This module resolves, for each view, **what type it actually wraps** (from its use
sites: ``(XView *)self`` / ``reinterpret_cast<XView *>(expr)``), then classifies:

  vendored   wrapped object lives in a vendored, retail-pinned header
             (``nw4r/``, ``revolution/``, ``RVL_SDK/``, ``CriWare/``) -> legitimate.
  opaque     wraps ``u8*``/``void*``/a temp buffer/a call result -> legitimate.
  duplicate  wrapped type is a project class that ALREADY declares a member at a
             viewed offset -> the view contradicts the header (a hack).
  debt       wrapped type is a project class with no member there -> migratable to
             a real member (prefer a line-neutral in-place padding replacement).
  unresolved the wrapped type could not be determined (heuristic); inspect by hand.

It is a heuristic triage: owner resolution requires a simple declaration in the
same file or a ``this``/``self`` receiver. Every other case is labelled
``unresolved`` rather than guessed.

Usage:
    python3 tools/coop/view_audit.py [--roots R ...] [--json] [--limit N]
"""

from __future__ import annotations

import argparse
import json
import re
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DEFAULT_ROOTS = ("src/kyoshin", "libs/monolib/src", "libs/nw4r/src")
VENDORED = ("nw4r/", "revolution/", "RVL_SDK/", "CriWare/", "PowerPC_EABI_Support/")
OPAQUE_TYPES = {"u8", "s8", "char", "void", "u16", "s16", "u32", "s32", "int", "unsigned", "s64", "u64"}

VIEW_DECL = re.compile(r'^(?:struct|class)\s+([A-Za-z_]\w*View)\s*\{', re.M)
VIEW_TOKEN = re.compile(r'\b([A-Za-z_]\w*View)\b')
ATTEMPTS = ROOT / "docs" / "evidence" / "decomp" / "attempts.jsonl"
TYPE_DECL = re.compile(r'^(?:class|struct)\s+([A-Za-z_]\w*)\b', re.M)
USE_RE = re.compile(r'reinterpret_cast<\s*([A-Za-z_]\w*View)\s*\*>|\(\s*([A-Za-z_]\w*View)\s*\*\s*\)')
# local declaration `Type* ident` (pointer forms only; view wraps pointers)
LOCAL_DECL = re.compile(r'^\s*([A-Za-z_][\w:<>]*)\s*\*+\s*([A-Za-z_]\w*)\s*(?:[=;,)\[]|$)', re.M)
# `Ret ClassName::Method(` at column 0-ish (definition)
METHOD_DEF = re.compile(r'^[A-Za-z_][\w:<>,*&\s]*?\b([A-Za-z_]\w*)\s*::\s*([A-Za-z_~]\w*)\s*\(', re.M)
SELF_PARAM = re.compile(r'\(\s*([A-Za-z_][\w:]*)\s*\*\s*self\b')
OFF_COMMENT = re.compile(r'/\*\s*0x([0-9A-Fa-f]+)\s*\*/|//\s*0x([0-9A-Fa-f]+)')
SIZE_OF = {"u8": 1, "s8": 1, "char": 1, "bool": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4,
           "int": 4, "unsigned": 4, "f32": 4, "float": 4, "u64": 8, "s64": 8, "double": 8, "f64": 8}


@dataclass
class View:
    name: str
    file: str
    line: int
    base: int
    fields: list[tuple[int, str, str]] = field(default_factory=list)  # (offset, type, name)
    wrapped: str | None = None
    klass: str = "unresolved"
    note: str = ""


def _read(path: Path) -> str:
    try:
        return path.read_text(errors="replace")
    except OSError:
        return ""


def _brace_body(text: str, open_idx: int) -> str:
    depth = 0
    for i in range(open_idx, len(text)):
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return text[open_idx + 1:i]
    return text[open_idx + 1:]


def _field_size(ty: str, arr: str | None) -> int:
    base = SIZE_OF.get(ty.split("::")[-1].strip(), 4)
    if arr:
        try:
            n = int(arr, 0)
            return base * n
        except ValueError:
            return base
    return base


def parse_views(text: str, rel: str) -> list[View]:
    out: list[View] = []
    for m in VIEW_DECL.finditer(text):
        name = m.group(1)
        body = _brace_body(text, m.end() - 1)
        line = text[:m.start()].count("\n") + 1
        fields: list[tuple[int, str, str]] = []
        cursor = 0
        base = 0
        parsed_any = False
        for raw in body.splitlines():
            s = raw.split("//", 1)[0].strip()
            if not s or s.startswith(("/*", "*")):
                continue
            fm = re.match(r'^([A-Za-z_][\w:<>]*)\s+([A-Za-z_]\w*)\s*(?:\[\s*([^\]]+)\s*\])?\s*;', s)
            if not fm:
                continue
            ty, fname, arr = fm.group(1), fm.group(2), fm.group(3)
            om = OFF_COMMENT.search(raw)
            if om:
                cursor = int(om.group(1) or om.group(2), 16)
            if not parsed_any:
                # leading byte pad defines the base offset; a name-encoded offset
                # (e.g. SaveHead04View) is handled outside when there is no pad.
                if ty.split("::")[-1] in ("u8", "char", "s8") and arr and re.match(r'^(_|pad|unk)', fname):
                    base = _field_size("u8", arr)
                    cursor = base
                    parsed_any = True
                    continue
            is_pad = bool(re.match(r'^(_|pad|unk|b\d)', fname))
            fields.append((cursor, ty, fname if not is_pad else "_pad"))
            cursor += _field_size(ty, arr)
            parsed_any = True
        if base == 0:
            nm = re.search(r'(?:^|[A-Za-z])([0-9A-Fa-f]{2,4})View$', name)
            if nm:
                try:
                    base = int(nm.group(1), 16) if not nm.group(1).isdigit() or len(nm.group(1)) > 2 else int(nm.group(1), 16)
                except ValueError:
                    base = 0
        out.append(View(name=name, file=rel, line=line, base=base, fields=fields))
    return out


def recorded_reverts(path: Path = ATTEMPTS) -> set[str]:
    """View names whose attempt log records a tested-and-kept (reverted) outcome.

    Agents retire or revert views and log the disposition; re-reporting them as
    debt every run is noise. A view is treated as `reverted` when an attempt line
    naming it also says revert/keep (case-insensitive)."""
    out: set[str] = set()
    for line in _read(path).splitlines():
        if "revert" not in line.lower() and "keep" not in line.lower():
            continue
        out.update(m.lower() for m in VIEW_TOKEN.findall(line))
    return out


def project_types() -> tuple[dict[str, str], set[str]]:
    """{class_name: header_path} for project headers, plus the vendored name set."""
    where: dict[str, str] = {}
    vendored: set[str] = set()
    for base in ("src", "libs"):
        for p in ROOT.joinpath(base).rglob("*.hpp"):
            rel = str(p.relative_to(ROOT))
            for m in TYPE_DECL.finditer(_read(p)):
                nm = m.group(1)
                where.setdefault(nm, rel)
                if rel.startswith(VENDORED):
                    vendored.add(nm)
    return where, vendored


def resolve_wrapped(text: str, view: View, where: dict[str, str]) -> str | None:
    """Most common resolved type the view is cast FROM, or None (opaque/unknown)."""
    locals_: dict[str, str] = {m.group(2): m.group(1) for m in LOCAL_DECL.finditer(text)}
    candidates: Counter[str] = Counter()
    for m in USE_RE.finditer(text):
        if (m.group(1) or m.group(2)) != view.name:
            continue
        tail = text[m.end():m.end() + 80]
        om = re.match(r'\s*&?\s*([A-Za-z_]\w*)', tail)
        if not om:
            candidates["<opaque>"] += 1
            continue
        op = om.group(1)
        if op in ("this", "self"):
            head = text[:m.start()]
            mm = list(METHOD_DEF.finditer(head))
            if mm:
                candidates[mm[-1].group(1)] += 1
                continue
            sm = list(SELF_PARAM.finditer(text))
            if sm:
                candidates[sm[-1].group(1).split("::")[-1]] += 1
            else:
                candidates["<opaque>"] += 1
        elif op in locals_:
            ty = locals_[op].split("::")[-1].strip("*")
            candidates[ty if ty in where or ty in OPAQUE_TYPES else "<opaque>"] += 1
        else:
            candidates["<opaque>"] += 1
    if not candidates:
        return None
    ty, n = candidates.most_common(1)[0]
    return None if ty == "<opaque>" else ty


def klass_members(header_text: str, klass: str) -> dict[int, tuple[str, str]]:
    """{offset: (type, name)} from a class body, using `0xNN` offset comments."""
    m = re.search(r'^(?:class|struct)\s+' + re.escape(klass) + r'\b[^;{]*\{', header_text, re.M)
    if not m:
        return {}
    body = _brace_body(header_text, m.end() - 1)
    out: dict[int, tuple[str, str]] = {}
    for raw in body.splitlines():
        om = OFF_COMMENT.search(raw)
        if not om:
            continue
        fm = re.match(r'^(?:/\*\s*0x[0-9A-Fa-f]+\s*\*/\s*)?([A-Za-z_][\w:<>]*)\s+([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;',
                      raw.split("//", 1)[0].strip())
        if fm:
            out[int(om.group(1) or om.group(2), 16)] = (fm.group(1), fm.group(2))
    return out


def classify(views: list[View], texts: dict[str, str], where: dict[str, str], vendored: set[str]) -> None:
    headers: dict[str, str] = {}
    reverted = recorded_reverts()
    for v in views:
        if v.name.lower() in reverted:
            v.klass = "reverted"
            v.note = "tested & kept (recorded in attempts.jsonl)"
            continue
        v.wrapped = resolve_wrapped(texts.get(v.file, ""), v, where)
        ty = v.wrapped
        if ty is None:
            v.klass = "opaque"
            continue
        if ty in OPAQUE_TYPES:
            v.klass = "opaque"
            continue
        if ty in vendored:
            v.klass = "vendored"
            continue
        hdr = where.get(ty)
        if not hdr:
            v.klass = "unresolved"
            continue
        if hdr not in headers:
            headers[hdr] = _read(ROOT / hdr)
        mem = klass_members(headers[hdr], ty)
        dup, pad = [], []
        for off, _ftype, fname in v.fields:
            real = mem.get(off)
            if real and not re.match(r'^(_|pad|unk|b\d)', real[1]):
                dup.append((off, real))
            elif real:
                pad.append(off)
        if dup:
            v.klass = "duplicate"
            v.note = "; ".join(f"0x{o:x}->{n}" for o, (_t, n) in dup[:3])
        elif mem:
            v.klass = "debt"
            v.note = f"{ty} (+0x{v.base:x}) not declared"
        else:
            # owner header carries no offset comments -> cannot confirm absence
            v.klass = "unconfirmed"
            v.note = f"{ty} (+0x{v.base:x}); header has no offset comments"


def scan_roots(roots=DEFAULT_ROOTS) -> list[View]:
    where, vendored = project_types()
    texts: dict[str, str] = {}
    views: list[View] = []
    for r in roots:
        for p in ROOT.joinpath(r).rglob("*.cpp"):
            rel = str(p.relative_to(ROOT))
            t = _read(p)
            texts[rel] = t
            views.extend(parse_views(t, rel))
    classify(views, texts, where, vendored)
    return views


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--roots", nargs="+", default=list(DEFAULT_ROOTS))
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--limit", type=int, default=0)
    args = ap.parse_args()

    views = scan_roots(tuple(args.roots))
    by = Counter(v.klass for v in views)
    if args.json:
        print(json.dumps({"summary": dict(by), "views": [v.__dict__ for v in views]}, indent=1))
        return 0
    print(f"view structs: {len(views)}")
    for k, n in by.most_common():
        print(f"  {n:4d}  {k}")
    debt = [v for v in views if v.klass in ("debt", "duplicate")]
    print(f"\n{len(debt)} debt/duplicate view(s):")
    for v in (debt[:args.limit] if args.limit else debt):
        print(f"  {v.klass:9s} {v.name:34s} {v.file}:{v.line}  {v.note}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

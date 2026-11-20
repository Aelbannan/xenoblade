#!/usr/bin/env python3
"""Convert retail ``.sdata2`` label references into source literals.

The tool is deliberately conservative:

* it only reads sources/headers belonging to one objdiff unit (never
  ``*.ctx.c``);
* it reads the retail value from the split retail object, using
  ``config/<region>/symbols.txt`` for size/type;
* it never invents a source name for a compiler literal pool;
* magic int->float doubles, NaN/Inf and shared 0.0/1.0 pool entries are
  classified but left alone (they need a cast or a deliberate pool-order
  decision, not a literal);
* ``--apply`` snapshots ``hexdiff <unit> --all --json`` before/after and
  reverts any edit that costs a check mark or lowers the percentage of a
  function that was checked before.

Usage::

    .venv/bin/python3 tools/coop/faithful/float_literals.py --unit <u> [--dry-run|--apply]
"""
from __future__ import annotations

import argparse
import json
import re
import struct
import subprocess
import sys
from dataclasses import dataclass, field
from decimal import Decimal
from pathlib import Path
from typing import Iterable

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.faithful import header_index, pick_header  # noqa: E402
from tools.coop.lib.atomic_write import atomic_write_text  # noqa: E402
from tools.coop.lib.config import load_config  # noqa: E402
from tools.coop.lib.objread import OBJDUMP, section_bytes  # noqa: E402
from tools.coop.lib.project import Project  # noqa: E402
from tools.symbolrecover.lib.parser import load_symbols  # noqa: E402

LABEL_RE = re.compile(r"\blbl_eu_([0-9A-Fa-f]{8})\b")
DATA_KIND_RE = re.compile(r"\bdata:([A-Za-z_]+)")
OBJDUMP_SYMBOL_RE = re.compile(r"^([0-9A-Fa-f]+)\s+(\S+)\s+(\S+)\s+(\.sdata2)\s+([0-9A-Fa-f]+)\s+(\S+)\s*$")

MAGIC_DOUBLES = {
    0x4330000080000000,  # s32 -> f32 with the 0x80000000 offset bit
    0x4330000000000000,  # u32 -> f32
}
FLOAT_ZERO = 0x00000000
FLOAT_ONE = 0x3F800000
DOUBLE_ZERO = 0x0000000000000000
DOUBLE_ONE = 0x3FF0000000000000

KIND_FLOAT = "float"
KIND_DOUBLE = "double"
KIND_MAGIC = "magic"
KIND_NONFINITE = "nan_inf"
KIND_SHARED = "shared_0_1"
KIND_UNSUPPORTED = "unsupported"

SKIP_REASONS = {
    KIND_MAGIC: "int->float magic double: write a cast at the use site, not a literal",
    KIND_NONFINITE: "NaN/Inf word: keep the named label",
    KIND_SHARED: "shared 0.0/1.0 pool entry: keep the named label (rodata spill / reloc drift)",
    KIND_UNSUPPORTED: "symbols.txt does not describe a 4-byte float or 8-byte double",
}


@dataclass(frozen=True)
class LabelValue:
    """Pure value classification, independent of files and objects."""

    kind: str
    bits: int
    size: int
    spelling: str | None = None
    value: float | None = None
    section: str = ""

    @property
    def replaceable(self) -> bool:
        return self.kind in (KIND_FLOAT, KIND_DOUBLE)

    @property
    def reason(self) -> str:
        return SKIP_REASONS.get(self.kind, "")


def _shortest_decimal(value: float, max_digits: int, pack=None) -> str:
    """Shortest %g spelling whose parsed value round-trips to the same bits."""
    for digits in range(1, max_digits + 1):
        text = f"{value:.{digits}g}"
        try:
            parsed = float(text)
            ok = pack(parsed) == pack(value) if pack is not None else parsed == value
            if ok:
                # Prefer ordinary decimal spelling when it is no longer than
                # the exponent form (source reads like the original: 10.0f).
                if "e" in text.lower():
                    fixed = format(Decimal(text), "f")
                    if "." in fixed:
                        fixed = fixed.rstrip("0").rstrip(".")
                    if len(fixed) <= len(text):
                        return fixed
                return text
        except (ValueError, OverflowError):
            continue
    return repr(value)


def float_spelling(bits: int) -> str:
    """Shortest C float literal reproducing a big-endian IEEE-754 word."""
    value = struct.unpack(">f", struct.pack(">I", bits & 0xFFFFFFFF))[0]
    pack = lambda x: struct.pack(">f", x)
    text = _shortest_decimal(value, 9, pack)
    if not any(c in text for c in ".eEnNaAiI"):
        text += ".0"
    return text + "f"


def double_spelling(bits: int) -> str:

    value = struct.unpack(">d", struct.pack(">Q", bits & 0xFFFFFFFFFFFFFFFF))[0]
    text = _shortest_decimal(value, 17)
    if not any(c in text for c in ".eEnNaAiI"):
        text += ".0"
    return text


def classify(bits: int, size: int, data_kind: str, section: str = "") -> LabelValue:
    """Classify a raw sdata2 value read from retail bytes."""
    if size == 4 and data_kind == "float":
        if bits == FLOAT_ZERO or bits == FLOAT_ONE:
            return LabelValue(KIND_SHARED, bits, size, section=section)
        if (bits & 0x7F800000) == 0x7F800000:
            return LabelValue(KIND_NONFINITE, bits, size, section=section)
        value = struct.unpack(">f", struct.pack(">I", bits))[0]
        return LabelValue(KIND_FLOAT, bits, size, float_spelling(bits), value, section)
    if size == 8 and data_kind == "double":
        if bits in MAGIC_DOUBLES:
            return LabelValue(KIND_MAGIC, bits, size, section=section)
        if bits == DOUBLE_ZERO or bits == DOUBLE_ONE:
            return LabelValue(KIND_SHARED, bits, size, section=section)
        if (bits & 0x7FF0000000000000) == 0x7FF0000000000000:
            return LabelValue(KIND_NONFINITE, bits, size, section=section)
        value = struct.unpack(">d", struct.pack(">Q", bits))[0]
        return LabelValue(KIND_DOUBLE, bits, size, double_spelling(bits), value, section)
    return LabelValue(KIND_UNSUPPORTED, bits, size, section=section)


# ── source scanning ──────────────────────────────────────────────────────────

def mask_comments(text: str) -> str:
    """Replace comment bytes with spaces, preserving offsets and newlines."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            for k in range(i, j):
                out[k] = " "
            i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j
        else:
            i += 1
    return "".join(out)


PREPRO_DECL_RE = re.compile(
    r"^[ \t]*#[ \t]*(?:define|undef)[ \t]+(lbl_eu_[0-9A-Fa-f]{8})\b", re.M
)
EXTERN_DECL_RE = re.compile(
    r"^[ \t]*extern\b[^;{}]*\b(lbl_eu_[0-9A-Fa-f]{8})\b[^;{}]*;[ \t]*\r?\n?", re.M
)
# A label can also be declared inside a C-linkage block, on one line or many:
#   extern "C" { extern const float lbl_eu_8066AAC0; }
# EXTERN_DECL_RE cannot see the one-line spelling (the `{` blocks it), which
# used to let scan_source replace the label inside its own declaration with a
# float literal and produce `extern const float 0.1f;`.
EXTERN_C_BLOCK_RE = re.compile(
    r'^[ \t]*extern\s+"C"\s*\{(?P<body>[^{}]*?)\}[ \t]*\r?\n?', re.M | re.S
)
BLOCK_STMT_RE = re.compile(r"[^;{}]*;")
DEFINITION_RE = re.compile(r"\b(lbl_eu_[0-9A-Fa-f]{8})\b[ \t]*(?:=|\()")


def _linkage_declarations(code: str) -> list[tuple[str, int, int, tuple[str, ...] | None, int, int]]:
    """Label declarations in ``extern "C" { ... }`` blocks.

    Returns ``(name, start, end, whole_names, block_start, block_end)``.  When
    the block contains nothing but label declarations, ``whole_names`` lists
    them all: removing the declarations must remove the whole block, otherwise
    an empty ``extern "C" { }`` is left behind.
    """
    sites: list[tuple[str, int, int, tuple[str, ...] | None, int, int]] = []
    for block in EXTERN_C_BLOCK_RE.finditer(code):
        body = block.group("body")
        body_start = block.start("body")
        stmts = [s for s in BLOCK_STMT_RE.finditer(body) if s.group(0).strip()]
        if not stmts:
            continue
        found = [(LABEL_RE.search(s.group(0)), s) for s in stmts]
        # An initializer is a definition, not a removable extern declaration.
        decls = [(lab, s) for lab, s in found if lab and "=" not in s.group(0)]
        if not decls:
            continue
        whole = len(decls) == len(stmts)
        names = tuple(lab.group(0) for lab, _ in decls)
        for lab, s in decls:
            sites.append((lab.group(0),
                          body_start + s.start(), body_start + s.end(),
                          names if whole else None, block.start(), block.end()))
    return sites


POOL_ALIAS_RE = re.compile(
    r"^[ \t]*#[ \t]*define[ \t]+(lbl_eu_[0-9A-Fa-f]{8})[ \t]+([A-Za-z_]\w*)",
    re.M,
)


def _line_bounds(text: str, offset: int) -> tuple[int, int]:
    start = text.rfind("\n", 0, offset) + 1
    end = text.find("\n", offset)
    end = len(text) if end < 0 else end + 1
    return start, end


def _is_declaration_line(
    code: str, offset: int, name: str,
    linkage: list[tuple[str, int, int, tuple[str, ...] | None, int, int]] | None = None,
) -> bool:
    start, end = _line_bounds(code, offset)
    line = code[start:end]
    if PREPRO_DECL_RE.match(line) or EXTERN_DECL_RE.search(line):
        return True
    if re.search(r"^[ \t]*(?:__declspec|const|static|extern)[^;{]*\b" + re.escape(name) + r"\b[^;{]*=", line):
        return True
    for dname, dstart, dend, _names, _bstart, _bend in (
        linkage if linkage is not None else _linkage_declarations(code)
    ):
        if dname == name and dstart <= offset < dend:
            return True
    return False


@dataclass
class Edit:
    path: Path
    start: int
    end: int
    old: str
    new: str
    label: str
    kind: str  # "literal" | "decl"
    functions: tuple[str, ...] = ()


@dataclass
class ScanResult:
    texts: dict[Path, str]
    edits: list[Edit] = field(default_factory=list)
    values: dict[str, LabelValue] = field(default_factory=dict)
    occurrences: dict[str, int] = field(default_factory=dict)
    kept: dict[str, int] = field(default_factory=dict)
    declaration_lines: dict[Path, list[tuple[str, int, int]]] = field(default_factory=dict)


def _function_spans(code: str, symbols: Iterable[str]) -> list[tuple[int, int, tuple[str, ...]]]:
    """Best-effort top-level function spans for edit attribution."""
    syms = tuple(symbols)
    # Mask strings too, so braces in literals cannot confuse the scanner.
    masked = mask_comments(code)
    masked = re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', lambda m: " " * len(m.group(0)), masked)
    spans: list[tuple[int, int, tuple[str, ...]]] = []
    depth = 0
    start = 0
    i = 0
    while i < len(masked):
        ch = masked[i]
        if ch == "{":
            if depth == 0:
                sig = masked[start:i]
                # The function being defined is the last `name(...)` in the
                # declaration; earlier names are return types/calls/attributes.
                candidates = re.findall(r"([A-Za-z_]\w*)\s*\(", sig)
                names: list[str] = []
                for cand in candidates:
                    for sym in syms:
                        if _name_matches(cand, sym) and sym not in names:
                            names.append(sym)
                    if not names and len(cand) >= 4:
                        names.append(cand)
                spans.append((start, i, tuple(sorted(set(names)))))
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                if spans:
                    s, _, n = spans[-1]
                    spans[-1] = (s, i + 1, n)
                start = i + 1
            if depth < 0:
                depth = 0
        elif ch == ";" and depth == 0:
            start = i + 1
        elif ch == "\n" and depth == 0:
            stripped = masked[start:i].strip()
            if stripped.startswith("#") or not stripped:
                start = i + 1
        i += 1
    return spans


def _attribute(offset: int, spans: list[tuple[int, int, tuple[str, ...]]]) -> tuple[str, ...]:
    for start, end, names in spans:
        if start <= offset < end:
            return names
    return ()


def _statement_span(code: str, name: str) -> tuple[int, int] | None:
    """Span of a multi-line C++ definition containing ``name = ...;``."""
    m = re.search(r"\b" + re.escape(name) + r"\b[ \t]*=(?!=)", code)
    if m is None:
        return None
    start = max(code.rfind(";", 0, m.start()), code.rfind("}", 0, m.start()),
                code.rfind("{", 0, m.start())) + 1
    end = code.find(";", m.end())
    if end < 0:
        return None
    return start, end + 1


def _struct_span(code: str, root: str) -> tuple[int, int] | None:
    """Span of a ``struct Sdata2_<TU> { ... };`` pool definition."""
    for m in re.finditer(r"\bstruct\s+([A-Za-z_]\w*)\s*\{", code):
        type_name = m.group(1)
        if "sdata2" not in type_name.lower() and root.lower() not in type_name.lower():
            continue
        depth, i = 0, m.end() - 1
        while i < len(code):
            if code[i] == "{":
                depth += 1
            elif code[i] == "}":
                depth -= 1
                if depth == 0:
                    j = code.find(";", i)
                    return m.start(), (j + 1 if j >= 0 else i + 1)
            i += 1
    return None


def scan_source(
    paths: list[Path],
    values: dict[str, LabelValue],
    symbols: Iterable[str],
    shared_elsewhere: set[str],
) -> ScanResult:
    result = ScanResult(texts={p: p.read_text(encoding="utf-8") for p in paths})
    used_globally: set[str] = set()
    all_decls: list[tuple[Path, str, int, int, tuple[tuple[str, ...], int, int] | None]] = []
    codes: dict[Path, str] = {}
    for path in paths:
        code = mask_comments(result.texts[path])
        codes[path] = code
        sites = _linkage_declarations(code)
        spans = _function_spans(code, symbols)
        whole_blocks = {(bstart, bend) for _n, _s, _e, names, bstart, bend in sites if names}
        for m in EXTERN_DECL_RE.finditer(code):
            if any(bs <= m.start() < be for bs, be in whole_blocks):
                continue  # the whole linkage block is removed instead
            all_decls.append((path, m.group(1), m.start(), m.end(), None))
        for name, start, end, names, bstart, bend in sites:
            all_decls.append((path, name, start, end,
                              (names, bstart, bend) if names else None))

        for m in LABEL_RE.finditer(code):
            name = m.group(0)
            value = values.get(name)
            if value is None:
                continue
            result.occurrences[name] = result.occurrences.get(name, 0) + 1
            if not _is_declaration_line(code, m.start(), name, sites) and not DEFINITION_RE.match(code, m.start()):
                # Any surviving reference (including deliberately kept magic
                # / shared labels) pins its declaration in place.
                used_globally.add(name)
            if not value.replaceable:
                result.kept[name] = result.kept.get(name, 0) + 1
                continue
            if _is_declaration_line(code, m.start(), name, sites):
                continue
            if DEFINITION_RE.match(code, m.start()):
                continue
            result.edits.append(Edit(
                path=path,
                start=m.start(),
                end=m.end(),
                old=name,
                new=value.spelling or "",
                label=name,
                kind="literal",
                functions=_attribute(m.start(), spans),
            ))

    # Re-check the post-literal text across *all* files of the unit before any
    # declaration or pool definition is removed.
    rendered = render(result.texts, result.edits)
    remaining: dict[str, int] = {}
    for path, text in rendered.items():
        code = mask_comments(text)
        sites = _linkage_declarations(code)
        for m in LABEL_RE.finditer(code):
            name = m.group(0)
            if _is_declaration_line(code, m.start(), name, sites) or DEFINITION_RE.match(code, m.start()):
                continue  # declarations are not references
            remaining[name] = remaining.get(name, 0) + 1

    # Unused extern spellings may be deleted, but only for real .sdata2
    # symbols with zero remaining references in the unit and no other users.
    dropped_blocks: set[tuple[int, int]] = set()
    for path, name, start, end, block in all_decls:
        names = block[0] if block else (name,)
        if all(
            (n in values and values[n].replaceable and values[n].section == ".sdata2"
             and not remaining.get(n, 0) and n not in shared_elsewhere)
            for n in names
        ):
            if block and block[1:] in dropped_blocks:
                continue
            start, end = (block[1], block[2]) if block else (start, end)
            if block:
                dropped_blocks.add(block[1:])
            result.edits.append(Edit(
                path=path,
                start=start,
                end=end,
                old=result.texts[path][start:end],
                new="",
                label=name,
                kind="decl",
                functions=(),
            ))

    # A TU-owned literal pool whose every labelled slot became a source
    # literal is dead duplicate data: delete the pool definition as well.
    aliases: dict[tuple[Path, str], list[str]] = {}
    for path, code in codes.items():
        for m in POOL_ALIAS_RE.finditer(code):
            aliases.setdefault((path, m.group(2)), []).append(m.group(1))
    for (path, root), names in sorted(aliases.items()):
        if not names or not all(
            (n in values and values[n].replaceable and values[n].section == ".sdata2"
             and remaining.get(n, 0) == 0) for n in names
        ):
            continue
        code = codes[path]
        spans: list[tuple[int, int]] = []
        for m in POOL_ALIAS_RE.finditer(code):
            if m.group(1) in names:
                spans.append((m.start(), m.end()))
        for span in (_struct_span(code, root), _statement_span(code, root)):
            if span is not None:
                spans.append(span)
        for start, end in sorted(set(spans), reverse=True):
            result.edits.append(Edit(
                path=path,
                start=start,
                end=end,
                old=result.texts[path][start:end],
                new="",
                label=root,
                kind="defn",
                functions=(),
            ))
    return result


# ── retail value reads ──────────────────────────────────────────────────────

def sdata2_offsets(retail_obj: Path) -> dict[str, tuple[int, int]]:
    """Symbol offsets inside a split retail object's .sdata2 section."""
    r = subprocess.run([str(OBJDUMP), "-t", str(retail_obj)], capture_output=True, text=True)
    if r.returncode != 0:
        raise RuntimeError(f"objdump -t failed for {retail_obj}: {r.stderr.strip()}")
    out: dict[str, tuple[int, int]] = {}
    for line in r.stdout.splitlines():
        m = OBJDUMP_SYMBOL_RE.match(line)
        if m:
            out[m.group(6)] = (int(m.group(1), 16), int(m.group(5), 16))
    return out


def read_values(
    retail_obj: Path,
    symbols_path: Path,
    labels: set[str],
) -> dict[str, LabelValue]:
    if not labels:
        return {}
    entries = {e.name: e for e in load_symbols(symbols_path) if e.name in labels}
    offsets = sdata2_offsets(retail_obj)
    data = section_bytes(retail_obj, ".sdata2")
    if data is None:
        raise RuntimeError(f"cannot read .sdata2 from {retail_obj}")
    values: dict[str, LabelValue] = {}
    for name in sorted(labels):
        entry = entries.get(name)
        if entry is None or entry.section != ".sdata2" or entry.size is None:
            continue
        dm = DATA_KIND_RE.search(entry.raw_line)
        if dm is None:
            continue
        sym = offsets.get(name)
        if sym is None or sym[1] != entry.size:
            continue
        offset, size = sym
        if offset + size > len(data):
            continue
        bits = int.from_bytes(data[offset:offset + size], "big")
        values[name] = classify(bits, size, dm.group(1), entry.section)
    return values


def find_other_users(labels: set[str], own_paths: set[Path]) -> set[str]:
    """Labels still referenced outside this unit (so headers cannot lose them)."""
    if not labels:
        return set()
    pat = re.compile("|".join(re.escape(x) for x in sorted(labels)))
    found: set[str] = set()
    for root_name in ("src", "include", "libs"):
        for path in (ROOT / root_name).rglob("*"):
            if not path.is_file() or path.suffix == ".ctx.c" or path in own_paths:
                continue
            if path.suffix not in {".c", ".cpp", ".h", ".hpp", ".inl"}:
                continue
            text = mask_comments(path.read_text(encoding="utf-8", errors="replace"))
            for m in pat.finditer(text):
                found.add(m.group(0))
                if len(found) == len(labels):
                    return found
    return found


# ── apply / verify ──────────────────────────────────────────────────────────

def render(texts: dict[Path, str], edits: list[Edit]) -> dict[Path, str]:
    out = dict(texts)
    by_path: dict[Path, list[Edit]] = {}
    for e in edits:
        by_path.setdefault(e.path, []).append(e)
    for path, group in by_path.items():
        text = out[path]
        for e in sorted(group, key=lambda x: x.start, reverse=True):
            text = text[:e.start] + e.new + text[e.end:]
        out[path] = text
    return out


# ── write scope ────────────────────────────────────────────────────────────────

def writable_paths(unit) -> list[Path]:
    """The only files this tool is ever allowed to write for ``unit``."""
    src = unit.source_path
    if src is None or src.suffix == ".ctx.c":
        raise ValueError(f"unit {getattr(unit, 'name', '?')} has no writable source")
    paths = [src]
    hdr = pick_header(src, header_index())
    if hdr is not None and hdr != src and hdr.suffix != ".ctx.c":
        paths.append(hdr)
    for path in paths:
        if not path.resolve().is_relative_to(ROOT):
            raise ValueError(f"refusing to write outside the repository: {path}")
    return paths


def assert_writable(paths: Iterable[Path], allowed: Iterable[Path]) -> None:
    extra = set(paths) - set(allowed)
    if extra:
        raise ValueError(f"refusing to write files outside the --unit scope: {sorted(map(str, extra))}")


def hexdiff_snapshot(unit: str) -> dict:
    r = subprocess.run(
        # Byte-identity sweep; baseline regressions are a separate gate.
        [sys.executable, "tools/coop/hexdiff.py", unit, "--all", "--json", "--no-baseline"],
        cwd=ROOT, capture_output=True, text=True,
    )
    if r.returncode not in (0, 5) or not r.stdout.strip().startswith("{"):
        raise RuntimeError(
            f"hexdiff --all --json failed (rc={r.returncode}):\n{r.stderr[-4000:]}\n{r.stdout[-1000:]}"
        )
    return json.loads(r.stdout)


def snapshot_rows(data: dict) -> dict[str, tuple[bool, float]]:
    rows: dict[str, tuple[bool, float]] = {}
    for f in data.get("functions", []):
        if not f.get("present"):
            continue
        total_words = max(f.get("retail_size", 0), f.get("decomp_size", 0)) / 4.0
        pct = 100.0 if total_words == 0 else 100.0 * (total_words - f.get("mismatch", 0)) / total_words
        # This sweep asks "did the natural rewrite change codegen?" — the
        # byte/reloc proof is the right signal, not objdiff-aware full_match.
        rows[f["symbol"]] = (bool(f.get("bytes_match", f.get("match", False))), pct)
    return rows


def regressions(before: dict, after: dict) -> list[str]:
    bad: list[str] = []
    rows_before = snapshot_rows(before)
    rows_after = snapshot_rows(after)
    for name, (matched, pct) in rows_before.items():
        now = rows_after.get(name)
        if now is None:
            bad.append(name)
        elif matched and not now[0]:
            bad.append(name)
        elif matched and now[1] < pct - 1e-9:
            bad.append(name)
    return bad


def _name_matches(fn: str, symbol: str) -> bool:
    """Conservative source-name -> retail-symbol identity check."""
    if fn == symbol:
        return True
    for long, short in ((symbol, fn), (fn, symbol)):
        if long.startswith(short) and len(long) > len(short):
            if long[len(short)] in "_F(P":
                return True
    return False


def pick_reverts(regressed: list[str], edits: list[Edit]) -> set[int]:
    """Indices of edits to drop for the regressed function set."""
    drop: set[int] = set()
    global_drop = {i for i, e in enumerate(edits) if e.kind in ("decl", "defn")}
    for i, e in enumerate(edits):
        if e.kind in ("decl", "defn"):
            continue
        if any(_name_matches(fn, sym) for fn in e.functions for sym in regressed):
            drop.add(i)
    if not drop:
        # Unattributed regression: line-number/header effects are global.
        # Drop everything still applied and let the next round start over.
        drop = set(range(len(edits)))
    # Declaration deletions shift __LINE__ for the whole TU; if any regressed
    # function has no directly attributed literal edit, drop those too.
    named = {fn for i, e in enumerate(edits) if i in drop for fn in e.functions}
    if any(not any(_name_matches(fn, s) for fn in named) for s in regressed):
        drop |= global_drop
    return drop


def apply_verified(unit: str, scan: ScanResult, allowed_paths: Iterable[Path],
                   max_rounds: int = 8) -> tuple[list[Edit], dict, list[str]]:
    """Apply edits, reverting any that hurt a matched function."""
    allowed = set(allowed_paths)
    assert_writable([e.path for e in scan.edits], allowed)
    before = hexdiff_snapshot(unit)
    active = list(scan.edits)
    dropped: list[Edit] = []
    notes: list[str] = []
    original = dict(scan.texts)
    for round_no in range(1, max_rounds + 1):
        if not active:
            break
        current = render(original, active)
        for path, text in current.items():
            atomic_write_text(path, text)
        try:
            after = hexdiff_snapshot(unit)
        except Exception as exc:
            for path, text in original.items():
                atomic_write_text(path, text)
            raise exc
        bad = regressions(before, after)
        if not bad:
            return active, after, notes + [f"round {round_no}: accepted {len(active)} edit(s)"]
        drop = pick_reverts(bad, active)
        if not drop:
            for path, text in original.items():
                atomic_write_text(path, text)
            raise RuntimeError(f"regression with no attributable edits: {bad}")
        notes.append(
            f"round {round_no}: reverted {len(drop)} edit(s) after regressions in "
            + ", ".join(sorted(bad)[:6])
        )
        for idx in sorted(drop, reverse=True):
            dropped.append(active[idx])
            del active[idx]
    final = render(original, active)
    for path, text in final.items():
        atomic_write_text(path, text)
    after = hexdiff_snapshot(unit)
    if regressions(before, after):
        for path, text in original.items():
            atomic_write_text(path, text)
        raise RuntimeError("could not reach a non-regressing edit set; restored original sources")
    notes.append(f"final: {len(active)} edit(s) kept, {len(dropped)} reverted")
    return active, after, notes


# ── CLI ─────────────────────────────────────────────────────────────────────

def _print_report(unit: str, values: dict[str, LabelValue], scan: ScanResult, kept_edits: set[int]) -> None:
    print(f"unit: {unit}")
    print(f"{'label':20s} {'kind':13s} {'bytes':>4s}  {'before':>14s}  {'after':>14s}  refs")
    for name in sorted(values):
        v = values[name]
        n_before = scan.occurrences.get(name, 0)
        converted = sum(
            1 for e in scan.edits if e.label == name and e.kind == "literal" and id(e) in kept_edits
        )
        after = n_before - converted
        print(
            f"{name:20s} {v.kind:13s} {v.size:4d}  "
            f"{'lbl_eu_' + name[7:]:>14s}  {(v.spelling or '(kept)'):>14s}  "
            f"{n_before}->{after}"
        )
        if not v.replaceable:
            print(f"{'':20s}  kept: {v.reason}")
    print(f"literal replacements: {sum(1 for e in scan.edits if e.kind == 'literal' and id(e) in kept_edits)}")
    print(f"declaration removals: {sum(1 for e in scan.edits if e.kind == 'decl' and id(e) in kept_edits)}")
    print(f"pool definitions removed: {sum(1 for e in scan.edits if e.kind == 'defn' and id(e) in kept_edits)}")


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--unit", required=True, help="objdiff unit hint or source path")
    mode = p.add_mutually_exclusive_group()
    mode.add_argument("--dry-run", action="store_true", help="analyze only (default)")
    mode.add_argument("--apply", action="store_true", help="apply and hexdiff-verify each change")
    args = p.parse_args(argv)

    config = load_config(None, ROOT)
    project = Project(config)
    try:
        unit = project.resolve_unit(args.unit)
    except ValueError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2
    if unit.source_path is None or unit.target_path is None:
        print(f"ERROR: unit {unit.name} has no source or retail object", file=sys.stderr)
        return 2

    try:
        paths = writable_paths(unit)
    except ValueError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2
    texts = {path: path.read_text(encoding="utf-8") for path in paths}
    labels = {m.group(0) for path in paths for m in LABEL_RE.finditer(mask_comments(texts[path]))}
    values = read_values(unit.target_path, ROOT / "config" / config.region / "symbols.txt", labels)
    if not values:
        print("no readable .sdata2 label values in this unit", file=sys.stderr)
        return 1

    symbols: list[str] = []
    if args.apply:
        try:
            base = hexdiff_snapshot(unit.name)
        except Exception as exc:
            print(f"ERROR: baseline {exc}", file=sys.stderr)
            return 3
        symbols = [f["symbol"] for f in base.get("functions", [])]

    shared = find_other_users(set(values), set(paths))
    scan = scan_source(paths, values, symbols, shared)
    kept: set[int] = set()
    if args.apply:
        kept_edits, after, notes = apply_verified(unit.name, scan, paths)
        kept = {id(e) for e in kept_edits}
        for note in notes:
            print(note)
        before_rows = snapshot_rows(base)
        after_rows = snapshot_rows(after)
        print(
            f"matched: {sum(x[0] for x in before_rows.values())} -> "
            f"{sum(x[0] for x in after_rows.values())} of {len(before_rows)}"
        )
    else:
        print("dry-run: no files changed")
        kept = {id(e) for e in scan.edits}
    _print_report(unit.name, values, scan, kept)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

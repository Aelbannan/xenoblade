"""Opt-in post-pass that credits *proven* objdiff report artifacts.

``build/<region>/report.json`` is objdiff's raw output.  Two legitimate
artifacts make it under-report byte-identical objects:

* **linker tail padding** — a retail split section ends at the next unit's
  start, so it carries zero bytes that align the next TU.  :mod:`data_match`
  already credits those in ``run.py data diff``; here the same proof is
  projected into a *copy* of the report.
* **anonymous ``@N`` literal relocs** — MWCC numbers TU-local string/float
  pools per translation unit, so retail names ``@1254`` and decomp names
  ``@18982`` can be the same datum.  objdiff's ``functionRelocDiffs`` modes
  cannot equate them reliably: ``data_value`` scores pluginEve's byte-identical
  ``fadeIn``/`fadeOut` at 99.69/99.77, while ``name_address`` fixes those two
  but regresses 749 other functions repo-wide.  This post-pass proves the
  equivalence from the objects (same instruction bytes, same reloc offsets /
  types, and the differing local targets resolve to the same section offset
  with identical data), then credits only those rows.

The raw report is never overwritten.  Every adjusted item keeps its original
number in ``raw_fuzzy_match_percent`` and each measures block keeps
``raw_measures``.  The adjusted copy also records a ``credit`` block with the
census and the source report path.  Nothing here changes source code, object
files, or promotion gates.
"""

from __future__ import annotations

import copy
import re
import struct
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any, Mapping

from tools.coop.lib.data_match import (
    DATA_SECTIONS,
    NOBITS,
    _parse,
    link_tail_padding,
)
from tools.coop.lib.reloc_canon import type_class
from tools.ppc_equivalence.elf_symbols import (
    ElfSymbolError,
    FunctionBytes,
    FunctionRelocation,
    list_text_functions,
)

#: MWCC TU-local anonymous literal: ``@1254`` or ``@1254@alias``.  ``@N``
#: numbering is a compiler artifact, not a symbol identity.
_ANON_LOCAL_RE = re.compile(r"^@\d+(?:@.+)?$")
RAW_PERCENT = "raw_fuzzy_match_percent"
RAW_MEASURES = "raw_measures"


@dataclass
class CreditCensus:
    units_scanned: int = 0
    units_changed: int = 0
    sections_scanned: int = 0
    sections_credited: int = 0
    functions_scanned: int = 0
    functions_credited: int = 0
    bytes_credited: int = 0
    skipped_units: int = 0
    details: list[str] = field(default_factory=list)

    def as_dict(self) -> dict[str, Any]:
        return asdict(self)

    def render(self) -> str:
        lines = [
            f"units:     {self.units_scanned} scanned, {self.units_changed} credited "
            f"({self.skipped_units} skipped: missing objects)",
            f"sections:  {self.sections_scanned} candidate data sections, "
            f"{self.sections_credited} credited",
            f"functions: {self.functions_scanned} candidate functions, "
            f"{self.functions_credited} credited",
            f"bytes:     {self.bytes_credited} retail padding bytes credited",
        ]
        if self.details:
            lines.append("credits:")
            lines.extend(f"  {detail}" for detail in self.details[:20])
            if len(self.details) > 20:
                lines.append(f"  ... and {len(self.details) - 20} more")
        return "\n".join(lines)


def _set_percent(item: dict[str, Any], value: float) -> None:
    old = item.get("fuzzy_match_percent")
    if old is None:
        return
    item.setdefault(RAW_PERCENT, old)
    item["fuzzy_match_percent"] = float(value)


def _remember_measures(unit: dict[str, Any]) -> dict[str, Any]:
    measures = unit.get("measures") or {}
    if RAW_MEASURES not in unit:
        unit[RAW_MEASURES] = copy.deepcopy(measures)
    return measures


def _retally_unit(
    unit: dict[str, Any], *, functions_changed: bool, sections_changed: bool
) -> None:
    """Recompute the count-style measures affected by the item credits.

    objdiff's unit measures are simple counts: ``matched_data``/``matched_code``
    sum the sizes of items whose fuzzy score is exactly 100, and the unit fuzzy
    score is the size-weighted mean of its functions' fuzzy scores.  Untouched
    measures are left exactly as objdiff wrote them; the raw copies above
    preserve the originals of the ones we do change.
    """
    measures = _remember_measures(unit)
    functions = unit.get("functions") or []
    sections = unit.get("sections") or []

    def size(item: dict[str, Any]) -> int:
        try:
            return int(item.get("size") or 0)
        except (TypeError, ValueError):
            return 0

    def scalar(value: Any) -> int:
        try:
            return int(value or 0)
        except (TypeError, ValueError):
            return 0

    def full(item: dict[str, Any]) -> bool:
        pct = item.get("fuzzy_match_percent")
        return pct is not None and float(pct) >= 100.0

    if functions_changed:
        matched_code = sum(size(fn) for fn in functions if full(fn))
        total_code = scalar(measures.get("total_code")) or sum(size(fn) for fn in functions)
        matched_functions = sum(1 for fn in functions if full(fn))
        total_functions = scalar(measures.get("total_functions")) or len(functions)
        weight = sum(size(fn) for fn in functions
                     if fn.get("fuzzy_match_percent") is not None)
        fuzzy = (sum(size(fn) * float(fn["fuzzy_match_percent"]) for fn in functions
                     if fn.get("fuzzy_match_percent") is not None) / weight) if weight else 0.0
        measures["matched_code"] = str(matched_code)
        measures["matched_code_percent"] = (
            round(100.0 * matched_code / total_code, 5) if total_code else 0.0)
        measures["matched_functions"] = matched_functions
        measures["matched_functions_percent"] = (
            round(100.0 * matched_functions / total_functions, 5) if total_functions else 0.0)
        measures["fuzzy_match_percent"] = round(fuzzy, 5)
        # The .text section row mirrors the unit's function-weighted score.
        for sec in sections:
            if sec.get("name") == ".text" and sec.get("fuzzy_match_percent") is not None:
                _set_percent(sec, measures["fuzzy_match_percent"])
    if sections_changed:
        matched_data = sum(size(sec) for sec in sections
                           if sec.get("name") != ".text" and full(sec))
        total_data = scalar(measures.get("total_data")) or sum(
            size(sec) for sec in sections if sec.get("name") != ".text")
        measures["matched_data"] = str(matched_data)
        measures["matched_data_percent"] = (
            round(100.0 * matched_data / total_data, 5) if total_data else 0.0)


def _retally_report(report: dict[str, Any]) -> None:
    measures = report.get("measures")
    if not isinstance(measures, dict):
        return
    report.setdefault(RAW_MEASURES, copy.deepcopy(measures))
    units = [u for u in report.get("units", []) if isinstance(u.get("measures"), dict)]
    code_total = sum(int(u["measures"].get("total_code") or 0) for u in units)
    data_total = sum(int(u["measures"].get("total_data") or 0) for u in units)
    fn_total = sum(int(u["measures"].get("total_functions") or 0) for u in units)
    matched_code = sum(int(u["measures"].get("matched_code") or 0) for u in units)
    matched_data = sum(int(u["measures"].get("matched_data") or 0) for u in units)
    matched_fn = sum(int(u["measures"].get("matched_functions") or 0) for u in units)
    fuzzy = (sum(int(u["measures"].get("total_code") or 0)
                 * float(u["measures"].get("fuzzy_match_percent") or 0.0)
                 for u in units) / code_total) if code_total else 0.0
    measures["matched_code"] = str(matched_code)
    measures["matched_data"] = str(matched_data)
    measures["matched_functions"] = matched_fn
    measures["matched_code_percent"] = round(100.0 * matched_code / code_total, 5) if code_total else 0.0
    measures["matched_data_percent"] = round(100.0 * matched_data / data_total, 5) if data_total else 0.0
    measures["matched_functions_percent"] = round(100.0 * matched_fn / fn_total, 5) if fn_total else 0.0
    measures["fuzzy_match_percent"] = round(fuzzy, 5)


def _credit_sections(
    unit: dict[str, Any],
    retail: Any,
    decomp: Any,
    retail_bytes: bytes,
    decomp_bytes: bytes,
    census: CreditCensus,
) -> None:
    for sec in unit.get("sections", []):
        name = sec.get("name")
        pct = sec.get("fuzzy_match_percent")
        if name not in DATA_SECTIONS or pct is None or float(pct) >= 100.0:
            continue
        census.sections_scanned += 1
        pad = link_tail_padding(retail, decomp, retail_bytes, decomp_bytes, name)
        if not pad:
            continue
        _set_percent(sec, 100.0)
        sec["pad_credit_bytes"] = pad
        census.sections_credited += 1
        census.bytes_credited += pad
        census.details.append(f"{unit.get('name')}/{name}: +0x{pad:X} link tail padding")


def _unique_function(functions: list[FunctionBytes], *names: str) -> FunctionBytes | None:
    for name in names:
        if not name:
            continue
        hits = [fn for fn in functions if fn.name == name]
        if len(hits) == 1:
            return hits[0]
    return None


def _anon_target_equal(
    r_reloc: FunctionRelocation,
    d_reloc: FunctionRelocation,
    r_parsed: Any,
    d_parsed: Any,
    r_bytes: bytes,
    d_bytes: bytes,
) -> dict[str, Any] | None:
    """Prove two differently-named ``@N`` data relocs are the same datum.

    The target must be an anonymous local symbol defined in the same named,
    file-backed section at the same section offset (including the RELA
    addend).  The bytes from that offset to the shorter section end must be
    identical; that is the literal/pool *content* objdiff's ``data_value``
    mode fails to see when only the compiler-assigned number changed.  No
    linked-address guess is used: unknown targets fail closed.
    """
    if not (_ANON_LOCAL_RE.match(r_reloc.symbol) and _ANON_LOCAL_RE.match(d_reloc.symbol)):
        return None
    if (r_reloc.addend or 0) != (d_reloc.addend or 0):
        return None
    r_home = r_parsed.symbols.get(r_reloc.symbol)
    d_home = d_parsed.symbols.get(d_reloc.symbol)
    if not r_home or not d_home:
        return None
    r_sec = r_parsed.section_names.get(r_home[0])
    d_sec = d_parsed.section_names.get(d_home[0])
    if r_sec != d_sec or r_sec not in DATA_SECTIONS or r_sec in NOBITS:
        return None
    r_sh, d_sh = r_parsed.sections.get(r_sec), d_parsed.sections.get(r_sec)
    if not r_sh or not d_sh:
        return None
    off = r_home[1] + (r_reloc.addend or 0)
    if off != d_home[1] + (d_reloc.addend or 0):
        return None
    end = min(r_sh["size"], d_sh["size"])
    if not (0 <= off < end):
        return None
    r_data = r_bytes[r_sh["off"] + off: r_sh["off"] + end]
    d_data = d_bytes[d_sh["off"] + off: d_sh["off"] + end]
    if r_data != d_data:
        return None
    return {"section": r_sec, "offset": off,
            "retail_symbol": r_reloc.symbol, "decomp_symbol": d_reloc.symbol,
            "reloc_type": type_class(r_reloc.relocation_type), "content_bytes": len(r_data)}


def _credit_function(
    r_fn: FunctionBytes,
    d_fn: FunctionBytes,
    r_parsed: Any,
    d_parsed: Any,
    r_bytes: bytes,
    d_bytes: bytes,
) -> list[dict[str, Any]] | None:
    """Return proven anonymous-reloc credits, or None when not provable."""
    if r_fn.code != d_fn.code or len(r_fn.relocations) != len(d_fn.relocations):
        return None
    r_by = {r.offset: r for r in r_fn.relocations}
    d_by = {d.offset: d for d in d_fn.relocations}
    if set(r_by) != set(d_by) or len(r_by) != len(r_fn.relocations):
        return None
    credits = []
    for off in sorted(r_by):
        r_reloc, d_reloc = r_by[off], d_by[off]
        if r_reloc.relocation_type != d_reloc.relocation_type:
            return None
        if r_reloc.symbol == d_reloc.symbol:
            continue
        evidence = _anon_target_equal(r_reloc, d_reloc, r_parsed, d_parsed, r_bytes, d_bytes)
        if evidence is None:
            return None
        evidence["site"] = off
        credits.append(evidence)
    return credits or None


def _credit_functions(
    unit: dict[str, Any],
    r_path: Path,
    d_path: Path,
    r_parsed: Any,
    d_parsed: Any,
    r_bytes: bytes,
    d_bytes: bytes,
    census: CreditCensus,
) -> None:
    try:
        r_functions = list_text_functions(r_path)
        d_functions = list_text_functions(d_path)
    except (OSError, ElfSymbolError, ValueError, struct.error):
        return
    for fn in unit.get("functions", []):
        pct = fn.get("fuzzy_match_percent")
        if pct is None or float(pct) >= 100.0:
            continue
        census.functions_scanned += 1
        names = (fn.get("name"), (fn.get("metadata") or {}).get("demangled_name"))
        r_fn = _unique_function(r_functions, *names)
        d_fn = _unique_function(d_functions, *names)
        if r_fn is None or d_fn is None:
            continue
        credits = _credit_function(r_fn, d_fn, r_parsed, d_parsed, r_bytes, d_bytes)
        if not credits:
            continue
        _set_percent(fn, 100.0)
        fn["anonymous_reloc_credit"] = credits
        census.functions_credited += 1
        census.details.append(
            f"{unit.get('name')}/{fn.get('name')}: "
            + ", ".join(f"@{c['retail_symbol'].lstrip('@')}->"
                        f"@{c['decomp_symbol'].lstrip('@')} ({c['section']}+0x{c['offset']:X})"
                        for c in credits))


def credit_report(
    report: Mapping[str, Any],
    unit_objects: Mapping[str, tuple[Path, Path]],
    *,
    credit_anonymous_relocs: bool = True,
    source_report: str | Path | None = None,
) -> tuple[dict[str, Any], CreditCensus]:
    """Return an adjusted deep copy of *report* plus a credit census.

    ``unit_objects`` maps a report unit name to ``(retail_object, decomp_object)``.
    Units without both objects are skipped (never guessed).  The input report
    is left untouched; callers write the result to a *different* path.
    """
    if isinstance(report.get("credit"), dict):
        raise ValueError("report is already a credit-adjusted copy")
    adjusted = copy.deepcopy(dict(report))
    census = CreditCensus(units_scanned=len(adjusted.get("units", [])))
    dirty_units = 0
    for unit in adjusted.get("units", []):
        paths = unit_objects.get(unit.get("name"))
        if not paths:
            census.skipped_units += 1
            continue
        r_path, d_path = (Path(p) for p in paths)
        if not r_path.is_file() or not d_path.is_file():
            census.skipped_units += 1
            continue
        r_parsed, d_parsed = _parse(r_path), _parse(d_path)
        r_bytes, d_bytes = r_path.read_bytes(), d_path.read_bytes()
        before = (census.sections_credited, census.functions_credited)
        _credit_sections(unit, r_parsed, d_parsed, r_bytes, d_bytes, census)
        if credit_anonymous_relocs:
            _credit_functions(unit, r_path, d_path, r_parsed, d_parsed,
                              r_bytes, d_bytes, census)
        after_sections, after_functions = census.sections_credited, census.functions_credited
        if before != (after_sections, after_functions):
            dirty_units += 1
            _retally_unit(unit, functions_changed=after_functions != before[1],
                          sections_changed=after_sections != before[0])
    census.units_changed = dirty_units
    if dirty_units:
        _retally_report(adjusted)
    adjusted["credit"] = {
        "tool": "tools/coop/lib/report_credit.py",
        "version": 1,
        "source_report": str(source_report) if source_report else None,
        "anonymous_relocs": bool(credit_anonymous_relocs),
        "census": census.as_dict(),
    }
    return adjusted, census

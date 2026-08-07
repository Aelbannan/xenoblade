"""Catalog-wide source-presence scan (tools/coop).

For every retail function in the binary (via the objdiff unit list), check
whether a decompiled implementation exists: the decompiled object for the
same unit is parsed and matched by mangled symbol name against the retail
object (the same pairing the project uses for hexdiff/objdiff).  For each
retail function we record:

- ``in_source``      — a .text function with the same symbol exists in the
                       decompiled object (i.e. a reconstruction was written
                       and compiled at the time of the last build);
- ``stale``          — the decompiled object is older than the owning source
                       file, so the scan result may not reflect current
                       source (rebuild with hexdiff/ninja to refresh);
- ``byte_identical`` — both sides are relocation-free and the raw bodies
                       are byte-identical (a cheap FULL_MATCH signal).  None
                       when either side carries relocations (reloc sites are
                       linker placeholders, so raw byte equality is not
                       meaningful there).

The scan is read-only over existing build artifacts (``build/<region>/obj``
retail objects and ``build/<region>/src`` decompiled objects) using the
ppc_equivalence ELF parser — the same one hexdiff uses.  It never builds.

Caveats (recorded per-unit in the report):

- Functions written under a *different* name than the retail symbol (e.g. a
  recovered semantic name whose retail symbols.txt entry was not renamed in
  sync) will be reported absent even though source exists.  The project's
  ``symbols rename-*`` tooling keeps the two sides aligned, so this should
  be rare; the decompiled-only list in the report surfaces the extras.
- Template/inline instantiations emitted in a different TU than the retail
  symbol's unit (e.g. ``__ct__Q22ml10FixStr<64>Fv``) will be reported absent
  in their retail unit.  Check the unit's decompiled-only list.
"""

from __future__ import annotations

import json
import re
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Optional

from tools.coop.lib.project import ObjdiffUnit, Project
from tools.coop.reloc_map import analyze_function_pair
from tools.ppc_equivalence.elf_symbols import ElfSymbolError, list_text_functions


@dataclass
class FunctionSourceStatus:
    unit: str
    symbol: str
    size: int
    in_source: bool
    stale: bool = False
    unbuilt: bool = False
    """True when the decompiled object does not exist but the source file does
    (never built / stale build tree): source-level presence only."""
    byte_identical: Optional[bool] = None
    reloc_compatible: Optional[bool] = None
    """True when both sides carry relocations but the bodies match with the
    reloc sites masked out (same size, same reloc offsets+types, equal bytes
    elsewhere).  A strong FULL_MATCH candidate — not proof, because reloc
    symbol names/addends could still differ (name-drift / link-address
    cases).  Confirm with hexdiff/cycle before accepting."""
    decompiled_only: bool = False
    """True for symbols present in the decompiled object but not in retail."""


@dataclass
class UnitSourceScan:
    unit: str
    retail_path: Optional[Path]
    decomp_path: Optional[Path]
    source_path: Optional[Path]
    scanned: bool
    error: str = ""
    statuses: list[FunctionSourceStatus] = field(default_factory=list)

    @property
    def retail_count(self) -> int:
        return len(self.statuses)

    @property
    def in_source_count(self) -> int:
        return sum(1 for s in self.statuses if s.in_source)

    @property
    def byte_identical_count(self) -> int:
        return sum(1 for s in self.statuses if s.byte_identical)


def _obj_mtime(path: Path) -> Optional[float]:
    try:
        return path.stat().st_mtime
    except OSError:
        return None


def _source_is_newer(obj_mtime: Optional[float], source: Optional[Path]) -> bool:
    if obj_mtime is None or source is None:
        return False
    try:
        return source.stat().st_mtime > obj_mtime
    except OSError:
        return False


def _reloc_masked_identical(ret: Any, dec: Any) -> bool:
    """Same size, same reloc offsets+types, equal bytes outside reloc sites."""
    if ret.size != dec.size:
        return False
    ro = sorted(r.offset for r in ret.relocations)
    do = sorted(r.offset for r in dec.relocations)
    if ro != do:
        return False
    rtype = {r.offset: r.relocation_type for r in ret.relocations}
    dtype = {r.offset: r.relocation_type for r in dec.relocations}
    if any(rtype.get(o) != dtype.get(o) for o in ro):
        return False
    mask = set(ro)
    for i in range(0, ret.size, 4):
        if i in mask:
            continue
        if ret.code[i : i + 4] != dec.code[i : i + 4]:
            return False
    return True


def _resolve_decomp_match(
    decomp_by_name: dict[str, Any], symbol: str
) -> tuple[Optional[Any], Optional[str]]:
    """Resolve a retail symbol against the decompiled object's symbol table.

    Mirrors the project's pairing cascade (exact -> case-insensitive ->
    substring -> digit-strip) but requires a UNIQUE match: ambiguous
    fallbacks (e.g. a bare ``Draw`` matching many ``Draw__...`` bodies) are
    not silently paired and return ``(None, reason)``.

    Returns ``(decomp_function, None)`` on a unique match, or
    ``(None, reason)`` explaining why no reliable pair exists.
    """
    # 1. exact
    fn = decomp_by_name.get(symbol)
    if fn is not None:
        return fn, None
    # 2. case-insensitive exact
    lowered = symbol.lower()
    ci = [f for n, f in decomp_by_name.items() if n.lower() == lowered]
    if len(ci) == 1:
        return ci[0], None
    if len(ci) > 1:
        return None, "ambiguous case-insensitive match"
    # 3. plain substring (registry short form vs object full mangling),
    #    both directions, uniqueness enforced
    sub = [
        f
        for n, f in decomp_by_name.items()
        if lowered in n.lower() or n.lower() in lowered
    ]
    if len(sub) == 1:
        return sub[0], None
    if len(sub) > 1:
        return None, "ambiguous substring match"
    # 4. digit-strip (Itanium length prefixes) as last resort
    def _strip_digits(s: str) -> str:
        return re.sub(r"\d+", "", s)

    stripped = _strip_digits(lowered)
    if len(stripped) >= 5:
        hits = [
            f
            for n, f in decomp_by_name.items()
            if stripped in _strip_digits(n.lower())
        ]
        if len(hits) == 1:
            return hits[0], None
        if len(hits) > 1:
            return None, "ambiguous digit-strip match"
    return None, "no match"


def _list_functions_any(path: Path) -> list[Any]:
    """List functions from .text, falling back to .init (boot-vector objects)."""
    try:
        return list_text_functions(path, ".text")
    except ElfSymbolError:
        return list_text_functions(path, ".init")


def scan_unit(unit: ObjdiffUnit) -> UnitSourceScan:
    """Compare one retail/decompiled object pair by symbol name."""
    retail_o = unit.target_path
    decomp_o = unit.base_path
    source = unit.source_path
    scan = UnitSourceScan(
        unit=unit.name,
        retail_path=retail_o,
        decomp_path=decomp_o,
        source_path=source,
        scanned=False,
    )
    if retail_o is None or not retail_o.is_file():
        scan.error = "retail object missing (not split?)"
        return scan
    try:
        retail_funcs = _list_functions_any(retail_o)
    except ElfSymbolError as exc:
        scan.error = f"retail object has no .text/.init ({exc})"
        return scan

    # No decompiled object: fall back to source-file presence (the object may
    # simply never have been built — stale/incomplete build tree).
    if decomp_o is None or not decomp_o.is_file():
        source_exists = source is not None and source.is_file()
        for fn in retail_funcs:
            scan.statuses.append(
                FunctionSourceStatus(
                    unit=unit.name,
                    symbol=fn.name,
                    size=fn.size,
                    in_source=source_exists,
                    unbuilt=source_exists,
                )
            )
        scan.scanned = True
        scan.error = (
            "decompiled object missing; source present (unbuilt)"
            if source_exists
            else "decompiled object missing (no source / never built)"
        )
        return scan

    try:
        decomp_funcs = _list_functions_any(decomp_o)
    except ElfSymbolError as exc:
        scan.error = f"decompiled object unparseable ({exc})"
        return scan

    decomp_by_name: dict[str, Any] = {}
    for fn in decomp_funcs:
        decomp_by_name.setdefault(fn.name, fn)

    decomp_o_mtime = _obj_mtime(decomp_o)
    stale = _source_is_newer(decomp_o_mtime, source)

    for fn in retail_funcs:
        dec, reason = _resolve_decomp_match(decomp_by_name, fn.name)
        if dec is not None:
            # consume: one decompiled body pairs with one retail function
            decomp_by_name.pop(dec.name, None)
        byte_identical: Optional[bool] = None
        reloc_compatible: Optional[bool] = None
        if dec is not None:
            if not fn.relocations and not dec.relocations:
                byte_identical = fn.code == dec.code
            else:
                # Reloc-carrying pair: FULL_MATCH requires the non-reloc bytes
                # to match (masked check) AND every reloc site to carry the
                # same word and the same symbol (the word holds the addend;
                # a same-symbol register difference would otherwise slip
                # through the masked comparison).
                masked_ok = _reloc_masked_identical(fn, dec)
                d_by_off = {r.offset: r for r in dec.relocations}
                words_ok = True
                syms_ok = True
                for off, r in sorted((r.offset, r) for r in fn.relocations):
                    d = d_by_off.get(off)
                    if d is None or d.relocation_type != r.relocation_type:
                        words_ok = False
                        break
                    rw = int.from_bytes(fn.code[off : off + 4], "big")
                    dw = int.from_bytes(dec.code[off : off + 4], "big")
                    if rw != dw:
                        words_ok = False
                    if (r.symbol or "") != (d.symbol or ""):
                        syms_ok = False
                if masked_ok and words_ok and syms_ok:
                    byte_identical = True  # byte-identical incl. reloc names
                elif masked_ok:
                    # code identical; only name/addend drift remains (fixable)
                    kinds = {d.kind for d in analyze_function_pair(fn, dec)}
                    reloc_compatible = kinds <= {"name", "addend"}
                else:
                    reloc_compatible = False
        scan.statuses.append(
            FunctionSourceStatus(
                unit=unit.name,
                symbol=fn.name,
                size=fn.size,
                in_source=dec is not None,
                stale=stale if dec is not None else False,
                byte_identical=byte_identical,
                reloc_compatible=reloc_compatible,
            )
        )
    # Decompiled symbols with no retail counterpart (extras: FORCEACTIVE
    # stubs, dtors emitted in another TU, renamed functions, ...).
    for name, fn in decomp_by_name.items():
        scan.statuses.append(
            FunctionSourceStatus(
                unit=unit.name,
                symbol=name,
                size=fn.size,
                in_source=True,
                stale=stale,
                byte_identical=None,
                decompiled_only=True,
            )
        )
    scan.scanned = True
    return scan


def scan_project(project: Project) -> dict[str, UnitSourceScan]:
    """Scan every objdiff unit.  Never builds; parses existing objects."""
    by_unit: dict[str, UnitSourceScan] = {}
    for unit in project.load_objdiff_units():
        by_unit[unit.name] = scan_unit(unit)
    return by_unit


def summarize(by_unit: dict[str, UnitSourceScan]) -> dict[str, Any]:
    units = len(by_unit)
    scanned_units = sum(1 for u in by_unit.values() if u.scanned)
    # retail functions = statuses paired from the retail symbol table
    # (decompiled-only extras are reported separately below)
    retail_total = sum(
        1 for u in by_unit.values() for s in u.statuses if not s.decompiled_only
    )
    in_source = sum(
        1 for u in by_unit.values() for s in u.statuses if s.in_source and not s.decompiled_only
    )
    byte_identical = sum(1 for u in by_unit.values() for s in u.statuses if s.byte_identical)
    stale = sum(1 for u in by_unit.values() for s in u.statuses if s.stale)
    unbuilt = sum(1 for u in by_unit.values() for s in u.statuses if s.unbuilt)
    absent = retail_total - in_source
    decompiled_only = sum(
        1 for u in by_unit.values() for s in u.statuses if s.decompiled_only
    )
    reloc_compatible = sum(
        1 for u in by_unit.values() for s in u.statuses if s.reloc_compatible
    )
    return {
        "units": units,
        "scanned_units": scanned_units,
        "unscanned_units": units - scanned_units,
        "retail_functions": retail_total,
        "in_source": in_source,
        "absent_from_source": absent,
        "stale_objects": stale,
        "unbuilt_source": unbuilt,
        "byte_identical": byte_identical,
        "reloc_compatible": reloc_compatible,
        "decompiled_only": decompiled_only,
    }


def symbol_status_map(by_unit: dict[str, UnitSourceScan]) -> dict[str, FunctionSourceStatus]:
    """Symbol name -> status, for registry updates (symbols are unique)."""
    out: dict[str, FunctionSourceStatus] = {}
    for unit_scan in by_unit.values():
        for st in unit_scan.statuses:
            if not st.decompiled_only:
                out[st.symbol] = st
    return out


def write_scan_report(
    project: Project,
    by_unit: dict[str, UnitSourceScan],
    report_path: Path,
) -> Path:
    """Write the catalog-wide scan to build/<region>/coop-source-scan.json."""
    payload: dict[str, Any] = {
        "version": 1,
        "region": project.config.region,
        "generated_at": datetime.now(timezone.utc).isoformat(timespec="seconds"),
        "summary": summarize(by_unit),
        "units": {},
    }
    for unit_name, scan in by_unit.items():
        payload["units"][unit_name] = {
            "scanned": scan.scanned,
            "error": scan.error,
            "retail": str(scan.retail_path) if scan.retail_path else None,
            "decomp": str(scan.decomp_path) if scan.decomp_path else None,
            "source": str(scan.source_path) if scan.source_path else None,
            "functions": [
                {
                    "symbol": st.symbol,
                    "size": st.size,
                    "in_source": st.in_source,
                    "stale": st.stale,
                    "unbuilt": st.unbuilt,
                    "byte_identical": st.byte_identical,
                    "reloc_compatible": st.reloc_compatible,
                    "decompiled_only": st.decompiled_only,
                }
                for st in sorted(scan.statuses, key=lambda s: s.symbol)
            ],
        }
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(
        json.dumps(payload, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )
    return report_path


def apply_to_registry(
    config: Any,
    by_symbol: dict[str, FunctionSourceStatus],
    *,
    dry_run: bool,
) -> int:
    """Set ``source_present`` on every target row with a scanned symbol.

    Matches by retail symbol name (catalog rows were imported from
    symbols.txt, whose names equal the retail object symbols).  Returns the
    number of rows whose value changed.  With ``dry_run`` nothing is written.
    """
    from tools.coop.lib.targets import locked_targets_document

    changed = 0
    with locked_targets_document(config) as (data, write):
        for row in data.get("targets") or []:
            sym = row.get("symbol")
            if not sym or sym not in by_symbol:
                continue
            present = by_symbol[sym].in_source
            if row.get("source_present") != present:
                row["source_present"] = present
                changed += 1
        if changed and not dry_run:
            write()
    return changed

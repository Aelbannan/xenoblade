#!/usr/bin/env python3
"""Relocation name-drift detection and named-symbol map mining (PLAN.md §17.6,
docs/MWCC_REFERENCE.md §1 — "Relocation name drift (99.3-99.9%, instructions
identical)").

The #1 cause of near-misses: instruction bytes are identical but the relocation
*symbol names* differ (retail ``lbl_eu_806656A0`` vs decomp ``spInstance__9CDeviceGX``,
TU-local ``@N`` float pools, ``@stringBase0`` offsets). This tool:

  * detects and classifies reloc drift on a per-function or per-unit basis,
  * mines the repo-wide named-symbol map (decomp symbol → retail symbol +
    addend delta) from every retail/decomp objdiff unit pair,
  * emits concrete fix suggestions (source ``extern "C"`` declaration or
    ``postprocess_reloc_names.py`` ``exact_renames`` rule) for each drift.

Subcommands:

  diff <unit> --symbol <sym>   per-function reloc drift + suggestions
  mine [--out PATH]            batch-mine the map across all objdiff units
  show [--map PATH]            pretty-print the mined map

The map is consumed by ``tools/coop/hexdiff.py`` (``reloc_drift`` section /
JSON key) to surface one-line fixes inside the rapid feedback loop.
"""

from __future__ import annotations

import argparse
import json
import sys
import time
from collections import defaultdict
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import Optional

# Ensure the repo root is on sys.path.
_REPO = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(_REPO))

from tools.coop.lib.config import load_config
from tools.coop.lib.project import Project
from tools.ppc_equivalence.census_elf_relocs import relocation_type_name
from tools.ppc_equivalence.elf_symbols import (
    extract_allocatable_section,
    list_text_functions,
)

DEFAULT_MAP = _REPO / "tools" / "coop" / "retail_reloc_map.json"
MAP_VERSION = 1

# ── reloc kind classification ──────────────────────────────────────────────

# Call relocs (callee mangling) are handled by the symbol-recovery pipeline
# (MWCC_REFERENCE §2); the reloc map keeps them separate.
_CALL_TYPES = frozenset({10, 18, 23})  # REL24, PLTREL24, LOCAL24PC
_BRANCH_TYPES = frozenset({7, 8, 9, 11, 12, 13})  # ADDR14, REL14 (±BRTAKEN/NTAKEN)


def reloc_kind(reloc_type: int) -> str:
    if reloc_type in _CALL_TYPES:
        return "call"
    if reloc_type in _BRANCH_TYPES:
        return "branch"
    return "data"


# In-place addend field masks for REL (MWCC) objects. For each reloc type the
# addend lives in a fixed bit field of the instruction/data word; the rest of
# the word is the opcode + register operands (or address bits for ADDR32/24).
_ADDEND_MASKS: dict[int, int] = {
    1: 0xFFFFFFFF,  # ADDR32
    2: 0x00FFFFFF,  # ADDR24
    3: 0x0000FFFF,  # ADDR16
    4: 0x0000FFFF,  # ADDR16_LO
    5: 0x0000FFFF,  # ADDR16_HI
    6: 0x0000FFFF,  # ADDR16_HA
    7: 0x0000FFFC,  # ADDR14
    8: 0x0000FFFC,  # ADDR14_BRTAKEN
    9: 0x0000FFFC,  # ADDR14_BRNTAKEN
    10: 0x03FFFFFC,  # REL24
    11: 0x0000FFFC,  # REL14
    12: 0x0000FFFC,  # REL14_BRTAKEN
    13: 0x0000FFFC,  # REL14_BRNTAKEN
    24: 0xFFFFFFFF,  # UADDR32
    25: 0x0000FFFF,  # UADDR16
    32: 0x0000FFFF,  # SDAREL16
    33: 0x0000FFFF,  # SECTOFF
    34: 0x0000FFFF,  # SECTOFF_LO
    35: 0x0000FFFF,  # SECTOFF_HI
    36: 0x0000FFFF,  # SECTOFF_HA
    101: 0xFFFFFFFF,  # EMB_NADDR32
    102: 0x0000FFFF,  # EMB_NADDR16
    103: 0x0000FFFF,  # EMB_NADDR16_LO
    104: 0x0000FFFF,  # EMB_NADDR16_HI
    105: 0x0000FFFF,  # EMB_NADDR16_HA
    106: 0x0000FFFF,  # EMB_SDAI16
    107: 0x0000FFFF,  # EMB_SDA2I16
    109: 0x0000FFFF,  # EMB_SDA21
    110: 0x0000FFFF,  # EMB_SDA2I16 (dup)
    111: 0x0000FFFF,  # EMB_SDA2REL (dup)
    112: 0x0000FFFF,  # EMB_SDA2I16 (dup)
}


def inplace_addend(word: int, reloc_type: int) -> Optional[int]:
    """Extract the in-place addend field of ``word`` for a REL relocation."""
    mask = _ADDEND_MASKS.get(reloc_type)
    if mask is None:
        return None
    return word & mask


def addend_field_mask(reloc_type: int) -> Optional[int]:
    return _ADDEND_MASKS.get(reloc_type)


@dataclass(frozen=True)
class RelocDrift:
    """One classified reloc difference between retail and decomp at an offset."""

    offset: int
    reloc_type: int
    retail_symbol: str
    decomp_symbol: str
    # "name"    → symbols differ, instruction bytes identical (pure rename)
    # "addend"  → symbols differ, only the addend field differs (rename + delta)
    # "layout"  → same symbol, only the addend field differs (pool offset drift;
    #             report-only, not a map candidate)
    # "structural" → other fields differ (not reloc-name fixable)
    # "type"    → both sides have a reloc at the same offset but a different
    #             relocation_type (bytes may match; linker semantics differ)
    # "presence" → reloc at an offset on ONE side only (the other has none)
    kind: str
    retail_addend: Optional[int]
    decomp_addend: Optional[int]
    # Site-drift types (set for kind="type" — the reloc_type field then holds
    # the retail side's type; for kind="presence" the present side's type).
    retail_type: Optional[int] = None
    decomp_type: Optional[int] = None

    @property
    def addend_delta(self) -> Optional[int]:
        if self.retail_addend is None or self.decomp_addend is None:
            return None
        return self.decomp_addend - self.retail_addend

    @property
    def type_name(self) -> str:
        return relocation_type_name(self.reloc_type)

    @property
    def retail_type_name(self) -> str:
        return relocation_type_name(self.retail_type) if self.retail_type is not None else "—"

    @property
    def decomp_type_name(self) -> str:
        return relocation_type_name(self.decomp_type) if self.decomp_type is not None else "—"

    @property
    def category(self) -> str:
        return reloc_kind(self.reloc_type)

    def to_dict(self) -> dict:
        return {
            "offset": self.offset,
            "reloc_type": self.reloc_type,
            "type": self.type_name,
            "retail_symbol": self.retail_symbol,
            "decomp_symbol": self.decomp_symbol,
            "kind": self.kind,
            "category": self.category,
            "retail_addend": self.retail_addend,
            "decomp_addend": self.decomp_addend,
            "addend_delta": self.addend_delta,
            "retail_type": self.retail_type_name,
            "decomp_type": self.decomp_type_name,
        }


def classify_drift(
    reloc_type: int,
    retail_symbol: str,
    decomp_symbol: str,
    retail_word: int,
    decomp_word: int,
) -> Optional[RelocDrift]:
    """Classify one retail/decomp reloc pair at the same offset + type.

    Returns ``None`` when there is no reloc-name fixable drift (symbols match,
    or the difference is structural / outside the addend field).
    """
    if not retail_symbol or not decomp_symbol:
        # Section-relative relocs (empty symbol) are not name-fixable.
        return None

    mask = addend_field_mask(reloc_type)
    r_add = inplace_addend(retail_word, reloc_type)
    d_add = inplace_addend(decomp_word, reloc_type)

    if retail_word == decomp_word:
        if retail_symbol == decomp_symbol:
            # Same symbol, identical bytes: no drift at all.
            return None
        return RelocDrift(
            offset=0,  # filled by caller
            reloc_type=reloc_type,
            retail_symbol=retail_symbol,
            decomp_symbol=decomp_symbol,
            kind="name",
            retail_addend=r_add,
            decomp_addend=d_add,
        )

    # Words differ: drift is reloc-name fixable only when the difference is
    # confined to the addend field.
    if mask is not None and (retail_word & ~mask) == (decomp_word & ~mask):
        if r_add is None or d_add is None or r_add == d_add:
            # Same addend but different opcode/operands elsewhere: structural.
            return None
        if retail_symbol == decomp_symbol:
            kind = "layout"  # same symbol, offset drift (string pools etc.)
        else:
            kind = "addend"
        return RelocDrift(
            offset=0,
            reloc_type=reloc_type,
            retail_symbol=retail_symbol,
            decomp_symbol=decomp_symbol,
            kind=kind,
            retail_addend=r_add,
            decomp_addend=d_add,
        )

    # Non-addend bits differ: structural, not reloc-name fixable.
    if retail_symbol == decomp_symbol:
        return None
    return RelocDrift(
        offset=0,
        reloc_type=reloc_type,
        retail_symbol=retail_symbol,
        decomp_symbol=decomp_symbol,
        kind="structural",
        retail_addend=r_add,
        decomp_addend=d_add,
    )


# ── pair analysis ──────────────────────────────────────────────────────────


def _drifts_from_pairs(unit_name: str, r_by_off, d_by_off, r_data, r_size, d_data, d_size) -> list[RelocDrift]:
    drifts: list[RelocDrift] = []
    for off in sorted(set(r_by_off) | set(d_by_off)):
        r = r_by_off.get(off)
        d = d_by_off.get(off)
        if off + 4 > r_size or off + 4 > d_size:
            continue
        # Presence drift: reloc at this offset on one side only. Previously
        # skipped (d is None -> continue); now reported so agents can see a
        # reloc the retail resolves inline (or vice versa) — the reloc-site
        # gate that byte-identity alone cannot reveal.
        if r is None or d is None:
            if r is None and d is None:
                continue
            present = r if r is not None else d
            drifts.append(
                RelocDrift(
                    offset=off,
                    reloc_type=present.relocation_type,
                    retail_symbol=(r.symbol or "") if r else "",
                    decomp_symbol=(d.symbol or "") if d else "",
                    kind="presence",
                    retail_addend=r.addend if r else None,
                    decomp_addend=d.addend if d else None,
                    retail_type=r.relocation_type if r else None,
                    decomp_type=d.relocation_type if d else None,
                )
            )
            continue
        # Type drift: same offset, both sides have a reloc, different type
        # (HI vs HA, REL24 vs ADDR24, ...). Bytes may match while the linked
        # value differs — the reloc-site gate. Previously skipped.
        if d.relocation_type != r.relocation_type:
            drifts.append(
                RelocDrift(
                    offset=off,
                    reloc_type=r.relocation_type,
                    retail_symbol=r.symbol or "",
                    decomp_symbol=d.symbol or "",
                    kind="type",
                    retail_addend=r.addend,
                    decomp_addend=d.addend,
                    retail_type=r.relocation_type,
                    decomp_type=d.relocation_type,
                )
            )
            continue
        r_word = int.from_bytes(r_data[off:off + 4], "big")
        d_word = int.from_bytes(d_data[off:off + 4], "big")
        drift = classify_drift(
            r.relocation_type, r.symbol or "", d.symbol or "", r_word, d_word
        )
        if drift is None:
            continue
        drifts.append(
            RelocDrift(
                offset=off,
                reloc_type=drift.reloc_type,
                retail_symbol=drift.retail_symbol,
                decomp_symbol=drift.decomp_symbol,
                kind=drift.kind,
                retail_addend=drift.retail_addend,
                decomp_addend=drift.decomp_addend,
            )
        )
    return drifts


def analyze_text_pair(retail_obj: Path, decomp_obj: Path) -> list[RelocDrift]:
    """Analyze one retail/decomp .o pair at the .text section level."""
    try:
        r_sec = extract_allocatable_section(retail_obj, ".text")
        d_sec = extract_allocatable_section(decomp_obj, ".text")
    except Exception:
        return []
    r_by_off = {r.offset: r for r in r_sec.relocations}
    d_by_off = {r.offset: r for r in d_sec.relocations}
    return _drifts_from_pairs(
        retail_obj.name, r_by_off, d_by_off,
        r_sec.data, r_sec.size, d_sec.data, d_sec.size,
    )


def analyze_function_pair(retail_fn, decomp_fn) -> list[RelocDrift]:
    """Analyze one function-level retail/decomp pair (FunctionBytes objects)."""
    r_by_off = {r.offset: r for r in retail_fn.relocations}
    d_by_off = {r.offset: r for r in decomp_fn.relocations}
    return _drifts_from_pairs(
        retail_fn.name, r_by_off, d_by_off,
        retail_fn.code, len(retail_fn.code), decomp_fn.code, len(decomp_fn.code),
    )


# ── map load / save ────────────────────────────────────────────────────────


def load_map(path: Path | str = DEFAULT_MAP) -> dict:
    path = Path(path)
    if not path.is_file():
        return {"version": MAP_VERSION, "region": "", "count": 0, "entries": {}}
    with path.open(encoding="utf-8") as f:
        data = json.load(f)
    if data.get("version") != MAP_VERSION:
        return {"version": MAP_VERSION, "region": "", "count": 0, "entries": {}}
    data.setdefault("entries", {})
    return data


def save_map(data: dict, path: Path | str = DEFAULT_MAP) -> None:
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    with tmp.open("w", encoding="utf-8") as f:
        json.dump(data, f, indent=2, sort_keys=True)
        f.write("\n")
    tmp.replace(path)


def ensure_fresh(
    project: Project,
    *,
    force: bool = False,
    out: Path | str = DEFAULT_MAP,
) -> bool:
    """Re-mine the reloc map iff it is stale; return True when re-mined.

    Staleness (doc 33 §0.5): the map is fresh iff it exists, carries a
    ``generated`` timestamp, and NO objdiff retail/decomp ``.o`` pair has an
    mtime newer than ``generated``.  A stale map silently breaks witness gate 2
    (TU-local ``@N`` labels shift on every rebuild) and — worse — can alias a
    rebuilt ``@N`` to the wrong retail name (a false-certificate input, F1).
    Re-mining is cheap (~2 s wall, no build lock).

    Concurrency: ``mine()`` writes atomically (tmp + replace) and takes no
    build lock, so concurrent agents re-mining cannot corrupt each other or
    deadlock with hexdiff builds.  A rebuild that lands inside the mine window
    is caught by the witness retry belt (prove_unit_symbol), not here.
    """
    out = Path(out)
    map_data = load_map(out)
    generated = map_data.get("generated")
    if force or not generated:
        data = mine(project, include_kinds={"data"})
        save_map(data, out)
        return True
    try:
        gen_dt = datetime.fromisoformat(generated)
    except ValueError:
        data = mine(project, include_kinds={"data"})
        save_map(data, out)
        return True
    stale = False
    for unit in project.load_objdiff_units():
        for p in (unit.target_path, unit.base_path):
            if p is None or not p.is_file():
                continue
            try:
                mtime_dt = datetime.fromtimestamp(p.stat().st_mtime)
                if gen_dt.tzinfo is not None:
                    # Aware stamp: compare in the stamp's wall-clock zone so a
                    # tz-aware stamp cannot raise (round-4 NIT).
                    mtime_dt = mtime_dt.replace(tzinfo=gen_dt.tzinfo)
                if mtime_dt > gen_dt:
                    stale = True
                    break
            except (OSError, TypeError):
                continue
        if stale:
            break
    if not stale:
        return False
    data = mine(project, include_kinds={"data"})
    save_map(data, out)
    return True


def _is_section_symbol(symbol: str) -> bool:
    """MWCC emits relocs against section symbols (``...bss.0``, ``...data.0``)
    when the source references data without a named variable. Unit-local."""
    return symbol.startswith("...")


def _is_tu_local(symbol: str) -> bool:
    """MWCC TU-local labels (``@N`` pools, ``@LOCAL@...``, section symbols)
    are meaningless across translation units; they need content matching, not
    name mapping."""
    return (
        symbol.startswith("@")
        or "@LOCAL@" in symbol
        or _is_section_symbol(symbol)
    )


# Addend deltas beyond this are misaligned-pair noise (different fields/vars),
# not rename candidates; they stay in `diff` reports but not in the map.
MAX_MAP_ADDEND_DELTA = 0x40


def map_key_for(drift: RelocDrift, unit_name: str) -> str:
    """Global key for named symbols; unit-scoped key for TU-local labels."""
    if _is_tu_local(drift.decomp_symbol):
        return f"{unit_name}@{drift.decomp_symbol.lstrip('@')}"
    return drift.decomp_symbol


def is_unit_scoped_key(key: str) -> bool:
    return "@" in key


# ── mining ─────────────────────────────────────────────────────────────────


def mine(project: Project, *, include_kinds: set[str], limit: int | None = None) -> dict:
    """Aggregate reloc drift across every objdiff unit pair into a map.

    Drift is analyzed per **function pair** (matched by name with equal .text
    sizes) so reloc offsets align meaningfully — section-level alignment
    produces false positives when retail/decomp function order or sizes
    differ. ``include_kinds`` filters which drift categories become map
    entries (default data-only; call/branch kinds are separate pipelines).
    """
    units = project.load_objdiff_units()
    agg: dict[str, dict] = defaultdict(lambda: defaultdict(list))
    total_units = 0
    scanned = 0
    pair_count = 0
    # Stamp ``generated`` at START (not at write time) with sub-second
    # precision: the timestamp is the freshness watermark compared against .o
    # mtimes (``ensure_fresh`` / prove_unit_symbol); a second-resolution stamp
    # at end would miss rebuilds during the mine window, and a second-
    # resolution stamp at start spuriously re-mines objects built in the same
    # second (round-4 finding).
    generated = datetime.now().isoformat()
    for unit in units:
        if unit.target_path is None or unit.base_path is None:
            continue
        if not unit.target_path.is_file() or not unit.base_path.is_file():
            continue
        try:
            retail_fns = list_text_functions(unit.target_path)
            decomp_fns = list_text_functions(unit.base_path)
        except Exception:
            continue
        decomp_by_name: dict[str, list] = {}
        for fn in decomp_fns:
            decomp_by_name.setdefault(fn.name, []).append(fn)
        for rfn in retail_fns:
            matches = decomp_by_name.get(rfn.name)
            if not matches:
                continue
            dfn = matches[0]
            if dfn.size != rfn.size:
                # Offsets inside the function are not comparable when sizes
                # differ (scheduling shifts the stream) — skip, don't guess.
                continue
            drifts = analyze_function_pair(rfn, dfn)
            pair_count += 1
            for drift in drifts:
                if drift.kind not in ("name", "addend"):
                    continue
                if drift.category not in include_kinds:
                    continue
                if drift.addend_delta is not None and abs(drift.addend_delta) > MAX_MAP_ADDEND_DELTA:
                    # Misaligned-pair noise: different fields/variables, not rename.
                    continue
                key = map_key_for(drift, unit.name)
                agg[key][drift.type_name].append(
                    (drift.retail_symbol, drift.retail_addend, drift.decomp_addend, unit.name)
                )
        scanned += 1
        if limit is not None and scanned >= limit:
            break

    entries: dict[str, dict] = {}
    for key, by_type in sorted(agg.items()):
        for type_name, records in sorted(by_type.items()):
            # Majority retail symbol for this (key, type).
            by_sym: dict[str, list] = defaultdict(list)
            for retail_sym, r_add, d_add, unit_name in records:
                by_sym[retail_sym].append((r_add, d_add, unit_name))
            best_sym = max(by_sym, key=lambda s: len(by_sym[s]))
            best = by_sym[best_sym]
            r_add = best[0][0]
            d_add = best[0][1]
            delta = d_add - r_add if (r_add is not None and d_add is not None) else None
            alternatives = [
                {"retail_symbol": s, "count": len(recs)}
                for s, recs in sorted(by_sym.items(), key=lambda kv: -len(kv[1]))
                if s != best_sym
            ]
            entry = {
                "retail_symbol": best_sym,
                "retail_addend": r_add,
                "addend_delta": delta,
                "kind": "call" if type_name in _CALL_TYPES else "branch" if type_name in _BRANCH_TYPES else "data",
                "count": len(records),
                "units": sorted({rec[2] for rec in best})[:8],
                "alternatives": alternatives,
            }
            if is_unit_scoped_key(key):
                # TU-local entries stay unit-scoped; surface only confident ones.
                entry["scoped"] = True
            entries.setdefault(key, {})[type_name] = entry

    return {
        "version": MAP_VERSION,
        "region": project.config.region,
        "generated": generated,
        "units_scanned": scanned,
        "function_pairs": pair_count,
        "count": sum(len(v) for v in entries.values()),
        "entries": entries,
    }


# ── suggestions ────────────────────────────────────────────────────────────


def _lookup(drift: RelocDrift, unit_name: str, reloc_map: dict) -> Optional[dict]:
    if not reloc_map:
        return None
    entries = reloc_map.get("entries", {})
    for key in (map_key_for(drift, unit_name), drift.decomp_symbol):
        sub = entries.get(key)
        if sub:
            return sub.get(drift.type_name) or sub.get("R_PPC_EMB_SDA21") or next(iter(sub.values()))
    return None


def suggestions(drift: RelocDrift, unit_name: str, decomp_obj_name: str, reloc_map: dict) -> list[str]:
    """Concrete fix lines for one drift (empty when nothing to suggest)."""
    if drift.kind == "type":
        return [
            f"reloc TYPE differs at 0x{drift.offset:04x} "
            f"(retail {drift.retail_type_name} vs decomp {drift.decomp_type_name}) "
            f"— bytes may match but the linker-generated value's reloc class differs",
            f"  fix: reloc class follows the expression/operand shape, not a builtin — "
            f"`@ha` vs `@hi` (sign-corrected high half) is chosen by how the address is "
            f"formed, and SDA-eligibility (EMB_SDA21/SDA16) vs absolute (ADDR16/ADDR24) "
            f"follows from which section the referenced symbol lives in (.sdata/.sdata2 "
            f"vs ordinary data) and how the address is computed "
            f"(docs/MWCC_REFERENCE.md §1a SDA globals / §1b float pools)",
            f"  NOT reloc-NAME fixable — no extern \"C\" rename applies",
        ]
    if drift.kind == "presence":
        # retail_type is None ⇔ retail has no reloc at this offset (a
        # section/null-symbol reloc decodes to "" on its present side, so the
        # symbol strings cannot determine the side).
        side = "decomp" if drift.retail_type is None else "retail"
        other = "retail" if side == "decomp" else "decomp"
        sym = drift.decomp_symbol or drift.retail_symbol
        lines = [
            f"reloc present on {side} side ONLY at 0x{drift.offset:04x} "
            f"({sym} [{drift.type_name}]) — {other} has no reloc at this offset",
        ]
        if side == "decomp":
            # Retail has NO reloc here — the reloc-site gate requires decomp to
            # have none either. Adding/referencing an extern only KEEPS the
            # reloc and cannot fix this; the reloc must be removed.
            lines.append(
                f"  decomp emits a reloc where retail resolves the value INLINE — the fix is "
                f"to make decomp emit NO reloc at this offset: inline the constant/value or "
                f"restructure the expression so the reference is resolved at compile time "
                f"(do NOT add an extern reference — that keeps the reloc and cannot pass "
                f"the reloc-site gate)"
            )
        else:
            lines.append(
                f"  retail references a linker symbol decomp resolved INLINE — add an "
                f"extern reference to that retail name so MWCC emits the reloc"
            )
        lines.append(
            f"  NOT reloc-NAME fixable; check the expression/operand shape "
            f"(docs/MWCC_REFERENCE.md §1a SDA globals / §1b float pools)"
        )
        return lines
    if drift.kind not in ("name", "addend"):
        return []
    entry = _lookup(drift, unit_name, reloc_map)
    retail = entry["retail_symbol"] if entry else drift.retail_symbol
    delta = entry["addend_delta"] if entry else drift.addend_delta

    if _is_section_symbol(drift.decomp_symbol):
        return [
            f"decomp emits a section-symbol reloc {drift.decomp_symbol}; retail names it "
            f"{retail}  [{drift.type_name}]",
            f"  fix (approved): give the referenced data a named variable in source so "
            f"MWCC emits the symbol instead of a section reloc (access via the retail "
            f"name), or accept at EQUIVALENT_MATCH — postprocess reloc patching is "
            f"deprecated (PLAN.md §17.6)",
        ]

    lines: list[str] = []
    if drift.kind == "name":
        lines.append(
            f"rename reloc {drift.decomp_symbol} → {retail}"
            f"  [{drift.type_name}]"
        )
        if entry:
            lines.append(f"  (repo map: {entry['count']}× in {len(entry['units'])} units)")
        lines.append(
            f"  fix (approved, PLAN.md §17.6): declare `extern \"C\" <TYPE> {retail};` "
            f"in source and reference it (docs/MWCC_REFERENCE.md §1a SDA globals / "
            f"§1b float pools / §1h string pools)"
        )
        lines.append(
            f"  if the symbol is an implicit MWCC pool/magic constant that cannot be "
            f"named in source: accept at EQUIVALENT_MATCH — binary reloc patching "
            f"(postprocess_reloc_names.py rules / objcopy --redefine-sym) is deprecated "
            f"(PLAN.md §17.6)"
        )
    else:  # addend
        lines.append(
            f"reloc {drift.decomp_symbol} → {retail} with addend delta "
            f"{delta:+d}  [{drift.type_name}]"
        )
        lines.append(
            f"  source symbol sits {delta:+d} bytes from the retail label; "
            f"declare `extern \"C\" <TYPE> {retail};` and access at "
            f"{retail} {f'+ {delta}' if delta and delta > 0 else f'- {-delta}' if delta and delta < 0 else '+ 0'}"
            f" (or adjust the source declaration so the addend is 0)"
        )
        lines.append(
            f"  same acceptance note as pure name drift: no postprocess reloc patching "
            f"(PLAN.md §17.6)"
        )
    return lines


# ── CLI ────────────────────────────────────────────────────────────────────


def _resolve_candidates(functions, symbol: str):
    exact = [f for f in functions if f.name == symbol]
    if exact:
        return exact
    lowered = symbol.lower()
    ci = [f for f in functions if f.name.lower() == lowered]
    if ci:
        return ci
    partial = [f for f in functions if lowered in f.name.lower()]
    return partial


def _resolve_unit(project: Project, hint: str):
    unit = project.resolve_unit(hint)
    if unit.target_path is None or unit.base_path is None:
        raise SystemExit(f"ERROR: unit {hint!r} has no retail/decomp object pair")
    return unit


def cmd_diff(args) -> int:
    from tools.coop.lib.object_size import check_object_size, format_size_check

    config = load_config(Path("coop.json"), _REPO)
    project = Project(config)
    unit = _resolve_unit(project, args.unit)
    retail_path, decomp_path = unit.target_path, unit.base_path

    if not args.no_build:
        import subprocess

        rel_path = str(decomp_path.relative_to(project.root))
        print(f"building {rel_path} ...", file=sys.stderr, flush=True)
        result = subprocess.run(
            [project.ninja_bin(), rel_path],
            cwd=project.root, check=False, capture_output=True, text=True,
            timeout=args.build_timeout,
        )
        if result.returncode != 0:
            print(result.stdout, file=sys.stderr)
            print(result.stderr, file=sys.stderr)
            return 2

    try:
        retail_fn = list_text_functions(retail_path)
        decomp_fn = list_text_functions(decomp_path)
    except Exception as exc:
        print(f"ERROR reading objects: {exc}", file=sys.stderr)
        return 3

    retail_match = _resolve_candidates(retail_fn, args.symbol)
    if not retail_match:
        print(f"ERROR: symbol {args.symbol!r} not found in {retail_path}", file=sys.stderr)
        print(f"  available: {', '.join(f.name for f in retail_fn[:20])}", file=sys.stderr)
        return 4
    decomp_match = _resolve_candidates(decomp_fn, args.symbol)
    if not decomp_match:
        retail_val = retail_match[0].value
        decomp_match = [f for f in decomp_fn if f.value == retail_val]
    if not decomp_match:
        print(f"ERROR: symbol {args.symbol!r} not found in {decomp_path}", file=sys.stderr)
        print(f"  available: {', '.join(f.name for f in decomp_fn[:20])}", file=sys.stderr)
        return 4

    retail, decomp = retail_match[0], decomp_match[0]
    drifts = analyze_function_pair(retail, decomp)
    map_path = Path(args.map) if args.map else DEFAULT_MAP
    reloc_map = load_map(map_path) if map_path.is_file() else {}

    size_check = check_object_size(
        project_root=project.root,
        region=config.region,
        unit_hint=unit.name,
        retail_object=retail_path,
        decomp_object=decomp_path,
    )

    if args.json:
        output = {
            "symbol": retail.name,
            "unit": unit.name,
            "retail_size": retail.size,
            "decomp_size": decomp.size,
            "size_check": {
                "over_by": size_check.over_by if size_check else None,
                "detail": format_size_check(size_check) if size_check else None,
            },
            "drifts": [d.to_dict() for d in drifts],
            "suggestions": {
                f"0x{d.offset:04x}": suggestions(
                    d, unit.name, decomp_path.stem, reloc_map
                )
                for d in drifts
            },
        }
        print(json.dumps(output, indent=2))
    else:
        print(f"function: {retail.name}")
        print(f"  retail:  {retail.path}  (0x{retail.size:x} bytes)")
        print(f"  decomp:  {decomp.path}  (0x{decomp.size:x} bytes)")
        print()
        if not drifts:
            print("no reloc name drift detected")
        for d in drifts:
            delta = f" (addend delta {d.addend_delta:+d})" if d.addend_delta else ""
            print(
                f"  +0x{d.offset:04x} {d.type_name:24s} {d.kind:10s} "
                f"{d.retail_symbol} → {d.decomp_symbol}{delta}"
            )
            for line in suggestions(d, unit.name, decomp_path.stem, reloc_map):
                print(f"      {line}")
        if size_check and size_check.over_by:
            print()
            print(format_size_check(size_check))
    return 0


def cmd_mine(args) -> int:
    config = load_config(Path("coop.json"), _REPO)
    project = Project(config)
    include_kinds = {"data"}
    if args.all_kinds:
        include_kinds = {"data", "call", "branch"}
    data = mine(project, include_kinds=include_kinds, limit=args.limit)

    out = Path(args.out) if args.out else DEFAULT_MAP
    if args.dry_run:
        if args.json:
            print(json.dumps(data, indent=2))
        else:
            print(f"dry-run: would write {out} ({data['count']} entries from {data['units_scanned']} units)")
        return 0

    save_map(data, out)
    print(f"wrote {out} — {data['count']} entries from {data['units_scanned']} units")
    if args.json:
        print(json.dumps(data, indent=2))
    return 0


def cmd_ensure_fresh(args) -> int:
    config = load_config(Path("coop.json"), _REPO)
    project = Project(config)
    out = Path(args.out) if args.out else DEFAULT_MAP
    re_mined = ensure_fresh(project, force=args.force, out=out)
    print("re-mined" if re_mined else "fresh")
    return 0


def cmd_show(args) -> int:
    reloc_map = load_map(args.map or DEFAULT_MAP)
    entries = reloc_map.get("entries", {})
    count = 0
    shown = 0
    for key in sorted(entries):
        scoped = is_unit_scoped_key(key)
        if args.unit_scoped is False and scoped:
            continue
        if args.unit_scoped is True and not scoped:
            continue
        if args.symbol and args.symbol.lower() not in key.lower():
            continue
        for type_name, entry in sorted(entries[key].items()):
            count += 1
            if shown >= (args.limit if args.limit else 10**9):
                continue
            shown += 1
            delta = f" delta {entry['addend_delta']:+d}" if entry.get("addend_delta") else ""
            print(
                f"{key:48s} {type_name:20s} {entry['kind']:6s} "
                f"→ {entry['retail_symbol']}{delta}  "
                f"({entry['count']}×)"
            )
            if args.verbose:
                print(f"    units: {', '.join(entry['units'][:5])}")
                for alt in entry.get("alternatives", [])[:3]:
                    print(f"    alt:   {alt['retail_symbol']} ({alt['count']}×)")
    print(f"\n{count} entries total (showing {shown})")
    return 0


def _parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Reloc name-drift detection + named-symbol map mining (§17.6, MWCC_REFERENCE §1)."
    )
    sub = parser.add_subparsers(dest="command", required=True)

    p_diff = sub.add_parser("diff", help="per-function reloc drift analysis")
    p_diff.add_argument("unit", help="objdiff unit hint (e.g. kyoshin/COccCulling) or source path")
    p_diff.add_argument("-s", "--symbol", required=True, help="function symbol (mangled name or unique substring)")
    p_diff.add_argument("--no-build", action="store_true", help="do not build the decomp object first")
    p_diff.add_argument("--build-timeout", type=int, default=600)
    p_diff.add_argument("--json", action="store_true")
    p_diff.add_argument("--map", default=None, help="path to mined reloc map (default: tools/coop/retail_reloc_map.json)")
    p_diff.set_defaults(func=cmd_diff)

    p_mine = sub.add_parser("mine", help="batch-mine the named-symbol reloc map across all objdiff units")
    p_mine.add_argument("--out", default=None, help="output map path (default: tools/coop/retail_reloc_map.json)")
    p_mine.add_argument("--all-kinds", action="store_true", help="include call/branch reloc kinds (default: data only)")
    p_mine.add_argument("--limit", type=int, default=None, help="analyze at most N units (testing)")
    p_mine.add_argument("--dry-run", action="store_true", help="analyze and print stats without writing")
    p_mine.add_argument("--json", action="store_true")
    p_mine.set_defaults(func=cmd_mine)

    p_fresh = sub.add_parser("ensure-fresh", help="re-mine the reloc map iff stale (doc 33 Item 0.5)")
    p_fresh.add_argument("--out", default=None, help="map path (default: tools/coop/retail_reloc_map.json)")
    p_fresh.add_argument("--force", action="store_true", help="re-mine unconditionally")
    p_fresh.set_defaults(func=cmd_ensure_fresh)

    p_show = sub.add_parser("show", help="pretty-print the mined reloc map")
    p_show.add_argument("--map", default=None, help="map path (default: tools/coop/retail_reloc_map.json)")
    p_show.add_argument("--symbol", default=None, help="filter by symbol substring")
    p_show.add_argument("--limit", type=int, default=None)
    p_show.add_argument("--verbose", action="store_true")
    p_show.add_argument("--unit-scoped", action="store_true", default=None, help="only TU-local (unit-scoped) entries")
    p_show.add_argument("--global-only", dest="unit_scoped", action="store_false", help="only named (global) entries")
    p_show.set_defaults(func=cmd_show)

    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = _parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())

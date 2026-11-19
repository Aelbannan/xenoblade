#!/usr/bin/env python3
"""Supervised BULK member-ization driver.

Takes the ``extc scan --json`` candidate inventory, discovers the owning
class per candidate via the planner's refusal message, proposes a
conservative method name, classifies with ``demote_import_band=True``
(falling back to ``+no_header_insert``), and -- in ``apply`` mode --
applies the CLEAN set one UNIT at a time with a per-function
byte-identity gate (address/size/instruction_match) enforced mechanically.

Naming policy (conservative, documented here and in ``propose_method_name``):
  0. Refuse primitive/typedef-only self types outright (``char``, ``int``,
     ``u8``/``s32``/``f32``/..., ``void``, ``bool``, ``float``, ``double``,
     ``short``, ``long``, ``unsigned``, ``signed`` and multi-word combos like
     ``unsigned int``).  A ``char* self`` is not a class; such candidates are
     BLOCKED, never CLEAN.
  1. Strip ONE trailing ``_<HEX4>`` suffix
     ``_791C``) -- retail disambiguators, never part of the source name.
  2. Iteratively strip a leading class-derived prefix and a following ``_``:
     - the full class name (case-insensitive, e.g. ``SLList_``),
     - the class name without a leading ``C`` (``CColiObject`` -> ``ColiObject_``),
     - any CamelCase token of the class (``SLList`` -> ``SL``/``List``;
       ``CColiObject`` -> ``C``/``Coli``/``Object``),
     - for CamelCase (no-underscore) names, strip only when the remainder
       starts with an uppercase letter (word boundary), so ``ColiBuild...``
       -> ``Build...`` but ``Collision`` is untouched.
  3. Strip known noise prefixes (intentionally tiny: ``sched`` only) the
     same way -- e.g. ``SchedList_DropItem`` -> ``List_DropItem`` -> (then
     class token ``List``) -> ``DropItem``.
  4. Refuse (return None) when the result is empty, starts with a digit, or
     is not a plain C identifier (``[A-Za-z_][A-Za-z0-9_]*``).

  The policy never guesses abbreviations (``Widget_`` vs ``Wdg`` stays
  ``Widget_Paint``) and never empties the name.  Whatever survives is a
  valid identifier; the planner still verifies the full signature and
  refuses on any mismatch.

Read-only guarantee: ``plan`` never writes except to explicit ``--json`` /
``--report`` paths.  ``apply`` defaults to dry-run (no writes) unless
``--execute`` is passed.

Single-unit retry: ``apply --plan-json <plan> --unit <unit> --execute``
applies just that unit's CLEAN rows (re-plan, snapshot, per-function apply,
address-keyed gate, revert on delta) and, on PASS, runs the sanctioned
post-pass (``targets sync-symbols`` + ``.ctx.c`` regen for that unit's
files) before writing the report.  Other units in the plan are untouched.
"""

from __future__ import annotations

import argparse
import datetime
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Callable, Optional

ROOT = Path(__file__).resolve().parent.parent.parent
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.atomic_write import atomic_write_bytes  # noqa: E402
from tools.coop.symbol_fn_rename import (  # noqa: E402
    REGIONS,
    FnPlan,
    FnRenameRefusal,
    apply_function_rename,
    plan_function_rename,
)

PROBE_CLASS = "BulkProbeZZ"
BACKUP_ROOT = Path("/tmp/memberize_bulk_backup")
# Region control: the US pipeline is what matters and jp is currently
# corrupt, so bulk apply defaults to us only.  A rename must never rewrite
# a region the caller did not ask for.
DEFAULT_REGIONS: tuple[str, ...] = ("us",)


def parse_region_list(value: str | list[str] | tuple[str, ...] | None) -> list[str]:
    """Parse a ``--region`` value (comma list) into a validated region list.

    ``None``/empty means the default (us only).  Raises :class:`ValueError`
    on unknown region names.
    """
    if value is None:
        return list(DEFAULT_REGIONS)
    if isinstance(value, str):
        items = [v.strip() for v in value.split(",")]
    else:
        items = [str(v).strip() for v in value]
    items = [v for v in items if v]
    if not items:
        return list(DEFAULT_REGIONS)
    for r in items:
        if r not in REGIONS:
            raise ValueError(
                f"unknown region {r!r} (expected one of {', '.join(REGIONS)})"
            )
    seen: list[str] = []
    for r in items:
        if r not in seen:
            seen.append(r)
    return seen
NOISE_PREFIXES = ["sched"]
_IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*$")
_TRAILING_HEX_RE = re.compile(r"_([0-9A-Fa-f]{4})$")
_SELF_MISMATCH_RE = re.compile(
    r"definition self type '([^']+)' != target class '([^']+)'"
)
LEADING_DIGIT_RE = re.compile(r"^[0-9]")

# Primitive/typedef-only self types: a ``char* self`` (or ``u16``/``int``/
# ``unsigned int``/...) is not a class, so no ``Class::method`` proposal is
# sound.  Matched token-wise (after stripping pointers/references and
# cv-qualifiers) so multi-word spellings like ``unsigned int`` are caught.
PRIMITIVE_SELF_TYPES = frozenset({
    "char", "int", "u8", "u16", "u32", "u64",
    "s8", "s16", "s32", "s64", "f32", "f64",
    "void", "bool", "float", "double",
    "short", "long", "unsigned", "signed",
})
_CV_QUALIFIERS = frozenset({"const", "volatile"})


def _is_primitive_self_type(cls_name: str | None) -> bool:
    """True when *cls_name* is a primitive/typedef-only token, not a class."""
    if not cls_name:
        return False
    leaf = str(cls_name).split("::")[-1].split("<")[0]
    cleaned = leaf.replace("*", " ").replace("&", " ")
    toks = [t.strip().lower() for t in cleaned.split() if t.strip()]
    toks = [t for t in toks if t not in _CV_QUALIFIERS]
    if not toks:
        return False
    return all(t in PRIMITIVE_SELF_TYPES for t in toks)


# ---------------------------------------------------------------------------
# Scan inventory
# ---------------------------------------------------------------------------


def load_scan_candidates(scan_path: Path) -> list[dict]:
    """Load extc scan JSON into [{file,line,name,kind,self_style}]."""
    data = json.loads(scan_path.read_text(encoding="utf-8"))
    out: list[dict] = []
    for rec in data.get("member_candidates", []):
        # extc appends bool(self_style) to [rel, lineno, name, kind].
        if len(rec) < 5:
            continue
        rel, lineno, name, kind, self_style = rec[0], rec[1], rec[2], rec[3], rec[4]
        out.append(
            {"file": str(rel), "line": int(lineno), "name": str(name),
             "kind": str(kind), "self_style": bool(self_style)}
        )
    return out


def filter_true_memberizable(candidates: list[dict]) -> list[dict]:
    """TRUE member-izable set: self_style=True AND '__' not in name."""
    return [c for c in candidates if c.get("self_style") and "__" not in c.get("name", "")]


def dedupe_by_old(candidates: list[dict]) -> list[dict]:
    """Deduplicate by old symbol name, keeping the first occurrence."""
    seen: dict[str, dict] = {}
    for c in candidates:
        if c["name"] not in seen:
            seen[c["name"]] = c
    return list(seen.values())


# ---------------------------------------------------------------------------
# Class discovery via the refusal-message trick
# ---------------------------------------------------------------------------


def discover_class(
    project_root: Path, old: str, regions: list[str] | None = None
) -> str:
    """Discover the owning class via the planner's self-type refusal.

    Calls ``plan_function_rename`` with a dummy target
    ``<PROBE_CLASS>::<old>`` and parses
    ``definition self type '<Class>' != target class '<target>'``.
    Raises :class:`FnRenameRefusal` when the class cannot be discovered
    (void* self, missing definition, unknown symbol, ...).
    """
    probe_spec = f"{PROBE_CLASS}::{old}"
    try:
        plan_function_rename(project_root, old, probe_spec, regions=regions)
    except FnRenameRefusal as exc:
        msg = str(exc)
        m = _SELF_MISMATCH_RE.search(msg)
        if m:
            return m.group(1)
        raise
    # No refusal: void* self (class comes from the spec only) or another
    # shape where the probe accidentally succeeded -- cannot discover.
    raise FnRenameRefusal(
        f"refusing: cannot discover owning class for {old!r} "
        f"(probe {probe_spec!r} did not hit the self-type guard; "
        f"likely void* self -- needs manual class)"
    )


# ---------------------------------------------------------------------------
# Naming policy
# ---------------------------------------------------------------------------


def _camel_tokens(class_name: str) -> list[str]:
    """Split a class name into CamelCase tokens (SLList -> [SL, List])."""
    # Strip any namespace qualifier first (discovery returns the leaf, but
    # be defensive).
    leaf = class_name.split("::")[-1].split("<")[0]
    toks = re.findall(r"[A-Z]+(?![a-z])|[A-Z][a-z0-9]*|[a-z0-9]+", leaf)
    return [t for t in toks if t]


def _class_prefixes(class_name: str) -> list[str]:
    """Candidate class-derived prefixes, longest first."""
    leaf = class_name.split("::")[-1].split("<")[0]
    cands: set[str] = {leaf}
    if len(leaf) > 1 and leaf[0] == "C" and leaf[1].isupper():
        cands.add(leaf[1:])  # CColiObject -> ColiObject
    for tok in _camel_tokens(leaf):
        cands.add(tok)
    # Longest first so 'ColiObject' beats 'Coli'.
    return sorted(cands, key=len, reverse=True)


def _try_strip_once(base: str, prefix: str) -> Optional[str]:
    """Strip one leading *prefix* (case-insensitive) + optional '_'.

    Returns the stripped remainder, or None when the prefix does not apply.
    CamelCase (no-underscore) strips require the remainder to start with an
    uppercase letter (word boundary); underscore strips always apply.
    """
    if not prefix or len(base) <= len(prefix):
        return None
    if base[: len(prefix)].lower() != prefix.lower():
        return None
    rest = base[len(prefix):]
    if rest.startswith("_"):
        rest = rest[1:]
        return rest if rest else None
    # No underscore: only a CamelCase word boundary (e.g. Coli+Build).
    if rest and rest[0].isupper():
        return rest
    return None


def propose_method_name(old: str, class_name: str) -> Optional[str]:
    """Propose a member name for *old* in *class_name* (None = refuse).

    Implements the module-docstring policy.  Refuses when the discovered
    class is a primitive/typedef-only token, or when the result is empty,
    starts with a digit, or is not a plain C identifier.
    """
    if _is_primitive_self_type(class_name):
        return None
    base = old
    # 1. trailing _HEX4 (one suffix only).
    m = _TRAILING_HEX_RE.search(base)
    if m:
        stripped = base[: -len(m.group(0))]
        if stripped:
            base = stripped
    # 2+3. iterative class-derived + noise stripping (longest first).
    prefixes = _class_prefixes(class_name) + list(NOISE_PREFIXES)
    # Deduplicate longest-first while keeping order.
    seen: set[str] = set()
    ordered: list[str] = []
    for p in sorted(prefixes, key=len, reverse=True):
        if p.lower() not in seen:
            seen.add(p.lower())
            ordered.append(p)
    for _ in range(6):  # bounded: e.g. Sched->List->DropItem needs 2 rounds
        progressed = False
        for pre in ordered:
            nxt = _try_strip_once(base, pre)
            if nxt is not None and nxt != base:
                base = nxt
                progressed = True
                break
        if not progressed:
            break
    # A stray leading '_' left over (e.g. '_DropItem') is trimmed once.
    if base.startswith("_") and len(base) > 1:
        base = base[1:]
    if not base:
        return None
    if LEADING_DIGIT_RE.match(base):
        return None
    if not _IDENT_RE.match(base):
        return None
    return base


# ---------------------------------------------------------------------------
# Classification
# ---------------------------------------------------------------------------


@dataclass
class Classified:
    old: str
    file: str
    line: int
    cls: Optional[str]
    method: Optional[str]
    mangled: Optional[str]
    classification: str  # CLEAN | NEEDS-REVIEW | blocked:<reason>
    unit: str
    no_header_insert: bool = False
    detail: Optional[dict] = None


from functools import lru_cache as _lru_cache

@_lru_cache(maxsize=8)
def _unit_cache_cached(root_str: str) -> tuple[tuple[str, str, str], ...]:
    """Cached (unit_name, source_path, stem) rows from objdiff.json."""
    project_root = Path(root_str)
    path = project_root / "objdiff.json"
    if not path.is_file():
        return ()
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return ()
    rows: list[tuple[str, str, str]] = []
    for u in data.get("units", []):
        name = str(u.get("name", ""))
        sp = str(((u.get("metadata") or {}).get("source_path") or ""))
        stem = Path(sp).stem if sp else Path(name).stem
        rows.append((name, sp, stem))
    return tuple(rows)


def _unit_cache(project_root: Path) -> list[tuple[str, str, str]]:
    """Load (unit_name, source_path_str, stem) rows from objdiff.json."""
    return list(_unit_cache_cached(str(project_root)))


def unit_for_file(project_root: Path, relpath: str) -> str:
    """Resolve the owning objdiff unit for a repo-relative source path.

    Falls back to the parent-directory name when objdiff.json is absent
    (test fixtures), else ``unknown``.
    """
    rows = _unit_cache(project_root)
    if rows:
        norm = relpath.replace("\\", "/")
        for name, sp, _stem in rows:
            if sp and (sp == norm or sp.endswith("/" + norm) or norm.endswith(sp)):
                return name
        stem = Path(norm).stem
        matches = [n for n, _sp, st in rows if st == stem]
        if len(matches) == 1:
            return matches[0]
        return "unknown"
    # No objdiff.json (unit tests): group by parent directory.
    parent = str(Path(relpath).parent)
    return parent if parent and parent != "." else "unknown"


def classify_candidate(
    project_root: Path, old: str, cls: str, method: str,
    regions: list[str] | None = None,
) -> tuple[FnPlan, bool]:
    """Classify ``Class::method`` with demote_import_band, falling back.

    Returns (plan, used_no_header_insert).  Raises FnRenameRefusal when
    both attempts error.  *regions* restricts the symbols.txt scope; None
    means all regions (backward compatible).
    """
    spec = f"{cls}::{method}"
    try:
        plan = plan_function_rename(
            project_root, old, spec, regions=regions, demote_import_band=True
        )
        return plan, False
    except FnRenameRefusal as first:
        try:
            plan = plan_function_rename(
                project_root, old, spec, regions=regions,
                demote_import_band=True, no_header_insert=True,
            )
            return plan, True
        except FnRenameRefusal:
            raise first


def plan_all(
    project_root: Path, candidates: list[dict],
    regions: list[str] | None = None,
) -> dict:
    """Plan every deduplicated TRUE member-izable candidate (read-only)."""
    uniq = dedupe_by_old(filter_true_memberizable(candidates))
    out: list[dict] = []
    total = len(uniq)
    for idx, cand in enumerate(uniq, start=1):
        if idx == 1 or idx % 50 == 0 or idx == total:
            print(f"memberize-bulk plan: {idx}/{total}", file=sys.stderr, flush=True)
        old = cand["name"]
        relfile = cand["file"]
        line = cand["line"]
        # 1. discover class.
        try:
            cls = discover_class(project_root, old, regions=regions)
        except FnRenameRefusal as exc:
            out.append({
                "old": old, "file": relfile, "line": line,
                "class": None, "proposed_method": None, "mangled": None,
                "classification": f"blocked:{exc}",
                "unit": unit_for_file(project_root, relfile),
                "no_header_insert": False,
            })
            continue
        # 2. refuse primitive self types (char* self is not a class).
        if _is_primitive_self_type(cls):
            out.append({
                "old": old, "file": relfile, "line": line,
                "class": cls, "proposed_method": None, "mangled": None,
                "classification": (
                    f"blocked:self type {cls!r} is a primitive/typedef-only "
                    f"token, not a class (e.g. char* self); needs manual review"
                ),
                "unit": unit_for_file(project_root, relfile),
                "no_header_insert": False,
            })
            continue
        # 3. propose method name.
        method = propose_method_name(old, cls)
        if method is None:
            out.append({
                "old": old, "file": relfile, "line": line,
                "class": cls, "proposed_method": None, "mangled": None,
                "classification": (
                    f"blocked:naming policy refused old={old!r} class={cls!r}"
                ),
                "unit": unit_for_file(project_root, relfile),
                "no_header_insert": False,
            })
            continue
        # 4. classify.
        try:
            plan, used_nhi = classify_candidate(
                project_root, old, cls, method, regions=regions
            )
        except FnRenameRefusal as exc:
            out.append({
                "old": old, "file": relfile, "line": line,
                "class": cls, "proposed_method": method, "mangled": None,
                "classification": f"blocked:{exc}",
                "unit": unit_for_file(project_root, relfile),
                "no_header_insert": False,
            })
            continue
        if plan.reviews or plan.addr_refs or plan.configure_hits or plan.splits_hits:
            classification = "NEEDS-REVIEW"
        else:
            classification = "CLEAN"
        # Owning unit: prefer the definition file's unit.
        if plan.defns:
            try:
                rel_defn = str(plan.defns[0].path.relative_to(project_root))
            except ValueError:
                rel_defn = str(plan.defns[0].path)
            unit = unit_for_file(project_root, rel_defn)
        else:
            unit = unit_for_file(project_root, relfile)
        out.append({
            "old": old, "file": relfile, "line": line,
            "class": cls, "proposed_method": method,
            "mangled": plan.new_mangled,
            "classification": classification,
            "unit": unit,
            "no_header_insert": used_nhi,
            "detail": {
                "reviews": len(plan.reviews),
                "addr_refs": len(plan.addr_refs),
                "configure_hits": len(plan.configure_hits),
                "splits_hits": len(plan.splits_hits),
                "demotions": len(plan.demotions),
                "header_edits": len(plan.header_edits),
            },
        })
    by_unit: dict[str, int] = {}
    n_clean = n_review = n_blocked = 0
    for row in out:
        if row["classification"] == "CLEAN":
            n_clean += 1
            by_unit[row["unit"]] = by_unit.get(row["unit"], 0) + 1
        elif row["classification"] == "NEEDS-REVIEW":
            n_review += 1
        else:
            n_blocked += 1
    return {
        "candidates": out,
        "summary": {
            "total": len(out),
            "clean": n_clean,
            "needs_review": n_review,
            "blocked": n_blocked,
            "by_unit": by_unit,
        },
    }


def render_table(plan_data: dict) -> str:
    """Human-readable per-candidate table."""
    lines = ["old | file | class | method | mangled | classification | unit"]
    for row in plan_data.get("candidates", []):
        lines.append(
            f"{row['old']} | {row['file']}:{row.get('line','')} | "
            f"{row.get('class') or '-'} | {row.get('proposed_method') or '-'} | "
            f"{row.get('mangled') or '-'} | {row['classification']} | {row['unit']}"
        )
    s = plan_data.get("summary", {})
    lines.append(
        f"summary: total={s.get('total',0)} clean={s.get('clean',0)} "
        f"needs_review={s.get('needs_review',0)} blocked={s.get('blocked',0)}"
    )
    for unit, n in sorted((s.get("by_unit") or {}).items()):
        lines.append(f"  clean[{unit}] = {n}")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Apply side: snapshots, backup/restore, per-unit gate
# ---------------------------------------------------------------------------


def snapshot_from_hexdiff(
    project_root: Path, unit: str, aliases: dict[str, str] | None = None
) -> dict:
    """Capture a per-function baseline via ``hexdiff <unit> --all``.

    Returns {address: {symbol, decomp_symbol, present, retail_size,
    decomp_size, mismatch}} where *address* is the retail ``--list``
    address (stable across renames), *symbol* is the retail symbol, and
    *decomp_symbol* is the paired decomp symbol (``None`` when hexdiff
    could not pair the function).  Unpaired rows keep ``None`` sizes --
    never zero-filled -- so a rename that hexdiff cannot pair reads as
    "unpaired", not as a bogus ``size -> 0`` delta.

    *aliases* ({old: new_mangled}) is forwarded as ``--pair-alias`` so a
    planned rename pairs deterministically even when the new mangled
    name no longer contains the old one.
    """
    hexdiff = str(ROOT / "tools" / "coop" / "hexdiff.py")
    py = sys.executable
    # Tool-internal byte-identity snapshot: baseline LOST/GAIN is a separate
    # regression gate, so do not let the default baseline change exit codes.
    argv = [py, hexdiff, unit, "--all", "--json", "--no-baseline"]
    for old, new in (aliases or {}).items():
        argv += ["--pair-alias", f"{old}={new}"]
    all_proc = subprocess.run(
        argv,
        cwd=str(project_root), capture_output=True, text=True, check=False,
    )
    if all_proc.returncode not in (0, 5):
        raise RuntimeError(
            f"hexdiff --all failed for {unit}: {all_proc.stderr[:500]}"
        )
    try:
        payload = json.loads(all_proc.stdout or "{}")
    except ValueError as exc:
        raise RuntimeError(f"hexdiff --all JSON unparsable for {unit}: {exc}") from exc
    list_proc = subprocess.run(
        [py, hexdiff, unit, "--list"],
        cwd=str(project_root), capture_output=True, text=True, check=False,
    )
    addr_by_symbol: dict[str, int] = {}
    if list_proc.returncode == 0:
        for line in (list_proc.stdout or "").splitlines():
            m = re.match(r"\s*(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+(\S+)", line)
            if m:
                try:
                    addr_by_symbol[m.group(3)] = int(m.group(1), 16)
                except ValueError:
                    pass
    snap: dict[int, dict] = {}
    for fn in payload.get("functions", []):
        sym = str(fn.get("symbol", ""))
        addr = addr_by_symbol.get(sym)
        if addr is None:
            # Fall back to hashing the symbol when --list is unavailable;
            # stable for unchanged names (renames handled in compare).
            addr = hash(sym) & 0xFFFFFFFF
        present = bool(fn.get("present", True))
        snap[addr] = {
            "symbol": sym,
            "decomp_symbol": fn.get("decomp_symbol"),
            "present": present,
            "retail_size": (int(fn["retail_size"]) if "retail_size" in fn else None),
            "decomp_size": (int(fn["decomp_size"]) if "decomp_size" in fn else None),
            "mismatch": (int(fn["mismatch"]) if "mismatch" in fn else None),
        }
    return snap


def match_percent(rec: dict) -> float | None:
    """Instruction-match % from a snapshot record (mismatch slots).

    Returns ``None`` when the record is unpaired (sizes unknown).
    """
    r, d, m = rec.get("retail_size"), rec.get("decomp_size"), rec.get("mismatch")
    if r is None or d is None or m is None:
        return None
    total = max(int(r), int(d)) // 4
    if total <= 0:
        return 100.0
    return 100.0 * (total - int(m)) / total


def _ident(rec: dict) -> Optional[str]:
    """Stable function identity: the decomp symbol when known, else retail."""
    d = rec.get("decomp_symbol")
    if isinstance(d, str) and d:
        return d
    return rec.get("symbol")


def _sizes_equal(b: dict, a: dict) -> bool:
    return all(b.get(k) == a.get(k) for k in ("retail_size", "decomp_size", "mismatch"))


# ---------------------------------------------------------------------------
# LIVE rename plumbing (second-wave fix): address-keyed reconciliation +
# nm-verified NEW.
#
# The plan's rename key is the SOURCE-level old name (e.g.
# ``EvtSeqProbeEvtSlot``), but the snapshot / ``hexdiff --list`` symbol is
# the RETAIL ``symbols.txt`` spelling, which may ALREADY be a mangled name
# (``EvtSeqProbeEvtSlot__FP12EvtBdabModelPUl``).  Looking up
# ``renames[old]`` then misses (``planned: ... -> ?``).  Worse, the plan's
# proposed mangling (``...FUi`` for a ``u32`` param) may not match what MWCC
# actually emits (``...FUl`` -- ``u32`` is ``unsigned long`` under MWCC),
# so ``--pair-alias OLD=NEW`` silently fails to pair and the gate reports a
# false ``unpaired post-apply``.
#
# Fix: join plan -> pre-snapshot BY ADDRESS (not by raw source name), key
# the hexdiff alias by the snapshot's RETAIL symbol at that address (the
# spelling hexdiff's ``rf.name`` lookup expects), and verify NEW against
# ``powerpc-eabi-nm`` on the freshly built ``.o`` instead of trusting the
# plan's proposed mangling.
# ---------------------------------------------------------------------------


def _strip_mangle(sym: str | None) -> str | None:
    """Base name before any ``__...`` MWCC mangling suffix."""
    if not isinstance(sym, str) or not sym:
        return None
    return sym.split("__", 1)[0] or None


def _strip_hex4(sym: str | None) -> str | None:
    """Strip one trailing ``_<HEX4>`` retail disambiguator, if present."""
    if not isinstance(sym, str) or not sym:
        return None
    m = _TRAILING_HEX_RE.search(sym)
    if m and len(sym) > len(m.group(0)):
        return sym[: -len(m.group(0))]
    return sym


def _name_variants(name: str | None) -> set[str]:
    """Match variants for a source-level old name.

    ``EvtSeqProbeEvtSlot`` must match the retail ``EvtSeqProbeEvtSlot``,
    the decomp ``EvtSeqProbeEvtSlot__FP12EvtBdabModelPUl``, and any
    ``_<HEX4>``-suffixed retail spelling -- all of which share the same
    base once mangling/hex suffixes are stripped.
    """
    if not isinstance(name, str) or not name:
        return set()
    out: set[str] = {name}
    base = _strip_mangle(name)
    if base:
        out.add(base)
    nohex = _strip_hex4(name)
    if nohex:
        out.add(nohex)
        b2 = _strip_mangle(nohex)
        if b2:
            out.add(b2)
    return out


def find_before_addr(before: dict, old: str) -> int | None:
    """Find the snapshot address for plan-old *old* (address-keyed join).

    Matches when ANY of the record's retail ``symbol`` / ``decomp_symbol``
    (or their mangling/hex-stripped bases) equals ANY variant of *old*.
    Prefers exact retail matches, then exact decomp matches, then base
    matches.  Returns ``None`` when *old* is absent from the snapshot.
    """
    if not old:
        return None
    want = _name_variants(old)
    # 1. exact retail symbol.
    for addr, rec in before.items():
        if rec.get("symbol") in want and rec.get("symbol") == old:
            return addr
    # 2. exact decomp symbol.
    for addr, rec in before.items():
        if rec.get("decomp_symbol") == old:
            return addr
    # 3. retail symbol in variants (mangled retail whose base is old).
    for addr, rec in before.items():
        sym = rec.get("symbol")
        if isinstance(sym, str) and sym in want:
            return addr
        if _strip_mangle(sym) in want or _strip_hex4(sym) in want:
            # Only accept base matches when the base equals old's base,
            # to avoid cross-matching unrelated ``__`` names.
            if _strip_mangle(sym) == _strip_mangle(old):
                return addr
    # 4. decomp symbol variants (mangled old, e.g.
    #    ``EvtSeqProbeEvtSlot__FP...`` for source ``EvtSeqProbeEvtSlot``).
    for addr, rec in before.items():
        dsym = rec.get("decomp_symbol")
        if isinstance(dsym, str) and dsym in want:
            return addr
        if _strip_mangle(dsym) in want:
            return addr
    return None


def build_address_keyed_maps(
    before: dict, rows: list[dict]
) -> tuple[dict[str, str], dict[str, str], dict[int, tuple[str, str]], list[str]]:
    """Join plan rows -> pre-snapshot BY ADDRESS.

    Returns ``(aliases, gate_renames, addr_map, unresolved)`` where:
      - ``aliases`` maps BEFORE-RETAIL symbol -> planned NEW (the key
        spelling ``hexdiff --all`` ``rf.name`` lookup expects);
      - ``gate_renames`` maps BEFORE-IDENT (``_ident``, decomp-preferred)
        -> planned NEW (the key ``compare_snapshots`` looks up);
      - ``addr_map`` maps address -> (old, planned NEW);
      - ``unresolved`` lists plan-olds with no address in ``before``.
    """
    aliases: dict[str, str] = {}
    gate: dict[str, str] = {}
    addr_map: dict[int, tuple[str, str]] = {}
    unresolved: list[str] = []
    for row in rows:
        old = row.get("old")
        new = row.get("mangled")
        if not old or not new:
            continue
        addr = find_before_addr(before, old)
        if addr is None:
            unresolved.append(old)
            continue
        rec = before[addr]
        retail_sym = rec.get("symbol")
        ident = _ident(rec)
        if isinstance(retail_sym, str) and retail_sym:
            aliases[retail_sym] = new
        if isinstance(ident, str) and ident:
            gate[ident] = new
        addr_map[addr] = (old, new)
    return aliases, gate, addr_map, unresolved


def decomp_object_for_unit(project_root: Path, unit: str) -> Path | None:
    """Locate the built decomp ``.o`` for *unit* via objdiff.json."""
    try:
        for cand in (project_root / "objdiff.json",):
            if not cand.is_file():
                continue
            data = json.loads(cand.read_text(encoding="utf-8"))
            for u in data.get("units", []):
                if str(u.get("name", "")) == unit:
                    bp = u.get("base_path")
                    if bp:
                        p = Path(bp)
                        if not p.is_absolute():
                            p = project_root / p
                        return p
    except (OSError, ValueError):
        pass
    return None


def nm_symbols(decomp_object: Path, project_root: Path) -> set[str]:
    """List symbol names in *decomp_object* via powerpc-eabi-nm."""
    nm = project_root / "build" / "binutils" / "powerpc-eabi-nm"
    if not nm.is_file():
        nm = Path(shutil.which("powerpc-eabi-nm") or "nm")
    try:
        proc = subprocess.run(
            [str(nm), str(decomp_object)],
            capture_output=True, text=True, check=False, timeout=60,
        )
    except (OSError, ValueError):
        return set()
    if proc.returncode != 0:
        return set()
    out: set[str] = set()
    for line in (proc.stdout or "").splitlines():
        parts = line.split()
        if len(parts) >= 3:
            out.add(parts[-1])
        elif len(parts) == 2 and len(parts[-1]) > 1:
            # `nm -g` short form; still take the name column.
            out.add(parts[-1])
    return out


def correct_new_via_nm(
    project_root: Path,
    unit: str,
    method: str,
    class_name: str,
    planned_new: str,
) -> tuple[str, bool, str]:
    """Verify planned NEW against nm on the freshly built ``.o``.

    Returns ``(actual_new, corrected, note)``.  When the planned symbol is
    present, it is returned unchanged.  Otherwise the ``.o`` is searched
    for the unique ``{method}__{classenc}F``-prefixed symbol (same method +
    class, any param suffix -- e.g. MWCC's real ``...FUl`` vs the plan's
    ``...FUi`` for a ``u32``/``unsigned long`` param); a unique hit
    replaces the plan.  Ambiguous/absent stays planned so the gate still
    FAILs loudly instead of silently pairing the wrong overload.
    """
    decomp_o = decomp_object_for_unit(project_root, unit)
    if decomp_o is None or not decomp_o.is_file():
        return planned_new, False, "nm unavailable (no decomp .o)"
    syms = nm_symbols(decomp_o, project_root)
    if not syms:
        return planned_new, False, "nm unavailable (empty)"
    if planned_new in syms:
        return planned_new, False, "planned NEW present in .o"
    # Prefix: method + '__' + class encoding + 'F' (param list follows).
    try:
        from tools.coop.symbol_fn_rename import encode_class as _enc_cls

        class_enc = _enc_cls(class_name)
    except Exception:  # noqa: BLE001
        return planned_new, False, "cannot encode class for nm search"
    prefix = f"{method}__{class_enc}F"
    hits = sorted(s for s in syms if s.startswith(prefix))
    # Destructor/constructor spellings (__dt__/__ct__) never match the
    # method-prefix search; fall back to a method-substring search only
    # when the strict prefix finds nothing (avoids false overload hits).
    if not hits and method and not method.startswith("~"):
        loose = sorted(
            s for s in syms
            if method in s and class_enc in s and "__" in s
        )
        if len(loose) == 1:
            return loose[0], True, f"nm loose match {planned_new} -> {loose[0]}"
        return planned_new, False, f"nm: no unique {prefix}* match"
    if len(hits) == 1:
        return hits[0], True, f"nm corrected {planned_new} -> {hits[0]}"
    if not hits:
        return planned_new, False, f"nm: no {prefix}* match"
    return planned_new, False, f"nm: ambiguous {prefix}* ({len(hits)} hits)"


def compare_snapshots(
    before: dict, after: dict, renames: dict[str, str]
) -> tuple[bool, list[str]]:
    """Compare per-function snapshots keyed by ADDRESS (stable identity).

    For each address present in either snapshot:
      - same identity in both -> (size, mismatch, match%) must be identical;
      - identity changed old -> new -> allowed ONLY when ``new`` is the
        planned mangled name for ``old`` (``renames[old] == new``), and the
        sizes/mismatch/match% must STILL be identical (a real codegen delta
        under a rename still FAILS);
      - an old address absent post-apply is satisfied (not vanished) only
        when its planned new mangled symbol exists post-apply with
        identical sizes; any other extra/missing address FAILS.

    Every planned rename must be observed post-apply; an unobserved rename
    (rename silently did not take effect) FAILS.  Returns (ok, diffs).
    """
    diffs: list[str] = []
    rev = {v: k for k, v in renames.items()}
    observed: set[tuple[str, str]] = set()

    def _pct_str(rec: dict) -> str:
        p = match_percent(rec)
        return f"{p:.1f}" if p is not None else "n/a"

    for addr in sorted(set(before.keys()) | set(after.keys())):
        b, a = before.get(addr), after.get(addr)
        if b is not None and a is not None:
            ob, nb = _ident(b), _ident(a)
            bname = b.get("symbol")
            if ob == nb:
                if not _sizes_equal(b, a):
                    for key in ("retail_size", "decomp_size", "mismatch"):
                        if b.get(key) != a.get(key):
                            diffs.append(
                                f"{hex(addr)} {bname}: {key} "
                                f"{b.get(key)} -> {a.get(key)}"
                            )
                pb, pa = match_percent(b), match_percent(a)
                if pb is not None and pa is not None and abs(pb - pa) > 1e-9:
                    diffs.append(
                        f"{hex(addr)} {bname}: match% {_pct_str(b)} -> {_pct_str(a)}"
                    )
                continue
            # Identity changed: allowed ONLY as the planned rename pair,
            # with byte-identical sizes/mismatch/match%.
            if ob is not None and renames.get(ob) == nb:
                observed.add((ob, nb))  # type: ignore[arg-type]
                if not b.get("present", True) or not a.get("present", True):
                    diffs.append(
                        f"{hex(addr)} {bname}: rename {ob} -> {nb} unpaired "
                        f"post-apply; cannot verify byte-identity"
                    )
                    continue
                if not _sizes_equal(b, a):
                    for key in ("retail_size", "decomp_size", "mismatch"):
                        if b.get(key) != a.get(key):
                            diffs.append(
                                f"{hex(addr)} {bname}: {key} "
                                f"{b.get(key)} -> {a.get(key)}"
                            )
                pb, pa = match_percent(b), match_percent(a)
                if pb is not None and pa is not None and abs(pb - pa) > 1e-9:
                    diffs.append(
                        f"{hex(addr)} {bname}: match% {_pct_str(b)} -> {_pct_str(a)}"
                    )
                continue
            if (nb in rev) or (ob in renames):
                diffs.append(
                    f"{hex(addr)}: unexpected rename {ob} -> {nb} "
                    f"(planned: {ob} -> {renames.get(ob) if ob in renames else '?'})"
                )
            else:
                diffs.append(f"{hex(addr)}: symbol changed {ob} -> {nb}")
            continue
        if b is not None:
            # Address absent post-apply: satisfied only when the planned new
            # mangled symbol exists post-apply with identical sizes.
            ob = _ident(b)
            want = renames.get(ob) if ob is not None else None
            found = (
                want is not None
                and any(
                    _ident(r) == want and _sizes_equal(b, r)
                    for r in after.values()
                )
            )
            if found:
                observed.add((ob, want))  # type: ignore[arg-type]
                continue
            diffs.append(
                f"{hex(addr)} {_ident(b)}: address missing post-apply "
                f"(planned: {ob} -> {want})"
            )
            continue
        # Extra address post-apply: satisfied only as the new side of a
        # planned rename whose old side vanished (hash-keyed snapshots).
        na = _ident(a)
        old = rev.get(na) if na is not None else None
        found_old = (
            old is not None
            and any(
                _ident(r) == old and _sizes_equal(r, a)
                for r in before.values()
            )
        )
        if found_old:
            observed.add((old, na))  # type: ignore[arg-type]
            continue
        diffs.append(f"{hex(addr)} {na}: unexplained extra address post-apply")
    for old, new in renames.items():
        if old == new:
            continue
        if (old, new) not in observed:
            # A planned rename that changed nothing observable: either the
            # rename did not take effect, or pairing was lost.  Both FAIL.
            still_old = any(_ident(r) == old for r in after.values())
            if still_old:
                diffs.append(
                    f"planned rename {old} -> {new} not observed post-apply "
                    f"(old symbol still present)"
                )
            else:
                diffs.append(
                    f"planned rename {old} -> {new} not observed post-apply"
                )
    return (len(diffs) == 0), diffs


def touched_files_for_plan(plan: FnPlan, project_root: Path) -> list[Path]:
    """Every repo file an apply would write for *plan*."""
    files: set[Path] = set()
    for edit in plan.symbol_edits:
        files.add(project_root / "config" / edit.region / "symbols.txt")
    for hit in plan.source_hits:
        files.add(hit.path)
    for he in plan.header_edits:
        if not he.already_present:
            files.add(he.path)
    return sorted(files)


def backup_files(
    project_root: Path, files: list[Path], stamp: str
) -> Path:
    """Copy *files* under /tmp/memberize_bulk_backup/<stamp>/; return dir."""
    dest_root = BACKUP_ROOT / stamp
    for path in files:
        try:
            rel = path.relative_to(project_root)
        except ValueError:
            rel = Path(path.name)
        dest = dest_root / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        if path.is_file():
            shutil.copy2(path, dest)
    return dest_root


def restore_files(backup_dir: Path, project_root: Path, files: list[Path]) -> None:
    """Restore *files* byte-identically from *backup_dir*.

    Atomic (incident hardening): each restore copies the backup to a temp
    file in the SAME directory and ``os.replace()``s it into place, so an
    interrupted restore can never truncate ``config/<region>/symbols.txt``
    (or any other restored file).
    """
    for path in files:
        try:
            rel = path.relative_to(project_root)
        except ValueError:
            rel = Path(path.name)
        src = backup_dir / rel
        if src.is_file():
            path.parent.mkdir(parents=True, exist_ok=True)
            fd, tmp_name = tempfile.mkstemp(
                prefix=f".{path.name}.", suffix=".tmp",
                dir=str(path.parent),
            )
            os.close(fd)
            try:
                shutil.copy2(src, tmp_name)
                os.replace(tmp_name, path)
            except BaseException:
                try:
                    os.unlink(tmp_name)
                except OSError:
                    pass
                raise



# --- caller-TU compile guard -------------------------------------------------
# The byte-identity gate only rebuilds the DEFINING unit.  A rename also
# rewrites call sites in other TUs (``Foo_Bar(&x, ...)`` -> ``(&x)->Bar(...)``)
# and those were never compiled: when ``x``'s type in the caller TU is a
# placeholder / TU-local view without the new member (or ``void*``), the
# caller silently stops compiling (2026-10-23: 12 broken TUs).  This guard
# rebuilds every object whose recorded deps include a touched file and
# rejects the rename on any compile failure.
GUARD_BUILD_REGION = "us"


def _ninja_bin(project_root: Path) -> str:
    local = project_root / "build" / "tools" / "ninja"
    return str(local) if local.is_file() else (shutil.which("ninja") or "ninja")


def dependent_objects(project_root: Path, files: list[Path],
                      region: str = GUARD_BUILD_REGION) -> list[str]:
    """Objects under build/<region>/ whose ninja deps include any of *files*.

    Uses ``ninja -t deps`` (the depfile log), so both rewritten sources and
    rewritten headers map to every TU that actually includes them.
    """
    want: set[str] = set()
    for f in files:
        if f.suffix in (".c", ".cpp", ".h", ".hpp", ".inc"):
            p = f if f.is_absolute() else project_root / f
            want.add(str(p.resolve()))
    if not want:
        return []
    proc = subprocess.run([_ninja_bin(project_root), "-t", "deps"],
                          cwd=str(project_root), capture_output=True, text=True)
    prefix = f"build/{region}/"
    objs: set[str] = set()
    cur: Optional[str] = None
    for line in proc.stdout.splitlines():
        if line and not line[0].isspace():
            tgt = line.split(":", 1)[0]
            cur = tgt if tgt.startswith(prefix) and tgt.endswith(".o") else None
            continue
        dep = line.strip()
        if cur is None or not dep:
            continue
        dp = Path(dep)
        if not dp.is_absolute():
            dp = project_root / dp
        if str(dp.resolve()) in want:
            objs.add(cur)
    return sorted(objs)


def compile_dependents(project_root: Path, files: list[Path]) -> list[str]:
    """Rebuild every object depending on *files*; return failing objects.

    Holds the repo-wide hexdiff build lock like hexdiff itself.  Returns
    ``[]`` when everything compiles (or nothing depends on *files*).
    """
    objs = dependent_objects(project_root, files)
    if not objs:
        return []
    lock_path = project_root / "build" / GUARD_BUILD_REGION / ".hexdiff.lock"
    lock_path.parent.mkdir(parents=True, exist_ok=True)
    import fcntl
    fd = os.open(str(lock_path), os.O_CREAT | os.O_RDWR)
    try:
        try:
            fcntl.flock(fd, fcntl.LOCK_EX)
        except OSError:
            pass
        proc = subprocess.run([_ninja_bin(project_root), "-k", "0", *objs],
                              cwd=str(project_root), capture_output=True,
                              text=True)
    finally:
        os.close(fd)
    if proc.returncode == 0:
        return []
    failed = re.findall(r"^FAILED: (?:\[code=\d+\] )?(\S+)", proc.stdout, re.M)
    return failed or [f"ninja exit {proc.returncode}"]


def sync_targets(project_root: Path) -> int:
    """Sanctioned registry sync: ``run.py targets sync-symbols``."""
    cmd = [sys.executable, str(ROOT / "tools" / "coop" / "run.py"),
           "targets", "sync-symbols"]
    proc = subprocess.run(cmd, cwd=str(project_root), capture_output=True, text=True)
    print(proc.stdout, end="")
    if proc.stderr:
        print(proc.stderr, end="", file=sys.stderr)
    return proc.returncode


def regen_ctx(project_root: Path, ctx_files: list[str]) -> list[str]:
    """Regenerate only the .ctx.c files named by applied plans."""
    done: list[str] = []
    for ctx in sorted(set(ctx_files)):
        ctx_path = project_root / ctx
        # Sibling source: Foo.ctx.c -> Foo.cpp (else .c).
        sibling = ctx_path.with_name(ctx_path.name.replace(".ctx.c", ".cpp"))
        if not sibling.is_file():
            sibling = ctx_path.with_name(ctx_path.name.replace(".ctx.c", ".c"))
        if not sibling.is_file():
            print(f"ctx: no sibling source for {ctx}; skipping")
            continue
        cmd = [sys.executable, str(ROOT / "tools" / "coop" / "run.py"),
               "ctx", str(sibling.relative_to(project_root))]
        proc = subprocess.run(cmd, cwd=str(project_root), capture_output=True, text=True)
        if proc.returncode != 0:
            print(f"ctx regen failed for {sibling}: {proc.stderr[:300]}", file=sys.stderr)
            continue
        done.append(ctx)
    return done


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------


def cmd_plan(args: argparse.Namespace, project_root: Path) -> int:
    scan_path = Path(args.scan_json)
    if not scan_path.is_absolute():
        scan_path = project_root / scan_path
    candidates = load_scan_candidates(scan_path)
    plan_data = plan_all(project_root, candidates)
    text = render_table(plan_data)
    if args.report:
        rp = Path(args.report)
        if not rp.is_absolute():
            rp = project_root / rp
        rp.write_text(text, encoding="utf-8")
        print(f"wrote {rp}")
    else:
        print(text, end="")
    if args.json:
        jp = Path(args.json)
        if not jp.is_absolute():
            jp = project_root / jp
        jp.write_text(json.dumps(plan_data, indent=1) + "\n", encoding="utf-8")
        print(f"wrote {jp}")
    return 0


def _load_plan_rows(plan_json: Path) -> list[dict]:
    data = json.loads(plan_json.read_text(encoding="utf-8"))
    if isinstance(data, dict) and "candidates" in data:
        return list(data["candidates"])
    if isinstance(data, list):
        return list(data)
    raise ValueError(f"unrecognized plan JSON shape in {plan_json}")


def cmd_apply(args: argparse.Namespace, project_root: Path) -> int:
    execute = bool(getattr(args, "execute", False))
    if not execute:
        print("dry-run: no files written (pass --execute to apply).")
    plan_json = Path(args.plan_json) if args.plan_json else None
    if plan_json is not None and not plan_json.is_absolute():
        plan_json = project_root / plan_json
    rows = _load_plan_rows(plan_json) if plan_json else []
    clean = [r for r in rows if r.get("classification") == "CLEAN"]
    if getattr(args, "candidate", None):
        wanted = set(args.candidate)
        clean = [r for r in clean if r.get("old") in wanted]
    if getattr(args, "unit", None):
        clean = [r for r in clean if r.get("unit") == args.unit]
    if getattr(args, "limit", None):
        clean = clean[: args.limit]
    by_unit: dict[str, list[dict]] = {}
    for row in clean:
        by_unit.setdefault(row.get("unit") or "unknown", []).append(row)
    try:
        regions = parse_region_list(getattr(args, "region", None))
    except ValueError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2
    report: dict = {"units": {}, "summary": {}, "regions": regions}
    n_pass = n_partial = n_fail = 0
    snapshotter: Optional[Callable[[Path, str], dict]] = None
    if getattr(args, "_snapshotter", None) is not None:
        snapshotter = args._snapshotter  # test seam
    applier: Optional[Callable] = getattr(args, "_applier", None)
    if not execute:
        for unit in sorted(by_unit):
            print(f"would apply unit {unit}: {len(by_unit[unit])} function(s)")
            for row in by_unit[unit]:
                print(f"  {row['old']} -> {row.get('class')}::{row.get('proposed_method')}")
        if getattr(args, "json", None):
            jp = Path(args.json)
            if not jp.is_absolute():
                jp = project_root / jp
            jp.write_text(json.dumps({"dry_run": True,
                                      "units": sorted(by_unit.keys()),
                                      "regions": regions}, indent=1) + "\n")
            print(f"wrote {jp}")
        return 0
    stamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    all_ctx: list[str] = []
    use_test_seam = getattr(args, "_snapshotter", None) is not None
    # Caller compile guard (real path only; tests inject ``_compile_guard``).
    guard: Optional[Callable[[Path, list[Path]], list[str]]] = getattr(
        args, "_compile_guard", None)
    if guard is None and not use_test_seam:
        guard = compile_dependents
    if execute:
        print(f"memberize-bulk apply regions: {', '.join(regions)} "
              f"(a rename never rewrites an unlisted region)")
    for unit in sorted(by_unit):
        rows_u = by_unit[unit]
        raw_renames = {r["old"]: r["mangled"] for r in rows_u if r.get("mangled")}
        # Collect touched files WITHOUT writing: re-plan each candidate to
        # enumerate symbol/source/header paths.
        touched: set[Path] = set()
        plans: list[tuple[dict, FnPlan]] = []
        pre_skipped: list[dict] = []
        pre_candidates: list[dict] = []
        for row in rows_u:
            try:
                plan = plan_function_rename(
                    project_root, row["old"],
                    f"{row['class']}::{row['proposed_method']}",
                    regions=regions,
                    demote_import_band=True,
                    no_header_insert=bool(row.get("no_header_insert")),
                )
            except FnRenameRefusal as exc:
                reason = f"re-plan refused for {row['old']}: {exc}"
                pre_skipped.append({"old": row["old"], "reason": reason})
                pre_candidates.append({"old": row["old"], "status": "SKIP",
                                         "reason": reason})
                continue
            plans.append((row, plan))
            for f in touched_files_for_plan(plan, project_root):
                touched.add(f)
        if not plans:
            reason = ("; ".join(s["reason"] for s in pre_skipped)
                        if pre_skipped else "re-plan refused (no plannable candidates)")
            report["units"][unit] = {"status": "FAIL", "reason": reason,
                                     "functions": [], "skipped": pre_skipped,
                                     "candidates": pre_candidates, "renames": {}}
            n_fail += 1
            continue
        backup_dir = backup_files(project_root, sorted(touched), stamp)
        # Fresh manglings from re-plan (not the possibly-stale JSON): the
        # encoder may have been fixed since the plan was written (u32->Ul).
        fresh_rows = [{"old": r["old"], "mangled": p.new_mangled,
                        "class": r.get("class"),
                        "proposed_method": r.get("proposed_method")}
                       for r, p in plans]
        fresh_raw = {r["old"]: r["mangled"] for r in fresh_rows
                      if r.get("mangled")}
        try:
            snap_fn = snapshotter or snapshot_from_hexdiff
            before = snap_fn(project_root, unit, fresh_raw)
        except Exception as exc:  # noqa: BLE001
            report["units"][unit] = {"status": "FAIL",
                                     "reason": f"baseline snapshot failed: {exc}",
                                     "functions": []}
            n_fail += 1
            continue
        # Address-keyed reconciliation: join plan -> pre-snapshot by
        # ADDRESS, not by raw source name.  The plan key (source old) may
        # not equal the snapshot's retail spelling (already-mangled) nor
        # its decomp spelling (MWCC-mangled old); both miss as
        # ``planned: ... -> ?`` unless re-keyed here.
        aliases, gate_renames, _addr_map, unresolved = \
            build_address_keyed_maps(before, fresh_rows)
        if unresolved:
            restore_files(backup_dir, project_root, sorted(touched))
            report["units"][unit] = {
                "status": "FAIL",
                "reason": ("plan->snapshot join failed: no address for "
                             + ", ".join(unresolved[:5])),
                "functions": [],
            }
            n_fail += 1
            continue
        apply_error: Optional[str] = None
        for row, plan in plans:
            try:
                if applier is not None:
                    applier(project_root, row, plan)
                else:
                    apply_function_rename(
                        project_root, row["old"],
                        f"{row['class']}::{row['proposed_method']}",
                        regions=regions,
                        dry_run=False, allow_review=False,
                        demote_import_band=True,
                        no_header_insert=bool(row.get("no_header_insert")),
                    )
            except Exception as exc:  # noqa: BLE001
                apply_error = f"apply failed for {row['old']}: {exc}"
                break
        if apply_error is not None:
            restore_files(backup_dir, project_root, sorted(touched))
            report["units"][unit] = {"status": "FAIL", "reason": apply_error,
                                     "functions": []}
            n_fail += 1
            continue
        post_fail_reason: Optional[str] = None
        after = None
        try:
            snap_fn = snapshotter or snapshot_from_hexdiff
            after = snap_fn(project_root, unit, aliases)
        except Exception as exc:  # noqa: BLE001
            restore_files(backup_dir, project_root, sorted(touched))
            # A build failure here usually means ONE candidate emitted invalid
            # C++ (e.g. a declaration-drift return type: decl `void*` vs
            # definition `u32`).  Route multi-candidate units into the
            # per-candidate fallback so the good candidates still land; a lone
            # candidate just FAILs.
            if len(plans) <= 1:
                report["units"][unit] = {"status": "FAIL",
                                         "reason": f"post snapshot failed: {exc}",
                                         "functions": []}
                n_fail += 1
                continue
            post_fail_reason = f"post snapshot failed: {exc}"
        # nm verification (real path only): the plan's proposed mangling
        # (e.g. ``...FUi`` for a ``u32`` param) may not match what MWCC
        # actually emits (``...FUl`` -- ``u32`` is ``unsigned long`` under
        # MWCC).  An unverified NEW silently fails ``--pair-alias`` and
        # reads as a false ``unpaired post-apply``.  Check the freshly
        # built .o (the after-snapshot just rebuilt it) and, on a unique
        # method+class hit, re-key the alias/gate to the ACTUAL symbol and
        # re-snapshot once so the renamed function pairs.
        if post_fail_reason is None and not use_test_seam:
            corrected_aliases = dict(aliases)
            corrected_gate = dict(gate_renames)
            notes: list[str] = []
            for row in fresh_rows:
                old, planned = row.get("old"), row.get("mangled")
                if not old or not planned:
                    continue
                actual, fixed, note = correct_new_via_nm(
                    project_root, unit,
                    str(row.get("proposed_method") or ""),
                    str(row.get("class") or ""), planned,
                )
                if note:
                    notes.append(f"{old}: {note}")
                if fixed and actual != planned:
                    # Re-key both maps: alias by retail symbol at the
                    # address, gate by before-ident at the address.
                    addr = find_before_addr(before, old)
                    if addr is not None and addr in before:
                        rec = before[addr]
                        rsym = rec.get("symbol")
                        ident = _ident(rec)
                        if isinstance(rsym, str) and rsym:
                            corrected_aliases[rsym] = actual
                            # Drop the stale planned alias value if it
                            # differs (keep the retail key, fix the value).
                        if isinstance(ident, str) and ident:
                            corrected_gate.pop(ident, None)
                            corrected_gate[ident] = actual
            if corrected_aliases != aliases or corrected_gate != gate_renames:
                aliases, gate_renames = corrected_aliases, corrected_gate
                try:
                    after = snap_fn(project_root, unit, aliases)
                except Exception as exc:  # noqa: BLE001
                    restore_files(backup_dir, project_root, sorted(touched))
                    report["units"][unit] = {
                        "status": "FAIL",
                        "reason": f"post snapshot (nm-corrected) failed: {exc}",
                        "functions": [],
                    }
                    n_fail += 1
                    continue
        if post_fail_reason is not None:
            ok, diffs = False, [post_fail_reason]
        else:
            ok, diffs = compare_snapshots(before, after, gate_renames)
        if not ok:
            # --- per-candidate fallback: one bad candidate must not kill
            # the good ones in its unit.  Restore the combined attempt,
            # then retry each plannable candidate ONE AT A TIME under the
            # same strict byte-identity gate (address/size/mismatch/match%
            # identical; a rename may differ in name only).  Keep each
            # candidate that verifies; revert only the one that fails.
            combined_reason = "; ".join(diffs)[:2000]
            restore_files(backup_dir, project_root, sorted(touched))
            if len(plans) <= 1:
                skipped = list(pre_skipped)
                cands = list(pre_candidates)
                failed_old = plans[0][0]["old"] if plans else "?"
                skipped.append({"old": failed_old, "reason": combined_reason})
                cands.append({"old": failed_old, "status": "SKIP",
                                "reason": combined_reason})
                report["units"][unit] = {"status": "FAIL",
                                         "reason": combined_reason,
                                         "functions": [], "skipped": skipped,
                                         "candidates": cands, "renames": {}}
                n_fail += 1
                continue
            print(f"unit {unit}: combined apply failed verification; "
                  f"retrying {len(plans)} candidate(s) one at a time")
            passed_rows: list[dict] = []
            passed_renames: dict[str, str] = {}
            skipped: list[dict] = list(pre_skipped)
            cands: list[dict] = list(pre_candidates)
            for row, plan in plans:
                single_old = row["old"]
                single_new = plan.new_mangled
                # Fresh baseline per candidate: captures previously-kept
                # renames in this unit (files already contain them).
                try:
                    cur_before = snap_fn(project_root, unit,
                                          {single_old: single_new})
                except Exception as exc:  # noqa: BLE001
                    reason = f"baseline snapshot failed: {exc}"
                    skipped.append({"old": single_old, "reason": reason})
                    cands.append({"old": single_old, "status": "SKIP",
                                    "reason": reason})
                    continue
                _s_aliases, s_gate, _s_map, s_unresolved = \
                    build_address_keyed_maps(
                        cur_before,
                        [{"old": single_old, "mangled": single_new}])
                if s_unresolved:
                    reason = ("plan->snapshot join failed: no address for "
                              + single_old)
                    skipped.append({"old": single_old, "reason": reason})
                    cands.append({"old": single_old, "status": "SKIP",
                                    "reason": reason})
                    continue
                single_touched = touched_files_for_plan(plan, project_root)
                single_backup = backup_files(
                    project_root, single_touched, stamp + f"-retry-{single_old}")
                try:
                    if applier is not None:
                        applier(project_root, row, plan)
                    else:
                        apply_function_rename(
                            project_root, row["old"],
                            f"{row['class']}::{row['proposed_method']}",
                            regions=regions,
                            dry_run=False, allow_review=False,
                            demote_import_band=True,
                            no_header_insert=bool(row.get("no_header_insert")),
                        )
                except Exception as exc:  # noqa: BLE001
                    restore_files(single_backup, project_root, single_touched)
                    reason = f"apply failed for {single_old}: {exc}"
                    skipped.append({"old": single_old, "reason": reason})
                    cands.append({"old": single_old, "status": "SKIP",
                                    "reason": reason})
                    continue
                try:
                    single_after = snap_fn(project_root, unit, _s_aliases)
                except Exception as exc:  # noqa: BLE001
                    restore_files(single_backup, project_root, single_touched)
                    reason = f"post snapshot failed: {exc}"
                    skipped.append({"old": single_old, "reason": reason})
                    cands.append({"old": single_old, "status": "SKIP",
                                    "reason": reason})
                    continue
                # NOTE: no nm correction in the per-candidate path when
                # running under the test seam (same rule as combined).  On
                # the real path the combined attempt already ran nm
                # correction; a single retry whose planned NEW mismatches
                # nm simply FAILs the strict gate loudly (never silently
                # pairs the wrong overload).
                ok_s, diffs_s = compare_snapshots(
                    cur_before, single_after, s_gate)
                if ok_s and guard is not None:
                    broken = guard(project_root, single_touched)
                    if broken:
                        ok_s = False
                        diffs_s = ["caller compile guard: rename broke "
                                   + ", ".join(broken[:10])]
                if ok_s:
                    passed_rows.append(row)
                    passed_renames.update(s_gate)
                    all_ctx.extend(plan.ctx_files)
                    cands.append({
                        "old": single_old, "status": "PASS",
                        "reason": ("byte-identical (address/size/mismatch/"
                                   "match% identical; name only)"),
                    })
                else:
                    restore_files(single_backup, project_root, single_touched)
                    reason = "; ".join(diffs_s)[:2000]
                    skipped.append({"old": single_old, "reason": reason})
                    cands.append({"old": single_old, "status": "SKIP",
                                    "reason": reason})
            if passed_rows and skipped:
                report["units"][unit] = {
                    "status": "PARTIAL",
                    "reason": (f"combined failed ({combined_reason}); "
                               f"per-candidate: {len(passed_rows)} PASS / "
                               f"{len(skipped)} SKIP"),
                    "functions": [r["old"] for r in passed_rows],
                    "skipped": skipped, "candidates": cands,
                    "renames": passed_renames,
                }
                n_partial += 1
            elif passed_rows and not skipped:
                for _row, _plan in plans:
                    pass
                report["units"][unit] = {
                    "status": "PASS",
                    "functions": [r["old"] for r in passed_rows],
                    "renames": passed_renames, "skipped": [],
                    "candidates": cands,
                }
                n_pass += 1
            else:
                report["units"][unit] = {
                    "status": "FAIL",
                    "reason": (f"combined failed ({combined_reason}); "
                               f"per-candidate: all {len(skipped)} SKIP"),
                    "functions": [], "skipped": skipped,
                    "candidates": cands, "renames": {},
                }
                n_fail += 1
            continue
        # Caller-TU compile guard: the gate above only rebuilt *unit*.
        if guard is not None:
            broken = guard(project_root, sorted(touched))
            if broken:
                restore_files(backup_dir, project_root, sorted(touched))
                report["units"][unit] = {
                    "status": "FAIL",
                    "reason": ("caller compile guard: rename broke "
                               + ", ".join(broken[:10])),
                    "functions": [],
                }
                n_fail += 1
                continue
        # PASS: collect ctx files named by the plans.
        for _row, plan in plans:
            all_ctx.extend(plan.ctx_files)
        passed_cands = [{"old": r["old"], "status": "PASS",
                           "reason": ("byte-identical (address/size/mismatch/"
                                      "match% identical; name only)")}
                          for r in rows_u if r["old"] in [p[0]["old"] for p in plans]]
        passed_cands.extend(pre_candidates)
        report["units"][unit] = {"status": "PASS",
                                 "functions": [r[0]["old"] for r in plans],
                                 "renames": gate_renames,
                                 "skipped": pre_skipped,
                                 "candidates": passed_cands}
        n_pass += 1
    # Post-pass: sanctioned sync + ctx regen (only after real writes).
    # PARTIAL units kept passing candidates, so they join the post-pass.
    if n_pass + n_partial:
        if getattr(args, "_no_post", False):
            pass
        else:
            sync_targets(project_root)
            regen_ctx(project_root, all_ctx)
    report["summary"] = {"pass": n_pass, "partial": n_partial,
                         "fail": n_fail, "units": len(by_unit)}
    def _unit_line(u: str, info: dict) -> str:
        status = info.get("status")
        if status == "PASS":
            return f"unit {u}: PASS ({'; '.join(info.get('functions', []))})"
        if status == "PARTIAL":
            ok = "; ".join(info.get("functions", []))
            bad = "; ".join(s.get("old", "?") for s in info.get("skipped", []))
            return f"unit {u}: PARTIAL (PASS: {ok}; SKIP: {bad})"
        return f"unit {u}: {status} ({info.get('reason', '')[:200]})"
    text_lines = [_unit_line(u, info)
                  for u, info in sorted(report["units"].items())]
    # Per-candidate detail lines (PASS/SKIP with reason).
    for u, info in sorted(report["units"].items()):
        for cand in info.get("candidates", []):
            text_lines.append(
                f"  candidate {cand.get('old')}: {cand.get('status')}" +
                (f" ({cand.get('reason', '')[:160]})" if cand.get("reason") else ""))
    text_lines.append(
        f"summary: {n_pass} PASS / {n_partial} PARTIAL / {n_fail} FAIL")
    text = "\n".join(text_lines) + "\n"
    if getattr(args, "report", None):
        rp = Path(args.report)
        if not rp.is_absolute():
            rp = project_root / rp
        rp.write_text(text, encoding="utf-8")
        print(f"wrote {rp}")
    else:
        print(text, end="")
    if getattr(args, "json", None):
        jp = Path(args.json)
        if not jp.is_absolute():
            jp = project_root / jp
        jp.write_text(json.dumps(report, indent=1) + "\n", encoding="utf-8")
        print(f"wrote {jp}")
    # PARTIAL still exits nonzero: some candidates were skipped and need
    # attention, even though the passing ones were kept.
    return 0 if (n_fail == 0 and n_partial == 0) else 1


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        prog="memberize-bulk",
        description=__doc__,
    )
    parser.add_argument("--project-root", type=Path, default=ROOT,
                        help="repo root (tests pass a fixture dir)")
    sub = parser.add_subparsers(dest="cmd", required=True)
    p_plan = sub.add_parser("plan", help="read-only bulk member-ization plan")
    p_plan.add_argument("--scan-json", required=True,
                        help="extc scan JSON (member_candidates)")
    p_plan.add_argument("--json", default=None, help="write plan JSON here")
    p_plan.add_argument("--report", default=None, help="write human table here")
    p_apply = sub.add_parser("apply", help="supervised bulk apply (default dry-run)")
    p_apply.add_argument("--plan-json", default=None,
                         help="plan JSON from the plan step (CLEAN set)")
    p_apply.add_argument("--candidate", action="append", default=[],
                         help="restrict to this old symbol (repeatable)")
    p_apply.add_argument(
        "--unit", default=None,
        help="restrict to this unit (single-unit retry: applies just this "
        "unit's CLEAN rows with the byte-identity gate, then runs targets "
        "sync-symbols + ctx regen on PASS; other plan units untouched)",
    )
    p_apply.add_argument("--limit", type=int, default=None)
    p_apply.add_argument("--execute", action="store_true",
                         help="actually write (default is dry-run)")
    p_apply.add_argument("--dry-run", action="store_true", default=False,
                         help="explicit dry-run (default anyway without --execute)")
    p_apply.add_argument("--region", default="us",
                         help=("restrict symbols.txt rewrites to these regions "
                               "(comma list, e.g. --region us or --region us,eu; "
                               "default: us only; a rename never rewrites an "
                               "unlisted region)"))
    p_apply.add_argument("--json", default=None, help="write apply report JSON here")
    p_apply.add_argument("--report", default=None, help="write human report here")
    args = parser.parse_args(argv)
    project_root = Path(args.project_root)
    if args.cmd == "plan":
        return cmd_plan(args, project_root)
    if args.cmd == "apply":
        return cmd_apply(args, project_root)
    parser.error(f"unknown subcommand: {args.cmd}")
    return 2


if __name__ == "__main__":
    raise SystemExit(main())

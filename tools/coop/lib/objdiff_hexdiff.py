"""Objdiff cross-check, combined verdicts and baseline regression for hexdiff.

hexdiff proves instruction bytes and relocation *link addresses*; objdiff's
``fuzzy_match_percent`` is what checkpoint reports (``build/us/report.json``)
judge.  They disagree in both directions:

* proof pass / objdiff < 100 — a decomp reloc points at an anonymous
  ``@stringBase0+off`` while retail names ``lbl_eu_X`` (same linked address);
* objdiff 100 / proof fail — both sides name the same symbol but hexdiff sees
  a different linked address.

This module joins the two signals.  Report files are treated as read-only:
hexdiff generates its own report into a temp file, never touching
``build/<region>/report.json``.

Objdiff reports name functions after the **retail** symbol.  A function is
only paired when the decomp object has a code symbol with exactly that name.
objdiff-cli 3.0.0-beta.14 (``report.rs`` ~258-262) assigns
``fuzzy_match_percent = 100.0`` to unpaired rows in ``complete: true`` units,
so a name mismatch can masquerade as a perfect match.  Callers that know the
decomp symbols pass them to ``parse_units``/``parse_report`` so those rows are
marked unpaired (and phantoms) before any verdict or baseline comparison.
"""

from __future__ import annotations

import json
import re
import subprocess
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Any, Iterable, Mapping, Sequence

from tools.ppc_equivalence.elf_symbols import _resolve_candidates as _resolve_symbol_names

PERFECT_PERCENT = 100.0
_EPS = 1e-9

FULL_MARK = "✓"
RELOC_NAME_DRIFT_MARK = "≈"
LINKED_ADDRESS_MISMATCH_MARK = "◇"
BOTH_FAIL_MARK = "✗"
UNPAIRED_MARK = "?"

# JSON uses semantic codes; `mark` carries the single-character terminal glyph.
VERDICT_CODES = {
    FULL_MARK: "full_match",
    RELOC_NAME_DRIFT_MARK: "reloc_name_drift",
    LINKED_ADDRESS_MISMATCH_MARK: "linked_address_mismatch",
    BOTH_FAIL_MARK: "both_fail",
    UNPAIRED_MARK: "objdiff_unpaired",
}

EXIT_LOST_BASELINE = 6

VERDICT_LEGEND = (
    "marks: ✓ both proofs pass | ≈ reloc-name drift (objdiff) | "
    "◇ linked-address mismatch (hexdiff) | ✗ both fail | "
    "? objdiff unpaired (name/linkage mismatch)"
)

REASONS = {
    FULL_MARK: "all checks pass",
    RELOC_NAME_DRIFT_MARK: "reloc-name drift (objdiff)",
    LINKED_ADDRESS_MISMATCH_MARK: "linked-address mismatch (hexdiff)",
    BOTH_FAIL_MARK: "byte/reloc proof and objdiff both fail",
    UNPAIRED_MARK: "objdiff unpaired (name/linkage mismatch)",
}


@dataclass(frozen=True)
class ReportFunction:
    """One function entry from an objdiff report.json (name + fuzzy percent)."""

    name: str
    percent: float | None
    address: int | None = None
    demangled_name: str | None = None
    # True when objdiff omitted fuzzy_match_percent entirely (unpaired symbol)
    # or when no exact-name decomp code symbol exists (phantom pairing).
    unpaired: bool = False
    # The report's unit metadata ``complete`` flag (Matching in configure.py).
    complete: bool = False
    # Unpaired row that objdiff still scored 100 because the unit is complete.
    phantom: bool = False

    @property
    def perfect(self) -> bool:
        return self.percent is not None and self.percent >= PERFECT_PERCENT - _EPS


@dataclass(frozen=True)
class Verdict:
    mark: str
    bytes_match: bool
    full_match: bool
    objdiff_percent: float | None
    unpaired: bool = False

    @property
    def code(self) -> str:
        return VERDICT_CODES[self.mark]

    @property
    def reason(self) -> str:
        return REASONS[self.mark]

    def json_fields(self) -> dict[str, Any]:
        """JSON keys shared by the --symbol and --all JSON outputs.

        ``match`` is an alias of ``full_match``; ``bytes_match`` preserves the
        old byte/reloc-only meaning.  See consumers listed in the handoff.
        """
        return {
            "bytes_match": self.bytes_match,
            "full_match": self.full_match,
            "match": self.full_match,
            "objdiff_percent": self.objdiff_percent,
            "verdict": self.code,
            "mark": self.mark,
            "unpaired": self.unpaired,
            "verdict_reason": self.reason,
        }


@dataclass(frozen=True)
class BaselineChange:
    kind: str  # LOST, GAIN or PHANTOM
    symbol: str
    old: float | None
    new: float | None
    paired_symbol: str | None = None

    def render(self) -> str:
        old_s = f"{self.old:.2f}%" if self.old is not None else "n/a"
        new_s = f"{self.new:.2f}%" if self.new is not None else "n/a"
        notes = []
        if self.paired_symbol and self.paired_symbol != self.symbol:
            notes.append(f"paired: {self.paired_symbol}")
        if self.kind == "PHANTOM":
            notes.append("baseline complete-unit 100; decomp name differs")
        note = f" [{' | '.join(notes)}]" if notes else ""
        return f"{self.kind} {self.symbol} {old_s} -> {new_s}{note}"

    def as_dict(self) -> dict[str, Any]:
        return asdict(self)


def _to_int(value: Any) -> int | None:
    if value is None or value == "":
        return None
    try:
        return int(str(value), 0)
    except (TypeError, ValueError):
        return None


def parse_units(
    report: Mapping[str, Any],
    *,
    decomp_symbols: Mapping[str, Iterable[str]] | None = None,
) -> dict[str, list[ReportFunction]]:
    """Parse an objdiff report into {unit name: [ReportFunction, ...]}.

    ``decomp_symbols`` maps unit name to the exact code-symbol names in that
    unit's decomp object.  Report names are retail names: without an exact
    decomp symbol the row is unpaired even if objdiff printed 100.0 (the
    complete-unit phantom bug).  A missing fuzzy key is always unpaired.
    When the decomp symbols are unknown (e.g. a bare checkpoint report) only
    the fuzzy key is available, preserving the old behaviour.
    """
    out: dict[str, list[ReportFunction]] = {}
    for unit in report.get("units", []):
        unit_name = str(unit.get("name", ""))
        complete = bool((unit.get("metadata") or {}).get("complete"))
        known = None
        if decomp_symbols is not None:
            known = set(decomp_symbols.get(unit_name, ()))
        functions = []
        for fn in unit.get("functions", []):
            meta = fn.get("metadata") or {}
            raw = fn.get("fuzzy_match_percent")
            try:
                percent = float(raw) if raw is not None else None
            except (TypeError, ValueError):
                percent = None
            name = str(fn.get("name", ""))
            no_exact = known is not None and name not in known
            unpaired = raw is None or no_exact
            phantom = bool(
                no_exact and complete and raw is not None
                and percent is not None and percent >= PERFECT_PERCENT - _EPS
            )
            functions.append(ReportFunction(
                name=name,
                percent=percent,
                address=_to_int(meta.get("virtual_address")),
                demangled_name=meta.get("demangled_name"),
                unpaired=unpaired,
                complete=complete,
                phantom=phantom,
            ))
        out[unit_name] = functions
    return out


def parse_report(
    report: Mapping[str, Any],
    unit_name: str,
    *,
    decomp_symbols: Mapping[str, Iterable[str]] | None = None,
) -> list[ReportFunction] | None:
    """Return this unit's functions; None means the report has no such unit."""
    return parse_units(report, decomp_symbols=decomp_symbols).get(unit_name)


def load_report_unit(
    path: Path,
    unit_name: str,
    *,
    decomp_symbols: Mapping[str, Iterable[str]] | None = None,
) -> list[ReportFunction] | None:
    with Path(path).open(encoding="utf-8") as handle:
        return parse_report(json.load(handle), unit_name,
                            decomp_symbols=decomp_symbols)


def generate_report(project: Any, output_path: Path) -> dict[str, Any]:
    """Generate a whole-project objdiff report at *output_path* and parse it.

    Uses the project's ``objdiff_report_args`` so percentages match the
    checkpoint reports (``--config functionRelocDiffs=data_value`` in this
    fork).  Runs in ~0.1s; no mtime cache is needed.  Callers own the temp dir.
    """
    cmd = [
        project.objdiff_bin(), "report", "generate",
        "-p", ".", "-o", str(output_path), "-f", "json",
        *project.config.objdiff_report_args,
    ]
    proc = subprocess.run(cmd, cwd=project.root, check=False,
                          capture_output=True, text=True)
    if proc.returncode != 0 or not Path(output_path).is_file():
        raise RuntimeError(
            f"objdiff report generate failed (rc={proc.returncode}): "
            f"{proc.stderr[-2000:]}"
        )
    with Path(output_path).open(encoding="utf-8") as handle:
        return json.load(handle)


def pair_report_function(
    functions: Sequence[ReportFunction], *names: str | None
) -> ReportFunction | None:
    """Find the report entry for one of *names* (decomp name first).

    Report function names are decomp symbol names; retail objects often keep
    short/mangled variants.  Reuse the hexdiff/ppc_equivalence resolver after
    exact and case-insensitive lookups, accepting only unique hits.
    """
    wanted = [name for name in names if name]
    for name in wanted:
        for fn in functions:
            if fn.name == name or fn.demangled_name == name:
                return fn
    lowered = {name.lower() for name in wanted}
    for fn in functions:
        if fn.name.lower() in lowered or (fn.demangled_name or "").lower() in lowered:
            return fn
    for name in wanted:
        hits = _resolve_symbol_names(list(functions), name)
        if len(hits) == 1:
            return hits[0]
    return None


def objdiff_percent(
    functions: Sequence[ReportFunction], *names: str | None
) -> float | None:
    hit = pair_report_function(functions, *names)
    return hit.percent if hit is not None else None


def combine_verdict(bytes_match: bool, objdiff_pct: float | None,
                    *, unpaired: bool = False) -> Verdict:
    """Combine the hexdiff proof with objdiff's per-function percentage.

    An unpaired objdiff function is its own verdict: objdiff never scored the
    function, so "<100" would hide a name/linkage pairing failure.
    """
    if unpaired:
        mark = UNPAIRED_MARK
    else:
        objdiff_ok = objdiff_pct is not None and objdiff_pct >= PERFECT_PERCENT - _EPS
        if bytes_match and objdiff_ok:
            mark = FULL_MARK
        elif bytes_match:
            mark = RELOC_NAME_DRIFT_MARK
        elif objdiff_ok:
            mark = LINKED_ADDRESS_MISMATCH_MARK
        else:
            mark = BOTH_FAIL_MARK
    return Verdict(mark=mark, bytes_match=bool(bytes_match),
                   full_match=mark == FULL_MARK, objdiff_percent=objdiff_pct,
                   unpaired=unpaired)


def load_retail_addresses(root: Path, region: str) -> dict[str, int]:
    """Symbol -> retail address from targets.json and config/<region>/symbols.txt.

    Used only as a rename fallback: if a baseline symbol disappears, its
    address still identifies the function in a fresh report.
    """
    out: dict[str, int] = {}
    targets = Path(root) / "tools" / "coop" / "targets.json"
    try:
        rows = json.loads(targets.read_text(encoding="utf-8")).get("targets", [])
        for row in rows:
            addr = _to_int(row.get("address"))
            if addr is None:
                continue
            for key in ("symbol", "function"):
                name = row.get(key)
                if isinstance(name, str) and name:
                    out.setdefault(name, addr)
    except (OSError, ValueError, AttributeError):
        pass
    symbols = Path(root) / "config" / region / "symbols.txt"
    try:
        for line in symbols.read_text(encoding="utf-8", errors="replace").splitlines():
            match = re.match(r"^\s*([^\s=]+)\s*=\s*\.(?:text|init):(0x[0-9a-fA-F]+)", line)
            if match:
                out.setdefault(match.group(1), int(match.group(2), 16))
    except OSError:
        pass
    return out


def _counterpart(
    source: ReportFunction,
    candidates: Sequence[ReportFunction],
    addresses: Mapping[str, int],
) -> ReportFunction | None:
    hit = pair_report_function(candidates, source.name, source.demangled_name)
    if hit is not None:
        return hit
    by_address = {fn.address: fn for fn in candidates if fn.address is not None}
    addresses_to_try = {source.address}
    for name in (source.name, source.demangled_name):
        if name and name in addresses:
            addresses_to_try.add(addresses[name])
    for address in addresses_to_try:
        if address is not None and address in by_address:
            return by_address[address]
    return None


def compare_baseline(
    old_functions: Sequence[ReportFunction],
    new_functions: Sequence[ReportFunction],
    *,
    addresses: Mapping[str, int] | None = None,
    decomp_symbols: Iterable[str] | None = None,
) -> list[BaselineChange]:
    """Per-symbol PHANTOM/LOST/GAIN relative to a checkpoint report.

    Only functions at 100.0 are tracked: a LOST is a 100 -> <100 (or missing)
    transition, a GAIN is a <100 (or missing) -> 100 transition.  Missing
    symbols are paired by name resolver first, then by retail address
    (report metadata or targets.json/symbols.txt) before being called LOST.

    A baseline 100 in a ``complete: true`` unit is suspect: objdiff 3.0.0-beta.14
    scores unpaired rows 100 there.  When the function is unpaired *now* and
    the current decomp symbol name differs from the retail name (or the
    function was renamed), the baseline 100 was likely the same phantom, so
    the change is reported as PHANTOM instead of LOST (and does not trigger
    the baseline exit code).  ``decomp_symbols`` is the current unit's exact
    decomp code-symbol names; without them the old conservative LOST is kept.
    """
    addresses = addresses or {}
    decomp_names = set(decomp_symbols) if decomp_symbols is not None else None
    changes: list[BaselineChange] = []
    for old in old_functions:
        if not old.perfect:
            continue
        new = _counterpart(old, new_functions, addresses)
        if new is not None and new.perfect and not new.unpaired:
            continue
        name_differs = (
            (new is not None and new.name != old.name)
            or (decomp_names is not None and old.name not in decomp_names)
        )
        kind = (
            "PHANTOM"
            if old.complete and new is not None and new.unpaired and name_differs
            else "LOST"
        )
        changes.append(BaselineChange(
            kind=kind, symbol=old.name, old=old.percent,
            new=new.percent if new is not None else None,
            paired_symbol=new.name if new is not None and new.name != old.name else None,
        ))

    for new in new_functions:
        if not new.perfect or new.unpaired:
            continue
        old = _counterpart(new, old_functions, addresses)
        if old is not None and old.perfect:
            continue
        changes.append(BaselineChange(
            kind="GAIN", symbol=new.name,
            old=old.percent if old is not None else None,
            new=new.percent,
            paired_symbol=old.name if old is not None and old.name != new.name else None,
        ))
    return changes


def changes_for_symbols(
    changes: Iterable[BaselineChange], *names: str | None
) -> list[BaselineChange]:
    """Narrow unit-wide baseline changes to one --symbol query."""
    wanted = {name for name in names if name}
    return [c for c in changes
            if c.symbol in wanted or (c.paired_symbol and c.paired_symbol in wanted)]

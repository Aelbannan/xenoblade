#!/usr/bin/env python3
"""Reusable registry-drift verification gate for the decomp function registry.

Compares the accepted entries in ``tools/coop/targets.json`` against a fresh
``hexdiff.py <unit> --all --json`` build for each unit and fails on any drift.

Usage:
  python3 tools/coop/verify_units.py <unit> [<unit> ...]
  python3 tools/coop/verify_units.py --units-file <path>
  python3 tools/coop/verify_units.py --all
  python3 tools/coop/verify_units.py --all --roots src/kyoshin libs/monolib/src
  python3 tools/coop/verify_units.py kyoshin/cf/object/CfObjectEff --no-build
  python3 tools/coop/verify_units.py --all --limit 5 --json

Options:
  --units-file <path>   file with one unit per line (blank lines / `#` ignored)
  --all                 every unit that has an accepted target in targets.json
  --roots R [R ...]     only units under these source roots (prefix match
                        against the target `source`, e.g. `src/kyoshin`)
  --limit N             check at most N units (sorted order; for smoke runs)
  --json                machine-readable JSON on stdout instead of the table
  --no-build            pass through to hexdiff.py (skip the ninja rebuild)
  --build-timeout SEC   pass through to hexdiff.py (default: 600)
  --tolerance PCT       allowed drop below the recorded EQUIVALENT_MATCH
                        percent, in percentage points (default: 0.1)
  --targets PATH        alternate targets.json (default: tools/coop/targets.json)

Rules (read-only w.r.t. source and targets.json — this tool never writes):
  * Only `FULL_MATCH` / `EQUIVALENT_MATCH` registry entries are checked
    (mirrors `ptr_sites.ACCEPTED_STATUSES`); anything else is not a promise.
  * Targets whose `source` contains `.ctx.` are skipped (preprocessed context
    dumps, not source — mirrors `detect_pointer_arithmetic.find_source_files`).
  * `FULL_MATCH` requires the hexdiff row to have `match == true`.
  * `EQUIVALENT_MATCH` requires the current percent to be no lower than the
    recorded `instruction_match` minus the tolerance. The current percent is
    computed from the JSON row exactly the way the human `hexdiff --all`
    `NN.N%` column is computed: ``100 * (total - mismatch) / total`` with
    ``total = max(retail_size, decomp_size) // 4`` (i.e. `hexdiff._counts_for`
    semantics, so size-difference slots count as mismatches).
  * An accepted symbol with no hexdiff row, or a row with `present == false`,
    is reported separately as `not-in-object` and counts as drift.
  * Object rows with no accepted registry entry are listed separately as
    informational (`unregistered`) and never fail the gate.

Exit codes: 0 = no drift, 1 = drift found, 2 = operational error.

CI note: this fork has no committed `orig/`, so CI cannot build — this is a
developer tool, not a CI job. Do not add a build-based CI workflow for it.
"""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

_REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(_REPO))

TARGETS_DEFAULT = _REPO / "tools" / "coop" / "targets.json"
HEXDIFF = _REPO / "tools" / "coop" / "hexdiff.py"

# Mirrors tools/coop/ptr_sites.py.
ACCEPTED_STATUSES = ("FULL_MATCH", "EQUIVALENT_MATCH")

#: Allowed drop below the recorded EQUIVALENT_MATCH percent (percentage
#: points). Covers the one-decimal rounding of the human `--all` table.
DEFAULT_TOLERANCE = 0.1


# ── pure comparison logic (stubbed in tests; no builds here) ────────────────


def extract_json_blob(text: str) -> dict:
    """Return the `hexdiff --all --json` payload embedded in build output.

    hexdiff prints the ninja build log (to stderr) before the JSON, so when
    stdout/stderr are combined the blob is the line beginning `{"unit":`.
    Scans every line and parses the first one that decodes with a `unit` and
    a `functions` key. Raises ValueError when no blob is found.
    """
    for line in text.splitlines():
        s = line.strip()
        if not s.startswith('{"unit":'):
            continue
        try:
            doc = json.loads(s)
        except ValueError:
            continue
        if isinstance(doc, dict) and "unit" in doc and "functions" in doc:
            return doc
    raise ValueError("no hexdiff JSON blob (line starting with '{\"unit\":') found in output")


def percent_from_row(row: dict) -> float | None:
    """Current match percent for one `--all --json` function row.

    Mirrors `hexdiff._counts_for` / the human `--all` `NN.N%` column:
    ``100 * (total - mismatch) / total`` with
    ``total = max(retail_size, decomp_size) // 4``. Returns None for a
    `present == false` row (no decomp counterpart built); callers treat that
    as `not-in-object`, not as 0%.
    """
    if not row.get("present", False):
        return None
    total = max(int(row.get("retail_size", 0)), int(row.get("decomp_size", 0))) // 4
    mismatch = int(row.get("mismatch", 0))
    if total <= 0:
        return 100.0 if mismatch == 0 else 0.0
    return 100.0 * (total - mismatch) / total


def normalize_unit(unit: str) -> str:
    """Strip the plan prefix (`main/`) so registry and hexdiff units compare."""
    return unit[5:] if unit.startswith("main/") else unit


def is_ctx_source(source: str) -> bool:
    """True for preprocessed `.ctx.` dumps (not real source; never checked)."""
    return ".ctx." in (source or "")


def accepted_targets(targets: list[dict]) -> dict[str, list[dict]]:
    """Index accepted, non-`.ctx.` targets by normalized unit."""
    by_unit: dict[str, list[dict]] = {}
    for t in targets:
        if not isinstance(t, dict):
            continue
        if t.get("status") not in ACCEPTED_STATUSES:
            continue
        if is_ctx_source(t.get("source") or ""):
            continue
        unit = t.get("unit") or ""
        if not unit:
            continue
        by_unit.setdefault(normalize_unit(unit), []).append(t)
    return by_unit


def unit_matches_roots(unit: str, sources: list[str], roots: list[str]) -> bool:
    """True when the unit belongs under one of the `--roots` prefixes.

    Matches against the target `source` paths (`src/kyoshin/...`) and, for
    bare roots like `kyoshin/...`, against both `src/<root>` sources and the
    unit path itself (registry units omit the `src/` prefix).
    """
    if not roots:
        return True
    unit_norm = normalize_unit(unit)
    for root in roots:
        r = root.strip().rstrip("/")
        if not r:
            continue
        candidates = {r, "src/" + r.lstrip("/")}
        if r.startswith("src/"):
            candidates.add(r[4:])
        if r.startswith("libs/"):
            candidates.add(r)
        for src in sources:
            if any(src == c or src.startswith(c + "/") for c in candidates):
                return True
        if any(unit_norm == c or unit_norm.startswith(c + "/") for c in candidates):
            return True
    return False


def check_target(target: dict, row_by_symbol: dict[str, dict],
                 tolerance: float = DEFAULT_TOLERANCE) -> dict:
    """Compare one accepted registry entry against its hexdiff row.

    Returns a finding dict with `verdict` in {"ok", "drift-full",
    "drift-equiv", "not-in-object"} plus `recorded` / `current` percents
    (`current` is None when the symbol has no live object row).
    """
    symbol = target.get("symbol") or ""
    status = target.get("status") or ""
    recorded = target.get("instruction_match")
    recorded_pct = float(recorded) if isinstance(recorded, (int, float)) else 0.0
    row = row_by_symbol.get(symbol)
    base = {
        "unit": target.get("unit") or "",
        "symbol": symbol,
        "function": target.get("function") or "",
        "status": status,
        "recorded": recorded_pct,
        "current": None,
    }
    if row is None or not row.get("present", False):
        return {**base, "verdict": "not-in-object",
                "detail": "accepted symbol has no row in the built object"}
    current = percent_from_row(row)
    out = {**base, "current": current}
    if status == "FULL_MATCH":
        if row.get("match", False):
            return {**out, "verdict": "ok", "detail": "100% match"}
        return {**out, "verdict": "drift-full",
                "detail": f"recorded FULL_MATCH but current is {current:.1f}% "
                           f"({row.get('mismatch')} mismatch, "
                           f"{row.get('structural')} structural + "
                           f"{row.get('reg_swap')} reg_swap)"}
    if status == "EQUIVALENT_MATCH":
        assert current is not None
        if current + 1e-9 >= recorded_pct - tolerance:
            return {**out, "verdict": "ok",
                    "detail": f"{current:.1f}% within tolerance of recorded {recorded_pct}"}
        return {**out, "verdict": "drift-equiv",
                "detail": f"current {current:.1f}% below recorded {recorded_pct} "
                           f"(tolerance {tolerance})"}
    return {**out, "verdict": "ok", "detail": f"status {status} not gated"}


def evaluate_unit(unit: str, hexdiff_doc: dict, accepted: list[dict],
                  tolerance: float = DEFAULT_TOLERANCE) -> dict:
    """Check one unit's accepted targets against a parsed hexdiff `--all --json` doc.

    Pure (no subprocess) so tests can stub `hexdiff_doc`. `accepted` holds the
    registry entries for `unit` (normalized match). Returns
    {"unit", "findings", "drifts", "not_in_object", "unregistered", "stats"}.
    """
    rows = hexdiff_doc.get("functions", []) if isinstance(hexdiff_doc, dict) else []
    row_by_symbol = {r.get("symbol"): r for r in rows if isinstance(r, dict) and r.get("symbol")}
    accepted_symbols = {t.get("symbol") for t in accepted if isinstance(t, dict)}

    findings = [check_target(t, row_by_symbol, tolerance) for t in accepted]
    drifts = [f for f in findings if f["verdict"] in ("drift-full", "drift-equiv")]
    not_in_object = [f for f in findings if f["verdict"] == "not-in-object"]

    # Object rows with no accepted registry entry: informational only. Split
    # absent (`present == false`) rows out so they read separately, mirroring
    # the hexdiff "(not written yet)" marker.
    unregistered: list[dict] = []
    for r in rows:
        sym = r.get("symbol")
        if sym in accepted_symbols:
            continue
        if not r.get("present", False):
            unregistered.append({"symbol": sym, "kind": "absent-unregistered",
                                 "detail": "in retail object, no decomp counterpart, no accepted target"})
        elif not r.get("match", False):
            current = percent_from_row(r)
            unregistered.append({"symbol": sym, "kind": "mismatch-unregistered",
                                 "current": current,
                                 "detail": f"mismatch ({r.get('mismatch')}) with no accepted target"})

    stats = {"accepted": len(accepted),
             "ok": sum(1 for f in findings if f["verdict"] == "ok"),
             "drifts": len(drifts),
             "not_in_object": len(not_in_object),
             "object_functions": len(rows)}
    return {"unit": unit, "findings": findings, "drifts": drifts,
            "not_in_object": not_in_object, "unregistered": unregistered,
            "stats": stats}


# ── subprocess + CLI (thin wrappers over the pure logic above) ──────────────


def run_hexdiff_all(unit: str, no_build: bool = False,
                    build_timeout: int = 600) -> dict:
    """Run `hexdiff.py <unit> --all --json` and return the parsed JSON blob."""
    cmd = [sys.executable, str(HEXDIFF), unit, "--all", "--json"]
    if no_build:
        cmd.append("--no-build")
    if build_timeout != 600:
        cmd += ["--build-timeout", str(build_timeout)]
    try:
        proc = subprocess.run(cmd, cwd=str(_REPO), capture_output=True,
                              text=True, timeout=build_timeout + 60)
    except subprocess.TimeoutExpired as exc:
        raise RuntimeError(f"hexdiff timed out for {unit}: {exc}") from exc
    combined = (proc.stdout or "") + "\n" + (proc.stderr or "")
    try:
        return extract_json_blob(combined)
    except ValueError as exc:
        raise RuntimeError(
            f"hexdiff produced no JSON blob for {unit} "
            f"(exit={proc.returncode}): {(proc.stderr or proc.stdout or '')[-500:]}"
        ) from exc


def resolve_units(args: argparse.Namespace, by_unit: dict[str, list[dict]]) -> list[str]:
    """Collect the normalized unit list from positionals / --units-file / --all."""
    units: list[str] = []
    for u in args.units or []:
        units.append(normalize_unit(u))
    if args.units_file:
        text = Path(args.units_file).read_text(encoding="utf-8", errors="replace")
        for line in text.splitlines():
            s = line.strip()
            if s and not s.startswith("#"):
                units.append(normalize_unit(s))
    if args.all:
        for unit in sorted(by_unit):
            targets = by_unit[unit]
            sources = [t.get("source") or "" for t in targets]
            if unit_matches_roots(unit, sources, args.roots or []):
                units.append(unit)
    elif args.roots:
        # --roots without --all filters the explicitly listed units.
        kept = []
        for unit in units:
            sources = [t.get("source") or "" for t in by_unit.get(unit, [])]
            if unit_matches_roots(unit, sources, args.roots):
                kept.append(unit)
        units = kept
    # De-duplicate, preserving order.
    seen: set[str] = set()
    ordered = []
    for u in units:
        if u not in seen:
            seen.add(u)
            ordered.append(u)
    if args.limit is not None:
        ordered = ordered[: args.limit]
    return ordered


def fmt_pct(value: float | None) -> str:
    return "—" if value is None else f"{value:.1f}%"


def print_table(results: list[dict]) -> None:
    rows: list[tuple[str, str, str, str, str]] = []
    for res in results:
        for f in res["findings"]:
            if f["verdict"] == "ok":
                continue
            rows.append((res["unit"], f["symbol"], f["verdict"],
                         fmt_pct(f["recorded"]), fmt_pct(f["current"])))
    if rows:
        w_unit = max(len(r[0]) for r in rows)
        w_sym = min(max(len(r[1]) for r in rows), 64)
        print(f"  {'unit':<{w_unit}}  {'symbol':<{w_sym}}  {'status':<14}  {'recorded':>8}  {'current':>8}")
        for unit, sym, verdict, rec, cur in rows:
            shown = sym if len(sym) <= w_sym else sym[: w_sym - 1] + "…"
            print(f"  {unit:<{w_unit}}  {shown:<{w_sym}}  {verdict:<14}  {rec:>8}  {cur:>8}")
    else:
        print("  no drift — every accepted target still matches its record.")

    n_units = len(results)
    n_acc = sum(r["stats"]["accepted"] for r in results)
    n_ok = sum(r["stats"]["ok"] for r in results)
    n_drift = sum(r["stats"]["drifts"] for r in results)
    n_nio = sum(r["stats"]["not_in_object"] for r in results)
    print(f"\nsummary: {n_units} unit(s), {n_acc} accepted target(s): "
          f"{n_ok} ok, {n_drift} drift, {n_nio} not-in-object.")

    nio_all = [f for r in results for f in r["not_in_object"]]
    if nio_all:
        print("\nnot-in-object (accepted symbol with no live object row):")
        for f in nio_all:
            print(f"  {f['unit']}  {f['symbol']}  [{f['status']}]")
    unreg = [u for r in results for u in r["unregistered"]]
    if unreg:
        print(f"\nunregistered object rows (informational, {len(unreg)}):")
        for u in unreg[:20]:
            extra = f" {u['current']:.1f}%" if u.get("current") is not None else ""
            print(f"  {u['symbol']}{extra}  ({u['kind']})")
        if len(unreg) > 20:
            print(f"  … and {len(unreg) - 20} more (see --json)")


def build_report(results: list[dict], errors: list[dict]) -> dict:
    return {
        "units": [
            {"unit": r["unit"], "stats": r["stats"], "findings": r["findings"],
             "drifts": r["drifts"], "not_in_object": r["not_in_object"],
             "unregistered": r["unregistered"]}
            for r in results
        ],
        "drifts": [f for r in results for f in r["drifts"]],
        "not_in_object": [f for r in results for f in r["not_in_object"]],
        "unregistered": [u for r in results for u in r["unregistered"]],
        "errors": errors,
        "summary": {
            "units": len(results),
            "accepted": sum(r["stats"]["accepted"] for r in results),
            "ok": sum(r["stats"]["ok"] for r in results),
            "drifts": sum(r["stats"]["drifts"] for r in results),
            "not_in_object": sum(r["stats"]["not_in_object"] for r in results),
        },
    }


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Verify accepted targets.json entries against fresh hexdiff builds.")
    p.add_argument("units", nargs="*", help="unit(s) to verify (e.g. kyoshin/cf/object/CfObjectEff)")
    p.add_argument("--units-file", default=None, help="file with one unit per line")
    p.add_argument("--all", action="store_true",
                   help="every unit with an accepted target in targets.json")
    p.add_argument("--roots", nargs="*", default=[],
                   help="only units under these source roots (e.g. src/kyoshin)")
    p.add_argument("--limit", type=int, default=None, help="check at most N units")
    p.add_argument("--json", action="store_true", help="machine-readable JSON output")
    p.add_argument("--no-build", action="store_true", help="pass through to hexdiff.py")
    p.add_argument("--build-timeout", type=int, default=600)
    p.add_argument("--tolerance", type=float, default=DEFAULT_TOLERANCE,
                   help="allowed drop below recorded EQUIVALENT_MATCH percent")
    p.add_argument("--targets", default=str(TARGETS_DEFAULT))
    return p.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        doc = json.loads(Path(args.targets).read_text(encoding="utf-8"))
    except (OSError, ValueError) as exc:
        print(f"ERROR: cannot read targets {args.targets}: {exc}", file=sys.stderr)
        return 2
    by_unit = accepted_targets(doc.get("targets", []))
    units = resolve_units(args, by_unit)
    if not units:
        print("ERROR: no units selected (pass <unit>, --units-file, or --all).", file=sys.stderr)
        return 2

    results: list[dict] = []
    errors: list[dict] = []
    for unit in units:
        accepted = by_unit.get(unit, [])
        if not accepted:
            errors.append({"unit": unit, "error": "no accepted targets for unit"})
            continue
        try:
            blob = run_hexdiff_all(unit, no_build=args.no_build,
                                   build_timeout=args.build_timeout)
        except RuntimeError as exc:
            errors.append({"unit": unit, "error": str(exc)})
            continue
        results.append(evaluate_unit(unit, blob, accepted, tolerance=args.tolerance))

    report = build_report(results, errors)
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print_table(results)
        if errors:
            print("\nerrors:")
            for e in errors:
                print(f"  {e['unit']}: {e['error']}")

    if report["summary"]["drifts"] or report["summary"]["not_in_object"]:
        return 1
    return 2 if errors else 0


if __name__ == "__main__":
    raise SystemExit(main())

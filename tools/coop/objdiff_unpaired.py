#!/usr/bin/env python3
"""List objdiff-unpaired functions that hexdiff can likely pair by name.

objdiff normally omits ``fuzzy_match_percent`` when retail/decomp symbols do
not pair (e.g. a C retail name vs a C++-mangled decomp symbol). In
``complete: true`` units, objdiff-cli 3.0.0-beta.14 instead scores those rows
100.0; those are reported here as ``phantom`` rows. hexdiff's resolver can
still pair both kinds, so these are cheap-to-fix reporting blind spots.

Usage:
  python3 tools/coop/objdiff_unpaired.py [build/us/report.json] [--json]
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any, Callable, Mapping

_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(_ROOT))

from tools.coop.hexdiff import _resolve_candidates  # noqa: E402
from tools.coop.lib.config import load_config  # noqa: E402
from tools.coop.lib.objdiff_hexdiff import parse_units  # noqa: E402
from tools.coop.lib.project import Project  # noqa: E402
from tools.ppc_equivalence.elf_symbols import list_code_functions as list_text_functions  # noqa: E402


def collect_unpaired(
    report: Mapping[str, Any],
    *,
    resolve_unit: Callable[[str], Any],
    list_functions: Callable[[Path], list],
) -> dict[str, list[dict]]:
    """{unit: [{"symbol", "decomp_symbol", "address", "phantom"}, ...]}.

    Pure apart from injected project/object readers so it can be tested with
    small synthetic reports and objects. ``phantom`` marks a row objdiff
    scored 100 in a complete unit without an exact-name decomp symbol; the
    rest are classic rows with no fuzzy score at all. Only unique
    ``_resolve_candidates`` hits fill ``decomp_symbol`` — an ambiguous pairing
    is not evidence of a simple name/linkage mismatch — but phantoms are
    listed even when no unique candidate exists.
    """
    decomp_functions: dict[str, list] = {}
    decomp_symbols: dict[str, set[str]] = {}
    for raw_unit in report.get("units", []):
        unit_name = str(raw_unit.get("name", ""))
        try:
            unit = resolve_unit(unit_name)
            base_path = getattr(unit, "base_path", None)
        except Exception:
            continue
        if base_path is None or not Path(base_path).is_file():
            continue
        try:
            functions = list_functions(Path(base_path))
        except Exception:
            continue
        decomp_functions[unit_name] = functions
        decomp_symbols[unit_name] = {fn.name for fn in functions}

    found: dict[str, list[dict]] = {}
    for unit_name, functions in parse_units(report, decomp_symbols=decomp_symbols).items():
        missing = [fn for fn in functions if fn.unpaired]
        if not missing:
            continue
        decomp_fn = decomp_functions.get(unit_name, [])
        rows = []
        for fn in missing:
            candidates = _resolve_candidates(decomp_fn, fn.name)
            decomp_symbol = candidates[0].name if len(candidates) == 1 else None
            if decomp_symbol is None and not fn.phantom:
                continue
            rows.append({
                "symbol": fn.name,
                "decomp_symbol": decomp_symbol,
                "address": fn.address,
                "phantom": fn.phantom,
                "complete": fn.complete,
                "fuzzy_match_percent": fn.percent,
            })
        if rows:
            found[unit_name] = rows
    return found


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", nargs="?", help="objdiff report.json (default: build/<region>/report.json)")
    parser.add_argument("--region", help="override the configured region")
    parser.add_argument("--json", action="store_true", help="emit a machine-readable summary")
    args = parser.parse_args(argv)

    config = load_config(None, _ROOT)
    if args.region:
        config.region = args.region
    path = Path(args.report) if args.report else _ROOT / "build" / config.region / "report.json"
    if not path.is_file():
        print(f"ERROR: objdiff report not found: {path}", file=sys.stderr)
        return 2
    try:
        report = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, ValueError) as exc:
        print(f"ERROR: cannot read {path}: {exc}", file=sys.stderr)
        return 2

    found = collect_unpaired(report, resolve_unit=Project(config).resolve_unit,
                             list_functions=list_text_functions)
    total = sum(len(rows) for rows in found.values())
    phantom_total = sum(1 for rows in found.values() for row in rows if row["phantom"])
    noscore_total = total - phantom_total
    if args.json:
        print(json.dumps({"report": str(path), "unit_count": len(found),
                          "total": total, "phantom": phantom_total,
                          "no_score": noscore_total, "units": found}, indent=2))
        return 0
    for unit_name, rows in sorted(found.items()):
        print(f"{unit_name}: {len(rows)}")
        for row in rows:
            addr = f" @0x{row['address']:x}" if row["address"] is not None else ""
            decomp = row["decomp_symbol"] or "?"
            flag = " [phantom 100]" if row["phantom"] else ""
            print(f"  {row['symbol']} -> {decomp}{addr}{flag}")
    print(f"TOTAL {total} unpaired function(s) in {len(found)} unit(s): "
          f"{phantom_total} phantom-100, {noscore_total} no-score")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

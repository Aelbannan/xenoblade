#!/usr/bin/env python3
"""Shrink-only exception-table gate (``run.py extab-gate``).

``.text`` and the data sections were the only things the matching gates
compared, so a source edit that changed a cleanup action (a dtor or
``operator delete`` signature, a base class gaining or losing a non-trivial
dtor) could change ``extab``/``extabindex`` without any gate noticing.

For every completed unit (objdiff ``metadata.complete``: ``Matching`` and
``MatchingCodeOnly``) this compares the decomp object's ``extab`` and
``extabindex`` with the retail object, reloc-aware
(:func:`tools.coop.lib.data_match.check_extab_sections`).  The result is
checked against ``docs/evidence/extab_baseline.json``:

* a unit that fails and is not in the baseline -> FAIL
* a baselined unit whose per-function diff count rises -> FAIL
  (:func:`tools.coop.lib.data_match.extab_function_diffs`)

``--ratchet`` rewrites the baseline, but only by removing units or lowering
counts, and it refuses when anything regressed.  The gate reads existing
objects.  Build first, holding the lock (``tools/pi_harness/build_lock.py us
-- ninja all_source``).

Pure comparison logic lives in module functions, so the tests need no build.
"""
from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BASELINE = ROOT / "docs" / "evidence" / "extab_baseline.json"


def compare(baseline: dict, current: dict) -> tuple[list[str], list[str]]:
    """(regressions, improvements) of *current* against *baseline*.

    Both map unit name -> per-function extab diff count, failing units only.
    A count of 0 means the records agree per function and only their order
    differs (or the strict check failed on something else).
    """
    regress: list[str] = []
    improve: list[str] = []
    for unit in sorted(set(baseline) | set(current)):
        b, c = baseline.get(unit), current.get(unit)
        if b is None:
            regress.append(f"new extab mismatch: {unit} ({c} function diff(s))")
        elif c is None:
            improve.append(f"extab now matches: {unit}")
        elif c > b:
            regress.append(f"extab diffs rose in {unit}: {b} -> {c}")
        elif c < b:
            improve.append(f"extab diffs fell in {unit}: {b} -> {c}")
    return regress, improve


def ratchet(baseline: dict, current: dict) -> dict:
    """Shrink-only update: keep units still failing, at the lower count."""
    return {u: min(n, current[u]) for u, n in baseline.items() if u in current}


def completed_units(root: Path = ROOT) -> list[dict]:
    units = json.loads((root / "objdiff.json").read_text())["units"]
    return [u for u in units if (u.get("metadata") or {}).get("complete")
            and u.get("target_path") and u.get("base_path")]


def scan(root: Path = ROOT, region: str = "us", verbose: bool = False) -> tuple[dict, int, list[str]]:
    """(failing unit -> per-function diff count, units audited, missing objects)."""
    from tools.coop.lib.data_match import check_extab_sections, extab_function_diffs
    from tools.coop.lib.reloc_canon import LinkerLayout

    layout = LinkerLayout.load(root, region)
    failing: dict[str, int] = {}
    missing: list[str] = []
    audited = 0
    for unit in completed_units(root):
        retail, decomp = root / unit["target_path"], root / unit["base_path"]
        if not retail.is_file() or not decomp.is_file():
            missing.append(unit["name"])
            continue
        audited += 1
        if check_extab_sections(retail, decomp, layout=layout).ok:
            continue
        diffs = extab_function_diffs(retail, decomp, layout=layout)
        failing[unit["name"]] = len(diffs)
        if verbose:
            print(f"{unit['name']}: {len(diffs)} function diff(s)")
            for line in diffs:
                print(f"    {line[:220]}")
    return failing, audited, missing


def load_baseline(path: Path = BASELINE) -> dict:
    return json.loads(path.read_text())["units"]


def write_baseline(units: dict, path: Path = BASELINE) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    doc = {
        "_comment": "Shrink-only. Completed units whose extab/extabindex differ from retail "
                    "(value: per-function diff count). See tools/coop/extab_gate.py and "
                    "docs/evidence/extab_audit_2026-10-24.md.",
        "units": dict(sorted(units.items())),
    }
    path.write_text(json.dumps(doc, indent=1) + "\n")


def gate(region: str = "us", do_ratchet: bool = False, init: bool = False,
         verbose: bool = False) -> int:
    current, audited, missing = scan(ROOT, region, verbose)
    print(f"extab-gate: {audited} completed unit(s) audited, {len(current)} mismatch(es)")
    if missing:
        print(f"extab-gate: {len(missing)} unit(s) without a built object (build first): "
              + ", ".join(missing[:5]), file=sys.stderr)
    if init:
        write_baseline(current)
        print(f"extab-gate: wrote baseline {BASELINE.relative_to(ROOT)}")
        return 0
    baseline = load_baseline()
    regress, improve = compare(baseline, current)
    for line in improve:
        print(f"  improved: {line}")
    if regress or missing:
        for line in regress:
            print(f"FAIL: {line}", file=sys.stderr)
        if missing:
            print("FAIL: missing decomp objects", file=sys.stderr)
        print("  (diagnose: run.py extab-gate --verbose, or run.py data diff <unit>)", file=sys.stderr)
        return 1
    if do_ratchet and improve:
        write_baseline(ratchet(baseline, current))
        print(f"extab-gate: ratcheted baseline ({len(improve)} change(s))")
    elif improve:
        print("extab-gate: OK; run with --ratchet to lock in the improvements")
    else:
        print("extab-gate: OK")
    return 0


def baseline_has(unit_name: str, path: Path = BASELINE) -> bool:
    """True when *unit_name* is a recorded pre-existing extab mismatch."""
    try:
        return unit_name in load_baseline(path)
    except (OSError, ValueError, KeyError):
        return False


if __name__ == "__main__":
    import argparse

    sys.path.insert(0, str(ROOT))
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--region", default="us")
    ap.add_argument("--ratchet", action="store_true")
    ap.add_argument("--init", action="store_true")
    ap.add_argument("--verbose", action="store_true")
    a = ap.parse_args()
    sys.exit(gate(a.region, a.ratchet, a.init, a.verbose))

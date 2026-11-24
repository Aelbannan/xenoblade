#!/usr/bin/env python3
"""Checkpoint checks (orchestrator only; no git here).

    .venv/bin/python3 -m tools.coop.land.checkpoint            # build report, compare, run every gate
    .venv/bin/python3 -m tools.coop.land.checkpoint --accept   # after the commit: make this report the baseline

Baseline report: build/us/checkpoints/report_last.json (survives /tmp wipes; build/ is untracked).
Prints one PASS/FAIL line per step and exits non-zero on any FAIL.  A FAIL listed in
KNOWN_FAIL is reported as KNOWN and does not fail the run; keep that list tiny and documented.
"""
from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PY = str(ROOT / ".venv/bin/python3")
REPORT = ROOT / "build/us/report.json"
BASE = ROOT / "build/us/checkpoints/report_last.json"
LOGS = ROOT / "build/us/checkpoints/logs"

# step name -> reason; a FAIL here is expected debt, not a regression
KNOWN_FAIL: dict[str, str] = {
    "faithful": "cp21: approved CtrlPc revert (17->26) and code_804BC9EC.hpp label 1->2",
}

STEPS = [
    ("build", ["./ninja", "-k", "0", "build/us/report.json"]),
    ("link-gate", [PY, "tools/coop/run.py", "link-gate"]),
    ("extab-gate", [PY, "tools/coop/run.py", "extab-gate"]),
    ("text-order-gate", [PY, "tools/coop/run.py", "text-order-gate", "--ratchet"]),
    ("registry", [PY, "tools/coop/registry_check.py"]),
    ("faithful", [PY, "tools/coop/run.py", "faithful", "--ratchet"]),
    ("tests", [PY, "-m", "unittest", "discover", "-s", "tools/coop/tests", "-t", ".", "-q"]),
    ("hazards", [PY, "tools/coop/check_script_hazards.py"]),
]


def _fns(path: Path) -> tuple[dict, dict[str, float]]:
    d = json.loads(path.read_text())
    fns = {}
    for u in d["units"]:
        for f in u.get("functions", []):
            fns[f"{u['name']}::{f['name']}"] = f.get("fuzzy_match_percent", 0.0)
    return d["measures"], fns


def compare(base: Path, new: Path) -> tuple[bool, list[str]]:
    bm, bf = _fns(base)
    nm, nf = _fns(new)
    lost = sorted(k for k, v in bf.items() if v == 100.0 and nf.get(k, 0.0) < 100.0)
    lines = [f"matched_functions {bm.get('matched_functions')} -> {nm.get('matched_functions')}",
             f"fuzzy {bm.get('fuzzy_match_percent', 0):.2f} -> {nm.get('fuzzy_match_percent', 0):.2f}",
             f"matched_data_percent {bm.get('matched_data_percent', 0):.2f} -> {nm.get('matched_data_percent', 0):.2f}"]
    lines += [f"LOST {k} -> {nf.get(k, 'missing')}" for k in lost]
    return not lost, lines


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--accept", action="store_true", help="copy the current report to the baseline and exit")
    a = ap.parse_args(argv)
    if a.accept:
        BASE.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(REPORT, BASE)
        print(f"baseline <- {REPORT}")
        return 0
    LOGS.mkdir(parents=True, exist_ok=True)
    bad = 0
    for name, cmd in STEPS:
        log = LOGS / f"{name}.log"
        with log.open("w") as fh:
            rc = subprocess.run(cmd, cwd=ROOT, stdout=fh, stderr=subprocess.STDOUT).returncode
        tail = (log.read_text().strip().splitlines() or [""])[-1][:150]
        if rc == 0:
            print(f"PASS  {name:<16} {tail}")
        elif name in KNOWN_FAIL:
            print(f"KNOWN {name:<16} {tail}   [{KNOWN_FAIL[name]}]  log {log}")
        else:
            bad += 1
            print(f"FAIL  {name:<16} rc={rc} {tail}  log {log}")
        if name == "build" and rc != 0:
            break
    if BASE.is_file() and REPORT.is_file():
        ok, lines = compare(BASE, REPORT)
        bad += not ok
        print(("PASS" if ok else "FAIL") + "  report-compare   vs " + str(BASE.relative_to(ROOT)))
        for l in lines:
            print("      " + l)
    else:
        print("NOTE  report-compare   no baseline yet (run --accept after this checkpoint commits)")
    od = (ROOT / "objdiff.json").read_text()
    if "/tmp/xb" in od:
        bad += 1
        print("FAIL  objdiff.json      contains /tmp/xb paths")
    print("CHECKPOINT " + ("OK" if not bad else f"FAILED ({bad})"))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""Build every FULL_MATCH unit through hexdiff and revalidate every FULL row.

No score, absent symbol or command failure can silently preserve FULL_MATCH.
Run without --apply to inspect; --output retains the entire audit evidence.
"""
from __future__ import annotations
import argparse
import contextlib
import io
import json
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.coop.hexdiff import run as hexdiff
from tools.coop.lib.config import load_config
from tools.coop.lib.targets import load_targets_document, locked_targets_document


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    config = load_config(None, ROOT)
    groups = defaultdict(list)
    for row in load_targets_document(config)["targets"]:
        if row.get("status") == "FULL_MATCH":
            groups[row["unit"]].append(row)
    audit = {"criterion": "equal size and instruction bytes after proven relocation writes",
             "units": len(groups), "targets": sum(map(len, groups.values())), "results": []}
    for index, (unit, targets) in enumerate(sorted(groups.items()), 1):
        stdout, stderr = io.StringIO(), io.StringIO()
        try:
            with contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr):
                rc = hexdiff([unit, "--all", "--json"])
            report = json.loads(stdout.getvalue()) if rc in (0, 5) else {}
            functions = report.get("functions", [])
            by_name = {f["symbol"]: f for f in functions}
            for target in targets:
                f = by_name.get(target["symbol"])
                if f is None:
                    # Registry short names may differ from the recovered retail
                    # name. Resolve through the same unique symbol resolver.
                    from tools.coop.lib.full_match import check_unit_function
                    from tools.coop.lib.project import Project
                    project = Project(config)
                    try:
                        comparison = check_unit_function(project, project.resolve_unit(unit), target["symbol"])
                        from tools.coop.hexdiff import _counts_for
                        c = _counts_for(comparison.retail, comparison.decomp)
                        f = dict(c, match=comparison.match, present=True,
                                 retail_size=comparison.retail.size, decomp_size=comparison.decomp.size,
                                 reloc=len(comparison.unresolved), relocation_errors=list(comparison.reasons))
                    except (OSError, RuntimeError, ValueError) as exc:
                        f = {"present": False, "error": str(exc)}
                result = {"id": target["id"], "symbol": target["symbol"], "unit": unit, **f}
                if not f.get("present", True):
                    status, pct = "COMPILES", 0.0
                    result["classification"] = "unproven: missing/ambiguous function pair or build failure"
                elif f.get("match"):
                    status, pct = "FULL_MATCH", 100.0
                    result["classification"] = "proven byte match"
                else:
                    total = max(f.get("retail_size", 0), f.get("decomp_size", 0)) // 4
                    pct = 100.0 * (total - f.get("mismatch", total)) / (total or 1)
                    status = "CODE_MATCH" if pct >= 95 else "HIGH_MATCH" if pct >= 70 else "STRUCTURAL" if pct > 0 else "COMPILES"
                    result["classification"] = ("byte difference" if f.get("mismatch") else
                        "wrong-address relocation" if any("wrong-address" in e for e in f.get("relocation_errors", [])) else "unproven relocation")
                result["status"], result["instruction_match"] = status, round(pct, 3)
                audit["results"].append(result)
        except Exception as exc:
            # Persist failures as explicit demotions, not skipped rows.
            for target in targets:
                audit["results"].append({"id": target["id"], "symbol": target["symbol"],
                    "unit": unit, "status": "COMPILES", "instruction_match": 0.0,
                    "classification": "unproven: audit/build error", "error": str(exc),
                    "stderr": stderr.getvalue()[-2000:]})
        if index % 25 == 0 or index == len(groups):
            print(f"{index}/{len(groups)} units; {sum(r['status'] != 'FULL_MATCH' for r in audit['results'])} demotions", flush=True)
            args.output.write_text(json.dumps(audit, indent=2) + "\n")
    if args.apply:
        with locked_targets_document(config) as (document, write):
            by_id = {r["id"]: r for r in document["targets"]}
            for result in audit["results"]:
                row = by_id[result["id"]]
                # A different agent may have cycled the target during this run.
                if row.get("status") != "FULL_MATCH":
                    continue
                row["status"] = result["status"]
                row["instruction_match"] = result["instruction_match"]
                row["full_match_revalidation"] = {"date": "2026-11-17", "classification": result["classification"]}
                if result["status"] != "FULL_MATCH":
                    row["workflow_status"] = "ACTIVE"
                    for key in ("equivalence_certificate", "equivalence_status", "equivalence_confidence", "equivalence_policy"):
                        row.pop(key, None)
            write()
    args.output.write_text(json.dumps(audit, indent=2) + "\n")
    print(f"Validated {audit['targets']} targets; {sum(r['status'] != 'FULL_MATCH' for r in audit['results'])} demotions")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())

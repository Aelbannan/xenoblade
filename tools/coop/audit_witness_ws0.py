"""Witness WS0 population audit (witness_expansion_plan §4-WS3.7).

Reproducible, committed population measurement for the register-renaming
witness expansion.  Computes, from the registry + the real witness gates:

  - near-match non-indirect targets (status in CODE/HIGH/STRUCTURAL,
    instruction_match >= 95, has_indirect_calls = false);
  - per-target witness gate outcome via ``check_gates`` (gates 1-6);
  - callee-clean classification via the REAL gate ``_load_certified_callees``
    (transitive re-attestation + self-recursion carveout — exactly what
    ``_try_renaming_witness`` checks before the witness runs);
  - first-cut loop predicate via ``_has_loop_or_non_return_indirect`` (pinned
    as committed code + fixture, rev-4 finding 8: two reviewers disagreed
    21-vs-19 on the same targets by eyeball — this code is the authority);
  - per-target ``witness_failure_gate`` bucket (rho / loop / indirect-branch /
    callee-blocked / extract-error / PASS).

All counts are a run-time snapshot: the registry is live and targets drift
between runs.  Never cite a fixed number from this plan — re-run this tool.

Usage:
  .venv/bin/python3 tools/coop/audit_witness_ws0.py [--json OUT]
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))

from tools.coop.lib.config import load_config
from tools.coop.lib import equivalence_check as ec
from tools.coop.lib.equivalence_check import _canonical_symbols_for_unit, _load_certified_callees
from tools.coop.lib.project import Project
from tools.coop.lib.renaming_witness import check_gates, _has_loop_or_non_return_indirect
from tools.coop.lib.targets import load_targets

ACCEPTED = {"FULL_MATCH", "EQUIVALENT_MATCH"}
NEAR_MATCH_STATUSES = {"CODE_MATCH", "HIGH_MATCH", "STRUCTURAL"}


def audit(config_path: str | None, project_root: Path) -> dict:
    config = load_config(config_path, project_root)
    project = Project(config)
    targets = load_targets(config)

    near: list[dict] = []
    for t in targets:
        if t.status in ACCEPTED or t.status not in NEAR_MATCH_STATUSES:
            continue
        if float(t.extra.get("instruction_match") or 0) < 95:
            continue
        if t.extra.get("has_indirect_calls"):
            continue
        near.append(
            {
                "id": t.id, "status": t.status,
                "match": t.extra.get("instruction_match"),
                "eq": t.extra.get("equivalence_status"),
                "tier": t.tier,
                "gate": None, "loop": None, "callee_clean": None,
                "callee_errors": [],
            }
        )

    for info in near:
        t = next(x for x in targets if x.id == info["id"])
        try:
            unit = project.resolve_unit(t.unit)
            if not unit.base_path or not unit.target_path:
                info["gate"] = "extract-error:unit-missing"
                continue
            canonical = _canonical_symbols_for_unit(getattr(unit, "name", None)) or {}
            left, right = ec.extract_function_pair(
                unit.target_path, unit.base_path, t.symbol,
            )
            orig = ec.decode_block(
                left.code, left.base, validate_with_capstone=False,
                relocations=left.relocations, local_symbol=left.name,
                canonical_symbols=canonical,
            )
            cand = ec.decode_block(
                right.code, right.base, validate_with_capstone=False,
                relocations=right.relocations, local_symbol=right.name,
                canonical_symbols=canonical,
            )
            out = check_gates(orig, cand)
            info["gate"] = "PASS" if out.certified else getattr(out.failure, "gate", "?")
            # First-cut loop predicate (pinned code, not eyeball disasm).
            info["loop"] = (
                _has_loop_or_non_return_indirect(orig, local_symbol=left.name)
                or _has_loop_or_non_return_indirect(cand, local_symbol=right.name)
            )
        except Exception as exc:  # extraction / decode failures
            info["gate"] = f"extract-error:{type(exc).__name__}"
            continue
        # Real callee gate (transitive re-attestation + self-recursion
        # carveout), exactly as _try_renaming_witness checks it.
        try:
            ctx = _load_certified_callees(project, t.id)
            info["callee_errors"] = list(ctx.errors)
            info["callee_clean"] = not ctx.errors
        except Exception as exc:
            info["callee_errors"] = [f"load failed: {type(exc).__name__}: {exc}"]
            info["callee_clean"] = False

    rho_fail = [i for i in near if i["gate"] == "rho"]
    clean = [i for i in rho_fail if i["callee_clean"]]
    blocked = [i for i in rho_fail if not i["callee_clean"]]
    not_equiv = [i for i in clean if i["eq"] == "not_equivalent"]
    loop_free = [i for i in clean if not i["loop"]]
    candidate_pool = [i for i in loop_free if i["eq"] != "not_equivalent"]

    summary = {
        "snapshot": "run-time; registry is live — counts drift between runs",
        "filters": (
            "status in CODE/HIGH/STRUCTURAL; instruction_match >= 95; "
            "has_indirect_calls = false"
        ),
        "near_match_count": len(near),
        "rho_fail": len(rho_fail),
        "callee_clean": len(clean),
        "callee_blocked": len(blocked),
        "not_equivalent_among_clean": len(not_equiv),
        "loop_free_among_clean": len(loop_free),
        "candidate_pool_first_cut": len(candidate_pool),
        "candidate_pool_ids": sorted(i["id"] for i in candidate_pool),
        "per_target": {
            i["id"]: {
                "status": i["status"], "match": i["match"], "eq": i["eq"],
                "tier": i["tier"], "gate": i["gate"], "loop": i["loop"],
                "callee_clean": i["callee_clean"],
            }
            for i in near
        },
    }
    return summary


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", metavar="OUT", help="write JSON summary to OUT")
    parser.add_argument("--config", help="coop config path (default ./coop.json)")
    args = parser.parse_args(argv)
    root = Path(__file__).resolve().parents[2]
    summary = audit(args.config, root)
    text = (
        f"near-match non-indirect: {summary['near_match_count']}\n"
        f"  rho-fail:              {summary['rho_fail']}\n"
        f"    callee-clean:        {summary['callee_clean']}\n"
        f"    callee-blocked:      {summary['callee_blocked']}\n"
        f"    not_equivalent:      {summary['not_equivalent_among_clean']}\n"
        f"    loop-free:           {summary['loop_free_among_clean']}\n"
        f"  candidate pool (first cut): {summary['candidate_pool_first_cut']}\n"
        f"  ids: {', '.join(summary['candidate_pool_ids'])}\n"
        f"(snapshot — registry is live; re-run for current counts)"
    )
    print(text)
    if args.json:
        Path(args.json).write_text(
            json.dumps(summary, indent=1), encoding="utf-8",
        )
        print(f"wrote {args.json}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

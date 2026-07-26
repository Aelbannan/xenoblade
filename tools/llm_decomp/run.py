"""llm_decomp CLI.

Usage:
    python3 tools/llm_decomp/run.py reconcile          # restore orphaned sessions
    python3 tools/llm_decomp/run.py solve <target-id> [--dry-run] [--type match]
    python3 tools/llm_decomp/run.py show-config

Design: docs/llm_decomp_design.md
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from tools.llm_decomp import state  # noqa: E402
from tools.llm_decomp.config import load_config  # noqa: E402
from tools.llm_decomp.policies import get_policy, resolve_writable  # noqa: E402
from tools.llm_decomp.session import Session  # noqa: E402


def _load_target(target_id: str):
    from tools.coop.lib.config import load_config as load_coop_config
    from tools.coop.lib.targets import get_target, load_targets
    coop_cfg = load_coop_config(None, REPO_ROOT)
    return coop_cfg, get_target(load_targets(coop_cfg), target_id)


def _retail_asm(region: str, unit: str, symbol: str) -> str:
    from tools.llm_decomp.asm_listings import assembly_function_block
    asm_path = REPO_ROOT / "build" / region / "asm" / f"{unit}.s"
    if not asm_path.exists():
        return f"(retail asm listing not found: {asm_path})"
    block = assembly_function_block(
        asm_path.read_text(encoding="utf-8", errors="replace"), symbol)
    return block or f"(symbol {symbol} not found in {asm_path})"


def _unit_paths(unit: str) -> tuple[str | None, str | None]:
    """Best-effort source/header for a unit like 'kyoshin/cf/CfPadTask'.

    Source is None when the TU file does not exist on disk (target needs
    scaffolding before it is session-ready). The designated header path is
    returned even when absent — sessions may create it.
    """
    for ext in (".cpp", ".c"):
        src = f"src/{unit}{ext}"
        if (REPO_ROOT / src).exists():
            return src, f"include/{unit}.hpp"
    return None, f"include/{unit}.hpp"


def cmd_reconcile(_args) -> int:
    report = state.reconcile(REPO_ROOT)
    if not report:
        print("reconcile: no orphaned sessions.")
        return 0
    for entry in report:
        print(f"aborted {entry['session_id']} ({entry['target_id']}): "
              f"restored {len(entry['restored'])} file(s)")
    return 0


def cmd_solve(args) -> int:
    from tools.llm_decomp.providers import get_provider

    cfg = load_config(REPO_ROOT)
    _coop_cfg, target = _load_target(args.target_id)
    if not target.buildable:
        print(f"target {args.target_id} is not buildable "
              f"(source={target.source}, unit={target.unit})")
        return 1

    policy = get_policy(args.type)
    unit = target.unit or ""
    symbol = target.symbol or ""
    if target.source and Path(target.source).exists():
        src = str(Path(target.source).relative_to(REPO_ROOT))
    else:
        src, _ = _unit_paths(unit)
    header = f"include/{unit}.hpp"
    if src is None:
        print(f"target {args.target_id}: unit source for {unit} does "
              "not exist — scaffold the TU before running sessions on it.")
        return 1
    writable = resolve_writable(policy, src, header)

    retail_asm = _retail_asm(_coop_cfg.region, unit, symbol)
    signature = target.function  # demangled signature from the registry

    if args.dry_run:
        print(json.dumps({
            "target_id": target.id, "unit": unit, "symbol": symbol,
            "session_type": policy.name, "writable": writable,
            "model": cfg.model_for(policy.name).model,
            "budgets": cfg.budgets_for(policy.name),
            "retail_asm_chars": len(retail_asm),
        }, indent=2))
        return 0

    provider = get_provider(cfg.model_for(policy.name).provider)

    from tools.coop.lib.targets import claim_target, release_target
    claim_target(_coop_cfg, target.id, owner=cfg.auto_promote_owner,
                 allowed_paths=writable)
    meta, sdir = state.init_session(
        REPO_ROOT, target.id, policy.name, unit, symbol, writable,
        cfg.auto_promote_owner)
    try:
        session = Session(
            REPO_ROOT, cfg, policy, meta, sdir, provider,
            demangled=target.function, signature=signature,
            retail_asm=retail_asm, target_id=target.id)
        outcome = session.run()
        if outcome.accepted and cfg.auto_promote and \
                session._verdict is not None:
            from tools.llm_decomp.promote import PromotionQueue
            summary = PromotionQueue(REPO_ROOT, cfg).promote_accepted(
                meta, session._verdict)
            print(json.dumps({"promoted": summary}, indent=2))
    except Exception:
        # Crash safety: restore writable files and abort the session so a
        # later `reconcile` is a no-op (design §6, review #1).
        from tools.llm_decomp import paths as _paths
        from tools.llm_decomp.contracts import SessionStatus as _SS
        for rel in meta.writable:
            snap = _paths.init_snapshot_dir(sdir) / rel
            if snap.exists():
                import shutil
                shutil.copy2(snap, REPO_ROOT / rel)
        state.set_status(sdir, meta, _SS.ABORTED)
        raise
    finally:
        release_target(_coop_cfg, target.id, owner=cfg.auto_promote_owner)
    print(json.dumps(outcome.__dict__, indent=2))
    return 0 if outcome.accepted else 1


def cmd_probe(args) -> int:
    from tools.llm_decomp.probe import eligible_targets, probe_targets
    from dataclasses import asdict

    if args.target_ids:
        from tools.coop.lib.config import load_config as load_coop_config
        from tools.coop.lib.targets import get_target, load_targets
        coop_cfg = load_coop_config(None, REPO_ROOT)
        all_t = load_targets(coop_cfg)
        targets = [get_target(all_t, tid) for tid in args.target_ids]
    else:
        number = args.number or None
        targets = eligible_targets(REPO_ROOT, number=number, tu=args.tu)
    if not targets:
        print("probe: no eligible targets")
        return 1
    rows = probe_targets(REPO_ROOT, targets, min_fuzzy=args.min_fuzzy,
                         write=args.write and not args.dry_run)
    accepted = sum(1 for r in rows if r.accepted)
    print(json.dumps({"total": len(rows), "accepted": accepted,
                      "rows": [asdict(r) for r in rows]}, indent=2))
    return 0


def cmd_pipeline(args) -> int:
    from tools.llm_decomp.orchestrator import (Orchestrator, results_to_json,
                                               select_tus)
    cfg = load_config(REPO_ROOT)
    if args.tu:
        units = list(args.tu)
    elif args.number:
        units = select_tus(REPO_ROOT, cfg, args.number)
    else:
        print("pipeline: pass --tu <unit> or --number N")
        return 1
    if args.dry_run:
        print(json.dumps({"units": units}, indent=2))
        return 0
    results = Orchestrator(REPO_ROOT, cfg).run(units)
    print(results_to_json(results))
    return 0 if all(r.accepted or r.skipped for r in results) else 1


def cmd_show_config(_args) -> int:
    cfg = load_config(REPO_ROOT)
    print(json.dumps({k: v for k, v in cfg.__dict__.items()
                      if k != "session_types"}, indent=2, default=str))
    for name, st in cfg.session_types.items():
        print(f"\n[{name}]")
        print(json.dumps({"model": st.model.__dict__,
                          "budgets": st.budgets}, indent=2, default=str))
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("reconcile", help="restore orphaned sessions")
    p.set_defaults(fn=cmd_reconcile)

    p = sub.add_parser("solve", help="run one conversational session")
    p.add_argument("target_id")
    p.add_argument("--type", default="match",
                   choices=["match", "type-recovery", "rename",
                            "tu-cleanup", "size-trim"])
    p.add_argument("--dry-run", action="store_true")
    p.set_defaults(fn=cmd_solve)

    p = sub.add_parser("show-config", help="print effective config")
    p.set_defaults(fn=cmd_show_config)

    p = sub.add_parser("probe", help="no-LLM objdiff + SMT equivalence probe")
    p.add_argument("target_ids", nargs="*")
    p.add_argument("--number", type=int, default=0)
    p.add_argument("--tu", help="probe all eligible targets of one unit")
    p.add_argument("--min-fuzzy", type=float, default=50.0)
    p.add_argument("--write", action="store_true",
                   help="persist accepted FULL/EQUIVALENT into targets.json")
    p.add_argument("--dry-run", action="store_true",
                   help="run probes but never persist")
    p.set_defaults(fn=cmd_probe)

    p = sub.add_parser("pipeline", help="run the session pipeline on TUs")
    p.add_argument("--tu", nargs="+",
                   help="one or more units, e.g. kyoshin/cf/CfPadTask")
    p.add_argument("--number", type=int, default=0,
                   help="auto-select N TUs (disjoint, parallel)")
    p.add_argument("--dry-run", action="store_true")
    p.set_defaults(fn=cmd_pipeline)

    args = parser.parse_args(argv)
    return args.fn(args)


if __name__ == "__main__":
    sys.exit(main())

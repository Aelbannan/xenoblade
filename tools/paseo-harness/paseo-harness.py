#!/usr/bin/env python3
"""paseo-harness — generic section-based batch matching harness for paseo.

Each batch runs in its own paseo-managed git worktree (isolated branch),
with cost controls: tiered thinking, provider-epidemic backoff, relaunch
budgets with report seeding, an agent-hour budget gate, and conflict-free
merge-back of the shared registries via custom 3-way merge drivers.

Usage (always via the venv python):
    .venv/bin/python3 tools/paseo-harness/paseo-harness.py plan  --section criware
    .venv/bin/python3 tools/paseo-harness/paseo-harness.py run   --section criware
    .venv/bin/python3 tools/paseo-harness/paseo-harness.py report --section criware
    .venv/bin/python3 tools/paseo-harness/paseo-harness.py merge  --section criware --batch criware-01
    .venv/bin/python3 tools/paseo-harness/paseo-harness.py cleanup --section criware

Sections match the unit prefix case-insensitively: criware -> "CriWare/...",
kyoshin -> "kyoshin/...", rvl_sdk -> "RVL_SDK/...", monolib, nw4r, ...
"""
import argparse
import json
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from lib import gitutil, paseo  # noqa: E402
from lib.common import ROOT, VENV_PY, now_iso, slug, state_dir  # noqa: E402
from lib.planner import plan as plan_fn  # noqa: E402


def cmd_plan(args):
    out = args.state_dir / "plan.json"
    plan_fn(
        args.section,
        batch_size=args.batch_size,
        tu_threshold=args.tu_threshold,
        hard_batch_size=args.hard_batch_size,
        thinking=args.thinking,
        hard_model=args.hard_model,
        kb_digest=not args.no_kb_digest,
        digests_dir=args.state_dir / "digests" if not args.dry_run else None,
        out_path=None if args.dry_run else out,
        dry_run=args.dry_run,
    )


def cmd_preflight(args):
    """Cheap, script-side pre-flight steps that make the ready frontier and
    hexdiff hints accurate. recertify is opt-in (it can run certifiers)."""
    steps = [("targets sync-calls", ["tools/coop/run.py", "targets", "sync-calls"]),
             ("reloc-map mine", ["tools/coop/reloc_map.py", "mine"])]
    if args.recertify:
        steps.append(("targets recertify --bottom-up",
                      ["tools/coop/run.py", "targets", "recertify", "--bottom-up"]))
    for name, cmd in steps:
        print(f"== {name}")
        if args.dry_run:
            continue
        r = subprocess.run([str(VENV_PY)] + cmd, cwd=str(ROOT), timeout=7200)
        print(f"   rc={r.returncode}")


def cmd_run(args):
    from lib.monitor import Monitor

    plan_path = args.plan or (args.state_dir / "plan.json")
    if not Path(plan_path).exists():
        sys.exit(f"no plan at {plan_path} — run `paseo-harness.py plan --section {args.section}` first")
    plan = json.load(open(plan_path))
    if plan.get("section") != args.section:
        print(f"warning: plan section={plan.get('section')} != --section {args.section}", file=sys.stderr)

    m = Monitor(args.section, plan, args)

    if args.dry_run:
        print("dry-run: would launch batches in this order (first up to --running):")
        pending = sorted(
            [b for b in plan["batches"] if b["batch_id"] not in m.st["batches"]],
            key=lambda b: b.get("launch_rank", 999),
        )
        for b in pending[: args.running]:
            branch = f"paseo/{m.secslug}/{b['batch_id']}"
            mode = "checkout-branch" if gitutil.branch_exists(branch) else "branch-off"
            print(f"  {b['batch_id']:<12} thinking={m._thinking_for(b, 1):<7} "
                  f"{b['total']:3d} fns ({b['leaf_count']} leaf) branch={branch} [{mode}]")
        print(f"state dir: {args.state_dir}")
        return

    m.run()


def cmd_report(args):
    from lib.monitor import Monitor

    plan_path = args.plan or (args.state_dir / "plan.json")
    plan = json.load(open(plan_path))
    m = Monitor(args.section, plan, args)
    m.report()


def cmd_merge(args):
    from lib.monitor import Monitor

    plan_path = args.plan or (args.state_dir / "plan.json")
    plan = json.load(open(plan_path))
    m = Monitor(args.section, plan, args)
    rec = m.st["batches"].get(args.batch)
    if not rec:
        sys.exit(f"batch {args.batch} not tracked in {m.state_path}")
    status, detail = gitutil.merge_batch(rec["branch"], f"paseo-harness: merge {args.batch}")
    print(f"{args.batch}: merge -> {status}")
    if detail:
        print(detail[-800:])
    if status == "merged":
        rec["status"] = "done"
        rec["merged_at"] = now_iso()
        if rec.get("workspace_id"):
            paseo.archive_workspace(rec["workspace_id"])
        if args.batch not in m.st["done"]:
            m.st["done"].append(args.batch)
        m._save()
        print(f"{args.batch}: merged and archived")


def cmd_cleanup(args):
    secslug = slug(args.section)
    if args.archive_agents:
        agents = paseo.ls_agents(global_=True, labels=[f"paseo-harness={secslug}"])
        print(f"{len(agents)} harness agent(s) for section {args.section}")
        for a in agents:
            aid = a.get("id")
            st = (a.get("status") or "").lower()
            if args.force or st in ("idle", "completed", "finished", "error"):
                ok = paseo.archive_agent(aid)
                print(f"  archive {aid[:8]} [{st}] -> {'ok' if ok else 'FAILED'}")
            else:
                print(f"  skip {aid[:8]} [{st}] (running; use --force to interrupt)")
    else:
        agents = paseo.ls_agents(global_=True, labels=[f"paseo-harness={secslug}"])
        print(f"{len(agents)} harness agent(s) for section {args.section} (use --archive-agents to archive)")

    # worktree workspaces for this section (name starts with the section slug)
    ws = [w for w in paseo.list_workspaces() if (w.get("name") or "").startswith(secslug)]
    print(f"{len(ws)} harness workspace(s) for section {args.section}")
    for w in ws:
        wid = w.get("workspaceId")
        if args.archive_agents:
            ok = paseo.archive_workspace(wid)
            print(f"  archive {wid} ({w.get('cwd')}) -> {'ok' if ok else 'FAILED'}")
        else:
            print(f"  {wid} ({w.get('cwd')})")

    # branches
    r = gitutil._git(["branch", "--list", f"paseo/{secslug}/*"])
    branches = [l.strip() for l in r.stdout.splitlines() if l.strip()]
    print(f"{len(branches)} branch(es) paseo/{secslug}/*")
    for b in branches:
        if args.delete_branches:
            if args.force:
                gitutil.force_delete_branch(b)
            else:
                gitutil.delete_branch(b)  # -d: only if merged
            print(f"  delete {b}")
        else:
            print(f"  {b}")


def main():
    ap = argparse.ArgumentParser(prog="paseo-harness", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--state-dir", type=Path, default=None,
                    help="override state dir (default ~/.paseo/harness/<section>)")
    sub = ap.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("plan", help="build a batch plan for a section")
    p.add_argument("--section", required=True)
    p.add_argument("--state-dir", type=Path, default=None, help=argparse.SUPPRESS)
    p.add_argument("--batch-size", type=int, default=30)
    p.add_argument("--tu-threshold", type=int, default=20)
    p.add_argument("--thinking", choices=["medium", "high", "max"], default=None,
                   help="force a thinking tier for all batches (default: per-batch auto)")
    p.add_argument("--hard-batch-size", type=int, default=14,
                   help="pooled hard (majority non-leaf) batches flush at this size")
    p.add_argument("--hard-model", default=None,
                   help="model assigned to max-thinking (hard) batches in the plan")
    p.add_argument("--no-kb-digest", action="store_true",
                   help="skip per-batch MWCC_CASES KB digest generation")
    p.add_argument("--dry-run", action="store_true")
    p.set_defaults(fn=cmd_plan)

    p = sub.add_parser("preflight", help="sync-calls + reloc-map mine (ready-frontier freshness)")
    p.add_argument("--section", required=True, help="unused; kept for consistency")
    p.add_argument("--state-dir", type=Path, default=None, help=argparse.SUPPRESS)
    p.add_argument("--recertify", action="store_true",
                   help="also run targets recertify --bottom-up (may take a while)")
    p.add_argument("--dry-run", action="store_true")
    p.set_defaults(fn=cmd_preflight)

    p = sub.add_parser("run", help="run the monitor loop for a section")
    p.add_argument("--section", required=True)
    p.add_argument("--state-dir", type=Path, default=None, help=argparse.SUPPRESS)
    p.add_argument("--plan", type=Path, default=None)
    p.add_argument("--running", type=int, default=6, help="max concurrent batch agents (default 6)")
    p.add_argument("--interval", type=int, default=180, help="sweep interval seconds")
    p.add_argument("--max-idle-min", type=int, default=60, help="hang threshold minutes")
    p.add_argument("--max-relaunch", type=int, default=3, help="per-batch relaunch budget")
    p.add_argument("--backoff-threshold", type=int, default=3,
                   help="errors/hangs per sweep that trigger launch backoff")
    p.add_argument("--backoff-sweeps", type=int, default=2,
                   help="sweeps to pause launches after an error epidemic")
    p.add_argument("--max-agent-hours", type=float, default=0.0,
                   help="stop launching new batches after this many cumulative agent-hours (0=unlimited)")
    p.add_argument("--model", default=None, help="model for batch agents")
    p.add_argument("--fallback-model", default=None,
                   help="model used for relaunch attempt >= 2")
    p.add_argument("--thinking", choices=["medium", "high", "max"], default=None,
                   help="force a thinking tier for all batches (default: per-batch from plan)")
    p.add_argument("--allow-smt", action="store_true",
                   help="allow in-session cycle --smt for stuck targets above 90 percent (off by default)")
    p.add_argument("--base", default="main", help="base branch for worktrees/merges")
    p.add_argument("--prewarm", action="store_true",
                   help="configure + build each batch's unit objects in its worktree before launch")
    p.add_argument("--once", action="store_true", help="single sweep pass")
    p.add_argument("--dry-run", action="store_true", help="print what would launch, change nothing")
    p.set_defaults(fn=cmd_run)

    p = sub.add_parser("report", help="show batch state for a section")
    p.add_argument("--section", required=True)
    p.add_argument("--state-dir", type=Path, default=None, help=argparse.SUPPRESS)
    p.add_argument("--plan", type=Path, default=None)
    p.set_defaults(fn=cmd_report)

    p = sub.add_parser("merge", help="merge one batch branch back into main")
    p.add_argument("--section", required=True)
    p.add_argument("--state-dir", type=Path, default=None, help=argparse.SUPPRESS)
    p.add_argument("--batch", required=True)
    p.add_argument("--plan", type=Path, default=None)
    p.set_defaults(fn=cmd_merge)

    p = sub.add_parser("cleanup", help="archive agents/workspaces/branches for a section")
    p.add_argument("--section", required=True)
    p.add_argument("--state-dir", type=Path, default=None, help=argparse.SUPPRESS)
    p.add_argument("--archive-agents", action="store_true", help="archive agents and worktree workspaces")
    p.add_argument("--delete-branches", action="store_true", help="delete paseo/<section>/* branches")
    p.add_argument("--force", action="store_true", help="interrupt running agents / force-delete branches")
    p.set_defaults(fn=cmd_cleanup)

    args = ap.parse_args()
    if args.state_dir is None:
        args.state_dir = state_dir(args.section)
    args.state_dir.mkdir(parents=True, exist_ok=True)
    args.fn(args)


if __name__ == "__main__":
    main()

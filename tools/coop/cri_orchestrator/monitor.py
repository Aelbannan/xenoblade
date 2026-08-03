#!/usr/bin/env python3
"""CriWare paseo batch babysitter.

Keeps ~N paseo agents running over the batch queue in plan.json. Launches a
new batch whenever an agent finishes, errors, or hangs (> max-idle with no
activity). Writes per-batch completion reports (agent log tail) to reports/.

Usage:
  monitor.py                          # run the loop (defaults below)
  monitor.py --report                 # print current state once, no launching
  monitor.py --once                   # one pass (launch up to --running, no loop)
"""
import argparse
import json
import os
import re
import subprocess
import sys
import time
from datetime import datetime, timezone

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
PASEO = os.environ.get("PASEO", "/Applications/Paseo.app/Contents/Resources/bin/paseo")
PLAN = os.path.join(HERE, "plan.json")
STATE = os.path.join(HERE, "state.json")
REPORTS = os.path.join(HERE, "reports")
LOG = os.path.join(HERE, "monitor.log")

MODEL = "openrouter/deepseek/deepseek-v4-flash-0731"
THINKING = "max"
TITLE_PREFIX = "CRI-MATCH-"

MAX_RELAUNCH = 3


def log(msg):
    line = f"[{datetime.now(timezone.utc).isoformat(timespec='seconds')}] {msg}"
    print(line, flush=True)
    with open(LOG, "a") as f:
        f.write(line + "\n")


def sh(args, **kw):
    return subprocess.run([PASEO] + args, capture_output=True, text=True, **kw)


def load_state():
    if os.path.exists(STATE):
        return json.load(open(STATE))
    return {"batches": {}, "launch_order": [], "done": []}


def save_state(st):
    tmp = STATE + ".tmp"
    json.dump(st, open(tmp, "w"), indent=1)
    os.replace(tmp, STATE)


def paseo_ls():
    r = sh(["ls", "--json"])
    if r.returncode != 0:
        log(f"paseo ls failed: {r.stderr.strip()}")
        return []
    return json.loads(r.stdout)


def paseo_inspect(agent_id):
    r = sh(["inspect", agent_id, "--json"])
    if r.returncode != 0:
        return None
    try:
        return json.loads(r.stdout)
    except Exception:
        return None


def paseo_archive(agent_id):
    r = sh(["archive", agent_id, "--force"])
    return r.returncode == 0


def parse_launch(stdout):
    m = re.search(r"([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})", stdout)
    if m:
        return m.group(1)
    m = re.search(r"\b([0-9a-f]{7,8})\b", stdout)
    return m.group(1) if m else None


def launch_batch(batch, agent_name):
    prompt = build_prompt(batch)
    title = f"{TITLE_PREFIX}{batch['batch_id']}: {batch['label']} ({batch['total']} fns)"
    cmd = [
        "run",
        "--background",
        "--provider", "pi",
        "--model", MODEL,
        "--thinking", THINKING,
        "--cwd", ROOT,
        "--title", title,
        prompt,
    ]
    r = sh(cmd)
    if r.returncode != 0:
        log(f"launch {batch['batch_id']} FAILED: {r.stderr.strip()[:300]}")
        return None
    aid = parse_launch(r.stdout)
    log(f"launched {batch['batch_id']} as agent {aid} ({title})")
    return aid


def build_prompt(batch):
    rows = []
    for td in batch["target_details"]:
        leaf = "leaf" if td.get("leaf") else "    "
        rows.append(
            f"  {td['id']}  {leaf}  {td['function']}  [{td['unit'].split('/')[-1]}]  "
            f"cur={td.get('status')} {td.get('match_percent') or '-'}% size={td.get('size')}"
        )
    table = "\n".join(rows)
    units = ", ".join(u.split("/")[-1] for u in batch["units"])
    return f"""You are a Xenoblade Chronicles Wii decompilation matching agent in the co-op fork at {ROOT}. Match and ACCEPT every target in batch {batch['batch_id']} below ({batch['total']} targets, {batch['leaf_count']} leaf). Work leaf targets first (bottom-up): a target whose callees are not yet accepted is a blocker to record, not a target to cycle.

READ FIRST (in this order):
1. AGENTS.md and .agents/skills/xenoblade-decomp/SKILL.md — operational workflow and acceptance policy (follow it).
2. PLAN.md sections 2, 3, 17 — legal boundaries, architecture invariants, matching policy.
3. docs/MWCC_REFERENCE.md — before editing source for a mismatch, search the KB: .venv/bin/python3 tools/mwcc_kb.py search "<mangled-symbol>" --json then one short query per observed mismatch category.
4. Per target: .venv/bin/python3 tools/coop/run.py targets show <target-id> (callee readiness gate).

YOUR BATCH {batch['batch_id']} — {batch['total']} targets across units: {units}
{table}

RULES (non-negotiable):
- Always use .venv/bin/python3 (venv has Python 3.13.6), never system python3.
- Do NOT use the pi harness (tools/pi_harness).
- Do NOT git reset — other agents are working on this branch right now. Only modify YOUR unit's source files, shared headers, configure.py Object(...) for YOUR units (documented flag fixes only), and docs/evidence/decomp/attempts.jsonl. Commit only your own work (git add your files, git commit). Never push, never revert/cleanup other agents' files.
- Do NOT use subagents — do the work yourself.
- Do NOT ask questions — keep working until the whole batch is done or every remaining target is blocked.
- Cycle and commit targets once accepted.
- Do NOT investigate other compilers — MWCC (GC/3.0a5.2) is the correct compiler; every target is reproducible in high-level C/C++.
- SMT probes are OUT-OF-BAND — you must NOT run `cycle --smt`, `run.py diff`, `equivalence check-unit/check-objects`, inline python against tools/ppc_equivalence, or any `--contract` variant in-session. Your acceptance path is `cycle` WITHOUT --smt (FULL_MATCH or witness-certified EQUIVALENT_MATCH). An orchestrator runs the full Z3 probes out-of-band after your batch reports.
- Witness-blocked but semantically clean (hexdiff shows 0 structural, reg-swap-only diffs): append a stall note to attempts.jsonl with next_change "accept via --smt out-of-band" and move on. Do NOT retry that target with strict/live-out/memory/ppc-eabi contracts.
- Callee readiness first: if a target has indirect calls, unresolved callees, or called_functions not yet FULL_MATCH/EQUIVALENT_MATCH, record the blocker in attempts.jsonl and move to the next target — do NOT spend a cycle on it.
- Per-unit compiler flags are a legitimate matching tool: if hexdiff shows a diff MWCC_REFERENCE attributes to flags (-func_align 4/16, -ipa off, mw_version="GC/3.0a5.2"), apply the documented fix to YOUR unit's Object(...) in configure.py, hexdiff-verify, revert if it doesn't help. No blind flag-sweeping, no editing cflags_sdk globals or other units' entries, no failed flag experiments left in place.
- No external source hunting (no web_search / fetch_content / curl).
- Search budget: max 3 grep/find per function; read hexdiff --json output directly, don't grep tool output.
- No hand disassembly (no objdump / llvm-objdump / powerpc-eabi-objdump) — hexdiff prints the same disassembly.
- Shared headers are your domain: new/fixed struct/class/enum/vtable/declarations go in the shared header when the type is shared; keep changes additive; fix or revert if another unit's build breaks. Do NOT edit tooling (tools/coop, tools/ppc_equivalence, retail_reloc_map.json, configure.py globals) or other agents' source files.
- Minimal git: status/diff once at start and once before commit.
- Decompile to high-level C/C++ only — no asm, no register/stack micro-management (isolated PS backend exception in PLAN.md §17.6 only).

WORKFLOW per target:
1. Claim: .venv/bin/python3 tools/coop/run.py targets claim <target-id> --owner cri-b{batch['batch_id'].split('-')[1]}
2. Inspect: .venv/bin/python3 tools/coop/run.py targets show <target-id> — if callee-blocked, record and skip.
3. Iterate fast with hexdiff (it builds itself and holds the repo build lock; never run ninja/configure.py directly): .venv/bin/python3 tools/coop/hexdiff.py <unit> --symbol <mangled-or-substr> --brief ; read --json for detail.
4. Accept with cycle WITHOUT --smt: .venv/bin/python3 tools/coop/run.py cycle <target-id> --hypothesis "..." --next-change "..." ; or mass-accept several at once with .venv/bin/python3 tools/coop/batch-cycle.py <id...> --default-hypothesis "..." --default-next-change "...".
5. Commit accepted work: git add <your unit files> docs/evidence/decomp/attempts.jsonl && git commit -m "cri: match <units> batch {batch['batch_id']}".

FINAL REPORT (end of your session, plain text):
- Per target: id | function | final status (FULL_MATCH/EQUIVALENT_MATCH/ACTIVE + %) | blocker (if not accepted)
- List of targets accepted
- List of blockers for the out-of-band probe pass (witness-blocked-but-clean, callee-blocked, etc.)
"""


def agent_finished_status(inspect):
    """Classify a paseo inspect payload by status string.

    paseo's status flaps (error->running) on transient provider flakes; the
    authoritative liveness signal is UpdatedAt advancing across sweeps, which
    callers gate on. This only classifies the status string.
    """
    if inspect is None:
        return "error"
    st = (inspect.get("Status") or "").lower()
    if st in ("running", "pending"):
        return None
    if st in ("idle", "completed", "finished"):
        return "done"
    return "error"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--running", type=int, default=10)
    ap.add_argument("--interval", type=int, default=180)
    ap.add_argument("--max-idle-min", type=int, default=90)
    ap.add_argument("--report", action="store_true")
    ap.add_argument("--once", action="store_true")
    args = ap.parse_args()

    if not args.report:
        # single-instance guard: refuse to run if another monitor is alive
        pidfile = os.path.join(HERE, "monitor.pid")
        if os.path.exists(pidfile):
            try:
                old = int(open(pidfile).read().strip())
                os.kill(old, 0)
                print(f"another monitor is running (pid {old}); exiting")
                return
            except (ProcessLookupError, ValueError):
                pass
        open(pidfile, "w").write(str(os.getpid()))

    os.makedirs(REPORTS, exist_ok=True)
    plan = json.load(open(PLAN))
    st = load_state()
    by_id = {b["batch_id"]: b for b in plan["batches"]}

    if args.report:
        for bid, rec in sorted(st["batches"].items()):
            print(f"{bid}: agent={rec.get('agent_id')} status={rec.get('status')} launches={rec.get('launches')}")
        return

    # prune done batches from plan memory
    done = set(st["done"])

    def pending_ids():
        return [b for b in plan["batches"] if b["batch_id"] not in st["batches"] and b["batch_id"] not in done]

    while True:
        agents = paseo_ls()
        by_aid = {a["id"]: a for a in agents}

        # sweep tracked agents
        for bid, rec in list(st["batches"].items()):
            if rec.get("status") in ("done", "error", "relaunch") and not rec.get("agent_id"):
                continue
            aid = rec.get("agent_id")
            if not aid:
                continue
            info = by_aid.get(aid)
            insp = paseo_inspect(aid) if info is not None else None
            if info is None:
                # disappeared from ls: we archived it (status relaunch/done) or it crashed
                if rec.get("status") != "relaunch":
                    log(f"CRASHED: {bid} agent {aid} no longer listed — relaunching")
                    rec["status"] = "relaunch"
                    rec["agent_id"] = None
                    save_state(st)
                continue

            # activity age since last agent activity (authoritative liveness)
            upd = None
            if insp:
                upd = insp.get("UpdatedAt") or insp.get("LastActivityAt")
            ts = None
            try:
                if upd:
                    ts = datetime.fromisoformat(str(upd).replace("Z", "+00:00"))
            except Exception:
                ts = None
            age_min = (datetime.now(timezone.utc) - ts).total_seconds() / 60 if ts else None

            if age_min is None:
                continue  # can't tell; leave alone

            if age_min < 5:
                # fresh activity: alive, whatever the status string says (provider flake recovery)
                rec["status"] = "running"
                rec["error_since"] = None
                continue

            # stale activity (>= 5 min): status now matters
            finished = agent_finished_status(insp)
            if finished is None:
                # status running/pending but stale -> hang
                if age_min > args.max_idle_min:
                    log(f"HANG: {bid} agent {aid} no activity {age_min:.0f}m — archiving and relaunching")
                    paseo_archive(aid)
                    rec["status"] = "relaunch"
                    rec["agent_id"] = None
                    save_state(st)
                continue
            if finished == "done":
                log(f"FINISHED: {bid} agent {aid} -> done")
                rec["status"] = "done"
                rec["finished_at"] = datetime.now(timezone.utc).isoformat(timespec="seconds")
                # save log tail as report
                r = sh(["logs", aid])
                with open(os.path.join(REPORTS, f"{bid}.txt"), "w") as f:
                    f.write(r.stdout[-20000:] if r.stdout else "(no log)")
                if bid not in st["done"]:
                    st["done"].append(bid)
                save_state(st)
            else:
                # error status + stale activity: grace period for provider flake recovery
                first = rec.get("error_since")
                if not first:
                    rec["error_since"] = datetime.now(timezone.utc).isoformat(timespec="seconds")
                    save_state(st)
                    continue
                grace_min = (datetime.now(timezone.utc) - datetime.fromisoformat(first)).total_seconds() / 60
                if grace_min < 12:
                    continue  # still in grace, it may recover
                log(f"DEAD: {bid} agent {aid} error for {grace_min:.0f}m — archiving and relaunching")
                paseo_archive(aid)
                rec["launches"] = rec.get("launches", 1)
                if rec["launches"] < MAX_RELAUNCH:
                    rec["status"] = "relaunch"
                    rec["agent_id"] = None
                else:
                    log(f"{bid}: exceeded relaunch budget ({MAX_RELAUNCH}), marking done")
                    if bid not in st["done"]:
                        st["done"].append(bid)
                    rec["status"] = "done"
                rec["error_since"] = None
                save_state(st)

        # relaunch slots marked relaunch
        for bid, rec in st["batches"].items():
            if rec.get("status") == "relaunch" and not rec.get("agent_id"):
                b = by_id[bid]
                rec["launches"] = rec.get("launches", 1) + 1
                if rec["launches"] > MAX_RELAUNCH:
                    log(f"{bid}: launch budget exceeded ({MAX_RELAUNCH}), marking done")
                    if bid not in st["done"]:
                        st["done"].append(bid)
                    rec["status"] = "done"
                    save_state(st)
                    continue
                aid = launch_batch(b, f"CRI-MATCH-{bid}")
                if aid:
                    rec["agent_id"] = aid
                    rec["launched_at"] = datetime.now(timezone.utc).isoformat(timespec="seconds")
                    rec["status"] = "running"
                save_state(st)

        # launch new batches up to --running
        running = sum(1 for r in st["batches"].values() if r.get("agent_id") and r.get("status") == "running")
        for b in pending_ids():
            if running >= args.running:
                break
            aid = launch_batch(b, f"CRI-MATCH-{b['batch_id']}")
            if not aid:
                continue
            st["batches"][b["batch_id"]] = {
                "agent_id": aid,
                "launched_at": datetime.now(timezone.utc).isoformat(timespec="seconds"),
                "status": "running",
                "launches": 1,
            }
            st["launch_order"].append(b["batch_id"])
            running += 1
            save_state(st)

        remaining = len(pending_ids()) + sum(
            1 for r in st["batches"].values() if r.get("status") in ("running", "relaunch", "error")
        )
        log(
            f"running={running} done={len(st['done'])}/{len(plan['batches'])} "
            f"pending={len(pending_ids())} remaining={remaining} — next sweep in {args.interval}s"
        )
        if args.once:
            break
        if remaining == 0 and running == 0:
            log("ALL BATCHES COMPLETE")
            break
        time.sleep(args.interval)


if __name__ == "__main__":
    main()

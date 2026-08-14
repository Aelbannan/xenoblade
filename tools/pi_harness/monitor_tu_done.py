#!/usr/bin/env python3
"""Monitor the 4 big TUs of run 6 until terminal state.

Terminal per TU: NOT_STARTED==0 OR no ledger activity for 15+ min with
NOT_STARTED stable across 3 polls (exhausted -> dropped from pool).
Writes one status line per poll to build/pi-harness/monitor_tu_done.log
and exits 0 with ALL_DONE once every TU is terminal.
"""
import json
import os
import subprocess
import sys
import time
from datetime import datetime, timezone

ROOT = "/Users/ahmedelbannan/Ahmed/xenoblade"
LEDGER = os.path.join(ROOT, "build/pi-harness/ledger.jsonl")
RUNLOG = os.path.join(ROOT, "build/pi-harness/run-20260810-135336.log")
OUT = os.path.join(ROOT, "build/pi-harness/monitor_tu_done.log")
TARGETS = os.path.join(ROOT, "tools/coop/targets.json")
TUS = [
    "monolib/src/coli/code_804A6C60",
    "kyoshin/CItemBoxInfo",
    "kyoshin/CItemBoxGrid",
    "kyoshin/CEquipItemBox",
]

POLL_S = 600
IDLE_TERMINAL_S = 900
STABLE_POLLS = 3


def parse_iso(ts: str) -> float:
    try:
        return datetime.fromisoformat(ts.replace("Z", "+00:00")).timestamp()
    except ValueError:
        return 0.0


def load_units():
    with open(TARGETS) as f:
        raw = json.load(f)
    units = {}
    for t in raw["targets"]:
        units.setdefault(t["unit"], set()).add(t["id"])
        if t.get("status") == "NOT_STARTED":
            pass
    ns = {u: sum(1 for t in raw["targets"] if t["unit"] == u and t.get("status") == "NOT_STARTED") for u in units}
    return units, ns


def last_activity(unit: str, unit_ids: set) -> float:
    last = 0.0
    with open(LEDGER) as f:
        for line in f:
            try:
                e = json.loads(line)
            except json.JSONDecodeError:
                continue
            d = e.get("detail") or {}
            ids = []
            if isinstance(d, dict):
                ids = d.get("targetIds") or [r.get("targetId") for r in (d.get("results") or [])]
            if ids and any(i in unit_ids for i in ids):
                ts = parse_iso(e.get("ts", ""))
                if ts > last:
                    last = ts
    return last


def main() -> int:
    stable = {u: 0 for u in TUS}
    prev = {u: -1 for u in TUS}
    with open(OUT, "w") as out:
        while True:
            units, ns = load_units()
            now = time.time()
            ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            line = ts
            alldone = True
            for u in TUS:
                unit_ns = ns.get(u, 0)
                last = last_activity(u, units.get(u, set()))
                age = int(now - last) if last else 99999
                if unit_ns == 0:
                    state = "DONE(ns=0)"
                    done = True
                elif age > IDLE_TERMINAL_S:
                    if prev[u] == unit_ns:
                        stable[u] += 1
                    else:
                        stable[u] = 0
                    done = stable[u] >= STABLE_POLLS
                    state = f"DONE(exhausted, idle {age}s)" if done else f"idle {age}s (stable {stable[u]}/{STABLE_POLLS})"
                else:
                    stable[u] = 0
                    state = f"active (idle {age}s)"
                    done = False
                prev[u] = unit_ns
                if not done:
                    alldone = False
                line += f" | {u.split('/')[-1]}: ns={unit_ns} {state}"
            print(line, file=out, flush=True)
            # Run 6 natural exit: process gone + "Done." in the log.
            proc_alive = subprocess.run(["pgrep", "-f", "tsx src/index.ts"], capture_output=True).returncode == 0
            log_done = False
            try:
                with open(RUNLOG) as f:
                    log_done = "[pi-harness] Done." in f.read()
            except FileNotFoundError:
                pass
            if not proc_alive and log_done:
                print(f"RUN_EXITED {ts}", file=out, flush=True)
                return 0
            if alldone:
                print(f"ALL_DONE {ts}", file=out, flush=True)
                return 0
            time.sleep(POLL_S)


if __name__ == "__main__":
    sys.exit(main())

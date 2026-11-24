"""Mechanical wave steps for a cheap orchestrator (see .agents/skills/xb-orchestrator).

Each command either succeeds or exits non-zero with one ``ERROR:`` line per problem, so the
orchestrator only has to follow exit codes:

    wave launch W [--ds-max 2] [--dry-run]   # start workers; idempotent; hy4 all at once, ds throttled
    wave manage W [--timeout S]              # wait loop: backfill ds, answer permissions, time out stuck tasks
    wave check-plan W                        # validate the planner's targets.tsv / hints / plan.md
    wave check-review W                      # validate the reviewer's review.tsv
    wave decide W                            # record decisions, move held patches, write W.multi, prep Wr

Exit codes: 0 ok; 1 validation errors; 3 ``manage`` timed out with tasks still pending.
"""
from __future__ import annotations

import json
import re
import shutil
import subprocess
import time
from pathlib import Path

from tools.coop.land import shapes, wave

PASEO = "/Applications/Paseo.app/Contents/Resources/bin/paseo"
WORKSPACE = "wks_5ac8e57131b986e1"
ORCH = Path("/tmp/xb/orch")
TASK_TIMEOUT = 2 * 3600
DECISIONS = ("land", "hold", "drop", "retry-ds")
COMMANDS = ("launch", "manage", "check-plan", "check-review", "decide", "shapes", "check-counter")
COLS = ("task", "model", "unit", "symbol", "pct", "structural", "size", "src")


def _wd(w: str) -> Path:
    return wave.WAVES / w


def _paseo(*args: str) -> subprocess.CompletedProcess:
    return subprocess.run([PASEO, *args], capture_output=True, text=True)


def _has_result(w: str, task: str) -> bool:
    return (_wd(w) / f"{task}.result").is_file()


# --------------------------------------------------------------------------- launch
def _agents_file(w: str) -> Path:
    return _wd(w) / "agents.json"


def _agents(w: str) -> dict[str, dict]:
    f = _agents_file(w)
    return json.loads(f.read_text()) if f.is_file() else {}


def launch(w: str, ds_max: int = 2, dry_run: bool = False, runner=_paseo) -> list[str]:
    """Start every hy4 task not yet launched and up to ``ds_max`` concurrent ds tasks."""
    agents = _agents(w)
    started = []
    ds_running = sum(1 for t, a in agents.items() if a["model"] == "ds" and not _has_result(w, t))
    for t in wave._targets(w):
        task = t["task"]
        if task in agents or _has_result(w, task):
            continue
        if t["model"] == "ds":
            if ds_running >= ds_max:
                continue
            ds_running += 1
        prompt = _wd(w) / f"{task}.prompt"
        if not prompt.is_file():
            raise SystemExit(f"ERROR: {prompt} missing (run `wave prep {w}` first)")
        title = f"{task} {t['symbol']}"[:60]
        model_id = wave.MODELS.get(t["model"], t["model"])
        if dry_run:
            started.append(f"{task}\t{model_id}\t(dry run)")
            continue
        r = runner("run", "--no-wait", "-q", "--provider", "pi", "--model", model_id, "--thinking", "high",
                   "--workspace", WORKSPACE, "--title", title, "--label", f"wave={w}", prompt.read_text())
        aid = (r.stdout.strip().splitlines() or [""])[-1].strip()
        if r.returncode != 0 or not aid:
            raise SystemExit(f"ERROR: launch {task} failed: {(r.stderr or r.stdout).strip()[:200]}")
        agents[task] = {"id": aid, "model": t["model"], "started": time.time()}
        _agents_file(w).write_text(json.dumps(agents, indent=1))
        started.append(f"{task}\t{model_id}\t{aid}")
    return started


# --------------------------------------------------------------------------- permissions
_ALLOWED_ROOTS = (str(wave.ROOT), "/tmp/xb")
_DENY = [
    (re.compile(r"(^|[\s;&|(])git(\s|$)"), "git"),
    (re.compile(r"\brm\s+-[a-zA-Z]*r[a-zA-Z]*f|\brm\s+-[a-zA-Z]*f[a-zA-Z]*r"), "rm -rf"),
    (re.compile(r"tools\.coop\.land\s+(check|apply|adopt)\b|XB_LAND_OWNER"), "land gate (orchestrator only)"),
    (re.compile(r"faithful\b.*--init"), "faithful --init"),
    (re.compile(r"\.hexdiff\.lock"), "hexdiff lock"),
    (re.compile(r"\.ctx\.c\b"), ".ctx.c"),
]
_PATH = re.compile(r"(?<![\w.])(?:~|/)[^\s'\"`;|&<>()]*")


def _strings(o) -> list[str]:
    if isinstance(o, str):
        return [o]
    if isinstance(o, dict):
        return [s for v in o.values() for s in _strings(v)]
    if isinstance(o, list):
        return [s for v in o for s in _strings(v)]
    return []


def classify_permission(req: dict) -> tuple[str, str]:
    """('allow'|'deny', reason) for one pending permission request (format-agnostic)."""
    body = req.get("request", req)
    text = "\n".join(_strings({k: v for k, v in body.items() if k not in ("agentId", "agent", "id")}))
    if not text.strip():
        return "deny", "unreadable request"
    for pat, why in _DENY:
        if pat.search(text):
            return "deny", why
    for p in _PATH.findall(text):
        if p in ("/", "~") or p.startswith("~") or ".." in p.split("/"):
            return "deny", f"path {p}"
        if p.startswith("/dev/null"):
            continue
        if not any(p == r or p.startswith(r + "/") for r in _ALLOWED_ROOTS):
            return "deny", f"outside repo: {p}"
    return "allow", "in repo"


def _req_ids(req: dict) -> tuple[str | None, str | None]:
    agent = req.get("agentId") or req.get("agent_id") or req.get("agent")
    if isinstance(agent, dict):
        agent = agent.get("id")
    rid = req.get("requestId") or req.get("request_id") or req.get("id")
    if isinstance(req.get("request"), dict):
        rid = req["request"].get("id") or rid
    return agent, rid


def handle_permissions(w: str, runner=_paseo) -> list[str]:
    r = runner("permit", "ls", "--json")
    try:
        pending = json.loads(r.stdout or "[]")
    except json.JSONDecodeError:
        return [f"WARN permit ls unreadable: {r.stdout[:120]!r}"]
    mine = {a["id"] for a in _agents(w).values()}
    log = []
    for req in pending if isinstance(pending, list) else []:
        agent, rid = _req_ids(req)
        if not agent or not rid:
            log.append(f"WARN unparseable permission request: {json.dumps(req)[:160]}")
            continue
        if not any(agent == m or m.startswith(agent) or agent.startswith(m) for m in mine):
            continue  # another wave's / the user's agent: not ours to answer
        verdict, why = classify_permission(req)
        runner("permit", verdict, agent, rid)
        log.append(f"{verdict} {agent[:8]} {rid}: {why}")
    if log:
        with (_wd(w) / "permissions.log").open("a") as f:
            f.write("".join(f"{time.strftime('%FT%T')} {l}\n" for l in log))
    return log


# --------------------------------------------------------------------------- manage
def manage(w: str, timeout: int = 1200, poll: int = 30, ds_max: int = 2, runner=_paseo) -> int:
    end = time.time() + timeout
    tasks = [t["task"] for t in wave._targets(w)]
    while True:
        for line in launch(w, ds_max, runner=runner):
            print("launched", line)
        for line in handle_permissions(w, runner):
            print("permission", line)
        agents = _agents(w)
        for task, a in agents.items():
            if not _has_result(w, task) and time.time() - a["started"] > TASK_TIMEOUT:
                runner("stop", a["id"])
                (_wd(w) / f"{task}.result").write_text(f"BLOCKED {task}: timeout (stopped after 2h)\n")
                print("timeout", task)
        pending = [t for t in tasks if not _has_result(w, t)]
        if not pending:
            print(f"ALL DONE {len(tasks)} tasks")
            return 0
        if time.time() >= end:
            print(f"PENDING {len(pending)}: {' '.join(pending)}")
            return 3
        time.sleep(poll)


# --------------------------------------------------------------------------- checks
def _rows(path: Path, ncols: int) -> list[list[str]]:
    return [l.split("\t") for l in path.read_text().splitlines() if l.strip()] if path.is_file() else []


def check_plan(w: str, max_ds: int = 6) -> list[str]:
    d = _wd(w)
    errs = []
    sel = {r[0]: r for r in _rows(d / "targets.sel.tsv", 8)}
    rows = _rows(d / "targets.tsv", 8)
    if not sel:
        errs.append("targets.sel.tsv missing (run `wave select` again)")
    if not 10 <= len(rows) <= 24:
        errs.append(f"targets.tsv has {len(rows)} rows (want 10-24)")
    seen = set()
    for r in rows:
        if len(r) != 8:
            errs.append(f"row {r[:1]}: {len(r)} columns (want 8)")
            continue
        task = r[0]
        if task in seen:
            errs.append(f"{task}: duplicate row")
        seen.add(task)
        if task not in sel:
            errs.append(f"{task}: not in the selection")
        elif [r[i] for i in (2, 3, 7)] != [sel[task][i] for i in (2, 3, 7)]:
            errs.append(f"{task}: unit/symbol/src changed from the selection")
        if r[1] not in wave.MODELS:
            errs.append(f"{task}: model {r[1]!r} (want one of {sorted(wave.MODELS)})")
    if sum(1 for r in rows if len(r) == 8 and r[1] == "ds") > max_ds:
        errs.append(f"more than {max_ds} ds rows")
    for h in (d / "hints").glob("*.md") if (d / "hints").is_dir() else []:
        if h.stem not in seen:
            errs.append(f"hint {h.name} has no target row")
        elif len(h.read_text().strip().splitlines()) > 6:
            errs.append(f"hint {h.name} longer than 6 lines")
    if not (d / "plan.md").is_file():
        errs.append("plan.md missing")
    return errs


def check_review(w: str) -> list[str]:
    d = _wd(w)
    errs = []
    tasks = [t["task"] for t in wave._targets(w)]
    rows = _rows(d / "review.tsv", 3)
    got: dict[str, list[str]] = {}
    for r in rows:
        if len(r) < 3 or not r[2].strip():
            errs.append(f"row {r[:1]}: want TASK<TAB>DECISION<TAB>REASON")
            continue
        if r[0] in got:
            errs.append(f"{r[0]}: duplicate row")
        got[r[0]] = r
        if r[0] not in tasks:
            errs.append(f"{r[0]}: not a task of {w}")
        allowed = DECISIONS[:3] if w.endswith("r") else DECISIONS
        if r[1] not in allowed:
            errs.append(f"{r[0]}: decision {r[1]!r} (want one of {allowed})")
        if r[1] == "land" and not (wave.PLANS / f"{r[0]}.patch").is_file():
            errs.append(f"{r[0]}: land but no {wave.PLANS}/{r[0]}.patch")
        if r[1] == "hold":
            slug = shapes.tag(r[2])
            if not slug:
                errs.append(f"{r[0]}: hold reason must start with [shape:<slug>]")
            elif slug not in shapes.registry() and not (d / "shapes" / f"{slug}.md").is_file():
                errs.append(f"{r[0]}: new shape {slug!r} needs shapes/{slug}.md (line 1 description, then example)")
        if r[1] == "retry-ds" and not (d / "hints" / f"{r[0]}.md").is_file():
            errs.append(f"{r[0]}: retry-ds without hints/{r[0]}.md")
    errs += [f"{t}: no review row" for t in tasks if t not in got]
    return errs


# --------------------------------------------------------------------------- decide
def decide(w: str) -> dict:
    errs = check_review(w)
    if errs:
        raise SystemExit("\n".join(f"ERROR: {e}" for e in errs))
    d = _wd(w)
    rows = _rows(d / "review.tsv", 3)
    targets = {t["task"]: t for t in wave._targets(w)}
    lands, retries, counts = [], [], {k: 0 for k in DECISIONS}
    reg = shapes.registry()
    for f in sorted((d / "shapes").glob("*.md")) if (d / "shapes").is_dir() else []:
        if f.stem not in reg:
            reg[f.stem] = {**shapes.read_new_shape(f), "added": w}
    shapes.save_registry(reg)
    for task, dec, reason, *_ in rows:
        counts[dec] += 1
        if dec in ("land", "hold"):
            shapes.archive("landed" if dec == "land" else "held", w, task, d / task)
        wave.record(w, task, "hold" if dec == "retry-ds" else dec, reason)
        if dec == "land":
            lands.append(task)
        else:
            p = wave.PLANS / f"{task}.patch"
            if p.is_file():
                (wave.PLANS / "held").mkdir(parents=True, exist_ok=True)
                shutil.move(str(p), str(wave.PLANS / "held" / p.name))
            if dec == "retry-ds":
                retries.append(task)
    ORCH.mkdir(parents=True, exist_ok=True)
    multi = ORCH / f"{w}.multi"
    if lands:
        wave.queue(w, lands, str(multi))
    else:
        multi.write_text("")
    out = {"counts": counts, "multi": str(multi), "lands": lands, "retry_wave": None}
    if retries:
        rw = f"{w}r"
        rd = _wd(rw)
        (rd / "hints").mkdir(parents=True, exist_ok=True)
        body = "".join("\t".join(["ds" if c == "model" else str(targets[t][c]) for c in COLS]) + "\n"
                       for t in retries)
        (rd / "targets.tsv").write_text(body)
        (rd / "targets.sel.tsv").write_text(body)
        for t in retries:
            shutil.copyfile(d / "hints" / f"{t}.md", rd / "hints" / f"{t}.md")
        out["retry_wave"] = rw
    return out


# --------------------------------------------------------------------------- CLI
def add_parsers(sub) -> None:
    s = sub.add_parser("launch"); s.add_argument("wave"); s.add_argument("--ds-max", type=int, default=2)
    s.add_argument("--dry-run", action="store_true")
    s = sub.add_parser("manage"); s.add_argument("wave"); s.add_argument("--timeout", type=int, default=1200)
    s.add_argument("--ds-max", type=int, default=2)
    s = sub.add_parser("check-plan"); s.add_argument("wave")
    s = sub.add_parser("check-review"); s.add_argument("wave")
    s = sub.add_parser("decide"); s.add_argument("wave")
    s = sub.add_parser("shapes"); s.add_argument("--promote", action="store_true"); s.add_argument("--show")
    s = sub.add_parser("check-counter"); s.add_argument("slug", nargs="+")


def run(a) -> int:
    if a.cmd == "launch":
        for line in launch(a.wave, a.ds_max, a.dry_run):
            print(line)
        return 0
    if a.cmd == "manage":
        return manage(a.wave, a.timeout, ds_max=a.ds_max)
    if a.cmd in ("check-plan", "check-review"):
        errs = (check_plan if a.cmd == "check-plan" else check_review)(a.wave)
        for e in errs:
            print("ERROR:", e)
        print(f"{a.cmd} {a.wave}: " + ("OK" if not errs else f"{len(errs)} error(s)"))
        return 1 if errs else 0
    if a.cmd == "decide":
        r = decide(a.wave)
        c = r["counts"]
        print(f"decide {a.wave}: land {c['land']} hold {c['hold']} drop {c['drop']} retry-ds {c['retry-ds']}")
        print(f"queue: {r['multi']} ({len(r['lands'])} plans)")
        if r["retry_wave"]:
            print(f"retry wave: {r['retry_wave']} (run prep / launch / manage / collect on it)")
        return 0
    if a.cmd == "shapes":
        reg, tal = shapes.registry(), shapes.tally()
        if a.show:
            e = reg.get(a.show, {})
            print(f"{a.show}: {e.get('desc', '(not in registry)')}\ncounter: {e.get('counter')}\nexample:\n{e.get('example', '')}")
            for kind in ("held", "landed"):
                tasks = tal.get(a.show, {}).get("tasks", []) if kind == "held" else None
                for p in shapes.examples(kind, tasks):
                    print(f"{kind}\t{p}")
            return 0
        for slug, t in sorted(tal.items(), key=lambda kv: -kv[1]["count"]):
            st = shapes.status(slug, t, reg)
            if a.promote and st != "promote":
                continue
            print(f"{'PROMOTE' if st == 'promote' else st}\t{slug}\t{t['count']} holds\t{len(t['waves'])} waves"
                  f"\t{(reg.get(slug) or {}).get('desc', '?')}")
        return 0
    if a.cmd == "check-counter":
        errs = [e for s in a.slug for e in shapes.check_counter(s)]
        for e in errs:
            print("ERROR:", e)
        print("check-counter: " + ("OK" if not errs else f"{len(errs)} error(s)"))
        return 1 if errs else 0
    return 2

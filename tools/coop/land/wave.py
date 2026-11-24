"""Low-touch wave pipeline for cheap matching agents.

The orchestrator's own context is the expensive part of a wave, so every step
here is designed to be one command with a compact summary:

    python -m tools.coop.land wave census                 # residual census of every unit (~5 min)
    python -m tools.coop.land wave select W --n 20        # pick structural targets, one per file
    python -m tools.coop.land wave prep W                 # packs, prompts, owned files, originals
      (launch agents from the printed table with notifications off)
    python -m tools.coop.land wave wait W [--timeout S]   # blocks until every task wrote its .result
    python -m tools.coop.land wave collect W              # harvest + auto-screen -> one table
    python -m tools.coop.land wave queue W T... --out Q   # write the land-gate queue for accepted tasks

Layout: /tmp/xb/waves/<W>/{targets.tsv, <T>.prompt, <T>.files, <T>.result,
<T>/orig/..., <T>/final/..., summary.tsv}; patches go to /tmp/xb/plans/<T>.patch,
auto-held ones to /tmp/xb/plans/held/.  Task outcomes are appended to
tools/coop/land/state/wave_history.jsonl so later selections skip functions
that were already blocked or held at the same source state.

Selection policy (wave 1 evidence): functions whose residual is register
allocation only produced every BLOCKED regalloc result and most steering
attempts, so they are excluded; targets need at least one structural diff.
"""
from __future__ import annotations

import concurrent.futures as cf
import hashlib
import json
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

from tools.coop import faithful
from tools.coop.land import bans, ledger, pack

ROOT = pack.ROOT
PY = sys.executable
WAVES = Path("/tmp/xb/waves")
PLANS = Path("/tmp/xb/plans")
CENSUS = ledger.STATE / "residual_census.json"
HISTORY = ledger.STATE / "wave_history.jsonl"

MODELS = {"hy4": "opencode-go/hy4-flash-free", "ds": "opencode-go/deepseek-v4.1-flash"}

PROMPT = """\
You are a decompilation matching agent on a Wii game decomp (MWCC compiler, PowerPC).
Repo: {root}

Your whole task brief is in {pack}. Read it first, completely: it has the rules,
the current source of your function, and the retail-vs-decomp instruction diff.

Work only in the files listed under "Files" in the brief. Never run git, never run
`python -m tools.coop.land`, never edit tools/, config/, configure.py or *.ctx.c.
Scratch files go under /tmp/xb/{task}/ only. Use .venv/bin/python3.

When you finish (DONE, BLOCKED, or partial), your LAST action must be writing a
one-line result file:

    {result}

containing exactly one line, e.g.
    DONE {task}: <one-line summary> (<before>% -> 100%)
    BLOCKED {task}: <class>: <one-line reason> (<pct>%)
    PARTIAL {task}: <one-line summary> (<before>% -> <after>%)
Then reply with the same line and stop.
"""


# --------------------------------------------------------------------------- census
def _units() -> list[str]:
    d = json.loads((ROOT / "objdiff.json").read_text())
    return [u["name"].removeprefix("main/") for u in d.get("units", [])
            if u.get("base_path") and u.get("target_path")]


def _unit_rows(unit: str) -> tuple[str, list[dict] | str]:
    try:
        r = subprocess.run([PY, "tools/coop/hexdiff.py", unit, "--all", "--json", "--no-build", "--no-baseline"],
                           cwd=ROOT, capture_output=True, text=True, timeout=600)
        d = json.loads(r.stdout)
    except Exception as e:  # noqa: BLE001
        return unit, f"error: {e!r}"[:200]
    keep = ("symbol", "mark", "objdiff_percent", "structural", "reg_swap", "pure_reg_swap",
            "retail_size", "decomp_size", "unpaired")
    return unit, [{k: f.get(k) for k in keep} for f in d.get("functions", [])]


def census(jobs: int = 8) -> dict:
    units = _units()
    out: dict = {"time": time.time(), "units": {}, "errors": {}}
    with cf.ThreadPoolExecutor(jobs) as ex:
        for unit, rows in ex.map(_unit_rows, units):
            (out["errors"] if isinstance(rows, str) else out["units"])[unit] = rows
    CENSUS.write_text(json.dumps(out))
    return out


# --------------------------------------------------------------------------- selection
def _file_sha(rel: str) -> str:
    p = ROOT / rel
    return hashlib.sha1(p.read_bytes()).hexdigest() if p.is_file() else ""


def _history() -> dict[tuple[str, str], dict]:
    h: dict[tuple[str, str], dict] = {}
    if HISTORY.is_file():
        for line in HISTORY.read_text().splitlines():
            if line.strip():
                r = json.loads(line)
                h[(r["unit"], r["symbol"])] = r
    return h


def select(wave: str, n: int = 20, lo: float = 80.0, hi: float = 99.99, max_size: int = 0x400,
           model: str = "hy4") -> list[dict]:
    c = json.loads(CENSUS.read_text())
    hist = _history()
    cands = []
    for unit, rows in c["units"].items():
        src = pack._source_for(unit)
        if src is None:
            continue
        rel = str(src.relative_to(ROOT))
        for r in rows:
            if r.get("mark") == "✓" or r.get("unpaired"):
                continue
            pct = r.get("objdiff_percent") or 0.0
            if not (lo <= pct <= hi) or (r.get("structural") or 0) < 1:
                continue  # register-allocation-only residuals are excluded
            if (r.get("retail_size") or 0) > max_size:
                continue
            prev = hist.get((unit, r["symbol"]))
            if prev and prev.get("src_sha") == _file_sha(rel):
                continue  # already tried at this exact source state
            cands.append((unit, r, rel))
    # closest first: few structural diffs, high percent, small functions
    cands.sort(key=lambda t: ((t[1].get("structural") or 0), -(t[1].get("objdiff_percent") or 0),
                              t[1].get("retail_size") or 0))
    used: set[str] = set()
    picked = []
    for unit, r, rel in cands:
        hdr = str(Path(rel).with_suffix(".hpp"))
        if rel in used or hdr in used:
            continue
        used.update({rel, hdr})
        picked.append({"task": f"{wave}-{len(picked):02d}", "model": model, "unit": unit,
                       "symbol": r["symbol"], "pct": r.get("objdiff_percent"),
                       "structural": r.get("structural"), "size": r.get("retail_size"), "src": rel})
        if len(picked) >= n:
            break
    d = WAVES / wave
    d.mkdir(parents=True, exist_ok=True)
    cols = ("task", "model", "unit", "symbol", "pct", "structural", "size", "src")
    body = "".join("\t".join(str(p[k]) for k in cols) + "\n" for p in picked)
    (d / "targets.tsv").write_text(body)
    (d / "targets.sel.tsv").write_text(body)  # untouched selection: check-plan compares against it
    return picked


def _targets(wave: str) -> list[dict]:
    cols = ("task", "model", "unit", "symbol", "pct", "structural", "size", "src")
    return [dict(zip(cols, l.rstrip("\n").split("\t")))
            for l in (WAVES / wave / "targets.tsv").read_text().splitlines() if l.strip()]


# --------------------------------------------------------------------------- prep
def prep(wave: str) -> list[dict]:
    d = WAVES / wave
    unledgered = set(ledger.unledgered())
    rows = []
    for t in _targets(wave):
        task = t["task"]
        body = pack.build(t["unit"], t["symbol"], task)
        pk = Path("/tmp/xb/packs") / f"{task}.md"
        pk.parent.mkdir(parents=True, exist_ok=True)
        pk.write_text(body)
        files = re.findall(r"(?m)^- (\S+)$", body.split("## Files", 1)[1].split("\n\n(only", 1)[0])
        dirty = [f for f in files if f in unledgered]
        if dirty:
            raise SystemExit(f"ABORT prep {task}: owned file(s) not at a ledgered state: {dirty}")
        (d / f"{task}.files").write_text("\n".join(files) + "\n")
        for f in files:
            dst = d / task / "orig" / f
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / f, dst)
        res = d / f"{task}.result"
        res.unlink(missing_ok=True)
        prompt = PROMPT.format(root=ROOT, pack=pk, task=task, result=res)
        hint = d / "hints" / f"{task}.md"  # optional planner note (wave plan step)
        if hint.is_file():
            prompt += "\nPlanner note for this task (advice, not a licence to break the brief's rules):\n" \
                      + hint.read_text().strip() + "\n"
        (d / f"{task}.prompt").write_text(prompt)
        rows.append({**t, "prompt": str(d / f"{task}.prompt"), "model_id": MODELS.get(t["model"], t["model"])})
    return rows


# --------------------------------------------------------------------------- wait
def wait(wave: str, timeout: int = 5400, poll: int = 20) -> dict[str, str]:
    tasks = [t["task"] for t in _targets(wave)]
    end = time.time() + timeout
    while time.time() < end:
        missing = [t for t in tasks if not (WAVES / wave / f"{t}.result").is_file()]
        if not missing:
            break
        time.sleep(poll)
    return {t: ((WAVES / wave / f"{t}.result").read_text().strip().splitlines() or ["(empty)"])[-1]
            if (WAVES / wave / f"{t}.result").is_file() else "(no result)" for t in tasks}


# --------------------------------------------------------------------------- collect
_HARD = {k: v for k, v in faithful.HARD.items() if k != "packed_string"}


def _counts(text: str) -> dict[str, int]:
    out = {f"ban:{k}": v for k, v in bans.count(text).items()}
    code = faithful.strip_comments(text)
    out.update({k: len(p.findall(code)) for k, p in _HARD.items()})
    return out


def _code_only(text: str) -> str:
    return re.sub(r"\s+", " ", faithful.strip_comments(text)).strip()


def screen(orig: dict[str, str], final: dict[str, str]) -> tuple[str, list[str]]:
    """('drop'|'hold'|'review', flags) for one task's owned-file contents."""
    flags: list[str] = []
    changed = [f for f in final if final[f] != orig.get(f)]
    if not changed:
        return "drop", ["no change"]
    if all(_code_only(final[f]) == _code_only(orig.get(f, "")) for f in changed):
        return "drop", ["comment/whitespace only"]
    rises = []
    for f in changed:
        b, a = _counts(orig.get(f, "")), _counts(final[f])
        rises += [f"{k} +{a[k] - b.get(k, 0)}" for k in a if a[k] > b.get(k, 0)]
    if rises:
        return "hold", rises
    # review hints (not auto-hold): shapes worth a look in the diff
    for f in changed:
        added = set(final[f].splitlines()) - set(orig.get(f, "").splitlines())
        add = "\n".join(added)
        if re.search(r"\bstatic\s+inline\b", add):
            flags.append("new static inline")
        if re.search(r"\(\s*[\w:]+\s*\*\s*\)\s*&", add):
            flags.append("new (T*)& cast")
        if re.search(r"\breinterpret_cast\b", add):
            flags.append("new reinterpret_cast")
        if re.search(r"\bextern\b", add):
            flags.append("new extern")
    return "review", flags


def collect(wave: str) -> list[dict]:
    d = WAVES / wave
    PLANS.mkdir(parents=True, exist_ok=True)
    (PLANS / "held").mkdir(exist_ok=True)
    owned_all: set[str] = set()
    rows = []
    for t in _targets(wave):
        task = t["task"]
        files = (d / f"{task}.files").read_text().split()
        owned_all.update(files)
        orig = {f: (d / task / "orig" / f).read_text(encoding="utf-8", errors="replace") for f in files}
        final = {f: (ROOT / f).read_text(encoding="utf-8", errors="replace") for f in files}
        verdict, flags = screen(orig, final)
        parts = []
        for f in files:
            if final[f] == orig[f]:
                continue
            o = d / task / "orig" / f
            parts.append(subprocess.run(["diff", "-u", "--label", f, "--label", f, str(o), str(ROOT / f)],
                                        capture_output=True, text=True).stdout)
            dst = d / task / "final" / f
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / f, dst)
            shutil.copyfile(o, ROOT / f)  # restore: the tree goes back to its ledgered state
        patch = "".join(parts)
        res_f = d / f"{task}.result"
        result = res_f.read_text().strip().splitlines()[-1] if res_f.is_file() and res_f.read_text().strip() else "(no result)"
        for p in (PLANS / f"{task}.patch", PLANS / "held" / f"{task}.patch"):
            p.unlink(missing_ok=True)
        if verdict == "review":
            (PLANS / f"{task}.patch").write_text(patch)
        elif verdict == "hold":
            (PLANS / "held" / f"{task}.patch").write_text(patch)
        rows.append({"task": task, "model": t["model"], "symbol": t["symbol"], "result": result,
                     "verdict": verdict, "flags": "; ".join(flags), "lines": patch.count("\n")})
        with HISTORY.open("a") as h:
            h.write(json.dumps({"wave": wave, "task": task, "unit": t["unit"], "symbol": t["symbol"],
                                "result": result, "verdict": verdict, "src_sha": _file_sha(t["src"])}) + "\n")
    stray = [f for f in ledger.unledgered() if f not in owned_all]
    cols = ("task", "model", "verdict", "lines", "result", "flags")
    (d / "summary.tsv").write_text("".join("\t".join(str(r[k]) for k in cols) + "\n" for r in rows))
    if stray:
        (d / "stray.txt").write_text("\n".join(stray) + "\n")
    return rows + ([{"task": "STRAY", "model": "", "verdict": "hold", "lines": len(stray),
                     "result": "gated files changed outside owned sets", "flags": ", ".join(stray[:6])}]
                   if stray else [])


def record(wave: str, task: str, decision: str, note: str = "") -> None:
    """Orchestrator's final decision for a reviewed task (land/hold/drop)."""
    with HISTORY.open("a") as h:
        h.write(json.dumps({"wave": wave, "task": task, "decision": decision, "note": note}) + "\n")


def queue(wave: str, tasks: list[str], out: str) -> str:
    lines = []
    for task in tasks:
        if not (PLANS / f"{task}.patch").is_file():
            raise SystemExit(f"no reviewed patch for {task} in {PLANS}")
        lines.append(f"{task}\t\t\t\t\n")
    Path(out).write_text("".join(lines))
    return out


# --------------------------------------------------------------------------- CLI
def _table(rows: list[dict], cols: tuple[str, ...]) -> str:
    return "\n".join("\t".join(str(r.get(c, "")) for c in cols) for r in rows)


def main(argv: list[str]) -> int:
    import argparse
    ap = argparse.ArgumentParser(prog="python -m tools.coop.land wave")
    sub = ap.add_subparsers(dest="cmd", required=True)
    s = sub.add_parser("census"); s.add_argument("--jobs", type=int, default=8)
    s = sub.add_parser("select"); s.add_argument("wave"); s.add_argument("--n", type=int, default=20)
    s.add_argument("--lo", type=float, default=80.0); s.add_argument("--hi", type=float, default=99.99)
    s.add_argument("--max-size", type=lambda x: int(x, 0), default=0x400); s.add_argument("--model", default="hy4")
    s = sub.add_parser("prep"); s.add_argument("wave")
    s = sub.add_parser("wait"); s.add_argument("wave"); s.add_argument("--timeout", type=int, default=5400)
    s = sub.add_parser("collect"); s.add_argument("wave")
    s = sub.add_parser("record"); s.add_argument("wave"); s.add_argument("task")
    s.add_argument("decision", choices=("land", "hold", "drop")); s.add_argument("note", nargs="?", default="")
    s = sub.add_parser("queue"); s.add_argument("wave"); s.add_argument("tasks", nargs="+"); s.add_argument("--out", required=True)
    from tools.coop.land import wave_ops
    wave_ops.add_parsers(sub)
    a = ap.parse_args(argv)
    if a.cmd in wave_ops.COMMANDS:
        return wave_ops.run(a)
    if a.cmd == "census":
        c = census(a.jobs)
        n = sum(len(v) for v in c["units"].values())
        print(f"census: {len(c['units'])} units, {n} functions, {len(c['errors'])} errors -> {CENSUS}")
    elif a.cmd == "select":
        print(_table(select(a.wave, a.n, a.lo, a.hi, a.max_size, a.model),
                     ("task", "model", "pct", "structural", "size", "symbol", "src")))
    elif a.cmd == "prep":
        print(_table(prep(a.wave), ("task", "model_id", "prompt")))
    elif a.cmd == "wait":
        for t, r in wait(a.wave, a.timeout).items():
            print(t, r, sep="\t")
    elif a.cmd == "collect":
        print(_table(collect(a.wave), ("task", "model", "verdict", "lines", "result", "flags")))
    elif a.cmd == "record":
        record(a.wave, a.task, a.decision, a.note)
    elif a.cmd == "queue":
        print(queue(a.wave, a.tasks, a.out))
    return 0

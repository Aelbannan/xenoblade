"""Rejected-shape feedback loop: reviewer holds -> worker briefs -> enforced ban counters.

* The wave reviewer tags every hold ``[shape:<slug>] reason``.  A slug the registry does not know yet
  needs ``/tmp/xb/waves/<W>/shapes/<slug>.md`` (line 1 = one-line description, rest = a short
  example); ``wave decide`` merges it into the tracked registry ``tools/coop/land/shapes.json``.
* ``decide`` archives the owned files' orig/final of every held and landed task under
  ``state/shape_examples/{held,landed}/<W>/<T>/`` (persistent; /tmp/xb is not).
* Tier 1 (automatic, advisory): a shape held at least ADVISORY times is listed in every worker
  pack (``pack_section``).
* Tier 2 (curated, enforced): a shape held at least PROMOTE times across at least 2 waves becomes
  a promotion candidate.  An Opus curator (skill ``xb-rule-curator``) writes a ``bans.py``
  counter and sets ``counter`` in the registry; ``check_counter`` accepts it only if it rises on
  every held example and on no landed example.
"""
from __future__ import annotations

import json
import re
import shutil
from collections import defaultdict
from pathlib import Path

from tools.coop.land import bans, ledger

REGISTRY = Path(__file__).with_name("shapes.json")
EXAMPLES = ledger.STATE / "shape_examples"
HISTORY = ledger.STATE / "wave_history.jsonl"
ADVISORY, PROMOTE, PROMOTE_WAVES = 2, 3, 2
TAG = re.compile(r"^\s*\[shape:([a-z0-9][a-z0-9-]*)\]")


def tag(reason: str) -> str | None:
    m = TAG.match(reason or "")
    return m.group(1) if m else None


def registry() -> dict[str, dict]:
    return json.loads(REGISTRY.read_text()) if REGISTRY.is_file() else {}


def save_registry(reg: dict[str, dict]) -> None:
    REGISTRY.write_text(json.dumps(dict(sorted(reg.items())), indent=1, ensure_ascii=False) + "\n")


def read_new_shape(path: Path) -> dict:
    lines = path.read_text().strip().splitlines()
    return {"desc": lines[0].strip() if lines else "", "example": "\n".join(lines[1:]).strip(), "counter": None}


def archive(kind: str, wave: str, task: str, task_dir: Path) -> None:
    """Copy a task's orig/ and final/ trees (from the wave dir) into the persistent example store."""
    dst = EXAMPLES / kind / wave / task
    for sub in ("orig", "final"):
        if (task_dir / sub).is_dir():
            if (dst / sub).exists():
                shutil.rmtree(dst / sub)
            shutil.copytree(task_dir / sub, dst / sub)


def tally(history: Path | None = None) -> dict[str, dict]:
    out: dict[str, dict] = defaultdict(lambda: {"count": 0, "waves": set(), "tasks": []})
    h = history or HISTORY
    for line in h.read_text().splitlines() if h.is_file() else []:
        r = json.loads(line)
        if r.get("decision") != "hold":
            continue
        s = tag(r.get("note", ""))
        if s:
            t = out[s]
            t["count"] += 1
            t["waves"].add(r["wave"].rstrip("r"))
            t["tasks"].append(f"{r['wave']}/{r['task']}")
    return dict(out)


def status(slug: str, t: dict, reg: dict) -> str:
    e = reg.get(slug) or {}
    if e.get("counter"):
        return "enforced"
    if e.get("enforceable") is False:  # curator declined: stays pack advice only
        return "advisory" if t["count"] >= ADVISORY else "new"
    if t["count"] >= PROMOTE and len(t["waves"]) >= PROMOTE_WAVES:
        return "promote"
    return "advisory" if t["count"] >= ADVISORY else "new"


def pack_section() -> str:
    reg, tal = registry(), tally()
    rows = []
    for slug, t in sorted(tal.items(), key=lambda kv: -kv[1]["count"]):
        if status(slug, t, reg) == "new" or slug not in reg:
            continue
        e = reg[slug]
        ex = f"\n  e.g. `{e['example'].splitlines()[0].strip()}`" if e.get("example") else ""
        rows.append(f"- **{slug}** (rejected {t['count']}x): {e['desc']}{ex}")
    if not rows:
        return ""
    return ("## Recently rejected shapes (reviewer holds; do not produce these)\n\n"
            + "\n".join(rows) + "\n\n")


def _rise(key: str, orig: Path, final: Path) -> int:
    rise = 0
    for f in final.rglob("*") if final.is_dir() else []:
        if f.is_file():
            o = orig / f.relative_to(final)
            before = bans.count(o.read_text(errors="replace")).get(key, 0) if o.is_file() else 0
            rise += bans.count(f.read_text(errors="replace")).get(key, 0) - before
    return rise


def examples(kind: str, tasks: list[str] | None = None) -> list[Path]:
    root = EXAMPLES / kind
    dirs = sorted(p for p in root.glob("*/*") if p.is_dir()) if root.is_dir() else []
    return [d for d in dirs if tasks is None or f"{d.parent.name}/{d.name}" in tasks]


def check_counter(slug: str) -> list[str]:
    reg = registry()
    e = reg.get(slug)
    if not e:
        return [f"{slug}: not in {REGISTRY.name}"]
    key = e.get("counter")
    if not key:
        return [f"{slug}: registry has no 'counter' set"]
    probe = bans.count("")
    if key not in probe:
        return [f"{slug}: counter {key!r} is not a bans.py counter"]
    errs = []
    held = examples("held", tally().get(slug, {}).get("tasks", []))
    if not held:
        errs.append(f"{slug}: no archived held examples")
    for d in held:
        if _rise(key, d / "orig", d / "final") <= 0:
            errs.append(f"{slug}: counter {key} does not rise on held example {d.parent.name}/{d.name}")
    for d in examples("landed"):
        if _rise(key, d / "orig", d / "final") > 0:
            errs.append(f"{slug}: counter {key} rises on LANDED example {d.parent.name}/{d.name} (false positive)")
    return errs

"""Queue runner: apply plans in conflict-free groups, judge each group with the
whole-tree land gate, bisect failing groups down to single plans.

Queue line: ``TASK<TAB>RENAMES<TAB>FLAGS[<TAB>PRESTEP[<TAB>ALLOW]]``

* RENAMES  space-separated ``old=new`` (applied with rename_syms.py)
* FLAGS    ``;``-separated ``SYM@0xADDR|flags`` (rename_syms.py --flags)
* PRESTEP  path to a bash script declaring ``# TOUCHES: files...``
* ALLOW    ``;``-separated allowances ``unit hard:<category> +N -- reason``

The patch is ``<plans>/<TASK>.patch`` (-p0).  Nothing in a plan's .md is
trusted: only the measured snapshots decide.
"""
from __future__ import annotations

import fcntl
import os
import re
import shutil
import subprocess
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path

from tools.coop.land import compare as cmp
from tools.coop.land import ledger, snapshot

ROOT = Path(__file__).resolve().parents[3]
PY = sys.executable
BUILD_LOCK = ROOT / "build" / "us" / ".hexdiff.lock"
APPLIER_LOCK = ledger.STATE / "applier.lock"
SYMF = ("config/us/symbols.txt", "config/eu/symbols.txt", "config/jp/symbols.txt", "tools/coop/targets.json")
ADDR_RE = re.compile(r"0x([0-9A-Fa-f]{8})|(?:lbl|fn|func)_(?:eu_|jp_)?([0-9A-Fa-f]{8})(?![0-9A-Fa-f])")
_LOG = None


def log(*a) -> None:
    line = " ".join(str(x) for x in a)
    print(line, flush=True)
    if _LOG:
        with open(_LOG, "a") as fh:
            fh.write(line + "\n")


@dataclass
class Plan:
    task: str
    renames: list[str] = field(default_factory=list)
    flags: list[str] = field(default_factory=list)
    prestep: str = ""
    allow: list[cmp.Allowance] = field(default_factory=list)
    patch: str = ""
    files: set[str] = field(default_factory=set)
    syms: set[str] = field(default_factory=set)

    def conflict_keys(self) -> set[str]:
        return {"F:" + f for f in self.files if f not in SYMF} | {"S:" + s for s in self.syms}

    def rename_map(self) -> dict[str, str]:
        return dict(r.split("=", 1) for r in self.renames if "=" in r)


def sym_keys(renames, flags, extra=()) -> set[str]:
    keys: set[str] = set()

    def add(tok: str) -> None:
        tok = tok.strip()
        if not tok:
            return
        keys.add(tok)
        for m in ADDR_RE.finditer(tok):
            keys.add("0x" + (m.group(1) or m.group(2)).upper())
    for r in renames:
        for side in r.split("=", 1):
            add(side)
    for fl in flags:
        sym = fl.split("|", 1)[0].strip()
        add(sym)
        if "@0x" in sym:
            add(sym.rsplit("@0x", 1)[0])
    for e in extra:
        add(e)
    # bare per-unit pool names (@stringBase0, @9587) only conflict via address
    return {k for k in keys if not (k.startswith("@") and "@0x" not in k)}


def parse_queue(text: str, plans_dir: str) -> list[Plan]:
    plans = []
    for ln, line in enumerate(text.splitlines(), 1):
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        parts = line.rstrip("\n").split("\t")
        if len(parts) < 3:
            raise SystemExit(f"queue line {ln}: need TASK<TAB>RENAMES<TAB>FLAGS[<TAB>PRESTEP[<TAB>ALLOW]]")
        parts += [""] * (5 - len(parts))
        task, ren, fl, pre, al = (p.strip() for p in parts[:5])
        fls = [x for x in fl.split(";") if x.strip()]
        for x in fls:
            if "|" not in x:
                raise SystemExit(f"queue line {ln} ({task}): malformed FLAGS entry {x!r}")
        if pre.lower() in ("none", "-", "n/a"):
            pre = ""
        allows = [cmp.Allowance.parse(x) for x in al.split(";") if x.strip()]
        plans.append(Plan(task, ren.split(), fls, pre, allows, patch=os.path.join(plans_dir, task + ".patch")))
    return plans


def prestep_decl(path: str) -> tuple[list[str], list[str]]:
    touches, syms = [], []
    for line in Path(path).read_text().splitlines():
        m = re.match(r"\s*#\s*TOUCHES:\s*(.*)", line)
        if m:
            touches += m.group(1).split()
        m = re.match(r"\s*#\s*SYMS:\s*(.*)", line)
        if m:
            syms += m.group(1).split()
    return touches, syms


def resolve(plan: Plan, patch_text: str | None = None) -> Plan:
    txt = patch_text if patch_text is not None else Path(plan.patch).read_text()
    pfiles = [l.split()[1] for l in txt.splitlines() if l.startswith("+++") and len(l.split()) > 1]
    pre_files, extra = prestep_decl(plan.prestep) if plan.prestep else ([], [])
    plan.files = set(pfiles) | set(pre_files)
    plan.syms = sym_keys(plan.renames, plan.flags, extra)
    return plan


def group_plans(plans: list[Plan], n: int) -> list[list[Plan]]:
    groups: list[tuple[list[Plan], set[str]]] = []
    for p in plans:
        k = p.conflict_keys()
        for g in groups:
            if len(g[0]) < n and not (g[1] & k):
                g[0].append(p)
                g[1].update(k)
                break
        else:
            groups.append(([p], set(k)))
    return [g[0] for g in groups]


def bisect_apply(group, try_group, on_ok, on_fail_single) -> None:
    ok, reason = try_group(group)
    if ok:
        on_ok(group)
        return
    if len(group) == 1:
        on_fail_single(group[0], reason)
        return
    mid = len(group) // 2
    bisect_apply(group[:mid], try_group, on_ok, on_fail_single)
    bisect_apply(group[mid:], try_group, on_ok, on_fail_single)


def run(cmd, **k) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, **k)


def locked_build(link: bool = False) -> tuple[subprocess.CompletedProcess, dict | None]:
    """Full object build + progress (and optionally the final link) under the
    repo-wide build lock.  Returns (build result, link_gate result or None)."""
    BUILD_LOCK.parent.mkdir(parents=True, exist_ok=True)
    with open(BUILD_LOCK, "a+") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        try:
            b = run([str(ROOT / "ninja"), "-k", "0", "all_source", "progress"])
            res = None
            if link:
                from tools.coop import link_gate
                res, _log, _rc = link_gate.run_link("us")
            return b, res
        finally:
            fcntl.flock(lk, fcntl.LOCK_UN)


def measure(jobs: int) -> tuple[dict, dict[str, str]]:
    """Build, hash the gated tree, snapshot.  Hashes are taken before measuring
    so a concurrent edit is never recorded as having passed."""
    b, link = locked_build(link=True)
    hashes = ledger.tree_hashes()
    snap = snapshot.take(jobs=jobs, build_ok=b.returncode == 0)
    if link is not None:
        snap["link"] = link
    snap["tree"] = ledger.tree_id(hashes)
    if b.returncode:
        snap["build_log"] = "\n".join(l for l in b.stdout.splitlines() if l.startswith("FAILED"))[:2000]
    return snap, hashes


class Repo:
    def __init__(self, jobs: int, cache: Path):
        self.jobs = jobs
        self.cache = cache
        self.base: dict | None = None
        self._snap: dict[str, str | None] | None = None
        self.snapdir = ledger.STATE / "restore"

    def baseline(self) -> dict:
        tid = ledger.tree_id(ledger.tree_hashes())
        keys = list(snapshot.bans.KEYS)
        if self.base is not None and self.base.get("tree") == tid and self.base.get("ban_keys") == keys:
            return self.base
        if self.cache.is_file():
            c = snapshot.load(self.cache)
            # a baseline taken with another ban-counter set is stale: every
            # pre-existing hit of a new counter would read as a rise
            if c.get("tree") == tid and c.get("ban_keys") == keys:
                self.base = c
                return c
        log("   measuring baseline (whole tree)")
        self.base, _ = measure(self.jobs)
        snapshot.save(self.base, self.cache)
        return self.base

    def save_files(self, files) -> dict[str, str | None]:
        if self.snapdir.exists():
            shutil.rmtree(self.snapdir)
        self.snapdir.mkdir(parents=True)
        snap: dict[str, str | None] = {}
        for i, f in enumerate(sorted(files)):
            src = ROOT / f
            if src.exists():
                dst = self.snapdir / f"{i:04d}_{f.replace('/', '__')}"
                shutil.copyfile(src, dst)
                snap[f] = str(dst)
            else:
                snap[f] = None
        self._snap = snap
        return snap

    def restore(self, snap: dict[str, str | None]) -> None:
        for f, s in snap.items():
            p = ROOT / f
            if s is None:
                if p.exists():
                    p.unlink()
            else:
                p.write_bytes(Path(s).read_bytes())

    def try_group(self, group: list[Plan]):
        self._snap = None
        try:
            return self._try_group(group)
        except SystemExit:
            raise
        except BaseException as e:
            if self._snap is not None:
                self.restore(self._snap)
            raise SystemExit(f"ABORT: exception {e!r} in {[p.task for p in group]} (files restored)")

    def _try_group(self, group: list[Plan]):
        log(f"=== TRY group of {len(group)}: {' '.join(p.task for p in group)}")
        base = self.baseline()
        if not base.get("build_ok", True):
            raise SystemExit("ABORT: baseline tree does not build; fix the tree first")
        owned = {f for p in group for f in p.files} | set(SYMF)
        foreign = [f for f in ledger.unledgered() if f not in owned]
        if foreign:
            # Someone else's in-flight edit: measuring now would judge (and
            # ledger) it as part of this group.  Drain/harvest first.
            raise SystemExit(f"ABORT: dirty tree, {len(foreign)} unledgered file(s) outside this group: {foreign[:8]}")
        snap = self.save_files(owned)
        for p in group:
            r = run(["patch", "-p0", "--dry-run", "-i", p.patch])
            if r.returncode:
                log(f"   DRYRUN FAIL {p.task}", r.stdout.strip()[:300])
                self.restore(snap)
                return False, f"dry-run failed {p.task}"
        for p in group:
            r = run(["patch", "-p0", "--no-backup-if-mismatch", "-i", p.patch])
            if r.returncode:
                self.restore(snap)
                return False, f"patch failed {p.task}"
            if p.prestep:
                r = run(["bash", p.prestep])
                log(f"   prestep {p.task}: rc={r.returncode}", r.stderr.strip()[:200])
                if r.returncode:
                    self.restore(snap)
                    return False, f"prestep failed {p.task}"
            if p.renames:
                rr = run([PY, "tools/coop/faithful/rename_syms.py", *p.renames])
                log(f"   renames {p.task}:", rr.stdout.strip().replace("\n", " | ")[:200])
                if rr.returncode:
                    self.restore(snap)
                    return False, f"renames failed {p.task}"
            for fl in p.flags:
                sym, flags = fl.split("|", 1)
                rf = run([PY, "tools/coop/faithful/rename_syms.py", "--flags", sym.strip(), flags.strip()])
                if rf.returncode:
                    self.restore(snap)
                    return False, f"flags failed {p.task}"
        if any(f == "configure.py" or f.startswith("config/") for p in group for f in p.files):
            r = run([PY, "configure.py"])
            if r.returncode:
                self.restore(snap)
                run([PY, "configure.py"])
                return False, "configure.py failed"
        after, hashes = measure(self.jobs)
        renames: dict[str, str] = {}
        for p in group:
            renames.update(p.rename_map())
        verdict = cmp.compare(base, after, renames, [a for p in group for a in p.allow])
        log(cmp.format_verdict(verdict))
        if not verdict.ok:
            self.restore(snap)
            if any(f == "configure.py" or f.startswith("config/") for p in group for f in p.files):
                run([PY, "configure.py"])
            # hard violations keep their category ("hard:U:ban:x +1") so `wave gate-alarm` can count counters
            return False, "; ".join(f"{v.kind}:{v.unit}" + (f":{v.detail}" if v.kind == "hard" else "")
                                    for v in verdict.violations[:6])
        n = ledger.record(hashes, task=",".join(p.task for p in group))
        log(f"   ledger: +{n} file state(s)")
        self.base = after
        snapshot.save(after, self.cache)
        return True, None


def main(argv=None) -> int:
    import argparse
    global _LOG
    ap = argparse.ArgumentParser(prog="land apply")
    ap.add_argument("queue")
    ap.add_argument("--plans", default="/tmp/xb/plans")
    ap.add_argument("--group", type=int, default=8)
    ap.add_argument("--jobs", type=int, default=8)
    ap.add_argument("--log")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args(argv)
    _LOG = a.log
    plans = parse_queue(Path(a.queue).read_text(), a.plans)
    missing = [p.task for p in plans if not os.path.exists(p.patch)]
    if missing:
        raise SystemExit(f"missing patches: {missing}")
    for p in plans:
        resolve(p)
    groups = group_plans(plans, a.group)
    for i, g in enumerate(groups):
        log(f"GROUP {i} ({len(g)}): " + " ".join(p.task for p in g))
    if a.dry_run:
        for p in plans:
            r = run(["patch", "-p0", "--dry-run", "-i", p.patch])
            log(f"   dry-run {p.task}: {'ok' if r.returncode == 0 else 'FAIL'}")
        log(f"DRY RUN: {len(plans)} plans in {len(groups)} groups")
        return 0
    APPLIER_LOCK.parent.mkdir(parents=True, exist_ok=True)
    lk = open(APPLIER_LOCK, "a+")
    fcntl.flock(lk, fcntl.LOCK_EX)
    repo = Repo(a.jobs, ledger.STATE / "last_snapshot.json")
    ok: list[str] = []
    rev: list[str] = []
    for g in groups:
        bisect_apply(g, repo.try_group,
                     lambda gg: [ok.append(p.task) or log(f"APPLIED OK {p.task}") for p in gg],
                     lambda p, r: (rev.append(p.task), log(f"REVERTED {p.task}: {r}")))
    log(f"BATCH DONE ok={len(ok)} reverted={len(rev)} {time.strftime('%H:%M')}")
    return 0

"""CLI for the land gate.  Run from the repo root:

  .venv/bin/python3 -m tools.coop.land snapshot [--out F] [--build]
  .venv/bin/python3 -m tools.coop.land compare BEFORE AFTER [--rename old=new ...] [--allow '...']
  .venv/bin/python3 -m tools.coop.land check --task T [--baseline F] [--rename ...] [--allow '...']
        hand edits: judge the current tree against the cached/last-passing snapshot,
        record the ledger on PASS (exit 1 on REJECT)
  .venv/bin/python3 -m tools.coop.land apply QUEUE [--plans DIR] [--log F]
  .venv/bin/python3 -m tools.coop.land adopt --task T --reason '...'
        one-time bootstrap: record the current tree as passed without measuring
  .venv/bin/python3 -m tools.coop.land verify-staged      (pre-commit hook)
  .venv/bin/python3 -m tools.coop.land install-hook
"""
from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path

from tools.coop.land import compare as cmp
from tools.coop.land import ledger, runner, snapshot

CACHE = ledger.STATE / "last_snapshot.json"


def _renames(items) -> dict[str, str]:
    return dict(x.split("=", 1) for x in items or [])


def cmd_snapshot(a) -> int:
    if a.build:
        snap, _ = runner.measure(a.jobs)
    else:
        snap = snapshot.take(jobs=a.jobs)
        snap["tree"] = ledger.tree_id(ledger.tree_hashes())
    snapshot.save(snap, Path(a.out))
    print(f"snapshot: {len(snap['units'])} units, {len(snap['errors'])} error(s) -> {a.out}")
    return 0


def cmd_compare(a) -> int:
    v = cmp.compare(snapshot.load(a.before), snapshot.load(a.after), _renames(a.rename),
                    [cmp.Allowance.parse(x) for x in a.allow or []])
    print(cmp.format_verdict(v))
    return 0 if v.ok else 1


def cmd_check(a) -> int:
    base_path = Path(a.baseline) if a.baseline else CACHE
    if not base_path.is_file():
        print(f"no baseline snapshot at {base_path}: take one before editing "
              "(snapshot --build --out ...), or run apply once", file=sys.stderr)
        return 2
    base = snapshot.load(base_path)
    pending = ledger.unledgered()
    print(f"check {a.task}: {len(pending)} unledgered file(s) will be judged: {pending[:20]}")
    if a.only and set(pending) - set(a.only):
        print(f"ABORT: unledgered files outside --only: {sorted(set(pending) - set(a.only))[:20]}", file=sys.stderr)
        return 2
    after, hashes = runner.measure(a.jobs)
    v = cmp.compare(base, after, _renames(a.rename), [cmp.Allowance.parse(x) for x in a.allow or []])
    print(cmp.format_verdict(v))
    if not v.ok:
        return 1
    n = ledger.record(hashes, task=a.task, kind="check",
                      reason="; ".join(f"{x.unit} {x.kind}:{x.key} +{x.amount} -- {x.reason}" for x in v.used_allowances))
    snapshot.save(after, CACHE)
    print(f"ledger: +{n} file state(s); baseline cache updated")
    return 0


def cmd_adopt(a) -> int:
    if not a.reason.strip():
        print("adopt needs --reason", file=sys.stderr)
        return 2
    n = ledger.record(ledger.tree_hashes(), task=a.task, kind="adopt", reason=a.reason)
    print(f"adopted {n} file state(s) without measurement (task {a.task}: {a.reason})")
    return 0


def _git(*args: str) -> subprocess.CompletedProcess:
    # Only ever invoked by the human's own `git commit` (pre-commit hook).
    return subprocess.run(["git", *args], cwd=ledger.ROOT, capture_output=True)


def cmd_verify_staged(a) -> int:
    r = _git("diff", "--cached", "--name-status", "-z", "--no-renames")
    parts = r.stdout.decode().split("\0")
    pairs: list[tuple[str, str | None]] = []
    for st, path in zip(parts[0::2], parts[1::2]):
        if not path or not ledger.is_gated(path):
            continue
        if st.startswith("D"):
            pairs.append((path, None))
            continue
        blob = _git("rev-parse", f":{path}").stdout.decode().strip()
        pairs.append((path, blob))
    bad = ledger.verify(pairs)
    if bad:
        print("land gate: these staged files never passed the gate "
              "(run `python -m tools.coop.land check --task ...` or land them via `apply`):")
        for p in bad[:50]:
            print("  " + p)
        if len(bad) > 50:
            print(f"  ... and {len(bad) - 50} more")
        return 1
    print(f"land gate: {len(pairs)} staged gated file(s) verified")
    return 0


HOOK = """#!/bin/sh
# installed by tools/coop/land (land gate): refuse staged gated files that never passed the gate
cd "$(git rev-parse --show-toplevel)" || exit 1
exec .venv/bin/python3 -m tools.coop.land verify-staged
"""


def cmd_install_hook(a) -> int:
    gitdir = ledger.ROOT / ".git"
    hook = gitdir / "hooks" / "pre-commit"
    if hook.exists() and "land gate" not in hook.read_text(errors="replace"):
        print(f"{hook} exists and is not ours; not overwriting", file=sys.stderr)
        return 1
    hook.parent.mkdir(parents=True, exist_ok=True)
    hook.write_text(HOOK)
    hook.chmod(0o755)
    print(f"installed {hook}")
    return 0


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(prog="python -m tools.coop.land")
    sub = ap.add_subparsers(dest="cmd", required=True)
    s = sub.add_parser("snapshot"); s.add_argument("--out", default=str(CACHE)); s.add_argument("--build", action="store_true"); s.add_argument("--jobs", type=int, default=8)
    c = sub.add_parser("compare"); c.add_argument("before"); c.add_argument("after"); c.add_argument("--rename", nargs="*"); c.add_argument("--allow", action="append")
    k = sub.add_parser("check"); k.add_argument("--task", required=True); k.add_argument("--baseline"); k.add_argument("--rename", nargs="*"); k.add_argument("--allow", action="append"); k.add_argument("--jobs", type=int, default=8); k.add_argument("--only", nargs="*", help="refuse unless every unledgered file is one of these")
    sub.add_parser("apply", add_help=False)
    sub.add_parser("pack", add_help=False)
    d = sub.add_parser("adopt"); d.add_argument("--task", required=True); d.add_argument("--reason", required=True)
    sub.add_parser("verify-staged")
    sub.add_parser("install-hook")
    argv = sys.argv[1:] if argv is None else argv
    if argv and argv[0] in ("check", "apply", "adopt") and os.environ.get("XB_LAND_OWNER") != "1":
        # These judge and ledger the WHOLE tree; a worker agent running one
        # mid-wave would bless (or be blamed for) other agents' edits.
        print(f"land {argv[0]}: orchestrator only (set XB_LAND_OWNER=1)", file=sys.stderr)
        return 2
    if argv and argv[0] == "apply":
        return runner.main(argv[1:])
    if argv and argv[0] == "pack":
        from tools.coop.land import pack
        return pack.main(argv[1:])
    a = ap.parse_args(argv)
    return {"snapshot": cmd_snapshot, "compare": cmd_compare, "check": cmd_check, "adopt": cmd_adopt,
            "verify-staged": cmd_verify_staged, "install-hook": cmd_install_hook}[a.cmd](a)


if __name__ == "__main__":
    raise SystemExit(main())

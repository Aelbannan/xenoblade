#!/usr/bin/env python3
"""Scaffold remaining missing-region targets in batches until none remain.

Usage:
  python3 tools/coop/scaffold_all_loop.py
  python3 tools/coop/scaffold_all_loop.py --batch 10 --max-rounds 80
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.project import Project
from tools.coop.lib.targets import load_targets
from tools.coop.scaffold_missing_slots import begin_marker, collect_plans
from tools.ppc_equivalence.elf_symbols import list_text_functions

HARNESS_BLOCK_RE = re.compile(
    r"\n?// LLM-HARNESS-BEGIN: [^\n]+\n.*?// LLM-HARNESS-END: [^\n]+\n",
    re.S,
)


def strip_harness(path: Path) -> int:
    text = path.read_text(encoding="utf-8")
    new, n = HARNESS_BLOCK_RE.subn("", text)
    if n:
        path.write_text(new, encoding="utf-8")
    return n


def remaining_by_tu(cfg, project, exclude: set[str]):
    pool = [
        t
        for t in load_targets(cfg)
        if t.status == "NOT_STARTED"
        and t.kind == "function"
        and t.symbol
        and t.source
        and t.source.is_file()
    ]
    plans, skips = collect_plans(
        pool,
        project=project,
        project_root=cfg.project_root,
        include_header_inline=False,
        skip_sinit=True,
        allow_undeclared=False,
        allow_foreign_class=False,
        tu=None,
    )
    by: dict[str, list] = defaultdict(list)
    for p in plans:
        u = (p.unit or "").removeprefix("main/")
        if u in exclude:
            continue
        by[u].append(p)
    return by, plans, skips


def source_for_tu(targets, tu: str) -> Path | None:
    stem = Path(tu).name
    for t in targets:
        if t.source and t.source.exists() and t.source.stem == stem:
            try:
                text = t.source.read_text(encoding="utf-8")
            except OSError:
                continue
            if "LLM-HARNESS-BEGIN" in text:
                return t.source
    return None


def verify_tu(cfg, project, targets, tu: str) -> dict:
    r = subprocess.run(
        [sys.executable, "tools/coop/run.py", "size", tu],
        capture_output=True,
        text=True,
        cwd=str(ROOT),
    )
    out = r.stdout + r.stderr
    compile_fail = "ninja: build stopped" in out or "ERROR: command failed" in out
    size_line = next((ln for ln in out.splitlines() if ln.startswith("size:")), "")
    if compile_fail:
        errs = []
        for ln in out.splitlines():
            if "Error" in ln or re.search(r"#\s+\d+:", ln):
                errs.append(ln)
                if "Error" in ln:
                    break
        return {
            "ok": False,
            "compile": False,
            "hit": 0,
            "miss": 0,
            "errs": errs,
            "size": size_line,
            "out": out[-2000:],
        }

    u = project.resolve_unit(tu)
    decomp = {fn.name for fn in list_text_functions(u.base_path)}
    src = source_for_tu(targets, tu)
    if not src:
        return {
            "ok": False,
            "compile": True,
            "hit": 0,
            "miss": 0,
            "errs": ["no src"],
            "size": size_line,
        }

    text = src.read_text(encoding="utf-8")
    hit = miss = 0
    misses: list[str] = []
    for t in targets:
        if begin_marker(t.id) not in text:
            continue
        if t.symbol in decomp:
            hit += 1
        else:
            miss += 1
            misses.append(t.symbol)
    return {
        "ok": miss == 0,
        "compile": True,
        "hit": hit,
        "miss": miss,
        "misses": misses,
        "errs": [],
        "size": size_line,
        "src": src,
    }


def write_tu(tu: str) -> str:
    r = subprocess.run(
        [
            sys.executable,
            "tools/coop/scaffold_missing_slots.py",
            "--tu",
            tu,
            "--selection",
            "all",
            "--write",
        ],
        capture_output=True,
        text=True,
        cwd=str(ROOT),
    )
    summary = next(
        (
            ln
            for ln in r.stdout.splitlines()
            if ln.startswith("pool=") or ln.startswith("updated")
        ),
        r.stdout.splitlines()[0] if r.stdout else "no-output",
    )
    return summary


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--batch", type=int, default=10)
    parser.add_argument("--max-rounds", type=int, default=80)
    parser.add_argument(
        "--exclude",
        action="append",
        default=[],
        help="TU to exclude (repeatable)",
    )
    args = parser.parse_args(argv)

    cfg = load_config(None, ROOT)
    project = Project(cfg)
    targets = load_targets(cfg)
    exclude = set(args.exclude)
    round_no = 0
    tus_ok = 0
    stubs_ok = 0

    while round_no < args.max_rounds:
        by, plans, _skips = remaining_by_tu(cfg, project, exclude)
        remaining = sorted(by.items(), key=lambda kv: -len(kv[1]))
        print(
            f"\n===== ROUND {round_no + 1}: "
            f"{sum(len(v) for v in by.values())} stubs across {len(remaining)} TUs "
            f"(exclude={len(exclude)}) =====",
            flush=True,
        )
        if not remaining:
            print("DONE: no remaining scaffoldable plans", flush=True)
            break

        batch = [u for u, _ in remaining[: args.batch]]
        print(
            "batch: " + ", ".join(f"{u}({len(by[u])})" for u in batch),
            flush=True,
        )

        for tu in batch:
            summary = write_tu(tu)
            print(f"  WRITE {tu}: {summary}", flush=True)

        failed: list[tuple[str, dict]] = []
        for tu in batch:
            res = verify_tu(cfg, project, targets, tu)
            st = "ok" if res["ok"] else ("COMPILE" if not res["compile"] else "MISS")
            print(
                f"  CHECK {st} hit={res['hit']} miss={res['miss']} {tu} "
                f"{res['size'][:55]}",
                flush=True,
            )
            if res["ok"]:
                tus_ok += 1
                stubs_ok += res["hit"]
            else:
                failed.append((tu, res))

        for tu, res in failed:
            src = res.get("src") or source_for_tu(targets, tu)
            if src is None:
                # try resolve from unit source path conventions
                for cand in (
                    ROOT / "src" / f"{tu}.cpp",
                    ROOT / "libs" / f"{tu}.cpp",
                    ROOT / "libs" / f"{tu}.c",
                ):
                    if cand.exists():
                        src = cand
                        break
            if src and src.exists():
                n = strip_harness(src)
                print(f"  RETRY strip {n} from {src}", flush=True)
                print(f"  RETRY write {tu}: {write_tu(tu)}", flush=True)
                res2 = verify_tu(cfg, project, targets, tu)
                st = "ok" if res2["ok"] else ("COMPILE" if not res2["compile"] else "MISS")
                print(
                    f"  RETRY CHECK {st} hit={res2['hit']} miss={res2['miss']} {tu}",
                    flush=True,
                )
                if res2["ok"]:
                    tus_ok += 1
                    stubs_ok += res2["hit"]
                    continue
                print(f"  EXCLUDE {tu}: {res2.get('errs') or res2.get('misses', [])[:3]}", flush=True)
                exclude.add(tu)
            else:
                print(f"  EXCLUDE {tu}: no source to retry", flush=True)
                exclude.add(tu)

        round_no += 1
    else:
        print("HIT MAX_ROUNDS", flush=True)

    by, plans, _ = remaining_by_tu(cfg, project, exclude)
    print(
        f"\nprogress: rounds={round_no} tus_ok={tus_ok} stubs_checked≈{stubs_ok}",
        flush=True,
    )
    print(
        f"remaining: stubs={sum(len(v) for v in by.values())} tus={len(by)} "
        f"excluded={sorted(exclude)}",
        flush=True,
    )
    for u, ps in sorted(by.items(), key=lambda kv: -len(kv[1]))[:20]:
        print(f"  {len(ps):4d}  {u}", flush=True)
    return 0 if not by else 1


if __name__ == "__main__":
    raise SystemExit(main())

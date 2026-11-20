#!/usr/bin/env python3
"""Lock-free probe compile + diff for one unit.

Compiles a unit with its exact MWCC command line (taken from the ninja build
graph) into a PRIVATE scratch directory, then runs hexdiff on that object via
``--decomp-obj``. It takes no repo build lock and never writes under build/,
so any number of investigators can probe at once without queueing behind
implementers' hexdiff runs or link-gates.

Typical uses:

  # probe the working-tree source as-is
  .venv/bin/python3 tools/coop/probe.py kyoshin/menu/CMenuGetItemMulti --all

  # probe a scratch variant WITHOUT touching the shared tree. The copy must
  # keep the original basename (MWCC derives the object name and
  # @unnamed@<file>@ namespaces from it).
  .venv/bin/python3 tools/coop/probe.py kyoshin/menu/CMenuGetItemMulti \\
      --source /tmp/xb/ME/v1/CMenuGetItemMulti.cpp --symbol Foo__Fv

  # also shadow headers with scratch variants (searched before the repo).
  # The overlay MIRRORS include paths: a header included as
  # "kyoshin/menu/CMenuX.hpp" goes at /tmp/xb/ME/hdr/kyoshin/menu/CMenuX.hpp
  ... --include-first /tmp/xb/ME/hdr

Everything after the probe options is passed to hexdiff (--symbol/--all/
--brief/--asm/--json/--relocs/...). Probe results are evidence for a plan;
landing a change still needs a real hexdiff run (with the lock) on the edited
tree.
"""
from __future__ import annotations

import argparse
import json
import os
import shlex
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
PROBE_ROOT = Path("/tmp/xb/probe")


def _resolve_unit(hint: str) -> dict:
    units = json.loads((REPO / "objdiff.json").read_text())["units"]
    h = hint.strip().removeprefix("main/")
    exact = [u for u in units if u["name"] in (hint, f"main/{h}")]
    if exact:
        return exact[0]
    by_src = [u for u in units
              if (u.get("metadata") or {}).get("source_path") in (hint, h)]
    if by_src:
        return by_src[0]
    tail = [u for u in units if u["name"].endswith("/" + h.split("/")[-1])]
    if len(tail) == 1:
        return tail[0]
    names = ", ".join(u["name"] for u in tail[:8])
    raise SystemExit(f"probe: cannot resolve unit {hint!r}" + (f" (candidates: {names})" if tail else ""))


def _compile_command(base_path: str) -> list[str]:
    """The MWCC part of ninja's command for *base_path* (read-only query)."""
    ninja = REPO / "ninja"
    exe = str(ninja) if ninja.is_file() else "ninja"
    proc = subprocess.run([exe, "-t", "commands", base_path], cwd=REPO,
                          capture_output=True, text=True, check=False)
    if proc.returncode != 0 or not proc.stdout.strip():
        raise SystemExit(f"probe: ninja -t commands failed for {base_path}: {proc.stderr.strip()}")
    line = proc.stdout.strip().splitlines()[-1]
    first = line.split(" && ")[0]
    argv = shlex.split(first)
    if not any("mwcceppc" in a for a in argv):
        raise SystemExit(f"probe: last command for {base_path} is not an MWCC compile:\n{first}")
    return argv


def _rewrite(argv: list[str], source: Path | None, out_dir: Path,
             include_first: list[Path]) -> tuple[list[str], str]:
    out = list(argv)
    try:
        ci = out.index("-c")
        oi = out.index("-o")
    except ValueError:
        raise SystemExit("probe: compile command has no -c/-o operands")
    orig_src = out[ci + 1]
    if source is not None:
        if source.name != Path(orig_src).name:
            raise SystemExit(f"probe: --source basename must be {Path(orig_src).name!r} "
                             f"(MWCC names the object and @unnamed@ namespaces after it)")
        out[ci + 1] = str(source)
    out[oi + 1] = str(out_dir)
    # Search order: scratch header overlays, then the original source dir
    # (so a relocated --source still finds its sibling headers), then the
    # unit's normal include paths.
    extra: list[str] = []
    for d in include_first:
        extra += ["-i", str(d)]
    if source is not None:
        extra += ["-i", str((REPO / orig_src).parent)]
    if extra:
        first_i = out.index("-i") if "-i" in out else ci
        out[first_i:first_i] = extra
    return out, orig_src


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 epilog=__doc__.split("\n\n", 1)[1])
    ap.add_argument("unit", help="objdiff unit (kyoshin/menu/CMenuX) or source path")
    ap.add_argument("--source", type=Path, help="scratch copy of the unit's source to compile instead")
    ap.add_argument("--include-first", type=Path, action="append", default=[],
                    help="directory searched before the repo include paths (repeatable)")
    ap.add_argument("--keep", action="store_true", help="keep the probe directory (path printed)")
    ap.add_argument("--print-command", action="store_true", help="print the rewritten MWCC command")
    args, hexdiff_args = ap.parse_known_args(argv)

    unit = _resolve_unit(args.unit)
    source = args.source.expanduser().resolve() if args.source else None
    if source is not None and not source.is_file():
        raise SystemExit(f"probe: --source not found: {source}")
    try:
        if source is not None and source.is_relative_to(REPO):
            raise SystemExit("probe: --source must be a scratch copy outside the repo "
                             "(omit --source to probe the working tree)")
    except AttributeError:
        pass

    PROBE_ROOT.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix=Path(unit["name"]).name + "-", dir=PROBE_ROOT))
    try:
        cmd = _compile_command(unit["base_path"])
        cmd, orig_src = _rewrite(cmd, source, work, [p.expanduser().resolve() for p in args.include_first])
        if args.print_command:
            print(shlex.join(cmd), file=sys.stderr)
        proc = subprocess.run(cmd, cwd=REPO, capture_output=True, text=True, check=False)
        obj = work / (Path(orig_src).stem + ".o")
        if proc.returncode != 0 or not obj.is_file():
            sys.stderr.write(proc.stdout + proc.stderr)
            print(f"probe: compile FAILED for {unit['name']}", file=sys.stderr)
            return 2
        # Same post-compile step hexdiff applies to its own builds.
        notesplit = REPO / "tools" / "postprocess_notesplit.py"
        if notesplit.is_file():
            subprocess.run([sys.executable, str(notesplit), str(obj)], cwd=REPO,
                           capture_output=True, check=False)
        hx = [sys.executable, str(REPO / "tools" / "coop" / "hexdiff.py"), unit["name"],
              "--decomp-obj", str(obj), *hexdiff_args]
        print(f"probe: {unit['name']} -> {obj} (no build lock)", file=sys.stderr)
        return subprocess.run(hx, cwd=REPO, check=False).returncode
    finally:
        if args.keep:
            print(f"probe: kept {work}", file=sys.stderr)
        else:
            shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    raise SystemExit(main())

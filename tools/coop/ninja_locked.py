"""Serialise every ninja process in this build tree.

Concurrent ninja processes (builds, and even ``-t``/``-n`` tool calls, which
"recover" a log mid-write by truncating it) corrupt ``.ninja_deps``.  Once
the deps log is truncated every object loses its dependency record and is
rebuilt on every invocation.  This wrapper takes an exclusive flock on
``build/.ninja.lock`` around the real binary.

Installed as ``.venv/bin/ninja`` (the repo's ``./ninja`` symlink points there)
with the real binary moved to ``.venv/bin/ninja-real``:

    .venv/bin/python3 tools/coop/ninja_locked.py --install

Lock order (never inverted, so no deadlock): ``build/us/.hexdiff.lock`` is
always taken before this lock.
"""
from __future__ import annotations

import fcntl
import os
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
REAL = ROOT / ".venv" / "bin" / "ninja-real"
LOCK = ROOT / "build" / ".ninja.lock"

STUB = """#!{py}
# land gate: serialised ninja (see tools/coop/ninja_locked.py)
import runpy, sys
sys.argv[0] = "ninja"
runpy.run_path({wrapper!r}, run_name="__main__")
"""


def install() -> int:
    venv_ninja = ROOT / ".venv" / "bin" / "ninja"
    if not REAL.exists():
        if not venv_ninja.exists():
            print("no .venv/bin/ninja to wrap", file=sys.stderr)
            return 1
        venv_ninja.rename(REAL)
    venv_ninja.write_text(STUB.format(py=ROOT / ".venv" / "bin" / "python3", wrapper=str(Path(__file__).resolve())))
    venv_ninja.chmod(0o755)
    print(f"installed lock wrapper at {venv_ninja} (real binary: {REAL})")
    return 0


def main() -> int:
    if sys.argv[1:2] == ["--install"]:
        return install()
    LOCK.parent.mkdir(parents=True, exist_ok=True)
    fd = os.open(LOCK, os.O_RDWR | os.O_CREAT, 0o644)
    fcntl.flock(fd, fcntl.LOCK_EX)
    # The lock fd stays open (and held) across exec into the real binary,
    # so it is released only when ninja itself exits.
    os.set_inheritable(fd, True)
    os.execv(str(REAL), [str(REAL), *sys.argv[1:]])
    return 127


if __name__ == "__main__":
    raise SystemExit(main())

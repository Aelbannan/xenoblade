#!/usr/bin/env python3
from __future__ import annotations

import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TOOL_ROOT = Path(__file__).resolve().parent
VENV_ROOT = TOOL_ROOT / ".venv"


def _venv_python() -> Path:
    if os.name == "nt":
        return VENV_ROOT / "Scripts" / "python.exe"
    return VENV_ROOT / "bin" / "python"


def _install_dependencies() -> int:
    if not _venv_python().is_file():
        subprocess.run([sys.executable, "-m", "venv", str(VENV_ROOT)], check=True)
    subprocess.run(
        [str(_venv_python()), "-m", "pip", "install", "-r", str(TOOL_ROOT / "requirements.txt")],
        cwd=ROOT,
        check=True,
    )
    print(f"ppc_equivalence dependencies installed in {VENV_ROOT}")
    return 0


if sys.argv[1:] == ["--install-deps"]:
    raise SystemExit(_install_dependencies())

# A repository-local environment makes the proof dependencies available to
# both this entry point and `tools/coop/run.py equivalence ...` without asking
# callers to activate a shell environment first.
venv_python = _venv_python()
if venv_python.is_file() and Path(sys.prefix).resolve() != VENV_ROOT.resolve():
    os.execv(str(venv_python), [str(venv_python), str(Path(__file__).resolve()), *sys.argv[1:]])

if sys.argv[1:] == ["--self-test"]:
    raise SystemExit(
        subprocess.run(
            [sys.executable, "-m", "unittest", "discover", "-s", "tools/ppc_equivalence/tests", "-v"],
            cwd=ROOT,
            check=False,
        ).returncode
    )

if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.ppc_equivalence.cli import main


if __name__ == "__main__":
    raise SystemExit(main())

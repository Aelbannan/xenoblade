"""Shared paths, config and small utilities for the paseo harness."""
import json
import os
import re
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

HERE = Path(__file__).resolve().parent          # tools/paseo-harness/lib
HARNESS = HERE.parent                           # tools/paseo-harness
ROOT = HARNESS.parent.parent                    # repo root
TARGETS = ROOT / "tools" / "coop" / "targets.json"

VENV_PY = ROOT / ".venv" / "bin" / "python3"
if not VENV_PY.exists():
    VENV_PY = Path(sys.executable)

DEFAULT_PASEO = [
    "/Applications/Paseo.app/Contents/Resources/bin/paseo",
    str(Path.home() / ".local" / "bin" / "paseo"),
]


def paseo_bin():
    env = os.environ.get("PASEO")
    if env:
        return env
    for c in DEFAULT_PASEO:
        if Path(c).exists():
            return c
    return "paseo"  # let PATH resolve it


def state_root():
    env = os.environ.get("PASEO_HARNESS_STATE")
    return Path(env) if env else Path.home() / ".paseo" / "harness"


def state_dir(section):
    return state_root() / section


def slug(s):
    return re.sub(r"[^a-z0-9-]+", "-", str(s).lower()).strip("-") or "s"


def now_iso():
    return datetime.now(timezone.utc).isoformat(timespec="seconds")


def parse_iso(s):
    if not s:
        return None
    try:
        return datetime.fromisoformat(str(s).replace("Z", "+00:00"))
    except Exception:
        return None


def load_json(path):
    try:
        return json.load(open(path))
    except Exception:
        return None


def save_json(path, obj):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = str(path) + ".tmp"
    json.dump(obj, open(tmp, "w"), indent=1)
    os.replace(tmp, path)


def sh(cmd, cwd=None, timeout=120, quiet=True):
    """Run a command, return CompletedProcess with text streams."""
    r = subprocess.run(
        [str(c) for c in cmd],
        capture_output=True,
        text=True,
        cwd=str(cwd) if cwd else None,
        timeout=timeout,
    )
    return r


def utc_age_minutes(ts_iso):
    """Minutes since an ISO-8601 UTC timestamp (None if unparseable)."""
    ts = parse_iso(ts_iso)
    if ts is None:
        return None
    return (datetime.now(timezone.utc) - ts).total_seconds() / 60.0

"""Worktree build-cache helpers for the LLM harness.

## Why ninja re-runs SPLIT in every worktree

``XenobladeAdapter.prepare_workspace`` already symlinks shared retail outputs:

- ``build/<region>/config.json``
- ``build/<region>/obj``
- ``build/<region>/include``
- ``build/<region>/asm``

It then runs ``configure.py``, which regenerates a worktree-local ``build.ninja``.
That graph still contains the decomp-toolkit edge:

```text
build build/<region>/config.json: split config/<region>/config.yml | dtk
```

Ninja may still treat ``config.json`` as dirty (symlink mtime vs ``config.yml``,
missing restat, or depfile pressure). Re-running ``dtk dol split`` is expensive
and racy under ``target_parallel`` because a symlink write lands in the shared
main build tree.

## Safe stamp (this module)

After configure succeeds, replace the ``config.json`` *symlink* with a regular
file copy whose mtime is newer than ``config.yml``. Keep ``obj`` / ``include`` /
``asm`` as symlinks. Ninja then skips SPLIT while still compiling only the
candidate object under ``build/<region>/src``.

Call from ``prepare_workspace`` after configure:

```python
from tools.llm_harness.eval_cache import stamp_split_config

stamp_split_config(workspace, self.config.region)
```

Do **not** delete or rewrite shared ``obj`` / ``asm`` from worktrees.
"""

from __future__ import annotations

import os
import shutil
import time
from pathlib import Path
from typing import Optional


def stamp_split_config(
    workspace: Path,
    region: str = "us",
    *,
    config_yml: Optional[Path] = None,
) -> bool:
    """Ensure worktree ``config.json`` is a fresh regular file so ninja skips SPLIT.

    Returns True when a stamp was applied (copy or mtime bump), False when the
    split config was missing and nothing could be done.
    """
    region_build = workspace / "build" / region
    config_json = region_build / "config.json"
    yml = config_yml or (workspace / "config" / region / "config.yml")
    if not config_json.exists() and not config_json.is_symlink():
        return False

    # Materialize a private copy so SPLIT cannot rewrite the main tree via symlink.
    if config_json.is_symlink() or not config_json.is_file():
        source = config_json.resolve() if config_json.exists() else None
        if source is None or not source.is_file():
            return False
        tmp = config_json.with_suffix(".json.stamp-tmp")
        shutil.copy2(source, tmp)
        config_json.unlink(missing_ok=True)
        tmp.replace(config_json)

    now = time.time()
    if yml.is_file():
        yml_mtime = yml.stat().st_mtime
        # Keep output strictly newer than the split input.
        now = max(now, yml_mtime + 1.0)
    os.utime(config_json, (now, now))
    return True

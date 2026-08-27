#!/usr/bin/env python3
"""CI gate: registry schema + README status-block freshness.

Runs without build artifacts, ninja, or objdiff — pure stdlib over
``tools/coop/targets.json`` and ``README.md``.

    python3 tools/coop/registry_check.py             # exit 1 if either gate fails

This is the schema-only counterpart of ``run.py targets validate`` (which
also checks certificate staleness — a separate revalidation concern).
"""

from __future__ import annotations

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config  # noqa: E402
from tools.coop.lib.targets import validate_targets  # noqa: E402


def check_schema() -> list[str]:
    config = load_config(None, ROOT)
    return validate_targets(config, check_certificates=False)


def check_readme_block() -> list[str]:
    # Imported lazely so a config error above does not mask the README gate.
    from tools.coop import readme_status

    config = load_config(None, ROOT)
    if not readme_status.README.is_file():
        return [f"{readme_status.README} missing"]
    block = readme_status.generate_block(config)
    text = readme_status.README.read_text(encoding="utf-8")
    if readme_status.extract_block(text) != block:
        return [
            "README status block is stale — run: "
            f"{sys.executable} {readme_status.__file__} --write"
        ]
    return []


def main() -> int:
    failures: list[str] = []
    schema_errors = check_schema()
    if schema_errors:
        failures.extend(f"registry: {error}" for error in schema_errors)
    else:
        print("registry schema OK")
    readme_errors = check_readme_block()
    if readme_errors:
        failures.extend(f"readme: {error}" for error in readme_errors)
    else:
        print("README status block is current")
    if failures:
        for line in failures:
            print(f"ERROR: {line}", file=sys.stderr)
        print(f"registry check failed: {len(failures)} error(s)", file=sys.stderr)
        return 1
    print("registry check passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())

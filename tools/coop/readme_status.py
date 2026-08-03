#!/usr/bin/env python3
"""Regenerate the project-status block embedded in the root README.

The block between the GENERATED markers is produced from the target registry
(tools/coop/targets.json), not hand-edited.

Usage:
    python3 tools/coop/readme_status.py             # print the block to stdout
    python3 tools/coop/readme_status.py --write     # update README.md in place
    python3 tools/coop/readme_status.py --check     # CI: exit 1 if README is stale
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import CoopConfig, load_config  # noqa: E402
from tools.coop.lib.targets import ACCEPTED_MATCH_STATUSES, load_targets  # noqa: E402

README = ROOT / "README.md"
BEGIN = "<!-- BEGIN GENERATED COOP STATUS -->"
END = "<!-- END GENERATED COOP STATUS -->"


def generate_block(config: CoopConfig) -> str:
    targets = load_targets(config)
    buildable = sum(1 for t in targets if t.buildable)
    accepted = [t for t in targets if t.status in ACCEPTED_MATCH_STATUSES]
    full = sum(1 for t in accepted if t.status == "FULL_MATCH")
    equivalent = sum(1 for t in accepted if t.status == "EQUIVALENT_MATCH")
    active = sum(1 for t in targets if t.workflow_status == "ACTIVE")
    tiers: list[str] = []
    for tier in ("P0", "P1", "P2"):
        in_tier = [t for t in targets if t.tier == tier]
        if not in_tier:
            continue
        acc = sum(1 for t in in_tier if t.status in ACCEPTED_MATCH_STATUSES)
        tiers.append(f"{tier} {acc}/{len(in_tier)}")

    lines = [
        BEGIN,
        "",
        f"Region: `{config.region}` · acceptance bar: `EQUIVALENT_MATCH` or `FULL_MATCH` "
        f"(policy `{config.match_policy}`)",
        "",
        "| Metric | Count |",
        "|---|---|",
        f"| Targets (registry) | {len(targets)} |",
        f"| Buildable | {buildable} |",
        f"| Accepted | {len(accepted)} (`FULL_MATCH` {full} · `EQUIVALENT_MATCH` {equivalent}) |",
        f"| Active (in progress) | {active} |",
        f"| Accepted / total by tier | {' · '.join(tiers)} |",
        "",
        END,
    ]
    return "\n".join(lines)


def extract_block(text: str) -> str | None:
    start = text.find(BEGIN)
    end = text.find(END)
    if start < 0 or end < 0 or end <= start:
        return None
    return text[start : end + len(END)]


def replace_block(text: str, block: str) -> str:
    """Swap the generated block in *text*; append if the markers are absent."""
    current = extract_block(text)
    if current is None:
        return text.rstrip() + "\n\n" + block + "\n"
    return text.replace(current, block)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--write", action="store_true", help="update README.md in place")
    group.add_argument("--check", action="store_true", help="exit 1 if README.md is stale")
    args = parser.parse_args()

    config = load_config(None, ROOT)
    block = generate_block(config)

    if args.check:
        if not README.is_file():
            print(f"ERROR: {README} missing", file=sys.stderr)
            return 1
        text = README.read_text(encoding="utf-8")
        if extract_block(text) != block:
            print(
                f"README status block is stale — run: "
                f"{sys.executable} {Path(__file__).name} --write",
                file=sys.stderr,
            )
            return 1
        print("README status block is current")
        return 0

    if args.write:
        if not README.is_file():
            print(f"ERROR: {README} missing", file=sys.stderr)
            return 1
        text = README.read_text(encoding="utf-8")
        updated = replace_block(text, block)
        if updated != text:
            README.write_text(updated, encoding="utf-8")
            print(f"updated {README}")
        else:
            print(f"{README} already current")
        return 0

    print(block)
    return 0


if __name__ == "__main__":
    sys.exit(main())

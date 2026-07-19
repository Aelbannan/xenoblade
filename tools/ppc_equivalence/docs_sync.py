#!/usr/bin/env python3
"""Synchronise version metadata and ProofStatus docs across project manuals.

Usage:
    python -m tools.ppc_equivalence.docs_sync --write
    python -m tools.ppc_equivalence.docs_sync --check
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT, ProofStatus
from tools.coop.lib.targets import EQUIVALENCE_CERTIFICATE_VERSION

PACKAGE = Path(__file__).resolve().parent
README = ROOT / "README.md"
PACKAGE_README = PACKAGE / "README.md"
SOUNDNESS = PACKAGE / "SOUNDNESS.md"

# Root README plus package manuals that carry generated version/status blocks.
DOC_PATHS: tuple[Path, ...] = (README, PACKAGE_README, SOUNDNESS)

VERSION_BEGIN = "<!-- BEGIN GENERATED PPC_EQUIVALENCE_VERSION -->"
VERSION_END = "<!-- END GENERATED PPC_EQUIVALENCE_VERSION -->"
TABLE_BEGIN = "<!-- BEGIN GENERATED PROOF_STATUS_TABLE -->"
TABLE_END = "<!-- END GENERATED PROOF_STATUS_TABLE -->"


def generate_version_block() -> str:
    lines = [
        VERSION_BEGIN,
        "",
        f"- Architecture model: `{ARCHITECTURE_MODEL}`",
        f"- Result format: `{RESULT_FORMAT}`",
        f"- Certificate format: `{EQUIVALENCE_CERTIFICATE_VERSION}`",
        "",
        VERSION_END,
    ]
    return "\n".join(lines)


def generate_status_table() -> str:
    lines = [
        TABLE_BEGIN,
        "",
        "| Status | Value |",
        "|---|---|",
    ]
    for member in ProofStatus:
        lines.append(f"| `{member.name}` | `{member.value}` |")
    lines.append("")
    lines.append(TABLE_END)
    return "\n".join(lines)


def _find_marker_line(text: str, marker: str) -> int | None:
    lines = text.splitlines(keepends=True)
    for i, line in enumerate(lines):
        if marker in line:
            return i
    return None


def replace_section(text: str, begin: str, end: str, replacement: str) -> str:
    start = _find_marker_line(text, begin)
    end_line = _find_marker_line(text, end)
    if start is not None and end_line is not None and end_line > start:
        lines = text.splitlines(keepends=True)
        before = lines[:start]
        after = lines[end_line + 1:]
        return "".join(before) + replacement + "\n" + "".join(after)
    lines = text.splitlines(keepends=True)
    insert_at = 0
    for i, line in enumerate(lines):
        if line.startswith("## "):
            insert_at = i
            break
    lines.insert(insert_at, replacement + "\n")
    return "".join(lines)


def generate_document(path: Path) -> str:
    """Refresh generated version/status sections in a single markdown file."""
    text = path.read_text(encoding="utf-8")
    text = replace_section(text, VERSION_BEGIN, VERSION_END, generate_version_block())
    text = replace_section(text, TABLE_BEGIN, TABLE_END, generate_status_table())
    return text


def generate_readme(readme_path: Path) -> str:
    """Backward-compatible alias for :func:`generate_document`."""
    return generate_document(readme_path)


def _normalize_paths(paths: Path | list[Path] | tuple[Path, ...]) -> list[Path]:
    if isinstance(paths, Path):
        return [paths]
    return list(paths)


def _run(
    write: bool,
    check: bool,
    paths: Path | list[Path] | tuple[Path, ...] = DOC_PATHS,
) -> int:
    targets = _normalize_paths(paths)
    if write:
        for path in targets:
            updated = generate_document(path)
            path.write_text(updated, encoding="utf-8")
            print(f"wrote {path}")
        return 0
    if check:
        stale: list[Path] = []
        for path in targets:
            current = path.read_text(encoding="utf-8")
            expected = generate_document(path)
            if current != expected:
                stale.append(path)
        if not stale:
            return 0
        for path in stale:
            print(f"{path} is stale; run docs_sync.py --write", file=sys.stderr)
        return 1
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "--write",
        action="store_true",
        help="write generated content to tracked manuals",
    )
    group.add_argument(
        "--check",
        action="store_true",
        help="verify tracked manuals are up to date",
    )
    args = parser.parse_args()
    return _run(args.write, args.check, DOC_PATHS)


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""Generate per-status target list files from the coop target registry.

Produces the same lists used for triage/queueing, one file per tier, each
sorted by function size (smallest first):

  unmatched-highmatch.txt          unmatched targets at exactly HIGH_MATCH
  code-match.txt                   all CODE_MATCH targets
  structural-functions.txt         all STRUCTURAL targets
  compiles-behavior-verified.txt   all COMPILES / BEHAVIOR_VERIFIED targets

Sizes come from the registry ``size`` field when present; otherwise they are
derived from the retail asm comments (``| 0xADDR | size: 0x..``) in
build/<region>/asm/<unit>.s.

Usage:
  python3 tools/coop/list_targets.py --all                 # all four lists
  python3 tools/coop/list_targets.py --unmatched-highmatch --code-match
  python3 tools/coop/list_targets.py --all --output-dir /tmp
  python3 tools/coop/list_targets.py --all --dry-run       # preview counts only

Exit code: 0 on success.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Callable, Dict, List, Optional

# ---------------------------------------------------------------------------
# Path setup — import from sibling modules
# ---------------------------------------------------------------------------
ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import CoopConfig, load_config
from tools.coop.lib.targets import Target, load_targets

# ---------------------------------------------------------------------------
# List definitions
# ---------------------------------------------------------------------------

LISTS: Dict[str, Dict[str, object]] = {
    "unmatched-highmatch": {
        "filename": "unmatched-highmatch.txt",
        "help": "unmatched targets at exactly HIGH_MATCH",
        "filter": lambda t: (
            t.workflow_status != "ACCEPTED" and t.status == "HIGH_MATCH"
        ),
    },
    "code-match": {
        "filename": "code-match.txt",
        "help": "all CODE_MATCH targets",
        "filter": lambda t: t.status == "CODE_MATCH",
    },
    "structural": {
        "filename": "structural-functions.txt",
        "help": "all STRUCTURAL targets",
        "filter": lambda t: t.status == "STRUCTURAL",
    },
    "compiles-behavior-verified": {
        "filename": "compiles-behavior-verified.txt",
        "help": "all COMPILES / BEHAVIOR_VERIFIED targets",
        "filter": lambda t: t.status in {"COMPILES", "BEHAVIOR_VERIFIED"},
    },
}

# ---------------------------------------------------------------------------
# Size resolution
# ---------------------------------------------------------------------------

_asm_cache: Dict[str, Optional[str]] = {}


def _asm_text(path: Path) -> Optional[str]:
    key = str(path)
    if key not in _asm_cache:
        _asm_cache[key] = (
            path.read_text(encoding="utf-8", errors="replace")
            if path.is_file()
            else None
        )
    return _asm_cache[key]


def _asm_size(asm_dir: Path, target: Target) -> Optional[int]:
    """Derive a function's size from the retail asm size comment."""
    unit, addr = target.unit, target.address
    if not unit or not addr:
        return None
    text = _asm_text(asm_dir / f"{unit}.s")
    if not text:
        return None
    pattern = re.compile(
        r"\| 0x" + re.escape(addr[2:]) + r" \| size: (0x[0-9a-fA-F]+)", re.IGNORECASE
    )
    matches = list(pattern.finditer(text))
    return int(matches[-1].group(1), 16) if matches else None


def target_size(asm_dir: Path, target: Target) -> Optional[int]:
    """Registry size when present, else the retail asm size comment."""
    raw = target.extra.get("size")
    if isinstance(raw, str):
        try:
            return int(raw, 16)
        except ValueError:
            pass
    if isinstance(raw, (int, float)):
        return int(raw)
    return _asm_size(asm_dir, target)


# ---------------------------------------------------------------------------
# Rendering
# ---------------------------------------------------------------------------

def render_rows(asm_dir: Path, targets: List[Target]) -> str:
    def key(t: Target) -> tuple:
        size = target_size(asm_dir, t)
        return (size is None, size or 0)

    lines = []
    for t in sorted(targets, key=key):
        size = target_size(asm_dir, t)
        size_text = "n/a" if size is None else f"0x{size:X}"
        lines.append(
            f"{size_text:>7}  {t.status:<16} {t.workflow_status:<10} "
            f"{t.id:<32} {t.function}"
        )
    return "\n".join(lines) + ("\n" if lines else "")


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument("--config", help="Path to coop.json or coop.yaml")
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=Path("/tmp"),
        help="Directory for generated files (default: /tmp)",
    )
    parser.add_argument("--dry-run", action="store_true", help="Print counts only")
    for name, spec in LISTS.items():
        parser.add_argument(
            f"--{name}",
            action="store_true",
            help=spec["help"],
        )
    parser.add_argument("--all", action="store_true", help="Generate every list")
    return parser


def main(argv: Optional[List[str]] = None) -> int:
    args = build_parser().parse_args(argv)

    config = load_config(ROOT, args.config) if args.config else CoopConfig(
        project_root=ROOT, region="us"
    )
    asm_dir = ROOT / "build" / config.region / "asm"
    targets = load_targets(config)

    selected = [name for name in LISTS if getattr(args, name.replace("-", "_"))]
    if args.all or not selected:
        selected = list(LISTS)
    if args.all:
        selected = list(LISTS)

    for name in selected:
        spec = LISTS[name]
        rows = [t for t in targets if spec["filter"](t)]  # type: ignore[operator]
        if args.dry_run:
            print(f"{name}: {len(rows)} targets")
            continue
        out = args.output_dir / spec["filename"]  # type: ignore[index]
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(render_rows(asm_dir, rows), encoding="utf-8")
        print(f"{out} ({len(rows)} targets)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

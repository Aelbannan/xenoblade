#!/usr/bin/env python3
"""Render an SVG progress map of the main.dol code section.

Each pixel column of the strip is one slice of the retail address space;
functions from the target registry (tools/coop/targets.json) are plotted at
their retail address and colored by match state. Regenerate after matching
work and commit the result.

Usage:
    python3 tools/coop/progress_map.py                  # write assets/progress-map.svg
    python3 tools/coop/progress_map.py -o /tmp/map.svg  # custom output path
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config  # noqa: E402
from tools.coop.lib.targets import ACCEPTED_MATCH_STATUSES, load_targets  # noqa: E402

DEFAULT_OUTPUT = ROOT / "assets" / "progress-map.svg"

# Strip geometry.
WIDTH = 2400
STRIP_TOP = 40
STRIP_HEIGHT = 64
AXIS_TOP = STRIP_TOP + STRIP_HEIGHT + 6
TEXT_TOP = AXIS_TOP + 18
HEIGHT = 160

# Column colour priority (highest wins when several functions share a column).
PRIORITY = {
    "FULL_MATCH": 4,
    "EQUIVALENT_MATCH": 3,
    "in-progress": 2,
    "NOT_STARTED": 1,
}
FILL = {
    "FULL_MATCH": "#1f883d",
    "EQUIVALENT_MATCH": "#0969da",
    "in-progress": "#d4a72c",
    "NOT_STARTED": "#6e7681",
}
INK = "#24292f"
MUTED = "#57606a"

IN_PROGRESS = {
    "ACTIVE", "ACCEPTED", "BLOCKED", "CODE_MATCH", "COMPILES", "HIGH_MATCH",
    "STALLED", "STALL", "STALLED_BLOCKED_EXTERNAL", "STRUCTURAL",
}


def classify(status: str) -> str:
    if status == "FULL_MATCH":
        return "FULL_MATCH"
    if status == "EQUIVALENT_MATCH":
        return "EQUIVALENT_MATCH"
    if status in IN_PROGRESS:
        return "in-progress"
    return "NOT_STARTED"


def render(targets: list) -> str:
    rows = [
        t for t in targets
        if t.kind == "function" and t.address is not None
    ]
    if not rows:
        raise SystemExit("ERROR: no function targets with addresses in the registry")
    rows.sort(key=lambda t: int(t.address, 16))

    lo = int(rows[0].address, 16)
    hi = int(rows[-1].address, 16)
    span = max(hi - lo, 1)

    def x_of(addr: int) -> int:
        return round((addr - lo) / span * (WIDTH - 1))

    # Per-column best status (highest priority present).
    column: dict[int, str] = {}
    for t in rows:
        cls = classify(t.status)
        col = x_of(int(t.address, 16))
        if cls not in column or PRIORITY[cls] > PRIORITY[column[col]]:
            column[col] = cls

    counts = {k: 0 for k in FILL}
    for t in rows:
        counts[classify(t.status)] += 1
    total = len(rows)
    accepted = sum(counts[k] for k in ("FULL_MATCH", "EQUIVALENT_MATCH"))

    parts: list[str] = []
    parts.append(
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{WIDTH}" height="{HEIGHT}" '
        f'viewBox="0 0 {WIDTH} {HEIGHT}" font-family="ui-sans-serif, system-ui, sans-serif">'
    )
    parts.append(
        f'<text x="0" y="20" font-size="17" font-weight="600" fill="{INK}">'
        f"Xenoblade Chronicles (Wii) — code section progress</text>"
    )
    parts.append(
        f'<text x="{WIDTH}" y="20" font-size="14" fill="{MUTED}" text-anchor="end">'
        f"{accepted}/{total} accepted ({100.0 * accepted / total:.1f}%)</text>"
    )
    parts.append(
        f'<rect x="0" y="{STRIP_TOP}" width="{WIDTH}" height="{STRIP_HEIGHT}" '
        f'fill="#ffffff" stroke="{MUTED}" stroke-width="1"/>'
    )
    # One rect per occupied column (≤ WIDTH elements regardless of target count).
    for col, cls in sorted(column.items()):
        parts.append(
            f'<rect x="{col}" y="{STRIP_TOP}" width="1" height="{STRIP_HEIGHT}" fill="{FILL[cls]}"/>'
        )

    # Axis: ticks every 1 MiB of address space.
    tick = ((lo + 0xFFFFF) // 0x100000) * 0x100000
    while tick <= hi:
        x = x_of(tick)
        parts.append(
            f'<line x1="{x}" y1="{STRIP_TOP + STRIP_HEIGHT}" x2="{x}" '
            f'y2="{AXIS_TOP}" stroke="{MUTED}" stroke-width="1"/>'
        )
        parts.append(
            f'<text x="{x}" y="{TEXT_TOP}" font-size="11" fill="{MUTED}" '
            f'text-anchor="middle">0x{tick:08X}</text>'
        )
        tick += 0x100000

    parts.append(
        f'<text x="0" y="{TEXT_TOP}" font-size="11" fill="{MUTED}" ' 
        f'text-anchor="start">0x{lo:08X}</text>'
    )
    parts.append(
        f'<text x="{WIDTH}" y="{TEXT_TOP}" font-size="11" fill="{MUTED}" ' 
        f'text-anchor="end">0x{hi:08X}</text>'
    )

    # Legend with counts.
    legend = [
        ("FULL_MATCH", "FULL_MATCH", counts["FULL_MATCH"]),
        ("EQUIVALENT_MATCH", "EQUIVALENT_MATCH", counts["EQUIVALENT_MATCH"]),
        ("in progress", "in-progress", counts["in-progress"]),
        ("not started", "NOT_STARTED", counts["NOT_STARTED"]),
    ]
    lx = 0
    for label, key, count in legend:
        parts.append(
            f'<rect x="{lx}" y="{HEIGHT - 24}" width="12" height="12" rx="2" fill="{FILL[key]}"/>'
        )
        parts.append(
            f'<text x="{lx + 18}" y="{HEIGHT - 13}" font-size="12" fill="{INK}">'
            f"{label} · {count}</text>"
        )
        lx += 18 + len(label) * 7.2 + len(str(count)) * 7.2 + 28
    parts.append("</svg>")
    return "\n".join(parts)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("-o", "--output", type=Path, default=DEFAULT_OUTPUT,
                        help=f"output path (default: {DEFAULT_OUTPUT.relative_to(ROOT)})")
    args = parser.parse_args()

    config = load_config(None, ROOT)
    targets = load_targets(config)
    svg = render(targets)

    output = args.output if args.output.is_absolute() else ROOT / args.output
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(svg + "\n", encoding="utf-8")
    print(output)
    return 0


if __name__ == "__main__":
    sys.exit(main())

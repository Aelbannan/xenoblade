#!/usr/bin/env python3
"""Regenerate ``assets/progress-map.svg`` from the target registry.

Each column of the map is one slice of the retail ``main.dol`` code section
(addresses ``0x80004000``-``0x804F9FA4``, 2400 slices total), colored by the
*best* match state among the functions whose address falls in that slice:

* green  ``#1f883d``  ``FULL_MATCH``
* blue   ``#0969da``  ``EQUIVALENT_MATCH``
* amber  ``#d4a72c``  in progress (any status other than the above or
                      ``NOT_STARTED``)
* gray   ``#6e7681``  not started (``NOT_STARTED``)

Slices that contain no function start are left blank (white). The header and
legend counts are derived from the same per-status tallies.

Usage::

    .venv/bin/python3 tools/coop/progress_map.py            # rewrite the SVG
    .venv/bin/python3 tools/coop/progress_map.py --check    # exit 1 if stale

The image is intentionally rendered at 3x logical scale (3px per slice, larger
type) with a taller banner layout so it reads well both standalone and at
README width; tweak ``SCALE`` below to change it globally.
"""

import argparse
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TARGETS_PATH = os.path.join(ROOT, "tools", "coop", "targets.json")
DEFAULT_OUT = os.path.join(ROOT, "assets", "progress-map.svg")

# Code-section address range covered by the map (US main.dol).
CODE_BASE = 0x80004000
CODE_END = 0x804F9FA4
COLUMNS = 2400

# Global size multiplier. 1.0 reproduces the original (tiny) proportions;
# the committed default of 3.0 triples every dimension.
SCALE = 3.0

# Match-state colors (best state wins when several functions share a slice).
COLOR_FULL = "#1f883d"
COLOR_EQUIV = "#0969da"
COLOR_IN_PROGRESS = "#d4a72c"
COLOR_NOT_STARTED = "#6e7681"
COLOR_TEXT = "#24292f"
COLOR_MUTED = "#57606a"
COLOR_WHITE = "#ffffff"

FULL = "FULL_MATCH"
EQUIV = "EQUIVALENT_MATCH"
NOT_STARTED = "NOT_STARTED"

TITLE = "Xenoblade Chronicles (Wii) — code section progress"


def tally(targets):
    """Return (full, equiv, in_progress, not_started, accepted, total)."""
    full = equiv = prog = ns = 0
    for t in targets:
        s = t.get("status")
        if s == FULL:
            full += 1
        elif s == EQUIV:
            equiv += 1
        elif s == NOT_STARTED:
            ns += 1
        else:
            prog += 1
    return full, equiv, prog, ns, full + equiv, len(targets)


def slice_colors(targets, n=COLUMNS, base=CODE_BASE, end=CODE_END):
    """Map each address-bearing target to a slice; pick best color per slice.

    Returns {slice_index: color} for slices that contain at least one target.
    """
    step = (end - base) / n
    best = {}
    for t in targets:
        a = t.get("address")
        if a is None:
            continue
        try:
            addr = int(a, 16) if isinstance(a, str) else int(a)
        except ValueError:
            continue
        if not (base <= addr <= end):
            continue
        idx = int((addr - base) / step)
        if idx >= n:
            idx = n - 1
        s = t.get("status")
        color = (
            COLOR_FULL
            if s == FULL
            else COLOR_EQUIV
            if s == EQUIV
            else COLOR_NOT_STARTED
            if s == NOT_STARTED
            else COLOR_IN_PROGRESS
        )
        prev = best.get(idx)
        if prev is None or color != prev:
            # priority: full > equiv > in-progress > not-started
            if prev is None:
                best[idx] = color
            elif prev == COLOR_NOT_STARTED:
                best[idx] = color
            elif color == COLOR_FULL:
                best[idx] = color
            elif color == COLOR_EQUIV and prev in (COLOR_IN_PROGRESS, COLOR_NOT_STARTED):
                best[idx] = color
            elif color == COLOR_IN_PROGRESS and prev == COLOR_NOT_STARTED:
                best[idx] = color
    return best


def axis_x(addr, base=CODE_BASE, end=CODE_END, n=COLUMNS):
    """Column x position (in unscaled px) for an absolute address."""
    return round((addr - base) / (end - base) * n, 1)


def render(counts, slices):
    """Assemble the SVG document at SCALE resolution.

    Layout constants below are given in 1x design units (2400px wide) and
    scaled by SCALE. The 1x proportions: 740px tall (bar 420px, legend ~80px)
    so the image renders as a larger, readable banner at README width.
    """
    full, equiv, prog, ns, accepted, total = counts
    s = SCALE
    w = int(COLUMNS * s)
    h = int(740 * s)
    col_w = s

    title_font = round(48 * s)
    title_y = round(84 * s)
    count_font = round(38 * s)
    count_y = title_y
    axis_font = round(26 * s)
    legend_font = round(36 * s)
    bar_top = round(150 * s)
    bar_h = round(420 * s)
    axis_y = round(620 * s)
    legend_swatch = round(40 * s)
    legend_y = round(660 * s)
    legend_text_y = legend_y + round(27 * s)

    pad = round(32 * s)
    stroke = max(1, round(2 * s))

    out = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="{h}" '
        f'viewBox="0 0 {w} {h}" font-family="ui-sans-serif, system-ui, sans-serif">',
        # opaque card so the text stays readable on light and dark themes
        f'<rect x="0" y="0" width="{w}" height="{h}" fill="{COLOR_WHITE}"/>',
        f'<text x="{pad}" y="{title_y}" font-size="{title_font}" font-weight="700" '
        f'fill="{COLOR_TEXT}">{TITLE}</text>',
        f'<text x="{w - pad}" y="{count_y}" font-size="{count_font}" '
        f'fill="{COLOR_MUTED}" text-anchor="end">{accepted}/{total} accepted '
        f"({accepted / total * 100:.1f}%)</text>",
        f'<rect x="0" y="{bar_top}" width="{w}" height="{bar_h}" fill="{COLOR_WHITE}" '
        f'stroke="{COLOR_MUTED}" stroke-width="{stroke}"/>',
    ]
    for idx in sorted(slices):
        x = round(idx * s, 1)
        out.append(
            f'<rect x="{x}" y="{bar_top}" width="{col_w}" height="{bar_h}" '
            f'fill="{slices[idx]}"/>'
        )
    # address axis labels: section start, each 0x00100000 boundary, section end
    axis_labels = [(CODE_BASE, pad, "start"), (0x80100000, axis_x(0x80100000) * s, "middle"),
                   (0x80200000, axis_x(0x80200000) * s, "middle"),
                   (0x80300000, axis_x(0x80300000) * s, "middle"),
                   (0x80400000, axis_x(0x80400000) * s, "middle"),
                   (CODE_END, w - pad, "end")]
    for addr, x, anchor in axis_labels:
        out.append(
            f'<text x="{x}" y="{axis_y}" font-size="{axis_font}" fill="{COLOR_MUTED}" '
            f'text-anchor="{anchor}">0x{addr:08X}</text>'
        )

    # legend
    items = [
        (COLOR_FULL, f"FULL_MATCH · {full}"),
        (COLOR_EQUIV, f"EQUIVALENT_MATCH · {equiv}"),
        (COLOR_IN_PROGRESS, f"in progress · {prog}"),
        (COLOR_NOT_STARTED, f"not started · {ns}"),
    ]
    # rough glyph advance for layout only (ui-sans-serif, ~0.58em average)
    x = pad
    gap = round(60 * s)
    for color, label in items:
        out.append(
            f'<rect x="{x}" y="{legend_y}" width="{legend_swatch}" height="{legend_swatch}" '
            f'rx="{round(6 * s)}" fill="{color}"/>'
        )
        out.append(
            f'<text x="{x + legend_swatch + round(12 * s)}" y="{legend_text_y}" '
            f'font-size="{legend_font}" fill="{COLOR_TEXT}">{label}</text>'
        )
        x += legend_swatch + round(12 * s) + round(len(label) * 0.58 * legend_font) + gap
    out.append("</svg>")
    return "\n".join(out) + "\n"


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true",
                    help="exit 1 if the SVG is stale instead of rewriting it")
    ap.add_argument("--out", default=DEFAULT_OUT, help="output path (default: %(default)s)")
    args = ap.parse_args(argv)

    with open(TARGETS_PATH, encoding="utf-8") as fh:
        targets = json.load(fh)["targets"]

    counts = tally(targets)
    full, equiv, prog, ns, accepted, total = counts
    print(f"accepted {accepted}/{total} ({accepted / total * 100:.1f}%) — "
          f"FULL_MATCH {full} · EQUIVALENT_MATCH {equiv} · in progress {prog} · "
          f"not started {ns}")

    svg = render(counts, slice_colors(targets))

    if args.check:
        try:
            with open(args.out, encoding="utf-8") as fh:
                current = fh.read()
        except OSError:
            print(f"{args.out}: missing", file=sys.stderr)
            return 1
        if current == svg:
            return 0
        print(f"{args.out} is stale — regenerate with {os.path.basename(__file__)}",
              file=sys.stderr)
        return 1

    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w", encoding="utf-8") as fh:
        fh.write(svg)
    print(f"wrote {args.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

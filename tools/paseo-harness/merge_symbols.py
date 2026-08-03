#!/usr/bin/env python3
"""Git merge driver: per-key 3-way union for config/<region>/symbols.txt.

Key = first whitespace-delimited token (the symbol name). For each key:
theirs wins if it changed vs base; else ours wins if it changed vs base;
else keep ours. Ours order preserved; theirs-only keys appended.
"""
import sys


def key(line):
    return line.split(None, 1)[0] if line.strip() else None


def index(lines):
    d = {}
    order = []
    for i, line in enumerate(lines):
        k = key(line)
        if k is None:
            continue
        if k not in d:
            d[k] = []
            order.append(k)
        d[k].append(line)
    return d, order


def main():
    if len(sys.argv) != 4:
        sys.stderr.write("usage: merge_symbols.py %O %A %B\n")
        return 1
    base_p, ours_p, theirs_p = sys.argv[1:4]
    try:
        with open(base_p) as f:
            base_lines = f.read().splitlines()
        with open(ours_p) as f:
            ours_lines = f.read().splitlines()
        with open(theirs_p) as f:
            theirs_lines = f.read().splitlines()
    except Exception as e:
        sys.stderr.write(f"merge_symbols: read failed, keeping ours: {e}\n")
        return 0

    b, b_order = index(base_lines)
    o, o_order = index(ours_lines)
    t, t_order = index(theirs_lines)

    merged = {}
    for k in o_order:
        merged[k] = list(o[k])
    for k in t_order:
        if k not in merged:
            merged[k] = list(t[k])
        elif b.get(k) is not None and t[k] != b[k]:
            merged[k] = list(t[k])  # theirs changed it vs base
        elif b.get(k) is None and t[k] != o[k]:
            merged[k] = list(t[k])  # new on both sides, differing

    out = []
    for k in o_order:
        out.extend(merged[k])
    for k in t_order:
        if k not in o:
            out.extend(merged[k])

    try:
        with open(ours_p, "w") as f:
            f.write("\n".join(out))
            if out:
                f.write("\n")
    except Exception as e:
        sys.stderr.write(f"merge_symbols: write failed: {e}\n")
        return 0
    return 0


if __name__ == "__main__":
    sys.exit(main())

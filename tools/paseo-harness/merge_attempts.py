#!/usr/bin/env python3
"""Git merge driver: unique-line union for docs/evidence/decomp/attempts.jsonl
(append-only history). Base lines first, then ours, then theirs; exact-duplicate
lines collapse. Order is otherwise preserved.
"""
import sys


def main():
    if len(sys.argv) != 4:
        sys.stderr.write("usage: merge_attempts.py %O %A %B\n")
        return 1
    base_p, ours_p, theirs_p = sys.argv[1:4]
    try:
        with open(base_p) as f:
            base = f.read().splitlines()
        with open(ours_p) as f:
            ours = f.read().splitlines()
        with open(theirs_p) as f:
            theirs = f.read().splitlines()
    except Exception as e:
        sys.stderr.write(f"merge_attempts: read failed, keeping ours: {e}\n")
        return 0

    seen = set()
    out = []
    for chunk in (base, ours, theirs):
        for line in chunk:
            if line in seen:
                continue
            seen.add(line)
            out.append(line)

    try:
        with open(ours_p, "w") as f:
            f.write("\n".join(out))
            if out:
                f.write("\n")
    except Exception as e:
        sys.stderr.write(f"merge_attempts: write failed: {e}\n")
        return 0
    return 0


if __name__ == "__main__":
    sys.exit(main())

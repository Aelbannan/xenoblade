#!/usr/bin/env python3
"""Git merge driver: 3-way per-target-id union for tools/coop/targets.json.

Invoked by git as:  merge_targets.py %O %A %B
For each target id: theirs wins if it changed vs base; else ours wins if it
changed vs base; else keep ours. Same rule for the callgraph dict. Ours order
is preserved; ids new in theirs are appended in theirs order.
"""
import json
import sys


def union_3way(base, ours, theirs):
    """Generic per-key 3-way union over dicts."""
    out = dict(ours)
    for k, tv in theirs.items():
        bv = base.get(k)
        ov = ours.get(k)
        if k not in out or (bv is not None and tv != bv):
            out[k] = tv
        elif bv is None and ov is not None and tv != ov:
            out[k] = tv  # new key appeared on both sides with different values
    return out


def main():
    if len(sys.argv) != 4:
        sys.stderr.write("usage: merge_targets.py %O %A %B\n")
        return 1
    base_p, ours_p, theirs_p = sys.argv[1:4]
    try:
        base = json.load(open(base_p)) or {}
        ours = json.load(open(ours_p)) or {}
        theirs = json.load(open(theirs_p)) or {}
    except Exception as e:
        sys.stderr.write(f"merge_targets: parse failed, keeping ours: {e}\n")
        return 0  # keep %A (ours) untouched

    merged = dict(ours)
    # scalar/structural top-level keys: theirs wins only if it differs from base
    for k, tv in theirs.items():
        if k == "targets" or k == "callgraph":
            continue
        if base.get(k) != tv:
            merged[k] = tv

    # targets: per-id 3-way union
    ours_t = {t["id"]: t for t in ours.get("targets", [])}
    base_t = {t["id"]: t for t in base.get("targets", [])}
    theirs_t = {t["id"]: t for t in theirs.get("targets", [])}
    merged_t = dict(ours_t)
    for tid, tv in theirs_t.items():
        bv = base_t.get(tid)
        if tid not in merged_t or (bv is not None and tv != bv):
            merged_t[tid] = tv
        elif bv is None and tv != ours_t.get(tid):
            merged_t[tid] = tv
    # preserve ours order, append theirs-only ids in theirs order
    order = [t["id"] for t in ours.get("targets", [])]
    seen = set(order)
    for t in theirs.get("targets", []):
        if t["id"] not in seen:
            order.append(t["id"])
            seen.add(t["id"])
    merged["targets"] = [merged_t[i] for i in order if i in merged_t]

    # callgraph: per-key 3-way union
    if "callgraph" in theirs or "callgraph" in ours:
        merged["callgraph"] = union_3way(
            base.get("callgraph", {}),
            ours.get("callgraph", {}),
            theirs.get("callgraph", {}),
        )

    try:
        with open(ours_p, "w") as f:
            json.dump(merged, f)
    except Exception as e:
        sys.stderr.write(f"merge_targets: write failed: {e}\n")
        return 0
    return 0


if __name__ == "__main__":
    sys.exit(main())

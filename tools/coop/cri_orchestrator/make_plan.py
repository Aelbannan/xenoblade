#!/usr/bin/env python3
"""Generate the CriWare batch plan for the paseo matching campaign.

Batching rules (per user):
- Unmatched = workflow_status != ACCEPTED in tools/coop/targets.json, unit starts with 'CriWare'.
- Leaf targets (no direct/unresolved/indirect calls) go first.
- TUs with >= 20 unmatched targets -> one batch per TU.
- TUs with < 20 unmatched targets -> pooled, chunked to ~30 targets per batch.
"""
import json
import os
import sys
from collections import Counter, defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
TARGETS = os.path.join(ROOT, "tools", "coop", "targets.json")
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "plan.json")

BATCH_SIZE = 30
SINGLE_TU_THRESHOLD = 20


def is_leaf(t):
    return (
        not t.get("called_functions")
        and not t.get("unresolved_called_functions")
        and not t.get("has_indirect_calls")
    )


def main():
    d = json.load(open(TARGETS))
    targets = d["targets"]
    cri = [t for t in targets if (t.get("unit") or "").lower().startswith("cri")]
    unmatched = [t for t in cri if t.get("workflow_status") != "ACCEPTED"]
    unmatched.sort(key=lambda t: (t["address"] or ""))

    # sanity
    by_status = Counter(t.get("workflow_status") for t in unmatched)
    print(f"unmatched total: {len(unmatched)}  ({dict(by_status)})")
    leaves = [t for t in unmatched if is_leaf(t)]
    print(f"unmatched leaves: {len(leaves)}")

    by_unit = defaultdict(list)
    for t in unmatched:
        by_unit[t["unit"]].append(t)

    batches = []  # list of dicts
    bid = 0

    def make_batch(tlist, label):
        nonlocal bid
        bid += 1
        tlist = sorted(tlist, key=lambda t: (0 if is_leaf(t) else 1, int(t.get("size") or "0x0", 16), t["id"]))
        units = sorted(set(t["unit"] for t in tlist))
        leaf_count = sum(1 for t in tlist if is_leaf(t))
        batches.append(
            {
                "batch_id": f"cri-{bid:02d}",
                "label": label,
                "targets": [t["id"] for t in tlist],
                "target_details": [
                    {
                        "id": t["id"],
                        "unit": t["unit"],
                        "function": t.get("function") or t.get("symbol"),
                        "status": t.get("status"),
                        "match_percent": t.get("instruction_match"),
                        "size": t.get("size"),
                        "leaf": is_leaf(t),
                    }
                    for t in tlist
                ],
                "units": units,
                "leaf_count": leaf_count,
                "total": len(tlist),
            }
        )

    # 1) TUs with >= threshold unmatched -> own batch
    big = {u: ts for u, ts in by_unit.items() if len(ts) >= SINGLE_TU_THRESHOLD}
    for u in sorted(big, key=lambda u: -len(big[u])):
        make_batch(big[u], u)

    # 2) pool the rest, order units leaf-rich first
    pool = {u: ts for u, ts in by_unit.items() if len(ts) < SINGLE_TU_THRESHOLD}
    pool_order = sorted(
        pool,
        key=lambda u: (
            -sum(1 for t in pool[u] if is_leaf(t)),
            -len(pool[u]),
        ),
    )
    cur = []
    for u in pool_order:
        cur.extend(pool[u])
        if len(cur) >= BATCH_SIZE:
            make_batch(cur, "mixed")
            cur = []
    if cur:
        make_batch(cur, "mixed")

    plan = {
        "generated_at": None,  # filled by monitor on write? keep here
        "batches": batches,
        "total_targets": len(unmatched),
        "total_batches": len(batches),
        "leaf_targets": len(leaves),
    }
    json.dump(plan, open(OUT, "w"), indent=1)
    print(f"wrote {OUT}: {len(batches)} batches, {len(unmatched)} targets")
    for b in batches:
        print(
            f"  {b['batch_id']}: {b['total']:3d} targets ({b['leaf_count']:2d} leaf) "
            f"[{b['units'][0].split('/')[-1]}" + (f", ... {len(b['units'])} units" if len(b["units"]) > 1 else "") + "]"
        )


if __name__ == "__main__":
    main()

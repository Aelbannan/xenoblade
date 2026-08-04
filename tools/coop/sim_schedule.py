#!/usr/bin/env python3
"""
Similarity-anchored target ranking (Phase 4, read-only re-ranker).

For a unit, rank its NOT_STARTED/COMPILES targets by instruction-opcode
similarity to matched siblings (FULL_MATCH / EQUIVALENT_MATCH), and emit
the closest matched sibling(s) per target as a *pointer* for the brief.

This is a re-ranker, NOT a selector: the call-graph `ready` ordering stays
authoritative (SMT acceptance fails closed on unaccepted callees). The
consumer (pi-harness) intersects this ranking with its ready wave.

Read-only: never writes targets.json, no git ops, no SMT.

Usage:
  .venv/bin/python3 tools/coop/sim_schedule.py --unit kyoshin/cf/CfRes \
      --limit 5 --min-size 0x30 --json
"""

import argparse
import json
import os
import re
import sys
from collections import Counter

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TARGETS_JSON = os.path.join(REPO_ROOT, "tools", "coop", "targets.json")
ASM_DIR = os.path.join(REPO_ROOT, "build", "us", "asm")

ACCEPTED = frozenset({"FULL_MATCH", "EQUIVALENT_MATCH"})
CANDIDATE = frozenset({"NOT_STARTED", "COMPILES"})

_FN_RE = re.compile(r'^\.fn\s+"?([^",]+)"?\s*(?:,.*)?$')
_ENDFN_RE = re.compile(r"^\.endfn(?:\s+.*)?$")


# ---------------------------------------------------------------------------
# Mnemonic extraction — adapted from tools/batch_cluster.py::analyze_instrs
# (label skip) and tools/batch_getters.py::parse_instructions (comment
# strip). We deliberately do NOT import those legacy batch pipelines: besides
# the pure parsers they also carry target-mutation paths (load_targets /
# save_targets / cycle_one / claim_targets) with direct unlocked
# targets.json writes.
# ---------------------------------------------------------------------------
def parse_instructions(body):
    """Extract (mnemonic, operands) tuples from an assembly body."""
    instrs = []
    for line in body.split("\n"):
        line = line.strip()
        if not line:
            continue
        # Strip the objdiff bytecode comment prefix: /* VA OFFSET BYTES */
        idx = line.find("*/")
        if idx >= 0:
            line = line[idx + 2 :].strip()
            if not line:
                continue
        # Skip labels (.L_xxx:), directives (.4byte) and comment lines (# ...)
        if line.startswith((".", "#")):
            continue
        parts = line.split()
        if parts:
            instrs.append((parts[0], " ".join(parts[1:])))
    return instrs


def split_asm_blocks(asm_text):
    """Split a `.s` listing into {symbol: body-text} in one pass."""
    blocks = {}
    current = None
    body = []
    for raw in asm_text.split("\n"):
        line = raw.strip()
        fn = _FN_RE.match(line)
        if fn:
            if current is not None:
                blocks[current] = "\n".join(body)
            current = fn.group(1)
            body = []
        elif _ENDFN_RE.match(line):
            if current is not None:
                blocks[current] = "\n".join(body)
            current = None
            body = []
        elif current is not None:
            body.append(line)
    if current is not None:
        blocks[current] = "\n".join(body)
    return blocks


def mnemonic_counts(instrs):
    """Multiset of mnemonics for a function body (cached per symbol)."""
    counts = Counter()
    for mnem, _ops in instrs:
        counts[mnem] += 1
    return counts


def overlap_ratio(a, b):
    """Multiset Jaccard (Tanimoto) over mnemonic counts: sum(min)/sum(max).

    1.0 = identical opcode profile; 0.0 = no shared mnemonics. Count-weighted,
    so a 100-instruction sibling sharing every mnemonic with a 10-instruction
    candidate scores 1.0 — they compile to the same codegen pattern.
    """
    if not a or not b:
        return 0.0
    inter = union = 0
    for mnem in set(a) | set(b):
        ca = a.get(mnem, 0)
        cb = b.get(mnem, 0)
        inter += min(ca, cb)
        union += max(ca, cb)
    return inter / union if union else 0.0


def load_targets():
    with open(TARGETS_JSON) as f:
        data = json.load(f)
    return data.get("targets", []) if isinstance(data, dict) else []


def parse_size(raw):
    """Registry sizes are hex strings ("0x210"); tolerate decimal ints."""
    if raw is None:
        return None
    s = str(raw)
    try:
        if s.lower().startswith("0x"):
            return int(s, 16)
        return int(s, 10)
    except ValueError:
        return None


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--unit", required=True, help="objdiff unit, e.g. kyoshin/cf/CfRes")
    ap.add_argument("--limit", type=int, default=None, help="emit at most N ranked targets (default: all)")
    ap.add_argument("--min-size", default=None, help="skip candidates smaller than HEX, e.g. 0x30")
    ap.add_argument("--json", action="store_true", help="emit one JSON object per line (JSONL)")
    args = ap.parse_args(argv)

    min_size = parse_size(args.min_size)

    siblings = []
    candidates = []
    for t in load_targets():
        if t.get("unit") != args.unit:
            continue
        kind = t.get("kind")
        if kind is not None and kind != "function":
            continue
        status = t.get("status", "NOT_STARTED")
        if status in ACCEPTED:
            siblings.append(t)
        elif status in CANDIDATE:
            size = parse_size(t.get("size"))
            if min_size is not None and (size is None or size < min_size):
                continue
            candidates.append(t)
        # Other statuses (STRUCTURAL / HIGH_MATCH / CODE_MATCH / STALLED …)
        # are in-progress work — never re-ranked here.

    # Parse each matched function's ASM once, cache mnemonic sets in memory.
    asm_path = os.path.join(ASM_DIR, args.unit + ".s")
    blocks = {}
    if os.path.exists(asm_path):
        with open(asm_path) as f:
            blocks = split_asm_blocks(f.read())
    else:
        sys.stderr.write(f"sim_schedule: WARNING no asm listing at {asm_path}\n")

    mnem_cache = {}

    def counts_for(sym):
        counts = mnem_cache.get(sym)
        if counts is None:
            body = blocks.get(sym)
            counts = mnemonic_counts(parse_instructions(body)) if body else Counter()
            mnem_cache[sym] = counts
        return counts

    rows = []
    for t in candidates:
        tc = counts_for(t["symbol"])
        scored = []
        for s in siblings:
            if s["symbol"] == t["symbol"]:
                continue  # a function is never its own sibling
            score = overlap_ratio(tc, counts_for(s["symbol"]))
            scored.append((score, s))
        scored.sort(key=lambda item: (-item[0], item[1]["symbol"]))
        # Pointer only for siblings with real overlap — a 0-score "closest"
        # sibling shares no codegen pattern and would mislead the brief.
        top = [item for item in scored if item[0] > 0.0][:3]
        rows.append(
            {
                "targetId": t["id"],
                "symbol": t["symbol"],
                "size": t.get("size"),
                "topSiblings": [
                    {
                        "symbol": s["symbol"],
                        "unit": s.get("unit"),
                        "status": s["status"],
                        "score": score,
                    }
                    for score, s in top
                ],
                "score": top[0][0] if top else 0.0,
            }
        )

    rows.sort(key=lambda r: (-r["score"], r["targetId"]))
    if args.limit is not None:
        rows = rows[: args.limit]

    if args.json:
        for r in rows:
            print(json.dumps(r))
        return 0

    print(f"sim_schedule: {args.unit} — {len(candidates)} candidate(s), "
          f"{len(siblings)} matched sibling(s), min-size {args.min_size or 'none'}")
    print(f"{'rank':>4}  {'targetId':<14} {'symbol':<36} {'size':>6} {'score':>6}  best sibling(s)")
    for i, r in enumerate(rows, 1):
        best = r["topSiblings"][0] if r["topSiblings"] else None
        best_str = f"{best['symbol']} ({best['status']}, {best['score']:.2f})" if best else "-"
        print(f"{i:4d}  {r['targetId']:<14} {r['symbol']:<36} {str(r['size']):>6} {r['score']:6.2f}  {best_str}")
    print(f"\n{len(rows)} ranked (of {len(candidates)} candidates)")
    return 0


if __name__ == "__main__":
    sys.exit(main())

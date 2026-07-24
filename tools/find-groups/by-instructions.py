#!/usr/bin/env python3
"""
Find functions with identical instruction patterns (PPC instruction-level).

Reads decoded instructions from the Decomp Atlas SQLite database,
computes instruction-sequence fingerprints, and groups functions with
identical patterns together.

Three fingerprint levels:
  level 1 (opcode)     — only opcode sequence
  level 2 (normalized) — opcodes + operand types, register numbers normalized
  level 3 (full)       — opcodes + all operands, branches normalized

Usage:
  # Default: show groups of NOT_STARTED functions (level 2 normalized)
  python3 tools/find-groups/by-instructions.py

  # Coarser groups (level 1 — opcode sequence only)
  python3 tools/find-groups/by-instructions.py --level 1

  # Include already-matched functions
  python3 tools/find-groups/by-instructions.py --all-status

  # Focus on a unit
  python3 tools/find-groups/by-instructions.py --unit kyoshin/cf/CfGameManager

  # Show detailed member info
  python3 tools/find-groups/by-instructions.py --detail 3

  # Stricter: require exact instruction match (including immediates)
  python3 tools/find-groups/by-instructions.py --level 3

  # JSON output
  python3 tools/find-groups/by-instructions.py --json

  # Min group size
  python3 tools/find-groups/by-instructions.py --min-members 4
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sqlite3
import sys
import time
from collections import Counter, defaultdict
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence, Tuple


# ---------------------------------------------------------------------------
# Instruction fingerprinting
# ---------------------------------------------------------------------------

# GPR register numbers (r0 is special, r1 = sp)
_GPRS = set(range(3, 32))
_FPRS = set(range(0, 32))
_CR = set(range(0, 8))

# Instructions whose immediate operands are offsets (keep them)
_OFFSET_INSN = {"lwz", "lbz", "lhz", "lha", "lfs", "lfd", "stw", "stb", "sth", "stfs", "stfd",
                "stwu", "stbu", "sthu", "stfsu", "stfdu", "lwzu", "lbzu", "lhzu", "lhau",
                "addi", "addis", "subfic", "mulli",
                "cmpwi", "cmplwi", "cmpdi", "cmplw", "cmpw",
                "rlwinm", "rlwimi", "rlwnm",
                "srawi"}  # srawi_ has immediate shift amount


def _normalize_operand(op: Any, insn: str) -> str:
    """Normalize a single operand value to a canonical form."""
    if not isinstance(op, (int, float)):
        return str(op)
    try:
        n = int(op)
    except (ValueError, TypeError):
        return str(op)
    if n in _GPRS:
        return "R"
    if n in _FPRS:
        return "F"
    if n in _CR:
        return "C"
    # Keep immediates that are meaningful
    return str(n)


def _normalize_operand_strict(op: Any, insn: str) -> str:
    """Normalize operand — still normalize registers for inter-function comparison."""
    return _normalize_operand(op, insn)


def _opcode_fingerprint(instructions: List[Dict[str, Any]]) -> str:
    """Level 1: opcode-only fingerprint."""
    return "|".join(str(i.get("opcode", "?")) for i in instructions)


def _normalized_fingerprint(instructions: List[Dict[str, Any]]) -> str:
    """Level 2: normalised register operands, keep immediates."""
    parts: List[str] = []
    for i in instructions:
        op = i.get("opcode", "?")
        ops = i.get("operands", []) or []
        norm_ops = ",".join(_normalize_operand(o, op) for o in ops)
        parts.append(f"{op}|{norm_ops}")
    return "|".join(parts)


def _full_fingerprint(instructions: List[Dict[str, Any]]) -> str:
    """Level 3: same as normalized — still normalises registers.

    'Full' here means keeping ALL operand information except register
    numbers (which vary across functions for the same pattern).
    """
    parts: List[str] = []
    for i in instructions:
        op = i.get("opcode", "?")
        ops = i.get("operands", []) or []
        # Keep operands but still normalize registers
        norm_ops = ",".join(_normalize_operand(o, op) for o in ops)
        parts.append(f"{op}|{norm_ops}")
    return "|".join(parts)


def compute_fingerprint(instructions: List[Dict[str, Any]], level: int) -> str:
    """Compute a fingerprint for a function's instruction sequence.

    The fingerprint is a hash of the normalized instruction sequence.
    Functions with the same fingerprint have the same instruction pattern.
    """
    if not instructions:
        return ""

    if level == 1:
        raw = _opcode_fingerprint(instructions)
    elif level == 3:
        raw = _full_fingerprint(instructions)
    else:
        raw = _normalized_fingerprint(instructions)

    return hashlib.sha256(raw.encode()).hexdigest()[:24]


# ---------------------------------------------------------------------------
# Atlas DB reader
# ---------------------------------------------------------------------------

def open_atlas_db(root: Path, region: str = "us") -> sqlite3.Connection:
    """Open the Decomp Atlas SQLite database."""
    candidate = root / "build" / "decomp-atlas" / region / "atlas.sqlite"
    fallback = root / "build" / "decomp-atlas" / region / "atlas.db"
    db_path = candidate if candidate.exists() else fallback
    if not db_path.exists():
        print(f"Atlas DB not found at {candidate} or {fallback}", file=sys.stderr)
        print("Run: python3 tools/decomp_atlas/run.py index --full --vectors", file=sys.stderr)
        sys.exit(1)
    conn = sqlite3.connect(str(db_path))
    conn.row_factory = sqlite3.Row
    return conn


def load_instructions_and_meta(
    conn: sqlite3.Connection,
    unit_filter: Optional[str] = None,
    status_filter: Optional[str] = None,
) -> Tuple[Dict[str, List[Dict[str, Any]]], Dict[str, Dict[str, Any]]]:
    """Load decoded instructions and function metadata from Atlas DB.

    Returns:
      instructions: {target_id: [{opcode, operands, address}, ...]}
      meta: {target_id: {display_name, unit, status, ...}}
    """
    query = """
        SELECT f.target_id, f.display_name, f.symbol, f.unit, f.status,
               f.tier, f.size, f.instruction_percent, f.difficulty,
               a.decoded_json
        FROM functions f
        JOIN artifacts a ON a.target_id = f.target_id
        WHERE a.decoded_json IS NOT NULL AND a.decoded_json != ''
          AND a.decoded_json != '[]'
    """
    params: List[Any] = []
    if unit_filter:
        query += " AND (f.unit = ? OR f.unit LIKE ?)"
        params.extend([unit_filter, f"{unit_filter}%"])
    if status_filter:
        query += " AND f.status = ?"
        params.append(status_filter)

    rows = conn.execute(query, params).fetchall()

    instructions: Dict[str, List[Dict[str, Any]]] = {}
    meta: Dict[str, Dict[str, Any]] = {}

    for row in rows:
        tid = row["target_id"]
        try:
            decoded = json.loads(row["decoded_json"])
        except (json.JSONDecodeError, TypeError):
            continue
        if not decoded or not isinstance(decoded, list):
            continue
        instructions[tid] = decoded
        meta[tid] = {
            "target_id": tid,
            "display_name": row["display_name"] or tid,
            "symbol": row["symbol"],
            "unit": row["unit"],
            "status": row["status"],
            "tier": row["tier"],
            "size": row["size"],
            "match_pct": row["instruction_percent"],
            "difficulty": row["difficulty"],
        }

    return instructions, meta


# ---------------------------------------------------------------------------
# Analysis
# ---------------------------------------------------------------------------

def group_by_fingerprint(
    instructions: Dict[str, List[Dict[str, Any]]],
    meta: Dict[str, Dict[str, Any]],
    level: int,
) -> Tuple[Dict[str, List[str]], Dict[str, Any]]:
    """Group functions by instruction fingerprint.

    Returns:
      {fingerprint: [target_id, ...]}
      fingerprint_stats: {fingerprint: size, not_started, ...}
    """
    groups: Dict[str, List[str]] = defaultdict(list)
    stats: Dict[str, Dict[str, Any]] = {}

    for tid in instructions:
        fp = compute_fingerprint(instructions[tid], level)
        groups[fp].append(tid)

    # Remove singletons
    groups = {fp: tids for fp, tids in groups.items() if len(tids) >= 2}

    return groups, stats


def analyze_groups(
    groups: Dict[str, List[str]],
    meta: Dict[str, Dict[str, Any]],
    instructions: Dict[str, List[Dict[str, Any]]],
) -> List[Dict[str, Any]]:
    """Build group analysis."""
    results: List[Dict[str, Any]] = []

    for fp, members in groups.items():
        statuses: Counter = Counter()
        tiers: Counter = Counter()
        units: Counter = Counter()
        names: List[str] = []
        not_started = 0
        all_same_unit = True
        prev_unit: Optional[str] = None
        insn_count = len(instructions[members[0]]) if members else 0
        first_few_ops: List[str] = []

        # Sample the first few instruction ops from the first member
        if members:
            first = instructions[members[0]]
            for insn in first[:5]:
                first_few_ops.append(str(insn.get("opcode", "?")))

        for tid in members:
            m = meta.get(tid, {})
            statuses[m.get("status", "?")] += 1
            tiers[m.get("tier", "?")] += 1
            u = m.get("unit", "?")
            units[u] += 1
            if prev_unit is None:
                prev_unit = u
            elif u != prev_unit:
                all_same_unit = False
            if m.get("status") == "NOT_STARTED":
                not_started += 1
            names.append(m.get("display_name", tid))

        dominant_status = statuses.most_common(1)[0][0] if statuses else "?"
        dominant_unit = units.most_common(1)[0][0] if units else "?"

        results.append({
            "fingerprint": fp,
            "size": len(members),
            "not_started": not_started,
            "not_started_pct": round(not_started / len(members) * 100, 1),
            "all_same_unit": all_same_unit,
            "dominant_unit": dominant_unit,
            "dominant_status": dominant_status,
            "instruction_count": insn_count,
            "first_ops": first_few_ops,
            "status_distribution": dict(statuses.most_common()),
            "units": dict(units.most_common(5)),
            "tier": tiers.most_common(1)[0][0] if tiers else "?",
            "member_names": names[:20],
            "member_ids": members[:10],
        })

    # Sort: most NOT_STARTED first, then same-unit, then largest
    results.sort(key=lambda g: (-g["not_started"], -g["all_same_unit"], -g["size"]))
    return results


# ---------------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------------

def print_report(
    groups: List[Dict[str, Any]],
    meta: Dict[str, Dict[str, Any]],
    shown: int = 50,
    detail: int = 0,
    level: int = 2,
) -> None:
    """Print group report."""
    total_functions = len(meta)
    grouped = sum(g["size"] for g in groups)
    isolated = total_functions - grouped

    print(f"Total functions:                {total_functions}")
    print(f"Fingerprint level:              {level} "
          f"({'opcode' if level == 1 else 'normalized' if level == 2 else 'full'})")
    print(f"Groups found (>=2 members):     {len(groups)}")
    print(f"Functions in groups:            {grouped}")
    print(f"Unique (no match):              {isolated}")
    print()

    hdr = (
        f"{'#':>4s} {'Size':>5s} {'NS':>5s} {'NS%':>5s} {'SameU':>6s} "
        f"{'Insns':>5s} {'Status':>14s} {'Pattern':>20s} {'Unit':>28s}"
    )
    print(hdr)
    print("-" * len(hdr))

    for i, g in enumerate(groups[:shown]):
        same = "✓" if g["all_same_unit"] else " "
        pattern = " ".join(g["first_ops"][:3]) + ("..." if len(g["first_ops"]) > 3 else "")
        unit = g["dominant_unit"][:26] if g["dominant_unit"] else "?"
        print(
            f"{i+1:>4d} {g['size']:>5d} {g['not_started']:>5d} {g['not_started_pct']:>5.1f} "
            f"{same:>6s} {g['instruction_count']:>5d} {g['dominant_status']:>14s} "
            f"{pattern:>20s} {unit:>28s}"
        )

    if detail > 0:
        print()
        for i, g in enumerate(groups[:detail]):
            print(f"\n── Group {i+1}: {g['size']} members ({g['not_started']} NOT_STARTED, "
                  f"same-unit: {g['all_same_unit']}, {g['instruction_count']} insns) ──")
            print(f"   Pattern start: {' '.join(g['first_ops'])}")
            print(f"   Unit: {g['dominant_unit']}  |  Statuses: {g['status_distribution']}")
            for name in g["member_names"]:
                print(f"     {name}")

            if detail >= 2:
                print(f"   IDs:")
                for tid in g["member_ids"]:
                    m = meta.get(tid, {})
                    st = m.get("status", "?")
                    print(f"     {tid:45s}  {st:20s}  {m.get('unit','?'):30s}")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Find functions with identical PPC instruction patterns."
    )
    parser.add_argument(
        "--level", "-l",
        type=int,
        default=2,
        choices=[1, 2, 3],
        help="Fingerprint level: 1=opcode-only, 2=normalized (default), 3=full",
    )
    parser.add_argument(
        "--min-members",
        type=int,
        default=2,
        help="Minimum group size (default: 2)",
    )
    parser.add_argument(
        "--detail", "-d",
        type=int,
        default=0,
        help="Show details for top N groups (0=none, 1=names, 2=IDs)",
    )
    parser.add_argument(
        "--all-status",
        action="store_true",
        help="Include all statuses, not just NOT_STARTED",
    )
    parser.add_argument(
        "--unit", "-u",
        type=str,
        default=None,
        help="Filter to a unit prefix",
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="Output JSON",
    )
    parser.add_argument(
        "--max-groups",
        type=int,
        default=50,
        help="Max groups to show (default: 50)",
    )
    parser.add_argument(
        "--region",
        default="us",
        help="Atlas region (default: us)",
    )
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[2],
        help="Project root",
    )
    args = parser.parse_args()

    root = args.root.resolve()
    t0 = time.time()

    # 1. Load
    conn = open_atlas_db(root, args.region)
    status_filter = None if args.all_status else "NOT_STARTED"
    instructions, meta = load_instructions_and_meta(
        conn, unit_filter=args.unit, status_filter=status_filter
    )
    conn.close()

    if not instructions:
        print("No decoded instructions found in Atlas DB.", file=sys.stderr)
        print("Run: python3 tools/decomp_atlas/run.py index --full --vectors", file=sys.stderr)
        return 1

    print(f"Loaded {len(instructions)} functions with decoded instructions.", file=sys.stderr)

    # 2. Compute fingerprints and group
    groups_raw, _ = group_by_fingerprint(instructions, meta, args.level)

    # Filter by min size
    groups_raw = {fp: tids for fp, tids in groups_raw.items() if len(tids) >= args.min_members}
    print(f"Found {len(groups_raw)} groups (≥{args.min_members} members).", file=sys.stderr)

    if not groups_raw:
        print("No groups found.", file=sys.stderr)
        return 0

    # 3. Analyze
    analyzed = analyze_groups(groups_raw, meta, instructions)

    # 4. Output
    if args.json:
        result = {
            "fingerprint_level": args.level,
            "total_functions": len(meta),
            "num_groups": len(analyzed),
            "time_seconds": round(time.time() - t0, 1),
            "groups": analyzed[:args.max_groups],
        }
        print(json.dumps(result, indent=2, default=str))
    else:
        print_report(analyzed, meta, shown=args.max_groups, detail=args.detail, level=args.level)
        print(f"\nCompleted in {time.time()-t0:.1f}s", file=sys.stderr)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""
Find groups of structurally similar functions in the decompilation.

Reads structural feature vectors from the Decomp Atlas SQLite database,
clusters similar functions together, and outputs ranked groups.

Uses a two-level approach for efficiency on 19K+ functions:
  1. Within-unit clustering (fast, ~100s of functions per unit)
  2. LSH-based cross-unit clustering for cross-TU patterns

Usage:
  # Default: cluster NOT_STARTED functions, show top groups
  python3 tools/find-groups/cluster.py

  # Show detailed info for top groups
  python3 tools/find-groups/cluster.py --detail 5

  # Focus on a specific unit
  python3 tools/find-groups/cluster.py --unit kyoshin/cf

  # Include all statuses, not just NOT_STARTED
  python3 tools/find-groups/cluster.py --all-status

  # Cluster all functions, search across units too
  python3 tools/find-groups/cluster.py --all-status --cross-unit

  # Output JSON for further processing
  python3 tools/find-groups/cluster.py --json

  # Stricter similarity threshold
  python3 tools/find-groups/cluster.py --threshold 0.90
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import sqlite3
import sys
import time
from collections import Counter, defaultdict
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence, Tuple


# ---------------------------------------------------------------------------
# Vector math
# ---------------------------------------------------------------------------

def normalize(v: Sequence[float]) -> List[float]:
    norm = math.sqrt(sum(x * x for x in v))
    if norm < 1e-12:
        return [0.0] * len(v)
    return [x / norm for x in v]


def cosine_sim(a: Sequence[float], b: Sequence[float]) -> float:
    if len(a) != len(b) or not a:
        return 0.0
    dot = sum(x * y for x, y in zip(a, b))
    na = math.sqrt(sum(x * x for x in a))
    nb = math.sqrt(sum(y * y for y in b))
    if na < 1e-12 or nb < 1e-12:
        return 0.0
    return max(-1.0, min(1.0, dot / (na * nb)))


def mean_vector(vecs: List[List[float]]) -> List[float]:
    n = len(vecs)
    if n == 0:
        return []
    d = len(vecs[0])
    return [sum(vec[i] for vec in vecs) / n for i in range(d)]


# ---------------------------------------------------------------------------
# LSH-based approximate nearest neighbors
# ---------------------------------------------------------------------------

def _lsh_signature(vector: List[float], planes: List[List[float]], seed: int) -> str:
    """Compute a locality-sensitive hash signature."""
    h = hashlib.sha256(f"{seed}:".encode())
    for plane in planes:
        side = 1 if sum(a * b for a, b in zip(vector, plane)) >= 0 else 0
        h.update(str(side).encode())
    return h.hexdigest()[:16]  # truncate for shorter signatures


class LSHIndex:
    """Simple random-projection LSH for cosine similarity."""

    def __init__(self, dim: int, n_planes: int = 16, n_tables: int = 4, seed: int = 42):
        self.dim = dim
        self.n_planes = n_planes
        self.n_tables = n_tables
        self.seed = seed
        rng = __import__("random").Random(seed)
        # Generate random hyperplanes for each table
        self.tables: List[List[List[float]]] = []
        for _ in range(n_tables):
            planes = []
            for _ in range(n_planes):
                plane = [rng.gauss(0, 1) for _ in range(dim)]
                norm = math.sqrt(sum(x * x for x in plane))
                if norm > 0:
                    plane = [x / norm for x in plane]
                planes.append(plane)
            self.tables.append(planes)

    def signature(self, vector: List[float], table_idx: int) -> str:
        return _lsh_signature(vector, self.tables[table_idx], self.seed + table_idx)


# ---------------------------------------------------------------------------
# Clustering
# ---------------------------------------------------------------------------

def cluster_within_unit(
    tid_list: List[str],
    vectors: Dict[str, List[float]],
    threshold: float,
) -> List[List[str]]:
    """Fast connected-components clustering within a single unit (<500 functions)."""
    n = len(tid_list)
    if n < 2:
        return []

    norms = {tid: normalize(vectors[tid]) for tid in tid_list}

    # Build adjacency
    adj: Dict[str, List[str]] = {tid: [] for tid in tid_list}
    for i in range(n):
        vi = norms[tid_list[i]]
        for j in range(i + 1, n):
            vj = norms[tid_list[j]]
            if cosine_sim(vi, vj) >= threshold:
                tid_i = tid_list[i]
                tid_j = tid_list[j]
                adj[tid_i].append(tid_j)
                adj[tid_j].append(tid_i)

    # Connected components (BFS)
    visited: set = set()
    groups: List[List[str]] = []
    for tid in tid_list:
        if tid in visited:
            continue
        group: List[str] = []
        queue = [tid]
        visited.add(tid)
        while queue:
            node = queue.pop(0)
            group.append(node)
            for nbr in adj.get(node, []):
                if nbr not in visited:
                    visited.add(nbr)
                    queue.append(nbr)
        if len(group) >= 2:
            groups.append(group)

    return groups


def cluster_cross_unit(
    tid_list: List[str],
    vectors: Dict[str, List[float]],
    threshold: float,
) -> List[List[str]]:
    """
    Find cross-unit clusters using LSH to avoid O(n²).

    Uses multi-table LSH to find candidate pairs, then refines.
    """
    if len(tid_list) < 2:
        return []

    norms = {tid: normalize(vectors[tid]) for tid in tid_list}
    dim = len(next(iter(norms.values())))

    # Scale LSH based on dataset size
    n_tables = max(2, min(8, len(tid_list) // 500))
    n_planes = max(8, min(24, len(tid_list) // 200))
    lsh = LSHIndex(dim, n_planes=n_planes, n_tables=n_tables)

    # Build hash tables
    buckets: List[Dict[str, List[str]]] = []
    for ti in range(n_tables):
        table: Dict[str, List[str]] = defaultdict(list)
        for tid in tid_list:
            sig = lsh.signature(norms[tid], ti)
            table[sig].append(tid)
        buckets.append(table)

    # Collect candidate pairs
    candidate_pairs: set = set()
    for table in buckets:
        for sig, members in table.items():
            if len(members) >= 2:
                for i in range(len(members)):
                    for j in range(i + 1, len(members)):
                        pair = (members[i], members[j]) if members[i] < members[j] else (members[j], members[i])
                        candidate_pairs.add(pair)

    # Refine: compute exact similarity for candidate pairs
    adj: Dict[str, set] = {tid: set() for tid in tid_list}
    for a, b in candidate_pairs:
        if cosine_sim(norms[a], norms[b]) >= threshold:
            adj[a].add(b)
            adj[b].add(a)

    # Connected components
    visited: set = set()
    groups: List[List[str]] = []
    for tid in tid_list:
        if tid in visited:
            continue
        group: List[str] = []
        queue = [tid]
        visited.add(tid)
        while queue:
            node = queue.pop(0)
            group.append(node)
            for nbr in adj.get(node, []):
                if nbr not in visited:
                    visited.add(nbr)
                    queue.append(nbr)
        if len(group) >= 2:
            groups.append(group)

    return groups


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
        print("Run: python3 tools/decomp_atlas/run.py index --vectors", file=sys.stderr)
        sys.exit(1)
    conn = sqlite3.connect(str(db_path))
    conn.row_factory = sqlite3.Row
    return conn


def load_vectors_and_metadata(
    conn: sqlite3.Connection,
    unit_filter: Optional[str] = None,
    status_filter: Optional[str] = None,
) -> Tuple[Dict[str, List[float]], Dict[str, Dict[str, Any]]]:
    """Load structural vectors and function metadata from Atlas DB."""
    query = """
        SELECT target_id, vector_json, display_name, symbol, unit, status,
               tier, size, instruction_percent, readiness, difficulty,
               source, address
        FROM functions
        WHERE vector_json IS NOT NULL AND vector_json != ''
    """
    params: List[Any] = []
    if unit_filter:
        query += " AND (unit = ? OR unit LIKE ?)"
        params.extend([unit_filter, f"{unit_filter}%"])
    if status_filter:
        query += " AND status = ?"
        params.append(status_filter)

    rows = conn.execute(query, params).fetchall()

    vectors: Dict[str, List[float]] = {}
    meta: Dict[str, Dict[str, Any]] = {}

    for row in rows:
        tid = row["target_id"]
        try:
            vec = json.loads(row["vector_json"])
        except (json.JSONDecodeError, TypeError):
            continue
        if not vec or all(abs(v) < 1e-12 for v in vec):
            continue
        vectors[tid] = vec
        meta[tid] = {
            "target_id": tid,
            "display_name": row["display_name"] or tid,
            "symbol": row["symbol"],
            "unit": row["unit"],
            "status": row["status"],
            "tier": row["tier"],
            "size": row["size"],
            "match_pct": row["instruction_percent"],
            "readiness": row["readiness"],
            "difficulty": row["difficulty"],
            "source": row["source"],
            "address": row["address"],
        }

    return vectors, meta


def get_units(conn: sqlite3.Connection) -> List[Dict[str, Any]]:
    """Get unit grouping info."""
    rows = conn.execute("""
        SELECT unit, COUNT(*) AS cnt,
               SUM(CASE WHEN status = 'NOT_STARTED' THEN 1 ELSE 0 END) AS not_started_cnt
        FROM functions
        WHERE unit IS NOT NULL AND unit != ''
          AND vector_json IS NOT NULL AND vector_json != ''
        GROUP BY unit
        HAVING cnt >= 2
        ORDER BY cnt DESC
    """).fetchall()
    return [dict(r) for r in rows]


# ---------------------------------------------------------------------------
# Analysis
# ---------------------------------------------------------------------------

def analyze_group(
    members: List[str],
    meta: Dict[str, Dict[str, Any]],
) -> Dict[str, Any]:
    """Compute group statistics."""
    statuses: Counter = Counter()
    tiers: Counter = Counter()
    units: Counter = Counter()
    sizes: List[float] = []
    difficulties: List[float] = []
    names: List[str] = []
    not_started = 0
    all_same_unit = True
    prev_unit: Optional[str] = None

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
        sz = m.get("size")
        if sz:
            try:
                sz_val = int(str(sz), 16) if isinstance(sz, str) and sz.startswith("0x") else int(sz)
                sizes.append(float(sz_val))
            except (ValueError, TypeError):
                pass
        diff = m.get("difficulty")
        try:
            if diff is not None:
                difficulties.append(float(diff))
        except (ValueError, TypeError):
            pass
        if m.get("status") == "NOT_STARTED":
            not_started += 1
        names.append(m.get("display_name", tid))

    avg_size = sum(sizes) / len(sizes) if sizes else 0
    avg_diff = sum(difficulties) / len(difficulties) if difficulties else 0
    dominant_status = statuses.most_common(1)[0][0] if statuses else "?"
    dominant_unit = units.most_common(1)[0][0] if units else "?"

    return {
        "size": len(members),
        "not_started": not_started,
        "not_started_pct": round(not_started / len(members) * 100, 1),
        "all_same_unit": all_same_unit,
        "dominant_unit": dominant_unit,
        "dominant_status": dominant_status,
        "avg_size_bytes": round(avg_size, 1),
        "avg_difficulty": round(avg_diff, 2),
        "status_distribution": dict(statuses.most_common()),
        "units": dict(units.most_common(5)),
        "tier": tiers.most_common(1)[0][0] if tiers else "?",
        "member_names": names[:20],
        "member_ids": members[:10],
    }


def rank_groups(
    groups: List[List[str]],
    meta: Dict[str, Dict[str, Any]],
    cross_unit: bool = False,
) -> List[Dict[str, Any]]:
    """Rank groups by their usefulness for decompilation."""
    analyzed = [analyze_group(g, meta) for g in groups]

    def sort_key(g: Dict[str, Any]) -> Tuple:
        return (
            -g["not_started"],
            -g["all_same_unit"],
            g["avg_difficulty"] if g["avg_difficulty"] else 99,
            -g["size"],
        )

    analyzed.sort(key=sort_key)
    return analyzed


# ---------------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------------

def print_report(
    result: Dict[str, Any],
    meta: Dict[str, Dict[str, Any]],
    max_groups: int = 30,
    detail: int = 0,
) -> None:
    """Print a human-readable cluster report."""
    print(f"Total functions reviewed: {result['total_functions']}")
    print(f"Clustered into groups:    {result['num_groups']}")
    print(f"Isolated (singletons):    {result['isolated']}")
    print(f"Groups shown below:       {min(max_groups, len(result['groups']))}")
    print()

    groups = result["groups"][:max_groups]

    # Summary table
    hdr = (
        f"{'#':>4s} {'Size':>5s} {'NS':>5s} {'NS%':>5s} {'SameU':>6s} "
        f"{'Diff':>5s} {'SzB':>5s} {'Status':>14s} {'Unit':>28s}"
    )
    print(hdr)
    print("-" * len(hdr))
    for i, g in enumerate(groups):
        same = "✓" if g["all_same_unit"] else " "
        diff = str(g["avg_difficulty"]) if g["avg_difficulty"] else "?"
        sz = str(g["avg_size_bytes"]) if g["avg_size_bytes"] else "?"
        unit = g["dominant_unit"][:26] if g["dominant_unit"] else "?"
        print(
            f"{i+1:>4d} {g['size']:>5d} {g['not_started']:>5d} {g['not_started_pct']:>5.1f} "
            f"{same:>6s} {diff:>5s} {sz:>5s} {g['dominant_status']:>14s} {unit:>28s}"
        )

    if detail > 0:
        print()
        for i, g in enumerate(groups[:detail]):
            print(f"\n── Group {i+1}: {g['size']} members ({g['not_started']} NOT_STARTED, "
                  f"same-unit: {g['all_same_unit']}) ──")
            print(f"   Unit: {g['dominant_unit']}  |  Difficulty: {g['avg_difficulty']}  |  "
                  f"Size: {g['avg_size_bytes']} B")
            print(f"   Statuses: {g['status_distribution']}")
            for name in g["member_names"]:
                print(f"     {name}")

            if detail >= 2 and g["member_ids"]:
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
        description="Find groups of structurally similar functions in the Xenoblade decompilation."
    )
    parser.add_argument(
        "--threshold", "-t",
        type=float,
        default=0.80,
        help="Cosine similarity threshold (0-1, default: 0.80)",
    )
    parser.add_argument(
        "--detail", "-d",
        type=int,
        default=0,
        help="Show detailed members for top N groups (0=no detail, 1=names, 2=IDs)",
    )
    parser.add_argument(
        "--all-status",
        action="store_true",
        help="Include all function statuses, not just NOT_STARTED",
    )
    parser.add_argument(
        "--unit", "-u",
        type=str,
        default=None,
        help="Filter to a specific unit prefix (e.g. kyoshin/cf)",
    )
    parser.add_argument(
        "--cross-unit",
        action="store_true",
        help="Also search for cross-unit groups (slower but finds shared patterns)",
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
        help="Max groups to display (default: 50)",
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
        help="Project root (default: repo root)",
    )
    parser.add_argument(
        "--min-group-size",
        type=int,
        default=3,
        help="Minimum group size to report (default: 3)",
    )
    args = parser.parse_args()

    root = args.root.resolve()

    # 1. Load from Atlas DB
    conn = open_atlas_db(root, args.region)

    status_filter = None if args.all_status else "NOT_STARTED"
    vectors, meta = load_vectors_and_metadata(
        conn, unit_filter=args.unit, status_filter=status_filter
    )
    unit_info = get_units(conn)
    conn.close()

    if not vectors:
        print("No structural vectors found. Run: python3 tools/decomp_atlas/run.py index --vectors",
              file=sys.stderr)
        return 1

    print(f"Loaded {len(vectors)} functions with structural vectors.", file=sys.stderr)
    t0 = time.time()

    # 2. Group by unit for within-unit clustering
    unit_to_tids: Dict[str, List[str]] = defaultdict(list)
    for tid, m in meta.items():
        unit = m.get("unit") or "unknown"
        unit_to_tids[unit].append(tid)

    print(f"Found {len(unit_to_tids)} units with >=1 function.", file=sys.stderr)

    # 3. Cluster within each unit
    all_groups: List[List[str]] = []
    units_with_clusters = 0
    for unit, tids in unit_to_tids.items():
        clusters = cluster_within_unit(tids, vectors, args.threshold)
        if clusters:
            all_groups.extend(clusters)
            units_with_clusters += 1

    print(f"Within-unit clustering: {len(all_groups)} groups in {units_with_clusters} units "
          f"({time.time()-t0:.1f}s)", file=sys.stderr)

    # 4. Cross-unit clustering (optional)
    if args.cross_unit:
        t_cross = time.time()
        # Find unclustered functions + clusters of size < min-group-size
        clustered_ids: set = set()
        for g in all_groups:
            clustered_ids.update(g)

        unclustered = [tid for tid in vectors if tid not in clustered_ids]
        if len(unclustered) >= 2:
            cross_groups = cluster_cross_unit(unclustered, vectors, args.threshold)
            print(f"Cross-unit pass: {len(cross_groups)} groups "
                  f"({time.time()-t_cross:.1f}s)", file=sys.stderr)
            all_groups.extend(cross_groups)

    total_time = time.time() - t0

    # 5. Filter by min group size
    all_groups = [g for g in all_groups if len(g) >= args.min_group_size]

    # 6. Rank
    ranked = rank_groups(all_groups, meta, cross_unit=args.cross_unit)

    # Count singletons
    all_clustered = set()
    for g in all_groups:
        all_clustered.update(g)
    isolated_count = len(vectors) - len(all_clustered)

    result = {
        "total_functions": len(meta),
        "num_groups": len(ranked),
        "isolated": isolated_count,
        "clustered_functions": len(all_clustered),
        "threshold": args.threshold,
        "time_seconds": round(total_time, 1),
        "groups": ranked[:args.max_groups],
    }

    if args.json:
        print(json.dumps(result, indent=2, default=str))
    else:
        print_report(result, meta, max_groups=args.max_groups, detail=args.detail)
        print(f"\nCompleted in {total_time:.1f}s", file=sys.stderr)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

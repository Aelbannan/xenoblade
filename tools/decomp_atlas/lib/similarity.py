"""Structural feature vectors, cosine similarity, neighbors, and 2D projection."""

from __future__ import annotations

import hashlib
import math
import random
from typing import Any, Dict, Iterable, List, Optional, Sequence, Tuple

from tools.decomp_atlas.lib.config import PROJECTION_SEED, PROJECTION_VERSION, VECTOR_METHOD

# Fixed feature layout so cosine similarity is comparable across functions.
FEATURE_KEYS: Tuple[str, ...] = (
    "len_log",
    "size_log",
    "branch_density",
    "call_direct",
    "call_unresolved",
    "indirect",
    "reloc_count",
    "stack_frame_log",
    "match_percent",
    "cls_arith",
    "cls_logical",
    "cls_load",
    "cls_store",
    "cls_branch",
    "cls_float",
    "cls_compare",
    "cls_other",
)

_ARITH = {
    "addi", "addis", "addic", "addic.", "subfic", "mulli", "add", "addc", "adde",
    "addme", "addze", "subf", "subfc", "subfe", "subfme", "subfze", "neg",
    "mulhw", "mulhwu", "mullw", "divw", "divwu",
}
_LOGICAL = {
    "ori", "oris", "xori", "xoris", "andi.", "andis.", "and", "andc", "eqv",
    "nand", "nor", "or", "orc", "xor", "slw", "srw", "sraw", "srawi",
    "rlwinm", "rlwimi", "rlwnm", "extsb", "extsh", "cntlzw",
}
_LOAD = {
    "lbz", "lbzu", "lhz", "lhzu", "lha", "lhau", "lwz", "lwzu", "lmw",
    "lbzx", "lbzux", "lhzx", "lhzux", "lhax", "lhaux", "lwzx", "lwzux",
    "lhbrx", "lwbrx", "lfs", "lfsu", "lfd", "lfdu", "lfsx", "lfsux", "lfdx", "lfdux",
}
_STORE = {
    "stb", "stbu", "sth", "sthu", "stw", "stwu", "stmw",
    "stbx", "stbux", "sthx", "sthux", "stwx", "stwux",
    "sthbrx", "stwbrx", "stfs", "stfsu", "stfd", "stfdu",
    "stfsx", "stfsux", "stfdx", "stfdux", "stfiwx",
}
_BRANCH = {"b", "bc", "bclr", "bcctr"}
_COMPARE = {"cmpwi", "cmplwi", "cmpw", "cmplw", "fcmpu", "fcmpo"}
_FLOAT = {
    "fadds", "fsubs", "fmuls", "fdivs", "fres", "fmadds", "fmsubs", "fnmadds",
    "fnmsubs", "fadd", "fsub", "fmul", "fdiv", "frsqrte", "fsel", "fmadd",
    "fmsub", "fnmadd", "fnmsub", "frsp", "fctiw", "fctiwz", "fneg", "fmr",
    "fnabs", "fabs",
}


def _log1p(value: float) -> float:
    return math.log1p(max(0.0, value))


def _opcode_class(opcode: str) -> str:
    if opcode in _ARITH:
        return "cls_arith"
    if opcode in _LOGICAL:
        return "cls_logical"
    if opcode in _LOAD:
        return "cls_load"
    if opcode in _STORE:
        return "cls_store"
    if opcode in _BRANCH:
        return "cls_branch"
    if opcode in _COMPARE:
        return "cls_compare"
    if opcode in _FLOAT or opcode.startswith("ps_") or opcode.startswith("psq_"):
        return "cls_float"
    return "cls_other"


def structural_vector(
    *,
    instructions: Optional[Sequence[Any]] = None,
    size: Optional[float] = None,
    direct_call_count: float = 0.0,
    unresolved_call_count: float = 0.0,
    has_indirect_calls: bool = False,
    relocation_count: Optional[float] = None,
    stack_frame: Optional[float] = None,
    instruction_count: Optional[float] = None,
    branch_count: Optional[float] = None,
    match_percent: Optional[float] = None,
) -> List[float]:
    """Build a fixed-length structural feature vector."""
    features = {key: 0.0 for key in FEATURE_KEYS}
    insn_n = float(instruction_count or 0.0)
    branch_n = float(branch_count or 0.0)
    size_n = float(size or 0.0)

    if instructions:
        insn_n = float(len(instructions))
        branch_n = 0.0
        for insn in instructions:
            opcode = getattr(getattr(insn, "opcode", None), "value", None)
            if opcode is None:
                opcode = str(getattr(insn, "opcode", "other"))
            opcode = str(opcode)
            features[_opcode_class(opcode)] += 1.0
            if opcode in _BRANCH:
                branch_n += 1.0
        if insn_n > 0:
            for key in (
                "cls_arith", "cls_logical", "cls_load", "cls_store",
                "cls_branch", "cls_float", "cls_compare", "cls_other",
            ):
                features[key] /= insn_n

    features["len_log"] = _log1p(insn_n)
    features["size_log"] = _log1p(size_n if size_n > 0 else insn_n * 4.0)
    features["branch_density"] = (branch_n / insn_n) if insn_n > 0 else 0.0
    features["call_direct"] = _log1p(float(direct_call_count))
    features["call_unresolved"] = _log1p(float(unresolved_call_count))
    features["indirect"] = 1.0 if has_indirect_calls else 0.0
    features["reloc_count"] = _log1p(float(relocation_count or 0.0))
    features["stack_frame_log"] = _log1p(float(stack_frame or 0.0))
    features["match_percent"] = max(0.0, min(1.0, float(match_percent or 0.0) / 100.0))
    return [features[key] for key in FEATURE_KEYS]


def normalize_vector(vector: Sequence[float]) -> List[float]:
    norm = math.sqrt(sum(v * v for v in vector))
    if norm <= 1e-12:
        return [0.0 for _ in vector]
    return [v / norm for v in vector]


def cosine_similarity(a: Sequence[float], b: Sequence[float]) -> float:
    if len(a) != len(b) or not a:
        return 0.0
    dot = sum(x * y for x, y in zip(a, b))
    na = math.sqrt(sum(x * x for x in a))
    nb = math.sqrt(sum(y * y for y in b))
    if na <= 1e-12 or nb <= 1e-12:
        return 0.0
    return max(-1.0, min(1.0, dot / (na * nb)))


def hash_layout_2d(target_id: str, seed: int = PROJECTION_SEED) -> Tuple[float, float]:
    digest = hashlib.sha256(f"{seed}:{target_id}".encode("utf-8")).digest()
    x = int.from_bytes(digest[:8], "big") / float(1 << 64) * 2.0 - 1.0
    y = int.from_bytes(digest[8:16], "big") / float(1 << 64) * 2.0 - 1.0
    return x, y


def project_2d(
    vectors: Dict[str, Sequence[float]],
    *,
    seed: int = PROJECTION_SEED,
) -> Dict[str, Tuple[float, float]]:
    """Project vectors to 2D with a seeded random projection (pure Python).

    When a vector is missing or zero, fall back to a deterministic hash layout.
    """
    ids = list(vectors.keys())
    if not ids:
        return {}

    dim = len(next(iter(vectors.values()))) if vectors else 0
    rng = random.Random(seed)
    # Two orthonormal-ish random directions.
    w1 = [rng.uniform(-1.0, 1.0) for _ in range(dim)]
    w2 = [rng.uniform(-1.0, 1.0) for _ in range(dim)]
    w1 = normalize_vector(w1)
    # Gram-Schmidt against w1.
    dot = sum(a * b for a, b in zip(w2, w1))
    w2 = normalize_vector([b - dot * a for a, b in zip(w1, w2)])

    points: Dict[str, Tuple[float, float]] = {}
    xs: List[float] = []
    ys: List[float] = []
    for target_id, vector in vectors.items():
        if not vector or all(abs(v) < 1e-12 for v in vector):
            points[target_id] = hash_layout_2d(target_id, seed)
            continue
        nv = normalize_vector(vector)
        x = sum(a * b for a, b in zip(nv, w1))
        y = sum(a * b for a, b in zip(nv, w2))
        points[target_id] = (x, y)
        xs.append(x)
        ys.append(y)

    if xs:
        mean_x = sum(xs) / len(xs)
        mean_y = sum(ys) / len(ys)
        for target_id, (x, y) in list(points.items()):
            if target_id in vectors and vectors[target_id] and any(
                abs(v) >= 1e-12 for v in vectors[target_id]
            ):
                points[target_id] = (x - mean_x, y - mean_y)
    return points


def compute_neighbors(
    vectors: Dict[str, Sequence[float]],
    *,
    top_k: int = 10,
    method: str = VECTOR_METHOD,
    max_catalog: int = 2500,
) -> List[Dict[str, Any]]:
    """Return neighbor rows ``{target_id, neighbor_id, score, method, rank}``.

    Full pairwise precompute is O(n²). For large catalogs return an empty list
    and compute per-target neighbors on demand via :func:`neighbors_for_target`.
    """
    ids = list(vectors.keys())
    if len(ids) > max_catalog:
        return []
    rows: List[Dict[str, Any]] = []
    for tid in ids:
        rows.extend(neighbors_for_target(tid, vectors, top_k=top_k, method=method))
    return rows


def neighbors_for_target(
    target_id: str,
    vectors: Dict[str, Sequence[float]],
    *,
    top_k: int = 10,
    method: str = VECTOR_METHOD,
) -> List[Dict[str, Any]]:
    """O(n) neighbors for a single target against a vector catalog."""
    if target_id not in vectors:
        return []
    va = normalize_vector(vectors[target_id])
    scored: List[Tuple[str, float]] = []
    for other, vector in vectors.items():
        if other == target_id:
            continue
        score = cosine_similarity(va, normalize_vector(vector))
        if score > 0.0:
            scored.append((other, score))
    scored.sort(key=lambda item: (-item[1], item[0]))
    return [
        {
            "target_id": target_id,
            "neighbor_id": neighbor_id,
            "id": neighbor_id,
            "score": round(score, 6),
            "method": method,
            "rank": rank,
        }
        for rank, (neighbor_id, score) in enumerate(scored[:top_k], start=1)
    ]


def projection_rows(
    points: Dict[str, Tuple[float, float]],
    *,
    version: str = PROJECTION_VERSION,
) -> List[Dict[str, Any]]:
    return [
        {"target_id": tid, "x": float(xy[0]), "y": float(xy[1]), "version": version}
        for tid, xy in points.items()
    ]

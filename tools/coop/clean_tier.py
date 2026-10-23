#!/usr/bin/env python3
"""Derived "clean" faithfulness tier on top of FULL_MATCH/EQUIVALENT_MATCH.

A function is **clean** when it is accepted (``targets.json`` status
FULL_MATCH or EQUIVALENT_MATCH) AND its source body carries zero tracked
smells (the gated ``smell_report.METRICS`` families: extern "C", ``self``
params, ``void*``, raw pointer / deref arithmetic, inline asm, rN params,
goto, fakematch-candidate shapes). A TU is clean when every registered target
in it is clean.

The tier is purely derived (targets.json + a source scan) — it is not a
registry status. Function bodies are located with the same column-0
definition-span heuristic as ``ptr_sites`` (a span runs to the next
definition), so this is a burn-down metric, not a C++ parser. Accepted targets
whose definition cannot be located are reported as ``unlocated`` and never
counted clean. When several spans match one target (overloads), the target is
clean only if all of them are.

Usage:
    python3 tools/coop/clean_tier.py            # summary to stdout
    python3 tools/coop/clean_tier.py --dirty    # list accepted-but-dirty functions
"""

from __future__ import annotations

import argparse
import json
import sys
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.ptr_sites import (  # noqa: E402
    ACCEPTED_STATUSES,
    TARGETS,
    _chain_ok,
    _qualified,
    _read_lines,
    function_spans,
)
from tools.coop.smell_scan import scan_text  # noqa: E402

# Must mirror smell_report.METRICS keys (kept here to avoid an import cycle;
# test_smell_ratchet pins the equality).
CLEAN_METRICS = (
    "extern_c_nonlbl_decl",
    "extern_c_nonlbl_def",
    "self_params",
    "void_ptr",
    "ptr_arith",
    "deref_arith",
    "asm_code",
    "rn_params",
    "goto_count",
    "asm_insn_shim",
    "schedule_pragma",
    "init_side_effect",
)


@dataclass
class FuncResult:
    id: str
    source: str
    function: str
    size: int
    accepted: bool
    located: bool
    smells: dict[str, int] = field(default_factory=dict)

    @property
    def clean(self) -> bool:
        return self.accepted and self.located and not self.smells


def body_smells(text: str) -> dict[str, int]:
    st = scan_text(text)
    return {k: getattr(st, k) for k in CLEAN_METRICS if getattr(st, k)}


def _span_matches(span_name: str, fn: str, symbol: str = "") -> bool:
    if span_name == fn or (symbol and span_name == symbol):
        return True
    if span_name.split("::")[-1] != fn.split("::")[-1]:
        return False
    if "::" not in fn:
        return True  # registry records the name unqualified: last-component match
    if "::" not in span_name:
        return False
    return _chain_ok(span_name, fn)


def _size(v) -> int:
    if isinstance(v, int):
        return v
    try:
        return int(str(v), 0)
    except (TypeError, ValueError):
        return 0


def _target_name(t: dict) -> str:
    fn = _qualified(t.get("function") or "")
    if fn:
        return fn
    sym = t.get("symbol") or ""
    return sym.split("__")[0] if "__" in sym else sym


def classify(targets: list[dict], roots, root: Path = ROOT) -> list[FuncResult]:
    """One FuncResult per target whose source lives under ``roots``."""
    by_src: dict[str, list[dict]] = {}
    for t in targets:
        src = t.get("source") or ""
        if any(src.startswith(r.rstrip("/") + "/") for r in roots):
            by_src.setdefault(src, []).append(t)
    out: list[FuncResult] = []
    for src in sorted(by_src):
        lines = _read_lines(root / src)
        spans = function_spans(lines)
        span_smells: dict[int, dict[str, int]] = {}
        for t in by_src[src]:
            fn = _target_name(t)
            accepted = t.get("status") in ACCEPTED_STATUSES
            res = FuncResult(
                id=t.get("id", ""), source=src, function=fn,
                size=_size(t.get("size")), accepted=accepted, located=False,
            )
            if accepted and fn:
                for k, (start, end, name) in enumerate(spans):
                    if not _span_matches(name, fn, t.get("symbol") or ""):
                        continue
                    res.located = True
                    if k not in span_smells:
                        span_smells[k] = body_smells("\n".join(lines[start - 1:end]))
                    for m, v in span_smells[k].items():
                        res.smells[m] = res.smells.get(m, 0) + v
            out.append(res)
    return out


@dataclass
class Summary:
    accepted: int = 0
    accepted_bytes: int = 0
    clean: int = 0
    clean_bytes: int = 0
    dirty: int = 0
    dirty_bytes: int = 0
    unlocated: int = 0
    unlocated_bytes: int = 0
    tus: int = 0
    clean_tus: int = 0
    dirty_by_metric: dict[str, int] = field(default_factory=dict)
    per_tu: dict[str, dict[str, int]] = field(default_factory=dict)


def summarize(results: list[FuncResult]) -> Summary:
    s = Summary()
    tu_all_clean: dict[str, bool] = {}
    for r in results:
        tu = s.per_tu.setdefault(r.source, {"clean": 0, "clean_bytes": 0, "dirty": 0, "dirty_bytes": 0})
        tu_all_clean[r.source] = tu_all_clean.get(r.source, True) and r.clean
        if not r.accepted:
            continue
        s.accepted += 1
        s.accepted_bytes += r.size
        if not r.located:
            s.unlocated += 1
            s.unlocated_bytes += r.size
        elif r.clean:
            s.clean += 1
            s.clean_bytes += r.size
            tu["clean"] += 1
            tu["clean_bytes"] += r.size
        else:
            s.dirty += 1
            s.dirty_bytes += r.size
            tu["dirty"] += 1
            tu["dirty_bytes"] += r.size
            for m in r.smells:
                s.dirty_by_metric[m] = s.dirty_by_metric.get(m, 0) + 1
    s.tus = len(tu_all_clean)
    s.clean_tus = sum(tu_all_clean.values())
    return s


def compute(roots, targets_path: Path | None = None) -> tuple[list[FuncResult], Summary]:
    data = json.loads(Path(targets_path or TARGETS).read_text())
    results = classify(data.get("targets", []), roots)
    return results, summarize(results)


def main() -> int:
    from tools.coop.smell_report import ROOTS

    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dirty", action="store_true", help="list accepted-but-dirty functions")
    args = ap.parse_args()
    results, s = compute(ROOTS)
    if args.dirty:
        for r in results:
            if r.accepted and r.located and not r.clean:
                sm = ",".join(f"{k}={v}" for k, v in sorted(r.smells.items()))
                print(f"{r.source}\t{r.function}\t{r.size}\t{sm}")
        return 0
    print(f"accepted {s.accepted} ({s.accepted_bytes} B): clean {s.clean} ({s.clean_bytes} B), "
          f"dirty {s.dirty} ({s.dirty_bytes} B), unlocated {s.unlocated} ({s.unlocated_bytes} B); "
          f"clean TUs {s.clean_tus}/{s.tus}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

"""Probe retail jump-table auto-context hydration for corpus exact-pattern hits.

Used by regression tests and manual coop diagnostics. Fail-closed: reports why
auto-context returned ``None`` without attempting heuristic promotion.
"""

from __future__ import annotations

import argparse
import json
import sys
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Any

from tools.ppc_equivalence.dol_symbols import DolSymbolError, extract_by_address
from tools.ppc_equivalence.jump_table import find_jump_table_candidates
from tools.ppc_equivalence.jump_table_auto import (
    _LINKED_TEXT_LOOKBACK_BYTES,
    _decode_linked_text_window,
    expand_jump_table_instructions,
    resolve_table_base_va,
    try_auto_jump_table_context,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_retail import (
    DEFAULT_RETAIL_DOL,
    find_retail_jump_table_candidates,
)

# Documented exact-pattern examples from docs/ppc_equiv_work/28-loop-jt-corpus-hits.md
DOCUMENTED_EXACT_BRANCH_PCS: tuple[int, ...] = (
    0x8022C8B8,
    0x80245444,
    0x80255EA4,
    0x803119E8,
    0x8031BD74,
    0x802C8D28,
    0x803B28B8,
)


@dataclass(frozen=True)
class JumpTableProbeResult:
    branch_pc: int
    dol_path: str
    confidence: str | None
    bound_imm: int | None
    table_base_va: int | None
    auto_context: bool
    detail: str

    def to_dict(self) -> dict[str, Any]:
        payload = asdict(self)
        payload["branch_pc"] = f"{self.branch_pc:#x}"
        if self.table_base_va is not None:
            payload["table_base_va"] = f"{self.table_base_va:#x}"
        return payload


def _decode_tail_window(
    dol_path: Path,
    branch_pc: int,
    *,
    lookback_bytes: int = _LINKED_TEXT_LOOKBACK_BYTES,
):
    start = max(0, branch_pc - lookback_bytes)
    size = branch_pc + 4 - start
    slice_ = extract_by_address(dol_path, start, size)
    return _decode_linked_text_window(slice_.code, slice_.base)


def probe_branch_pc(
    branch_pc: int,
    dol_path: Path | str,
    *,
    lookback_bytes: int = _LINKED_TEXT_LOOKBACK_BYTES,
) -> JumpTableProbeResult:
    """Report pattern recognition, base recovery, and auto-context for one tail."""
    path = Path(dol_path)
    if not path.is_file():
        return JumpTableProbeResult(
            branch_pc=branch_pc,
            dol_path=str(path),
            confidence=None,
            bound_imm=None,
            table_base_va=None,
            auto_context=False,
            detail=f"retail DOL missing: {path}",
        )
    try:
        insns = _decode_tail_window(path, branch_pc, lookback_bytes=lookback_bytes)
    except (DolSymbolError, ValueError) as exc:
        return JumpTableProbeResult(
            branch_pc=branch_pc,
            dol_path=str(path.resolve()),
            confidence=None,
            bound_imm=None,
            table_base_va=None,
            auto_context=False,
            detail=f"decode failed: {exc}",
        )

    expanded = expand_jump_table_instructions(insns, dol_path=path)
    candidates = [
        item for item in find_jump_table_candidates(expanded)
        if item.branch_pc == branch_pc
    ]
    if len(candidates) != 1:
        return JumpTableProbeResult(
            branch_pc=branch_pc,
            dol_path=str(path.resolve()),
            confidence=None,
            bound_imm=None,
            table_base_va=None,
            auto_context=False,
            detail=f"expected one candidate, found {len(candidates)}",
        )

    candidate = candidates[0]
    base_va = resolve_table_base_va(expanded, candidate)
    context = try_auto_jump_table_context(
        insns,
        insns,
        original_dol_path=path,
        dol_path=path,
    )
    if context is not None:
        detail = "auto-context built"
    elif base_va is None:
        detail = "table base VA unresolved"
    else:
        detail = "hydration or pairing rejected"
    return JumpTableProbeResult(
        branch_pc=branch_pc,
        dol_path=str(path.resolve()),
        confidence=candidate.confidence,
        bound_imm=candidate.bound_imm,
        table_base_va=base_va,
        auto_context=context is not None,
        detail=detail,
    )


def probe_retail_exact_patterns(
    dol_path: Path | str,
    *,
    lookback_bytes: int = _LINKED_TEXT_LOOKBACK_BYTES,
) -> list[JumpTableProbeResult]:
    """Scan linked DOL text for exact-pattern tails and probe auto-context."""
    path = Path(dol_path)
    if not path.is_file():
        return [
            JumpTableProbeResult(
                branch_pc=0,
                dol_path=str(path),
                confidence=None,
                bound_imm=None,
                table_base_va=None,
                auto_context=False,
                detail=f"retail DOL missing: {path}",
            )
        ]

    results: list[JumpTableProbeResult] = []
    seen: set[int] = set()
    for candidate in find_retail_jump_table_candidates(path, lookback_bytes=lookback_bytes):
        if candidate.confidence != "exact-pattern":
            continue
        if candidate.branch_pc is None or candidate.branch_pc in seen:
            continue
        seen.add(candidate.branch_pc)
        results.append(probe_branch_pc(candidate.branch_pc, path, lookback_bytes=lookback_bytes))
    results.sort(key=lambda item: item.branch_pc)
    return results


def summarize_probe_results(results: list[JumpTableProbeResult]) -> dict[str, Any]:
    exact = [item for item in results if item.confidence == "exact-pattern"]
    hydrated = [item for item in exact if item.auto_context]
    return {
        "scanned": len(results),
        "exact_pattern": len(exact),
        "auto_context_ok": len(hydrated),
        "missing_dol": any("missing" in item.detail for item in results[:1] if item.branch_pc == 0),
        "failures": [
            item.to_dict()
            for item in exact
            if not item.auto_context
        ],
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Probe jump-table auto-context for retail exact-pattern tails",
    )
    parser.add_argument(
        "--dol",
        type=Path,
        default=DEFAULT_RETAIL_DOL,
        help="Retail main.dol path (default: orig/us/sys/main.dol)",
    )
    parser.add_argument(
        "--branch-pc",
        type=lambda value: int(value, 0),
        action="append",
        dest="branch_pcs",
        help="Probe one branch PC (repeatable; default: full DOL exact-pattern scan)",
    )
    parser.add_argument(
        "--documented",
        action="store_true",
        help="Probe the seven documented examples from doc 28 instead of scanning",
    )
    parser.add_argument("--json", action="store_true", help="Emit JSON summary")
    args = parser.parse_args(argv)

    if args.branch_pcs:
        results = [probe_branch_pc(pc, args.dol) for pc in args.branch_pcs]
    elif args.documented:
        results = [probe_branch_pc(pc, args.dol) for pc in DOCUMENTED_EXACT_BRANCH_PCS]
    else:
        results = probe_retail_exact_patterns(args.dol)

    summary = summarize_probe_results(results)
    summary["results"] = [item.to_dict() for item in results]

    if args.json:
        json.dump(summary, sys.stdout, indent=2, sort_keys=True)
        sys.stdout.write("\n")
    else:
        print(
            f"DOL: {args.dol} ({'present' if args.dol.is_file() else 'missing'})",
        )
        print(
            f"exact-pattern: {summary['exact_pattern']}  "
            f"auto-context ok: {summary['auto_context_ok']}/{summary['exact_pattern']}",
        )
        for item in results:
            if item.branch_pc == 0 and "missing" in item.detail:
                print(item.detail)
                break
            status = "OK" if item.auto_context else "FAIL"
            base = f"{item.table_base_va:#x}" if item.table_base_va is not None else "-"
            print(
                f"  {item.branch_pc:#x} [{status}] conf={item.confidence} "
                f"base={base} — {item.detail}",
            )
    return 0 if summary["auto_context_ok"] == summary["exact_pattern"] else 1


if __name__ == "__main__":
    raise SystemExit(main())

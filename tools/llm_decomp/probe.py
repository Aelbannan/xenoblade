"""No-LLM equivalence probe (ported from the retired harness).

For compiling, non-accepted targets: build, compute fuzzy match vs retail,
and run the SMT equivalence probe when fuzzy is high enough. Discovers
EQUIVALENT_MATCH wins without spending model budget.

Rows land in build/llm-decomp/probe/<batch>/results.jsonl (+ summary.json).
``write=True`` persists accepted FULL_MATCH / EQUIVALENT_MATCH into
targets.json; dry runs never mutate the registry.
"""
from __future__ import annotations

import json
from dataclasses import asdict, dataclass
from datetime import datetime, timezone
from pathlib import Path

from . import verify


@dataclass
class ProbeRow:
    target_id: str
    unit: str
    symbol: str
    status: str  # FULL_MATCH | EQUIVALENT_MATCH | CODE_MATCH | COMPILE_ERROR | PROBE_ERROR
    match_percent: float
    equivalence: str  # EQUIVALENT | NOT_EQUIVALENT | INCONCLUSIVE | ''
    accepted: bool
    written: bool
    detail: str = ""


def eligible_targets(repo_root: Path, number: int | None = None,
                     tu: str | None = None) -> list:
    """Buildable, non-accepted targets with symbols (registry order)."""
    from tools.coop.lib.config import load_config as load_coop_config
    from tools.coop.lib.targets import load_targets
    coop_cfg = load_coop_config(None, repo_root)
    out = [
        t for t in load_targets(coop_cfg)
        if t.buildable and t.symbol
        and t.status not in ("FULL_MATCH", "EQUIVALENT_MATCH")
        and (tu is None or t.unit == tu)
    ]
    return out[:number] if number is not None else out


def _fuzzy_for_target(repo_root: Path, target) -> tuple[float, int]:
    """(fuzzy %, mismatch_count) of the target symbol vs retail."""
    from tools.ppc_equivalence.elf_symbols import list_text_functions
    decomp_obj = verify.current_object_path(repo_root, target.unit)
    retail_obj = verify._resolve_retail_path(repo_root, target.unit)
    decomp = {f.name: f for f in list_text_functions(decomp_obj)}
    retail = {f.name: f for f in list_text_functions(retail_obj)}
    if target.symbol not in decomp:
        raise KeyError(f"{target.symbol} missing from decomp object")
    if target.symbol not in retail:
        raise KeyError(f"{target.symbol} missing from retail object")
    r, d = retail[target.symbol], decomp[target.symbol]
    mismatches = verify._count_mismatches(r.code, d.code)
    return verify.estimate_fuzzy(mismatches, max(1, len(r.code) // 4)), \
        mismatches


def _persist(repo_root: Path, row: ProbeRow) -> None:
    from tools.coop.lib.config import load_config as load_coop_config
    from tools.coop.lib.targets import update_target_result
    coop_cfg = load_coop_config(None, repo_root)
    update_target_result(
        coop_cfg, row.target_id, status=row.status,
        instruction_match=row.match_percent,
        equivalence_status=("EQUIVALENT"
                            if row.status == "EQUIVALENT_MATCH" else None))


def probe_targets(repo_root: Path, targets: list, *,
                  min_fuzzy: float = 50.0, write: bool = False,
                  out_dir: Path | None = None) -> list[ProbeRow]:
    if out_dir is None:
        batch = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
        out_dir = (repo_root / "build" / "llm-decomp" / "probe" / batch)
    out_dir.mkdir(parents=True, exist_ok=True)

    rows: list[ProbeRow] = []
    with open(out_dir / "results.jsonl", "w", encoding="utf-8") as out:
        for target in targets:
            row = ProbeRow(target_id=target.id, unit=target.unit or "",
                           symbol=target.symbol or "", status="PROBE_ERROR",
                           match_percent=0.0, equivalence="",
                           accepted=False, written=False)
            try:
                ok, diagnostics = verify.build_unit(repo_root, target.unit)
                if not ok:
                    row.status = "COMPILE_ERROR"
                    row.detail = diagnostics[-500:]
                else:
                    fuzzy, _mism = _fuzzy_for_target(repo_root, target)
                    row.match_percent = round(fuzzy, 2)
                    if fuzzy >= 100.0:
                        row.status = "FULL_MATCH"
                        row.accepted = True
                    elif fuzzy >= min_fuzzy:
                        row.equivalence = verify.run_equivalence(
                            repo_root, target.unit, target.symbol)
                        if row.equivalence == "EQUIVALENT":
                            row.status = "EQUIVALENT_MATCH"
                            row.accepted = True
                        else:
                            row.status = "CODE_MATCH"
                    else:
                        row.status = "CODE_MATCH"
            except Exception as exc:
                row.status = "PROBE_ERROR"
                row.detail = f"{type(exc).__name__}: {exc}"
            if row.accepted and write:
                try:
                    _persist(repo_root, row)
                    row.written = True
                except Exception as exc:
                    row.detail = f"persist failed: {exc}"
            rows.append(row)
            out.write(json.dumps(asdict(row), separators=(",", ":")) + "\n")
            out.flush()

    by_status: dict[str, int] = {}
    for row in rows:
        by_status[row.status] = by_status.get(row.status, 0) + 1
    summary = {
        "batch": out_dir.name,
        "total": len(rows),
        "accepted": sum(1 for r in rows if r.accepted),
        "errors": sum(1 for r in rows if r.status == "PROBE_ERROR"),
        "by_status": by_status,
    }
    (out_dir / "summary.json").write_text(json.dumps(summary, indent=2),
                                          encoding="utf-8")
    return rows

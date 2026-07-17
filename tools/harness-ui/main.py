#!/usr/bin/env python3
from __future__ import annotations

import json
import os
import subprocess
import sys
import time
from collections import Counter, defaultdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Optional

from fastapi import FastAPI, HTTPException, Query
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.targets import load_targets

app = FastAPI(title="Xenoblade Harness UI")

STATIC_DIR = Path(__file__).parent / "static"

HARNESS_OUTPUT = ROOT / "build" / "llm-harness"
EXPERIMENTS_JSONL = HARNESS_OUTPUT / "experiments.jsonl"
IO_JSONL = HARNESS_OUTPUT / "io.jsonl"
HARNESS_CONFIG = ROOT / "llm-harness.json"
TARGETS_PATH = ROOT / "tools" / "coop" / "targets.json"

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _load_jsonl(path: Path, max_lines: int = 0) -> list[dict[str, Any]]:
    if not path.is_file():
        return []
    lines = []
    with path.open("r", encoding="utf-8") as f:
        for i, line in enumerate(f):
            line = line.strip()
            if not line:
                continue
            if max_lines and len(lines) >= max_lines:
                break
            try:
                lines.append(json.loads(line))
            except json.JSONDecodeError:
                continue
    return lines


def _run_harness_cli(args: list[str]) -> tuple[str, str, int]:
    cmd = [sys.executable, str(ROOT / "tools" / "llm_harness" / "run.py")]
    cmd.extend(args)
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT, timeout=600)
    return proc.stdout, proc.stderr, proc.returncode


def _run_coop_cli(args: list[str]) -> tuple[str, str, int]:
    cmd = [sys.executable, str(ROOT / "tools" / "coop" / "run.py")]
    cmd.extend(args)
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT, timeout=600)
    return proc.stdout, proc.stderr, proc.returncode


def _read_file(path: Path) -> Optional[str]:
    if not path.is_file():
        return None
    return path.read_text(encoding="utf-8")


def _get_experiment_dirs() -> list[Path]:
    if not HARNESS_OUTPUT.is_dir():
        return []
    dirs = []
    for entry in HARNESS_OUTPUT.iterdir():
        if entry.is_dir() and entry.name not in ("batches",):
            dirs.append(entry)
    return sorted(dirs, key=lambda p: p.stat().st_mtime, reverse=True)


def _read_experiment_state(exp_dir: Path) -> Optional[dict]:
    state = exp_dir / "state.json"
    if state.is_file():
        return json.loads(state.read_text(encoding="utf-8"))
    return None


def _read_best(exp_dir: Path) -> Optional[dict]:
    best = exp_dir / "best.json"
    if best.is_file():
        return json.loads(best.read_text(encoding="utf-8"))
    return None


def _collect_target_stats() -> dict:
    try:
        config = load_config(None, ROOT)
        targets = load_targets(config)
    except Exception as e:
        return {"error": str(e)}

    total = len(targets)
    by_status: Counter[str] = Counter()
    by_tier: Counter[str] = Counter()
    by_workflow: Counter[str] = Counter()
    started = 0
    fully_matched = 0
    equivalent_matched = 0

    for t in targets:
        s = t.status or "NOT_STARTED"
        by_status[s] += 1
        by_tier[t.tier or "P9"] += 1
        ws = t.workflow_status or "DISCOVERY"
        by_workflow[ws] += 1
        if s != "NOT_STARTED":
            started += 1
        if s == "FULL_MATCH":
            fully_matched += 1
        if s == "EQUIVALENT_MATCH":
            equivalent_matched += 1

    return {
        "total": total,
        "started": started,
        "fully_matched": fully_matched,
        "equivalent_matched": equivalent_matched,
        "by_status": dict(by_status),
        "by_tier": dict(by_tier),
        "by_workflow": dict(by_workflow),
    }


def _get_harness_config() -> dict:
    if not HARNESS_CONFIG.is_file():
        return {}
    return json.loads(HARNESS_CONFIG.read_text(encoding="utf-8"))


# ---------------------------------------------------------------------------
# API Routes
# ---------------------------------------------------------------------------

@app.get("/api/stats")
def get_stats():
    records = _load_jsonl(EXPERIMENTS_JSONL)
    if not records:
        return {"models": [], "overall": {}}

    groups: dict[str, list[dict]] = {}
    for r in records:
        groups.setdefault(r.get("model_id", "unknown"), []).append(r)

    overall = {
        "total_experiments": len(records),
        "total_targets": len(set(r.get("target_id") for r in records)),
        "winners": sum(1 for r in records if r.get("winner")),
        "accepted": sum(1 for r in records if r.get("evaluation", {}).get("accepted")),
        "errors": sum(1 for r in records if r.get("error")),
        "total_cost": sum(float(r.get("cost") or 0) for r in records),
        "total_input_tokens": sum(int(r.get("input_tokens") or 0) for r in records),
        "total_output_tokens": sum(int(r.get("output_tokens") or 0) for r in records),
    }

    models = []
    for model_id, rows in sorted(groups.items()):
        completed = [r for r in rows if not r.get("error")]
        measured = [r for r in completed if not r.get("timed_out")]
        matches = [
            float(r["evaluation"]["match_percent"])
            for r in measured
            if r.get("evaluation", {}).get("match_percent") is not None
        ]
        models.append({
            "model_id": model_id,
            "attempts": len(rows),
            "completed": len(completed),
            "wins": sum(bool(r.get("winner")) for r in rows),
            "accepted_wins": sum(
                bool(r.get("winner"))
                and bool(r.get("evaluation", {}).get("accepted"))
                for r in rows
            ),
            "accepted": sum(
                bool(r.get("evaluation", {}).get("accepted"))
                for r in completed
            ),
            "average_match_percent": (
                sum(matches) / len(matches) if matches else None
            ),
            "average_seconds": (
                sum(float(r.get("duration_seconds", 0)) for r in measured)
                / len(measured)
                if measured
                else None
            ),
            "input_tokens": sum(int(r.get("input_tokens") or 0) for r in rows),
            "output_tokens": sum(int(r.get("output_tokens") or 0) for r in rows),
            "non_cached_tokens": sum(
                max(0, int(r.get("input_tokens") or 0) - int(r.get("cache_read_tokens") or 0))
                for r in rows
            ),
            "cache_read_tokens": sum(int(r.get("cache_read_tokens") or 0) for r in rows),
            "cache_write_tokens": sum(int(r.get("cache_write_tokens") or 0) for r in rows),
            "total_cost": sum(float(r.get("cost") or 0) for r in rows),
            "average_cost": (
                sum(float(r.get("cost") or 0) for r in measured) / len(measured)
                if measured
                else None
            ),
            "errors": sum(bool(r.get("error")) for r in rows),
            "timeouts": sum(
                bool(r.get("timed_out"))
                or "TimeoutExpired" in str(r.get("error") or "")
                for r in rows
            ),
        })

    return {"models": models, "overall": overall}


@app.get("/api/stats/detail")
def get_stats_detail(
    limit: int = Query(500, description="Max records to analyze"),
    model: Optional[str] = Query(None),
):
    records = _load_jsonl(EXPERIMENTS_JSONL, max_lines=limit)
    if not records:
        return {"records": [], "by_status": {}, "by_date": {}, "by_workflow": {}}

    if model:
        records = [r for r in records if r.get("model_id") == model]

    by_status: Counter[str] = Counter()
    by_workflow: Counter[str] = Counter()
    by_date: Counter[str] = Counter()

    for r in records:
        ev = r.get("evaluation", {}) or {}
        by_status[ev.get("status", "UNKNOWN")] += 1
        by_workflow[r.get("workflow", "unknown")] += 1
        ts = r.get("timestamp", "")
        if ts:
            date_key = ts[:10]
            by_date[date_key] += 1

    return {
        "records": records,
        "by_status": dict(by_status),
        "by_workflow": dict(by_workflow),
        "by_date": dict(sorted(by_date.items())),
    }


@app.get("/api/targets")
def get_targets(
    tier: Optional[str] = Query(None),
    status: Optional[str] = Query(None),
    workflow: Optional[str] = Query(None),
    search: Optional[str] = Query(None),
    limit: int = Query(200),
    offset: int = Query(0),
):
    config = load_config(None, ROOT)
    targets = load_targets(config)

    filtered = []
    for t in targets:
        if tier and (t.tier or "") != tier:
            continue
        if status and (t.status or "") != status:
            continue
        if workflow and (t.workflow_status or "") != workflow:
            continue
        if search:
            q = search.lower()
            safe = lambda v: (v or "").lower()
            if (
                q not in safe(t.id)
                and q not in safe(t.function)
                and q not in safe(t.symbol)
                and q not in safe(t.source)
            ):
                continue
        filtered.append(t)

    total = len(filtered)
    page = filtered[offset : offset + limit]

    return {
        "total": total,
        "offset": offset,
        "limit": limit,
        "targets": [
            {
                "id": t.id,
                "tier": t.tier,
                "milestone": t.milestone,
                "function": t.function,
                "symbol": t.symbol,
                "source": str(t.source) if t.source else None,
                "unit": t.unit,
                "status": t.status,
                "instruction_match": t.extra.get("instruction_match"),
                "workflow_status": t.workflow_status,
                "required_level": t.required_level,
                "address": t.address,
                "kind": t.kind,
                "origin": t.extra.get("origin"),
            }
            for t in page
        ],
    }


@app.get("/api/targets/{target_id}")
def get_target(target_id: str):
    config = load_config(None, ROOT)
    targets = load_targets(config)

    for t in targets:
        if t.id == target_id:
            return {
                "id": t.id,
                "tier": t.tier,
                "milestone": t.milestone,
                "function": t.function,
                "symbol": t.symbol,
                "source": str(t.source) if t.source else None,
                "unit": t.unit,
                "status": t.status,
                "instruction_match": t.extra.get("instruction_match"),
                "workflow_status": t.workflow_status,
                "required_level": t.required_level,
                "address": t.address,
                "size": t.extra.get("size"),
                "kind": t.kind,
                "origin": t.extra.get("origin"),
                "called_functions": t.extra.get("called_functions", []),
                "unresolved_called_functions": t.extra.get("unresolved_called_functions", []),
                "has_indirect_calls": t.extra.get("has_indirect_calls", False),
                "callgraph_status": t.extra.get("callgraph_status"),
            }

    raise HTTPException(404, f"Target '{target_id}' not found")


@app.get("/api/runs")
def get_runs(
    limit: int = Query(50),
    target_id: Optional[str] = Query(None),
    model: Optional[str] = Query(None),
    winner: Optional[bool] = Query(None),
):
    records = _load_jsonl(EXPERIMENTS_JSONL)
    if not records:
        return {"runs": []}

    if target_id:
        records = [r for r in records if r.get("target_id") == target_id]
    if model:
        records = [r for r in records if r.get("model_id") == model]
    if winner is not None:
        records = [r for r in records if r.get("winner") == winner]

    records.sort(key=lambda r: r.get("timestamp", ""), reverse=True)

    return {
        "runs": [
            {
                "experiment_id": r.get("experiment_id"),
                "timestamp": r.get("timestamp"),
                "workflow": r.get("workflow"),
                "target_id": r.get("target_id"),
                "model_id": r.get("model_id"),
                "model": r.get("model"),
                "run_index": r.get("run_index"),
                "duration_seconds": r.get("duration_seconds"),
                "input_tokens": r.get("input_tokens"),
                "output_tokens": r.get("output_tokens"),
                "cost": r.get("cost"),
                "evaluation_status": (r.get("evaluation") or {}).get("status"),
                "match_percent": (r.get("evaluation") or {}).get("match_percent"),
                "accepted": (r.get("evaluation") or {}).get("accepted"),
                "winner": r.get("winner"),
                "error": r.get("error"),
                "timed_out": r.get("timed_out"),
            }
            for r in records[:limit]
        ],
    }


@app.get("/api/runs/{experiment_id}")
def get_run_detail(experiment_id: str):
    records = _load_jsonl(EXPERIMENTS_JSONL)
    matches = [r for r in records if r.get("experiment_id") == experiment_id]
    if not matches:
        raise HTTPException(404, f"Experiment '{experiment_id}' not found")

    return {"records": matches}


@app.get("/api/runs/{experiment_id}/io")
def get_run_io(experiment_id: str):
    io_records = _load_jsonl(IO_JSONL)
    matches = [r for r in io_records if r.get("experiment_id") == experiment_id]
    if not matches:
        # Try finding experiment dir
        for d in _get_experiment_dirs():
            if d.name == experiment_id:
                prompt = _read_file(d / "prompt.md")
                if prompt:
                    return {
                        "prompt": prompt,
                        "responses": [],
                        "experiment_id": experiment_id,
                    }
        raise HTTPException(404, f"No IO data for experiment '{experiment_id}'")
    return {
        "records": matches,
        "experiment_id": experiment_id,
    }


@app.get("/api/targets/{target_id}/io")
def get_target_io(target_id: str, limit: int = Query(20)):
    io_records = _load_jsonl(IO_JSONL)
    matches = [
        r for r in io_records if r.get("target_id") == target_id
    ]
    matches.sort(key=lambda r: r.get("timestamp", ""), reverse=True)
    return {"records": matches[:limit], "target_id": target_id, "total": len(matches)}


@app.get("/api/experiments")
def get_experiments(limit: int = Query(20)):
    dirs = _get_experiment_dirs()[:limit]
    result = []
    for d in dirs:
        state = _read_experiment_state(d)
        best = _read_best(d)
        result.append({
            "experiment_id": d.name,
            "target_id": state.get("target_id") if state else None,
            "workflow": state.get("workflow") if state else None,
            "status": state.get("status") if state else None,
            "records_count": len(state.get("records", [])) if state else 0,
            "best_status": (
                best.get("evaluation", {}).get("status") if best else None
            ),
            "best_match": (
                best.get("evaluation", {}).get("match_percent") if best else None
            ),
            "best_accepted": (
                best.get("evaluation", {}).get("accepted") if best else None
            ),
            "prompt_exists": (d / "prompt.md").is_file(),
        })
    return {"experiments": result}


@app.get("/api/experiments/{experiment_id}/prompt")
def get_experiment_prompt(experiment_id: str):
    for d in _get_experiment_dirs():
        if d.name == experiment_id:
            prompt = _read_file(d / "prompt.md")
            if prompt is None:
                raise HTTPException(404, "No prompt file found")
            return {"experiment_id": experiment_id, "prompt": prompt}
    raise HTTPException(404, f"Experiment '{experiment_id}' not found")


@app.get("/api/experiments/{experiment_id}/artifact")
def get_experiment_artifact(experiment_id: str, run_index: int = Query(1)):
    for d in _get_experiment_dirs():
        if d.name == experiment_id:
            best = _read_best(d)
            if best:
                return {"experiment_id": experiment_id, "artifact": best}
            raise HTTPException(404, "No best.json artifact found")
    raise HTTPException(404, f"Experiment '{experiment_id}' not found")


@app.get("/api/progress")
def get_progress():
    target_stats = _collect_target_stats()
    config = _get_harness_config()
    records = _load_jsonl(EXPERIMENTS_JSONL)

    pipeline_info = {}
    for workflow in ("new", "improve", "tu-complete"):
        models = config.get("models", {}).get(workflow, [])
        execution = config.get("execution", {}).get("pipelines", {}).get(workflow, {})
        pipeline_info[workflow] = {
            "models": [m.get("id") for m in models],
            "max_parallel": execution.get("max_parallel", config.get("execution", {}).get("max_parallel", 1)),
            "runs_per_model": [m.get("runs", 1) for m in models],
        }

    return {
        "targets": target_stats,
        "harness_config": {
            "providers": list(config.get("providers", {}).keys()),
            "pipeline_info": pipeline_info,
            "output_dir": config.get("output_dir", "build/llm-harness"),
        },
        "experiments_count": len(records),
    }


@app.get("/api/code-map")
def get_code_map(
    tier: Optional[str] = Query(None),
    milestone: Optional[str] = Query(None),
):
    config = load_config(None, ROOT)
    targets = load_targets(config)

    by_unit: dict[str, dict] = {}
    for t in targets:
        if tier and (t.tier or "") != tier:
            continue
        if milestone and (t.milestone or "") != milestone:
            continue

        unit = t.unit or "orphan"
        if unit not in by_unit:
            by_unit[unit] = {
                "unit": unit,
                "total": 0,
                "not_started": 0,
                "compiles": 0,
                "structural": 0,
                "high_match": 0,
                "code_match": 0,
                "equivalent_match": 0,
                "full_match": 0,
                "functions": [],
            }
        info = by_unit[unit]
        info["total"] += 1
        s = t.status or "NOT_STARTED"
        s_key = s.lower()
        if s_key in ("not_started",):
            info["not_started"] += 1
        elif s_key == "compiles":
            info["compiles"] += 1
        elif s_key == "structural":
            info["structural"] += 1
        elif s_key == "high_match":
            info["high_match"] += 1
        elif s_key == "code_match":
            info["code_match"] += 1
        elif s_key == "equivalent_match":
            info["equivalent_match"] += 1
        elif s_key == "full_match":
            info["full_match"] += 1
        else:
            info["not_started"] += 1

        info["functions"].append({
            "id": t.id,
            "function": t.function,
            "symbol": t.symbol,
            "status": t.status,
            "instruction_match": t.extra.get("instruction_match"),
            "workflow_status": t.workflow_status,
        })

    units = list(by_unit.values())
    units.sort(key=lambda u: u["full_match"] / max(u["total"], 1), reverse=True)

    # Overall totals
    totals = {
        "total": sum(u["total"] for u in units),
        "not_started": sum(u["not_started"] for u in units),
        "compiles": sum(u["compiles"] for u in units),
        "structural": sum(u["structural"] for u in units),
        "high_match": sum(u["high_match"] for u in units),
        "code_match": sum(u["code_match"] for u in units),
        "equivalent_match": sum(u["equivalent_match"] for u in units),
        "full_match": sum(u["full_match"] for u in units),
    }
    totals["started_pct"] = (
        (totals["total"] - totals["not_started"]) / totals["total"] * 100
        if totals["total"]
        else 0
    )

    return {"units": units, "totals": totals}


@app.get("/api/start-options")
def get_start_options():
    config = _get_harness_config()
    providers = list(config.get("providers", {}).keys())

    target_config = load_config(None, ROOT)
    targets = load_targets(target_config)

    tiers = sorted(set(t.tier for t in targets if t.tier))
    workflows = list(config.get("models", {}).keys()) or ["new", "improve", "tu-complete"]
    if "default" in workflows:
        workflows = ["new", "improve", "tu-complete"]

    return {
        "workflows": workflows,
        "providers": providers,
        "tiers": tiers,
        "models": {
            w: [
                {"id": m.get("id"), "model": m.get("model"), "provider": m.get("provider"), "runs": m.get("runs", 1)}
                for m in config.get("models", {}).get(w, [])
            ]
            for w in workflows
        },
    }


@app.post("/api/runs/start")
def start_run(body: dict):
    workflow = body.get("workflow", "new")
    target_id = body.get("target_id")
    runs = body.get("runs")
    dry_run = body.get("dry_run", False)

    if not target_id and not body.get("number"):
        raise HTTPException(400, "Either target_id or number is required")

    args = [workflow]
    if target_id:
        args.append(target_id)
    if body.get("number"):
        args.extend(["--number", str(body["number"])])
    if runs:
        args.extend(["--runs", str(runs)])
    if dry_run:
        args.append("--dry-run")
    if body.get("resume"):
        args.extend(["--resume", str(body["resume"])])
    if body.get("retry_errors"):
        args.append("--retry-errors")
    if body.get("tu"):
        args.extend(["--tu", str(body["tu"])])
    if body.get("random"):
        args.append("--random")
    if body.get("certified_funcs"):
        args.append("--certified-funcs")

    stdout, stderr, rc = _run_harness_cli(args)
    return {
        "exit_code": rc,
        "stdout": stdout,
        "stderr": stderr,
        "command": " ".join(args),
    }


@app.post("/api/runs/batch")
def start_batch(body: dict):
    workflow = body.get("workflow", "new")
    target_ids = body.get("target_ids", [])
    tu = body.get("tu")
    dry_run = body.get("dry_run", False)
    max_parallel = body.get("max_target_parallel")

    if not target_ids and not tu:
        raise HTTPException(400, "Either target_ids or tu is required")

    args = ["batch", workflow]
    args.extend(target_ids)
    if tu:
        args.extend(["--tu", tu])
    if dry_run:
        args.append("--dry-run")
    if max_parallel:
        args.extend(["--max-target-parallel", str(max_parallel)])

    stdout, stderr, rc = _run_harness_cli(args)
    return {
        "exit_code": rc,
        "stdout": stdout,
        "stderr": stderr,
        "command": " ".join(args),
    }


@app.get("/api/batches")
def get_batches():
    batches_dir = HARNESS_OUTPUT / "batches"
    if not batches_dir.is_dir():
        return {"batches": []}
    result = []
    for d in sorted(batches_dir.iterdir(), key=lambda p: p.stat().st_mtime, reverse=True):
        manifest = d / "batch.json"
        if manifest.is_file():
            data = json.loads(manifest.read_text(encoding="utf-8"))
            result.append({
                "batch_id": d.name,
                "workflow": data.get("workflow"),
                "status": data.get("status"),
                "target_count": len(data.get("targets", {})),
                "success_count": sum(
                    1 for v in data.get("targets", {}).values()
                    if v.get("error") is None and v.get("experiment")
                ),
                "error_count": sum(
                    1 for v in data.get("targets", {}).values()
                    if v.get("error")
                ),
            })
    return {"batches": result}


@app.get("/api/source/{target_id}")
def get_source(target_id: str):
    config_ = load_config(None, ROOT)
    targets = load_targets(config_)
    for t in targets:
        if t.id == target_id and t.source:
            path = ROOT / t.source
            if path.is_file():
                content = path.read_text(encoding="utf-8")
                return {
                    "target_id": target_id,
                    "source_path": str(t.source),
                    "content": content,
                }
    raise HTTPException(404, f"Source not found for target '{target_id}'")


@app.get("/api/asm/{target_id}")
def get_asm(target_id: str):
    config_ = load_config(None, ROOT)
    targets = load_targets(config_)
    for t in targets:
        if t.id == target_id:
            asm_path = t.extra.get("callgraph_source")
            if asm_path:
                path = Path(asm_path)
                if not path.is_absolute():
                    path = ROOT / path
                if path.is_file():
                    content = path.read_text(encoding="utf-8")
                    return {
                        "target_id": target_id,
                        "asm_path": str(asm_path),
                        "content": content,
                    }
    raise HTTPException(404, f"Assembly not found for target '{target_id}'")


@app.get("/api/targets/{target_id}/diff")
def get_target_diff(target_id: str):
    config_ = load_config(None, ROOT)
    targets = load_targets(config_)

    target = None
    for t in targets:
        if t.id == target_id:
            target = t
            break
    if not target:
        raise HTTPException(404, f"Target '{target_id}' not found")

    stdout, stderr, rc = _run_coop_cli(
        ["diff", target.unit or target_id, "--symbol", target.symbol or ""]
    )
    return {
        "exit_code": rc,
        "stdout": stdout,
        "stderr": stderr,
        "target_id": target_id,
    }


# ---------------------------------------------------------------------------
# Static files & catch-all
# ---------------------------------------------------------------------------

app.mount("/static", StaticFiles(directory=str(STATIC_DIR)), name="static")


@app.get("/")
def index():
    return FileResponse(str(STATIC_DIR / "index.html"))


@app.get("/{full_path:path}")
def fallback(full_path: str):
    file_path = STATIC_DIR / full_path
    if file_path.is_file():
        return FileResponse(str(file_path))
    return FileResponse(str(STATIC_DIR / "index.html"))


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main():
    import uvicorn
    port = int(os.environ.get("HARNESS_UI_PORT", "3001"))
    host = os.environ.get("HARNESS_UI_HOST", "127.0.0.1")
    print(f"Starting Xenoblade Harness UI at http://{host}:{port}/")
    uvicorn.run(app, host=host, port=port, log_level="info")


if __name__ == "__main__":
    main()

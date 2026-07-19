"""Decomp Atlas local HTTP API (FastAPI preferred, stdlib fallback)."""

from __future__ import annotations

import json
import mimetypes
import re
import sys
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from typing import Any, Dict, List, Optional
from urllib.parse import parse_qs, unquote, urlparse

from tools.decomp_atlas.lib.config import atlas_root, db_path, index_state_path, ui_dir
from tools.decomp_atlas.lib.database import get_all_meta, open_db, row_to_dict, rows_to_dicts
from tools.decomp_atlas.lib.jobs import JobController
from tools.decomp_atlas.lib.project_adapter import AtlasProjectAdapter
from tools.decomp_atlas.lib.prompt_bridge import preview_prompt

try:
    from fastapi import FastAPI, HTTPException, Query, Request
    from fastapi.responses import FileResponse, JSONResponse, Response
    from fastapi.staticfiles import StaticFiles

    HAS_FASTAPI = True
except ImportError:  # pragma: no cover
    HAS_FASTAPI = False
    FastAPI = None  # type: ignore


FUNCTION_SUMMARY_COLS = (
    "target_id", "region", "kind", "display_name", "symbol", "address", "size",
    "source", "unit", "tier", "milestone", "status", "workflow_status",
    "instruction_percent", "readiness", "difficulty", "buildable", "owner",
    "required_level", "origin", "direct_call_count", "unresolved_call_count",
    "has_indirect_calls", "callgraph_status",
)


def _build_unit_tree(flat: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    """Build a nested source/unit tree for the sidebar."""
    root_children: Dict[str, Dict[str, Any]] = {}

    def ensure(parts: List[str], count: int) -> Dict[str, Any]:
        children = root_children
        node: Dict[str, Any] = {}
        path = ""
        for index, part in enumerate(parts):
            path = f"{path}/{part}" if path else part
            if part not in children:
                children[part] = {
                    "path": path,
                    "name": part,
                    "kind": "folder",
                    "function_count": 0,
                    "children": {},
                }
            node = children[part]
            node["function_count"] += count
            if index == len(parts) - 1:
                node["kind"] = "unit"
            children = node["children"]
        return node

    for row in flat:
        unit = str(row.get("unit") or "")
        source = str(row.get("source") or "")
        count = int(row.get("function_count") or 0)
        path = source or unit
        if not path:
            continue
        parts = [p for p in path.replace("\\", "/").split("/") if p]
        if not parts:
            continue
        node = ensure(parts, count)
        if source:
            node["kind"] = "source"
        elif unit:
            node["kind"] = "unit"

    def freeze(mapping: Dict[str, Dict[str, Any]]) -> List[Dict[str, Any]]:
        nodes = []
        for key in sorted(mapping.keys()):
            node = mapping[key]
            child_map = node.pop("children")
            node["children"] = freeze(child_map)
            nodes.append(node)
        return nodes

    return freeze(root_children)


class AtlasAPI:
    """Shared query helpers used by FastAPI and stdlib handlers."""

    def __init__(
        self,
        root: Path,
        *,
        region: Optional[str] = None,
        enable_jobs: bool = False,
        config_path: Optional[Path] = None,
    ) -> None:
        self.root = Path(root).resolve()
        self.config_path = config_path
        self.adapter = AtlasProjectAdapter(self.root, config_path=config_path)
        self.region = region or self.adapter.region
        self.enable_jobs = enable_jobs
        self.jobs = JobController(self.root, self.region) if enable_jobs else None
        self._lock = threading.Lock()

    def db(self, *, readonly: bool = True):
        path = db_path(self.root, self.region)
        return open_db(path, readonly=readonly)

    def project_info(self) -> Dict[str, Any]:
        state_path = index_state_path(self.root, self.region)
        state = {}
        if state_path.is_file():
            try:
                state = json.loads(state_path.read_text(encoding="utf-8"))
            except json.JSONDecodeError:
                state = {}
        meta: Dict[str, str] = {}
        try:
            with self.db() as conn:
                meta = get_all_meta(conn)
        except FileNotFoundError:
            meta = {}
        targets_hash = meta.get("targets_hash") or ""
        current = None
        try:
            from tools.decomp_atlas.lib.database import file_sha256

            current = file_sha256(self.adapter.targets_path())
        except Exception:
            current = None
        stale = bool(targets_hash and current and targets_hash != current)
        function_count = int(
            meta.get("function_count") or state.get("counts", {}).get("functions") or 0
        )
        return {
            "name": self.root.name,
            "project_root": str(self.root),
            "region": self.region,
            "atlas_root": str(atlas_root(self.root, self.region).relative_to(self.root)),
            "meta": meta,
            "index_state": state,
            "stale": stale,
            "jobs_enabled": self.enable_jobs,
            "function_count": function_count,
            "generated_at": meta.get("generated_at") or state.get("generated_at"),
            "schema_version": int(meta.get("schema_version") or 0) or None,
            "indexer_version": meta.get("indexer_version"),
        }

    def list_functions(self, params: Dict[str, Any]) -> Dict[str, Any]:
        page = max(1, int(params.get("page") or 1))
        page_size = min(500, max(1, int(params.get("page_size") or params.get("pageSize") or 100)))
        clauses: List[str] = []
        args: List[Any] = []

        def add(clause: str, *values: Any) -> None:
            clauses.append(clause)
            args.extend(values)

        search = params.get("search") or params.get("q")
        if search:
            like = f"%{search}%"
            add(
                "(target_id LIKE ? OR display_name LIKE ? OR symbol LIKE ? OR address LIKE ?)",
                like, like, like, like,
            )
        for key, column in (
            ("region", "region"),
            ("kind", "kind"),
            ("status", "status"),
            ("workflow", "workflow_status"),
            ("workflow_status", "workflow_status"),
            ("milestone", "milestone"),
            ("tier", "tier"),
            ("owner", "owner"),
            ("readiness", "readiness"),
        ):
            value = params.get(key)
            if value not in (None, ""):
                add(f"{column} = ?", value)
        path_filter = params.get("path") or params.get("unit")
        if path_filter:
            add(
                "(unit = ? OR unit LIKE ? OR source = ? OR source LIKE ?)",
                path_filter,
                f"{path_filter}%",
                path_filter,
                f"{path_filter}%",
            )
        source_prefix = params.get("source_prefix") or params.get("source")
        if source_prefix and source_prefix != path_filter:
            add("source LIKE ?", f"{source_prefix}%")
        if params.get("buildable") not in (None, ""):
            add("buildable = ?", 1 if str(params["buildable"]).lower() in {"1", "true", "yes"} else 0)
        indirect = params.get("has_indirect")
        if indirect in (None, ""):
            indirect = params.get("indirect")
        if indirect not in (None, ""):
            add(
                "has_indirect_calls = ?",
                1 if str(indirect).lower() in {"1", "true", "yes"} else 0,
            )
        unresolved = params.get("has_unresolved")
        if unresolved in (None, ""):
            unresolved = params.get("unresolved")
        if unresolved not in (None, ""):
            flag = str(unresolved).lower() in {"1", "true", "yes"}
            add("unresolved_call_count > 0" if flag else "unresolved_call_count = 0")
        if params.get("min_match") not in (None, ""):
            add("instruction_percent >= ?", float(params["min_match"]))
        if params.get("max_match") not in (None, ""):
            add("instruction_percent <= ?", float(params["max_match"]))
        if params.get("min_size") not in (None, ""):
            add("size >= ?", int(params["min_size"]))
        if params.get("max_size") not in (None, ""):
            add("size <= ?", int(params["max_size"]))
        capability = params.get("capability")
        if capability:
            add("capabilities LIKE ?", f"%{capability}%")

        where = (" WHERE " + " AND ".join(clauses)) if clauses else ""
        cols = ", ".join(FUNCTION_SUMMARY_COLS)
        with self.db() as conn:
            total = conn.execute(f"SELECT COUNT(*) AS n FROM functions{where}", args).fetchone()["n"]
            rows = conn.execute(
                f"SELECT {cols} FROM functions{where} "
                f"ORDER BY tier ASC, display_name ASC LIMIT ? OFFSET ?",
                [*args, page_size, (page - 1) * page_size],
            ).fetchall()
        items = rows_to_dicts(rows)
        for item in items:
            item["id"] = item["target_id"]
            item["buildable"] = bool(item.get("buildable"))
            item["has_indirect_calls"] = bool(item.get("has_indirect_calls"))
        return {
            "items": items,
            "page": page,
            "page_size": page_size,
            "total": int(total),
            "pages": max(1, (int(total) + page_size - 1) // page_size),
        }

    def get_function(self, target_id: str) -> Dict[str, Any]:
        with self.db() as conn:
            row = conn.execute("SELECT * FROM functions WHERE target_id = ?", (target_id,)).fetchone()
        if row is None:
            raise KeyError(target_id)
        data = row_to_dict(row) or {}
        data["id"] = data["target_id"]
        data["buildable"] = bool(data.get("buildable"))
        data["has_indirect_calls"] = bool(data.get("has_indirect_calls"))
        data["artifact_fresh"] = bool(data.get("artifact_fresh"))
        if data.get("vector_json"):
            try:
                data["vector"] = json.loads(data["vector_json"])
            except json.JSONDecodeError:
                data["vector"] = None
        return data

    def get_artifacts(self, target_id: str) -> Dict[str, Any]:
        with self.db() as conn:
            row = conn.execute("SELECT * FROM artifacts WHERE target_id = ?", (target_id,)).fetchone()
            fn = conn.execute(
                "SELECT target_id, region, kind, display_name, symbol, address, size, source, unit, "
                "target_object, base_object, status, workflow_status "
                "FROM functions WHERE target_id = ?",
                (target_id,),
            ).fetchone()

        data = row_to_dict(row) if row is not None else None
        needs_extract = data is None or (
            not (data.get("retail_asm") or "").strip()
            and not (data.get("candidate_asm") or "").strip()
            and not (data.get("cpp_source") or "").strip()
        )
        # Also refresh when retail is empty but we have a function record (e.g. .s fallback).
        if data is not None and not (data.get("retail_asm") or "").strip() and fn is not None:
            needs_extract = True

        if needs_extract and fn is not None:
            from tools.decomp_atlas.lib import artifacts as artifacts_mod

            record = row_to_dict(fn) or {}
            artifact = artifacts_mod.extract_artifacts(root=self.root, record=record)
            try:
                with self.db(readonly=False) as conn:
                    artifacts_mod.upsert_artifact(conn, artifact)
                    conn.commit()
            except Exception:
                # Still return the live extraction even if cache write fails.
                pass
            data = {
                "target_id": target_id,
                "cpp_source": artifact.get("cpp_source") or "",
                "retail_asm": artifact.get("retail_asm") or "",
                "candidate_asm": artifact.get("candidate_asm") or "",
                "retail_object_hash": artifact.get("retail_object_hash"),
                "candidate_object_hash": artifact.get("candidate_object_hash"),
                "source_hash": artifact.get("source_hash"),
                "updated_at": artifact.get("updated_at"),
                "relocations": artifact.get("relocations") or [],
                "decoded": artifact.get("decoded") or [],
                "warnings": artifact.get("warnings") or [],
            }
        elif data is None:
            data = {
                "target_id": target_id,
                "cpp_source": "",
                "retail_asm": "",
                "candidate_asm": "",
                "relocations": [],
                "decoded": [],
                "warnings": [
                    "function not in Atlas index; run: python3 tools/decomp_atlas/run.py index"
                ],
            }
        else:
            for key, field in (
                ("relocations", "relocations_json"),
                ("decoded", "decoded_json"),
                ("warnings", "warnings_json"),
            ):
                raw = data.pop(field, "[]")
                try:
                    data[key] = json.loads(raw or "[]")
                except json.JSONDecodeError:
                    data[key] = []

        # Frontend-friendly aliases
        data["source_cpp"] = data.get("cpp_source") or ""
        data["retail_ppc"] = data.get("retail_asm") or ""
        data["candidate_ppc"] = data.get("candidate_asm") or ""
        return data

    def get_neighbors(self, target_id: str) -> Dict[str, Any]:
        with self.db() as conn:
            rows = conn.execute(
                "SELECT n.neighbor_id AS id, n.neighbor_id, n.score, n.method, n.rank, "
                "f.display_name, f.status "
                "FROM neighbors n LEFT JOIN functions f ON f.target_id = n.neighbor_id "
                "WHERE n.target_id = ? ORDER BY n.rank ASC",
                (target_id,),
            ).fetchall()
            items = rows_to_dicts(rows)
            if items:
                return {"target_id": target_id, "items": items}

            # On-demand for large catalogs where pairwise neighbors were skipped.
            from tools.decomp_atlas.lib.similarity import neighbors_for_target

            target_row = conn.execute(
                "SELECT vector_json FROM functions WHERE target_id = ?",
                (target_id,),
            ).fetchone()
            if target_row is None or not target_row["vector_json"]:
                return {"target_id": target_id, "items": []}
            vector_rows = conn.execute(
                "SELECT target_id, vector_json, display_name, status FROM functions "
                "WHERE vector_json IS NOT NULL AND vector_json != ''"
            ).fetchall()
        vectors: Dict[str, Any] = {}
        meta: Dict[str, Dict[str, Any]] = {}
        for row in vector_rows:
            try:
                vectors[row["target_id"]] = json.loads(row["vector_json"])
            except json.JSONDecodeError:
                continue
            meta[row["target_id"]] = {
                "display_name": row["display_name"],
                "status": row["status"],
            }
        computed = neighbors_for_target(target_id, vectors, top_k=10)
        for item in computed:
            info = meta.get(item["neighbor_id"], {})
            item["display_name"] = info.get("display_name")
            item["status"] = info.get("status")
        return {"target_id": target_id, "items": computed}
    def get_attempts(self, target_id: str) -> Dict[str, Any]:
        with self.db() as conn:
            rows = conn.execute(
                "SELECT target_id, timestamp, status, instruction_match, hypothesis, "
                "next_change, runtime_test, policy_exception, equivalence_status "
                "FROM attempts WHERE target_id = ? ORDER BY id ASC",
                (target_id,),
            ).fetchall()
        items = rows_to_dicts(rows)
        for item in items:
            item["policy_exception"] = bool(item.get("policy_exception"))
            item["match_percent"] = item.get("instruction_match")
        return {"target_id": target_id, "items": items}

    def get_experiments(self, target_id: str) -> Dict[str, Any]:
        with self.db() as conn:
            rows = conn.execute(
                "SELECT experiment_id, target_id, workflow, model, model_id, run_index, "
                "timestamp, duration_seconds, input_tokens, output_tokens, cost, "
                "result_status, match_percent, equivalence_result, size_ok, accepted, "
                "winner, error, artifact FROM experiments WHERE target_id = ? "
                "ORDER BY id ASC",
                (target_id,),
            ).fetchall()
        items = rows_to_dicts(rows)
        for item in items:
            item["winner"] = bool(item.get("winner"))
            item["status"] = item.get("result_status")
            item["equivalence"] = item.get("equivalence_result")
            if item.get("accepted") is not None:
                item["accepted"] = bool(item["accepted"])
            if item.get("size_ok") is not None:
                item["size_ok"] = bool(item["size_ok"])
        return {"target_id": target_id, "items": items}

    def get_edges(self, target_id: str, *, direction: str) -> Dict[str, Any]:
        with self.db() as conn:
            if direction == "callers":
                rows = conn.execute(
                    "SELECT e.caller_id AS id, e.kind, f.display_name, f.symbol, "
                    "f.status, f.readiness "
                    "FROM edges e LEFT JOIN functions f ON f.target_id = e.caller_id "
                    "WHERE e.callee_id = ?",
                    (target_id,),
                ).fetchall()
            else:
                rows = conn.execute(
                    "SELECT e.callee_id AS id, e.kind, f.display_name, f.symbol, "
                    "f.status, f.readiness "
                    "FROM edges e LEFT JOIN functions f ON f.target_id = e.callee_id "
                    "WHERE e.caller_id = ?",
                    (target_id,),
                ).fetchall()
        return {"target_id": target_id, "items": rows_to_dicts(rows)}

    def get_projection(self) -> Dict[str, Any]:
        with self.db() as conn:
            rows = conn.execute(
                "SELECT p.target_id, p.x, p.y, p.version, f.status, f.tier, f.readiness, "
                "f.milestone, f.buildable, f.owner, f.unit, f.display_name, f.workflow_status "
                "FROM projection p LEFT JOIN functions f ON f.target_id = p.target_id"
            ).fetchall()
        items = rows_to_dicts(rows)
        for item in items:
            item["id"] = item["target_id"]
            item["buildable"] = bool(item.get("buildable"))
        version = items[0]["version"] if items else None
        return {"items": items, "points": items, "version": version}

    def get_units(self) -> Dict[str, Any]:
        with self.db() as conn:
            rows = conn.execute(
                "SELECT unit, source, COUNT(*) AS function_count "
                "FROM functions WHERE unit IS NOT NULL AND unit != '' "
                "GROUP BY unit, source ORDER BY unit ASC"
            ).fetchall()
        flat = rows_to_dicts(rows)
        tree = _build_unit_tree(flat)
        return {"items": flat, "tree": tree}

    def preview(self, target_id: str, body: Dict[str, Any]) -> Dict[str, Any]:
        workflow = body.get("workflow") or "new"
        full_context = bool(body.get("full_context"))
        return preview_prompt(
            self.root,
            target_id,
            workflow=workflow,  # type: ignore[arg-type]
            full_context=full_context,
        )


def create_fastapi_app(api: AtlasAPI) -> Any:
    app = FastAPI(title="Xenoblade Decomp Atlas", version="1.0.0")

    @app.get("/api/project")
    def api_project() -> Dict[str, Any]:
        return api.project_info()

    @app.get("/api/functions")
    def api_functions(request: Request) -> Dict[str, Any]:
        params = {k: v for k, v in request.query_params.items()}
        try:
            return api.list_functions(params)
        except FileNotFoundError as exc:
            raise HTTPException(status_code=503, detail=str(exc)) from exc

    @app.get("/api/functions/{target_id}")
    def api_function(target_id: str) -> Dict[str, Any]:
        try:
            return api.get_function(target_id)
        except KeyError as exc:
            raise HTTPException(status_code=404, detail="function not found") from exc
        except FileNotFoundError as exc:
            raise HTTPException(status_code=503, detail=str(exc)) from exc

    @app.get("/api/functions/{target_id}/artifacts")
    def api_artifacts(target_id: str) -> Dict[str, Any]:
        try:
            return api.get_artifacts(target_id)
        except FileNotFoundError as exc:
            raise HTTPException(status_code=503, detail=str(exc)) from exc

    @app.get("/api/functions/{target_id}/neighbors")
    def api_neighbors(target_id: str) -> Dict[str, Any]:
        return api.get_neighbors(target_id)

    @app.get("/api/functions/{target_id}/attempts")
    def api_attempts(target_id: str) -> Dict[str, Any]:
        return api.get_attempts(target_id)

    @app.get("/api/functions/{target_id}/experiments")
    def api_experiments(target_id: str) -> Dict[str, Any]:
        return api.get_experiments(target_id)

    @app.get("/api/functions/{target_id}/callers")
    def api_callers(target_id: str) -> Dict[str, Any]:
        return api.get_edges(target_id, direction="callers")

    @app.get("/api/functions/{target_id}/callees")
    def api_callees(target_id: str) -> Dict[str, Any]:
        return api.get_edges(target_id, direction="callees")

    @app.get("/api/projection")
    def api_projection() -> Dict[str, Any]:
        try:
            return api.get_projection()
        except FileNotFoundError as exc:
            raise HTTPException(status_code=503, detail=str(exc)) from exc

    @app.get("/api/units")
    def api_units() -> Dict[str, Any]:
        try:
            return api.get_units()
        except FileNotFoundError as exc:
            raise HTTPException(status_code=503, detail=str(exc)) from exc

    @app.post("/api/functions/{target_id}/prompt-preview")
    async def api_prompt_preview(target_id: str, request: Request) -> Dict[str, Any]:
        try:
            body = await request.json()
        except Exception:
            body = {}
        if not isinstance(body, dict):
            body = {}
        return api.preview(target_id, body)

    @app.get("/api/jobs")
    def api_list_jobs() -> Dict[str, Any]:
        if not api.enable_jobs or api.jobs is None:
            raise HTTPException(status_code=403, detail="jobs disabled; pass --enable-jobs")
        return {"items": api.jobs.list_jobs()}

    @app.get("/api/jobs/{job_id}")
    def api_get_job(job_id: str) -> Dict[str, Any]:
        if not api.enable_jobs or api.jobs is None:
            raise HTTPException(status_code=403, detail="jobs disabled")
        meta = api.jobs.get_job(job_id)
        if meta is None:
            raise HTTPException(status_code=404, detail="job not found")
        return meta

    @app.get("/api/jobs/{job_id}/events")
    def api_job_events(job_id: str, offset: int = Query(0), limit: int = Query(65536)) -> Dict[str, Any]:
        if not api.enable_jobs or api.jobs is None:
            raise HTTPException(status_code=403, detail="jobs disabled")
        try:
            return api.jobs.job_events(job_id, offset=offset, limit=limit)
        except KeyError as exc:
            raise HTTPException(status_code=404, detail="job not found") from exc

    @app.post("/api/functions/{target_id}/jobs")
    async def api_start_job(target_id: str, request: Request) -> Dict[str, Any]:
        if not api.enable_jobs or api.jobs is None:
            raise HTTPException(status_code=403, detail="jobs disabled; pass --enable-jobs")
        try:
            body = await request.json()
        except Exception:
            body = {}
        workflow = (body or {}).get("workflow") or "new"
        try:
            return api.jobs.start_job(target_id, workflow)
        except ValueError as exc:
            raise HTTPException(status_code=400, detail=str(exc)) from exc
        except RuntimeError as exc:
            raise HTTPException(status_code=429, detail=str(exc)) from exc

    @app.post("/api/jobs/{job_id}/cancel")
    def api_cancel_job(job_id: str) -> Dict[str, Any]:
        if not api.enable_jobs or api.jobs is None:
            raise HTTPException(status_code=403, detail="jobs disabled")
        try:
            return api.jobs.cancel_job(job_id)
        except KeyError as exc:
            raise HTTPException(status_code=404, detail="job not found") from exc

    static_root = ui_dir(api.root, api.region)
    if static_root.is_dir():
        index_html = static_root / "index.html"
        assets = static_root / "assets"
        if assets.is_dir():
            app.mount("/assets", StaticFiles(directory=str(assets)), name="assets")

        @app.get("/")
        def spa_index() -> FileResponse:
            if index_html.is_file():
                return FileResponse(index_html)
            raise HTTPException(status_code=404, detail="UI not built")

        @app.get("/{path:path}")
        def spa_fallback(path: str) -> Response:
            if path.startswith("api/"):
                raise HTTPException(status_code=404, detail="not found")
            candidate = static_root / path
            if candidate.is_file():
                return FileResponse(candidate)
            if index_html.is_file():
                return FileResponse(index_html)
            raise HTTPException(status_code=404, detail="not found")

    return app


def _json_bytes(payload: Any, status: int = 200) -> tuple[int, bytes, str]:
    return status, json.dumps(payload).encode("utf-8"), "application/json; charset=utf-8"


def make_stdlib_handler(api: AtlasAPI) -> type:
    static_root = ui_dir(api.root, api.region)

    class Handler(BaseHTTPRequestHandler):
        def log_message(self, fmt: str, *args: Any) -> None:  # noqa: A003
            sys.stderr.write("%s - %s\n" % (self.address_string(), fmt % args))

        def _send(self, status: int, body: bytes, content_type: str) -> None:
            self.send_response(status)
            self.send_header("Content-Type", content_type)
            self.send_header("Content-Length", str(len(body)))
            self.send_header("Access-Control-Allow-Origin", "*")
            self.end_headers()
            self.wfile.write(body)

        def _read_json(self) -> Dict[str, Any]:
            length = int(self.headers.get("Content-Length") or 0)
            if length <= 0:
                return {}
            raw = self.rfile.read(length)
            try:
                data = json.loads(raw.decode("utf-8"))
            except json.JSONDecodeError:
                return {}
            return data if isinstance(data, dict) else {}

        def do_OPTIONS(self) -> None:  # noqa: N802
            self.send_response(204)
            self.send_header("Access-Control-Allow-Origin", "*")
            self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
            self.send_header("Access-Control-Allow-Headers", "Content-Type")
            self.end_headers()

        def do_GET(self) -> None:  # noqa: N802
            parsed = urlparse(self.path)
            path = unquote(parsed.path)
            qs = {k: v[0] for k, v in parse_qs(parsed.query).items() if v}
            try:
                status, body, ctype = self._route_get(path, qs)
            except FileNotFoundError as exc:
                status, body, ctype = _json_bytes({"detail": str(exc)}, 503)
            except KeyError:
                status, body, ctype = _json_bytes({"detail": "not found"}, 404)
            except Exception as exc:  # noqa: BLE001
                status, body, ctype = _json_bytes({"detail": str(exc)}, 500)
            self._send(status, body, ctype)

        def do_POST(self) -> None:  # noqa: N802
            parsed = urlparse(self.path)
            path = unquote(parsed.path)
            body_json = self._read_json()
            try:
                status, body, ctype = self._route_post(path, body_json)
            except KeyError:
                status, body, ctype = _json_bytes({"detail": "not found"}, 404)
            except ValueError as exc:
                status, body, ctype = _json_bytes({"detail": str(exc)}, 400)
            except RuntimeError as exc:
                status, body, ctype = _json_bytes({"detail": str(exc)}, 429)
            except PermissionError as exc:
                status, body, ctype = _json_bytes({"detail": str(exc)}, 403)
            except Exception as exc:  # noqa: BLE001
                status, body, ctype = _json_bytes({"detail": str(exc)}, 500)
            self._send(status, body, ctype)

        def _route_get(self, path: str, qs: Dict[str, str]) -> tuple[int, bytes, str]:
            if path == "/api/project":
                return _json_bytes(api.project_info())
            if path == "/api/functions":
                return _json_bytes(api.list_functions(qs))
            m = re.fullmatch(r"/api/functions/([^/]+)", path)
            if m:
                return _json_bytes(api.get_function(m.group(1)))
            m = re.fullmatch(r"/api/functions/([^/]+)/artifacts", path)
            if m:
                return _json_bytes(api.get_artifacts(m.group(1)))
            m = re.fullmatch(r"/api/functions/([^/]+)/neighbors", path)
            if m:
                return _json_bytes(api.get_neighbors(m.group(1)))
            m = re.fullmatch(r"/api/functions/([^/]+)/attempts", path)
            if m:
                return _json_bytes(api.get_attempts(m.group(1)))
            m = re.fullmatch(r"/api/functions/([^/]+)/experiments", path)
            if m:
                return _json_bytes(api.get_experiments(m.group(1)))
            m = re.fullmatch(r"/api/functions/([^/]+)/callers", path)
            if m:
                return _json_bytes(api.get_edges(m.group(1), direction="callers"))
            m = re.fullmatch(r"/api/functions/([^/]+)/callees", path)
            if m:
                return _json_bytes(api.get_edges(m.group(1), direction="callees"))
            if path == "/api/projection":
                return _json_bytes(api.get_projection())
            if path == "/api/units":
                return _json_bytes(api.get_units())
            if path == "/api/jobs":
                if not api.enable_jobs or api.jobs is None:
                    raise PermissionError("jobs disabled; pass --enable-jobs")
                return _json_bytes({"items": api.jobs.list_jobs()})
            m = re.fullmatch(r"/api/jobs/([^/]+)", path)
            if m:
                if not api.enable_jobs or api.jobs is None:
                    raise PermissionError("jobs disabled")
                meta = api.jobs.get_job(m.group(1))
                if meta is None:
                    raise KeyError(m.group(1))
                return _json_bytes(meta)
            m = re.fullmatch(r"/api/jobs/([^/]+)/events", path)
            if m:
                if not api.enable_jobs or api.jobs is None:
                    raise PermissionError("jobs disabled")
                return _json_bytes(
                    api.jobs.job_events(
                        m.group(1),
                        offset=int(qs.get("offset") or 0),
                        limit=int(qs.get("limit") or 65536),
                    )
                )
            return self._static(path)

        def _route_post(self, path: str, body: Dict[str, Any]) -> tuple[int, bytes, str]:
            m = re.fullmatch(r"/api/functions/([^/]+)/prompt-preview", path)
            if m:
                return _json_bytes(api.preview(m.group(1), body))
            m = re.fullmatch(r"/api/functions/([^/]+)/jobs", path)
            if m:
                if not api.enable_jobs or api.jobs is None:
                    raise PermissionError("jobs disabled; pass --enable-jobs")
                workflow = body.get("workflow") or "new"
                return _json_bytes(api.jobs.start_job(m.group(1), workflow))
            m = re.fullmatch(r"/api/jobs/([^/]+)/cancel", path)
            if m:
                if not api.enable_jobs or api.jobs is None:
                    raise PermissionError("jobs disabled")
                return _json_bytes(api.jobs.cancel_job(m.group(1)))
            return _json_bytes({"detail": "not found"}, 404)

        def _static(self, path: str) -> tuple[int, bytes, str]:
            if not static_root.is_dir():
                return _json_bytes({"detail": "UI not available"}, 404)
            rel = path.lstrip("/") or "index.html"
            candidate = (static_root / rel).resolve()
            try:
                candidate.relative_to(static_root.resolve())
            except ValueError:
                return _json_bytes({"detail": "forbidden"}, 403)
            if candidate.is_file():
                data = candidate.read_bytes()
                ctype = mimetypes.guess_type(str(candidate))[0] or "application/octet-stream"
                return 200, data, ctype
            index_html = static_root / "index.html"
            if index_html.is_file():
                return 200, index_html.read_bytes(), "text/html; charset=utf-8"
            return _json_bytes({"detail": "not found"}, 404)

    return Handler


def serve(
    root: Path | str,
    *,
    host: str = "127.0.0.1",
    port: int = 8765,
    enable_jobs: bool = False,
    region: Optional[str] = None,
    config_path: Optional[Path | str] = None,
) -> None:
    """Bind and serve the Atlas API (blocks)."""
    api = AtlasAPI(
        Path(root),
        region=region,
        enable_jobs=enable_jobs,
        config_path=Path(config_path) if config_path else None,
    )
    if HAS_FASTAPI:
        try:
            import uvicorn
        except ImportError:
            uvicorn = None  # type: ignore
        if uvicorn is not None:
            app = create_fastapi_app(api)
            uvicorn.run(app, host=host, port=port, log_level="info")
            return
    handler = make_stdlib_handler(api)
    server = ThreadingHTTPServer((host, port), handler)
    print(f"Decomp Atlas listening on http://{host}:{port} (stdlib)", flush=True)
    server.serve_forever()

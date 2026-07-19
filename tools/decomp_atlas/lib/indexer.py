"""Build and atomically replace the Decomp Atlas SQLite index."""

from __future__ import annotations

import json
import traceback
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.decomp_atlas.lib import artifacts as artifacts_mod
from tools.decomp_atlas.lib import attempts as attempts_mod
from tools.decomp_atlas.lib import experiments as experiments_mod
from tools.decomp_atlas.lib.config import (
    ATLAS_SCHEMA_VERSION,
    INDEXER_VERSION,
    PROJECTION_SEED,
    PROJECTION_VERSION,
    VECTOR_METHOD,
    atlas_root,
    db_path,
    index_state_path,
)
from tools.decomp_atlas.lib.database import (
    atomic_replace,
    file_sha256,
    init_schema,
    open_db,
    set_meta,
    write_json_atomic,
)
from tools.decomp_atlas.lib.project_adapter import AtlasProjectAdapter
from tools.decomp_atlas.lib.scoring import classify_readiness, difficulty_score
from tools.decomp_atlas.lib.similarity import (
    compute_neighbors,
    project_2d,
    projection_rows,
    structural_vector,
)


def _insert_edges(conn, records: List[Dict[str, Any]]) -> int:
    count = 0
    for record in records:
        caller = record["target_id"]
        for callee in record.get("called_functions") or []:
            conn.execute(
                "INSERT OR IGNORE INTO edges(caller_id, callee_id, kind) VALUES(?, ?, ?)",
                (caller, str(callee), "direct"),
            )
            count += 1
        for callee in record.get("unresolved_called_functions") or []:
            conn.execute(
                "INSERT OR IGNORE INTO edges(caller_id, callee_id, kind) VALUES(?, ?, ?)",
                (caller, str(callee), "unresolved"),
            )
            count += 1
        for callee in record.get("abi_helper_calls") or []:
            conn.execute(
                "INSERT OR IGNORE INTO edges(caller_id, callee_id, kind) VALUES(?, ?, ?)",
                (caller, str(callee), "abi-helper"),
            )
            count += 1
        cert = record.get("equivalence_certificate")
        if isinstance(cert, dict):
            for dep in cert.get("callees") or []:
                if isinstance(dep, dict) and dep.get("target_id"):
                    conn.execute(
                        "INSERT OR IGNORE INTO edges(caller_id, callee_id, kind) VALUES(?, ?, ?)",
                        (caller, str(dep["target_id"]), "dependency"),
                    )
                    count += 1
    return count


def _function_insert_tuple(record: Dict[str, Any]) -> tuple:
    return (
        record["target_id"],
        record.get("region"),
        record.get("kind"),
        record.get("display_name"),
        record.get("symbol"),
        record.get("address"),
        record.get("size"),
        record.get("source"),
        record.get("unit"),
        record.get("target_object"),
        record.get("base_object"),
        record.get("tier"),
        record.get("milestone"),
        record.get("capabilities"),
        record.get("required_level"),
        record.get("workflow_status"),
        record.get("status"),
        record.get("instruction_percent"),
        1 if record.get("buildable") else 0,
        record.get("owner"),
        record.get("claimed_at"),
        record.get("origin"),
        record.get("notes") or "",
        int(record.get("direct_call_count") or 0),
        int(record.get("unresolved_call_count") or 0),
        1 if record.get("has_indirect_calls") else 0,
        record.get("instruction_count"),
        record.get("branch_count"),
        record.get("relocation_count"),
        record.get("stack_frame"),
        record.get("readiness"),
        record.get("difficulty"),
        1 if record.get("artifact_fresh") else 0,
        record.get("callgraph_status"),
        record.get("vector_json"),
    )


FUNCTION_INSERT_SQL = """
INSERT INTO functions(
    target_id, region, kind, display_name, symbol, address, size, source, unit,
    target_object, base_object, tier, milestone, capabilities, required_level,
    workflow_status, status, instruction_percent, buildable, owner, claimed_at,
    origin, notes, direct_call_count, unresolved_call_count, has_indirect_calls,
    instruction_count, branch_count, relocation_count, stack_frame, readiness,
    difficulty, artifact_fresh, callgraph_status, vector_json
) VALUES (
    ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?
)
"""


def run_index(
    root: Path | str,
    *,
    full: bool = False,
    vectors: bool = False,
    region: Optional[str] = None,
    config_path: Optional[Path | str] = None,
) -> Dict[str, Any]:
    """Index targets into SQLite under ``build/decomp-atlas/<region>/``.

    On failure the previous database is preserved. Returns a summary dict.
    """
    root_path = Path(root).resolve()
    adapter = AtlasProjectAdapter(root_path, config_path=config_path)
    use_region = region or adapter.region
    out_dir = atlas_root(root_path, use_region)
    out_dir.mkdir(parents=True, exist_ok=True)
    final_db = db_path(root_path, use_region)
    tmp_db = final_db.with_suffix(".db.tmp")
    if tmp_db.exists():
        tmp_db.unlink()

    started = datetime.now(timezone.utc).isoformat()
    warnings: List[str] = []
    artifact_ok = 0
    artifact_fail = 0

    try:
        records = adapter.list_function_records()
        certified = adapter.certified_ids()
        raw_attempts = adapter.load_attempts()
        normalized_attempts = attempts_mod.normalize_attempts(raw_attempts)
        attempt_stats = attempts_mod.attempt_stats_by_target(normalized_attempts)
        raw_experiments = adapter.load_experiments()
        normalized_experiments = experiments_mod.normalize_experiments(raw_experiments)

        vectors_by_id: Dict[str, List[float]] = {}

        for record in records:
            called = record.get("called_functions") or []
            unresolved = record.get("unresolved_called_functions") or []
            record["direct_call_count"] = len(called)
            record["unresolved_call_count"] = len(unresolved)
            # Light metrics from size when artifacts are not extracted.
            size = record.get("size")
            if size and not record.get("instruction_count"):
                record["instruction_count"] = int(size) // 4

            scoring_row = {
                **record,
                "buildable": record.get("buildable"),
                "called_functions": called,
                "unresolved_called_functions": unresolved,
            }
            record["readiness"] = classify_readiness(scoring_row, certified)
            stats = attempt_stats.get(record["target_id"], {})
            record["difficulty"] = difficulty_score(
                size=record.get("size"),
                instruction_count=record.get("instruction_count"),
                branch_count=record.get("branch_count"),
                relocation_count=record.get("relocation_count"),
                stack_frame=record.get("stack_frame"),
                direct_call_count=record["direct_call_count"],
                unresolved_call_count=record["unresolved_call_count"],
                has_indirect_calls=bool(record.get("has_indirect_calls")),
                instruction_percent=record.get("instruction_percent"),
                attempt_count=int(stats.get("attempt_count") or 0),
                non_improving_attempts=int(stats.get("non_improving_attempts") or 0),
            )
            record["artifact_fresh"] = False
            record["vector_json"] = None

            if full:
                try:
                    artifact = artifacts_mod.extract_artifacts(root=root_path, record=record)
                    if artifact.get("instruction_count") is not None:
                        record["instruction_count"] = artifact["instruction_count"]
                    if artifact.get("branch_count") is not None:
                        record["branch_count"] = artifact["branch_count"]
                    if artifact.get("relocation_count") is not None:
                        record["relocation_count"] = artifact["relocation_count"]
                    if artifact.get("stack_frame") is not None:
                        record["stack_frame"] = artifact["stack_frame"]
                    record["difficulty"] = difficulty_score(
                        size=record.get("size"),
                        instruction_count=record.get("instruction_count"),
                        branch_count=record.get("branch_count"),
                        relocation_count=record.get("relocation_count"),
                        stack_frame=record.get("stack_frame"),
                        direct_call_count=record["direct_call_count"],
                        unresolved_call_count=record["unresolved_call_count"],
                        has_indirect_calls=bool(record.get("has_indirect_calls")),
                        instruction_percent=record.get("instruction_percent"),
                        attempt_count=int(stats.get("attempt_count") or 0),
                        non_improving_attempts=int(stats.get("non_improving_attempts") or 0),
                    )
                    record["_artifact"] = artifact
                    record["artifact_fresh"] = True
                    if artifact.get("warnings"):
                        warnings.extend(
                            f"{record['target_id']}: {w}" for w in artifact["warnings"]
                        )
                    artifact_ok += 1
                except Exception as exc:  # noqa: BLE001 — per-function isolation
                    artifact_fail += 1
                    warnings.append(f"{record['target_id']}: artifact crash: {exc}")
                    record["_artifact"] = {
                        "target_id": record["target_id"],
                        "warnings": [str(exc)],
                        "updated_at": datetime.now(timezone.utc).isoformat(),
                    }

            if vectors:
                instructions = None
                artifact = record.get("_artifact")
                if isinstance(artifact, dict):
                    instructions = artifact.get("instructions")
                vec = structural_vector(
                    instructions=instructions,
                    size=record.get("size"),
                    direct_call_count=float(record["direct_call_count"]),
                    unresolved_call_count=float(record["unresolved_call_count"]),
                    has_indirect_calls=bool(record.get("has_indirect_calls")),
                    relocation_count=record.get("relocation_count"),
                    stack_frame=record.get("stack_frame"),
                    instruction_count=record.get("instruction_count"),
                    branch_count=record.get("branch_count"),
                    match_percent=record.get("instruction_percent"),
                )
                record["vector_json"] = json.dumps(vec)
                vectors_by_id[record["target_id"]] = vec

        conn = open_db(tmp_db)
        try:
            init_schema(conn)
            set_meta(conn, "schema_version", str(ATLAS_SCHEMA_VERSION))
            set_meta(conn, "indexer_version", INDEXER_VERSION)
            set_meta(conn, "region", use_region)
            set_meta(conn, "generated_at", started)
            set_meta(conn, "project_root", str(root_path))
            set_meta(conn, "targets_hash", file_sha256(adapter.targets_path()) or "")
            set_meta(conn, "objdiff_hash", file_sha256(adapter.objdiff_path()) or "")
            symbols = root_path / "config" / use_region / "symbols.txt"
            set_meta(conn, "symbols_hash", file_sha256(symbols) or "")
            set_meta(conn, "attempts_hash", file_sha256(adapter.attempts_path()) or "")
            set_meta(conn, "experiments_hash", file_sha256(adapter.experiments_path()) or "")
            set_meta(conn, "vector_method", VECTOR_METHOD if vectors else "")
            set_meta(conn, "projection_seed", str(PROJECTION_SEED) if vectors else "")
            set_meta(conn, "projection_version", PROJECTION_VERSION if vectors else "")
            set_meta(conn, "full_index", "1" if full else "0")
            set_meta(conn, "vectors_index", "1" if vectors else "0")

            for record in records:
                conn.execute(FUNCTION_INSERT_SQL, _function_insert_tuple(record))

            edge_count = _insert_edges(conn, records)

            for attempt in normalized_attempts:
                conn.execute(
                    """
                    INSERT INTO attempts(
                        target_id, timestamp, status, instruction_match, hypothesis,
                        next_change, runtime_test, policy_exception, equivalence_status, raw_json
                    ) VALUES (?,?,?,?,?,?,?,?,?,?)
                    """,
                    (
                        attempt["target_id"],
                        attempt.get("timestamp"),
                        attempt.get("status"),
                        attempt.get("instruction_match"),
                        attempt.get("hypothesis"),
                        attempt.get("next_change"),
                        attempt.get("runtime_test"),
                        attempt.get("policy_exception", 0),
                        attempt.get("equivalence_status"),
                        json.dumps(attempt.get("raw") or {}),
                    ),
                )

            for experiment in normalized_experiments:
                conn.execute(
                    """
                    INSERT INTO experiments(
                        experiment_id, target_id, workflow, model, model_id, run_index,
                        timestamp, duration_seconds, input_tokens, output_tokens, cost,
                        result_status, match_percent, equivalence_result, size_ok,
                        accepted, winner, error, artifact, raw_json
                    ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)
                    """,
                    (
                        experiment.get("experiment_id"),
                        experiment["target_id"],
                        experiment.get("workflow"),
                        experiment.get("model"),
                        experiment.get("model_id"),
                        experiment.get("run_index"),
                        experiment.get("timestamp"),
                        experiment.get("duration_seconds"),
                        experiment.get("input_tokens"),
                        experiment.get("output_tokens"),
                        experiment.get("cost"),
                        experiment.get("result_status"),
                        experiment.get("match_percent"),
                        experiment.get("equivalence_result"),
                        experiment.get("size_ok"),
                        experiment.get("accepted"),
                        experiment.get("winner"),
                        experiment.get("error"),
                        experiment.get("artifact"),
                        json.dumps(experiment.get("raw") or {}),
                    ),
                )

            if full:
                for record in records:
                    artifact = record.get("_artifact")
                    if not isinstance(artifact, dict):
                        continue
                    row = artifacts_mod.artifact_row_for_db(artifact)
                    conn.execute(
                        """
                        INSERT INTO artifacts(
                            target_id, retail_object_hash, candidate_object_hash, source_hash,
                            cpp_source, retail_asm, candidate_asm, relocations_json,
                            decoded_json, warnings_json, updated_at
                        ) VALUES (?,?,?,?,?,?,?,?,?,?,?)
                        """,
                        (
                            row["target_id"],
                            row["retail_object_hash"],
                            row["candidate_object_hash"],
                            row["source_hash"],
                            row["cpp_source"],
                            row["retail_asm"],
                            row["candidate_asm"],
                            row["relocations_json"],
                            row["decoded_json"],
                            row["warnings_json"],
                            row["updated_at"],
                        ),
                    )

            neighbor_count = 0
            projection_count = 0
            if vectors and vectors_by_id:
                for neighbor in compute_neighbors(vectors_by_id, top_k=10):
                    conn.execute(
                        """
                        INSERT INTO neighbors(target_id, neighbor_id, score, method, rank)
                        VALUES (?,?,?,?,?)
                        """,
                        (
                            neighbor["target_id"],
                            neighbor["neighbor_id"],
                            neighbor["score"],
                            neighbor["method"],
                            neighbor["rank"],
                        ),
                    )
                    neighbor_count += 1
                points = project_2d(vectors_by_id, seed=PROJECTION_SEED)
                for prow in projection_rows(points, version=PROJECTION_VERSION):
                    conn.execute(
                        "INSERT INTO projection(target_id, x, y, version) VALUES (?,?,?,?)",
                        (prow["target_id"], prow["x"], prow["y"], prow["version"]),
                    )
                    projection_count += 1

            set_meta(conn, "function_count", str(len(records)))
            set_meta(conn, "edge_count", str(edge_count))
            set_meta(conn, "attempt_count", str(len(normalized_attempts)))
            set_meta(conn, "experiment_count", str(len(normalized_experiments)))
            set_meta(conn, "neighbor_count", str(neighbor_count))
            set_meta(conn, "projection_count", str(projection_count))
            set_meta(conn, "artifact_ok", str(artifact_ok))
            set_meta(conn, "artifact_fail", str(artifact_fail))
            conn.commit()
        finally:
            conn.close()

        atomic_replace(tmp_db, final_db)

        state = {
            "ok": True,
            "region": use_region,
            "generated_at": started,
            "db_path": (
                str(final_db.relative_to(root_path))
                if str(final_db).startswith(str(root_path))
                else str(final_db)
            ),
            "schema_version": ATLAS_SCHEMA_VERSION,
            "indexer_version": INDEXER_VERSION,
            "full": full,
            "vectors": vectors,
            "counts": {
                "functions": len(records),
                "edges": edge_count,
                "attempts": len(normalized_attempts),
                "experiments": len(normalized_experiments),
                "neighbors": neighbor_count,
                "projection": projection_count,
                "artifact_ok": artifact_ok,
                "artifact_fail": artifact_fail,
            },
            "hashes": {
                "targets": file_sha256(adapter.targets_path()),
                "objdiff": file_sha256(adapter.objdiff_path()),
                "attempts": file_sha256(adapter.attempts_path()),
                "experiments": file_sha256(adapter.experiments_path()),
            },
            "warnings": warnings[:200],
            "warning_count": len(warnings),
        }
        write_json_atomic(index_state_path(root_path, use_region), state)
        return state
    except Exception as exc:
        if tmp_db.exists():
            try:
                tmp_db.unlink()
            except OSError:
                pass
        return {
            "ok": False,
            "error": str(exc),
            "traceback": traceback.format_exc(),
            "region": use_region,
            "preserved_db": final_db.is_file(),
        }

from __future__ import annotations

import importlib
import importlib.util
import inspect
import json
import re
import subprocess
import sys
import threading
import time
import uuid
from concurrent.futures import FIRST_COMPLETED, ThreadPoolExecutor, as_completed, wait
from dataclasses import asdict, replace
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from .candidate_sanitize import normalize_source_for_compare
from .compile_diagnostic import (
    clean_mwcc_diagnostics,
    cookbook_for_diagnostics,
    normalize_compile_output,
    select_root_diagnostic,
)
from .metrics import TimingRecorder, timings_from_records
from .promotion import PromotionManager, evaluation_to_candidate, capture_baseline
from .providers import (
    CodexProvider,
    DeepSeekRawProvider,
    LMStudioProvider,
    OmlxProvider,
    OpenCodeCircuitBreaker,
    OpenCodeEmptyResponse,
    OpenCodeProvider,
    OpenRouterProvider,
    ReasonixProvider,
)
from .types import (
    BaselineSnapshot,
    Candidate,
    CandidateEvaluation,
    ExperimentRecord,
    ModelConfig,
    ProjectAdapter,
    PromotionPolicy,
    PromotionResult,
    SourcePatch,
)
from .workspace import GitWorktreeManager

# Defaults for ``solve-local`` (scaffold+sanitize path for smaller local models).
# Intentionally does *not* inherit solve.initial_candidates / repair budgets —
# those are tuned for stronger models that escape COMPILE_ERROR.
SOLVE_LOCAL_DEFAULTS: Dict[str, Any] = {
    "initial_candidates": 1,
    "compile_repairs": 2,
    "match_repairs": 3,
    "max_repeated_fingerprint": 1,
    "stop_on_full_match": True,
    "stop_on_equivalent_match": True,
    "stop_if_all_initial_compile_error": True,
    "reject_noop_compile_repair": True,
    "strategies": ["typed"],
}


def _debug_value(value: Any) -> str:
    """Keep progress messages single-line and bounded when errors contain paths/output."""
    return " ".join(str(value).split())[:300]


class Harness:
    def __init__(self, config_path: Path) -> None:
        self.config_path = config_path.resolve()
        self.config = json.loads(self.config_path.read_text(encoding="utf-8"))
        self._validate_config()
        self.root = self.config_path.parent.resolve()
        self.output_dir = (self.root / self.config.get("output_dir", "build/llm-harness")).resolve()
        self.log_path = self.output_dir / "experiments.jsonl"
        self.models, self.workflow_models = self._load_models(
            self.config.get("models", [])
        )
        self.adapter = self._load_adapter(self.root)
        execution = self.config.get("execution", {})
        self.max_parallel = int(
            execution.get(
                "max_parallel",
                execution.get("initial_parallel", 1),
            )
        )
        self.max_target_parallel = int(
            execution.get(
                "max_target_parallel",
                execution.get("target_parallel", 1),
            )
        )
        self.batch_model_parallel = int(execution.get("batch_model_parallel", 1))
        self.workflow_execution = self._load_execution_pipelines(
            execution.get("pipelines", {})
        )
        self.max_retries = int(execution.get("max_retries", 1))
        self._adapter_lock = threading.RLock()
        # Serializes auto-promote into canonical source / targets.json.
        self._promote_lock = threading.Lock()
        self._promotion_lock = self._promote_lock  # alias for older call sites
        # Serializes evaluate() when a solve reuses one worktree across threads.
        self._workspace_eval_lock = threading.Lock()
        self._log_lock = threading.Lock()
        self._debug_lock = threading.Lock()
        # METRICS: per-harness phase accumulator; solve/finalize snapshots into state.json
        self.timings = TimingRecorder()
        # Pause OpenCode after consecutive empty/provider failures (batch-wide).
        self._opencode_breaker = OpenCodeCircuitBreaker(threshold=3)
        self.promotion_manager = PromotionManager(self.root, policy=PromotionPolicy())
        # Auto-promote FULL_MATCH / EQUIVALENT_MATCH winners into canonical source.
        self.auto_promote = bool(execution.get("auto_promote", True))
        self.auto_promote_owner = str(
            execution.get("auto_promote_owner") or "llm-harness"
        )
        # When true, parallel solve batches defer promote until all targets finish.
        self.auto_promote_deferred = bool(
            execution.get("auto_promote_deferred", False)
        )
        isolation = execution.get("isolation", {})
        self.isolation_mode = isolation.get("mode", "none")
        self.workspace_manager = (
            GitWorktreeManager(self.root, copy_dirty=bool(isolation.get("copy_dirty", True)))
            if self.isolation_mode == "git-worktree"
            else None
        )
        _PROVIDER_CLASSES = {
            "opencode": OpenCodeProvider,
            "reasonix": ReasonixProvider,
            "deepseek-raw": DeepSeekRawProvider,
            "lmstudio": LMStudioProvider,
            "omlx": OmlxProvider,
            "openrouter": OpenRouterProvider,
            "codex": CodexProvider,
        }
        self.providers = {}
        for name, cfg in self.config.get("providers", {}).items():
            cls = _PROVIDER_CLASSES.get(name)
            if cls is None:
                continue
            if name == "openrouter":
                api_key_raw = cfg.get("api_key")
                self.providers[name] = cls(
                    timeout_seconds=int(cfg.get("timeout_seconds", 300)),
                    temperature=float(cfg.get("temperature", 0.1)),
                    max_tokens=int(cfg.get("max_tokens", 4096)),
                    json_object=bool(cfg.get("json_object", True)),
                    api_key=str(api_key_raw) if api_key_raw is not None else None,
                    site_url=str(
                        cfg.get("site_url", OpenRouterProvider.DEFAULT_SITE_URL)
                    ),
                    app_name=str(
                        cfg.get("app_name", OpenRouterProvider.DEFAULT_APP_NAME)
                    ),
                    allow_fallbacks=bool(cfg.get("allow_fallbacks", True)),
                    pure=bool(cfg.get("pure", True)),
                )
            elif name == "lmstudio":
                budget_raw = cfg.get("thinking_budget")
                effort_raw = cfg.get("reasoning_effort")
                self.providers[name] = cls(
                    base_url=str(cfg.get("base_url", LMStudioProvider.DEFAULT_BASE_URL)),
                    api_key=str(cfg.get("api_key", "lm-studio")),
                    timeout_seconds=int(cfg.get("timeout_seconds", 900)),
                    temperature=float(cfg.get("temperature", 0.1)),
                    max_tokens=int(cfg.get("max_tokens", 4096)),
                    json_object=bool(cfg.get("json_object", True)),
                    enable_thinking=bool(cfg.get("enable_thinking", False)),
                    thinking_budget=int(budget_raw) if budget_raw is not None else None,
                    reasoning_effort=str(effort_raw) if effort_raw is not None else None,
                    pure=bool(cfg.get("pure", True)),
                )
            elif name == "omlx":
                budget_raw = cfg.get("thinking_budget")
                effort_raw = cfg.get("reasoning_effort")
                api_key_raw = cfg.get("api_key")
                self.providers[name] = cls(
                    base_url=str(cfg.get("base_url", OmlxProvider.DEFAULT_BASE_URL)),
                    api_key=str(api_key_raw) if api_key_raw is not None else None,
                    timeout_seconds=int(cfg.get("timeout_seconds", 900)),
                    temperature=float(cfg.get("temperature", 0.1)),
                    max_tokens=int(cfg.get("max_tokens", 4096)),
                    json_object=bool(cfg.get("json_object", True)),
                    enable_thinking=bool(cfg.get("enable_thinking", False)),
                    thinking_budget=int(budget_raw) if budget_raw is not None else None,
                    reasoning_effort=str(effort_raw) if effort_raw is not None else None,
                    pure=bool(cfg.get("pure", True)),
                )
            elif name == "deepseek-raw":
                self.providers[name] = cls(
                    binary=cfg.get("binary", name),
                    timeout_seconds=int(cfg.get("timeout_seconds", 300)),
                    pure=bool(cfg.get("pure", True)),
                )
            elif name == "opencode":
                self.providers[name] = cls(
                    binary=str(cfg.get("binary", "opencode")),
                    timeout_seconds=int(cfg.get("timeout_seconds", 900)),
                    pure=bool(cfg.get("pure", True)),
                )
            elif name == "codex":
                self.providers[name] = cls(
                    binary=str(cfg.get("binary", "codex")),
                    timeout_seconds=int(cfg.get("timeout_seconds", 900)),
                    pure=bool(cfg.get("pure", True)),
                    sandbox=str(cfg.get("sandbox", "read-only")),
                    ephemeral=bool(cfg.get("ephemeral", True)),
                    json_schema=bool(cfg.get("json_schema", True)),
                    skip_git_repo_check=bool(cfg.get("skip_git_repo_check", False)),
                )
            else:
                self.providers[name] = cls(
                    binary=cfg.get("binary", name),
                    timeout_seconds=int(cfg.get("timeout_seconds", 900)),
                    pure=bool(cfg.get("pure", True)),
                )

    def _validate_config(self) -> None:
        """Phase 0: Reject misplaced/removed config keys explicitly."""
        removed_top_level = {
            "pipeline", "dossier", "structural", "promotion", "knowledge", "features"
        }
        found_removed = [k for k in removed_top_level if k in self.config]
        if found_removed:
            raise ValueError(
                f"llm-harness.json: removed top-level keys found: {', '.join(found_removed)}. "
                f"These are no longer used by the harness. See implementation plan for migration."
            )
        execution = self.config.get("execution", {})
        if "promotion" in execution:
            raise ValueError(
                "llm-harness.json: execution.promotion is no longer read; "
                "use execution.auto_promote / execution.auto_promote_owner, "
                "or explicit 'promote --write --owner'"
            )
        # Check project.mwcc_knowledge_* (removed in Phase 1)
        project = self.config.get("project", {})
        deprecated_project_keys = {
            "mwcc_knowledge_enabled", "mwcc_knowledge_limit", "mwcc_knowledge_body_chars",
            "mwcc_knowledge_reference", "cookbook_path"
        }
        found_deprecated = [k for k in deprecated_project_keys if k in project]
        if found_deprecated:
            raise ValueError(
                f"llm-harness.json: project contains deprecated knowledge keys: {', '.join(found_deprecated)}. "
                f"Knowledge base removed from model path in Phase 1."
            )
        
        # Phase 8: Validate all config keys are recognized
        self._validate_config_keys()
        
        # Phase 8: Compute effective config for dry-run
        self.effective_config = self._compute_effective_config()

    def _validate_config_keys(self) -> None:
        """Phase 8: Validate that all config keys are recognized."""
        # Known top-level keys
        known_top = {
            "project_adapter",
            "output_dir",
            "project",
            "providers",
            "execution",
            "models",
            "solve",
            "solve-local",
            "prompt",
        }
        unknown_top = set(self.config.keys()) - known_top
        if unknown_top:
            raise ValueError(f"llm-harness.json: unknown top-level keys: {', '.join(sorted(unknown_top))}")
        
        # Known project keys
        known_project = {"coop_config", "prompt_dir", "max_source_chars", "context_similar_limit", 
                         "tu_context_chars", "tu_section_byte_limit"}
        project = self.config.get("project", {})
        unknown_project = set(project.keys()) - known_project
        if unknown_project:
            raise ValueError(f"llm-harness.json: project contains unknown keys: {', '.join(sorted(unknown_project))}")
        
        # Known execution keys
        known_exec = {
            "max_parallel", "max_target_parallel", "batch_model_parallel",
            "initial_parallel", "target_parallel", "max_retries", "isolation", "pipelines",
            "auto_promote", "auto_promote_owner", "auto_promote_deferred",
        }
        execution = self.config.get("execution", {})
        unknown_exec = set(execution.keys()) - known_exec
        if unknown_exec:
            raise ValueError(f"llm-harness.json: execution contains unknown keys: {', '.join(sorted(unknown_exec))}")
        
        # Known solve keys (shared acceptance / budget knobs)
        known_solve = {
            "initial_candidates",
            "compile_repairs",
            "match_repairs",
            "max_repeated_fingerprint",
            "stop_on_full_match",
            "stop_on_equivalent_match",
        }
        solve = self.config.get("solve", {})
        unknown_solve = set(solve.keys()) - known_solve
        if unknown_solve:
            raise ValueError(f"llm-harness.json: solve contains unknown keys: {', '.join(sorted(unknown_solve))}")

        known_solve_local = known_solve | {
            "strategies",
            "stop_if_all_initial_compile_error",
            "reject_noop_compile_repair",
        }
        solve_local = self.config.get("solve-local", {})
        if solve_local is None:
            solve_local = {}
        if not isinstance(solve_local, dict):
            raise ValueError("llm-harness.json: solve-local must be an object")
        unknown_solve_local = set(solve_local.keys()) - known_solve_local
        if unknown_solve_local:
            raise ValueError(
                "llm-harness.json: solve-local contains unknown keys: "
                f"{', '.join(sorted(unknown_solve_local))}"
            )
        strategies = solve_local.get("strategies")
        if strategies is not None:
            if not isinstance(strategies, list) or not strategies:
                raise ValueError("llm-harness.json: solve-local.strategies must be a non-empty list")
            allowed = {"literal", "typed", "alternate_cfg"}
            bad = [s for s in strategies if s not in allowed]
            if bad:
                raise ValueError(
                    "llm-harness.json: solve-local.strategies has unknown entries: "
                    f"{', '.join(sorted(str(s) for s in bad))}"
                )
        
        # Known prompt keys
        known_prompt = {"max_chars", "max_decoded_instructions", "max_declaration_chars",
                        "max_callers", "max_sibling_bodies", "include_raw_hex",
                        "max_output_tokens"}
        prompt = self.config.get("prompt", {})
        unknown_prompt = set(prompt.keys()) - known_prompt
        if unknown_prompt:
            raise ValueError(f"llm-harness.json: prompt contains unknown keys: {', '.join(sorted(unknown_prompt))}")
        
        # Validate positive values
        for prefix, section in (("solve", solve), ("solve-local", solve_local)):
            for key in (
                "initial_candidates",
                "compile_repairs",
                "match_repairs",
                "max_repeated_fingerprint",
            ):
                value = section.get(key)
                if value is not None and value < 0:
                    raise ValueError(f"{prefix}.{key} must be nonnegative, got {value}")

    def _compute_effective_config(self) -> Dict[str, Any]:
        """Phase 8: Compute effective config with defaults for dry-run."""
        import copy
        # Deep copy with defaults applied
        config = copy.deepcopy(self.config)
        
        # Apply defaults
        project = config.setdefault("project", {})
        project.setdefault("coop_config", "coop.json")
        project.setdefault("prompt_dir", "tools/llm_harness/prompts")
        project.setdefault("max_source_chars", 120000)
        project.setdefault("context_similar_limit", 4)
        project.setdefault("tu_context_chars", 1500)
        project.setdefault("tu_section_byte_limit", 16384)
        
        execution = config.setdefault("execution", {})
        if "initial_parallel" in execution and "max_parallel" not in execution:
            execution["max_parallel"] = execution["initial_parallel"]
        if "target_parallel" in execution and "max_target_parallel" not in execution:
            execution["max_target_parallel"] = execution["target_parallel"]
        execution.setdefault("max_parallel", 1)
        execution.setdefault("max_target_parallel", 10)
        execution.setdefault("batch_model_parallel", 1)
        execution.setdefault("max_retries", 0)
        execution.setdefault("isolation", {"mode": "none"})
        execution.setdefault("pipelines", {})
        execution.setdefault("auto_promote", True)
        execution.setdefault("auto_promote_owner", "llm-harness")
        execution.setdefault("auto_promote_deferred", False)
        
        solve = config.setdefault("solve", {})
        solve.setdefault("initial_candidates", 2)
        solve.setdefault("compile_repairs", 1)
        solve.setdefault("match_repairs", 3)
        solve.setdefault("max_repeated_fingerprint", 2)
        solve.setdefault("stop_on_full_match", True)
        solve.setdefault("stop_on_equivalent_match", True)

        solve_local = config.setdefault("solve-local", {})
        for key, value in SOLVE_LOCAL_DEFAULTS.items():
            solve_local.setdefault(key, value)
        # Inherit acceptance stops from solve when not overridden.
        for key in ("stop_on_full_match", "stop_on_equivalent_match"):
            if key not in (self.config.get("solve-local") or {}):
                solve_local[key] = solve[key]
        
        prompt = config.setdefault("prompt", {})
        prompt.setdefault("max_chars", 60000)
        prompt.setdefault("max_decoded_instructions", 400)
        prompt.setdefault("max_declaration_chars", 12000)
        prompt.setdefault("max_callers", 3)
        prompt.setdefault("max_sibling_bodies", 3)
        prompt.setdefault("include_raw_hex", False)
        prompt.setdefault("max_output_tokens", 4096)
        
        return config

    def get_effective_config(self) -> Dict[str, Any]:
        """Get the effective config (with defaults applied)."""
        return self.effective_config

    def _load_adapter(self, root: Path) -> ProjectAdapter:
        ref = self.config["project_adapter"]
        if ref.endswith(".py"):
            path = (root / ref).resolve()
            spec = importlib.util.spec_from_file_location("llm_harness_project_adapter", path)
            if spec is None or spec.loader is None:
                raise ImportError(f"Cannot load project adapter: {path}")
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
        else:
            module = importlib.import_module(ref)
        settings = dict(self.config.get("project", {}) or {})
        # Prompt budgets live at harness top-level; the adapter needs them for
        # dossier truncation and sibling/caller context limits.
        settings["prompt"] = dict(self.config.get("prompt", {}) or {})
        return module.create_adapter(root, settings)

    @staticmethod
    def _load_models(
        configured: Any,
    ) -> tuple[List[ModelConfig], Dict[str, List[ModelConfig]]]:
        if isinstance(configured, list):
            models = [ModelConfig(**row) for row in configured]
            if not models:
                raise ValueError("Harness config must define at least one model")
            return models, {}
        if not isinstance(configured, dict):
            raise ValueError("Harness config 'models' must be a list or object")

        allowed = {
            "default", "new", "improve", "tu-complete", "tu-decomp",
            "initial", "repair", "tu", "sample",
        }
        unknown = sorted(set(configured) - allowed)
        if unknown:
            raise ValueError(f"Unknown models pipeline(s): {', '.join(unknown)}")
        pipelines: Dict[str, List[ModelConfig]] = {}
        for workflow, rows in configured.items():
            if not isinstance(rows, list):
                raise ValueError(f"Harness models.{workflow} must be a list")
            pipelines[workflow] = [ModelConfig(**row) for row in rows]
        if not any(pipelines.values()):
            raise ValueError("Harness config must define at least one model")
        return pipelines.get("default", []), pipelines

    def resolve_solve_config(self, *, local: bool = False) -> Dict[str, Any]:
        """Return effective solve / solve-local knobs (defaults applied)."""
        solve = dict(self.config.get("solve") or {})
        if not local:
            out = {
                "initial_candidates": int(solve.get("initial_candidates", 2)),
                "compile_repairs": int(solve.get("compile_repairs", 1)),
                "match_repairs": int(solve.get("match_repairs", 3)),
                "max_repeated_fingerprint": int(solve.get("max_repeated_fingerprint", 2)),
                "stop_on_full_match": bool(solve.get("stop_on_full_match", True)),
                "stop_on_equivalent_match": bool(solve.get("stop_on_equivalent_match", True)),
                "stop_if_all_initial_compile_error": False,
                "reject_noop_compile_repair": False,
                "strategies": ["literal", "typed", "alternate_cfg"][
                    : max(1, int(solve.get("initial_candidates", 2)))
                ],
            }
            return out

        local_raw = dict(self.config.get("solve-local") or {})
        merged = dict(SOLVE_LOCAL_DEFAULTS)
        # Inherit acceptance stops from solve when not set in solve-local.
        for key in ("stop_on_full_match", "stop_on_equivalent_match"):
            if key not in local_raw and key in solve:
                merged[key] = solve[key]
        merged.update(local_raw)

        strategies = merged.get("strategies") or list(SOLVE_LOCAL_DEFAULTS["strategies"])
        if not isinstance(strategies, list):
            strategies = list(SOLVE_LOCAL_DEFAULTS["strategies"])
        initial_n = int(merged.get("initial_candidates", 1))
        strategies = [str(s) for s in strategies][: max(1, initial_n)]
        if not strategies:
            strategies = ["typed"]

        return {
            "initial_candidates": initial_n,
            "compile_repairs": int(merged.get("compile_repairs", 2)),
            "match_repairs": int(merged.get("match_repairs", 3)),
            "max_repeated_fingerprint": int(merged.get("max_repeated_fingerprint", 1)),
            "stop_on_full_match": bool(merged.get("stop_on_full_match", True)),
            "stop_on_equivalent_match": bool(merged.get("stop_on_equivalent_match", True)),
            "stop_if_all_initial_compile_error": bool(
                merged.get("stop_if_all_initial_compile_error", True)
            ),
            "reject_noop_compile_repair": bool(
                merged.get("reject_noop_compile_repair", True)
            ),
            "strategies": strategies,
        }

    def models_for_workflow(self, workflow: str) -> List[ModelConfig]:
        """Resolve models for a workflow or role, with legacy/role aliases."""
        aliases = {
            "solve": ("initial", "new", "default"),
            "solve-local": ("solve-local", "initial", "new", "default"),
            "initial": ("initial", "new", "default"),
            "repair": ("repair", "improve", "default"),
            "new": ("new", "initial", "default"),
            "improve": ("improve", "repair", "default"),
            "tu-complete": ("tu-complete", "tu", "default"),
            "tu-decomp": ("tu-decomp", "tu", "tu-complete", "default"),
            "tu": ("tu", "tu-decomp", "tu-complete", "default"),
            "sample": ("sample", "new", "initial", "default"),
        }
        for key in aliases.get(workflow, (workflow, "default")):
            models = self.workflow_models.get(key)
            if models:
                return models
        if self.models:
            return self.models
        raise ValueError(
            f"Harness config must define models.{workflow} or models.default"
        )

    @staticmethod
    def _load_execution_pipelines(configured: Any) -> Dict[str, Dict[str, int]]:
        if not isinstance(configured, dict):
            raise ValueError("Harness execution.pipelines must be an object")
        allowed_workflows = {"new", "improve", "tu-complete"}
        unknown_workflows = sorted(set(configured) - allowed_workflows)
        if unknown_workflows:
            raise ValueError(
                f"Unknown execution pipeline(s): {', '.join(unknown_workflows)}"
            )
        allowed_options = {
            "max_parallel",
            "max_target_parallel",
            "batch_model_parallel",
        }
        pipelines: Dict[str, Dict[str, int]] = {}
        for workflow, options in configured.items():
            if not isinstance(options, dict):
                raise ValueError(f"Harness execution.pipelines.{workflow} must be an object")
            unknown_options = sorted(set(options) - allowed_options)
            if unknown_options:
                raise ValueError(
                    f"Unknown execution option(s) for {workflow}: "
                    f"{', '.join(unknown_options)}"
                )
            values = {key: int(value) for key, value in options.items()}
            if any(value < 1 for value in values.values()):
                raise ValueError(
                    f"Harness execution.pipelines.{workflow} values must be positive"
                )
            pipelines[workflow] = values
        return pipelines

    def parallelism_for_workflow(self, workflow: str, option: str) -> int:
        global_values = {
            "max_parallel": self.max_parallel,
            "max_target_parallel": self.max_target_parallel,
            "batch_model_parallel": self.batch_model_parallel,
        }
        return self.workflow_execution.get(workflow, {}).get(
            option, global_values[option]
        )

    def run(
        self,
        workflow: str,
        target_id: str,
        runs: Optional[int] = None,
        dry_run: bool = False,
        *,
        resume: Optional[Path] = None,
        max_cost: Optional[float] = None,
        max_tokens: Optional[int] = None,
        max_parallel: Optional[int] = None,
        retry_errors: bool = False,
        full_context: bool = False,
    ) -> Path:
        if workflow not in {"new", "improve", "tu-complete", "tu-decomp"}:
            raise ValueError(f"Unknown workflow: {workflow}")
        if resume:
            experiment_dir = resume.resolve()
            state = json.loads((experiment_dir / "state.json").read_text(encoding="utf-8"))
            if "logged_keys" not in state:
                state["logged_keys"] = (
                    [_record_key(row) for row in state["records"]] if state.get("logged") else []
                )
            if state["workflow"] != workflow or state["target_id"] != target_id:
                raise ValueError("Resume state workflow/target does not match the command")
            experiment_id = state["experiment_id"]
            prompt = (experiment_dir / "prompt.md").read_text(encoding="utf-8")
            if retry_errors:
                error_keys = {_record_key(row) for row in state["records"] if row.get("error")}
                state["records"] = [row for row in state["records"] if not row.get("error")]
                state["logged_keys"] = [
                    key for key in state.get("logged_keys", []) if key not in error_keys
                ]
                state["status"] = "running"
                self._write_state(experiment_dir, state)
        else:
            experiment_id = f"{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')}-{uuid.uuid4().hex[:8]}"
            experiment_dir = self.output_dir / target_id / experiment_id
            experiment_dir.mkdir(parents=True, exist_ok=True)
            with self._adapter_lock:
                history = self.records(target_id=target_id)
                build_prompt = self.adapter.build_prompt
                options = {"full_context": full_context}
                # Phase 2: load best prior candidate for improve workflow
                if workflow == "improve":
                    best_ctx = self._load_best_candidate_context(target_id, self.root)
                    if best_ctx:
                        options["repair_context"] = best_ctx
                if "options" in inspect.signature(build_prompt).parameters:
                    prompt = build_prompt(workflow, target_id, history, options)
                else:
                    prompt = build_prompt(workflow, target_id, history)
            (experiment_dir / "prompt.md").write_text(prompt, encoding="utf-8")
            context_dir = self._write_model_context(
                workflow, target_id, history, prompt, experiment_dir, options
            )
            context_mode_fn = getattr(self.adapter, "model_context_mode", None)
            context_mode = context_mode_fn(workflow) if context_mode_fn else "files"
            state = {
                "schema_version": 2,
                "experiment_id": experiment_id,
                "workflow": workflow,
                "target_id": target_id,
                "status": "running",
                "logged": False,
                "logged_keys": [],
                "records": [],
                "baseline": None,
                "workflow_options": {"full_context": full_context},
                "context_dir": str(context_dir.relative_to(self.root)),
                "context_mode": context_mode,
            }
            # Phase 0/5: mandatory baseline before any model call when supported
            evaluate_canon = getattr(self.adapter, "evaluate_canon", None)
            if evaluate_canon is not None:
                # TU-level workflows don't have a single function target for baseline
                if workflow not in {"tu-complete", "tu-decomp"}:
                    baseline = capture_baseline(self.adapter, target_id, experiment_dir)
                    if baseline.evaluation is None:
                        raise ValueError(
                            f"baseline evaluation failed for {target_id!r}; "
                            "fix the evaluator before making model calls"
                        )
                    baseline_accepted = bool(
                        getattr(baseline.evaluation, "symbol_accepted", False)
                    )
                    state["baseline"] = {
                        "source_hash": baseline.source_hash,
                        "source_text": baseline.source_text,
                        "evaluation": {
                            "status": baseline.evaluation.status.value,
                            "match_percent": baseline.evaluation.match_percent,
                            "accepted": baseline_accepted,
                            "symbol_accepted": baseline_accepted,
                        },
                    }
                    if baseline_accepted:
                        self._debug(
                            f"baseline already accepted target={target_id} "
                            f"status={baseline.evaluation.status.value} "
                            f"match={baseline.evaluation.match_percent}"
                        )
                        record = getattr(self.adapter, "record_baseline_accepted", None)
                        if record is not None:
                            with self._adapter_lock:
                                record(target_id, baseline.evaluation)
                        state["status"] = "complete"
                        state["reason"] = "baseline_already_accepted"
                        state["records"] = []
                        state["baseline_accepted"] = True
                        state["model_calls"] = 0
                        self._write_state(experiment_dir, state)
                        return experiment_dir
            self._write_state(experiment_dir, state)
        self._debug(
            "function decompile started "
            f"target={target_id} workflow={workflow} experiment={experiment_id} "
            f"dry_run={str(dry_run).lower()}"
        )
        if dry_run:
            return experiment_dir

        if state.get("status") == "complete":
            return experiment_dir
        effective_full_context = bool(
            state.get("workflow_options", {}).get(
                "full_context",
                workflow == "tu-complete"
                and "complete replacement translation-unit source" in prompt,
            )
        )
        context_dir = self.root / state.get(
            "context_dir", str(experiment_dir.relative_to(self.root) / "context")
        )
        context_dir.mkdir(parents=True, exist_ok=True)
        if state.get("context_mode", "files") != "inline":
            task_path = context_dir / "TASK.md"
            if not task_path.is_file():
                task_path.write_text(prompt, encoding="utf-8")
        completed = {(row["model_id"], int(row["run_index"])) for row in state["records"]}
        workflow_models = self.models_for_workflow(workflow)
        specs = [
            (model, index)
            for model in workflow_models
            for index in range(1, (runs if runs is not None else model.runs) + 1)
            if (model.id, index) not in completed
        ]
        parallel = (
            max_parallel
            if max_parallel is not None
            else self.parallelism_for_workflow(workflow, "max_parallel")
        )
        if max_cost is not None or max_tokens is not None:
            parallel = 1
        try:
            if parallel > 1 and len(specs) > 1:
                with ThreadPoolExecutor(max_workers=parallel) as executor:
                    pending_specs = iter(specs)
                    futures = {}

                    def submit_next() -> None:
                        try:
                            model, index = next(pending_specs)
                        except StopIteration:
                            return
                        future = executor.submit(
                            self._run_one,
                            workflow,
                            target_id,
                            prompt,
                            experiment_id,
                            experiment_dir,
                            model,
                            index,
                            effective_full_context,
                            context_dir,
                        )
                        futures[future] = (model.id, index)

                    for _ in range(min(parallel, len(specs))):
                        submit_next()
                    stop_submitting = False
                    while futures:
                        done, _ = wait(futures, return_when=FIRST_COMPLETED)
                        for future in done:
                            state["records"].append(future.result().to_json())
                            self._write_state(experiment_dir, state)
                            del futures[future]
                            if self._record_is_full_match(state["records"][-1]):
                                stop_submitting = True
                        if stop_submitting:
                            # Do not launch more agents after FULL_MATCH. Futures
                            # that have not started can be cancelled; already-running
                            # agents must be drained so their evidence is persisted.
                            for pending in list(futures):
                                if pending.cancel():
                                    del futures[pending]
                        else:
                            for _ in range(len(done)):
                                submit_next()
            else:
                for model, index in specs:
                    if self._budget_reached(state["records"], max_cost, max_tokens):
                        state["status"] = "budget_exhausted"
                        self._write_state(experiment_dir, state)
                        self._debug(
                            f"function decompile budget exhausted target={target_id} "
                            f"experiment={experiment_id}"
                        )
                        return experiment_dir
                    record = self._run_one(
                        workflow,
                        target_id,
                        prompt,
                        experiment_id,
                        experiment_dir,
                        model,
                        index,
                        effective_full_context,
                        context_dir,
                    )
                    state["records"].append(record.to_json())
                    self._write_state(experiment_dir, state)
                    if self._record_is_full_match(record.to_json()):
                        self._debug(
                            f"full match reached; stopping remaining agents target={target_id}"
                        )
                        break

            records = [ExperimentRecord(**row) for row in state["records"]]
            successful = [r for r in records if not r.error]
            best_record = None
            if successful:
                best_record = max(
                    successful,
                    key=lambda r: self._rank_record(workflow, r),
                )
                best_record.winner = True
                (experiment_dir / "best.json").write_text(
                    (self.root / best_record.artifact).read_text(encoding="utf-8"), encoding="utf-8"
                )
            state["records"] = [record.to_json() for record in records]
            state["status"] = "complete"
            logged_keys = set(state.get("logged_keys", []))
            for record in records:
                key = _record_key(record.to_json())
                if key not in logged_keys:
                    self._append_record(record)
                    logged_keys.add(key)
            state["logged_keys"] = sorted(logged_keys)
            state["logged"] = True
            self._write_state(experiment_dir, state)
            self._maybe_auto_promote(experiment_dir)
        finally:
            self.adapter.finalize()
        self._debug(
            f"function decompile completed target={target_id} workflow={workflow} "
            f"experiment={experiment_id} status={state['status']}"
        )
        return experiment_dir

    def run_batch(
        self,
        workflow: str,
        target_ids: List[str],
        *,
        runs: Optional[int] = None,
        dry_run: bool = False,
        max_target_parallel: Optional[int] = None,
        model_parallel: Optional[int] = None,
        full_context: bool = False,
        local: bool = False,
    ) -> Path:
        if workflow not in {"new", "improve", "tu-complete", "solve", "solve-local"}:
            raise ValueError(
                "Batch mode supports new, improve, tu-complete, solve, or solve-local"
            )
        solve_local = workflow == "solve-local" or local
        if workflow == "solve-local":
            workflow = "solve"
        targets = list(dict.fromkeys(target_ids))
        if not targets:
            raise ValueError("Batch mode requires at least one target")
        target_parallel = (
            max_target_parallel
            if max_target_parallel is not None
            else self.parallelism_for_workflow(workflow, "max_target_parallel")
        )
        per_target_parallel = (
            model_parallel
            if model_parallel is not None
            else self.parallelism_for_workflow(workflow, "batch_model_parallel")
        )
        if target_parallel < 1 or per_target_parallel < 1:
            raise ValueError("Parallelism values must be positive")
        if target_parallel > 1 and not dry_run and self.workspace_manager is None:
            raise ValueError(
                "Parallel target evaluation requires execution.isolation.mode=git-worktree"
            )
        # Defer auto-promote until the batch ends so concurrent solves do not
        # contend on canonical source; lock still serializes if deferred=false.
        defer_promote = bool(
            workflow == "solve" and self.auto_promote_deferred and not dry_run
        )
        batch_id = f"{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')}-{uuid.uuid4().hex[:8]}"
        batch_dir = self.output_dir / "batches" / batch_id
        batch_dir.mkdir(parents=True, exist_ok=True)
        batch_workflow = "solve-local" if solve_local else workflow
        self._debug(
            f"batch started workflow={batch_workflow} batch={batch_id} "
            f"targets={len(targets)} target_parallel={target_parallel} "
            f"model_parallel={per_target_parallel}"
        )
        manifest: Dict[str, Any] = {
            "schema_version": 1,
            "batch_id": batch_id,
            "workflow": batch_workflow,
            "status": "running",
            "max_target_parallel": target_parallel,
            "model_parallel": per_target_parallel,
            "auto_promote_deferred": defer_promote,
            "targets": {},
        }
        self._write_batch_manifest(batch_dir, manifest)

        def run_target(target_id: str) -> tuple[str, str, Optional[str]]:
            try:
                if workflow == "solve":
                    output = self.solve(
                        target_id,
                        dry_run=dry_run,
                        max_parallel=per_target_parallel,
                        skip_auto_promote=defer_promote,
                        local=solve_local or None,
                    )
                else:
                    output = self.run(
                        workflow,
                        target_id,
                        runs=runs,
                        dry_run=dry_run,
                        max_parallel=per_target_parallel,
                        full_context=full_context,
                    )
                return target_id, str(output.relative_to(self.root)), None
            except Exception as exc:
                return target_id, "", f"{type(exc).__name__}: {exc}"

        with ThreadPoolExecutor(max_workers=min(target_parallel, len(targets))) as executor:
            futures = [executor.submit(run_target, target_id) for target_id in targets]
            for future in as_completed(futures):
                target_id, output, error = future.result()
                manifest["targets"][target_id] = {
                    "experiment": output or None,
                    "error": error,
                }
                self._debug(
                    f"batch target completed batch={batch_id} target={target_id} "
                    f"status={'error' if error else 'ok'}"
                    + (f" detail={_debug_value(error)}" if error else "")
                )
                self._write_batch_manifest(batch_dir, manifest)

        if defer_promote:
            for target_id, row in manifest["targets"].items():
                experiment_rel = row.get("experiment")
                if row.get("error") or not experiment_rel:
                    continue
                experiment_dir = (self.root / experiment_rel).resolve()
                try:
                    self._maybe_auto_promote(experiment_dir)
                except Exception as exc:
                    row["promote_error"] = f"{type(exc).__name__}: {exc}"
                    self._debug(
                        f"batch deferred promote failed batch={batch_id} "
                        f"target={target_id} detail={_debug_value(row['promote_error'])}"
                    )

        manifest["status"] = (
            "complete"
            if all(not row["error"] for row in manifest["targets"].values())
            else "partial"
        )
        self._write_batch_manifest(batch_dir, manifest)
        self._debug(f"batch completed batch={batch_id} status={manifest['status']}")
        return batch_dir

    def solve_batch(
        self,
        target_ids: List[str],
        *,
        dry_run: bool = False,
        max_target_parallel: Optional[int] = None,
        model_parallel: Optional[int] = None,
    ) -> Path:
        """Parallel solve wrapper; same path as ``run_batch(..., workflow='solve')``."""
        return self.run_batch(
            "solve",
            target_ids,
            dry_run=dry_run,
            max_target_parallel=max_target_parallel,
            model_parallel=model_parallel,
        )

    def select_new_targets(
        self, number: int, *, ignore_called_functions: bool = False, certified_funcs: bool = False,
        high_match_callees: bool = False,
        tu: Optional[str] = None,
    ) -> List[str]:
        """Ask the project adapter for fresh function targets, optionally filtered to a TU."""
        if number < 1:
            raise ValueError("number must be positive")
        select = getattr(self.adapter, "select_new_targets", None)
        if select is None:
            raise ValueError("Configured project adapter does not support automatic target selection")
        with self._adapter_lock:
            try:
                target_ids = list(
                    select(
                        number,
                        self.records(),
                        ignore_called_functions=ignore_called_functions,
                        certified_funcs=certified_funcs,
                        high_match_callees=high_match_callees,
                        tu=tu,
                    )
                )
            except TypeError:
                target_ids = list(
                    select(
                        number,
                        self.records(),
                        ignore_called_functions=ignore_called_functions,
                        certified_funcs=certified_funcs,
                        tu=tu,
                    )
                )
        if len(target_ids) != number:
            raise ValueError(
                f"Project adapter returned {len(target_ids)} targets; expected {number}"
            )
        return target_ids

    def select_targets(
        self,
        workflow: str,
        number: int,
        *,
        randomize: bool = False,
        certified_funcs: bool = False,
        high_match_callees: bool = False,
        tu: Optional[str] = None,
        selection: Optional[str] = None,
        min_fuzzy: Optional[float] = None,
    ) -> List[str]:
        """Ask the project adapter for an automatic workflow target selection, optionally filtered to a TU."""
        if workflow not in {"improve", "solve", "tu-complete", "probe"}:
            raise ValueError("Automatic selection supports improve, solve, tu-complete, or probe")
        if number < 1:
            raise ValueError("number must be positive")
        select = getattr(self.adapter, "select_targets", None)
        if select is None:
            raise ValueError("Configured project adapter does not support automatic target selection")
        kwargs: Dict[str, Any] = {
            "randomize": randomize,
            "certified_funcs": certified_funcs,
            "high_match_callees": high_match_callees,
            "tu": tu,
        }
        if selection is not None:
            kwargs["selection"] = selection
        if min_fuzzy is not None:
            kwargs["min_fuzzy"] = min_fuzzy
        with self._adapter_lock:
            try:
                target_ids = list(select(workflow, number, **kwargs))
            except TypeError:
                # Older adapters without selection=/min_fuzzy/high_match_callees kwargs.
                kwargs.pop("selection", None)
                kwargs.pop("min_fuzzy", None)
                kwargs.pop("high_match_callees", None)
                target_ids = list(select(workflow, number, **kwargs))
        if len(target_ids) != number:
            raise ValueError(
                f"Project adapter returned {len(target_ids)} targets; expected {number}"
            )
        return target_ids

    def run_probe(
        self,
        target_ids: List[str],
        *,
        dry_run: bool = False,
        write: bool = False,
        linked: bool = False,
        rebuild: bool = True,
    ) -> Path:
        """Run ppc_equivalence probes on compiling non-accepted targets (no LLM).

        ``dry_run`` still runs objdiff + SMT; it only suppresses ``targets.json``
        updates (forces ``write=False``). Pass ``write=True`` without ``dry_run``
        to persist accepted FULL/EQUIVALENT matches.
        """
        if not target_ids:
            raise ValueError("probe requires at least one target id")
        probe_fn = getattr(self.adapter, "probe_target", None)
        if probe_fn is None:
            raise ValueError("Configured project adapter does not support probe")

        # dry_run = probe for real, but never mutate the registry.
        persist = bool(write) and not dry_run

        batch_id = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
        batch_dir = self.output_dir / "probe" / batch_id
        batch_dir.mkdir(parents=True, exist_ok=True)
        results_path = batch_dir / "results.jsonl"
        summary_path = batch_dir / "summary.json"

        self._debug(
            f"probe started batch={batch_id} targets={len(target_ids)} "
            f"dry_run={dry_run} write={persist}"
        )

        results: List[Dict[str, Any]] = []
        accepted = 0
        errors = 0
        with results_path.open("w", encoding="utf-8") as out:
            for target_id in target_ids:
                self._debug(f"probe target={target_id}")
                try:
                    with self._adapter_lock:
                        row = dict(
                            probe_fn(
                                target_id,
                                write=persist,
                                linked=linked,
                                rebuild=rebuild,
                            )
                        )
                except Exception as exc:
                    row = {
                        "target_id": target_id,
                        "status": "PROBE_ERROR",
                        "accepted": False,
                        "detail": f"{type(exc).__name__}: {exc}",
                        "written": False,
                    }
                    errors += 1
                if row.get("accepted"):
                    accepted += 1
                elif str(row.get("status") or "").endswith("ERROR"):
                    errors += 1
                results.append(row)
                out.write(json.dumps(row, separators=(",", ":")) + "\n")
                out.flush()
                self._debug(
                    f"probe done target={target_id} status={row.get('status')} "
                    f"match={row.get('match_percent')} equiv={row.get('equivalence')} "
                    f"written={row.get('written')}"
                )

        summary = {
            "workflow": "probe",
            "batch_id": batch_id,
            "dry_run": dry_run,
            "write": persist,
            "linked": linked,
            "rebuild": rebuild,
            "count": len(target_ids),
            "accepted": accepted,
            "errors": errors,
            "results": results,
        }
        summary_path.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
        self._debug(
            f"probe complete batch={batch_id} accepted={accepted} errors={errors}"
        )
        return batch_dir

    def target_ids_for_unit(self, unit_name: str, workflow: str) -> List[str]:
        """Ask the project adapter for all eligible target IDs in a translation unit."""
        resolve = getattr(self.adapter, "target_ids_for_unit", None)
        if resolve is None:
            raise ValueError("Configured project adapter does not support TU target resolution")
        with self._adapter_lock:
            return list(resolve(unit_name, workflow))

    def _write_model_context(
        self,
        workflow: str,
        target_id: str,
        history: List[Dict[str, Any]],
        prompt: str,
        experiment_dir: Path,
        options: Dict[str, Any],
    ) -> Path:
        context_dir = experiment_dir / "context"
        context_dir.mkdir(parents=True, exist_ok=True)
        context_mode_fn = getattr(self.adapter, "model_context_mode", None)
        context_mode = context_mode_fn(workflow) if context_mode_fn else "files"
        files: Dict[str, str] = {"TASK.md": prompt} if context_mode != "inline" else {}
        build_files = getattr(self.adapter, "build_context_files", None)
        if build_files is not None:
            with self._adapter_lock:
                files.update(build_files(workflow, target_id, history, prompt, options))
        for relative, content in files.items():
            path = Path(relative)
            if path.is_absolute() or ".." in path.parts:
                raise ValueError(f"Unsafe model context path: {relative}")
            destination = context_dir / path
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_text(content, encoding="utf-8")
        return context_dir

    def _model_with_output_budget(
        self, model: ModelConfig, target_id: str, workflow: str
    ) -> ModelConfig:
        """Clamp generation length so stub functions cannot emit 8k-token essays."""
        if model.max_tokens is not None:
            return model
        prompt_cfg = self.config.get("prompt", {}) or {}
        configured_cap = int(prompt_cfg.get("max_output_tokens", 4096))
        suggested = configured_cap
        suggest = getattr(self.adapter, "suggest_max_output_tokens", None)
        if callable(suggest) and workflow != "tu-complete":
            try:
                suggested = int(suggest(target_id))
            except Exception:
                suggested = configured_cap
        effective = max(512, min(configured_cap, suggested))
        return replace(model, max_tokens=effective)

    @staticmethod
    def _write_batch_manifest(batch_dir: Path, manifest: Dict[str, Any]) -> None:
        path = batch_dir / "batch.json"
        temporary = path.with_suffix(".json.tmp")
        temporary.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
        temporary.replace(path)

    def _run_one(
        self,
        workflow: str,
        target_id: str,
        prompt: str,
        experiment_id: str,
        experiment_dir: Path,
        model: ModelConfig,
        index: int,
        full_context: bool = False,
        context_dir: Optional[Path] = None,
        workspace: Optional[Path] = None,
    ) -> ExperimentRecord:
        provider = self.providers.get(model.provider)
        if provider is None:
            raise ValueError(f"Unsupported provider: {model.provider}")
        artifact = experiment_dir / f"{model.id}-{index}.json"
        error: Optional[str] = None
        timed_out = False
        evaluation: Dict[str, Any] = {}
        candidate_summary: Dict[str, Any] = {}
        result = None
        provider_attempts = 0
        candidate = None
        started = time.monotonic()
        agent = model.agent or "default"
        self._debug(
            f"agent started target={target_id} agent={agent} model={model.model} "
            f"run={index}"
        )
        format_repair_attempted = False
        call_model = self._model_with_output_budget(model, target_id, workflow)
        try:
            while True:
                provider_attempts += 1
                try:
                    if isinstance(provider, OpenCodeProvider):
                        self._opencode_breaker.raise_if_open()
                    # METRICS: llm = provider wall time (including retries of this attempt)
                    with self.timings.measure("llm"):
                        result = provider.invoke(
                            prompt, call_model, context_dir or Path(self.adapter.root)
                        )
                    if isinstance(provider, OpenCodeProvider):
                        self._opencode_breaker.record_success()
                    break
                except Exception as invoke_exc:
                    if isinstance(provider, OpenCodeProvider):
                        self._opencode_breaker.record_failure()
                    # Empty responses are terminal: do not burn retries on them.
                    if self._is_empty_provider_exception(invoke_exc):
                        raise
                    if provider_attempts > self.max_retries:
                        raise
                    self._debug(
                        f"agent retry target={target_id} agent={agent} model={model.model} "
                        f"run={index} attempt={provider_attempts + 1}"
                    )
                    time.sleep(min(2 ** (provider_attempts - 1), 4))
            try:
                candidate = parse_candidate(
                    result.text, workflow=workflow, full_context=full_context
                )
            except (json.JSONDecodeError, ValueError) as parse_exc:
                error_msg = f"{type(parse_exc).__name__}: {parse_exc}"
                self._debug(
                    f"agent format-repair target={target_id} agent={agent} model={model.model} "
                    f"run={index} error={_debug_value(error_msg)}"
                )
                # METRICS: format-repair is a second LLM call
                with self.timings.measure("llm"):
                    repair_result = self._attempt_format_repair(
                        result.text,
                        error_msg,
                        call_model,
                        provider,
                        context_dir,
                        workflow=workflow,
                        full_context=full_context,
                    )
                if repair_result is not None:
                    candidate = repair_result
                    format_repair_attempted = True
                else:
                    raise parse_exc
            candidate_summary = {
                "hypothesis": candidate.hypothesis,
                "notes": candidate.notes,
                "next_change": candidate.next_change,
                "provider_attempts": provider_attempts,
                "patch_ids": [patch.slot_id for patch in candidate.patches],
            }
            evaluation_obj = self._evaluate_candidate(
                workflow,
                target_id,
                candidate,
                f"{model.id}-{index}",
                workspace=workspace,
            )
            evaluation = asdict(evaluation_obj)
            artifact.write_text(
                json.dumps({"candidate": asdict(candidate), "evaluation": evaluation}, indent=2) + "\n",
                encoding="utf-8",
            )
            _write_tu_decomp_sidecars(artifact, candidate)
        except Exception as exc:
            timed_out = isinstance(exc, subprocess.TimeoutExpired)
            error = f"{type(exc).__name__}: {exc}"
            self._debug(
                f"agent result target={target_id} agent={agent} model={model.model} "
                f"run={index} status=error duration={time.monotonic() - started:.1f}s "
                f"detail={_debug_value(error)}"
            )
            payload: Dict[str, Any] = {"error": error, "provider_attempts": provider_attempts}
            if candidate is not None:
                payload["candidate"] = asdict(candidate)
            payload["format_repair_attempted"] = format_repair_attempted
            if self._is_empty_provider_error(error):
                payload["empty_provider_response"] = True
            artifact.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
        else:
            self._debug(
                f"agent result target={target_id} agent={agent} model={model.model} "
                f"run={index} status={evaluation.get('status', 'unknown')} "
                f"match={evaluation.get('match_percent')} accepted="
                f"{evaluation.get('accepted')} duration={time.monotonic() - started:.1f}s"
            )
        record = ExperimentRecord(
            schema_version=4,
            experiment_id=experiment_id,
            timestamp=datetime.now(timezone.utc).isoformat(),
            workflow=workflow,
            target_id=target_id,
            model_id=model.id,
            model=model.model,
            run_index=index,
            duration_seconds=result.duration_seconds if result else time.monotonic() - started,
            input_tokens=result.input_tokens if result else None,
            output_tokens=result.output_tokens if result else None,
            cache_read_tokens=result.cache_read_tokens if result else None,
            cache_write_tokens=result.cache_write_tokens if result else None,
            timed_out=timed_out,
            cost=result.cost if result else None,
            evaluation=evaluation,
            artifact=str(artifact.relative_to(self.root)),
            candidate_summary=candidate_summary,
            error=error,
        )
        # Log full prompt/response to io.jsonl when a provider result was received
        if result is not None:
            self._append_io_log(prompt, result.text, record)
        return record

    def _evaluate_candidate(
        self,
        workflow: str,
        target_id: str,
        candidate: Candidate,
        label: str,
        *,
        workspace: Optional[Path] = None,
    ) -> Any:
        if self.workspace_manager is None:
            self._bind_adapter_timings(self.adapter)
            return self.adapter.evaluate(workflow, target_id, candidate)
        # Reuse a caller-owned worktree (solve scoped) without create/remove.
        # Serial max_target_parallel==1 still prefers reuse over in-place evaluate
        # so canonical source stays untouched (safer for accidental mutation).
        if workspace is not None:
            with self._workspace_eval_lock:
                return self._evaluate_in_workspace(workspace, workflow, target_id, candidate)
        # Legacy per-call isolation for non-solve workflows / parallel initial strategies.
        with self.timings.measure("worktree_create"):
            created = self.workspace_manager.create(label)
        try:
            prepare = getattr(self.adapter, "prepare_workspace", None)
            if prepare is None:
                raise ValueError("Project adapter does not support isolated workspaces")
            with self.timings.measure("configure"):
                prepare(created)
            return self._evaluate_in_workspace(created, workflow, target_id, candidate)
        finally:
            self.workspace_manager.remove(created)

    def _bind_adapter_timings(self, adapter: Any) -> None:
        """Point the adapter at this harness's TimingRecorder (ninja/objdiff/smt)."""
        if hasattr(adapter, "timings"):
            adapter.timings = self.timings

    def _evaluate_in_workspace(
        self,
        workspace: Path,
        workflow: str,
        target_id: str,
        candidate: Candidate,
    ) -> Any:
        adapter = self._load_adapter(workspace)
        self._bind_adapter_timings(adapter)
        try:
            return adapter.evaluate(workflow, target_id, candidate)
        finally:
            adapter.finalize()

    @staticmethod
    def _is_empty_provider_exception(exc: BaseException) -> bool:
        if isinstance(exc, OpenCodeEmptyResponse):
            return True
        return "empty assistant text" in str(exc).lower()

    @classmethod
    def _is_empty_provider_error(cls, error: Optional[str]) -> bool:
        if not error:
            return False
        return (
            "OpenCodeEmptyResponse" in error
            or "empty assistant text" in error.lower()
        )

    def _write_solve_context(
        self,
        experiment_dir: Path,
        prompt: str,
        *,
        suffix: Optional[str] = None,
    ) -> Path:
        """Create/update experiment context dir and write TASK.md for the prompt."""
        context_dir = (
            experiment_dir / "context" / suffix
            if suffix
            else experiment_dir / "context"
        )
        context_dir.mkdir(parents=True, exist_ok=True)
        context_mode_fn = getattr(self.adapter, "model_context_mode", None)
        context_mode = context_mode_fn("solve") if context_mode_fn else "files"
        if context_mode != "inline":
            (context_dir / "TASK.md").write_text(prompt, encoding="utf-8")
        return context_dir

    def _prepare_solve_workspace(self, experiment_id: str) -> Optional[Path]:
        """Create one reusable worktree for a solve experiment when isolation is on."""
        if self.workspace_manager is None:
            return None
        workspace = self.workspace_manager.create(f"solve-{experiment_id}")
        prepare = getattr(self.adapter, "prepare_workspace", None)
        if prepare is None:
            self.workspace_manager.remove(workspace)
            raise ValueError("Project adapter does not support isolated workspaces")
        with self.timings.measure("configure"):
            prepare(workspace)
        return workspace

    def _write_state(self, experiment_dir: Path, state: Dict[str, Any]) -> None:
        path = experiment_dir / "state.json"
        temporary = path.with_suffix(".json.tmp")
        temporary.write_text(json.dumps(state, indent=2) + "\n", encoding="utf-8")
        temporary.replace(path)

    def _debug(self, message: str) -> None:
        """Emit flushed, serialized progress output for interactive harness runs."""
        timestamp = datetime.now().astimezone().strftime("%H:%M:%S")
        with self._debug_lock:
            print(
                f"[llm-harness {timestamp}] {message}",
                file=sys.stderr,
                flush=True,
            )

    @staticmethod
    def _record_is_full_match(record: Dict[str, Any]) -> bool:
        evaluation = record.get("evaluation") or {}
        return (
            not record.get("error")
            and evaluation.get("status") == "FULL_MATCH"
            and float(evaluation.get("match_percent") or 0.0) >= 100.0
            and bool(evaluation.get("accepted"))
        )

    @staticmethod
    def _budget_reached(
        records: List[Dict[str, Any]], max_cost: Optional[float], max_tokens: Optional[int]
    ) -> bool:
        cost = sum(float(row.get("cost") or 0) for row in records)
        tokens = sum(
            int(row.get("input_tokens") or 0) + int(row.get("output_tokens") or 0)
            for row in records
        )
        return (max_cost is not None and cost >= max_cost) or (
            max_tokens is not None and tokens >= max_tokens
        )


    def solve(
        self,
        target_id: str,
        *,
        dry_run: bool = False,
        resume: Optional[Path] = None,
        max_parallel: Optional[int] = None,
        skip_auto_promote: bool = False,
        local: Optional[bool] = None,
    ) -> Path:
        """Adaptive closed loop: initial candidates -> compile/binary repair.

        When isolation.mode=git-worktree, one worktree is created for the
        experiment and reused across serial evaluations (including when
        max_target_parallel==1). Parallel initial strategies share that
        worktree under ``_workspace_eval_lock`` so provider calls overlap
        while evaluate stays serialized.

        ``local=True`` enables the ``solve-local`` config section: scaffold+sanitize
        hygiene, local prompts, aggressive candidate sanitizer, no-op repair
        rejection, and early-stop when every initial fails to compile.
        """
        local_mode = bool(local) if local is not None else False
        solve_cfg = self.resolve_solve_config(local=local_mode)
        initial_n = int(solve_cfg["initial_candidates"])
        compile_budget = int(solve_cfg["compile_repairs"])
        match_budget = int(solve_cfg["match_repairs"])
        max_repeated = int(solve_cfg["max_repeated_fingerprint"])
        stop_on_full = bool(solve_cfg["stop_on_full_match"])
        stop_on_equiv = bool(solve_cfg["stop_on_equivalent_match"])
        stop_all_compile = bool(solve_cfg["stop_if_all_initial_compile_error"])
        reject_noop = bool(solve_cfg["reject_noop_compile_repair"])
        strategies = list(solve_cfg["strategies"])[: max(1, initial_n)]

        prev_local = getattr(self.adapter, "local_sanitize", False)
        setattr(self.adapter, "local_sanitize", local_mode)

        if resume:
            experiment_dir = resume.resolve()
            state = json.loads((experiment_dir / "state.json").read_text(encoding="utf-8"))
            if state.get("workflow") not in {"solve", "solve-local"} or state.get("target_id") != target_id:
                raise ValueError("Resume state workflow/target does not match solve command")
            experiment_id = state["experiment_id"]
            prompt = (experiment_dir / "prompt.md").read_text(encoding="utf-8")
            context_dir = self._write_solve_context(experiment_dir, prompt)
            local_mode = bool(state.get("local_mode", local_mode))
            solve_state = state.get("solve") or {}
            stop_all_compile = bool(
                solve_state.get("stop_if_all_initial_compile_error", stop_all_compile)
            )
            reject_noop = bool(
                solve_state.get("reject_noop_compile_repair", reject_noop)
            )
            setattr(self.adapter, "local_sanitize", local_mode)
        else:
            experiment_id = (
                f"{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')}-"
                f"{uuid.uuid4().hex[:8]}"
            )
            experiment_dir = self.output_dir / target_id / experiment_id
            experiment_dir.mkdir(parents=True, exist_ok=True)
            with self._adapter_lock:
                history = self.records(target_id=target_id)
                prompt = self.adapter.build_prompt("new", target_id, history, {})
            (experiment_dir / "prompt.md").write_text(prompt, encoding="utf-8")
            context_dir = self._write_solve_context(experiment_dir, prompt)
            context_mode_fn = getattr(self.adapter, "model_context_mode", None)
            context_mode = context_mode_fn("solve") if context_mode_fn else "files"
            state = {
                "schema_version": 3,
                "experiment_id": experiment_id,
                "workflow": "solve-local" if local_mode else "solve",
                "target_id": target_id,
                "local_mode": local_mode,
                "status": "running",
                "logged": False,
                "logged_keys": [],
                "records": [],
                "branches": [],
                "baseline": None,
                "context_dir": str(context_dir.relative_to(self.root)),
                "context_mode": context_mode,
                "solve": {
                    "initial_candidates": initial_n,
                    "compile_repairs": compile_budget,
                    "match_repairs": match_budget,
                    "max_repeated_fingerprint": max_repeated,
                    "strategies": strategies,
                    "local_mode": local_mode,
                    "stop_if_all_initial_compile_error": stop_all_compile,
                    "reject_noop_compile_repair": reject_noop,
                },
                "model_calls": 0,
            }
            baseline = capture_baseline(self.adapter, target_id, experiment_dir)
            if baseline.evaluation is None:
                raise ValueError(
                    f"baseline evaluation failed for {target_id!r}; "
                    "fix the evaluator before making model calls"
                )
            baseline_accepted = bool(getattr(baseline.evaluation, "symbol_accepted", False))
            state["baseline"] = {
                "source_hash": baseline.source_hash,
                "source_text": baseline.source_text,
                "evaluation": {
                    "status": baseline.evaluation.status.value,
                    "match_percent": baseline.evaluation.match_percent,
                    "accepted": baseline_accepted,
                    "symbol_accepted": baseline_accepted,
                },
            }
            if baseline_accepted:
                self._debug(
                    f"baseline already accepted target={target_id} "
                    f"status={baseline.evaluation.status.value} "
                    f"match={baseline.evaluation.match_percent}"
                )
                record = getattr(self.adapter, "record_baseline_accepted", None)
                if record is not None:
                    with self._adapter_lock:
                        record(target_id, baseline.evaluation)
                state["status"] = "complete"
                state["reason"] = "baseline_already_accepted"
                state["model_calls"] = 0
                self._write_state(experiment_dir, state)
                setattr(self.adapter, "local_sanitize", prev_local)
                return experiment_dir
            self._write_state(experiment_dir, state)
            if dry_run:
                (experiment_dir / "effective-config.json").write_text(
                    json.dumps(self.effective_config, indent=2) + "\n",
                    encoding="utf-8",
                )
                setattr(self.adapter, "local_sanitize", prev_local)
                return experiment_dir

        if dry_run:
            setattr(self.adapter, "local_sanitize", prev_local)
            return experiment_dir

        model_role = "solve-local" if local_mode else "initial"
        initial_models = self.models_for_workflow(model_role)
        repair_models = self.models_for_workflow("repair")
        initial_model = initial_models[0]
        repair_model = repair_models[0]
        run_index = len(state.get("records", []))
        branches: List[Dict[str, Any]] = list(state.get("branches", []))
        fingerprint_counts: Dict[str, int] = {}
        compile_used = 0
        match_used = 0
        solve_workspace: Optional[Path] = None

        def persist() -> None:
            state["records"] = [
                r.to_json() if isinstance(r, ExperimentRecord) else r
                for r in state["records"]
            ]
            state["branches"] = branches
            state["model_calls"] = len(state["records"])
            self._write_state(experiment_dir, state)

        try:
            # Prefer one reusable worktree for the whole solve (serial or parallel
            # target batches each own their own solve() call / worktree).
            if not dry_run:
                with self.timings.measure("worktree_create"):
                    solve_workspace = self._prepare_solve_workspace(experiment_id)

            # --- initial diverse candidates ---
            if not any(b.get("phase") == "initial" for b in branches):
                with self._adapter_lock:
                    history = self.records(target_id=target_id)
                    base_prompt = self.adapter.build_prompt("new", target_id, history, {})

                def _initial_prompt(strategy: str) -> str:
                    return (
                        f"{base_prompt}\n\n"
                        f"## Initial strategy: {strategy}\n"
                        f"- literal: preserve decoded control flow and memory ops directly\n"
                        f"- typed: prioritize declarations, signedness, and class/member APIs\n"
                        f"- alternate_cfg: use a different evidence-supported high-level branch shape\n"
                        f"Apply the `{strategy}` strategy for this candidate.\n"
                    )

                def _run_initial(strategy: str, index: int) -> tuple[str, int, ExperimentRecord]:
                    prompt_text = _initial_prompt(strategy)
                    # Per-run context so parallel initials do not race on TASK.md.
                    run_context = self._write_solve_context(
                        experiment_dir, prompt_text, suffix=f"run-{index}"
                    )
                    record = self._run_one(
                        "new",
                        target_id,
                        prompt_text,
                        experiment_id,
                        experiment_dir,
                        initial_model,
                        index,
                        context_dir=run_context,
                        workspace=solve_workspace,
                    )
                    return strategy, index, record

                initial_results: List[tuple[str, int, ExperimentRecord]] = []
                parallel_initial = (
                    self.workspace_manager is not None and len(strategies) > 1
                )
                if parallel_initial:
                    indexed = [
                        (strategy, run_index + offset + 1)
                        for offset, strategy in enumerate(strategies)
                    ]
                    run_index += len(strategies)
                    workers = min(len(strategies), 3)
                    with ThreadPoolExecutor(max_workers=workers) as executor:
                        futures = [
                            executor.submit(_run_initial, strategy, index)
                            for strategy, index in indexed
                        ]
                        initial_results = [future.result() for future in as_completed(futures)]
                    initial_results.sort(key=lambda row: row[1])
                else:
                    # Serial path: stop early on accepted FULL/EQUIVALENT when configured.
                    for strategy in strategies:
                        run_index += 1
                        strategy, index, record = _run_initial(strategy, run_index)
                        initial_results.append((strategy, index, record))
                        if self._record_is_symbol_accepted(record.to_json()):
                            status = str((record.evaluation or {}).get("status", "")).upper()
                            if (
                                (stop_on_full and status == "FULL_MATCH")
                                or (stop_on_equiv and status == "EQUIVALENT_MATCH")
                                or self._record_is_symbol_accepted(record.to_json())
                            ):
                                break

                for strategy, index, record in initial_results:
                    record.candidate_summary = {
                        **(record.candidate_summary or {}),
                        "strategy": strategy,
                        "parent_id": None,
                        "branch_id": f"init-{strategy}",
                        "iteration": 0,
                    }
                    state["records"].append(record.to_json())
                    branches.append({
                        "branch_id": f"init-{strategy}",
                        "parent_id": None,
                        "phase": "initial",
                        "strategy": strategy,
                        "artifact": record.artifact,
                        "run_index": index,
                        "status": (record.evaluation or {}).get("status"),
                        "match_percent": (record.evaluation or {}).get("match_percent"),
                        "fingerprint": ((record.evaluation or {}).get("metrics") or {}).get(
                            "mismatch_fingerprint", ""
                        ),
                        "error": record.error,
                    })
                persist()

            best = self._best_solve_record(state["records"])
            if best and self._record_is_symbol_accepted(best):
                self._finalize_solve(
                    experiment_dir,
                    state,
                    branches,
                    best,
                    skip_auto_promote=skip_auto_promote,
                )
                return experiment_dir

            initial_branches = [b for b in branches if b.get("phase") == "initial"]
            if (
                stop_all_compile
                and initial_branches
                and all(
                    str(b.get("status") or "").upper() == "COMPILE_ERROR"
                    for b in initial_branches
                )
            ):
                state["reason"] = "all_initial_compile_error"
                self._finalize_solve(
                    experiment_dir,
                    state,
                    branches,
                    best,
                    skip_auto_promote=skip_auto_promote,
                )
                return experiment_dir

            # --- adaptive repair loop ---
            while True:
                best = self._best_solve_record(state["records"])
                if best is None:
                    state["status"] = "complete"
                    state["reason"] = "no_viable_candidates"
                    persist()
                    return experiment_dir
                if self._record_is_symbol_accepted(best):
                    break

                evaluation = best.get("evaluation") or {}
                status = str(evaluation.get("status", "")).upper()
                metrics = evaluation.get("metrics") or {}
                fingerprint = str(metrics.get("mismatch_fingerprint") or "")

                if status == "COMPILE_ERROR":
                    if compile_used >= compile_budget:
                        state["reason"] = "compile_repair_budget_exhausted"
                        break
                    phase = "compile_repair"
                    compile_used += 1
                else:
                    # Do not burn match_repairs when blocked on unvalidated callees.
                    if self._blocked_unvalidated_callee(evaluation):
                        state["reason"] = "blocked_unvalidated_callee"
                        break
                    if match_used >= match_budget:
                        state["reason"] = "match_repair_budget_exhausted"
                        break
                    phase = "match_repair"
                    match_used += 1

                parent_artifact = self.root / best["artifact"]
                parent_payload = json.loads(parent_artifact.read_text(encoding="utf-8"))
                parent_candidate = Candidate(**parent_payload["candidate"])
                parent_branch = next(
                    (b for b in branches if b.get("artifact") == best.get("artifact")),
                    {"branch_id": f"parent-{best.get('run_index')}"},
                )
                branch_id = f"{phase}-{run_index + 1}"
                iteration = int(parent_branch.get("iteration", 0)) + 1

                repair_context = {
                    "source": parent_candidate.source,
                    "evaluation": evaluation,
                    "binary_feedback": metrics.get("binary_feedback"),
                    "hypothesis": parent_candidate.hypothesis,
                    "next_change": parent_candidate.next_change,
                    "artifact": best.get("artifact"),
                    "rejected_fingerprints": [
                        b.get("fingerprint")
                        for b in branches
                        if b.get("fingerprint") and b.get("fingerprint") != fingerprint
                    ][-8:],
                }

                if phase == "compile_repair":
                    raw_detail = str(evaluation.get("detail", ""))
                    diagnostics = normalize_compile_output(raw_detail)
                    cleaned_detail = clean_mwcc_diagnostics(raw_detail) or raw_detail
                    retail_asm = ""
                    getter = getattr(self.adapter, "get_retail_asm", None)
                    if callable(getter):
                        try:
                            with self._adapter_lock:
                                retail_asm = str(getter(target_id) or "")
                        except Exception:
                            retail_asm = ""
                    prompt = self._build_repair_prompt(
                        parent_candidate,
                        cleaned_detail,
                        diagnostics,
                        state,
                        budget=compile_budget,
                        repair_index=compile_used - 1,
                        seen_fingerprints=list(fingerprint_counts),
                        retail_asm=retail_asm,
                        local_mode=local_mode,
                    )
                else:
                    with self._adapter_lock:
                        history = self.records(target_id=target_id)
                        prompt = self.adapter.build_prompt(
                            "improve",
                            target_id,
                            history,
                            {"repair_context": repair_context},
                        )

                run_index += 1
                run_context = self._write_solve_context(
                    experiment_dir, prompt, suffix=f"run-{run_index}"
                )
                record = self._run_one(
                    "improve" if phase == "match_repair" else "new",
                    target_id,
                    prompt,
                    experiment_id,
                    experiment_dir,
                    repair_model,
                    run_index,
                    context_dir=run_context,
                    workspace=solve_workspace,
                )
                # Empty/error provider responses still count as model_calls but must
                # not consume repair budget or become the best candidate.
                if self._is_empty_provider_error(record.error):
                    if phase == "compile_repair":
                        compile_used = max(0, compile_used - 1)
                    else:
                        match_used = max(0, match_used - 1)
                child_source = ""
                if record.artifact:
                    try:
                        child_payload = json.loads(
                            (self.root / record.artifact).read_text(encoding="utf-8")
                        )
                        child_source = str(
                            ((child_payload.get("candidate") or {}).get("source")) or ""
                        )
                    except (OSError, json.JSONDecodeError, TypeError):
                        child_source = ""
                noop_repair = (
                    reject_noop
                    and phase == "compile_repair"
                    and not record.error
                    and bool(child_source)
                    and normalize_source_for_compare(child_source)
                    == normalize_source_for_compare(parent_candidate.source)
                )
                if noop_repair:
                    # Refund budget; do not keep a duplicate branch as best.
                    compile_used = max(0, compile_used - 1)
                    record.candidate_summary = {
                        **(record.candidate_summary or {}),
                        "strategy": phase,
                        "parent_id": parent_branch.get("branch_id"),
                        "branch_id": branch_id,
                        "iteration": iteration,
                        "noop_repair": True,
                    }
                    state["records"].append(record.to_json())
                    branches.append({
                        "branch_id": branch_id,
                        "parent_id": parent_branch.get("branch_id"),
                        "phase": phase,
                        "strategy": phase,
                        "artifact": record.artifact,
                        "run_index": run_index,
                        "status": (record.evaluation or {}).get("status"),
                        "match_percent": (record.evaluation or {}).get("match_percent"),
                        "fingerprint": fingerprint,
                        "iteration": iteration,
                        "error": record.error,
                        "noop_repair": True,
                    })
                    fingerprint_counts[fingerprint or "noop"] = (
                        fingerprint_counts.get(fingerprint or "noop", 0) + 1
                    )
                    if fingerprint_counts[fingerprint or "noop"] >= max_repeated:
                        state["reason"] = "noop_compile_repair"
                        persist()
                        break
                    persist()
                    continue

                child_fp = ((record.evaluation or {}).get("metrics") or {}).get(
                    "mismatch_fingerprint", ""
                )
                record.candidate_summary = {
                    **(record.candidate_summary or {}),
                    "strategy": phase,
                    "parent_id": parent_branch.get("branch_id"),
                    "branch_id": branch_id,
                    "iteration": iteration,
                }
                state["records"].append(record.to_json())
                branches.append({
                    "branch_id": branch_id,
                    "parent_id": parent_branch.get("branch_id"),
                    "phase": phase,
                    "strategy": phase,
                    "artifact": record.artifact,
                    "run_index": run_index,
                    "status": (record.evaluation or {}).get("status"),
                    "match_percent": (record.evaluation or {}).get("match_percent"),
                    "fingerprint": child_fp,
                    "iteration": iteration,
                    "error": record.error,
                })

                # Best-so-far is preserved via ranking; stop on repeated mismatch state.
                if (
                    not record.error
                    and child_fp
                    and child_fp == fingerprint
                ):
                    fingerprint_counts[child_fp] = fingerprint_counts.get(child_fp, 0) + 1
                    if fingerprint_counts[child_fp] >= max_repeated:
                        state["reason"] = "repeated_fingerprint"
                        persist()
                        break

                persist()
                if self._record_is_symbol_accepted(record.to_json()):
                    best = record.to_json()
                    break

            best = self._best_solve_record(state["records"])
            self._finalize_solve(
                experiment_dir,
                state,
                branches,
                best,
                skip_auto_promote=skip_auto_promote,
            )
            return experiment_dir
        finally:
            setattr(self.adapter, "local_sanitize", prev_local)
            if solve_workspace is not None and self.workspace_manager is not None:
                self.workspace_manager.remove(solve_workspace)

    @staticmethod
    def _blocked_unvalidated_callee(evaluation: Dict[str, Any]) -> bool:
        """True when match repair cannot proceed due to unvalidated callees."""
        blocked = evaluation.get("blocked_reason")
        if not blocked:
            metrics = evaluation.get("metrics") or {}
            blocked = metrics.get("blocked_reason")
        if blocked == "unvalidated_callee":
            return True
        equiv = str(
            evaluation.get("equivalence")
            or evaluation.get("equivalence_status")
            or ""
        ).upper()
        return "INCONCLUSIVE_UNVALIDATED_CALLEE" in equiv

    @staticmethod
    def _record_is_symbol_accepted(record: Dict[str, Any]) -> bool:
        evaluation = record.get("evaluation") or {}
        if record.get("error"):
            return False
        if evaluation.get("symbol_accepted"):
            return True
        metrics = evaluation.get("metrics") or {}
        if metrics.get("symbol_accepted"):
            return True
        status = str(evaluation.get("status", "")).upper()
        if status == "FULL_MATCH" and bool(evaluation.get("accepted")):
            return True
        if status == "EQUIVALENT_MATCH" and bool(evaluation.get("accepted")):
            return True
        return False

    def _best_solve_record(
        self, records: List[Dict[str, Any]]
    ) -> Optional[Dict[str, Any]]:
        successful = [row for row in records if not row.get("error") and row.get("evaluation")]
        if not successful:
            return None
        return max(successful, key=lambda row: self._rank_record("solve", row))

    def _finalize_solve(
        self,
        experiment_dir: Path,
        state: Dict[str, Any],
        branches: List[Dict[str, Any]],
        best: Optional[Dict[str, Any]],
        *,
        skip_auto_promote: bool = False,
    ) -> None:
        state["branches"] = branches
        state["model_calls"] = len(state.get("records", []))
        state["status"] = "complete"
        blocked_reason: Optional[str] = None
        if best is not None:
            best = dict(best)
            evaluation = best.get("evaluation") or {}
            blocked_reason = evaluation.get("blocked_reason")
            if not blocked_reason:
                blocked_reason = (evaluation.get("metrics") or {}).get("blocked_reason")
            if not state.get("reason") and self._blocked_unvalidated_callee(evaluation):
                state["reason"] = "blocked_unvalidated_callee"
            updated = []
            for row in state.get("records", []):
                copy = dict(row)
                copy["winner"] = copy.get("artifact") == best.get("artifact")
                updated.append(copy)
            state["records"] = updated
            artifact_path = self.root / best["artifact"]
            if artifact_path.is_file():
                (experiment_dir / "best.json").write_text(
                    artifact_path.read_text(encoding="utf-8"), encoding="utf-8"
                )
            logged = set(state.get("logged_keys", []))
            for row in updated:
                key = _record_key(row)
                if key in logged:
                    continue
                payload = {
                    "schema_version": int(row.get("schema_version", 4)),
                    "experiment_id": row.get("experiment_id", state["experiment_id"]),
                    "timestamp": row.get(
                        "timestamp", datetime.now(timezone.utc).isoformat()
                    ),
                    "workflow": "solve",
                    "target_id": state["target_id"],
                    "model_id": row.get("model_id", ""),
                    "model": row.get("model", ""),
                    "run_index": int(row.get("run_index", 0)),
                    "duration_seconds": float(row.get("duration_seconds") or 0.0),
                    "input_tokens": row.get("input_tokens"),
                    "output_tokens": row.get("output_tokens"),
                    "cost": row.get("cost"),
                    "evaluation": row.get("evaluation") or {},
                    "artifact": row.get("artifact", ""),
                    "candidate_summary": row.get("candidate_summary") or {},
                    "winner": bool(row.get("winner")),
                    "error": row.get("error"),
                }
                self._append_record(ExperimentRecord(**payload))
                logged.add(key)
            state["logged_keys"] = sorted(logged)
            state["logged"] = True
        if blocked_reason:
            state["blocked_reason"] = blocked_reason
        # METRICS: prefer live phase hooks; fall back to record duration_seconds as llm
        live = self.timings.summary()
        if live.get("total_seconds", 0) > 0:
            state["timings"] = live
        else:
            state["timings"] = timings_from_records(list(state.get("records") or []))
        self._write_state(experiment_dir, state)
        if not skip_auto_promote:
            self._maybe_auto_promote(experiment_dir)
        reason = state.get("reason", "done")
        blocked_part = f" blocked_reason={blocked_reason}" if blocked_reason else ""
        self._debug(
            f"solve completed target={state.get('target_id')} "
            f"experiment={state.get('experiment_id')} "
            f"calls={state.get('model_calls')} reason={reason}{blocked_part}"
        )

    def records(self, target_id: Optional[str] = None) -> List[Dict[str, Any]]:
        if not self.log_path.is_file():
            return []
        with self._log_lock:
            text = self.log_path.read_text(encoding="utf-8")
        rows = [json.loads(line) for line in text.splitlines() if line]
        latest: Dict[tuple[Any, ...], Dict[str, Any]] = {}
        for offset, row in enumerate(rows):
            key = (
                row.get("experiment_id", f"legacy-{offset}"),
                row.get("model_id"),
                row.get("run_index"),
            )
            latest[key] = row
        return [r for r in latest.values() if target_id is None or r.get("target_id") == target_id]

    def _append_record(self, record: ExperimentRecord) -> None:
        self.log_path.parent.mkdir(parents=True, exist_ok=True)
        with self._log_lock:
            with self.log_path.open("a", encoding="utf-8") as f:
                f.write(json.dumps(record.to_json(), separators=(",", ":")) + "\n")

    def _append_io_log(
        self,
        prompt: str,
        response: str,
        record: ExperimentRecord,
    ) -> None:
        """Log full input/output for one provider call to io.jsonl in main output dir."""
        io_path = self.output_dir / "io.jsonl"
        io_path.parent.mkdir(parents=True, exist_ok=True)
        entry = {
            "timestamp": record.timestamp,
            "experiment_id": record.experiment_id,
            "target_id": record.target_id,
            "workflow": record.workflow,
            "model_id": record.model_id,
            "model": record.model,
            "run_index": record.run_index,
            "duration_seconds": record.duration_seconds,
            "input_tokens": record.input_tokens,
            "output_tokens": record.output_tokens,
            "cache_read_tokens": record.cache_read_tokens,
            "cache_write_tokens": record.cache_write_tokens,
            "cost": record.cost,
            "evaluation_status": (record.evaluation or {}).get("status"),
            "match_percent": (record.evaluation or {}).get("match_percent"),
            "accepted": (record.evaluation or {}).get("accepted"),
            "error": record.error,
            "prompt": prompt,
            "response": response,
        }
        with self._log_lock:
            with io_path.open("a", encoding="utf-8") as f:
                f.write(json.dumps(entry, separators=(",", ":")) + "\n")

    def stats(self) -> List[Dict[str, Any]]:
        groups: Dict[str, List[Dict[str, Any]]] = {}
        for row in self.records():
            groups.setdefault(row["model_id"], []).append(row)
        output = []
        for model_id, rows in sorted(groups.items()):
            completed = [r for r in rows if not r.get("error")]
            measured = [r for r in completed if not r.get("timed_out")]
            matches = [r["evaluation"].get("match_percent") for r in measured]
            matches = [float(v) for v in matches if v is not None]
            by_status: Dict[str, int] = {}
            for row in rows:
                status = _stats_status(row)
                by_status[status] = by_status.get(status, 0) + 1
            output.append({
                "model_id": model_id,
                "attempts": len(rows),
                "completed": len(completed),
                "wins": sum(bool(r.get("winner")) for r in rows),
                "accepted_wins": sum(
                    bool(r.get("winner")) and bool(r.get("evaluation", {}).get("accepted"))
                    for r in rows
                ),
                "accepted": sum(bool(r.get("evaluation", {}).get("accepted")) for r in completed),
                "by_status": dict(sorted(by_status.items())),
                "average_match_percent": sum(matches) / len(matches) if matches else None,
                "average_seconds": (
                    sum(float(r.get("duration_seconds", 0)) for r in measured) / len(measured)
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
        return output

    def build_external_prompt(
        self,
        workflow: str,
        target_id: str,
        *,
        repair_context: Optional[Dict[str, Any]] = None,
    ) -> str:
        """Build a function prompt for an external (non-provider) workflow."""
        options: Optional[Dict[str, Any]] = None
        if repair_context is not None:
            # Adapter expects repair_context.source for current-body override.
            normalized = dict(repair_context)
            if "source" not in normalized and normalized.get("candidate_source"):
                normalized["source"] = normalized["candidate_source"]
            options = {"repair_context": normalized}
        with self._adapter_lock:
            history = self.records(target_id=target_id)
            return self.adapter.build_prompt(workflow, target_id, history, options)

    def evaluate_external_candidate(
        self,
        target_id: str,
        source: str,
        *,
        hypothesis: str = "",
        workspace_root: Optional[Path] = None,
    ) -> Any:
        """Evaluate a candidate through the same isolated path as harness experiments."""
        candidate = Candidate(source=source, hypothesis=hypothesis or "external candidate")
        # Prefer improve workflow for evaluation; adapter function path is shared.
        workflow = "improve"
        label = f"web-{target_id}"
        if workspace_root is not None:
            return self._evaluate_in_workspace(workspace_root, workflow, target_id, candidate)
        return self._evaluate_candidate(workflow, target_id, candidate, label)

    def promote_external_accepted(
        self,
        target_id: str,
        source: str,
        evaluation: Any,
        *,
        owner: str,
        batch_id: str = "",
        batch_round: int = 0,
        dry_run: bool = False,
    ) -> PromotionResult:
        """Promote an externally supplied FULL_MATCH / certified EQUIVALENT_MATCH.

        Claims the target for ``owner`` only at promote time (via
        ``ensure_auto_promote_claim`` when available), then writes through the
        same auto-promote path used by normal harness winners.
        """
        from .types import PromotionResult as PR

        evaluation_dict: Dict[str, Any]
        if isinstance(evaluation, dict):
            evaluation_dict = dict(evaluation)
        elif hasattr(evaluation, "__dataclass_fields__"):
            evaluation_dict = asdict(evaluation)
        else:
            evaluation_dict = {
                "status": getattr(evaluation, "status", ""),
                "match_percent": getattr(evaluation, "match_percent", 0.0),
                "accepted": getattr(evaluation, "accepted", False),
                "symbol_accepted": getattr(evaluation, "symbol_accepted", False),
                "equivalence": getattr(evaluation, "equivalence", None),
                "detail": getattr(evaluation, "detail", ""),
                "metrics": getattr(evaluation, "metrics", {}) or {},
            }

        if not self._record_is_symbol_accepted(
            {"evaluation": evaluation_dict, "error": None}
        ):
            return PR(
                promoted=False,
                rolled_back=False,
                reason="candidate is not symbol-accepted (FULL_MATCH or certified EQUIVALENT_MATCH)",
                target_id=target_id,
            )

        if dry_run:
            return PR(
                promoted=False,
                rolled_back=False,
                reason="dry-run",
                target_id=target_id,
            )

        experiment_id = f"web-{batch_id or 'batch'}-r{batch_round}"
        experiment_dir = self.output_dir / target_id / experiment_id
        experiment_dir.mkdir(parents=True, exist_ok=True)
        status = str(evaluation_dict.get("status") or "").upper()
        workflow = "improve"
        state = {
            "schema_version": 2,
            "experiment_id": experiment_id,
            "workflow": workflow,
            "target_id": target_id,
            "source": "chatgpt-web",
            "batch_id": batch_id,
            "batch_round": batch_round,
            "status": "complete",
            "records": [],
        }
        candidate = Candidate(
            source=source,
            hypothesis=f"web-batch {batch_id} round {batch_round}",
        )
        best = {
            "candidate": asdict(candidate),
            "evaluation": evaluation_dict,
            "winner": True,
        }
        (experiment_dir / "prompt.md").write_text(
            f"# External web-batch candidate\n\nbatch={batch_id} round={batch_round}\n",
            encoding="utf-8",
        )
        (experiment_dir / "candidate.json").write_text(
            json.dumps(asdict(candidate), indent=2) + "\n", encoding="utf-8"
        )
        (experiment_dir / "evaluation.json").write_text(
            json.dumps(evaluation_dict, indent=2, default=str) + "\n", encoding="utf-8"
        )
        self._write_state(experiment_dir, state)
        (experiment_dir / "best.json").write_text(
            json.dumps(best, indent=2, default=str) + "\n", encoding="utf-8"
        )

        promote_fn = getattr(self.adapter, "promote", None)
        if promote_fn is None:
            return PR(
                promoted=False,
                rolled_back=False,
                reason="adapter does not support promote",
                target_id=target_id,
            )

        try:
            with self._promote_lock:
                ensure = getattr(self.adapter, "ensure_auto_promote_claim", None)
                if ensure is not None:
                    ensure(target_id, owner)
                read_source = getattr(self.adapter, "read_target_source", None)
                source_path_fn = getattr(self.adapter, "target_source_path", None)
                source_before = ""
                source_path: Optional[Path] = None
                original_bytes: Optional[bytes] = None
                if read_source is not None:
                    source_before = read_source(target_id)
                if source_path_fn is not None:
                    try:
                        source_path = source_path_fn(target_id)
                        if source_path is not None and source_path.is_file():
                            original_bytes = source_path.read_bytes()
                    except Exception:
                        source_path = None
                result = promote_fn(
                    workflow,
                    target_id,
                    candidate,
                    write=True,
                    owner=owner,
                )
                try:
                    certify = getattr(self.adapter, "certify_web_promotion", None)
                    if certify is not None:
                        certify(target_id, evaluation_dict)
                    else:
                        record = getattr(self.adapter, "record_auto_promotion", None)
                        if record is not None:
                            record(target_id, evaluation_dict)
                        cycle = getattr(self.adapter, "run_auto_promote_cycle", None)
                        if cycle is not None:
                            cycle(target_id, evaluation_dict)
                except Exception as cert_exc:
                    # Roll back canonical source if certification fails after write.
                    if original_bytes is not None and source_path is not None:
                        source_path.write_bytes(original_bytes)
                    elif read_source is not None and source_before and source_path is not None:
                        source_path.write_text(source_before, encoding="utf-8")
                    raise RuntimeError(
                        f"promotion certification failed after write; rolled back: {cert_exc}"
                    ) from cert_exc
            state["auto_promoted"] = True
            state["auto_promote_owner"] = owner
            self._write_state(experiment_dir, state)
            return PR(
                promoted=True,
                rolled_back=False,
                reason=f"promoted status={status}",
                target_id=target_id,
                validation_steps=[],
            )
        except Exception as exc:
            state["auto_promoted"] = False
            state["auto_promote_error"] = f"{type(exc).__name__}: {exc}"
            try:
                self._write_state(experiment_dir, state)
            except OSError:
                pass
            rolled_back = "rolled back" in str(exc).lower()
            return PR(
                promoted=False,
                rolled_back=rolled_back,
                reason=f"{type(exc).__name__}: {exc}",
                target_id=target_id,
            )

    # Back-compat alias from the design doc (FULL_MATCH name; accepts EQUIVALENT too).
    def promote_external_full_match(
        self,
        target_id: str,
        source: str,
        evaluation: Any,
        *,
        owner: str,
        dry_run: bool = False,
    ) -> PromotionResult:
        return self.promote_external_accepted(
            target_id,
            source,
            evaluation,
            owner=owner,
            dry_run=dry_run,
        )

    def promote(self, experiment_dir: Path, *, write: bool = False, owner: Optional[str] = None) -> str:
        directory = experiment_dir.resolve()
        state = json.loads((directory / "state.json").read_text(encoding="utf-8"))
        best = json.loads((directory / "best.json").read_text(encoding="utf-8"))
        candidate = Candidate(**best["candidate"])
        promote_fn = getattr(self.adapter, "promote", None)
        if promote_fn is None:
            raise ValueError("Configured project adapter does not support promotion")
        
        # Phase 0: require owner for write promotion
        if write and not owner:
            raise ValueError("promote --write requires --owner <owner>")
        
        return promote_fn(
            state["workflow"], state["target_id"], candidate, write=write, owner=owner
        )

    def _maybe_auto_promote(self, experiment_dir: Path) -> Optional[str]:
        """Write FULL_MATCH / EQUIVALENT_MATCH winners into canonical source."""
        if not self.auto_promote:
            return None
        directory = experiment_dir.resolve()
        best_path = directory / "best.json"
        state_path = directory / "state.json"
        if not best_path.is_file() or not state_path.is_file():
            return None
        try:
            state = json.loads(state_path.read_text(encoding="utf-8"))
            best = json.loads(best_path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as exc:
            self._debug(f"auto-promote skipped: cannot read experiment ({exc})")
            return None
        evaluation = best.get("evaluation") or {}
        if not self._record_is_symbol_accepted({"evaluation": evaluation, "error": None}):
            return None
        if not isinstance(best.get("candidate"), dict):
            return None
        target_id = str(state.get("target_id") or "")
        workflow = str(state.get("workflow") or "new")
        if not target_id:
            return None
        promote_fn = getattr(self.adapter, "promote", None)
        if promote_fn is None:
            self._debug(f"auto-promote skipped target={target_id}: adapter has no promote")
            return None
        owner = self.auto_promote_owner
        candidate = Candidate(**best["candidate"])
        try:
            with self._promote_lock:
                ensure = getattr(self.adapter, "ensure_auto_promote_claim", None)
                if ensure is not None:
                    ensure(target_id, owner)
                # solve uses the function promote path (same as new/improve).
                promote_workflow = "new" if workflow == "solve" else workflow
                result = promote_fn(
                    promote_workflow,
                    target_id,
                    candidate,
                    write=True,
                    owner=owner,
                )
                record = getattr(self.adapter, "record_auto_promotion", None)
                if record is not None:
                    record(target_id, evaluation)
                cycle = getattr(self.adapter, "run_auto_promote_cycle", None)
                if cycle is not None:
                    cycle(target_id, evaluation)
            state["auto_promoted"] = True
            state["auto_promote_owner"] = owner
            self._write_state(directory, state)
            self._debug(
                f"auto-promoted target={target_id} status={evaluation.get('status')} "
                f"owner={owner}"
            )
            return str(result)
        except Exception as exc:
            self._debug(
                f"auto-promote failed target={target_id}: "
                f"{type(exc).__name__}: {exc}"
            )
            state["auto_promoted"] = False
            state["auto_promote_error"] = f"{type(exc).__name__}: {exc}"
            try:
                self._write_state(directory, state)
            except OSError:
                pass
            return None

    def promote_accepted(self, *, dry_run: bool = False) -> List[Dict[str, Any]]:
        """Promote every saved FULL_MATCH / EQUIVALENT_MATCH winner not yet in registry."""
        rows = self.records()
        winners: Dict[str, Dict[str, Any]] = {}
        for row in rows:
            if not row.get("winner"):
                continue
            if not self._record_is_symbol_accepted(row):
                continue
            target_id = str(row.get("target_id") or "")
            if not target_id:
                continue
            prev = winners.get(target_id)
            if prev is None or str(row.get("timestamp") or "") >= str(prev.get("timestamp") or ""):
                winners[target_id] = row

        already_accepted = set()
        status_fn = getattr(self.adapter, "accepted_target_ids", None)
        if status_fn is not None:
            already_accepted = set(status_fn())

        results: List[Dict[str, Any]] = []
        previous = self.auto_promote
        try:
            # Force promote path even if config disabled it for this backfill command.
            self.auto_promote = True
            for target_id, row in sorted(winners.items()):
                artifact = str(row.get("artifact") or "")
                if artifact:
                    experiment_dir = (self.root / artifact).resolve().parent
                else:
                    experiment_dir = (
                        self.output_dir / target_id / str(row.get("experiment_id") or "")
                    ).resolve()
                status = str((row.get("evaluation") or {}).get("status") or "")
                entry: Dict[str, Any] = {
                    "target_id": target_id,
                    "experiment_dir": str(experiment_dir),
                    "status": status,
                }
                try:
                    entry["experiment_dir"] = str(experiment_dir.relative_to(self.root))
                except ValueError:
                    pass
                if target_id in already_accepted:
                    entry["action"] = "skipped_already_accepted"
                    results.append(entry)
                    continue
                if not (experiment_dir / "best.json").is_file():
                    entry["action"] = "skipped_missing_best"
                    results.append(entry)
                    continue
                if dry_run:
                    entry["action"] = "would_promote"
                    results.append(entry)
                    continue
                promoted = self._maybe_auto_promote(experiment_dir)
                entry["action"] = "promoted" if promoted is not None else "failed"
                if promoted is not None:
                    entry["detail"] = promoted[:500]
                    already_accepted.add(target_id)
                else:
                    state_path = experiment_dir / "state.json"
                    if state_path.is_file():
                        try:
                            state = json.loads(state_path.read_text(encoding="utf-8"))
                            if state.get("auto_promote_error"):
                                entry["error"] = state["auto_promote_error"]
                        except (OSError, json.JSONDecodeError):
                            pass
                results.append(entry)
        finally:
            self.auto_promote = previous
        return results

    def rescore(self, experiment_dir: Path, *, max_parallel: Optional[int] = None) -> Path:
        directory = experiment_dir.resolve()
        state = json.loads((directory / "state.json").read_text(encoding="utf-8"))
        rows = state.get("records", [])
        candidates = []
        for row in rows:
            artifact = self.root / row["artifact"]
            payload = json.loads(artifact.read_text(encoding="utf-8"))
            if isinstance(payload.get("candidate"), dict):
                candidates.append((row, artifact, Candidate(**payload["candidate"])))
        parallel = max_parallel if max_parallel is not None else self.max_parallel

        def score(item):
            row, artifact, candidate = item
            evaluation = asdict(self._evaluate_candidate(
                state["workflow"], state["target_id"], candidate,
                f"rescore-{row['model_id']}-{row['run_index']}",
            ))
            artifact.write_text(
                json.dumps({"candidate": asdict(candidate), "evaluation": evaluation}, indent=2) + "\n",
                encoding="utf-8",
            )
            _write_tu_decomp_sidecars(artifact, candidate)
            row["evaluation"] = evaluation
            row["error"] = None
            row["winner"] = False
            row["timestamp"] = datetime.now(timezone.utc).isoformat()
            return row

        if parallel > 1:
            with ThreadPoolExecutor(max_workers=parallel) as executor:
                updated = list(executor.map(score, candidates))
        else:
            updated = [score(item) for item in candidates]
        by_key = {(row["model_id"], row["run_index"]): row for row in updated}
        state["records"] = [by_key.get((row["model_id"], row["run_index"]), row) for row in rows]
        records = [ExperimentRecord(**row) for row in state["records"]]
        successful = [record for record in records if not record.error]
        if successful:
            best = max(successful, key=lambda record: self._rank_record(state["workflow"], record))
            best.winner = True
            best_text = (self.root / best.artifact).read_text(encoding="utf-8")
            (directory / "best.json").write_text(best_text, encoding="utf-8")
            best_payload = json.loads(best_text)
            if isinstance(best_payload.get("candidate"), dict):
                _write_tu_decomp_sidecars(
                    directory / "best.json", Candidate(**best_payload["candidate"])
                )
        state["records"] = [record.to_json() for record in records]
        state["status"] = "complete"
        state["rescored_at"] = datetime.now(timezone.utc).isoformat()
        self._write_state(directory, state)
        for record in records:
            self._append_record(record)
        return directory

    def _rank_record(self, workflow: str, record: Any) -> tuple[Any, ...]:
        if isinstance(record, ExperimentRecord):
            evaluation = record.evaluation or {}
            as_dict = record.to_json()
        else:
            evaluation = (record or {}).get("evaluation") or {}
            as_dict = record or {}
        rank_fn = getattr(self.adapter, "rank_candidate", None)
        if rank_fn is not None:
            return tuple(rank_fn(workflow, evaluation))
        return (
            1 if self._record_is_symbol_accepted(as_dict) else 0,
            float(evaluation.get("match_percent") or 0.0),
        )

    def _load_best_candidate_context(
        self, target_id: str, root: Path
    ) -> Optional[Dict[str, Any]]:
        """Load the best prior candidate artifact for repair context."""
        records = self.records(target_id=target_id)
        if not records:
            return None
        
        # Filter records that have artifacts and successful evaluations
        valid_records = []
        for row in records:
            if row.get("error"):
                continue
            artifact_path = root / row["artifact"]
            if not artifact_path.is_file():
                continue
            try:
                artifact = json.loads(artifact_path.read_text(encoding="utf-8"))
                if not isinstance(artifact.get("candidate"), dict):
                    continue
                valid_records.append((row, artifact))
            except (OSError, json.JSONDecodeError):
                continue
        
        if not valid_records:
            return None
        
        # Rank by accepted status then match percentage
        def rank_key(item):
            row, artifact = item
            ev = row.get("evaluation") or {}
            return (bool(ev.get("accepted")), float(ev.get("match_percent") or 0.0))
        
        best_row, best_artifact = max(valid_records, key=rank_key)
        
        candidate = best_artifact.get("candidate", {})
        evaluation = best_row.get("evaluation", {})
        metrics = evaluation.get("metrics", {})
        binary_feedback = metrics.get("binary_feedback")
        
        return {
            "source": candidate.get("source", ""),
            "evaluation": evaluation,
            "binary_feedback": binary_feedback,
            "hypothesis": candidate.get("hypothesis", ""),
            "next_change": candidate.get("next_change", ""),
            "artifact": str(best_row["artifact"]),
        }

    def repair(
        self,
        experiment_dir: Path,
        *,
        budget: int = 3,
        dry_run: bool = False,
    ) -> Path:
        directory = experiment_dir.resolve()
        state = json.loads((directory / "state.json").read_text(encoding="utf-8"))
        best = json.loads((directory / "best.json").read_text(encoding="utf-8"))
        candidate = Candidate(**best["candidate"])
        evaluation = best.get("evaluation", {})

        status = evaluation.get("status", "")
        if status != "COMPILE_ERROR":
            self._debug(
                f"repair skipped target={state.get('target_id')}: "
                f"status={status} (expected COMPILE_ERROR)"
            )
            return directory

        compile_detail = str(evaluation.get("detail", ""))
        diagnostics = normalize_compile_output(compile_detail)
        root = select_root_diagnostic(diagnostics)
        root_fp = root.fingerprint if root else "unknown"
        self._debug(
            f"repair started target={state.get('target_id')} "
            f"experiment={state.get('experiment_id')} "
            f"root_diagnostic={root_fp} diagnostics={len(diagnostics)}"
        )

        if dry_run:
            return directory

        prompt = self._build_repair_prompt(
            candidate, compile_detail, diagnostics, state, budget=budget,
        )

        models = self.models_for_workflow(state.get("workflow", "new"))
        if not models:
            raise ValueError("No model configured for repair")

        model = models[0]
        provider = self.providers.get(model.provider)
        if provider is None:
            raise ValueError(f"Unsupported provider: {model.provider}")

        started = time.monotonic()
        repairs_dir = directory / "repairs"
        repairs_dir.mkdir(exist_ok=True)
        seen_fingerprints: set[str] = set()

        for repair_index in range(1, budget + 1):
            self._debug(
                f"repair iteration {repair_index}/{budget} "
                f"target={state.get('target_id')}"
            )

            try:
                result = provider.invoke(
                    prompt, model, directory / "context" if (directory / "context").is_dir() else directory
                )
            except Exception as exc:
                repair_artifact = repairs_dir / f"repair-{repair_index}.json"
                repair_artifact.write_text(
                    json.dumps({"error": str(exc), "repair_index": repair_index}) + "\n",
                    encoding="utf-8",
                )
                self._debug(
                    f"repair iteration {repair_index} failed: {exc}"
                )
                break

            response_text = result.text
            new_source = _extract_repair_source(response_text)
            if not new_source:
                repair_artifact = repairs_dir / f"repair-{repair_index}.json"
                repair_artifact.write_text(
                    json.dumps({
                        "repair_index": repair_index,
                        "error": "model returned empty source",
                        "model_response": response_text,
                    }) + "\n",
                    encoding="utf-8",
                )
                break

            new_candidate = Candidate(
                source=new_source,
                hypothesis=f"repair {repair_index}: {root_fp}",
                notes=[f"Compile repair iteration {repair_index}"],
            )

            try:
                ev = self.adapter.evaluate(
                    state.get("workflow", "new"),
                    state.get("target_id", ""),
                    new_candidate,
                )
            except Exception as exc:
                repair_artifact = repairs_dir / f"repair-{repair_index}.json"
                repair_artifact.write_text(
                    json.dumps({
                        "repair_index": repair_index,
                        "error": f"evaluation failed: {exc}",
                        "candidate": asdict(new_candidate),
                    }) + "\n",
                    encoding="utf-8",
                )
                break

            repair_artifact = repairs_dir / f"repair-{repair_index}.json"
            repair_artifact.write_text(
                json.dumps({
                    "repair_index": repair_index,
                    "candidate": asdict(new_candidate),
                    "evaluation": asdict(ev),
                }, indent=2) + "\n",
                encoding="utf-8",
            )

            ev_status = str(getattr(ev, "status", ev.get("status", "")))
            if ev_status != "COMPILE_ERROR":
                self._debug(
                    f"repair iteration {repair_index} succeeded: "
                    f"status={ev_status} match={getattr(ev, 'match_percent', ev.get('match_percent'))}%"
                )
                return directory

            # Parse new compile errors for next iteration
            new_detail = str(getattr(ev, "detail", ev.get("detail", "")))
            new_diagnostics = normalize_compile_output(new_detail)
            new_root = select_root_diagnostic(new_diagnostics)
            new_fp = new_root.fingerprint if new_root else "unknown"

            if new_fp in seen_fingerprints:
                self._debug(
                    f"repair iteration {repair_index} blocked: "
                    f"repeated diagnostic {new_fp}"
                )
                break
            seen_fingerprints.add(new_fp)

            # Build prompt for next iteration
            prompt = self._build_repair_prompt(
                new_candidate, new_detail, new_diagnostics, state,
                budget=budget,
                repair_index=repair_index,
                seen_fingerprints=list(seen_fingerprints),
            )

        self._debug(
            f"repair exhausted target={state.get('target_id')} "
            f"budget={budget} iterations={repair_index}"
        )
        return directory

    def _prompt_dir(self) -> Path:
        adapter_dir = getattr(self.adapter, "prompt_dir", None)
        if isinstance(adapter_dir, Path):
            return adapter_dir
        project = self.config.get("project", {}) or {}
        return self.root / project.get("prompt_dir", "tools/llm_harness/prompts")

    def _attempt_format_repair(
        self,
        raw_output: str,
        error_message: str,
        model: ModelConfig,
        provider: Any,
        context_dir: Optional[Path],
        *,
        workflow: str = "new",
        full_context: bool = False,
    ) -> Optional[Candidate]:
        repair_name = (
            "format_repair_tu_decomp.md"
            if workflow == "tu-decomp"
            else "format_repair.md"
        )
        repair_path = self._prompt_dir() / repair_name
        if not repair_path.is_file():
            self._debug(f"format-repair skipped: {repair_path} not found")
            return None
        template = repair_path.read_text(encoding="utf-8")
        lines = raw_output.split("\n")
        if len(lines) > 80:
            lines = lines[:80]
            lines.append("... (truncated)")
        truncated_raw = "\n".join(lines)
        prompt_text = template.replace("{{RAW_OUTPUT}}", truncated_raw).replace(
            "{{ERROR_MESSAGE}}", error_message
        )
        try:
            # Format repair must not burn tokens on thinking loops.
            repair_model = ModelConfig(
                id=model.id,
                provider=model.provider,
                model=model.model,
                runs=1,
                agent=model.agent,
                variant=model.variant,
                max_tokens=model.max_tokens,
                enable_thinking=False,
                thinking_budget=0,
                reasoning_effort="none",
            )
            result = provider.invoke(
                prompt_text, repair_model, context_dir or Path(self.adapter.root)
            )
            return parse_candidate(
                result.text, workflow=workflow, full_context=full_context
            )
        except Exception as exc:
            self._debug(f"format-repair failed: {exc}")
            return None

    def _build_repair_prompt(
        self,
        candidate: Candidate,
        compile_detail: str,
        diagnostics,
        state: Dict[str, Any],
        *,
        budget: int = 3,
        repair_index: int = 0,
        seen_fingerprints: list[str] | None = None,
        retail_asm: str = "",
        local_mode: bool = False,
    ) -> str:
        repair_name = "compile_repair_local.md" if local_mode else "compile_repair.md"
        repair_path = self._prompt_dir() / repair_name
        if local_mode and not repair_path.is_file():
            repair_path = self._prompt_dir() / "compile_repair.md"
        repair_instruction = (
            repair_path.read_text(encoding="utf-8") if repair_path.is_file() else ""
        )

        # compile_repair*.md is self-contained (same JSON schema as common.md).
        # Do not paste common.md here — its {{WORKFLOW_PROMPT}} placeholders
        # would ship unsubstituted and waste context.
        lines: list[str] = [
            repair_instruction,
            "",
            "---",
            "",
            "## Current candidate source",
            candidate.source,
            "",
            "## Compiler diagnostic report",
            compile_detail,
        ]

        if diagnostics:
            lines.append("")
            lines.append("## Normalized diagnostics")
            for i, d in enumerate(diagnostics):
                loc = f"{d.file}:{d.line}" if d.file else (str(d.line) if d.line else "?")
                symbol = f" symbol={d.symbol}" if d.symbol else ""
                lines.append(
                    f"  {i+1}. [{d.category}] {loc}{symbol} — {d.message}"
                )
            if local_mode:
                tips = cookbook_for_diagnostics(list(diagnostics))
                if tips:
                    lines.append("")
                    lines.append("## Fix cookbook")
                    for tip in tips:
                        lines.append(f"  - {tip}")

        if seen_fingerprints:
            lines.append("")
            lines.append("## Previously attempted repairs (fingerprints already seen)")
            for fp in seen_fingerprints:
                lines.append(f"  - {fp}")

        asm = (retail_asm or "").strip()
        if asm:
            # Keep ABI/signature check cheap — full dossiers belong on match repair.
            excerpt = asm if len(asm) <= 4000 else asm[:4000] + "\n... (truncated)"
            lines.append("")
            lines.append("## Retail ASM (signature/ABI check)")
            lines.append(excerpt)

        lines.append("")
        lines.append(f"## Repair iteration {repair_index + 1} of {budget}")
        return "\n".join(lines)


def _extract_repair_source(text: str) -> str:
    cleaned = text.strip()
    fenced = re.search(r"```(?:json)?\s*(\{.*\})\s*```", cleaned, re.DOTALL)
    if fenced:
        cleaned = fenced.group(1)
    else:
        start, end = cleaned.find("{"), cleaned.rfind("}")
        if start >= 0 and end > start:
            cleaned = cleaned[start : end + 1]
    try:
        data = json.loads(cleaned)
        src = data.get("source", "")
        if isinstance(src, str) and src.strip():
            return src
    except json.JSONDecodeError:
        try:
            decoder = json.JSONDecoder()
            data, _ = decoder.raw_decode(cleaned)
            src = data.get("source", "")
            if isinstance(src, str) and src.strip():
                return src
        except (json.JSONDecodeError, ValueError):
            pass
    except ValueError:
        pass
    return ""


def _try_json_parse(text: str) -> Optional[Dict[str, Any]]:
    """Parse JSON from text, trying raw_decode if strict parse fails."""
    decoder = json.JSONDecoder()
    try:
        return json.loads(text)
    except json.JSONDecodeError:
        pass
    start = text.find("{")
    if start < 0:
        return None
    try:
        data, end_pos = decoder.raw_decode(text, start)
        return data
    except (json.JSONDecodeError, ValueError, IndexError):
        pass
    return _salvage_candidate_dict(text)


def _unescape_json_string(value: str) -> str:
    """Best-effort unescape for a JSON string body (may include raw newlines)."""
    out: list[str] = []
    i = 0
    while i < len(value):
        ch = value[i]
        if ch == "\\" and i + 1 < len(value):
            nxt = value[i + 1]
            mapping = {"n": "\n", "r": "\r", "t": "\t", '"': '"', "\\": "\\", "/": "/"}
            if nxt in mapping:
                out.append(mapping[nxt])
                i += 2
                continue
            if nxt == "u" and i + 5 < len(value):
                hex_digits = value[i + 2 : i + 6]
                try:
                    out.append(chr(int(hex_digits, 16)))
                    i += 6
                    continue
                except ValueError:
                    pass
        out.append(ch)
        i += 1
    return "".join(out)


def _extract_json_string_field(text: str, field: str) -> Optional[str]:
    """Extract a JSON string field even when the value has raw newlines / truncation."""
    match = re.search(rf'"{re.escape(field)}"\s*:\s*"', text)
    if not match:
        return None
    i = match.end()
    raw_chars: list[str] = []
    while i < len(text):
        ch = text[i]
        if ch == "\\" and i + 1 < len(text):
            raw_chars.append(ch)
            raw_chars.append(text[i + 1])
            i += 2
            continue
        if ch == '"':
            # End of string if followed by JSON punctuation or EOF-ish noise.
            rest = text[i + 1 :].lstrip()
            if not rest or rest[0] in ",}":
                return _unescape_json_string("".join(raw_chars))
            # Otherwise treat as a literal quote inside a broken string.
        raw_chars.append(ch)
        i += 1
    # Truncated: accept whatever we captured.
    salvaged = _unescape_json_string("".join(raw_chars)).rstrip()
    if salvaged.endswith(",") or salvaged.endswith("{"):
        salvaged = salvaged.rstrip(",{ \t")
    return salvaged or None


def _salvage_candidate_dict(text: str) -> Optional[Dict[str, Any]]:
    """Recover candidate fields from broken/truncated model JSON."""
    source = _extract_json_string_field(text, "source")
    if source is None or not source.strip():
        return None
    data: Dict[str, Any] = {"source": source}
    for field in ("hypothesis", "next_change", "change"):
        value = _extract_json_string_field(text, field)
        if value is not None:
            data[field] = value
    notes_match = re.search(r'"notes"\s*:\s*(\[[^\]]*\])', text, re.DOTALL)
    if notes_match:
        try:
            notes = json.loads(notes_match.group(1))
            if isinstance(notes, list):
                data["notes"] = [str(item) for item in notes if isinstance(item, str)]
        except json.JSONDecodeError:
            pass
    return data


def _write_tu_decomp_sidecars(artifact: Path, candidate: Candidate) -> None:
    """Write readable phase1/phase2 sidecar files next to a candidate artifact."""
    if not candidate.phase1_header.strip() and not candidate.phase2_cpp.strip():
        return
    stem = artifact.with_suffix("")
    if candidate.phase1_header.strip():
        stem.with_name(stem.name + "-phase1.hpp").write_text(
            candidate.phase1_header.rstrip() + "\n", encoding="utf-8"
        )
    if candidate.phase2_cpp.strip():
        stem.with_name(stem.name + "-phase2.cpp").write_text(
            candidate.phase2_cpp.rstrip() + "\n", encoding="utf-8"
        )


def _looks_like_function_definition(source: str) -> bool:
    """True when source looks like a C/C++ function definition, not a bare name."""
    text = source.strip()
    if not text:
        return False
    # Body-only (scaffold mode): harness reattaches the locked signature.
    if text.startswith("{") and text.endswith("}"):
        return True
    if "(" not in text or "{" not in text:
        return False
    # Reject pure declarations / dossier fragments.
    if re.fullmatch(r"[A-Za-z_][\w:<>\s*&]*", text):
        return False
    if '"declaration"' in text:
        return False
    return True


def _parse_tu_decomp_sections(text: str) -> Optional[Dict[str, str]]:
    """Parse delimiter-based tu-decomp output into section bodies."""
    if "===PHASE2_CPP===" not in text and "===PHASE1_HEADER===" not in text:
        return None
    pattern = re.compile(
        r"^===(PHASE1_HEADER|PHASE2_CPP|HYPOTHESIS|NOTES|NEXT_CHANGE)===\s*\n?",
        re.MULTILINE,
    )
    matches = list(pattern.finditer(text))
    if not matches:
        return None
    sections: Dict[str, str] = {}
    for i, match in enumerate(matches):
        name = match.group(1)
        start = match.end()
        end = matches[i + 1].start() if i + 1 < len(matches) else len(text)
        sections[name] = text[start:end].strip("\n")
    return sections


def _candidate_from_tu_decomp_fields(
    *,
    phase1: str,
    phase2: str,
    hypothesis: str,
    notes: List[str],
    next_change: str,
    rejected_source: str = "",
) -> Candidate:
    if not isinstance(phase1, str):
        raise ValueError("TU-decomp output 'phase1_header' must be a string")
    if not isinstance(phase2, str):
        raise ValueError("TU-decomp output 'phase2_cpp' must be a string")
    if not phase2.strip():
        raise ValueError(
            "TU-decomp output requires non-empty 'phase2_cpp' "
            "(got single-function 'source' or empty body)"
            if rejected_source.strip()
            else "TU-decomp output requires non-empty 'phase2_cpp'"
        )
    notes = [str(note)[:120] for note in notes[:3]]
    return Candidate(
        source="",
        hypothesis=str(hypothesis)[:160],
        notes=notes,
        next_change=str(next_change)[:120],
        patches=[],
        phase1_header=phase1,
        phase2_cpp=phase2,
    )


def parse_candidate(
    text: str, *, workflow: str = "new", full_context: bool = False
) -> Candidate:
    raw_text = text.strip()

    if workflow == "tu-decomp":
        sections = _parse_tu_decomp_sections(raw_text)
        if sections is not None:
            notes_raw = sections.get("NOTES", "")
            notes = [
                line.lstrip("-* ").strip()
                for line in notes_raw.splitlines()
                if line.strip()
            ]
            return _candidate_from_tu_decomp_fields(
                phase1=sections.get("PHASE1_HEADER", ""),
                phase2=sections.get("PHASE2_CPP", ""),
                hypothesis=sections.get("HYPOTHESIS", "").strip(),
                notes=notes,
                next_change=sections.get("NEXT_CHANGE", "").strip(),
            )

    cleaned = raw_text
    fenced = re.search(r"```(?:json)?\s*(\{.*\})\s*```", cleaned, re.DOTALL)
    if fenced:
        cleaned = fenced.group(1)
    else:
        start, end = cleaned.find("{"), cleaned.rfind("}")
        if start >= 0 and end > start:
            cleaned = cleaned[start : end + 1]
        elif start >= 0:
            # Truncated object with no closing brace — keep from first '{'.
            cleaned = cleaned[start:]
    data = _try_json_parse(cleaned)
    if data is None and cleaned != raw_text:
        data = _try_json_parse(raw_text)
    if data is None:
        raise json.JSONDecodeError("No valid JSON found in model output", cleaned, 0)

    notes = data.get("notes", [])
    if isinstance(notes, str):
        notes = [notes]
    if not isinstance(notes, list) or not all(isinstance(v, str) for v in notes):
        raise ValueError("Model output 'notes' must be a list of strings")
    # Clamp metadata so verbose models cannot dominate the candidate record.
    notes = [str(note)[:120] for note in notes[:3]]
    hypothesis = str(data.get("hypothesis", ""))[:160]
    next_change = str(data.get("next_change", ""))[:120]

    if workflow == "tu-decomp":
        # Legacy JSON two-phase responses still accepted.
        return _candidate_from_tu_decomp_fields(
            phase1=data.get("phase1_header", ""),
            phase2=data.get("phase2_cpp", ""),
            hypothesis=hypothesis,
            notes=notes,
            next_change=next_change,
            rejected_source=str(data.get("source") or "")
            if isinstance(data.get("source"), str)
            else "",
        )

    source = data.get("source", "")
    if not isinstance(source, str):
        raise ValueError("Model output 'source' must be a string")
    # Reject dossier echoes / bare symbol names that are not definitions.
    if source.strip() and workflow != "tu-complete":
        if not _looks_like_function_definition(source):
            raise ValueError(
                "Model output 'source' must be a complete function definition "
                f"(got {source.strip()[:80]!r})"
            )
    patches_data = data.get("patches", [])
    if not isinstance(patches_data, list):
        raise ValueError("Model output 'patches' must be a list")
    patches: List[SourcePatch] = []
    for value in patches_data:
        if not isinstance(value, dict):
            raise ValueError("Each model output patch must be an object")
        slot_id = value.get("slot_id")
        patch_source = value.get("source")
        if not isinstance(slot_id, str) or not slot_id.strip():
            raise ValueError("Each model output patch requires non-empty string 'slot_id'")
        if not isinstance(patch_source, str) or not patch_source.strip():
            raise ValueError("Each model output patch requires non-empty string 'source'")
        patches.append(SourcePatch(slot_id=slot_id, source=patch_source))
    if source.strip() and patches:
        raise ValueError("Model output must use either 'source' or 'patches', not both")
    if workflow == "tu-complete":
        if not source.strip() and not patches:
            raise ValueError("TU model output requires non-empty 'patches' or fallback 'source'")
        if full_context and patches:
            raise ValueError("Full-context TU output must contain complete 'source', not patches")
        if not full_context and source.strip():
            raise ValueError("Targeted TU output must contain bounded 'patches', not full source")
    elif not source.strip():
        raise ValueError("Model output must contain non-empty string 'source'")
    return Candidate(
        source=source,
        hypothesis=hypothesis,
        notes=notes,
        next_change=next_change,
        patches=patches,
    )


def _record_key(row: Dict[str, Any]) -> str:
    return f"{row.get('model_id')}:{int(row.get('run_index', 0))}"


def _stats_status(row: Dict[str, Any]) -> str:
    """Normalize an experiment row into a stats status bucket."""
    evaluation = row.get("evaluation") or {}
    status = evaluation.get("status")
    if status:
        return str(status)
    if row.get("timed_out") or "TimeoutExpired" in str(row.get("error") or ""):
        return "TIMEOUT"
    if row.get("error"):
        return "ERROR"
    return "NONE"

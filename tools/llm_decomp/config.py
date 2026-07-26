"""Configuration for llm_decomp (llm-decomp.json at repo root).

Every setting is documented in docs/llm_decomp_design.md §9.
Precedence: session_types.<type> overrides top-level defaults.
"""
from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path

CONFIG_FILENAME = "llm-decomp.json"

DEFAULT_BUDGETS = {
    "match": {"max_turns": 25, "max_builds": 15, "max_sweeps": 5,
              "max_patch_failures": 4, "cleanup_turns": 5,
              "max_session_tokens": 400_000},
    "type-recovery": {"max_turns": 20, "max_builds": 8, "max_sweeps": 5,
                      "max_patch_failures": 4, "cleanup_turns": 0,
                      "max_session_tokens": 300_000},
    "rename": {"max_turns": 10, "max_builds": 6, "max_sweeps": 2,
               "max_patch_failures": 4, "cleanup_turns": 0,
               "max_session_tokens": 100_000},
    "tu-cleanup": {"max_turns": 20, "max_builds": 8, "max_sweeps": 5,
                   "max_patch_failures": 4, "cleanup_turns": 0,
                   "max_session_tokens": 300_000},
    "size-trim": {"max_turns": 20, "max_builds": 12, "max_sweeps": 5,
                  "max_patch_failures": 4, "cleanup_turns": 0,
                  "max_session_tokens": 300_000},
}


@dataclass
class ModelConfig:
    provider: str = "openrouter"
    model: str = ""
    max_tokens: int | None = 8192  # None = unlimited
    thinking_budget: int | None = None  # None = provider default; -1 = unlimited


@dataclass
class SessionTypeConfig:
    model: ModelConfig  # resolved (default merged with per-type override)
    budgets: dict


@dataclass
class Config:
    default_model: ModelConfig = field(default_factory=ModelConfig)
    session_types: dict[str, SessionTypeConfig] = field(default_factory=dict)
    workers: int = 8
    target_parallel: int = 4
    prompt_max_chars: int = 60_000
    prompt_include_raw_hex: bool = False
    equivalence_min_fuzzy: float = 50.0
    cross_tu_full_sweep_max: int = 10
    cross_tu_refuse_over: int = 40
    max_repeated_fingerprint: int = 3
    max_compile_error_streak: int = 4
    auto_size_trim: bool = True
    max_size_trim_attempts: int = 2
    ledger_path: str = "tools/llm_decomp/tu_ledger.json"
    auto_promote: bool = True
    auto_promote_owner: str = "llm-decomp"
    checkpoint_commits: bool = True

    def budgets_for(self, session_type: str) -> dict:
        st = self.session_types.get(session_type)
        if st is not None:
            return dict(st.budgets)
        return dict(DEFAULT_BUDGETS.get(session_type, DEFAULT_BUDGETS["match"]))

    def model_for(self, session_type: str) -> ModelConfig:
        st = self.session_types.get(session_type)
        if st is not None:
            return st.model
        return self.default_model


def _model_from(d: dict, base: ModelConfig) -> ModelConfig:
    if not d:
        return base
    max_tokens = d.get("max_tokens", base.max_tokens)
    if max_tokens == "unlimited":
        max_tokens = None
    thinking = d.get("thinking_budget", base.thinking_budget)
    if thinking == "unlimited":
        thinking = -1
    return ModelConfig(
        provider=d.get("provider", base.provider),
        model=d.get("model", base.model),
        max_tokens=max_tokens,
        thinking_budget=thinking,
    )


def load_config(repo_root: Path) -> Config:
    path = repo_root / CONFIG_FILENAME
    raw: dict = {}
    if path.exists():
        raw = json.loads(path.read_text(encoding="utf-8"))

    cfg = Config()
    cfg.default_model = _model_from(raw.get("models", {}).get("default", {}),
                                    ModelConfig())

    models_raw = raw.get("models", {})
    for name, default_budgets in DEFAULT_BUDGETS.items():
        st_raw = raw.get("session_types", {}).get(name, {})
        model = _model_from(st_raw.get("model") or models_raw.get(name, {}),
                            cfg.default_model)
        budgets = dict(default_budgets)
        budgets.update(st_raw.get("budgets", {}))
        cfg.session_types[name] = SessionTypeConfig(model=model, budgets=budgets)

    conc = raw.get("concurrency", {})
    cfg.workers = conc.get("workers", cfg.workers)
    cfg.target_parallel = conc.get("target_parallel", cfg.target_parallel)

    prompt = raw.get("prompt", {})
    cfg.prompt_max_chars = prompt.get("max_chars", cfg.prompt_max_chars)
    cfg.prompt_include_raw_hex = prompt.get("include_raw_hex",
                                            cfg.prompt_include_raw_hex)

    ver = raw.get("verification", {})
    cfg.equivalence_min_fuzzy = ver.get("equivalence_min_fuzzy",
                                        cfg.equivalence_min_fuzzy)
    cfg.cross_tu_full_sweep_max = ver.get("cross_tu_full_sweep_max",
                                          cfg.cross_tu_full_sweep_max)
    cfg.cross_tu_refuse_over = ver.get("cross_tu_refuse_over",
                                       cfg.cross_tu_refuse_over)

    stop = raw.get("stop", {})
    cfg.max_repeated_fingerprint = stop.get("max_repeated_fingerprint",
                                            cfg.max_repeated_fingerprint)
    cfg.max_compile_error_streak = stop.get("max_compile_error_streak",
                                            cfg.max_compile_error_streak)

    pipe = raw.get("pipeline", {})
    cfg.auto_size_trim = pipe.get("auto_size_trim", cfg.auto_size_trim)
    cfg.max_size_trim_attempts = pipe.get("max_size_trim_attempts",
                                          cfg.max_size_trim_attempts)
    cfg.ledger_path = pipe.get("ledger", cfg.ledger_path)

    exe = raw.get("execution", {})
    cfg.auto_promote = exe.get("auto_promote", cfg.auto_promote)
    cfg.auto_promote_owner = exe.get("auto_promote_owner",
                                     cfg.auto_promote_owner)
    cfg.checkpoint_commits = exe.get("checkpoint_commits",
                                     cfg.checkpoint_commits)
    return cfg

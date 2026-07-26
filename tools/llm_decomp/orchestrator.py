"""Pipeline orchestrator (design §12).

Runs the session-type pipeline on TUs: match×N (chained, frontier order)
with promotion per acceptance, ledger tracking, checkpoint commits, and
parallel pipelines across disjoint TUs. Rename stages run after all
parallel work drains (global quiesce, review #4).
"""
from __future__ import annotations

import json
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass, field
from pathlib import Path

from . import paths, state
from .config import Config
from .contracts import SessionStatus
from .ledger import Ledger
from .policies import get_policy, resolve_writable
from .promote import PromotionQueue
from .providers import get_provider
from .session import Session


@dataclass
class PipelineResult:
    unit: str
    accepted: list[str] = field(default_factory=list)  # target ids
    failed: list[str] = field(default_factory=list)
    skipped: bool = False
    reason: str = ""
    stages_run: list[str] = field(default_factory=list)


def _unit_targets(repo_root: Path, unit: str) -> list:
    from tools.coop.lib.config import load_config as load_coop_config
    from tools.coop.lib.targets import load_targets
    coop_cfg = load_coop_config(None, repo_root)
    return [t for t in load_targets(coop_cfg)
            if t.unit == unit and t.buildable
            and t.status not in ("FULL_MATCH", "EQUIVALENT_MATCH")]


def _all_unit_targets(repo_root: Path, unit: str) -> list:
    """All targets of a unit, including accepted ones (stage skip checks)."""
    from tools.coop.lib.config import load_config as load_coop_config
    from tools.coop.lib.targets import load_targets
    coop_cfg = load_coop_config(None, repo_root)
    return [t for t in load_targets(coop_cfg) if t.unit == unit]


def select_tus(repo_root: Path, cfg: Config, number: int) -> list[str]:
    """Rank TUs for auto ingestion (design §12)."""
    from tools.coop.lib.config import load_config as load_coop_config
    from tools.coop.lib.targets import load_targets
    coop_cfg = load_coop_config(None, repo_root)
    targets = load_targets(coop_cfg)
    ledger = Ledger(repo_root / cfg.ledger_path)
    done = set(ledger.complete_units())

    by_unit: dict[str, list] = {}
    for t in targets:
        if t.buildable and t.unit:
            by_unit.setdefault(t.unit, []).append(t)

    ranked = []
    for unit, ts in by_unit.items():
        if unit in done:
            continue
        remaining = [t for t in ts
                     if t.status not in ("FULL_MATCH", "EQUIVALENT_MATCH")]
        if not remaining:
            continue  # fully accepted -> rename/cleanup-only (not auto)
        src = Path(ts[0].source) if ts[0].source else None
        if src is None or not src.exists():
            continue
        matched = len(ts) - len(remaining)
        tiers = {t.tier for t in remaining}
        ranked.append((
            -matched,                      # partially-matched first
            0 if tiers & {"P0", "P1"} else 1,
            len(remaining),                # least remaining work
            unit))
    ranked.sort()
    return [unit for *_rest, unit in ranked[:number]]


class Orchestrator:
    def __init__(self, repo_root: Path, cfg: Config):
        self.repo_root = repo_root
        self.cfg = cfg
        self.promote = PromotionQueue(repo_root, cfg)
        self.ledger = Ledger(repo_root / cfg.ledger_path)
        self._unit_locks: set[str] = set()
        self._lock = threading.Lock()
        self._rename_pending: list[str] = []

    # ── session chaining ─────────────────────────────────────────────

    def _run_session(self, target, carryover: str | None):
        from .run import _retail_asm  # late import: shares CLI helpers
        policy = get_policy("match")
        unit, symbol = target.unit or "", target.symbol or ""
        src = str(Path(target.source).relative_to(self.repo_root))
        header = f"include/{unit}.hpp"
        writable = resolve_writable(policy, src, header)
        provider = get_provider(self.cfg.model_for("match").provider)
        meta, sdir = state.init_session(
            self.repo_root, target.id, "match", unit, symbol, writable,
            self.cfg.auto_promote_owner)
        from tools.coop.lib.config import load_config as load_coop_config
        from tools.coop.lib.targets import claim_target, release_target
        coop_cfg = load_coop_config(None, self.repo_root)
        claim_target(coop_cfg, target.id, owner=self.cfg.auto_promote_owner,
                     allowed_paths=writable)
        try:
            session = Session(
                self.repo_root, self.cfg, policy, meta, sdir, provider,
                demangled=target.function, signature=target.function,
                retail_asm=_retail_asm(coop_cfg.region, unit, symbol),
                target_id=target.id, carryover=carryover)
            outcome = session.run()
            if outcome.accepted and self.cfg.auto_promote and \
                    session._verdict is not None:
                self.promote.promote_accepted(meta, session._verdict)
            return outcome, session
        finally:
            release_target(coop_cfg, target.id,
                           owner=self.cfg.auto_promote_owner)

    @staticmethod
    def _carryover(prev_outcome, prev_session, target) -> str:
        v = prev_session._verdict
        lines = [f"Previous target on this unit: {target.function}",
                 f"Outcome: {'accepted' if prev_outcome.accepted else 'failed'}"
                 f" ({prev_outcome.reason})."]
        if v is not None:
            lines.append(f"Final verdict: {v.rule}, target mismatches "
                         f"{v.target_mismatches}.")
        if prev_session.best_mismatches is not None:
            lines.append(f"Best achieved mismatches: "
                         f"{prev_session.best_mismatches}.")
        return "\n".join(lines)

    # ── typed (non-match) sessions ───────────────────────────────────

    def _run_typed_session(self, session_type: str, unit: str):
        policy = get_policy(session_type)
        targets = _all_unit_targets(self.repo_root, unit)
        src = None
        for t in targets:
            if t.source and Path(t.source).exists():
                src = str(Path(t.source).relative_to(self.repo_root))
                break
        if src is None:
            return None, None
        header = f"include/{unit}.hpp"
        writable = resolve_writable(policy, src, header)
        provider = get_provider(
            self.cfg.model_for(session_type).provider)
        meta, sdir = state.init_session(
            self.repo_root, f"unit:{unit}", session_type, unit, "",
            writable, self.cfg.auto_promote_owner)
        session = Session(
            self.repo_root, self.cfg, policy, meta, sdir, provider,
            demangled=unit, signature="", retail_asm="",
            target_id=meta.target_id)
        outcome = session.run()
        if outcome.accepted:
            self.ledger.mark_stage(unit, session_type, meta.session_id)
        return outcome, session

    # ── pipelines ────────────────────────────────────────────────────

    def run_pipeline(self, unit: str,
                     stages: tuple[str, ...] | None = None,
                     defer_rename: bool = False) -> PipelineResult:
        if stages is None:
            stages = ("type-recovery", "match", "rename", "tu-cleanup")
        result = PipelineResult(unit=unit)
        with self._lock:
            if unit in self._unit_locks:
                result.skipped = True
                result.reason = "unit locked by another pipeline"
                return result
            self._unit_locks.add(unit)
        try:
            for stage in stages:
                remaining = _unit_targets(self.repo_root, unit)
                all_targets = _all_unit_targets(self.repo_root, unit)
                has_unk = any("UnkClass_" in (t.function or "")
                              for t in all_targets)

                if stage == "type-recovery":
                    if not has_unk:
                        continue
                    outcome, _s = self._run_typed_session(stage, unit)
                    if outcome is None:
                        continue
                    result.stages_run.append(stage)

                elif stage == "match":
                    carryover = None
                    prev = None
                    for target in remaining:
                        if prev is not None:
                            carryover = self._carryover(*prev, target)
                        outcome, session = self._run_session(
                            target, carryover)
                        prev = (outcome, session)
                        (result.accepted if outcome.accepted
                         else result.failed).append(target.id)
                    if remaining:
                        result.stages_run.append(stage)
                    else:
                        result.reason = "no remaining targets"

                elif stage == "rename":
                    if not has_unk or remaining:
                        continue
                    if defer_rename:
                        with self._lock:
                            self._rename_pending.append(unit)
                        continue
                    self._run_rename_stage(unit)
                    result.stages_run.append(stage)

                elif stage == "tu-cleanup":
                    if remaining:
                        continue
                    outcome, _s = self._run_typed_session(stage, unit)
                    if outcome is None:
                        continue
                    result.stages_run.append(stage)

                self.promote.checkpoint(
                    f"llm-decomp: {stage} stage complete for {unit}",
                    ["tools/coop/targets.json", self.cfg.ledger_path])

            if not result.failed and (result.accepted
                                      or result.stages_run):
                self.ledger.set_status(unit, "complete")
                self.promote.mark_unit_matching(unit)
            return result
        finally:
            with self._lock:
                self._unit_locks.discard(unit)

    def _run_rename_stage(self, unit: str) -> None:
        """Rename stage — must only run with no other pipelines live."""
        outcome, session = self._run_typed_session("rename", unit)
        if outcome is not None and outcome.accepted:
            self.promote.checkpoint(
                f"llm-decomp: rename stage complete for {unit}",
                ["tools/coop/targets.json", self.cfg.ledger_path])

    def run(self, units: list[str]) -> list[PipelineResult]:
        """Parallel pipelines across disjoint TUs (shared build lock).

        Rename stages are deferred until every pipeline drains (global
        quiesce, review #4).
        """
        if len(units) <= 1:
            return [self.run_pipeline(u) for u in units]
        results: list[PipelineResult] = []
        with ThreadPoolExecutor(
                max_workers=self.cfg.target_parallel) as pool:
            futures = {pool.submit(self.run_pipeline, u, defer_rename=True):
                       u for u in units}
            for fut in as_completed(futures):
                results.append(fut.result())
        # Quiesced rename stages.
        pending: list[str] = []
        with self._lock:
            pending, self._rename_pending = self._rename_pending, []
        for unit in pending:
            self._run_rename_stage(unit)
        return results


def results_to_json(results: list[PipelineResult]) -> str:
    return json.dumps([r.__dict__ for r in results], indent=2)

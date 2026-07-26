"""Single-writer promotion path (design §7, review #5).

All writes to checked-in state (targets.json, configure.py, the TU ledger,
git checkpoints) go through PromotionQueue. A module-level lock serializes
mutations in-process; targets.json has its own interprocess lock inside
tools.coop.lib.targets.
"""
from __future__ import annotations

import subprocess
import sys
import threading
from pathlib import Path

from .config import Config
from .contracts import SessionMeta, Verdict
from .ledger import Ledger
from .verify import BuildLock

_LOCK = threading.Lock()


class PromotionError(RuntimeError):
    pass


def _git(repo_root: Path, *args: str) -> subprocess.CompletedProcess:
    return subprocess.run(["git", *args], cwd=repo_root, capture_output=True,
                          text=True, timeout=120)


class PromotionQueue:
    def __init__(self, repo_root: Path, cfg: Config):
        self.repo_root = repo_root
        self.cfg = cfg

    def checkpoint(self, message: str, add_paths: list[str]) -> bool:
        """Git checkpoint commit (design §7). False when disabled or empty."""
        if not self.cfg.checkpoint_commits:
            return False
        with _LOCK:
            add = _git(self.repo_root, "add", "--", *add_paths)
            if add.returncode != 0:
                raise PromotionError(f"git add failed: {add.stderr[-500:]}")
            commit = _git(self.repo_root, "commit", "-m", message)
            out = (commit.stdout + commit.stderr).lower()
            if commit.returncode != 0:
                if "nothing to commit" in out or "no changes added" in out:
                    return False
                raise PromotionError(f"git commit failed: {out[-500:]}")
            return True

    def promote_accepted(self, meta: SessionMeta, verdict: Verdict) -> dict:
        """Record an accepted match: targets.json + ledger + checkpoint."""
        from tools.coop.lib.config import load_config as load_coop_config
        from tools.coop.lib.targets import update_target_result

        tier = ("FULL_MATCH" if verdict.rule == "full_match"
                else "EQUIVALENT_MATCH")
        fuzzy = verdict.target_fuzzy if verdict.target_fuzzy is not None \
            else 100.0
        with _LOCK:
            coop_cfg = load_coop_config(None, self.repo_root)
            update_target_result(
                coop_cfg, meta.target_id, status=tier,
                instruction_match=fuzzy,
                equivalence_status=("EQUIVALENT"
                                    if tier == "EQUIVALENT_MATCH" else None))
            Ledger(self.repo_root / self.cfg.ledger_path).mark_stage(
                meta.unit, "match", meta.session_id)
            committed = self.checkpoint(
                f"llm-decomp: match {meta.symbol} ({meta.target_id}) in "
                f"{meta.unit} [{tier}]",
                [*meta.writable, "tools/coop/targets.json",
                 self.cfg.ledger_path])
        return {"target_id": meta.target_id, "tier": tier,
                "committed": committed}

    def mark_unit_matching(self, unit: str) -> bool:
        """Flip configure.py Object(NonMatching, "<unit>") -> Matching."""
        configure = self.repo_root / "configure.py"
        text = configure.read_text(encoding="utf-8")
        replaced = False
        for ext in (".cpp", ".c"):
            needle = f'Object(NonMatching, "{unit}{ext}"'
            if needle in text:
                text = text.replace(
                    needle, f'Object(Matching, "{unit}{ext}"', 1)
                replaced = True
                break
        if not replaced:
            return False
        from tools.coop.lib.config import load_config as load_coop_config
        region = load_coop_config(None, self.repo_root).region
        with _LOCK, BuildLock(self.repo_root):
            configure.write_text(text, encoding="utf-8")
            proc = subprocess.run(
                [sys.executable, "configure.py", "--version", region],
                cwd=self.repo_root, capture_output=True, text=True,
                timeout=600)
        if proc.returncode != 0:
            raise PromotionError(
                f"configure.py failed: {(proc.stdout + proc.stderr)[-500:]}")
        return True

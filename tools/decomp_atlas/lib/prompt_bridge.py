"""Bridge to llm-harness prompt construction (read-only preview)."""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any, Dict, Literal, Optional

Workflow = Literal["new", "improve"]


def preview_prompt(
    root: Path | str,
    target_id: str,
    workflow: Workflow = "new",
    full_context: bool = False,
    *,
    config_path: Optional[Path | str] = None,
) -> Dict[str, Any]:
    """Build a harness prompt without running models or mutating state."""
    root_path = Path(root).resolve()
    if workflow not in {"new", "improve"}:
        return {
            "ok": False,
            "prompt": None,
            "error": f"unsupported workflow {workflow!r}; allowed: new, improve",
            "bytes": 0,
        }
    harness_config = Path(config_path) if config_path else root_path / "llm-harness.json"
    if not harness_config.is_file():
        example = root_path / "llm-harness.example.json"
        harness_config = example if example.is_file() else harness_config
    if not harness_config.is_file():
        return {
            "ok": False,
            "prompt": None,
            "error": f"llm-harness config not found at {harness_config}",
            "bytes": 0,
        }
    try:
        settings = json.loads(harness_config.read_text(encoding="utf-8"))
        project_settings = settings.get("project") or {}
        from tools.llm_harness.xenoblade_project import XenobladeAdapter

        adapter = XenobladeAdapter(root_path, project_settings)
        options = {"full_context": bool(full_context)}
        prompt = adapter.build_prompt(workflow, target_id, history=[], options=options)
        encoded = prompt.encode("utf-8")
        return {
            "ok": True,
            "prompt": prompt,
            "error": None,
            "bytes": len(encoded),
            "workflow": workflow,
            "target_id": target_id,
            "full_context": bool(full_context),
        }
    except Exception as exc:  # noqa: BLE001 — surface to API clients
        return {
            "ok": False,
            "prompt": None,
            "error": str(exc),
            "bytes": 0,
            "workflow": workflow,
            "target_id": target_id,
            "full_context": bool(full_context),
        }

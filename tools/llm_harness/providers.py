from __future__ import annotations

import json
import os
import subprocess
import tempfile
import time
from pathlib import Path
from typing import Any, Dict, Iterable, Optional

from .types import ModelConfig, ProviderResult


class OpenCodeProvider:
    def __init__(self, binary: str = "opencode", timeout_seconds: int = 900) -> None:
        self.binary = binary
        self.timeout_seconds = timeout_seconds

    def invoke(self, prompt: str, model: ModelConfig, cwd: Path) -> ProviderResult:
        started = time.monotonic()
        prompt_file: Optional[Path] = None
        try:
            with tempfile.NamedTemporaryFile(
                mode="w", suffix=".md", prefix="llm-harness-", encoding="utf-8", delete=False
            ) as f:
                f.write(prompt)
                prompt_file = Path(f.name)
            cmd = [
                self.binary,
                "run",
                "--model",
                model.model,
                "--format",
                "json",
                "--dir",
                str(cwd),
                "--file",
                str(prompt_file),
            ]
            if model.agent:
                cmd.extend(["--agent", model.agent])
            if model.variant:
                cmd.extend(["--variant", model.variant])
            cmd.append(
                "Read the attached decompilation dossier. Return only the requested JSON object; "
                "do not edit files or run tools."
            )
            env = os.environ.copy()
            # Dossiers can contain proprietary function bytes; never inherit auto-sharing.
            env["OPENCODE_AUTO_SHARE"] = "false"
            completed = subprocess.run(
                cmd,
                cwd=cwd,
                env=env,
                text=True,
                capture_output=True,
                timeout=self.timeout_seconds,
                check=False,
            )
            if completed.returncode:
                detail = completed.stderr.strip() or completed.stdout.strip()
                raise RuntimeError(f"OpenCode exited {completed.returncode}: {detail[-2000:]}")
            text, events, usage = parse_opencode_output(completed.stdout)
            return ProviderResult(
                text=text,
                duration_seconds=time.monotonic() - started,
                input_tokens=_int_or_none(usage.get("input")),
                output_tokens=_int_or_none(usage.get("output")),
                cost=_float_or_none(usage.get("cost")),
                raw_events=events,
            )
        finally:
            if prompt_file is not None:
                prompt_file.unlink(missing_ok=True)


def parse_opencode_output(stdout: str) -> tuple[str, list[Dict[str, Any]], Dict[str, Any]]:
    """Parse OpenCode's raw JSON events while tolerating plain JSON/text output."""
    events: list[Dict[str, Any]] = []
    for line in stdout.splitlines():
        try:
            value = json.loads(line)
        except json.JSONDecodeError:
            continue
        if isinstance(value, dict):
            events.append(value)

    if not events:
        return stdout.strip(), [], {}

    chunks: list[str] = []
    usage: Dict[str, Any] = {}
    for event in events:
        for value in _walk(event):
            if isinstance(value, dict):
                candidate_usage = value.get("usage")
                if isinstance(candidate_usage, dict):
                    usage = _normalise_usage(candidate_usage, value)
        part = event.get("part")
        if isinstance(part, dict) and part.get("type") in {"text", "output_text"}:
            if isinstance(part.get("text"), str):
                chunks.append(part["text"])
        elif event.get("type") in {"text", "output_text"} and isinstance(event.get("text"), str):
            chunks.append(event["text"])

    if chunks:
        return "".join(chunks).strip(), events, usage
    # Some versions emit a single JSON response object rather than text events.
    for event in reversed(events):
        for key in ("text", "content", "output", "response"):
            if isinstance(event.get(key), str):
                return event[key].strip(), events, usage
    return stdout.strip(), events, usage


def _walk(value: Any) -> Iterable[Any]:
    yield value
    if isinstance(value, dict):
        for child in value.values():
            yield from _walk(child)
    elif isinstance(value, list):
        for child in value:
            yield from _walk(child)


def _normalise_usage(usage: Dict[str, Any], parent: Dict[str, Any]) -> Dict[str, Any]:
    return {
        "input": usage.get("input") or usage.get("input_tokens") or usage.get("promptTokens"),
        "output": usage.get("output") or usage.get("output_tokens") or usage.get("completionTokens"),
        "cost": usage.get("cost") or parent.get("cost"),
    }


def _int_or_none(value: Any) -> Optional[int]:
    try:
        return int(value) if value is not None else None
    except (TypeError, ValueError):
        return None


def _float_or_none(value: Any) -> Optional[float]:
    try:
        return float(value) if value is not None else None
    except (TypeError, ValueError):
        return None

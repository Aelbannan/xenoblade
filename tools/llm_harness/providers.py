from __future__ import annotations

import base64
import json
import os
import subprocess
import time
import urllib.error
import urllib.parse
import urllib.request
from pathlib import Path
from typing import Any, Dict, Iterable, Optional

from .types import ModelConfig, ProviderResult


def _load_env_vars(path: Path) -> None:
    """Load KEY=value lines from a .env file into os.environ (no overwrite)."""
    try:
        text = path.read_text(encoding="utf-8")
    except (FileNotFoundError, OSError):
        return
    for line in text.splitlines():
        stripped = line.strip()
        if not stripped or stripped.startswith("#"):
            continue
        if "=" not in stripped:
            continue
        key, _, value = stripped.partition("=")
        key = key.strip()
        value = value.strip().strip("'\"")
        if key and key not in os.environ:
            os.environ[key] = value


class OpenCodeProvider:
    """OpenCode provider via the headless HTTP server (`opencode serve`).

    Talks to POST /session and POST /session/{id}/message instead of spawning
    `opencode run`. Start the server separately, e.g.:

        opencode serve --port 4096 --hostname 127.0.0.1

    Optional basic auth uses OPENCODE_SERVER_USERNAME / OPENCODE_SERVER_PASSWORD
    (or username/password in llm-harness.json).
    """

    DEFAULT_BASE_URL = "http://127.0.0.1:4096"
    USERNAME_ENV = "OPENCODE_SERVER_USERNAME"
    PASSWORD_ENV = "OPENCODE_SERVER_PASSWORD"

    # Mutating / interactive tools always denied for harness purity.
    _DENIED_TOOLS = (
        "bash",
        "edit",
        "write",
        "apply_patch",
        "task",
        "webfetch",
        "websearch",
        "todowrite",
        "question",
        "skill",
    )
    _READ_TOOLS = ("read", "glob", "grep")

    def __init__(
        self,
        base_url: str = DEFAULT_BASE_URL,
        timeout_seconds: int = 900,
        pure: bool = True,
        username: Optional[str] = None,
        password: Optional[str] = None,
        delete_session: bool = True,
        binary: str = "opencode",  # accepted for config compat; unused
        **_ignored: Any,
    ) -> None:
        self.base_url = base_url.rstrip("/")
        self.timeout_seconds = timeout_seconds
        self.pure = pure
        self.username = username
        self.password = password
        self.delete_session = delete_session
        self.binary = binary
        _load_env_vars(Path(".env"))

    def invoke(self, prompt: str, model: ModelConfig, cwd: Path) -> ProviderResult:
        started = time.monotonic()
        self._ensure_healthy()
        directory = str(cwd.resolve())
        has_context_files = any(cwd.iterdir())
        guardrail = (
            "Read the attached dossier and the curated files in the current context directory. "
            "Use read/search tools only when helpful, stay inside this context, do not edit files "
            "or run shell commands, and return only the requested JSON object."
            if has_context_files
            else "Read the attached self-contained dossier. Do not use tools or inspect files. "
            "Return only the requested JSON object."
        )
        full_prompt = f"{guardrail}\n\n{prompt}"
        provider_id, model_id = _split_opencode_model(model.model)
        session_id: Optional[str] = None
        try:
            session = self._request(
                "POST",
                "/session",
                body={
                    "title": f"llm-harness:{model.id}",
                    "permission": self._permission_ruleset(has_context_files),
                },
                query={"directory": directory},
            )
            session_id = str(session.get("id") or "")
            if not session_id:
                raise RuntimeError(f"OpenCode server: session create missing id: {session!r}")

            message_body: Dict[str, Any] = {
                "model": {"providerID": provider_id, "modelID": model_id},
                "parts": [{"type": "text", "text": full_prompt}],
                "tools": self._tools_map(has_context_files),
            }
            if model.agent:
                message_body["agent"] = model.agent
            if model.variant:
                message_body["variant"] = model.variant

            response = self._request(
                "POST",
                f"/session/{session_id}/message",
                body=message_body,
                query={"directory": directory},
            )
            text, events, usage = parse_opencode_message(response)
            if not text:
                raise RuntimeError(
                    "OpenCode server: empty assistant text in message response"
                )
            return ProviderResult(
                text=text,
                duration_seconds=time.monotonic() - started,
                input_tokens=_int_or_none(usage.get("input")),
                output_tokens=_int_or_none(usage.get("output")),
                cache_read_tokens=_int_or_none(usage.get("cache_read")),
                cache_write_tokens=_int_or_none(usage.get("cache_write")),
                cost=_float_or_none(usage.get("cost")),
                raw_events=events,
            )
        finally:
            if self.delete_session and session_id:
                try:
                    self._request(
                        "DELETE",
                        f"/session/{session_id}",
                        query={"directory": directory},
                    )
                except Exception:
                    pass

    def _ensure_healthy(self) -> None:
        try:
            health = self._request("GET", "/global/health")
        except Exception as exc:
            raise RuntimeError(
                f"OpenCode server unreachable at {self.base_url} "
                f"(start with `opencode serve --port 4096`): {exc}"
            ) from exc
        if not (isinstance(health, dict) and health.get("healthy")):
            raise RuntimeError(
                f"OpenCode server unhealthy at {self.base_url}: {health!r}"
            )

    def _tools_map(self, has_context_files: bool) -> Dict[str, bool]:
        tools = {name: False for name in self._DENIED_TOOLS}
        for name in self._READ_TOOLS:
            tools[name] = bool(has_context_files)
        return tools

    def _permission_ruleset(self, has_context_files: bool) -> list[Dict[str, str]]:
        rules: list[Dict[str, str]] = []
        for name in self._DENIED_TOOLS:
            rules.append({"permission": name, "pattern": "*", "action": "deny"})
        read_action = "allow" if has_context_files else "deny"
        for name in self._READ_TOOLS:
            rules.append({"permission": name, "pattern": "*", "action": read_action})
        return rules

    def _auth_header(self) -> Optional[str]:
        password = self.password or os.environ.get(self.PASSWORD_ENV)
        if not password:
            return None
        username = (
            self.username
            or os.environ.get(self.USERNAME_ENV)
            or "opencode"
        )
        token = base64.b64encode(f"{username}:{password}".encode("utf-8")).decode("ascii")
        return f"Basic {token}"

    def _request(
        self,
        method: str,
        path: str,
        body: Optional[Dict[str, Any]] = None,
        query: Optional[Dict[str, str]] = None,
    ) -> Any:
        url = f"{self.base_url}{path}"
        if query:
            url = f"{url}?{urllib.parse.urlencode(query)}"
        data = None
        headers = {"Accept": "application/json"}
        auth = self._auth_header()
        if auth:
            headers["Authorization"] = auth
        if body is not None:
            data = json.dumps(body).encode("utf-8")
            headers["Content-Type"] = "application/json"
        request = urllib.request.Request(url, data=data, headers=headers, method=method)
        try:
            with urllib.request.urlopen(request, timeout=self.timeout_seconds) as response:
                raw = response.read().decode("utf-8")
                if not raw:
                    return None
                return json.loads(raw)
        except urllib.error.HTTPError as exc:
            detail = exc.read().decode("utf-8", errors="replace")
            raise RuntimeError(
                f"OpenCode server {method} {path} failed "
                f"(HTTP {exc.code}): {detail[-2000:]}"
            ) from exc
        except urllib.error.URLError as exc:
            raise RuntimeError(
                f"OpenCode server {method} {path} failed: {exc.reason}"
            ) from exc


def _split_opencode_model(model: str) -> tuple[str, str]:
    """Split `provider/model` into OpenCode providerID + modelID."""
    provider_id, sep, model_id = model.partition("/")
    if not sep or not provider_id or not model_id:
        raise RuntimeError(
            f"OpenCode model must be 'provider/model', got: {model!r}"
        )
    return provider_id, model_id


def parse_opencode_message(
    response: Any,
) -> tuple[str, list[Dict[str, Any]], Dict[str, Any]]:
    """Parse POST /session/:id/message JSON into text, events, and usage."""
    if not isinstance(response, dict):
        return "", [], {}
    events = [response]
    info = response.get("info") if isinstance(response.get("info"), dict) else {}
    if isinstance(info.get("error"), dict):
        err = info["error"]
        message = err.get("data", {}).get("message") if isinstance(err.get("data"), dict) else None
        message = message or err.get("name") or str(err)
        raise RuntimeError(f"OpenCode server message error: {message}")

    chunks: list[str] = []
    for part in response.get("parts") or []:
        if not isinstance(part, dict):
            continue
        if part.get("type") in {"text", "output_text"} and isinstance(part.get("text"), str):
            chunks.append(part["text"])

    tokens = info.get("tokens") if isinstance(info.get("tokens"), dict) else {}
    cache = tokens.get("cache") if isinstance(tokens.get("cache"), dict) else {}
    usage = {
        "input": tokens.get("input"),
        "output": tokens.get("output"),
        "cache_read": cache.get("read"),
        "cache_write": cache.get("write"),
        "cost": info.get("cost"),
    }
    structured = info.get("structured_output")
    if structured is not None and not chunks:
        return json.dumps(structured), events, usage
    return "".join(chunks).strip(), events, usage


class ReasonixProvider:
    def __init__(
        self, binary: str = "reasonix", timeout_seconds: int = 900, pure: bool = True
    ) -> None:
        self.binary = binary
        self.timeout_seconds = timeout_seconds
        self.pure = pure

    def invoke(self, prompt: str, model: ModelConfig, cwd: Path) -> ProviderResult:
        started = time.monotonic()
        has_context_files = any(cwd.iterdir())
        guardrail = (
            "Read the attached dossier and the curated files in the current context directory. "
            "Use read/search tools only when helpful, stay inside this context, do not edit files "
            "or run shell commands, and return only the requested JSON object."
            if has_context_files
            else "Read the attached self-contained dossier. Do not use tools or inspect files. "
            "Return only the requested JSON object."
        )
        full_prompt = f"{guardrail}\n\n{prompt}"
        cmd = [
            self.binary,
            "-p",
            "--model",
            model.model,
            "--output-format",
            "json",
            "--permission-mode",
            "bypassPermissions",
            "--dir",
            str(cwd),
        ]
        env = os.environ.copy()
        env["REASONIX_AUTO_SHARE"] = "false"
        completed = subprocess.run(
            cmd,
            input=full_prompt,
            cwd=cwd,
            env=env,
            text=True,
            capture_output=True,
            timeout=self.timeout_seconds,
            check=False,
        )
        if completed.returncode:
            detail = completed.stderr.strip() or completed.stdout.strip()
            raise RuntimeError(f"Reasonix exited {completed.returncode}: {detail[-2000:]}")
        text, events, usage = parse_reasonix_output(completed.stdout)
        return ProviderResult(
            text=text,
            duration_seconds=time.monotonic() - started,
            input_tokens=_int_or_none(usage.get("input")),
            output_tokens=_int_or_none(usage.get("output")),
            cache_read_tokens=_int_or_none(usage.get("cache_read")),
            cache_write_tokens=_int_or_none(usage.get("cache_write")),
            cost=_float_or_none(usage.get("cost")),
            raw_events=events,
        )


class DeepSeekRawProvider:
    """Direct DeepSeek API provider — bypasses Reasonix agent scaffolding.

    Calls https://api.deepseek.com/chat/completions directly with *only* the
    user message (no system prompt, no tool schemas, no environment probes).
    This gives full control over the exact message bytes so DeepSeek's
    provider-side prefix caching works reliably across repeated runs.

    The api_key is read from the DEEPSEEK_API_KEY environment variable
    (same one Reasonix reads from ~/.reasonix/.env).
    """

    MODEL_MAP = {
        "deepseek-flash": "deepseek-v4-flash",
        "deepseek-pro": "deepseek-v4-pro",
    }
    API_URL = "https://api.deepseek.com/chat/completions"
    API_KEY_ENV = "DEEPSEEK_API_KEY"

    def __init__(
        self, binary: str = "deepseek-raw", timeout_seconds: int = 300, pure: bool = True
    ) -> None:
        self.timeout_seconds = timeout_seconds
        # Load .env from project root and ~/.reasonix/.env as fallbacks
        _load_env_vars(Path(".env"))
        _load_env_vars(Path.home() / ".reasonix" / ".env")

    def invoke(self, prompt: str, model: ModelConfig, cwd: Path) -> ProviderResult:
        started = time.monotonic()
        api_key = os.environ.get(self.API_KEY_ENV)
        if not api_key:
            raise RuntimeError(
                f"DeepSeek API key not found in ${self.API_KEY_ENV} — "
                f"set it in ~/.reasonix/.env or your shell profile"
            )

        deepseek_model = self.MODEL_MAP.get(model.model, model.model)
        max_tokens = int(model.max_tokens) if model.max_tokens else 4096
        request_body = json.dumps({
            "model": deepseek_model,
            "messages": [{"role": "user", "content": prompt}],
            "temperature": 0.1,
            "max_tokens": max_tokens,
            "response_format": {"type": "json_object"},
        })

        cmd = [
            "curl", "-s", "-w", "\n%{http_code}",
            "-X", "POST", self.API_URL,
            "-H", "Content-Type: application/json",
            "-H", f"Authorization: Bearer {api_key}",
            "-d", request_body,
        ]
        completed = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=self.timeout_seconds,
        )
        # curl -w injects the HTTP status code as the last line of stdout
        stdout = completed.stdout.strip()
        lines = stdout.rsplit("\n", 1)
        if len(lines) != 2:
            detail = (completed.stderr.strip() or stdout)[:2000]
            raise RuntimeError(f"DeepSeek API: unexpected curl output: {detail}")
        response_body, http_code = lines[0].strip(), lines[1]

        if completed.returncode or not http_code.startswith("2"):
            detail = (completed.stderr.strip() or response_body)[:2000]
            raise RuntimeError(
                f"DeepSeek API error (HTTP {http_code}): {detail}"
            )

        result = json.loads(response_body)
        choices = result.get("choices", [])
        if not choices:
            raise RuntimeError(
                f"DeepSeek API: no choices in response: {response_body[:500]}"
            )

        text = choices[0].get("message", {}).get("content", "").strip()
        usage = result.get("usage", {}) or {}

        return ProviderResult(
            text=text,
            duration_seconds=time.monotonic() - started,
            input_tokens=_int_or_none(usage.get("prompt_tokens")),
            output_tokens=_int_or_none(usage.get("completion_tokens")),
            cache_read_tokens=_int_or_none(
                usage.get("prompt_cache_hit_tokens")
                or (usage.get("prompt_tokens_details") or {}).get("cached_tokens")
            ),
            cache_write_tokens=_int_or_none(
                usage.get("prompt_cache_miss_tokens")
            ),
            cost=None,
            raw_events=[result],
        )


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
    step_usage: list[Dict[str, Any]] = []
    for event in events:
        for value in _walk(event):
            if isinstance(value, dict):
                candidate_usage = value.get("usage")
                if isinstance(candidate_usage, dict):
                    usage = _normalise_usage(candidate_usage, value)
                candidate_tokens = value.get("tokens")
                if isinstance(candidate_tokens, dict):
                    step_usage.append(_normalise_usage(candidate_tokens, value))
        part = event.get("part")
        if isinstance(part, dict) and part.get("type") in {"text", "output_text"}:
            if isinstance(part.get("text"), str):
                chunks.append(part["text"])
        elif event.get("type") in {"text", "output_text"} and isinstance(event.get("text"), str):
            chunks.append(event["text"])

    if step_usage:
        usage = _sum_usage(step_usage)
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


def parse_reasonix_output(stdout: str) -> tuple[str, list[Dict[str, Any]], Dict[str, Any]]:
    """Parse Reasonix --output-format json result."""
    try:
        result = json.loads(stdout.strip())
    except json.JSONDecodeError:
        return stdout.strip(), [], {}

    if not isinstance(result, dict):
        return stdout.strip(), [], {}

    text = result.get("result", "")
    if not isinstance(text, str):
        text = stdout.strip()

    usage_data = result.get("usage") if isinstance(result.get("usage"), dict) else {}
    total_cost = result.get("total_cost_usd")
    usage = {
        "input": usage_data.get("input_tokens"),
        "output": usage_data.get("output_tokens"),
        "cache_read": usage_data.get("cache_read_input_tokens"),
        "cache_write": usage_data.get("cache_creation_input_tokens"),
        "cost": total_cost if total_cost is not None else usage_data.get("cost"),
    }
    return text, [result], usage


def _normalise_usage(usage: Dict[str, Any], parent: Dict[str, Any]) -> Dict[str, Any]:
    cache = usage.get("cache") if isinstance(usage.get("cache"), dict) else {}
    return {
        "input": usage.get("input") or usage.get("input_tokens") or usage.get("promptTokens"),
        "output": usage.get("output") or usage.get("output_tokens") or usage.get("completionTokens"),
        "cache_read": (
            usage.get("cache_read_tokens")
            or usage.get("cacheReadTokens")
            or cache.get("read")
        ),
        "cache_write": (
            usage.get("cache_write_tokens")
            or usage.get("cacheWriteTokens")
            or (
                int(usage.get("cache_write_5m_tokens") or 0)
                + int(usage.get("cache_write_1h_tokens") or 0)
            )
            or cache.get("write")
        ),
        "cost": usage.get("cost") or parent.get("cost"),
    }


def _sum_usage(rows: list[Dict[str, Any]]) -> Dict[str, Any]:
    return {
        "input": sum(int(row.get("input") or 0) for row in rows),
        "output": sum(int(row.get("output") or 0) for row in rows),
        "cache_read": sum(int(row.get("cache_read") or 0) for row in rows),
        "cache_write": sum(int(row.get("cache_write") or 0) for row in rows),
        "cost": sum(float(row.get("cost") or 0) for row in rows),
    }


def _int_or_none(value: Any) -> Optional[int]:
    try:
        return int(value) if value is not None else None
    except (TypeError, ValueError):
        return None


class LMStudioProvider:
    """Local LM Studio OpenAI-compatible chat completions provider.

    Calls POST {base_url}/chat/completions (default http://localhost:1234/v1).
    Start the local server in LM Studio's Developer tab and load a model first.
    Model IDs are those reported by GET /v1/models (or the LM Studio UI).

    When json_object is enabled, LM Studio receives response_format type
    json_schema (not OpenAI's json_object — LM Studio rejects that type).

    Optional thinking knobs are passed through as request fields only:
    enable_thinking, thinking_budget, reasoning_effort. No assistant prefills
    or multi-phase workarounds.
    """

    DEFAULT_BASE_URL = "http://localhost:1234/v1"
    API_KEY_ENV = "LMSTUDIO_API_KEY"
    CANDIDATE_JSON_SCHEMA: Dict[str, Any] = {
        "name": "decomp_candidate",
        "strict": True,
        "schema": {
            "type": "object",
            "properties": {
                "source": {
                    "type": "string",
                    "description": "Complete replacement high-level C/C++ function definition only",
                },
                "hypothesis": {
                    "type": "string",
                    "maxLength": 160,
                    "description": "One short hypothesis (<=160 chars)",
                },
                "notes": {
                    "type": "array",
                    "maxItems": 3,
                    "items": {"type": "string", "maxLength": 120},
                    "description": "Up to 3 short notes (<=120 chars each)",
                },
                "next_change": {
                    "type": "string",
                    "maxLength": 120,
                    "description": "One short follow-up (<=120 chars); empty if none",
                },
                "change": {
                    "type": "string",
                    "maxLength": 120,
                    "description": "One short change summary (<=120 chars); empty for first candidates",
                },
            },
            "required": ["source", "hypothesis", "notes", "next_change", "change"],
            "additionalProperties": False,
        },
    }

    def __init__(
        self,
        base_url: str = DEFAULT_BASE_URL,
        api_key: str = "lm-studio",
        timeout_seconds: int = 900,
        temperature: float = 0.1,
        max_tokens: int = 4096,
        json_object: bool = True,
        enable_thinking: bool = False,
        thinking_budget: Optional[int] = None,
        reasoning_effort: Optional[str] = None,
        pure: bool = True,
        **_ignored: Any,
    ) -> None:
        self.base_url = base_url.rstrip("/")
        self.api_key = api_key
        self.timeout_seconds = timeout_seconds
        self.temperature = temperature
        self.max_tokens = max_tokens
        self.json_object = json_object
        self.enable_thinking = enable_thinking
        self.thinking_budget = thinking_budget
        self.reasoning_effort = reasoning_effort
        self.pure = pure
        _load_env_vars(Path(".env"))

    @staticmethod
    def _effort_to_budget(effort: str) -> Optional[int]:
        mapping = {
            "none": 0,
            "minimal": 128,
            "low": 256,
            "medium": 1024,
            "high": 4096,
            "xhigh": 8192,
        }
        return mapping.get(effort.strip().lower())

    def invoke(self, prompt: str, model: ModelConfig, cwd: Path) -> ProviderResult:
        started = time.monotonic()
        api_key = os.environ.get(self.API_KEY_ENV) or self.api_key
        max_tokens = int(model.max_tokens) if model.max_tokens else self.max_tokens
        enable_thinking = (
            bool(model.enable_thinking)
            if model.enable_thinking is not None
            else self.enable_thinking
        )
        reasoning_effort = model.reasoning_effort or self.reasoning_effort
        thinking_budget = model.thinking_budget
        if thinking_budget is None:
            thinking_budget = self.thinking_budget
        if thinking_budget is None and reasoning_effort:
            thinking_budget = self._effort_to_budget(reasoning_effort)
        if reasoning_effort and str(reasoning_effort).strip().lower() == "none":
            enable_thinking = False
            thinking_budget = 0

        body: Dict[str, Any] = {
            "model": model.model,
            "messages": [{"role": "user", "content": prompt}],
            "temperature": self.temperature,
            "max_tokens": max_tokens,
        }
        if self.json_object:
            body["response_format"] = {
                "type": "json_schema",
                "json_schema": self.CANDIDATE_JSON_SCHEMA,
            }
        if reasoning_effort:
            body["reasoning_effort"] = str(reasoning_effort).strip().lower()
        if thinking_budget is not None:
            body["thinking_budget"] = int(thinking_budget)
        body["chat_template_kwargs"] = {"enable_thinking": bool(enable_thinking)}
        body["enable_thinking"] = bool(enable_thinking)
        body["enableThinking"] = bool(enable_thinking)

        request_body = json.dumps(body)
        url = f"{self.base_url}/chat/completions"
        cmd = [
            "curl", "-s", "-w", "\n%{http_code}",
            "-X", "POST", url,
            "-H", "Content-Type: application/json",
            "-H", f"Authorization: Bearer {api_key}",
            "-d", request_body,
        ]
        completed = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=self.timeout_seconds,
        )
        stdout = completed.stdout.strip()
        lines = stdout.rsplit("\n", 1)
        if len(lines) != 2:
            detail = (completed.stderr.strip() or stdout)[:2000]
            raise RuntimeError(f"LM Studio API: unexpected curl output: {detail}")
        response_body, http_code = lines[0].strip(), lines[1]
        if completed.returncode or not http_code.startswith("2"):
            detail = (completed.stderr.strip() or response_body)[:2000]
            raise RuntimeError(
                f"LM Studio API error (HTTP {http_code}): {detail}"
            )

        result = json.loads(response_body)
        choices = result.get("choices", [])
        if not choices:
            raise RuntimeError(
                f"LM Studio API: no choices in response: {response_body[:500]}"
            )

        message = choices[0].get("message", {}) or {}
        content = _strip_think_blocks(str(message.get("content") or "")).strip()
        reasoning = str(
            message.get("reasoning_content") or message.get("reasoning") or ""
        ).strip()
        text = _coerce_json_text(content) or _coerce_json_text(reasoning) or content
        if not text and reasoning:
            raise RuntimeError(
                "LM Studio returned empty content with reasoning_content — "
                "thinking consumed the token budget. Disable thinking in the "
                "LM Studio UI / model template, or raise max_tokens."
            )

        usage = result.get("usage", {}) or {}
        return ProviderResult(
            text=text,
            duration_seconds=time.monotonic() - started,
            input_tokens=_int_or_none(usage.get("prompt_tokens")),
            output_tokens=_int_or_none(usage.get("completion_tokens")),
            cache_read_tokens=None,
            cache_write_tokens=None,
            cost=None,
            raw_events=[result],
        )


def _strip_think_blocks(text: str) -> str:
    """Remove Qwen-style think blocks that may prefix the real answer."""
    open_tag = "<" + "think" + ">"
    close_tag = "</" + "think" + ">"
    while True:
        start = text.find(open_tag)
        if start < 0:
            break
        end = text.find(close_tag, start)
        if end < 0:
            text = text[:start]
            break
        text = text[:start] + text[end + len(close_tag) :]
    return text


def _coerce_json_text(text: str) -> str:
    """Return a JSON-looking payload from model text, or empty string."""
    cleaned = (text or "").strip()
    if not cleaned:
        return ""
    if cleaned.startswith("{") and cleaned.endswith("}"):
        return cleaned
    start, end = cleaned.find("{"), cleaned.rfind("}")
    if start >= 0 and end > start:
        return cleaned[start : end + 1]
    return ""


class OpenRouterProvider:
    """Direct OpenRouter API provider — supports multiple model providers via OpenRouter gateway.

    Calls https://openrouter.ai/api/v1/chat/completions with the specified model.
    Supports provider routing via the "provider" field in the request body or model config variant.
    """

    API_URL = "https://openrouter.ai/api/v1/chat/completions"
    API_KEY_ENV = "OPENROUTER_API_KEY"

    def __init__(
        self, timeout_seconds: int = 300, pure: bool = True
    ) -> None:
        self.timeout_seconds = timeout_seconds
        self.pure = pure
        _load_env_vars(Path(".env"))
        _load_env_vars(Path.home() / ".openrouter" / ".env")

    def invoke(self, prompt: str, model: ModelConfig, cwd: Path) -> ProviderResult:
        import time
        import subprocess
        import json
        import os

        started = time.monotonic()
        api_key = os.environ.get(self.API_KEY_ENV)
        if not api_key:
            raise RuntimeError(
                f"OpenRouter API key not found in ${self.API_KEY_ENV} — "
                f"set it in ~/.openrouter/.env or your shell profile"
            )

        # OpenRouter supports provider routing via the "provider" field
        # Use model.variant if set (e.g., "anthropic", "openai", "google"), otherwise let OpenRouter choose
        provider_routing = {}
        if model.variant:
            provider_routing["provider"] = {"order": [model.variant], "allow_fallbacks": False}

        request_body = json.dumps({
            "model": model.model,
            "messages": [{"role": "user", "content": prompt}],
            "temperature": 0.1,
            "max_tokens": int(model.max_tokens) if model.max_tokens else 4096,
            "response_format": {"type": "json_object"},
            **provider_routing,
        })

        cmd = [
            "curl", "-s", "-w", "\n%{http_code}",
            "-X", "POST", self.API_URL,
            "-H", "Content-Type: application/json",
            "-H", f"Authorization: Bearer {api_key}",
            "-H", "HTTP-Referer: https://github.com/xenoblade-coop/xenoblade",
            "-H", "X-Title: Xenoblade Co-op Decompilation",
            "-d", request_body,
        ]
        completed = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=self.timeout_seconds,
        )
        stdout = completed.stdout.strip()
        lines = stdout.rsplit("\n", 1)
        if len(lines) != 2:
            detail = (completed.stderr.strip() or stdout)[:2000]
            raise RuntimeError(f"OpenRouter API: unexpected curl output: {detail}")
        response_body, http_code = lines[0].strip(), lines[1]

        if completed.returncode or not http_code.startswith("2"):
            detail = (completed.stderr.strip() or response_body)[:2000]
            raise RuntimeError(
                f"OpenRouter API error (HTTP {http_code}): {detail}"
            )

        result = json.loads(response_body)
        choices = result.get("choices", [])
        if not choices:
            raise RuntimeError(
                f"OpenRouter API: no choices in response: {response_body[:500]}"
            )

        text = choices[0].get("message", {}).get("content", "").strip()
        usage = result.get("usage", {}) or {}

        return ProviderResult(
            text=text,
            duration_seconds=time.monotonic() - started,
            input_tokens=_int_or_none(usage.get("prompt_tokens")),
            output_tokens=_int_or_none(usage.get("completion_tokens")),
            cache_read_tokens=_int_or_none(
                usage.get("prompt_cache_hit_tokens")
                or (usage.get("prompt_tokens_details") or {}).get("cached_tokens")
            ),
            cache_write_tokens=_int_or_none(
                usage.get("prompt_cache_miss_tokens")
            ),
            cost=_float_or_none(usage.get("cost")),
            raw_events=[result],
        )


def _float_or_none(value: Any) -> Optional[float]:
    try:
        return float(value) if value is not None else None
    except (TypeError, ValueError):
        return None

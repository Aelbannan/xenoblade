"""OpenRouter provider adapter.

Sends OpenAI-format chat-completion requests to OpenRouter's API via
stdlib ``urllib``. Supports tool calling, thinking/reasoning budgets,
and automatic retry on transient HTTP errors.

API key resolution (in order):
1. ``OPENROUTER_API_KEY`` environment variable
2. ``~/.openrouter/.env`` file (``KEY=VALUE`` lines)
"""
from __future__ import annotations

import json
import os
import time
import urllib.error
import urllib.request
from pathlib import Path

from tools.llm_decomp.config import ModelConfig
from tools.llm_decomp.contracts import ToolCall
from .base import Provider, ProviderError, ProviderReply

_API_BASE = "https://openrouter.ai/api/v1/chat/completions"
_REFERER = "https://github.com/xenoblade-decomp"
_TITLE = "llm-decomp"
_TIMEOUT = 600
_RETRY_CODES = {429, 502, 503, 504}
_RETRY_BACKOFF = 5.0
_DOTENV_RELPATH = ".openrouter/.env"


def _resolve_api_key() -> str:
    """Resolve the OpenRouter API key from env or dotfile.

    Returns:
        The API key string.

    Raises:
        ProviderError: If no key is found, with a helpful message.
    """
    key = os.environ.get("OPENROUTER_API_KEY", "").strip()
    if key:
        return key
    for dotenv in (Path(".env"), Path.home() / _DOTENV_RELPATH):
        try:
            text = dotenv.read_text(encoding="utf-8")
        except (FileNotFoundError, OSError):
            continue
        for line in text.splitlines():
            line = line.strip()
            if line.startswith("OPENROUTER_API_KEY="):
                key = line.split("=", 1)[1].strip().strip('"').strip("'")
                if key:
                    return key
    raise ProviderError(
        "OpenRouter API key not found. Set OPENROUTER_API_KEY in your "
        "environment, add it to the repo .env, or create "
        "~/.openrouter/.env with the line:\n"
        "  OPENROUTER_API_KEY=sk-or-..."
    )


def _build_body(messages: list[dict], tools: list[dict], model: ModelConfig) -> dict:
    """Assemble the JSON request body per OpenRouter API requirements."""
    body: dict = {
        "model": model.model,
        "messages": messages,
        "tool_choice": "auto",
    }
    if model.max_tokens is not None:
        body["max_tokens"] = model.max_tokens
    if tools:
        body["tools"] = tools
    if model.thinking_budget is not None:
        reasoning: dict = {}
        if model.thinking_budget >= 0:
            reasoning["max_tokens"] = model.thinking_budget
        body["reasoning"] = reasoning
    return body


def _parse_response(data: dict) -> ProviderReply:
    """Parse an OpenRouter chat completion response into a ProviderReply."""
    usage_raw = data.get("usage") or {}
    usage = {
        "input_tokens": usage_raw.get("prompt_tokens", 0),
        "output_tokens": usage_raw.get("completion_tokens", 0),
    }
    choices = data.get("choices") or []
    if not choices:
        return ProviderReply(text="", usage=usage, finish_reason="stop")

    choice = choices[0]
    message = choice.get("message") or {}
    content = message.get("content")
    text = content if isinstance(content, str) else ""
    finish_reason = choice.get("finish_reason", "stop")

    raw_calls = message.get("tool_calls") or []
    tool_calls: list[ToolCall] = []
    for tc in raw_calls:
        fn = tc.get("function") or {}
        arguments_raw = fn.get("arguments", "{}")
        try:
            args = json.loads(arguments_raw)
        except (json.JSONDecodeError, TypeError):
            args = {"_raw": arguments_raw}
        tool_calls.append(ToolCall(name=fn.get("name", ""), args=args,
                                   id=str(tc.get("id", ""))))

    return ProviderReply(
        text=text,
        tool_calls=tool_calls,
        usage=usage,
        finish_reason=finish_reason,
    )


class OpenRouterProvider:
    """Provider adapter for OpenRouter's chat completions API.

    Uses ``urllib.request`` for HTTP. Configured via ``ModelConfig``;
    API key is resolved automatically from environment or dotfile.

    Example::

        provider = OpenRouterProvider()
        reply = provider.send(messages, tools, model_config)
    """

    def __init__(
        self,
        api_base: str = _API_BASE,
        timeout: int = _TIMEOUT,
    ) -> None:
        """Initialise the provider.

        Args:
            api_base: Override the API endpoint URL.
            timeout: Request timeout in seconds. Defaults to 600.
        """
        self._api_base = api_base
        self._timeout = timeout

    def send(
        self,
        messages: list[dict],
        tools: list[dict],
        model: ModelConfig,
    ) -> ProviderReply:
        """Send a conversation turn to OpenRouter.

        Args:
            messages: OpenAI-format message list.
            tools: OpenAI-format tool/function schemas.
            model: Model configuration specifying the model slug and options.

        Returns:
            A structured ProviderReply.

        Raises:
            ProviderError: On missing API key, HTTP errors after retry,
                or unparseable responses.
        """
        api_key = _resolve_api_key()
        body = _build_body(messages, tools, model)
        payload = json.dumps(body).encode("utf-8")
        req = urllib.request.Request(
            self._api_base,
            data=payload,
            headers={
                "Authorization": f"Bearer {api_key}",
                "Content-Type": "application/json",
                "HTTP-Referer": _REFERER,
                "X-Title": _TITLE,
            },
            method="POST",
        )

        last_error: Exception | None = None
        for attempt in range(2):  # initial + 1 retry
            try:
                with urllib.request.urlopen(req, timeout=self._timeout) as resp:
                    raw = resp.read().decode("utf-8")
                    data = json.loads(raw)
                return _parse_response(data)
            except urllib.error.HTTPError as exc:
                last_error = exc
                if exc.code in _RETRY_CODES and attempt == 0:
                    time.sleep(_RETRY_BACKOFF)
                    continue
                reason = exc.reason.decode("utf-8") if isinstance(exc.reason, bytes) else exc.reason
                body_snippet = ""
                try:
                    body_snippet = exc.read().decode("utf-8", errors="replace")[:500]
                except Exception:
                    pass
                raise ProviderError(
                    f"OpenRouter HTTP {exc.code} {reason}: {body_snippet}"
                ) from exc
            except (urllib.error.URLError, OSError) as exc:
                last_error = exc
                if attempt == 0:
                    time.sleep(_RETRY_BACKOFF)
                    continue
                raise ProviderError(
                    f"OpenRouter request failed after retry: {exc}"
                ) from exc
        # Should not be reached, but satisfies the type checker.
        raise ProviderError(
            f"OpenRouter request failed after retry: {last_error}"
        ) from last_error
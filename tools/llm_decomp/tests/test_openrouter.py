"""Tests for OpenRouter provider adapter.

Mocks ``urllib.request.urlopen`` to avoid real HTTP calls. Covers request
body shape (tools, max_tokens omission, reasoning budget), tool_calls
parsing including malformed JSON arguments, and API key resolution errors.
"""
from __future__ import annotations

import io
import json
import os
import unittest
from pathlib import Path
from unittest import mock

from tools.llm_decomp.config import ModelConfig
from tools.llm_decomp.providers.base import ProviderError
from tools.llm_decomp.providers.openrouter import OpenRouterProvider
from tools.llm_decomp.contracts import ToolCall


def _make_response(
    content: str | None = None,
    tool_calls: list[dict] | None = None,
    finish_reason: str = "stop",
    prompt_tokens: int = 10,
    completion_tokens: int = 5,
) -> bytes:
    """Build a JSON-encoded OpenRouter API response."""
    msg: dict = {}
    if content is not None:
        msg["content"] = content
    if tool_calls:
        msg["tool_calls"] = tool_calls

    data = {
        "choices": [{"message": msg, "finish_reason": finish_reason}],
        "usage": {
            "prompt_tokens": prompt_tokens,
            "completion_tokens": completion_tokens,
        },
    }
    return json.dumps(data).encode("utf-8")


def _mock_urlopen(response_bytes: bytes, status: int = 200) -> mock.MagicMock:
    """Return a MagicMock that behaves like a successful urlopen response."""
    cm = mock.MagicMock()
    cm.__enter__.return_value = io.BytesIO(response_bytes)
    cm.__enter__.return_value.status = status
    return cm


class OpenRouterProviderTests(unittest.TestCase):
    """Test suite for OpenRouterProvider."""

    def setUp(self) -> None:
        # Ensure OPENROUTER_API_KEY is set in env for tests that need it
        self._patcher = mock.patch.dict(os.environ, {"OPENROUTER_API_KEY": "sk-test-key"}, clear=True)
        self._patcher.start()
        self.provider = OpenRouterProvider()
        self.model = ModelConfig(
            provider="openrouter",
            model="deepseek/deepseek-chat",
            max_tokens=4096,
        )

    def tearDown(self) -> None:
        self._patcher.stop()

    # -- Basic send / parse -----------------------------------------------

    def test_send_parses_text_response(self) -> None:
        """A simple text response is parsed correctly."""
        response_bytes = _make_response(content="Hello, world!")
        with mock.patch("urllib.request.urlopen", return_value=_mock_urlopen(response_bytes)):
            reply = self.provider.send(
                [{"role": "user", "content": "hi"}],
                [],
                self.model,
            )
        self.assertEqual(reply.text, "Hello, world!")
        self.assertEqual(reply.tool_calls, [])
        self.assertEqual(reply.usage["input_tokens"], 10)
        self.assertEqual(reply.usage["output_tokens"], 5)
        self.assertEqual(reply.finish_reason, "stop")

    def test_send_null_content_becomes_empty_string(self) -> None:
        """Null content from a tool-only turn becomes ''."""
        tc = [{"id": "1", "type": "function", "function": {"name": "read_file", "arguments": '{"path":"x"}'}}]
        response_bytes = _make_response(content=None, tool_calls=tc)
        with mock.patch("urllib.request.urlopen", return_value=_mock_urlopen(response_bytes)):
            reply = self.provider.send(
                [{"role": "user", "content": "read"}],
                [],
                self.model,
            )
        self.assertEqual(reply.text, "")
        self.assertEqual(len(reply.tool_calls), 1)
        self.assertEqual(reply.tool_calls[0].name, "read_file")

    # -- Request body shape -----------------------------------------------

    def test_request_body_includes_tools(self) -> None:
        """Tools are included in the request body when provided."""
        tools = [
            {
                "type": "function",
                "function": {
                    "name": "read_file",
                    "parameters": {"type": "object", "properties": {}, "required": []},
                },
            }
        ]
        sentinel = {}

        def _check_urlopen(req, **kwargs):
            body = json.loads(req.data)
            self.assertIn("tools", body)
            self.assertEqual(len(body["tools"]), 1)
            self.assertEqual(body["tool_choice"], "auto")
            self.assertEqual(body["model"], "deepseek/deepseek-chat")
            self.assertEqual(body["max_tokens"], 4096)
            self.assertEqual(req.headers.get("Authorization"), "Bearer sk-test-key")
            lowered = {k.lower(): v for k, v in req.headers.items()}
            self.assertEqual(lowered.get("http-referer"), "https://github.com/xenoblade-decomp")
            self.assertEqual(lowered.get("x-title"), "llm-decomp")
            return _mock_urlopen(_make_response(content="ok"))

        with mock.patch("urllib.request.urlopen", _check_urlopen):
            self.provider.send(
                [{"role": "user", "content": "hello"}],
                tools,
                self.model,
            )

    def test_max_tokens_omitted_when_none(self) -> None:
        """max_tokens is absent from the body when ModelConfig.max_tokens is None."""
        model_no_max = ModelConfig(
            provider="openrouter",
            model="deepseek/deepseek-chat",
            max_tokens=None,
        )

        def _check(req, **kwargs):
            body = json.loads(req.data)
            self.assertNotIn("max_tokens", body)
            return _mock_urlopen(_make_response(content="ok"))

        with mock.patch("urllib.request.urlopen", _check):
            self.provider.send(
                [{"role": "user", "content": "hi"}],
                [],
                model_no_max,
            )

    def test_reasoning_present_when_thinking_budget_set(self) -> None:
        """reasoning block is present when thinking_budget is not None."""
        model_with_thinking = ModelConfig(
            provider="openrouter",
            model="openai/o3",
            max_tokens=8192,
            thinking_budget=2048,
        )

        def _check(req, **kwargs):
            body = json.loads(req.data)
            self.assertIn("reasoning", body)
            self.assertEqual(body["reasoning"]["max_tokens"], 2048)
            return _mock_urlopen(_make_response(content="ok"))

        with mock.patch("urllib.request.urlopen", _check):
            self.provider.send(
                [{"role": "user", "content": "think"}],
                [],
                model_with_thinking,
            )

    def test_reasoning_empty_dict_when_thinking_budget_minus_one(self) -> None:
        """reasoning: {} is sent when thinking_budget == -1 (unlimited)."""
        model_no_limit = ModelConfig(
            provider="openrouter",
            model="openai/o3",
            max_tokens=8192,
            thinking_budget=-1,
        )

        def _check(req, **kwargs):
            body = json.loads(req.data)
            self.assertIn("reasoning", body)
            self.assertEqual(body["reasoning"], {})
            return _mock_urlopen(_make_response(content="ok"))

        with mock.patch("urllib.request.urlopen", _check):
            self.provider.send(
                [{"role": "user", "content": "think unlimited"}],
                [],
                model_no_limit,
            )

    def test_no_tools_does_not_include_tools_key(self) -> None:
        """If tools list is empty, request body does not have a 'tools' key."""
        def _check(req, **kwargs):
            body = json.loads(req.data)
            self.assertNotIn("tools", body)
            return _mock_urlopen(_make_response(content="ok"))

        with mock.patch("urllib.request.urlopen", _check):
            self.provider.send(
                [{"role": "user", "content": "no tools"}],
                [],
                self.model,
            )

    # -- Tool calls parsing -----------------------------------------------

    def test_parses_tool_calls(self) -> None:
        """Tool calls in response are parsed into ToolCall objects."""
        raw_tc = [
            {
                "id": "call_1",
                "type": "function",
                "function": {
                    "name": "read_file",
                    "arguments": '{"path": "src/test.cpp", "start": 1, "count": 10}',
                },
            }
        ]
        response_bytes = _make_response(content=None, tool_calls=raw_tc, finish_reason="tool_calls")
        with mock.patch("urllib.request.urlopen", return_value=_mock_urlopen(response_bytes)):
            reply = self.provider.send(
                [{"role": "user", "content": "read"}],
                [],
                self.model,
            )
        self.assertEqual(len(reply.tool_calls), 1)
        tc = reply.tool_calls[0]
        self.assertIsInstance(tc, ToolCall)
        self.assertEqual(tc.name, "read_file")
        self.assertEqual(tc.args, {"path": "src/test.cpp", "start": 1, "count": 10})
        self.assertEqual(reply.finish_reason, "tool_calls")

    def test_tolerates_malformed_json_arguments(self) -> None:
        """Malformed JSON in function.arguments results in args={'_raw': ...}."""
        raw_tc = [
            {
                "id": "call_bad",
                "type": "function",
                "function": {
                    "name": "build",
                    "arguments": "{unit: 'broken'}",
                },
            }
        ]
        response_bytes = _make_response(content=None, tool_calls=raw_tc)
        with mock.patch("urllib.request.urlopen", return_value=_mock_urlopen(response_bytes)):
            reply = self.provider.send(
                [{"role": "user", "content": "build"}],
                [],
                self.model,
            )
        self.assertEqual(len(reply.tool_calls), 1)
        tc = reply.tool_calls[0]
        self.assertEqual(tc.name, "build")
        self.assertEqual(tc.args, {"_raw": "{unit: 'broken'}"})

    # -- Error handling ---------------------------------------------------

    def test_missing_api_key_raises_provider_error(self) -> None:
        """ProviderError when no API key is available."""
        provider = OpenRouterProvider()
        # Clear env and mock home to avoid dotenv fallback
        with mock.patch.dict(os.environ, {}, clear=True):
            with mock.patch.object(Path, "home", return_value=Path("/nonexistent/home")), \
                 mock.patch.object(Path, "read_text", side_effect=OSError("no file")):
                with self.assertRaises(ProviderError) as ctx:
                    provider.send(
                        [{"role": "user", "content": "hi"}],
                        [],
                        self.model,
                    )
        self.assertIn("OPENROUTER_API_KEY", str(ctx.exception))

    def test_http_429_retries_once_then_raises(self) -> None:
        """HTTP 429 is retried once; after retry, ProviderError is raised."""
        # Mock urlopen to raise HTTPError on first call, also on retry
        http_error = urllib.error.HTTPError(
            url="https://openrouter.ai/api/v1/chat/completions",
            code=429,
            msg="Too Many Requests",
            hdrs={},
            fp=io.BytesIO(b'{"error":"rate limited"}'),
        )

        call_count = [0]

        def _side_effect(*args, **kwargs):
            call_count[0] += 1
            raise http_error

        with mock.patch("urllib.request.urlopen", side_effect=_side_effect):
            with self.assertRaises(ProviderError):
                self.provider.send(
                    [{"role": "user", "content": "test"}],
                    [],
                    self.model,
                )
        self.assertEqual(call_count[0], 2)

    def test_http_401_raises_immediately_no_retry(self) -> None:
        """Non-retryable HTTP 401 raises ProviderError on first attempt."""
        http_error = urllib.error.HTTPError(
            url="https://openrouter.ai/api/v1/chat/completions",
            code=401,
            msg="Unauthorized",
            hdrs={},
            fp=io.BytesIO(b'{"error":"bad key"}'),
        )

        call_count = [0]

        def _side_effect(*args, **kwargs):
            call_count[0] += 1
            raise http_error

        with mock.patch("urllib.request.urlopen", side_effect=_side_effect):
            with self.assertRaises(ProviderError):
                self.provider.send(
                    [{"role": "user", "content": "test"}],
                    [],
                    self.model,
                )
        self.assertEqual(call_count[0], 1)

    def test_finish_reason_passthrough(self) -> None:
        """finish_reason is passed through from the API response."""
        response_bytes = _make_response(content="partial", finish_reason="length")
        with mock.patch("urllib.request.urlopen", return_value=_mock_urlopen(response_bytes)):
            reply = self.provider.send(
                [{"role": "user", "content": "go"}],
                [],
                self.model,
            )
        self.assertEqual(reply.finish_reason, "length")


# Import urllib.error at module level for the HTTP tests
import urllib.error


if __name__ == "__main__":
    unittest.main()
"""OpenCode provider / candidate-schema unit tests."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path
from typing import Any

from tools.llm_harness.candidate_schema import CANDIDATE_JSON_SCHEMA
from tools.llm_harness.providers import (
    LMStudioProvider,
    OpenCodeCircuitBreaker,
    OpenCodeEmptyResponse,
    OpenCodeProvider,
    parse_opencode_message,
)
from tools.llm_harness.types import ModelConfig


def _healthy_then_message(
    *,
    session_id: str = "ses_test",
    message_response: dict[str, Any] | None = None,
    capture: dict[str, Any] | None = None,
):
    """Build a fake `_request` that answers health/session/message/delete."""

    def fake_request(method: str, path: str, body=None, query=None):
        if capture is not None:
            if method == "POST" and path == "/session":
                capture["session_body"] = body
            if method == "POST" and path.endswith("/message"):
                capture["message_body"] = body
        if method == "GET" and path == "/global/health":
            return {"healthy": True, "version": "1.18.2"}
        if method == "POST" and path == "/session":
            return {"id": session_id}
        if method == "POST" and path.endswith("/message"):
            return message_response or {
                "info": {
                    "cost": 0,
                    "tokens": {"input": 1, "output": 1, "cache": {}},
                },
                "parts": [{"type": "text", "text": '{"source":"void f(){}"}'}],
            }
        if method == "DELETE":
            return True
        raise AssertionError(f"unexpected {method} {path}")

    return fake_request


class PureModeToolsTests(unittest.TestCase):
    def test_pure_disables_all_tools_even_with_cwd_files(self) -> None:
        provider = OpenCodeProvider(
            base_url="http://127.0.0.1:4096", timeout_seconds=30, pure=True
        )
        capture: dict[str, Any] = {}
        provider._request = _healthy_then_message(capture=capture)  # type: ignore[method-assign]

        with tempfile.TemporaryDirectory() as tmp:
            cwd = Path(tmp)
            (cwd / "TASK.md").write_text("# task\n", encoding="utf-8")
            (cwd / "dossier.json").write_text("{}", encoding="utf-8")
            provider.invoke(
                prompt="hi",
                model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                cwd=cwd,
            )

        tools = capture["message_body"]["tools"]
        for name in OpenCodeProvider._DENIED_TOOLS + OpenCodeProvider._READ_TOOLS:
            self.assertFalse(tools[name], msg=f"tool {name} should be False in pure mode")
        perms = {row["permission"]: row["action"] for row in capture["session_body"]["permission"]}
        for name in OpenCodeProvider._READ_TOOLS:
            self.assertEqual(perms[name], "deny")
        prompt_text = capture["message_body"]["parts"][0]["text"]
        self.assertIn("Do not use tools", prompt_text)
        self.assertIn("self-contained dossier", prompt_text)

    def test_non_pure_enables_read_tools_when_cwd_has_files(self) -> None:
        provider = OpenCodeProvider(
            base_url="http://127.0.0.1:4096", timeout_seconds=30, pure=False
        )
        capture: dict[str, Any] = {}
        provider._request = _healthy_then_message(capture=capture)  # type: ignore[method-assign]

        with tempfile.TemporaryDirectory() as tmp:
            cwd = Path(tmp)
            (cwd / "TASK.md").write_text("# task\n", encoding="utf-8")
            provider.invoke(
                prompt="hi",
                model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                cwd=cwd,
            )

        tools = capture["message_body"]["tools"]
        self.assertTrue(tools["read"])
        self.assertTrue(tools["glob"])
        self.assertTrue(tools["grep"])
        self.assertFalse(tools["bash"])
        self.assertNotIn("format", capture["message_body"])


class JsonSchemaFormatTests(unittest.TestCase):
    def test_pure_message_includes_json_schema_format(self) -> None:
        provider = OpenCodeProvider(
            base_url="http://127.0.0.1:4096", timeout_seconds=30, pure=True
        )
        capture: dict[str, Any] = {}
        provider._request = _healthy_then_message(capture=capture)  # type: ignore[method-assign]

        with tempfile.TemporaryDirectory() as tmp:
            provider.invoke(
                prompt="hi",
                model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                cwd=Path(tmp),
            )

        fmt = capture["message_body"]["format"]
        self.assertEqual(fmt["type"], "json_schema")
        self.assertEqual(fmt["retryCount"], 2)
        self.assertEqual(fmt["schema"], CANDIDATE_JSON_SCHEMA)
        self.assertEqual(fmt["schema"]["name"], "decomp_candidate")


class EmptyResponseTests(unittest.TestCase):
    def test_empty_response_raises_opencode_empty_response(self) -> None:
        provider = OpenCodeProvider(
            base_url="http://127.0.0.1:4096", timeout_seconds=30, pure=True
        )
        empty = {
            "info": {"tokens": {"input": 0, "output": 0, "cache": {}}},
            "parts": [],
        }
        provider._request = _healthy_then_message(  # type: ignore[method-assign]
            message_response=empty
        )

        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaises(OpenCodeEmptyResponse) as ctx:
                provider.invoke(
                    prompt="hi",
                    model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                    cwd=Path(tmp),
                )
        self.assertIn("empty assistant text", str(ctx.exception))
        self.assertIn("response_debug=", str(ctx.exception))
        self.assertIn("part_types", str(ctx.exception))

    def test_structured_output_avoids_empty_raise(self) -> None:
        provider = OpenCodeProvider(
            base_url="http://127.0.0.1:4096", timeout_seconds=30, pure=True
        )
        response = {
            "info": {
                "tokens": {"input": 5, "output": 2, "cache": {}},
                "structured_output": {
                    "source": "void f(){}",
                    "hypothesis": "h",
                    "notes": [],
                    "next_change": "",
                    "change": "",
                },
            },
            "parts": [],
        }
        provider._request = _healthy_then_message(  # type: ignore[method-assign]
            message_response=response
        )
        with tempfile.TemporaryDirectory() as tmp:
            result = provider.invoke(
                prompt="hi",
                model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                cwd=Path(tmp),
            )
        self.assertIn("void f()", result.text)

    def test_parse_prefers_structured_output(self) -> None:
        text, _, usage = parse_opencode_message(
            {
                "info": {
                    "tokens": {"input": 1, "output": 1, "cache": {}},
                    "structured_output": {"source": "x"},
                },
                "parts": [{"type": "reasoning", "text": "thinking"}],
            }
        )
        self.assertEqual(text, '{"source": "x"}')
        self.assertEqual(usage["input"], 1)


class SharedSchemaTests(unittest.TestCase):
    def test_lmstudio_uses_shared_schema(self) -> None:
        self.assertIs(LMStudioProvider.CANDIDATE_JSON_SCHEMA, CANDIDATE_JSON_SCHEMA)
        self.assertEqual(
            CANDIDATE_JSON_SCHEMA["schema"]["required"],
            ["source", "hypothesis", "notes", "next_change", "change"],
        )


class CircuitBreakerTests(unittest.TestCase):
    def test_opens_after_n_failures(self) -> None:
        breaker = OpenCodeCircuitBreaker(threshold=3)
        self.assertTrue(breaker.allow())
        breaker.record_failure()
        breaker.record_failure()
        self.assertTrue(breaker.allow())
        breaker.record_failure()
        self.assertFalse(breaker.allow())
        with self.assertRaisesRegex(RuntimeError, "circuit open"):
            breaker.raise_if_open()

    def test_success_resets(self) -> None:
        breaker = OpenCodeCircuitBreaker(threshold=2)
        breaker.record_failure()
        breaker.record_success()
        breaker.record_failure()
        self.assertTrue(breaker.allow())
        breaker.record_failure()
        self.assertFalse(breaker.allow())


if __name__ == "__main__":
    unittest.main()

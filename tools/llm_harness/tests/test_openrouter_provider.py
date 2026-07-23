"""OpenRouter provider unit tests."""

from __future__ import annotations

import json
import os
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.llm_harness.providers import OpenRouterProvider
from tools.llm_harness.types import ModelConfig


def _curl_ok(payload: dict, http_code: str = "200") -> SimpleNamespace:
    return SimpleNamespace(
        returncode=0,
        stdout=json.dumps(payload) + f"\n{http_code}",
        stderr="",
    )


class OpenRouterProviderTests(unittest.TestCase):
    def test_invoke_parses_openai_compatible_response(self) -> None:
        response = {
            "choices": [{"message": {"content": '{"source":"int f(){return 1;}"}'}}],
            "usage": {
                "prompt_tokens": 100,
                "completion_tokens": 20,
                "cost": 0.0012,
                "prompt_tokens_details": {"cached_tokens": 40},
            },
        }
        provider = OpenRouterProvider(timeout_seconds=30, api_key="test-key")
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ) as run:
            result = provider.invoke(
                '{"task":"decompile"}',
                ModelConfig(
                    id="or",
                    provider="openrouter",
                    model="deepseek/deepseek-chat",
                ),
                Path("."),
            )
        self.assertEqual(result.text, '{"source":"int f(){return 1;}"}')
        self.assertEqual(result.input_tokens, 100)
        self.assertEqual(result.output_tokens, 20)
        self.assertEqual(result.cache_read_tokens, 40)
        self.assertAlmostEqual(result.cost or 0.0, 0.0012)
        cmd = run.call_args.args[0]
        self.assertIn("https://openrouter.ai/api/v1/chat/completions", cmd)
        self.assertIn("Authorization: Bearer test-key", cmd)
        body = json.loads(cmd[cmd.index("-d") + 1])
        self.assertEqual(body["model"], "deepseek/deepseek-chat")
        self.assertEqual(body["response_format"]["type"], "json_object")
        self.assertNotIn("provider", body)
        self.assertNotIn("reasoning", body)

    def test_variant_sets_provider_routing(self) -> None:
        response = {"choices": [{"message": {"content": "{}"}}], "usage": {}}
        provider = OpenRouterProvider(
            api_key="k",
            allow_fallbacks=False,
            json_object=True,
        )
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ) as run:
            provider.invoke(
                "{}",
                ModelConfig(
                    id="or",
                    provider="openrouter",
                    model="anthropic/claude-sonnet-4",
                    variant="anthropic",
                ),
                Path("."),
            )
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertEqual(
            body["provider"],
            {"order": ["anthropic"], "allow_fallbacks": False},
        )

    def test_reasoning_effort_and_budget(self) -> None:
        response = {"choices": [{"message": {"content": "{}"}}], "usage": {}}
        provider = OpenRouterProvider(api_key="k")
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ) as run:
            provider.invoke(
                "{}",
                ModelConfig(
                    id="or",
                    provider="openrouter",
                    model="openai/o3",
                    reasoning_effort="high",
                    thinking_budget=2048,
                ),
                Path("."),
            )
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertEqual(body["reasoning"], {"effort": "high", "max_tokens": 2048})

    def test_json_object_can_be_disabled(self) -> None:
        response = {"choices": [{"message": {"content": "ok"}}], "usage": {}}
        provider = OpenRouterProvider(api_key="k", json_object=False)
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ) as run:
            provider.invoke(
                "hi",
                ModelConfig(id="or", provider="openrouter", model="m"),
                Path("."),
            )
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertNotIn("response_format", body)

    def test_multipart_content_and_think_blocks(self) -> None:
        response = {
            "choices": [
                {
                    "message": {
                        "content": [
                            {"type": "text", "text": "<think>scratch</think>"},
                            {"type": "text", "text": '{"source":"void g(){}"}'},
                        ]
                    }
                }
            ],
            "usage": {},
        }
        provider = OpenRouterProvider(api_key="k")
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ):
            result = provider.invoke(
                "{}",
                ModelConfig(id="or", provider="openrouter", model="m"),
                Path("."),
            )
        self.assertEqual(result.text, '{"source":"void g(){}"}')

    def test_missing_api_key_raises(self) -> None:
        provider = OpenRouterProvider(api_key="")
        env = {k: v for k, v in os.environ.items() if k != "OPENROUTER_API_KEY"}
        with mock.patch.dict(os.environ, env, clear=True):
            with self.assertRaises(RuntimeError) as ctx:
                provider.invoke(
                    "{}",
                    ModelConfig(id="or", provider="openrouter", model="m"),
                    Path("."),
                )
        self.assertIn("OPENROUTER_API_KEY", str(ctx.exception))

    def test_http_error_raises(self) -> None:
        provider = OpenRouterProvider(api_key="k")
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok({"error": {"message": "nope"}}, http_code="401"),
        ):
            with self.assertRaises(RuntimeError) as ctx:
                provider.invoke(
                    "{}",
                    ModelConfig(id="or", provider="openrouter", model="m"),
                    Path("."),
                )
        self.assertIn("HTTP 401", str(ctx.exception))


if __name__ == "__main__":
    unittest.main()

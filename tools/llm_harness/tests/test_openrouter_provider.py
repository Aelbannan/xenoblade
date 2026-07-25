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


# Minimal valid schema for testing (avoids the full CANDIDATE_JSON_SCHEMA import)
_TEST_SCHEMA = {
    "name": "decomp_candidate",
    "strict": True,
    "schema": {
        "type": "object",
        "properties": {
            "source": {"type": "string"},
            "hypothesis": {"type": "string"},
        },
        "required": ["source", "hypothesis"],
        "additionalProperties": False,
    },
}


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

    def test_json_schema_sends_candidate_schema(self) -> None:
        response = {"choices": [{"message": {"content": '{"source":"int f(){}","hypothesis":"ok"}'}}], "usage": {}}
        provider = OpenRouterProvider(api_key="k", json_object=False, json_schema=True)
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ) as run:
            provider.invoke(
                "test",
                ModelConfig(id="or", provider="openrouter", model="m"),
                Path("."),
            )
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        rf = body["response_format"]
        self.assertEqual(rf["type"], "json_schema")
        self.assertIn("json_schema", rf)
        schema = rf["json_schema"]
        self.assertEqual(schema["name"], "decomp_candidate")
        self.assertTrue(schema["strict"])
        props = schema["schema"]["properties"]
        self.assertIn("source", props)
        self.assertIn("hypothesis", props)
        self.assertIn("notes", props)
        self.assertIn("next_change", props)
        self.assertIn("change", props)

    def test_json_schema_takes_priority_over_json_object(self) -> None:
        """When both json_schema and json_object are True, json_schema wins."""
        response = {"choices": [{"message": {"content": '{"source":"int f(){}","hypothesis":"ok"}'}}], "usage": {}}
        provider = OpenRouterProvider(api_key="k", json_object=True, json_schema=True)
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ) as run:
            provider.invoke(
                "test",
                ModelConfig(id="or", provider="openrouter", model="m"),
                Path("."),
            )
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertEqual(body["response_format"]["type"], "json_schema")

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

    def test_both_disabled_no_response_format(self) -> None:
        response = {"choices": [{"message": {"content": "ok"}}], "usage": {}}
        provider = OpenRouterProvider(api_key="k", json_object=False, json_schema=False)
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


    def test_finish_reason_captured(self) -> None:
        response = {
            "choices": [
                {"message": {"content": "{}"}, "finish_reason": "length"}
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
        self.assertEqual(result.finish_reason, "length")

    def test_unlimited_output_omits_max_tokens(self) -> None:
        response = {"choices": [{"message": {"content": "{}"}}], "usage": {}}
        provider = OpenRouterProvider(api_key="k")
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ) as run:
            provider.invoke(
                "{}",
                ModelConfig(
                    id="or", provider="openrouter", model="m",
                    unlimited_output=True,
                ),
                Path("."),
            )
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertNotIn("max_tokens", body)

    def test_unlimited_thinking_omits_reasoning_budget(self) -> None:
        response = {"choices": [{"message": {"content": "{}"}}], "usage": {}}
        provider = OpenRouterProvider(api_key="k")
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ) as run:
            provider.invoke(
                "{}",
                ModelConfig(
                    id="or", provider="openrouter", model="m",
                    reasoning_effort="high",
                    unlimited_thinking=True,
                ),
                Path("."),
            )
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertEqual(body["reasoning"], {"effort": "high"})

    def test_empty_content_with_reasoning_raises_terminal_error(self) -> None:
        # Reasoning models can burn the whole completion budget on hidden
        # thinking, returning empty content. This must surface as the
        # "empty assistant text" error class so the harness treats it as
        # terminal and refunds repair budget.
        response = {
            "choices": [
                {
                    "message": {"content": "", "reasoning": "thinking..."},
                    "finish_reason": "length",
                }
            ],
            "usage": {},
        }
        provider = OpenRouterProvider(api_key="k")
        with mock.patch(
            "tools.llm_harness.providers.subprocess.run",
            return_value=_curl_ok(response),
        ):
            with self.assertRaises(RuntimeError) as ctx:
                provider.invoke(
                    "{}",
                    ModelConfig(id="or", provider="openrouter", model="m"),
                    Path("."),
                )
        self.assertIn("empty assistant text", str(ctx.exception))
        self.assertIn("finish_reason=length", str(ctx.exception))


if __name__ == "__main__":
    unittest.main()

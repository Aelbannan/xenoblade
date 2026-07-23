"""OpenCode provider / candidate-schema unit tests."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.llm_harness.candidate_schema import CANDIDATE_JSON_SCHEMA
from tools.llm_harness.providers import (
    LMStudioProvider,
    OpenCodeCircuitBreaker,
    OpenCodeEmptyResponse,
    OpenCodeProvider,
    parse_opencode_message,
)
from tools.llm_harness.types import ModelConfig


def _cli_json_stdout(text: str = '{"source":"void f(){}"}', *, input_tokens: int = 9, output_tokens: int = 3) -> str:
    return "\n".join(
        [
            json.dumps(
                {
                    "type": "step_finish",
                    "part": {
                        "type": "step-finish",
                        "cost": 0.02,
                        "tokens": {
                            "input": input_tokens,
                            "output": output_tokens,
                            "cache": {"read": 0, "write": 0},
                        },
                    },
                }
            ),
            json.dumps({"type": "text", "part": {"type": "text", "text": text}}),
        ]
    )


class PureModeCliTests(unittest.TestCase):
    def test_pure_passes_pure_flag_and_self_contained_guardrail(self) -> None:
        provider = OpenCodeProvider(timeout_seconds=30, pure=True)
        completed = SimpleNamespace(returncode=0, stdout=_cli_json_stdout(), stderr="")
        with tempfile.TemporaryDirectory() as tmp:
            cwd = Path(tmp)
            (cwd / "TASK.md").write_text("# task\n", encoding="utf-8")
            with mock.patch(
                "tools.llm_harness.providers.subprocess.run", return_value=completed
            ) as run:
                provider.invoke(
                    prompt="hi",
                    model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                    cwd=cwd,
                )
        cmd = run.call_args.args[0]
        self.assertIn("--pure", cmd)
        self.assertIn("Do not use tools", cmd[-1])
        self.assertIn("self-contained dossier", cmd[-1])

    def test_non_pure_allows_context_guardrail_when_cwd_has_files(self) -> None:
        provider = OpenCodeProvider(timeout_seconds=30, pure=False)
        completed = SimpleNamespace(returncode=0, stdout=_cli_json_stdout(), stderr="")
        with tempfile.TemporaryDirectory() as tmp:
            cwd = Path(tmp)
            (cwd / "TASK.md").write_text("# task\n", encoding="utf-8")
            with mock.patch(
                "tools.llm_harness.providers.subprocess.run", return_value=completed
            ) as run:
                provider.invoke(
                    prompt="hi",
                    model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                    cwd=cwd,
                )
        cmd = run.call_args.args[0]
        self.assertNotIn("--pure", cmd)
        self.assertIn("curated files", cmd[-1])


class EmptyResponseTests(unittest.TestCase):
    def test_empty_response_raises_opencode_empty_response(self) -> None:
        provider = OpenCodeProvider(timeout_seconds=30, pure=True)
        completed = SimpleNamespace(returncode=0, stdout="", stderr="")
        with tempfile.TemporaryDirectory() as tmp:
            with mock.patch(
                "tools.llm_harness.providers.subprocess.run", return_value=completed
            ):
                with self.assertRaises(OpenCodeEmptyResponse) as ctx:
                    provider.invoke(
                        prompt="hi",
                        model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                        cwd=Path(tmp),
                    )
        self.assertIn("empty assistant text", str(ctx.exception))
        self.assertIn("stdout_chars=", str(ctx.exception))

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

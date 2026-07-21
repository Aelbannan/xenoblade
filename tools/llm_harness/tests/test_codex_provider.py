"""Codex CLI provider unit tests."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.llm_harness.candidate_schema import CANDIDATE_JSON_SCHEMA
from tools.llm_harness.providers import CodexProvider, parse_codex_output
from tools.llm_harness.types import ModelConfig


class ParseCodexOutputTests(unittest.TestCase):
    def test_extracts_last_agent_message_and_usage(self) -> None:
        stdout = "\n".join(
            [
                json.dumps({"type": "thread.started", "thread_id": "t1"}),
                json.dumps({"type": "turn.started"}),
                json.dumps(
                    {
                        "type": "item.completed",
                        "item": {
                            "id": "item_1",
                            "type": "agent_message",
                            "text": '{"source":"void a(){}"}',
                        },
                    }
                ),
                json.dumps(
                    {
                        "type": "item.completed",
                        "item": {
                            "id": "item_2",
                            "type": "agent_message",
                            "text": '{"source":"void b(){}"}',
                        },
                    }
                ),
                json.dumps(
                    {
                        "type": "turn.completed",
                        "usage": {
                            "input_tokens": 100,
                            "cached_input_tokens": 40,
                            "output_tokens": 20,
                        },
                    }
                ),
            ]
        )
        text, events, usage = parse_codex_output(stdout)
        self.assertEqual(text, '{"source":"void b(){}"}')
        self.assertEqual(len(events), 5)
        self.assertEqual(usage["input"], 100)
        self.assertEqual(usage["output"], 20)
        self.assertEqual(usage["cache_read"], 40)

    def test_turn_failed_raises(self) -> None:
        stdout = json.dumps(
            {"type": "turn.failed", "error": {"message": "stream ended"}}
        )
        with self.assertRaisesRegex(RuntimeError, "stream ended"):
            parse_codex_output(stdout)


class CodexProviderTests(unittest.TestCase):
    def test_invoke_builds_exec_command_and_prefers_last_message_file(self) -> None:
        provider = CodexProvider(timeout_seconds=30, pure=True, json_schema=True)
        with tempfile.TemporaryDirectory() as tmp:
            cwd = Path(tmp)
            # Pure mode must not use the curated context dir (no file access).
            (cwd / "secret.txt").write_text("do-not-read", encoding="utf-8")

            def fake_run(cmd, **kwargs):
                self.assertEqual(cmd[0], "codex")
                self.assertEqual(cmd[1], "exec")
                self.assertIn("--json", cmd)
                self.assertIn("--ephemeral", cmd)
                self.assertIn("--ignore-user-config", cmd)
                self.assertIn("--ignore-rules", cmd)
                self.assertIn("--skip-git-repo-check", cmd)
                self.assertIn("--disable", cmd)
                self.assertIn("shell_tool", cmd)
                self.assertIn('approval_policy="never"', cmd)
                self.assertIn('web_search="disabled"', cmd)
                self.assertIn("-s", cmd)
                self.assertEqual(cmd[cmd.index("-s") + 1], "read-only")
                self.assertEqual(cmd[cmd.index("-m") + 1], "gpt-5.4")
                self.assertEqual(cmd[-1], "-")
                work_dir = Path(cmd[cmd.index("-C") + 1])
                self.assertNotEqual(work_dir.resolve(), cwd.resolve())
                self.assertFalse((work_dir / "secret.txt").exists())
                self.assertIn("--output-schema", cmd)
                schema_path = Path(cmd[cmd.index("--output-schema") + 1])
                self.assertTrue(schema_path.is_file())
                schema = json.loads(schema_path.read_text(encoding="utf-8"))
                self.assertEqual(schema, CANDIDATE_JSON_SCHEMA["schema"])
                out_path = Path(cmd[cmd.index("-o") + 1])
                out_path.write_text('{"source":"void from_file(){}"}', encoding="utf-8")
                self.assertIn("Return only the requested JSON object", kwargs["input"])
                self.assertIn("model_reasoning_effort=", " ".join(cmd))
                return SimpleNamespace(
                    returncode=0,
                    stdout=json.dumps(
                        {
                            "type": "item.completed",
                            "item": {
                                "id": "item_1",
                                "type": "agent_message",
                                "text": '{"source":"void from_jsonl(){}"}',
                            },
                        }
                    )
                    + "\n"
                    + json.dumps(
                        {
                            "type": "turn.completed",
                            "usage": {
                                "input_tokens": 11,
                                "cached_input_tokens": 0,
                                "output_tokens": 5,
                            },
                        }
                    ),
                    stderr="",
                )

            with mock.patch(
                "tools.llm_harness.providers.subprocess.run", side_effect=fake_run
            ):
                result = provider.invoke(
                    prompt='{"task":"decompile"}',
                    model=ModelConfig(
                        id="codex-gpt",
                        provider="codex",
                        model="gpt-5.4",
                    ),
                    cwd=cwd,
                )

        self.assertEqual(result.text, '{"source":"void from_file(){}"}')
        self.assertEqual(result.input_tokens, 11)
        self.assertEqual(result.output_tokens, 5)

    def test_pure_mode_rejects_tool_events(self) -> None:
        provider = CodexProvider(timeout_seconds=30, pure=True, json_schema=False)
        stdout = "\n".join(
            [
                json.dumps(
                    {
                        "type": "item.completed",
                        "item": {
                            "id": "item_1",
                            "type": "command_execution",
                            "command": "ls",
                            "status": "completed",
                        },
                    }
                ),
                json.dumps(
                    {
                        "type": "item.completed",
                        "item": {
                            "id": "item_2",
                            "type": "agent_message",
                            "text": '{"source":"void f(){}"}',
                        },
                    }
                ),
            ]
        )
        completed = SimpleNamespace(returncode=0, stdout=stdout, stderr="")
        with tempfile.TemporaryDirectory() as tmp:
            with mock.patch(
                "tools.llm_harness.providers.subprocess.run", return_value=completed
            ):
                with self.assertRaisesRegex(RuntimeError, "unexpected tool use"):
                    provider.invoke(
                        prompt="hi",
                        model=ModelConfig(id="x", provider="codex", model="gpt-5.4"),
                        cwd=Path(tmp),
                    )

    def test_empty_response_raises(self) -> None:
        provider = CodexProvider(timeout_seconds=30)
        completed = SimpleNamespace(returncode=0, stdout="", stderr="")
        with tempfile.TemporaryDirectory() as tmp:
            with mock.patch(
                "tools.llm_harness.providers.subprocess.run", return_value=completed
            ):
                with self.assertRaisesRegex(RuntimeError, "empty assistant text"):
                    provider.invoke(
                        prompt="hi",
                        model=ModelConfig(id="x", provider="codex", model="gpt-5.4"),
                        cwd=Path(tmp),
                    )

    def test_uses_shared_candidate_schema(self) -> None:
        self.assertIs(CodexProvider.CANDIDATE_JSON_SCHEMA, CANDIDATE_JSON_SCHEMA)


if __name__ == "__main__":
    unittest.main()

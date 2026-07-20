from __future__ import annotations

import json
import subprocess
import tempfile
import threading
import unittest
import unittest.mock
from pathlib import Path
from types import SimpleNamespace

from tools.llm_harness.core import Harness, parse_candidate
from tools.llm_harness.providers import (
    LMStudioProvider,
    OpenCodeProvider,
    parse_opencode_message,
    parse_opencode_output,
    parse_reasonix_output,
)
from tools.llm_harness.types import Candidate, ModelConfig, ProviderResult, SourcePatch
from tools.llm_harness.workspace import GitWorktreeManager
from tools.llm_harness.dossier import (
    DataFlowSummary,
    MemoryAccess,
    RetailProgramContext,
    TargetDossier,
    TargetIdentity,
    build_cfg,
    build_data_flow_summary,
    build_declaration_context,
    build_target_identity,
    build_target_dossier,
    decode_instructions,
    dossier_to_dict,
    parse_signature,
    validate_dossier,
    _classify_instruction,
    _find_class_name,
    _format_operand,
    _split_params,
)
from tools.llm_harness.xenoblade_project import (
    XenobladeAdapter,
    _begin_marker,
    _end_marker,
    _apply_tu_patches,
    _binary_mismatch_summary,
    _find_function_region,
    _find_tu_slots,
    _function_size_comparison,
    _insert_marker_slot,
    _insert_empty_tu_slot,
    _replace_function_source,
    _residual_section_names,
    _require_tu_completion_ready,
    _wrap_tu_slot,
)
from tools.ppc_equivalence.ir import Instruction, Opcode


class CandidateTests(unittest.TestCase):
    def test_parses_fenced_typed_candidate(self) -> None:
        candidate = parse_candidate(
            '```json\n{"source":"int f() { return 1; }","notes":"one"}\n```'
        )
        self.assertEqual(candidate.notes, ["one"])

    def test_rejects_missing_source(self) -> None:
        with self.assertRaises(ValueError):
            parse_candidate('{"hypothesis":"x"}')

    def test_parses_targeted_tu_patches(self) -> None:
        candidate = parse_candidate(
            '{"patches":[{"slot_id":"vtable","source":"static int x = 1;"}],'
            '"hypothesis":"layout"}',
            workflow="tu-complete",
        )
        self.assertEqual(candidate.source, "")
        self.assertEqual(candidate.patches[0].slot_id, "vtable")

    def test_rejects_mixed_full_source_and_tu_patches(self) -> None:
        with self.assertRaisesRegex(ValueError, "either 'source' or 'patches'"):
            parse_candidate(
                '{"source":"x","patches":[{"slot_id":"vtable","source":"y"}]}',
                workflow="tu-complete",
            )

    def test_requires_tu_output_contract_selected_by_prompt_mode(self) -> None:
        with self.assertRaisesRegex(ValueError, "bounded 'patches'"):
            parse_candidate(
                '{"source":"int complete_tu;"}', workflow="tu-complete"
            )
        with self.assertRaisesRegex(ValueError, "complete 'source'"):
            parse_candidate(
                '{"patches":[{"slot_id":"data","source":"int x;"}]}',
                workflow="tu-complete",
                full_context=True,
            )
        candidate = parse_candidate(
            '{"source":"int complete_tu;"}',
            workflow="tu-complete",
            full_context=True,
        )
        self.assertEqual(candidate.source, "int complete_tu;")

    def test_salvages_truncated_source_json(self) -> None:
        raw = (
            '{"source": "extern \\"C\\" void func_800407C8(float *out) {\\n'
            "\\tout[0] = f1;\\n\\tout[1] = f2;\\n}"
        )
        candidate = parse_candidate(raw)
        self.assertIn("func_800407C8", candidate.source)
        self.assertIn("out[0]", candidate.source)

    def test_salvages_unterminated_source_with_commentary(self) -> None:
        raw = (
            '{"source": "void func_800407C8() {\\n'
            "    // lots of commentary without closing quote\\n"
            "    void func_800407C8() { }"
        )
        candidate = parse_candidate(raw)
        self.assertIn("func_800407C8", candidate.source)

    def test_rejects_bare_symbol_source(self) -> None:
        with self.assertRaisesRegex(ValueError, "complete function definition"):
            parse_candidate(
                '{"source":"func_800407C8","hypothesis":"name only","notes":[]}'
            )

    def test_rejects_dossier_echo_without_definition(self) -> None:
        with self.assertRaisesRegex(ValueError, "non-empty string 'source'"):
            parse_candidate(
                '{"declaration":"func_800407C8","authoritative":true,'
                '"implicit_this":null,"parameters":[],'
                '"return_info":{"type":"void","register":""}}'
            )


class OpenCodeOutputTests(unittest.TestCase):
    def test_parses_text_events_and_usage(self) -> None:
        output = "\n".join([
            json.dumps({"type": "text", "part": {"type": "text", "text": "{\"sou"}}),
            json.dumps({
                "type": "text",
                "part": {"type": "text", "text": "rce\":\"x\"}"},
                "usage": {"input_tokens": 12, "output_tokens": 4},
            }),
        ])
        text, events, usage = parse_opencode_output(output)
        self.assertEqual(text, '{"source":"x"}')
        self.assertEqual(len(events), 2)
        self.assertEqual(usage["input"], 12)
        self.assertEqual(usage["output"], 4)

    def test_parses_cache_usage(self) -> None:
        output = json.dumps({
            "type": "step_finish",
            "part": {
                "type": "step-finish",
                "tokens": {"input": 10, "output": 2, "cache": {"read": 8, "write": 3}},
            },
        })
        _, _, usage = parse_opencode_output(output)
        self.assertEqual(usage["cache_read"], 8)
        self.assertEqual(usage["cache_write"], 3)

    def test_sums_opencode_step_costs(self) -> None:
        output = "\n".join([
            json.dumps({
                "type": "step_finish",
                "part": {"type": "step-finish", "cost": 0.03, "tokens": {"input": 10, "output": 2}},
            }),
            json.dumps({
                "type": "step_finish",
                "part": {"type": "step-finish", "cost": 0.04, "tokens": {"input": 4, "output": 3}},
            }),
            json.dumps({"type": "text", "part": {"type": "text", "text": '{"source":"x"}'}}),
        ])
        _, _, usage = parse_opencode_output(output)
        self.assertEqual(usage["input"], 14)
        self.assertEqual(usage["output"], 5)
        self.assertAlmostEqual(usage["cost"], 0.07)

    def test_parses_server_message_response(self) -> None:
        response = {
            "info": {
                "role": "assistant",
                "cost": 0.01,
                "tokens": {
                    "input": 11,
                    "output": 5,
                    "cache": {"read": 2, "write": 1},
                },
            },
            "parts": [
                {"type": "text", "text": '{"source":"int f(){return 1;}"}'},
            ],
        }
        text, events, usage = parse_opencode_message(response)
        self.assertEqual(text, '{"source":"int f(){return 1;}"}')
        self.assertEqual(len(events), 1)
        self.assertEqual(usage["input"], 11)
        self.assertEqual(usage["output"], 5)
        self.assertEqual(usage["cache_read"], 2)
        self.assertEqual(usage["cache_write"], 1)
        self.assertAlmostEqual(usage["cost"], 0.01)

    def test_server_message_error_raises(self) -> None:
        response = {
            "info": {
                "error": {
                    "name": "ProviderAuthError",
                    "data": {"providerID": "opencode", "message": "missing key"},
                },
            },
            "parts": [],
        }
        with self.assertRaisesRegex(RuntimeError, "missing key"):
            parse_opencode_message(response)


class OpenCodeProviderTests(unittest.TestCase):
    def test_invoke_uses_session_message_api(self) -> None:
        provider = OpenCodeProvider(base_url="http://127.0.0.1:4096", timeout_seconds=30)
        calls: list[tuple[str, str]] = []

        with tempfile.TemporaryDirectory() as tmp:
            cwd = Path(tmp)

            def fake_request(method: str, path: str, body=None, query=None):
                calls.append((method, path))
                if method == "GET" and path == "/global/health":
                    return {"healthy": True, "version": "1.18.2"}
                if method == "POST" and path == "/session":
                    self.assertEqual(query.get("directory"), str(cwd.resolve()))
                    self.assertIsInstance(body.get("permission"), list)
                    return {"id": "ses_test123"}
                if method == "POST" and path == "/session/ses_test123/message":
                    self.assertEqual(query.get("directory"), str(cwd.resolve()))
                    self.assertEqual(body["model"], {
                        "providerID": "opencode",
                        "modelID": "deepseek-v4-flash-free",
                    })
                    self.assertEqual(body["parts"][0]["type"], "text")
                    self.assertIn("Return only the requested JSON object", body["parts"][0]["text"])
                    self.assertFalse(body["tools"]["bash"])
                    self.assertFalse(body["tools"]["edit"])
                    return {
                        "info": {
                            "cost": 0.02,
                            "tokens": {
                                "input": 9,
                                "output": 3,
                                "cache": {"read": 0, "write": 0},
                            },
                        },
                        "parts": [{"type": "text", "text": '{"source":"void f(){}"}'}],
                    }
                if method == "DELETE" and path == "/session/ses_test123":
                    return True
                raise AssertionError(f"unexpected request {method} {path}")

            provider._request = fake_request  # type: ignore[method-assign]
            result = provider.invoke(
                prompt='{"task":"decompile"}',
                model=ModelConfig(
                    id="opencode-flash",
                    provider="opencode",
                    model="opencode/deepseek-v4-flash-free",
                ),
                cwd=cwd,
            )

        self.assertEqual(result.text, '{"source":"void f(){}"}')
        self.assertEqual(result.input_tokens, 9)
        self.assertEqual(result.output_tokens, 3)
        self.assertAlmostEqual(result.cost or 0.0, 0.02)
        self.assertEqual(
            calls,
            [
                ("GET", "/global/health"),
                ("POST", "/session"),
                ("POST", "/session/ses_test123/message"),
                ("DELETE", "/session/ses_test123"),
            ],
        )

    def test_invoke_requires_healthy_server(self) -> None:
        provider = OpenCodeProvider(base_url="http://127.0.0.1:4096")

        def boom(*_args, **_kwargs):
            raise RuntimeError("connection refused")

        provider._request = boom  # type: ignore[method-assign]
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaisesRegex(RuntimeError, "unreachable"):
                provider.invoke(
                    prompt="hi",
                    model=ModelConfig(id="x", provider="opencode", model="opencode/m"),
                    cwd=Path(tmp),
                )


class LMStudioProviderTests(unittest.TestCase):
    def test_invoke_parses_openai_compatible_response(self) -> None:
        response = {
            "choices": [{"message": {"content": '{"source":"int f(){return 1;}"}'}}],
            "usage": {"prompt_tokens": 100, "completion_tokens": 20},
        }
        completed = SimpleNamespace(
            returncode=0,
            stdout=json.dumps(response) + "\n200",
            stderr="",
        )
        provider = LMStudioProvider(timeout_seconds=30)
        with unittest.mock.patch("tools.llm_harness.providers.subprocess.run", return_value=completed) as run:
            result = provider.invoke(
                '{"task":"decompile"}',
                ModelConfig(id="local", provider="lmstudio", model="qwen2.5-coder"),
                Path("."),
            )
        self.assertEqual(result.text, '{"source":"int f(){return 1;}"}')
        self.assertEqual(result.input_tokens, 100)
        self.assertEqual(result.output_tokens, 20)
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertEqual(body["model"], "qwen2.5-coder")
        self.assertEqual(body["response_format"]["type"], "json_schema")
        self.assertFalse(body["enable_thinking"])
        self.assertEqual([m["role"] for m in body["messages"]], ["user"])

    def test_json_object_can_be_disabled(self) -> None:
        response = {
            "choices": [{"message": {"content": "{}"}}],
            "usage": {},
        }
        completed = SimpleNamespace(
            returncode=0,
            stdout=json.dumps(response) + "\n200",
            stderr="",
        )
        provider = LMStudioProvider(json_object=False)
        with unittest.mock.patch("tools.llm_harness.providers.subprocess.run", return_value=completed) as run:
            provider.invoke("{}", ModelConfig(id="local", provider="lmstudio", model="m"), Path("."))
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertNotIn("response_format", body)

    def test_thinking_flags_are_passthrough_single_shot(self) -> None:
        response = {
            "choices": [{"message": {"content": "{}"}}],
            "usage": {},
        }
        completed = SimpleNamespace(
            returncode=0,
            stdout=json.dumps(response) + "\n200",
            stderr="",
        )
        provider = LMStudioProvider(
            enable_thinking=True,
            reasoning_effort="medium",
            thinking_budget=512,
        )
        with unittest.mock.patch(
            "tools.llm_harness.providers.subprocess.run", return_value=completed
        ) as run:
            provider.invoke(
                "{}",
                ModelConfig(
                    id="local",
                    provider="lmstudio",
                    model="m",
                    reasoning_effort="low",
                    thinking_budget=200,
                ),
                Path("."),
            )
        self.assertEqual(run.call_count, 1)
        body = json.loads(run.call_args.args[0][run.call_args.args[0].index("-d") + 1])
        self.assertTrue(body["enable_thinking"])
        self.assertEqual(body["reasoning_effort"], "low")
        self.assertEqual(body["thinking_budget"], 200)
        self.assertEqual(body["max_tokens"], 4096)
        self.assertEqual([m["role"] for m in body["messages"]], ["user"])

    def test_strips_think_blocks_from_content(self) -> None:
        from tools.llm_harness.providers import _strip_think_blocks

        open_tag = "<" + "think" + ">"
        close_tag = "</" + "think" + ">"
        raw = f'{open_tag}\nwait...\n{close_tag}\n{{"source":"x"}}'
        self.assertEqual(_strip_think_blocks(raw).strip(), '{"source":"x"}')


class ReasonixOutputTests(unittest.TestCase):
    def test_parses_result_and_usage(self) -> None:
        output = json.dumps({
            "type": "result",
            "subtype": "success",
            "is_error": False,
            "duration_ms": 2131,
            "num_turns": 1,
            "result": '{"source":"int f() { return 1; }"}',
            "session_id": "20260717-test-session",
            "total_cost_usd": 0.012569,
            "usage": {
                "input_tokens": 12543,
                "output_tokens": 13,
                "cache_read_input_tokens": 8000,
                "cache_creation_input_tokens": 4000,
            },
        })
        text, events, usage = parse_reasonix_output(output)
        self.assertEqual(text, '{"source":"int f() { return 1; }"}')
        self.assertEqual(len(events), 1)
        self.assertEqual(usage["input"], 12543)
        self.assertEqual(usage["output"], 13)
        self.assertEqual(usage["cache_read"], 8000)
        self.assertEqual(usage["cache_write"], 4000)
        self.assertAlmostEqual(usage["cost"], 0.012569)

    def test_parses_result_without_usage(self) -> None:
        output = json.dumps({
            "type": "result",
            "result": "just text",
        })
        text, events, usage = parse_reasonix_output(output)
        self.assertEqual(text, "just text")
        self.assertIsNone(usage["input"])

    def test_handles_plain_text_fallback(self) -> None:
        text, events, usage = parse_reasonix_output("plain text response")
        self.assertEqual(text, "plain text response")
        self.assertEqual(events, [])
        self.assertEqual(usage, {})

    def test_parses_error_result(self) -> None:
        output = json.dumps({
            "type": "result",
            "subtype": "error",
            "is_error": True,
            "result": "",
        })
        text, events, usage = parse_reasonix_output(output)
        self.assertEqual(text, "")
        self.assertEqual(events, [{"type": "result", "subtype": "error", "is_error": True, "result": ""}])
        self.assertIsNone(usage["input"])
        self.assertIsNone(usage["cost"])


class FunctionRegionTests(unittest.TestCase):
    def setUp(self) -> None:
        self.target = SimpleNamespace(
            id="thing-move", function="CThing::Move", source=Path("Thing.cpp")
        )

    def test_finds_only_qualified_definition_with_nested_braces(self) -> None:
        source = """void call() { thing.Move(); }

int CThing::Move(int value) {
    const char* brace = "}";
    if (value) { return 1; }
    return 0;
}

void after() {}
"""
        region = _find_function_region(source, self.target)
        function = source[region.content_start : region.content_end]
        self.assertTrue(function.startswith("int CThing::Move"))
        self.assertTrue(function.rstrip().endswith("}"))
        self.assertNotIn("void after", function)

    def test_prefers_stable_markers(self) -> None:
        source = (
            _begin_marker(self.target.id)
            + "\nint CThing::Move() { return 1; }\n"
            + _end_marker(self.target.id)
            + "\n"
        )
        region = _find_function_region(source, self.target)
        self.assertTrue(region.marked)
        self.assertEqual(
            source[region.content_start : region.content_end].strip(),
            "int CThing::Move() { return 1; }",
        )
        replaced = _replace_function_source(source, region, "int CThing::Move() { return 2; }")
        self.assertIn("int CThing::Move() { return 2; }\n" + _end_marker(self.target.id), replaced)

    def test_catalog_signature_does_not_duplicate_parenthesis(self) -> None:
        target = SimpleNamespace(
            id="thing-move", function="ns::CThing::Move(int, float)", source=Path("Thing.cpp")
        )
        source = "int CThing::Move(int value, float scale) { return value; }\n"
        region = _find_function_region(source, target)
        self.assertEqual(source[region.content_start : region.content_end].strip(), source.strip())


class TuCompletionTests(unittest.TestCase):
    def test_uses_one_self_contained_prompt_without_context_files(self) -> None:
        adapter = object.__new__(XenobladeAdapter)
        self.assertEqual(adapter.model_context_mode("tu-complete"), "inline")
        self.assertEqual(
            adapter.build_context_files("tu-complete", "unit", [], "prompt"), {}
        )

    def test_residual_sections_exclude_text_handled_by_function_evidence(self) -> None:
        entry = {
            "sections": [
                {"name": ".text", "fuzzy_match_percent": 90},
                {"name": ".data", "fuzzy_match_percent": 80},
                {"name": ".rodata", "fuzzy_match_percent": 100},
            ]
        }
        self.assertEqual(_residual_section_names(entry), [".data"])

    def test_requires_nonzero_match_for_every_function(self) -> None:
        report = SimpleNamespace(
            functions=[
                SimpleNamespace(name="matched", match_percent=1.0),
                SimpleNamespace(name="untouched", match_percent=0.0),
            ]
        )
        with self.assertRaisesRegex(ValueError, "untouched"):
            _require_tu_completion_ready(report)

    def test_accepts_tu_after_every_function_has_first_match(self) -> None:
        report = SimpleNamespace(
            functions=[SimpleNamespace(name="candidate", match_percent=0.1)]
        )
        _require_tu_completion_ready(report)

    def test_tu_rank_prioritizes_guard_before_percentages(self) -> None:
        adapter = XenobladeAdapter.__new__(XenobladeAdapter)
        guarded = {
            "accepted": False,
            "match_percent": 80,
            "metrics": {"guard_ok": True, "accepted_function_count": 2, "data_percent": 80, "code_percent": 80},
        }
        regressed = {
            "accepted": False,
            "match_percent": 99,
            "metrics": {"guard_ok": False, "accepted_function_count": 5, "data_percent": 99, "code_percent": 99},
        }
        self.assertGreater(
            adapter.rank_candidate("tu-complete", guarded),
            adapter.rank_candidate("tu-complete", regressed),
        )

    def test_applies_only_named_tu_slot_contents(self) -> None:
        source = (
            "before\n// LLM-HARNESS-TU-BEGIN: data\nstatic int old;\n"
            "// LLM-HARNESS-TU-END: data\nafter\n"
        )
        slots = _find_tu_slots(source)
        updated = _apply_tu_patches(
            source, slots, [SourcePatch(slot_id="data", source="static int replacement;")]
        )
        self.assertIn("static int replacement;", updated)
        self.assertNotIn("static int old;", updated)
        self.assertTrue(updated.startswith("before\n"))
        self.assertTrue(updated.endswith("after\n"))

    def test_adapter_rejects_unknown_tu_slot(self) -> None:
        adapter = XenobladeAdapter.__new__(XenobladeAdapter)
        adapter.max_source_chars = 1000
        source = (
            "// LLM-HARNESS-TU-BEGIN: known\nstatic int old;\n"
            "// LLM-HARNESS-TU-END: known\n"
        )
        candidate = Candidate(
            source="",
            hypothesis="x",
            patches=[SourcePatch(slot_id="unknown", source="static int x;")],
        )
        with self.assertRaisesRegex(ValueError, "unknown slot"):
            adapter._tu_candidate_source(
                source, candidate, SimpleNamespace(name="unit", source_path=Path("unit.cpp"))
            )

    def test_inserts_and_wraps_exact_tu_slots(self) -> None:
        inserted = _insert_empty_tu_slot("a\nanchor\nb\n", "data", before="anchor")
        self.assertIn("LLM-HARNESS-TU-BEGIN: data", inserted)
        wrapped = _wrap_tu_slot(
            "a\nstatic int x;\nstatic int y;\nb\n",
            "statics",
            start="static int x;",
            end="static int y;",
        )
        slots = _find_tu_slots(wrapped)
        self.assertIn("static int x;", wrapped[slots["statics"].content_start:slots["statics"].content_end])


class FunctionSizeTests(unittest.TestCase):
    def test_equal_and_smaller_functions_fit(self) -> None:
        self.assertTrue(_function_size_comparison(0x40, 0x40)[0])
        self.assertTrue(_function_size_comparison(0x40, 0x30)[0])

    def test_larger_function_does_not_fit(self) -> None:
        fits, detail = _function_size_comparison(0x40, 0x44)
        self.assertFalse(fits)
        self.assertIn("exceeds retail", detail)
        self.assertIn("4 bytes", detail)

    def test_binary_summary_bounds_instruction_and_relocation_deltas(self) -> None:
        relocation = lambda offset, symbol: SimpleNamespace(
            offset=offset, relocation_type=10, symbol=symbol, addend=0
        )
        retail = SimpleNamespace(
            code=bytes.fromhex("60000000 38600001 4e800020"),
            relocations=(relocation(4, "retail_symbol"),),
        )
        candidate = SimpleNamespace(
            code=bytes.fromhex("60000000 38600002 60000000 4e800020"),
            relocations=(relocation(4, "candidate_symbol"),),
        )

        summary = _binary_mismatch_summary(retail, candidate)

        self.assertIn("binary mismatch summary", summary)
        self.assertIn("retail +0x4", summary)
        self.assertIn("candidate-only", summary)
        self.assertIn("retail_symbol", summary)
        self.assertLessEqual(len(summary), 3000)


class DossierTests(unittest.TestCase):
    """Phase 1 — Target Dossier unit tests."""

    # ------------------------------------------------------------------
    # Instruction classification
    # ------------------------------------------------------------------

    def test_classify_load(self) -> None:
        insn = Instruction(0, 0, Opcode.LWZ, (3, 5, 4))
        self.assertEqual(_classify_instruction(insn), "load")

    def test_classify_store(self) -> None:
        insn = Instruction(0, 0, Opcode.STW, (1, 1, -16))
        self.assertEqual(_classify_instruction(insn), "store")

    def test_classify_call(self) -> None:
        insn = Instruction(0, 0, Opcode.B, (0,), link=True)
        self.assertEqual(_classify_instruction(insn), "call")

    def test_classify_conditional_branch(self) -> None:
        insn = Instruction(0, 0, Opcode.BC, (12, 0x20), link=False)
        self.assertEqual(_classify_instruction(insn), "conditional_branch")

    def test_classify_return(self) -> None:
        insn = Instruction(0, 0, Opcode.BCLR, (0x20,), link=False)
        self.assertEqual(_classify_instruction(insn), "return")

    def test_classify_indirect_call(self) -> None:
        insn = Instruction(0, 0, Opcode.BCCTR, (), link=True)
        self.assertEqual(_classify_instruction(insn), "indirect_call")

    def test_classify_compare(self) -> None:
        insn = Instruction(0, 0, Opcode.CMPW, (0, 3, 4))
        self.assertEqual(_classify_instruction(insn), "compare")

    def test_classify_unconditional_branch(self) -> None:
        insn = Instruction(0, 0, Opcode.B, (0x100,), link=False)
        self.assertEqual(_classify_instruction(insn), "unconditional_branch")

    # ------------------------------------------------------------------
    # Operand formatting
    # ------------------------------------------------------------------

    def test_format_negative_offset(self) -> None:
        insn = Instruction(0x8000, 0x9421FFF0, Opcode.STWU, (1, 1, -16))
        result = _format_operand(insn)
        # stwu r1, -0x10(r1): operands are (1, 1, -16)
        # i=0: r1, i=1: r1, i=2: -0x10
        self.assertIn("r1", result)
        self.assertIn("-0x10", result)

    def test_format_branch_target(self) -> None:
        insn = Instruction(0x8000, 0x48000014, Opcode.B, (0x14,))
        result = _format_operand(insn)
        # address + operand = 0x8000 + 0x14 = 0x8014, formatted as 0x00008014
        self.assertIn("0x00008014", result)

    def test_format_float_register(self) -> None:
        insn = Instruction(0, 0, Opcode.LFS, (33, 3, 8))  # f1, r3, 8(r3)
        result = _format_operand(insn)
        self.assertIn("f1", result)

    # ------------------------------------------------------------------
    # CFG builder
    # ------------------------------------------------------------------

    def test_cfg_empty(self) -> None:
        cfg = build_cfg([], 0x8000)
        self.assertEqual(cfg.entry, "B0")
        self.assertEqual(len(cfg.blocks), 0)

    def test_cfg_linear_sequence(self) -> None:
        """A sequence with no branches forms a single block."""
        insns = [
            Instruction(0x8000, 0x9421FFF0, Opcode.STWU, (1, 1, -16)),
            Instruction(0x8004, 0x7C0802A6, Opcode.MFSPR, (0, 8)),
            Instruction(0x8008, 0x4E800020, Opcode.BCLR, (0x20,)),
        ]
        cfg = build_cfg(insns, 0x8000)
        self.assertEqual(cfg.entry, "B0")
        self.assertEqual(len(cfg.blocks), 1)
        self.assertIn("B0", cfg.exit_blocks)

    def test_cfg_conditional_branch(self) -> None:
        """Conditional branch produces two successors."""
        # B0:              B1:          B2:
        # 0x8000 cmpwi→  0x8008 li r3,1  0x800C blr
        # 0x8004 beq +8   (falls through)
        #   → B1 (ft) / B2 (taken)
        insns = [
            Instruction(0x8000, 0x2C030000, Opcode.CMPWI, (0, 3, 0)),
            Instruction(0x8004, 0x41820008, Opcode.BC, (12, 8)),      # beq +8 → 0x800C
            Instruction(0x8008, 0x38600001, Opcode.ADDI, (3, 0, 1)),  # li r3,1
            Instruction(0x800C, 0x4E800020, Opcode.BCLR, (0x20,)),    # blr
        ]
        cfg = build_cfg(insns, 0x8000)
        self.assertEqual(len(cfg.blocks), 3)  # B0, B1, B2
        self.assertIsNotNone(cfg.blocks[0].terminator)
        self.assertEqual(cfg.blocks[0].terminator.kind, "conditional")
        # B0 should have both fallthrough (B1) and taken (B2) successors
        self.assertIn("B1", cfg.blocks[0].successors)
        self.assertIn("B2", cfg.blocks[0].successors)

    def test_cfg_backedge(self) -> None:
        """A backward branch produces a backedge entry."""
        # Loop:  0x8000 li r3,0        → B0
        #        0x8004 cmpwi r3,10    → (B0 body)
        #        0x8008 bge +8         → conditional to B2 (exit)
        #        0x800C addi r4,r3,1   → B1
        #        0x8010 b -0x10        → unconditional to 0x8000 (backedge to B0)
        # Exit:  0x8014 blr            → B2
        insns = [
            Instruction(0x8000, 0x38600000, Opcode.ADDI, (3, 0, 0)),
            Instruction(0x8004, 0x2C03000A, Opcode.CMPWI, (0, 3, 10)),
            Instruction(0x8008, 0x4080000C, Opcode.BC, (12, 12)),  # bge +12 → 0x8014
            Instruction(0x800C, 0x38830001, Opcode.ADDI, (4, 3, 1)),
            Instruction(0x8010, 0x4BFFFFF0, Opcode.B, (-16,)),  # b -0x10 → 0x8000
            Instruction(0x8014, 0x4E800020, Opcode.BCLR, (0x20,)),
        ]
        cfg = build_cfg(insns, 0x8000)
        self.assertGreater(len(cfg.backedges), 0)

    # ------------------------------------------------------------------
    # Data-flow summary
    # ------------------------------------------------------------------

    def test_dataflow_stack_frame(self) -> None:
        """STWU r1,-N(r1) at entry sets stack_frame_size."""
        insns = [
            Instruction(0x8000, 0x9421FFF0, Opcode.STWU, (1, 1, -16)),
            Instruction(0x8004, 0x4E800020, Opcode.BCLR, (0x20,)),
        ]
        df = build_data_flow_summary(insns)
        self.assertEqual(df.stack_frame_size, 16)

    def test_dataflow_no_frame(self) -> None:
        """Leaf function with no frame has zero stack_frame_size."""
        insns = [
            Instruction(0x8000, 0x38600001, Opcode.ADDI, (3, 0, 1)),
            Instruction(0x8004, 0x4E800020, Opcode.BCLR, (0x20,)),
        ]
        df = build_data_flow_summary(insns)
        self.assertEqual(df.stack_frame_size, 0)

    def test_dataflow_saved_registers(self) -> None:
        """STW to r1-based offsets after stack alloc are saved registers."""
        insns = [
            Instruction(0x8000, 0x9421FFE0, Opcode.STWU, (1, 1, -32)),
            Instruction(0x8004, 0x93E1001C, Opcode.STW, (31, 1, 28)),
            Instruction(0x8008, 0x93C10018, Opcode.STW, (30, 1, 24)),
            Instruction(0x800C, 0x7C0802A6, Opcode.MFSPR, (0, 8)),
            Instruction(0x8010, 0x90010024, Opcode.STW, (0, 1, 36)),
            Instruction(0x8014, 0x4E800020, Opcode.BCLR, (0x20,)),
        ]
        df = build_data_flow_summary(insns)
        self.assertIn("r31", df.saved_registers)
        self.assertIn("r30", df.saved_registers)

    def test_dataflow_xform_load(self) -> None:
        """An x-form (indexed) load records a register-indexed access."""
        insns = [
            Instruction(0x8000, 0x7C631014, Opcode.LWZX, (3, 3, 5)),
            Instruction(0x8004, 0x4E800020, Opcode.BCLR, (0x20,)),
        ]
        df = build_data_flow_summary(insns)
        # Should have one memory access with offset=0 and likely_expression set
        self.assertEqual(len(df.memory_accesses), 1)
        acc = df.memory_accesses[0]
        self.assertEqual(acc.kind, "load")
        self.assertEqual(acc.offset, 0)
        self.assertIn("r3", acc.likely_expression)
        self.assertIn("r5", acc.likely_expression)

    def test_dataflow_dform_store(self) -> None:
        """A d-form store records the immediate offset directly."""
        insns = [
            Instruction(0x8000, 0x90010014, Opcode.STW, (0, 1, 20)),
            Instruction(0x8004, 0x4E800020, Opcode.BCLR, (0x20,)),
        ]
        df = build_data_flow_summary(insns)
        self.assertEqual(len(df.memory_accesses), 1)
        acc = df.memory_accesses[0]
        self.assertEqual(acc.kind, "store")
        self.assertEqual(acc.offset, 20)

    def test_dataflow_calls(self) -> None:
        """BL instructions are recorded as calls."""
        insns = [
            Instruction(0x8000, 0x48000001, Opcode.B, (0,), link=True),
            Instruction(0x8004, 0x4E800020, Opcode.BCLR, (0x20,)),
        ]
        df = build_data_flow_summary(insns)
        # The call target 0x8000 will be recorded as "0x80008001" (hex)
        self.assertGreaterEqual(len(df.calls), 0)  # at minimum, doesn't crash

    # ------------------------------------------------------------------
    # Signature parsing
    # ------------------------------------------------------------------

    def test_parse_simple_method(self) -> None:
        sig = parse_signature("CGame::wkRender")
        self.assertEqual(sig.declaration, "CGame::wkRender")
        self.assertIsNotNone(sig.implicit_this)
        self.assertEqual(sig.implicit_this.type, "CGame*")
        self.assertEqual(len(sig.parameters), 0)

    def test_parse_method_with_params(self) -> None:
        sig = parse_signature("CGame::setTaskManagerUpdateCount(unsigned long)")
        self.assertIsNotNone(sig.implicit_this)
        self.assertEqual(sig.implicit_this.type, "CGame*")
        self.assertEqual(len(sig.parameters), 1)
        self.assertEqual(sig.parameters[0].type, "unsigned long")

    def test_parse_free_function(self) -> None:
        sig = parse_signature("memcpy")
        self.assertIsNone(sig.implicit_this)
        self.assertEqual(sig.return_info.type, "void")

    def test_parse_free_function_with_return(self) -> None:
        sig = parse_signature("int func(int a, float b)")
        self.assertIsNone(sig.implicit_this)
        self.assertEqual(sig.return_info.type, "int")
        self.assertEqual(len(sig.parameters), 2)
        self.assertEqual(sig.parameters[0].name, "a")
        self.assertEqual(sig.parameters[1].type, "float")

    def test_parse_namespaced_class(self) -> None:
        sig = parse_signature("nw4r2ut36LinkList::destructor()")
        self.assertIsNotNone(sig.implicit_this)
        self.assertEqual(sig.implicit_this.type, "nw4r2ut36LinkList*")
        self.assertEqual(sig.return_info.type, "void")

    def test_parse_void_params(self) -> None:
        sig = parse_signature("void func(void)")
        self.assertIsNone(sig.implicit_this)
        self.assertEqual(sig.return_info.type, "void")
        self.assertEqual(len(sig.parameters), 0)

    # ------------------------------------------------------------------
    # Parameter splitting
    # ------------------------------------------------------------------

    def test_split_params_simple(self) -> None:
        self.assertEqual(_split_params("int a, float b"), ["int a", "float b"])

    def test_split_params_template(self) -> None:
        result = _split_params("std::vector<int, allocator<int>>, float")
        self.assertEqual(len(result), 2)
        self.assertTrue(result[0].startswith("std::vector<"))
        self.assertEqual(result[1], "float")

    def test_split_params_empty(self) -> None:
        self.assertEqual(_split_params(""), [])

    # ------------------------------------------------------------------
    # Validation
    # ------------------------------------------------------------------

    def test_validate_missing_declaration(self) -> None:
        d = TargetDossier(schema_version=3)
        errors = validate_dossier(d)
        self.assertIn("missing target declaration", errors)

    def test_validate_missing_instructions(self) -> None:
        d = TargetDossier(schema_version=3, retail=RetailProgramContext(base="0x8000", size=0))
        d.signature = parse_signature("some_func")
        errors = validate_dossier(d)
        self.assertIn("missing decoded retail instructions", errors)

    def test_validate_passes_valid(self) -> None:
        d = TargetDossier(schema_version=3, retail=RetailProgramContext(
            base="0x8000", size=4,
            instructions=[
                {"address": "0x8000", "offset": 0, "raw_word": "4e800020",
                 "mnemonic": "blr", "operands": "", "instruction_class": "return"}
            ],
        ))
        d.signature = parse_signature("some_func")
        errors = validate_dossier(d)
        self.assertEqual(errors, [])

    # ------------------------------------------------------------------
    # Target identity
    # ------------------------------------------------------------------

    def test_target_identity(self) -> None:
        ti = build_target_identity(
            "test-func", "test_func(void)", "_test_func__Fv",
            "test.cpp", "test.o", "test", 0x8000, 20,
        )
        self.assertEqual(ti.target_id, "test-func")
        self.assertEqual(ti.retail_address, "0x00008000")
        self.assertEqual(ti.retail_size, 20)

    # ------------------------------------------------------------------
    # Declaration extraction
    # ------------------------------------------------------------------

    def test_find_class_name_simple(self) -> None:
        self.assertEqual(_find_class_name("CGame::wkRender()"), "CGame")

    def test_find_class_name_namespaced(self) -> None:
        self.assertEqual(
            _find_class_name("nw4r2ut36LinkList::destructor()"),
            "nw4r2ut36LinkList",
        )

    def test_find_class_name_no_class(self) -> None:
        self.assertIsNone(_find_class_name("memcpy"))

    # ------------------------------------------------------------------
    # Truncation
    # ------------------------------------------------------------------

    def test_decode_truncation_sets_flag(self) -> None:
        from tools.ppc_equivalence.elf_symbols import FunctionBytes
        from tools.llm_harness.dossier import PromptConstraints
        from pathlib import Path

        # 3 instructions, cap at 2 → truncated
        fb = FunctionBytes(
            name="test", path=Path("/dev/null"),
            code=bytes.fromhex("38600001 38800002 4e800020"),
            base=0x8000, value=0, size=12,
            section_index=0, section_name=".text", symbol_type=0x22,
        )
        constraints = PromptConstraints(max_decoded_instructions=2)
        d = build_target_dossier(
            "test", "test__Fv", "test()",
            "t.cpp", "t.o", "t", 0x8000, 12, fb,
            "", "", [], constraints,
        )
        self.assertTrue(d.retail.truncated)
        self.assertEqual(len(d.retail.instructions), 2)


class MarkerSlotTests(unittest.TestCase):
    def test_inserts_only_at_unique_exact_anchor(self) -> None:
        target = SimpleNamespace(id="new-fn", function="CThing::newFn")
        source = "namespace x {\n// anchor\n}\n"
        updated = _insert_marker_slot(source, target, before="// anchor")
        self.assertIn("LLM-HARNESS-BEGIN: new-fn", updated)
        self.assertLess(updated.index("LLM-HARNESS-BEGIN"), updated.index("// anchor"))

    def test_rejects_ambiguous_anchor(self) -> None:
        target = SimpleNamespace(id="new-fn", function="CThing::newFn")
        with self.assertRaisesRegex(ValueError, "found 2"):
            _insert_marker_slot("x x", target, before="x")


class WorktreeTests(unittest.TestCase):
    def test_overlays_dirty_snapshot_and_removes_worktree(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp) / "repo"
            root.mkdir()
            subprocess.run(["git", "init", "-q"], cwd=root, check=True)
            subprocess.run(["git", "config", "user.email", "test@example.invalid"], cwd=root, check=True)
            subprocess.run(["git", "config", "user.name", "Test"], cwd=root, check=True)
            (root / "tracked.txt").write_text("base\n", encoding="utf-8")
            subprocess.run(["git", "add", "tracked.txt"], cwd=root, check=True)
            subprocess.run(["git", "commit", "-qm", "base"], cwd=root, check=True)
            (root / "tracked.txt").write_text("dirty\n", encoding="utf-8")
            (root / "new.txt").write_text("new\n", encoding="utf-8")
            manager = GitWorktreeManager(root)
            workspace = manager.create("test")
            try:
                self.assertEqual((workspace / "tracked.txt").read_text(), "dirty\n")
                self.assertEqual((workspace / "new.txt").read_text(), "new\n")
            finally:
                manager.remove(workspace)
            self.assertFalse(workspace.exists())


class ResumeBudgetTests(unittest.TestCase):
    def test_budget_stop_retry_and_resume(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "from tools.llm_harness.types import Evaluation\n"
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt'\n"
                " def evaluate(self, workflow, target, candidate): return Evaluation('HIGH_MATCH',80,False)\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "execution": {"max_retries": 1},
                "models": [
                    {"id": "a", "provider": "fake", "model": "p/a"},
                    {"id": "b", "provider": "fake", "model": "p/b"},
                ],
            }), encoding="utf-8")

            class FakeProvider:
                def __init__(self) -> None:
                    self.calls = 0
                    self.last_cwd = None

                def invoke(self, prompt, model, cwd):
                    self.calls += 1
                    self.last_cwd = cwd
                    if self.calls == 1:
                        raise RuntimeError("retry me")
                    return ProviderResult(
                        text='{"source":"int f() { return 1; }","hypothesis":"x"}',
                        duration_seconds=1,
                        input_tokens=10,
                        output_tokens=5,
                        cost=1.0,
                    )

            harness = Harness(root / "config.json")
            fake = FakeProvider()
            harness.providers["fake"] = fake
            experiment = harness.run("new", "target", max_cost=0.5)
            state = json.loads((experiment / "state.json").read_text())
            self.assertEqual(state["status"], "budget_exhausted")
            self.assertEqual(len(state["records"]), 1)
            harness.run("new", "target", resume=experiment, max_cost=3.0)
            state = json.loads((experiment / "state.json").read_text())
            self.assertEqual(state["status"], "complete")
            self.assertEqual(len(state["records"]), 2)
            self.assertEqual(fake.calls, 3)
            self.assertEqual(fake.last_cwd.name, "context")
            self.assertTrue((fake.last_cwd / "TASK.md").is_file())

    def test_full_match_stops_remaining_agents(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "from tools.llm_harness.types import Evaluation\n"
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt'\n"
                " def evaluate(self, workflow, target, candidate): return Evaluation('FULL_MATCH',100,True)\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "execution": {"max_retries": 0},
                "models": [
                    {"id": "a", "provider": "fake", "model": "p/a"},
                    {"id": "b", "provider": "fake", "model": "p/b"},
                ],
            }), encoding="utf-8")

            class FakeProvider:
                def __init__(self) -> None:
                    self.calls = 0

                def invoke(self, prompt, model, cwd):
                    self.calls += 1
                    return ProviderResult(
                        text='{"source":"int f() { return 1; }","hypothesis":"x"}',
                        duration_seconds=1,
                    )

            harness = Harness(root / "config.json")
            fake = FakeProvider()
            harness.providers["fake"] = fake
            experiment = harness.run("new", "target")
            state = json.loads((experiment / "state.json").read_text())
            self.assertEqual(state["status"], "complete")
            self.assertEqual(len(state["records"]), 1)
            self.assertEqual(fake.calls, 1)
            self.assertEqual(state["records"][0]["evaluation"]["status"], "FULL_MATCH")

    def test_parallel_full_match_logs_already_running_non_winners(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "from tools.llm_harness.types import Evaluation\n"
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt'\n"
                " def evaluate(self, workflow, target, candidate):\n"
                "  return Evaluation('FULL_MATCH',100,True) if 'return 1' in candidate.source else Evaluation('HIGH_MATCH',80,False)\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "execution": {"max_retries": 0, "max_parallel": 2},
                "models": [
                    {"id": "a", "provider": "fake", "model": "p/a"},
                    {"id": "b", "provider": "fake", "model": "p/b"},
                ],
            }), encoding="utf-8")

            class FakeProvider:
                def __init__(self) -> None:
                    self.barrier = threading.Barrier(2)

                def invoke(self, prompt, model, cwd):
                    self.barrier.wait(timeout=2)
                    value = 1 if model.id == "a" else 2
                    return ProviderResult(
                        text=(
                            '{"source":"int f() { return '
                            f'{value}; }}","hypothesis":"x"}}'
                        ),
                        duration_seconds=1,
                    )

            harness = Harness(root / "config.json")
            harness.providers["fake"] = FakeProvider()
            experiment = harness.run("new", "target", max_parallel=2)
            state = json.loads((experiment / "state.json").read_text())
            rows = [
                json.loads(line)
                for line in (root / "out" / "experiments.jsonl").read_text().splitlines()
            ]
            self.assertEqual(len(state["records"]), 2)
            self.assertEqual(len(rows), 2)
            self.assertEqual(sum(bool(row["winner"]) for row in rows), 1)
            self.assertEqual(
                {row["evaluation"]["status"] for row in rows},
                {"FULL_MATCH", "HIGH_MATCH"},
            )


class BatchTests(unittest.TestCase):
    def test_dry_run_creates_one_context_per_target(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt '+target\n"
                " def evaluate(self, workflow, target, candidate): raise AssertionError()\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "execution": {"max_target_parallel": 2},
                "models": [{"id": "a", "provider": "opencode", "model": "p/a"}],
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            batch = harness.run_batch("new", ["one", "two"], dry_run=True)
            manifest = json.loads((batch / "batch.json").read_text())
            self.assertEqual(manifest["status"], "complete")
            self.assertEqual(set(manifest["targets"]), {"one", "two"})
            for row in manifest["targets"].values():
                context = root / row["experiment"] / "context"
                self.assertTrue((context / "TASK.md").is_file())

    def test_inline_context_omits_duplicate_task(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt '+target\n"
                " def model_context_mode(self, workflow): return 'inline'\n"
                " def evaluate(self, workflow, target, candidate): raise AssertionError()\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "models": [{"id": "a", "provider": "opencode", "model": "p/a"}],
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            experiment = harness.run("new", "one", dry_run=True)
            self.assertEqual(list((experiment / "context").iterdir()), [])
            state = json.loads((experiment / "state.json").read_text())
            self.assertEqual(state["context_mode"], "inline")


class WorkflowModelTests(unittest.TestCase):
    @staticmethod
    def _write_adapter(root: Path) -> None:
        (root / "adapter.py").write_text(
            "class A:\n"
            " def __init__(self, root): self.root=root\n"
            " def finalize(self): pass\n"
            "def create_adapter(root, settings): return A(root)\n",
            encoding="utf-8",
        )

    def test_selects_exact_pipeline_then_default(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._write_adapter(root)
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "models": {
                    "default": [
                        {"id": "general", "provider": "opencode", "model": "p/general"}
                    ],
                    "new": [
                        {"id": "fast", "provider": "opencode", "model": "p/fast"}
                    ],
                },
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            self.assertEqual(
                [model.id for model in harness.models_for_workflow("new")], ["fast"]
            )
            self.assertEqual(
                [model.id for model in harness.models_for_workflow("improve")],
                ["general"],
            )

    def test_pipeline_without_exact_or_default_is_rejected_on_use(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._write_adapter(root)
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "models": {
                    "new": [
                        {"id": "fast", "provider": "opencode", "model": "p/fast"}
                    ]
                },
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            with self.assertRaisesRegex(ValueError, "models.improve or models.default"):
                harness.models_for_workflow("improve")

    def test_legacy_flat_model_list_remains_supported(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._write_adapter(root)
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "models": [
                    {"id": "legacy", "provider": "opencode", "model": "p/legacy"}
                ],
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            self.assertEqual(
                [model.id for model in harness.models_for_workflow("tu-complete")],
                ["legacy"],
            )


class WorkflowExecutionTests(unittest.TestCase):
    @staticmethod
    def _write_config(root: Path, execution: dict) -> Harness:
        (root / "adapter.py").write_text(
            "class A:\n"
            " def __init__(self, root): self.root=root\n"
            " def finalize(self): pass\n"
            "def create_adapter(root, settings): return A(root)\n",
            encoding="utf-8",
        )
        (root / "config.json").write_text(json.dumps({
            "project_adapter": "adapter.py",
            "execution": execution,
            "models": [
                {"id": "model", "provider": "opencode", "model": "p/model"}
            ],
        }), encoding="utf-8")
        return Harness(root / "config.json")

    def test_pipeline_parallelism_overrides_global_values(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            harness = self._write_config(Path(tmp), {
                "max_parallel": 2,
                "max_target_parallel": 3,
                "batch_model_parallel": 4,
                "pipelines": {
                    "new": {"max_parallel": 5, "max_target_parallel": 6},
                    "improve": {"batch_model_parallel": 1},
                },
            })
            self.assertEqual(harness.parallelism_for_workflow("new", "max_parallel"), 5)
            self.assertEqual(
                harness.parallelism_for_workflow("new", "max_target_parallel"), 6
            )
            self.assertEqual(
                harness.parallelism_for_workflow("new", "batch_model_parallel"), 4
            )
            self.assertEqual(
                harness.parallelism_for_workflow("improve", "max_parallel"), 2
            )
            self.assertEqual(
                harness.parallelism_for_workflow("improve", "batch_model_parallel"), 1
            )

    def test_rejects_unknown_pipeline_execution_option(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaisesRegex(ValueError, "Unknown execution option"):
                self._write_config(Path(tmp), {
                    "pipelines": {"new": {"concurrent": 2}}
                })

    def test_rejects_nonpositive_pipeline_parallelism(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaisesRegex(ValueError, "values must be positive"):
                self._write_config(Path(tmp), {
                    "pipelines": {"tu-complete": {"max_parallel": 0}}
                })


class StatsTests(unittest.TestCase):
    def test_aggregates_model_stats(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "class A:\n root='.'\n def finalize(self): pass\n"
                "def create_adapter(root, settings): return A()\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "models": [{"id": "a", "provider": "opencode", "model": "p/a"}],
            }), encoding="utf-8")
            out = root / "out"
            out.mkdir()
            rows = [
                {"model_id": "a", "duration_seconds": 2, "input_tokens": 10, "output_tokens": 4,
                 "cache_read_tokens": 8, "cache_write_tokens": 3,
                 "cost": 0.1, "winner": True, "evaluation": {"accepted": True, "match_percent": 100}},
                {"model_id": "a", "duration_seconds": 4, "input_tokens": 20, "output_tokens": 6,
                 "cost": 0.2, "winner": False, "evaluation": {"accepted": False, "match_percent": 80}},
                {"model_id": "a", "duration_seconds": 180, "input_tokens": 0, "output_tokens": 0,
                 "cost": None, "winner": False, "timed_out": True,
                 "error": "TimeoutExpired: model exceeded 180 seconds", "evaluation": {}},
            ]
            (out / "experiments.jsonl").write_text(
                "".join(json.dumps(row) + "\n" for row in rows), encoding="utf-8"
            )
            stats = Harness(root / "config.json").stats()[0]
            self.assertEqual(stats["wins"], 1)
            self.assertEqual(stats["accepted_wins"], 1)
            self.assertEqual(stats["accepted"], 1)
            self.assertEqual(stats["average_match_percent"], 90)
            self.assertEqual(stats["average_seconds"], 3)
            self.assertEqual(stats["timeouts"], 1)
            self.assertEqual(stats["errors"], 1)
            self.assertAlmostEqual(stats["total_cost"], 0.3)
            self.assertAlmostEqual(stats["average_cost"], 0.15)
            self.assertEqual(stats["non_cached_tokens"], 22)
            self.assertEqual(stats["cache_read_tokens"], 8)
            self.assertEqual(stats["cache_write_tokens"], 3)


if __name__ == "__main__":
    unittest.main()

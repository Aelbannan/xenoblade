from __future__ import annotations

import inspect
import json
import sys
import textwrap
import unittest
from pathlib import Path
from typing import Any

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.compile_diagnostic import (
    NormalizedDiagnostic,
    normalize_diagnostic,
    normalize_compile_output,
    select_root_diagnostic,
    _classify_message,
)
from tools.llm_harness.reconstruction import (
    RECONSTRUCTION_STRATEGIES,
    ReconstructionResponse,
    SemanticSummary,
    parse_reconstruction_response,
    parse_strategy_label,
    get_strategy_prompt_suffix,
)
from tools.llm_harness.repair_loop import (
    RepairBranch,
    RepairAttempt,
    repairability_tier,
    compile_repair_priority,
)


class TestCompileDiagnosticNormalization(unittest.TestCase):
    """§10.3 — normalization"""

    def test_gcc_format(self):
        diag = normalize_diagnostic("src/foo.cpp:91: error: 'mFoo' was not declared")
        self.assertIsNotNone(diag)
        self.assertEqual(diag.category, "unknown_identifier")
        self.assertEqual(diag.symbol, "mFoo")
        self.assertEqual(diag.line, 91)
        self.assertEqual(diag.fingerprint, "unknown_identifier:mFoo")

    def test_mwcc_format(self):
        diag = normalize_diagnostic("src/foo.cpp(42) : error: argument is incompatible with parameter")
        self.assertIsNotNone(diag)
        self.assertEqual(diag.line, 42)

    def test_gcc_with_column(self):
        diag = normalize_diagnostic("src/bar.cpp:15:10: error: expected ',' before 'x'")
        self.assertIsNotNone(diag)
        self.assertEqual(diag.line, 15)
        self.assertEqual(diag.column, 10)

    def test_workspace_root_normalization(self):
        diag = normalize_diagnostic(
            "/home/user/project/src/main.cpp:22: error: 'x' was not declared",
            workspace_roots=["/home/user/project"],
        )
        self.assertIsNotNone(diag)
        self.assertEqual(diag.file, "src/main.cpp")

    def test_non_diagnostic_line(self):
        diag = normalize_diagnostic("make: *** [Makefile:42: target] Error 1")
        self.assertIsNone(diag)

    def test_empty_line(self):
        diag = normalize_diagnostic("")
        self.assertIsNone(diag)

    def test_multi_diagnostic_output(self):
        output = textwrap.dedent("""\
            src/a.cpp:10: error: 'Foo' was not declared
            src/a.cpp:11: error: 'Foo' has no member named 'bar'
            make: *** [all] Error 1
        """)
        diagnostics = normalize_compile_output(output)
        self.assertEqual(len(diagnostics), 2)
        self.assertEqual(diagnostics[0].category, "unknown_identifier")
        self.assertEqual(diagnostics[1].category, "unknown_member")


class TestDiagnosticClassification(unittest.TestCase):
    """§10.4 — diagnostic categories"""

    def test_syntax_error(self):
        self.assertEqual(_classify_message("parse error before 'x'"), "syntax_error")
        self.assertEqual(_classify_message("expected ';' before 'return'"), "syntax_error")

    def test_unknown_identifier(self):
        self.assertEqual(_classify_message("'myVar' was not declared"), "unknown_identifier")

    def test_unknown_member(self):
        self.assertEqual(_classify_message("has no member named 'value'"), "unknown_member")

    def test_argument_count(self):
        self.assertEqual(_classify_message("too few arguments to function"), "argument_count")
        self.assertEqual(_classify_message("too many arguments to function"), "argument_count")

    def test_invalid_conversion(self):
        self.assertEqual(_classify_message("cannot convert 'int*' to 'float*'"), "invalid_conversion")

    def test_incomplete_type(self):
        self.assertEqual(_classify_message("incomplete type 'MyStruct'"), "incomplete_type")

    def test_access_control(self):
        self.assertEqual(_classify_message("'mSecret' is private"), "access_control")
        self.assertEqual(_classify_message("'mSecret' is protected"), "access_control")

    def test_linkage_error(self):
        self.assertEqual(_classify_message("undefined reference to 'foo'"), "linkage_error")

    def test_unknown_category(self):
        self.assertEqual(_classify_message("some random warning"), "other")
        self.assertEqual(_classify_message(""), "other")

    def test_linkage_error_detail(self):
        self.assertEqual(_classify_message("undefined reference to `foo bar'"), "linkage_error")

    def test_experiment_id_normalization(self):
        """10. Experiment ID normalization."""
        diag = normalize_diagnostic(
            "/home/user/exp_20260101/src/main.cpp:22: error: 'x' was not declared",
            workspace_roots=["/home/user/exp_20260101"],
        )
        self.assertIsNotNone(diag)
        self.assertEqual(diag.file, "src/main.cpp")

    def test_duplicate_diagnostic_grouping(self):
        """11. Duplicate diagnostic grouping."""
        output = (
            "src/a.cpp:10: error: 'Foo' was not declared\n"
            "src/a.cpp:11: error: 'Foo' was not declared\n"
        )
        diags = normalize_compile_output(output)
        self.assertEqual(len(diags), 2)
        self.assertEqual(diags[0].fingerprint, diags[1].fingerprint)

    def test_stable_fingerprint(self):
        """13. Stable fingerprint across different worktree paths."""
        diag1 = normalize_diagnostic(
            "/home/user/worktree1/src/main.cpp:5: error: 'x' was not declared",
        )
        diag2 = normalize_diagnostic(
            "/home/user/worktree2/src/main.cpp:5: error: 'x' was not declared",
        )
        if diag1 and diag2:
            self.assertEqual(diag1.fingerprint, diag2.fingerprint)

    def test_repeated_fingerprint_detection(self):
        """14. Repeated fingerprint detection."""
        diag = normalize_diagnostic("src/test.cpp:3: error: 'myVar' was not declared")
        self.assertIsNotNone(diag)
        self.assertEqual(diag.fingerprint, "unknown_identifier:myVar")


class TestRootDiagnosticSelection(unittest.TestCase):
    """§10.5 — root diagnostic"""

    def test_syntax_error_takes_priority(self):
        diags = [
            NormalizedDiagnostic(category="unknown_identifier", message="'Foo' not declared"),
            NormalizedDiagnostic(category="syntax_error", message="parse error"),
        ]
        root = select_root_diagnostic(diags)
        self.assertEqual(root.category, "syntax_error")

    def test_first_non_cascade_returned(self):
        diags = [
            NormalizedDiagnostic(category="unknown_identifier", message="'Foo' not declared"),
            NormalizedDiagnostic(category="unknown_member", message="'Foo' has no member 'bar'"),
        ]
        root = select_root_diagnostic(diags)
        self.assertEqual(root.category, "unknown_identifier")

    def test_linkage_fallback(self):
        diags = [
            NormalizedDiagnostic(category="unknown_member", message="'Foo::bar' unknown"),
            NormalizedDiagnostic(category="linkage_error", message="undefined reference"),
        ]
        root = select_root_diagnostic(diags)
        self.assertEqual(root.category, "unknown_member")

    def test_empty_input(self):
        self.assertIsNone(select_root_diagnostic([]))

    def test_cascade_detection(self):
        diags = [
            NormalizedDiagnostic(category="unknown_identifier", message="'Foo' missing"),
            NormalizedDiagnostic(category="invalid_conversion", message="cannot convert"),
        ]
        root = select_root_diagnostic(diags)
        self.assertEqual(root.category, "unknown_identifier")


class TestReconstructionStrategies(unittest.TestCase):
    """§10.1 — reconstruction strategies"""

    def test_strategies_defined(self):
        self.assertIn("A", RECONSTRUCTION_STRATEGIES)
        self.assertIn("B", RECONSTRUCTION_STRATEGIES)
        self.assertIn("C", RECONSTRUCTION_STRATEGIES)
        for key in ("A", "B", "C"):
            self.assertTrue(len(RECONSTRUCTION_STRATEGIES[key]) > 20)

    def test_parse_strategy_label(self):
        self.assertEqual(parse_strategy_label("<STRATEGY A>"), "STRATEGY A")
        self.assertEqual(parse_strategy_label("<STRATEGY B>"), "STRATEGY B")
        self.assertEqual(parse_strategy_label("<STRATEGY C>"), "STRATEGY C")
        self.assertIsNone(parse_strategy_label("plain text"))

    def test_get_strategy_suffix(self):
        suffix = get_strategy_prompt_suffix("A")
        self.assertIn("Strategy A", suffix)
        suffix_none = get_strategy_prompt_suffix(None)
        self.assertEqual(suffix_none, "")
        suffix_unknown = get_strategy_prompt_suffix("D")
        self.assertEqual(suffix_unknown, "")


class TestReconstructionResponseParsing(unittest.TestCase):
    """§10.2 — response schema"""

    def test_minimal_valid_response(self):
        resp = parse_reconstruction_response(
            '{"source":"int x;","semantic_summary":{"calls":["foo"]}}'
        )
        self.assertIsNotNone(resp)
        self.assertEqual(resp.source, "int x;")
        self.assertEqual(resp.semantic_summary.calls, ["foo"])

    def test_fenced_json(self):
        resp = parse_reconstruction_response(
            '```json\n{"source":"void f(){}","semantic_summary":{}}\n```'
        )
        self.assertIsNotNone(resp)
        self.assertEqual(resp.source, "void f(){}")

    def test_full_response(self):
        raw = json.dumps({
            "response_schema_version": 2,
            "stage": "reconstruct",
            "source": "bool check(int x) { return x > 0; }",
            "semantic_summary": {
                "calls": [],
                "memory_reads": [{"base": "this", "offset": 4, "width_bits": 32}],
                "memory_writes": [],
                "return_paths": [
                    {"condition": "x > 0", "value": "true"},
                    {"condition": "otherwise", "value": "false"},
                ],
                "constants": [0],
            },
            "assumptions": ["x is unsigned"],
            "blocked_on": "",
            "hypothesis": "Simple comparison function",
            "confidence": 0.85,
        })
        resp = parse_reconstruction_response(raw)
        self.assertIsNotNone(resp)
        self.assertEqual(resp.stage, "reconstruct")
        self.assertEqual(len(resp.semantic_summary.calls), 0)
        self.assertEqual(len(resp.semantic_summary.memory_reads), 1)
        self.assertEqual(len(resp.semantic_summary.return_paths), 2)
        self.assertEqual(resp.semantic_summary.constants, [0])
        self.assertAlmostEqual(resp.confidence, 0.85)

    def test_empty_source_rejected(self):
        resp = parse_reconstruction_response('{"source":"  ","semantic_summary":{}}')
        self.assertIsNone(resp)

    def test_invalid_json_rejected(self):
        resp = parse_reconstruction_response("{invalid")
        self.assertIsNone(resp)

    def test_missing_source_field(self):
        resp = parse_reconstruction_response('{"hypothesis":"test","semantic_summary":{}}')
        self.assertIsNone(resp)


class TestRepairBranch(unittest.TestCase):
    """§10.7-10.9 — repair branch types"""

    def test_branch_creation(self):
        branch = RepairBranch(
            target_id="SomeClass__func",
            strategy_label="A",
            initial_source="void f() {}",
        )
        self.assertEqual(branch.target_id, "SomeClass__func")
        self.assertIsNone(branch.latest)
        self.assertFalse(branch.blocked)

    def test_latest_attempt(self):
        branch = RepairBranch(target_id="t", strategy_label="A", initial_source="int x;")
        branch.attempts.append(RepairAttempt(repair_index=1, source="int x;"))
        self.assertIsNotNone(branch.latest)
        self.assertEqual(branch.latest.repair_index, 1)
        self.assertEqual(branch.latest.source, "int x;")

    def test_block(self):
        branch = RepairBranch(target_id="t", strategy_label="A", initial_source="src")
        branch.block("repeated_diagnostic", "unknown_identifier:Foo")
        self.assertTrue(branch.blocked)
        self.assertIn("repeated_diagnostic", branch.blocked_reason)

    def test_repairability_tier(self):
        self.assertEqual(repairability_tier([]), 5)
        self.assertEqual(
            repairability_tier([NormalizedDiagnostic(category="syntax_error", message="err")]),
            1,
        )
        self.assertEqual(
            repairability_tier([NormalizedDiagnostic(category="unknown_identifier", message="err")]),
            2,
        )
        self.assertEqual(
            repairability_tier([NormalizedDiagnostic(category="invalid_conversion", message="err")]),
            3,
        )
        self.assertEqual(
            repairability_tier([NormalizedDiagnostic(category="linkage_error", message="err")]),
            4,
        )
        self.assertEqual(
            repairability_tier([NormalizedDiagnostic(category="other", message="err")]),
            0,
        )

    def test_compile_repair_priority(self):
        branch = RepairBranch(target_id="t", strategy_label="A", initial_source="")
        branch.attempts.append(
            RepairAttempt(
                repair_index=1,
                source="int x;",
                diagnostics=[NormalizedDiagnostic(category="syntax_error", message="err")],
                root_fingerprint="syntax_error:err",
            )
        )
        priority = compile_repair_priority(branch)
        self.assertEqual(len(priority), 6)
        self.assertEqual(priority[0], 1)  # has source

    def test_priority_fresh_vs_repeated(self):
        branch = RepairBranch(target_id="t", strategy_label="A", initial_source="")
        branch.attempts.append(
            RepairAttempt(
                repair_index=1,
                source="x",
                diagnostics=[NormalizedDiagnostic(category="syntax_error", message="err")],
                root_fingerprint="syntax_error:err",
            )
        )
        priority_fresh = compile_repair_priority(branch)

        branch.blocked = True
        branch.blocked_reason = "repeated_diagnostic"
        priority_blocked = compile_repair_priority(branch)
        # Blocked branch should have lower fingerprint freshness
        self.assertNotEqual(priority_fresh, priority_blocked)


class TestIntegrationWithExisting(unittest.TestCase):
    """Verify Phase 2 types integrate with existing systems"""

    def test_normalization_used_by_repair_loop(self):
        from tools.llm_harness.compile_diagnostic import normalize_compile_output
        output = "src/test.cpp:42: error: 'MyType' was not declared"
        diags = normalize_compile_output(output)
        self.assertEqual(len(diags), 1)
        self.assertEqual(diags[0].category, "unknown_identifier")

    def test_reconstruction_response_roundtrip(self):
        resp = ReconstructionResponse(
            stage="reconstruct",
            source="void test() {}",
            semantic_summary=SemanticSummary(
                calls=["foo"],
                constants=[1, 2, 3],
            ),
            hypothesis="Test function",
            confidence=0.9,
        )
        as_dict = {
            "response_schema_version": resp.response_schema_version,
            "stage": resp.stage,
            "source": resp.source,
            "semantic_summary": {
                "calls": resp.semantic_summary.calls,
                "memory_reads": resp.semantic_summary.memory_reads,
                "memory_writes": resp.semantic_summary.memory_writes,
                "return_paths": resp.semantic_summary.return_paths,
                "constants": resp.semantic_summary.constants,
            },
            "assumptions": resp.assumptions,
            "blocked_on": resp.blocked_on,
            "hypothesis": resp.hypothesis,
            "confidence": resp.confidence,
        }
        reparsed = parse_reconstruction_response(json.dumps(as_dict))
        self.assertIsNotNone(reparsed)
        self.assertEqual(reparsed.source, "void test() {}")
        self.assertEqual(reparsed.semantic_summary.calls, ["foo"])
        self.assertAlmostEqual(reparsed.confidence, 0.9)


if __name__ == "__main__":
    raise SystemExit(unittest.main())

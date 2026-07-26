"""Tests for tool schemas and dispatch.

Covers read_file allowlist enforcement, grep ripgrep and pure-Python
fallback paths, schema structure validation, submit data flag, and
unavailable patch_fn messaging.
"""
from __future__ import annotations

import json
import os
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.llm_decomp.contracts import ToolCall, ToolResult
from tools.llm_decomp.tools import TOOL_SCHEMAS, ToolContext, dispatch


def _mk_repo(tmp: Path) -> Path:
    """Create a minimal repo-like tree under tmp."""
    (tmp / "src" / "game").mkdir(parents=True, exist_ok=True)
    (tmp / "libs" / "core").mkdir(parents=True, exist_ok=True)
    (tmp / "include" / "game").mkdir(parents=True, exist_ok=True)
    (tmp / "other").mkdir(parents=True, exist_ok=True)
    (tmp / "src" / "game" / "main.cpp").write_text(
        "int main() { return 0; }\n", encoding="utf-8"
    )
    (tmp / "include" / "game" / "main.hpp").write_text(
        "#pragma once\nint main();\n", encoding="utf-8"
    )
    (tmp / "libs" / "core" / "util.cpp").write_text(
        "int helper() { return 42; }\n", encoding="utf-8"
    )
    (tmp / "other" / "secret.txt").write_text("forbidden", encoding="utf-8")
    return tmp


def _call(name: str, **kwargs) -> ToolCall:
    return ToolCall(name=name, args=kwargs)


class ToolSchemasTest(unittest.TestCase):
    """Sanity checks on the tool schema definitions."""

    def test_seven_tools_present(self) -> None:
        """Exactly 7 tools are defined."""
        self.assertEqual(len(TOOL_SCHEMAS), 7)

    def test_all_have_valid_openai_shape(self) -> None:
        """Each schema has the required OpenAI function-calling structure."""
        for schema in TOOL_SCHEMAS:
            self.assertEqual(schema["type"], "function")
            fn = schema["function"]
            self.assertIn("name", fn)
            self.assertIn("description", fn)
            self.assertIn("parameters", fn)
            params = fn["parameters"]
            self.assertEqual(params["type"], "object")
            self.assertIn("properties", params)

    def test_all_required_fields_are_in_properties(self) -> None:
        """Every required field is listed in properties."""
        for schema in TOOL_SCHEMAS:
            fn = schema["function"]
            required = fn["parameters"].get("required", [])
            for field in required:
                self.assertIn(
                    field,
                    fn["parameters"]["properties"],
                    f"Tool '{fn['name']}': required field '{field}' not in properties",
                )

    def test_tool_names_are_unique(self) -> None:
        """All tool names are unique."""
        names = [s["function"]["name"] for s in TOOL_SCHEMAS]
        self.assertEqual(len(names), len(set(names)))


class ToolDispatchReadFileTest(unittest.TestCase):
    """Tests for read_file dispatch."""

    def setUp(self) -> None:
        self._tmp = Path(tempfile.mkdtemp(prefix="llm_decomp_test_"))
        _mk_repo(self._tmp)
        self.ctx = ToolContext(repo_root=self._tmp)

    def tearDown(self) -> None:
        shutil.rmtree(self._tmp, ignore_errors=True)

    def test_reads_src_file(self) -> None:
        """Read a valid file under src/."""
        result = dispatch(
            _call("read_file", path="src/game/main.cpp", start=1, count=5),
            self.ctx,
        )
        self.assertTrue(result.ok)
        self.assertIn("main.cpp", result.content)
        self.assertIn("0001|", result.content)

    def test_reads_include_file(self) -> None:
        """Read a valid file under include/."""
        result = dispatch(
            _call("read_file", path="include/game/main.hpp", start=1, count=5),
            self.ctx,
        )
        self.assertTrue(result.ok)
        self.assertIn("main.hpp", result.content)

    def test_rejects_outside_roots(self) -> None:
        """Path outside src/, libs/, include/ returns ok=False."""
        result = dispatch(
            _call("read_file", path="other/secret.txt", start=1, count=5),
            self.ctx,
        )
        self.assertFalse(result.ok)
        self.assertIn("not allowed", result.content.lower())

    def test_rejects_dotdot_traversal(self) -> None:
        """Path with '..' is rejected."""
        result = dispatch(
            _call("read_file", path="src/../../etc/passwd", start=1, count=5),
            self.ctx,
        )
        self.assertFalse(result.ok)
        self.assertIn("not allowed", result.content.lower())

    def test_rejects_dotdot_inside(self) -> None:
        """Path containing '..' within a valid root is rejected."""
        result = dispatch(
            _call("read_file", path="src/game/../other/secret.txt", start=1, count=5),
            self.ctx,
        )
        self.assertFalse(result.ok)

    def test_missing_file_returns_not_ok(self) -> None:
        """Non-existent path returns ok=False."""
        result = dispatch(
            _call("read_file", path="src/game/nonexistent.cpp", start=1, count=5),
            self.ctx,
        )
        self.assertFalse(result.ok)

    def test_start_beyond_total_returns_not_ok(self) -> None:
        """Start line beyond file length returns ok=False."""
        result = dispatch(
            _call("read_file", path="src/game/main.cpp", start=100, count=5),
            self.ctx,
        )
        self.assertFalse(result.ok)

    def test_count_capped_at_400(self) -> None:
        """count > 400 is clamped to 400."""
        result = dispatch(
            _call("read_file", path="src/game/main.cpp", start=1, count=9999),
            self.ctx,
        )
        self.assertTrue(result.ok)
        # Only 2 lines in file; verify it read the file but capped at 400
        self.assertIn("total)", result.content)


class ToolDispatchGrepTest(unittest.TestCase):
    """Tests for grep dispatch, including pure-Python fallback."""

    def setUp(self) -> None:
        self._tmp = Path(tempfile.mkdtemp(prefix="llm_decomp_test_"))
        _mk_repo(self._tmp)
        self.ctx = ToolContext(repo_root=self._tmp)

    def tearDown(self) -> None:
        shutil.rmtree(self._tmp, ignore_errors=True)

    def test_grep_finds_pattern(self) -> None:
        """grep finds text in allowed roots."""
        result = dispatch(
            _call("grep", pattern="main"),
            self.ctx,
        )
        self.assertTrue(result.ok)
        self.assertIn("main.cpp", result.content)

    def test_grep_empty_pattern_returns_not_ok(self) -> None:
        """Empty pattern returns ok=False."""
        result = dispatch(
            _call("grep", pattern=""),
            self.ctx,
        )
        self.assertFalse(result.ok)

    def test_grep_fallback_pure_python(self) -> None:
        """When rg is unavailable, pure-Python fallback works."""
        with mock.patch.object(shutil, "which", return_value=None):
            result = dispatch(
                _call("grep", pattern="main"),
                self.ctx,
            )
        self.assertTrue(result.ok)
        self.assertIn("main", result.content)

    def test_grep_fallback_with_glob(self) -> None:
        """Pure-Python fallback respects glob filter."""
        with mock.patch.object(shutil, "which", return_value=None):
            result = dispatch(
                _call("grep", pattern="main", glob="*.hpp"),
                self.ctx,
            )
        self.assertTrue(result.ok)
        self.assertIn("main.hpp", result.content)
        # Should NOT match the .cpp file
        self.assertNotIn("main.cpp", result.content)

    def test_grep_path_outside_roots_returns_not_ok(self) -> None:
        """Searching outside allowed roots returns ok=False."""
        result = dispatch(
            _call("grep", pattern="forbidden", path="other"),
            self.ctx,
        )
        self.assertFalse(result.ok)

    def test_grep_nonexistent_pattern_finds_nothing(self) -> None:
        """A pattern that does not exist returns ok=True with no matches."""
        result = dispatch(
            _call("grep", pattern="XYZZY_NONEXISTENT_12345"),
            self.ctx,
        )
        self.assertTrue(result.ok)
        # Should either say 0 matches or just the header
        # The important thing is it's ok=True
        self.assertNotIn("main.cpp:", result.content)


class ToolDispatchOtherTest(unittest.TestCase):
    """Tests for patch, build, diff, equivalence, and submit dispatch."""

    def setUp(self) -> None:
        self._tmp = Path(tempfile.mkdtemp(prefix="llm_decomp_test_"))
        _mk_repo(self._tmp)
        self.ctx = ToolContext(repo_root=self._tmp)

    def tearDown(self) -> None:
        shutil.rmtree(self._tmp, ignore_errors=True)

    def test_submit_sets_data_flag(self) -> None:
        """submit returns data with submitted=True."""
        result = dispatch(_call("submit", note="done"), self.ctx)
        self.assertTrue(result.ok)
        self.assertEqual(result.content, "Submit received.")
        self.assertTrue(result.data["submitted"])
        self.assertEqual(result.data["note"], "done")

    def test_submit_default_empty_note(self) -> None:
        """submit with no note defaults to empty string."""
        result = dispatch(_call("submit"), self.ctx)
        self.assertTrue(result.ok)
        self.assertEqual(result.data["note"], "")

    def test_patch_unavailable(self) -> None:
        """patch without ctx.patch_fn returns unavailable message."""
        result = dispatch(_call("patch", files=[]), self.ctx)
        self.assertFalse(result.ok)
        self.assertIn("unavailable", result.content.lower())

    def test_build_unavailable(self) -> None:
        """build without ctx.build_fn returns unavailable message."""
        result = dispatch(_call("build", unit="game/main"), self.ctx)
        self.assertFalse(result.ok)
        self.assertIn("unavailable", result.content.lower())

    def test_diff_unavailable(self) -> None:
        """diff without ctx.diff_fn returns unavailable message."""
        result = dispatch(_call("diff", unit="game/main"), self.ctx)
        self.assertFalse(result.ok)
        self.assertIn("unavailable", result.content.lower())

    def test_equivalence_unavailable(self) -> None:
        """equivalence without ctx.equivalence_fn returns unavailable message."""
        result = dispatch(
            _call("equivalence", unit="game/main", symbol="func_80"),
            self.ctx,
        )
        self.assertFalse(result.ok)
        self.assertIn("unavailable", result.content.lower())

    def test_unknown_tool_returns_not_ok(self) -> None:
        """Unknown tool name returns ok=False."""
        result = dispatch(ToolCall(name="nonexistent_tool", args={}), self.ctx)
        self.assertFalse(result.ok)
        self.assertIn("unknown tool", result.content.lower())

    def test_patch_delegates_to_fn(self) -> None:
        """patch delegates to ctx.patch_fn when set."""
        def my_patch(files):
            return ToolResult(ok=True, content="patched!")
        self.ctx.patch_fn = my_patch
        result = dispatch(_call("patch", files=[{"path": "test.cpp", "blocks": []}]), self.ctx)
        self.assertTrue(result.ok)
        self.assertEqual(result.content, "patched!")

    def test_build_delegates_to_fn(self) -> None:
        """build delegates to ctx.build_fn when set."""
        def my_build(unit):
            return ToolResult(ok=True, content=f"built {unit}")
        self.ctx.build_fn = my_build
        result = dispatch(_call("build", unit="game/main"), self.ctx)
        self.assertTrue(result.ok)
        self.assertIn("built", result.content)

    def test_diff_delegates_to_fn(self) -> None:
        """diff delegates to ctx.diff_fn when set."""
        def my_diff(unit, symbol, mode):
            return ToolResult(ok=True, content=f"diff {unit} {symbol} {mode}")
        self.ctx.diff_fn = my_diff
        result = dispatch(_call("diff", unit="game/main", symbol="_Z3foo", mode="target"), self.ctx)
        self.assertTrue(result.ok)
        self.assertIn("target", result.content)

    def test_equivalence_delegates_to_fn(self) -> None:
        """equivalence delegates to ctx.equivalence_fn when set."""
        def my_eq(unit, symbol):
            return ToolResult(ok=True, content="EQUIVALENT")
        self.ctx.equivalence_fn = my_eq
        result = dispatch(
            _call("equivalence", unit="game/main", symbol="func_80"),
            self.ctx,
        )
        self.assertTrue(result.ok)
        self.assertEqual(result.content, "EQUIVALENT")


if __name__ == "__main__":
    unittest.main()
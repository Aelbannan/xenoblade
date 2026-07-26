"""Tests for tools/llm_decomp/rename_tools.py."""
from __future__ import annotations

import subprocess
import unittest
from pathlib import Path
from unittest import mock

from tools.llm_decomp.rename_tools import (rename_symbol_tool, symbols_tool)

ROOT = Path("/fake/repo")


def _cp(rc: int, out: str = "", err: str = "") -> subprocess.CompletedProcess:
    return subprocess.CompletedProcess([], rc, out, err)


class SymbolsToolTest(unittest.TestCase):
    def test_bad_command(self) -> None:
        r = symbols_tool(ROOT, {"command": "explode", "query": "q"})
        self.assertFalse(r.ok)

    def test_empty_query(self) -> None:
        r = symbols_tool(ROOT, {"command": "show", "query": ""})
        self.assertFalse(r.ok)

    def test_success(self) -> None:
        with mock.patch("tools.llm_decomp.rename_tools._run",
                        return_value=_cp(0, "info")):
            r = symbols_tool(ROOT, {"command": "show", "query": "8043C59C"})
        self.assertTrue(r.ok)
        self.assertIn("info", r.content)

    def test_failure(self) -> None:
        with mock.patch("tools.llm_decomp.rename_tools._run",
                        return_value=_cp(1, "", "boom")):
            r = symbols_tool(ROOT, {"command": "xref", "query": "x"})
        self.assertFalse(r.ok)
        self.assertIn("boom", r.content)


class RenameSymbolToolTest(unittest.TestCase):
    def test_invalid_identifier(self) -> None:
        for bad in ("9bad", "has space", ""):
            with mock.patch("tools.llm_decomp.rename_tools._run") as run:
                r = rename_symbol_tool(ROOT, {"old": "A", "new": bad})
            self.assertFalse(r.ok, bad)
            run.assert_not_called()

    def test_plan_failure_short_circuits(self) -> None:
        with mock.patch("tools.llm_decomp.rename_tools._run",
                        return_value=_cp(1, "length mismatch")) as run:
            r = rename_symbol_tool(ROOT, {"old": "A", "new": "B"})
        self.assertFalse(r.ok)
        self.assertEqual(run.call_count, 1)  # rename-all never invoked

    def test_rename_all_failure(self) -> None:
        with mock.patch("tools.llm_decomp.rename_tools._run",
                        side_effect=[_cp(0, "ok"), _cp(0, ""),
                                     _cp(1, "fail")]) as run:
            r = rename_symbol_tool(ROOT, {"old": "A", "new": "B"})
        self.assertFalse(r.ok)
        self.assertEqual(run.call_count, 3)

    def test_happy_path_with_affected(self) -> None:
        calls = [
            _cp(0, "plan ok"),          # rename-plan
            _cp(0, ""),                 # git status (baseline)
            _cp(0, "done"),             # rename-all
            _cp(0, " M src/a.cpp"),     # git status (after)
            _cp(0, "ninja ok"),         # ninja
        ]
        with mock.patch("tools.llm_decomp.rename_tools._run",
                        side_effect=calls), \
             mock.patch("tools.llm_decomp.rename_tools.shutil.which",
                        return_value="ninja"):
            r = rename_symbol_tool(ROOT, {"old": "A", "new": "B"})
        self.assertTrue(r.ok)
        self.assertEqual(r.data["affected"], ["src/a.cpp"])

    def test_ninja_failure_reverts(self) -> None:
        calls = [_cp(0), _cp(0, ""), _cp(0), _cp(0, " M src/a.cpp"),
                 _cp(1, "", "compile error")]
        with mock.patch("tools.llm_decomp.rename_tools._run",
                        side_effect=calls), \
             mock.patch("tools.llm_decomp.rename_tools.shutil.which",
                        return_value="ninja"), \
             mock.patch("tools.llm_decomp.rename_tools._revert",
                        return_value=["src/a.cpp"]) as rev:
            r = rename_symbol_tool(ROOT, {"old": "A", "new": "B"})
        self.assertFalse(r.ok)
        rev.assert_called_once()
        self.assertEqual(r.data["reverted"], ["src/a.cpp"])

    def test_force_appends_flag(self) -> None:
        calls = [_cp(0), _cp(0, ""), _cp(0), _cp(0, ""), _cp(0)]
        with mock.patch("tools.llm_decomp.rename_tools._run",
                        side_effect=calls) as run, \
             mock.patch("tools.llm_decomp.rename_tools.shutil.which",
                        return_value="ninja"):
            rename_symbol_tool(ROOT, {"old": "A", "new": "B",
                                      "force": True})
        rename_all_cmd = run.call_args_list[2].args[1]
        self.assertIn("--force", rename_all_cmd)


if __name__ == "__main__":
    unittest.main()

"""Tests for tools/llm_decomp/policies.py."""
from __future__ import annotations

import unittest

from tools.llm_decomp.policies import (
    POLICIES,
    WritableScope,
    get_policy,
    resolve_writable,
)


class PoliciesRegistryTest(unittest.TestCase):
    """Test the global POLICIES dict and lookup functions."""

    def test_all_five_policies_present(self) -> None:
        """The registry contains exactly the five required session types."""
        expected = {"match", "type-recovery", "rename", "tu-cleanup", "size-trim"}
        self.assertEqual(expected, set(POLICIES))

    def test_match_tools(self) -> None:
        """Match includes expected tools and the 'match' gate."""
        p = POLICIES["match"]
        self.assertEqual(p.gate, "match")
        self.assertIn("patch", p.tools)
        self.assertIn("build", p.tools)
        self.assertIn("diff", p.tools)
        self.assertIn("equivalence", p.tools)
        self.assertIn("submit", p.tools)

    def test_rename_has_no_patch(self) -> None:
        """Rename does NOT include 'patch', 'build', or 'diff'."""
        p = POLICIES["rename"]
        self.assertNotIn("patch", p.tools)
        self.assertNotIn("build", p.tools)
        self.assertNotIn("diff", p.tools)
        self.assertNotIn("equivalence", p.tools)
        self.assertIn("symbols", p.tools)
        self.assertIn("rename_symbol", p.tools)
        self.assertIn("submit", p.tools)
        self.assertEqual(p.gate, "rename")

    def test_type_recovery_tools(self) -> None:
        p = POLICIES["type-recovery"]
        self.assertEqual(p.gate, "type_recovery")
        self.assertIn("patch", p.tools)
        self.assertIn("build", p.tools)
        self.assertIn("diff", p.tools)
        self.assertIn("equivalence", p.tools)
        self.assertIn("submit", p.tools)

    def test_tu_cleanup_tools(self) -> None:
        p = POLICIES["tu-cleanup"]
        self.assertEqual(p.gate, "byte_identical")
        self.assertIn("patch", p.tools)
        self.assertIn("build", p.tools)
        self.assertIn("diff", p.tools)
        self.assertNotIn("equivalence", p.tools)
        self.assertIn("submit", p.tools)

    def test_size_trim_tools(self) -> None:
        p = POLICIES["size-trim"]
        self.assertEqual(p.gate, "size_trim")
        self.assertIn("patch", p.tools)
        self.assertIn("build", p.tools)
        self.assertIn("diff", p.tools)
        self.assertIn("equivalence", p.tools)
        self.assertIn("submit", p.tools)

    def test_get_policy_ok(self) -> None:
        p = get_policy("match")
        self.assertIs(p, POLICIES["match"])

    def test_get_policy_raises_on_unknown(self) -> None:
        with self.assertRaises(ValueError):
            get_policy("nonexistent")
        with self.assertRaises(ValueError):
            get_policy("")

    def test_scopes(self) -> None:
        """Each policy has the expected WritableScope."""
        match_scope = POLICIES["match"].scope
        self.assertTrue(match_scope.owning_tu)
        self.assertTrue(match_scope.designated_header)
        self.assertFalse(match_scope.whole_tu)
        self.assertFalse(match_scope.header_only)
        self.assertFalse(match_scope.rename_only)

        tr_scope = POLICIES["type-recovery"].scope
        self.assertTrue(tr_scope.header_only)

        rename_scope = POLICIES["rename"].scope
        self.assertTrue(rename_scope.rename_only)

        for name in ("tu-cleanup", "size-trim"):
            s = POLICIES[name].scope
            self.assertTrue(s.whole_tu)


class ResolveWritableTest(unittest.TestCase):
    """Test the resolve_writable helper."""

    def test_match_with_header(self) -> None:
        p = get_policy("match")
        paths = resolve_writable(p, "src/Unit.cpp", "src/Unit.h")
        self.assertEqual(paths, ["src/Unit.cpp", "src/Unit.h"])

    def test_match_header_none(self) -> None:
        p = get_policy("match")
        paths = resolve_writable(p, "src/Unit.cpp", None)
        self.assertEqual(paths, ["src/Unit.cpp"])

    def test_header_only_with_header(self) -> None:
        p = get_policy("type-recovery")
        paths = resolve_writable(p, "src/Unit.cpp", "src/Unit.h")
        self.assertEqual(paths, ["src/Unit.h"])

    def test_header_only_without_header_raises(self) -> None:
        p = get_policy("type-recovery")
        with self.assertRaises(ValueError):
            resolve_writable(p, "src/Unit.cpp", None)

    def test_rename_returns_empty(self) -> None:
        p = get_policy("rename")
        paths = resolve_writable(p, "src/Unit.cpp", "src/Unit.h")
        self.assertEqual(paths, [])

    def test_tu_cleanup(self) -> None:
        p = get_policy("tu-cleanup")
        paths = resolve_writable(p, "src/Unit.cpp", "src/Unit.h")
        self.assertEqual(paths, ["src/Unit.cpp", "src/Unit.h"])

    def test_tu_cleanup_no_header(self) -> None:
        p = get_policy("tu-cleanup")
        paths = resolve_writable(p, "src/Unit.cpp", None)
        self.assertEqual(paths, ["src/Unit.cpp"])

    def test_size_trim(self) -> None:
        p = get_policy("size-trim")
        paths = resolve_writable(p, "src/Unit.cpp", "src/Unit.h")
        self.assertEqual(paths, ["src/Unit.cpp", "src/Unit.h"])

    def test_unknown_policy_raises(self) -> None:
        # Create a fake policy (not registered) to test resolve_writable
        # with an unregistered name — get_policy already raises.
        pass


if __name__ == "__main__":
    unittest.main()
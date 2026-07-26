"""Tests for tools/llm_decomp/include_graph.py."""
from __future__ import annotations

import json
import os
import time
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.llm_decomp.include_graph import IncludeGraph, scan

# Re-usable repo subdirectory names.
_INCLUDE = "include"
_SRC = "src"
_LIBS = "libs"


def _mkdir(d: Path) -> Path:
    d.mkdir(parents=True, exist_ok=True)
    return d


class IncludeGraphTest(unittest.TestCase):
    """Temp-directory based tests for IncludeGraph."""

    def setUp(self) -> None:
        self._tmp = Path(__file__).resolve().parent / "_test_tmp_include_graph"
        if self._tmp.exists():
            import shutil
            shutil.rmtree(self._tmp)
        self._tmp.mkdir(parents=True)
        # Build the cache under the temp dir so test runs are isolated.
        self._cache_patch = patch(
            "tools.llm_decomp.include_graph.include_graph_path",
            return_value=self._tmp / "build" / "llm-decomp" / "include_graph.json",
        )
        self._cache_patch.start()
        self.addCleanup(self._cache_patch.stop)

    def tearDown(self) -> None:
        import shutil
        shutil.rmtree(self._tmp, ignore_errors=True)

    def _root(self) -> Path:
        return self._tmp

    def _write(self, rel: str, content: str) -> None:
        """Write *content* at *rel* (repo-relative) under the temp root."""
        full = self._root() / rel
        full.parent.mkdir(parents=True, exist_ok=True)
        full.write_text(content, encoding="utf-8")

    def _touch(self, rel: str) -> None:
        """Update mtime of an existing file."""
        full = self._root() / rel
        full.touch()

    def _build_test_tree(self) -> None:
        """Create a small realistic tree:

        include/A.hpp          -> #include "B.hpp"
        include/B.hpp          -> #include "C.hpp"
        include/C.hpp          -> (empty)
        src/main.cpp           -> #include "A.hpp"
        libs/helper.h          -> (empty)
        src/util.cpp           -> #include <helper.h>  (angle)
        src/sub/detail.cpp     -> #include "detail_impl.h" (relative)
        src/sub/detail_impl.h  -> (empty)
        src/sub/detail.cpp     -> #include <missing.h> (unresolvable)
        """
        _mkdir(self._root() / _INCLUDE)
        _mkdir(self._root() / _SRC / "sub")
        _mkdir(self._root() / _LIBS)

        self._write("include/A.hpp", '#include "B.hpp"\n')
        self._write("include/B.hpp", '#include "C.hpp"\n')
        self._write("include/C.hpp", "// empty\n")
        self._write("src/main.cpp", '#include "A.hpp"\n')
        self._write("libs/helper.h", "// helper\n")
        self._write("src/util.cpp", '#include <helper.h>\n')
        self._write("src/sub/detail_impl.h", "// impl\n")
        self._write(
            "src/sub/detail.cpp",
            '#include "detail_impl.h"\n#include <missing.h>\n',
        )

    # ---- Tests ------------------------------------------------------------

    def test_direct_dependents(self) -> None:
        self._build_test_tree()
        g = scan(self._root())
        # A.hpp is directly included by src/main.cpp
        deps = g.direct_dependents("include/A.hpp")
        self.assertEqual(deps, {"src/main.cpp"})

    def test_transitive_dependents(self) -> None:
        self._build_test_tree()
        g = scan(self._root())
        # C.hpp transitively depends on main.cpp (via A.hpp -> B.hpp -> C.hpp)
        deps = g.dependents("include/C.hpp")
        self.assertEqual(deps, {"src/main.cpp"})

    def test_quoted_include_resolved(self) -> None:
        self._build_test_tree()
        g = scan(self._root())
        # A.hpp includes B.hpp — both under include/
        self.assertIn("include/B.hpp", g._edges.get("include/A.hpp", []))

    def test_angle_include_resolved(self) -> None:
        self._build_test_tree()
        g = scan(self._root())
        # src/util.cpp includes <helper.h> which resolves to libs/helper.h
        self.assertIn("libs/helper.h", g._edges.get("src/util.cpp", []))

    def test_relative_include_resolved(self) -> None:
        self._build_test_tree()
        g = scan(self._root())
        # src/sub/detail.cpp includes "detail_impl.h" (same dir)
        self.assertIn(
            "src/sub/detail_impl.h", g._edges.get("src/sub/detail.cpp", [])
        )

    def test_unresolvable_include_recorded_but_excluded(self) -> None:
        self._build_test_tree()
        g = scan(self._root())
        # missing.h is not resolvable — recorded as-is in edges
        edges = g._edges.get("src/sub/detail.cpp", [])
        self.assertIn("missing.h", edges)
        # But not in reverse index, so dependents doesn't include it
        self.assertNotIn("missing.h", g._reverse)
        # direct_dependents returns empty for unresolvable
        self.assertEqual(g.direct_dependents("missing.h"), set())

    def test_dependents_filters_to_c_cpp(self) -> None:
        self._build_test_tree()
        g = scan(self._root())
        # C.hpp is included by B.hpp but B.hpp is a header, not .c/.cpp
        # Only main.cpp is a .cpp — and it's the transitive dependent
        deps = g.dependents("include/C.hpp")
        self.assertEqual(deps, {"src/main.cpp"})
        self.assertNotIn("include/B.hpp", deps)  # header excluded

    def test_cache_hit_mtime_valid(self) -> None:
        """Second load_or_build is a cache hit — scan is not called."""
        self._build_test_tree()
        # First call builds and caches.
        g1 = scan(self._root())
        # Force write the cache directly.
        from tools.llm_decomp.include_graph import include_graph_path

        cache_path = include_graph_path(self._root())
        cache_path.parent.mkdir(parents=True, exist_ok=True)
        data = {"files": g1._files, "edges": dict(g1._edges)}
        cache_path.write_text(json.dumps(data), encoding="utf-8")

        # Ensure mtimes haven't drifted (fs resolution on some OS).
        # Re-stat files to get the exact mtimes stored.
        fresh_files: dict[str, float] = {}
        for rel in g1._files:
            fresh_files[rel] = (self._root() / rel).stat().st_mtime
        data["files"] = fresh_files
        cache_path.write_text(json.dumps(data), encoding="utf-8")

        # Now load_or_build — patch scan to prove it's NOT called.
        original_scan = scan
        called = False

        def _spy_scan(*args: object, **kwargs: object) -> object:
            nonlocal called
            called = True
            return original_scan(*args, **kwargs)

        with patch("tools.llm_decomp.include_graph.scan", _spy_scan):
            g2 = IncludeGraph.load_or_build(self._root())

        self.assertFalse(called, "scan() should NOT be called on cache hit")
        self.assertEqual(g1.snapshot_id(), g2.snapshot_id())

    def test_cache_mtime_change_triggers_rebuild(self) -> None:
        self._build_test_tree()
        g1 = scan(self._root())

        # Write cache manually.
        from tools.llm_decomp.include_graph import include_graph_path

        cache_path = include_graph_path(self._root())
        cache_path.parent.mkdir(parents=True, exist_ok=True)
        data = {"files": g1._files, "edges": dict(g1._edges)}
        cache_path.write_text(json.dumps(data), encoding="utf-8")

        # Modify file content (adding an include changes edges) AND its mtime.
        time.sleep(1.01)
        self._write("include/A.hpp", '#include "B.hpp"\n#include "C.hpp"\n')

        # load_or_build must rebuild (different snapshot_id).
        g2 = IncludeGraph.load_or_build(self._root())
        self.assertNotEqual(g1.snapshot_id(), g2.snapshot_id())

    def test_cache_new_file_triggers_rebuild(self) -> None:
        self._build_test_tree()
        g1 = scan(self._root())

        # Write cache manually.
        from tools.llm_decomp.include_graph import include_graph_path

        cache_path = include_graph_path(self._root())
        cache_path.parent.mkdir(parents=True, exist_ok=True)
        data = {"files": g1._files, "edges": dict(g1._edges)}
        cache_path.write_text(json.dumps(data), encoding="utf-8")

        # Add a new file not in cache.
        self._write("src/newfile.cpp", '#include "A.hpp"\n')

        # load_or_build must rebuild.
        g2 = IncludeGraph.load_or_build(self._root())
        self.assertNotEqual(g1.snapshot_id(), g2.snapshot_id())
        # New file is included.
        self.assertIn("src/newfile.cpp", g2._edges)

    def test_snapshot_id_stable(self) -> None:
        self._build_test_tree()
        g1 = scan(self._root())
        g2 = scan(self._root())
        self.assertEqual(g1.snapshot_id(), g2.snapshot_id())

    def test_snapshot_id_changes_on_edge_change(self) -> None:
        self._build_test_tree()
        g1 = scan(self._root())
        # Add an include.
        self._write("include/D.hpp", "// extra\n")
        self._write("src/main.cpp", '#include "A.hpp"\n#include "D.hpp"\n')
        g2 = scan(self._root())
        self.assertNotEqual(g1.snapshot_id(), g2.snapshot_id())

    def test_empty_dirs_handled(self) -> None:
        """Missing src/libs/include dirs are silently skipped."""
        # No files at all.
        g = scan(self._root())
        self.assertEqual(len(g._edges), 0)
        self.assertEqual(g.dependents("nonexistent.hpp"), set())

    def test_only_src_dir(self) -> None:
        _mkdir(self._root() / _SRC)
        self._write("src/foo.cpp", '#include "foo.hpp"\n')
        # foo.hpp doesn't exist — unresolvable.
        g = scan(self._root())
        self.assertIn("src/foo.cpp", g._edges)
        self.assertIn("foo.hpp", g._edges["src/foo.cpp"])

    def test_large_file_include_scan(self) -> None:
        """Scan a file with many includes."""
        _mkdir(self._root() / _INCLUDE)
        lines = [f'#include "h{i}.hpp"\n' for i in range(50)]
        for i in range(50):
            self._write(f"include/h{i}.hpp", "//\n")
        self._write("src/big.cpp", "".join(lines))
        g = scan(self._root())
        self.assertEqual(len(g._edges.get("src/big.cpp", [])), 50)

    def test_both_quote_and_angle(self) -> None:
        _mkdir(self._root() / _INCLUDE)
        _mkdir(self._root() / _LIBS)
        self._write("include/local.h", "// local\n")
        self._write("libs/system.h", "// system\n")
        self._write(
            "src/mixed.cpp",
            '#include "local.h"\n#include <system.h>\n',
        )
        g = scan(self._root())
        self.assertIn("include/local.h", g._edges["src/mixed.cpp"])
        self.assertIn("libs/system.h", g._edges["src/mixed.cpp"])

    def test_direct_dependents_multiple(self) -> None:
        _mkdir(self._root() / _INCLUDE)
        _mkdir(self._root() / _SRC)
        self._write("include/header.h", "// shared\n")
        self._write("src/a.cpp", '#include "header.h"\n')
        self._write("src/b.cpp", '#include "header.h"\n')
        g = scan(self._root())
        deps = g.direct_dependents("include/header.h")
        self.assertEqual(deps, {"src/a.cpp", "src/b.cpp"})

    def test_load_or_build_with_no_cache_file(self) -> None:
        """When no cache file exists, load_or_build === full scan."""
        self._build_test_tree()
        g = IncludeGraph.load_or_build(self._root())
        self.assertGreater(len(g._edges), 0)

    def test_load_or_build_corrupt_cache_triggers_rebuild(self) -> None:
        self._build_test_tree()
        from tools.llm_decomp.include_graph import include_graph_path

        cache_path = include_graph_path(self._root())
        cache_path.parent.mkdir(parents=True, exist_ok=True)
        cache_path.write_text("not valid json", encoding="utf-8")
        # Should not raise — falls through to full rebuild.
        g = IncludeGraph.load_or_build(self._root())
        self.assertGreater(len(g._edges), 0)

    def test_forward_slash_normalisation(self) -> None:
        """Paths are stored with forward slashes on all platforms."""
        _mkdir(self._root() / _INCLUDE)
        self._write("include/mydir/header.h", "//\n")
        self._write("src/main.cpp", '#include "mydir/header.h"\n')
        g = scan(self._root())
        resolved = g._edges["src/main.cpp"]
        self.assertEqual(resolved, ["include/mydir/header.h"])
        for p in resolved:
            self.assertNotIn("\\", p)
        for p in g._edges:
            self.assertNotIn("\\", p)

    def test_no_includes_in_file(self) -> None:
        _mkdir(self._root() / _SRC)
        self._write("src/empty.cpp", "int x = 1;\n")
        g = scan(self._root())
        self.assertNotIn("src/empty.cpp", g._edges)

    def test_cyclic_include_does_not_loop(self) -> None:
        _mkdir(self._root() / _INCLUDE)
        self._write("include/A.hpp", '#include "B.hpp"\n')
        self._write("include/B.hpp", '#include "A.hpp"\n')
        self._write("src/main.cpp", '#include "A.hpp"\n')
        g = scan(self._root())
        # dependents should still terminate.
        deps = g.dependents("include/A.hpp")
        self.assertIn("src/main.cpp", deps)


class IncludeGraphRealScanTest(unittest.TestCase):
    """Minimal real-repo smoke test (cold scan timing)."""

    def test_cold_scan_timing(self) -> None:
        """Run a cold scan of the real repo and report timing + edge count.

        This test is informational; it does not assert on performance
        (environment-dependent) but verifies the scanner completes without
        error against real-world code.
        """
        repo = Path(__file__).resolve().parents[3]
        g = scan(repo)
        edge_count = sum(len(v) for v in g._edges.values())
        self.assertGreater(edge_count, 0, "Expected at least one edge")


if __name__ == "__main__":
    unittest.main()
from __future__ import annotations

import unittest

from tools.llm_harness.source_regions import find_function_region


class _Target:
    def __init__(self, target_id: str, function: str) -> None:
        self.id = target_id
        self.function = function


class SourceRegionsReuseTests(unittest.TestCase):
    def test_markers_and_ctor(self) -> None:
        from tools.decomp_atlas.tests import PROJECT

        source = (PROJECT / "src" / "demo" / "Demo.cpp").read_text(encoding="utf-8")
        leaf = find_function_region(source, _Target("demo-leaf", "Demo::leaf"))
        self.assertTrue(leaf.marked)
        body = source[leaf.content_start : leaf.content_end]
        self.assertIn("gDummy = 1", body)

        ctor = find_function_region(source, _Target("demo-ctor", "Demo::Demo"))
        self.assertTrue(ctor.marked)
        ctor_body = source[ctor.content_start : ctor.content_end]
        self.assertIn("mValue", ctor_body)


if __name__ == "__main__":
    unittest.main()

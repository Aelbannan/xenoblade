"""tools/coop/probe.py: lock-free private compile command rewrite + hexdiff --decomp-obj."""
from __future__ import annotations

import unittest
from pathlib import Path

from tools.coop import hexdiff, probe

CMD = ["build/tools/wibo", "build/tools/sjiswrap.exe", "mwcceppc.exe", "-O4,p",
       "-i", "include", "-i", "src/", "-lang=c++", "-MMD",
       "-c", "src/kyoshin/menu/CMenuX.cpp", "-o", "build/us/src/kyoshin/menu"]


class RewriteTests(unittest.TestCase):
    def test_working_tree_probe_only_moves_output(self):
        out, src = probe._rewrite(CMD, None, Path("/tmp/xb/probe/w"), [])
        self.assertEqual(src, "src/kyoshin/menu/CMenuX.cpp")
        self.assertEqual(out[out.index("-o") + 1], "/tmp/xb/probe/w")
        self.assertEqual(out[out.index("-c") + 1], "src/kyoshin/menu/CMenuX.cpp")
        self.assertNotIn("build/us/src/kyoshin/menu", out)
        self.assertEqual(out.count("-i"), 2)

    def test_scratch_source_keeps_sibling_headers_and_overlay_first(self):
        out, _ = probe._rewrite(CMD, Path("/tmp/xb/A/CMenuX.cpp"), Path("/tmp/w"),
                                [Path("/tmp/xb/A/hdr")])
        incs = [out[i + 1] for i, a in enumerate(out) if a == "-i"]
        self.assertEqual(incs[0], "/tmp/xb/A/hdr")
        self.assertTrue(incs[1].endswith("src/kyoshin/menu"))
        self.assertEqual(incs[2:], ["include", "src/"])
        self.assertEqual(out[out.index("-c") + 1], "/tmp/xb/A/CMenuX.cpp")

    def test_scratch_source_must_keep_basename(self):
        with self.assertRaises(SystemExit):
            probe._rewrite(CMD, Path("/tmp/xb/A/CMenuX_v2.cpp"), Path("/tmp/w"), [])

    def test_input_command_not_mutated(self):
        before = list(CMD)
        probe._rewrite(CMD, Path("/tmp/xb/A/CMenuX.cpp"), Path("/tmp/w"), [Path("/tmp/o")])
        self.assertEqual(CMD, before)


class HexdiffDecompObjTests(unittest.TestCase):
    def test_decomp_obj_implies_no_build(self):
        args = hexdiff._parse_args(["kyoshin/X", "--all", "--decomp-obj", "/tmp/x.o"])
        self.assertTrue(args.no_build)
        self.assertEqual(args.decomp_obj, "/tmp/x.o")

    def test_default_still_builds(self):
        args = hexdiff._parse_args(["kyoshin/X", "--all"])
        self.assertFalse(args.no_build)
        self.assertIsNone(args.decomp_obj)


if __name__ == "__main__":
    unittest.main()

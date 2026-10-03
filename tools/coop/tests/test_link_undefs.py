"""Frontier accounting: object-level missing symbols vs linker-provided ones.

`link_undefs.py` censuses object files, so symbols the *linker* provides
(``build/us/ldscript.lcf`` absolutes, EABI small-data bases) must be split out
of the frontier or the "what is left" number overstates object work.
"""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.reloc_canon import ldscript_assigned_names
from tools.coop.link_undefs import (
    EABI_LINKER_SYMBOLS,
    ROOT,
    ldscript_provided,
    split_missing,
)

SYNTHETIC_LCF = """\
MEMORY
{
    text : origin = 0x80004000
}

SECTIONS
{
    GROUP:
    {
        .text ALIGN(0x20):{}
    } > text

    _stack_end = _f_sbss2 + SIZEOF(.sbss2);
    _stack_addr = (_stack_end + 0xFFFF + 0x7) & ~0x7;
    __ArenaHi = 0x81700000;
}

FORCEACTIVE
{
    sinit_8003A4F8
}
"""


class LdscriptNames(unittest.TestCase):
    def test_collects_assignments_only(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            lcf = Path(tmp) / "ldscript.lcf"
            lcf.write_text(SYNTHETIC_LCF)
            names = ldscript_assigned_names(lcf)
        self.assertEqual(names, {"_stack_end", "_stack_addr", "__ArenaHi"})
        # block directives and MEMORY's `origin = ...` are not assignments
        self.assertNotIn("origin", names)
        self.assertNotIn("text", names)
        self.assertNotIn("sinit_8003A4F8", names)

    def test_comments_are_ignored(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            lcf = Path(tmp) / "ldscript.lcf"
            lcf.write_text("# _fake = 0x1;\n    _real = 0x2;\n")
            names = ldscript_assigned_names(lcf)
        self.assertEqual(names, {"_real"})


class FrontierSplit(unittest.TestCase):
    def test_linker_names_are_not_object_work(self) -> None:
        undef = {"_stack_addr", "func_80000000", "_SDA_BASE_", "also_def"}
        defined = {"also_def"}
        provided = {"_stack_addr", "_SDA_BASE_"}
        missing, linker = split_missing(undef, defined, provided)
        self.assertEqual(missing, ["func_80000000"])
        self.assertEqual(linker, ["_SDA_BASE_", "_stack_addr"])

    def test_object_definition_wins_over_linker_name(self) -> None:
        # A name that some object *does* define is neither missing nor
        # linker-provided, even if a script assigns it.
        missing, linker = split_missing({"x"}, {"x"}, {"x"})
        self.assertEqual((missing, linker), ([], []))

    def test_eabi_bases_are_always_provided(self) -> None:
        self.assertLessEqual({"_SDA_BASE_", "_SDA2_BASE_"}, set(EABI_LINKER_SYMBOLS))
        missing, linker = split_missing({"_SDA2_BASE_"}, set(), set(EABI_LINKER_SYMBOLS))
        self.assertEqual(missing, [])
        self.assertEqual(linker, ["_SDA2_BASE_"])

    def test_build_tree_lcf_is_the_source_of_truth(self) -> None:
        lcf = ROOT / "build" / "us" / "ldscript.lcf"
        if not lcf.is_file():
            self.skipTest("build/us/ldscript.lcf not present")
        provided, source = ldscript_provided("us")
        self.assertEqual(source, str(lcf))
        # the retail-accurate stack/arena block dtk wrote
        self.assertLessEqual(
            {"_stack_addr", "_stack_end", "_db_stack_end", "__ArenaLo", "__ArenaHi"},
            provided,
        )


if __name__ == "__main__":
    unittest.main()

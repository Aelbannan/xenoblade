"""link_gate: comparison, shrink-only ratchet, log parsing, symbols.txt dupes.

Fixture-only; the real link is never run here.
"""

from __future__ import annotations

import io
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path

from tools.coop import link_gate as lg

BASE = {
    "missing": ["a", "b"],
    "multiply_defined_names": ["m1"],
    "symbols_dupes": ["config/jp/symbols.txt: X"],
    "counts": {"multiply_defined": 1, "sda_reloc": 2, "elf_internal": 0},
}


def cur(**kw):
    out = {k: (list(v) if isinstance(v, list) else dict(v)) for k, v in BASE.items()}
    out.update(kw)
    return out


class CompareTest(unittest.TestCase):
    def test_identical_passes(self):
        self.assertEqual(lg.compare(BASE, cur()), ([], []))

    def test_new_missing_fails(self):
        reg, _ = lg.compare(BASE, cur(missing=["a", "b", "c"]))
        self.assertEqual(reg, ["new missing: c"])

    def test_new_multiply_defined_fails_even_at_same_count(self):
        reg, imp = lg.compare(BASE, cur(multiply_defined_names=["m2"]))
        self.assertIn("new multiply-defined: m2", reg)
        self.assertIn("retired multiply-defined: m1", imp)

    def test_count_increase_fails(self):
        c = cur(counts={"multiply_defined": 1, "sda_reloc": 3, "elf_internal": 1})
        reg, _ = lg.compare(BASE, c)
        self.assertEqual(reg, ["sda_reloc count rose 2 -> 3", "elf_internal count rose 0 -> 1"])

    def test_new_symbols_dupe_fails(self):
        reg, _ = lg.compare(BASE, cur(symbols_dupes=["config/jp/symbols.txt: X", "config/us/symbols.txt: Y"]))
        self.assertEqual(reg, ["new symbols.txt duplicate global: config/us/symbols.txt: Y"])

    def test_improvement_reported(self):
        reg, imp = lg.compare(BASE, cur(missing=["a"]))
        self.assertEqual(reg, [])
        self.assertEqual(imp, ["retired missing: b"])


class RatchetTest(unittest.TestCase):
    def test_only_shrinks(self):
        c = cur(missing=["a", "zzz"], counts={"multiply_defined": 5, "sda_reloc": 0, "elf_internal": 0})
        out = lg.ratchet(BASE, c)
        self.assertEqual(out["missing"], ["a"])  # zzz never added
        self.assertEqual(out["counts"], {"multiply_defined": 1, "sda_reloc": 0, "elf_internal": 0})
        self.assertEqual(out["symbols_dupes"], BASE["symbols_dupes"])

    def test_sha1_gate_inactive_until_zero_missing(self):
        self.assertFalse(lg.sha1_gate_active(BASE))
        self.assertTrue(lg.sha1_gate_active(cur(missing=[])))


class LogParseTest(unittest.TestCase):
    LOG = "\n".join([
        "[1/1] LINK build/us/main.elf",
        "### mwldeppc.exe Linker Error:",
        "#   Small data relocation (109) in function 'f'",
        "### mwldeppc.exe Linker Error:",
        "#   'dup_sym' in a.o is multiply-defined",
        "### mwldeppc.exe Linker Error:",
        "#   ELF_linker internal error",
    ])

    def test_counts_and_names(self):
        info = lg.parse_link_log(self.LOG)
        self.assertEqual(info["counts"], {"multiply_defined": 1, "sda_reloc": 1, "elf_internal": 1})
        self.assertEqual(info["multiply_defined_names"], ["dup_sym"])

    def test_link_attempted(self):
        self.assertTrue(lg.link_attempted(self.LOG, "us"))
        self.assertFalse(lg.link_attempted("FAILED: build/us/x.o\nninja: build stopped", "us"))

    def test_make_result(self):
        r = lg.make_result({"missing": [{"symbol": "b"}, {"symbol": "a"}]}, lg.parse_link_log(""))
        self.assertEqual(r["missing"], ["a", "b"])


class DuplicateGlobalsTest(unittest.TestCase):
    TEXT = "\n".join([
        "foo = .text:0x80000000; // type:function size:0x4 scope:global",
        "bar = .text:0x80000004; // type:function size:0x4 scope:global",
        "foo = .text:0x80000008; // type:function size:0x4 scope:global",
        "@1 = .data:0x80001000; // type:object size:0x4 scope:local",
        "@1 = .data:0x80001004; // type:object size:0x4 scope:local",
        "// comment = nope",
    ])

    def test_detects_global_dupes_only(self):
        self.assertEqual(lg.duplicate_globals(self.TEXT), ["foo"])

    def test_symbols_only_against_baseline(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            (root / "config" / "us").mkdir(parents=True)
            (root / "config" / "us" / "symbols.txt").write_text(self.TEXT)
            self.assertEqual(lg.check_symbols_dupes(root), ["config/us/symbols.txt: foo"])
            base = root / "b.json"
            lg.write_baseline({"symbols_dupes": ["config/us/symbols.txt: foo"]}, base)
            orig = lg.check_symbols_dupes
            try:
                lg.check_symbols_dupes = lambda: orig(root)
                with redirect_stdout(io.StringIO()), redirect_stderr(io.StringIO()):
                    self.assertEqual(lg.symbols_only(base), 0)
                    lg.write_baseline({"symbols_dupes": []}, base)
                    self.assertEqual(lg.symbols_only(base), 1)
            finally:
                lg.check_symbols_dupes = orig


if __name__ == "__main__":
    unittest.main()


class CanonicalMissingTest(unittest.TestCase):
    def test_local_labels_compare_by_count_per_referrer(self):
        before = lg.canonical_missing([
            {"symbol": "@1575", "referrers": ["build/us/src/x/CfTaskMain.reloc.o"]},
            {"symbol": "@1576", "referrers": ["build/us/src/x/CfTaskMain.reloc.o"]},
            {"symbol": "lbl_eu_80000000", "referrers": ["a.o"]},
        ])
        renumbered = lg.canonical_missing([
            {"symbol": "@1581", "referrers": ["build/us/src/x/CfTaskMain.reloc.o"]},
            {"symbol": "@1584", "referrers": ["build/us/src/x/CfTaskMain.reloc.o"]},
            {"symbol": "lbl_eu_80000000", "referrers": ["a.o"]},
        ])
        self.assertEqual(before, renumbered)
        self.assertIn("@local[CfTaskMain]#2", before)

    def test_extra_local_label_is_new(self):
        base = {"missing": lg.canonical_missing([{"symbol": "@1", "referrers": ["T.o"]}])}
        cur = {"missing": lg.canonical_missing([
            {"symbol": "@1", "referrers": ["T.o"]}, {"symbol": "@2", "referrers": ["T.o"]}])}
        regress, _ = lg.compare(base, cur)
        self.assertEqual(regress, ["new missing: @local[T]#2"])

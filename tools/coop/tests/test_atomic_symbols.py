"""Atomic symbols.txt writes (incident hardening).

Every code path that writes ``config/<region>/symbols.txt`` must write to a
temp file in the SAME directory and ``os.replace()`` it into place, so an
interrupted/partial write can never truncate the file (cf. the truncated
``config/jp/symbols.txt`` incident).
"""
from __future__ import annotations

import os
import unittest
from pathlib import Path
from tempfile import TemporaryDirectory
from unittest import mock

from tools.coop.lib import atomic_write as aw
from tools.coop import symbol_fn_rename as sfr


def _write_symbol_fixture(root: Path, *, old: str = "Widget_Paint") -> None:
    (root / "config" / "us").mkdir(parents=True, exist_ok=True)
    (root / "config" / "eu").mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    (root / "config" / "us" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000; // type:function size:0x40\n", encoding="utf-8")
    (root / "config" / "eu" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000;\n", encoding="utf-8")
    (root / "src" / "unit" / "Widget.hpp").write_text(
        "#pragma once\nclass WdgState {\npublic:\n    void Existing();\n};\n",
        encoding="utf-8")
    (root / "src" / "unit" / "Widget.cpp").write_text(
        '#include "Widget.hpp"\n'
        f'extern "C" void {old}(WdgState* self, int x, int y);\n'
        f'void {old}(WdgState* self, int x, int y) {{\n'
        '    self->value = x;\n'
        '}\n',
        encoding="utf-8")
    (root / "src" / "other" / "Caller.cpp").write_text(
        f'#include "Widget.hpp"\nvoid run() {{ {old}(a, 1, 2); }}\n',
        encoding="utf-8")
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")


class AtomicHelperTest(unittest.TestCase):
    def test_write_creates_file_atomically(self):
        with TemporaryDirectory() as td:
            target = Path(td) / "symbols.txt"
            aw.atomic_write_text(target, "hello\n")
            self.assertEqual(target.read_text(encoding="utf-8"), "hello\n")
            # No temp files left behind.
            leftovers = [p for p in Path(td).iterdir() if p.name != "symbols.txt"]
            self.assertEqual(leftovers, [])

    def test_temp_lives_in_same_directory(self):
        with TemporaryDirectory() as td:
            target = Path(td) / "sub" / "symbols.txt"
            seen_dirs: list[str] = []
            real_mkstemp = __import__("tempfile").mkstemp

            def spy_mkstemp(*a, **k):
                seen_dirs.append(k.get("dir", ""))
                return real_mkstemp(*a, **k)

            with mock.patch("tempfile.mkstemp", side_effect=spy_mkstemp):
                aw.atomic_write_text(target, "x\n")
            self.assertEqual(seen_dirs, [str(target.parent)])

    def test_replace_called(self):
        with TemporaryDirectory() as td:
            target = Path(td) / "symbols.txt"
            target.write_text("orig\n", encoding="utf-8")
            with mock.patch("os.replace", wraps=os.replace) as mrep:
                aw.atomic_write_text(target, "new\n")
            self.assertTrue(mrep.called)
            self.assertEqual(target.read_text(encoding="utf-8"), "new\n")

    def test_interrupted_write_leaves_original_intact(self):
        # Simulate a power cut at os.replace: the original file must be
        # left intact and complete, and the temp must be cleaned up.
        with TemporaryDirectory() as td:
            target = Path(td) / "symbols.txt"
            original = "Widget_Paint = .text:0x80001000;\nOther = .text:0x1;\n"
            target.write_text(original, encoding="utf-8")
            with mock.patch("os.replace", side_effect=RuntimeError("power cut")):
                with self.assertRaises(RuntimeError):
                    aw.atomic_write_text(target, "TRUNCATED-PARTIAL")
            self.assertEqual(target.read_text(encoding="utf-8"), original)
            leftovers = [p for p in Path(td).iterdir() if p.name != "symbols.txt"]
            self.assertEqual(leftovers, [])

    def test_interrupted_bytes_write_leaves_original_intact(self):
        with TemporaryDirectory() as td:
            target = Path(td) / "symbols.txt"
            original = b"a" * 65536 + b"\n"
            target.write_bytes(original)
            with mock.patch("os.replace", side_effect=OSError("disk full")):
                with self.assertRaises(OSError):
                    aw.atomic_write_bytes(target, b"partial")
            self.assertEqual(target.read_bytes(), original)


class ApplyUsesAtomicTest(unittest.TestCase):
    def test_apply_symbols_write_goes_through_atomic_helper(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_symbol_fixture(root)
            calls: list[Path] = []
            real = sfr.atomic_write_text

            def spy(path: Path, text: str, encoding: str = "utf-8"):
                calls.append(Path(path))
                return real(path, text, encoding=encoding)

            with mock.patch.object(sfr, "atomic_write_text", side_effect=spy):
                plan, _log = sfr.apply_function_rename(
                    root, "Widget_Paint", "WdgState::Paint(int, int)",
                    regions=["us"], dry_run=False,
                    demote_import_band=True,
                )
            sym_writes = [p for p in calls
                          if p.name == "symbols.txt"]
            self.assertTrue(sym_writes, "symbols.txt must go through atomic_write_text")
            self.assertTrue(all(p.parent.parent.name == "config" for p in sym_writes))
            self.assertIn("Paint__8WdgStateFii", plan.new_mangled or "")

    def test_failed_apply_leaves_symbols_complete(self):
        # Fault injection: fail the symbols.txt write itself; the original
        # file must be left intact and complete (never truncated).
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_symbol_fixture(root)
            sym = root / "config" / "us" / "symbols.txt"
            before = sym.read_bytes()
            real = sfr.atomic_write_text

            def fail_on_symbols(path: Path, text: str, encoding: str = "utf-8"):
                if Path(path).name == "symbols.txt":
                    raise RuntimeError("simulated crash mid-write")
                return real(path, text, encoding=encoding)

            with mock.patch.object(sfr, "atomic_write_text",
                                   side_effect=fail_on_symbols):
                with self.assertRaises(RuntimeError):
                    sfr.apply_function_rename(
                        root, "Widget_Paint", "WdgState::Paint(int, int)",
                        regions=["us"], dry_run=False,
                        demote_import_band=True,
                    )
            self.assertEqual(sym.read_bytes(), before)
            # Complete: still has the trailing newline and both... (single line here).
            self.assertTrue(sym.read_bytes().endswith(b"\n"))
            self.assertIn(b"Widget_Paint", sym.read_bytes())


if __name__ == "__main__":
    unittest.main()

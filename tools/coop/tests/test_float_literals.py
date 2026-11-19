"""Unit tests for the sdata2 literal pipeline's pure value layer."""
from __future__ import annotations

import importlib.util
import struct
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[3]
TOOL = ROOT / "tools" / "coop" / "faithful" / "float_literals.py"

_spec = importlib.util.spec_from_file_location("xb_float_literals", TOOL)
fl = importlib.util.module_from_spec(_spec)
sys.modules[_spec.name] = fl
_spec.loader.exec_module(fl)


def f32_bits(value: float) -> int:
    return struct.unpack(">I", struct.pack(">f", value))[0]


def f64_bits(value: float) -> int:
    return struct.unpack(">Q", struct.pack(">d", value))[0]


class ClassificationTests(unittest.TestCase):
    def test_plain_float_and_double(self) -> None:
        half = fl.classify(f32_bits(0.5), 4, "float")
        self.assertEqual(half.kind, fl.KIND_FLOAT)
        self.assertEqual(half.spelling, "0.5f")
        self.assertTrue(half.replaceable)

        three = fl.classify(f64_bits(3.0), 8, "double")
        self.assertEqual(three.kind, fl.KIND_DOUBLE)
        self.assertEqual(three.spelling, "3.0")
        self.assertTrue(three.replaceable)

    def test_shared_zero_and_one(self) -> None:
        for bits, size, kind in (
            (0x00000000, 4, "float"),
            (0x3F800000, 4, "float"),
            (0x0000000000000000, 8, "double"),
            (0x3FF0000000000000, 8, "double"),
        ):
            with self.subTest(bits=bits):
                value = fl.classify(bits, size, kind)
                self.assertEqual(value.kind, fl.KIND_SHARED)
                self.assertFalse(value.replaceable)
                self.assertIn("0.0/1.0", value.reason)

    def test_magic_doubles_are_not_literals(self) -> None:
        for bits in (0x4330000080000000, 0x4330000000000000):
            with self.subTest(bits=hex(bits)):
                value = fl.classify(bits, 8, "double")
                self.assertEqual(value.kind, fl.KIND_MAGIC)
                self.assertIsNone(value.spelling)
                self.assertIn("cast", value.reason)

    def test_nan_and_inf_stay_named(self) -> None:
        for bits, size, kind in (
            (0x7FC00000, 4, "float"),
            (0xFF800000, 4, "float"),
            (0x7FF8000000000000, 8, "double"),
            (0x7FF0000000000000, 8, "double"),
        ):
            with self.subTest(bits=hex(bits)):
                value = fl.classify(bits, size, kind)
                self.assertEqual(value.kind, fl.KIND_NONFINITE)
                self.assertFalse(value.replaceable)

    def test_unsupported_metadata(self) -> None:
        self.assertEqual(fl.classify(0x3F800000, 4, "string").kind, fl.KIND_UNSUPPORTED)
        self.assertEqual(fl.classify(0, 2, "float").kind, fl.KIND_UNSUPPORTED)


class LiteralSpellingTests(unittest.TestCase):
    def test_shortest_float_round_trips(self) -> None:
        cases = {
            0x3DCCCCCD: "0.1f",
            0x4048F5C3: "3.14f",
            0x3E8F5C29: "0.28f",
            0x80000000: "-0.0f",
            0x3F000000: "0.5f",
            0x42C80000: "100.0f",
        }
        for bits, spelling in cases.items():
            with self.subTest(bits=hex(bits)):
                got = fl.float_spelling(bits)
                self.assertEqual(got, spelling)
                self.assertEqual(f32_bits(float(got[:-1])), bits)

    def test_shortest_double_round_trips(self) -> None:
        for value in (0.5, 3.0, 3.6, 4503599627370496.0, 1e-7):
            with self.subTest(value=value):
                bits = f64_bits(value)
                got = fl.double_spelling(bits)
                self.assertEqual(f64_bits(float(got)), bits)
                self.assertFalse(got.endswith("f"))

    def test_integer_values_get_a_decimal_point(self) -> None:
        self.assertEqual(fl.float_spelling(f32_bits(10.0)), "10.0f")
        self.assertEqual(fl.double_spelling(f64_bits(10.0)), "10.0")


class WriteScopeTests(unittest.TestCase):
    def test_writable_paths_only_source_and_own_header(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            root = Path(td).resolve()
            src = root / "Unit.cpp"
            hdr = root / "Unit.hpp"
            other = root / "Other.cpp"
            for path in (src, hdr, other):
                path.write_text("")
            unit = SimpleNamespace(name="main/Unit", source_path=src, target_path=root / "Unit.o")
            with patch.object(fl, "ROOT", root), \
                 patch.object(fl, "header_index", return_value={"Unit": [hdr]}), \
                 patch.object(fl, "pick_header", return_value=hdr):
                paths = fl.writable_paths(unit)
            self.assertEqual(set(paths), {src, hdr})
            self.assertNotIn(other, paths)

    def test_foreign_path_is_rejected(self) -> None:
        src = Path("/repo/Unit.cpp")
        foreign = Path("/repo/CfCam.cpp")
        with self.assertRaisesRegex(ValueError, "outside the --unit scope"):
            fl.assert_writable([foreign], [src])

    def test_dry_run_never_calls_atomic_write(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            root = Path(td).resolve()
            src = root / "Unit.cpp"
            src.write_text("extern const f32 lbl_eu_11111111;\nvoid f() { (void)lbl_eu_11111111; }\n")
            unit = SimpleNamespace(name="main/Unit", source_path=src,
                                   target_path=root / "retail.o")
            value = fl.LabelValue(fl.KIND_FLOAT, f32_bits(0.5), 4, "0.5f", 0.5, ".sdata2")
            with patch.object(fl, "ROOT", root), \
                 patch.object(fl, "load_config", return_value=SimpleNamespace(region="us")), \
                 patch.object(fl, "Project", return_value=SimpleNamespace(resolve_unit=lambda _u: unit)), \
                 patch.object(fl, "header_index", return_value={}), \
                 patch.object(fl, "read_values", return_value={"lbl_eu_11111111": value}), \
                 patch.object(fl, "find_other_users", return_value=set()), \
                 patch.object(fl, "atomic_write_text", side_effect=AssertionError("dry-run wrote")):
                self.assertEqual(fl.main(["--unit", "main/Unit", "--dry-run"]), 0)


class DeclarationRemovalTests(unittest.TestCase):
    def test_only_sdata2_externs_with_no_remaining_refs_are_removed(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            src = Path(td) / "Unit.cpp"
            src.write_text(
                "extern char lbl_eu_80527154[];\n"
                "extern const f32 lbl_eu_DEADBEEF;\n"
                "extern const f32 lbl_eu_CAFEBABE;\n"
                "float f() { return lbl_eu_DEADBEEF + lbl_eu_CAFEBABE; }\n"
            )
            values = {
                "lbl_eu_80527154": fl.LabelValue(fl.KIND_UNSUPPORTED, 0, 0xC, section=".data"),
                "lbl_eu_DEADBEEF": fl.LabelValue(
                    fl.KIND_FLOAT, f32_bits(1.5), 4, "1.5f", 1.5, ".sdata2"),
                "lbl_eu_CAFEBABE": fl.LabelValue(
                    fl.KIND_SHARED, f32_bits(0.0), 4, section=".sdata2"),
            }
            scan = fl.scan_source([src], values, ["f"], set())
            decls = {e.label for e in scan.edits if e.kind == "decl"}
            self.assertNotIn("lbl_eu_80527154", decls)  # not .sdata2
            self.assertNotIn("lbl_eu_CAFEBABE", decls)  # reference survives
            self.assertIn("lbl_eu_DEADBEEF", decls)     # converted, zero refs left


class PoolDefinitionTests(unittest.TestCase):
    def test_converted_pool_definition_is_deleted(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            src = Path(td) / "Unit.cpp"
            src.write_text(
                "struct Sdata2_Unit {\n    float a;\n};\n"
                "__declspec(section \".sdata2\") const Sdata2_Unit sdata2_Unit = { 0.5f };\n"
                "#define lbl_eu_11111111 sdata2_Unit.a\n"
                "float f() { return lbl_eu_11111111; }\n"
            )
            values = {"lbl_eu_11111111": fl.LabelValue(
                fl.KIND_FLOAT, f32_bits(0.5), 4, "0.5f", 0.5, ".sdata2")}
            scan = fl.scan_source([src], values, ["f"], set())
            defs = [e for e in scan.edits if e.kind == "defn"]
            self.assertTrue(defs)
            for edit in scan.edits:
                self.assertIsNotNone(edit.start)
                self.assertIsNotNone(edit.end)
                self.assertIsNotNone(edit.old)
            after = fl.render(scan.texts, scan.edits)[src]
            self.assertNotIn("lbl_eu_11111111", after)
            self.assertNotIn("sdata2_Unit", after)
            self.assertNotIn("Sdata2_Unit", after)
            self.assertIn("return 0.5f;", after)


if __name__ == "__main__":
    unittest.main()

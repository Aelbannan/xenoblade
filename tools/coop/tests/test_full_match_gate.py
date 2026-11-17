"""FULL_MATCH must survive byte, size, relocation and identity checks."""
import json
import struct
import tempfile
import unittest
from dataclasses import replace
from pathlib import Path
from unittest import mock

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.full_match import compare_functions, pair_function, recover_retail_names
from tools.coop.lib.objdiff_report import (UnitReport, FunctionMatch, classify_status,
                                         evaluate_unit_match, meets_required_level)
from tools.coop.lib.equivalence_check import EquivalenceProbe
from tools.coop.lib.project import Project, ObjdiffUnit
from tools.coop.lib.reloc_canon import LinkerLayout
from tools.coop.lib.targets import update_target_result, sync_results_from_attempts
from tools.coop.lib.weak_dedup import _Elf
from tools.ppc_equivalence.elf_symbols import FunctionBytes, FunctionRelocation, extract_function
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.tests.test_elf_symbols import build_reloc_elf


def function(code="48000001 4e800020", relocs=()):
    code = bytes.fromhex(code)
    return FunctionBytes("f", Path("absent.o"), code, 0, 0, len(code), 1, ".text", 2,
                         tuple(FunctionRelocation(*r) for r in relocs))


def report():
    return UnitReport("u", 50., 100., 100., 1, 1, [FunctionMatch("f", None, 100., 8)])


class FullMatchGateTests(unittest.TestCase):
    def test_fuzzy_100_and_failed_certificate_cannot_promote(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            left, right = root / "retail.o", root / "decomp.o"
            left.write_bytes(build_reloc_elf({"f": bytes.fromhex("38630004 4e800020")}))
            right.write_bytes(build_reloc_elf({"f": bytes.fromhex("38630005 4e800020")}))
            config = CoopConfig(project_root=root, region="us")
            (root / "tools/coop").mkdir(parents=True)
            (root / "tools/coop/targets.json").write_text('{"targets": []}')
            unit = ObjdiffUnit("u", left, right, None)
            with mock.patch("tools.coop.lib.objdiff_report.report_unit", return_value=report()), mock.patch(
                    "tools.coop.lib.objdiff_report.certify_unit_symbol",
                    return_value=EquivalenceProbe(ProofStatus.INCONCLUSIVE_SMT_DISABLED, "different bytes")):
                ev = evaluate_unit_match(Project(config), unit, "f", target_id="id", run_smt=False)
            self.assertEqual(ev.status, "CODE_MATCH")
            for level in ("FULL_MATCH", "EQUIVALENT_MATCH"):
                self.assertFalse(meets_required_level(level, ev.status, function_match=100., unit=report(), symbol="f"))

    def test_fuzzy_or_old_full_certificate_is_not_byte_evidence(self):
        self.assertEqual(classify_status(100., report(), symbol="f"), "CODE_MATCH")
        self.assertEqual(classify_status(100., report(), symbol="f", equivalence=ProofStatus.EQUIVALENT,
                         certificate={"evidence": "full-instruction-match"},
                         policy=CoopConfig(project_root=Path("."), automatic_promotion=False)), "CODE_MATCH")
        self.assertEqual(classify_status(0., report(), symbol="f", full_match_verified=True), "FULL_MATCH")

    def test_register_swap_and_truncated_stub_fail(self):
        self.assertFalse(compare_functions(function("38630004 4e800020"), function("38840004 4e800020")).match)
        self.assertFalse(compare_functions(function(), function("4e800020")).match)

    def test_null_is_not_a_relocation_wildcard(self):
        a = function(relocs=((0, 10, "callee", 0),))
        b = function(relocs=((0, 10, "(null)", 0),))
        self.assertFalse(compare_functions(a, b).match)

    def test_wrong_callee_and_unproved_suffix_fail(self):
        a = function(relocs=((0, 10, "callee", 0),))
        for name in ("wrong", "callee__Fi"):
            self.assertFalse(compare_functions(a, function(relocs=((0, 10, name, 0),))).match)

    def test_resolved_aliases_and_addends_pass(self):
        layout = LinkerLayout("test", Path("/nonexistent"), symbols={"a": (".text", 0x80000100), "b": (".text", 0x800000fc)})
        a = function(relocs=((0, 10, "a", 0),))
        b = function(relocs=((0, 10, "b", 4),))
        self.assertTrue(compare_functions(a, b, linker_layout=layout).match)
        self.assertFalse(compare_functions(a, replace(b, relocations=(FunctionRelocation(0, 10, "b", 0),)), linker_layout=layout).match)

    def test_baked_ha_lo_requires_exact_resolved_address(self):
        layout = LinkerLayout("test", Path("/nonexistent"), symbols={"stack": (".bss", 0x8067f000)})
        retail = function("3c608068 3863f000")
        decomp = function("3c600000 38630000", ((2, 6, "stack", 0), (6, 4, "stack", 0)))
        self.assertTrue(compare_functions(retail, decomp, linker_layout=layout).match)
        self.assertFalse(compare_functions(retail, decomp).match)
        wrong = function("3c608067 3863f000")
        self.assertFalse(compare_functions(wrong, decomp, linker_layout=layout).match)

    def test_none_writes_nothing_but_unknown_types_fail(self):
        self.assertTrue(compare_functions(function(), function(relocs=((0, 0, "unknown", None),))).match)
        for kind in (99, 255):
            self.assertFalse(compare_functions(function(relocs=((0, kind, "x", 0),)), function(relocs=((0, kind, "x", 0),))).match)

    def test_implicit_addends_and_duplicate_sites_fail(self):
        a = function(relocs=((0, 10, "x", None),))
        self.assertFalse(compare_functions(a, a).match)
        a = function(relocs=((0, 10, "x", 0), (0, 10, "x", 0)))
        self.assertFalse(compare_functions(a, a).match)

    def test_stripped_symbol_resolves_by_elf_section_value(self):
        with tempfile.TemporaryDirectory() as tmp:
            left, right = Path(tmp) / "l.o", Path(tmp) / "r.o"
            left.write_bytes(build_reloc_elf({"f": function().code, "(null)": bytes.fromhex("4e800020")}, relocations=((0, "(null)", 10, 0),)))
            right.write_bytes(build_reloc_elf({"f": function().code, "target": bytes.fromhex("4e800020")}, relocations=((0, "target", 10, 0),)))
            a, b = extract_function(left, "f"), extract_function(right, "f")
            self.assertTrue(compare_functions(a, b, section_bases={".text": 0x80004000}).match)
            data = bytearray(right.read_bytes()); elf = _Elf(data)
            symtab = elf.sh[elf.symtab][4]
            struct.pack_into(">I", data, symtab + 2 * 16 + 4, 4)
            right.write_bytes(data)
            b = extract_function(right, "f")
            self.assertFalse(compare_functions(a, b, section_bases={".text": 0x80004000}).match)

    def test_weak_dedup_precedes_local_target_address_resolution(self):
        from tools.coop.tests.test_weak_dedup import build, GLOBAL, LOCAL, WEAK, FUNC, OBJECT
        from tools.coop.lib.full_match import compare_unit_functions
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp); (root / "config/us").mkdir(parents=True)
            (root / "build/us").mkdir(parents=True)
            (root / "config/us/splits.txt").write_text(
                "Sections:\n    .text start:0x80004000 end:0x80005000\n    .rodata start:0x80500000 end:0x80500100\n"
                "demo/Unit.cpp:\n    .text start:0x80004000 end:0x80004004\n    .rodata start:0x80500000 end:0x80500008\n"
                "demo/Other.cpp:\n    .text start:0x80004100 end:0x80004104\n")
            (root / "config/us/symbols.txt").write_text(
                "f = .text:0x80004000;\nweak = .text:0x80004100;\npool = .rodata:0x80500000;\n")
            (root / "build/us/ldscript.lcf").write_text("")
            def make(path, text, rodata, symbols, relocs):
                build(path, text=text, rodata=rodata, sdata=b"", symbols=symbols, sdata_relocs=relocs)
                # Reuse the generic fixture builder's relocation table for code.
                data = bytearray(path.read_bytes().replace(b".rela.sdata\0", b".rela.text\0\0"))
                elf = _Elf(data)
                ri = elf.names.index(".rela.text")
                struct.pack_into(">I", data, elf.shoff + ri * elf.shentsize + 28, elf.names.index(".text"))
                path.write_bytes(data)
            left, right = root / "retail.o", root / "decomp.o"
            make(left, b"\0" * 4, b"Unit000\0",
                 [("f", ".text", 0, 4, GLOBAL, FUNC), ("pool", ".rodata", 0, 8, GLOBAL, OBJECT)], [(0, "pool")])
            make(right, b"\0" * 8, b"Base000\0Unit000\0",
                 [("f", ".text", 0, 4, GLOBAL, FUNC), ("weak", ".text", 4, 4, WEAK, FUNC),
                  ("@base", ".rodata", 0, 8, LOCAL, OBJECT), ("@own", ".rodata", 8, 8, LOCAL, OBJECT)],
                 [(0, "@own"), (4, "@base")])
            a, b = extract_function(left, "f"), extract_function(right, "f")
            layout = LinkerLayout.load(root, "us")
            self.assertFalse(compare_functions(a, b, linker_layout=layout, section_bases={".rodata": 0x80500000}).match)
            project = Project(CoopConfig(project_root=root, region="us"))
            unit = ObjdiffUnit("demo/Unit", left, right, None)
            self.assertTrue(compare_unit_functions(project, unit, a, b).match)

    def test_name_pairing_refuses_offsets_and_ambiguous_substrings(self):
        a, b = replace(function(), name="FooA"), replace(function(), name="FooB")
        self.assertIsNone(pair_function([a, b], "Foo"))
        self.assertIsNone(pair_function([a], "Missing"))
        thunk = replace(function(), name="__dt__4BaseFv")
        self.assertEqual(pair_function([thunk], "@8@__dt__4BaseFv"), thunk)

    def test_retail_names_use_split_base(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp); (root / "config/us").mkdir(parents=True)
            (root / "config/us/splits.txt").write_text("demo/Unit.cpp:\n    .text start:0x80004000 end:0x80005000\n")
            unnamed = replace(function(), name="(null)", value=0x10)
            got = recover_retail_names([unnamed], root, "us", "demo/Unit", [{"symbol": "Correct", "address": "0x80004010"}])
            self.assertEqual(got[0].name, "Correct")

    def test_hexdiff_reads_init_only_object(self):
        from tools.coop.hexdiff import list_text_functions
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "boot.o"
            path.write_bytes(build_reloc_elf({"__start": bytes.fromhex("4e800020")}).replace(b".text", b".init"))
            self.assertEqual(list_text_functions(path)[0].name, "__start")

    def test_registry_rejects_full_without_current_byte_evidence_and_history_cannot_restore_it(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp); (root / "tools/coop").mkdir(parents=True)
            path = root / "tools/coop/targets.json"
            original = {"targets": [{"id": "f", "unit": "u", "symbol": "f", "status": "CODE_MATCH"}]}
            path.write_text(json.dumps(original))
            config = CoopConfig(project_root=root)
            with mock.patch("tools.coop.lib.targets._verify_full_row", side_effect=ValueError("wrong target")):
                with self.assertRaises(ValueError):
                    update_target_result(config, "f", status="FULL_MATCH", instruction_match=100.)
                self.assertEqual(sync_results_from_attempts(config, [{"target_id": "f", "status": "FULL_MATCH", "instruction_match": 100.}]), 0)
            self.assertEqual(json.loads(path.read_text()), original)

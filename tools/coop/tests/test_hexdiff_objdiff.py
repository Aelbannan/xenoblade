"""hexdiff ↔ objdiff joined verdicts, baseline regressions and JSON keys.

Uses small synthetic objdiff report.json files (no build, no repo objects).
"""

from __future__ import annotations

import contextlib
import io
import json
import pathlib
import sys
import tempfile
import unittest
from types import SimpleNamespace
from unittest import mock

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))

from tools.coop import hexdiff  # noqa: E402
from tools.coop.lib.objdiff_hexdiff import (  # noqa: E402
    BOTH_FAIL_MARK, FULL_MARK, LINKED_ADDRESS_MISMATCH_MARK, RELOC_NAME_DRIFT_MARK,
    UNPAIRED_MARK, BaselineChange, ReportFunction, compare_baseline, combine_verdict,
    load_report_unit, load_retail_addresses, pair_report_function, parse_report, parse_units,
)
from tools.coop import objdiff_unpaired  # noqa: E402
from tools.coop.objdiff_unpaired import collect_unpaired  # noqa: E402
from tools.ppc_equivalence.elf_symbols import FunctionBytes  # noqa: E402


def _report(rows: list[tuple[str, float | None, int | None]], unit: str = "u") -> dict:
    functions = []
    for name, percent, address in rows:
        fn = {"name": name, "size": "8"}
        if percent is not None:
            fn["fuzzy_match_percent"] = percent
        if address is not None:
            fn["metadata"] = {"virtual_address": str(address)}
        functions.append(fn)
    return {"units": [{"name": unit, "functions": functions}]}


def _write_report(directory: pathlib.Path, name: str, rows: list[tuple]) -> pathlib.Path:
    path = directory / name
    path.write_text(json.dumps(_report(rows)), encoding="utf-8")
    return path


def _fn(name: str, code: bytes = b"\x60\x00\x00\x00" * 2) -> FunctionBytes:
    return FunctionBytes(
        name=name, path=pathlib.Path("x.o"), code=code, base=0, value=0,
        size=len(code), section_index=0, section_name=".text", symbol_type=0,
    )


class TestCombinedVerdict(unittest.TestCase):
    def test_all_four_combinations(self):
        cases = [
            (True, 100.0, FULL_MARK, True),
            (True, 99.79, RELOC_NAME_DRIFT_MARK, False),
            (False, 100.0, LINKED_ADDRESS_MISMATCH_MARK, False),
            (False, 88.0, BOTH_FAIL_MARK, False),
        ]
        for proof, percent, mark, full in cases:
            with self.subTest(proof=proof, percent=percent):
                verdict = combine_verdict(proof, percent)
                self.assertEqual(verdict.mark, mark)
                self.assertEqual(verdict.full_match, full)
                self.assertEqual(verdict.bytes_match, proof)

    def test_missing_percent_without_unpaired_flag_is_not_full(self):
        verdict = combine_verdict(True, None)
        self.assertEqual(verdict.mark, RELOC_NAME_DRIFT_MARK)
        self.assertFalse(verdict.full_match)

    def test_objdiff_unpaired_is_its_own_verdict(self):
        entry = parse_report(_report([("MCCrystal_LoadLayouts", None, 0x100)]), "u")[0]
        self.assertTrue(entry.unpaired)
        verdict = combine_verdict(True, entry.percent, unpaired=entry.unpaired)
        self.assertEqual(verdict.mark, UNPAIRED_MARK)
        self.assertEqual(verdict.reason, "objdiff unpaired (name/linkage mismatch)")
        self.assertFalse(verdict.full_match)
        fields = verdict.json_fields()
        self.assertEqual(fields["verdict"], "objdiff_unpaired")
        self.assertEqual(fields["mark"], "?")
        self.assertTrue(fields["unpaired"])
        self.assertTrue(fields["bytes_match"])
        summary = hexdiff._summary_line(_fn("MCCrystal_LoadLayouts"), _fn("v"),
                                        {"total": 2, "mismatch": 0, "structural": 0,
                                         "reg_swap": 0, "reloc": 0},
                                        objdiff_percent=None, objdiff_unpaired=True)
        self.assertIn("objdiff=unpaired", summary)

    def test_json_fields_keep_old_proof_and_new_alias(self):
        fields = combine_verdict(True, 99.5).json_fields()
        self.assertTrue(fields["bytes_match"])
        self.assertFalse(fields["full_match"])
        self.assertEqual(fields["match"], fields["full_match"])
        self.assertEqual(fields["objdiff_percent"], 99.5)
        self.assertEqual(fields["verdict"], "reloc_name_drift")


class TestSyntheticReport(unittest.TestCase):
    def test_parse_and_pair_by_decomp_or_retail_name(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = _write_report(pathlib.Path(tmp), "r.json", [
                ("MenuSnd_CanPlayGate_9450", 99.791664, 0x8018AA04),
                ("Unmapped", None, 0x8018AB00),
            ])
            fns = load_report_unit(path, "u")
        self.assertIsNotNone(fns)
        self.assertEqual(len(fns), 2)
        self.assertIsNone(fns[1].percent)
        self.assertEqual(fns[0].address, 0x8018AA04)
        self.assertFalse(fns[0].unpaired)
        self.assertTrue(fns[1].unpaired)
        self.assertIs(pair_report_function(fns, "MenuSnd_CanPlayGate_9450"), fns[0])

    def test_parse_missing_unit_and_mangled_fallback(self):
        self.assertIsNone(parse_report(_report([]), "other-unit"))
        fns = parse_report(_report([("__ct__7CFooFv", 100.0, None)]), "u")
        self.assertEqual(pair_report_function(fns, "__ct__CFoo").name, "__ct__7CFooFv")


class TestBaseline(unittest.TestCase):
    def test_lost_and_gain(self):
        with tempfile.TemporaryDirectory() as tmp:
            old = _write_report(pathlib.Path(tmp), "old.json", [
                ("A", 100.0, 0x10), ("B", 90.0, 0x20), ("C", 100.0, 0x30),
            ])
            new = _write_report(pathlib.Path(tmp), "new.json", [
                ("A", 99.5, 0x10), ("B", 100.0, 0x20), ("C", 100.0, 0x30),
            ])
            changes = compare_baseline(load_report_unit(old, "u"), load_report_unit(new, "u"))
        self.assertEqual([(c.kind, c.symbol) for c in changes], [("LOST", "A"), ("GAIN", "B")])
        self.assertIn("100.00% -> 99.50%", changes[0].render())

    def test_rename_paired_by_report_address(self):
        old = parse_report(_report([("OldSym", 100.0, 0x80123400)]), "u")
        new = parse_report(_report([("NewSym", 99.0, 0x80123400)]), "u")
        changes = compare_baseline(old, new)
        self.assertEqual(len(changes), 1)
        self.assertEqual(changes[0].kind, "LOST")
        self.assertEqual(changes[0].symbol, "OldSym")
        self.assertEqual(changes[0].paired_symbol, "NewSym")
        self.assertIn("paired: NewSym", changes[0].render())

    def test_rename_paired_by_symbol_resolver(self):
        old = parse_report(_report([("__ct__CFoo", 100.0, None)]), "u")
        new = parse_report(_report([("__ct__7CFooFv", 91.0, None)]), "u")
        changes = compare_baseline(old, new)
        self.assertEqual([(c.kind, c.symbol) for c in changes], [("LOST", "__ct__CFoo")])

    def test_rename_paired_via_targets_and_symbols_txt(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            (root / "config" / "us").mkdir(parents=True)
            (root / "tools" / "coop").mkdir(parents=True)
            (root / "config" / "us" / "symbols.txt").write_text(
                "OldSym = .text:0x80001000; // type:function\n", encoding="utf-8")
            (root / "tools" / "coop" / "targets.json").write_text(
                json.dumps({"targets": [{"symbol": "OldSym", "address": "0x80001000"}]}),
                encoding="utf-8")
            addresses = load_retail_addresses(root, "us")
            self.assertEqual(addresses["OldSym"], 0x80001000)
            old = parse_report(_report([("OldSym", 100.0, None)]), "u")
            new = parse_report(_report([("NewSym", 98.0, 0x80001000)]), "u")
            changes = compare_baseline(old, new, addresses=addresses)
        self.assertEqual([(c.kind, c.symbol) for c in changes], [("LOST", "OldSym")])

    def test_both_perfect_after_rename_is_not_a_change(self):
        old = parse_report(_report([("OldSym", 100.0, 0x100)]), "u")
        new = parse_report(_report([("NewSym", 100.0, 0x100)]), "u")
        self.assertEqual(compare_baseline(old, new), [])


class TestAllJsonKeys(unittest.TestCase):
    def test_all_json_bytes_match_full_match_and_baseline_exit(self):
        fn = _fn("Foo")
        args = SimpleNamespace(
            json=True, pair_alias=[], objdiff_unit=parse_report(
                _report([("Foo", 99.79, 0x100)]), "u"),
            baseline_path=pathlib.Path("/tmp/baseline.json"),
            baseline_changes=[BaselineChange("LOST", "Foo", 100.0, 99.79)],
        )
        comparison = SimpleNamespace(match=True, retail=fn, decomp=fn,
                                     unresolved=[], reasons=[])
        project = SimpleNamespace(root=ROOT)
        unit = SimpleNamespace(name="u")
        config = SimpleNamespace(region="us")
        with mock.patch.object(hexdiff, "list_text_functions", side_effect=[[fn], [fn]]), \
             mock.patch.object(hexdiff, "_recover_retail_names"), \
             mock.patch.object(hexdiff, "check_object_size", return_value=None), \
             mock.patch.object(hexdiff, "_compare", return_value=comparison), \
             mock.patch.object(hexdiff, "_extab_check", return_value={"ok": None}):
            out = io.StringIO()
            with contextlib.redirect_stdout(out), contextlib.redirect_stderr(io.StringIO()):
                rc = hexdiff._cmd_all(args, project, unit,
                                      pathlib.Path("r.o"), pathlib.Path("d.o"), config)
        doc = json.loads(out.getvalue())
        row = doc["functions"][0]
        self.assertEqual(rc, 6)
        self.assertTrue(row["bytes_match"])
        self.assertFalse(row["full_match"])
        self.assertEqual(row["match"], row["full_match"])
        self.assertEqual(row["objdiff_percent"], 99.79)
        self.assertEqual(doc["matched"], 0)
        self.assertEqual(doc["bytes_matched"], 1)
        self.assertEqual(doc["baseline"]["lost"], 1)
        self.assertEqual(row["verdict"], "reloc_name_drift")

    def test_all_json_plain_byte_match_keeps_full_match_false(self):
        fn = _fn("Foo")
        args = SimpleNamespace(json=True, pair_alias=[], objdiff_unit=[],
                               baseline_path=None, baseline_changes=[])
        comparison = SimpleNamespace(match=True, retail=fn, decomp=fn,
                                     unresolved=[], reasons=[])
        with mock.patch.object(hexdiff, "list_text_functions", side_effect=[[fn], [fn]]), \
             mock.patch.object(hexdiff, "_recover_retail_names"), \
             mock.patch.object(hexdiff, "check_object_size", return_value=None), \
             mock.patch.object(hexdiff, "_compare", return_value=comparison), \
             mock.patch.object(hexdiff, "_extab_check", return_value={"ok": None}):
            out = io.StringIO()
            with contextlib.redirect_stdout(out):
                rc = hexdiff._cmd_all(args, SimpleNamespace(root=ROOT),
                                      SimpleNamespace(name="u"),
                                      pathlib.Path("r.o"), pathlib.Path("d.o"),
                                      SimpleNamespace(region="us"))
        row = json.loads(out.getvalue())["functions"][0]
        self.assertEqual(rc, 5)  # no objdiff proof → not ✓
        self.assertTrue(row["bytes_match"])
        self.assertFalse(row["full_match"])


class TestObjdiffUnpairedReport(unittest.TestCase):
    def test_lists_only_unique_resolver_hits(self):
        report = _report([
            ("MCCrystal_LoadLayouts", None, 0x80300000),
            ("MCCrystal_Paired", 100.0, 0x80300100),
            ("NoDecompSymbol", None, 0x80300200),
        ], unit="u")
        with tempfile.TemporaryDirectory() as tmp:
            obj = pathlib.Path(tmp) / "u.o"
            obj.write_bytes(b"")
            decomp = [
                _fn("MCCrystal_LoadLayouts__FP15CWorkThread"),
                _fn("MCCrystal_Paired"),
            ]
            found = collect_unpaired(
                report,
                resolve_unit=lambda _name: SimpleNamespace(base_path=obj),
                list_functions=lambda _path: decomp,
            )
        self.assertEqual(list(found), ["u"])
        self.assertEqual(len(found["u"]), 1)
        self.assertEqual(found["u"][0]["symbol"], "MCCrystal_LoadLayouts")
        self.assertEqual(found["u"][0]["decomp_symbol"],
                         "MCCrystal_LoadLayouts__FP15CWorkThread")


def _complete_report(rows: list[tuple], unit: str = "u") -> dict:
    report = _report(rows, unit=unit)
    report["units"][0]["metadata"] = {"complete": True}
    return report


class TestPhantomPairing(unittest.TestCase):
    """objdiff names report rows after retail symbols; exact decomp name pairs."""

    def test_complete_unit_phantom_is_unpaired_even_at_100(self):
        report = _complete_report([("Checker", 100.0, 0x100)])
        fn = parse_report(report, "u",
                          decomp_symbols={"u": {"Checker__Q22cf9CfGimmickFv"}})[0]
        self.assertTrue(fn.complete)
        self.assertTrue(fn.unpaired)
        self.assertTrue(fn.phantom)
        self.assertEqual(fn.percent, 100.0)
        verdict = combine_verdict(True, fn.percent, unpaired=fn.unpaired)
        self.assertEqual(verdict.mark, UNPAIRED_MARK)
        self.assertFalse(verdict.full_match)

    def test_exact_decomp_symbol_is_paired(self):
        report = _complete_report([("Checker", 100.0, 0x100)])
        fn = parse_report(report, "u", decomp_symbols={"u": {"Checker"}})[0]
        self.assertFalse(fn.unpaired)
        self.assertFalse(fn.phantom)
        self.assertEqual(combine_verdict(True, fn.percent, unpaired=fn.unpaired).mark,
                         FULL_MARK)

    def test_noncomplete_unit_100_is_not_a_phantom_flag(self):
        fn = parse_report(_report([("Checker", 100.0, 0x100)]), "u",
                          decomp_symbols={"u": {"Other"}})[0]
        self.assertFalse(fn.complete)
        self.assertTrue(fn.unpaired)  # no exact decomp symbol, regardless of score
        self.assertFalse(fn.phantom)

    def test_missing_fuzzy_is_unpaired_even_if_decomp_symbol_matches(self):
        fn = parse_report(_complete_report([("Sym", None, 0x100)]), "u",
                          decomp_symbols={"u": {"Sym"}})[0]
        self.assertTrue(fn.unpaired)
        self.assertFalse(fn.phantom)

    def test_original_cfgimmick_checker_case(self):
        """The proven phantom case: mangled decomp names, 100 in a complete unit."""
        unit = "main/kyoshin/cf/CfGimmick"
        report = _complete_report([
            ("CfGimmick_CheckerCylinderLocal", 100.0, 0x100),
            ("CfGimmick_CheckerBoxLocal", 100.0, 0x200),
        ], unit=unit)
        fns = parse_report(report, unit, decomp_symbols={unit: {
            "CfGimmick_CheckerCylinderLocal__Q22cf9CfGimmickFv",
            "CfGimmick_CheckerBoxLocal__Q22cf9CfGimmickFv",
        }})
        for fn in fns:
            self.assertTrue(fn.phantom)
            self.assertEqual(combine_verdict(True, fn.percent,
                                             unpaired=fn.unpaired).mark, UNPAIRED_MARK)

    def test_unknown_decomp_symbols_keep_the_old_fuzzy_key_rule(self):
        fns = parse_units(_complete_report([("Sym", 100.0, 0x100)]))
        self.assertFalse(fns["u"][0].unpaired)
        self.assertFalse(fns["u"][0].phantom)


class TestBaselinePhantom(unittest.TestCase):
    def test_complete_baseline_100_plus_current_unpaired_is_phantom(self):
        old = parse_report(_complete_report([("Checker", 100.0, 0x100)]), "u")
        new = parse_report(_complete_report([("Checker", 100.0, 0x100)]), "u",
                           decomp_symbols={"u": {"Checker__Q22cf9CfGimmickFv"}})
        changes = compare_baseline(old, new, decomp_symbols={"Checker__Q22cf9CfGimmickFv"})
        self.assertEqual([(c.kind, c.symbol) for c in changes], [("PHANTOM", "Checker")])
        self.assertIn("PHANTOM Checker", changes[0].render())
        self.assertEqual(hexdiff._finish_baseline(
            SimpleNamespace(baseline_changes=changes, baseline_path=None, json=True)), 0)

    def test_renamed_current_symbol_is_also_phantom(self):
        old = parse_report(_complete_report([("OldName", 100.0, 0x100)]), "u")
        new = parse_report(_complete_report([("NewName", 100.0, 0x100)]), "u",
                           decomp_symbols={"u": {"NewName__mangled"}})
        changes = compare_baseline(old, new, decomp_symbols={"NewName__mangled"})
        self.assertEqual([(c.kind, c.symbol) for c in changes], [("PHANTOM", "OldName")])
        self.assertEqual(changes[0].paired_symbol, "NewName")

    def test_real_regression_is_still_lost(self):
        old = parse_report(_complete_report([("Checker", 100.0, 0x100)]), "u")
        new = parse_report(_complete_report([("Checker", 54.8, 0x100)]), "u",
                           decomp_symbols={"u": {"Checker"}})
        changes = compare_baseline(old, new, decomp_symbols={"Checker"})
        self.assertEqual([(c.kind, c.symbol) for c in changes], [("LOST", "Checker")])
        self.assertEqual(hexdiff._finish_baseline(
            SimpleNamespace(baseline_changes=changes, baseline_path=None, json=True)), 6)

    def test_incomplete_baseline_is_not_a_phantom(self):
        old = parse_report(_report([("Checker", 100.0, 0x100)]), "u")
        new = parse_report(_report([("Checker", 100.0, 0x100)]), "u",
                           decomp_symbols={"u": {"Other"}})
        self.assertEqual([c.kind for c in compare_baseline(
            old, new, decomp_symbols={"Other"})], ["LOST"])

    def test_without_decomp_symbols_the_conservative_lost_is_kept(self):
        old = parse_report(_complete_report([("Checker", 100.0, 0x100)]), "u")
        new = parse_report(_report([("Checker", None, 0x100)]), "u")
        self.assertEqual([c.kind for c in compare_baseline(old, new)], ["LOST"])

    def test_unpaired_current_100_is_not_a_gain(self):
        old = parse_report(_complete_report([("Checker", 50.0, 0x100)]), "u")
        new = parse_report(_complete_report([("Checker", 100.0, 0x100)]), "u",
                           decomp_symbols={"u": {"Other"}})
        self.assertEqual(compare_baseline(old, new, decomp_symbols={"Other"}), [])


class TestPhantomInventory(unittest.TestCase):
    def test_collect_unpaired_flags_phantoms_and_no_score_rows(self):
        report = _complete_report([
            ("Checker", 100.0, 0x100),  # exact decomp name missing -> phantom
            ("MissingScore", None, 0x200),
            ("Paired", 100.0, 0x300),
        ])
        with tempfile.TemporaryDirectory() as tmp:
            obj = pathlib.Path(tmp) / "u.o"
            obj.write_bytes(b"")
            decomp = [
                _fn("Checker__Q22cf9CfGimmickFv"),
                _fn("MissingScore__FPv"),
                _fn("Paired"),
            ]
            found = collect_unpaired(
                report,
                resolve_unit=lambda _name: SimpleNamespace(base_path=obj),
                list_functions=lambda _path: decomp,
            )
        self.assertEqual(list(found), ["u"])
        rows = {row["symbol"]: row for row in found["u"]}
        self.assertEqual(set(rows), {"Checker", "MissingScore"})
        self.assertTrue(rows["Checker"]["phantom"])
        self.assertFalse(rows["MissingScore"]["phantom"])
        self.assertEqual(rows["Checker"]["fuzzy_match_percent"], 100.0)

    def test_main_prints_phantom_and_no_score_totals(self):
        report = _complete_report([
            ("Checker", 100.0, 0x100),
            ("MissingScore", None, 0x200),
        ])
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            obj = root / "u.o"
            obj.write_bytes(b"")
            path = root / "report.json"
            path.write_text(json.dumps(report), encoding="utf-8")
            decomp = [_fn("Checker__mangled"), _fn("MissingScore__FPv")]
            with mock.patch.object(objdiff_unpaired, "load_config",
                                   return_value=SimpleNamespace(region="us")),                  mock.patch.object(objdiff_unpaired, "Project") as project,                  mock.patch.object(objdiff_unpaired, "list_text_functions",
                                   return_value=decomp):
                project.return_value.resolve_unit.side_effect = (
                    lambda _name: SimpleNamespace(base_path=obj))
                out = io.StringIO()
                with contextlib.redirect_stdout(out):
                    rc = objdiff_unpaired.main([str(path)])
        self.assertEqual(rc, 0)
        self.assertIn("1 phantom-100, 1 no-score", out.getvalue())
        self.assertIn("[phantom 100]", out.getvalue())

    def test_all_table_marks_a_complete_unit_phantom_as_unpaired(self):
        fn = _fn("Checker")
        phantom = ReportFunction("Checker", 100.0, complete=True,
                                 unpaired=True, phantom=True)
        args = SimpleNamespace(
            json=False, pair_alias=[], objdiff_unit=[phantom],
            baseline_path=None, baseline_changes=[],
        )
        comparison = SimpleNamespace(match=True, retail=fn, decomp=fn,
                                     unresolved=[], reasons=[])
        out = io.StringIO()
        with mock.patch.object(hexdiff, "list_text_functions", side_effect=[[fn], [fn]]),              mock.patch.object(hexdiff, "_recover_retail_names"),              mock.patch.object(hexdiff, "check_object_size", return_value=None),              mock.patch.object(hexdiff, "_compare", return_value=comparison),              mock.patch.object(hexdiff, "_extab_check", return_value={"ok": None}):
            with contextlib.redirect_stdout(out):
                rc = hexdiff._cmd_all(args, SimpleNamespace(root=ROOT),
                                      SimpleNamespace(name="u"),
                                      pathlib.Path("r.o"), pathlib.Path("d.o"),
                                      SimpleNamespace(region="us"))
        line = next(line for line in out.getvalue().splitlines() if "Checker" in line)
        self.assertIn("?", line)
        self.assertIn("unpaired", line)
        self.assertNotIn("100.00%", line)
        self.assertEqual(rc, 5)  # unpaired is never a full match



if __name__ == "__main__":
    unittest.main()

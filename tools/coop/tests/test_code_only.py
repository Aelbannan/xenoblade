"""Code-only unit kind (PLAN.md §17.7): label parsing, project plumbing,
README rows and the declarations-only checker's pure logic.

Fixture-only; no build is run here.
"""

from __future__ import annotations

import unittest
from pathlib import Path

from tools import project
from tools.coop import code_only_check as coc
from tools.coop import readme_status
from tools.coop.lib.code_only import code_only_units, object_stem, unit_kind_counts
from tools.coop.lib.data_match import ParsedObject

CONFIGURE = '''
            Object(Matching, "kyoshin/A.cpp"),
            Object(MatchingCodeOnly, "kyoshin/B.cpp", extra_cflags=["-x"]),
            # Object(MatchingCodeOnly, "kyoshin/Commented.cpp"),
            Object(NonMatching, "kyoshin/C.cpp"),
            Object( MatchingCodeOnly ,'monolib/src/math/D.cpp'),
'''


def _obj(sections: dict[str, int], symbols: dict[str, str] | None = None) -> ParsedObject:
    p = ParsedObject()
    for i, (name, size) in enumerate(sections.items(), start=1):
        p.section_names[i] = name
        p.sections[name] = {"off": 0, "size": size, "addr": 0, "align": 4}
    index = {n: i for i, n in p.section_names.items()}
    for sym, sec in (symbols or {}).items():
        p.symbols[sym] = (index[sec], 0)
    return p


class ParseTests(unittest.TestCase):
    def test_code_only_units_ignores_comments(self) -> None:
        self.assertEqual(
            code_only_units(CONFIGURE), ["kyoshin/B.cpp", "monolib/src/math/D.cpp"]
        )

    def test_unit_kind_counts(self) -> None:
        self.assertEqual(
            unit_kind_counts(CONFIGURE),
            {"Matching": 1, "MatchingCodeOnly": 2, "NonMatching": 1},
        )

    def test_object_stem(self) -> None:
        self.assertEqual(object_stem("monolib/src/math/D.cpp"), "monolib/src/math/D")


class ProjectKindTests(unittest.TestCase):
    def test_code_only_links_like_matching(self) -> None:
        obj = project.Object(project.MatchingCodeOnly, "kyoshin/B.cpp")
        self.assertIs(obj.completed, True)
        self.assertTrue(obj.code_only)

    def test_plain_kinds_are_not_code_only(self) -> None:
        self.assertFalse(project.Object(True, "a.cpp").code_only)
        self.assertFalse(project.Object(False, "a.cpp").code_only)
        self.assertIs(project.Object(False, "a.cpp").completed, False)

    def test_resolve_keeps_code_only(self) -> None:
        cfg = project.ProjectConfig()
        cfg.progress_categories = []
        obj = project.Object(project.MatchingCodeOnly, "kyoshin/B.cpp")
        self.assertTrue(obj.resolve(cfg, {"lib": "kyoshin"}).code_only)


class ReadmeRowTests(unittest.TestCase):
    def test_rows_list_units(self) -> None:
        (row,) = readme_status.code_only_rows(CONFIGURE)
        self.assertIn("| 2 (`B`, `D`) |", row)
        self.assertIn("not counted as data", row)

    def test_rows_none(self) -> None:
        (row,) = readme_status.code_only_rows('Object(Matching, "a.cpp")')
        self.assertIn("| 0 (none) |", row)


class DeclarationsOnlyTests(unittest.TestCase):
    def test_extern_only_is_clean(self) -> None:
        retail = _obj({".sdata2": 0, ".data": 0x10})
        decomp = _obj({".data": 0x10}, {"vt": ".data"})
        self.assertEqual(coc.foreign_definitions(retail, decomp), ([], []))

    def test_foreign_definition_is_violation(self) -> None:
        retail = _obj({".data": 0})
        decomp = _obj({".sdata2": 8, ".data": 0}, {"@123": ".sdata2"})
        violations, transitional = coc.foreign_definitions(retail, decomp)
        self.assertEqual(transitional, [])
        self.assertEqual(len(violations), 1)
        self.assertIn(".sdata2", violations[0])
        self.assertIn("@123", violations[0])

    def test_eds_covered_definition_is_transitional(self) -> None:
        retail = _obj({})
        decomp = _obj({".sdata2": 8}, {"@1": ".sdata2"})
        violations, transitional = coc.foreign_definitions(retail, decomp, (".sdata2",))
        self.assertEqual(violations, [])
        self.assertEqual(len(transitional), 1)

    def test_owned_section_is_not_foreign(self) -> None:
        # Retail owns .rodata: a size/byte mismatch there is the data gate's
        # business, not a declarations-only violation.
        retail = _obj({".rodata": 0x20})
        decomp = _obj({".rodata": 0x28}, {"s": ".rodata"})
        self.assertEqual(coc.foreign_definitions(retail, decomp), ([], []))


class ProgressTests(unittest.TestCase):
    def test_headline_data_excludes_code_only(self) -> None:
        import io
        import json
        import tempfile
        from contextlib import redirect_stdout

        with tempfile.TemporaryDirectory() as tmp:
            cfg = project.ProjectConfig()
            cfg.build_dir = Path(tmp)
            cfg.version = "us"
            for attr in ("src_dir", "tools_dir", "check_sha_path", "config_path"):
                setattr(cfg, attr, Path(tmp))
            cfg.ldflags, cfg.linker_version, cfg.libs = [], "x", []
            (Path(tmp) / "us").mkdir()
            measures = {"total_code": "100", "complete_code": "50", "total_data": "200",
                        "complete_data": "80", "complete_units": 3, "total_units": 9}
            co = {"complete_units": 1, "complete_code": "10", "complete_data": "30"}
            (Path(tmp) / "us" / "report.json").write_text(json.dumps({
                "measures": measures,
                "categories": [{"id": "code_only", "name": "c", "measures": co}],
            }))
            out = io.StringIO()
            with redirect_stdout(out):
                project.calculate_progress(cfg)
        text = out.getvalue()
        self.assertIn("Data linked from owning TUs: 50 / 200 bytes", text)
        self.assertIn("Code-only units", text)
        self.assertIn("30 data bytes excluded", text)


if __name__ == "__main__":
    unittest.main()

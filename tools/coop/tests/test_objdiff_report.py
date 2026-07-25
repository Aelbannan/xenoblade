from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.objdiff_report import _parse_unit_report, find_function_match


class ParseUnitReportTests(unittest.TestCase):
    def _write_report(self, functions: list[dict]) -> Path:
        tmp = Path(tempfile.mkdtemp())
        path = tmp / "report.json"
        path.write_text(
            json.dumps(
                {
                    "units": [
                        {
                            "name": "main/unit",
                            "measures": {
                                "matched_code": "8",
                                "total_code": "16",
                                "matched_data": "0",
                                "total_data": "4",
                                "fuzzy_match_percent": 50.0,
                                "total_functions": len(functions),
                                "matched_functions": 1,
                            },
                            "functions": functions,
                        }
                    ]
                }
            ),
            encoding="utf-8",
        )
        return path

    def test_mapped_flag_present_when_fuzzy_scored(self) -> None:
        path = self._write_report(
            [
                {
                    "name": "foo__5CBarFv",
                    "size": "8",
                    "fuzzy_match_percent": 100.0,
                    "metadata": {"demangled_name": "CBar::foo()"},
                }
            ]
        )
        report = _parse_unit_report(path, "main/unit")
        fn = find_function_match(report, "foo__5CBarFv")
        self.assertIsNotNone(fn)
        self.assertTrue(fn.mapped)
        self.assertEqual(fn.match_percent, 100.0)

    def test_unmapped_when_fuzzy_match_percent_missing(self) -> None:
        # objdiff omits fuzzy_match_percent when it cannot pair the symbol
        # between retail and candidate objects (name/linkage mismatch). The
        # parser must surface that as unmapped instead of a believable 0.0.
        path = self._write_report(
            [
                {
                    "name": "foo__5CBarFv",
                    "size": "8",
                    "metadata": {"demangled_name": "CBar::foo()"},
                }
            ]
        )
        report = _parse_unit_report(path, "main/unit")
        fn = find_function_match(report, "foo__5CBarFv")
        self.assertIsNotNone(fn)
        self.assertFalse(fn.mapped)
        self.assertEqual(fn.match_percent, 0.0)


if __name__ == "__main__":
    unittest.main()

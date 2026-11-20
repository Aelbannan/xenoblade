"""Hard ``packed_string`` detector: NUL padding written into string literals."""
import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[3]))

from tools.coop import faithful as f  # noqa: E402


class PackedStrings(unittest.TestCase):
    def test_every_padding_spelling(self):
        src = '''
        const char* a = "thread\\0";
        const char* b = "x\\0\\0\\0";
        const char* c = "name\\0value";
        '''
        hits = f.detect_packed_strings(src)
        self.assertEqual([h.line for h in hits], [2, 3, 4])
        self.assertEqual(f.count_packed_strings(src), 3)
        hard, _ = f.count_smells(src)
        self.assertEqual(hard["packed_string"], 3)

    def test_adjacent_literals_are_one_expression(self):
        src = '''
        const char* names =
            "BTL_pclist\\0"
            "BTL_enelist\\0"
            "BTL_skilllist\\0";
        #define TBL "a\\0" \\
                    "b\\0"
        '''
        hits = f.detect_packed_strings(src)
        self.assertEqual([h.line for h in hits], [3, 6])
        self.assertEqual(f.count_packed_strings(src), 2)

    def test_several_expressions_on_one_line(self):
        src = 'report("first\\0", "second\\0", "plain");\n'
        self.assertEqual(f.count_packed_strings(src), 2)

    def test_non_string_or_nul_free_literals_are_ignored(self):
        src = r'''
        char c = '\0';                 // char literal
        const char* backslash = "\\0"; // backslash + '0'
        const char* octal = "\012";    // newline, not NUL
        const char* plain = "no nul";
        // const char* commented = "hidden\0";
        /* const char* commented2 = "hidden\0"; */
        const char* url = "http://host/path";
        '''
        self.assertEqual(f.count_packed_strings(src), 0)

    def test_comment_markers_inside_strings_are_not_comments(self):
        src = 'const char* s = "http://host\\0";\n'
        self.assertEqual(f.count_packed_strings(src), 1)

    def test_wide_strings_count(self):
        self.assertEqual(f.count_packed_strings('const wchar_t* s = L"wide\\0";\n'), 1)

    def test_gate_and_finish_line_wiring(self):
        src = '''
        // faithful-ok: should not hide padding-by-literal
        const char* s = "thread\\0";
        '''
        hard, soft = f.count_smells(src)
        self.assertEqual(hard["packed_string"], 1)
        self.assertNotIn("packed_string", soft)
        self.assertEqual(sum(hard.values()), 1)  # scored by the gate
        totals, exceptions, _, _ = f.count_finish_line_file(src, "src/T.cpp")
        self.assertEqual(totals["hard/packed_string"], 1)
        self.assertEqual(len(exceptions), 1)
        self.assertIn(("packed_string", "hard", "hard/packed_string"),
                      f.FINISH_CATEGORIES)


class PackedAttribution(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        f.real_classes.cache_clear()
        self.addCleanup(f.real_classes.cache_clear)
        self.root = Path(self.tmp.name)
        files = {
            "objdiff.json": json.dumps({"units": [{
                "name": "A", "base_path": "A",
                "metadata": {"source_path": "src/A.cpp", "complete": True},
            }]}),
            "src/A.cpp": 'const char* s = "from_source\\0";\n',
            "src/A.hpp": 'const char* h = "from_header\\0";\n',
            "src/Side.hpp": 'const char* h = "from_side\\0";\n',
            "src/Side.ctx.c": 'const char* h = "generated\\0";\n',
        }
        for rel, text in files.items():
            path = self.root / rel
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text)

    def test_source_header_and_pseudo_unit_counts(self):
        from tools.coop.lib import weak_dedup as W
        import tools.postprocess_reloc_names as P
        with mock.patch.object(f, "ROOT", self.root), \
             mock.patch.object(f, "foreign_data_counts", return_value={}), \
             mock.patch.object(f, "rtti_off_units", return_value=set()), \
             mock.patch.object(W, "_layout", return_value=({}, None)), \
             mock.patch.object(W, "resolve_unit", return_value=None), \
             mock.patch.object(P, "unit_has_rules", return_value=False):
            res = f.measure("us")
            headers = f.measure_unpaired_headers(res)
        self.assertEqual(res["A"]["hard"]["packed_string"], 2)
        self.assertEqual(res["A"]["score"], sum(res["A"]["hard"].values()))
        self.assertEqual(headers["hdr/src/Side.hpp"]["hard"]["packed_string"], 1)
        self.assertNotIn("hdr/src/Side.ctx.c", headers)


if __name__ == "__main__":
    unittest.main()

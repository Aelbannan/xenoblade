"""Regression tests for the Shift-JIS build landmine guard.

Background: MWCC only reads Shift JIS, so every TU passes through sjiswrap,
which re-encodes UTF-8 sources on the fly. A single character with no
Shift-JIS encoding (e.g. U+2014 EM DASH) makes sjiswrap emit
"contains Shift JIS encoding errors" for the TU. The breakage is latent —
an up-to-date .o masks it until the source is touched — so 60+ TUs were
silently uneditable until a batch replaced comment-only punctuation with
ASCII equivalents (plus tools/coop/check_sjis.py to stop it recurring).

These tests pin down (a) the checker flags what sjiswrap rejects,
(b) clean files (ASCII, SJIS-safe arrows/box-drawing/Japanese) pass, and
(c) the repo tree stays clean outside the explicit allowlist.
"""

from __future__ import annotations

import pathlib
import sys
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools" / "coop"))

import check_sjis as S  # noqa: E402  (path inserted above)

EM_DASH_CPP = "// retail interleaves the two \u2014 exact issue\nint f() { return 0; }\n"
EM_DASH_LITERAL_CPP = 'const char* s = "a\u2014b";\n'
ARROW_CPP = "// A \u2192 B box \u2500 ellipsis \u2026 theta \u03b8\nint f() { return 0; }\n"
JP_CPP = "// \u3057\u304f\u307f setter\nint f() { return 0; }\n"
CLEAN_CPP = "int f() { return 0; }\n"


class ScanText(unittest.TestCase):
    def test_em_dash_in_comment_is_flagged(self):
        hits = S.scan_text(EM_DASH_CPP, "x.cpp")
        self.assertEqual(len(hits), 1)
        self.assertEqual(hits[0]["line"], 1)
        self.assertIn("U+2014", hits[0]["codepoints"])

    def test_em_dash_in_string_is_flagged(self):
        # sjiswrap encodes the whole file without parsing C++ — a bad byte in
        # a literal breaks the build too (which is why fixes stay comment-only).
        hits = S.scan_text(EM_DASH_LITERAL_CPP, "x.cpp")
        self.assertEqual(len(hits), 1)
        self.assertIn("U+2014", hits[0]["codepoints"])

    def test_sjis_safe_punctuation_passes(self):
        # arrows / box-drawing / ellipsis / theta all have SJIS encodings,
        # so sjiswrap accepts them and the guard must not flag them.
        self.assertEqual(S.scan_text(ARROW_CPP, "x.cpp"), [])

    def test_japanese_passes(self):
        self.assertEqual(S.scan_text(JP_CPP, "x.cpp"), [])

    def test_clean_file_passes(self):
        self.assertEqual(S.scan_text(CLEAN_CPP, "x.cpp"), [])

    def test_non_utf8_bytes_flagged(self):
        hits = S.scan_bytes(b"int f() { return \xff; }\n", "x.cpp")
        self.assertEqual(len(hits), 1)
        self.assertIn("not valid UTF-8", hits[0]["error"])


class ScanFile(unittest.TestCase):
    def test_allowlisted_path_is_skipped(self):
        with tempfile.TemporaryDirectory() as td:
            p = pathlib.Path(td) / "encjapanese.c"
            p.write_text(EM_DASH_CPP, encoding="utf-8")
            # same basename elsewhere is NOT exempt ...
            self.assertEqual(len(S.scan_file(p)), 1)
            # ... but the real allowlisted repo path is
            real = ROOT / "libs/RVL_SDK/src/revolution/enc/encjapanese.c"
            self.assertEqual(S.scan_file(real), [])

    def test_ctx_c_is_not_a_source(self):
        names = [p.name for p in S.iter_sources([ROOT / "src" / "kyoshin"])]
        self.assertNotIn("CTitle.ctx.c", names)


class RepoTree(unittest.TestCase):
    def test_repo_tree_is_sjis_clean(self):
        """src/ and libs/ must hold no non-SJIS bytes outside ALLOWLIST."""
        hits = S.check_tree()
        self.assertEqual(hits, [], f"sjis violations: {hits}")


if __name__ == "__main__":
    unittest.main()

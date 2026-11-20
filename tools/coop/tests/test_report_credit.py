"""Post-pass crediting for objdiff reports.

Covers both artifacts handled by :mod:`tools.coop.lib.report_credit`:

* retail linker tail padding (the ``run.py data diff`` rule, now projected
  into an adjusted report copy);
* anonymous MWCC ``@N`` literal relocs (pluginEve fadeIn/fadeOut shape):
  same instruction bytes, same reloc sites/types, differing local literal
  numbers but identical literal content.

Every failure case must fail closed: the raw report copy must stay untouched.
"""

from __future__ import annotations

import json
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))

from tools.coop.lib.report_credit import credit_report  # noqa: E402
from tools.coop.tests.test_data_link_tail import _make_data_object  # noqa: E402

SHT_PROGBITS = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_REL = 9
SHF_ALLOC = 2
SHF_EXECINSTR = 4
STB_GLOBAL = 1
STT_FUNC = 2
STT_OBJECT = 1
R_PPC_EMB_SDA21 = 109


def _make_code_object(
    path: Path,
    *,
    code: bytes,
    rodata: bytes,
    symbol: str,
    function: str = "fadeIn",
    reloc_type: int = R_PPC_EMB_SDA21,
    reloc_offset: int = 0,
) -> Path:
    """Minimal ET_REL: .text (one function), .rodata, .rel.text, symtab."""
    names = ["", ".text", ".rodata", ".rel.text", ".symtab", ".strtab", ".shstrtab"]
    shname = {}
    shstr = b"\0"
    for name in names[1:]:
        shname[name] = len(shstr)
        shstr += name.encode() + b"\0"
    strtab = b"\0" + function.encode() + b"\0" + symbol.encode() + b"\0"
    name_off = 1
    sym_off = 1 + len(function) + 1

    body = bytearray()
    def place(data: bytes) -> int:
        while (0x34 + len(body)) % 4:
            body.append(0)
        off = 0x34 + len(body)
        body.extend(data)
        return off

    off_text = place(code)
    off_rodata = place(rodata)
    off_rel = place(struct.pack(">II", reloc_offset, (2 << 8) | reloc_type))
    # symtab: null, function, anonymous local literal
    syms = b"\0" * 16
    syms += struct.pack(">IIIBBH", name_off, 0, len(code),
                        (STB_GLOBAL << 4) | STT_FUNC, 0, 1)
    syms += struct.pack(">IIIBBH", sym_off, 0, 0,
                        (STB_GLOBAL << 4) | STT_OBJECT, 0, 2)
    off_symtab = place(syms)
    off_strtab = place(strtab)
    off_shstr = place(shstr)
    while (0x34 + len(body)) % 4:
        body.append(0)
    shoff = 0x34 + len(body)

    def shdr(name: str, typ: int, flags: int, off: int, size: int,
             link: int = 0, info: int = 0, align: int = 4, entsize: int = 0) -> bytes:
        return struct.pack(">IIIIIIIIII", shname[name], typ, flags, 0, off,
                           size, link, info, align, entsize)

    shdrs = [
        b"\0" * 40,
        shdr(".text", SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR, off_text, len(code)),
        shdr(".rodata", SHT_PROGBITS, SHF_ALLOC, off_rodata, len(rodata)),
        shdr(".rel.text", SHT_REL, 0, off_rel, 8, link=4, info=1, align=4, entsize=8),
        shdr(".symtab", SHT_SYMTAB, 0, off_symtab, len(syms), link=5, align=4, entsize=16),
        shdr(".strtab", SHT_STRTAB, 0, off_strtab, len(strtab), align=1),
        shdr(".shstrtab", SHT_STRTAB, 0, off_shstr, len(shstr), align=1),
    ]
    ehdr = (b"\x7fELF\x01\x02\x01" + b"\0" * 9
            + struct.pack(">HHIIIIIHHHHHH", 1, 20, 1, 0, 0, shoff, 0,
                          0x34, 0, 0, 40, len(shdrs), 6))
    path.write_bytes(ehdr + bytes(body) + b"".join(shdrs))
    return path


def _report(*, rodata_pct: float = 99.0, fn_pct: float = 95.0) -> dict:
    return {
        "measures": {
            "fuzzy_match_percent": 90.0, "total_code": "16", "matched_code": "0",
            "total_data": "24", "matched_data": "0", "total_functions": 1,
            "matched_functions": 0,
        },
        "units": [{
            "name": "main/u",
            "measures": {
                "fuzzy_match_percent": 90.0, "total_code": "16", "matched_code": "0",
                "matched_code_percent": 0.0, "total_data": "24", "matched_data": "0",
                "matched_data_percent": 0.0, "total_functions": 1,
                "matched_functions": 0, "matched_functions_percent": 0.0,
            },
            "sections": [
                {"name": ".text", "size": "16", "fuzzy_match_percent": 95.0},
                {"name": ".rodata", "size": "24", "fuzzy_match_percent": rodata_pct},
            ],
            "functions": [
                {"name": "fadeIn", "size": "16", "fuzzy_match_percent": fn_pct},
            ],
        }],
    }


class PaddingCreditTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.d = Path(self.tmp.name)
        self.literal = b"WorkThreadSystem\0"

    def tearDown(self):
        self.tmp.cleanup()

    def _pair(self, retail: bytes, decomp: bytes, *, align: int = 8):
        r = _make_data_object(self.d / "r.o", retail, align=align)
        c = _make_data_object(self.d / "c.o", decomp, align=align)
        return r, c

    def _run(self, retail: bytes, decomp: bytes, *, align: int = 8):
        r, c = self._pair(retail, decomp, align=align)
        return credit_report(_report(), {"main/u": (r, c)},
                             credit_anonymous_relocs=False)

    def test_normal_case_is_credited_and_raw_numbers_kept(self):
        report = _report()
        adjusted, census = self._run(self.literal + bytes(3), self.literal)
        sec = adjusted["units"][0]["sections"][1]
        self.assertEqual(sec["fuzzy_match_percent"], 100.0)
        self.assertEqual(sec["raw_fuzzy_match_percent"], 99.0)
        self.assertEqual(sec["pad_credit_bytes"], 3)
        self.assertEqual(census.sections_credited, 1)
        self.assertEqual(census.bytes_credited, 3)
        self.assertEqual(census.units_changed, 1)
        unit = adjusted["units"][0]
        self.assertEqual(unit["measures"]["matched_data"], "24")
        self.assertEqual(unit["raw_measures"]["matched_data"], "0")
        self.assertEqual(adjusted["raw_measures"]["matched_data"], "0")

        self.assertEqual(report["units"][0]["sections"][1]["fuzzy_match_percent"], 99.0)
        self.assertNotIn("credit", report)

    def test_nonzero_tail_is_rejected(self):
        _, census = self._run(self.literal + b"\0\x01\0", self.literal)
        self.assertEqual(census.sections_credited, 0)
        self.assertEqual(census.bytes_credited, 0)

    def test_tail_at_alignment_is_rejected(self):
        _, census = self._run(self.literal + bytes(8), self.literal)
        self.assertEqual(census.sections_credited, 0)

    def test_unaligned_retail_end_is_rejected(self):
        literal = b"abc\0"
        _, census = self._run(literal + bytes(3), literal)
        self.assertEqual(census.sections_credited, 0)

    def test_differing_prefix_is_rejected(self):
        _, census = self._run(self.literal + bytes(3), self.literal[:-1] + b"X")
        self.assertEqual(census.sections_credited, 0)

    def test_adjusted_report_cannot_be_recredited(self):
        adjusted, _ = self._run(self.literal + bytes(3), self.literal)
        with self.assertRaises(ValueError):
            credit_report(adjusted, {})


class AnonymousRelocCreditTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.d = Path(self.tmp.name)
        self.code = b"\x60\x00\x00\x00" + b"\x48\x00\x00\x01" + b"\x4e\x80\x00\x20" + b"\x60\x00\x00\x00"

    def tearDown(self):
        self.tmp.cleanup()

    def _run(self, *, retail_lit=b"1.000", decomp_lit=b"1.000",
             retail_sym="@1254", decomp_sym="@18982",
             code=None, decomp_code=None, reloc_type=R_PPC_EMB_SDA21,
             decomp_reloc_type=None, anonym=True):
        if not anonym:
            retail_sym, decomp_sym = "lbl_retail", "lbl_decomp"
        r = _make_code_object(self.d / "r.o", code=self.code if code is None else code,
                              rodata=retail_lit, symbol=retail_sym,
                              reloc_type=reloc_type)
        c = _make_code_object(self.d / "c.o",
                              code=self.code if decomp_code is None else decomp_code,
                              rodata=decomp_lit, symbol=decomp_sym,
                              reloc_type=decomp_reloc_type or reloc_type)
        return credit_report(_report(rodata_pct=100.0), {"main/u": (r, c)})

    def test_same_content_different_anon_number_is_credited(self):
        adjusted, census = self._run()
        fn = adjusted["units"][0]["functions"][0]
        self.assertEqual(fn["fuzzy_match_percent"], 100.0)
        self.assertEqual(fn["raw_fuzzy_match_percent"], 95.0)
        self.assertEqual(fn["anonymous_reloc_credit"][0]["retail_symbol"], "@1254")
        self.assertEqual(census.functions_credited, 1)
        self.assertEqual(adjusted["units"][0]["measures"]["matched_code"], "16")
        self.assertEqual(adjusted["units"][0]["raw_measures"]["matched_code"], "0")
        self.assertEqual(adjusted["measures"]["matched_functions"], 1)

    def test_different_content_is_rejected(self):
        _, census = self._run(decomp_lit=b"2.000")
        self.assertEqual(census.functions_credited, 0)

    def test_named_symbol_drift_is_not_anonymous(self):
        _, census = self._run(anonym=False)
        self.assertEqual(census.functions_credited, 0)

    def test_different_instructions_are_rejected(self):
        _, census = self._run(decomp_code=self.code[:-4] + b"\x60\x00\x00\x01")


    def test_different_reloc_type_is_rejected(self):
        _, census = self._run(decomp_reloc_type=1)


    def test_flag_can_disable_anonymous_credits(self):
        r = _make_code_object(self.d / "r.o", code=self.code, rodata=b"1.000",
                              symbol="@1254")
        c = _make_code_object(self.d / "c.o", code=self.code, rodata=b"1.000",
                              symbol="@18982")
        _, census = credit_report(_report(rodata_pct=100.0), {"main/u": (r, c)},
                                  credit_anonymous_relocs=False)
        self.assertEqual(census.functions_credited, 0)


class CreditCliTest(unittest.TestCase):
    def test_cli_never_overwrites_raw_report(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            literal = b"WorkThreadSystem\0"
            r = _make_data_object(root / "r.o", literal + bytes(3), align=8)
            c = _make_data_object(root / "c.o", literal, align=8)
            raw = root / "report.json"
            out = root / "adjusted.json"
            raw.write_text(json.dumps(_report()), encoding="utf-8")
            (root / "objdiff.json").write_text(json.dumps({"units": [{
                "name": "main/u", "target_path": str(r), "base_path": str(c),
            }]}), encoding="utf-8")
            (root / "coop.json").write_text(json.dumps({
                "project_root": ".", "region": "us",
            }), encoding="utf-8")
            before = raw.read_bytes()
            proc = subprocess.run(
                [sys.executable, str(ROOT / "tools/coop/run.py"),
                 "--config", str(root / "coop.json"),
                 "report-credit-padding", "--input", str(raw),
                 "--output", str(out), "--objdiff-json", str(root / "objdiff.json"),
                 "--no-anonymous-relocs", "--json"],
                cwd=ROOT, capture_output=True, text=True,
            )
            self.assertEqual(proc.returncode, 0, proc.stderr)
            census = json.loads(proc.stdout)
            self.assertEqual(census["sections_credited"], 1)
            self.assertEqual(raw.read_bytes(), before)
            adjusted = json.loads(out.read_text(encoding="utf-8"))
            self.assertEqual(adjusted["credit"]["source_report"], str(raw))


if __name__ == "__main__":
    unittest.main()

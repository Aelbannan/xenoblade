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
SHT_RELA = 4
SHF_ALLOC = 2
SHF_EXECINSTR = 4
STB_LOCAL = 0
STB_GLOBAL = 1
STB_WEAK = 2
STT_FUNC = 2
STT_OBJECT = 1
R_PPC_ADDR32 = 1
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


def _make_rel_object(path: Path, sections: dict[str, bytes], symbols: list[tuple],
                     relocs: dict[str, list[tuple[int, str, int]]] | None = None) -> Path:
    """ET_REL with arbitrary sections and RELA relocs, for weak-dedup fixtures.

    ``symbols`` entries are (name, section, value, size, bind, type);
    ``relocs`` maps a section to (offset, symbol name, addend) triples.
    """
    relocs = relocs or {}
    rela_names = [f".rela{n}" for n in sections if relocs.get(n)]
    names = list(sections) + rela_names + [".symtab", ".strtab", ".shstrtab"]
    idx = {n: i + 1 for i, n in enumerate(names)}
    shstr, shname = b"\0", {}
    for name in names:
        shname[name] = len(shstr)
        shstr += name.encode() + b"\0"
    strtab, syms, sym_index = b"\0", [b"\0" * 16], {}
    for name, sec, value, size, bind, typ in sorted(symbols, key=lambda s: s[4] != STB_LOCAL):
        sym_index[name] = len(syms)
        syms.append(struct.pack(">IIIBBH", len(strtab), value, size,
                                (bind << 4) | typ, 0, idx[sec]))
        strtab += name.encode() + b"\0"
    blobs = dict(sections)
    for rela in rela_names:
        target = rela[5:]
        blobs[rela] = b"".join(
            struct.pack(">IIi", off, sym_index[sym] << 8 | R_PPC_ADDR32, addend)
            for off, sym, addend in relocs[target])
    blobs[".symtab"] = b"".join(syms)
    blobs[".strtab"] = strtab
    blobs[".shstrtab"] = shstr

    body, offs = bytearray(), {}
    for name in names:
        while (0x34 + len(body)) % 4:
            body.append(0)
        offs[name] = 0x34 + len(body)
        body.extend(blobs[name])
    while (0x34 + len(body)) % 4:
        body.append(0)
    shoff = 0x34 + len(body)
    shdrs = [b"\0" * 40]
    for name in names:
        typ = {".symtab": SHT_SYMTAB, ".strtab": SHT_STRTAB,
               ".shstrtab": SHT_STRTAB}.get(name, SHT_PROGBITS)
        if name in rela_names:
            typ = SHT_RELA
        link = idx[".symtab"] if name in rela_names else (
            idx[".strtab"] if name == ".symtab" else 0)
        info = idx[name[5:]] if name in rela_names else 0
        shdrs.append(struct.pack(">IIIIIIIIII", shname[name],
                                 typ, 0, 0, offs[name], len(blobs[name]),
                                 link, info, 4, 0))
    ehdr = (b"\x7fELF\x01\x02\x01" + b"\0" * 9
            + struct.pack(">HHIIIIIHHHHHH", 1, 20, 1, 0, 0, shoff, 0,
                          0x34, 0, 0, 40, len(shdrs), idx[".shstrtab"]))
    path.write_bytes(ehdr + bytes(body) + b"".join(shdrs))
    return path


def _weak_project(root: Path) -> tuple[Path, Path]:
    """Splits/symbols layout with this unit and a different weak-copy owner."""
    cfg = root / "config" / "us"
    cfg.mkdir(parents=True, exist_ok=True)
    splits = cfg / "splits.txt"
    splits.write_text(
        "Sections:\n"
        "\t.text       type:code align:32\n"
        "\t.rodata     type:rodata align:32\n"
        "\t.data       type:data align:16\n"
        "\t.sdata      type:data align:16\n"
        "\textab       type:rodata align:32\n"
        "\textabindex  type:rodata align:32\n\n"
        "other/Base.cpp:\n"
        "\t.text start:0x80000000 end:0x80000100\n"
        "\t.rodata start:0x80300000 end:0x80300040\n"
        "\t.sdata start:0x80600000 end:0x80600010\n\n"
        "this/Unit.cpp:\n"
        "\t.text start:0x80001000 end:0x80001100\n"
        "\t.rodata start:0x80301000 end:0x80301040\n"
        "\t.sdata start:0x80601000 end:0x80601010\n"
        "\textab start:0x80006000 end:0x80006040\n"
        "\textabindex start:0x80020000 end:0x80020040\n",
        encoding="utf-8")
    symbols = cfg / "symbols.txt"
    symbols.write_text(
        "f = .text:0x80001000;\n"
        "keep = .sdata:0x80601000;\n"
        "__dt__4BaseFv = .text:0x80000010;\n"
        "__RTTI__4Base = .sdata:0x80600004;\n",
        encoding="utf-8")
    return root, cfg


def _weak_objects(root: Path, *, sdata: bytes = b"\0" * 4):
    """Decomp object with a weak RTTI/dtor copy; retail object without it."""
    retail = _make_rel_object(
        root / "r.o",
        sections={".text": b"\x60\0\0\0" * 4, ".rodata": b"Unit",
                  ".sdata": sdata},
        symbols=[("f", ".text", 0, 0x10, STB_GLOBAL, STT_FUNC),
                 ("@1", ".rodata", 0, 4, STB_LOCAL, STT_OBJECT),
                 ("keep", ".sdata", 0, 4, STB_GLOBAL, STT_OBJECT)],
        relocs={".sdata": [(0, "@1", 0)]})
    decomp = _make_rel_object(
        root / "c.o",
        sections={".text": b"\x60\0\0\0" * 8, ".rodata": b"UnitBase",
                  ".sdata": b"\0" * 8},
        symbols=[("f", ".text", 0, 0x10, STB_GLOBAL, STT_FUNC),
                 ("__dt__4BaseFv", ".text", 0x10, 0x10, STB_WEAK, STT_FUNC),
                 ("@1", ".rodata", 0, 4, STB_LOCAL, STT_OBJECT),
                 ("@2", ".rodata", 4, 4, STB_LOCAL, STT_OBJECT),
                 ("keep", ".sdata", 0, 4, STB_GLOBAL, STT_OBJECT),
                 ("__RTTI__4Base", ".sdata", 4, 4, STB_WEAK, STT_OBJECT)],
        relocs={".sdata": [(0, "@1", 0), (4, "@2", 0)]})
    return retail, decomp


def _weak_report(*, sections=None) -> dict:
    return {
        "measures": {"total_code": "0", "matched_code": "0",
                      "total_data": "12", "matched_data": "0",
                      "total_functions": 0, "matched_functions": 0},
        "units": [{
            "name": "main/this/Unit",
            "measures": {"total_code": "0", "matched_code": "0",
                          "total_data": "12", "matched_data": "0",
                          "total_functions": 0, "matched_functions": 0},
            "sections": sections if sections is not None else [
                {"name": ".rodata", "size": "4", "fuzzy_match_percent": 50.0},
                {"name": ".sdata", "size": "4", "fuzzy_match_percent": 50.0},
            ],
            "functions": [],
        }],
    }


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


class WeakDedupCreditTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        _weak_project(self.root)
        self.r, self.c = _weak_objects(self.root)

    def tearDown(self):
        self.tmp.cleanup()

    def _run(self, report=None, *, objects=None, **kwargs):
        return credit_report(
            report or _weak_report(),
            {"main/this/Unit": objects or (self.r, self.c)},
            credit_anonymous_relocs=False, project_root=self.root,
            region="us", **kwargs)

    def test_linked_view_exclusions_are_credited(self):
        report = _weak_report()
        adjusted, census = self._run(report)
        secs = adjusted["units"][0]["sections"]
        for sec in secs:
            self.assertEqual(sec["fuzzy_match_percent"], 100.0)
            self.assertEqual(sec["raw_fuzzy_match_percent"], 50.0)
            self.assertIn("__RTTI__4Base", str(sec["weak_dedup_credit"]["exclusions"]))
            self.assertIn("identical", sec["weak_dedup_credit"]["proof"])
        self.assertEqual(census.weak_units_viewed, 1)
        self.assertEqual((census.weak_data_sections, census.weak_extab_sections), (2, 0))
        self.assertEqual(census.weak_bytes_credited, 8)
        unit = adjusted["units"][0]
        self.assertEqual(unit["measures"]["matched_data"], "8")
        self.assertEqual(unit["raw_measures"]["matched_data"], "0")
        self.assertEqual(adjusted["credit"]["weak_dedup"], True)
        # Raw input is never mutated.
        self.assertEqual([s["fuzzy_match_percent"] for s in report["units"][0]["sections"]],
                         [50.0, 50.0])

    def test_no_exclusions_leaves_raw_percentages(self):
        empty = self.root / "no-layout"
        empty.mkdir()
        _, census = credit_report(
            _weak_report(), {"main/this/Unit": (self.r, self.c)},
            credit_anonymous_relocs=False, project_root=empty, region="us")
        self.assertEqual(census.weak_data_sections, 0)
        self.assertEqual(census.weak_units_viewed, 0)

    def test_flag_disables_weak_credits(self):
        adjusted, census = self._run(credit_weak_dedup=False)
        self.assertEqual(census.weak_data_sections, 0)
        self.assertEqual(adjusted["credit"]["weak_dedup"], False)
        self.assertEqual([s["fuzzy_match_percent"] for s in adjusted["units"][0]["sections"]],
                         [50.0, 50.0])

    def test_unproven_section_is_left_alone(self):
        objects = _weak_objects(self.root, sdata=b"\x01\0\0\0")
        adjusted, census = self._run(objects=objects)
        rodata, sdata = adjusted["units"][0]["sections"]
        self.assertEqual(rodata["fuzzy_match_percent"], 100.0)
        self.assertEqual(sdata["fuzzy_match_percent"], 50.0)
        self.assertEqual(census.weak_data_sections, 1)

    def test_extab_sections_use_the_same_proven_linked_view(self):
        record0 = b"\0\0\0\0\x01\0\0\0"
        record1 = b"\0\0\0\0\x02\0\0\0"
        triple = b"\0" * 4 + struct.pack(">I", 0x10) + b"\0" * 4
        retail = _make_rel_object(
            self.root / "re.o",
            sections={".text": b"\x60\0\0\0" * 4, "extab": record0,
                      "extabindex": triple},
            symbols=[("f", ".text", 0, 0x10, STB_GLOBAL, STT_FUNC),
                     ("@rec0", "extab", 0, 8, STB_LOCAL, STT_OBJECT)],
            relocs={"extabindex": [(0, "f", 0), (8, "@rec0", 0)]})
        decomp = _make_rel_object(
            self.root / "ce.o",
            sections={".text": b"\x60\0\0\0" * 8, "extab": record0 + record1,
                      "extabindex": triple + triple},
            symbols=[("f", ".text", 0, 0x10, STB_GLOBAL, STT_FUNC),
                     ("__dt__4BaseFv", ".text", 0x10, 0x10, STB_WEAK, STT_FUNC),
                     ("@rec0", "extab", 0, 8, STB_LOCAL, STT_OBJECT),
                     ("@rec1", "extab", 8, 8, STB_LOCAL, STT_OBJECT)],
            relocs={"extabindex": [(0, "f", 0), (8, "@rec0", 0),
                                    (12, "__dt__4BaseFv", 0), (20, "@rec1", 0)]})
        report = _weak_report(sections=[
            {"name": "extab", "size": "8", "fuzzy_match_percent": 50.0},
            {"name": "extabindex", "size": "12", "fuzzy_match_percent": 50.0},
        ])
        adjusted, census = self._run(report, objects=(retail, decomp))
        secs = adjusted["units"][0]["sections"]
        self.assertEqual([s["fuzzy_match_percent"] for s in secs], [100.0, 100.0])
        self.assertEqual(census.weak_extab_sections, 2)
        self.assertEqual(census.weak_data_sections, 0)


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

    def test_cli_no_weak_dedup_flag(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            _weak_project(root)
            r, c = _weak_objects(root)
            raw = root / "report.json"
            raw.write_text(json.dumps(_weak_report()), encoding="utf-8")
            objdiff = root / "objdiff.json"
            objdiff.write_text(json.dumps({"units": [{
                "name": "main/this/Unit", "target_path": str(r),
                "base_path": str(c),
            }]}), encoding="utf-8")
            cfg = root / "coop.json"
            cfg.write_text(json.dumps({"project_root": str(root), "region": "us"}),
                           encoding="utf-8")

            def invoke(extra: list[str], out: Path):
                return subprocess.run(
                    [sys.executable, str(ROOT / "tools/coop/run.py"),
                     "--config", str(cfg), "report-credit-padding",
                     "--input", str(raw), "--output", str(out),
                     "--objdiff-json", str(objdiff),
                     "--no-anonymous-relocs", *extra, "--json"],
                    cwd=ROOT, capture_output=True, text=True)

            enabled = root / "enabled.json"
            disabled = root / "disabled.json"
            on = invoke([], enabled)
            off = invoke(["--no-weak-dedup"], disabled)
            self.assertEqual(on.returncode, 0, on.stderr)
            self.assertEqual(off.returncode, 0, off.stderr)
            self.assertEqual(json.loads(on.stdout)["weak_data_sections"], 2)
            self.assertEqual(json.loads(off.stdout)["weak_data_sections"], 0)
            secs = json.loads(disabled.read_text(encoding="utf-8"))["units"][0]["sections"]
            self.assertEqual([s["fuzzy_match_percent"] for s in secs], [50.0, 50.0])


if __name__ == "__main__":
    unittest.main()

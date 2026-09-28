"""Category A comparator tests: canonical reloc resolution + ADDR16 linker absolutes."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.data_match import (
    DATA_SECTIONS,
    ParsedObject,
    Reloc,
    canonical_reloc_key,
    compare_reloc_sets,
    derive_section_bases,
)
from tools.coop.lib.equivalence_check import _byte_identical_with_relocs
from tools.coop.lib.reloc_canon import (
    ADDR16_HA,
    ADDR16_HI,
    ADDR16_LO,
    RETAIL_LINKER_ABSOLUTES,
    R_PPC_ADDR16_HA,
    R_PPC_ADDR16_HI,
    R_PPC_ADDR16_LO,
    R_PPC_REL24,
    LinkerLayout,
    addr16_immediate,
    eval_ldscript,
    parse_splits,
    parse_symbols,
    type_class,
    verify_known_absolutes,
)

REPO_ROOT = Path(__file__).resolve().parents[3]


# ── ADDR16 immediates ──────────────────────────────────────────────────────


class Addr16ImmediateTests(unittest.TestCase):
    def test_hi_lo_ha_of_baked_stack_addr(self):
        # __start.o / OSThread.o: lis r1,0x8067 / ori r1,r1,0xb560
        self.assertEqual(addr16_immediate(0x8067B560, R_PPC_ADDR16_HI), 0x8067)
        self.assertEqual(addr16_immediate(0x8067B560, R_PPC_ADDR16_LO), 0xB560)
        # low half >= 0x8000: @ha carries the +0x8000 so the sign-extending
        # addi/ori pair still reconstructs the address.
        self.assertEqual(addr16_immediate(0x8067B560, R_PPC_ADDR16_HA), 0x8068)

    def test_ha_is_sign_corrected_hi(self):
        # @ha differs from @hi exactly when bit 15 of the address is set.
        self.assertEqual(addr16_immediate(0x80007FFF, R_PPC_ADDR16_HI), 0x8000)
        self.assertEqual(addr16_immediate(0x80007FFF, R_PPC_ADDR16_HA), 0x8000)
        self.assertEqual(addr16_immediate(0x80008000, R_PPC_ADDR16_HI), 0x8000)
        self.assertEqual(addr16_immediate(0x80008000, R_PPC_ADDR16_HA), 0x8001)

    def test_non_addr16_type_is_none(self):
        self.assertIsNone(addr16_immediate(0x80004000, 10))  # R_PPC_REL24

    def test_type_classes(self):
        self.assertEqual(type_class(R_PPC_ADDR16_HA), ADDR16_HA)
        self.assertEqual(type_class(R_PPC_ADDR16_HI), ADDR16_HI)
        self.assertEqual(type_class(R_PPC_ADDR16_LO), ADDR16_LO)
        self.assertNotEqual(type_class(R_PPC_ADDR16_HI), type_class(R_PPC_ADDR16_LO))


# ── ldscript / splits parsing ──────────────────────────────────────────────


def _write_synthetic_layout(root: Path, *, sbss2_start: int, sbss2_end: int) -> None:
    (root / "config" / "xx").mkdir(parents=True)
    (root / "build" / "xx").mkdir(parents=True)
    (root / "config" / "xx" / "splits.txt").write_text(
        "Sections:\n"
        "\t.text       type:code align:32\n"
        "\t.sbss2      type:bss align:32\n"
        "\nfoo/Bar.cpp:\n"
        f"\t.text       start:0x80004000 end:0x80005000\n"
        f"\t.sbss2      start:0x{sbss2_start:08X} end:0x{sbss2_end:08X}\n"
    )
    (root / "config" / "xx" / "symbols.txt").write_text(
        "memcpy = .text:0x80004000; // type:function size:0x29C scope:global\n"
        "@86 = .sbss2:0x80700000; // type:object size:0x4 scope:local\n"
    )
    (root / "build" / "xx" / "ldscript.lcf").write_text(
        "MEMORY\n{\n    text : origin = 0x80004000\n}\n"
        "SECTIONS\n{\n"
        "    _stack_end = _f_sbss2 + SIZEOF(.sbss2);\n"
        "    _stack_addr = (_stack_end + 0xFFFF + 0x7) & ~0x7;\n"
        "    _db_stack_addr = (_stack_addr + 0x2000);\n"
        "    _db_stack_end = _stack_addr;\n"
        "    __ArenaLo = (_db_stack_addr + 0x1f) & ~0x1f;\n"
        "    __ArenaHi = 0x81700000;\n"
        "}\n"
    )


class LayoutTests(unittest.TestCase):
    def test_ldscript_formula_evaluation(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            # sbss2 = 0x8066B540..0x8066B55C (US retail) -> _stack_addr 0x8067B560
            _write_synthetic_layout(root, sbss2_start=0x8066B540, sbss2_end=0x8066B55C)
            layout = LinkerLayout.load(root, "xx")
            self.assertEqual(layout.linker_absolutes["_stack_addr"], 0x8067B560)
            self.assertEqual(layout.linker_absolutes["_db_stack_end"], 0x8067B560)
            self.assertEqual(layout.linker_absolutes["__ArenaLo"], 0x8067D560)
            self.assertEqual(layout.linker_absolutes["_stack_end"], 0x8066B55C)

    def test_ldscript_formula_tracks_layout_drift(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            _write_synthetic_layout(root, sbss2_start=0x8066B540, sbss2_end=0x8066B564)
            layout = LinkerLayout.load(root, "xx")
            # +8 bytes of .sbss2 -> the honest link can no longer bake 0x8067B560
            self.assertEqual(layout.linker_absolutes["_stack_addr"], 0x8067B568)

    def test_parse_splits_and_symbols(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            _write_synthetic_layout(root, sbss2_start=0x8066B540, sbss2_end=0x8066B55C)
            sections = parse_splits(root / "config" / "xx" / "splits.txt")
            self.assertEqual(sections[".sbss2"].start, 0x8066B540)
            self.assertEqual(sections[".sbss2"].size, 0x1C)
            symbols = parse_symbols(root / "config" / "xx" / "symbols.txt")
            self.assertEqual(symbols["memcpy"], (".text", 0x80004000))

    def test_verify_known_absolutes_flags_drift(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            _write_synthetic_layout(root, sbss2_start=0x8066B540, sbss2_end=0x8066B564)
            layout = LinkerLayout.load(root, "xx")
            problems = verify_known_absolutes(layout)
            self.assertTrue(any("_stack_addr" in p for p in problems))

    def test_real_region_layout_matches_the_retired_bakes(self):
        """The three bake_linker_addrs values + memcpy stay resolvable as retail."""
        layout = LinkerLayout.load(REPO_ROOT, "us")
        self.assertEqual(verify_known_absolutes(layout), [])
        for name, expected in RETAIL_LINKER_ABSOLUTES.items():
            _, addr = layout.resolve_name(name)
            self.assertEqual(addr, expected, name)


# ── canonical reloc targets ────────────────────────────────────────────────


def _parsed(section: str, symbols: dict[str, tuple[str, int]], relocs: list[Reloc]) -> ParsedObject:
    """Build a ParsedObject for one data section plus its symbol homes."""
    parsed = ParsedObject()
    names: dict[int, str] = {0: ""}
    for sec in [section] + [home for home, _ in symbols.values()]:
        if sec and sec not in names.values():
            names[len(names)] = sec
    parsed.section_names = names
    parsed.sections = {s: {"off": 0, "size": 0x40, "addr": 0, "align": 4}
                       for s in names.values() if s}
    for name, (home, value) in symbols.items():
        parsed.symbols[name] = (next(i for i, s in names.items() if s == home), value)
    parsed.relocs = {section: relocs}
    return parsed


class CanonicalRelocTests(unittest.TestCase):
    def setUp(self):
        layout = LinkerLayout.load(REPO_ROOT, "us")
        self.layout = layout

    def test_object_local_definition_wins_over_symbols_txt_collision(self):
        # `@86` is a TU-local name that also exists in symbols.txt for another
        # unit.  The object's own (section, st_value) must win.
        base = 0x804FA1E0  # .rodata of the New unit
        parsed = _parsed(".rodata", {"@86": (".rodata", 0)},
                         [Reloc(0, 1, "@86", 0)])
        parsed.symbols["lbl_global"] = (0, 0)  # UNDEF, unused
        bases = {".rodata": base}
        key = canonical_reloc_key(parsed.relocs[".rodata"][0], parsed, self.layout, bases)
        self.assertEqual(key, ("ADDR32", "abs", base))

    def test_linker_absolute_reloc_resolves_through_ldscript(self):
        parsed = _parsed(".rodata", {}, [Reloc(0, R_PPC_ADDR16_LO, "_stack_addr", 0)])
        key = canonical_reloc_key(parsed.relocs[".rodata"][0], parsed, self.layout, {})
        self.assertEqual(key, (ADDR16_LO, "abs", 0x8067B560))
        parsed_hi = _parsed(".rodata", {}, [Reloc(0, R_PPC_ADDR16_HI, "_stack_addr", 0)])
        key_hi = canonical_reloc_key(parsed_hi.relocs[".rodata"][0], parsed_hi, self.layout, {})
        self.assertEqual(key_hi, (ADDR16_HI, "abs", 0x8067B560))

    def test_unresolvable_target_falls_back_to_name(self):
        parsed = _parsed(".rodata", {}, [Reloc(0, 1, "mystery_symbol", 4)])
        key = canonical_reloc_key(parsed.relocs[".rodata"][0], parsed, self.layout, {})
        self.assertEqual(key, ("ADDR32", "name", "mystery_symbol", 4))

    def test_name_drift_with_same_address_compares_equal(self):
        """retail `@86` vs decomp `exceptionName...`, same (section, offset)."""
        retail = _parsed(".rodata", {"@86": (".rodata", 0)}, [Reloc(0, 1, "@86", 0)])
        decomp = _parsed(".rodata", {"exceptionName__16@unnamed@New_cp@": (".rodata", 0)},
                         [Reloc(0, 1, "exceptionName__16@unnamed@New_cp@", 0)])
        bases = {".rodata": 0x804FA1E0}
        rk = canonical_reloc_key(retail.relocs[".rodata"][0], retail, self.layout, bases)
        dk = canonical_reloc_key(decomp.relocs[".rodata"][0], decomp, self.layout, bases)
        self.assertEqual(rk, dk)

    def test_addend_drift_compares_unequal(self):
        retail = _parsed(".rodata", {"@86": (".rodata", 0)}, [Reloc(0, 1, "@86", 0)])
        decomp = _parsed(".rodata", {"@86": (".rodata", 0)}, [Reloc(0, 1, "@86", 4)])
        bases = {".rodata": 0x804FA1E0}
        rk = canonical_reloc_key(retail.relocs[".rodata"][0], retail, self.layout, bases)
        dk = canonical_reloc_key(decomp.relocs[".rodata"][0], decomp, self.layout, bases)
        self.assertNotEqual(rk, dk)

    def test_derive_section_bases_from_retail_symbols(self):
        # `@86` in the synthetic symbols.txt lives at .sbss2:0x80700000; an
        # object-local hit at .sbss2+4 therefore derives base 0x806FFFFC.
        parsed = _parsed(".sbss2", {"lbl_named_data": (".sbss2", 4)}, [])
        bases = derive_section_bases(parsed, self.layout)
        # nothing in the real symbols.txt matches this synthetic name -> no base
        self.assertEqual(bases, {})
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            _write_synthetic_layout(root, sbss2_start=0x8066B540, sbss2_end=0x8066B55C)
            layout = LinkerLayout.load(root, "xx")
            parsed2 = _parsed(".sbss2", {"@86": (".sbss2", 4)}, [])
            # `@86` is TU-local in the object but exists in this symbols.txt;
            # derive_section_bases must skip TU-local names.
            self.assertEqual(derive_section_bases(parsed2, layout), {})

    def test_tu_local_names_do_not_derive_bases(self):
        parsed = _parsed(".sbss2", {"@86": (".sbss2", 0)}, [])
        self.assertEqual(derive_section_bases(parsed, self.layout), {})


class CompareRelocSetTests(unittest.TestCase):
    def setUp(self):
        self.layout = LinkerLayout.load(REPO_ROOT, "us")

    def test_order_insensitive_and_name_drift_canonical(self):
        retail = _parsed(".sdata", {
            "@86": (".rodata", 0),
            "lbl_eu_80663AA8": (".rodata", 8),
        }, [Reloc(0, 1, "@86", 0), Reloc(4, 1, "lbl_eu_80663AA8", 0)])
        decomp = _parsed(".sdata", {
            "exceptionName": (".rodata", 0),
            "sdata_ref_needle": (".rodata", 8),
        }, [Reloc(4, 1, "sdata_ref_needle", 0), Reloc(0, 1, "exceptionName", 0)])
        self.assertIsNone(compare_reloc_sets(
            retail, decomp, ".sdata", self.layout))
        # sanity: with no resolvable definition the names are compared
        del retail.symbols["@86"]
        del decomp.symbols["exceptionName"]
        retail.symbols["@86"] = (0, 0)  # UNDEF: not a definition
        decomp.symbols["exceptionName"] = (0, 0)
        drift = compare_reloc_sets(retail, decomp, ".sdata", self.layout)
        self.assertIsNotNone(drift)

    def test_presence_drift_reported(self):
        retail = _parsed(".sdata", {"@86": (".rodata", 0)}, [Reloc(0, 1, "@86", 0)])
        decomp = _parsed(".sdata", {"@86": (".rodata", 0)}, [])
        drift = compare_reloc_sets(retail, decomp, ".sdata", self.layout)
        self.assertIn("retail-only", drift)

    def test_type_class_drift_reported(self):
        retail = _parsed(".sdata", {"@86": (".rodata", 0)}, [Reloc(0, R_PPC_ADDR16_LO, "@86", 0)])
        decomp = _parsed(".sdata", {"@86": (".rodata", 0)}, [Reloc(0, R_PPC_ADDR16_HI, "@86", 0)])
        drift = compare_reloc_sets(retail, decomp, ".sdata", self.layout)
        self.assertIn("reloc drift", drift)




# ── witness-side canonicalization (equivalence_check) ─────────────────────


def _fn(code: bytes, relocs=()):
    from tools.ppc_equivalence.elf_symbols import FunctionBytes, FunctionRelocation

    return FunctionBytes(
        name="fn", path=Path("fn.o"), code=code, base=0, value=0, size=len(code),
        section_index=1, section_name=".text", symbol_type=2,
        relocations=tuple(FunctionRelocation(*r) for r in relocs),
    )


class WitnessCanonicalRelocTests(unittest.TestCase):
    """`_byte_identical_with_relocs` extensions used by the FULL_MATCH fast path."""

    def setUp(self):
        self.layout = LinkerLayout.load(REPO_ROOT, "us")
        # lis r1,0 / lis r1,0x8067  (the __start.o _stack_addr bake)
        self.decomp = _fn(b"\x3c\x20\x00\x00", [(2, R_PPC_ADDR16_HI, "_stack_addr", 0)])
        self.retail = _fn(b"\x3c\x20\x80\x67", [])

    def test_baked_addr16_matches_materialized_immediate(self):
        self.assertTrue(_byte_identical_with_relocs(
            self.decomp, self.retail, linker_layout=self.layout))

    def test_baked_addr16_requires_layout(self):
        # fail closed without the resolver
        self.assertFalse(_byte_identical_with_relocs(self.decomp, self.retail))

    def test_wrong_baked_immediate_rejected(self):
        wrong = _fn(b"\x3c\x20\x80\x68", [])
        self.assertFalse(_byte_identical_with_relocs(
            self.decomp, wrong, linker_layout=self.layout))

    def test_unresolvable_addr16_rejected(self):
        bogus = _fn(b"\x3c\x20\x00\x00", [(2, R_PPC_ADDR16_HI, "not_a_symbol_xyz", 0)])
        self.assertFalse(_byte_identical_with_relocs(
            bogus, self.retail, linker_layout=self.layout))

    def test_difference_outside_field_rejected(self):
        other = _fn(b"\x3c\x20\x80\x67", [])
        moved = _fn(b"\x3d\x20\x80\x67", [])
        self.assertFalse(_byte_identical_with_relocs(
            other, moved, linker_layout=self.layout))

    def test_lo_and_ha_classes(self):
        # LO of _stack_addr = 0xb560
        decomp_lo = _fn(b"\x60\x21\x00\x00", [(2, R_PPC_ADDR16_LO, "_stack_addr", 0)])
        retail_lo = _fn(b"\x60\x21\xb5\x60", [])
        self.assertTrue(_byte_identical_with_relocs(
            decomp_lo, retail_lo, linker_layout=self.layout))
        # memcpy .init:0x80004000, HA = 0x8000 / LO = 0x4000
        decomp_ha = _fn(b"\x3c\x80\x00\x00", [(2, R_PPC_ADDR16_HA, "memcpy", 0)])
        retail_ha = _fn(b"\x3c\x80\x80\x00", [])
        self.assertTrue(_byte_identical_with_relocs(
            decomp_ha, retail_ha, linker_layout=self.layout))

    def test_presence_asymmetry_for_non_addr16_rejected(self):
        left = _fn(b"\x48\x00\x00\x01", [(0, R_PPC_REL24, "some_callee", 0)])
        right = _fn(b"\x48\x00\x00\x01", [])
        self.assertFalse(_byte_identical_with_relocs(
            left, right, linker_layout=self.layout))

    def test_unrelated_tu_local_addr16_site_does_not_block(self):
        # a TU-local @N ADDR16 reloc (unresolvable without the unit reloc map)
        # present on both sides at an identical-byte offset must not stop the
        # difference-driven comparison
        left = _fn(b"\x3c\x20\x00\x00\x80\x00\x00\x00",
                   [(2, R_PPC_ADDR16_HI, "_stack_addr", 0),
                    (6, R_PPC_ADDR16_LO, "@1234", 0)])
        right = _fn(b"\x3c\x20\x80\x67\x80\x00\x00\x00",
                    [(6, R_PPC_ADDR16_LO, "@1234", 0)])
        self.assertTrue(_byte_identical_with_relocs(
            left, right, linker_layout=self.layout))

    def test_unverifiable_addr16_presence_asymmetry_rejected(self):
        # a reloc present on one side only cannot be verified -> fail closed
        left = _fn(b"\x3c\x20\x00\x00\x80\x00\x00\x00",
                   [(2, R_PPC_ADDR16_HI, "_stack_addr", 0),
                    (6, R_PPC_ADDR16_LO, "@1234", 0)])
        right = _fn(b"\x3c\x20\x80\x67\x80\x00\x00\x00", [])
        self.assertFalse(_byte_identical_with_relocs(
            left, right, linker_layout=self.layout))

    def test_address_equal_names_accepted(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            _write_synthetic_layout(root, sbss2_start=0x8066B540, sbss2_end=0x8066B55C)
            (root / "config" / "xx" / "symbols.txt").write_text(
                "memcpy = .text:0x80004000; // type:function size:0x29C scope:global\n"
                "memcpy_alias = .text:0x80004000; // type:function size:0x29C scope:global\n"
            )
            layout = LinkerLayout.load(root, "xx")
            left = _fn(b"\x80\x00\x40\x00", [(0, 1, "memcpy", 0)])
            right = _fn(b"\x80\x00\x40\x00", [(0, 1, "memcpy_alias", 0)])
            self.assertTrue(_byte_identical_with_relocs(
                left, right, linker_layout=layout))
            distant = _fn(b"\x80\x00\x40\x00", [(0, 1, "not_a_symbol_xyz", 0)])
            self.assertFalse(_byte_identical_with_relocs(
                left, distant, linker_layout=layout))


if __name__ == "__main__":
    unittest.main()

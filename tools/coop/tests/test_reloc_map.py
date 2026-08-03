from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.reloc_map import (
    MAX_MAP_ADDEND_DELTA,
    RelocDrift,
    _is_section_symbol,
    _is_tu_local,
    classify_drift,
    inplace_addend,
    is_unit_scoped_key,
    load_map,
    map_key_for,
    mine,
    reloc_kind,
    save_map,
    suggestions,
)


def _drift(**kw) -> RelocDrift:
    base = {
        "offset": 0,
        "reloc_type": 109,  # R_PPC_EMB_SDA21
        "retail_symbol": "lbl_eu_806656A0",
        "decomp_symbol": "spInstance__9CDeviceGX",
        "kind": "name",
        "retail_addend": 0,
        "decomp_addend": 0,
    }
    base.update(kw)
    return RelocDrift(**base)


class InplaceAddendTests(unittest.TestCase):
    def test_sda21_low16(self):
        # lwz r3, 0x12(r0) — addend field is the low 16 bits.
        word = 0x80030012
        self.assertEqual(inplace_addend(word, 109), 0x12)
        # addi r3, r13, -8 — low 16 bits as the displacement
        self.assertEqual(inplace_addend(0x386DFFF8, 109), 0xFFF8)

    def test_rel24_26bit(self):
        # bl +0x3FFFC → bl to a target; only LI field differs
        self.assertEqual(inplace_addend(0x48000001, 10), 0x00000000)
        self.assertEqual(inplace_addend(0x4803FFFD, 10), 0x0003FFFC)

    def test_addr16_ha(self):
        self.assertEqual(inplace_addend(0x3C600000, 6), 0)
        self.assertEqual(inplace_addend(0x3C600123, 6), 0x0123)

    def test_unknown_type(self):
        self.assertIsNone(inplace_addend(0x12345678, 999))


class ClassifyDriftTests(unittest.TestCase):
    def test_same_symbol_not_drift(self):
        self.assertIsNone(
            classify_drift(109, "lbl_eu_806656A0", "lbl_eu_806656A0", 0x80030000, 0x80030000)
        )

    def test_empty_symbol_not_drift(self):
        self.assertIsNone(classify_drift(109, "lbl_x", "", 0x80030000, 0x80030000))

    def test_name_drift_identical_words(self):
        d = classify_drift(
            109, "lbl_eu_806656A0", "spInstance__9CDeviceGX", 0x80030000, 0x80030000
        )
        self.assertIsNotNone(d)
        self.assertEqual(d.kind, "name")
        self.assertEqual(d.addend_delta, 0)

    def test_addend_drift_only_field_differs(self):
        # Same opcode + registers, addend field 0x10 vs 0x00 → addend drift.
        d = classify_drift(109, "lbl_x", "sym_y", 0x80030010, 0x80030000)
        self.assertIsNotNone(d)
        self.assertEqual(d.kind, "addend")
        self.assertEqual(d.addend_delta, -0x10)

    def test_layout_drift_same_symbol(self):
        # Same symbol on both sides but addend differs (string pool offset).
        d = classify_drift(4, "lbl_8055113C", "lbl_8055113C", 0x38630020, 0x38630000)
        self.assertIsNotNone(d)
        self.assertEqual(d.kind, "layout")

    def test_structural_other_bits_differ(self):
        # Register field differs → structural, not reloc-name fixable.
        d = classify_drift(109, "lbl_x", "sym_y", 0x80050000, 0x80030000)
        self.assertIsNotNone(d)
        self.assertEqual(d.kind, "structural")

    def test_structural_same_symbol_word_differs(self):
        self.assertIsNone(classify_drift(109, "lbl_x", "lbl_x", 0x80050000, 0x80030000))


class TuLocalTests(unittest.TestCase):
    def test_tu_local_names(self):
        self.assertTrue(_is_tu_local("@9144"))
        self.assertTrue(_is_tu_local("@LOCAL@getInstance@instance"))
        self.assertTrue(_is_tu_local("...bss.0"))
        self.assertTrue(_is_section_symbol("...data.0"))
        self.assertFalse(_is_tu_local("spInstance__9CDeviceGX"))
        self.assertFalse(_is_tu_local("__vt__9CDeviceGX"))

    def test_map_key_scoping(self):
        d = _drift(decomp_symbol="spInstance__9CDeviceGX")
        self.assertEqual(map_key_for(d, "main/unit"), "spInstance__9CDeviceGX")
        self.assertFalse(is_unit_scoped_key(map_key_for(d, "main/unit")))

        d2 = _drift(decomp_symbol="@9144")
        key = map_key_for(d2, "main/unit")
        self.assertEqual(key, "main/unit@9144")
        self.assertTrue(is_unit_scoped_key(key))


class MapIOTests(unittest.TestCase):
    def test_load_missing(self):
        data = load_map(Path("/nonexistent/reloc_map.json"))
        self.assertEqual(data["count"], 0)
        self.assertEqual(data["entries"], {})

    def test_save_load_roundtrip(self):
        tmp = Path(tempfile.mkdtemp()) / "map.json"
        data = {
            "version": 1,
            "region": "us",
            "count": 1,
            "entries": {
                "spInstance__9CDeviceGX": {
                    "R_PPC_EMB_SDA21": {
                        "retail_symbol": "lbl_eu_806656A0",
                        "retail_addend": 0,
                        "addend_delta": 0,
                        "kind": "data",
                        "count": 29,
                        "units": ["main/monolib/src/device/CDeviceGX"],
                        "alternatives": [],
                    }
                }
            },
        }
        save_map(data, tmp)
        loaded = load_map(tmp)
        self.assertEqual(loaded, data)

    def test_wrong_version_rejected(self):
        tmp = Path(tempfile.mkdtemp()) / "map.json"
        tmp.write_text(json.dumps({"version": 99, "entries": {"x": {}}}))
        loaded = load_map(tmp)
        self.assertEqual(loaded["count"], 0)


class SuggestionTests(unittest.TestCase):
    def test_name_drift_suggests_rename(self):
        d = _drift(kind="name", retail_addend=0, decomp_addend=0)
        lines = suggestions(d, "main/monolib/src/device/CDeviceGX", "CDeviceGX", {})
        self.assertTrue(any("lbl_eu_806656A0" in line for line in lines))
        self.assertTrue(any("extern \"C\"" in line for line in lines))
        # Approved fix is source-level; postprocess patching is deprecated.
        self.assertTrue(any("PLAN.md §17.6" in line for line in lines))
        self.assertFalse(any("exact_renames" in line for line in lines))

    def test_section_symbol_suggestion(self):
        d = _drift(decomp_symbol="...bss.0", retail_symbol="NandInfo")
        lines = suggestions(d, "main/unit", "dvderror", {})
        self.assertTrue(any("section-symbol" in line for line in lines))

    def test_no_suggestion_for_structural(self):
        d = _drift(kind="structural")
        self.assertEqual(suggestions(d, "u", "o", {}), [])

    def test_map_lookup_confirms(self):
        reloc_map = {
            "entries": {
                "spInstance__9CDeviceGX": {
                    "R_PPC_EMB_SDA21": {
                        "retail_symbol": "lbl_eu_806656A0",
                        "retail_addend": 0,
                        "addend_delta": 0,
                        "kind": "data",
                        "count": 29,
                        "units": ["main/monolib/src/device/CDeviceGX"],
                        "alternatives": [],
                    }
                }
            }
        }
        d = _drift(kind="name")
        lines = suggestions(d, "main/monolib/src/device/CDeviceGX", "CDeviceGX", reloc_map)
        self.assertTrue(any("29×" in line for line in lines))


class RelocKindTests(unittest.TestCase):
    def test_kinds(self):
        self.assertEqual(reloc_kind(10), "call")   # REL24
        self.assertEqual(reloc_kind(7), "branch")  # ADDR14
        self.assertEqual(reloc_kind(109), "data")  # EMB_SDA21
        self.assertEqual(reloc_kind(6), "data")    # ADDR16_HA
        self.assertEqual(reloc_kind(1), "data")    # ADDR32


class MineAggregationTests(unittest.TestCase):
    def test_large_addend_delta_filtered(self):
        # Entries beyond MAX_MAP_ADDEND_DELTA are misalignment noise.
        self.assertGreater(MAX_MAP_ADDEND_DELTA, 0)
        self.assertLess(MAX_MAP_ADDEND_DELTA, 0x1000)

    def test_mine_requires_function_pairs(self):
        # mine() needs real objects; just assert the guard against missing files.
        class FakeUnit:
            def __init__(self, name):
                self.name = name
                self.target_path = None
                self.base_path = None

        class FakeProject:
            def __init__(self):
                self.config = type("C", (), {"region": "us"})()

            def load_objdiff_units(self):
                return [FakeUnit("main/missing"), FakeUnit("main/missing2")]

        data = mine(FakeProject(), include_kinds={"data"})
        self.assertEqual(data["units_scanned"], 0)
        self.assertEqual(data["count"], 0)


class EnsureFreshTests(unittest.TestCase):
    """doc 33 Item 0.5: reloc-map freshness precondition."""

    def _make_unit(self, name, target_mtime, base_mtime):
        import tempfile

        d = tempfile.mkdtemp()
        t = Path(d) / "target.o"
        b = Path(d) / "base.o"
        t.write_bytes(b"")
        b.write_bytes(b"")
        ts = 1_000_000_000.0
        import os

        os.utime(t, (ts, target_mtime))
        os.utime(b, (ts, base_mtime))
        return type("U", (), {"name": name, "target_path": t, "base_path": b})(), d

    def test_fresh_map_is_noop(self) -> None:
        from datetime import datetime, timezone

        from tools.coop.reloc_map import ensure_fresh, save_map

        unit, d = self._make_unit("main/u", 1_000_000_000.0, 1_000_000_000.0)
        proj = type("P", (), {
            "config": type("C", (), {"region": "us"})(),
            "load_objdiff_units": lambda self: [unit],
        })()
        out = Path(d) / "map.json"
        future = datetime(2030, 1, 1, tzinfo=timezone.utc).strftime("%Y-%m-%dT%H:%M:%S")
        save_map({"version": 1, "generated": future, "entries": {}}, out)
        self.assertFalse(ensure_fresh(proj, out=out))  # fresh → no re-mine

    def test_stale_unit_remines(self) -> None:
        from datetime import datetime, timezone

        from tools.coop.reloc_map import ensure_fresh, save_map

        unit, d = self._make_unit("main/u", 2_000_000_000.0, 2_000_000_000.0)
        proj = type("P", (), {
            "config": type("C", (), {"region": "us"})(),
            "load_objdiff_units": lambda self: [unit],
        })()
        out = Path(d) / "map.json"
        past = datetime(2020, 1, 1, tzinfo=timezone.utc).strftime("%Y-%m-%dT%H:%M:%S")
        save_map({"version": 1, "generated": past, "entries": {}}, out)
        self.assertTrue(ensure_fresh(proj, out=out))  # .o newer → re-mine
        refreshed = load_map(out)
        self.assertNotEqual(refreshed.get("generated"), past)

    def test_missing_map_remines(self) -> None:
        import tempfile

        from tools.coop.reloc_map import ensure_fresh

        unit, d = self._make_unit("main/u", 1_000_000_000.0, 1_000_000_000.0)
        proj = type("P", (), {
            "config": type("C", (), {"region": "us"})(),
            "load_objdiff_units": lambda self: [unit],
        })()
        out = Path(d) / "map.json"
        self.assertTrue(ensure_fresh(proj, out=out))
        self.assertTrue(out.is_file())

    def test_force_remines(self) -> None:
        from datetime import datetime, timezone

        from tools.coop.reloc_map import ensure_fresh, save_map

        unit, d = self._make_unit("main/u", 1_000_000_000.0, 1_000_000_000.0)
        proj = type("P", (), {
            "config": type("C", (), {"region": "us"})(),
            "load_objdiff_units": lambda self: [unit],
        })()
        out = Path(d) / "map.json"
        future = datetime(2030, 1, 1, tzinfo=timezone.utc).strftime("%Y-%m-%dT%H:%M:%S")
        save_map({"version": 1, "generated": future, "entries": {}}, out)
        self.assertTrue(ensure_fresh(proj, force=True, out=out))


if __name__ == "__main__":
    unittest.main()

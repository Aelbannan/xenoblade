from __future__ import annotations

import hashlib
import unittest

from tools.ppc_equivalence.address_space import (
    AddressSpace,
    Region,
    RegionKind,
    rom_image_region,
)


class TestRomImageRegion(unittest.TestCase):
    def test_classify_inside(self):
        image = b"\x01\x02\x03\x04"
        region = rom_image_region(0x80200000, image, label="jump-table")
        space = AddressSpace((region,))

        self.assertIs(space.classify(0x80200000), region)
        self.assertIs(space.classify(0x80200003), region)
        self.assertEqual(region.kind, RegionKind.ROM_IMAGE)
        self.assertEqual(region.image_bytes, image)
        self.assertEqual(region.image_sha256, hashlib.sha256(image).hexdigest())
        self.assertEqual(region.label, "jump-table")

    def test_classify_outside(self):
        region = rom_image_region(0x80200000, b"\x00\x01")
        space = AddressSpace((region,))

        self.assertIsNone(space.classify(0x801FFFFF))
        self.assertIsNone(space.classify(0x80200002))


class TestAddressSpaceValidation(unittest.TestCase):
    def test_overlap_rejected(self):
        left = Region(0x1000, 0x2000, RegionKind.RAM)
        right = Region(0x1800, 0x2800, RegionKind.RAM)
        with self.assertRaisesRegex(ValueError, "overlapping regions"):
            AddressSpace((left, right))

    def test_adjacent_regions_allowed(self):
        left = Region(0x1000, 0x17FF, RegionKind.RAM)
        right = Region(0x1800, 0x2000, RegionKind.RAM)
        space = AddressSpace((right, left))
        self.assertIs(space.classify(0x1000), left)
        self.assertIs(space.classify(0x1800), right)


class TestClassifyRange(unittest.TestCase):
    def test_single_region(self):
        ram = Region(0x80000000, 0x80001000, RegionKind.RAM)
        space = AddressSpace((ram,))
        result = space.classify_range(0x80000010, 4)
        self.assertIs(result.region, ram)
        self.assertFalse(result.spans_multiple_regions)

    def test_multi_region_span_detected(self):
        ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
        rom = rom_image_region(0x80200000, b"\x00" * 16)
        space = AddressSpace((ram, rom))

        result = space.classify_range(0x801FFFFE, 4)
        self.assertIsNone(result.region)
        self.assertTrue(result.spans_multiple_regions)
        self.assertEqual(result.regions, (ram, rom))

    def test_unmapped_range(self):
        space = AddressSpace(())
        result = space.classify_range(0x90000000, 4)
        self.assertIsNone(result.region)
        self.assertFalse(result.spans_multiple_regions)


class TestEmptyAddressSpace(unittest.TestCase):
    def test_classify_returns_none(self):
        space = AddressSpace(())
        self.assertIsNone(space.classify(0x80000000))

    def test_classify_range_unmapped(self):
        space = AddressSpace(())
        result = space.classify_range(0x80000000, 1)
        self.assertIsNone(result.region)
        self.assertFalse(result.spans_multiple_regions)


if __name__ == "__main__":
    unittest.main()

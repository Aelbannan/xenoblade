"""Config + cache-identity knobs for AbiShape inference and MEM1/MMIO guards."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.config import (
    CoopConfig,
    abi_shape_inference_enabled,
    load_config,
    object_base_mem1_enabled,
    ram_only_when_no_mmio_enabled,
)
from tools.coop.lib.equivalence_check import _cache_key
from tools.ppc_equivalence.abi_shape import AbiShape


class AbiMem1ConfigTests(unittest.TestCase):
    def test_defaults_are_off(self) -> None:
        cfg = CoopConfig(project_root=Path("."))
        self.assertFalse(object_base_mem1_enabled(cfg))
        self.assertFalse(ram_only_when_no_mmio_enabled(cfg))
        self.assertFalse(abi_shape_inference_enabled(cfg))

    def test_load_config_parses_flags(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            path = root / "coop.json"
            path.write_text(
                "{\n"
                '  "region": "us",\n'
                '  "object_base_mem1": true,\n'
                '  "ram_only_when_no_mmio": true,\n'
                '  "abi_shape_inference": true\n'
                "}\n",
                encoding="utf-8",
            )
            cfg = load_config(path, root)
            self.assertTrue(cfg.object_base_mem1)
            self.assertTrue(cfg.ram_only_when_no_mmio)
            self.assertTrue(cfg.abi_shape_inference)

    def test_cache_key_changes_with_abi_shape_and_mem1(self) -> None:
        base = dict(
            contract_name="auto",
            observables=("r3", "r4", "memory"),
            original_hex="4e800020",
            candidate_hex="4e800020",
            original_base=0,
            candidate_base=0,
        )
        k0 = _cache_key(**base)
        k1 = _cache_key(
            **base,
            abi_shape=AbiShape(outgoing_gpr_args=1, outgoing_fpr_args=0, source="symbol:Fv").to_dict(),
        )
        k2 = _cache_key(
            **base,
            initial_gpr_ranges={"3": [0x80000000, 0x817FFFFF]},
        )
        k3 = _cache_key(**base, ram_only_projection=True)
        self.assertNotEqual(k0, k1)
        self.assertNotEqual(k0, k2)
        self.assertNotEqual(k0, k3)
        self.assertNotEqual(k1, k2)


if __name__ == "__main__":
    unittest.main()

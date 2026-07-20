"""Phase 1 scalar-FP contract v2 loader and completeness tests."""

from __future__ import annotations

import os
import unittest

from tools.ppc_equivalence.fp_bitwise import FP_BITWISE_OPS
from tools.ppc_equivalence.fp_capabilities import (
    FP_COMPARE_MODEL_VERSION,
    FP_COMPARE_OPS,
    FP_CONVERT_MODEL_VERSION,
    FP_CONVERT_OPS,
    FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS,
    FP_FUSED_ARITH_OPS,
    FP_FUSED_MODEL_VERSION,
    FP_LOAD_STORE_MODEL_VERSION,
    FP_LOAD_STORE_OPS,
    FP_SCALAR_ARITH_OPS,
    FP_SCALAR_MODEL_VERSION,
    FP_TRAPS_MODEL_VERSION,
    experimental_model_version_for_capability,
    model_version_for_capability,
    scalar_fp_exact_v2_enabled,
    set_scalar_fp_exact_v2_module_flag,
)
from tools.ppc_equivalence.scalar_fp_contract_v2 import (
    FP_FPSCR_CONTROL_CAPABILITY,
    FP_FPSCR_CONTROL_OPS,
    VALID_SCALAR_FP_CAPABILITY_OWNERS,
    load_scalar_fp_contract_v2,
    required_contract_opcodes,
    validate_scalar_fp_contract_v2,
)


class ScalarFPContractV2Tests(unittest.TestCase):
    def setUp(self) -> None:
        load_scalar_fp_contract_v2.cache_clear()
        set_scalar_fp_exact_v2_module_flag(None)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)
        load_scalar_fp_contract_v2.cache_clear()

    def test_loads_yaml_and_covers_all_scalar_opcode_sets(self) -> None:
        contract = load_scalar_fp_contract_v2()
        required = required_contract_opcodes()
        self.assertEqual(
            required,
            FP_LOAD_STORE_OPS
            | FP_COMPARE_OPS
            | FP_CONVERT_OPS
            | FP_SCALAR_ARITH_OPS
            | FP_FUSED_ARITH_OPS
            | FP_BITWISE_OPS,
        )
        self.assertEqual(frozenset(contract.opcodes), required)
        self.assertEqual(contract.schema_version, 1)
        self.assertEqual(contract.contract_id, "scalar-fp-contract-v2")

    def test_validate_reports_no_errors(self) -> None:
        self.assertEqual(validate_scalar_fp_contract_v2(), [])

    def test_capability_owners_are_valid(self) -> None:
        contract = load_scalar_fp_contract_v2()
        for opcode, row in contract.opcodes.items():
            with self.subTest(opcode=opcode):
                self.assertIn(row.capability_owner, VALID_SCALAR_FP_CAPABILITY_OWNERS)

    def test_fpscr_control_ops_use_proposed_capability(self) -> None:
        contract = load_scalar_fp_contract_v2()
        cap = contract.capabilities[FP_FPSCR_CONTROL_CAPABILITY]
        self.assertEqual(cap["migration_from"], "fp-scalar-arithmetic")
        self.assertEqual(frozenset(cap["opcodes"]), FP_FPSCR_CONTROL_OPS)
        for opcode in FP_FPSCR_CONTROL_OPS:
            self.assertEqual(
                contract.row_for(opcode).capability_owner,
                FP_FPSCR_CONTROL_CAPABILITY,
            )

    def test_experimental_model_versions_differ_from_production(self) -> None:
        production_pairs = {
            "fp-load-store": FP_LOAD_STORE_MODEL_VERSION,
            "fp-compare": FP_COMPARE_MODEL_VERSION,
            "fp-convert": FP_CONVERT_MODEL_VERSION,
            "fp-scalar-arithmetic": FP_SCALAR_MODEL_VERSION,
            "fp-fused-arithmetic": FP_FUSED_MODEL_VERSION,
            "fp-traps": FP_TRAPS_MODEL_VERSION,
        }
        for capability, production in production_pairs.items():
            experimental = FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS[capability]
            with self.subTest(capability=capability):
                self.assertNotEqual(experimental, production)

        self.assertEqual(
            FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS[FP_FPSCR_CONTROL_CAPABILITY],
            "broadway-fp-fpscr-control-v1",
        )

    def test_scalar_fp_exact_v2_disabled_by_default(self) -> None:
        env = os.environ.pop("SCALAR_FP_EXACT_V2", None)
        try:
            self.assertFalse(scalar_fp_exact_v2_enabled())
            self.assertEqual(
                experimental_model_version_for_capability("fp-load-store"),
                FP_LOAD_STORE_MODEL_VERSION,
            )
        finally:
            if env is not None:
                os.environ["SCALAR_FP_EXACT_V2"] = env

    def test_scalar_fp_exact_v2_module_flag_enables_experimental_models(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        self.assertTrue(scalar_fp_exact_v2_enabled())
        self.assertEqual(
            experimental_model_version_for_capability("fp-scalar-arithmetic"),
            FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS["fp-scalar-arithmetic"],
        )
        self.assertNotEqual(
            experimental_model_version_for_capability("fp-scalar-arithmetic"),
            model_version_for_capability("fp-scalar-arithmetic"),
        )

    def test_production_model_versions_unchanged(self) -> None:
        self.assertEqual(FP_LOAD_STORE_MODEL_VERSION, "broadway-fp-load-store-v1")
        self.assertEqual(FP_COMPARE_MODEL_VERSION, "broadway-fp-compare-v1")
        self.assertEqual(FP_CONVERT_MODEL_VERSION, "broadway-fp-convert-v1")
        self.assertEqual(FP_SCALAR_MODEL_VERSION, "broadway-fp-scalar-v2")
        self.assertEqual(FP_FUSED_MODEL_VERSION, "broadway-fp-fused-v1")
        self.assertEqual(FP_TRAPS_MODEL_VERSION, "broadway-fp-traps-v1")


if __name__ == "__main__":
    unittest.main()

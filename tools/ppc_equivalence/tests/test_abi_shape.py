"""AbiShape: exit-kind-aware narrowing of r4/f1 observation."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.abi_infer import infer_abi_shape
from tools.ppc_equivalence.abi_shape import AbiShape
from tools.ppc_equivalence.contract import (
    make_contract,
    observables_for_exit,
    refine_auto_contract_with_writes,
    with_abi_shape,
)
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import automatic_live_out


_RETAIL_R12 = "81830000 818c0020 7d8903a6 4e800420"
_CAND_R4 = "80830000 80840020 7c8903a6 4e800420"
_CAND_R5 = "80a30000 80a50020 7ca903a6 4e800420"


def _decode(hex_words: str):
    return decode_block(parse_hex(hex_words), validate_with_capstone=False)


def _prove(original_hex: str, candidate_hex: str, *, abi_shape: AbiShape | None = None):
    original = _decode(original_hex)
    candidate = _decode(candidate_hex)
    contract = make_contract(
        preset="auto",
        observe=None,
        timeout_ms=5_000,
        original_live_out=automatic_live_out(original),
        candidate_live_out=automatic_live_out(candidate),
        abi_shape=abi_shape,
    )
    return check_equivalence(
        original,
        candidate,
        contract,
        original_hex=original_hex.replace(" ", ""),
        candidate_hex=candidate_hex.replace(" ", ""),
    )


class AbiShapeUnitTests(unittest.TestCase):
    def test_conservative_defaults(self) -> None:
        shape = AbiShape.conservative()
        self.assertTrue(shape.returns_i64)
        self.assertTrue(shape.returns_float)
        self.assertEqual(shape.outgoing_gpr_args, 8)
        self.assertEqual(shape.outgoing_fpr_args, 8)
        self.assertEqual(shape.source, "default-conservative")

    def test_from_dict_roundtrip(self) -> None:
        shape = AbiShape(
            returns_i64=False,
            outgoing_gpr_args=1,
            outgoing_fpr_args=0,
            source="test",
        )
        restored = AbiShape.from_dict(shape.to_dict())
        self.assertEqual(restored, shape)

    def test_outgoing_args_range_validated(self) -> None:
        with self.assertRaises(ValueError):
            AbiShape(outgoing_gpr_args=9)
        with self.assertRaises(ValueError):
            AbiShape(outgoing_fpr_args=-1)


class ObservablesForExitAbiShapeTests(unittest.TestCase):
    def test_none_abi_shape_keeps_r4_and_f1(self) -> None:
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=1_000)
        self.assertIsNone(contract.abi_shape)
        for kind in ("return", "indirect-branch", "call-indirect", "fallthrough"):
            names = {item.name for item in observables_for_exit(contract, kind)}
            self.assertIn("r4", names)
            self.assertIn("f1", names)
            self.assertIn("f1.ps1", names)
        # Fail-closed: indirect exits also observe r5–r10 / f2–f8.
        ind = {
            item.name
            for item in observables_for_exit(contract, "indirect-branch")
        }
        self.assertIn("r5", ind)
        self.assertIn("r10", ind)
        self.assertIn("f2", ind)
        self.assertIn("f8", ind)
        ret = {item.name for item in observables_for_exit(contract, "return")}
        self.assertNotIn("r5", ret)
        self.assertNotIn("f2", ret)

    def test_returns_i64_false_drops_r4_on_return_only(self) -> None:
        shape = AbiShape(returns_i64=False, source="test")
        contract = make_contract(
            preset="ppc-eabi", observe=None, timeout_ms=1_000, abi_shape=shape,
        )
        ret = {item.name for item in observables_for_exit(contract, "return")}
        fall = {item.name for item in observables_for_exit(contract, "fallthrough")}
        ind = {item.name for item in observables_for_exit(contract, "indirect-branch")}
        self.assertNotIn("r4", ret)
        self.assertNotIn("r4", fall)
        self.assertIn("r4", ind)
        self.assertIn("f1", ret)
        self.assertIn("r3", ret)

    def test_outgoing_gpr_args_1_drops_r4_on_indirect_only(self) -> None:
        shape = AbiShape(outgoing_gpr_args=1, outgoing_fpr_args=0, source="test")
        contract = make_contract(
            preset="ppc-eabi", observe=None, timeout_ms=1_000, abi_shape=shape,
        )
        ind = {item.name for item in observables_for_exit(contract, "indirect-branch")}
        call = {item.name for item in observables_for_exit(contract, "call-indirect")}
        ret = {item.name for item in observables_for_exit(contract, "return")}
        self.assertNotIn("r4", ind)
        self.assertNotIn("f1", ind)
        self.assertNotIn("f1.ps1", ind)
        self.assertNotIn("r4", call)
        self.assertIn("r4", ret)
        self.assertIn("f1", ret)
        self.assertIn("r3", ind)

    def test_refine_auto_preserves_abi_shape(self) -> None:
        shape = AbiShape(outgoing_gpr_args=1, source="test")
        contract = make_contract(
            preset="auto",
            observe=None,
            timeout_ms=1_000,
            original_live_out=("r3",),
            candidate_live_out=("r3",),
            abi_shape=shape,
        )
        refined = refine_auto_contract_with_writes(
            contract, symbolic_writes={"fpscr"},
        )
        self.assertEqual(refined.abi_shape, shape)
        self.assertIn("fpscr", refined.auto_added)

    def test_with_abi_shape_helper(self) -> None:
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=1_000)
        shape = AbiShape(returns_i64=False)
        attached = with_abi_shape(contract, shape)
        self.assertEqual(attached.abi_shape, shape)
        self.assertIsNone(contract.abi_shape)

    def test_resolution_dict_includes_abi_shape(self) -> None:
        shape = AbiShape(outgoing_gpr_args=1, source="test")
        contract = make_contract(
            preset="auto",
            observe=None,
            timeout_ms=1_000,
            original_live_out=("r3",),
            candidate_live_out=("r3",),
            abi_shape=shape,
        )
        resolution = contract.resolution_dict()
        assert resolution is not None
        self.assertEqual(resolution["abi_shape"], shape.to_dict())


class AbiShapeProofTests(unittest.TestCase):
    def test_conservative_r12_vs_r4_not_equivalent(self) -> None:
        result = _prove(_RETAIL_R12, _CAND_R4, abi_shape=AbiShape.conservative())
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r4")

    def test_none_abi_shape_r12_vs_r4_not_equivalent(self) -> None:
        result = _prove(_RETAIL_R12, _CAND_R4, abi_shape=None)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r4")

    def test_outgoing_gpr_1_makes_r12_vs_r4_equivalent(self) -> None:
        shape = AbiShape(outgoing_gpr_args=1, outgoing_fpr_args=0, source="explicit")
        result = _prove(_RETAIL_R12, _CAND_R4, abi_shape=shape)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)

    def test_r12_vs_r5_still_equivalent_under_narrow(self) -> None:
        shape = AbiShape(outgoing_gpr_args=1, outgoing_fpr_args=0, source="explicit")
        result = _prove(_RETAIL_R12, _CAND_R5, abi_shape=shape)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)


class AbiInferTests(unittest.TestCase):
    def test_disabled_returns_conservative(self) -> None:
        original = _decode(_RETAIL_R12)
        candidate = _decode(_CAND_R5)
        shape = infer_abi_shape(original, candidate, enabled=False)
        self.assertEqual(shape, AbiShape.conservative())

    def test_r12_vs_r4_without_symbol_stays_conservative(self) -> None:
        """Candidate touches r4 as scratch — do not auto-narrow."""
        original = _decode(_RETAIL_R12)
        candidate = _decode(_CAND_R4)
        shape = infer_abi_shape(original, candidate)
        self.assertEqual(shape.outgoing_gpr_args, 8)
        self.assertEqual(shape.outgoing_fpr_args, 8)
        self.assertEqual(shape.source, "default-conservative")

    def test_r12_vs_r4_with_fv_symbol_narrows(self) -> None:
        original = _decode(_RETAIL_R12)
        candidate = _decode(_CAND_R4)
        shape = infer_abi_shape(
            original, candidate, symbol="foo__Q2_3cf4CBarFv",
        )
        self.assertEqual(shape.outgoing_gpr_args, 1)
        self.assertEqual(shape.outgoing_fpr_args, 0)
        self.assertIn("symbol:Fv", shape.source)

    def test_r12_vs_r5_structural_does_not_narrow(self) -> None:
        """Using r5 as CTR scratch clobbers a live outgoing arg — stay fail-closed."""
        original = _decode(_RETAIL_R12)
        candidate = _decode(_CAND_R5)
        shape = infer_abi_shape(original, candidate)
        self.assertEqual(shape.outgoing_gpr_args, 8)
        self.assertEqual(shape.outgoing_fpr_args, 8)
        self.assertEqual(shape.source, "default-conservative")

    def test_matching_r12_thunks_structural_narrow(self) -> None:
        """Identical r12-only thunks may narrow to a single outgoing GPR arg."""
        original = _decode(_RETAIL_R12)
        candidate = _decode(_RETAIL_R12)
        shape = infer_abi_shape(original, candidate)
        self.assertEqual(shape.outgoing_gpr_args, 1)
        self.assertEqual(shape.outgoing_fpr_args, 0)
        self.assertIn("simple-vtable-dispatch", shape.source)

    def test_return_without_r4_write_sets_returns_i64_false(self) -> None:
        # li r3,1; blr
        body = _decode("38600001 4e800020")
        shape = infer_abi_shape(body, body)
        self.assertFalse(shape.returns_i64)
        self.assertIn("no-r4-write-return", shape.source)

    def test_return_with_r4_write_keeps_returns_i64(self) -> None:
        # li r4,1; blr
        body = _decode("38800001 4e800020")
        shape = infer_abi_shape(body, body)
        self.assertTrue(shape.returns_i64)


if __name__ == "__main__":
    unittest.main()

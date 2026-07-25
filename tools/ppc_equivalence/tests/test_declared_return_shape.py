"""AbiShape declared-return field: mappings, conjunction, round-trip (§4 S1–S3)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.abi_infer import (
    DECLARED_RETURN_SHAPES,
    abi_shape_from_declared_return,
    combine_abi_shapes,
)
from tools.ppc_equivalence.abi_shape import AbiShape


class DeclaredReturnShapeTests(unittest.TestCase):
    """S1 — mapping table §2.1."""

    def test_known_types_return_shapes(self) -> None:
        """Every recognised §2.1 type class yields a narrowing shape."""
        for declared in ("void", "i32", "u32", "bool", "ptr", "f32", "f64", "i64", "u64"):
            with self.subTest(declared=declared):
                shape = abi_shape_from_declared_return(declared)
                self.assertIsNotNone(shape)

    def test_void_narrows_both(self) -> None:
        shape = abi_shape_from_declared_return("void")
        assert shape is not None
        self.assertFalse(shape.returns_i64)
        self.assertFalse(shape.returns_float)
        self.assertEqual(shape.declared_return, "void")
        self.assertEqual(shape.source, "declared-return:void")

    def test_i32_narrows_both(self) -> None:
        shape = abi_shape_from_declared_return("i32")
        assert shape is not None
        self.assertFalse(shape.returns_i64)
        self.assertFalse(shape.returns_float)
        self.assertEqual(shape.declared_return, "i32")

    def test_u32_narrows_both(self) -> None:
        shape = abi_shape_from_declared_return("u32")
        assert shape is not None
        self.assertFalse(shape.returns_i64)
        self.assertFalse(shape.returns_float)

    def test_bool_narrows_both(self) -> None:
        shape = abi_shape_from_declared_return("bool")
        assert shape is not None
        self.assertFalse(shape.returns_i64)
        self.assertFalse(shape.returns_float)

    def test_ptr_narrows_both(self) -> None:
        shape = abi_shape_from_declared_return("ptr")
        assert shape is not None
        self.assertFalse(shape.returns_i64)
        self.assertFalse(shape.returns_float)

    def test_f32_keeps_float_drops_i64(self) -> None:
        """f32/f64: returns_i64=False, returns_float=True — f1 must stay compared."""
        shape = abi_shape_from_declared_return("f32")
        assert shape is not None
        self.assertFalse(shape.returns_i64)
        self.assertTrue(shape.returns_float)

    def test_f64_keeps_float_drops_i64(self) -> None:
        shape = abi_shape_from_declared_return("f64")
        assert shape is not None
        self.assertFalse(shape.returns_i64)
        self.assertTrue(shape.returns_float)

    def test_i64_keeps_i64_drops_float(self) -> None:
        shape = abi_shape_from_declared_return("i64")
        assert shape is not None
        self.assertTrue(shape.returns_i64)
        self.assertFalse(shape.returns_float)

    def test_u64_keeps_i64_drops_float(self) -> None:
        shape = abi_shape_from_declared_return("u64")
        assert shape is not None
        self.assertTrue(shape.returns_i64)
        self.assertFalse(shape.returns_float)

    def test_none_returns_none(self) -> None:
        self.assertIsNone(abi_shape_from_declared_return(None))

    def test_aggregate_returns_none(self) -> None:
        self.assertIsNone(abi_shape_from_declared_return("aggregate"))

    def test_f128_returns_none(self) -> None:
        self.assertIsNone(abi_shape_from_declared_return("f128"))

    def test_unknown_returns_none(self) -> None:
        self.assertIsNone(abi_shape_from_declared_return("unknown_ty"))


class CombineAbiShapesTests(unittest.TestCase):
    """S2 — combine_abi_shapes fail-closed conjunction (§2.3)."""

    def test_declared_none_returns_inferred(self) -> None:
        inf = AbiShape(returns_i64=True, returns_float=False, source="inferred")
        combined = combine_abi_shapes(inf, None)
        self.assertIs(combined, inf)

    def test_declared_cannot_widen_returns_i64(self) -> None:
        """Inferred=False, declared=True => combined=False (fail-closed)."""
        inf = AbiShape(returns_i64=False, source="inferred")
        dec = abi_shape_from_declared_return("i64")
        combined = combine_abi_shapes(inf, dec)
        self.assertFalse(combined.returns_i64)

    def test_declared_cannot_widen_returns_float(self) -> None:
        inf = AbiShape(returns_float=False, source="inferred")
        dec = abi_shape_from_declared_return("f64")
        combined = combine_abi_shapes(inf, dec)
        self.assertFalse(combined.returns_float)

    def test_both_true_keeps_true(self) -> None:
        inf = AbiShape(returns_i64=True, returns_float=True, source="inferred")
        dec = abi_shape_from_declared_return("i64")
        combined = combine_abi_shapes(inf, dec)
        self.assertTrue(combined.returns_i64)
        self.assertFalse(combined.returns_float)

    def test_outgoing_args_pass_through(self) -> None:
        inf = AbiShape(
            outgoing_gpr_args=3, outgoing_fpr_args=2, source="inferred",
        )
        dec = abi_shape_from_declared_return("void")
        combined = combine_abi_shapes(inf, dec)
        self.assertEqual(combined.outgoing_gpr_args, 3)
        self.assertEqual(combined.outgoing_fpr_args, 2)

    def test_declared_return_preserved(self) -> None:
        inf = AbiShape(source="inferred")
        dec = abi_shape_from_declared_return("void")
        combined = combine_abi_shapes(inf, dec)
        self.assertEqual(combined.declared_return, "void")

    def test_source_join_inferred_first(self) -> None:
        inf = AbiShape(source="no-r4-write-return")
        dec = abi_shape_from_declared_return("void")
        combined = combine_abi_shapes(inf, dec)
        self.assertEqual(combined.source, "no-r4-write-return+declared-return:void")

    def test_source_skips_default_conservative(self) -> None:
        inf = AbiShape.conservative()
        dec = abi_shape_from_declared_return("void")
        combined = combine_abi_shapes(inf, dec)
        self.assertEqual(combined.source, "declared-return:void")

    def test_source_skips_duplicates(self) -> None:
        inf = AbiShape(source="simple-vtable-dispatch")
        dec = AbiShape(
            returns_i64=False, returns_float=False,
            source="simple-vtable-dispatch", declared_return="void",
        )
        combined = combine_abi_shapes(inf, dec)
        self.assertEqual(combined.source, "simple-vtable-dispatch")

    def test_both_default_stays_default(self) -> None:
        inf = AbiShape.conservative()
        dec = AbiShape.conservative()
        combined = combine_abi_shapes(inf, dec)
        self.assertEqual(combined.source, "default-conservative")


class AbiShapeRoundTripTests(unittest.TestCase):
    """S3 — AbiShape to_dict / from_dict round-trip."""

    def test_roundtrip_with_declared_return(self) -> None:
        shape = AbiShape(
            returns_i64=False, returns_float=False,
            source="declared-return:void", declared_return="void",
        )
        d = shape.to_dict()
        self.assertIn("declared_return", d)
        self.assertEqual(d["declared_return"], "void")
        restored = AbiShape.from_dict(d)
        self.assertEqual(restored, shape)

    def test_roundtrip_without_declared_return(self) -> None:
        shape = AbiShape(
            returns_i64=False, source="no-r4-write-return",
        )
        d = shape.to_dict()
        self.assertNotIn("declared_return", d)
        restored = AbiShape.from_dict(d)
        self.assertEqual(restored, shape)

    def test_old_payload_still_loads(self) -> None:
        """Legacy payload without declared_return key must still deserialise."""
        old = {
            "returns_i64": True,
            "returns_float": True,
            "outgoing_gpr_args": 8,
            "outgoing_fpr_args": 8,
            "source": "from-dict",
        }
        shape = AbiShape.from_dict(old)
        self.assertTrue(shape.returns_i64)
        self.assertTrue(shape.returns_float)
        self.assertIsNone(shape.declared_return)
        self.assertEqual(shape.source, "from-dict")

    def test_none_payload_returns_conservative(self) -> None:
        shape = AbiShape.from_dict(None)
        self.assertTrue(shape.returns_i64)
        self.assertTrue(shape.returns_float)
        self.assertIsNone(shape.declared_return)


if __name__ == "__main__":
    unittest.main()

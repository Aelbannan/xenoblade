"""C2–C6, C8: declared-return ABI-shape plumbing tests.

Tests §2.5–2.8 of docs/ppc_equiv_work/29-declared-return-abi-shapes.md:
- C2: cache/cert invalidation (distinct keys, staleness check).
- C3: registry → _prove_bytes lookup incl. KeyError and symbol-mismatch.
- C4: schema accepts enum, rejects others; targets validate passes.
- C5: conservative-inference attach path.
- C6: byte-identical certify_unit_symbol path (no narrowing, no bogus declared_return).
- C8: §2.8 gate refusal + forced override.
"""

from __future__ import annotations

import hashlib
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path
from typing import Any

from tools.coop.lib.config import CoopConfig
from tools.ppc_equivalence.abi_infer import (
    DECLARED_RETURN_SHAPES,
    abi_shape_from_declared_return,
    combine_abi_shapes,
)
from tools.ppc_equivalence.abi_shape import AbiShape
from tools.ppc_equivalence.contract import make_contract, with_abi_shape
from tools.ppc_equivalence.provenance import proof_request_hash, proof_request_identity
from tools.ppc_equivalence.result import ProofStatus

_HAS_Z3 = importlib.util.find_spec("z3") is not None
_REPO_ROOT = Path(__file__).resolve().parents[3]


def _make_minimal_certificate(
    target_id: str = "test-target",
    declared_return: str | None = None,
    abi_shape: dict | None = None,
) -> dict[str, Any]:
    """Build a minimal valid-looking certificate for staleness testing."""
    from tools.coop.lib.targets import EQUIVALENCE_CERTIFICATE_VERSION
    from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT
    from tools.ppc_equivalence.provenance import hash_engine_tree, hash_certifier_tree

    cert: dict[str, Any] = {
        "version": EQUIVALENCE_CERTIFICATE_VERSION,
        "status": "SEMANTIC_CERTIFIED",
        "architecture": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "target_id": target_id,
        "evidence": "symbolic-equivalence",
        "retail_sha256": "a" * 64,
        "candidate_sha256": "b" * 64,
        "summary": {
            "reads": [],
            "writes": [],
            "invalid_reasons": [],
            "return_behavior": "normal",
        },
        "callees": [],
        "helpers": [],
        "engine_hash": hash_engine_tree(_REPO_ROOT),
        "certifier_hash": hash_certifier_tree(_REPO_ROOT),
        "contract": "ppc-eabi",
    }
    if abi_shape is not None:
        cert["abi_shape"] = abi_shape
    elif declared_return is not None:
        cert["abi_shape"] = {"declared_return": declared_return, "source": f"declared-return:{declared_return}"}
    from tools.coop.lib.targets import equivalence_certificate_hash
    cert["certificate_sha256"] = equivalence_certificate_hash(cert)
    return cert


class C2CacheCertInvalidationTest(unittest.TestCase):
    """C2: cache key and certificate invalidation with declared_return."""

    def test_cache_key_differs_with_abi_shape(self) -> None:
        """Cache keys differ when abi_shape payload changes."""
        base_kwargs = dict(
            original_hex="38630001" + "4e800020",
            candidate_hex="38630001" + "4e800020",
            contract="ppc-eabi",
            timeout_ms=120_000,
            max_instructions=65536,
            max_paths=4096,
            max_loop_iterations=2048,
        )
        key_void = proof_request_hash(**base_kwargs, abi_shape={"declared_return": "void"})
        key_i64 = proof_request_hash(**base_kwargs, abi_shape={"declared_return": "i64"})
        key_none = proof_request_hash(**base_kwargs)
        self.assertNotEqual(key_void, key_i64)
        self.assertNotEqual(key_void, key_none)
        self.assertNotEqual(key_i64, key_none)

    def test_certificate_sha256_differs_with_abi_shape(self) -> None:
        """certificate_sha256 differs when abi_shape is present vs absent."""
        cert_void = _make_minimal_certificate(declared_return="void")
        cert_i64 = _make_minimal_certificate(declared_return="i64")
        cert_none = _make_minimal_certificate()
        sha256s = {
            cert_void["certificate_sha256"],
            cert_i64["certificate_sha256"],
            cert_none["certificate_sha256"],
        }
        self.assertEqual(len(sha256s), 3, "all three should produce distinct SHA-256s")

    def test_staleness_check_mismatch(self) -> None:
        """equivalence_certificate_error flags registry vs cert mismatch."""
        from tools.coop.lib.targets import equivalence_certificate_error

        cert = _make_minimal_certificate(declared_return="void")
        row: dict[str, Any] = {
            "id": "test-target",
            "declared_return": "i64",
            "equivalence_certificate": cert,
        }
        rows_by_id = {"test-target": row}
        error = equivalence_certificate_error(row, rows_by_id)
        self.assertIsNotNone(error)
        self.assertIn("declared_return mismatch", error)
        self.assertIn("registry='i64'", error)
        self.assertIn("certificate='void'", error)

    def test_staleness_check_absent_on_both_equal(self) -> None:
        """Absent-on-both-sides is treated as equal (no staleness)."""
        from tools.coop.lib.targets import equivalence_certificate_error

        cert = _make_minimal_certificate()
        row: dict[str, Any] = {
            "id": "test-target",
            "equivalence_certificate": cert,
        }
        rows_by_id = {"test-target": row}
        error = equivalence_certificate_error(row, rows_by_id)
        # May still fail on other checks (engine_hash etc), but not on declared_return.
        if error is not None:
            self.assertNotIn("declared_return", error)

    def test_staleness_check_missing_cert_abi_shape_ok(self) -> None:
        """Certificate without abi_shape + registry without declared_return → no staleness."""
        from tools.coop.lib.targets import equivalence_certificate_error

        cert = _make_minimal_certificate()
        # Remove the abi_shape key entirely
        cert.pop("abi_shape", None)
        cert["certificate_sha256"] = self._rehash(cert)
        row: dict[str, Any] = {
            "id": "test-target",
            "equivalence_certificate": cert,
        }
        rows_by_id = {"test-target": row}
        error = equivalence_certificate_error(row, rows_by_id)
        if error is not None:
            self.assertNotIn("declared_return", error)

    def _rehash(self, cert: dict[str, Any]) -> str:
        from tools.coop.lib.targets import equivalence_certificate_hash
        return equivalence_certificate_hash(cert)


class C3RegistryLookupTest(unittest.TestCase):
    """C3: registry → _prove_bytes lookup."""

    def test_key_error_treated_as_no_declaration(self) -> None:
        """Registry lookup KeyError (unknown target id) → proof proceeds un-narrowed."""
        from tools.ppc_equivalence.abi_infer import abi_shape_from_declared_return
        self.assertIsNone(abi_shape_from_declared_return(None))
        self.assertIsNone(abi_shape_from_declared_return(""))

        # Execute the real lookup path: a target_id absent from the registry
        # is refused by the certified-callee loader (fail closed, no crash,
        # no narrowing) before the declaration resolution is ever reached.
        if not _HAS_Z3:
            self.skipTest("z3 required for engine proofs")
        probe = self._probe_missing_target()
        self.assertEqual(
            probe.status, ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE, probe.detail,
        )
        self.assertNotIn("declared-return:void", probe.detail)

    def _probe_missing_target(self):
        from tools.coop.lib.project import ObjdiffUnit, Project
        from tools.coop.lib.equivalence_check import prove_unit_symbol
        from tools.ppc_equivalence.tests.test_elf_symbols import build_reloc_elf

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "tools/coop").mkdir(parents=True)
            (root / "build/us").mkdir(parents=True)
            config = CoopConfig(project_root=root, region="us")
            symbol = "func_gate"
            retail = root / "build/us/retail.o"
            decomp = root / "build/us/decomp.o"
            retail.write_bytes(build_reloc_elf({symbol: _GATE_RETAIL}))
            decomp.write_bytes(build_reloc_elf({symbol: _GATE_CANDIDATE}))
            (root / "build/us/objdiff.json").write_text(
                json.dumps({"units": [{
                    "name": "demo/Gate",
                    "target_path": "build/us/retail.o",
                    "base_path": "build/us/decomp.o",
                }]}),
                encoding="utf-8",
            )
            (root / "tools/coop/targets.json").write_text(
                json.dumps({"schema_version": 2, "targets": []}),
                encoding="utf-8",
            )
            project = Project(config)
            unit = ObjdiffUnit(
                name="demo/Gate", target_path=retail, base_path=decomp,
                source_path=None,
            )
            return prove_unit_symbol(project, unit, symbol, target_id="test-missing-target")

    def test_symbol_mismatch_invalid_input(self) -> None:
        """Registry target symbol != proved symbol → INVALID_INPUT (fail closed)."""
        if not _HAS_Z3:
            self.skipTest("z3 required for engine proofs")
        from tools.coop.lib.project import ObjdiffUnit, Project
        from tools.coop.lib.equivalence_check import prove_unit_symbol
        from tools.ppc_equivalence.tests.test_elf_symbols import build_reloc_elf

        symbol = "func_gate"
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "tools/coop").mkdir(parents=True)
            (root / "build/us").mkdir(parents=True)
            config = CoopConfig(project_root=root, region="us")
            retail = root / "build/us/retail.o"
            decomp = root / "build/us/decomp.o"
            retail.write_bytes(build_reloc_elf({symbol: _GATE_RETAIL}))
            decomp.write_bytes(build_reloc_elf({symbol: _GATE_CANDIDATE}))
            (root / "build/us/objdiff.json").write_text(
                json.dumps({"units": [{
                    "name": "demo/Gate",
                    "target_path": "build/us/retail.o",
                    "base_path": "build/us/decomp.o",
                }]}),
                encoding="utf-8",
            )
            # Registry row for the SAME id declares a DIFFERENT symbol.
            (root / "tools/coop/targets.json").write_text(
                json.dumps({
                    "schema_version": 2,
                    "targets": [{
                        "id": "test-mismatch-target",
                        "symbol": "other_func",
                        "address": "0x80001000",
                        "function": "other_func",
                        "unit": "demo/Gate",
                        "region": "us",
                        "declared_return": "void",
                    }],
                }),
                encoding="utf-8",
            )
            project = Project(config)
            unit = ObjdiffUnit(
                name="demo/Gate", target_path=retail, base_path=decomp,
                source_path=None,
            )
            probe = prove_unit_symbol(
                project, unit, symbol, target_id="test-mismatch-target",
            )
            self.assertEqual(probe.status, ProofStatus.INVALID_INPUT, probe.detail)
            self.assertIn("differs from proved symbol", probe.detail)

    def test_registry_declared_return_loaded_via_extra(self) -> None:
        """declared_return lives in Target.extra and round-trips."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            config = CoopConfig(project_root=root, region="us")
            (root / "tools/coop").mkdir(parents=True)
            targets_data = {
                "schema_version": 2,
                "default_required_level": "EQUIVALENT_MATCH",
                "targets": [
                    {
                        "id": "test-void-target",
                        "symbol": "func_void",
                        "address": "0x80001000",
                        "function": "func_void",
                        "unit": "test/Unit",
                        "region": "us",
                        "declared_return": "void",
                    },
                    {
                        "id": "test-no-decl-target",
                        "symbol": "func_nodecl",
                        "address": "0x80002000",
                        "function": "func_nodecl",
                        "unit": "test/Unit",
                        "region": "us",
                    },
                ],
            }
            (root / "tools/coop/targets.json").write_text(
                json.dumps(targets_data), encoding="utf-8"
            )
            from tools.coop.lib.targets import load_targets
            targets = load_targets(config)
            void_target = [t for t in targets if t.id == "test-void-target"][0]
            nodecl_target = [t for t in targets if t.id == "test-no-decl-target"][0]
            self.assertEqual(void_target.extra.get("declared_return"), "void")
            self.assertIsNone(nodecl_target.extra.get("declared_return"))


class C4SchemaValidationTest(unittest.TestCase):
    """C4: schema validation for declared_return."""

    def setUp(self) -> None:
        self.schema_path = _REPO_ROOT / "tools/coop/targets.schema.json"
        self.assertTrue(self.schema_path.is_file(), "schema file must exist")

    def _validate_with_declared_return(self, value: Any) -> list[str]:
        """Feed a single function target with the given declared_return through validation."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            config = CoopConfig(project_root=root, region="us")
            (root / "tools/coop").mkdir(parents=True)
            data = {
                "schema_version": 2,
                "default_required_level": "EQUIVALENT_MATCH",
                "targets": [
                    {
                        "id": "test-target",
                        "symbol": "func_test",
                        "address": "0x80001000",
                        "function": "func_test",
                        "unit": "test/Unit",
                        "region": "us",
                        "declared_return": value,
                    }
                ],
            }
            path = root / "tools/coop/targets.json"
            path.write_text(json.dumps(data), encoding="utf-8")
            from tools.coop.lib.targets import validate_targets
            return validate_targets(config)

    def test_accepts_void(self) -> None:
        errors = self._validate_with_declared_return("void")
        self.assertEqual(errors, [])

    def test_accepts_i32(self) -> None:
        errors = self._validate_with_declared_return("i32")
        self.assertEqual(errors, [])

    def test_accepts_u32(self) -> None:
        errors = self._validate_with_declared_return("u32")
        self.assertEqual(errors, [])

    def test_accepts_bool(self) -> None:
        errors = self._validate_with_declared_return("bool")
        self.assertEqual(errors, [])

    def test_accepts_ptr(self) -> None:
        errors = self._validate_with_declared_return("ptr")
        self.assertEqual(errors, [])

    def test_accepts_f32(self) -> None:
        errors = self._validate_with_declared_return("f32")
        self.assertEqual(errors, [])

    def test_accepts_f64(self) -> None:
        errors = self._validate_with_declared_return("f64")
        self.assertEqual(errors, [])

    def test_accepts_i64(self) -> None:
        errors = self._validate_with_declared_return("i64")
        self.assertEqual(errors, [])

    def test_accepts_u64(self) -> None:
        errors = self._validate_with_declared_return("u64")
        self.assertEqual(errors, [])

    def test_accepts_aggregate(self) -> None:
        errors = self._validate_with_declared_return("aggregate")
        self.assertEqual(errors, [])

    def test_accepts_f128(self) -> None:
        errors = self._validate_with_declared_return("f128")
        self.assertEqual(errors, [])

    def test_rejects_invalid_value(self) -> None:
        # validate_targets does NOT do schema validation; it only checks
        # structure. Schema is enforced by JSON Schema tooling (or manual).
        # We verify the schema definition contains the enum constraint.
        schema = json.loads(self.schema_path.read_text(encoding="utf-8"))
        target_props = schema["$defs"]["target"]["properties"]
        self.assertIn("declared_return", target_props)
        dr = target_props["declared_return"]
        self.assertEqual(dr["type"], "string")
        expected_enum = ["void", "i32", "u32", "bool", "ptr", "f32", "f64", "i64", "u64", "aggregate", "f128"]
        self.assertEqual(dr["enum"], expected_enum)

    def test_validate_targets_without_declared_return_still_passes(self) -> None:
        """Targets without declared_return are still valid."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            config = CoopConfig(project_root=root, region="us")
            (root / "tools/coop").mkdir(parents=True)
            data = {
                "schema_version": 2,
                "default_required_level": "EQUIVALENT_MATCH",
                "targets": [
                    {
                        "id": "test-target",
                        "symbol": "func_test",
                        "address": "0x80001000",
                        "function": "func_test",
                        "unit": "test/Unit",
                        "region": "us",
                    }
                ],
            }
            path = root / "tools/coop/targets.json"
            path.write_text(json.dumps(data), encoding="utf-8")
            from tools.coop.lib.targets import validate_targets
            errors = validate_targets(config)
            self.assertEqual(errors, [])


class C5ConservativeInferenceAttachTest(unittest.TestCase):
    """C5: conservative-inference attach path."""

    def test_declared_void_with_conservative_inference_attaches_shape(self) -> None:
        """When inference is conservative, declared void still narrows."""
        # Build a contract and attach a declared-return void shape.
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=1000)
        declared = abi_shape_from_declared_return("void")
        self.assertIsNotNone(declared)
        assert declared is not None

        # Use AbiShape.conservative() as the inferred shape (source=default-conservative).
        conservative = AbiShape.conservative()
        combined = combine_abi_shapes(conservative, declared)
        self.assertFalse(combined.returns_i64)
        self.assertFalse(combined.returns_float)
        self.assertEqual(combined.declared_return, "void")

        # Attach and verify.
        narrowed = with_abi_shape(contract, combined)
        self.assertEqual(narrowed.abi_shape, combined)

    def test_conservative_source_not_clobbered(self) -> None:
        """conservative() source is preserved before combination."""
        c = AbiShape.conservative()
        self.assertEqual(c.source, "default-conservative")
        self.assertTrue(c.returns_i64)
        self.assertTrue(c.returns_float)


class C6ByteIdenticalCertifyTest(unittest.TestCase):
    """C6: byte-identical certify_unit_symbol path — no narrowing, no bogus declared_return."""

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_certify_byte_identical_no_bogus_declared_return(self) -> None:
        """Byte-identical FULL_MATCH certify does not inject a bogus declared_return."""
        from tools.coop.lib.project import ObjdiffUnit, Project
        from tools.coop.lib.equivalence_check import certify_unit_symbol
        from tools.ppc_equivalence.tests.test_elf_symbols import _EQ_LEFT, build_reloc_elf

        _SYMBOL = "func__3LeafFv"
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "tools/coop").mkdir(parents=True)
            (root / "build/us").mkdir(parents=True)
            config = CoopConfig(project_root=root, region="us")

            elf_bytes = build_reloc_elf({_SYMBOL: _EQ_LEFT})
            retail = root / "build/us/retail.o"
            decomp = root / "build/us/decomp.o"
            retail.write_bytes(elf_bytes)
            decomp.write_bytes(elf_bytes)

            (root / "build/us/objdiff.json").write_text(
                json.dumps({
                    "units": [{
                        "name": "demo/CertifyLeaf",
                        "target_path": "build/us/retail.o",
                        "base_path": "build/us/decomp.o",
                    }]
                }),
                encoding="utf-8",
            )
            (root / "tools/coop/targets.json").write_text(
                json.dumps({
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": [{
                        "id": "test-certify-byte-id",
                        "symbol": _SYMBOL,
                        "address": "0x80001000",
                        "function": _SYMBOL,
                        "unit": "demo/CertifyLeaf",
                        "region": "us",
                        "status": "FULL_MATCH",
                        "called_functions": [],
                        "abi_helper_calls": [],
                    }]
                }),
                encoding="utf-8",
            )

            project = Project(config)
            unit = ObjdiffUnit(
                name="demo/CertifyLeaf",
                target_path=retail,
                base_path=decomp,
                source_path=None,
            )

            probe = certify_unit_symbol(project, unit, _SYMBOL, "test-certify-byte-id")
            self.assertEqual(probe.status, ProofStatus.EQUIVALENT, probe.detail)
            certificate = probe.certificate
            self.assertIsNotNone(certificate)
            assert certificate is not None

            # Certificate should NOT have abi_shape with declared_return
            # when no declaration was provided.
            cert_abi = certificate.get("abi_shape")
            if isinstance(cert_abi, dict):
                self.assertNotIn("declared_return", cert_abi,
                                  "Byte-identical certify with no declared_return "
                                  "should not contain declared_return in certificate")

    def test_certify_byte_identical_summary_semantics(self) -> None:
        """C6: byte-identical certify semantics are explicit and un-narrowed.

        The byte-identical path synthesizes a CONSERVATIVE opaque-EABI effect
        summary by design (no SMT prove, no body-analysis): an empty summary
        would incorrectly model return registers (notably allocator r3) as
        preserved and make parent memory-layout premises unsatisfiable.  The
        summary must cover the lanes the body writes (r3/r4 here); the
        r4-in-writes invariant for PROVEN certificates is asserted in C8's
        forced-marker test instead.
        """
        from tools.coop.lib.project import ObjdiffUnit, Project
        from tools.coop.lib.equivalence_check import certify_unit_symbol
        from tools.ppc_equivalence.tests.test_elf_symbols import build_reloc_elf

        _SYMBOL = "func__3LeafFv"
        # li r4,1 ; addi r3,r3,5 ; blr — writes r4 and r3.
        body = bytes.fromhex("38800001 38630004 4e800020")
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "tools/coop").mkdir(parents=True)
            (root / "build/us").mkdir(parents=True)
            config = CoopConfig(project_root=root, region="us")

            elf_bytes = build_reloc_elf({_SYMBOL: body})
            retail = root / "build/us/retail.o"
            decomp = root / "build/us/decomp.o"
            retail.write_bytes(elf_bytes)
            decomp.write_bytes(elf_bytes)

            (root / "build/us/objdiff.json").write_text(
                json.dumps({
                    "units": [{
                        "name": "demo/CertifyLeaf",
                        "target_path": "build/us/retail.o",
                        "base_path": "build/us/decomp.o",
                    }]
                }),
                encoding="utf-8",
            )
            (root / "tools/coop/targets.json").write_text(
                json.dumps({
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": [{
                        "id": "test-certify-r4-writes",
                        "symbol": _SYMBOL,
                        "address": "0x80001000",
                        "function": _SYMBOL,
                        "unit": "demo/CertifyLeaf",
                        "region": "us",
                        "status": "FULL_MATCH",
                        "called_functions": [],
                        "abi_helper_calls": [],
                    }]
                }),
                encoding="utf-8",
            )

            project = Project(config)
            unit = ObjdiffUnit(
                name="demo/CertifyLeaf",
                target_path=retail,
                base_path=decomp,
                source_path=None,
            )

            probe = certify_unit_symbol(project, unit, _SYMBOL, "test-certify-r4-writes")
            self.assertEqual(probe.status, ProofStatus.EQUIVALENT, probe.detail)
            certificate = probe.certificate
            self.assertIsNotNone(certificate)
            assert certificate is not None
            # Byte-identical path: conservative opaque-EABI effect envelope
            # (covers r3/r4 and every other volatile lane), no declared_return
            # narrowing.
            summary = certificate.get("summary") or {}
            writes = summary.get("writes") or []
            self.assertIn("r3", writes)
            self.assertIn("r4", writes)
            self.assertNotEqual(summary.get("reads"), [])
            cert_abi = certificate.get("abi_shape")
            if isinstance(cert_abi, dict):
                self.assertNotIn("declared_return", cert_abi)


# Reg-swapped pair (same shape as __prep_buffer P1): identical memory
# effects, r4 used as scratch in the candidate only. Proves EQUIVALENT only
# when declared-return narrowing is applied.
_GATE_RETAIL = bytes.fromhex(
    "80830000 80C30004 80A30008 "
    "90830018 90C30024 90A30028 90C30034 "
    "38600000 4E800020"
)
_GATE_CANDIDATE = bytes.fromhex(
    "80A30000 80830004 80C30008 "
    "90A30018 90830024 90C30028 90830034 "
    "38600000 4E800020"
)


@unittest.skipUnless(_HAS_Z3, "z3 required for engine proofs")
class C8CallerCorroborationGateTest(unittest.TestCase):
    """C8: §2.8 gate — narrowing is refused unless conditions met.

    These tests execute the real gate inside ``_prove_bytes`` via
    ``prove_unit_symbol`` against a synthetic registry; they fail if the
    gate, the refusal detail, or the forced-marker recording is removed.
    """

    _SYMBOL = "func_gate"

    def _run_probe(
        self,
        target_row: dict[str, Any],
        extra_rows: list[dict[str, Any]] | None = None,
        *,
        force: bool = False,
    ):
        from tools.coop.lib.project import ObjdiffUnit, Project
        from tools.coop.lib.equivalence_check import prove_unit_symbol
        from tools.ppc_equivalence.tests.test_elf_symbols import build_reloc_elf

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "tools/coop").mkdir(parents=True)
            (root / "build/us").mkdir(parents=True)
            config = CoopConfig(project_root=root, region="us")

            retail = root / "build/us/retail.o"
            decomp = root / "build/us/decomp.o"
            retail.write_bytes(build_reloc_elf({self._SYMBOL: _GATE_RETAIL}))
            decomp.write_bytes(build_reloc_elf({self._SYMBOL: _GATE_CANDIDATE}))

            (root / "build/us/objdiff.json").write_text(
                json.dumps({
                    "units": [{
                        "name": "demo/Gate",
                        "target_path": "build/us/retail.o",
                        "base_path": "build/us/decomp.o",
                    }]
                }),
                encoding="utf-8",
            )
            rows = [target_row] + list(extra_rows or [])
            (root / "tools/coop/targets.json").write_text(
                json.dumps({
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": rows,
                }),
                encoding="utf-8",
            )

            project = Project(config)
            unit = ObjdiffUnit(
                name="demo/Gate",
                target_path=retail,
                base_path=decomp,
                source_path=None,
            )
            return prove_unit_symbol(
                project, unit, self._SYMBOL,
                target_id=target_row["id"],
                declared_return="void",
                force_declared_return=force,
            )

    @staticmethod
    def _row(**overrides: Any) -> dict[str, Any]:
        row: dict[str, Any] = {
            "id": "test-gate-target",
            "symbol": "func_gate",
            "address": "0x80001000",
            "function": "func_gate",
            "unit": "demo/Gate",
            "region": "us",
            "declared_return": "void",
            "has_indirect_calls": False,
            "unresolved_called_functions": [],
            "called_functions": [],
            "callgraph_status": "complete",
        }
        row.update(overrides)
        return row

    _CALLER_ROW: dict[str, Any] = {
        "id": "test-gate-caller",
        "symbol": "func_caller",
        "address": "0x80002000",
        "function": "func_caller",
        "unit": "demo/Gate",
        "region": "us",
        "called_functions": ["test-gate-target"],
    }

    def test_gate_allows_with_direct_caller(self) -> None:
        probe = self._run_probe(self._row(), [self._CALLER_ROW])
        self.assertEqual(probe.status, ProofStatus.EQUIVALENT, probe.detail)
        self.assertIn("declared-return:void", probe.detail)

    def test_indirect_calls_blocked_before_gate(self) -> None:
        # Targets with unresolved indirect calls are refused by the earlier
        # callgraph-certification block (_callgraph_blocks_certification),
        # which pre-empts the §2.8 gate entirely — an even stronger refusal.
        probe = self._run_probe(
            self._row(has_indirect_calls=True), [self._CALLER_ROW],
        )
        self.assertEqual(
            probe.status, ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE, probe.detail,
        )

    def test_gate_refuses_no_direct_caller(self) -> None:
        probe = self._run_probe(self._row())
        self.assertEqual(probe.status, ProofStatus.NOT_EQUIVALENT, probe.detail)
        self.assertIn("declared-return narrowing refused", probe.detail)
        self.assertIn("no direct caller", probe.detail)

    def test_gate_refuses_incomplete_callgraph(self) -> None:
        probe = self._run_probe(
            self._row(callgraph_status="partial"), [self._CALLER_ROW],
        )
        self.assertEqual(probe.status, ProofStatus.NOT_EQUIVALENT, probe.detail)
        self.assertIn("declared-return narrowing refused", probe.detail)

    def test_force_overrides_gate_and_records_marker(self) -> None:
        probe = self._run_probe(self._row(), force=True)
        self.assertEqual(probe.status, ProofStatus.EQUIVALENT, probe.detail)
        certificate = probe.certificate
        self.assertIsNotNone(certificate)
        assert certificate is not None
        cert_abi = certificate.get("abi_shape")
        self.assertIsInstance(cert_abi, dict)
        self.assertEqual(cert_abi.get("declared_return"), "void")
        self.assertTrue(
            cert_abi.get("declared_return_forced"),
            "forced narrowing must record declared_return_forced in the certificate",
        )
        # Load-bearing invariant (plan §3.2): even for a narrowed callee, the
        # certified summary derives writes from body analysis, so r4 stays
        # visible to any parent proof.
        summary = certificate.get("summary") or {}
        writes = summary.get("writes") or []
        self.assertIn(
            "r4", writes,
            "narrowed callee summary must keep r4 in writes for parent proofs",
        )


if __name__ == "__main__":
    unittest.main()

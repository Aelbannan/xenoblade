"""Scalar FP architectural contract v2 (Phase 1 — SCALAR_FP_V2.md).

Loads ``scalar_fp_contract_v2.yaml``: one row per non-paired, non-PSQ Broadway
scalar FP opcode. Production proofs remain on v1/v2 model identities until Phase
12; enable ``SCALAR_FP_EXACT_V2=1`` (or the module flag) only for experimental
work against the v2 contract.
"""

from __future__ import annotations

from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path
from typing import Any, Mapping

import yaml

from tools.ppc_equivalence.fp_bitwise import FP_BITWISE_OPS
from tools.ppc_equivalence.fp_capabilities import (
    FP_COMPARE_OPS,
    FP_CONVERT_OPS,
    FP_FUSED_ARITH_OPS,
    FP_LOAD_STORE_OPS,
    FP_SCALAR_ARITH_OPS,
    SCALAR_FP_EXACT_V2_ENV,
    scalar_fp_exact_v2_enabled,
    set_scalar_fp_exact_v2_module_flag,
)

CONTRACT_V2_FILENAME = "scalar_fp_contract_v2.yaml"
CONTRACT_V2_PATH = Path(__file__).resolve().parent / CONTRACT_V2_FILENAME

# Proposed v2 capability owners (Phase 1 contract; production attachment unchanged).
FP_FPSCR_CONTROL_CAPABILITY = "fp-fpscr-control"

FP_FPSCR_CONTROL_OPS: frozenset[str] = frozenset(
    {
        "mffs",
        "mtfsf",
        "mtfsfi",
        "mtfsb0",
        "mtfsb1",
        "mcrfs",
    }
)

VALID_SCALAR_FP_CAPABILITY_OWNERS: frozenset[str] = frozenset(
    {
        "fp-bitwise",
        "fp-load-store",
        "fp-compare",
        "fp-convert",
        "fp-scalar-arithmetic",
        "fp-fused-arithmetic",
        "fp-fpscr-control",
    }
)


def required_contract_opcodes() -> frozenset[str]:
    """Opcode names that must appear in the Phase 1 contract table."""
    return (
        FP_LOAD_STORE_OPS
        | FP_COMPARE_OPS
        | FP_CONVERT_OPS
        | FP_SCALAR_ARITH_OPS
        | FP_FUSED_ARITH_OPS
        | FP_BITWISE_OPS
    )


@dataclass(frozen=True)
class ScalarFPContractRow:
    opcode: str
    capability_owner: str
    experimental_model_version: str
    operand_precision_order: str
    result_precision_fpr: str
    rn_sensitive: bool | str
    ni_operand: str
    ni_result: str
    nan_selection_quieting: str
    signed_zero: str
    invalid_subcause: str
    ox_ux_zx_xx: str
    fi_fr_fprf: str
    destination_suppression: str
    record_form_cr1: str
    fe0_fe1_delivery: str
    corpus_coverage_status: str

    @classmethod
    def from_mapping(cls, opcode: str, row: Mapping[str, Any]) -> ScalarFPContractRow:
        return cls(
            opcode=opcode,
            capability_owner=str(row["capability_owner"]),
            experimental_model_version=str(row["experimental_model_version"]),
            operand_precision_order=str(row["operand_precision_order"]),
            result_precision_fpr=str(row["result_precision_fpr"]),
            rn_sensitive=row["rn_sensitive"],
            ni_operand=str(row["ni_operand"]),
            ni_result=str(row["ni_result"]),
            nan_selection_quieting=str(row["nan_selection_quieting"]),
            signed_zero=str(row["signed_zero"]),
            invalid_subcause=str(row["invalid_subcause"]),
            ox_ux_zx_xx=str(row["ox_ux_zx_xx"]),
            fi_fr_fprf=str(row["fi_fr_fprf"]),
            destination_suppression=str(row["destination_suppression"]),
            record_form_cr1=str(row["record_form_cr1"]),
            fe0_fe1_delivery=str(row["fe0_fe1_delivery"]),
            corpus_coverage_status=str(row["corpus_coverage_status"]),
        )


@dataclass(frozen=True)
class ScalarFPContractV2:
    schema_version: int
    contract_id: str
    capabilities: Mapping[str, Any]
    opcodes: dict[str, ScalarFPContractRow]

    def row_for(self, opcode: str) -> ScalarFPContractRow:
        return self.opcodes[opcode]


@lru_cache(maxsize=1)
def load_scalar_fp_contract_v2(
    path: Path | None = None,
) -> ScalarFPContractV2:
    """Load and parse the Phase 1 scalar-FP contract YAML."""
    yaml_path = path or CONTRACT_V2_PATH
    raw = yaml.safe_load(yaml_path.read_text(encoding="utf-8"))
    if not isinstance(raw, dict):
        raise ValueError(f"{yaml_path}: root must be a mapping")
    opcodes_raw = raw.get("opcodes")
    if not isinstance(opcodes_raw, dict):
        raise ValueError(f"{yaml_path}: missing opcodes mapping")

    rows: dict[str, ScalarFPContractRow] = {}
    for opcode, row in opcodes_raw.items():
        if not isinstance(row, dict):
            raise ValueError(f"{yaml_path}: opcode {opcode!r} row must be a mapping")
        rows[str(opcode)] = ScalarFPContractRow.from_mapping(str(opcode), row)

    return ScalarFPContractV2(
        schema_version=int(raw.get("schema_version", 0)),
        contract_id=str(raw.get("contract_id", "")),
        capabilities=dict(raw.get("capabilities") or {}),
        opcodes=rows,
    )


def validate_scalar_fp_contract_v2(
    contract: ScalarFPContractV2 | None = None,
) -> list[str]:
    """Return validation errors; empty list means the contract is complete."""
    contract = contract or load_scalar_fp_contract_v2()
    errors: list[str] = []

    required = required_contract_opcodes()
    present = frozenset(contract.opcodes)
    missing = sorted(required - present)
    extra = sorted(present - required)
    if missing:
        errors.append(f"missing opcode rows: {missing}")
    if extra:
        errors.append(f"unexpected opcode rows: {extra}")

    for opcode, row in sorted(contract.opcodes.items()):
        if row.capability_owner not in VALID_SCALAR_FP_CAPABILITY_OWNERS:
            errors.append(
                f"{opcode}: invalid capability_owner {row.capability_owner!r}"
            )
        if opcode in FP_FPSCR_CONTROL_OPS:
            if row.capability_owner != FP_FPSCR_CONTROL_CAPABILITY:
                errors.append(
                    f"{opcode}: FPSCR control op must use "
                    f"{FP_FPSCR_CONTROL_CAPABILITY!r}"
                )
        elif opcode in FP_BITWISE_OPS:
            if row.capability_owner != "fp-bitwise":
                errors.append(f"{opcode}: bitwise op must use 'fp-bitwise'")
        elif opcode in FP_LOAD_STORE_OPS:
            if row.capability_owner != "fp-load-store":
                errors.append(f"{opcode}: load/store op must use 'fp-load-store'")
        elif opcode in FP_COMPARE_OPS:
            if row.capability_owner != "fp-compare":
                errors.append(f"{opcode}: compare op must use 'fp-compare'")
        elif opcode in FP_CONVERT_OPS:
            if row.capability_owner != "fp-convert":
                errors.append(f"{opcode}: convert op must use 'fp-convert'")
        elif opcode in FP_FUSED_ARITH_OPS:
            if row.capability_owner != "fp-fused-arithmetic":
                errors.append(
                    f"{opcode}: fused op must use 'fp-fused-arithmetic'"
                )
        elif opcode in FP_SCALAR_ARITH_OPS:
            if row.capability_owner != "fp-scalar-arithmetic":
                errors.append(
                    f"{opcode}: scalar arith op must use 'fp-scalar-arithmetic'"
                )

    fpscr_cap = contract.capabilities.get(FP_FPSCR_CONTROL_CAPABILITY)
    if not isinstance(fpscr_cap, dict):
        errors.append(f"missing capabilities.{FP_FPSCR_CONTROL_CAPABILITY} section")
    else:
        migrated = fpscr_cap.get("migration_from")
        if migrated != "fp-scalar-arithmetic":
            errors.append(
                f"capabilities.{FP_FPSCR_CONTROL_CAPABILITY}.migration_from "
                f"must be 'fp-scalar-arithmetic'"
            )

    return errors


__all__ = [
    "CONTRACT_V2_PATH",
    "FP_FPSCR_CONTROL_CAPABILITY",
    "FP_FPSCR_CONTROL_OPS",
    "SCALAR_FP_EXACT_V2_ENV",
    "ScalarFPContractRow",
    "ScalarFPContractV2",
    "VALID_SCALAR_FP_CAPABILITY_OWNERS",
    "load_scalar_fp_contract_v2",
    "required_contract_opcodes",
    "scalar_fp_exact_v2_enabled",
    "set_scalar_fp_exact_v2_module_flag",
    "validate_scalar_fp_contract_v2",
]

"""FP sub-capability classification (Wave 3–4 capability assurance).

A proof demands every FP sub-capability its opcodes (and trap domain) use.
Only ``fp-bitwise`` is currently allowlisted for promotion-grade; all other
FP capabilities remain empty in the Tier-A allowlist and fail closed.
Wave 4 advanced stubs (fused / paired / PSQ / traps) stay incomplete while
midpoint/sticky-residue and FE0/FE1 remain unfinished.
"""

from __future__ import annotations

import os
from typing import Any, Iterable

from tools.ppc_equivalence.fp_bitwise import FP_BITWISE_OPS, fp_opcodes_among

# Model-version stubs (Wave 3 + Phase 12 scalar FP exact v2 identities).
FP_LOAD_STORE_MODEL_VERSION = "broadway-fp-load-store-v2"
FP_COMPARE_MODEL_VERSION = "broadway-fp-compare-v2"
FP_CONVERT_MODEL_VERSION = "broadway-fp-convert-v2"
FP_SCALAR_MODEL_VERSION = "broadway-fp-scalar-v3"
FP_FUSED_MODEL_VERSION = "broadway-fp-fused-v2"
FP_PAIRED_MODEL_VERSION = "broadway-fp-paired-v1"
FP_PSQ_MODEL_VERSION = "broadway-fp-psq-v1"
FP_TRAPS_MODEL_VERSION = "broadway-fp-traps-v2"
FP_FPSCR_CONTROL_MODEL_VERSION = "broadway-fp-fpscr-control-v1"

# Aliases retained for contract / rollout code that references v2 experimental ids.
FP_LOAD_STORE_MODEL_VERSION_V2 = FP_LOAD_STORE_MODEL_VERSION
FP_COMPARE_MODEL_VERSION_V2 = FP_COMPARE_MODEL_VERSION
FP_CONVERT_MODEL_VERSION_V2 = FP_CONVERT_MODEL_VERSION
FP_SCALAR_MODEL_VERSION_V3 = FP_SCALAR_MODEL_VERSION
FP_FUSED_MODEL_VERSION_V2 = FP_FUSED_MODEL_VERSION
FP_TRAPS_MODEL_VERSION_V2 = FP_TRAPS_MODEL_VERSION
FP_FPSCR_CONTROL_MODEL_VERSION_V1 = FP_FPSCR_CONTROL_MODEL_VERSION

SCALAR_FP_EXACT_V2_ENV = "SCALAR_FP_EXACT_V2"
SCALAR_FP_EXACT_V2_PRODUCTION_ENV = "SCALAR_FP_EXACT_V2_PRODUCTION"
_SCALAR_FP_EXACT_V2_MODULE_FLAG: bool | None = None
_SCALAR_FP_EXACT_V2_PRODUCTION_MODULE_FLAG: bool | None = None

FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS: dict[str, str] = {
    "fp-load-store": FP_LOAD_STORE_MODEL_VERSION_V2,
    "fp-compare": FP_COMPARE_MODEL_VERSION_V2,
    "fp-convert": FP_CONVERT_MODEL_VERSION_V2,
    "fp-scalar-arithmetic": FP_SCALAR_MODEL_VERSION_V3,
    "fp-fused-arithmetic": FP_FUSED_MODEL_VERSION_V2,
    "fp-traps": FP_TRAPS_MODEL_VERSION_V2,
    "fp-fpscr-control": FP_FPSCR_CONTROL_MODEL_VERSION_V1,
    # fp-bitwise, fp-paired-single, fp-psq: no v2 experimental bump in Phase 1.
}

FP_SCALAR_ORACLE_ALGORITHM = "fp-scalar-oracle-v1"
FP_OUTCOME_UNIFY_ALGORITHM = "fp-outcome-unify-v1"

FP_LOAD_STORE_OPS: frozenset[str] = frozenset(
    {
        "lfs",
        "lfsu",
        "lfd",
        "lfdu",
        "stfs",
        "stfsu",
        "stfd",
        "stfdu",
        "lfsx",
        "lfsux",
        "lfdx",
        "lfdux",
        "stfsx",
        "stfsux",
        "stfdx",
        "stfdux",
        "stfiwx",
    }
)

FP_COMPARE_OPS: frozenset[str] = frozenset({"fcmpu", "fcmpo"})

FP_CONVERT_OPS: frozenset[str] = frozenset({"frsp", "fctiw", "fctiwz"})

FP_SCALAR_ARITH_OPS: frozenset[str] = frozenset(
    {
        "fadd",
        "fadds",
        "fsub",
        "fsubs",
        "fmul",
        "fmuls",
        "fdiv",
        "fdivs",
        "fsel",
        "fres",
        "frsqrte",
        # FPSCR moves / field ops demand scalar until a dedicated cap exists.
        "mffs",
        "mtfsf",
        "mtfsfi",
        "mtfsb0",
        "mtfsb1",
        "mcrfs",
    }
)

FP_FUSED_ARITH_OPS: frozenset[str] = frozenset(
    {
        "fmadd",
        "fmadds",
        "fmsub",
        "fmsubs",
        "fnmadd",
        "fnmadds",
        "fnmsub",
        "fnmsubs",
    }
)

FP_PSQ_OPS: frozenset[str] = frozenset(
    {
        "psq_l",
        "psq_lu",
        "psq_st",
        "psq_stu",
        "psq_lx",
        "psq_lux",
        "psq_stx",
        "psq_stux",
    }
)

# Any paired-single opcode outside PSQ load/store.
FP_PAIRED_SINGLE_OPS: frozenset[str] = frozenset(
    {
        "ps_div",
        "ps_sub",
        "ps_add",
        "ps_sel",
        "ps_res",
        "ps_mul",
        "ps_rsqrte",
        "ps_msub",
        "ps_madd",
        "ps_nmsub",
        "ps_nmadd",
        "ps_sum0",
        "ps_sum1",
        "ps_muls0",
        "ps_muls1",
        "ps_madds0",
        "ps_madds1",
        "ps_neg",
        "ps_mr",
        "ps_nabs",
        "ps_abs",
        "ps_cmpu0",
        "ps_cmpo0",
        "ps_cmpu1",
        "ps_cmpo1",
        "ps_merge00",
        "ps_merge01",
        "ps_merge10",
        "ps_merge11",
    }
)

# Capability → model_version stubs for manifest / drafts.
FP_SUBCAPABILITY_MODEL_VERSIONS: dict[str, str] = {
    "fp-bitwise": "fp-bitwise-v1",
    "fp-load-store": FP_LOAD_STORE_MODEL_VERSION,
    "fp-compare": FP_COMPARE_MODEL_VERSION,
    "fp-convert": FP_CONVERT_MODEL_VERSION,
    "fp-scalar-arithmetic": FP_SCALAR_MODEL_VERSION,
    "fp-fused-arithmetic": FP_FUSED_MODEL_VERSION,
    "fp-paired-single": FP_PAIRED_MODEL_VERSION,
    "fp-psq": FP_PSQ_MODEL_VERSION,
    "fp-traps": FP_TRAPS_MODEL_VERSION,
    "fp-fpscr-control": FP_FPSCR_CONTROL_MODEL_VERSION,
}

_OPCODE_TO_CAPABILITY: tuple[tuple[frozenset[str], str], ...] = (
    (FP_BITWISE_OPS, "fp-bitwise"),
    (FP_LOAD_STORE_OPS, "fp-load-store"),
    (FP_COMPARE_OPS, "fp-compare"),
    (FP_CONVERT_OPS, "fp-convert"),
    (FP_SCALAR_ARITH_OPS, "fp-scalar-arithmetic"),
    (FP_FUSED_ARITH_OPS, "fp-fused-arithmetic"),
    (FP_PAIRED_SINGLE_OPS, "fp-paired-single"),
    (FP_PSQ_OPS, "fp-psq"),
)


def classify_fp_capabilities(
    opcodes: Iterable[str],
    *,
    traps_enabled: bool = False,
) -> frozenset[str]:
    """Map used FP opcodes / trap domain to distinct capability names.

    Wave 3: each FP family demands its own capability. Unknown FP opcodes
    fail closed as ``fp-scalar-arithmetic`` so they cannot silently skip
    assurance.
    """
    fp_ops = fp_opcodes_among(opcodes)
    caps: set[str] = set()
    classified: set[str] = set()
    for op_set, capability in _OPCODE_TO_CAPABILITY:
        hit = fp_ops & op_set
        if hit:
            caps.add(capability)
            classified |= hit
    remainder = fp_ops - classified
    if remainder:
        # Fail closed: unclassified supported FP opcodes still demand a cap.
        caps.add("fp-scalar-arithmetic")
    if traps_enabled:
        caps.add("fp-traps")
    return frozenset(caps)


def set_scalar_fp_exact_v2_module_flag(enabled: bool | None) -> None:
    """Override ``SCALAR_FP_EXACT_V2`` for tests; ``None`` restores env-only."""
    global _SCALAR_FP_EXACT_V2_MODULE_FLAG
    _SCALAR_FP_EXACT_V2_MODULE_FLAG = enabled


def scalar_fp_exact_v2_module_flag_override() -> bool | None:
    """Return the test override, or ``None`` when env-only."""
    return _SCALAR_FP_EXACT_V2_MODULE_FLAG


def scalar_fp_exact_v2_enabled() -> bool:
    """True when the experimental scalar-FP v2 path is enabled (default off)."""
    if _SCALAR_FP_EXACT_V2_MODULE_FLAG is not None:
        return _SCALAR_FP_EXACT_V2_MODULE_FLAG
    raw = os.environ.get(SCALAR_FP_EXACT_V2_ENV, "0")
    return raw not in ("", "0", "false", "False", "no", "NO", "off", "OFF")


def set_scalar_fp_exact_v2_production_module_flag(enabled: bool | None) -> None:
    """Override ``SCALAR_FP_EXACT_V2_PRODUCTION`` for tests; ``None`` restores env-only."""
    global _SCALAR_FP_EXACT_V2_PRODUCTION_MODULE_FLAG
    _SCALAR_FP_EXACT_V2_PRODUCTION_MODULE_FLAG = enabled


def scalar_fp_exact_v2_production_module_flag_override() -> bool | None:
    """Return the production test override, or ``None`` when env-only."""
    return _SCALAR_FP_EXACT_V2_PRODUCTION_MODULE_FLAG


def scalar_fp_exact_v2_production_enabled() -> bool:
    """True when scalar FP exact v2 production execution is armed (default off)."""
    if _SCALAR_FP_EXACT_V2_PRODUCTION_MODULE_FLAG is not None:
        return _SCALAR_FP_EXACT_V2_PRODUCTION_MODULE_FLAG
    raw = os.environ.get(SCALAR_FP_EXACT_V2_PRODUCTION_ENV, "0")
    return raw not in ("", "0", "false", "False", "no", "NO", "off", "OFF")


def model_version_for_capability(capability: str) -> str | None:
    """Return the Wave-3 model-version stub for an FP capability, if known."""
    return FP_SUBCAPABILITY_MODEL_VERSIONS.get(capability)


def experimental_model_version_for_capability(capability: str) -> str | None:
    """Return the exact-v2 model identity when production or experimental path is on."""
    if not (scalar_fp_exact_v2_enabled() or scalar_fp_exact_v2_production_enabled()):
        return model_version_for_capability(capability)
    return FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS.get(
        capability,
        FP_SUBCAPABILITY_MODEL_VERSIONS.get(capability),
    )


def traps_enabled_from_result(result: Any) -> bool:
    """True when the proof's floating-point domain enables traps."""
    domain = getattr(result, "floating_point_domain", None)
    if domain is None:
        return False
    return bool(getattr(domain, "traps_enabled", False))

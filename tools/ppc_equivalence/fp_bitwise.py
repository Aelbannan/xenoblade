"""FP bitwise / sign-bit move subset (Wave 2 capability assurance).

Promoteable under ``fp-bitwise`` / ``fp-bitwise-v1`` only:

* ``fmr`` — FPR copy (bit-identical)
* ``fabs`` — clear sign bit
* ``fneg`` — xor sign bit
* ``fnabs`` — set sign bit

These are pure bit operations on the IEEE-754 binary64 encoding. They do not
use host floating-point arithmetic, SoftFloat, rounding, FPSCR sticky updates
(aside from optional record-form CR1 shadow of existing FPSCR), traps, or
converts. Do not claim scalar arithmetic, compare, convert, fused, paired,
load/store, or traps under this capability.
"""

from __future__ import annotations

from typing import Iterable

# Opcode *names* (``Opcode.value``), not enum members.
FP_BITWISE_OPS: frozenset[str] = frozenset(
    {
        "fmr",
        "fabs",
        "fneg",
        "fnabs",
    }
)

FP_BITWISE_MODEL_VERSION = "fp-bitwise-v1"
FP_BITWISE_ALGORITHM = "fp-bitwise-ledger-v1"


def _supported_fp_opcode_names() -> frozenset[str]:
    from tools.ppc_equivalence.ir import SUPPORTED_FP_OPCODES

    return frozenset(op.value for op in SUPPORTED_FP_OPCODES)


def fp_opcodes_among(opcodes: Iterable[str]) -> frozenset[str]:
    """Return the FP opcode names present in ``opcodes``."""
    supported = _supported_fp_opcode_names()
    return frozenset(str(op) for op in opcodes if str(op) in supported)


def non_bitwise_fp_opcodes(opcodes: Iterable[str]) -> frozenset[str]:
    """FP opcodes outside the promoteable bitwise subset."""
    return fp_opcodes_among(opcodes) - FP_BITWISE_OPS


def is_fp_bitwise_only(opcodes: Iterable[str]) -> bool:
    """True when every FP opcode used is in ``FP_BITWISE_OPS`` (and at least one)."""
    fp_ops = fp_opcodes_among(opcodes)
    return bool(fp_ops) and fp_ops <= FP_BITWISE_OPS


def classify_fp_capabilities(
    opcodes: Iterable[str],
    *,
    traps_enabled: bool = False,
) -> frozenset[str]:
    """Map used FP opcodes to capability-assurance demand names.

    Wave 3 delegates to :mod:`tools.ppc_equivalence.fp_capabilities` for the
    fine split (load/store, compare, convert, scalar, fused, paired, psq,
    traps). Bitwise-only proofs still demand only ``fp-bitwise``.
    """
    from tools.ppc_equivalence.fp_capabilities import (
        classify_fp_capabilities as _classify,
    )

    return _classify(opcodes, traps_enabled=traps_enabled)

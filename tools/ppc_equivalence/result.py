from __future__ import annotations

from dataclasses import asdict, dataclass, field
from enum import Enum
from typing import Any


class ProofStatus(str, Enum):
    EQUIVALENT = "equivalent"
    NOT_EQUIVALENT = "not_equivalent"
    INCONCLUSIVE_TIMEOUT = "inconclusive_timeout"
    INCONCLUSIVE_UNKNOWN = "inconclusive_unknown"
    INCONCLUSIVE_UNSUPPORTED = "inconclusive_unsupported"
    INVALID_INPUT = "invalid_input"
    INTERNAL_ERROR = "internal_error"


@dataclass(slots=True)
class ProofResult:
    status: ProofStatus
    architecture_model: str = "broadway-ppc32-be-v12"
    format: int = 2
    contract: str = "manual"
    observables: list[str] = field(default_factory=list)
    assumptions: list[str] = field(default_factory=lambda: [
        "32-bit big-endian user-mode integer and IEEE 754 floating-point semantics",
        "shared byte-addressed initial memory",
        "all accessed addresses are mapped ordinary RAM and naturally aligned",
        "FP invalid/divide-zero and conversion flags plus VE/ZE suppression are tracked; arithmetic OX/UX/XX and traps are not",
        "FP arithmetic requires RN=nearest-even and NI=0; finite-input overflow is excluded, modeled invalid/ZX cases are included",
        "fused-single proofs require finite operands to be exact binary32 values expanded in FPRs",
        "cache hints/order operations assume coherent ordinary RAM with no DMA or self-modifying code; dcbz_l lock state is unobserved",
        "privileged register operations are defined only in supervisor mode; segment/MSR translation effects are outside ordinary-RAM checks",
        "the 64-bit time base is stable for the duration of a bounded checked block",
        "twi/sc/rfi model synchronous exception entry/return; asynchronous interrupts are absent",
        "later-ISA fsqrt/fsqrts encodings are reserved on Broadway; VMX and atomics are unsupported",
        "division results compared only on architecturally defined inputs",
        "loops and external call continuations are not summarized",
    ])
    original_instruction_count: int = 0
    candidate_instruction_count: int = 0
    solver: dict[str, Any] = field(default_factory=dict)
    mismatch: dict[str, Any] | None = None
    counterexample: dict[str, Any] | None = None
    replay: dict[str, Any] | None = None
    unsupported: list[str] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)

    def to_dict(self) -> dict[str, Any]:
        value = asdict(self)
        value["status"] = self.status.value
        return value

from __future__ import annotations

from dataclasses import asdict, dataclass, field
from enum import Enum
from typing import Any

ARCHITECTURE_MODEL = "broadway-ppc32-be-v14"
RESULT_FORMAT = 3


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
    architecture_model: str = ARCHITECTURE_MODEL
    format: int = RESULT_FORMAT
    contract: str = "manual"
    contract_resolution: dict[str, Any] | None = None
    observables: list[str] = field(default_factory=list)
    assumptions: list[str] = field(default_factory=lambda: [
        "32-bit big-endian user-mode integer and IEEE 754 floating-point semantics",
        "shared byte-addressed initial memory",
        "all accessed addresses are mapped ordinary RAM and naturally aligned",
        "FP invalid/divide-zero and conversion flags are tracked; scalar VE/ZE suppression and Broadway paired-single unconditional writeback are modeled; arithmetic OX/UX/XX and traps are not",
        "FP arithmetic requires RN=nearest-even and NI=0; finite-input overflow is excluded, modeled invalid/ZX cases are included",
        "fused-single and paired-fused proofs require finite operands to be exact binary32 values expanded in FPRs",
        "cache hints/order operations assume coherent ordinary RAM with no DMA or self-modifying code; dcbz requires HID0.DCE and dcbz_l also requires HID2.LCE",
        "privileged register operations are defined only in supervisor mode; segment/MSR/BAT translation effects are outside ordinary-RAM checks",
        "auxiliary SPR values are tracked, but HID/L2/cache-lock/DMA/debug register side effects are outside the value-semantics model",
        "the time base, decrementer, and performance counters are stable during a bounded block except for explicit modeled writes",
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

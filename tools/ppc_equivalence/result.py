from __future__ import annotations

from dataclasses import asdict, dataclass, field
from enum import Enum
from typing import Any

from tools.ppc_equivalence.memory_profile import MemoryEnvironment

ARCHITECTURE_MODEL = "broadway-ppc32-be-v19"
RESULT_FORMAT = 8


MASKING_SEMANTICS = "per-implementation-independent-v1"


@dataclass(slots=True)
class PrivateStackInfo:
    enabled_on_all_terminal_paths: bool
    disabled_reasons: list[str] = field(default_factory=list)
    frame_relation: str = "symbolic-below-entry-sp"

    def to_dict(self) -> dict[str, Any]:
        return {
            "enabled_on_all_terminal_paths": self.enabled_on_all_terminal_paths,
            "disabled_reasons": list(self.disabled_reasons),
            "frame_relation": self.frame_relation,
        }


@dataclass(slots=True)
class MemoryScope:
    comparison: str = "touched-byte-extensional"
    masking_semantics: str = MASKING_SEMANTICS
    original: PrivateStackInfo | None = None
    candidate: PrivateStackInfo | None = None

    @classmethod
    def from_terminals(
        cls,
        original_terminals: list[Any],
        candidate_terminals: list[Any],
        ops: Any,
    ) -> MemoryScope:
        """Build a MemoryScope by inspecting terminal stack-private states."""
        def _side_info(terminals: list[Any]) -> PrivateStackInfo:
            enabled_all = True
            disabled_reasons: list[str] = []
            for term in terminals:
                sp = term.state.stack_private
                if sp is not None:
                    if isinstance(sp, ops.z3.BoolRef) and ops.z3.is_false(sp):
                        enabled_all = False
            if not enabled_all:
                disabled_reasons.append("stack-escape-or-call")
            return PrivateStackInfo(
                enabled_on_all_terminal_paths=enabled_all,
                disabled_reasons=disabled_reasons,
            )

        if not original_terminals or not candidate_terminals:
            return cls()
        original = _side_info(original_terminals)
        candidate = _side_info(candidate_terminals)
        return cls(original=original, candidate=candidate)

    def to_dict(self) -> dict[str, Any]:
        d: dict[str, Any] = {
            "comparison": self.comparison,
            "private_stack": {
                "masking_semantics": self.masking_semantics,
            },
        }
        if self.original is not None:
            d["private_stack"]["original"] = self.original.to_dict()
        if self.candidate is not None:
            d["private_stack"]["candidate"] = self.candidate.to_dict()
        return d


class ProofStatus(str, Enum):
    EQUIVALENT = "equivalent"
    NOT_EQUIVALENT = "not_equivalent"
    INCONCLUSIVE_TIMEOUT = "inconclusive_timeout"
    INCONCLUSIVE_UNKNOWN = "inconclusive_unknown"
    INCONCLUSIVE_UNSUPPORTED = "inconclusive_unsupported"
    INCONCLUSIVE_ABSTRACTION = "inconclusive_abstraction"
    INCONCLUSIVE_LAYOUT = "inconclusive_layout"
    INCONCLUSIVE_UNVALIDATED_CALLEE = "inconclusive_unvalidated_callee"
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
        "unresolved supported PPC relocations share canonical symbolic addresses across both objects",
        "shared byte-addressed initial memory",
        "non-escaping writes below the entry stack pointer, down to each implementation's lowest observed stack pointer, are function-private with independent per-implementation masking; calls or storing an r1-derived pointer disable that masking",
        "well-formed function stacks do not move r1 above the entry stack pointer or wrap around address zero",
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
        "loops are unsupported; external call continuations require explicit matched-callee lemmas and use deterministic opaque ABI summaries",
        "matched-callee summaries preserve EABI nonvolatile registers and machine-control state; a memory-writing callee may modify any aliased caller-frame or public-memory byte",
        "matched callees are location-independent EABI functions: the absolute link-register return address is not a semantic input",
        "MWCC _save/_rest gpr/fpr helpers use their fixed EABI r11-relative load/store layouts",
    ])
    original_instruction_count: int = 0
    candidate_instruction_count: int = 0
    solver: dict[str, Any] = field(default_factory=dict)
    mismatch: dict[str, Any] | None = None
    counterexample: dict[str, Any] | None = None
    replay: dict[str, Any] | None = None
    repair_hint: dict[str, Any] | None = None
    assumed_callees: list[int | str] = field(default_factory=list)
    callee_contracts: dict[str, dict[str, Any]] = field(default_factory=dict)
    abstractions: list[str] = field(default_factory=list)
    environment: MemoryEnvironment | None = None
    memory_scope: MemoryScope | None = None
    counterexample_kind: str | None = None
    unsupported: list[str] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)

    def to_dict(self) -> dict[str, Any]:
        value = asdict(self)
        value["status"] = self.status.value
        if self.memory_scope is not None:
            value["memory_scope"] = self.memory_scope.to_dict()
        else:
            value.pop("memory_scope", None)
        if isinstance(self.environment, MemoryEnvironment):
            value["environment"] = self.environment.to_dict()
        else:
            value.pop("environment", None)
        return value

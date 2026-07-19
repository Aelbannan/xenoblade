from __future__ import annotations

from dataclasses import asdict, dataclass, field
from enum import Enum
from typing import Any

from tools.ppc_equivalence.memory_profile import MemoryEnvironment


# Static coverage labels drawn from SOUNDNESS.md FP limits (P1-11).
# Machine-readable companion to FloatingPointDomain presence/tiering.
# "proven" entries are SMT-constrained under the default domain; "assumed"
# entries are required but not yet encoded as solver predicates.
FP_COVERAGE_PROVEN: tuple[str, ...] = (
    "nearest-even-rounding",
    "ni-required-zero",
    "finite-input-overflow-excluded",
    "fused-operands-exact-expanded-binary32",
    "vx-zx-exception-causes",
    "paired-single-independent-binary32-lanes",
)
FP_COVERAGE_ASSUMED: tuple[str, ...] = (
    "traps-disabled",
)
FP_COVERAGE_UNSUPPORTED: tuple[str, ...] = (
    "underflow-flag",
    "inexact-flag",
    "overflow-flag",
    "fsqrt-fsqrts-broadway-reserved",
    "full-fpscr-status-modeling",
    "fp-exception-trap-delivery",
)

SUPPORTED_FP_ROUNDING_MODES: frozenset[str] = frozenset({"nearest-even"})
SUPPORTED_FUSED_INPUT_DOMAINS: frozenset[str] = frozenset({
    "exact-expanded-binary32",
    "unrestricted",
})


@dataclass(slots=True, frozen=True)
class FloatingPointDomain:
    rounding_modes: tuple[str, ...] = ("nearest-even",)
    require_ni_zero: bool = True
    traps_enabled: bool = False
    allow_nan: bool = True
    allow_infinity: bool = True
    # Default True matches the historical SMT domain (subnormals were never
    # excluded). Set False to constrain them out via FP_DOMAIN_EXCLUDED.
    allow_subnormal: bool = True
    exclude_finite_overflow: bool = True
    model_underflow_flag: bool = False
    model_inexact_flag: bool = False
    fused_input_domain: str = "exact-expanded-binary32"

    def unsupported_reasons(self) -> list[str]:
        """Return why this domain cannot be honored by the current FP model."""
        reasons: list[str] = []
        modes = tuple(self.rounding_modes)
        if not modes:
            reasons.append("rounding_modes must be non-empty")
        elif any(mode not in SUPPORTED_FP_ROUNDING_MODES for mode in modes):
            reasons.append(
                "only nearest-even rounding is modeled "
                f"(got {list(modes)!r})"
            )
        elif modes != ("nearest-even",):
            reasons.append(
                "rounding_modes must be exactly ['nearest-even'] "
                f"(got {list(modes)!r})"
            )
        if not self.require_ni_zero:
            reasons.append("FPSCR.NI≠0 is not modeled; require_ni_zero must be true")
        if self.traps_enabled:
            reasons.append(
                "FP exception trap delivery is not modeled; traps_enabled must be false"
            )
        if self.model_underflow_flag:
            reasons.append("underflow flag modeling is unsupported")
        if self.model_inexact_flag:
            reasons.append("inexact flag modeling is unsupported")
        if self.fused_input_domain not in SUPPORTED_FUSED_INPUT_DOMAINS:
            reasons.append(
                f"fused_input_domain {self.fused_input_domain!r} is unsupported "
                f"(supported: {sorted(SUPPORTED_FUSED_INPUT_DOMAINS)})"
            )
        return reasons

    def validate(self) -> None:
        reasons = self.unsupported_reasons()
        if reasons:
            raise ValueError(
                "unsupported floating-point domain: " + "; ".join(reasons)
            )

    def coverage_dict(self) -> dict[str, list[str]]:
        proven = list(FP_COVERAGE_PROVEN)
        assumed = list(FP_COVERAGE_ASSUMED)
        if not self.exclude_finite_overflow:
            proven = [item for item in proven if item != "finite-input-overflow-excluded"]
        if self.fused_input_domain != "exact-expanded-binary32":
            proven = [
                item for item in proven
                if item != "fused-operands-exact-expanded-binary32"
            ]
        if not self.allow_subnormal:
            proven = [*proven, "subnormals-excluded"]
        if not self.allow_nan:
            proven = [*proven, "nan-excluded"]
        if not self.allow_infinity:
            proven = [*proven, "infinity-excluded"]
        return {
            "proven": proven,
            "assumed": assumed,
            "unsupported": list(FP_COVERAGE_UNSUPPORTED),
        }

    def to_dict(self) -> dict[str, Any]:
        return {
            "used": True,
            "rounding_modes": list(self.rounding_modes),
            "ni": "required-zero" if self.require_ni_zero else "not-required",
            "traps": "excluded" if not self.traps_enabled else "enabled",
            "finite_overflow": "excluded" if self.exclude_finite_overflow else "allowed",
            "underflow_flag": "not-fully-modeled",
            "inexact_flag": "not-fully-modeled",
            "fused_input_domain": self.fused_input_domain,
            "allow_nan": self.allow_nan,
            "allow_infinity": self.allow_infinity,
            "allow_subnormal": self.allow_subnormal,
            "coverage": self.coverage_dict(),
        }

    @classmethod
    def from_dict(cls, d: dict[str, Any]) -> FloatingPointDomain:
        """Accept result ``to_dict`` keys or constructor-style kwargs."""
        rounding = d.get("rounding_modes", ("nearest-even",))
        if "ni" in d:
            require_ni_zero = d.get("ni") == "required-zero"
        else:
            require_ni_zero = bool(d.get("require_ni_zero", True))
        if "traps" in d:
            traps_enabled = d.get("traps") == "enabled"
        else:
            traps_enabled = bool(d.get("traps_enabled", False))
        if "finite_overflow" in d:
            exclude_finite_overflow = d.get("finite_overflow") == "excluded"
        else:
            exclude_finite_overflow = bool(d.get("exclude_finite_overflow", True))
        return cls(
            rounding_modes=tuple(rounding),
            require_ni_zero=require_ni_zero,
            traps_enabled=traps_enabled,
            allow_nan=bool(d.get("allow_nan", True)),
            allow_infinity=bool(d.get("allow_infinity", True)),
            allow_subnormal=bool(d.get("allow_subnormal", True)),
            exclude_finite_overflow=exclude_finite_overflow,
            model_underflow_flag=bool(d.get("model_underflow_flag", False)),
            model_inexact_flag=bool(d.get("model_inexact_flag", False)),
            fused_input_domain=str(
                d.get("fused_input_domain", "exact-expanded-binary32")
            ),
        )

    @classmethod
    def parse(cls, raw: Any) -> FloatingPointDomain:
        if isinstance(raw, FloatingPointDomain):
            return raw
        if isinstance(raw, str):
            import json
            raw = json.loads(raw)
        if not isinstance(raw, dict):
            raise TypeError(
                f"floating-point domain must be a dict or JSON object, got {type(raw)!r}"
            )
        domain = cls.from_dict(raw)
        domain.validate()
        return domain

ARCHITECTURE_MODEL = "broadway-ppc32-be-v22"
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

    @classmethod
    def from_dict(cls, d: dict[str, Any]) -> MemoryScope:
        private = d.get("private_stack") if isinstance(d.get("private_stack"), dict) else {}
        masking = private.get(
            "masking_semantics",
            d.get("masking_semantics", MASKING_SEMANTICS),
        )

        def _side(key: str) -> PrivateStackInfo | None:
            raw = private.get(key)
            if not isinstance(raw, dict):
                return None
            return PrivateStackInfo(
                enabled_on_all_terminal_paths=bool(
                    raw.get("enabled_on_all_terminal_paths", True)
                ),
                disabled_reasons=list(raw.get("disabled_reasons", [])),
                frame_relation=str(
                    raw.get("frame_relation", "symbolic-below-entry-sp")
                ),
            )

        return cls(
            comparison=str(d.get("comparison", "touched-byte-extensional")),
            masking_semantics=str(masking),
            original=_side("original"),
            candidate=_side("candidate"),
        )


class ProofStatus(str, Enum):
    EQUIVALENT = "equivalent"
    NOT_EQUIVALENT = "not_equivalent"
    INCONCLUSIVE_TIMEOUT = "inconclusive_timeout"
    INCONCLUSIVE_UNKNOWN = "inconclusive_unknown"
    INCONCLUSIVE_UNSUPPORTED = "inconclusive_unsupported"
    INCONCLUSIVE_ABSTRACTION = "inconclusive_abstraction"
    INCONCLUSIVE_LAYOUT = "inconclusive_layout"
    INCONCLUSIVE_UNVALIDATED_CALLEE = "inconclusive_unvalidated_callee"
    INCONCLUSIVE_UNMODELED_EXCEPTION = "inconclusive_unmodeled_exception"
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
        "back-edges are explored with a per-PC visit bound (max_loop_iterations); "
        "exhausting the bound is inconclusive and never silently truncated; "
        "external call continuations require explicit matched-callee lemmas and use deterministic opaque ABI summaries",
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
    git_commit: str = ""
    git_dirty: bool = False
    engine_hash: str = ""
    source_hash: str = ""
    platform: str = ""
    python_version: str = ""
    z3_version: str = ""
    capstone_version: str = ""
    floating_point_domain: FloatingPointDomain | None = None
    counterexample_bundle: dict[str, Any] | None = None
    solver_diagnostics: dict[str, Any] | None = None
    # Optional ConcreteOps random/interesting-state sampling report. Secondary
    # defense only — never a stand-alone equivalence certificate.
    concrete_sampling: dict[str, Any] | None = None
    # Opcodes appearing in original+candidate; used with ValidationLedger for
    # Tier A/B confidence (P1-06). Empty means not enumerated (legacy certs).
    opcodes_used: list[str] = field(default_factory=list)
    # CFG exploration bounds recorded on every proof (including inconclusive).
    limits: dict[str, int] = field(default_factory=dict)

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
        if self.floating_point_domain is not None:
            value["floating_point_domain"] = self.floating_point_domain.to_dict()
        else:
            value.pop("floating_point_domain", None)
        if self.concrete_sampling is None:
            value.pop("concrete_sampling", None)
        if not self.limits:
            value.pop("limits", None)
        return value

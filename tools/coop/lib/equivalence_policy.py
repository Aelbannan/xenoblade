from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from tools.coop.lib.config import CoopConfig
from tools.ppc_equivalence.memory_profile import MemoryEnvironment, MemoryProfile
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    MASKING_SEMANTICS,
    RESULT_FORMAT,
    FloatingPointDomain,
    MemoryScope,
    ProofResult,
    ProofStatus,
)


SUPPORTED_CONFIDENCE_TIERS = frozenset({"A", "B", "C"})


@dataclass(frozen=True)
class ValidationLedger:
    """Ledger of independently validated opcodes (via Dolphin corpus)."""

    dolphin_validated_opcodes: frozenset[str]
    dolphin_version: str | None = None
    corpus_hash: str | None = None

    @classmethod
    def load(cls, path: Path | None) -> "ValidationLedger":
        if path is None or not path.is_file():
            return cls(frozenset())
        import json

        data = json.loads(path.read_text(encoding="utf-8"))
        return cls(
            dolphin_validated_opcodes=frozenset(
                data.get("dolphin_validated_opcodes", [])
            ),
            dolphin_version=data.get("dolphin_version"),
            corpus_hash=data.get("corpus_hash"),
        )

    def validate_opcode(self, opcode: str) -> bool:
        return opcode in self.dolphin_validated_opcodes


def compute_confidence_tier_proofresult(
    result: ProofResult,
    ledger: ValidationLedger | None = None,
) -> str | None:
    """Classify a proof into tier A/B/C based on result evidence.

    Tier A — narrow, strongest modeled proofs.
    Tier B — ordinary compiler-generated memory/function proofs.
    Tier C — assumption-heavy proofs.

    Returns None when the proof is not EQUIVALENT.
    """
    if result.status is not ProofStatus.EQUIVALENT:
        return None

    # Presence of a floating-point domain means FP was used in the proof.
    has_fp = result.floating_point_domain is not None

    has_callees = bool(result.assumed_callees)

    has_memory_access = "memory" in result.observables

    has_domain_exceptions = result.counterexample_kind == "definedness"

    has_assumed_ram = (
        result.environment is not None
        and result.environment.profile.value == MemoryProfile.ASSUMED_ORDINARY_RAM.value
    )

    has_complete_provenance = bool(
        result.engine_hash and result.source_hash and result.git_commit
    )

    ledger_incomplete = False
    if ledger is not None and ledger.dolphin_validated_opcodes:
        # Without a machine-readable opcode list on ProofResult, an active ledger
        # means we cannot yet claim Tier A/B independent validation.
        ledger_incomplete = True

    if (
        not has_fp
        and not has_memory_access
        and not has_callees
        and not has_domain_exceptions
        and not ledger_incomplete
        and has_complete_provenance
        and result.architecture_model == ARCHITECTURE_MODEL
        and result.format == RESULT_FORMAT
        and result.memory_scope is not None
        and result.memory_scope.masking_semantics == MASKING_SEMANTICS
    ):
        return "A"

    if (
        has_fp
        or has_domain_exceptions
        or has_assumed_ram
        or not has_complete_provenance
        or ledger_incomplete
    ):
        return "C"

    return "B"


def compute_confidence_tier_from_certificate(
    certificate: dict[str, Any] | None,
) -> str | None:
    """Classify based on certificate summary dict (legacy / registry path)."""
    if certificate is None:
        return None

    summary = certificate.get("summary")
    if not isinstance(summary, dict):
        return None

    writes: list[str] = sorted(summary.get("writes", []))
    reads: list[str] = sorted(summary.get("reads", []))

    has_memory_access = "memory" in writes or "memory" in reads
    has_fp_access = any(
        w.startswith("f") and (w[1:].isdigit() or w in ("fpscr",))
        for w in writes + reads
    )
    has_callee_calls = bool(certificate.get("callees"))
    invalid_reasons: list[int] = summary.get("invalid_reasons", [])
    has_domain_exception = bool(invalid_reasons)

    if (
        not has_memory_access
        and not has_fp_access
        and not has_callee_calls
        and not has_domain_exception
    ):
        return "A"
    if has_fp_access or has_domain_exception:
        return "C"
    return "B"


def compute_confidence_tier(
    result_or_certificate: ProofResult | dict[str, Any] | None,
    ledger: ValidationLedger | None = None,
) -> str | None:
    """Dispatch tier classification for ProofResult or certificate dict."""
    if isinstance(result_or_certificate, ProofResult):
        return compute_confidence_tier_proofresult(result_or_certificate, ledger)
    return compute_confidence_tier_from_certificate(result_or_certificate)


@dataclass(frozen=True)
class PromotionDecision:
    allowed: bool
    confidence_tier: str | None = None
    blockers: tuple[str, ...] = field(default_factory=tuple)
    warnings: tuple[str, ...] = field(default_factory=tuple)


def _check_tier_allowed(
    tier: str | None,
    allowed: frozenset[str],
    blockers: list[str],
) -> None:
    if tier is None:
        blockers.append("no-certificate-for-tier-classification")
    elif tier not in allowed:
        blockers.append(f"confidence-tier-{tier}-not-allowed")


def _check_engine_provenance(
    result: ProofResult,
    allowed_sha256: str | None,
    blockers: list[str],
) -> None:
    if allowed_sha256 is None:
        return
    if not result.engine_hash:
        blockers.append("no-certificate-for-engine-provenance-check")
        return
    if result.engine_hash != allowed_sha256:
        blockers.append(
            f"engine-sha256-{result.engine_hash}!=allowed-{allowed_sha256}"
        )


def proof_result_from_certificate(
    status: ProofStatus,
    certificate: dict[str, Any] | None,
) -> ProofResult:
    """Rebuild a ProofResult from a stored equivalence certificate."""
    if certificate is None:
        return ProofResult(status=status)

    memory_scope = None
    raw_scope = certificate.get("memory_scope")
    if isinstance(raw_scope, MemoryScope):
        memory_scope = raw_scope
    elif isinstance(raw_scope, dict):
        memory_scope = MemoryScope.from_dict(raw_scope)

    environment = None
    raw_env = certificate.get("environment")
    if isinstance(raw_env, MemoryEnvironment):
        environment = raw_env
    elif isinstance(raw_env, dict):
        environment = MemoryEnvironment.from_dict(raw_env)

    floating_point_domain = None
    raw_fp = certificate.get("floating_point_domain")
    if isinstance(raw_fp, FloatingPointDomain):
        floating_point_domain = raw_fp
    elif isinstance(raw_fp, dict):
        floating_point_domain = FloatingPointDomain.from_dict(raw_fp)

    assumed_callees: list[int | str] = []
    raw_callees = certificate.get("assumed_callees")
    if isinstance(raw_callees, list):
        assumed_callees = list(raw_callees)
    elif certificate.get("callees"):
        assumed_callees = [
            item.get("target_id", item) if isinstance(item, dict) else item
            for item in certificate["callees"]
        ]

    observables = certificate.get("observables")
    if not isinstance(observables, list):
        summary = certificate.get("summary")
        if isinstance(summary, dict):
            observables = sorted(
                set(summary.get("reads", [])) | set(summary.get("writes", []))
            )
        else:
            observables = []

    return ProofResult(
        status=status,
        architecture_model=str(
            certificate.get("architecture", ARCHITECTURE_MODEL)
        ),
        format=int(certificate.get("result_format", RESULT_FORMAT)),
        observables=list(observables),
        assumed_callees=assumed_callees,
        environment=environment,
        memory_scope=memory_scope,
        engine_hash=str(certificate.get("engine_hash", "")),
        source_hash=str(certificate.get("source_hash", "")),
        git_commit=str(certificate.get("git_commit", "")),
        floating_point_domain=floating_point_domain,
        counterexample_kind=certificate.get("counterexample_kind"),
    )


def classify_for_promotion(
    result: ProofResult,
    policy: "PromotionPolicy",
    ledger: ValidationLedger,
) -> PromotionDecision:
    blockers: list[str] = []
    warnings: list[str] = []

    if not policy.automatic_promotion:
        blockers.append("automatic-promotion-disabled-by-config")

    if result.status is not ProofStatus.EQUIVALENT:
        blockers.append("proof-status-is-not-equivalent")

    if result.architecture_model in policy.reject_architecture_models:
        blockers.append(f"architecture-model-{result.architecture_model}-rejected")

    if result.format < policy.minimum_result_format:
        blockers.append(f"result-format-{result.format}-too-old")

    if not result.engine_hash:
        blockers.append("missing-engine-provenance")

    if result.memory_scope is not None:
        masking = result.memory_scope.masking_semantics
        if masking != MASKING_SEMANTICS:
            blockers.append(f"unsafe-or-unknown-private-stack-masking-{masking}")
    elif result.status is ProofStatus.EQUIVALENT:
        warnings.append("missing-memory-scope-on-equivalent-proof")

    if (
        policy.require_bounded_ram
        and result.environment is not None
        and result.environment.profile.value
        == MemoryProfile.ASSUMED_ORDINARY_RAM.value
        and result.counterexample_kind != "definedness"
    ):
        blockers.append("unconstrained-symbolic-memory-domain")

    tier = compute_confidence_tier_proofresult(result, ledger)
    _check_tier_allowed(tier, policy.allowed_confidence_tiers, blockers)
    _check_engine_provenance(result, policy.allowed_engine_sha256, blockers)

    if ledger.dolphin_validated_opcodes and tier in {"A", "B"}:
        warnings.append("validation-ledger-present-but-opcodes-not-enumerated")

    return PromotionDecision(
        allowed=not blockers,
        confidence_tier=tier,
        blockers=tuple(blockers),
        warnings=tuple(warnings),
    )


def classify_for_promotion_legacy(
    equivalence: ProofStatus | None,
    match_percent: float | None,
    config: CoopConfig,
    *,
    certificate: dict[str, Any] | None = None,
    proof: ProofResult | None = None,
) -> PromotionDecision:
    """Adapter for objdiff_report: status + certificate/proof → decision."""
    del match_percent  # fuzzy floor is enforced by the caller
    policy = PromotionPolicy.from_config(config)
    ledger = ValidationLedger.load(None)
    if proof is not None:
        return classify_for_promotion(proof, policy, ledger)
    result = proof_result_from_certificate(
        equivalence or ProofStatus.INVALID_INPUT,
        certificate,
    )
    return classify_for_promotion(result, policy, ledger)


@dataclass(frozen=True)
class PromotionPolicy:
    automatic_promotion: bool = True
    reject_architecture_models: frozenset[str] = frozenset(
        {"broadway-ppc32-be-v18"}
    )
    minimum_result_format: int = RESULT_FORMAT
    allowed_confidence_tiers: frozenset[str] = field(
        default_factory=lambda: frozenset({"A", "B"})
    )
    allowed_engine_sha256: str | None = None
    require_bounded_ram: bool = False

    @classmethod
    def from_config(cls, config: CoopConfig) -> "PromotionPolicy":
        return cls(
            automatic_promotion=config.automatic_promotion,
            reject_architecture_models=frozenset(config.reject_architecture_models),
            minimum_result_format=RESULT_FORMAT,
            allowed_confidence_tiers=frozenset(config.allowed_confidence_tiers),
            allowed_engine_sha256=config.allowed_engine_sha256,
            require_bounded_ram=bool(
                getattr(config, "require_bounded_ram", False)
            ),
        )

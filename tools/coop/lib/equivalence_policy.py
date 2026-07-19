from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.equivalence_check import EQUIVALENT_MATCH_MIN_PERCENT
from tools.coop.lib.targets import EQUIVALENCE_CERTIFICATE_VERSION
from tools.ppc_equivalence.memory_profile import MemoryProfile
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    MASKING_SEMANTICS,
    RESULT_FORMAT,
    ProofStatus,
)
from tools.ppc_equivalence.model import InvalidReason


SUPPORTED_CONFIDENCE_TIERS = frozenset({"A", "B", "C"})


def compute_confidence_tier(certificate: dict[str, Any] | None) -> str | None:
    """Classify a proof into tier A/B/C based on certificate evidence.

    Returns None when there is no certificate to classify.
    """
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

    if not has_memory_access and not has_fp_access and not has_callee_calls and not has_domain_exception:
        return "A"
    if has_fp_access or has_domain_exception:
        return "C"
    return "B"


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
    certificate: dict[str, Any] | None,
    allowed_sha256: str | None,
    blockers: list[str],
) -> None:
    if allowed_sha256 is None:
        return
    if certificate is None:
        blockers.append("no-certificate-for-engine-provenance-check")
        return
    actual = certificate.get("engine_sha256")
    if not actual:
        blockers.append("certificate-missing-engine-sha256")
    elif actual != allowed_sha256:
        blockers.append(f"engine-sha256-{actual}!=allowed-{allowed_sha256}")


def classify_for_promotion(
    equivalence: ProofStatus | None,
    match_percent: float | None,
    config: CoopConfig,
    *,
    certificate: dict[str, Any] | None = None,
) -> PromotionDecision:
    blockers: list[str] = []
    warnings: list[str] = []

    if not config.automatic_promotion:
        blockers.append("automatic-promotion-disabled-by-config")

    if equivalence is not ProofStatus.EQUIVALENT:
        blockers.append("proof-status-is-not-equivalent")

    if match_percent is None or match_percent < EQUIVALENT_MATCH_MIN_PERCENT:
        blockers.append("match-percent-below-threshold")

    tier = None
    if certificate:
        cert_version = certificate.get("version")
        if cert_version is not None and cert_version != EQUIVALENCE_CERTIFICATE_VERSION:
            blockers.append(
                f"certificate-version-{cert_version}!=current-{EQUIVALENCE_CERTIFICATE_VERSION}"
            )

        cert_arch = certificate.get("architecture")
        if cert_arch in config.reject_architecture_models:
            blockers.append(
                f"certificate-architecture-{cert_arch}-in-rejected-models"
            )

        cert_format = certificate.get("result_format")
        if cert_format is not None and cert_format != RESULT_FORMAT:
            blockers.append(
                f"certificate-result-format-{cert_format}!=current-{RESULT_FORMAT}"
            )

        mem_scope = certificate.get("memory_scope")
        if mem_scope is not None:
            ps = mem_scope.get("private_stack", {})
            masking = ps.get("masking_semantics")
            if masking is not None and masking != MASKING_SEMANTICS:
                blockers.append(
                    f"private-stack-masking-semantics-{masking}!=expected-{MASKING_SEMANTICS}"
                )

        env = certificate.get("environment")
        if env is not None:
            profile = env.get("memory_profile")
            if profile is not None and profile == MemoryProfile.ASSUMED_ORDINARY_RAM.value:
                warnings.append("memory-profile-is-assumed-ordinary-ram")
            elif profile is not None and profile == MemoryProfile.BOUNDED_ORDINARY_RAM.value:
                ranges = env.get("ranges", [])
                if not ranges:
                    warnings.append(
                        "memory-profile-is-bounded-ordinary-ram-with-no-ranges"
                    )

        tier = compute_confidence_tier(certificate)
        _check_tier_allowed(tier, config.allowed_confidence_tiers, blockers)
        _check_engine_provenance(certificate, config.allowed_engine_sha256, blockers)

    return PromotionDecision(
        allowed=not blockers,
        confidence_tier=tier,
        blockers=tuple(blockers),
        warnings=tuple(warnings),
    )

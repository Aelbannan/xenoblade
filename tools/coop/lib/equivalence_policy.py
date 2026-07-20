from __future__ import annotations

import hashlib
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from tools.coop.lib.config import CoopConfig
from tools.ppc_equivalence.capability_assurance import (
    CapabilityAssurance,
    CapabilityAssuranceResult,
    CapabilityManifest,
    compute_confidence_tier_from_assurance,
    default_capability_manifest_path,
    evaluate_capability_assurance,
    load_capability_manifest,
)
from tools.ppc_equivalence.memory_profile import MemoryEnvironment, MemoryProfile
from tools.ppc_equivalence.proof_features import (
    PROOF_OBLIGATION_FIELDS,
    enforce_equivalent_proof_features,
    parse_proof_features,
    apply_proof_obligations,
)
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    MASKING_SEMANTICS,
    RESULT_FORMAT,
    FP_COVERAGE_STATUS_ASSUMED,
    FP_COVERAGE_STATUS_NONE,
    FloatingPointDomain,
    MemoryScope,
    ProofResult,
    ProofStatus,
    fp_coverage_status,
    proof_fp_coverage_status,
)


SUPPORTED_CONFIDENCE_TIERS = frozenset({"A", "B", "C"})

# tools/coop/lib/equivalence_policy.py → tools/ppc_equivalence/validation_ledger.yaml
DEFAULT_VALIDATION_LEDGER = (
    Path(__file__).resolve().parents[2]
    / "ppc_equivalence"
    / "validation_ledger.yaml"
)

_DEFAULT_CORPUS_PATH = (
    Path(__file__).resolve().parents[2]
    / "ppc_equivalence"
    / "fixtures"
    / "broadway.jsonl"
)


def default_validation_ledger_path() -> Path:
    return DEFAULT_VALIDATION_LEDGER


def _expected_corpus_hash() -> str | None:
    """Cheap SHA-256 of the default Dolphin corpus when present on disk."""
    if not _DEFAULT_CORPUS_PATH.is_file():
        return None
    return hashlib.sha256(_DEFAULT_CORPUS_PATH.read_bytes()).hexdigest()


@dataclass(frozen=True)
class ValidationLedger:
    """Ledger of independently validated opcodes (via Dolphin corpus).

    ``intentionally_loaded`` distinguishes "no ledger in use" from "a ledger
    file/mapping was parsed". ``load`` / ``from_mapping`` set it True.
    Absent ledgers (``is_absent``) fail closed for promotion and opcode
    completeness — they never authorize skipping Gate 2/3.
    """

    dolphin_validated_opcodes: frozenset[str]
    dolphin_version: str | None = None
    corpus_hash: str | None = None
    architecture_model: str | None = None
    corpus_version: int | None = None
    intentionally_loaded: bool = False

    @classmethod
    def load(cls, path: Path | None) -> "ValidationLedger":
        # Missing path / file → absent ledger (fail closed for promotion).
        if path is None or not path.is_file():
            return cls(frozenset(), intentionally_loaded=False)
        suffix = path.suffix.lower()
        if suffix in {".yaml", ".yml"}:
            data = _load_mapping(path, yaml=True)
        else:
            data = _load_mapping(path, yaml=False)
        return cls.from_mapping(data)

    @classmethod
    def from_mapping(cls, data: dict[str, Any]) -> "ValidationLedger":
        dolphin: set[str] = set()
        raw_list = data.get("dolphin_validated_opcodes")
        if isinstance(raw_list, list):
            dolphin.update(str(item) for item in raw_list)

        opcodes = data.get("opcodes")
        if isinstance(opcodes, dict):
            for name, meta in opcodes.items():
                if not isinstance(meta, dict):
                    continue
                if bool(meta.get("dolphin_interpreter")):
                    dolphin.add(str(name))

        corpus_version = data.get("corpus_version")
        return cls(
            dolphin_validated_opcodes=frozenset(dolphin),
            dolphin_version=(
                str(data["dolphin_version"])
                if data.get("dolphin_version") is not None
                else None
            ),
            corpus_hash=(
                str(data["corpus_hash"])
                if data.get("corpus_hash") is not None
                else None
            ),
            architecture_model=(
                str(data["architecture_model"])
                if data.get("architecture_model") is not None
                else None
            ),
            corpus_version=int(corpus_version) if corpus_version is not None else None,
            intentionally_loaded=True,
        )

    def is_absent(self) -> bool:
        """True when no ledger file/mapping was loaded and no opcode set was supplied.

        Absent ledgers must not authorize promotion: opcode gating would otherwise
        be skipped under ``automatic_promotion: true``.
        """
        return not self.intentionally_loaded and not self.dolphin_validated_opcodes

    def validate_opcode(self, opcode: str) -> bool:
        return opcode in self.dolphin_validated_opcodes

    def promotion_metadata_invalid(self) -> bool:
        """True when a loaded ledger's provenance mismatches the live engine."""
        if not self.intentionally_loaded:
            return False
        if (
            self.architecture_model is not None
            and self.architecture_model != ARCHITECTURE_MODEL
        ):
            return True
        if self.corpus_hash is not None:
            expected = _expected_corpus_hash()
            if expected is not None and self.corpus_hash != expected:
                return True
        return False

    def missing_dolphin_opcodes(self, opcodes_used: list[str] | frozenset[str]) -> list[str]:
        """Return opcodes that lack dolphin_interpreter evidence, sorted.

        Absent ledgers (``is_absent``) fail closed: every used opcode is reported
        missing so tiering and promotion cannot treat "no ledger" as validated.
        A ledger that was intentionally loaded but has an empty dolphin set also
        fails closed for all used opcodes. Architecture / corpus mismatches on a
        loaded ledger likewise fail closed for all used opcodes.
        """
        used = {str(op) for op in opcodes_used}
        if not used:
            return []
        if self.is_absent():
            return sorted(used)
        if self.promotion_metadata_invalid() or not self.dolphin_validated_opcodes:
            return sorted(used)
        return sorted(op for op in used if not self.validate_opcode(op))


def _load_mapping(path: Path, *, yaml: bool) -> dict[str, Any]:
    text = path.read_text(encoding="utf-8")
    if yaml:
        try:
            import yaml as pyyaml
        except ImportError as exc:  # pragma: no cover
            raise RuntimeError(
                f"PyYAML is required to read {path}. Install with: pip install pyyaml"
            ) from exc
        data = pyyaml.safe_load(text) or {}
    else:
        import json

        data = json.loads(text)
    if not isinstance(data, dict):
        raise ValueError(f"validation ledger {path} must be a mapping")
    return data


def _compute_confidence_tier_legacy(
    result: ProofResult,
    ledger: ValidationLedger | None = None,
) -> str | None:
    """Legacy effect-type tiering (authoritative while shadow_mode is true).

    Tier A — narrow, strongest modeled proofs.
    Tier B — ordinary compiler-generated memory/function proofs.
    Tier C — assumption-heavy proofs.

    Returns None when the proof is not EQUIVALENT.
    """
    if result.status is not ProofStatus.EQUIVALENT:
        return None

    # P1-11: any FP proof or merely-assumed FP restriction stays Tier C until
    # promotion policy explicitly allows stronger tiers for proven domains.
    fp_coverage = proof_fp_coverage_status(result)
    has_fp = fp_coverage != FP_COVERAGE_STATUS_NONE
    has_assumed_fp = fp_coverage == FP_COVERAGE_STATUS_ASSUMED

    has_callees = bool(result.assumed_callees)

    has_memory_access = "memory" in result.observables

    has_memory_bus = (
        "memory-bus" in (result.proof_features or [])
        or getattr(result, "memory_bus", None) is not None
    )

    has_domain_exceptions = (
        result.counterexample_kind == "definedness"
        or bool(result.invalid_reasons)
        or any(
            str(item).startswith("domain-exception:")
            for item in (result.assumptions or [])
        )
    )

    # Assumed ordinary RAM is Tier C only when memory is an observable; the
    # engine always records an effective environment (default assumed-RAM), so
    # register-only proofs must not be demoted solely for that recording.
    has_assumed_ram = (
        has_memory_access
        and result.environment is not None
        and result.environment.profile.value == MemoryProfile.ASSUMED_ORDINARY_RAM.value
    )

    has_complete_provenance = bool(
        result.engine_hash and result.source_hash and result.git_commit
    )

    # P1-06: absent or incomplete ledgers demote; when opcodes_used is present,
    # every opcode must carry dolphin_interpreter evidence for Tier A/B.
    # Loaded ledgers with wrong architecture_model / corpus_hash also demote.
    ledger_incomplete = False
    if ledger is not None:
        if ledger.is_absent() or ledger.promotion_metadata_invalid():
            ledger_incomplete = True
        elif result.opcodes_used:
            ledger_incomplete = bool(ledger.missing_dolphin_opcodes(result.opcodes_used))

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
        or has_assumed_fp
        or has_domain_exceptions
        or has_assumed_ram
        or has_memory_bus
        or not has_complete_provenance
        or ledger_incomplete
    ):
        return "C"

    return "B"


def resolve_capability_manifest(
    manifest: CapabilityManifest | dict[str, Any] | Path | None = None,
    *,
    config: CoopConfig | None = None,
) -> CapabilityManifest:
    """Resolve a capability manifest from an object, mapping, path, or coop config."""
    if isinstance(manifest, CapabilityManifest):
        return manifest
    if isinstance(manifest, dict):
        return CapabilityManifest.from_dict(manifest)
    if isinstance(manifest, Path):
        return load_capability_manifest(manifest)
    if config is not None:
        path = config.resolve(config.capability_manifest_path)
        loaded = load_capability_manifest(path)
        # CoopConfig knobs override file defaults when set explicitly.
        allowed = dict(config.allowed_tier_a_capabilities) or dict(
            loaded.allowed_tier_a_capabilities
        )
        return CapabilityManifest(
            schema_version=loaded.schema_version,
            policy=loaded.policy,
            allowed_tier_a_capabilities=allowed,
            shadow_mode=bool(config.capability_assurance_shadow_mode),
            require_capability_assurance=bool(config.require_capability_assurance),
        )
    return load_capability_manifest(default_capability_manifest_path())


def _attach_shadow_warning(result: ProofResult, assurance_tier: str | None) -> None:
    if assurance_tier is None:
        return
    warning = f"capability-assurance-shadow-tier-{assurance_tier}"
    current = list(result.warnings or [])
    if warning not in current:
        current.append(warning)
        result.warnings = current


def compute_confidence_tier_proofresult(
    result: ProofResult,
    ledger: ValidationLedger | None = None,
    *,
    manifest: CapabilityManifest | dict[str, Any] | Path | None = None,
    config: CoopConfig | None = None,
    assurance_out: list[CapabilityAssuranceResult] | None = None,
) -> str | None:
    """Classify a proof into tier A/B/C.

    Always evaluates capability assurance when possible. While
    ``shadow_mode`` is true (Wave 1 default), the authoritative return value
    remains the legacy effect-type tier; the assurance tier is attached as a
    ``capability-assurance-shadow-tier-*`` warning.
    """
    resolved = resolve_capability_manifest(manifest, config=config)
    legacy = _compute_confidence_tier_legacy(result, ledger)
    assurance = evaluate_capability_assurance(
        result,
        ledger,
        resolved,
        shadow_legacy_tier=legacy,
    )
    if assurance_out is not None:
        assurance_out.append(assurance)
    assurance_tier = (
        compute_confidence_tier_from_assurance(assurance)
        if result.status is ProofStatus.EQUIVALENT
        else None
    )
    if resolved.shadow_mode:
        _attach_shadow_warning(result, assurance_tier)
        return legacy
    return assurance_tier


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
    raw_fp = certificate.get("floating_point_domain")
    fp_coverage = FP_COVERAGE_STATUS_NONE
    if isinstance(raw_fp, FloatingPointDomain):
        fp_coverage = raw_fp.coverage_status()
    elif isinstance(raw_fp, dict):
        fp_coverage = fp_coverage_status(FloatingPointDomain.from_dict(raw_fp))
    has_assumed_fp = fp_coverage == FP_COVERAGE_STATUS_ASSUMED
    has_callee_calls = bool(certificate.get("callees"))
    invalid_reasons: list[int] = summary.get("invalid_reasons", [])
    has_domain_exception = bool(invalid_reasons)

    if (
        not has_memory_access
        and not has_fp_access
        and not has_callee_calls
        and not has_domain_exception
        and not has_assumed_fp
    ):
        return "A"
    if has_fp_access or has_assumed_fp or has_domain_exception:
        return "C"
    return "B"


def compute_confidence_tier(
    result_or_certificate: ProofResult | dict[str, Any] | None,
    ledger: ValidationLedger | None = None,
    *,
    manifest: CapabilityManifest | dict[str, Any] | Path | None = None,
    config: CoopConfig | None = None,
    assurance_out: list[CapabilityAssuranceResult] | None = None,
) -> str | None:
    """Dispatch tier classification for ProofResult or certificate dict."""
    if isinstance(result_or_certificate, ProofResult):
        return compute_confidence_tier_proofresult(
            result_or_certificate,
            ledger,
            manifest=manifest,
            config=config,
            assurance_out=assurance_out,
        )
    if isinstance(result_or_certificate, dict):
        resolved = resolve_capability_manifest(manifest, config=config)
        legacy = compute_confidence_tier_from_certificate(result_or_certificate)
        status_raw = result_or_certificate.get("status")
        try:
            if status_raw in {None, "SEMANTIC_CERTIFIED", "equivalent"}:
                status = ProofStatus.EQUIVALENT
            else:
                status = ProofStatus(str(status_raw))
        except ValueError:
            status = ProofStatus.EQUIVALENT
        # Rebuild for assurance evaluation only. Legacy certificates without
        # capability_assurance must not invent attestations (→ assurance C).
        proof = proof_result_from_certificate(status, result_or_certificate)
        assurance = evaluate_capability_assurance(
            proof,
            ledger,
            resolved,
            shadow_legacy_tier=legacy,
        )
        if assurance_out is not None:
            assurance_out.append(assurance)
        if resolved.shadow_mode:
            return legacy
        if status is not ProofStatus.EQUIVALENT and legacy is None:
            return None
        return compute_confidence_tier_from_assurance(assurance)
    return None


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

    opcodes_used: list[str] = []
    raw_opcodes = certificate.get("opcodes_used")
    if isinstance(raw_opcodes, list):
        opcodes_used = [str(op) for op in raw_opcodes]

    limits: dict[str, int] = {}
    raw_limits = certificate.get("limits")
    if isinstance(raw_limits, dict):
        limits = {str(k): int(v) for k, v in raw_limits.items()}

    assumptions: list[str] | None = None
    raw_assumptions = certificate.get("assumptions")
    if isinstance(raw_assumptions, list):
        assumptions = [str(item) for item in raw_assumptions]

    callee_contracts: dict[str, dict[str, Any]] = {}
    raw_callee_contracts = certificate.get("callee_contracts")
    if isinstance(raw_callee_contracts, dict):
        callee_contracts = {
            str(name): dict(entry) if isinstance(entry, dict) else {"source": str(entry)}
            for name, entry in raw_callee_contracts.items()
        }

    repair_hint = certificate.get("repair_hint")
    if repair_hint is not None and not isinstance(repair_hint, dict):
        repair_hint = None

    invalid_reasons: list[int] = []
    summary = certificate.get("summary")
    if isinstance(summary, dict):
        raw_reasons = summary.get("invalid_reasons", [])
        if isinstance(raw_reasons, list):
            invalid_reasons = [int(item) for item in raw_reasons]

    # Surface domain exceptions into tier classification even when the
    # certificate omits counterexample_kind.
    counterexample_kind = certificate.get("counterexample_kind")
    if invalid_reasons and not counterexample_kind:
        counterexample_kind = "definedness"

    parsed = parse_proof_features(certificate)

    capability_assurance = None
    raw_assurance = certificate.get("capability_assurance")
    if isinstance(raw_assurance, CapabilityAssurance):
        capability_assurance = raw_assurance.to_dict()
    elif isinstance(raw_assurance, dict):
        capability_assurance = dict(raw_assurance)

    result = ProofResult(
        status=status,
        architecture_model=str(
            certificate.get("architecture", ARCHITECTURE_MODEL)
        ),
        format=int(certificate.get("result_format", RESULT_FORMAT)),
        contract=str(certificate.get("contract", "manual")),
        observables=list(observables),
        assumed_callees=assumed_callees,
        callee_contracts=callee_contracts,
        environment=environment,
        memory_scope=memory_scope,
        engine_hash=str(certificate.get("engine_hash", "")),
        source_hash=str(certificate.get("source_hash", "")),
        git_commit=str(certificate.get("git_commit", "")),
        floating_point_domain=floating_point_domain,
        counterexample_kind=counterexample_kind,
        opcodes_used=opcodes_used,
        limits=limits,
        repair_hint=repair_hint,
        invalid_reasons=invalid_reasons,
        proof_features=list(parsed.features),
        capability_assurance=capability_assurance,
    )
    raw_oracle = certificate.get("fp_oracle_version")
    if raw_oracle:
        result.fp_oracle_version = str(raw_oracle)
    apply_proof_obligations(result, parsed.obligations)
    if assumptions is not None:
        result.assumptions = list(assumptions)
    elif invalid_reasons:
        result.assumptions = list(result.assumptions) + [
            f"domain-exception:{code}" for code in invalid_reasons
        ]
    return enforce_equivalent_proof_features(result)


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
        and result.counterexample_kind != "definedness"
    ):
        from tools.ppc_equivalence.memory_profile import is_bounded_with_ranges

        if not is_bounded_with_ranges(result.environment):
            blockers.append("unconstrained-symbolic-memory-domain")

    if ledger.is_absent():
        blockers.append("validation-ledger-absent")
    elif ledger.intentionally_loaded or ledger.dolphin_validated_opcodes:
        if ledger.promotion_metadata_invalid():
            if (
                ledger.architecture_model is not None
                and ledger.architecture_model != ARCHITECTURE_MODEL
            ):
                blockers.append(
                    f"ledger-architecture-model-{ledger.architecture_model}"
                    f"!=live-{ARCHITECTURE_MODEL}"
                )
            else:
                blockers.append("ledger-corpus-hash-mismatch")
        for opcode in ledger.missing_dolphin_opcodes(result.opcodes_used):
            blockers.append(f"opcode-{opcode}-not-dolphin-validated")

    manifest = CapabilityManifest(
        allowed_tier_a_capabilities=dict(policy.allowed_tier_a_capabilities),
        shadow_mode=policy.capability_assurance_shadow_mode,
        require_capability_assurance=policy.require_capability_assurance,
    )
    assurance_box: list[CapabilityAssuranceResult] = []
    tier = compute_confidence_tier_proofresult(
        result,
        ledger,
        manifest=manifest,
        assurance_out=assurance_box,
    )
    _check_tier_allowed(tier, policy.allowed_confidence_tiers, blockers)
    _check_engine_provenance(result, policy.allowed_engine_sha256, blockers)

    assurance = assurance_box[0] if assurance_box else None
    if not policy.capability_assurance_shadow_mode:
        if policy.require_capability_assurance and result.capability_assurance is None:
            blockers.append("capability-assurance-required")
        if assurance is not None and assurance.has_unmodeled_or_unproven_capability:
            blockers.append("capability-assurance-unmodeled-or-unproven")
        if (
            policy.require_capability_assurance
            and policy.allowed_engine_sha256 is None
        ):
            blockers.append("allowed-engine-sha256-required-for-assurance-promotion")
        for warning in result.warnings or []:
            if str(warning).startswith("capability-assurance-shadow-tier-"):
                warnings.append(str(warning))
    else:
        for warning in result.warnings or []:
            if str(warning).startswith("capability-assurance-shadow-tier-"):
                warnings.append(str(warning))

    return PromotionDecision(
        allowed=not blockers,
        confidence_tier=tier,
        blockers=tuple(blockers),
        warnings=tuple(sorted(set(warnings))),
    )


def classify_for_promotion_legacy(
    equivalence: ProofStatus | None,
    match_percent: float | None,
    config: CoopConfig,
    *,
    certificate: dict[str, Any] | None = None,
    proof: ProofResult | dict[str, Any] | None = None,
    ledger_path: Path | None = None,
) -> PromotionDecision:
    """Adapter for objdiff_report: status + certificate/proof → decision."""
    del match_percent  # fuzzy floor is enforced by the caller
    policy = PromotionPolicy.from_config(config)
    path = ledger_path if ledger_path is not None else default_validation_ledger_path()
    ledger = ValidationLedger.load(path)
    if proof is not None:
        if isinstance(proof, dict):
            merged: dict[str, Any] = dict(certificate or {})
            merged.update(proof)
            proof = proof_result_from_certificate(
                equivalence or ProofStatus.INVALID_INPUT,
                merged,
            )
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
        {
            "broadway-ppc32-be-v18",
            "broadway-ppc32-be-v19",
            "broadway-ppc32-be-v20",
            "broadway-ppc32-be-v21",
            "broadway-ppc32-be-v22",
            "broadway-ppc32-be-v23",
            "broadway-ppc32-be-v24",
            "broadway-ppc32-be-v25",
            "broadway-ppc32-be-v26",
            "broadway-ppc32-be-v27",
            "broadway-ppc32-be-v28",
            "broadway-ppc32-be-v29",
            "broadway-ppc32-be-v30",
            "broadway-ppc32-be-v31",
            "broadway-ppc32-be-v32",
            "broadway-ppc32-be-v33",
            "broadway-ppc32-be-v34",
            "broadway-ppc32-be-v35",
            "broadway-ppc32-be-v36",
        }
    )
    minimum_result_format: int = RESULT_FORMAT
    allowed_confidence_tiers: frozenset[str] = field(
        default_factory=lambda: frozenset({"A", "B"})
    )
    # Required when capability assurance is authoritative; optional in Wave 1
    # shadow mode.
    allowed_engine_sha256: str | None = None
    require_bounded_ram: bool = False
    allowed_tier_a_capabilities: dict[str, tuple[str, ...]] = field(
        default_factory=dict
    )
    capability_assurance_shadow_mode: bool = True
    require_capability_assurance: bool = False

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
            allowed_tier_a_capabilities=dict(
                getattr(config, "allowed_tier_a_capabilities", {}) or {}
            ),
            capability_assurance_shadow_mode=bool(
                getattr(config, "capability_assurance_shadow_mode", True)
            ),
            require_capability_assurance=bool(
                getattr(config, "require_capability_assurance", False)
            ),
        )

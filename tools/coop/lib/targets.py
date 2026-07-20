from __future__ import annotations

import json
import hashlib
import re
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, Iterable, List, Optional

from tools.coop.lib.config import CoopConfig, _load_yaml
from tools.symbolrecover.lib.mwcc import demangle_symbol
from tools.symbolrecover.lib.parser import SymbolEntry, load_symbols
from tools.ppc_equivalence.proof_features import validate_proof_features
from tools.ppc_equivalence.provenance import hash_certifier_tree, hash_engine_tree
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT


MATCH_STATUSES = {
    "NOT_STARTED",
    "COMPILES",
    "STRUCTURAL",
    "HIGH_MATCH",
    "CODE_MATCH",
    "EQUIVALENT_MATCH",
    "FULL_MATCH",
    "BEHAVIOR_VERIFIED",
}

WORKFLOW_STATUSES = {
    "BACKLOG",
    "DISCOVERY",
    "QUEUED",
    "CLAIMED",
    "ACTIVE",
    "ACCEPTED",
    "REVALIDATION_REQUIRED",
    "BLOCKED",
    "NOT_REQUIRED",
}

ACCEPTED_MATCH_STATUSES = {"EQUIVALENT_MATCH", "FULL_MATCH"}
EQUIVALENCE_CERTIFICATE_VERSION = 12
EQUIVALENCE_PROMOTION_POLICY = "auto-promotion-v2"

_REPO_ROOT = Path(__file__).resolve().parents[3]


def equivalence_certificate_hash(certificate: Dict[str, Any]) -> str:
    """Hash a certificate's signed payload (everything except its own hash)."""
    payload = {key: value for key, value in certificate.items() if key != "certificate_sha256"}
    encoded = json.dumps(payload, sort_keys=True, separators=(",", ":"), ensure_ascii=True)
    return hashlib.sha256(encoded.encode("utf-8")).hexdigest()


def equivalence_certificate_error(
    row: Dict[str, Any],
    rows_by_id: Dict[str, Dict[str, Any]],
    *,
    _checking: Optional[set[str]] = None,
) -> Optional[str]:
    """Return why a registry certificate cannot be trusted, or ``None``."""
    certificate = row.get("equivalence_certificate")
    if not isinstance(certificate, dict):
        return "missing equivalence_certificate"
    required = {
        "version", "status", "architecture", "result_format", "target_id",
        "retail_sha256", "candidate_sha256", "summary", "callees",
        "helpers", "certificate_sha256",
    }
    missing = sorted(required - certificate.keys())
    if missing:
        return "certificate missing " + ", ".join(missing)
    if certificate.get("version") != EQUIVALENCE_CERTIFICATE_VERSION:
        return f"certificate version is not {EQUIVALENCE_CERTIFICATE_VERSION}"
    if certificate.get("status") != "SEMANTIC_CERTIFIED":
        return "certificate status is not SEMANTIC_CERTIFIED"
    if certificate.get("architecture") != ARCHITECTURE_MODEL:
        return f"certificate architecture is not {ARCHITECTURE_MODEL}"
    if certificate.get("result_format") != RESULT_FORMAT:
        return f"certificate result_format is not {RESULT_FORMAT}"
    if certificate.get("target_id") != row.get("id"):
        return "certificate target_id does not match target"
    if certificate.get("evidence") not in {"symbolic-equivalence", "full-instruction-match"}:
        return "certificate evidence is not recognized"
    for name in ("retail_sha256", "candidate_sha256", "certificate_sha256"):
        value = certificate.get(name)
        if not isinstance(value, str) or re.fullmatch(r"[0-9a-f]{64}", value) is None:
            return f"certificate {name} is not a lowercase SHA-256"
    engine_hash = certificate.get("engine_hash")
    if not isinstance(engine_hash, str) or re.fullmatch(r"[0-9a-f]{64}", engine_hash) is None:
        return "certificate engine_hash is missing or not a lowercase SHA-256"
    expected_engine = hash_engine_tree(_REPO_ROOT)
    if engine_hash != expected_engine:
        return "certificate engine_hash does not match current engine tree"
    certifier_hash = certificate.get("certifier_hash")
    if not isinstance(certifier_hash, str) or re.fullmatch(r"[0-9a-f]{64}", certifier_hash) is None:
        return "certificate certifier_hash is missing or not a lowercase SHA-256"
    expected_certifier = hash_certifier_tree(_REPO_ROOT)
    if certifier_hash != expected_certifier:
        return "certificate certifier_hash does not match current certifier tree"
    summary = certificate.get("summary")
    if not isinstance(summary, dict):
        return "certificate summary is not an object"
    for name in ("reads", "writes"):
        value = summary.get(name)
        if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
            return f"certificate summary.{name} is not a string array"
    if summary.get("return_behavior") != "normal":
        return "certificate does not guarantee normal return"
    if certificate.get("certificate_sha256") != equivalence_certificate_hash(certificate):
        return "certificate_sha256 does not match certificate payload"

    proof_features_error = validate_proof_features(
        certificate,
        require_equivalent_ready=row.get("status") == "EQUIVALENT_MATCH",
    )
    if proof_features_error is not None:
        return f"certificate proof_features invalid: {proof_features_error}"

    raw_assurance = certificate.get("capability_assurance")
    if raw_assurance is not None:
        try:
            from tools.ppc_equivalence.capability_assurance import CapabilityAssurance

            CapabilityAssurance.from_dict(raw_assurance).validate_structure()
        except (TypeError, ValueError, KeyError) as exc:
            return f"certificate capability_assurance invalid: {exc}"

    dependencies = certificate.get("callees")
    if not isinstance(dependencies, list):
        return "certificate callees is not an array"
    helpers = certificate.get("helpers")
    if not isinstance(helpers, list) or not all(isinstance(item, str) for item in helpers):
        return "certificate helpers is not a string array"
    target_id = str(row.get("id", ""))
    checking = set(_checking or ())
    if target_id in checking:
        return "certificate dependency cycle"
    checking.add(target_id)
    for dependency in dependencies:
        if not isinstance(dependency, dict):
            return "certificate callee entry is not an object"
        callee_id = dependency.get("target_id")
        expected_hash = dependency.get("certificate_sha256")
        if not isinstance(callee_id, str) or not isinstance(expected_hash, str):
            return "certificate callee entry needs target_id and certificate_sha256"
        callee = rows_by_id.get(callee_id)
        if callee is None:
            return f"certificate refers to unknown callee {callee_id!r}"
        actual = callee.get("equivalence_certificate")
        if not isinstance(actual, dict) or actual.get("certificate_sha256") != expected_hash:
            return f"callee certificate changed for {callee_id!r}"
        error = equivalence_certificate_error(callee, rows_by_id, _checking=checking)
        if error:
            return f"callee {callee_id!r}: {error}"
    return None


@dataclass
class Target:
    id: str
    tier: str
    milestone: str
    function: str
    symbol: Optional[str]
    address: Optional[str]
    source: Optional[Path]
    unit: Optional[str]
    required_level: str
    region: Optional[str] = None
    kind: str = "function"
    workflow_status: str = "BACKLOG"
    notes: str = ""
    status: str = "NOT_STARTED"
    extra: Dict[str, Any] = field(default_factory=dict)

    @property
    def buildable(self) -> bool:
        return self.source is not None and self.unit is not None and self.kind == "function"


def targets_path(config: CoopConfig) -> Path:
    return config.resolve(config.targets_file)


def load_targets_document(config: CoopConfig) -> Dict[str, Any]:
    path = targets_path(config)
    if not path.is_file():
        raise FileNotFoundError(f"Targets file not found: {path}")
    if path.suffix in {".yaml", ".yml"}:
        return _load_yaml(path)
    with path.open(encoding="utf-8") as f:
        return json.load(f)


def write_targets_document(config: CoopConfig, data: Dict[str, Any]) -> Path:
    path = targets_path(config)
    if path.suffix in {".yaml", ".yml"}:
        raise ValueError("Symbol import currently writes JSON target registries only")
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    return path


def update_target_result(
    config: CoopConfig,
    target_id: str,
    *,
    status: str,
    instruction_match: Optional[float],
    equivalence_status: Optional[str] = None,
    equivalence_certificate: Optional[Dict[str, Any]] = None,
    certificate_checked: bool = False,
    equivalence_confidence: Optional[str] = None,
    equivalence_policy: Optional[str] = None,
) -> Path:
    """Persist the latest result so the registry remains current state."""
    data = load_targets_document(config)
    for row in data.get("targets", []):
        if row.get("id") != target_id:
            continue
        row["status"] = status
        if instruction_match is not None:
            row["instruction_match"] = round(float(instruction_match), 3)
        if equivalence_status:
            row["equivalence_status"] = equivalence_status
        if certificate_checked:
            if equivalence_certificate is None:
                row.pop("equivalence_certificate", None)
            else:
                row["equivalence_certificate"] = equivalence_certificate
        if status == "EQUIVALENT_MATCH":
            if equivalence_confidence is not None:
                row["equivalence_confidence"] = equivalence_confidence
            if equivalence_policy is not None:
                row["equivalence_policy"] = equivalence_policy
        else:
            row.pop("equivalence_confidence", None)
            row.pop("equivalence_policy", None)
        if status in {"FULL_MATCH", "EQUIVALENT_MATCH"}:
            row["workflow_status"] = "ACCEPTED"
        elif row.get("workflow_status") in {None, "BACKLOG", "QUEUED", "CLAIMED"}:
            row["workflow_status"] = "ACTIVE"
        return write_targets_document(config, data)
    raise KeyError(f"Unknown target id: {target_id}")


def sync_results_from_attempts(config: CoopConfig, attempts: Iterable[Dict[str, Any]]) -> int:
    """Migrate latest append-only attempt results into the canonical registry."""
    latest: Dict[str, Dict[str, Any]] = {}
    for attempt in attempts:
        target_id = attempt.get("target_id")
        if target_id:
            latest[str(target_id)] = attempt
    data = load_targets_document(config)
    changed = 0
    for row in data.get("targets", []):
        attempt = latest.get(str(row.get("id")))
        if not attempt:
            continue
        before = (
            row.get("status"), row.get("workflow_status"), row.get("instruction_match"),
            row.get("equivalence_status"),
        )
        status = str(attempt.get("status", row.get("status", "NOT_STARTED")))
        row["status"] = status
        if attempt.get("instruction_match") is not None:
            row["instruction_match"] = round(float(attempt["instruction_match"]), 3)
        if attempt.get("equivalence_status"):
            row["equivalence_status"] = attempt["equivalence_status"]
        if status in {"FULL_MATCH", "EQUIVALENT_MATCH"}:
            row["workflow_status"] = "ACCEPTED"
        elif row.get("workflow_status") in {None, "BACKLOG", "QUEUED", "CLAIMED"}:
            row["workflow_status"] = "ACTIVE"
        after = (
            row.get("status"), row.get("workflow_status"), row.get("instruction_match"),
            row.get("equivalence_status"),
        )
        changed += before != after
    if changed:
        write_targets_document(config, data)
    return changed


def claim_target(
    config: CoopConfig,
    target_id: str,
    *,
    owner: str,
    allowed_paths: List[str],
    note: str = "",
) -> Path:
    data = load_targets_document(config)
    for row in data.get("targets", []):
        if row.get("id") != target_id:
            continue
        claim = row.get("claim")
        if isinstance(claim, dict) and claim.get("owner") and claim.get("owner") != owner:
            raise ValueError(
                f"target {target_id!r} is already claimed by {claim['owner']!r}; release it first"
            )
        row["claim"] = {
            "owner": owner,
            "claimed_at": datetime.now(timezone.utc).isoformat(),
            "allowed_paths": allowed_paths,
            "note": note,
        }
        if row.get("workflow_status") not in {"ACCEPTED", "NOT_REQUIRED"}:
            row["workflow_status"] = "CLAIMED"
        return write_targets_document(config, data)
    raise KeyError(f"Unknown target id: {target_id}")


def release_target(config: CoopConfig, target_id: str, *, owner: Optional[str]) -> Path:
    data = load_targets_document(config)
    for row in data.get("targets", []):
        if row.get("id") != target_id:
            continue
        claim = row.get("claim")
        if not isinstance(claim, dict):
            raise ValueError(f"target {target_id!r} is not claimed")
        current_owner = claim.get("owner")
        if owner and current_owner != owner:
            raise ValueError(
                f"target {target_id!r} is claimed by {current_owner!r}, not {owner!r}"
            )
        row.pop("claim", None)
        if row.get("workflow_status") == "CLAIMED":
            row["workflow_status"] = (
                "ACCEPTED"
                if row.get("status") in {"FULL_MATCH", "EQUIVALENT_MATCH"}
                else "BACKLOG"
            )
        return write_targets_document(config, data)
    raise KeyError(f"Unknown target id: {target_id}")


def load_targets(config: CoopConfig) -> List[Target]:
    data = load_targets_document(config)
    targets: List[Target] = []
    known = {
        "id", "tier", "priority", "milestone", "function", "symbol", "address",
        "source", "unit", "required_level", "region", "kind", "workflow_status",
        "tracking", "notes", "status",
    }
    for item in data.get("targets", []):
        source = item.get("source")
        targets.append(
            Target(
                id=item["id"],
                tier=item.get("tier", item.get("priority", "P9")),
                milestone=item.get("milestone", "unassigned"),
                function=item.get("function", item.get("symbol", item["id"])),
                symbol=item.get("symbol"),
                address=item.get("address"),
                source=config.resolve(source) if source else None,
                unit=item.get("unit"),
                required_level=item.get(
                    "required_level",
                    data.get("default_required_level", config.default_required_level),
                ),
                region=item.get("region", config.region),
                kind=item.get("kind", "function"),
                workflow_status=item.get("workflow_status", item.get("tracking", "BACKLOG")),
                notes=item.get("notes", ""),
                status=item.get("status", "NOT_STARTED"),
                extra={key: value for key, value in item.items() if key not in known},
            )
        )
    return targets


def get_target(targets: List[Target], target_id: str) -> Target:
    for target in targets:
        if target.id == target_id:
            return target
    raise KeyError(f"Unknown target id: {target_id}")


def pending_targets(targets: List[Target], tier: Optional[str] = None) -> List[Target]:
    done = {"FULL_MATCH", "EQUIVALENT_MATCH"}
    closed = {"ACCEPTED", "NOT_REQUIRED"}
    result = [
        target for target in targets
        if target.buildable
        and target.extra.get("origin") != "symbols.txt"
        and target.status not in done
        and target.workflow_status not in closed
    ]
    if tier:
        result = [target for target in result if target.tier == tier]
    tier_order = {"P0": 0, "P1": 1, "P2": 2, "P3": 3}
    result.sort(key=lambda target: (tier_order.get(target.tier, 99), target.id))
    return result


def _target_registry_rows(targets: List[Target]) -> Dict[str, Dict[str, Any]]:
    """Build certificate-validation rows from loaded Target objects."""
    return {
        target.id: {
            "id": target.id,
            "status": target.status,
            "symbol": target.symbol,
            "unit": target.unit,
            "address": target.address,
            **target.extra,
        }
        for target in targets
    }


def _certified_target_ids(
    targets: List[Target],
    rows_by_id: Optional[Dict[str, Dict[str, Any]]] = None,
) -> set[str]:
    rows = rows_by_id if rows_by_id is not None else _target_registry_rows(targets)
    return {
        target.id
        for target in targets
        if target.status in ACCEPTED_MATCH_STATUSES
        and equivalence_certificate_error(rows[target.id], rows) is None
    }


def _callgraph_blocks_certification(target: Target) -> Optional[str]:
    """Return why a target cannot safely receive a compositional certificate."""
    if target.extra.get("callgraph_status") != "complete":
        return "call graph incomplete"
    if target.extra.get("has_indirect_calls"):
        return "has unresolved indirect calls"
    unresolved = target.extra.get("unresolved_called_functions", [])
    if unresolved:
        return "has unresolved callees: " + ", ".join(str(item) for item in unresolved)
    called = target.extra.get("called_functions", [])
    if not isinstance(called, list) or not all(isinstance(item, str) for item in called):
        return "called_functions is not an array of target ids"
    return None


@dataclass(frozen=True)
class RecertifyPlan:
    """Bottom-up certificate refresh plan for accepted targets."""

    ordered: List[Target]
    blocked: List[Target]
    reasons: Dict[str, str]
    block_reasons: Dict[str, str]


def plan_recertify_bottom_up(
    targets: List[Target],
    *,
    include_catalog: bool = False,
) -> RecertifyPlan:
    """Order accepted targets that lack a current semantic certificate.

    Leaves and targets whose direct callees are already certified come first.
    When projecting the dry-run order, each queued target is treated as becoming
    certified so dependents can appear later in the same plan. Targets that
    remain blocked (incomplete call graph, uncertified callees outside the
    queue, cycles) are returned separately.
    """
    rows_by_id = _target_registry_rows(targets)
    certified_ids = _certified_target_ids(targets, rows_by_id)
    reasons: Dict[str, str] = {}
    need: Dict[str, Target] = {}
    for target in targets:
        if target.status not in ACCEPTED_MATCH_STATUSES:
            continue
        if not target.buildable:
            continue
        if not include_catalog and target.extra.get("origin") == "symbols.txt":
            continue
        cert_error = equivalence_certificate_error(rows_by_id[target.id], rows_by_id)
        if cert_error is None:
            continue
        need[target.id] = target
        reasons[target.id] = cert_error

    ordered: List[Target] = []
    remaining = dict(need)
    projected_certified = set(certified_ids)
    while remaining:
        wave = []
        for target in remaining.values():
            block = _callgraph_blocks_certification(target)
            if block:
                continue
            called = target.extra.get("called_functions", [])
            if all(callee_id in projected_certified for callee_id in called):
                wave.append(target)
        if not wave:
            break
        wave.sort(key=lambda item: item.id)
        for target in wave:
            ordered.append(target)
            del remaining[target.id]
            projected_certified.add(target.id)

    blocked: List[Target] = []
    block_reasons: Dict[str, str] = {}
    for target in sorted(remaining.values(), key=lambda item: item.id):
        block = _callgraph_blocks_certification(target)
        if block is None:
            called = target.extra.get("called_functions", [])
            missing = [
                callee_id for callee_id in called if callee_id not in projected_certified
            ]
            if missing:
                block = "waiting on uncertified callees: " + ", ".join(missing)
            else:
                block = "dependency cycle or ordering stuck"
        blocked.append(target)
        block_reasons[target.id] = block

    return RecertifyPlan(
        ordered=ordered,
        blocked=blocked,
        reasons=reasons,
        block_reasons=block_reasons,
    )


def recertify_ready_wave(
    targets: List[Target],
    *,
    include_catalog: bool = False,
    skip_ids: Optional[set[str]] = None,
) -> List[Target]:
    """Return accepted targets that can be certified with the current registry."""
    skip = skip_ids or set()
    rows_by_id = _target_registry_rows(targets)
    certified_ids = _certified_target_ids(targets, rows_by_id)
    wave: List[Target] = []
    for target in targets:
        if target.id in skip:
            continue
        if target.status not in ACCEPTED_MATCH_STATUSES or not target.buildable:
            continue
        if not include_catalog and target.extra.get("origin") == "symbols.txt":
            continue
        if equivalence_certificate_error(rows_by_id[target.id], rows_by_id) is None:
            continue
        if _callgraph_blocks_certification(target) is not None:
            continue
        called = target.extra.get("called_functions", [])
        if all(callee_id in certified_ids for callee_id in called):
            wave.append(target)
    wave.sort(key=lambda item: item.id)
    return wave


def harness_targets(
    targets: List[Target],
    *,
    selection: str,
    tier: Optional[str] = None,
    include_catalog: bool = False,
) -> List[Target]:
    """Select a safe bottom-up call-graph frontier for the cycle harness."""
    rows_by_id = _target_registry_rows(targets)
    certified_ids = _certified_target_ids(targets, rows_by_id)
    candidates = [
        target
        for target in targets
        if target.buildable
        and target.status not in ACCEPTED_MATCH_STATUSES
        and target.workflow_status not in {"ACCEPTED", "NOT_REQUIRED", "BLOCKED"}
        and (include_catalog or target.extra.get("origin") != "symbols.txt")
    ]
    if tier:
        candidates = [target for target in candidates if target.tier == tier]

    def is_leaf(target: Target) -> bool:
        return (
            target.extra.get("callgraph_status") == "complete"
            and not target.extra.get("called_functions", [])
            and not target.extra.get("unresolved_called_functions", [])
            and not target.extra.get("has_indirect_calls", False)
        )

    def has_accepted_callees(target: Target) -> bool:
        called = target.extra.get("called_functions", [])
        return (
            target.extra.get("callgraph_status") == "complete"
            and bool(called)
            and not target.extra.get("unresolved_called_functions", [])
            and not target.extra.get("has_indirect_calls", False)
            and all(target_id in certified_ids for target_id in called)
        )

    if selection == "leaf":
        candidates = [target for target in candidates if is_leaf(target)]
    elif selection == "callees-accepted":
        candidates = [target for target in candidates if has_accepted_callees(target)]
    elif selection == "ready":
        candidates = [
            target for target in candidates if is_leaf(target) or has_accepted_callees(target)
        ]
    elif selection != "pending":
        raise ValueError(f"Unknown harness selection: {selection}")

    tier_order = {"P0": 0, "P1": 1, "P2": 2, "P3": 3, "P9": 9}
    candidates.sort(key=lambda target: (tier_order.get(target.tier, 99), target.id))
    return candidates


def validate_targets(config: CoopConfig) -> List[str]:
    data = load_targets_document(config)
    errors: List[str] = []
    if not isinstance(data.get("schema_version"), int):
        errors.append("top-level schema_version must be an integer")
    rows = data.get("targets")
    if not isinstance(rows, list):
        return [*errors, "top-level targets must be an array"]

    ids: Dict[str, int] = {}
    identities: Dict[tuple[str, str, Optional[str]], int] = {}
    for index, row in enumerate(rows, start=1):
        label = f"targets[{index - 1}]"
        if not isinstance(row, dict):
            errors.append(f"{label} must be an object")
            continue
        for field_name in ("id", "symbol", "address"):
            if field_name not in row:
                errors.append(f"{label} is missing {field_name}")
        target_id = row.get("id")
        if isinstance(target_id, str):
            if target_id in ids:
                errors.append(f"duplicate id {target_id!r} at rows {ids[target_id]} and {index}")
            ids[target_id] = index
        region = str(row.get("region", config.region))
        symbol = row.get("symbol")
        if isinstance(symbol, str):
            identity = (region, symbol, row.get("address"))
            if identity in identities:
                errors.append(
                    f"duplicate symbol identity {region}:{symbol}@{row.get('address')} "
                    f"at rows {identities[identity]} and {index}"
                )
            identities[identity] = index
        status = row.get("status", "NOT_STARTED")
        if status not in MATCH_STATUSES:
            errors.append(f"{label} has unknown match status {status!r}")
        workflow = row.get("workflow_status", row.get("tracking", "BACKLOG"))
        if workflow not in WORKFLOW_STATUSES:
            errors.append(f"{label} has unknown workflow_status {workflow!r}")
    known_ids = set(ids)
    rows_by_id = {
        str(row.get("id")): row for row in rows
        if isinstance(row, dict) and isinstance(row.get("id"), str)
    }
    for index, row in enumerate(rows):
        if not isinstance(row, dict):
            continue
        called = row.get("called_functions", [])
        if not isinstance(called, list) or not all(isinstance(value, str) for value in called):
            errors.append(f"targets[{index}].called_functions must be an array of target IDs")
            continue
        for called_id in called:
            if called_id not in known_ids:
                errors.append(f"targets[{index}] calls unknown target id {called_id!r}")
        # Certificates are acceptance evidence only for EQUIVALENT_MATCH.
        # FULL_MATCH / downgraded rows may retain historical certificates.
        if row.get("status") == "EQUIVALENT_MATCH" and "equivalence_certificate" in row:
            error = equivalence_certificate_error(row, rows_by_id)
            if error:
                errors.append(f"targets[{index}].equivalence_certificate: {error}")
    return errors


def equivalence_certificate_staleness_error(
    row: Dict[str, Any],
    rows_by_id: Dict[str, Dict[str, Any]],
    *,
    reject_architecture_models: Iterable[str] = (),
) -> Optional[str]:
    """Return why an equivalence certificate is stale relative to live constants."""
    certificate = row.get("equivalence_certificate")
    if not isinstance(certificate, dict):
        return "missing equivalence_certificate"
    error = equivalence_certificate_error(row, rows_by_id)
    if error:
        return error
    architecture = certificate.get("architecture")
    if isinstance(architecture, str) and architecture in reject_architecture_models:
        return f"architecture model {architecture!r} is rejected"
    return None


def equivalence_certificate_migration_report(
    config: CoopConfig,
) -> Dict[str, Any]:
    """Enumerate EQUIVALENT_MATCH rows with stale or missing equivalence certificates.

    FULL_MATCH rows are counted separately and never listed as affected.
    """
    data = load_targets_document(config)
    rows: List[Dict[str, Any]] = list(data.get("targets", []))
    rows_by_id = {
        str(row["id"]): row
        for row in rows
        if isinstance(row, dict) and isinstance(row.get("id"), str)
    }
    reject_models = set(config.reject_architecture_models)

    valid: List[Dict[str, Any]] = []
    stale: List[Dict[str, Any]] = []
    no_certificate: List[Dict[str, Any]] = []
    skipped_full_match = 0
    skipped_full_match_stale_cert = 0

    for row in rows:
        if not isinstance(row, dict):
            continue
        status = row.get("status", "NOT_STARTED")
        target_id = str(row.get("id", ""))

        if status == "FULL_MATCH":
            skipped_full_match += 1
            certificate = row.get("equivalence_certificate")
            if isinstance(certificate, dict):
                reason = equivalence_certificate_staleness_error(
                    row, rows_by_id, reject_architecture_models=reject_models
                )
                if reason:
                    skipped_full_match_stale_cert += 1
            continue

        if status != "EQUIVALENT_MATCH":
            continue

        certificate = row.get("equivalence_certificate")
        if not isinstance(certificate, dict):
            no_certificate.append(
                {
                    "id": target_id,
                    "status": status,
                    "workflow_status": row.get("workflow_status", "unknown"),
                    "certificate_error": "missing equivalence_certificate",
                }
            )
            continue

        reason = equivalence_certificate_staleness_error(
            row, rows_by_id, reject_architecture_models=reject_models
        )
        entry = {
            "id": target_id,
            "status": status,
            "workflow_status": row.get("workflow_status", "unknown"),
            "certificate_error": reason,
            "architecture": certificate.get("architecture"),
            "result_format": certificate.get("result_format"),
            "certificate_version": certificate.get("version"),
        }
        if reason:
            stale.append(entry)
        else:
            valid.append(entry)

    return {
        "architecture_model": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "equivalence_certificate_version": EQUIVALENCE_CERTIFICATE_VERSION,
        "reject_architecture_models": sorted(reject_models),
        "valid_count": len(valid),
        "stale_count": len(stale),
        "no_certificate_count": len(no_certificate),
        "skipped_full_match_count": skipped_full_match,
        "skipped_full_match_stale_cert_count": skipped_full_match_stale_cert,
        "affected_count": len(stale) + len(no_certificate),
        "valid": valid,
        "stale": stale,
        "no_certificate": no_certificate,
    }


def audit_promotion_registry(
    config: CoopConfig,
    *,
    apply: bool = False,
) -> Dict[str, Any]:
    """Audit EQUIVALENT_MATCH rows; optionally mark stale proofs for revalidation.

    Does not downgrade FULL_MATCH. Idempotent: already-downgraded rows are skipped.
    Historical evidence is preserved in the append-only attempt log on apply.
    """
    from tools.coop.lib.attempts import AttemptRecord, append_attempt
    from tools.coop.lib.equivalence_policy import (
        PromotionPolicy,
        ValidationLedger,
        classify_for_promotion,
        default_validation_ledger_path,
        proof_result_from_certificate,
    )
    from tools.ppc_equivalence.result import ProofStatus

    data = load_targets_document(config)
    rows: List[Dict[str, Any]] = list(data.get("targets", []))
    rows_by_id = {
        str(row["id"]): row
        for row in rows
        if isinstance(row, dict) and isinstance(row.get("id"), str)
    }
    policy = PromotionPolicy.from_config(config)
    ledger = ValidationLedger.load(default_validation_ledger_path())
    affected: List[Dict[str, Any]] = []
    valid_count = 0
    skipped_full_match = 0

    for row in rows:
        if not isinstance(row, dict):
            continue
        status = row.get("status", "NOT_STARTED")
        if status == "FULL_MATCH":
            skipped_full_match += 1
            continue
        if status != "EQUIVALENT_MATCH":
            continue

        target_id = str(row["id"])
        cert_error = equivalence_certificate_error(row, rows_by_id)
        blockers: tuple[str, ...] = ()
        confidence: Optional[str] = None
        action = "require-revalidation"

        if cert_error:
            reason = cert_error
        else:
            certificate = row.get("equivalence_certificate")
            proof = proof_result_from_certificate(
                ProofStatus.EQUIVALENT,
                certificate if isinstance(certificate, dict) else None,
            )
            decision = classify_for_promotion(proof, policy, ledger)
            confidence = decision.confidence_tier
            if decision.allowed:
                valid_count += 1
                continue
            blockers = decision.blockers
            reason = ",".join(blockers) if blockers else "promotion-policy-rejected"

        affected.append(
            {
                "id": target_id,
                "status": status,
                "workflow_status": row.get("workflow_status", "unknown"),
                "certificate_error": reason,
                "blockers": list(blockers),
                "confidence_tier": confidence,
                "action": action,
                "instruction_match": row.get("instruction_match"),
            }
        )

    applied: List[Dict[str, Any]] = []
    if apply and affected:
        attempt_log = config.resolve(config.attempt_log)
        for entry in affected:
            row = rows_by_id[entry["id"]]
            prior_status = row.get("status")
            prior_workflow = row.get("workflow_status")
            prior_cert = row.get("equivalence_certificate")
            cert_hash = None
            if isinstance(prior_cert, dict):
                cert_hash = prior_cert.get("certificate_sha256")

            append_attempt(
                attempt_log,
                AttemptRecord(
                    target_id=entry["id"],
                    function=str(row.get("function") or row.get("symbol") or entry["id"]),
                    region=str(row.get("region") or config.region),
                    unit=str(row.get("unit") or ""),
                    symbol=row.get("symbol"),
                    status="CODE_MATCH",
                    instruction_match=(
                        float(row["instruction_match"])
                        if row.get("instruction_match") is not None
                        else None
                    ),
                    relocation_match=None,
                    code_match_percent=None,
                    data_match_percent=None,
                    hypothesis=(
                        "audit-promotion: revalidation required — "
                        + str(entry["certificate_error"])
                    ),
                    next_change="Re-prove under current architecture model and promotion policy",
                    equivalence_status=row.get("equivalence_status"),
                    equivalence_detail=(
                        f"prior_status={prior_status}; prior_workflow={prior_workflow}; "
                        f"certificate_sha256={cert_hash}"
                    ),
                    equivalence_confidence=row.get("equivalence_confidence"),
                    equivalence_policy=row.get("equivalence_policy"),
                ),
            )
            row["status"] = "CODE_MATCH"
            row["workflow_status"] = "REVALIDATION_REQUIRED"
            row.pop("equivalence_confidence", None)
            row.pop("equivalence_policy", None)
            applied.append(
                {
                    "id": entry["id"],
                    "from_status": prior_status,
                    "to_status": "CODE_MATCH",
                    "from_workflow": prior_workflow,
                    "to_workflow": "REVALIDATION_REQUIRED",
                    "reason": entry["certificate_error"],
                }
            )
        write_targets_document(config, data)

    return {
        "architecture_model": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "equivalence_policy": EQUIVALENCE_PROMOTION_POLICY,
        "reject_architecture_models": list(config.reject_architecture_models),
        "automatic_promotion": config.automatic_promotion,
        "allowed_confidence_tiers": sorted(config.allowed_confidence_tiers),
        "valid_count": valid_count,
        "affected_count": len(affected),
        "skipped_full_match": skipped_full_match,
        "applied": apply,
        "applied_count": len(applied),
        "affected": affected,
        "mutations": applied,
    }


@dataclass(frozen=True)
class SplitRange:
    unit: str
    section: str
    start: int
    end: int


_SPLIT_UNIT_RE = re.compile(r"^(?P<unit>[^\s].*):$")
_SPLIT_RANGE_RE = re.compile(
    r"^\s*(?P<section>\S+)\s+start:0x(?P<start>[0-9A-Fa-f]+)\s+end:0x(?P<end>[0-9A-Fa-f]+)"
)


def load_split_ranges(path: Path) -> List[SplitRange]:
    ranges: List[SplitRange] = []
    current_unit: Optional[str] = None
    for line in path.read_text(encoding="utf-8").splitlines():
        unit_match = _SPLIT_UNIT_RE.match(line)
        if unit_match and not line.startswith((" ", "\t")) and line != "Sections:":
            current_unit = unit_match.group("unit")
            continue
        range_match = _SPLIT_RANGE_RE.match(line)
        if current_unit and range_match:
            ranges.append(
                SplitRange(
                    unit=current_unit,
                    section=range_match.group("section"),
                    start=int(range_match.group("start"), 16),
                    end=int(range_match.group("end"), 16),
                )
            )
    return ranges


def _unit_for_symbol(symbol: SymbolEntry, ranges: Iterable[SplitRange]) -> Optional[str]:
    for item in ranges:
        if item.section == symbol.section and item.start <= symbol.address < item.end:
            unit = str(Path(item.unit).with_suffix(""))
            return unit
    return None


def _display_name(symbol: str) -> str:
    info = demangle_symbol(symbol)
    function = info.function
    if info.is_ctor and info.class_name:
        function = info.class_name
    elif info.is_dtor and info.class_name:
        function = f"~{info.class_name}"
    parts = [part for part in (info.namespace, info.class_name, function) if part]
    if not parts or (len(parts) == 1 and parts[0] == symbol):
        return symbol
    return "::".join(parts) + (info.args or "")


def _source_maps(project: Any) -> tuple[Dict[str, str], Dict[str, str]]:
    source_by_unit: Dict[str, str] = {}
    objdiff_by_unit: Dict[str, str] = {}
    try:
        units = project.load_objdiff_units()
    except FileNotFoundError:
        return source_by_unit, objdiff_by_unit
    for unit in units:
        canonical = unit.name.removeprefix("main/")
        objdiff_by_unit[canonical] = canonical
        if unit.source_path:
            source_by_unit[canonical] = str(unit.source_path.relative_to(project.root))
    return source_by_unit, objdiff_by_unit


def import_symbols(
    project: Any,
    config: CoopConfig,
    *,
    kind: str = "function",
) -> tuple[Dict[str, Any], int, int]:
    """Return an updated registry plus (added, skipped) counts.

    Curated records are never replaced. Imported IDs are address-based so a later
    semantic rename does not create a second target.
    """
    data = load_targets_document(config)
    data.setdefault("schema_version", 2)
    rows = data.setdefault("targets", [])
    for row in rows:
        if row.get("origin") == "symbols.txt" and row.get("workflow_status") == "BACKLOG":
            row["workflow_status"] = "DISCOVERY"
    symbols_path = project.root / "config" / config.region / "symbols.txt"
    splits_path = project.root / "config" / config.region / "splits.txt"
    symbols = load_symbols(symbols_path)
    ranges = load_split_ranges(splits_path) if splits_path.is_file() else []
    source_by_unit, _ = _source_maps(project)

    existing_symbols: Dict[tuple[str, Any], set[Optional[str]]] = {}
    for row in rows:
        key = (str(row.get("region", config.region)), row.get("symbol"))
        existing_symbols.setdefault(key, set()).add(row.get("address"))
    existing_ids = {row["id"] for row in rows if isinstance(row, dict) and "id" in row}
    added = 0
    skipped = 0
    for entry in symbols:
        symbol_kind = entry.sym_type or "unknown"
        if kind != "all" and symbol_kind != kind:
            continue
        identity = (config.region, entry.name)
        known_addresses = existing_symbols.get(identity, set())
        if entry.address_hex in known_addresses or None in known_addresses:
            skipped += 1
            continue
        base_id = f"{config.region}-{entry.address:08x}"
        target_id = base_id
        suffix = 2
        while target_id in existing_ids:
            target_id = f"{base_id}-{suffix}"
            suffix += 1
        unit = _unit_for_symbol(entry, ranges)
        row: Dict[str, Any] = {
            "id": target_id,
            "region": config.region,
            "kind": symbol_kind,
            "tier": "P9",
            "milestone": "unassigned",
            "function": _display_name(entry.name),
            "symbol": entry.name,
            "address": entry.address_hex,
            "size": f"0x{entry.size:X}" if entry.size is not None else None,
            "unit": unit,
            "required_level": data.get("default_required_level", config.default_required_level),
            "workflow_status": "DISCOVERY" if symbol_kind == "function" else "NOT_REQUIRED",
            "status": "NOT_STARTED",
            "origin": "symbols.txt",
        }
        if unit and unit in source_by_unit:
            row["source"] = source_by_unit[unit]
        rows.append(row)
        existing_symbols.setdefault(identity, set()).add(entry.address_hex)
        existing_ids.add(target_id)
        added += 1
    rows.sort(key=lambda row: (str(row.get("region", config.region)), str(row.get("address") or ""), row["id"]))
    return data, added, skipped


@dataclass
class FunctionCalls:
    symbol: str
    address: Optional[int]
    direct: List[tuple[Optional[str], int]] = field(default_factory=list)
    has_indirect: bool = False


_ASM_FN_RE = re.compile(r'^\.fn\s+"?(?P<symbol>[^",\s]+)"?\s*,')
_ASM_ENDFN_RE = re.compile(r"^\.endfn\b")
_ABI_HELPER_RE = re.compile(r"^_(?:save|rest)(?:gpr|fpr)_\d+$")
_ASM_INSN_RE = re.compile(
    r"^/\*\s*(?P<address>[0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s+"
    r"(?P<bytes>(?:[0-9A-Fa-f]{2}\s+){3}[0-9A-Fa-f]{2})\s*\*/\s*"
    r"(?P<mnemonic>\S+)(?:\s+(?P<operand>\S+))?"
)


def _branch_destination(address: int, word: int) -> int:
    displacement = word & 0x03FFFFFC
    if displacement & 0x02000000:
        displacement -= 0x04000000
    if word & 0x2:  # AA: absolute address
        return displacement & 0xFFFFFFFF
    return (address + displacement) & 0xFFFFFFFF


def parse_asm_calls(path: Path) -> List[FunctionCalls]:
    functions: List[FunctionCalls] = []
    current: Optional[FunctionCalls] = None
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        fn_match = _ASM_FN_RE.match(line)
        if fn_match:
            current = FunctionCalls(symbol=fn_match.group("symbol"), address=None)
            continue
        if current is None:
            continue
        if _ASM_ENDFN_RE.match(line):
            functions.append(current)
            current = None
            continue
        instruction = _ASM_INSN_RE.match(line)
        if not instruction:
            continue
        address = int(instruction.group("address"), 16)
        if current.address is None:
            current.address = address
        mnemonic = instruction.group("mnemonic")
        if mnemonic == "bl":
            word = int(instruction.group("bytes").replace(" ", ""), 16)
            operand = (instruction.group("operand") or "").split("+")[0]
            current.direct.append((operand or None, _branch_destination(address, word)))
        elif mnemonic in {"bctrl", "blrl"}:
            current.has_indirect = True
    return functions


def sync_called_functions(
    project: Any,
    config: CoopConfig,
) -> tuple[Dict[str, Any], int, int, int]:
    """Populate direct call edges from generated retail assembly.

    Returns the updated document and counts for scanned functions, resolved
    direct edges, and unresolved direct edges.
    """
    data = load_targets_document(config)
    rows = data.get("targets", [])
    by_address: Dict[tuple[str, str], List[Dict[str, Any]]] = {}
    by_symbol: Dict[tuple[str, str], List[Dict[str, Any]]] = {}
    for row in rows:
        if row.get("kind", "function") != "function":
            continue
        region = str(row.get("region", config.region))
        address = row.get("address")
        symbol = row.get("symbol")
        if address:
            by_address.setdefault((region, str(address).upper()), []).append(row)
        if symbol:
            by_symbol.setdefault((region, str(symbol)), []).append(row)

    scanned = 0
    resolved_edges = 0
    unresolved_edges = 0
    asm_root = project.root / "build" / config.region / "asm"
    if not asm_root.is_dir():
        raise FileNotFoundError(
            f"Retail assembly not found: {asm_root}; run configure/baseline first"
        )
    for asm_path in sorted(asm_root.rglob("*.s")):
        for function in parse_asm_calls(asm_path):
            caller_rows = (
                by_address.get((config.region, f"0X{function.address:08X}"), [])
                if function.address is not None
                else []
            )
            if not caller_rows:
                caller_rows = by_symbol.get((config.region, function.symbol), [])
            if not caller_rows:
                continue
            called_ids: set[str] = set()
            unresolved: set[str] = set()
            abi_helpers: set[str] = set()
            for operand, destination in function.direct:
                if operand and _ABI_HELPER_RE.match(operand):
                    abi_helpers.add(operand)
                    continue
                destination_rows = by_address.get(
                    (config.region, f"0X{destination:08X}"), []
                )
                if not destination_rows and operand:
                    destination_rows = by_symbol.get((config.region, operand), [])
                if operand and len(destination_rows) > 1:
                    exact = [row for row in destination_rows if row.get("symbol") == operand]
                    if exact:
                        destination_rows = exact
                if destination_rows:
                    called_ids.add(str(destination_rows[0]["id"]))
                    resolved_edges += 1
                else:
                    unresolved.add(operand or f"0x{destination:08X}")
                    unresolved_edges += 1
            for row in caller_rows:
                row["called_functions"] = sorted(called_ids)
                row["unresolved_called_functions"] = sorted(unresolved)
                row["abi_helper_calls"] = sorted(abi_helpers)
                row["has_indirect_calls"] = function.has_indirect
                row["callgraph_status"] = "complete"
                row["callgraph_source"] = str(asm_path.relative_to(project.root))
                scanned += 1
    data["callgraph"] = {
        "region": config.region,
        "source": f"build/{config.region}/asm",
        "format": 1,
        "direct_calls": "PPC bl destination",
        "indirect_calls_are_unresolved": True,
    }
    return data, scanned, resolved_edges, unresolved_edges

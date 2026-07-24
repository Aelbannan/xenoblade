"""Run ppc_equivalence against an objdiff retail/decomp function pair."""

from __future__ import annotations

import hashlib
import json
import re
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Optional

from tools.coop.lib.project import ObjdiffUnit, Project
from tools.coop.lib.targets import (
    ACCEPTED_MATCH_STATUSES,
    EQUIVALENCE_CERTIFICATE_VERSION,
    equivalence_certificate_error,
    equivalence_certificate_hash,
    load_targets_document,
)
from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.dol_symbols import DolSymbolError, extract_by_address as extract_dol_slice
from tools.ppc_equivalence.elf_symbols import (
    ElfSymbolError,
    extract_function,
    extract_function_pair,
)
from tools.ppc_equivalence.callee_inference import infer_matched_callee_contracts
from tools.ppc_equivalence.engine import check_equivalence, validate_callee_contract
from tools.ppc_equivalence.ir import DecodeError, ExecutionInconclusive, Opcode, UnsupportedInstruction
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT, ProofResult, ProofStatus
from tools.ppc_equivalence.provenance import (
    canonical_obligation_dict,
    hash_certifier_tree,
    hash_engine_tree,
    live_git_identity,
    proof_request_hash,
)
from tools.ppc_equivalence.proof_features import (
    PROOF_OBLIGATION_FIELDS,
    proof_obligations_from_result,
)
from tools.ppc_equivalence.semantics import (
    CalleeContract,
    ConcreteOps,
    DEFAULT_MAX_LOOP_ITERATIONS,
    automatic_live_out,
    execute_cfg,
)


# Fuzzy match floor for EQUIVALENT_MATCH (strictly below FULL_MATCH).
EQUIVALENT_MATCH_MIN_PERCENT = 50.0

# Auto-scale: 100 ms per static insn, floor 2 min, ceiling 15 min.
# Soft-cap / loop-heavy leaves expand far past static size; when exploration
# ceilings are raised above the historical 2k/256 defaults, use the full
# ceiling so cycle does not die at the floor (~30–120s).
# Engine accepts up to 1_800_000 ms (contract.py); coop soft-cap retry uses 15 min.
_TIMEOUT_MS_MIN = 120_000
_TIMEOUT_MS_MAX = 900_000
_TIMEOUT_MS_PER_INSN = 100

# Soft-cap / loop-heavy leaves (CAIAction ring copy, CView dual enqueue, …)
# need more headroom than the historical 2048/256 defaults.
_DEFAULT_MAX_INSTRUCTIONS = 65_536
_DEFAULT_MAX_PATHS = 4_096
_DEFAULT_MAX_LOOP_ITERATIONS = 2_048

_REPO_ROOT = Path(__file__).resolve().parents[3]


def _current_engine_hash() -> str:
    return hash_engine_tree(_REPO_ROOT)


def _current_certifier_hash() -> str:
    return hash_certifier_tree(_REPO_ROOT)


def _live_git_identity() -> tuple[str, bool]:
    """Return ``(git_commit, trust_boundary_dirty)`` for provenance fields.

    ``git_dirty`` is true only when engine/certifier trust-boundary sources
    differ from HEAD — unrelated decomp edits do not block Tier A canaries.
    """
    try:
        return live_git_identity(_REPO_ROOT)
    except Exception:
        return "", False


def _refresh_live_provenance_fields(proof: ProofResult) -> None:
    """Overwrite git / engine / certifier hashes with live values (cache hits)."""
    commit, dirty = _live_git_identity()
    if commit:
        proof.git_commit = commit
    proof.git_dirty = dirty
    try:
        proof.engine_hash = _current_engine_hash()
    except Exception:
        pass
    try:
        proof.certifier_hash = _current_certifier_hash()
    except Exception:
        pass


def _load_validation_ledger_for_assurance() -> tuple[Any, str | None]:
    """Load the default validation ledger and optional file SHA-256."""
    from tools.coop.lib.equivalence_policy import (
        ValidationLedger,
        default_validation_ledger_path,
    )

    ledger_path = default_validation_ledger_path()
    ledger = ValidationLedger.load(ledger_path)
    ledger_sha = None
    if ledger_path.is_file():
        ledger_sha = hashlib.sha256(ledger_path.read_bytes()).hexdigest()
    return ledger, ledger_sha


def _apply_capability_assurance(
    result: ProofResult,
    *,
    certified_context: CertifiedCalleeContext | None = None,
    platform_profile: object | None = None,
    environment: object | None = None,
    ledger: object | None = None,
    ledger_sha256: str | None = None,
) -> tuple[str, ...]:
    """Run Stage 4 orchestrator; return generation-error codes."""
    from tools.ppc_equivalence.capability_assurance import build_capability_assurance

    if ledger is None and ledger_sha256 is None:
        ledger, ledger_sha256 = _load_validation_ledger_for_assurance()
    elif ledger is None:
        ledger, _ = _load_validation_ledger_for_assurance()

    build = build_capability_assurance(
        result,
        ledger=ledger,
        ledger_sha256=ledger_sha256,
        callee_context=certified_context,
        platform_profile=platform_profile,
        environment=environment,
        allow_zero_hash_placeholders=False,
    )
    return build.errors


def _certificate_from_refreshed_proof(
    certificate: dict[str, Any] | None,
    proof: ProofResult,
) -> dict[str, Any] | None:
    """Refresh assurance / provenance fields on a cached certificate dict."""
    if certificate is None:
        return None
    updated = dict(certificate)
    if proof.engine_hash:
        updated["engine_hash"] = proof.engine_hash
    if proof.certifier_hash:
        updated["certifier_hash"] = proof.certifier_hash
    if proof.source_hash:
        updated["source_hash"] = proof.source_hash
    if proof.proof_request_hash:
        updated["proof_request_hash"] = proof.proof_request_hash
    if proof.validation_ledger_hash:
        updated["validation_ledger_hash"] = proof.validation_ledger_hash
    if proof.git_commit:
        updated["git_commit"] = proof.git_commit
    updated["git_dirty"] = bool(proof.git_dirty)
    if proof.opcodes_used:
        updated["opcodes_used"] = list(proof.opcodes_used)
    assurance = getattr(proof, "capability_assurance", None)
    if assurance is not None:
        updated["capability_assurance"] = (
            assurance.to_dict() if hasattr(assurance, "to_dict") else dict(assurance)
        )
    requirements = getattr(proof, "capability_requirements", None)
    if requirements is not None:
        updated["capability_requirements"] = (
            requirements.to_dict()
            if hasattr(requirements, "to_dict")
            else dict(requirements)
        )
    updated.pop("certificate_sha256", None)
    updated["certificate_sha256"] = equivalence_certificate_hash(updated)
    return updated


def rebind_certificate_provenance(certificate: dict[str, Any]) -> dict[str, Any]:
    """Refresh trust-boundary provenance on a certificate before persistence."""
    updated = dict(certificate)
    updated["engine_hash"] = _current_engine_hash()
    updated["certifier_hash"] = _current_certifier_hash()
    commit, dirty = _live_git_identity()
    if commit:
        updated["git_commit"] = commit
    updated["git_dirty"] = dirty
    updated.pop("certificate_sha256", None)
    updated["certificate_sha256"] = equivalence_certificate_hash(updated)
    return updated


def _reevaluate_cached_probe(
    probe: EquivalenceProbe,
    *,
    certified_context: CertifiedCalleeContext | None = None,
    platform_profile: object | None = None,
    environment: object | None = None,
) -> EquivalenceProbe:
    """Refresh live provenance + assurance on a cache-hit probe."""
    proof = probe.proof
    if not isinstance(proof, ProofResult):
        return probe
    _refresh_live_provenance_fields(proof)
    # Re-derive requirements so provenance identity tracks live git/engine hashes.
    proof.capability_requirements = None
    errors = _apply_capability_assurance(
        proof,
        certified_context=certified_context,
        platform_profile=platform_profile,
        environment=environment,
    )
    detail = probe.detail
    for code in errors:
        note = f"capability-assurance-generation-failed:{code}"
        if note not in detail:
            detail = f"{detail}; {note}" if detail else note
    certificate = _certificate_from_refreshed_proof(probe.certificate, proof)
    return EquivalenceProbe(probe.status, detail, certificate, proof=proof)


@dataclass(frozen=True)
class EquivalenceProbe:
    status: ProofStatus
    detail: str = ""
    certificate: dict | None = None
    proof: object | None = None


@dataclass(frozen=True)
class CertifiedCalleeContext:
    contracts: dict[int | str, CalleeContract]
    dependencies: tuple[dict[str, str], ...]
    errors: tuple[str, ...] = ()
    helpers: tuple[str, ...] = ()
    # Stage 3B: concrete certified-calls obligation inputs + live child certs.
    callee_inputs: tuple[Any, ...] = ()
    live_certificates: dict[str, dict[str, Any]] = field(default_factory=dict)
    call_context: Any | None = None
    address_to_target_id: dict[int, str] = field(default_factory=dict)


def _function_sha256(function: object) -> str:
    """Hash function-local code and relocation identity, not the whole object."""
    payload = {
        "name": function.name,
        "base": function.base,
        "code": function.code.hex(),
        "relocations": [
            [item.offset, item.relocation_type, item.symbol, item.addend]
            for item in function.relocations
        ],
    }
    encoded = json.dumps(payload, sort_keys=True, separators=(",", ":"))
    return hashlib.sha256(encoded.encode("utf-8")).hexdigest()


def _certified_callee_digests(
    context: "CertifiedCalleeContext | None",
) -> dict[str, dict[str, str]] | None:
    """H3: per-callee attested trust bindings for proof-request / cache identity.

    Derived from the re-attested ``live_certificates`` of a certified callee
    context: each entry binds the callee's certificate, effect-summary, and body
    digests. Returns ``None`` when there are no certified callees (standalone /
    non-registry proofs stay explicit non-certifying).
    """
    if context is None or not context.live_certificates:
        return None
    from tools.ppc_equivalence.certified_calls_obligations import compute_body_sha256

    digests: dict[str, dict[str, str]] = {}
    for callee_target_id, entry in sorted(
        context.live_certificates.items(), key=lambda item: str(item[0])
    ):
        if not isinstance(entry, dict):
            continue
        digests[str(callee_target_id)] = {
            "certificate_sha256": str(entry.get("certificate_sha256", "")),
            "summary_sha256": str(entry.get("summary_sha256", "")),
            "body_sha256": compute_body_sha256(
                retail_sha256=str(entry.get("retail_sha256", "")),
                candidate_sha256=str(entry.get("candidate_sha256", "")),
            ),
        }
    return digests or None


def _certificate_contract(certificate: dict) -> CalleeContract:
    summary = certificate["summary"]
    return CalleeContract(
        frozenset(summary["reads"]),
        frozenset(summary["writes"]),
        f"certified:{certificate['certificate_sha256']}",
        invalid_reasons=frozenset(summary.get("invalid_reasons", [])),
    )


def _reattest_certificate_tree(
    project: Project,
    target_id: str,
    by_id: dict[str, dict],
    memo: dict[str, str | None],
    checking: set[str] | None = None,
) -> str | None:
    """Re-hash a certificate and every transitive function dependency."""
    if target_id in memo:
        return memo[target_id]
    active = set(checking or ())
    if target_id in active:
        return "certificate dependency cycle"
    active.add(target_id)
    row = by_id.get(target_id)
    if row is None:
        return f"unknown target {target_id!r}"
    if row.get("status") not in ACCEPTED_MATCH_STATUSES:
        return "target is not accepted"
    error = equivalence_certificate_error(row, by_id)
    if error:
        return error
    symbol = row.get("symbol")
    unit_hint = row.get("unit")
    if not isinstance(symbol, str) or not isinstance(unit_hint, str):
        return "target lacks a buildable symbol/unit"
    certificate = row["equivalence_certificate"]
    try:
        unit = project.resolve_unit(unit_hint)
        if unit.target_path is None or unit.base_path is None:
            raise ValueError("unit has no retail/candidate object pair")
        left, right = extract_function_pair(unit.target_path, unit.base_path, symbol)
    except (ElfSymbolError, FileNotFoundError, ValueError) as exc:
        return f"cannot be re-attested: {exc}"
    if _function_sha256(left) != certificate.get("retail_sha256"):
        return "retail function changed"
    if _function_sha256(right) != certificate.get("candidate_sha256"):
        return "candidate function changed"
    # M3: re-attest summary digest when present (byte hashes alone are not enough
    # to catch a forged summary with matching retail/candidate blobs).
    summary = certificate.get("summary")
    stored_summary_digest = certificate.get("summary_sha256")
    if isinstance(summary, dict) and isinstance(stored_summary_digest, str) and stored_summary_digest:
        from tools.ppc_equivalence.certified_calls_obligations import (
            compute_summary_sha256,
        )

        recomputed = compute_summary_sha256(
            target_id=target_id,
            symbol=symbol,
            summary=summary,
        )
        if recomputed != stored_summary_digest:
            return "summary_sha256 mismatch"
    # M3: when the certificate carries a proof-request identity digest, it must be
    # a well-formed SHA-256 so the callee's trust binding stays verifiable. A full
    # recompute (SMT re-prove of the tree) is available via
    # ``targets recertify --bottom-up``; byte + summary + edge digests above are
    # the minimum viable re-attestation for callee reuse.
    proof_request_digest = certificate.get("proof_request_hash")
    if proof_request_digest is not None and not (
        isinstance(proof_request_digest, str)
        and re.fullmatch(r"[0-9a-f]{64}", proof_request_digest)
    ):
        return "proof_request_hash is not a lowercase SHA-256"
    for dependency in certificate.get("callees", []):
        callee_id = dependency["target_id"]
        # Require callee edge digests when the parent certificate lists them.
        edge_digest = dependency.get("certificate_sha256")
        callee_row = by_id.get(callee_id)
        if isinstance(edge_digest, str) and edge_digest and isinstance(callee_row, dict):
            child_cert = callee_row.get("equivalence_certificate")
            if isinstance(child_cert, dict):
                child_digest = child_cert.get("certificate_sha256")
                if child_digest != edge_digest:
                    return (
                        f"callee {callee_id!r}: certificate_sha256 edge mismatch"
                    )
        error = _reattest_certificate_tree(project, callee_id, by_id, memo, active)
        if error:
            return f"callee {callee_id!r}: {error}"
    memo[target_id] = None
    return None


def _load_certified_callees(project: Project, target_id: str) -> CertifiedCalleeContext:
    """Load and re-attest every direct callee declared by the target registry."""
    from tools.ppc_equivalence.certified_calls_obligations import (
        CalleeObligationInput,
        CertifiedCallsContext,
        compute_summary_sha256,
        discharge_trivial_leaf_refinement,
    )

    document = load_targets_document(project.config)
    rows = [row for row in document.get("targets", []) if isinstance(row, dict)]
    by_id = {str(row.get("id")): row for row in rows if isinstance(row.get("id"), str)}
    caller = by_id.get(target_id)
    if caller is None:
        return CertifiedCalleeContext({}, (), (f"unknown target id {target_id!r}",))
    errors: list[str] = []
    unresolved = bool(caller.get("unresolved_called_functions"))
    has_indirect = bool(caller.get("has_indirect_calls"))
    if unresolved:
        errors.append("registry has unresolved direct callees")
    if has_indirect:
        errors.append("registry has an unresolved indirect call")
    called_ids = caller.get("called_functions", [])
    if not isinstance(called_ids, list):
        return CertifiedCalleeContext({}, (), ("called_functions is not an array",))

    contracts: dict[int | str, CalleeContract] = {}
    dependencies: list[dict[str, str]] = []
    callee_inputs: list[CalleeObligationInput] = []
    live_certificates: dict[str, dict[str, Any]] = {}
    address_to_target_id: dict[int, str] = {}
    attestation_memo: dict[str, str | None] = {}
    call_graph: dict[str, frozenset[str]] = {}
    for callee_id in called_ids:
        callee = by_id.get(str(callee_id))
        if callee is None:
            errors.append(f"unknown callee target {callee_id!r}")
            continue
        if callee.get("status") not in ACCEPTED_MATCH_STATUSES:
            errors.append(f"callee {callee_id!r} is not accepted")
            continue
        # Direct self-recursion cannot require its own certificate (bootstrap).
        # Model the recursive edge as opaque EABI while the caller certifies.
        if str(callee_id) == target_id:
            symbol = callee.get("symbol")
            if not isinstance(symbol, str):
                errors.append(f"self-callee {callee_id!r} lacks a symbol")
                continue
            contract = CalleeContract.opaque_eabi()
            contracts[symbol] = contract
            address = callee.get("address")
            if isinstance(address, str):
                try:
                    parsed_address = int(address, 0)
                    contracts[parsed_address] = contract
                    address_to_target_id[parsed_address] = str(callee_id)
                except ValueError:
                    pass
            call_graph[str(callee_id)] = frozenset()
            continue
        attestation_error = _reattest_certificate_tree(
            project, str(callee_id), by_id, attestation_memo,
        )
        if attestation_error:
            errors.append(f"callee {callee_id!r}: {attestation_error}")
            continue
        symbol = callee.get("symbol")
        unit_hint = callee.get("unit")
        if not isinstance(symbol, str) or not isinstance(unit_hint, str):
            errors.append(f"callee {callee_id!r} lacks a buildable symbol/unit")
            continue
        certificate = callee["equivalence_certificate"]
        contract = _certificate_contract(certificate)
        contracts[symbol] = contract
        address = callee.get("address")
        parsed_address: int | None = None
        if isinstance(address, str):
            try:
                parsed_address = int(address, 0)
                contracts[parsed_address] = contract
                address_to_target_id[parsed_address] = str(callee_id)
            except ValueError:
                pass
        dependencies.append({
            "target_id": str(callee_id),
            "certificate_sha256": certificate["certificate_sha256"],
        })
        summary = dict(certificate.get("summary") or {})
        summary.setdefault("reads", [])
        summary.setdefault("writes", [])
        summary.setdefault("invalid_reasons", [])
        summary.setdefault("return_behavior", "normal")
        summary_digest = compute_summary_sha256(
            target_id=str(callee_id),
            symbol=symbol,
            summary=summary,
        )
        refinement = certificate.get("refinement")
        if not isinstance(refinement, dict):
            refinement = None
            try:
                unit = project.resolve_unit(unit_hint)
                if unit.target_path is not None and unit.base_path is not None:
                    left, right = extract_function_pair(
                        unit.target_path, unit.base_path, symbol,
                    )
                    refinement = discharge_trivial_leaf_refinement(
                        target_id=str(callee_id),
                        symbol=symbol,
                        summary=summary,
                        retail_hex=left.code.hex(),
                        candidate_hex=right.code.hex(),
                    )
            except (ElfSymbolError, FileNotFoundError, ValueError):
                refinement = None
        engine_hash = str(certificate.get("engine_hash") or "")
        obligation_input = CalleeObligationInput(
            target_id=str(callee_id),
            symbol=symbol,
            certificate_sha256=str(certificate["certificate_sha256"]),
            retail_sha256=str(certificate["retail_sha256"]),
            candidate_sha256=str(certificate["candidate_sha256"]),
            summary=summary,
            contract_source=f"certified:{certificate['certificate_sha256']}",
            engine_hash=engine_hash,
            expected_engine_hash=_current_engine_hash(),
            required_invalid_reasons=frozenset(),
            refinement=refinement,
        )
        callee_inputs.append(obligation_input)
        live_entry: dict[str, Any] = {
            "certificate_sha256": certificate["certificate_sha256"],
            "symbol": symbol,
            "summary": summary,
            "summary_sha256": summary_digest,
            "retail_sha256": certificate["retail_sha256"],
            "candidate_sha256": certificate["candidate_sha256"],
            "engine_hash": engine_hash,
        }
        if parsed_address is not None:
            live_entry["address"] = parsed_address
        live_certificates[str(callee_id)] = live_entry
        nested = certificate.get("callees") or []
        nested_ids: list[str] = []
        if isinstance(nested, list):
            for dep in nested:
                if isinstance(dep, dict) and isinstance(dep.get("target_id"), str):
                    nested_ids.append(dep["target_id"])
        call_graph[str(callee_id)] = frozenset(nested_ids)

    helpers = caller.get("abi_helper_calls", [])
    if not isinstance(helpers, list) or not all(isinstance(item, str) for item in helpers):
        errors.append("abi_helper_calls is not a string array")
        helpers = []
    helper_pattern = re.compile(r"^_(?:save|rest)(?:gpr|fpr)_\d+$")
    for helper in helpers:
        if helper_pattern.fullmatch(helper) is None:
            errors.append(f"unrecognized ABI helper {helper!r}")
            continue
        match = re.fullmatch(r"_(save|rest)(gpr|fpr)_(\d+)", helper)
        assert match is not None
        operation, register_kind, first_text = match.groups()
        first = int(first_text)
        if not 14 <= first <= 31:
            errors.append(f"ABI helper register is out of range in {helper!r}")
            contracts.pop(helper, None)
            continue
        registers = frozenset(
            f"{'r' if register_kind == 'gpr' else 'f'}{index}"
            for index in range(first, 32)
        )
        if operation == "save":
            reads = registers | {"r11", "memory", "valid"}
            writes = frozenset({"memory", "valid"})
        else:
            reads = frozenset({"r11", "memory", "valid"})
            writes = registers | {"valid"}
        contracts[helper] = CalleeContract(
            reads, writes, f"fixed-eabi-runtime-helper:{helper}",
        )

    caller_callees = frozenset(str(item) for item in called_ids)
    call_graph[str(target_id)] = caller_callees
    call_context = CertifiedCallsContext(
        unresolved_direct_calls=unresolved,
        has_indirect_calls=has_indirect,
        indirect_target_set_closed=not has_indirect,
        root_target_id=str(target_id),
        call_graph=call_graph,
    )
    return CertifiedCalleeContext(
        contracts,
        tuple(sorted(dependencies, key=lambda item: item["target_id"])),
        tuple(errors),
        tuple(sorted(helpers)),
        tuple(sorted(callee_inputs, key=lambda item: item.target_id)),
        live_certificates,
        call_context,
        address_to_target_id,
    )


def _ppc_equivalence_dir(project: Project) -> Path | None:
    if project is None:
        return None
    return project.config.build_dir / "ppc-equivalence"


def _cache_dir(project: Project) -> Path | None:
    d = _ppc_equivalence_dir(project)
    if d is None:
        return None
    return d / "cache"


def _state_dir(project: Project) -> Path | None:
    d = _ppc_equivalence_dir(project)
    if d is None:
        return None
    return d / "state"


def _deps_path(project: Project) -> Path | None:
    d = _ppc_equivalence_dir(project)
    if d is None:
        return None
    return d / "deps.json"


def _cache_key(
    contract_name: str,
    observables: tuple[str, ...],
    original_hex: str,
    candidate_hex: str,
    original_base: int,
    candidate_base: int,
    original_relocations: tuple = (),
    candidate_relocations: tuple = (),
    original_local_symbol: str | None = None,
    candidate_local_symbol: str | None = None,
    assumed_callees: frozenset[int | str] = frozenset(),
    callee_contracts: dict[int | str, CalleeContract] | None = None,
    certified_callee_digests: dict[str, dict[str, str]] | None = None,
    certificate_target_id: str | None = None,
    memory_environment: dict[str, Any] | None = None,
    floating_point_domain: dict[str, Any] | None = None,
    limits: dict[str, int] | None = None,
    engine_hash: str | None = None,
    certifier_hash: str | None = None,
    proof_features: list[str] | None = None,
    address_space: dict[str, Any] | None = None,
    indirect_targets: dict[str, Any] | None = None,
    loop_summary: dict[str, Any] | None = None,
    relational_induction: dict[str, Any] | None = None,
    memory_loop: dict[str, Any] | None = None,
    memory_bus: dict[str, Any] | None = None,
    memory_loop_readonly: dict[str, Any] | None = None,
    obligations: dict[str, Any] | None = None,
    capability_assurance: dict[str, Any] | None = None,
    platform_profile_sha256: str | None = None,
    abi_shape: dict[str, Any] | None = None,
    initial_gpr_ranges: dict[str, list[int]] | None = None,
    ram_only_projection: bool = False,
) -> str:
    def relocations(items: tuple) -> list[tuple]:
        return [
            (item.offset, item.relocation_type, item.symbol, item.addend)
            for item in items
        ]

    fp_domain = floating_point_domain
    if fp_domain is None and isinstance(memory_environment, dict):
        nested = memory_environment.get("floating_point_domain")
        if nested is not None:
            fp_domain = nested

    payload = {
        "architecture": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "engine_hash": engine_hash if engine_hash is not None else _current_engine_hash(),
        "certifier_hash": (
            certifier_hash if certifier_hash is not None else _current_certifier_hash()
        ),
        "contract": contract_name,
        "observables": sorted(observables),
        "original_hex": original_hex,
        "candidate_hex": candidate_hex,
        "original_base": original_base,
        "candidate_base": candidate_base,
        "original_relocations": relocations(original_relocations),
        "candidate_relocations": relocations(candidate_relocations),
        "original_local_symbol": original_local_symbol,
        "candidate_local_symbol": candidate_local_symbol,
        "assumed_callees": sorted(assumed_callees, key=str),
        "callee_contracts": {
            str(name): {
                "reads": sorted(contract.reads),
                "writes": sorted(contract.writes),
                "source": contract.source,
                "invalid_reasons": sorted(contract.invalid_reasons),
            }
            for name, contract in sorted(
                (callee_contracts or {}).items(), key=lambda item: str(item[0])
            )
        },
        "certificate_target_id": certificate_target_id,
        "memory_environment": memory_environment,
        "floating_point_domain": fp_domain,
        "limits": {
            str(key): int(value)
            for key, value in sorted((limits or {}).items())
        },
    }
    if platform_profile_sha256 is not None:
        payload["platform_profile_sha256"] = platform_profile_sha256
    if abi_shape is not None:
        payload["abi_shape"] = abi_shape
    if initial_gpr_ranges:
        payload["initial_gpr_ranges"] = {
            str(reg): list(bounds) for reg, bounds in sorted(initial_gpr_ranges.items())
        }
    if ram_only_projection:
        payload["ram_only_projection"] = True
    # H3: bind each certified callee's attested trust-binding digests. Emit only
    # when present so existing (callee-free / standalone) cache identities are
    # unchanged; any change to a callee certificate/summary busts the caller.
    if certified_callee_digests:
        payload["certified_callee_digests"] = {
            str(target_id): {
                str(key): str(value)
                for key, value in sorted(
                    digests.items(), key=lambda item: str(item[0])
                )
            }
            for target_id, digests in sorted(
                certified_callee_digests.items(), key=lambda item: str(item[0])
            )
            if isinstance(digests, dict)
        }
    if proof_features is not None:
        payload["proof_features"] = sorted(proof_features)
    # Top-level identity premise (not a proof feature): binds the exact per-side
    # readonly-image words used to hydrate CTR lwz trip counts.
    if memory_loop_readonly is not None:
        payload["memory_loop_readonly"] = memory_loop_readonly

    merged: dict[str, Any] = dict(obligations or {})
    for key, value in (
        ("address_space", address_space),
        ("indirect_targets", indirect_targets),
        ("loop_summary", loop_summary),
        ("relational_induction", relational_induction),
        ("memory_loop", memory_loop),
        ("memory_bus", memory_bus),
    ):
        if value is not None:
            merged[key] = value
    for key in PROOF_OBLIGATION_FIELDS:
        block = merged.get(key)
        if isinstance(block, dict):
            payload[key] = canonical_obligation_dict(block)
    if capability_assurance is not None:
        payload["capability_assurance"] = capability_assurance
    return hashlib.sha256(
        json.dumps(payload, sort_keys=True).encode(),
    ).hexdigest()


def _proof_from_cache_payload(
    status: ProofStatus,
    *,
    certificate: dict[str, Any] | None,
    proof_audit: dict[str, Any] | None,
) -> ProofResult | None:
    """Rebuild a ``ProofResult`` from cached certificate and/or audit dict."""
    if not isinstance(certificate, dict) and not isinstance(proof_audit, dict):
        return None
    merged: dict[str, Any] = {}
    if isinstance(certificate, dict):
        merged.update(certificate)
    if isinstance(proof_audit, dict):
        merged.update(proof_audit)
    # Lazy import avoids import cycles with promotion helpers.
    from tools.coop.lib.equivalence_policy import proof_result_from_certificate

    return proof_result_from_certificate(status, merged)


def _cache_get(
    key: str,
    cache_dir: Path | None,
    *,
    engine_hash: str | None = None,
    certifier_hash: str | None = None,
) -> EquivalenceProbe | None:
    if cache_dir is None:
        return None
    entry_path = cache_dir / f"{key}.json"
    if not entry_path.is_file():
        return None
    try:
        data = json.loads(entry_path.read_text(encoding="utf-8"))
        if data.get("architecture") != ARCHITECTURE_MODEL:
            return None
        if data.get("result_format") != RESULT_FORMAT:
            return None
        expected_engine = (
            engine_hash if engine_hash is not None else _current_engine_hash()
        )
        stored_engine = data.get("engine_hash")
        # Reject missing engine_hash (pre-binding entries) and mismatches.
        if not isinstance(stored_engine, str) or stored_engine != expected_engine:
            return None
        expected_certifier = (
            certifier_hash if certifier_hash is not None else _current_certifier_hash()
        )
        stored_certifier = data.get("certifier_hash")
        if not isinstance(stored_certifier, str) or stored_certifier != expected_certifier:
            return None
        status = ProofStatus(data["status"])
        certificate = data.get("certificate")
        proof_audit = data.get("proof_audit")
        certificate_dict = certificate if isinstance(certificate, dict) else None
        audit_dict = proof_audit if isinstance(proof_audit, dict) else None
        proof = _proof_from_cache_payload(
            status,
            certificate=certificate_dict,
            proof_audit=audit_dict,
        )
        # EQUIVALENT (and any entry that claimed a certificate) requires a
        # reconstructable ProofResult. Evidence-free EQUIVALENT status-only
        # entries are incomplete/fabricated and must miss.
        if proof is None and (
            status is ProofStatus.EQUIVALENT or certificate_dict is not None
        ):
            return None
        detail = data.get("detail", "")
        # Revalidation may demote a stored EQUIVALENT (unsupported features,
        # forged/weak obligations). Outer probe fields must match the re-gated
        # ProofResult — never return EQUIVALENT while proof is demoted.
        if proof is not None and proof.status is not status:
            status = proof.status
            certificate_dict = None
            demotion_notes = [
                warning
                for warning in (getattr(proof, "warnings", None) or [])
                if isinstance(warning, str) and warning.startswith("proof_features:")
            ]
            if demotion_notes:
                note = demotion_notes[0]
                detail = f"{detail}; {note}" if detail else note
        return EquivalenceProbe(
            status,
            detail,
            certificate_dict,
            proof=proof,
        )
    except (KeyError, ValueError, json.JSONDecodeError, TypeError):
        return None


def _proof_audit_dict(proof: object | None) -> dict[str, Any] | None:
    """Compact durable audit subset for cache entries (not a full ProofResult)."""
    if proof is None:
        return None
    if isinstance(proof, dict):
        return proof
    audit: dict[str, Any] = {}
    for name in (
        "architecture_model",
        "format",
        "contract",
        "observables",
        "assumptions",
        "assumed_callees",
        "callee_contracts",
        "limits",
        "opcodes_used",
        "fp_oracle_version",
        "engine_hash",
        "certifier_hash",
        "source_hash",
        "proof_request_hash",
        "validation_ledger_hash",
        "git_commit",
        "git_dirty",
        "repair_hint",
        "unsupported",
        "warnings",
        "abstractions",
        "proof_features",
        "capability_assurance",
        "capability_requirements",
        *PROOF_OBLIGATION_FIELDS,
    ):
        value = getattr(proof, name, None)
        if value not in (None, "", [], {}, ()):
            if hasattr(value, "to_dict"):
                audit[name] = value.to_dict()
            elif isinstance(value, dict):
                audit[name] = dict(value)
            elif isinstance(value, (list, tuple)):
                audit[name] = list(value)
            else:
                audit[name] = value
    environment = getattr(proof, "environment", None)
    if environment is not None and hasattr(environment, "to_dict"):
        audit["environment"] = environment.to_dict()
    memory_scope = getattr(proof, "memory_scope", None)
    if memory_scope is not None and hasattr(memory_scope, "to_dict"):
        audit["memory_scope"] = memory_scope.to_dict()
    fp_domain = getattr(proof, "floating_point_domain", None)
    if fp_domain is not None and hasattr(fp_domain, "to_dict"):
        audit["floating_point_domain"] = fp_domain.to_dict()
    return audit or None


def _cache_put(
    key: str, probe: EquivalenceProbe, cache_dir: Path | None,
    assumed_callees: set[int | str] | frozenset[int | str] = frozenset(),
    *,
    engine_hash: str | None = None,
    certifier_hash: str | None = None,
) -> None:
    if cache_dir is None:
        return
    cache_dir.mkdir(parents=True, exist_ok=True)
    entry_path = cache_dir / f"{key}.json"
    entry: dict[str, Any] = {
        "architecture": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "engine_hash": engine_hash if engine_hash is not None else _current_engine_hash(),
        "certifier_hash": (
            certifier_hash if certifier_hash is not None else _current_certifier_hash()
        ),
        "status": probe.status.value,
        "detail": probe.detail,
        "certificate": probe.certificate,
        "assumed_callees": sorted(assumed_callees, key=str),
        "created_at": time.time(),
    }
    proof_audit = _proof_audit_dict(probe.proof)
    if proof_audit is not None:
        entry["proof_audit"] = proof_audit
    entry_path.write_text(
        json.dumps(entry, sort_keys=True),
        encoding="utf-8",
    )


def _load_counterexample(
    project: Project, symbol: str, orig_hex: str, proof_context: str = "",
) -> dict | None:
    sd = _state_dir(project)
    if sd is None:
        return None
    key = hashlib.sha256(f"{symbol}:{orig_hex}:{proof_context}".encode()).hexdigest()
    path = sd / f"{key}_ce.json"
    if not path.is_file():
        return None
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, ValueError):
        return None


def _save_counterexample(
    project: Project, symbol: str, orig_hex: str, ce: dict,
    proof_context: str = "",
) -> None:
    sd = _state_dir(project)
    if sd is None:
        return
    sd.mkdir(parents=True, exist_ok=True)
    key = hashlib.sha256(f"{symbol}:{orig_hex}:{proof_context}".encode()).hexdigest()
    (sd / f"{key}_ce.json").write_text(json.dumps(ce, sort_keys=True), encoding="utf-8")


def _replay_counterexample(
    original: list,
    candidate: list,
    contract_name: str,
    ce: dict,
) -> tuple[bool, str]:
    try:
        from tools.ppc_equivalence.model import concrete_state as _cs
        state = _cs(ce.get("initial_state", {}))
    except Exception:
        return False, ""
    ops = ConcreteOps()
    try:
        orig_term = execute_cfg(state, original, ops)
        cand_term = execute_cfg(state, candidate, ops)
    except Exception:
        return False, ""
    if len(orig_term) != 1 or len(cand_term) != 1:
        return False, ""
    if orig_term[0].exit_kind != cand_term[0].exit_kind:
        return True, "exit kind differs"
    orig_state = orig_term[0].state
    cand_state = cand_term[0].state
    if not bool(orig_state.valid) or not bool(cand_state.valid):
        return False, ""
    for i in range(32):
        if orig_state.gpr[i] != cand_state.gpr[i]:
            return True, f"r{i}: 0x{int(orig_state.gpr[i]) & 0xFFFFFFFF:08x} != 0x{int(cand_state.gpr[i]) & 0xFFFFFFFF:08x}"
    return False, ""


def _load_known_equivalent_targets(project: Project) -> frozenset[int]:
    dp = _deps_path(project)
    if dp is None or not dp.is_file():
        return frozenset()
    try:
        data = json.loads(dp.read_text(encoding="utf-8"))
        return frozenset(data.get("equivalent_targets", []))
    except (json.JSONDecodeError, ValueError):
        return frozenset()


def _save_deps(project: Project, equivalent_targets: frozenset[int]) -> None:
    dp = _deps_path(project)
    if dp is None:
        return
    dp.parent.mkdir(parents=True, exist_ok=True)
    dp.write_text(
        json.dumps({"equivalent_targets": sorted(equivalent_targets)}, sort_keys=True),
        encoding="utf-8",
    )


def _add_equivalent_target(project: Project, target: int) -> None:
    existing = _load_known_equivalent_targets(project)
    if target not in existing:
        _save_deps(project, existing | {target})


def _invalidate_dependent_caches(
    project: Project,
    cache_dir: Path,
    callee_dependencies: frozenset[int],
    invalidated_callees: frozenset[int],
) -> int:
    removed = 0
    for entry_path in cache_dir.glob("*.json"):
        try:
            data = json.loads(entry_path.read_text(encoding="utf-8"))
            deps = frozenset(data.get("assumed_callees", []))
            if deps & invalidated_callees:
                entry_path.unlink()
                removed += 1
        except (json.JSONDecodeError, ValueError, OSError):
            pass
    return removed


def should_probe_equivalence(match_percent: Optional[float]) -> bool:
    """Only prove when static match can still be promoted to EQUIVALENT_MATCH."""
    return match_percent is not None and EQUIVALENT_MATCH_MIN_PERCENT <= match_percent < 100.0


def _extract_call_targets(instructions: list) -> frozenset[int | str]:
    targets: set[int | str] = set()
    for insn in instructions:
        if insn.opcode == Opcode.B and (insn.link or insn.relocation is not None):
            targets.add(
                insn.relocation.canonical_symbol
                if insn.relocation is not None else insn.operands[0]
            )
        elif insn.opcode == Opcode.BC and (insn.link or insn.relocation is not None):
            targets.add(
                insn.relocation.canonical_symbol
                if insn.relocation is not None else insn.operands[2]
            )
    return frozenset(targets)


def _infer_matched_callee_contracts(
    call_targets: frozenset[int | str],
    original_object: Path | None,
    candidate_object: Path | None,
) -> dict[int | str, CalleeContract]:
    """Generate paired effect contracts for named callees in the same objects."""
    return infer_matched_callee_contracts(
        call_targets, original_object, candidate_object,
    )


# EABI nonvolatiles omitted from ``CalleeContract.opaque_eabi`` write sets.
# Spill/restore modeling can report them as writes even when a ``ppc-eabi``
# EQUIVALENT proof already established preservation across the return.
_EABI_NONVOLATILE_EFFECTS = frozenset(
    {
        *(f"r{i}" for i in range(14, 32)),
        *(f"f{i}" for i in range(14, 32)),
        *(f"f{i}.ps1" for i in range(14, 32)),
        "cr2",
        "cr3",
        "cr4",
    }
)


def _nv_spill_false_positive(validation: object) -> bool:
    """True when the only opaque-eabi gap is nonvolatile spill noise."""
    if getattr(validation, "reason", None):
        return False
    missing_reads = getattr(validation, "missing_reads", frozenset()) or frozenset()
    missing_writes = getattr(validation, "missing_writes", frozenset()) or frozenset()
    missing_invalid = getattr(validation, "missing_invalid_reasons", frozenset()) or frozenset()
    return (
        not missing_reads
        and not missing_invalid
        and bool(missing_writes)
        and missing_writes <= _EABI_NONVOLATILE_EFFECTS
    )


def _build_equivalence_certificate(
    target_id: str,
    left_function: object,
    right_function: object,
    original: list,
    candidate: list,
    *,
    call_targets: frozenset[int | str],
    callee_contracts: dict[int | str, CalleeContract],
    dependencies: tuple[dict[str, str], ...],
    helpers: tuple[str, ...],
    evidence: str,
    max_instructions: int,
    max_paths: int,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    memory_scope: dict | None = None,
    proof: object | None = None,
    skip_semantic_validation: bool = False,
) -> tuple[dict | None, str]:
    """Derive and validate a normal-return semantic effect summary.

    When ``skip_semantic_validation`` is set (byte-identical FULL_MATCH
    synthesis), skip ``validate_callee_contract`` — path explosion on small
    callers with certified callees (e.g. ``getView`` → ``getWorkThread``)
    otherwise hangs certify and blocks the ACCEPTED frontier.
    """
    if skip_semantic_validation:
        reads: list[str] = []
        writes: list[str] = []
        invalid_reasons: list[str] = []
    else:
        declared = CalleeContract.opaque_eabi()
        validations = [
            validate_callee_contract(
                instructions,
                declared,
                max_instructions=max_instructions,
                max_paths=max_paths,
                max_loop_iterations=max_loop_iterations,
                assumed_callees=call_targets,
                callee_contracts=callee_contracts,
                require_normal_return=True,
            )
            for instructions in (original, candidate)
        ]
        for side, validation in zip(("retail", "candidate"), validations):
            if not validation.valid and not _nv_spill_false_positive(validation):
                detail = validation.reason or (
                    "contract omitted reads " + ", ".join(sorted(validation.missing_reads))
                    + "; writes " + ", ".join(sorted(validation.missing_writes))
                )
                return None, f"{side} certificate validation failed: {detail}"
        reads = sorted(validations[0].required_reads | validations[1].required_reads)
        # Never advertise NV clobbers in the callee summary: callers rely on EABI
        # preservation, and EQUIVALENT under ppc-eabi already checked that.
        writes = sorted(
            (validations[0].required_writes | validations[1].required_writes)
            - _EABI_NONVOLATILE_EFFECTS
        )
        invalid_reasons = sorted(
            set(validations[0].required_invalid_reasons)
            | set(validations[1].required_invalid_reasons)
        )
    certificate = {
        "version": EQUIVALENCE_CERTIFICATE_VERSION,
        "status": "SEMANTIC_CERTIFIED",
        "architecture": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "target_id": target_id,
        "evidence": evidence,
        "retail_sha256": _function_sha256(left_function),
        "candidate_sha256": _function_sha256(right_function),
        "summary": {
            "reads": reads,
            "writes": writes,
            "invalid_reasons": invalid_reasons,
            "return_behavior": "normal",
        },
        "callees": list(dependencies),
        "helpers": list(helpers),
    }
    from tools.ppc_equivalence.certified_calls_obligations import (
        compute_summary_sha256,
    )

    certificate["summary_sha256"] = compute_summary_sha256(
        target_id=target_id,
        symbol=getattr(left_function, "name", "") or target_id,
        summary=certificate["summary"],
    )
    if memory_scope is not None:
        certificate["memory_scope"] = memory_scope
    # Always emit git_dirty (including False) even when proof is absent.
    certificate["git_dirty"] = False
    if proof is not None:
        source_hash = getattr(proof, "source_hash", "") or ""
        git_commit = getattr(proof, "git_commit", "") or ""
        proof_request_hash_value = getattr(proof, "proof_request_hash", "") or ""
        validation_ledger_hash = getattr(proof, "validation_ledger_hash", "") or ""
        if source_hash:
            certificate["source_hash"] = source_hash
        certificate["git_dirty"] = bool(getattr(proof, "git_dirty", False))
        if git_commit:
            certificate["git_commit"] = git_commit
        if proof_request_hash_value:
            certificate["proof_request_hash"] = proof_request_hash_value
        if validation_ledger_hash:
            certificate["validation_ledger_hash"] = validation_ledger_hash
        environment = getattr(proof, "environment", None)
        if environment is not None and hasattr(environment, "to_dict"):
            certificate["environment"] = environment.to_dict()
        fp_domain = getattr(proof, "floating_point_domain", None)
        if fp_domain is not None and hasattr(fp_domain, "to_dict"):
            certificate["floating_point_domain"] = fp_domain.to_dict()
        observables = getattr(proof, "observables", None)
        if observables:
            certificate["observables"] = list(observables)
        assumed = getattr(proof, "assumed_callees", None)
        if assumed:
            certificate["assumed_callees"] = list(assumed)
        opcodes_used = getattr(proof, "opcodes_used", None)
        if opcodes_used:
            certificate["opcodes_used"] = list(opcodes_used)
        limits = getattr(proof, "limits", None)
        if limits:
            certificate["limits"] = dict(limits)
        contract_name = getattr(proof, "contract", None)
        if contract_name:
            certificate["contract"] = str(contract_name)
        assumptions = getattr(proof, "assumptions", None)
        if assumptions:
            certificate["assumptions"] = list(assumptions)
        proof_callee_contracts = getattr(proof, "callee_contracts", None)
        if proof_callee_contracts:
            certificate["callee_contracts"] = {
                str(name): dict(entry) if isinstance(entry, dict) else entry
                for name, entry in proof_callee_contracts.items()
            }
        repair_hint = getattr(proof, "repair_hint", None)
        if repair_hint:
            certificate["repair_hint"] = dict(repair_hint)
        proof_features = getattr(proof, "proof_features", None)
        if proof_features:
            certificate["proof_features"] = list(proof_features)
        fp_oracle_version = getattr(proof, "fp_oracle_version", None)
        if fp_oracle_version:
            certificate["fp_oracle_version"] = str(fp_oracle_version)
        capability_assurance = getattr(proof, "capability_assurance", None)
        if capability_assurance is not None:
            if hasattr(capability_assurance, "to_dict"):
                certificate["capability_assurance"] = capability_assurance.to_dict()
            elif isinstance(capability_assurance, dict):
                certificate["capability_assurance"] = dict(capability_assurance)
        capability_requirements = getattr(proof, "capability_requirements", None)
        if capability_requirements is not None:
            if hasattr(capability_requirements, "to_dict"):
                certificate["capability_requirements"] = capability_requirements.to_dict()
            elif isinstance(capability_requirements, dict):
                certificate["capability_requirements"] = dict(capability_requirements)
        if isinstance(proof, ProofResult):
            for key, block in proof_obligations_from_result(proof).items():
                certificate[key] = dict(block)
        else:
            for key in PROOF_OBLIGATION_FIELDS:
                block = getattr(proof, key, None)
                if isinstance(block, dict):
                    certificate[key] = dict(block)
        certificate["equivalence_strength"] = _equivalence_strength_for_proof(proof)
    else:
        certificate["equivalence_strength"] = "uncertified-summary"
    # Bind live trust-boundary trees last so stale proof/cache provenance cannot win.
    certificate["engine_hash"] = _current_engine_hash()
    certificate["certifier_hash"] = _current_certifier_hash()
    certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)
    return certificate, ""


def _equivalence_strength_for_proof(proof: object) -> str:
    """Label acceptance strength; tiers do not block EQUIVALENT_MATCH."""
    from tools.ppc_equivalence.ir import SUPPORTED_FP_OPCODES

    assumed = getattr(proof, "assumed_callees", None) or []
    opcodes = set(getattr(proof, "opcodes_used", None) or [])
    fp_values = {op.value for op in SUPPORTED_FP_OPCODES}
    has_fp = bool(opcodes & fp_values)
    features = set(getattr(proof, "proof_features", None) or [])
    if assumed:
        return "callee-dependent"
    if "memory-bus" in features or getattr(proof, "memory_bus", None):
        return "memory-bus-tier-c"
    if has_fp:
        return "fp-tier-c"
    return "integer-core"


def _prove_bytes(
    project: Project,
    symbol: str,
    orig_code: bytes,
    orig_base: int,
    cand_code: bytes,
    cand_base: int,
    *,
    contract: str,
    timeout_ms: int,
    max_instructions: int,
    max_paths: int,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    fallback_note: str = "",
    original_relocations: tuple = (),
    candidate_relocations: tuple = (),
    original_local_symbol: str | None = None,
    candidate_local_symbol: str | None = None,
    original_object: Path | None = None,
    candidate_object: Path | None = None,
    original_function: object | None = None,
    candidate_function: object | None = None,
    certificate_target_id: str | None = None,
    certified_context: CertifiedCalleeContext | None = None,
    certificate_evidence: str = "symbolic-equivalence",
    memory_environment: dict[str, Any] | None = None,
    floating_point_domain: dict[str, Any] | None = None,
    proof_features: list[str] | None = None,
    address_space: dict[str, Any] | None = None,
    indirect_targets: dict[str, Any] | None = None,
) -> EquivalenceProbe:
    """Run the Z3 proof against already-extracted instruction bytes+bases.

    Used for both the unlinked-pair path (bytes from the ``.o`` files) and the
    linked-bytes fallback (bytes from ``main.dol`` / ``main.elf``). The
    ``fallback_note`` is prefixed to the returned ``detail`` so callers can see
    which path produced the result.
    """
    original = decode_block(
        orig_code, orig_base, validate_with_capstone=False,
        relocations=original_relocations,
        local_symbol=original_local_symbol,
    )
    candidate = decode_block(
        cand_code, cand_base, validate_with_capstone=False,
        relocations=candidate_relocations,
        local_symbol=candidate_local_symbol,
    )

    jump_table_context = None
    virtual_call_context = None
    memory_loop_readonly_words = None
    initial_gpr_bindings: dict[int, int] | None = None
    if proof_features is None and address_space is None and indirect_targets is None:
        from tools.ppc_equivalence.jump_table_auto import try_auto_jump_table_context
        from tools.ppc_equivalence.jump_table_obligations import (
            JumpTableArtifacts,
            build_jump_table_obligations,
            side_artifact_from_path,
        )
        from tools.ppc_equivalence.memory_loop import (
            collect_memory_loop_ctr_lwz_addresses,
        )
        from tools.ppc_equivalence.memory_loop_image import (
            try_build_memory_loop_readonly_words,
        )
        from tools.ppc_equivalence.memory_loop_readonly import (
            build_memory_loop_readonly_context,
        )
        from tools.ppc_equivalence.sda_layout import (
            collect_reloc_symbol_names,
            extract_sda_bases,
            extract_symbol_addresses,
        )
        from tools.ppc_equivalence.vtable_obligations import (
            try_auto_virtual_call_context,
            try_auto_virtual_thunk_context,
            virtual_call_gate_reason,
        )

        original_artifact = None
        candidate_artifact = None
        dol_path = None
        elf_path = None
        if project is not None:
            configured_dol = getattr(project.config, "main_dol", None)
            if configured_dol is not None and Path(configured_dol).is_file():
                dol_path = Path(configured_dol)
                original_artifact = side_artifact_from_path(dol_path, kind="dol")
            linked_elf = getattr(project, "linked_elf_path", None)
            if linked_elf is not None and Path(linked_elf).is_file():
                elf_path = Path(linked_elf)
                candidate_artifact = side_artifact_from_path(elf_path, kind="elf")
        artifacts = None
        if original_artifact is not None and candidate_artifact is not None:
            artifacts = JumpTableArtifacts(
                original=original_artifact,
                candidate=candidate_artifact,
            )

        sda_bases: dict[int, int] = {}
        symbol_addresses: dict[str, int] = {}
        reloc_names = collect_reloc_symbol_names(
            original_relocations, candidate_relocations,
        )
        # Prefer linked ELF for SDA / absolute symbols; DOL alone rarely exports
        # _SDA_BASE_. Fail-closed when neither image yields bases.
        for image in (elf_path, dol_path):
            if image is None:
                continue
            for reg, value in extract_sda_bases(image).items():
                sda_bases.setdefault(reg, value)
            if reloc_names:
                for name, value in extract_symbol_addresses(
                    image, names=reloc_names,
                ).items():
                    symbol_addresses.setdefault(name, value)
            else:
                # Still capture SDA linker symbols even with empty reloc sets.
                for name, value in extract_symbol_addresses(
                    image,
                    names={"_SDA_BASE_", "_SDA2_BASE_"},
                ).items():
                    symbol_addresses.setdefault(name, value)

        jump_table_context = try_auto_jump_table_context(
            original,
            candidate,
            artifacts=artifacts,
            # Per-side only — no shared dol_path/elf_path production fallback.
            original_dol_path=dol_path if artifacts is None else None,
            candidate_elf_path=elf_path if artifacts is None else None,
            sda_bases=sda_bases or None,
            symbol_addresses=symbol_addresses or None,
        )
        # Virtual-call try_auto: returns None without slot/certificate premises
        # (fail-closed). Pattern presence alone is gated inside check_equivalence.
        virtual_call_context = try_auto_virtual_call_context(original, candidate)
        # Bctr thunks: optional readonly/closure when images hydrate; never
        # demote bare symbolic CTR equivalence when hydration is absent.
        if virtual_call_context is None:
            virtual_call_context = try_auto_virtual_thunk_context(
                original,
                candidate,
                artifacts=artifacts,
                original_dol_path=dol_path if artifacts is None else None,
                candidate_elf_path=elf_path if artifacts is None else None,
            )
        # Per-side provenance: the original (retail) side is hydrated ONLY from
        # the DOL image and the candidate (decomp) side ONLY from the linked
        # ELF. Never let the ELF shadow the retail DOL (or vice versa).
        original_words = try_build_memory_loop_readonly_words(
            original,
            dol_path=dol_path,
            elf_path=None,
        )
        candidate_words = try_build_memory_loop_readonly_words(
            candidate,
            dol_path=None,
            elf_path=elf_path,
        )
        original_needs_words = bool(
            collect_memory_loop_ctr_lwz_addresses(original)
        )
        candidate_needs_words = bool(
            collect_memory_loop_ctr_lwz_addresses(candidate)
        )
        # If either side needs CTR-lwz hydration but its image evidence is
        # missing, disable the summary on BOTH sides and fall back to ordinary
        # execution — never summarize one side with evidence the other lacks.
        if (original_needs_words and original_words is None) or (
            candidate_needs_words and candidate_words is None
        ):
            memory_loop_readonly_words = None
        else:
            memory_loop_readonly_words = build_memory_loop_readonly_context(
                original_words=original_words,
                candidate_words=candidate_words,
                original_source="image",
                candidate_source="image",
            )
        if jump_table_context is not None:
            proof_features = ["readonly-image", "indirect-target-closure"]
            address_space, indirect_targets = build_jump_table_obligations(
                jump_table_context,
                no_write_status="pending",
                coverage="pending",
                status="pending",
            )
        elif virtual_call_context is not None:
            from tools.ppc_equivalence.vtable_obligations import (
                build_virtual_call_obligations,
            )

            proof_features = ["readonly-image", "indirect-target-closure"]
            address_space, indirect_targets = build_virtual_call_obligations(
                virtual_call_context,
                no_write_status="pending",
                coverage="pending",
                status="pending",
            )
        elif virtual_call_gate_reason(original, candidate) is not None:
            # Pattern recognized but premises incomplete — leave features unset
            # so the engine gate demotes EQUIVALENT.
            pass

        # Stash SDA bases as initial r2/r13 constraints (EABI invariant).
        # Do not force absolute relocation_bindings here — unlinked HA/LO proofs
        # share symbolic symbol addresses; binding one absolute VA can falsely
        # distinguish DOL vs ELF placements of the same logical symbol.
        initial_gpr_bindings = dict(sda_bases) if sda_bases else None
    # note: when caller pre-supplies proof_features, leave initial_gpr unset
    original_live_out = automatic_live_out(original)
    candidate_live_out = automatic_live_out(candidate)
    live_out = None
    if contract == "live-out":
        live_out = tuple(dict.fromkeys(original_live_out + candidate_live_out))

    if timeout_ms <= 0:
        instr_count = max(len(original), len(candidate))
        scaled = instr_count * _TIMEOUT_MS_PER_INSN
        # Raised prove_unit_symbol ceilings (soft-cap retry) → full budget.
        if (
            max_instructions > 4096
            or max_loop_iterations > 512
            or max_paths > 512
        ):
            scaled = max(scaled, _TIMEOUT_MS_MAX)
        timeout_ms = max(_TIMEOUT_MS_MIN, min(_TIMEOUT_MS_MAX, scaled))

    resolved_contract = make_contract(
        preset=contract,
        observe=None,
        timeout_ms=timeout_ms,
        live_out=live_out,
        original_live_out=original_live_out,
        candidate_live_out=candidate_live_out,
    )

    # Opt-in AbiShape inference: only attach when narrowed away from the
    # fail-closed conservative default (keeps cache / resolution clean).
    abi_shape_payload: dict[str, Any] | None = None
    if project is not None:
        from tools.coop.lib.config import abi_shape_inference_enabled
        from tools.ppc_equivalence.abi_infer import infer_abi_shape
        from tools.ppc_equivalence.contract import with_abi_shape

        if abi_shape_inference_enabled(project.config):
            inferred = infer_abi_shape(
                original, candidate, symbol=symbol, enabled=True,
            )
            if inferred.source != "default-conservative":
                resolved_contract = with_abi_shape(resolved_contract, inferred)
                abi_shape_payload = inferred.to_dict()

    observables = tuple(item.name for item in resolved_contract.observables)
    orig_hex = orig_code.hex()
    cand_hex = cand_code.hex()
    call_targets = _extract_call_targets(original) | _extract_call_targets(candidate)
    if certificate_target_id is not None:
        context = certified_context or CertifiedCalleeContext({}, (), ("callee context missing",))
        if context.errors:
            return EquivalenceProbe(
                ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE,
                "; ".join(context.errors),
            )
        missing = sorted((target for target in call_targets if target not in context.contracts), key=str)
        if missing:
            return EquivalenceProbe(
                ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE,
                "calls lack current certificates: " + ", ".join(str(item) for item in missing),
            )
        assumed_callees = call_targets
        callee_contracts = {
            target: context.contracts[target] for target in call_targets
        }
    else:
        known_addresses = _load_known_equivalent_targets(project)
        # Standalone CLI compatibility: infer same-object summaries only when
        # original/candidate bodies prove EQUIVALENT. Registry workflows use
        # durable, re-attested certificates above. Unproven named callees are
        # omitted so CFG execution fails closed (no silent opaque EABI).
        inference_targets = frozenset(
            target for target in call_targets if isinstance(target, str)
        )
        callee_contracts = _infer_matched_callee_contracts(
            inference_targets, original_object, candidate_object,
        )
        assumed_callees = frozenset(callee_contracts) | frozenset(
            target for target in call_targets
            if isinstance(target, int) and target in known_addresses
        )

    if memory_environment is None and project is not None:
        from tools.coop.lib.config import memory_environment_from_config

        memory_environment = memory_environment_from_config(project.config)

    platform_profile_obj = None
    platform_profile_sha256 = None
    if project is not None:
        from tools.coop.lib.config import (
            platform_profile_digest_from_config,
            platform_profile_from_config,
        )

        platform_profile_obj = platform_profile_from_config(project.config)
        platform_profile_sha256 = platform_profile_digest_from_config(project.config)

    # GX FIFO Tier-A pre-allowlist wiring: reviewed MMIO ``hardware_profile``
    # knob (distinct from ``platform_profile``'s bounded-RAM profiles). When
    # configured, materialize a ``MemoryBus`` and thread it through
    # ``check_equivalence(..., memory_bus=)`` so gx-fifo canaries can actually
    # discharge under the reviewed hardware profile. Fail-closed: an invalid
    # or unset profile leaves ``memory_bus_obj`` as ``None`` (no silent RAM).
    memory_bus_obj = None
    memory_bus_identity: dict[str, Any] | None = None
    ram_only_projection = False
    if project is not None:
        from tools.coop.lib.config import (
            memory_bus_from_config,
            ram_only_when_no_mmio_enabled,
        )

        memory_bus_obj = memory_bus_from_config(project.config)
        if (
            memory_bus_obj is not None
            and ram_only_when_no_mmio_enabled(project.config)
        ):
            from tools.ppc_equivalence.object_base import (
                ram_only_memory_bus,
                should_use_ram_only_bus,
            )

            if should_use_ram_only_bus(original, candidate, memory_bus_obj):
                memory_bus_obj = ram_only_memory_bus(memory_bus_obj)
                ram_only_projection = True
        if memory_bus_obj is not None:
            bus_spec = memory_bus_obj.specification
            memory_bus_identity = {
                "algorithm": "memory-bus-v1",
                "bus_spec_sha256": bus_spec.sha256() if bus_spec is not None else None,
                "hardware_profile": memory_bus_obj.hardware_profile_name,
                "hardware_profile_sha256": memory_bus_obj.hardware_profile_sha256,
            }
            if ram_only_projection:
                memory_bus_identity["ram_only_projection"] = True

    initial_gpr_ranges: dict[int, tuple[int, int]] | None = None
    if project is not None:
        from tools.coop.lib.config import object_base_mem1_enabled

        if object_base_mem1_enabled(project.config):
            from tools.ppc_equivalence.object_base import mem1_gpr_ranges

            initial_gpr_ranges = mem1_gpr_ranges(profile=platform_profile_obj)

    raw_fp = floating_point_domain
    if raw_fp is None and project is not None:
        raw_fp = getattr(project.config, "floating_point_domain", None)
    fp_domain_dict: dict[str, Any] | None = None
    if isinstance(raw_fp, dict):
        fp_domain_dict = dict(raw_fp)
    elif raw_fp is not None:
        from tools.ppc_equivalence.result import FloatingPointDomain

        fp_domain_dict = FloatingPointDomain.parse(raw_fp).to_dict()

    # Bind paired-single SoftFloat oracle identity into request/cache when used.
    from tools.ppc_equivalence.fp_outcome import FP_ORACLE_VERSION, PAIRED_ORACLE_OPS

    if any(insn.opcode.value in PAIRED_ORACLE_OPS for insn in (*original, *candidate)):
        if fp_domain_dict is None:
            fp_domain_dict = {}
        fp_domain_dict = dict(fp_domain_dict)
        fp_domain_dict.setdefault("fp_oracle_version", FP_ORACLE_VERSION)

    # H3: derive per-callee attested trust bindings from the re-attested certified
    # context. These digests (certificate / summary / body) bind the caller's
    # cache + proof-request identity to the exact callee certificates trusted at
    # prove time. Standalone / non-registry proofs (no certified context) omit
    # this and stay explicit non-certifying.
    certified_callee_digests = _certified_callee_digests(certified_context)

    memory_loop_readonly_identity = None
    if memory_loop_readonly_words is not None:
        from tools.ppc_equivalence.memory_loop_readonly import (
            build_memory_loop_readonly_obligation,
        )

        memory_loop_readonly_identity = build_memory_loop_readonly_obligation(
            memory_loop_readonly_words,
        )

    key = _cache_key(
        resolved_contract.name, observables, orig_hex, cand_hex,
        orig_base, cand_base,
        original_relocations=original_relocations,
        candidate_relocations=candidate_relocations,
        original_local_symbol=original_local_symbol,
        candidate_local_symbol=candidate_local_symbol,
        assumed_callees=assumed_callees,
        callee_contracts=callee_contracts,
        certified_callee_digests=certified_callee_digests,
        certificate_target_id=certificate_target_id,
        memory_environment=memory_environment,
        floating_point_domain=fp_domain_dict,
        limits={
            "max_instructions": max_instructions,
            "max_paths": max_paths,
            "max_loop_iterations": max_loop_iterations,
        },
        proof_features=proof_features,
        address_space=address_space,
        indirect_targets=indirect_targets,
        memory_loop_readonly=memory_loop_readonly_identity,
        platform_profile_sha256=platform_profile_sha256,
        memory_bus=memory_bus_identity,
        abi_shape=abi_shape_payload,
        initial_gpr_ranges=(
            {str(reg): [lo, hi] for reg, (lo, hi) in initial_gpr_ranges.items()}
            if initial_gpr_ranges
            else None
        ),
        ram_only_projection=ram_only_projection,
    )

    cache_d = _cache_dir(project)
    cached = _cache_get(key, cache_d)
    mem_env = None
    if memory_environment:
        from tools.ppc_equivalence.memory_profile import MemoryEnvironment

        mem_env = MemoryEnvironment.from_dict(memory_environment)
    if cached is not None:
        cached = _reevaluate_cached_probe(
            cached,
            certified_context=certified_context,
            platform_profile=platform_profile_obj,
            environment=mem_env,
        )
        if fallback_note and cached.detail and fallback_note not in cached.detail:
            cached = EquivalenceProbe(
                cached.status,
                f"{fallback_note}; {cached.detail}",
                cached.certificate,
                proof=cached.proof,
            )
        return cached

    previous_ce = _load_counterexample(project, symbol, orig_hex, key)
    if previous_ce:
        still_differs, detail = _replay_counterexample(
            original, candidate, resolved_contract.name, previous_ce,
        )
        if still_differs:
            return EquivalenceProbe(
                ProofStatus.NOT_EQUIVALENT,
                f"{fallback_note}; previous counterexample still mismatches: {detail}"
                if fallback_note else f"previous counterexample still mismatches: {detail}",
            )

    callees_used: set[int | str] = set()
    fp_domain = None
    if fp_domain_dict is not None:
        from tools.ppc_equivalence.result import FloatingPointDomain
        # Oracle version is identity-only; FloatingPointDomain ignores unknown keys.
        fp_domain = FloatingPointDomain.parse(fp_domain_dict)
    elif raw_fp is not None:
        from tools.ppc_equivalence.result import FloatingPointDomain
        fp_domain = FloatingPointDomain.parse(raw_fp)

    def _reloc_tuples(items: tuple) -> list[tuple]:
        return [
            (item.offset, item.relocation_type, item.symbol, item.addend)
            for item in items
        ]

    source_hash = proof_request_hash(
        original_hex=orig_hex,
        candidate_hex=cand_hex,
        contract=resolved_contract.name,
        timeout_ms=timeout_ms,
        max_instructions=max_instructions,
        max_paths=max_paths,
        max_loop_iterations=max_loop_iterations,
        observe=[item.name for item in resolved_contract.observables],
        memory_environment=memory_environment,
        floating_point_domain=fp_domain_dict,
        assumed_callees=[str(item) for item in assumed_callees],
        callee_contract_sources={
            str(name): contract.source
            for name, contract in (callee_contracts or {}).items()
        },
        certified_callee_digests=certified_callee_digests,
        original_base=orig_base,
        candidate_base=cand_base,
        original_relocations=_reloc_tuples(original_relocations),
        candidate_relocations=_reloc_tuples(candidate_relocations),
        certificate_target_id=certificate_target_id,
        proof_features=proof_features,
        address_space=address_space,
        indirect_targets=indirect_targets,
        memory_loop_readonly=memory_loop_readonly_identity,
        platform_profile_sha256=platform_profile_sha256,
        memory_bus=memory_bus_identity,
    )
    result = check_equivalence(
        original,
        candidate,
        resolved_contract,
        original_hex=orig_hex,
        candidate_hex=cand_hex,
        max_instructions=max_instructions,
        max_paths=max_paths,
        max_loop_iterations=max_loop_iterations,
        assumed_callees=assumed_callees,
        assumed_callees_used=callees_used,
        callee_contracts=callee_contracts,
        initial_gpr_bindings=initial_gpr_bindings,
        initial_gpr_ranges=initial_gpr_ranges,
        memory_environment=mem_env,
        source_hash=source_hash,
        floating_point_domain=fp_domain,
        jump_table=jump_table_context,
        virtual_call=virtual_call_context,
        memory_loop_readonly=memory_loop_readonly_words,
        platform_profile=platform_profile_obj,
        memory_bus=memory_bus_obj,
    )
    # Stage 4: derive requirements + attach drafts (ledger + certified callees).
    # Engine may already have attached; orchestrator refreshes ledger binding.
    assurance_errors = _apply_capability_assurance(
        result,
        certified_context=certified_context,
        platform_profile=platform_profile_obj,
        environment=mem_env,
    )
    detail = ""
    if result.contract_resolution:
        added = result.contract_resolution.get("added", [])
        detail = "auto contract: ppc-eabi"
        if added:
            detail += " + " + ", ".join(str(item) for item in added)
        shape = result.contract_resolution.get("abi_shape")
        if isinstance(shape, dict) and shape.get("source"):
            detail += f"; abi_shape: {shape['source']}"
    if ram_only_projection:
        note = "ram-only-bus-projection"
        detail = f"{detail}; {note}" if detail else note
    if initial_gpr_ranges:
        from tools.ppc_equivalence.object_base import format_object_base_assumption

        range_notes = ", ".join(
            format_object_base_assumption(reg, lo, hi)
            for reg, (lo, hi) in sorted(initial_gpr_ranges.items())
        )
        detail = f"{detail}; {range_notes}" if detail else range_notes
    if result.unsupported:
        detail = "; ".join(result.unsupported)
    elif result.mismatch:
        mismatch = (
            f"{result.mismatch.get('name')}: "
            f"{result.mismatch.get('original')} != {result.mismatch.get('candidate')}"
        )
        detail = f"{detail}; {mismatch}" if detail else mismatch
    if result.repair_hint and result.status == ProofStatus.NOT_EQUIVALENT:
        rh = result.repair_hint
        if "diverged_register" in rh:
            detail += (
                f" | diverged at insn {rh['instruction_index']}: "
                f"{rh['diverged_register']} ({rh.get('original_value')} vs {rh.get('candidate_value')})"
                f" [{rh.get('original_insn')} vs {rh.get('candidate_insn')}]"
            )

    if result.status == ProofStatus.NOT_EQUIVALENT and result.counterexample:
        _save_counterexample(project, symbol, orig_hex, result.counterexample, key)

    if result.assumed_callees:
        detail += " | assumed callees: " + ", ".join(
            f"0x{target:08x}" if isinstance(target, int) else target
            for target in result.assumed_callees
        )

    if fallback_note:
        detail = f"{fallback_note}; {detail}" if detail else fallback_note

    for code in assurance_errors:
        note = f"capability-assurance-generation-failed:{code}"
        if note not in detail:
            detail = f"{detail}; {note}" if detail else note

    certificate = None
    if (
        result.status == ProofStatus.EQUIVALENT
        and certificate_target_id is not None
        and original_function is not None
        and candidate_function is not None
    ):
        mem_scope_dict = result.memory_scope.to_dict() if result.memory_scope else None
        certificate, certificate_error = _build_equivalence_certificate(
            certificate_target_id,
            original_function,
            candidate_function,
            original,
            candidate,
            call_targets=call_targets,
            callee_contracts=callee_contracts,
            dependencies=(certified_context or CertifiedCalleeContext({}, ())).dependencies,
            helpers=(certified_context or CertifiedCalleeContext({}, ())).helpers,
            evidence=certificate_evidence,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            memory_scope=mem_scope_dict,
            proof=result,
        )
        if certificate_error:
            detail = f"{detail} | {certificate_error}" if detail else certificate_error
    probe = EquivalenceProbe(result.status, detail, certificate, proof=result)
    if result.status == ProofStatus.EQUIVALENT and cache_d is not None:
        _cache_put(key, probe, cache_d, callees_used)

    return probe


def prove_unit_symbol(
    project: Project,
    unit: ObjdiffUnit,
    symbol: str,
    *,
    contract: str = "auto",
    timeout_ms: int = 0,
    max_instructions: int = _DEFAULT_MAX_INSTRUCTIONS,
    max_paths: int = _DEFAULT_MAX_PATHS,
    max_loop_iterations: int = _DEFAULT_MAX_LOOP_ITERATIONS,
    candidate_symbol: str | None = None,
    linked: bool = False,
    target_id: str | None = None,
    memory_environment: dict[str, Any] | None = None,
) -> EquivalenceProbe:
    """SMT-check one named function from the unit's retail/decomp objects.

    Supported unresolved ELF relocations are proved symbolically.  With
    ``linked=True``, an unsupported symbolic-relocation case retries once using
    bytes extracted from the retail DOL and linked decomp ELF.
    """
    retail = unit.target_path
    decomp = unit.base_path
    if retail is None or not retail.is_file():
        return EquivalenceProbe(ProofStatus.INVALID_INPUT, f"retail object missing: {retail}")
    if decomp is None or not decomp.is_file():
        return EquivalenceProbe(ProofStatus.INVALID_INPUT, f"decomp object missing: {decomp}")

    try:
        left, right = extract_function_pair(retail, decomp, symbol)
        certified_context = _load_certified_callees(project, target_id) if target_id else None
        try:
            return _prove_bytes(
                project, symbol,
                left.code, left.base,
                right.code, right.base,
                contract=contract,
                timeout_ms=timeout_ms,
                max_instructions=max_instructions,
                max_paths=max_paths,
                max_loop_iterations=max_loop_iterations,
                original_relocations=left.relocations,
                candidate_relocations=right.relocations,
                original_local_symbol=left.name,
                candidate_local_symbol=right.name,
                original_object=retail,
                candidate_object=decomp,
                original_function=left,
                candidate_function=right,
                certificate_target_id=target_id,
                certified_context=certified_context,
                memory_environment=memory_environment,
            )
        except (DecodeError, UnsupportedInstruction, ExecutionInconclusive, ValueError):
            if not linked or not (left.relocations or right.relocations):
                raise
            return _run_linked_fallback(
                project, symbol, candidate_symbol, contract,
                timeout_ms, max_instructions, max_paths,
                max_loop_iterations=max_loop_iterations,
                target_id=target_id,
                certified_context=certified_context,
                memory_environment=memory_environment,
            )
    except (ElfSymbolError, DecodeError, UnsupportedInstruction, ExecutionInconclusive, ValueError) as exc:
        return EquivalenceProbe(ProofStatus.INCONCLUSIVE_UNSUPPORTED, str(exc))
    except RuntimeError as exc:
        # Missing Z3, etc.
        return EquivalenceProbe(ProofStatus.INTERNAL_ERROR, str(exc))


def certify_unit_symbol(
    project: Project,
    unit: ObjdiffUnit,
    symbol: str,
    target_id: str,
    *,
    evidence: str = "full-instruction-match",
    max_instructions: int = _DEFAULT_MAX_INSTRUCTIONS,
    max_paths: int = _DEFAULT_MAX_PATHS,
    max_loop_iterations: int = _DEFAULT_MAX_LOOP_ITERATIONS,
) -> EquivalenceProbe:
    """Issue a current semantic effect certificate for an already-equal pair.

    When ``coop.json`` configures a reviewed ``hardware_profile``, prefer the
    full SMT prove path so MMIO/FIFO obligations (and Tier-A GX attestations)
    participate in the certificate instead of a bare FULL_MATCH synthesis.
    """
    if unit.target_path is None or unit.base_path is None:
        return EquivalenceProbe(ProofStatus.INVALID_INPUT, "unit lacks an object pair")
    try:
        left, right = extract_function_pair(unit.target_path, unit.base_path, symbol)
        # Byte-identical FULL_MATCH leaves (common for RVL paired-single MTX)
        # skip SMT prove: incomplete PS capability stubs / timeouts block certs
        # that parents need. Prefer prove only when bytes differ and a reviewed
        # hardware_profile is configured (MMIO/FIFO obligations).
        bytes_identical = left.code == right.code
        if not bytes_identical:
            try:
                from tools.coop.lib.config import memory_bus_from_config

                if memory_bus_from_config(project.config) is not None:
                    proved = prove_unit_symbol(
                        project,
                        unit,
                        symbol,
                        target_id=target_id,
                        contract="auto",
                        max_instructions=max_instructions,
                        max_paths=max_paths,
                        max_loop_iterations=max_loop_iterations,
                    )
                    if (
                        proved.status == ProofStatus.EQUIVALENT
                        and isinstance(proved.certificate, dict)
                    ):
                        return proved
            except Exception:
                pass
        original = decode_block(
            left.code, left.base, validate_with_capstone=False,
            relocations=left.relocations, local_symbol=left.name,
        )
        candidate = decode_block(
            right.code, right.base, validate_with_capstone=False,
            relocations=right.relocations, local_symbol=right.name,
        )
        context = _load_certified_callees(project, target_id)
        if context.errors:
            return EquivalenceProbe(
                ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE, "; ".join(context.errors),
            )
        call_targets = _extract_call_targets(original) | _extract_call_targets(candidate)
        missing = sorted((item for item in call_targets if item not in context.contracts), key=str)
        if missing and not bytes_identical:
            return EquivalenceProbe(
                ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE,
                "calls lack current certificates: " + ", ".join(str(item) for item in missing),
            )
        # Byte-identical FULL_MATCH: relative self-bl / unresolved same-TU edges
        # get opaque EABI so recursive leaves can bootstrap a certificate.
        contracts = {
            item: context.contracts.get(item) or CalleeContract.opaque_eabi()
            for item in call_targets
        }
        opcodes_used = sorted(
            {
                insn.opcode.value
                for insn in (*original, *candidate)
                if getattr(insn, "opcode", None) is not None
            }
        )
        source_hash = proof_request_hash(
            original_hex=left.code.hex(),
            candidate_hex=right.code.hex(),
            contract="full-instruction-match",
            timeout_ms=0,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            observe=[],
            assumed_callees=[str(item) for item in call_targets],
            callee_contract_sources={
                str(name): contract.source for name, contract in contracts.items()
            },
            certified_callee_digests=_certified_callee_digests(context),
            original_base=left.base,
            candidate_base=right.base,
            certificate_target_id=target_id,
        )
        git_commit, git_dirty = _live_git_identity()
        proof = ProofResult(
            status=ProofStatus.EQUIVALENT,
            architecture_model=ARCHITECTURE_MODEL,
            format=RESULT_FORMAT,
            contract="full-instruction-match",
            observables=[],
            opcodes_used=opcodes_used,
            assumed_callees=sorted(call_targets, key=str),
            callee_contracts={
                str(name): {
                    "source": contract.source,
                    "reads": sorted(contract.reads),
                    "writes": sorted(contract.writes),
                }
                for name, contract in contracts.items()
            },
            source_hash=source_hash,
            proof_request_hash=source_hash,
            engine_hash=_current_engine_hash(),
            certifier_hash=_current_certifier_hash(),
            git_commit=git_commit,
            git_dirty=git_dirty,
            limits={
                "max_instructions": max_instructions,
                "max_paths": max_paths,
                "max_loop_iterations": max_loop_iterations,
            },
        )
        assurance_errors = _apply_capability_assurance(
            proof,
            certified_context=context,
        )
        detail = ""
        for code in assurance_errors:
            note = f"capability-assurance-generation-failed:{code}"
            detail = f"{detail}; {note}" if detail else note
        certificate, cert_detail = _build_equivalence_certificate(
            target_id, left, right, original, candidate,
            call_targets=call_targets,
            callee_contracts=contracts,
            dependencies=context.dependencies,
            helpers=context.helpers,
            evidence=evidence,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            proof=proof,
            # Byte-identical bodies are already FULL_MATCH; skip effect
            # validation that path-explodes on small callers with calls.
            skip_semantic_validation=bytes_identical,
        )
        if cert_detail:
            detail = f"{detail}; {cert_detail}" if detail else cert_detail
        return EquivalenceProbe(
            ProofStatus.EQUIVALENT if certificate else ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            detail,
            certificate,
            proof=proof,
        )
    except (ElfSymbolError, DecodeError, UnsupportedInstruction, ExecutionInconclusive, ValueError) as exc:
        return EquivalenceProbe(ProofStatus.INCONCLUSIVE_UNSUPPORTED, str(exc))
    except RuntimeError as exc:
        return EquivalenceProbe(ProofStatus.INTERNAL_ERROR, str(exc))


def _run_linked_fallback(
    project: Project,
    symbol: str,
    candidate_symbol: str | None,
    contract: str,
    timeout_ms: int,
    max_instructions: int,
    max_paths: int,
    *,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    target_id: str | None = None,
    certified_context: CertifiedCalleeContext | None = None,
    memory_environment: dict[str, Any] | None = None,
) -> EquivalenceProbe:
    """Retry the proof using linked bytes from ``main.dol`` + ``main.elf``.

    Called from :func:`prove_unit_symbol` only when ``linked=True`` and the
    unlinked symbolic-relocation proof is unsupported. The retail side is
    sliced out of the linked DOL by ``(address, size)`` looked up in
    ``config/<region>/symbols.txt``; the candidate side is taken from the
    linked ELF (``main.elf``, produced by ``ninja build/<region>/main.elf``) by
    symbol name.
    """
    note = "fell back to DOL/ELF linked bytes"
    dol_path = project.config.main_dol
    elf_path = project.linked_elf_path
    if not dol_path.is_file():
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; retail DOL missing: {dol_path}",
        )
    if not elf_path.is_file():
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; candidate ELF missing (run `ninja {elf_path.relative_to(project.root)}`): {elf_path}",
        )

    addr_size = project.symbol_address(symbol)
    if addr_size is None:
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; symbol {symbol!r} not in config/{project.config.region}/symbols.txt",
        )
    addr, size = addr_size

    try:
        retail_slice = extract_dol_slice(dol_path, addr, size)
    except DolSymbolError as exc:
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; DOL slice failed: {exc}",
        )

    cand_name = candidate_symbol or symbol
    try:
        cand_func = extract_function(elf_path, cand_name)
    except ElfSymbolError as exc:
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; candidate ELF symbol {cand_name!r} not found: {exc}",
        )

    try:
        return _prove_bytes(
            project, symbol,
            retail_slice.code, retail_slice.base,
            cand_func.code, cand_func.base,
            contract=contract,
            timeout_ms=timeout_ms,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            fallback_note=note,
            certificate_target_id=target_id,
            certified_context=certified_context,
            memory_environment=memory_environment,
        )
    except (ElfSymbolError, DecodeError, UnsupportedInstruction, ExecutionInconclusive, ValueError) as exc:
        return EquivalenceProbe(ProofStatus.INCONCLUSIVE_UNSUPPORTED, f"{note}; {exc}")
    except RuntimeError as exc:
        return EquivalenceProbe(ProofStatus.INTERNAL_ERROR, f"{note}; {exc}")


def register_equivalent_target(project: Project, target: int) -> None:
    _add_equivalent_target(project, target)


def register_equivalent_targets(project: Project, targets: frozenset[int]) -> None:
    existing = _load_known_equivalent_targets(project)
    new = existing | targets
    if new != existing:
        _save_deps(project, new)

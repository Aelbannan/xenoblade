"""Run ppc_equivalence against an objdiff retail/decomp function pair."""

from __future__ import annotations

import hashlib
import json
import re
import time
from dataclasses import dataclass
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
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT, ProofStatus
from tools.ppc_equivalence.provenance import (
    canonical_obligation_dict,
    hash_certifier_tree,
    hash_engine_tree,
    proof_request_hash,
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

# Auto-scale: 20 ms per instruction, floor 5 s, ceiling 120 s.
_TIMEOUT_MS_MIN = 5_000
_TIMEOUT_MS_MAX = 120_000
_TIMEOUT_MS_PER_INSN = 20

_REPO_ROOT = Path(__file__).resolve().parents[3]


def _current_engine_hash() -> str:
    return hash_engine_tree(_REPO_ROOT)


def _current_certifier_hash() -> str:
    return hash_certifier_tree(_REPO_ROOT)


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
    for dependency in certificate.get("callees", []):
        callee_id = dependency["target_id"]
        error = _reattest_certificate_tree(project, callee_id, by_id, memo, active)
        if error:
            return f"callee {callee_id!r}: {error}"
    memo[target_id] = None
    return None


def _load_certified_callees(project: Project, target_id: str) -> CertifiedCalleeContext:
    """Load and re-attest every direct callee declared by the target registry."""
    document = load_targets_document(project.config)
    rows = [row for row in document.get("targets", []) if isinstance(row, dict)]
    by_id = {str(row.get("id")): row for row in rows if isinstance(row.get("id"), str)}
    caller = by_id.get(target_id)
    if caller is None:
        return CertifiedCalleeContext({}, (), (f"unknown target id {target_id!r}",))
    errors: list[str] = []
    if caller.get("unresolved_called_functions"):
        errors.append("registry has unresolved direct callees")
    if caller.get("has_indirect_calls"):
        errors.append("registry has an unresolved indirect call")
    called_ids = caller.get("called_functions", [])
    if not isinstance(called_ids, list):
        return CertifiedCalleeContext({}, (), ("called_functions is not an array",))

    contracts: dict[int | str, CalleeContract] = {}
    dependencies: list[dict[str, str]] = []
    attestation_memo: dict[str, str | None] = {}
    for callee_id in called_ids:
        callee = by_id.get(str(callee_id))
        if callee is None:
            errors.append(f"unknown callee target {callee_id!r}")
            continue
        if callee.get("status") not in ACCEPTED_MATCH_STATUSES:
            errors.append(f"callee {callee_id!r} is not accepted")
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
        if isinstance(address, str):
            try:
                contracts[int(address, 0)] = contract
            except ValueError:
                pass
        dependencies.append({
            "target_id": str(callee_id),
            "certificate_sha256": certificate["certificate_sha256"],
        })
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
    return CertifiedCalleeContext(
        contracts,
        tuple(sorted(dependencies, key=lambda item: item["target_id"])),
        tuple(errors),
        tuple(sorted(helpers)),
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
    certificate_target_id: str | None = None,
    memory_environment: dict[str, Any] | None = None,
    floating_point_domain: dict[str, Any] | None = None,
    limits: dict[str, int] | None = None,
    engine_hash: str | None = None,
    certifier_hash: str | None = None,
    proof_features: list[str] | None = None,
    address_space: dict[str, Any] | None = None,
    indirect_targets: dict[str, Any] | None = None,
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
    if proof_features is not None:
        payload["proof_features"] = sorted(proof_features)
    if address_space is not None:
        payload["address_space"] = canonical_obligation_dict(address_space)
    if indirect_targets is not None:
        payload["indirect_targets"] = canonical_obligation_dict(indirect_targets)
    return hashlib.sha256(
        json.dumps(payload, sort_keys=True).encode(),
    ).hexdigest()


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
        proof = data.get("proof_audit")
        return EquivalenceProbe(
            status, data.get("detail", ""),
            certificate if isinstance(certificate, dict) else None,
            proof=proof if isinstance(proof, dict) else None,
        )
    except (KeyError, ValueError, json.JSONDecodeError):
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
        "engine_hash",
        "source_hash",
        "git_commit",
        "git_dirty",
        "repair_hint",
        "unsupported",
        "warnings",
        "abstractions",
        "proof_features",
        "address_space",
        "indirect_targets",
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
) -> tuple[dict | None, str]:
    """Derive and validate a normal-return semantic effect summary."""
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
        if not validation.valid:
            detail = validation.reason or (
                "contract omitted reads " + ", ".join(sorted(validation.missing_reads))
                + "; writes " + ", ".join(sorted(validation.missing_writes))
            )
            return None, f"{side} certificate validation failed: {detail}"
    reads = sorted(validations[0].required_reads | validations[1].required_reads)
    writes = sorted(validations[0].required_writes | validations[1].required_writes)
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
    if memory_scope is not None:
        certificate["memory_scope"] = memory_scope
    # Always bind the live engine and certifier trees so certificates fail closed
    # after semantic or policy edits.
    certificate["engine_hash"] = _current_engine_hash()
    certificate["certifier_hash"] = _current_certifier_hash()
    if proof is not None:
        engine_hash = getattr(proof, "engine_hash", "") or ""
        source_hash = getattr(proof, "source_hash", "") or ""
        git_commit = getattr(proof, "git_commit", "") or ""
        if engine_hash:
            certificate["engine_hash"] = engine_hash
        if source_hash:
            certificate["source_hash"] = source_hash
        if git_commit:
            certificate["git_commit"] = git_commit
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
        address_space = getattr(proof, "address_space", None)
        if address_space is not None:
            certificate["address_space"] = dict(address_space)
        indirect_targets = getattr(proof, "indirect_targets", None)
        if indirect_targets is not None:
            certificate["indirect_targets"] = dict(indirect_targets)
        loop_summary = getattr(proof, "loop_summary", None) if proof is not None else None
        if loop_summary is not None:
            certificate["loop_summary"] = dict(loop_summary)
        relational_induction = (
            getattr(proof, "relational_induction", None) if proof is not None else None
        )
        if relational_induction is not None:
            certificate["relational_induction"] = dict(relational_induction)
    certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)
    return certificate, ""


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
    if proof_features is None and address_space is None and indirect_targets is None:
        from tools.ppc_equivalence.jump_table_auto import try_auto_jump_table_context
        from tools.ppc_equivalence.jump_table_obligations import (
            build_indirect_targets_obligation,
            build_readonly_image_obligation,
        )

        dol_path = None
        elf_path = None
        if project is not None:
            configured_dol = getattr(project.config, "main_dol", None)
            if configured_dol is not None and Path(configured_dol).is_file():
                dol_path = Path(configured_dol)
            linked_elf = getattr(project, "linked_elf_path", None)
            if linked_elf is not None and Path(linked_elf).is_file():
                elf_path = Path(linked_elf)
        jump_table_context = try_auto_jump_table_context(
            original,
            candidate,
            dol_path=dol_path,
            elf_path=elf_path,
            original_dol_path=dol_path,
            candidate_elf_path=elf_path,
        )
        if jump_table_context is not None:
            proof_features = ["readonly-image", "indirect-target-closure"]
            address_space = build_readonly_image_obligation(
                jump_table_context.table,
                no_write_status="pending",
            )
            indirect_targets = build_indirect_targets_obligation(
                branch_pc=jump_table_context.branch_pc,
                targets=tuple(
                    (f"case-{index}", word & 0xFFFFFFFC)
                    for index, word in enumerate(jump_table_context.table.words)
                ),
                source=jump_table_context.table.source,
                artifact_hashes=(jump_table_context.table.image_sha256,),
                coverage="pending",
            )

    original_live_out = automatic_live_out(original)
    candidate_live_out = automatic_live_out(candidate)
    live_out = None
    if contract == "live-out":
        live_out = tuple(dict.fromkeys(original_live_out + candidate_live_out))

    if timeout_ms <= 0:
        instr_count = max(len(original), len(candidate))
        timeout_ms = max(_TIMEOUT_MS_MIN, min(_TIMEOUT_MS_MAX, instr_count * _TIMEOUT_MS_PER_INSN))

    resolved_contract = make_contract(
        preset=contract,
        observe=None,
        timeout_ms=timeout_ms,
        live_out=live_out,
        original_live_out=original_live_out,
        candidate_live_out=candidate_live_out,
    )

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
    raw_fp = floating_point_domain
    if raw_fp is None and project is not None:
        raw_fp = getattr(project.config, "floating_point_domain", None)
    fp_domain_dict: dict[str, Any] | None = None
    if isinstance(raw_fp, dict):
        fp_domain_dict = raw_fp
    elif raw_fp is not None:
        from tools.ppc_equivalence.result import FloatingPointDomain

        fp_domain_dict = FloatingPointDomain.parse(raw_fp).to_dict()

    key = _cache_key(
        resolved_contract.name, observables, orig_hex, cand_hex,
        orig_base, cand_base,
        original_relocations=original_relocations,
        candidate_relocations=candidate_relocations,
        original_local_symbol=original_local_symbol,
        candidate_local_symbol=candidate_local_symbol,
        assumed_callees=assumed_callees,
        callee_contracts=callee_contracts,
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
    )

    cache_d = _cache_dir(project)
    cached = _cache_get(key, cache_d)
    if cached is not None:
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
    mem_env = None
    if memory_environment:
        from tools.ppc_equivalence.memory_profile import MemoryEnvironment
        mem_env = MemoryEnvironment.from_dict(memory_environment)
    fp_domain = None
    if raw_fp is not None:
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
        floating_point_domain=(
            fp_domain.to_dict() if fp_domain is not None else None
        ),
        assumed_callees=[str(item) for item in assumed_callees],
        callee_contract_sources={
            str(name): contract.source
            for name, contract in (callee_contracts or {}).items()
        },
        original_base=orig_base,
        candidate_base=cand_base,
        original_relocations=_reloc_tuples(original_relocations),
        candidate_relocations=_reloc_tuples(candidate_relocations),
        certificate_target_id=certificate_target_id,
        proof_features=proof_features,
        address_space=address_space,
        indirect_targets=indirect_targets,
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
        memory_environment=mem_env,
        source_hash=source_hash,
        floating_point_domain=fp_domain,
        jump_table=jump_table_context,
    )
    detail = ""
    if result.contract_resolution:
        added = result.contract_resolution.get("added", [])
        detail = "auto contract: ppc-eabi"
        if added:
            detail += " + " + ", ".join(str(item) for item in added)
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
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
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
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
) -> EquivalenceProbe:
    """Issue a current semantic effect certificate for an already-equal pair."""
    if unit.target_path is None or unit.base_path is None:
        return EquivalenceProbe(ProofStatus.INVALID_INPUT, "unit lacks an object pair")
    try:
        left, right = extract_function_pair(unit.target_path, unit.base_path, symbol)
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
        if missing:
            return EquivalenceProbe(
                ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE,
                "calls lack current certificates: " + ", ".join(str(item) for item in missing),
            )
        contracts = {item: context.contracts[item] for item in call_targets}
        certificate, detail = _build_equivalence_certificate(
            target_id, left, right, original, candidate,
            call_targets=call_targets,
            callee_contracts=contracts,
            dependencies=context.dependencies,
            helpers=context.helpers,
            evidence=evidence,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
        )
        return EquivalenceProbe(
            ProofStatus.EQUIVALENT if certificate else ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            detail,
            certificate,
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

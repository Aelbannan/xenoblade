import hashlib
import json
from pathlib import Path


# Declared trust-boundary inputs for engine-tree provenance (P1-08).
# Paths are relative to repo root and matched deterministically.
ENGINE_SOURCE_PATTERNS = [
    "tools/ppc_equivalence/*.py",
    "tools/ppc_equivalence/generators/**/*.py",
    "tools/ppc_equivalence/fixtures/*.py",
    "tools/ppc_equivalence/requirements.lock",
    "tools/ppc_equivalence/validation_ledger.yaml",
    "tools/ppc_equivalence/validation_ledger.json",
]


def _collect_engine_paths(repo_root: Path) -> list[Path]:
    """Return sorted unique paths included in the engine-tree hash."""
    base = repo_root / "tools" / "ppc_equivalence"
    candidates: list[Path] = []

    candidates.extend(p for p in base.glob("*.py") if p.is_file())

    generators = base / "generators"
    if generators.is_dir():
        candidates.extend(p for p in generators.rglob("*.py") if p.is_file())

    fixtures = base / "fixtures"
    if fixtures.is_dir():
        # Hash Python helpers only — not giant JSONL corpora.
        candidates.extend(p for p in fixtures.glob("*.py") if p.is_file())

    lock = base / "requirements.lock"
    if lock.is_file():
        candidates.append(lock)

    for name in ("validation_ledger.yaml", "validation_ledger.json"):
        ledger = base / name
        if ledger.is_file():
            candidates.append(ledger)

    by_rel = {
        path.relative_to(repo_root).as_posix(): path
        for path in candidates
    }
    return [by_rel[key] for key in sorted(by_rel)]


def hash_engine_tree(repo_root: Path) -> str:
    """Deterministic SHA-256 over engine trust-boundary sources."""
    digest = hashlib.sha256()
    for path in _collect_engine_paths(repo_root):
        relative = path.relative_to(repo_root).as_posix().encode("utf-8")
        content = path.read_bytes()
        digest.update(len(relative).to_bytes(4, "big"))
        digest.update(relative)
        digest.update(len(content).to_bytes(8, "big"))
        digest.update(content)
    return digest.hexdigest()


def canonical_json_sha256(value: object) -> str:
    payload = json.dumps(
        value, sort_keys=True, separators=(",", ":"), ensure_ascii=True, allow_nan=False,
    ).encode("utf-8")
    return hashlib.sha256(payload).hexdigest()


def proof_request_identity(
    *,
    original_hex: str,
    candidate_hex: str,
    contract: str,
    timeout_ms: int | None = None,
    max_instructions: int | None = None,
    max_paths: int | None = None,
    max_loop_iterations: int | None = None,
    observe: list[str] | None = None,
    memory_profile: str | None = None,
    memory_ranges: list[str] | None = None,
    memory_environment: object | None = None,
    floating_point_domain: object | None = None,
    assumed_callees: list[str] | None = None,
    callee_contract_sources: dict[str, str] | None = None,
    original_base: int | None = None,
    candidate_base: int | None = None,
    original_relocations: list | None = None,
    candidate_relocations: list | None = None,
    certificate_target_id: str | None = None,
) -> dict:
    """Canonical proof-request fields hashed into ``ProofResult.source_hash``.

    Callers should pass every premise that can change the theorem; omit only
    fields that are not part of the request (leave them ``None`` so they are
    dropped rather than hashed as null).
    """
    payload: dict = {
        "original_hex": original_hex,
        "candidate_hex": candidate_hex,
        "contract": contract,
    }
    if timeout_ms is not None:
        payload["timeout_ms"] = timeout_ms
    if max_instructions is not None:
        payload["max_instructions"] = max_instructions
    if max_paths is not None:
        payload["max_paths"] = max_paths
    if max_loop_iterations is not None:
        payload["max_loop_iterations"] = max_loop_iterations
    if observe is not None:
        payload["observe"] = sorted(observe)
    if memory_profile is not None:
        payload["memory_profile"] = memory_profile
    if memory_ranges is not None:
        payload["memory_ranges"] = sorted(memory_ranges)
    if memory_environment is not None:
        payload["memory_environment"] = memory_environment
    if floating_point_domain is not None:
        payload["floating_point_domain"] = floating_point_domain
    if assumed_callees is not None:
        payload["assumed_callees"] = sorted(assumed_callees, key=str)
    if callee_contract_sources is not None:
        payload["callee_contract_sources"] = {
            str(name): source
            for name, source in sorted(
                callee_contract_sources.items(), key=lambda item: str(item[0])
            )
        }
    if original_base is not None:
        payload["original_base"] = original_base
    if candidate_base is not None:
        payload["candidate_base"] = candidate_base
    if original_relocations is not None:
        payload["original_relocations"] = original_relocations
    if candidate_relocations is not None:
        payload["candidate_relocations"] = candidate_relocations
    if certificate_target_id is not None:
        payload["certificate_target_id"] = certificate_target_id
    return payload


def proof_request_hash(**kwargs: object) -> str:
    """SHA-256 of :func:`proof_request_identity` kwargs."""
    return canonical_json_sha256(proof_request_identity(**kwargs))  # type: ignore[arg-type]

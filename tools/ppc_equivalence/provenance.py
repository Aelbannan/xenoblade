import hashlib
import json
import subprocess
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

# Coop certifier/policy trust boundary (separate from engine_hash so solver
# provenance stays scoped to tools/ppc_equivalence/**).
CERTIFIER_SOURCE_PATHS = [
    "tools/coop/lib/equivalence_check.py",
    "tools/coop/lib/equivalence_policy.py",
    "tools/coop/lib/targets.py",
]

# Print live engine hash (for coop.json allowed_engine_sha256 canary pins):
#   python3 -c "from pathlib import Path; from tools.ppc_equivalence.provenance import hash_engine_tree; print(hash_engine_tree(Path('.')))"


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


def _hash_source_tree(repo_root: Path, paths: list[Path]) -> str:
    """Deterministic SHA-256 over sorted repo-relative source paths."""
    digest = hashlib.sha256()
    for path in paths:
        relative = path.relative_to(repo_root).as_posix().encode("utf-8")
        content = path.read_bytes()
        digest.update(len(relative).to_bytes(4, "big"))
        digest.update(relative)
        digest.update(len(content).to_bytes(8, "big"))
        digest.update(content)
    return digest.hexdigest()


def _collect_certifier_paths(repo_root: Path) -> list[Path]:
    """Return sorted coop certifier/policy paths included in certifier_hash."""
    paths: list[Path] = []
    for relative in CERTIFIER_SOURCE_PATHS:
        path = repo_root / relative
        if not path.is_file():
            raise FileNotFoundError(f"certifier trust-boundary source missing: {relative}")
        paths.append(path)
    return paths


def hash_engine_tree(repo_root: Path) -> str:
    """Deterministic SHA-256 over engine trust-boundary sources."""
    return _hash_source_tree(repo_root, _collect_engine_paths(repo_root))


def hash_certifier_tree(repo_root: Path) -> str:
    """Deterministic SHA-256 over coop certifier/policy trust-boundary sources."""
    return _hash_source_tree(repo_root, _collect_certifier_paths(repo_root))


def is_trust_boundary_relative_path(relative: str) -> bool:
    """True if *relative* (repo-root posix path) is engine or certifier TCB.

    Inclusion mirrors :func:`_collect_engine_paths` /
    :func:`_collect_certifier_paths` (not whole-repo dirt).
    """
    normalized = relative.replace("\\", "/").lstrip("./")
    if normalized in CERTIFIER_SOURCE_PATHS:
        return True
    prefix = "tools/ppc_equivalence/"
    if not normalized.startswith(prefix):
        return False
    rest = normalized[len(prefix) :]
    if rest in (
        "requirements.lock",
        "validation_ledger.yaml",
        "validation_ledger.json",
    ):
        return True
    if "/" not in rest and rest.endswith(".py"):
        # tools/ppc_equivalence/*.py
        return True
    if rest.startswith("generators/") and rest.endswith(".py"):
        # tools/ppc_equivalence/generators/**/*.py
        return True
    if rest.startswith("fixtures/") and rest.endswith(".py"):
        # tools/ppc_equivalence/fixtures/*.py (no nested dirs hashed)
        fixture_name = rest[len("fixtures/") :]
        return "/" not in fixture_name
    return False


def _unquote_git_path(path: str) -> str:
    path = path.strip()
    if len(path) >= 2 and path[0] == '"' and path[-1] == '"':
        return bytes(path[1:-1], "utf-8").decode("unicode_escape")
    return path


def _iter_porcelain_paths(porcelain: str) -> list[str]:
    """Extract repo-relative paths from ``git status --porcelain`` output."""
    paths: list[str] = []
    for raw_line in porcelain.splitlines():
        line = raw_line.rstrip("\r")
        if len(line) < 4:
            continue
        # Porcelain v1: XY<space>PATH  or  XY<space>ORIG -> PATH
        entry = line[3:]
        if " -> " in entry:
            left, right = entry.split(" -> ", 1)
            paths.append(_unquote_git_path(left))
            paths.append(_unquote_git_path(right))
        else:
            paths.append(_unquote_git_path(entry))
    return paths


def git_trust_boundary_dirty(repo_root: Path) -> bool:
    """True when engine/certifier trust-boundary sources differ from HEAD.

    Unrelated working-tree edits (decomp ``src/**``, docs, harness churn, …)
    do **not** count as dirty for promotion / provenance canaries.
    """
    try:
        completed = subprocess.run(
            ["git", "status", "--porcelain"],
            capture_output=True,
            text=True,
            cwd=repo_root,
            check=False,
        )
    except OSError:
        return False
    if completed.returncode != 0:
        return False
    for relative in _iter_porcelain_paths(completed.stdout):
        if is_trust_boundary_relative_path(relative):
            return True
    return False


def live_git_identity(repo_root: Path) -> tuple[str, bool]:
    """Return ``(git_commit, trust_boundary_dirty)`` for provenance fields."""
    commit = ""
    try:
        completed = subprocess.run(
            ["git", "rev-parse", "HEAD"],
            capture_output=True,
            text=True,
            cwd=repo_root,
            check=False,
        )
        if completed.returncode == 0:
            commit = completed.stdout.strip()
    except OSError:
        pass
    return commit, git_trust_boundary_dirty(repo_root)


def canonical_json_sha256(value: object) -> str:
    payload = json.dumps(
        value, sort_keys=True, separators=(",", ":"), ensure_ascii=True, allow_nan=False,
    ).encode("utf-8")
    return hashlib.sha256(payload).hexdigest()


def canonical_obligation_dict(value: dict) -> dict:
    """Deep-canonicalize jump-table obligation payloads for stable identity."""
    result: dict = {}
    for key in sorted(value.keys()):
        item = value[key]
        if key == "targets" and isinstance(item, list):
            entries = [
                canonical_obligation_dict(entry)
                for entry in item
                if isinstance(entry, dict)
            ]
            result[key] = sorted(entries, key=lambda entry: str(entry.get("identity", "")))
        elif key == "artifact_hashes" and isinstance(item, list):
            result[key] = sorted(str(entry) for entry in item)
        elif isinstance(item, dict):
            result[key] = canonical_obligation_dict(item)
        elif isinstance(item, list):
            result[key] = list(item)
        else:
            result[key] = item
    return result


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
    platform_profile_sha256: str | None = None,
    floating_point_domain: object | None = None,
    assumed_callees: list[str] | None = None,
    callee_contract_sources: dict[str, str] | None = None,
    original_base: int | None = None,
    candidate_base: int | None = None,
    original_relocations: list | None = None,
    candidate_relocations: list | None = None,
    certificate_target_id: str | None = None,
    proof_features: list[str] | None = None,
    address_space: dict | None = None,
    indirect_targets: dict | None = None,
    loop_summary: dict | None = None,
    relational_induction: dict | None = None,
    memory_loop: dict | None = None,
    memory_bus: dict | None = None,
    memory_loop_readonly: dict | None = None,
    obligations: dict | None = None,
    capability_assurance: dict | None = None,
) -> dict:
    """Canonical proof-request fields hashed into ``ProofResult.source_hash``.

    Callers should pass every premise that can change the theorem; omit only
    fields that are not part of the request (leave them ``None`` so they are
    dropped rather than hashed as null).
    """
    from tools.ppc_equivalence.proof_features import PROOF_OBLIGATION_FIELDS

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
    # Stage 3A: reviewed platform-profile digest (bounded-memory promotion).
    # Field name must stay ``platform_profile_sha256`` for cache / cert identity.
    if platform_profile_sha256 is not None:
        payload["platform_profile_sha256"] = platform_profile_sha256
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
    if proof_features is not None:
        payload["proof_features"] = sorted(proof_features)
    # Identity-only readonly-image premise (not a proof feature); binds the
    # exact per-side words used to hydrate CTR lwz trip counts.
    if memory_loop_readonly is not None:
        payload["memory_loop_readonly"] = memory_loop_readonly

    merged: dict = dict(obligations or {})
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
    return payload


def proof_request_hash(**kwargs: object) -> str:
    """SHA-256 of :func:`proof_request_identity` kwargs."""
    return canonical_json_sha256(proof_request_identity(**kwargs))  # type: ignore[arg-type]

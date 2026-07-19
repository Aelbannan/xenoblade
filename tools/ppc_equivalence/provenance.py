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

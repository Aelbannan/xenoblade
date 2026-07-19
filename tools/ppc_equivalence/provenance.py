import hashlib
import json
from pathlib import Path


ENGINE_SOURCE_PATTERNS = [
    "tools/ppc_equivalence/*.py",
]


def hash_engine_tree(repo_root: Path) -> str:
    import glob
    paths = sorted(glob.glob(str(repo_root / "tools/ppc_equivalence" / "*.py")))
    lock = repo_root / "tools/ppc_equivalence" / "requirements.lock"
    if lock.exists():
        paths.append(str(lock))
    digest = hashlib.sha256()
    for path in paths:
        p = Path(path)
        relative = p.relative_to(repo_root).as_posix().encode("utf-8")
        content = p.read_bytes()
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

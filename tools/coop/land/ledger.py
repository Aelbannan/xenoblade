"""Content-hash ledger of gated files whose state passed the land gate.

The ledger is a set of ``(path, git-blob-sha1)`` pairs (JSON lines with the
task and time).  ``verify_staged`` is used by the pre-commit hook: every
staged gated file must carry a content hash that the gate has seen pass.
Blob hashes match ``git hash-object`` so the hook can compare staged blobs
without re-reading the working tree.
"""
from __future__ import annotations

import hashlib
import json
import os
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
STATE = ROOT / "tools" / "coop" / "land" / "state"
LEDGER = STATE / "ledger.jsonl"

GATED_DIRS = ("src", "include", "libs", "config")
GATED_FILES = ("configure.py",)
GATED_EXT = (".c", ".cc", ".cpp", ".h", ".hpp", ".inl", ".txt", ".yml", ".yaml", ".py", ".s", ".S")


def is_gated(rel: str) -> bool:
    rel = rel.replace(os.sep, "/")
    if rel.endswith(".ctx.c"):
        return False
    if rel in GATED_FILES:
        return True
    return rel.split("/", 1)[0] in GATED_DIRS and rel.endswith(GATED_EXT)


def blob_sha(data: bytes) -> str:
    return hashlib.sha1(b"blob %d\0" % len(data) + data).hexdigest()


def tree_hashes(root: Path = ROOT) -> dict[str, str]:
    """{repo-relative gated path: blob sha} for the current working tree."""
    out: dict[str, str] = {}
    for top in GATED_DIRS:
        for dp, dns, fns in os.walk(root / top):
            dns[:] = [d for d in dns if not d.startswith(".")]
            for fn in fns:
                rel = os.path.relpath(os.path.join(dp, fn), root).replace(os.sep, "/")
                if is_gated(rel):
                    out[rel] = blob_sha((root / rel).read_bytes())
    for rel in GATED_FILES:
        if (root / rel).is_file():
            out[rel] = blob_sha((root / rel).read_bytes())
    return out


def tree_id(hashes: dict[str, str]) -> str:
    h = hashlib.sha1()
    for k in sorted(hashes):
        h.update(f"{k}\0{hashes[k]}\n".encode())
    return h.hexdigest()


def load(ledger: Path = LEDGER) -> set[tuple[str, str]]:
    seen: set[tuple[str, str]] = set()
    if ledger.is_file():
        for line in ledger.read_text().splitlines():
            if line.strip():
                rec = json.loads(line)
                seen.add((rec["path"], rec["sha"]))
    return seen


def record(hashes: dict[str, str], task: str, kind: str = "gate", reason: str = "",
           ledger: Path = LEDGER) -> int:
    """Append every (path, sha) not yet in the ledger; returns the count added."""
    ledger.parent.mkdir(parents=True, exist_ok=True)
    seen = load(ledger)
    now = time.strftime("%Y-%m-%dT%H:%M:%S")
    new = [(p, s) for p, s in sorted(hashes.items()) if (p, s) not in seen]
    with open(ledger, "a") as fh:
        for p, s in new:
            fh.write(json.dumps({"path": p, "sha": s, "task": task, "kind": kind,
                                 "reason": reason, "time": now}) + "\n")
    return len(new)


def verify(pairs: list[tuple[str, str | None]], ledger: Path = LEDGER) -> list[str]:
    """Paths whose (path, sha) never passed the gate.  sha None = deletion (allowed)."""
    seen = load(ledger)
    return [p for p, s in pairs if is_gated(p) and s is not None and (p, s) not in seen]


def unledgered(hashes: dict[str, str] | None = None, ledger: Path = LEDGER) -> list[str]:
    """Gated tree files whose current content never passed the gate."""
    hashes = tree_hashes() if hashes is None else hashes
    return verify(sorted(hashes.items()), ledger=ledger)

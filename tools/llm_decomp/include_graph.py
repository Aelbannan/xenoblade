"""Bidirectional include graph for the llm_decomp harness.

Builds a cached, repo-relative include graph by scanning ``#include`` directives
in source and header files under ``src/``, ``libs/``, and ``include/``.

Design reference: docs/llm_decomp_design.md §7 "Include graph".
"""
from __future__ import annotations

import hashlib
import json
import os
import re
from pathlib import Path

from tools.llm_decomp.paths import include_graph_path

# Regex matching both quoted and angle-bracket includes on a line.
# Prefixes that indicate a resolved repo-relative path (as opposed to an
# unresolvable SDK/system include).
_RESOLVED_PREFIXES = ("include/", "src/", "libs/")
_INCLUDE_RE = re.compile(
    r'^\s*#\s*include\s*([<"])([^>"]+)[>"]',
    re.MULTILINE,
)

# File extensions we consider scannable.
_SCANNABLE_EXTENSIONS = frozenset({".c", ".cpp", ".h", ".hpp", ".hxx", ".hh"})


class IncludeGraph:
    """Bidirectional include graph for a repo snapshot.

    Parameters
    ----------
    edges : dict[str, list[str]]
        ``edges[including_file]`` = list of resolved-or-unresolved include targets
        (repo-relative forward-slash paths, or the original include text when
        unresolvable).
    files : dict[str, float] | None
        ``files[path]`` = ``st_mtime`` for cache staleness checking.
    """

    def __init__(
        self,
        edges: dict[str, list[str]],
        files: dict[str, float] | None = None,
    ) -> None:
        self._edges = edges
        # Reverse index: resolved path -> set of paths that directly include it.
        # Unresolvable includes (SDK/system) are recorded in edges but excluded
        # from the reverse index so dependents() naturally skips them.
        self._reverse: dict[str, set[str]] = {}
        for src, targets in edges.items():
            for tgt in targets:
                if tgt.startswith(_RESOLVED_PREFIXES):
                    self._reverse.setdefault(tgt, set()).add(src)
        self._files = files if files is not None else {}

    # ---- Public API --------------------------------------------------------

    @classmethod
    def load_or_build(cls, repo_root: Path) -> IncludeGraph:
        """Load from cache if fresh, otherwise build from scratch.

        Fast path: stat only the files listed in the cache plus walk roots to
        detect new files.  Any mismatch or new file triggers a full rebuild.
        """
        cache_path = include_graph_path(repo_root)
        if cache_path.is_file():
            try:
                data = json.loads(cache_path.read_text(encoding="utf-8"))
            except (json.JSONDecodeError, OSError):
                data = {}

            cached_files: dict[str, float] = data.get("files", {})
            cached_edges: dict[str, list[str]] = data.get("edges", {})

            if cached_files and _cache_is_fresh(cached_files, repo_root):
                return cls(cached_edges, cached_files)

        # Full rebuild.
        graph = scan(repo_root)

        # Persist to disk (atomic write).
        cache_path.parent.mkdir(parents=True, exist_ok=True)
        tmp = cache_path.with_suffix(f".tmp_{os.urandom(4).hex()}")
        data = {
            "files": graph._files,
            "edges": {k: v for k, v in graph._edges.items()},
        }
        tmp.write_text(json.dumps(data, separators=(",", ":")), encoding="utf-8")
        os.replace(tmp, cache_path)
        return graph

    def direct_dependents(self, header: str) -> set[str]:
        """Files (any extension) that directly include *header*."""
        return set(self._reverse.get(header, set()))

    def dependents(self, header: str) -> set[str]:
        """Transitive dependents: repo-relative ``.c`` / ``.cpp`` files.

        Walks the reverse-edge graph and returns every source file that
        transitively depends on *header*.
        """
        visited: set[str] = set()
        stack = [header]
        while stack:
            node = stack.pop()
            if node in visited:
                continue
            visited.add(node)
            for dep in self._reverse.get(node, set()):
                if dep not in visited:
                    stack.append(dep)
        return {p for p in visited if p.endswith((".c", ".cpp"))}

    def snapshot_id(self) -> str:
        """Content hash that changes when the edge set changes (hex[:16]).

        Stable across loads — computed from the sorted edge list.
        """
        pairs: list[str] = []
        for src in sorted(self._edges):
            for tgt in sorted(self._edges[src]):
                pairs.append(f"{src}->{tgt}")
        raw = "\n".join(pairs)
        return hashlib.sha256(raw.encode()).hexdigest()[:16]


# ---- Scan / resolve -------------------------------------------------------


def scan(repo_root: Path) -> IncludeGraph:
    """Full rebuild of the include graph.

    Scans ``src/``, ``libs/``, ``include/`` (missing dirs silently skipped)
    and returns a new ``IncludeGraph`` with all edges and file mtimes recorded.
    """
    edges: dict[str, list[str]] = {}
    files: dict[str, float] = {}

    roots = [
        repo_root / "src",
        repo_root / "libs",
        repo_root / "include",
    ]

    # Collect all scannable files in sorted order for deterministic output.
    scannable: list[Path] = []
    for root in roots:
        if not root.is_dir():
            continue
        for dirpath, _dirnames, filenames in os.walk(root):
            for fn in filenames:
                if _is_scannable(fn):
                    scannable.append(Path(dirpath) / fn)

    scannable.sort()

    for fpath in scannable:
        rel = os.path.relpath(fpath, repo_root).replace(os.sep, "/")
        try:
            st = fpath.stat()
        except OSError:
            continue
        files[rel] = st.st_mtime

        includes = _resolve_includes(fpath, rel, repo_root)
        if includes:
            edges[rel] = includes

    return IncludeGraph(edges, files)


# ---- Internal helpers -----------------------------------------------------


def _is_scannable(filename: str) -> bool:
    """True if *filename* has a recognised source/header extension."""
    _, ext = os.path.splitext(filename)
    return ext.lower() in _SCANNABLE_EXTENSIONS


def _cache_is_fresh(
    cached_files: dict[str, float], repo_root: Path
) -> bool:
    """Check cached mtimes and detect new files not in the cache.

    Returns ``True`` when every cached file still has the same mtime *and*
    re-walking the roots finds no file absent from the cache.
    """
    # 1. Verify mtimes of all cached paths.
    for path_str, cached_mtime in cached_files.items():
        full = repo_root / path_str
        try:
            if full.stat().st_mtime != cached_mtime:
                return False
        except OSError:
            return False  # deleted or inaccessible

    # 2. Walk roots to detect brand-new files.
    roots = [repo_root / d for d in ("src", "libs", "include")]
    for root in roots:
        if not root.is_dir():
            continue
        for dirpath, _dirnames, filenames in os.walk(root):
            for fn in filenames:
                if not _is_scannable(fn):
                    continue
                rel = os.path.relpath(
                    os.path.join(dirpath, fn), repo_root
                ).replace(os.sep, "/")
                if rel not in cached_files:
                    return False
    return True


def _resolve_includes(
    fpath: Path, source_rel: str, repo_root: Path
) -> list[str]:
    """Parse ``#include`` lines in *fpath* and resolve each to a repo-relative
    path.

    Unresolvable includes (SDK / system headers) are **recorded** with their
    original include text — they will not appear in the reverse index and
    therefore are excluded from ``dependents()``.
    """
    try:
        text = fpath.read_text(encoding="utf-8", errors="replace")
    except (OSError, UnicodeDecodeError):
        return []

    includes: list[str] = []
    for m in _INCLUDE_RE.finditer(text):
        inc = m.group(2).strip()
        resolved = _resolve_one(inc, source_rel, repo_root)
        includes.append(resolved if resolved is not None else inc)
    return includes


def _resolve_one(
    inc_path: str, source_rel: str, repo_root: Path
) -> str | None:
    """Try to resolve one include to a repo-relative path.

    Resolution order (first match wins):
      1. ``include/<inc_path>``
      2. relative to the including file's directory
      3. ``libs/<inc_path>``

    Returns ``None`` when unresolvable.
    """
    normalised = Path(inc_path).as_posix()

    # Strategy 1: include/<path>
    candidate = repo_root / "include" / normalised
    if candidate.is_file():
        return os.path.relpath(candidate, repo_root).replace(os.sep, "/")

    # Strategy 2: relative to the including file's directory
    source_dir = Path(source_rel).parent
    candidate = repo_root / source_dir / normalised
    if candidate.is_file():
        return os.path.relpath(candidate, repo_root).replace(os.sep, "/")

    # Strategy 3: libs/<path>
    candidate = repo_root / "libs" / normalised
    if candidate.is_file():
        return os.path.relpath(candidate, repo_root).replace(os.sep, "/")

    return None
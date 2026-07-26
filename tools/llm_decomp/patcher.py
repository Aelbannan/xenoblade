"""SEARCH/REPLACE patch engine (all-or-nothing).

Design reference: docs/llm_decomp_design.md §4.3.

Semantics:
- Every file must be inside the session's writable set.
- All work happens in memory; files are written only when every block in
  every file succeeds. On any failure the disk is untouched.
- A block's `search` must occur exactly once in the current content.
- Blocks apply in order, each against the result of the previous.
"""
from __future__ import annotations

import os
from dataclasses import dataclass, field
from pathlib import Path


@dataclass
class SearchReplace:
    search: str
    replace: str


@dataclass
class FilePatch:
    path: str  # repo-relative, forward slashes
    blocks: list[SearchReplace] = field(default_factory=list)
    create: bool = False  # True -> full-file creation via content
    content: str | None = None


@dataclass
class PatchOutcome:
    ok: bool
    changed_ranges: dict[str, list[tuple[int, int]]] = field(default_factory=dict)
    error: str | None = None  # markdown-readable failure message
    failing: tuple[str, int] | None = None  # (path, block_index)


def _normalize(path: str) -> str | None:
    """Normalize a repo-relative path; None if unsafe."""
    path = path.replace("\\", "/").lstrip("./")
    if not path or os.path.isabs(path):
        return None
    if ".." in path.split("/"):
        return None
    return path


def _anchor_hint(search: str) -> str:
    first = search.strip().splitlines()[0] if search.strip() else "(empty)"
    return first[:80]


def _fail(path: str, index: int, message: str) -> PatchOutcome:
    return PatchOutcome(ok=False, error=message, failing=(path, index))


def _block_line_span(new_content: str, replaced_text: str,
                     search_start: int) -> tuple[int, int]:
    """1-indexed line span of the applied block in the new content."""
    start_line = new_content.count("\n", 0, search_start) + 1
    end_line = start_line + replaced_text.count("\n")
    return start_line, end_line


def apply_patch(repo_root: Path, files: list[FilePatch],
                writable: set[str]) -> PatchOutcome:
    if not files:
        return PatchOutcome(ok=False, error="patch: no files given.",
                            failing=None)

    staged: dict[str, str] = {}  # normalized path -> new content
    changed: dict[str, list[tuple[int, int]]] = {}

    for fp in files:
        norm = _normalize(fp.path)
        if norm is None:
            return _fail(fp.path, -1,
                         f"patch: unsafe path `{fp.path}` "
                         "(absolute or contains `..`).")
        if norm not in writable:
            return _fail(norm, -1,
                         f"patch: `{norm}` is outside this session's "
                         "writable scope.")

        if fp.create:
            if fp.blocks:
                return _fail(norm, -1,
                             f"patch: `{norm}` uses create=true with "
                             "blocks; create takes full content only.")
            if fp.content is None:
                return _fail(norm, -1,
                             f"patch: create=true for `{norm}` but no "
                             "content given.")
            if (repo_root / norm).exists() or norm in staged:
                return _fail(norm, -1,
                             f"patch: create=true but `{norm}` already "
                             "exists.")
            staged[norm] = fp.content
            total = max(1, len(fp.content.splitlines()))
            changed.setdefault(norm, []).append((1, total))
            continue

        src = repo_root / norm
        if norm in staged:
            content = staged[norm]
        elif src.is_file():
            content = src.read_text(encoding="utf-8")
        else:
            return _fail(norm, -1, f"patch: `{norm}` does not exist "
                                   "(use create=true for new files).")
        if not fp.blocks:
            return _fail(norm, -1,
                         f"patch: no blocks given for `{norm}`.")

        for i, block in enumerate(fp.blocks):
            occurrences = content.count(block.search)
            if occurrences != 1:
                hint = _anchor_hint(block.search)
                return _fail(
                    norm, i,
                    f"patch: block {i} in `{norm}` matches "
                    f"{occurrences} times (must be exactly 1). "
                    f"Nearest anchor: `{hint}`")
            start = content.index(block.search)
            span = _block_line_span(content, block.replace, start)
            content = content[:start] + block.replace + \
                content[start + len(block.search):]
            changed.setdefault(norm, []).append(span)
        staged[norm] = content

    # Merge overlapping/adjacent ranges per file.
    merged: dict[str, list[tuple[int, int]]] = {}
    for path, spans in changed.items():
        spans.sort()
        out: list[list[int]] = []
        for s, e in spans:
            if out and s <= out[-1][1] + 1:
                out[-1][1] = max(out[-1][1], e)
            else:
                out.append([s, e])
        merged[path] = [(s, e) for s, e in out]

    # All validation passed — write to disk.
    for norm, content in staged.items():
        dest = repo_root / norm
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_text(content, encoding="utf-8")

    return PatchOutcome(ok=True, changed_ranges=merged)

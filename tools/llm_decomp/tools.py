"""Tool JSON schemas and dispatch for the llm_decomp session loop.

Every tool is defined as an OpenAI-format function-calling JSON schema in
``TOOL_SCHEMAS``. The ``dispatch()`` function maps a ``ToolCall`` to an
implementation based on ``ToolContext``.

Design reference: docs/llm_decomp_design.md §4.
"""
from __future__ import annotations

import os
import re
import shutil
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable

from tools.llm_decomp.contracts import ToolCall, ToolResult

# Repository roots that read_file and grep may access.
_ALLOWED_ROOTS = ("src/", "libs/", "include/")

# ── Tool schemas ────────────────────────────────────────────────────────

TOOL_SCHEMAS: list[dict] = [
    {
        "type": "function",
        "function": {
            "name": "read_file",
            "description": "Read lines from a file under src/, libs/, or include/. "
                           "Returns numbered lines (1-indexed) with a total line count header. "
                           "Paths must be repo-relative and stay within the allowed roots.",
            "parameters": {
                "type": "object",
                "properties": {
                    "path": {
                        "type": "string",
                        "description": "Repo-relative file path, e.g. src/kyoshin/cf/CfPadTask.cpp",
                    },
                    "start": {
                        "type": "integer",
                        "minimum": 1,
                        "description": "1-indexed start line number (default: 1)",
                    },
                    "count": {
                        "type": "integer",
                        "minimum": 1,
                        "maximum": 400,
                        "description": "Number of lines to read (1-400, default: 200)",
                    },
                },
                "required": ["path"],
            },
        },
    },
    {
        "type": "function",
        "function": {
            "name": "grep",
            "description": "Search for a pattern across allowed roots (src/, libs/, include/) "
                           "using ripgrep. Falls back to a pure-Python walk if rg is unavailable. "
                           "Results are capped at 50 matches with a truncation marker.",
            "parameters": {
                "type": "object",
                "properties": {
                    "pattern": {
                        "type": "string",
                        "description": "Regex pattern to search for",
                    },
                    "path": {
                        "type": "string",
                        "description": "Subdirectory under which to search (default: '.')",
                    },
                    "glob": {
                        "type": "string",
                        "description": "Optional file glob filter, e.g. '*.hpp'",
                    },
                },
                "required": ["pattern"],
            },
        },
    },
    {
        "type": "function",
        "function": {
            "name": "patch",
            "description": "Apply SEARCH/REPLACE blocks to one or more files. Each SEARCH must "
                           "match exactly once in the current file state; all-or-nothing across "
                           "all files. Use 'create' for new files (requires 'content', no blocks). "
                           "Use 'content' without blocks to overwrite an existing file entirely.",
            "parameters": {
                "type": "object",
                "properties": {
                    "files": {
                        "type": "array",
                        "items": {
                            "type": "object",
                            "properties": {
                                "path": {"type": "string"},
                                "create": {"type": "boolean", "default": False},
                                "content": {"type": "string"},
                                "blocks": {
                                    "type": "array",
                                    "items": {
                                        "type": "object",
                                        "properties": {
                                            "search": {"type": "string"},
                                            "replace": {"type": "string"},
                                        },
                                        "required": ["search", "replace"],
                                    },
                                },
                            },
                            "required": ["path"],
                        },
                    },
                },
                "required": ["files"],
            },
        },
    },
    {
        "type": "function",
        "function": {
            "name": "build",
            "description": "Build a translation unit via ninja. Acquires the global build lock. "
                           "Returns cleaned compile diagnostics. Only the owning session's build "
                           "results are cached for subsequent diff/equivalence calls.",
            "parameters": {
                "type": "object",
                "properties": {
                    "unit": {
                        "type": "string",
                        "description": "Unit path relative to src/, e.g. kyoshin/cf/CfPadTask",
                    },
                },
                "required": ["unit"],
            },
        },
    },
    {
        "type": "function",
        "function": {
            "name": "diff",
            "description": "Hexdiff one symbol ('target' mode) or sweep all defined symbols "
                           "against the INIT baseline ('sweep' mode). Assumes a successful build "
                           "by this session already exists. Sweep is rate-limited to 5 per session "
                           "for agent-initiated calls.",
            "parameters": {
                "type": "object",
                "properties": {
                    "unit": {
                        "type": "string",
                        "description": "Unit path relative to src/",
                    },
                    "symbol": {
                        "type": "string",
                        "description": "Mangled symbol name (required for target mode)",
                    },
                    "mode": {
                        "type": "string",
                        "enum": ["target", "sweep"],
                        "default": "target",
                    },
                },
                "required": ["unit"],
            },
        },
    },
    {
        "type": "function",
        "function": {
            "name": "equivalence",
            "description": "Run ppc_equivalence SMT check on a fuzzy-[50,100) symbol. "
                           "Returns EQUIVALENT or NOT_EQUIVALENT verdict. "
                           "Used when the target is structurally close but not byte-identical.",
            "parameters": {
                "type": "object",
                "properties": {
                    "unit": {
                        "type": "string",
                        "description": "Unit path relative to src/",
                    },
                    "symbol": {
                        "type": "string",
                        "description": "Mangled symbol name to check",
                    },
                },
                "required": ["unit", "symbol"],
            },
        },
    },
    {
        "type": "function",
        "function": {
            "name": "submit",
            "description": "Submit the current attempt for verification. "
                           "Ends the matching phase and triggers full verification "
                           "(build, sweep, lint, size check). The optional note is "
                           "appended to the transcript for the orchestrator.",
            "parameters": {
                "type": "object",
                "properties": {
                    "note": {
                        "type": "string",
                        "description": "Optional note attached to the submission (e.g. "
                                       "observations about sibling functions or blocked status)",
                    },
                },
            },
        },
    },
]

# ── Dispatch ────────────────────────────────────────────────────────────

@dataclass
class ToolContext:
    repo_root: Path
    writable: set[str] = field(default_factory=set)  # repo-relative paths the session may write
    patch_fn: Callable | None = None  # (files: list[dict]) -> ToolResult
    build_fn: Callable | None = None  # (unit: str) -> ToolResult
    diff_fn: Callable | None = None  # (unit, symbol|None, mode) -> ToolResult
    equivalence_fn: Callable | None = None  # (unit, symbol) -> ToolResult
    symbols_fn: Callable | None = None  # (args: dict) -> ToolResult
    rename_symbol_fn: Callable | None = None  # (args: dict) -> ToolResult


def _resolve_allowed(repo_root: Path, rel: str) -> Path | None:
    """Resolve a repo-relative path, enforcing the read allowlist."""
    if os.path.isabs(rel) or ".." in Path(rel).parts:
        return None
    resolved = (repo_root / rel).resolve()
    try:
        resolved.relative_to(repo_root.resolve())
    except ValueError:
        return None
    normalized = rel.replace(os.sep, "/")
    if not normalized.startswith(_ALLOWED_ROOTS):
        return None
    return resolved


def _read_file(repo_root: Path, args: dict) -> ToolResult:
    rel = str(args.get("path", ""))
    start = max(1, int(args.get("start", 1)))
    count = min(400, max(1, int(args.get("count", 200))))
    resolved = _resolve_allowed(repo_root, rel)
    if resolved is None:
        return ToolResult(ok=False, content=(
            f"**read_file:** `{rel}` is not allowed — paths must stay under "
            "src/, libs/, or include/ and must not contain `..`."))
    if not resolved.is_file():
        return ToolResult(ok=False,
                          content=f"**read_file:** `{rel}` does not exist.")
    lines = resolved.read_text(encoding="utf-8", errors="replace").splitlines()
    if lines and start > len(lines):
        return ToolResult(ok=False, content=(
            f"**read_file:** start line {start} is beyond the end of "
            f"`{rel}` ({len(lines)} lines total)."))
    window = lines[start - 1:start - 1 + count]
    body = "\n".join(f"{start + i:04d}| {line}" for i, line in enumerate(window))
    header = f"{rel} (showing {start}-{start + len(window) - 1} of {len(lines)} lines total)"
    return ToolResult(ok=True, content=f"{header}\n{body}",
                      data={"path": rel, "total_lines": len(lines)})


def _grep(repo_root: Path, args: dict) -> ToolResult:
    pattern = str(args.get("pattern", ""))
    if not pattern:
        return ToolResult(ok=False, content="**grep:** `pattern` is required.")
    rel = str(args.get("path", ".")) or "."
    glob = args.get("glob") or None
    if rel == ".":
        base = repo_root
    else:
        resolved = _resolve_allowed(repo_root, rel)
        if resolved is None:
            return ToolResult(ok=False, content=(
                f"**grep:** `{rel}` is not allowed — paths must stay under "
                "src/, libs/, or include/ and must not contain `..`."))
        base = resolved
    if shutil.which("rg"):
        cmd = ["rg", "--line-number", "--max-count=50", "--no-heading",
               pattern, str(base)]
        if glob:
            cmd += ["--glob", glob]
        proc = subprocess.run(cmd, cwd=repo_root, capture_output=True,
                              text=True, timeout=60)
        out = proc.stdout.strip()
        if len(out.splitlines()) >= 50:
            out += "\n... (truncated at 50 matches)"
        return ToolResult(ok=True, content=out or "(no matches)",
                          data={"matches": len(proc.stdout.strip().splitlines())})
    return _grep_python(repo_root, pattern, base, glob)


def _grep_python(repo_root: Path, pattern: str, base: Path,
                 glob: str | None) -> ToolResult:
    import fnmatch
    rx = re.compile(pattern)
    matches: list[str] = []
    truncated = False
    for dirpath, _dirs, files in os.walk(base):
        for name in files:
            if glob and not fnmatch.fnmatch(name, glob):
                continue
            fpath = Path(dirpath) / name
            try:
                with open(fpath, encoding="utf-8", errors="replace") as fh:
                    for lineno, line in enumerate(fh, 1):
                        if rx.search(line):
                            rel = fpath.relative_to(repo_root)
                            matches.append(f"{rel}:{lineno}:{line.rstrip()}")
                            if len(matches) >= 50:
                                truncated = True
                                break
            except (OSError, UnicodeError):
                continue
            if truncated:
                break
        if truncated:
            break
    content = "\n".join(matches) or "(no matches)"
    if truncated:
        content += "\n... (truncated at 50 matches)"
    return ToolResult(ok=True, content=content,
                      data={"matches": len(matches), "truncated": truncated})


def _delegate(fn: Callable | None, *args) -> ToolResult:
    if fn is None:
        return ToolResult(ok=False,
                          content="Tool unavailable in this session type.")
    return fn(*args)


def dispatch(call: ToolCall, ctx: ToolContext) -> ToolResult:
    name = call.name
    args = call.args or {}
    if name == "read_file":
        return _read_file(ctx.repo_root, args)
    if name == "grep":
        return _grep(ctx.repo_root, args)
    if name == "patch":
        return _delegate(ctx.patch_fn, args.get("files", []))
    if name == "build":
        return _delegate(ctx.build_fn, str(args.get("unit", "")))
    if name == "diff":
        return _delegate(ctx.diff_fn, str(args.get("unit", "")),
                         args.get("symbol"), str(args.get("mode", "target")))
    if name == "equivalence":
        return _delegate(ctx.equivalence_fn, str(args.get("unit", "")),
                         str(args.get("symbol", "")))
    if name == "symbols":
        return _delegate(ctx.symbols_fn, args)
    if name == "rename_symbol":
        return _delegate(ctx.rename_symbol_fn, args)
    if name == "submit":
        return ToolResult(ok=True, content="Submit received.",
                          data={"submitted": True,
                                "note": str(args.get("note", ""))})
    return ToolResult(ok=False, content=f"Unknown tool: `{name}`.")

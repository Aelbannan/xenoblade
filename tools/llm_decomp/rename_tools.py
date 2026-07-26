"""Rename session tools — symrecover, harness-mediated (design §10).

The agent never gets a shell; these wrap tools/symrecover.py subcommands.
rename_symbol validates, snapshots, applies, rebuilds, and reverts on
failure.
"""
from __future__ import annotations

import re
import shutil
import subprocess
import sys
from pathlib import Path

from .contracts import ToolResult

_IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")

RENAME_TOOL_SCHEMAS: list[dict] = [
    {
        "type": "function",
        "function": {
            "name": "symbols",
            "description": "Read-only symbol recovery info: show symbols for "
                           "a placeholder type, cross-reference its splits/"
                           "source, or demangle MWCC symbol names.",
            "parameters": {
                "type": "object",
                "properties": {
                    "command": {"type": "string",
                                "enum": ["show", "xref", "demangle"]},
                    "query": {"type": "string",
                              "description": "Type name/address or symbol."},
                },
                "required": ["command", "query"],
            },
        },
    },
    {
        "type": "function",
        "function": {
            "name": "rename_symbol",
            "description": "Rename a placeholder type repo-wide (symbols.txt "
                           "+ source + configure + splits). Validated first; "
                           "reverted automatically if the build breaks.",
            "parameters": {
                "type": "object",
                "properties": {
                    "old": {"type": "string"},
                    "new": {"type": "string"},
                    "force": {"type": "boolean",
                              "description": "Allow mangling-length mismatch."},
                },
                "required": ["old", "new"],
            },
        },
    },
]


def _run(repo_root: Path, cmd: list[str], timeout: int = 120
         ) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, cwd=repo_root, capture_output=True, text=True,
                          timeout=timeout)


def _tail(text: str, limit: int = 4000) -> str:
    return text if len(text) <= limit else text[:limit] + "\n... (truncated)"


def symbols_tool(repo_root: Path, args: dict) -> ToolResult:
    command = str(args.get("command", ""))
    query = str(args.get("query", ""))
    if command not in ("show", "xref", "demangle"):
        return ToolResult(ok=False,
                          content=f"**symbols:** unknown command `{command}` "
                                  "(show|xref|demangle).")
    if not query:
        return ToolResult(ok=False, content="**symbols:** query required.")
    try:
        proc = _run(repo_root,
                    [sys.executable, "tools/symrecover.py", command, query])
    except subprocess.TimeoutExpired:
        return ToolResult(ok=False, content="**symbols:** timed out.")
    if proc.returncode != 0:
        return ToolResult(ok=False,
                          content="**symbols failed:**\n```\n"
                                  + _tail(proc.stderr or proc.stdout)[-2000:]
                                  + "\n```")
    return ToolResult(ok=True, content=_tail(proc.stdout),
                      data={"command": command, "query": query})


def _porcelain(repo_root: Path) -> set[str]:
    proc = _run(repo_root, ["git", "status", "--porcelain"])
    out = set()
    for line in proc.stdout.splitlines():
        if len(line) > 3:
            out.add(line[3:].strip())
    return out


def _revert(repo_root: Path, affected: set[str]) -> list[str]:
    reverted: list[str] = []
    tracked: list[str] = []
    for rel in sorted(affected):
        if (repo_root / rel).exists():
            check = _run(repo_root, ["git", "ls-files", "--error-unmatch",
                                     rel])
            if check.returncode == 0:
                tracked.append(rel)
            else:
                (repo_root / rel).unlink()
                reverted.append(rel)
        else:
            tracked.append(rel)  # deleted by rename; restore from git
    if tracked:
        _run(repo_root, ["git", "checkout", "--", *tracked])
        reverted.extend(tracked)
    return reverted


def rename_symbol_tool(repo_root: Path, args: dict) -> ToolResult:
    old = str(args.get("old", ""))
    new = str(args.get("new", ""))
    force = bool(args.get("force", False))
    if not old or not _IDENTIFIER.match(new):
        return ToolResult(
            ok=False,
            content=f"**rename_symbol:** invalid identifier `{new}`.")

    steps = [[sys.executable, "tools/symrecover.py", "rename-plan",
              old, new, "--verbose"]]
    try:
        plan = _run(repo_root, steps[0])
    except subprocess.TimeoutExpired:
        return ToolResult(ok=False, content="**rename-plan timed out.**")
    if plan.returncode != 0:
        return ToolResult(ok=False,
                          content="**rename-plan rejected:**\n```\n"
                                  + _tail(plan.stdout + plan.stderr)[-2000:]
                                  + "\n```")

    baseline_dirt = _porcelain(repo_root)
    apply_cmd = [sys.executable, "tools/symrecover.py", "rename-all",
                 old, new] + (["--force"] if force else [])
    try:
        applied = _run(repo_root, apply_cmd, timeout=300)
    except subprocess.TimeoutExpired:
        return ToolResult(ok=False, content="**rename-all timed out.**")
    if applied.returncode != 0:
        return ToolResult(ok=False,
                          content="**rename-all failed:**\n```\n"
                                  + _tail(applied.stdout
                                          + applied.stderr)[-2000:]
                                  + "\n```")

    affected = _porcelain(repo_root) - baseline_dirt

    ninja = shutil.which("ninja") or "ninja"
    try:
        build = _run(repo_root, [ninja], timeout=900)
    except subprocess.TimeoutExpired:
        build = None
    if build is None or build.returncode != 0:
        reverted = _revert(repo_root, affected)
        tail = "(ninja timed out)" if build is None else \
            _tail(build.stdout + build.stderr)[-2000:]
        return ToolResult(
            ok=False,
            content=("**rename reverted — build broke.**\n```\n" + tail
                     + "\n```\nReverted files: "
                     + ", ".join(reverted)),
            data={"reverted": reverted})

    listing = "\n".join(f"- `{f}`" for f in sorted(affected))
    return ToolResult(
        ok=True,
        content=(f"**renamed `{old}` → `{new}`.** Affected files:\n"
                 + (listing or "(none detected)")),
        data={"affected": sorted(affected)})

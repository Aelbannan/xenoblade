"""Session-type policy registry for the LLM decompilation harness.

Design reference: docs/llm_decomp_design.md §10.
"""
from __future__ import annotations

from dataclasses import dataclass
from typing import NoReturn

from tools.llm_decomp.config import DEFAULT_BUDGETS


@dataclass(frozen=True)
class WritableScope:
    """Defines which files a session type is allowed to modify.

    Only one of *header_only* / *rename_only* / *whole_tu* should be
    *True* at a time under normal use; the dataclass itself does not
    enforce this.
    """

    owning_tu: bool = True
    designated_header: bool = True
    whole_tu: bool = False
    shared_headers: bool = False  # v1.1: may write headers beyond the TU
    header_only: bool = False
    rename_only: bool = False


@dataclass(frozen=True)
class SessionPolicy:
    """Everything the session loop needs to know about a session type."""

    name: str
    scope: WritableScope
    tools: tuple[str, ...]
    gate: str
    prompt_template: str
    budgets: dict


# ---------------------------------------------------------------------------
# Registry
# ---------------------------------------------------------------------------

POLICIES: dict[str, SessionPolicy] = {
    "match": SessionPolicy(
        name="match",
        scope=WritableScope(owning_tu=True, designated_header=True,
                             shared_headers=True),
        tools=("read_file", "patch", "build", "diff", "equivalence", "submit"),
        gate="match",
        prompt_template="prompts/match.md",
        budgets=dict(DEFAULT_BUDGETS["match"]),
    ),
    "type-recovery": SessionPolicy(
        name="type-recovery",
        scope=WritableScope(header_only=True, shared_headers=True),
        tools=("read_file", "patch", "build", "diff", "equivalence", "submit"),
        gate="type_recovery",
        prompt_template="prompts/type-recovery.md",
        budgets=dict(DEFAULT_BUDGETS["type-recovery"]),
    ),
    "rename": SessionPolicy(
        name="rename",
        scope=WritableScope(rename_only=True),
        tools=("read_file", "symbols", "rename_symbol", "submit"),
        gate="rename",
        prompt_template="prompts/rename.md",
        budgets=dict(DEFAULT_BUDGETS["rename"]),
    ),
    "tu-cleanup": SessionPolicy(
        name="tu-cleanup",
        scope=WritableScope(whole_tu=True, shared_headers=True),
        tools=("read_file", "patch", "build", "diff", "submit"),
        gate="byte_identical",
        prompt_template="prompts/tu-cleanup.md",
        budgets=dict(DEFAULT_BUDGETS["tu-cleanup"]),
    ),
    "size-trim": SessionPolicy(
        name="size-trim",
        scope=WritableScope(whole_tu=True, shared_headers=True),
        tools=("read_file", "patch", "build", "diff", "equivalence", "submit"),
        gate="size_trim",
        prompt_template="prompts/size-trim.md",
        budgets=dict(DEFAULT_BUDGETS["size-trim"]),
    ),
}


# ---------------------------------------------------------------------------
# Lookup helpers
# ---------------------------------------------------------------------------


def get_policy(name: str) -> SessionPolicy:
    """Return the :class:`SessionPolicy` registered under *name*.

    Raises ``ValueError`` if *name* is unknown.
    """
    try:
        return POLICIES[name]
    except KeyError:
        raise ValueError(f"Unknown session type: {name!r}. Known: {list(POLICIES)}") from None


def _raise_unresolvable(policy: SessionPolicy) -> NoReturn:
    raise ValueError(
        f"Cannot resolve writable scope for policy {policy.name!r}: "
        f"header_only=True but no unit_header provided"
    )


def resolve_writable(
    policy: SessionPolicy, unit_src: str, unit_header: str | None
) -> list[str]:
    """Return the list of repo-relative writable paths for *policy*.

    Parameters
    ----------
    policy
        A registered session policy.
    unit_src
        Repo-relative path to the TU's source file (e.g. ``src/kyoshin/.../Unit.cpp``).
    unit_header
        Repo-relative path to the TU's designated header, or *None*.

    Returns
    -------
    Ordered list of writable paths (may be empty).

    Raises
    ------
    ValueError
        If the policy's *WritableScope* cannot be satisfied with the
        provided arguments (e.g. *header_only* but *unit_header* is *None*).
    """
    scope = policy.scope
    paths: list[str] = []

    if scope.rename_only:
        # rename-only sessions use the rename_symbol tool, no raw file writes.
        return []

    if scope.header_only:
        if unit_header is None:
            _raise_unresolvable(policy)
        return [unit_header]

    if scope.owning_tu or scope.whole_tu:
        paths.append(unit_src)

    if scope.designated_header or scope.whole_tu:
        if unit_header is not None:
            paths.append(unit_header)

    return paths
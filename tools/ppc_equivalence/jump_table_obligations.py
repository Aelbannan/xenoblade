"""Jump-table proof obligations: readonly image binding and target closure.

Pattern recognition (``jump_table.find_jump_table_candidates``) is not a proof.
These helpers build the obligation payloads and SMT constraints required before
``readonly-image`` / ``indirect-target-closure`` may authorize ``EQUIVALENT``.
Until the engine discharges them end-to-end, both features remain in
``UNSUPPORTED_FOR_EQUIVALENT``.
"""

from __future__ import annotations

import hashlib
from dataclasses import dataclass
from typing import TYPE_CHECKING, Any, Sequence

from tools.ppc_equivalence.address_space import Region, RegionKind, rom_image_region
from tools.ppc_equivalence.ir import Instruction
from tools.ppc_equivalence.jump_table import JumpTableCandidate, find_jump_table_candidates

if TYPE_CHECKING:
    from tools.ppc_equivalence.jump_table_pairing import JumpTablePairing


@dataclass(frozen=True)
class JumpTableProofContext:
    """Premises for a jump-table EQUIVALENT attempt.

    ``branch_pc`` is the ``bctr`` address. Optional ``candidate_branch_pc``
    covers differently-based candidate images. ``table_base_reg`` / ``index_reg``
    are constrained as proof premises (base VA and unsigned index ``< len(words)``).

    When ``candidate_table`` is set, CFG expansion and obligations use per-side
    target lists and ROM images. ``pairing`` binds logical case identities when
    retail and candidate tables live at different linked VAs.
    """

    table: JumpTableWords
    branch_pc: int
    candidate_branch_pc: int | None = None
    candidate_table: JumpTableWords | None = None
    pairing: JumpTablePairing | None = None
    table_base_reg: int = 3
    candidate_table_base_reg: int | None = None
    index_reg: int = 0

    def candidate_table_words(self) -> JumpTableWords:
        return self.candidate_table if self.candidate_table is not None else self.table

    def original_expansion_map(self) -> dict[int, tuple[int, ...]]:
        targets = tuple(word & 0xFFFFFFFC for word in self.table.words)
        return {self.branch_pc: targets}

    def candidate_expansion_map(self) -> dict[int, tuple[int, ...]]:
        targets = tuple(
            word & 0xFFFFFFFC for word in self.candidate_table_words().words
        )
        branch = (
            self.candidate_branch_pc
            if self.candidate_branch_pc is not None
            else self.branch_pc
        )
        return {branch: targets}

    def expansion_map(self) -> dict[int, tuple[int, ...]]:
        """Merge per-side maps; safe only when branch PCs differ."""
        original = self.original_expansion_map()
        candidate = self.candidate_expansion_map()
        if original == candidate:
            return original
        merged = dict(original)
        merged.update(candidate)
        return merged

    def readonly_tables(self) -> tuple[JumpTableWords, ...]:
        """Distinct ROM images pinned during proof (deduped by base + digest)."""
        cand = self.candidate_table_words()
        if cand.base == self.table.base and cand.image_sha256 == self.table.image_sha256:
            return (self.table,)
        if cand.base == self.table.base:
            raise ValueError(
                "jump-table words disagree at the same base; "
                "cannot bind two images at one VA"
            )
        return (self.table, cand)

    def dual_base(self) -> bool:
        return self.candidate_table_words().base != self.table.base


REQUIRED_ADDRESS_SPACE_KEYS = frozenset({
    "kind",
    "base",
    "end",
    "image_sha256",
    "byte_count",
    "source",
})

REQUIRED_INDIRECT_TARGETS_KEYS = frozenset({
    "branch_pc",
    "targets",
    "coverage",
    "source",
    "artifact_hashes",
})


@dataclass(frozen=True)
class JumpTableWords:
    """Concrete table contents used to bind a ROM image and close targets."""

    base: int
    words: tuple[int, ...]
    source: str
    artifact_path: str = ""
    entry_size: int = 4

    def __post_init__(self) -> None:
        if self.base < 0 or self.base > 0xFFFFFFFF:
            raise ValueError(f"table base {self.base:#x} out of 32-bit space")
        if self.entry_size != 4:
            raise ValueError("only 4-byte ADDR32 jump tables are supported")
        if not self.words:
            raise ValueError("jump table words must be nonempty")
        for word in self.words:
            if word < 0 or word > 0xFFFFFFFF:
                raise ValueError(f"table word {word:#x} out of 32-bit space")

    @property
    def byte_count(self) -> int:
        return len(self.words) * self.entry_size

    @property
    def end(self) -> int:
        return self.base + self.byte_count - 1

    @property
    def image_bytes(self) -> bytes:
        return b"".join(word.to_bytes(4, "big") for word in self.words)

    @property
    def image_sha256(self) -> str:
        return hashlib.sha256(self.image_bytes).hexdigest()

    def region(self, *, label: str | None = None) -> Region:
        return rom_image_region(self.base, self.image_bytes, label=label)


def build_readonly_image_obligation(
    table: JumpTableWords,
    *,
    no_write_status: str = "pending",
    algorithm: str = "rom-image-v1",
) -> dict[str, Any]:
    """Obligation block for ``proof_features: [\"readonly-image\"]``."""
    return _readonly_image_obligation_body(
        table,
        no_write_status=no_write_status,
        algorithm=algorithm,
    )


def build_dual_readonly_image_obligation(
    original: JumpTableWords,
    candidate: JumpTableWords | None = None,
    *,
    no_write_status: str = "pending",
    algorithm: str = "rom-image-v1",
) -> dict[str, Any]:
    """Primary readonly-image obligation with optional candidate companion."""
    obligation = _readonly_image_obligation_body(
        original,
        no_write_status=no_write_status,
        algorithm=algorithm,
    )
    if candidate is not None and (
        candidate.base != original.base
        or candidate.image_sha256 != original.image_sha256
    ):
        obligation["candidate"] = _readonly_image_obligation_body(
            candidate,
            no_write_status=no_write_status,
            algorithm=algorithm,
        )
    return obligation


def build_jump_table_obligations(
    context: JumpTableProofContext,
    *,
    no_write_status: str = "unsat",
    coverage: str = "unsat-remainder",
) -> tuple[dict[str, Any], dict[str, Any]]:
    """Build ``address_space`` and ``indirect_targets`` for a proof context."""
    cand_table = context.candidate_table_words()
    address_space = build_dual_readonly_image_obligation(
        context.table,
        cand_table,
        no_write_status=no_write_status,
    )
    artifact_hashes = tuple(
        dict.fromkeys(table.image_sha256 for table in context.readonly_tables())
    )
    if context.pairing is not None:
        from tools.ppc_equivalence.jump_table_pairing import (
            indirect_targets_obligations_for_pairing,
        )

        original_ob, candidate_ob = indirect_targets_obligations_for_pairing(
            context.pairing,
            branch_pc_original=context.branch_pc,
            branch_pc_candidate=(
                context.candidate_branch_pc
                if context.candidate_branch_pc is not None
                else context.branch_pc
            ),
            source_original=context.table.source,
            source_candidate=cand_table.source,
            artifact_hashes=artifact_hashes,
            coverage=coverage,
        )
        indirect_targets = original_ob
        if candidate_ob["targets"] != original_ob["targets"]:
            indirect_targets = {**original_ob, "candidate": candidate_ob}
        return address_space, indirect_targets

    indirect_targets = build_indirect_targets_obligation(
        branch_pc=context.branch_pc,
        targets=tuple(
            (f"case-{index}", word & 0xFFFFFFFC)
            for index, word in enumerate(context.table.words)
        ),
        source=context.table.source,
        artifact_hashes=artifact_hashes,
        coverage=coverage,
    )
    if (
        context.candidate_branch_pc is not None
        or cand_table.words != context.table.words
    ):
        candidate_targets = build_indirect_targets_obligation(
            branch_pc=(
                context.candidate_branch_pc
                if context.candidate_branch_pc is not None
                else context.branch_pc
            ),
            targets=tuple(
                (f"case-{index}", word & 0xFFFFFFFC)
                for index, word in enumerate(cand_table.words)
            ),
            source=cand_table.source,
            artifact_hashes=artifact_hashes,
            coverage=coverage,
        )
        if candidate_targets["targets"] != indirect_targets["targets"]:
            indirect_targets = {**indirect_targets, "candidate": candidate_targets}
    return address_space, indirect_targets


def _readonly_image_obligation_body(
    table: JumpTableWords,
    *,
    no_write_status: str,
    algorithm: str,
) -> dict[str, Any]:
    return {
        "kind": RegionKind.ROM_IMAGE.value,
        "base": table.base,
        "end": table.end,
        "image_sha256": table.image_sha256,
        "byte_count": table.byte_count,
        "entry_count": len(table.words),
        "entry_size": table.entry_size,
        "source": table.source,
        "artifact_path": table.artifact_path,
        "no_write": no_write_status,
        "algorithm": algorithm,
    }


def build_indirect_targets_obligation(
    *,
    branch_pc: int,
    targets: Sequence[tuple[str, int]],
    source: str,
    artifact_hashes: Sequence[str],
    coverage: str = "pending",
    algorithm: str = "enumerated-addr32-v1",
) -> dict[str, Any]:
    """Obligation block for ``proof_features: [\"indirect-target-closure\"]``.

    ``targets`` entries are ``(logical_case_id, linked_pc)``.
    """
    if not targets:
        raise ValueError("indirect target set must be nonempty")
    resolved = []
    for identity, pc in targets:
        if not identity:
            raise ValueError("logical case identity must be non-empty")
        if pc < 0 or pc > 0xFFFFFFFF:
            raise ValueError(f"target pc {pc:#x} out of 32-bit space")
        if pc & 3:
            raise ValueError(f"target pc {pc:#x} is not word-aligned")
        resolved.append({"identity": identity, "pc": pc})
    return {
        "branch_pc": branch_pc,
        "targets": resolved,
        "coverage": coverage,
        "source": source,
        "artifact_hashes": list(artifact_hashes),
        "algorithm": algorithm,
    }


def validate_readonly_image_obligation(obligation: dict[str, Any]) -> str | None:
    missing = sorted(REQUIRED_ADDRESS_SPACE_KEYS - obligation.keys())
    if missing:
        return "address_space missing " + ", ".join(missing)
    if obligation.get("kind") != RegionKind.ROM_IMAGE.value:
        return "address_space.kind must be rom-image"
    for key in ("base", "end", "byte_count"):
        value = obligation.get(key)
        if not isinstance(value, int) or value < 0:
            return f"address_space.{key} must be a non-negative int"
    digest = obligation.get("image_sha256")
    if not isinstance(digest, str) or len(digest) != 64:
        return "address_space.image_sha256 must be a 64-hex digest"
    if obligation["end"] < obligation["base"]:
        return "address_space.end < base"
    if obligation["byte_count"] != obligation["end"] - obligation["base"] + 1:
        return "address_space.byte_count does not match [base, end]"
    return None


def validate_indirect_targets_obligation(obligation: dict[str, Any]) -> str | None:
    missing = sorted(REQUIRED_INDIRECT_TARGETS_KEYS - obligation.keys())
    if missing:
        return "indirect_targets missing " + ", ".join(missing)
    targets = obligation.get("targets")
    if not isinstance(targets, list) or not targets:
        return "indirect_targets.targets must be a nonempty list"
    seen: set[str] = set()
    for index, entry in enumerate(targets):
        if not isinstance(entry, dict):
            return f"indirect_targets.targets[{index}] must be an object"
        identity = entry.get("identity")
        pc = entry.get("pc")
        if not isinstance(identity, str) or not identity:
            return f"indirect_targets.targets[{index}].identity must be a nonempty string"
        if identity in seen:
            return f"duplicate logical case identity {identity!r}"
        seen.add(identity)
        if not isinstance(pc, int) or pc < 0 or pc > 0xFFFFFFFF or (pc & 3):
            return f"indirect_targets.targets[{index}].pc must be a word-aligned u32"
    hashes = obligation.get("artifact_hashes")
    if not isinstance(hashes, list) or not all(isinstance(item, str) and item for item in hashes):
        return "indirect_targets.artifact_hashes must be a nonempty string list"
    if not hashes:
        return "indirect_targets.artifact_hashes must be a nonempty string list"
    coverage = obligation.get("coverage")
    if not isinstance(coverage, str) or not coverage:
        return "indirect_targets.coverage must be a nonempty string"
    return None


def rom_image_byte_constraints(initial_memory: Any, table: JumpTableWords, ops: Any) -> list[Any]:
    """Pin initial memory bytes in ``[base, end]`` to the immutable table image."""
    constraints: list[Any] = []
    image = table.image_bytes
    for offset, value in enumerate(image):
        address = ops.const((table.base + offset) & 0xFFFFFFFF)
        constraints.append(
            ops.eq(ops.load_byte(initial_memory, address), ops.const(value)),
        )
    return constraints


def rom_image_no_write_constraints(
    terminals: Sequence[Any],
    initial_memory: Any,
    table: JumpTableWords,
    ops: Any,
) -> list[Any]:
    """Require every terminal memory to preserve ROM image bytes (alias-safe)."""
    z3 = ops.z3
    constraints: list[Any] = []
    image = table.image_bytes
    for terminal in terminals:
        for offset, value in enumerate(image):
            address = ops.const((table.base + offset) & 0xFFFFFFFF)
            preserved = ops.eq(
                ops.load_byte(terminal.state.memory, address),
                ops.const(value),
            )
            constraints.append(z3.Implies(terminal.condition, preserved))
    return constraints


def indirect_target_closure_constraints(
    terminals: Sequence[Any],
    *,
    target_pcs: Sequence[int],
    ops: Any,
) -> list[Any]:
    """Under indirect-branch exits, require exit_target ∈ enumerated PCs."""
    if not target_pcs:
        raise ValueError("target_pcs must be nonempty")
    z3 = ops.z3
    members = [ops.const(pc & 0xFFFFFFFC) for pc in target_pcs]
    constraints: list[Any] = []
    for terminal in terminals:
        if getattr(terminal, "exit_kind", None) != "indirect-branch":
            continue
        if terminal.exit_target is None:
            continue
        membership = z3.Or(*[ops.eq(terminal.exit_target, member) for member in members])
        constraints.append(z3.Implies(terminal.condition, membership))
    return constraints


def jump_table_gate_reason(
    original: Sequence[Instruction],
    candidate: Sequence[Instruction],
) -> str | None:
    """Return a fail-closed reason when jump tables are present but unproven."""
    left = find_jump_table_candidates(original)
    right = find_jump_table_candidates(candidate)
    if not left and not right:
        return None
    notes: list[str] = []
    if left:
        notes.append(f"original={len(left)}")
    if right:
        notes.append(f"candidate={len(right)}")
    return (
        "jump-table pattern requires readonly-image + indirect-target-closure "
        f"obligations before EQUIVALENT ({', '.join(notes)})"
    )


def summarize_candidates(candidates: Sequence[JumpTableCandidate]) -> list[dict[str, Any]]:
    return [
        {
            "branch_pc": item.branch_pc,
            "index_reg": item.index_reg,
            "bound_imm": item.bound_imm,
            "confidence": item.confidence,
            "notes": list(item.notes),
        }
        for item in candidates
    ]

"""Jump-table proof obligations: readonly image binding and target closure.

Pattern recognition (``jump_table.find_jump_table_candidates``) is not a proof.
These helpers build the obligation payloads and SMT constraints required before
``readonly-image`` / ``indirect-target-closure`` may authorize ``EQUIVALENT``.

Independent UNSAT discharge (see ``discharge.py``) proves coverage and no-write;
the main equivalence solver must not treat those as mismatch constraints alone.
"""

from __future__ import annotations

import hashlib
import re
from dataclasses import dataclass
from pathlib import Path
from typing import TYPE_CHECKING, Any, Literal, Sequence

from tools.ppc_equivalence.address_space import Region, RegionKind, rom_image_region
from tools.ppc_equivalence.discharge import UnsatDischarge
from tools.ppc_equivalence.ir import Instruction
from tools.ppc_equivalence.jump_table import JumpTableCandidate, find_jump_table_candidates

if TYPE_CHECKING:
    from tools.ppc_equivalence.deadline import Deadline
    from tools.ppc_equivalence.jump_table_pairing import JumpTablePairing


@dataclass(frozen=True)
class SideArtifact:
    """Per-implementation linked image used to hydrate jump-table words."""

    kind: Literal["dol", "elf"]
    path: Path
    sha256: str

    def __post_init__(self) -> None:
        if self.kind not in ("dol", "elf"):
            raise ValueError(f"unsupported artifact kind {self.kind!r}")
        if not isinstance(self.path, Path):
            object.__setattr__(self, "path", Path(self.path))
        if not isinstance(self.sha256, str) or len(self.sha256) != 64:
            raise ValueError("artifact sha256 must be a 64-hex digest")


@dataclass(frozen=True)
class JumpTableArtifacts:
    """Fail-closed per-side artifacts; no shared dol/elf fallback."""

    original: SideArtifact
    candidate: SideArtifact


@dataclass(frozen=True)
class JumpTableProofContext:
    """Premises for a jump-table EQUIVALENT attempt.

    ``branch_pc`` is the ``bctr`` address. Optional ``candidate_branch_pc``
    covers differently-based candidate images.

    ``table_base_reg`` / ``index_reg`` are descriptive only — they must not be
    auto-pinned as solver premises. Base and index bounds come from executed
    code path conditions (or an explicit future Tier-C contract precondition).

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
    artifacts: JumpTableArtifacts | None = None

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


REQUIRED_SIDE_ROM_KEYS = frozenset({
    "base",
    "end",
    "image_sha256",
    "byte_count",
    "source",
})

REQUIRED_SIDE_TARGETS_KEYS = frozenset({
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


def _discharge_status_payload(
    no_write: str | dict[str, Any],
) -> str | dict[str, Any]:
    return no_write


def _side_rom_body(
    table: JumpTableWords,
    *,
    no_write: str | dict[str, Any] = "pending",
    artifact_sha256: str | None = None,
) -> dict[str, Any]:
    body: dict[str, Any] = {
        "base": table.base,
        "end": table.end,
        "image_sha256": table.image_sha256,
        "byte_count": table.byte_count,
        "entry_count": len(table.words),
        "entry_size": table.entry_size,
        "source": table.source,
        "artifact_path": table.artifact_path,
        "no_write": _discharge_status_payload(no_write),
    }
    if artifact_sha256 is not None:
        body["artifact_sha256"] = artifact_sha256
    return body


def build_readonly_image_obligation(
    table: JumpTableWords,
    *,
    no_write_status: str | dict[str, Any] = "pending",
    algorithm: str = "rom-image-v2",
    artifact_sha256: str | None = None,
    status: str = "pending",
) -> dict[str, Any]:
    """Obligation block for ``proof_features: [\"readonly-image\"]`` (schema v2)."""
    return {
        "schema_version": 2,
        "algorithm": algorithm,
        "status": status,
        "kind": RegionKind.ROM_IMAGE.value,
        "original": _side_rom_body(
            table,
            no_write=no_write_status,
            artifact_sha256=artifact_sha256,
        ),
        # Flat mirrors for identity/cache consumers that still read top-level keys.
        "base": table.base,
        "end": table.end,
        "image_sha256": table.image_sha256,
        "byte_count": table.byte_count,
        "source": table.source,
        "no_write": no_write_status if isinstance(no_write_status, str) else no_write_status.get("result", "pending"),
    }


def build_dual_readonly_image_obligation(
    original: JumpTableWords,
    candidate: JumpTableWords | None = None,
    *,
    no_write_status: str | dict[str, Any] = "pending",
    candidate_no_write_status: str | dict[str, Any] | None = None,
    algorithm: str = "rom-image-v2",
    original_artifact_sha256: str | None = None,
    candidate_artifact_sha256: str | None = None,
    status: str = "pending",
) -> dict[str, Any]:
    """Primary readonly-image obligation with per-side original/candidate bodies."""
    cand_no_write = (
        no_write_status if candidate_no_write_status is None else candidate_no_write_status
    )
    obligation = build_readonly_image_obligation(
        original,
        no_write_status=no_write_status,
        algorithm=algorithm,
        artifact_sha256=original_artifact_sha256,
        status=status,
    )
    if candidate is not None and (
        candidate.base != original.base
        or candidate.image_sha256 != original.image_sha256
    ):
        obligation["candidate"] = _side_rom_body(
            candidate,
            no_write=cand_no_write,
            artifact_sha256=candidate_artifact_sha256,
        )
    else:
        obligation["candidate"] = obligation["original"]
    return obligation


def build_indirect_targets_obligation(
    *,
    branch_pc: int,
    targets: Sequence[tuple[str, int]],
    source: str,
    artifact_hashes: Sequence[str],
    coverage: str | dict[str, Any] = "pending",
    algorithm: str = "enumerated-addr32-v2",
    status: str = "pending",
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
    side = {
        "branch_pc": branch_pc,
        "targets": resolved,
        "coverage": coverage,
        "source": source,
        "artifact_hashes": list(artifact_hashes),
    }
    coverage_flat = coverage if isinstance(coverage, str) else coverage.get("result", "pending")
    return {
        "schema_version": 2,
        "algorithm": algorithm,
        "status": status,
        "original": side,
        # Flat mirrors for identity/cache consumers.
        "branch_pc": branch_pc,
        "targets": resolved,
        "coverage": coverage_flat,
        "source": source,
        "artifact_hashes": list(artifact_hashes),
    }


def build_jump_table_obligations(
    context: JumpTableProofContext,
    *,
    no_write_status: str | dict[str, Any] = "pending",
    candidate_no_write_status: str | dict[str, Any] | None = None,
    coverage: str | dict[str, Any] = "pending",
    candidate_coverage: str | dict[str, Any] | None = None,
    status: str = "pending",
) -> tuple[dict[str, Any], dict[str, Any]]:
    """Build ``address_space`` and ``indirect_targets`` for a proof context."""
    cand_table = context.candidate_table_words()
    orig_art = None
    cand_art = None
    if context.artifacts is not None:
        orig_art = context.artifacts.original.sha256
        cand_art = context.artifacts.candidate.sha256
    address_space = build_dual_readonly_image_obligation(
        context.table,
        cand_table,
        no_write_status=no_write_status,
        candidate_no_write_status=candidate_no_write_status,
        original_artifact_sha256=orig_art,
        candidate_artifact_sha256=cand_art,
        status=status,
    )
    artifact_hashes = tuple(
        dict.fromkeys(table.image_sha256 for table in context.readonly_tables())
    )
    cand_cov = coverage if candidate_coverage is None else candidate_coverage
    branch_candidate = (
        context.candidate_branch_pc
        if context.candidate_branch_pc is not None
        else context.branch_pc
    )
    if context.pairing is not None:
        from tools.ppc_equivalence.jump_table_pairing import (
            indirect_targets_obligations_for_pairing,
        )

        original_ob, candidate_ob = indirect_targets_obligations_for_pairing(
            context.pairing,
            branch_pc_original=context.branch_pc,
            branch_pc_candidate=branch_candidate,
            source_original=context.table.source,
            source_candidate=cand_table.source,
            artifact_hashes=artifact_hashes,
            coverage=coverage if isinstance(coverage, str) else "pending",
        )
        # Lift into schema v2 wrapper with discharge digests when provided.
        indirect_targets = build_indirect_targets_obligation(
            branch_pc=context.branch_pc,
            targets=tuple(
                (entry["identity"], entry["pc"]) for entry in original_ob["targets"]
            ),
            source=context.table.source,
            artifact_hashes=artifact_hashes,
            coverage=coverage,
            status=status,
        )
        candidate_side = build_indirect_targets_obligation(
            branch_pc=branch_candidate,
            targets=tuple(
                (entry["identity"], entry["pc"]) for entry in candidate_ob["targets"]
            ),
            source=cand_table.source,
            artifact_hashes=artifact_hashes,
            coverage=cand_cov,
            status=status,
        )
        indirect_targets["candidate"] = candidate_side["original"]
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
        status=status,
    )
    if (
        context.candidate_branch_pc is not None
        or cand_table.words != context.table.words
    ):
        candidate_targets = build_indirect_targets_obligation(
            branch_pc=branch_candidate,
            targets=tuple(
                (f"case-{index}", word & 0xFFFFFFFC)
                for index, word in enumerate(cand_table.words)
            ),
            source=cand_table.source,
            artifact_hashes=artifact_hashes,
            coverage=cand_cov,
            status=status,
        )
        indirect_targets["candidate"] = candidate_targets["original"]
    else:
        indirect_targets["candidate"] = indirect_targets["original"]
    return address_space, indirect_targets


def _validate_side_rom(side: Any, *, label: str) -> str | None:
    if not isinstance(side, dict):
        return f"{label} must be an object"
    missing = sorted(REQUIRED_SIDE_ROM_KEYS - side.keys())
    if missing:
        return f"{label} missing " + ", ".join(missing)
    for key in ("base", "end", "byte_count"):
        value = side.get(key)
        if not isinstance(value, int) or value < 0:
            return f"{label}.{key} must be a non-negative int"
    digest = side.get("image_sha256")
    if not isinstance(digest, str) or len(digest) != 64:
        return f"{label}.image_sha256 must be a 64-hex digest"
    if side["end"] < side["base"]:
        return f"{label}.end < base"
    if side["byte_count"] != side["end"] - side["base"] + 1:
        return f"{label}.byte_count does not match [base, end]"
    no_write = side.get("no_write")
    if no_write is None:
        return f"{label}.no_write is required"
    if isinstance(no_write, str):
        if not no_write:
            return f"{label}.no_write must be a nonempty string"
    elif isinstance(no_write, dict):
        result = no_write.get("result")
        query = no_write.get("query_sha256")
        if not isinstance(result, str) or not result:
            return f"{label}.no_write.result must be a nonempty string"
        if not isinstance(query, str) or len(query) != 64:
            return f"{label}.no_write.query_sha256 must be a 64-hex digest"
    else:
        return f"{label}.no_write must be a string or discharge object"
    return None


def _validate_side_targets(side: Any, *, label: str) -> str | None:
    if not isinstance(side, dict):
        return f"{label} must be an object"
    missing = sorted(REQUIRED_SIDE_TARGETS_KEYS - side.keys())
    if missing:
        return f"{label} missing " + ", ".join(missing)
    targets = side.get("targets")
    if not isinstance(targets, list) or not targets:
        return f"{label}.targets must be a nonempty list"
    seen: set[str] = set()
    for index, entry in enumerate(targets):
        if not isinstance(entry, dict):
            return f"{label}.targets[{index}] must be an object"
        identity = entry.get("identity")
        pc = entry.get("pc")
        if not isinstance(identity, str) or not identity:
            return f"{label}.targets[{index}].identity must be a nonempty string"
        if identity in seen:
            return f"duplicate logical case identity {identity!r}"
        seen.add(identity)
        if not isinstance(pc, int) or pc < 0 or pc > 0xFFFFFFFF or (pc & 3):
            return f"{label}.targets[{index}].pc must be a word-aligned u32"
    hashes = side.get("artifact_hashes")
    if not isinstance(hashes, list) or not all(isinstance(item, str) and item for item in hashes):
        return f"{label}.artifact_hashes must be a nonempty string list"
    if not hashes:
        return f"{label}.artifact_hashes must be a nonempty string list"
    coverage = side.get("coverage")
    if isinstance(coverage, str):
        if not coverage:
            return f"{label}.coverage must be a nonempty string"
    elif isinstance(coverage, dict):
        result = coverage.get("result")
        query = coverage.get("query_sha256")
        if not isinstance(result, str) or not result:
            return f"{label}.coverage.result must be a nonempty string"
        if not isinstance(query, str) or len(query) != 64:
            return f"{label}.coverage.query_sha256 must be a 64-hex digest"
    else:
        return f"{label}.coverage must be a string or discharge object"
    return None


def validate_readonly_image_obligation(obligation: dict[str, Any]) -> str | None:
    schema = obligation.get("schema_version", 1)
    if schema == 2:
        if obligation.get("kind") != RegionKind.ROM_IMAGE.value:
            return "address_space.kind must be rom-image"
        original = obligation.get("original")
        reason = _validate_side_rom(original, label="address_space.original")
        if reason is not None:
            return reason
        if "candidate" in obligation:
            reason = _validate_side_rom(
                obligation["candidate"], label="address_space.candidate",
            )
            if reason is not None:
                return reason
        return None
    # Legacy flat schema (v1).
    missing = sorted(
        frozenset({
            "kind", "base", "end", "image_sha256", "byte_count", "source",
        })
        - obligation.keys()
    )
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


# Strict validators for the EQUIVALENT-ready path (no legacy schema fallback).
_SHA256_LOWER_RE = re.compile(r"^[0-9a-f]{64}$")

# rom-image-v2 / enumerated-addr32-v2 are the jump-table algorithms; the
# virtual-call variants share the readonly-image / indirect-target-closure
# feature names and must also be accepted by the strict validators.
KNOWN_READONLY_IMAGE_ALGORITHMS = frozenset({
    "rom-image-v2",
    "virtual-call-rom-image-v1",
})
KNOWN_INDIRECT_TARGET_ALGORITHMS = frozenset({
    "enumerated-addr32-v2",
    "virtual-call-enumerated-v1",
})


def _is_sha256_lower(value: Any) -> bool:
    return isinstance(value, str) and _SHA256_LOWER_RE.fullmatch(value) is not None


def _discharge_result(value: Any) -> str | None:
    """Return the discharge result string from a str or discharge-dict form."""
    if isinstance(value, str):
        return value
    if isinstance(value, dict):
        result = value.get("result")
        return result if isinstance(result, str) else None
    return None


def _validate_side_rom_strict(
    side: Any, *, label: str, require_discharged: bool,
) -> str | None:
    reason = _validate_side_rom(side, label=label)
    if reason is not None:
        return reason
    digest = side.get("image_sha256")
    if not _is_sha256_lower(digest):
        return f"{label}.image_sha256 must be a lowercase 64-hex digest"
    artifact = side.get("artifact_sha256")
    if artifact is not None and not _is_sha256_lower(artifact):
        return f"{label}.artifact_sha256 must be a lowercase 64-hex digest"
    if require_discharged:
        no_write = side.get("no_write")
        if not isinstance(no_write, dict):
            return f"{label}.no_write must be a discharge object for discharged proofs"
        if not _is_sha256_lower(no_write.get("query_sha256")):
            return f"{label}.no_write.query_sha256 must be a lowercase 64-hex digest"
        if no_write.get("result") != "unsat":
            return f"{label}.no_write.result must be 'unsat' for discharged proofs"
    return None


def _validate_side_targets_strict(
    side: Any, *, label: str, require_discharged: bool,
) -> str | None:
    reason = _validate_side_targets(side, label=label)
    if reason is not None:
        return reason
    for index, digest in enumerate(side.get("artifact_hashes", [])):
        if not _is_sha256_lower(digest):
            return f"{label}.artifact_hashes[{index}] must be a lowercase 64-hex digest"
    if require_discharged:
        coverage = side.get("coverage")
        if not isinstance(coverage, dict):
            return f"{label}.coverage must be a discharge object for discharged proofs"
        if not _is_sha256_lower(coverage.get("query_sha256")):
            return f"{label}.coverage.query_sha256 must be a lowercase 64-hex digest"
        if coverage.get("result") != "unsat":
            return f"{label}.coverage.result must be 'unsat' for discharged proofs"
    return None


def validate_readonly_image_obligation_strict(
    obligation: dict[str, Any], *, require_discharged: bool = False,
) -> str | None:
    """Strict schema-v2 readonly-image validation for the EQUIVALENT path."""
    if not isinstance(obligation, dict):
        return "address_space must be an object"
    if obligation.get("schema_version") != 2:
        return "address_space.schema_version must be exactly 2 for EQUIVALENT proofs"
    if obligation.get("kind") != RegionKind.ROM_IMAGE.value:
        return "address_space.kind must be rom-image"
    algorithm = obligation.get("algorithm")
    if algorithm not in KNOWN_READONLY_IMAGE_ALGORITHMS:
        return (
            f"address_space.algorithm {algorithm!r} is not a known "
            "readonly-image algorithm"
        )
    for label in ("original", "candidate"):
        side = obligation.get(label)
        if not isinstance(side, dict):
            return f"address_space.{label} is required (both sides)"
        reason = _validate_side_rom_strict(
            side, label=f"address_space.{label}", require_discharged=require_discharged,
        )
        if reason is not None:
            return reason
    original = obligation["original"]
    for key in ("base", "end", "image_sha256", "byte_count", "source"):
        if key in obligation and obligation[key] != original.get(key):
            return f"address_space.{key} flat mirror disagrees with original side"
    if "no_write" in obligation:
        flat = _discharge_result(obligation["no_write"])
        side_result = _discharge_result(original.get("no_write"))
        if flat != side_result:
            return "address_space.no_write flat mirror disagrees with original side"
    if require_discharged and obligation.get("status") != "discharged":
        return "address_space.status must be 'discharged' for EQUIVALENT proofs"
    return None


def validate_indirect_targets_obligation_strict(
    obligation: dict[str, Any], *, require_discharged: bool = False,
) -> str | None:
    """Strict schema-v2 indirect-target-closure validation for EQUIVALENT."""
    if not isinstance(obligation, dict):
        return "indirect_targets must be an object"
    if obligation.get("schema_version") != 2:
        return "indirect_targets.schema_version must be exactly 2 for EQUIVALENT proofs"
    algorithm = obligation.get("algorithm")
    if algorithm not in KNOWN_INDIRECT_TARGET_ALGORITHMS:
        return (
            f"indirect_targets.algorithm {algorithm!r} is not a known "
            "indirect-target-closure algorithm"
        )
    for label in ("original", "candidate"):
        side = obligation.get(label)
        if not isinstance(side, dict):
            return f"indirect_targets.{label} is required (both sides)"
        reason = _validate_side_targets_strict(
            side,
            label=f"indirect_targets.{label}",
            require_discharged=require_discharged,
        )
        if reason is not None:
            return reason
    original = obligation["original"]
    for key in ("branch_pc", "source"):
        if key in obligation and obligation[key] != original.get(key):
            return f"indirect_targets.{key} flat mirror disagrees with original side"
    if "coverage" in obligation:
        flat = _discharge_result(obligation["coverage"])
        side_result = _discharge_result(original.get("coverage"))
        if flat != side_result:
            return "indirect_targets.coverage flat mirror disagrees with original side"
    if require_discharged and obligation.get("status") != "discharged":
        return "indirect_targets.status must be 'discharged' for EQUIVALENT proofs"
    return None


def validate_indirect_targets_obligation(obligation: dict[str, Any]) -> str | None:
    schema = obligation.get("schema_version", 1)
    if schema == 2:
        original = obligation.get("original")
        reason = _validate_side_targets(original, label="indirect_targets.original")
        if reason is not None:
            return reason
        if "candidate" in obligation:
            reason = _validate_side_targets(
                obligation["candidate"], label="indirect_targets.candidate",
            )
            if reason is not None:
                return reason
        return None
    # Legacy flat schema (v1).
    missing = sorted(
        frozenset({
            "branch_pc", "targets", "coverage", "source", "artifact_hashes",
        })
        - obligation.keys()
    )
    if missing:
        return "indirect_targets missing " + ", ".join(missing)
    return _validate_side_targets(obligation, label="indirect_targets")


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
    """Legacy final-value preservation constraints (alias-safe).

    Prefer :func:`write_hits_table_conditions` + independent discharge for
    sound no-write proofs; final-value equality alone misses store-same-value.
    """
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


def write_hits_table_conditions(
    terminals: Sequence[Any],
    table: JumpTableWords,
    ops: Any,
) -> list[Any]:
    """Conditions where a recorded symbolic write byte address hits the table.

    Uses ``memory_writes`` tracking — not final memory values — so store-same-
    value and store-restore into ROM remain detectable.
    """
    base = ops.const(table.base & 0xFFFFFFFF)
    end = ops.const(table.end & 0xFFFFFFFF)
    bad: list[Any] = []
    for terminal in terminals:
        writes = getattr(terminal.state, "memory_writes", ()) or ()
        for write_addr in writes:
            hits = ops.land(
                ops.lnot(ops.unsigned_lt(write_addr, base)),
                ops.lnot(ops.unsigned_lt(end, write_addr)),
            )
            bad.append(ops.land(terminal.condition, hits))
    return bad


def remainder_closure_conditions(terminals: Sequence[Any]) -> list[Any]:
    """Collect path conditions for explicit ``indirect-branch`` remainder exits."""
    return [
        terminal.condition
        for terminal in terminals
        if getattr(terminal, "exit_kind", None) == "indirect-branch"
    ]


def indirect_target_closure_constraints(
    terminals: Sequence[Any],
    *,
    target_pcs: Sequence[int],
    ops: Any,
) -> list[Any]:
    """Legacy membership constraints under indirect-branch exits.

    Prefer remainder retention + :func:`remainder_closure_conditions` discharge.
    """
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


@dataclass(frozen=True)
class JumpTableDischargeBundle:
    """Per-side closure + no-write discharge results for obligation digests."""

    coverage_original: UnsatDischarge
    coverage_candidate: UnsatDischarge
    no_write_original: UnsatDischarge
    no_write_candidate: UnsatDischarge

    def all_unsat(self) -> bool:
        return all(
            item.status == "unsat"
            for item in (
                self.coverage_original,
                self.coverage_candidate,
                self.no_write_original,
                self.no_write_candidate,
            )
        )

    def failure_reason(self) -> str | None:
        mapping = (
            ("original coverage", self.coverage_original),
            ("candidate coverage", self.coverage_candidate),
            ("original no-write", self.no_write_original),
            ("candidate no-write", self.no_write_candidate),
        )
        for label, item in mapping:
            if item.status != "unsat":
                return f"jump-table {label} discharge {item.status}"
        return None


def discharge_jump_table_obligations(
    context: JumpTableProofContext,
    *,
    original_terminals: Sequence[Any],
    candidate_terminals: Sequence[Any],
    premises: Sequence[Any],
    ops: Any,
    deadline: Deadline,
) -> JumpTableDischargeBundle:
    """Independent coverage + no-write UNSAT queries (no mismatch formula)."""
    from tools.ppc_equivalence.discharge import discharge_bad_conditions

    z3 = ops.z3
    coverage_original = discharge_bad_conditions(
        premises=premises,
        bad_conditions=remainder_closure_conditions(original_terminals),
        deadline=deadline,
        algorithm="indirect-target-closure-v2",
        z3_module=z3,
    )
    coverage_candidate = discharge_bad_conditions(
        premises=premises,
        bad_conditions=remainder_closure_conditions(candidate_terminals),
        deadline=deadline,
        algorithm="indirect-target-closure-v2",
        z3_module=z3,
    )
    no_write_original = discharge_bad_conditions(
        premises=premises,
        bad_conditions=write_hits_table_conditions(
            original_terminals, context.table, ops,
        ),
        deadline=deadline,
        algorithm="rom-image-no-write-v2",
        z3_module=z3,
    )
    cand_table = context.candidate_table_words()
    no_write_candidate = discharge_bad_conditions(
        premises=premises,
        bad_conditions=write_hits_table_conditions(
            candidate_terminals, cand_table, ops,
        ),
        deadline=deadline,
        algorithm="rom-image-no-write-v2",
        z3_module=z3,
    )
    return JumpTableDischargeBundle(
        coverage_original=coverage_original,
        coverage_candidate=coverage_candidate,
        no_write_original=no_write_original,
        no_write_candidate=no_write_candidate,
    )


def obligations_from_discharge(
    context: JumpTableProofContext,
    bundle: JumpTableDischargeBundle,
) -> tuple[dict[str, Any], dict[str, Any]]:
    """Build schema-v2 obligations carrying discharge digests."""
    status = "discharged" if bundle.all_unsat() else "failed"
    return build_jump_table_obligations(
        context,
        no_write_status=bundle.no_write_original.to_obligation_digest(),
        candidate_no_write_status=bundle.no_write_candidate.to_obligation_digest(),
        coverage=bundle.coverage_original.to_obligation_digest(),
        candidate_coverage=bundle.coverage_candidate.to_obligation_digest(),
        status=status,
    )


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


def side_artifact_from_path(path: Path | str, *, kind: Literal["dol", "elf"] | None = None) -> SideArtifact:
    """Build a ``SideArtifact`` by hashing file bytes."""
    resolved = Path(path)
    data = resolved.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    inferred: Literal["dol", "elf"]
    if kind is not None:
        inferred = kind
    elif resolved.suffix.lower() == ".dol":
        inferred = "dol"
    else:
        inferred = "elf"
    return SideArtifact(kind=inferred, path=resolved, sha256=digest)

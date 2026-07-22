"""Virtual-call proof obligations: vtable image binding and target closure.

Pattern recognition (``vtable.find_virtual_call_candidates``) is not a proof.
These helpers build obligation payloads and discharge scaffolding required
before ``readonly-image`` / ``indirect-target-closure`` may authorize
``EQUIVALENT`` for linked virtual calls.
"""

from __future__ import annotations

import hashlib
from dataclasses import dataclass
from pathlib import Path
from typing import TYPE_CHECKING, Any, Literal, Mapping, Sequence

from tools.ppc_equivalence.address_space import Region, rom_image_region
from tools.ppc_equivalence.discharge import UnsatDischarge
from tools.ppc_equivalence.jump_table_obligations import (
    SideArtifact,
    build_dual_readonly_image_obligation,
    build_indirect_targets_obligation,
    remainder_closure_conditions,
)
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.vtable import (
    VirtualCallCandidate,
    find_virtual_call_candidates,
    find_virtual_thunk_candidates,
)
from tools.ppc_equivalence.vtable_provenance import (
    VptrProvenance,
    classify_vptr_provenance,
    provenance_gate_reason,
)
from tools.ppc_equivalence.vtable_pairing import (
    VirtualCallPairing,
    VirtualCallPairingError,
    indirect_targets_obligations_for_pairing,
    pair_virtual_call_targets,
)

if TYPE_CHECKING:
    from tools.ppc_equivalence.deadline import Deadline
    from tools.ppc_equivalence.ir import Instruction


@dataclass(frozen=True)
class VirtualCallArtifacts:
    """Fail-closed per-side artifacts; no shared dol/elf fallback."""

    original: SideArtifact
    candidate: SideArtifact


@dataclass(frozen=True)
class VtableSlotWords:
    """Concrete vtable slot word(s) bound to a readonly ROM image."""

    base: int
    words: tuple[int, ...]
    slot_offset: int
    source: str
    artifact_path: str = ""
    entry_size: int = 4

    def __post_init__(self) -> None:
        if self.base < 0 or self.base > 0xFFFFFFFF:
            raise ValueError(f"vtable base {self.base:#x} out of 32-bit space")
        if self.entry_size != 4:
            raise ValueError("only 4-byte ADDR32 vtable slots are supported")
        if not self.words:
            raise ValueError("vtable slot words must be nonempty")
        if self.slot_offset < 0 or self.slot_offset > 0xFFFFFFFF:
            raise ValueError(f"slot offset {self.slot_offset:#x} out of 32-bit space")
        if self.slot_offset & 3:
            raise ValueError(f"slot offset {self.slot_offset:#x} is not word-aligned")
        for word in self.words:
            if word < 0 or word > 0xFFFFFFFF:
                raise ValueError(f"vtable word {word:#x} out of 32-bit space")

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


@dataclass(frozen=True)
class SemanticCalleeCertificate:
    """Current semantic callee certificate required for composition."""

    target_id: str
    symbol: str
    certificate_sha256: str
    retail_sha256: str
    candidate_sha256: str


@dataclass(frozen=True)
class VirtualCallProofContext:
    """Premises for a virtual-call EQUIVALENT attempt."""

    candidate: VirtualCallCandidate
    slot: VtableSlotWords
    pairing: VirtualCallPairing
    provenance: VptrProvenance
    branch_pc: int
    candidate_branch_pc: int | None = None
    candidate_slot: VtableSlotWords | None = None
    artifacts: VirtualCallArtifacts | None = None
    callee_certificate: SemanticCalleeCertificate | None = None

    def candidate_slot_words(self) -> VtableSlotWords:
        return self.candidate_slot if self.candidate_slot is not None else self.slot

    def original_expansion_map(self) -> dict[int, tuple[int, ...]]:
        targets = tuple(word & 0xFFFFFFFC for word in self.slot.words)
        return {self.branch_pc: targets}

    def candidate_expansion_map(self) -> dict[int, tuple[int, ...]]:
        cand = self.candidate_slot_words()
        targets = tuple(word & 0xFFFFFFFC for word in cand.words)
        branch = (
            self.candidate_branch_pc
            if self.candidate_branch_pc is not None
            else self.branch_pc
        )
        return {branch: targets}

    def readonly_tables(self) -> tuple[VtableSlotWords, ...]:
        cand = self.candidate_slot_words()
        if cand.base == self.slot.base and cand.image_sha256 == self.slot.image_sha256:
            return (self.slot,)
        if cand.base == self.slot.base:
            raise ValueError(
                "vtable words disagree at the same base; "
                "cannot bind two images at one VA"
            )
        return (self.slot, cand)


def detect_callee_cycle(
    edges: Mapping[str, frozenset[str]],
    *,
    root: str,
) -> tuple[str, ...] | None:
    """Return a cycle path containing ``root``, or ``None`` when acyclic."""
    path: list[str] = []
    active: set[str] = set()

    def _visit(node: str) -> tuple[str, ...] | None:
        if node in active:
            start = path.index(node)
            return tuple(path[start:] + [node])
        active.add(node)
        path.append(node)
        for callee in sorted(edges.get(node, ())):
            cycle = _visit(callee)
            if cycle is not None:
                return cycle
        path.pop()
        active.remove(node)
        return None

    return _visit(root)


def virtual_call_scc_status(
    edges: Mapping[str, frozenset[str]],
    *,
    root: str,
) -> ProofStatus | None:
    """Map an SCC containing ``root`` to the fail-closed proof status."""
    cycle = detect_callee_cycle(edges, root=root)
    if cycle is None:
        return None
    return ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE


def validate_semantic_callee_certificate(
    pairing: VirtualCallPairing,
    certificate: SemanticCalleeCertificate | None,
) -> str | None:
    """Require a current semantic callee certificate for the paired identity."""
    if certificate is None:
        identity = pairing.cases[0].identity if pairing.cases else "<unknown>"
        return f"missing semantic callee certificate for {identity!r}"
    if not certificate.certificate_sha256 or len(certificate.certificate_sha256) != 64:
        return "callee certificate_sha256 must be a 64-hex digest"
    if certificate.symbol != pairing.cases[0].identity and pairing.cases:
        # Allow slot-* identities when no symbol was resolved on either side.
        if not pairing.cases[0].identity.startswith("slot-"):
            return (
                f"certificate symbol {certificate.symbol!r} "
                f"does not match pairing identity {pairing.cases[0].identity!r}"
            )
    return None


def build_virtual_call_obligations(
    context: VirtualCallProofContext,
    *,
    no_write_status: str | dict[str, Any] = "pending",
    candidate_no_write_status: str | dict[str, Any] | None = None,
    coverage: str | dict[str, Any] = "pending",
    candidate_coverage: str | dict[str, Any] | None = None,
    status: str = "pending",
) -> tuple[dict[str, Any], dict[str, Any]]:
    """Build ``address_space`` and ``indirect_targets`` for a proof context."""
    reason = provenance_gate_reason(context.provenance)
    if reason is not None:
        raise ValueError(reason)
    cert_reason = validate_semantic_callee_certificate(
        context.pairing,
        context.callee_certificate,
    )
    if cert_reason is not None:
        raise ValueError(cert_reason)

    cand_slot = context.candidate_slot_words()
    orig_art = None
    cand_art = None
    if context.artifacts is not None:
        orig_art = context.artifacts.original.sha256
        cand_art = context.artifacts.candidate.sha256

    address_space = build_dual_readonly_image_obligation(
        _slot_as_jump_table_words(context.slot),
        _slot_as_jump_table_words(cand_slot),
        no_write_status=no_write_status,
        candidate_no_write_status=candidate_no_write_status,
        original_artifact_sha256=orig_art,
        candidate_artifact_sha256=cand_art,
        status=status,
    )
    address_space["slot_offset"] = context.slot.slot_offset
    address_space["algorithm"] = "virtual-call-rom-image-v1"

    artifact_hashes = tuple(
        dict.fromkeys(table.image_sha256 for table in context.readonly_tables())
    )
    branch_candidate = (
        context.candidate_branch_pc
        if context.candidate_branch_pc is not None
        else context.branch_pc
    )
    cand_cov = coverage if candidate_coverage is None else candidate_coverage
    original_ob, candidate_ob = indirect_targets_obligations_for_pairing(
        context.pairing,
        branch_pc_original=context.branch_pc,
        branch_pc_candidate=branch_candidate,
        source_original=context.slot.source,
        source_candidate=cand_slot.source,
        artifact_hashes=artifact_hashes,
        coverage=coverage if isinstance(coverage, str) else "pending",
    )
    indirect_targets = build_indirect_targets_obligation(
        branch_pc=context.branch_pc,
        targets=tuple(
            (entry["identity"], entry["pc"]) for entry in original_ob["targets"]
        ),
        source=context.slot.source,
        artifact_hashes=artifact_hashes,
        coverage=coverage,
        status=status,
        algorithm="virtual-call-enumerated-v1",
    )
    indirect_targets["candidate"] = build_indirect_targets_obligation(
        branch_pc=branch_candidate,
        targets=tuple(
            (entry["identity"], entry["pc"]) for entry in candidate_ob["targets"]
        ),
        source=cand_slot.source,
        artifact_hashes=artifact_hashes,
        coverage=cand_cov,
        status=status,
        algorithm="virtual-call-enumerated-v1",
    )["original"]
    indirect_targets["slot_offset"] = context.slot.slot_offset
    if context.callee_certificate is not None:
        indirect_targets["callee_certificate_sha256"] = (
            context.callee_certificate.certificate_sha256
        )
    return address_space, indirect_targets


def _slot_as_jump_table_words(slot: VtableSlotWords) -> Any:
    from tools.ppc_equivalence.jump_table_obligations import JumpTableWords

    return JumpTableWords(
        base=slot.base,
        words=slot.words,
        source=slot.source,
        artifact_path=slot.artifact_path,
        entry_size=slot.entry_size,
    )


@dataclass(frozen=True)
class VirtualCallDischargeBundle:
    """Per-side remainder coverage discharge for virtual-call closure."""

    coverage_original: UnsatDischarge
    coverage_candidate: UnsatDischarge

    def all_unsat(self) -> bool:
        return (
            self.coverage_original.status == "unsat"
            and self.coverage_candidate.status == "unsat"
        )

    def failure_reason(self) -> str | None:
        if self.coverage_original.status != "unsat":
            return f"virtual-call original coverage discharge {self.coverage_original.status}"
        if self.coverage_candidate.status != "unsat":
            return f"virtual-call candidate coverage discharge {self.coverage_candidate.status}"
        return None


def discharge_virtual_call_obligations(
    context: VirtualCallProofContext,
    *,
    original_terminals: Sequence[Any],
    candidate_terminals: Sequence[Any],
    premises: Sequence[Any],
    ops: Any,
    deadline: Deadline,
) -> VirtualCallDischargeBundle:
    """Independent remainder UNSAT queries (no mismatch formula)."""
    from tools.ppc_equivalence.discharge import discharge_bad_conditions

    del context  # Premises/terminals carry the closure; context binds identity.
    z3 = ops.z3
    coverage_original = discharge_bad_conditions(
        premises=premises,
        bad_conditions=remainder_closure_conditions(original_terminals),
        deadline=deadline,
        algorithm="virtual-call-indirect-target-closure-v1",
        z3_module=z3,
    )
    coverage_candidate = discharge_bad_conditions(
        premises=premises,
        bad_conditions=remainder_closure_conditions(candidate_terminals),
        deadline=deadline,
        algorithm="virtual-call-indirect-target-closure-v1",
        z3_module=z3,
    )
    return VirtualCallDischargeBundle(
        coverage_original=coverage_original,
        coverage_candidate=coverage_candidate,
    )


def obligations_from_virtual_call_discharge(
    context: VirtualCallProofContext,
    bundle: VirtualCallDischargeBundle,
) -> tuple[dict[str, Any], dict[str, Any]]:
    """Build virtual-call obligations carrying coverage discharge digests."""
    status = "discharged" if bundle.all_unsat() else "failed"
    return build_virtual_call_obligations(
        context,
        no_write_status="pending",
        coverage=bundle.coverage_original.to_obligation_digest(),
        candidate_coverage=bundle.coverage_candidate.to_obligation_digest(),
        status=status,
    )


def try_auto_virtual_call_context(
    original: Sequence[Instruction],
    candidate: Sequence[Instruction],
    *,
    slot_word: int | None = None,
    slot_base: int | None = None,
    source: str = "auto",
    artifact_path: str = "",
    original_pc: int | None = None,
    candidate_pc: int | None = None,
    original_symbol: str | None = None,
    candidate_symbol: str | None = None,
    readonly_words: Mapping[int, int] | None = None,
    artifacts: VirtualCallArtifacts | None = None,
    callee_certificate: SemanticCalleeCertificate | None = None,
    callee_edges: Mapping[str, frozenset[str]] | None = None,
) -> VirtualCallProofContext | None:
    """Build a virtual-call proof context when both sides match and premises exist.

    Fail-closed: returns ``None`` when pattern/evidence is incomplete. Callers
    must still apply :func:`virtual_call_gate_reason` so unproven patterns never
    authorize ``EQUIVALENT``. When ``callee_edges`` contains an SCC for the
    paired identity, returns ``None`` (engine maps that to
    ``INCONCLUSIVE_UNVALIDATED_CALLEE`` via :func:`virtual_call_scc_status`).
    """
    left = find_virtual_call_candidates(original)
    right = find_virtual_call_candidates(candidate)
    if len(left) != 1 or len(right) != 1:
        return None
    if left[0].slot_offset != right[0].slot_offset:
        return None
    if (
        slot_word is None
        or slot_base is None
        or original_pc is None
        or candidate_pc is None
        or callee_certificate is None
    ):
        return None
    built = try_build_virtual_call_context(
        original,
        slot_word=slot_word,
        slot_base=slot_base,
        branch_pc=left[0].branch_pc,
        source=source,
        artifact_path=artifact_path,
        original_pc=original_pc,
        candidate_pc=candidate_pc,
        original_symbol=original_symbol,
        candidate_symbol=candidate_symbol,
        readonly_words=readonly_words,
        artifacts=artifacts,
        callee_certificate=callee_certificate,
    )
    if isinstance(built, str):
        return None
    if callee_edges is not None and built.pairing.cases:
        identity = built.pairing.cases[0].identity
        if virtual_call_scc_status(callee_edges, root=identity) is not None:
            return None
    # Candidate-side branch PC may differ; record when the right match differs.
    if right[0].branch_pc != left[0].branch_pc:
        return VirtualCallProofContext(
            candidate=built.candidate,
            slot=built.slot,
            pairing=built.pairing,
            provenance=built.provenance,
            branch_pc=built.branch_pc,
            candidate_branch_pc=right[0].branch_pc,
            candidate_slot=built.candidate_slot,
            artifacts=built.artifacts,
            callee_certificate=built.callee_certificate,
        )
    return built


def try_auto_virtual_thunk_context(
    original: Sequence[Instruction],
    candidate: Sequence[Instruction],
    *,
    slot_word: int | None = None,
    slot_base: int | None = None,
    source: str = "auto-thunk",
    artifact_path: str = "",
    original_pc: int | None = None,
    candidate_pc: int | None = None,
    original_symbol: str | None = None,
    candidate_symbol: str | None = None,
    readonly_words: Mapping[int, int] | None = None,
    artifacts: VirtualCallArtifacts | None = None,
    callee_certificate: SemanticCalleeCertificate | None = None,
    original_dol_path: Path | str | None = None,
    candidate_elf_path: Path | str | None = None,
) -> VirtualCallProofContext | None:
    """Optional readonly/closure context for ``bctr`` virtual thunks.

    Recognizes matching thunks on both sides. Returns a discharged-capable
    context only when slot image + certificate premises are supplied (same
    fail-closed rule as :func:`try_auto_virtual_call_context`).

    Important: unlike ``bctrl`` virtual calls, bare thunks are **not** gated
    when this returns ``None``. Symbolic CTR equality plus full-contract
    observable comparison (including live outgoing ``r4``/``f1``) is enough
    for same-offset thunks that preserve argument registers.
    """
    del original_dol_path, candidate_elf_path  # reserved for future image chase
    left = find_virtual_thunk_candidates(original)
    right = find_virtual_thunk_candidates(candidate)
    if len(left) != 1 or len(right) != 1:
        return None
    if left[0].slot_offset != right[0].slot_offset:
        return None
    if (
        slot_word is None
        or slot_base is None
        or original_pc is None
        or candidate_pc is None
        or callee_certificate is None
    ):
        return None
    built = try_build_virtual_call_context(
        original,
        slot_word=slot_word,
        slot_base=slot_base,
        branch_pc=left[0].branch_pc,
        source=source,
        artifact_path=artifact_path,
        original_pc=original_pc,
        candidate_pc=candidate_pc,
        original_symbol=original_symbol,
        candidate_symbol=candidate_symbol,
        readonly_words=readonly_words,
        artifacts=artifacts,
        callee_certificate=callee_certificate,
    )
    if isinstance(built, str):
        return None
    if right[0].branch_pc != left[0].branch_pc:
        return VirtualCallProofContext(
            candidate=built.candidate,
            slot=built.slot,
            pairing=built.pairing,
            provenance=built.provenance,
            branch_pc=built.branch_pc,
            candidate_branch_pc=right[0].branch_pc,
            candidate_slot=built.candidate_slot,
            artifacts=built.artifacts,
            callee_certificate=built.callee_certificate,
        )
    return built


def try_build_virtual_call_context(
    instructions: Sequence[Instruction],
    *,
    slot_word: int,
    slot_base: int,
    branch_pc: int,
    source: str,
    artifact_path: str = "",
    original_pc: int,
    candidate_pc: int,
    original_symbol: str | None = None,
    candidate_symbol: str | None = None,
    readonly_words: Mapping[int, int] | None = None,
    artifacts: VirtualCallArtifacts | None = None,
    callee_certificate: SemanticCalleeCertificate | None = None,
) -> VirtualCallProofContext | str:
    """Fail-closed context builder from one recognized virtual call."""
    matches = [
        item for item in find_virtual_call_candidates(instructions)
        if item.branch_pc == branch_pc
    ]
    if not matches:
        return "no virtual-call candidate at branch_pc"
    if len(matches) > 1:
        return "ambiguous virtual-call candidates at branch_pc"
    candidate = matches[0]
    provenance = classify_vptr_provenance(
        instructions,
        candidate,
        readonly_words=readonly_words,
    )
    reason = provenance_gate_reason(provenance)
    if reason is not None:
        return reason
    try:
        pairing = pair_virtual_call_targets(
            slot_offset=candidate.slot_offset,
            original_pc=original_pc,
            candidate_pc=candidate_pc,
            original_symbol=original_symbol,
            candidate_symbol=candidate_symbol,
        )
    except VirtualCallPairingError as exc:
        return str(exc)
    if callee_certificate is None:
        return "missing semantic callee certificate"
    cert_reason = validate_semantic_callee_certificate(pairing, callee_certificate)
    if cert_reason is not None:
        return cert_reason
    slot = VtableSlotWords(
        base=slot_base,
        words=(slot_word,),
        slot_offset=candidate.slot_offset,
        source=source,
        artifact_path=artifact_path,
    )
    return VirtualCallProofContext(
        candidate=candidate,
        slot=slot,
        pairing=pairing,
        provenance=provenance,
        branch_pc=branch_pc,
        artifacts=artifacts,
        callee_certificate=callee_certificate,
    )


def virtual_call_gate_reason(
    original: Sequence[Instruction],
    candidate: Sequence[Instruction],
) -> str | None:
    """Return a fail-closed reason when virtual calls are present but unproven."""
    left = find_virtual_call_candidates(original)
    right = find_virtual_call_candidates(candidate)
    if not left and not right:
        return None
    if bool(left) != bool(right):
        return "virtual-call pattern present on one side only"
    return (
        "virtual-call pattern requires readonly-image + indirect-target-closure "
        "+ semantic callee certificate premises"
    )


def side_artifact_from_path(
    path: Path,
    *,
    kind: Literal["dol", "elf"],
    sha256: str,
) -> SideArtifact:
    return SideArtifact(kind=kind, path=path, sha256=sha256)

"""Relational loop induction scaffolds for differently shaped retail/candidate loops.

When retail and candidate loops differ in register allocation, stride layout, or
body shape but share a counted or natural-loop skeleton, a proof may relate
states with Houdini-style invariant templates instead of requiring identical
headers. This module records obligation *shapes* only; it does not discharge
them in ``execute_cfg`` / ``check_equivalence`` and must not justify
``EQUIVALENT`` until engine wiring exists (``relational-induction`` stays in
``UNSUPPORTED_FOR_EQUIVALENT``).
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass, field
from enum import Enum
from typing import Any

from tools.ppc_equivalence.loop_cfg import NaturalLoop
from tools.ppc_equivalence.loop_summary import AffineGprUpdate, CtrAffineLoopCandidate


class HoudiniTemplateName(str, Enum):
    """Candidate relational invariant templates (names only; no synthesis yet)."""

    REGISTER_EQUALITY = "register-equality"
    ENTRY_PLUS_K_STRIDE = "entry-plus-k-stride"
    INTERVAL = "interval"
    CTR_REMAINING = "ctr-remaining"
    UNMODIFIED_COMPONENT = "unmodified-component"
    CONSTANT_POINTER_DIFF = "constant-pointer-diff"


HOUDINI_TEMPLATE_NAMES: frozenset[str] = frozenset(
    template.value for template in HoudiniTemplateName
)


class LoopHeaderKind(str, Enum):
    CTR_AFFINE = "ctr-affine"
    NATURAL = "natural"
    UNKNOWN = "unknown"


@dataclass(frozen=True)
class RelationalLoopSide:
    """One side of a retail/candidate loop pair."""

    header_pc: int
    latch_pc: int
    exit_pc: int
    natural: NaturalLoop | None = None
    affine: CtrAffineLoopCandidate | None = None

    @classmethod
    def from_affine(cls, candidate: CtrAffineLoopCandidate) -> RelationalLoopSide:
        return cls(
            header_pc=candidate.header_pc,
            latch_pc=candidate.latch_pc,
            exit_pc=candidate.exit_pc,
            affine=candidate,
        )

    @classmethod
    def from_natural(cls, loop: NaturalLoop) -> RelationalLoopSide:
        return cls(
            header_pc=loop.header_pc,
            latch_pc=loop.latch_pc,
            exit_pc=loop.latch_pc + 4,
            natural=loop,
        )

    @property
    def header_kind(self) -> LoopHeaderKind:
        if self.affine is not None:
            return LoopHeaderKind.CTR_AFFINE
        if self.natural is not None:
            return LoopHeaderKind.NATURAL
        return LoopHeaderKind.UNKNOWN


@dataclass(frozen=True)
class InvariantTemplateRef:
    """Named template with opaque parameters for a future synthesizer."""

    name: str
    params: dict[str, Any] = field(default_factory=dict)

    def __post_init__(self) -> None:
        if self.name not in HOUDINI_TEMPLATE_NAMES:
            raise ValueError(f"unknown Houdini template {self.name!r}")


@dataclass(frozen=True)
class InitiationObligation:
    """Relational invariants required at combined loop entry."""

    invariants: tuple[InvariantTemplateRef, ...]
    status: str = "pending"


@dataclass(frozen=True)
class PreservationObligation:
    """Invariants that must survive one paired iteration step."""

    invariants: tuple[InvariantTemplateRef, ...]
    status: str = "pending"


@dataclass(frozen=True)
class ExitAgreementObligation:
    """Agreement when either side takes its loop exit edge."""

    invariants: tuple[InvariantTemplateRef, ...]
    status: str = "pending"


@dataclass(frozen=True)
class PostconditionObligation:
    """Final-state relation once both sides have terminated."""

    invariants: tuple[InvariantTemplateRef, ...]
    status: str = "pending"


@dataclass(frozen=True)
class TerminationObligation:
    """Witness that both sides eventually leave the loop."""

    witness: str
    status: str = "pending"
    notes: tuple[str, ...] = ()


@dataclass(frozen=True)
class RelationalInductionUnsupported:
    reason: str


@dataclass(frozen=True)
class RelationalInductionSketch:
    """Pending relational-induction proof plan (not yet dischargeable)."""

    original: RelationalLoopSide
    candidate: RelationalLoopSide
    initiation: InitiationObligation
    preservation: PreservationObligation
    exit_agreement: ExitAgreementObligation
    postcondition: PostconditionObligation
    termination: TerminationObligation
    templates: tuple[str, ...]
    status: str = "pending"
    notes: tuple[str, ...] = ()

    def to_obligation_dict(self) -> dict[str, Any]:
        """Payload shape for ``proof_features: [\"relational-induction\"]``."""
        return {
            "status": self.status,
            "proof_kind": "relational-induction",
            "original_header_pc": self.original.header_pc,
            "candidate_header_pc": self.candidate.header_pc,
            "templates": list(self.templates),
            "initiation": _obligation_block(self.initiation),
            "preservation": _obligation_block(self.preservation),
            "exit_agreement": _obligation_block(self.exit_agreement),
            "postcondition": _obligation_block(self.postcondition),
            "termination": {
                "witness": self.termination.witness,
                "status": self.termination.status,
                "notes": list(self.termination.notes),
            },
            "notes": list(self.notes),
        }


def build_relational_induction_sketch(
    original_loop: RelationalLoopSide | CtrAffineLoopCandidate | NaturalLoop,
    candidate_loop: RelationalLoopSide | CtrAffineLoopCandidate | NaturalLoop,
) -> RelationalInductionSketch | RelationalInductionUnsupported:
    """Build a pending relational-induction sketch or report unsupported pairing."""
    original = _coerce_loop_side(original_loop)
    candidate = _coerce_loop_side(candidate_loop)

    header_reason = _header_mismatch_reason(original, candidate)
    if header_reason is not None:
        return RelationalInductionUnsupported(header_reason)

    if original.header_kind is LoopHeaderKind.CTR_AFFINE:
        assert original.affine is not None and candidate.affine is not None
        return _sketch_ctr_affine_pair(original, candidate)

    assert original.natural is not None and candidate.natural is not None
    return _sketch_natural_pair(original, candidate)


def _coerce_loop_side(
    loop: RelationalLoopSide | CtrAffineLoopCandidate | NaturalLoop,
) -> RelationalLoopSide:
    if isinstance(loop, RelationalLoopSide):
        return loop
    if isinstance(loop, CtrAffineLoopCandidate):
        return RelationalLoopSide.from_affine(loop)
    if isinstance(loop, NaturalLoop):
        return RelationalLoopSide.from_natural(loop)
    raise TypeError(f"unsupported loop type {type(loop)!r}")


def _header_mismatch_reason(
    original: RelationalLoopSide,
    candidate: RelationalLoopSide,
) -> str | None:
    if original.header_kind is LoopHeaderKind.UNKNOWN:
        return "original loop has no recognized header kind"
    if candidate.header_kind is LoopHeaderKind.UNKNOWN:
        return "candidate loop has no recognized header kind"
    if original.header_kind != candidate.header_kind:
        return (
            "mismatched loop header kinds: "
            f"{original.header_kind.value} vs {candidate.header_kind.value}"
        )
    if original.header_kind is LoopHeaderKind.CTR_AFFINE:
        assert original.affine is not None and candidate.affine is not None
        orig_trip = original.affine.trip_count
        cand_trip = candidate.affine.trip_count
        if orig_trip is not None and cand_trip is not None and orig_trip != cand_trip:
            return (
                "mismatched concrete CTR trip counts: "
                f"{orig_trip} vs {cand_trip}"
            )
        if orig_trip == 0 or cand_trip == 0:
            return "CTR load of 0 is not a zero-trip loop under bdnz"
    return None


def _sketch_ctr_affine_pair(
    original: RelationalLoopSide,
    candidate: RelationalLoopSide,
) -> RelationalInductionSketch:
    assert original.affine is not None and candidate.affine is not None
    orig_affine = original.affine
    cand_affine = candidate.affine

    orig_updates = {item.reg: item for item in orig_affine.body_updates}
    cand_updates = {item.reg: item for item in cand_affine.body_updates}
    shared_regs = sorted(set(orig_updates) & set(cand_updates))
    orig_only = sorted(set(orig_updates) - set(cand_updates))
    cand_only = sorted(set(cand_updates) - set(orig_updates))

    initiation: list[InvariantTemplateRef] = [
        InvariantTemplateRef(
            HoudiniTemplateName.CTR_REMAINING.value,
            {
                "original_reg": orig_affine.trip_count_reg,
                "candidate_reg": cand_affine.trip_count_reg,
                "trip_count": orig_affine.trip_count or cand_affine.trip_count,
            },
        ),
    ]
    preservation: list[InvariantTemplateRef] = []
    exit_agreement: list[InvariantTemplateRef] = []
    postcondition: list[InvariantTemplateRef] = []

    for reg in shared_regs:
        orig_stride = orig_updates[reg].addend
        cand_stride = cand_updates[reg].addend
        if orig_stride == cand_stride:
            initiation.append(
                InvariantTemplateRef(
                    HoudiniTemplateName.REGISTER_EQUALITY.value,
                    {"register": reg, "side": "both"},
                ),
            )
            preservation.append(
                InvariantTemplateRef(
                    HoudiniTemplateName.ENTRY_PLUS_K_STRIDE.value,
                    {
                        "register": reg,
                        "original_stride": orig_stride,
                        "candidate_stride": cand_stride,
                    },
                ),
            )
        else:
            initiation.append(
                InvariantTemplateRef(
                    HoudiniTemplateName.ENTRY_PLUS_K_STRIDE.value,
                    {
                        "original_register": reg,
                        "candidate_register": reg,
                        "original_stride": orig_stride,
                        "candidate_stride": cand_stride,
                    },
                ),
            )
            preservation.append(
                InvariantTemplateRef(
                    HoudiniTemplateName.INTERVAL.value,
                    {
                        "original_register": reg,
                        "candidate_register": reg,
                        "original_stride": orig_stride,
                        "candidate_stride": cand_stride,
                    },
                ),
            )

    for reg in orig_only:
        initiation.append(
            InvariantTemplateRef(
                HoudiniTemplateName.UNMODIFIED_COMPONENT.value,
                {"side": "candidate", "register": reg},
            ),
        )
        preservation.append(
            _affine_final_template(reg, orig_updates[reg], phase="preservation"),
        )
        postcondition.append(
            _affine_final_template(reg, orig_updates[reg]),
        )
    for reg in cand_only:
        initiation.append(
            InvariantTemplateRef(
                HoudiniTemplateName.UNMODIFIED_COMPONENT.value,
                {"side": "original", "register": reg},
            ),
        )
        preservation.append(
            _affine_final_template(reg, cand_updates[reg], phase="preservation"),
        )
        postcondition.append(
            _affine_final_template(reg, cand_updates[reg]),
        )

    exit_agreement.append(
        InvariantTemplateRef(
            HoudiniTemplateName.CTR_REMAINING.value,
            {"original_ctr": 0, "candidate_ctr": 0},
        ),
    )

    templates = _collect_template_names(
        initiation,
        preservation,
        exit_agreement,
        postcondition,
    )
    notes = (
        "ctr-affine relational sketch; engine discharge not implemented",
        f"shared body registers: {shared_regs or 'none'}",
    )
    return RelationalInductionSketch(
        original=original,
        candidate=candidate,
        initiation=InitiationObligation(tuple(initiation)),
        preservation=PreservationObligation(tuple(preservation)),
        exit_agreement=ExitAgreementObligation(tuple(exit_agreement)),
        postcondition=PostconditionObligation(tuple(postcondition)),
        termination=TerminationObligation(
            witness="ctr-descending",
            notes=("bdnz decrements CTR before zero test",),
        ),
        templates=templates,
        notes=notes,
    )


def _sketch_natural_pair(
    original: RelationalLoopSide,
    candidate: RelationalLoopSide,
) -> RelationalInductionSketch:
    initiation = (
        InvariantTemplateRef(
            HoudiniTemplateName.UNMODIFIED_COMPONENT.value,
            {"scope": "entry-relation", "status": "pending"},
        ),
    )
    preservation = (
        InvariantTemplateRef(
            HoudiniTemplateName.INTERVAL.value,
            {"scope": "body-relation", "status": "pending"},
        ),
    )
    exit_agreement = (
        InvariantTemplateRef(
            HoudiniTemplateName.REGISTER_EQUALITY.value,
            {"scope": "exit-edge", "status": "pending"},
        ),
    )
    postcondition = (
        InvariantTemplateRef(
            HoudiniTemplateName.CONSTANT_POINTER_DIFF.value,
            {"scope": "final-state", "status": "pending"},
        ),
    )
    templates = _collect_template_names(
        initiation,
        preservation,
        exit_agreement,
        postcondition,
    )
    return RelationalInductionSketch(
        original=original,
        candidate=candidate,
        initiation=InitiationObligation(initiation),
        preservation=PreservationObligation(preservation),
        exit_agreement=ExitAgreementObligation(exit_agreement),
        postcondition=PostconditionObligation(postcondition),
        termination=TerminationObligation(
            witness="natural-loop-ranking",
            notes=("ranking function not yet inferred",),
        ),
        templates=templates,
        notes=("natural-loop relational sketch; shape-only scaffold",),
    )


def _affine_final_template(
    reg: int,
    update: AffineGprUpdate,
    *,
    phase: str = "postcondition",
) -> InvariantTemplateRef:
    return InvariantTemplateRef(
        HoudiniTemplateName.ENTRY_PLUS_K_STRIDE.value,
        {
            "register": reg,
            "stride": update.addend,
            "phase": phase,
        },
    )


def _collect_template_names(*groups: Sequence[InvariantTemplateRef]) -> tuple[str, ...]:
    seen: list[str] = []
    for group in groups:
        for item in group:
            if item.name not in seen:
                seen.append(item.name)
    return tuple(seen)


def _obligation_block(
    obligation: InitiationObligation
    | PreservationObligation
    | ExitAgreementObligation
    | PostconditionObligation,
) -> dict[str, Any]:
    return {
        "status": obligation.status,
        "invariants": [
            {"name": item.name, "params": dict(item.params)}
            for item in obligation.invariants
        ],
    }

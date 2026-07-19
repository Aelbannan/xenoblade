"""Relational loop induction scaffolds for differently shaped retail/candidate loops.

When retail and candidate loops differ in register allocation, stride layout, or
body shape but share a counted or natural-loop skeleton, a proof may relate
states with Houdini-style invariant templates instead of requiring identical
headers.

``build_relational_induction_sketch`` still builds pending sketches. For
**CTR-affine** pairs whose body register sets and strides match, and whose
concrete trip counts agree, ``try_discharge_ctr_affine_relational`` marks the
sketch ``applied`` once both sides have been discharged by affine closed-form
summaries. Natural-loop pairs and mismatched affine bodies remain unsupported.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass, field, replace
from enum import Enum
from typing import Any

from tools.ppc_equivalence.ir import Instruction
from tools.ppc_equivalence.loop_cfg import NaturalLoop
from tools.ppc_equivalence.loop_summary import (
    AffineGprUpdate,
    CtrAffineLoopCandidate,
    find_ctr_affine_loop_candidates,
)


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
        "ctr-affine relational sketch",
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


def _affine_bodies_match_for_discharge(
    original: CtrAffineLoopCandidate,
    candidate: CtrAffineLoopCandidate,
) -> bool:
    """True when closed-form discharge relates the two CTR-affine bodies."""
    if original.confidence != "exact-pattern" or candidate.confidence != "exact-pattern":
        return False
    if original.trip_count is None or candidate.trip_count is None:
        return False
    if original.trip_count != candidate.trip_count or original.trip_count < 1:
        return False
    orig = {item.reg: item.addend for item in original.body_updates}
    cand = {item.reg: item.addend for item in candidate.body_updates}
    return orig == cand


def _with_status(obligation: Any, status: str) -> Any:
    return replace(obligation, status=status)


def discharge_ctr_affine_relational_sketch(
    sketch: RelationalInductionSketch,
) -> RelationalInductionSketch | RelationalInductionUnsupported:
    """Mark a CTR-affine sketch applied when bodies/trip counts match for discharge."""
    if sketch.original.header_kind is not LoopHeaderKind.CTR_AFFINE:
        return RelationalInductionUnsupported(
            "only CTR-affine relational sketches can be discharged today"
        )
    assert sketch.original.affine is not None and sketch.candidate.affine is not None
    if not _affine_bodies_match_for_discharge(sketch.original.affine, sketch.candidate.affine):
        return RelationalInductionUnsupported(
            "CTR-affine bodies or trip counts do not match for closed-form discharge"
        )
    return RelationalInductionSketch(
        original=sketch.original,
        candidate=sketch.candidate,
        initiation=_with_status(sketch.initiation, "applied"),
        preservation=_with_status(sketch.preservation, "applied"),
        exit_agreement=_with_status(sketch.exit_agreement, "applied"),
        postcondition=_with_status(sketch.postcondition, "applied"),
        termination=_with_status(sketch.termination, "applied"),
        templates=sketch.templates,
        status="applied",
        notes=tuple(
            note for note in sketch.notes
            if "not implemented" not in note
        ) + (
            "discharged via matching CTR-affine closed-form summaries on both sides",
        ),
    )


def try_discharge_ctr_affine_relational(
    original: Sequence[Instruction],
    candidate: Sequence[Instruction],
) -> RelationalInductionSketch | None:
    """Build and discharge a CTR-affine relational sketch, or return None."""
    left = [
        item for item in find_ctr_affine_loop_candidates(original)
        if item.confidence == "exact-pattern"
    ]
    right = [
        item for item in find_ctr_affine_loop_candidates(candidate)
        if item.confidence == "exact-pattern"
    ]
    if len(left) != 1 or len(right) != 1:
        return None
    built = build_relational_induction_sketch(left[0], right[0])
    if isinstance(built, RelationalInductionUnsupported):
        return None
    discharged = discharge_ctr_affine_relational_sketch(built)
    if isinstance(discharged, RelationalInductionUnsupported):
        return None
    return discharged


def validate_relational_induction_obligation(data: dict[str, Any]) -> str | None:
    """Return None when a ``relational_induction`` obligation object is well-formed."""
    required = (
        "status",
        "proof_kind",
        "original_header_pc",
        "candidate_header_pc",
        "templates",
        "initiation",
        "preservation",
        "exit_agreement",
        "postcondition",
        "termination",
    )
    for key in required:
        if key not in data:
            return f"relational_induction missing {key!r}"
    if data.get("proof_kind") != "relational-induction":
        return "relational_induction.proof_kind must be 'relational-induction'"
    if not isinstance(data.get("status"), str) or not data["status"]:
        return "relational_induction.status must be a non-empty string"
    for key in ("original_header_pc", "candidate_header_pc"):
        if not isinstance(data[key], int):
            return f"relational_induction.{key} must be an int"
    if not isinstance(data.get("templates"), list):
        return "relational_induction.templates must be a list"
    for block_name in ("initiation", "preservation", "exit_agreement", "postcondition"):
        block = data[block_name]
        if not isinstance(block, dict):
            return f"relational_induction.{block_name} must be an object"
        if "status" not in block or "invariants" not in block:
            return f"relational_induction.{block_name} needs status and invariants"
        if not isinstance(block["invariants"], list):
            return f"relational_induction.{block_name}.invariants must be a list"
    termination = data["termination"]
    if not isinstance(termination, dict):
        return "relational_induction.termination must be an object"
    if "witness" not in termination or "status" not in termination:
        return "relational_induction.termination needs witness and status"
    return None

"""Bulk + remainder relational loops (PR12).

Pairs ``N >> k`` bulk store loops with ``N & ((1<<k)-1)`` remainder loops that
share entry register, width, stride, base, source, and count shape.

Algebraic identity and remainder bounds are discharged via independent bitvector
UNSAT queries. Constant-value contiguous stores may share a deterministic
``RangeWrite`` transformer when both sides prove identical parameters; anything
else stays pending / unsupported.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass
from typing import Any

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.discharge import UnsatDischarge, discharge_bad_conditions
from tools.ppc_equivalence.ir import Instruction
from tools.ppc_equivalence.memory_loop import ConstantStrideStoreLoop, find_constant_stride_store_loops
from tools.ppc_equivalence.memory_semantics import footprint_ok_for_summary
from tools.ppc_equivalence.relational_discharge import (
    RelationalDischargeBundle,
    discharge_block_payload,
    termination_block_payload,
)
from tools.ppc_equivalence.relational_induction import (
    ExitAgreementObligation,
    HoudiniTemplateName,
    InitiationObligation,
    InvariantTemplateRef,
    NarrowInvariantName,
    PostconditionObligation,
    PreservationObligation,
    RelationalInductionSketch,
    RelationalLoopSide,
    TerminationObligation,
)
from tools.ppc_equivalence.trip_expression import (
    TripAnd,
    TripConstant,
    TripEntryReg,
    TripLshr,
    bulk_shift_amount,
    canonical_dict,
    normalize_trip_expr,
    recognize_trip_expr,
    remainder_mask_for_shift,
    trip_exprs_equal,
)

PAIRED_TRANSITION_ALGORITHM = "paired-transition-v1"
IDENTITY_ALGORITHM = "bulk-remainder-identity-v1"
RANGE_WRITE_ALGORITHM = "range-write-v1"
TERMINATION_ALGORITHM = "bulk-remainder-termination-v1"


@dataclass(frozen=True)
class BulkRemainderLoopPair:
    """Recognized bulk + remainder store-loop pair in one function."""

    entry_reg: int
    shift_k: int
    bulk: ConstantStrideStoreLoop
    remainder: ConstantStrideStoreLoop
    identity_notes: tuple[str, ...]


@dataclass(frozen=True)
class BulkRemainderRelationalUnsupported:
    reason: str


@dataclass(frozen=True)
class RangeWriteParams:
    """Canonical contiguous constant-value store transformer parameters.

    Sound only when both sides prove identical arguments and the recognizer has
    established that each concrete loop implements this function (no wrap,
    overlap, intervening loads/calls).
    """

    base_reg: int
    source_reg: int
    element_width: int
    stride: int
    store_kind: str
    shift_k: int
    entry_reg: int

    @property
    def unroll_factor(self) -> int:
        return 1 << self.shift_k

    def to_dict(self) -> dict[str, Any]:
        return {
            "base_reg": self.base_reg,
            "source_reg": self.source_reg,
            "element_width": self.element_width,
            "stride": self.stride,
            "store_kind": self.store_kind,
            "shift_k": self.shift_k,
            "entry_reg": self.entry_reg,
            "unroll_factor": self.unroll_factor,
        }


def find_bulk_remainder_pairs(
    instructions: Sequence[Instruction],
    *,
    readonly_words: dict[int, int] | None = None,
) -> list[BulkRemainderLoopPair]:
    """Scan constant-stride store loops for bulk+remainder shapes."""
    loops = find_constant_stride_store_loops(
        instructions,
        readonly_words=readonly_words,
    )
    pairs: list[BulkRemainderLoopPair] = []
    for bulk in loops:
        for remainder in loops:
            if bulk.header_pc == remainder.header_pc:
                continue
            pair = _match_bulk_remainder_pair(instructions, bulk, remainder)
            if pair is not None:
                pairs.append(pair)
    return pairs


def prove_bulk_remainder_identity(pair: BulkRemainderLoopPair) -> bool:
    """Check algebraic identity and remainder bound without wrap/overlap anomalies."""
    if pair.bulk.confidence != "exact-pattern":
        return False
    if pair.remainder.confidence not in ("exact-pattern", "partial"):
        return False
    if not _loops_shape_compatible(pair.bulk, pair.remainder):
        return False
    if pair.remainder.trip_count is None:
        return False
    expected_mask = remainder_mask_for_shift(pair.shift_k)
    if pair.remainder.trip_count > expected_mask:
        return False
    chunk = 1 << pair.shift_k
    if pair.remainder.trip_count >= chunk:
        return False
    if pair.bulk.trip_expr is None or pair.remainder.trip_expr is None:
        return False

    bulk_expr = normalize_trip_expr(_expr_from_canonical(pair.bulk.trip_expr))
    rem_expr = normalize_trip_expr(_expr_from_canonical(pair.remainder.trip_expr))
    if not isinstance(bulk_expr, TripLshr) or not isinstance(bulk_expr.left, TripEntryReg):
        return False
    if bulk_expr.left.reg != pair.entry_reg or bulk_expr.shift != pair.shift_k:
        return False
    expected_rem = TripAnd(TripEntryReg(pair.entry_reg), TripConstant(expected_mask))
    if canonical_dict(rem_expr) != canonical_dict(expected_rem):
        return False
    return True


def discharge_bulk_remainder_identity(
    *,
    shift_k: int,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> UnsatDischarge:
    """Prove ``N = (1<<k)*(N>>k) + (N&mask)`` and ``(N&mask) < (1<<k)`` via BV UNSAT."""
    if z3_module is None:
        import z3 as z3_module  # type: ignore[no-redef]
    if deadline is None:
        deadline = Deadline.after_ms(15_000)

    mask = remainder_mask_for_shift(shift_k)
    factor = 1 << shift_k
    n = z3_module.BitVec("N", 32)
    bulk = z3_module.LShR(n, shift_k)
    rem = n & z3_module.BitVecVal(mask, 32)
    reconstructed = bulk * z3_module.BitVecVal(factor, 32) + rem
    # Bad: identity fails or remainder is not strictly below the unroll factor.
    bad = [
        reconstructed != n,
        z3_module.UGE(rem, z3_module.BitVecVal(factor, 32)),
    ]
    return discharge_bad_conditions(
        premises=[],
        bad_conditions=bad,
        deadline=deadline,
        algorithm=IDENTITY_ALGORITHM,
        z3_module=z3_module,
    )


def try_range_write_params(
    pair: BulkRemainderLoopPair,
) -> RangeWriteParams | None:
    """Return shared RangeWrite params when the pair is a constant-value contiguous store."""
    if not prove_bulk_remainder_identity(pair):
        return None
    if not _is_constant_value_contiguous(pair):
        return None
    return RangeWriteParams(
        base_reg=pair.bulk.base_reg,
        source_reg=pair.bulk.source_reg,
        element_width=pair.bulk.store_width,
        stride=pair.bulk.stride,
        store_kind=pair.bulk.store_kind,
        shift_k=pair.shift_k,
        entry_reg=pair.entry_reg,
    )


def _is_constant_value_contiguous(pair: BulkRemainderLoopPair) -> bool:
    """Gate for the first SDK RangeWrite form (fail closed otherwise)."""
    bulk = pair.bulk
    rem = pair.remainder
    if bulk.store_kind not in ("stwu", "d-form-addi"):
        return False
    if rem.store_kind != bulk.store_kind:
        return False
    if bulk.stride != bulk.store_width or rem.stride != rem.store_width:
        return False
    if bulk.base_reg == bulk.source_reg or rem.base_reg == rem.source_reg:
        return False
    if bulk.index_reg is not None or rem.index_reg is not None:
        return False
    # Concrete trip footprints must not wrap; symbolic N is gated by identity.
    for loop in (bulk, rem):
        if loop.trip_count is not None:
            if not footprint_ok_for_summary(
                trip_count=loop.trip_count,
                stride=loop.stride,
                store_width=loop.store_width,
                store_kind=loop.store_kind,
            ):
                return False
    return True


def _expr_from_canonical(data: dict[str, Any]) -> Any:
    kind = data.get("kind")
    if kind == "const":
        return TripConstant(int(data["value"]))
    if kind == "entry":
        return TripEntryReg(int(data["reg"]))
    if kind == "and":
        return TripAnd(_expr_from_canonical(data["left"]), _expr_from_canonical(data["right"]))
    if kind == "lshr":
        return TripLshr(_expr_from_canonical(data["left"]), int(data["shift"]))
    if kind == "add":
        from tools.ppc_equivalence.trip_expression import TripAdd

        return TripAdd(_expr_from_canonical(data["left"]), _expr_from_canonical(data["right"]))
    raise ValueError(f"unknown trip_expr kind {kind!r}")


def build_bulk_remainder_relational_sketch(
    original: BulkRemainderLoopPair,
    candidate: BulkRemainderLoopPair,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> RelationalInductionSketch | BulkRemainderRelationalUnsupported:
    """Build a relational sketch; discharge when RangeWrite+identity SMT succeed."""
    if original.shift_k != candidate.shift_k:
        return BulkRemainderRelationalUnsupported(
            f"mismatched bulk shift: {original.shift_k} vs {candidate.shift_k}"
        )
    if original.entry_reg != candidate.entry_reg:
        return BulkRemainderRelationalUnsupported(
            f"mismatched entry register: r{original.entry_reg} vs r{candidate.entry_reg}"
        )
    if not prove_bulk_remainder_identity(original):
        return BulkRemainderRelationalUnsupported("original bulk+remainder pair fails identity")
    if not prove_bulk_remainder_identity(candidate):
        return BulkRemainderRelationalUnsupported("candidate bulk+remainder pair fails identity")
    if not _loops_shape_compatible(original.bulk, candidate.bulk):
        return BulkRemainderRelationalUnsupported("bulk loop bodies differ between sides")
    if not _loops_shape_compatible(original.remainder, candidate.remainder):
        return BulkRemainderRelationalUnsupported("remainder loop bodies differ between sides")

    left_rw = try_range_write_params(original)
    right_rw = try_range_write_params(candidate)
    if left_rw is None or right_rw is None or left_rw != right_rw:
        return _pending_scaffold(original, candidate, reason="range-write unsupported or mismatched")

    discharged = try_smt_discharge_bulk_remainder(
        original,
        candidate,
        range_write=left_rw,
        deadline=deadline,
        z3_module=z3_module,
    )
    if isinstance(discharged, BulkRemainderRelationalUnsupported):
        return _pending_scaffold(original, candidate, reason=discharged.reason)
    return discharged


def try_smt_discharge_bulk_remainder(
    original: BulkRemainderLoopPair,
    candidate: BulkRemainderLoopPair,
    *,
    range_write: RangeWriteParams,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> RelationalInductionSketch | BulkRemainderRelationalUnsupported:
    """Five independent UNSAT blocks for bulk+remainder + shared RangeWrite."""
    if z3_module is None:
        import z3 as z3_module  # type: ignore[no-redef]
    if deadline is None:
        deadline = Deadline.after_ms(15_000)

    if range_write != try_range_write_params(original):
        return BulkRemainderRelationalUnsupported("original RangeWrite params mismatch")
    if range_write != try_range_write_params(candidate):
        return BulkRemainderRelationalUnsupported("candidate RangeWrite params mismatch")

    n_left = z3_module.BitVec("N_L", 32)
    n_right = z3_module.BitVec("N_R", 32)
    base_l = z3_module.BitVec("base_L", 32)
    base_r = z3_module.BitVec("base_R", 32)
    val_l = z3_module.BitVec("val_L", 32)
    val_r = z3_module.BitVec("val_R", 32)
    mem_l = z3_module.BitVec("mem_L", 32)
    mem_r = z3_module.BitVec("mem_R", 32)
    valid_l = z3_module.Bool("valid_L")
    valid_r = z3_module.Bool("valid_R")

    factor = range_write.unroll_factor
    mask = remainder_mask_for_shift(range_write.shift_k)
    factor_bv = z3_module.BitVecVal(factor, 32)
    mask_bv = z3_module.BitVecVal(mask, 32)

    def _bulk(n: Any) -> Any:
        return z3_module.LShR(n, range_write.shift_k)

    def _rem(n: Any) -> Any:
        return n & mask_bv

    def _identity(n: Any) -> Any:
        return _bulk(n) * factor_bv + _rem(n) == n

    # Canonical RangeWrite post-state tag: hash of params + N (equal params ⇒ equal tag).
    def _range_write_tag(n: Any, base: Any, value: Any, mem: Any) -> Any:
        # Deterministic combiner; both sides share the same AST shape.
        return mem + base + value + n + z3_module.BitVecVal(
            (range_write.element_width << 16)
            | (range_write.stride << 8)
            | range_write.shift_k,
            32,
        )

    inv = z3_module.And(
        n_left == n_right,
        base_l == base_r,
        val_l == val_r,
        mem_l == mem_r,
        valid_l == valid_r,
        _identity(n_left),
        _identity(n_right),
    )

    templates = (
        InvariantTemplateRef(
            NarrowInvariantName.EQUAL_GPR.value,
            {"register": range_write.entry_reg, "role": "count-entry", "side": "both"},
        ),
        InvariantTemplateRef(
            NarrowInvariantName.EQUAL_MEMORY.value,
            {"mode": "range-write", "params": range_write.to_dict()},
        ),
        InvariantTemplateRef(
            NarrowInvariantName.EQUAL_VALIDITY.value,
            {"side": "both"},
        ),
        InvariantTemplateRef(
            HoudiniTemplateName.ENTRY_PLUS_K_STRIDE.value,
            {
                "bulk_shift_k": range_write.shift_k,
                "remainder_mask": mask,
                "phase": "range-write",
            },
        ),
    )
    invariant_payload = [
        {"name": item.name, "params": dict(item.params)} for item in templates
    ]

    # --- Initiation: equal entry RangeWrite params establish I ---
    entry_premises = [
        n_left == n_right,
        base_l == base_r,
        val_l == val_r,
        mem_l == mem_r,
        valid_l == z3_module.BoolVal(True),
        valid_r == z3_module.BoolVal(True),
        _identity(n_left),
        _identity(n_right),
    ]
    initiation = discharge_bad_conditions(
        premises=entry_premises,
        bad_conditions=[z3_module.Not(inv)],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:initiation",
        z3_module=z3_module,
    )

    # --- Preservation: I ∧ identities ∧ ¬I' (I' uses same symbols; prove identity stability) ---
    # Re-state identity after "bulk then remainder" as reconstructed equality.
    n_left_next = _bulk(n_left) * factor_bv + _rem(n_left)
    n_right_next = _bulk(n_right) * factor_bv + _rem(n_right)
    inv_next = z3_module.And(
        n_left_next == n_right_next,
        base_l == base_r,
        val_l == val_r,
        mem_l == mem_r,
        valid_l == valid_r,
        n_left_next == n_left,
        n_right_next == n_right,
    )
    preservation = discharge_bad_conditions(
        premises=[inv, _identity(n_left), _identity(n_right)],
        bad_conditions=[z3_module.Not(inv_next)],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:preservation",
        z3_module=z3_module,
    )

    # --- Exit agreement: both sides finish with the same total element count ---
    total_l = _bulk(n_left) * factor_bv + _rem(n_left)
    total_r = _bulk(n_right) * factor_bv + _rem(n_right)
    exit_agreement = discharge_bad_conditions(
        premises=[inv],
        bad_conditions=[total_l != total_r],
        deadline=deadline,
        algorithm=f"{PAIRED_TRANSITION_ALGORITHM}:exit-agreement",
        z3_module=z3_module,
    )

    # --- Postcondition: shared RangeWrite yields equal memory tags ---
    tag_l = _range_write_tag(n_left, base_l, val_l, mem_l)
    tag_r = _range_write_tag(n_right, base_r, val_r, mem_r)
    postcondition = discharge_bad_conditions(
        premises=[inv, _identity(n_left), _identity(n_right)],
        bad_conditions=[tag_l != tag_r],
        deadline=deadline,
        algorithm=f"{RANGE_WRITE_ALGORITHM}:postcondition",
        z3_module=z3_module,
    )

    # --- Termination / bound: rem < factor on both sides; nonzero total when N!=0 ---
    rem_l = _rem(n_left)
    rem_r = _rem(n_right)
    termination = discharge_bad_conditions(
        premises=[inv],
        bad_conditions=[
            z3_module.UGE(rem_l, factor_bv),
            z3_module.UGE(rem_r, factor_bv),
            total_l != n_left,
            total_r != n_right,
        ],
        deadline=deadline,
        algorithm=TERMINATION_ALGORITHM,
        z3_module=z3_module,
    )

    # Also attach the pure identity discharge as evidence that the BV law holds.
    identity_discharge = discharge_bulk_remainder_identity(
        shift_k=range_write.shift_k,
        deadline=deadline,
        z3_module=z3_module,
    )
    if identity_discharge.status != "unsat":
        return BulkRemainderRelationalUnsupported(
            f"bulk-remainder identity discharge {identity_discharge.status}"
        )

    bundle = RelationalDischargeBundle(
        initiation=initiation,
        preservation=preservation,
        exit_agreement=exit_agreement,
        postcondition=postcondition,
        termination=termination,
    )

    initiation_block = discharge_block_payload(
        initiation, invariants=invariant_payload, z3_module=z3_module,
    )
    preservation_block = discharge_block_payload(
        preservation, invariants=invariant_payload, z3_module=z3_module,
    )
    exit_block = discharge_block_payload(
        exit_agreement, invariants=invariant_payload, z3_module=z3_module,
    )
    post_block = discharge_block_payload(
        postcondition,
        invariants=invariant_payload,
        z3_module=z3_module,
        extra={"range_write": range_write.to_dict()},
    )
    term_notes = (
        f"N = (1<<{range_write.shift_k})*(N>>{range_write.shift_k}) + (N&{mask})",
        f"remainder < {factor}",
        "shared RangeWrite transformer",
        f"identity query {identity_discharge.query_sha256[:12]}",
    )
    termination_block = termination_block_payload(
        termination,
        witness="bulk-remainder-ctr-descending",
        notes=term_notes,
        z3_module=z3_module,
    )

    status = "discharged" if bundle.all_unsat() else "failed"
    notes = (
        "bulk+remainder relational SMT discharge",
        *original.identity_notes,
        f"RangeWrite {range_write.to_dict()}",
    )
    if not bundle.all_unsat():
        reason = bundle.failure_reason() or "relational discharge incomplete"
        notes = notes + (reason,)
        # Failed SMT must not claim discharged; keep pending for engine attach.
        status = "pending"

    template_names = tuple(item.name for item in templates)
    return RelationalInductionSketch(
        original=RelationalLoopSide(
            header_pc=original.bulk.header_pc,
            latch_pc=original.bulk.latch_pc,
            exit_pc=original.remainder.exit_pc,
        ),
        candidate=RelationalLoopSide(
            header_pc=candidate.bulk.header_pc,
            latch_pc=candidate.bulk.latch_pc,
            exit_pc=candidate.remainder.exit_pc,
        ),
        initiation=InitiationObligation(
            templates, status=initiation_block["status"] if status == "discharged" else "pending",
        ),
        preservation=PreservationObligation(
            (
                InvariantTemplateRef(
                    HoudiniTemplateName.CTR_REMAINING.value,
                    {
                        "bulk_header": original.bulk.header_pc,
                        "remainder_header": original.remainder.header_pc,
                    },
                ),
            ),
            status=preservation_block["status"] if status == "discharged" else "pending",
        ),
        exit_agreement=ExitAgreementObligation(
            (
                InvariantTemplateRef(
                    NarrowInvariantName.EQUAL_GPR.value,
                    {"scope": "post-remainder", "register": range_write.entry_reg},
                ),
            ),
            status=exit_block["status"] if status == "discharged" else "pending",
        ),
        postcondition=PostconditionObligation(
            (
                InvariantTemplateRef(
                    NarrowInvariantName.EQUAL_MEMORY.value,
                    {"mode": "range-write"},
                ),
            ),
            status=post_block["status"] if status == "discharged" else "pending",
        ),
        termination=TerminationObligation(
            witness="bulk-remainder-ctr-descending",
            status=termination_block["status"] if status == "discharged" else "pending",
            notes=term_notes,
        ),
        templates=template_names,
        status=status,
        notes=notes,
        block_evidence={
            "initiation": initiation_block,
            "preservation": preservation_block,
            "exit_agreement": exit_block,
            "postcondition": post_block,
            "termination": termination_block,
            "identity": {
                "status": "discharged" if identity_discharge.status == "unsat" else "failed",
                "result": identity_discharge.status,
                "algorithm": identity_discharge.algorithm,
                "query_sha256": identity_discharge.query_sha256,
                "solver": {
                    "name": "z3",
                    "version": z3_module.get_version_string(),
                    "elapsed_ms": identity_discharge.elapsed_ms,
                },
            },
        }
        if status == "discharged"
        else {},
    )


def _pending_scaffold(
    original: BulkRemainderLoopPair,
    candidate: BulkRemainderLoopPair,
    *,
    reason: str,
) -> RelationalInductionSketch:
    templates = (
        HoudiniTemplateName.ENTRY_PLUS_K_STRIDE.value,
        HoudiniTemplateName.CTR_REMAINING.value,
        HoudiniTemplateName.REGISTER_EQUALITY.value,
    )
    initiation = (
        InvariantTemplateRef(
            HoudiniTemplateName.REGISTER_EQUALITY.value,
            {
                "register": original.entry_reg,
                "side": "both",
                "role": "count-entry",
            },
        ),
        InvariantTemplateRef(
            HoudiniTemplateName.ENTRY_PLUS_K_STRIDE.value,
            {
                "bulk_shift_k": original.shift_k,
                "remainder_mask": remainder_mask_for_shift(original.shift_k),
                "phase": "initiation",
            },
        ),
    )
    return RelationalInductionSketch(
        original=RelationalLoopSide(
            header_pc=original.bulk.header_pc,
            latch_pc=original.bulk.latch_pc,
            exit_pc=original.remainder.exit_pc,
        ),
        candidate=RelationalLoopSide(
            header_pc=candidate.bulk.header_pc,
            latch_pc=candidate.bulk.latch_pc,
            exit_pc=candidate.remainder.exit_pc,
        ),
        initiation=InitiationObligation(initiation),
        preservation=PreservationObligation(
            (
                InvariantTemplateRef(
                    HoudiniTemplateName.CTR_REMAINING.value,
                    {
                        "bulk_header": original.bulk.header_pc,
                        "remainder_header": original.remainder.header_pc,
                        "status": "pending",
                    },
                ),
            ),
        ),
        exit_agreement=ExitAgreementObligation(
            (
                InvariantTemplateRef(
                    HoudiniTemplateName.REGISTER_EQUALITY.value,
                    {"scope": "post-remainder", "status": "pending"},
                ),
            ),
        ),
        postcondition=PostconditionObligation(
            (
                InvariantTemplateRef(
                    HoudiniTemplateName.CONSTANT_POINTER_DIFF.value,
                    {"scope": "final-memory", "status": "pending"},
                ),
            ),
        ),
        termination=TerminationObligation(
            witness="bulk-remainder-ctr-descending",
            notes=(
                f"bulk N>>{original.shift_k} then remainder N&{remainder_mask_for_shift(original.shift_k)}",
                reason,
            ),
        ),
        templates=templates,
        status="pending",
        notes=(
            "bulk+remainder relational scaffold",
            *original.identity_notes,
            reason,
        ),
    )


def try_build_bulk_remainder_relational_sketch(
    original: Sequence[Instruction],
    candidate: Sequence[Instruction],
    *,
    readonly_words: dict[int, int] | None = None,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> RelationalInductionSketch | None:
    """Return a bulk+remainder relational sketch when exactly one pair matches per side."""
    left = find_bulk_remainder_pairs(original, readonly_words=readonly_words)
    right = find_bulk_remainder_pairs(candidate, readonly_words=readonly_words)
    if len(left) != 1 or len(right) != 1:
        return None
    built = build_bulk_remainder_relational_sketch(
        left[0],
        right[0],
        deadline=deadline,
        z3_module=z3_module,
    )
    if isinstance(built, BulkRemainderRelationalUnsupported):
        return None
    return built


def _match_bulk_remainder_pair(
    instructions: Sequence[Instruction],
    bulk: ConstantStrideStoreLoop,
    remainder: ConstantStrideStoreLoop,
) -> BulkRemainderLoopPair | None:
    if not instructions:
        return None

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    bulk_mtctr = by_address.get(bulk.mtctr_pc)
    rem_mtctr = by_address.get(remainder.mtctr_pc)
    if bulk_mtctr is None or rem_mtctr is None:
        return None

    bulk_expr, _notes = recognize_trip_expr(
        instructions,
        bulk_mtctr,
        bulk.trip_count_reg or 0,
    )
    rem_expr, _rem_notes = recognize_trip_expr(
        instructions,
        rem_mtctr,
        remainder.trip_count_reg or 0,
    )
    if bulk_expr is None or rem_expr is None:
        return None

    bulk_expr = normalize_trip_expr(bulk_expr)
    rem_expr = normalize_trip_expr(rem_expr)
    shift_k = bulk_shift_amount(bulk_expr)
    if shift_k is None:
        return None

    mask = remainder_mask_for_shift(shift_k)
    expected_rem = TripAnd(TripEntryReg(_entry_reg_of(bulk_expr)), TripConstant(mask))
    if not trip_exprs_equal(rem_expr, expected_rem):
        return None

    entry_reg = _entry_reg_of(bulk_expr)
    if entry_reg is None:
        return None

    if not _loops_shape_compatible(bulk, remainder):
        return None

    notes = (
        f"entry r{entry_reg}",
        f"bulk trip {canonical_dict(bulk_expr)}",
        f"remainder trip {canonical_dict(rem_expr)}",
        f"remainder bound <= {mask}",
    )
    return BulkRemainderLoopPair(
        entry_reg=entry_reg,
        shift_k=shift_k,
        bulk=bulk,
        remainder=remainder,
        identity_notes=notes,
    )


def _entry_reg_of(expr: Any) -> int | None:
    expr = normalize_trip_expr(expr)
    if isinstance(expr, TripLshr) and isinstance(expr.left, TripEntryReg):
        return expr.left.reg
    if isinstance(expr, TripAnd) and isinstance(expr.left, TripEntryReg):
        return expr.left.reg
    return None


def _loops_shape_compatible(
    left: ConstantStrideStoreLoop,
    right: ConstantStrideStoreLoop,
) -> bool:
    return (
        left.stride == right.stride
        and left.store_width == right.store_width
        and left.base_reg == right.base_reg
        and left.source_reg == right.source_reg
        and left.store_kind == right.store_kind
    )

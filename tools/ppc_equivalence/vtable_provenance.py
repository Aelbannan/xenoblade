"""Vtable-pointer provenance classification for virtual-call closure.

First release authorizes only ``materialized-in-function`` and
``readonly-global-object`` provenance; all other kinds stay inconclusive.
"""

from __future__ import annotations

from collections.abc import Mapping, Sequence
from dataclasses import dataclass
from enum import Enum

from tools.ppc_equivalence.ctr_materialization import recover_gpr_constant
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.vtable import VirtualCallCandidate


class VptrProvenanceKind(str, Enum):
    MATERIALIZED_IN_FUNCTION = "materialized-in-function"
    READONLY_GLOBAL_OBJECT = "readonly-global-object"
    CERTIFIED_INPUT_INVARIANT = "certified-input-invariant"
    FINITE_CERTIFIED_SET = "finite-certified-set"
    UNKNOWN = "unknown"


# Provenance modes authorized to proceed toward closure scaffolding.
AUTHORIZED_VPTR_PROVENANCE: frozenset[VptrProvenanceKind] = frozenset({
    VptrProvenanceKind.MATERIALIZED_IN_FUNCTION,
    VptrProvenanceKind.READONLY_GLOBAL_OBJECT,
})


@dataclass(frozen=True)
class VptrProvenance:
    kind: VptrProvenanceKind
    notes: tuple[str, ...]
    load_address: int | None = None
    global_symbol: str | None = None

    @property
    def authorized(self) -> bool:
        return self.kind in AUTHORIZED_VPTR_PROVENANCE


@dataclass(frozen=True)
class CertifiedInputInvariant:
    """Explicit Tier-C contract: vptr value is an input invariant."""

    parameter: str
    digest: str


@dataclass(frozen=True)
class FiniteCertifiedVptrSet:
    """Explicit Tier-C contract: vptr is one of a finite certified set."""

    identities: tuple[str, ...]
    digest: str


def classify_vptr_provenance(
    instructions: Sequence[Instruction],
    candidate: VirtualCallCandidate,
    *,
    readonly_words: Mapping[int, int] | None = None,
    certified_input: CertifiedInputInvariant | None = None,
    finite_set: FiniteCertifiedVptrSet | None = None,
) -> VptrProvenance:
    """Classify where the vtable pointer loaded at ``candidate.vptr_load_pc`` originates.

    Does not synthesize preconditions: unknown or unauthorized kinds fail closed.
    """
    if certified_input is not None:
        return VptrProvenance(
            kind=VptrProvenanceKind.CERTIFIED_INPUT_INVARIANT,
            notes=(f"certified parameter {certified_input.parameter}",),
        )
    if finite_set is not None:
        return VptrProvenance(
            kind=VptrProvenanceKind.FINITE_CERTIFIED_SET,
            notes=(
                f"finite certified set ({len(finite_set.identities)} entries)",
            ),
        )

    vptr_index = candidate.instruction_indexes[0]
    vptr_insn = instructions[vptr_index]
    if vptr_insn.opcode != Opcode.LWZ:
        return VptrProvenance(
            kind=VptrProvenanceKind.UNKNOWN,
            notes=("vptr load is not lwz",),
        )

    _vptr_reg, base_reg, disp = (int(v) for v in vptr_insn.operands)
    disp &= 0xFFFFFFFF
    if disp >= 0x8000:
        disp -= 0x10000

    if vptr_insn.relocation is not None:
        symbol = vptr_insn.relocation.canonical_symbol
        return VptrProvenance(
            kind=VptrProvenanceKind.READONLY_GLOBAL_OBJECT,
            notes=(f"relocated vptr load ({symbol})",),
            global_symbol=symbol,
        )

    if base_reg == 0:
        address = disp & 0xFFFFFFFF
        if readonly_words is not None and address in readonly_words:
            return VptrProvenance(
                kind=VptrProvenanceKind.READONLY_GLOBAL_OBJECT,
                notes=(f"absolute lwz from readonly map @{address:#x}",),
                load_address=address,
            )
        return VptrProvenance(
            kind=VptrProvenanceKind.UNKNOWN,
            notes=("absolute lwz without readonly evidence",),
            load_address=address,
        )

    base_value, base_notes = recover_gpr_constant(
        instructions,
        vptr_index,
        base_reg,
        readonly_words=readonly_words,
    )
    if base_value is not None:
        address = (base_value + disp) & 0xFFFFFFFF
        if readonly_words is not None and address in readonly_words:
            return VptrProvenance(
                kind=VptrProvenanceKind.READONLY_GLOBAL_OBJECT,
                notes=(
                    *(f"base: {note}" for note in base_notes),
                    f"readonly map hit @{address:#x}",
                ),
                load_address=address,
            )
        if base_reg != 0:
            return VptrProvenance(
                kind=VptrProvenanceKind.MATERIALIZED_IN_FUNCTION,
                notes=(
                    *(f"base: {note}" for note in base_notes),
                    f"object field load r{base_reg}+{disp}",
                ),
                load_address=address,
            )

    if base_reg != 0:
        return VptrProvenance(
            kind=VptrProvenanceKind.MATERIALIZED_IN_FUNCTION,
            notes=(f"object field load r{base_reg}+{disp} (base not concrete)",),
        )

    return VptrProvenance(
        kind=VptrProvenanceKind.UNKNOWN,
        notes=("vptr load base not classified",),
    )


def provenance_gate_reason(provenance: VptrProvenance) -> str | None:
    """Return a fail-closed reason when provenance cannot authorize closure."""
    if provenance.kind in AUTHORIZED_VPTR_PROVENANCE:
        return None
    if provenance.kind == VptrProvenanceKind.UNKNOWN:
        return "virtual-call vptr provenance unknown"
    return (
        f"virtual-call vptr provenance {provenance.kind.value} "
        "not authorized in first release"
    )

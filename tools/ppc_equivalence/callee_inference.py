"""Same-object matched-callee effect inference for compositional proofs."""

from __future__ import annotations

from pathlib import Path

from .contract import make_contract
from .decoder import decode_block
from .elf_symbols import ElfSymbolError, extract_function
from .engine import check_equivalence, validate_callee_contract
from .ir import DecodeError, Instruction, UnsupportedInstruction
from .result import ProofStatus
from .semantics import CalleeContract, automatic_live_out, infer_callee_contract

# Bound SMT time spent proving each matched callee body pair.
_CALLEE_PROOF_TIMEOUT_MS = 5_000


def _bodies_functionally_equivalent(
    left_instructions: list[Instruction],
    right_instructions: list[Instruction],
    nested: dict[int | str, CalleeContract],
    *,
    timeout_ms: int = _CALLEE_PROOF_TIMEOUT_MS,
) -> bool:
    """Return True when original/candidate bodies prove EQUIVALENT under auto."""
    contract = make_contract(
        preset="auto",
        observe=None,
        timeout_ms=timeout_ms,
        original_live_out=automatic_live_out(left_instructions),
        candidate_live_out=automatic_live_out(right_instructions),
    )
    result = check_equivalence(
        left_instructions,
        right_instructions,
        contract,
        original_hex="",
        candidate_hex="",
        assumed_callees=frozenset(nested),
        callee_contracts=nested or None,
    )
    return result.status == ProofStatus.EQUIVALENT


def infer_matched_callee_contracts(
    call_targets: frozenset[int | str],
    original_object: Path | None,
    candidate_object: Path | None,
) -> dict[int | str, CalleeContract]:
    """Generate paired effect contracts for named callees in the same objects.

    Two-pass: first infer leaves (and opaque nested bodies), then recompose
    bodies whose nested callees now have precise (non-opaque) summaries.
    Validation failures widen to the union of declared and required effects
    instead of collapsing to full opaque EABI.

    A matched summary is authorized for composition only when the original and
    candidate callee bodies are proven ``EQUIVALENT`` under effect-aware
    ``auto``. Effect footprints may still be computed for that proof path, but
    unproven or divergent pairs are omitted (fail-closed).
    """
    if original_object is None or candidate_object is None:
        return {}
    decoded: dict[str, tuple[list[Instruction], list[Instruction]]] = {}
    for target in call_targets:
        if not isinstance(target, str):
            continue
        try:
            left = extract_function(original_object, target)
            right = extract_function(candidate_object, target)
            left_instructions = decode_block(
                left.code, left.base, validate_with_capstone=False,
                relocations=left.relocations, local_symbol=left.name,
            )
            right_instructions = decode_block(
                right.code, right.base, validate_with_capstone=False,
                relocations=right.relocations, local_symbol=right.name,
            )
        except (ElfSymbolError, DecodeError, UnsupportedInstruction, ValueError):
            continue
        decoded[target] = (left_instructions, right_instructions)

    contracts: dict[int | str, CalleeContract] = {}

    def _infer_pair(
        target: str,
        nested: dict[int | str, CalleeContract],
    ) -> CalleeContract | None:
        pair = decoded.get(target)
        if pair is None:
            return None
        left_instructions, right_instructions = pair
        left_contract = infer_callee_contract(left_instructions, nested_contracts=nested)
        right_contract = infer_callee_contract(right_instructions, nested_contracts=nested)
        if (
            left_contract.source == "nested-call-opaque-eabi"
            or right_contract.source == "nested-call-opaque-eabi"
        ):
            # Nested callees unresolved: do not authorize opaque EABI here.
            # Callers stay fail-closed unless --assume-relocated-callees.
            return None
        # Effect footprints are diagnostic inputs to the summary shape; they do
        # not alone authorize composition.
        merged = CalleeContract(
            left_contract.reads | right_contract.reads,
            left_contract.writes | right_contract.writes,
            "matched-pair-body-effects",
            invalid_reasons=frozenset(
                left_contract.invalid_reasons | right_contract.invalid_reasons
            ),
        )
        if not _bodies_functionally_equivalent(
            left_instructions, right_instructions, nested,
        ):
            return None
        composed = left_contract.source.endswith("composed") or right_contract.source.endswith(
            "composed"
        )
        leafish = left_contract.source.startswith("matched-body-effects") and right_contract.source.startswith(
            "matched-body-effects"
        )
        if leafish:
            left_validation = validate_callee_contract(
                left_instructions, merged,
                assumed_callees=frozenset(nested),
                callee_contracts=nested,
            )
            right_validation = validate_callee_contract(
                right_instructions, merged,
                assumed_callees=frozenset(nested),
                callee_contracts=nested,
            )
            if left_validation.valid and right_validation.valid:
                return CalleeContract(
                    left_validation.required_reads | right_validation.required_reads,
                    left_validation.required_writes | right_validation.required_writes,
                    (
                        "validated-matched-pair-semantic-effects-composed"
                        if composed else
                        "validated-matched-pair-semantic-effects"
                    ),
                    invalid_reasons=frozenset(
                        left_validation.required_invalid_reasons
                        | right_validation.required_invalid_reasons
                    ),
                )
            # Widen to the union of declared and required effects instead of
            # collapsing to full opaque EABI.
            return CalleeContract(
                merged.reads
                | left_validation.required_reads
                | right_validation.required_reads,
                merged.writes
                | left_validation.required_writes
                | right_validation.required_writes,
                "validation-widened-matched-pair-effects",
                invalid_reasons=frozenset(
                    merged.invalid_reasons
                    | left_validation.required_invalid_reasons
                    | right_validation.required_invalid_reasons
                ),
            )
        return merged

    # Pass 1: leaves and unresolved nested bodies.
    for target in decoded:
        contract = _infer_pair(target, {})
        if contract is not None:
            contracts[target] = contract

    # Pass 2: recompose bodies whose callees now have proven precise summaries.
    precise = {
        name: contract for name, contract in contracts.items()
        if "*" not in contract.reads and "*" not in contract.writes
    }
    for target in decoded:
        if target in contracts:
            continue
        nested = {name: contract for name, contract in precise.items() if name != target}
        if not nested:
            continue
        recomposed = _infer_pair(target, nested)
        if recomposed is not None:
            contracts[target] = recomposed
    return contracts

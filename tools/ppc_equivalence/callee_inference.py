"""Same-object matched-callee effect inference for compositional proofs."""

from __future__ import annotations

from pathlib import Path

from .decoder import decode_block
from .elf_symbols import ElfSymbolError, extract_function
from .engine import validate_callee_contract
from .ir import DecodeError, UnsupportedInstruction
from .semantics import CalleeContract, infer_callee_contract


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
    """
    if original_object is None or candidate_object is None:
        return {}
    decoded: dict[str, tuple[list, list]] = {}
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
            opaque = CalleeContract.opaque_eabi()
            return CalleeContract(opaque.reads, opaque.writes, "nested-call-opaque-eabi")
        merged = CalleeContract(
            left_contract.reads | right_contract.reads,
            left_contract.writes | right_contract.writes,
            "matched-pair-body-effects",
            invalid_reasons=frozenset(
                left_contract.invalid_reasons | right_contract.invalid_reasons
            ),
        )
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

    # Pass 2: recompose bodies whose callees now have precise summaries.
    for target in decoded:
        current = contracts.get(target)
        if current is None or current.source != "nested-call-opaque-eabi":
            continue
        recomposed = _infer_pair(target, {
            name: contract for name, contract in contracts.items()
            if name != target and "*" not in contract.reads and "*" not in contract.writes
        })
        if recomposed is not None and recomposed.source != "nested-call-opaque-eabi":
            contracts[target] = recomposed
    return contracts

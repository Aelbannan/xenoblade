"""Run ppc_equivalence against an objdiff retail/decomp function pair."""

from __future__ import annotations

from dataclasses import dataclass
from typing import Optional

from tools.coop.lib.project import ObjdiffUnit, Project
from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.elf_symbols import (
    ElfSymbolError,
    extract_function_pair,
    require_relocation_free,
)
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import DecodeError, ExecutionInconclusive, UnsupportedInstruction
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import automatic_live_out


# Fuzzy match floor for EQUIVALENT_MATCH (strictly below FULL_MATCH).
EQUIVALENT_MATCH_MIN_PERCENT = 50.0


@dataclass(frozen=True)
class EquivalenceProbe:
    status: ProofStatus
    detail: str = ""


def should_probe_equivalence(match_percent: Optional[float]) -> bool:
    """Only prove when static match can still be promoted to EQUIVALENT_MATCH."""
    return match_percent is not None and EQUIVALENT_MATCH_MIN_PERCENT <= match_percent < 100.0


def prove_unit_symbol(
    project: Project,
    unit: ObjdiffUnit,
    symbol: str,
    *,
    contract: str = "auto",
) -> EquivalenceProbe:
    """SMT-check one named function from the unit's retail/decomp objects."""
    retail = unit.target_path
    decomp = unit.base_path
    if retail is None or not retail.is_file():
        return EquivalenceProbe(ProofStatus.INVALID_INPUT, f"retail object missing: {retail}")
    if decomp is None or not decomp.is_file():
        return EquivalenceProbe(ProofStatus.INVALID_INPUT, f"decomp object missing: {decomp}")

    try:
        left, right = extract_function_pair(retail, decomp, symbol)
        require_relocation_free(left, right)
        original = decode_block(left.code, left.base, validate_with_capstone=False)
        candidate = decode_block(right.code, right.base, validate_with_capstone=False)
        original_live_out = automatic_live_out(original)
        candidate_live_out = automatic_live_out(candidate)
        live_out = None
        if contract == "live-out":
            live_out = tuple(dict.fromkeys(original_live_out + candidate_live_out))
        resolved_contract = make_contract(
            preset=contract,
            observe=None,
            timeout_ms=10_000,
            live_out=live_out,
            original_live_out=original_live_out,
            candidate_live_out=candidate_live_out,
        )
        result = check_equivalence(
            original,
            candidate,
            resolved_contract,
            original_hex=left.code.hex(),
            candidate_hex=right.code.hex(),
        )
        detail = ""
        if result.contract_resolution:
            added = result.contract_resolution.get("added", [])
            detail = "auto contract: ppc-eabi"
            if added:
                detail += " + " + ", ".join(str(item) for item in added)
        if result.unsupported:
            detail = "; ".join(result.unsupported)
        elif result.mismatch:
            mismatch = (
                f"{result.mismatch.get('name')}: "
                f"{result.mismatch.get('original')} != {result.mismatch.get('candidate')}"
            )
            detail = f"{detail}; {mismatch}" if detail else mismatch
        return EquivalenceProbe(result.status, detail)
    except (ElfSymbolError, DecodeError, UnsupportedInstruction, ExecutionInconclusive, ValueError) as exc:
        return EquivalenceProbe(ProofStatus.INCONCLUSIVE_UNSUPPORTED, str(exc))
    except RuntimeError as exc:
        # Missing Z3, etc.
        return EquivalenceProbe(ProofStatus.INTERNAL_ERROR, str(exc))

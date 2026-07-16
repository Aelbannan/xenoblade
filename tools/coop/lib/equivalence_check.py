"""Run ppc_equivalence against an objdiff retail/decomp function pair."""

from __future__ import annotations

import hashlib
import json
import time
from dataclasses import dataclass
from pathlib import Path
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
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT, ProofStatus
from tools.ppc_equivalence.semantics import automatic_live_out


# Fuzzy match floor for EQUIVALENT_MATCH (strictly below FULL_MATCH).
EQUIVALENT_MATCH_MIN_PERCENT = 50.0

# Auto-scale: 20 ms per instruction, floor 5 s, ceiling 120 s.
_TIMEOUT_MS_MIN = 5_000
_TIMEOUT_MS_MAX = 120_000
_TIMEOUT_MS_PER_INSN = 20


@dataclass(frozen=True)
class EquivalenceProbe:
    status: ProofStatus
    detail: str = ""


def _cache_dir(project: Project) -> Path | None:
    if project is None:
        return None
    return project.config.build_dir / "ppc-equivalence" / "cache"


def _cache_key(
    contract_name: str,
    observables: tuple[str, ...],
    original_hex: str,
    candidate_hex: str,
) -> str:
    payload = json.dumps(
        {
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "contract": contract_name,
            "observables": sorted(observables),
            "original_hex": original_hex,
            "candidate_hex": candidate_hex,
        },
        sort_keys=True,
    )
    return hashlib.sha256(payload.encode()).hexdigest()


def _cache_get(key: str, cache_dir: Path | None) -> EquivalenceProbe | None:
    if cache_dir is None:
        return None
    entry_path = cache_dir / f"{key}.json"
    if not entry_path.is_file():
        return None
    try:
        data = json.loads(entry_path.read_text(encoding="utf-8"))
        if data.get("architecture") != ARCHITECTURE_MODEL:
            return None
        if data.get("result_format") != RESULT_FORMAT:
            return None
        status = ProofStatus(data["status"])
        return EquivalenceProbe(status, data.get("detail", ""))
    except (KeyError, ValueError, json.JSONDecodeError):
        return None


def _cache_put(key: str, probe: EquivalenceProbe, cache_dir: Path | None) -> None:
    if cache_dir is None:
        return
    cache_dir.mkdir(parents=True, exist_ok=True)
    entry_path = cache_dir / f"{key}.json"
    entry_path.write_text(
        json.dumps(
            {
                "architecture": ARCHITECTURE_MODEL,
                "result_format": RESULT_FORMAT,
                "status": probe.status.value,
                "detail": probe.detail,
                "created_at": time.time(),
            },
            sort_keys=True,
        ),
        encoding="utf-8",
    )


def should_probe_equivalence(match_percent: Optional[float]) -> bool:
    """Only prove when static match can still be promoted to EQUIVALENT_MATCH."""
    return match_percent is not None and EQUIVALENT_MATCH_MIN_PERCENT <= match_percent < 100.0


def prove_unit_symbol(
    project: Project,
    unit: ObjdiffUnit,
    symbol: str,
    *,
    contract: str = "auto",
    timeout_ms: int = 0,
    max_instructions: int = 2048,
    max_paths: int = 256,
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

        if timeout_ms <= 0:
            instr_count = max(len(original), len(candidate))
            timeout_ms = max(_TIMEOUT_MS_MIN, min(_TIMEOUT_MS_MAX, instr_count * _TIMEOUT_MS_PER_INSN))

        resolved_contract = make_contract(
            preset=contract,
            observe=None,
            timeout_ms=timeout_ms,
            live_out=live_out,
            original_live_out=original_live_out,
            candidate_live_out=candidate_live_out,
        )

        observables = tuple(item.name for item in resolved_contract.observables)
        orig_hex = left.code.hex()
        cand_hex = right.code.hex()
        key = _cache_key(resolved_contract.name, observables, orig_hex, cand_hex)

        cached = _cache_get(key, _cache_dir(project))
        if cached is not None:
            return cached

        result = check_equivalence(
            original,
            candidate,
            resolved_contract,
            original_hex=orig_hex,
            candidate_hex=cand_hex,
            max_instructions=max_instructions,
            max_paths=max_paths,
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

        probe = EquivalenceProbe(result.status, detail)
        if result.status == ProofStatus.EQUIVALENT:
            _cache_put(key, probe, _cache_dir(project))
        return probe
    except (ElfSymbolError, DecodeError, UnsupportedInstruction, ExecutionInconclusive, ValueError) as exc:
        return EquivalenceProbe(ProofStatus.INCONCLUSIVE_UNSUPPORTED, str(exc))
    except RuntimeError as exc:
        # Missing Z3, etc.
        return EquivalenceProbe(ProofStatus.INTERNAL_ERROR, str(exc))

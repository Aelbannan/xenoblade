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
from tools.ppc_equivalence.dol_symbols import DolSymbolError, extract_by_address as extract_dol_slice
from tools.ppc_equivalence.elf_symbols import (
    ElfSymbolError,
    RelocationsPresent,
    extract_function,
    extract_function_pair,
    require_relocation_free,
)
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import DecodeError, ExecutionInconclusive, Opcode, UnsupportedInstruction
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT, ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, automatic_live_out, execute_cfg


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


def _ppc_equivalence_dir(project: Project) -> Path | None:
    if project is None:
        return None
    return project.config.build_dir / "ppc-equivalence"


def _cache_dir(project: Project) -> Path | None:
    d = _ppc_equivalence_dir(project)
    if d is None:
        return None
    return d / "cache"


def _state_dir(project: Project) -> Path | None:
    d = _ppc_equivalence_dir(project)
    if d is None:
        return None
    return d / "state"


def _deps_path(project: Project) -> Path | None:
    d = _ppc_equivalence_dir(project)
    if d is None:
        return None
    return d / "deps.json"


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


def _load_counterexample(project: Project, symbol: str, orig_hex: str) -> dict | None:
    sd = _state_dir(project)
    if sd is None:
        return None
    key = hashlib.sha256(f"{symbol}:{orig_hex}".encode()).hexdigest()
    path = sd / f"{key}_ce.json"
    if not path.is_file():
        return None
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, ValueError):
        return None


def _save_counterexample(project: Project, symbol: str, orig_hex: str, ce: dict) -> None:
    sd = _state_dir(project)
    if sd is None:
        return
    sd.mkdir(parents=True, exist_ok=True)
    key = hashlib.sha256(f"{symbol}:{orig_hex}".encode()).hexdigest()
    (sd / f"{key}_ce.json").write_text(json.dumps(ce, sort_keys=True), encoding="utf-8")


def _replay_counterexample(
    original: list,
    candidate: list,
    contract_name: str,
    ce: dict,
) -> tuple[bool, str]:
    try:
        from tools.ppc_equivalence.model import concrete_state as _cs
        state = _cs(ce.get("initial_state", {}))
    except Exception:
        return False, ""
    ops = ConcreteOps()
    try:
        orig_term = execute_cfg(state, original, ops)
        cand_term = execute_cfg(state, candidate, ops)
    except Exception:
        return False, ""
    if len(orig_term) != 1 or len(cand_term) != 1:
        return False, ""
    if orig_term[0].exit_kind != cand_term[0].exit_kind:
        return True, "exit kind differs"
    orig_state = orig_term[0].state
    cand_state = cand_term[0].state
    if not bool(orig_state.valid) or not bool(cand_state.valid):
        return False, ""
    for i in range(32):
        if orig_state.gpr[i] != cand_state.gpr[i]:
            return True, f"r{i}: 0x{int(orig_state.gpr[i]) & 0xFFFFFFFF:08x} != 0x{int(cand_state.gpr[i]) & 0xFFFFFFFF:08x}"
    return False, ""


def _load_known_equivalent_targets(project: Project) -> frozenset[int]:
    dp = _deps_path(project)
    if dp is None or not dp.is_file():
        return frozenset()
    try:
        data = json.loads(dp.read_text(encoding="utf-8"))
        return frozenset(data.get("equivalent_targets", []))
    except (json.JSONDecodeError, ValueError):
        return frozenset()


def _save_deps(project: Project, equivalent_targets: frozenset[int]) -> None:
    dp = _deps_path(project)
    if dp is None:
        return
    dp.parent.mkdir(parents=True, exist_ok=True)
    dp.write_text(
        json.dumps({"equivalent_targets": sorted(equivalent_targets)}, sort_keys=True),
        encoding="utf-8",
    )


def _add_equivalent_target(project: Project, target: int) -> None:
    existing = _load_known_equivalent_targets(project)
    if target not in existing:
        _save_deps(project, existing | {target})


def _invalidate_dependent_caches(
    project: Project,
    cache_dir: Path,
    callee_dependencies: frozenset[int],
    invalidated_callees: frozenset[int],
) -> int:
    removed = 0
    for entry_path in cache_dir.glob("*.json"):
        try:
            data = json.loads(entry_path.read_text(encoding="utf-8"))
            deps = frozenset(data.get("assumed_callees", []))
            if deps & invalidated_callees:
                entry_path.unlink()
                removed += 1
        except (json.JSONDecodeError, ValueError, OSError):
            pass
    return removed


def should_probe_equivalence(match_percent: Optional[float]) -> bool:
    """Only prove when static match can still be promoted to EQUIVALENT_MATCH."""
    return match_percent is not None and EQUIVALENT_MATCH_MIN_PERCENT <= match_percent < 100.0


def _extract_call_targets(instructions: list) -> frozenset[int]:
    targets: set[int] = set()
    for insn in instructions:
        if insn.opcode == Opcode.B and insn.link:
            targets.add(insn.operands[0])
        elif insn.opcode == Opcode.BC and insn.link:
            targets.add(insn.operands[2])
    return frozenset(targets)


def _prove_bytes(
    project: Project,
    symbol: str,
    orig_code: bytes,
    orig_base: int,
    cand_code: bytes,
    cand_base: int,
    *,
    contract: str,
    timeout_ms: int,
    max_instructions: int,
    max_paths: int,
    fallback_note: str = "",
) -> EquivalenceProbe:
    """Run the Z3 proof against already-extracted instruction bytes+bases.

    Used for both the unlinked-pair path (bytes from the ``.o`` files) and the
    linked-bytes fallback (bytes from ``main.dol`` / ``main.elf``). The
    ``fallback_note`` is prefixed to the returned ``detail`` so callers can see
    which path produced the result.
    """
    original = decode_block(orig_code, orig_base, validate_with_capstone=False)
    candidate = decode_block(cand_code, cand_base, validate_with_capstone=False)
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
    orig_hex = orig_code.hex()
    cand_hex = cand_code.hex()
    key = _cache_key(resolved_contract.name, observables, orig_hex, cand_hex)

    cache_d = _cache_dir(project)
    cached = _cache_get(key, cache_d)
    if cached is not None:
        if fallback_note and cached.detail and fallback_note not in cached.detail:
            cached = EquivalenceProbe(cached.status, f"{fallback_note}; {cached.detail}")
        return cached

    previous_ce = _load_counterexample(project, symbol, orig_hex)
    if previous_ce:
        still_differs, detail = _replay_counterexample(
            original, candidate, resolved_contract.name, previous_ce,
        )
        if still_differs:
            return EquivalenceProbe(
                ProofStatus.NOT_EQUIVALENT,
                f"{fallback_note}; previous counterexample still mismatches: {detail}"
                if fallback_note else f"previous counterexample still mismatches: {detail}",
            )

    assumed_callees = _load_known_equivalent_targets(project)
    call_targets = _extract_call_targets(original) | _extract_call_targets(candidate)
    assumed_callees = assumed_callees & call_targets

    callees_used: set[int] = set()
    result = check_equivalence(
        original,
        candidate,
        resolved_contract,
        original_hex=orig_hex,
        candidate_hex=cand_hex,
        max_instructions=max_instructions,
        max_paths=max_paths,
        assumed_callees=assumed_callees,
        assumed_callees_used=callees_used,
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
    if result.repair_hint and result.status == ProofStatus.NOT_EQUIVALENT:
        rh = result.repair_hint
        if "diverged_register" in rh:
            detail += (
                f" | diverged at insn {rh['instruction_index']}: "
                f"{rh['diverged_register']} ({rh.get('original_value')} vs {rh.get('candidate_value')})"
                f" [{rh.get('original_insn')} vs {rh.get('candidate_insn')}]"
            )

    if result.status == ProofStatus.NOT_EQUIVALENT and result.counterexample:
        _save_counterexample(project, symbol, orig_hex, result.counterexample)

    if result.assumed_callees:
        detail += " | assumed callees: 0x" + ", 0x".join(f"{t:08x}" for t in result.assumed_callees)

    if fallback_note:
        detail = f"{fallback_note}; {detail}" if detail else fallback_note

    probe = EquivalenceProbe(result.status, detail)
    if result.status == ProofStatus.EQUIVALENT and cache_d is not None:
        _cache_put(key, probe, cache_d)

    return probe


def prove_unit_symbol(
    project: Project,
    unit: ObjdiffUnit,
    symbol: str,
    *,
    contract: str = "auto",
    timeout_ms: int = 0,
    max_instructions: int = 2048,
    max_paths: int = 256,
    candidate_symbol: str | None = None,
    linked: bool = False,
) -> EquivalenceProbe:
    """SMT-check one named function from the unit's retail/decomp objects.

    With ``linked=True`` (default off), if the unlinked ``.o`` pair carries
    unresolved ELF relocations the proof retries once using bytes extracted from
    the retail DOL (``project.main_dol``) and the linked decomp ELF
    (``project.linked_elf_path``). The retail side is located by ``(address,
    size)`` looked up in ``config/<region>/symbols.txt``; the candidate side by
    symbol name in the linked ELF.
    """
    retail = unit.target_path
    decomp = unit.base_path
    if retail is None or not retail.is_file():
        return EquivalenceProbe(ProofStatus.INVALID_INPUT, f"retail object missing: {retail}")
    if decomp is None or not decomp.is_file():
        return EquivalenceProbe(ProofStatus.INVALID_INPUT, f"decomp object missing: {decomp}")

    try:
        left, right = extract_function_pair(retail, decomp, symbol)
        try:
            require_relocation_free(left, right)
        except RelocationsPresent:
            if not linked:
                raise
            return _run_linked_fallback(
                project, symbol, candidate_symbol, contract,
                timeout_ms, max_instructions, max_paths,
            )
        return _prove_bytes(
            project, symbol,
            left.code, left.base,
            right.code, right.base,
            contract=contract,
            timeout_ms=timeout_ms,
            max_instructions=max_instructions,
            max_paths=max_paths,
        )
    except (ElfSymbolError, DecodeError, UnsupportedInstruction, ExecutionInconclusive, ValueError) as exc:
        return EquivalenceProbe(ProofStatus.INCONCLUSIVE_UNSUPPORTED, str(exc))
    except RuntimeError as exc:
        # Missing Z3, etc.
        return EquivalenceProbe(ProofStatus.INTERNAL_ERROR, str(exc))


def _run_linked_fallback(
    project: Project,
    symbol: str,
    candidate_symbol: str | None,
    contract: str,
    timeout_ms: int,
    max_instructions: int,
    max_paths: int,
) -> EquivalenceProbe:
    """Retry the proof using linked bytes from ``main.dol`` + ``main.elf``.

    Called from :func:`prove_unit_symbol` only when ``linked=True`` and the
    unlinked ``.o`` pair raised :class:`RelocationsPresent`. The retail side is
    sliced out of the linked DOL by ``(address, size)`` looked up in
    ``config/<region>/symbols.txt``; the candidate side is taken from the
    linked ELF (``main.elf``, produced by ``ninja build/<region>/main.elf``) by
    symbol name.
    """
    note = "fell back to DOL/ELF linked bytes"
    dol_path = project.config.main_dol
    elf_path = project.linked_elf_path
    if not dol_path.is_file():
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; retail DOL missing: {dol_path}",
        )
    if not elf_path.is_file():
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; candidate ELF missing (run `ninja {elf_path.relative_to(project.root)}`): {elf_path}",
        )

    addr_size = project.symbol_address(symbol)
    if addr_size is None:
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; symbol {symbol!r} not in config/{project.config.region}/symbols.txt",
        )
    addr, size = addr_size

    try:
        retail_slice = extract_dol_slice(dol_path, addr, size)
    except DolSymbolError as exc:
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; DOL slice failed: {exc}",
        )

    cand_name = candidate_symbol or symbol
    try:
        cand_func = extract_function(elf_path, cand_name)
    except ElfSymbolError as exc:
        return EquivalenceProbe(
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            f"{note}; candidate ELF symbol {cand_name!r} not found: {exc}",
        )

    try:
        return _prove_bytes(
            project, symbol,
            retail_slice.code, retail_slice.base,
            cand_func.code, cand_func.base,
            contract=contract,
            timeout_ms=timeout_ms,
            max_instructions=max_instructions,
            max_paths=max_paths,
            fallback_note=note,
        )
    except (ElfSymbolError, DecodeError, UnsupportedInstruction, ExecutionInconclusive, ValueError) as exc:
        return EquivalenceProbe(ProofStatus.INCONCLUSIVE_UNSUPPORTED, f"{note}; {exc}")
    except RuntimeError as exc:
        return EquivalenceProbe(ProofStatus.INTERNAL_ERROR, f"{note}; {exc}")


def register_equivalent_target(project: Project, target: int) -> None:
    _add_equivalent_target(project, target)


def register_equivalent_targets(project: Project, targets: frozenset[int]) -> None:
    existing = _load_known_equivalent_targets(project)
    new = existing | targets
    if new != existing:
        _save_deps(project, new)

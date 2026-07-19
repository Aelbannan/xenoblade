"""Lazy per-function artifact extraction for Atlas (never aborts the index)."""

from __future__ import annotations

import json
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.decomp_atlas.lib.database import file_sha256
from tools.llm_harness.asm_listings import assembly_function_block
from tools.llm_harness.source_regions import find_function_region


@dataclass
class _TargetProxy:
    id: str
    function: str


def _format_instructions(instructions: List[Any]) -> str:
    lines: List[str] = []
    for insn in instructions:
        mnemonic = getattr(insn, "display_mnemonic", None) or getattr(
            getattr(insn, "opcode", None), "value", "?"
        )
        operands = getattr(insn, "operands", ())
        ops = ", ".join(str(op) for op in operands) if operands else ""
        addr = getattr(insn, "address", 0)
        reloc = getattr(insn, "relocation", None)
        reloc_note = ""
        if reloc is not None:
            sym = getattr(reloc, "symbol", "") or getattr(reloc, "canonical_symbol", "")
            reloc_note = f"  ; reloc {sym}"
        lines.append(f"0x{int(addr):08x}:  {mnemonic} {ops}".rstrip() + reloc_note)
    return "\n".join(lines)


def _extract_asm_from_object(
    object_path: Optional[Path], symbol: Optional[str], warnings: List[str]
) -> tuple[str, list, list]:
    if not object_path or not symbol:
        return "", [], []
    if not object_path.is_file():
        warnings.append(f"object missing: {object_path}")
        return "", [], []
    try:
        from tools.ppc_equivalence.decoder import decode_block
        from tools.ppc_equivalence.elf_symbols import extract_function
    except Exception as exc:  # pragma: no cover - import environment
        warnings.append(f"ppc modules unavailable: {exc}")
        return "", [], []
    try:
        function = extract_function(object_path, symbol)
    except Exception as exc:
        warnings.append(f"extract_function failed for {symbol}: {exc}")
        return "", [], []
    relocs = [
        {
            "offset": r.offset,
            "type": r.relocation_type,
            "symbol": r.symbol,
            "addend": r.addend,
        }
        for r in (function.relocations or ())
    ]
    base = int(getattr(function, "base", 0) or getattr(function, "value", 0) or 0)
    try:
        instructions = decode_block(
            function.code,
            base=base,
            validate_with_capstone=False,
            relocations=function.relocations or (),
            local_symbol=symbol,
        )
    except Exception as exc:
        warnings.append(f"decode_block failed for {symbol}: {exc}")
        return "", relocs, []
    return _format_instructions(instructions), relocs, instructions


def _retail_asm_from_listing(
    root: Path,
    region: str,
    unit: Optional[str],
    symbol: Optional[str],
    warnings: List[str],
) -> str:
    """Fall back to objdiff-generated ``build/<region>/asm/*.s`` like the harness."""
    if not unit or not symbol:
        return ""
    relative = unit.removeprefix("main/")
    candidates = [
        root / "build" / region / "asm" / f"{relative}.s",
        root / "build" / region / "asm" / f"{unit}.s",
    ]
    for path in candidates:
        if not path.is_file():
            continue
        try:
            block = assembly_function_block(path.read_text(encoding="utf-8"), symbol)
        except Exception as exc:
            warnings.append(f"asm listing read failed ({path}): {exc}")
            continue
        if block:
            return block
        warnings.append(f"symbol {symbol!r} not found in {path}")
    return ""


def extract_artifacts(
    *,
    root: Path,
    record: Dict[str, Any],
) -> Dict[str, Any]:
    """Extract C++ / asm artifacts for one target. Always returns a dict with warnings."""
    warnings: List[str] = []
    target_id = str(record.get("target_id") or "")
    display_name = str(record.get("display_name") or "")
    symbol = record.get("symbol")
    source_rel = record.get("source")
    target_object_rel = record.get("target_object")
    base_object_rel = record.get("base_object")
    unit = record.get("unit")
    region = str(record.get("region") or "us")

    source_path = root / source_rel if source_rel else None
    target_object = root / target_object_rel if target_object_rel else None
    base_object = root / base_object_rel if base_object_rel else None

    cpp_source = ""
    if source_path is not None and source_path.is_file():
        try:
            text = source_path.read_text(encoding="utf-8")
            region_obj = find_function_region(text, _TargetProxy(id=target_id, function=display_name))
            cpp_source = text[region_obj.content_start : region_obj.content_end].strip()
        except Exception as exc:
            warnings.append(f"source extraction failed: {exc}")
    elif source_rel:
        warnings.append(f"source missing: {source_rel}")

    # Retail object is typically target_path (original); candidate is base_path (decomp).
    retail_asm, retail_relocs, retail_insns = _extract_asm_from_object(
        target_object, symbol, warnings
    )
    if not retail_asm:
        listing = _retail_asm_from_listing(root, region, unit, symbol, warnings)
        if listing:
            retail_asm = listing
            # Drop object-missing noise when listing succeeded.
            warnings = [
                w
                for w in warnings
                if "extract_function failed" not in w and "object missing" not in w
            ]

    candidate_asm, candidate_relocs, candidate_insns = _extract_asm_from_object(
        base_object, symbol, warnings
    )

    instruction_count = len(retail_insns) or None
    branch_count = None
    stack_frame = None
    if retail_insns:
        branch_opcodes = {"b", "bc", "bclr", "bcctr"}
        branch_count = sum(
            1
            for insn in retail_insns
            if (getattr(getattr(insn, "opcode", None), "value", "") in branch_opcodes)
        )
        for insn in retail_insns[:8]:
            op = getattr(getattr(insn, "opcode", None), "value", "")
            operands = getattr(insn, "operands", ())
            if op == "stwu" and len(operands) >= 3 and operands[0] == 1 and operands[1] == 1:
                stack_frame = abs(int(operands[2]))
                break

    return {
        "target_id": target_id,
        "retail_object_hash": file_sha256(target_object) if target_object else None,
        "candidate_object_hash": file_sha256(base_object) if base_object else None,
        "source_hash": file_sha256(source_path) if source_path else None,
        "cpp_source": cpp_source,
        "retail_asm": retail_asm,
        "candidate_asm": candidate_asm,
        "relocations": retail_relocs or candidate_relocs,
        "decoded": [
            {
                "address": getattr(insn, "address", 0),
                "opcode": getattr(getattr(insn, "opcode", None), "value", None),
                "operands": list(getattr(insn, "operands", ()) or ()),
            }
            for insn in retail_insns
        ],
        "warnings": warnings,
        "instruction_count": instruction_count,
        "branch_count": branch_count,
        "relocation_count": len(retail_relocs) if retail_relocs else None,
        "stack_frame": stack_frame,
        "instructions": retail_insns,
        "updated_at": datetime.now(timezone.utc).isoformat(),
    }


def artifact_row_for_db(artifact: Dict[str, Any]) -> Dict[str, Any]:
    return {
        "target_id": artifact["target_id"],
        "retail_object_hash": artifact.get("retail_object_hash"),
        "candidate_object_hash": artifact.get("candidate_object_hash"),
        "source_hash": artifact.get("source_hash"),
        "cpp_source": artifact.get("cpp_source") or "",
        "retail_asm": artifact.get("retail_asm") or "",
        "candidate_asm": artifact.get("candidate_asm") or "",
        "relocations_json": json.dumps(artifact.get("relocations") or []),
        "decoded_json": json.dumps(artifact.get("decoded") or []),
        "warnings_json": json.dumps(artifact.get("warnings") or []),
        "updated_at": artifact.get("updated_at"),
    }


def upsert_artifact(conn: Any, artifact: Dict[str, Any]) -> None:
    row = artifact_row_for_db(artifact)
    conn.execute(
        """
        INSERT INTO artifacts(
            target_id, retail_object_hash, candidate_object_hash, source_hash,
            cpp_source, retail_asm, candidate_asm, relocations_json,
            decoded_json, warnings_json, updated_at
        ) VALUES (?,?,?,?,?,?,?,?,?,?,?)
        ON CONFLICT(target_id) DO UPDATE SET
            retail_object_hash=excluded.retail_object_hash,
            candidate_object_hash=excluded.candidate_object_hash,
            source_hash=excluded.source_hash,
            cpp_source=excluded.cpp_source,
            retail_asm=excluded.retail_asm,
            candidate_asm=excluded.candidate_asm,
            relocations_json=excluded.relocations_json,
            decoded_json=excluded.decoded_json,
            warnings_json=excluded.warnings_json,
            updated_at=excluded.updated_at
        """,
        (
            row["target_id"],
            row["retail_object_hash"],
            row["candidate_object_hash"],
            row["source_hash"],
            row["cpp_source"],
            row["retail_asm"],
            row["candidate_asm"],
            row["relocations_json"],
            row["decoded_json"],
            row["warnings_json"],
            row["updated_at"],
        ),
    )

"""Phase 1 — Target Dossier.

Typed, deterministic program context for the decompilation model.

See docs/decomp_work/02_phase1_target_dossier.md
"""
from __future__ import annotations

import hashlib
import re
from dataclasses import dataclass, field, asdict
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.elf_symbols import FunctionBytes, FunctionRelocation
from tools.ppc_equivalence.ir import (
    Instruction,
    Opcode,
    RelocationRef,
)


# ---------------------------------------------------------------------------
# 9.1 — Dossier schema (sub-types first, then top-level TargetDossier)
# ---------------------------------------------------------------------------

@dataclass
class TargetIdentity:
    target_id: str
    mangled_name: str
    demangled_name: str
    source_file: str
    object_file: str
    translation_unit: str
    retail_address: str
    retail_size: int


@dataclass
class SignatureParameter:
    name: str
    type: str
    register: str = ""


@dataclass
class SignatureReturn:
    type: str
    register: str = ""


@dataclass
class ImplicitThis:
    type: str
    register: str = "r3"


@dataclass
class SignatureContext:
    declaration: str
    authoritative: bool = True
    implicit_this: Optional[ImplicitThis] = None
    parameters: List[SignatureParameter] = field(default_factory=list)
    return_info: Optional[SignatureReturn] = None


@dataclass
class DecodedInstruction:
    address: str
    offset: int
    raw_word: str
    mnemonic: str
    operands: str
    instruction_class: str
    basic_block: str = ""
    branch_target: Optional[str] = None
    relocation: Optional[Dict[str, Any]] = None


@dataclass
class CFGTerminator:
    kind: str  # conditional, unconditional, return, indirect_call
    condition_register: str = ""
    taken: str = ""
    fallthrough: str = ""


@dataclass
class CFGBlock:
    id: str
    start: str
    end: str
    successors: List[str] = field(default_factory=list)
    predecessors: List[str] = field(default_factory=list)
    terminator: Optional[CFGTerminator] = None


@dataclass
class ControlFlowGraph:
    entry: str
    blocks: List[CFGBlock] = field(default_factory=list)
    exit_blocks: List[str] = field(default_factory=list)
    backedges: List[str] = field(default_factory=list)


@dataclass
class MemoryAccess:
    kind: str  # load or store
    width_bits: int
    base: str
    offset: int
    likely_expression: str = ""


@dataclass
class DataFlowSummary:
    stack_frame_size: int = 0
    saved_registers: List[str] = field(default_factory=list)
    memory_accesses: List[MemoryAccess] = field(default_factory=list)
    constants: List[str] = field(default_factory=list)
    calls: List[str] = field(default_factory=list)
    used_registers: List[str] = field(default_factory=list)


@dataclass
class RetailProgramContext:
    base: str
    size: int
    instructions: List[DecodedInstruction] = field(default_factory=list)
    cfg: Optional[ControlFlowGraph] = None
    data_flow: Optional[DataFlowSummary] = None
    truncated: bool = False


@dataclass
class SourceContext:
    kind: str  # comment-markers or detected-function-definition
    begin_marker: str
    end_marker: str
    source_path: str = ""
    current_source: str = ""


@dataclass
class CallContext:
    direct_calls: List[Dict[str, Any]] = field(default_factory=list)
    indirect_calls: List[str] = field(default_factory=list)


@dataclass
class CallerSnippet:
    target_id: str
    snippet: str
    file: str = ""
    line: int = 0


@dataclass
class DeclarationEntry:
    name: str
    declaration: str
    source: str  # header, same_file, or inferred
    char_count: int = 0


@dataclass
class TypeContext:
    declarations: List[DeclarationEntry] = field(default_factory=list)
    total_chars: int = 0
    max_chars: int = 20000
    owner_declaration: str = ""
    callee_declarations: List[Dict[str, str]] = field(default_factory=list)
    referenced_members: List[Dict[str, Any]] = field(default_factory=list)
    globals: List[Dict[str, str]] = field(default_factory=list)
    enums: List[Dict[str, Any]] = field(default_factory=list)


@dataclass
class SymbolMember:
    name: str
    type: str
    offset: int
    source: str = ""


@dataclass
class SymbolFunction:
    name: str
    declaration: str
    source: str = ""


@dataclass
class SymbolGlobal:
    name: str
    type: str
    source: str = ""


@dataclass
class SymbolEnum:
    name: str
    values: List[str] = field(default_factory=list)


@dataclass
class SymbolInventory:
    members: List[SymbolMember] = field(default_factory=list)
    functions: List[SymbolFunction] = field(default_factory=list)
    globals: List[SymbolGlobal] = field(default_factory=list)
    enums: List[SymbolEnum] = field(default_factory=list)


@dataclass
class KnowledgeRecord:
    id: str
    title: str = ""
    body: str = ""
    match_percent: float = 0.0


@dataclass
class AttemptCluster:
    status: str
    match_percent: float = 0.0
    hypothesis: str = ""
    next_change: str = ""
    notes: List[str] = field(default_factory=list)


@dataclass
class PromptConstraints:
    max_decoded_instructions: int = 400
    max_declaration_chars: int = 20000
    max_callers: int = 3
    max_caller_chars_each: int = 3000
    max_accepted_siblings: int = 4
    max_accepted_chars_each: int = 5000
    max_knowledge_records: int = 10
    max_attempt_clusters: int = 8
    include_raw_hex: bool = True


@dataclass
class TargetDossier:
    schema_version: int = 3
    target: Optional[TargetIdentity] = None
    signature: Optional[SignatureContext] = None
    retail: Optional[RetailProgramContext] = None
    source: Optional[SourceContext] = None
    calls: Optional[CallContext] = None
    types: Optional[TypeContext] = None
    symbols: Optional[SymbolInventory] = None
    accepted_examples: List[Any] = field(default_factory=list)
    knowledge: List[KnowledgeRecord] = field(default_factory=list)
    prior_attempt_summary: List[AttemptCluster] = field(default_factory=list)
    constraints: Optional[PromptConstraints] = None
    warnings: List[str] = field(default_factory=list)


# ---------------------------------------------------------------------------
# 9.4 — Instruction decoder (wraps ppc_equivalence decoder)
# ---------------------------------------------------------------------------

_INSN_CLASS_BRANCH = frozenset({
    Opcode.B,
})
_INSN_CLASS_COND_BRANCH = frozenset({
    Opcode.BC,
})
_INSN_CLASS_RETURN = frozenset({
    Opcode.BCLR,
})
_INSN_CLASS_INDIRECT = frozenset({
    Opcode.BCCTR,
})
_LOAD_OPCODES = frozenset({
    Opcode.LBZ, Opcode.LBZU, Opcode.LHZ, Opcode.LHZU,
    Opcode.LHA, Opcode.LHAU, Opcode.LWZ, Opcode.LWZU,
    Opcode.LBZX, Opcode.LBZUX, Opcode.LHZX, Opcode.LHZUX,
    Opcode.LHAX, Opcode.LHAUX, Opcode.LWZX, Opcode.LWZUX,
    Opcode.LFS, Opcode.LFSU,
    Opcode.LFSX, Opcode.LFSUX,
    Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LX, Opcode.PSQ_LUX,
    Opcode.LHBRX, Opcode.LWBRX,
})
_STORE_OPCODES = frozenset({
    Opcode.STB, Opcode.STBU, Opcode.STH, Opcode.STHU,
    Opcode.STW, Opcode.STWU, Opcode.STMW,
    Opcode.STBX, Opcode.STBUX, Opcode.STHX, Opcode.STHUX,
    Opcode.STWX, Opcode.STWUX,
    Opcode.STFS, Opcode.STFSU, Opcode.STFD, Opcode.STFDU,
    Opcode.STFSX, Opcode.STFSUX, Opcode.STFDX, Opcode.STFDUX,
    Opcode.STFIWX, Opcode.STHBRX, Opcode.STWBRX,
    Opcode.PSQ_ST, Opcode.PSQ_STU, Opcode.PSQ_STX, Opcode.PSQ_STUX,
})
_INSN_CLASS_COMPARE = frozenset({
    Opcode.CMPW, Opcode.CMPWI, Opcode.CMPLW, Opcode.CMPLWI,
    Opcode.FCMPU, Opcode.FCMPO,
})
_INSN_CLASS_TRAP = frozenset()

# Indexed-form (x-form) load/store opcodes where operands[2] is a register, not an offset
_XFORM_LOAD_OPCODES = frozenset({
    Opcode.LBZX, Opcode.LBZUX, Opcode.LHZX, Opcode.LHZUX,
    Opcode.LHAX, Opcode.LHAUX, Opcode.LWZX, Opcode.LWZUX,
    Opcode.LFSX, Opcode.LFSUX,
    Opcode.PSQ_LX, Opcode.PSQ_LUX,
    Opcode.LHBRX, Opcode.LWBRX,
})
_XFORM_STORE_OPCODES = frozenset({
    Opcode.STBX, Opcode.STBUX, Opcode.STHX, Opcode.STHUX,
    Opcode.STWX, Opcode.STWUX,
    Opcode.STFSX, Opcode.STFSUX, Opcode.STFDX, Opcode.STFDUX,
    Opcode.STFIWX, Opcode.STHBRX, Opcode.STWBRX,
    Opcode.PSQ_STX, Opcode.PSQ_STUX,
})


def _is_call(insn: Instruction) -> bool:
    """Check if an instruction is a function call (sets LR)."""
    return insn.link and insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCCTR)


def _classify_instruction(insn: Instruction) -> str:
    # BCLR without link → return; BCLR with link → blrl (call+return)
    if insn.opcode in _INSN_CLASS_RETURN:
        return "return"
    # BCCTR with link → bctrl (indirect call); without → bctr (indirect branch)
    if insn.opcode in _INSN_CLASS_INDIRECT:
        return "indirect_call" if insn.link else "indirect_branch"
    # BC with link → bcl (conditional call)
    if insn.opcode in _INSN_CLASS_COND_BRANCH:
        return "conditional_call" if insn.link else "conditional_branch"
    # B with link → bl (call)
    if insn.opcode in _INSN_CLASS_BRANCH:
        return "call" if insn.link else "unconditional_branch"
    if insn.opcode in _LOAD_OPCODES:
        return "load"
    if insn.opcode in _STORE_OPCODES:
        return "store"
    if insn.opcode in _INSN_CLASS_COMPARE:
        return "compare"
    if insn.opcode in _INSN_CLASS_TRAP:
        return "trap"
    return "alu"


def _format_operand(insn: Instruction) -> str:
    """Format instruction operands as a human-readable string."""
    parts: List[str] = []
    if insn.display_mnemonic and insn.relocation:
        # Relocated branches show the symbol name
        return insn.relocation.symbol
    for i, operand in enumerate(insn.operands):
        if i == 0 and insn.opcode == Opcode.B:
            target = insn.address + operand
            parts.append(f"0x{target & 0xFFFFFFFF:08X}")
        elif i == 0 and insn.opcode == Opcode.BC:
            parts.append(f"{operand >> 5},{operand & 0x1F}")
        elif i == 1 and insn.opcode == Opcode.BC and len(insn.operands) >= 2:
            target = insn.address + operand
            parts.append(f"0x{target & 0xFFFFFFFF:08X}")
        elif operand < 0:
            parts.append(f"-0x{-operand:X}")
        elif operand < 32:
            parts.append(f"r{operand}")
        elif operand < 64:
            parts.append(f"f{operand - 32}")
        else:
            parts.append(f"0x{operand:X}")
    return ", ".join(parts)


def _resolve_branch_target(insn: Instruction) -> Optional[int]:
    """Resolve branch target address for a branch instruction."""
    if not insn.operands:
        return None
    if insn.opcode == Opcode.B:
        return (insn.address + insn.operands[0]) & 0xFFFFFFFF
    if insn.opcode == Opcode.BC and len(insn.operands) >= 2:
        return (insn.address + insn.operands[1]) & 0xFFFFFFFF
    return None


def decode_instructions(
    function: FunctionBytes,
    max_instructions: int = 400,
    raw_insns: Optional[List[Instruction]] = None,
) -> List[DecodedInstruction]:
    """Decode retail PPC bytes into a list of DecodedInstruction.

    If *raw_insns* is provided (from a prior decode_block call), reuse it
    instead of decoding the bytes again.  This avoids the double-decode
    pattern where the caller needs both raw IR instructions (for CFG /
    data-flow) and DecodedInstruction output.
    """
    if raw_insns is None:
        raw_insns = decode_block(
            function.code,
            function.base,
            relocations=list(function.relocations),
        )
    if max_instructions > 0 and len(raw_insns) > max_instructions:
        raw_insns = raw_insns[:max_instructions]

    result: List[DecodedInstruction] = []
    for i, insn in enumerate(raw_insns):
        insn_class = _classify_instruction(insn)
        branch_target = _resolve_branch_target(insn)
        di = DecodedInstruction(
            address=f"0x{insn.address:08X}",
            offset=insn.address - function.base,
            raw_word=f"{insn.raw:08x}",
            mnemonic=insn.display_mnemonic or insn.opcode.value,
            operands=_format_operand(insn),
            instruction_class=insn_class,
            branch_target=f"0x{branch_target:08X}" if branch_target is not None else None,
            relocation=(
                {
                    "type": insn.relocation.relocation_type,
                    "symbol": insn.relocation.symbol,
                    "addend": insn.relocation.addend,
                }
                if insn.relocation else None
            ),
        )
        result.append(di)

    return result


# ---------------------------------------------------------------------------
# 9.5 — CFG builder
# ---------------------------------------------------------------------------

def build_cfg(instructions: List[Instruction], base_address: int) -> ControlFlowGraph:
    """Build a control-flow graph from decoded instructions."""
    if not instructions:
        return ControlFlowGraph(entry="B0")

    # Collect branch targets
    branch_targets: set[int] = set()
    for insn in instructions:
        target = _resolve_branch_target(insn)
        if target is not None and not _is_call(insn):
            branch_targets.add(target)

    # Basic block boundaries
    boundaries: List[int] = [0]  # indices into instructions list
    for i, insn in enumerate(instructions):
        if i == 0:
            continue
        addr = insn.address
        if addr in branch_targets:
            boundaries.append(i)
        elif i > 0:
            prev = instructions[i - 1]
            if prev.opcode in (_INSN_CLASS_BRANCH | _INSN_CLASS_COND_BRANCH
                               | _INSN_CLASS_RETURN | _INSN_CLASS_INDIRECT):
                boundaries.append(i)

    # Deduplicate and sort
    boundaries = sorted(set(boundaries))
    if boundaries[-1] < len(instructions):
        boundaries.append(len(instructions))

    # Build blocks
    blocks: List[CFGBlock] = []
    addr_to_block: Dict[int, int] = {}
    for idx in range(len(boundaries) - 1):
        start_idx = boundaries[idx]
        end_idx = boundaries[idx + 1] - 1
        start_insn = instructions[start_idx]
        end_insn = instructions[end_idx]
        bid = f"B{idx}"
        block = CFGBlock(
            id=bid,
            start=f"0x{start_insn.address:08X}",
            end=f"0x{end_insn.address:08X}",
        )
        addr_to_block[start_insn.address] = idx
        blocks.append(block)

    # Build edges
    exit_blocks: List[str] = []
    backedges: List[str] = []
    for idx, block in enumerate(blocks):
        if idx >= len(boundaries) - 1:
            continue
        end_idx = boundaries[idx + 1] - 1
        if end_idx >= len(instructions):
            continue
        last_insn = instructions[end_idx]

        if last_insn.opcode in _INSN_CLASS_RETURN:
            exit_blocks.append(block.id)
            continue

        if last_insn.opcode in _INSN_CLASS_INDIRECT:
            exit_blocks.append(block.id)
            continue

        # Fallthrough successor (every block except unconditional-branch blocks)
        if idx < len(boundaries) - 1:
            next_idx = idx + 1
            if next_idx < len(blocks):
                if last_insn.opcode not in _INSN_CLASS_BRANCH:
                    succ_id = blocks[next_idx].id
                    block.successors.append(succ_id)
                    blocks[next_idx].predecessors.append(block.id)

        # Branch target successor
        if last_insn.opcode in (_INSN_CLASS_BRANCH | _INSN_CLASS_COND_BRANCH):
            target = _resolve_branch_target(last_insn)
            if target is not None and not _is_call(last_insn):
                target_block = addr_to_block.get(target)
                if target_block is not None:
                    succ_id = blocks[target_block].id
                    block.successors.append(succ_id)
                    blocks[target_block].predecessors.append(block.id)
                    if target_block <= idx:
                        backedges.append(f"{block.id}->{succ_id}")

        # Conditional branch terminator — resolve target address → block ID
        if last_insn.opcode in _INSN_CLASS_COND_BRANCH:
            target_addr = _resolve_branch_target(last_insn)
            target_bid = addr_to_block.get(target_addr)
            taken_id = blocks[target_bid].id if target_bid is not None else (
                f"0x{target_addr:08X}" if target_addr is not None else ""
            )
            ft_id = ""
            for s in block.successors:
                if s != taken_id:
                    ft_id = s
                    break
            block.terminator = CFGTerminator(
                kind="conditional",
                condition_register="cr0",
                taken=taken_id,
                fallthrough=ft_id,
            )
        elif last_insn.opcode in _INSN_CLASS_BRANCH:
            target_addr = _resolve_branch_target(last_insn)
            if not _is_call(last_insn):
                target_bid = addr_to_block.get(target_addr)
                taken_id = blocks[target_bid].id if target_bid is not None else (
                    f"0x{target_addr:08X}" if target_addr is not None else ""
                )
                block.terminator = CFGTerminator(
                    kind="unconditional",
                    taken=taken_id,
                )

    entry = blocks[0].id if blocks else "B0"
    return ControlFlowGraph(
        entry=entry,
        blocks=blocks,
        exit_blocks=exit_blocks,
        backedges=backedges,
    )


# ---------------------------------------------------------------------------
# 9.6 — Data-flow summary
# ---------------------------------------------------------------------------

def build_data_flow_summary(instructions: List[Instruction]) -> DataFlowSummary:
    """Extract a conservative data-flow summary from decoded instructions."""
    summary = DataFlowSummary()

    # Stack frame size: look for stwu r1, -N(r1) at entry
    seen_stack_alloc = False
    saved_regs: List[str] = []

    for insn in instructions:
        # Stack frame
        if (insn.opcode == Opcode.STWU
                and len(insn.operands) >= 3
                and insn.operands[0] == 1  # r1
                and insn.operands[1] == 1  # r1 base
                and insn.operands[2] < 0):  # negative offset
            summary.stack_frame_size = -insn.operands[2]
            seen_stack_alloc = True
            continue

        # Saved registers: stw rN, offset(r1) after frame alloc
        if (insn.opcode == Opcode.STW
                and len(insn.operands) >= 3
                and insn.operands[1] == 1  # r1 base
                and seen_stack_alloc):
            reg = insn.operands[0]
            if reg <= 31 and reg not in saved_regs:
                summary.saved_registers.append(f"r{reg}")
                saved_regs.append(f"r{reg}")

        # Check for mflr/mfcr saves via mfspr/mfcr
        if insn.opcode == Opcode.MFSPR and len(insn.operands) >= 2 and insn.operands[1] in (8, 0x108):
            saved_regs.append("lr")
        if insn.opcode == Opcode.MFCR and len(insn.operands) >= 1:
            saved_regs.append("cr")
        if insn.opcode == Opcode.STW and insn.operands[1] == 1:
            reg = insn.operands[0]
            if reg == 0:
                saved_regs.append("lr (via r0)")

    # Deduplicate saved_regs
    seen: set[str] = set()
    unique_saved: List[str] = []
    for r in saved_regs:
        if r not in seen:
            seen.add(r)
            unique_saved.append(r)
    summary.saved_registers = unique_saved

    # Memory accesses, constants, calls
    seen_calls: set[str] = set()
    seen_constants: set[str] = set()

    for insn in instructions:
        # Calls
        if _is_call(insn):
            if insn.relocation:
                name = insn.relocation.canonical_symbol or insn.relocation.symbol
            elif insn.display_mnemonic:
                name = insn.display_mnemonic
            else:
                target = _resolve_branch_target(insn)
                name = f"0x{target:08X}" if target else "indirect"
            if name not in seen_calls:
                seen_calls.add(name)
                summary.calls.append(name)

        # Memory accesses (d-form has immediate offset; x-form uses register index)
        if insn.opcode in _LOAD_OPCODES and len(insn.operands) >= 3:
            base = f"r{insn.operands[1]}"
            width_bits = _load_width(insn.opcode) * 8
            if insn.opcode in _XFORM_LOAD_OPCODES:
                summary.memory_accesses.append(MemoryAccess(
                    kind="load", width_bits=width_bits,
                    base=base, offset=0,
                    likely_expression=f"r{insn.operands[1]}+r{insn.operands[2]}",
                ))
            else:
                summary.memory_accesses.append(MemoryAccess(
                    kind="load", width_bits=width_bits,
                    base=base, offset=insn.operands[2],
                ))
        elif insn.opcode in _STORE_OPCODES and len(insn.operands) >= 3:
            base = f"r{insn.operands[1]}"
            width_bits = _store_width(insn.opcode) * 8
            if insn.opcode in _XFORM_STORE_OPCODES:
                summary.memory_accesses.append(MemoryAccess(
                    kind="store", width_bits=width_bits,
                    base=base, offset=0,
                    likely_expression=f"r{insn.operands[1]}+r{insn.operands[2]}",
                ))
            else:
                summary.memory_accesses.append(MemoryAccess(
                    kind="store", width_bits=width_bits,
                    base=base, offset=insn.operands[2],
                ))

        # Constants (li/lis patterns)
        if insn.opcode == Opcode.ADDI and len(insn.operands) >= 3 and insn.operands[1] == 0:
            val = insn.operands[2]
            h = f"0x{val & 0xFFFF:04X}" if -32768 <= val <= 65535 else f"0x{val:X}"
            if h not in seen_constants:
                seen_constants.add(h)
                summary.constants.append(h)
        elif insn.opcode == Opcode.ADDIS and len(insn.operands) >= 3 and insn.operands[1] == 0:
            val = (insn.operands[2] << 16) & 0xFFFFFFFF
            h = f"0x{val:08X}"
            if h not in seen_constants:
                seen_constants.add(h)
                summary.constants.append(h)

        # Used registers
        for op in insn.operands:
            if 0 <= op <= 31 and f"r{op}" not in summary.used_registers:
                summary.used_registers.append(f"r{op}")

    summary.used_registers.sort()
    return summary


def _load_width(opcode: Opcode) -> int:
    if opcode in (Opcode.LBZ, Opcode.LBZU, Opcode.LBZX, Opcode.LBZUX):
        return 1
    if opcode in (Opcode.LHA, Opcode.LHAU, Opcode.LHZ, Opcode.LHZU,
                  Opcode.LHAX, Opcode.LHAUX, Opcode.LHZX, Opcode.LHZUX,
                  Opcode.LHBRX):
        return 2
    if opcode in (Opcode.LWZ, Opcode.LWZU, Opcode.LWZX, Opcode.LWZUX,
                  Opcode.LWBRX):
        return 4
    if opcode in (Opcode.LFS, Opcode.LFSU, Opcode.LFSX, Opcode.LFSUX):
        return 4
    return 4


def _store_width(opcode: Opcode) -> int:
    if opcode in (Opcode.STB, Opcode.STBU, Opcode.STBX, Opcode.STBUX):
        return 1
    if opcode in (Opcode.STH, Opcode.STHU, Opcode.STHX, Opcode.STHUX,
                  Opcode.STHBRX):
        return 2
    if opcode in (Opcode.STW, Opcode.STWU, Opcode.STWX, Opcode.STWUX,
                  Opcode.STMW, Opcode.STWBRX):
        return 4
    if opcode in (Opcode.STFS, Opcode.STFSU, Opcode.STFSX, Opcode.STFSUX):
        return 4
    if opcode in (Opcode.STFD, Opcode.STFDU, Opcode.STFDX, Opcode.STFDUX,
                  Opcode.STFIWX):
        return 8
    return 4


# ---------------------------------------------------------------------------
# 9.7 — Declaration extractor (bounded)
# ---------------------------------------------------------------------------

def _find_class_name(signature: str) -> Optional[str]:
    """Extract the class name from a function signature like `Class::method`."""
    m = re.search(r'(\w+)::\w+\s*\(', signature)
    if m:
        return m.group(1)
    return None


def _find_declarations_in_source(
    source: str,
    names: List[str],
    max_chars: int,
) -> List[DeclarationEntry]:
    """Find class/struct/enum declarations in source by name, bounded by max_chars."""
    entries: List[DeclarationEntry] = []
    total = 0

    for name in names:
        if total >= max_chars:
            break
        # Look for `class Name`, `struct Name`
        patterns = [
            rf'(?:class|struct)\s+{re.escape(name)}\s*[\{{:\n]',
            rf'enum\s+(?:class\s+)?{re.escape(name)}\s*[\{{:\n]',
        ]
        for pat in patterns:
            for m in re.finditer(pat, source, re.MULTILINE):
                # Find the opening brace or semicolon
                brace = _find_matching_brace(source, m.end())
                if brace is None:
                    continue
                decl = source[m.start():brace + 1]
                char_count = len(decl)
                if total + char_count > max_chars:
                    decl = decl[:max_chars - total] + "\n// [truncated]"
                    char_count = len(decl)
                entries.append(DeclarationEntry(
                    name=name,
                    declaration=decl.strip(),
                    source="header",
                    char_count=char_count,
                ))
                total += char_count
                break  # First match only per name
    return entries


def _find_matching_brace(source: str, start: int) -> Optional[int]:
    """Find the matching closing brace from position start."""
    brace = source.find("{", start)
    if brace < 0:
        # Forward declaration with no body
        semi = source.find(";", start)
        return semi if semi >= 0 else None
    depth = 1
    i = brace + 1
    while i < len(source) and depth > 0:
        c = source[i]
        if c == '{':
            depth += 1
        elif c == '}':
            depth -= 1
        i += 1
    return i - 1 if depth == 0 else None


def build_declaration_context(
    source_text: str,
    header_text: str,
    signature: str,
    callee_symbols: List[str],
    max_chars: int = 20000,
) -> TypeContext:
    """Extract relevant declarations bounded by char limit."""
    names: List[str] = []
    cls = _find_class_name(signature)
    if cls:
        names.append(cls)
    # Add callee base names
    for sym in callee_symbols:
        base = sym.split("::")[-1] if "::" in sym else sym
        # Extract type-like names (capitalized or containing 'Class')
        if base and (base[0].isupper() or 'Class' in base or 'struct' in base.lower()):
            names.append(base.split("<")[0])

    names = list(dict.fromkeys(names))  # dedup preserving order

    entries: List[DeclarationEntry] = []
    total = 0

    # Try header first
    if header_text:
        header_entries = _find_declarations_in_source(header_text, names, max_chars)
        for e in header_entries:
            if total + e.char_count > max_chars:
                break
            entries.append(e)
            total += e.char_count

    # Then source file for any remaining
    if total < max_chars:
        remaining = max_chars - total
        source_entries = _find_declarations_in_source(source_text, names, remaining)
        for e in source_entries:
            if total + e.char_count > max_chars:
                break
            entries.append(e)
            total += e.char_count

    return TypeContext(
        declarations=entries,
        total_chars=total,
        max_chars=max_chars,
    )


# Phase 6: Enhanced type context extraction
def _extract_owner_class_declaration(source_text: str, class_name: str, max_chars: int = 5000) -> str:
    """Extract the owner class declaration with relevant members."""
    if not class_name:
        return ""
    # Look for class/struct declaration
    patterns = [
        rf'class\s+{re.escape(class_name)}\s*\{{',
        rf'struct\s+{re.escape(class_name)}\s*\{{',
    ]
    for pattern in patterns:
        match = re.search(pattern, source_text)
        if match:
            start = match.start()
            # Find matching closing brace
            depth = 0
            for i, ch in enumerate(source_text[start:]):
                if ch == '{':
                    depth += 1
                elif ch == '}':
                    depth -= 1
                    if depth == 0:
                        end = start + i + 1
                        decl = source_text[start:end]
                        return decl[:max_chars]
            break
    return ""


def _extract_callee_declarations(source_text: str, header_text: str, callee_symbols: List[str], max_chars: int = 8000) -> List[Dict[str, str]]:
    """Extract declarations for callee functions."""
    results = []
    all_text = (header_text or "") + "\n" + (source_text or "")
    total = 0
    for sym in callee_symbols:
        if total >= max_chars:
            break
        # Extract base name for searching
        base = sym.split("::")[-1] if "::" in sym else sym
        # Look for function declaration
        patterns = [
            rf'(?:^|\n)\s*(?:inline\s+|static\s+|virtual\s+)?[\w:\s+)?[\w\s\*\&]+\s+{re.escape(base)}\s*\(',
        ]
        for pattern in patterns:
            match = re.search(pattern, all_text)
            if match:
                start = match.start()
                # Find end of declaration (semicolon or opening brace)
                end_pos = all_text.find('{', start)
                semi = all_text.find(';', start)
                if end_pos == -1 or (semi != -1 and semi < end_pos):
                    end = all_text.find('\n', semi) if semi != -1 else start + 500
                else:
                    # Find matching brace
                    depth = 0
                    for i, ch in enumerate(all_text[end_pos:]):
                        if ch == '{':
                            depth += 1
                        elif ch == '}':
                            depth -= 1
                            if depth == 0:
                                end = end_pos + i + 1
                                break
                    else:
                        end = end_pos + 500
                decl = all_text[start:min(end, start + 2000)]
                results.append({"symbol": sym, "declaration": decl.strip()})
                total += len(decl)
                break
    return results


def _extract_referenced_members(source_text: str, function_bytes: FunctionBytes, max_chars: int = 5000) -> List[Dict[str, Any]]:
    """Extract member variables referenced by the function (via offsets)."""
    results = []
    # Parse relocations for data access
    for reloc in function_bytes.relocations:
        if reloc.relocation_type in ('ADDR16_HA', 'ADDR16_LO', 'ADDR16_HI'):
            # Likely a global or member access
            sym = reloc.symbol
            if '::' in sym:
                # Member access via class symbol
                parts = sym.split('::')
                if len(parts) == 2:
                    results.append({
                        "class": parts[0],
                        "member": parts[1],
                        "offset": reloc.addend,
                        "type": "inferred",
                    })
    return results[:10]  # Cap at 10


def _extract_globals(source_text: str, header_text: str, function_bytes: FunctionBytes, max_chars: int = 3000) -> List[Dict[str, str]]:
    """Extract global variable declarations referenced by the function."""
    results = []
    all_text = (header_text or "") + "\n" + (source_text or "")
    # Look for relocations to data sections
    for reloc in function_bytes.relocations:
        sym = reloc.symbol
        if sym.startswith('lbl_eu_'):
            # Likely a global data symbol
            # Search for its declaration
            pattern = rf'(?:^|\n)\s*(?:extern\s+)?[\w\s\*\&]+\s+{re.escape(sym)}\s*[\[;]'
            match = re.search(pattern, all_text)
            if match:
                start = match.start()
                end = all_text.find('\n', start)
                if end == -1:
                    end = start + 300
                results.append({"symbol": sym, "declaration": all_text[start:end].strip()})
    return results[:5]


def _extract_enums(source_text: str, header_text: str, max_chars: int = 2000) -> List[Dict[str, Any]]:
    """Extract enum definitions."""
    results = []
    all_text = (header_text or "") + "\n" + (source_text or "")
    pattern = r'enum\s+(?:class\s+)?(\w+)\s*\{([^}]+)\}'
    for match in re.finditer(pattern, all_text):
        name = match.group(1)
        body = match.group(2)
        values = [v.strip() for v in body.split(',') if v.strip()]
        results.append({"name": name, "values": values})
    return results[:5]


def _extract_selected_siblings(source_text: str, target_id: str, accepted_targets: List[Dict[str, str]], max_chars: int = 15000) -> List[Dict[str, Any]]:
    """Extract bodies of accepted functions in the same unit."""
    results = []
    total = 0
    for sibling in accepted_targets[:3]:  # Cap at 3 siblings
        if total >= max_chars:
            break
        # Find function in source
        func_name = sibling.get("function", "")
        if not func_name:
            continue
        # Look for function definition
        pattern = re.escape(func_name.split("::")[-1] if "::" in func_name else func_name)
        pattern = rf'(?:^|\n)\s*[\w\s\*\&]+\s+{pattern}\s*\('
        match = re.search(pattern, source_text)
        if match:
            start = match.start()
            # Find matching brace
            brace_start = source_text.find('{', start)
            if brace_start != -1:
                depth = 0
                for i, ch in enumerate(source_text[brace_start:]):
                    if ch == '{':
                        depth += 1
                    elif ch == '}':
                        depth -= 1
                        if depth == 0:
                            end = brace_start + i + 1
                            body = source_text[start:end]
                            results.append({
                                "id": sibling.get("id", ""),
                                "function": func_name,
                                "status": sibling.get("status", ""),
                                "body": body[:max_chars // 3],
                            })
                            total += len(body)
                            break
    return results


def _extract_callers(source_text: str, header_text: str, target_symbol: str, max_chars: int = 10000) -> List[Dict[str, Any]]:
    """Extract caller excerpts that call the target function."""
    results = []
    all_text = (header_text or "") + "\n" + (source_text or "")
    target_base = target_symbol.split("::")[-1] if "::" in target_symbol else target_symbol
    # Search for calls to the target
    pattern = rf'\b{re.escape(target_base)}\s*\('
    total = 0
    for match in re.finditer(pattern, all_text):
        if total >= max_chars:
            break
        # Get context around the call (500 chars before, 500 after)
        start = max(0, match.start() - 500)
        end = min(len(all_text), match.end() + 500)
        context = all_text[start:end]
        # Try to find the containing function
        # Look backwards for function signature
        func_start = context.rfind('{')
        if func_start != -1:
            # Find the function signature before this
            sig_start = context.rfind('\n', 0, func_start)
            if sig_start != -1:
                snippet = context[sig_start:end]
                results.append({"caller_snippet": snippet[:1000]})
                total += len(snippet)
    return results[:3]


# ---------------------------------------------------------------------------
# 9.12 — Dossier validation
# ---------------------------------------------------------------------------

def validate_dossier(d: TargetDossier) -> List[str]:
    """Pre-generation validation. Returns fatal errors that should block the model call."""
    errors: List[str] = []
    if d.signature is None or not d.signature.declaration:
        errors.append("missing target declaration")
    if d.retail is None or not d.retail.instructions:
        errors.append("missing decoded retail instructions")
    if d.signature and d.signature.authoritative is False:
        errors.append("signature is not authoritative")

    # Warnings for unresolved call symbols
    if d.symbols and d.retail:
        known_functions = {f.name for f in d.symbols.functions}
        for call in d.retail.data_flow.calls if d.retail.data_flow else []:
            if call not in known_functions:
                d.warnings.append(f"unresolved call symbol: {call}")

    return errors


def require_dossier_valid(d: TargetDossier) -> None:
    """Raise ValueError on fatal dossier errors."""
    errors = validate_dossier(d)
    if errors:
        raise ValueError(
            f"Dossier validation failed ({len(errors)} errors): "
            + "; ".join(errors)
        )


# ---------------------------------------------------------------------------
# 9.2, 9.3 — Target identity and signature helpers
# ---------------------------------------------------------------------------

def build_target_identity(
    target_id: str,
    demangled: str,
    mangled: str,
    source_file: str,
    object_file: str,
    unit: str,
    address: int,
    size: int,
) -> TargetIdentity:
    return TargetIdentity(
        target_id=target_id,
        mangled_name=mangled,
        demangled_name=demangled,
        source_file=source_file,
        object_file=object_file,
        translation_unit=unit,
        retail_address=f"0x{address:08X}",
        retail_size=size,
    )


def _split_params(params_str: str) -> List[str]:
    """Split a comma-separated parameter list respecting template nesting."""
    params: List[str] = []
    depth = 0
    current: List[str] = []
    for ch in params_str:
        if ch in '<(':
            depth += 1
            current.append(ch)
        elif ch in '>)':
            depth -= 1
            current.append(ch)
        elif ch == ',' and depth == 0:
            params.append(''.join(current).strip())
            current = []
        else:
            current.append(ch)
    remaining = ''.join(current).strip()
    if remaining:
        params.append(remaining)
    return params


def parse_signature(demangled: str) -> SignatureContext:
    """Parse a demangled function signature into structured form.

    Handles MWCC-format demangled names such as::

        Class::method(Params)
        ReturnType Class::method(Params)
        ns::Class::method(Params)
        function(Params)

    When the demangled name omits a return type (the common case), it
    defaults to ``"void"``.  Parameter names are best-effort — many
    demangled names in the registry omit them.
    """
    # Extract the parameter list from parentheses
    paren = demangled.find('(')
    if paren >= 0:
        sig_base = demangled[:paren].strip()
        close = demangled.rfind(')')
        params_str = demangled[paren + 1:close] if close > paren else ""
    else:
        sig_base = demangled.strip()
        params_str = ""

    # Remove trailing const qualifier from sig_base
    sig_base = re.sub(r'\s+const\s*$', '', sig_base).strip()

    implicit_this: Optional[ImplicitThis] = None
    return_info: Optional[SignatureReturn]
    parameters: List[SignatureParameter] = []

    if "::" in sig_base:
        # Split around the last ``::`` to separate the method name from
        # the class chain (``ns1::ns2::Class`` → ``method``).
        last_scope = sig_base.rfind("::")
        before_method = sig_base[:last_scope].strip()

        # The class chain is the trailing ``::``-separated identifiers.
        chain_match = re.search(r'((?:\w+::)*\w+)$', before_method)
        if chain_match:
            cls_chain = chain_match.group(1)
            ret_type = before_method[:chain_match.start()].strip()
        else:
            cls_chain = before_method
            ret_type = ""

        cls_name = cls_chain.rsplit("::", 1)[-1] if "::" in cls_chain else cls_chain
        implicit_this = ImplicitThis(type=f"{cls_name}*")
        return_info = SignatureReturn(type=ret_type or "void")
    else:
        # Free function — the first word may be a return type.
        parts = sig_base.rsplit(None, 1)
        if len(parts) >= 2:
            return_info = SignatureReturn(type=parts[0])
        else:
            return_info = SignatureReturn(type="void")

    # Known C++ type words that can appear mid-type (never parameter names)
    _TYPE_WORDS = frozenset({
        'unsigned', 'signed', 'short', 'long', 'int', 'char', 'double',
        'float', 'bool', 'void', 'const', 'volatile', 'struct', 'class',
        'wchar_t', 'size_t',
    })

    # Extract individual parameters
    raw = params_str.strip()
    if raw and raw != "void":
        for p in _split_params(raw):
            p = p.strip()
            if not p:
                continue
            # Best-effort type/name split: if the last token looks like a
            # parameter name (lowercase, not a C++ type keyword, not a
            # pointer/reference), treat it as the name.  Otherwise treat the
            # whole string as a type-only parameter.
            segs = p.rsplit(None, 1)
            if (
                len(segs) == 2
                and not segs[1].startswith('*')
                and not segs[1].startswith('&')
                and segs[1][0].islower()
                and segs[1] not in _TYPE_WORDS
            ):
                parameters.append(SignatureParameter(name=segs[1], type=segs[0]))
            else:
                parameters.append(SignatureParameter(name="", type=p))

    return SignatureContext(
        declaration=demangled,
        authoritative=True,
        implicit_this=implicit_this,
        parameters=parameters,
        return_info=return_info,
    )


# ---------------------------------------------------------------------------
# 9.1 — Dossier builder (orchestrator)
# ---------------------------------------------------------------------------

def build_target_dossier(
    target_id: str,
    mangled: str,
    demangled: str,
    source_file: str,
    object_file: str,
    unit: str,
    address: int,
    retail_size: int,
    function_bytes: FunctionBytes,
    source_text: str,
    header_text: str,
    callee_symbols: Optional[List[str]] = None,
    constraints: Optional[PromptConstraints] = None,
) -> TargetDossier:
    """Build a complete TargetDossier for a decompilation target."""
    if constraints is None:
        constraints = PromptConstraints()

    target = build_target_identity(
        target_id, demangled, mangled,
        source_file, object_file, unit, address, retail_size,
    )

    signature = parse_signature(demangled)

    # Decode once — reuse the raw IR for CFG / data-flow and for
    # DecodedInstruction construction, avoiding a second decode pass.
    all_raw = decode_block(
        function_bytes.code,
        function_bytes.base,
        relocations=list(function_bytes.relocations),
    )
    truncated = constraints.max_decoded_instructions > 0 and len(all_raw) > constraints.max_decoded_instructions

    decoded = decode_instructions(
        function_bytes,
        max_instructions=constraints.max_decoded_instructions,
        raw_insns=all_raw,
    )

    cfg = build_cfg(all_raw, function_bytes.base)
    data_flow = build_data_flow_summary(all_raw)

    retail = RetailProgramContext(
        base=f"0x{address:08X}",
        size=retail_size,
        instructions=decoded,
        cfg=cfg,
        data_flow=data_flow,
        truncated=truncated,
    )

    source = SourceContext(
        kind="detected-function-definition",
        begin_marker=f"// LLM-HARNESS-BEGIN: {target_id}",
        end_marker=f"// LLM-HARNESS-END: {target_id}",
        source_path=source_file,
    )

    call_list = data_flow.calls if data_flow else []
    calls = CallContext(
        direct_calls=[
            {"symbol": name} for name in call_list
        ],
    )

    types = build_declaration_context(
        source_text=source_text,
        header_text=header_text,
        signature=demangled,
        callee_symbols=callee_symbols or [],
        max_chars=constraints.max_declaration_chars,
    )

    # Phase 6: Enhanced type context
    cls = _find_class_name(demangled) or ""
    owner_decl = _extract_owner_class_declaration(source_text, cls)
    callee_decls = _extract_callee_declarations(source_text, header_text, callee_symbols or [])

    symbols = SymbolInventory(
        functions=[
            SymbolFunction(name=name, declaration=name)
            for name in call_list
        ],
    )

    dossier = TargetDossier(
        schema_version=3,
        target=target,
        signature=signature,
        retail=retail,
        source=source,
        calls=calls,
        types=types,
        symbols=symbols,
        constraints=constraints,
    )
    # Add enhanced type context as attributes
    dossier.types.owner_declaration = owner_decl
    dossier.types.callee_declarations = callee_decls

    return dossier


def dossier_to_dict(d: TargetDossier) -> Dict[str, Any]:
    """Convert a TargetDossier to a plain dict for JSON serialization."""
    def _convert(obj: Any) -> Any:
        if hasattr(obj, '__dataclass_fields__'):
            return {k: _convert(v) for k, v in asdict(obj).items()}
        if isinstance(obj, list):
            return [_convert(item) for item in obj]
        return obj

    return _convert(d)

from __future__ import annotations

from collections import Counter
from dataclasses import dataclass, field
from typing import Any, Dict, List, Optional, Tuple

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.elf_symbols import FunctionBytes
from tools.ppc_equivalence.ir import (
    Opcode,
    R_PPC_REL24,
    R_PPC_REL14,
)

from .types import (
    StructuralComponent,
    StructuralReport,
)


# Weights from §11.9
STRUCTURAL_WEIGHTS: dict[str, float] = {
    "calls": 0.20,
    "relocations": 0.10,
    "memory_accesses": 0.25,
    "cfg": 0.20,
    "constants": 0.10,
    "returns": 0.10,
    "instruction_classes": 0.05,
}

# Penalties from §11.10
PENALTIES: dict[str, float] = {
    "unexpected_call": 0.10,
    "unexpected_global_store": 0.15,
    "missing_global_store": 0.15,
    "indirect_call_difference": 0.20,
    "return_path_difference": 0.10,
}

# Routing thresholds from §11.11
# Three bands:
#   <  first_compile_promotion_min  → semantic_repair
#   [first_compile_promotion_min, match_improve_at_or_above) → promotion_gate
#   >= match_improve_at_or_above    → match_improve
ROUTING_THRESHOLDS: dict[str, float] = {
    "match_improve_at_or_above": 0.75,
    "first_compile_promotion_min": 0.60,
}


@dataclass(frozen=True)
class MemoryAccess:
    kind: str
    width_bits: int
    base_class: str
    offset: int | None = None
    symbol: str | None = None
    signed: bool | None = None


@dataclass
class CFGShape:
    num_blocks: int = 0
    num_edges: int = 0
    num_conditional_branches: int = 0
    num_unconditional_branches: int = 0
    num_returns: int = 0
    has_backedges: bool = False
    num_backedges: int = 0
    exit_block_count: int = 0
    loop_nesting_count: int = 0


RETURN_REG_OPS = frozenset({Opcode.BCLR})


def _extract_calls(instructions: list) -> Counter:
    calls: Counter = Counter()
    for insn in instructions:
        if insn.link and insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCCTR):
            sym = None
            if insn.relocation and insn.relocation.symbol:
                sym = insn.relocation.canonical_symbol or insn.relocation.symbol
            if sym:
                calls[(insn.relocation.relocation_type, sym)] += 1
            else:
                target = insn.address + insn.operands[0] if insn.operands else 0
                calls[(R_PPC_REL24, f"0x{target & 0xFFFFFFFF:08X}")] += 1
    return calls


def compare_calls(
    retail_calls: Counter,
    candidate_calls: Counter,
    allowlist: set[str] | None = None,
) -> StructuralComponent:
    if allowlist is None:
        allowlist = set()

    all_keys = set(retail_calls) | set(candidate_calls)
    matched = 0
    expected = len(retail_calls)
    details: list[str] = []

    for key in all_keys:
        r = retail_calls.get(key, 0)
        c = candidate_calls.get(key, 0)
        _, name = key
        if r == c:
            matched += r
        elif r > c:
            missing = r - c
            if name not in allowlist:
                details.append(f"missing call: {name} x{missing}")
        else:
            extra = c - r
            details.append(f"unexpected call: {name} x{extra}")

    score = (matched / max(expected, 1)) if expected else 1.0
    return StructuralComponent(score=score, matched=matched, expected=expected, details=details)


def _extract_relocations(insns: list) -> list[tuple[int, str]]:
    relocs: list[tuple[int, str]] = []
    for insn in insns:
        if insn.relocation:
            rt = insn.relocation.relocation_type
            sym = insn.relocation.canonical_symbol or insn.relocation.symbol
            relocs.append((rt, sym))
    return relocs


def compare_relocations(retail: list, candidate: list) -> StructuralComponent:
    r_counts = Counter(retail)
    c_counts = Counter(candidate)
    all_keys = set(r_counts) | set(c_counts)
    matched = 0
    expected = len(retail)
    details: list[str] = []

    for key in all_keys:
        r = r_counts.get(key, 0)
        c = c_counts.get(key, 0)
        if r == c:
            matched += r
        elif r > c:
            details.append(f"missing relocation: {key[1]} (type {key[0]}) x{r-c}")
        else:
            details.append(f"unexpected relocation: {key[1]} (type {key[0]}) x{c-r}")

    score = matched / max(expected, 1) if expected else 1.0
    return StructuralComponent(score=score, matched=matched, expected=expected, details=details)


_LOAD_WIDTH: dict[Opcode, int] = {
    Opcode.LBZ: 8, Opcode.LBZU: 8, Opcode.LBZX: 8, Opcode.LBZUX: 8,
    Opcode.LHZ: 16, Opcode.LHZU: 16, Opcode.LHZX: 16, Opcode.LHZUX: 16,
    Opcode.LHA: 16, Opcode.LHAU: 16, Opcode.LHAX: 16, Opcode.LHAUX: 16,
    Opcode.LWZ: 32, Opcode.LWZU: 32, Opcode.LWZX: 32, Opcode.LWZUX: 32,
    Opcode.LFS: 32, Opcode.LFSU: 32, Opcode.LFSX: 32, Opcode.LFSUX: 32,
    Opcode.LFD: 64, Opcode.LFDU: 64, Opcode.LFDX: 64, Opcode.LFDUX: 64,
}
_STORE_WIDTH: dict[Opcode, int] = {
    Opcode.STB: 8, Opcode.STBU: 8, Opcode.STBX: 8, Opcode.STBUX: 8,
    Opcode.STH: 16, Opcode.STHU: 16, Opcode.STHX: 16, Opcode.STHUX: 16,
    Opcode.STW: 32, Opcode.STWU: 32, Opcode.STWX: 32, Opcode.STWUX: 32,
    Opcode.STFS: 32, Opcode.STFSU: 32, Opcode.STFSX: 32, Opcode.STFSUX: 32,
    Opcode.STFD: 64, Opcode.STFDU: 64, Opcode.STFDX: 64, Opcode.STFDUX: 64,
}

_SIGNED_LOADS = frozenset({Opcode.LHA, Opcode.LHAU, Opcode.LHAX, Opcode.LHAUX})
_XFORM_LOADS = frozenset({
    Opcode.LBZX, Opcode.LBZUX, Opcode.LHZX, Opcode.LHZUX,
    Opcode.LHAX, Opcode.LHAUX, Opcode.LWZX, Opcode.LWZUX,
})
_XFORM_STORES = frozenset({
    Opcode.STBX, Opcode.STBUX, Opcode.STHX, Opcode.STHUX,
    Opcode.STWX, Opcode.STWUX,
})


def _memory_base_class(insn) -> str:
    # Accesses with a relocation + named symbol are global (§11.4)
    if insn.relocation:
        return "global"
    if not insn.operands:
        return "unknown"
    if len(insn.operands) >= 2:
        base_reg = insn.operands[1]
        if base_reg == 1:
            return "stack"
        if base_reg == 2:
            return "sda2"
        if base_reg == 13:
            return "sda"
        if base_reg == 3:
            return "argument"
    return "unknown"


def _extract_memory_accesses(insns: list) -> list[MemoryAccess]:
    accesses: list[MemoryAccess] = []
    for insn in insns:
        if insn.opcode in _LOAD_WIDTH:
            width = _LOAD_WIDTH[insn.opcode]
            kind = "load"
        elif insn.opcode in _STORE_WIDTH:
            width = _STORE_WIDTH[insn.opcode]
            kind = "store"
        else:
            continue

        base = _memory_base_class(insn)
        offset = None
        symbol = None
        signed = insn.opcode in _SIGNED_LOADS

        if insn.relocation:
            symbol = insn.relocation.canonical_symbol or insn.relocation.symbol
        elif insn.opcode in _XFORM_LOADS or insn.opcode in _XFORM_STORES:
            if len(insn.operands) >= 3:
                offset = insn.operands[2]
        else:
            if len(insn.operands) >= 3:
                offset = insn.operands[2]

        accesses.append(MemoryAccess(
            kind=kind,
            width_bits=width,
            base_class=base,
            offset=offset,
            symbol=symbol,
            signed=signed,
        ))
    return accesses


def _access_key(acc: MemoryAccess) -> tuple:
    return (acc.kind, acc.width_bits, acc.base_class, acc.offset, acc.symbol, acc.signed)


def compare_memory_accesses(retail: list[MemoryAccess], candidate: list[MemoryAccess]) -> tuple[StructuralComponent, list[str], list[str]]:
    r_sigs = Counter(_access_key(a) for a in retail)
    c_sigs = Counter(_access_key(a) for a in candidate)
    all_keys = set(r_sigs) | set(c_sigs)
    matched = 0
    expected = len(retail)
    details: list[str] = []
    unexpected: list[str] = []
    missing: list[str] = []

    for key in all_keys:
        r = r_sigs.get(key, 0)
        c = c_sigs.get(key, 0)
        kind, width, base, offset, sym, signed = key

        label_parts = [f"{width}-bit {kind}"]
        if offset is not None:
            label_parts.append(f"offset={offset}")
        if sym:
            label_parts.append(f"symbol={sym}")
        if base:
            label_parts.append(base)
        label = " ".join(label_parts)

        if r == c:
            matched += r
        elif r > c:
            missing.append(f"missing {label} x{r-c}")
            details.append(f"missing access: {label} x{r-c}")
        else:
            unexpected.append(f"unexpected {label} x{c-r}")
            details.append(f"unexpected access: {label} x{c-r}")

    score = matched / max(expected, 1) if expected else 1.0
    return (
        StructuralComponent(score=score, matched=matched, expected=expected, details=details),
        unexpected,
        missing,
    )


def _build_cfg_shape(instructions: list, address_base: int = 0) -> CFGShape:
    shape = CFGShape()
    blocks: list[set[int]] = []
    current_block: set[int] = set()
    backedges: list[tuple[int, int]] = []
    block_starts: list[int] = []
    returns = 0

    for i, insn in enumerate(instructions):
        if not current_block:
            block_starts.append(insn.address)

        current_block.add(insn.address)

        is_branch = insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR)
        is_call = insn.link and insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCCTR)
        is_return = insn.opcode == Opcode.BCLR and not insn.link

        if is_return:
            returns += 1
            if current_block:
                blocks.append(current_block)
                current_block = set()
            continue

        if is_branch and not is_call:
            if current_block:
                blocks.append(current_block)
                current_block = set()

        if is_branch and is_call:
            # Calls don't end blocks for CFG shape since they return
            pass

    if current_block:
        blocks.append(current_block)

    shape.num_blocks = len(blocks)
    shape.num_returns = returns

    # Count edges and backedges
    for i, block in enumerate(blocks):
        if i == 0:
            continue
        prev_last = max(instructions[j].address for j in range(len(instructions))
                        if instructions[j].address in blocks[i-1]) if blocks[i-1] else 0

    # Simpler edge/backedge counting
    block_addr_to_idx = {}
    for idx, blk in enumerate(blocks):
        for addr in blk:
            block_addr_to_idx[addr] = idx

    edges = 0
    for i, insn in enumerate(instructions):
        is_branch = insn.opcode in (Opcode.B, Opcode.BC)
        is_call = insn.link and insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCCTR)
        is_return = insn.opcode == Opcode.BCLR and not insn.link

        if is_branch and not is_call:
            edges += 1
            if insn.opcode == Opcode.BC:
                edges += 1  # conditional branch has fall-through + taken
            if insn.operands:
                target = insn.address + insn.operands[0] if insn.opcode == Opcode.B else (insn.address + insn.operands[1] if insn.opcode == Opcode.BC and len(insn.operands) >= 2 else None)
                if target is not None:
                    src_idx = block_addr_to_idx.get(insn.address)
                    tgt_idx = block_addr_to_idx.get(target & 0xFFFFFFFF)
                    if src_idx is not None and tgt_idx is not None and tgt_idx <= src_idx:
                        backedges.append((insn.address, target & 0xFFFFFFFF))

        if is_branch and is_call:
            edges += 1  # call edge

    # Fall-through edges between consecutive blocks
    edges += max(0, len(blocks) - 1)

    shape.num_edges = edges
    shape.num_backedges = len(backedges)
    shape.has_backedges = bool(backedges)
    shape.exit_block_count = returns
    shape.loop_nesting_count = len({tgt for _, tgt in backedges})

    cond_branches = sum(
        1 for insn in instructions
        if insn.opcode == Opcode.BC and not (insn.link and insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCCTR))
    )
    uncond_branches = sum(
        1 for insn in instructions
        if insn.opcode == Opcode.B and not insn.link
    )
    shape.num_conditional_branches = cond_branches
    shape.num_unconditional_branches = uncond_branches

    return shape


def compare_cfg(retail_shape: CFGShape, candidate_shape: CFGShape) -> StructuralComponent:
    attrs = [
        ("num_blocks", "blocks"),
        ("num_edges", "edges"),
        ("num_conditional_branches", "conditional branches"),
        ("num_unconditional_branches", "unconditional branches"),
        ("num_returns", "returns"),
        ("num_backedges", "backedges"),
        ("exit_block_count", "exit blocks"),
        ("loop_nesting_count", "loop nesting count"),
    ]
    details: list[str] = []
    matched = 0
    expected = len(attrs)

    for attr, label in attrs:
        rv = getattr(retail_shape, attr)
        cv = getattr(candidate_shape, attr)
        if rv == cv:
            matched += 1
        else:
            details.append(f"retail {label}={rv}, candidate {label}={cv}")

    score = matched / max(expected, 1) if expected else 1.0
    return StructuralComponent(score=score, matched=matched, expected=expected, details=details)


# Constant-classification heuristics (§11.6)
_CONSTANT_CLASSES: list[tuple[str, int | None, int | None]] = [
    ("immediate_compare", None, None),
    ("bit_mask", 0xFFFF, None),
    ("shift_amount", 0, 63),
    ("field_offset", None, None),
    ("enum_like", 0, 255),
]


def _extract_constants(instructions: list) -> list[int]:
    constants: list[int] = []
    for insn in instructions:
        # Skip prologue stack-frame constants (§11.6)
        if insn.opcode == Opcode.ADDI and len(insn.operands) >= 3:
            if insn.operands[1] == 1:
                continue  # stack-pointer-relative → prologue offset
            val = insn.operands[2]
            if abs(val) <= 0x7FFF:
                constants.append(val)
        elif insn.opcode == Opcode.ADDIS and len(insn.operands) >= 3:
            if insn.operands[1] == 1:
                continue  # stack-pointer-relative → prologue offset
            constants.append(insn.operands[2] << 16)
        elif insn.opcode in (Opcode.CMPWI, Opcode.CMPLWI) and len(insn.operands) >= 2:
            constants.append(insn.operands[0])
        elif insn.opcode in (Opcode.ORI, Opcode.ORIS, Opcode.XORI, Opcode.XORIS) and len(insn.operands) >= 2:
            constants.append(insn.operands[2] if len(insn.operands) >= 3 else insn.operands[1])
        elif insn.opcode in (Opcode.RLWINM, Opcode.RLWIMI) and len(insn.operands) >= 4:
            # rotate-and-mask — capture the mask
            constants.append(insn.operands[2])
    return constants


def _classify_constant(val: int) -> str:
    if val == 0:
        return "zero"
    if 1 <= val <= 31:
        return "shift_amount"
    if 1 <= val <= 255:
        return "small_enum"
    if abs(val) >= 0x7FFF:
        return "address_like"
    if (val & (val + 1)) == 0 and val != 0:
        return "bit_mask"
    return "other"


def compare_constants(retail: list[int], candidate: list[int]) -> StructuralComponent:
    r_counts = Counter(_classify_constant(c) for c in retail)
    c_counts = Counter(_classify_constant(c) for c in candidate)
    all_classes = set(r_counts) | set(c_counts)
    matched = 0
    expected = sum(r_counts.values())
    details: list[str] = []

    for cls in all_classes:
        r = r_counts.get(cls, 0)
        c = c_counts.get(cls, 0)
        matched += min(r, c)
        if r > c:
            details.append(f"missing constant class {cls!r}: retail={r}, candidate={c}")
        elif c > r:
            details.append(f"unexpected constant class {cls!r}: retail={r}, candidate={c}")

    denominator = max(expected, sum(c_counts.values()))
    score = matched / denominator if denominator else 1.0
    return StructuralComponent(score=score, matched=matched, expected=expected, details=details)


def _extract_return_paths(instructions: list) -> list[str]:
    paths: list[str] = []
    for insn in instructions:
        if insn.opcode == Opcode.BCLR and not insn.link:
            if insn.operands:
                bo = insn.operands[0] >> 5
                if bo & 4:
                    paths.append("unconditional")
                else:
                    paths.append("conditional")
            else:
                paths.append("unconditional")
    return paths


def compare_returns(retail: list[str], candidate: list[str]) -> StructuralComponent:
    r_counts = Counter(retail)
    c_counts = Counter(candidate)
    all_types = set(r_counts) | set(c_counts)
    matched = 0
    expected = sum(r_counts.values())
    details: list[str] = []

    for rt in all_types:
        r = r_counts.get(rt, 0)
        c = c_counts.get(rt, 0)
        if r == c:
            matched += r
        elif r > c:
            details.append(f"missing {rt} return x{r-c}")
        else:
            details.append(f"unexpected {rt} return x{c-r}")

    score = matched / max(expected, 1) if expected else 1.0
    return StructuralComponent(score=score, matched=matched, expected=expected, details=details)


_COARSE_CLASSES: dict[Opcode, str] = {}

_INT_ARITH = frozenset({
    Opcode.ADDI, Opcode.ADDIS, Opcode.ADDIC, Opcode.ADDIC_DOT,
    Opcode.SUBFIC, Opcode.MULLI, Opcode.ADDC, Opcode.ADDE,
    Opcode.SUBF, Opcode.SUBFC, Opcode.SUBFE, Opcode.SUBFZE,
    Opcode.NEG, Opcode.ADD, Opcode.DIVW, Opcode.DIVWU,
    Opcode.MULHW, Opcode.MULHWU, Opcode.MULLW,
})
_LOGICAL = frozenset({
    Opcode.AND, Opcode.OR, Opcode.XOR, Opcode.NAND, Opcode.NOR,
    Opcode.EQV, Opcode.ANDC, Opcode.ORC,
    Opcode.ORI, Opcode.ORIS, Opcode.XORI, Opcode.XORIS,
    Opcode.ANDI_DOT, Opcode.ANDIS_DOT,
})
_SHIFT_ROTATE = frozenset({
    Opcode.RLWINM, Opcode.RLWIMI, Opcode.RLWNM,
    Opcode.SLW, Opcode.SRW, Opcode.SRAW, Opcode.SRAWI,
})
_COMPARE = frozenset({
    Opcode.CMPW, Opcode.CMPWI, Opcode.CMPLW, Opcode.CMPLWI,
})
_FLOAT = frozenset({
    Opcode.FADD, Opcode.FSUB, Opcode.FMUL, Opcode.FDIV,
    Opcode.FMR, Opcode.FNEG, Opcode.FABS, Opcode.FNABS,
    Opcode.FSEL, Opcode.FCMPU, Opcode.FCMPO, Opcode.FCTIW,
    Opcode.FCTIWZ, Opcode.FRSP,
})
_SPECIAL_REG = frozenset({
    Opcode.MFSPR, Opcode.MTSPR, Opcode.MFCR, Opcode.MFMSR,
    Opcode.MTMSR, Opcode.MFSR, Opcode.MTSR, Opcode.MFTB,
})
_SIGN_EXTEND = frozenset({Opcode.EXTSB, Opcode.EXTSH})
_PAIRED_SINGLE = frozenset({
    Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LX, Opcode.PSQ_LUX,
    Opcode.PSQ_ST, Opcode.PSQ_STU, Opcode.PSQ_STX, Opcode.PSQ_STUX,
})


def _coarse_class(insn) -> str:
    if insn.opcode == Opcode.BCLR and not insn.link:
        return "return"
    if insn.link and insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCCTR):
        return "call"
    if insn.opcode == Opcode.B:
        return "unconditional_branch"
    if insn.opcode == Opcode.BC:
        return "conditional_branch"
    if insn.opcode in _INT_ARITH:
        return "integer_arithmetic"
    if insn.opcode in _LOGICAL:
        return "logical"
    if insn.opcode in _SHIFT_ROTATE:
        return "shift_rotate"
    if insn.opcode in _COMPARE:
        return "compare"
    if insn.opcode in _FLOAT:
        return "floating_point"
    if insn.opcode in _PAIRED_SINGLE:
        return "paired_single"
    if insn.opcode in _SPECIAL_REG:
        return "special_register"
    if insn.opcode in _LOAD_WIDTH:
        return "load"
    if insn.opcode in _STORE_WIDTH:
        return "store"
    if insn.opcode in _SIGN_EXTEND:
        return "sign_extension"
    return "other"


def _extract_instruction_classes(instructions: list) -> Counter:
    return Counter(_coarse_class(insn) for insn in instructions)


def compare_instruction_classes(retail: Counter, candidate: Counter) -> StructuralComponent:
    all_classes = set(retail) | set(candidate)
    matched = 0
    expected = len(retail)
    details: list[str] = []

    for cls in all_classes:
        r = retail.get(cls, 0)
        c = candidate.get(cls, 0)
        if r == c:
            matched += r
        elif r > c:
            details.append(f"missing {cls} x{r-c}")
        else:
            details.append(f"unexpected {cls} x{c-r}")

    score = matched / max(sum(retail.values()), 1) if retail else 1.0
    return StructuralComponent(score=score, matched=matched, expected=sum(retail.values()), details=details)


def compare_structural(
    retail_fn: FunctionBytes,
    candidate_fn: FunctionBytes,
    *,
    allowlist: set[str] | None = None,
) -> StructuralReport:
    retail_insns = decode_block(
        retail_fn.code, retail_fn.base,
        relocations=list(retail_fn.relocations),
    )
    candidate_insns = decode_block(
        candidate_fn.code, candidate_fn.base,
        relocations=list(candidate_fn.relocations),
    )

    # 1. Calls
    retail_calls = _extract_calls(retail_insns)
    candidate_calls = _extract_calls(candidate_insns)
    calls_result = compare_calls(retail_calls, candidate_calls, allowlist=allowlist)

    # 2. Relocations
    retail_relocs = _extract_relocations(retail_insns)
    candidate_relocs = _extract_relocations(candidate_insns)
    relocs_result = compare_relocations(retail_relocs, candidate_relocs)

    # 3. Memory accesses
    retail_mem = _extract_memory_accesses(retail_insns)
    candidate_mem = _extract_memory_accesses(candidate_insns)
    mem_result, unexpected_mem, missing_mem = compare_memory_accesses(retail_mem, candidate_mem)

    # 4. CFG
    retail_cfg = _build_cfg_shape(retail_insns, retail_fn.base)
    candidate_cfg = _build_cfg_shape(candidate_insns, candidate_fn.base)
    cfg_result = compare_cfg(retail_cfg, candidate_cfg)

    # 5. Constants
    retail_consts = _extract_constants(retail_insns)
    candidate_consts = _extract_constants(candidate_insns)
    consts_result = compare_constants(retail_consts, candidate_consts)

    # 6. Returns
    retail_returns = _extract_return_paths(retail_insns)
    candidate_returns = _extract_return_paths(candidate_insns)
    returns_result = compare_returns(retail_returns, candidate_returns)

    # 7. Instruction classes
    retail_classes = _extract_instruction_classes(retail_insns)
    candidate_classes = _extract_instruction_classes(candidate_insns)
    classes_result = compare_instruction_classes(retail_classes, candidate_classes)

    # Aggregate effects
    unexpected_effects: list[str] = []
    missing_effects: list[str] = []

    for d in calls_result.details:
        if d.startswith("unexpected"):
            unexpected_effects.append(d)
        elif d.startswith("missing"):
            missing_effects.append(d)
    for d in relocs_result.details:
        if d.startswith("unexpected"):
            unexpected_effects.append(d)
        elif d.startswith("missing"):
            missing_effects.append(d)
    unexpected_effects.extend(unexpected_mem)
    missing_effects.extend(missing_mem)

    # Weighted total score
    components = {
        "calls": calls_result.score,
        "relocations": relocs_result.score,
        "memory_accesses": mem_result.score,
        "cfg": cfg_result.score,
        "constants": consts_result.score,
        "returns": returns_result.score,
        "instruction_classes": classes_result.score,
    }

    total = sum(
        components[k] * STRUCTURAL_WEIGHTS[k]
        for k in STRUCTURAL_WEIGHTS
    )

    # Check for indirect call differences
    retail_indirect = sum(
        1 for insn in retail_insns
        if insn.opcode == Opcode.BCCTR and insn.link
    )
    candidate_indirect = sum(
        1 for insn in candidate_insns
        if insn.opcode == Opcode.BCCTR and insn.link
    )
    if retail_indirect != candidate_indirect:
        total -= PENALTIES["indirect_call_difference"]

    # Check for return path differences (§11.7)
    if returns_result.score < 1.0:
        total -= PENALTIES["return_path_difference"]

    # Apply penalty effects
    for effect in unexpected_effects:
        if "call" in effect:
            total -= PENALTIES["unexpected_call"]
        if "store" in effect:
            total -= PENALTIES["unexpected_global_store"]

    for effect in missing_effects:
        if "store" in effect:
            total -= PENALTIES["missing_global_store"]

    total = max(0.0, min(1.0, total))

    return StructuralReport(
        total_score=total,
        calls=calls_result,
        relocations=relocs_result,
        memory_accesses=mem_result,
        cfg=cfg_result,
        constants=consts_result,
        returns=returns_result,
        instruction_classes=classes_result,
        unexpected_effects=unexpected_effects,
        missing_effects=missing_effects,
    )


def route_candidate(score: float, thresholds: dict[str, float] | None = None) -> str:
    if thresholds is None:
        thresholds = ROUTING_THRESHOLDS

    if score >= thresholds["match_improve_at_or_above"]:
        return "match_improve"
    if score >= thresholds["first_compile_promotion_min"]:
        return "promotion_gate"
    return "semantic_repair"

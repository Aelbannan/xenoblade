"""Natural-loop CFG recognition for decoded PowerPC instruction lists."""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass

from .ir import Instruction, Opcode

_BRANCH_OPCODES = frozenset({Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR})


@dataclass(frozen=True, slots=True)
class BasicBlock:
    start_pc: int
    end_pc: int
    instructions: tuple[Instruction, ...]


@dataclass(frozen=True, slots=True)
class NaturalLoop:
    header_pc: int
    back_edge: tuple[int, int]
    body_pcs: frozenset[int]
    latch_pc: int


@dataclass(frozen=True, slots=True)
class LoopCfgAnalysis:
    blocks: tuple[BasicBlock, ...]
    predecessors: dict[int, frozenset[int]]
    successors: dict[int, frozenset[int]]
    natural_loops: tuple[NaturalLoop, ...]
    unsupported_reason: str | None = None


def analyze_loop_cfg(instructions: Sequence[Instruction]) -> LoopCfgAnalysis:
    """Build a CFG and enumerate natural loops from dominated back-edges."""
    if not instructions:
        return LoopCfgAnalysis((), {}, {}, ())

    ordered = tuple(sorted(instructions, key=lambda item: item.address))
    by_address = {item.address: item for item in ordered}
    entry_pc = ordered[0].address
    end_pc = ordered[-1].address + 4

    leaders = _collect_leaders(ordered, end_pc)
    blocks = _build_basic_blocks(ordered, leaders)
    block_starts = {block.start_pc for block in blocks}
    successors, predecessors = _build_cfg_edges(blocks, by_address, end_pc, block_starts)

    dominators = _compute_dominators(entry_pc, block_starts, predecessors)
    back_edges = _find_back_edges(successors, dominators)
    cyclic_sccs = _cyclic_strongly_connected_components(block_starts, successors)
    unsupported_reason = _irreducible_reason(
        cyclic_sccs,
        predecessors,
        entry_pc,
    )
    if unsupported_reason is not None:
        return LoopCfgAnalysis(
            blocks,
            predecessors,
            successors,
            (),
            unsupported_reason=unsupported_reason,
        )

    natural_loops = _natural_loops_from_back_edges(
        back_edges,
        predecessors,
        dominators,
        blocks,
    )
    return LoopCfgAnalysis(
        blocks,
        predecessors,
        successors,
        natural_loops,
    )


def _collect_leaders(instructions: Sequence[Instruction], end_pc: int) -> set[int]:
    leaders = {instructions[0].address}
    for insn in instructions:
        for target in _static_branch_targets(insn):
            if target != end_pc:
                leaders.add(target)
        if insn.opcode in _BRANCH_OPCODES:
            fallthrough = insn.address + 4
            if fallthrough != end_pc:
                leaders.add(fallthrough)
    return leaders


def _build_basic_blocks(
    instructions: Sequence[Instruction],
    leaders: set[int],
) -> tuple[BasicBlock, ...]:
    blocks: list[BasicBlock] = []
    index = 0
    while index < len(instructions):
        start = instructions[index].address
        if start not in leaders:
            index += 1
            continue
        body: list[Instruction] = []
        while index < len(instructions):
            insn = instructions[index]
            if insn.address in leaders and body:
                break
            body.append(insn)
            index += 1
            if insn.opcode in _BRANCH_OPCODES:
                break
        if body:
            blocks.append(
                BasicBlock(
                    start_pc=start,
                    end_pc=body[-1].address,
                    instructions=tuple(body),
                ),
            )
    return tuple(blocks)


def _static_branch_targets(insn: Instruction) -> tuple[int, ...]:
    if insn.opcode == Opcode.B:
        target = insn.operands[0]
        return (target,) if isinstance(target, int) else ()
    if insn.opcode == Opcode.BC:
        target = insn.operands[2]
        return (target,) if isinstance(target, int) else ()
    return ()


def _static_successors(
    insn: Instruction,
    *,
    end_pc: int,
    block_starts: set[int],
) -> tuple[int, ...]:
    pc = insn.address
    if insn.opcode not in _BRANCH_OPCODES:
        return (_successor_pc(pc + 4, end_pc=end_pc, block_starts=block_starts),)
    if insn.opcode == Opcode.B:
        target = insn.operands[0]
        if isinstance(target, int):
            resolved = _successor_pc(target, end_pc=end_pc, block_starts=block_starts)
            return (resolved,) if resolved is not None else ()
        return ()
    if insn.opcode == Opcode.BC:
        succs: list[int] = []
        fallthrough = _successor_pc(pc + 4, end_pc=end_pc, block_starts=block_starts)
        if fallthrough is not None:
            succs.append(fallthrough)
        target = insn.operands[2]
        if isinstance(target, int):
            resolved = _successor_pc(target, end_pc=end_pc, block_starts=block_starts)
            if resolved is not None and resolved not in succs:
                succs.append(resolved)
        return tuple(succs)
    fallthrough = _successor_pc(pc + 4, end_pc=end_pc, block_starts=block_starts)
    return (fallthrough,) if fallthrough is not None else ()


def _successor_pc(
    pc: int,
    *,
    end_pc: int,
    block_starts: set[int],
) -> int | None:
    if pc == end_pc:
        return None
    if pc in block_starts:
        return pc
    return None


def _build_cfg_edges(
    blocks: Sequence[BasicBlock],
    by_address: dict[int, Instruction],
    end_pc: int,
    block_starts: set[int],
) -> tuple[dict[int, frozenset[int]], dict[int, frozenset[int]]]:
    successors: dict[int, set[int]] = {start: set() for start in block_starts}
    predecessors: dict[int, set[int]] = {start: set() for start in block_starts}
    for block in blocks:
        last = block.instructions[-1]
        for target in _static_successors(
            last,
            end_pc=end_pc,
            block_starts=block_starts,
        ):
            successors[block.start_pc].add(target)
            predecessors[target].add(block.start_pc)
    return (
        {key: frozenset(value) for key, value in successors.items()},
        {key: frozenset(value) for key, value in predecessors.items()},
    )


def _compute_dominators(
    entry_pc: int,
    block_starts: set[int],
    predecessors: dict[int, frozenset[int]],
) -> dict[int, frozenset[int]]:
    all_nodes = frozenset(block_starts)
    dom: dict[int, set[int]] = {node: set(all_nodes) for node in block_starts}
    dom[entry_pc] = {entry_pc}
    changed = True
    while changed:
        changed = False
        for node in sorted(block_starts - {entry_pc}):
            preds = predecessors.get(node, frozenset())
            if not preds:
                new_dom = {node}
            else:
                new_dom = {node} | set.intersection(*(dom[pred] for pred in preds))
            if new_dom != dom[node]:
                dom[node] = new_dom
                changed = True
    return {node: frozenset(values) for node, values in dom.items()}


def _find_back_edges(
    successors: dict[int, frozenset[int]],
    dominators: dict[int, frozenset[int]],
) -> tuple[tuple[int, int], ...]:
    back_edges: list[tuple[int, int]] = []
    for tail, targets in sorted(successors.items()):
        for head in sorted(targets):
            if head in dominators.get(tail, frozenset()):
                back_edges.append((tail, head))
    return tuple(back_edges)


def _natural_loop_body(
    header_pc: int,
    latch_pc: int,
    predecessors: dict[int, frozenset[int]],
) -> frozenset[int]:
    loop_nodes = {header_pc}
    work = [latch_pc]
    while work:
        node = work.pop()
        if node in loop_nodes:
            continue
        loop_nodes.add(node)
        for pred in predecessors.get(node, frozenset()):
            if pred not in loop_nodes:
                work.append(pred)
    return frozenset(loop_nodes)


def _instruction_pcs_for_blocks(
    blocks: Sequence[BasicBlock],
    block_starts: frozenset[int],
) -> frozenset[int]:
    pcs: set[int] = set()
    for block in blocks:
        if block.start_pc in block_starts:
            pcs.update(insn.address for insn in block.instructions)
    return frozenset(pcs)


def _natural_loops_from_back_edges(
    back_edges: Sequence[tuple[int, int]],
    predecessors: dict[int, frozenset[int]],
    dominators: dict[int, frozenset[int]],
    blocks: Sequence[BasicBlock],
) -> tuple[NaturalLoop, ...]:
    block_end_by_start = {block.start_pc: block.end_pc for block in blocks}
    loops: list[NaturalLoop] = []
    seen: set[tuple[int, int]] = set()
    for tail, head in back_edges:
        if head not in dominators.get(tail, frozenset()):
            continue
        key = (head, tail)
        if key in seen:
            continue
        seen.add(key)
        loop_blocks = _natural_loop_body(head, tail, predecessors)
        loops.append(
            NaturalLoop(
                header_pc=head,
                back_edge=(block_end_by_start[tail], head),
                body_pcs=_instruction_pcs_for_blocks(blocks, loop_blocks),
                latch_pc=block_end_by_start[tail],
            ),
        )
    loops.sort(key=lambda item: (item.header_pc, item.latch_pc))
    return tuple(loops)


def _cyclic_strongly_connected_components(
    block_starts: set[int],
    successors: dict[int, frozenset[int]],
) -> tuple[frozenset[int], ...]:
    index = 0
    stack: list[int] = []
    on_stack: set[int] = set()
    indices: dict[int, int] = {}
    lowlink: dict[int, int] = {}
    sccs: list[frozenset[int]] = []

    def strongconnect(node: int) -> None:
        nonlocal index
        indices[node] = index
        lowlink[node] = index
        index += 1
        stack.append(node)
        on_stack.add(node)

        for succ in successors.get(node, frozenset()):
            if succ not in indices:
                strongconnect(succ)
                lowlink[node] = min(lowlink[node], lowlink[succ])
            elif succ in on_stack:
                lowlink[node] = min(lowlink[node], indices[succ])

        if lowlink[node] == indices[node]:
            component: set[int] = set()
            while True:
                member = stack.pop()
                on_stack.remove(member)
                component.add(member)
                if member == node:
                    break
            if len(component) > 1 or _has_self_loop(node, successors):
                sccs.append(frozenset(component))

    for node in sorted(block_starts):
        if node not in indices:
            strongconnect(node)
    return tuple(sccs)


def _has_self_loop(node: int, successors: dict[int, frozenset[int]]) -> bool:
    return node in successors.get(node, frozenset())


def _irreducible_reason(
    cyclic_sccs: Sequence[frozenset[int]],
    predecessors: dict[int, frozenset[int]],
    entry_pc: int,
) -> str | None:
    for scc in cyclic_sccs:
        entries = _scc_entry_nodes(scc, predecessors, entry_pc)
        if len(entries) > 1:
            formatted = ", ".join(f"0x{pc:08x}" for pc in sorted(entries))
            return f"irreducible control flow: SCC with multiple entries ({formatted})"
    return None


def _scc_entry_nodes(
    scc: frozenset[int],
    predecessors: dict[int, frozenset[int]],
    entry_pc: int,
) -> frozenset[int]:
    entries: set[int] = set()
    for node in scc:
        outside_preds = [pred for pred in predecessors.get(node, frozenset()) if pred not in scc]
        if outside_preds:
            entries.add(node)
        elif node == entry_pc:
            entries.add(node)
    return frozenset(entries)

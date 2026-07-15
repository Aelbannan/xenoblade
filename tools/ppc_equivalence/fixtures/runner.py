from __future__ import annotations

from dataclasses import dataclass

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.fixtures.corpus import CODE_BASE, FixtureCase
from tools.ppc_equivalence.fixtures.encode import pack_xer
from tools.ppc_equivalence.ir import Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg


@dataclass(frozen=True, slots=True)
class FixtureObservation:
    result: int
    cr: int
    xer: int
    memory: dict[int, int]
    fpr: dict[int, int]
    fpscr: int


def _memory_word(state, address: int) -> int:
    value = 0
    for offset in range(4):
        value = (value << 8) | (state.memory.read(address + offset) & 0xFF)
    return value & 0xFFFFFFFF


def run_fixture_concrete(case: FixtureCase) -> FixtureObservation:
    code = b"".join((word & 0xFFFFFFFF).to_bytes(4, "big") for word in case.code_words)
    instructions = decode_block(code, base=CODE_BASE, validate_with_capstone=False)
    # Drop a trailing blr if present so straight-line cases fall through.
    if instructions and instructions[-1].opcode == Opcode.BCLR and instructions[-1].operands[:2] == (20, 0):
        instructions = instructions[:-1]
    initial = concrete_state(case.initial)
    # Branch exploration may enqueue both arms; allow headroom beyond the
    # number of architectural outcomes we expect to observe.
    terminals = execute_cfg(
        initial,
        instructions,
        ConcreteOps(),
        max_instructions=case.max_instructions,
        max_paths=max(case.allow_paths * 4, 8),
    )
    taken = [item for item in terminals if bool(item.condition)]
    if len(taken) != 1:
        raise AssertionError(f"{case.id}: expected one concrete path, got {len(taken)} ({terminals!r})")
    final = taken[0].state
    xer = pack_xer(ca=int(bool(final.xer.ca)), ov=int(bool(final.xer.ov)), so=int(bool(final.xer.so)))
    memory = {
        offset: _memory_word(final, int(case.initial.get("gpr", {}).get("r4", "0x1000"), 0) + offset)
        for offset in case.expected_memory
    }
    # Prefer explicit sandbox base from initial GPR when present.
    base = 0
    gpr = case.initial.get("gpr", {})
    if isinstance(gpr, dict) and "r4" in gpr:
        base = int(gpr["r4"], 0) if isinstance(gpr["r4"], str) else int(gpr["r4"])
    memory = {offset: _memory_word(final, base + offset) for offset in case.expected_memory}
    return FixtureObservation(
        result=int(final.gpr[case.result_reg]) & 0xFFFFFFFF,
        cr=int(final.cr) & 0xFFFFFFFF,
        xer=xer,
        memory=memory,
        fpr={index: int(final.fpr[index]) & 0xFFFFFFFFFFFFFFFF for index in case.expected_fpr},
        fpscr=int(final.fpscr) & 0xFFFFFFFF,
    )


def compare_fixture(case: FixtureCase) -> list[str]:
    actual = run_fixture_concrete(case)
    mismatches: list[str] = []
    if actual.result != case.expected_result:
        mismatches.append(
            f"result 0x{actual.result:08x} != 0x{case.expected_result:08x}"
        )
    if actual.cr != case.expected_cr:
        mismatches.append(f"cr 0x{actual.cr:08x} != 0x{case.expected_cr:08x}")
    if actual.xer != case.expected_xer:
        mismatches.append(f"xer 0x{actual.xer:08x} != 0x{case.expected_xer:08x}")
    for offset, expected in case.expected_memory.items():
        got = actual.memory.get(offset)
        if got != expected:
            mismatches.append(
                f"mem+0x{offset:x} 0x{(got or 0):08x} != 0x{expected:08x}"
            )
    for index, expected in case.expected_fpr.items():
        got = actual.fpr.get(index)
        if got != expected:
            mismatches.append(
                f"f{index} 0x{(got or 0):016x} != 0x{expected:016x}"
            )
    if case.expected_fpscr is not None and actual.fpscr != case.expected_fpscr:
        mismatches.append(
            f"fpscr 0x{actual.fpscr:08x} != 0x{case.expected_fpscr:08x}"
        )
    return mismatches

from __future__ import annotations

import unittest

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, execute_block, execute_instruction


def _insn(
    opcode: Opcode, operands: tuple[int, ...], *, address: int = 0,
) -> Instruction:
    return Instruction(address, 0, opcode, operands)


class MemoryAccessTrackingTests(unittest.TestCase):
    def test_load_records_reads_only(self) -> None:
        state = concrete_state({
            "gpr": {"r3": 0x100},
            "memory": {"0x100": 0xAB, "0x101": 0xCD, "0x102": 0xEF, "0x103": 0x12},
        })
        result = execute_instruction(
            state, _insn(Opcode.LWZ, (4, 3, 0)), ConcreteOps(),
        )
        self.assertTrue(result.valid)
        self.assertEqual(result.memory_writes, ())
        self.assertEqual(len(result.memory_reads), 4)
        self.assertEqual(result.memory_touches, result.memory_reads)

    def test_store_same_value_records_writes(self) -> None:
        state = concrete_state({
            "gpr": {"r3": 0x100, "r4": 0xAB},
            "memory": {"0x100": 0xAB},
        })
        result = execute_instruction(
            state, _insn(Opcode.STB, (4, 3, 0)), ConcreteOps(),
        )
        self.assertTrue(result.valid)
        self.assertEqual(result.memory, state.memory)
        self.assertEqual(result.memory_reads, ())
        self.assertEqual(len(result.memory_writes), 1)
        self.assertEqual(result.memory_touches, result.memory_writes)

    def test_store_then_restore_keeps_writes(self) -> None:
        state = concrete_state({
            "gpr": {"r3": 0x100, "r4": 0x11, "r5": 0x22},
            "memory": {"0x100": 0x11},
        })
        result = execute_block(
            state,
            [
                _insn(Opcode.STB, (5, 3, 0), address=0),
                _insn(Opcode.STB, (4, 3, 0), address=4),
            ],
            ConcreteOps(),
        )
        self.assertTrue(result.valid)
        self.assertEqual(result.memory, state.memory)
        self.assertEqual(result.memory_reads, ())
        self.assertEqual(len(result.memory_writes), 2)
        self.assertEqual(result.memory_touches, result.memory_writes)

    def test_memory_touches_is_union_of_reads_and_writes(self) -> None:
        state = concrete_state({
            "gpr": {"r3": 0x100, "r4": 0x7F},
            "memory": {"0x100": 0x00},
        })
        result = execute_block(
            state,
            [
                _insn(Opcode.LBZ, (5, 3, 0), address=0),
                _insn(Opcode.STB, (4, 3, 0), address=4),
            ],
            ConcreteOps(),
        )
        self.assertTrue(result.valid)
        self.assertEqual(len(result.memory_reads), 1)
        self.assertEqual(len(result.memory_writes), 1)
        self.assertEqual(
            result.memory_touches,
            result.memory_reads + result.memory_writes,
        )


if __name__ == "__main__":
    unittest.main()

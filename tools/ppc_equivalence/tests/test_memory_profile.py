from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.memory_profile import (
    MemoryEnvironment,
    MemoryProfile,
    access_within_any_range,
    access_within_range,
    build_memory_constraints,
    parse_ranges,
)
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import SymbolicOps


class TestParseRanges(unittest.TestCase):
    def test_single_range_hex(self):
        r = parse_ranges(["0x80000000,0x817fffff"])
        self.assertEqual(r, [(0x80000000, 0x817FFFFF)])

    def test_single_range_decimal(self):
        r = parse_ranges(["100,200"])
        self.assertEqual(r, [(100, 200)])

    def test_multiple_ranges(self):
        r = parse_ranges(["0x80000000,0x80100000", "0x81000000,0x81100000"])
        self.assertEqual(r, [(0x80000000, 0x80100000), (0x81000000, 0x81100000)])

    def test_invalid_format(self):
        with self.assertRaises(ValueError):
            parse_ranges(["0x80000000"])

    def test_low_greater_than_high(self):
        with self.assertRaises(ValueError):
            parse_ranges(["100,50"])

    def test_out_of_range_negative(self):
        with self.assertRaises(ValueError):
            parse_ranges(["-1,100"])

    def test_out_of_range_overflow(self):
        with self.assertRaises(ValueError):
            parse_ranges(["0x100000000,0x100001000"])


class TestAccessWithinRange(unittest.TestCase):
    def setUp(self):
        import z3
        self.z3 = z3
        self.addr = z3.BitVec("addr", 32)

    def _sat(self, expr):
        s = self.z3.Solver()
        s.add(expr)
        return s.check() == self.z3.sat

    def _unsat(self, expr):
        s = self.z3.Solver()
        s.add(expr)
        return s.check() == self.z3.unsat

    def test_address_in_range(self):
        c = access_within_range(self.addr, 1, 0x1000, 0x2000)
        self.assertTrue(self._sat(self.z3.And(c, self.addr == 0x1500)))

    def test_address_below_range(self):
        c = access_within_range(self.addr, 1, 0x1000, 0x2000)
        self.assertTrue(self._unsat(self.z3.And(c, self.addr == 0x0FFF)))

    def test_address_above_range(self):
        c = access_within_range(self.addr, 1, 0x1000, 0x2000)
        self.assertTrue(self._unsat(self.z3.And(c, self.addr == 0x2001)))

    def test_address_at_low_boundary(self):
        c = access_within_range(self.addr, 1, 0x1000, 0x2000)
        self.assertTrue(self._sat(self.z3.And(c, self.addr == 0x1000)))

    def test_address_at_high_boundary(self):
        c = access_within_range(self.addr, 1, 0x1000, 0x2000)
        self.assertTrue(self._sat(self.z3.And(c, self.addr == 0x2000)))

    def test_multi_byte_span(self):
        c = access_within_range(self.addr, 4, 0x1000, 0x1003)
        self.assertTrue(self._sat(self.z3.And(c, self.addr == 0x1000)))

    def test_multi_byte_overflows_range(self):
        c = access_within_range(self.addr, 4, 0x1000, 0x1002)
        self.assertTrue(self._unsat(self.z3.And(c, self.addr == 0x1000)))

    def test_multi_byte_wraparound(self):
        c = access_within_range(self.addr, 4, 0, 0xFFFFFFFF)
        self.assertTrue(self._unsat(self.z3.And(c, self.addr == 0xFFFFFFFE)))

    def test_any_range_disjoint_first(self):
        c = access_within_any_range(self.addr, 1, [(0x1000, 0x2000), (0x3000, 0x4000)])
        self.assertTrue(self._sat(self.z3.And(c, self.addr == 0x1500)))
        self.assertTrue(self._sat(self.z3.And(c, self.addr == 0x3500)))
        self.assertTrue(self._unsat(self.z3.And(c, self.addr == 0x2500)))

    def test_any_range_empty(self):
        c = access_within_any_range(self.addr, 1, [])
        self.assertFalse(self._sat(self.z3.Not(c)))
        self.assertTrue(self._sat(c))

    def test_width_bytes_invalid(self):
        with self.assertRaises(ValueError):
            access_within_range(self.addr, 0, 0, 100)


class TestMemoryEnvironment(unittest.TestCase):
    def test_assumed_ordinary_ram_no_constraints(self):
        env = MemoryEnvironment(profile=MemoryProfile.ASSUMED_ORDINARY_RAM)
        ops = SymbolicOps()
        self.assertEqual(build_memory_constraints([], [], env, ops), [])

    def test_bounded_no_ranges_no_constraints(self):
        env = MemoryEnvironment(profile=MemoryProfile.BOUNDED_ORDINARY_RAM, ranges=[])
        ops = SymbolicOps()
        self.assertEqual(build_memory_constraints([], [], env, ops), [])

    def test_to_dict_assumed(self):
        env = MemoryEnvironment()
        d = env.to_dict()
        self.assertEqual(d["memory_profile"], "assumed-ordinary-ram")
        self.assertEqual(d["ranges"], [])
        self.assertEqual(d["mmio"], "unrestricted")
        self.assertEqual(d["address_wraparound"], "rejected")

    def test_to_dict_bounded(self):
        env = MemoryEnvironment(
            profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
            ranges=[(0x80000000, 0x817FFFFF)],
        )
        d = env.to_dict()
        self.assertEqual(d["memory_profile"], "bounded-ordinary-ram")
        self.assertEqual(len(d["ranges"]), 1)

    def test_from_dict_roundtrip(self):
        env = MemoryEnvironment(
            profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
            ranges=[(0x80000000, 0x817FFFFF)],
        )
        d = env.to_dict()
        restored = MemoryEnvironment.from_dict(d)
        self.assertEqual(restored.profile, env.profile)
        self.assertEqual(restored.ranges, env.ranges)
        self.assertEqual(restored.alignment, env.alignment)


class TestBoundedEquivalence(unittest.TestCase):
    def test_equivalent_within_range(self):
        code = bytes.fromhex("38630004")  # addi r3, r3, 4
        from tools.ppc_equivalence.decoder import decode_block, parse_hex
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=10000)
        env = MemoryEnvironment(
            profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
            ranges=[(0x80000000, 0x8FFFFFFF)],
        )
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            memory_environment=env,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_store_within_range_equivalent(self):
        code = bytes.fromhex("90830000")  # stw r4, 0(r3)
        from tools.ppc_equivalence.decoder import decode_block
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["memory"], timeout_ms=10000)
        env = MemoryEnvironment(
            profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
            ranges=[(0x80000000, 0x8FFFFFFF)],
        )
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            memory_environment=env,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_different_stores_within_range_not_equivalent(self):
        """Different values stored to the same address — not equivalent."""
        code_a = bytes.fromhex("90830000")  # stw r4, 0(r3)
        code_b = bytes.fromhex("90A30000")  # stw r5, 0(r3)
        from tools.ppc_equivalence.decoder import decode_block
        insns_a = decode_block(code_a, 0x80000000)
        insns_b = decode_block(code_b, 0x80000000)
        contract = make_contract(preset=None, observe=["memory"], timeout_ms=10000)
        env = MemoryEnvironment(
            profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
            ranges=[(0, 0xFFFFFFFF)],
        )
        result = check_equivalence(
            insns_a, insns_b, contract,
            original_hex=code_a.hex(),
            candidate_hex=code_b.hex(),
            memory_environment=env,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)

    def test_bounded_range_reported_in_environment(self):
        code = bytes.fromhex("38630004")
        from tools.ppc_equivalence.decoder import decode_block
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=10000)
        env = MemoryEnvironment(
            profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
            ranges=[(0x80000000, 0x8FFFFFFF)],
        )
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            memory_environment=env,
        )
        self.assertIsNotNone(result.environment)
        self.assertEqual(result.environment.profile, MemoryProfile.BOUNDED_ORDINARY_RAM)
        self.assertEqual(result.environment.ranges, [(0x80000000, 0x8FFFFFFF)])
        d = result.to_dict()
        self.assertIn("environment", d)
        self.assertEqual(d["environment"]["memory_profile"], "bounded-ordinary-ram")


if __name__ == "__main__":
    unittest.main()

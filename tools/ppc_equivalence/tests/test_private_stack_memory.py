from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import CalleeContract


def decode(hex_words: str):
    return decode_block(parse_hex(hex_words), validate_with_capstone=False)


def prove_bytes(original_hex: str, candidate_hex: str, timeout_ms: int = 10_000):
    return check_equivalence(
        decode(original_hex),
        decode(candidate_hex),
        EquivalenceContract(parse_observables(["memory"]), timeout_ms=timeout_ms),
        original_hex=original_hex,
        candidate_hex=candidate_hex,
    )


def prove_bytes_with_contract(
    original_hex: str, candidate_hex: str, *,
    callee_contracts: dict[int | str, CalleeContract] | None = None,
    timeout_ms: int = 10_000,
):
    assumed = frozenset(callee_contracts or {})
    return check_equivalence(
        decode(original_hex),
        decode(candidate_hex),
        EquivalenceContract(parse_observables(["memory"]), timeout_ms=timeout_ms),
        original_hex=original_hex,
        candidate_hex=candidate_hex,
        assumed_callees=assumed,
        callee_contracts=callee_contracts,
    )


# addi r1,r1,imm for frame setup (no back-chain save, keeps stack_private=True).
ADDI_NEG_64 = "3821ffc0"  # addi r1,r1,-64 -> stack_low=entry_sp-64, private [entry_sp-64, entry_sp)
ADDI_POS_64 = "38210040"  # addi r1,r1,64
ADDI_NEG_16 = "3821fff0"  # addi r1,r1,-16 -> stack_low=entry_sp-16, private [entry_sp-16, entry_sp)
ADDI_POS_16 = "38210010"  # addi r1,r1,16
BLR = "4e800020"

# Byte stores (stb) have no alignment restriction, so both sides share the
# same validity conditions. After frame setup r1 = entry_sp - N,
# address = entry_sp - N + signed_offset.
STB_R3_8 = "98610008"    # stb r3,8(r1)  -> entry_sp-N+8
STB_R3_NEG32 = "9861ffe0"  # stb r3,-32(r1) -> entry_sp-N-32
STB_R3_NEG48 = "9861ffd0"  # stb r3,-48(r1) -> entry_sp-N-48

# Store r1 to trigger stack-pointer escape (even byte-store of r1 detected).
STB_R1_0 = "98210000"    # stb r1,0(r1)


@unittest.skipUnless(__import__("importlib").util.find_spec("z3"), "z3-solver is not installed")
class PrivateStackMemoryTests(unittest.TestCase):
    def test_candidate_writes_inside_original_only_private_interval(self):
        original = ADDI_NEG_64 + BLR
        candidate = ADDI_NEG_16 + STB_R3_NEG32 + ADDI_POS_16 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_original_writes_inside_candidate_only_private_interval(self):
        original = ADDI_NEG_16 + STB_R3_NEG32 + ADDI_POS_16 + BLR
        candidate = ADDI_NEG_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_different_frames_each_writes_within_own_frame(self):
        original = ADDI_NEG_64 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_16 + STB_R3_8 + ADDI_POS_16 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_one_side_private_other_leaves_unchanged(self):
        original = ADDI_NEG_64 + BLR
        candidate = ADDI_NEG_64 + STB_R3_8 + ADDI_POS_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_same_public_value_despite_different_frames(self):
        original = ADDI_NEG_64 + "98610040" + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_16 + "98610010" + ADDI_POS_16 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_different_public_values_at_address_private_to_only_one_side(self):
        original = ADDI_NEG_64 + STB_R3_NEG32 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_16 + STB_R3_NEG48 + ADDI_POS_16 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_stack_escape_disables_masking_on_original_only(self):
        original = ADDI_NEG_64 + STB_R1_0 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_16 + STB_R3_8 + ADDI_POS_16 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_stack_escape_disables_masking_on_candidate_only(self):
        original = ADDI_NEG_64 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_16 + STB_R1_0 + STB_R3_8 + ADDI_POS_16 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_stack_escape_on_both_sides_public_writes_must_match(self):
        original = ADDI_NEG_64 + STB_R1_0 + "98610040" + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + STB_R1_0 + "98610040" + ADDI_POS_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_call_disables_private_stack_masking(self):
        # bl +8 with a dummy callee contract disables private-stack inference.
        # After the call, a private-frame write is no longer masked.
        # bl +8 (target=8, at address=0): opcode 18, LI=2, LK=1.
        BL_CALL = "48000005"
        CALLEE_CONTRACT = {8: CalleeContract(frozenset(), frozenset(), "test")}
        original = ADDI_NEG_64 + BL_CALL + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + BL_CALL + ADDI_POS_64 + BLR
        result = prove_bytes_with_contract(
            original, candidate, callee_contracts=CALLEE_CONTRACT,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_multiple_paths_different_frames(self):
        # Two paths: r3==0 uses 64-byte frame; r3!=0 uses 16-byte frame.
        # Each side writes within its own frame on both paths.
        CMPWI_R3_0 = "2c600000"
        BEQ_PATH0 = "4182000c"     # beq +12 (skip 16-byte path)
        B_AFTER_PATH1 = "4800000c"  # b +12 (skip 64-byte path after 16-byte path)
        path_16 = ADDI_NEG_16 + STB_R3_8 + ADDI_POS_16
        path_64 = ADDI_NEG_64 + STB_R3_8 + ADDI_POS_64
        code = CMPWI_R3_0 + BEQ_PATH0 + path_16 + B_AFTER_PATH1 + path_64 + BLR
        original = code
        candidate = code
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_upward_stack_produces_inconclusive_layout(self):
        # addi r1,r1,64 without prior frame setup moves r1 above entry_sp,
        # invalidating the stack layout.
        UP = "38210040"
        original = UP + STB_R3_8 + BLR
        candidate = UP + STB_R3_8 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_LAYOUT)

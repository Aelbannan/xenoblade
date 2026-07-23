from __future__ import annotations

import importlib.util
import unittest

from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import RelocationRef, R_PPC_REL24
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import CalleeContract


def decode(hex_words: str, relocations=()):
    return decode_block(
        parse_hex(hex_words),
        relocations=relocations,
        validate_with_capstone=False,
    )


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
    original_relocs=(),
    candidate_relocs=(),
    timeout_ms: int = 10_000,
):
    assumed = frozenset(callee_contracts or {})
    return check_equivalence(
        decode(original_hex, original_relocs),
        decode(candidate_hex, candidate_relocs),
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

# Store r1: frame-relative spill keeps masking; public base escapes.
STB_R1_0 = "98210000"    # stb r1,0(r1) — private back-chain-style spill
STW_R1_0_R5 = "90250000"  # stw r1,0(r5) — escape via public store of SP
MR_R4_R1 = "7c240b78"     # mr r4,r1  (or r4,r1,r1)
ADDI_R4_R1_8 = "38810008"  # addi r4,r1,8
ORI_R4_R1_0 = "60240000"   # ori r4,r1,0
STW_R4_0_R5 = "90850000"  # stw r4,0(r5)
STMW_R1_0_R5 = "bc250000"  # stmw r1,0(r5) — includes r1 in the store range
LI_R3_1 = "38600001"
LI_R3_2 = "38600002"

# Relocated bl leaf; offset is filled after the frame addi (word 1).
BL_LEAF = "48000001"
LEAF_RELOC = (
    RelocationRef(
        offset=4, relocation_type=R_PPC_REL24,
        symbol="leaf", canonical_symbol="leaf", addend=0,
    ),
)


@unittest.skipUnless(importlib.util.find_spec("z3"), "z3-solver is not installed")
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
        # Public SP publish on original only — private stb becomes compared.
        original = ADDI_NEG_64 + STW_R1_0_R5 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_16 + STB_R3_8 + ADDI_POS_16 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_stack_escape_disables_masking_on_candidate_only(self):
        original = ADDI_NEG_64 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_16 + STW_R1_0_R5 + STB_R3_8 + ADDI_POS_16 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_frame_relative_r1_spill_keeps_masking(self):
        """stb r1,0(r1) is a private-frame spill, not a public SP publish."""
        original = ADDI_NEG_64 + STB_R1_0 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + STB_R1_0 + ADDI_POS_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        scope = result.memory_scope.to_dict()["private_stack"]
        self.assertTrue(scope["original"]["enabled_on_all_terminal_paths"])
        self.assertTrue(scope["candidate"]["enabled_on_all_terminal_paths"])

    def test_stack_escape_on_both_sides_public_writes_must_match(self):
        original = ADDI_NEG_64 + STW_R1_0_R5 + "98610040" + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + STW_R1_0_R5 + "98610040" + ADDI_POS_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_escape_via_direct_store_of_r1_to_public(self):
        # Different private-frame bytes become observable once r1 is stored publicly.
        original = ADDI_NEG_64 + STW_R1_0_R5 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + STW_R1_0_R5 + ADDI_POS_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_escape_via_mr_alias_then_store(self):
        original = ADDI_NEG_64 + MR_R4_R1 + STW_R4_0_R5 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + MR_R4_R1 + STW_R4_0_R5 + ADDI_POS_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_escape_via_ori_alias_then_store(self):
        original = ADDI_NEG_64 + ORI_R4_R1_0 + STW_R4_0_R5 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + ORI_R4_R1_0 + STW_R4_0_R5 + ADDI_POS_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_escape_via_addi_derived_pointer_store(self):
        # Storing r1+8 still leaks an r1-derived pointer into public memory.
        original = (
            ADDI_NEG_64 + ADDI_R4_R1_8 + STW_R4_0_R5 + STB_R3_8 + ADDI_POS_64 + BLR
        )
        candidate = (
            ADDI_NEG_64 + ADDI_R4_R1_8 + STW_R4_0_R5 + ADDI_POS_64 + BLR
        )
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_escape_via_stmw_including_r1(self):
        original = ADDI_NEG_64 + STMW_R1_0_R5 + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + STMW_R1_0_R5 + ADDI_POS_64 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_escaped_alias_distinguishes_private_frame_bytes(self):
        # Same escape shape on both sides; only the private stb value differs.
        # Without escape detection this would spuriously prove equivalent.
        original = (
            ADDI_NEG_16 + LI_R3_1 + STB_R3_8 + ADDI_R4_R1_8 + STW_R4_0_R5
            + ADDI_POS_16 + BLR
        )
        candidate = (
            ADDI_NEG_16 + LI_R3_2 + STB_R3_8 + ADDI_R4_R1_8 + STW_R4_0_R5
            + ADDI_POS_16 + BLR
        )
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertIsNotNone(result.counterexample)

    def test_call_disables_private_stack_masking(self):
        # Relocated bl leaf must apply the call summary (internal bl targets are
        # CFG edges, not calls). After the call, a private-frame write is no
        # longer masked. Matched-callee SAT differences are reported as
        # inconclusive_abstraction rather than concrete inequivalence.
        contracts = {"leaf": CalleeContract(frozenset(), frozenset(), "test")}
        original = ADDI_NEG_64 + BL_LEAF + STB_R3_8 + ADDI_POS_64 + BLR
        candidate = ADDI_NEG_64 + BL_LEAF + ADDI_POS_64 + BLR
        result = prove_bytes_with_contract(
            original, candidate,
            callee_contracts=contracts,
            original_relocs=LEAF_RELOC,
            candidate_relocs=LEAF_RELOC,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertIsNotNone(result.counterexample)
        scope = result.memory_scope.to_dict()["private_stack"]
        self.assertIn("stack-escape-or-call", scope["original"]["disabled_reasons"])
        self.assertIn("stack-escape-or-call", scope["candidate"]["disabled_reasons"])

    def test_savegpr_helper_disables_private_stack_masking(self):
        # Fixed EABI helpers are calls: they must disable private-stack masking
        # even though they rewrite memory precisely instead of opaquely.
        helper = "_savegpr_31"
        contracts = {
            helper: CalleeContract(
                frozenset({"r11", "r31"}),
                frozenset({"memory"}),
                f"fixed-eabi-runtime-helper:{helper}",
            ),
        }
        # addi r11,r1,64 after frame setup puts r11 at entry_sp.
        ADDI_R11_R1_64 = "39610040"
        original = (
            ADDI_NEG_64 + ADDI_R11_R1_64 + BL_LEAF + STB_R3_8 + ADDI_POS_64 + BLR
        )
        candidate = (
            ADDI_NEG_64 + ADDI_R11_R1_64 + BL_LEAF + ADDI_POS_64 + BLR
        )
        # BL is at word index 2 (offset 8) after the two addi instructions.
        reloc = (
            RelocationRef(
                offset=8, relocation_type=R_PPC_REL24,
                symbol=helper, canonical_symbol=helper, addend=0,
            ),
        )
        result = prove_bytes_with_contract(
            original, candidate,
            callee_contracts=contracts,
            original_relocs=reloc,
            candidate_relocs=reloc,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertIsNotNone(result.counterexample)
        scope = result.memory_scope.to_dict()["private_stack"]
        self.assertIn("stack-escape-or-call", scope["original"]["disabled_reasons"])

    def test_savegpr_storing_r1_alias_marks_escape(self):
        # mr r31,r1 then _savegpr_31 stores an r1-derived value via the helper.
        helper = "_savegpr_31"
        contracts = {
            helper: CalleeContract(
                frozenset({"r11", "r31"}),
                frozenset({"memory"}),
                f"fixed-eabi-runtime-helper:{helper}",
            ),
        }
        MR_R31_R1 = "7c3f0b78"  # or r31,r1,r1
        # Point r11 at r5+4 so the save lands in caller-controlled public memory.
        ADDI_R11_R5_4 = "39650004"
        reloc = (
            RelocationRef(
                offset=12, relocation_type=R_PPC_REL24,
                symbol=helper, canonical_symbol=helper, addend=0,
            ),
        )
        # Frame, copy SP into r31, aim r11 at public, call helper, private stb.
        original = (
            ADDI_NEG_64 + MR_R31_R1 + ADDI_R11_R5_4 + BL_LEAF
            + STB_R3_8 + ADDI_POS_64 + BLR
        )
        candidate = (
            ADDI_NEG_64 + MR_R31_R1 + ADDI_R11_R5_4 + BL_LEAF
            + ADDI_POS_64 + BLR
        )
        result = prove_bytes_with_contract(
            original, candidate,
            callee_contracts=contracts,
            original_relocs=reloc,
            candidate_relocs=reloc,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertIsNotNone(result.counterexample)
        scope = result.memory_scope.to_dict()["private_stack"]
        self.assertIn("stack-escape-or-call", scope["original"]["disabled_reasons"])

    def test_multiple_paths_different_frames(self):
        # Two paths: r3==0 uses 64-byte frame; r3!=0 uses 16-byte frame.
        # Each side writes within its own frame on both paths.
        # Layout (word offsets):
        # 0 cmpwi; 4 beq -> path_64; 8.. path_16; 20 b -> end; 24.. path_64; 36 blr
        CMPWI_R3_0 = "2c030000"
        BEQ_PATH64 = "41820014"  # beq +20 -> word 6 (path_64)
        B_AFTER_PATH16 = "48000010"  # b +16 -> blr
        path_16 = ADDI_NEG_16 + STB_R3_8 + ADDI_POS_16
        path_64 = ADDI_NEG_64 + STB_R3_8 + ADDI_POS_64
        code = CMPWI_R3_0 + BEQ_PATH64 + path_16 + B_AFTER_PATH16 + path_64 + BLR
        result = prove_bytes(code, code)
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_upward_stack_produces_inconclusive_layout(self):
        # addi r1,r1,64 without prior frame setup moves r1 above entry_sp
        # (or wraps through zero). Both are rejected as impossible layouts.
        UP = "38210040"
        original = UP + STB_R3_8 + BLR
        candidate = UP + STB_R3_8 + BLR
        result = prove_bytes(original, candidate)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_LAYOUT)

    def test_upward_stack_wrap_cannot_mask_public_memory(self):
        # Adversarial: upward addi that wraps must not make [0, entry) private
        # and hide a divergent public store.
        UP = "38210040"  # addi r1,r1,64
        # stb r3,8(r5) — public store unrelated to the stack pointer.
        STB_R3_8_R5 = "98650008"
        original = UP + STB_R3_8_R5 + BLR
        candidate = UP + BLR
        result = prove_bytes(original, candidate)
        # Layout is impossible (upward/wrap), so fail closed — never EQUIVALENT.
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_LAYOUT)

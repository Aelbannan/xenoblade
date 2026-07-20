"""Virtual-call pattern recognition, provenance, pairing, and SCC tests."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.ir import Instruction, Opcode, RelocationRef
from tools.ppc_equivalence.jump_table import find_jump_table_candidates
from tools.ppc_equivalence.jump_table_obligations import (
    validate_indirect_targets_obligation,
    validate_readonly_image_obligation,
)
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.vtable import VirtualCallCandidate, find_virtual_call_candidates
from tools.ppc_equivalence.vtable_obligations import (
    SemanticCalleeCertificate,
    VirtualCallProofContext,
    VtableSlotWords,
    build_virtual_call_obligations,
    detect_callee_cycle,
    try_build_virtual_call_context,
    virtual_call_gate_reason,
    virtual_call_scc_status,
)
from tools.ppc_equivalence.vtable_pairing import (
    VirtualCallPairingError,
    pair_virtual_call_targets,
)
from tools.ppc_equivalence.vtable_provenance import (
    AUTHORIZED_VPTR_PROVENANCE,
    VptrProvenanceKind,
    classify_vptr_provenance,
    provenance_gate_reason,
)

CODE_BASE = 0x80104000

# lwz r12,0(r3); lwz r12,0x48(r12); mtctr r12; bctrl
_VIRTUAL_CALL_HEX = "81830000 818c0048 7d8903a6 4e800421"


def _decode(text: str) -> list[Instruction]:
    return decode_block(parse_hex(text), base=CODE_BASE, validate_with_capstone=False)


def _virtual_call_program() -> list[Instruction]:
    return _decode(_VIRTUAL_CALL_HEX)


def _cert(symbol: str = "CView::wkUpdate") -> SemanticCalleeCertificate:
    digest = "a" * 64
    return SemanticCalleeCertificate(
        target_id="view-wkupdate",
        symbol=symbol,
        certificate_sha256=digest,
        retail_sha256="b" * 64,
        candidate_sha256="c" * 64,
    )


class VirtualCallRecognizerTests(unittest.TestCase):
    def test_happy_path_real_encoding(self) -> None:
        instructions = _virtual_call_program()
        candidates = find_virtual_call_candidates(instructions)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "exact-pattern")
        self.assertEqual(candidate.this_reg, 3)
        self.assertEqual(candidate.this_offset, 0)
        self.assertEqual(candidate.vptr_reg, 12)
        self.assertEqual(candidate.slot_offset, 0x48)
        self.assertEqual(candidate.target_reg, 12)
        self.assertEqual(candidate.branch_pc, CODE_BASE + 12)
        self.assertEqual(candidate.instruction_indexes, (0, 1, 2, 3))

    def test_rejects_bctr_not_bctrl(self) -> None:
        instructions = _decode("81830000 818c0048 7d8903a6 4e800420")
        self.assertEqual(find_virtual_call_candidates(instructions), [])

    def test_rejects_unrelated_code(self) -> None:
        instructions = _decode("38600001 38600002 4e800020")
        self.assertEqual(find_virtual_call_candidates(instructions), [])

    def test_rejects_register_mismatch(self) -> None:
        program = [
            Instruction(CODE_BASE, 0, Opcode.LWZ, (12, 3, 0)),
            Instruction(CODE_BASE + 4, 0, Opcode.LWZ, (11, 12, 0x48)),
            Instruction(CODE_BASE + 8, 0, Opcode.MTSPR, (12, 9)),
            Instruction(CODE_BASE + 12, 0, Opcode.BCCTR, (20, 0, 0), link=True),
        ]
        self.assertEqual(find_virtual_call_candidates(program), [])

    def test_rejects_unmodeled_vptr_clobber(self) -> None:
        program = [
            Instruction(CODE_BASE, 0, Opcode.LWZ, (12, 3, 0)),
            Instruction(CODE_BASE + 4, 0, Opcode.ADDI, (12, 12, 4)),
            Instruction(CODE_BASE + 8, 0, Opcode.LWZ, (12, 12, 0x48)),
            Instruction(CODE_BASE + 12, 0, Opcode.MTSPR, (12, 9)),
            Instruction(CODE_BASE + 16, 0, Opcode.BCCTR, (20, 0, 0), link=True),
        ]
        self.assertEqual(find_virtual_call_candidates(program), [])

    def test_rejects_unmodeled_target_clobber(self) -> None:
        program = [
            Instruction(CODE_BASE, 0, Opcode.LWZ, (12, 3, 0)),
            Instruction(CODE_BASE + 4, 0, Opcode.LWZ, (12, 12, 0x48)),
            Instruction(CODE_BASE + 8, 0, Opcode.ADDI, (12, 12, 0)),
            Instruction(CODE_BASE + 12, 0, Opcode.MTSPR, (12, 9)),
            Instruction(CODE_BASE + 16, 0, Opcode.BCCTR, (20, 0, 0), link=True),
        ]
        self.assertEqual(find_virtual_call_candidates(program), [])

    def test_jump_table_stays_lk_zero(self) -> None:
        jump = _decode("28000008 5400103a 7c63002e 7c6903a6 4e800420")
        self.assertEqual(len(find_jump_table_candidates(jump)), 1)
        linked = _virtual_call_program()
        self.assertEqual(find_jump_table_candidates(linked), [])


class VptrProvenanceTests(unittest.TestCase):
    def test_materialized_in_function_object_field(self) -> None:
        instructions = _virtual_call_program()
        candidate = find_virtual_call_candidates(instructions)[0]
        provenance = classify_vptr_provenance(instructions, candidate)
        self.assertEqual(provenance.kind, VptrProvenanceKind.MATERIALIZED_IN_FUNCTION)
        self.assertTrue(provenance.authorized)

    def test_readonly_global_with_relocation(self) -> None:
        reloc = RelocationRef(
            offset=0,
            relocation_type=1,
            symbol="__vt__5CView",
            canonical_symbol="__vt__5CView",
            addend=0,
        )
        program = [
            Instruction(
                CODE_BASE, 0, Opcode.LWZ, (12, 13, 0), relocation=reloc,
            ),
            Instruction(CODE_BASE + 4, 0, Opcode.LWZ, (12, 12, 0x48)),
            Instruction(CODE_BASE + 8, 0, Opcode.MTSPR, (12, 9)),
            Instruction(CODE_BASE + 12, 0, Opcode.BCCTR, (20, 0, 0), link=True),
        ]
        candidate = find_virtual_call_candidates(program)[0]
        provenance = classify_vptr_provenance(program, candidate)
        self.assertEqual(provenance.kind, VptrProvenanceKind.READONLY_GLOBAL_OBJECT)
        self.assertEqual(provenance.global_symbol, "__vt__5CView")

    def test_unknown_without_evidence(self) -> None:
        program = [
            Instruction(CODE_BASE, 0, Opcode.LWZ, (12, 0, 0x1000)),
            Instruction(CODE_BASE + 4, 0, Opcode.LWZ, (12, 12, 0x48)),
            Instruction(CODE_BASE + 8, 0, Opcode.MTSPR, (12, 9)),
            Instruction(CODE_BASE + 12, 0, Opcode.BCCTR, (20, 0, 0), link=True),
        ]
        candidate = find_virtual_call_candidates(program)[0]
        provenance = classify_vptr_provenance(program, candidate)
        self.assertEqual(provenance.kind, VptrProvenanceKind.UNKNOWN)
        self.assertIsNotNone(provenance_gate_reason(provenance))

    def test_certified_input_not_authorized(self) -> None:
        from tools.ppc_equivalence.vtable_provenance import CertifiedInputInvariant

        instructions = _virtual_call_program()
        candidate = find_virtual_call_candidates(instructions)[0]
        provenance = classify_vptr_provenance(
            instructions,
            candidate,
            certified_input=CertifiedInputInvariant(
                parameter="vptr",
                digest="d" * 64,
            ),
        )
        self.assertEqual(provenance.kind, VptrProvenanceKind.CERTIFIED_INPUT_INVARIANT)
        self.assertNotIn(provenance.kind, AUTHORIZED_VPTR_PROVENANCE)
        self.assertIn("not authorized", provenance_gate_reason(provenance) or "")


class VirtualCallPairingTests(unittest.TestCase):
    def test_pairs_by_shared_symbol(self) -> None:
        pairing = pair_virtual_call_targets(
            slot_offset=0x48,
            original_pc=0x80101000,
            candidate_pc=0x80202000,
            original_symbol="CView::wkUpdate",
            candidate_symbol="CView::wkUpdate",
        )
        self.assertEqual(pairing.cases[0].identity, "CView::wkUpdate")
        self.assertEqual(pairing.cases[0].original_pc, 0x80101000)
        self.assertEqual(pairing.cases[0].candidate_pc, 0x80202000)

    def test_pairs_by_slot_when_symbols_absent(self) -> None:
        pairing = pair_virtual_call_targets(
            slot_offset=0x48,
            original_pc=0x80101000,
            candidate_pc=0x80202000,
        )
        self.assertEqual(pairing.cases[0].identity, "slot-0x48")

    def test_rejects_symbol_mismatch(self) -> None:
        with self.assertRaises(VirtualCallPairingError):
            pair_virtual_call_targets(
                slot_offset=0x48,
                original_pc=0x80101000,
                candidate_pc=0x80202000,
                original_symbol="CView::wkUpdate",
                candidate_symbol="CView::draw",
            )


class VirtualCallClosureTests(unittest.TestCase):
    def _context(self, *, symbol: str = "CView::wkUpdate") -> VirtualCallProofContext:
        instructions = _virtual_call_program()
        candidate = find_virtual_call_candidates(instructions)[0]
        provenance = classify_vptr_provenance(instructions, candidate)
        pairing = pair_virtual_call_targets(
            slot_offset=candidate.slot_offset,
            original_pc=0x80105000,
            candidate_pc=0x80106000,
            original_symbol=symbol,
            candidate_symbol=symbol,
        )
        slot = VtableSlotWords(
            base=0x8056B5E0 + candidate.slot_offset,
            words=(0x80105000,),
            slot_offset=candidate.slot_offset,
            source="test-vtable",
        )
        return VirtualCallProofContext(
            candidate=candidate,
            slot=slot,
            pairing=pairing,
            provenance=provenance,
            branch_pc=candidate.branch_pc,
            callee_certificate=_cert(symbol),
        )

    def test_builds_valid_obligations(self) -> None:
        context = self._context()
        address_space, indirect_targets = build_virtual_call_obligations(context)
        self.assertIsNone(validate_readonly_image_obligation(address_space))
        self.assertIsNone(validate_indirect_targets_obligation(indirect_targets))
        self.assertEqual(address_space["slot_offset"], 0x48)
        self.assertEqual(indirect_targets["targets"][0]["identity"], "CView::wkUpdate")
        self.assertEqual(indirect_targets["algorithm"], "virtual-call-enumerated-v1")

    def test_try_build_requires_certificate(self) -> None:
        instructions = _virtual_call_program()
        branch_pc = CODE_BASE + 12
        result = try_build_virtual_call_context(
            instructions,
            slot_word=0x80105000,
            slot_base=0x8056B5E0 + 0x48,
            branch_pc=branch_pc,
            source="test",
            original_pc=0x80105000,
            candidate_pc=0x80106000,
            original_symbol="CView::wkUpdate",
            candidate_symbol="CView::wkUpdate",
        )
        self.assertEqual(result, "missing semantic callee certificate")

    def test_gate_reason_detects_virtual_call(self) -> None:
        insns = _virtual_call_program()
        self.assertIsNotNone(virtual_call_gate_reason(insns, insns))
        leaf = _decode("38630004 4e800020")
        self.assertIsNone(virtual_call_gate_reason(leaf, leaf))

    def test_try_auto_without_premises_returns_none(self) -> None:
        from tools.ppc_equivalence.vtable_obligations import try_auto_virtual_call_context

        insns = _virtual_call_program()
        self.assertIsNone(try_auto_virtual_call_context(insns, insns))

    def test_engine_gate_demotes_equivalent_without_context(self) -> None:
        from tools.ppc_equivalence.contract import make_contract
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        # Leaf program without virtual call stays eligible for EQUIVALENT.
        leaf = _decode("38630004 4e800020")
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=5_000)
        leaf_result = check_equivalence(
            leaf, leaf, contract, original_hex="00", candidate_hex="00",
        )
        self.assertEqual(leaf_result.status, ProofStatus.EQUIVALENT)

    def test_engine_scc_returns_unvalidated_callee(self) -> None:
        from tools.ppc_equivalence.contract import make_contract
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        context = self._context()
        edges = {
            "CView::wkUpdate": frozenset({"CView::wkUpdate"}),
        }
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=5_000)
        # Minimal leaf bodies — SCC check runs before CFG.
        leaf = _decode("38630004 4e800020")
        result = check_equivalence(
            leaf,
            leaf,
            contract,
            original_hex="00",
            candidate_hex="00",
            virtual_call=context,
            callee_edges=edges,
        )
        self.assertEqual(
            result.status,
            ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE,
            result.unsupported,
        )


class VirtualCallSccTests(unittest.TestCase):
    def test_detects_cycle(self) -> None:
        edges = {
            "A": frozenset({"B"}),
            "B": frozenset({"C"}),
            "C": frozenset({"A"}),
        }
        cycle = detect_callee_cycle(edges, root="A")
        self.assertIsNotNone(cycle)
        self.assertEqual(virtual_call_scc_status(edges, root="A"),
                         ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE)

    def test_acyclic_returns_none(self) -> None:
        edges = {
            "A": frozenset({"B"}),
            "B": frozenset({"C"}),
        }
        self.assertIsNone(detect_callee_cycle(edges, root="A"))
        self.assertIsNone(virtual_call_scc_status(edges, root="A"))


if __name__ == "__main__":
    unittest.main()

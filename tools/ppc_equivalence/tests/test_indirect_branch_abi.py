"""Indirect-branch / ``bctr`` terminal ABI: live outgoing args stay compared."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.abi_shape import AbiShape
from tools.ppc_equivalence.contract import (
    INDIRECT_BRANCH_OMITTED_OBSERVABLES,
    make_contract,
    observables_for_exit,
)
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import automatic_live_out


def _decode(hex_words: str):
    return decode_block(parse_hex(hex_words), validate_with_capstone=False)


def _prove(original_hex: str, candidate_hex: str, *, preset: str = "auto"):
    original = _decode(original_hex)
    candidate = _decode(candidate_hex)
    contract = make_contract(
        preset=preset,
        observe=None,
        timeout_ms=5_000,
        original_live_out=tuple(),
        candidate_live_out=tuple(),
    )
    if preset == "auto":
        from tools.ppc_equivalence.semantics import automatic_live_out

        contract = make_contract(
            preset="auto",
            observe=None,
            timeout_ms=5_000,
            original_live_out=automatic_live_out(original),
            candidate_live_out=automatic_live_out(candidate),
        )
    return check_equivalence(
        original,
        candidate,
        contract,
        original_hex=original_hex.replace(" ", ""),
        candidate_hex=candidate_hex.replace(" ", ""),
    )


class ObservablesForExitTests(unittest.TestCase):
    def test_indirect_branch_keeps_r4_and_f1_under_ppc_eabi(self) -> None:
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=1_000)
        filtered = observables_for_exit(contract, "indirect-branch")
        names = {item.name for item in filtered}
        # No exit-kind omission: r4/f1 are live outgoing args at a tail call.
        self.assertEqual(INDIRECT_BRANCH_OMITTED_OBSERVABLES, frozenset())
        self.assertIn("r4", names)
        self.assertIn("f1", names)
        self.assertIn("r3", names)
        self.assertIn("memory", names)
        self.assertIn("r13", names)

    def test_return_keeps_r4(self) -> None:
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=1_000)
        names = {item.name for item in observables_for_exit(contract, "return")}
        self.assertIn("r4", names)
        self.assertIn("f1", names)

    def test_strict_keeps_r4_on_indirect_branch(self) -> None:
        contract = make_contract(preset="strict", observe=None, timeout_ms=1_000)
        names = {item.name for item in observables_for_exit(contract, "indirect-branch")}
        self.assertIn("r4", names)

    def test_manual_keeps_explicit_r4(self) -> None:
        contract = make_contract(
            preset=None, observe=("r3", "r4", "memory"), timeout_ms=1_000,
        )
        names = {item.name for item in observables_for_exit(contract, "indirect-branch")}
        self.assertEqual(names, {"r3", "r4", "memory"})


class IndirectBranchAbiProofTests(unittest.TestCase):
    # lwz r12,0(r3); lwz r12,0x20(r12); mtctr r12; bctr
    _RETAIL_R12 = "81830000 818c0020 7d8903a6 4e800420"
    # lwz r5,0(r3); lwz r5,0x20(r5); mtctr r5; bctr  (non-observed scratch)
    _CAND_R5 = "80a30000 80a50020 7ca903a6 4e800420"
    # lwz r4,0(r3); lwz r4,0x20(r4); mtctr r4; bctr  (clobbers live arg)
    _CAND_R4 = "80830000 80840020 7c8903a6 4e800420"
    # same r12 chain, wrong slot 0x24
    _CAND_WRONG_SLOT = "81830000 818c0024 7d8903a6 4e800420"

    def test_r12_vs_r5_scratch_same_slot_is_equivalent(self) -> None:
        """Non-observed volatile scratch is fine when outgoing args match."""
        result = _prove(self._RETAIL_R12, self._CAND_R5, preset="auto")
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)

    def test_r12_vs_r4_scratch_same_slot_is_not_equivalent(self) -> None:
        """Using r4 as CTR scratch destroys the live outgoing integer arg."""
        result = _prove(self._RETAIL_R12, self._CAND_R4, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r4")

    def test_wrong_vtable_slot_still_not_equivalent(self) -> None:
        result = _prove(self._RETAIL_R12, self._CAND_WRONG_SLOT, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "exit.target")

    def test_return_path_still_observes_r4(self) -> None:
        # li r4,1; blr  vs  li r4,2; blr
        result = _prove("38800001 4e800020", "38800002 4e800020", preset="ppc-eabi")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r4")

    def test_this_adjust_thunk_r12_vs_r5(self) -> None:
        # lwz r3,0x8c(r3); lwz r12,0(r3); lwz r12,0x20(r12); mtctr r12; bctr
        retail = "8063008c 81830000 818c0020 7d8903a6 4e800420"
        # lwz r3,0x8c(r3); lwz r5,0(r3); lwz r5,0x20(r5); mtctr r5; bctr
        cand = "8063008c 80a30000 80a50020 7ca903a6 4e800420"
        result = _prove(retail, cand, preset="auto")
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)

    def test_this_adjust_thunk_r12_vs_r4_not_equivalent(self) -> None:
        retail = "8063008c 81830000 818c0020 7d8903a6 4e800420"
        cand = "8063008c 80830000 80840020 7c8903a6 4e800420"
        result = _prove(retail, cand, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r4")

    def test_explicit_abi_shape_allows_r4_scratch_for_single_arg(self) -> None:
        """Opt-in AbiShape(outgoing_gpr_args=1) drops r4 at indirect-branch."""
        original = _decode(self._RETAIL_R12)
        candidate = _decode(self._CAND_R4)
        shape = AbiShape(outgoing_gpr_args=1, outgoing_fpr_args=0, source="explicit")
        contract = make_contract(
            preset="auto",
            observe=None,
            timeout_ms=5_000,
            original_live_out=automatic_live_out(original),
            candidate_live_out=automatic_live_out(candidate),
            abi_shape=shape,
        )
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex=self._RETAIL_R12.replace(" ", ""),
            candidate_hex=self._CAND_R4.replace(" ", ""),
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)

    def test_abi_shape_none_keeps_default_indirect_observation(self) -> None:
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=1_000)
        self.assertIsNone(contract.abi_shape)
        names = {
            item.name for item in observables_for_exit(contract, "indirect-branch")
        }
        self.assertIn("r4", names)
        self.assertIn("f1", names)


class UnresolvedIndirectExitGateTests(unittest.TestCase):
    """M1: broaden the indirect-exit gate beyond the canonical jump table."""

    # mtctr r3; bctr — a non-canonical dispatcher: the CTR source is an input,
    # with no adjacent lwzx jump-table shape and no vtable-thunk load chain.
    _NONCANONICAL_BCTR = "7c6903a6 4e800420"

    def test_noncanonical_bcctr_without_context_is_gated(self) -> None:
        # Both sides are byte-identical, so the solver would find the terminals
        # equivalent; the gate must demote before that becomes EQUIVALENT.
        result = _prove(self._NONCANONICAL_BCTR, self._NONCANONICAL_BCTR, preset="auto")
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertTrue(
            any(
                "unresolved indirect-branch/call-indirect" in item
                for item in (result.unsupported or [])
            )
            or any(
                "unresolved indirect-branch/call-indirect" in item
                for item in (result.warnings or [])
            ),
        )

    def test_gate_reason_fires_on_indirect_branch_terminal(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import (
            unresolved_indirect_exit_gate_reason,
        )

        class _Term:
            def __init__(self, kind: str) -> None:
                self.exit_kind = kind

        reason = unresolved_indirect_exit_gate_reason(
            [_Term("indirect-branch")], [_Term("return")],
        )
        self.assertIsNotNone(reason)
        self.assertIn("unresolved indirect-branch/call-indirect", reason or "")

    def test_matching_virtual_thunk_exempt_from_unresolved_gate(self) -> None:
        from tools.ppc_equivalence.decoder import decode_block, parse_hex
        from tools.ppc_equivalence.jump_table_obligations import (
            unresolved_indirect_exit_gate_reason,
        )

        class _Term:
            def __init__(self, kind: str) -> None:
                self.exit_kind = kind

        # An indirect-branch terminal produced by a matching thunk on both sides
        # is exempt: exit.target equality is sufficient without a proof context.
        thunk = decode_block(
            parse_hex("81830000 818c0020 7d8903a6 4e800420"),
            validate_with_capstone=False,
        )
        reason = unresolved_indirect_exit_gate_reason(
            [_Term("indirect-branch")],
            [_Term("indirect-branch")],
            original=thunk,
            candidate=thunk,
        )
        self.assertIsNone(reason)

        # Without the instruction sequences the exemption cannot fire.
        reason_no_insn = unresolved_indirect_exit_gate_reason(
            [_Term("indirect-branch")], [_Term("indirect-branch")],
        )
        self.assertIsNotNone(reason_no_insn)


if __name__ == "__main__":
    unittest.main()

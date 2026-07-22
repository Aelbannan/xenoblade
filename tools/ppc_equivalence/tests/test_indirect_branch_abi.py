"""Exit-kind ABI filtering for ``bctr`` / ``indirect-branch`` terminals."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import (
    INDIRECT_BRANCH_OMITTED_OBSERVABLES,
    make_contract,
    observables_for_exit,
    parse_observables,
)
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.result import ProofStatus


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
    def test_indirect_branch_omits_r4_and_f1_under_ppc_eabi(self) -> None:
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=1_000)
        filtered = observables_for_exit(contract, "indirect-branch")
        names = {item.name for item in filtered}
        self.assertTrue(INDIRECT_BRANCH_OMITTED_OBSERVABLES.isdisjoint(names))
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
    # lwz r4,0(r3); lwz r4,0x20(r4); mtctr r4; bctr
    _CAND_R4 = "80830000 80840020 7c8903a6 4e800420"
    # same chain, wrong slot 0x24
    _CAND_WRONG_SLOT = "80830000 80840024 7c8903a6 4e800420"

    def test_r12_vs_r4_scratch_same_slot_is_equivalent(self) -> None:
        result = _prove(self._RETAIL_R12, self._CAND_R4, preset="auto")
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)

    def test_wrong_vtable_slot_still_not_equivalent(self) -> None:
        result = _prove(self._RETAIL_R12, self._CAND_WRONG_SLOT, preset="auto")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "exit.target")

    def test_return_path_still_observes_r4(self) -> None:
        # li r4,1; blr  vs  li r4,2; blr
        result = _prove("38800001 4e800020", "38800002 4e800020", preset="ppc-eabi")
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r4")

    def test_this_adjust_thunk_r12_vs_r4(self) -> None:
        # lwz r3,0x8c(r3); lwz r12,0(r3); lwz r12,0x20(r12); mtctr r12; bctr
        retail = "8063008c 81830000 818c0020 7d8903a6 4e800420"
        # lwz r3,0x8c(r3); lwz r4,0(r3); lwz r4,0x20(r4); mtctr r4; bctr
        cand = "8063008c 80830000 80840020 7c8903a6 4e800420"
        result = _prove(retail, cand, preset="auto")
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)


if __name__ == "__main__":
    unittest.main()

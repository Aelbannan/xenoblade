"""Process-pool isolation for Z3 entry points."""

from __future__ import annotations

import os
import threading
import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.engine import check_equivalence, validate_callee_contract
from tools.ppc_equivalence.process_pool import (
    configure_for_tests,
    should_isolate,
    shutdown_pool,
)
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import CalleeContract


def _blr() -> list:
    return decode_block(bytes.fromhex("4e800020"), 0x80000000, validate_with_capstone=False)


class ProcessPoolModeTests(unittest.TestCase):
    def tearDown(self) -> None:
        configure_for_tests(enabled=False)
        os.environ.pop("PPC_EQUIV_PROCESS_POOL", None)
        os.environ.pop("PPC_EQUIV_PROCESS_POOL_WORKERS", None)
        shutdown_pool(wait=True)

    def test_auto_isolates_off_main_thread(self) -> None:
        os.environ["PPC_EQUIV_PROCESS_POOL"] = "auto"
        self.assertFalse(should_isolate())
        seen: list[bool] = []

        def worker() -> None:
            seen.append(should_isolate())

        thread = threading.Thread(target=worker)
        thread.start()
        thread.join()
        self.assertEqual(seen, [True])

    def test_off_never_isolates(self) -> None:
        os.environ["PPC_EQUIV_PROCESS_POOL"] = "off"
        seen: list[bool] = []

        def worker() -> None:
            seen.append(should_isolate())

        thread = threading.Thread(target=worker)
        thread.start()
        thread.join()
        self.assertEqual(seen, [False])


class ProcessPoolProofTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        configure_for_tests(enabled=True, workers=2)

    @classmethod
    def tearDownClass(cls) -> None:
        configure_for_tests(enabled=False)
        os.environ.pop("PPC_EQUIV_PROCESS_POOL", None)
        os.environ.pop("PPC_EQUIV_PROCESS_POOL_WORKERS", None)
        shutdown_pool(wait=True)

    def test_identical_blr_equivalent_via_pool(self) -> None:
        insns = _blr()
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=5_000)
        result = check_equivalence(
            insns,
            insns,
            contract,
            original_hex="4e800020",
            candidate_hex="4e800020",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)

    def test_threaded_parallel_proofs_do_not_segfault(self) -> None:
        insns = _blr()
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=5_000)
        errors: list[BaseException] = []
        results: list[ProofStatus] = []

        def worker() -> None:
            try:
                result = check_equivalence(
                    insns,
                    insns,
                    contract,
                    original_hex="4e800020",
                    candidate_hex="4e800020",
                )
                results.append(result.status)
            except BaseException as exc:  # noqa: BLE001 — surface any crash path
                errors.append(exc)

        threads = [threading.Thread(target=worker) for _ in range(8)]
        for thread in threads:
            thread.start()
        for thread in threads:
            thread.join()

        self.assertEqual(errors, [])
        self.assertEqual(results, [ProofStatus.EQUIVALENT] * 8)

    def test_validate_callee_contract_via_pool(self) -> None:
        insns = _blr()
        validation = validate_callee_contract(insns, CalleeContract.opaque_eabi())
        self.assertTrue(validation.valid)


if __name__ == "__main__":
    unittest.main()

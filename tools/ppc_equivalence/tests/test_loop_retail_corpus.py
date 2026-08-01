"""Retail-corpus loop recognition regression (doc 28 / doc 30 Phase B).

Gated on ``build/us/asm`` being present (CI without the retail tree skips).
Verifies that the documented retail loop shapes are recognized by the engine
finders, that skip guards discharge, and that the bounded-remainder plans are
built. Full-function SMT self-equivalence is covered by synthetic fixtures;
retail functions are large and call-heavy, so the corpus test stays at
recognition + discharge + plan-map level.
"""

from __future__ import annotations

import os
import re
import unittest
from pathlib import Path

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.engine import _symbolic_initial
from tools.ppc_equivalence.loop_summary import find_ctr_affine_loop_candidates
from tools.ppc_equivalence.memory_loop import (
    build_memory_loop_plan_map,
    find_constant_stride_store_loops,
)
from tools.ppc_equivalence.semantics import SymbolicOps
from tools.ppc_equivalence.skip_guard import (
    SkipGuardInfo,
    discharge_skip_guard,
    find_mtctr_with_guard,
)
from tools.ppc_equivalence.trip_expression import recognize_trip_expr

_ASM_ROOT = Path("build/us/asm")
_INSN_RE = re.compile(
    r"^/\*\s*(?P<address>[0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s+"
    r"(?P<bytes>(?:[0-9A-Fa-f]{2}\s+){3}[0-9A-Fa-f]{2})\s*\*/\s*"
    r"(?P<mnemonic>\S+)",
)
_FN_RE = re.compile(r'^\.fn\s+\"?([^\",\s]+)\"?\s*,')

# Documented retail loops (doc 28 table) that Phase B must recognize.
RETAIL_LOOPS: tuple[tuple[str, str, int, str], ...] = (
    ("RVL_SDK/src/revolution/ax/AXVPB.s", "__AXVPBInitCommon", 0x802D7A38, "memory"),
    ("RVL_SDK/src/revolution/ax/AXVPB.s", "__AXVPBInitCommon", 0x802D7A98, "memory"),
    ("RVL_SDK/src/revolution/ax/AXVPB.s", "__AXVPBInitCommon", 0x802D7B00, "memory"),
    ("RVL_SDK/src/revolution/os/OS.s", "OSExceptionInit", 0x80355690, "memory"),
    ("RVL_SDK/src/revolution/bte/stack/hcic/hcicmds.s", "btsnd_hcic_pin_code_req_reply", 0x802F4078, "memory"),
    ("nw4r/src/snd/snd_Util.s", "CalcPitchRatio__Q44nw4r3snd6detail4UtilFi", 0x8042B774, "ctr-affine"),
    ("nw4r/src/snd/snd_Util.s", "CalcPitchRatio__Q44nw4r3snd6detail4UtilFi", 0x8042B7B4, "ctr-affine"),
)


def _load_function(path: Path, symbol: str) -> list[tuple[int, int]]:
    out: list[tuple[int, int]] = []
    in_fn = False
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        stripped = line.strip()
        fn_match = _FN_RE.match(stripped)
        if fn_match:
            in_fn = fn_match.group(1) == symbol
            continue
        if re.match(r"^\.endfn", stripped):
            if in_fn:
                break
            continue
        if not in_fn:
            continue
        insn_match = _INSN_RE.match(stripped)
        if insn_match:
            out.append(
                (
                    int(insn_match.group("address"), 16),
                    int(insn_match.group("bytes").replace(" ", ""), 16),
                ),
            )
    return out


def _decode(path: Path, symbol: str):
    words = _load_function(path, symbol)
    if not words:
        return None
    try:
        return decode_block(
            b"".join(word.to_bytes(4, "big") for _addr, word in words),
            words[0][0],
            validate_with_capstone=False,
        )
    except Exception:
        return None


@unittest.skipUnless(
    _ASM_ROOT.is_dir(),
    "build/us/asm retail tree not present",
)
class RetailLoopCorpusTests(unittest.TestCase):
    def _assert_guard_discharges(self, instructions, header_pc: int) -> None:
        header = next(
            i for i, insn in enumerate(instructions) if insn.address == header_pc
        )
        mtctr_index, guard = find_mtctr_with_guard(instructions, header)
        self.assertIsNotNone(mtctr_index)
        self.assertIsNotNone(guard)
        assert guard is not None
        expr, _notes = recognize_trip_expr(
            instructions, mtctr_index, guard.trip_reg,
        )
        self.assertIsNotNone(expr)
        ops = SymbolicOps()
        initial = _symbolic_initial(ops)
        discharge = discharge_skip_guard(guard, expr, initial.gpr, ops)
        self.assertIsNotNone(discharge)
        assert discharge is not None
        self.assertTrue(
            discharge.all_unsat(),
            f"skip guard at 0x{header_pc:08X} did not discharge",
        )

    def test_documented_memory_loops_recognized_with_guards(self) -> None:
        seen: set[int] = set()
        for relative, symbol, header_pc, _kind in RETAIL_LOOPS:
            if _kind != "memory":
                continue
            path = _ASM_ROOT / relative
            if not path.exists():
                self.skipTest(f"missing {path}")
            instructions = _decode(path, symbol)
            self.assertIsNotNone(instructions, f"{symbol} did not decode")
            assert instructions is not None
            loops = find_constant_stride_store_loops(instructions)
            matches = [loop for loop in loops if loop.header_pc == header_pc]
            self.assertEqual(
                len(matches), 1,
                f"{symbol} @0x{header_pc:08X} not recognized",
            )
            loop = matches[0]
            self.assertEqual(loop.confidence, "bounded-remainder")
            self.assertIsNotNone(loop.skip_guard)
            self.assertEqual(loop.zero_guard, "skip-branch")
            seen.add(header_pc)
            plans = build_memory_loop_plan_map(instructions)
            self.assertIn(header_pc, plans)
            self._assert_guard_discharges(instructions, header_pc)
        self.assertEqual(len(seen), 5)

    def test_documented_ctr_affine_loops_recorded(self) -> None:
        seen: set[int] = set()
        for relative, symbol, header_pc, _kind in RETAIL_LOOPS:
            if _kind != "ctr-affine":
                continue
            path = _ASM_ROOT / relative
            if not path.exists():
                self.skipTest(f"missing {path}")
            instructions = _decode(path, symbol)
            self.assertIsNotNone(instructions, f"{symbol} did not decode")
            assert instructions is not None
            candidates = find_ctr_affine_loop_candidates(instructions)
            matches = [
                candidate for candidate in candidates
                if candidate.header_pc == header_pc
            ]
            self.assertEqual(
                len(matches), 1,
                f"{symbol} @0x{header_pc:08X} not recognized",
            )
            candidate = matches[0]
            self.assertEqual(candidate.confidence, "symbolic-trip")
            self.assertIsNotNone(candidate.skip_guard)
            self.assertIsNotNone(candidate.trip_expr)
            seen.add(header_pc)
            self._assert_guard_discharges(instructions, header_pc)
        self.assertEqual(len(seen), 2)


if __name__ == "__main__":
    unittest.main()

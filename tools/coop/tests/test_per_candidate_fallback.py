"""Per-candidate fallback: one bad candidate must not kill its unit.

When a unit's combined apply fails verification, the driver retries that
unit's candidates ONE AT A TIME under the same strict byte-identity gate
(address/size/mismatch/match% identical; name only), keeps each passing
candidate, and reverts only the failing one; the report carries
per-candidate PASS/SKIP with reasons.
"""
from __future__ import annotations

import argparse
import json
import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

from tools.coop import memberize_bulk as mb


OLD_A = "Alpha_Do"
OLD_B = "Beta_Do"
ADDR_A = 0x1000
ADDR_B = 0x2000
SIZE = 0x40


def _rec(symbol, decomp=None, retail=SIZE, decomp_size=None, mis=0, present=True):
    return {
        "symbol": symbol,
        "decomp_symbol": decomp if decomp is not None else symbol,
        "present": present,
        "retail_size": retail,
        "decomp_size": decomp_size if decomp_size is not None else retail,
        "mismatch": mis,
    }


def _write_two_candidate_fixture(root: Path) -> None:
    for reg in ("us", "eu", "jp"):
        (root / "config" / reg).mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    sym_line = (f"{OLD_A} = .text:0x80001000; // type:function size:0x40\n"
                f"{OLD_B} = .text:0x80002000; // type:function size:0x40\n")
    for reg in ("us", "eu", "jp"):
        (root / "config" / reg / "symbols.txt").write_text(sym_line, encoding="utf-8")
    (root / "src" / "unit" / "Owner.hpp").write_text(
        "#pragma once\nclass Alpha {\npublic:\n    void Existing();\n};\n"
        "class Beta {\npublic:\n    void Existing();\n};\n",
        encoding="utf-8")
    (root / "src" / "unit" / "DefA.cpp").write_text(
        '#include "Owner.hpp"\n'
        f'extern "C" void {OLD_A}(Alpha* self, int x);\n'
        f'void {OLD_A}(Alpha* self, int x) {{ (void)self; (void)x; }}\n',
        encoding="utf-8")
    (root / "src" / "unit" / "DefB.cpp").write_text(
        '#include "Owner.hpp"\n'
        f'extern "C" void {OLD_B}(Beta* self, int x);\n'
        f'void {OLD_B}(Beta* self, int x) {{ (void)self; (void)x; }}\n',
        encoding="utf-8")
    (root / "src" / "other" / "Caller.cpp").write_text(
        '#include "Owner.hpp"\n'
        f'void run() {{ {OLD_A}(a, 1); {OLD_B}(b, 2); }}\n',
        encoding="utf-8")
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")


def _plan_json(root: Path) -> Path:
    scan = [
        {"file": "src/unit/DefA.cpp", "line": 2, "name": OLD_A,
         "kind": "decl", "self_style": True},
        {"file": "src/unit/DefB.cpp", "line": 2, "name": OLD_B,
         "kind": "decl", "self_style": True},
    ]
    data = mb.plan_all(root, scan)
    assert data["summary"]["clean"] == 2, data
    pj = root / "plan.json"
    pj.write_text(json.dumps(data), encoding="utf-8")
    return pj


def _make_snap(*, bad: str | None, mismatch_only: bool = False,
               unobserved: str | None = None):
    """Stateful test-seam snapshotter.

    Odd calls (before) return the all-Old baseline; even calls (after)
    apply the requested *aliases*: good renames keep identical sizes, the
    *bad* old gets a changed size (or mismatch), and the *unobserved* old
    stays Old (rename never took effect).
    """
    calls: list[dict] = []

    def snap(_root: Path, _unit: str, aliases=None):
        aliases = dict(aliases or {})
        calls.append(dict(aliases))
        if len(calls) % 2 == 1:
            return {ADDR_A: _rec(OLD_A), ADDR_B: _rec(OLD_B)}
        out = {}
        for addr, old in ((ADDR_A, OLD_A), (ADDR_B, OLD_B)):
            if old in aliases:
                new = aliases[old]
                if old == unobserved:
                    out[addr] = _rec(old)  # old still present
                elif old == bad:
                    if mismatch_only:
                        out[addr] = _rec(old, new, mis=4)
                    else:
                        out[addr] = _rec(old, new, retail=SIZE,
                                         decomp_size=SIZE + 8, mis=4)
                else:
                    out[addr] = _rec(old, new)
            else:
                out[addr] = _rec(old)
        return out

    snap.calls = calls  # type: ignore[attr-defined]
    return snap


def _run_apply(root: Path, pj: Path, snap, **kw) -> tuple[int, dict]:
    out_json = root / "apply_report.json"
    ns = argparse.Namespace(
        plan_json=str(pj), candidate=kw.get("candidate", []), unit=None,
        limit=None, execute=True, dry_run=False,
        json=str(out_json), report=None, region="us",
        _snapshotter=snap, _applier=None, _no_post=True)
    rc = mb.cmd_apply(ns, root)
    report = json.loads(out_json.read_text(encoding="utf-8"))
    return rc, report


class FallbackTest(unittest.TestCase):
    def test_combined_fail_falls_back_keeps_good(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_two_candidate_fixture(root)
            pj = _plan_json(root)
            rc, report = _run_apply(root, pj, _make_snap(bad=OLD_B))
            # PARTIAL exits nonzero (skips need attention) but keeps the good one.
            self.assertEqual(rc, 1)
            units = report["units"]
            self.assertEqual(len(units), 1)
            unit = next(iter(units.values()))
            self.assertEqual(unit["status"], "PARTIAL")
            self.assertEqual(unit["functions"], [OLD_A])
            self.assertEqual(len(unit["skipped"]), 1)
            self.assertEqual(unit["skipped"][0]["old"], OLD_B)
            by_old = {c["old"]: c for c in unit["candidates"]}
            self.assertEqual(by_old[OLD_A]["status"], "PASS")
            self.assertEqual(by_old[OLD_B]["status"], "SKIP")
            self.assertTrue(by_old[OLD_B]["reason"])
            # Files: good kept, bad reverted (us only; eu/jp untouched).
            us_sym = (root / "config" / "us" / "symbols.txt").read_text()
            self.assertNotIn(OLD_A + " =", us_sym)
            self.assertIn("Do__", us_sym)
            self.assertIn(OLD_B + " =", us_sym)
            for reg in ("eu", "jp"):
                txt = (root / "config" / reg / "symbols.txt").read_text()
                self.assertIn(OLD_A + " =", txt)
                self.assertIn(OLD_B + " =", txt)
            defa = (root / "src" / "unit" / "DefA.cpp").read_text()
            defb = (root / "src" / "unit" / "DefB.cpp").read_text()
            self.assertIn("Alpha::Do", defa)
            self.assertNotIn("Beta::Do", defb)
            self.assertIn(OLD_B, defb)

    def test_all_good_combined_passes(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_two_candidate_fixture(root)
            pj = _plan_json(root)
            rc, report = _run_apply(root, pj, _make_snap(bad=None))
            self.assertEqual(rc, 0)
            unit = next(iter(report["units"].values()))
            self.assertEqual(unit["status"], "PASS")
            self.assertEqual(sorted(unit["functions"]), sorted([OLD_A, OLD_B]))
            by_old = {c["old"]: c for c in unit["candidates"]}
            self.assertEqual(by_old[OLD_A]["status"], "PASS")
            self.assertEqual(by_old[OLD_B]["status"], "PASS")
            us_sym = (root / "config" / "us" / "symbols.txt").read_text()
            self.assertNotIn(OLD_A + " =", us_sym)
            self.assertNotIn(OLD_B + " =", us_sym)

    def test_single_bad_still_fails_and_reverts(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_two_candidate_fixture(root)
            pj = _plan_json(root)
            before_us = (root / "config" / "us" / "symbols.txt").read_bytes()
            rc, report = _run_apply(
                root, pj, _make_snap(bad=OLD_B), candidate=[OLD_B])
            self.assertEqual(rc, 1)
            unit = next(iter(report["units"].values()))
            self.assertEqual(unit["status"], "FAIL")
            self.assertEqual(unit["functions"], [])
            # Single-candidate path reports the SKIP too.
            self.assertTrue(any(c["old"] == OLD_B and c["status"] == "SKIP"
                                for c in unit.get("candidates", [])))
            self.assertEqual(
                (root / "config" / "us" / "symbols.txt").read_bytes(), before_us)

    def test_mismatch_change_still_skipped(self):
        # Gate stays strict: a pure mismatch delta (same sizes) still fails.
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_two_candidate_fixture(root)
            pj = _plan_json(root)
            rc, report = _run_apply(
                root, pj, _make_snap(bad=OLD_B, mismatch_only=True))
            self.assertEqual(rc, 1)
            unit = next(iter(report["units"].values()))
            self.assertEqual(unit["status"], "PARTIAL")
            by_old = {c["old"]: c for c in unit["candidates"]}
            self.assertEqual(by_old[OLD_A]["status"], "PASS")
            self.assertEqual(by_old[OLD_B]["status"], "SKIP")
            self.assertTrue(by_old[OLD_B]["reason"])

    def test_unobserved_rename_skipped(self):
        # A rename that took no observable effect must not silently PASS.
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_two_candidate_fixture(root)
            pj = _plan_json(root)
            rc, report = _run_apply(
                root, pj, _make_snap(bad=None, unobserved=OLD_B))
            self.assertEqual(rc, 1)
            unit = next(iter(report["units"].values()))
            self.assertEqual(unit["status"], "PARTIAL")
            by_old = {c["old"]: c for c in unit["candidates"]}
            self.assertEqual(by_old[OLD_A]["status"], "PASS")
            self.assertEqual(by_old[OLD_B]["status"], "SKIP")
            self.assertIn("not observed", by_old[OLD_B]["reason"])

    def test_partial_report_schema(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_two_candidate_fixture(root)
            pj = _plan_json(root)
            _rc, report = _run_apply(root, pj, _make_snap(bad=OLD_B))
            unit = next(iter(report["units"].values()))
            for key in ("status", "functions", "skipped", "candidates", "renames"):
                self.assertIn(key, unit, f"missing report key {key}")
            self.assertIn("partial", report["summary"])
            # Renames only cover the passing candidate.
            self.assertEqual(len(unit["renames"]), 1)


if __name__ == "__main__":
    unittest.main()

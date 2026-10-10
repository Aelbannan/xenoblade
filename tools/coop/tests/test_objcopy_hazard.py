"""Regression tests for the objcopy in-place hazard (wave-DL incident).

Background: a verification script called

    powerpc-eabi-objcopy -O binary --only-section=.text <object>

with no output path. GNU objcopy treats that as in-place conversion and
rewrote 66 retail + 20 decomp objects as raw `.text` dumps, which then read as
~600 suddenly-undefined symbols in the link frontier.

These tests pin down (a) the static checker that flags the pattern,
(b) that legitimate two-operand/dump-section usage is not flagged, and
(c) that the safe helpers never modify their input.
"""

from __future__ import annotations

import hashlib
import pathlib
import sys
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools" / "coop"))

import check_script_hazards as H  # noqa: E402  (path inserted above)
from tools.coop.lib import objread  # noqa: E402

# the exact call shape that caused the incident
BAD_PY = 'subprocess.run([objcopy, "-O", "binary", "--only-section=.text", str(obj)], capture_output=True)\n'  # hazard-lint: allow (fixture)
BAD_SH = './build/binutils/powerpc-eabi-objcopy -O binary --only-section=.text build/us/obj/x.o\n'  # hazard-lint: allow (fixture)


class HazardLint(unittest.TestCase):
    def test_detects_inplace_python_call(self):
        hits = H.scan_text(BAD_PY, "x.py")
        self.assertEqual(len(hits), 1)
        self.assertIn("in-place", hits[0]["reason"])
        self.assertEqual(hits[0]["operands"], ["str(obj)"])

    def test_detects_inplace_shell_call(self):
        hits = H.scan_text(BAD_SH, "x.sh")
        self.assertEqual(len(hits), 1)
        self.assertEqual(hits[0]["operands"], ["build/us/obj/x.o"])

    def test_accepts_two_operand_forms(self):
        good = [
            'subprocess.run([objcopy, "-O", "binary", "--only-section", sec, str(obj), tmp], capture_output=True)\n',
            'subprocess.run([objcopy, "-O", "binary", f"--only-section={sec}", str(obj), str(out)], check=True)\n',
            './build/binutils/powerpc-eabi-objcopy -O binary --only-section=.text in.o out.bin\n',
            './build/binutils/powerpc-eabi-objcopy -O binary -j .text a.o b.bin 2>/dev/null || rm -f b.bin\n',
        ]
        for text in good:
            with self.subTest(text=text):
                self.assertEqual(H.scan_text(text), [])

    def test_multiline_list_calls(self):
        bad = 'subprocess.run([\n    objcopy, "-O", "binary",\n    "--only-section", sec, str(obj),\n], capture_output=True)\n'  # hazard-lint: allow (fixture)
        good = bad.replace("str(obj),\n]", "str(obj),\n    tmp,\n]")
        self.assertEqual(len(H.scan_text(bad)), 1)
        self.assertEqual(H.scan_text(good), [])

    def test_dump_section_is_a_legitimate_read(self):
        text = 'subprocess.run([objcopy, "--dump-section", f"{sec}={tmp}", str(retail)], capture_output=True)\n'
        self.assertEqual(H.scan_text(text), [])

    def test_helper_calls_and_prose_are_not_invocations(self):
        for text in (
            '_prefix_object(objcopy, retail_obj, retail_prefixed, "rb_")\n',
            '        objcopy, dtk, ldscript = _tool_paths(root)\n',
            'objcopy shows zeros because the linker cannot see them\n',
            '# the objcopy pass rewrites every symbol in place\n',
        ):
            with self.subTest(text=text):
                self.assertEqual(H.scan_text(text), [])

    def test_allow_marker(self):
        text = 'subprocess.run([objcopy, "-G", sym, "--strip-unneeded", str(dst)], check=True)  # hazard-lint: allow\n'
        self.assertEqual(H.scan_text(text), [])

    def test_repo_has_no_hazards(self):
        """tools/ and .scratch/ must stay free of in-place object writes."""
        hazards = []
        for root in ("tools", ".scratch"):
            for path in H.iter_sources([ROOT / root]):
                hazards += H.scan_file(path)
        self.assertEqual(hazards, [], f"hazards: {hazards}")


class SafeObjectReads(unittest.TestCase):
    def _some_elf(self) -> pathlib.Path | None:
        for base in ("build/us/obj", "build/us/src"):
            for p in sorted((ROOT / base).rglob("*.o")):
                if objread.is_elf(p):
                    return p
        return None

    def test_section_bytes_does_not_modify_input(self):
        obj = self._some_elf()
        if obj is None:
            self.skipTest("no built ELF object available")
        before = (obj.stat().st_mtime_ns, hashlib.sha256(obj.read_bytes()).hexdigest())
        data = objread.section_bytes(obj, ".text")
        after = (obj.stat().st_mtime_ns, hashlib.sha256(obj.read_bytes()).hexdigest())
        self.assertTrue(data)
        self.assertEqual(before, after, "safe helper must never rewrite its input")

    def test_require_elf_rejects_raw_dump(self):
        import tempfile

        with tempfile.TemporaryDirectory() as td:
            raw = pathlib.Path(td) / "raw.o"
            raw.write_bytes(b"\x3c\x80\x00\x00" * 16)  # PPC code, no ELF header
            self.assertFalse(objread.is_elf(raw))
            self.assertIsNone(objread.section_bytes(raw, ".text"))
            with self.assertRaises(objread.NotAnObjectError):
                objread.require_elf(raw)


if __name__ == "__main__":
    unittest.main()

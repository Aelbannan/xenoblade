from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.test.compare_behaviour.lib.manifest import load_manifest
from tools.test.compare_behaviour.lib.ppc_runner import _write_dolphin_user_ini


class ManifestTests(unittest.TestCase):
    def test_loads_per_test_dolphin_cpu_core(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "manifest.json"
            path.write_text(
                json.dumps(
                    {
                        "region": "us",
                        "tests": [
                            {
                                "id": "oracle",
                                "unit": "unit",
                                "symbol": "symbol",
                                "ppc_cpu_core": 0,
                            }
                        ],
                    }
                ),
                encoding="utf-8",
            )

            _, tests = load_manifest(path)

        self.assertEqual(tests[0].ppc_cpu_core, 0)

    def test_cpu_core_is_optional(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "manifest.json"
            path.write_text(
                json.dumps(
                    {
                        "tests": [
                            {"id": "ordinary", "unit": "unit", "symbol": "symbol"}
                        ]
                    }
                ),
                encoding="utf-8",
            )

            _, tests = load_manifest(path)

        self.assertIsNone(tests[0].ppc_cpu_core)


class DolphinConfigTests(unittest.TestCase):
    def test_writes_interpreter_core_for_architecture_oracle(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            user_dir = Path(directory)
            log_path = user_dir / "dolphin.log"

            _write_dolphin_user_ini(user_dir, gdb_port=2160, log_path=log_path, cpu_core=0)

            text = (user_dir / "Config" / "Dolphin.ini").read_text(encoding="utf-8")
        self.assertIn("[Core]\nCPUCore = 0\n", text)

    def test_leaves_normal_tests_on_dolphin_default_core(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            user_dir = Path(directory)

            _write_dolphin_user_ini(
                user_dir, gdb_port=2160, log_path=user_dir / "dolphin.log"
            )

            text = (user_dir / "Config" / "Dolphin.ini").read_text(encoding="utf-8")
        self.assertNotIn("CPUCore", text)

    def test_disables_panic_handlers_and_analytics_prompts(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            user_dir = Path(directory)
            _write_dolphin_user_ini(
                user_dir, gdb_port=2160, log_path=user_dir / "dolphin.log"
            )
            text = (user_dir / "Config" / "Dolphin.ini").read_text(encoding="utf-8")
        self.assertIn("[Interface]\nUsePanicHandlers = False\n", text)
        self.assertIn("ConfirmStop = False\n", text)
        self.assertIn("[Analytics]\nEnabled = False\nPermissionAsked = True\n", text)


if __name__ == "__main__":
    unittest.main()

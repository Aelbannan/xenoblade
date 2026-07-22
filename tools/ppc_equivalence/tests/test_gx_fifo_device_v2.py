from __future__ import annotations

import json
import unittest
from pathlib import Path

from tools.ppc_equivalence.device_model import (
    AccessOutcome,
    GxFifoDevice,
    GxFifoStreamDevice,
)
from tools.ppc_equivalence.hardware_profile import (
    REVIEWED_HARDWARE_PROFILES,
    compute_hardware_profile_sha256,
    device_models_sha256,
    is_reviewed_hardware_profile,
    load_hardware_profile,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256

_PROFILES_DIR = Path(__file__).resolve().parents[1] / "platform_profiles"
_V1_DIGEST = "9cc02c7cc286b8a8be11631270336e31a3949806020322a05fb991fc247d732e"


class GxFifoProfileV2Tests(unittest.TestCase):
    def test_v2_profile_loads_and_is_reviewed(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v2")
        self.assertEqual(profile["profile"], "wii-broadway-xenoblade-us-v2")
        self.assertIn("wii-broadway-xenoblade-us-v2", REVIEWED_HARDWARE_PROFILES)
        self.assertTrue(is_reviewed_hardware_profile(profile))
        self.assertEqual(
            profile["profile_sha256"],
            compute_hardware_profile_sha256(profile),
        )
        gx_regions = [
            region
            for region in profile["regions"]
            if region.get("device_id") == "gx-fifo"
        ]
        self.assertEqual(len(gx_regions), 1)
        self.assertEqual(gx_regions[0]["start"], "0xcc008000")
        self.assertEqual(gx_regions[0]["end"], "0xcc00807f")

    def test_v1_digest_unchanged(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v1")
        self.assertEqual(profile["profile_sha256"], _V1_DIGEST)
        self.assertEqual(compute_hardware_profile_sha256(profile), _V1_DIGEST)

    def test_v2_device_models_sha256_stable(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v2")
        expected = device_models_sha256(profile)
        self.assertEqual(len(expected), 64)
        self.assertEqual(device_models_sha256(profile), expected)


class GxFifoStreamDeviceV2Tests(unittest.TestCase):
    def setUp(self) -> None:
        self.base = 0xCC008000
        self.device = GxFifoStreamDevice(base=self.base, span=0x80)

    def test_write_records_event_with_digests_and_advances_cursor(self) -> None:
        result = self.device.write(self.base, 4, 0x12345678)
        self.assertEqual(result.outcome, AccessOutcome.OK)
        self.assertEqual(self.device.event_cursor, 1)
        self.assertEqual(len(self.device.write_events), 1)
        event = self.device.write_events[0]
        self.assertEqual(event["kind"], "write")
        self.assertEqual(event["device_id"], "gx-fifo")
        self.assertEqual(event["event_index"], 0)
        self.assertEqual(event["address"], hex(self.base))
        self.assertEqual(event["width"], 4)
        self.assertEqual(event["value"], hex(0x12345678))
        self.assertEqual(len(event["pre_state_digest"]), 64)
        self.assertEqual(len(event["post_state_digest"]), 64)
        self.assertNotEqual(event["pre_state_digest"], event["post_state_digest"])

    def test_read_unsupported_does_not_advance_cursor(self) -> None:
        cursor_before = self.device.event_cursor
        result = self.device.read(self.base, 4)
        self.assertEqual(result.outcome, AccessOutcome.UNSUPPORTED)
        self.assertEqual(self.device.event_cursor, cursor_before)
        self.assertEqual(self.device.read_events, [])

    def test_clone_independence(self) -> None:
        self.device.write(self.base, 4, 0x11111111)
        cloned = self.device.clone()
        cloned.write(self.base + 4, 2, 0x2222)
        self.assertEqual(len(self.device.write_events), 1)
        self.assertEqual(len(cloned.write_events), 2)
        self.assertEqual(self.device.event_cursor, 1)
        self.assertEqual(cloned.event_cursor, 2)

    def test_misalignment_fail_closed(self) -> None:
        self.assertEqual(
            self.device.validate_access(self.base + 1, 4, is_write=True),
            AccessOutcome.MISALIGNED,
        )
        result = self.device.write(self.base + 1, 4, 0)
        self.assertEqual(result.outcome, AccessOutcome.MISALIGNED)
        self.assertEqual(self.device.event_cursor, 0)
        self.assertEqual(self.device.write_events, [])

    def test_bad_width_fail_closed(self) -> None:
        self.assertEqual(
            self.device.validate_access(self.base, 8, is_write=True),
            AccessOutcome.UNSUPPORTED,
        )
        result = self.device.write(self.base, 8, 0)
        self.assertEqual(result.outcome, AccessOutcome.UNSUPPORTED)
        self.assertEqual(self.device.event_cursor, 0)

    def test_gx_fifo_device_alias(self) -> None:
        self.assertIs(GxFifoDevice, GxFifoStreamDevice)


class GxFifoProfileShaIntegrityTests(unittest.TestCase):
    def test_on_disk_v2_profile_sha_matches_identity(self) -> None:
        path = _PROFILES_DIR / "wii-broadway-xenoblade-us-v2.json"
        raw = json.loads(path.read_text(encoding="utf-8"))
        expected = compute_hardware_profile_sha256(raw)
        declared = raw.get("profile_sha256")
        if declared is not None:
            self.assertEqual(declared, expected)
        recomputed_from_loaded = load_hardware_profile("wii-broadway-xenoblade-us-v2")
        self.assertEqual(recomputed_from_loaded["profile_sha256"], expected)


if __name__ == "__main__":
    unittest.main()

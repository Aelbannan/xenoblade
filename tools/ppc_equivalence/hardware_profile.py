"""Immutable, hash-bound Wii/Broadway hardware profiles for MMIO assurance.

Wave 3: reviewed profiles under ``platform_profiles/`` may eventually authorize
promotion-grade ``mmio-register-bank`` attestations. Ad-hoc CLI / caller-built
``MemoryBus`` maps are never promotion-grade — only profiles whose
``profile_sha256`` matches the on-disk reviewed artifact.
"""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any, Mapping

from tools.ppc_equivalence.provenance import canonical_json_sha256

_PLATFORM_PROFILES_DIR = Path(__file__).resolve().parent / "platform_profiles"

# Reviewed hardware profiles eligible for MMIO promotion-grade paths (once
# allowlisted). Ad-hoc maps are never listed here.
REVIEWED_HARDWARE_PROFILES: frozenset[str] = frozenset(
    {
        "wii-broadway-xenoblade-us-v1",
        "wii-broadway-xenoblade-us-v2",
    }
)

SOURCE_HARDWARE_PROFILE = "hardware-profile"
SOURCE_AD_HOC_BUS = "ad-hoc-bus-map"


def hardware_profiles_dir() -> Path:
    return _PLATFORM_PROFILES_DIR


def _normalize_addr_hex(value: Any) -> str:
    if isinstance(value, int):
        return hex(value & 0xFFFFFFFF)
    text = str(value).strip().lower()
    if text.startswith("0x"):
        return hex(int(text, 16) & 0xFFFFFFFF)
    return hex(int(text, 0) & 0xFFFFFFFF)


def _profile_name(profile: Mapping[str, Any]) -> str:
    name = profile.get("profile") or profile.get("platform_profile")
    if not isinstance(name, str) or not name:
        raise ValueError("hardware profile missing profile name")
    return name


_FIFO_DEVICE_KEYS: tuple[str, ...] = (
    "supported_widths",
    "alignment_required",
    "read_policy",
    "read_model_version",
    "write_event_semantics",
    "read_side_effects",
    "external_input",
    "max_fifo_events",
    "legal_aliases",
)


def _reset_state_sort_key(key: str) -> tuple[int, int | str]:
    if key.startswith("0x"):
        return (0, int(key, 16))
    return (1, key)


def _normalize_reset_state_key(key: Any) -> str:
    if isinstance(key, int):
        return _normalize_addr_hex(key)
    text = str(key).strip()
    if text.startswith("0x") or text.isdigit():
        return _normalize_addr_hex(key)
    return str(key)


def _normalize_gx_fifo_block(block: Mapping[str, Any]) -> dict[str, Any]:
    supported = block.get("supported_widths", [1, 2, 4])
    if not isinstance(supported, list):
        raise ValueError("gx_fifo supported_widths must be a list")
    return {
        "alignment_required": bool(block.get("alignment_required", True)),
        "base": _normalize_addr_hex(block["base"]),
        "endian": str(block.get("endian", "big")),
        "external_input": bool(block.get("external_input", False)),
        "max_fifo_events": int(block.get("max_fifo_events", 256)),
        "read_model_version": str(block["read_model_version"]),
        "read_policy": str(block["read_policy"]),
        "read_side_effects": bool(block.get("read_side_effects", False)),
        "span": int(block["span"]),
        "supported_widths": sorted(int(width) for width in supported),
        "write_event_semantics": str(block["write_event_semantics"]),
    }


def _normalize_device_entry(device: Mapping[str, Any]) -> dict[str, Any]:
    theory = str(device["theory"])
    entry: dict[str, Any] = {
        "base": _normalize_addr_hex(device["base"]),
        "device_id": str(device["device_id"]),
        "endian": str(device.get("endian", "big")),
        "reg_width": int(device.get("reg_width", 4)),
        "span": int(device.get("span", 0)),
        "theory": theory,
    }
    include_fifo = theory == "gxfifo-stream"
    for key in _FIFO_DEVICE_KEYS:
        if key not in device and not include_fifo:
            continue
        if key == "supported_widths":
            widths = device.get(key, [1, 2, 4])
            if not isinstance(widths, list):
                raise ValueError("device supported_widths must be a list")
            entry[key] = sorted(int(width) for width in widths)
        elif key == "legal_aliases":
            aliases = device.get(key, [])
            if not isinstance(aliases, list):
                raise ValueError("device legal_aliases must be a list")
            entry[key] = sorted(_normalize_addr_hex(alias) for alias in aliases)
        elif key in {"alignment_required", "read_side_effects", "external_input"}:
            default = key == "alignment_required"
            entry[key] = bool(device.get(key, default))
        elif key == "max_fifo_events":
            entry[key] = int(device.get(key, 256))
        else:
            defaults = {
                "read_policy": "unsupported",
                "read_model_version": "gx-fifo-read-v1",
                "write_event_semantics": "gx-fifo-trace-v2",
            }
            if key in device:
                entry[key] = str(device[key])
            elif include_fifo and key in defaults:
                entry[key] = defaults[key]
            else:
                raise ValueError(f"device missing required FIFO field {key!r}")
    return entry


def hardware_profile_identity(profile: Mapping[str, Any]) -> dict[str, Any]:
    """Canonical identity payload (excludes ``profile_sha256`` and labels)."""
    regions = profile.get("regions", [])
    if not isinstance(regions, list):
        raise ValueError("hardware profile regions must be a list")
    normalized_regions: list[dict[str, Any]] = []
    for region in regions:
        if not isinstance(region, dict):
            raise ValueError("hardware profile region must be an object")
        entry: dict[str, Any] = {
            "kind": str(region["kind"]),
            "start": _normalize_addr_hex(region["start"]),
            "end": _normalize_addr_hex(region["end"]),
        }
        device_id = region.get("device_id")
        if device_id is not None:
            entry["device_id"] = str(device_id)
        normalized_regions.append(entry)

    devices = profile.get("devices", [])
    if not isinstance(devices, list):
        raise ValueError("hardware profile devices must be a list")
    normalized_devices: list[dict[str, Any]] = []
    for device in devices:
        if not isinstance(device, dict):
            raise ValueError("hardware profile device must be an object")
        normalized_devices.append(_normalize_device_entry(device))

    register_specs = profile.get("register_specs", [])
    if not isinstance(register_specs, list):
        raise ValueError("hardware profile register_specs must be a list")
    normalized_specs: list[dict[str, Any]] = []
    for spec in register_specs:
        if not isinstance(spec, dict):
            raise ValueError("hardware profile register_spec must be an object")
        normalized_specs.append(
            {
                "access": str(spec.get("access", "rw")),
                "device_id": str(spec["device_id"]),
                "initial": int(spec.get("initial", 0)),
                "offset": int(spec["offset"]),
                "read_clear": bool(spec.get("read_clear", False)),
                "reserved_mask": int(spec.get("reserved_mask", 0)),
                "w1c": bool(spec.get("w1c", False)),
            }
        )

    reset_state = profile.get("reset_state", {})
    if not isinstance(reset_state, dict):
        raise ValueError("hardware profile reset_state must be an object")
    normalized_reset: dict[str, dict[str, int]] = {}
    for device_id, values in sorted(reset_state.items()):
        if not isinstance(values, dict):
            raise ValueError("hardware profile reset_state values must be objects")
        fixed: dict[str, int] = {}
        for offset, value in values.items():
            key = _normalize_reset_state_key(offset)
            fixed[key] = int(value)
        normalized_reset[str(device_id)] = {
            key: fixed[key] for key in sorted(fixed, key=_reset_state_sort_key)
        }

    identity: dict[str, Any] = {
        "devices": normalized_devices,
        "profile": _profile_name(profile),
        "regions": normalized_regions,
        "register_specs": normalized_specs,
        "reset_state": normalized_reset,
    }
    if "fifo_bound" in profile:
        identity["fifo_bound"] = int(profile["fifo_bound"])
    gx_fifo = profile.get("gx_fifo")
    if gx_fifo is not None:
        if not isinstance(gx_fifo, dict):
            raise ValueError("hardware profile gx_fifo must be an object")
        identity["gx_fifo"] = _normalize_gx_fifo_block(gx_fifo)
    return identity


def compute_hardware_profile_sha256(profile: Mapping[str, Any]) -> str:
    return canonical_json_sha256(hardware_profile_identity(profile))


def load_hardware_profile(
    name_or_path: str | Path,
    *,
    profiles_dir: Path | None = None,
) -> dict[str, Any]:
    """Load a hardware profile JSON and verify ``profile_sha256`` when present."""
    path = Path(name_or_path)
    if not path.suffix:
        base = profiles_dir if profiles_dir is not None else hardware_profiles_dir()
        path = base / f"{name_or_path}.json"
    data = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise ValueError(f"hardware profile {path} must be a JSON object")
    if "regions" not in data or "devices" not in data:
        raise ValueError(f"hardware profile {path} missing required fields")
    if "profile" not in data and "platform_profile" not in data:
        raise ValueError(f"hardware profile {path} missing profile name")
    expected = compute_hardware_profile_sha256(data)
    declared = data.get("profile_sha256")
    if declared is not None and declared != expected:
        raise ValueError(
            f"hardware profile {path} profile_sha256 mismatch "
            f"(declared={declared}, recomputed={expected})"
        )
    out = dict(data)
    out["profile"] = _profile_name(out)
    out["profile_sha256"] = expected
    return out


def is_reviewed_hardware_profile(profile: Mapping[str, Any]) -> bool:
    """True when *profile* matches an on-disk reviewed hardware artifact."""
    try:
        name = _profile_name(profile)
    except ValueError:
        return False
    if name not in REVIEWED_HARDWARE_PROFILES:
        return False
    try:
        loaded = load_hardware_profile(name)
    except (OSError, ValueError, json.JSONDecodeError):
        return False
    digest = profile.get("profile_sha256") or profile.get("hardware_profile_sha256")
    return (
        loaded.get("profile_sha256") == digest
        and compute_hardware_profile_sha256(profile) == loaded.get("profile_sha256")
    )


def device_models_sha256(profile: Mapping[str, Any]) -> str:
    """Digest over devices + register_specs + reset_state (profile-bound)."""
    identity = hardware_profile_identity(profile)
    return canonical_json_sha256(
        {
            "devices": identity["devices"],
            "register_specs": identity["register_specs"],
            "reset_state": identity["reset_state"],
        }
    )

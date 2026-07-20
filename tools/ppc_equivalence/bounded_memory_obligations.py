"""Independent address-coverage / wraparound obligations for bounded memory.

Wave 2: bounded-memory may eventually reach Tier A only when every memory
access is proven in-range by a dedicated UNSAT query (not by assuming range
constraints into the equivalence formula). Assumed-ordinary-ram remains
non-promotable. CLI / ad-hoc ranges never qualify as promotion-grade; only
hash-bound reviewed platform profiles do.
"""

from __future__ import annotations

import json
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.discharge import discharge_bad_conditions
from tools.ppc_equivalence.memory_profile import (
    MemoryEnvironment,
    MemoryProfile,
    access_within_any_range,
    is_bounded_with_ranges,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256

BOUNDED_MEMORY_OBLIGATION_SCHEMA_VERSION = 2
BOUNDED_MEMORY_ALGORITHM = "bounded-memory-v2"
BOUNDED_MEMORY_MODEL_VERSION = "bounded-memory-v2"
BOUNDED_MEMORY_ATTESTATION_ALGORITHM = "bounded-memory-coverage-v2"

_SHA256_LEN = 64
_PLATFORM_PROFILES_DIR = Path(__file__).resolve().parent / "platform_profiles"

# Reviewed platform profiles that may authorize promotion-grade bounded-memory.
REVIEWED_PLATFORM_PROFILES: frozenset[str] = frozenset({"xenoblade-us-retail-v1"})

SOURCE_PLATFORM_PROFILE = "platform-profile"
SOURCE_AD_HOC = "ad-hoc-ranges"
SOURCE_ASSUMED = "assumed-ordinary-ram"
SOURCE_INCOMPLETE = "incomplete"


@dataclass(frozen=True, slots=True)
class MemoryAccessObligation:
    """One memory access under a path condition for coverage discharge."""

    addr: Any
    width: int
    path_condition: Any
    is_write: bool = False
    label: str | None = None

    def __post_init__(self) -> None:
        if self.width <= 0:
            raise ValueError("width must be positive")


def _is_sha256(value: Any) -> bool:
    if not isinstance(value, str) or len(value) != _SHA256_LEN:
        return False
    try:
        int(value, 16)
    except ValueError:
        return False
    return True


def platform_profiles_dir() -> Path:
    return _PLATFORM_PROFILES_DIR


def platform_profile_identity(profile: Mapping[str, Any]) -> dict[str, Any]:
    """Canonical identity payload (excludes ``profile_sha256``)."""
    regions = profile.get("regions", [])
    if not isinstance(regions, list):
        raise ValueError("platform profile regions must be a list")
    normalized_regions: list[dict[str, Any]] = []
    for region in regions:
        if not isinstance(region, dict):
            raise ValueError("platform profile region must be an object")
        entry: dict[str, Any] = {
            "kind": str(region["kind"]),
            "start": _normalize_addr_hex(region["start"]),
            "end": _normalize_addr_hex(region["end"]),
        }
        # Labels are descriptive; omit from identity so optional labels do not
        # fork digests for the same address space.
        normalized_regions.append(entry)
    return {
        "platform_profile": str(profile["platform_profile"]),
        "regions": normalized_regions,
    }


def _normalize_addr_hex(value: Any) -> str:
    if isinstance(value, int):
        return hex(value & 0xFFFFFFFF)
    text = str(value).strip().lower()
    if text.startswith("0x"):
        return hex(int(text, 16) & 0xFFFFFFFF)
    return hex(int(text, 0) & 0xFFFFFFFF)


def compute_platform_profile_sha256(profile: Mapping[str, Any]) -> str:
    return canonical_json_sha256(platform_profile_identity(profile))


def regions_to_ranges(regions: Sequence[Mapping[str, Any]]) -> list[tuple[int, int]]:
    """Extract inclusive (start, end) ranges from region descriptors."""
    ranges: list[tuple[int, int]] = []
    for region in regions:
        start = int(str(region["start"]), 0) & 0xFFFFFFFF
        end = int(str(region["end"]), 0) & 0xFFFFFFFF
        if start > end:
            raise ValueError(f"region start > end: {start:#x} > {end:#x}")
        ranges.append((start, end))
    return ranges


def load_platform_profile(
    name_or_path: str | Path,
    *,
    profiles_dir: Path | None = None,
) -> dict[str, Any]:
    """Load a platform profile JSON and verify ``profile_sha256`` when present."""
    path = Path(name_or_path)
    if not path.suffix:
        base = profiles_dir if profiles_dir is not None else platform_profiles_dir()
        path = base / f"{name_or_path}.json"
    data = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise ValueError(f"platform profile {path} must be a JSON object")
    if "platform_profile" not in data or "regions" not in data:
        raise ValueError(f"platform profile {path} missing required fields")
    expected = compute_platform_profile_sha256(data)
    declared = data.get("profile_sha256")
    if declared is not None and declared != expected:
        raise ValueError(
            f"platform profile {path} profile_sha256 mismatch "
            f"(declared={declared}, recomputed={expected})"
        )
    # Normalize: always expose recomputed digest.
    out = dict(data)
    out["profile_sha256"] = expected
    return out


def is_reviewed_platform_profile(profile: Mapping[str, Any]) -> bool:
    name = str(profile.get("platform_profile", ""))
    if name not in REVIEWED_PLATFORM_PROFILES:
        return False
    try:
        loaded = load_platform_profile(name)
    except (OSError, ValueError, json.JSONDecodeError):
        return False
    return (
        loaded.get("profile_sha256") == profile.get("profile_sha256")
        and compute_platform_profile_sha256(profile)
        == loaded.get("profile_sha256")
    )


def access_outside_all_ranges(
    addr: Any,
    width_bytes: int,
    ranges: Sequence[tuple[int, int]],
) -> Any:
    """Predicate: access is not fully contained in any configured range."""
    import z3

    return z3.Not(access_within_any_range(addr, width_bytes, list(ranges)))


def access_wraps_32bit(addr: Any, width_bytes: int) -> Any:
    """Predicate: multi-byte access wraps the 32-bit address space."""
    import z3

    if width_bytes <= 1:
        return z3.BoolVal(False)
    last = addr + z3.BitVecVal(width_bytes - 1, 32)
    return z3.ULT(last, addr)


def address_space_sha256(ranges: Sequence[tuple[int, int]]) -> str:
    return canonical_json_sha256(
        {
            "kind": "bounded-memory-address-space",
            "schema_version": 1,
            "ranges": [
                {"start": hex(low & 0xFFFFFFFF), "end": hex(high & 0xFFFFFFFF)}
                for low, high in ranges
            ],
        }
    )


def accesses_from_terminals(
    terminals: Sequence[Any],
    *,
    default_width: int = 1,
) -> list[MemoryAccessObligation]:
    """Lift per-byte ``memory_touches`` / ``memory_writes`` into access records."""
    accesses: list[MemoryAccessObligation] = []
    for index, terminal in enumerate(terminals):
        condition = getattr(terminal, "condition", None)
        if condition is None:
            continue
        state = getattr(terminal, "state", None)
        if state is None:
            continue
        writes = set()
        for write_addr in getattr(state, "memory_writes", ()) or ():
            writes.add(id(write_addr))
            accesses.append(
                MemoryAccessObligation(
                    addr=write_addr,
                    width=default_width,
                    path_condition=condition,
                    is_write=True,
                    label=f"terminal[{index}].write",
                )
            )
        for touch in getattr(state, "memory_touches", ()) or ():
            if id(touch) in writes:
                continue
            accesses.append(
                MemoryAccessObligation(
                    addr=touch,
                    width=default_width,
                    path_condition=condition,
                    is_write=False,
                    label=f"terminal[{index}].touch",
                )
            )
    return accesses


def _worst_result(results: Sequence[str]) -> str:
    if any(item == "sat" for item in results):
        return "sat"
    if any(item in ("unknown", "timeout") for item in results):
        return "unknown"
    if any(item == "not-queried" for item in results):
        return "not-queried"
    return "unsat"


def _aggregate_side_query_sha256(
    *,
    kind: str,
    terminals: Sequence[dict[str, Any]],
) -> str:
    return canonical_json_sha256(
        {
            "kind": kind,
            "schema_version": 1,
            "terminals": [dict(entry) for entry in terminals],
        }
    )


def _discharge_access_predicate(
    accesses: Sequence[MemoryAccessObligation],
    *,
    kind: str,
    predicate_builder,
    deadline: Deadline,
    z3: Any,
) -> dict[str, Any]:
    """Discharge ``path ∧ predicate(access)`` for each access; aggregate digests."""
    if not accesses:
        vacuous = discharge_bad_conditions(
            premises=[],
            bad_conditions=[],
            deadline=deadline,
            algorithm=BOUNDED_MEMORY_ALGORITHM,
            z3_module=z3,
        )
        return {
            "result": "unsat",
            "query_sha256": vacuous.query_sha256,
            "status": "vacuously-discharged",
            "terminals": [],
            "solver": {
                "name": "z3",
                "version": z3.get_version_string(),
                "elapsed_ms": vacuous.elapsed_ms,
            },
        }

    terminal_results: list[dict[str, Any]] = []
    results: list[str] = []
    total_elapsed = 0.0
    for index, access in enumerate(accesses):
        bad = predicate_builder(access)
        discharge = discharge_bad_conditions(
            premises=[access.path_condition],
            bad_conditions=[bad],
            deadline=deadline,
            algorithm=BOUNDED_MEMORY_ALGORITHM,
            z3_module=z3,
        )
        total_elapsed += discharge.elapsed_ms
        entry = {
            "index": index,
            "width": access.width,
            "is_write": access.is_write,
            "label": access.label,
            "result": discharge.status,
            "query_sha256": discharge.query_sha256,
            "inconclusive": discharge.status != "unsat",
            "solver": {
                "name": "z3",
                "version": z3.get_version_string(),
                "elapsed_ms": discharge.elapsed_ms,
            },
        }
        terminal_results.append(entry)
        results.append(discharge.status)

    worst = _worst_result(results)
    return {
        "result": worst if worst != "timeout" else "unknown",
        "query_sha256": _aggregate_side_query_sha256(
            kind=kind, terminals=terminal_results,
        ),
        "terminals": terminal_results,
        "solver": {
            "name": "z3",
            "version": z3.get_version_string(),
            "elapsed_ms": total_elapsed,
        },
    }


def discharge_address_coverage(
    accesses: Sequence[MemoryAccessObligation],
    ranges: Sequence[tuple[int, int]],
    *,
    deadline: Deadline | None = None,
    z3: Any | None = None,
) -> dict[str, Any]:
    """Prove ``path ∧ access_outside_all_ranges`` is UNSAT for every access."""
    if z3 is None:
        import z3 as z3  # type: ignore[no-redef]
    effective = deadline if deadline is not None else Deadline.after_ms(5_000)

    def _pred(access: MemoryAccessObligation) -> Any:
        return access_outside_all_ranges(access.addr, access.width, ranges)

    return _discharge_access_predicate(
        accesses,
        kind="bounded-memory-address-coverage-aggregate",
        predicate_builder=_pred,
        deadline=effective,
        z3=z3,
    )


def discharge_wraparound(
    accesses: Sequence[MemoryAccessObligation],
    *,
    deadline: Deadline | None = None,
    z3: Any | None = None,
) -> dict[str, Any]:
    """Prove ``path ∧ access_wraps_32bit`` is UNSAT for every access."""
    if z3 is None:
        import z3 as z3  # type: ignore[no-redef]
    effective = deadline if deadline is not None else Deadline.after_ms(5_000)

    def _pred(access: MemoryAccessObligation) -> Any:
        return access_wraps_32bit(access.addr, access.width)

    return _discharge_access_predicate(
        accesses,
        kind="bounded-memory-wraparound-aggregate",
        predicate_builder=_pred,
        deadline=effective,
        z3=z3,
    )


def discharge_rom_writes(
    accesses: Sequence[MemoryAccessObligation],
    rom_ranges: Sequence[tuple[int, int]],
    *,
    deadline: Deadline | None = None,
    z3: Any | None = None,
) -> dict[str, Any] | None:
    """Prove writes never hit ROM ranges (when ROM is modeled)."""
    if not rom_ranges:
        return None
    if z3 is None:
        import z3 as z3  # type: ignore[no-redef]
    effective = deadline if deadline is not None else Deadline.after_ms(5_000)
    writes = [access for access in accesses if access.is_write]
    if not writes:
        vacuous = discharge_bad_conditions(
            premises=[],
            bad_conditions=[],
            deadline=effective,
            algorithm=BOUNDED_MEMORY_ALGORITHM,
            z3_module=z3,
        )
        return {
            "result": "unsat",
            "query_sha256": vacuous.query_sha256,
            "status": "vacuously-discharged",
            "terminals": [],
        }

    def _pred(access: MemoryAccessObligation) -> Any:
        # Write hits ROM when any touched byte lies in a ROM range.
        return access_within_any_range(access.addr, access.width, list(rom_ranges))

    return _discharge_access_predicate(
        writes,
        kind="bounded-memory-rom-write-aggregate",
        predicate_builder=_pred,
        deadline=effective,
        z3=z3,
    )


def _rom_ranges_from_regions(
    regions: Sequence[Mapping[str, Any]],
) -> list[tuple[int, int]]:
    rom: list[tuple[int, int]] = []
    for region in regions:
        kind = str(region.get("kind", "")).lower()
        if kind in ("rom", "rom-image", "rom_image"):
            start = int(str(region["start"]), 0) & 0xFFFFFFFF
            end = int(str(region["end"]), 0) & 0xFFFFFFFF
            rom.append((start, end))
    return rom


def classify_range_source(
    *,
    environment: MemoryEnvironment | None = None,
    platform_profile: Mapping[str, Any] | None = None,
) -> str:
    """Classify how ranges were supplied (promotion eligibility gate)."""
    if platform_profile is not None and is_reviewed_platform_profile(platform_profile):
        return SOURCE_PLATFORM_PROFILE
    if environment is not None:
        if environment.profile == MemoryProfile.ASSUMED_ORDINARY_RAM:
            return SOURCE_ASSUMED
        if is_bounded_with_ranges(environment):
            return SOURCE_AD_HOC
    return SOURCE_INCOMPLETE


def build_bounded_memory_obligation(
    *,
    original_accesses: Sequence[MemoryAccessObligation] | None = None,
    candidate_accesses: Sequence[MemoryAccessObligation] | None = None,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
    ranges: Sequence[tuple[int, int]] | None = None,
    environment: MemoryEnvironment | None = None,
    platform_profile: Mapping[str, Any] | str | Path | None = None,
    regions: Sequence[Mapping[str, Any]] | None = None,
    deadline: Deadline | None = None,
    z3: Any | None = None,
) -> dict[str, Any]:
    """Build a bounded-memory-v2 obligation with per-side coverage digests."""
    if z3 is None:
        import z3 as z3  # type: ignore[no-redef]
    effective = deadline if deadline is not None else Deadline.after_ms(5_000)

    profile_obj: dict[str, Any] | None = None
    if isinstance(platform_profile, (str, Path)):
        profile_obj = load_platform_profile(platform_profile)
    elif isinstance(platform_profile, Mapping):
        profile_obj = dict(platform_profile)
        if "profile_sha256" not in profile_obj:
            profile_obj["profile_sha256"] = compute_platform_profile_sha256(profile_obj)

    resolved_regions: list[dict[str, Any]] = []
    if profile_obj is not None:
        resolved_regions = [dict(r) for r in profile_obj.get("regions", [])]
    elif regions is not None:
        resolved_regions = [dict(r) for r in regions]

    if ranges is not None:
        resolved_ranges = [(int(low), int(high)) for low, high in ranges]
    elif resolved_regions:
        resolved_ranges = regions_to_ranges(resolved_regions)
    elif environment is not None and environment.ranges:
        resolved_ranges = list(environment.ranges)
    else:
        resolved_ranges = []

    source = classify_range_source(
        environment=environment,
        platform_profile=profile_obj,
    )
    # Ad-hoc explicit ranges without a reviewed profile stay ad-hoc even when
    # environment was omitted.
    if source == SOURCE_INCOMPLETE and resolved_ranges and profile_obj is None:
        source = SOURCE_AD_HOC

    orig = (
        list(original_accesses)
        if original_accesses is not None
        else accesses_from_terminals(original_terminals or ())
    )
    cand = (
        list(candidate_accesses)
        if candidate_accesses is not None
        else accesses_from_terminals(candidate_terminals or ())
    )

    rom_ranges = _rom_ranges_from_regions(resolved_regions)

    original_block = {
        "address_coverage": discharge_address_coverage(
            orig, resolved_ranges, deadline=effective, z3=z3,
        ),
        "wraparound": discharge_wraparound(orig, deadline=effective, z3=z3),
    }
    candidate_block = {
        "address_coverage": discharge_address_coverage(
            cand, resolved_ranges, deadline=effective, z3=z3,
        ),
        "wraparound": discharge_wraparound(cand, deadline=effective, z3=z3),
    }

    rom_original = discharge_rom_writes(
        orig, rom_ranges, deadline=effective, z3=z3,
    )
    rom_candidate = discharge_rom_writes(
        cand, rom_ranges, deadline=effective, z3=z3,
    )
    if rom_original is not None:
        original_block["rom_write"] = rom_original
    if rom_candidate is not None:
        candidate_block["rom_write"] = rom_candidate

    side_results = [
        original_block["address_coverage"].get("result"),
        original_block["wraparound"].get("result"),
        candidate_block["address_coverage"].get("result"),
        candidate_block["wraparound"].get("result"),
    ]
    if rom_original is not None:
        side_results.append(rom_original.get("result"))
    if rom_candidate is not None:
        side_results.append(rom_candidate.get("result"))

    all_unsat = all(result == "unsat" for result in side_results)
    promotion_eligible = (
        source == SOURCE_PLATFORM_PROFILE
        and profile_obj is not None
        and is_reviewed_platform_profile(profile_obj)
        and bool(resolved_ranges)
        and all_unsat
    )

    obligation: dict[str, Any] = {
        "schema_version": BOUNDED_MEMORY_OBLIGATION_SCHEMA_VERSION,
        "capability": "bounded-memory",
        "model_version": BOUNDED_MEMORY_MODEL_VERSION,
        "algorithm": BOUNDED_MEMORY_ALGORITHM,
        "status": "promotion-grade" if promotion_eligible else (
            "scoped-assumption" if source == SOURCE_AD_HOC and all_unsat
            else "incomplete"
        ),
        "source": source,
        "address_space_sha256": address_space_sha256(resolved_ranges),
        "ranges": [
            {"start": hex(low), "end": hex(high)} for low, high in resolved_ranges
        ],
        "original": original_block,
        "candidate": candidate_block,
    }
    if profile_obj is not None:
        obligation["platform_profile"] = str(profile_obj["platform_profile"])
        obligation["profile_sha256"] = str(profile_obj["profile_sha256"])
        obligation["regions"] = resolved_regions
    if environment is not None:
        obligation["memory_profile"] = environment.profile.value
    return obligation


def _validate_query_block(
    block: Any,
    *,
    label: str,
    require_terminals_when_unsat: bool = True,
) -> str | None:
    if not isinstance(block, dict):
        return f"{label} must be an object"
    result = block.get("result")
    if result not in ("unsat", "sat", "unknown", "not-queried"):
        return f"{label}.result must be unsat|sat|unknown|not-queried"
    if result == "not-queried":
        return None
    if not _is_sha256(block.get("query_sha256")):
        return f"{label}.query_sha256 must be a 64-hex digest"
    if block.get("status") == "vacuously-discharged":
        return None
    terminals = block.get("terminals")
    if result == "unsat" and require_terminals_when_unsat:
        if not isinstance(terminals, list):
            return f"{label}.terminals must be a list"
        # Empty terminals only valid for vacuous; already handled above.
        if terminals:
            for index, entry in enumerate(terminals):
                if not isinstance(entry, dict):
                    return f"{label}.terminals[{index}] must be an object"
                if entry.get("result") != "unsat":
                    return (
                        f"{label}.terminals[{index}].result must be 'unsat' "
                        f"(got {entry.get('result')!r})"
                    )
                if not _is_sha256(entry.get("query_sha256")):
                    return (
                        f"{label}.terminals[{index}].query_sha256 "
                        "must be a 64-hex digest"
                    )
            # Detect kind from label suffix.
            if "address_coverage" in label:
                kind = "bounded-memory-address-coverage-aggregate"
            elif "wraparound" in label:
                kind = "bounded-memory-wraparound-aggregate"
            elif "rom_write" in label:
                kind = "bounded-memory-rom-write-aggregate"
            else:
                kind = "bounded-memory-query-aggregate"
            expected = _aggregate_side_query_sha256(kind=kind, terminals=terminals)
            if block.get("query_sha256") != expected:
                return (
                    f"{label}.query_sha256 must match aggregate digest of "
                    "every terminal record"
                )
    return None


def validate_bounded_memory_obligation(obligation: Mapping[str, Any]) -> str | None:
    """Return None when a bounded-memory-v2 obligation is well-formed."""
    if not isinstance(obligation, Mapping):
        return "bounded_memory obligation must be an object"
    if obligation.get("algorithm") != BOUNDED_MEMORY_ALGORITHM:
        return f"bounded_memory.algorithm must be {BOUNDED_MEMORY_ALGORITHM}"
    if obligation.get("schema_version") != BOUNDED_MEMORY_OBLIGATION_SCHEMA_VERSION:
        return (
            "bounded_memory.schema_version must be "
            f"{BOUNDED_MEMORY_OBLIGATION_SCHEMA_VERSION}"
        )
    if obligation.get("capability") != "bounded-memory":
        return "bounded_memory.capability must be 'bounded-memory'"
    if obligation.get("model_version") != BOUNDED_MEMORY_MODEL_VERSION:
        return (
            "bounded_memory.model_version must be "
            f"{BOUNDED_MEMORY_MODEL_VERSION}"
        )
    if not _is_sha256(obligation.get("address_space_sha256")):
        return "bounded_memory.address_space_sha256 must be a 64-hex digest"

    source = obligation.get("source")
    if source not in (
        SOURCE_PLATFORM_PROFILE,
        SOURCE_AD_HOC,
        SOURCE_ASSUMED,
        SOURCE_INCOMPLETE,
    ):
        return "bounded_memory.source is unrecognized"

    for side in ("original", "candidate"):
        side_block = obligation.get(side)
        if not isinstance(side_block, dict):
            return f"bounded_memory.{side} must be an object"
        for key in ("address_coverage", "wraparound"):
            reason = _validate_query_block(
                side_block.get(key),
                label=f"bounded_memory.{side}.{key}",
            )
            if reason is not None:
                return reason
        if "rom_write" in side_block:
            reason = _validate_query_block(
                side_block.get("rom_write"),
                label=f"bounded_memory.{side}.rom_write",
            )
            if reason is not None:
                return reason

    status = obligation.get("status")
    if status == "promotion-grade":
        if source != SOURCE_PLATFORM_PROFILE:
            return (
                "bounded_memory.status=promotion-grade requires "
                f"source={SOURCE_PLATFORM_PROFILE}"
            )
        profile_name = obligation.get("platform_profile")
        profile_digest = obligation.get("profile_sha256")
        if not isinstance(profile_name, str) or profile_name not in REVIEWED_PLATFORM_PROFILES:
            return (
                "bounded_memory.status=promotion-grade requires a reviewed "
                "platform_profile"
            )
        if not _is_sha256(profile_digest):
            return "bounded_memory.profile_sha256 must be a 64-hex digest"
        try:
            reviewed = load_platform_profile(profile_name)
        except (OSError, ValueError, json.JSONDecodeError) as exc:
            return f"bounded_memory platform profile load failed: {exc}"
        if reviewed.get("profile_sha256") != profile_digest:
            return (
                "bounded_memory.profile_sha256 does not match reviewed "
                f"platform profile {profile_name}"
            )
        for side in ("original", "candidate"):
            side_block = obligation[side]
            assert isinstance(side_block, dict)
            for key in ("address_coverage", "wraparound"):
                if side_block[key].get("result") != "unsat":
                    return (
                        f"bounded_memory.status=promotion-grade requires "
                        f"{side}.{key}.result=unsat"
                    )
            rom = side_block.get("rom_write")
            if isinstance(rom, dict) and rom.get("result") != "unsat":
                return (
                    f"bounded_memory.status=promotion-grade requires "
                    f"{side}.rom_write.result=unsat"
                )
    return None


def obligation_is_promotion_grade(obligation: Mapping[str, Any]) -> bool:
    """True when obligation validates and claims promotion-grade honestly."""
    if validate_bounded_memory_obligation(obligation) is not None:
        return False
    return obligation.get("status") == "promotion-grade"


def recompute_bounded_memory_attestation_status(
    evidence: Mapping[str, Any],
    *,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
    model_version: str = "",
    allowed_versions: Sequence[str] = (),
) -> str:
    """Recompute attestation grade from obligation evidence (ignore caller status)."""
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        STATUS_PROMOTION_GRADE,
        STATUS_SCOPED_ASSUMPTION,
    )

    if unsupported:
        return STATUS_INCOMPLETE
    # Empty allowlist fail-closes (same posture as opcode-ledger-v2).
    if model_version not in tuple(allowed_versions):
        return STATUS_INCOMPLETE

    obligation = evidence.get("bounded_memory") or evidence.get("obligation")
    if not isinstance(obligation, dict):
        return STATUS_INCOMPLETE
    reason = validate_bounded_memory_obligation(obligation)
    if reason is not None:
        return STATUS_INCOMPLETE

    source = obligation.get("source")
    if source == SOURCE_ASSUMED:
        return STATUS_SCOPED_ASSUMPTION if assumptions else STATUS_INCOMPLETE
    if source == SOURCE_AD_HOC:
        # Ad-hoc / CLI ranges never promotion-grade.
        return STATUS_SCOPED_ASSUMPTION if (
            obligation.get("original", {}).get("address_coverage", {}).get("result")
            == "unsat"
        ) else STATUS_INCOMPLETE
    if obligation_is_promotion_grade(obligation):
        if assumptions:
            return STATUS_SCOPED_ASSUMPTION
        return STATUS_PROMOTION_GRADE
    return STATUS_INCOMPLETE


def build_bounded_memory_attestation(
    obligation: Mapping[str, Any],
    *,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
) -> Any:
    """Emit a ``bounded-memory`` CapabilityAttestation from a discharged obligation."""
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        build_attestation,
    )

    status = recompute_bounded_memory_attestation_status(
        {"bounded_memory": dict(obligation)},
        assumptions=assumptions,
        unsupported=unsupported,
        model_version=str(obligation.get("model_version", BOUNDED_MEMORY_MODEL_VERSION)),
    )
    # Advisory only — evaluate_capability_assurance recomputes.
    return build_attestation(
        capability="bounded-memory",
        model_version=BOUNDED_MEMORY_MODEL_VERSION,
        algorithm=BOUNDED_MEMORY_ATTESTATION_ALGORITHM,
        status=status if status else STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported),
        evidence={
            "bounded_memory": dict(obligation),
            "address_space_sha256": obligation.get("address_space_sha256"),
            "source": obligation.get("source"),
            "platform_profile": obligation.get("platform_profile"),
            "profile_sha256": obligation.get("profile_sha256"),
        },
    )

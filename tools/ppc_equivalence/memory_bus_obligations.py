"""SMT obligations and proof metadata for opt-in ``MemoryBus`` routing.

MMIO register-bank / FIFO formulas live in ``symbolic_bus`` /
``symbolic_event_trace``. Reachable unsupported accesses are separate proof
queries (``path ∧ ¬supported``); ``supported`` is never assumed into the
equivalence query. CFG routing is live for SymbolicOps. ``memory-bus`` may
authorize ``EQUIVALENT`` only when the engine attaches ``status=discharged``
with schema v2 attestations (Track D enablement gate).

Discharged obligations (``status=discharged``) require schema v2 attestations:
``bus_spec_sha256``, per-side unsupported-access UNSAT or vacuous digests,
theory blocks (or explicit none), device-state-in-compare attestation, and
access-coverage attestation. Caller-forged discharged claims without digests
fail closed.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Sequence

from tools.ppc_equivalence.address_space import RegionKind
from tools.ppc_equivalence.jump_table_obligations import (
    rom_image_byte_constraints,
    rom_image_no_write_constraints,
)
from tools.ppc_equivalence.memory_bus import MemoryBus
from tools.ppc_equivalence.memory_profile import access_within_any_range
from tools.ppc_equivalence.provenance import canonical_json_sha256

# Schema v2 adds discharged attestations (vacuous coverage, theory blocks,
# device-state-in-compare). Architecture model is unchanged (Track D).
MEMORY_BUS_OBLIGATION_SCHEMA_VERSION = 2
MEMORY_BUS_ALGORITHM = "memory-bus-v1"

# Track C coverage / rejection policy (additive; discharged shape owned by Track A).
LOOP_FIFO_POLICY = "hard-reject"
LOOP_FIFO_EMISSION = "unsupported"
LOOP_FIFO_REJECTION_REASON = "symbolic-loop-fifo-emission"
MIXED_SPACE_POLICY = "fail-closed"
MIXED_SPACE_REJECTION_REASON = "symbolic-mmio-mixed-address-space"

_SHA256_LEN = 64


def _bus_has_fifo_devices(memory_bus: MemoryBus) -> bool:
    spec = memory_bus.specification
    if spec is None:
        return False
    return any(device.theory == "gxfifo-stream" for device in spec.devices)


def _bus_has_mmio_regions(memory_bus: MemoryBus) -> bool:
    return any(
        region.kind is RegionKind.MMIO for region in memory_bus.address_space.regions
    )


def _bus_has_non_mmio_regions(memory_bus: MemoryBus) -> bool:
    return any(
        region.kind is not RegionKind.MMIO
        for region in memory_bus.address_space.regions
    )


def memory_bus_coverage_policies(memory_bus: MemoryBus) -> dict[str, Any]:
    """Static coverage attestation for loop×FIFO and mixed-space policies.

    Bounded summarized FIFO emission is intentionally unsupported: the CFG
    hard-rejects when loop summaries coexist with FIFO devices. Pure-MMIO
    symbolic ``addr == register`` routing remains allowed; mixed RAM/ROM+MMIO
    symbolic addresses stay fail-closed.
    """
    coverage: dict[str, Any] = {
        "mixed_space_symbolic_mmio": MIXED_SPACE_POLICY,
        "pure_mmio_symbolic": "supported",
        "bounded_summarized_fifo_emission": LOOP_FIFO_EMISSION,
        "note": (
            "Pure-MMIO symbolic routing exists; mixed RAM/ROM/MMIO symbolic "
            "addresses remain fail-closed. Loop-summary × FIFO has a hard "
            "rejection; bounded summarized emission is not supported."
        ),
    }
    if _bus_has_fifo_devices(memory_bus):
        coverage["loop_fifo_policy"] = LOOP_FIFO_POLICY
        coverage["loop_fifo_emission"] = LOOP_FIFO_EMISSION
    return coverage


@dataclass(frozen=True, slots=True)
class RomImageBinding:
    """Minimal ROM image handle for ``rom_image_*`` constraint helpers."""

    base: int
    image_bytes: bytes
    source: str = "memory-bus"
    label: str | None = None

    @property
    def byte_count(self) -> int:
        return len(self.image_bytes)


def build_memory_bus_constraints(
    memory_bus: MemoryBus,
    initial_memory: Any,
    terminals: Sequence[Any],
    ops: Any,
) -> list[Any]:
    """Bind symbolic memory to the bus address space (Tier C, fail-closed).

    MMIO touches that hit declared devices are excluded from the RAM/ROM range
    check — device routing / unsupported-access digests own those addresses.
    """
    z3 = ops.z3
    constraints: list[Any] = []
    allowed_ranges: list[tuple[int, int]] = []
    mmio_ranges: list[tuple[int, int]] = []

    for region in memory_bus.address_space.regions:
        if region.kind is RegionKind.ROM_IMAGE:
            assert region.image_bytes is not None
            binding = RomImageBinding(
                base=region.start,
                image_bytes=region.image_bytes,
                label=region.label,
            )
            constraints.extend(
                rom_image_byte_constraints(initial_memory, binding, ops)
            )
            constraints.extend(
                rom_image_no_write_constraints(
                    terminals, initial_memory, binding, ops,
                )
            )
            allowed_ranges.append((region.start, region.end))
        elif region.kind is RegionKind.RAM:
            allowed_ranges.append((region.start, region.end))
        elif region.kind is RegionKind.MMIO and region.device_id is not None:
            mmio_ranges.append((region.start, region.end))

    if not allowed_ranges and not mmio_ranges:
        return [z3.BoolVal(False)]

    seen: set[int] = set()
    for terminal in terminals:
        for addr in terminal.state.memory_touches:
            key = hash(addr)
            if key in seen:
                continue
            seen.add(key)
            if allowed_ranges and mmio_ranges:
                in_ram_rom = access_within_any_range(addr, 1, allowed_ranges)
                in_mmio = access_within_any_range(addr, 1, mmio_ranges)
                constraints.append(
                    z3.Implies(terminal.condition, z3.Or(in_ram_rom, in_mmio))
                )
            elif allowed_ranges:
                constraints.append(
                    z3.Implies(
                        terminal.condition,
                        access_within_any_range(addr, 1, allowed_ranges),
                    )
                )
            else:
                constraints.append(
                    z3.Implies(
                        terminal.condition,
                        access_within_any_range(addr, 1, mmio_ranges),
                    )
                )
    return constraints


def build_memory_bus_obligation(memory_bus: MemoryBus) -> dict[str, Any]:
    """Proof obligation block for ``proof_features: [\"memory-bus\"]``."""
    regions: list[dict[str, Any]] = []
    for region in memory_bus.address_space.regions:
        entry: dict[str, Any] = {
            "kind": region.kind.value,
            "start": region.start,
            "end": region.end,
        }
        if region.label is not None:
            entry["label"] = region.label
        if region.kind is RegionKind.ROM_IMAGE and region.image_sha256 is not None:
            entry["image_sha256"] = region.image_sha256
        if region.kind is RegionKind.MMIO and region.device_id is not None:
            entry["device_id"] = region.device_id
        regions.append(entry)
    obligation: dict[str, Any] = {
        "schema_version": MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
        "algorithm": MEMORY_BUS_ALGORITHM,
        "status": "pending",
        "regions": regions,
        "load_kinds": ["ram", "rom-image", "mmio-register-bank"],
        "store_kinds": ["ram", "mmio-register-bank", "gxfifo-stream"],
        "mmio": "cfg-routed",
        "unmapped": "fail-closed",
        "cfg_rejection_reasons": [],
        "loop_fifo_reject_markers": [],
        "mixed_space_symbolic_mmio": MIXED_SPACE_POLICY,
        "coverage": memory_bus_coverage_policies(memory_bus),
    }
    if _bus_has_fifo_devices(memory_bus):
        obligation["loop_fifo_policy"] = LOOP_FIFO_POLICY
        obligation["loop_fifo_emission"] = LOOP_FIFO_EMISSION
    spec = memory_bus.specification
    if spec is not None:
        obligation["bus_spec_sha256"] = spec.sha256()
        # Optional rebuild payload so JSON-only validators can recompute the
        # digest without a live MemoryBus.
        obligation["bus_spec_canonical"] = spec.canonical_dict()
    return obligation


def symbolic_mmio_still_fail_closed() -> bool:
    """Hook marker for pure-MMIO promotion authorization.

    Returns ``False`` after Track D unfreeze: ``memory-bus`` may authorize
    ``EQUIVALENT`` when the engine builds ``status=discharged``. Mixed
    RAM/ROM/MMIO symbolic addresses remain fail-closed at the CFG layer.
    """
    from tools.ppc_equivalence.proof_features import UNSUPPORTED_FOR_EQUIVALENT

    return "memory-bus" in UNSUPPORTED_FOR_EQUIVALENT


def _device_theory_map(memory_bus: MemoryBus) -> dict[str, str]:
    spec = memory_bus.specification
    if spec is None:
        return {}
    return {device.device_id: device.theory for device in spec.devices}


def _serialize_mmio_touch(touch: Any) -> dict[str, Any]:
    payload: dict[str, Any] = {
        "device_id": touch.device_id,
        "theory": touch.theory,
        "access": touch.access,
        "width": touch.width,
        "addr": touch.addr,
    }
    if touch.register_offset is not None:
        payload["register_offset"] = hex(touch.register_offset)
    return payload


def _serialize_symbolic_bus_observability(
    terminals: Sequence[Any] | None,
) -> dict[str, Any]:
    """Collect final symbolic register-bank / FIFO snapshots from terminals."""
    banks: dict[str, Any] = {}
    fifo: dict[str, Any] = {}
    if not terminals:
        return {"register_banks": banks, "fifo_traces": fifo}
    for terminal in terminals:
        bus = getattr(terminal.state, "symbolic_bus", None)
        if bus is None:
            continue
        for device_id, bank in bus.banks.items():
            if device_id not in bus.touched_devices:
                continue
            banks[device_id] = {
                hex(offset): str(value)
                for offset, value in sorted(bank.values.items())
            }
        for device_id, trace in bus.fifo_traces.items():
            if device_id not in bus.touched_devices:
                continue
            fifo[device_id] = [
                {
                    "addr": str(event.addr),
                    "width": event.width,
                    "value": str(event.value),
                }
                for event in trace.writes
            ]
    return {"register_banks": banks, "fifo_traces": fifo}


def collect_bus_mmio_observability(
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
) -> dict[str, Any]:
    """Collect touched MMIO evidence and live device snapshots for obligations."""
    from tools.ppc_equivalence.symbolic_bus import collect_mmio_touches_from_terminals

    theories = _device_theory_map(memory_bus)
    observability: dict[str, Any] = {
        "register_banks": {},
        "fifo_traces": {},
        "symbolic": {
            "original": _serialize_symbolic_bus_observability(original_terminals),
            "candidate": _serialize_symbolic_bus_observability(candidate_terminals),
        },
        "touches": {"original": [], "candidate": []},
    }

    if original_terminals is not None:
        for touch in collect_mmio_touches_from_terminals(
            original_terminals,
            memory_bus.address_space,
            side="original",
            device_theories=theories,
            device_widths=_device_width_map(memory_bus),
        ):
            observability["touches"]["original"].append(_serialize_mmio_touch(touch))
    if candidate_terminals is not None:
        for touch in collect_mmio_touches_from_terminals(
            candidate_terminals,
            memory_bus.address_space,
            side="candidate",
            device_theories=theories,
            device_widths=_device_width_map(memory_bus),
        ):
            observability["touches"]["candidate"].append(_serialize_mmio_touch(touch))

    state = memory_bus.snapshot_state()
    for device_id, values in state.device_values.items():
        observability["register_banks"][device_id] = {
            hex(offset): hex(value) for offset, value in sorted(values.items())
        }
    for device_id, events in state.event_logs.items():
        observability["fifo_traces"][device_id] = list(events)
    return observability


def _device_width_map(memory_bus: MemoryBus) -> dict[str, int]:
    spec = memory_bus.specification
    if spec is None:
        return {}
    widths: dict[str, int] = {}
    for device in spec.devices:
        if device.theory == "register-bank":
            widths[device.device_id] = device.reg_width
        elif device.theory == "gxfifo-stream":
            widths[device.device_id] = 4
    return widths


def _terminals_have_symbolic_bus(terminals: Sequence[Any] | None) -> bool:
    if not terminals:
        return False
    return any(getattr(t.state, "symbolic_bus", None) is not None for t in terminals)


def _touched_device_ids(
    original_terminals: Sequence[Any] | None,
    candidate_terminals: Sequence[Any] | None,
) -> set[str]:
    touched: set[str] = set()
    for terminals in (original_terminals, candidate_terminals):
        if not terminals:
            continue
        for terminal in terminals:
            bus = getattr(terminal.state, "symbolic_bus", None)
            if bus is None:
                continue
            touched.update(bus.touched_devices)
    return touched


def _side_coverage_identity(
    *,
    attested: bool,
    status: str,
    opcode_families: Sequence[str],
    rejections: Sequence[str],
) -> dict[str, Any]:
    """Canonical per-side coverage fields (digest excludes ``sha256``)."""
    return {
        "schema_version": 1,
        "status": status,
        "attested": attested,
        "opcode_families": list(opcode_families),
        "rejections": list(rejections),
    }


def build_side_access_coverage(
    *,
    attested: bool = True,
    opcode_families: Sequence[str] | None = None,
    rejections: Sequence[str] | None = None,
    status: str = "stubbed",
) -> dict[str, Any]:
    """One side's access-coverage attestation with recomputable ``sha256``."""
    identity = _side_coverage_identity(
        attested=attested,
        status=status,
        opcode_families=list(opcode_families or []),
        rejections=list(rejections or []),
    )
    return {
        **identity,
        "sha256": canonical_json_sha256(identity),
    }


def _merge_side_coverage(
    *,
    side: str,
    attested: bool,
    instructions: Sequence[Any] | None,
    opcode_families: Sequence[str] | None,
    rejections: Sequence[str] | None,
    status: str,
) -> dict[str, Any]:
    """Build one side: prefer static opcodes, merge dynamic routed/rejects."""
    from tools.ppc_equivalence.bus_access import (
        side_bus_access_coverage,
        static_opcode_families,
    )

    families: list[str] = []
    rejection_list: list[str] = []
    side_status = status
    sources: list[str] = []

    if opcode_families is not None:
        families = sorted(set(opcode_families))
        sources.append("explicit")
    elif instructions is not None:
        families = static_opcode_families(instructions)
        sources.append("static")

    # Pure stub requests (no instructions / families) must not inherit leftover
    # ContextVar dynamic snaps from earlier CFG runs in the same process.
    allow_dynamic = not (
        status == "stubbed"
        and instructions is None
        and opcode_families is None
        and rejections is None
    )
    dynamic = side_bus_access_coverage(side) if allow_dynamic else None  # type: ignore[arg-type]
    if dynamic is not None:
        dyn_families = list(
            dynamic.get("opcode_families") or dynamic.get("families") or []
        )
        families = sorted(set(families) | set(dyn_families))
        if rejections is None:
            rejection_list = list(dynamic.get("rejections") or [])
        sources.append("dynamic")
        if side_status == "stubbed":
            side_status = "observed"

    if rejections is not None:
        rejection_list = list(rejections)

    # Discharged validator requires ``complete``. Prefer ``complete`` when both
    # static decode and dynamic routing evidence exist; ``observed`` remains
    # valid only for pending/diagnostic (non-discharged) attestations.
    if sources:
        if "static" in sources and "dynamic" in sources:
            side_status = "complete"
        elif side_status in ("stubbed", "static", "static+dynamic"):
            side_status = "observed"

    return build_side_access_coverage(
        attested=attested,
        opcode_families=families,
        rejections=rejection_list,
        status=side_status,
    )


def build_access_coverage_attestation(
    *,
    attested: bool = True,
    opcode_families: Sequence[str] | None = None,
    rejections: Sequence[str] | None = None,
    status: str = "stubbed",
    original: dict[str, Any] | None = None,
    candidate: dict[str, Any] | None = None,
    original_instructions: Sequence[Any] | None = None,
    candidate_instructions: Sequence[Any] | None = None,
    original_opcode_families: Sequence[str] | None = None,
    candidate_opcode_families: Sequence[str] | None = None,
    original_rejections: Sequence[str] | None = None,
    candidate_rejections: Sequence[str] | None = None,
) -> dict[str, Any]:
    """Per-side access-coverage attestation (Track B opcode families + rejects).

    Shape (Finding 2 / Track C)::

        {
          "schema_version": 1,
          "attested": true,
          "status": "observed"|"complete",
          "original": {"opcode_families": [...], "rejections": [...], "sha256": "...", ...},
          "candidate": {...},
          "sides": {"original": <same>, "candidate": <same>},
          "sha256": "<aggregate of per-side digests>"
        }

    Prefer static decoded-opcode families from ``*_instructions``. Dynamic
    routed/rejected evidence is merged from ``bus_access.side_bus_access_coverage``
    when present. Legacy flat ``opcode_families`` (no sides) is mirrored onto
    both sides for transitional callers.
    """
    if original is None:
        # Legacy single-list kwargs: apply to both sides when no per-side input.
        legacy_both = (
            opcode_families is not None
            and original_instructions is None
            and candidate_instructions is None
            and original_opcode_families is None
            and candidate_opcode_families is None
        )
        original = _merge_side_coverage(
            side="original",
            attested=attested,
            instructions=original_instructions,
            opcode_families=(
                opcode_families if legacy_both else original_opcode_families
            ),
            rejections=(
                rejections if legacy_both and original_rejections is None
                else original_rejections
            ),
            status=status,
        )
    if candidate is None:
        legacy_both = (
            opcode_families is not None
            and original_instructions is None
            and candidate_instructions is None
            and original_opcode_families is None
            and candidate_opcode_families is None
        )
        candidate = _merge_side_coverage(
            side="candidate",
            attested=attested,
            instructions=candidate_instructions,
            opcode_families=(
                opcode_families if legacy_both else candidate_opcode_families
            ),
            rejections=(
                rejections if legacy_both and candidate_rejections is None
                else candidate_rejections
            ),
            status=status,
        )

    side_statuses = {original.get("status"), candidate.get("status")}
    if side_statuses <= {"complete"}:
        parent_status = "complete"
    elif "complete" in side_statuses or "observed" in side_statuses:
        parent_status = "observed"
    elif status not in ("stubbed", None, ""):
        parent_status = status
    elif original.get("opcode_families") or candidate.get("opcode_families"):
        parent_status = "observed"
    else:
        parent_status = status if status else "stubbed"

    aggregate = canonical_json_sha256(
        {
            "kind": "access-coverage-aggregate",
            "schema_version": 1,
            "original": original["sha256"],
            "candidate": candidate["sha256"],
        }
    )
    sides = {"original": original, "candidate": candidate}
    return {
        "schema_version": 1,
        "status": parent_status,
        "attested": attested,
        "original": original,
        "candidate": candidate,
        # Track C alias: same objects under ``sides``.
        "sides": sides,
        "sha256": aggregate,
    }


def access_coverage_side_digest(
    access_coverage: dict[str, Any] | None,
    side: str,
) -> str | None:
    """Return the recomputable per-side coverage digest, or legacy top-level."""
    if not isinstance(access_coverage, dict):
        return None
    # Prefer Finding 2 top-level original/candidate, then Track C ``sides``.
    side_block = access_coverage.get(side)
    if isinstance(side_block, dict) and isinstance(side_block.get("sha256"), str):
        return side_block["sha256"]
    nested = access_coverage.get("sides")
    if isinstance(nested, dict):
        side_block = nested.get(side)
        if isinstance(side_block, dict) and isinstance(side_block.get("sha256"), str):
            return side_block["sha256"]
    digest = access_coverage.get("sha256")
    return digest if isinstance(digest, str) else None


def build_device_state_in_compare_attestation(
    *,
    included: bool,
    observability: dict[str, Any] | None = None,
) -> dict[str, Any]:
    """Attest that touched device/FIFO state entered the final compare."""
    digest = None
    if observability is not None:
        digest = canonical_json_sha256(
            {
                "kind": "device-state-in-compare",
                "observability": observability,
            }
        )
    return {
        "included": included,
        "digest_sha256": digest,
    }


def _theory_none_block() -> dict[str, Any]:
    return {"status": "none", "devices": []}


def _merge_unsupported_sides(
    cfg_side: dict[str, Any],
    observed_side: dict[str, Any],
) -> dict[str, Any]:
    """Prefer CFG digests (including vacuous); else observed-touch queries.

    Never rewrite empty CFG predicates into forged ``result=unsat``.
    """
    if cfg_side.get("status") == "vacuously-discharged":
        return cfg_side
    if cfg_side.get("result") not in (None, "not-queried"):
        return cfg_side
    return observed_side


def discharge_observed_mmio_unsupported_accesses(
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
    ops: Any | None = None,
    deadline: Any | None = None,
    access_coverage_sha256: str | None = None,
    access_coverage: dict[str, Any] | None = None,
) -> dict[str, Any]:
    """Per-terminal unsupported-access queries for MMIO touches / CFG predicates.

    Prefers CFG-collected ``unsupported_predicates`` on ``SymbolicBusState``.
    Falls back to concrete observed-touch discharge. Does not assume
    ``supported`` into the equivalence query. Empty CFG predicates emit
    vacuous discharge (not forged UNSAT) when coverage is attested.

    Prefer per-side digests from ``access_coverage``; fall back to a shared
    ``access_coverage_sha256`` for transitional callers.
    """
    from tools.ppc_equivalence.bus_spec import lift_symbolic_register_banks
    from tools.ppc_equivalence.deadline import Deadline
    from tools.ppc_equivalence.symbolic_bus import (
        access_supported,
        collect_mmio_touches_from_terminals,
        discharge_cfg_unsupported_accesses,
        discharge_unsupported_access,
    )

    empty = {
        "original": {"result": "not-queried", "query_sha256": None, "terminals": []},
        "candidate": {"result": "not-queried", "query_sha256": None, "terminals": []},
    }
    spec = memory_bus.specification
    if spec is None or ops is None or getattr(ops, "z3", None) is None:
        return empty

    z3 = ops.z3
    effective_deadline = deadline if deadline is not None else Deadline.after_ms(5_000)

    original_cov = (
        access_coverage_side_digest(access_coverage, "original")
        or access_coverage_sha256
    )
    candidate_cov = (
        access_coverage_side_digest(access_coverage, "candidate")
        or access_coverage_sha256
    )

    cfg_original = discharge_cfg_unsupported_accesses(
        original_terminals or (),
        side="original",
        deadline=effective_deadline,
        z3=z3,
        access_coverage_sha256=original_cov,
    )
    cfg_candidate = discharge_cfg_unsupported_accesses(
        candidate_terminals or (),
        side="candidate",
        deadline=effective_deadline,
        z3=z3,
        access_coverage_sha256=candidate_cov,
    )

    banks = lift_symbolic_register_banks(spec, z3)
    theories = _device_theory_map(memory_bus)
    widths = _device_width_map(memory_bus)

    def _observed_side(
        terminals: Sequence[Any] | None,
        *,
        side: str,
    ) -> dict[str, Any]:
        if terminals is None:
            return {"result": "not-queried", "query_sha256": None, "terminals": []}
        touches = collect_mmio_touches_from_terminals(
            terminals,
            memory_bus.address_space,
            side=side,
            device_theories=theories,
            device_widths=widths,
        )
        if not touches:
            return {"result": "not-queried", "query_sha256": None, "terminals": []}

        from tools.ppc_equivalence.symbolic_bus import (
            aggregate_unsupported_access_query_sha256,
        )

        terminal_results: list[dict[str, Any]] = []
        worst = "unsat"
        total_elapsed_ms = 0.0
        for touch in touches:
            if touch.theory != "register-bank":
                continue
            bank = banks.get(touch.device_id)
            if bank is None:
                continue
            try:
                addr_int = int(touch.addr, 0) & 0xFFFFFFFF
            except (TypeError, ValueError):
                continue
            addr = z3.BitVecVal(addr_int, 32)
            supported = access_supported(addr, bank, z3, width=max(1, int(touch.width)))
            query = discharge_unsupported_access(
                path_condition=z3.BoolVal(True),
                supported=supported,
                deadline=effective_deadline,
                z3=z3,
            )
            total_elapsed_ms += query.elapsed_ms
            terminal_results.append(
                {
                    "device_id": touch.device_id,
                    "addr": touch.addr,
                    "width": touch.width,
                    "result": query.result,
                    "query_sha256": query.query_sha256,
                    "inconclusive": query.inconclusive,
                    "solver": {
                        "name": "z3",
                        "version": z3.get_version_string(),
                        "elapsed_ms": query.elapsed_ms,
                    },
                }
            )
            if query.status.value == "sat":
                worst = "sat"
            elif query.status.value == "unknown" and worst == "unsat":
                worst = "unknown"
        if not terminal_results:
            return {"result": "not-queried", "query_sha256": None, "terminals": []}
        return {
            "result": worst,
            "query_sha256": aggregate_unsupported_access_query_sha256(terminal_results),
            "solver": {
                "name": "z3",
                "version": z3.get_version_string(),
                "elapsed_ms": total_elapsed_ms,
            },
            "terminals": terminal_results,
        }

    return {
        "original": _merge_unsupported_sides(
            cfg_original,
            _observed_side(original_terminals, side="original"),
        ),
        "candidate": _merge_unsupported_sides(
            cfg_candidate,
            _observed_side(candidate_terminals, side="candidate"),
        ),
    }


def _cfg_rejections(terminals: Sequence[Any] | None) -> list[str]:
    reasons: list[str] = []
    if not terminals:
        return reasons
    for terminal in terminals:
        bus = getattr(terminal.state, "symbolic_bus", None)
        if bus is None:
            continue
        reasons.extend(bus.rejections)
    return reasons


def _loop_fifo_reject_markers(terminals: Sequence[Any] | None) -> list[str]:
    """Track C hook: collect loop×FIFO rejection markers from CFG state."""
    markers: list[str] = []
    if not terminals:
        return markers
    for terminal in terminals:
        bus = getattr(terminal.state, "symbolic_bus", None)
        if bus is None:
            continue
        for reason in bus.rejections:
            if "fifo" in reason or "loop" in reason:
                markers.append(reason)
    return markers


def _side_is_discharged_form(side: dict[str, Any]) -> bool:
    if side.get("status") == "vacuously-discharged":
        return True
    return side.get("result") == "unsat" and isinstance(side.get("query_sha256"), str)


def enrich_memory_bus_obligation_with_symbolic_mmio(
    obligation: dict[str, Any],
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
    original_instructions: Sequence[Any] | None = None,
    candidate_instructions: Sequence[Any] | None = None,
    ops: Any | None = None,
    deadline: Any | None = None,
    loop_summaries_active: bool = False,
) -> dict[str, Any]:
    """Attach PR-14/15 CFG routing blocks and MMIO observability.

    ``loop_summaries_active`` records the CFG hard-reject when affine/memory
    loop summaries coexist with FIFO devices (even if ``execute_cfg`` raised
    before producing terminals).

    ``original_instructions`` / ``candidate_instructions`` feed static
    per-side opcode-family coverage (preferred over last-execution-only).
    """
    from tools.ppc_equivalence.symbolic_bus import build_register_bank_extensional_obligation
    from tools.ppc_equivalence.symbolic_event_trace import build_gxfifo_trace_obligation

    spec = memory_bus.specification
    if spec is None:
        return obligation

    enriched = dict(obligation)
    enriched["schema_version"] = MEMORY_BUS_OBLIGATION_SCHEMA_VERSION
    enriched.setdefault("algorithm", MEMORY_BUS_ALGORITHM)

    # Track C static policy attestation (additive / idempotent).
    coverage = dict(enriched.get("coverage") or {})
    coverage.update(memory_bus_coverage_policies(memory_bus))
    enriched["coverage"] = coverage
    enriched["mixed_space_symbolic_mmio"] = MIXED_SPACE_POLICY
    if _bus_has_fifo_devices(memory_bus):
        enriched["loop_fifo_policy"] = LOOP_FIFO_POLICY
        enriched["loop_fifo_emission"] = LOOP_FIFO_EMISSION

    rejections = _cfg_rejections(original_terminals) + _cfg_rejections(candidate_terminals)

    # Loop-summary × FIFO: hard-reject attestation (CFG raises before terminals).
    if _bus_has_fifo_devices(memory_bus) and loop_summaries_active:
        if LOOP_FIFO_REJECTION_REASON not in rejections:
            rejections.append(LOOP_FIFO_REJECTION_REASON)
        enriched["loop_fifo_rejection"] = {
            "reason": LOOP_FIFO_REJECTION_REASON,
            "policy": LOOP_FIFO_POLICY,
            "status": "hard-rejected",
            "bounded_summarized_emission": LOOP_FIFO_EMISSION,
        }
        coverage["loop_fifo"] = "hard-rejected"
        enriched["coverage"] = coverage

    # Mixed-space symbolic MMIO: explicit coverage/rejection reason (not silent).
    if MIXED_SPACE_REJECTION_REASON in rejections:
        enriched["mixed_space_rejection"] = MIXED_SPACE_REJECTION_REASON
        coverage["mixed_space"] = "rejected"
        enriched["coverage"] = coverage
    elif _bus_has_mmio_regions(memory_bus) and _bus_has_non_mmio_regions(memory_bus):
        coverage.setdefault("mixed_space", "fail-closed-policy")
        enriched["coverage"] = coverage

    rejection_reasons = sorted(set(rejections))
    enriched["cfg_rejection_reasons"] = rejection_reasons
    # Back-compat alias used by engine demotion path.
    if rejection_reasons:
        enriched["cfg_rejections"] = rejection_reasons
        enriched["symbolic_mmio"] = "cfg-routed-rejected"
    else:
        enriched["symbolic_mmio"] = "cfg-routed"

    loop_markers = sorted(
        set(
            _loop_fifo_reject_markers(original_terminals)
            + _loop_fifo_reject_markers(candidate_terminals)
        )
    )
    if LOOP_FIFO_REJECTION_REASON in rejection_reasons:
        if LOOP_FIFO_REJECTION_REASON not in loop_markers:
            loop_markers.append(LOOP_FIFO_REJECTION_REASON)
        loop_markers = sorted(set(loop_markers))
    enriched["loop_fifo_reject_markers"] = loop_markers

    observability = collect_bus_mmio_observability(
        memory_bus,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
    )
    enriched["observability"] = observability

    # Per-side static opcode coverage (+ dynamic side snaps when present).
    # Discharged validators reject stubbed coverage status.
    access_coverage = build_access_coverage_attestation(
        attested=True,
        status="stubbed",
        original_instructions=original_instructions,
        candidate_instructions=candidate_instructions,
    )
    if access_coverage.get("status") == "stubbed":
        access_coverage = build_access_coverage_attestation(
            attested=True,
            status="observed",
            original=build_side_access_coverage(
                attested=True,
                opcode_families=list(
                    (access_coverage.get("original") or {}).get("opcode_families") or []
                ),
                rejections=list(
                    (access_coverage.get("original") or {}).get("rejections") or []
                ),
                status="observed",
            ),
            candidate=build_side_access_coverage(
                attested=True,
                opcode_families=list(
                    (access_coverage.get("candidate") or {}).get("opcode_families") or []
                ),
                rejections=list(
                    (access_coverage.get("candidate") or {}).get("rejections") or []
                ),
                status="observed",
            ),
        )
    enriched["access_coverage"] = access_coverage

    cfg_active = _terminals_have_symbolic_bus(original_terminals) or _terminals_have_symbolic_bus(
        candidate_terminals
    )
    enriched["device_state_in_compare"] = build_device_state_in_compare_attestation(
        included=cfg_active,
        observability=observability if cfg_active else None,
    )

    unsupported_access = discharge_observed_mmio_unsupported_accesses(
        memory_bus,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
        ops=ops,
        deadline=deadline,
        access_coverage=access_coverage,
    )
    # RAM/ROM-only buses never lift a SymbolicBusState. When both sides ran a
    # CFG (terminals present), access coverage is attested, and there are no
    # MMIO devices, emit vacuous unsupported-access so discharge can complete.
    if (
        not _bus_has_mmio_regions(memory_bus)
        and access_coverage.get("attested") is True
        and original_terminals
        and candidate_terminals
    ):
        from tools.ppc_equivalence.symbolic_bus import (
            cfg_trace_sha256,
            vacuous_unsupported_access_block,
        )

        for side_name, terminals in (
            ("original", original_terminals),
            ("candidate", candidate_terminals),
        ):
            side_digest = access_coverage_side_digest(access_coverage, side_name)
            if not isinstance(side_digest, str):
                continue
            side = unsupported_access.get(side_name) or {}
            if side.get("result") in (None, "not-queried") and side.get("status") != (
                "vacuously-discharged"
            ):
                unsupported_access[side_name] = vacuous_unsupported_access_block(
                    cfg_trace_sha256=cfg_trace_sha256(terminals),
                    access_coverage_sha256=side_digest,
                )
    enriched["unsupported_access"] = unsupported_access
    if "bus_spec_sha256" not in enriched:
        enriched["bus_spec_sha256"] = spec.sha256()
    if "bus_spec_canonical" not in enriched:
        enriched["bus_spec_canonical"] = spec.canonical_dict()

    touched = _touched_device_ids(original_terminals, candidate_terminals)
    register_devices: list[dict[str, Any]] = []
    fifo_devices: list[dict[str, Any]] = []
    for device in spec.devices:
        for region in spec.address_space.regions:
            if (
                region.kind is RegionKind.MMIO
                and region.device_id == device.device_id
            ):
                entry = {
                    "device_id": device.device_id,
                    "theory": device.theory,
                    "region": {"start": region.start, "end": region.end},
                }
                if device.theory == "register-bank":
                    register_devices.append(entry)
                elif device.theory == "gxfifo-stream":
                    fifo_devices.append(entry)

    # Theory blocks: include when devices exist (touched or declared); else none.
    if register_devices:
        enriched["register_bank_theory"] = {
            "status": "present",
            "devices": register_devices,
            "touched": sorted(d["device_id"] for d in register_devices if d["device_id"] in touched),
        }
    else:
        enriched["register_bank_theory"] = _theory_none_block()
    if fifo_devices:
        enriched["fifo_theory"] = {
            "status": "present",
            "devices": fifo_devices,
            "touched": sorted(d["device_id"] for d in fifo_devices if d["device_id"] in touched),
        }
    else:
        enriched["fifo_theory"] = _theory_none_block()

    status = "cfg-routed" if not rejection_reasons else "cfg-routed-rejected"
    if register_devices:
        enriched["register_bank_extensional"] = build_register_bank_extensional_obligation(
            bus_spec_sha256=spec.sha256(),
            devices=register_devices,
            unsupported_access=unsupported_access,
            observability=enriched["observability"],
            status=status,
        )
    if fifo_devices:
        gxfifo = build_gxfifo_trace_obligation(
            bus_spec_sha256=spec.sha256(),
            devices=fifo_devices,
            status=status,
        )
        gxfifo["loop_fifo_policy"] = LOOP_FIFO_POLICY
        gxfifo["bounded_summarized_emission"] = LOOP_FIFO_EMISSION
        enriched["gxfifo_trace"] = gxfifo

    # Engine-proven discharge: both sides discharged form, no CFG rejections,
    # and per-side access coverage status=complete (schema v2).
    # Loop×FIFO / mixed-space rejects also block discharged (Track C).
    original_ok = _side_is_discharged_form(unsupported_access.get("original") or {})
    candidate_ok = _side_is_discharged_form(unsupported_access.get("candidate") or {})
    coverage_complete = (
        access_coverage.get("status") == "complete"
        and (access_coverage.get("original") or {}).get("status") == "complete"
        and (access_coverage.get("candidate") or {}).get("status") == "complete"
    )
    hard_reject = bool(rejection_reasons) or bool(loop_markers) or bool(
        enriched.get("loop_fifo_rejection") or enriched.get("mixed_space_rejection")
    )
    if original_ok and candidate_ok and coverage_complete and not hard_reject:
        enriched["status"] = "discharged"
    elif hard_reject:
        enriched["status"] = "cfg-routed-rejected"
    else:
        enriched["status"] = "pending"

    return enriched


def _is_sha256(value: Any) -> bool:
    if not isinstance(value, str) or len(value) != _SHA256_LEN:
        return False
    try:
        int(value, 16)
    except ValueError:
        return False
    return True


# Discharged access-coverage must be ``complete`` (static + dynamic).
# ``observed`` remains valid only for pending/diagnostic non-discharged states.
# Transitional shape: top-level ``status`` / ``opcode_families`` / ``sha256``.
# Per-side (Track B final): ``sides.{original,candidate}`` each with the same
# fields (optional top-level aggregate ``sha256``).
_COVERAGE_DISCHARGED_STATUSES = frozenset({"complete"})
_COVERAGE_ATTESTED_STATUSES = frozenset({"observed", "complete"})
_COVERAGE_REJECT_STATUSES = frozenset({"stubbed", "missing", "absent", ""})


def _coverage_identity_payload(
    *,
    status: str,
    attested: bool,
    opcode_families: Sequence[str],
    rejections: Sequence[str],
    schema_version: int = 1,
) -> dict[str, Any]:
    return {
        "schema_version": schema_version,
        "status": status,
        "attested": attested,
        "opcode_families": list(opcode_families),
        "rejections": list(rejections),
    }


def _recompute_access_coverage_sha256(block: dict[str, Any]) -> str:
    return canonical_json_sha256(
        _coverage_identity_payload(
            status=str(block.get("status") or ""),
            attested=bool(block.get("attested")),
            opcode_families=list(block.get("opcode_families") or []),
            rejections=list(block.get("rejections") or []),
            schema_version=int(block.get("schema_version") or 1),
        )
    )


def _mmio_device_ids_from_regions(regions: Sequence[Any]) -> set[str]:
    ids: set[str] = set()
    for region in regions:
        if not isinstance(region, dict):
            continue
        if region.get("kind") != "mmio":
            continue
        device_id = region.get("device_id")
        if isinstance(device_id, str) and device_id:
            ids.add(device_id)
    return ids


def _validate_mmio_regions_device_ids(
    regions: Sequence[Any],
) -> str | None:
    """Reject MMIO regions missing device_id or with duplicate device_ids."""
    seen: set[str] = set()
    for index, region in enumerate(regions):
        if not isinstance(region, dict):
            continue
        if region.get("kind") != "mmio":
            continue
        device_id = region.get("device_id")
        if not isinstance(device_id, str) or not device_id:
            return (
                f"memory_bus.regions[{index}] MMIO requires nonempty device_id "
                "for discharged obligations"
            )
        if device_id in seen:
            return (
                f"memory_bus.regions duplicate MMIO device_id {device_id!r}"
            )
        seen.add(device_id)
    return None


def _theory_device_entries(
    block: dict[str, Any] | None,
    *,
    label: str,
    expected_theory: str | None = None,
) -> tuple[list[dict[str, Any]], str | None]:
    """Return theory device entries or a validation error.

    Detects duplicate device_ids and optional theory/type mismatches against
    the containing block (register-bank vs gxfifo-stream).
    """
    if not isinstance(block, dict):
        return [], None
    devices = block.get("devices")
    if not isinstance(devices, list):
        return [], None
    entries: list[dict[str, Any]] = []
    seen: set[str] = set()
    for index, entry in enumerate(devices):
        if not isinstance(entry, dict):
            return [], f"{label}.devices[{index}] must be an object"
        device_id = entry.get("device_id")
        if not isinstance(device_id, str) or not device_id:
            return [], (
                f"{label}.devices[{index}].device_id must be a nonempty string"
            )
        if device_id in seen:
            return [], f"{label}.devices duplicate device_id {device_id!r}"
        seen.add(device_id)
        theory = entry.get("theory")
        if expected_theory is not None:
            if theory != expected_theory:
                return [], (
                    f"{label}.devices[{index}].theory must be "
                    f"{expected_theory!r} (got {theory!r})"
                )
        entries.append(entry)
    return entries, None


def _theory_device_ids(block: dict[str, Any] | None) -> set[str]:
    if not isinstance(block, dict):
        return set()
    devices = block.get("devices")
    if not isinstance(devices, list):
        return set()
    ids: set[str] = set()
    for entry in devices:
        if isinstance(entry, dict):
            device_id = entry.get("device_id")
            if isinstance(device_id, str) and device_id:
                ids.add(device_id)
    return ids


def _observability_implies_device_compare(observability: Any) -> bool:
    if not isinstance(observability, dict):
        return False
    banks = observability.get("register_banks")
    if isinstance(banks, dict) and banks:
        return True
    fifo = observability.get("fifo_traces")
    if isinstance(fifo, dict) and fifo:
        return True
    touches = observability.get("touches")
    if isinstance(touches, dict):
        for side in ("original", "candidate"):
            side_touches = touches.get(side)
            if isinstance(side_touches, list) and side_touches:
                return True
    symbolic = observability.get("symbolic")
    if isinstance(symbolic, dict):
        for side in ("original", "candidate"):
            side_sym = symbolic.get(side)
            if not isinstance(side_sym, dict):
                continue
            if side_sym.get("register_banks") or side_sym.get("fifo_traces"):
                return True
    return False


def _validate_solver_metadata(solver: Any, *, label: str) -> str | None:
    if not isinstance(solver, dict):
        return f"{label} must be an object"
    if solver.get("name") != "z3":
        return f"{label}.name must be 'z3'"
    if not isinstance(solver.get("version"), str) or not solver["version"]:
        return f"{label}.version must be a non-empty string"
    if not isinstance(solver.get("elapsed_ms"), (int, float)):
        return f"{label}.elapsed_ms must be a number"
    return None


def _coverage_digest_for_side(
    access_coverage: dict[str, Any],
    *,
    side_name: str,
) -> str | None:
    """Resolve the coverage digest bound into a vacuous unsupported-access side.

    Prefer Finding 2 top-level ``access_coverage.<side>.sha256``, then Track C
    ``access_coverage.sides.<side>.sha256``, then transitional parent digest.
    """
    return access_coverage_side_digest(access_coverage, side_name)


def _iter_access_coverage_sides(
    access_coverage: dict[str, Any],
) -> dict[str, dict[str, Any]] | None:
    """Return per-side coverage blocks when present (top-level or ``sides``)."""
    if all(isinstance(access_coverage.get(name), dict) for name in ("original", "candidate")):
        return {
            "original": access_coverage["original"],
            "candidate": access_coverage["candidate"],
        }
    sides = access_coverage.get("sides")
    if isinstance(sides, dict) and all(
        isinstance(sides.get(name), dict) for name in ("original", "candidate")
    ):
        return {
            "original": sides["original"],
            "candidate": sides["candidate"],
        }
    return None


def _validate_access_coverage_attestation(
    access_coverage: Any,
) -> str | None:
    """Strict coverage checks for ``status=discharged`` (recompute digests)."""
    if not isinstance(access_coverage, dict):
        return "memory_bus.discharged requires access_coverage object"
    if access_coverage.get("attested") is not True:
        return "memory_bus.discharged requires access_coverage.attested=true"

    sides = _iter_access_coverage_sides(access_coverage)
    if sides is not None:
        for side_name in ("original", "candidate"):
            side = sides[side_name]
            status = side.get("status")
            label = f"memory_bus.access_coverage.{side_name}"
            if status in _COVERAGE_REJECT_STATUSES or status is None:
                return (
                    f"{label}.status must be complete "
                    "(not stubbed/missing/observed)"
                )
            if status not in _COVERAGE_DISCHARGED_STATUSES:
                return (
                    f"{label}.status must be complete "
                    f"(got {status!r}; observed is not sufficient for discharge)"
                )
            if side.get("attested") is False:
                return f"{label}.attested must not be false"
            expected = _recompute_access_coverage_sha256(
                {
                    **side,
                    "attested": side.get("attested", True),
                }
            )
            if side.get("sha256") != expected:
                return f"{label}.sha256 does not match recomputed digest"
        if "sha256" in access_coverage:
            if not _is_sha256(access_coverage.get("sha256")):
                return "memory_bus.access_coverage.sha256 must be 64-hex"
            expected_agg = canonical_json_sha256(
                {
                    "kind": "access-coverage-aggregate",
                    "schema_version": 1,
                    "original": sides["original"]["sha256"],
                    "candidate": sides["candidate"]["sha256"],
                }
            )
            if access_coverage["sha256"] != expected_agg:
                return (
                    "memory_bus.access_coverage.sha256 does not match "
                    "per-side aggregate digest"
                )
        parent_status = access_coverage.get("status")
        if parent_status is not None and parent_status not in _COVERAGE_DISCHARGED_STATUSES:
            return (
                "memory_bus.access_coverage.status must be complete "
                f"for discharged (got {parent_status!r})"
            )
        return None

    # Transitional top-level shape (flat opcode_families).
    status = access_coverage.get("status")
    if status in _COVERAGE_REJECT_STATUSES or status is None:
        return (
            "memory_bus.discharged requires access_coverage.status "
            "complete (not stubbed/missing/observed)"
        )
    if status not in _COVERAGE_DISCHARGED_STATUSES:
        return (
            "memory_bus.discharged requires access_coverage.status "
            f"complete (got {status!r}; observed is not sufficient for discharge)"
        )
    if not _is_sha256(access_coverage.get("sha256")):
        return "memory_bus.discharged requires access_coverage.sha256"
    expected = _recompute_access_coverage_sha256(access_coverage)
    if access_coverage.get("sha256") != expected:
        return (
            "memory_bus.access_coverage.sha256 does not match "
            "recomputation from attested fields"
        )
    return None


def _validate_unsupported_access_side(
    side: Any,
    *,
    label: str,
    side_name: str,
    access_coverage: dict[str, Any] | None,
    terminals: Sequence[Any] | None = None,
) -> str | None:
    if not isinstance(side, dict):
        return f"{label} must be an object"

    if side.get("status") == "vacuously-discharged":
        if side.get("reason") != "no-unsupported-predicates":
            return (
                f"{label}.reason must be 'no-unsupported-predicates' "
                "for vacuous discharge"
            )
        if not _is_sha256(side.get("cfg_trace_sha256")):
            return f"{label}.cfg_trace_sha256 must be a 64-hex digest"
        coverage_digest = side.get("access_coverage_sha256")
        if not _is_sha256(coverage_digest):
            return f"{label}.access_coverage_sha256 must be a 64-hex digest"
        if access_coverage is None:
            return f"{label} vacuous discharge requires parent access_coverage"
        if access_coverage.get("attested") is not True:
            return (
                f"{label} vacuous discharge requires "
                "access_coverage.attested=true"
            )
        expected = _coverage_digest_for_side(
            access_coverage, side_name=side_name,
        )
        if expected != coverage_digest:
            return (
                f"{label}.access_coverage_sha256 must match "
                "access_coverage digest for this side"
            )
        if terminals is not None:
            from tools.ppc_equivalence.symbolic_bus import cfg_trace_sha256

            recomputed = cfg_trace_sha256(terminals)
            if side.get("cfg_trace_sha256") != recomputed:
                return f"{label}.cfg_trace_sha256 does not match recomputed CFG trace"
        return None

    if side.get("result") == "unsat":
        if not _is_sha256(side.get("query_sha256")):
            return f"{label}.query_sha256 must be a 64-hex digest"
        reason = _validate_solver_metadata(
            side.get("solver"), label=f"{label}.solver",
        )
        if reason is not None:
            return reason
        terminals_block = side.get("terminals")
        if not isinstance(terminals_block, list) or not terminals_block:
            return (
                f"{label}.terminals must be a nonempty list for "
                "non-vacuous result=unsat discharge"
            )
        from tools.ppc_equivalence.symbolic_bus import (
            aggregate_unsupported_access_query_sha256,
        )

        for index, entry in enumerate(terminals_block):
            if not isinstance(entry, dict):
                return f"{label}.terminals[{index}] must be an object"
            if entry.get("result") != "unsat":
                return (
                    f"{label}.terminals[{index}].result must be 'unsat' "
                    f"(got {entry.get('result')!r}); SAT/unknown children "
                    "cannot authorize a parent unsat aggregate"
                )
            # Prefer explicit false: absent inconclusive is rejected.
            if entry.get("inconclusive") is not False:
                return (
                    f"{label}.terminals[{index}].inconclusive must be "
                    "explicitly false for result=unsat"
                )
            if not _is_sha256(entry.get("query_sha256")):
                return (
                    f"{label}.terminals[{index}].query_sha256 "
                    "must be a 64-hex digest"
                )
            child_solver = _validate_solver_metadata(
                entry.get("solver"),
                label=f"{label}.terminals[{index}].solver",
            )
            if child_solver is not None:
                return child_solver
        expected = aggregate_unsupported_access_query_sha256(terminals_block)
        if side.get("query_sha256") != expected:
            return (
                f"{label}.query_sha256 must be the aggregate digest of "
                "every complete terminal record"
            )
        return None

    return (
        f"{label} must be result=unsat with digests or "
        "status=vacuously-discharged"
    )


def _validate_theory_block(
    block: Any,
    *,
    label: str,
    expected_theory: str | None = None,
) -> str | None:
    if not isinstance(block, dict):
        return f"{label} must be an object"
    status = block.get("status")
    if status == "none":
        devices = block.get("devices")
        if devices is not None and devices != []:
            return f"{label}.devices must be empty when status=none"
        return None
    if status != "present":
        return f"{label}.status must be 'none' or 'present'"
    devices = block.get("devices")
    if not isinstance(devices, list) or not devices:
        return f"{label}.devices must be a nonempty list when status=present"
    _, err = _theory_device_entries(
        block, label=label, expected_theory=expected_theory,
    )
    return err


def _validate_bus_spec_canonical(obligation: dict[str, Any]) -> str | None:
    """When ``bus_spec_canonical`` is present, recompute ``bus_spec_sha256``."""
    canonical = obligation.get("bus_spec_canonical")
    if canonical is None:
        return None
    if not isinstance(canonical, dict):
        return "memory_bus.bus_spec_canonical must be an object when present"
    recomputed = canonical_json_sha256(canonical)
    if obligation.get("bus_spec_sha256") != recomputed:
        return (
            "memory_bus.bus_spec_sha256 does not match recomputation from "
            "bus_spec_canonical"
        )
    return None


def _validate_register_bank_extensional(
    block: Any,
    *,
    bus_spec_sha256: str,
    theory_ids: set[str],
) -> str | None:
    if not isinstance(block, dict):
        return "memory_bus.register_bank_extensional must be an object"
    from tools.ppc_equivalence.symbolic_bus import ALGORITHM as RB_ALGORITHM

    if block.get("algorithm") != RB_ALGORITHM:
        return (
            "memory_bus.register_bank_extensional.algorithm must be "
            f"{RB_ALGORITHM}"
        )
    if block.get("bus_spec_sha256") != bus_spec_sha256:
        return (
            "memory_bus.register_bank_extensional.bus_spec_sha256 must match "
            "memory_bus.bus_spec_sha256"
        )
    devices = block.get("devices")
    if not isinstance(devices, list) or not devices:
        return "memory_bus.register_bank_extensional.devices must be nonempty"
    block_ids = _theory_device_ids({"devices": devices})
    if not block_ids:
        return "memory_bus.register_bank_extensional.devices need device_id"
    if not block_ids.issubset(theory_ids):
        return (
            "memory_bus.register_bank_extensional.devices must be subset of "
            "register_bank_theory.devices"
        )
    return None


def _validate_gxfifo_trace(
    block: Any,
    *,
    bus_spec_sha256: str,
    theory_ids: set[str],
) -> str | None:
    if not isinstance(block, dict):
        return "memory_bus.gxfifo_trace must be an object"
    from tools.ppc_equivalence.symbolic_event_trace import ALGORITHM as FIFO_ALGORITHM

    if block.get("algorithm") != FIFO_ALGORITHM:
        return f"memory_bus.gxfifo_trace.algorithm must be {FIFO_ALGORITHM}"
    if block.get("bus_spec_sha256") != bus_spec_sha256:
        return (
            "memory_bus.gxfifo_trace.bus_spec_sha256 must match "
            "memory_bus.bus_spec_sha256"
        )
    if block.get("reads") != "unsupported":
        return "memory_bus.gxfifo_trace.reads must be 'unsupported'"
    devices = block.get("devices")
    if not isinstance(devices, list) or not devices:
        return "memory_bus.gxfifo_trace.devices must be nonempty"
    block_ids = _theory_device_ids({"devices": devices})
    if not block_ids.issubset(theory_ids):
        return (
            "memory_bus.gxfifo_trace.devices must be subset of "
            "fifo_theory.devices"
        )
    return None


def _validate_theory_region_binding(
    obligation: dict[str, Any],
    *,
    bus: MemoryBus | None,
) -> str | None:
    regions = obligation.get("regions")
    assert isinstance(regions, list)

    mmio_err = _validate_mmio_regions_device_ids(regions)
    if mmio_err is not None:
        return mmio_err

    canonical_err = _validate_bus_spec_canonical(obligation)
    if canonical_err is not None:
        return canonical_err

    mmio_ids = _mmio_device_ids_from_regions(regions)
    register_theory = obligation.get("register_bank_theory")
    fifo_theory = obligation.get("fifo_theory")

    register_entries, register_err = _theory_device_entries(
        register_theory if isinstance(register_theory, dict) else None,
        label="memory_bus.register_bank_theory",
        expected_theory="register-bank",
    )
    if register_err is not None:
        return register_err
    fifo_entries, fifo_err = _theory_device_entries(
        fifo_theory if isinstance(fifo_theory, dict) else None,
        label="memory_bus.fifo_theory",
        expected_theory="gxfifo-stream",
    )
    if fifo_err is not None:
        return fifo_err

    register_ids = {
        entry["device_id"]
        for entry in register_entries
        if isinstance(entry.get("device_id"), str)
    }
    fifo_ids = {
        entry["device_id"]
        for entry in fifo_entries
        if isinstance(entry.get("device_id"), str)
    }

    overlap = register_ids & fifo_ids
    if overlap:
        return (
            "memory_bus.register_bank_theory and fifo_theory must not share "
            f"device_ids ({', '.join(sorted(overlap))})"
        )

    if bus is not None and bus.specification is not None:
        spec = bus.specification
        expected_spec = spec.sha256()
        if obligation.get("bus_spec_sha256") != expected_spec:
            return (
                "memory_bus.bus_spec_sha256 does not match live MemoryBus "
                "specification digest"
            )
        spec_register = {
            device.device_id
            for device in spec.devices
            if device.theory == "register-bank"
        }
        spec_fifo = {
            device.device_id
            for device in spec.devices
            if device.theory == "gxfifo-stream"
        }
        if spec_register:
            if not isinstance(register_theory, dict) or register_theory.get("status") != "present":
                return (
                    "memory_bus.register_bank_theory.status must be 'present' "
                    "when bus has register-bank devices"
                )
            if register_ids != spec_register:
                return (
                    "memory_bus.register_bank_theory.devices must match bus "
                    "register-bank device_ids"
                )
        elif isinstance(register_theory, dict) and register_theory.get("status") == "present":
            return (
                "memory_bus.register_bank_theory.status=present but bus has "
                "no register-bank devices"
            )
        if spec_fifo:
            if not isinstance(fifo_theory, dict) or fifo_theory.get("status") != "present":
                return (
                    "memory_bus.fifo_theory.status must be 'present' "
                    "when bus has gxfifo-stream devices"
                )
            if fifo_ids != spec_fifo:
                return (
                    "memory_bus.fifo_theory.devices must match bus "
                    "gxfifo-stream device_ids"
                )
        elif isinstance(fifo_theory, dict) and fifo_theory.get("status") == "present":
            return (
                "memory_bus.fifo_theory.status=present but bus has "
                "no gxfifo-stream devices"
            )
        # Live bus still requires exact MMIO region coverage from theories.
        if mmio_ids and (register_ids | fifo_ids) != mmio_ids:
            return (
                "memory_bus theory device_ids must exactly cover every MMIO "
                "region device_id"
            )
    else:
        # JSON-only: exact coverage of every MMIO region device (not subset).
        if mmio_ids:
            if (
                isinstance(register_theory, dict)
                and register_theory.get("status") == "none"
                and isinstance(fifo_theory, dict)
                and fifo_theory.get("status") == "none"
            ):
                return (
                    "memory_bus.register_bank_theory/fifo_theory cannot both "
                    "be status=none when regions include MMIO devices"
                )
            declared = register_ids | fifo_ids
            if declared != mmio_ids:
                missing = sorted(mmio_ids - declared)
                extra = sorted(declared - mmio_ids)
                detail_parts: list[str] = []
                if missing:
                    detail_parts.append(f"missing={missing}")
                if extra:
                    detail_parts.append(f"extra={extra}")
                detail = "; ".join(detail_parts) if detail_parts else "mismatch"
                return (
                    "memory_bus theory device_ids must exactly equal region "
                    f"MMIO device_ids ({detail})"
                )

    return None


def _validate_device_state_in_compare(
    obligation: dict[str, Any],
) -> str | None:
    compare = obligation.get("device_state_in_compare")
    if not isinstance(compare, dict):
        return "memory_bus.discharged requires device_state_in_compare object"
    included = compare.get("included")
    if not isinstance(included, bool):
        return "memory_bus.device_state_in_compare.included must be a bool"
    digest = compare.get("digest_sha256")
    if digest is not None and not _is_sha256(digest):
        return "memory_bus.device_state_in_compare.digest_sha256 must be 64-hex or null"

    register_theory = obligation.get("register_bank_theory")
    fifo_theory = obligation.get("fifo_theory")
    regions = obligation.get("regions")
    mmio_ids = _mmio_device_ids_from_regions(
        regions if isinstance(regions, list) else []
    )
    needs_compare = bool(mmio_ids) or bool(
        isinstance(register_theory, dict) and register_theory.get("status") == "present"
    ) or bool(
        isinstance(fifo_theory, dict) and fifo_theory.get("status") == "present"
    ) or obligation.get("register_bank_extensional") is not None or (
        obligation.get("gxfifo_trace") is not None
    ) or _observability_implies_device_compare(obligation.get("observability"))

    if needs_compare and included is not True:
        return (
            "memory_bus.device_state_in_compare.included must be true when "
            "MMIO / register-bank / FIFO theory is present"
        )

    observability = obligation.get("observability")
    if included is True and isinstance(observability, dict):
        expected = canonical_json_sha256(
            {
                "kind": "device-state-in-compare",
                "observability": observability,
            }
        )
        if digest != expected:
            return (
                "memory_bus.device_state_in_compare.digest_sha256 does not "
                "match recomputation from observability"
            )
    return None


def validate_memory_bus_obligation(
    obligation: dict[str, Any],
    *,
    bus: MemoryBus | None = None,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
) -> str | None:
    """Return None when a ``memory_bus`` obligation is well-formed.

    Structural checks always apply. ``status=discharged`` fails closed unless
    schema v2 attestations and per-side unsupported-access digests are present
    **and** digests recompute from obligation fields. Pass ``bus`` / terminals
    when available for live CFG-trace and bus-spec cross-checks; JSON-only
    callers still reject inconsistent shapes without the live bus.
    """
    if obligation.get("algorithm") != MEMORY_BUS_ALGORITHM:
        return "memory_bus.algorithm must be memory-bus-v1"
    regions = obligation.get("regions")
    if not isinstance(regions, list) or not regions:
        return "memory_bus.regions must be a nonempty list"
    for index, region in enumerate(regions):
        if not isinstance(region, dict):
            return f"memory_bus.regions[{index}] must be an object"
        kind = region.get("kind")
        if not isinstance(kind, str) or not kind:
            return f"memory_bus.regions[{index}].kind must be a nonempty string"
        for field in ("start", "end"):
            value = region.get(field)
            if not isinstance(value, int) or value < 0 or value > 0xFFFFFFFF:
                return f"memory_bus.regions[{index}].{field} must be a u32"

    status = obligation.get("status")
    if status is None or status in ("pending", "cfg-routed", "cfg-routed-rejected", "failed"):
        return None

    if status != "discharged":
        return (
            "memory_bus.status must be pending|cfg-routed|cfg-routed-rejected|"
            "failed|discharged"
        )

    schema = obligation.get("schema_version")
    if schema != MEMORY_BUS_OBLIGATION_SCHEMA_VERSION:
        return (
            "memory_bus.status=discharged requires "
            f"schema_version={MEMORY_BUS_OBLIGATION_SCHEMA_VERSION}"
        )
    if not _is_sha256(obligation.get("bus_spec_sha256")):
        return "memory_bus.discharged requires bus_spec_sha256 (64-hex)"

    unsupported = obligation.get("unsupported_access")
    if not isinstance(unsupported, dict):
        return "memory_bus.discharged requires unsupported_access object"
    access_coverage = obligation.get("access_coverage")
    coverage_reason = _validate_access_coverage_attestation(access_coverage)
    if coverage_reason is not None:
        return coverage_reason
    assert isinstance(access_coverage, dict)

    terminals_by_side = {
        "original": original_terminals,
        "candidate": candidate_terminals,
    }
    for side_name in ("original", "candidate"):
        reason = _validate_unsupported_access_side(
            unsupported.get(side_name),
            label=f"memory_bus.unsupported_access.{side_name}",
            side_name=side_name,
            access_coverage=access_coverage,
            terminals=terminals_by_side[side_name],
        )
        if reason is not None:
            return reason

    for key, expected_theory in (
        ("register_bank_theory", "register-bank"),
        ("fifo_theory", "gxfifo-stream"),
    ):
        reason = _validate_theory_block(
            obligation.get(key),
            label=f"memory_bus.{key}",
            expected_theory=expected_theory,
        )
        if reason is not None:
            return reason

    binding_reason = _validate_theory_region_binding(obligation, bus=bus)
    if binding_reason is not None:
        return binding_reason

    compare_reason = _validate_device_state_in_compare(obligation)
    if compare_reason is not None:
        return compare_reason

    bus_spec = obligation.get("bus_spec_sha256")
    assert isinstance(bus_spec, str)
    register_theory = obligation.get("register_bank_theory")
    fifo_theory = obligation.get("fifo_theory")
    register_ids = _theory_device_ids(
        register_theory if isinstance(register_theory, dict) else None
    )
    fifo_ids = _theory_device_ids(
        fifo_theory if isinstance(fifo_theory, dict) else None
    )

    if "register_bank_extensional" in obligation:
        reason = _validate_register_bank_extensional(
            obligation.get("register_bank_extensional"),
            bus_spec_sha256=bus_spec,
            theory_ids=register_ids,
        )
        if reason is not None:
            return reason
        if not register_ids:
            return (
                "memory_bus.register_bank_extensional present requires "
                "register_bank_theory.status=present"
            )

    if "gxfifo_trace" in obligation:
        reason = _validate_gxfifo_trace(
            obligation.get("gxfifo_trace"),
            bus_spec_sha256=bus_spec,
            theory_ids=fifo_ids,
        )
        if reason is not None:
            return reason
        if not fifo_ids:
            return (
                "memory_bus.gxfifo_trace present requires "
                "fifo_theory.status=present"
            )

    cfg_reasons = obligation.get("cfg_rejection_reasons")
    if not isinstance(cfg_reasons, list):
        return "memory_bus.discharged requires cfg_rejection_reasons list"
    if cfg_reasons:
        joined = ",".join(str(item) for item in cfg_reasons)
        if MIXED_SPACE_REJECTION_REASON in cfg_reasons:
            return (
                "memory_bus.discharged incompatible with mixed-space CFG "
                f"rejection ({MIXED_SPACE_REJECTION_REASON})"
            )
        if LOOP_FIFO_REJECTION_REASON in cfg_reasons:
            return (
                "memory_bus.discharged incompatible with loop×FIFO "
                f"hard-reject ({LOOP_FIFO_REJECTION_REASON})"
            )
        return f"memory_bus.discharged requires empty cfg_rejection_reasons ({joined})"

    loop_markers = obligation.get("loop_fifo_reject_markers")
    if not isinstance(loop_markers, list):
        return "memory_bus.discharged requires loop_fifo_reject_markers list"
    if loop_markers:
        return (
            "memory_bus.discharged incompatible with loop×FIFO reject markers "
            f"({','.join(str(item) for item in loop_markers)})"
        )

    # Track C policy fields: when present, must not contradict discharge.
    if obligation.get("loop_fifo_rejection") is not None:
        return "memory_bus.discharged incompatible with loop×FIFO rejection attestation"
    if obligation.get("mixed_space_rejection") is not None:
        return "memory_bus.discharged incompatible with mixed-space rejection attestation"
    coverage = obligation.get("coverage")
    if isinstance(coverage, dict):
        if coverage.get("mixed_space") == "rejected":
            return "memory_bus.discharged incompatible with mixed-space coverage=rejected"
        if coverage.get("loop_fifo") == "hard-rejected":
            return "memory_bus.discharged incompatible with loop×FIFO coverage=hard-rejected"

    return None


def validate_memory_bus_obligation_strict(
    obligation: dict[str, Any],
    *,
    bus: MemoryBus | None = None,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
) -> str | None:
    """Alias for :func:`validate_memory_bus_obligation` (digest recompute path).

    Prefer this name at ``require_equivalent_ready`` / engine call sites that
    intentionally opt into strict recomputation + optional live ``bus``.
    """
    return validate_memory_bus_obligation(
        obligation,
        bus=bus,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
    )

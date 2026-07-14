from __future__ import annotations

from dataclasses import dataclass

from tools.coop.lib.object_size import format_size_check
from tools.test.compare_behaviour.lib.manifest import BehaviourTest, load_manifest
from tools.test.compare_behaviour.lib.static_compare import run_static_compare


@dataclass
class AuditRow:
    test_id: str
    unit: str
    symbol: str
    match_percent: float | None
    status: str
    ppc_source: str | None
    ok: bool
    notes: str


def audit_all() -> list[AuditRow]:
    _, tests = load_manifest()
    return [audit_one(test) for test in tests]


def audit_one(test: BehaviourTest) -> AuditRow:
    result = run_static_compare(test)
    notes: list[str] = []

    size_ok = result.size_check is None or result.size_check.ok
    if result.size_check is not None and not result.size_check.ok:
        notes.append(format_size_check(result.size_check))

    if result.match_percent is not None and result.match_percent >= 100.0:
        if not notes:
            notes.append("FULL_MATCH")
    elif test.ppc_source:
        notes.append(f"ppc={test.ppc_source}")
    else:
        notes.append("below FULL_MATCH; no PPC harness")

    return AuditRow(
        test_id=test.id,
        unit=test.unit,
        symbol=test.symbol,
        match_percent=result.match_percent,
        status=result.status,
        ppc_source=test.ppc_source,
        ok=size_ok,
        notes="; ".join(notes) if notes else "ok",
    )

from __future__ import annotations

from dataclasses import dataclass

from tools.test.compare_behaviour.lib.manifest import BehaviourTest, load_manifest
from tools.test.compare_behaviour.lib.paths import HOST_DIR
from tools.test.compare_behaviour.lib.policy import count_host_scenarios, min_scenarios_for_match
from tools.test.compare_behaviour.lib.static_compare import run_static_compare
from tools.coop.lib.object_size import format_size_check
from tools.coop.lib.object_size import format_size_check


@dataclass
class AuditRow:
    test_id: str
    unit: str
    symbol: str
    match_percent: float | None
    status: str
    host_binary: str | None
    scenario_count: int
    scenario_required: int
    ok: bool
    notes: str


def audit_all() -> list[AuditRow]:
    _, tests = load_manifest()
    rows: list[AuditRow] = []
    for test in tests:
        rows.append(audit_one(test))
    return rows


def audit_one(test: BehaviourTest) -> AuditRow:
    result = run_static_compare(test)
    required = min_scenarios_for_match(result.match_percent)
    host_binary = test.host_binary
    scenario_count = 0
    notes: list[str] = []

    if result.size_check is not None and not result.size_check.ok:
        notes.append(format_size_check(result.size_check))

    if result.match_percent is not None and result.match_percent >= 100.0:
        size_ok = result.size_check is None or result.size_check.ok
        if host_binary:
            notes.append("host optional at FULL_MATCH")
        return AuditRow(
            test_id=test.id,
            unit=test.unit,
            symbol=test.symbol,
            match_percent=result.match_percent,
            status=result.status,
            host_binary=host_binary,
            scenario_count=0,
            scenario_required=0,
            ok=size_ok,
            notes="; ".join(notes) if notes else "FULL_MATCH",
        )

    if not host_binary:
        notes.append("MISSING host_binary — required below FULL_MATCH")
        return AuditRow(
            test_id=test.id,
            unit=test.unit,
            symbol=test.symbol,
            match_percent=result.match_percent,
            status=result.status,
            host_binary=None,
            scenario_count=0,
            scenario_required=required,
            ok=False,
            notes="; ".join(notes),
        )

    host_source = HOST_DIR / f"{host_binary}.cpp"
    scenario_count = count_host_scenarios(host_source)
    if not host_source.is_file():
        notes.append(f"missing host source {host_source.name}")
    elif scenario_count < required:
        notes.append(f"need {required} scenarios, have {scenario_count}")

    ok = host_source.is_file() and scenario_count >= required
    if result.size_check is not None and not result.size_check.ok:
        ok = False
        if format_size_check(result.size_check) not in notes:
            notes.append(format_size_check(result.size_check))
    return AuditRow(
        test_id=test.id,
        unit=test.unit,
        symbol=test.symbol,
        match_percent=result.match_percent,
        status=result.status,
        host_binary=host_binary,
        scenario_count=scenario_count,
        scenario_required=required,
        ok=ok,
        notes="; ".join(notes) if notes else "ok",
    )

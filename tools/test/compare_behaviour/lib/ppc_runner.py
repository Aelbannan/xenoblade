from __future__ import annotations

import json
import os
import re
import shutil
import struct
import subprocess
import time
from dataclasses import dataclass
from pathlib import Path

from tools.test.compare_behaviour.lib.manifest import BehaviourTest
from tools.test.compare_behaviour.lib.paths import repo_root
from tools.test.compare_behaviour.lib.gdb_stub import gdb_read_memory_at_exit
from tools.test.compare_behaviour.lib.map_lookup import lookup_symbol_address
from tools.test.compare_behaviour.lib.ppc_builder import BEHAVIOUR_RESULT_MAGIC, build_ppc_test

BEHAVIOUR_LOG_ADDR = 0x80003800
BEHAVIOUR_LOG_SIZE = 2048
RESULT_STRUCT = struct.Struct(">IIII64s")
SUMMARY_RE = re.compile(
    r"BEHAVIOUR_SUMMARY\s+test=(\S+)\s+passed=(\d+)\s+failed=(\d+)\s+exit=(\d+)"
)
FAIL_RE = re.compile(r"BEHAVIOUR_FAIL\s+(\S+)")
DIFF_RE = re.compile(r"BEHAVIOUR_DIFF\s+(\S+)\s+(.+)")


@dataclass
class PpcRunResult:
    dol: Path
    ok: bool
    skipped: bool
    passed: int | None
    failed: int | None
    failed_scenarios: list[str]
    failure_details: list[str]
    last_fail: str | None
    output: str
    dolphin: str | None


def _coop_config_path() -> Path:
    return repo_root() / "coop.json"


def find_dolphin() -> str | None:
    env = os.environ.get("DOLPHIN")
    if env and Path(env).exists():
        return env

    coop = _coop_config_path()
    if coop.is_file():
        try:
            data = json.loads(coop.read_text(encoding="utf-8"))
            configured = data.get("dolphin")
            if configured and Path(configured).exists():
                return str(configured)
        except json.JSONDecodeError:
            pass

    for candidate in (
        "dolphin-emu",
        "Dolphin",
        "/Applications/Dolphin.app/Contents/MacOS/Dolphin",
    ):
        found = shutil.which(candidate)
        if found:
            return found
        if candidate.startswith("/") and Path(candidate).is_file():
            return candidate
    return None


def _write_dolphin_user_ini(
    user_dir: Path, *, gdb_port: int, log_path: Path, cpu_core: int | None = None
) -> None:
    config_dir = user_dir / "Config"
    config_dir.mkdir(parents=True, exist_ok=True)
    ini = config_dir / "Dolphin.ini"
    # Keep headless PPC runs non-interactive: no panic dialogs / analytics prompts.
    lines = [
        "[General]",
        f"GDBPort = {gdb_port}",
        "",
        "[Interface]",
        "UsePanicHandlers = False",
        "ConfirmStop = False",
        "",
        "[Analytics]",
        "Enabled = False",
        "PermissionAsked = True",
        "",
        "[Logger]",
        "LogToFile = True",
        f"LogFilePath = {log_path}",
        "",
    ]
    if cpu_core is not None:
        lines.extend(["[Core]", f"CPUCore = {cpu_core}", ""])
    ini.write_text("\n".join(lines), encoding="utf-8")


def _read_guest_mem(region: str, test_id: str, address: int, size: int) -> bytes | None:
    return None


def _result_address(build) -> int | None:
    if build.map_path is None:
        return None
    return lookup_symbol_address(build.map_path, "g_behaviour_result")


def _parse_failures(text: str) -> list[str]:
    return FAIL_RE.findall(text)


def _parse_failure_details(text: str) -> list[str]:
    return [f"{name}: {fields}" for name, fields in DIFF_RE.findall(text)]


def _parse_summary(text: str) -> tuple[int, int, int] | None:
    for line in text.splitlines():
        match = SUMMARY_RE.search(line)
        if match:
            passed = int(match.group(2))
            failed = int(match.group(3))
            exit_code = int(match.group(4))
            return passed, failed, exit_code
    return None


def run_ppc_compare(test: BehaviourTest, *, require_dolphin: bool = False) -> PpcRunResult:
    build = build_ppc_test(test)
    if not build.ok:
        return PpcRunResult(
            dol=build.dol,
            ok=False,
            skipped=False,
            passed=None,
            failed=None,
            failed_scenarios=[],
            failure_details=[],
            last_fail=None,
            output=build.output,
            dolphin=None,
        )

    dolphin = find_dolphin()
    if dolphin is None:
        msg = "Dolphin not found (set coop.json dolphin or DOLPHIN env)"
        if require_dolphin:
            return PpcRunResult(
                dol=build.dol,
                ok=False,
                skipped=False,
                passed=None,
                failed=None,
                failed_scenarios=[],
            failure_details=[],
                last_fail=None,
                output=msg,
                dolphin=None,
            )
        return PpcRunResult(
            dol=build.dol,
            ok=True,
            skipped=True,
            passed=None,
            failed=None,
            failed_scenarios=[],
            failure_details=[],
            last_fail=None,
            output=f"{msg}; PPC DOL built at {build.dol}",
            dolphin=None,
        )

    log_path = build.dol.parent / "dolphin.log"
    user_dir = build.dol.parent / "dolphin_user"
    user_dir.mkdir(parents=True, exist_ok=True)
    if log_path.is_file():
        log_path.unlink()

    gdb_port = 2160
    _write_dolphin_user_ini(
        user_dir, gdb_port=gdb_port, log_path=log_path, cpu_core=test.ppc_cpu_core
    )
    result_addr = _result_address(build)
    exit_addr = lookup_symbol_address(build.map_path, "exit") if build.map_path else None

    cmd = [
        dolphin,
        "--exec",
        str(build.dol),
        "--batch",
        f"--user={user_dir}",
    ]
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    mem_regions: dict[int, bytes] | None = None
    stdout = ""
    stderr = ""
    try:
        time.sleep(3.0)
        if result_addr is not None and exit_addr is not None:
            mem_regions = gdb_read_memory_at_exit(
                "127.0.0.1",
                gdb_port,
                exit_addr=exit_addr,
                reads=[
                    (result_addr, RESULT_STRUCT.size),
                    (BEHAVIOUR_LOG_ADDR, BEHAVIOUR_LOG_SIZE),
                ],
                timeout_s=15.0,
            )
    finally:
        proc.terminate()
        try:
            stdout, stderr = proc.communicate(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill()
            stdout, stderr = proc.communicate(timeout=1)
    output = "\n".join(part.strip() for part in (stdout, stderr) if part and part.strip())

    if log_path.is_file():
        output += "\n" + log_path.read_text(encoding="utf-8", errors="replace")

    log_text = ""
    if mem_regions is not None:
        log_blob = mem_regions.get(BEHAVIOUR_LOG_ADDR)
        if log_blob:
            log_text = log_blob.split(b"\x00", 1)[0].decode("utf-8", errors="replace")
            output += "\n" + log_text

    failed_scenarios = _parse_failures(output)
    failure_details = _parse_failure_details(output)

    summary = _parse_summary(output)
    if summary is not None:
        passed, failed, exit_code = summary
        if not failed_scenarios and failed:
            failed_scenarios = _parse_failures(log_text)
        if not failure_details:
            failure_details = _parse_failure_details(log_text)
        # A guest that never executes scenarios can still emit exit=0; treat that
        # as failure so a crashed/aborted Dolphin run cannot look green.
        ok = exit_code == 0 and failed == 0 and passed > 0
        return PpcRunResult(
            dol=build.dol,
            ok=ok,
            skipped=False,
            passed=passed,
            failed=failed,
            failed_scenarios=failed_scenarios,
            failure_details=failure_details,
            last_fail=failed_scenarios[-1] if failed_scenarios else None,
            output=output,
            dolphin=dolphin,
        )

    mem = mem_regions.get(result_addr) if mem_regions and result_addr is not None else None
    if mem is not None:
        magic, passed, failed, exit_code, last_fail_raw = RESULT_STRUCT.unpack(mem)
        if magic == BEHAVIOUR_RESULT_MAGIC:
            last_fail = last_fail_raw.split(b"\x00", 1)[0].decode("utf-8", errors="replace")
            if not failed_scenarios:
                failed_scenarios = [last_fail] if last_fail and failed else []
            if not failure_details:
                failure_details = _parse_failure_details(log_text)
            return PpcRunResult(
                dol=build.dol,
                ok=exit_code == 0 and failed == 0,
                skipped=False,
                passed=passed,
                failed=failed,
                failed_scenarios=failed_scenarios,
                failure_details=failure_details,
                last_fail=failed_scenarios[-1] if failed_scenarios else (last_fail or None),
                output=output,
                dolphin=dolphin,
            )

    ok = False
    if mem_regions is None:
        detail = "could not read g_behaviour_result via Dolphin GDB stub"
        if result_addr is None:
            detail = "g_behaviour_result missing from PPC map file"
        elif exit_addr is None:
            detail = "exit missing from PPC map file"
        output += f"\n(PPC: {detail})"
    return PpcRunResult(
        dol=build.dol,
        ok=ok,
        skipped=False,
        passed=None,
        failed=None,
        failed_scenarios=[],
        failure_details=[],
        last_fail=None,
        output=output + "\n(missing BEHAVIOUR_SUMMARY line in Dolphin log)",
        dolphin=dolphin,
    )

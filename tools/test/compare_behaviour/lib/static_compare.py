from __future__ import annotations

import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.object_size import ObjectSizeCheck, check_object_size, format_size_check
from tools.coop.lib.objdiff_report import classify_status, find_function_match, report_unit
from tools.coop.lib.project import Project
from tools.test.compare_behaviour.lib.manifest import BehaviourTest
from tools.test.compare_behaviour.lib.paths import decomp_object, retail_object, repo_root


@dataclass
class StaticCompareResult:
    retail_path: Path
    decomp_path: Path
    match_percent: float | None
    status: str
    ok: bool
    size_check: ObjectSizeCheck | None


def ensure_objects_built(test: BehaviourTest) -> None:
    retail = retail_object(test.region, test.unit)
    decomp = decomp_object(test.region, test.unit)
    if retail.is_file() and decomp.is_file():
        return

    subprocess.run(
        ["python3", "tools/coop/run.py", "build", test.unit],
        cwd=repo_root(),
        check=True,
    )


def run_static_compare(test: BehaviourTest) -> StaticCompareResult:
    ensure_objects_built(test)

    retail = retail_object(test.region, test.unit)
    decomp = decomp_object(test.region, test.unit)
    if not retail.is_file():
        raise FileNotFoundError(f"Retail object missing: {retail}")
    if not decomp.is_file():
        raise FileNotFoundError(f"Decomp object missing: {decomp}")

    root = repo_root()
    config = load_config(None, root)
    project = Project(config)
    unit = project.resolve_unit(test.unit)
    unit_report = report_unit(project, unit)
    fn_match = find_function_match(unit_report, test.symbol)
    match_percent = fn_match.match_percent if fn_match else None
    status = classify_status(match_percent, unit_report, symbol=test.symbol)
    ok = match_percent is not None and match_percent >= 100.0
    size_check = check_object_size(
        project_root=root,
        region=test.region,
        unit_hint=test.unit,
        retail_object=retail,
        decomp_object=decomp,
    )

    return StaticCompareResult(
        retail_path=retail,
        decomp_path=decomp,
        match_percent=match_percent,
        status=status,
        ok=ok,
        size_check=size_check,
    )

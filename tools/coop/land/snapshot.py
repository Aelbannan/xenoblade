"""Whole-tree measurement snapshot for the land gate.

A snapshot is a JSON dict::

    {"units": {unit: {"fns": {retail_symbol: mark},
                      "data": {section: identical_bytes},
                      "extab": bool | None,
                      "hard": {category: n}}},
     "headers": {"hdr/...": {"hard": {...}}},
     "build_ok": bool, "errors": {unit: str}}

Objects must already be built (the caller runs one locked full build);
hexdiff runs with ``--no-build`` so units can be measured in parallel.
"""
from __future__ import annotations

import json
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from tools.coop.land import bans

ROOT = Path(__file__).resolve().parents[3]
PY = sys.executable
_IDENT = re.compile(r"(\d+) bytes identical")
_TAIL = re.compile(r"\+ 0x([0-9A-Fa-f]+) retail link tail padding")


def _project():
    from tools.coop.lib.config import load_config
    from tools.coop.lib.project import Project
    cfg = load_config(None, ROOT)
    return Project(cfg), cfg


def section_identical(detail: str) -> int:
    """Identical bytes credited for one data-section result line."""
    m = _IDENT.search(detail)
    if not m:
        return 0
    t = _TAIL.search(detail)
    return int(m.group(1)) + (int(t.group(1), 16) if t else 0)


FN_CACHE = ROOT / "tools" / "coop" / "land" / "state" / "fn_cache.json"
# Inputs that change any unit's hexdiff verdict besides its own two objects.
# hexdiff's whole library is hashed (it imports most of tools/coop/lib).
_GLOBAL_INPUTS = ("config/{r}/symbols.txt", "config/{r}/splits.txt", "objdiff.json",
                  "tools/coop/hexdiff.py", "tools/postprocess_reloc_names.py", "tools/coop/lib/*.py")


def _sha(path: Path) -> str:
    import hashlib
    try:
        return hashlib.sha1(path.read_bytes()).hexdigest()
    except OSError:
        return "-"


def _global_key(region: str) -> str:
    import hashlib
    h = hashlib.sha1()
    for p in _GLOBAL_INPUTS:
        for f in sorted(ROOT.glob(p.format(r=region))) or [ROOT / p.format(r=region)]:
            h.update(f"{f.name}:{_sha(f)}".encode())
    return h.hexdigest()


def _unit_key(project, unit, gkey: str) -> str | None:
    if unit.base_path is None or unit.target_path is None:
        return None
    b = unit.base_path if Path(unit.base_path).is_absolute() else ROOT / unit.base_path
    t = unit.target_path if Path(unit.target_path).is_absolute() else ROOT / unit.target_path
    if not b.is_file() or not t.is_file():
        return None
    return f"{gkey}:{_sha(b)}:{_sha(t)}"


def _load_fn_cache() -> dict:
    try:
        return json.loads(FN_CACHE.read_text())
    except (OSError, json.JSONDecodeError):
        return {}


def _save_fn_cache(cache: dict) -> None:
    FN_CACHE.parent.mkdir(parents=True, exist_ok=True)
    tmp = FN_CACHE.with_suffix(".tmp")
    tmp.write_text(json.dumps(cache))
    tmp.replace(FN_CACHE)


def measure_functions(unit: str, timeout: int = 300) -> tuple[dict[str, str] | None, str]:
    """{retail symbol: mark} from hexdiff --all --json --no-build."""
    try:
        r = subprocess.run(
            [PY, "tools/coop/hexdiff.py", unit, "--all", "--json", "--no-build", "--no-baseline"],
            cwd=ROOT, capture_output=True, text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return None, "hexdiff timeout"
    try:
        d = json.loads(r.stdout)
    except json.JSONDecodeError:
        return None, (r.stderr or r.stdout).strip()[-300:]
    return {f["symbol"]: f.get("mark", "?") for f in d.get("functions", [])}, ""


def measure_data(project, cfg, unit_obj, layout) -> tuple[dict[str, int], bool | None]:
    from tools.coop import run as runmod
    from tools.coop.lib.data_match import check_data_sections, check_extab_sections, has_data_sections

    retail, decomp = runmod._data_object_paths(project, unit_obj)
    if retail is None or decomp is None or not Path(retail).is_file() or not (ROOT / decomp).is_file():
        return {}, None
    decomp = ROOT / decomp if not Path(decomp).is_absolute() else Path(decomp)
    if not has_data_sections(decomp) and not has_data_sections(retail):
        data = {}
    else:
        view = runmod._linked_object(project, cfg, unit_obj, decomp)
        res = check_data_sections(retail, view.path)
        data = {s.name: section_identical(s.detail) for s in res.sections}
        decomp = view.path
    ex = check_extab_sections(retail, decomp, layout=layout)
    return data, bool(ex.ok)


def measure_hard() -> tuple[dict[str, dict[str, int]], dict[str, dict[str, int]]]:
    """Per-unit and per-unpaired-header hard counts: faithful HARD + extra bans."""
    from tools.coop import faithful
    m = faithful.measure("us")
    hdr = faithful.measure_unpaired_headers(m)

    def ban_counts(paths):
        tot: dict[str, int] = {}
        for p in paths:
            if not p:
                continue
            f = ROOT / p
            if f.is_file():
                for k, v in bans.count(f.read_text(encoding="utf-8", errors="replace")).items():
                    tot["ban:" + k] = tot.get("ban:" + k, 0) + v
        return tot

    units = {u: {**dict(v["hard"]), **ban_counts([v.get("source"), v.get("header")])} for u, v in m.items()}
    headers = {}
    for name, v in hdr.items():
        src = name.removeprefix(faithful.HEADER_PSEUDO_PREFIX)
        headers[name] = {**dict(v["hard"]), **ban_counts([src])}
    return units, headers


def take(units: list[str] | None = None, jobs: int = 8, build_ok: bool = True) -> dict:
    from tools.coop import run as runmod
    project, cfg = _project()
    all_units = project.load_objdiff_units()
    if units is not None:
        want = {u.removeprefix("main/") for u in units}
        all_units = [u for u in all_units if u.name.removeprefix("main/") in want]
    layout = runmod._extab_layout(project, cfg)
    out: dict = {"units": {}, "headers": {}, "errors": {}, "build_ok": build_ok}

    # hexdiff subprocesses in parallel; data/extab in-process and sequential
    # (weak_dedup's linked view writes a cache, so keep it single-threaded).
    # Function marks are cached per unit by (decomp object, retail object,
    # global inputs) so unchanged units are not re-diffed.
    gkey = _global_key(cfg.region)
    cache = _load_fn_cache()
    keys = {u.name: _unit_key(project, u, gkey) for u in all_units}
    todo = [u for u in all_units if not (keys[u.name] and cache.get(u.name, [None])[0] == keys[u.name])]
    fn_results = {u.name: (cache[u.name][1], "") for u in all_units if u not in todo}
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        fn_results.update(zip([u.name for u in todo], ex.map(lambda u: measure_functions(u.name), todo)))
    for u in todo:
        fns, err = fn_results[u.name]
        if fns is not None and not err and keys[u.name]:
            cache[u.name] = [keys[u.name], fns]
    _save_fn_cache(cache)
    out["measured"] = len(todo)
    for u in all_units:
        fns, err = fn_results[u.name]
        try:
            data, extab = measure_data(project, cfg, u, layout)
        except Exception as exc:  # measurement failure is recorded, never treated as a pass
            data, extab, err = {}, None, (err + f" data:{exc!r}").strip()
        out["units"][u.name] = {"fns": fns or {}, "data": data, "extab": extab}
        if fns is None or err:
            out["errors"][u.name] = err or "no hexdiff result"
    hard, headers = measure_hard()
    for name, rec in out["units"].items():
        rec["hard"] = hard.get(name, {})
    out["headers"] = {k: {"hard": v} for k, v in headers.items()}
    out["ban_keys"] = list(bans.KEYS)
    return out


def save(snap: dict, path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(json.dumps(snap, sort_keys=True))
    tmp.replace(path)


def load(path: Path) -> dict:
    return json.loads(Path(path).read_text())

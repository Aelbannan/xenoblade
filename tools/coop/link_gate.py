#!/usr/bin/env python3
"""Shrink-only main.elf link ratchet (``run.py link-gate``).

Modeled on ``tools/check_unit_rules_frozen.py --ratchet``.  The full link is
attempted (``ninja -k 0 build/<region>/main.elf``), its log is scanned for
multiply-defined / small-data-relocation / ELF_linker internal errors, and the
uncapped missing-symbol list comes from ``link_undefs.py``.  The result is
compared to ``docs/evidence/link_undefs_baseline.json``:

* any missing name not in the baseline            -> FAIL
* any multiply-defined name not in the baseline   -> FAIL
* any error count above its baseline count        -> FAIL
* ``config/*/symbols.txt`` duplicate global names -> FAIL

``--ratchet`` rewrites the baseline, but only by removing entries / lowering
counts (it refuses when anything regressed).  Once the baseline has zero
missing names the gate additionally requires ``main.elf`` to exist and the
``build/<region>/ok`` (``build.sha1``) check to pass.

CI: only the symbols.txt duplicate check runs in CI (build.yml,
ppc-equivalence job).  The full gate is LOCAL-ONLY: the CI build job never
links main.elf (it builds ``all_source progress``), needs the private
BUILD_IMAGE container plus /orig, and the link currently fails by design.
Run ``.venv/bin/python3 tools/coop/run.py link-gate`` before landing link work.

Pure comparison logic lives in module functions so tests never run the link.
"""
from __future__ import annotations

import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BASELINE = ROOT / "docs" / "evidence" / "link_undefs_baseline.json"
COUNT_KEYS = ("multiply_defined", "sda_reloc", "elf_internal")

#: Shrink-only name sets: (baseline key, label). ``symbols_dupes`` holds
#: pre-existing config/*/symbols.txt duplicate globals ("<path>: <name>").
NAME_KEYS = (
    ("missing", "missing"),
    ("multiply_defined_names", "multiply-defined"),
    ("symbols_dupes", "symbols.txt duplicate global"),
)

_MULTI_RE = re.compile(r"multiply[- ]defined", re.I)
_SDA_RE = re.compile(r"Small data relocation", re.I)
_INTERNAL_RE = re.compile(r"ELF_linker.*internal|internal.*ELF_linker", re.I)
_QUOTED_RE = re.compile(r"'([^']+)'")


# ---------------------------------------------------------------- parsing
def parse_link_log(text: str) -> dict:
    """Count linker error classes and collect multiply-defined names."""
    lines = text.splitlines()
    multi: list[str] = []
    counts = {k: 0 for k in COUNT_KEYS}
    for i, line in enumerate(lines):
        if _MULTI_RE.search(line):
            counts["multiply_defined"] += 1
            # mwld puts the quoted symbol on the same or the preceding line.
            m = _QUOTED_RE.search(line) or (i and _QUOTED_RE.search(lines[i - 1]))
            multi.append(m.group(1) if m else line.strip("# ").strip())
        if _SDA_RE.search(line):
            counts["sda_reloc"] += 1
        if _INTERNAL_RE.search(line):
            counts["elf_internal"] += 1
    return {"counts": counts, "multiply_defined_names": sorted(set(multi))}


def link_attempted(log: str, region: str) -> bool:
    """True iff ninja actually ran the main.elf link step (or it was up to date)."""
    return f"LINK build/{region}/main.elf" in log or "ninja: no work to do" in log


def canonical_missing(rows: list[dict]) -> list[str]:
    """Missing names, with compiler-local ``@N`` labels made edit-stable.

    MWCC numbers ``@N`` labels per TU in declaration order, so any header edit
    renumbers them without changing what is missing. Compare them as a count
    per referring object instead: ``@local[<referrer stem>]#<k>``.
    """
    names: set[str] = set()
    local: dict[str, int] = {}
    for row in rows:
        sym = row["symbol"]
        if not re.fullmatch(r"@\d+", sym):
            names.add(sym)
            continue
        refs = row.get("referrers") or ["?"]
        stem = Path(refs[0]).name.split(".")[0]
        local[stem] = local.get(stem, 0) + 1
    for stem, n in local.items():
        names.update(f"@local[{stem}]#{k}" for k in range(1, n + 1))
    return sorted(names)


def make_result(undefs: dict, log_info: dict) -> dict:
    return {
        "missing": canonical_missing(undefs.get("missing", [])),
        "multiply_defined_names": log_info["multiply_defined_names"],
        "counts": dict(log_info["counts"]),
    }


# ------------------------------------------------------------- comparison
def compare(baseline: dict, current: dict) -> tuple[list[str], list[str]]:
    """(regressions, improvements) of *current* against *baseline*."""
    regress: list[str] = []
    improve: list[str] = []
    for key, label in NAME_KEYS:
        base, cur = set(baseline.get(key, [])), set(current.get(key, []))
        regress += [f"new {label}: {n}" for n in sorted(cur - base)]
        improve += [f"retired {label}: {n}" for n in sorted(base - cur)]
    for k in COUNT_KEYS:
        b, c = baseline.get("counts", {}).get(k, 0), current.get("counts", {}).get(k, 0)
        if c > b:
            regress.append(f"{k} count rose {b} -> {c}")
        elif c < b:
            improve.append(f"{k} count fell {b} -> {c}")
    return regress, improve


def ratchet(baseline: dict, current: dict) -> dict:
    """Shrink-only update: intersect name sets, take min counts."""
    out = dict(baseline)
    for key, _ in NAME_KEYS:
        out[key] = sorted(set(baseline.get(key, [])) & set(current.get(key, [])))
    out["counts"] = {
        k: min(baseline.get("counts", {}).get(k, 0), current.get("counts", {}).get(k, 0))
        for k in COUNT_KEYS
    }
    return out


def sha1_gate_active(baseline: dict) -> bool:
    return not baseline.get("missing")


# ------------------------------------------------------ symbols.txt dupes
_SYM_RE = re.compile(r"^\s*([^\s=]+)\s*=")


def duplicate_globals(text: str) -> list[str]:
    """Global (non-``scope:local``) names defined more than once."""
    seen: set[str] = set()
    dupes: set[str] = set()
    for line in text.splitlines():
        m = _SYM_RE.match(line)
        if not m or "scope:local" in line:
            continue
        name = m.group(1)
        (dupes if name in seen else seen).add(name)
    return sorted(dupes)


def check_symbols_dupes(root: Path = ROOT) -> list[str]:
    errs = []
    for path in sorted((root / "config").glob("*/symbols.txt")):
        for name in duplicate_globals(path.read_text(errors="replace")):
            errs.append(f"{path.relative_to(root)}: {name}")
    return errs


# ------------------------------------------------------------- live run
def run_link(region: str, ninja: str = str(Path(__file__).resolve().parents[2] / "ninja")) -> tuple[dict, str, int]:
    """Caller MUST hold the build lock. Returns (result, log, link rc)."""
    elf = f"build/{region}/main.elf"
    proc = subprocess.run(
        [ninja, "-k", "0", elf], cwd=ROOT, capture_output=True, text=True
    )
    log = proc.stdout + proc.stderr
    with_json = ROOT / "build" / region / "link_undefs.json"
    subprocess.run(
        [sys.executable, str(ROOT / "tools/coop/link_undefs.py"),
         "--region", region, "--json", str(with_json)],
        cwd=ROOT, capture_output=True, text=True,
    )
    undefs = json.loads(with_json.read_text()) if with_json.is_file() else {"missing": []}
    return make_result(undefs, parse_link_log(log)), log, proc.returncode


def check_sha1(region: str, ninja: str = str(Path(__file__).resolve().parents[2] / "ninja")) -> list[str]:
    """Active only at zero missing: main.elf must exist and build.sha1 match."""
    if not (ROOT / "build" / region / "main.elf").is_file():
        return [f"build/{region}/main.elf missing"]
    proc = subprocess.run([ninja, f"build/{region}/ok"], cwd=ROOT, capture_output=True, text=True)
    return [] if proc.returncode == 0 else [f"build.sha1 mismatch:\n{proc.stdout[-2000:]}"]


def load_baseline(path: Path = BASELINE) -> dict:
    return json.loads(path.read_text())


def write_baseline(data: dict, path: Path = BASELINE) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=1, sort_keys=True) + "\n")


def gate(region: str, do_ratchet: bool, init: bool, with_lock) -> int:
    """Entry point used by ``run.py link-gate``. *with_lock(fn)* holds the build lock."""
    dupes = check_symbols_dupes()
    errors: list[str] = []
    holder: dict = {}

    def _locked() -> int:
        holder["result"], holder["log"], holder["rc"] = run_link(region)
        if not init and BASELINE.is_file() and sha1_gate_active(load_baseline()):
            holder["sha1"] = check_sha1(region)
        return 0

    with_lock(_locked)
    if "result" not in holder:
        print("link-gate: could not acquire build lock", file=sys.stderr)
        return 2
    current = holder["result"]
    log_path = ROOT / "build" / region / "link_gate.log"
    log_path.write_text(holder["log"])
    if not link_attempted(holder["log"], region):
        print(
            f"FAIL: link never ran (an input object failed to build); see {log_path.relative_to(ROOT)}",
            file=sys.stderr,
        )
        return 1
    current["region"] = region
    current["symbols_dupes"] = dupes
    if init:
        write_baseline(current)
        print(f"link-gate: wrote initial baseline {BASELINE.relative_to(ROOT)}")
        _summary(current)
        return 0

    baseline = load_baseline()
    regress, improve = compare(baseline, current)
    errors += regress + holder.get("sha1", [])
    _summary(current)
    for line in improve:
        print(f"  improved: {line}")
    if errors:
        for line in errors:
            print(f"FAIL: {line}", file=sys.stderr)
        print(f"(link log: {log_path.relative_to(ROOT)})", file=sys.stderr)
        return 1
    if do_ratchet and improve:
        write_baseline(ratchet(baseline, current))
        print(f"link-gate: ratcheted baseline ({len(improve)} change(s))")
    elif improve:
        print("link-gate: OK; run with --ratchet to lock in the improvements")
    else:
        print("link-gate: OK")
    return 0


def _summary(r: dict) -> None:
    c = r["counts"]
    print(
        f"link-gate: missing={len(r['missing'])} multiply_defined={c['multiply_defined']} "
        f"sda_reloc={c['sda_reloc']} elf_internal={c['elf_internal']} "
        f"symbols_dupes={len(r.get('symbols_dupes', []))}"
    )


def symbols_only(path: Path = BASELINE) -> int:
    """Cheap standalone check: no symbols.txt duplicate globals beyond the baseline."""
    allowed = set(load_baseline(path).get("symbols_dupes", [])) if path.is_file() else set()
    cur = check_symbols_dupes()
    new = [d for d in cur if d not in allowed]
    for d in new:
        print(f"FAIL: new symbols.txt duplicate global: {d}", file=sys.stderr)
    print(f"symbols.txt: {len(cur)} duplicate global(s), {len(new)} new vs baseline")
    return 1 if new else 0

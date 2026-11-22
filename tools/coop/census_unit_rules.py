#!/usr/bin/env python3
"""UNIT_RULES no-op census and retirement (tools/postprocess_reloc_names.py).

For every decomp object whose basename has a UNIT_RULES entry, re-run
``postprocess_object`` on a scratch copy with (a) the full rules and (b) each
non-default field reset to its default.  An entry is a **no-op** when the full
output equals the input for every object sharing its basename key (entries are
keyed by basename, so nw4r/nw4hbm ``lyt_*`` / ``ut_*`` share one entry); a
field is a no-op when resetting it leaves the output byte-identical for every
such object.

    python tools/coop/census_unit_rules.py                 # census -> JSON + summary
    python tools/coop/census_unit_rules.py --write         # also delete no-op entries/fields
    python tools/coop/census_unit_rules.py --skip a.o,b.o  # never touch these keys

``--write`` edits only the UNIT_RULES literal, removes each retired field's
keyword (with the comment block directly above it), and re-parses the file.
Run ``tools/check_unit_rules_frozen.py --ratchet`` and the land gate afterwards.
"""
from __future__ import annotations

import argparse
import ast
import contextlib
import dataclasses
import hashlib
import io
import json
import shutil
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools import postprocess_reloc_names as pp  # noqa: E402

PP = ROOT / "tools" / "postprocess_reloc_names.py"
OUT = Path("/tmp/xb/orch/unit_rules_census.json")


def _sha(p: Path) -> str:
    return hashlib.sha1(p.read_bytes()).hexdigest()


def _run(src: Path, rules, tmp: str) -> str:
    dst = Path(tmp) / src.name
    shutil.copyfile(src, dst)
    with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
        try:
            pp.postprocess_object(dst, rules)
        except Exception as e:  # noqa: BLE001
            return "EXC:" + repr(e)[:80]
    return _sha(dst)


def _defaults() -> dict:
    out = {}
    for f in dataclasses.fields(pp.UnitRules):
        if f.default is not dataclasses.MISSING:
            out[f.name] = f.default
        elif f.default_factory is not dataclasses.MISSING:  # type: ignore[misc]
            out[f.name] = f.default_factory()  # type: ignore[misc]
    return out


def census() -> dict[str, dict]:
    """{basename key: {objects, full_noop, noop_fields}} over every built object."""
    defaults = _defaults()
    objs = [o for o in sorted((ROOT / "build/us/src").rglob("*.o"))
            if not o.name.endswith(".reloc.o") and pp.unit_has_rules(o.name)]
    per: dict[str, dict] = {}
    with tempfile.TemporaryDirectory() as tmp:
        for src in objs:
            rules = pp.select_unit_rules(src)
            if rules is None:
                continue
            full = _run(src, rules, tmp)
            rec = per.setdefault(src.name, {"objects": [], "full_noop": True, "field_noop": {}})
            rec["objects"].append(str(src.relative_to(ROOT)))
            rec["full_noop"] &= full == _sha(src)
            for f in dataclasses.fields(rules):
                if f.name not in defaults or getattr(rules, f.name) == defaults[f.name]:
                    continue
                alt = _run(src, dataclasses.replace(rules, **{f.name: defaults[f.name]}), tmp)
                rec["field_noop"][f.name] = rec["field_noop"].get(f.name, True) and alt == full
    for rec in per.values():
        rec["noop_fields"] = sorted(k for k, v in rec.pop("field_noop").items() if v)
    return per


def _entries(tree: ast.Module):
    node = next(n for n in tree.body if isinstance(n, (ast.Assign, ast.AnnAssign))
                and any(getattr(t, "id", None) == "UNIT_RULES"
                        for t in (n.targets if isinstance(n, ast.Assign) else [n.target])))
    return list(zip(node.value.keys, node.value.values))


def _with_comments(lines: list[str], a: int) -> int:
    while a > 0 and lines[a - 1].lstrip().startswith("#"):
        a -= 1
    return a


def retire(per: dict[str, dict], skip: set[str]) -> tuple[list[str], list[str]]:
    src = PP.read_text()
    lines = src.splitlines(keepends=True)
    spans: list[tuple[int, int]] = []
    gone, fields = [], []
    for kn, vn in _entries(ast.parse(src)):
        key = kn.value if isinstance(kn, ast.Constant) else None
        rec = per.get(key)
        if not rec or key in skip:
            continue
        if rec["full_noop"]:
            spans.append((_with_comments(lines, kn.lineno - 1), vn.end_lineno - 1))
            gone.append(key)
            continue
        if not isinstance(vn, ast.Call):
            continue
        for kw in vn.keywords:
            if kw.arg in rec["noop_fields"]:
                a, b = kw.value.lineno - 1, kw.value.end_lineno - 1
                if not lines[a].lstrip().startswith(f"{kw.arg}=") or not lines[b].rstrip().endswith(","):
                    continue  # not one-keyword-per-line layout; leave for a human
                spans.append((_with_comments(lines, a), b))
                fields.append(f"{key}:{kw.arg}")
    for a, b in sorted(spans, reverse=True):
        del lines[a:b + 1]
    out = "".join(lines)
    ast.parse(out)
    PP.write_text(out)
    return gone, fields


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--skip", default="", help="comma-separated basename keys to leave alone")
    ap.add_argument("--out", type=Path, default=OUT)
    a = ap.parse_args(argv)
    per = census()
    a.out.parent.mkdir(parents=True, exist_ok=True)
    a.out.write_text(json.dumps(per, indent=1))
    noop = sorted(k for k, v in per.items() if v["full_noop"])
    nf = sum(len(v["noop_fields"]) for k, v in per.items() if not v["full_noop"])
    print(f"keys with built objects: {len(per)}; whole-entry no-op: {len(noop)}; no-op fields: {nf}")
    print("no-op entries:", noop)
    if a.write:
        gone, fields = retire(per, {s for s in a.skip.split(",") if s})
        print(f"retired {len(gone)} entries, {len(fields)} fields:", gone, fields)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

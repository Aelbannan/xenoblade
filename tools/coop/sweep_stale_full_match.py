#!/usr/bin/env python3
"""One-shot sweep (r6 F1): demote FULL_MATCH rows whose current bodies are
not byte-identical.

The pre-r5 certify fallthrough minted full-instruction-match certificates for
non-byte-identical pairs (CRITICAL, fixed in beef11e2f). The registry still
holds rows certified by that path. r5 demoted only us-8036c9c0; this sweep
finds every remaining FULL_MATCH row whose retail/decomp bodies differ
(byte-identity + reloc sites) and demotes it to NOT_STARTED with the stale
certificate cleared (targets.json + sidecar).

Read-only w.r.t. builds: uses the objects already in build/<region>/, no
rebuild. Uses the same byte-identity check the pipeline uses
(_byte_identical_with_relocs), so rows that legitimately certify today are
untouched. Rows whose objects/symbols are unavailable are left alone.
"""

from __future__ import annotations

import argparse
import json
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from tools.coop.lib.config import load_config  # noqa: E402
from tools.coop.lib.project import Project  # noqa: E402
from tools.coop.lib.targets import (  # noqa: E402
    certs_sidecar_path,
    exclusive_targets_lock,
    load_targets_document,
    targets_path,
)
from tools.ppc_equivalence.elf_symbols import ElfSymbolError, extract_function_pair  # noqa: E402
from tools.coop.lib.equivalence_check import _byte_identical_with_relocs  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true", help="print the demotion list, change nothing")
    ap.add_argument("--limit", type=int, default=0, help="max demotions (0 = all)")
    args = ap.parse_args()

    config = load_config(None, Path.cwd().resolve())
    project = Project(config)
    document = load_targets_document(config)
    rows = [r for r in document.get("targets", []) if isinstance(r, dict)]

    stale: list[dict] = []
    errors: Counter[str] = Counter()
    for row in rows:
        if row.get("status") != "FULL_MATCH":
            continue
        unit_name = row.get("unit")
        symbol = row.get("symbol")
        if not unit_name or not symbol:
            errors["no-unit-or-symbol"] += 1
            continue
        try:
            unit = project.resolve_unit(unit_name)
            if unit.target_path is None or unit.base_path is None:
                errors["no-objects"] += 1
                continue
            left, right = extract_function_pair(unit.target_path, unit.base_path, symbol)
            if not _byte_identical_with_relocs(left, right):
                stale.append(row)
        except (ElfSymbolError, FileNotFoundError, KeyError, ValueError) as exc:
            errors[type(exc).__name__] += 1
        except Exception as exc:  # noqa: BLE001 — sweep must not die on one row
            errors[f"{type(exc).__name__}:{str(exc)[:40]}"] += 1

    print(f"FULL_MATCH rows scanned: {len([r for r in rows if r.get('status')=='FULL_MATCH'])}")
    print(f"stale (non-byte-identical) FULL_MATCH rows: {len(stale)}")
    print(f"skipped due to errors: {dict(errors)}")

    if args.limit:
        stale = stale[: args.limit]

    if args.dry_run:
        for row in stale:
            print(f"  [dry-run] would demote {row.get('id')} {row.get('unit','').split('/')[-1]}")
        return 0

    ids = [row.get("id") for row in stale]
    with exclusive_targets_lock(config):
        document = load_targets_document(config)
        # load_targets_document merges the certificate sidecar into rows —
        # strip embedded certs before writing so the main registry file never
        # carries sidecar data (r6 F1 sweep regression: first run wrote 5481
        # merged certs back, ballooning targets.json 17.7MB -> 112.9MB).
        for row in document.get("targets", []):
            if isinstance(row, dict):
                row.pop("equivalence_certificate", None)
        by_id = {str(r.get("id")): r for r in document.get("targets", []) if isinstance(r, dict)}
        changed = 0
        for row_id in ids:
            row = by_id.get(str(row_id))
            if row is None or row.get("status") != "FULL_MATCH":
                continue
            row["status"] = "NOT_STARTED"
            row["workflow_status"] = "BACKLOG"
            row.pop("equivalence_status", None)
            row.pop("equivalence_certificate", None)
            row.pop("certificate_checked", None)
            row.pop("equivalence_confidence", None)
            row.pop("equivalence_policy", None)
            changed += 1
        with targets_path(config).open("w", encoding="utf-8") as f:
            json.dump(document, f, indent=2, ensure_ascii=False)
            f.write("\n")
        # Clear stale sidecar certs for demoted ids.
        sidecar_path = certs_sidecar_path(targets_path(config))
        if sidecar_path.is_file() and ids:
            import gzip

            kept: list[str] = []
            with gzip.open(sidecar_path, "rt", encoding="utf-8") as f:
                for line in f:
                    line = line.strip()
                    if not line:
                        continue
                    try:
                        pair = json.loads(line)
                    except ValueError:
                        continue
                    if isinstance(pair, list) and len(pair) == 2 and pair[0] in ids:
                        continue  # drop stale cert
                    kept.append(line)
            with gzip.open(sidecar_path, "wt", encoding="utf-8") as f:
                for line in kept:
                    f.write(line + "\n")
        print(f"demoted {changed} rows (targets.json + sidecar cleared)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

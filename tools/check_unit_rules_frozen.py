#!/usr/bin/env python3
"""Frozen-baseline gate for tools/postprocess_reloc_names.py's UNIT_RULES.

PLAN.md §17.6 does not approve object post-processing. The table is legacy
debt and may only SHRINK. This checker enforces:

  * no new rule keys (current keys must be a subset of the frozen manifest),
  * no new fields and no changed field values on a remaining entry
    (removing a field is burn-down progress and is allowed),
  * current entry count <= the frozen ``baseline_count``.

Removals are always allowed and do not require editing the manifest. After a
burn-down batch, run ``--ratchet`` once to lower ``baseline_count``. A changed
field value also requires ``--ratchet``: the command prints the old -> new
diff for review, so every payload change is visible in the manifest diff.
Entries removed in the past stay in the manifest as tombstones: a key that was
legitimately retired can never be re-added without a human regenerating the
manifest (``--write --force``), which is the point.

Metadata: every entry carries ``class`` (auto-derived triage hint:
content / layout / rename / other), ``owner`` and ``reason``. Agents claim
in-scope entries with ``--claim`` as they work the burn-down. ``--check``
hard-fails on new/changed entries and count growth; missing owner/reason is
reported and only fails with ``--strict`` (enable once the backfill lands).

Usage:
  python3 tools/check_unit_rules_frozen.py --check
  python3 tools/check_unit_rules_frozen.py --report
  python3 tools/check_unit_rules_frozen.py --ratchet
  python3 tools/check_unit_rules_frozen.py --claim "<key>" --owner <name> \
      [--reason "..."] [--class content|layout|rename|other]
  python3 tools/check_unit_rules_frozen.py --write [--force]   # first creation
"""

from __future__ import annotations

import argparse
import contextlib
import dataclasses
import hashlib
import json
import sys
import tempfile
from pathlib import Path

try:  # POSIX; CI is Linux, agents are macOS.
    import fcntl
except ImportError:  # pragma: no cover - Windows
    fcntl = None  # type: ignore[assignment]

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.postprocess_reloc_names import UNIT_RULES, UnitRules  # noqa: E402

MANIFEST = ROOT / "tools" / "postprocess_reloc_names.frozen.json"
SCHEMA = 1

# Triage classes. Overlaps resolve in this priority order (content > layout >
# rename > other); this is a hint for routing, not a verdict — category agents
# refine it via --claim.
CONTENT_FIELDS = {
    "patch_data",
    "copy_data_sections",
    "swap_data_blocks",
    "permute_sdata2_words",
    "swap_sdata2_leading_f32_words",
    "swap_sdata2_leading_f32_pair",
    "reverse_sdata2_trailing_f32x4",
    "patch_unsigned_magic",
    "insn_patches",
    "insn_patches_post",
    "insert_insns",
    "reloc_offset_moves",
    "data_reloc_offset_moves",
    "addend_patches",
    "addend_sets",
    "inject_relocs",
    "bake_linker_addrs",
    "force_symbol_relocs",
}
LAYOUT_FIELDS = {
    "drop_data_tail",
    "drop_data_range",
    "drop_nobits_range",
    "pad_data_section",
    "extern_data_sections",
    "trim_text_size",
    "pad_text_size",
    "drop_text_symbols",
    "drop_text_symbols_as_undef",
    "repack_after_drop",
    "zero_nobits",
    "zero_data_range",
    "retarget_relocs",
    "retarget_relocs_local",
    "pad_sdata2_size",
    "trim_sdata2_size",
}
RENAME_FIELDS = {
    "exact_renames",
    "prefix_renames",
    "pool_patterns",
    "data_pool_patterns",
    "add_symbols",
    "globalize_symbols",
    "symbol_sizes",
    "set_data_align",
}
CLASS_PRIORITY = (
    ("content", CONTENT_FIELDS),
    ("layout", LAYOUT_FIELDS),
    ("rename", RENAME_FIELDS),
)
CLASSES = ("content", "layout", "rename", "other")


def rule_class(rules: UnitRules) -> str:
    used = {f.name for f in dataclasses.fields(UnitRules) if getattr(rules, f.name)}
    for name, fields in CLASS_PRIORITY:
        if used & fields:
            return name
    return "other"


def _canon(value):
    if isinstance(value, bytes):
        return "hex:" + value.hex()
    if isinstance(value, bool) or value is None or isinstance(value, (int, str)):
        return value
    if isinstance(value, (tuple, list)):
        return [_canon(v) for v in value]
    raise TypeError(f"unexpected UNIT_RULES field value type: {type(value).__name__}")


def canonical_fields(rules: UnitRules) -> dict:
    """Wire-stable payload for one entry: non-empty dataclass fields only."""
    out: dict = {}
    for f in dataclasses.fields(UnitRules):
        value = getattr(rules, f.name)
        if value:
            out[f.name] = _canon(value)
    return out


def table_snapshot() -> dict[str, dict]:
    return {key: canonical_fields(rules) for key, rules in UNIT_RULES.items()}


def load_manifest() -> dict:
    if not MANIFEST.is_file():
        return {"schema": SCHEMA, "baseline_count": None, "entries": {}}
    data = json.loads(MANIFEST.read_text())
    if not isinstance(data, dict) or not isinstance(data.get("entries"), dict):
        raise SystemExit(f"ERROR: malformed manifest: {MANIFEST}")
    return data


@contextlib.contextmanager
def _manifest_lock():
    """Serialize read-modify-write on the manifest across concurrent agents."""
    if fcntl is None:  # pragma: no cover - Windows
        yield
        return
    digest = hashlib.sha1(str(MANIFEST).encode()).hexdigest()[:12]
    lock_path = Path(tempfile.gettempdir()) / f"xenoblade-unit-rules-{digest}.lock"
    with open(lock_path, "w") as fh:
        fcntl.flock(fh, fcntl.LOCK_EX)
        try:
            yield
        finally:
            fcntl.flock(fh, fcntl.LOCK_UN)


def _save(manifest: dict) -> None:
    MANIFEST.write_text(json.dumps(manifest, indent=1, sort_keys=True) + "\n")


def _field_diff(current: dict, frozen: dict) -> tuple[list[str], list[str]]:
    """Return (new fields, changed field values) for one entry.

    Fields present in ``frozen`` but absent from ``current`` are allowed:
    removing a rule field from an entry is burn-down progress.
    """
    new = sorted(k for k in current if k not in frozen)
    changed = sorted(k for k in current if k in frozen and current[k] != frozen[k])
    return new, changed


def _manifest_label() -> str:
    try:
        return str(MANIFEST.relative_to(ROOT))
    except ValueError:  # tests monkeypatch MANIFEST to a temp dir
        return str(MANIFEST)


def cmd_check(*, strict: bool) -> int:
    snapshot = table_snapshot()
    manifest = load_manifest()
    entries: dict = manifest.get("entries", {})
    baseline = manifest.get("baseline_count")

    problems: list[str] = []
    for key in sorted(set(snapshot) - set(entries)):
        problems.append(f"NEW     {key}: not in frozen manifest (UNIT_RULES is shrink-only)")
    retired_fields = 0
    for key in sorted(set(snapshot) & set(entries)):
        new_fields, changed_fields = _field_diff(
            snapshot[key], entries[key].get("fields", {})
        )
        for field in new_fields:
            problems.append(f"NEWFIELD {key}.{field}: field not in frozen baseline")
        for field in changed_fields:
            problems.append(f"CHANGED  {key}.{field}: value differs from frozen baseline")
        retired_fields += len(set(entries[key].get("fields", {})) - set(snapshot[key]))
    if baseline is None:
        problems.append("MANIFEST: baseline_count missing")
    elif len(snapshot) > baseline:
        problems.append(
            f"COUNT   {len(snapshot)} entries > frozen baseline {baseline}"
        )

    missing_owner = sorted(
        key
        for key in snapshot
        if not entries.get(key, {}).get("owner")
        or entries[key].get("owner") == "unassigned"
    )
    missing_reason = sorted(key for key in snapshot if not entries.get(key, {}).get("reason"))
    removed = sorted(set(entries) - set(snapshot))

    print(f"manifest: {_manifest_label()}")
    print(f"entries:  current {len(snapshot)}  baseline {baseline}  retired {len(removed)}")
    print(
        f"metadata: {len(missing_owner)} without owner, "
        f"{len(missing_reason)} without reason"
    )
    if retired_fields:
        print(f"progress: {retired_fields} retired field(s) awaiting --ratchet")
    if strict and (missing_owner or missing_reason):
        for key in missing_owner:
            problems.append(f"META    {key}: no owner")
        for key in missing_reason:
            problems.append(f"META    {key}: no reason")
    if problems:
        print("\nFAIL:")
        for problem in problems:
            print(f"  {problem}")
        return 1
    print("\nOK: UNIT_RULES is within the frozen baseline.")
    return 0


def cmd_report() -> int:
    snapshot = table_snapshot()
    manifest = load_manifest()
    entries: dict = manifest.get("entries", {})
    by_class: dict[str, int] = {}
    by_owner: dict[str, int] = {}
    unassigned = 0
    for key in snapshot:
        meta = entries.get(key, {})
        by_class[meta.get("class", "?")] = by_class.get(meta.get("class", "?"), 0) + 1
        owner = meta.get("owner") or "unassigned"
        by_owner[owner] = by_owner.get(owner, 0) + 1
        if owner == "unassigned":
            unassigned += 1
    removed = len(set(entries) - set(snapshot))
    print(f"current entries: {len(snapshot)}  (baseline {manifest.get('baseline_count')}, retired {removed})")
    print("by class:  " + ", ".join(f"{k}={v}" for k, v in sorted(by_class.items())))
    print("by owner:  " + ", ".join(f"{k}={v}" for k, v in sorted(by_owner.items())))
    print(f"unassigned: {unassigned}")
    return 0


def cmd_ratchet() -> int:
    with _manifest_lock():
        snapshot = table_snapshot()
        manifest = load_manifest()
        entries: dict = manifest.get("entries", {})
        new_keys = sorted(set(snapshot) - set(entries))
        new_fields: list[str] = []
        changed_fields: list[tuple[str, str]] = []
        for key in set(snapshot) & set(entries):
            nf, cf = _field_diff(snapshot[key], entries[key].get("fields", {}))
            new_fields += [f"{key}.{field}" for field in nf]
            changed_fields += [(key, field) for field in cf]
        if new_keys or new_fields:
            print("ERROR: cannot ratchet: table has new keys/fields", file=sys.stderr)
            for key in new_keys:
                print(f"  NEW      {key}", file=sys.stderr)
            for item in sorted(new_fields):
                print(f"  NEWFIELD {item}", file=sys.stderr)
            return 1
        if changed_fields:
            print("payload changes to be recorded by this ratchet (review the diff):")
            for key, field in sorted(changed_fields):
                before = json.dumps(entries[key].get("fields", {}).get(field))[:100]
                after = json.dumps(snapshot[key].get(field))[:100]
                print(f"  {key}.{field}: {before} -> {after}")
        for key, fields in snapshot.items():
            entries[key]["fields"] = fields
        old = manifest.get("baseline_count")
        manifest["baseline_count"] = len(snapshot)
        manifest.setdefault("entries", entries)
        _save(manifest)
        print(f"baseline_count: {old} -> {len(snapshot)}")
    return 0


def cmd_claim(key: str, owner: str | None, reason: str | None, cls: str | None) -> int:
    with _manifest_lock():
        manifest = load_manifest()
        entries: dict = manifest.get("entries", {})
        if key not in entries:
            print(
                f"ERROR: {key!r} is not in the frozen manifest. New entries are "
                "forbidden; only existing entries can be claimed.",
                file=sys.stderr,
            )
            return 1
        entry = entries[key]
        if cls is not None:
            entry["class"] = cls
        if owner is not None:
            entry["owner"] = owner
        if reason is not None:
            entry["reason"] = reason
        _save(manifest)
        print(
            f"claimed {key}: class={entry.get('class')} owner={entry.get('owner')} "
            f"reason={entry.get('reason')!r}"
        )
    return 0


def cmd_write(*, force: bool) -> int:
    if MANIFEST.is_file() and not force:
        print(
            f"ERROR: {_manifest_label()} exists. Use --check/--claim/--ratchet, "
            "or --write --force to regenerate from the current table (human-reviewed).",
            file=sys.stderr,
        )
        return 2
    old_entries: dict = {}
    if force and MANIFEST.is_file():
        old_entries = load_manifest().get("entries", {})
    entries = {}
    for key, rules in UNIT_RULES.items():
        old = old_entries.get(key, {})
        entries[key] = {
            "class": old.get("class") or rule_class(rules),
            "owner": old.get("owner") or "unassigned",
            "reason": old.get("reason") or "",
            "fields": canonical_fields(rules),
        }
    _save({"schema": SCHEMA, "baseline_count": len(entries), "entries": entries})
    print(f"wrote {_manifest_label()} ({len(entries)} entries)")
    return 0


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--check", action="store_true", help="verify the freeze (default)")
    group.add_argument("--report", action="store_true", help="print class/owner summary")
    group.add_argument("--ratchet", action="store_true", help="lower baseline_count to the current table size")
    group.add_argument("--write", action="store_true", help="create the manifest (first time / --force)")
    group.add_argument("--claim", metavar="KEY", help="annotate one existing manifest entry")
    parser.add_argument("--force", action="store_true", help="with --write: regenerate an existing manifest")
    parser.add_argument("--strict", action="store_true", help="with --check: fail on missing owner/reason")
    parser.add_argument("--owner", help="with --claim: owner name")
    parser.add_argument("--reason", help="with --claim: one-line justification")
    parser.add_argument("--class", dest="cls", choices=CLASSES, help="with --claim: triage class")
    args = parser.parse_args(argv[1:])

    if args.claim:
        return cmd_claim(args.claim, args.owner, args.reason, args.cls)
    if args.ratchet:
        return cmd_ratchet()
    if args.write:
        return cmd_write(force=args.force)
    if args.report:
        return cmd_report()
    return cmd_check(strict=args.strict)


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))

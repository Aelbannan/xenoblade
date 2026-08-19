"""Generic section-based batch planner.

Sections are matched case-insensitively against the target's `unit` prefix,
e.g.  --section criware matches "CriWare/...", --section kyoshin matches
"kyoshin/...", --section rvl_sdk matches "RVL_SDK/...".

Wave 1 only batches targets on the live `ready` frontier (leaf, or every
callee accepted and certified) as computed by tools/coop's harness_targets;
everything else in the section is listed as deferred (not buildable, or
callee-blocked) so agents never burn cycles on targets that cannot be matched
yet.  Known stalls from attempts.jsonl are carried into the batch table so
agents do not repeat ruled-out experiments.
"""
import json
import os
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

from .common import ROOT, TARGETS, VENV_PY, now_iso, save_json, slug

BATCH_SIZE = 30
TU_THRESHOLD = 20          # units with >= this many unmatched targets get their own batch
HARD_BATCH_SIZE = 14       # pooled hard batches flush at this size (cheaper re-exploration)
EASY_SIZE_MAX = 0x400      # per-target size bound for an "easy" batch


def is_leaf(t):
    return (
        not t.get("called_functions")
        and not t.get("unresolved_called_functions")
        and not t.get("has_indirect_calls")
    )


def _size(t):
    try:
        return int(str(t.get("size") or "0x0"), 16)
    except Exception:
        return 0


def _thinking_for(targets):
    """Tiered thinking, one notch below the old defaults: max only for the
    genuinely hard (majority non-leaf) batches."""
    total = len(targets) or 1
    leaves = sum(1 for t in targets if is_leaf(t))
    leaf_frac = leaves / total
    sizes = [_size(t) for t in targets]
    any_big = any(s > EASY_SIZE_MAX for s in sizes)
    if leaf_frac >= 0.8 and not any_big:
        return "medium"
    if leaf_frac >= 0.4:
        return "high"
    return "max"


def section_matches(unit, section):
    return str(unit or "").lower().startswith(section.lower())


def _ready_ids(targets_path):
    """Authoritative ready frontier (leaf or all callees accepted+certified)
    via tools/coop. Returns a set of target ids, or None on failure."""
    try:
        sys.path.insert(0, str(ROOT))
        from tools.coop.lib.config import load_config
        from tools.coop.lib.targets import harness_targets, load_targets
        cfg = load_config(None, ROOT)
        all_t = load_targets(cfg)
        ready = harness_targets(all_t, selection="ready", include_catalog=True)
        return {t.id for t in ready}
    except Exception as e:
        print(f"warning: ready-frontier computation failed ({e}); "
              f"falling back to static leaf/callee gate", file=sys.stderr)
        return None


def _static_ready_ids(targets):
    accepted = {t["id"] for t in targets if t.get("workflow_status") == "ACCEPTED"}
    out = set()
    for t in targets:
        if is_leaf(t):
            out.add(t["id"])
            continue
        called = t.get("called_functions") or []
        if (not t.get("unresolved_called_functions") and not t.get("has_indirect_calls")
                and called and all(c in accepted for c in called)):
            out.add(t["id"])
    return out


def _stall_map(attempt_log):
    """target_id -> latest stall packet summary from attempts.jsonl."""
    stalls = {}
    if not attempt_log.exists():
        return stalls
    try:
        with open(attempt_log) as f:
            for line in f:
                try:
                    d = json.loads(line)
                except Exception:
                    continue
                if d.get("stall_note") and d.get("target_id"):
                    stalls[d["target_id"]] = (
                        f"{d.get('status') or '?'} "
                        f"{d.get('instruction_match') or d.get('match_percent') or '-'}% "
                        f"stall: {(d.get('hypothesis') or d.get('next_change') or '')[:80]}"
                    )
    except Exception:
        pass
    return stalls


def _kb_digest(batch, digests_dir, max_hits=3, max_searches=5):
    """Per-batch KB digest: top MWCC_PATTERNS.md records for representative symbols.

    Iterates over candidate symbols until several produced hits, skipping
    zero-recall symbols so the digest is not wasted on dead ends.
    """
    os.makedirs(digests_dir, exist_ok=True)
    out = digests_dir / f"{batch['batch_id']}.md"
    lines = [f"# KB digest for {batch['batch_id']} (from docs/MWCC_PATTERNS.md / attempts)\n"]
    symbols = [td["function"] for td in batch["target_details"] if td.get("function")]
    hit_symbols = 0
    for sym in symbols:
        if hit_symbols >= max_hits:
            break
        if max_searches <= 0:
            break
        max_searches -= 1
        try:
            r = subprocess.run(
                [str(VENV_PY), str(ROOT / "tools" / "mwcc_kb.py"), "search", sym,
                 "--kind", "reference", "--json"],
                capture_output=True, text=True, timeout=120, cwd=str(ROOT),
            )
            if r.returncode != 0:
                continue
            d = json.loads(r.stdout)
            results = d.get("results", [])[:3]
            if not results:
                continue
            hit_symbols += 1
            lines.append(f"\n## {sym} ({d.get('count', 0)} results)")
            for hit in results:
                snip = (hit.get("snippet") or "").replace("\n", " ")[:140]
                lines.append(f"- {hit.get('id')} | {hit.get('title')}: {snip}")
        except Exception as e:
            print(f"warning: KB digest search for {sym} failed: {e}", file=sys.stderr)
    if len(lines) == 1:
        lines.append("(no KB records found for this batch)")
    path = out
    try:
        path.write_text("\n".join(lines))
    except Exception:
        return None
    return str(path)


def plan(section, *, batch_size=BATCH_SIZE, tu_threshold=TU_THRESHOLD,
         hard_batch_size=HARD_BATCH_SIZE, thinking=None, hard_model=None,
         kb_digest=True, targets_path=None, out_path=None, digests_dir=None,
         dry_run=False):
    """Build a batch plan for one section. Returns the plan dict."""
    d = json.load(open(targets_path or TARGETS))
    targets = d["targets"]
    sec = [t for t in targets if section_matches(t.get("unit"), section)]
    unmatched = [t for t in sec if t.get("workflow_status") != "ACCEPTED"]
    unmatched.sort(key=lambda t: t.get("address") or "")

    by_status = Counter(t.get("workflow_status") for t in unmatched)
    leaves = [t for t in unmatched if is_leaf(t)]
    print(f"section={section}: {len(sec)} targets, {len(unmatched)} unmatched "
          f"({dict(by_status)}), {len(leaves)} leaf")

    ready_ids = _ready_ids(targets_path or TARGETS)
    if ready_ids is None:
        ready_ids = _static_ready_ids(targets)
    batchable = [t for t in unmatched if t["id"] in ready_ids]
    deferred = [t for t in unmatched if t["id"] not in ready_ids]
    print(f"ready frontier: {len(batchable)} batchable, {len(deferred)} deferred "
          f"(callee-blocked / not buildable)")

    # deferred classification for the report
    accepted = {t["id"] for t in targets if t.get("workflow_status") == "ACCEPTED"}
    defer_reasons = Counter()
    for t in deferred:
        if t.get("has_indirect_calls") or t.get("unresolved_called_functions"):
            defer_reasons["indirect_or_unresolved_calls"] += 1
        elif t.get("called_functions"):
            defer_reasons["unaccepted_callees"] += 1
        else:
            defer_reasons["not_buildable_or_other"] += 1

    stalls = _stall_map(ROOT / "docs" / "evidence" / "decomp" / "attempts.jsonl")

    by_unit = defaultdict(list)
    for t in batchable:
        by_unit[t["unit"]].append(t)

    batches = []
    bid = 0
    secslug = slug(section)

    def make_batch(tlist, label):
        nonlocal bid
        bid += 1
        tlist = sorted(tlist, key=lambda t: (0 if is_leaf(t) else 1, _size(t), t.get("id") or ""))
        units = sorted({t["unit"] for t in tlist})
        leaf_count = sum(1 for t in tlist if is_leaf(t))
        likely_blocked = 0
        for t in tlist:
            cals = (t.get("called_functions") or []) + (t.get("unresolved_called_functions") or [])
            if any(c not in accepted for c in cals):
                likely_blocked += 1
        tier = thinking or _thinking_for(tlist)
        details = []
        for t in tlist:
            td = {
                "id": t["id"],
                "unit": t["unit"],
                "function": t.get("function") or t.get("symbol"),
                "status": t.get("status"),
                "match_percent": t.get("instruction_match"),
                "size": t.get("size"),
                "leaf": is_leaf(t),
            }
            st = stalls.get(t["id"])
            if st:
                td["stall"] = st
            details.append(td)
        b = {
            "batch_id": f"{secslug}-{bid:02d}",
            "label": label,
            "targets": [t["id"] for t in tlist],
            "target_details": details,
            "units": units,
            "leaf_count": leaf_count,
            "likely_blocked": likely_blocked,
            "total": len(tlist),
            "thinking": tier,
            "model": hard_model if (tier == "max" and hard_model) else None,
        }
        if kb_digest and digests_dir:
            b["digest_path"] = _kb_digest(b, digests_dir)
        batches.append(b)

    # 1) units with >= threshold unmatched targets -> own batch
    big = {u: ts for u, ts in by_unit.items() if len(ts) >= tu_threshold}
    for u in sorted(big, key=lambda u: -len(big[u])):
        make_batch(big[u], u)

    # 2) pool the rest — whole units only (never straddle batches); hard pools
    #    flush earlier than easy ones so a wall hit wastes fewer tokens
    pool = {u: ts for u, ts in by_unit.items() if len(ts) < tu_threshold}
    pool_order = sorted(
        pool,
        key=lambda u: (-sum(1 for t in pool[u] if is_leaf(t)), -len(pool[u])),
    )
    cur = []
    for u in pool_order:
        cur.extend(pool[u])
        cap = hard_batch_size if _thinking_for(cur) == "max" else batch_size
        if len(cur) >= cap:
            make_batch(cur, "mixed")
            cur = []
    if cur:
        make_batch(cur, "mixed")

    # launch order: easy/leaf-rich batches first so callee frontiers populate early
    order = sorted(batches, key=lambda b: (
        0 if b["thinking"] == "medium" else (1 if b["thinking"] == "high" else 2),
        -b["leaf_count"] / max(1, b["total"]),
    ))
    for i, b in enumerate(order):
        b["launch_rank"] = i

    plan = {
        "section": section,
        "generated_at": now_iso(),
        "total_targets": len(unmatched),
        "ready_targets": len(batchable),
        "deferred_targets": len(deferred),
        "defer_reasons": dict(defer_reasons),
        "total_batches": len(batches),
        "leaf_targets": len(leaves),
        "batches": batches,
    }

    if not dry_run and out_path:
        save_json(out_path, plan)
        print(f"wrote {out_path}: {len(batches)} batches, {len(batchable)} ready targets "
              f"({len(deferred)} deferred)")
    for b in batches:
        print(f"  {b['batch_id']}: {b['total']:3d} targets ({b['leaf_count']:2d} leaf) "
              f"thinking={b['thinking']}" + (f" model={b['model']}" if b.get("model") else "")
              + f" [{'/'.join(u.split('/')[-1] for u in b['units'][:2])}"
              + (f", ... {len(b['units'])} units" if len(b["units"]) > 2 else "") + "]")
    print(f"deferred reasons: {dict(defer_reasons)}")
    return plan

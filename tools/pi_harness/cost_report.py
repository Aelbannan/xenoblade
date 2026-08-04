#!/usr/bin/env python3
"""Read-only cost report over the pi-harness ledger.

Standalone stdlib script (argparse/json/collections only). Consumes
build/pi-harness/ledger.jsonl (one JSON object per line, appended by
tools/pi_harness) and tools/coop/targets.json, and prints a markdown cost
report to stdout (or a machine-readable JSON object with --json).

Read-only: never writes to the ledger, targets.json, or any repo file.
Mirrors tools/pi_harness/src/ledger.ts readLedger() tolerance: skips blank
and malformed lines, tolerates a partial final line.

Prices are USD per 1M tokens. CLI flags are the defaults; if
<repo-root>/pi-harness.json has a `costModel` block with *nonzero* values
({inputPerM, outputPerM, cacheReadPerM, cacheWritePerM}), those override the
CLI defaults per key (0 in costModel = "not priced", keeps CLI default).

Usage:
  .venv/bin/python3 tools/pi_harness/cost_report.py [--repo-root .] \
      [--ledger build/pi-harness/ledger.jsonl] [--targets tools/coop/targets.json] \
      [--input-price 0.15] [--output-price 0.60] \
      [--cache-read-price 0.075] [--cache-write-price 0.15] [--json] [--limit N]
"""

from __future__ import annotations

import argparse
import datetime
import json
import os
import sys
from collections import Counter, defaultdict

# Per-1M-token price keys (types.ts CostModel) <-> token field names.
PRICE_KEYS = ["inputPerM", "outputPerM", "cacheReadPerM", "cacheWritePerM"]
TOKEN_KEYS = ["input", "output", "cacheRead", "cacheWrite"]
TOKEN_TO_PRICE = {
    "input": "inputPerM",
    "output": "outputPerM",
    "cacheRead": "cacheReadPerM",
    "cacheWrite": "cacheWritePerM",
}

# Events that represent a failed / abandoned outcome (per-TU failure mix).
FAILURE_EVENTS = {
    "batch-session-exhausted",
    "batch-error",
    "batch-rejected",
    "target-skipped",
    "tu-incomplete",
    "tu-final-failed",
    "tu-error",
    "rebatch-skipped",
}

# Bankable near-miss statuses in targets.json (harness produced, not accepted).
NEAR_MISS_STATUSES = {"COMPILES", "CODE_MATCH", "HIGH_MATCH", "STRUCTURAL"}

# Event names that carry a per-target result list / targetId (acceptance).
RESULT_EVENTS = {"batch-accept", "batch-cycle", "batch-rejected"}


def resolve_path(repo_root: str, path: str) -> str:
    """Mirror ledger.ts: relative paths resolve against repo-root."""
    if os.path.isabs(path):
        return path
    return os.path.join(repo_root, path)


def read_ledger(path: str) -> list[dict]:
    """Mirror readLedger(): skip blank + malformed lines, tolerate no final newline."""
    if not os.path.exists(path):
        return []
    entries: list[dict] = []
    try:
        with open(path, "r", encoding="utf-8") as fh:
            data = fh.read()
    except OSError as err:
        print(f"WARNING: cannot read ledger {path}: {err}", file=sys.stderr)
        return []
    for line in data.split("\n"):
        trimmed = line.strip()
        if not trimmed:
            continue
        try:
            parsed = json.loads(trimmed)
        except json.JSONDecodeError:
            continue  # skip malformed lines (e.g. crash mid-append)
        if isinstance(parsed, dict):
            entries.append(parsed)
    return entries


def load_targets(path: str) -> tuple[list[dict], dict]:
    """Load targets.json; returns (targets, id->target index). Empty on failure."""
    if not os.path.exists(path):
        return [], {}
    try:
        with open(path, "r", encoding="utf-8") as fh:
            data = json.load(fh)
    except (OSError, json.JSONDecodeError) as err:
        print(f"WARNING: cannot read targets {path}: {err}", file=sys.stderr)
        return [], {}
    targets = data.get("targets", []) if isinstance(data, dict) else []
    index = {t.get("id"): t for t in targets if isinstance(t, dict) and t.get("id")}
    return targets, index


def load_prices(args: argparse.Namespace, repo_root: str) -> tuple[dict, str]:
    """CLI defaults, overridden per-key by nonzero costModel in pi-harness.json."""
    prices = {
        "inputPerM": args.input_price,
        "outputPerM": args.output_price,
        "cacheReadPerM": args.cache_read_price,
        "cacheWritePerM": args.cache_write_price,
    }
    source = "cli"
    cfg_path = os.path.join(repo_root, "pi-harness.json")
    if os.path.exists(cfg_path):
        try:
            with open(cfg_path, "r", encoding="utf-8") as fh:
                cfg = json.load(fh)
        except (OSError, json.JSONDecodeError):
            cfg = None
        if isinstance(cfg, dict) and isinstance(cfg.get("costModel"), dict):
            cm = cfg["costModel"]
            overridden = False
            for key in PRICE_KEYS:
                v = cm.get(key)
                if isinstance(v, (int, float)) and not isinstance(v, bool) and v > 0:
                    prices[key] = float(v)
                    overridden = True
            if overridden:
                source = "config"
    return prices, source


def aggregate(entries: list[dict], target_index: dict) -> dict:
    """Collapse the ledger into per-TU + global aggregates."""
    per_tu: dict[str, dict] = defaultdict(
        lambda: {
            "tokens": Counter(),
            "sessions": 0,
            "unknownUsageSessions": 0,
            "unknownTokenFields": Counter(),
            "accepted": 0,
            "events": Counter(),
        }
    )
    event_hist = Counter()
    touched_results: list[tuple[str, str]] = []  # (targetId, ledgerStatus)
    unknown_fields_global = Counter()
    unknown_sessions_global = 0

    for entry in entries:
        tu = entry.get("tu")
        if not tu:
            tu = "(unknown)"
        event = entry.get("event", "?")
        detail = entry.get("detail")
        if not isinstance(detail, dict):
            detail = {}
        agg = per_tu[tu]
        agg["events"][event] += 1
        event_hist[event] += 1

        if event == "session-usage":
            agg["sessions"] += 1
            known = True
            for key in TOKEN_KEYS:
                val = detail.get(key)
                if isinstance(val, (int, float)) and not isinstance(val, bool):
                    agg["tokens"][key] += val
                else:
                    # Missing/unknown token field: skip, do NOT assume 0.
                    known = False
                    agg["unknownTokenFields"][key] += 1
                    unknown_fields_global[key] += 1
            if not known:
                agg["unknownUsageSessions"] += 1
                unknown_sessions_global += 1

        if event in ("batch-accept", "batch-cycle"):
            # Accepted count: batch events carry acceptedCount; singleton
            # accepts carry targetId (implicitly 1 accepted target).
            if isinstance(detail.get("acceptedCount"), (int, float)) and not isinstance(
                detail.get("acceptedCount"), bool
            ):
                agg["accepted"] += int(detail["acceptedCount"])
            elif "targetId" in detail:
                agg["accepted"] += 1
            for res in detail.get("results") or []:
                if isinstance(res, dict) and res.get("targetId"):
                    touched_results.append((res["targetId"], res.get("status")))
            if "targetId" in detail:
                touched_results.append((detail["targetId"], detail.get("status")))

        if event == "batch-rejected" and detail.get("targetId"):
            touched_results.append((detail["targetId"], detail.get("status")))

    return {
        "per_tu": per_tu,
        "event_hist": event_hist,
        "touched_results": touched_results,
        "unknown_fields_global": unknown_fields_global,
        "unknown_sessions_global": unknown_sessions_global,
        "entries": len(entries),
    }


def cost_of(tokens: Counter, prices: dict) -> float:
    total = 0.0
    for tok_key, price_key in TOKEN_TO_PRICE.items():
        total += tokens.get(tok_key, 0) / 1_000_000.0 * prices[price_key]
    return total


def fmt_money(x: float | None, priced: bool) -> str:
    if not priced or x is None:
        return "n/a"
    return f"${x:,.2f}"


def fmt_tokens(x: float) -> str:
    return f"{x:,.0f}"


def build_report(args: argparse.Namespace) -> dict:
    repo_root = args.repo_root
    ledger_path = resolve_path(repo_root, args.ledger)
    targets_path = resolve_path(repo_root, args.targets)

    entries = read_ledger(ledger_path)
    targets, target_index = load_targets(targets_path)
    prices, price_source = load_prices(args, repo_root)
    priced = any(v > 0 for v in prices.values())

    agg = aggregate(entries, target_index)
    per_tu = agg["per_tu"]

    # ---- Overall token + cost totals -------------------------------------
    overall_tokens = Counter()
    for a in per_tu.values():
        overall_tokens.update(a["tokens"])
    total_accepted = sum(a["accepted"] for a in per_tu.values())
    total_cost = cost_of(overall_tokens, prices)
    usd_per_accept = total_cost / total_accepted if total_accepted else None
    tokens_per_accept = {
        "input": overall_tokens["input"] / total_accepted if total_accepted else None,
        "cacheRead": overall_tokens["cacheRead"] / total_accepted if total_accepted else None,
        "total": sum(overall_tokens.values()) / total_accepted if total_accepted else None,
    }

    # ---- Per-TU rows ------------------------------------------------------
    rows = []
    for tu, a in per_tu.items():
        cost = cost_of(a["tokens"], prices)
        rows.append(
            {
                "tu": tu,
                "accepted": a["accepted"],
                "sessions": a["sessions"],
                "tokens": dict(a["tokens"]),
                "costUsd": cost,
                "usdPerAccept": cost / a["accepted"] if a["accepted"] else None,
                "failureMix": {ev: n for ev, n in a["events"].items() if ev in FAILURE_EVENTS},
                "unknownUsageSessions": a["unknownUsageSessions"],
            }
        )
    rows.sort(key=lambda r: (r["costUsd"], r["accepted"]), reverse=True)

    # ---- Global failure-mix histogram -------------------------------------
    event_hist = dict(sorted(agg["event_hist"].items(), key=lambda kv: -kv[1]))

    # ---- Current targets.json status of ledger-touched targets ------------
    ledger_status_current = Counter()
    ledger_status_reported = Counter()
    not_in_targets = 0
    for tid, reported in agg["touched_results"]:
        ledger_status_reported[reported or "(none)"] += 1
        t = target_index.get(tid)
        if t is None:
            not_in_targets += 1
            ledger_status_current["NOT_IN_TARGETS"] += 1
        else:
            ledger_status_current[t.get("status") or "(none)"] += 1

    # ---- Near-miss pile (targets.json, non-accepted, bankable status) -----
    near_miss_total = 0
    near_miss_per_unit: Counter = Counter()
    harness_units = {e.get("tu") for e in entries if e.get("tu")}
    near_miss_harness_touched = 0
    for t in targets:
        if t.get("workflow_status") == "ACCEPTED":
            continue
        if t.get("status") in NEAR_MISS_STATUSES:
            near_miss_total += 1
            unit = t.get("unit") or "(none)"
            near_miss_per_unit[unit] += 1
            if unit in harness_units:
                near_miss_harness_touched += 1

    notes = []
    if not priced:
        notes.append(
            "Cost is n/a: all prices are 0. Set costModel in pi-harness.json "
            "({inputPerM, outputPerM, cacheReadPerM, cacheWritePerM}) or pass "
            "--*-price flags."
        )
    if agg["unknown_sessions_global"]:
        notes.append(
            f"{agg['unknown_sessions_global']} session-usage event(s) are missing token "
            f"fields {dict(agg['unknown_fields_global'])}; the missing fields are excluded "
            "from totals/cost (unknown, not assumed 0 — a 0 would read as 'free'). "
            "Present fields of those sessions still count."
        )
    if not entries:
        notes.append(f"Ledger {ledger_path} is empty or missing; report is all zeros.")
    if not targets:
        notes.append(f"Targets file {targets_path} missing or unreadable; near-miss and "
                     "target-status sections are n/a.")
    if not_in_targets:
        notes.append(
            f"{not_in_targets} ledger result targetId(s) not found in targets.json "
            "(reloc/synthetic ids or stale ledger); counted under NOT_IN_TARGETS."
        )
    notes.append(
        f"attempts = number of session-usage events (one session per batch or "
        f"singleton attempt); accepted = sum of batch-accept/batch-cycle acceptedCount "
        f"(singleton accepts count 1 each)."
    )
    notes.append(
        "near-miss = targets.json targets with workflow_status != ACCEPTED and status in "
        f"{sorted(NEAR_MISS_STATUSES)}; the harness-touched subset is marked separately."
    )

    return {
        "generatedAt": None,  # filled by caller (ISO8601)
        "ledger": ledger_path,
        "targets": targets_path,
        "prices": {**prices, "source": price_source},
        "priced": priced,
        "overall": {
            "sessions": sum(a["sessions"] for a in per_tu.values()),
            "tus": len(per_tu),
            "ledgerEvents": agg["entries"],
            "accepted": total_accepted,
            "tokens": dict(overall_tokens),
            "unknownUsageSessions": agg["unknown_sessions_global"],
            "unknownTokenFields": dict(agg["unknown_fields_global"]),
            "costUsd": total_cost,
            "usdPerAccept": usd_per_accept,
            "tokensPerAccept": tokens_per_accept,
        },
        "perTu": rows,
        "failureMix": {
            "events": event_hist,
            "targetStatusCurrent": dict(ledger_status_current),
            "targetStatusReported": dict(ledger_status_reported),
        },
        "nearMiss": {
            "count": near_miss_total,
            "harnessTouched": near_miss_harness_touched,
            "perUnit": dict(sorted(near_miss_per_unit.items(), key=lambda kv: -kv[1])),
        },
        "notes": notes,
    }


def emit_markdown(report: dict, limit: int) -> str:
    out = []
    p = report["prices"]
    o = report["overall"]
    nm = report["nearMiss"]

    out.append("# pi-harness cost report")
    out.append("")
    out.append(
        f"Ledger: `{report['ledger']}` — {o['ledgerEvents']:,} events, "
        f"{o['tus']} TUs, {o['sessions']:,} sessions"
    )
    if report["priced"]:
        out.append(
            f"Prices (USD/1M tokens, source: {p['source']}): "
            f"in ${p['inputPerM']:.4g}, out ${p['outputPerM']:.4g}, "
            f"cacheRead ${p['cacheReadPerM']:.4g}, cacheWrite ${p['cacheWritePerM']:.4g}"
        )
    else:
        out.append("Prices: **not priced** (all 0) — costs reported as n/a.")
    out.append("")

    # ---- 1. Overall ------------------------------------------------------
    out.append("## 1. Overall")
    out.append("")
    tk = o["tokens"]
    out.append(
        f"| metric | value |\n|---|---|\n"
        f"| fresh input tokens | {fmt_tokens(tk.get('input', 0))} |\n"
        f"| output tokens | {fmt_tokens(tk.get('output', 0))} |\n"
        f"| cacheRead tokens | {fmt_tokens(tk.get('cacheRead', 0))} |\n"
        f"| cacheWrite tokens | {fmt_tokens(tk.get('cacheWrite', 0))} |\n"
        f"| est. cost | {fmt_money(o['costUsd'], report['priced'])} |\n"
        f"| accepted matches | {o['accepted']} |\n"
        f"| $ per accepted match | {fmt_money(o['usdPerAccept'], report['priced'])} |\n"
    )
    tpa = o["tokensPerAccept"]
    if tpa["input"] is not None:
        out.append(
            f"| tokens per match (fresh input) | {fmt_tokens(tpa['input'])} |\n"
            f"| tokens per match (cacheRead) | {fmt_tokens(tpa['cacheRead'])} |\n"
        )
    if o["unknownUsageSessions"]:
        out.append(
            f"\n*{o['unknownUsageSessions']} session(s) with unknown token fields "
            f"({o['unknownTokenFields']}) excluded from totals.*"
        )
    out.append("")

    # ---- 2. Per-TU table --------------------------------------------------
    out.append("## 2. Per-TU (top by spend)")
    out.append("")
    rows = report["perTu"]
    if limit and limit > 0:
        rows = rows[:limit]
    if rows:
        out.append(
            "| TU | accepts | attempts | $ | $/accept | in-tok | cr-tok | failures |\n"
            "|---|---|---|---|---|---|---|---|"
        )
        for r in rows:
            fm = " ".join(f"{ev}:{n}" for ev, n in r["failureMix"].items()) or "—"
            out.append(
                f"| {r['tu']} | {r['accepted']} | {r['sessions']} | "
                f"{fmt_money(r['costUsd'], report['priced'])} | "
                f"{fmt_money(r['usdPerAccept'], report['priced'])} | "
                f"{fmt_tokens(r['tokens'].get('input', 0))} | "
                f"{fmt_tokens(r['tokens'].get('cacheRead', 0))} | {fm} |"
            )
        if limit and limit > 0 and len(report["perTu"]) > limit:
            out.append(f"\n*Showing top {limit} of {len(report['perTu'])} TUs.*")
    else:
        out.append("_No TUs in ledger._")
    out.append("")

    # ---- 3. Failure-mix histogram -----------------------------------------
    out.append("## 3. Failure-mix histogram")
    out.append("")
    out.append("### Ledger events by type")
    out.append("")
    if report["failureMix"]["events"]:
        out.append("| event | count |\n|---|---|")
        for ev, n in report["failureMix"]["events"].items():
            out.append(f"| {ev} | {n} |")
    out.append("")
    out.append("### Current targets.json status of ledger-touched targets")
    out.append("")
    if report["failureMix"]["targetStatusCurrent"]:
        out.append("| status (targets.json) | results |\n|---|---|")
        for st, n in sorted(
            report["failureMix"]["targetStatusCurrent"].items(), key=lambda kv: -kv[1]
        ):
            out.append(f"| {st} | {n} |")
    out.append("")

    # ---- 4. Near-miss pile -------------------------------------------------
    out.append("## 4. Near-miss pile (targets.json, non-accepted)")
    out.append("")
    out.append(
        f"**{nm['count']} near-miss targets overall** "
        f"({nm['harnessTouched']} in harness-touched units) with status in "
        f"{sorted(NEAR_MISS_STATUSES)}."
    )
    out.append("")
    per_unit = nm["perUnit"]
    if per_unit:
        if limit and limit > 0:
            per_unit = dict(list(per_unit.items())[:limit])
        out.append("| unit | near-miss |\n|---|---|")
        for u, n in per_unit.items():
            out.append(f"| {u} | {n} |")
        if limit and limit > 0 and len(nm["perUnit"]) > limit:
            out.append(f"\n*Showing top {limit} of {len(nm['perUnit'])} units.*")
    out.append("")

    # ---- Notes -------------------------------------------------------------
    out.append("## Notes")
    out.append("")
    for note in report["notes"]:
        out.append(f"- {note}")
    out.append("")
    return "\n".join(out)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Read-only cost report over the pi-harness ledger."
    )
    parser.add_argument("--repo-root", default=".", help="Repo root (default: .)")
    parser.add_argument(
        "--ledger", default="build/pi-harness/ledger.jsonl",
        help="Ledger path, absolute or relative to repo-root "
             "(default: build/pi-harness/ledger.jsonl)",
    )
    parser.add_argument(
        "--targets", default="tools/coop/targets.json",
        help="Targets path, absolute or relative to repo-root "
             "(default: tools/coop/targets.json)",
    )
    parser.add_argument("--input-price", type=float, default=0.15,
                        help="USD per 1M fresh-input tokens (default: 0.15)")
    parser.add_argument("--output-price", type=float, default=0.60,
                        help="USD per 1M output tokens (default: 0.60)")
    parser.add_argument("--cache-read-price", type=float, default=0.075,
                        help="USD per 1M cacheRead tokens (default: 0.075)")
    parser.add_argument("--cache-write-price", type=float, default=0.15,
                        help="USD per 1M cacheWrite tokens (default: 0.15)")
    parser.add_argument("--json", action="store_true",
                        help="Emit a machine-readable JSON object instead of markdown")
    parser.add_argument("--limit", type=int, default=0,
                        help="Cap rows in the per-TU and near-miss tables (0 = no cap)")
    args = parser.parse_args(argv)

    report = build_report(args)
    report["generatedAt"] = datetime.datetime.now(datetime.timezone.utc).isoformat()

    if args.json:
        print(json.dumps(report, indent=2, sort_keys=False))
    else:
        print(emit_markdown(report, args.limit))
    return 0


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""
Read-only web viewer for the pi-harness.

Phase 3 viewer: a small, dependency-free HTTP server that exposes pi-harness
state (ledger event stream, targets registry, near-miss draft bank, session
transcripts) as JSON. Read-only — never writes to the ledger, targets.json,
nearmiss/, or anything else.

Usage:
    .venv/bin/python3 tools/pi_harness/viewer/serve.py [--port 8766]

    --port PORT       listen port (default 8766; atlas serve owns 8765)
    --repo-root DIR   repo root; auto-detected by walking up from CWD
                      (mirrors tools/pi_harness/src/index.ts findRepoRoot)
    --ledger PATH     ledger path, relative to repo root
                      (default build/pi-harness/ledger.jsonl)
    --targets PATH    targets registry path, relative to repo root
                      (default tools/coop/targets.json)

Endpoints:
    GET /                        UI (static/index.html)
    GET /app.js                  UI script
    GET /api/overview            live TU progress + totals + $/match ticker
    GET /api/tu/<unit>           per-target rows for a unit
    GET /api/cost                cost report (cost_report.py --json, TTL-cached
                                ~30s; falls back to a lightweight viewer
                                aggregation if the report can't run)
    GET /api/ledger?event=&limit=  tail of the ledger (newest last)
    GET /api/nearmiss/<targetId> banked near-miss draft info
    GET /api/transcripts?tu=     session transcript list for a TU
    GET /transcripts/<path...>   served transcript file (strict allowlist)

Python 3 stdlib only. Read-only server.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import threading
import time
import urllib.parse
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

ACCEPTED_STATUSES = {"FULL_MATCH", "EQUIVALENT_MATCH", "ACCEPTED"}
RESULT_EVENTS = ("batch-cycle", "batch-accept", "batch-rejected")
TERMINAL_EVENTS = ("tu-incomplete", "tu-final-failed")

COST_KEYS = ("inputPerM", "outputPerM", "cacheReadPerM", "cacheWritePerM")
TOKEN_KEYS = ("input", "output", "cacheRead", "cacheWrite")

SESSION_REL = "build/pi-harness/sessions"
NEARMISS_REL = "build/pi-harness/nearmiss"
CONFIG_REL = "pi-harness.json"

# Cost report: the standalone cost_report.py is the single source of truth
# for cost math; the viewer runs it via subprocess and TTL-caches the JSON.
COST_REPORT_REL = "tools/pi_harness/cost_report.py"
COST_TTL_SECS = 30.0
COST_TIMEOUT_SECS = 30
COST_PYTHON_CANDIDATES = (".venv/bin/python3", ".venv/bin/python")

# Ledger detail fields that can hold multi-KB payloads (model error dumps,
# lint rejections, build output). Truncated in API responses so the live
# log stays responsive.
TRUNCATED_DETAIL_FIELDS = (
    "error", "lastRejection", "recoveryOutput", "buildOutput", "finalTextPreview",
)
TRUNCATED_FIELD_LEN = 1200
TRUNCATED_SUFFIX = " … [truncated]"

TRANSCRIPT_EXTENSIONS = (".transcript.md", ".md", ".jsonl")

# ---------------------------------------------------------------------------
# File state (cached reads)
# ---------------------------------------------------------------------------


def _mtime_size(path: str):
    """Return (mtime_ns, size) for change detection, or None if absent."""
    try:
        st = os.stat(path)
        return (st.st_mtime_ns, st.st_size)
    except OSError:
        return None


class ViewerState:
    """Read-only accessors over harness state files, with mtime-keyed caches."""

    def __init__(self, repo_root: str, ledger_rel: str, targets_rel: str) -> None:
        self.repo_root = os.path.abspath(repo_root)
        self.ledger_path = os.path.join(self.repo_root, ledger_rel)
        self.targets_path = os.path.join(self.repo_root, targets_rel)
        self.session_dir = os.path.join(self.repo_root, SESSION_REL)
        self.nearmiss_dir = os.path.join(self.repo_root, NEARMISS_REL)
        self.static_dir = os.path.join(
            os.path.dirname(os.path.abspath(__file__)), "static"
        )
        self._lock = threading.Lock()
        self._ledger = None      # (key, entries)
        self._targets = None     # (key, data | None)
        self._nearmiss = None    # (key, {tid: [row, ...]})
        self._config = None      # (key, cfg)
        self._transcripts = None  # (monotonic_ts, {topdir: [items]})
        self._cost_report = None  # (expiry_ts, payload) TTL cache

    # -- low-level reads ----------------------------------------------------

    def read_ledger(self):
        """All ledger entries in file order. Malformed and partial-final
        lines are skipped; a missing file yields []."""
        key = _mtime_size(self.ledger_path)
        if key is None:
            self._ledger = None
            return []
        with self._lock:
            cached = self._ledger
        if cached and cached[0] == key:
            return cached[1]
        entries = []
        try:
            with open(self.ledger_path, "r", encoding="utf-8", errors="replace") as fh:
                for line in fh:
                    line = line.strip()
                    if not line:
                        continue
                    try:
                        entries.append(json.loads(line))
                    except ValueError:
                        pass  # malformed line or partial final line (no \n)
        except OSError:
            return []
        with self._lock:
            self._ledger = (key, entries)
        return entries

    def read_targets(self):
        """Parsed targets.json, cached by mtime. If a mid-write parse fails,
        the last good parse is served instead of a partial one. Returns None
        when absent or never parsed successfully."""
        key = _mtime_size(self.targets_path)
        if key is None:
            self._targets = None
            return None
        with self._lock:
            cached = self._targets
        if cached and cached[0] == key:
            return cached[1]
        data = None
        try:
            with open(self.targets_path, "r", encoding="utf-8", errors="replace") as fh:
                data = json.load(fh)
        except (OSError, ValueError):
            # Truncated/non-atomic write in progress: fall back to last good.
            with self._lock:
                if self._targets is not None:
                    return self._targets[1]
            return None
        with self._lock:
            self._targets = (key, data)
        return data

    def read_nearmiss_index(self):
        """Near-miss bank index, {tid: [rows...]} keyed by mtime. A missing
        dir/index is an empty dict, never an error."""
        path = os.path.join(self.nearmiss_dir, "index.jsonl")
        key = _mtime_size(path)
        if key is None:
            self._nearmiss = None
            return {}
        with self._lock:
            cached = self._nearmiss
        if cached and cached[0] == key:
            return cached[1]
        by_tid = {}
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as fh:
                for line in fh:
                    line = line.strip()
                    if not line:
                        continue
                    try:
                        row = json.loads(line)
                    except ValueError:
                        continue
                    tid = row.get("tid")
                    if tid:
                        by_tid.setdefault(tid, []).append(row)
        except OSError:
            by_tid = {}
        with self._lock:
            self._nearmiss = (key, by_tid)
        return by_tid

    def read_config(self):
        """pi-harness.json merged view; missing/invalid file yields {}."""
        path = os.path.join(self.repo_root, CONFIG_REL)
        key = _mtime_size(path)
        if key is None:
            self._config = None
            return {}
        with self._lock:
            cached = self._config
        if cached and cached[0] == key:
            return cached[1]
        cfg = {}
        if key is not None:
            try:
                with open(path, "r", encoding="utf-8", errors="replace") as fh:
                    cfg = json.load(fh)
            except (OSError, ValueError):
                cfg = {}
        with self._lock:
            self._config = (key, cfg)
        return cfg

    # -- helpers ------------------------------------------------------------

    def cost_model(self):
        cm = self.read_config().get("costModel") or {}
        out = {}
        for k in COST_KEYS:
            v = cm.get(k)
            try:
                out[k] = float(v) if v is not None else 0.0
            except (TypeError, ValueError):
                out[k] = 0.0
        return out

    def priced(self):
        return any(v > 0 for v in self.cost_model().values())

    @staticmethod
    def tokens(detail):
        out = []
        for k in TOKEN_KEYS:
            v = detail.get(k)
            try:
                out.append(int(v) if v is not None else 0)
            except (TypeError, ValueError):
                out.append(0)
        return tuple(out)

    def est_cost(self, detail, cost_model):
        inp, out, cr, cw = self.tokens(detail)
        return (
            inp / 1e6 * cost_model["inputPerM"]
            + out / 1e6 * cost_model["outputPerM"]
            + cr / 1e6 * cost_model["cacheReadPerM"]
            + cw / 1e6 * cost_model["cacheWritePerM"]
        )

    # -- cost report (delegated to cost_report.py) --------------------------

    def _python_bin(self):
        """Resolve the venv python (prefer .venv/bin/python3, then python).
        Falls back to `python3` on PATH when the venv is absent."""
        for rel in COST_PYTHON_CANDIDATES:
            cand = os.path.join(self.repo_root, rel)
            if os.path.isfile(cand):
                return cand
        return "python3"

    def _run_cost_report(self):
        """Run cost_report.py --json once (30s timeout, read-only). Returns
        the parsed report dict, or None on any failure (missing script,
        nonzero exit, timeout, malformed JSON)."""
        script = os.path.join(self.repo_root, COST_REPORT_REL)
        if not os.path.isfile(script):
            return None
        try:
            proc = subprocess.run(
                [self._python_bin(), COST_REPORT_REL, "--json",
                 "--repo-root", self.repo_root,
                 "--ledger", self.ledger_path,
                 "--targets", self.targets_path],
                cwd=self.repo_root,
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=COST_TIMEOUT_SECS,
            )
        except (OSError, subprocess.SubprocessError) as exc:
            print(f"[viewer] cost_report.py failed to run: {exc}", file=sys.stderr)
            return None
        if proc.returncode != 0:
            print(
                f"[viewer] cost_report.py exited {proc.returncode}: "
                f"{(proc.stderr or '')[-400:]}",
                file=sys.stderr,
            )
            return None
        try:
            report = json.loads(proc.stdout)
        except ValueError as exc:
            print(f"[viewer] cost_report.py emitted non-JSON output: {exc}", file=sys.stderr)
            return None
        return report if isinstance(report, dict) else None

    def cost_report_json(self):
        """TTL-cached (30s) cost report payload. The report is O(ledger) and
        the viewer polls every 2s, so the subprocess runs at most once per
        TTL window. Returns {source, cachedAt, ...report} or None on failure
        (failures are not cached — a transient error recovers on next poll)."""
        now = time.time()
        with self._lock:
            cached = self._cost_report
        if cached and cached[0] > now:
            return cached[1]
        report = self._run_cost_report()
        if report is None:
            return None
        cached_at = report.get("generatedAt") or time.strftime(
            "%Y-%m-%dT%H:%M:%SZ", time.gmtime()
        )
        payload = {"source": "cost_report.py", "cachedAt": cached_at, **report}
        with self._lock:
            self._cost_report = (now + COST_TTL_SECS, payload)
        return payload

    def api_cost_payload(self):
        """Primary: cost_report.py --json (single source of truth for cost
        math). Fallback: the viewer's own lightweight aggregation if the
        report is missing or fails."""
        report = self.cost_report_json()
        if report is not None:
            return report
        return {"source": "viewer-lite", "cachedAt": None, **self.api_cost()}

    def transcript_map(self):
        """All transcripts grouped by top-level session dir, TTL-cached."""
        now = time.monotonic()
        with self._lock:
            cached = self._transcripts
        if cached and now - cached[0] < 3.0:
            return cached[1]
        mapping = {}
        base = self.session_dir
        if os.path.isdir(base):
            try:
                for entry in sorted(os.listdir(base)):
                    full = os.path.join(base, entry)
                    if not os.path.isdir(full):
                        continue
                    items = []
                    for root, _dirs, files in os.walk(full):
                        for fn in files:
                            if fn.endswith(".transcript.md"):
                                rel = os.path.relpath(os.path.join(root, fn), base)
                                items.append(
                                    {"label": rel, "path": os.path.join(SESSION_REL, rel)}
                                )
                    items.sort(key=lambda i: i["label"], reverse=True)
                    mapping[entry] = items
            except OSError:
                mapping = {}
        with self._lock:
            self._transcripts = (now, mapping)
        return mapping

    def accepted_by_tu(self, agg, targets_data):
        """Distinct accepted targetIds per TU: ledger results ∪ registry status."""
        acc = {}
        for tu, evs in agg["results"].items():
            s = set()
            for e in evs:
                for r in (e.get("detail") or {}).get("results") or []:
                    if r.get("status") in ACCEPTED_STATUSES and r.get("targetId"):
                        s.add(r["targetId"])
            acc[tu] = s
        if isinstance(targets_data, dict):
            for t in targets_data.get("targets") or []:
                if t.get("status") in ACCEPTED_STATUSES and t.get("id") and t.get("unit"):
                    acc.setdefault(t["unit"], set()).add(t["id"])
        return acc

    def aggregate(self):
        """One pass over the ledger into per-TU maps."""
        ledger = self.read_ledger()
        started = {}      # tu -> {targetIds, unmatchedCount}
        results = {}      # tu -> [entry]
        skipped = {}      # tu -> set(targetId)
        exhausted = {}    # tu -> [targetId,...] (event count preserved)
        usage = []        # session-usage entries in order
        last = {}         # tu -> (ts, event)
        for e in ledger:
            ev = e.get("event")
            tu = e.get("tu")
            d = e.get("detail") or {}
            if ev == "tu-started":
                started.setdefault(tu, {
                    "targetIds": list(d.get("targetIds") or []),
                    "unmatchedCount": d.get("unmatchedCount"),
                })
            elif ev in RESULT_EVENTS:
                results.setdefault(tu, []).append(e)
            elif ev == "target-skipped":
                if tu and d.get("targetId"):
                    skipped.setdefault(tu, set()).add(d["targetId"])
            elif ev == "batch-session-exhausted":
                if tu and d.get("targetId"):
                    exhausted.setdefault(tu, []).append(d["targetId"])
            elif ev == "session-usage":
                if tu:
                    usage.append(e)
            ts = e.get("ts")
            if tu and ts:
                prev = last.get(tu)
                if prev is None or ts >= prev[0]:
                    last[tu] = (ts, ev)
        return {
            "started": started,
            "results": results,
            "skipped": skipped,
            "exhausted": exhausted,
            "usage": usage,
            "last": last,
        }

    # -- API payloads -------------------------------------------------------

    def api_overview(self):
        agg = self.aggregate()
        targets = self.read_targets()
        accepted_by_tu = self.accepted_by_tu(agg, targets)
        cost_model = self.cost_model()
        priced = any(v > 0 for v in cost_model.values())

        def sort_key(tu):
            return agg["last"].get(tu, ("", ""))[0]

        tus = []
        for tu in sorted(agg["started"], key=sort_key, reverse=True):
            st = agg["started"][tu]
            total = st["unmatchedCount"]
            if not isinstance(total, int):
                total = len(st["targetIds"])
            accepted = len(accepted_by_tu.get(tu, set()))
            skipped = len(agg["skipped"].get(tu, set()))
            exhausted = len(set(agg["exhausted"].get(tu, [])))
            last_ts, last_event = agg["last"].get(tu, (None, None))
            tus.append({
                "tu": tu,
                "total": total,
                "accepted": accepted,
                "skipped": skipped,
                "exhausted": exhausted,
                "remaining": max(0, (total or 0) - accepted),
                "running": last_event not in TERMINAL_EVENTS,
                "lastEvent": last_event,
                "lastActivity": last_ts,
            })

        totals = {
            "accepted": sum(t["accepted"] for t in tus),
            "skipped": sum(t["skipped"] for t in tus),
            "exhausted": sum(t["exhausted"] for t in tus),
        }
        tokens = 0
        total_cost = 0.0
        for e in agg["usage"]:
            inp, out, cr, cw = self.tokens(e.get("detail") or {})
            tokens += inp + out + cr + cw
            total_cost += self.est_cost(e.get("detail") or {}, cost_model)

        active = []
        for e in agg["usage"][-10:]:
            d = e.get("detail") or {}
            inp, out, cr, cw = self.tokens(d)
            active.append({
                "ts": e.get("ts"),
                "tu": e.get("tu"),
                "label": d.get("label"),
                "input": inp, "output": out,
                "cacheRead": cr, "cacheWrite": cw,
                "tokens": inp + out + cr + cw,
            })

        accepted = totals["accepted"]
        if priced:
            per_match = round(total_cost / accepted, 6) if accepted else 0.0
        else:
            per_match = tokens // accepted if accepted else 0

        return {
            "costModel": cost_model,
            "priced": priced,
            "tus": tus,
            "totals": totals,
            "ticker": {
                "accepted": accepted,
                "tokens": tokens,
                "cost": round(total_cost, 6),
                "priced": priced,
                "perMatch": per_match,
            },
            "activeSessions": active,
            "sessionCount": len(agg["usage"]),
            "liveTuCount": len(tus),
            "runningTuCount": sum(1 for t in tus if t["running"]),
            "generatedAt": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
        }

    def api_tu(self, unit):
        unit = unit.strip("/")
        if not unit or any(p == ".." for p in unit.split("/")):
            return {"error": "bad unit"}
        agg = self.aggregate()
        targets = self.read_targets()

        ledger_status = {}
        attempts = {}
        for tu, evs in agg["results"].items():
            if tu != unit:
                continue
            for e in evs:
                for r in (e.get("detail") or {}).get("results") or []:
                    tid = r.get("targetId")
                    if not tid:
                        continue
                    attempts[tid] = attempts.get(tid, 0) + 1
                    if r.get("status"):
                        ledger_status[tid] = r["status"]
        for tid in agg["exhausted"].get(unit, []):
            attempts[tid] = attempts.get(tid, 0) + 1
        skipped_ids = agg["skipped"].get(unit, set())

        by_id = {}
        if isinstance(targets, dict):
            for t in targets.get("targets") or []:
                if t.get("unit") == unit and t.get("id"):
                    by_id[t["id"]] = t

        # Registry roster first (authoritative status), then ledger-only ids.
        roster_ids = list(by_id.keys())
        for tid in agg["started"].get(unit, {}).get("targetIds", []):
            if tid not in by_id:
                roster_ids.append(tid)
        for tid in ledger_status:
            if tid not in by_id and tid not in roster_ids:
                roster_ids.append(tid)
        for tid in attempts:
            if tid not in by_id and tid not in roster_ids:
                roster_ids.append(tid)
        for tid in skipped_ids:
            if tid not in by_id and tid not in roster_ids:
                roster_ids.append(tid)

        near = self.read_nearmiss_index()
        rows = []
        for tid in roster_ids:
            t = by_id.get(tid, {})
            near_rows = near.get(tid, [])
            best_mc = None
            for r in near_rows:
                mc = r.get("mismatchCount")
                if mc is not None and (best_mc is None or mc < best_mc):
                    best_mc = mc
            size_raw = t.get("size")
            rows.append({
                "id": tid,
                "symbol": t.get("symbol"),
                "function": t.get("function"),
                "source": t.get("source"),
                "status": t.get("status") or ledger_status.get(tid),
                "size": size_raw,
                "sizeBytes": parse_size(size_raw),
                "attempts": attempts.get(tid, 0),
                "bestDivergence": best_mc,
                "nearmissBanked": bool(near_rows),
                "skipped": tid in skipped_ids,
                "workflowStatus": t.get("workflow_status"),
            })

        tr = self.transcript_map()
        transcripts = tr.get(unit.replace("/", "__"), [])
        return {
            "unit": unit,
            "targets": rows,
            "ledgerOnly": len(rows) - len(by_id),
            "latestTranscript": transcripts[0] if transcripts else None,
            "transcriptCount": len(transcripts),
        }

    def api_cost(self):
        agg = self.aggregate()
        cm = self.cost_model()
        priced = any(v > 0 for v in cm.values())

        per_tu = {}
        per_batch = {}
        for e in agg["usage"]:
            tu = e.get("tu")
            d = e.get("detail") or {}
            if not tu:
                continue
            t = per_tu.setdefault(tu, {
                "tu": tu, "sessions": 0, "acceptedIds": set(),
                "input": 0, "output": 0, "cacheRead": 0, "cacheWrite": 0,
            })
            t["sessions"] += 1
            inp, out, cr, cw = self.tokens(d)
            t["input"] += inp
            t["output"] += out
            t["cacheRead"] += cr
            t["cacheWrite"] += cw
            label = d.get("label") or ""
            m = re.match(r"^batch-(\d+)", label)
            bname = "batch-" + m.group(1) if m else (label or "session")
            bkey = (tu, bname)
            b = per_batch.setdefault(bkey, {
                "tu": tu, "batch": bname, "sessions": 0, "acceptedIds": set(),
                "input": 0, "output": 0, "cacheRead": 0, "cacheWrite": 0,
            })
            b["sessions"] += 1
            b["input"] += inp
            b["output"] += out
            b["cacheRead"] += cr
            b["cacheWrite"] += cw

        for tu, evs in agg["results"].items():
            for e in evs:
                d = e.get("detail") or {}
                bi = d.get("batchIndex")
                for r in d.get("results") or []:
                    if r.get("status") not in ACCEPTED_STATUSES or not r.get("targetId"):
                        continue
                    t = per_tu.setdefault(tu, {
                        "tu": tu, "sessions": 0, "acceptedIds": set(),
                        "input": 0, "output": 0, "cacheRead": 0, "cacheWrite": 0,
                    })
                    t["acceptedIds"].add(r["targetId"])
                    if bi is not None:
                        bname = "batch-%d" % bi
                        b = per_batch.setdefault((tu, bname), {
                            "tu": tu, "batch": bname, "sessions": 0,
                            "acceptedIds": set(), "input": 0, "output": 0,
                            "cacheRead": 0, "cacheWrite": 0,
                        })
                        b["acceptedIds"].add(r["targetId"])

        def finalize(row):
            row["accepted"] = len(row.pop("acceptedIds"))
            row["estCost"] = round(
                row["input"] / 1e6 * cm["inputPerM"]
                + row["output"] / 1e6 * cm["outputPerM"]
                + row["cacheRead"] / 1e6 * cm["cacheReadPerM"]
                + row["cacheWrite"] / 1e6 * cm["cacheWritePerM"],
                6,
            )
            row["costPerMatch"] = (
                round(row["estCost"] / row["accepted"], 6) if row["accepted"] else None
            )
            return row

        per_tu_list = [finalize(per_tu[k]) for k in per_tu]
        per_batch_list = [finalize(per_batch[k]) for k in per_batch]
        per_tu_list.sort(key=lambda r: r["estCost"], reverse=True)
        per_batch_list.sort(
            key=lambda r: (r["tu"], natural_batch_key(r["batch"]))
        )

        totals = {
            "sessions": len(agg["usage"]),
            "input": sum(r["input"] for r in per_tu_list),
            "output": sum(r["output"] for r in per_tu_list),
            "cacheRead": sum(r["cacheRead"] for r in per_tu_list),
            "cacheWrite": sum(r["cacheWrite"] for r in per_tu_list),
            "estCost": round(sum(r["estCost"] for r in per_tu_list), 6),
            "accepted": sum(r["accepted"] for r in per_tu_list),
        }

        return {
            "costModel": cm,
            "priced": priced,
            "totals": totals,
            "perTu": per_tu_list,
            "perBatch": per_batch_list,
        }

    def api_ledger(self, query):
        try:
            limit = int((query.get("limit") or ["200"])[0])
        except (TypeError, ValueError):
            limit = 200
        limit = max(1, min(1000, limit))
        ev = (query.get("event") or [None])[0]
        entries = []
        for e in self.read_ledger():
            if ev and e.get("event") != ev:
                continue
            entries.append(trim_ledger_entry(e))
        return {
            "count": len(entries),
            "limit": limit,
            "event": ev,
            "entries": entries[-limit:],
        }

    def api_nearmiss(self, tid):
        rows = self.read_nearmiss_index().get(tid)
        if not rows:
            return {"banked": False}

        def quality(r):
            mc = r.get("mismatchCount")
            sr = r.get("statusRank")
            return (mc if mc is not None else 10 ** 9,
                    sr if sr is not None else 10 ** 9)

        best = min(rows, key=quality)
        return {
            "banked": True,
            "indexRow": best,
            "file": best.get("file"),
            "mismatchCount": best.get("mismatchCount"),
            "statusRank": best.get("statusRank"),
            "candidates": len(rows),
        }

    def api_transcripts(self, unit):
        unit = unit.strip("/")
        if not unit:
            return []
        sanitized = unit.replace("/", "__")
        mapping = self.transcript_map()
        if sanitized in mapping:
            return mapping[sanitized]
        # Fallback: substring match for partially-sanitized names.
        out = []
        for key in mapping:
            if sanitized and sanitized in key:
                out.extend(mapping[key])
        return out

    # -- file serving -------------------------------------------------------

    def serve_static(self, handler, name, content_type):
        path = os.path.join(self.static_dir, name)
        try:
            with open(path, "rb") as fh:
                body = fh.read()
        except OSError:
            handler.send_json({"error": "not found"}, 404)
            return
        handler.send_bytes(body, content_type)

    def serve_transcript(self, handler, rel):
        rel = urllib.parse.unquote(rel)
        if rel.startswith("/"):
            rel = rel[1:]
        # UI links use the repo-relative path (build/pi-harness/sessions/...);
        # strip that prefix so it resolves relative to session_dir.
        if rel.startswith(SESSION_REL + "/"):
            rel = rel[len(SESSION_REL) + 1:]
        parts = rel.split("/")
        if any(p in ("", ".", "..") for p in parts):
            handler.send_json({"error": "forbidden"}, 403)
            return
        base = os.path.realpath(self.session_dir)
        candidate = os.path.realpath(os.path.join(self.session_dir, rel))
        if candidate != base and not candidate.startswith(base + os.sep):
            handler.send_json({"error": "forbidden"}, 403)
            return
        if not candidate.endswith(TRANSCRIPT_EXTENSIONS) or not os.path.isfile(candidate):
            handler.send_json({"error": "not found"}, 404)
            return
        try:
            with open(candidate, "rb") as fh:
                body = fh.read()
        except OSError:
            handler.send_json({"error": "not found"}, 404)
            return
        handler.send_bytes(body, "text/markdown; charset=utf-8")


def parse_size(raw):
    """targets.json sizes are hex strings like '0x29C' (some ints)."""
    if raw is None:
        return None
    if isinstance(raw, bool):
        return None
    if isinstance(raw, (int, float)):
        return int(raw)
    if isinstance(raw, str):
        try:
            return int(raw.strip(), 0)
        except ValueError:
            return None
    return None


def natural_batch_key(name):
    m = re.match(r"^batch-(\d+)$", name)
    if m:
        return (0, int(m.group(1)), name)
    return (1, 0, name)


def trim_ledger_entry(entry):
    """Copy of an entry with the known multi-KB detail fields truncated."""
    d = entry.get("detail")
    if not isinstance(d, dict):
        return entry
    changed = False
    out = dict(d)
    for k in TRUNCATED_DETAIL_FIELDS:
        v = out.get(k)
        if isinstance(v, str) and len(v) > TRUNCATED_FIELD_LEN:
            out[k] = v[:TRUNCATED_FIELD_LEN] + TRUNCATED_SUFFIX
            changed = True
    if not changed:
        return entry
    return {**entry, "detail": out}


# ---------------------------------------------------------------------------
# HTTP handler
# ---------------------------------------------------------------------------


class Handler(BaseHTTPRequestHandler):
    server_version = "pi-harness-viewer/1.0"
    protocol_version = "HTTP/1.1"

    def log_message(self, fmt, *args):
        sys.stderr.write("[%s] %s\n" % (self.log_date_time_string(), fmt % args))

    # -- helpers ------------------------------------------------------------

    def send_json(self, obj, code=200):
        body = json.dumps(obj).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        try:
            self.wfile.write(body)
        except BrokenPipeError:
            pass

    def send_bytes(self, body, content_type, code=200):
        self.send_response(code)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-cache")
        self.end_headers()
        try:
            self.wfile.write(body)
        except BrokenPipeError:
            pass

    # -- routing ------------------------------------------------------------

    def do_GET(self):
        try:
            self._route()
        except (BrokenPipeError, ConnectionResetError):
            pass
        except Exception as exc:  # never crash a poll
            try:
                self.send_json({"error": "internal: %s" % exc}, 500)
            except Exception:
                pass

    def _route(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path
        query = urllib.parse.parse_qs(parsed.query)
        st = self.server.viewer  # type: ignore[attr-defined]

        if path in ("/", "/index.html"):
            return st.serve_static(self, "index.html", "text/html; charset=utf-8")
        if path == "/app.js":
            return st.serve_static(self, "app.js", "application/javascript; charset=utf-8")
        if path == "/api/overview":
            return self.send_json(st.api_overview())
        if path.startswith("/api/tu/"):
            unit = urllib.parse.unquote(path[len("/api/tu/"):])
            res = st.api_tu(unit)
            return self.send_json(res, 400 if "error" in res else 200)
        if path == "/api/cost":
            return self.send_json(st.api_cost_payload())
        if path == "/api/ledger":
            return self.send_json(st.api_ledger(query))
        if path.startswith("/api/nearmiss/"):
            tid = urllib.parse.unquote(path[len("/api/nearmiss/"):])
            return self.send_json(st.api_nearmiss(tid))
        if path == "/api/transcripts":
            unit = (query.get("tu") or [""])[0]
            return self.send_json(st.api_transcripts(unit))
        if path.startswith("/transcripts/"):
            return st.serve_transcript(self, path[len("/transcripts/"):])
        return self.send_json({"error": "not found"}, 404)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------


def find_repo_root(start=None):
    """Walk up from `start` (max 4 parents) until tools/coop/targets.json is
    found — mirrors tools/pi_harness/src/index.ts findRepoRoot."""
    d = os.path.abspath(start or os.getcwd())
    for _ in range(5):
        if os.path.isfile(os.path.join(d, "tools", "coop", "targets.json")):
            return d
        parent = os.path.dirname(d)
        if parent == d:
            break
        d = parent
    return None


def main(argv=None):
    ap = argparse.ArgumentParser(
        description="Read-only pi-harness web viewer (stdlib only)."
    )
    ap.add_argument("--port", type=int, default=8766,
                    help="listen port (default 8766; atlas serve owns 8765)")
    ap.add_argument("--repo-root", default=None,
                    help="repo root (default: auto-detected from CWD)")
    ap.add_argument("--ledger", default="build/pi-harness/ledger.jsonl",
                    help="ledger path relative to repo root")
    ap.add_argument("--targets", default="tools/coop/targets.json",
                    help="targets registry path relative to repo root")
    args = ap.parse_args(argv)

    root = os.path.abspath(args.repo_root) if args.repo_root else find_repo_root()
    if not root:
        sys.exit(
            "error: could not locate repo root (tools/coop/targets.json); "
            "run from inside the repository or pass --repo-root"
        )

    state = ViewerState(root, args.ledger, args.targets)
    server = ThreadingHTTPServer(("127.0.0.1", args.port), Handler)
    server.daemon_threads = True
    server.viewer = state  # type: ignore[attr-defined]

    print(
        "pi-harness viewer: http://127.0.0.1:%d  (repo: %s)"
        % (args.port, root),
        file=sys.stderr,
    )
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nshutting down", file=sys.stderr)
    finally:
        server.server_close()


if __name__ == "__main__":
    main()

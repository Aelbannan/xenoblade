// ---------------------------------------------------------------------------
// Target loading from tools/coop/targets.json + writable-scope computation.
// ---------------------------------------------------------------------------

interface RawTarget {
  id: string;
  symbol: string;
  function: string;
  unit: string;
  source: string;
  status: string;
  kind?: string;
  size?: number | string;
  region?: string;
  callgraph_source?: string;
  called_functions?: string[];
  unresolved_called_functions?: string[];
  has_indirect_calls?: boolean;
  callgraph_status?: string;
  claim?: { owner?: string } | null;
  [key: string]: unknown;
}

interface TargetsFile {
  targets: RawTarget[];
}

// Target ids flow into child-process argv; reject anything that could be
// parsed as a flag or used for injection.
const SAFE_ID = /^[A-Za-z0-9][\w.:-]*$/;

// source / callgraph_source are joined against repoRoot for fs operations;
// reject absolute paths and `..` traversal.
function isSafeRelPath(p: string | undefined | null): p is string {
  if (!p) return false;
  if (p.startsWith("/") || /^[A-Za-z]:/.test(p)) return false;
  return !p.split("/").includes("..");
}

import { readFileSync, existsSync, statSync } from "node:fs";
import { join, dirname, basename, isAbsolute } from "node:path";
import type { Target } from "./types.js";
import { scanExhaustedTargets, scanTargetSessionCounts } from "./ledger.js";

// ---------------------------------------------------------------------------
// In-memory targets.json cache. targets.json is large (~17.8 MB / 19k targets)
// and read repeatedly (once per TU × 15+ calls per TU). Reading + JSON.parse
// wholesale on every access is the dominant cost during a run (it starved the
// ConcurrencyPool). We cache the parsed array keyed by (path, mtime, size):
// a stat() syscall is ~free vs. 17.8MB of JSON.parse. Any subprocess write
// (run.py cycle/claim/etc) bumps mtime/size, so the cache stays correct.
// ---------------------------------------------------------------------------
interface TargetsCacheEntry {
  mtimeMs: number;
  size: number;
  targets: readonly RawTarget[];
}
const targetsCache = new Map<string, TargetsCacheEntry>();

export function readTargetsFile(repoRoot: string, opts?: { force?: boolean }): RawTarget[] {
  const targetsPath = join(repoRoot, "tools/coop/targets.json");
  // Single pre-read stat: used BOTH for the cache lookup AND the cache store.
  // Keying the store on a separate POST-read stat opens a TOCTOU — a
  // concurrent write between read and stat would pin OLD content under the NEW
  // (mtime,size) and serve stale data until the next write (adversarial review:
  // Kimi HIGH-1 / GLM CRITICAL-1). One stat describes the content we read.
  // mtimeNs (BigInt, nanosecond) + size eliminates same-size/same-ms-tick flips
  // (Kimi LOW-3 / GLM MED).
  let st: ReturnType<typeof statSync> | undefined;
  if (!opts?.force) {
    try {
      st = statSync(targetsPath);
    } catch {
      /* stat failed (e.g. ENOENT) — fall through to read below */
    }
  }
  if (st) {
    const cached = targetsCache.get(targetsPath);
    if (cached && cached.mtimeMs === st.mtimeMs && cached.size === st.size) {
      return cached.targets as RawTarget[];
    }
    if (cached) targetsCache.delete(targetsPath);
  }
  let data: TargetsFile | undefined;
  try {
    data = JSON.parse(readFileSync(targetsPath, "utf-8")) as TargetsFile;
  } catch (err) {
    // Parse/read failure. ENOENT is a legitimately-empty registry -> []. A
    // mid-write / corrupt read is NOT: fall back to ANY cached content for THIS
    // call only, but EVICT so the next caller re-reads, and warn loudly.
    const cached = targetsCache.get(targetsPath);
    if (cached) {
      targetsCache.delete(targetsPath);
      process.stderr.write(
        `[pi-harness] WARNING: targets.json read failed ("${(err as Error).message}") — serving cached targets for this call; will re-read next call\n`,
      );
      return cached.targets as RawTarget[];
    }
    return [];
  }
  const targets = Array.isArray(data.targets) ? data.targets : [];
  if (st) {
    const frozen = Object.freeze(targets);
    // Shallow-freeze each raw target + its shared arrays so a future caller
    // cannot mutate the cached array through a returned reference (Kimi LOW-2 /
    // GLM LOW). Object.freeze on 19k objects is done once per cache fill.
    for (const t of frozen) {
      if (t && typeof t === "object") {
        Object.freeze(t);
        const rf = t as unknown as Record<string, unknown>;
        if (Array.isArray(rf.called_functions)) Object.freeze(rf.called_functions);
        if (Array.isArray(rf.unresolved_called_functions)) Object.freeze(rf.unresolved_called_functions);
      }
    }
    targetsCache.set(targetsPath, { mtimeMs: Number(st.mtimeMs), size: Number(st.size), targets: frozen });
    return frozen as RawTarget[];
  }
  return targets;
}

/** Drop the cache (e.g. after the harness itself rewrote targets.json). */
export function invalidateTargetsCache(repoRoot: string): void {
  targetsCache.delete(join(repoRoot, "tools/coop/targets.json"));
}

function toTarget(raw: RawTarget): Target {
  const size = parseTargetSize(raw.size);
  return {
    id: raw.id,
    symbol: raw.symbol,
    function: raw.function,
    unit: raw.unit,
    source: raw.source,
    status: raw.status,
    kind: raw.kind,
    size,
    instructionMatch:
      typeof raw.instruction_match === "number" ? raw.instruction_match
      : typeof raw.instruction_match === "string" ? Number.parseFloat(raw.instruction_match)
      : undefined,
    callgraph_source: raw.callgraph_source,
    called_functions: raw.called_functions,
    unresolved_called_functions: raw.unresolved_called_functions,
    has_indirect_calls: raw.has_indirect_calls,
    callgraph_status: raw.callgraph_status,
  };
}

/**
 * Load targets for a unit, sorted by size ascending (missing size last).
 * Excludes FULL_MATCH / EQUIVALENT_MATCH unless includeMatched is set.
 */
export function loadUnitTargets(
  repoRoot: string,
  region: string,
  unit: string,
  includeMatched = true,
): Target[] {
  const excludedStatuses = new Set(["FULL_MATCH", "EQUIVALENT_MATCH"]);
  const filtered: Target[] = [];

  for (const raw of readTargetsFile(repoRoot)) {
    if (raw.unit !== unit) continue;
    if (!includeMatched && excludedStatuses.has(raw.status)) continue;
    if (raw.kind !== undefined && raw.kind !== null && raw.kind !== "function") continue;
    if (raw.region !== undefined && raw.region !== null && raw.region !== region) continue;

    if (!SAFE_ID.test(raw.id) || !isSafeRelPath(raw.source)) {
      process.stderr.write(
        `[pi-harness] WARNING: skipping target with unsafe id/source: ${JSON.stringify(raw.id)}\n`,
      );
      continue;
    }
    if (
      raw.callgraph_source !== undefined &&
      raw.callgraph_source !== null &&
      !isSafeRelPath(raw.callgraph_source)
    ) {
      process.stderr.write(
        `[pi-harness] WARNING: skipping target with unsafe callgraph_source: ${JSON.stringify(raw.id)}\n`,
      );
      continue;
    }

    filtered.push(toTarget(raw));
  }

  filtered.sort((a, b) => (a.size ?? Infinity) - (b.size ?? Infinity));
  return filtered;
}

/** Parse a target `size` field: hex strings ("0x29C" or "29C" — targets.json
 *  stores retail sizes as hex) and non-negative numbers pass through;
 *  anything unparseable yields undefined (size is optional). Guards against
 *  the parseInt foot-gun where a bare decimal like "668" would silently
 *  parse as hex 0x668 = 1640 (Kimi L4 / GLM F11b: size-parse guard). */
export function parseTargetSize(size: unknown): number | undefined {
  if (typeof size === "number") {
    return Number.isFinite(size) && size >= 0 ? size : undefined;
  }
  if (typeof size === "string") {
    const s = size.trim();
    if (s === "" || s === "0x") return undefined;
    const v = Number.parseInt(s, 16);
    return Number.isNaN(v) ? undefined : v;
  }
  return undefined;
}

/** Optional filtering for loadUnmatchedTargets. Shape-compatible with the
 *  `ledgerPath` / `retryExhausted` / `exhaustionThreshold` fields of
 *  HarnessConfig, so callers can pass those through directly (they are NOT
 *  a full config — unknown fields are ignored). */
export interface UnmatchedOptions {
  /** JSONL ledger path (absolute, or repo-root relative). When set and
   *  `retryExhausted` is false, targets the ledger marked exhausted are
   *  removed from the result. */
  ledgerPath?: string;
  /** Re-attempt targets the ledger marked exhausted (default: false). */
  retryExhausted?: boolean;
  /** Minimum independent dead-end ledger records before a target counts as
   *  exhausted. Threaded to `scanExhaustedTargets`. */
  exhaustionThreshold?: number;
}

/** Load only unmatched (not FULL_MATCH / EQUIVALENT_MATCH) targets for a unit.
 *  Pass `options` to additionally subtract targets marked exhausted in the
 *  ledger (a no-op when no ledgerPath is given, so existing callers are
 *  unaffected). */
export function loadUnmatchedTargets(
  repoRoot: string,
  region: string,
  unit: string,
  options?: UnmatchedOptions,
): Target[] {
  const targets = loadUnitTargets(repoRoot, region, unit, false);
  if (!options?.ledgerPath || options.retryExhausted) return targets;

  const absLedger = isAbsolute(options.ledgerPath)
    ? options.ledgerPath
    : join(repoRoot, options.ledgerPath);
  const exhausted = scanExhaustedTargets(absLedger, options.exhaustionThreshold);
  if (exhausted.size === 0) return targets;

  const filtered = targets.filter((t) => !exhausted.has(t.id));
  const skipped = targets.length - filtered.length;
  if (skipped > 0) {
    process.stderr.write(
      `[pi-harness] skipping ${skipped} exhausted target(s) (use retryExhausted to override)\n`,
    );
  }
  return filtered;
}

/** True if the unit has at least one unmatched target that is NOT exhausted.
 *  Used by --order/--all selection to skip TUs whose remaining work is all
 *  exhausted (they'd be picked up by the pool only to instantly return,
 *  starving the ConcurrencyPool of real work — see the run33 v5 stall).
 *
 *  Also returns true if the PHASE-3 TU-final escape would fire for the unit
 *  (every remaining target has >= maxAttemptsPerTarget ledger sessions): that
 *  path finalises the unit's already-matched functions + data/rename work, so
 *  dropping it from --all would silently lose that work (adversarial review:
 *  GLM HIGH — snd_MidiSeqPlayer was mis-dropped by the threshold-3 exhausted
 *  check even though the orchestrator would have TU-final'd it). */
export function unitHasActionableWork(
  repoRoot: string,
  region: string,
  unit: string,
  ledgerPath: string | undefined,
  exhaustionThreshold?: number,
  retryExhausted?: boolean,
  maxAttemptsPerTarget?: number,
): boolean {
  const targets = loadUnmatchedTargets(repoRoot, region, unit, {
    ledgerPath,
    exhaustionThreshold,
    retryExhausted: retryExhausted ?? false,
  });
  if (targets.length > 0) return true;
  // No non-exhausted targets. If the phase-3 TU-final escape would fire (all
  // remaining unmatched targets ledger-exhausted at the session budget), the
  // unit still has TU-final work — keep it so the orchestrator can run it.
  if (maxAttemptsPerTarget && maxAttemptsPerTarget > 0 && ledgerPath) {
    const remaining = loadUnitTargets(repoRoot, region, unit, false);
    if (remaining.length > 0) {
      const sessionsByTarget = scanTargetSessionCounts(repoRoot, ledgerPath);
      const allExhausted = remaining.every(
        (t) => (sessionsByTarget.get(t.id) ?? 0) >= maxAttemptsPerTarget,
      );
      if (allExhausted) return true;
    }
  }
  return false;
}

/** Per-TU match summary. */
export interface UnitSummary {
  unit: string;
  total: number;
  matched: number;
  remaining: number;
  /** Sum of .text sizes (bytes) of unmatched targets; 0 when none has a size. */
  remainingSize: number;
}

export type UnitOrder = "most-remaining" | "least-remaining" | "smallest" | "alphabetical";

/**
 * Scan all function-kind targets grouped by unit and return per-TU match
 * summaries. Only units with at least one function-kind target are included.
 * Ordered by `order` (default: most-remaining first).
 */
export function loadAllUnitSummaries(
  repoRoot: string,
  region: string,
  order: UnitOrder = "most-remaining",
): UnitSummary[] {
  const accepted = new Set(["FULL_MATCH", "EQUIVALENT_MATCH"]);
  const map = new Map<string, { total: number; matched: number; remainingSize: number }>();

  for (const raw of readTargetsFile(repoRoot)) {
    if (raw.kind !== undefined && raw.kind !== null && raw.kind !== "function") continue;
    if (raw.region !== undefined && raw.region !== null && raw.region !== region) continue;
    const unit = raw.unit ?? "unknown";
    let entry = map.get(unit);
    if (!entry) {
      entry = { total: 0, matched: 0, remainingSize: 0 };
      map.set(unit, entry);
    }
    entry.total++;
    if (accepted.has(raw.status)) {
      entry.matched++;
    } else {
      // Sum unmatched .text size (bytes) for the "smallest" order: a TU
      // with few, large functions may be bigger than one with many small
      // ones, so remaining-COUNT is a poor size proxy. Missing sizes count 0.
      const sz = parseTargetSize(raw.size);
      if (typeof sz === "number") entry.remainingSize += sz;
    }
  }

  const summaries: UnitSummary[] = [];
  for (const [unit, counts] of map) {
    if (counts.total === 0) continue;
    summaries.push({
      unit,
      total: counts.total,
      matched: counts.matched,
      remaining: counts.total - counts.matched,
      remainingSize: counts.remainingSize,
    });
  }

  switch (order) {
    case "most-remaining":
      summaries.sort((a, b) => b.remaining - a.remaining || a.unit.localeCompare(b.unit));
      break;
    case "least-remaining":
      summaries.sort((a, b) => a.remaining - b.remaining || a.unit.localeCompare(b.unit));
      break;
    case "smallest":
      // Smallest total UNMATCHED size first (sum of target .text bytes).
      // Ties broken by remaining count, then unit name.
      summaries.sort(
        (a, b) => (a.remainingSize - b.remainingSize)
          || (a.remaining - b.remaining)
          || a.unit.localeCompare(b.unit),
      );
      break;
    case "alphabetical":
      summaries.sort((a, b) => a.unit.localeCompare(b.unit));
      break;
  }
  return summaries;
}

/** Ids of targets currently claimed by `owner` (for stale-claim cleanup). */
export function findClaimsByOwner(repoRoot: string, owner: string): string[] {
  const ids: string[] = [];
  for (const raw of readTargetsFile(repoRoot)) {
    if (raw.claim && raw.claim.owner === owner && SAFE_ID.test(raw.id)) {
      ids.push(raw.id);
    }
  }
  return ids;
}

// ── Call-graph readiness (Phase 4 similarity re-ranker) ───────────────────

const ACCEPTED_MATCH = new Set(["FULL_MATCH", "EQUIVALENT_MATCH"]);

/** Map of target id -> match status across the whole registry. Callees can
 *  live in other units, so readiness must be computed against the full file,
 *  not just the unit's own target list. */
export function targetStatusById(repoRoot: string): Map<string, string> {
  const statuses = new Map<string, string>();
  for (const raw of readTargetsFile(repoRoot)) {
    statuses.set(raw.id, raw.status);
  }
  return statuses;
}

/**
 * Per-target { status, workflowStatus, claimedBy } for post-cycle re-checks.
 * Function acceptance is per-function: cmd_cycle no longer records BACKLOG
 * for unit-size overruns (the size gate lives in TU-final promotion, user
 * policy 2026-08), so the status alone is sufficient. This helper remains
 * for the workflowStatus/claimedBy fields used elsewhere.
 */
export function targetRowById(repoRoot: string): Map<string, { status: string; workflowStatus?: string; claimedBy?: string }> {
  const rows = new Map<string, { status: string; workflowStatus?: string; claimedBy?: string }>();
  for (const raw of readTargetsFile(repoRoot)) {
    const claim = raw.claim as { owner?: string } | null | undefined;
    rows.set(raw.id, {
      status: raw.status,
      workflowStatus: typeof raw.workflow_status === "string" ? (raw.workflow_status as string) : undefined,
      claimedBy: claim?.owner,
    });
  }
  return rows;
}

/** Mirror of `run.py harness --selection ready`
 *  (tools/coop/lib/targets.py::harness_targets): leaf = no direct,
 *  unresolved, or indirect calls; otherwise every direct callee must already
 *  be FULL_MATCH / EQUIVALENT_MATCH. Lenient when callgraph fields are
 *  absent (treated as leaf) so a small ready set never starves a batch — the
 *  hard callee gate stays in `run.py cycle`, which fails closed on
 *  unaccepted callees regardless of this ranking. */
export function isCallGraphReady(target: Target, statusById: Map<string, string>): boolean {
  const unresolved = target.unresolved_called_functions ?? [];
  if (unresolved.length > 0) return false;
  if (target.has_indirect_calls) return false;
  const called = target.called_functions ?? [];
  if (called.length === 0) return true;
  return called.every((id) => ACCEPTED_MATCH.has(statusById.get(id) ?? ""));
}

/**
 * Compute the writable scope for a set of targets: unique source paths plus,
 * for each `X.cpp`, the header `X.hpp` if it exists (same dir, then the
 * src/ → include/ mirror).
 */
export function writableScopeForTargets(repoRoot: string, targets: Target[]): string[] {
  const seen = new Set<string>();
  const result: string[] = [];

  for (const target of targets) {
    if (!target.source || seen.has(target.source)) continue;
    seen.add(target.source);
    result.push(target.source);

    const srcDir = dirname(target.source);
    const srcBase = basename(target.source);
    const headerName = srcBase.replace(/\.(c|cpp|cc)$/, ".hpp");
    if (headerName === srcBase) continue;

    const sameDirHeader = join(srcDir, headerName);
    if (existsSync(join(repoRoot, sameDirHeader)) && !seen.has(sameDirHeader)) {
      seen.add(sameDirHeader);
      result.push(sameDirHeader);
    }

    if (target.source.startsWith("src/")) {
      const mirror = target.source
        .replace(/^src\//, "include/")
        .replace(/\.(c|cpp|cc)$/, ".hpp");
      if (mirror !== target.source && existsSync(join(repoRoot, mirror)) && !seen.has(mirror)) {
        seen.add(mirror);
        result.push(mirror);
      }
    }
  }

  return result;
}

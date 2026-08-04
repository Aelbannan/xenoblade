// ---------------------------------------------------------------------------
// Target loading from tools/coop/targets.json + writable-scope computation.
// ---------------------------------------------------------------------------

import { readFileSync, existsSync } from "node:fs";
import { join, dirname, basename, isAbsolute } from "node:path";
import type { Target } from "./types.js";
import { scanExhaustedTargets } from "./ledger.js";

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

function readTargetsFile(repoRoot: string): RawTarget[] {
  const targetsPath = join(repoRoot, "tools/coop/targets.json");
  const data = JSON.parse(readFileSync(targetsPath, "utf-8")) as TargetsFile;
  return Array.isArray(data.targets) ? data.targets : [];
}

function toTarget(raw: RawTarget): Target {
  let size: number | undefined;
  if (raw.size !== undefined && raw.size !== null) {
    if (typeof raw.size === "string") {
      size = Number.parseInt(raw.size, 16);
      if (Number.isNaN(size)) size = undefined;
    } else if (typeof raw.size === "number") {
      size = raw.size;
    }
  }
  return {
    id: raw.id,
    symbol: raw.symbol,
    function: raw.function,
    unit: raw.unit,
    source: raw.source,
    status: raw.status,
    kind: raw.kind,
    size,
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

/** Optional filtering for loadUnmatchedTargets. Shape-compatible with the
 *  `ledgerPath` / `retryExhausted` fields of HarnessConfig, so a config object
 *  can be passed directly as `options`. */
export interface UnmatchedOptions {
  /** JSONL ledger path (absolute, or repo-root relative). When set and
   *  `retryExhausted` is false, targets the ledger marked exhausted are
   *  removed from the result. */
  ledgerPath?: string;
  /** Re-attempt targets the ledger marked exhausted (default: false). */
  retryExhausted?: boolean;
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
  const exhausted = scanExhaustedTargets(absLedger);
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

/** Per-TU match summary. */
export interface UnitSummary {
  unit: string;
  total: number;
  matched: number;
  remaining: number;
}

export type UnitOrder = "most-remaining" | "least-remaining" | "alphabetical";

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
  const map = new Map<string, { total: number; matched: number }>();

  for (const raw of readTargetsFile(repoRoot)) {
    if (raw.kind !== undefined && raw.kind !== null && raw.kind !== "function") continue;
    if (raw.region !== undefined && raw.region !== null && raw.region !== region) continue;
    const unit = raw.unit ?? "unknown";
    let entry = map.get(unit);
    if (!entry) {
      entry = { total: 0, matched: 0 };
      map.set(unit, entry);
    }
    entry.total++;
    if (accepted.has(raw.status)) entry.matched++;
  }

  const summaries: UnitSummary[] = [];
  for (const [unit, counts] of map) {
    if (counts.total === 0) continue;
    summaries.push({
      unit,
      total: counts.total,
      matched: counts.matched,
      remaining: counts.total - counts.matched,
    });
  }

  switch (order) {
    case "most-remaining":
      summaries.sort((a, b) => b.remaining - a.remaining || a.unit.localeCompare(b.unit));
      break;
    case "least-remaining":
      summaries.sort((a, b) => a.remaining - b.remaining || a.unit.localeCompare(b.unit));
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

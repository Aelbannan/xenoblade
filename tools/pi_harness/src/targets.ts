// ---------------------------------------------------------------------------
// Target loading from tools/coop/targets.json + writable-scope computation.
// ---------------------------------------------------------------------------

import { readFileSync, existsSync } from "node:fs";
import { join, dirname, basename } from "node:path";
import type { Target } from "./types.js";

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

/** Load only unmatched (not FULL_MATCH / EQUIVALENT_MATCH) targets for a unit. */
export function loadUnmatchedTargets(
  repoRoot: string,
  region: string,
  unit: string,
): Target[] {
  return loadUnitTargets(repoRoot, region, unit, false);
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

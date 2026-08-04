// ---------------------------------------------------------------------------
// JSONL ledger. Appends are serialised through an in-process queue: parallel
// TU workers share this process, and concurrent appends with lines larger
// than PIPE_BUF interleave and corrupt the JSONL.
// ---------------------------------------------------------------------------

import { readFileSync, existsSync, mkdirSync } from "node:fs";
import { appendFile } from "node:fs/promises";
import { dirname, join } from "node:path";
import type { LedgerEntry } from "./types.js";

let writeQueue: Promise<void> = Promise.resolve();

/** Wait for all queued ledger appends to flush (call before process exit). */
export async function drainLedger(): Promise<void> {
  await writeQueue;
}

/** Append one JSON line to the ledger (queued, ordered, fire-and-forget). */
export function appendLedger(repoRoot: string, ledgerPath: string, entry: LedgerEntry): void {
  const absPath = join(repoRoot, ledgerPath);
  const parent = dirname(absPath);
  if (!existsSync(parent)) mkdirSync(parent, { recursive: true });

  const line = JSON.stringify(entry) + "\n";
  writeQueue = writeQueue.then(async () => {
    try {
      await appendFile(absPath, line, "utf-8");
    } catch (err) {
      process.stderr.write(
        `[pi-harness] WARNING: ledger append failed: ${err instanceof Error ? err.message : String(err)}\n`,
      );
    }
  });
}

/** Parse a ledger file body into entries, skipping malformed lines. */
function parseLedgerLines(text: string): LedgerEntry[] {
  const entries: LedgerEntry[] = [];
  for (const line of text.split("\n")) {
    const trimmed = line.trim();
    if (!trimmed) continue;
    try {
      entries.push(JSON.parse(trimmed) as LedgerEntry);
    } catch {
      // Skip malformed lines (e.g. a crash mid-append).
    }
  }
  return entries;
}

/** Read all ledger entries; returns [] when the file is missing. */
export function readLedger(repoRoot: string, ledgerPath: string): LedgerEntry[] {
  const absPath = join(repoRoot, ledgerPath);
  if (!existsSync(absPath)) return [];
  return parseLedgerLines(readFileSync(absPath, "utf-8"));
}

// ---------------------------------------------------------------------------
// Dead-end / session-count scan. Sync cores mirror readLedger's malformed-line
// tolerance; the async wrappers are the primary API for the orchestrator.
// ---------------------------------------------------------------------------

/** `batch-cycle` result statuses that mark a target as a dead end. */
const DEAD_CYCLE_STATUSES: ReadonlySet<string> = new Set(["NOT_BUILDABLE", "NOT_FOUND"]);

/** Extract `targetId` / `targetIds` from an event's detail (tolerant of
 *  missing or oddly-shaped detail). */
function extractTargetIds(detail: unknown): string[] {
  if (!detail || typeof detail !== "object" || Array.isArray(detail)) return [];
  const d = detail as Record<string, unknown>;
  const ids: string[] = [];
  const single = d.targetId;
  if (typeof single === "string" && single) ids.push(single);
  const many = d.targetIds;
  if (Array.isArray(many)) {
    for (const id of many) {
      if (typeof id === "string" && id) ids.push(id);
    }
  }
  return ids;
}

/** Target ids in a `batch-cycle` `results[]` whose status is a dead end. */
function cycleDeadEndIds(detail: unknown, deadStatuses: ReadonlySet<string>): string[] {
  if (!detail || typeof detail !== "object" || Array.isArray(detail)) return [];
  const results = (detail as Record<string, unknown>).results;
  if (!Array.isArray(results)) return [];
  const ids: string[] = [];
  for (const r of results) {
    if (!r || typeof r !== "object" || Array.isArray(r)) continue;
    const row = r as Record<string, unknown>;
    if (typeof row.targetId !== "string" || !row.targetId) continue;
    if (typeof row.status === "string" && deadStatuses.has(row.status)) {
      ids.push(row.targetId);
    }
  }
  return ids;
}

/** Whether a `batch-cycle` `results[]` names the target (any status). */
function cycleContainsTarget(detail: unknown, targetId: string): boolean {
  if (!detail || typeof detail !== "object" || Array.isArray(detail)) return false;
  const results = (detail as Record<string, unknown>).results;
  if (!Array.isArray(results)) return false;
  return results.some(
    (r) =>
      !!r &&
      typeof r === "object" &&
      !Array.isArray(r) &&
      (r as Record<string, unknown>).targetId === targetId,
  );
}

/**
 * Scan the ledger for targets the harness has marked as dead ends:
 * `batch-session-exhausted` and `target-skipped` events (detail carries
 * `targetId` / `targetIds`), plus `batch-cycle` rows whose `results[]` has a
 * `NOT_BUILDABLE` / `NOT_FOUND` status. Missing or unreadable ledger yields an
 * empty set. Single pass over the file.
 */
export function scanExhaustedTargets(ledgerPath: string): Set<string> {
  const exhausted = new Set<string>();
  let text: string;
  try {
    text = readFileSync(ledgerPath, "utf-8");
  } catch {
    return exhausted;
  }
  for (const entry of parseLedgerLines(text)) {
    switch (entry.event) {
      case "batch-session-exhausted":
      case "target-skipped":
        for (const id of extractTargetIds(entry.detail)) exhausted.add(id);
        break;
      case "batch-cycle":
        for (const id of cycleDeadEndIds(entry.detail, DEAD_CYCLE_STATUSES)) exhausted.add(id);
        break;
      default:
        break;
    }
  }
  return exhausted;
}

/** Async API: see {@link scanExhaustedTargets}. */
export async function readExhaustedTargets(ledgerPath: string): Promise<Set<string>> {
  return scanExhaustedTargets(ledgerPath);
}

/**
 * Count how many sessions a target has been through: `batch-cycle` events
 * whose `results[]` names it, plus `batch-session-exhausted` rows for it
 * (only singleton-path rows carry a `targetId` in detail). Single pass.
 */
export function scanTargetSessionCount(ledgerPath: string, targetId: string): number {
  let sessions = 0;
  let text: string;
  try {
    text = readFileSync(ledgerPath, "utf-8");
  } catch {
    return 0;
  }
  for (const entry of parseLedgerLines(text)) {
    if (entry.event === "batch-session-exhausted") {
      if (extractTargetIds(entry.detail).includes(targetId)) sessions++;
    } else if (entry.event === "batch-cycle") {
      if (cycleContainsTarget(entry.detail, targetId)) sessions++;
    }
  }
  return sessions;
}

/** Async API: see {@link scanTargetSessionCount}. */
export async function countTargetSessions(ledgerPath: string, targetId: string): Promise<number> {
  return scanTargetSessionCount(ledgerPath, targetId);
}

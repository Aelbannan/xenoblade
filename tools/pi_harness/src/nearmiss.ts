// ---------------------------------------------------------------------------
// Near-miss draft banking (Phase 2): whole-file snapshot copies + an index.
//
// On a fail-with-compiling-draft, onVerify banks the CURRENT worktree state
// of the writable files (which hold the model's best attempt) BEFORE any
// restoreSnapshot wipes them. The bank is keyed per target, stores whole-file
// copies (multi-function TUs can't be cut per-function), and only replaces the
// stored best when the composite score improves (statusRank asc, then
// structuralCount desc — reg_swap is free noise for SMT acceptance).
//
// Retry then restores the banked file into the worktree instead of the
// pristine snapshot, so the next session literally resumes from the draft.
// ---------------------------------------------------------------------------

import { copyFile, mkdir, readFile, writeFile, appendFile, readdir, stat } from "node:fs/promises";
import { existsSync } from "node:fs";
import { join, dirname, relative, resolve } from "node:path";
import type { HarnessConfig } from "./types.js";

export interface NearMissRow {
  tid: string;
  unit: string;
  /** Repo-relative paths banked (the writable scope at bank time). */
  files: string[];
  /** Composite closeness: higher is better (see scoreRank). */
  statusRank: number;
  /** Structural mismatches — the real codegen-shape signal. */
  structuralCount: number;
  /** Total mismatches (incl. reg_swap noise). -1 = hexdiff failed. */
  mismatchCount: number;
  sessionFile?: string;
  round?: number;
  ts: string;
  /** Round dir (relative to the bank dir) this row's copies live in. Lets
   *  restoreBankedDraft restore the BEST row's exact files instead of the
   *  lexicographically-newest dir (adversarial review M4). */
  roundDir?: string;
}

/** Status ladder rank from batch-cycle results; higher = closer to accept. */
const STATUS_RANK: Record<string, number> = {
  COMPILES: 1,
  STRUCTURAL: 2,
  HIGH_MATCH: 3,
  CODE_MATCH: 4,
  FULL_MATCH: 5,
  EQUIVALENT_MATCH: 6,
};

export function statusRank(status: string | undefined): number {
  if (!status) return 0;
  const rank = STATUS_RANK[status];
  return rank ?? (status === "PASS" ? 6 : 0);
}

/**
 * Composite closeness key: (statusRank, -structuralCount). A higher rank wins;
 * within the same rank a LOWER structuralCount wins. reg_swap is deliberately
 * ignored (it is noise for the SMT-equivalence acceptance model).
 */
export function betterThan(a: NearMissRow, b: NearMissRow): boolean {
  if (a.statusRank !== b.statusRank) return a.statusRank > b.statusRank;
  return a.structuralCount < b.structuralCount;
}

function indexPath(repoRoot: string, config: HarnessConfig): string {
  return join(repoRoot, config.nearmissDir, "index.jsonl");
}

function bankDirFor(repoRoot: string, config: HarnessConfig, unit: string, tid: string): string {
  return join(repoRoot, config.nearmissDir, unit.replace(/\//g, "__"), tid);
}

/** Read all banked rows (tolerant of malformed/partial lines). */
export async function readNearMissIndex(
  repoRoot: string, config: HarnessConfig,
): Promise<NearMissRow[]> {
  const p = indexPath(repoRoot, config);
  if (!existsSync(p)) return [];
  const rows: NearMissRow[] = [];
  try {
    const raw = await readFile(p, "utf-8");
    for (const line of raw.split("\n")) {
      const t = line.trim();
      if (!t) continue;
      try {
        rows.push(JSON.parse(t) as NearMissRow);
      } catch {
        // malformed line — skip
      }
    }
  } catch {
    // unreadable — treat as empty
  }
  return rows;
}

/** Best banked draft for one target, or undefined. */
export async function bestBankedDraft(
  repoRoot: string, config: HarnessConfig, tid: string,
): Promise<NearMissRow | undefined> {
  const rows = (await readNearMissIndex(repoRoot, config)).filter((r) => r.tid === tid);
  if (rows.length === 0) return undefined;
  return rows.reduce((best, r) => (betterThan(r, best) ? r : best));
}

/**
 * Bank the current worktree state of `files` (repo-relative writable scope)
 * for `tid` if it beats the stored best (and bankOnlyOnBetter is on).
 * Copies are whole files — the same copy serves every target sharing the TU.
 */
export async function bankDraft(
  repoRoot: string,
  config: HarnessConfig,
  unit: string,
  tid: string,
  files: string[],
  status: string | undefined,
  structuralCount: number,
  mismatchCount: number,
  sessionFile?: string,
  round?: number,
): Promise<NearMissRow | undefined> {
  if (structuralCount < 0 || mismatchCount < 0) return undefined; // hexdiff failed — never bank
  const rank = statusRank(status);
  const candidate: NearMissRow = {
    tid, unit, files, statusRank: rank, structuralCount, mismatchCount,
    sessionFile, round, ts: new Date().toISOString(),
  };
  const best = await bestBankedDraft(repoRoot, config, tid);
  if (config.bankOnlyOnBetter && best && !betterThan(candidate, best)) {
    return undefined; // not better — keep the stored best
  }

  // Copy the whole writable-scope files into a round-stamped dir.
  const dir = join(bankDirFor(repoRoot, config, unit, tid), `r${round ?? 0}-${Date.now()}`);
  await mkdir(dir, { recursive: true });
  const bankedFiles: string[] = [];
  for (const file of files) {
    const src = join(repoRoot, file);
    if (!existsSync(src)) continue;
    const dst = join(dir, file);
    await mkdir(dirname(dst), { recursive: true });
    await copyFile(src, dst);
    bankedFiles.push(file);
  }
  if (bankedFiles.length === 0) return undefined;
  candidate.files = bankedFiles;
  candidate.roundDir = `r${round ?? 0}-${Date.now()}`;

  // Append to the index (append-only; last row for a tid is not authoritative,
  // readers reduce by betterThan — see bestBankedDraft). Appends are
  // serialised through a process-wide queue: parallel TU workers bank
  // concurrently, and interleaved appendFile calls can corrupt the JSONL
  // (adversarial review M3 — the same class of bug the ledger queue exists for).
  const idx = indexPath(repoRoot, config);
  await mkdir(dirname(idx), { recursive: true });
  await enqueueIndexAppend(idx, JSON.stringify(candidate) + "\n");
  return candidate;
}

/** In-process FIFO for index.jsonl appends (see M3). */
let indexAppendQueue: Promise<void> = Promise.resolve();
function enqueueIndexAppend(idxPath: string, line: string): Promise<void> {
  indexAppendQueue = indexAppendQueue.then(async () => {
    await appendFile(idxPath, line, "utf-8");
  });
  return indexAppendQueue;
}

/**
 * Restore the banked files into the worktree (used on retry so the session
 * resumes FROM the draft instead of the pristine snapshot). Returns the
 * number of files restored.
 */
export async function restoreBankedDraft(
  repoRoot: string, config: HarnessConfig, tid: string,
): Promise<number> {
  const best = await bestBankedDraft(repoRoot, config, tid);
  if (!best) return 0;
  let restored = 0;
  for (const file of best.files) {
    // Restore from the BEST row's own round dir (roundDir, recorded at bank
    // time) — NOT the lexicographically-newest dir. With bankOnlyOnBetter the
    // newest dir usually equals the best, but with bankOnlyOnBetter=false (or
    // drifted index rows) the newest dir can hold a WORSE draft than the one
    // bestBankedDraft selected (adversarial review M4).
    const dir = join(bankDirFor(repoRoot, config, best.unit, tid));
    if (!existsSync(dir)) continue;
    const roundDir = best.roundDir;
    if (!roundDir) {
      // Legacy row without roundDir — fall back to the newest dir.
      const rounds = (await readdir(dir)).sort().reverse();
      if (rounds.length === 0) continue;
      const src = join(dir, rounds[0], file);
      if (existsSync(src)) {
        const dst = join(repoRoot, file);
        await mkdir(dirname(dst), { recursive: true });
        await copyFile(src, dst);
        restored++;
      }
      continue;
    }
    const src = join(dir, roundDir, file);
    if (existsSync(src)) {
      const dst = join(repoRoot, file);
      await mkdir(dirname(dst), { recursive: true });
      await copyFile(src, dst);
      restored++;
    }
  }
  return restored;
}

/** True when the target has any banked draft. */
export async function hasBankedDraft(
  repoRoot: string, config: HarnessConfig, tid: string,
): Promise<boolean> {
  return (await bestBankedDraft(repoRoot, config, tid)) !== undefined;
}

/** One-line draft note for the brief (Phase 2 refine-from-draft). */
export function draftNoteFor(row: NearMissRow): string {
  return (
    `A previous best draft is banked (status=${row.statusRank ? "rank " + row.statusRank : "unknown"}, ` +
    `structural=${row.structuralCount}, mismatch=${row.mismatchCount}). Continue from the current file ` +
    `state if it still reflects that draft; refine toward structural=0.`
  );
}

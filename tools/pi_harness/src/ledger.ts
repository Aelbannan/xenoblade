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

/** Read all ledger entries; returns [] when the file is missing. */
export function readLedger(repoRoot: string, ledgerPath: string): LedgerEntry[] {
  const absPath = join(repoRoot, ledgerPath);
  if (!existsSync(absPath)) return [];

  const entries: LedgerEntry[] = [];
  for (const line of readFileSync(absPath, "utf-8").split("\n")) {
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

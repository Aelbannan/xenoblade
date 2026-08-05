// ---------------------------------------------------------------------------
// Pure routing / budget helpers for the retry phases (LOW-4: extracted from
// orchestrator.ts so the rebatch budget derivation and the Pass-2 near-match
// partition are unit-testable without dragging in session machinery).
// ---------------------------------------------------------------------------

export interface NearMatchPartition {
  /** Failed ids at ≥ minPct instruction match (or CODE_MATCH / HIGH_MATCH
   *  status), sorted by instructionMatch descending (best first). */
  nearMatch: string[];
  /** The rest, in input order. */
  rest: string[];
}

/** Rebatch sessions needed to cover the pool ONCE — each failed small target
 *  is included in exactly one rebatch session. */
export function sessionsNeededForPool(poolSize: number, batchSize: number): number {
  if (poolSize <= 0 || batchSize <= 0) return 0;
  return Math.ceil(poolSize / batchSize);
}

/** Effective rebatch session budget = max(configured, sessions needed to
 *  cover the pool once). A configured 0 means auto-derive from the pool. */
export function effectiveRebatchBudget(
  configured: number,
  poolSize: number,
  batchSize: number,
): number {
  return Math.max(configured, sessionsNeededForPool(poolSize, batchSize));
}

/** Partition failed target ids into near-match (priority singletons) vs the
 *  rest. A target is near-match when its status is CODE_MATCH / HIGH_MATCH or
 *  its instructionMatch is ≥ minPct (r8 WS-2: these are the
 *  highest-conversion-probability singletons and override the size-based
 *  rebatch routing). */
export function partitionFailedTargets(
  failedIds: string[],
  byId: Map<string, { status?: string; instructionMatch?: number }>,
  minPct = 90,
): NearMatchPartition {
  const nearMatch: string[] = [];
  const rest: string[] = [];
  for (const fid of failedIds) {
    const target = byId.get(fid);
    const near =
      target && (target.status === "CODE_MATCH" || target.status === "HIGH_MATCH"
        || (target.instructionMatch ?? 0) >= minPct);
    (near ? nearMatch : rest).push(fid);
  }
  nearMatch.sort((a, b) =>
    (byId.get(b)?.instructionMatch ?? 0) - (byId.get(a)?.instructionMatch ?? 0));
  return { nearMatch, rest };
}

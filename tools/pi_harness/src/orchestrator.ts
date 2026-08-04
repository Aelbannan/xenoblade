/**
 * TU-level orchestration for the pi-harness.
 *
 * runTus manages a concurrency-limited pool of TU workers. Each TU:
 *   1. Loads unmatched targets
 *   2. Processes them in batches:
 *      claim → brief → snapshot → session (with optional in-session
 *      continuation on compile/lint failure via multi-prompt) →
 *      batch-cycle (acceptance). Restore from snapshot copies ONLY when
 *      falling back to a fresh session.
 *   3. Routes large failed targets to singleton sessions
 *   4. Collects small failed targets for a re-batch pass
 *   5. Runs a TU-finalisation session once every function matches
 *
 * @module orchestrator
 */

import { execFileSync } from "node:child_process";
import { readFileSync } from "node:fs";
import { join } from "node:path";
import { ModelRuntime } from "@earendil-works/pi-coding-agent";
import type {
  HarnessConfig, Target, TargetBrief, SessionUsage, VerifyResult, SiblingPointer,
  TriageRow, TriageSummary,
} from "./types.js";
import {
  findClaimsByOwner,
  loadUnmatchedTargets,
  loadUnitTargets,
  writableScopeForTargets,
  targetStatusById,
  isCallGraphReady,
} from "./targets.js";
import { extractRetailAsm, buildBatchBrief } from "./brief.js";
import { appendLedger, readLedger, drainLedger } from "./ledger.js";
import {
  bankDraft, bestBankedDraft, draftNoteFor, restoreBankedDraft,
} from "./nearmiss.js";
import { buildBatchPrompt, buildTuFinalPrompt } from "./prompts.js";
import { runAgentSession, type SessionRunResult } from "./session.js";
import {
  snapshotUnit,
  restoreSnapshot,
  runLint,
  buildUnit,
  runBatchCycle,
  runHexdiff,
  claimTargets,
  releaseTargets,
  execFilePromise,
  type Snapshot,
  type LintOutcome,
} from "./acceptance.js";

const OWNER = `pi-harness-${process.pid}`;

/**
 * Find claims owned by dead pi-harness processes.
 * Scans targets.json for claims with owner matching `pi-harness-*`,
 * then checks if the owning process is still alive.
 */
function findOrphanedClaims(repoRoot: string): Array<{ id: string; owner: string }> {
  const orphaned: Array<{ id: string; owner: string }> = [];
  try {
    const targetsPath = join(repoRoot, "tools", "coop", "targets.json");
    const raw = JSON.parse(readFileSync(targetsPath, "utf-8"));
    const targets = raw.targets ?? [];
    for (const t of targets) {
      if (!t.claim?.owner?.startsWith("pi-harness-")) continue;
      // Skip our own claims (handled separately).
      if (t.claim.owner === OWNER) continue;
      // Extract PID from owner name.
      const pid = parseInt(t.claim.owner.replace("pi-harness-", ""), 10);
      if (isNaN(pid)) continue;
      // Check if process is still alive.
      try {
        process.kill(pid, 0); // Signal 0 = check existence
      } catch {
        // Process doesn't exist — claim is orphaned.
        orphaned.push({ id: t.id, owner: t.claim.owner });
      }
    }
  } catch {
    // best-effort
  }
  return orphaned;
}

// Currently-held claims, released on SIGINT/SIGTERM.
const activeClaims = new Set<string>();

// TU-final phases are serialised process-wide: they run configure.py and
// full ninja builds, which must never overlap each other.
let tuFinalQueue: Promise<void> = Promise.resolve();

// ─────────────────────────────────────────────────────────────────────
//  Concurrency pool
// ─────────────────────────────────────────────────────────────────────

class ConcurrencyPool {
  private running = 0;
  private queue: (() => Promise<void>)[] = [];
  private done: (() => void) | null = null;

  constructor(private max: number) {}

  add(fn: () => Promise<void>): void {
    this.queue.push(fn);
    this.schedule();
  }

  private schedule(): void {
    while (this.running < this.max && this.queue.length > 0) {
      const fn = this.queue.shift()!;
      this.running++;
      fn().finally(() => {
        this.running--;
        this.schedule();
        if (this.running === 0 && this.queue.length === 0 && this.done) {
          this.done();
        }
      });
    }
  }

  drain(): Promise<void> {
    if (this.running === 0 && this.queue.length === 0) return Promise.resolve();
    return new Promise((resolve) => {
      this.done = resolve;
      this.schedule();
    });
  }
}

// ─────────────────────────────────────────────────────────────────────
//  Public entry point
// ─────────────────────────────────────────────────────────────────────

export async function runTus(
  repoRoot: string,
  units: string[],
  config: HarnessConfig,
  opts: { dryRun: boolean },
): Promise<void> {
  const modelRuntime = await ModelRuntime.create();

  // Fail fast on a misconfigured model BEFORE claiming targets or spawning
  // TU workers. runAgentSession would throw mid-run otherwise, burning an
  // entire TU's claims + a wall-clock session on a config typo (the ledger
  // has ~113 batch-error events from exactly this: "Model X not found").
  for (const [label, spec] of [
    ["matchModel", config.matchModel],
    ["cleanupModel", config.cleanupModel],
  ] as const) {
    if (!modelRuntime.getModel(spec.provider, spec.model)) {
      const available = await modelRuntime.getAvailable();
      const names = available.map((m) => `${m.provider}/${m.id}`).join(", ");
      throw new Error(
        `[preflight] ${label} "${spec.provider}/${spec.model}" not found. ` +
          `Available models: ${names || "(none configured)"}. ` +
          `Fix pi-harness.json before running.`,
      );
    }
  }

  // Release claims left behind by crashed/killed previous runs.
  // With PID-based owners, we release:
  // 1. Claims from this exact process (in case of restart with same PID)
  // 2. Claims from dead processes (orphans with pi-harness-* owner)
  if (!opts.dryRun) {
    // Release claims from this process (if any leftover from a restart).
    const stale = findClaimsByOwner(repoRoot, OWNER);
    if (stale.length > 0) {
      process.stderr.write(`[pi-harness] Releasing ${stale.length} stale claim(s) from this process\n`);
      await releaseTargets(repoRoot, config.pythonBin, stale, OWNER);
    }

    // Release claims from dead pi-harness processes.
    const orphaned = findOrphanedClaims(repoRoot);
    if (orphaned.length > 0) {
      process.stderr.write(`[pi-harness] Releasing ${orphaned.length} orphaned claim(s) from dead processes\n`);
      for (const { id, owner } of orphaned) {
        try {
          execFileSync(
            config.pythonBin,
            ["tools/coop/run.py", "targets", "release", id, "--owner", owner],
            { cwd: repoRoot, stdio: "ignore", timeout: 5000 },
          );
        } catch {
          // best-effort
        }
      }
    }
  }

  // Best-effort claim release on interrupt/termination.
  const onSignal = async (): Promise<void> => {
    for (const id of activeClaims) {
      try {
        execFileSync(
          config.pythonBin,
          ["tools/coop/run.py", "targets", "release", id, "--owner", OWNER],
          { cwd: repoRoot, stdio: "ignore", timeout: 5000 },
        );
      } catch {
        // best-effort
      }
    }
    await drainLedger();
    process.exit(130);
  };
  process.on("SIGINT", onSignal);
  process.on("SIGTERM", onSignal);

  try {
    const pool = new ConcurrencyPool(config.maxParallelTUs);
    for (const unit of units) {
      pool.add(async () => {
        try {
          await runOneTu(repoRoot, unit, config, modelRuntime, opts.dryRun);
        } catch (err) {
          const msg = err instanceof Error ? err.message : String(err);
          process.stderr.write(`[pi-harness] Error processing unit "${unit}": ${msg}\n`);
          appendLedger(repoRoot, config.ledgerPath, {
            ts: new Date().toISOString(), event: "tu-error", tu: unit, detail: { error: msg },
          });
        }
      });
    }
    await pool.drain();
  } finally {
    process.removeListener("SIGINT", onSignal);
    process.removeListener("SIGTERM", onSignal);
  }
}

// ─────────────────────────────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────────────────────────────

function buildBriefs(
  repoRoot: string,
  targets: Target[],
  siblingsByTarget?: Map<string, SiblingPointer[]>,
): { briefs: TargetBrief[]; missingAsm: string[] } {
  const briefs: TargetBrief[] = [];
  const missingAsm: string[] = [];
  for (const t of targets) {
    let asm = "";
    try {
      asm = extractRetailAsm(join(repoRoot, t.callgraph_source ?? ""), t.symbol);
    } catch {
      // handled below
    }
    if (!asm) {
      missingAsm.push(t.id);
      process.stderr.write(
        `[pi-harness] WARNING: no retail ASM for ${t.id} (${t.symbol}) — skipping\n`,
      );
      continue;
    }
    const siblings = siblingsByTarget?.get(t.id);
    briefs.push({
      targetId: t.id,
      symbol: t.symbol,
      demangled: t.function,
      retailAsm: asm,
      ...(siblings && siblings.length > 0 ? { siblings } : {}),
    });
  }
  return { briefs, missingAsm };
}

/**
 * Attach the best banked near-miss draft note to each brief (Phase 2
 * refine-from-draft). Async — reads the nearmiss index.
 */
async function attachBankedDrafts(
  repoRoot: string,
  config: HarnessConfig,
  briefs: TargetBrief[],
): Promise<TargetBrief[]> {
  if (!config.nearmissDir) return briefs;
  const withDraft: TargetBrief[] = [];
  for (const b of briefs) {
    const best = await bestBankedDraft(repoRoot, config, b.targetId);
    if (best) {
      withDraft.push({ ...b, draftNote: draftNoteFor(best) });
    } else {
      withDraft.push(b);
    }
  }
  return withDraft;
}

// ─────────────────────────────────────────────────────────────────────
//  Phase 4: similarity-anchored selection (re-ranker, never a replacer)
// ─────────────────────────────────────────────────────────────────────

interface SimilarityRank {
  score: number;
  topSiblings: SiblingPointer[];
}

/** Run `tools/coop/sim_schedule.py --json` for the unit (read-only) and
 *  parse its JSONL output into targetId -> {score, topSiblings}. Returns an
 *  empty map on any failure — callers fall back to claim-order. */
function fetchSimilarityRanking(
  repoRoot: string,
  unit: string,
  config: HarnessConfig,
): Map<string, SimilarityRank> {
  const ranking = new Map<string, SimilarityRank>();
  let stdout: string;
  try {
    stdout = execFileSync(
      config.pythonBin,
      ["tools/coop/sim_schedule.py", "--unit", unit, "--json"],
      { cwd: repoRoot, encoding: "utf-8", timeout: 30_000, maxBuffer: 64 * 1024 * 1024 },
    );
  } catch (err) {
    process.stderr.write(
      `[pi-harness] ${unit}: sim_schedule.py failed ` +
        `(${err instanceof Error ? err.message : String(err)}) — falling back to claim-order\n`,
    );
    return ranking;
  }
  for (const line of stdout.split("\n")) {
    const trimmed = line.trim();
    if (!trimmed) continue;
    let row: unknown;
    try {
      row = JSON.parse(trimmed);
    } catch {
      continue; // tolerate malformed rows
    }
    const rec = row as {
      targetId?: unknown; score?: unknown; topSiblings?: unknown;
    };
    if (typeof rec.targetId !== "string" || typeof rec.score !== "number") continue;
    const topSiblings: SiblingPointer[] = [];
    if (Array.isArray(rec.topSiblings)) {
      for (const s of rec.topSiblings) {
        const sb = s as { symbol?: unknown; unit?: unknown; status?: unknown };
        if (typeof sb.symbol === "string" && typeof sb.status === "string") {
          topSiblings.push({ symbol: sb.symbol, unit: typeof sb.unit === "string" ? sb.unit : "", status: sb.status });
        }
      }
    }
    ranking.set(rec.targetId, { score: rec.score, topSiblings });
  }
  return ranking;
}

interface TriageData {
  byClass: Map<string, number>;
  clsById: Map<string, string>;
}

/** Run `tools/coop/triage.py --unit <unit> --region <region> --json` ONCE per
 *  TU (read-only classifier; never writes targets.json) and parse its JSONL
 *  rows into targetId -> cls plus a per-class count. Returns empty maps on
 *  any failure — callers fall back to today's behavior (no routing). */
function fetchTriage(
  repoRoot: string,
  unit: string,
  config: HarnessConfig,
): TriageData {
  const data: TriageData = { byClass: new Map(), clsById: new Map() };
  let stdout: string;
  try {
    stdout = execFileSync(
      config.pythonBin,
      ["tools/coop/triage.py", "--unit", unit, "--region", config.region, "--json"],
      { cwd: repoRoot, encoding: "utf-8", timeout: 60_000, maxBuffer: 64 * 1024 * 1024 },
    );
  } catch (err) {
    process.stderr.write(
      `[pi-harness] ${unit}: triage.py failed ` +
        `(${err instanceof Error ? err.message : String(err)}) — no routing for this TU\n`,
    );
    return data;
  }
  for (const line of stdout.split("\n")) {
    const trimmed = line.trim();
    if (!trimmed) continue;
    let row: unknown;
    try {
      row = JSON.parse(trimmed);
    } catch {
      continue; // tolerate malformed rows
    }
    const rec = row as Partial<TriageRow>;
    if (typeof rec.targetId !== "string" || typeof rec.cls !== "string") continue;
    data.clsById.set(rec.targetId, rec.cls);
    data.byClass.set(rec.cls, (data.byClass.get(rec.cls) ?? 0) + 1);
  }
  return data;
}

interface SelectionResult {
  ordered: Target[];
  siblingsByTarget: Map<string, SiblingPointer[]>;
  /** Targets triage classified `regswap_only`: pulled OUT of the LLM batch,
   *  routed to a witness-only singleton (`run.py cycle`, no model session,
   *  no --smt). Empty when triage is off. */
  witnessTargets: Target[];
  /** Per-TU triage summary for the `triage` ledger event; undefined when
   *  triage is off or triage.py produced no rows. */
  triage?: TriageSummary;
}

/** Order the unmatched wave for pass 1.
 *  - "claim-order": unchanged (call-graph wave stays the default).
 *  - "random": plain uniform Fisher-Yates shuffle of the wave.
 *  - "similarity": intersect the sim_schedule ranking with the
 *    call-graph-ready subset (leaf, or every callee FULL_MATCH /
 *    EQUIVALENT_MATCH), re-rank that subset by similarity score, then append
 *    the rest of the wave as fallback so a small ready set never starves a
 *    batch. Also returns per-target sibling pointers for the briefs. */
async function applySelection(
  repoRoot: string,
  unit: string,
  config: HarnessConfig,
  targets: Target[],
): Promise<SelectionResult> {
  // Phase 5 (no-SMT): pre-batch triage classification, once per TU. The
  // classifier is a read-only PREDICTOR — the register-renaming witness
  // (run inside `cycle`) stays the source of truth. When triage is off
  // (default), nothing below changes today's behavior.
  let clsById = new Map<string, string>();
  let triage: TriageSummary | undefined;
  if (config.triage === "route") {
    const t = fetchTriage(repoRoot, unit, config);
    if (t.clsById.size > 0) {
      clsById = t.clsById;
      triage = {
        byClass: {
          regswap_only: t.byClass.get("regswap_only") ?? 0,
          strict: t.byClass.get("strict") ?? 0,
          structural: t.byClass.get("structural") ?? 0,
          unknown: t.byClass.get("unknown") ?? 0,
        },
        routedToWitness: targets
          .filter((x) => clsById.get(x.id) === "regswap_only")
          .map((x) => x.id),
      };
    }
  }

  // Phase 4: selection order on the full wave (unchanged semantics).
  let ordered: Target[];
  let siblingsByTarget = new Map<string, SiblingPointer[]>();
  if (config.selection === "random") {
    const copy = [...targets];
    for (let i = copy.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [copy[i], copy[j]] = [copy[j], copy[i]];
    }
    ordered = copy;
  } else if (config.selection === "similarity") {
    const ranking = fetchSimilarityRanking(repoRoot, unit, config);
    if (ranking.size === 0) {
      ordered = targets; // sim_schedule unavailable -> claim-order
    } else {
      const statusById = targetStatusById(repoRoot);
      const ready: Target[] = [];
      const fallback: Target[] = [];
      for (const t of targets) {
        (isCallGraphReady(t, statusById) ? ready : fallback).push(t);
      }

      // Re-rank the call-graph-ready subset by similarity; unranked targets
      // (e.g. in-progress statuses sim_schedule skips) sort to the back of
      // the ready subset. Tie-break by id for determinism.
      ready.sort((a, b) => {
        const sa = ranking.get(a.id)?.score ?? -1;
        const sb = ranking.get(b.id)?.score ?? -1;
        return sb - sa || a.id.localeCompare(b.id);
      });

      // Sibling pointers attach to every ranked target in the wave (ready
      // and fallback alike) — they are hints, not readiness claims.
      for (const t of targets) {
        const rank = ranking.get(t.id);
        if (rank && rank.topSiblings.length > 0) siblingsByTarget.set(t.id, rank.topSiblings);
      }
      ordered = [...ready, ...fallback];
    }
  } else {
    ordered = targets;
  }

  // Phase 5 routing: pull `regswap_only` targets OUT of the LLM batch into
  // the witness-only singleton path, and front-load `strict` targets (the
  // highest LLM hit rate). When triage is off, `ordered` is untouched.
  let witnessTargets: Target[] = [];
  if (triage) {
    witnessTargets = targets.filter((x) => clsById.get(x.id) === "regswap_only");
    const witnessIds = new Set(witnessTargets.map((x) => x.id));
    const rest = ordered.filter((x) => !witnessIds.has(x.id));
    const strict = rest.filter((x) => clsById.get(x.id) === "strict");
    const other = rest.filter((x) => clsById.get(x.id) !== "strict");
    ordered = [...strict, ...other];
  }

  return { ordered, siblingsByTarget, witnessTargets, triage };
}


function usageFromError(err: unknown): SessionUsage | null {
  const usage = (err as { usage?: unknown })?.usage;
  if (!usage || typeof usage !== "object") return null;
  const u = usage as Partial<SessionUsage>;
  return {
    input: u.input ?? 0,
    output: u.output ?? 0,
    cacheRead: u.cacheRead ?? 0,
    cacheWrite: u.cacheWrite ?? 0,
  };
}

function logUsage(
  repoRoot: string,
  config: HarnessConfig,
  unit: string,
  label: string,
  usage: SessionUsage,
  timedOut: boolean,
): void {
  appendLedger(repoRoot, config.ledgerPath, {
    ts: new Date().toISOString(),
    event: "session-usage",
    tu: unit,
    detail: { label, timedOut, ...usage },
  });
  process.stderr.write(
    `[pi-harness] ${unit} ${label}: tokens in=${usage.input.toLocaleString()} ` +
      `out=${usage.output.toLocaleString()} cacheRead=${usage.cacheRead.toLocaleString()}\n`,
  );
}

async function claimBatch(
  repoRoot: string, config: HarnessConfig, ids: string[],
): Promise<void> {
  const result = await claimTargets(
    repoRoot, config.pythonBin, ids, OWNER,
    (id) => activeClaims.add(id),
  );
  if (result.failed.length > 0) {
    await releaseBatch(repoRoot, config, result.ok);
    throw new Error(`claim failed for target(s): ${result.failed.join(", ")}`);
  }
}

async function releaseBatch(
  repoRoot: string, config: HarnessConfig, ids: string[],
): Promise<void> {
  await releaseTargets(repoRoot, config.pythonBin, ids, OWNER);
  for (const id of ids) activeClaims.delete(id);
}

/**
 * Phase 5 (no-SMT): witness-only singleton. Runs `run.py cycle <id>` under
 * the repo-wide build lock — NO model session and NO `--smt` (cycle's
 * default is the cheap register-renaming witness, which certifies
 * position-aligned same-mnemonic pairs whose diffs are register-only).
 *
 * The witness is the source of truth: exit 0 means the required level is
 * met, but a non-zero exit can still have flipped the registry (e.g. the
 * unit split-size gate failed after the function itself certified), so
 * targets.json is re-checked either way. Returns true when the target is
 * now FULL_MATCH / EQUIVALENT_MATCH; false -> the caller re-adds it to the
 * LLM batch pool (never dropped).
 */
async function runWitnessCycle(
  repoRoot: string, unit: string, targetId: string, config: HarnessConfig,
): Promise<boolean> {
  console.log(`[pi-harness] ${unit}: witness-only cycle ${targetId} (no model session, no --smt)`);
  const claim = await claimTargets(repoRoot, config.pythonBin, [targetId], OWNER, (id) => activeClaims.add(id));
  if (claim.failed.length > 0) {
    process.stderr.write(
      `[pi-harness] ${unit}: ${targetId} claim failed for witness cycle — falling back to batch\n`,
    );
    return false;
  }
  let certified = false;
  try {
    try {
      await execFilePromise(config.pythonBin, [
        "tools/pi_harness/build_lock.py", "--timeout", "1800", config.region, "--",
        config.pythonBin, "tools/coop/run.py", "cycle", targetId,
        "--hypothesis",
        "triage: reg-swap template vs nearest matched sibling — witness-only route (no LLM round)",
        "--next-change",
        "accept if the register-renaming witness certifies; otherwise fall back to the LLM batch",
      ], { cwd: repoRoot });
      certified = true; // cycle exit 0 = required level met
    } catch (err) {
      // Non-zero exit: witness may not have certified, or the unit-level
      // split size gate failed. The registry records actual acceptance —
      // re-check instead of trusting the exit code alone.
      const status = targetStatusById(repoRoot).get(targetId);
      if (status === "FULL_MATCH" || status === "EQUIVALENT_MATCH") {
        certified = true;
      } else {
        process.stderr.write(
          `[pi-harness] ${unit}: witness did not certify ${targetId} ` +
            `(status ${status ?? "UNKNOWN"}) — re-added to batch pool\n`,
        );
      }
    }
  } finally {
    await releaseTargets(repoRoot, config.pythonBin, [targetId], OWNER);
    activeClaims.delete(targetId);
  }
  return certified;
}

// ─────────────────────────────────────────────────────────────────────
//  Shared onVerify factory
// ─────────────────────────────────────────────────────────────────────

interface VerifySession {
  onVerify: (finalText: string, timedOut: boolean, rePromptCount: number) => Promise<VerifyResult>;
  getResults: () => Awaited<ReturnType<typeof runBatchCycle>> | null;
  /** Clean up per-phase snapshots; does NOT restore the original. */
  cleanup: () => Promise<void>;
  /** Last rejection feedback (for carryover to fresh sessions). */
  lastFeedback: () => string | undefined;
}

function makeVerifyCallback(opts: {
  repoRoot: string;
  config: HarnessConfig;
  unit: string;
  targetIds: string[];
  targetSymbols: Map<string, string>;
  writable: string[];
  snapshot: Snapshot;
}): VerifySession {
  const { repoRoot, config, unit, targetIds, targetSymbols, writable, snapshot } = opts;
  let compilableSnapshot: Snapshot | null = null;
  let batchResults: Awaited<ReturnType<typeof runBatchCycle>> | null = null;
  let lastFeedback: string | undefined;

  // Phase 2/3 state: per-target best divergence seen THIS session + how many
  // consecutive no-match rounds it has failed to improve (stale-round
  // early-stop). structuralCount is the codegen-shape signal (reg_swap is
  // noise for the SMT-equivalence acceptance model).
  const sessionBest = new Map<string, { structural: number; mismatch: number }>();
  const staleRounds = new Map<string, number>();

  /** Best-effort bank of a failed target's compiling draft (whole-file). */
  const bankTarget = async (tid: string, status: string | undefined, round: number): Promise<void> => {
    const sym = targetSymbols.get(tid);
    if (!sym) return;
    try {
      // allowBuildRetry: the accept-path runs right after batch-cycle's
      // per-target rebuilds, so the --no-build read can race a mid-write
      // object — retry with a build before giving up on banking.
      const hd = await runHexdiff(repoRoot, config.pythonBin, unit, sym, { allowBuildRetry: true });
      if (!hd.ok) return; // hexdiff failed — never bank on a blind draft
      await bankDraft(
        repoRoot, config, unit, tid, writable,
        status, hd.structuralCount, hd.mismatchCount, undefined, round,
      );
      const prev = sessionBest.get(tid);
      if (!prev || hd.structuralCount < prev.structural) {
        sessionBest.set(tid, { structural: hd.structuralCount, mismatch: hd.mismatchCount });
      }
    } catch {
      // banking is best-effort — never fail the session over it
    }
  };

  const onVerify = async (
    _finalText: string,
    timedOut: boolean,
    rePromptCount: number,
  ): Promise<VerifyResult> => {
    process.stderr.write(`[orchestrator] ${unit}: onVerify starting (rePromptCount=${rePromptCount})\n`);
    // ── Build check ──
    process.stderr.write(`[orchestrator] ${unit}: buildUnit starting\n`);
    const build = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
    process.stderr.write(`[orchestrator] ${unit}: buildUnit completed (ok=${build.ok})\n`);
    if (!build.ok) {
      // If we have a compilable checkpoint (from prior lint pass that
      // later broke), restore to it and tell the model what happened.
      if (compilableSnapshot) {
        await restoreSnapshot(repoRoot, compilableSnapshot);
        compilableSnapshot = null;
        lastFeedback =
          `## Build Failure — Lint fix reverted\n\n` +
          `Your lint fix broke compilation. The code has been reverted ` +
          `to the last compilable state (which still has lint violations). ` +
          `Re-apply the lint fix without breaking compilation.\n\n` +
          `Build error from the broken attempt:\n\`\`\`\n${build.output}\n\`\`\``;
        return { action: "re-prompt", feedback: lastFeedback };
      }
      // No checkpoint: keep broken code in place so the model can fix
      // it with full context.
      lastFeedback =
        `## Build Failure — Fix This First\n\n` +
        `Your code does not compile. Fix the errors below before ` +
        `continuing with any unmatched targets.\n\n` +
        `\`\`\`\n${build.output}\n\`\`\``;
      return { action: "re-prompt", feedback: lastFeedback };
    }

    // Build passed — clear any stale compilable checkpoint.
    if (compilableSnapshot) {
      compilableSnapshot = null;
    }

    // ── Acceptance (match check) — run before lint so we never
    //    skip match evaluation due to lint-related re-prompt caps. ──
    process.stderr.write(`[orchestrator] ${unit}: runBatchCycle starting\n`);
    batchResults = await runBatchCycle(repoRoot, config.pythonBin, config.region, targetIds);
    const acceptedCount = batchResults.filter(r => r.accepted).length;
    process.stderr.write(`[orchestrator] ${unit}: runBatchCycle completed (${acceptedCount} accepted)\n`);
    if (acceptedCount > 0) {
      // Partial accept: bank the FAILED targets too — their drafts are in
      // the worktree right now and would otherwise die unbanked (Phase 2).
      for (const r of batchResults) {
        if (!r.accepted) await bankTarget(r.targetId, r.status, rePromptCount);
      }
      return { action: "accept" };
    }

    // ── Lint check ──
    process.stderr.write(`[orchestrator] ${unit}: runLint starting\n`);
    const lint = await runLint(repoRoot, config.pythonBin, snapshot);
    process.stderr.write(`[orchestrator] ${unit}: runLint completed (ok=${lint.ok})\n`);
    if (!lint.ok) {
      // Save compilable checkpoint before re-prompting for lint fix.
      compilableSnapshot = await snapshotUnit(repoRoot, unit, writable);
      lastFeedback =
        `## Lint Failure — Fix This First\n\n` +
        `Your code compiles but violates these rules. Fix ONLY the ` +
        `violations — do not make any other changes.\n\n` +
        `\`\`\`json\n${JSON.stringify(lint.violations, null, 2)}\n\`\`\``;
      return { action: "re-prompt", feedback: lastFeedback };
    }

    // ── Cap check: build passed but no match yet. Only reject on
    //    re-prompt budget exhaustion. ──
    const effectiveMax = timedOut ? config.timeoutRetries : config.rejectionRetries;
    if (rePromptCount >= effectiveMax) {
      lastFeedback = `re-prompt cap hit (${effectiveMax})`;
      return { action: "fail", reason: lastFeedback };
    }

    // Haven't hit cap yet — re-prompt with hexdiff feedback, banking each
    // failed target's compiling draft BEFORE the caller restores the
    // snapshot (Phase 2), and early-stopping stale rounds (Phase 3).
    const hexdiffOutputs: string[] = [];
    // Phase 2 triage: track reg-swap-only targets (structural=0, mismatch>0) —
    // code structure is right, only register allocation differs; the witness
    // path (batch-cycle runs it every round) may certify without more edits.
    const regSwapOnlyTargets: string[] = [];
    for (const tid of targetIds) {
      const sym = targetSymbols.get(tid);
      if (!sym) continue;
      const hd = await runHexdiff(repoRoot, config.pythonBin, unit, sym);
      if (hd.ok) {
        hexdiffOutputs.push(`### ${tid} (\`${sym}\`) — ${hd.mismatchCount} mismatch(es)\n${hd.output}`);
        if (hd.structuralCount === 0 && hd.mismatchCount > 0) {
          regSwapOnlyTargets.push(tid);
        }
        // Bank the compiling draft (status from batch-cycle results).
        const status = batchResults?.find((r) => r.targetId === tid)?.status;
        await bankTarget(tid, status, rePromptCount);
        // Stale-round tracking: no structural improvement for N rounds ->
        // stop THIS target's loop (cross-session budget still governs retries).
        const prev = sessionBest.get(tid);
        if (prev && hd.structuralCount >= prev.structural) {
          staleRounds.set(tid, (staleRounds.get(tid) ?? 0) + 1);
        } else {
          staleRounds.set(tid, 0);
        }
        if ((staleRounds.get(tid) ?? 0) >= config.staleRoundThreshold) {
          process.stderr.write(
            `[pi-harness] stale-round early-stop for ${tid} (structural stuck at ${hd.structuralCount} for ${staleRounds.get(tid)} round(s))\n`,
          );
          lastFeedback =
            `Target ${tid} has not improved its divergence for ${staleRounds.get(tid)} round(s) — ` +
            `stopping this attempt (structural=${hd.structuralCount}, mismatch=${hd.mismatchCount}). ` +
            `Its best draft is banked; a later pass will refine from it.`;
          return { action: "fail", reason: lastFeedback };
        }
      } else {
        process.stderr.write(`[pi-harness] hexdiff failed for ${tid}: ${hd.output.slice(0, 200)}\n`);
      }
    }
    lastFeedback =
      `## No Matches Found\n\n` +
      `Your code compiled and passed lint, but does not match the retail binary.` +
      (hexdiffOutputs.length > 0
        ? `\n\nHexdiff output:\n\n${hexdiffOutputs.join("\n\n")}`
        : `\n\n(hexdiff unavailable — review the assembly patterns carefully)`) +
      `\n\nCommon causes:\n` +
      `- Wrong field offset or struct type\n` +
      `- Expression order (affects MWCC register allocation)\n` +
      `- Signed vs unsigned comparison\n` +
      `- Missing or extra function calls`;
    if (regSwapOnlyTargets.length > 0) {
      lastFeedback += `\n\n### Reg-swap-only (structure correct)\n\n` +
        `These targets have structural=0 — the code shape is right and only ` +
        `register allocation differs: ${regSwapOnlyTargets.join(", ")}. The ` +
        `register-renaming witness may certify them without further edits. If ` +
        `they were not accepted this round, try ONE small structural nudge ` +
        `(declaration order / statement order) and stop — do not grind.`;
    }
    return { action: "re-prompt", feedback: lastFeedback };
  };

  const cleanup = async (): Promise<void> => {
    // compilableSnapshot is a temp directory; nothing we can easily
    // delete, but we null the ref so it's not mistakenly used later.
    compilableSnapshot = null;
  };

  return {
    onVerify,
    getResults: () => batchResults,
    cleanup,
    lastFeedback: () => lastFeedback,
  };
}

// ─────────────────────────────────────────────────────────────────────
//  Per-TU worker
// ─────────────────────────────────────────────────────────────────────

async function runOneTu(
  repoRoot: string,
  unit: string,
  config: HarnessConfig,
  modelRuntime: ModelRuntime,
  dryRun: boolean,
): Promise<void> {
  const sanitized = unit.replace(/\//g, "__");
  const targetsRaw = loadUnmatchedTargets(repoRoot, config.region, unit, {
    ledgerPath: config.ledgerPath, retryExhausted: config.retryExhausted,
  });
  const selection = await applySelection(repoRoot, unit, config, targetsRaw);
  let targets = selection.ordered;
  const siblingsByTarget = selection.siblingsByTarget;

  // Phase 5 (no-SMT): route triage `regswap_only` targets to witness-only
  // cycles BEFORE any LLM session. Each is one `run.py cycle` under the
  // build lock (no model session, no --smt): the register-renaming witness
  // either certifies EQUIVALENT_MATCH / FULL_MATCH with ZERO tokens, or
  // fails cheaply and the target goes back into the batch pool below
  // (never dropped). Skipped in dry-run — witness cycles build objects and
  // update the registry.
  const witnessTargets = selection.witnessTargets ?? [];
  const witnessFallback: string[] = [];
  if (selection.triage && !dryRun) {
    if (witnessTargets.length > 0) {
      console.log(`[pi-harness] ${unit}: triage routed ${witnessTargets.length} regswap_only target(s) to witness-only cycle`);
      for (const wt of witnessTargets) {
        const ok = await runWitnessCycle(repoRoot, unit, wt.id, config);
        if (!ok) witnessFallback.push(wt.id);
      }
    }
    appendLedger(repoRoot, config.ledgerPath, {
      ts: new Date().toISOString(), event: "triage", tu: unit,
      detail: {
        unit,
        byClass: selection.triage.byClass,
        routedToWitness: selection.triage.routedToWitness,
        certified: witnessTargets.filter((w) => !witnessFallback.includes(w.id)).map((w) => w.id),
        fallback: witnessFallback,
      },
    });
    if (witnessFallback.length > 0) {
      // Never dropped: re-add failed witness candidates to the FRONT of the
      // batch pool — they were strong template candidates, so the LLM has a
      // good shot and they stay ahead of the rest of the wave.
      const fallbackTargets = targetsRaw.filter((t) => witnessFallback.includes(t.id));
      targets = [...fallbackTargets, ...targets.filter((t) => !witnessFallback.includes(t.id))];
      console.log(`[pi-harness] ${unit}: witness did not certify ${witnessFallback.length} target(s) — re-added to batch front`);
    }
  } else if (dryRun && witnessTargets.length > 0) {
    console.log(`[pi-harness] ${unit}: DRY-RUN: ${witnessTargets.length} regswap_only target(s) would route to witness-only cycle (no model session)`);
  }

  if (targets.length === 0) {
    const entries = readLedger(repoRoot, config.ledgerPath).filter((e) => e.tu === unit);
    const wasWorked = entries.some(
      (e) => e.event === "batch-accept" || e.event === "tu-started",
    ) || witnessTargets.length > witnessFallback.length; // witness certified >=1
    if ((wasWorked || dryRun) && loadUnitTargets(repoRoot, config.region, unit).length > 0) {
      await queueTuFinal(repoRoot, unit, config, modelRuntime, dryRun, sanitized);
      return;
    }
    console.log(`[pi-harness] ${unit}: nothing to do (0 unmatched, never worked)`);
    return;
  }

  console.log(`[pi-harness] ${unit}: ${targets.length} unmatched target(s)`);
  if (!dryRun) {
    appendLedger(repoRoot, config.ledgerPath, {
      ts: new Date().toISOString(),
      event: "tu-started",
      tu: unit,
      detail: { unmatchedCount: targets.length, targetIds: targets.map((t) => t.id) },
    });
  }

  let carryover = "";
  const allIds = targets.map((t) => t.id);
  const smallRetryPool: string[] = [];
  const firstPassFailed: string[] = [];
  const targetCarryover = new Map<string, string>(); // per-target carryover for pass 2

  // ── Pass 1: Run all batches with 1 attempt each ─────────────────
  console.log(`[pi-harness] ${unit}: Pass 1 — running ${Math.ceil(allIds.length / config.batchSize)} batches`);

  for (let batchIndex = 0; batchIndex < allIds.length; batchIndex += config.batchSize) {
    const batchIds = allIds.slice(batchIndex, batchIndex + config.batchSize);
    const batchTargets = targets.filter((t) => batchIds.includes(t.id));

    const { briefs: rawBriefs, missingAsm } = buildBriefs(repoRoot, batchTargets, siblingsByTarget);
    const briefs = await attachBankedDrafts(repoRoot, config, rawBriefs);
    for (const id of missingAsm) {
      if (!dryRun) handleSkipped(repoRoot, config, unit, id, "no retail asm");
    }
    const briefTargets = batchTargets.filter((t) => !missingAsm.includes(t.id));
    const writable = writableScopeForTargets(repoRoot, briefTargets);
    const currentIds = batchIds.filter((id) => !missingAsm.includes(id));

    if (currentIds.length === 0) continue;

    const brief = buildBatchBrief({
      targets: briefs, unit, writable, carryover,
      maxChars: config.maxBriefChars, pythonBin: config.pythonBin,
      repoRoot, knownWallsPath: config.knownWallsPath,
      briefTargetChars: config.briefTargetChars,
    });
    const prompt = buildBatchPrompt({
      brief, unit, targetIds: currentIds, pythonBin: config.pythonBin,
    });

    if (dryRun) {
      console.log(`\n[pi-harness] DRY-RUN: unit=${unit}, batch=${batchIndex}`);
      console.log(`  targets: ${currentIds.join(", ")}`);
      const lines = prompt.split("\n");
      console.log(lines.slice(0, 40).join("\n"));
      if (lines.length > 40) console.log(`  … (${lines.length - 40} more lines)`);
      continue;
    }

    let snapshot: Snapshot | null = null;
    let attemptError: string | null = null;
    let batchResults: Awaited<ReturnType<typeof runBatchCycle>> | null = null;

    try {
      await claimBatch(repoRoot, config, currentIds);
      snapshot = await snapshotUnit(repoRoot, unit, writable);

      const batchTargetSymbols = new Map(briefTargets.map(t => [t.id, t.symbol]));
      const verify = makeVerifyCallback({
        repoRoot, config, unit, targetIds: currentIds,
        targetSymbols: batchTargetSymbols, writable, snapshot,
      });

      const sessionResult = await runAgentSession({
        repoRoot,
        modelRuntime,
        spec: config.matchModel,
        prompt,
        sessionDir: join(config.sessionDir, sanitized, `batch-${batchIndex}`),
        label: `batch-${batchIndex}-session-1`,
        python: config.pythonBin,
        timeoutMinutes: config.maxBatchMinutes,
        maxTokens: config.maxTokens,
        multiPrompt: {
          timeoutRetries: config.timeoutRetries,
          rejectionRetries: config.rejectionRetries,
          onVerify: verify.onVerify,
          totalTimeoutMinutes: config.maxBatchMinutes * (1 + config.timeoutRetries),
        },
      });

      // Log per-round usage.
      if (sessionResult.roundUsages) {
        for (let r = 0; r < sessionResult.roundUsages.length; r++) {
          logUsage(
            repoRoot, config, unit,
            `batch-${batchIndex}-session-1-round-${r}`,
            sessionResult.roundUsages[r],
            r === sessionResult.roundUsages.length - 1 && sessionResult.timedOut,
          );
        }
      } else {
        logUsage(repoRoot, config, unit, `batch-${batchIndex}-session-1`,
          sessionResult.usage, sessionResult.timedOut);
      }

      if (sessionResult.outcome === "accepted") {
        batchResults = verify.getResults();
      } else {
        // Session gave up or failed — restore snapshot, build carryover
        // from the last rejection feedback.
        await verify.cleanup();
        if (snapshot) await restoreSnapshot(repoRoot, snapshot);

        const rejectionReason = sessionResult.lastRejection
          ?? verify.lastFeedback()
          ?? `${sessionResult.outcome} after ${sessionResult.rePromptsUsed} re-prompt(s)`;
        const batchCarryover = `Previous session failed (${sessionResult.outcome}). ${rejectionReason}`;
        carryover = batchCarryover;
        // Store per-target carryover for pass 2
        for (const id of currentIds) {
          targetCarryover.set(id, batchCarryover);
        }

        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-session-exhausted", tu: unit,
          detail: {
            batchIndex, attempt: 1, outcome: sessionResult.outcome,
            rePromptsUsed: sessionResult.rePromptsUsed,
            lastRejection: sessionResult.lastRejection,
          },
        });
        await releaseBatch(repoRoot, config, currentIds);
        firstPassFailed.push(...currentIds);
        continue;
      }
    } catch (err) {
      attemptError = err instanceof Error ? err.message : String(err);
      const usage = usageFromError(err);
      if (usage) {
        logUsage(repoRoot, config, unit, `batch-${batchIndex}-session-1`, usage, false);
      }
    }

    if (attemptError !== null && batchResults === null) {
      let recoveryOutput = "";
      if (snapshot) {
        const recovery = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
        recoveryOutput = recovery.output;
        if (!recovery.ok) await restoreSnapshot(repoRoot, snapshot);
      }
      const batchCarryover = `Previous attempt failed unexpectedly:\n${attemptError}\n${recoveryOutput}`;
      carryover = batchCarryover;
      // Store per-target carryover for pass 2
      for (const id of currentIds) {
        targetCarryover.set(id, batchCarryover);
      }
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "batch-error", tu: unit,
        detail: { batchIndex, attempt: 1, error: attemptError, recoveryOutput },
      });
      await releaseBatch(repoRoot, config, currentIds);
      firstPassFailed.push(...currentIds);
      continue;
    }

    if (batchResults === null) {
      const batchCarryover = "Previous attempt produced no results (internal error).";
      carryover = batchCarryover;
      // Store per-target carryover for pass 2
      for (const id of currentIds) {
        targetCarryover.set(id, batchCarryover);
      }
      await releaseBatch(repoRoot, config, currentIds);
      firstPassFailed.push(...currentIds);
      continue;
    }

    // ── Acceptance results ──────────────────────────────────────
    const acceptedCount = batchResults.filter((r) => r.accepted).length;
    appendLedger(repoRoot, config.ledgerPath, {
      ts: new Date().toISOString(),
      event: acceptedCount > 0 ? "batch-accept" : "batch-cycle",
      tu: unit,
      detail: {
        batchIndex, attempt: 1, acceptedCount,
        results: batchResults.map((r) => ({ targetId: r.targetId, status: r.status })),
      },
    });
    await releaseBatch(repoRoot, config, currentIds);

    // Store per-target carryover for failed targets BEFORE clearing.
    const failedIds = batchResults.filter((r) => !r.accepted).map((r) => r.targetId);
    firstPassFailed.push(...failedIds);
    for (const id of failedIds) {
      targetCarryover.set(id, carryover);
    }

    // Clear carryover on success — don't leak stale failure text to subsequent batches
    if (acceptedCount > 0) {
      carryover = "";
    }
  }

  // ── Pass 2: Retry failed targets ──────────────────────────────
  if (firstPassFailed.length > 0 && !dryRun) {
    console.log(`[pi-harness] ${unit}: Pass 2 — retrying ${firstPassFailed.length} failed targets`);

    for (const fid of firstPassFailed) {
      // Check if already matched (might have been matched in a previous batch)
      const target = targets.find((t) => t.id === fid);
      if (!target) continue;

      if (config.singletonMinSize > 0) {
        // Route small targets to rebatch pool based on retail binary size.
        const targetSize = target.size ?? 0;
        if (targetSize > 0 && targetSize < config.singletonMinSize) {
          smallRetryPool.push(fid);
          continue;
        }
      }

      // Each failed target gets its own singleton budget
      if (!config.singletonEnabled) {
        handleSkipped(repoRoot, config, unit, fid, "singleton retry disabled");
        continue;
      }
      const ok = await runSingleton(
        repoRoot, unit, fid, config, modelRuntime, sanitized,
        targetCarryover.get(fid) || carryover, // Use per-target carryover if available
        config.maxBatchRetries,
        siblingsByTarget,
      );
      if (!ok) {
        handleSkipped(repoRoot, config, unit, fid, "exhausted singleton retries");
      }
    }
  }

  // ── Re-batch phase for small failed targets ─────────────────────
  if (smallRetryPool.length > 0 && !dryRun) {
    const { skipped: rebatchSkipped, targetCarryover: rebatchCarryover } = await runRebatchPhase(
      repoRoot, unit, config, modelRuntime, sanitized,
      smallRetryPool, targets, carryover, config.maxRebatchAttempts,
      siblingsByTarget,
    );
    // Merge rebatch carryover into the main map for singleton routing.
    for (const [id, fb] of rebatchCarryover) {
      targetCarryover.set(id, fb);
    }
    // Route targets that rebatch didn't process (disabled, budget
    // exhausted, or failed) to singletons or skip.
    for (const fid of rebatchSkipped) {
      if (!config.singletonEnabled) {
        handleSkipped(repoRoot, config, unit, fid, "singleton retry disabled");
        continue;
      }
      const ok = await runSingleton(
        repoRoot, unit, fid, config, modelRuntime, sanitized,
        targetCarryover.get(fid) || carryover,
        config.maxBatchRetries,
        siblingsByTarget,
      );
      if (!ok) {
        handleSkipped(repoRoot, config, unit, fid, "exhausted singleton retries");
      }
    }
  }

  // ── TU-final phase ────────────────────────────────────────────────
  if (dryRun) {
    console.log(`[pi-harness] ${unit}: dry-run complete (no sessions run, no acceptance)`);
    return;
  }

  const remaining = loadUnmatchedTargets(repoRoot, config.region, unit, {
    ledgerPath: config.ledgerPath, retryExhausted: config.retryExhausted,
  });
  if (remaining.length === 0) {
    await queueTuFinal(repoRoot, unit, config, modelRuntime, dryRun, sanitized);
  } else {
    // Phase 3 escape: if every remaining target is ledger-exhausted (budget
    // spent, not just un-attempted), TU-final is still worthwhile — the
    // exhaustions are stable, so running the finalization pass can't regress
    // them and unblocks the unit's data/rename/comment work. Without this,
    // early-stop on the last few targets deadlocks the TU forever.
    const exhaustedAll = remaining.every((t) => countLedgerSessions(repoRoot, config, t.id) >= config.maxAttemptsPerTarget);
    if (exhaustedAll) {
      process.stderr.write(
        `[pi-harness] ${unit}: all ${remaining.length} remaining target(s) are ledger-exhausted — running TU-final anyway\n`,
      );
      await queueTuFinal(repoRoot, unit, config, modelRuntime, dryRun, sanitized);
      return;
    }
    appendLedger(repoRoot, config.ledgerPath, {
      ts: new Date().toISOString(), event: "tu-incomplete", tu: unit,
      detail: { remainingCount: remaining.length, stuckIds: remaining.map((t) => t.id) },
    });
    console.log(`[pi-harness] ${unit}: ${remaining.length} target(s) remain unmatched after retries`);
    for (const t of remaining) console.log(`  - ${t.id} (${t.status})`);
  }
}

// ─────────────────────────────────────────────────────────────────────
//  Re-batch phase
// ─────────────────────────────────────────────────────────────────────

/**
 * Run rebatch phase for small failed targets with a per-TU session budget.
 *
 * @param maxAttempts Per-TU rebatch session budget. 0 = no rebatch attempts.
 * @returns Skipped target IDs and per-target carryover for singleton routing.
 */
async function runRebatchPhase(
  repoRoot: string, unit: string, config: HarnessConfig,
  modelRuntime: ModelRuntime, sanitized: string,
  smallRetryPool: string[], targets: Target[], carryover: string,
  maxAttempts: number, siblingsByTarget?: Map<string, SiblingPointer[]>,
): Promise<{ skipped: string[]; targetCarryover: Map<string, string> }> {
  const uniqueSmall = [...new Set(smallRetryPool)];
  const targetCarryover = new Map<string, string>();

  if (!config.rebatchEnabled) {
    console.log(`[pi-harness] ${unit}: rebatch disabled — ${uniqueSmall.length} small target(s) routed to singletons`);
    return { skipped: uniqueSmall, targetCarryover };
  }

  if (maxAttempts <= 0) {
    console.log(`[pi-harness] ${unit}: rebatch budget is 0 — ${uniqueSmall.length} small target(s) routed to singletons`);
    return { skipped: uniqueSmall, targetCarryover };
  }

  console.log(
    `[pi-harness] ${unit}: re-batching ${uniqueSmall.length} small target(s) ` +
    `(below ${config.singletonMinSize} bytes, budget: ${maxAttempts} session(s))`,
  );

  const smallTargets = targets.filter((t) => uniqueSmall.includes(t.id));
  let sharedCarryover = carryover;
  let rebatchBudget = maxAttempts;
  const skipped: string[] = [];

  for (let rbIdx = 0; rbIdx < uniqueSmall.length; rbIdx += config.batchSize) {
    if (rebatchBudget <= 0) {
      // Budget exhausted — remaining targets go to caller.
      for (const id of uniqueSmall.slice(rbIdx)) {
        targetCarryover.set(id, sharedCarryover);
      }
      skipped.push(...uniqueSmall.slice(rbIdx));
      break;
    }

    const rbIds = uniqueSmall.slice(rbIdx, rbIdx + config.batchSize);
    const rbTargets = smallTargets.filter((t) => rbIds.includes(t.id));

    const { briefs: rbRawBriefs, missingAsm: rbMissing } = buildBriefs(repoRoot, rbTargets, siblingsByTarget);
    const rbBriefs = await attachBankedDrafts(repoRoot, config, rbRawBriefs);
    for (const id of rbMissing) {
      handleSkipped(repoRoot, config, unit, id, "no retail asm");
    }
    const rbFiltered = rbIds.filter((id) => !rbMissing.includes(id));
    if (rbFiltered.length === 0) continue;

    const writable = writableScopeForTargets(
      repoRoot,
      rbTargets.filter((t) => !rbMissing.includes(t.id)),
    );
    let rbCurrent = rbFiltered;

    while (rbCurrent.length > 0 && rebatchBudget > 0) {
      const brief = buildBatchBrief({
        targets: rbBriefs, unit, writable, carryover: sharedCarryover,
        maxChars: config.maxBriefChars, pythonBin: config.pythonBin,
        repoRoot, knownWallsPath: config.knownWallsPath,
        briefTargetChars: config.briefTargetChars,
      });
      const prompt = buildBatchPrompt({
        brief, unit, targetIds: rbCurrent, pythonBin: config.pythonBin,
      });

      let snapshot: Snapshot | null = null;
      let attemptError: string | null = null;
      let batchResults: Awaited<ReturnType<typeof runBatchCycle>> | null = null;

      try {
        await claimBatch(repoRoot, config, rbCurrent);
        snapshot = await snapshotUnit(repoRoot, unit, writable);
        rebatchBudget--; // Only decrement after claim+snapshot succeed.

        const rbTargetSymbols = new Map(rbTargets.map(t => [t.id, t.symbol]));
        const verify = makeVerifyCallback({
          repoRoot, config, unit, targetIds: rbCurrent,
          targetSymbols: rbTargetSymbols, writable, snapshot,
        });

        const sessionResult = await runAgentSession({
          repoRoot, modelRuntime, spec: config.matchModel, prompt,
          sessionDir: join(config.sessionDir, sanitized, `rebatch-${rbIdx}`),
          label: `rebatch-${rbIdx}-session`,
          python: config.pythonBin,
          timeoutMinutes: config.maxBatchMinutes,
          maxTokens: config.maxTokens,
          multiPrompt: {
            timeoutRetries: config.timeoutRetries,
            rejectionRetries: config.rejectionRetries,
            onVerify: verify.onVerify,
            totalTimeoutMinutes: config.maxBatchMinutes * (1 + config.timeoutRetries),
          },
        });

        if (sessionResult.roundUsages) {
          for (let r = 0; r < sessionResult.roundUsages.length; r++) {
            logUsage(repoRoot, config, unit, `rebatch-${rbIdx}-round-${r}`,
              sessionResult.roundUsages[r],
              r === sessionResult.roundUsages.length - 1 && sessionResult.timedOut);
          }
        } else {
          logUsage(repoRoot, config, unit, `rebatch-${rbIdx}`,
            sessionResult.usage, sessionResult.timedOut);
        }

        if (sessionResult.outcome === "accepted") {
          batchResults = verify.getResults();
        } else {
          await verify.cleanup();
          if (snapshot) await restoreSnapshot(repoRoot, snapshot);

          const rejectionReason = sessionResult.lastRejection
            ?? verify.lastFeedback()
            ?? `${sessionResult.outcome} after ${sessionResult.rePromptsUsed} re-prompt(s)`;
          sharedCarryover = `Previous re-batch session failed (${sessionResult.outcome}). ${rejectionReason}`;
          for (const id of rbCurrent) targetCarryover.set(id, sharedCarryover);

          appendLedger(repoRoot, config.ledgerPath, {
            ts: new Date().toISOString(), event: "batch-session-exhausted", tu: unit,
            detail: {
              batchIndex: rbIdx, phase: "rebatch",
              outcome: sessionResult.outcome, rePromptsUsed: sessionResult.rePromptsUsed,
              lastRejection: sessionResult.lastRejection,
              rebatchBudgetRemaining: rebatchBudget,
            },
          });
          await releaseBatch(repoRoot, config, rbCurrent);
          continue;
        }
      } catch (err) {
        attemptError = err instanceof Error ? err.message : String(err);
        const usage = usageFromError(err);
        if (usage) {
          logUsage(repoRoot, config, unit, `rebatch-${rbIdx}`, usage, false);
        }
        // Error before session ran — restore budget.
        if (batchResults === null && attemptError !== null) {
          rebatchBudget++;
        }
      }

      if (attemptError !== null && batchResults === null) {
        let recoveryOutput = "";
        if (snapshot) {
          const recovery = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
          recoveryOutput = recovery.output;
          if (!recovery.ok) await restoreSnapshot(repoRoot, snapshot);
        } else {
          // Snapshot not taken yet — restore pre-loop state.
          await restoreSnapshot(repoRoot, await snapshotUnit(repoRoot, unit, writable));
        }
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-error", tu: unit,
          detail: { batchIndex: rbIdx, error: attemptError, recoveryOutput, phase: "rebatch", rebatchBudgetRemaining: rebatchBudget },
        });
        await releaseBatch(repoRoot, config, rbCurrent);
        sharedCarryover = `Previous re-batch attempt failed unexpectedly:\n${attemptError}\n${recoveryOutput}`;
        for (const id of rbCurrent) targetCarryover.set(id, sharedCarryover);
        continue;
      }

      if (batchResults === null) {
        await releaseBatch(repoRoot, config, rbCurrent);
        sharedCarryover = "Previous re-batch attempt produced no results.";
        for (const id of rbCurrent) targetCarryover.set(id, sharedCarryover);
        continue;
      }

      const acceptedCount = batchResults.filter((r) => r.accepted).length;
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(),
        event: acceptedCount > 0 ? "batch-accept" : "batch-cycle",
        tu: unit,
        detail: {
          batchIndex: rbIdx, acceptedCount, phase: "rebatch",
          results: batchResults.map((r) => ({ targetId: r.targetId, status: r.status })),
          rebatchBudgetRemaining: rebatchBudget,
        },
      });
      await releaseBatch(repoRoot, config, rbCurrent);

      // Failed targets get routed to singletons by the caller.
      // Store carryover so singletons know why rebatch failed.
      const rbFailed = batchResults.filter((r) => !r.accepted).map((r) => r.targetId);
      for (const id of rbFailed) targetCarryover.set(id, sharedCarryover);
      skipped.push(...rbFailed);

      rbCurrent = [];
    }

    if (rbCurrent.length > 0) {
      for (const id of rbCurrent) targetCarryover.set(id, sharedCarryover);
      skipped.push(...rbCurrent);
    }
  }

  if (skipped.length > 0) {
    appendLedger(repoRoot, config.ledgerPath, {
      ts: new Date().toISOString(), event: "rebatch-skipped", tu: unit,
      detail: {
        skippedIds: skipped,
        reason: rebatchBudget <= 0 ? "budget-exhausted" : "session-failed",
        rebatchBudgetRemaining: rebatchBudget,
      },
    });
    console.log(
      `[pi-harness] ${unit}: ${skipped.length} target(s) skipped by rebatch (budget remaining: ${rebatchBudget})`,
    );
  }

  return { skipped, targetCarryover };
}

// ─────────────────────────────────────────────────────────────────────
//  Singleton retry
// ─────────────────────────────────────────────────────────────────────

async function runSingleton(
  repoRoot: string, unit: string, targetId: string, config: HarnessConfig,
  modelRuntime: ModelRuntime, sanitized: string,
  carryover: string, maxBatchRetries: number,
  siblingsByTarget?: Map<string, SiblingPointer[]>,
): Promise<boolean> {
  const targets = loadUnmatchedTargets(repoRoot, config.region, unit).filter(
    (t) => t.id === targetId,
  );
  if (targets.length === 0) return true; // already matched meanwhile

  const target = targets[0];
  const writable = writableScopeForTargets(repoRoot, [target]);
  const { briefs: rawBriefs, missingAsm } = buildBriefs(repoRoot, [target], siblingsByTarget);
  const briefs = await attachBankedDrafts(repoRoot, config, rawBriefs);
  if (missingAsm.length > 0) return false;

  let feedback = carryover;

  // Phase 3: unified per-target budget across runs. Count prior sessions the
  // ledger recorded for this target (batch/rebatch/singleton all log
  // batch-cycle or batch-session-exhausted rows mentioning the target), then
  // cap this singleton's iterations so pass1+rebatch+singleton never exceed
  // maxAttemptsPerTarget in total.
  const ledgerAttempts = countLedgerSessions(repoRoot, config, targetId);
  const budgetRemaining = Math.max(0, config.maxAttemptsPerTarget - ledgerAttempts);
  if (budgetRemaining <= 0) {
    process.stderr.write(
      `[pi-harness] ${unit}: ${targetId} already exhausted ${ledgerAttempts} session(s) >= maxAttemptsPerTarget — skipping\n`,
    );
    handleSkipped(repoRoot, config, unit, targetId, "maxAttemptsPerTarget exhausted (ledger)");
    return false;
  }
  const maxAttempts = Math.min(config.maxBatchRetries, budgetRemaining);
  console.log(`[pi-harness] ${unit}: starting singleton for ${targetId} (up to ${maxAttempts} session(s); ledger says ${ledgerAttempts} so far)`);

  for (let sessionAttempt = 1; sessionAttempt <= maxAttempts; sessionAttempt++) {
    const brief = buildBatchBrief({
      targets: briefs, unit, writable, carryover: feedback,
      maxChars: config.maxBriefChars, pythonBin: config.pythonBin,
      repoRoot, knownWallsPath: config.knownWallsPath,
      briefTargetChars: config.briefTargetChars,
    });
    const prompt = buildBatchPrompt({
      brief, unit, targetIds: [targetId], pythonBin: config.pythonBin,
    });

    let snapshot: Snapshot | null = null;
    try {
      await claimBatch(repoRoot, config, [targetId]);
      snapshot = await snapshotUnit(repoRoot, unit, writable);

      // Phase 2 refine-from-draft: if a banked draft exists, restore it into
      // the worktree so the session resumes FROM the draft, not from the
      // pristine snapshot (which would throw the previous best away).
      try {
        const restored = await restoreBankedDraft(repoRoot, config, targetId);
        if (restored > 0) {
          process.stderr.write(`[pi-harness] ${unit}: restored ${restored} banked draft file(s) for ${targetId}\n`);
        }
      } catch {
        // best-effort — fall back to the pristine snapshot
      }

      const singletonSymbols = new Map([[targetId, target.symbol]]);
      const verify = makeVerifyCallback({
        repoRoot, config, unit, targetIds: [targetId],
        targetSymbols: singletonSymbols, writable, snapshot,
      });

      const sessionResult = await runAgentSession({
        repoRoot, modelRuntime, spec: config.matchModel, prompt,
        sessionDir: join(config.sessionDir, sanitized, `singleton-${targetId}`),
        label: `singleton-${targetId}-session-${sessionAttempt}`,
        python: config.pythonBin,
        timeoutMinutes: config.maxBatchMinutes,
        maxTokens: config.maxTokens,
        multiPrompt: {
          timeoutRetries: config.timeoutRetries,
          rejectionRetries: config.rejectionRetries,
          onVerify: verify.onVerify,
          totalTimeoutMinutes: config.maxBatchMinutes * (1 + config.timeoutRetries),
        },
      });

      if (sessionResult.roundUsages) {
        for (let r = 0; r < sessionResult.roundUsages.length; r++) {
          logUsage(repoRoot, config, unit, `singleton-${targetId}-session-${sessionAttempt}-round-${r}`,
            sessionResult.roundUsages[r],
            r === sessionResult.roundUsages.length - 1 && sessionResult.timedOut);
        }
      } else {
        logUsage(repoRoot, config, unit, `singleton-${targetId}-session-${sessionAttempt}`,
          sessionResult.usage, sessionResult.timedOut);
      }

      console.log(
        `[pi-harness] ${unit}: singleton ${targetId} session ${sessionAttempt}/${maxAttempts} ` +
        `completed (outcome: ${sessionResult.outcome ?? "single-prompt"}, ` +
        `re-prompts: ${sessionResult.rePromptsUsed ?? 0})`,
      );

      if (sessionResult.outcome === "accepted") {
        const results = verify.getResults();
        const accepted = results?.find((r) => r.targetId === targetId)?.accepted ?? false;
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(),
          event: accepted ? "batch-accept" : "batch-rejected",
          tu: unit,
          detail: {
            targetId, sessionAttempt, singleton: true,
            status: results?.[0]?.status,
            rePromptsUsed: sessionResult.rePromptsUsed,
          },
        });
        await releaseBatch(repoRoot, config, [targetId]);
        if (accepted) return true;
        // Compile + lint passed but batch-cycle rejected.
        feedback = `Previous singleton session passed compile and lint but was not accepted by batch-cycle (status: ${results?.[0]?.status ?? "unknown"}).`;
        continue;
      }

      // Session gave up or failed — restore and try next session.
      await verify.cleanup();
      if (snapshot) await restoreSnapshot(repoRoot, snapshot);

      const rejectionReason = sessionResult.lastRejection
        ?? verify.lastFeedback()
        ?? `${sessionResult.outcome} after ${sessionResult.rePromptsUsed} re-prompt(s)`;
      feedback = `Previous singleton session failed (${sessionResult.outcome}). ${rejectionReason}`;

      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "batch-session-exhausted", tu: unit,
        detail: {
          targetId, sessionAttempt, singleton: true,
          outcome: sessionResult.outcome, rePromptsUsed: sessionResult.rePromptsUsed,
          lastRejection: sessionResult.lastRejection,
        },
      });
      await releaseBatch(repoRoot, config, [targetId]);
    } catch (err) {
      const msg = err instanceof Error ? err.message : String(err);
      const usage = usageFromError(err);
      if (usage) {
        logUsage(repoRoot, config, unit, `singleton-${targetId}-session-${sessionAttempt}`, usage, false);
      }
      let recoveryOutput = "";
      if (snapshot) {
        const recovery = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
        recoveryOutput = recovery.output;
        if (!recovery.ok) await restoreSnapshot(repoRoot, snapshot);
      }
      feedback = `Previous singleton session failed unexpectedly:\n${msg}\n${recoveryOutput}`;
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "batch-error", tu: unit,
        detail: { targetId, sessionAttempt, singleton: true, error: msg },
      });
      await releaseBatch(repoRoot, config, [targetId]);
    }
  }

  return false;
}

// ─────────────────────────────────────────────────────────────────────
//  TU-final phase
// ─────────────────────────────────────────────────────────────────────

async function queueTuFinal(
  repoRoot: string, unit: string, config: HarnessConfig,
  modelRuntime: ModelRuntime, dryRun: boolean, sanitized: string,
): Promise<void> {
  tuFinalQueue = tuFinalQueue
    .catch(() => {})
    .then(() => runTuFinal(repoRoot, unit, config, modelRuntime, dryRun, sanitized));
  await tuFinalQueue;
}

async function runTuFinal(
  repoRoot: string, unit: string, config: HarnessConfig,
  modelRuntime: ModelRuntime, dryRun: boolean, sanitized: string,
): Promise<void> {
  const writable = writableScopeForTargets(
    repoRoot,
    loadUnitTargets(repoRoot, config.region, unit),
  );
  const sourceFiles = writable.filter(
    (f) => f.endsWith(".cpp") || f.endsWith(".c") || f.endsWith(".hpp") || f.endsWith(".h"),
  );

  if (dryRun) {
    console.log(`\n[pi-harness] DRY-RUN: TU-final phase for ${unit}`);
    console.log(`  source files: ${sourceFiles.join(", ")}`);
    return;
  }

  const snapshot = await snapshotUnit(repoRoot, unit, writable);

  // TU-final retry loop: lint or build rejection restores the snapshot and
  // re-runs the cleanup session with the violations/errors as feedback
  // (bounded by tuFinalAttempts). Previously a single lint
  // violation permanently left the unit non-Matching despite all functions
  // matching.
  const basePrompt = buildTuFinalPrompt({
    unit, sourceFiles, pythonBin: config.pythonBin, region: config.region,
  });
  const maxTuFinalAttempts = Math.max(1, config.tuFinalAttempts);
  const tuFinalTimeout = config.tuFinalTimeoutMinutes > 0
    ? config.tuFinalTimeoutMinutes
    : config.maxBatchMinutes * 2;
  let feedback = "";
  let sessionResult: SessionRunResult | null = null;
  let buildOk = false;
  let buildOutput = "";
  let sizeOutput = "";
  let finalLintOk = true;
  let lintViolations: LintOutcome["violations"] = [];
  let lastReason = "";
  // Lint-improvement tracking: a retry that REDUCES violations keeps its
  // improvements (no revert); only a regression restores. bestLintCount =
  // Infinity means "no lint-accepted state yet".
  let bestLintCount = Infinity;
  let bestLintSnapshot: Snapshot | null = null;

  for (let attempt = 1; attempt <= maxTuFinalAttempts; attempt++) {
    const prompt = feedback
      ? basePrompt + `\n\n## TU-final rejection feedback (attempt ${attempt - 1})\n\n${feedback}\n\nThe previous attempt's changes were ${
          lastReason.startsWith("lint") && bestLintSnapshot ? "KEPT (it reduced lint violations)" : "reverted"
        }. Apply the fixes and retry.\n`
      : basePrompt;

    if (attempt > 1) {
      // Restore only on regression (or when nothing improved): if the last
      // attempt reduced violations, its state is already the working tree.
      if (bestLintSnapshot) {
        // No restore — the improved state is in the worktree.
        process.stderr.write(`[pi-harness] ${unit}: TU-final attempt ${attempt}/${maxTuFinalAttempts} keeps the improved worktree (${bestLintCount} lint violation(s) remaining)\n`);
      } else {
        await restoreSnapshot(repoRoot, snapshot);
      }
      process.stderr.write(`[pi-harness] ${unit}: TU-final attempt ${attempt}/${maxTuFinalAttempts} (retry after ${lastReason})\n`);
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "tu-final-retry", tu: unit,
        detail: { attempt, reason: lastReason, keptImprovement: !!bestLintSnapshot, feedback: feedback.slice(0, 500) },
      });
    }

    try {
      sessionResult = await runAgentSession({
        repoRoot, modelRuntime, spec: config.cleanupModel, prompt,
        sessionDir: join(config.sessionDir, sanitized, "tu-final"),
        label: attempt === 1 ? "tu-final" : `tu-final-retry-${attempt}`,
        kind: "tu-final",
        python: config.pythonBin,
        timeoutMinutes: tuFinalTimeout,
        maxTokens: config.maxTokens,
      });
      logUsage(repoRoot, config, unit, attempt === 1 ? "tu-final" : `tu-final-retry-${attempt}`, sessionResult.usage, sessionResult.timedOut);
    } catch (err) {
      const usage = usageFromError(err);
      if (usage) logUsage(repoRoot, config, unit, `tu-final-retry-${attempt}`, usage, false);
      const recovery = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
      if (!recovery.ok) await restoreSnapshot(repoRoot, snapshot);
      lastReason = "session-error";
      feedback = `The TU-final session itself failed: ${err instanceof Error ? err.message : String(err)}`;
      if (attempt < maxTuFinalAttempts) continue;
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "tu-final-failed", tu: unit,
        detail: {
          reason: lastReason,
          error: err instanceof Error ? err.message : String(err),
          recoveryOutput: recovery.output,
        },
      });
      return;
    }

    // ── Build gate (1/2): unit compile check — lint only judges code that
    //    compiles (matches the batch loop's build → lint order). The full
    //    tree rebuild (configure.py + ninja) runs later, after lint passes. ──
    const unitBuild = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
    if (!unitBuild.ok) {
      // A compile failure invalidates any prior lint-improved state — the
      // worktree now holds broken code, so the next retry must restore.
      bestLintSnapshot = null;
      bestLintCount = Infinity;
      lastReason = "build-failed";
      feedback =
        `The unit does not compile after the polish. Error tail:\n\`\`\`text\n` +
        unitBuild.output.slice(-1500) +
        `\n\`\`\`\n\nFix the compile error and retry.`;
      if (attempt < maxTuFinalAttempts) continue;
      await restoreSnapshot(repoRoot, snapshot);
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "tu-final-failed", tu: unit,
        detail: { reason: "build-failed", buildOutput: unitBuild.output.slice(-1500), attempts: maxTuFinalAttempts },
      });
      console.log(`[pi-harness] ${unit}: TU-final compile failed after ${maxTuFinalAttempts} attempt(s) (restored)`);
      return;
    }

    // ── Lint gate (delta vs the pre-session snapshot) ──
    const finalLint = await runLint(repoRoot, config.pythonBin, snapshot);
    if (!finalLint.ok) {
      lastReason = "lint-rejected";
      lintViolations = finalLint.violations;
      finalLintOk = false;
      const rules = [...new Set(lintViolations.map((v) => v.rule))].join(", ");
      feedback =
        `Lint rejected the polish (rules: ${rules}). Violations:\n` +
        lintViolations.slice(0, 10).map(
          (v) => `- ${v.path}:${v.line ?? "?"} [${v.rule}] ${v.detail}`,
        ).join("\n") +
        `\n\nFix ONLY these violations — do not redo the whole finalisation.`;
      // Lint-improvement: if this attempt reduced violations vs the best
      // prior state, snapshot the improved worktree and keep it (no revert).
      const current = lintViolations.length;
      if (current < bestLintCount) {
        bestLintCount = current;
        bestLintSnapshot = await snapshotUnit(repoRoot, unit, writable);
        process.stderr.write(`[pi-harness] ${unit}: lint improved ${bestLintCount} violation(s) remaining — keeping worktree\n`);
      } else {
        bestLintSnapshot = null; // regression — next retry restores
      }
      if (attempt < maxTuFinalAttempts) continue;
    }

    // Lint passed — we are out of lint-recovery mode. A later build failure
    // must restore to the pristine snapshot, not the last lint-improved state.
    bestLintSnapshot = null;
    bestLintCount = Infinity;

    if (!finalLintOk) {
      // Lint still failing after all attempts — keep the best-improved state
      // if any (it is strictly fewer violations than the original), else
      // restore the pristine snapshot. The unit stays non-Matching either way.
      if (!bestLintSnapshot) await restoreSnapshot(repoRoot, snapshot);
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "tu-final-failed", tu: unit,
        detail: {
          reason: "lint-rejected", violations: lintViolations, attempts: maxTuFinalAttempts,
          keptBest: !!bestLintSnapshot, bestLintCount: bestLintCount === Infinity ? undefined : bestLintCount,
        },
      });
      console.log(`[pi-harness] ${unit}: TU-final lint failed after ${maxTuFinalAttempts} attempt(s)` +
        (bestLintSnapshot ? ` (kept best-improved state with ${bestLintCount} violation(s))` : " (restored)"));
      return;
    }

    // ── Build gate (2/2): full-tree rebuild (configure.py + ninja) — runs
    //    only after the unit compiles AND lint passes. ──
    buildOk = false;
    buildOutput = "";
    sizeOutput = "";
    try {
      const { stdout: cfgOut } = await execFilePromise(config.pythonBin, [
        "tools/pi_harness/build_lock.py", "--timeout", "1800", config.region, "--", config.pythonBin, "configure.py",
      ], { cwd: repoRoot });
      const { stdout: ninjaOut } = await execFilePromise(config.pythonBin, [
        "tools/pi_harness/build_lock.py", "--timeout", "1800", config.region, "--", "ninja",
      ], { cwd: repoRoot });
      buildOutput = (cfgOut + ninjaOut).slice(-2000);
      buildOk = true;
    } catch (err) {
      buildOutput = (err instanceof Error ? err.message : String(err)).slice(-2000);
    }

    if (!buildOk) {
      lastReason = "build-failed";
      feedback =
        `The full-tree build failed after the polish. Error tail:\n\`\`\`text\n` +
        buildOutput.slice(-1500) +
        `\n\`\`\`\n\nFix the compile error and retry.`;
      if (attempt < maxTuFinalAttempts) continue;
      await restoreSnapshot(repoRoot, snapshot);
      break;
    }

    if (buildOk) {
      try {
        const { stdout } = await execFilePromise(config.pythonBin, [
          "tools/pi_harness/build_lock.py", "--timeout", "1800", config.region, "--",
          config.pythonBin, "tools/coop/run.py", "size", unit,
        ], { cwd: repoRoot });
        sizeOutput = stdout.slice(-1000);
      } catch (err) {
        sizeOutput = (err instanceof Error ? err.message : String(err)).slice(-1000);
      }
    }
    break; // success
  }

  appendLedger(repoRoot, config.ledgerPath, {
    ts: new Date().toISOString(),
    event: buildOk ? "tu-final-done" : "tu-final-failed",
    tu: unit,
    detail: {
      buildOk, buildOutput, sizeOutput, attempts: buildOk ? 1 : maxTuFinalAttempts,
      finalTextPreview: (sessionResult?.finalText ?? "").slice(0, 1000),
    },
  });
  console.log(
    buildOk
      ? `[pi-harness] ${unit}: TU-final complete`
      : `[pi-harness] ${unit}: TU-final build failed after ${maxTuFinalAttempts} attempt(s) (restored)`,
  );
}

function handleSkipped(
  repoRoot: string, config: HarnessConfig, unit: string, targetId: string, reason: string,
): void {
  appendLedger(repoRoot, config.ledgerPath, {
    ts: new Date().toISOString(), event: "target-skipped", tu: unit,
    detail: { targetId, reason },
  });
}

/**
 * Count sessions the ledger recorded for one target (Phase 3 unified budget).
 * Scans batch-cycle / batch-accept / batch-rejected / batch-session-exhausted
 * rows mentioning the target id. O(ledger) single pass — the ledger is small
 * enough that one scan per singleton is fine.
 */
function countLedgerSessions(
  repoRoot: string, config: HarnessConfig, targetId: string,
): number {
  const events = readLedger(repoRoot, config.ledgerPath);
  let n = 0;
  for (const e of events) {
    const d = (e as { detail?: Record<string, unknown> }).detail ?? {};
    if (e.event === "batch-session-exhausted" || e.event === "batch-rejected") {
      if (d.targetId === targetId || (Array.isArray(d.targetIds) && (d.targetIds as string[]).includes(targetId))) n++;
    } else if (e.event === "batch-cycle" || e.event === "batch-accept") {
      const results = Array.isArray(d.results) ? (d.results as Array<{ targetId?: string }>) : [];
      if (results.some((r) => r.targetId === targetId)) n++;
    }
  }
  return n;
}
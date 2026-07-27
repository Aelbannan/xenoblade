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
import { join } from "node:path";
import { ModelRuntime } from "@earendil-works/pi-coding-agent";
import type {
  HarnessConfig, Target, TargetBrief, SessionUsage, VerifyResult,
} from "./types.js";
import {
  findClaimsByOwner,
  loadUnmatchedTargets,
  loadUnitTargets,
  writableScopeForTargets,
} from "./targets.js";
import { extractRetailAsm, buildBatchBrief } from "./brief.js";
import { appendLedger, readLedger, drainLedger } from "./ledger.js";
import { buildBatchPrompt, buildTuFinalPrompt } from "./prompts.js";
import { runAgentSession, type SessionRunResult } from "./session.js";
import {
  snapshotUnit,
  restoreSnapshot,
  runLint,
  buildUnit,
  runBatchCycle,
  claimTargets,
  releaseTargets,
  execFilePromise,
  type Snapshot,
} from "./acceptance.js";

const OWNER = "pi-harness";

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

  // Release claims left behind by a crashed/killed previous run.
  if (!opts.dryRun) {
    const stale = findClaimsByOwner(repoRoot, OWNER);
    if (stale.length > 0) {
      process.stderr.write(`[pi-harness] Releasing ${stale.length} stale claim(s)\n`);
      await releaseTargets(repoRoot, config.pythonBin, stale, OWNER);
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
    briefs.push({ targetId: t.id, symbol: t.symbol, demangled: t.function, retailAsm: asm });
  }
  return { briefs, missingAsm };
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
  writable: string[];
  snapshot: Snapshot;
}): VerifySession {
  const { repoRoot, config, unit, targetIds, writable, snapshot } = opts;
  let compilableSnapshot: Snapshot | null = null;
  let batchResults: Awaited<ReturnType<typeof runBatchCycle>> | null = null;
  let lastFeedback: string | undefined;

  const onVerify = async (
    _finalText: string,
    timedOut: boolean,
    rePromptCount: number,
  ): Promise<VerifyResult> => {
    // ── Build check ──
    const build = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
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

    // ── Lint check ──
    const lint = await runLint(repoRoot, config.pythonBin, snapshot);
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

    // ── Cap check: both compile and lint passed, so this round is
    //    productive. Only reject on re-prompt budget exhaustion. ──
    const effectiveMax = timedOut ? config.maxRePrompts : config.maxStuckRePrompts;
    if (rePromptCount >= effectiveMax) {
      lastFeedback = `re-prompt cap hit (${effectiveMax})`;
      return { action: "fail", reason: lastFeedback };
    }

    // ── Acceptance ──
    batchResults = await runBatchCycle(repoRoot, config.pythonBin, config.region, targetIds);
    return { action: "accept" };
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
  const targets = loadUnmatchedTargets(repoRoot, config.region, unit);

  if (targets.length === 0) {
    const entries = readLedger(repoRoot, config.ledgerPath).filter((e) => e.tu === unit);
    const wasWorked = entries.some(
      (e) => e.event === "batch-accept" || e.event === "tu-started",
    );
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

  for (let batchIndex = 0; batchIndex < allIds.length; batchIndex += config.batchSize) {
    const batchIds = allIds.slice(batchIndex, batchIndex + config.batchSize);
    const batchTargets = targets.filter((t) => batchIds.includes(t.id));

    const { briefs, missingAsm } = buildBriefs(repoRoot, batchTargets);
    for (const id of missingAsm) {
      if (!dryRun) handleSkipped(repoRoot, config, unit, id, "no retail asm");
    }
    const briefTargets = batchTargets.filter((t) => !missingAsm.includes(t.id));
    const writable = writableScopeForTargets(repoRoot, briefTargets);
    const batchSnapshot = dryRun
      ? null
      : await snapshotUnit(repoRoot, unit, writable);
    let currentIds = batchIds.filter((id) => !missingAsm.includes(id));

    let attempt = 0;
    const maxAttempts = config.maxBatchRetries;

    while (currentIds.length > 0 && attempt < maxAttempts) {
      attempt++;

      const brief = buildBatchBrief({
        targets: briefs, unit, writable, carryover,
        maxChars: config.maxBriefChars, pythonBin: config.pythonBin,
      });
      const prompt = buildBatchPrompt({
        brief, unit, targetIds: currentIds, pythonBin: config.pythonBin,
      });

      if (dryRun) {
        console.log(`\n[pi-harness] DRY-RUN: unit=${unit}, batch=${batchIndex}, attempt=${attempt}`);
        console.log(`  targets: ${currentIds.join(", ")}`);
        const lines = prompt.split("\n");
        console.log(lines.slice(0, 40).join("\n"));
        if (lines.length > 40) console.log(`  … (${lines.length - 40} more lines)`);
        break;
      }

      let snapshot: Snapshot | null = batchSnapshot;
      let attemptError: string | null = null;
      let batchResults: Awaited<ReturnType<typeof runBatchCycle>> | null = null;

      try {
        await claimBatch(repoRoot, config, currentIds);
        snapshot = await snapshotUnit(repoRoot, unit, writable);

        const verify = makeVerifyCallback({
          repoRoot, config, unit, targetIds: currentIds, writable, snapshot,
        });

        const sessionResult = await runAgentSession({
          repoRoot,
          modelRuntime,
          spec: config.matchModel,
          prompt,
          sessionDir: join(config.sessionDir, sanitized, `batch-${batchIndex}`),
          label: `batch-${batchIndex}-session-${attempt}`,
          timeoutMinutes: config.maxBatchMinutes,
          maxTokens: config.maxTokens,
          multiPrompt: {
            maxRePrompts: config.maxRePrompts,
            maxStuckRePrompts: config.maxStuckRePrompts,
            onVerify: verify.onVerify,
            totalTimeoutMinutes: config.maxBatchMinutes * (1 + config.maxRePrompts),
          },
        });

        // Log per-round usage.
        if (sessionResult.roundUsages) {
          for (let r = 0; r < sessionResult.roundUsages.length; r++) {
            logUsage(
              repoRoot, config, unit,
              `batch-${batchIndex}-session-${attempt}-round-${r}`,
              sessionResult.roundUsages[r],
              r === sessionResult.roundUsages.length - 1 && sessionResult.timedOut,
            );
          }
        } else {
          logUsage(repoRoot, config, unit, `batch-${batchIndex}-session-${attempt}`,
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
          carryover = `Previous session failed (${sessionResult.outcome}). ${rejectionReason}`;

          appendLedger(repoRoot, config.ledgerPath, {
            ts: new Date().toISOString(), event: "batch-session-exhausted", tu: unit,
            detail: {
              batchIndex, attempt, outcome: sessionResult.outcome,
              rePromptsUsed: sessionResult.rePromptsUsed,
              lastRejection: sessionResult.lastRejection,
            },
          });
          await releaseBatch(repoRoot, config, currentIds);
          continue;
        }
      } catch (err) {
        attemptError = err instanceof Error ? err.message : String(err);
        const usage = usageFromError(err);
        if (usage) {
          logUsage(repoRoot, config, unit, `batch-${batchIndex}-session-${attempt}`, usage, false);
        }
      }

      if (attemptError !== null && batchResults === null) {
        let recoveryOutput = "";
        if (batchSnapshot) {
          const recovery = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
          recoveryOutput = recovery.output;
          if (!recovery.ok) await restoreSnapshot(repoRoot, batchSnapshot);
        }
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-error", tu: unit,
          detail: { batchIndex, attempt, error: attemptError, recoveryOutput },
        });
        await releaseBatch(repoRoot, config, currentIds);
        carryover = `Previous attempt failed unexpectedly:\n${attemptError}\n${recoveryOutput}`;
        continue;
      }

      if (batchResults === null) {
        await releaseBatch(repoRoot, config, currentIds);
        carryover = "Previous attempt produced no results (internal error).";
        continue;
      }

      // ── Acceptance results ──────────────────────────────────────
      const acceptedCount = batchResults.filter((r) => r.accepted).length;
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(),
        event: acceptedCount > 0 ? "batch-accept" : "batch-cycle",
        tu: unit,
        detail: {
          batchIndex, attempt, acceptedCount,
          results: batchResults.map((r) => ({ targetId: r.targetId, status: r.status })),
        },
      });
      await releaseBatch(repoRoot, config, currentIds);

      const failedIds = batchResults.filter((r) => !r.accepted).map((r) => r.targetId);
      for (const fid of failedIds) {
        // Each failed target gets its own singleton budget, independent
        // of the batch attempt count.
        const singletonAttempts = config.maxBatchRetries;
        if (!config.singletonRetry || singletonAttempts === 0) {
          handleSkipped(repoRoot, config, unit, fid,
            config.singletonRetry ? "singleton retries exhausted" : "singleton retry disabled");
          continue;
        }
        if (config.singletonMinSize > 0) {
          const b = briefs.find((x) => x.targetId === fid);
          if (b && b.retailAsm.length < config.singletonMinSize) {
            smallRetryPool.push(fid);
            continue;
          }
        }
        const ok = await runSingleton(
          repoRoot, unit, fid, config, modelRuntime, sanitized, carryover, singletonAttempts,
        );
        if (!ok) {
          handleSkipped(repoRoot, config, unit, fid, "exhausted singleton retries");
        }
      }

      currentIds = [];
    }

    if (!dryRun && currentIds.length > 0) {
      for (const id of currentIds) {
        handleSkipped(repoRoot, config, unit, id, "exhausted batch retries");
      }
    }
  }

  // ── Re-batch phase for small failed targets ─────────────────────
  if (smallRetryPool.length > 0) {
    await runRebatchPhase(
      repoRoot, unit, config, modelRuntime, sanitized,
      smallRetryPool, targets, carryover,
    );
  }

  // ── TU-final phase ────────────────────────────────────────────────
  if (dryRun) {
    console.log(`[pi-harness] ${unit}: dry-run complete (no sessions run, no acceptance)`);
    return;
  }

  const remaining = loadUnmatchedTargets(repoRoot, config.region, unit);
  if (remaining.length === 0) {
    await queueTuFinal(repoRoot, unit, config, modelRuntime, dryRun, sanitized);
  } else {
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

async function runRebatchPhase(
  repoRoot: string, unit: string, config: HarnessConfig,
  modelRuntime: ModelRuntime, sanitized: string,
  smallRetryPool: string[], targets: Target[], carryover: string,
): Promise<void> {
  const uniqueSmall = [...new Set(smallRetryPool)];
  console.log(
    `[pi-harness] ${unit}: re-batching ${uniqueSmall.length} small target(s) ` +
    `(below ${config.singletonMinSize} bytes)`,
  );

  const smallTargets = targets.filter((t) => uniqueSmall.includes(t.id));
  let sharedCarryover = carryover;

  for (let rbIdx = 0; rbIdx < uniqueSmall.length; rbIdx += config.batchSize) {
    const rbIds = uniqueSmall.slice(rbIdx, rbIdx + config.batchSize);
    const rbTargets = smallTargets.filter((t) => rbIds.includes(t.id));

    const { briefs: rbBriefs, missingAsm: rbMissing } = buildBriefs(repoRoot, rbTargets);
    for (const id of rbMissing) {
      handleSkipped(repoRoot, config, unit, id, "no retail asm");
    }
    const rbFiltered = rbIds.filter((id) => !rbMissing.includes(id));
    if (rbFiltered.length === 0) continue;

    const writable = writableScopeForTargets(
      repoRoot,
      rbTargets.filter((t) => !rbMissing.includes(t.id)),
    );
    const rbSnapshot = await snapshotUnit(repoRoot, unit, writable);
    let rbCurrent = rbFiltered;
    let rbAttempt = 0;
    const rbMax = config.maxBatchRetries;

    while (rbCurrent.length > 0 && rbAttempt < rbMax) {
      rbAttempt++;

      const brief = buildBatchBrief({
        targets: rbBriefs, unit, writable, carryover: sharedCarryover,
        maxChars: config.maxBriefChars, pythonBin: config.pythonBin,
      });
      const prompt = buildBatchPrompt({
        brief, unit, targetIds: rbCurrent, pythonBin: config.pythonBin,
      });

      let snapshot: Snapshot | null = rbSnapshot;
      let attemptError: string | null = null;
      let batchResults: Awaited<ReturnType<typeof runBatchCycle>> | null = null;

      try {
        await claimBatch(repoRoot, config, rbCurrent);
        snapshot = await snapshotUnit(repoRoot, unit, writable);

        const verify = makeVerifyCallback({
          repoRoot, config, unit, targetIds: rbCurrent, writable, snapshot,
        });

        const sessionResult = await runAgentSession({
          repoRoot, modelRuntime, spec: config.matchModel, prompt,
          sessionDir: join(config.sessionDir, sanitized, `rebatch-${rbIdx}`),
          label: `rebatch-${rbIdx}-session-${rbAttempt}`,
          timeoutMinutes: config.maxBatchMinutes,
          maxTokens: config.maxTokens,
          multiPrompt: {
            maxRePrompts: config.maxRePrompts,
            maxStuckRePrompts: config.maxStuckRePrompts,
            onVerify: verify.onVerify,
            totalTimeoutMinutes: config.maxBatchMinutes * (1 + config.maxRePrompts),
          },
        });

        if (sessionResult.roundUsages) {
          for (let r = 0; r < sessionResult.roundUsages.length; r++) {
            logUsage(repoRoot, config, unit, `rebatch-${rbIdx}-session-${rbAttempt}-round-${r}`,
              sessionResult.roundUsages[r],
              r === sessionResult.roundUsages.length - 1 && sessionResult.timedOut);
          }
        } else {
          logUsage(repoRoot, config, unit, `rebatch-${rbIdx}-session-${rbAttempt}`,
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

          appendLedger(repoRoot, config.ledgerPath, {
            ts: new Date().toISOString(), event: "batch-session-exhausted", tu: unit,
            detail: {
              batchIndex: rbIdx, attempt: rbAttempt, phase: "rebatch",
              outcome: sessionResult.outcome, rePromptsUsed: sessionResult.rePromptsUsed,
              lastRejection: sessionResult.lastRejection,
            },
          });
          await releaseBatch(repoRoot, config, rbCurrent);
          continue;
        }
      } catch (err) {
        attemptError = err instanceof Error ? err.message : String(err);
        const usage = usageFromError(err);
        if (usage) {
          logUsage(repoRoot, config, unit, `rebatch-${rbIdx}-session-${rbAttempt}`, usage, false);
        }
      }

      if (attemptError !== null && batchResults === null) {
        let recoveryOutput = "";
        if (rbSnapshot) {
          const recovery = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
          recoveryOutput = recovery.output;
          if (!recovery.ok) await restoreSnapshot(repoRoot, rbSnapshot);
        }
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-error", tu: unit,
          detail: { batchIndex: rbIdx, attempt: rbAttempt, error: attemptError, recoveryOutput, phase: "rebatch" },
        });
        await releaseBatch(repoRoot, config, rbCurrent);
        sharedCarryover = `Previous re-batch attempt failed unexpectedly:\n${attemptError}\n${recoveryOutput}`;
        continue;
      }

      if (batchResults === null) {
        await releaseBatch(repoRoot, config, rbCurrent);
        sharedCarryover = "Previous re-batch attempt produced no results.";
        continue;
      }

      const acceptedCount = batchResults.filter((r) => r.accepted).length;
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(),
        event: acceptedCount > 0 ? "batch-accept" : "batch-cycle",
        tu: unit,
        detail: {
          batchIndex: rbIdx, attempt: rbAttempt, acceptedCount, phase: "rebatch",
          results: batchResults.map((r) => ({ targetId: r.targetId, status: r.status })),
        },
      });
      await releaseBatch(repoRoot, config, rbCurrent);

      const rbFailed = batchResults.filter((r) => !r.accepted).map((r) => r.targetId);
      for (const fid of rbFailed) {
        // Re-batch failed targets still go to singletons (if
        // singletonRetry is on) — they got a second chance in the
        // re-batch and still didn't match.
        if (!config.singletonRetry) {
          handleSkipped(repoRoot, config, unit, fid, "singleton retry disabled");
          continue;
        }
        const ok = await runSingleton(
          repoRoot, unit, fid, config, modelRuntime, sanitized, sharedCarryover, config.maxBatchRetries,
        );
        if (!ok) {
          handleSkipped(repoRoot, config, unit, fid, "exhausted singleton retries");
        }
      }

      rbCurrent = [];
    }

    if (rbCurrent.length > 0) {
      for (const id of rbCurrent) {
        handleSkipped(repoRoot, config, unit, id, "exhausted rebatch retries");
      }
    }
  }
}

// ─────────────────────────────────────────────────────────────────────
//  Singleton retry
// ─────────────────────────────────────────────────────────────────────

async function runSingleton(
  repoRoot: string, unit: string, targetId: string, config: HarnessConfig,
  modelRuntime: ModelRuntime, sanitized: string,
  carryover: string, maxAttempts: number,
): Promise<boolean> {
  const targets = loadUnmatchedTargets(repoRoot, config.region, unit).filter(
    (t) => t.id === targetId,
  );
  if (targets.length === 0) return true; // already matched meanwhile

  const target = targets[0];
  const writable = writableScopeForTargets(repoRoot, [target]);
  const { briefs, missingAsm } = buildBriefs(repoRoot, [target]);
  if (missingAsm.length > 0) return false;

  let feedback = carryover;

  console.log(`[pi-harness] ${unit}: starting singleton for ${targetId} (up to ${maxAttempts} session(s))`);

  for (let sessionAttempt = 1; sessionAttempt <= maxAttempts; sessionAttempt++) {
    const brief = buildBatchBrief({
      targets: briefs, unit, writable, carryover: feedback,
      maxChars: config.maxBriefChars, pythonBin: config.pythonBin,
    });
    const prompt = buildBatchPrompt({
      brief, unit, targetIds: [targetId], pythonBin: config.pythonBin,
    });

    let snapshot: Snapshot | null = null;
    try {
      await claimBatch(repoRoot, config, [targetId]);
      snapshot = await snapshotUnit(repoRoot, unit, writable);

      const verify = makeVerifyCallback({
        repoRoot, config, unit, targetIds: [targetId], writable, snapshot,
      });

      const sessionResult = await runAgentSession({
        repoRoot, modelRuntime, spec: config.matchModel, prompt,
        sessionDir: join(config.sessionDir, sanitized, `singleton-${targetId}`),
        label: `singleton-${targetId}-session-${sessionAttempt}`,
        timeoutMinutes: config.maxBatchMinutes,
        maxTokens: config.maxTokens,
        multiPrompt: {
          maxRePrompts: config.maxRePrompts,
          maxStuckRePrompts: config.maxStuckRePrompts,
          onVerify: verify.onVerify,
          totalTimeoutMinutes: config.maxBatchMinutes * (1 + config.maxRePrompts),
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

  const prompt = buildTuFinalPrompt({
    unit, sourceFiles, pythonBin: config.pythonBin, region: config.region,
  });

  const snapshot = await snapshotUnit(repoRoot, unit, writable);
  let sessionResult: SessionRunResult;
  try {
    sessionResult = await runAgentSession({
      repoRoot, modelRuntime, spec: config.cleanupModel, prompt,
      sessionDir: join(config.sessionDir, sanitized, "tu-final"),
      label: "tu-final",
      timeoutMinutes: config.maxBatchMinutes * 2,
      maxTokens: config.maxTokens,
    });
    logUsage(repoRoot, config, unit, "tu-final", sessionResult.usage, sessionResult.timedOut);
  } catch (err) {
    const usage = usageFromError(err);
    if (usage) logUsage(repoRoot, config, unit, "tu-final", usage, false);
    const recovery = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
    if (!recovery.ok) await restoreSnapshot(repoRoot, snapshot);
    appendLedger(repoRoot, config.ledgerPath, {
      ts: new Date().toISOString(), event: "tu-final-failed", tu: unit,
      detail: {
        reason: "session-error",
        error: err instanceof Error ? err.message : String(err),
        recoveryOutput: recovery.output,
      },
    });
    return;
  }

  const finalLint = await runLint(repoRoot, config.pythonBin, snapshot);
  if (!finalLint.ok) {
    appendLedger(repoRoot, config.ledgerPath, {
      ts: new Date().toISOString(), event: "tu-final-failed", tu: unit,
      detail: { reason: "lint-rejected", violations: finalLint.violations },
    });
    console.log(`[pi-harness] ${unit}: TU-final lint failed (restored)`);
    return;
  }

  let buildOk = false;
  let buildOutput = "";
  let sizeOutput = "";

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

  if (!buildOk) {
    await restoreSnapshot(repoRoot, snapshot);
  }

  appendLedger(repoRoot, config.ledgerPath, {
    ts: new Date().toISOString(),
    event: buildOk ? "tu-final-done" : "tu-final-failed",
    tu: unit,
    detail: {
      buildOk, buildOutput, sizeOutput,
      finalTextPreview: sessionResult.finalText.slice(0, 1000),
    },
  });
  console.log(
    buildOk
      ? `[pi-harness] ${unit}: TU-final complete`
      : `[pi-harness] ${unit}: TU-final build failed (restored)`,
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
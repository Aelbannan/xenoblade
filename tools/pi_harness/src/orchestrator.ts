/**
 * TU-level orchestration for the pi-harness.
 *
 * runTus manages a concurrency-limited pool of TU workers. Each TU:
 *   1. Loads unmatched targets
 *   2. Processes them in batches:
 *      claim → brief → snapshot → session → compile check → lint →
 *      batch-cycle (acceptance). Restore from snapshot copies ONLY when the
 *      build fails or lint rejects.
 *   3. Retries failed targets as singleton sessions
 *   4. Runs a TU-finalisation session once every function matches
 *
 * @module orchestrator
 */

import { execFileSync } from "node:child_process";
import { join } from "node:path";
import { ModelRuntime } from "@earendil-works/pi-coding-agent";
import type { HarnessConfig, Target, TargetBrief, SessionUsage } from "./types.js";
import {
  findClaimsByOwner,
  loadUnmatchedTargets,
  loadUnitTargets,
  writableScopeForTargets,
} from "./targets.js";
import { extractRetailAsm, buildBatchBrief } from "./brief.js";
import { appendLedger, readLedger } from "./ledger.js";
import { buildBatchPrompt, buildTuFinalPrompt } from "./prompts.js";
import { runAgentSession } from "./session.js";
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
  const onSignal = (): void => {
    for (const id of activeClaims) {
      try {
        execFileSync(
          config.pythonBin,
          ["tools/coop/run.py", "targets", "release", id, "--owner", OWNER],
          { cwd: repoRoot, stdio: "ignore" },
        );
      } catch {
        // best-effort
      }
    }
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
  await claimTargets(repoRoot, config.pythonBin, ids, OWNER);
  for (const id of ids) activeClaims.add(id);
}

async function releaseBatch(
  repoRoot: string, config: HarnessConfig, ids: string[],
): Promise<void> {
  await releaseTargets(repoRoot, config.pythonBin, ids, OWNER);
  for (const id of ids) activeClaims.delete(id);
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

  // Nothing unmatched: only run TU-final if this harness previously worked
  // the unit (or dry-run), otherwise there is nothing to do.
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
  const handled = new Set<string>();

  for (let batchIndex = 0; batchIndex < allIds.length; batchIndex += config.batchSize) {
    const batchIds = allIds.slice(batchIndex, batchIndex + config.batchSize);
    const batchTargets = targets.filter((t) => batchIds.includes(t.id));

    // Build briefs once per batch; targets without retail ASM are skipped.
    const { briefs, missingAsm } = buildBriefs(repoRoot, batchTargets);
    for (const id of missingAsm) {
      if (!dryRun) handleSkipped(repoRoot, config, unit, id, "no retail asm");
      handled.add(id);
    }
    const briefTargets = batchTargets.filter((t) => !missingAsm.includes(t.id));
    let currentIds = batchIds.filter((id) => !missingAsm.includes(id));

    let attempt = 0;
    const maxAttempts = config.maxBatchRetries;

    while (currentIds.length > 0 && attempt < maxAttempts) {
      attempt++;

      const writable = writableScopeForTargets(repoRoot, briefTargets);
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
        for (const id of currentIds) handled.add(id);
        break;
      }

      // ── Session + acceptance, exception-contained ────────────────
      let snapshot: Snapshot | null = null;
      let attemptError: string | null = null;
      let buildOk = false;
      let buildOutput = "";
      let lintOk = false;
      let lintViolations: unknown[] = [];
      let batchResults: Awaited<ReturnType<typeof runBatchCycle>> | null = null;

      try {
        await claimBatch(repoRoot, config, currentIds);
        snapshot = await snapshotUnit(repoRoot, unit, writable);

        const sessionResult = await runAgentSession({
          repoRoot, modelRuntime, spec: config.matchModel, prompt,
          sessionDir: join(config.sessionDir, sanitized, `batch-${batchIndex}-attempt-${attempt}`),
          label: `batch-${batchIndex}-attempt-${attempt}`,
          timeoutMinutes: config.maxBatchMinutes,
        });
        logUsage(repoRoot, config, unit, `batch-${batchIndex}-attempt-${attempt}`,
          sessionResult.usage, sessionResult.timedOut);
        carryover = sessionResult.finalText.slice(0, 1500);

        const build = await buildUnit(repoRoot, config.pythonBin, unit);
        buildOk = build.ok;
        buildOutput = build.output;

        if (buildOk) {
          const lint = await runLint(repoRoot, config.pythonBin, snapshot);
          lintOk = lint.ok;
          lintViolations = lint.violations;
          if (lintOk) {
            batchResults = await runBatchCycle(repoRoot, config.pythonBin, currentIds);
          }
        }
      } catch (err) {
        attemptError = err instanceof Error ? err.message : String(err);
      }

      // ── Failure handling: restore from snapshot ONLY on build/lint
      //    failure; keep edits otherwise (best-known candidate). ──────
      if (attemptError !== null) {
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-error", tu: unit,
          detail: { batchIndex, attempt, error: attemptError },
        });
        await releaseBatch(repoRoot, config, currentIds);
        continue;
      }

      if (!buildOk || !lintOk) {
        if (snapshot) await restoreSnapshot(repoRoot, snapshot);
        const reason = !buildOk ? "compile-failed" : "lint-rejected";
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-rejected", tu: unit,
          detail: {
            batchIndex, attempt, reason,
            buildOutput: !buildOk ? buildOutput : undefined,
            violations: !lintOk ? lintViolations : undefined,
          },
        });
        // Feed the rejection back into the next attempt's brief.
        carryover =
          `Previous attempt was rejected (${reason}). Fix this before anything else:\n` +
          (!buildOk ? buildOutput : JSON.stringify(lintViolations).slice(0, 1200));
        await releaseBatch(repoRoot, config, currentIds);
        continue;
      }

      // ── Acceptance results ────────────────────────────────────────
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "batch-accept", tu: unit,
        detail: {
          batchIndex, attempt,
          results: batchResults!.map((r) => ({ targetId: r.targetId, status: r.status })),
        },
      });
      await releaseBatch(repoRoot, config, currentIds);

      for (const r of batchResults!) {
        if (r.accepted) handled.add(r.targetId);
      }

      const failedIds = batchResults!.filter((r) => !r.accepted).map((r) => r.targetId);
      for (const fid of failedIds) {
        const remainingAttempts = Math.max(0, maxAttempts - attempt);
        if (!config.singletonRetry || remainingAttempts === 0) {
          handleSkipped(repoRoot, config, unit, fid,
            config.singletonRetry ? "exhausted retries" : "singleton retry disabled");
          handled.add(fid);
          continue;
        }
        const ok = await runSingleton(
          repoRoot, unit, fid, config, modelRuntime, sanitized, batchIndex, carryover, remainingAttempts,
        );
        if (ok) handled.add(fid);
        else {
          handleSkipped(repoRoot, config, unit, fid, "exhausted singleton retries");
          handled.add(fid);
        }
      }

      currentIds = [];
    }

    // Attempts exhausted with targets still unhandled — record the skip.
    if (!dryRun && currentIds.length > 0) {
      for (const id of currentIds) {
        handleSkipped(repoRoot, config, unit, id, "exhausted batch retries");
        handled.add(id);
      }
    }
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
//  Singleton retry
// ─────────────────────────────────────────────────────────────────────

async function runSingleton(
  repoRoot: string, unit: string, targetId: string, config: HarnessConfig,
  modelRuntime: ModelRuntime, sanitized: string, batchIndex: number,
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

  const brief = buildBatchBrief({
    targets: briefs, unit, writable, carryover,
    maxChars: config.maxBriefChars, pythonBin: config.pythonBin,
  });
  const prompt = buildBatchPrompt({
    brief, unit, targetIds: [targetId], pythonBin: config.pythonBin,
  });

  let feedback = carryover;

  for (let attempt = 1; attempt <= maxAttempts; attempt++) {
    let snapshot: Snapshot | null = null;
    try {
      await claimBatch(repoRoot, config, [targetId]);
      snapshot = await snapshotUnit(repoRoot, unit, writable);

      const sessionResult = await runAgentSession({
        repoRoot, modelRuntime, spec: config.matchModel, prompt,
        sessionDir: join(config.sessionDir, sanitized, `singleton-${targetId}-attempt-${attempt}`),
        label: `singleton-${targetId}-attempt-${attempt}`,
        timeoutMinutes: config.maxBatchMinutes,
      });
      logUsage(repoRoot, config, unit, `singleton-${targetId}-attempt-${attempt}`,
        sessionResult.usage, sessionResult.timedOut);

      const build = await buildUnit(repoRoot, config.pythonBin, unit);
      if (!build.ok) {
        await restoreSnapshot(repoRoot, snapshot);
        feedback = `Previous attempt was rejected (compile-failed):\n${build.output}`;
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-rejected", tu: unit,
          detail: { targetId, attempt, singleton: true, reason: "compile-failed", buildOutput: build.output },
        });
        await releaseBatch(repoRoot, config, [targetId]);
        continue;
      }

      const lint = await runLint(repoRoot, config.pythonBin, snapshot);
      if (!lint.ok) {
        await restoreSnapshot(repoRoot, snapshot);
        feedback = `Previous attempt was rejected (lint):\n${JSON.stringify(lint.violations).slice(0, 1200)}`;
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-rejected", tu: unit,
          detail: { targetId, attempt, singleton: true, reason: "lint-rejected", violations: lint.violations },
        });
        await releaseBatch(repoRoot, config, [targetId]);
        continue;
      }

      const results = await runBatchCycle(repoRoot, config.pythonBin, [targetId]);
      const accepted = results.find((r) => r.targetId === targetId)?.accepted ?? false;
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: accepted ? "batch-accept" : "batch-rejected", tu: unit,
        detail: { targetId, attempt, singleton: true, status: results[0]?.status },
      });
      await releaseBatch(repoRoot, config, [targetId]);
      if (accepted) return true;
    } catch (err) {
      const msg = err instanceof Error ? err.message : String(err);
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "batch-error", tu: unit,
        detail: { targetId, attempt, singleton: true, error: msg },
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
  const sessionResult = await runAgentSession({
    repoRoot, modelRuntime, spec: config.cleanupModel, prompt,
    sessionDir: join(config.sessionDir, sanitized, "tu-final"),
    label: "tu-final",
    timeoutMinutes: config.maxBatchMinutes * 2,
  });
  logUsage(repoRoot, config, unit, "tu-final", sessionResult.usage, sessionResult.timedOut);

  // Verify: configure + ninja + size, each under the repo build lock.
  let buildOk = false;
  let buildOutput = "";
  let sizeOutput = "";

  try {
    const { stdout: cfgOut } = await execFilePromise(config.pythonBin, [
      "tools/pi_harness/build_lock.py", config.region, "--", config.pythonBin, "configure.py",
    ], { cwd: repoRoot });
    const { stdout: ninjaOut } = await execFilePromise(config.pythonBin, [
      "tools/pi_harness/build_lock.py", config.region, "--", "ninja",
    ], { cwd: repoRoot });
    buildOutput = (cfgOut + ninjaOut).slice(-2000);
    buildOk = true;
  } catch (err) {
    buildOutput = (err instanceof Error ? err.message : String(err)).slice(-2000);
  }

  if (buildOk) {
    try {
      const { stdout } = await execFilePromise(config.pythonBin, [
        "tools/pi_harness/build_lock.py", config.region, "--",
        config.pythonBin, "tools/coop/run.py", "size", unit,
      ], { cwd: repoRoot });
      sizeOutput = stdout.slice(-1000);
    } catch (err) {
      sizeOutput = (err instanceof Error ? err.message : String(err)).slice(-1000);
    }
  }

  if (!buildOk) {
    // The finalised TU does not build — restore the pre-session copies.
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

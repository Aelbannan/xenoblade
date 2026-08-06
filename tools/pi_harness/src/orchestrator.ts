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
import { existsSync, readFileSync } from "node:fs";
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
  targetRowById,
  isCallGraphReady,
} from "./targets.js";
import { extractRetailAsm, buildBatchBrief } from "./brief.js";
import { appendLedger, readLedger, drainLedger } from "./ledger.js";
import {
  bankDraft, bestBankedDraft, draftNoteFor, restoreBankedDraft,
} from "./nearmiss.js";
import { buildBatchPrompt, buildTuFinalPrompt } from "./prompts.js";
import {
  sessionsNeededForPool,
  effectiveRebatchBudget,
  partitionFailedTargets,
} from "./routing.js";
import { witnessGateName, witnessHintForGate } from "./witness-hints.js";
import {
  scanUnitState, scoreState, buildUnitFeedback, diffUnitScans,
  type UnitScan,
} from "./tufinal-scan.js";
import { runAgentSession, type SessionRunResult } from "./session.js";
import {
  snapshotUnit,
  restoreSnapshot,
  runLint,
  buildUnit,
  runBatchCycle,
  readBatchResults,
  runHexdiff,
  claimTargets,
  releaseTargets,
  execFilePromise,
  type Snapshot,
  type LintOutcome,
} from "./acceptance.js";

const OWNER = `pi-harness-${process.pid}`;
/** True when a live PID is actually a pi-harness process (not a PID-reused
 *  unrelated process). Best-effort: on macOS/Linux reads `ps -p <pid> -o
 *  command=`; if ps is unavailable or errors, falls back to treating the
 *  process as alive (conservative — no false release). */
function isPiHarnessProcess(pid: number): boolean {
  try {
    const out = execFileSync("ps", ["-p", String(pid), "-o", "command="], {
      encoding: "utf-8", timeout: 5000, stdio: ["ignore", "pipe", "ignore"],
    }).trim();
    // The command line of a pi-harness worker contains "pi-harness" or
    // "tsx src/index.ts". A reused PID pointing at some other program is not
    // a claim owner and must be released (adversarial review K).
    return /pi-harness|tsx\s+src\/index\.ts|run\.py\s+targets/i.test(out);
  } catch {
    // ps unavailable / errors — be conservative: assume it is a live harness.
    return true;
  }
}

/**
 * Find claims owned by dead pi-harness processes.
 * Scans targets.json for claims with owner matching `pi-harness-*`,
 * then checks if the owning process is still alive AND actually a
 * pi-harness process (guards against PID reuse marking a stale claim live).
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
      // Check if the owning process is still alive AND still a harness.
      let alive = false;
      try {
        process.kill(pid, 0); // Signal 0 = check existence
        alive = true;
      } catch {
        alive = false;
      }
      if (!alive || !isPiHarnessProcess(pid)) {
        // Process doesn't exist, or the PID was reused by a non-harness
        // process — claim is orphaned.
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

// r8 WS-1/WS-2: last witness rejection gate per target (e.g. "reloc | …"),
// captured when a witness-only cycle fails and surfaced in the near-match
// singleton brief's diagnosis block so the model gets an actionable hint.
// Per-process only (not persisted) — safe under the restart model.
const witnessGateByTarget = new Map<string, string>();

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
    ["singletonModel", config.singletonModel],
    ["rebatchModel", config.rebatchModel],
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
    // Detect overlapping writable scopes across the requested units. Two
    // units can share a sibling header (same src/…/ dir); parallel TUs both
    // snapshot and edit it, and one TU's restore can clobber the other's
    // in-flight edits (adversarial review M7). When overlaps exist, serialize
    // the run (concurrency 1) instead of risking cross-TU file races.
    let effectiveParallel = config.maxParallelTUs;
    if (units.length > 1) {
      const scopes = new Map<string, Set<string>>();
      for (const unit of units) {
        const files = writableScopeForTargets(repoRoot, loadUnitTargets(repoRoot, config.region, unit));
        scopes.set(unit, new Set(files));
      }
      const overlap = new Set<string>();
      const seen = new Set<string>();
      for (const [unit, files] of scopes) {
        for (const f of files) {
          if (seen.has(f)) overlap.add(f);
          seen.add(f);
        }
      }
      if (overlap.size > 0) {
        process.stderr.write(
          `[pi-harness] WARNING: overlapping writable scope across TUs (${[...overlap].join(", ")}) — serializing run (maxParallelTUs forced to 1) to prevent cross-TU clobbering\n`,
        );
        effectiveParallel = 1;
      }
    }
    const pool = new ConcurrencyPool(effectiveParallel);
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
    // Flush queued ledger appends before the process exits normally
    // (drainLedger was previously only called on signals — cheap insurance
    // against losing the final batch-accept / tu-final-done events).
    await drainLedger();
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
  // When the witness is disabled (witnessEnabled=false) there is no
  // witness-only path — regswap_only targets stay in the LLM batch and
  // chase FULL_MATCH (byte-identical) like everyone else.
  let witnessTargets: Target[] = [];
  if (triage && config.witnessEnabled) {
    witnessTargets = targets.filter((x) => clsById.get(x.id) === "regswap_only");
    const witnessIds = new Set(witnessTargets.map((x) => x.id));
    const rest = ordered.filter((x) => !witnessIds.has(x.id));
    const strict = rest.filter((x) => clsById.get(x.id) === "strict");
    const other = rest.filter((x) => clsById.get(x.id) !== "strict");
    ordered = [...strict, ...other];
  } else if (triage) {
    // witness disabled: keep the strict front-load, keep regswap_only in the
    // LLM batch (no witness-only route to pull them into).
    const strict = ordered.filter((x) => clsById.get(x.id) === "strict");
    const other = ordered.filter((x) => clsById.get(x.id) !== "strict");
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

/** Registry acceptance predicate shared by BOTH branches of runWitnessCycle
 *  (exit-0 and non-zero) and exported for unit tests: a row certifies when its
 *  status is FULL_MATCH, or (witness enabled) EQUIVALENT_MATCH, and it is not
 *  size-gate BACKLOG. With the witness disabled, EQUIVALENT_MATCH is never
 *  accepted — only byte-identical FULL_MATCH counts (adversarial-review H1:
 *  the exit-0 branch previously trusted the exit code unconditionally, so a
 *  --no-witness cycle that minted EQUIVALENT_MATCH via the re-enabled
 *  memory-bus/SMT path would have been accepted). */
export function isCertifiedRow(
  row: { status: string; workflowStatus?: string } | undefined,
  witnessEnabled: boolean,
): boolean {
  return !!row
    && (row.status === "FULL_MATCH" || (witnessEnabled && row.status === "EQUIVALENT_MATCH"))
    && row.workflowStatus !== "BACKLOG";
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
  opts?: { sessionOwnsClaim?: boolean },
): Promise<boolean> {
  const witnessEnabled = config.witnessEnabled;
  console.log(
    `[pi-harness] ${unit}: witness-only cycle ${targetId} (no model session, no --smt${witnessEnabled ? ", witness" : ", --no-witness"})`,
  );
  // HIGH-2 guard: when the witness is disabled, NEVER run the cycle on a
  // target that is already EQUIVALENT_MATCH — cmd_cycle --no-witness would
  // demote it (status → CODE_MATCH, cert fields popped, irreversible). A
  // stray CERTIFY: marker can still parse (parseCertifyRequests is not
  // witness-aware), and a concurrent agent can flip the row between the
  // batch load and this cycle, so look the CURRENT status up here.
  if (!witnessEnabled) {
    const current = targetRowById(repoRoot).get(targetId);
    if (current?.status === "EQUIVALENT_MATCH") {
      process.stderr.write(
        `[pi-harness] ${unit}: ${targetId} is already EQUIVALENT_MATCH — would downgrade EQUIVALENT_MATCH without witness; skipping\n`,
      );
      return false;
    }
  }
  // When the target is already claimed by the batch session (certify-request
  // and 0-mismatch paths), do NOT re-claim or release: re-claim resets
  // claimed_at, and the finally-release would drop the session's own claim
  // mid-session (r5 finding 7) — another process could then claim the target
  // while we still edit it, and the batch-end releaseTargets would warn.
  // Only the triage path (target not session-owned) claims+releases itself.
  const sessionOwnsClaim = opts?.sessionOwnsClaim ?? false;
  if (!sessionOwnsClaim) {
    const claim = await claimTargets(repoRoot, config.pythonBin, [targetId], OWNER, (id) => activeClaims.add(id));
    if (claim.failed.length > 0) {
      process.stderr.write(
        `[pi-harness] ${unit}: ${targetId} claim failed for witness cycle — falling back to batch\n`,
      );
      return false;
    }
  }
  let certified = false;
  try {
    try {
      await execFilePromise(config.pythonBin, [
        // cmd_build now takes the repo build lock itself (build-only scope);
        // do NOT wrap the whole cycle — the witness after the build must run
        // lock-free (run30 incident: z3 spin under the lock froze all agents).
        "tools/coop/run.py", "cycle", targetId,
        "--witness-timeout", String(config.witnessTimeoutMs),
        ...(witnessEnabled ? [] : ["--no-witness"]),
        "--hypothesis",
        witnessEnabled
          ? "triage: reg-swap template vs nearest matched sibling — witness-only route (no LLM round)"
          : "triage: byte-identical chase (witness disabled) — FULL_MATCH only, no LLM round",
        "--next-change",
        witnessEnabled
          ? "accept if the register-renaming witness certifies; otherwise fall back to the LLM batch"
          : "accept if byte-identical (FULL_MATCH); otherwise fall back to the LLM batch",
      ], { cwd: repoRoot });
      // cycle exit 0 = required level met — but NEVER trust the exit code
      // alone: the registry records actual acceptance (adversarial-review H1).
      // A size-gate failure records FULL_MATCH with workflow BACKLOG while the
      // cycle still exits 0, and a --no-witness run can only legitimately
      // produce FULL_MATCH — re-read the row and apply the same predicate as
      // the non-zero branch below.
      const row = targetRowById(repoRoot).get(targetId);
      certified = isCertifiedRow(row, witnessEnabled);
      if (!certified) {
        process.stderr.write(
          `[pi-harness] ${unit}: witness cycle exited 0 for ${targetId} but the registry does not certify it ` +
          `(status ${row?.status ?? "UNKNOWN"}${row?.workflowStatus === "BACKLOG" ? ", size-gate BACKLOG" : ""}) — treated as not certified\n`,
        );
      }
    } catch (err) {
      // Non-zero exit: witness may not have certified, or the unit-level
      // split size gate failed. The registry records actual acceptance —
      // re-check instead of trusting the exit code alone. A size-gate
      // failure records FULL_MATCH but workflow BACKLOG (r5 finding 4):
      // status alone would falsely accept it.
      const row = targetRowById(repoRoot).get(targetId);
      const backlogged = row?.workflowStatus === "BACKLOG";
      if (isCertifiedRow(row, witnessEnabled)) {
        certified = true;
      } else {
        // r8 WS-1: surface the witness rejection gate (reloc/rho/execute/
        // structural + diverging component) so the failure is actionable —
        // run.py cycle now prints `witness-gate: <gate> | <reason>` on the
        // no-SMT witness rejection path.
        const e = err as { stdout?: string; stderr?: string };
        const gateMatch = `${e.stdout ?? ""}\n${e.stderr ?? ""}`.match(
          /witness-gate: (\S+?) \| ([^\n]*)/,
        );
        const gate = gateMatch ? ` (witness gate: ${gateMatch[1]}${gateMatch[2] ? ` — ${gateMatch[2]}` : ""})` : "";
        if (gateMatch) {
          witnessGateByTarget.set(targetId, `${gateMatch[1]}${gateMatch[2] ? ` — ${gateMatch[2]}` : ""}`);
          // LOW-3: persist the last witness gate to the ledger so a restart
          // (which loses the per-process witnessGateByTarget map) can still
          // key the near-match singleton diagnosis off the actual gate.
          appendLedger(repoRoot, config.ledgerPath, {
            ts: new Date().toISOString(), event: "witness-gate", tu: unit,
            detail: {
              targetId,
              gate: gateMatch[1],
              reason: gateMatch[2]?.trim() ?? "",
            },
          });
        }
        process.stderr.write(
          `[pi-harness] ${unit}: witness did not certify ${targetId} ` +
            `(status ${row?.status ?? "UNKNOWN"}${backlogged ? ", size-gate BACKLOG" : ""}${gate}) — re-added to batch pool\n`,
        );
      }
    }
  } finally {
    if (!sessionOwnsClaim) {
      await releaseTargets(repoRoot, config.pythonBin, [targetId], OWNER);
      activeClaims.delete(targetId);
    }
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

/** Parse `CERTIFY: <target-id>` markers from a session's final text.
 *  Returns ids that are also in the session's owned targets (a certify
 *  request for an unclaimed target is ignored — the harness only runs the
 *  accepting cycle for targets this session owns). */
export function parseCertifyRequests(finalText: string, ownedTargetIds: string[]): string[] {
  const owned = new Set(ownedTargetIds);
  return [...finalText.matchAll(/CERTIFY:\s*([A-Za-z0-9_-]+)/g)]
    .map((m) => m[1])
    .filter((id) => owned.has(id));
}

function makeVerifyCallback(opts: {
  repoRoot: string;
  config: HarnessConfig;
  unit: string;
  targetIds: string[];
  targetSymbols: Map<string, string>;
  writable: string[];
}): VerifySession {
  const { repoRoot, config, unit, targetIds, targetSymbols, writable } = opts;
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
      // Keep broken code in place so the model can fix it with full context.
      lastFeedback =
        `## Build Failure — Fix This First\n\n` +
        `Your code does not compile. Fix the errors below before ` +
        `continuing with any unmatched targets.\n\n` +
        `\`\`\`\n${build.output}\n\`\`\``;
      return { action: "re-prompt", feedback: lastFeedback };
    }

    // ── Acceptance (match check) — run before lint so we never
    //    skip match evaluation due to lint-related re-prompt caps. ──
    // Model-initiated certify requests: parse `CERTIFY: <target-id>` markers
    // from the final text and run the harness's witness cycle for each. The
    // certify tool is read-only; this is where the actual (safe) cycle runs
    // — build lock + claim check + registry re-verification, no SMT.
    const certifyIds = parseCertifyRequests(_finalText, targetIds);
    // Attempt ALL requested certifies (don't return on the first success —
    // the model may request several; each is an independent witness cycle).
    // Accept if ANY certified; the registry records each one individually.
    let certifyAccepted: string | null = null;
    for (const cid of certifyIds) {
      process.stderr.write(`[orchestrator] ${unit}: model requested certify for ${cid} — running witness cycle\n`);
      const certified = await runWitnessCycle(repoRoot, unit, cid, config, { sessionOwnsClaim: true });
      process.stderr.write(`[orchestrator] ${unit}: certify request for ${cid}: ${certified ? "CERTIFIED" : "not certified"}\n`);
      // Log EVERY certify attempt so the ledger fully reflects the certify
      // path (the runBatchCycle path logs batch-accept OR batch-cycle per
      // round; the certify path bypasses batch-cycle.py entirely and would
      // otherwise be silent on failed attempts).
      if (certified) {
        if (!certifyAccepted) certifyAccepted = cid;
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-accept", tu: unit,
          detail: { batchIndex: 0, attempt: 1, acceptedCount: 1, results: [{ targetId: cid, status: "FULL_MATCH" }], source: "certify-request" },
        });
      } else {
        appendLedger(repoRoot, config.ledgerPath, {
          // Distinct event: a failed certify is a witness cycle, NOT a model
          // session — logging it as batch-cycle would make countLedgerSessions
          // count it against the per-target session budget (M5).
          ts: new Date().toISOString(), event: "certify-failed", tu: unit,
          detail: { batchIndex: 0, attempt: 1, acceptedCount: 0, results: [{ targetId: cid, status: "COMPILES" }], source: "certify-request-failed" },
        });
      }
    }
    if (certifyAccepted) {
      // The witness cycles already updated the registry — read the batch
      // results WITHOUT re-running batch-cycle (readBatchResults is a pure
      // registry read). Without this, batchResults stays null and the caller
      // treats the whole batch as "internal error", pushing every target
      // (including the certified one) into pass-2 singleton retries
      // (adversarial review H6).
      batchResults = await readBatchResults(repoRoot, targetIds, config.witnessEnabled);
      return { action: "accept", reason: `CERTIFY: ${certifyAccepted}` };
    }

    process.stderr.write(`[orchestrator] ${unit}: runBatchCycle starting\n`);
    batchResults = await runBatchCycle(repoRoot, config.pythonBin, targetIds, config.witnessTimeoutMs, config.witnessEnabled);
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
    // Targets the model reported as matched (mismatch:0) or that are
    // reg-swap-only — these are DONE or certifiable; tell the model so it
    // stops editing them. Only truly-unmatched targets get hexdiff feedback.
    const matchedTargets: string[] = []; // mismatch:0 in the current state
    const unmatchedTargets: string[] = [];
    let hexdiffFailedCount = 0;
    const hexdiffFailedIds: string[] = [];
    for (const tid of targetIds) {
      const sym = targetSymbols.get(tid);
      if (!sym) continue;
      const hd = await runHexdiff(repoRoot, config.pythonBin, unit, sym);
      if (hd.ok) {
        if (hd.mismatchCount === 0) {
          matchedTargets.push(tid);
          // FULL MATCH (mismatch: 0): certify THIS state NOW via the witness
          // cycle — before any further model edits can regress it. The model
          // often reaches a perfect match mid-session then keeps editing (the
          // run-9 transcripts show 6/10 perfect matches lost to regression);
          // runBatchCycle already ran at the round start, so certify the exact
          // 0-mismatch state here under the build lock.
          process.stderr.write(`[orchestrator] ${unit}: ${tid} is at mismatch:0 — certifying now\n`);
          const certified = await runWitnessCycle(repoRoot, unit, tid, config, { sessionOwnsClaim: true });
          process.stderr.write(`[orchestrator] ${unit}: ${tid} 0-mismatch certify: ${certified ? "ACCEPTED" : "not certified (reloc/witness gate)"}\n`);
          if (certified) {
            appendLedger(repoRoot, config.ledgerPath, {
              ts: new Date().toISOString(), event: "batch-accept", tu: unit,
              detail: { batchIndex: 0, attempt: 1, acceptedCount: 1, results: [{ targetId: tid, status: "FULL_MATCH" }], source: "0-mismatch-certify" },
            });
            return { action: "accept", reason: `${tid} certified at mismatch:0` };
          }
          appendLedger(repoRoot, config.ledgerPath, {
            // Distinct event (see certify-request-failed above): a failed
            // 0-mismatch certify is a witness cycle, not a model session — do
            // not count it against the per-target session budget (M5).
            ts: new Date().toISOString(), event: "certify-failed", tu: unit,
            detail: { batchIndex: 0, attempt: 1, acceptedCount: 0, results: [{ targetId: tid, status: "COMPILES" }], source: "0-mismatch-certify-failed" },
          });
          continue;
        }
        unmatchedTargets.push(tid);
        hexdiffOutputs.push(`### ${tid} (\`${sym}\`) — ${hd.mismatchCount} mismatch(es)\n${hd.output}`);
        if (hd.structuralCount === 0 && hd.mismatchCount > 0) {
          regSwapOnlyTargets.push(tid);
        }
        // Bank the compiling draft (status from batch-cycle results).
        const status = batchResults?.find((r) => r.targetId === tid)?.status;
        // Stale-round tracking: no structural improvement for N rounds ->
        // stop THIS target's loop (cross-session budget still governs retries).
        // IMPORTANT: read the session-best BEFORE bankTarget — bankTarget
        // folds the current count into sessionBest, so comparing after it
        // would always see current >= best and increment staleRounds on every
        // round, even when the target improved (adversarial review H1).
        const prevBest = sessionBest.get(tid);
        await bankTarget(tid, status, rePromptCount);
        if (prevBest && hd.structuralCount >= prevBest.structural) {
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
        hexdiffFailedCount++;
        hexdiffFailedIds.push(tid);
        process.stderr.write(`[pi-harness] hexdiff failed for ${tid}: ${hd.output.slice(0, 200)}\n`);
      }
    }
    // Feedback: tell the model which targets are matched/certifiable and ONLY
    // show hexdiff for genuinely-unmatched targets. Never say "No Matches
    // Found" while listing 0-mismatch targets underneath, and NEVER say
    // "All Targets Matched" when hexdiff could not verify anything (run 9b:
    // CBattleManager's objects have null symbols — every hexdiff failed, but
    // the feedback falsely claimed "All Targets Matched", so the model
    // stopped working and produced empty re-prompts).
    const feedbackParts: string[] = [];
    if (matchedTargets.length > 0) {
      feedbackParts.push(
        `## Targets Are Byte-Identical (mismatch:0) but NOT Accepted

` +
        `These targets show mismatch:0 in hexdiff, but the acceptance cycle did ` +
        `NOT certify them (this session is still in the re-prompt path = 0 ` +
        `accepted). They are byte-identical but a ` +
        (config.witnessEnabled ? "witness/reloc/size gate" : "reloc/size gate (witness disabled)") +
        ` rejected them. **Do NOT edit the code bytes** — instead investigate ` +
        `the gate with ` +
        (config.witnessEnabled ? "`witness <unit> <symbol>` and " : "") +
        `\`unit-status <unit>\`: ` +
        matchedTargets.map((id) => `\`${id}\``).join(", ") + `.\n`,
      );
    }
    if (hexdiffFailedCount === targetIds.length) {
      // Every target's hexdiff failed — the objects may have null/stripped
      // symbols or the build is broken. Do NOT claim matches.
      feedbackParts.push(
        `## ⚠️ HEXDIFF UNAVAILABLE — no target could be verified\n\n` +
        `hexdiff failed for all ${hexdiffFailedCount} target(s). This usually means the ` +
        `decomp object has NULL/stripped symbols (the ELF parser cannot resolve ` +
        `function names) or the unit fails to build. The harness could NOT ` +
        `confirm any match. Use \`symbols <unit>\` and \`targets <id>\` to check ` +
        `what the registry/objects expose, and review the retail ASM manually. ` +
        `The acceptance cycle cannot certify targets it cannot resolve.`,
      );
    } else if (hexdiffFailedCount > 0) {
      // M1: partial failures were silently dropped — the model must know
      // which targets couldn't be verified (so it doesn't think they match).
      feedbackParts.push(
        `## ⚠️ Partial verification — ${hexdiffFailedCount} target(s) could not be diffed\n\n` +
        `hexdiff failed for: ${hexdiffFailedIds.map((id) => `\`${id}\``).join(", ")}. ` +
        `Their match status is UNKNOWN (null/stripped symbols or build issue). ` +
        `The targets listed elsewhere are the ones verified.`,
      );
    } else if (unmatchedTargets.length === 0 && matchedTargets.length === 0) {
      feedbackParts.push(`## ⚠️ Verification incomplete — nothing could be confirmed as matched.`);
    } else if (unmatchedTargets.length === 0) {
      // Every target shows mismatch:0 in hexdiff, but the acceptance cycle
      // did NOT certify them (we are in the re-prompt path = 0 accepted).
      // hexdiff byte-identity != acceptance: the witness can still reject on
      // reloc drift / size / structural gates. NEVER tell the model its work
      // is done here — it stops and the session dies (run 9b: CBattleManager
      // model stopped after being told 'All Targets Matched' while 0 were
      // accepted). Say what matched at byte level, then direct it to the
      // witness to find the gate.
      feedbackParts.push(
        `## ⚠️ Targets are byte-identical but NOT accepted\n\n` +
        `hexdiff shows mismatch:0 for all targets (${matchedTargets.length} byte-identical), ` +
        `BUT the acceptance cycle did NOT certify them — otherwise this session ` +
        `would have accepted. A ` +
        (config.witnessEnabled ? "witness/reloc/size gate" : "reloc/size gate (witness disabled)") +
        ` rejected them. ` +
        (config.witnessEnabled
          ? `Investigate with the \`witness\` tool (` +
            `\`witness <unit> <symbol>\`) on each: it reports the exact equivalence ` +
            `status. Likely causes:\n` +
            `- reloc drift (decomp has relocs where retail doesn't, or different names)\n` +
            `- unit split-size over budget\n` +
            `- the witness gate (nonvolatile preservation, ABI fixedness)`
          : `Likely causes:\n` +
            `- reloc drift (decomp has relocs where retail doesn't, or different names)\n` +
            `- unit split-size over budget`
        ) +
        `\n\n` +
        `Do NOT stop — find and fix the gate.`,
      );
    } else {
      feedbackParts.push(
        `## ${matchedTargets.length > 0 ? "Remaining Targets" : "No Matches Found"}\n\n` +
        `Your code compiled, but ${unmatchedTargets.length} target(s) still do not match the retail binary.` +
        (hexdiffOutputs.length > 0
          ? `\n\nHexdiff output for unmatched targets:\n\n${hexdiffOutputs.join("\n\n")}`
          : `\n\n(hexdiff unavailable — review the assembly patterns carefully)`) +
        `\n\nCommon causes:\n` +
        `- Wrong field offset or struct type\n` +
        `- Expression order (affects MWCC register allocation)\n` +
        `- Signed vs unsigned comparison\n` +
        `- Missing or extra function calls`,
      );
    }
    if (regSwapOnlyTargets.length > 0) {
      feedbackParts.push(config.witnessEnabled
        ? `### Reg-swap-only (structure correct)

` +
          `These targets have structural=0 — the code shape is right and only ` +
          `register allocation differs: ${regSwapOnlyTargets.join(", ")}. ` +
          `**CERTIFY THEM instead of editing**: call the \`witness\` tool (` +
          `\`witness <unit> <symbol>\`) to confirm, then \`certify <target-id>\` and ` +
          `include \`CERTIFY: <target-id>\` in your final response. The register-` +
          `renaming witness (no SMT) certifies these — do NOT keep editing or you ` +
          `will regress the match.`
        : `### Reg-swap-only (structure correct) but witness DISABLED

` +
          `These targets have structural=0 — the code shape is right and only ` +
          `register allocation differs: ${regSwapOnlyTargets.join(", ")}. The ` +
          `witness is DISABLED for this run, so they CANNOT be accepted as-is — ` +
          `you must eliminate the register differences too (reorder expressions, ` +
          `match MWCC allocation, fix field types) until hexdiff shows ` +
          `mismatch: 0. There is no \`witness\`/\`certify\` path.`);
    }
    lastFeedback = feedbackParts.join("\n\n");
    return { action: "re-prompt", feedback: lastFeedback };
  };

  const cleanup = async (): Promise<void> => {
    // No per-phase temp state to release (lint gating is TU-final only).
    return;
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
    exhaustionThreshold: config.exhaustionThreshold,
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
  const witnessTargets = config.witnessEnabled ? (selection.witnessTargets ?? []) : [];
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
        // LOW (GLM F5 / DeepSeek F7): when the witness is disabled the
        // regswap_only targets went to the LLM batch, NOT a witness cycle —
        // logging the raw triage routing would claim a witness run that
        // never happened.
        routedToWitness: config.witnessEnabled ? selection.triage.routedToWitness : [],
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
      witnessEnabled: config.witnessEnabled,
    });
    const prompt = buildBatchPrompt({
      brief, unit, targetIds: currentIds, pythonBin: config.pythonBin,
      witnessEnabled: config.witnessEnabled,
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
      snapshot = config.snapshotsEnabled
        ? await snapshotUnit(repoRoot, unit, writable)
        : null;

      const batchTargetSymbols = new Map(briefTargets.map(t => [t.id, t.symbol]));
      const verify = makeVerifyCallback({
        repoRoot, config, unit, targetIds: currentIds,
        targetSymbols: batchTargetSymbols, writable,
      });

      const sessionResult = await runAgentSession({
        repoRoot,
        modelRuntime,
        spec: config.matchModel,
        prompt,
        sessionDir: join(config.sessionDir, sanitized, `batch-${batchIndex}`),
        label: `batch-${batchIndex}-session-1`,
        python: config.pythonBin,
        writable,
        timeoutMinutes: config.maxBatchMinutes,
        maxTokens: config.maxTokens,
        witnessEnabled: config.witnessEnabled,
        silenceThresholdMs: config.silenceThresholdMs,
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
            // Include the batch's target ids so the per-target session budget
            // (countLedgerSessions / maxAttemptsPerTarget) counts batch
            // exhaustions, not just singleton ones (adversarial review M5).
            targetIds: currentIds,
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
    // r8 WS-2: near-match routing — targets at ≥90% instruction match (or
    // CODE_MATCH/HIGH_MATCH status) are the highest-conversion-probability
    // singletons and get priority. They ALSO override the singletonMinSize
    // rebatch routing (the motivating us-8025658c/us-8025650c were 188B/128B
    // — below the 500B rebatch threshold — so the plain size route would
    // have starved them exactly as the incident showed).
    const NEAR_MATCH_MIN = 90;
    const byId = new Map(targets.map((t) => [t.id, t]));
    const { nearMatch, rest } = partitionFailedTargets(firstPassFailed, byId, NEAR_MATCH_MIN);
    const ordered = [...nearMatch, ...rest];
    console.log(
      `[pi-harness] ${unit}: Pass 2 — ${firstPassFailed.length} failed targets ` +
      `(${nearMatch.length} near-match ≥${NEAR_MATCH_MIN}% prioritized)`,
    );

    for (const fid of ordered) {
      // Check if already matched (might have been matched in a previous batch)
      const target = targets.find((t) => t.id === fid);
      if (!target) continue;

      if (config.singletonMinSize > 0 && !nearMatch.includes(fid)) {
        // Route small targets to rebatch pool based on retail binary size.
        // Near-match targets override this (see above) so they get a
        // focused singleton with the diagnosis block.
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
        siblingsByTarget,
      );
      if (!ok) {
        handleSkipped(repoRoot, config, unit, fid, "exhausted singleton retries");
      }
    }
  }

  // ── Re-batch phase for small failed targets ─────────────────────
  if (smallRetryPool.length > 0 && !dryRun) {
    const { skipped: rebatchSkipped, targetCarryover: rebatchCarryover, rebatchDisabled } = await runRebatchPhase(
      repoRoot, unit, config, modelRuntime, sanitized,
      smallRetryPool, targets, carryover, config.maxRebatchAttempts,
      siblingsByTarget,
    );
    // Merge rebatch carryover into the main map for singleton routing.
    for (const [id, fb] of rebatchCarryover) {
      targetCarryover.set(id, fb);
    }
    if (rebatchDisabled) {
      // MEDIUM-3: rebatch DISABLED → small targets fall through to
      // singletons again (the pre-c447b53e5 fallthrough). Only a rebatch
      // that RAN and failed skips them (c447b53e5 policy).
      for (const fid of rebatchSkipped) {
        if (!config.singletonEnabled) {
          handleSkipped(repoRoot, config, unit, fid, "singleton retry disabled");
          continue;
        }
        const ok = await runSingleton(
          repoRoot, unit, fid, config, modelRuntime, sanitized,
          targetCarryover.get(fid) || carryover,
          siblingsByTarget,
        );
        if (!ok) {
          handleSkipped(repoRoot, config, unit, fid, "exhausted singleton retries");
        }
      }
    } else {
      // Small targets (below singletonMinSize, not near-match) never get
      // routed to singletons — rebatch was their one retry. Near-match small
      // targets already bypassed the size route in Pass 2 (they ran as
      // singletons there). So everything rebatch didn't process is skipped.
      for (const fid of rebatchSkipped) {
        handleSkipped(repoRoot, config, unit, fid, "below singletonMinSize — rebatch was the only retry");
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
    exhaustionThreshold: config.exhaustionThreshold,
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
): Promise<{ skipped: string[]; targetCarryover: Map<string, string>; rebatchDisabled: boolean }> {
  const uniqueSmall = [...new Set(smallRetryPool)];
  const targetCarryover = new Map<string, string>();

  if (!config.rebatchEnabled) {
    // MEDIUM-3: rebatch DISABLED is not a skip. Signal the caller so small
    // targets fall through to singletons again (the pre-c447b53e5
    // fallthrough) instead of being hard-skipped with a misleading
    // "routed to singletons" message. Only a rebatch that RAN and failed
    // skips its targets.
    console.log(`[pi-harness] ${unit}: rebatch disabled — ${uniqueSmall.length} small target(s) routed to singletons`);
    return { skipped: uniqueSmall, targetCarryover, rebatchDisabled: true };
  }

  if (maxAttempts <= 0) {
    console.log(`[pi-harness] ${unit}: rebatch budget auto-derived (0 = cover pool once) — ${uniqueSmall.length} small target(s)`);
  }

  // Cover the pool ONCE: each failed function is included in exactly one
  // rebatch session. If the configured budget is too small to fit the whole
  // pool (or 0 = auto), derive it from the pool size so no failed small
  // target is starved of its single rebatch attempt.
  const sessionsNeeded = sessionsNeededForPool(uniqueSmall.length, config.batchSize);
  let rebatchBudget = effectiveRebatchBudget(maxAttempts, uniqueSmall.length, config.batchSize);
  console.log(
    `[pi-harness] ${unit}: re-batching ${uniqueSmall.length} small target(s) ` +
    `(below ${config.singletonMinSize} bytes, budget: ${rebatchBudget} session(s))`,
  );

  const smallTargets = targets.filter((t) => uniqueSmall.includes(t.id));
  let sharedCarryover = carryover;
  const skipped: string[] = [];
  // MEDIUM-1(c): two consecutive group-level infrastructure errors (claim /
  // snapshot / build failures) mean rebatch itself is broken, not the
  // targets — stop the phase instead of burning budget on doomed groups.
  let consecutiveGroupErrors = 0;

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
        witnessEnabled: config.witnessEnabled,
      });
      const prompt = buildBatchPrompt({
        brief, unit, targetIds: rbCurrent, pythonBin: config.pythonBin,
        witnessEnabled: config.witnessEnabled,
      });

      let snapshot: Snapshot | null = null;
      let attemptError: string | null = null;
      let batchResults: Awaited<ReturnType<typeof runBatchCycle>> | null = null;
      // MEDIUM-1(b): set right after rebatchBudget--; the catch refund only
      // applies when the budget was actually decremented (claim/snapshot
      // failure throws before the decrement — the unconditional refund was a
      // net +1 budget inflation).
      let budgetDecremented = false;

      try {
        await claimBatch(repoRoot, config, rbCurrent);
        snapshot = config.snapshotsEnabled
          ? await snapshotUnit(repoRoot, unit, writable)
          : null;
        rebatchBudget--; // Only decrement after claim+snapshot succeed.
        budgetDecremented = true;

        const rbTargetSymbols = new Map(rbTargets.map(t => [t.id, t.symbol]));
        const verify = makeVerifyCallback({
          repoRoot, config, unit, targetIds: rbCurrent,
          targetSymbols: rbTargetSymbols, writable,
        });

        const sessionResult = await runAgentSession({
          repoRoot, modelRuntime, spec: config.rebatchModel, prompt,
          sessionDir: join(config.sessionDir, sanitized, `rebatch-${rbIdx}`),
          label: `rebatch-${rbIdx}-session`,
          python: config.pythonBin,
          writable,
          timeoutMinutes: config.maxBatchMinutes,
          maxTokens: config.maxTokens,
        witnessEnabled: config.witnessEnabled,
        silenceThresholdMs: config.silenceThresholdMs,
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
              // See pass-1 exhaustions (M5): budget must count these too.
              targetIds: rbCurrent,
            },
          });
          await releaseBatch(repoRoot, config, rbCurrent);
          // MEDIUM-1(a): one session per group — a failed session is NOT
          // re-run. Route the group to the caller as skipped (c447b53e5
          // policy: rebatch enabled but failed → skip), never re-loop.
          skipped.push(...rbCurrent);
          consecutiveGroupErrors = 0;
          rbCurrent = [];
          continue;
        }
      } catch (err) {
        attemptError = err instanceof Error ? err.message : String(err);
        const usage = usageFromError(err);
        if (usage) {
          logUsage(repoRoot, config, unit, `rebatch-${rbIdx}`, usage, false);
        }
      }

      if (attemptError !== null && batchResults === null) {
        let recoveryOutput = "";
        if (snapshot) {
          const recovery = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
          recoveryOutput = recovery.output;
          if (!recovery.ok) await restoreSnapshot(repoRoot, snapshot);
        }
        // No snapshot -> the claim or snapshotUnit failed before any session
        // ran, so no model edits were made for this batch; the worktree is
        // already the pre-loop state (nothing to restore). The previous
        // `restoreSnapshot(await snapshotUnit(...))` here was a no-op that
        // snapshotted the current state and immediately restored it.
        appendLedger(repoRoot, config.ledgerPath, {
          ts: new Date().toISOString(), event: "batch-error", tu: unit,
          detail: { batchIndex: rbIdx, error: attemptError, recoveryOutput, phase: "rebatch", rebatchBudgetRemaining: rebatchBudget },
        });
        await releaseBatch(repoRoot, config, rbCurrent);
        sharedCarryover = `Previous re-batch attempt failed unexpectedly:\n${attemptError}\n${recoveryOutput}`;
        for (const id of rbCurrent) targetCarryover.set(id, sharedCarryover);
        // MEDIUM-1(b): refund the session ONLY when it was actually
        // decremented (claim/snapshot failure throws before rebatchBudget--)
        // AND the error streak is below the cap — an infra-broken phase
        // stops without burning (or refunding) budget.
        if (budgetDecremented && consecutiveGroupErrors < 2) {
          rebatchBudget++;
        }
        // MEDIUM-1(a): the error'd group is attempted exactly once — clear
        // it so the while loop cannot re-run the same targets.
        // MEDIUM-1(c): two consecutive group errors stop the phase.
        skipped.push(...rbCurrent);
        rbCurrent = [];
        if (consecutiveGroupErrors >= 2) {
          process.stderr.write(
            `[pi-harness] ${unit}: rebatch aborting after ${consecutiveGroupErrors} consecutive group errors — remaining groups skipped\n`,
          );
          rebatchBudget = 0;
        }
        continue;
      }

      if (batchResults === null) {
        await releaseBatch(repoRoot, config, rbCurrent);
        sharedCarryover = "Previous re-batch attempt produced no results.";
        for (const id of rbCurrent) targetCarryover.set(id, sharedCarryover);
        // MEDIUM-1(a): no-results groups are attempted once too; route them
        // to the caller as skipped (never re-run). Counts toward the
        // consecutive-error streak (internal-error family).
        consecutiveGroupErrors++;
        skipped.push(...rbCurrent);
        rbCurrent = [];
        if (consecutiveGroupErrors >= 2) rebatchBudget = 0;
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

      consecutiveGroupErrors = 0;
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

  return { skipped, targetCarryover, rebatchDisabled: false };
}

// ─────────────────────────────────────────────────────────────────────
//  Singleton retry
// ─────────────────────────────────────────────────────────────────────

async function runSingleton(
  repoRoot: string, unit: string, targetId: string, config: HarnessConfig,
  modelRuntime: ModelRuntime, sanitized: string,
  carryover: string,
  siblingsByTarget?: Map<string, SiblingPointer[]>,
): Promise<boolean> {
  const targets = loadUnmatchedTargets(repoRoot, config.region, unit, {
    // LOW-2: apply the SAME exhaustion filter as every other load site so
    // the singleton budget bookkeeping is consistent (previously a config
    // object could not be passed directly — the field is exhaustionThreshold).
    ledgerPath: config.ledgerPath,
    retryExhausted: config.retryExhausted,
    exhaustionThreshold: config.exhaustionThreshold,
  }).filter(
    (t) => t.id === targetId,
  );
  if (targets.length === 0) {
    // LOW-2: the target is not in the unmatched+unexhausted set — it either
    // matched meanwhile (fine) or the ledger excluded it (exhausted). Report
    // skip-not-success instead of "already matched": claiming a run happened
    // when none did misreports the retry outcome to the caller.
    process.stderr.write(
      `[pi-harness] ${unit}: ${targetId} not found in unmatched targets — already matched or ledger-exhausted; skipping singleton\n`,
    );
    return false;
  }

  const target = targets[0];
  const writable = writableScopeForTargets(repoRoot, [target]);
  const { briefs: rawBriefs, missingAsm } = buildBriefs(repoRoot, [target], siblingsByTarget);
  const briefs = await attachBankedDrafts(repoRoot, config, rawBriefs);
  if (missingAsm.length > 0) return false;

  let feedback = carryover;

  // r8 WS-2 / LOW-3: for near-match targets (≥90% or CODE/HIGH_MATCH),
  // prepend the last witness rejection gate as a diagnosis block so the
  // model gets the actionable failure (reloc drift / rho / callee /
  // structural + diverging component) instead of re-deriving it. The gate
  // comes from the per-process map first, then the persisted ledger event
  // (survives restarts — the map is per-process only). The advice is keyed
  // off the ACTUAL gate, not a hard-coded reloc/structural text. When the
  // witness is DISABLED the block is skipped entirely: stale witness-gate
  // ledger events from prior witness-enabled runs must not surface a false
  // "the witness rejected it" header on a run that never consulted it
  // (adversarial review GLM F3).
  const near = target.status === "CODE_MATCH" || target.status === "HIGH_MATCH"
    || (target.instructionMatch ?? 0) >= 90;
  const persistedGate = lastWitnessGate(repoRoot, config, targetId);
  const gate = witnessGateByTarget.get(targetId) ?? persistedGate?.gate;
  const gateReason = witnessGateByTarget.has(targetId)
    ? undefined
    : persistedGate?.reason;
  if (config.witnessEnabled && near && gate) {
    feedback =
      `## Near-match diagnosis (witness gate: ${witnessGateName(gate)}${gateReason ? ` — ${gateReason}` : ""})\n\n` +
      `This target is close to acceptance but the witness rejected it at the ` +
      `gate above.\n\n${witnessHintForGate(gate, gateReason)}\n\n${feedback}`;
  }

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
  const maxAttempts = Math.min(config.maxSingletonSessions, budgetRemaining);
  console.log(`[pi-harness] ${unit}: starting singleton for ${targetId} (up to ${maxAttempts} session(s); ledger says ${ledgerAttempts} so far)`);

  for (let sessionAttempt = 1; sessionAttempt <= maxAttempts; sessionAttempt++) {
    const brief = buildBatchBrief({
      targets: briefs, unit, writable, carryover: feedback,
      maxChars: config.maxBriefChars, pythonBin: config.pythonBin,
      repoRoot, knownWallsPath: config.knownWallsPath,
      briefTargetChars: config.briefTargetChars,
      witnessEnabled: config.witnessEnabled,
    });
    const prompt = buildBatchPrompt({
      brief, unit, targetIds: [targetId], pythonBin: config.pythonBin,
      witnessEnabled: config.witnessEnabled,
    });

    let snapshot: Snapshot | null = null;
    try {
      await claimBatch(repoRoot, config, [targetId]);
      snapshot = config.snapshotsEnabled
        ? await snapshotUnit(repoRoot, unit, writable)
        : null;

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
        targetSymbols: singletonSymbols, writable,
      });

      const sessionResult = await runAgentSession({
        repoRoot, modelRuntime, spec: config.singletonModel, prompt,
        sessionDir: join(config.sessionDir, sanitized, `singleton-${targetId}`),
        label: `singleton-${targetId}-session-${sessionAttempt}`,
        python: config.pythonBin,
        writable,
        timeoutMinutes: config.maxBatchMinutes,
        maxTokens: config.maxTokens,
        witnessEnabled: config.witnessEnabled,
        silenceThresholdMs: config.silenceThresholdMs,
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
        feedback = `Previous singleton session compiled but was not accepted by batch-cycle (status: ${results?.[0]?.status ?? "unknown"}). Review the hexdiff/witness state and continue.`;
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
  let writable = writableScopeForTargets(
    repoRoot,
    loadUnitTargets(repoRoot, config.region, unit),
  );
  // The TU-final model is instructed to flip the unit's object in
  // configure.py from NonMatching to Matching. Include it in the writable
  // scope so (a) the scoped edit/write tools allow the flip (H4), and (b) it
  // is snapshotted/restored with everything else — a failed TU-final now
  // reverts a model-made configure.py flip instead of leaving the unit
  // marked Matching with regressed code (the previous restore never covered
  // configure.py because it was outside the writable scope).
  if (!writable.includes("configure.py")) {
    writable = [...writable, "configure.py"];
  }
  const sourceFiles = writable.filter(
    (f) => f.endsWith(".cpp") || f.endsWith(".c") || f.endsWith(".hpp") || f.endsWith(".h"),
  );

  if (dryRun) {
    console.log(`\n[pi-harness] DRY-RUN: TU-final phase for ${unit}`);
    console.log(`  source files: ${sourceFiles.join(", ")}`);
    return;
  }

  const snapshot: Snapshot | null = config.snapshotsEnabled
    ? await snapshotUnit(repoRoot, unit, writable)
    : null;

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
  let lintViolations: LintOutcome["violations"] = [];
  let lastReason = "";
  let recertOk = true;
  let recertOutput = "";
  // ── Regression-aware state tracking (tufinal-scan.ts) ──
  // Baseline captured at session start; each attempt's state is scored
  // lexicographically (match regressions > TU size/data > lint). The best-
  // scoring snapshot is restored ONCE at the very end — never mid-session
  // (the model may be working toward something).
  const baselineScan: UnitScan = await scanUnitState(repoRoot, config.pythonBin, unit, config.witnessEnabled);
  // M2: seed bestScore with the BASELINE itself (zero regressions by
  // definition) so a state worse than session start never wins the end-of-
  // session restore. bestSnapshot stays null → the pristine snapshot is the
  // fallback (it IS the baseline).
  let bestScore: bigint = scoreState(baselineScan, baselineScan, Infinity);
  let bestScan: UnitScan | null = null;
  let bestSnapshot: Snapshot | null = null;
  let bestLintCount = Infinity;

  for (let attempt = 1; attempt <= maxTuFinalAttempts; attempt++) {
    const prompt = feedback
      ? basePrompt + `\n\n## TU-final rejection feedback (attempt ${attempt - 1})\n\n${feedback}\n\nThe previous attempt's changes were ${
          lastReason.startsWith("kept") ? "KEPT (best-scoring state so far)" : "NOT reverted mid-session (the model may be building toward something; the best state restores at the end)"
        }. Apply the fixes and retry.\n`
      : basePrompt;

    if (attempt > 1) {
      // NO mid-session restore: the model may be working toward something,
      // and reverting between attempts discards that progress. The best-
      // scoring state is snapshotted per-attempt and restored ONCE at the
      // very end (after all attempts). If the worktree is left build-broken
      // by a prior attempt, the next attempt sees the error in feedback and
      // fixes forward.
      process.stderr.write(`[pi-harness] ${unit}: TU-final attempt ${attempt}/${maxTuFinalAttempts} (retry after ${lastReason}) — no mid-session restore\n`);
      appendLedger(repoRoot, config.ledgerPath, {
        ts: new Date().toISOString(), event: "tu-final-retry", tu: unit,
        detail: { attempt, reason: lastReason, feedback: feedback.slice(0, 500) },
      });
    }

    try {
      sessionResult = await runAgentSession({
        repoRoot, modelRuntime, spec: config.cleanupModel, prompt,
        sessionDir: join(config.sessionDir, sanitized, "tu-final"),
        label: attempt === 1 ? "tu-final" : `tu-final-retry-${attempt}`,
        kind: "tu-final",
        python: config.pythonBin,
        writable,
        timeoutMinutes: tuFinalTimeout,
        maxTokens: config.maxTokens,
        witnessEnabled: config.witnessEnabled,
        silenceThresholdMs: config.silenceThresholdMs,
      });
      logUsage(repoRoot, config, unit, attempt === 1 ? "tu-final" : `tu-final-retry-${attempt}`, sessionResult.usage, sessionResult.timedOut);
    } catch (err) {
      const usage = usageFromError(err);
      if (usage) logUsage(repoRoot, config, unit, `tu-final-retry-${attempt}`, usage, false);
      lastReason = "session-error";
      feedback = `The TU-final session itself failed: ${err instanceof Error ? err.message : String(err)}`;
      if (attempt < maxTuFinalAttempts) continue;
      // Fall through to the end-of-session restore: break out of the loop
      // instead of returning early, so a final-attempt session error still
      // restores the pristine snapshot (the previous early return skipped the
      // restore and left the last attempt's state in the worktree —
      // adversarial review F11).
      buildOk = false;
      recertOk = false;
      break;
    }

    // ── Regression-aware state capture (AFTER each session, BEFORE gates) ──
    // Scan the unit (hexdiff --all --json + size + data%), diff vs baseline,
    // score, and snapshot the best-scoring state so far. A state that
    // regressed a match is scored worst and NEVER kept — even if it reduced
    // lint. Restore happens only at the very end, to the best snapshot.
    const currentScan: UnitScan = await scanUnitState(repoRoot, config.pythonBin, unit, config.witnessEnabled);
    // Lint only the source files, not configure.py (which is in the writable
    // scope for the flip but is not a C/C++ file runLint can diff).
    const lintSnapshot: Snapshot | null = snapshot
      ? { ...snapshot, files: snapshot.files.filter(
        (f) => f.endsWith(".cpp") || f.endsWith(".c") || f.endsWith(".hpp") || f.endsWith(".h"),
      ) }
      : null;
    const lintAfter = await runLint(repoRoot, config.pythonBin, lintSnapshot, sourceFiles);
    lintViolations = lintAfter.violations;
    const currentScore = scoreState(currentScan, baselineScan, lintAfter.violations.length);
    const unitFeedback = buildUnitFeedback(baselineScan, currentScan, lintAfter.violations.length);
    if (currentScore < bestScore) {
      bestScore = currentScore;
      bestScan = currentScan;
      bestSnapshot = config.snapshotsEnabled
        ? await snapshotUnit(repoRoot, unit, writable)
        : null;
      bestLintCount = lintAfter.violations.length;
      process.stderr.write(
        `[pi-harness] ${unit}: attempt ${attempt} is the best state so far ` +
        `(score ${currentScore}, lint ${lintAfter.violations.length}, matched ${currentScan.matched}/${currentScan.total}) — snapshotted\n`,
      );
    }

    // ── Build gate: unit compile check. A broken build is scored worst
    //    (scanUnitState returns empty on build failure) and the next attempt
    //    fixes forward — no mid-session restore. ──
    const unitBuild = await buildUnit(repoRoot, config.pythonBin, config.region, unit);
    if (!unitBuild.ok) {
      lastReason = "build-failed";
      feedback =
        `The unit does not compile after the polish. Error tail:\n\`\`\`text\n` +
        unitBuild.output.slice(-1500) +
        `\n\`\`\`\n\nFix the compile error and retry.`;
      if (attempt < maxTuFinalAttempts) continue;
      break;
    }

    // ── Feedback from the regression-aware scan (built in the capture
    //    block above; includes unit size/data, loud regressions, and lint).
    //    Lint no longer preempts: matches/regressions/size are shown first,
    //    lint violations follow. If lint is clean AND the scan is best-so-far
    //    with no regressions, this attempt succeeds. ──
    lastReason = "regression-or-lint";
    const rules = [...new Set(lintViolations.map((v) => v.rule))].join(", ");
    feedback = unitFeedback +
      (lintViolations.length > 0
        ? `\n\n## Lint Violations (${lintViolations.length})\n\nRules: ${rules}\n` +
          lintViolations.slice(0, 10).map(
            (v) => `- ${v.path}:${v.line ?? "?"} [${v.rule}] ${v.detail}`,
          ).join("\n") +
          `\n\nFix ONLY these violations — do not redo the whole finalisation.`
        : `\n\n## Lint clean ✅`);

    // Only a lint-clean attempt proceeds to the full-tree rebuild + size
    // gate. Lint violations re-prompt (the scan feedback shows what matters).
    if (lintViolations.length > 0) {
      if (attempt < maxTuFinalAttempts) continue;
      break;
    }

    // ── Regression gate (C2): lint-clean + compiles is NOT 'done' — a
    //    matched function that the polish regressed must reject this attempt.
    //    'A regression is never acceptable' (the TU-final contract). ──
    const regressions = diffUnitScans(baselineScan, currentScan).filter((d) => d.regressed);
    if (regressions.length > 0) {
      lastReason = "regression";
      feedback =
        `## ⚠️ MATCH REGRESSION — ${regressions.length} function(s) regressed (not acceptable)\n\n` +
        `The polish REGRESSED previously-matched functions. This attempt is ` +
        `REJECTED even though it compiles and passes lint — a regression is ` +
        `never acceptable. Fix these back to their baseline:\n\n` +
        regressions.map((d) =>
          `- \`${d.symbol}\`: structural ${d.before.structural}→${d.after.structural}, ` +
          `mismatch ${d.before.mismatch}→${d.after.mismatch}` +
          (d.witnessBefore === true && d.witnessAfter === false
            ? ` — WITNESS NO LONGER CERTIFIES`
            : ""),
        ).join("\n") +
        `\n\nRun \`unit-status <unit>\` for the full state, restore the regressed ` +
        `functions to their prior implementations, and retry.`;
      if (attempt < maxTuFinalAttempts) continue;
      break;
    }

    // ── Build gate (2/2): full-tree rebuild (configure.py + ninja) — runs
    //    only after the unit compiles AND lint is clean. ──
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

    // ── Mass re-certification gate: after the full rebuild, re-run the
    //    acceptance cycle (batch-cycle → run.py cycle → register-renaming
    //    witness, NO SMT) over every target in the unit. This re-certifies
    //    each FULL_MATCH/EQUIVALENT_MATCH after the polish — catching any
    //    target that silently stopped certifying (reloc drift, size, ABI
    //    gate) that the regression sweep detected but didn't re-certify.
    //    The unit is only 'done' if every target re-certifies. When the
    //    witness is DISABLED, EQUIVALENT_MATCH targets are EXCLUDED from the
    //    recert list (adversarial-review GLM F1 / DeepSeek F3): batch-cycle
    //    --no-witness cannot re-prove them, so cycling demotes them
    //    (status → CODE_MATCH, cert fields popped) — irreversible, and the
    //    snapshot does not cover targets.json. ──
    let recertOkLocal = true;
    let recertOutputLocal = "";
    if (buildOk) {
      const unitTargets = loadUnitTargets(repoRoot, config.region, unit)
        .filter((t) => t.status === "FULL_MATCH" || t.status === "ACCEPTED" ||
          (config.witnessEnabled && t.status === "EQUIVALENT_MATCH"));
      if (unitTargets.length > 0) {
        process.stderr.write(`[pi-harness] ${unit}: mass re-certification of ${unitTargets.length} target(s) after polish\n`);
        try {
          const { stdout } = await execFilePromise(config.pythonBin, [
            "tools/pi_harness/build_lock.py", "--timeout", "1800", config.region, "--",
            config.pythonBin, "tools/coop/batch-cycle.py",
            "--default-hypothesis", "TU-final mass re-certification after polish",
            "--default-next-change", "none (re-certify only)",
            "--witness-timeout", String(config.witnessTimeoutMs),
            ...(config.witnessEnabled ? [] : ["--no-witness"]),
            "--", ...unitTargets.map((t) => t.id),
          ], { cwd: repoRoot });
          recertOutputLocal = stdout.slice(-1200);
        } catch (err) {
          recertOutputLocal = (err instanceof Error ? err.message : String(err)).slice(-1200);
        }
        // batch-cycle exits non-zero when ANY target fails — re-check the
        // registry for the actual accepted set.
        const postStatus = new Map(loadUnitTargets(repoRoot, config.region, unit).map((t) => [t.id, t.status]));
        // A size-gate-failed target records FULL_MATCH but workflow BACKLOG —
        // treat it as NOT re-certified (same gate as runBatchCycle/runWitnessCycle,
        // adversarial review H2).
        const postWorkflow = new Map(
          loadUnitTargets(repoRoot, config.region, unit)
            .map((t) => [t.id, (t as { workflow_status?: string }).workflow_status]),
        );
        const notAccepted = unitTargets.filter((t) => {
          const ok = postStatus.get(t.id) === "FULL_MATCH"
            || (config.witnessEnabled && postStatus.get(t.id) === "EQUIVALENT_MATCH")
            || postStatus.get(t.id) === "ACCEPTED";
          return !(ok && postWorkflow.get(t.id) !== "BACKLOG");
        });
        if (notAccepted.length > 0) {
          recertOkLocal = false;
          lastReason = "recert-failed";
          feedback =
            `## ⚠️ Mass re-certification failed — ${notAccepted.length} target(s) no longer certify\n\n` +
            `After the full rebuild, these previously-accepted targets did NOT re-certify ` +
            `(the polish changed their compiled output):\n\n` +
            notAccepted.map((t) => `- \`${t.id}\` (\`${t.symbol}\`)`).join("\n") +
            `\n\nRun \`unit-status <unit>\` to see the per-function state, fix the ` +
            `regression, and retry.`;
          if (attempt < maxTuFinalAttempts) continue;
        }
      }
    }
    recertOk = recertOkLocal;
    recertOutput = recertOutputLocal;
    break; // success
  }

  // ── End-of-session restore: the ONLY restore point. If we did not finish
  //    with a clean lint + build, restore the best-scoring state captured
  //    across attempts (match-regression-aware). When NO attempt beat the
  //    baseline (bestSnapshot stays null — e.g. every attempt regressed a
  //    match or build-broke), restore the pristine pre-session snapshot: the
  //    comment previously claimed this fallback but the code did not do it,
  //    leaving the last attempt's (possibly broken) state in the worktree
  //    (adversarial review M2).
  const done = buildOk && recertOk;
  if (!done) {
    if (bestSnapshot) {
      await restoreSnapshot(repoRoot, bestSnapshot);
      process.stderr.write(
        `[pi-harness] ${unit}: TU-final restored best-scoring state (score ${bestScore}, lint ${bestLintCount}, matched ${bestScan?.matched ?? "?"}/${bestScan?.total ?? "?"})\n`,
      );
    } else if (snapshot) {
      // Pristine fallback: restore the pre-session snapshot.
      await restoreSnapshot(repoRoot, snapshot);
      process.stderr.write(
        `[pi-harness] ${unit}: TU-final restored PRISTINE pre-session state (no attempt beat the baseline)\n`,
      );
    } else {
      // Snapshots disabled — nothing to restore; leave the worktree as-is.
      process.stderr.write(
        `[pi-harness] ${unit}: TU-final incomplete with snapshots disabled — worktree left as-is\n`,
      );
    }
  }

  appendLedger(repoRoot, config.ledgerPath, {
    ts: new Date().toISOString(),
    event: done ? "tu-final-done" : "tu-final-failed",
    tu: unit,
    detail: {
      buildOk, recertOk, recertOutput: recertOutput.slice(-600), sizeOutput, attempts: done ? 1 : maxTuFinalAttempts,
      finalTextPreview: (sessionResult?.finalText ?? "").slice(0, 1000),
      bestScore: bestScore === null ? undefined : bestScore.toString(),
      bestLintCount: bestLintCount === Infinity ? undefined : bestLintCount,
      bestMatched: bestScan?.matched,
      bestTotal: bestScan?.total,
      restoredBest: !done && !!bestSnapshot,
    },
  });
  console.log(
    done
      ? `[pi-harness] ${unit}: TU-final complete (${bestScan?.matched ?? "?"}/${bestScan?.total ?? "?"} matched, mass re-certified)`
      : `[pi-harness] ${unit}: TU-final did not finish clean after ${maxTuFinalAttempts} attempt(s) ` +
        (lastReason === "recert-failed" ? "(mass re-certification failed)" : "") +
        (bestSnapshot ? ` (restored best-scoring state, lint ${bestLintCount}, matched ${bestScan?.matched ?? "?"}/${bestScan?.total ?? "?"})` : ""),
  );

  if (done) {
    // L2: fall back to the baseline matched count instead of 0 when no
    // attempt beat the baseline (bestScan stays null).
    const matchedAtDone = bestScan?.matched ?? baselineScan.matched ?? 0;
    await commitUnitOnTuFinal(repoRoot, config, unit, writable, matchedAtDone);
  }
}

/**
 * Commit a unit's source files + configure.py flip to git after a
 * successful TU-final. Stages ONLY the unit's writable-scope files, plus the
 * registry (targets.json + certs sidecar) and the generated docs refreshed
 * by the pre-commit scripts (never `git add -A`), so unrelated dirty state
 * on other paths is never swept in. Best-effort: any failure (dirty
 * porcelain, mid-edit file, git error) is logged and swallowed — a commit
 * must never fail the run or block acceptance.
 *
 * r11 review (M1/M2/M3):
 *  - M1 configure.py is repo-global and dirty by design — the commit stages
 *    the whole file, which may bundle other units'/agents' uncommitted
 *    flips. The message therefore does NOT claim exclusive ownership of the
 *    configure.py diff; it names the unit and reports the flip as part of
 *    this TU-final. (Staging only this unit's hunk would need `git add -p`
 *    scripting; accepted tradeoff for now.)
 *  - M2 the registry (targets.json + targets.certs.jsonl.gz) is staged too,
 *    so the committed tree's acceptance state matches the source.
 *  - M3 generated docs are refreshed via the same scripts the pre-commit
 *    hook runs (progress_map, readme_status, docs_sync, smell_report), then
 *    staged — the harness commit carries fresh numbers and CI freshness
 *    gates (smell_report --check / docs_sync --check) stay green.
 */
async function commitUnitOnTuFinal(
  repoRoot: string,
  config: HarnessConfig,
  unit: string,
  files: string[],
  matchedCount: number,
): Promise<void> {
  if (!config.commitOnTuFinal) return;
  const python = config.pythonBin;
  const paths = [...new Set(files.filter((f) => existsSync(join(repoRoot, f))))];
  if (paths.length === 0) return;
  const run = async (args: string[]): Promise<void> => {
    await execFilePromise("git", args, { cwd: repoRoot });
  };
  try {
    // Stage ONLY this unit's files plus the registry + docs. If a file is
    // not tracked or was deleted by the session, `git add` still stages the
    // state; nothing else is touched.
    await run(["add", "--", ...paths]);
    // M2: registry + certs sidecar reflect this TU-final's acceptances.
    for (const reg of ["tools/coop/targets.json", "tools/coop/targets.certs.jsonl.gz"]) {
      if (existsSync(join(repoRoot, reg))) await run(["add", "--", reg]);
    }
    // M3: refresh generated docs (same scripts as the pre-commit hook) so
    // the commit is CI-fresh. Best-effort: a failing refresh script is
    // logged but does not abort the commit (CI enforces on push anyway).
    const refreshScripts: Array<[string, string[]]> = [
      [python, ["tools/coop/progress_map.py"]],
      [python, ["tools/coop/readme_status.py", "--write"]],
      [python, ["-m", "tools.ppc_equivalence.docs_sync", "--write"]],
      [python, ["tools/coop/smell_report.py", "--write"]],
    ];
    for (const [cmd, args] of refreshScripts) {
      try {
        await execFilePromise(cmd, args, { cwd: repoRoot });
      } catch (err) {
        process.stderr.write(
          `[pi-harness] ${unit}: doc refresh failed (${args.join(" ")}): ${err instanceof Error ? err.message : String(err)}\n`,
        );
      }
    }
    await run(["add", "--", "README.md", "assets/progress-map.svg",
      "tools/ppc_equivalence/README.md", "tools/ppc_equivalence/SOUNDNESS.md",
      "docs/CODE_SMELLS.md"]);
    // L3: commit with an explicit pathspec so a leftover staged path from a
    // prior failed commit can never be swept into this one.
    // L1: verify the configure.py flip actually happened before claiming it
    // in the message (done=true proves build+recert, not the flip itself).
    let flipClaim = "";
    try {
      const cfgText = readFileSync(join(repoRoot, "configure.py"), "utf-8");
      // Configure.py names the unit with a .cpp suffix and the flip is
      // Object(NonMatching, ...) -> Object(Matching...). MatchingFor(...) is
      // a helper used for region-tagged objects; treat any Object(Matching*,
      // "<unit>.cpp") as flipped.
      const q = unit.replace(/[.\\^$|?*+()\[\]{}]/g, "\\$&");
      const reM = new RegExp(`Object\\(Matching[^)]*\\)?,?\\s*\\"${q}\\.cpp`);
      const reN = new RegExp(`Object\\(NonMatching,\\s*\\\"${q}\\.cpp`);
      if (reM.test(cfgText)) flipClaim = ", configure.py flipped to Matching";
      else if (reN.test(cfgText)) flipClaim = " (configure.py still NonMatching)";
      else flipClaim = " (unit not found in configure.py)";
    } catch {
      // configure.py unreadable — omit the flip claim entirely.
    }
    const msg = `pi-harness: ${unit} TU-final complete (${matchedCount} matched${flipClaim})`;
    await run(["commit", "-m", msg, "--no-verify", "--", ...paths,
      "tools/coop/targets.json", "tools/coop/targets.certs.jsonl.gz",
      "README.md", "assets/progress-map.svg",
      "tools/ppc_equivalence/README.md", "tools/ppc_equivalence/SOUNDNESS.md",
      "docs/CODE_SMELLS.md"]);
    process.stderr.write(`[pi-harness] ${unit}: committed TU-final (${paths.length} source file(s) + registry + docs)\n`);
  } catch (err) {
    // Best-effort only. A dirty index or concurrent agent edit is expected;
    // unstage what we staged so the next commit attempt starts clean.
    process.stderr.write(
      `[pi-harness] ${unit}: TU-final commit skipped (best-effort): ${err instanceof Error ? err.message : String(err)}\n`,
    );
    try {
      await run(["reset", "--", ...paths]);
    } catch {
      // ignore unstage failure — nothing else we can do
    }
  }
}

function handleSkipped(
  repoRoot: string, config: HarnessConfig, unit: string, targetId: string, reason: string,
): void {
  // "no retail asm" is a TRANSIENT condition (the listing may be generated
  // later); it must not permanently exhaust the target. target-skipped rows
  // feed scanExhaustedTargets, which permanently excludes the target from
  // future waves (adversarial review N). Use the distinct target-deferred
  // event for transient skips so they stay retryable.
  const event = reason === "no retail asm" ? "target-deferred" : "target-skipped";
  appendLedger(repoRoot, config.ledgerPath, {
    ts: new Date().toISOString(), event, tu: unit,
    detail: { targetId, reason },
  });
}

/** Last persisted `witness-gate` ledger event for a target (LOW-3). The
 *  per-process witnessGateByTarget map is lost on restart; the ledger row
 *  written by runWitnessCycle survives, so runSingleton can still key its
 *  near-match diagnosis off the actual gate. Returns the most recent row.
 */
function lastWitnessGate(
  repoRoot: string, config: HarnessConfig, targetId: string,
): { gate: string; reason?: string } | undefined {
  const events = readLedger(repoRoot, config.ledgerPath);
  for (let i = events.length - 1; i >= 0; i--) {
    const e = events[i];
    if (e.event !== "witness-gate") continue;
    const d = e.detail ?? {};
    if (d.targetId === targetId && typeof d.gate === "string" && d.gate) {
      return {
        gate: d.gate,
        reason: typeof d.reason === "string" && d.reason ? d.reason : undefined,
      };
    }
  }
  return undefined;
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
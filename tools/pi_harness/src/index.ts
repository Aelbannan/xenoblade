#!/usr/bin/env node
/**
 * CLI entry point for the pi-harness.
 *
 * ```bash
 * npm --prefix tools/pi_harness run pi-harness -- \
 *   --tu kyoshin/CGame [--tu other/Unit] [--dry-run] [--max-parallel 2] [--config path]
 *   --all --order smallest --match-less-than 50
 * ```
 *
 * May be run from anywhere inside the repository.
 *
 * @module index
 */

import { chdir, cwd } from "node:process";
import { resolve, dirname, join } from "node:path";
import { existsSync } from "node:fs";
import { loadConfig } from "./config.js";
import { loadUnmatchedTargets, loadAllUnitSummaries, unitHasActionableWork, type UnitOrder, type UnitSummary } from "./targets.js";
import { runTus } from "./orchestrator.js";
import type { HarnessConfig } from "./types.js";

/** Walk up from `start` (max 4 parents) until tools/coop/targets.json is found. */
function findRepoRoot(start: string): string {
  let dir = start;
  for (let i = 0; i <= 4; i++) {
    if (existsSync(resolve(dir, "tools/coop/targets.json"))) return dir;
    const parent = dirname(dir);
    if (parent === dir) break;
    dir = parent;
  }
  throw new Error(
    `Could not locate repo root (tools/coop/targets.json) from ${start}. ` +
      "Run from within the xenoblade repository.",
  );
}

interface Args {
  tus: string[];
  configPath?: string;
  dryRun: boolean;
  maxParallel?: number;
  showStatus: boolean;
  all: boolean;
  order: UnitOrder;
  matchLessThan?: number;
  greenfield: boolean;
}

function parseArgs(argv: string[]): Args {
  const args: Args = { tus: [], dryRun: false, showStatus: false, all: false, order: "most-remaining", greenfield: false };
  const rest = argv.slice(2);

  for (let i = 0; i < rest.length; i++) {
    const a = rest[i];
    switch (a) {
      case "--tu": {
        const v = rest[++i];
        if (!v || v.startsWith("--")) throw new Error("--tu requires a value");
        if (v.startsWith("/") || v.split(/[\\/]/).includes("..")) {
          throw new Error(`unsafe --tu path: ${v}`);
        }
        args.tus.push(v);
        break;
      }
      case "--config": {
        const v = rest[++i];
        if (!v || v.startsWith("--")) throw new Error("--config requires a value");
        args.configPath = v;
        break;
      }
      case "--max-parallel": {
        const v = rest[++i];
        const n = Number(v);
        if (!Number.isInteger(n) || n < 1) {
          throw new Error("--max-parallel requires an integer >= 1");
        }
        args.maxParallel = n;
        break;
      }
      case "--order": {
        const v = rest[++i];
        if (!v || !["most-remaining", "least-remaining", "smallest", "alphabetical"].includes(v)) {
          throw new Error("--order must be one of: most-remaining, least-remaining, smallest, alphabetical");
        }
        args.order = v as UnitOrder;
        break;
      }
      case "--match-less-than": {
        const v = rest[++i];
        if (!v || v.startsWith("--")) throw new Error("--match-less-than requires a value");
        const n = Number(v);
        if (!Number.isFinite(n) || n < 0 || n > 100) {
          throw new Error("--match-less-than requires a percentage in [0, 100]");
        }
        args.matchLessThan = n;
        break;
      }
      case "--status":
        args.showStatus = true;
        break;
      case "--all":
        args.all = true;
        break;
      case "--greenfield":
        args.greenfield = true;
        break;
      case "--dry-run":
        args.dryRun = true;
        break;
      case "--help":
      case "-h":
        printUsage();
        process.exit(0);
        break;
      default:
        throw new Error(`Unknown argument: ${a}`);
    }
  }

  args.tus = [...new Set(args.tus)];

  if (args.showStatus) {
    // --status is a standalone mode; --tu is optional filter.
    return args;
  }

  if (args.all) {
    if (args.tus.length > 0) {
      throw new Error("--all and --tu are mutually exclusive");
    }
    // TUs will be populated after config is loaded (need region).
    return args;
  }

  if (args.tus.length === 0) {
    printUsage();
    throw new Error("At least one --tu <unit> is required, or use --all / --status");
  }
  return args;
}

function printUsage(): void {
  console.log(`Usage: pi-harness --tu <unit> [--tu <unit>...] [options]
       pi-harness --all [options]
       pi-harness --status [options]

Options:
  --tu <unit>          Translation unit to process (repeatable), e.g. kyoshin/CGame
  --all                Auto-discover all TUs with unmatched targets and process them
  --greenfield         Broad-sweep mode: only session targets with status NOT_STARTED
                       (never worked). Previously-attempted targets (STRUCTURAL /
                       HIGH_MATCH / CODE_MATCH / …) are excluded from batches and
                       TUs whose only remaining work is previously-worked are skipped
  --status             Print per-TU match summary table and exit (no sessions run)
  --order <order>      TU ordering for --all and --status: most-remaining (default),
                       least-remaining, smallest (unmatched .text bytes), alphabetical
  --match-less-than <pct>
                       Keep only TUs with match % strictly below <pct> (0-100);
                       applies to --all discovery and --status preview
  --config <path>      Config file (default: <repoRoot>/pi-harness.json)
  --dry-run            Print batch prompts without running sessions
  --max-parallel <n>   Override maxParallelTUs
  --help               This message`);
}

/** Keep only TUs whose match % (matched/total*100) is strictly below the
 *  threshold. Shared by --all (run selection) and --status (preview) so the
 *  preview table always matches what a run would select. */
function filterMatchLessThan(summaries: UnitSummary[], threshold: number): UnitSummary[] {
  return summaries.filter((s) => s.total > 0 && (s.matched / s.total) * 100 < threshold);
}

/** Print a terminal-wide progress-bar table of per-TU match status. */
function printStatusTable(summaries: UnitSummary[], greenfieldOnly = false): void {
  const unitWidth = Math.max(14, ...summaries.map((s) => s.unit.length)) + 2;
  const numWidth = 8;

  // Header
  const header = `${pad("Unit", unitWidth)} ${padR("Total", numWidth)} ${padR("Matched", numWidth)} ${padR("Remaining", numWidth)}`;
  console.log(header);
  console.log("─".repeat(header.length));

  let grandTotal = 0;
  let grandMatched = 0;
  let grandRemaining = 0;
  let fullyMatched = 0;

  for (const s of summaries) {
    grandTotal += s.total;
    grandMatched += s.matched;
    grandRemaining += s.remaining;
    if (s.remaining === 0) fullyMatched++;

    // Progress bar: use block chars scaled to fit ~30 cols
    const barLen = 30;
    const filled = s.total > 0 ? Math.round((s.matched / s.total) * barLen) : 0;
    const empty = barLen - filled;
    const bar = "█".repeat(filled) + "░".repeat(empty);
    const pct = s.total > 0 ? `${((s.matched / s.total) * 100).toFixed(0)}%` : "  -";

    console.log(
      `${pad(s.unit, unitWidth)} ${padR(String(s.total), numWidth)} ${padR(String(s.matched), numWidth)} ${padR(String(s.remaining), numWidth)}  ${bar} ${pct}`,
    );
  }

  console.log("─".repeat(header.length));
  console.log(
    `${pad(`${summaries.length} TUs`, unitWidth)} ${padR(String(grandTotal), numWidth)} ${padR(String(grandMatched), numWidth)} ${padR(String(grandRemaining), numWidth)}`,
  );
  console.log(
    `\n${greenfieldOnly ? "No greenfield work left" : "Fully matched"}: ${fullyMatched}/${summaries.length} TUs  |  ` +
    `Functions: ${grandMatched}/${grandTotal} (${((grandMatched / grandTotal) * 100).toFixed(1)}%)`,
  );
}

function pad(s: string, w: number): string {
  if (s.length >= w) return s;
  return s + " ".repeat(w - s.length);
}

function padR(s: string, w: number): string {
  if (s.length >= w) return s;
  return " ".repeat(w - s.length) + s;
}

async function main(): Promise<void> {
  const args = parseArgs(process.argv);

  const repoRoot = findRepoRoot(cwd());
  process.stderr.write(`[pi-harness] Repo root: ${repoRoot}\n`);
  chdir(repoRoot);

  // Put the repo venv on PATH: agent bash sessions inherit it, so even a
  // plain `python3` resolves to a Python that can run the coop tooling
  // (system python3 is 3.9 and cannot).
  const venvBin = join(repoRoot, ".venv", "bin");
  if (existsSync(venvBin)) {
    process.env.PATH = `${venvBin}:${process.env.PATH ?? ""}`;
  }

  const configPath = args.configPath ? resolve(repoRoot, args.configPath) : undefined;
  const config: HarnessConfig = loadConfig(repoRoot, configPath);

  const effectiveConfig: HarnessConfig = args.maxParallel !== undefined
    ? { ...config, maxParallelTUs: args.maxParallel }
    : config;
  // CLI --greenfield overrides the config key (like --max-parallel).
  if (args.greenfield) {
    effectiveConfig.greenfieldOnly = true;
  }

  process.stderr.write("[pi-harness] Effective config:\n");
  process.stderr.write(`  matchModel:     ${effectiveConfig.matchModel.provider}/${effectiveConfig.matchModel.model} (thinking: ${effectiveConfig.matchModel.thinkingLevel})\n`);
  process.stderr.write(`  singletonModel: ${effectiveConfig.singletonModel.provider}/${effectiveConfig.singletonModel.model} (thinking: ${effectiveConfig.singletonModel.thinkingLevel})\n`);
  process.stderr.write(`  rebatchModel:   ${effectiveConfig.rebatchModel.provider}/${effectiveConfig.rebatchModel.model} (thinking: ${effectiveConfig.rebatchModel.thinkingLevel})\n`);
  process.stderr.write(`  cleanupModel:   ${effectiveConfig.cleanupModel.provider}/${effectiveConfig.cleanupModel.model} (thinking: ${effectiveConfig.cleanupModel.thinkingLevel})\n`);
  process.stderr.write(`  batchSize:      ${effectiveConfig.batchSize}\n`);
  process.stderr.write(`  maxParallel:    ${effectiveConfig.maxParallelTUs}\n`);
  process.stderr.write(`  maxRetries:     ${effectiveConfig.maxBatchRetries}\n`);
  process.stderr.write(`  singletonEnabled: ${effectiveConfig.singletonEnabled}\n`);
  process.stderr.write(`  rebatchEnabled: ${effectiveConfig.rebatchEnabled}\n`);
  process.stderr.write(`  maxRebatchAttempts: ${effectiveConfig.maxRebatchAttempts === 0 ? `off` : effectiveConfig.maxRebatchAttempts}\n`);
  process.stderr.write(`  maxTokens:      ${effectiveConfig.maxTokens === 0 ? "unlimited (model default)" : effectiveConfig.maxTokens}\n`);
  process.stderr.write(`  singletonMinSize: ${effectiveConfig.singletonMinSize === 0 ? "off (all use singletons)" : effectiveConfig.singletonMinSize + " bytes"}\n`);
  process.stderr.write(`  timeoutRetries:   ${effectiveConfig.timeoutRetries} (in-session continuation after wall-clock timeout, 0=off)\n`);
  process.stderr.write(`  rejectionRetries: ${effectiveConfig.rejectionRetries} (re-prompts when the model finished but code failed)\n`);
  process.stderr.write(`  rpmLimit:         ${effectiveConfig.rpmLimit === 0 ? "off (SDK auto-retry ON)" : `${effectiveConfig.rpmLimit} req/min (pacer active, SDK auto-retry OFF)`}\n`);
  process.stderr.write(`  greenfieldOnly:   ${effectiveConfig.greenfieldOnly}\n`);
  process.stderr.write(`  pythonBin:      ${effectiveConfig.pythonBin}\n`);
  process.stderr.write(`  dryRun:         ${args.dryRun}\n`);

  // ── --status mode: print table and exit ─────────────────────────
  if (args.showStatus) {
    let summaries = loadAllUnitSummaries(repoRoot, effectiveConfig.region, args.order, effectiveConfig.greenfieldOnly);
    if (args.matchLessThan !== undefined) {
      const before = summaries.length;
      summaries = filterMatchLessThan(summaries, args.matchLessThan);
      const excluded = before - summaries.length;
      if (excluded > 0) {
        process.stderr.write(
          `[pi-harness] --match-less-than ${args.matchLessThan}: excluding ${excluded} TU(s) with match % >= ${args.matchLessThan}\n`,
        );
      }
    }
    if (args.tus.length > 0) {
      const filter = new Set(args.tus);
      summaries = summaries.filter((s) => filter.has(s.unit));
      if (summaries.length === 0) {
        console.log(`No matching TUs found for: ${args.tus.join(", ")}`);
        return;
      }
    }
    printStatusTable(summaries, effectiveConfig.greenfieldOnly);
    return;
  }

  // ── --all mode: discover TUs from targets.json ──────────────────
  let tus = args.tus;
  if (args.all) {
    let summaries = loadAllUnitSummaries(repoRoot, effectiveConfig.region, args.order, effectiveConfig.greenfieldOnly);
    if (args.matchLessThan !== undefined) {
      const before = summaries.length;
      summaries = filterMatchLessThan(summaries, args.matchLessThan);
      const excluded = before - summaries.length;
      if (excluded > 0) {
        process.stderr.write(
          `[pi-harness] --match-less-than ${args.matchLessThan}: excluding ${excluded} TU(s) with match % >= ${args.matchLessThan}\n`,
        );
      }
    }
    // Skip fully-matched TUs AND TUs whose remaining work is all exhausted
    // (they'd be added to the pool only to instantly return 0 unmatched,
    // starving the ConcurrencyPool of real work and collapsing parallelism
    // — the run33 v5 stall where only 4/10 slots ran sessions).
    let selected = summaries.filter((s) => s.remaining > 0);
    if (!effectiveConfig.retryExhausted) {
      const before = selected.length;
      selected = selected.filter((s) =>
        unitHasActionableWork(
          repoRoot, effectiveConfig.region, s.unit,
          effectiveConfig.ledgerPath, effectiveConfig.exhaustionThreshold,
          effectiveConfig.retryExhausted, effectiveConfig.maxAttemptsPerTarget,
          effectiveConfig.greenfieldOnly,
        ),
      );
      if (selected.length < before) {
        process.stderr.write(
          `[pi-harness] --all: excluding ${before - selected.length} TU(s) with only exhausted targets\n`,
        );
      }
    }
    tus = selected.map((s) => s.unit);
    process.stderr.write(
      `\n[pi-harness] --all: discovered ${tus.length} TU(s) with unmatched targets ` +
      `(order: ${args.order})${effectiveConfig.retryExhausted ? " (retryExhausted)" : ""}${effectiveConfig.greenfieldOnly ? " (greenfieldOnly)" : ""}\n`,
    );
    if (tus.length === 0) {
      console.log("No TUs match the current filters (--all) — nothing to do.");
      return;
    }
  }

  process.stderr.write("\n[pi-harness] Target summary:\n");
  for (const unit of tus) {
    const targets = loadUnmatchedTargets(repoRoot, effectiveConfig.region, unit, {
      ledgerPath: effectiveConfig.ledgerPath,
      retryExhausted: effectiveConfig.retryExhausted,
      exhaustionThreshold: effectiveConfig.exhaustionThreshold,
      greenfieldOnly: effectiveConfig.greenfieldOnly,
    });
    process.stderr.write(`  ${unit}: ${targets.length} unmatched\n`);
  }

  await runTus(repoRoot, tus, effectiveConfig, { dryRun: args.dryRun });

  process.stderr.write("\n[pi-harness] Done.\n");
}

// Catch unhandled promise rejections that would otherwise crash silently.
process.on("unhandledRejection", (reason, promise) => {
  const msg = reason instanceof Error ? reason.message : String(reason);
  console.error(`[pi-harness] Unhandled rejection: ${msg}`);
  console.error(reason);
});

process.on("uncaughtException", (err) => {
  console.error(`[pi-harness] Uncaught exception: ${err.message}`);
  console.error(err.stack);
  process.exit(1);
});

main().catch((err) => {
  const msg = err instanceof Error ? err.message : String(err);
  console.error(`[pi-harness] Fatal error: ${msg}`);
  process.exit(1);
});

#!/usr/bin/env node
/**
 * CLI entry point for the pi-harness.
 *
 * ```bash
 * npm --prefix tools/pi_harness run pi-harness -- \
 *   --tu kyoshin/CGame [--tu other/Unit] [--dry-run] [--max-parallel 2] [--config path]
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
import { loadUnmatchedTargets, loadAllUnitSummaries, type UnitOrder, type UnitSummary } from "./targets.js";
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
}

function parseArgs(argv: string[]): Args {
  const args: Args = { tus: [], dryRun: false, showStatus: false, all: false, order: "most-remaining" };
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
        if (!v || !["most-remaining", "least-remaining", "alphabetical"].includes(v)) {
          throw new Error("--order must be one of: most-remaining, least-remaining, alphabetical");
        }
        args.order = v as UnitOrder;
        break;
      }
      case "--status":
        args.showStatus = true;
        break;
      case "--all":
        args.all = true;
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
  --status             Print per-TU match summary table and exit (no sessions run)
  --order <order>      TU ordering for --all and --status: most-remaining (default),
                       least-remaining, alphabetical
  --config <path>      Config file (default: <repoRoot>/pi-harness.json)
  --dry-run            Print batch prompts without running sessions
  --max-parallel <n>   Override maxParallelTUs
  --help               This message`);
}

/** Print a terminal-wide progress-bar table of per-TU match status. */
function printStatusTable(summaries: UnitSummary[]): void {
  const unitWidth = Math.max(14, ...summaries.map((s) => s.unit.length)) + 2;
  const numWidth = 8;

  // Header
  const header = `${pad("Unit", unitWidth)} ${padR("Total", numWidth)} ${padR("Matched", numWidth)} ${padR("Remaining", numWidth)}`;
  console.log(header);
  console.log("─".repeat(header.length));

  let grandTotal = 0;
  let grandMatched = 0;
  let fullyMatched = 0;

  for (const s of summaries) {
    grandTotal += s.total;
    grandMatched += s.matched;
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
    `${pad(`${summaries.length} TUs`, unitWidth)} ${padR(String(grandTotal), numWidth)} ${padR(String(grandMatched), numWidth)} ${padR(String(grandTotal - grandMatched), numWidth)}`,
  );
  console.log(
    `\nFully matched: ${fullyMatched}/${summaries.length} TUs  |  ` +
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

  process.stderr.write("[pi-harness] Effective config:\n");
  process.stderr.write(`  matchModel:     ${effectiveConfig.matchModel.provider}/${effectiveConfig.matchModel.model} (thinking: ${effectiveConfig.matchModel.thinkingLevel})\n`);
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
  process.stderr.write(`  pythonBin:      ${effectiveConfig.pythonBin}\n`);
  process.stderr.write(`  dryRun:         ${args.dryRun}\n`);

  // ── --status mode: print table and exit ─────────────────────────
  if (args.showStatus) {
    let summaries = loadAllUnitSummaries(repoRoot, effectiveConfig.region, args.order);
    if (args.tus.length > 0) {
      const filter = new Set(args.tus);
      summaries = summaries.filter((s) => filter.has(s.unit));
      if (summaries.length === 0) {
        console.log(`No matching TUs found for: ${args.tus.join(", ")}`);
        return;
      }
    }
    printStatusTable(summaries);
    return;
  }

  // ── --all mode: discover TUs from targets.json ──────────────────
  let tus = args.tus;
  if (args.all) {
    const summaries = loadAllUnitSummaries(repoRoot, effectiveConfig.region, args.order);
    tus = summaries.filter((s) => s.remaining > 0).map((s) => s.unit);
    process.stderr.write(
      `\n[pi-harness] --all: discovered ${tus.length} TU(s) with unmatched targets ` +
      `(order: ${args.order})\n`,
    );
    if (tus.length === 0) {
      console.log("All TUs are fully matched — nothing to do.");
      return;
    }
  }

  process.stderr.write("\n[pi-harness] Target summary:\n");
  for (const unit of tus) {
    const targets = loadUnmatchedTargets(repoRoot, effectiveConfig.region, unit);
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

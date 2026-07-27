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
import { loadUnmatchedTargets } from "./targets.js";
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
}

function parseArgs(argv: string[]): Args {
  const args: Args = { tus: [], dryRun: false };
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
  if (args.tus.length === 0) {
    printUsage();
    throw new Error("At least one --tu <unit> is required");
  }
  return args;
}

function printUsage(): void {
  console.log(`Usage: pi-harness --tu <unit> [--tu <unit>...] [options]

Options:
  --tu <unit>          Translation unit to process (repeatable), e.g. kyoshin/CGame
  --config <path>      Config file (default: <repoRoot>/pi-harness.json)
  --dry-run            Print batch prompts without running sessions
  --max-parallel <n>   Override maxParallelTUs
  --help               This message`);
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
  process.stderr.write(`  singletonRetry: ${effectiveConfig.singletonRetry}\n`);
  process.stderr.write(`  maxTokens:      ${effectiveConfig.maxTokens === 0 ? "unlimited (model default)" : effectiveConfig.maxTokens}\n`);
  process.stderr.write(`  singletonMinSize: ${effectiveConfig.singletonMinSize === 0 ? "off (all use singletons)" : effectiveConfig.singletonMinSize + " bytes"}\n`);
  process.stderr.write(`  maxRePrompts:   ${effectiveConfig.maxRePrompts} (in-session continuation, 0=off)\n`);
  process.stderr.write(`  maxStuckRePrompts: ${effectiveConfig.maxStuckRePrompts} (when model completed but code fails)\n`);
  process.stderr.write(`  pythonBin:      ${effectiveConfig.pythonBin}\n`);
  process.stderr.write(`  dryRun:         ${args.dryRun}\n`);

  process.stderr.write("\n[pi-harness] Target summary:\n");
  for (const unit of args.tus) {
    const targets = loadUnmatchedTargets(repoRoot, effectiveConfig.region, unit);
    process.stderr.write(`  ${unit}: ${targets.length} unmatched\n`);
  }

  await runTus(repoRoot, args.tus, effectiveConfig, { dryRun: args.dryRun });

  process.stderr.write("\n[pi-harness] Done.\n");
}

main().catch((err) => {
  const msg = err instanceof Error ? err.message : String(err);
  console.error(`[pi-harness] Fatal error: ${msg}`);
  process.exit(1);
});

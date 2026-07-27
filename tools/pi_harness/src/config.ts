// ---------------------------------------------------------------------------
// Config loading: pi-harness.json at the repo root, merged over defaults.
// ---------------------------------------------------------------------------

import { readFileSync, existsSync } from "node:fs";
import { join, resolve, isAbsolute } from "node:path";
import type { HarnessConfig, ModelSpec, ThinkingLevel } from "./types.js";

const THINKING_LEVELS: readonly ThinkingLevel[] = [
  "off", "minimal", "low", "medium", "high", "xhigh",
];

const DEFAULT_MODEL: ModelSpec = {
  provider: "openai-codex",
  model: "gpt-5.3-codex",
  thinkingLevel: "high",
};

function defaultConfig(): HarnessConfig {
  return {
    matchModel: { ...DEFAULT_MODEL },
    cleanupModel: { ...DEFAULT_MODEL, thinkingLevel: "medium" },
    batchSize: 5,
    maxParallelTUs: 2,
    maxBatchRetries: 2,
    singletonRetry: true,
    maxBriefChars: 80_000,
    maxBatchMinutes: 60,
    region: "us",
    sessionDir: "build/pi-harness/sessions",
    ledgerPath: "build/pi-harness/ledger.jsonl",
    pythonBin: "", // resolved below
  };
}

function validateModel(spec: ModelSpec, name: string): void {
  if (typeof spec.provider !== "string" || !spec.provider) {
    throw new Error(`config.${name}.provider must be a non-empty string`);
  }
  if (typeof spec.model !== "string" || !spec.model) {
    throw new Error(`config.${name}.model must be a non-empty string`);
  }
  if (!THINKING_LEVELS.includes(spec.thinkingLevel)) {
    throw new Error(
      `config.${name}.thinkingLevel must be one of ${THINKING_LEVELS.join(", ")}`,
    );
  }
}

/**
 * Load config from `configPath` or `<repoRoot>/pi-harness.json`, merged over
 * defaults. A missing file yields defaults.
 */
export function loadConfig(repoRoot: string, configPath?: string): HarnessConfig {
  const path = configPath ?? join(repoRoot, "pi-harness.json");
  const config = defaultConfig();

  if (existsSync(path)) {
    let raw: Record<string, unknown>;
    try {
      raw = JSON.parse(readFileSync(path, "utf-8")) as Record<string, unknown>;
    } catch (err) {
      throw new Error(
        `Failed to parse ${path}: ${err instanceof Error ? err.message : String(err)}`,
      );
    }

    for (const [key, value] of Object.entries(raw)) {
      if (value === undefined || value === null) continue;
      if (key === "matchModel" || key === "cleanupModel") {
        if (typeof value !== "object" || value === null || Array.isArray(value)) {
          throw new Error(`config.${key} must be an object`);
        }
        Object.assign(config[key], value);
      } else if (key in config) {
        (config as unknown as Record<string, unknown>)[key] = value;
      }
    }
  }

  if (typeof config.pythonBin !== "string") {
    throw new Error("config.pythonBin must be a string");
  }

  // Empty pythonBin means auto-detect; an explicit "python3" is respected
  // as a literal command rather than being treated as a sentinel.
  if (config.pythonBin === "") {
    const venvPython = join(repoRoot, ".venv", "bin", "python");
    config.pythonBin = existsSync(venvPython) ? venvPython : "python3";
  } else if (isAbsolute(config.pythonBin) || config.pythonBin.includes("/") || config.pythonBin.includes("\\")) {
    config.pythonBin = isAbsolute(config.pythonBin)
      ? config.pythonBin
      : resolve(repoRoot, config.pythonBin);
  }

  if (typeof config.singletonRetry !== "boolean") {
    throw new Error("config.singletonRetry must be a boolean");
  }
  if (typeof config.pythonBin !== "string" || !config.pythonBin) {
    throw new Error("config.pythonBin must be a non-empty string");
  }
  if (typeof config.region !== "string" || !config.region) {
    throw new Error("config.region must be a non-empty string");
  }
  if (typeof config.sessionDir !== "string" || typeof config.ledgerPath !== "string") {
    throw new Error("config.sessionDir and config.ledgerPath must be strings");
  }
  if (!Number.isInteger(config.batchSize) || config.batchSize < 1) {
    throw new Error("config.batchSize must be an integer >= 1");
  }
  if (!Number.isInteger(config.maxParallelTUs) || config.maxParallelTUs < 1) {
    throw new Error("config.maxParallelTUs must be an integer >= 1");
  }
  if (!Number.isInteger(config.maxBatchRetries) || config.maxBatchRetries < 1) {
    throw new Error("config.maxBatchRetries must be an integer >= 1");
  }
  if (!(config.maxBriefChars >= 1000)) {
    throw new Error("config.maxBriefChars must be >= 1000");
  }
  if (!(config.maxBatchMinutes > 0)) {
    throw new Error("config.maxBatchMinutes must be > 0");
  }
  validateModel(config.matchModel, "matchModel");
  validateModel(config.cleanupModel, "cleanupModel");

  return config;
}

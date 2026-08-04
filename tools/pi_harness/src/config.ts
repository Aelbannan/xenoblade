// ---------------------------------------------------------------------------
// Config loading: pi-harness.json at the repo root, merged over defaults.
// ---------------------------------------------------------------------------

import { readFileSync, existsSync } from "node:fs";
import { join, resolve, isAbsolute } from "node:path";
import type { HarnessConfig, ModelSpec, SelectionMode, ThinkingLevel, TriageMode } from "./types.js";

const THINKING_LEVELS: readonly ThinkingLevel[] = [
  "off", "minimal", "low", "medium", "high", "xhigh",
];

const SELECTION_MODES: readonly SelectionMode[] = [
  "claim-order", "similarity", "random",
];

const TRIAGE_MODES: readonly TriageMode[] = [
  "off", "route",
];

const DEFAULT_MODEL: ModelSpec = {
  provider: "openai-codex",
  model: "gpt-5.3-codex",
  thinkingLevel: "high",
};

function defaultConfig(): HarnessConfig {
  return {
    matchModel: { ...DEFAULT_MODEL },
    singletonModel: { ...DEFAULT_MODEL },
    rebatchModel: { ...DEFAULT_MODEL },
    cleanupModel: { ...DEFAULT_MODEL, thinkingLevel: "medium" },
    batchSize: 5,
    maxParallelTUs: 2,
    selection: "claim-order", // call-graph wave stays the default
    triage: "off", // no-SMT pre-batch routing is opt-in; "off" == today's behavior
    maxBatchRetries: 2,
    singletonEnabled: true,
    rebatchEnabled: true,
    maxRebatchAttempts: 0, // 0 = no rebatch attempts
    maxTokens: 0,
    singletonMinSize: 0,
    maxBriefChars: 80_000,
    briefTargetChars: 12_000,
    maxBatchMinutes: 60,
    timeoutRetries: 3,
    rejectionRetries: 1,
    tuFinalAttempts: 2,
    tuFinalTimeoutMinutes: 0, // 0 = derive (maxBatchMinutes * 2)
    maxAttemptsPerTarget: 4,
    staleRoundThreshold: 2,
    retryExhausted: false,
    bankOnlyOnBetter: true,
    nearmissDir: "build/pi-harness/nearmiss",
    knownWallsPath: "docs/KNOWN_WALLS.md",
    costModel: { inputPerM: 0, outputPerM: 0, cacheReadPerM: 0, cacheWritePerM: 0 },
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

    // Backwards compat: map deprecated keys.
    if ("singletonRetry" in raw) {
      if (!("singletonEnabled" in raw)) {
        process.stderr.write("[pi-harness] config: 'singletonRetry' is deprecated, use 'singletonEnabled'\n");
        raw.singletonEnabled = raw.singletonRetry;
      } else {
        process.stderr.write("[pi-harness] config: 'singletonRetry' ignored (both old and new keys present)\n");
      }
    }

    for (const [key, value] of Object.entries(raw)) {
      if (value === undefined || value === null) continue;
      if (key === "matchModel" || key === "cleanupModel" || key === "singletonModel" || key === "rebatchModel") {
        if (typeof value !== "object" || value === null || Array.isArray(value)) {
          throw new Error(`config.${key} must be an object`);
        }
        Object.assign(config[key], value);
      } else if (key in config) {
        (config as unknown as Record<string, unknown>)[key] = value;
      }
    }

    // Phase models inherit from matchModel when not explicitly set. Because
    // partial specs merge over the DEFAULT_MODEL defaults, a phase spec that
    // left `model`/`thinkingLevel` unset still carries DEFAULT_MODEL's values
    // — those match the inherited default sentinel and get replaced.
    for (const phase of ["singletonModel", "rebatchModel"] as const) {
      const spec = config[phase];
      if (spec.model === DEFAULT_MODEL.model && spec.provider === DEFAULT_MODEL.provider) {
        spec.model = config.matchModel.model;
        spec.provider = config.matchModel.provider;
      }
      if (spec.thinkingLevel === DEFAULT_MODEL.thinkingLevel) {
        spec.thinkingLevel = config.matchModel.thinkingLevel;
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

  if (typeof config.singletonEnabled !== "boolean") {
    throw new Error("config.singletonEnabled must be a boolean");
  }
  if (typeof config.rebatchEnabled !== "boolean") {
    throw new Error("config.rebatchEnabled must be a boolean");
  }
  if (!Number.isInteger(config.maxRebatchAttempts) || config.maxRebatchAttempts < 0) {
    throw new Error("config.maxRebatchAttempts must be an integer >= 0");
  }
  if (typeof config.maxTokens !== "number" || config.maxTokens < 0 || !Number.isInteger(config.maxTokens)) {
    throw new Error("config.maxTokens must be an integer >= 0");
  }
  if (typeof config.singletonMinSize !== "number" || config.singletonMinSize < 0 || !Number.isInteger(config.singletonMinSize)) {
    throw new Error("config.singletonMinSize must be an integer >= 0");
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
  if (!SELECTION_MODES.includes(config.selection)) {
    throw new Error(`config.selection must be one of ${SELECTION_MODES.join(", ")}`);
  }
  if (!TRIAGE_MODES.includes(config.triage)) {
    throw new Error(`config.triage must be one of ${TRIAGE_MODES.join(", ")}`);
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
  if (!Number.isInteger(config.briefTargetChars) || config.briefTargetChars < 1) {
    throw new Error("config.briefTargetChars must be an integer > 0");
  }
  if (!(config.maxBatchMinutes > 0)) {
    throw new Error("config.maxBatchMinutes must be > 0");
  }
  if (typeof config.timeoutRetries !== "number" || config.timeoutRetries < 0 || !Number.isInteger(config.timeoutRetries)) {
    throw new Error("config.timeoutRetries must be an integer >= 0");
  }
  if (typeof config.rejectionRetries !== "number" || config.rejectionRetries < 0 || !Number.isInteger(config.rejectionRetries)) {
    throw new Error("config.rejectionRetries must be an integer >= 0");
  }
  if (!Number.isInteger(config.tuFinalAttempts) || config.tuFinalAttempts < 1) {
    throw new Error("config.tuFinalAttempts must be an integer >= 1");
  }
  if (!Number.isInteger(config.tuFinalTimeoutMinutes) || config.tuFinalTimeoutMinutes < 0) {
    throw new Error("config.tuFinalTimeoutMinutes must be an integer >= 0");
  }
  if (!Number.isInteger(config.maxAttemptsPerTarget) || config.maxAttemptsPerTarget < 1) {
    throw new Error("config.maxAttemptsPerTarget must be an integer >= 1");
  }
  if (!Number.isInteger(config.staleRoundThreshold) || config.staleRoundThreshold < 1) {
    throw new Error("config.staleRoundThreshold must be an integer >= 1");
  }
  if (typeof config.retryExhausted !== "boolean") {
    throw new Error("config.retryExhausted must be a boolean");
  }
  if (typeof config.bankOnlyOnBetter !== "boolean") {
    throw new Error("config.bankOnlyOnBetter must be a boolean");
  }
  if (typeof config.nearmissDir !== "string" || !config.nearmissDir) {
    throw new Error("config.nearmissDir must be a non-empty string");
  }
  if (typeof config.knownWallsPath !== "string") {
    throw new Error("config.knownWallsPath must be a string");
  }
  for (const k of ["inputPerM", "outputPerM", "cacheReadPerM", "cacheWritePerM"] as const) {
    const v = config.costModel[k];
    if (typeof v !== "number" || v < 0 || !Number.isFinite(v)) {
      throw new Error(`config.costModel.${k} must be a number >= 0`);
    }
  }
  validateModel(config.matchModel, "matchModel");
  validateModel(config.singletonModel, "singletonModel");
  validateModel(config.rebatchModel, "rebatchModel");
  validateModel(config.cleanupModel, "cleanupModel");

  // Cross-field warnings (non-fatal).
  if (config.rebatchEnabled && config.singletonMinSize === 0) {
    process.stderr.write("[pi-harness] config: rebatchEnabled=true but singletonMinSize=0 — no target qualifies as small, rebatch will never run\n");
  }
  if (!config.rebatchEnabled && config.maxRebatchAttempts > 0) {
    process.stderr.write("[pi-harness] config: rebatchEnabled=false but maxRebatchAttempts>0 — budget will be ignored\n");
  }
  if (!config.singletonEnabled && !config.rebatchEnabled) {
    process.stderr.write("[pi-harness] config: both singletonEnabled and rebatchEnabled are false — all failed targets will be skipped\n");
  }

  return config;
}

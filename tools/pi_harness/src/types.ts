// ---------------------------------------------------------------------------
// Shared types for the pi-harness.
// ---------------------------------------------------------------------------

export type ThinkingLevel = "off" | "minimal" | "low" | "medium" | "high" | "xhigh";

export interface ModelSpec {
  provider: string;
  model: string;
  thinkingLevel: ThinkingLevel;
}

export interface HarnessConfig {
  matchModel: ModelSpec;
  cleanupModel: ModelSpec;
  batchSize: number;
  maxParallelTUs: number;
  maxBatchRetries: number;
  singletonEnabled: boolean;
  rebatchEnabled: boolean;
  /** Per-TU budget for total rebatch sessions across all small-target
   *  groups. Once exhausted, remaining targets route to singletons
   *  (or get skipped). 0 = no rebatch attempts. */
  maxRebatchAttempts: number;
  /** Max output tokens per session (0 = model default, no override). */
  maxTokens: number;
  /** Targets with retail binary size smaller than this (bytes) go to
   *  rebatch instead of singleton retries. 0 = all use singletons. */
  singletonMinSize: number;
  maxBriefChars: number;
  maxBatchMinutes: number;
  region: string;
  sessionDir: string;
  ledgerPath: string;
  /** Python interpreter for coop tooling; auto-detected from .venv. */
  pythonBin: string;
  /** Total in-session re-prompts on compile/lint failure before falling
   *  back to a fresh session. 0 = single-prompt, no continuation. */
  maxTimeoutRePrompts: number;
  /** In-session re-prompts when the model completed but code still
   *  fails (not a timeout). Lower cap avoids entrenchment on dead ends. */
  maxNoMatchRePrompts: number;
}

export interface Target {
  id: string;
  symbol: string;
  function: string;
  unit: string;
  source: string;
  status: string;
  kind?: string;
  size?: number;
  callgraph_source?: string;
}

export interface TargetBrief {
  targetId: string;
  symbol: string;
  demangled: string;
  retailAsm: string;
}

export interface BatchResult {
  targetId: string;
  status: string;
  accepted: boolean;
}

export interface LedgerEntry {
  ts: string;
  event: string;
  tu: string;
  detail: Record<string, unknown>;
}

export interface SessionUsage {
  input: number;
  output: number;
  cacheRead: number;
  cacheWrite: number;
}

export interface VerifyResult {
  action: "accept" | "re-prompt" | "fail";
  /** Required when action is "re-prompt". */
  feedback?: string;
  /** Optional when action is "fail". */
  reason?: string;
}

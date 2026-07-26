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
  singletonRetry: boolean;
  maxBriefChars: number;
  maxBatchMinutes: number;
  region: string;
  sessionDir: string;
  ledgerPath: string;
  /** Python interpreter for coop tooling; auto-detected from .venv. */
  pythonBin: string;
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

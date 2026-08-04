// ---------------------------------------------------------------------------
// Shared types for the pi-harness.
// ---------------------------------------------------------------------------

export type ThinkingLevel = "off" | "minimal" | "low" | "medium" | "high" | "xhigh";

/** Batch-selection mode (Phase 4). Default stays "claim-order" — the
 *  call-graph wave must remain authoritative; "similarity" re-ranks the
 *  call-graph-ready subset by opcode similarity to matched siblings, and
 *  "random" is a plain uniform shuffle of the wave. */
export type SelectionMode = "claim-order" | "similarity" | "random";

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
  selection: SelectionMode;
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
  /** Per-target ASM share cap inside a batch brief (chars): one huge
   *  target can't eat the whole budget; freed headroom is redistributed
   *  to the remaining targets. */
  briefTargetChars: number;
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
  /** Unified per-target session budget across pass1 + singleton + rebatch. */
  maxAttemptsPerTarget: number;
  /** Consecutive verify rounds with no divergence improvement before early-stop. */
  staleRoundThreshold: number;
  /** Re-attempt targets the ledger marked exhausted on a previous run. */
  retryExhausted: boolean;
  /** Only bank a near-miss draft that beats the stored best (composite score). */
  bankOnlyOnBetter: boolean;
  /** Near-miss draft bank (whole-file snapshots + index.jsonl). */
  nearmissDir: string;
  /** Curated known-walls doc (repo-relative, e.g. docs/KNOWN_WALLS.md) included
   *  in the brief. Empty = omit. */
  knownWallsPath: string;
  /** Token price model for $/match reporting (0 = not priced). */
  costModel: CostModel;
}

/** Per-million-token prices for cost accounting (0 = not priced). */
export interface CostModel {
  inputPerM: number;
  outputPerM: number;
  cacheReadPerM: number;
  cacheWritePerM: number;
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
  /** Call-graph fields mirrored from tools/coop/targets.json — used by the
   *  similarity re-ranker to keep selection call-graph-ready. */
  called_functions?: string[];
  unresolved_called_functions?: string[];
  has_indirect_calls?: boolean;
  callgraph_status?: string;
}

export interface TargetBrief {
  targetId: string;
  symbol: string;
  demangled: string;
  retailAsm: string;
  /** Optional banked near-miss draft note (Phase 2): where the previous
   *  best compiling draft lives + its divergence, so the next session
   *  refines from it instead of starting from scratch. */
  draftNote?: string;
  /** Phase 4 sibling pointers: matched same-unit functions with a similar
   *  opcode profile. Pointer only — NO source bodies (files are
   *  multi-function, e.g. CGame.cpp holds hundreds of targets). */
  siblings?: SiblingPointer[];
}

/** A matched sibling the similarity re-ranker flagged for this target. */
export interface SiblingPointer {
  symbol: string;
  unit: string;
  status: string;
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

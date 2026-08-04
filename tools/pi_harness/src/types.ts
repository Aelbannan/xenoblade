// ---------------------------------------------------------------------------
// Shared types for the pi-harness.
// ---------------------------------------------------------------------------

export type ThinkingLevel = "off" | "minimal" | "low" | "medium" | "high" | "xhigh";

/** Batch-selection mode (Phase 4). Default stays "claim-order" — the
 *  call-graph wave must remain authoritative; "similarity" re-ranks the
 *  call-graph-ready subset by opcode similarity to matched siblings, and
 *  "random" is a plain uniform shuffle of the wave. */
export type SelectionMode = "claim-order" | "similarity" | "random";

/** Pre-batch triage routing (no-SMT efficiency). "off" = today's
 *  behavior; "route" = classify every unmatched target with
 *  tools/coop/triage.py once per TU, send `regswap_only` targets to a
 *  witness-only `run.py cycle` (zero LLM rounds; falls back to the batch
 *  if the witness does not certify) and front-load `strict` targets. */
export type TriageMode = "off" | "route";

export interface ModelSpec {
  provider: string;
  model: string;
  thinkingLevel: ThinkingLevel;
}

export interface HarnessConfig {
  matchModel: ModelSpec;
  /** Model for singleton retries (fallback after a batch target failed).
   *  These are the HARD residue — the batch already failed them at
   *  matchModel's thinking, so singletons should reason HARDER (xhigh),
   *  not cheaper: better conversion = fewer retry sessions = less
   *  cacheRead (the dominant cost). Defaults to matchModel. */
  singletonModel: ModelSpec;
  /** Model for the re-batch phase (small failed targets). Defaults to
   *  matchModel. */
  rebatchModel: ModelSpec;
  cleanupModel: ModelSpec;
  batchSize: number;
  maxParallelTUs: number;
  selection: SelectionMode;
  /** Pre-batch triage routing ("off" | "route"); see TriageMode. */
  triage: TriageMode;
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
  /** Total in-session re-prompts when the session hit the wall-clock
   *  timeout (model was still working). 0 = single-prompt, no continuation. */
  timeoutRetries: number;
  /** In-session re-prompts when the model FINISHED but the harness rejected
   *  the result (compile fail, lint fail, or no match). Lower cap avoids
   *  entrenchment on dead ends. */
  rejectionRetries: number;
  /** TU-final phase levers (independent of batch/singleton knobs). */
  tuFinalAttempts: number;
  /** TU-final session timeout in minutes; 0 = derive (maxBatchMinutes * 2). */
  tuFinalTimeoutMinutes: number;
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

/** One triage.py JSONL row (tools/coop/triage.py, read-only classifier). */
export interface TriageRow {
  targetId: string;
  symbol: string;
  size?: number;
  cls: "strict" | "regswap_only" | "structural" | "unknown";
  nearestMatched?: { symbol: string; score: number };
  structuralCount?: number;
}

/** Per-TU triage summary consumed by the routing path (also logged to the
 *  ledger as a `triage` event). */
export interface TriageSummary {
  byClass: { regswap_only: number; strict: number; structural: number; unknown: number };
  routedToWitness: string[];
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

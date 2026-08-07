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
  /** Max singleton sessions per target (1 = one chance per function after
   *  the batch). Independent of maxBatchRetries (batch re-prompt retries). */
  maxSingletonSessions: number;
  /** When false, skip pre-session snapshotUnit + all restoreSnapshot calls
   *  (no rollback on failed sessions; the worktree keeps whatever the last
   *  session left). Default true. */
  snapshotsEnabled: boolean;
  /** When true, commit the unit's source files + configure.py flip to git
   *  after a successful TU-final (all targets matched, unit flipped to
   *  Matching). Stages ONLY the unit's own files (never `git add -A`), so
   *  other agents' / other TUs' dirty state is never swept in. Best-effort:
   *  a commit failure is logged and never fails the run. Default false. */
  commitOnTuFinal: boolean;
  singletonEnabled: boolean;
  rebatchEnabled: boolean;
  /** Per-TU budget for total rebatch sessions across all small-target
   *  groups. The effective budget is max(configured, sessions needed to
   *  cover the small pool once) — every failed small target is included in
   *  exactly one rebatch session. 0 = auto-derive from pool size. Small
   *  targets never route to singletons (rebatch is their only retry). */
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
  /** Hard timeout (ms) for the witness's z3 rewriter calls; 0 = use the
   *  witness's own internal simplify budget/deadline (deterministic node-cap
   *  + memoization + timeout backstop — the recommended default). A nonzero
   *  value overrides the per-call timeout for harness acceptance runs. */
  witnessTimeoutMs: number;
  /** When false, the register-renaming witness is disabled completely: only
   *  byte-identical FULL_MATCH (with reloc-site equality) can be accepted —
   *  EQUIVALENT_MATCH is never produced. The witness/certify session tools
   *  are removed and the prompts tell the model to chase FULL_MATCH only.
   *  Default true. */
  witnessEnabled: boolean;
  region: string;
  sessionDir: string;
  ledgerPath: string;
  /** Python interpreter for coop tooling; auto-detected from .venv. */
  pythonBin: string;
  /** Total in-session re-prompts when the session hit the wall-clock
   *  timeout (model was still working). 0 = single-prompt, no continuation. */
  timeoutRetries: number;
  /** Max SECONDS of model silence (no message_update / tool activity) before
   *  the session is aborted as dead. 0 = auto-derive from the session model's
   *  thinking level (xhigh: 600s, high: 300s, else 120s). High-thinking
   *  models are silent for minutes while reasoning (no streamed tokens) —
   *  the 120s default aborts them mid-think (run31: 40 DEAD SESSION flags
   *  on high-thinking rebatch rounds). Units are SECONDS (a human-facing
   *  timeout), converted to ms internally. Default 0. */
  silenceThresholdSec: number;
  /** How many times to re-run a prompt round that returned EMPTY (no
   *  assistant output at all — the 429-rate-limit-empty case: the SDK retry
   *  gave up after maxRetries and returned nothing, burning the whole 45-min
   *  slot on a verify->re-prompt loop that finds no changes). Each retry
   *  sleeps a short jitter first to let the provider's concurrency burst
   *  pass. 0 = no retry (fail fast). Default 2. */
  emptyRoundRetries: number;
  /** Max random delay (ms) applied BEFORE each prompt round's first request.
   *  All 30 parallel sessions hit round-0 simultaneously -> the provider's
   *  per-user concurrency cap trips (run32: 38x 429 in one burst). A random
   *  0..jitter delay per round start spreads the wave. Default 15000. */
  roundStartJitterMs: number;
  /** Global cross-session request budget (requests per minute) for the model
   *  provider. 0 = disabled (no throttle; SDK auto-retry stays on — current
   *  behavior). When > 0, paces ALL concurrent sessions' HTTP requests to
   *  this rate (wrapping ModelRuntime.streamSimple/simple) AND disables SDK
   *  auto-retry so the harness/limiter own backoff. The nube gateway serves
   *  DeepSeek with a hard ~20 rpm per-deployment cap; 14-20 parallel sessions
   *  saturate it (79% of session wall-clock wasted on 429/empty-retry churn).
   *  Default 0. */
  rpmLimit: number;
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
  /** Independent ledger dead-end records required before a target is treated
   *  as exhausted and excluded from future waves (default 3). A target that
   *  failed a single batch is retried, not blacklisted. */
  exhaustionThreshold: number;
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
  /** Latest objdiff instruction-match % from targets.json (r8 WS-2: near-match
   *  classification for singleton routing — ≥90% targets get priority). */
  instructionMatch?: number;
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

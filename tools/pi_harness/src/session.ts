/**
 * Agent session management using the pi-coding-agent SDK.
 *
 * runAgentSession: resolve model → persistent on-disk session → streaming
 * transcript → prompt with wall-clock timeout → final assistant text +
 * summed token usage. Supports optional multi-prompt continuation where
 * an onVerify callback decides whether to re-prompt the same session with
 * compile/lint feedback.
 *
 * @module session
 */

import { writeFile, mkdir } from "node:fs/promises";
import { join } from "node:path";
import * as undici from "undici";
import {
  createAgentSession,
  ModelRuntime,
  SessionManager,
  SettingsManager,
  type AgentSession,
} from "@earendil-works/pi-coding-agent";
import type { ModelSpec, SessionUsage, VerifyResult } from "./types.js";
import { batchSessionTools, tuFinalSessionTools } from "./session-tools.js";
import { getSharedRateLimiter } from "./rate-limit.js";

/**
 * SettingsManager for throttled runs: disables SDK auto-retry so 429s surface
 * immediately and re-enter the pacer queue (SDK retries would double-count
 * tokens and slip past the throttle). Exported for tests.
 */
export function createThrottledSettingsManager(): SettingsManager {
  const sm = SettingsManager.create(process.cwd());
  sm.setRetryEnabled(false);
  return sm;
}

/** 0 when no pacer is configured, else the number of requests queued on the
 *  cross-session limiter (used by the dead-session heartbeat to avoid aborting
 *  a session parked in the queue). */
function pacerQueueDepth(): number {
  const limiter = getSharedRateLimiter();
  return limiter ? limiter.queueDepth : 0;
}

// Configure HTTP dispatcher with 5-minute body/headers timeout.
// Without this, undici uses Node.js defaults (no timeout), so HTTP
// requests can hang indefinitely if the server stops responding.
// This must be called once before any createAgentSession calls.
const HTTP_TIMEOUT_MS = 300_000; // 5 min
let httpDispatcherConfigured = false;
function ensureHttpDispatcher(): void {
  if (httpDispatcherConfigured) return;
  httpDispatcherConfigured = true;
  try {
    const dispatcher = new undici.EnvHttpProxyAgent({
      allowH2: false,
      bodyTimeout: HTTP_TIMEOUT_MS,
      headersTimeout: HTTP_TIMEOUT_MS,
    });
    undici.setGlobalDispatcher(dispatcher);
  } catch (err) {
    process.stderr.write(
      `[session] WARNING: Failed to configure HTTP dispatcher: ${err instanceof Error ? err.message : String(err)}\n`,
    );
  }
}

export interface SessionRunResult {
  /** Concatenated text from the last assistant message. */
  finalText: string;
  /** Path to the persisted session file, if any. */
  sessionFile?: string;
  /** Whether the final prompt round hit the wall-clock timeout. */
  timedOut: boolean;
  /** Summed token usage across all assistant messages. */
  usage: SessionUsage;
  /** Multi-prompt additions (undefined when multiPrompt not used). */
  outcome?: "accepted" | "failed" | "gave-up";
  rePromptsUsed?: number;
  /** Usage per prompt round (index 0 = initial, 1+ = re-prompts). */
  roundUsages?: SessionUsage[];
  /** The last rejection feedback from onVerify, for carryover to the
   *  next fresh session (set when outcome is "failed" or "gave-up"). */
  lastRejection?: string;
  /** Set when the heartbeat flagged the session as dead (silence) and
   *  aborted it. Previously write-only; surfaced so callers can tell a
   *  dead-session abort apart from a normal completion (review H3). */
  deadSessionReason?: string;
}

export interface MultiPromptOpts {
  /** Re-prompts when the session hit the wall-clock timeout. */
  timeoutRetries: number;
  /** Re-prompts when the model finished but the harness rejected (compile/lint/no-match). */
  rejectionRetries: number;
  /** Called after each prompt round. Receives the latest final text,
   *  whether it timed out, and the re-prompt count so far (0 after
   *  initial prompt). Returns the next action. */
  onVerify: (
    finalText: string,
    timedOut: boolean,
    rePromptCount: number,
  ) => Promise<VerifyResult>;
  /** Overall session deadline in minutes (default: timeoutMinutes * (1 + timeoutRetries)). */
  totalTimeoutMinutes?: number;
}

/** Sum token usage across assistant messages in a session state. */
function sumUsage(messages: readonly unknown[]): SessionUsage {
  const usage: SessionUsage = { input: 0, output: 0, cacheRead: 0, cacheWrite: 0 };
  for (const m of messages) {
    if ((m as { role?: unknown }).role !== "assistant") continue;
    const u = (m as { usage?: Partial<SessionUsage> }).usage;
    if (!u) continue;
    usage.input += u.input ?? 0;
    usage.output += u.output ?? 0;
    usage.cacheRead += u.cacheRead ?? 0;
    usage.cacheWrite += u.cacheWrite ?? 0;
  }
  return usage;
}

/** Compute delta between two cumulative SessionUsage snapshots. */
function usageDelta(before: SessionUsage, after: SessionUsage): SessionUsage {
  return {
    input: after.input - before.input,
    output: after.output - before.output,
    cacheRead: after.cacheRead - before.cacheRead,
    cacheWrite: after.cacheWrite - before.cacheWrite,
  };
}

export async function runAgentSession(opts: {
  repoRoot: string;
  modelRuntime: ModelRuntime;
  spec: ModelSpec;
  prompt: string;
  sessionDir: string;
  label: string;
  timeoutMinutes: number;
  maxTokens?: number;
  multiPrompt?: MultiPromptOpts;
  /** Session kind: batch (default) gets structured tools, NO bash;
   *  tu-final gets the same + bash behind a spawnHook allowlist. */
  kind?: "batch" | "tu-final";
  /** Python interpreter for the structured tools (hexdiff/run.py). */
  python?: string;
  /** Writable-scope files (repo-relative) for this session — the built-in
   *  edit/write tools are replaced by scoped versions restricted to these
   *  (adversarial review H4). Default [] = no writes allowed. */
  writable?: string[];
  /** When false, omit the witness/certify tools (witness path disabled).
   *  Default true. */
  witnessEnabled?: boolean;
  /** Max SECONDS of model silence before the session is aborted as dead.
   *  0 = auto-derive from spec.thinkingLevel (xhigh 600, high 300, else
   *  120). High-thinking models stream nothing while reasoning, so a
   *  low fixed threshold kills them mid-think. */
  silenceThresholdSec?: number;
  /** Re-run a prompt round that returned EMPTY (no assistant output — the
   *  429-rate-limit-empty case) up to this many times, with a jitter sleep
   *  between attempts. 0 = fail fast. Default 2. */
  emptyRoundRetries?: number;
  /** Global cross-session request budget (requests/min). When > 0, SDK
   *  auto-retry is DISABLED (the harness pacer owns backoff — a retry that
   *  429s would merely re-enter the queue and double-count tokens). When 0,
   *  pass no SettingsManager (SDK auto-retry stays ON = current behavior). */
  rpmLimit?: number;
  /** Random 0..N ms delay before each round's first request, spreading the
   *  provider-concurrency burst when many sessions start simultaneously.
   *  Default 15000. */
  roundStartJitterMs?: number;
}): Promise<SessionRunResult> {
  const {
    repoRoot,
    modelRuntime,
    spec,
    prompt,
    sessionDir,
    label,
    timeoutMinutes,
    maxTokens,
    multiPrompt,
    kind = "batch",
    python = "python3",
    writable = [],
    witnessEnabled = true,
    silenceThresholdSec = 0,
    emptyRoundRetries = 2,
    roundStartJitterMs = 15_000,
    rpmLimit = 0,
  } = opts;

  const model = modelRuntime.getModel(spec.provider, spec.model);
  if (!model) {
    const available = await modelRuntime.getAvailable();
    const names = available.map((m) => `${m.provider}/${m.id}`).join(", ");
    throw new Error(
      `Model "${spec.provider}/${spec.model}" not found. ` +
        `Available models: ${names || "(none configured)"}.`,
    );
  }

  // Override maxTokens on the model if explicitly set (> 0).
  if (maxTokens && maxTokens > 0) {
    (model as { maxTokens?: number }).maxTokens = maxTokens;
  }

  const absSessionDir = join(repoRoot, sessionDir);
  await mkdir(absSessionDir, { recursive: true });

  const transcriptPath = join(absSessionDir, `${label}.transcript.md`);
  let transcriptContent = `# Session: ${label}\n\n`;
  await writeFile(transcriptPath, transcriptContent, "utf-8");

  // Ensure HTTP dispatcher has proper timeouts before creating sessions.
  ensureHttpDispatcher();

  // When rpmLimit > 0 the process-wide pacer owns backoff: disable the SDK's
  // auto-retry so a 429 surfaces immediately and re-enters the pacer queue
  // (retrying inside the SDK would double-count tokens and let retries slip
  // past the throttle). When rpmLimit = 0 pass no settingsManager — SDK
  // auto-retry stays ON (current behavior).
  const settingsManager = rpmLimit > 0 ? createThrottledSettingsManager() : undefined;

  const { session } = await createAgentSession({
    model,
    thinkingLevel: spec.thinkingLevel,
    modelRuntime,
    sessionManager: SessionManager.create(repoRoot, sessionDir),
    ...(settingsManager ? { settingsManager } : {}),
    // Tool surface by session kind (see session-tools.ts):
    //  - batch: read/edit/write/grep/find/ls + hexdiff/symbols/targets, NO
    //    bash — SMT/git/ninja/registry writes are structurally impossible.
    //  - tu-final: same + bash behind a spawnHook allowlist (diff/size/
    //    symbols/build_lock/configure/ninja only).
    tools: kind === "batch"
      ? ["read", "edit", "write", "grep", "find", "ls", "hexdiff", "symbols", "targets", "kb", "ctx", ...(witnessEnabled ? ["witness", "certify"] : []), "unit-status"]
      : ["read", "bash", "edit", "write", "grep", "find", "ls", "hexdiff", "symbols", "targets", "kb", "ctx", ...(witnessEnabled ? ["witness", "certify"] : []), "unit-status"],
    customTools: kind === "batch"
      ? batchSessionTools(repoRoot, python, writable, witnessEnabled)
      : tuFinalSessionTools(repoRoot, python, writable, witnessEnabled),
  });

  // Transcript writes are serialised — concurrent fire-and-forget writeFile
  // calls to the same path can truncate each other.
  let transcriptQueue: Promise<void> = Promise.resolve();
  const queueTranscriptWrite = (): void => {
    const content = transcriptContent;
    transcriptQueue = transcriptQueue
      .then(() => writeFile(transcriptPath, content, "utf-8"))
      .catch(() => {});
  };

  // Track session activity for dead-session detection.
  // lastActivityTime is null until the FIRST activity event (message_update /
  // tool start / tool end / retry): a session that has not yet emitted
  // anything is in its thinking phase (high-thinking models stream no tokens
  // while reasoning), NOT dead — the silence clock must not run before the
  // first message update. After first activity, silence beyond the threshold
  // means the model stopped responding mid-turn.
  let lastActivityTime: number | null = null;
  let lastAgentEnd: { timestamp: number; willRetry: boolean } | undefined;
  // True when the CURRENT prompt round produced at least one assistant
  // message with content (text/thinking/toolCall). A round that returns
  // empty (429-rate-limit give-up: SDK retries exhausted -> nothing) has
  // this false — the harness retries the prompt instead of burning the
  // 45-min slot on a verify->re-prompt loop that finds no changes.
  let roundProducedOutput = false;
  // Number of tool executions currently in flight. The heartbeat must never
  // flag a session dead while a tool is running (long tool calls — hexdiff
  // builds, unit-status witness probes — can exceed the silence threshold;
  // review H3).
  let toolInFlight = 0;

  const unsubscribe = session.subscribe((event) => {
    // Track activity on meaningful events.
    if (event.type === "message_update" ||
        event.type === "tool_execution_start" ||
        event.type === "tool_execution_end" ||
        event.type === "auto_retry_start") {
      lastActivityTime = Date.now();    }
    // Empty-round detection: a message_update carrying an assistant message
    // with any content (thinking/text/toolCall) means this round produced
    // output. A 429-give-up round never emits one — the harness can then
    // retry the prompt instead of burning the session on an empty verify.
    if (event.type === "message_update") {
      const msg = (event as { message?: { role?: string; content?: unknown } }).message;
      if (msg?.role === "assistant" && Array.isArray(msg.content) && msg.content.length > 0) {
        roundProducedOutput = true;
      }
    }

    // Log SDK retry events.
    if (event.type === "auto_retry_start") {
      process.stderr.write(
        `[session] ${label}: SDK retry ${event.attempt}/${event.maxAttempts}` +
        ` (delay ${event.delayMs}ms): ${event.errorMessage}\n`,
      );
    }
    if (event.type === "auto_retry_end") {
      process.stderr.write(
        `[session] ${label}: SDK retry ${event.attempt} ${event.success ? "succeeded" : "failed"}` +
        (event.finalError ? `: ${event.finalError}` : "") + `\n`,
      );
    }

    // Track agent_end for diagnostics.
    if (event.type === "agent_end") {
      lastAgentEnd = { timestamp: Date.now(), willRetry: event.willRetry };
    }

    if (event.type === "message_update") {
      const msg = event.assistantMessageEvent;
      if (msg.type === "text_delta" || msg.type === "thinking_delta") {
        transcriptContent += msg.delta;
        queueTranscriptWrite();
      }
    }
    if (event.type === "tool_execution_start") {
      toolInFlight++;
      transcriptContent += `\n\n---\n**🛠 Tool: ${event.toolName}**\n\n`;
      // Include the tool's input parameters (compact JSON) so transcripts
      // are self-contained for debugging (which symbol/unit was passed).
      const args = event.args as Record<string, unknown> | undefined;
      if (args && Object.keys(args).length > 0) {
        try {
          const compact = JSON.stringify(args);
          transcriptContent +=
            compact.length > 800
              ? `*Args: ${compact.slice(0, 800)}… (${compact.length} chars)*\n\n`
              : `*Args: ${compact}*\n\n`;
        } catch { /* args not JSON-serialisable — skip */ }
      }
      queueTranscriptWrite();
    }
    if (event.type === "tool_execution_update") {
      // Streaming partial output — record only SMALL chunks (<2KB) so
      // long streams (build output) don't bloat the transcript AND don't
      // near-duplicate the final result recorded at tool_execution_end.
      // The final result (possibly truncated) is the authoritative capture.
      const pr = event.partialResult as { text?: string; content?: unknown } | undefined;
      const text = typeof pr === "string" ? pr : pr && typeof pr.text === "string" ? pr.text : undefined;
      if (text && text.length > 0 && text.length <= 2000) {
        transcriptContent += `  …${text.slice(0, 2000)}\n`;
        queueTranscriptWrite();
      }
    }
    if (event.type === "tool_execution_end") {
      toolInFlight = Math.max(0, toolInFlight - 1);
      const result = event.result as { content?: Array<{ type?: string; text?: string }> } | string | undefined;
      let outText = "";
      if (typeof result === "string") {
        outText = result;
      } else if (result && Array.isArray(result.content)) {
        outText = result.content
          .map((c) => (typeof c === "string" ? c : c?.text ?? ""))
          .join("\n");
      }
      // Truncate long outputs (hexdiff JSON, build logs) to keep the
      // transcript readable — full detail stays in the tool's own JSON.
      const MAX_TOOL_OUT = 6000;
      let shown =
        outText.length > MAX_TOOL_OUT
          ? outText.slice(0, MAX_TOOL_OUT) + `\n… (${outText.length - MAX_TOOL_OUT} more chars)`
          : outText;
      // Keep the markdown code block intact if the output contains ```.
      shown = shown.replace(/```/g, "` ``");
      transcriptContent += `*Result: ${event.isError ? "❌ error" : "✅ success"}*\n\n\`\`\`text\n${shown}\n\`\`\`\n\n---\n\n`;
      queueTranscriptWrite();
    }
  });

  // Heartbeat monitor: detect dead sessions (no activity for the silence
  // threshold while the session should be working). Uses SDK state to avoid
  // false positives during builds (session.isIdle) and compaction (session.isCompacting).
  //
  // Adversarial review H3: the previous logic aborted on 120s of silence
  // EVEN while a long tool call was in flight (unit-status runs N sequential
  // witness probes; a hexdiff full build can exceed 2min), and the abort
  // reason was write-only — invisible in the result. Now we (a) track tool
  // execution explicitly and never flag a session dead mid-tool, (b) surface
  // the reason in the SessionRunResult, (c) start the silence clock only at
  // the first message update (thinking is silent for high-thinking models),
  // and (d) scale the threshold with the thinking level (or a config
  // override, in SECONDS) instead of a fixed 120s.
  const SILENCE_THRESHOLD_MS =
    silenceThresholdSec > 0
      ? silenceThresholdSec * 1000 // config is in seconds (human-facing)
      : spec.thinkingLevel === "xhigh"
        ? 600_000 // 10 min: xhigh reasoning streams nothing for minutes
        : spec.thinkingLevel === "high"
          ? 300_000 // 5 min: high reasoning is silent up to several minutes
          : 120_000; // low/minimal: streams promptly
  const HEARTBEAT_INTERVAL_MS = 15_000;
  let deadSessionReason: string | undefined;

  // Random 0..roundStartJitterMs delay before each round's first request.
  // All 30 parallel sessions hit round-0 simultaneously -> the provider's
  // per-user concurrency cap trips (run32: 38x 429 in one burst); spreading
  // the wave removes most of it.
  const jitterDelay = (): Promise<void> =>
    roundStartJitterMs > 0
      ? new Promise((r) => setTimeout(r, Math.random() * roundStartJitterMs))
      : Promise.resolve();
  const heartbeat = setInterval(() => {
    if (toolInFlight > 0) return; // a tool is executing — it has its own timeout
    if (lastActivityTime === null) return; // no first message yet — still thinking, not dead
    // Global pacer queue: while ANY request is waiting on the cross-session
    // rate limiter, a session may be parked there (no events fire while
    // queued) and would look dead to this heartbeat. Skip aborting while the
    // queue is non-empty (adversarial review C-3: per-request throttling must
    // not convert 429-stalls into false DEAD SESSION aborts).
    if (pacerQueueDepth() > 0) return;
    const silenceMs = Date.now() - lastActivityTime;
    if (silenceMs > SILENCE_THRESHOLD_MS) {
      // Only flag as dead if session should be active (not idle, not compacting).
      if (!session.isIdle && !session.isCompacting && session.retryAttempt === 0) {
        deadSessionReason = `Silent for ${Math.round(silenceMs / 1000)}s, session not idle`;
        process.stderr.write(`[session] ${label}: DEAD SESSION DETECTED — ${deadSessionReason}\n`);
        session.abort().catch(() => {});
      }
    }
  }, HEARTBEAT_INTERVAL_MS); // check every 15 seconds
  if (heartbeat && typeof heartbeat === "object" && "unref" in heartbeat) {
    heartbeat.unref();
  }

  let disposed = false;
  const dispose = (): void => {
    if (disposed) return;
    disposed = true;
    clearInterval(heartbeat);
    unsubscribe();
    session.dispose();
  };

  try {
    // ── Single-prompt path (no multiPrompt provided) ──
    if (!multiPrompt) {
      process.stderr.write(`[session] ${label}: starting single-prompt (timeout=${timeoutMinutes}min)\n`);
      const result = await (async () => {
        await jitterDelay();
        return runOnePrompt(session, prompt, timeoutMinutes);
      })();
      await result.settle();
      const usage = sumUsage(session.state.messages);
      process.stderr.write(`[session] ${label}: completed single-prompt (timedOut=${result.timedOut})\n`);
      return {
        finalText: result.finalText,
        sessionFile: session.sessionFile,
        timedOut: result.timedOut,
        usage,
        ...(deadSessionReason ? { deadSessionReason } : {}),
      };
    }

    // ── Multi-prompt path ──
    // Even when caps are 0, we enter this path so onVerify runs once.
    process.stderr.write(`[session] ${label}: starting multi-prompt (timeout=${timeoutMinutes}min, timeoutRetries=${multiPrompt.timeoutRetries})\n`);
    const { timeoutRetries, rejectionRetries, onVerify, totalTimeoutMinutes } = multiPrompt;
    const overallDeadline =
      totalTimeoutMinutes && totalTimeoutMinutes > 0
        ? Date.now() + totalTimeoutMinutes * 60 * 1000
        : Infinity;

    const roundUsages: SessionUsage[] = [];
    let rePromptsUsed = 0;
    let finalText = "";
    let timedOut = false;
    let lastRejection: string | undefined;

    // Run one prompt round with empty-round retries: a round that returned
    // NO assistant output (429 give-up: SDK retries exhausted -> empty) is
    // re-run up to emptyRoundRetries times with a jitter sleep between, so
    // the 45-min slot isn't burned on an empty verify->re-prompt loop.
    const runRound = async (roundPrompt: string, roundMinutes: number): Promise<PromptResult> => {
      let result: PromptResult | null = null;
      for (let attempt = 0; attempt <= emptyRoundRetries; attempt++) {
        if (attempt > 0) {
          const waitMs = Math.floor(Math.random() * roundStartJitterMs) + 5_000;
          process.stderr.write(
            `[session] ${label}: empty round (no assistant output) — retry ${attempt}/${emptyRoundRetries} after ${waitMs}ms (429 give-up recovery)\n`,
          );
          await new Promise((r) => setTimeout(r, waitMs));
        } else {
          await jitterDelay();
        }
        roundProducedOutput = false;
        result = await runOnePrompt(session, roundPrompt, roundMinutes);
        await result.settle();
        if (!result.timedOut && !roundProducedOutput && emptyRoundRetries > 0) {
          // Empty + not timed out + retries remain — retry (likely 429 give-up).
          continue;
        }
        break;
      }
      return result!;
    };

    // ── Initial prompt ──
    process.stderr.write(`[session] ${label}: round 0 starting\n`);
    transcriptContent += `\n\n## Round 0 (initial)\n\n`;
    // Record the FULL initial prompt so the transcript is self-contained
    // (what the model was asked, including the brief + harness instructions).
    const promptBlock = prompt.replace(/```/g, "` ``");
    transcriptContent += `### 📤 PROMPT (Round 0 — initial)\n\n\`\`\`markdown\n${promptBlock}\n\`\`\`\n\n---\n\n### 🤖 MODEL RESPONSE\n\n`;
    queueTranscriptWrite();
    const usageBefore = sumUsage(session.state.messages);
    const initialResult = await runRound(prompt, timeoutMinutes);
    // L: the initial prompt must fully settle before onVerify runs (it
    // builds / runs batch-cycle against the worktree — a still-streaming
    // prompt could keep issuing tool calls concurrently).
    await initialResult.settle();
    let usageAfter = sumUsage(session.state.messages);
    roundUsages.push(usageDelta(usageBefore, usageAfter));
    finalText = initialResult.finalText;
    timedOut = initialResult.timedOut;
    process.stderr.write(`[session] ${label}: round 0 completed (timedOut=${timedOut})\n`);

    // ── Verify → re-prompt loop ──
    let lastVerifyResult: VerifyResult | null = null;

    while (true) {
      // onVerify is responsible for checking caps and returning fail
      // when exhausted. It receives timedOut + rePromptCount so it can
      // use the appropriate cap.
      process.stderr.write(`[session] ${label}: verify round ${rePromptsUsed}\n`);
      const verifyResult = await onVerify(finalText, timedOut, rePromptsUsed);
      lastVerifyResult = verifyResult;
      process.stderr.write(`[session] ${label}: verify result: ${verifyResult.action}\n`);

      if (verifyResult.action === "accept") {
        // Flush transcript before returning.
        await transcriptQueue;
        return {
          finalText,
          sessionFile: session.sessionFile,
          timedOut,
          usage: usageAfter,
          outcome: "accepted",
          rePromptsUsed,
          roundUsages,
          ...(deadSessionReason ? { deadSessionReason } : {}),
        };
      }

      if (verifyResult.action === "fail") {
        lastRejection = verifyResult.reason ?? verifyResult.feedback;
        await transcriptQueue;
        return {
          finalText,
          sessionFile: session.sessionFile,
          timedOut,
          usage: usageAfter,
          outcome: "failed",
          rePromptsUsed,
          roundUsages,
          lastRejection,
          ...(deadSessionReason ? { deadSessionReason } : {}),
        };
      }

      // action === "re-prompt" — check budget and deadline
      const effectiveMax = timedOut ? timeoutRetries : rejectionRetries;
      if (rePromptsUsed >= effectiveMax) {
        lastRejection = verifyResult.feedback;
        await transcriptQueue;
        return {
          finalText,
          sessionFile: session.sessionFile,
          timedOut,
          usage: usageAfter,
          outcome: "gave-up",
          rePromptsUsed,
          roundUsages,
          lastRejection,
          ...(deadSessionReason ? { deadSessionReason } : {}),
        };
      }

      const now = Date.now();
      if (now >= overallDeadline) {
        lastRejection = "session deadline exceeded";
        await transcriptQueue;
        return {
          finalText,
          sessionFile: session.sessionFile,
          timedOut: true,
          usage: usageAfter,
          outcome: "gave-up",
          rePromptsUsed,
          roundUsages,
          lastRejection,
          ...(deadSessionReason ? { deadSessionReason } : {}),
        };
      }

      const remainingMs =
        overallDeadline === Infinity ? undefined : Math.max(1, overallDeadline - now);
      const remainingMinutes = remainingMs !== undefined
        ? Math.min(timeoutMinutes, remainingMs / 60_000)
        : timeoutMinutes;

      rePromptsUsed++;
      process.stderr.write(`[session] ${label}: re-prompt ${rePromptsUsed} starting\n`);
      transcriptContent += `\n\n## Round ${rePromptsUsed} (re-prompt)\n\n`;
      // Record the harness's injected feedback (the re-prompt prompt) with a
      // clear delimiter — this is what told the model to continue/fix, and is
      // essential for debugging why a round went nowhere.
      const fbBlock = (verifyResult.feedback ?? "(empty feedback)").replace(/```/g, "` ``");
      transcriptContent += `### 📥 FEEDBACK (Round ${rePromptsUsed} — harness)\n\n\`\`\`markdown\n${fbBlock}\n\`\`\`\n\n---\n\n### 🤖 MODEL RESPONSE\n\n`;
      queueTranscriptWrite();

      const roundUsageBefore = sumUsage(session.state.messages);
      const rePromptResult = await runRound(
        verifyResult.feedback ?? "",
        remainingMinutes,
      );
      // L: never let the NEXT prompt start while the previous one is still
      // settling (a hard-timeout can return while the SDK stream is stuck).
      // Without this, the loop would issue a concurrent session.prompt() —
      // SDK behavior for overlapping prompts is undefined.
      await rePromptResult.settle();
      usageAfter = sumUsage(session.state.messages);
      roundUsages.push(usageDelta(roundUsageBefore, usageAfter));
      finalText = rePromptResult.finalText;
      timedOut = rePromptResult.timedOut;
      process.stderr.write(`[session] ${label}: re-prompt ${rePromptsUsed} completed (timedOut=${timedOut})\n`);
    }
  } catch (err) {
    // Attach partial usage so the caller can ledger it even on failure.
    try {
      if (err && typeof err === "object") {
        (err as { usage?: SessionUsage }).usage = sumUsage(session.state.messages);
      }
    } catch {
      // ignore
    }
    throw err;
  } finally {
    await transcriptQueue;
    dispose();
  }
}

// ─────────────────────────────────────────────────────────────────────
//  Single-prompt runner
// ─────────────────────────────────────────────────────────────────────

interface PromptResult {
  finalText: string;
  timedOut: boolean;
  /** Resolves when the underlying session.prompt() call has fully settled
   *  (after abort or hard-timeout). The multi-prompt loop awaits this before
   *  issuing the NEXT prompt, so we never call session.prompt() concurrently
   *  on the same session (adversarial review L). */
  settle: () => Promise<void>;
}

const POST_ABORT_SETTLE_MS = 30_000;

async function runOnePrompt(
  session: AgentSession,
  prompt: string,
  timeoutMinutes: number,
): Promise<PromptResult> {
  let timedOut = false;
  let timer: ReturnType<typeof setTimeout> | undefined;
  let settled = false;
  const waitSettled = async (): Promise<void> => {
    if (settled) return;
    // Wait for the in-flight prompt to finish settling (resolve or reject),
    // then clear the abort so the next prompt starts clean.
    try {
      await promptPromise.catch(() => {});
    } finally {
      settled = true;
    }
  };

  let promptPromise: Promise<undefined | string>;
  try {
    const timeoutMs = timeoutMinutes * 60 * 1000;
    promptPromise = session
      .prompt(prompt, { expandPromptTemplates: false })
      .then(() => undefined as undefined | string);
    const timeoutPromise = new Promise<undefined | string>((resolve) => {
      if (timeoutMs <= 0) return;
      timer = setTimeout(() => {
        timedOut = true;
        session.abort().catch(() => {});
        resolve("timeout");
      }, timeoutMs);
      if (timer && typeof timer === "object" && "unref" in timer) timer.unref();
    });

    const result = await Promise.race([promptPromise, timeoutPromise]);
    if (result === "timeout") {
      // The HTTP stream may be stuck (server stopped responding).
      // AbortSignal cannot interrupt a blocked stream read, so we
      // need a hard deadline to prevent hanging indefinitely.
      // After POST_ABORT_SETTLE_MS, we give up waiting and continue.
      const hardDeadline = new Promise<undefined | string>((resolve) => {
        const t = setTimeout(() => resolve("hard-timeout"), POST_ABORT_SETTLE_MS);
        if (t && typeof t === "object" && "unref" in t) t.unref();
      });
      const settleResult = await Promise.race([promptPromise, hardDeadline]);
      if (settleResult === "hard-timeout") {
        process.stderr.write(`[session] Hard timeout after ${POST_ABORT_SETTLE_MS}ms — HTTP stream likely stuck\n`);
      }
      try {
        // Try to abort again in case the stream became unstuck
        session.abort().catch(() => {});
      } catch {
        // best-effort
      }
    }

    // Final text: last assistant message's text content parts.
    const messages = session.state.messages;
    const assistant = messages.filter((m) => (m as { role?: string }).role === "assistant");
    const last = assistant.length > 0 ? assistant[assistant.length - 1] : undefined;

    let finalText = "";
    if (last) {
      const content = (last as { content?: unknown }).content;
      if (typeof content === "string") {
        finalText = content;
      } else if (Array.isArray(content)) {
        finalText = content
          .filter(
            (c): c is { type: "text"; text: string } =>
              typeof c === "object" &&
              c !== null &&
              (c as { type?: unknown }).type === "text" &&
              typeof (c as { text?: unknown }).text === "string",
          )
          .map((c) => c.text)
          .join("\n");
      }
    }

    return { finalText, timedOut, settle: waitSettled };
  } catch (err) {
    // Attach partial usage so the caller can ledger it even on failure.
    try {
      if (err && typeof err === "object") {
        (err as { usage?: SessionUsage }).usage = sumUsage(session.state.messages);
      }
    } catch {
      // ignore
    }
    // Even on throw, mark the prompt settled so callers don't deadlock
    // awaiting waitSettled.
    settled = true;
    throw err;
  } finally {
    if (timer) clearTimeout(timer);
  }
}
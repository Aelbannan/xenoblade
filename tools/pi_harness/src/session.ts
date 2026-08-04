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
  type AgentSession,
} from "@earendil-works/pi-coding-agent";
import type { ModelSpec, SessionUsage, VerifyResult } from "./types.js";

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
}

export interface MultiPromptOpts {
  maxTimeoutRePrompts: number;
  maxNoMatchRePrompts: number;
  /** Called after each prompt round. Receives the latest final text,
   *  whether it timed out, and the re-prompt count so far (0 after
   *  initial prompt). Returns the next action. */
  onVerify: (
    finalText: string,
    timedOut: boolean,
    rePromptCount: number,
  ) => Promise<VerifyResult>;
  /** Overall session deadline in minutes (default: timeoutMinutes * (1 + maxTimeoutRePrompts)). */
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

  const { session } = await createAgentSession({
    model,
    thinkingLevel: spec.thinkingLevel,
    modelRuntime,
    sessionManager: SessionManager.create(repoRoot, sessionDir),
    // Pin the exact built-in tool surface. The xenoblade-decomp workflow is
    // CLI-driven (hexdiff/run.py via bash) and the skill mandates those tools;
    // an explicit allowlist stops future SDK default tool additions from
    // silently leaking into headless batch sessions. `bash` is load-bearing
    // (hexdiff + run.py diff/size/symbols are bash invocations); the model is
    // prevented from running cycle/batch-cycle/ninja/configure/git by prompt
    // + snapshot-restore, and acceptance is harness-owned regardless.
    tools: ["read", "bash", "edit", "write", "grep", "find", "ls"],
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
  let lastActivityTime = Date.now();
  let lastAgentEnd: { timestamp: number; willRetry: boolean } | undefined;

  const unsubscribe = session.subscribe((event) => {
    // Track activity on meaningful events.
    if (event.type === "message_update" ||
        event.type === "tool_execution_start" ||
        event.type === "tool_execution_end" ||
        event.type === "auto_retry_start") {
      lastActivityTime = Date.now();
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
      transcriptContent += `\n\n---\n**🛠 Tool: ${event.toolName}**\n\n`;
      queueTranscriptWrite();
    }
    if (event.type === "tool_execution_end") {
      transcriptContent += `\n*Result: ${event.isError ? "❌ error" : "✅ success"}*\n\n---\n\n`;
      queueTranscriptWrite();
    }
  });

  // Heartbeat monitor: detect dead sessions (no activity for 2+ minutes
  // while the session should be working). Uses SDK state to avoid false
  // positives during builds (session.isIdle) and compaction (session.isCompacting).
  const SILENCE_THRESHOLD_MS = 120_000; // 2 minutes
  let deadSessionReason: string | undefined;
  const heartbeat = setInterval(() => {
    const silenceMs = Date.now() - lastActivityTime;
    if (silenceMs > SILENCE_THRESHOLD_MS) {
      // Only flag as dead if session should be active (not idle, not compacting).
      if (!session.isIdle && !session.isCompacting && session.retryAttempt === 0) {
        deadSessionReason = `Silent for ${Math.round(silenceMs / 1000)}s, session not idle`;
        process.stderr.write(`[session] ${label}: DEAD SESSION DETECTED — ${deadSessionReason}\n`);
        session.abort().catch(() => {});
      }
    }
  }, 15_000); // check every 15 seconds
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
      const result = await runOnePrompt(session, prompt, timeoutMinutes);
      const usage = sumUsage(session.state.messages);
      process.stderr.write(`[session] ${label}: completed single-prompt (timedOut=${result.timedOut})\n`);
      return {
        finalText: result.finalText,
        sessionFile: session.sessionFile,
        timedOut: result.timedOut,
        usage,
      };
    }

    // ── Multi-prompt path ──
    // Even when caps are 0, we enter this path so onVerify runs once.
    process.stderr.write(`[session] ${label}: starting multi-prompt (timeout=${timeoutMinutes}min, maxTimeoutRePrompts=${multiPrompt.maxTimeoutRePrompts})\n`);
    const { maxTimeoutRePrompts, maxNoMatchRePrompts, onVerify, totalTimeoutMinutes } = multiPrompt;
    const overallDeadline =
      totalTimeoutMinutes && totalTimeoutMinutes > 0
        ? Date.now() + totalTimeoutMinutes * 60 * 1000
        : Infinity;

    const roundUsages: SessionUsage[] = [];
    let rePromptsUsed = 0;
    let finalText = "";
    let timedOut = false;
    let lastRejection: string | undefined;

    // ── Initial prompt ──
    process.stderr.write(`[session] ${label}: round 0 starting\n`);
    transcriptContent += `\n\n## Round 0 (initial)\n\n`;
    queueTranscriptWrite();
    const usageBefore = sumUsage(session.state.messages);
    const initialResult = await runOnePrompt(session, prompt, timeoutMinutes);
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
        };
      }

      // action === "re-prompt" — check budget and deadline
      const effectiveMax = timedOut ? maxTimeoutRePrompts : maxNoMatchRePrompts;
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
      queueTranscriptWrite();

      const roundUsageBefore = sumUsage(session.state.messages);
      const rePromptResult = await runOnePrompt(
        session,
        verifyResult.feedback ?? "",
        remainingMinutes,
      );
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
}

const POST_ABORT_SETTLE_MS = 30_000;

async function runOnePrompt(
  session: AgentSession,
  prompt: string,
  timeoutMinutes: number,
): Promise<PromptResult> {
  let timedOut = false;
  let timer: ReturnType<typeof setTimeout> | undefined;

  try {
    const timeoutMs = timeoutMinutes * 60 * 1000;
    const promptPromise = session
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

    return { finalText, timedOut };
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
    if (timer) clearTimeout(timer);
  }
}
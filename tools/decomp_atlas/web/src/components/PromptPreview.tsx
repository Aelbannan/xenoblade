import { useState } from "react";
import { previewPrompt, startJob } from "../api";
import type { PromptPreviewResult, WorkflowKind } from "../types";

interface PromptPreviewProps {
  functionId: string;
  onJobStarted?: () => void;
}

export function PromptPreview({ functionId, onJobStarted }: PromptPreviewProps) {
  const [workflow, setWorkflow] = useState<WorkflowKind>("new");
  const [fullContext, setFullContext] = useState(false);
  const [result, setResult] = useState<PromptPreviewResult | null>(null);
  const [loading, setLoading] = useState(false);
  const [launching, setLaunching] = useState(false);
  const [copyState, setCopyState] = useState<"idle" | "copied" | "failed">("idle");
  const [launchMsg, setLaunchMsg] = useState<string | null>(null);

  const generate = async () => {
    setLoading(true);
    setLaunchMsg(null);
    setCopyState("idle");
    try {
      const res = await previewPrompt(functionId, {
        workflow,
        full_context: fullContext,
      });
      setResult(res);
    } catch (err) {
      setResult({
        ok: false,
        error: err instanceof Error ? err.message : String(err),
        target_id: functionId,
        workflow,
      });
    } finally {
      setLoading(false);
    }
  };

  const copy = async () => {
    if (!result?.prompt) return;
    try {
      await navigator.clipboard.writeText(result.prompt);
      setCopyState("copied");
    } catch {
      setCopyState("failed");
    }
  };

  const launch = async () => {
    setLaunching(true);
    setLaunchMsg(null);
    try {
      const job = await startJob(functionId, { workflow });
      setLaunchMsg(`Started job ${job.id} (${job.status}). Does not promote or claim.`);
      onJobStarted?.();
    } catch (err) {
      setLaunchMsg(err instanceof Error ? err.message : String(err));
    } finally {
      setLaunching(false);
    }
  };

  const bytes =
    result?.bytes ??
    (result?.prompt ? new TextEncoder().encode(result.prompt).length : undefined);

  return (
    <div className="prompt-preview">
      <div className="toolbar">
        <label>
          Workflow{" "}
          <select
            className="select-input"
            style={{ width: "auto", display: "inline-block", marginLeft: 6 }}
            value={workflow}
            onChange={(e) => setWorkflow(e.target.value as WorkflowKind)}
          >
            <option value="new">new</option>
            <option value="improve">improve</option>
          </select>
        </label>
        <label className="muted" style={{ display: "inline-flex", gap: 6, alignItems: "center" }}>
          <input
            type="checkbox"
            checked={fullContext}
            onChange={(e) => setFullContext(e.target.checked)}
          />
          full_context
        </label>
        <button type="button" className="btn btn-primary" onClick={generate} disabled={loading}>
          {loading ? "Generating…" : "Generate"}
        </button>
        <button
          type="button"
          className="btn"
          onClick={copy}
          disabled={!result?.ok || !result.prompt}
        >
          {copyState === "copied" ? "Copied" : "Copy"}
        </button>
        <button type="button" className="btn" onClick={launch} disabled={launching}>
          {launching ? "Launching…" : "Launch job"}
        </button>
      </div>

      <div className="note">
        Launch runs the allow-listed harness workflow only. Atlas does not promote candidates, claim
        targets, or write source.
      </div>

      {launchMsg ? (
        <div className={launchMsg.startsWith("Started") ? "success-text" : "error-text"}>
          {launchMsg}
        </div>
      ) : null}

      {result && !result.ok ? (
        <div className="error-text">{result.error ?? "Prompt generation failed"}</div>
      ) : null}

      {result?.ok && result.prompt ? (
        <>
          <div className="prompt-meta">
            <span className="mono">{functionId}</span>
            <span>{workflow}</span>
            <span>{bytes?.toLocaleString() ?? "?"} bytes</span>
            <span>{result.prompt.split("\n").length.toLocaleString()} lines</span>
          </div>
          <pre className="code-block">{result.prompt}</pre>
        </>
      ) : !loading && !result ? (
        <div className="empty-state">Generate a prompt preview for this target.</div>
      ) : null}
    </div>
  );
}

import { useEffect, useState } from "react";
import {
  getArtifacts,
  getAttempts,
  getCallees,
  getCallers,
  getFunction,
  getNeighbors,
} from "../api";
import type {
  Artifacts,
  Attempt,
  DetailTab,
  EdgeRef,
  FunctionDetail,
  Neighbor,
} from "../types";
import { ExperimentHistory } from "./ExperimentHistory";
import { PromptPreview } from "./PromptPreview";

interface FunctionDetailsProps {
  functionId: string;
  onSelect: (id: string) => void;
  onClose: () => void;
  onJobStarted?: () => void;
}

const TABS: { id: DetailTab; label: string }[] = [
  { id: "overview", label: "Overview" },
  { id: "source", label: "Source" },
  { id: "retail", label: "Retail PPC" },
  { id: "candidate", label: "Candidate PPC" },
  { id: "graph", label: "Graph" },
  { id: "attempts", label: "Attempts" },
  { id: "experiments", label: "Experiments" },
  { id: "prompt", label: "Prompt" },
];

function CodePane({ text, empty }: { text?: string | null; empty: string }) {
  if (!text) return <div className="empty-state">{empty}</div>;
  return <pre className="code-block">{text}</pre>;
}

function Pill({
  edge,
  onSelect,
}: {
  edge: EdgeRef;
  onSelect: (id: string) => void;
}) {
  return (
    <button type="button" className="pill" onClick={() => onSelect(edge.id)} title={edge.id}>
      {edge.display_name || edge.id}
      {edge.kind ? <span className="dim"> · {edge.kind}</span> : null}
    </button>
  );
}

export function FunctionDetails({
  functionId,
  onSelect,
  onClose,
  onJobStarted,
}: FunctionDetailsProps) {
  const [tab, setTab] = useState<DetailTab>("overview");
  const [fn, setFn] = useState<FunctionDetail | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(true);

  const [artifacts, setArtifacts] = useState<Artifacts | null>(null);
  const [artifactsError, setArtifactsError] = useState<string | null>(null);
  const [artifactsLoading, setArtifactsLoading] = useState(false);

  const [callers, setCallers] = useState<EdgeRef[]>([]);
  const [callees, setCallees] = useState<EdgeRef[]>([]);
  const [neighbors, setNeighbors] = useState<Neighbor[]>([]);
  const [graphLoading, setGraphLoading] = useState(false);

  const [attempts, setAttempts] = useState<Attempt[]>([]);
  const [attemptsLoading, setAttemptsLoading] = useState(false);
  const [attemptsError, setAttemptsError] = useState<string | null>(null);

  useEffect(() => {
    setTab("overview");
    setArtifacts(null);
    setCallers([]);
    setCallees([]);
    setNeighbors([]);
    setAttempts([]);
  }, [functionId]);

  useEffect(() => {
    let cancelled = false;
    setLoading(true);
    setError(null);
    getFunction(functionId)
      .then((row) => {
        if (!cancelled) setFn(row);
      })
      .catch((err: unknown) => {
        if (!cancelled) {
          setFn(null);
          setError(err instanceof Error ? err.message : String(err));
        }
      })
      .finally(() => {
        if (!cancelled) setLoading(false);
      });
    return () => {
      cancelled = true;
    };
  }, [functionId]);

  useEffect(() => {
    if (tab !== "source" && tab !== "retail" && tab !== "candidate") return;
    if (artifacts || artifactsLoading) return;
    let cancelled = false;
    setArtifactsLoading(true);
    setArtifactsError(null);
    getArtifacts(functionId)
      .then((row) => {
        if (!cancelled) setArtifacts(row);
      })
      .catch((err: unknown) => {
        if (!cancelled) setArtifactsError(err instanceof Error ? err.message : String(err));
      })
      .finally(() => {
        if (!cancelled) setArtifactsLoading(false);
      });
    return () => {
      cancelled = true;
    };
  }, [tab, functionId, artifacts, artifactsLoading]);

  useEffect(() => {
    if (tab !== "graph") return;
    let cancelled = false;
    setGraphLoading(true);
    Promise.all([getCallers(functionId), getCallees(functionId), getNeighbors(functionId)])
      .then(([cIn, cOut, nbrs]) => {
        if (cancelled) return;
        setCallers(cIn);
        setCallees(cOut);
        setNeighbors(nbrs);
      })
      .catch(() => {
        if (!cancelled) {
          setCallers([]);
          setCallees([]);
          setNeighbors([]);
        }
      })
      .finally(() => {
        if (!cancelled) setGraphLoading(false);
      });
    return () => {
      cancelled = true;
    };
  }, [tab, functionId]);

  useEffect(() => {
    if (tab !== "attempts") return;
    let cancelled = false;
    setAttemptsLoading(true);
    setAttemptsError(null);
    getAttempts(functionId)
      .then((rows) => {
        if (!cancelled) setAttempts(rows);
      })
      .catch((err: unknown) => {
        if (!cancelled) setAttemptsError(err instanceof Error ? err.message : String(err));
      })
      .finally(() => {
        if (!cancelled) setAttemptsLoading(false);
      });
    return () => {
      cancelled = true;
    };
  }, [tab, functionId]);

  return (
    <section className="panel details-panel">
      <div className="panel-header">
        <div style={{ minWidth: 0 }}>
          <div style={{ display: "flex", gap: "0.5rem", alignItems: "center", flexWrap: "wrap" }}>
            <strong className="mono" style={{ color: "#fff" }}>
              {fn?.display_name ?? functionId}
            </strong>
            {fn?.stale || artifacts?.stale ? (
              <span className="badge badge-warn">stale</span>
            ) : null}
            {fn ? <span className="badge badge-neutral">{fn.status}</span> : null}
          </div>
          <div className="muted mono" style={{ fontSize: "0.8rem" }}>
            {functionId}
          </div>
        </div>
        <button type="button" className="btn btn-ghost" onClick={onClose} aria-label="Close">
          ✕
        </button>
      </div>

      <div className="details-tabs">
        {TABS.map((t) => (
          <button
            key={t.id}
            type="button"
            className={`details-tab ${tab === t.id ? "active" : ""}`}
            onClick={() => setTab(t.id)}
          >
            {t.label}
          </button>
        ))}
      </div>

      <div className="details-body">
        {loading ? (
          <div className="loading-block">
            <div className="spinner" />
            Loading…
          </div>
        ) : null}
        {error ? <div className="error-text">{error}</div> : null}

        {!loading && fn && tab === "overview" ? (
          <div className="kv-grid">
            <div className="kv">
              <span className="label">Target ID</span>
              <span className="value mono">{fn.id}</span>
            </div>
            <div className="kv">
              <span className="label">Display name</span>
              <span className="value">{fn.display_name}</span>
            </div>
            <div className="kv">
              <span className="label">MWCC symbol</span>
              <span className="value mono">{fn.symbol ?? "—"}</span>
            </div>
            <div className="kv">
              <span className="label">Address / size</span>
              <span className="value mono">
                {fn.address ?? "—"}
                {fn.size != null ? ` · ${fn.size} bytes` : ""}
              </span>
            </div>
            <div className="kv">
              <span className="label">Source</span>
              <span className="value mono">{fn.source ?? "—"}</span>
            </div>
            <div className="kv">
              <span className="label">Unit</span>
              <span className="value mono">{fn.unit ?? "—"}</span>
            </div>
            <div className="kv">
              <span className="label">Workflow / match</span>
              <span className="value">
                {fn.workflow_status} · {fn.status}
                {fn.instruction_percent != null
                  ? ` · ${fn.instruction_percent.toFixed(1)}%`
                  : ""}
              </span>
            </div>
            <div className="kv">
              <span className="label">Readiness</span>
              <span className="value">{fn.readiness}</span>
            </div>
            <div className="kv">
              <span className="label">Tier / milestone</span>
              <span className="value">
                {fn.tier ?? "—"} / {fn.milestone ?? "—"}
              </span>
            </div>
            <div className="kv">
              <span className="label">Claim</span>
              <span className="value">
                {fn.owner ? `${fn.owner}${fn.claimed_at ? ` @ ${fn.claimed_at}` : ""}` : "unclaimed"}
              </span>
            </div>
            <div className="kv">
              <span className="label">Buildable</span>
              <span className="value">{fn.buildable ? "yes" : "no"}</span>
            </div>
            <div className="kv">
              <span className="label">Difficulty</span>
              <span className="value mono">
                {fn.difficulty != null ? fn.difficulty.toFixed(1) : "—"}
              </span>
            </div>
            {fn.readiness_blockers && fn.readiness_blockers.length > 0 ? (
              <div className="kv" style={{ gridColumn: "1 / -1" }}>
                <span className="label">Readiness blockers</span>
                <span className="value">{fn.readiness_blockers.join("; ")}</span>
              </div>
            ) : null}
            {fn.warnings && fn.warnings.length > 0 ? (
              <div className="kv" style={{ gridColumn: "1 / -1" }}>
                <span className="label">Warnings</span>
                <span className="value error-text">{fn.warnings.join("; ")}</span>
              </div>
            ) : null}
            {fn.notes ? (
              <div className="kv" style={{ gridColumn: "1 / -1" }}>
                <span className="label">Notes</span>
                <span className="value">{fn.notes}</span>
              </div>
            ) : null}
          </div>
        ) : null}

        {tab === "source" ? (
          artifactsLoading ? (
            <div className="loading-block">Loading source…</div>
          ) : artifactsError ? (
            <div className="error-text">{artifactsError}</div>
          ) : (
            <CodePane text={artifacts?.source_cpp} empty="No C++ source extracted for this target." />
          )
        ) : null}

        {tab === "retail" ? (
          artifactsLoading ? (
            <div className="loading-block">Loading retail PPC…</div>
          ) : artifactsError ? (
            <div className="error-text">{artifactsError}</div>
          ) : (
            <CodePane text={artifacts?.retail_ppc} empty="No retail PPC available." />
          )
        ) : null}

        {tab === "candidate" ? (
          artifactsLoading ? (
            <div className="loading-block">Loading candidate PPC…</div>
          ) : artifactsError ? (
            <div className="error-text">{artifactsError}</div>
          ) : (
            <CodePane text={artifacts?.candidate_ppc} empty="No candidate PPC available." />
          )
        ) : null}

        {tab === "graph" ? (
          graphLoading ? (
            <div className="loading-block">Loading graph…</div>
          ) : (
            <div style={{ display: "grid", gap: "1rem" }}>
              <div>
                <div className="label" style={{ marginBottom: 8 }}>
                  Callers
                </div>
                <div className="pill-row">
                  {callers.length === 0 ? <span className="dim">None</span> : null}
                  {callers.map((e) => (
                    <Pill key={`in-${e.id}-${e.kind ?? ""}`} edge={e} onSelect={onSelect} />
                  ))}
                </div>
              </div>
              <div>
                <div className="label" style={{ marginBottom: 8 }}>
                  Callees
                </div>
                <div className="pill-row">
                  {callees.length === 0 ? <span className="dim">None</span> : null}
                  {callees.map((e) => (
                    <Pill key={`out-${e.id}-${e.kind ?? ""}`} edge={e} onSelect={onSelect} />
                  ))}
                </div>
              </div>
              <div>
                <div className="label" style={{ marginBottom: 8 }}>
                  Similar (advisory)
                </div>
                <div className="pill-row">
                  {neighbors.length === 0 ? <span className="dim">None</span> : null}
                  {neighbors.map((n) => (
                    <button
                      key={n.id}
                      type="button"
                      className="pill"
                      onClick={() => onSelect(n.id)}
                      title={n.id}
                    >
                      {n.display_name} · {(n.score * 100).toFixed(0)}%
                    </button>
                  ))}
                </div>
              </div>
            </div>
          )
        ) : null}

        {tab === "attempts" ? (
          attemptsLoading ? (
            <div className="loading-block">Loading attempts…</div>
          ) : attemptsError ? (
            <div className="error-text">{attemptsError}</div>
          ) : attempts.length === 0 ? (
            <div className="empty-state">No attempts recorded.</div>
          ) : (
            <div className="data-table-wrap" style={{ maxHeight: 360 }}>
              <table className="data-table">
                <thead>
                  <tr>
                    <th>When</th>
                    <th>Status</th>
                    <th>Match</th>
                    <th>Hypothesis</th>
                    <th>Next change</th>
                  </tr>
                </thead>
                <tbody>
                  {attempts.map((a, i) => (
                    <tr key={`${a.timestamp ?? i}-${a.status}`}>
                      <td className="dim">{a.timestamp ?? "—"}</td>
                      <td>
                        <span className="badge badge-neutral">{a.status}</span>
                      </td>
                      <td className="mono">
                        {a.match_percent != null ? `${a.match_percent.toFixed(1)}%` : "—"}
                      </td>
                      <td title={a.hypothesis ?? undefined}>{a.hypothesis ?? "—"}</td>
                      <td className="dim" title={a.next_change ?? undefined}>
                        {a.next_change ?? "—"}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          )
        ) : null}

        {tab === "experiments" ? <ExperimentHistory functionId={functionId} /> : null}

        {tab === "prompt" ? (
          <PromptPreview functionId={functionId} onJobStarted={onJobStarted} />
        ) : null}
      </div>
    </section>
  );
}

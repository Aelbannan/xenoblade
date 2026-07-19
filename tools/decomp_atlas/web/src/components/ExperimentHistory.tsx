import { useEffect, useState } from "react";
import { getExperiments } from "../api";
import type { Experiment } from "../types";

interface ExperimentHistoryProps {
  functionId: string;
}

function formatDuration(sec: number | null | undefined): string {
  if (sec == null) return "—";
  if (sec < 60) return `${sec.toFixed(1)}s`;
  return `${(sec / 60).toFixed(1)}m`;
}

export function ExperimentHistory({ functionId }: ExperimentHistoryProps) {
  const [items, setItems] = useState<Experiment[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    let cancelled = false;
    setLoading(true);
    setError(null);
    getExperiments(functionId)
      .then((rows) => {
        if (!cancelled) setItems(rows);
      })
      .catch((err: unknown) => {
        if (!cancelled) setError(err instanceof Error ? err.message : String(err));
      })
      .finally(() => {
        if (!cancelled) setLoading(false);
      });
    return () => {
      cancelled = true;
    };
  }, [functionId]);

  if (loading) {
    return (
      <div className="loading-block">
        <div className="spinner" />
        Loading experiments…
      </div>
    );
  }

  if (error) return <div className="error-text">{error}</div>;

  if (items.length === 0) {
    return <div className="empty-state">No harness experiments for this function.</div>;
  }

  return (
    <div className="data-table-wrap" style={{ maxHeight: 360 }}>
      <table className="data-table">
        <thead>
          <tr>
            <th>When</th>
            <th>Workflow</th>
            <th>Model</th>
            <th>Run</th>
            <th>Duration</th>
            <th>Match</th>
            <th>Equiv</th>
            <th>Size</th>
            <th>Winner</th>
            <th>Error / hypothesis</th>
            <th>Artifact</th>
          </tr>
        </thead>
        <tbody>
          {items.map((ex, idx) => (
            <tr key={`${ex.experiment_id}-${ex.run_index ?? idx}`}>
              <td className="dim">{ex.timestamp ?? "—"}</td>
              <td>{ex.workflow}</td>
              <td className="mono" title={ex.model ?? undefined}>
                {ex.model_id ?? ex.model ?? "—"}
              </td>
              <td className="mono">{ex.run_index ?? "—"}</td>
              <td>{formatDuration(ex.duration_seconds)}</td>
              <td className="mono">
                {ex.match_percent != null ? `${ex.match_percent.toFixed(1)}%` : "—"}
              </td>
              <td>{ex.equivalence ?? "—"}</td>
              <td>
                {ex.size_ok == null ? "—" : ex.size_ok ? "ok" : "over"}
              </td>
              <td>{ex.winner ? <span className="badge badge-status-accepted">winner</span> : "—"}</td>
              <td title={ex.error ?? ex.hypothesis ?? undefined}>
                {ex.error ? (
                  <span className="error-text">{ex.error}</span>
                ) : (
                  <span className="dim">{ex.hypothesis ?? ex.status ?? "—"}</span>
                )}
              </td>
              <td className="mono dim" title={ex.artifact ?? undefined}>
                {ex.artifact ? ex.artifact.split("/").slice(-2).join("/") : "—"}
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

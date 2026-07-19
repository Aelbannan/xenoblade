import { useCallback, useEffect, useState } from "react";
import { cancelJob, listJobs } from "../api";
import type { Job } from "../types";

interface JobPanelProps {
  refreshKey?: number;
  compact?: boolean;
}

function isActive(status: string): boolean {
  const s = status.toLowerCase();
  return s === "running" || s === "queued";
}

export function JobPanel({ refreshKey = 0, compact }: JobPanelProps) {
  const [jobs, setJobs] = useState<Job[]>([]);
  const [error, setError] = useState<string | null>(null);
  const [busyId, setBusyId] = useState<string | null>(null);

  const refresh = useCallback(() => {
    listJobs()
      .then(setJobs)
      .catch((err: unknown) => setError(err instanceof Error ? err.message : String(err)));
  }, []);

  useEffect(() => {
    refresh();
    const timer = window.setInterval(refresh, 4000);
    return () => window.clearInterval(timer);
  }, [refresh, refreshKey]);

  const onCancel = async (jobId: string) => {
    setBusyId(jobId);
    try {
      await cancelJob(jobId);
      refresh();
    } catch (err) {
      setError(err instanceof Error ? err.message : String(err));
    } finally {
      setBusyId(null);
    }
  };

  return (
    <div className="panel">
      <div className="panel-header">
        <strong>Jobs</strong>
        <button type="button" className="btn" onClick={refresh}>
          Refresh
        </button>
      </div>
      <div className="panel-body">
        {error ? <div className="error-text" style={{ marginBottom: "0.5rem" }}>{error}</div> : null}
        {jobs.length === 0 ? (
          <div className="empty-state" style={{ padding: "1rem" }}>
            No jobs yet.
          </div>
        ) : (
          <div className="job-list">
            {(compact ? jobs.slice(0, 8) : jobs).map((job) => (
              <div key={job.id} className="job-row">
                <span className={`badge ${isActive(job.status) ? "badge-status-active" : "badge-neutral"}`}>
                  {job.status}
                </span>
                <div className="grow">
                  <div className="mono">{job.target_id}</div>
                  <div className="dim">
                    {job.workflow}
                    {job.started_at ? ` · ${job.started_at}` : ""}
                    {job.error ? ` · ${job.error}` : ""}
                  </div>
                </div>
                {isActive(job.status) ? (
                  <button
                    type="button"
                    className="btn btn-danger"
                    disabled={busyId === job.id}
                    onClick={() => onCancel(job.id)}
                  >
                    Cancel
                  </button>
                ) : null}
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}

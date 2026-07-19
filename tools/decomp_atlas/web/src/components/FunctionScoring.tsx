import { useEffect, useState } from "react";
import { listFunctions } from "../api";
import type { FunctionSummary } from "../types";

interface FunctionScoringProps {
  selectedId: string | null;
  pathFilter: string | null;
  search: string;
  onSelect: (id: string) => void;
}

export function FunctionScoring({
  selectedId,
  pathFilter,
  search,
  onSelect,
}: FunctionScoringProps) {
  const [items, setItems] = useState<FunctionSummary[]>([]);
  const [total, setTotal] = useState(0);
  const [page, setPage] = useState(1);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const pageSize = 100;

  useEffect(() => {
    setPage(1);
  }, [pathFilter, search]);

  useEffect(() => {
    let cancelled = false;
    setLoading(true);
    setError(null);
    listFunctions(
      {
        search: search || undefined,
        path: pathFilter || undefined,
        source: pathFilter || undefined,
        unit: pathFilter || undefined,
        sort: "difficulty",
      },
      page,
      pageSize,
    )
      .then((res) => {
        if (cancelled) return;
        const sorted = [...(res.items ?? [])].sort(
          (a, b) => (b.difficulty ?? -1) - (a.difficulty ?? -1),
        );
        setItems(sorted);
        setTotal(res.total);
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
  }, [page, pathFilter, search]);

  const totalPages = Math.max(1, Math.ceil(total / pageSize));

  return (
    <div className="panel">
      <div className="panel-header">
        <div>
          <strong>Function scoring</strong>
          <div className="muted" style={{ fontSize: "0.8rem" }}>
            Advisory difficulty (0–100) · sorted hardest first
          </div>
        </div>
        <span className="muted">{total.toLocaleString()} functions</span>
      </div>
      {error ? (
        <div className="error-text" style={{ padding: "1rem" }}>
          {error}
        </div>
      ) : null}
      {loading && items.length === 0 ? (
        <div className="loading-block">
          <div className="spinner" />
          Loading scores…
        </div>
      ) : (
        <>
          <div className="data-table-wrap">
            <table className="data-table">
              <thead>
                <tr>
                  <th>Difficulty</th>
                  <th>Readiness</th>
                  <th>Display</th>
                  <th>Status</th>
                  <th>Size</th>
                  <th>Calls</th>
                  <th>Unit</th>
                </tr>
              </thead>
              <tbody>
                {items.map((fn) => (
                  <tr
                    key={fn.id}
                    className={selectedId === fn.id ? "selected" : undefined}
                    onClick={() => onSelect(fn.id)}
                  >
                    <td className="mono">
                      {fn.difficulty != null ? fn.difficulty.toFixed(1) : "—"}
                    </td>
                    <td>
                      <span className="badge badge-neutral">{fn.readiness}</span>
                    </td>
                    <td title={fn.display_name}>{fn.display_name}</td>
                    <td>
                      <span className="badge badge-neutral">{fn.status}</span>
                    </td>
                    <td className="mono dim">{fn.size ?? "—"}</td>
                    <td className="mono dim">
                      {(fn.direct_call_count ?? 0) +
                        (fn.unresolved_call_count ? ` / ${fn.unresolved_call_count}u` : "")}
                    </td>
                    <td className="dim">{fn.unit ?? "—"}</td>
                  </tr>
                ))}
                {items.length === 0 && !loading ? (
                  <tr>
                    <td colSpan={7} className="empty-state">
                      No scored functions.
                    </td>
                  </tr>
                ) : null}
              </tbody>
            </table>
          </div>
          <div className="pager">
            <span>
              Page {page} / {totalPages}
              {loading ? " · refreshing…" : ""}
            </span>
            <div className="pager-actions">
              <button
                type="button"
                className="btn"
                disabled={page <= 1}
                onClick={() => setPage((p) => p - 1)}
              >
                Prev
              </button>
              <button
                type="button"
                className="btn"
                disabled={page >= totalPages}
                onClick={() => setPage((p) => p + 1)}
              >
                Next
              </button>
            </div>
          </div>
        </>
      )}
    </div>
  );
}

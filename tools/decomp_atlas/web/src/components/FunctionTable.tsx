import type { FunctionSummary } from "../types";

interface FunctionTableProps {
  items: FunctionSummary[];
  total: number;
  page: number;
  pageSize: number;
  selectedId: string | null;
  loading?: boolean;
  error?: string | null;
  onSelect: (id: string) => void;
  onPageChange: (page: number) => void;
}

function statusClass(status: string): string {
  const key = status.toLowerCase().replace(/\s+/g, "_");
  return `badge badge-status-${key}`;
}

function formatMatch(pct: number | null | undefined): string {
  if (pct == null || Number.isNaN(pct)) return "—";
  return `${pct.toFixed(1)}%`;
}

export function FunctionTable({
  items,
  total,
  page,
  pageSize,
  selectedId,
  loading,
  error,
  onSelect,
  onPageChange,
}: FunctionTableProps) {
  const totalPages = Math.max(1, Math.ceil(total / pageSize));
  const from = total === 0 ? 0 : (page - 1) * pageSize + 1;
  const to = Math.min(total, page * pageSize);

  return (
    <div className="panel">
      <div className="panel-header">
        <strong>Catalog</strong>
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
          Loading functions…
        </div>
      ) : (
        <>
          <div className="data-table-wrap">
            <table className="data-table">
              <thead>
                <tr>
                  <th>Status</th>
                  <th>Match</th>
                  <th>Display</th>
                  <th>Symbol</th>
                  <th>Unit</th>
                  <th>Readiness</th>
                  <th>Tier</th>
                </tr>
              </thead>
              <tbody>
                {items.map((fn) => (
                  <tr
                    key={fn.id}
                    className={selectedId === fn.id ? "selected" : undefined}
                    onClick={() => onSelect(fn.id)}
                  >
                    <td>
                      <span className={statusClass(fn.status)}>{fn.status}</span>
                    </td>
                    <td className="mono">{formatMatch(fn.instruction_percent)}</td>
                    <td title={fn.display_name}>{fn.display_name}</td>
                    <td className="mono dim" title={fn.symbol ?? undefined}>
                      {fn.symbol ?? "—"}
                    </td>
                    <td className="dim" title={fn.unit ?? undefined}>
                      {fn.unit ?? "—"}
                    </td>
                    <td>
                      <span className="badge badge-neutral">{fn.readiness}</span>
                    </td>
                    <td>{fn.tier ?? "—"}</td>
                  </tr>
                ))}
                {items.length === 0 && !loading ? (
                  <tr>
                    <td colSpan={7} className="empty-state">
                      No functions match the current filters.
                    </td>
                  </tr>
                ) : null}
              </tbody>
            </table>
          </div>
          <div className="pager">
            <span>
              {from}–{to} of {total.toLocaleString()}
              {loading ? " · refreshing…" : ""}
            </span>
            <div className="pager-actions">
              <button
                type="button"
                className="btn"
                disabled={page <= 1}
                onClick={() => onPageChange(page - 1)}
              >
                Prev
              </button>
              <span className="muted">
                Page {page} / {totalPages}
              </span>
              <button
                type="button"
                className="btn"
                disabled={page >= totalPages}
                onClick={() => onPageChange(page + 1)}
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

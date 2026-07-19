import { useCallback, useEffect, useMemo, useState } from "react";
import { getProject, getUnits, listFunctions } from "./api";
import { FunctionCloud } from "./components/FunctionCloud";
import { FunctionDetails } from "./components/FunctionDetails";
import { FunctionScoring } from "./components/FunctionScoring";
import { FunctionTable } from "./components/FunctionTable";
import { Header } from "./components/Header";
import { JobPanel } from "./components/JobPanel";
import { Sidebar } from "./components/Sidebar";
import type { FunctionSummary, MainTab, ProjectInfo, UnitNode } from "./types";

const PAGE_SIZE = 100;

interface UrlState {
  id: string | null;
  tab: MainTab;
  search: string;
  path: string | null;
  page: number;
}

function parseUrl(): UrlState {
  const params = new URLSearchParams(window.location.search);
  const tabRaw = params.get("tab");
  const tab: MainTab =
    tabRaw === "cloud" || tabRaw === "scoring" || tabRaw === "catalog" ? tabRaw : "catalog";
  const page = Math.max(1, Number(params.get("page") || "1") || 1);
  return {
    id: params.get("id"),
    tab,
    search: params.get("q") ?? "",
    path: params.get("path"),
    page,
  };
}

function writeUrl(state: UrlState) {
  const params = new URLSearchParams();
  if (state.id) params.set("id", state.id);
  if (state.tab !== "catalog") params.set("tab", state.tab);
  if (state.search) params.set("q", state.search);
  if (state.path) params.set("path", state.path);
  if (state.page > 1) params.set("page", String(state.page));
  const qs = params.toString();
  const next = qs ? `?${qs}` : window.location.pathname;
  window.history.replaceState(null, "", next);
}

export function App() {
  const initial = useMemo(() => parseUrl(), []);
  const [project, setProject] = useState<ProjectInfo | null>(null);
  const [projectError, setProjectError] = useState<string | null>(null);

  const [units, setUnits] = useState<UnitNode[]>([]);
  const [unitsLoading, setUnitsLoading] = useState(true);
  const [unitsError, setUnitsError] = useState<string | null>(null);

  const [tab, setTab] = useState<MainTab>(initial.tab);
  const [search, setSearch] = useState(initial.search);
  const [debouncedSearch, setDebouncedSearch] = useState(initial.search);
  const [path, setPath] = useState<string | null>(initial.path);
  const [page, setPage] = useState(initial.page);
  const [selectedId, setSelectedId] = useState<string | null>(initial.id);

  const [items, setItems] = useState<FunctionSummary[]>([]);
  const [total, setTotal] = useState(0);
  const [listLoading, setListLoading] = useState(false);
  const [listError, setListError] = useState<string | null>(null);

  const [jobRefresh, setJobRefresh] = useState(0);

  useEffect(() => {
    const t = window.setTimeout(() => setDebouncedSearch(search), 250);
    return () => window.clearTimeout(t);
  }, [search]);

  useEffect(() => {
    writeUrl({ id: selectedId, tab, search: debouncedSearch, path, page });
  }, [selectedId, tab, debouncedSearch, path, page]);

  useEffect(() => {
    getProject()
      .then(setProject)
      .catch((err: unknown) =>
        setProjectError(err instanceof Error ? err.message : String(err)),
      );
    getUnits()
      .then(setUnits)
      .catch((err: unknown) =>
        setUnitsError(err instanceof Error ? err.message : String(err)),
      )
      .finally(() => setUnitsLoading(false));
  }, []);

  useEffect(() => {
    if (tab !== "catalog") return;
    let cancelled = false;
    setListLoading(true);
    setListError(null);
    listFunctions(
      {
        search: debouncedSearch || undefined,
        path: path || undefined,
        source: path || undefined,
        unit: path || undefined,
      },
      page,
      PAGE_SIZE,
    )
      .then((res) => {
        if (cancelled) return;
        setItems(res.items ?? []);
        setTotal(res.total ?? 0);
      })
      .catch((err: unknown) => {
        if (!cancelled) setListError(err instanceof Error ? err.message : String(err));
      })
      .finally(() => {
        if (!cancelled) setListLoading(false);
      });
    return () => {
      cancelled = true;
    };
  }, [tab, debouncedSearch, path, page]);

  const onPathSelect = useCallback((next: string | null) => {
    setPath(next);
    setPage(1);
  }, []);

  const onSearchChange = useCallback((value: string) => {
    setSearch(value);
    setPage(1);
  }, []);

  const onSelect = useCallback((id: string) => {
    setSelectedId(id);
  }, []);

  return (
    <div className="app-shell">
      <Header project={project} />
      {projectError ? (
        <div className="panel" style={{ padding: "0.75rem 1rem" }}>
          <span className="error-text">Failed to load project: {projectError}</span>
          <span className="muted"> — start the Atlas server on :8765</span>
        </div>
      ) : null}

      <div className="app-body">
        <Sidebar
          units={units}
          selectedPath={path}
          search={search}
          onSearchChange={onSearchChange}
          onPathSelect={onPathSelect}
          loading={unitsLoading}
          error={unitsError}
        />

        <div className="main-column">
          <div className="tabs" role="tablist">
            {(
              [
                ["catalog", "Catalog"],
                ["cloud", "Cloud"],
                ["scoring", "Scoring"],
              ] as const
            ).map(([id, label]) => (
              <button
                key={id}
                type="button"
                role="tab"
                aria-selected={tab === id}
                className={`tab-btn ${tab === id ? "active" : ""}`}
                onClick={() => setTab(id)}
              >
                {label}
              </button>
            ))}
          </div>

          {tab === "catalog" ? (
            <FunctionTable
              items={items}
              total={total}
              page={page}
              pageSize={PAGE_SIZE}
              selectedId={selectedId}
              loading={listLoading}
              error={listError}
              onSelect={onSelect}
              onPageChange={setPage}
            />
          ) : null}

          {tab === "cloud" ? (
            <FunctionCloud
              selectedId={selectedId}
              pathFilter={path}
              onSelect={onSelect}
            />
          ) : null}

          {tab === "scoring" ? (
            <FunctionScoring
              selectedId={selectedId}
              pathFilter={path}
              search={debouncedSearch}
              onSelect={onSelect}
            />
          ) : null}

          <JobPanel refreshKey={jobRefresh} compact />

          {selectedId ? (
            <FunctionDetails
              functionId={selectedId}
              onSelect={onSelect}
              onClose={() => setSelectedId(null)}
              onJobStarted={() => setJobRefresh((n) => n + 1)}
            />
          ) : null}
        </div>
      </div>
    </div>
  );
}

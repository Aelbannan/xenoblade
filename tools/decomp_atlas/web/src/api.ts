import type {
  Artifacts,
  EdgeRef,
  Experiment,
  FunctionDetail,
  FunctionFilters,
  Job,
  Neighbor,
  PaginatedFunctions,
  ProjectionResponse,
  PromptPreviewResult,
  ProjectInfo,
  Attempt,
  UnitNode,
  WorkflowKind,
} from "./types";

const BASE = "/api";

async function request<T>(path: string, init?: RequestInit): Promise<T> {
  const res = await fetch(`${BASE}${path}`, {
    headers: {
      Accept: "application/json",
      ...(init?.body ? { "Content-Type": "application/json" } : {}),
      ...init?.headers,
    },
    ...init,
  });

  if (!res.ok) {
    let detail = res.statusText;
    try {
      const body = (await res.json()) as { error?: string; detail?: string };
      detail = body.error || body.detail || detail;
    } catch {
      /* ignore */
    }
    throw new Error(`${res.status} ${detail}`);
  }

  if (res.status === 204) {
    return undefined as T;
  }

  return (await res.json()) as T;
}

function toQuery(params: Record<string, string | number | boolean | null | undefined>): string {
  const sp = new URLSearchParams();
  for (const [key, value] of Object.entries(params)) {
    if (value === undefined || value === null || value === "") continue;
    sp.set(key, String(value));
  }
  const qs = sp.toString();
  return qs ? `?${qs}` : "";
}

function asItems<T>(payload: T[] | { items?: T[]; tree?: T[] } | null | undefined): T[] {
  if (Array.isArray(payload)) return payload;
  if (payload && Array.isArray(payload.tree) && payload.tree.length) return payload.tree;
  if (payload && Array.isArray(payload.items)) return payload.items;
  return [];
}

export function getProject(): Promise<ProjectInfo> {
  return request<ProjectInfo>("/project");
}

export function listFunctions(
  filters: FunctionFilters = {},
  page = 1,
  pageSize = 100,
): Promise<PaginatedFunctions> {
  return request<PaginatedFunctions>(
    `/functions${toQuery({
      page,
      page_size: pageSize,
      search: filters.search,
      region: filters.region,
      kind: filters.kind,
      status: filters.status,
      workflow: filters.workflow,
      milestone: filters.milestone,
      tier: filters.tier,
      capability: filters.capability,
      unit: filters.unit,
      source: filters.source,
      path: filters.path,
      owner: filters.owner,
      readiness: filters.readiness,
      buildable: filters.buildable ?? undefined,
      indirect: filters.indirect ?? undefined,
      unresolved: filters.unresolved ?? undefined,
      min_match: filters.min_match ?? undefined,
      max_match: filters.max_match ?? undefined,
      min_size: filters.min_size ?? undefined,
      max_size: filters.max_size ?? undefined,
      sort: filters.sort,
    })}`,
  );
}

export function getFunction(id: string): Promise<FunctionDetail> {
  return request<FunctionDetail>(`/functions/${encodeURIComponent(id)}`);
}

export async function getArtifacts(id: string): Promise<Artifacts> {
  const raw = await request<Artifacts & {
    cpp_source?: string | null;
    retail_asm?: string | null;
    candidate_asm?: string | null;
  }>(`/functions/${encodeURIComponent(id)}/artifacts`);
  return {
    ...raw,
    source_cpp: raw.source_cpp ?? raw.cpp_source ?? null,
    retail_ppc: raw.retail_ppc ?? raw.retail_asm ?? null,
    candidate_ppc: raw.candidate_ppc ?? raw.candidate_asm ?? null,
  };
}

export async function getNeighbors(id: string): Promise<Neighbor[]> {
  const raw = await request<Neighbor[] | { items?: Neighbor[] }>(
    `/functions/${encodeURIComponent(id)}/neighbors`,
  );
  return asItems(raw);
}

export async function getAttempts(id: string): Promise<Attempt[]> {
  const raw = await request<Attempt[] | { items?: Attempt[] }>(
    `/functions/${encodeURIComponent(id)}/attempts`,
  );
  return asItems(raw);
}

export async function getExperiments(id: string): Promise<Experiment[]> {
  const raw = await request<Experiment[] | { items?: Experiment[] }>(
    `/functions/${encodeURIComponent(id)}/experiments`,
  );
  return asItems(raw);
}

export async function getCallers(id: string): Promise<EdgeRef[]> {
  const raw = await request<EdgeRef[] | { items?: EdgeRef[] }>(
    `/functions/${encodeURIComponent(id)}/callers`,
  );
  return asItems(raw);
}

export async function getCallees(id: string): Promise<EdgeRef[]> {
  const raw = await request<EdgeRef[] | { items?: EdgeRef[] }>(
    `/functions/${encodeURIComponent(id)}/callees`,
  );
  return asItems(raw);
}

export async function getProjection(): Promise<ProjectionResponse> {
  const raw = await request<ProjectionResponse & { items?: ProjectionResponse["points"] }>(
    "/projection",
  );
  return {
    points: raw.points ?? raw.items ?? [],
    version: raw.version,
  };
}

export async function getUnits(): Promise<UnitNode[]> {
  const raw = await request<UnitNode[] | { items?: UnitNode[]; tree?: UnitNode[] }>("/units");
  const tree = asItems(raw);
  if (tree.length && tree[0].path) return tree;
  // Fallback: flatten unit rows into a shallow tree
  const flat = Array.isArray(raw) ? raw : raw?.items ?? [];
  return flat.map((row) => {
    const unit = (row as { unit?: string; path?: string }).unit || row.path || "unknown";
    return {
      path: unit,
      name: unit.split("/").pop() || unit,
      kind: "unit",
      function_count: row.function_count ?? 0,
      children: [],
    };
  });
}

export function previewPrompt(
  id: string,
  opts: { workflow: WorkflowKind; full_context?: boolean },
): Promise<PromptPreviewResult> {
  return request<PromptPreviewResult>(`/functions/${encodeURIComponent(id)}/prompt-preview`, {
    method: "POST",
    body: JSON.stringify({
      workflow: opts.workflow,
      full_context: opts.full_context ?? false,
    }),
  });
}

export async function listJobs(): Promise<Job[]> {
  const raw = await request<Job[] | { items?: Job[] }>("/jobs");
  return asItems(raw).map((job) => ({
    ...job,
    id: job.id || (job as { job_id?: string }).job_id || "",
  }));
}

export function startJob(
  id: string,
  opts: { workflow: WorkflowKind },
): Promise<Job> {
  return request<Job>(`/functions/${encodeURIComponent(id)}/jobs`, {
    method: "POST",
    body: JSON.stringify({ workflow: opts.workflow }),
  });
}

export function cancelJob(jobId: string): Promise<Job | { ok: boolean }> {
  return request(`/jobs/${encodeURIComponent(jobId)}/cancel`, {
    method: "POST",
    body: JSON.stringify({}),
  });
}

/** Readiness classifications from Atlas scoring (advisory). */
export type Readiness =
  | "accepted"
  | "safe_leaf"
  | "callees_accepted"
  | "blocked_unresolved"
  | "blocked_indirect"
  | "claimed"
  | "not_buildable"
  | "active_unclaimed"
  | "backlog"
  | string;

export type MainTab = "catalog" | "cloud" | "scoring";

export type DetailTab =
  | "overview"
  | "source"
  | "retail"
  | "candidate"
  | "graph"
  | "attempts"
  | "experiments"
  | "prompt";

export type CloudColorMode =
  | "status"
  | "tier"
  | "readiness"
  | "milestone"
  | "buildable"
  | "claimed";

export type WorkflowKind = "new" | "improve";

export type JobStatus = "queued" | "running" | "completed" | "failed" | "cancelled" | string;

export interface FunctionSummary {
  id: string;
  display_name: string;
  symbol: string | null;
  address: string | null;
  size: number | null;
  source: string | null;
  unit: string | null;
  tier: string | null;
  milestone: string | null;
  status: string;
  workflow_status: string;
  instruction_percent: number | null;
  readiness: Readiness;
  difficulty: number | null;
  buildable: boolean;
  owner: string | null;
  kind?: string;
  region?: string;
  required_level?: string;
  claimed_at?: string | null;
  notes?: string;
  direct_call_count?: number;
  unresolved_call_count?: number;
  indirect_call?: boolean;
  instruction_count?: number;
  branch_count?: number;
  relocation_count?: number;
  stale?: boolean;
  artifact_fresh?: boolean | null;
}

export interface FunctionDetail extends FunctionSummary {
  capabilities?: string[];
  readiness_blockers?: string[];
  warnings?: string[];
  equivalence_status?: string | null;
  origin?: string | null;
}

export interface FunctionFilters {
  search?: string;
  region?: string;
  kind?: string;
  status?: string;
  workflow?: string;
  milestone?: string;
  tier?: string;
  capability?: string;
  unit?: string;
  source?: string;
  path?: string;
  owner?: string;
  readiness?: string;
  buildable?: boolean | null;
  indirect?: boolean | null;
  unresolved?: boolean | null;
  min_match?: number | null;
  max_match?: number | null;
  min_size?: number | null;
  max_size?: number | null;
  sort?: string;
}

export interface PaginatedFunctions {
  items: FunctionSummary[];
  total: number;
  page: number;
  page_size: number;
}

export interface ProjectInfo {
  name: string;
  region: string;
  function_count: number;
  stale: boolean;
  generated_at?: string | null;
  schema_version?: number;
  indexer_version?: string;
  warnings?: string[];
  accepted_count?: number;
  buildable_count?: number;
}

export interface UnitNode {
  path: string;
  name: string;
  kind?: "source" | "unit" | "folder" | string;
  function_count: number;
  children?: UnitNode[];
}

export interface ProjectionPoint {
  id: string;
  x: number;
  y: number;
  status?: string;
  tier?: string;
  readiness?: string;
  milestone?: string;
  buildable?: boolean;
  owner?: string | null;
  unit?: string | null;
  display_name?: string;
}

export interface ProjectionResponse {
  points: ProjectionPoint[];
  version?: string;
}

export interface EdgeRef {
  id: string;
  display_name: string;
  symbol?: string | null;
  kind?: string;
  status?: string;
  readiness?: string;
}

export interface Artifacts {
  source_cpp?: string | null;
  retail_ppc?: string | null;
  candidate_ppc?: string | null;
  relocations?: unknown;
  warnings?: string[];
  stale?: boolean;
  source_hash?: string | null;
  retail_object_hash?: string | null;
  candidate_object_hash?: string | null;
}

export interface Attempt {
  target_id: string;
  timestamp?: string | null;
  order?: number | null;
  status: string;
  match_percent?: number | null;
  hypothesis?: string | null;
  next_change?: string | null;
  runtime_test?: string | null;
  policy_exception?: boolean;
}

export interface Experiment {
  experiment_id: string;
  target_id: string;
  workflow: string;
  model?: string | null;
  model_id?: string | null;
  run_index?: number | null;
  timestamp?: string | null;
  duration_seconds?: number | null;
  input_tokens?: number | null;
  output_tokens?: number | null;
  cost?: number | null;
  status?: string | null;
  match_percent?: number | null;
  equivalence?: string | null;
  size_ok?: boolean | null;
  accepted?: boolean;
  winner?: boolean;
  error?: string | null;
  artifact?: string | null;
  hypothesis?: string | null;
}

export interface Neighbor {
  id: string;
  display_name: string;
  score: number;
  method?: string;
  rank?: number;
  status?: string;
}

export interface PromptPreviewResult {
  ok: boolean;
  prompt?: string;
  error?: string | null;
  bytes?: number;
  workflow?: string;
  target_id?: string;
}

export interface Job {
  id: string;
  target_id: string;
  workflow: string;
  status: JobStatus;
  started_at?: string | null;
  finished_at?: string | null;
  error?: string | null;
  log_path?: string | null;
}

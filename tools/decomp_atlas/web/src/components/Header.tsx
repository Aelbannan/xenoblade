import type { ProjectInfo } from "../types";

interface HeaderProps {
  project: ProjectInfo | null;
}

export function Header({ project }: HeaderProps) {
  return (
    <header className="panel app-header">
      <div>
        <h1>
          <span className="brand-accent">Decomp</span> Atlas
        </h1>
        {project?.stale ? (
          <div className="stale-banner">
            Index is stale relative to targets / artifacts. Re-run{" "}
            <code className="mono">python3 tools/decomp_atlas/run.py index</code>.
          </div>
        ) : null}
      </div>
      <div style={{ textAlign: "right" }}>
        <div style={{ color: "#fff", fontWeight: 600 }}>{project?.name ?? "…"}</div>
        <div className="meta">
          {project
            ? `${project.function_count.toLocaleString()} functions · ${project.region}`
            : "Loading project…"}
          {project?.accepted_count != null
            ? ` · ${project.accepted_count.toLocaleString()} accepted`
            : null}
        </div>
      </div>
    </header>
  );
}

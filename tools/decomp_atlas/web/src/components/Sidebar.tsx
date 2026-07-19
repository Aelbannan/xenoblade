import { useEffect, useMemo, useState } from "react";
import type { UnitNode } from "../types";

interface SidebarProps {
  units: UnitNode[];
  selectedPath: string | null;
  search: string;
  onSearchChange: (value: string) => void;
  onPathSelect: (path: string | null) => void;
  loading?: boolean;
  error?: string | null;
}

function matchesSearch(node: UnitNode, q: string): boolean {
  if (!q) return true;
  const needle = q.toLowerCase();
  if (node.path.toLowerCase().includes(needle) || node.name.toLowerCase().includes(needle)) {
    return true;
  }
  return (node.children ?? []).some((child) => matchesSearch(child, q));
}

function filterTree(nodes: UnitNode[], q: string): UnitNode[] {
  if (!q) return nodes;
  const out: UnitNode[] = [];
  for (const node of nodes) {
    if (!matchesSearch(node, q)) continue;
    out.push({ ...node, children: filterTree(node.children ?? [], q) });
  }
  return out;
}

function TreeNodeView({
  node,
  depth,
  selectedPath,
  onPathSelect,
}: {
  node: UnitNode;
  depth: number;
  selectedPath: string | null;
  onPathSelect: (path: string | null) => void;
}) {
  const hasChildren = (node.children?.length ?? 0) > 0;
  const containsSelected =
    selectedPath != null &&
    (selectedPath === node.path || selectedPath.startsWith(`${node.path}/`));
  const [expanded, setExpanded] = useState(depth < 1 || containsSelected);

  useEffect(() => {
    if (containsSelected) setExpanded(true);
  }, [containsSelected]);

  const selected = selectedPath === node.path;

  return (
    <div>
      <div className={`tree-row ${selected ? "selected" : ""}`} style={{ paddingLeft: depth * 12 + 4 }}>
        {hasChildren ? (
          <button
            type="button"
            className="tree-toggle"
            aria-label={expanded ? "Collapse" : "Expand"}
            onClick={() => setExpanded((v) => !v)}
          >
            {expanded ? "▾" : "▸"}
          </button>
        ) : (
          <span className="tree-toggle" />
        )}
        <button
          type="button"
          style={{
            flex: 1,
            textAlign: "left",
            background: "none",
            border: "none",
            color: "inherit",
            fontWeight: containsSelected ? 600 : 400,
            overflow: "hidden",
            textOverflow: "ellipsis",
            whiteSpace: "nowrap",
          }}
          onClick={() => onPathSelect(selected ? null : node.path)}
          title={node.path}
        >
          {node.name || node.path}
        </button>
        <span className="count">{node.function_count}</span>
      </div>
      {expanded && hasChildren
        ? node.children!.map((child) => (
            <TreeNodeView
              key={child.path}
              node={child}
              depth={depth + 1}
              selectedPath={selectedPath}
              onPathSelect={onPathSelect}
            />
          ))
        : null}
    </div>
  );
}

export function Sidebar({
  units,
  selectedPath,
  search,
  onSearchChange,
  onPathSelect,
  loading,
  error,
}: SidebarProps) {
  const filtered = useMemo(() => filterTree(units, search), [units, search]);

  return (
    <aside className="panel sidebar">
      <div className="sidebar-search">
        <input
          className="search-input"
          type="search"
          placeholder="Search name / symbol / id / addr"
          value={search}
          onChange={(e) => onSearchChange(e.target.value)}
          aria-label="Search functions"
        />
        {selectedPath ? (
          <button
            type="button"
            className="btn btn-ghost"
            style={{ marginTop: "0.5rem", width: "100%" }}
            onClick={() => onPathSelect(null)}
          >
            Clear path filter
          </button>
        ) : null}
      </div>
      <div className="sidebar-tree">
        {loading ? <div className="loading-block">Loading units…</div> : null}
        {error ? <div className="error-text" style={{ padding: "0.75rem" }}>{error}</div> : null}
        {!loading && !error && filtered.length === 0 ? (
          <div className="empty-state">No units</div>
        ) : null}
        {filtered.map((node) => (
          <TreeNodeView
            key={node.path}
            node={node}
            depth={0}
            selectedPath={selectedPath}
            onPathSelect={onPathSelect}
          />
        ))}
      </div>
    </aside>
  );
}

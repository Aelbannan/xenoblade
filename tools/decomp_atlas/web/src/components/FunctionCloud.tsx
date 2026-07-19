import { useEffect, useMemo, useRef, useState } from "react";
import { getCallees, getCallers, getProjection } from "../api";
import type { CloudColorMode, EdgeRef, ProjectionPoint } from "../types";

interface FunctionCloudProps {
  selectedId: string | null;
  pathFilter: string | null;
  onSelect: (id: string) => void;
}

const COLOR_MODES: { id: CloudColorMode; label: string }[] = [
  { id: "status", label: "Status" },
  { id: "tier", label: "Tier" },
  { id: "readiness", label: "Readiness" },
  { id: "milestone", label: "Milestone" },
  { id: "buildable", label: "Buildable" },
  { id: "claimed", label: "Claimed" },
];

const PALETTE = [
  "#22d3ee",
  "#34d399",
  "#fbbf24",
  "#fb7185",
  "#a78bfa",
  "#60a5fa",
  "#f472b6",
  "#94a3b8",
];

function colorFor(point: ProjectionPoint, mode: CloudColorMode, keyIndex: Map<string, number>): string {
  let key = "unknown";
  switch (mode) {
    case "status":
      key = (point.status ?? "unknown").toLowerCase();
      break;
    case "tier":
      key = (point.tier ?? "none").toLowerCase();
      break;
    case "readiness":
      key = (point.readiness ?? "unknown").toLowerCase();
      break;
    case "milestone":
      key = (point.milestone ?? "none").toLowerCase();
      break;
    case "buildable":
      key = point.buildable ? "buildable" : "unbuildable";
      break;
    case "claimed":
      key = point.owner ? "claimed" : "unclaimed";
      break;
  }
  if (!keyIndex.has(key)) keyIndex.set(key, keyIndex.size);
  return PALETTE[keyIndex.get(key)! % PALETTE.length];
}

function pointInPath(point: ProjectionPoint, path: string | null): boolean {
  if (!path) return true;
  const unit = point.unit ?? "";
  return unit === path || unit.startsWith(`${path}/`);
}

export function FunctionCloud({ selectedId, pathFilter, onSelect }: FunctionCloudProps) {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [points, setPoints] = useState<ProjectionPoint[]>([]);
  const [version, setVersion] = useState<string | undefined>();
  const [mode, setMode] = useState<CloudColorMode>("status");
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [callers, setCallers] = useState<EdgeRef[]>([]);
  const [callees, setCallees] = useState<EdgeRef[]>([]);
  const [hoverId, setHoverId] = useState<string | null>(null);

  useEffect(() => {
    let cancelled = false;
    setLoading(true);
    setError(null);
    getProjection()
      .then((res) => {
        if (cancelled) return;
        setPoints(res.points ?? []);
        setVersion(res.version);
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
  }, []);

  useEffect(() => {
    if (!selectedId) {
      setCallers([]);
      setCallees([]);
      return;
    }
    let cancelled = false;
    Promise.all([getCallers(selectedId), getCallees(selectedId)])
      .then(([cIn, cOut]) => {
        if (cancelled) return;
        setCallers(cIn);
        setCallees(cOut);
      })
      .catch(() => {
        if (!cancelled) {
          setCallers([]);
          setCallees([]);
        }
      });
    return () => {
      cancelled = true;
    };
  }, [selectedId]);

  const visible = useMemo(
    () => points.filter((p) => pointInPath(p, pathFilter)),
    [points, pathFilter],
  );

  const byId = useMemo(() => {
    const m = new Map<string, ProjectionPoint>();
    for (const p of visible) m.set(p.id, p);
    return m;
  }, [visible]);

  const legendKeys = useMemo(() => {
    const keyIndex = new Map<string, number>();
    for (const p of visible) colorFor(p, mode, keyIndex);
    return [...keyIndex.entries()].map(([key, idx]) => ({
      key,
      color: PALETTE[idx % PALETTE.length],
    }));
  }, [visible, mode]);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const parent = canvas.parentElement;
    if (!parent) return;

    const dpr = window.devicePixelRatio || 1;
    const width = parent.clientWidth;
    const height = parent.clientHeight;
    canvas.width = Math.floor(width * dpr);
    canvas.height = Math.floor(height * dpr);
    canvas.style.width = `${width}px`;
    canvas.style.height = `${height}px`;

    const ctx = canvas.getContext("2d");
    if (!ctx) return;
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    ctx.clearRect(0, 0, width, height);

    if (visible.length === 0) {
      ctx.fillStyle = "#64748b";
      ctx.font = "14px sans-serif";
      ctx.fillText(loading ? "Loading projection…" : "No projection points", 16, 28);
      return;
    }

    let minX = Infinity;
    let maxX = -Infinity;
    let minY = Infinity;
    let maxY = -Infinity;
    for (const p of visible) {
      minX = Math.min(minX, p.x);
      maxX = Math.max(maxX, p.x);
      minY = Math.min(minY, p.y);
      maxY = Math.max(maxY, p.y);
    }
    const pad = 24;
    const spanX = maxX - minX || 1;
    const spanY = maxY - minY || 1;

    const project = (p: ProjectionPoint) => ({
      x: pad + ((p.x - minX) / spanX) * (width - pad * 2),
      y: pad + ((p.y - minY) / spanY) * (height - pad * 2),
    });

    const keyIndex = new Map<string, number>();
    const callerIds = new Set(callers.map((e) => e.id));
    const calleeIds = new Set(callees.map((e) => e.id));
    const selected = selectedId ? byId.get(selectedId) : undefined;

    if (selected) {
      const sel = project(selected);
      ctx.lineWidth = 1.25;
      for (const edge of callers) {
        const other = byId.get(edge.id);
        if (!other) continue;
        const o = project(other);
        ctx.strokeStyle = "rgba(167, 139, 250, 0.55)";
        ctx.beginPath();
        ctx.moveTo(sel.x, sel.y);
        ctx.lineTo(o.x, o.y);
        ctx.stroke();
      }
      for (const edge of callees) {
        const other = byId.get(edge.id);
        if (!other) continue;
        const o = project(other);
        ctx.strokeStyle = "rgba(34, 211, 238, 0.55)";
        ctx.beginPath();
        ctx.moveTo(sel.x, sel.y);
        ctx.lineTo(o.x, o.y);
        ctx.stroke();
      }
    }

    for (const p of visible) {
      const { x, y } = project(p);
      const isSelected = p.id === selectedId;
      const isHover = p.id === hoverId;
      const isCaller = callerIds.has(p.id);
      const isCallee = calleeIds.has(p.id);
      let radius = isSelected || isHover ? 5.5 : 2.4;
      if (isCaller || isCallee) radius = Math.max(radius, 4);

      ctx.beginPath();
      ctx.fillStyle = isSelected
        ? "#ffffff"
        : isCaller
          ? "#a78bfa"
          : isCallee
            ? "#22d3ee"
            : colorFor(p, mode, keyIndex);
      ctx.globalAlpha = selectedId && !isSelected && !isCaller && !isCallee ? 0.35 : 0.9;
      ctx.arc(x, y, radius, 0, Math.PI * 2);
      ctx.fill();
      ctx.globalAlpha = 1;
    }

    // store projected coords for hit testing on the canvas element
    (canvas as HTMLCanvasElement & { __hit?: { id: string; x: number; y: number }[] }).__hit =
      visible.map((p) => {
        const pt = project(p);
        return { id: p.id, x: pt.x, y: pt.y };
      });
  }, [visible, mode, selectedId, callers, callees, byId, hoverId, loading]);

  const findHit = (clientX: number, clientY: number): string | null => {
    const canvas = canvasRef.current;
    if (!canvas) return null;
    const rect = canvas.getBoundingClientRect();
    const x = clientX - rect.left;
    const y = clientY - rect.top;
    const hits =
      (canvas as HTMLCanvasElement & { __hit?: { id: string; x: number; y: number }[] }).__hit ??
      [];
    let best: { id: string; d: number } | null = null;
    for (const h of hits) {
      const d = (h.x - x) ** 2 + (h.y - y) ** 2;
      if (d <= 10 * 10 && (!best || d < best.d)) best = { id: h.id, d };
    }
    return best?.id ?? null;
  };

  return (
    <div className="panel">
      <div className="panel-header">
        <div>
          <strong>Function cloud</strong>
          <div className="muted" style={{ fontSize: "0.8rem" }}>
            Precomputed projection{version ? ` · ${version}` : ""} · {visible.length.toLocaleString()}{" "}
            points
          </div>
        </div>
        <select
          className="select-input"
          style={{ width: "auto", minWidth: 140 }}
          value={mode}
          onChange={(e) => setMode(e.target.value as CloudColorMode)}
          aria-label="Color mode"
        >
          {COLOR_MODES.map((m) => (
            <option key={m.id} value={m.id}>
              {m.label}
            </option>
          ))}
        </select>
      </div>
      <div className="panel-body">
        {error ? <div className="error-text">{error}</div> : null}
        <div className="cloud-canvas-wrap">
          <canvas
            ref={canvasRef}
            onMouseMove={(e) => setHoverId(findHit(e.clientX, e.clientY))}
            onMouseLeave={() => setHoverId(null)}
            onClick={(e) => {
              const id = findHit(e.clientX, e.clientY);
              if (id) onSelect(id);
            }}
          />
        </div>
        {hoverId ? (
          <div className="muted" style={{ marginTop: "0.5rem", fontFamily: "var(--font-mono)" }}>
            {hoverId}
            {byId.get(hoverId)?.display_name ? ` · ${byId.get(hoverId)!.display_name}` : ""}
          </div>
        ) : null}
        <div className="cloud-legend">
          {legendKeys.slice(0, 12).map((item) => (
            <span key={item.key}>
              <span className="legend-swatch" style={{ background: item.color }} />
              {item.key}
            </span>
          ))}
          {selectedId ? (
            <>
              <span>
                <span className="legend-swatch" style={{ background: "#a78bfa" }} />
                callers
              </span>
              <span>
                <span className="legend-swatch" style={{ background: "#22d3ee" }} />
                callees
              </span>
            </>
          ) : null}
        </div>
      </div>
    </div>
  );
}

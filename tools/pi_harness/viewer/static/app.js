"use strict";
/* pi-harness viewer — vanilla JS, no frameworks, 2s polling.
   Design language borrowed from tangOS (aero glass, treemap atlas,
   segmented controls, status dots). Same read-only API as before. */

// Older browsers lack ctx.roundRect; the atlas needs it.
if (typeof CanvasRenderingContext2D !== "undefined" &&
    !CanvasRenderingContext2D.prototype.roundRect) {
  CanvasRenderingContext2D.prototype.roundRect = function (x, y, w, h, r) {
    r = Math.min(r, w / 2, h / 2);
    this.moveTo(x + r, y);
    this.arcTo(x + w, y, x + w, y + h, r);
    this.arcTo(x + w, y + h, x, y + h, r);
    this.arcTo(x, y + h, x, y, r);
    this.arcTo(x, y, x + w, y, r);
    this.closePath();
    return this;
  };
}

const $ = (sel) => document.querySelector(sel);

const state = {
  view: "dashboard",
  tu: "",
  transcripts: [],
  tuData: null,
  tuSearch: "",
  tuFilter: "all",
  tuSort: "id",
  expanded: new Set(),
  nearmissCache: new Map(),
  logFilter: null,
  logSearch: "",
  logEvents: [],
  atlasSize: "count",
  atlasData: null,
  atlasHover: null,
  busy: false,
  lastPoll: null,
};

const THEMES = ["aero", "deepsea", "sunset", "bubblegum", "lemonlime"];

/* ---------- formatting ---------- */

const fmtTokens = (n) =>
  n >= 1e9 ? (n / 1e9).toFixed(2) + "B" :
  n >= 1e6 ? (n / 1e6).toFixed(1) + "M" :
  n >= 1e3 ? (n / 1e3).toFixed(1) + "k" : String(n ?? 0);

const fmtInt = (n) => (n ?? 0).toLocaleString();

const fmtMoney = (n) =>
  n === null || n === undefined ? "—" :
  n >= 100 ? "$" + n.toFixed(0) :
  n >= 1 ? "$" + n.toFixed(2) : "$" + n.toFixed(4);

const fmtTime = (ts) => {
  if (!ts) return "—";
  const d = new Date(ts);
  return d.toLocaleTimeString() + " " + d.toLocaleDateString();
};

const fmtTimeShort = (ts) => {
  if (!ts) return "—";
  const d = new Date(ts);
  return d.toLocaleTimeString();
};

const esc = (s) =>
  String(s ?? "").replace(/[&<>"']/g, (c) =>
    ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;" }[c]));

const el = (tag, cls, text) => {
  const e = document.createElement(tag);
  if (cls) e.className = cls;
  if (text !== undefined) e.textContent = text;
  return e;
};

const STATUS_CLASS = {
  "FULL_MATCH": "ok", "EQUIVALENT_MATCH": "ok", "ACCEPTED": "ok",
  "HIGH_MATCH": "warn", "CODE_MATCH": "warn",
  "STRUCTURAL": "info", "COMPILES": "info", "ACTIVE": "info",
  "NOT_STARTED": "dim",
  "STALLED": "bad", "STALL": "bad", "BLOCKED": "bad",
  "STALLED_BLOCKED_EXTERNAL": "bad",
};

const EVENT_CLASS = {
  "session-usage": "ev-usage", "batch-cycle": "ev-ok", "batch-accept": "ev-ok",
  "batch-rejected": "ev-warn", "target-skipped": "ev-dim",
  "batch-session-exhausted": "ev-warn", "batch-error": "ev-bad",
  "tu-started": "ev-info", "tu-incomplete": "ev-dim", "tu-final-failed": "ev-bad",
};

/* ---------- fetch helpers ---------- */

async function api(path) {
  const r = await fetch(path, { cache: "no-store" });
  if (!r.ok) {
    let msg = r.status + " " + path;
    try { msg = (await r.json()).error || msg; } catch (_) { /* ignore */ }
    throw new Error(msg);
  }
  return r.json();
}

let lastError = null;
function flash(msg) {
  lastError = msg;
  const t = $("#toast");
  t.textContent = msg;
  t.style.display = "block";
  clearTimeout(t._h);
  t._h = setTimeout(() => { t.style.display = "none"; }, 6000);
}

function liveChip(msg, ok) {
  const chip = $("#live-chip");
  $("#live-text").textContent = msg;
  chip.classList.toggle("ok", ok === true);
  chip.classList.toggle("err", ok === false);
}

/* ---------- theme ---------- */

function initTheme() {
  let theme = "deepsea";
  try { theme = localStorage.getItem("pi-harness-theme") || theme; } catch (_) { /* ignore */ }
  if (!THEMES.includes(theme)) theme = "deepsea";
  document.documentElement.dataset.theme = theme;
  const box = $("#themes");
  box.innerHTML = "";
  THEMES.forEach((t) => {
    const b = el("button", "t " + t);
    b.dataset.theme = t;
    b.title = t + " theme";
    b.classList.toggle("on", t === theme);
    b.onclick = () => {
      document.documentElement.dataset.theme = t;
      box.querySelectorAll("button").forEach((x) => x.classList.toggle("on", x.dataset.theme === t));
      try { localStorage.setItem("pi-harness-theme", t); } catch (_) { /* ignore */ }
      if (state.view === "atlas") drawAtlas();
    };
    box.appendChild(b);
  });
}

/* ---------- view switching ---------- */

function switchView(name) {
  state.view = name;
  document.querySelectorAll("#nav button").forEach((b) =>
    b.classList.toggle("on", b.dataset.view === name));
  document.querySelectorAll("main section").forEach((s) =>
    s.classList.toggle("active", s.id === "view-" + name));
  if (name === "dashboard") renderDashboard();
  else if (name === "atlas") renderAtlas();
  else if (name === "cost") renderCost();
  else if (name === "log") renderLog();
  else if (name === "settings") renderSettings();
  else if (name === "tu") { if (state.tu) loadTu(); }
}

$("#nav").addEventListener("click", (ev) => {
  const b = ev.target.closest("button");
  if (b) switchView(b.dataset.view);
});

/* ---------- dashboard ---------- */

async function renderDashboard() {
  try {
    const o = await api("/api/overview");
    state.lastPoll = new Date();
    $("#d-sub").textContent =
      `live TU progress · generated ${fmtTime(o.generatedAt)} · last poll ${fmtTime(state.lastPoll.toISOString())}`;
    $("#d-tus").textContent = o.liveTuCount;
    $("#d-running").textContent = o.runningTuCount;
    $("#d-accepted").textContent = fmtInt(o.totals.accepted);
    $("#d-skipped").textContent = fmtInt(o.totals.skipped);
    $("#d-exhausted").textContent = fmtInt(o.totals.exhausted);
    const tk = o.ticker;
    $("#d-coin").textContent = tk.priced ? "$/match" : "tok/match";
    $("#d-permatch").textContent = tk.priced
      ? fmtMoney(tk.perMatch)
      : fmtTokens(tk.perMatch) + " tok/match";
    $("#d-coin2").textContent = tk.priced ? "Total cost" : "Total tokens";
    $("#d-totalcost").textContent = tk.priced
      ? fmtMoney(tk.cost)
      : fmtTokens(tk.tokens) + " tok";
    liveChip(`live · ${fmtInt(o.sessionCount)} sessions`, true);

    const box = $("#d-tulist");
    box.innerHTML = "";
    if (!o.tus.length) box.appendChild(el("div", "empty", "No TU activity in the ledger yet."));
    else o.tus.slice(0, 60).forEach((t) => box.appendChild(tuRow(o, t)));

    const as = $("#d-sessions");
    as.innerHTML = "";
    if (!o.activeSessions.length) as.appendChild(el("div", "empty", "No sessions yet."));
    o.activeSessions.slice().reverse().forEach((s) => as.appendChild(sessionRow(s)));
  } catch (err) {
    liveChip("poll failed", false);
    flash("dashboard: " + err.message);
  }
}

function tuRow(o, t) {
  const wrap = el("div", "aero-panel turow");
  const top = el("div", "turow-top");
  top.appendChild(el("span", "status-dot " + (t.running ? "running" : "done")));
  const name = el("a", "tu-name", t.tu);
  name.href = "#";
  name.title = t.tu;
  name.onclick = (ev) => {
    ev.preventDefault();
    openTu(t.tu);
  };
  top.appendChild(name);
  top.appendChild(el("span", "chip " + (t.running ? "run" : "done"),
    t.running ? "running" : (t.lastEvent || "ended")));
  wrap.appendChild(top);
  const meta = el("div", "tu-meta",
    `${t.total} total · ${t.accepted} accepted · ${t.skipped} skipped · ${t.exhausted} exhausted` +
    (t.lastActivity ? " · last " + fmtTime(t.lastActivity) : ""));
  wrap.appendChild(meta);
  const bar = el("div", "bar");
  const pct = t.total ? Math.round((100 * t.accepted) / t.total) : 0;
  const skipPct = t.total ? Math.round((100 * (t.skipped + t.exhausted)) / t.total) : 0;
  const fill = el("div", "fill");
  fill.style.width = pct + "%";
  fill.title = `${t.accepted} accepted (${pct}%)`;
  const fill2 = el("div", "fill skip");
  fill2.style.width = skipPct + "%";
  fill2.title = `${t.skipped + t.exhausted} skipped/exhausted`;
  bar.appendChild(fill);
  bar.appendChild(fill2);
  wrap.appendChild(bar);
  wrap.onclick = (ev) => { if (!ev.target.closest("a")) openTu(t.tu); };
  return wrap;
}

function sessionRow(s) {
  const row = el("div", "session-row");
  const when = el("span", "log-time", fmtTime(s.ts));
  const tu = el("span", "tu", s.tu);
  tu.title = "open in TU detail";
  tu.onclick = () => openTu(s.tu);
  const label = el("span", "mono", s.label || "");
  const toks = el("span", "num", fmtTokens(s.tokens) + " tok");
  row.append(when, tu, label, toks);
  return row;
}

function openTu(unit) {
  state.tu = unit;
  $("#tu-input").value = unit;
  switchView("tu");
  loadTu();
}

/* ===================== atlas (treemap) ===================== */

async function renderAtlas() {
  try {
    const o = await api("/api/overview");
    state.atlasData = o;
    liveChip(`live · ${fmtInt(o.sessionCount)} sessions`, true);
    $("#atlas-gen").textContent = `generated ${fmtTime(o.generatedAt)} · last poll ${fmtTime(new Date().toISOString())}`;
    drawAtlas();
  } catch (err) {
    liveChip("poll failed", false);
    flash("atlas: " + err.message);
  }
}

/* Bruls et al. squarified treemap — compact vanilla implementation. */
function treemap(items, x, y, w, h) {
  const total = items.reduce((s, c) => s + c.value, 0);
  if (total <= 0 || w <= 0 || h <= 0) {
    return items.map((c) => ({ ...c, x, y, w: 0, h: 0 }));
  }
  const scaled = items.map((c) => ({ ...c, v: (c.value * w * h) / total }));
  const out = [];
  const worst = (row, len, thick) => {
    const s = row.reduce((a, b) => a + b, 0);
    const max = Math.max(...row), min = Math.min(...row);
    const s2 = s * s;
    return Math.max((thick * thick * max) / s2, s2 / (thick * thick * min));
  };
  let i = 0, rx = x, ry = y, rw = w, rh = h;
  while (i < scaled.length) {
    const thick = Math.min(rw, rh);
    const len = Math.max(rw, rh);
    let row = [scaled[i].v];
    i++;
    while (i < scaled.length) {
      const next = scaled[i].v;
      if (worst([...row, next], len, thick) <= worst(row, len, thick)) { row.push(next); i++; }
      else break;
    }
    const s = row.reduce((a, b) => a + b, 0);
    if (rw >= rh) {
      const stripW = s / rh;
      let cy = ry;
      row.forEach((v, k) => {
        const it = scaled[i - row.length + k];
        out.push({ ...it, x: rx, y: cy, w: stripW, h: (v / s) * rh });
        cy += (v / s) * rh;
      });
      rx += stripW; rw -= stripW;
    } else {
      const stripH = s / rw;
      let cx = rx;
      row.forEach((v, k) => {
        const it = scaled[i - row.length + k];
        out.push({ ...it, x: cx, y: ry, w: (v / s) * rw, h: stripH });
        cx += (v / s) * rw;
      });
      ry += stripH; rh -= stripH;
    }
  }
  return out;
}

function matchColor(pct) {
  const hue = Math.round(120 * Math.max(0, Math.min(1, pct)));
  return `hsl(${hue} 62% 42%)`;
}

function drawAtlas() {
  const o = state.atlasData;
  const canvas = $("#atlas-canvas");
  const wrap = $("#atlas-wrap");
  const tip = $("#atlas-tip");
  const empty = $("#atlas-empty");
  if (!o) return;

  const dpr = window.devicePixelRatio || 1;
  const cw = wrap.clientWidth, ch = wrap.clientHeight;
  canvas.width = Math.max(1, Math.round(cw * dpr));
  canvas.height = Math.max(1, Math.round(ch * dpr));
  const ctx = canvas.getContext("2d");
  ctx.setTransform(dpr, 0, 0, dpr, 0, 0);

  const tus = (o.tus || []).filter((t) => (t.total || 0) > 0);
  if (!tus.length) {
    empty.style.display = "grid";
    empty.textContent = "No TU activity in the ledger yet.";
    ctx.clearRect(0, 0, cw, ch);
    tip.style.display = "none";
    return;
  }
  empty.style.display = "none";

  const useBytes = state.atlasSize === "bytes" && tus.some((t) => (t.totalBytes || 0) > 0);
  const items = tus.map((t) => ({
    tu: t,
    value: useBytes ? (t.totalBytes || 0) : t.total,
  }));

  const rects = treemap(items, 8, 8, Math.max(1, cw - 16), Math.max(1, ch - 16));

  const tileText = getComputedStyle(document.documentElement).getPropertyValue("--atlas-tile-text").trim() || "#fff";
  ctx.textBaseline = "middle";
  ctx.font = "10.5px ui-monospace, SFMono-Regular, Menlo, monospace";

  let hovered = null;
  const px = state.atlasHover ? state.atlasHover.x : null;
  const py = state.atlasHover ? state.atlasHover.y : null;

  rects.forEach((r) => {
    const t = r.tu;
    const total = t.total || 1;
    const pct = Math.max(0, Math.min(1, t.accepted / total));
    const skipShare = Math.max(0, Math.min(1, (t.skipped + t.exhausted) / total));
    const isHover = px !== null && px >= r.x && px < r.x + r.w && py >= r.y && py < r.y + r.h;

    ctx.fillStyle = matchColor(pct);
    ctx.beginPath();
    ctx.roundRect(r.x, r.y, r.w, r.h, 4);
    ctx.fill();

    // inner progress strip: accepted (light) then skipped/exhausted (gray)
    const stripH = Math.max(3, Math.min(6, r.h * 0.12));
    if (r.h > 16) {
      ctx.fillStyle = "rgba(255,255,255,0.30)";
      ctx.fillRect(r.x + 3, r.y + r.h - stripH - 2, Math.max(0, (r.w - 6) * pct), stripH - 2);
      if (skipShare > 0) {
        ctx.fillStyle = "rgba(150,160,175,0.75)";
        const sx = r.x + 3 + (r.w - 6) * pct;
        ctx.fillRect(sx, r.y + r.h - stripH - 2, Math.max(0, (r.w - 6) * skipShare), stripH - 2);
      }
    }

    if (isHover) {
      ctx.strokeStyle = "#ffffff";
      ctx.lineWidth = 2;
      ctx.beginPath();
      ctx.roundRect(r.x + 1, r.y + 1, r.w - 2, r.h - 2, 4);
      ctx.stroke();
      hovered = r;
    }

    // label
    if (r.w > 66 && r.h > 26) {
      const short = t.tu.includes("/") ? t.tu.split("/").pop() : t.tu;
      ctx.fillStyle = tileText;
      ctx.fillText(short, r.x + 7, r.y + r.h / 2 - 7, r.w - 14);
      ctx.font = "9px ui-monospace, Menlo, monospace";
      ctx.fillText(Math.round(pct * 100) + "% · " + t.accepted + "/" + total,
        r.x + 7, r.y + r.h / 2 + 8, r.w - 14);
      ctx.font = "10.5px ui-monospace, SFMono-Regular, Menlo, monospace";
    } else if (r.w > 30 && r.h > 14) {
      ctx.fillStyle = tileText;
      ctx.fillText(t.tu.includes("/") ? t.tu.split("/").pop() : t.tu, r.x + 5, r.y + r.h / 2, r.w - 10);
    }
  });

  // tooltip
  if (hovered) {
    const t = hovered.tu;
    const pct = t.total ? Math.round((100 * t.accepted) / t.total) : 0;
    const bytesLine = (t.totalBytes || 0) > 0
      ? `code ${fmtInt(t.acceptedBytes)} / ${fmtInt(t.totalBytes)} B (${t.totalBytes ? Math.round(100 * t.acceptedBytes / t.totalBytes) : 0}%)`
      : null;
    tip.innerHTML =
      `<b>${esc(t.tu)}</b> · ${t.running ? "running" : (t.lastEvent || "ended")}<br>` +
      `${fmtInt(t.accepted)} / ${fmtInt(t.total)} accepted (${pct}%)<br>` +
      `${fmtInt(t.skipped)} skipped · ${fmtInt(t.exhausted)} exhausted` +
      (bytesLine ? "<br>" + esc(bytesLine) : "") +
      (t.lastActivity ? "<br>last " + esc(fmtTime(t.lastActivity)) : "");
    tip.style.display = "block";
    // keep the tip inside the wrap
    const tw = tip.offsetWidth, th = tip.offsetHeight;
    let tx = hovered.x + hovered.w + 10;
    if (tx + tw > cw - 6) tx = hovered.x - tw - 10;
    let ty = hovered.y + 10;
    if (ty + th > ch - 6) ty = hovered.y - th - 10;
    tip.style.left = Math.max(6, tx) + "px";
    tip.style.top = Math.max(6, ty) + "px";
  } else {
    tip.style.display = "none";
  }

  $("#atlas-hint").textContent =
    `${rects.length} TU tiles · sized by ${useBytes ? "code bytes" : "function count"}`;
}

function bindAtlasPointer() {
  const canvas = $("#atlas-canvas");
  canvas.addEventListener("mousemove", (ev) => {
    const r = canvas.getBoundingClientRect();
    state.atlasHover = { x: ev.clientX - r.left, y: ev.clientY - r.top };
    drawAtlas();
  });
  canvas.addEventListener("mouseleave", () => {
    state.atlasHover = null;
    drawAtlas();
  });
  canvas.addEventListener("click", (ev) => {
    const o = state.atlasData;
    if (!o) return;
    const r = canvas.getBoundingClientRect();
    const mx = ev.clientX - r.left, my = ev.clientY - r.top;
    const tus = (o.tus || []).filter((t) => (t.total || 0) > 0);
    if (!tus.length) return;
    const useBytes = state.atlasSize === "bytes" && tus.some((t) => (t.totalBytes || 0) > 0);
    const rects = treemap(tus.map((t) => ({
      tu: t, value: useBytes ? (t.totalBytes || 0) : t.total,
    })), 8, 8, Math.max(1, r.width - 16), Math.max(1, r.height - 16));
    const hit = rects.find((q) => mx >= q.x && mx < q.x + q.w && my >= q.y && my < q.y + q.h);
    if (hit) openTu(hit.tu.tu);
  });
  $("#atlas-size-seg").addEventListener("click", (ev) => {
    const b = ev.target.closest("button");
    if (!b) return;
    state.atlasSize = b.dataset.size;
    $("#atlas-size-seg button").forEach((x) => x.classList.toggle("on", x === b));
    drawAtlas();
  });
  if (window.ResizeObserver) {
    new ResizeObserver(() => { if (state.view === "atlas") drawAtlas(); }).observe($("#atlas-wrap"));
  }
}

/* ---------- TU detail ---------- */

async function loadTu() {
  const unit = state.tu.trim();
  if (!unit || state.busy) return;
  state.busy = true;
  $("#tu-status").textContent = "loading…";
  try {
    const [d, tr] = await Promise.all([
      api("/api/tu/" + encodeURIComponent(unit)),
      api("/api/transcripts?tu=" + encodeURIComponent(unit)),
    ]);
    if (d.error) throw new Error(d.error);
    state.transcripts = tr;
    state.tuData = d.targets;
    state.expanded = new Set();
    renderTranscripts(tr);
    $("#tu-toolbar").style.display = "flex";
    renderTuFilters();
    applyTuFilters();
    $("#tu-status").textContent =
      `${d.targets.length} targets` + (d.transcriptCount ? ` · ${d.transcriptCount} transcripts` : "");
  } catch (err) {
    $("#tu-status").textContent = "error: " + err.message;
    $("#tu-toolbar").style.display = "none";
  } finally {
    state.busy = false;
  }
}

function renderTuFilters() {
  const box = $("#tu-filters");
  box.innerHTML = "";
  const mk = (label, val) => {
    const b = el("button", state.tuFilter === val ? "active" : "", label);
    b.onclick = () => { state.tuFilter = val; renderTuFilters(); applyTuFilters(); };
    box.appendChild(b);
  };
  mk("all", "all");
  mk("ok", "ok"); mk("warn", "warn"); mk("info", "info");
  mk("bad", "bad"); mk("dim", "dim"); mk("skipped", "skipped");
}

function filteredTargets() {
  let rows = state.tuData || [];
  const q = state.tuSearch.trim().toLowerCase();
  if (q) {
    rows = rows.filter((t) =>
      (t.id || "").toLowerCase().includes(q) ||
      (t.symbol || "").toLowerCase().includes(q) ||
      (t.function || "").toLowerCase().includes(q));
  }
  if (state.tuFilter === "skipped") rows = rows.filter((t) => t.skipped);
  else if (state.tuFilter !== "all") {
    rows = rows.filter((t) => (STATUS_CLASS[t.status] || "dim") === state.tuFilter);
  }
  const sort = state.tuSort;
  rows = rows.slice().sort((a, b) => {
    if (sort === "size") return (b.sizeBytes || 0) - (a.sizeBytes || 0);
    if (sort === "attempts") return (b.attempts || 0) - (a.attempts || 0);
    if (sort === "div") {
      const ad = a.bestDivergence == null ? Infinity : a.bestDivergence;
      const bd = b.bestDivergence == null ? Infinity : b.bestDivergence;
      return ad - bd;
    }
    return (a.id || "").localeCompare(b.id || "");
  });
  return rows;
}

function applyTuFilters() {
  const box = $("#tu-table");
  const rows = filteredTargets();
  box.innerHTML = "";
  const total = (state.tuData || []).length;
  $("#tu-count").textContent = `showing ${rows.length} of ${total}`;
  if (!rows.length) {
    box.appendChild(el("div", "empty", total ? "No targets match the current filter." : "No targets for this unit in the ledger or registry."));
    return;
  }
  const table = el("table");
  const thead = el("thead");
  const hr = el("tr");
  ["", "ID", "Function", "Status", "Size", "Attempts", "Best div", "Draft"].forEach((h) => {
    const num = h === "Attempts" || h === "Size" || h === "Best div";
    hr.appendChild(el("th", num ? "num" : "", h));
  });
  thead.appendChild(hr);
  const tbody = el("tbody");
  rows.forEach((t) => {
    const tr = el("tr", "clickable");
    tr.appendChild(el("td", "", state.expanded.has(t.id) ? "▾" : "▸"));
    tr.appendChild(el("td", "mono", t.id));
    const fn = el("td", "", t.function || t.symbol || "");
    fn.title = t.source || "";
    tr.appendChild(fn);
    const st = el("td");
    st.appendChild(el("span", "chip " + (STATUS_CLASS[t.status] || "dim"), t.status || "—"));
    if (t.skipped) st.appendChild(el("span", "chip dim", "skipped"));
    tr.appendChild(st);
    tr.appendChild(el("td", "num mono", t.size || "—"));
    tr.appendChild(el("td", "num", String(t.attempts ?? 0)));
    tr.appendChild(el("td", "num", t.bestDivergence != null ? String(t.bestDivergence) : "—"));
    const badge = el("td");
    badge.appendChild(el("span", "chip " + (t.nearmissBanked ? "warn" : "dim"),
      t.nearmissBanked ? "draft" : "none"));
    tr.appendChild(badge);
    tr.onclick = () => toggleTargetDetail(t.id);
    tbody.appendChild(tr);
  });
  table.appendChild(thead);
  table.appendChild(tbody);
  box.appendChild(table);
  // re-render expanded detail rows after their anchor rows
  rows.forEach((t) => {
    if (state.expanded.has(t.id)) appendTargetDetail(tbody, t);
  });
}

async function toggleTargetDetail(id) {
  if (state.expanded.has(id)) {
    state.expanded.delete(id);
  } else {
    state.expanded.add(id);
    if (!state.nearmissCache.has(id)) {
      try { state.nearmissCache.set(id, await api("/api/nearmiss/" + encodeURIComponent(id))); }
      catch (_) { state.nearmissCache.set(id, { banked: false, error: true }); }
    }
  }
  applyTuFilters();
}

function appendTargetDetail(tbody, t) {
  const tr = el("tr", "detail-row");
  const td = el("td");
  td.colSpan = 8;
  const box = el("div", "detail-box");
  const meta = el("div", "detail-meta");
  if (t.symbol) meta.appendChild(el("span", "mono", "sym: " + t.symbol));
  if (t.source) meta.appendChild(el("span", "", "src: " + t.source));
  if (t.workflowStatus) meta.appendChild(el("span", "", "wf: " + t.workflowStatus));
  box.appendChild(meta);

  const nm = state.nearmissCache.get(t.id);
  if (nm && nm.banked) {
    box.appendChild(el("div", "tip-box",
      "<b>Near-miss draft</b> · mismatch " + (nm.mismatchCount ?? "?") +
      " · rank " + (nm.statusRank ?? "?") + (nm.file ? " · " + nm.file : "") +
      (nm.candidates > 1 ? ` · ${nm.candidates} candidates` : "")));
  }

  if (t.history && t.history.length) {
    const hk = el("div", "k", "Prior tries (" + t.history.length + ")");
    const ul = el("ul", "history-list");
    t.history.forEach((h) => {
      const li = el("li", "history-row");
      li.appendChild(el("span", "h-time", fmtTimeShort(h.ts)));
      const label = h.status || h.event || "?";
      const cls = STATUS_CLASS[h.status] ? "ad-status st-" + h.status : "ad-status st-" + (h.event || "");
      li.appendChild(el("span", cls, label));
      const extra = [h.batchIndex != null ? "batch " + h.batchIndex : null,
                     h.attempt != null ? "attempt " + h.attempt : null]
        .filter(Boolean).join(" · ");
      li.appendChild(el("span", "hint", extra));
      ul.appendChild(li);
    });
    box.appendChild(hk);
    box.appendChild(ul);
  } else {
    box.appendChild(el("div", "hint", "No attempts logged in the ledger for this target yet."));
  }
  td.appendChild(box);
  tr.appendChild(td);
  tbody.appendChild(tr);
}

$("#tu-go").addEventListener("click", () => { state.tu = $("#tu-input").value; loadTu(); });
$("#tu-input").addEventListener("keydown", (ev) => {
  if (ev.key === "Enter") { state.tu = $("#tu-input").value; loadTu(); }
});
$("#tu-search").addEventListener("input", (ev) => { state.tuSearch = ev.target.value; applyTuFilters(); });
$("#tu-sort").addEventListener("change", (ev) => { state.tuSort = ev.target.value; applyTuFilters(); });

function renderTranscripts(trs) {
  const box = $("#tu-transcripts");
  box.innerHTML = "";
  if (!trs.length) return;
  const h = el("h2", "section-title", "Transcripts (" + trs.length + ")");
  box.appendChild(h);
  trs.slice(0, 25).forEach((t) => {
    const a = el("a", "mono", t.label);
    a.href = "/transcripts/" + encodeURI(t.path);
    a.target = "_blank";
    const line = el("div", "tr-line");
    line.appendChild(a);
    box.appendChild(line);
  });
}

/* ---------- cost ---------- */

async function renderCost() {
  try {
    const c = await api("/api/cost");
    if (c.source === "cost_report.py") renderCostReport(c);
    else renderCostLite(c);
  } catch (err) {
    flash("cost: " + err.message);
  }
}

function costCards(sum, entries) {
  sum.innerHTML = "";
  entries.forEach(([k, v, cls]) => {
    const card = el("div", "aero-panel card " + (cls || ""));
    card.appendChild(el("div", "k", k));
    card.appendChild(el("div", "v", v));
    sum.appendChild(card);
  });
}

function histRow(label, value, total) {
  const row = el("div", "");
  row.style.cssText =
    "display:grid;grid-template-columns:200px 1fr 60px;gap:10px;" +
    "align-items:center;padding:3px 0;font-size:12px";
  const name = el("span", "mono", label);
  name.style.cssText =
    "overflow:hidden;text-overflow:ellipsis;white-space:nowrap;color:var(--aero-muted)";
  const barWrap = el("div", "");
  barWrap.style.cssText =
    "height:10px;background:rgba(0,0,0,0.12);border:1px solid var(--aero-border);" +
    "border-radius:999px;overflow:hidden";
  const fill = el("div", "");
  const pct = total ? Math.max(2, Math.round((100 * value) / total)) : 0;
  fill.style.cssText = "height:100%;background:linear-gradient(90deg,rgb(var(--aero-primary-rgb)),rgb(var(--aero-accent-rgb)))";
  fill.style.width = pct + "%";
  fill.title = `${value} of ${total}`;
  barWrap.appendChild(fill);
  const num = el("span", "num", String(value));
  row.append(name, barWrap, num);
  return row;
}

function failureMixBlock(fm) {
  fm = fm || {};
  const wrap = el("div", "");
  wrap.appendChild(el("div", "sub", "Failure mix"));
  const evs = Object.entries(fm.events || {});
  if (!evs.length) {
    wrap.appendChild(el("div", "empty", "No ledger events yet."));
  } else {
    const total = evs.reduce((s, [, v]) => s + v, 0);
    evs.sort((a, b) => b[1] - a[1]).slice(0, 12)
      .forEach(([ev, n]) => wrap.appendChild(histRow(ev, n, total)));
  }
  const st = Object.entries(fm.targetStatusCurrent || {});
  if (st.length) {
    wrap.appendChild(el("div", "sub", "targets.json status (ledger-touched)"));
    const total = st.reduce((s, [, v]) => s + v, 0);
    st.sort((a, b) => b[1] - a[1]).slice(0, 12)
      .forEach(([s, n]) => wrap.appendChild(histRow(s, n, total)));
  }
  return wrap;
}

function nearMissBlock(nm) {
  nm = nm || {};
  const wrap = el("div", "");
  wrap.appendChild(el("div", "sub", "Near-miss pile"));
  if (!nm.count) {
    wrap.appendChild(el("div", "empty", "No banked near-miss targets in targets.json."));
    return wrap;
  }
  wrap.appendChild(el("div", "sub",
    `${nm.count} near-miss target(s)` +
    (nm.harnessTouched ? ` · ${nm.harnessTouched} in harness-touched units` : "")));
  const per = Object.entries(nm.perUnit || {});
  if (per.length) {
    const table = el("table");
    const thead = el("thead");
    const hr = el("tr");
    ["Unit", "Near-miss"].forEach((h) =>
      hr.appendChild(el("th", h === "Unit" ? "" : "num", h)));
    thead.appendChild(hr);
    const tbody = el("tbody");
    per.forEach(([u, n]) => {
      const tr = el("tr");
      tr.appendChild(el("td", "mono", u));
      tr.appendChild(el("td", "num", String(n)));
      tbody.appendChild(tr);
    });
    table.appendChild(thead);
    table.appendChild(tbody);
    wrap.appendChild(table);
  }
  return wrap;
}

function renderCostReport(c) {
  const o = c.overall || {};
  const tk = o.tokens || {};
  const p = c.prices || {};
  const basis = c.priced
    ? "cost basis: priced — USD per 1M tokens from pi-harness.json costModel (" +
      ["input $" + p.inputPerM + "/M",
       "output $" + p.outputPerM + "/M",
       "cacheRead $" + p.cacheReadPerM + "/M",
       "cacheWrite $" + p.cacheWritePerM + "/M"].join(" · ") + ")"
    : "cost basis: tokens-only — no costModel prices in pi-harness.json, costs shown as n/a";
  $("#cost-note").innerHTML =
    `<div>${esc(basis)}</div>` +
    `<div>source: cost_report.py · generated ${fmtTime(c.cachedAt)} · ledger ${esc(c.ledger || "")}</div>`;

  costCards($("#cost-summary"), [
    ["Sessions", fmtInt(o.sessions), ""],
    ["Input", fmtTokens(tk.input), ""],
    ["Output", fmtTokens(tk.output), ""],
    ["Cache read", fmtTokens(tk.cacheRead), ""],
    ["Est cost", c.priced ? fmtMoney(o.costUsd) : "—", "c-cyan"],
    ["Accepted", fmtInt(o.accepted), "c-green"],
  ]);

  const tuBox = $("#cost-tu");
  tuBox.innerHTML = "";
  tuBox.appendChild(failureMixBlock(c.failureMix));
  renderCostTuReport(tuBox, c.perTu || [], c.priced);

  const batchBox = $("#cost-batch");
  batchBox.innerHTML = "";
  batchBox.appendChild(nearMissBlock(c.nearMiss));
  batchBox.appendChild(el("div", "empty",
    "per-batch granularity is not part of cost_report.py output (the report aggregates at TU level); " +
    "the viewer-lite fallback shows per-batch rows."));
}

function renderCostTuReport(box, rows, priced) {
  if (!rows.length) {
    box.appendChild(el("div", "empty", "No data yet."));
    return;
  }
  const maxCost = Math.max(...rows.map((r) => r.costUsd || 0));
  const table = el("table");
  const thead = el("thead");
  const hr = el("tr");
  ["TU", "Accepts", "Sessions", "Input", "Output", "CacheR", "Cost", "$/accept"]
    .forEach((h) => hr.appendChild(el("th", h === "TU" ? "" : "num", h)));
  thead.appendChild(hr);
  const tbody = el("tbody");
  rows.forEach((r) => {
    const tr = el("tr", "clickable");
    const a = el("td", "mono", r.tu);
    a.title = "open in TU detail";
    a.style.cursor = "pointer";
    a.onclick = () => openTu(r.tu);
    tr.appendChild(a);
    tr.appendChild(el("td", "num", String(r.accepted ?? 0)));
    tr.appendChild(el("td", "num", String(r.sessions ?? 0)));
    const tk = r.tokens || {};
    tr.appendChild(el("td", "num", fmtTokens(tk.input)));
    tr.appendChild(el("td", "num", fmtTokens(tk.output)));
    tr.appendChild(el("td", "num", fmtTokens(tk.cacheRead)));
    const costTd = el("td", "num");
    if (priced && r.costUsd != null) {
      const bar = el("span", "cost-bar");
      bar.style.width = Math.max(4, Math.round(60 * (r.costUsd / maxCost))) + "px";
      costTd.appendChild(bar);
      costTd.appendChild(document.createTextNode(fmtMoney(r.costUsd)));
    } else {
      costTd.textContent = priced ? "—" : fmtTokens(tk.input + tk.output);
    }
    tr.appendChild(costTd);
    tr.appendChild(el("td", "num", priced ? fmtMoney(r.usdPerAccept) : "—"));
    tbody.appendChild(tr);
  });
  table.appendChild(thead);
  table.appendChild(tbody);
  box.appendChild(table);
}

function renderCostLite(c) {
  const t = c.totals;
  const basis = c.priced
    ? "priced via pi-harness.json costModel: " +
      ["input $" + c.costModel.inputPerM + "/M",
       "output $" + c.costModel.outputPerM + "/M",
       "cacheRead $" + c.costModel.cacheReadPerM + "/M",
       "cacheWrite $" + c.costModel.cacheWritePerM + "/M"].join(" · ")
    : "no costModel prices in pi-harness.json — showing tokens instead of $";
  $("#cost-note").innerHTML =
    `<div>${esc(basis)}</div>` +
    `<div>source: viewer-lite (cost_report.py unavailable) — lightweight viewer aggregation</div>`;

  costCards($("#cost-summary"), [
    ["Sessions", fmtInt(t.sessions), ""],
    ["Input", fmtTokens(t.input), ""],
    ["Output", fmtTokens(t.output), ""],
    ["Cache read", fmtTokens(t.cacheRead), ""],
    ["Est cost", c.priced ? fmtMoney(t.estCost) : "—", "c-cyan"],
    ["Accepted", fmtInt(t.accepted), "c-green"],
  ]);

  renderCostTable($("#cost-tu"), c.perTu, c.priced, "tu");
  renderCostTable($("#cost-batch"), c.perBatch, c.priced, "batch");
}

function renderCostTable(box, rows, priced, mode) {
  box.innerHTML = "";
  if (!rows.length) {
    box.appendChild(el("div", "empty", "No data yet."));
    return;
  }
  const costs = rows.map((r) => r.estCost || 0);
  const maxCost = Math.max(...costs, 1);
  const table = el("table");
  const thead = el("thead");
  const hr = el("tr");
  const cols = mode === "tu"
    ? ["TU", "Sessions", "Input", "Output", "CacheR", "Est cost", "Accepted", "$/match"]
    : ["TU", "Batch", "Sessions", "Input", "Output", "CacheR", "Est cost", "Accepted"];
  cols.forEach((h) => {
    const num = h === "TU" || h === "Batch" ? "" : "num";
    hr.appendChild(el("th", num, h));
  });
  thead.appendChild(hr);
  const tbody = el("tbody");
  rows.forEach((r) => {
    const tr = el("tr", "clickable");
    if (mode === "tu") {
      const a = el("td", "mono", r.tu);
      a.title = "open in TU detail";
      a.style.cursor = "pointer";
      a.onclick = () => openTu(r.tu);
      tr.appendChild(a);
    } else {
      tr.appendChild(el("td", "mono", r.tu));
      tr.appendChild(el("td", "mono", r.batch));
    }
    tr.appendChild(el("td", "num", String(r.sessions)));
    tr.appendChild(el("td", "num", fmtTokens(r.input)));
    tr.appendChild(el("td", "num", fmtTokens(r.output)));
    tr.appendChild(el("td", "num", fmtTokens(r.cacheRead)));
    const costTd = el("td", "num");
    if (priced && r.estCost != null) {
      const bar = el("span", "cost-bar");
      bar.style.width = Math.max(4, Math.round(60 * ((r.estCost || 0) / maxCost))) + "px";
      costTd.appendChild(bar);
      costTd.appendChild(document.createTextNode(fmtMoney(r.estCost)));
    } else {
      costTd.textContent = priced ? "—" : "—";
    }
    tr.appendChild(costTd);
    tr.appendChild(el("td", "num", String(r.accepted)));
    if (mode === "tu") {
      tr.appendChild(el("td", "num", priced ? fmtMoney(r.costPerMatch) : "—"));
    }
    tbody.appendChild(tr);
  });
  table.appendChild(thead);
  table.appendChild(tbody);
  box.appendChild(table);
}

/* ---------- live log ---------- */

async function renderLog() {
  try {
    const path = "/api/ledger?limit=200" +
      (state.logFilter ? "&event=" + encodeURIComponent(state.logFilter) : "");
    const d = await api(path);
    if (!state.logEvents.length) {
      state.logEvents = [...new Set(d.entries.map((e) => e.event))].sort();
      renderChips();
    }
    $("#log-count").textContent =
      `ledger tail · ${fmtInt(d.count)} matching, showing ${d.entries.length} (newest last)`;
    renderLogRows(d.entries);
  } catch (err) {
    flash("log: " + err.message);
  }
}

function renderChips() {
  const box = $("#log-chips");
  box.innerHTML = "";
  const mk = (label, val) => {
    const b = el("button", state.logFilter === val ? "active" : "", label);
    b.onclick = () => { state.logFilter = val; renderChips(); renderLog(); };
    box.appendChild(b);
  };
  mk("All", null);
  state.logEvents.forEach((e) => mk(e, e));
}

function renderLogRows(entries) {
  const box = $("#log-rows");
  box.innerHTML = "";
  const q = state.logSearch.trim().toLowerCase();
  let rows = entries;
  if (q) {
    rows = entries.filter((e) => summary(e.detail).toLowerCase().includes(q) ||
      (e.tu || "").toLowerCase().includes(q) || (e.event || "").toLowerCase().includes(q));
  }
  if (!rows.length) {
    box.appendChild(el("div", "empty", q ? "No entries match the search." : "No entries."));
    return;
  }
  rows.slice().reverse().forEach((e) => {
    const row = el("div", "log-row");
    row.appendChild(el("span", "log-time", fmtTime(e.ts)));
    const ev = el("span", "chip " + (EVENT_CLASS[e.event] || "dim"), e.event);
    row.appendChild(ev);
    row.appendChild(el("span", "log-tu", e.tu || ""));
    const sum = el("span", "log-summary", truncate(summary(e.detail), 300));
    sum.title = summary(e.detail);
    row.appendChild(sum);
    box.appendChild(row);
  });
}

$("#log-search").addEventListener("input", (ev) => {
  state.logSearch = ev.target.value;
  renderLog();
});

function summary(d) {
  d = d || {};
  const bits = [];
  if (d.label) bits.push(d.label);
  if (d.targetId) bits.push(d.targetId);
  if (d.batchIndex !== undefined) bits.push("batch " + d.batchIndex);
  if (d.attempt !== undefined) bits.push("attempt " + d.attempt);
  if (d.acceptedCount !== undefined) bits.push("accepted " + d.acceptedCount);
  if (d.outcome) bits.push(d.outcome);
  if (d.reason) bits.push(d.reason);
  if (d.remainingCount !== undefined) bits.push("remaining " + d.remainingCount);
  if (d.unmatchedCount !== undefined) bits.push("unmatched " + d.unmatchedCount);
  if (d.buildOk !== undefined) bits.push("buildOk=" + d.buildOk);
  if (d.error) bits.push(String(d.error));
  if (d.recoveryOutput) bits.push(String(d.recoveryOutput));
  if (d.results && d.results.length) {
    bits.push(d.results.map((r) => (r.targetId || "?") + ":" + (r.status || "?")).join(", "));
  }
  return bits.join(" · ");
}

function truncate(s, n) {
  return s.length > n ? s.slice(0, n) + " …" : s;
}

/* ---------- settings (form editor for pi-harness.json) ---------- */

// Defaults mirror tools/pi_harness/src/config.ts defaultConfig().
const CFG_DEFAULTS = {
  matchModel: { provider: "openai-codex", model: "gpt-5.3-codex", thinkingLevel: "high" },
  cleanupModel: { provider: "openai-codex", model: "gpt-5.3-codex", thinkingLevel: "medium" },
  batchSize: 5,
  maxParallelTUs: 2,
  selection: "claim-order",
  triage: "off",
  maxBatchRetries: 2,
  singletonEnabled: true,
  rebatchEnabled: true,
  maxRebatchAttempts: 0,
  maxTokens: 0,
  singletonMinSize: 0,
  maxBriefChars: 80000,
  briefTargetChars: 12000,
  maxBatchMinutes: 60,
  timeoutRetries: 3,
  rejectionRetries: 1,
  maxAttemptsPerTarget: 4,
  staleRoundThreshold: 2,
  retryExhausted: false,
  bankOnlyOnBetter: true,
  region: "us",
  sessionDir: "build/pi-harness/sessions",
  ledgerPath: "build/pi-harness/ledger.jsonl",
  nearmissDir: "build/pi-harness/nearmiss",
  knownWallsPath: "docs/KNOWN_WALLS.md",
  pythonBin: "",
  costModel: { inputPerM: 0, outputPerM: 0, cacheReadPerM: 0, cacheWritePerM: 0 },
};

const THINK_LEVELS = ["off", "minimal", "low", "medium", "high", "xhigh"];

function mkModelFields(prefix) {
  return [
    { id: prefix + "-provider", key: prefix + ".provider", label: "Provider", type: "text", list: "provider-list",
      desc: "Provider id, e.g. openrouter, openai-codex." },
    { id: prefix + "-model", key: prefix + ".model", label: "Model", type: "text",
      desc: "e.g. gpt-5.3-codex or deepseek/deepseek-v4-flash." },
    { id: prefix + "-thinking", key: prefix + ".thinkingLevel", label: "Thinking level", type: "select", options: THINK_LEVELS,
      desc: "Reasoning effort for this phase." },
  ];
}

const SECTIONS = [
  {
    title: "Models",
    desc: "Which model drives matching and the TU-final cleanup pass.",
    panels: [
      { title: "Match model", fields: mkModelFields("matchModel") },
      { title: "Cleanup model", fields: mkModelFields("cleanupModel") },
    ],
  },
  {
    title: "Batching & parallelism",
    desc: "How targets are grouped and how many sessions run at once.",
    fields: [
      { id: "batchSize", key: "batchSize", label: "Batch size", type: "int", min: 1,
        desc: "Targets per batch session." },
      { id: "maxParallelTUs", key: "maxParallelTUs", label: "Parallel TUs", type: "int", min: 1,
        desc: "Translation units running at the same time." },
      { id: "selection", key: "selection", label: "Target selection", type: "select",
        options: [["claim-order", "Claim order"], ["similarity", "Similarity"], ["random", "Random"]],
        desc: "How targets are picked for a batch." },
      { id: "triage", key: "triage", label: "Pre-batch triage", type: "select",
        options: [["off", "Off"], ["route", "Route"]],
        desc: "No-SMT routing before batching (opt-in)." },
      { id: "maxBatchRetries", key: "maxBatchRetries", label: "Max batch retries", type: "int", min: 1,
        desc: "Total re-batch attempts for a group." },
      { id: "singletonEnabled", key: "singletonEnabled", label: "Singleton retries", type: "bool",
        desc: "Retry big targets as their own session." },
      { id: "rebatchEnabled", key: "rebatchEnabled", label: "Rebatch small targets", type: "bool",
        desc: "Small targets go back through a rebatch pass." },
      { id: "maxRebatchAttempts", key: "maxRebatchAttempts", label: "Rebatch budget", type: "int", min: 0,
        desc: "Sessions for rebatch across a TU. 0 = none." },
      { id: "singletonMinSize", key: "singletonMinSize", label: "Singleton min size", type: "int", min: 0,
        desc: "Retail bytes below which a target rebatches instead of singleton. 0 = all singletons." },
      { id: "maxAttemptsPerTarget", key: "maxAttemptsPerTarget", label: "Attempts per target", type: "int", min: 1,
        desc: "Total sessions a target may use across passes." },
    ],
  },
  {
    title: "Sessions & budgets",
    desc: "Per-session limits and retry caps.",
    fields: [
      { id: "timeoutRetries", key: "timeoutRetries", label: "Timeout re-prompts", type: "int", min: 0,
        desc: "In-session retries when the session timed out." },
      { id: "rejectionRetries", key: "rejectionRetries", label: "Rejection re-prompts", type: "int", min: 0,
        desc: "Retries when the model finished but code still fails." },
      { id: "maxTokens", key: "maxTokens", label: "Max output tokens", type: "int", min: 0,
        desc: "Per session. 0 = model default." },
      { id: "maxBriefChars", key: "maxBriefChars", label: "Max brief chars", type: "int", min: 1000,
        desc: "Total ASM/context budget per batch brief." },
      { id: "briefTargetChars", key: "briefTargetChars", label: "Brief share per target", type: "int", min: 1,
        desc: "ASM cap per target inside a brief." },
      { id: "maxBatchMinutes", key: "maxBatchMinutes", label: "Batch time limit", type: "number", min: 0.1, step: "any",
        desc: "Minutes before a batch session is cut off." },
      { id: "staleRoundThreshold", key: "staleRoundThreshold", label: "Stale round threshold", type: "int", min: 1,
        desc: "Verify rounds without improvement before early-stop." },
      { id: "retryExhausted", key: "retryExhausted", label: "Retry exhausted targets", type: "bool",
        desc: "Re-attempt targets the ledger marked exhausted." },
      { id: "bankOnlyOnBetter", key: "bankOnlyOnBetter", label: "Bank only when better", type: "bool",
        desc: "Only bank a near-miss draft that beats the stored best." },
    ],
  },
  {
    title: "Paths",
    desc: "Repo-relative locations; pythonBin empty = auto-detect from .venv.",
    fields: [
      { id: "region", key: "region", label: "Region", type: "text", desc: "Build region, e.g. us." },
      { id: "sessionDir", key: "sessionDir", label: "Session dir", type: "text", desc: "Where pi sessions and transcripts live." },
      { id: "ledgerPath", key: "ledgerPath", label: "Ledger path", type: "text", desc: "Event log (JSONL)." },
      { id: "nearmissDir", key: "nearmissDir", label: "Near-miss dir", type: "text", desc: "Draft bank + index.jsonl." },
      { id: "knownWallsPath", key: "knownWallsPath", label: "Known walls doc", type: "text", desc: "Included in briefs. Empty = omit." },
      { id: "pythonBin", key: "pythonBin", label: "Python binary", type: "text", desc: "Empty = auto-detect .venv." },
    ],
  },
  {
    title: "Cost model",
    desc: "USD per 1M tokens. All zeros = not priced (Cost view shows tokens).",
    fields: [
      { id: "inPerM", key: "costModel.inputPerM", label: "Input $/M", type: "number", min: 0, step: "any" },
      { id: "outPerM", key: "costModel.outputPerM", label: "Output $/M", type: "number", min: 0, step: "any" },
      { id: "crPerM", key: "costModel.cacheReadPerM", label: "Cache read $/M", type: "number", min: 0, step: "any" },
      { id: "cwPerM", key: "costModel.cacheWritePerM", label: "Cache write $/M", type: "number", min: 0, step: "any" },
    ],
  },
];

let configLoaded = false;
let cfgDirty = false;
let configPath = "";
let unknownKeys = {};

function showCfgErrors(errs, ok) {
  const box = $("#cfg-errors");
  box.classList.toggle("ok", !!ok);
  box.style.display = "block";
  box.innerHTML = (errs || []).map((e) => "<div>• " + esc(e) + "</div>").join("");
}

function hideCfgErrors() {
  $("#cfg-errors").style.display = "none";
}

function deepMerge(base, over) {
  const out = { ...base };
  for (const k of Object.keys(over || {})) {
    if (out[k] && typeof out[k] === "object" && !Array.isArray(out[k]) &&
        over[k] && typeof over[k] === "object" && !Array.isArray(over[k])) {
      out[k] = deepMerge(out[k], over[k]);
    } else {
      out[k] = over[k];
    }
  }
  return out;
}

function valueAt(obj, path) {
  let v = obj;
  for (const k of path.split(".")) {
    if (v == null || typeof v !== "object") return undefined;
    v = v[k];
  }
  return v;
}

function defVal(path) {
  let v = CFG_DEFAULTS;
  for (const k of path.split(".")) v = v && v[k];
  return v;
}

function fId(key) { return "f-" + key.replace(/\./g, "-"); }

function fieldEl(f, cfg) {
  const val = valueAt(cfg, f.key);
  const wrap = el("div", "cfg-field" + (f.type === "bool" ? " bool" : ""));
  const label = el("label", "", f.label);
  label.htmlFor = fId(f.key);
  let input;
  if (f.type === "bool") {
    input = el("input");
    input.type = "checkbox";
    input.id = label.htmlFor;
    input.checked = !!val;
    wrap.append(input, label);
  } else if (f.type === "select") {
    input = el("select");
    input.id = label.htmlFor;
    f.options.forEach((o) => {
      const [value, text] = Array.isArray(o) ? o : [o, o];
      const opt = el("option", "", text);
      opt.value = value;
      if (value === val) opt.selected = true;
      input.appendChild(opt);
    });
    wrap.appendChild(label);
    wrap.appendChild(input);
  } else if (f.type === "text") {
    input = el("input");
    input.type = "text";
    input.id = label.htmlFor;
    input.value = val ?? "";
    if (f.list) input.setAttribute("list", f.list);
    wrap.appendChild(label);
    wrap.appendChild(input);
  } else {
    input = el("input");
    input.type = "number";
    input.id = label.htmlFor;
    input.step = f.step || "1";
    if (f.min !== undefined) input.min = f.min;
    input.value = val ?? "";
    input.placeholder = String(defVal(f.key) ?? "");
    wrap.appendChild(label);
    wrap.appendChild(input);
  }
  if (f.desc) wrap.appendChild(el("p", "f-desc", f.desc));
  input.addEventListener("input", markCfgDirty);
  input.addEventListener("change", markCfgDirty);
  return wrap;
}

function markCfgDirty() {
  cfgDirty = true;
  const s = $("#cfg-status");
  if (s.textContent && !s.textContent.includes("unsaved")) {
    s.textContent += " · unsaved changes";
  } else if (!s.textContent) {
    s.textContent = "unsaved changes";
  }
}

function renderConfigForm(cfg) {
  const box = $("#cfg-form");
  box.innerHTML = "";
  SECTIONS.forEach((sec) => {
    const panel = el("div", "aero-panel cfg-section");
    panel.appendChild(el("h2", "cfg-sec-title", sec.title));
    if (sec.desc) panel.appendChild(el("p", "hint", sec.desc));
    const grid = el("div", "cfg-grid");
    if (sec.panels) {
      sec.panels.forEach((p) => {
        const sub = el("div", "cfg-subpanel");
        sub.appendChild(el("h3", "cfg-sub-title", p.title));
        const g2 = el("div", "cfg-grid");
        p.fields.forEach((f) => g2.appendChild(fieldEl(f, cfg)));
        sub.appendChild(g2);
        grid.appendChild(sub);
      });
    } else {
      (sec.fields || []).forEach((f) => grid.appendChild(fieldEl(f, cfg)));
    }
    panel.appendChild(grid);
    box.appendChild(panel);
  });
}

function collectForm() {
  const out = {};
  const set = (f) => {
    const inp = document.getElementById(fId(f.key));
    if (!inp) return;
    const path = f.key.split(".");
    let obj = out;
    for (let i = 0; i < path.length - 1; i++) obj = obj[path[i]] = obj[path[i]] || {};
    const leaf = path[path.length - 1];
    if (f.type === "bool") obj[leaf] = inp.checked;
    else if (f.type === "text" || f.type === "select") obj[leaf] = inp.value;
    else {
      const n = Number(inp.value);
      obj[leaf] = inp.value === "" || Number.isNaN(n) ? defVal(f.key) : n;
    }
  };
  SECTIONS.forEach((sec) => {
    (sec.fields || []).forEach(set);
    (sec.panels || []).forEach((p) => p.fields.forEach(set));
  });
  return out;
}

function applyCfg(c) {
  let parsed = c.config || {};
  if (c.parseError) {
    showCfgErrors(["pi-harness.json failed to parse: " + c.parseError +
      " — defaults are shown below; use Raw JSON to fix it, then Apply to form."]);
  } else {
    hideCfgErrors();
  }
  unknownKeys = {};
  // Deprecated alias: harness maps singletonRetry -> singletonEnabled.
  if ("singletonRetry" in parsed && !("singletonEnabled" in parsed)) {
    parsed.singletonEnabled = parsed.singletonRetry;
  }
  Object.keys(parsed).forEach((k) => {
    if (!(k in CFG_DEFAULTS) && k !== "singletonRetry") unknownKeys[k] = parsed[k];
  });
  renderConfigForm(deepMerge(CFG_DEFAULTS, parsed));
  const raw = c.raw ?? JSON.stringify(parsed, null, 2);
  $("#cfg-editor").value = raw;
  const unk = Object.keys(unknownKeys);
  $("#cfg-status").textContent = c.exists
    ? "editing " + c.path + (cfgDirty ? " · unsaved changes" : "")
    : c.path + " does not exist — saving creates it";
  $("#cfg-note").innerHTML =
    "Most settings apply at harness startup — restart the running harness to pick them up. " +
    "<b>costModel</b> is read live by the Cost view (≤30s). " +
    "<b>pythonBin</b> is auto-resolved by the harness (empty = auto-detect). " +
    (unk.length
      ? "<b>" + unk.length + " unknown key" + (unk.length > 1 ? "s" : "") +
        " preserved</b> (ignored by the harness): " + esc(unk.join(", ")) + ". "
      : "") +
    "Each save keeps a timestamped backup beside the file.";
}

async function renderSettings() {
  if (configLoaded) return; // don't clobber unsaved edits on tab switches
  try {
    const c = await api("/api/config");
    configLoaded = true;
    configPath = c.path;
    cfgDirty = false;
    applyCfg(c);
  } catch (err) {
    flash("settings: " + err.message);
  }
}

async function saveCfg() {
  const data = collectForm();
  Object.assign(data, unknownKeys); // preserve unknown top-level keys
  const btn = $("#cfg-save");
  btn.disabled = true;
  try {
    const r = await fetch("/api/config", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data),
    });
    const res = await r.json();
    if (res.ok) {
      cfgDirty = false;
      hideCfgErrors();
      $("#cfg-editor").value = JSON.stringify(data, null, 2);
      $("#cfg-status").textContent = "saved " + res.path +
        (res.backup ? " · backup " + res.backup.split("/").pop() : "");
      flash("settings saved");
    } else {
      showCfgErrors(res.errors || [res.error || "save failed"]);
      $("#cfg-status").textContent = "save rejected — fix the errors above";
    }
  } catch (err) {
    showCfgErrors(["network error: " + err.message]);
  } finally {
    btn.disabled = false;
  }
}

async function resetCfg() {
  try {
    const c = await api("/api/config");
    cfgDirty = false;
    applyCfg(c);
  } catch (err) {
    flash("settings: " + err.message);
  }
}

function formatRawCfg() {
  try {
    const v = JSON.parse($("#cfg-editor").value);
    $("#cfg-editor").value = JSON.stringify(v, null, 2);
  } catch (err) {
    showCfgErrors(["JSON parse error: " + err.message]);
  }
}

function applyRawCfg() {
  let parsed;
  try {
    parsed = JSON.parse($("#cfg-editor").value);
  } catch (err) {
    showCfgErrors(["JSON parse error: " + err.message]);
    return;
  }
  if (!parsed || typeof parsed !== "object" || Array.isArray(parsed)) {
    showCfgErrors(["config must be a JSON object"]);
    return;
  }
  hideCfgErrors();
  cfgDirty = true;
  applyCfg({ config: parsed, raw: JSON.stringify(parsed, null, 2), exists: configPath ? true : false });
  $("#cfg-status").textContent = (configPath ? "editing " + configPath : "new file") + " · unsaved changes";
}

$("#cfg-save").addEventListener("click", saveCfg);
$("#cfg-reset").addEventListener("click", resetCfg);
$("#cfg-format").addEventListener("click", formatRawCfg);
$("#cfg-apply-raw").addEventListener("click", applyRawCfg);
$("#cfg-editor").addEventListener("keydown", (ev) => {
  if ((ev.metaKey || ev.ctrlKey) && ev.key === "s") {
    ev.preventDefault();
    saveCfg();
  }
});

/* ---------- polling + datalist ---------- */

async function refreshDatalist() {
  try {
    const o = await api("/api/overview");
    const dl = $("#tu-list");
    dl.innerHTML = "";
    o.tus.forEach((t) => {
      const opt = el("option", "", t.tu);
      opt.value = t.tu;
      dl.appendChild(opt);
    });
  } catch (_) { /* non-fatal */ }
}

setInterval(() => {
  if (document.visibilityState === "hidden") return;
  if (state.view === "dashboard") renderDashboard();
  else if (state.view === "atlas") renderAtlas();
  else if (state.view === "cost") renderCost();
  else if (state.view === "log") renderLog();
}, 2000);

setInterval(() => {
  if (document.visibilityState === "hidden") return;
  refreshDatalist();
}, 15000);

initTheme();
bindAtlasPointer();
refreshDatalist();
renderDashboard();

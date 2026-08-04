"use strict";
/* pi-harness viewer — vanilla JS, no frameworks, 2s polling. */

const $ = (sel) => document.querySelector(sel);

const state = {
  view: "dashboard",
  tu: "",
  transcripts: [],
  logFilter: null,
  logEvents: [],
  busy: false,
  lastPoll: null,
};

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

function sideStatus(msg, ok) {
  const box = $("#side-status");
  if (ok === undefined) box.textContent = msg;
  else box.innerHTML = `<span class="${ok ? "ok" : "err"}">${esc(msg)}</span>`;
}

/* ---------- view switching ---------- */

function switchView(name) {
  state.view = name;
  document.querySelectorAll("#nav button").forEach((b) =>
    b.classList.toggle("active", b.dataset.view === name));
  document.querySelectorAll("main section").forEach((s) =>
    s.classList.toggle("active", s.id === "view-" + name));
  if (name === "dashboard") renderDashboard();
  else if (name === "cost") renderCost();
  else if (name === "log") renderLog();
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
    sideStatus(`live · ${fmtInt(o.sessionCount)} sessions`, true);

    // TU progress bars
    const box = $("#d-tulist");
    box.innerHTML = "";
    if (!o.tus.length) box.appendChild(el("div", "empty", "No TU activity in the ledger yet."));
    else o.tus.slice(0, 60).forEach((t) => box.appendChild(tuRow(o, t)));

    // recent sessions
    const as = $("#d-sessions");
    as.innerHTML = "";
    if (!o.activeSessions.length) as.appendChild(el("div", "empty", "No sessions yet."));
    o.activeSessions.slice().reverse().forEach((s) => as.appendChild(sessionRow(s)));
  } catch (err) {
    sideStatus("poll failed", false);
    flash("dashboard: " + err.message);
  }
}

function tuRow(o, t) {
  const wrap = el("div", "turow");
  const top = el("div", "turow-top");
  const name = el("a", "tu-name", t.tu);
  name.href = "#";
  name.title = t.tu;
  name.onclick = (ev) => {
    ev.preventDefault();
    state.tu = t.tu;
    $("#tu-input").value = t.tu;
    switchView("tu");
    loadTu();
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
  const fill = el("div", "bar-fill");
  fill.style.width = pct + "%";
  fill.title = `${t.accepted} accepted (${pct}%)`;
  const fill2 = el("div", "bar-skip");
  fill2.style.width = skipPct + "%";
  fill2.title = `${t.skipped + t.exhausted} skipped/exhausted`;
  bar.appendChild(fill);
  bar.appendChild(fill2);
  wrap.appendChild(bar);
  return wrap;
}

function sessionRow(s) {
  const row = el("div", "session-row");
  const when = el("span", "log-time", fmtTime(s.ts));
  const tu = el("span", "tu", s.tu);
  tu.title = "open in TU detail";
  tu.onclick = () => {
    state.tu = s.tu;
    $("#tu-input").value = s.tu;
    switchView("tu");
    loadTu();
  };
  const label = el("span", "mono", s.label || "");
  const toks = el("span", "num", fmtTokens(s.tokens) + " tok");
  row.append(when, tu, label, toks);
  return row;
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
    renderTuTable(d);
    renderTranscripts(tr);
    $("#tu-status").textContent =
      `${d.targets.length} targets` + (d.transcriptCount ? ` · ${d.transcriptCount} transcripts` : "");
  } catch (err) {
    $("#tu-status").textContent = "error: " + err.message;
  } finally {
    state.busy = false;
  }
}

function renderTuTable(d) {
  const box = $("#tu-table");
  box.innerHTML = "";
  if (!d.targets.length) {
    box.appendChild(el("div", "empty", "No targets for this unit in the ledger or registry."));
    return;
  }
  const table = el("table");
  const thead = el("thead");
  const hr = el("tr");
  ["ID", "Function", "Status", "Size", "Attempts", "Best div", "Draft"].forEach((h) => {
    const th = el("th", h === "Attempts" || h === "Size" ? "num" : "", h);
    hr.appendChild(th);
  });
  thead.appendChild(hr);
  const tbody = el("tbody");
  d.targets.forEach((t) => {
    const tr = el("tr");
    const id = el("td", "mono", t.id);
    id.title = t.symbol || "";
    tr.appendChild(id);
    const fn = el("td", "", t.function || t.symbol || "");
    fn.title = t.source || "";
    tr.appendChild(fn);
    const st = el("td");
    st.appendChild(el("span", "chip " + (STATUS_CLASS[t.status] || "dim"), t.status || "—"));
    if (t.skipped) st.appendChild(el("span", "chip dim", "skipped"));
    tr.appendChild(st);
    tr.appendChild(el("td", "num mono", t.size || "—"));
    tr.appendChild(el("td", "num", String(t.attempts ?? 0)));
    tr.appendChild(el("td", "num", t.bestDivergence !== null && t.bestDivergence !== undefined
      ? String(t.bestDivergence) : "—"));
    const badge = el("td");
    badge.appendChild(el("span", "badge " + (t.nearmissBanked ? "banked" : "none"),
      t.nearmissBanked ? "draft" : "none"));
    tr.appendChild(badge);
    tbody.appendChild(tr);
  });
  table.appendChild(thead);
  table.appendChild(tbody);
  box.appendChild(table);
}

function renderTranscripts(trs) {
  const box = $("#tu-transcripts");
  box.innerHTML = "";
  if (!trs.length) return;
  const h = el("h2", "", "Transcripts (" + trs.length + ")");
  box.appendChild(h);
  trs.slice(0, 25).forEach((t) => {
    const a = el("a", "mono", t.label);
    a.href = "/transcripts/" + encodeURI(t.path);
    a.target = "_blank";
    const line = el("div", "");
    line.style.margin = "3px 0";
    line.appendChild(a);
    box.appendChild(line);
  });
}

$("#tu-go").addEventListener("click", () => { state.tu = $("#tu-input").value; loadTu(); });
$("#tu-input").addEventListener("keydown", (ev) => {
  if (ev.key === "Enter") { state.tu = $("#tu-input").value; loadTu(); }
});

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
    const card = el("div", "card " + (cls || ""));
    card.appendChild(el("div", "k", k));
    card.appendChild(el("div", "v", v));
    sum.appendChild(card);
  });
}

/* Horizontal bar for the failure-mix / status histograms. Inline styles —
   the viewer has no stylesheet for these. */
function histRow(label, value, total) {
  const row = el("div", "");
  row.style.cssText =
    "display:grid;grid-template-columns:200px 1fr 60px;gap:10px;" +
    "align-items:center;padding:3px 0;font-size:12px";
  const name = el("span", "mono", label);
  name.style.cssText =
    "overflow:hidden;text-overflow:ellipsis;white-space:nowrap;color:var(--muted)";
  const barWrap = el("div", "");
  barWrap.style.cssText =
    "height:10px;background:var(--panel2);border:1px solid var(--border);" +
    "border-radius:5px;overflow:hidden";
  const fill = el("div", "");
  const pct = total ? Math.max(2, Math.round((100 * value) / total)) : 0;
  fill.style.cssText = "height:100%;background:var(--accent)";
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

/* Primary: cost_report.py --json output (single source of truth for cost
   math). Renders the summary cards, the cost-basis line, the failure-mix
   histogram and near-miss pile, and the per-TU table. */
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
    ["Est cost", c.priced ? fmtMoney(o.costUsd) : "—", c.priced ? "cyan" : ""],
    ["Accepted", fmtInt(o.accepted), "green"],
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

/* Per-TU table for the cost_report.py row shape: tokens are nested, costs
   are costUsd / usdPerAccept, sessions == attempts. */
function renderCostTuReport(box, rows, priced) {
  if (!rows.length) {
    box.appendChild(el("div", "empty", "No data yet."));
    return;
  }
  const table = el("table");
  const thead = el("thead");
  const hr = el("tr");
  ["TU", "Accepts", "Sessions", "Input", "Output", "CacheR", "Cost", "$/accept"]
    .forEach((h) => hr.appendChild(el("th", h === "TU" ? "" : "num", h)));
  thead.appendChild(hr);
  const tbody = el("tbody");
  rows.forEach((r) => {
    const tr = el("tr");
    const a = el("td", "mono", r.tu);
    a.title = "open in TU detail";
    a.style.cursor = "pointer";
    a.onclick = () => { state.tu = r.tu; $("#tu-input").value = r.tu; switchView("tu"); loadTu(); };
    tr.appendChild(a);
    tr.appendChild(el("td", "num", String(r.accepted ?? 0)));
    tr.appendChild(el("td", "num", String(r.sessions ?? 0)));
    const tk = r.tokens || {};
    tr.appendChild(el("td", "num", fmtTokens(tk.input)));
    tr.appendChild(el("td", "num", fmtTokens(tk.output)));
    tr.appendChild(el("td", "num", fmtTokens(tk.cacheRead)));
    tr.appendChild(el("td", "num", priced ? fmtMoney(r.costUsd) : "—"));
    tr.appendChild(el("td", "num", priced ? fmtMoney(r.usdPerAccept) : "—"));
    tbody.appendChild(tr);
  });
  table.appendChild(thead);
  table.appendChild(tbody);
  box.appendChild(table);
}

/* Fallback: the viewer's lightweight aggregation (cost_report.py missing or
   failed). Same cards/tables as before, flagged as viewer-lite. */
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
    ["Est cost", c.priced ? fmtMoney(t.estCost) : "—", c.priced ? "cyan" : ""],
    ["Accepted", fmtInt(t.accepted), "green"],
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
    const tr = el("tr");
    if (mode === "tu") {
      const a = el("td", "mono", r.tu);
      a.title = "open in TU detail";
      a.style.cursor = "pointer";
      a.onclick = () => { state.tu = r.tu; $("#tu-input").value = r.tu; switchView("tu"); loadTu(); };
      tr.appendChild(a);
    } else {
      tr.appendChild(el("td", "mono", r.tu));
      tr.appendChild(el("td", "mono", r.batch));
    }
    tr.appendChild(el("td", "num", String(r.sessions)));
    tr.appendChild(el("td", "num", fmtTokens(r.input)));
    tr.appendChild(el("td", "num", fmtTokens(r.output)));
    tr.appendChild(el("td", "num", fmtTokens(r.cacheRead)));
    tr.appendChild(el("td", "num", priced ? fmtMoney(r.estCost) : "—"));
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
  if (!entries.length) {
    box.appendChild(el("div", "empty", "No entries."));
    return;
  }
  entries.slice().reverse().forEach((e) => {
    const row = el("div", "log-row");
    row.appendChild(el("span", "log-time", fmtTime(e.ts)));
    row.appendChild(el("span", "chip " + (EVENT_CLASS[e.event] || "ev-dim"), e.event));
    row.appendChild(el("span", "log-tu", e.tu || ""));
    const sum = el("span", "log-summary", truncate(summary(e.detail), 300));
    sum.title = summary(e.detail);
    row.appendChild(sum);
    box.appendChild(row);
  });
}

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
  else if (state.view === "log") renderLog();
}, 2000);

setInterval(() => {
  if (document.visibilityState === "hidden") return;
  refreshDatalist();
}, 15000);

refreshDatalist();
renderDashboard();

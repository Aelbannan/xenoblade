const API = '/api';
let currentView = 'dashboard';

async function api (path, opts = {}) {
  const res = await fetch(`${API}${path}`, {
    headers: { 'Content-Type': 'application/json', ...opts.headers },
    ...opts,
  });
  if (!res.ok) {
    const text = await res.text().catch(() => '');
    throw new Error(`${res.status} ${res.statusText}${text ? ': ' + text.slice(0, 200) : ''}`);
  }
  return res.json();
}

function $ (sel, ctx = document) { return ctx.querySelector(sel); }
function $$ (sel, ctx = document) { return [...ctx.querySelectorAll(sel)]; }

function html (strings, ...vals) {
  return strings.reduce((acc, s, i) => acc + s + (vals[i] !== undefined ? vals[i] : ''), '');
}

function escape (s) {
  if (s == null) return '';
  return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}

function badge (status) {
  if (!status) return '<span class="badge badge-not-started">—</span>';
  const cls = {
    'FULL_MATCH': 'badge-full-match',
    'EQUIVALENT_MATCH': 'badge-equivalent-match',
    'CODE_MATCH': 'badge-code-match',
    'HIGH_MATCH': 'badge-high-match',
    'STRUCTURAL': 'badge-structural',
    'COMPILES': 'badge-compiles',
    'NOT_STARTED': 'badge-not-started',
    'ACCEPTED': 'badge-accepted',
    'ACTIVE': 'badge-active',
    'DISCOVERY': 'badge-discovery',
    'BACKLOG': 'badge-not-started',
    'ERROR': 'badge-error',
  }[status] || 'badge-not-started';
  return `<span class="badge ${cls}">${escape(status)}</span>`;
}

function pct (v) { return v != null ? `${v.toFixed(1)}%` : '—'; }
function fmt (v) { return v != null ? v.toLocaleString() : '—'; }
function fmtCost (v) { return v != null ? `$${v.toFixed(4)}` : '—'; }

function pctBar (segments) {
  const total = segments.reduce((s, x) => s + x.value, 0);
  if (!total) return '';
  const bars = segments.map(s => {
    const w = (s.value / total * 100).toFixed(1);
    if (w < 0.1) return '';
    return `<div class="progress-segment progress-${s.cls}" style="width:${w}%" title="${escape(s.label)}: ${fmt(s.value)} (${w}%)"></div>`;
  }).join('');
  return `<div class="progress-bar">${bars}</div>`;
}

/* ─── Navigation ─── */

$$('.nav-item').forEach(el => {
  el.addEventListener('click', () => {
    $$('.nav-item.active').forEach(e => e.classList.remove('active'));
    el.classList.add('active');
    navigate(el.dataset.view);
  });
});

function navigate (view) {
  currentView = view;
  const container = $('#viewContainer');
  container.innerHTML = '<div class="loading">Loading…</div>';
  switch (view) {
    case 'dashboard': return renderDashboard(container);
    case 'targets': return renderTargets(container);
    case 'code-map': return renderCodeMap(container);
    case 'runs': return renderRuns(container);
    case 'experiments': return renderExperiments(container);
    case 'start-run': return renderStartRun(container);
    case 'batches': return renderBatches(container);
    case 'io-browser': return renderIOBrowser(container);
    default: container.innerHTML = '<div class="empty">Unknown view</div>';
  }
}

/* ─── Dashboard ─── */

async function renderDashboard (container) {
  try {
    const [stats, progress, codeMap] = await Promise.all([
      api('/stats'),
      api('/progress'),
      api('/code-map'),
    ]);

    const overall = stats.overall || {};
    const targets = progress.targets || {};
    const totals = codeMap.totals || {};

    container.innerHTML = html`
      <div class="card-grid" style="margin-bottom:16px">
        <div class="card">
          <div class="stat">
            <span class="stat-label">Total Experiments</span>
            <span class="stat-value blue">${fmt(overall.total_experiments)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Targets Hit</span>
            <span class="stat-value">${fmt(overall.total_targets)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Winners</span>
            <span class="stat-value green">${fmt(overall.winners)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Accepted</span>
            <span class="stat-value purple">${fmt(overall.accepted)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Errors</span>
            <span class="stat-value red">${fmt(overall.errors)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Total Cost</span>
            <span class="stat-value orange">${fmtCost(overall.total_cost)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Input Tokens</span>
            <span class="stat-value">${fmt(overall.total_input_tokens)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Output Tokens</span>
            <span class="stat-value">${fmt(overall.total_output_tokens)}</span>
          </div>
        </div>
      </div>

      <div class="card-grid" style="margin-bottom:16px">
        <div class="card">
          <div class="stat">
            <span class="stat-label">Total Functions</span>
            <span class="stat-value">${fmt(targets.total)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">FULL_MATCH</span>
            <span class="stat-value green">${fmt(targets.fully_matched)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">EQUIVALENT_MATCH</span>
            <span class="stat-value cyan" style="color:var(--accent-cyan)">${fmt(targets.equivalent_matched)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Started</span>
            <span class="stat-value blue">${fmt(targets.started)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Not Started</span>
            <span class="stat-value red">${fmt(targets.total - targets.started)}</span>
          </div>
        </div>
        <div class="card">
          <div class="stat">
            <span class="stat-label">Decomp Progress</span>
            <span class="stat-value">${pct(totals.started_pct)}</span>
          </div>
        </div>
      </div>

      <div class="card">
        <div class="card-title">Decompilation Progress by Status</div>
        ${pctBar([
          { label: 'FULL_MATCH', value: totals.full_match, cls: 'full-match' },
          { label: 'EQUIVALENT_MATCH', value: totals.equivalent_match, cls: 'equivalent-match' },
          { label: 'CODE_MATCH', value: totals.code_match, cls: 'code-match' },
          { label: 'HIGH_MATCH', value: totals.high_match, cls: 'high-match' },
          { label: 'STRUCTURAL', value: totals.structural, cls: 'structural' },
          { label: 'COMPILES', value: totals.compiles, cls: 'compiles' },
          { label: 'NOT_STARTED', value: totals.not_started, cls: 'not-started' },
        ])}
        <div style="display:flex;flex-wrap:wrap;gap:16px;margin-top:12px;font-size:12px">
          <span><span style="color:var(--accent-green)">■</span> FULL_MATCH: ${fmt(totals.full_match)}</span>
          <span><span style="color:var(--accent-cyan)">■</span> EQUIVALENT_MATCH: ${fmt(totals.equivalent_match)}</span>
          <span><span style="color:#7ee787">■</span> CODE_MATCH: ${fmt(totals.code_match)}</span>
          <span><span style="color:var(--accent-orange)">■</span> HIGH_MATCH: ${fmt(totals.high_match)}</span>
          <span><span style="color:#f97583">■</span> STRUCTURAL: ${fmt(totals.structural)}</span>
          <span><span style="color:var(--accent)">■</span> COMPILES: ${fmt(totals.compiles)}</span>
          <span><span style="color:var(--text-dim)">■</span> NOT_STARTED: ${fmt(totals.not_started)}</span>
        </div>
      </div>

      <div class="card">
        <div class="card-title">Per-Model Statistics</div>
        ${stats.models && stats.models.length ? html`
          <div class="table-wrap">
            <table>
              <thead><tr>
                <th>Model</th><th>Attempts</th><th>Completed</th><th>Wins</th>
                <th>Accepted</th><th>Avg Match%</th><th>Avg Sec</th>
                <th>Input T</th><th>Output T</th><th>Cost</th><th>Errors</th>
              </tr></thead>
              <tbody>
                ${stats.models.map(m => html`
                  <tr>
                    <td><strong>${escape(m.model_id)}</strong></td>
                    <td>${fmt(m.attempts)}</td>
                    <td>${fmt(m.completed)}</td>
                    <td>${fmt(m.wins)}</td>
                    <td>${fmt(m.accepted)}</td>
                    <td>${pct(m.average_match_percent)}</td>
                    <td>${m.average_seconds != null ? m.average_seconds.toFixed(1) + 's' : '—'}</td>
                    <td>${fmt(m.input_tokens)}</td>
                    <td>${fmt(m.output_tokens)}</td>
                    <td>${fmtCost(m.total_cost)}</td>
                    <td>${m.errors ? `<span class="badge badge-error">${m.errors}</span>` : '0'}</td>
                  </tr>
                `).join('')}
              </tbody>
            </table>
          </div>
        ` : '<div class="empty">No model data yet. Run some experiments first.</div>'}
      </div>
    `;
  } catch (e) {
    container.innerHTML = `<div class="empty">Error loading dashboard: ${escape(e.message)}</div>`;
  }
}

/* ─── Targets ─── */

let targetsState = { offset: 0, limit: 200, tier: '', status: '', workflow: '', search: '' };

async function renderTargets (container) {
  container.innerHTML = html`
    <div class="card">
      <div class="card-title">Targets Browser</div>
      <div class="filters">
        <select id="targetTierFilter">
          <option value="">All Tiers</option>
        </select>
        <select id="targetStatusFilter">
          <option value="">All Statuses</option>
          <option value="NOT_STARTED">NOT_STARTED</option>
          <option value="COMPILES">COMPILES</option>
          <option value="STRUCTURAL">STRUCTURAL</option>
          <option value="HIGH_MATCH">HIGH_MATCH</option>
          <option value="CODE_MATCH">CODE_MATCH</option>
          <option value="EQUIVALENT_MATCH">EQUIVALENT_MATCH</option>
          <option value="FULL_MATCH">FULL_MATCH</option>
        </select>
        <select id="targetWorkflowFilter">
          <option value="">All Workflows</option>
          <option value="DISCOVERY">DISCOVERY</option>
          <option value="BACKLOG">BACKLOG</option>
          <option value="CLAIMED">CLAIMED</option>
          <option value="ACCEPTED">ACCEPTED</option>
        </select>
        <div class="search-box" style="position:relative">
          <input type="text" id="targetSearch" placeholder="Search ID/function/symbol…" style="padding-left:30px">
        </div>
        <button id="targetSearchBtn">Search</button>
      </div>
      <div id="targetsTableWrap"><div class="loading">Loading…</div></div>
    </div>
  `;

  // Load tiers
  try {
    const opts = await api('/start-options');
    const sel = $('#targetTierFilter');
    opts.tiers.forEach(t => { sel.innerHTML += `<option value="${escape(t)}">${escape(t)}</option>`; });
  } catch (_) {}

  const load = async () => {
    const wrap = $('#targetsTableWrap');
    try {
      const params = new URLSearchParams({
        offset: targetsState.offset,
        limit: targetsState.limit,
      });
      if (targetsState.tier) params.set('tier', targetsState.tier);
      if (targetsState.status) params.set('status', targetsState.status);
      if (targetsState.workflow) params.set('workflow', targetsState.workflow);
      if (targetsState.search) params.set('search', targetsState.search);

      const data = await api(`/targets?${params}`);
      const { targets, total } = data;

      wrap.innerHTML = html`
        <div style="margin-bottom:8px;font-size:12px;color:var(--text-dim)">
          ${fmt(total)} targets · showing ${targetsState.offset + 1}–${Math.min(targetsState.offset + targets.length, total)}
        </div>
        ${targets.length ? html`
          <div class="table-wrap">
            <table>
              <thead><tr>
                <th>ID</th><th>Function</th><th>Tier</th><th>Status</th><th>Match%</th>
                <th>Workflow</th><th>Required</th><th>Source</th>
              </tr></thead>
              <tbody>
                ${targets.map(t => html`
                  <tr style="cursor:pointer" data-id="${escape(t.id)}" class="target-row">
                    <td style="font-family:monospace;font-size:12px">${escape(t.id)}</td>
                    <td title="${escape(t.symbol || '')}">${escape(t.function || '—')}</td>
                    <td>${badge(t.tier)}</td>
                    <td>${badge(t.status)}</td>
                    <td>${t.instruction_match != null ? pct(t.instruction_match) : '—'}</td>
                    <td>${badge(t.workflow_status)}</td>
                    <td style="font-size:11px">${escape(t.required_level)}</td>
                    <td style="font-size:11px;color:var(--text-dim);max-width:200px;overflow:hidden;text-overflow:ellipsis;white-space:nowrap">${escape(t.source || '')}</td>
                  </tr>
                `).join('')}
              </tbody>
            </table>
          </div>
        ` : '<div class="empty">No targets match filters</div>'}
      `;

      $$('.target-row').forEach(el => {
        el.addEventListener('click', () => showTargetDetail(el.dataset.id));
      });
    } catch (e) {
      wrap.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
    }
  };

  const onFilter = () => {
    targetsState.tier = $('#targetTierFilter').value;
    targetsState.status = $('#targetStatusFilter').value;
    targetsState.workflow = $('#targetWorkflowFilter').value;
    targetsState.search = $('#targetSearch').value;
    targetsState.offset = 0;
    load();
  };

  $('#targetTierFilter').addEventListener('change', onFilter);
  $('#targetStatusFilter').addEventListener('change', onFilter);
  $('#targetWorkflowFilter').addEventListener('change', onFilter);
  $('#targetSearchBtn').addEventListener('click', onFilter);
  $('#targetSearch').addEventListener('keydown', e => { if (e.key === 'Enter') onFilter(); });

  load();
}

async function showTargetDetail (id) {
  const container = $('#viewContainer');
  container.innerHTML = '<div class="loading">Loading target details…</div>';
  try {
    const [target, sourceData] = await Promise.all([
      api(`/targets/${encodeURIComponent(id)}`),
      api(`/source/${encodeURIComponent(id)}`).catch(() => null),
    ]);

    container.innerHTML = html`
      <div style="margin-bottom:16px">
        <button class="filters button secondary" onclick="navigate('targets')" style="padding:6px 12px;border-radius:6px;border:1px solid var(--border);background:var(--bg-hover);color:var(--text);cursor:pointer">← Back to Targets</button>
      </div>

      <div class="card">
        <div class="card-title">${escape(target.id)}</div>
        <div style="display:grid;grid-template-columns:repeat(auto-fill,minmax(250px,1fr));gap:12px">
          <div class="stat"><span class="stat-label">Function</span><span>${escape(target.function || '—')}</span></div>
          <div class="stat"><span class="stat-label">Symbol</span><span style="font-family:monospace;font-size:12px">${escape(target.symbol || '—')}</span></div>
          <div class="stat"><span class="stat-label">Status</span><span>${badge(target.status)}</span></div>
          <div class="stat"><span class="stat-label">Match%</span><span>${target.instruction_match != null ? pct(target.instruction_match) : '—'}</span></div>
          <div class="stat"><span class="stat-label">Workflow</span><span>${badge(target.workflow_status)}</span></div>
          <div class="stat"><span class="stat-label">Required Level</span><span>${escape(target.required_level)}</span></div>
          <div class="stat"><span class="stat-label">Tier</span><span>${escape(target.tier)}</span></div>
          <div class="stat"><span class="stat-label">Unit</span><span style="font-family:monospace;font-size:12px">${escape(target.unit || '—')}</span></div>
          <div class="stat"><span class="stat-label">Address</span><span style="font-family:monospace">${escape(target.address || '—')}</span></div>
          <div class="stat"><span class="stat-label">Size</span><span>${escape(target.size || '—')}</span></div>
          <div class="stat"><span class="stat-label">Source</span><span style="font-size:12px">${escape(target.source || '—')}</span></div>
          <div class="stat"><span class="stat-label">Origin</span><span>${escape(target.origin || '—')}</span></div>
        </div>
        ${target.called_functions && target.called_functions.length ? html`
          <div style="margin-top:12px">
            <div class="stat-label" style="margin-bottom:4px">Called Functions (${target.called_functions.length})</div>
            <div style="display:flex;flex-wrap:wrap;gap:4px;font-size:12px;font-family:monospace">
              ${target.called_functions.map(c => `<span style="background:var(--bg);padding:2px 6px;border-radius:4px">${escape(c)}</span>`).join('')}
            </div>
          </div>
        ` : ''}
      </div>

      ${sourceData ? html`
        <div class="card">
          <div class="card-title">Source Code <span style="font-size:12px;color:var(--text-dim)">${escape(sourceData.source_path)}</span></div>
          <div class="code-block">${escape(sourceData.content)}</div>
        </div>
      ` : ''}

      <div class="card" id="targetRunsCard">
        <div class="card-title">Recent Runs</div>
        <div class="loading">Loading runs…</div>
      </div>
    `;

    // Load runs for this target
    const runsData = await api(`/runs?target_id=${encodeURIComponent(id)}&limit=20`);
    const runsWrap = $('#targetRunsCard .loading');
    if (runsData.runs && runsData.runs.length) {
      runsWrap.outerHTML = html`
        <div class="table-wrap">
          <table>
            <thead><tr>
              <th>Time</th><th>Workflow</th><th>Model</th><th>Run</th><th>Status</th>
              <th>Match%</th><th>Accepted</th><th>Winner</th><th>Duration</th><th>Cost</th>
            </tr></thead>
            <tbody>
              ${runsData.runs.map(r => html`
                <tr>
                  <td style="font-size:11px;white-space:nowrap">${(r.timestamp || '').slice(0, 19).replace('T', ' ')}</td>
                  <td>${escape(r.workflow)}</td>
                  <td>${escape(r.model_id)}</td>
                  <td>${r.run_index}</td>
                  <td>${badge(r.evaluation_status)}</td>
                  <td>${r.match_percent != null ? pct(r.match_percent) : '—'}</td>
                  <td>${r.accepted ? '✅' : '❌'}</td>
                  <td>${r.winner ? '🏆' : ''}</td>
                  <td>${r.duration_seconds != null ? r.duration_seconds.toFixed(1) + 's' : '—'}</td>
                  <td>${fmtCost(r.cost)}</td>
                </tr>
              `).join('')}
            </tbody>
          </table>
        </div>
      `;
    } else {
      runsWrap.outerHTML = '<div class="empty">No runs recorded for this target</div>';
    }
  } catch (e) {
    container.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
  }
}
window.showTargetDetail = showTargetDetail;

/* ─── Code Map ─── */

async function renderCodeMap (container) {
  container.innerHTML = '<div class="loading">Loading code map…</div>';
  try {
    const data = await api('/code-map');
    const { units, totals } = data;

    container.innerHTML = html`
      <div class="card-grid" style="margin-bottom:16px">
        <div class="card">
          <div class="stat"><span class="stat-label">Total Functions</span><span class="stat-value">${fmt(totals.total)}</span></div>
        </div>
        <div class="card">
          <div class="stat"><span class="stat-label">FULL_MATCH</span><span class="stat-value green">${fmt(totals.full_match)}</span></div>
        </div>
        <div class="card">
          <div class="stat"><span class="stat-label">EQUIVALENT_MATCH</span><span class="stat-value" style="color:var(--accent-cyan)">${fmt(totals.equivalent_match)}</span></div>
        </div>
        <div class="card">
          <div class="stat"><span class="stat-label">Not Started</span><span class="stat-value red">${fmt(totals.not_started)}</span></div>
        </div>
      </div>

      <div class="card">
        <div class="card-title">Translation Units <span style="font-size:12px;color:var(--text-dim)">${units.length} units</span></div>
        <div class="table-wrap">
          <table>
            <thead><tr>
              <th>Unit</th><th>Total</th><th>Progress</th>
              <th style="text-align:right">FULL</th><th style="text-align:right">EQUIV</th>
              <th style="text-align:right">CODE</th><th style="text-align:right">HIGH</th>
              <th style="text-align:right">STRUC</th><th style="text-align:right">COMP</th>
              <th style="text-align:right">NS</th>
            </tr></thead>
            <tbody>
              ${units.map(u => html`
                <tr>
                  <td style="font-family:monospace;font-size:12px;max-width:300px;overflow:hidden;text-overflow:ellipsis;white-space:nowrap" title="${escape(u.unit)}">${escape(u.unit)}</td>
                  <td>${u.total}</td>
                  <td style="min-width:120px">
                    ${pctBar([
                      { label: 'FULL_MATCH', value: u.full_match, cls: 'full-match' },
                      { label: 'EQUIVALENT_MATCH', value: u.equivalent_match, cls: 'equivalent-match' },
                      { label: 'CODE_MATCH', value: u.code_match, cls: 'code-match' },
                      { label: 'HIGH_MATCH', value: u.high_match, cls: 'high-match' },
                      { label: 'STRUCTURAL', value: u.structural, cls: 'structural' },
                      { label: 'COMPILES', value: u.compiles, cls: 'compiles' },
                      { label: 'NOT_STARTED', value: u.not_started, cls: 'not-started' },
                    ])}
                  </td>
                  <td style="text-align:right">${u.full_match || ''}</td>
                  <td style="text-align:right">${u.equivalent_match || ''}</td>
                  <td style="text-align:right">${u.code_match || ''}</td>
                  <td style="text-align:right">${u.high_match || ''}</td>
                  <td style="text-align:right">${u.structural || ''}</td>
                  <td style="text-align:right">${u.compiles || ''}</td>
                  <td style="text-align:right;color:${u.not_started ? 'var(--accent-red)' : 'var(--text-dim)'}">${u.not_started || ''}</td>
                </tr>
              `).join('')}
            </tbody>
          </table>
        </div>
      </div>
    `;
  } catch (e) {
    container.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
  }
}

/* ─── Runs ─── */

async function renderRuns (container) {
  container.innerHTML = html`
    <div class="card">
      <div class="card-title">Recent Runs <span class="hint">— from experiments.jsonl</span></div>
      <div class="filters">
        <input type="text" id="runTargetFilter" placeholder="Filter by target ID…">
        <input type="text" id="runModelFilter" placeholder="Filter by model…">
        <select id="runWinnerFilter">
          <option value="">All</option>
          <option value="true">Winners</option>
          <option value="false">Non-winners</option>
        </select>
        <button id="runFilterBtn">Filter</button>
      </div>
      <div id="runsTableWrap"><div class="loading">Loading…</div></div>
    </div>
  `;

  const load = async () => {
    const wrap = $('#runsTableWrap');
    try {
      const params = new URLSearchParams({ limit: '100' });
      const t = $('#runTargetFilter').value.trim();
      if (t) params.set('target_id', t);
      const m = $('#runModelFilter').value.trim();
      if (m) params.set('model', m);
      const w = $('#runWinnerFilter').value;
      if (w) params.set('winner', w);

      const data = await api(`/runs?${params}`);
      if (!data.runs || !data.runs.length) {
        wrap.innerHTML = '<div class="empty">No runs found</div>';
        return;
      }

      wrap.innerHTML = html`
        <div class="table-wrap">
          <table>
            <thead><tr>
              <th>Time</th><th>Target</th><th>Workflow</th><th>Model</th>
              <th>Run</th><th>Status</th><th>Match%</th>
              <th>✓</th><th>🏆</th><th>Duration</th><th>Tokens</th><th>Cost</th>
            </tr></thead>
            <tbody>
              ${data.runs.map(r => html`
                <tr style="cursor:pointer" class="run-row" data-eid="${escape(r.experiment_id || '')}">
                  <td style="font-size:11px;white-space:nowrap">${(r.timestamp || '').slice(0, 19).replace('T', ' ')}</td>
                  <td style="font-family:monospace;font-size:12px">${escape(r.target_id)}</td>
                  <td>${escape(r.workflow)}</td>
                  <td>${escape(r.model_id)}</td>
                  <td>${r.run_index}</td>
                  <td>${badge(r.evaluation_status)}</td>
                  <td>${r.match_percent != null ? pct(r.match_percent) : '—'}</td>
                  <td>${r.accepted ? '✅' : ''}</td>
                  <td>${r.winner ? '🏆' : ''}</td>
                  <td style="white-space:nowrap">${r.duration_seconds != null ? r.duration_seconds.toFixed(1) + 's' : '—'}</td>
                  <td style="white-space:nowrap;font-size:11px">${r.input_tokens != null ? fmt(r.input_tokens) : '—'} / ${r.output_tokens != null ? fmt(r.output_tokens) : '—'}</td>
                  <td style="white-space:nowrap">${fmtCost(r.cost)}</td>
                </tr>
              `).join('')}
            </tbody>
          </table>
        </div>
      `;

      $$('.run-row').forEach(el => {
        el.addEventListener('click', () => showRunDetail(el.dataset.eid));
      });
    } catch (e) {
      wrap.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
    }
  };

  $('#runFilterBtn').addEventListener('click', load);
  $('#runTargetFilter').addEventListener('keydown', e => { if (e.key === 'Enter') load(); });
  $('#runModelFilter').addEventListener('keydown', e => { if (e.key === 'Enter') load(); });
  $('#runWinnerFilter').addEventListener('change', load);

  load();
}

async function showRunDetail (experimentId) {
  if (!experimentId) return;
  const container = $('#viewContainer');
  container.innerHTML = '<div class="loading">Loading run details…</div>';
  try {
    const [detail, ioData] = await Promise.all([
      api(`/runs/${encodeURIComponent(experimentId)}`),
      api(`/runs/${encodeURIComponent(experimentId)}/io`).catch(() => null),
    ]);

    const recs = detail.records || [];
    const first = recs[0] || {};

    container.innerHTML = html`
      <div style="margin-bottom:16px">
        <button class="filters button secondary" onclick="navigate('runs')" style="padding:6px 12px;border-radius:6px;border:1px solid var(--border);background:var(--bg-hover);color:var(--text);cursor:pointer">← Back to Runs</button>
      </div>

      <div class="card">
        <div class="card-title">Run: ${escape(experimentId)}</div>
        <div style="display:grid;grid-template-columns:repeat(auto-fill,minmax(200px,1fr));gap:12px">
          <div class="stat"><span class="stat-label">Target</span><span style="font-family:monospace">${escape(first.target_id || '—')}</span></div>
          <div class="stat"><span class="stat-label">Workflow</span><span>${escape(first.workflow || '—')}</span></div>
          <div class="stat"><span class="stat-label">Model</span><span>${escape(first.model_id || '—')}</span></div>
          <div class="stat"><span class="stat-label">Status</span><span>${badge(first.evaluation_status)}</span></div>
          <div class="stat"><span class="stat-label">Match%</span><span>${first.match_percent != null ? pct(first.match_percent) : '—'}</span></div>
          <div class="stat"><span class="stat-label">Accepted</span><span>${first.accepted ? '✅ Yes' : '❌ No'}</span></div>
          <div class="stat"><span class="stat-label">Duration</span><span>${first.duration_seconds != null ? first.duration_seconds.toFixed(1) + 's' : '—'}</span></div>
          <div class="stat"><span class="stat-label">Cost</span><span>${fmtCost(first.cost)}</span></div>
          <div class="stat"><span class="stat-label">Input Tokens</span><span>${fmt(first.input_tokens)}</span></div>
          <div class="stat"><span class="stat-label">Output Tokens</span><span>${fmt(first.output_tokens)}</span></div>
          ${first.error ? html`<div class="stat"><span class="stat-label">Error</span><span class="badge badge-error">${escape(first.error)}</span></div>` : ''}
        </div>
      </div>

      ${ioData ? html`
        <div class="card">
          <div class="card-title">Input / Output</div>
          <div class="split-panel">
            <div>
              <div style="font-size:12px;font-weight:600;margin-bottom:8px;color:var(--text-dim)">PROMPT</div>
              <div class="code-block" style="max-height:600px">${escape(ioData.records && ioData.records[0] ? (ioData.records[0].prompt || 'No prompt captured') : ioData.prompt || 'No prompt')}</div>
            </div>
            <div>
              <div style="font-size:12px;font-weight:600;margin-bottom:8px;color:var(--text-dim)">RESPONSE</div>
              <div class="code-block" style="max-height:600px">${escape(ioData.records && ioData.records[0] ? (ioData.records[0].response || 'No response captured') : 'No response')}</div>
            </div>
          </div>
        </div>
      ` : ''}

      <div class="card">
        <div class="card-title">All Records (${recs.length})</div>
        <div class="table-wrap">
          <table>
            <thead><tr>
              <th>Model</th><th>Run</th><th>Status</th><th>Match%</th>
              <th>Accepted</th><th>Winner</th><th>Duration</th><th>Tokens</th><th>Cost</th>
            </tr></thead>
            <tbody>
              ${recs.map(r => html`
                <tr>
                  <td>${escape(r.model_id || r.model)}</td>
                  <td>${r.run_index}</td>
                  <td>${badge((r.evaluation || {}).status)}</td>
                  <td>${(r.evaluation || {}).match_percent != null ? pct((r.evaluation || {}).match_percent) : '—'}</td>
                  <td>${(r.evaluation || {}).accepted ? '✅' : '❌'}</td>
                  <td>${r.winner ? '🏆' : ''}</td>
                  <td>${r.duration_seconds != null ? r.duration_seconds.toFixed(1) + 's' : '—'}</td>
                  <td style="font-size:11px">${fmt(r.input_tokens)}/${fmt(r.output_tokens)}</td>
                  <td>${fmtCost(r.cost)}</td>
                </tr>
              `).join('')}
            </tbody>
          </table>
        </div>
      </div>
    `;
  } catch (e) {
    container.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
  }
}

/* ─── Experiments ─── */

async function renderExperiments (container) {
  container.innerHTML = '<div class="loading">Loading experiments…</div>';
  try {
    const data = await api('/experiments?limit=50');
    const exps = data.experiments || [];

    container.innerHTML = html`
      <div class="card">
        <div class="card-title">Experiments <span style="font-size:12px;color:var(--text-dim)">${exps.length} recent</span></div>
        ${exps.length ? html`
          <div class="table-wrap">
            <table>
              <thead><tr>
                <th>Experiment ID</th><th>Target</th><th>Workflow</th><th>Status</th>
                <th>Records</th><th>Best Status</th><th>Best Match%</th><th>Prompt</th>
              </tr></thead>
              <tbody>
                ${exps.map(e => html`
                  <tr style="cursor:pointer" class="exp-row" data-eid="${escape(e.experiment_id)}">
                    <td style="font-family:monospace;font-size:11px">${escape(e.experiment_id)}</td>
                    <td style="font-family:monospace;font-size:12px">${escape(e.target_id || '—')}</td>
                    <td>${escape(e.workflow || '—')}</td>
                    <td>${badge(e.status)}</td>
                    <td>${e.records_count}</td>
                    <td>${badge(e.best_status)}</td>
                    <td>${e.best_match != null ? pct(e.best_match) : '—'}</td>
                    <td>${e.prompt_exists ? '📄' : '—'}</td>
                  </tr>
                `).join('')}
              </tbody>
            </table>
          </div>
        ` : '<div class="empty">No experiments found</div>'}
      </div>
    `;

    $$('.exp-row').forEach(el => {
      el.addEventListener('click', () => showRunDetail(el.dataset.eid));
    });
  } catch (e) {
    container.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
  }
}

/* ─── Start Run ─── */

async function renderStartRun (container) {
  container.innerHTML = '<div class="loading">Loading options…</div>';
  try {
    const opts = await api('/start-options');

    container.innerHTML = html`
      <div class="card">
        <div class="card-title">▶️ Start a New Run</div>
        <div id="startRunForm">
          <div class="form-group">
            <label>Workflow</label>
            <select id="startWorkflow">
              ${opts.workflows.map(w => `<option value="${escape(w)}">${escape(w)}</option>`).join('')}
            </select>
          </div>

          <div class="form-group">
            <label>Target ID <span class="hint">(or leave empty and use --number for auto-selection)</span></label>
            <input type="text" id="startTargetId" placeholder="e.g. us-800449bc">
          </div>

          <div class="form-group">
            <label>Auto-select count <span class="hint">(--number, requires target ID to be empty)</span></label>
            <input type="number" id="startNumber" placeholder="e.g. 5" min="1">
          </div>

          <div class="form-group">
            <label>Runs per model</label>
            <input type="number" id="startRuns" placeholder="Default from config" min="1">
          </div>

          <div class="form-group">
            <label>TU filter <span class="hint">(--tu, only for new/improve)</span></label>
            <input type="text" id="startTu" placeholder="e.g. kyoshin/cf/CfPadTask">
          </div>

          <div class="form-group" style="display:flex;gap:24px;flex-wrap:wrap">
            <label style="display:flex;align-items:center;gap:6px;cursor:pointer">
              <input type="checkbox" id="startDryRun"> Dry run
            </label>
            <label style="display:flex;align-items:center;gap:6px;cursor:pointer">
              <input type="checkbox" id="startRandom"> Random selection
            </label>
            <label style="display:flex;align-items:center;gap:6px;cursor:pointer">
              <input type="checkbox" id="startCertified"> Certified funcs only
            </label>
            <label style="display:flex;align-items:center;gap:6px;cursor:pointer">
              <input type="checkbox" id="startRetryErrors"> Retry errors
            </label>
          </div>

          <div style="display:flex;gap:8px;margin-top:16px">
            <button id="startRunBtn" style="padding:10px 24px;background:var(--accent);color:#fff;border:none;border-radius:6px;font-weight:600;cursor:pointer">▶️ Start Run</button>
            <button id="startBatchBtn" style="padding:10px 24px;background:var(--bg-hover);color:var(--text);border:1px solid var(--border);border-radius:6px;font-weight:500;cursor:pointer">📦 Batch Mode</button>
          </div>
        </div>
      </div>

      <div class="card">
        <div class="card-title">Available Models</div>
        <div class="table-wrap">
          <table>
            <thead><tr><th>Workflow</th><th>Model</th><th>Provider</th><th>Runs</th></tr></thead>
            <tbody>
              ${Object.entries(opts.models || {}).map(([wf, models]) => models.map(m => html`
                <tr><td>${escape(wf)}</td><td>${escape(m.id)}</td><td>${escape(m.provider)}</td><td>${m.runs}</td></tr>
              `).join('')).join('')}
            </tbody>
          </table>
        </div>
      </div>

      <div id="runOutput"></div>
    `;

    $('#startRunBtn').addEventListener('click', async () => {
      const body = {
        workflow: $('#startWorkflow').value,
        target_id: $('#startTargetId').value.trim() || undefined,
        number: $('#startNumber').value ? parseInt($('#startNumber').value) : undefined,
        runs: $('#startRuns').value ? parseInt($('#startRuns').value) : undefined,
        tu: $('#startTu').value.trim() || undefined,
        dry_run: $('#startDryRun').checked,
        random: $('#startRandom').checked,
        certified_funcs: $('#startCertified').checked,
        retry_errors: $('#startRetryErrors').checked,
      };
      // Remove undefined
      Object.keys(body).forEach(k => body[k] === undefined && delete body[k]);

      const output = $('#runOutput');
      output.innerHTML = '<div class="loading">Starting run…</div>';
      try {
        const res = await api('/runs/start', {
          method: 'POST',
          body: JSON.stringify(body),
        });
        output.innerHTML = html`
          <div class="card">
            <div class="card-title">Run Output <span class="hint">exit code: ${res.exit_code}</span></div>
            ${res.stdout ? html`<div class="code-block" style="max-height:400px">${escape(res.stdout)}</div>` : ''}
            ${res.stderr ? html`<div class="code-block" style="max-height:200px;border-color:var(--accent-red)">${escape(res.stderr)}</div>` : ''}
          </div>
        `;
      } catch (e) {
        output.innerHTML = `<div class="card"><div class="card-title" style="color:var(--accent-red)">Error</div><div class="code-block">${escape(e.message)}</div></div>`;
      }
    });

    $('#startBatchBtn').addEventListener('click', async () => {
      // Batch mode: open simple prompt
      const targetIds = prompt('Enter target IDs (comma-separated):');
      if (!targetIds) return;
      const tu = prompt('TU filter (optional, leave blank if using target IDs):');
      const body = {
        workflow: $('#startWorkflow').value,
        target_ids: targetIds.split(',').map(s => s.trim()).filter(Boolean),
        tu: tu || undefined,
        dry_run: $('#startDryRun').checked,
      };

      const output = $('#runOutput');
      output.innerHTML = '<div class="loading">Starting batch…</div>';
      try {
        const res = await api('/runs/batch', {
          method: 'POST',
          body: JSON.stringify(body),
        });
        output.innerHTML = html`
          <div class="card">
            <div class="card-title">Batch Output <span class="hint">exit code: ${res.exit_code}</span></div>
            ${res.stdout ? html`<div class="code-block" style="max-height:400px">${escape(res.stdout)}</div>` : ''}
            ${res.stderr ? html`<div class="code-block" style="max-height:200px;border-color:var(--accent-red)">${escape(res.stderr)}</div>` : ''}
          </div>
        `;
      } catch (e) {
        output.innerHTML = `<div class="card"><div class="card-title" style="color:var(--accent-red)">Error</div><div class="code-block">${escape(e.message)}</div></div>`;
      }
    });
  } catch (e) {
    container.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
  }
}

/* ─── Batches ─── */

async function renderBatches (container) {
  container.innerHTML = '<div class="loading">Loading batches…</div>';
  try {
    const data = await api('/batches');
    const batches = data.batches || [];

    container.innerHTML = html`
      <div class="card">
        <div class="card-title">📦 Batches</div>
        ${batches.length ? html`
          <div class="table-wrap">
            <table>
              <thead><tr>
                <th>Batch ID</th><th>Workflow</th><th>Status</th>
                <th>Targets</th><th>Success</th><th>Errors</th>
              </tr></thead>
              <tbody>
                ${batches.map(b => html`
                  <tr>
                    <td style="font-family:monospace;font-size:11px">${escape(b.batch_id)}</td>
                    <td>${escape(b.workflow)}</td>
                    <td>${badge(b.status)}</td>
                    <td>${b.target_count}</td>
                    <td style="color:var(--accent-green)">${b.success_count}</td>
                    <td style="color:var(--accent-red)">${b.error_count || ''}</td>
                  </tr>
                `).join('')}
              </tbody>
            </table>
          </div>
        ` : '<div class="empty">No batch runs found</div>'}
      </div>
    `;
  } catch (e) {
    container.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
  }
}

/* ─── IO Browser ─── */

async function renderIOBrowser (container) {
  container.innerHTML = html`
    <div class="card">
      <div class="card-title">📝 IO Browser</div>
      <div class="filters">
        <input type="text" id="ioTargetFilter" placeholder="Target ID…">
        <button id="ioLoadBtn">Load IO</button>
      </div>
      <div id="ioOutput"><div class="empty">Enter a target ID to see its prompt/response data</div></div>
    </div>
  `;

  $('#ioLoadBtn').addEventListener('click', async () => {
    const targetId = $('#ioTargetFilter').value.trim();
    if (!targetId) return;
    const output = $('#ioOutput');
    output.innerHTML = '<div class="loading">Loading IO data…</div>';

    try {
      const data = await api(`/targets/${encodeURIComponent(targetId)}/io?limit=10`);

      if (!data.records || !data.records.length) {
        output.innerHTML = '<div class="empty">No IO data for this target</div>';
        return;
      }

      output.innerHTML = html`
        <div style="font-size:12px;color:var(--text-dim);margin-bottom:8px">${data.total} IO records, showing ${Math.min(10, data.total)}</div>
        ${data.records.map((r, i) => html`
          <div style="margin-bottom:12px;border:1px solid var(--border);border-radius:6px;overflow:hidden">
            <div style="background:var(--bg);padding:8px 12px;font-size:12px;display:flex;justify-content:space-between;border-bottom:1px solid var(--border)">
              <span><strong>#${i + 1}</strong> · ${escape(r.evaluation_status || '?')} · ${r.match_percent != null ? pct(r.match_percent) : '—'} </span>
              <span style="color:var(--text-dim)">${(r.timestamp || '').slice(0, 19).replace('T', ' ')} · ${escape(r.model_id)}</span>
            </div>
            <div class="split-panel">
              <div class="code-block" style="border:none;border-radius:0;max-height:400px;border-right:1px solid var(--border)">${escape(r.prompt || 'No prompt')}</div>
              <div class="code-block" style="border:none;border-radius:0;max-height:400px">${escape(r.response || 'No response')}</div>
            </div>
          </div>
        `).join('')}
      `;
    } catch (e) {
      output.innerHTML = `<div class="empty">Error: ${escape(e.message)}</div>`;
    }
  });

  $('#ioTargetFilter').addEventListener('keydown', e => {
    if (e.key === 'Enter') $('#ioLoadBtn').click();
  });
}

/* ─── Init ─── */

async function init () {
  // Load env badge
  try {
    const progress = await api('/progress');
    const targets = progress.targets || {};
    $('#envBadge').textContent = `${targets.total} targets · ${targets.fully_matched} matched · ${progress.experiments_count} exps`;
  } catch (_) {
    $('#envBadge').textContent = 'disconnected';
  }

  navigate('dashboard');
}

init();

# Agentic LLM decompilation harness — design

Conversational, tool-using successor to the single-shot `llm_harness` solve loop.
The model drives the decompilation loop (compile → hexdiff → revise) itself;
the harness owns file writes, verification, and acceptance.

Status: **design, not implemented**. Discussion captured 2026; see
`tools/llm_harness/README.md` for the current (single-shot) harness.

## 1. Scope and placement

- New package `tools/llm_decomp/` (not a provider mode of `tools/llm_harness`).
  The session loop, patch engine, and concurrency model are different enough
  that shoehorning into `core.py` would entangle two architectures.
- `tools/llm_harness` is **retired**. Vendor the still-useful modules into
  `tools/llm_decomp` (copy, don't import — the old package is deleted):
  `compile_diagnostic`, `candidate_sanitize` (extended into the lint gate),
  `asm_listings` (retail ASM extraction for the brief), `eval_cache`
  stamping, `metrics.TimingRecorder`.
- **No promotion splicing** (`promotion.py` / `source_regions.py` are
  dead): sessions work in the live tree, so the accepted snapshot *is* the
  source. Promotion = `targets.json` update + `configure.py` `Matching`
  flag, both harness-owned.
- **Not reused:** `dossier.py` / `compact_model_facing_dossier`. The
  dossier's failure mode is guessing relevance up front — bloated context
  (type snippets, sibling exemplars) that may not matter. The
  conversational flow inverts this: a slim **brief** (§11) at INIT,
  everything else pulled on demand via `read_file` / `grep`.
- Reuse from `tools/coop`: `hexdiff.py` (JSON mode), `run.py size`,
  `ppc_equivalence` check-unit, `targets.json` claim/release.

## 2. Session lifecycle (state machine)

```
INIT ──► MATCHING ──► ACCEPTED ──► CLEANUP ──► DONE
  │         │            ▲           │  ▲        │
  │         │            │           │  └── cleanup breaks match:
  │         │            │           │      revert-to-snapshot offered,
  │         │            │           │      limited fix turns, else DONE
  │         ├─ submit ──►├─ final check fails ─► DONE (failed session)
  │         └─ budget exhausted ─────────────────► DONE
  └─ claim/setup failure ────────────────────────► DONE (error)
```

States:

| State | Meaning |
|---|---|
| `INIT` | Claim target, snapshot writable files, capture TU baseline, build initial prompt. |
| `MATCHING` | Normal tool loop. Turn/compile budgets apply. |
| `ACCEPTED` | Target symbol verified `FULL_MATCH`/`EQUIVALENT_MATCH` **and** regression sweep clean. Harness snapshots accepted file state immediately — this snapshot is the deliverable regardless of what follows. |
| `CLEANUP` | Optional, bounded (`cleanup_turns`, default 5). Agent may tidy (dead structs, comments, unused helpers). Every patch re-verified. Breaking the match or any regression → revert to accepted snapshot, offer remaining fix turns; on budget end, whichever accepted snapshot stands is final. |
| `DONE` | Terminal. Harness releases claim, finalizes transcript, promotes or discards. |

`submit` is only "end the conversation now". It triggers the full final
check but **never decides acceptance** — acceptance is always automatic on
verification. Exit paths: `submit`, acceptance → cleanup → budget, or
budget exhausted. A session that ends without acceptance is a failed
session; writable files are restored to the pre-session snapshot.

## 3. Writable scope

Per session, the write set is fixed at INIT:

- the owning TU source file (`src/.../<Unit>.cpp`)
- its designated header (TU-local; `create=true` allowed if absent)
- **shared headers are read-only** in v1 (see §7 for the cross-TU path)

Other functions in the TU: registered/exported symbols are not editable.
File-scope helpers **are** editable if they stay unexported — `static`,
`inline`, or anonymous-namespace (enforced via post-build symbol-table diff
+ lint gate, see §11.2). If the agent believes another registered function
is wrong, it reports it in `submit.note`; the orchestrator queues it as a
separate target. Multi-target sessions are rejected in favor of
orchestrator session chaining with a `carryover` summary (§11.3).

## 4. Tools (the whole set)

All tool calls are **provider-native structured JSON** (function-calling
JSON Schema per tool); malformed calls are rejected at the provider layer
and schema violations feed back as tool errors. The `patch` block payload
stays text (SEARCH/REPLACE) inside the JSON envelope — the schema validates
structure, the patch engine validates anchoring.

Core tools below; session-type-specific tools (§10) extend the set.
Read-only tools never touch the build lock; `build` is the only core tool
that acquires it.

### 4.1 `read_file`

```json
{"path": "src/kyoshin/cf/CfPadTask.cpp", "start": 1, "count": 200}
```

- Any path under `src/`, `libs/`, `include/` — **repo-wide**, including
  `src/kyoshin` and every sibling TU, not just the owning unit. Reading
  neighboring units (same-class sibling bodies, call sites) replaces the
  old dossier's fixed "sibling exemplars" section with on-demand lookup.
- Line-ranged. Returns numbered lines + total line count. Read-only set
  enforced by path allowlist, not by sandboxing the model.

### 4.2 `grep`

```json
{"pattern": "class CViewRectData", "path": "include/", "glob": "*.hpp"}
```

- ripgrep over the allowlisted roots; capped result count with truncation
  marker. Exists so the model can *find* things; `read_file` to read them.

### 4.3 `patch`

```json
{
  "files": [
    {"path": "src/kyoshin/cf/CfPadTask.cpp",
     "blocks": [{"search": "<exact text>", "replace": "<new text>"}]},
    {"path": "include/kyoshin/cf/CfPadTask.hpp", "create": true,
     "content": "<full file>"}
  ]
}
```

- Applied immediately, in order. All-or-nothing: any SEARCH block that does
  not match **exactly once** in current file state rejects the entire call
  (returned as tool error naming the failing block index + nearest anchor
  hint). No partial application.
- `create=true` fails if the file exists; without it, at least one block is
  required.
- After applying, the **lint gate** (§5) runs on final file content; a lint
  failure reverts the patch and returns the violated rules.
- Post-patch, returns the changed line ranges so the model can re-orient
  (SEARCH/REPLACE drifts after self-edits; echoing the new state keeps
  anchors honest).

### 4.4 `build`

```json
{"unit": "kyoshin/cf/CfPadTask"}
```

- Acquires the global build lock, runs ninja for the unit, releases.
- Returns cleaned diagnostics (`compile_diagnostic.py` formatting) and
  success/failure. Compile errors are tool results, not session failures.

### 4.5 `diff`

```json
{"unit": "kyoshin/cf/CfPadTask", "symbol": "<mangled>", "mode": "target|sweep"}
```

- Assumes a successful `build` (refuses on stale object — tracks last
  built content hash per unit).
- `target`: hexdiff JSON for one symbol — `mismatch_count`,
  reg-swap vs structural split, register mapping table.
- `sweep`: every defined symbol vs the INIT baseline, parallel; used by the
  harness at verification, also available to the agent (rate-limited,
  `max_sweeps` 5 per session for agent-initiated calls; harness sweeps
  exempt) for self-checking header changes.

### 4.6 `submit`

```json
{"note": "matched; sibling func_80... looks wrong, worth its own target"}
```

- Ends the conversation → final verification → DONE.
- `note` is free-text appended to `attempts.jsonl`.

### 4.7 `equivalence` (v1.1, not v1)

```json
{"unit": "...", "symbol": "..."}
```

- `ppc_equivalence` check-unit for the fuzzy-[50,100) band. Deferred
  because v1 targets the FULL_MATCH-or-keep-iterating loop; the harness
  still runs equivalence internally at verification time.

Deliberately absent: objdiff CLI, `cycle`, `targets.json` writes, git ops,
promotion. All harness-owned.

## 5. Lint gate (anti-cheat, runs on every `patch`)

Prompt rules are advisory; the gate is enforcement. Rejects (patch reverted,
rule list returned):

- `asm` / `__asm`, `asm void`, `.s` includes
- `register` keyword, `asm("rN")` bindings, identifiers matching `^r([0-9]|[12][0-9]|3[01])$` for params/locals
- `extern "C"` except the approved reloc-name pattern (`lbl_`/approved globals)
- `void*` parameters/casts where a struct type is expressible
- `volatile` byte arrays, fake stack buffers (`u8 sp[...]`)
- raw offset arithmetic (`*(u32*)((u8*)p + 0xNN)`) when the struct field exists in a known type
- `DECOMP_PPC_*`, `DECOMP_FORCELITERAL`, `DECOMP_FORCEACTIVE*` — forbidden
  **unconditionally** for the LLM (no §17.6 exception; that hatch is
  human-only). If an opcode-selection gap (e.g. `rlwinm` vs shift) can't be
  closed with clean C, the agent reports it in `submit.note` —
  `ppc_equivalence` can still carry the function to `EQUIVALENT_MATCH`
- edits outside the writable scope (belt-and-braces; the patch engine already refuses)
- signature drift: the target function's signature must remain the locked
  dossier signature (parsed and compared)

The violated rule names go back as tool feedback; models correct in one turn.

**Scope of linting:** patch-introduced content only, never whole files —
scaffold-TU stub bodies (`extern "C" u8 func_80...(void* self)`) violate
half the rules by design, and whole-file linting would fail every
un-matched TU before the agent acts.

### Legacy scaffold markers (removed)

`// LLM-HARNESS-BEGIN/END: <target-id>` markers and banner comments
(`// Auto-scaffolded catalog TU`, `// Mangled extern stubs…`) are
**stripped repo-wide** in the llm_harness retirement — they were only
display/scaffolding conveniences for the old tooling. Stub *bodies* stay
as plain functions until matched (they keep scaffold TUs compiling). The
patch engine anchors on **symbol name** (locate the `func_80…` / mangled
definition in the TU) instead of markers.

## 6. Verification pipeline

### INIT baseline (once per session)

1. Snapshot writable files (in-memory copies; restore = write back).
2. Build the unit (build lock).
3. hexdiff every defined symbol → `baseline[symbol] = mismatch_count`;
   record split `.text` budget + current size; cache the object hash.

### Per `submit` (and per cleanup patch)

1. Lint gate on current files (already run per-patch; re-run cheap).
2. Build lock → ninja TU → release.
3. Parallel fan-out (thread pool):
   - hexdiff target symbol
   - hexdiff sweep of all other defined symbols vs baseline
   - split `.text` size check
   - equivalence probe (if target fuzzy ∈ [50,100))
4. Verdict:
   - **accept** = target `FULL_MATCH`, or fuzzy ≥ 50 + SMT `EQUIVALENT`
     under effect-aware policy, **and** zero sibling regressions, **and**
     size within budget.
   - Already-matched siblings (baseline 0) that regress → **hard reject,
     auto-revert to last-good file state**, conversation continues.
   - Unmatched sibling got worse → soft reject: agent must fix or revert
     its own change before any acceptance.
5. Verdict JSON back to the agent (target mismatches, regression list with
   baseline-vs-now, size, rule).

### Cross-TU (shared-header) path — v1.1

v1: shared headers read-only, so cross-TU regressions are impossible by
construction. v1.1: unlock shared-header writes; patch application consults
an include graph (precomputed from compile_commands / `ninja -t deps`) and
the verification sweep extends to dependent units per the tiered policy in
§11.4 (full sweep ≤10 dependents, accepted-symbols-only diff ≤40, refuse
above). Same verdict rules, wider sweep; a broken build in any dependent
is always a hard reject.

## 7. Concurrency model

**Parallel sessions are the default operating mode** (`target_parallel`),
with the scheduler assigning targets on **disjoint TUs/headers** so write
locks never contend in practice. Two resource classes, one lock:

- **Global build lock (mutex):** `ninja`, `configure.py`, objdiff report
  generation — anything writing under `build/`. Held for the shortest
  possible window.
- **Read-only, parallel:** hexdiff (`--no-build`), SMT/equivalence,
  symbol/KB lookups, lint, patch application. Thread pool, sized by config
  (`workers`, default 8).

No worktrees. Sessions run in the live tree; isolation comes from:

- **write locks per path**: a session holds locks on its TU + header; a
  second session needing a locked path is skipped (scheduler picks another
  target). Contention is expected to be rare (disjoint TUs); revisit
  worktrees only if profiling says otherwise.
- harness-mediated writes only; rollback = restore the two snapshots.

`target_parallel` sessions share the build lock; the common case (several
agents, disjoint units) serializes only on seconds-long builds. The
scheduler skips a candidate target whose TU or designated header is
write-locked by a live session and picks the next frontier target instead.

### Include graph (built in v1)

MWCC emits no gcc-style depfiles, so the graph is self-built, not
ninja-derived:

- **Scanner:** regex `#include` lines over `src/`, `libs/`, `include/`,
  resolved against include dirs from `configure.py` cw flags. Both
  directions: unit → includes and header → dependents.
- **Cache:** `build/llm-decomp/include_graph.json`; staleness check at
  session INIT (mtime/content hash of scanned roots). After a header edit,
  only that file's entry is re-parsed — incremental updates are trivial.
- SDK/system includes recorded but excluded from dependent sweeps.

Built in v1 even though shared-header writes stay locked: the graph also
feeds dossier assembly (right type snippets) and the dependent-count tiers
(§11.4) later.

## 8. Transcript log

Per session, append-per-turn (fsync per append; survives crashes):

`build/llm-decomp/<target-id>/<session-id>/conversation.jsonl`

```json
{"turn": 0, "role": "system", "config_hash": "...", "prompt_chars": 4210}
{"turn": 1, "role": "user", "kind": "dossier", "chars": 18300}
{"turn": 2, "role": "assistant", "text": "...", "tool_calls": [{"name": "patch", "args_digest": "...", "blocks": 2}], "tokens": {"in": 19000, "out": 900}}
{"turn": 2, "role": "tool", "name": "patch", "ok": true, "changed_ranges": [[120, 158]], "duration_s": 0.02}
{"turn": 9, "role": "harness", "kind": "verdict", "accepted": true, "target": {"mismatches": 0}, "regressions": [], "size": {"text": 1240, "budget": 1280}}
```

Final record: outcome, accepted snapshot paths, promotion result, totals
(turns, builds, tokens, wall time, per-phase timings via `TimingRecorder`).

## 9. Configuration reference (`llm-decomp.json`)

Everything tunable lives in one checked-in JSON file. Precedence:
`session_types.<type>` overrides top-level defaults.

```json
{
  "models": {
    "default": {"provider": "openrouter", "model": "...", "max_tokens": 8192, "thinking_budget": null}
  },
  "session_types": {
    "match":         {"model": null, "budgets": {"max_turns": 25, "max_builds": 15, "max_sweeps": 5, "max_patch_failures": 4, "cleanup_turns": 5}},
    "type-recovery": {"model": null, "budgets": {"max_turns": 20, "max_builds": 8}},
    "rename":        {"model": null, "budgets": {"max_turns": 10, "max_builds": 6}},
    "tu-cleanup":    {"model": null, "budgets": {"max_turns": 20, "max_builds": 8}},
    "size-trim":     {"model": null, "budgets": {"max_turns": 20, "max_builds": 12}}
  },
  "concurrency": {"workers": 8, "target_parallel": 4},
  "prompt": {"max_chars": 60000, "include_raw_hex": false},
  "verification": {"equivalence_min_fuzzy": 50, "cross_tu_full_sweep_max": 10, "cross_tu_refuse_over": 40},
  "stop": {"max_repeated_fingerprint": 3, "max_compile_error_streak": 4},
  "pipeline": {"auto_size_trim": true, "ledger": "tools/llm_decomp/tu_ledger.json"},
  "execution": {"auto_promote": true, "auto_promote_owner": "llm-decomp"}
}
```

### `models`

| Setting | Default | Effect |
|---|---|---|
| `models.default.provider` | — | Backend adapter id (§13). |
| `models.default.model` | — | Model slug passed to the provider. |
| `models.default.max_tokens` | 8192 | Completion cap per turn. `"unlimited"` omits the cap. |
| `models.default.thinking_budget` | null | Reasoning-token cap for thinking models; `"unlimited"` for none. |
| `session_types.<type>.model` | null | Full model override for that session type (e.g. strong model for `match`, cheap model for `tu-cleanup`/`rename`). null → `models.default`. |

### `session_types.<type>.budgets`

| Setting | Default | Effect |
|---|---|---|
| `max_turns` | 25 | Hard cap on assistant turns per session. |
| `max_builds` | 15 | Cap on `build` tool calls (each holds the global build lock). |
| `max_sweeps` | 5 | Cap on agent-initiated `diff mode=sweep` calls; harness sweeps exempt (§14.1). |
| `max_patch_failures` | 4 | Consecutive unparseable/unanchoring `patch` calls before the session ends — protects against anchor drift loops. |
| `cleanup_turns` | 5 | Turns allowed in CLEANUP after acceptance (§2). |

### `concurrency`

| Setting | Default | Effect |
|---|---|---|
| `workers` | 8 | Thread-pool size for read-only verification fan-out (hexdiff, SMT). |
| `target_parallel` | 4 | Max concurrent sessions (disjoint TUs enforced by the scheduler). |

### `prompt`

| Setting | Default | Effect |
|---|---|---|
| `max_chars` | 60000 | Cap on assembled brief + tool-result sizes; oversized retail ASM listings are truncated with a marker. |
| `include_raw_hex` | false | Include raw instruction bytes alongside disassembly in the brief. |

### `verification`

| Setting | Default | Effect |
|---|---|---|
| `equivalence_min_fuzzy` | 50 | Fuzzy % at/above which the SMT equivalence probe runs at verification (below → keep matching). |
| `cross_tu_full_sweep_max` | 10 | §11.4/§14.4 tier: dependent-count at/below which every dependent unit gets built + diffed. |
| `cross_tu_refuse_over` | 40 | Dependent-count above which a shared-header edit is refused outright. |

### `stop`

| Setting | Default | Effect |
|---|---|---|
| `max_repeated_fingerprint` | 3 | Identical mismatch fingerprint (count + diff hash) this many times in a row → nudge once, then end the session. |
| `max_compile_error_streak` | 4 | Consecutive failed builds with no progress → end session (escalate model or target). |

### `pipeline`

| Setting | Default | Effect |
|---|---|---|
| `auto_size_trim` | true | Insert a `size-trim` stage automatically whenever a stage reports the unit over its split budget. |
| `ledger` | `tools/llm_decomp/tu_ledger.json` | Checked-in TU completion ledger (§12). |

### `execution`

| Setting | Default | Effect |
|---|---|---|
| `auto_promote` | true | On acceptance, harness updates `targets.json` (+ `configure.py` `Matching` at full-TU) without manual review. |
| `auto_promote_owner` | `"llm-decomp"` | Owner string recorded for claims/promotions. |

Stop conditions also include the unvalidated-callee block (target has
unresolved/indirect callees → session ends, orchestrator re-queues after
callees are accepted).

## 10. Session types

One generic loop; a session type is a policy object:

```python
SessionPolicy(
    name="match",
    writable=[OwningTU, DesignatedHeader],
    tools=[read_file, grep, patch, build, diff, submit],
    gate=MatchGate(),
    prompt_template="prompts/match.md",
    budgets=SessionBudgets(max_turns=25, max_builds=15),
)
```

The loop, transcript, budgets, concurrency, and verification fan-out are
shared; a new type is ~50 lines of policy + a prompt template.

### Catalog

| Type | Scope | Gate |
|---|---|---|
| `match` (core) | target fn + unexported helpers + TU header | FULL/EQUIVALENT + zero regressions + size |
| `tu-cleanup` | whole TU + its header | all symbols byte-identical to baseline + size; commenting, local renames, dead-code/stub-comment removal only |
| `type-recovery` | header only (+ read-only asm/xrefs) | TU builds, zero symbol regressions; model `UnkClass_*` structs/classes without touching bodies |
| `rename` | `rename_symbol` tool only (no raw `patch`) | build + all affected symbols still match post-rename |
| `size-trim` | whole TU, accepted symbols locked | accepted symbols stay accepted, `.text` shrinks toward budget |

### Per-type lifecycle notes

- **`match`** — as designed: dossier prompt, target symbol, acceptance
  snapshots.
- **`tu-cleanup`** — INIT baseline captures every symbol's byte state; the
  gate is *stricter* than match: mismatch delta exactly 0 in both
  directions + size within budget. Lint gate gains cleanup-mode heuristics
  (flag statement-level/control-flow edits; declarations, names, comments
  pass). This is also where style-guideline polish lands (naming,
  method-vs-free-function, comment quality) under the byte-identical gate.
- **`type-recovery`** — writable scope is header-only; prompt carries
  Ghidra/asm exports + xref data for the `UnkClass_*` being modeled. Pure
  header additions shouldn't change codegen at all, so the sweep turns
  layout mistakes into immediate feedback.
- **`rename`** — symrecover is harness-mediated; the agent never gets a
  shell. Two extra tools replace `patch` in this session type:
  - **`symbols`** — read-only wrapper over symrecover `show` / `xref` /
    `demangle` (methods, split unit, mangled signatures).
  - **`rename_symbol(old, new)`** — validated pipeline: `rename-plan`
    (mangling-length compatibility) → identifier/namespace checks
    (`KNOWN_NS_PREFIXES`) → snapshot affected file set → `rename-all` →
    reconfigure + rebuild affected units (build lock) → re-diff every
    affected symbol in parallel. All match → done; any regression →
    restore snapshot, return failing symbols as tool feedback.
- **`size-trim`** — accepted symbols locked (byte-identical or still
  accepted); gate rewards `.text` reduction toward the split budget.

No separate "equivalence-closure" type — that is `match` with a different
starting fuzzy.

### Recommended pipeline order (per unit)

```
type-recovery → match×N (chained, leaves-first) → rename → tu-cleanup
      │                                            (unit fully matched,
      └────────────── re-enter if a match session  size-trim any time the
                      stalls on an unknown type    split budget is tight)
```

1. **`type-recovery` first** when the unit's types are `UnkClass_*` —
   struct models are the highest-leverage artifact; every later `match`
   session inherits them via carryover.
2. **`match` sessions chained** across the unit (callees-accepted frontier
   order), carryover accumulating the type model and failed hypotheses.
3. **`rename` once the unit is fully matched** — per the existing
   symrecover workflow rule (match first, rename after).
4. **`tu-cleanup` last** — commenting and hygiene on stable, matched code.
5. **`size-trim` on demand** whenever `coop run size` flags the unit.

## 11. Initial prompt (the brief)

Replaces the dossier. **Markdown, not JSON** — JSON prompts cost tokens
(braces, escaping) and add nothing for a document the model only reads.
Consistent split: LLM-facing content is markdown (brief, verdicts, tool
feedback, carryover); machine-facing content is JSON (tool calls,
transcript log).

Fixed sections, only what cannot be fetched on demand:

- `## Target` — target-id, mangled/demangled name, **locked signature**
- `## Retail ASM` — read-only reference listing (via `asm_listings`)
- `## Writable scope` — TU path, designated header path
- `## State` — baseline `mismatch_count`, fuzzy, split `.text` size/budget
- `## Rules` — lint-gate rules, style-doc reference, submit/acceptance
  contract
- `## Carryover` — when chained (§12.3)

No type snippets, no sibling exemplars, no KB dump — the agent fetches
exactly what it needs with `read_file` / `grep`. Expected size: a few KB
vs the dossier's ~18k chars.

### Style guidelines

Live in the existing **`docs/coding_style_guidelines.md`** (extended with a
decompilation section — typedefs, naming, includes, `.clang-format`
already covered there). Decomp-specific rules:

- **no assembly, at all** — no `asm`/`__asm` blocks, no inline asm, no
  `.s` includes (project policy bans asm in matched source, humans
  included). Same for
  codegen-steering macros (`DECOMP_PPC_*` intrinsics, `DECOMP_FORCE*`) —
  residual opcode gaps are covered by `ppc_equivalence` instead
- proper types (structs/classes); no pointer arithmetic
- members written as methods, not free functions with a `this`-like param
- human-readable names **only when understood**; unknown struct fields
  keep offset names (`field_0x1C`) — a wrong guess is worse than no name
- no `extern "C"` outside the approved reloc-name pattern (stubs aside)
- no dead helpers/scratch code left after matching; no `goto` outside
  `goto`; project typedefs (`u8`/`s32`/`f32`), not `uint32_t`/`int`

Enforced at three points, not just pipeline end: **lint gate** (per-patch,
mechanical subset), **brief/prompt** (style-doc reference in every
session), **`tu-cleanup`** (final human-level polish under the
byte-identical gate). End-only enforcement would let match sessions
produce matching-but-ugly code and put the match at risk during cleanup.

## 12. Pipeline command

Runs the full session-type pipeline (§10) on a whole TU:

```bash
python3 tools/llm_decomp/run.py pipeline --tu kyoshin/CExchangeWin
python3 tools/llm_decomp/run.py pipeline --number 4        # auto-select TUs
python3 tools/llm_decomp/run.py pipeline --tu X --resume
```

Orchestrator semantics:

- Stages: `type-recovery? → match×N (chained, frontier order) → rename? →
  tu-cleanup`. Stages marked `?` are skipped when vacuous (no `UnkClass_*`
  to model; nothing unnamed to rename). `size-trim` is inserted
  automatically whenever a stage reports the unit over budget.
- One pipeline per TU; multiple TUs run as parallel pipelines under the
  normal concurrency model (disjoint TUs, shared build lock).
- Stage gates: a stage only starts when the previous stage's gate passed;
  a failed stage leaves the pipeline resumable (`--resume` picks up at the
  failed stage; pipeline state lives in
  `build/llm-decomp/pipeline/<unit>/state.json` with the carryover chain).
- `match` stage iterates the unit's targets in callees-accepted/leaf
  frontier order, chaining sessions (carryover) and re-entering
  `type-recovery` if a session stalls on an unknown type.

### TU completion tracking

Two layers, following the repo's current-state/append-only pattern:

- **Checked-in ledger** `tools/llm_decomp/tu_ledger.json` — per-TU current
  pipeline state: stages completed (`type-recovery`/`match`/`rename`/
  `tu-cleanup`/`size-trim`), timestamps, session ids, carryover chain
  summary. Auto-selection **skips ledgered-complete TUs**; `--resume`
  reads it to pick the right stage. It records what `targets.json` can't:
  non-function stages (rename/cleanup) and pipeline-level notes.
- **Working state** `build/llm-decomp/pipeline/<unit>/state.json` —
  in-flight resume detail; ephemeral, safe to delete (ledger is canonical).

Function-level acceptance stays in `targets.json` (sole source of truth);
the ledger never duplicates per-function status, it references it.

### TU selection for auto ingestion (`--number`)

From `targets.json`, grouped by TU, ranked:

1. **Partially-matched TUs first** — carryover value is highest; finish
   what was started.
2. **Frontier density** — TUs whose remaining targets are mostly
   leaf/callees-accepted (`ready`) beat blocked ones.
3. **Priority tiers** — P0/P1 before the rest.
4. **Smallest remaining work** — fewest/smallest NOT_STARTED targets,
   for quick full-TU wins.

Excluded: TUs complete in the ledger, `buildable=no` units, write-locked
TUs (live session), TUs whose targets are all accepted (they go straight
to a rename/cleanup-only pipeline), units with unresolved/indirect-call
targets outside safe frontiers.

## 13. Provider interface

One role (`agent`), conversation-native. Any provider that supports tool
calling + multi-turn; the OpenCode/Codex `--pure` single-shot path from the
old harness does not apply here. Thin adapter per backend:

```
send(messages, tools) -> {text, tool_calls[], usage}
```

The harness drives the loop; the provider never sees the filesystem.

## 14. Resolved decisions (was: open questions)

1. **Agent-initiated `sweep` is capped at 5/session**; harness-initiated
   sweeps at submit/acceptance don't count. On exhaustion the tool returns
   a warning ("submit to get a full check") instead of hard-failing — the
   submit-time sweep is the real gate.
2. **Sibling-edit exception is symbol-table enforcement, not judgment**:
   the agent may freely create/edit file-scope symbols that are not in
   `targets.json` and not exported (`static`, `inline`, anonymous
   namespace). At each verification the post-build symbol table is diffed
   against the INIT snapshot — any new exported symbol or any modified
   registered symbol other than the target is a hard reject. Lint gate
   additionally rejects new non-static free functions in the TU ("make it
   static or header-inline"). Helpers worth registering become separate
   targets later via `submit.note`.
3. **No multi-target sessions.** One target per session keeps claims,
   snapshots, budgets, transcripts, and the bounded-attempt protocol clean.
   Context-reuse is recovered by orchestrator-level **session chaining**:
   the next session on the same TU starts with a `carryover` field (prior
   outcome, established struct shapes, failed hypotheses, ~10 lines) and
   the on-disk file state already reflects the previous win. Rationale:
   cold-start economics — a fresh session otherwise burns 5–10 tool turns
   and tens of thousands of tokens re-reading the TU to rebuild context
   (struct model, sibling conventions, failed MWCC patterns) that a ~500
   token `carryover` delivers immediately, with the previous win's type
   definitions already on disk.
4. **Cross-TU sweep tiers by include-graph dependent count** of the edited
   header: 0 → TU sweep only; 1–10 → full dependent sweep (build + diff
   all); 11–40 → build all dependents (compile regressions always checked)
   but diff only units with ≥1 accepted baseline symbol; >40 → edit refused
   (restructure into a TU-local header). Broken build in any dependent is
   always a hard reject. Threshold 40 is a placeholder pending the real
   include graph.

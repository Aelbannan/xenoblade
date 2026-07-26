# Agentic LLM decompilation harness — design

Conversational, tool-using successor to the single-shot `llm_harness` solve loop.
The model drives the decompilation loop (compile → hexdiff → revise) itself;
the harness owns file writes, verification, and acceptance.

Status: **design, not implemented**. Discussion captured 2026; see
`tools/llm_harness/README.md` for the current (single-shot) harness.

## 1. Scope and placement

- New package `tools/llm_agent/` (not a provider mode of `tools/llm_harness`).
  The session loop, patch engine, and concurrency model are different enough
  that shoehorning into `core.py` would entangle two architectures.
- Reuse from `tools/llm_harness` via import: `compile_diagnostic`,
  `candidate_sanitize` (extended into the lint gate), `dossier` /
  `compact_model_facing_dossier` (initial prompt), `eval_cache` stamping,
  `metrics.TimingRecorder`, promotion splicing in `promotion.py` /
  `source_regions.py`.
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

Five tools. Read-only tools never touch the build lock; `build` is the only
tool that acquires it.

### 4.1 `read_file`

```json
{"path": "src/kyoshin/cf/CfPadTask.cpp", "start": 1, "count": 200}
```

- Any path under `src/`, `libs/`, `include/`. Line-ranged.
- Returns numbered lines + total line count. Read-only set enforced by path
  allowlist, not by sandboxing the model.

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
- `DECOMP_PPC_*` / `DECOMP_FORCELITERAL` outside §17.6-logged exceptions
- edits outside the writable scope (belt-and-braces; the patch engine already refuses)
- signature drift: the target function's signature must remain the locked
  dossier signature (parsed and compared)

The violated rule names go back as tool feedback; models correct in one turn.

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

Two resource classes, one lock:

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
agents, disjoint units) serializes only on seconds-long builds.

## 8. Transcript log

Per session, append-per-turn (fsync per append; survives crashes):

`build/llm-agent/<target-id>/<session-id>/conversation.jsonl`

```json
{"turn": 0, "role": "system", "config_hash": "...", "prompt_chars": 4210}
{"turn": 1, "role": "user", "kind": "dossier", "chars": 18300}
{"turn": 2, "role": "assistant", "text": "...", "tool_calls": [{"name": "patch", "args_digest": "...", "blocks": 2}], "tokens": {"in": 19000, "out": 900}}
{"turn": 2, "role": "tool", "name": "patch", "ok": true, "changed_ranges": [[120, 158]], "duration_s": 0.02}
{"turn": 9, "role": "harness", "kind": "verdict", "accepted": true, "target": {"mismatches": 0}, "regressions": [], "size": {"text": 1240, "budget": 1280}}
```

Final record: outcome, accepted snapshot paths, promotion result, totals
(turns, builds, tokens, wall time, per-phase timings via `TimingRecorder`).

## 9. Budgets and config

`llm-agent.json`:

```json
{
  "session": {
    "max_turns": 25,
    "max_builds": 15,
    "max_sweeps": 5,
    "cleanup_turns": 5,
    "max_patch_failures": 4
  },
  "concurrency": {"workers": 8, "target_parallel": 4},
  "prompt": {"max_chars": 60000, "include_raw_hex": false},
  "models": {"agent": [{"id": "...", "provider": "...", "model": "..."}]},
  "execution": {"auto_promote": true, "auto_promote_owner": "llm-agent"}
}
```

Stop conditions mirror the current harness where applicable: repeated
mismatch fingerprint (same `mismatch_count` + diff hash 3× → nudge, 4× →
end), all-compile-error streak, unvalidated-callee block.

## 10. Provider interface

One role (`agent`), conversation-native. Any provider that supports tool
calling + multi-turn; the OpenCode/Codex `--pure` single-shot path from the
old harness does not apply here. Thin adapter per backend:

```
send(messages, tools) -> {text, tool_calls[], usage}
```

The harness drives the loop; the provider never sees the filesystem.

## 11. Resolved decisions (was: open questions)

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
   the on-disk file state already reflects the previous win.
4. **Cross-TU sweep tiers by include-graph dependent count** of the edited
   header: 0 → TU sweep only; 1–10 → full dependent sweep (build + diff
   all); 11–40 → build all dependents (compile regressions always checked)
   but diff only units with ≥1 accepted baseline symbol; >40 → edit refused
   (restructure into a TU-local header). Broken build in any dependent is
   always a hard reject. Threshold 40 is a placeholder pending the real
   include graph.

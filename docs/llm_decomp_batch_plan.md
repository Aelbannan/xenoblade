# Batch-target matching plan for tools/llm_decomp

Upgrade the pipeline so one session matches N targets in a TU, amortizing
the per-session fixed costs: baseline capture (one full TU build), the
brief (source + header resend, ~7K tokens), system prompt, and session
init. With N=5 that saves 4 baseline builds and 4 brief resends per group.

Batching is a **new session type** (`batch-match`). The single-target
`match` path stays intact and is reused for retries (batch_size=1 is the
degenerate case).

## Key simplifications vs the draft

1. **No address-range locking (drop draft §2c, `symbol_address_ranges`).**
   Mapping text SEARCH/REPLACE edits to ELF address ranges is fragile
   (function reordering, size shifts, inlining). The existing auto-verify
   sweep already catches an accepted target's bytes changing — post-build,
   not pre-patch — and the restore mechanism reverts. Same protection,
   zero new machinery.
2. **No separate accepted-symbol overlay + `check_accepted_regressions()`.**
   On accept, mutate the effective baseline in place:
   `baseline.symbols[symbol] = SymbolBaseline(byte_hash=accepted_hash, mismatch_count=0)`.
   The existing `sweep()` + `sibling_regression` logic then does exactly
   the right thing — any later byte change to an accepted target fires
   HARD_REJECT automatically. No new check code.
3. **No per-file restore (drop draft §2g).** On HARD_REJECT restore the
   whole writable set from the *latest accepted snapshot* if one exists,
   else INIT. Reverting pending targets' in-progress edits is acceptable —
   they may have caused the regression.
4. **No `AcceptedCheckpoint` dataclass with hashes in JSON.** Reuse the
   existing accepted-snapshot dir; append plain dict entries to an
   `accepted.json` manifest and save the accepted `.o`. Hashes are
   recomputed from the `.o` if ever needed.
5. **Minimal tool-schema churn:** only `submit` gains a required
   `target_id` (enum-constrained to the batch). `diff` keeps its free-form
   `symbol` param; `patch` is unchanged.
6. ~~**`cleanup_turns: 0` for batch-match**~~ — **superseded (review R1):**
   setting the budget to 0 makes the existing loop break at the first
   accept. Instead, `BatchSession` overrides accept handling to stay in
   MATCHING and never enter CLEANUP. Accept → next target.

## Phase 0: correctness prerequisites (also latent-bug fixes)

These fix bugs that exist *today* in the single-target flow; batching just
makes them fatal.

### 0a. Promote accepted state into the baseline (`session.py`, no verify.py change)

Bug: `sweep()` compares against the INIT baseline forever. Two consequences:

- *Batch:* after target A accepts, an edit regressing A reads as an
  improvement vs INIT (30→12) — no regression fires.
- *Today, single-target:* during CLEANUP turns, a patch breaking the
  accepted match (0→5 mismatches vs an INIT baseline of 30) yields
  `not_matched` SOFT_REJECT — and `_handle_verdict` only restores the
  accepted snapshot on HARD_REJECT. The match can be silently broken in
  cleanup despite the "every patch is re-verified" promise.

Fix: on accept, before entering CLEANUP / moving to the next target,
overwrite `self.baseline.symbols[symbol]` with the accepted hash and
`mismatch_count=0`. Any later byte change → `was_matched=True` →
HARD_REJECT → restore accepted snapshot. One-line change, no new
verdict paths.

### 0b. Crash-safe accepted state (`state.py`)

Bug: `reconcile()` restores from INIT unconditionally. Crash mid-CLEANUP
(or mid-batch after 4/7 accepts) loses verified accepts even though the
accepted snapshot exists on disk.

Changes:

- `reconcile()`: if `paths.accepted_snapshot_dir(sdir)` exists and is
  non-empty, restore writable files from *it* instead of INIT.
- New: `record_accepted_entry(sdir, entry: dict) -> None` /
  `load_accepted_entries(sdir) -> list[dict]` — appends to
  `accepted.json` in the session dir:
  `{target_id, symbol, rule, fuzzy, ts}`. Written (fsynced) at each
  accept, together with the existing `record_accepted()` file snapshot
  and a copy of the accepted `.o` (`accepted.o`).
- Batch sessions call this per accepted target. Single-target sessions
  get it for free (one entry).

## Phase 1: batch brief (`brief.py`)

- Add `build_batch_brief(*, targets: list[TargetBrief], ...)` where
  `TargetBrief = (target_id, symbol, demangled, signature, retail_asm)`.
- Layout: heading → writable scope → source file (once) → header (once)
  → per-target blocks (id, mangled symbol, locked signature, retail ASM)
  → state (shared TU baseline stats) → rules → carryover.
- ASM budget: compute headroom as today, divide by N, truncate each
  target's ASM independently with the existing `_truncate_asm`.
- Rules section gains: "Submit each target separately with
  `submit(target_id)`. Accepted targets are frozen — a patch that changes
  an accepted target's compiled bytes is reverted."
- Each target block header states the exact `target_id` string the model
  must echo in `submit` (enum in the schema constrains it anyway).

## Phase 2: batch session (`session.py`, `tools.py`, `verify.py`)

### 2a. State model

Add to `Session` (or a `BatchSession` subclass — prefer subclass to keep
the single-target loop readable):

```python
self.targets: dict[str, TargetRun]  # target_id -> TargetRun

@dataclass
class TargetRun:
    target_id: str
    symbol: str
    demangled: str
    signature: str
    retail_asm: str
    status: str = "pending"          # pending | accepted | failed
    best_mismatches: int | None = None
    submit_failures: int = 0         # consecutive failed submits
```

Stays session-wide (scalars): `_dirty`, `baseline` (one TU baseline,
mutated per 0a), builds/sweeps/token counters, compile_error_streak,
`patch_failures` (anchoring failures are not target-specific).

### 2b. Tools

- `submit` schema gains required `target_id` (enum = batch ids) under the
  `batch-match` policy only. `dispatch()` routes it into
  `data={"submitted": True, "target_id": ...}`.
- `diff`: unchanged (`symbol` param already exists; batch `meta.symbol`
  is `""`, so the model must pass a symbol — it always does in practice).
- `patch`: unchanged.

### 2c. Per-target verify-and-accept

On `submit(target_id)`:

1. `verdict_for_submit(repo, unit, symbol, baseline, pending_symbols=...)`
   (see 2d).
2. Accept → apply 0a baseline promotion; `state.record_accepted()` +
   save `accepted.o`; `state.record_accepted_entry(...)`;
   `targets[id].status = "accepted"`; feedback names the next pending
   target. Continue the loop.
3. Reject → `submit_failures += 1`; at `max_submit_failures` (new budget,
   default 3) mark the target `failed` and tell the model to move on.

### 2d. Verdict change (`verify.py`)

`verdict_for_submit(..., pending_symbols: set[str] | None = None)`:

- `sibling_worse` soft-reject skips symbols in `pending_symbols` (batch
  members the model hasn't worked on yet — expected to drift), **except
  when `current_mismatches == -1`** (symbol missing from the object:
  deleted/renamed/inlined). A missing batch member is always ≥
  SOFT_REJECT — otherwise deleted-target damage gets snapshotted and
  promoted (review R2).
- `sibling_regression` needs no change: INIT-matched siblings and
  accepted targets (via 0a) already have `mismatch_count == 0` in the
  effective baseline.

### 2e. Termination and restore

- Loop ends when every target is `accepted`/`failed`
  (`reason="batch_complete"`), or on the existing budget/streak exits.
- HARD_REJECT restore: accepted snapshot if it exists, else INIT (whole
  writable set).
- Session-end restore: accepted snapshot if any target accepted, else
  INIT.
- `SessionOutcome` gains `accepted_ids: list[str]`, `failed_ids: list[str]`.

### 2f. Budgets

New `DEFAULT_BUDGETS["batch-match"]`, scaled for N (orchestrator can
further scale by batch size):

```python
{"max_turns": 25 * n, "max_builds": 8 + 4 * n, "max_sweeps": 3 * n,
 "max_patch_failures": 4, "max_submit_failures": 3, "cleanup_turns": 0,
 "max_session_tokens": 400_000 + 150_000 * n}
```

Builds are shared across targets (one build covers the TU), so they scale
sub-linearly; turns scale linearly.

## Phase 3: orchestrator (`orchestrator.py`, `policies.py`, config)

- `policies.py`: register `batch-match` (same scope/tools/gate as
  `match`, prompt `prompts/batch-match.md`).
- `_group_into_batches(unit, targets, batch_size)`: group by
  `target.source`, then chunk. Batches never span source files (the brief
  shows one source file).
- `_run_batch_session(batch, carryover)`: claim all targets; one
  `init_session` with `target_id=f"batch:{unit}:{i}"`,
  `session_type="batch-match"`, `symbol=""`; run `BatchSession`; on exit
  promote each entry in `accepted.json` (Phase 4); release all claims.
- `run_pipeline` match stage: first pass with `batch_size` from
  `llm-decomp.json` (default 3 — start conservative, scale after observing
  transcripts); retry pass runs remaining/failed targets as **single
  `match` sessions** (reuses the untouched existing path, no second batch
  code path).
- Carryover: summarize per-target outcomes from the previous batch.

## Phase 4: promotion (`promote.py`)

`promote_accepted_from_manifest(meta, sdir) -> list[dict]`: reads
`accepted.json`, and for each entry calls the same
`update_target_result` + ledger + checkpoint logic as
`promote_accepted` (refactor the shared body; tier from `rule`, fuzzy
from the entry). Disk-driven, so promotion no longer depends on the
in-memory session object — and crash recovery can re-promote from the
manifest alone.

## File-by-file

| File | Change |
|---|---|
| `contracts.py` | `TargetRun` (or keep in session.py); `SessionOutcome.accepted_ids/failed_ids` |
| `state.py` | 0b: accepted-snapshot-aware `reconcile()`; `record/load_accepted_entries` |
| `session.py` | 0a baseline promotion on accept; `BatchSession` subclass (or `targets` dict + branches); batch termination/restore |
| `brief.py` | `build_batch_brief` |
| `tools.py` | `submit` schema: optional `target_id` enum (batch policy) |
| `verify.py` | `pending_symbols` param on `verdict_for_submit` |
| `policies.py` | `batch-match` policy |
| `config.py` | `DEFAULT_BUDGETS["batch-match"]` |
| `orchestrator.py` | `_group_into_batches`, `_run_batch_session`, retry-as-single, batch_size from config |
| `promote.py` | `promote_accepted_from_manifest`; shared-body refactor |
| `llm-decomp.json` | `batch-match` model config + `batch_size` |
| tests | update `test_session`, `test_verify`, `test_promote`, `test_brief`; add batch-session end-to-end with mock provider; regression tests for the two Phase-0 bugs |

## Risks

| Risk | Mitigation |
|---|---|
| Model confuses targets | `submit.target_id` is an enum; accepted targets protected by 0a+auto-verify; brief labels ids explicitly |
| One hard target stalls the batch | per-target `max_submit_failures` skip; leftovers retried as single sessions |
| Conversation growth with N | brief is one message; per-target turns only. ~5 × 15 turns fits the scaled token budget; existing `_elide_if_needed` covers overflow |
| Verdict wrongly blocks on pending siblings | `pending_symbols` filter (2d) — the one genuinely new verdict rule; covered by tests |
| Untested batch prompting | default `batch_size: 3`; inspect transcripts before scaling up |

## Sequencing (revised after adversarial review)

0. **Fix the red tree first** — done (see review addendum F0).
1. **Phase 0+4 together** — 0a is *not* safe standalone (review F3/F4):
   promotion currently uses the session's *last* verdict, so after 0a a
   cleanup HARD_REJECT promotes as EQUIVALENT_MATCH. Ship 0a + 0b +
   manifest-driven promotion as one unit.
2. Phase 1 → Phase 2 → Phase 3.

---

## Adversarial review addendum (Kimi + GLM, consolidated)

Two independent adversarial reviews of this plan were run against the
code. Findings below are deduplicated; plan text above is amended where
noted.

### F0 — fixed during review: red tree

Uncommitted refactor dropped `ranges = ...` in `_patch_fn`
(session.py:254) — every successful patch raised `NameError`, 4 session
tests erroring. Fixed; `test_session` green.

### Critical — accepted, plan amended

- **R1. `cleanup_turns: 0` does not remove CLEANUP — it ends the session
  at the first accept.** `_handle_verdict` sets `status=CLEANUP`
  unconditionally on accept, and the loop breaks on
  `CLEANUP && cleanup_turns_left <= 0` (session.py:551-555). Three other
  break paths fire on accept too (post-verify no-feedback, no-change
  re-submit). Simplification #6 is wrong as stated: **BatchSession must
  override accept handling** — stay in MATCHING, never enter CLEANUP, run
  verdicts per-target. This is a real `run()`/`_handle_verdict` override,
  not a budget tweak. Phase 2a now mandates the BatchSession subclass
  (the "or branches" option is removed).
- **R2. `pending_symbols` filter is unsound for missing symbols.** A
  deleted/renamed/inlined pending target sweeps as
  `current_mismatches=-1`, and `-1 > baseline` is False — invisible to
  `sibling_worse` even without the filter (pre-existing hole). With the
  filter, the model can delete pending target C, `submit(A)` passes, and
  the accepted snapshot + promotion permanently commit C-deleted source.
  **Amendment (2d): `current_mismatches == -1` for any batch member is
  always ≥ SOFT_REJECT**, pending or not.
- **R3. Promotion uses the last verdict, not the accept verdict**
  (orchestrator.py:84-87 reads `session._verdict`). Today this can
  promote a cleanup-broken match as EQUIVALENT_MATCH with a stale fuzzy;
  after 0a it mis-tiers a restored FULL_MATCH as EQUIVALENT_MATCH.
  **Amendment (Phase 4): the manifest entry written at accept time stores
  the accept verdict (rule + fuzzy); promotion reads only manifest
  entries.** This also fixes the current single-target bug.
- **R4. "Crash recovery can re-promote from the manifest" had no
  mechanism.** Promotion runs only on clean exit; reconcile marks ABORTED
  and moves on. **Amendment (Phase 4): startup/reconcile scans non-DONE
  *and* DONE sessions for manifests with entries not yet reflected in
  targets.json and re-promotes them.**
- **R5. Two INIT-restore crash paths, plan named one.** `run.py
  cmd_solve` (run.py:128-135) has its own exception handler restoring
  from INIT and marking ABORTED — which also makes reconcile skip the
  session. **0b must fix both handlers** (prefer accepted snapshot in
  each).

### High — accepted, plan amended

- **R6. Submit builds are uncounted.** `verdict_for_submit` calls
  `verify.build_unit` directly, bypassing `_build_fn`'s counter. Up to
  ~4n extra builds (max_submit_failures+1 per target). **Amendment (2f):
  count verify builds against `max_builds`; budget `8 + 6*n`.**
- **R7. Manifest/snapshot write ordering unspecified.** Required order:
  snapshot files → save accepted `.o` → fsync → append manifest entry →
  fsync. Manifest is durable *last*, so a crash can lose an accept but
  can never promote unverified bytes. Reconcile checks snapshot
  completeness (all writable present) before preferring it.
- **R8. Batch session ids must not contain `/` or `:`.**
  `paths.session_dir` joins `target_id` verbatim and reconcile globs
  depth-2 (`*/*/`); `batch:kyoshin/cf/Foo:0` nests deeper and becomes
  invisible to reconcile. **Amendment (Phase 3): sanitize
  (`batch-<unit_slug>-<i>`, slashes → `__`).**
- **R9. Re-submit of accepted/failed targets is a state-machine hole.**
  Re-accept burns a build and duplicates manifest entries; failed →
  accepted transitions are undefined. **Amendment (2c): `submit` on a
  terminal target returns an error ToolResult without verifying.**
- **R10. Scalar streak counters end batches early.**
  `compile_error_streak` and `_no_tool_turns` accumulate across targets.
  **Amendment (2e): reset both on each accept.**
- **R11. Shared headers are outside every snapshot/restore** (pre-existing:
  `_restore` and both snapshot fns cover only `meta.writable`; a
  `cross_tu_regression` HARD_REJECT claims "reverted" while the header
  edit stays on disk). **Amendment (Phase 3): batch-match v1 sets
  `shared_headers=False`; the snapshot/restore gap for shared headers is
  filed as a separate pre-existing bug for the single-target path.**

### Medium — accepted, plan amended

- **R12. Multiple submits in one reply collapse into one verdict**
  (scalar `_submitted`, one verify per turn). **BatchSession verifies per
  submit call, in order.**
- **R13. Agent-facing sweep output isn't pending-filtered** — the model
  would see pending targets flagged as regressions and waste turns.
  **Amendment (2b): `_diff_fn` sweep labels batch-pending symbols
  separately ("pending — expected to drift").**
- **R14. Partial-claim failure leaks claims** (no rollback; reconcile
  never releases claims). **Amendment (Phase 3): claim with rollback —
  on conflict, release already-claimed members and skip the batch.**
- **R15. `max_session_tokens` is an elision threshold, not a budget** —
  nothing terminates on it. Don't cite it as a cost cap; the real caps
  are turns/builds. (Hard cap enforcement: optional follow-up.)
- **R16. Size-1 remainder batches** must route to the existing single
  `match` session, not `batch-match` (Phase 3 amended).
- **R17. `_carryover` reads scalar session fields** (`_verdict`,
  `best_mismatches`); rewrite to summarize the batch manifest (Phase 3
  amended). `record_best` stays session-wide (best state = whole writable
  set at lowest total mismatches across pending targets); per-target
  `best_mismatches` is reporting-only.
- **R18. `diff`/`equivalence` empty-symbol fallback** — with
  `meta.symbol=''`, omitting `symbol` launches hexdiff/equivalence with
  `''`. **Amendment (2b): under batch-match, `symbol` is required;
  `_diff_fn`/`_equivalence_fn` hard-reject empty symbols.**

### Review verdicts on plan claims

- "0a needs no new check code" — **confirmed sound** as a mechanism.
- "`sibling_regression` needs no change" — **confirmed**.
- "0a/0b shippable standalone" — **refuted** (R3/R5); ship with Phase 4.
- "batch_size=1 is the degenerate case of the same path" — **refuted**;
  size-1 must dispatch to the legacy `match` session (R16).
- 0a's deepest single-target benefit (found by review): today a cleanup
  patch that stays fuzzy≥50 and probes EQUIVALENT *re-accepts* as
  `equivalent_match`, overwriting the FULL_MATCH snapshot and
  downgrading targets.json. 0a converts this into HARD_REJECT.

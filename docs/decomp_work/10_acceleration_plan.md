# Acceleration plan — workstreams 2, 4, 6

Derived from the 2026 project analysis: 5,564/19,221 functions matched (29%) but only
**10.4% of bytes** (512KB / 4.9MB). LLM harness acceptance is ~4% (deepseek-v4-flash-free)
to 7% (kimi-k3); 40% of experiments die before evaluation, 34% at COMPILE_ERROR.

## WS-2 — Zero-LLM acceptance sweep (probe + recertify)

**Goal:** convert already-compiling functions into accepted targets at zero model cost,
and refresh certificates so the callgraph frontier un-stalls.

Background: 1,252 catalog functions compile but are unmatched (638 COMPILES,
360 STRUCTURAL, 180 CODE_MATCH, 97 HIGH_MATCH). `probe` runs objdiff + ppc_equivalence
SMT and flips fuzzy ≥50% functions with proven equivalence to `EQUIVALENT_MATCH`.
The `callees-accepted` frontier is only 37 because accepted targets hold stale
certificates; `recertify --bottom-up` refreshes them leaves-first.

Steps:

1. `cp tools/coop/targets.json tools/coop/targets.json.bak-pre-ws2`
2. `python3 tools/coop/run.py targets recertify --bottom-up`
3. Probe in batches, highest-value first: HIGH_MATCH → CODE_MATCH → STRUCTURAL →
   COMPILES; within a tier, largest byte-size first (bytes are the real gap).
   `python3 tools/llm_harness/run.py probe --number 100 --write` repeatedly, or
   per-TU with `--tu <unit> --write` for targeted sweeps.
4. Measure: accepted count, callees-accepted frontier size before/after
   (`python3 tools/coop/run.py harness --selection callees-accepted --include-catalog --dry-run | grep -c '^==>'`).

Success metric: ≥50 new EQUIVALENT_MATCH acceptances; callees-accepted frontier ≥150.

**Exclusive write access to `tools/coop/targets.json` while running.**

## WS-4 — Large-function track (byte-gap attack)

**Goal:** stop optimizing for function count; build the tooling to attack the 84% of
bytes sitting in NOT_STARTED. 776 functions >1KB and 1,087 in 513B–1K hold most of it.

Deliverables (new files only; **no writes to targets.json**):

1. `tools/coop/large_queue.py` — ranks NOT_STARTED functions ≥512B by readiness:
   - score = (fraction of called_functions already FULL_MATCH/EQUIVALENT_MATCH)
     then byte size descending; exclude `has_indirect_calls` and functions with
     `unresolved_called_functions` from tier A, list them in tier B (blocked).
   - `--json out.json` emits batches consumable by `tools/coop/batch-cycle.py`
     and `tools/llm_harness/run.py solve <id>`.
2. `llm-harness-large.json` — profile for large functions: `initial_candidates` 3–4,
   `match_repairs` 8–10, larger `prompt.max_output_tokens` (8192), longer provider
   timeout. Large functions need more repair budget than the leaf defaults.
3. Decomposition list: for every tier-A function >1KB, list its NOT_STARTED callees —
   those callees get queued first (decompose-before-parent per SKILL.md).
4. Validate with a dry run printing the top 50 queue entries.

Success metric: ordered queue of ≥300 large functions with readiness scores;
profile validated with `python3 tools/llm_harness/run.py solve <one-large-id> --dry-run`
using the large config.

## WS-6 — Struct/global recovery ahead of the frontier

**Goal:** the dominant LLM failure mode is guessing wrong about globals/types
(34% COMPILE_ERROR). Recover the unknowns the 2,711-function ready frontier actually
references, before mass-solving.

Deliverables (new files + dry-run rename plans only; **no writes to targets.json**,
no `symbols rename-all` without `--dry-run` — application is deferred to avoid
write races with WS-2):

1. `tools/coop/frontier_refs.py` — for every ready-frontier function, scan its retail
   asm in `build/us/asm/**.s` for operands referencing `UnkClass_*`, `lbl_*`, and
   `func_*` symbols; rank by reference count across the frontier; cross-reference
   `config/us/symbols.txt`.
2. `docs/decomp_work/frontier_unknowns.md` — top ~50 unknowns blocking the frontier,
   each with: symbol, address, reference count, units referencing it, and (where
   derivable from `tools/coop/run.py symbols show/xref`) a proposed semantic name
   and same-length check for `rename-plan`.
3. Rename plans (`symbols rename-plan <old> <new>`, dry-run) for the confidently
   derivable names, appended to the report as ready-to-run `rename-all` commands.

Success metric: ranked report covering ≥80% of frontier references to unknowns;
≥15 ready-to-apply rename plans with same-length names.

## Sequencing

- WS-2, WS-4, WS-6 run in parallel (WS-4/WS-6 are read-only on shared state).
- After WS-2 completes: apply WS-6 rename plans, then feed WS-4 tier-A queue to the
  LLM harness with `llm-harness-large.json`.
- WS-1 (frontier-model switch) and WS-3 (dead-run pipeline fixes) are config/provider
  work tracked separately.

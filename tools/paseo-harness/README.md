# paseo-harness

Generic, section-based batch matching harness for paseo. Each batch runs in its
own paseo-managed **git worktree** (isolated branch), with cost controls and
conflict-free merge-back of the shared registries.

> Worktree isolation: agents no longer share one dirty working tree — the #1
> source of lost work, mixed commits, and "who committed my work?" token waste.

## Install / requirements

- Paseo daemon running (CLI at `/Applications/Paseo.app/Contents/Resources/bin/paseo`
  or `~/.local/bin/paseo`; override with `PASEO` env).
- The daemon's workspace auto-naming needs **at least one provider with
  credits** (it tries opencode/claude-haiku, codex, … when naming a new
  worktree workspace). If all naming providers are out of credits, launches
  are slow/fail — top up one, or launch `paseo run` once manually.
- Run everything with the venv python (Python 3.13.6):

```bash
.venv/bin/python3 tools/paseo-harness/paseo-harness.py --help
```

## Commands

```bash
# 1. Plan: split unmatched targets for a section into batches
.venv/bin/python3 tools/paseo-harness/paseo-harness.py plan --section criware
.venv/bin/python3 tools/paseo-harness/paseo-harness.py plan --section kyoshin --batch-size 25

# 2. Run the monitor (babysitter) — keep in its own terminal
.venv/bin/python3 tools/paseo-harness/paseo-harness.py run --section criware
.venv/bin/python3 tools/paseo-harness/paseo-harness.py run --section criware \
    --running 6 --max-agent-hours 24 --allow-smt

# 3. Status / merge / cleanup
.venv/bin/python3 tools/paseo-harness/paseo-harness.py report --section criware
.venv/bin/python3 tools/paseo-harness/paseo-harness.py merge  --section criware --batch criware-01
.venv/bin/python3 tools/paseo-harness/paseo-harness.py cleanup --section criware --archive-agents
```

Sections match the target `unit` prefix case-insensitively:
`criware` → `CriWare/...`, `kyoshin`, `monolib`, `nw4r`, `rvl_sdk` → `RVL_SDK/...`,
`powerpc_eabi_support`, … — any prefix works.

State lives in `~/.paseo/harness/<section>/` (plan.json, state.json,
monitor.log, reports/<batch>.txt). Override with `PASEO_HARNESS_STATE` or
`--state-dir`.

## How it works

**Plan (`plan`)** — unmatched = `workflow_status != ACCEPTED`. Two filters:
- **Ready frontier only**: batches are built exclusively from the live `ready`
  frontier (leaf, or every callee accepted + certified) as computed by
  tools/coop's `harness_targets`; everything else in the section is listed as
  **deferred** (unaccepted callees / indirect or unresolved calls / not
  buildable) so agents never burn cycles on targets that cannot be matched yet.
- **Whole units**: units with ≥ 20 unmatched targets get their own batch; the
  rest are pooled with whole units (a unit never straddles two batches → no
  source conflicts on merge). Pooled **hard** batches (majority non-leaf) flush
  at `--hard-batch-size 14` instead of 30 so a wall hit wastes fewer tokens.

Each batch gets a thinking tier (leaf-only + small → `medium`, leaf-heavy →
`high`, else `max` — one notch lower than before) and optionally a hard-batch
model (`--hard-model`). Known stalls from attempts.jsonl are carried into the
batch table (targets with prior `stall_note`s are flagged so agents don't repeat
ruled-out experiments), and a per-batch **KB digest** (`state/digests/<batch>.md`,
skip with `--no-kb-digest`) lists the top MWCC_REFERENCE records for the batch's
symbols — documented walls become blockers to record, not experiments to repeat.

**Run (`run`)** — the monitor loop:
- creates the paseo worktree workspace **first**, prepares it (symlinks the
  untracked `orig/` retail files from the main checkout — worktrees cannot
  build without them — and quiets it in git), then launches the agent into it
  with `--workspace <id>`; `--prewarm` additionally runs `configure.py` +
  `run.py build <unit>` per batch unit so the agent's first hexdiff is
  incremental instead of a full-tree build;
- branch `paseo/<section>/<batch>` branched off `main` (`branch-off` on first
  launch, `checkout-branch` on relaunch so the branch carries all attempts);
- merges finished batches back into `main` with **custom 3-way merge drivers**
  registered locally (`.git/config` + `.git/info/attributes`):
  - `tools/coop/targets.json` — per-target-id 3-way union (theirs wins if it
    changed vs base, else ours)
  - `docs/evidence/decomp/attempts.jsonl` — unique-line union (append-only log)
  - `config/*/symbols.txt` — per-key 3-way union
  - `README.md`, `assets/progress-map.svg` — keep ours (pre-commit hook
    regenerates them from the merged registry)
  - source/configure.py use normal 3-way merge; real conflicts abort the merge
    (`merge_conflict`) and the batch is reported for manual resolution
    (`paseo-harness.py merge --batch <id>`); a busy main tree defers the merge
    (`merge_deferred`) and retries each sweep.

**Report (`report`)** — the evidence loop: per-batch accepted/total, thinking
tier, model, agent-hours and **accepts/hour**, aggregated by thinking tier, so
you can see whether `medium`/`high`-thinking batches accept at the same rate as
`max` and lower the default with data instead of guessing.

## Cost controls (all the review recommendations)

| Control | Flag | Effect |
|---|---|---|
| Worktree isolation | always on | no shared-tree clobbering, mixed commits, or commit-forensics loops |
| Ready-frontier batching | always on | agents only ever see matchable targets — callee-blocked/wall targets are deferred |
| Stall carry + KB digests | always on | ruled-out experiments and documented walls are visible up front, not rediscovered |
| Lower concurrency | `--running 6` (default) | fewer concurrent max-thinking requests → fewer provider timeouts |
| Epidemic backoff | `--backoff-threshold 3 --backoff-sweeps 2` | ≥3 errors/hangs in one sweep pauses launches — no relaunching into the same provider jam |
| Relaunch budget | `--max-relaunch 3` | cap per batch; batches that exhaust it are `gave_up`, not marked done |
| Relaunch variant | — | relaunches continue the same branch, **lower thinking one notch** per attempt, optional `--fallback-model`, **seeded with the dead agent's report** (no re-exploration) |
| Agent-hour budget | `--max-agent-hours 24` | stops launching new batches once cumulative agent-hours are spent; `report` shows the running total |
| Tiered thinking | plan output | `medium`/`high` by default, `max` only for majority-non-leaf batches; `--hard-model` routes hard batches to a stronger model |
| Hard-batch sizing | `--hard-batch-size 14` | pooled hard batches are smaller → less to re-explore on failure/wall |
| Lazy docs | prompt | agents read AGENTS.md once; MWCC_REFERENCE/PLAN.md only on demand via `mwcc_kb` — big docs are no longer re-billed every turn |
| Prewarm | `--prewarm` | configure + build batch units before the agent starts (saves wall-clock, not tokens) |
| No size-hacking | prompt rule | stub/padding-to-match-size is explicitly forbidden (replaces the mid-run "fixer" agent) |
| FULL_MATCH-only | `--allow-smt` off | in-session `cycle --smt` is off by default; only FULL_MATCH / witness-certified EQUIVALENT_MATCH accepted via plain `cycle` |

## Pre-flight (recommended before each wave)

```bash
.venv/bin/python3 tools/coop/run.py targets sync-calls        # callgraph freshness
.venv/bin/python3 tools/coop/run.py targets recertify --bottom-up  # refresh callee certs (grows the ready frontier)
.venv/bin/python3 tools/coop/reloc_map.py mine                # fresh reloc-name-drift map (hexdiff hints)
```

These are script-side and free; they make the ready-frontier filter and the
agents' hexdiff hints accurate.

## Notes & caveats

- **Worktrees need `orig/`**: the untracked retail files are symlinked from the
  main checkout into each worktree by the harness before the agent starts
  (and quieted in the worktree's git exclude). If `orig/` moves or is missing
  in the main checkout, agents cannot build.
- **First build per worktree is a full configure+ninja** unless you pass
  `--prewarm` (configure + batch-unit builds happen before the agent starts).
  Either way builds are incremental afterwards and no longer contend on one
  shared build lock.
- The **goal is FULL_MATCH**: no SMT probes run in-session (they were
  15-30 min machine-time each and stalled the old wave). Add `--allow-smt` if
  you want the opt-in `cycle --smt` path for stuck >90% targets.
- Merge-back requires `main` to be reasonably clean; a busy `main` defers the
  merge (no stashing, never touches your tree).
- Don't run two monitors for the same section — state.json is single-writer
  (the old `monitor.pid` guard is per-process; the harness relies on you).
- The old wave's `tools/coop/cri_orchestrator/` is superseded by this harness
  (it had no isolation, no merge-back, no backoff, and relaunched identical
  agents into the same timeouts).

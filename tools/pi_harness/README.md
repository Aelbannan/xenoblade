# pi-harness

TU-level batch decompilation harness built on the pi SDK. Unmatched targets
in a translation unit are batched to pi agent sessions; the harness owns
acceptance (compile → lint → `batch-cycle.py`) and promotes
`FULL_MATCH` / `EQUIVALENT_MATCH`. When every function in the TU matches, a
TU-final session verifies the whole TU (including data), creates/cleans
classes and structs, renames symbols, adds comments, and flips the unit to
`Matching` in `configure.py`.

## Setup

```bash
cd tools/pi_harness && npm install
cp pi-harness.example.json ../../pi-harness.json   # then edit models
```

## Usage

```bash
npm --prefix tools/pi_harness run pi-harness -- --tu kyoshin/CGame [--tu other/Unit] [--dry-run] [--max-parallel 3] [--config path]
```

May be run from anywhere inside the repo (the root is auto-detected).

## How it works

Per batch (batches run sequentially within a TU, TUs in parallel up to
`maxParallelTUs`):

1. Claim targets, build a brief (retail ASM per target + rules), snapshot
   the writable-scope files (plain copies under `build/pi-harness/snapshots/`).
2. Run a pi SDK session (see config for model/thinking per phase).
3. **In-session continuation** (when `maxRePrompts > 0`): on compile or lint
   failure the harness re-prompts the **same** session with the error
   feedback — the model keeps full context and fixes its own code in place.
   - Compile failure: broken code is kept; model fixes it with the compiler
     output as guidance.
   - Lint failure: code compiles; model fixes only the violations. A
     compilable checkpoint is saved so a broken lint fix can roll back.
   - Re-prompts are capped (`maxRePrompts` when the session timed out,
     `maxStuckRePrompts` when the model finished but code still fails).
   - On cap hit or timeout the snapshot is restored and a fresh session
     retries with the rejection history embedded in the brief.
4. **Acceptance** (`batch-cycle.py` + targets.json). Accepted targets are
   promoted; failed ones either retry as singleton sessions (large
   targets) or are collected for a re-batch pass (small targets below
   `singletonMinSize` bytes), up to `maxBatchRetries` total attempts.
   Exhausted targets are skipped (ledgered).

Restore only ever copies back the snapshotted `.c/.cpp/.hpp` files when
falling back to a fresh session — there is deliberately **no git
machinery** in the revert path. Within a session, the model's edits stay
in place so it can fix failures with full context.

When a TU reaches zero unmatched targets, the TU-final session runs
(serialised process-wide; builds wrapped in `build_lock.py`, which holds
the same `.hexdiff.lock` as hexdiff).

## Config (`pi-harness.json` at repo root)

| Key | Default | Meaning |
|---|---|---|
| `matchModel` / `cleanupModel` | — | `{provider, model, thinkingLevel}` per phase (`off minimal low medium high xhigh`) |
| `batchSize` | 5 | Targets per batch session |
| `maxParallelTUs` | 2 | Concurrent TU workers |
| `maxBatchRetries` | 2 | Total attempts per target (batch + singletons) |
| `singletonRetry` | true | Retry failed targets one-per-session |
| `maxTokens` | 0 | Max output tokens per session (0 = model default) |
| `singletonMinSize` | 0 | Targets with retail ASM below this (bytes) go to a re-batch pass instead of singletons (0 = all use singletons) |
| `maxRePrompts` | 3 | In-session re-prompts on compile/lint failure before a fresh session (0 = single-prompt, no continuation). Higher cap used when model timed out mid-work. |
| `maxStuckRePrompts` | 1 | In-session re-prompts when model completed but code still fails (lower cap avoids entrenchment). |
| `maxBriefChars` | 80000 | Prompt budget; retail ASM is middle-elided to an equal per-target share |
| `maxBatchMinutes` | 60 | Wall-clock timeout per session (TU-final gets 2x) |
| `region` | `us` | Retail region |
| `pythonBin` | auto | Python for coop tooling; auto-detects `.venv/bin/python` |
| `sessionDir` / `ledgerPath` | build/pi-harness/... | Session + ledger state |

## State

- `build/pi-harness/sessions/<unit>/<label>/` — pi session files + transcripts
- `build/pi-harness/ledger.jsonl` — events incl. per-session token usage
  (`session-usage`), acceptances, rejections, skips
- `build/pi-harness/snapshots/` — pre-session file copies

Claims left by a crashed run are released automatically at startup;
SIGINT/SIGTERM releases active claims.

## Notes

- The harness prepends `.venv/bin` to `PATH`, so agents' `python3` resolves
  to the repo venv (system python3 is 3.9 and cannot run the coop tooling).
- hexdiff holds the repo-wide build lock, so concurrent agents' builds are
  safe; TU-final builds go through `build_lock.py` for the same guarantee.
- For reloc name-drift near-misses (bytes identical, reloc *names* differ —
  MWCC_REFERENCE §1, the #1 cause of 99.3-99.9% stalls), use
  `run.py reloc-map diff <unit> --symbol <sym> --no-build` for the approved
  source `extern "C"` fix, or `run.py reloc-map mine` to refresh the repo map.
  `hexdiff` already prints these suggestions inline.

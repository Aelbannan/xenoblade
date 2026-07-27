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
3. **Compile check** (`coop run build <unit>`) — on failure, restore the
   snapshot copies and retry with the compiler output fed back into the
   next brief.
4. **Lint** (tools/llm_decomp `lint_delta` — no asm/register tricks) — on
   failure, keep the compilable candidate and feed the violations back into
   the next attempt.
5. **Acceptance** (`batch-cycle.py` + targets.json). Accepted targets are
   promoted; failed ones retry as singleton sessions up to
   `maxBatchRetries` total attempts, then are skipped (ledgered).

Restore only ever copies back the snapshotted `.c/.cpp/.hpp` files when a
candidate fails to compile — there is deliberately **no git machinery** in
the revert path. Regression/acceptance decisions remain owned by
`batch-cycle.py`.

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

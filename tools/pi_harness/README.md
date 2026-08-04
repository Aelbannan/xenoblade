# pi-harness

TU-level batch decompilation harness built on the pi SDK. Unmatched targets
in a translation unit are batched to pi agent sessions; the harness owns
acceptance (compile → match check via `batch-cycle.py`) and promotes
`FULL_MATCH` / `EQUIVALENT_MATCH`. Lint is gated in the **TU-final** phase
only (the batch loop deliberately does not re-prompt on lint — the cleanup
model fixes violations when the unit reaches zero unmatched targets). When
every function in the TU matches, a TU-final session verifies the whole TU
(including data), creates/cleans classes and structs, renames symbols, adds
comments, and flips the unit to `Matching` in `configure.py`.

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
3. **In-session continuation** (when `timeoutRetries`/`rejectionRetries` > 0):
   on compile failure or a no-match round the harness re-prompts the **same**
   session with the error feedback — the model keeps full context and fixes
   its own code in place.
   - Compile failure: broken code is kept; model fixes it with the compiler
     output as guidance.
   - No-match: the model gets per-target hexdiff mismatch feedback; failed
     targets' compiling drafts are banked (see near-miss draft banking).
   - Re-prompts are capped (`timeoutRetries` when the session timed out,
     `rejectionRetries` when the model finished but code still fails).
   - On cap hit or timeout the snapshot is restored and a fresh session
     retries with the rejection history embedded in the brief.
4. **Acceptance** (`batch-cycle.py` + targets.json). Accepted targets are
   promoted; failed ones either retry as singleton sessions (each capped by
   `maxSingletonSessions`, ledger-bounded by `maxAttemptsPerTarget`) or are
   collected for a re-batch pass (small targets below
   `singletonMinSize` bytes, capped by `maxRebatchAttempts`).
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
| `matchModel` / `singletonModel` / `rebatchModel` / `cleanupModel` | — | `{provider, model, thinkingLevel}` per phase (`off minimal low medium high xhigh`) |
| `batchSize` | 5 | Targets per batch session |
| `maxParallelTUs` | 2 | Concurrent TU workers |
| `maxAttemptsPerTarget` | 4 | Unified per-target session budget across pass 1 + rebatch + singletons (ledger-counted) |
| `maxSingletonSessions` | 2 | Max singleton sessions per target after the batch |
| `singletonEnabled` | true | Retry failed targets one-per-session |
| `singletonMinSize` | 0 | Targets with retail ASM below this (bytes) go to a re-batch pass instead of singletons (0 = all use singletons) |
| `rebatchEnabled` / `maxRebatchAttempts` | true / 0 | Per-TU rebatch session budget for small failed targets (0 = none) |
| `maxTokens` | 0 | Max output tokens per session (0 = model default) |
| `timeoutRetries` | 3 | In-session re-prompts when the session hit the wall-clock timeout (0 = single-prompt, no continuation) |
| `rejectionRetries` | 1 | In-session re-prompts when the model finished but code still fails (compile fail or no match; lower cap avoids entrenchment) |
| `maxBriefChars` | 80000 | Prompt budget; retail ASM is middle-elided to an equal per-target share |
| `briefTargetChars` | 12000 | Per-target ASM share cap inside a batch brief |
| `maxBatchMinutes` | 60 | Wall-clock timeout per session (TU-final gets 2x) |
| `tuFinalAttempts` / `tuFinalTimeoutMinutes` | 2 / 0 | TU-final retry count / timeout (0 = derive 2x batch) |
| `staleRoundThreshold` | 2 | Consecutive verify rounds with no divergence improvement before early-stop |
| `selection` | `claim-order` | Batch ordering: `claim-order` \| `similarity` \| `random` |
| `triage` | `off` | Pre-batch routing: `off` \| `route` (reg-swap-only → witness-only cycle) |
| `retryExhausted` | false | Re-attempt targets the ledger marked exhausted on a previous run |
| `bankOnlyOnBetter` | true | Only bank a near-miss draft that beats the stored best |
| `nearmissDir` / `knownWallsPath` | build/pi-harness/nearmiss / docs/KNOWN_WALLS.md | Draft bank + known-walls doc |
| `costModel` | zeros | Token price model for $/match reporting (0 = not priced) |
| `region` | `us` | Retail region |
| `pythonBin` | auto | Python for coop tooling; auto-detects `.venv/bin/python` |
| `sessionDir` / `ledgerPath` | build/pi-harness/... | Session + ledger state |

Unknown keys are rejected with a warning on stderr (a typo no longer
silently falls back to defaults).

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
- hexdiff holds the repo-wide build lock (`build/<region>/.hexdiff.lock`) —
  it acquires the same advisory flock as `build_lock.py` around its ninja
  invocation, so agent hexdiff builds serialise against harness-driven
  builds (configure.py, ninja, batch-cycle).
- For reloc name-drift near-misses (bytes identical, reloc *names* differ —
  MWCC_REFERENCE §1, the #1 cause of 99.3-99.9% stalls), use
  `run.py reloc-map diff <unit> --symbol <sym> --no-build` for the approved
  source `extern "C"` fix, or `run.py reloc-map mine` to refresh the repo map.
  `hexdiff` already prints these suggestions inline.

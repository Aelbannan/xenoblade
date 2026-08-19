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
npm --prefix tools/pi_harness run pi-harness -- --tu kyoshin/CGame [--tu other/Unit] [--dry-run] [--max-parallel 3] [--greenfield] [--config path]
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

> **Scope of the TU-final bash allowlist**: the `bash` tool exposed to
> TU-final sessions (spawnHook allowlist: run.py diff/size/symbols, hexdiff,
> build_lock.py, configure.py, ninja) is a **best-effort accident guardrail,
> not a sandbox**. It blocks careless destructive commands (`git reset`,
> `rm -rf`, `--smt`, `run.py cycle`, targets mutations, redirection,
> command substitution) but is deliberately not airtight: the model already
> has full repo read + scoped write access, and `configure.py` / `ninja`
> must remain executable for TU-final's legit work — both are arbitrary
> code executors, so a deliberately adversarial model can always escape
> (e.g. write code into the in-scope `configure.py`, then run the
> allowlisted `configure.py`). Real containment would require replacing the
> bash tool with structured subcommands or containerising the session; both
> are out of scope here. Batch sessions have **no bash at all**, which —
> together with the scoped edit/write tools — is the actual write
> boundary.

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
| `silenceThresholdSec` | 0 | Max SECONDS of model silence before a session is aborted as dead (0 = auto: xhigh 600 / high 300 / else 120). High-thinking models stream nothing while reasoning, so the fixed 120s default killed them mid-think |
| `emptyRoundRetries` | 2 | Re-run a prompt round that returned EMPTY (no assistant output — the 429-rate-limit give-up case) up to N times with a jitter sleep between, so the 45-min slot isn't burned on an empty verify->re-prompt loop |
| `roundStartJitterMs` | 15000 | Random 0..N ms delay before each round's first request. All 30 parallel sessions hit round-0 simultaneously -> the provider's per-user concurrency cap trips (run32: 38x 429 in one burst); spreading the wave removes most of it |
| `rpmLimit` | 0 | Process-wide HTTP request-rate cap (requests/min), a strict pacer (one request per 60_000/rpm ms, no bursts) wrapping ModelRuntime.streamSimple/completeSimple. 0 = disabled: no pacing AND the SDK's auto-retry stays ON. > 0 = pacer active, SDK auto-retry OFF (the limiter owns all pacing/backoff). Pace HTTP calls, not prompt rounds — every assistant tool-call turn is its own request, so size for the provider's HTTP rpm budget |
| `rpmLimit` | 0 | Global cross-session request budget (requests/min). 0 = disabled (SDK auto-retry stays ON — current behavior). When > 0, paces ALL concurrent sessions' HTTP requests to this rate (one per 60s/rpm ms) AND disables SDK auto-retry so the harness pacer owns backoff. The nube gateway serves DeepSeek with a hard ~20 rpm per-deployment cap; 14-20 parallel sessions saturate it (79% of session wall-clock wasted on 429/empty-retry churn) — set to ~18-20 for multi-session runs on that provider |
| `rejectionRetries` | 1 | In-session re-prompts when the model finished but code still fails (compile fail or no match; lower cap avoids entrenchment) |
| `maxBriefChars` | 80000 | Prompt budget; retail ASM is middle-elided to an equal per-target share |
| `briefTargetChars` | 12000 | Per-target ASM share cap inside a batch brief |
| `maxBatchMinutes` | 60 | Wall-clock timeout per session (TU-final gets 2x) |
| `tuFinalAttempts` / `tuFinalTimeoutMinutes` | 2 / 0 | TU-final retry count / timeout (0 = derive 2x batch) |
| `staleRoundThreshold` | 2 | Consecutive verify rounds with no divergence improvement before early-stop |
| `selection` | `claim-order` | Batch ordering: `claim-order` \| `similarity` \| `random` |
| `triage` | `off` | Pre-batch routing: `off` \| `route` (reg-swap-only → witness-only cycle) |
| `retryExhausted` | false | Re-attempt targets the ledger marked exhausted on a previous run |
| `greenfieldOnly` | false | Broad-sweep mode: only session targets with status NOT_STARTED (never worked). Previously-attempted targets are excluded from batches and `--all` discovery skips TUs whose only remaining work is previously-worked. TU-final only fires on genuinely fully-matched units. CLI: `--greenfield` |
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
  MWCC_CASES §1, the #1 cause of 99.3-99.9% stalls), use
  `run.py reloc-map diff <unit> --symbol <sym> --no-build` for the approved
  source `extern "C"` fix, or `run.py reloc-map mine` to refresh the repo map.
  `hexdiff` already prints these suggestions inline.

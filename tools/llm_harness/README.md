# LLM decompilation harness

This harness runs evidence-driven function-decompilation experiments against MWCC builds, objdiff scoring, and PPC equivalence. By default, `FULL_MATCH` / `EQUIVALENT_MATCH` winners are auto-promoted into canonical source (`execution.auto_promote`).

## Recommended fast path

Use this configuration when matching many small/medium functions:

```bash
# Prefer leaf / callees-accepted / ready frontiers (coop harness selection)
python3 tools/coop/run.py harness --selection ready --include-catalog --limit 20

# Or let solve auto-pick non-accepted targets with certified callees only
python3 tools/llm_harness/run.py solve --number 10 --certified-funcs

# Explicit target
python3 tools/llm_harness/run.py solve <target-id>
```

### Equivalence probe (no LLM)

For functions that already compile but are not yet `FULL_MATCH` / current
`EQUIVALENT_MATCH`, run objdiff + `ppc_equivalence` without spending model budget:

```bash
# Probe (writes results.jsonl; does not touch targets.json)
python3 tools/llm_harness/run.py probe --number 20 --dry-run

# Same as dry-run, then also persist accepted FULL/EQUIVALENT into targets.json
python3 tools/llm_harness/run.py probe --number 20 --write

# Explicit IDs or a whole TU
python3 tools/llm_harness/run.py probe <target-id> --write
python3 tools/llm_harness/run.py probe --tu kyoshin/CTaskGame --dry-run
```

Selection defaults: `selection=ready`, `--certified-funcs`, `--min-fuzzy 50`.
Skips `FULL_MATCH` and `EQUIVALENT_MATCH` that already have a current certificate.
Requires a built decomp object that still exports the symbol. Results land under
`build/llm-harness/probe/<batch>/`. `--dry-run` still runs SMT; it only skips
`targets.json` updates (`--write` is required to persist).

Use `targets recertify --bottom-up` when the goal is only refreshing certificates
on already-accepted targets; use `probe` to discover new `EQUIVALENT_MATCH`
wins from the compiling pool.

**Throughput knobs** (`llm-harness.json`):

| Setting | Role |
| --- | --- |
| `execution.initial_parallel` | Parallel initial candidates inside one `solve` |
| `execution.target_parallel` | Parallel targets in `batch` / `--number` |
| `execution.isolation.mode=git-worktree` | Isolate each candidate evaluation |
| `providers.opencode.pure=true` | Inline dossier; tools disabled; JSON schema enforced |
| `providers.opencode.delete_session=true` | Drop OpenCode sessions after each call |

OpenCode with `pure: true` + JSON schema + tools disabled is the default high-volume path. `deepseek-raw` remains an optional alternative provider (same candidate schema); keep it configured if you want a second model/role, but you do not need to switch for the default loop.

**Worktree reuse / SPLIT:** each isolated evaluation currently creates a fresh git worktree, runs `configure.py`, then ninja-builds one object. `prepare_workspace` already symlinks shared `build/<region>/{obj,include,asm}` and stamps `config.json` via `eval_cache.stamp_split_config` so ninja should skip expensive `dtk dol split`. A persistent worktree pool (reuse configured trees across candidates) is a follow-up if configure still dominates wall time — see `tools/llm_harness/eval_cache.py`.

**Early stop on unvalidated callees:** `solve` stops match-repair when the best candidate is blocked with `blocked_reason=unvalidated_callee` (`CODE_MATCH` + `INCONCLUSIVE_UNVALIDATED_CALLEE`) instead of burning `match_repairs`. Prefer `--certified-funcs` / `--selection ready` so those targets are not selected until callees are accepted.

## ChatGPT Web batches (offline)

Export ready-frontier function prompts into budget-limited text files, paste them
into ChatGPT Web, then ingest the response for local MWCC/objdiff evaluation.
Only authoritative **`FULL_MATCH`** or **certified `EQUIVALENT_MATCH`** results are
promoted. ChatGPT-reported percentages are ignored.

```bash
python3 tools/llm_harness/run.py web-export --batches 3 --budget 40

# Upload web_batches/batch_001.txt to ChatGPT Web.
# Save its response as:
# web_batches/batch_001.response.txt

python3 tools/llm_harness/run.py web-ingest \
  web_batches/batch_001.response.txt

# Upload the overwritten web_batches/batch_001.txt for round 1.
# Resume an interrupted EVALUATING round:
python3 tools/llm_harness/run.py web-ingest \
  web_batches/batch_001.response.txt --resume
```

Defaults: `--selection ready`, `--certified-funcs` on (`--no-certified-funcs` to
disable). Active files live under `web_batches/`; every ingest round is archived
under `web_batches/history/<batch_id>/round_NNN/`. Response candidates must not
use Markdown code fences (`fence_policy=strict_reject_outer_code_fences` is
printed on completion).

## Recommended workflow

```bash
# Adaptive closed loop (default decompilation path)
python3 tools/llm_harness/run.py solve <target-id> --dry-run
python3 tools/llm_harness/run.py solve <target-id>

# Blind identical-prompt sampling for model research only
python3 tools/llm_harness/run.py sample new <target-id> --runs 10

# Explicit promotion (preview, then write)
python3 tools/llm_harness/run.py promote build/llm-harness/<target>/<experiment>
python3 tools/llm_harness/run.py promote build/llm-harness/<target>/<experiment> \
  --write --owner <agent>

# Backfill: promote every saved accepted winner not yet in the registry
python3 tools/llm_harness/run.py promote-accepted --dry-run
python3 tools/llm_harness/run.py promote-accepted
```

`solve` generates a few diverse initial candidates, then spends remaining budget on compile repair or binary-diff repair against the best saved candidate. It stops on `FULL_MATCH` / `EQUIVALENT_MATCH`, repeated mismatch fingerprints, unvalidated-callee blocks, or exhausted repair budgets. Accepted wins are written into source and `targets.json` automatically unless `execution.auto_promote` is false.

## Requirements

- A working `coop.json`, retail split objects, MWCC toolchain, Ninja, and objdiff.
- Providers configured in `llm-harness.json` (`opencode` via `opencode serve` HTTP API, `deepseek-raw`, optional `lmstudio` / `codex`).
- For OpenCode: run `opencode serve --port 4096` (optional basic auth via `OPENCODE_SERVER_PASSWORD`) and set `providers.opencode.base_url`.
- For Codex: install/login the Codex CLI (`codex login`), then set `providers.codex` and a model with `"provider": "codex"` (uses `codex exec --json`). With `providers.codex.pure=true` (default), Codex runs in an empty temp workspace with shell/apps/plugins/web-search disabled and no user/project config — prompt-only, no file/tool access.

```bash
python3 tools/coop/run.py baseline
python3 tools/llm_harness/run.py --show-config
```

## Low-level commands

`new`, `improve`, and `tu-complete` remain available for targeted sampling and TU completion. Prefer `solve` for function matching.

```bash
python3 tools/llm_harness/run.py new <target-id> --dry-run
python3 tools/llm_harness/run.py improve <target-id>
python3 tools/llm_harness/run.py tu-complete <unit>
python3 tools/llm_harness/run.py batch new <a> <b> <c>
python3 tools/llm_harness/run.py stats
```

`stats` aggregates `experiments.jsonl` per `model_id`, including a `by_status` map
(`FULL_MATCH`, `CODE_MATCH`, `HIGH_MATCH`, `COMPILES`, `COMPILE_ERROR`, `ERROR`, …).

## Acceptance semantics

| Field | Meaning |
| --- | --- |
| `symbol_accepted` | `FULL_MATCH`, or proved `EQUIVALENT_MATCH` under effect-aware policy |
| `promising` | Compiling non-accepted candidate worth retaining |
| `project_ready` | Symbol accepted **and** containing object/TU split-size/regression checks (set at promotion/TU) |
| `blocked_reason` | e.g. `unvalidated_callee` for unresolved-callee `CODE_MATCH` |

`CODE_MATCH` is never `symbol_accepted`. Per-function size delta is diagnostic only; split-object `.text` budget is enforced by canonical `coop cycle` during promotion.

## Safety contract

- Non-accepted `new` / `improve` / `solve` / `tu-complete` candidates only write under `build/llm-harness/`.
- Candidates are evaluated in git-worktree isolation when configured.
- Accepted `FULL_MATCH` / `EQUIVALENT_MATCH` winners are auto-promoted (`execution.auto_promote`, owner `execution.auto_promote_owner`, default `llm-harness`).
- Explicit `promote --write` still requires `--owner`.
- Set `execution.auto_promote` to `false` to keep the old experiment-only behavior.

## Configuration

Checked-in schema uses model roles:

```json
{
  "models": {
    "initial": [{"id": "fast", "provider": "opencode", "model": "...", "runs": 1}],
    "repair": [{"id": "strong", "provider": "deepseek-raw", "model": "...", "runs": 1}],
    "tu": [{"id": "strong", "provider": "deepseek-raw", "model": "...", "runs": 1}]
  },
  "solve": {
    "initial_candidates": 3,
    "compile_repairs": 2,
    "match_repairs": 4,
    "max_repeated_fingerprint": 2
  },
  "prompt": {
    "max_chars": 60000,
    "include_raw_hex": false
  },
  "execution": {
    "initial_parallel": 3,
    "target_parallel": 10,
    "isolation": {"mode": "git-worktree", "copy_dirty": true}
  },
  "providers": {
    "opencode": {
      "base_url": "http://127.0.0.1:4096",
      "pure": true,
      "delete_session": true
    }
  }
}
```

Legacy keys `models.new` / `models.improve` / `models.tu-complete` still resolve via aliases. Knowledge-base and pipeline config keys are rejected.

Print the effective config with:

```bash
python3 tools/llm_harness/run.py --show-config
```

## Prompt evidence

Prompts contain no frozen/retrieved MWCC knowledge corpus. Function prompts assemble
`common.md` + workflow (`new` / `improve`) + a **compacted** target dossier
(`compact_model_facing_dossier`) that prefers a retail ASM listing (`retail_asm`)
over a decoded-instruction JSON array, plus compact type snippets (not whole headers).
Same-class sibling bodies are included only when small enough to be useful style
exemplars. Repair turns also receive the best prior candidate, structured binary
feedback, and rejected mismatch fingerprints.

Compile-repair prompts are intentionally slim: candidate source + diagnostics, with an
optional short `## Retail ASM (signature/ABI check)` section when
`XenobladeAdapter.get_retail_asm` is available (wired from `solve`).

Outputs are capped: metadata fields have hard length limits, and `max_tokens` is chosen
from retail function size (see `prompt.max_output_tokens`, default 4096). Harness
`prompt.*` caps are applied by the adapter when building dossiers.

Human references such as `docs/MWCC_REFERENCE.md` remain available outside the model path
and must never appear in solve prompts.

## Metrics dashboard

`tools/llm_harness/metrics.py` provides `TimingRecorder` for phases:

`llm`, `configure`, `ninja`, `objdiff`, `smt`, `worktree_create`

`solve` resets the recorder per experiment and writes `state["timings"]` in
`_finalize_solve`. Core records `llm`, `configure` (prepare_workspace), and
`worktree_create`. The Xenoblade adapter (with harness-bound `timings`) records
`ninja` (`_build_object`), `objdiff` (`report_unit` / isolated report generate),
and `smt` (equivalence / certify probes via `evaluate_unit_match(..., phase_timer=)`).
If no live phases were recorded, finalize falls back to summing each record's
`duration_seconds` under `llm`.

## Worktree SPLIT skip

See `tools/llm_harness/eval_cache.py`. Root cause: worktree `build.ninja` still lists
`config.json` as a `split` output even when that path is a symlink into the main tree.
Safe mitigation (already wired in `prepare_workspace`): after configure, materialize a
private `config.json` copy and bump its mtime so ninja skips SPLIT. Retail `obj` /
`asm` / `include` stay symlinked.

Follow-ups if SPLIT still appears in `-d explain` output: persistent worktree pool,
or `ninja -t restat` after stamp.

## Benchmarking (Phases 9–10)

Golden corpus (accepted FULL_MATCH targets by size/shape category):

`tools/llm_harness/tests/benchmark/golden_corpus.json`

```bash
# Corpus summary
python3 tools/llm_harness/run.py benchmark

# Compare saved solve vs sample experiments
python3 tools/llm_harness/run.py benchmark \
  --solve-experiment build/llm-harness/<target>/<solve-exp> \
  --sample-experiment build/llm-harness/<target>/<sample-exp> \
  --output build/llm-harness/benchmark-report.json

# Offline trust suite (fake providers; no API cost)
python3 -m unittest tools.llm_harness.tests.test_phase9_10

# Optional real-toolchain hidden-source smoke (requires MWCC/retail)
XENOBLADE_HARNESS_GOLDEN=1 python3 -m unittest tools.llm_harness.tests.test_golden_optional
```

Reports track accepted matches per model call, match improvement, size-bucket rates, repeated-fingerprint stops, evaluation errors, and automatic source mutations (must stay zero).

**Live model proof:** run `solve` and `sample new --runs 10` on the golden corpus with your configured providers, then feed the experiment dirs to `benchmark`. Offline fake-provider tests already assert the solve>sample accepted-per-call gate.

**Corpus gap:** no `EQUIVALENT_MATCH` targets exist in the registry yet; that category slot is reserved.

## Adapter surface

Xenoblade-specific logic lives in `xenoblade_project.py`: dossier construction, MWCC builds, symbol-local objdiff evaluation, ranking, and explicit promotion splicing.

# LLM decompilation harness

This harness runs evidence-driven function-decompilation experiments against MWCC builds, objdiff scoring, and PPC equivalence. By default, `FULL_MATCH` / `EQUIVALENT_MATCH` winners are auto-promoted into canonical source (`execution.auto_promote`).

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

`solve` generates a few diverse initial candidates, then spends remaining budget on compile repair or binary-diff repair against the best saved candidate. It stops on `FULL_MATCH` / `EQUIVALENT_MATCH`, repeated mismatch fingerprints, or exhausted repair budgets. Accepted wins are written into source and `targets.json` automatically unless `execution.auto_promote` is false.

## Requirements

- A working `coop.json`, retail split objects, MWCC toolchain, Ninja, and objdiff.
- Provider binaries configured in `llm-harness.json` (`opencode`, `deepseek-raw`, optional `lmstudio`).

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
`common.md` + workflow (`new` / `improve`) + a truncated target dossier that prefers a
retail ASM listing (`retail_asm`) over a decoded-instruction JSON array, plus compact
type snippets (not whole headers). Same-class sibling bodies are included only when
small enough to be useful style exemplars. Repair turns also receive the best prior
candidate, structured binary feedback, and rejected mismatch fingerprints.

Outputs are capped: metadata fields have hard length limits, and `max_tokens` is chosen
from retail function size (see `prompt.max_output_tokens`, default 4096). Harness
`prompt.*` caps are applied by the adapter when building dossiers.

Human references such as `docs/MWCC_REFERENCE.md` remain available outside the model path.

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

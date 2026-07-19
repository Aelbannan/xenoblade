# LLM decompilation harness

This harness runs repeatable function-decompilation experiments, compares models, and records build/match evidence. Its core is project-agnostic; Xenoblade-specific prompts, byte extraction, MWCC knowledge retrieval, commands, and acceptance rules live in `xenoblade_project.py` and `prompts/`.

It never promotes generated code automatically. Each candidate is applied only long enough to build and score it, then the original source and object bytes are restored.

## Requirements

- A working `coop.json`, retail split objects, MWCC toolchain, Ninja, and objdiff configuration.
- A full baseline build. Objdiff report generation reads every object in `objdiff.json`, including unrelated units:

```bash
python3 tools/coop/run.py baseline
```

- OpenCode installed and authenticated. Confirm available model IDs with:

```bash
opencode models
```

The checked-in `llm-harness.json` runs these five models through OpenCode:

- `opencode-go/deepseek-v4-pro`
- `opencode-go/deepseek-v4-flash`
- `opencode-go/mimo-v2.5`
- `opencode-go/mimo-v2.5-pro`
- `opencode-go/minimax-m3`

Each model defaults to one sample. `--runs N` means **N calls per model**; with this configuration, `--runs 3` makes 15 calls.

### Providers

| Provider key | Backend |
| --- | --- |
| `opencode` | OpenCode CLI (`opencode run`) |
| `reasonix` | Reasonix CLI |
| `deepseek-raw` | Direct DeepSeek chat-completions API |
| `openrouter` | OpenRouter chat-completions API |
| `lmstudio` | Local [LM Studio](https://lmstudio.ai/) OpenAI-compatible server (`http://localhost:1234/v1` by default) |

To use a local LM Studio model, start the Developer server, load a model, then point a workflow entry at `provider: "lmstudio"` with the model ID from `GET /v1/models`:

```json
{
  "id": "local-qwen",
  "provider": "lmstudio",
  "model": "qwen2.5-coder-32b-instruct",
  "runs": 1
}
```

Optional `providers.lmstudio` knobs: `base_url`, `timeout_seconds`, `temperature`, `max_tokens`, `api_key`, and `json_object` (default `true`; sends `response_format: json_object`).

## Quick start

Use a canonical target ID from `tools/coop/targets.json`:

```bash
# Generate and inspect the prompt without calling a model.
python3 tools/llm_harness/run.py new pad-copy-input-flag --dry-run

# Run one sample for each configured model.
python3 tools/llm_harness/run.py new pad-copy-input-flag

# Automatically select and run the next three unattempted, ready functions.
python3 tools/llm_harness/run.py new --number 3

# Include functions whose callees are not matched yet.
python3 tools/llm_harness/run.py new --number 3 --ignore-called-functions

# Run another round with prior harness outcomes included as feedback.
python3 tools/llm_harness/run.py improve pad-copy-input-flag

# Improve the first eligible non-accepted functions, or sample them randomly.
python3 tools/llm_harness/run.py improve --number 3
python3 tools/llm_harness/run.py improve --number 3 --random

# Decompile several functions concurrently. Each gets an independent context.
python3 tools/llm_harness/run.py batch new <target-a> <target-b> <target-c>

# Complete several ready TUs concurrently.
python3 tools/llm_harness/run.py batch tu-complete <unit-a> <unit-b> <unit-c>

# Stop between calls when a recorded budget is reached; budgeted runs are sequential.
python3 tools/llm_harness/run.py new pad-copy-input-flag --max-cost 1.00 --max-tokens 50000

# Resume the same prompt and completed model/run pairs.
python3 tools/llm_harness/run.py new pad-copy-input-flag \
  --resume build/llm-harness/pad-copy-input-flag/<experiment-id>

# Drop failed records from that state and rerun only those model/run pairs.
python3 tools/llm_harness/run.py new pad-copy-input-flag \
  --resume build/llm-harness/pad-copy-input-flag/<experiment-id> --retry-errors

# Aggregate A/B results by model.
python3 tools/llm_harness/run.py stats
```

Pass a different configuration before the subcommand:

```bash
python3 tools/llm_harness/run.py --config another-harness.json new <target-id>
```

Models can be assigned per workflow. An exact workflow entry takes precedence
over `default`; the legacy flat `models` array remains supported:

```json
{
  "models": {
    "default": [{"id": "general", "provider": "opencode", "model": "provider/general"}],
    "new": [{"id": "fast", "provider": "opencode", "model": "provider/fast"}],
    "improve": [{"id": "reasoning", "provider": "opencode", "model": "provider/reasoning"}],
    "tu-complete": [{"id": "large-context", "provider": "opencode", "model": "provider/large-context"}]
  }
}
```

`batch` uses the list for its selected workflow. A workflow without an exact
entry falls back to `default`; if neither exists, the command reports a
configuration error. `rescore` does not call models.

## Function workflows

`new` and `improve` operate on one registered, buildable function. The model does not receive or return the whole translation unit.

When `new` is passed `--number N` without a target, it selects `N` `NOT_STARTED` functions from the safe bottom-up (`ready`) call-graph frontier: leaf functions or functions whose callees are `FULL_MATCH`/`EQUIVALENT_MATCH`. This includes catalog functions. Targets with prior function-harness records, active claims, source definitions that cannot be located, or missing retail symbols are skipped. Pass `--ignore-called-functions` to select from all otherwise eligible pending functions. The selected functions run through the normal batch workflow.

Interactive runs emit timestamped progress messages to stderr for function start, agent/model start and retries, evaluation results, and batch completion. The normal experiment or batch path remains on stdout.

The dossier contains:

- canonical target identity and acceptance level;
- retail function bytecode and relocations extracted from the objdiff target object;
- the current function definition;
- accepted functions in the same unit;
- prior harness feedback for `improve`;
- a bounded instruction-word alignment and relocation-delta summary from prior
  non-matching candidates for `improve`;
- ranked, deduplicated records from the MWCC knowledge base.

### Model input modes

All workflows use the attached prompt as a single self-contained input. Their `context/` directory is intentionally empty: this avoids duplicating `TASK.md`, dossier JSON, source, knowledge, and history across repeated model turns and gives providers a deterministic prompt prefix to cache.

Independent `new` experiments remain byte-stable while source, target metadata, prompt templates, and the MWCC knowledge base are unchanged. Prior mismatch history is introduced only by `improve`; use `--runs N` when you want multiple samples of one exactly identical cached prompt.

The repository-local `opencode.json` requests `setCacheKey` for `opencode-go`. This asks the gateway to cache eligible prefixes but cannot guarantee a hit; verify `cache_read_tokens` and `cache_write_tokens` in `stats` after a run.

TU completion follows the same inline path. Its prompt already contains the bounded source slots, residual object evidence, selected MWCC records, and prior attempts. The model therefore receives no duplicate mini-workspace and does not need file or search tools. `--full-context` remains an explicit fallback; it embeds the complete source in that one prompt.

The required model response is:

```json
{
  "source": "complete replacement function definition",
  "hypothesis": "one bounded hypothesis, naming any MWCC knowledge IDs used",
  "notes": ["evidence or uncertainty"],
  "next_change": "one follow-up experiment"
}
```

The adapter splices `source` into the owning file, builds the unit, runs function-level objdiff/equivalence, compares the generated symbol size with the retail symbol, then restores the original file and object. Function experiments deliberately do not apply a whole-TU split-size gate.

### Function boundaries

By default, the adapter locates an existing definition using its qualified C++ name and a brace-aware scanner. Stable comments can make the boundary explicit:

```bash
python3 tools/llm_harness/run.py prepare pad-copy-input-flag
python3 tools/llm_harness/run.py prepare pad-copy-input-flag --write --owner <claim-owner>
```

This edits the source once:

```cpp
// LLM-HARNESS-BEGIN: pad-copy-input-flag
void CfPadTask::copyInputFlag(...) {
    // ...
}
// LLM-HARNESS-END: pad-copy-input-flag
```

`prepare` prints a unified diff by default. `--write` requires the canonical target claim to match `--owner`. Markers are comments and do not affect MWCC output.

For a function with no definition, insert an empty slot into an existing source file at a unique exact anchor:

```bash
python3 tools/llm_harness/run.py slot <target-id> \
  --file src/path/File.cpp --before '// unique anchor' --unit path/to/ObjdiffUnit

python3 tools/llm_harness/run.py slot <target-id> \
  --file src/path/File.cpp --before '// unique anchor' --unit path/to/ObjdiffUnit \
  --write --owner <claim-owner>
```

`slot` is also dry-run by default. It refuses missing or repeated anchors, duplicate markers, paths outside the repository, non-C/C++ files, conflicting existing target locations, missing unit assignment, and claim-owner mismatches. A successful write updates the canonical target source/unit; if registry persistence fails, the source edit is rolled back.

### Function acceptance and ranking

A function candidate is accepted by the harness when it satisfies the symbol-local matching policy:

- `FULL_MATCH`: 100% static function match; or
- `EQUIVALENT_MATCH`: fuzzy match at least 50% plus an effect-aware PPC equivalence proof;
- in either case, the generated function must be no larger than its retail function.

The harness does not measure or gate on the containing object's size during `new` or `improve`. It records retail size, candidate size, and their delta for the selected symbol. Whole-object split fit is checked by `tu-complete`, after all functions have received a first match.

Within one experiment, candidates rank by:

1. accepted versus unaccepted;
2. function match percentage.

The winner is saved as `best.json`, but source and canonical target state are not changed.

## TU completion

`tu-complete` is a separate closure workflow for residual whole-object problems after every function reported by objdiff has a nonzero first match. By default, the model sees and edits only bounded slots:

```bash
python3 tools/llm_harness/run.py tu-complete kyoshin/cf/CfPadTask --dry-run
python3 tools/llm_harness/run.py tu-complete kyoshin/cf/CfPadTask

# Complete the first eligible residual units, or sample them randomly.
python3 tools/llm_harness/run.py tu-complete --number 3
python3 tools/llm_harness/run.py tu-complete --number 3 --random
```

It automatically exposes imperfect functions that map to canonical targets as `function:<target-id>` slots using the same brace-aware boundary scanner as `new`/`improve`. It never sends unrelated function source.

For residual static data, declarations, or ordering, add an empty TU slot at a unique exact anchor:

```bash
python3 tools/llm_harness/run.py tu-slot kyoshin/cf/CfPadTask pad-statics \
  --file src/kyoshin/cf/CfPadTask.cpp --before '// unique anchor'
```

Or wrap an existing bounded region; both anchors must occur exactly once:

```bash
python3 tools/llm_harness/run.py tu-slot kyoshin/cf/CfPadTask pad-statics \
  --file src/kyoshin/cf/CfPadTask.cpp \
  --start 'static const BeginType beginValue' \
  --end 'static const EndType endValue = {};'
```

Both forms preview a unified diff. Add `--write --owner <claim-owner>` to apply it. Writes require an owner, refuse another owner's claims, require the unit's actual C/C++ source file, reject malformed/nested/duplicate slots, and use comments that do not affect MWCC output:

```cpp
// LLM-HARNESS-TU-BEGIN: pad-statics
static const ...;
// LLM-HARNESS-TU-END: pad-statics
```

It refuses to start while any reported function remains at 0%, or when no imperfect mapped function and no explicit TU slot exists. The targeted dossier contains:

- each automatic function or explicit TU slot's current source and bounded surrounding context;
- bytecode and relocations only for mismatched functions;
- retail/candidate bytes and relocations for mismatched non-`.text` ELF sections;
- `.text` symbol layout, relocations, and non-function gaps when residual text remains;
- aggregate code/data percentages and relevant MWCC knowledge.

The required response replaces only named slot contents:

```json
{
  "patches": [{"slot_id": "pad-statics", "source": "bounded replacement C++"}],
  "hypothesis": "one residual-layout hypothesis",
  "notes": [],
  "next_change": "one bounded follow-up"
}
```

The harness splices those patches into the complete source, builds the complete object, and then restores the original file/object. This supports residual work such as:

- static data and vtables;
- literal and string pools;
- relocation/linkage differences;
- function or data ordering;
- bounded compiler-ordering interactions.

For a genuinely cross-cutting case, retain the previous complete-TU input/output contract explicitly:

```bash
python3 tools/llm_harness/run.py tu-complete kyoshin/cf/CfPadTask --full-context
```

`--full-context` is an opt-in fallback, not the default.

`project.tu_context_chars` limits surrounding source per slot, and `project.tu_section_byte_limit` caps raw bytes per residual section. Truncation is recorded in the dossier.

There is no unit-wide equivalence proof. TU acceptance therefore requires:

- 100% unit code;
- 100% unit data;
- split-size fit.

### TU regression guard

A partial TU candidate can improve overall percentages while regressing a previously accepted function. The harness therefore fingerprints every function before the candidate build and enforces:

- existing `FULL_MATCH` targets must remain 100%;
- unchanged `EQUIVALENT_MATCH` fingerprints retain their proof;
- changed `EQUIVALENT_MATCH` functions are proved again and regress unless still equivalent or now 100%;
- split-size is checked again for every candidate.

Partial TU candidates rank lexicographically by final acceptance, guard pass, accepted-function count, data percentage, code percentage, then fuzzy percentage. Guard failures are retained as evidence but cannot outrank guarded candidates merely by raising aggregate percentages.

## A/B results, tokens, and cost

Every model call records:

- model ID and provider model name;
- duration;
- input and output tokens when OpenCode reports them;
- provider-reported cost;
- status, match percentage, equivalence, and function-size result; TU candidates record the whole-object split-size result instead;
- hypothesis, notes, and next change;
- error text for failed calls;
- whether the candidate won its experiment.

`stats` reports per model:

- attempts, completed calls, and errors;
- wins, accepted wins, and accepted candidates;
- average match percentage and duration;
- total input/output tokens;
- provider-reported cache-read and cache-write tokens;
- total and average provider-reported cost.

Cost and token fields are `null` or zero when the provider does not emit them. The OpenCode parser supports both generic `usage` objects and step-level `tokens`/`cost` events. Auto-sharing is forcibly disabled for dossiers.

Provider failures retry according to `execution.max_retries`; the recorded `provider_attempts` includes the successful/final call. `--max-cost` and `--max-tokens` force sequential execution and stop before the next call after the recorded total reaches the limit. A single call can exceed the remaining budget because its cost is not known in advance.

## Isolation and parallel execution

The checked-in configuration uses up to five detached Git worktrees, one per candidate evaluation. OpenCode calls and builds can therefore run concurrently without swapping source or object files in the shared workspace.

Each worktree:

- starts at `HEAD` and overlays the current tracked/untracked dirty snapshot;
- receives its own generated build and candidate objects;
- shares immutable retail extraction, split objects, generated includes, compilers, binutils, and tools;
- is removed after evaluation, including error paths.

Configure under `execution`:

```json
{
  "max_parallel": 5,
  "max_target_parallel": 2,
  "batch_model_parallel": 1,
  "pipelines": {
    "new": {
      "max_parallel": 5,
      "max_target_parallel": 3,
      "batch_model_parallel": 2
    },
    "improve": {
      "max_parallel": 1,
      "max_target_parallel": 1,
      "batch_model_parallel": 1
    },
    "tu-complete": {
      "max_parallel": 2,
      "max_target_parallel": 1,
      "batch_model_parallel": 1
    }
  },
  "max_retries": 1,
  "isolation": {"mode": "git-worktree", "copy_dirty": true}
}
```

`max_parallel` controls model/run concurrency for a single target. `max_target_parallel` controls active functions in `batch`; `batch_model_parallel` controls model concurrency inside each active function. Their product is the maximum batch provider/build concurrency. Override them with `batch --max-target-parallel N --model-parallel M`.

Entries under `execution.pipelines` override those global values for the named
workflow. Resolution order is CLI override, workflow override, global value,
then the built-in default of `1`. Set all three workflow values to `1` to make
that workflow sequential.

Parallel batch evaluation requires Git-worktree isolation; dry-run context generation may run in parallel without it. Use `--max-parallel 1` for single-target diagnosis. Isolation requires permission to create Git worktree metadata. Setting `mode` to `none` restores sequential source/object swapping.

## Artifacts and state

Outputs default to:

```text
build/llm-harness/
  experiments.jsonl
  batches/<batch-id>/batch.json
  <target-or-unit>/<experiment-id>/
    prompt.md
    context/
    <model-id>-<run>.json
    best.json
    state.json
```

- `prompt.md` is the exact model dossier.
- `context/` is empty for every workflow; `prompt.md` is the complete frozen model input.
- `batch.json` records each target's experiment path or independent failure.
- Each model artifact contains the typed candidate and evaluation.
- `best.json` is the winning candidate for that experiment.
- `experiments.jsonl` is append-only A/B history and is separate from the canonical decomp attempt log.
- `state.json` is atomically updated after every candidate and enables resume after interruption or budget exhaustion.

Experiment and promotion workflows do not mutate `tools/coop/targets.json` or `docs/evidence/decomp/attempts.jsonl`. The explicit `slot --write` command updates only the target’s source/unit assignment. After applying a function winner, use the normal claimed-target `coop cycle` workflow for authoritative logging and acceptance.

Preview or apply a winner with:

```bash
python3 tools/llm_harness/run.py promote build/llm-harness/<target>/<experiment-id>
python3 tools/llm_harness/run.py promote build/llm-harness/<target>/<experiment-id> \
  --write --owner <claim-owner>
```

Promotion is dry-run by default. Function writes require the matching canonical claim. TU writes require an owner and refuse targets claimed by someone else. Promotion changes source only; it never marks targets accepted or appends canonical evidence.

Re-run build/diff evaluation for saved candidates without another provider call:

```bash
python3 tools/llm_harness/run.py rescore build/llm-harness/<target>/<experiment-id>
```

Rescored records supersede the same experiment/model/run in `stats`; raw JSONL history remains append-only.

## MWCC knowledge retrieval

The Xenoblade adapter searches `docs/MWCC_REFERENCE.md` and canonical attempt history through the generated SQLite FTS index described in `docs/MWCC_KNOWLEDGE_BASE.md`.

For functions it searches exact function/symbol identity first, then mismatch-category tags inferred from prior feedback. TU completion also searches relocation, literal-pool, vtable, and size patterns. The prompt records requested queries, effective fallback mode, returned stable IDs, and capped full records. Tags and search rank are retrieval aids, not acceptance evidence.

Relevant configuration under `project`:

```json
{
  "mwcc_knowledge_enabled": true,
  "mwcc_knowledge_limit": 8,
  "mwcc_knowledge_body_chars": 5000
}
```

The disposable index under `build/` is rebuilt when its Markdown or JSONL sources are stale. Prompt creation snapshots only the selected queries and capped full records into `prompt.md`, so parallel agents receive stable target-specific knowledge without another context file.

## Current limitations and next work

- Budget enforcement occurs between calls; providers do not expose a reliable preflight cost.
- Resume reuses the original prompt. Start a new `improve` experiment when canonical source or KB evidence changes materially.
- A new marker slot assigns source/unit but does not generate declarations, configure entries, headers, or namespace structure; the exact anchor is intentionally user-selected.
- Promotion remains source-only and requires the normal `coop cycle` for authoritative acceptance.
- Function contexts include raw bytecode, relocations, and the named block from generated retail PPC assembly when available. Narrow type/caller/callee context remains a useful follow-up improvement.

## Adapter boundary

The generic harness imports the configured project module and expects:

- `root`;
- `build_prompt(workflow, target_id, history)`;
- `evaluate(workflow, target_id, candidate)`;
- `finalize()`;
- optional `prepare(target_id)`.
- optional `prepare_workspace(path)`, `create_slot(...)`, `promote(...)`, and `rank_candidate(...)`.

Providers implement `invoke(prompt, model, cwd)` and return text, duration, token counts, cost, and raw events. The generic core contains no Xenoblade commands or prompts.

# LLM decompilation harness

This package separates a generic experiment runner from project knowledge. The generic core imports the module named by `project_adapter`; Xenoblade prompts, byte extraction, build/diff commands, safety checks, and acceptance policy live in `xenoblade_project.py` and `prompts/`.

The Xenoblade adapter also queries the generated [MWCC knowledge base](../../docs/MWCC_KNOWLEDGE_BASE.md). It searches exact function identity first, derives mismatch-category tags from prior harness feedback, opens the best full records by stable ID, and embeds a capped, deduplicated set in the dossier. The SQLite index is rebuilt automatically when its Markdown or attempt-log sources are stale.

## Setup

Install and authenticate [OpenCode](https://opencode.ai/docs/cli/), then copy and edit the example config:

```bash
cp llm-harness.example.json llm-harness.json
opencode models
python3 tools/coop/run.py baseline
```

Use OpenCode model names in `provider/model` form. Keeping `agent: "plan"` makes each inference read-only; the harness itself applies a returned candidate only long enough to build and score it, then restores the source and prior object bytes.

The normal repository baseline/full build is a scoring prerequisite because objdiff's report generator reads every object listed in `objdiff.json`. If an unrelated object is absent, the attempt is retained with `EVALUATION_ERROR` instead of aborting the experiment.

## Workflows

```bash
# Create only the dossier/prompt, without calling an LLM or compiling.
python3 tools/llm_harness/run.py new pad-copy-input-flag --dry-run

# N independent samples per configured model; config `runs` is the default.
python3 tools/llm_harness/run.py new pad-copy-input-flag --runs 3

# Include the latest harness outcomes as feedback for another bounded attempt.
python3 tools/llm_harness/run.py improve pad-copy-input-flag --runs 3

python3 tools/llm_harness/run.py stats
```

The output directory contains the exact prompt, every typed candidate/evaluation, `best.json`, and a separate append-only `experiments.jsonl`. Improvement prompts receive compact hypotheses, notes, next changes, and scores from prior harness attempts. A winner is the accepted candidate first, then the highest function match percentage. `stats` reports attempts, successful calls, experiment wins, accepted wins, accepted candidates, average match, latency, tokens, cost (when OpenCode emits it), and errors per model.

Knowledge retrieval is configured under `project` with `mwcc_knowledge_enabled`, `mwcc_knowledge_limit`, and `mwcc_knowledge_body_chars`. The prompt records both the queries and returned knowledge IDs so an experiment remains auditable.

The harness does not promote a winner into the repository or mutate `targets.json`/the canonical attempt log. Review `best.json`, apply it deliberately, and run the normal claimed-target `coop cycle` workflow for authoritative acceptance.

## Adapter contract

A project module exports `create_adapter(root, settings)` returning an object with:

- `build_prompt(workflow, target_id, history)`
- `evaluate(target_id, candidate)`
- `finalize()`
- `root`, used as the provider working directory

That is the only project-specific interface. Additional providers can implement the same `invoke(prompt, model, cwd)` result contract used by `OpenCodeProvider`.

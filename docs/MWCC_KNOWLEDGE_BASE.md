# MWCC matching knowledge base

`docs/MWCC_REFERENCE.md` and `docs/evidence/decomp/attempts.jsonl` can be searched together through a generated SQLite FTS5 index. The index is a disposable cache under `build/`; the reviewable Markdown and JSONL files remain the sources of truth.

## Use it

```bash
python3 tools/mwcc_kb.py build
python3 tools/mwcc_kb.py search "SDA relocation mismatch"
python3 tools/mwcc_kb.py search "stack frame spill" --status CODE_MATCH
python3 tools/mwcc_kb.py search "Chaitin exact size" --kind attempt
python3 tools/mwcc_kb.py search "vtable slot" --tag vtable
python3 tools/mwcc_kb.py search "regalloc prologue spill" --mode any --json
python3 tools/mwcc_kb.py show <result-id>
python3 tools/mwcc_kb.py show <result-id> --json
python3 tools/mwcc_kb.py stats
python3 tools/mwcc_kb.py tags
```

Search, show, and stats automatically rebuild a stale or missing index. Results point back to their source file and line. Reference heading IDs survive unrelated line shifts (renaming a heading changes its ID); attempt IDs include the target and append-only JSONL line number.

The initial inferred taxonomy covers ABI, compiler flags, control flow, inlining, literal pools, loop codegen, policy exceptions, register allocation, relocations, semantics, size, stack frames, struct layout, switch codegen, and vtables. Tags are search aids, not acceptance evidence.

## Agent search protocol

Agents should search from observed evidence, not paste an entire function or ask one oversized natural-language question. For every matching target:

1. Inspect canonical identity and current state with `targets show <target-id>`.
2. Search the exact function name and mangled symbol. This retrieves same-function reference notes and prior attempts.
3. Classify the current objdiff gap, then issue one short query per mismatch category. Useful vocabulary includes `relocation SDA`, `regalloc Chaitin`, `stack frame spill`, `struct layout offset`, `switch case order`, `loop mtctr bdnz`, `literal pool sdata2`, `vtable slot`, `ABI Fv`, and `inlining IPA`.
4. Search `--kind reference` for proven transformations and `--kind attempt` for experiments already tried. Do not repeat failed attempts without a new reason.
5. Start with the default `--mode all`. If it returns no useful result, remove the least important term or use `--mode any`; then narrow with `--tag`, `--status`, or `--kind`.
6. Open the best few records with `show <id> --json`. Search snippets are discovery aids; the full record contains caveats, failed variants, and evidence.
7. Name the selected knowledge IDs in the attempt hypothesis and explain why their symptoms match the current diff. Apply one bounded experiment, then run `cycle`.

Example:

```bash
python3 tools/coop/run.py targets show pad-update
python3 tools/mwcc_kb.py search "update__Q22cf9CfPadTaskFv" --json
python3 tools/mwcc_kb.py search "relocation SDA float pool" --kind reference --tag relocation --json
python3 tools/mwcc_kb.py search "float pool frame regression" --kind attempt --mode any --json
python3 tools/mwcc_kb.py show <result-id> --json
```

For automation, use `--json`; index rebuild notices go to stderr, so stdout remains valid JSON. A zero-result search returns a JSON object with an empty `results` array and exit status 1.

## Why SQLite is generated, not canonical

- SQLite FTS gives fast ranked full-text queries and joins the reference with attempt history.
- A binary database is poor for code review, merge conflicts, and blame.
- Markdown remains pleasant to read, while JSONL remains appropriate for append-only attempts.
- Rebuilding prevents stale duplicated truth.

Do not commit `build/mwcc_knowledge.sqlite`.

## Recommended end state

The current parser is a bridge. The long-term source of truth should be one small structured record per reusable pattern, with Markdown generated from those records and SQLite generated for querying.

Suggested layout:

```text
docs/mwcc/
  concepts/                 # stable compiler/ABI explanations
  patterns/
    reloc-retail-sda-name.json
    regalloc-block-scopes.json
  schema.json
docs/MWCC_REFERENCE.md       # generated human-readable handbook
build/mwcc_knowledge.sqlite  # generated search index
```

One-file-per-pattern is preferred over one large JSON/YAML file because concurrent matching work then produces smaller merge conflicts. JSON works with the Python standard library and permits strict schema validation. If authoring comfort becomes more important than zero dependencies, YAML can be added later without changing the logical schema.

Suggested pattern record:

```json
{
  "id": "reloc-retail-sda-name",
  "title": "Use the retail SDA symbol name",
  "kind": "transformation",
  "symptoms": ["instructions match but relocation names differ", "lwz or lfs uses @sda21"],
  "causes": ["source references a mangled singleton or anonymous pool instead of the retail label"],
  "actions": ["declare the retail label with C linkage", "use it directly at the confirmed access site"],
  "avoid": ["blanket replacement of unrelated literals", "objdiff UI-only symbol mappings"],
  "tags": ["relocation", "sda", "linkage"],
  "compiler": {"family": "MWCC", "platform": "Wii/1.1"},
  "evidence": [
    {
      "target_id": "pad-update-cfpad-data",
      "function": "cf::CfPadTask::updateCfPadData",
      "source": "src/kyoshin/cf/CfPadTask.cpp",
      "before_percent": 99.95,
      "after_status": "FULL_MATCH",
      "after_percent": 100.0
    }
  ],
  "confidence": "repo_proven",
  "policy_exception": false
}
```

Important field distinctions:

- `symptoms` describe what objdiff shows; `causes` are explanations; `actions` are bounded experiments.
- `evidence` points to target IDs and source paths rather than duplicating current target state.
- `confidence` should distinguish compiler documentation, repo-proven patterns, hypotheses, and negative results.
- `policy_exception` prevents ordinary C++ advice from being mixed with §17.6 escape hatches.
- Current status remains canonical in `tools/coop/targets.json`; attempts remain append-only history.

## Migration sequence

1. Use the generated index immediately and refine searches/tags from real matching work.
2. Add a JSON Schema and a `validate` command for structured records.
3. Migrate the highest-value families first: relocation/SDA, ABI/linkage, struct layout, control-flow lowering, register allocation, literal pools, and policy exceptions.
4. Generate the handbook from structured records, preserving a short hand-authored concepts section.
5. Add `suggest <target-id>`: combine the target record, recent failed hypotheses, objdiff mismatch categories, and ranked patterns to propose the next bounded experiment.
6. Add feedback fields to attempt logging (`pattern_ids_tried`, `pattern_id_succeeded`) so pattern effectiveness can be measured instead of inferred from prose.

The most valuable future feature is not a generic chatbot over the file. It is a deterministic recommender that can answer: “Given this target's mismatch category, compiler flags, current percentage, size result, and already-tried hypotheses, which repo-proven experiment has the strongest evidence and has not yet been tried?”

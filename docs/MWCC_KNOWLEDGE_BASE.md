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
4. Search `--kind reference` for proven transformations, `--kind attempt` for experiments already tried, and `--kind kb_contribution` for agent-contributed patterns. Do not repeat failed attempts without a new reason.
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

## Contributing to the KB (`add_to_kb`)

When an agent discovers a reusable pattern (especially when closing the last few percent of a match), it should contribute it to the knowledge base so future sessions can find it. There are two paths:

### 1. During a `cycle` (inline)

```bash
python3 tools/coop/run.py cycle <target-id> \
    --hypothesis "retail uses lbl_eu_80667EB0 not @N" \
    --next-change "extern the float label" \
    --add-to-kb '{"title":"Direct extern SDA float in a compare",
      "symptoms":["instructions byte-identical but reloc names differ","lfs from @N not lbl_eu_*"],
      "fix":"extern \"C\" const float lbl_eu_*; use verbatim at each site",
      "tags":["relocation","literal_pool"],
      "target_id":"<target-id>","function":"<qualified-name>"}'
```

The contribution is appended to `docs/mwcc/contributions.jsonl` alongside the attempt log.

### 2. Standalone (any agent, any time)

```bash
python3 tools/mwcc_kb.py add '{"title":"...", "symptoms":[...], "fix":"...", "tags":[...]}'
python3 tools/mwcc_kb.py add /path/to/payload.json --from-file
```

This appends to `docs/mwcc/contributions.jsonl` **and** immediately rebuilds the SQLite index — so the contribution is searchable by all subsequent `mwcc_kb.py search` calls within the same session. (It does **not** appear in prompts of already-running agents; their context was frozen at session start.)

### Contribution record format

A minimal record needs `title`. A useful record includes:

| Field | Type | Description |
|-------|------|-------------|
| `title` | string | **Required.** One-line summary of the pattern. |
| `symptoms` | string[] | What objdiff shows (e.g. `"instructions match but relocation names differ"`). |
| `fix` | string | The high-level C++ change that closed the gap. |
| `tags` | string[] | One or more from: `abi`, `compiler_flags`, `control_flow`, `inlining`, `literal_pool`, `loop_codegen`, `policy_exception`, `regalloc`, `relocation`, `semantics`, `size`, `stack_frame`, `struct_layout`, `switch_codegen`, `vtable`. |
| `target_id` | string | The target this was discovered on (for traceability). |
| `function` | string | Qualified function name. |
| `notes` | string | Caveats, pitfalls, or when NOT to use this pattern. |
| `confidence` | string | `repo_proven`, `hypothesis`, or `negative_result`. |

Records are stored as JSONL (one JSON object per line) in `docs/mwcc/contributions.jsonl`. The SQLite index picks them up with `source_kind=kb_contribution`.

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

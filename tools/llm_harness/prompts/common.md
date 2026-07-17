You are reconstructing one Xenoblade Chronicles Wii function for a private downstream fork.

{{WORKFLOW_PROMPT}}

Hard constraints:

- Return readable, high-level C or C++ only. Retail bytecode is read-only evidence.
- Do not emit whole-function assembly, register bindings/names, fake stack buffers, or control flow copied mechanically from assembly.
- Preserve unrelated functions and formatting in the translation unit.
- Function candidates are scored locally: either 100% static match, or at least 50% fuzzy plus a proved effect-aware PPC equivalence, and the generated function must not exceed the retail function's size. Whole-object split-size fit is deferred to the TU-completion workflow.
- This function prompt is self-contained. Do not read, edit, or search files and do not run shell commands.
- `source` must contain exactly one complete replacement function definition, without markers or Markdown fences. Do not return the rest of the translation unit.
- Treat MWCC knowledge tags and search rank as retrieval aids, not acceptance evidence. Use a retrieved record only when its symptoms fit this target.
- Name any selected MWCC knowledge IDs in `hypothesis`, and do not repeat a failed attempt record without explaining the new reason.

Return exactly one JSON object of this shape:

```json
{
  "source": "complete replacement function definition",
  "hypothesis": "the single main reconstruction or mismatch hypothesis",
  "notes": ["short evidence or uncertainty note"],
  "next_change": "one bounded follow-up if this candidate does not win",
  "confidence": 0.0
}
```

Function dossier:

```json
{{DOSSIER_JSON}}
```

Current target function:

```cpp
{{CURRENT_FUNCTION}}
```

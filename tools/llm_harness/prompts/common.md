You are reconstructing one Xenoblade Chronicles Wii function for a private downstream fork.

{{WORKFLOW_PROMPT}}

Hard constraints:

- Return readable, high-level C or C++ only. Retail bytecode is read-only evidence.
- Do not emit whole-function assembly, register bindings/names, fake stack buffers, or control flow copied mechanically from assembly.
- Preserve unrelated functions and formatting in the translation unit.
- The acceptance bar is either 100% static match with split-size fit, or at least 50% fuzzy plus a proved effect-aware PPC equivalence and split-size fit.
- Do not use tools or edit repository files. Reason from this self-contained dossier.
- `full_source` must contain the complete replacement contents of the current source file, without Markdown fences.
- Treat MWCC knowledge tags and search rank as retrieval aids, not acceptance evidence. Use a retrieved record only when its symptoms fit this target.
- Name any selected MWCC knowledge IDs in `hypothesis`, and do not repeat a failed attempt record without explaining the new reason.

Return exactly one JSON object of this shape:

```json
{
  "full_source": "complete replacement source file",
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

Current complete source file:

```cpp
{{CURRENT_SOURCE}}
```

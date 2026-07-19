You are completing one Xenoblade Chronicles Wii translation unit after every reported function has at least one nonzero match.

This is the full-context fallback. Focus on remaining whole-unit gaps: static data, vtables, literal/string pools, relocations, declaration/linkage, ordering. Preserve already matched functions unless a shared layout issue requires a bounded change.

Approach:
1. Use code_match_percent / data_match_percent plus residual_functions / residual_sections.
2. Reorder declarations when MWCC layout order is wrong; fix float/string pool contents exactly.
3. Map code deltas like `improve` (signedness, widths, expression order, branches).
4. Fix relocation name/mangling mismatches with `extern "C"` where needed.
5. Touch FULL_MATCH / EQUIVALENT_MATCH functions only for unavoidable cross-cutting layout fixes.

{{TU_SHARED}}
- `source` must be the complete replacement translation-unit source without Markdown fences.

Return exactly one JSON object. No preamble, no commentary, no Markdown fences:

```json
{
  "source": "complete replacement translation-unit source",
  "hypothesis": "one primary whole-unit mismatch hypothesis",
  "notes": ["short evidence or uncertainty note"],
  "next_change": "one bounded follow-up if this candidate does not win"
}
```

Unit dossier:

```json
{{DOSSIER_JSON}}
```

Current complete translation unit:

```cpp
{{CURRENT_SOURCE}}
```

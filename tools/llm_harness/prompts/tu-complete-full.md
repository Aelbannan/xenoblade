You are completing one Xenoblade Chronicles Wii translation unit in a private downstream fork after every reported function has received at least one nonzero match.

This is the explicit full-context fallback. Focus only on remaining whole-unit issues: static data, vtables, literal/string pools, relocations, declaration/linkage details, function or data ordering, and normal high-level C/C++ changes that close residual code/data differences. Preserve already matched functions unless a whole-unit interaction requires a bounded change.

Hard constraints:

- Return readable high-level C or C++ only; no whole-function assembly, register bindings, fake stack buffers, or retail assembly transcription.
- Unit acceptance requires 100% code and data plus split-size fit. Function-level equivalence is not a unit-wide proof.
- Use retrieved MWCC records only when their symptoms fit, and name selected stable knowledge IDs in `hypothesis`.
- Do not use tools or edit files.
- `source` must contain the complete replacement translation-unit source without Markdown fences.

Return exactly one JSON object:

```json
{
  "source": "complete replacement translation-unit source",
  "hypothesis": "one primary whole-unit mismatch hypothesis",
  "notes": ["short evidence or uncertainty note"],
  "next_change": "one bounded follow-up if this candidate does not win",
  "confidence": 0.0
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

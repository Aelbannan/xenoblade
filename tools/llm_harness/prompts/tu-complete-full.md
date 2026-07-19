You are completing one Xenoblade Chronicles Wii translation unit in a private downstream fork after every reported function has received at least one nonzero match.

This is the explicit full-context fallback. Focus only on remaining whole-unit issues: static data, vtables, literal/string pools, relocations, declaration/linkage details, function or data ordering, and normal high-level C/C++ changes that close residual code/data differences. Preserve already matched functions unless a whole-unit interaction requires a bounded change.

Before editing, reason systematically:

1. **Identify the gap**: The dossier shows code_match_percent and data_match_percent. If code < 100%, examine the residual_functions evidence for bytecode deltas on imperfect functions. If data < 100%, examine residual_sections for mismatched static data, vtables, or literal pools.
2. **For data mismatches**: Check whether static variable initializers, vtable entries, string contents, or floating-point constant pools differ. MWCC lays these out in declaration order — reorder declarations to match retail.
3. **For code mismatches**: Apply the same root-cause analysis as `improve` — map instruction-word deltas to signed/unsigned types, expression ordering, load/store widths, or branch shapes.
4. **For relocation mismatches**: If the candidate references a different symbol than retail (relocations section in evidence), the function or data declaration uses the wrong name, mangling, or section attribute. Add `extern "C"` where needed.
5. **Preserve accepted functions**: The regression guard rejects patches that regress previously accepted functions. Do not touch functions that are already at FULL_MATCH or EQUIVALENT_MATCH unless a cross-cutting issue (like a shared vtable or static data reordering) requires it. If you must modify an accepted function, make the smallest possible change.

MWCC compiler hints:

- Static data ordering matches declaration order. Move declarations around to match retail layout.
- Vtable entries: virtual methods appear in declaration order. If a slot is wrong, the class's virtual method order differs from retail.
- String literals: placed in `.rodata` or `.sdata2`. Exact contents (including null terminator) must match.
- Floating-point constants: `lfs`/`lfd` loads go to a literal pool. Ensure `1.0f` vs `1.0` matches (float vs double).
- Function ordering: MWCC emits functions in source order. If functions appear in the wrong order in .text, reorder them in the source file.

Hard constraints:

- Return readable high-level C or C++ only; no whole-function assembly, register bindings, fake stack buffers, or retail assembly transcription.
- Unit acceptance requires 100% code and data plus split-size fit. Function-level equivalence is not a unit-wide proof.
- This prompt is self-contained. Do not read, edit, or search files and do not run shell commands.
- `source` must contain the complete replacement translation-unit source without Markdown fences.

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
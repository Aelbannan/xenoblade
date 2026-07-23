Produce a conservative decompilation for the named function (local/scaffold mode).

Hard rules for this workflow:
1. Lock the function name, linkage, and parameter list from `{{CURRENT_FUNCTION}}` / the dossier signature unless retail ASM proves a different arity/return width — then change only those ABI fields.
2. Prefer returning a complete single function definition. Body-only `{ … }` is accepted; the harness will reattach the locked signature.
3. Do **not** emit forward declarations, `namespace` / `class` wrappers, or extra helper definitions.
4. Do **not** use mangled names as C identifiers (`func_8049B3FC__Fv` → `func_8049B3FC`). Call relocation symbols by their flat/`extern "C"` names from the dossier.
5. Never name a parameter `this` — use `this_` / `self` in free-function form.
6. Do not invent unavailable types/members. Prefer fields and callees already evidenced by ASM relocations or dossier snippets.
7. Flat `lbl_eu_*` data via ADDR16 needs `extern "C"` only when the symbol is not already declared in the TU — the harness strips redundant externs.

Walk `retail_asm` for calls, loads/stores, branches, and immediates. Ignore prologue shape.

Return only the target function (or its body). No Markdown fences.

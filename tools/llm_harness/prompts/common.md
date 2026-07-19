You are reconstructing one Xenoblade Chronicles Wii function for a private downstream fork.

Before writing your response, reason systematically about the function. Work through these steps:

1. **Symbol & purpose**: What does the function name and its class/namespace suggest about what it does?
2. **Bytecode scan**: Scan the retail bytecode for call instructions, branches, loads/stores, and immediate values. Identify the control flow shape (straight line, if/else, loop, switch, tail call).
3. **Relocation binding**: Map each relocation to the named symbol it imports. Calls to known functions reveal the operations performed. Data relocations reveal global accesses.
4. **Type inference**: Infer parameter types, return type, and local types from how values are used (e.g., byte loads -> u8, halfword -> u16, floating-point ops -> float/double, pointer arithmetic -> typed pointer).
5. **Accepted sibling patterns**: If the dossier includes accepted functions from the same unit, match their coding style, naming, and type conventions.

Hard constraints:

- Return readable, high-level C or C++ only. Retail bytecode is read-only evidence.
- Do not emit whole-function assembly, register bindings/names, fake stack buffers, or control flow copied mechanically from assembly.
- Preserve unrelated functions and formatting in the translation unit.
- Function candidates are scored locally: either 100% static match, or at least 50% fuzzy plus a proved effect-aware PPC equivalence, and the generated function must not exceed the retail function's size. Whole-object split-size fit is deferred to the TU-completion workflow.
- This function prompt is self-contained. Do not read, edit, or search files and do not run shell commands.
- `source` must contain exactly one complete replacement function definition, without markers or Markdown fences. Do not return the rest of the translation unit.

Output format — return EXACTLY one raw JSON object on a single logical line. No preamble, no commentary, no Markdown fences, no text before or after:

{"source": "complete replacement function definition", "hypothesis": "the single main reconstruction or mismatch hypothesis", "notes": ["short evidence or uncertainty note"], "next_change": "one bounded follow-up if this candidate does not win"}

---

{{WORKFLOW_PROMPT}}

Function dossier:

```json
{{DOSSIER_JSON}}
```

Current target function:

```cpp
{{CURRENT_FUNCTION}}
```
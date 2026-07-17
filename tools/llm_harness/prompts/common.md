You are reconstructing one Xenoblade Chronicles Wii function for a private downstream fork.

{{WORKFLOW_PROMPT}}

Before writing your response, reason systematically about the function. Work through these steps:

1. **Symbol & purpose**: What does the function name and its class/namespace suggest about what it does?
2. **Bytecode scan**: Scan the retail bytecode for call instructions, branches, loads/stores, and immediate values. Identify the control flow shape (straight line, if/else, loop, switch, tail call).
3. **Relocation binding**: Map each relocation to the named symbol it imports. Calls to known functions reveal the operations performed. Data relocations reveal global accesses.
4. **Type inference**: Infer parameter types, return type, and local types from how values are used (e.g., byte loads -> u8, halfword -> u16, floating-point ops -> float/double, pointer arithmetic -> typed pointer).
5. **Accepted sibling patterns**: If the dossier includes accepted functions from the same unit, match their coding style, naming, and type conventions.
6. **MWCC knowledge**: If the dossier includes MWCC knowledge records, check whether their symptoms (mismatch categories, instruction patterns) describe problems relevant to this function. Only use a record when it directly informs your change.

MWCC compiler hints (Metrowerks CodeWarrior for PPC, -O4,s):

- Signed comparisons of unsigned values: MWCC often uses unsigned loads (lbz, lhz, lwz) even when the source uses a signed type. The compiler may optimize a signed comparison by loading as unsigned and comparing differently. Prefer the semantically correct type; do not force signed loads.
- Expression order: MWCC reorders subexpressions aggressively. If the candidate matches in logic but not bytes, break complex expressions into named temporaries or reorder terms.
- Branch lowering: MWCC may lower `if (x > 0)` as `cmpwi cr0, rX, 0; ble target` (signed) or `cmplwi cr0, rX, 0; beq target` (unsigned). Match the source type, not the branch direction.
- Switch lowering: MWCC generates binary-search or jump-table patterns. Use a natural `switch` statement; do not replicate the lowering mechanically.
- Inline helpers: MWCC sometimes inlines simple getters or arithmetic. If a small called pattern appears mechanically inlined, consider pulling it out as a private helper or leaving it inline — both are valid high-level C++.

Hard constraints:

- Return readable, high-level C or C++ only. Retail bytecode is read-only evidence.
- Do not emit whole-function assembly, register bindings/names, fake stack buffers, or control flow copied mechanically from assembly.
- Preserve unrelated functions and formatting in the translation unit.
- Function candidates are scored locally: either 100% static match, or at least 50% fuzzy plus a proved effect-aware PPC equivalence, and the generated function must not exceed the retail function's size. Whole-object split-size fit is deferred to the TU-completion workflow.
- This function prompt is self-contained. Do not read, edit, or search files and do not run shell commands.
- `source` must contain exactly one complete replacement function definition, without markers or Markdown fences. Do not return the rest of the translation unit.
- Treat MWCC knowledge IDs as retrieval aids, not acceptance evidence. Use a record only when its symptoms fit this target.
- Name any selected MWCC knowledge IDs in `hypothesis`, and do not repeat a failed attempt record without explaining the new reason.
- bytecode_hex and bytes_hex are contiguous lowercase hex without separators; group every 8 chars (4 bytes) for PPC instruction decoding.

Output format — return EXACTLY one JSON object. No preamble, no commentary, no Markdown fences around the JSON, no text before or after:

```json
{
  "source": "complete replacement function definition",
  "hypothesis": "the single main reconstruction or mismatch hypothesis",
  "notes": ["short evidence or uncertainty note"],
  "next_change": "one bounded follow-up if this candidate does not win",
  "confidence": 0.0
}
```

The frozen MWCC knowledge base below is byte-identical across every prompt in this campaign: every reference and attempt record is inlined once so its body lives in the cacheable prompt prefix. Reference entries by their stable IDs only; do not echo bodies back.

{{FROZEN_KB}}

Function dossier:

```json
{{DOSSIER_JSON}}
```

Current target function:

```cpp
{{CURRENT_FUNCTION}}
```

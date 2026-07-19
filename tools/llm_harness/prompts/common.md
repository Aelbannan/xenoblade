You are matching one Xenoblade Wii function compiled by MWCC for PPC.

Hard constraints:
- Return exactly one JSON object matching the requested schema.
- Return high-level C/C++ only; no inline assembly.
- Preserve the declared ABI, signature, linkage, and observable effects.
- Use the retail ASM listing (`retail_asm`) and relocation symbols as evidence.
- Do not invent unavailable declarations or symbols.
- Optimize for MWCC-generated instruction and relocation matching, not style.
- Function candidates are scored locally: either 100% static match, or at least 50% fuzzy plus a proved effect-aware PPC equivalence. Per-function size delta is diagnostic only; containing-object split-size fit is enforced at promotion/TU completion.
- This function prompt is self-contained. Do not read, edit, or search files and do not run shell commands.
- `source` must contain exactly one complete replacement function definition, without markers or Markdown fences. Do not return the rest of the translation unit.

Output format — return EXACTLY one compact raw JSON object. No preamble, no commentary, no Markdown fences, no chain-of-thought, no text before or after.

Keep metadata tiny:
- `hypothesis`: ≤160 characters
- `notes`: ≤3 strings, each ≤120 characters
- `next_change` / `change`: ≤120 characters each (`change` empty for first candidates)

Do not restate the ASM, do not narrate step-by-step reasoning, and do not pad fields.

{"source":"complete replacement function definition","hypothesis":"short hypothesis","notes":["short note"],"next_change":"short follow-up or empty","change":""}

---

{{WORKFLOW_PROMPT}}

Function dossier:

```json
{{DOSSIER_JSON}}
```

Current / editable function:

```cpp
{{CURRENT_FUNCTION}}
```

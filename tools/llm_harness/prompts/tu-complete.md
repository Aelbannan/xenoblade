You are completing residual sections of one Xenoblade Chronicles Wii translation unit in a private downstream fork after every reported function has received at least one nonzero match.

You receive only bounded source slots (automatically located imperfect functions and/or explicit TU markers) plus mismatched retail/candidate ELF section bytes and relocations. Patch the smallest relevant slots. The harness will splice the patches into the complete source and validate the complete object.

Before patching, reason systematically:

1. **Identify residual mismatches**: The dossier shows code_match_percent and data_match_percent. If code < 100%, focus on function slots. If data < 100%, focus on static data, vtables, literal pools, or string pools shown in residual_sections.
2. **For function slots**: Apply the same mismatch analysis as `improve` — examine bytecode deltas and map them to C++ root causes (signed/unsigned, expression order, type width, missing side effects).
3. **For static data / residual sections**: The residual_sections show raw hex bytes and relocations for mismatched non-.text sections. Check whether:
   - Literal pool constants match (floating-point values, string data).
   - Vtable entries match (function pointer ordering).
   - Static initializer data matches (global variable initial values).
   - BSS layout matches (uninitialized globals — zero-sized but may affect alignment).
4. **For residual text layout**: The residual_text_layout shows non-function gaps in .text with raw bytes and relocations. These are typically:
   - Switch jump tables (arrays of branch targets).
   - Constant pools (floating-point or integer constants referenced by functions).
   - Padding between functions.
   If these don't match, verify that the source produces the correct layout ordering.
5. **Patch minimally**: Only modify slots whose content is actually mismatched. Do not touch slots that are already correct. The regression guard will reject patches that break previously accepted functions, so be extra careful with function slots that are already at 100%.
6. **For declaration/linkage issues**: If relocations differ (retail-only or candidate-only symbols), check whether a symbol needs `extern "C"` linkage, a different mangling, or belongs in a different section.

MWCC compiler hints:

- Static data ordering: MWCC places static/global variables in declaration order. If bytes differ because of ordering, reorder the declarations in the source slot.
- Literal pools: `lfs`/`lfd` instructions load from a literal pool near the function. Ensure floating-point literals match exactly (e.g., `1.0f` not `1.0`).
- Vtables: Virtual function table entries must appear in the order the class declares virtual methods. If a vtable slot contains the wrong function pointer, the declaration order differs from retail.
- String pools: String literals are placed in `.rodata` or `.sdata2`. Ensure string contents match exactly (including null terminators).

Hard constraints:

- Return readable high-level C or C++ only; no whole-function assembly, register bindings, fake stack buffers, or retail assembly transcription.
- Unit acceptance requires 100% code and data plus split-size fit. Function-level equivalence is not a unit-wide proof.
- This prompt is self-contained. Do not read, edit, or search files and do not run shell commands.
- Return only known `slot_id` values from the dossier.
- Each patch `source` replaces only the contents between that slot's markers; do not repeat the marker comments.
- Do not return a complete translation unit.

Return exactly one JSON object. No preamble, no commentary, no Markdown fences:

```json
{
  "patches": [
    {
      "slot_id": "one slot ID from the dossier",
      "source": "bounded replacement high-level C or C++"
    }
  ],
  "hypothesis": "one primary whole-unit mismatch hypothesis",
  "notes": ["short evidence or uncertainty note"],
  "next_change": "one bounded follow-up if this candidate does not win"
}
```

Unit dossier:

```json
{{DOSSIER_JSON}}
```
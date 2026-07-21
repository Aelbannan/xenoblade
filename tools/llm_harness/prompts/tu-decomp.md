You are decompiling an entire Xenoblade Chronicles Wii translation unit (TU) compiled by MWCC for PowerPC.

HIGH-LEVEL GOAL
- Produce complete, high-level C/C++ source for the TU that achieves 100% instruction match (FULL_MATCH) or provable equivalence (EQUIVALENT_MATCH with certificate).
- Output: an improved header (Phase 1) + complete .cpp body (Phase 2) in a SINGLE response.

HARD CONSTRAINTS
- Return EXACTLY ONE JSON object. No preamble, no commentary, no Markdown fences.
- High-level C/C++ only; NO inline assembly, NO register bindings, NO retail transcription.
- When header and retail ASM conflict: TRUST THE ASM (returns in r3/f1, r3 as this, param count from register use).
- MWCC layout rules: static/global/vtable order follows declaration order; float literals must match width (1.0f vs 1.0); strings must match exactly; function order in .text follows source order.
- Relocation/linkage mismatches often need `extern "C"` for flat `lbl_eu_*` symbols.
- Per-function size delta is diagnostic only; final containing-object split-size fit is enforced at TU promotion.

TWO-PHASE OUTPUT FORMAT (single JSON object)
```json
{
  "phase1_header": "complete improved .hpp content (may be unchanged if already high quality)",
  "phase2_cpp": "complete .cpp source for the TU — every function body filled",
  "hypothesis": "one primary whole-TU mismatch hypothesis (≤160 chars)",
  "notes": ["short evidence or uncertainty note", "..."],
  "next_change": "one bounded follow-up if this candidate does not win (≤120 chars)"
}
```

PHASE 1 — HEADER IMPROVEMENT GUIDELINES
- You see ALL methods in the TU simultaneously. Use cross-method access patterns to infer field names/types consistently.
- Only rename when HIGHLY confident; keep `unk_*` with a `//` comment when uncertain.
- Fix obvious type mismatches: `lwz` → pointer/ref, `lfs` → float, `lbz` → int8_t/bool, `stfs` → float store.
- Array dimensions from loop bounds / indexed access across methods.
- vtable layout / inheritance from `this` adjustments and virtual call patterns.
- Preserve existing field order and padding — MWCC follows declaration order.

PHASE 2 — COMPLETE .CPP GUIDELINES
- Every function slot from the dossier gets a body. Matched functions: keep their working body. Unmatched: write from scratch using retail_asm.
- Use matched_siblings as few-shot style exemplars — match their coding style, naming, structure.
- Callee summaries tell you what helpers do; call them correctly.
- Struct context shows class layouts from other TUs; maintain consistency.
- For each unmatched function: walk its retail_asm, map to high-level C++ constructs (loops, conditionals, calls, member access).
- **Output clean C++ source only.** Do NOT include `// LLM-HARNESS-BEGIN`, `// LLM-HARNESS-END`, `// LLM-HARNESS-TU-BEGIN`, or `// LLM-HARNESS-TU-END` markers — harness slot handling is automated.
- Keep `hypothesis` focused on the ONE biggest structural mismatch class (e.g., "vtable layout mismatch in CfPadTask", "float literal width in 3 functions", "missing extern \"C\" on 5 lbl_eu_* refs").
- `notes`: ≤3 short strings (≤120 chars each) for uncertainties or evidence.
- `next_change`: one concrete, bounded follow-up action.

UNIT DOSSIER (provided as {{DOSSIER_JSON}}):
- functions[]: every function in TU with retail_asm, current_source, status, callees, callers
- matched_siblings[]: FULL_MATCH/EQUIVALENT_MATCH bodies for style reference
- callee_summaries[]: purpose + signature of called functions
- struct_context: class/struct definitions from matched TUs
- constraints: output token budget, high_level_cpp_only, trust_asm_over_header, consistent_layout_across_functions

Return ONLY the JSON object described above.
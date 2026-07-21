You are decompiling an entire Xenoblade Chronicles Wii translation unit (TU) compiled by MWCC for PowerPC.

HIGH-LEVEL GOAL
- Produce complete, high-level C/C++ source for the TU that achieves 100% instruction match (FULL_MATCH) or provable equivalence (EQUIVALENT_MATCH with certificate).
- Output: an improved header (Phase 1) + complete .cpp body (Phase 2) in a SINGLE response.

HARD CONSTRAINTS
- Return EXACTLY the section format below. No preamble, no commentary, no Markdown fences around the whole reply.
- High-level C/C++ only; NO inline assembly, NO register bindings, NO retail transcription.
- When header and retail ASM conflict: TRUST THE ASM (returns in r3/f1, r3 as this, param count from register use).
- MWCC layout rules: static/global/vtable order follows declaration order; float literals must match width (1.0f vs 1.0); strings must match exactly; function order in .text follows source order.
- Relocation/linkage mismatches often need `extern "C"` for flat `lbl_eu_*` symbols.
- Per-function size delta is diagnostic only; final containing-object split-size fit is enforced at TU promotion.
- This prompt is self-contained. Do not read, edit, search files, or run shell commands.
- Do NOT use a single-function `source` field. Do NOT return a file path.

TWO-PHASE OUTPUT FORMAT (plain sections — real newlines, no JSON escaping)
- Paste header and .cpp as raw text between the markers. Do not wrap them in JSON strings.
- `phase2_cpp` must be the full .cpp text with real function bodies (not stubs or commentary).

===PHASE1_HEADER===
<complete improved .hpp content; may be unchanged if already high quality>
===PHASE2_CPP===
<complete .cpp source for the TU — every function body filled>
===HYPOTHESIS===
<one primary whole-TU mismatch hypothesis (≤160 chars)>
===NOTES===
<one short note per line; ≤3 lines; ≤120 chars each>
===NEXT_CHANGE===
<one bounded follow-up if this candidate does not win (≤120 chars)>

PHASE 1 — HEADER IMPROVEMENT GUIDELINES
- You see ALL methods in the TU simultaneously. Use cross-method access patterns to infer field names/types consistently.
- Only rename when HIGHLY confident; keep `unk_*` with a `//` comment when uncertain.
- Fix obvious type mismatches: `lwz` → pointer/ref, `lfs` → float, `lbz` → int8_t/bool, `stfs` → float store.
- Array dimensions from loop bounds / indexed access across methods.
- vtable layout / inheritance from `this` adjustments and virtual call patterns.
- Preserve existing field order and padding — MWCC follows declaration order.

PHASE 2 — COMPLETE .CPP GUIDELINES
- Every function slot from the dossier gets a body. Matched functions: keep their working body. Unmatched: write from scratch using retail_asm.
- Use matched_siblings as few-shot style exemplars when present — match their coding style, naming, structure.
- Callee summaries / struct_context when present: call helpers correctly and keep layouts consistent.
- For each unmatched function: walk its retail_asm, map to high-level C++ constructs (loops, conditionals, calls, member access).
- Output clean C++ source only. Do NOT include `// LLM-HARNESS-BEGIN`, `// LLM-HARNESS-END`, `// LLM-HARNESS-TU-BEGIN`, or `// LLM-HARNESS-TU-END` markers — harness slot handling is automated.
- Keep hypothesis focused on the ONE biggest structural mismatch class.
- Notes: ≤3 short lines for uncertainties or evidence.
- Next change: one concrete, bounded follow-up action.

UNIT DOSSIER:

{{DOSSIER_TEXT}}
